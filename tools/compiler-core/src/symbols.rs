//! `externalSymbol` / `externalSymbolAssembly` / `overlayCallViaBase`, now
//! implemented natively.
//!
//! The three TypeScript regexes are matched by hand. That is deliberate: the
//! `regex` crate would be a dependency, and more importantly its character
//! classes are not JavaScript's. The patterns being replaced are
//!
//! ```text
//! ADDRESS_SYMBOL  = /^(Func|Data|Value)_([0-9a-f]{8})(?:_[a-z])?$/
//! CALL_VIA_SYMBOL = /^_call_via_r(1[0-3]|[0-9])$/
//! CALL_VIA_ALIAS  = /^_call_via_(sl|fp|ip|sp)$/
//! ```
//!
//! PORT NOTE (JS semantics pinned by tests in this module):
//! * `[0-9]`, `[a-f]`, `[a-z]` in JS are literal ASCII ranges, never Unicode
//!   classes. `Func_０８００abcd` (fullwidth digits) must NOT match, and does
//!   not here because the bytes are compared as ASCII.
//! * `1[0-3]|[0-9]` is ordered-leftmost alternation over a fixed-width tail
//!   anchored by `$`, so it accepts exactly r0-r13 and cannot produce `r00`
//!   or `r1` followed by junk.
//! * `^`/`$` with no `m` flag anchor the whole string, and JS `$` does not
//!   permit a trailing newline, so `"_call_via_r3\n"` is not a symbol.
//! * `String.prototype.match` returns `null` on no match; the TypeScript then
//!   falls through to the next pattern. The prefixes are mutually exclusive, so
//!   returning `None` from inside a matched prefix arm is equivalent.
//!
//! PORT NOTE (pre-existing duplicate): `tools/verify/src/lib.rs` carries its
//! own `external_symbol`/`external_symbol_assembly` pair that hardcodes
//! `CALL_VIA_BASE` and has no `callViaBase` parameter -- correct for the main
//! image, silently wrong for any overlay. This crate is the complete port. That
//! duplicate is left alone here on purpose (it is another agent's file); see the
//! retirement note in `lib.rs`.

use crate::routing::source_key;

pub use crate::call_via_data::CALL_VIA_BASE;
use crate::call_via_data::{CALL_VIA_REGISTERS, OVERLAY_CALL_VIA_BASE, SOURCE_CALL_VIA_BASE};

/// `interface ExternalSymbol`.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct ExternalSymbol {
    pub address: u64,
    pub thumb: bool,
}

fn is_lower_hex8(value: &str) -> bool {
    value.len() == 8
        && value
            .bytes()
            .all(|b| b.is_ascii_digit() || (b'a'..=b'f').contains(&b))
}

/// `externalSymbol(name, callViaBase)`.
///
/// `call_via_base` names the `bx rN` bank the `_call_via_rN` stubs branch
/// through. Pass [`CALL_VIA_BASE`] for a `src/` translation unit, or
/// [`overlay_call_via_base`] for an overlay row.
pub fn external_symbol(name: &str, call_via_base: u64) -> Option<ExternalSymbol> {
    // ADDRESS_SYMBOL. `thumb` is `addressed[1] === "Func"`, so `Data_` and
    // `Value_` are both non-Thumb.
    for (prefix, thumb) in [("Func_", true), ("Data_", false), ("Value_", false)] {
        if let Some(rest) = name.strip_prefix(prefix) {
            let hex = match rest.len() {
                8 => rest,
                // The optional `(?:_[a-z])?` disambiguation suffix.
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
            if !is_lower_hex8(hex) {
                return None;
            }
            return Some(ExternalSymbol {
                address: u64::from_str_radix(hex, 16).ok()?,
                thumb,
            });
        }
    }
    // CALL_VIA_SYMBOL.
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
    // CALL_VIA_ALIAS.
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

/// `externalSymbolAssembly(name, callViaBase)`.
///
/// `.thumb_func` only marks a symbol defined by a label; on a `.set` alias the
/// branch type is lost, so a data reference to the stub links to the plain even
/// address. `.thumb_set` is the alias form that keeps the branch type, so both
/// the branch offset and the tagged pool word come out right.
///
/// PORT NOTE: the TypeScript `throw`s on an unsupported name; every call site
/// tests `externalSymbol(...) === null` first, so the throw is a guard rather
/// than control flow. Returned as `Err` here with the identical message text.
pub fn external_symbol_assembly(name: &str, call_via_base: u64) -> Result<String, String> {
    let symbol = external_symbol(name, call_via_base)
        .ok_or_else(|| format!("unsupported external symbol: {name}"))?;
    let directive = if symbol.thumb { ".thumb_set" } else { ".set" };
    // `symbol.address.toString(16).padStart(8, "0")`. Every address this can
    // produce is a non-negative integer below 2^32 for real inputs, but
    // `padStart` only pads and never truncates, so a wider value would widen the
    // field rather than wrap -- `{:08x}` behaves the same way.
    Ok(format!(
        ".global {name}\n{directive} {name}, 0x{:08x}\n",
        symbol.address
    ))
}

/// `overlayCallViaBase(overlay, source?)`.
///
/// The per-source override wins over the overlay's entry, which wins over the
/// main image's bank. Pass `None` for `source` where the TypeScript passes
/// `undefined`: the override table is not consulted at all in that case, which
/// matters because `sourceKey` resolves against the repository root and would
/// otherwise be asked to normalise nothing.
pub fn overlay_call_via_base(overlay: &str, source: Option<&str>) -> u64 {
    let override_base = source.and_then(|source| {
        let key = source_key(source);
        SOURCE_CALL_VIA_BASE
            .iter()
            .find(|(candidate, _)| *candidate == key)
            .map(|(_, base)| *base)
    });
    override_base
        .or_else(|| {
            OVERLAY_CALL_VIA_BASE
                .iter()
                .find(|(candidate, _)| *candidate == overlay)
                .map(|(_, base)| *base)
        })
        .unwrap_or(CALL_VIA_BASE)
}
