use crate::adopt_data::Cut;
use crate::compiler::routing::root;
use crate::ld_map::{Map, ROM};
use std::collections::HashMap;
use std::path::{Path, PathBuf};

const USAGE: &str = r#"usage: alchemy adopt ADDRESS LABEL [NEW_NAME] [--game tbs|tla] [--padding]
                    [--into MODULE.c [--head]] [--skip LANG,...]
After the function's C is in its module: removes recon/<game>/raw/ADDRESS.s,
its line among the English edition's objects and the draft recon/<game>/en/
main/ADDRESS.c; in each other edition removes the rom.s section that holds
exactly LABEL and its line among that edition's objects (☀️'s rows of
games/tbs/objects.tsv; ⚓️'s games/tla/main.ld for Japanese and
recon/tla/<lang>/main.ld). --padding also removes a two-byte section
directly before or after it, and English's two-byte padding listing linked
directly before or after the function. NEW_NAME renames LABEL across games/
and recon/. The game defaults to tbs. Then build all twelve editions.

An edition whose scaffold does not hold LABEL as one section of its own (no
label there, or more in its section) is cut by the function's bytes: the
English function, where out/<game>-en's map placed its listing, is looked
for among that edition's scaffold bytes in roms/<game>-<lang>.gba, as the
same bytes or with only its call targets and literal-pool words moved, near
where the edition's nearest labels say it lies. Those bytes leave their
section, which is split around them, and the edition's object lines link
what is left in the same place. With --padding the two zero bytes go with
them on each side where English's padding listing went. A listing that
itself ends in two bytes of padding leaves them to alignment: the listing
or scaffold section linked right after the function starts with ".balign 4"
in every edition, as C ends at its last instruction. An edition with no
such copy stops the adoption; --skip LANG,... leaves the named editions on
their scaffold (S4), for a function whose C those builds leave out.

--into first merges the draft into MODULE.c, its linker neighbour: the
draft's missing includes after the module's top include block, then the
declarations and functions the module lacks at its end, or with --head
before its first function; the draft's audit header stays behind. A name
the module or its headers already define otherwise refuses the merge.
A MODULE.c that does not exist is made from the draft, and an edition
whose object lines do not link the module yet links it where the function
was. Nothing changes unless every step can be made."#;

fn read(path: &Path) -> Result<String, String> {
    std::fs::read_to_string(path).map_err(|e| format!("{}: {e}", path.display()))
}

fn write(path: &Path, text: &str) -> Result<(), String> {
    std::fs::write(path, text).map_err(|e| format!("{}: {e}", path.display()))
}

fn section_name(line: &str) -> Option<&str> {
    line.strip_prefix("\t.section .rom.")?.strip_suffix(", \"ax\"")
}

fn is_label(line: &str) -> bool {
    line.ends_with(':') && !line.starts_with(['\t', ' ', '@', '.'])
}

fn section_of(lines: &[&str], label: &str) -> Option<(usize, usize)> {
    let at = lines.iter().position(|l| *l == format!("{label}:"))?;
    let start = (0..=at).rev().find(|&i| section_name(lines[i]).is_some())?;
    let end = (at + 1..lines.len()).find(|&i| section_name(lines[i]).is_some()).unwrap_or(lines.len());
    Some((start, end))
}

fn padding_section(lines: &[&str], start: usize, end: usize) -> bool {
    end - start == 2 && lines[start + 1].ends_with(", 0x00000002")
}

pub(crate) fn remove_section(text: &str, label: &str, padding: bool) -> Result<(String, Vec<String>), String> {
    let lines: Vec<&str> = text.lines().collect();
    let (start, end) = section_of(&lines, label).ok_or_else(|| format!("no section holds {label}"))?;
    let block = &lines[start..end];
    let labels = block.iter().filter(|l| is_label(l)).count();
    let incbins = block.iter().filter(|l| l.contains(".incbin")).count();
    if labels != 1 || incbins != 1 {
        return Err(format!(
            "the section holding {label} holds {labels} labels and {incbins} byte ranges; split it by hand"
        ));
    }
    let mut cut = vec![(start, end)];
    if padding {
        let previous = (0..start).rev().find(|&i| section_name(lines[i]).is_some());
        if let Some(p) = previous.filter(|&p| padding_section(&lines, p, start)) {
            cut.push((p, start));
        }
        let next_end = (end + 1..lines.len()).find(|&i| section_name(lines[i]).is_some()).unwrap_or(lines.len());
        if end < lines.len() && padding_section(&lines, end, next_end) {
            cut.push((end, next_end));
        }
    }
    let names = cut.iter().filter_map(|&(s, _)| section_name(lines[s]).map(str::to_string)).collect();
    let kept: Vec<&str> = lines
        .iter()
        .enumerate()
        .filter(|(i, _)| !cut.iter().any(|&(s, e)| *i >= s && *i < e))
        .map(|(_, l)| *l)
        .collect();
    Ok((kept.join("\n") + "\n", names))
}

/// Takes the removed scaffold sections' lines out of an edition's object
/// lines; `object`, a module not linked there yet, takes the place of the
/// function's own section, the first.
fn unlink_sections(script: &str, sections: &[String], object: Option<&str>) -> Result<String, String> {
    let mut out = Vec::new();
    let mut removed = 0;
    for line in script.lines() {
        match sections.iter().position(|section| line.contains(&format!("(.rom.{section})"))) {
            Some(which) => {
                removed += 1;
                if let Some(object) = object.filter(|_| which == 0) {
                    out.push(format!("{}{object}", &line[..line.len() - line.trim_start().len()]));
                }
            }
            None => out.push(line.to_string()),
        }
    }
    if removed != sections.len() {
        return Err(format!("expected {} section lines, removed {removed}", sections.len()));
    }
    Ok(out.join("\n") + "\n")
}

/// The scaffold section an edition's object lines link right after the
/// function's section `sections[0]`, past the other sections leaving with it.
fn next_scaffold_section(script: &str, scaffold: &str, sections: &[String]) -> Option<String> {
    let name = |line: &str| Some(line.trim().strip_prefix(&format!("\"*/{scaffold}\"(.rom."))?.strip_suffix(')')?.to_string());
    let mut lines = script.lines().skip_while(|line| name(line).as_ref() != sections.first()).skip(1);
    lines.find(|line| name(line).is_none_or(|name| !sections.contains(&name))).and_then(name)
}

/// The scaffold with `.balign 4` at the head of section `name`: what follows
/// a function whose listing ended in two bytes of padding stays where it was
/// once the function is C, which ends at its last instruction.
fn align_section(scaffold: &str, name: &str) -> String {
    let header = format!("\t.section .rom.{name}, \"ax\"");
    let lines: Vec<&str> = scaffold.lines().collect();
    let mut out = Vec::with_capacity(lines.len() + 1);
    for (i, line) in lines.iter().enumerate() {
        out.push(*line);
        if *line == header && lines.get(i + 1) != Some(&"\t.balign 4") {
            out.push("\t.balign 4");
        }
    }
    out.join("\n") + "\n"
}

/// A listing that starts on a 4-byte boundary, as `align_section` aligns a
/// scaffold section: `.balign 4` after its opening directives.
fn align_listing(listing: &str) -> String {
    let opening = |line: &str| matches!(line.trim(), ".syntax unified" | ".thumb" | ".arm");
    if listing.lines().any(|line| line.trim() == ".balign 4") {
        return listing.to_string();
    }
    let at = listing.lines().take_while(|line| opening(line)).count();
    let mut lines: Vec<&str> = listing.lines().collect();
    lines.insert(at, "\t.balign 4");
    lines.join("\n") + "\n"
}

/// A listing whose code ends in two bytes of alignment padding.
fn padding_tail(listing: &str) -> bool {
    listing.lines().map(str::trim).rfind(|line| !line.is_empty()) == Some(".2byte 0x0000")
}

/// A listing that holds nothing but two bytes of alignment padding.
fn padding_listing(text: &str) -> bool {
    let body: Vec<&str> = text.lines().map(str::trim).filter(|line| !line.is_empty() && !matches!(*line, ".syntax unified" | ".thumb")).collect();
    body == [".2byte 0x0000"]
}

/// The linker statement's object for a module under games/: `"*/games/.../x.o"`.
fn module_object(root: &Path, module: &Path) -> Result<String, String> {
    let relative = module.strip_prefix(root).unwrap_or(module);
    let stem = relative.with_extension("");
    let stem = stem.to_string_lossy();
    if !stem.starts_with("games/") || stem.contains("..") || relative.extension().is_none_or(|extension| extension != "c") {
        return Err(format!("{}: the module must be a .c file under games/, named from the checkout", module.display()));
    }
    Ok(format!("\"*/{stem}.o\""))
}

/// The English function an edition's copy is found by: its bytes in the
/// English ROM, where the last English build's map placed its listing.
struct Reference {
    bytes: Vec<u8>,
    base: u32,
    arm: bool,
    /// Each global name the English build placed, with its address.
    names: HashMap<String, u32>,
}

impl Reference {
    fn load(root: &Path, game: &str, address: &str) -> Result<Reference, String> {
        let path = root.join(format!("out/{game}-en/{game}-en.map"));
        let map = Map::parse(&read(&path).map_err(|e| format!("{e}; build {game}-en first (make compare-{game}-en)"))?);
        let object = format!("recon/{game}/raw/{address}.o");
        let placed = map
            .placed
            .iter()
            .find(|placed| placed.name == ".text" && placed.size > 0 && placed.path.ends_with(&object))
            .ok_or_else(|| format!("{}: no .text of {object}; build {game}-en first (make compare-{game}-en)", path.display()))?;
        let rom = root.join(format!("roms/{game}-en.gba"));
        let rom = std::fs::read(&rom).map_err(|e| format!("{}: {e}", rom.display()))?;
        let offset = placed.address.checked_sub(ROM).ok_or_else(|| format!("{object} is not in the ROM's main image"))? as usize;
        let bytes = rom.get(offset..offset + placed.size as usize).ok_or_else(|| format!("{object} lies beyond the English ROM"))?.to_vec();
        let arm = read(&root.join(format!("recon/{game}/raw/{address}.s")))?.lines().any(|line| line.trim() == ".arm");
        let names = map.placed.iter().flat_map(|placed| placed.symbols.iter().map(|(address, name)| (name.clone(), *address))).collect();
        Ok(Reference { bytes, base: placed.address, arm, names })
    }
}

#[derive(Debug)]
struct Carved {
    cut: Cut,
    note: String,
}

/// Cuts an edition's copy of the English function out of its scaffold,
/// wherever its labels are. The copy is looked for where the nearest label
/// both builds place says it should be, by the edition's shift there; with
/// --padding it leaves with the two zero bytes on each side on which
/// English's padding listing left (`pads`: before, after).
fn carve(reference: &Reference, scaffold: &str, rom: &[u8], label: &str, pads: (bool, bool)) -> Result<Carved, String> {
    let labels = crate::adopt_data::label_offsets(scaffold)?;
    let offset = (reference.base - ROM) as usize;
    let shift = labels
        .iter()
        .filter_map(|(name, file)| reference.names.get(name).map(|address| (address.abs_diff(reference.base), i64::from(*file) - (i64::from(*address) - i64::from(ROM)))))
        .min_by_key(|(distance, _)| *distance)
        .map_or(0, |(_, shift)| shift);
    let expected = (offset as i64 + shift).max(0) as usize;
    let extents = crate::adopt_data::extents(scaffold)?;
    let copy = crate::label::copy_of(&reference.bytes, reference.base, reference.arm, rom, &extents, expected)
        .ok_or("its scaffold holds no identical or relocated copy of the English function")?;
    if let Some((_, named)) = labels.iter().find(|(name, named)| name == label && *named as usize != copy.at) {
        return Err(format!("its scaffold names {label} at {named:#x}, but the nearest copy of the English function is at {:#x}", copy.at));
    }
    let zero = |at: usize| rom.get(at..at + 2) == Some(&[0, 0]);
    let (start, end) = (copy.at, copy.at + reference.bytes.len());
    if (pads.0 && !(start >= 2 && zero(start - 2))) || (pads.1 && !zero(end)) {
        return Err(format!("English's padding listing beside the function has no two zero bytes beside the copy at {start:#x}"));
    }
    let (start, end) = (start - if pads.0 { 2 } else { 0 }, end + if pads.1 { 2 } else { 0 });
    let cut = crate::adopt_data::cut_bytes(scaffold, start as u32, (end - start) as u32)?;
    let mut note = format!("{} copy at {:#x}", copy.class, copy.at);
    if copy.found > 1 {
        note.push_str(&format!(", the nearest of {} to {expected:#x}", copy.found));
    }
    note.push_str(&format!(", cut from .rom.{}", cut.section));
    let others: Vec<&str> = cut.labels.iter().map(String::as_str).filter(|name| *name != label).collect();
    if !others.is_empty() {
        note.push_str(&format!("; the C must define {}, which left with it", others.join(" ")));
    }
    Ok(Carved { cut, note })
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let mut positional = Vec::new();
    let (mut game, mut padding) = ("tbs", false);
    let (mut into, mut head) = (None, false);
    let mut skip: Vec<String> = Vec::new();
    let mut i = 0;
    while i < arguments.len() {
        match arguments[i].as_str() {
            "--padding" => padding = true,
            "--skip" => {
                i += 1;
                let list = arguments.get(i).ok_or_else(|| USAGE.to_string())?;
                skip = list.split(',').map(str::to_string).collect();
                if skip.iter().any(|lang| !["ja", "de", "es", "fr", "it"].contains(&lang.as_str())) {
                    return Err(USAGE.into());
                }
            }
            "--head" => head = true,
            "--into" => {
                i += 1;
                into = Some(PathBuf::from(arguments.get(i).ok_or_else(|| USAGE.to_string())?));
            }
            "--game" => {
                i += 1;
                game = match arguments.get(i).map(String::as_str) {
                    Some("tbs") => "tbs",
                    Some("tla") => "tla",
                    _ => return Err(USAGE.into()),
                };
            }
            "-h" | "--help" => return Err(USAGE.into()),
            value => positional.push(value.to_string()),
        }
        i += 1;
    }
    let (address, label, new_name) = match positional.as_slice() {
        [a, l] => (a.clone(), l.clone(), None),
        [a, l, n] => (a.clone(), l.clone(), Some(n.clone())),
        _ => return Err(USAGE.into()),
    };
    if head && into.is_none() {
        return Err(USAGE.into());
    }
    let root = root();
    let mut edits: Vec<(PathBuf, String)> = Vec::new();
    let module = into.as_deref().map(|module| module_object(root, module)).transpose()?;

    // Each edition's object lines: its linker script, or its rows of the
    // game's object list.
    let mut lines: Vec<(&str, String)> = Vec::new();
    let english = crate::object_list::main_lines_path(game, "en");
    let listing = |line: &str| -> Option<String> {
        let name = line.trim().strip_prefix(&format!("\"*/recon/{game}/raw/"))?.split_once(".o\"(")?.0;
        Some(name.to_string())
    };
    let is_padding = |line: Option<&&str>| {
        let file = line.and_then(|line| listing(line)).map(|name| root.join(format!("recon/{game}/raw/{name}.s")));
        padding && file.is_some_and(|file| std::fs::read_to_string(file).is_ok_and(|text| padding_listing(&text)))
    };
    let script = crate::object_list::main_lines(root, game, "en")?;
    let script_lines: Vec<&str> = script.lines().collect();
    let at: Vec<usize> = (0..script_lines.len()).filter(|&i| listing(script_lines[i]).as_deref() == Some(&address)).collect();
    let [at] = at[..] else {
        return Err(format!("{}: {} lines link \"*/recon/{game}/raw/{address}.o\"", english.display(), at.len()));
    };
    let pads = (at > 0 && is_padding(script_lines.get(at - 1)), is_padding(script_lines.get(at + 1)));
    let sections = script_lines[at].trim().split_once(".o\"").map_or("(.text)", |(_, sections)| sections);
    // The module's line, for each edition whose lines do not link it yet.
    let unlinked = |script: &str| module.as_ref().filter(|object| !script.contains(&format!("{object}("))).map(|object| format!("{object}{sections}"));
    let mut text = Vec::new();
    let mut listings = vec![address.clone()];
    for (i, line) in script_lines.iter().enumerate() {
        if i == at {
            let indent = &line[..line.len() - line.trim_start().len()];
            text.extend(unlinked(&script).map(|object| format!("{indent}{object}")));
        } else if (i + 1 == at && pads.0) || (i == at + 1 && pads.1) {
            listings.extend(listing(line));
        } else {
            text.push(line.to_string());
        }
    }
    lines.push(("en", text.join("\n") + "\n"));
    // C ends at its last instruction: what follows a listing that ended in
    // padding is aligned instead, in every edition.
    let tail = padding_tail(&read(&root.join(format!("recon/{game}/raw/{address}.s")))?);
    let after = script_lines.get(at + 1 + usize::from(pads.1)).and_then(|line| listing(line));
    if let Some(next) = after.filter(|_| tail) {
        let path = root.join(format!("recon/{game}/raw/{next}.s"));
        let aligned = align_listing(&read(&path)?);
        edits.push((path, aligned));
    }

    let mut reference = None;
    for lang in ["ja", "de", "es", "fr", "it"] {
        if skip.iter().any(|skipped| skipped == lang) {
            println!("{game}-{lang}: skipped, it keeps its scaffold; its build must not link the function's C");
            continue;
        }
        let scaffold = root.join(format!("recon/{game}/{lang}/rom.s"));
        let script = crate::object_list::main_lines(root, game, lang)?;
        let scaffold_text = read(&scaffold)?;
        let object = unlinked(&script);
        let rom_object = format!("recon/{game}/{lang}/rom.o");
        let (text, script, follows) = match remove_section(&scaffold_text, &label, padding) {
            Ok((text, sections)) => {
                let follows = next_scaffold_section(&script, &rom_object, &sections);
                let script = unlink_sections(&script, &sections, object.as_deref()).map_err(|e| format!("{}: {e}", crate::object_list::main_lines_path(game, lang).display()))?;
                (text, script, follows)
            }
            // The scaffold does not hold the function as one section of its
            // own: find its copy by its bytes and cut it out.
            Err(unlabelled) => {
                if reference.is_none() {
                    reference = Some(Reference::load(root, game, &address).map_err(|e| format!("{game}-{lang}: {unlabelled}; {e}"))?);
                }
                let reference = reference.as_ref().expect("loaded above");
                let rom = root.join(format!("roms/{game}-{lang}.gba"));
                let rom = std::fs::read(&rom).map_err(|e| format!("{}: {e}", rom.display()))?;
                let carved = carve(reference, &scaffold_text, &rom, &label, pads).map_err(|e| format!("{game}-{lang}: {e}; adopt the others with --skip {lang} once its build leaves the function's C out (S4)"))?;
                println!("{game}-{lang}: {}", carved.note);
                let follows = carved.cut.after.clone().or_else(|| next_scaffold_section(&script, &rom_object, std::slice::from_ref(&carved.cut.section)));
                let script = crate::adopt_data::relink_parts(&script, &rom_object, ".rom.", &carved.cut, object.as_deref())
                    .map_err(|e| format!("{}: {e}", crate::object_list::main_lines_path(game, lang).display()))?;
                (carved.cut.text, script, follows)
            }
        };
        let text = match follows.filter(|_| tail) {
            Some(next) => align_section(&text, &next),
            None => text,
        };
        edits.push((scaffold, text));
        lines.push((lang, script));
    }

    if let Some(module) = &into {
        // A module that does not exist yet starts as the draft alone.
        let path = if module.is_absolute() { module.clone() } else { root.join(module) };
        let new = !path.exists();
        if new {
            std::fs::create_dir_all(path.parent().unwrap_or(root)).map_err(|e| format!("{}: {e}", path.display()))?;
            write(&path, "")?;
        }
        if let Err(error) = crate::adopt_into::run(root, game, &address, module, head) {
            if new {
                let _ = std::fs::remove_file(&path);
            }
            return Err(error);
        }
    }
    for (path, text) in &edits {
        write(path, text)?;
    }
    crate::object_list::write_main_lines(root, game, &lines)?;
    let listings = listings.iter().map(|name| format!("recon/{game}/raw/{name}.s"));
    for file in listings.chain([format!("recon/{game}/en/main/{address}.c")]) {
        if root.join(&file).exists() {
            crate::process::run(&["git", "rm", "-q", "-f", &file], root)?;
        }
    }

    if let Some(name) = new_name.filter(|n| *n != label) {
        let files = crate::process::run(&["git", "grep", "-l", "-w", &label, "--", "games", "recon"], root).unwrap_or_default();
        let mut count = 0;
        for file in files.lines().filter(|f| !f.is_empty()) {
            let path = root.join(file);
            let text = read(&path)?;
            let renamed = rename_word(&text, &label, &name);
            if renamed != text {
                write(&path, &renamed)?;
                count += 1;
            }
        }
        println!("renamed {label} to {name} in {count} files");
    }
    println!("adopted {address} in all six {game} editions; build all twelve to confirm");
    Ok(())
}

fn rename_word(text: &str, from: &str, to: &str) -> String {
    let is_word = |c: char| c.is_ascii_alphanumeric() || c == '_';
    let mut out = String::with_capacity(text.len());
    let mut rest = text;
    while let Some(at) = rest.find(from) {
        let before = rest[..at].chars().next_back();
        let after = rest[at + from.len()..].chars().next();
        out.push_str(&rest[..at]);
        if before.is_some_and(is_word) || after.is_some_and(is_word) {
            out.push_str(from);
        } else {
            out.push_str(to);
        }
        rest = &rest[at + from.len()..];
    }
    out.push_str(rest);
    out
}

#[cfg(test)]
mod tests {
    use super::*;

    const SCAFFOLD: &str = "@ h\n\t.section .rom.00000100, \"ax\"\n\t.global A\nA:\n\t.incbin \"baserom.gba\", 0x00000100, 0x00000010\n\t.section .rom.00000110, \"ax\"\n\t.incbin \"baserom.gba\", 0x00000110, 0x00000002\n\t.section .rom.00000112, \"ax\"\n\t.global B\n\t.thumb_func\nB:\n\t.incbin \"baserom.gba\", 0x00000112, 0x00000020\n";

    #[test]
    fn removes_the_function_section_and_its_padding() {
        let (text, names) = remove_section(SCAFFOLD, "B", true).unwrap();
        assert_eq!(names, ["00000112", "00000110"]);
        assert_eq!(text, "@ h\n\t.section .rom.00000100, \"ax\"\n\t.global A\nA:\n\t.incbin \"baserom.gba\", 0x00000100, 0x00000010\n");
        assert!(remove_section(SCAFFOLD, "Missing", false).is_err());
    }

    #[test]
    fn an_unlinked_module_takes_the_function_sections_line() {
        let script = "    \"*/games/g/a.o\"(.text)\n    \"*/recon/g/de/rom.o\"(.rom.00000110)\n    \"*/recon/g/de/rom.o\"(.rom.00000112)\n    \"*/recon/g/de/rom.o\"(.rom.00000200)\n";
        let sections = ["00000112".to_string(), "00000110".to_string()];
        assert_eq!(unlink_sections(script, &sections, None).unwrap(), "    \"*/games/g/a.o\"(.text)\n    \"*/recon/g/de/rom.o\"(.rom.00000200)\n");
        assert_eq!(
            unlink_sections(script, &sections, Some("\"*/games/g/b.o\"(.text)")).unwrap(),
            "    \"*/games/g/a.o\"(.text)\n    \"*/games/g/b.o\"(.text)\n    \"*/recon/g/de/rom.o\"(.rom.00000200)\n"
        );
        assert!(unlink_sections(script, &["00000300".to_string()], None).is_err());
        // What follows a function whose listing ended in padding is aligned.
        let rest = ["00000112".to_string()];
        assert_eq!(next_scaffold_section(script, "recon/g/de/rom.o", &sections).as_deref(), Some("00000200"));
        assert_eq!(next_scaffold_section(script, "recon/g/de/rom.o", &["00000200".to_string()]), None);
        assert_eq!(next_scaffold_section("    \"*/recon/g/de/rom.o\"(.rom.00000112)\n    \"*/games/g/a.o\"(.text)\n", "recon/g/de/rom.o", &rest), None);
        let aligned = align_section(SCAFFOLD, "00000112");
        assert!(aligned.contains("\t.section .rom.00000112, \"ax\"\n\t.balign 4\n\t.global B\n"), "{aligned}");
        assert_eq!(align_section(&aligned, "00000112"), aligned);
        let listing = ".syntax unified\n\t.thumb\n\t.global F\n\t.thumb_func\nF:\n\tbx lr\n\t.2byte 0x0000\n";
        assert!(padding_tail(listing) && !padding_tail(&format!("{listing}.L0:\n\t.4byte 0x00000000\n")));
        let start = align_listing(listing);
        assert!(start.starts_with(".syntax unified\n\t.thumb\n\t.balign 4\n\t.global F\n"), "{start}");
        assert_eq!(align_listing(&start), start);
        assert!(padding_listing(".syntax unified\n\t.thumb\n\t.2byte 0x0000\n"));
        assert!(!padding_listing(".syntax unified\n\t.thumb\n\t.2byte 0x0000\n\t.2byte 0x0000\n"));
        assert_eq!(module_object(Path::new("/r"), Path::new("/r/games/g/src/b.c")).unwrap(), "\"*/games/g/src/b.o\"");
        assert_eq!(module_object(Path::new("/r"), Path::new("games/g/src/b.c")).unwrap(), "\"*/games/g/src/b.o\"");
        assert!(module_object(Path::new("/r"), Path::new("recon/g/en/main/b.c")).is_err());
        assert!(module_object(Path::new("/r"), Path::new("games/g/src/b.s")).is_err());
    }

    /// An edition whose scaffold holds the function unnamed, between a
    /// named neighbour and the code after it, two bytes of padding before it.
    #[test]
    fn carves_an_unlabelled_copy_out_of_its_section() {
        use crate::label::tests::function;
        let english = function(10, 1, 0x40, 0x0801_0000);
        let copy = function(10, 1, 0x58, 0x0801_0400);
        let length = copy.len() as u32;
        let mut rom = vec![0xffu8; 0x11e];
        rom.extend([0, 0]);
        rom.extend(&copy);
        rom.extend([0xff; 0x40]);
        let scaffold = format!(
            "\t.section .rom.00000100, \"ax\"\n\t.global Before\n\t.type Before, %function\n\t.thumb_func\nBefore:\n\t.incbin \"baserom.gba\", 0x00000100, 0x{:08x}\n",
            0x20 + length + 0x40
        );
        let names = HashMap::from([("Before".to_string(), 0x0800_00f0)]);
        let reference = Reference { bytes: english, base: 0x0800_0110, arm: false, names };
        let carved = carve(&reference, &scaffold, &rom, "Run", (false, false)).unwrap();
        assert_eq!(carved.note, "relocated copy at 0x120, cut from .rom.00000100");
        let after = 0x120 + length;
        assert_eq!(
            carved.cut.text,
            format!("\t.section .rom.00000100, \"ax\"\n\t.global Before\n\t.type Before, %function\n\t.thumb_func\nBefore:\n\t.incbin \"baserom.gba\", 0x00000100, 0x00000020\n\t.section .rom.{after:08x}, \"ax\"\n\t.incbin \"baserom.gba\", 0x{after:08x}, 0x00000040\n")
        );
        let script = "    \"*/recon/g/de/rom.o\"(.rom.00000100)\n    \"*/games/g/a.o\"(.text)\n";
        let relinked = crate::adopt_data::relink_parts(script, "recon/g/de/rom.o", ".rom.", &carved.cut, Some("\"*/games/g/b.o\"(.text)")).unwrap();
        assert_eq!(relinked, format!("    \"*/recon/g/de/rom.o\"(.rom.00000100)\n    \"*/games/g/b.o\"(.text)\n    \"*/recon/g/de/rom.o\"(.rom.{after:08x})\n    \"*/games/g/a.o\"(.text)\n"));
        let linked = crate::adopt_data::relink_parts(script, "recon/g/de/rom.o", ".rom.", &carved.cut, None).unwrap();
        assert_eq!(linked, format!("    \"*/recon/g/de/rom.o\"(.rom.00000100)\n    \"*/recon/g/de/rom.o\"(.rom.{after:08x})\n    \"*/games/g/a.o\"(.text)\n"));
        // The padding English's listing held leaves with the copy.
        let padded = carve(&reference, &scaffold, &rom, "Run", (true, false)).unwrap();
        assert!(padded.cut.text.contains("0x00000100, 0x0000001e\n\t.section"), "{}", padded.cut.text);
        assert!(carve(&reference, &scaffold, &rom, "Run", (false, true)).unwrap_err().contains("no two zero bytes"));
        // A scaffold that names the function elsewhere, or holds no copy, stops.
        assert!(carve(&reference, &scaffold, &rom, "Before", (false, false)).unwrap_err().contains("names Before at 0x100"));
        let other = Reference { bytes: function(10, 3, 0x40, 0), ..reference };
        assert!(carve(&other, &scaffold, &rom, "Run", (false, false)).unwrap_err().contains("no identical or relocated copy"));
        // A label and its .size line inside the copy leave with it.
        let named = scaffold.replace("0x00000100, ", "0x00000100, 0x00000020\n\t.global Run\nRun:\n\t.incbin \"baserom.gba\", 0x00000120, ").replace(&format!("0x{:08x}\n", 0x20 + length + 0x40), &format!("0x{:08x}\n\t.size Run, .-Run\n", length + 0x40));
        let known = carve(&other_named(&copy), &named, &rom, "Run", (false, false)).unwrap();
        assert_eq!(known.cut.labels, ["Run"]);
        assert!(!known.cut.text.contains("Run"), "{}", known.cut.text);
    }

    fn other_named(bytes: &[u8]) -> Reference {
        Reference { bytes: bytes.to_vec(), base: 0x0800_0110, arm: false, names: HashMap::new() }
    }

    #[test]
    fn renames_whole_words_only() {
        assert_eq!(rename_word("Func_08 Func_080 xFunc_08", "Func_08", "Run"), "Run Func_080 xFunc_08");
    }
}
