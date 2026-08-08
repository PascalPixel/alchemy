// JavaScript scalar semantics that this port depends on byte-for-byte.
//
// WHY this file exists: compose_scene.ts reads untrusted JSON into `Number`,
// formats counts into stdout with template literals, and derives catalog keys
// with `(n).toString(16)`. Rust's `Display`, `f64::from_str` and `format!("{:x}")`
// all disagree with the ECMAScript operations in ways that are reachable from
// the tracked asset tree, so each one is reimplemented here rather than
// approximated at the call site.

/// ECMAScript `Number.isInteger`.
///
/// PORT NOTE: JavaScript has one number type, so `1.0` *is* an integer here.
/// `decode_entry` leans on this: a metatile token that parsed to `1.0` must be
/// accepted, and one that parsed to `NaN` must be rejected.
pub fn is_integer(value: f64) -> bool {
    value.is_finite() && value.fract() == 0.0
}

/// ECMAScript `Number.isSafeInteger`.
pub fn is_safe_integer(value: f64) -> bool {
    is_integer(value) && value.abs() <= 9_007_199_254_740_991.0
}

/// ECMAScript `ToString(Number)` (spec 6.1.6.1.20).
///
/// PORT NOTE: not Rust's `Display`. JS writes an integral double as `1` where
/// Rust writes `1.0`, and switches to exponential notation below 1e-6 where
/// Rust never does. Reachable here through `map_index`, which arrives from
/// `map_load_table.json` as an arbitrary JSON number and is interpolated into
/// both the `--all` progress lines and the `scene_<cid>_map<NNN>.png` filename.
pub fn number_to_string(value: f64) -> String {
    if value.is_nan() {
        return "NaN".to_string();
    }
    if value == 0.0 {
        return "0".to_string();
    }
    if value < 0.0 {
        return format!("-{}", number_to_string(-value));
    }
    if value.is_infinite() {
        return "Infinity".to_string();
    }
    let exponential = format!("{value:e}");
    let (mantissa, exponent) = exponential.split_once('e').expect("{:e} always emits an exponent");
    let digits: String = mantissa.chars().filter(|character| *character != '.').collect();
    let digits = digits.trim_end_matches('0');
    let digits = if digits.is_empty() { "0" } else { digits };
    let k = digits.len() as i32;
    let n = exponent.parse::<i32>().expect("{:e} always emits an integer exponent") + 1;

    if k <= n && n <= 21 {
        return format!("{digits}{}", "0".repeat((n - k) as usize));
    }
    if 0 < n && n <= 21 {
        return format!("{}.{}", &digits[..n as usize], &digits[n as usize..]);
    }
    if -6 < n && n <= 0 {
        return format!("0.{}{digits}", "0".repeat((-n) as usize));
    }
    let tail = if n > 0 { format!("e+{}", n - 1) } else { format!("e-{}", 1 - n) };
    if k == 1 {
        return format!("{digits}{tail}");
    }
    format!("{}.{}{tail}", &digits[..1], &digits[1..])
}

/// ECMAScript `Number.prototype.toString(16)`, restricted to the finite
/// integral values `graphics_catalog` can produce.
///
/// PORT NOTE: the TypeScript reaches this with `Number.parseInt(familyId, 16)`
/// plus a small offset. A family id that is not hex gives `NaN`, and
/// `NaN.toString(16)` is the string `"NaN"` -- a perfectly usable (never
/// matching) catalog key, so it is reproduced rather than treated as an error.
/// Negative values get a leading `-`, matching JS and not two's complement.
pub fn to_string_radix16(value: f64) -> String {
    if value.is_nan() {
        return "NaN".to_string();
    }
    if value.is_infinite() {
        return if value > 0.0 { "Infinity".into() } else { "-Infinity".into() };
    }
    if value < 0.0 {
        return format!("-{}", to_string_radix16(-value));
    }
    // Every catalog key in the tracked manifest is a small non-negative
    // integer; fractional or huge values cannot arrive from a hex parseInt.
    let integral = value.trunc();
    format!("{:x}", integral as u128)
}

/// ECMAScript `Number.parseInt(text, 16)`.
///
/// PORT NOTE: skips leading ECMAScript whitespace, accepts an optional sign and
/// an optional `0x`/`0X` prefix, consumes the longest run of hex digits, and
/// yields `NaN` when that run is empty. `i64::from_str_radix` accepts none of
/// that shape and rejects trailing junk that JS silently truncates.
pub fn parse_int_radix16(text: &str) -> f64 {
    let units: Vec<char> = text.chars().collect();
    let mut index = 0usize;
    while index < units.len() && is_js_whitespace(units[index]) {
        index += 1;
    }
    let mut sign = 1.0f64;
    if index < units.len() && (units[index] == '+' || units[index] == '-') {
        if units[index] == '-' {
            sign = -1.0;
        }
        index += 1;
    }
    if index + 1 < units.len()
        && units[index] == '0'
        && (units[index + 1] == 'x' || units[index + 1] == 'X')
    {
        index += 2;
    }
    let start = index;
    let mut value = 0.0f64;
    while index < units.len() {
        match units[index].to_digit(16) {
            Some(digit) => {
                value = value * 16.0 + f64::from(digit);
                index += 1;
            }
            None => break,
        }
    }
    if index == start {
        return f64::NAN;
    }
    sign * value
}

/// The set matched by JavaScript's `\s`, which is also what `String#trim`
/// strips: Unicode `Space_Separator` plus tab/LF/VT/FF/CR, NBSP, U+FEFF and
/// the two line separators.
///
/// PORT NOTE: this deliberately differs from Rust's `char::is_whitespace`.
/// U+FEFF is whitespace to JS and not to Rust; U+0085 is whitespace to Rust and
/// not to JS. `parse_metatiles` splits on `/\s+/`, so a tracked `.tilemap` with
/// a stray BOM must tokenise the same in both implementations.
pub fn is_js_whitespace(character: char) -> bool {
    matches!(
        character,
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

/// `String.prototype.trim`.
pub fn js_trim(text: &str) -> &str {
    text.trim_matches(is_js_whitespace)
}

/// `text.split(/\s+/).filter(Boolean)` -- the filter drops the empty leading
/// and trailing fields JS produces when the string starts or ends with space.
pub fn split_whitespace(text: &str) -> Vec<&str> {
    text.split(is_js_whitespace).filter(|token| !token.is_empty()).collect()
}

/// `String.prototype.padStart(width, "0")`.
///
/// PORT NOTE: padding counts UTF-16 code units and never truncates. A negative
/// number pads in front of its sign (`(-2).toString().padStart(3, "0")` is
/// `"0-2"`), which is preserved rather than corrected.
pub fn pad_start_zero(text: &str, width: usize) -> String {
    let length = text.encode_utf16().count();
    if length >= width {
        return text.to_string();
    }
    format!("{}{text}", "0".repeat(width - length))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn integral_doubles_print_without_a_decimal_point() {
        assert_eq!(number_to_string(1.0), "1");
        assert_eq!(number_to_string(0.0), "0");
        assert_eq!(number_to_string(-2.0), "-2");
        assert_eq!(number_to_string(1.5), "1.5");
        assert_eq!(number_to_string(f64::NAN), "NaN");
        // Below 1e-6 JS switches to exponential; Rust's Display never does.
        assert_eq!(number_to_string(5.960464477539063e-8), "5.960464477539063e-8");
        assert_eq!(number_to_string(1e21), "1e+21");
    }

    #[test]
    fn one_point_zero_is_an_integer_because_js_has_one_number_type() {
        assert!(is_integer(1.0));
        assert!(!is_integer(1.5));
        assert!(!is_integer(f64::NAN));
        assert!(!is_integer(f64::INFINITY));
        assert!(is_safe_integer(9_007_199_254_740_991.0));
        assert!(!is_safe_integer(9_007_199_254_740_992.0));
    }

    #[test]
    fn parse_int_truncates_trailing_junk_and_nans_on_no_digits() {
        assert_eq!(parse_int_radix16("128"), 296.0);
        assert_eq!(parse_int_radix16("0x1f"), 31.0);
        assert_eq!(parse_int_radix16("  12zz"), 18.0);
        assert_eq!(parse_int_radix16("-a"), -10.0);
        assert!(parse_int_radix16("zz").is_nan());
        assert!(parse_int_radix16("").is_nan());
        // "0x" with nothing after it: the prefix is only consumed when a
        // character follows, and `x` is not a hex digit, so this is 0.
        assert_eq!(parse_int_radix16("0x"), 0.0);
    }

    #[test]
    fn radix16_matches_number_prototype_tostring() {
        assert_eq!(to_string_radix16(297.0), "129");
        assert_eq!(to_string_radix16(0.0), "0");
        assert_eq!(to_string_radix16(-16.0), "-10");
        assert_eq!(to_string_radix16(f64::NAN), "NaN");
    }

    #[test]
    fn whitespace_follows_javascript_not_rust() {
        assert!(is_js_whitespace('\u{feff}'), "U+FEFF is \\s in JS");
        assert!(!is_js_whitespace('\u{85}'), "U+0085 is not \\s in JS");
        assert!('\u{85}'.is_whitespace(), "...but Rust disagrees, which is the point");
        assert_eq!(split_whitespace(" 0001\u{feff}0002 "), vec!["0001", "0002"]);
        assert_eq!(js_trim("\u{feff} a \u{feff}"), "a");
    }

    #[test]
    fn pad_start_pads_in_front_of_a_minus_sign() {
        assert_eq!(pad_start_zero("2", 3), "002");
        assert_eq!(pad_start_zero("-2", 3), "0-2");
        assert_eq!(pad_start_zero("1234", 3), "1234");
    }
}
