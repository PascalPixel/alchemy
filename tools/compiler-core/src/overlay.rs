//! The resource loader at 08002d5c rewrites every Thumb BL-shaped halfword pair,
//! including pairs in literal pools. Owner addresses are resource coordinates.
use crate::translation_units::{AbsoluteSymbol, AbsoluteSymbolKind};
use std::collections::{BTreeMap, BTreeSet};

pub const RESOURCE_BASE: u32 = 0x0200_0000;
pub const RUNTIME_BASE: u32 = 0x0200_8000;

pub use crate::thumb::bl_displacement as displacement;

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
pub fn placeholder_extent(text: &str, address: u32) -> Option<usize> {
    let lines = text.lines().collect::<Vec<_>>();
    usize::try_from(placeholder_block(&lines, i64::from(address))?.span).ok()
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

/// Bind legacy disassembler names once from reference sites. Candidate offsets
/// are deliberately not an input: moving a call must not change its destination.
pub fn call_symbols(
    reference: &[u8],
    offset: usize,
    extent: usize,
) -> Result<BTreeMap<String, BTreeSet<u64>>, String> {
    let mut symbols = BTreeMap::<String, BTreeSet<u64>>::new();
    for (_, name, target) in call_sites(reference, offset, extent)? {
        symbols.entry(name).or_default().insert(target);
    }
    Ok(symbols)
}

pub fn call_sites(
    reference: &[u8],
    offset: usize,
    extent: usize,
) -> Result<Vec<(usize, String, u64)>, String> {
    let end = offset
        .checked_add(extent)
        .ok_or("overlay extent overflow")?;
    let body = reference
        .get(offset..end)
        .ok_or("overlay owner exceeds reference")?;
    if load(reference, 0)?[offset..end] != load(body, offset)? {
        return Err("overlay owner boundary splits a loader relocation".into());
    }
    let (_, references) =
        crate::thumb::relocation_info(body, u64::from(RESOURCE_BASE) + offset as u64);
    let literals = references
        .iter()
        .filter(|site| site.0 == b'L')
        .flat_map(|site| site.2..site.2 + 4)
        .collect::<BTreeSet<_>>();
    let mut sites = Vec::new();
    for site in (0..body.len().saturating_sub(3)).step_by(2) {
        if (site..site + 4).any(|at| literals.contains(&at)) {
            continue;
        }
        let Some(value) = displacement(&body[site..site + 4]) else {
            continue;
        };
        let target = i64::from(value) + 2;
        if target < 0 || target as usize >= reference.len() {
            continue;
        }
        let legacy = i64::from(RESOURCE_BASE) + (offset + site + 4) as i64 + i64::from(value);
        sites.push((
            site,
            format!("Func_{legacy:08x}"),
            u64::from(RUNTIME_BASE) + target as u64,
        ));
    }
    Ok(sites)
}

pub fn resolve_call(
    name: &str,
    symbols: &BTreeMap<String, BTreeSet<u64>>,
) -> Result<AbsoluteSymbol, String> {
    let targets = symbols.get(name).ok_or_else(|| {
        format!("{name}: no stable overlay call binding; declare the runtime symbol in its translation unit")
    })?;
    if targets.len() != 1 {
        return Err(format!("{name}: ambiguous overlay call identity ({targets:x?}); separate the C declarations and declare their runtime symbols"));
    }
    Ok(AbsoluteSymbol {
        address: *targets.first().unwrap(),
        kind: AbsoluteSymbolKind::Thumb,
    })
}

pub fn external(
    name: &str,
    is_call: bool,
    reference: &[u8],
    calls: &BTreeMap<String, BTreeSet<u64>>,
) -> Result<AbsoluteSymbol, String> {
    if name.starts_with("_call_via_") {
        let register = crate::external_symbol(name, 0)
            .ok_or("invalid call-via register")?
            .address
            / 4;
        let mut banks = BTreeSet::new();
        for target in calls.values().flatten() {
            let offset = (*target - u64::from(RUNTIME_BASE)) as usize;
            let instruction = u16::from_le_bytes(
                reference
                    .get(offset..offset + 2)
                    .ok_or("call-via target exceeds reference")?
                    .try_into()
                    .unwrap(),
            );
            if instruction & 0xff87 != 0x4700 {
                continue;
            }
            let reg = usize::from((instruction >> 3) & 15);
            let Some(base) = offset.checked_sub(reg * 4) else {
                continue;
            };
            if (0..14).all(|r| {
                reference.get(base + r * 4..base + r * 4 + 2)
                    == Some(&(0x4700u16 | (r as u16) << 3).to_le_bytes()[..])
            }) {
                banks.insert(u64::from(RUNTIME_BASE) + base as u64);
            }
        }
        if banks.len() != 1 {
            return Err(format!(
                "{name}: expected one reference-proven call-via bank, found {}",
                banks.len()
            ));
        }
        return Ok(AbsoluteSymbol {
            address: banks.first().unwrap() + register * 4,
            kind: AbsoluteSymbolKind::Thumb,
        });
    }
    if is_call && name.starts_with("Func_02") {
        return resolve_call(name, calls);
    }
    let symbol = crate::external_symbol(name, crate::CALL_VIA_BASE)
        .ok_or_else(|| format!("unsupported overlay symbol: {name}"))?;
    Ok(AbsoluteSymbol {
        address: symbol.address,
        kind: if symbol.thumb {
            AbsoluteSymbolKind::Thumb
        } else {
            AbsoluteSymbolKind::Data
        },
    })
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

    #[test]
    fn call_names_are_bound_from_reference_not_candidate_positions() {
        let mut reference = vec![0; 0x200];
        reference[0x20..0x24].copy_from_slice(&pair(0x7e));
        let bindings = call_symbols(&reference, 0x20, 4).unwrap();
        let target = resolve_call("Func_020000a2", &bindings).unwrap();
        assert_eq!(target.address, u64::from(RUNTIME_BASE) + 0x80);
        let moved = pair(target.address as i32 - (RUNTIME_BASE + 0x40 + 4) as i32);
        assert_eq!(displacement(&moved), Some(0x3c));
        assert_ne!(encode(&moved, 0x20).unwrap(), reference[0x20..0x24]);
    }

    #[test]
    fn colliding_legacy_names_require_explicit_distinct_bindings() {
        let mut reference = vec![0; 0x200];
        reference[0x20..0x24].copy_from_slice(&pair(0x7e));
        reference[0x40..0x44].copy_from_slice(&pair(0x5e));
        let bindings = call_symbols(&reference, 0, reference.len()).unwrap();
        assert!(resolve_call("Func_020000a2", &bindings)
            .unwrap_err()
            .contains("ambiguous"));
        assert!(resolve_call("Func_020000a2_a", &bindings).is_err());
    }

    #[test]
    fn a_pool_pair_is_loaded_but_never_becomes_a_call_binding() {
        let mut reference = vec![0; 0x100];
        reference[..2].copy_from_slice(&0x4800u16.to_le_bytes());
        reference[2..4].copy_from_slice(&0x4770u16.to_le_bytes());
        reference[4..8].copy_from_slice(&pair(0x7e));
        assert!(call_symbols(&reference, 0, 8).unwrap().is_empty());
        assert_ne!(load(&reference, 0).unwrap(), reference);
        assert!(call_symbols(&reference, 6, 2)
            .unwrap_err()
            .contains("boundary"));
    }
}
