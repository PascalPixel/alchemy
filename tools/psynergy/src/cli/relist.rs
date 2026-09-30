//! `psynergy relist`: regenerate a linked image's not-yet-C listings in pret
//! shape, one labelled piece per function, with every branch target and
//! every word that points into ROM or RAM written as a symbol.

use psynergy::elf::Elf;
use psynergy::relist::{
    self, scaffold, Entry, Image, Input, Name, Part, Pieces, Region, RegionKind, Segment,
};
use std::collections::BTreeSet;
use std::fmt::Write as _;
use std::fs;
use std::path::{Path, PathBuf};

pub const USAGE: &str = "usage: psynergy relist --image FILE --elf FILE --map FILE --listings DIR --script FILE [options]\n\
Reads a linked image (its bytes, its ELF symbols and link map) and regenerates,\n\
in place, every listing the map places from DIR (DIR/NAME.s; NAME.S listings\n\
are kept as written) so that each function is its own labelled piece,\n\
splitting the linker script's entries and the scaffolds to match.\n\
  --object NAME    regenerate only DIR/NAME.s, one section per piece: its\n\
                   .text sections are code, its .rodata sections data words\n\
  --base ADDR      where the image's first byte runs (default 0x08000000)\n\
  --spell ADDR     what a new placeholder spells for that byte (default ADDR)\n\
  --rom FILE       the cartridge image whose extent is ROM (default --image)\n\
  --foreign-map FILE  the link map of the image whose names are absolute here\n\
  --root DIR      where DIR, the script and the scaffolds are (default .)\n\
  --incbin FILE    an .incbin scaffold of data that may hold code (repeatable)\n\
  --space FILE     a .space RAM scaffold that may take labels (repeatable)\n\
  --objects DIR    object files whose references keep names (repeatable)\n\
  --report         print what would change and write nothing\n\
  --explain ADDR   print how the flow of a function at ADDR reads";

struct Options {
    image: PathBuf,
    elf: PathBuf,
    map: PathBuf,
    listings: String,
    script: String,
    root: PathBuf,
    incbin: Vec<String>,
    space: Vec<String>,
    objects: Vec<PathBuf>,
    object: Option<String>,
    base: u32,
    spell: Option<u32>,
    rom: Option<PathBuf>,
    foreign: Option<PathBuf>,
    report: bool,
    explain: Option<u32>,
}

fn address(flag: &str, value: &str) -> Result<u32, String> {
    u32::from_str_radix(value.trim_start_matches("0x"), 16)
        .map_err(|_| format!("{flag} wants a hexadecimal address: {value}"))
}

fn parse(arguments: &[String]) -> Result<Options, String> {
    let mut options = Options {
        image: PathBuf::new(),
        elf: PathBuf::new(),
        map: PathBuf::new(),
        listings: String::new(),
        script: String::new(),
        root: PathBuf::from("."),
        incbin: Vec::new(),
        space: Vec::new(),
        objects: Vec::new(),
        object: None,
        base: 0x0800_0000,
        spell: None,
        rom: None,
        foreign: None,
        report: false,
        explain: None,
    };
    let mut i = 0;
    while i < arguments.len() {
        let flag = arguments[i].as_str();
        if flag == "--report" {
            options.report = true;
            i += 1;
            continue;
        }
        let value = arguments
            .get(i + 1)
            .ok_or_else(|| format!("{flag} needs a value\n{USAGE}"))?
            .clone();
        match flag {
            "--image" => options.image = PathBuf::from(value),
            "--elf" => options.elf = PathBuf::from(value),
            "--map" => options.map = PathBuf::from(value),
            "--listings" => options.listings = value.trim_end_matches('/').to_string(),
            "--script" => options.script = value,
            "--root" => options.root = PathBuf::from(value),
            "--incbin" => options.incbin.push(value),
            "--space" => options.space.push(value),
            "--objects" => options.objects.push(PathBuf::from(value)),
            "--object" => options.object = Some(value),
            "--base" => options.base = address(flag, &value)?,
            "--spell" => options.spell = Some(address(flag, &value)?),
            "--rom" => options.rom = Some(PathBuf::from(value)),
            "--foreign-map" => options.foreign = Some(PathBuf::from(value)),
            "--explain" => options.explain = Some(address(flag, &value)?),
            _ => return Err(format!("unknown option {flag}\n{USAGE}")),
        }
        i += 2;
    }
    for (value, flag) in [
        (options.image.as_os_str().is_empty(), "--image"),
        (options.elf.as_os_str().is_empty(), "--elf"),
        (options.map.as_os_str().is_empty(), "--map"),
        (options.listings.is_empty(), "--listings"),
        (
            options.script.is_empty() && options.explain.is_none() && !options.report,
            "--script",
        ),
    ] {
        if value {
            return Err(format!("{flag} is required\n{USAGE}"));
        }
    }
    Ok(options)
}

fn read(path: &Path) -> Result<Vec<u8>, String> {
    fs::read(path).map_err(|error| format!("{}: {error}", path.display()))
}

fn read_text(path: &Path) -> Result<String, String> {
    String::from_utf8(read(path)?).map_err(|error| format!("{}: {error}", path.display()))
}

/// The source stem an object path names: the part after `/obj/`, without
/// its `.o`.
fn stem(object: &str) -> Option<&str> {
    let at = object.find("/obj/")?;
    object[at + 5..].strip_suffix(".o")
}

/// The ELF's defined global names: those of sections, and the absolute
/// ones the linker script gives a value.
pub fn names(elf: &Elf) -> Vec<Name> {
    const ABSOLUTE: u16 = 0xfff1;
    elf.symbols
        .iter()
        .filter(|symbol| {
            matches!(symbol.binding, 1 | 2)
                && symbol.section != 0
                && (symbol.section < 0xff00 || symbol.section == ABSOLUTE)
                && !matches!(symbol.kind, 3 | 4)
                && !symbol.name.is_empty()
                && !symbol.name.starts_with(['$', '.'])
        })
        .map(|symbol| {
            let absolute = symbol.section == ABSOLUTE;
            let thumb = (symbol.kind == 2 && symbol.value & 1 == 1) || symbol.kind == 13;
            Name {
                name: symbol.name.clone(),
                address: if thumb {
                    symbol.value & !1
                } else {
                    symbol.value
                },
                thumb,
                absolute,
            }
        })
        .collect()
}

/// The file names in `directory`, spelled exactly: `NAME.s` and `NAME.S` are
/// different sources even on a case-insensitive file system.
fn file_names(directory: &Path) -> BTreeSet<String> {
    fs::read_dir(directory)
        .map(|entries| {
            entries
                .flatten()
                .filter(|entry| entry.path().is_file())
                .map(|entry| entry.file_name().to_string_lossy().into_owned())
                .collect()
        })
        .unwrap_or_default()
}

fn scaffold_stem(file: &str) -> &str {
    file.strip_suffix(".s").unwrap_or(file)
}

fn regions(options: &Options, map: &str) -> Vec<Region> {
    let root = &options.root;
    let listing_files = file_names(&root.join(&options.listings));
    let mut regions: Vec<Region> = relist::placed_sections(map)
        .into_iter()
        .filter_map(|placed| {
            // The one listing object whose sections are pieces.
            if let Some(object) = &options.object {
                if Path::new(&placed.object)
                    .file_name()
                    .is_some_and(|leaf| leaf.to_string_lossy() == format!("{object}.o"))
                {
                    let kind = if placed.section.starts_with(".text") {
                        RegionKind::Listing { regenerate: true }
                    } else if placed.section.starts_with(".rodata") {
                        RegionKind::Words
                    } else {
                        RegionKind::Other
                    };
                    return Some(Region {
                        start: placed.address,
                        end: placed.address + placed.size,
                        kind,
                        object: object.clone(),
                        section: placed.section,
                    });
                }
            }
            // Objects from outside the build tree, such as the compiler's
            // library members, take bytes too.
            let Some(stem) = stem(&placed.object) else {
                return Some(Region {
                    start: placed.address,
                    end: placed.address + placed.size,
                    kind: RegionKind::Other,
                    object: placed.object.clone(),
                    section: placed.section,
                });
            };
            let path = Path::new(stem);
            let listing =
                options.object.is_none() && path.parent() == Some(Path::new(&options.listings));
            let leaf = path.file_name()?.to_string_lossy().into_owned();
            let position =
                |files: &[String]| files.iter().position(|file| scaffold_stem(file) == stem);
            // C is maintained code; so is the assembly beside a listing
            // linked alone, its veneers, whose pointers are its entries.
            let sources: &[&str] = if options.object.is_some() {
                &["C", "c", "S"]
            } else {
                &["C", "c"]
            };
            let kind = if listing {
                RegionKind::Listing {
                    regenerate: listing_files.contains(&format!("{leaf}.s"))
                        && placed.section == ".text",
                }
            } else if let Some(scaffold) = position(&options.incbin) {
                RegionKind::Incbin { scaffold }
            } else if let Some(scaffold) = position(&options.space) {
                RegionKind::Space { scaffold }
            } else if placed.section.starts_with(".text")
                && sources
                    .iter()
                    .any(|extension| root.join(format!("{stem}.{extension}")).is_file())
            {
                RegionKind::Code
            } else {
                RegionKind::Other
            };
            Some(Region {
                start: placed.address,
                end: placed.address + placed.size,
                kind,
                object: if listing { leaf } else { stem.to_string() },
                section: placed.section,
            })
        })
        .collect();
    // The ROM copies of sections that run elsewhere, where they take bytes
    // no other region does: named only by the script's LOADADDR symbols.
    for line in map.lines() {
        let fields: Vec<&str> = line.split_whitespace().collect();
        let [name, _, size, "load", "address", load] = fields.as_slice() else {
            continue;
        };
        let (Some(size), Some(load)) = (hex(size), hex(load)) else {
            continue;
        };
        let (start, end) = (load, load + size);
        if size == 0
            || regions
                .iter()
                .any(|region| region.start < end && start < region.end)
        {
            continue;
        }
        regions.push(Region {
            start,
            end,
            kind: RegionKind::Other,
            object: format!("LOADADDR({name})"),
            section: (*name).to_string(),
        });
    }
    regions.sort_by_key(|region| (region.start, region.end));
    // A listing placed after an object that ends off a word boundary was
    // aligned by the linker; it takes the padding as its own bytes.
    for index in 1..regions.len() {
        let before = regions[index - 1].end;
        let region = &mut regions[index];
        if matches!(
            region.kind,
            RegionKind::Listing { regenerate: true } | RegionKind::Words
        ) && before < region.start
            && region.start - before < 4
        {
            region.start = before;
        }
    }
    regions
}

/// Another image's placed sections, in address order, none overlapping.
fn foreign(map: &str) -> Vec<Region> {
    let mut placed = relist::placed_sections(map);
    placed.sort_by_key(|placed| (placed.address, placed.size));
    let mut regions: Vec<Region> = Vec::new();
    for placed in placed {
        if regions.last().is_some_and(|last| placed.address < last.end) {
            continue;
        }
        regions.push(Region {
            start: placed.address,
            end: placed.address + placed.size,
            kind: RegionKind::Other,
            object: placed.object,
            section: placed.section,
        });
    }
    regions
}

fn hex(field: &str) -> Option<u32> {
    u64::from_str_radix(field.strip_prefix("0x")?, 16)
        .ok()
        .and_then(|value| u32::try_from(value).ok())
}

fn maps_in(directory: &Path, found: &mut Vec<PathBuf>) {
    let Ok(entries) = fs::read_dir(directory) else {
        return;
    };
    for entry in entries.flatten() {
        let path = entry.path();
        if path.is_dir() {
            maps_in(&path, found);
        } else if path.extension().is_some_and(|extension| extension == "map") {
            found.push(path);
        }
    }
}

fn objects_in(directory: &Path, found: &mut Vec<PathBuf>) {
    let Ok(entries) = fs::read_dir(directory) else {
        return;
    };
    for entry in entries.flatten() {
        let path = entry.path();
        if path.is_dir() {
            objects_in(&path, found);
        } else if path.extension().is_some_and(|extension| extension == "o") {
            found.push(path);
        }
    }
}

/// The names an object refers to without defining.
fn undefined(object: &Path) -> Result<Vec<String>, String> {
    let bytes = read(object)?;
    let Ok(elf) = Elf::parse(&bytes) else {
        return Ok(Vec::new());
    };
    Ok(elf
        .symbols
        .iter()
        .filter(|symbol| symbol.section == 0 && symbol.binding != 0 && !symbol.name.is_empty())
        .map(|symbol| symbol.name.clone())
        .collect())
}

/// The names that objects refer to without defining, skipping the objects
/// of listings being regenerated. One listing object is linked alone: only
/// the other objects its map places refer to it.
fn external(
    options: &Options,
    map: &str,
    regenerated: &BTreeSet<String>,
) -> Result<BTreeSet<String>, String> {
    if let Some(object) = &options.object {
        let mut names = BTreeSet::new();
        let placed: BTreeSet<String> = relist::placed_sections(map)
            .into_iter()
            .map(|placed| placed.object)
            .collect();
        for path in placed.iter().map(Path::new) {
            let own = path
                .file_name()
                .is_some_and(|leaf| leaf.to_string_lossy() == format!("{object}.o"));
            if own || !path.is_file() {
                continue;
            }
            names.extend(undefined(path)?);
        }
        return Ok(names);
    }
    let mut objects = Vec::new();
    for directory in &options.objects {
        objects_in(directory, &mut objects);
    }
    // Only objects a link placed count: a build tree keeps stale ones.
    let mut maps = vec![options.map.clone()];
    for directory in &options.objects {
        maps_in(directory, &mut maps);
    }
    let mut linked = BTreeSet::new();
    for map in &maps {
        let text = read_text(map)?;
        linked.extend(
            relist::placed_sections(&text)
                .into_iter()
                .map(|placed| placed.object),
        );
    }
    let mut names = BTreeSet::new();
    for object in objects {
        let absolute =
            fs::canonicalize(&object).map_err(|error| format!("{}: {error}", object.display()))?;
        if !linked.contains(absolute.to_string_lossy().as_ref()) {
            continue;
        }
        let text = object.to_string_lossy();
        let listing = stem(&text).is_some_and(|stem| {
            Path::new(stem).parent() == Some(Path::new(&options.listings))
                && Path::new(stem)
                    .file_name()
                    .is_some_and(|leaf| regenerated.contains(leaf.to_string_lossy().as_ref()))
        });
        if listing {
            continue;
        }
        names.extend(undefined(&object)?);
    }
    Ok(names)
}

/// The sections of a listing object that a relisting does not regenerate:
/// every `.section` but `.text*` and `.rodata*`, with its lines as written.
fn other_sections(listing: &str) -> String {
    let mut kept = String::new();
    let mut keeping = false;
    for line in listing.lines() {
        if let Some(rest) = line.trim_start().strip_prefix(".section") {
            let name = rest.trim().split([',', ' ']).next().unwrap_or("");
            keeping = !name.starts_with(".text") && !name.starts_with(".rodata");
        }
        if keeping {
            kept.push_str(line);
            kept.push('\n');
        }
    }
    kept
}

/// Replace each area's linker-script entries with its new ones.
fn patch_script(
    script: &str,
    options: &Options,
    replacements: &[(Vec<Entry>, Vec<Entry>)],
) -> Result<String, String> {
    let needle = |entry: &Entry| match entry {
        Entry::Listing { object, section } => match &options.object {
            Some(_) => format!("\"*/{object}.o\"({section})"),
            None => format!("\"*/{}/{object}.o\"(", options.listings),
        },
        Entry::Incbin { scaffold, section } => format!(
            "\"*/{}.o\"({section})",
            scaffold_stem(&options.incbin[*scaffold])
        ),
    };
    let mut lines: Vec<String> = script.lines().map(str::to_string).collect();
    for (old, new) in replacements {
        let first = needle(&old[0]);
        let at = lines
            .iter()
            .position(|line| line.contains(&first))
            .ok_or_else(|| format!("the script has no entry {first}"))?;
        for (offset, entry) in old.iter().enumerate() {
            let expected = needle(entry);
            if !lines
                .get(at + offset)
                .is_some_and(|line| line.contains(&expected))
            {
                return Err(format!(
                    "the script's entries for {first} are not consecutive at {expected}"
                ));
            }
        }
        let indent: String = lines[at]
            .chars()
            .take_while(|c| c.is_whitespace())
            .collect();
        let fresh: Vec<String> = new
            .iter()
            .map(|entry| match (entry, &options.object) {
                (Entry::Listing { object, .. }, None) => {
                    format!("{indent}\"*/{}/{object}.o\"(.text)", options.listings)
                }
                _ => format!("{indent}{}", needle(entry)),
            })
            .collect();
        lines.splice(at..at + old.len(), fresh);
    }
    Ok(lines.join("\n") + "\n")
}

pub fn run(arguments: &[String]) -> Result<String, String> {
    let options = parse(arguments)?;
    let bytes = read(&options.image)?;
    let elf_bytes = read(&options.elf)?;
    let elf =
        Elf::parse(&elf_bytes).map_err(|error| format!("{}: {error}", options.elf.display()))?;
    let map = read_text(&options.map)?;
    let image = Image {
        bytes: &bytes,
        base: options.base,
    };
    let rom_size = match &options.rom {
        Some(rom) => fs::metadata(rom)
            .map_err(|error| format!("{}: {error}", rom.display()))?
            .len() as u32,
        None => bytes.len() as u32,
    };
    let regions = regions(&options, &map);
    let regenerated: BTreeSet<String> = regions
        .iter()
        .filter(|region| region.kind == RegionKind::Listing { regenerate: true })
        .map(|region| region.object.clone())
        .collect();
    let load = |files: &[String]| -> Result<Vec<scaffold::Scaffold>, String> {
        files
            .iter()
            .map(|file| {
                let path = options.root.join(file);
                scaffold::parse(&read_text(&path)?)
                    .map_err(|error| format!("{}: {error}", path.display()))
            })
            .collect()
    };
    let input = Input {
        image,
        names: names(&elf),
        external: external(&options, &map, &regenerated)?,
        incbin: load(&options.incbin)?,
        space: load(&options.space)?,
        regions,
        rom_end: 0x0800_0000 + rom_size,
        foreign: match &options.foreign {
            Some(path) => foreign(&read_text(path)?),
            None => Vec::new(),
        },
        spell: options.spell.unwrap_or(options.base),
        pieces: match &options.object {
            Some(object) => Pieces::Sections {
                object: object.clone(),
            },
            None => Pieces::Files,
        },
    };
    let mut text = String::new();
    if let Some(address) = options.explain {
        let plan = relist::plan(image, &input.regions, &input.names)?;
        let region = input
            .regions
            .iter()
            .find(|region| region.start <= address && address < region.end)
            .ok_or_else(|| format!("{address:08x} is in no placed section"))?;
        let limit = plan
            .areas
            .iter()
            .find(|planned| planned.area.start <= address && address < planned.area.end())
            .map_or(region.end, |planned| planned.area.end());
        match relist::trace(image, address, limit, &plan.entries) {
            Ok(function) => {
                let _ = writeln!(
                    text,
                    "function {:08x}-{:08x} in {} terminated={}",
                    function.start, function.end, region.object, function.terminated
                );
                for (at, ins) in &function.instructions {
                    let _ = writeln!(text, "{at:08x}: {}", ins.text);
                }
                for (word, case) in &function.tables {
                    let _ = writeln!(text, "table {word:08x} -> {case:08x}");
                }
                for start in plan.entries.starts.range(address + 1..limit) {
                    let firm = if plan.firm.contains(start) {
                        " firm"
                    } else {
                        ""
                    };
                    let _ = writeln!(text, "entry {start:08x}{firm}");
                }
            }
            Err((at, reason)) => {
                let _ = writeln!(text, "not a function: {reason} at {at:08x}");
            }
        }
        return Ok(text);
    }
    let output = relist::relist(&input)?;
    let functions = output.plan.functions().count();
    let (mut listing_data, mut carved) = (0u32, 0u32);
    for planned in &output.plan.areas {
        for segment in &planned.segments {
            match segment {
                Segment::Data {
                    start,
                    end,
                    part: Part::Code | Part::Words,
                } => listing_data += end - start,
                Segment::Function(function) => {
                    let mut at = function.start;
                    while at < function.end {
                        if planned.area.part(at).2 == Part::Included {
                            carved += 2;
                        }
                        at += 2;
                    }
                }
                Segment::Data { .. } => {}
            }
        }
    }
    let _ = writeln!(
        text,
        "pieces={} functions={functions} far_jumps={} inner_entries={} listing_data_bytes={listing_data} code_from_included_bytes={carved} script_changes={} notes={}",
        output.pieces.len(),
        output.plan.entries.far.len(),
        output.plan.inner.len(),
        output.script.len(),
        output.notes.len(),
    );
    if options.report {
        for note in &output.notes {
            let _ = writeln!(text, "note\t{note}");
        }
        for planned in &output.plan.areas {
            for segment in &planned.segments {
                let (kind, start, end) = match segment {
                    Segment::Function(function) => ("function", function.start, function.end),
                    Segment::Data {
                        start,
                        end,
                        part: Part::Code | Part::Words,
                    } => ("listing-data", *start, *end),
                    Segment::Data { start, end, .. } => ("data", *start, *end),
                };
                if kind != "data" {
                    let _ = writeln!(text, "{kind}\t{start:08x}\t{end:08x}\t{}", end - start);
                }
            }
        }
        return Ok(text);
    }
    // Write: the new pieces replace every regenerated listing.
    let directory = options.root.join(&options.listings);
    if let Some(object) = &options.object {
        // One listing object: each piece is its own section of it, and its
        // other sections, such as reserved RAM, stay as written.
        const HEADER: &str = ".syntax unified\n\t.thumb\n";
        let path = directory.join(format!("{object}.s"));
        let kept = other_sections(&read_text(&path)?);
        let mut source = String::from(HEADER);
        for (start, text) in &output.pieces {
            let words = output.words.contains(start);
            let flags = if words { "a" } else { "ax" };
            let _ = writeln!(
                source,
                "\t.section {},\"{flags}\",%progbits",
                relist::section(*start, words)
            );
            source.push_str(text.strip_prefix(HEADER).unwrap_or(text));
        }
        source.push_str(&kept);
        fs::write(&path, source).map_err(|error| format!("{}: {error}", path.display()))?;
    } else {
        for stem in &regenerated {
            let path = directory.join(format!("{stem}.s"));
            fs::remove_file(&path).map_err(|error| format!("{}: {error}", path.display()))?;
        }
        for (start, source) in &output.pieces {
            let path = directory.join(format!("{start:08x}.s"));
            fs::write(&path, source).map_err(|error| format!("{}: {error}", path.display()))?;
        }
    }
    for (file, scaffold) in options.incbin.iter().zip(&output.incbin) {
        let path = options.root.join(file);
        fs::write(&path, scaffold::render(scaffold))
            .map_err(|error| format!("{}: {error}", path.display()))?;
    }
    for (file, scaffold) in options.space.iter().zip(&output.space) {
        let path = options.root.join(file);
        fs::write(&path, scaffold::render(scaffold))
            .map_err(|error| format!("{}: {error}", path.display()))?;
    }
    let script_path = options.root.join(&options.script);
    let script = patch_script(&read_text(&script_path)?, &options, &output.script)?;
    fs::write(&script_path, script)
        .map_err(|error| format!("{}: {error}", script_path.display()))?;
    for note in &output.notes {
        let _ = writeln!(text, "note\t{note}");
    }
    Ok(text)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn a_relisting_keeps_the_reserved_ram_of_its_listing() {
        let listing = ".syntax unified\n\t.thumb\n\t.section .text.x02008000,\"ax\",%progbits\n\tbx lr\n\t.section .bss,\"aw\",%nobits\n\t.space 58\n\t.global gSprites\ngSprites:\n\t.space 216\n\t.section .rodata,\"a\",%progbits\n\t.4byte 0\n";
        assert_eq!(
            other_sections(listing),
            "\t.section .bss,\"aw\",%nobits\n\t.space 58\n\t.global gSprites\ngSprites:\n\t.space 216\n"
        );
    }
}
