use crate::targets::DecompTarget;
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::ops::Range;
use std::path::{Path, PathBuf};

/// The script an edition's main image links by: the game's main.ld, or for
/// an edition that derives its own, that script written under `output`.
pub(crate) fn script(root: &Path, target: DecompTarget, output: &Path) -> Result<PathBuf, String> {
    if target.object_list().is_some() {
        return Ok(target.script());
    }
    let Some(text) = script_text(root, target)? else {
        return Ok(target.script());
    };
    let path = root.join(output).join(format!("{}.ld", target.id));
    fs::create_dir_all(root.join(output)).map_err(|error| error.to_string())?;
    if fs::read_to_string(&path).ok().as_deref() != Some(text.as_str()) {
        fs::write(&path, text).map_err(|error| format!("{}: {error}", path.display()))?;
    }
    Ok(path)
}

/// The edition's derived linker script, or None for the Japanese base
/// edition, which links the game's main.ld as it stands.
pub(crate) fn script_text(root: &Path, target: DecompTarget) -> Result<Option<String>, String> {
    let game = target.script();
    let Some(edition) = target.edition_script() else {
        return Ok(None);
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
    Ok(Some(text))
}

/// One of an edition's linked images: the cartridge image, or an overlay by
/// its resource id.
#[derive(Clone, Copy, Debug)]
pub(crate) enum Image<'a> {
    Main,
    Overlay(&'a str),
}

/// One input-section statement of a linker script, `"*/OBJECT.o"(SECTIONS)`,
/// in the output section that holds it.
#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Statement {
    /// The output section's name: `.rom_main`, `ewram`, `/DISCARD/`.
    pub section: String,
    /// The object without `.o`: a path from the checkout, an archive member
    /// (`libgcc.a:_call_via_rX`), an overlay's own `resource_<id>_overlay`,
    /// or `*` for every object.
    pub object: String,
    /// The input sections, as the script spells them.
    pub sections: String,
}

impl Statement {
    /// The object the build makes from a source: not an archive member, and
    /// not a pattern for every object.
    pub(crate) fn built(&self) -> bool {
        !self.object.contains([':', '*'])
    }
}

/// What a statement in an output section's body is.
#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) enum Item {
    Input(Statement),
    /// A fixed placement, with its address expression: `ABSOLUTE(0x08009000)`.
    Placement(String),
    /// Any other assignment, as written.
    Assignment(String),
}

/// How an image of a game with an object list links: the one maintained
/// script, which names no object, and the fragments the build writes from
/// the list for the script to INCLUDE.
#[derive(Clone, Debug)]
pub(crate) struct Listed {
    /// The maintained script, from the checkout.
    pub script: PathBuf,
    /// Each fragment's name without `.ld`, and its text.
    pub fragments: Vec<(String, String)>,
    /// Where the fragments go under an edition's build output.
    pub directory: PathBuf,
}

impl Listed {
    /// The script as the linker reads it: each `INCLUDE "NAME.ld"` of a
    /// fragment replaced by the fragment. Every fragment is read once, and
    /// an INCLUDE of a path (games/tbs/constants.ld) is left to the linker.
    pub(crate) fn flat(&self, root: &Path) -> Result<String, String> {
        let text = fs::read_to_string(root.join(&self.script))
            .map_err(|error| format!("{}: {error}", self.script.display()))?;
        flat(&text, &self.fragments).map_err(|error| format!("{}: {error}", self.script.display()))
    }

    /// Write the fragments under `output`, each only when its text changed,
    /// and remove any other fragment found there. Returns their directory,
    /// for the linker's `-L`.
    pub(crate) fn write(&self, output: &Path) -> Result<PathBuf, String> {
        let directory = output.join(&self.directory);
        fs::create_dir_all(&directory).map_err(|error| format!("{}: {error}", directory.display()))?;
        for entry in fs::read_dir(&directory).into_iter().flatten().flatten() {
            let path = entry.path();
            let stale = path.extension().is_some_and(|extension| extension == "ld")
                && !self
                    .fragments
                    .iter()
                    .any(|(name, _)| path.file_stem().is_some_and(|stem| stem == name.as_str()));
            if stale {
                fs::remove_file(&path).map_err(|error| format!("{}: {error}", path.display()))?;
            }
        }
        for (name, text) in &self.fragments {
            let path = directory.join(format!("{name}.ld"));
            if fs::read_to_string(&path).ok().as_deref() != Some(text.as_str()) {
                fs::write(&path, text).map_err(|error| format!("{}: {error}", path.display()))?;
            }
        }
        Ok(directory)
    }
}

/// The name a line includes a fragment by: `INCLUDE "NAME.ld"`, a bare name.
fn included_fragment(line: &str) -> Option<&str> {
    let name = line
        .trim()
        .strip_prefix("INCLUDE")?
        .trim()
        .strip_prefix('"')?
        .strip_suffix(".ld\"")?;
    (!name.is_empty() && name.bytes().all(|byte| byte.is_ascii_lowercase() || byte.is_ascii_digit() || byte == b'_'))
        .then_some(name)
}

fn flat(script: &str, fragments: &[(String, String)]) -> Result<String, String> {
    let mut text = String::with_capacity(script.len());
    let mut read = BTreeSet::new();
    for line in script.split_inclusive('\n') {
        let Some(name) = included_fragment(line) else {
            text.push_str(line);
            continue;
        };
        let (_, fragment) = fragments
            .iter()
            .find(|(known, _)| known == name)
            .ok_or_else(|| format!("INCLUDE \"{name}.ld\" names no place of the object list"))?;
        if !read.insert(name) {
            return Err(format!("{name}.ld is included twice"));
        }
        text.push_str(fragment);
    }
    match fragments.iter().find(|(name, _)| !read.contains(name.as_str())) {
        Some((name, _)) => Err(format!("the object list has the place {name}, and no INCLUDE \"{name}.ld\"")),
        None => Ok(text),
    }
}

/// The listed form of an image, or None when the image links by a script
/// that names its own objects.
pub(crate) fn listed(root: &Path, target: DecompTarget, image: Image) -> Result<Option<Listed>, String> {
    let Some(list) = crate::object_list::read(root, target)? else {
        return Ok(None);
    };
    Ok(match image {
        Image::Main => Some(Listed {
            script: target.script(),
            fragments: crate::object_list::render(&list, crate::object_list::MAIN, target.language()),
            directory: PathBuf::from("ld"),
        }),
        Image::Overlay(_) => None,
    })
}

/// The linker script text an image links by, as the linker reads it: a
/// listed game's script with its fragments in place, else the edition's
/// derived main script, or its overlay's own.
pub(crate) fn image_script(root: &Path, target: DecompTarget, image: Image) -> Result<String, String> {
    if let Some(listed) = listed(root, target, image)? {
        return listed.flat(root);
    }
    let path = match image {
        Image::Main => match script_text(root, target)? {
            Some(text) => return Ok(text),
            None => target.script(),
        },
        Image::Overlay(id) => target.overlay_script(id),
    };
    fs::read_to_string(root.join(&path)).map_err(|error| format!("{}: {error}", path.display()))
}

/// The statements an edition's image links, in the linker's order: every
/// object with the input sections it gives each output section. This is the
/// one place that says what an image links.
pub(crate) fn linked_statements(
    root: &Path,
    target: DecompTarget,
    image: Image,
) -> Result<Vec<Statement>, String> {
    statements(&image_script(root, target, image)?)
        .map_err(|error| format!("{} {image:?}: {error}", target.id))
}

/// The objects the build makes for these statements, each once, in order.
pub(crate) fn built_objects(statements: &[Statement]) -> Vec<String> {
    let mut seen = BTreeSet::new();
    statements
        .iter()
        .filter(|statement| statement.built())
        .map(|statement| statement.object.clone())
        .filter(|object| seen.insert(object.clone()))
        .collect()
}

/// Every input-section statement of a linker script's text.
pub(crate) fn statements(text: &str) -> Result<Vec<Statement>, String> {
    Ok(items(text)?
        .into_iter()
        .flat_map(|(_, items)| items)
        .filter_map(|item| match item {
            Item::Input(statement) => Some(statement),
            _ => None,
        })
        .collect())
}

/// Each output section of a script, by name, with its body's statements.
pub(crate) fn items(text: &str) -> Result<Vec<(String, Vec<Item>)>, String> {
    output_sections(text)?
        .iter()
        .map(|group| {
            let name = group.name().to_owned();
            let items = body_items(&name, &text[group.body.clone()])
                .map_err(|error| format!("{name}: {error}"))?;
            Ok((name, items))
        })
        .collect()
}

fn body_items(section: &str, body: &str) -> Result<Vec<Item>, String> {
    let body = statement(body);
    let mut rest = body.as_str();
    let mut found = Vec::new();
    loop {
        rest = rest.trim_start();
        if rest.is_empty() {
            return Ok(found);
        }
        let (pattern, after) = match rest.strip_prefix('"') {
            Some(quoted) => {
                let end = quoted.find('"').ok_or("unterminated quoted name")?;
                (&quoted[..end], &quoted[end + 1..])
            }
            None => {
                let end = rest
                    .find(|c: char| c.is_whitespace() || matches!(c, '(' | '=' | ';'))
                    .unwrap_or(rest.len());
                (&rest[..end], &rest[end..])
            }
        };
        match after.trim_start().strip_prefix('(') {
            Some(list) => {
                let end = list
                    .find(')')
                    .ok_or_else(|| format!("{pattern} has no closing ')'"))?;
                let object = pattern
                    .strip_prefix('*')
                    .map(|path| path.strip_prefix('/').unwrap_or(path))
                    .and_then(|path| match path {
                        "" => Some("*"),
                        _ => path.strip_suffix(".o"),
                    })
                    .ok_or_else(|| format!("{pattern:?} names no object"))?;
                found.push(Item::Input(Statement {
                    section: section.to_owned(),
                    object: object.to_owned(),
                    sections: list[..end].split_whitespace().collect::<Vec<_>>().join(" "),
                }));
                rest = &list[end + 1..];
            }
            None => {
                let end = rest
                    .find(';')
                    .ok_or_else(|| format!("{pattern:?} starts no statement the linker reads"))?;
                let assignment = &rest[..end];
                found.push(match assignment.split_once('=') {
                    Some((name, value)) if name.trim() == "." => {
                        Item::Placement(value.trim().to_owned())
                    }
                    Some(_) => Item::Assignment(assignment.trim().to_owned()),
                    None => return Err(format!("{assignment:?} is no assignment")),
                });
                rest = &rest[end + 1..];
            }
        }
    }
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

    const GAME: &str = "SECTIONS\n{\n    .rom_main :\n    {\n    \"*/games/g/a.o\"(.text)\n    . = ABSOLUTE(0x08009000);\n    \"*/games/g/field.o\"(.text)\n    \"*/games/g/b.o\"(.text)\n} > ROM\n}\n";

    #[test]
    fn editions_place_their_banks_at_their_own_addresses() {
        let edition = ".rom_main :\n{\n    \"*/games/g/a.o\"(.text)\n    \"*/recon/g/de/rom.o\"(.rom.00000100)\n    . = ABSOLUTE(0x08008800);\n    \"*/games/g/field.o\"(.text)\n    \"*/games/g/b.o\"(.text)\n}\n";
        let text = derive(GAME, edition, "\"*/recon/g/de/").unwrap();
        assert!(text.contains(". = ABSOLUTE(0x08008800);"), "{text}");
        assert!(!text.contains("0x08009000"), "{text}");
        let reordered = ".rom_main :\n{\n    \"*/games/g/b.o\"(.text)\n    \"*/games/g/a.o\"(.text)\n}\n";
        assert!(derive(GAME, reordered, "\"*/recon/g/de/").is_err());
    }

    #[test]
    fn a_listed_script_reads_each_fragment_where_it_includes_it() {
        let script = "INCLUDE \"games/g/constants.ld\"\nSECTIONS\n{\n    .rom_main :\n    {\n        INCLUDE \"rom_main_00.ld\"\n        . = ABSOLUTE(DEFINED(G_EDITION_JA) ? 0x08009000 :\n                     0x08008800);\n        INCLUDE \"rom_main_01.ld\"\n    } > ROM\n}\n";
        let fragment = |name: &str, text: &str| (name.to_owned(), text.to_owned());
        let fragments = [
            fragment("rom_main_00", "    \"*/games/g/a.o\"(.text)\n"),
            fragment("rom_main_01", ""),
        ];
        let text = flat(script, &fragments).unwrap();
        assert_eq!(
            text,
            "INCLUDE \"games/g/constants.ld\"\nSECTIONS\n{\n    .rom_main :\n    {\n    \"*/games/g/a.o\"(.text)\n        . = ABSOLUTE(DEFINED(G_EDITION_JA) ? 0x08009000 :\n                     0x08008800);\n    } > ROM\n}\n"
        );
        let found = items(&text).unwrap();
        assert_eq!(found[0].1.len(), 2);
        assert!(matches!(&found[0].1[1], Item::Placement(address) if address.starts_with("ABSOLUTE(DEFINED(G_EDITION_JA)")));
        // The script and the list name the same places, each once.
        assert!(flat(script, &fragments[..1]).unwrap_err().contains("rom_main_01.ld\" names no place"));
        let more = [fragments[0].clone(), fragments[1].clone(), fragment("rom_tail", "")];
        assert!(flat(script, &more).unwrap_err().contains("the place rom_tail"));
        let twice = script.replace("rom_main_01", "rom_main_00");
        assert!(flat(&twice, &fragments[..1]).unwrap_err().contains("included twice"));
    }

    #[test]
    fn fragments_are_written_when_they_change_and_stale_ones_removed() {
        let output = tempfile::tempdir().unwrap();
        let listed = |fragments: &[(&str, &str)]| Listed {
            script: PathBuf::from("games/g/main.ld"),
            fragments: fragments.iter().map(|(name, text)| (name.to_string(), text.to_string())).collect(),
            directory: PathBuf::from("ld"),
        };
        let directory = listed(&[("rom_start", "a\n"), ("rom_tail", "b\n")]).write(output.path()).unwrap();
        assert_eq!(directory, output.path().join("ld"));
        let stamp = |name: &str| fs::metadata(directory.join(name)).unwrap().modified().unwrap();
        let before = stamp("rom_start.ld");
        listed(&[("rom_start", "a\n"), ("rom_main", "c\n")]).write(output.path()).unwrap();
        assert_eq!(stamp("rom_start.ld"), before);
        assert!(!directory.join("rom_tail.ld").exists());
        assert_eq!(fs::read_to_string(directory.join("rom_main.ld")).unwrap(), "c\n");
    }

    /// What the build read before linked_statements: every quoted object
    /// path in a script's text, each once.
    fn quoted_objects(script: &str) -> Vec<String> {
        let pattern = regex::Regex::new(r#""\*/([^"*:]+)\.o""#).unwrap();
        let mut seen = BTreeSet::new();
        pattern
            .captures_iter(script)
            .map(|capture| capture[1].to_owned())
            .filter(|path| seen.insert(path.clone()))
            .collect()
    }

    #[test]
    fn linked_statements_name_the_objects_every_script_names() {
        let root = crate::compiler::routing::root();
        let mut overlays = 0;
        for id in crate::targets::TARGET_IDS {
            let target = crate::targets::target_for(id);
            let text = image_script(root, target, Image::Main).unwrap();
            let linked = linked_statements(root, target, Image::Main).unwrap();
            assert!(linked.len() > 1000, "{id}");
            assert_eq!(built_objects(&linked), quoted_objects(&text), "{id}");
            let mut scripts: Vec<_> = fs::read_dir(root.join(target.overlay_dir))
                .unwrap()
                .flatten()
                .map(|entry| entry.file_name().to_string_lossy().into_owned())
                .filter_map(|name| {
                    let stem = name.strip_suffix(".ld")?;
                    let id = stem.strip_prefix("ovl_").or(stem.strip_prefix("resource_"))?;
                    Some(id.to_owned())
                })
                .collect();
            scripts.sort();
            for overlay in &scripts {
                let image = Image::Overlay(overlay);
                assert!(root.join(target.overlay_script(overlay)).is_file(), "{id} {overlay}");
                let text = image_script(root, target, image).unwrap();
                let linked = linked_statements(root, target, image).unwrap();
                assert_eq!(built_objects(&linked), quoted_objects(&text), "{id} {overlay}");
                overlays += 1;
            }
        }
        assert!(overlays > 1000, "{overlays}");
    }

    #[test]
    fn a_body_holds_inputs_placements_and_assignments() {
        let text = "EXTERN(_call_via_r0)\nSECTIONS\n{\n    ram (NOLOAD) : { \"*/g/ram.o\"(.sym) } > RAM\n    .text 0x2008000 :\n    {\n        \"*/g/a.o\"(.text  .rodata) /* note */\n        *libgcc.a:_call_via_rX.o(.text)\n        \"*libgcc.a:_pack_df.o\"(.text)\n        . = ABSOLUTE(0x08009000);\n        Size = ABSOLUTE(B - A);\n        \"*/resource_36f_overlay.o\"(.bss)\n    } > ROM\n    Top = LOADADDR(.text);\n    /DISCARD/ : { \"*/g/a.o\"(.text.x) *(.comment) *(.note*) }\n}\n";
        let found = items(text).unwrap();
        let names: Vec<&str> = found.iter().map(|(name, _)| name.as_str()).collect();
        assert_eq!(names, ["ram", ".text", "/DISCARD/"]);
        let input = |object: &str, sections: &str| {
            Item::Input(Statement {
                section: ".text".into(),
                object: object.into(),
                sections: sections.into(),
            })
        };
        assert_eq!(
            found[1].1,
            [
                input("g/a", ".text .rodata"),
                input("libgcc.a:_call_via_rX", ".text"),
                input("libgcc.a:_pack_df", ".text"),
                Item::Placement("ABSOLUTE(0x08009000)".into()),
                Item::Assignment("Size = ABSOLUTE(B - A)".into()),
                input("resource_36f_overlay", ".bss"),
            ]
        );
        let linked = statements(text).unwrap();
        assert_eq!(linked.len(), 8);
        assert_eq!(built_objects(&linked), ["g/ram", "g/a", "resource_36f_overlay"]);
        assert_eq!(linked[7].object, "*");
        assert!(items("SECTIONS\n{\n    .text :\n    {\n        stray\n    }\n}\n").is_err());
    }

    #[test]
    fn placements_are_assignments_to_dot_only() {
        assert!(placement(". = ABSOLUTE(0x08009000);"));
        assert!(placement(".=0x08009000;"));
        assert!(!placement(".rom_main :"));
        assert!(!placement("\"*/games/g/a.o\"(.text)"));
        assert!(!placement("Symbol = 0x08009000;"));
    }
}
