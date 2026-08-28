//! External address and call-via symbol handling.
//!
//! The parsers deliberately reproduce ASCII, lowercase, whole-string JS regex
//! semantics. Call-via registers are exactly r0-r13; trailing text is rejected.
//! Call-via bases differ between the main image and overlays.

pub use crate::call_via_data::CALL_VIA_BASE;
use crate::call_via_data::{CALL_VIA_REGISTERS, OVERLAY_CALL_VIA_BASE};
use crate::source_paths::lower_hex;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct ExternalSymbol {
    pub address: u64,
    pub thumb: bool,
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
