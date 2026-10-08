use object::{Object, ObjectSection, ObjectSymbol, SectionFlags};
use crate::decode::{decode_one, Kind};
use std::collections::{BTreeMap, BTreeSet};

const REMEDY: &str = "call main-image code through the overlay's own IMPORT.S veneers";

pub(crate) fn check(elf: &[u8], map: &str) -> Result<(), String> {
    let image = read(elf)?;
    let mut problems = linker_stubs(map);
    problems.extend(outside_branches(&image));
    if problems.is_empty() {
        return Ok(());
    }
    let range = image
        .sections
        .iter()
        .map(|section| {
            format!(
                "{:#010x}-{:#010x}",
                section.address,
                section.address + section.size
            )
        })
        .collect::<Vec<_>>()
        .join(", ");
    Err(format!(
        "overlay code leaves its image {range}:\n  {}\n{REMEDY}",
        problems.join("\n  ")
    ))
}

struct Section {
    address: u32,
    size: u32,
    bytes: Vec<u8>,
}

struct Image {
    sections: Vec<Section>,
    marks: BTreeMap<u32, u8>,
    names: BTreeMap<u32, String>,
}

fn mapping(name: &str) -> Option<u8> {
    let rest = name.strip_prefix('$')?.as_bytes();
    let kind = *rest.first()?;
    (matches!(kind, b'a' | b't' | b'd') && rest.get(1).is_none_or(|next| *next == b'.'))
        .then_some(kind)
}

fn read(elf: &[u8]) -> Result<Image, String> {
    let file = object::File::parse(elf).map_err(|error| error.to_string())?;
    let mut sections = Vec::new();
    let mut allocated = BTreeSet::new();
    for section in file.sections() {
        let SectionFlags::Elf { sh_flags } = section.flags() else {
            continue;
        };
        if sh_flags & u64::from(object::elf::SHF_ALLOC) == 0 || section.size() == 0 {
            continue;
        }
        allocated.insert(section.index().0);
        sections.push(Section {
            address: section.address() as u32,
            size: section.size() as u32,
            bytes: section.data().map_err(|error| error.to_string())?.to_vec(),
        });
    }
    let mut marks = BTreeMap::new();
    let mut names = BTreeMap::new();
    for symbol in file.symbols() {
        let Ok(name) = symbol.name() else {
            continue;
        };
        let address = symbol.address() as u32;
        match mapping(name) {
            Some(kind) => {
                if symbol
                    .section_index()
                    .is_some_and(|index| allocated.contains(&index.0))
                {
                    let mark = marks.entry(address).or_insert(kind);
                    if *mark == b'd' {
                        *mark = kind;
                    }
                }
            }
            None if !symbol.is_undefined()
                && name.starts_with(|first: char| first.is_ascii_alphabetic() || first == '_') =>
            {
                names.entry(address & !1).or_insert_with(|| name.to_owned());
            }
            None => {}
        }
    }
    Ok(Image {
        sections,
        marks,
        names,
    })
}

/// The modern build's gate (`alchemy build modern`): its compiler's runtime
/// is ARM code, so the linker's interworking glue inside an overlay is
/// expected, but no long-branch veneer and no branch may leave the image.
pub(crate) fn check_modern(elf: &[u8]) -> Result<(), String> {
    let image = read(elf)?;
    let file = object::File::parse(elf).map_err(|error| error.to_string())?;
    let mut problems: Vec<String> = file
        .symbols()
        .filter_map(|symbol| symbol.name().ok().map(str::to_owned))
        .filter(|name| name.ends_with("_veneer"))
        .map(|name| format!("the linker made the long-branch veneer {name}"))
        .collect();
    problems.extend(outside_branches(&image));
    if problems.is_empty() {
        Ok(())
    } else {
        Err(format!("overlay code leaves its image:\n  {}\n{REMEDY}", problems.join("\n  ")))
    }
}

fn linker_stubs(map: &str) -> Vec<String> {
    let pattern =
        regex::Regex::new(r"(?m)^ (\S+)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+) linker stubs\s*$")
            .expect("static pattern");
    pattern
        .captures_iter(map)
        .filter(|capture| capture[3].bytes().any(|digit| digit != b'0'))
        .map(|capture| {
            let address = u64::from_str_radix(&capture[2], 16).unwrap_or_default();
            let size = u64::from_str_radix(&capture[3], 16).unwrap_or_default();
            format!(
                "the linker made {} ({size:#x} bytes at {address:#010x}): a long-branch veneer or interworking glue",
                &capture[1]
            )
        })
        .collect()
}

fn outside_branches(image: &Image) -> Vec<String> {
    let inside = |target: u32| {
        image
            .sections
            .iter()
            .any(|section| target.wrapping_sub(section.address) < section.size)
    };
    let name = |address: u32| {
        image
            .names
            .range(..=address)
            .next_back()
            .map(|(start, name)| format!("{name}+{:#x} ", address - start))
            .unwrap_or_default()
    };
    let mut problems = Vec::new();
    for section in &image.sections {
        let end = section.address + section.bytes.len() as u32;
        let marks: Vec<(u32, u8)> = image
            .marks
            .range(section.address..end)
            .map(|(address, kind)| (*address, *kind))
            .collect();
        for (index, &(start, kind)) in marks.iter().enumerate() {
            let stop = marks.get(index + 1).map_or(end, |next| next.0);
            for (site, target, text) in branches(section, start, stop, kind) {
                if inside(target) {
                    continue;
                }
                let target_name = image
                    .names
                    .get(&(target & !1))
                    .map(|name| format!(" ({name})"))
                    .unwrap_or_default();
                problems.push(format!("{}({site:#010x}): {text}{target_name}", name(site)));
            }
        }
    }
    problems
}

fn branches(section: &Section, start: u32, stop: u32, kind: u8) -> Vec<(u32, u32, String)> {
    let mut found = Vec::new();
    match kind {
        b't' => {
            let mut pc = (start + 1) & !1;
            while pc + 2 <= stop {
                let Some(ins) = decode_one(&section.bytes, section.address, pc) else {
                    break;
                };
                if let Kind::B { target } | Kind::Bl { target } | Kind::Bcond { target, .. } =
                    ins.kind
                {
                    found.push((pc, target, ins.text));
                }
                pc += ins.size;
            }
        }
        b'a' => {
            let mut pc = (start + 3) & !3;
            while pc + 4 <= stop {
                let at = (pc - section.address) as usize;
                let Some(word) = section.bytes.get(at..at + 4) else {
                    break;
                };
                let word = u32::from_le_bytes(word.try_into().expect("four bytes"));
                if (word >> 25) & 7 == 0b101 {
                    let unconditional = word >> 28 == 0xf;
                    let offset = ((word << 8) as i32 >> 6) as u32;
                    let half = if unconditional { (word >> 23) & 2 } else { 0 };
                    let target = pc.wrapping_add(8).wrapping_add(offset).wrapping_add(half);
                    let mnemonic = match (unconditional, word & 1 << 24 != 0) {
                        (true, _) => "blx",
                        (false, true) => "bl",
                        (false, false) => "b",
                    };
                    found.push((pc, target, format!("{mnemonic} {target:#010x}")));
                }
                pc += 4;
            }
        }
        _ => {}
    }
    found
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::gate::elf::{build, Symbol, ABSOLUTE, SECTION};

    const BASE: u32 = 0x0200_8000;

    fn thumb_bl(site: u32, target: u32) -> [u8; 4] {
        let offset = target.wrapping_sub(site + 4);
        let high = 0xf000 | ((offset >> 12) & 0x7ff) as u16;
        let low = 0xf800 | ((offset >> 1) & 0x7ff) as u16;
        let mut bytes = [0; 4];
        bytes[..2].copy_from_slice(&high.to_le_bytes());
        bytes[2..].copy_from_slice(&low.to_le_bytes());
        bytes
    }

    fn put(image: &mut [u8], at: u32, bytes: &[u8]) {
        let at = (at - BASE) as usize;
        image[at..at + bytes.len()].copy_from_slice(bytes);
    }

    fn overlay() -> Vec<u8> {
        let mut image = vec![0u8; 0x40];
        put(&mut image, BASE, &thumb_bl(BASE, BASE + 0x10));
        put(&mut image, BASE + 4, &thumb_bl(BASE + 4, 0x0200_1000));
        let back = (0x0200_7ff0u32.wrapping_sub(BASE + 8 + 4) >> 1) & 0x7ff;
        put(&mut image, BASE + 8, &(0xe000 | back as u16).to_le_bytes());
        let past = ((BASE + 0x44).wrapping_sub(BASE + 0xa + 4) >> 1) & 0xff;
        put(
            &mut image,
            BASE + 0xa,
            &(0xd100 | past as u16).to_le_bytes(),
        );
        put(&mut image, BASE + 0xc, &0x4770u16.to_le_bytes());
        put(&mut image, BASE + 0x20, &thumb_bl(BASE + 0x20, 0x0200_1000));
        let arm = |site: u32, target: u32, link: bool| {
            let offset = (target.wrapping_sub(site + 8) >> 2) & 0x00ff_ffff;
            (0xea00_0000 | u32::from(link) << 24 | offset).to_le_bytes()
        };
        put(&mut image, BASE + 0x30, &arm(BASE + 0x30, BASE, false));
        put(
            &mut image,
            BASE + 0x34,
            &arm(BASE + 0x34, 0x0300_0000, true),
        );
        image
    }

    fn elf(image: &[u8]) -> Vec<u8> {
        build(
            ".text",
            BASE,
            image,
            &[
                Symbol("$t", BASE, SECTION, false),
                Symbol("$d", BASE + 0x20, SECTION, false),
                Symbol("$a", BASE + 0x30, SECTION, false),
                Symbol("$t", 0x0800_0000, ABSOLUTE, false),
                Symbol("Scene_Run", BASE, SECTION, true),
                Symbol("Scene_Arm", BASE + 0x30, SECTION, true),
                Symbol("Random16Far", 0x0200_1000, ABSOLUTE, true),
            ],
            &[],
        )
    }

    #[test]
    fn branches_out_of_the_image_fail_and_data_is_not_code() {
        let error = check(&elf(&overlay()), "").unwrap_err();
        let lines: Vec<&str> = error.lines().collect();
        assert_eq!(
            lines,
            [
                "overlay code leaves its image 0x02008000-0x02008040:",
                "  Scene_Run+0x4 (0x02008004): bl 0x02001000 (Random16Far)",
                "  Scene_Run+0x8 (0x02008008): b 0x02007ff0",
                "  Scene_Run+0xa (0x0200800a): bne 0x02008044",
                "  Scene_Arm+0x4 (0x02008034): bl 0x03000000",
                "call main-image code through the overlay's own IMPORT.S veneers",
            ]
        );
    }

    #[test]
    fn an_overlay_that_branches_only_inside_passes() {
        let mut image = overlay();
        image[4..0x0c].fill(0);
        image[0x34..0x38].fill(0);
        check(&elf(&image), "").unwrap();
    }

    #[test]
    fn linker_made_stubs_and_glue_fail_when_they_hold_bytes() {
        let map = "\
.glue_7         0x000000000200f82c        0x0
 .glue_7        0x000000000200f82c        0x0 linker stubs
.v4_bx          0x000000000200f82c        0x0
 .v4_bx         0x000000000200f82c        0x0 linker stubs
 .text.__stub   0x000000000200f82c        0x8 linker stubs
 .glue_7t_long_section_name
                0x000000000200f834        0xc linker stubs
 .text          0x0000000002008000       0x30 /out/obj/ENTRY.o
";
        assert_eq!(
            linker_stubs(map),
            [
                "the linker made .text.__stub (0x8 bytes at 0x0200f82c): a long-branch veneer or interworking glue",
                "the linker made .glue_7t_long_section_name (0xc bytes at 0x0200f834): a long-branch veneer or interworking glue",
            ]
        );
        let mut image = overlay();
        image[4..0x0c].fill(0);
        image[0x34..0x38].fill(0);
        let error = check(&elf(&image), map).unwrap_err();
        assert!(error.contains(".text.__stub"), "{error}");
        assert!(error.ends_with(REMEDY), "{error}");
    }

    #[test]
    fn mapping_symbols_are_read_with_their_suffixes() {
        assert_eq!(mapping("$t"), Some(b't'));
        assert_eq!(mapping("$a.startup"), Some(b'a'));
        assert_eq!(mapping("$d"), Some(b'd'));
        assert_eq!(mapping("$x"), None);
        assert_eq!(mapping("$tail"), None);
        assert_eq!(mapping("Scene_Run"), None);
    }
}
