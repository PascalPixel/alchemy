//! `externalSymbol` / `externalSymbolAssembly` / `overlayCallViaBase`, ported
//! from `tools/lib/alchemy_gcc.ts`.
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
//! PORT NOTE (pre-existing duplicate): `tools-rs/verify/src/lib.rs` carries its
//! own `external_symbol`/`external_symbol_assembly` pair that hardcodes
//! `CALL_VIA_BASE` and has no `callViaBase` parameter -- correct for the main
//! image, silently wrong for any overlay. This crate is the complete port. That
//! duplicate is left alone here on purpose (it is another agent's file); see the
//! retirement note in `lib.rs`.

use alchemy_routing::routing::source_key;

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

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn addressed_symbols_carry_their_own_address() {
        assert_eq!(
            external_symbol("Func_08002ee4", CALL_VIA_BASE),
            Some(ExternalSymbol {
                address: 0x0800_2ee4,
                thumb: true
            })
        );
        assert_eq!(
            external_symbol("Data_08002ee4", CALL_VIA_BASE),
            Some(ExternalSymbol {
                address: 0x0800_2ee4,
                thumb: false
            })
        );
        assert_eq!(
            external_symbol("Value_08002ee4", CALL_VIA_BASE),
            Some(ExternalSymbol {
                address: 0x0800_2ee4,
                thumb: false
            })
        );
        // The `(?:_[a-z])?` disambiguation suffix does not change the address.
        assert_eq!(
            external_symbol("Func_08002ee4_b", CALL_VIA_BASE),
            Some(ExternalSymbol {
                address: 0x0800_2ee4,
                thumb: true
            })
        );
    }

    #[test]
    fn address_symbol_rejects_what_the_javascript_regex_rejects() {
        for name in [
            "Func_08002EE4",     // [0-9a-f] is lowercase only
            "Func_8002ee4",      // {8} exactly
            "Func_08002ee44",    // ...and no more
            "Func_08002ee4_",    // the suffix needs its letter
            "Func_08002ee4_B",   // [a-z] is lowercase ASCII
            "Func_08002ee4_bc",  // one letter, not two
            "xFunc_08002ee4",    // ^ anchors
            "Func_08002ee4 ",    // $ anchors
            "Func_08002ee4\n",   // JS $ does not allow a trailing newline
            "Func_０８００2ee4", // JS [0-9] is ASCII-only, never Unicode Nd
            "Funk_08002ee4",
        ] {
            assert_eq!(external_symbol(name, CALL_VIA_BASE), None, "{name:?}");
        }
    }

    #[test]
    fn call_via_registers_index_the_bank() {
        for register in 0..=13u64 {
            let name = format!("_call_via_r{register}");
            assert_eq!(
                external_symbol(&name, CALL_VIA_BASE),
                Some(ExternalSymbol {
                    address: CALL_VIA_BASE + register * 4,
                    thumb: true
                }),
                "{name}"
            );
        }
        for (alias, register) in CALL_VIA_REGISTERS {
            let name = format!("_call_via_{alias}");
            assert_eq!(
                external_symbol(&name, CALL_VIA_BASE),
                Some(ExternalSymbol {
                    address: CALL_VIA_BASE + register * 4,
                    thumb: true
                }),
                "{name}"
            );
        }
    }

    #[test]
    fn call_via_rejects_what_the_alternation_cannot_produce() {
        for name in [
            "_call_via_r00", // `1[0-3]|[0-9]` has no leading-zero two-digit form
            "_call_via_r14",
            "_call_via_r-1",
            "_call_via_r",
            "_call_via_r3x",
            "_call_via_R3",
            "_call_via_SL",
            "_call_via_",
            "call_via_r3",
            "_call_via_r3\n",
        ] {
            assert_eq!(external_symbol(name, CALL_VIA_BASE), None, "{name:?}");
        }
    }

    #[test]
    fn call_via_base_moves_the_bank_but_not_addressed_symbols() {
        let overlay = 0x0200_61b4;
        assert_eq!(
            external_symbol("_call_via_r3", overlay).unwrap().address,
            overlay + 12
        );
        assert_eq!(
            external_symbol("Func_08002ee4", overlay).unwrap().address,
            0x0800_2ee4
        );
    }

    /// Salvaged from `callbackArityLint` in `alchemy_gcc.ts` (L3808), which is
    /// dead code -- it lints for `.map(externalSymbolAssembly)` passed by
    /// reference, where `Array#map`'s index argument would land in the
    /// `callViaBase` slot -- but whose two assertions are real.
    #[test]
    fn call_via_base_reaches_the_assembly_text() {
        assert_eq!(
            external_symbol_assembly("_call_via_r3", CALL_VIA_BASE).unwrap(),
            ".global _call_via_r3\n.thumb_set _call_via_r3, 0x080072f0\n"
        );
        assert!(external_symbol_assembly("_call_via_r3", 0x0200_61b4)
            .unwrap()
            .contains("0x020061c0"));
    }

    #[test]
    fn assembly_picks_the_directive_that_keeps_the_branch_type() {
        assert_eq!(
            external_symbol_assembly("Func_08002ee4", CALL_VIA_BASE).unwrap(),
            ".global Func_08002ee4\n.thumb_set Func_08002ee4, 0x08002ee4\n"
        );
        assert_eq!(
            external_symbol_assembly("Data_08002ee4", CALL_VIA_BASE).unwrap(),
            ".global Data_08002ee4\n.set Data_08002ee4, 0x08002ee4\n"
        );
        assert_eq!(
            external_symbol_assembly("nonsense", CALL_VIA_BASE),
            Err("unsupported external symbol: nonsense".to_string())
        );
    }

    /// `padStart(8, "0")` pads and never truncates. Pinned so nobody
    /// "simplifies" the format to a fixed-width truncating one.
    #[test]
    fn assembly_pads_to_eight_and_does_not_truncate() {
        assert!(external_symbol_assembly("Func_00000004", CALL_VIA_BASE)
            .unwrap()
            .contains("0x00000004"));
    }

    #[test]
    fn overlay_bank_lookup_follows_the_override_chain() {
        assert_eq!(overlay_call_via_base("resource_373", None), 0x0200_61b4);
        assert_eq!(overlay_call_via_base("resource_999", None), CALL_VIA_BASE);
        assert_eq!(
            overlay_call_via_base("resource_373", Some("exact/resource_373_c_02005610.c")),
            0x0200_b794
        );
        // The override is keyed on the repo-relative path, so a `./`-prefixed
        // or absolute spelling of the same file resolves to the same entry.
        assert_eq!(
            overlay_call_via_base("resource_373", Some("./exact/resource_373_c_02005610.c")),
            0x0200_b794
        );
        // An unlisted source falls through to the overlay's own entry.
        assert_eq!(
            overlay_call_via_base("resource_373", Some("exact/resource_373_c_02000030.c")),
            0x0200_61b4
        );
    }

    /// Every address this crate can emit is an integer, so no ECMAScript
    /// `ToString(Number)` implementation is needed. Walk the whole reachable
    /// output space and prove it.
    #[test]
    fn every_emitted_address_is_an_integer_in_hex() {
        let mut names: Vec<String> = (0..=13).map(|r| format!("_call_via_r{r}")).collect();
        names.extend(
            CALL_VIA_REGISTERS
                .iter()
                .map(|(a, _)| format!("_call_via_{a}")),
        );
        names.push("Func_ffffffff".to_string());
        names.push("Data_00000000".to_string());
        let mut bases: Vec<u64> = vec![CALL_VIA_BASE, 0x0200_b794];
        bases.extend(OVERLAY_CALL_VIA_BASE.iter().map(|(_, b)| *b));
        for base in bases {
            for name in &names {
                let text = external_symbol_assembly(name, base).unwrap();
                let hex = text.rsplit("0x").next().unwrap().trim_end();
                assert!(
                    hex.len() >= 8 && hex.bytes().all(|b| b.is_ascii_hexdigit()),
                    "{text:?}"
                );
            }
        }
    }
}
