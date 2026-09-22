//! Portable ARMv4T Thumb assembly reconstruction.
//!
//! The caller owns image discovery and function boundaries. This module turns
//! one complete Thumb extent into readable GAS source while preserving pools,
//! padding and undecoded data inside that extent.

use crate::decode::{decode_window_at, Kind};
use std::collections::{BTreeMap, BTreeSet};

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

    let instructions = decode_window_at(image, image_base, entry, span);
    let by_address: BTreeMap<u32, _> = instructions.iter().map(|ins| (ins.addr, ins)).collect();
    let mut labels = BTreeMap::new();
    for ins in &instructions {
        if let Some(target) = branch_target(&ins.kind) {
            if entry <= target && target < entry + span {
                let next = labels.len();
                labels.entry(target).or_insert_with(|| format!(".L{next}"));
            }
        }
    }

    let mut pool_words = BTreeSet::new();
    for ins in &instructions {
        if matches!(ins.kind, Kind::LdrPool { .. }) {
            let offset = (ins.addr - image_base) as usize;
            let half = u16_at(image, offset)?;
            let address = ((ins.addr + 4) & !3) + u32::from(half & 0xff) * 4;
            if entry <= address && address + 4 <= entry + span {
                pool_words.insert(address);
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
        if let Some(ins) = by_address.get(&cursor) {
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
    use super::thumb_source;

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
