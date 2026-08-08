//! JavaScript number semantics: `Number(x)`, `Number#toString(16)`,
//! `String#padStart`, and the `a - b` comparator contract.

use crate::jsstring::{is_js_space, js_trim};

/// `Number(text)` restricted to what this port can be handed.
///
/// PORT NOTE (trap: `parseInt(text, 16)` accepts a `0x` prefix and
/// `i64::from_str_radix` rejects it). Every numeric parse in this crate goes
/// through this function or [`parse_hex`]; there is deliberately no second
/// parsing path, because the same divergence has already cost this repository
/// bytes at two separate call sites.
///
/// `Number("")` and `Number("   ")` are `0`, not an error. `Number("8x")` is
/// `NaN` (unlike `parseInt("8x", 10)`, which is `8`).
pub fn js_number(text: &str) -> f64 {
    let trimmed = js_trim(text);
    if trimmed.is_empty() {
        return 0.0;
    }
    let lower = trimmed.to_ascii_lowercase();
    if let Some(digits) = lower.strip_prefix("0x") {
        return radix_literal(digits, 16);
    }
    if let Some(digits) = lower.strip_prefix("0o") {
        return radix_literal(digits, 8);
    }
    if let Some(digits) = lower.strip_prefix("0b") {
        return radix_literal(digits, 2);
    }
    match trimmed {
        "Infinity" | "+Infinity" => return f64::INFINITY,
        "-Infinity" => return f64::NEG_INFINITY,
        _ => {}
    }
    // Rust's `f64::from_str` accepts `inf`, `NaN`, `1e5`, `.5`, `5.`; of those
    // JavaScript rejects only the first spelling, which the guard above and the
    // check here between them exclude.
    if lower.contains("inf") || lower.contains("nan") {
        return f64::NAN;
    }
    trimmed.parse::<f64>().unwrap_or(f64::NAN)
}

fn radix_literal(digits: &str, radix: u32) -> f64 {
    if digits.is_empty() {
        return f64::NAN;
    }
    let mut value = 0.0f64;
    for c in digits.chars() {
        match c.to_digit(radix) {
            Some(d) => value = value * f64::from(radix) + f64::from(d),
            None => return f64::NAN,
        }
    }
    value
}

/// A `0x`-prefixed or bare hexadecimal literal, as an exact integer.
///
/// The single hex entry point for code that wants an integer rather than a
/// float. Returns `None` where `Number()` would return `NaN`.
pub fn parse_hex(text: &str) -> Option<u128> {
    let trimmed = js_trim(text);
    let body = trimmed
        .strip_prefix("0x")
        .or_else(|| trimmed.strip_prefix("0X"))
        .unwrap_or(trimmed);
    if body.is_empty() || !body.chars().all(|c| c.is_ascii_hexdigit()) {
        return None;
    }
    u128::from_str_radix(body, 16).ok()
}

/// ECMAScript `ToInt32`.
///
/// PORT NOTE (trap: `| 0` wraps mod 2^32, `as i32` saturates). `1e10 as i32`
/// is `2147483647` in Rust and `1410065408` in JavaScript. Everything that
/// mirrors a JS bitwise operator goes through here.
pub fn to_int32(value: f64) -> i32 {
    if !value.is_finite() {
        return 0;
    }
    let truncated = value.trunc();
    let wrapped = truncated.rem_euclid(4_294_967_296.0);
    if wrapped >= 2_147_483_648.0 {
        (wrapped - 4_294_967_296.0) as i32
    } else {
        wrapped as i32
    }
}

/// `Number#toString(16)`.
///
/// PORT NOTE (trap: `format!("{:x}")` handles neither sign nor fraction).
/// A negative number stringifies with a leading `-`, and a fractional one keeps
/// hex fraction digits; both then flow into `padStart(8, "0")`, which pads but
/// never truncates, so the result can be longer than eight characters and is
/// then used as a filename stem. That malformed stem is reproduced here rather
/// than repaired.
pub fn to_string_radix16(value: f64) -> String {
    if value.is_nan() {
        return "NaN".to_string();
    }
    if value.is_infinite() {
        return if value > 0.0 { "Infinity" } else { "-Infinity" }.to_string();
    }
    if value == 0.0 {
        return "0".to_string();
    }
    let negative = value < 0.0;
    let magnitude = value.abs();
    let mut integer = magnitude.trunc();
    let mut fraction = magnitude - integer;

    let mut digits: Vec<u8> = Vec::new();
    if integer == 0.0 {
        digits.push(b'0');
    } else {
        let mut reversed = Vec::new();
        while integer >= 1.0 {
            let digit = (integer % 16.0) as u32;
            reversed.push(hex_digit(digit));
            integer = (integer / 16.0).trunc();
        }
        reversed.reverse();
        digits = reversed;
    }
    let mut out = String::from_utf8(digits).expect("hex digits are ASCII");
    if fraction > 0.0 {
        out.push('.');
        // Hex fraction digits of a binary float terminate exactly.
        let mut guard = 0;
        while fraction > 0.0 && guard < 1100 {
            fraction *= 16.0;
            let digit = fraction.trunc();
            out.push(hex_digit(digit as u32) as char);
            fraction -= digit;
            guard += 1;
        }
    }
    if negative {
        format!("-{out}")
    } else {
        out
    }
}

fn hex_digit(value: u32) -> u8 {
    match value {
        0..=9 => b'0' + value as u8,
        _ => b'a' + (value - 10) as u8,
    }
}

/// `String#padStart(width, "0")`.
///
/// PORT NOTE (trap: `padStart` pads but never truncates). `format!("{:08x}")`
/// would silently widen *and* is unable to express the over-length case;
/// `hex_stem(-1)` is `"-1"` padded to `"000000-1"` in JavaScript, and a
/// nine-character input comes back unchanged.
pub fn pad_start_zero(text: &str, width: usize) -> String {
    let length = text.encode_utf16().count();
    if length >= width {
        return text.to_string();
    }
    let mut out = "0".repeat(width - length);
    out.push_str(text);
    out
}

/// `row.entry.toString(16).padStart(8, "0")`.
pub fn hex_stem(value: f64) -> String {
    pad_start_zero(&to_string_radix16(value), 8)
}

/// The ordering `Array#sort((l, r) => l - r)` actually produces.
///
/// PORT NOTE (trap: a comparator returning NaN is spec-treated as `+0`, i.e.
/// "equal"). `f64::partial_cmp` returns `None` and `f64::total_cmp` orders NaN
/// past infinity; both model this wrongly. `Infinity - Infinity` is NaN too, so
/// two rows that both carry `Infinity` compare *equal*, not identical-and-
/// ordered.
pub fn subtract_ordering(left: f64, right: f64) -> std::cmp::Ordering {
    let difference = left - right;
    if difference < 0.0 {
        std::cmp::Ordering::Less
    } else if difference > 0.0 {
        std::cmp::Ordering::Greater
    } else {
        // Covers +0, -0 and NaN, exactly as the specification's coercion does.
        std::cmp::Ordering::Equal
    }
}

/// `Number.isFinite`-free helper: is this value one JavaScript would print
/// without a fractional part and without a sign?
pub fn is_js_space_only(text: &str) -> bool {
    !text.is_empty() && text.chars().all(is_js_space)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn hex_prefix_is_accepted_where_from_str_radix_refuses() {
        assert_eq!(js_number("0x0800abcd"), 134_261_709.0);
        assert!(i64::from_str_radix("0x0800abcd", 16).is_err());
        assert_eq!(parse_hex("0x08000010"), Some(0x0800_0010));
        assert_eq!(parse_hex("08000010"), Some(0x0800_0010));
        assert_eq!(parse_hex("0X0800abcd"), Some(0x0800_abcd));
    }

    #[test]
    fn number_is_not_parse_int() {
        // `parseInt("8x", 10)` is 8; `Number("8x")` is NaN.
        assert!(js_number("8x").is_nan());
        // `parseInt("1.5")` is 1; `Number("1.5")` is 1.5.
        assert_eq!(js_number("1.5"), 1.5);
        assert_eq!(js_number(""), 0.0);
        assert_eq!(js_number("   "), 0.0);
        assert!(js_number("inf").is_nan());
        assert_eq!(js_number("Infinity"), f64::INFINITY);
    }

    #[test]
    fn to_int32_wraps_where_as_i32_saturates() {
        assert_eq!(to_int32(1e10), 1_410_065_408);
        assert_eq!(1e10_f64 as i32, i32::MAX);
        assert_eq!(to_int32(f64::NAN), 0);
        assert_eq!(to_int32(-1.0), -1);
    }

    #[test]
    fn hex_stem_reproduces_the_malformed_cases() {
        assert_eq!(hex_stem(134_217_952.0), "080000e0");
        // padStart pads, never truncates.
        assert_eq!(hex_stem(-1.0), "000000-1");
        assert_eq!(hex_stem(0.5), "000000.8");
        assert_eq!(hex_stem(f64::NAN), "00000NaN");
        // Already wide enough: returned unchanged, not truncated.
        assert_eq!(hex_stem(0xFFFF_FFFF_u32 as f64), "ffffffff");
        assert_eq!(hex_stem(0x1_0000_0000_u64 as f64), "100000000");
    }

    #[test]
    fn nan_comparator_is_equal_not_unordered() {
        assert_eq!(
            subtract_ordering(f64::NAN, 1.0),
            std::cmp::Ordering::Equal
        );
        assert_eq!(
            subtract_ordering(f64::INFINITY, f64::INFINITY),
            std::cmp::Ordering::Equal
        );
        assert!(f64::NAN.partial_cmp(&1.0).is_none());
        assert_eq!(f64::NAN.total_cmp(&1.0), std::cmp::Ordering::Greater);
    }

    #[test]
    fn space_only_helper() {
        assert!(is_js_space_only("\u{FEFF} "));
        assert!(!is_js_space_only(""));
    }
}
