//! The resource loader at 08002d5c rewrites every Thumb BL-shaped halfword pair,
//! including pairs in literal pools. Owner addresses are resource coordinates.
use std::collections::{BTreeMap, BTreeSet};

pub const RESOURCE_BASE: u32 = 0x0200_0000;
pub const RUNTIME_BASE: u32 = 0x0200_8000;

use psynergy::thumb::bl_displacement as displacement;

pub fn placeholder_addresses(assembly: &str) -> BTreeSet<u32> {
    assembly
        .lines()
        .filter_map(|line| {
            u32::from_str_radix(
                line.trim().strip_prefix("AlchemyC_")?.strip_suffix(':')?,
                16,
            )
            .ok()
        })
        .collect()
}

#[derive(Clone, Copy)]
pub struct Placeholder {
    pub start: usize,
    pub end: usize,
    pub span: i64,
}
pub fn space_size(line: &str) -> Option<i64> {
    let value = line.trim().strip_prefix(".space ")?.trim();
    value
        .strip_prefix("0x")
        .map_or_else(
            || value.parse().ok(),
            |hex| i64::from_str_radix(hex, 16).ok(),
        )
        .filter(|size| *size > 0)
}
pub fn placeholder_block(lines: &[&str], address: i64) -> Option<Placeholder> {
    let tag = format!("AlchemyC_{address:08x}:");
    let mut matches = lines
        .iter()
        .enumerate()
        .filter(|(_, line)| line.trim() == tag);
    let (start, _) = matches.next()?;
    if matches.next().is_some() {
        return None;
    }
    let (mut end, mut span) = (start + 1, 0i64);
    while let Some(line) = lines.get(end).map(|line| line.trim()) {
        if line.starts_with(".space ") {
            span = span.checked_add(space_size(line)?)?;
        } else if !(line.starts_with(".L_") && line.ends_with(':')) {
            break;
        }
        end += 1;
    }
    (span > 0).then_some(Placeholder { start, end, span })
}
/// Every `AlchemyC_` placeholder's address and extent, as `placeholder_extent`
/// reads each; a placeholder spelled twice or without space is left out.
pub fn placeholder_extents(text: &str) -> BTreeMap<u32, usize> {
    let lines = text.lines().collect::<Vec<_>>();
    let mut extents = BTreeMap::new();
    let mut refused = BTreeSet::new();
    for (index, line) in lines.iter().enumerate() {
        let Some(address) = line
            .trim()
            .strip_prefix("AlchemyC_")
            .and_then(|rest| rest.strip_suffix(':'))
            .and_then(|hex| u32::from_str_radix(hex, 16).ok())
        else {
            continue;
        };
        let mut span = Some(0i64);
        for line in lines[index + 1..].iter().map(|line| line.trim()) {
            if line.starts_with(".space ") {
                span = span
                    .zip(space_size(line))
                    .and_then(|(span, size)| span.checked_add(size));
            } else if !(line.starts_with(".L_") && line.ends_with(':')) {
                break;
            }
        }
        match span
            .filter(|span| *span > 0)
            .and_then(|span| usize::try_from(span).ok())
        {
            Some(span) if extents.insert(address, span).is_none() => {}
            _ => {
                refused.insert(address);
            }
        }
    }
    extents.retain(|address, _| !refused.contains(address));
    extents
}
pub fn placeholder_extent(text: &str, address: u32) -> Option<usize> {
    let lines = text.lines().collect::<Vec<_>>();
    usize::try_from(placeholder_block(&lines, i64::from(address))?.span).ok()
}
/// The one `AlchemyData_<address>:` block a unit's read-only data fills: a
/// label followed only by positive `.space` lines, as for C placeholders.
pub fn data_placeholder_extent(text: &str, address: u32) -> Option<usize> {
    let tag = format!("AlchemyData_{address:08x}:");
    let lines = text.lines().map(str::trim).collect::<Vec<_>>();
    let mut matches = (0..lines.len()).filter(|index| lines[*index] == tag);
    let start = matches.next()?;
    if matches.next().is_some() {
        return None;
    }
    let mut span = 0usize;
    for line in lines[start + 1..]
        .iter()
        .take_while(|line| line.starts_with(".space "))
    {
        span = span.checked_add(usize::try_from(space_size(line)?).ok()?)?;
    }
    (span > 0).then_some(span)
}

fn transform(bytes: &[u8], offset: usize, encode: bool) -> Result<Vec<u8>, String> {
    if !offset.is_multiple_of(2) || !bytes.len().is_multiple_of(2) {
        return Err("overlay image and offset must be halfword aligned".into());
    }
    let mut result = bytes.to_vec();
    for suffix in (2..result.len()).step_by(2) {
        let Some(value) = displacement(&result[suffix - 2..suffix + 2]) else {
            continue;
        };
        let site = u32::try_from(
            offset
                .checked_add(suffix)
                .ok_or("overlay offset overflow")?,
        )
        .map_err(|_| "overlay offset exceeds address space")?;
        let value = if encode {
            (value as u32).wrapping_add(site)
        } else {
            (value as u32).wrapping_sub(site)
        };
        let high = 0xf000 | ((value >> 12) & 0x7ff) as u16;
        let low = 0xf800 | ((value >> 1) & 0x7ff) as u16;
        result[suffix - 2..suffix].copy_from_slice(&high.to_le_bytes());
        result[suffix..suffix + 2].copy_from_slice(&low.to_le_bytes());
    }
    Ok(result)
}

pub fn load(bytes: &[u8], offset: usize) -> Result<Vec<u8>, String> {
    transform(bytes, offset, false)
}

pub fn encode(bytes: &[u8], offset: usize) -> Result<Vec<u8>, String> {
    let encoded = transform(bytes, offset, true)?;
    if load(&encoded, offset)? != bytes {
        return Err("overlay serialization failed the loader round trip".into());
    }
    Ok(encoded)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn placeholder_extents_are_unique_positive_and_include_local_aliases() {
        let text = "AlchemyC_02000100:\n .space 2\n.L_02000102:\n .space 0x6\n bx lr\n";
        let lines = text.lines().collect::<Vec<_>>();
        let block = placeholder_block(&lines, 0x02000100).unwrap();
        assert_eq!((block.start, block.end, block.span), (0, 4, 8));
        assert_eq!(placeholder_extent(text, 0x02000100), Some(8));
        assert_eq!(placeholder_extent(text, 0x02000102), None);
        for body in [
            ".space 0",
            ".space -2",
            ".space nope",
            ".L_02000100:",
            ".space 9223372036854775807\n.space 1",
        ] {
            assert_eq!(
                placeholder_extent(&format!("AlchemyC_02000100:\n{body}\n"), 0x02000100),
                None
            );
        }
        assert_eq!(
            placeholder_extent(&format!("{text}{text}"), 0x02000100),
            None
        );
    }

    #[test]
    fn data_placeholders_are_unique_labelled_space_blocks() {
        let text = "AlchemyData_020002d0:\n\t.space 0x100\n\t.space 0x100\n";
        assert_eq!(data_placeholder_extent(text, 0x020002d0), Some(0x200));
        assert_eq!(data_placeholder_extent(text, 0x020002d4), None);
        assert_eq!(
            data_placeholder_extent(&format!("{text}{text}"), 0x020002d0),
            None
        );
        assert_eq!(
            data_placeholder_extent("AlchemyData_020002d0:\n\t.4byte 1\n", 0x020002d0),
            None
        );
        assert_eq!(
            data_placeholder_extent("AlchemyC_020002d0:\n\t.space 4\n", 0x020002d0),
            None
        );
    }

    fn pair(value: i32) -> Vec<u8> {
        [
            0xf000 | (((value as u32) >> 12) & 0x7ff) as u16,
            0xf800 | (((value as u32) >> 1) & 0x7ff) as u16,
        ]
        .into_iter()
        .flat_map(u16::to_le_bytes)
        .collect()
    }

    #[test]
    fn loader_transforms_literals_and_preserves_ordinary_data() {
        let mut data = pair(0x1400);
        data.extend_from_slice(&0x02008101u32.to_le_bytes());
        let runtime = load(&data, 0x23fe).unwrap();
        assert_eq!(&runtime[..4], &pair(-0x1000));
        assert_eq!(&runtime[4..], &data[4..]);
        assert_eq!(encode(&runtime, 0x23fe).unwrap(), data);
    }

    #[test]
    fn codec_round_trips_signed_calls_at_different_owner_offsets() {
        for offset in [0, 2, 0x104, 0x2000, 0xfffe] {
            for value in [-0x400000, -0x1000, -2, 0, 2, 0x1400, 0x3ffffe] {
                let runtime = pair(value);
                assert_eq!(
                    load(&encode(&runtime, offset).unwrap(), offset).unwrap(),
                    runtime
                );
            }
        }
        assert!(load(&[0], 0).is_err());
        assert!(encode(&[0, 0], 1).is_err());
    }
}
