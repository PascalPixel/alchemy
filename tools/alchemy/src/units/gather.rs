//! `alchemy unit gather`: a run of one-asset assembler wrappers into one
//! assembler module, in link order.
//!
//! A wrapper is an assembler source of one object: its header lines
//! (`.include`s of headers and `.section .rodata`), then labels with their
//! `.incbin`s and tagged PACKER bytes. The module is the wrappers' bodies
//! in order under one header. A wrapper another game's linker script still
//! links as its own object stays where it is, and the module includes it.
//! A wrapper the linker aligned gets that alignment written before it.

use crate::object_list::{self, Row};
use crate::targets::{target_for, TARGET_IDS};
use std::fs;
use std::path::Path;

pub(super) const USAGE: &str = "       alchemy unit gather GAME MODULE.s FIRST LAST
       alchemy unit gather GAME --each-map FIRST LAST
gather takes the run of rows from FIRST to LAST (objects of GAME's main
image, as its object list names them) and writes their assembler wrappers
as the one module MODULE.s, which takes their place in the list: one header
(the wrappers' header includes, once each, and .section .rodata), then each
wrapper's body in link order. A wrapper another game's linker script links
stays, and the module includes it. A wrapper whose .rodata the assembler
aligned above 1 in a built edition gets `.balign N, 0` before it, the
linker's fill until now. The gathered wrappers are removed. --each-map
gathers each map object (games/GAME/src/map/NAME/map) of the run with the
rows up to the next map into that map's own source. Build every edition
first, and compare them all afterwards.";

/// One wrapper of a run.
pub(super) struct Wrapper {
    /// Its source, from the checkout.
    pub path: String,
    pub text: String,
    /// Another game links it as its own object.
    pub shared: bool,
    /// The alignment of its .rodata as the assembler wrote it.
    pub alignment: u32,
}

/// A wrapper's header lines and its body.
fn parts(wrapper: &Wrapper) -> Result<(Vec<&str>, Vec<&str>), String> {
    let lines: Vec<&str> = wrapper.text.lines().collect();
    let header = lines
        .iter()
        .take_while(|line| {
            let line = line.trim();
            line.starts_with(".include \"games/") || line == ".section .rodata"
        })
        .count();
    let (head, body) = lines.split_at(header);
    if !head.iter().any(|line| line.trim() == ".section .rodata") {
        return Err(format!("{}: a wrapper starts with its includes and .section .rodata", wrapper.path));
    }
    if let Some(line) = body.iter().find(|line| {
        let line = line.trim();
        line.starts_with(".section") || line.starts_with(".text") || line.starts_with(".data") || line.starts_with(".include \"games/")
    }) {
        return Err(format!("{}: {:?} is no line of a wrapper's body", wrapper.path, line.trim()));
    }
    Ok((head.to_vec(), body.to_vec()))
}

/// The module that takes the wrappers' place.
pub(super) fn module(wrappers: &[Wrapper]) -> Result<String, String> {
    let mut includes: Vec<&str> = Vec::new();
    let mut body = String::new();
    for (index, wrapper) in wrappers.iter().enumerate() {
        if !wrapper.alignment.is_power_of_two() {
            return Err(format!("{}: alignment {} is no power of two", wrapper.path, wrapper.alignment));
        }
        // The module's own section takes the first wrapper's alignment.
        let balign = format!("\t.balign {}, 0\n", wrapper.alignment);
        if index > 0 && wrapper.alignment > 1 && !body.ends_with(&balign) {
            body += &balign;
        }
        if wrapper.shared {
            body += &format!("\t.include \"{}\"\n", wrapper.path);
            continue;
        }
        let (head, lines) = parts(wrapper)?;
        for line in head.into_iter().filter(|line| line.trim() != ".section .rodata") {
            if !includes.contains(&line) {
                includes.push(line);
            }
        }
        // A wrapper under games/common that no other game links comes into
        // the module with its assets named as games/common's.
        let common = wrapper.path.starts_with("games/common/");
        for line in lines {
            match line.trim().strip_prefix(".incbin \"").filter(|_| common) {
                Some(name) if name.starts_with("graphics/") || name.starts_with("map/") => {
                    body += &line.replacen(".incbin \"", ".incbin \"common/", 1);
                }
                Some(name) if !name.starts_with("common/") => {
                    return Err(format!("{}: {:?} is no asset of games/common the module can name", wrapper.path, line.trim()));
                }
                _ => body += line,
            }
            body.push('\n');
        }
    }
    let mut text = String::new();
    for line in includes {
        text += line;
        text.push('\n');
    }
    Ok(text + "\t.section .rodata\n" + &body)
}

/// The list with the rows of a run replaced by the one row of its module.
pub(super) fn gathered_rows(rows: &[Row], run: std::ops::RangeInclusive<usize>, module: &str) -> Result<Vec<Row>, String> {
    let first = &rows[*run.start()];
    for row in &rows[run.clone()] {
        if row.image != object_list::MAIN || row.place != first.place || row.sections != ".rodata" || row.editions.len() != object_list::LANGUAGES.len() {
            return Err(format!("{} is no .rodata row of all six editions in {}", row.object, first.place));
        }
        if rows.iter().filter(|other| other.object == row.object).count() != 1 {
            return Err(format!("{} has several rows", row.object));
        }
    }
    let mut found = rows[..*run.start()].to_vec();
    found.push(Row { object: module.to_owned(), ..first.clone() });
    found.extend_from_slice(&rows[*run.end() + 1..]);
    Ok(found)
}

/// The alignment of a relocatable object's .rodata, and whether it is the
/// object's only section with bytes.
fn rodata_alignment(bytes: &[u8]) -> Option<u32> {
    let half = |at: usize| bytes.get(at..at + 2).map(|b| usize::from(u16::from_le_bytes([b[0], b[1]])));
    let word = |at: usize| bytes.get(at..at + 4).map(|b| u32::from_le_bytes([b[0], b[1], b[2], b[3]]) as usize);
    if bytes.get(..5)? != b"\x7fELF\x01" {
        return None;
    }
    let (table, size, count, names) = (word(0x20)?, half(0x2e)?, half(0x30)?, half(0x32)?);
    let strings = word(table + names * size + 0x10)?;
    let mut alignment = None;
    for index in 0..count {
        let header = table + index * size;
        let name = strings + word(header)?;
        let name = bytes.get(name..)?.split(|byte| *byte == 0).next()?;
        // SHT_PROGBITS with SHF_ALLOC.
        if word(header + 4)? != 1 || word(header + 8)? & 2 == 0 {
            continue;
        }
        if name == b".rodata" {
            alignment = Some(word(header + 0x20)? as u32);
        } else if word(header + 0x14)? != 0 {
            return None;
        }
    }
    alignment
}

fn read(root: &Path, path: &str) -> Result<String, String> {
    fs::read_to_string(root.join(path)).map_err(|error| format!("{path}: {error}"))
}

fn wrapper(root: &Path, game: &str, object: &str, others: &str) -> Result<Wrapper, String> {
    let path = format!("{object}.s");
    let text = read(root, &path)?;
    let mut alignment = 1;
    for id in TARGET_IDS.iter().filter(|id| id.as_str().starts_with(game)) {
        let built = Path::new(target_for(*id).output_dir).join("obj").join(format!("{object}.o"));
        let bytes = fs::read(root.join(&built)).map_err(|error| format!("{}: {error}; build every edition first", built.display()))?;
        let found = rodata_alignment(&bytes).ok_or_else(|| format!("{}: not an object of .rodata alone", built.display()))?;
        alignment = alignment.max(found);
    }
    Ok(Wrapper { shared: others.contains(&format!("/{object}.o\"")), path, text, alignment })
}

/// The linker scripts of the other games: what they still link by name.
fn other_scripts(root: &Path, game: &str) -> Result<String, String> {
    let listed = crate::process::run(&["git", "ls-files", "--", "*.ld"], root)?;
    let own = [format!("games/{game}/"), format!("recon/{game}/")];
    let mut text = String::new();
    for path in listed.lines().filter(|path| !own.iter().any(|prefix| path.starts_with(prefix))) {
        text += &read(root, path)?;
    }
    Ok(text)
}

pub(super) fn run(arguments: &[String]) -> Result<(), String> {
    let (game, each_map, named, first, last) = match arguments {
        [game, flag, first, last] if flag == "--each-map" => (game, true, None, first, last),
        [game, module, first, last] if module.ends_with(".s") => (game, false, Some(module.as_str()), first, last),
        _ => return Err(format!("usage:\n{USAGE}")),
    };
    let root = crate::compiler::routing::root();
    let list = crate::targets::object_list_path(game).ok_or_else(|| format!("{game} has no object list"))?;
    let mut rows = object_list::parse(&read(root, &list.to_string_lossy())?)?;
    let others = other_scripts(root, game)?;
    let at = |rows: &[Row], object: &str| {
        rows.iter().position(|row| row.image == object_list::MAIN && row.object == object).ok_or_else(|| format!("{object} is no row of the main image"))
    };
    let (start, end) = (at(&rows, first)?, at(&rows, last)?);
    if start > end {
        return Err(format!("{first} is linked after {last}"));
    }
    // Each run as its objects and the module that takes their place.
    let mut runs: Vec<(Vec<String>, String)> = Vec::new();
    let map = |object: &str| object.starts_with(&format!("games/{game}/src/map/")) && object.ends_with("/map");
    for row in &rows[start..=end] {
        match (each_map, runs.last_mut()) {
            (true, _) if map(&row.object) => runs.push((vec![row.object.clone()], format!("{}.s", row.object))),
            (true, None) => return Err(format!("{} is no map to start a run", row.object)),
            (false, None) => runs.push((vec![row.object.clone()], named.expect("a module").to_owned())),
            (_, Some(run)) => run.0.push(row.object.clone()),
        }
    }
    let (mut modules, mut removed, mut aligned) = (0, 0, Vec::new());
    for (objects, module_path) in runs.iter().filter(|(objects, _)| objects.len() > 1) {
        let wrappers = objects.iter().map(|object| wrapper(root, game, object, &others)).collect::<Result<Vec<_>, _>>()?;
        let text = module(&wrappers)?;
        let (start, end) = (at(&rows, &objects[0])?, at(&rows, &objects[objects.len() - 1])?);
        rows = gathered_rows(&rows, start..=end, module_path.trim_end_matches(".s"))?;
        for wrapper in wrappers.iter().filter(|wrapper| !wrapper.shared && wrapper.path != *module_path) {
            fs::remove_file(root.join(&wrapper.path)).map_err(|error| format!("{}: {error}", wrapper.path))?;
            // A folder that held only wrappers goes with them.
            if let Some(folder) = Path::new(&wrapper.path).parent() {
                let _ = fs::remove_dir(root.join(folder));
            }
            removed += 1;
        }
        aligned.extend(wrappers.iter().skip(1).filter(|wrapper| wrapper.alignment > 1).map(|wrapper| format!("{} {}", wrapper.path, wrapper.alignment)));
        let path = root.join(module_path);
        fs::create_dir_all(path.parent().expect("a module's folder")).map_err(|error| error.to_string())?;
        fs::write(&path, text).map_err(|error| format!("{module_path}: {error}"))?;
        modules += 1;
    }
    fs::write(root.join(&list), object_list::format(&rows)).map_err(|error| format!("{}: {error}", list.display()))?;
    println!("{modules} modules written, {removed} wrappers removed, {} rows in {}", rows.len(), list.display());
    for line in aligned {
        println!("aligned\t{line}");
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    fn wrapper(path: &str, text: &str, shared: bool, alignment: u32) -> Wrapper {
        Wrapper { path: path.into(), text: text.into(), shared, alignment }
    }

    #[test]
    fn wrappers_become_one_module_in_link_order() {
        let conditional = "\t.include \"games/common/include/game/ed_asm.h\"\n\t.section .rodata\n\t.global Title_Sun\nTitle_Sun:\n\t.incbin \"graphics/title/sun.parts.plz\"\n\t.ifeq EDITION_INTERNATIONAL\n\t.byte 0x43\t@ PACKER: left in the buffer by Ui_Tiles+0x1177\n\t.else\n\t.byte 0x44\t@ PACKER: left in the buffer by Ui_Tiles+0x1177\n\t.endif\n";
        let packer = "\t.section .rodata\n\t.global Title_Peak\nTitle_Peak:\n\t.incbin \"graphics/title/peak.4bpp.lz\"\n\t.byte 0xff, 0x00\t@ PACKER: left in the buffer by Title_Sun+0x14e\n";
        let aligned = "\t.include \"games/tbs/src/map/map.inc\"\n\t.include \"games/common/include/game/ed_asm.h\"\n\t.section .rodata\n\t.global Field_Map\nField_Map:\n\tmap_block Field_Map, .LField_Map_GRID\n.LField_Map_GRID:\n\t.incbin \"map/grid.parts.lz\"\n\t.balign 4, 0\n";
        let shared = "\t.include \"games/common/include/game/ed_asm.h\"\n\t.section .rodata\n\t.global Ui_Font\nUi_Font:\n\t.incbin \"graphics/ui/font.1bpp\"\n";
        let text = module(&[
            wrapper("games/tbs/src/graphics/title/sun.s", conditional, false, 1),
            wrapper("games/tbs/src/graphics/title/peak.s", packer, false, 1),
            wrapper("games/tbs/src/map/a/map.s", aligned, false, 4),
            wrapper("games/common/src/graphics/ui/font.s", shared, true, 4),
        ])
        .unwrap();
        assert_eq!(
            text,
            "\t.include \"games/common/include/game/ed_asm.h\"\n\
             \t.include \"games/tbs/src/map/map.inc\"\n\
             \t.section .rodata\n\
             \t.global Title_Sun\nTitle_Sun:\n\t.incbin \"graphics/title/sun.parts.plz\"\n\
             \t.ifeq EDITION_INTERNATIONAL\n\t.byte 0x43\t@ PACKER: left in the buffer by Ui_Tiles+0x1177\n\
             \t.else\n\t.byte 0x44\t@ PACKER: left in the buffer by Ui_Tiles+0x1177\n\t.endif\n\
             \t.global Title_Peak\nTitle_Peak:\n\t.incbin \"graphics/title/peak.4bpp.lz\"\n\
             \t.byte 0xff, 0x00\t@ PACKER: left in the buffer by Title_Sun+0x14e\n\
             \t.balign 4, 0\n\
             \t.global Field_Map\nField_Map:\n\tmap_block Field_Map, .LField_Map_GRID\n.LField_Map_GRID:\n\
             \t.incbin \"map/grid.parts.lz\"\n\t.balign 4, 0\n\
             \t.include \"games/common/src/graphics/ui/font.s\"\n"
        );
        // An alignment the previous wrapper already ends with is not repeated,
        // and the first wrapper's is the module's own section's.
        let first = module(&[wrapper("games/tbs/src/map/a/map.s", aligned, false, 4)]).unwrap();
        assert!(!first.contains("\t.section .rodata\n\t.balign"));
        // A wrapper of games/common no other game links comes in whole, its
        // asset still games/common's.
        let own = module(&[wrapper("games/common/src/graphics/ui/font.s", shared, false, 1)]).unwrap();
        assert!(own.contains("Ui_Font:\n\t.incbin \"common/graphics/ui/font.1bpp\"\n"));
        // A body that changes section or includes a source is no wrapper's.
        let two = format!("{packer}\t.section .data\n");
        assert!(module(&[wrapper("a.s", &two, false, 1)]).unwrap_err().contains("no line of a wrapper's body"));
        assert!(module(&[wrapper("a.s", "Label:\n", false, 1)]).unwrap_err().contains("starts with its includes"));
    }

    #[test]
    fn the_module_takes_the_runs_rows() {
        let list = format!(
            "{}\nmain\trom_tail\ta/directory\t.rodata\nmain\trom_tail\ta/sun\t.rodata\nmain\trom_tail\ta/peak\t.rodata\nmain\trom_tail\ta/map\t.rodata\nmain\trom_tail\ta/last\t.rodata\tja\n",
            object_list::HEADER
        );
        let rows = object_list::parse(&list).unwrap();
        let found = gathered_rows(&rows, 1..=3, "a/title").unwrap();
        assert_eq!(
            object_list::format(&found),
            format!("{}\nmain\trom_tail\ta/directory\t.rodata\nmain\trom_tail\ta/title\t.rodata\nmain\trom_tail\ta/last\t.rodata\tja\n", object_list::HEADER)
        );
        assert!(gathered_rows(&rows, 3..=4, "a/title").unwrap_err().contains("a/last is no .rodata row of all six editions"));
    }

    #[test]
    fn an_objects_rodata_alignment_is_read_from_its_section_header() {
        // An ELF32 header, one string table and one .rodata of alignment 4.
        let names = b"\0.rodata\0.shstrtab\0";
        let mut bytes = vec![0u8; 0x34];
        bytes[..5].copy_from_slice(b"\x7fELF\x01");
        bytes.extend_from_slice(names);
        bytes.resize(0x50, 0);
        let table = bytes.len() as u32;
        bytes[0x20..0x24].copy_from_slice(&table.to_le_bytes());
        bytes[0x2e..0x30].copy_from_slice(&40u16.to_le_bytes());
        bytes[0x30..0x32].copy_from_slice(&3u16.to_le_bytes());
        bytes[0x32..0x34].copy_from_slice(&2u16.to_le_bytes());
        let section = |name: u32, kind: u32, flags: u32, offset: u32, size: u32, alignment: u32| {
            let mut header = Vec::new();
            for word in [name, kind, flags, 0, offset, size, 0, 0, alignment, 0] {
                header.extend_from_slice(&word.to_le_bytes());
            }
            header
        };
        bytes.extend(section(0, 0, 0, 0, 0, 0));
        bytes.extend(section(1, 1, 2, 0x34, 8, 4));
        bytes.extend(section(9, 3, 0, 0x34, names.len() as u32, 1));
        assert_eq!(rodata_alignment(&bytes), Some(4));
        assert_eq!(rodata_alignment(b"not an object"), None);
    }
}
