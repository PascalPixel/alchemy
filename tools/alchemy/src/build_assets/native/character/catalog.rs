//! The runtime character catalog: one 20-byte descriptor per sprite id, the
//! frame directories those descriptors name and the shared sprite palette.
//! The Broken Seal ends each directory with a null slot; The Lost Age packs
//! its directories back to back, so a directory ends where the next begins.
use super::*;
use crate::compiler::routing::CompilerTarget;

pub(in crate::build_assets::native) const DESCRIPTOR_SIZE: usize = 20;
/// Fourteen sixteen-colour banks: every sprite index stays below 0xe0.
pub(in crate::build_assets::native) const PALETTE_COLORS: usize = 224;

/// Where one edition keeps its descriptor table and sprite palette.
#[derive(Debug, Clone, Copy)]
pub(in crate::build_assets::native) struct Catalog {
    pub game: CompilerTarget,
    pub table: usize,
    pub count: usize,
    pub palette: usize,
}

/// Registered from each edition's descriptor lookup: The Broken Seal reads
/// the table through `Func_08185000`; The Lost Age through `0x08021918`,
/// which clamps the id below 716 (the table ends at the first animation
/// table, `0x083037f0`). Editions whose lookup has not been read stay
/// unregistered rather than guessed.
pub(in crate::build_assets::native) fn catalog(target: &DecompTarget) -> Result<Catalog, String> {
    let (table, count, palette) = match target.id {
        DecompTargetId::TbsJa => (0x0817_c024, 512, 0x0800_779c),
        DecompTargetId::TbsEn => (0x0818_5024, 512, 0x0800_779c),
        DecompTargetId::TlaJa | DecompTargetId::TlaEn => (0x0830_0000, 716, 0x0801_7b10),
        DecompTargetId::TlaDe
        | DecompTargetId::TlaEs
        | DecompTargetId::TlaFr
        | DecompTargetId::TlaIt => (0x0830_0000, 716, 0x0801_7b3c),
        _ => {
            return Err(format!(
                "{} character catalog address is not registered",
                target.id
            ))
        }
    };
    Ok(Catalog {
        game: target.compiler,
        table,
        count,
        palette,
    })
}

#[derive(Debug, Clone)]
pub(in crate::build_assets::native) struct Descriptor {
    pub id: usize,
    pub address: usize,
    pub width: usize,
    pub height: usize,
    pub scale: u16,
    pub draw_kind: u8,
    pub animation_count: usize,
    pub adjustment: (i8, i8),
    pub anchor: (i8, u8),
    pub frame_codec: u8,
    pub reserved: u8,
    pub frame_directory: usize,
    pub animation_table: usize,
}

fn word(rom: &[u8], address: usize) -> Result<usize, String> {
    let start = address
        .checked_sub(ROM_BASE)
        .ok_or_else(|| format!("{address:#x} precedes ROM"))?;
    let bytes = rom
        .get(start..start + 4)
        .ok_or_else(|| format!("{address:#x} lies beyond ROM"))?;
    Ok(u32::from_le_bytes(bytes.try_into().unwrap()) as usize)
}

impl Catalog {
    pub fn descriptors(&self, rom: &[u8]) -> Result<Vec<Descriptor>, String> {
        let start = self.table - ROM_BASE;
        let table = rom
            .get(start..start + self.count * DESCRIPTOR_SIZE)
            .ok_or("character descriptor table outside ROM")?;
        table
            .chunks_exact(DESCRIPTOR_SIZE)
            .enumerate()
            .map(|(id, row)| {
                let descriptor = Descriptor {
                    id,
                    address: self.table + id * DESCRIPTOR_SIZE,
                    width: row[0].into(),
                    height: row[1].into(),
                    scale: u16::from_le_bytes([row[2], row[3]]),
                    draw_kind: row[4],
                    animation_count: row[5].into(),
                    adjustment: (row[6] as i8, row[7] as i8),
                    anchor: (row[8] as i8, row[9]),
                    frame_codec: row[10],
                    reserved: row[11],
                    frame_directory: u32::from_le_bytes(row[12..16].try_into().unwrap()) as usize,
                    animation_table: u32::from_le_bytes(row[16..20].try_into().unwrap()) as usize,
                };
                if descriptor.reserved != 0 || descriptor.frame_codec > 3 {
                    return Err(format!("descriptor {id} reserved or codec field differs"));
                }
                Ok(descriptor)
            })
            .collect()
    }

    /// The 224 BGR555 colours every character frame indexes.
    pub fn palette(&self, rom: &[u8]) -> Result<Vec<[u8; 3]>, String> {
        let start = self.palette - ROM_BASE;
        let bytes = rom
            .get(start..start + PALETTE_COLORS * 2)
            .ok_or("sprite palette outside ROM")?;
        Ok(bytes
            .chunks_exact(2)
            .map(|pair| {
                let color = u16::from_le_bytes([pair[0], pair[1]]);
                let channel = |shift: u16| {
                    let value = ((color >> shift) & 31) as u8;
                    value << 3 | value >> 2
                };
                [channel(0), channel(5), channel(10)]
            })
            .collect())
    }

    /// Every descriptor's frame directory slots. A Broken Seal directory runs
    /// to its null slot. A Lost Age directory runs to the next directory; the
    /// last one runs to the first animation script, which the animation tables
    /// (packed back to back before the directories) name.
    pub fn directories(
        &self,
        rom: &[u8],
        descriptors: &[Descriptor],
    ) -> Result<BTreeMap<usize, Vec<usize>>, String> {
        let starts = descriptors
            .iter()
            .map(|d| d.frame_directory)
            .filter(|d| *d != 0)
            .collect::<BTreeSet<_>>();
        let mut directories = BTreeMap::new();
        match self.game {
            CompilerTarget::Tbs => {
                for start in starts {
                    let mut slots = vec![];
                    loop {
                        let slot = word(rom, start + slots.len() * 4)?;
                        if slot == 0 {
                            break;
                        }
                        slots.push(slot);
                        if slots.len() > 4096 {
                            return Err(format!("directory {start:#x} lacks its null slot"));
                        }
                    }
                    directories.insert(start, slots);
                }
            }
            CompilerTarget::Tla => {
                let scripts = self.animation_scripts(rom, descriptors)?;
                let ordered = starts.iter().copied().collect::<Vec<_>>();
                for (index, start) in ordered.iter().enumerate() {
                    let end = match ordered.get(index + 1) {
                        Some(next) => *next,
                        None => *scripts
                            .range(start + 4..)
                            .next()
                            .ok_or("last frame directory has no following animation script")?,
                    };
                    if (end - start) % 4 != 0 || end == *start {
                        return Err(format!("directory {start:#x} extent is not whole slots"));
                    }
                    let slots = (*start..end)
                        .step_by(4)
                        .map(|address| word(rom, address))
                        .collect::<Result<Vec<_>, _>>()?;
                    if slots.iter().any(|slot| *slot < ROM_BASE) {
                        return Err(format!("directory {start:#x} holds a non-ROM slot"));
                    }
                    directories.insert(*start, slots);
                }
            }
        }
        Ok(directories)
    }

    fn animation_scripts(
        &self,
        rom: &[u8],
        descriptors: &[Descriptor],
    ) -> Result<BTreeSet<usize>, String> {
        let mut scripts = BTreeSet::new();
        for descriptor in descriptors {
            for index in 0..descriptor.animation_count {
                let script = word(rom, descriptor.animation_table + index * 4)?;
                if script != 0 {
                    scripts.insert(script);
                }
            }
        }
        Ok(scripts)
    }
}

/// How a descriptor's `frame_codec` stores one frame, by game.
pub(in crate::build_assets::native) fn frame_codec(
    game: CompilerTarget,
    codec: u8,
) -> Option<&'static str> {
    match (game, codec) {
        (_, 0) => Some("zero-skip"),
        (_, 1) => Some("golden-sun-tagged-lz/indexed-bytes"),
        (CompilerTarget::Tbs, 3) => Some("golden-sun-general-lz/zero-skip"),
        (CompilerTarget::Tla, 3) => Some("golden-sun-arena-lz/zero-skip"),
        _ => None,
    }
}

/// Decode one frame at `pointer` into `width * height` sprite indices.
pub(in crate::build_assets::native) fn frame(
    rom: &[u8],
    game: CompilerTarget,
    codec: u8,
    pointer: usize,
    width: usize,
    height: usize,
) -> Result<Vec<u8>, String> {
    read_frame(rom, game, codec, pointer, width, height).map(|(pixels, _)| pixels)
}

fn read_frame(
    rom: &[u8],
    game: CompilerTarget,
    codec: u8,
    pointer: usize,
    width: usize,
    height: usize,
) -> Result<(Vec<u8>, usize), String> {
    let start = pointer
        .checked_sub(ROM_BASE)
        .filter(|start| *start < rom.len())
        .ok_or_else(|| format!("frame {pointer:#x} lies outside ROM"))?;
    let size = width * height;
    // A lone terminator is a blank frame: the runtime stops drawing there.
    let skipped = |bytes: &[u8]| -> Result<Vec<u8>, String> {
        if bytes == [0] {
            return Ok(vec![0; size]);
        }
        zero_skip(bytes, size).map_err(|e| format!("frame {pointer:#x}: {e}"))
    };
    let name =
        frame_codec(game, codec).ok_or_else(|| format!("frame codec {codec} is unregistered"))?;
    let lz = |e: psynergy::assets::AssetError| format!("frame {pointer:#x}: {e}");
    let (pixels, consumed) = match name {
        "zero-skip" => {
            let end = rom[start..]
                .iter()
                .position(|byte| *byte == 0)
                .ok_or("zero-skip frame lacks terminator")?;
            (skipped(&rom[start..=start + end])?, end + 1)
        }
        "golden-sun-general-lz/zero-skip" => {
            let (bytes, size) =
                crate::build_assets::derive_index::tagged_extent(rom, start, rom.len())?;
            (skipped(&bytes)?, size)
        }
        "golden-sun-arena-lz/zero-skip" => {
            let (bytes, size, _) = psynergy::assets::lz::decode_arena(rom, start).map_err(lz)?;
            (skipped(&bytes)?, size)
        }
        // The tag byte selects the stream: 0 general, 1 palette, 2 MTF4.
        _ => crate::build_assets::derive_index::tagged_extent(rom, start, rom.len())?,
    };
    if pixels.len() != size
        || pixels
            .iter()
            .any(|pixel| usize::from(*pixel) >= PALETTE_COLORS)
    {
        return Err(format!("frame {pointer:#x} extent or indices differ"));
    }
    Ok((pixels, consumed))
}

/// Metadata only: catalog consumers establish sprite roles; successful frame
/// decoding establishes extents, not reconstructed-source or DONE credit.
pub(in crate::build_assets) fn inventory(
    rom: &[u8],
    target: &DecompTarget,
) -> Result<(Vec<Value>, Vec<Value>), String> {
    let catalog = catalog(target)?;
    let descriptors = catalog.descriptors(rom)?;
    let directories = catalog.directories(rom, &descriptors)?;
    let mut rows = vec![
        json!({"start":catalog.table,"end":catalog.table + catalog.count * DESCRIPTOR_SIZE,"kind":"record-table","label":"Sprite descriptors","evidence":"runtime character descriptor lookup"}),
    ];
    let mut failures = vec![];
    let mut seen = BTreeSet::new();
    for descriptor in descriptors {
        if descriptor.animation_count > 0 {
            rows.push(json!({"start":descriptor.animation_table,"end":descriptor.animation_table + descriptor.animation_count * 4,"kind":"pointer-table","label":"Sprite animation directory","evidence":format!("descriptor {}", descriptor.id)}));
        }
        let Some(slots) = directories.get(&descriptor.frame_directory) else {
            continue;
        };
        rows.push(json!({"start":descriptor.frame_directory,"end":descriptor.frame_directory + slots.len() * 4,"kind":"pointer-table","label":"Sprite frame directory","evidence":format!("descriptor {}", descriptor.id)}));
        for &pointer in slots {
            if !seen.insert((
                pointer,
                descriptor.frame_codec,
                descriptor.width,
                descriptor.height,
            )) {
                continue;
            }
            match read_frame(rom, catalog.game, descriptor.frame_codec, pointer, descriptor.width, descriptor.height) {
                Ok((_, size)) => rows.push(json!({"start":pointer,"end":pointer+size,"kind":"golden-sun-static-sprite-series","label":"Sprite frame","evidence":format!("descriptor {}, codec {}, {}x{} decoded pixels",descriptor.id,descriptor.frame_codec,descriptor.width,descriptor.height)})),
                Err(error) => failures.push(json!({"address":pointer,"descriptor":descriptor.id,"reason":error})),
            }
        }
    }
    Ok((rows, failures))
}

/// A sheet of `columns` frames per row, frames in directory order.
pub(in crate::build_assets::native) fn sheet(
    frames: &[Vec<u8>],
    width: usize,
    height: usize,
    columns: usize,
) -> (usize, usize, Vec<u8>) {
    let rows = frames.len().div_ceil(columns).max(1);
    let sheet_width = width * columns.min(frames.len().max(1));
    let mut pixels = vec![0; sheet_width * rows * height];
    for (index, frame) in frames.iter().enumerate() {
        for y in 0..height {
            let dst = (index / columns * height + y) * sheet_width + index % columns * width;
            pixels[dst..dst + width].copy_from_slice(&frame[y * width..(y + 1) * width]);
        }
    }
    (sheet_width, rows * height, pixels)
}

/// An indexed PNG coloured with the ROM sprite palette, index 0 transparent.
pub(in crate::build_assets::native) fn preview(
    pixels: &[u8],
    width: usize,
    height: usize,
    palette: &[[u8; 3]],
) -> Result<Vec<u8>, String> {
    let mut data = vec![];
    {
        let mut encoder = png::Encoder::new(&mut data, width as u32, height as u32);
        encoder.set_color(png::ColorType::Indexed);
        encoder.set_depth(png::BitDepth::Eight);
        encoder.set_palette(palette.concat());
        encoder.set_trns(vec![0]);
        encoder
            .write_header()
            .map_err(|e| e.to_string())?
            .write_image_data(pixels)
            .map_err(|e| e.to_string())?;
    }
    Ok(data)
}

#[test]
fn packed_directories_end_at_the_next_directory_or_first_script() {
    // Two descriptors, one animation each, directories packed back to back.
    let mut rom = vec![0u8; 0x100];
    let put = |rom: &mut Vec<u8>, at: usize, value: u32| {
        rom[at..at + 4].copy_from_slice(&value.to_le_bytes());
    };
    for (id, (directory, animation)) in [(0x60usize, 0x40usize), (0x68, 0x44)].iter().enumerate() {
        let row = id * DESCRIPTOR_SIZE;
        rom[row] = 32;
        rom[row + 1] = 32;
        rom[row + 5] = 1;
        rom[row + 10] = 0;
        put(&mut rom, row + 12, (ROM_BASE + directory) as u32);
        put(&mut rom, row + 16, (ROM_BASE + animation) as u32);
    }
    put(&mut rom, 0x40, (ROM_BASE + 0x70) as u32);
    put(&mut rom, 0x44, (ROM_BASE + 0x74) as u32);
    for (slot, frame) in [(0x60, 0x80), (0x64, 0x84), (0x68, 0x88), (0x6c, 0x8c)] {
        put(&mut rom, slot, (ROM_BASE + frame) as u32);
    }
    let catalog = Catalog {
        game: CompilerTarget::Tla,
        table: ROM_BASE,
        count: 2,
        palette: ROM_BASE,
    };
    let descriptors = catalog.descriptors(&rom).unwrap();
    let directories = catalog.directories(&rom, &descriptors).unwrap();
    assert_eq!(directories[&(ROM_BASE + 0x60)].len(), 2);
    assert_eq!(directories[&(ROM_BASE + 0x68)].len(), 2);
    // Without a following animation script the last directory has no end.
    put(&mut rom, 0x44, (ROM_BASE + 0x50) as u32);
    put(&mut rom, 0x40, (ROM_BASE + 0x50) as u32);
    assert!(catalog.directories(&rom, &descriptors).is_err());
}
