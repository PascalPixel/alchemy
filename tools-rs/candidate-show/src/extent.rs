//! `linkedFunctionExtent`, from `tools/lib/integrate_matches.ts`.
//!
//! INLINED COPY. `integrate_matches.ts` has no Rust crate yet, and this port is
//! forbidden from creating one, so the single 18-line function `candidate_show`
//! imports from it is reproduced here. If `tools-rs/integrate-matches` ever
//! lands, this module should be deleted in favour of it; the tests below are
//! the contract to check the replacement against.

use match_m2c::jsnum::math_max;
use match_m2c::jsstring::{js_split_lines, js_split_whitespace_runs, js_trim};

use crate::jsparse::js_parse_int_radix;

struct Entry {
    address: f64,
    size: f64,
    name: String,
}

/// `linkedFunctionExtent(output, target, address, byteLength)`.
///
/// PORT NOTE -- `/^Func_[0-9a-f]{8}$/` carries NO `i` flag, so an uppercase hex
/// symbol is not a function here. That is reproduced exactly; a missing-`i`
/// flag is a confirmed live defect class in this repository and "fixing" it
/// would change which symbols bound the extent.
///
/// PORT NOTE -- JavaScript `\d`-class shorthands are ASCII-only, and so are
/// these hand-written checks. `char::is_numeric` would accept Arabic-Indic
/// digits.
///
/// PORT NOTE -- `Math.max(...[])` is `-Infinity`, not an error. The guard above
/// it makes the array non-empty, so the case is unreachable; it is still
/// modelled with the JavaScript-shaped fold rather than `f64::max`, which
/// swallows NaN instead of propagating it.
pub fn linked_function_extent(
    output: &str,
    target: &str,
    address: f64,
    byte_length: f64,
) -> Result<f64, String> {
    let mut functions: Vec<Entry> = Vec::new();
    for line in js_split_lines(output) {
        // `.filter(Boolean)` drops only the empty string.
        if line.is_empty() {
            continue;
        }
        let fields = js_split_whitespace_runs(js_trim(line));
        if fields.len() < 4 {
            continue;
        }
        let kind = fields[fields.len() - 2];
        let name = fields[fields.len() - 1];
        if !is_text_symbol_kind(kind) || !is_func_symbol(name) {
            continue;
        }
        let entry = Entry {
            address: js_parse_int_radix(fields[0], 16),
            size: js_parse_int_radix(fields[1], 16),
            name: name.to_string(),
        };
        // `>=` and `<` against NaN are both false, so a NaN address is dropped
        // here rather than reaching the safe-integer guard. Reproduced by using
        // the same comparisons.
        if entry.address >= address && entry.address < address + byte_length {
            functions.push(entry);
        }
    }
    let has_target = functions
        .iter()
        .any(|entry| entry.name == target && entry.address == address);
    let malformed = functions.iter().any(|entry| {
        !is_safe_integer(entry.address) || !is_safe_integer(entry.size) || entry.size <= 0.0
    });
    if !has_target || malformed {
        return Err("compiled function symbols differ".to_string());
    }
    let mut end = f64::NEG_INFINITY;
    for entry in &functions {
        end = math_max(end, entry.address + entry.size);
    }
    if end <= address || end - address > byte_length {
        return Err("compiled function extent differs".to_string());
    }
    Ok(end - address)
}

/// `/^[Tt]$/`.
fn is_text_symbol_kind(field: &str) -> bool {
    field == "T" || field == "t"
}

/// `/^Func_[0-9a-f]{8}$/` -- lowercase only, exactly eight digits.
fn is_func_symbol(field: &str) -> bool {
    let rest = match field.strip_prefix("Func_") {
        Some(rest) => rest,
        None => return false,
    };
    rest.len() == 8
        && rest
            .bytes()
            .all(|b| b.is_ascii_digit() || (b'a'..=b'f').contains(&b))
}

/// `Number.isSafeInteger`.
fn is_safe_integer(value: f64) -> bool {
    value.is_finite() && value.fract() == 0.0 && value.abs() <= 9_007_199_254_740_991.0
}

#[cfg(test)]
mod tests {
    use super::*;

    const BASE: f64 = 0x0802_1360 as f64;

    #[test]
    fn absolute_external_symbol_does_not_change_the_extent() {
        let symbols = "08021360 00000030 T Func_08021360\n080770c1 A Func_080770c0";
        assert_eq!(
            linked_function_extent(symbols, "Func_08021360", BASE, 48.0),
            Ok(48.0)
        );
    }

    #[test]
    fn trailing_function_widens_the_extent() {
        let symbols =
            "08021360 00000030 T Func_08021360\n08021390 00000010 t Func_08021390\n";
        assert_eq!(
            linked_function_extent(symbols, "Func_08021360", BASE, 64.0),
            Ok(64.0)
        );
    }

    #[test]
    fn uppercase_hex_symbol_is_not_a_function_missing_i_flag_is_faithful() {
        let symbols = "08021360 00000030 T Func_08021360\n08021390 00000010 t Func_0802139A";
        // The uppercase row is invisible, so the extent stops at 48. A port
        // that "helpfully" added the `i` flag would return 64.
        assert_eq!(
            linked_function_extent(symbols, "Func_08021360", BASE, 64.0),
            Ok(48.0)
        );
    }

    #[test]
    fn hex_sizes_parse_as_hex_not_decimal() {
        // 0x30 is 48. A `from_str_radix(_, 10)` slip would read 30.
        let symbols = "08021360 00000030 T Func_08021360";
        assert_eq!(
            linked_function_extent(symbols, "Func_08021360", BASE, 48.0),
            Ok(48.0)
        );
    }

    #[test]
    fn missing_target_and_zero_size_both_report_symbols_differ() {
        let symbols = "08021390 00000010 t Func_08021390";
        assert_eq!(
            linked_function_extent(symbols, "Func_08021360", BASE, 64.0),
            Err("compiled function symbols differ".to_string())
        );
        let zero = "08021360 00000000 T Func_08021360";
        assert_eq!(
            linked_function_extent(zero, "Func_08021360", BASE, 64.0),
            Err("compiled function symbols differ".to_string())
        );
    }

    #[test]
    fn extent_longer_than_the_linked_image_is_rejected() {
        let symbols = "08021360 00000100 T Func_08021360";
        assert_eq!(
            linked_function_extent(symbols, "Func_08021360", BASE, 48.0),
            Err("compiled function extent differs".to_string())
        );
    }

    #[test]
    fn crlf_lines_still_split() {
        let symbols = "08021360 00000030 T Func_08021360\r\n";
        assert_eq!(
            linked_function_extent(symbols, "Func_08021360", BASE, 48.0),
            Ok(48.0)
        );
    }
}
