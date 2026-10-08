use crate::targets::DecompTarget;
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::ops::Range;
use std::path::{Path, PathBuf};

pub(crate) fn script(root: &Path, target: DecompTarget, output: &Path) -> Result<PathBuf, String> {
    let game = target.script();
    let Some(edition) = target.edition_script() else {
        return Ok(game);
    };
    let read = |path: &Path| {
        fs::read_to_string(root.join(path)).map_err(|error| format!("{}: {error}", path.display()))
    };
    let own = target.scaffold_dir().to_string_lossy().into_owned();
    let product = target.id.as_str().split_once('-').unwrap().0;
    let international_path = crate::targets::edition_script_path(product, "en");
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

#[derive(Debug)]
struct Group {
    header: String,
    body: Range<usize>,
}

impl Group {
    fn name(&self) -> &str {
        self.header
            .split(|c: char| c.is_whitespace() || c == ':')
            .next()
            .unwrap_or("")
    }
}

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

/// A fixed placement, `. = ABSOLUTE(ADDRESS);`: Camelot's far-call
/// banks start at addresses each edition chose, written in its own script
/// as pokeemerald pins its graphics (W3), with the gap as linker fill.
fn placement(line: &str) -> bool {
    line.strip_prefix('.')
        .map(str::trim_start)
        .is_some_and(|rest| rest.starts_with('=') && !rest.starts_with("=="))
}

fn object(line: &str) -> Option<&str> {
    let start = line.find("\"*/")? + 3;
    let end = start + line[start..].find(".o\"")?;
    Some(&line[start..end])
}

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
            if line.contains(own) || placement(&line) {
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

    const GAME: &str = "SECTIONS\n{\n    .rom_main :\n    {\n    \"*/games/G/A.o\"(.text)\n    . = ABSOLUTE(0x08009000);\n    \"*/games/G/FIELD.o\"(.text)\n    \"*/games/G/B.o\"(.text)\n} > ROM\n}\n";

    #[test]
    fn editions_place_their_banks_at_their_own_addresses() {
        let edition = ".rom_main :\n{\n    \"*/games/G/A.o\"(.text)\n    \"*/recon/g/de/rom.o\"(.rom.00000100)\n    . = ABSOLUTE(0x08008800);\n    \"*/games/G/FIELD.o\"(.text)\n    \"*/games/G/B.o\"(.text)\n}\n";
        let text = derive(GAME, edition, "\"*/recon/g/de/").unwrap();
        assert!(text.contains(". = ABSOLUTE(0x08008800);"), "{text}");
        assert!(!text.contains("0x08009000"), "{text}");
        let reordered = ".rom_main :\n{\n    \"*/games/G/B.o\"(.text)\n    \"*/games/G/A.o\"(.text)\n}\n";
        assert!(derive(GAME, reordered, "\"*/recon/g/de/").is_err());
    }

    #[test]
    fn placements_are_assignments_to_dot_only() {
        assert!(placement(". = ABSOLUTE(0x08009000);"));
        assert!(placement(".=0x08009000;"));
        assert!(!placement(".rom_main :"));
        assert!(!placement("\"*/games/G/A.o\"(.text)"));
        assert!(!placement("Symbol = 0x08009000;"));
    }
}
