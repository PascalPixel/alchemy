//! An edition's linker script, derived from its game's, as pret links every
//! version through one `ld_script`. The game's `MAIN.LD` gives the memory
//! map, the output sections in their order and every assignment. An
//! edition's `recon/<game>/<lang>/MAIN.LD` gives only the bodies of the
//! output sections it places differently: the game's own lines it links, in
//! the game's order, and between them its scaffold of base-ROM ranges.
use crate::targets::DecompTarget;
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::ops::Range;
use std::path::{Path, PathBuf};

/// The script `target` links with: the game's `MAIN.LD` for Japanese, or for
/// another edition the game's script with the edition's bodies, written
/// beside the build as pret writes its preprocessed `ld_script.ld`.
pub(crate) fn script(root: &Path, target: DecompTarget, output: &Path) -> Result<PathBuf, String> {
    let game = target.script();
    let Some(edition) = target.edition_script() else {
        return Ok(game);
    };
    let read = |path: &Path| {
        fs::read_to_string(root.join(path)).map_err(|error| format!("{}: {error}", path.display()))
    };
    let own = edition
        .parent()
        .expect("an edition script has a directory")
        .to_string_lossy()
        .into_owned();
    // English declares its native physical composition explicitly. Its object
    // order also admits the shared international source objects, including
    // those not reconstructed in Japanese yet. The other international
    // editions retain the same subsequence and whole-object checks as before.
    // These are source declarations; no reference image or report selects them.
    let product = target.id.as_str().split_once('-').unwrap().0;
    let international_path = Path::new("recon").join(product).join("en/MAIN.LD");
    let international = native(&read(&game)?, &read(&international_path)?)
        .map_err(|error| format!("{}: {error}", international_path.display()))?;
    let text = if edition == international_path {
        international
    } else {
        derive(&international, &read(&edition)?, &format!("\"*/{own}/"))
            .map_err(|error| format!("{}: {error}", edition.display()))?
    };
    let path = root.join(output).join(format!("{}.ld", target.id));
    fs::create_dir_all(root.join(output)).map_err(|error| error.to_string())?;
    if fs::read_to_string(&path).ok().as_deref() != Some(text.as_str()) {
        fs::write(&path, text).map_err(|error| format!("{}: {error}", path.display()))?;
    }
    Ok(path)
}

/// One brace group at a level of a linker script: the line before its `{`,
/// such as `.rom_main :`, and the text between its braces.
#[derive(Debug)]
struct Group {
    header: String,
    body: Range<usize>,
}

impl Group {
    /// The output section's name: the header's first word.
    fn name(&self) -> &str {
        self.header
            .split(|c: char| c.is_whitespace() || c == ':')
            .next()
            .unwrap_or("")
    }
}

/// The brace groups directly inside `range` of `text`, skipping comments
/// and quoted names.
fn groups(text: &str, range: Range<usize>) -> Result<Vec<Group>, String> {
    let bytes = text.as_bytes();
    let mut found = Vec::new();
    let mut depth = 0usize;
    let mut statement = String::new();
    let mut open = 0;
    let mut at = range.start;
    while at < range.end {
        let byte = bytes[at];
        if text[at..].starts_with("/*") {
            let end = text[at + 2..range.end]
                .find("*/")
                .ok_or("unterminated comment")?;
            at += end + 4;
            continue;
        }
        if byte == b'"' {
            let end = text[at + 1..range.end]
                .find('"')
                .ok_or("unterminated quoted name")?;
            if depth == 0 {
                statement.push_str(&text[at..at + end + 2]);
            }
            at += end + 2;
            continue;
        }
        match byte {
            b'{' => {
                if depth == 0 {
                    open = at + 1;
                }
                depth += 1;
            }
            b'}' => {
                depth = depth.checked_sub(1).ok_or("unbalanced '}'")?;
                if depth == 0 {
                    // Output section commands end without a `;`: what follows
                    // one's `}` (`> ROM`) runs on its own line up to the next.
                    let header = statement.trim().lines().last().unwrap_or("");
                    found.push(Group {
                        header: header.trim().to_owned(),
                        body: open..at,
                    });
                    statement.clear();
                }
            }
            b';' if depth == 0 => statement.clear(),
            _ if depth == 0 => statement.push(byte as char),
            _ => {}
        }
        at += 1;
    }
    if depth != 0 {
        return Err("unbalanced '{'".into());
    }
    Ok(found)
}

/// A body line without its comments, trimmed; empty for a comment alone.
fn statement(line: &str) -> String {
    let mut text = line.to_owned();
    while let Some(start) = text.find("/*") {
        let end = text[start..]
            .find("*/")
            .map_or(text.len(), |end| start + end + 2);
        text.replace_range(start..end, "");
    }
    text.trim().to_owned()
}

/// The object a script line places, as `script_objects` reads it.
fn object(line: &str) -> Option<&str> {
    let start = line.find("\"*/")? + 3;
    let end = start + line[start..].find(".o\"")?;
    Some(&line[start..end])
}

/// The game's output sections, inside its `SECTIONS` command.
fn output_sections(text: &str) -> Result<Vec<Group>, String> {
    let top = groups(text, 0..text.len())?;
    let [sections] = top
        .iter()
        .filter(|group| group.header == "SECTIONS")
        .collect::<Vec<_>>()[..]
    else {
        return Err("the game script needs exactly one SECTIONS command".into());
    };
    groups(text, sections.body.clone())
}

/// A native language composition supplies its section bodies directly. The
/// game's Japanese script owns the common memory map and section headers;
/// the English declaration owns the international object's physical order.
/// Native compositions cannot add sections or change those headers.
fn native(game: &str, edition: &str) -> Result<String, String> {
    let sections = output_sections(game)?;
    let mut bodies = BTreeMap::new();
    for group in groups(edition, 0..edition.len())? {
        let name = group.name();
        if group.header != format!("{name} :") {
            return Err(format!(
                "{:?} is not an output section's name",
                group.header
            ));
        }
        let [section] = sections
            .iter()
            .filter(|section| section.name() == name)
            .collect::<Vec<_>>()[..]
        else {
            return Err(format!("the game script has no one output section {name}"));
        };
        if bodies
            .insert(
                section.body.start,
                (section.body.clone(), &edition[group.body.clone()]),
            )
            .is_some()
        {
            return Err(format!("{name} is given twice"));
        }
    }
    let mut text = game.to_owned();
    for (range, body) in bodies.values().rev() {
        text.replace_range(range.clone(), body);
    }
    Ok(text)
}

/// The game script `game` with each output section `edition` lists taking
/// the edition's body. An edition body links the game's own lines of that
/// section in the game's order, and any line naming `own`, its scaffold;
/// every line of an object it links, in any section, comes with it.
pub(crate) fn derive(game: &str, edition: &str, own: &str) -> Result<String, String> {
    let sections = output_sections(game)?;
    let mut bodies = BTreeMap::new();
    for group in groups(edition, 0..edition.len())? {
        let name = group.name().to_owned();
        if group.header != format!("{name} :") {
            return Err(format!(
                "{:?} is not an output section's name",
                group.header
            ));
        }
        let [section] = sections
            .iter()
            .filter(|section| section.name() == name)
            .collect::<Vec<_>>()[..]
        else {
            return Err(format!("the game script has no one output section {name}"));
        };
        let body = &edition[group.body.clone()];
        let theirs: Vec<String> = game[section.body.clone()]
            .lines()
            .map(statement)
            .filter(|line| !line.is_empty())
            .collect();
        let mut next = 0;
        for line in body.lines().map(statement).filter(|line| !line.is_empty()) {
            if line.contains(own) {
                continue;
            }
            match theirs[next..].iter().position(|their| *their == line) {
                Some(offset) => next += offset + 1,
                None => {
                    return Err(format!(
                        "{name} links {line}, which the game's {name} does not list after the lines before it"
                    ))
                }
            }
        }
        if bodies
            .insert(section.body.start, (section.body.clone(), body))
            .is_some()
        {
            return Err(format!("{name} is given twice"));
        }
    }
    let mut text = game.to_owned();
    for (range, body) in bodies.values().rev() {
        text.replace_range(range.clone(), body);
    }
    // An object placed by one line and not another would leave its other
    // sections to the linker's orphan placement.
    let placed: BTreeSet<String> = text.lines().map(statement).collect();
    let linked: BTreeSet<&str> = placed.iter().filter_map(|line| object(line)).collect();
    for line in game.lines().map(statement) {
        if object(&line).is_some_and(|path| linked.contains(path)) && !placed.contains(&line) {
            return Err(format!("links {} without {line}", object(&line).unwrap()));
        }
    }
    Ok(text)
}

#[cfg(test)]
mod tests {
    use super::*;

    const GAME: &str = "OUTPUT_ARCH(arm)\nMEMORY\n{\n    ROM (rx) : ORIGIN = 0x8000000, LENGTH = 32M\n}\nSECTIONS\n{\n    ram (NOLOAD) : { \"*/recon/g/sym.o\"(.sym) } > RAM\n    .rom_start :\n    {\n        \"*/games/G/SRC/A.o\"(.text)\n        \"*/recon/g/raw/0800.o\"(.text)\n        \"*/games/G/SRC/B.o\"(.text)\n        /* a note */\n        Size = End - Start;\n    } > ROM\n    .rom_tail :\n    {\n        \"*/games/G/SRC/B.o\"(.rodata)\n        \"*/games/G/SRC/C.o\"(.rodata)\n    } > ROM\n    Alias = Start;\n}\n";

    #[test]
    fn an_edition_takes_the_bodies_it_gives_and_the_game_keeps_the_rest() {
        let edition = "/* ours */\n.rom_start :\n{\n    \"*/recon/g/ja/rom.o\"(.rom.00000000)\n    \"*/games/G/SRC/B.o\"(.text) /* same */\n}\n.rom_tail :\n{\n    \"*/games/G/SRC/B.o\"(.rodata)\n    \"*/recon/g/ja/rom.o\"(.rom.00000100)\n}\n";
        let text = derive(GAME, edition, "\"*/recon/g/ja/").unwrap();
        assert!(text.contains("ram (NOLOAD) : { \"*/recon/g/sym.o\"(.sym) } > RAM"));
        assert!(
            text.contains("(.rom.00000000)\n    \"*/games/G/SRC/B.o\"(.text) /* same */\n} > ROM")
        );
        assert!(!text.contains("raw/0800") && !text.contains("SRC/A.o") && !text.contains("C.o"));
        assert!(text.contains("    Alias = Start;\n}\n"));
        assert_eq!(output_sections(&text).unwrap().len(), 3);
    }

    #[test]
    fn an_edition_links_the_games_lines_in_order_and_whole_objects() {
        let own = "\"*/recon/g/ja/";
        for (edition, error) in [
            (".rom_start :\n{\n    \"*/games/G/SRC/B.o\"(.text)\n    \"*/games/G/SRC/A.o\"(.text)\n}\n", "does not list after"),
            (".rom_start :\n{\n    \"*/games/G/SRC/D.o\"(.text)\n}\n", "does not list after"),
            (".rom_start :\n{\n    \"*/games/G/SRC/C.o\"(.rodata)\n}\n", "does not list after"),
            (".rom_start :\n{\n    \"*/games/G/SRC/B.o\"(.text)\n}\n.rom_tail :\n{\n}\n", "without \"*/games/G/SRC/B.o\"(.rodata)"),
            (".rom_main :\n{\n}\n", "no one output section .rom_main"),
            (".rom_start 0x8000000 :\n{\n}\n", "not an output section's name"),
            (".rom_start :\n{\n}\n.rom_start :\n{\n}\n", "given twice"),
            (".rom_start :\n{\n    /* open\n}\n", "unterminated comment"),
        ] {
            let result = derive(GAME, edition, own).unwrap_err();
            assert!(result.contains(error), "{edition}: {result}");
        }
        // The game's own statements may come along, in their place.
        let edition = ".rom_start :\n{\n    \"*/games/G/SRC/A.o\"(.text)\n    Size = End - Start;\n}\n.rom_tail :\n{\n    \"*/games/G/SRC/C.o\"(.rodata)\n}\n";
        derive(GAME, edition, own).unwrap();
    }

    #[test]
    fn japanese_is_native_and_international_editions_keep_their_source_order_checks() {
        let japanese = ".rom_start :\n{\n    \"*/recon/g/ja/rom.o\"(.rom.00000000)\n    \"*/games/G/SRC/B.o\"(.text)\n}\n.rom_tail :\n{\n    \"*/games/G/SRC/B.o\"(.rodata)\n}\n";
        let game = native(GAME, japanese).unwrap();
        assert!(!game.contains("SRC/A.o") && !game.contains("SRC/C.o"));
        let english = ".rom_start :\n{\n    \"*/games/G/SRC/A.o\"(.text)\n    \"*/recon/g/raw/0800.o\"(.text)\n    \"*/games/G/SRC/B.o\"(.text)\n    Size = End - Start;\n}\n.rom_tail :\n{\n    \"*/games/G/SRC/B.o\"(.rodata)\n    \"*/games/G/SRC/C.o\"(.rodata)\n}\n";
        let international = native(&game, english).unwrap();
        assert!(international.contains("SRC/A.o") && international.contains("SRC/C.o"));
        assert!(international.contains("Size = End - Start;"));
        assert!(international.contains("ORIGIN = 0x8000000"));
        let localized = ".rom_start :\n{\n    \"*/games/G/SRC/A.o\"(.text)\n    \"*/recon/g/de/rom.o\"(.rom.00000020)\n    \"*/games/G/SRC/B.o\"(.text)\n}\n.rom_tail :\n{\n    \"*/games/G/SRC/B.o\"(.rodata)\n}\n";
        derive(&international, localized, "\"*/recon/g/de/").unwrap();
        for rejected in [
            ".rom_start :\n{\n    \"*/games/G/SRC/B.o\"(.text)\n    \"*/games/G/SRC/A.o\"(.text)\n}\n",
            ".rom_start :\n{\n    \"*/games/G/SRC/D.o\"(.text)\n}\n",
            ".rom_start :\n{\n    \"*/games/G/SRC/B.o\"(.text)\n}\n.rom_tail :\n{\n}\n",
        ] {
            assert!(derive(&international, rejected, "\"*/recon/g/de/").is_err());
        }
        for rejected in [
            ".missing :\n{\n}\n",
            ".rom_start 0x8000000 :\n{\n}\n",
            ".rom_start :\n{\n}\n.rom_start :\n{\n}\n",
        ] {
            assert!(native(&game, rejected).is_err());
        }
    }
}
