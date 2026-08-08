// `Number(string)` and `Number.isSafeInteger`, because the TypeScript relies on
// both in ways `f64::from_str` does not reproduce.
//
// The export summary sums `Number(source.size)` over records whose `size` field
// is the text "0x00000098". ECMAScript's StringNumericLiteral grammar reads
// that as hexadecimal (152); `f64::from_str` rejects it outright, which would
// turn the `bytes=1540` line into a parse failure. The same call decides the
// ROM address in `verify`, so getting it wrong there would compare against the
// wrong span rather than merely printing a wrong total.

/// `Number(text)`. Returns NaN for anything the grammar rejects.
pub fn to_number(text: &str) -> f64 {
    let trimmed = text.trim_matches(is_js_whitespace);
    if trimmed.is_empty() {
        return 0.0; // Number("") is 0, not NaN
    }
    if let Some(rest) = strip_prefix_ci(trimmed, "0x") {
        return radix(rest, 16);
    }
    if let Some(rest) = strip_prefix_ci(trimmed, "0o") {
        return radix(rest, 8);
    }
    if let Some(rest) = strip_prefix_ci(trimmed, "0b") {
        return radix(rest, 2);
    }
    let (sign, body) = match trimmed.strip_prefix('-') {
        Some(rest) => (-1.0, rest),
        None => (1.0, trimmed.strip_prefix('+').unwrap_or(trimmed)),
    };
    if body == "Infinity" {
        return sign * f64::INFINITY;
    }
    if !is_decimal_literal(body) {
        return f64::NAN;
    }
    match body.parse::<f64>() {
        Ok(value) => sign * value,
        Err(_) => f64::NAN,
    }
}

/// JS `StrWhiteSpace`.
///
/// PORT NOTE: this is deliberately not Rust's `char::is_whitespace`. JS counts
/// U+FEFF (ZWNBSP) as whitespace and does NOT count U+0085 (NEL); Rust's
/// `White_Space` property is the exact reverse on both characters.
fn is_js_whitespace(ch: char) -> bool {
    matches!(
        ch,
        '\u{9}' | '\u{a}' | '\u{b}' | '\u{c}' | '\u{d}' | '\u{20}'
            | '\u{a0}' | '\u{feff}' | '\u{1680}' | '\u{2000}'..='\u{200a}'
            | '\u{2028}' | '\u{2029}' | '\u{202f}' | '\u{205f}' | '\u{3000}'
    )
}

fn strip_prefix_ci<'a>(text: &'a str, prefix: &str) -> Option<&'a str> {
    let head = text.get(..prefix.len())?;
    if head.eq_ignore_ascii_case(prefix) {
        Some(&text[prefix.len()..])
    } else {
        None
    }
}

fn radix(digits: &str, base: u32) -> f64 {
    if digits.is_empty() {
        return f64::NAN;
    }
    let mut value = 0.0f64;
    for ch in digits.chars() {
        match ch.to_digit(base) {
            Some(digit) => value = value * f64::from(base) + f64::from(digit),
            None => return f64::NAN,
        }
    }
    value
}

/// StrUnsignedDecimalLiteral without `Infinity`: digits with an optional
/// fraction and exponent, or a bare fraction. Rejects "12abc", "1_0", "inf",
/// "NaN" and every other spelling `f64::from_str` would otherwise accept.
fn is_decimal_literal(body: &str) -> bool {
    let bytes = body.as_bytes();
    let mut at = 0;
    let integral = digits(bytes, &mut at);
    let mut fractional = 0;
    if bytes.get(at) == Some(&b'.') {
        at += 1;
        fractional = digits(bytes, &mut at);
    }
    if integral == 0 && fractional == 0 {
        return false;
    }
    if matches!(bytes.get(at), Some(b'e') | Some(b'E')) {
        at += 1;
        if matches!(bytes.get(at), Some(b'+') | Some(b'-')) {
            at += 1;
        }
        if digits(bytes, &mut at) == 0 {
            return false;
        }
    }
    at == bytes.len()
}

fn digits(bytes: &[u8], at: &mut usize) -> usize {
    let start = *at;
    while bytes.get(*at).is_some_and(u8::is_ascii_digit) {
        *at += 1;
    }
    *at - start
}

/// `Number.isSafeInteger(value)`.
///
/// PORT NOTE: JS has one number type, so `1.0` IS an integer here. Anything
/// that reaches this came out of `JSON.parse`, where `1` and `1.0` are the same
/// f64, and the TypeScript accepts both.
pub fn is_safe_integer(value: f64) -> bool {
    value.is_finite() && value.fract() == 0.0 && value.abs() <= 9_007_199_254_740_991.0
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn hex_sizes_from_the_records_parse_as_hexadecimal() {
        assert_eq!(to_number("0x00000098"), 152.0);
        assert_eq!(to_number("0x08033e60"), 134_430_304.0);
        assert_eq!(to_number("0X10"), 16.0);
        assert_eq!(to_number("0o17"), 15.0);
        assert_eq!(to_number("0b101"), 5.0);
    }

    #[test]
    fn empty_and_blank_are_zero_but_garbage_is_nan() {
        assert_eq!(to_number(""), 0.0);
        assert_eq!(to_number("   \u{feff}"), 0.0);
        assert!(to_number("12abc").is_nan());
        assert!(to_number("inf").is_nan());
        assert!(to_number("NaN").is_nan());
        assert!(to_number("1_0").is_nan());
        assert!(to_number("0x").is_nan());
        assert!(to_number("-0x10").is_nan(), "the hex form takes no sign");
    }

    #[test]
    fn decimal_forms_match_the_grammar() {
        assert_eq!(to_number(" 1.5 "), 1.5);
        assert_eq!(to_number("+.5"), 0.5);
        assert_eq!(to_number("5."), 5.0);
        assert_eq!(to_number("-1e3"), -1000.0);
        assert_eq!(to_number("Infinity"), f64::INFINITY);
        // U+0085 is whitespace to Rust but not to JS.
        assert!(to_number("\u{85}1").is_nan());
    }

    #[test]
    fn one_point_zero_is_a_safe_integer() {
        assert!(is_safe_integer(1.0));
        assert!(!is_safe_integer(1.5));
        assert!(!is_safe_integer(f64::NAN));
        assert!(!is_safe_integer(9_007_199_254_740_993.0));
    }
}
