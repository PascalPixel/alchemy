//! Portable ARMv4T Thumb assembly reconstruction.
//!
//! The caller owns image discovery and function boundaries. This module turns
//! one complete Thumb extent into readable GAS source while preserving pools,
//! padding and undecoded data inside that extent.

use crate::decode::{decode_one, decode_window_at, Ins, Kind};
use std::collections::{BTreeMap, BTreeSet};

pub mod addresses;

fn u16_at(image: &[u8], offset: usize) -> Result<u16, String> {
    let bytes = image
        .get(offset..offset + 2)
        .ok_or_else(|| "halfword is outside image".to_string())?;
    Ok(u16::from_le_bytes([bytes[0], bytes[1]]))
}

fn u32_at(image: &[u8], offset: usize) -> Result<u32, String> {
    let bytes = image
        .get(offset..offset + 4)
        .ok_or_else(|| "word is outside image".to_string())?;
    Ok(u32::from_le_bytes([bytes[0], bytes[1], bytes[2], bytes[3]]))
}

fn branch_target(kind: &Kind) -> Option<u32> {
    match kind {
        Kind::B { target } | Kind::Bcond { target, .. } | Kind::Bl { target } => Some(*target),
        _ => None,
    }
}

fn retarget(text: &str, target: u32, replacement: &str) -> String {
    let needle = format!("0x{target:08x}");
    text.replacen(&needle, replacement, 1)
}

/// Render one complete Thumb function extent as standalone GAS source.
pub fn thumb_source(
    image: &[u8],
    image_base: u32,
    entry: u32,
    span: u32,
) -> Result<String, String> {
    validate_extent(image, image_base, entry, span)?;
    let instructions = decode_window_at(image, image_base, entry, span);
    render(image, image_base, entry, span, &instructions)
}

/// Render instructions established by the caller's control-flow walk. This
/// keeps returns, register calls and switch successors under one flow owner.
pub fn thumb_source_from_instructions(
    image: &[u8],
    image_base: u32,
    entry: u32,
    span: u32,
    addresses: &BTreeSet<u32>,
) -> Result<String, String> {
    let end = validate_extent(image, image_base, entry, span)?;
    let mut instructions = Vec::new();
    let mut previous_end = entry;
    for &address in addresses {
        if address % 2 != 0 || address < previous_end || address >= end {
            return Err(
                "discovered instruction is unaligned, overlapping or outside extent".into(),
            );
        }
        let instruction = decode_one(image, image_base, address)
            .filter(|instruction| !matches!(instruction.kind, Kind::Unknown(_)))
            .ok_or("discovered instruction cannot be rendered as ARMv4T Thumb")?;
        previous_end = address
            .checked_add(instruction.size)
            .filter(|end_of_instruction| *end_of_instruction <= end)
            .ok_or("discovered instruction extends beyond extent")?;
        instructions.push(instruction);
    }
    if addresses.first() != Some(&entry) {
        return Err("discovered Thumb extent has no entry instruction".into());
    }
    render(image, image_base, entry, span, &instructions)
}

fn validate_extent(image: &[u8], image_base: u32, entry: u32, span: u32) -> Result<u32, String> {
    if span == 0 || span % 2 != 0 || entry % 2 != 0 {
        return Err("Thumb extent must be nonempty and halfword aligned".into());
    }
    let start = entry
        .checked_sub(image_base)
        .ok_or_else(|| "entry precedes image".to_string())? as usize;
    let end = start
        .checked_add(span as usize)
        .ok_or_else(|| "extent overflow".to_string())?;
    if end > image.len() {
        return Err("Thumb extent exceeds image".into());
    }
    entry
        .checked_add(span)
        .ok_or_else(|| "extent address overflow".into())
}

/// The literal-pool words that the extent's own loads read.
fn pool_words(
    image: &[u8],
    image_base: u32,
    entry: u32,
    span: u32,
    instructions: &[Ins],
) -> Result<BTreeSet<u32>, String> {
    let mut pool_words = BTreeSet::new();
    for ins in instructions {
        if matches!(ins.kind, Kind::LdrPool { .. }) {
            let offset = (ins.addr - image_base) as usize;
            let half = u16_at(image, offset)?;
            let address = ((ins.addr + 4) & !3) + u32::from(half & 0xff) * 4;
            if entry <= address && address + 4 <= entry + span {
                pool_words.insert(address);
            }
        }
    }
    Ok(pool_words)
}

/// The addresses at which rendering starts a row, stepping as `render` does.
fn row_starts(
    entry: u32,
    span: u32,
    by_address: &BTreeMap<u32, &Ins>,
    pool_words: &BTreeSet<u32>,
) -> BTreeSet<u32> {
    let mut rows = BTreeSet::new();
    let mut cursor = entry;
    while cursor < entry + span {
        rows.insert(cursor);
        cursor += if pool_words.contains(&cursor) {
            4
        } else {
            by_address.get(&cursor).map_or(2, |ins| ins.size)
        };
    }
    rows
}

/// A multiple transfer whose base register is also in its list: ARMv4T leaves
/// the result UNPREDICTABLE and GAS refuses or warns, so it stays a halfword.
fn unpredictable(kind: &Kind) -> bool {
    match *kind {
        Kind::Ldmia { rn, list } => list & (1 << rn) != 0,
        Kind::Stmia { rn, list } => list & (1 << rn) != 0 && list & ((1 << rn) - 1) != 0,
        _ => false,
    }
}

fn render(
    image: &[u8],
    image_base: u32,
    entry: u32,
    span: u32,
    instructions: &[Ins],
) -> Result<String, String> {
    let by_address: BTreeMap<u32, _> = instructions.iter().map(|ins| (ins.addr, ins)).collect();
    let pool_words = pool_words(image, image_base, entry, span, instructions)?;
    // Only a row the rendering will start can carry a label; any other
    // target stays an absolute address.
    let rows = row_starts(entry, span, &by_address, &pool_words);
    let mut labels = BTreeMap::new();
    for ins in instructions {
        if let Some(target) = branch_target(&ins.kind) {
            if entry <= target && target < entry + span && rows.contains(&target) {
                let next = labels.len();
                labels.entry(target).or_insert_with(|| format!(".L{next}"));
            }
        }
    }

    let name = format!("Func_{entry:08x}");
    let mut lines = vec![
        ".syntax unified".to_string(),
        "\t.thumb".to_string(),
        format!("\t.global {name}"),
        "\t.thumb_func".to_string(),
        format!("{name}:"),
    ];
    let mut cursor = entry;
    while cursor < entry + span {
        if let Some(label) = labels.get(&cursor) {
            lines.push(format!("{label}:"));
        }
        if pool_words.contains(&cursor) {
            let value = u32_at(image, (cursor - image_base) as usize)?;
            lines.push(format!("\t.4byte 0x{value:08x}"));
            cursor += 4;
            continue;
        }
        if let Some(ins) = by_address
            .get(&cursor)
            .filter(|ins| !unpredictable(&ins.kind))
        {
            let mut text = match ins.kind {
                Kind::LdrPool { rd, .. } => {
                    let half = u16_at(image, (cursor - image_base) as usize)?;
                    format!("ldr r{rd}, [pc, #{}]", u32::from(half & 0xff) * 4)
                }
                _ => ins.text.clone(),
            };
            if let Some(target) = branch_target(&ins.kind) {
                let replacement = labels
                    .get(&target)
                    .cloned()
                    .unwrap_or_else(|| format!("0x{target:08x}"));
                text = retarget(&text, target, &replacement);
            }
            lines.push(format!("\t{text}"));
            cursor += ins.size;
            continue;
        }
        let value = u16_at(image, (cursor - image_base) as usize)?;
        lines.push(format!("\t.2byte 0x{value:04x}"));
        cursor += 2;
    }
    Ok(lines.join("\n") + "\n")
}

#[cfg(test)]
mod tests {
    use super::{thumb_source, thumb_source_from_instructions};
    use crate::discovery::{Discovery, Mode};
    use std::collections::BTreeSet;

    #[test]
    fn an_unpredictable_multiple_transfer_stays_a_halfword() {
        let image = [
            0x8e, 0xc9, // ldmia r1!, {r1, r2, r3, r7}
            0x70, 0x47, // bx lr
        ];
        let source = thumb_source(&image, 0x08000100, 0x08000100, 4).unwrap();
        assert!(source.contains(".2byte 0xc98e"), "{source}");
    }

    #[test]
    fn a_branch_into_the_middle_of_a_row_keeps_its_absolute_target() {
        let image = [
            0xff, 0xf7, 0xff, 0xff, // bl 0x08000102, the second half of this bl
            0x70, 0x47, // bx lr
        ];
        let source = thumb_source(&image, 0x08000100, 0x08000100, 6).unwrap();
        assert!(source.contains("bl 0x08000102"), "{source}");
        assert!(!source.contains(".L0"), "{source}");
    }

    #[test]
    fn discovered_register_call_keeps_its_continuation_and_pool() {
        let image = [
            0x01, 0x4a, // ldr r2, [pc, #4]
            0xfc, 0x46, // mov ip, pc
            0x10, 0x47, // bx r2; RAM routine returns through ip
            0x70, 0x47, // bx lr
            0x18, 0x01, 0x00, 0x03,
        ];
        let base = 0x08000100;
        let mut discovery = Discovery::new(&image, base);
        discovery.add_seed(base, Mode::Thumb, "test");
        discovery.walk_function(base);
        let addresses = discovery
            .function(base)
            .unwrap()
            .instructions
            .iter()
            .map(|address| *address as u32)
            .collect();
        let source =
            thumb_source_from_instructions(&image, base as u32, base as u32, 12, &addresses)
                .unwrap();
        assert!(source.contains("bx r2\n\tbx lr"), "{source}");
        assert!(source.contains(".4byte 0x03000118"), "{source}");
        assert!(!source.contains(".2byte"), "{source}");
    }

    #[test]
    fn discovered_instructions_must_be_complete_decodable_and_disjoint() {
        let base = 0x08000100;
        let image = [0x00, 0xf0, 0x00, 0xf8, 0x70, 0x47, 0x00, 0xde];
        for addresses in [
            BTreeSet::from([base, base + 2]), // BL suffix overlaps the pair
            BTreeSet::from([base, base + 6]), // undefined ARMv4T instruction
            BTreeSet::from([base, base + 8]), // outside the extent
            BTreeSet::from([base + 4]),       // no entry instruction
        ] {
            assert!(thumb_source_from_instructions(&image, base, base, 8, &addresses).is_err());
        }
        assert!(
            thumb_source_from_instructions(&image, base, base, 2, &BTreeSet::from([base])).is_err()
        );
    }

    #[test]
    fn renders_calls_local_branches_and_pool_words() {
        // ldr r0,[pc,#4]; cmp r0,#0; beq +0; bx lr; pool word.
        let image = [
            0x01, 0x48, 0x00, 0x28, 0x00, 0xd0, 0x70, 0x47, 0x78, 0x56, 0x34, 0x12,
        ];
        let source = thumb_source(&image, 0x0200_0000, 0x0200_0000, 12).unwrap();
        assert!(source.contains("beq .L0"), "{source}");
        assert!(source.contains("ldr r0, [pc, #4]"), "{source}");
        assert!(source.contains(".L0:\n\t.4byte 0x12345678"), "{source}");
    }

    #[test]
    fn preserves_unreachable_halfwords_as_data() {
        let image = [0x70, 0x47, 0x34, 0x12];
        let source = thumb_source(&image, 0x0800_1000, 0x0800_1000, 4).unwrap();
        assert!(source.contains("bx lr"), "{source}");
        assert!(source.contains(".2byte 0x1234"), "{source}");
    }
}
