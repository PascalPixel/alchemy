use crate::compile::root;
use crate::decode::{decode_one, half_at, word_at, Kind};
use crate::elf::Elf;
use crate::ld_map::{preferred, Map, Placed, ROM};
use crate::targets::DecompTarget;
use std::collections::{BTreeMap, BTreeSet};
use std::path::Path;

#[derive(Debug, Clone, PartialEq, Eq)]
pub(crate) struct Line {
    pub address: u32,
    pub bytes: Vec<u8>,
    pub text: String,
    pub wide: bool,
}

impl Line {
}

pub(crate) type Names = BTreeMap<u32, String>;

fn named(names: &Names, value: u32) -> String {
    names
        .get(&value)
        .or_else(|| names.get(&(value & !1)).filter(|_| value & 1 == 1))
        .map_or(String::new(), |n| format!(" <{n}>"))
}

fn unknown(half: u16) -> String {
    match half >> 8 {
        0xdf => format!("swi {:#x}", half & 0xff),
        0xbe => format!("bkpt {:#x}", half & 0xff),
        _ if half & 0xf800 == 0xf800 => match half & 0x7ff {
            0 => "bl lr".to_string(),
            offset => format!("bl lr+{:#x}", offset << 1),
        },
        _ => format!(".2byte {half:#06x}"),
    }
}

pub(crate) fn thumb_lines(
    image: &[u8],
    base: u32,
    start: u32,
    end: u32,
    names: &Names,
) -> Vec<Line> {
    let mut lines = Vec::new();
    let mut pool = BTreeSet::new();
    let mut tables = BTreeSet::new();
    let at = |address: u32| address.wrapping_sub(base) as usize;
    let mut pc = start;
    while pc < end {
        if tables.remove(&pc) {
            let mut lowest = end;
            while pc + 4 <= end && pc < lowest {
                let Some(word) = word_at(image, at(pc)) else {
                    break;
                };
                if word <= pc || word >= end {
                    break;
                }
                lowest = lowest.min(word & !1);
                lines.push(Line {
                    address: pc,
                    bytes: image[at(pc)..at(pc) + 4].to_vec(),
                    text: format!(".word {word:#010x}"),
                    wide: true,
                });
                pc += 4;
            }
            continue;
        }
        if pool.contains(&pc) && pc + 4 <= end {
            if let Some(word) = word_at(image, at(pc)) {
                lines.push(Line {
                    address: pc,
                    bytes: image[at(pc)..at(pc) + 4].to_vec(),
                    text: format!(".word {word:#010x}{}", named(names, word)),
                    wide: true,
                });
                pc += 4;
                continue;
            }
        }
        let Some(half) = half_at(image, at(pc)) else {
            break;
        };
        let decoded = decode_one(image, base, pc).filter(|ins| pc + ins.size <= end);
        let (size, text) = match decoded {
            None => (2, unknown(half)),
            Some(ins) => match ins.kind {
                Kind::LdrPool { word, .. } => {
                    let target = (pc & !3) + 4 + u32::from(half & 0xff) * 4;
                    if target < end {
                        pool.insert(target);
                        if word > pc && word < end && word & 3 == 0 {
                            tables.insert(word);
                        }
                    }
                    (2, format!("{}{}", ins.text, named(names, word)))
                }
                Kind::Bl { target } | Kind::B { target } | Kind::Bcond { target, .. } => {
                    (ins.size, format!("{}{}", ins.text, named(names, target)))
                }
                Kind::Unknown(half) => (2, unknown(half)),
                Kind::SpAdjust(k) if k > 0 => (2, format!("add sp, #{k}")),
                _ => (ins.size, ins.text),
            },
        };
        lines.push(Line {
            address: pc,
            bytes: image[at(pc)..at(pc) + size as usize].to_vec(),
            text,
            wide: false,
        });
        pc += size;
    }
    lines
}

pub(crate) fn function_end(
    section: &Placed,
    start: u32,
    statics: &[u32],
    size: Option<u32>,
) -> u32 {
    let mut end = section.end();
    for &address in section.symbols.iter().map(|(a, _)| a).chain(statics) {
        if address > start && address < end {
            end = address;
        }
    }
    match size.filter(|&s| s > 0) {
        Some(size) => end.min(start + size),
        None => end,
    }
}

const STT_FUNC: u8 = 2;

fn is_mapping(name: &str) -> bool {
    name.starts_with('$')
}

pub(crate) fn resolve<'m>(
    map: &'m Map,
    elf: Option<&Elf>,
    functions: &[(u32, String)],
    wanted: &str,
) -> Result<(&'m Placed, u32, String), String> {
    if let Some((section, start)) = map.symbol(wanted) {
        return Ok((section, start, wanted.to_string()));
    }
    let in_section = |address: u32| {
        map.at_address(address)
            .ok_or_else(|| format!("{address:#010x} is in no linked section"))
    };
    let from_elf = functions
        .iter()
        .find(|(_, n)| n == wanted)
        .map(|(a, _)| *a)
        .or_else(|| {
            elf.and_then(|e| {
                e.symbols
                    .iter()
                    .find(|s| s.name == wanted && !is_mapping(&s.name))
                    .map(|s| s.value & !1)
            })
        });
    if let Some(start) = from_elf {
        return Ok((in_section(start)?, start, wanted.to_string()));
    }
    let digits = wanted.trim_start_matches("0x");
    let address = (digits.len() == 8)
        .then(|| u32::from_str_radix(digits, 16).ok())
        .flatten()
        .ok_or_else(|| format!("no symbol {wanted} in the map or the ELF"))?;
    let section = in_section(address)?;
    let start = section
        .symbols
        .iter()
        .map(|(a, _)| *a)
        .chain(
            functions
                .iter()
                .map(|(a, _)| *a)
                .filter(|&a| a >= section.address),
        )
        .filter(|&a| a <= address)
        .max()
        .unwrap_or(address);
    let name = preferred(&section.symbols, start)
        .map(str::to_string)
        .or_else(|| {
            functions
                .iter()
                .find(|(a, _)| *a == start)
                .map(|(_, n)| n.clone())
        })
        .unwrap_or_else(|| format!("{start:#010x}"));
    Ok((section, start, name))
}

struct LinkedImage {
    map: Map,
    elf: Option<Elf>,
    bytes: Vec<u8>,
    overlay_base: Option<u32>,
}

impl LinkedImage {
    fn read(directory: &Path, edition: &str, overlay: Option<&str>) -> Result<Self, String> {
        let prefix = overlay.map_or_else(
            || directory.join(edition),
            |id| directory.join("overlays").join(id),
        );
        let read = |extension: &str| {
            let path = prefix.with_extension(extension);
            std::fs::read(&path)
                .map_err(|error| format!("{}: {error}; build {edition} first", path.display()))
        };
        let map = Map::parse(&String::from_utf8_lossy(&read("map")?));
        let elf = read("elf").ok().map(|data| Elf::parse(&data)).transpose()?;
        let (bytes, overlay_base) = if overlay.is_some() {
            let elf = elf
                .as_ref()
                .ok_or("an overlay comparison needs its linked ELF")?;
            let (base, bytes) = overlay_bytes(elf)?;
            (bytes, Some(base))
        } else {
            (read("gba")?, None)
        };
        Ok(Self {
            map,
            elf,
            bytes,
            overlay_base,
        })
    }

    fn functions(&self) -> Vec<(u32, String)> {
        self.elf
            .iter()
            .flat_map(|elf| &elf.symbols)
            .filter(|symbol| {
                symbol.kind == STT_FUNC
                    && symbol.section != 0
                    && symbol.section != 0xfff1
                    && !is_mapping(&symbol.name)
            })
            .map(|symbol| (symbol.value & !1, symbol.name.clone()))
            .collect()
    }

    fn span(&self, wanted: &str) -> Result<(&Placed, u32, u32, u32, String), String> {
        let functions = self.functions();
        let (section, start, name) = resolve(&self.map, self.elf.as_ref(), &functions, wanted)?;
        let statics = functions
            .iter()
            .map(|(address, _)| *address)
            .collect::<Vec<_>>();
        let size = self.elf.as_ref().and_then(|elf| {
            elf.symbols
                .iter()
                .find(|symbol| symbol.name == name && symbol.kind == STT_FUNC)
                .map(|symbol| symbol.size)
        });
        let end = function_end(section, start, &statics, size);
        let base = match self.overlay_base {
            Some(base) => base,
            None if section.loaded && section.load >= ROM => {
                section.address.wrapping_sub(section.load - ROM)
            }
            None => {
                return Err(format!(
                    "{name} is in {}, which has no bytes in the ROM",
                    section.label()
                ))
            }
        };
        Ok((section, start, end, base, name))
    }
}

pub(crate) struct OverlayReference {
    pub bytes: Vec<u8>,
    pub start: u32,
    pub end: u32,
    pub name: String,
}

fn overlay_reference_code(built: &LinkedImage, reference: &[u8], wanted: &str) -> Result<OverlayReference, String> {
    let (section, start, end, base, name) = built.span(wanted)?;
    if !section.name.starts_with(".text") || end <= start {
        return Err(format!("{name}: no overlay code extent"));
    }
    let range = image_range(reference.len(), base, start, end)?;
    Ok(OverlayReference { bytes: reference[range].to_vec(), start, end, name })
}

/// Analysis only: the linked image supplies bounds, the ROM supplies code.
pub(crate) fn overlay_function_reference(target: DecompTarget, overlay: &str, wanted: &str) -> Result<OverlayReference, String> {
    let out = root().join(target.output_dir);
    let built = LinkedImage::read(&out, target.id.as_str(), Some(overlay))?;
    let map_path = out.join(format!("{}.map", target.id));
    let main_map = Map::parse(&std::fs::read_to_string(&map_path)
        .map_err(|error| format!("{}: {error}", map_path.display()))?);
    let rom_path = root().join(target.rom);
    let rom = std::fs::read(&rom_path).map_err(|error| format!("{}: {error}", rom_path.display()))?;
    target.verify_reference(&rom)?;
    let reference = reference_overlay(&rom, &main_map, overlay)?;
    overlay_reference_code(&built, &reference, wanted)
}

fn overlay_bytes(elf: &Elf) -> Result<(u32, Vec<u8>), String> {
    let base = elf
        .sections
        .iter()
        .map(|section| section.address)
        .min()
        .ok_or("overlay ELF has no placed bytes")?;
    let end = elf
        .sections
        .iter()
        .map(|section| {
            section
                .address
                .checked_add(section.bytes.len() as u32)
                .ok_or("overlay section address overflows")
        })
        .collect::<Result<Vec<_>, _>>()?
        .into_iter()
        .max()
        .unwrap();
    let size = end
        .checked_sub(base)
        .filter(|size| *size <= 0x100000)
        .ok_or("overlay ELF exceeds the diagnostic image limit")?;
    let mut image = vec![0; size as usize];
    for section in &elf.sections {
        let at = (section.address - base) as usize;
        image[at..at + section.bytes.len()].copy_from_slice(&section.bytes);
    }
    Ok((base, image))
}

pub(crate) fn reference_overlay(rom: &[u8], main_map: &Map, id: &str) -> Result<Vec<u8>, String> {
    let index = usize::from_str_radix(
        id.strip_prefix("resource_").ok_or("invalid overlay id")?,
        16,
    )
    .map_err(|_| "invalid overlay id")?;
    let (section, address) = main_map
        .symbol("Resource_DirectoryTable")
        .ok_or("the main map has no Resource_DirectoryTable")?;
    if !section.loaded || section.load < ROM {
        return Err("resource directory has no ROM placement".into());
    }
    let table = section
        .load
        .checked_sub(ROM)
        .and_then(|offset| offset.checked_add(address - section.address))
        .ok_or("resource directory offset overflows")? as usize;
    let word = |at: usize| {
        rom.get(at..at + 4)
            .map(|bytes| u32::from_le_bytes(bytes.try_into().unwrap()))
            .ok_or("resource directory is truncated")
    };
    if word(table + 4)? != address {
        return Err("reference resource directory disagrees with the main map".into());
    }
    let table_end = word(table + 8)?
        .checked_sub(ROM)
        .ok_or("resource directory end is outside the ROM")? as usize;
    if table_end <= table + 12 || table_end > rom.len() || !(table_end - table).is_multiple_of(4) {
        return Err("reference resource directory has invalid bounds".into());
    }
    let count = (table_end - table) / 4;
    if index >= count {
        return Err(format!("{id} is outside the reference resource directory"));
    }
    let start = word(table + 4 * index)?
        .checked_sub(ROM)
        .ok_or("overlay stream is outside the ROM")? as usize;
    if start >= rom.len() {
        return Err("overlay stream is outside the ROM".into());
    }
    let end = (0..count)
        .filter_map(|index| {
            word(table + 4 * index)
                .ok()?
                .checked_sub(ROM)
                .map(|offset| offset as usize)
        })
        .filter(|&offset| offset > start && offset <= rom.len())
        .min()
        .unwrap_or(rom.len());
    let decoded = match rom[start] {
        0 => ags::assets::lz::decode_general(rom, start, end, 0x100000),
        1 => ags::assets::lz::decode_palette(rom, start + 1, end, 0x100000),
        kind => return Err(format!("overlay stream has unsupported kind {kind}")),
    };
    let (mut decoded, _) = decoded.map_err(|error| error.0)?;
    restore_thumb_calls(&mut decoded);
    Ok(decoded)
}

fn restore_thumb_calls(block: &mut [u8]) {
    let half = |block: &[u8], at: usize| u32::from(u16::from_le_bytes([block[at], block[at + 1]]));
    let mut low_at = 2;
    while low_at + 2 <= block.len() {
        let (low, high) = (half(block, low_at), half(block, low_at - 2));
        if low >> 11 == 31 && high >> 11 == 30 {
            let stored = ((high & 0x7ff) << 12) | ((low & 0x7ff) << 1);
            let call = stored.wrapping_sub(low_at as u32) & 0x7f_fffe;
            block[low_at - 2..low_at]
                .copy_from_slice(&((0xf000 | (call >> 12)) as u16).to_le_bytes());
            block[low_at..low_at + 2]
                .copy_from_slice(&((0xf800 | ((call >> 1) & 0x7ff)) as u16).to_le_bytes());
        }
        low_at += 2;
    }
}


fn image_range(
    length: usize,
    base: u32,
    start: u32,
    end: u32,
) -> Result<std::ops::Range<usize>, String> {
    let range = start
        .checked_sub(base)
        .zip(end.checked_sub(base))
        .filter(|&(start, end)| start <= end && end as usize <= length)
        .ok_or("function runs past the end of an image")?;
    Ok(range.0 as usize..range.1 as usize)
}
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn loader_restoration_inverts_stored_calls_including_backwards_and_wrap() {
        for displacement in [0, 2, 0x1234, 0x3ffffe, 0x400000, 0x7ffffe] {
            let original = halves(&[
                0x2001,
                (0xf000 | (displacement >> 12)) as u16,
                (0xf800 | ((displacement >> 1) & 0x7ff)) as u16,
                0x4770,
            ]);
            let stored = (displacement + 4) & 0x7ffffe;
            let mut packed = halves(&[
                0x2001,
                (0xf000 | (stored >> 12)) as u16,
                (0xf800 | ((stored >> 1) & 0x7ff)) as u16,
                0x4770,
            ]);
            restore_thumb_calls(&mut packed);
            assert_eq!(packed, original);
        }
        let mut unrelated = halves(&[0x2001, 0xf800, 0xf000]);
        unrelated.push(0xab);
        let original = unrelated.clone();
        restore_thumb_calls(&mut unrelated);
        assert_eq!(unrelated, original);
    }

    #[test]
    fn overlay_image_preserves_section_gaps_and_rejects_overflow() {
        let elf = Elf {
            symbols: vec![],
            sections: vec![
                crate::elf::Section {
                    address: 0x2000,
                    bytes: vec![1, 2],
                },
                crate::elf::Section {
                    address: 0x2004,
                    bytes: vec![3, 4],
                },
            ],
        };
        assert_eq!(
            overlay_bytes(&elf).unwrap(),
            (0x2000, vec![1, 2, 0, 0, 3, 4])
        );
        let overflow = Elf {
            symbols: vec![],
            sections: vec![crate::elf::Section {
                address: u32::MAX,
                bytes: vec![1],
            }],
        };
        assert!(overlay_bytes(&overflow).is_err());
        assert!(image_range(6, 0x2000, 0x2004, 0x2008).is_err());
        assert!(image_range(6, 0x2000, 0x1ffe, 0x2002).is_err());
    }

    #[test]
    fn overlay_bounds_ignore_imported_absolute_functions() {
        let symbol = |name: &str, value, section| crate::elf::Symbol {
            name: name.into(),
            value,
            size: 0,
            kind: STT_FUNC,
            bind: 1,
            section,
        };
        let image = LinkedImage {
            map: Map::parse(""),
            bytes: vec![],
            overlay_base: Some(0x2000),
            elf: Some(Elf {
                sections: vec![],
                symbols: vec![
                    symbol("Defined", 0x2001, 1),
                    symbol("Imported", 0x2003, 0xfff1),
                    symbol("Undefined", 0, 0),
                ],
            }),
        };
        assert_eq!(image.functions(), vec![(0x2000, "Defined".into())]);
    }

    #[test]
    fn adopted_overlay_comparison_reads_native_code_and_checks_its_extent() {
        let base = 0x02008000;
        let image = LinkedImage {
            map: Map { placed: vec![Placed {
                name: ".text".into(), path: "games/tla/src/field/scene.o".into(),
                address: base, size: 8, load: base, loaded: true,
                symbols: vec![(base + 2, "Adopted".into())],
            }] },
            elf: Some(Elf { sections: vec![], symbols: vec![crate::elf::Symbol {
                name: "Adopted".into(), value: base + 3, size: 4,
                kind: STT_FUNC, bind: 1, section: 1,
            }] }),
            bytes: vec![0; 8], overlay_base: Some(base),
        };
        let reference = halves(&[0x2000, 0x2007, 0x4770, 0xffff]);
        let code = overlay_reference_code(&image, &reference, "Adopted").unwrap();
        assert_eq!((code.start, code.end, code.name.as_str()), (base + 2, base + 6, "Adopted"));
        assert_eq!(code.bytes, halves(&[0x2007, 0x4770]));
        assert!(overlay_reference_code(&image, &reference[..4], "Adopted").is_err());
        assert!(overlay_reference_code(&image, &reference, "Missing").is_err());
    }

    #[test]
    fn reference_overlay_uses_directory_bounds_with_duplicate_and_unsorted_rows() {
        let map = Map {
            placed: vec![Placed {
                name: ".rodata".into(),
                path: "games/tbs/directory.o".into(),
                address: ROM + 0x20,
                size: 24,
                load: ROM + 0x20,
                loaded: true,
                symbols: vec![(ROM + 0x20, "Resource_DirectoryTable".into())],
            }],
        };
        let payload = halves(&[0x2001, 0x4770, 0x2002, 0x4770]);
        let packed = ags::lz::compress_general(&payload, &ags::resource::PACKER).unwrap();
        let mut rom = vec![0; 0x100];
        let directory = [
            ROM,
            ROM + 0x20,
            ROM + 0x38,
            ROM + 0xc0,
            ROM + 0x80,
            ROM + 0x80,
        ];
        for (index, word) in directory.iter().enumerate() {
            rom[0x20 + index * 4..0x24 + index * 4].copy_from_slice(&word.to_le_bytes());
        }
        rom[0x80..0x80 + packed.len()].copy_from_slice(&packed);
        assert_eq!(
            reference_overlay(&rom, &map, "resource_004").unwrap(),
            payload
        );
        assert_eq!(
            reference_overlay(&rom, &map, "resource_005").unwrap(),
            payload
        );
        let palette = ags::lz::compress_tagged_palette(&payload, &ags::resource::PACKER).unwrap();
        rom[0x80..0x80 + palette.len()].copy_from_slice(&palette);
        assert_eq!(
            reference_overlay(&rom, &map, "resource_004").unwrap(),
            payload
        );
        assert!(reference_overlay(&rom, &map, "resource_006").is_err());
        rom[0x24] ^= 1;
        assert!(reference_overlay(&rom, &map, "resource_004").is_err());
    }

    fn halves(words: &[u16]) -> Vec<u8> {
        words.iter().flat_map(|h| h.to_le_bytes()).collect()
    }

    fn texts(lines: &[Line]) -> Vec<String> {
        lines
            .iter()
            .map(|l| format!("{:x} {}", l.address, l.text))
            .collect()
    }

    #[test]
    fn a_jump_table_reads_as_words() {
        let mut image = halves(&[0x4a05, 0x00bb, 0x589b, 0x469f]);
        image.extend(0x0800_0010u32.to_le_bytes());
        image.extend(0x0800_0014u32.to_le_bytes());
        image.extend(halves(&[0x2001, 0x4770, 0x2002, 0x4770]));
        image.extend(0x0800_0008u32.to_le_bytes());
        let lines = thumb_lines(&image, ROM, ROM, ROM + 0x1c, &Names::new());
        assert_eq!(
            texts(&lines),
            [
                "8000000 ldr r2, =0x8000008",
                "8000002 lsls r3, r7, #2",
                "8000004 ldr r3, [r3, r2]",
                "8000006 mov pc, r3",
                "8000008 .word 0x08000010",
                "800000c .word 0x08000014",
                "8000010 movs r0, #1",
                "8000012 bx lr",
                "8000014 movs r0, #2",
                "8000016 bx lr",
                "8000018 .word 0x08000008",
            ]
        );
    }

    #[test]
    fn an_address_stands_for_the_function_that_holds_it() {
        let map = Map::parse(crate::ld_map::tests::MAP);
        let functions = vec![(0x0800_0014, "Input_Static".to_string())];
        let found = |wanted: &str| {
            resolve(&map, None, &functions, wanted).map(|(p, a, n)| (p.label(), a, n))
        };
        let io = "games/tbs/src/system/io.o".to_string();
        assert_eq!(
            found("Input_Poll"),
            Ok((io.clone(), 0x0800_0018, "Input_Poll".to_string()))
        );
        assert_eq!(
            found("08000012"),
            Ok((io.clone(), 0x0800_0010, "Input_Handle".to_string()))
        );
        assert_eq!(
            found("0x08000016"),
            Ok((io.clone(), 0x0800_0014, "Input_Static".to_string()))
        );
        assert_eq!(
            found("Input_Static"),
            Ok((io, 0x0800_0014, "Input_Static".to_string()))
        );
        assert!(found("Missing").is_err());
    }

    #[test]
    fn a_function_ends_at_the_next_symbol_in_its_own_section() {
        let section = Placed {
            name: ".text".into(),
            path: "/x/obj/games/A.o".into(),
            address: 0x0800_0100,
            size: 0x40,
            load: 0x0800_0100,
            loaded: true,
            symbols: vec![
                (0x0800_0100, "First".into()),
                (0x0800_0120, "Second".into()),
            ],
        };
        assert_eq!(function_end(&section, 0x0800_0100, &[], None), 0x0800_0120);
        assert_eq!(
            function_end(&section, 0x0800_0120, &[0x0800_0200], None),
            0x0800_0140
        );
        assert_eq!(
            function_end(&section, 0x0800_0100, &[0x0800_0110], None),
            0x0800_0110
        );
        assert_eq!(
            function_end(&section, 0x0800_0100, &[], Some(0x18)),
            0x0800_0118
        );
    }

}
