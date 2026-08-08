// JavaScript `Number(string)` and `String(number)` semantics.
//
// WHY THIS EXISTS: `tools/lib/rtl_insn.ts` funnels every numeric field it
// extracts from a gcc RTL dump through `Number(atomText)` and then gates on
// `Number.isFinite`. That is *not* `f64::from_str`. The differences bite on
// exactly the shapes an RTL dump can contain:
//
//   * `Number("0x9")` is 9 (gcc prints bracketed hex hints like `[0x9]`, and
//     `parseRtlExpr`'s `const_int` branch has no digit regex guarding it, so a
//     hex atom really can reach `Number`). Rust's `from_str` rejects it.
//   * `Number("")` and `Number("   ")` are 0, not an error.
//   * `Number("12abc")` is NaN -- ECMAScript requires the WHOLE string to be a
//     StringNumericLiteral, unlike `parseInt`.
//   * `Number("inf")` / `Number("nan")` are NaN; only the exact spelling
//     `Infinity` (with an optional sign) is accepted. Rust's `from_str`
//     happily accepts "inf", "infinity" and "NaN" in any case, which would
//     silently turn a garbage atom into a finite-check pass or a different
//     value.
//
// The rendering direction matters too: JS has one number type, so a `uid` of
// `3` prints as `3`, never `3.0`, and `-0` prints as `0`. The parity harness
// compares `JSON.stringify` output byte-for-byte, so these must match.

/// The character class JavaScript's `\s` (and `String.prototype.trim`) match.
///
/// PORT NOTE: deliberately not `char::is_whitespace`. JS `\s` includes U+FEFF
/// and excludes U+0085 (NEL); Rust's `White_Space` property is the reverse.
fn is_js_space(ch: char) -> bool {
    matches!(
        ch,
        '\t' | '\n'
            | '\u{0b}'
            | '\u{0c}'
            | '\r'
            | ' '
            | '\u{a0}'
            | '\u{1680}'
            | '\u{2000}'..='\u{200a}'
            | '\u{2028}'
            | '\u{2029}'
            | '\u{202f}'
            | '\u{205f}'
            | '\u{3000}'
            | '\u{feff}'
    )
}

fn radix_value(text: &str, radix: u32) -> f64 {
    if text.is_empty() {
        return f64::NAN;
    }
    let mut accumulated = 0.0f64;
    for ch in text.chars() {
        match ch.to_digit(radix) {
            // PORT NOTE: accumulating in f64 (not u64) is what the spec's
            // MV -> Number rounding amounts to, and it is what keeps a
            // >64-bit hex literal from wrapping instead of going inexact.
            Some(digit) => accumulated = accumulated * f64::from(radix) + f64::from(digit),
            None => return f64::NAN,
        }
    }
    accumulated
}

/// ECMAScript `Number(value)` for a string argument.
pub fn js_number(raw: &str) -> f64 {
    let text = raw.trim_matches(is_js_space);
    if text.is_empty() {
        return 0.0;
    }
    if let Some(rest) = text.strip_prefix("0x").or_else(|| text.strip_prefix("0X")) {
        return radix_value(rest, 16);
    }
    if let Some(rest) = text.strip_prefix("0o").or_else(|| text.strip_prefix("0O")) {
        return radix_value(rest, 8);
    }
    if let Some(rest) = text.strip_prefix("0b").or_else(|| text.strip_prefix("0B")) {
        return radix_value(rest, 2);
    }
    let (sign, digits) = match text.as_bytes()[0] {
        b'+' => (1.0, &text[1..]),
        b'-' => (-1.0, &text[1..]),
        _ => (1.0, text),
    };
    if digits == "Infinity" {
        return sign * f64::INFINITY;
    }
    // Everything Rust's parser accepts but the StringNumericLiteral grammar
    // does not: the inf/nan word forms, and underscores.
    if digits.is_empty()
        || digits.starts_with('+')
        || digits.starts_with('-')
        || digits.contains('_')
        || digits
            .chars()
            .any(|ch| !matches!(ch, '0'..='9' | '.' | 'e' | 'E' | '+' | '-'))
    {
        return f64::NAN;
    }
    // JS accepts a trailing `.` ("5." is 5) and a leading `.` (".5"). Rust
    // accepts both as well, but a bare "." is NaN in JS and an error in Rust.
    if digits == "." {
        return f64::NAN;
    }
    match digits.parse::<f64>() {
        Ok(value) => sign * value,
        Err(_) => f64::NAN,
    }
}

/// ECMAScript `String(number)` / the JSON serialization of a finite number.
///
/// PORT NOTE: only the integral range is reproduced exactly, because every
/// numeric field in an RTL dump is an integer. Non-integral values fall back
/// to Rust's shortest round-trip formatting, which agrees with JS for the
/// values that can reach here (a malformed `(reg:SI 3.5 r3)`) but is not a
/// general implementation of the Number::toString algorithm.
pub fn js_number_to_string(value: f64) -> String {
    if value.is_nan() {
        return "NaN".to_string();
    }
    if value.is_infinite() {
        return if value > 0.0 { "Infinity" } else { "-Infinity" }.to_string();
    }
    if value == 0.0 {
        // Covers -0.0, which JS prints as "0".
        return "0".to_string();
    }
    if value.fract() == 0.0 && value.abs() < 1e21 {
        return format!("{}", value as i128);
    }
    format!("{value}")
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn hex_octal_and_binary_literals_parse_like_js() {
        assert_eq!(js_number("0x9"), 9.0);
        assert_eq!(js_number("0X1f"), 31.0);
        assert_eq!(js_number("0b101"), 5.0);
        assert_eq!(js_number("0o17"), 15.0);
        // A sign is not allowed in front of a radix prefix.
        assert!(js_number("-0x9").is_nan());
    }

    #[test]
    fn empty_and_whitespace_are_zero() {
        assert_eq!(js_number(""), 0.0);
        assert_eq!(js_number("   \n\t"), 0.0);
        // U+FEFF is trimmed by JS but is not Rust `White_Space`.
        assert_eq!(js_number("\u{feff}7\u{feff}"), 7.0);
    }

    #[test]
    fn trailing_garbage_is_nan_unlike_parse_int() {
        assert!(js_number("12abc").is_nan());
        assert!(js_number("9 9").is_nan());
        assert!(js_number("--3").is_nan());
    }

    #[test]
    fn rust_only_word_forms_are_rejected() {
        // These all parse in Rust and must not here.
        assert!(js_number("inf").is_nan());
        assert!(js_number("infinity").is_nan());
        assert!(js_number("NaN").is_nan());
        assert!(js_number("nan").is_nan());
        assert!(js_number("1_0").is_nan());
        assert!(js_number(".").is_nan());
        // ...but the exact JS spelling works.
        assert_eq!(js_number("Infinity"), f64::INFINITY);
        assert_eq!(js_number("-Infinity"), f64::NEG_INFINITY);
    }

    #[test]
    fn signed_and_fractional_decimals() {
        assert_eq!(js_number("-42"), -42.0);
        assert_eq!(js_number("+42"), 42.0);
        assert_eq!(js_number("5."), 5.0);
        assert_eq!(js_number(".5"), 0.5);
        assert_eq!(js_number("1e3"), 1000.0);
    }

    #[test]
    fn integers_render_without_a_decimal_point() {
        assert_eq!(js_number_to_string(3.0), "3");
        assert_eq!(js_number_to_string(-0.0), "0");
        assert_eq!(js_number_to_string(0.0), "0");
        assert_eq!(js_number_to_string(-1073741824.0), "-1073741824");
        assert_eq!(js_number_to_string(3.5), "3.5");
    }
}
