//! External address and call-via symbol handling.
//!
//! The parsers deliberately reproduce ASCII, lowercase, whole-string JS regex
//! semantics. Call-via registers are exactly r0-r13; trailing text is rejected.
//! Call-via bases differ between the main image and overlays.

pub use crate::compiler::call_via_data::CALL_VIA_BASE;
use crate::compiler::call_via_data::{CALL_VIA_REGISTERS, OVERLAY_CALL_VIA_BASE};
use crate::compiler::source_paths::lower_hex;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct ExternalSymbol {
    pub address: u64,
    pub thumb: bool,
}

/// GCC gives an out-of-line nested function a local numeric suffix.
pub fn nested_function_base(name: &str) -> Option<&str> {
    let (base, suffix) = name.rsplit_once('.')?;
    (!suffix.is_empty() && suffix.bytes().all(|byte| byte.is_ascii_digit())).then_some(base)
}

/// Resolve one sized code symbol, refusing ambiguous nested definitions.
pub fn function_symbol_fields<'a>(listing: &'a str, name: &str) -> Option<Vec<&'a str>> {
    let candidates = listing
        .lines()
        .map(|line| line.split_whitespace().collect::<Vec<_>>())
        .filter(|fields| {
            fields.len() == 4
                && matches!(fields[2], "T" | "t")
                && u64::from_str_radix(fields[0], 16).is_ok()
                && u64::from_str_radix(fields[1], 16).is_ok()
        })
        .collect::<Vec<_>>();
    let matches = candidates
        .into_iter()
        .filter(|fields| fields[3] == name || nested_function_base(fields[3]) == Some(name))
        .collect::<Vec<_>>();
    let first = matches.first()?;
    if matches.len() == 1 {
        return Some(first.clone());
    }
    // Materializing a local owner exports an alias of that same body.
    if matches.iter().all(|fields| fields[..2] == first[..2]) {
        return matches.iter().find(|fields| fields[3] == name).cloned();
    }
    None
}

#[cfg(test)]
mod nested_tests {
    use super::*;

    #[test]
    fn nested_code_resolution_is_unique_and_sized() {
        let listing = "00000000 00000066 t Func_080e7338.0\n00000068 00000062 t Func_080e73a0.1\n";
        assert_eq!(
            function_symbol_fields(listing, "Func_080e7338").unwrap()[3],
            "Func_080e7338.0"
        );
        let duplicate = format!("{listing}00000100 00000066 t Func_080e7338.2\n");
        assert!(function_symbol_fields(&duplicate, "Func_080e7338").is_none());
        let alias = format!("{listing}00000000 00000066 T Func_080e7338\n");
        assert_eq!(
            function_symbol_fields(&alias, "Func_080e7338").unwrap()[3],
            "Func_080e7338"
        );
        assert!(
            function_symbol_fields("00000000 00000066 D Func_080e7338.0\n", "Func_080e7338")
                .is_none()
        );
        assert!(nested_function_base("Func_080e7338.constprop").is_none());
    }
}

pub fn symbol_is_thumb(name: &str) -> bool {
    name.starts_with("Func_") || name.starts_with("_call_via_")
}

/// `call_via_base` names the `bx rN` bank the `_call_via_rN` stubs branch
/// through. Pass [`CALL_VIA_BASE`] for a `src/` translation unit, or
/// [`overlay_call_via_base`] for an overlay row.
pub fn external_symbol(name: &str, call_via_base: u64) -> Option<ExternalSymbol> {
    // Only Func symbols are Thumb; Data and Value are untagged.
    for (prefix, thumb) in [("Func_", true), ("Data_", false), ("Value_", false)] {
        if let Some(rest) = name.strip_prefix(prefix) {
            let hex = match rest.len() {
                8 => rest,
                10 => {
                    let (hex, suffix) = rest.split_at(8);
                    let suffix = suffix.as_bytes();
                    if suffix[0] != b'_' || !suffix[1].is_ascii_lowercase() {
                        return None;
                    }
                    hex
                }
                _ => return None,
            };
            if hex.len() != 8 || !lower_hex(hex) {
                return None;
            }
            return Some(ExternalSymbol {
                address: u64::from_str_radix(hex, 16).ok()?,
                thumb,
            });
        }
    }
    if let Some(rest) = name.strip_prefix("_call_via_r") {
        let bytes = rest.as_bytes();
        let register: u64 = match bytes.len() {
            1 if bytes[0].is_ascii_digit() => u64::from(bytes[0] - b'0'),
            2 if bytes[0] == b'1' && (b'0'..=b'3').contains(&bytes[1]) => {
                10 + u64::from(bytes[1] - b'0')
            }
            _ => return None,
        };
        return Some(ExternalSymbol {
            address: call_via_base + register * 4,
            thumb: true,
        });
    }
    if let Some(rest) = name.strip_prefix("_call_via_") {
        let register = CALL_VIA_REGISTERS
            .iter()
            .find(|(alias, _)| *alias == rest)
            .map(|(_, register)| *register)?;
        return Some(ExternalSymbol {
            address: call_via_base + register * 4,
            thumb: true,
        });
    }
    None
}

/// `.thumb_func` only marks a symbol defined by a label; on a `.set` alias the
pub fn external_symbol_assembly(name: &str, call_via_base: u64) -> Result<String, String> {
    let symbol = external_symbol(name, call_via_base)
        .ok_or_else(|| format!("unsupported external symbol: {name}"))?;
    let directive = if symbol.thumb { ".thumb_set" } else { ".set" };
    // Width 8 pads without truncating, matching JS padStart.
    Ok(format!(
        ".global {name}\n{directive} {name}, 0x{:08x}\n",
        symbol.address
    ))
}

/// Overlay-wide bank; owner exceptions live in `source-paths.json`.
pub fn overlay_call_via_base(overlay: &str) -> u64 {
    OVERLAY_CALL_VIA_BASE
        .iter()
        .find(|(candidate, _)| *candidate == overlay)
        .map(|(_, base)| *base)
        .unwrap_or(CALL_VIA_BASE)
}
