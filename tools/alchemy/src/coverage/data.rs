//! DONE data (C2, Pascal 2026-10-08): every byte an image places that is not
//! code, from its first placed byte to its last. A byte is done when an object
//! built from games/ places it or when it is fill the build itself writes;
//! bytes placed from recon/ (incbin scaffolds, not-yet-C listings, their
//! overlay counterparts, drafts) are not. Each overlay counts once, in its
//! unpacked image: its compressed stream in the ROM is outside the total.

use super::calcrom::{is_listing, is_text, relative_object, sections_in_outputs};
use object::elf::{PT_LOAD, SHF_ALLOC, SHT_NOBITS};
use object::read::elf::{ElfFile32, ProgramHeader, SectionHeader};
use object::LittleEndian;
use std::collections::BTreeMap;

/// One image's data: its non-code bytes, how many are done, and the recon
/// objects that place the rest.
#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub(crate) struct Data {
    pub total: i64,
    pub done: i64,
    /// Placed code bytes, the complement of `total` within the image.
    pub code: i64,
    /// Compressed overlay streams the image places, outside the total.
    pub streams: i64,
    pub pending: BTreeMap<String, i64>,
}

/// A loaded output section of a linked image: its name, run address, load
/// address and size.
#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Loaded {
    pub name: String,
    pub address: i64,
    pub load: i64,
    pub size: i64,
}

/// The sections whose bytes a linked image holds: allocated, not NOBITS,
/// non-empty, at the load address their segment gives them.
pub(crate) fn loaded(elf: &[u8]) -> Result<Vec<Loaded>, String> {
    let file = ElfFile32::<LittleEndian>::parse(elf).map_err(|e| e.to_string())?;
    let endian = file.endian();
    let table = file.elf_section_table();
    let segments = file
        .elf_program_headers()
        .iter()
        .filter(|segment| segment.p_type(endian) == PT_LOAD)
        .map(|segment| {
            (
                i64::from(segment.p_vaddr(endian)),
                i64::from(segment.p_paddr(endian)),
                i64::from(segment.p_filesz(endian)),
            )
        })
        .collect::<Vec<_>>();
    let mut found = Vec::new();
    for header in table.iter() {
        let size = i64::from(header.sh_size(endian));
        if size == 0
            || header.sh_type(endian) == SHT_NOBITS
            || header.sh_flags(endian) & SHF_ALLOC == 0
        {
            continue;
        }
        let name = table
            .section_name(endian, header)
            .map_err(|e| e.to_string())?;
        let name = String::from_utf8_lossy(name).into_owned();
        let address = i64::from(header.sh_addr(endian));
        let (vaddr, paddr, _) = segments
            .iter()
            .find(|(vaddr, _, filesz)| address >= *vaddr && address + size <= vaddr + filesz)
            .ok_or_else(|| format!("{name}: no loaded segment holds it"))?;
        found.push(Loaded {
            name,
            address,
            load: paddr + (address - vaddr),
            size,
        });
    }
    Ok(found)
}

/// Whether an object's bytes are scaffolding rather than source.
pub(crate) fn from_recon(object: &str, output: &str, overlay: bool) -> bool {
    relative_object(object, output, overlay)
        .is_some_and(|(path, listing)| path.starts_with("recon/") || (listing && is_listing(path)))
}

/// Each overlay stream a map places, by the object whose `.overlays` input
/// section holds its `Resource_Overlay<ID>` label: id (lowercase) to object.
pub(crate) fn stream_owners(map: &str) -> BTreeMap<String, String> {
    let mut owners = BTreeMap::new();
    let mut owner: Option<&str> = None;
    for line in map.lines() {
        let fields = line.split_whitespace().collect::<Vec<_>>();
        if let Some(rest) = line.strip_prefix(" .") {
            owner = rest.strip_prefix("overlays ").and_then(|tail| {
                let (_, rest) = tail.trim_start().split_once(char::is_whitespace)?;
                let (_, object) = rest.trim_start().split_once(char::is_whitespace)?;
                Some(object.trim())
            });
            continue;
        }
        if !line.starts_with(' ') {
            owner = None;
            continue;
        }
        let (Some(object), [address, label]) = (owner, fields.as_slice()) else {
            continue;
        };
        let Some(id) = label.strip_prefix("Resource_Overlay") else {
            continue;
        };
        if address.starts_with("0x") && !id.is_empty() {
            owners.insert(id.to_ascii_lowercase(), object.to_owned());
        }
    }
    owners
}

/// Tallies one image from its linker map and its loaded sections. `streams`
/// gives each overlay stream it places, by id, with its compressed size;
/// those bytes are counted in the overlay's own image, not here.
pub(crate) fn tally(
    map: &str,
    loaded: &[Loaded],
    output: &str,
    overlay: bool,
    streams: &BTreeMap<String, i64>,
) -> Result<Data, String> {
    let owners = stream_owners(map);
    let mut packed = BTreeMap::<&str, i64>::new();
    for (id, bytes) in streams {
        let object = owners
            .get(id)
            .ok_or_else(|| format!("overlay stream {id} has no Resource_Overlay label"))?;
        *packed.entry(object.as_str()).or_default() += bytes;
    }
    let Some(start) = loaded.iter().map(|s| s.load).min() else {
        return Ok(Data::default());
    };
    let end = loaded
        .iter()
        .map(|s| s.load + s.size)
        .max()
        .unwrap_or(start);
    let names = loaded
        .iter()
        .map(|section| section.name.as_str())
        .collect::<std::collections::BTreeSet<_>>();
    let mut data = Data::default();
    let mut outside = 0;
    for (section, placed) in sections_in_outputs(map) {
        if placed.size <= 0 {
            continue;
        }
        let held = section.is_some_and(|name| names.contains(name));
        if is_text(&placed) {
            if held {
                data.code += placed.size;
            } else {
                outside += placed.size;
            }
            continue;
        }
        let stream = if held && placed.name == ".overlays" {
            packed.remove(placed.object).unwrap_or_default()
        } else {
            0
        };
        if stream > placed.size {
            return Err(format!("{}: streams exceed its section", placed.object));
        }
        data.streams += stream;
        if held && from_recon(placed.object, output, overlay) {
            let object = relative_object(placed.object, output, overlay)
                .map_or(placed.object, |(path, _)| path);
            *data.pending.entry(object.to_owned()).or_default() += placed.size - stream;
        }
    }
    if let Some(object) = packed.keys().next() {
        return Err(format!("{object}: its overlay streams lie outside the image"));
    }
    if outside > 0 {
        return Err(format!(
            "{outside} code bytes lie outside the image's loaded sections"
        ));
    }
    data.total = end - start - data.code - data.streams;
    data.done = data.total - data.pending.values().sum::<i64>();
    if data.done < 0 {
        return Err("scaffold data exceeds the image's data".into());
    }
    Ok(data)
}

#[cfg(test)]
mod tests {
    use super::*;

    const OUT: &str = "/b/out/tbs-en";

    fn map() -> String {
        [
            "Linker script and memory map",
            "",
            ".rom_start      0x0000000008000000      0x40",
            " .rodata        0x0000000008000000       0x10 /b/out/tbs-en/obj/games/x/head.o",
            " .text          0x0000000008000010       0x20 /b/out/tbs-en/obj/games/x/code.o",
            " *fill*         0x0000000008000030        0x4 ",
            " .unidentified  0x0000000008000034        0xc /b/out/tbs-en/obj/recon/tbs/unidentified.o",
            "",
            "ewram           0x0000000002000000    0x100",
            " .sym_ewram     0x0000000002000000    0x100 /b/out/tbs-en/obj/recon/tbs/sym_ewram.o",
            "",
            ".iwram_runtime  0x0000000003000000     0x10 load address 0x0000000008000040",
            " .text          0x0000000003000000      0x8 /b/out/tbs-en/obj/recon/tbs/raw/08000040.o",
            " .data          0x0000000003000008      0x8 /b/out/tbs-en/obj/recon/tbs/raw/08000040.o",
        ]
        .join("\n")
    }

    fn sections() -> Vec<Loaded> {
        vec![
            Loaded {
                name: ".rom_start".into(),
                address: 0x0800_0000,
                load: 0x0800_0000,
                size: 0x40,
            },
            Loaded {
                name: ".iwram_runtime".into(),
                address: 0x0300_0000,
                load: 0x0800_0040,
                size: 0x10,
            },
        ]
    }

    #[test]
    fn data_is_every_placed_byte_but_code_and_recon_is_not_done() {
        let data = tally(&map(), &sections(), OUT, false, &BTreeMap::new()).unwrap();
        // 0x50 bytes placed: 0x28 code, 0x28 data (head 0x10, fill 4,
        // scaffold 0xc, recon data 8); NOBITS ewram is not in the image.
        assert_eq!(data.code, 0x28);
        assert_eq!(data.total, 0x28);
        assert_eq!(data.done, 0x10 + 4);
        assert_eq!(data.pending["recon/tbs/unidentified.o"], 0xc);
        assert_eq!(data.pending["recon/tbs/raw/08000040.o"], 8);
    }

    #[test]
    fn overlay_listings_are_not_done_and_code_outside_the_image_is_an_error() {
        let map = [
            "Linker script and memory map",
            ".text           0x0000000002008000      0x20",
            " .text          0x0000000002008000       0x8 /b/out/tbs-en/obj/games/x/entry.o",
            " .rodata.x02008008",
            "                0x0000000002008008      0x18 /b/out/tbs-en/overlays/resource_649_overlay.o",
        ]
        .join("\n");
        let image = [Loaded {
            name: ".text".into(),
            address: 0x0200_8000,
            load: 0x0200_8000,
            size: 0x20,
        }];
        let data = tally(&map, &image, OUT, true, &BTreeMap::new()).unwrap();
        assert_eq!((data.total, data.done), (0x18, 0));
        assert!(tally(&map, &[], OUT, true, &BTreeMap::new()).unwrap() == Data::default());
        let other = [Loaded {
            name: ".data".into(),
            ..image[0].clone()
        }];
        assert!(tally(&map, &other, OUT, true, &BTreeMap::new()).is_err());
    }

    #[test]
    fn an_overlay_stream_leaves_the_rom_total_and_its_packer_bytes_stay() {
        let map = [
            "Linker script and memory map",
            ".rom_tail       0x0000000008000000      0x60",
            " .rodata        0x0000000008000000       0x10 /b/out/tbs-en/obj/games/x/head.o",
            " */games/x/overlays.o(.overlays)",
            " .overlays      0x0000000008000010       0x28 /b/out/tbs-en/obj/games/x/overlays.o",
            "                0x0000000008000010                Resource_Overlay36F",
            "                0x0000000008000020                Resource_Overlay370",
            " .overlays      0x0000000008000038       0x28 /b/out/tbs-en/obj/recon/tla/overlays.o",
            "                0x0000000008000038                Resource_Overlay649",
        ]
        .join("\n");
        assert_eq!(
            stream_owners(&map).into_iter().collect::<Vec<_>>(),
            [
                ("36f".to_string(), "/b/out/tbs-en/obj/games/x/overlays.o".to_string()),
                ("370".to_string(), "/b/out/tbs-en/obj/games/x/overlays.o".to_string()),
                ("649".to_string(), "/b/out/tbs-en/obj/recon/tla/overlays.o".to_string()),
            ]
        );
        let image = [Loaded {
            name: ".rom_tail".into(),
            address: 0x0800_0000,
            load: 0x0800_0000,
            size: 0x60,
        }];
        let streams = BTreeMap::from([
            ("36f".to_string(), 0xe),
            ("370".to_string(), 0x14),
            ("649".to_string(), 0x24),
        ]);
        let data = tally(&map, &image, OUT, false, &streams).unwrap();
        // 0x60 placed, 0x46 of it streams: the header, 6 bytes of packer
        // leftovers and alignment from games/, and 4 from the recon scaffold.
        assert_eq!(data.streams, 0x46);
        assert_eq!(data.total, 0x60 - 0x46);
        assert_eq!(data.pending["recon/tla/overlays.o"], 4);
        assert_eq!(data.done, 0x10 + 6);
        let unknown = BTreeMap::from([("abc".to_string(), 1)]);
        assert!(tally(&map, &image, OUT, false, &unknown).is_err());
        let oversized = BTreeMap::from([("649".to_string(), 0x29)]);
        assert!(tally(&map, &image, OUT, false, &oversized).is_err());
    }
}
