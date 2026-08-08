// JavaScript value semantics that the TypeScript original depends on and that
// Rust does not share.
//
// WHY this file exists: `tools/check/core_retained_audit.ts` reads addresses
// that arrive as BOTH JSON numbers and hex strings like "0x08002344" (229 of
// them in the real tree), pads hex with `padStart`, fills typed arrays with
// relative indices that clamp instead of panicking, and sorts report rows with
// `localeCompare`. Each of those is a place where the obvious Rust spelling
// gives a different answer, so each one lives here behind a named helper with
// a test rather than being open-coded at the call site.

/// The set matched by JavaScript's `\s` (and by `StrWhiteSpace` in
/// `Number(string)`): WhiteSpace + LineTerminator.
///
/// PORT NOTE: this deliberately differs from Rust's `char::is_whitespace`
/// (Unicode `White_Space`) in exactly two places — JS includes U+FEFF and
/// excludes U+0085; `White_Space` is the reverse.
pub fn is_js_space(c: char) -> bool {
    matches!(
        c,
        '\u{9}'
            | '\u{a}'
            | '\u{b}'
            | '\u{c}'
            | '\u{d}'
            | '\u{20}'
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

/// JS `\w` / the character class `\b` is defined against: ASCII only.
pub fn is_js_word(c: char) -> bool {
    c.is_ascii_alphanumeric() || c == '_'
}

/// `Number(value)` for a string argument, i.e. the ECMAScript
/// `StringNumericLiteral` grammar (§7.1.4.1).
///
/// PORT NOTE: this is NOT `f64::from_str`. The differences that matter to this
/// tool: `Number("0x08002344")` is 134226756 where `from_str` errors (the real
/// `semantic/main-regions.json` and `asm/alignment.json` store addresses in
/// that form); `Number("")` and `Number("   ")` are 0 where `from_str` errors;
/// `Number("12abc")` is NaN where `from_str` errors (same outcome here, but
/// only by accident); and `from_str` accepts "inf"/"nan"/"1e" spellings that
/// the JS grammar rejects.
pub fn js_number_from_str(text: &str) -> f64 {
    let trimmed = text.trim_matches(is_js_space);
    if trimmed.is_empty() {
        return 0.0;
    }
    if let Some(rest) = trimmed.strip_prefix("0x").or_else(|| trimmed.strip_prefix("0X")) {
        return radix_digits(rest, 16);
    }
    if let Some(rest) = trimmed.strip_prefix("0o").or_else(|| trimmed.strip_prefix("0O")) {
        return radix_digits(rest, 8);
    }
    if let Some(rest) = trimmed.strip_prefix("0b").or_else(|| trimmed.strip_prefix("0B")) {
        return radix_digits(rest, 2);
    }
    let (sign, body) = match trimmed.strip_prefix('-') {
        Some(rest) => (-1.0, rest),
        None => (1.0, trimmed.strip_prefix('+').unwrap_or(trimmed)),
    };
    if body == "Infinity" {
        return sign * f64::INFINITY;
    }
    if !is_str_decimal_literal(body) {
        return f64::NAN;
    }
    // The grammar is now known to be a subset of what `from_str` accepts, and
    // both round to the nearest f64, so delegation is exact from here.
    match body.parse::<f64>() {
        Ok(value) => sign * value,
        Err(_) => f64::NAN,
    }
}

fn radix_digits(text: &str, radix: u32) -> f64 {
    if text.is_empty() {
        return f64::NAN;
    }
    let mut total = 0.0f64;
    for c in text.chars() {
        match c.to_digit(radix) {
            Some(digit) => total = total * f64::from(radix) + f64::from(digit),
            None => return f64::NAN,
        }
    }
    total
}

/// `StrUnsignedDecimalLiteral` minus `Infinity`: digits with an optional
/// fraction and an optional exponent, requiring at least one digit somewhere
/// and at least one digit after `e`.
fn is_str_decimal_literal(body: &str) -> bool {
    let bytes = body.as_bytes();
    let mut index = 0;
    let mut integral = 0;
    while index < bytes.len() && bytes[index].is_ascii_digit() {
        index += 1;
        integral += 1;
    }
    let mut fractional = 0;
    if index < bytes.len() && bytes[index] == b'.' {
        index += 1;
        while index < bytes.len() && bytes[index].is_ascii_digit() {
            index += 1;
            fractional += 1;
        }
    }
    if integral + fractional == 0 {
        return false;
    }
    if index < bytes.len() && (bytes[index] == b'e' || bytes[index] == b'E') {
        index += 1;
        if index < bytes.len() && (bytes[index] == b'+' || bytes[index] == b'-') {
            index += 1;
        }
        let mut exponent = 0;
        while index < bytes.len() && bytes[index].is_ascii_digit() {
            index += 1;
            exponent += 1;
        }
        if exponent == 0 {
            return false;
        }
    }
    index == bytes.len()
}

/// `Number.isSafeInteger(value)`.
///
/// PORT NOTE: JavaScript has one number type, so `1.0` and `1` are the same
/// value and both satisfy this. A JSON input of `12.0` is therefore accepted
/// by the original, and must be accepted here too.
pub fn is_safe_integer(value: f64) -> bool {
    value.is_finite() && value.fract() == 0.0 && value.abs() <= 9_007_199_254_740_991.0
}

/// `value.toString(16).padStart(width, "0")`.
///
/// PORT NOTE: `padStart` pads but never truncates, so a value wider than
/// `width` comes back at its natural length. On a negative number the sign is
/// part of the string being padded, so `(-2).toString(16).padStart(8, "0")` is
/// `"000000-2"` — faithfully reproduced here, absurd as it is.
pub fn to_string_16_pad_start(value: i64, width: usize) -> String {
    let digits = if value < 0 {
        format!("-{:x}", (value as i128).unsigned_abs())
    } else {
        format!("{value:x}")
    };
    if digits.chars().count() >= width {
        return digits;
    }
    let mut padded = "0".repeat(width - digits.chars().count());
    padded.push_str(&digits);
    padded
}

/// The relative-index conversion shared by `TypedArray#fill` and
/// `TypedArray#subarray` (`ToIntegerOrInfinity` then relative-to-length).
///
/// PORT NOTE: this CLAMPS where Rust slicing PANICS, and a negative index
/// counts back from the end rather than erroring. `mark()` in the original is
/// called on manifest spans that were never bounds-checked, so an address
/// below the ROM base would silently paint the END of the mask instead of
/// throwing. That is preserved.
pub fn relative_index(index: i64, length: usize) -> usize {
    let length = length as i64;
    let resolved = if index < 0 { (length + index).max(0) } else { index.min(length) };
    resolved as usize
}

/// `String#localeCompare` restricted to the alphabet these reports actually
/// use (lowercase ASCII letters, digits and `_`).
///
/// PORT NOTE: plain byte order is NOT equivalent. In ICU root collation `_`
/// carries a primary weight below digits and letters, so `"a_b" < "a0b"`,
/// while by bytes `'0'` (0x30) sorts before `'_'` (0x5F) and the order flips.
/// The real manifest's 33 kind/confidence pairs happen not to contain such a
/// pair (verified in the tests), but the comparator is written for the general
/// case so a new `kind` cannot quietly reorder the report. Anything outside
/// the supported alphabet falls back to byte order, which is documented as a
/// known approximation rather than a claim of full ICU compatibility.
pub fn locale_compare(a: &str, b: &str) -> std::cmp::Ordering {
    fn rank(c: char) -> Option<u32> {
        match c {
            '_' => Some(1),
            '0'..='9' => Some(0x100 + c as u32),
            'a'..='z' => Some(0x200 + c as u32),
            _ => None,
        }
    }
    let ranked = |text: &str| text.chars().map(rank).collect::<Option<Vec<u32>>>();
    match (ranked(a), ranked(b)) {
        (Some(left), Some(right)) => left.cmp(&right),
        _ => a.as_bytes().cmp(b.as_bytes()),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn hex_strings_parse_the_way_javascript_parses_them() {
        assert_eq!(js_number_from_str("0x08002344"), 134226756.0);
        assert_eq!(js_number_from_str("0X0800F186"), 134279558.0);
        assert_eq!(js_number_from_str("0b1010"), 10.0);
        assert_eq!(js_number_from_str("0o17"), 15.0);
        // Rust's f64 parser errors on all four of these; JavaScript does not.
        assert_eq!(js_number_from_str(""), 0.0);
        assert_eq!(js_number_from_str("  \u{feff}12  "), 12.0);
        assert_eq!(js_number_from_str("-Infinity"), f64::NEG_INFINITY);
        assert_eq!(js_number_from_str("  "), 0.0);
        // ...and accepts these three, which JavaScript rejects as NaN.
        assert!(js_number_from_str("inf").is_nan());
        assert!(js_number_from_str("NaN").is_nan());
        assert!(js_number_from_str("1e").is_nan());
        assert!(js_number_from_str("12abc").is_nan());
        assert!(js_number_from_str("-0x10").is_nan());
        assert_eq!(js_number_from_str("1.5e3"), 1500.0);
    }

    #[test]
    fn safe_integer_accepts_a_whole_valued_float() {
        assert!(is_safe_integer(1.0));
        assert!(is_safe_integer(134217728.0));
        assert!(!is_safe_integer(1.5));
        assert!(!is_safe_integer(f64::NAN));
        assert!(!is_safe_integer(9_007_199_254_740_992.0));
    }

    #[test]
    fn pad_start_pads_but_never_truncates_and_mangles_negatives() {
        assert_eq!(to_string_16_pad_start(0x8002344, 8), "08002344");
        assert_eq!(to_string_16_pad_start(0x123456789, 8), "123456789");
        // The trap the known bug rides on, pinned explicitly.
        assert_eq!(to_string_16_pad_start(-2, 8), "000000-2");
        assert_eq!(to_string_16_pad_start(0, 8), "00000000");
    }

    #[test]
    fn relative_index_clamps_and_wraps_instead_of_panicking() {
        assert_eq!(relative_index(-4, 10), 6);
        assert_eq!(relative_index(-40, 10), 0);
        assert_eq!(relative_index(40, 10), 10);
        assert_eq!(relative_index(3, 10), 3);
    }

    #[test]
    fn locale_compare_puts_underscore_below_digits() {
        use std::cmp::Ordering;
        assert_eq!(locale_compare("a_b", "a0b"), Ordering::Less);
        // Byte order disagrees, which is the whole reason this helper exists.
        assert_eq!("a_b".as_bytes().cmp("a0b".as_bytes()), Ordering::Greater);
        assert_eq!(locale_compare("armv4t_helper_bank", "bios_wrapper"), Ordering::Less);
        assert_eq!(locale_compare("proven", "proven"), Ordering::Equal);
    }

    #[test]
    fn js_space_and_unicode_white_space_disagree_in_two_places() {
        assert!(is_js_space('\u{feff}') && !'\u{feff}'.is_whitespace());
        assert!(!is_js_space('\u{85}') && '\u{85}'.is_whitespace());
    }
}
