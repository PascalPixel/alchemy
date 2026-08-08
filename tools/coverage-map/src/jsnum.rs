// JavaScript number and string primitives the coverage map depends on for
// byte-identical output.
//
// WHY: this tool writes a tracked JSON document, four tracked SVGs and a
// tracked README line. Every number in those artifacts is formatted by a
// JavaScript rule -- `String(x)`, `toFixed`, `toLocaleString`, `Math.round` --
// and none of those rules is Rust's default. Getting any one of them wrong
// changes tracked bytes, so they live here with tests rather than being
// open-coded at each call site.

/// `Number.isSafeInteger(value)`.
pub fn is_safe_integer(value: f64) -> bool {
    value.is_finite() && value.fract() == 0.0 && value.abs() <= 9_007_199_254_740_991.0
}

/// ECMAScript `ToString(Number)` (spec 6.1.6.1.20).
///
/// PORT NOTE: this is *not* Rust's `Display`. JS writes an integral double as
/// `1` where Rust writes `1.0`, and JS switches to exponential notation below
/// 1e-6 where Rust never does. The implementation follows the one in
/// `tools/compare-roms/src/lib.rs`, which was written and tested against the
/// same spec clause; it is copied rather than shared because each crate here
/// builds standalone.
pub fn js_number_string(value: f64) -> String {
    if value.is_nan() {
        return "NaN".to_string();
    }
    if value == 0.0 {
        return "0".to_string();
    }
    if value < 0.0 {
        return format!("-{}", js_number_string(-value));
    }
    if value.is_infinite() {
        return "Infinity".to_string();
    }
    let exponential = format!("{value:e}");
    let (mantissa, exponent) = exponential
        .split_once('e')
        .expect("{:e} always emits an exponent");
    let digits: String = mantissa
        .chars()
        .filter(|character| *character != '.')
        .collect();
    let digits = digits.trim_end_matches('0');
    let digits = if digits.is_empty() { "0" } else { digits };
    let k = digits.len() as i32;
    let n = exponent
        .parse::<i32>()
        .expect("{:e} always emits an integer exponent")
        + 1;

    if k <= n && n <= 21 {
        return format!("{digits}{}", "0".repeat((n - k) as usize));
    }
    if 0 < n && n <= 21 {
        return format!("{}.{}", &digits[..n as usize], &digits[n as usize..]);
    }
    if -6 < n && n <= 0 {
        return format!("0.{}{digits}", "0".repeat((-n) as usize));
    }
    let tail = if n > 0 {
        format!("e+{}", n - 1)
    } else {
        format!("e-{}", 1 - n)
    };
    if k == 1 {
        return format!("{digits}{tail}");
    }
    format!("{}.{}{tail}", &digits[..1], &digits[1..])
}

/// `Math.round(value)`: half rounds toward `+Infinity`, not away from zero.
///
/// PORT NOTE: Rust's `f64::round` is half-away-from-zero, so `-0.5` rounds to
/// `-1` there and to `-0` in JavaScript. The map rounds signed quantities
/// (`precise`, tile geometry offsets) so the difference is reachable.
pub fn js_round(value: f64) -> f64 {
    if value.is_nan() {
        return f64::NAN;
    }
    if !value.is_finite() {
        return value;
    }
    (value + 0.5).floor()
}

/// `Number.prototype.toFixed(digits)`.
///
/// PORT NOTE: JS picks the integer `n` minimising `|n / 10^f - x|` and breaks a
/// tie by taking the *larger* `n` (half-up on the exact binary value). Rust's
/// `{:.*}` formatting breaks the same tie to even. The map's values (`x.y`
/// percentages produced by `Math.round(...) / 10`, `bytes / 1048576`, and
/// `0.93 - 0.38 * fraction`) cannot land on an exact tie, but relying on that
/// silently would be fragile, so the tie is handled explicitly: format the
/// exact value to a generous 30 decimals (Rust's formatter is correctly
/// rounded, and an exact tie terminates well before that) and then round the
/// decimal string half-up.
pub fn to_fixed(value: f64, digits: usize) -> String {
    if value.is_nan() {
        return "NaN".to_string();
    }
    if !value.is_finite() {
        return if value > 0.0 {
            "Infinity".to_string()
        } else {
            "-Infinity".to_string()
        };
    }
    let negative = value < 0.0;
    let magnitude = value.abs();
    let wide = format!("{:.*}", 30usize.max(digits + 1), magnitude);
    let (integral, fraction) = wide.split_once('.').expect("precision is always non-zero");
    let mut integral: Vec<u8> = integral.bytes().collect();
    let fraction: Vec<u8> = fraction.bytes().collect();
    let mut kept: Vec<u8> = fraction[..digits].to_vec();
    // Half-up: round away from zero when the first dropped digit is >= 5.
    if fraction.get(digits).is_some_and(|digit| *digit >= b'5') {
        let mut index = kept.len();
        loop {
            if index == 0 {
                let mut carry = true;
                let mut position = integral.len();
                while carry && position > 0 {
                    position -= 1;
                    if integral[position] == b'9' {
                        integral[position] = b'0';
                    } else {
                        integral[position] += 1;
                        carry = false;
                    }
                }
                if carry {
                    integral.insert(0, b'1');
                }
                break;
            }
            index -= 1;
            if kept[index] == b'9' {
                kept[index] = b'0';
            } else {
                kept[index] += 1;
                break;
            }
        }
    }
    let integral = String::from_utf8(integral).expect("digits are ASCII");
    let kept = String::from_utf8(kept).expect("digits are ASCII");
    let body = if digits == 0 {
        integral
    } else {
        format!("{integral}.{kept}")
    };
    // `(-0.4).toFixed(0)` is `"-0"` in JavaScript, sign preserved.
    if negative {
        format!("-{body}")
    } else {
        body
    }
}

/// Thousands separators, the shape both `commas()` and
/// `Number.prototype.toLocaleString("en-US")` produce for an integer.
pub fn group_digits(digits: &str) -> String {
    let (sign, body) = match digits.strip_prefix('-') {
        Some(rest) => ("-", rest),
        None => ("", digits),
    };
    let mut out = String::new();
    let bytes = body.as_bytes();
    for (index, byte) in bytes.iter().enumerate() {
        if index > 0 && (bytes.len() - index) % 3 == 0 {
            out.push(',');
        }
        out.push(*byte as char);
    }
    format!("{sign}{out}")
}

/// `Math.round(value).toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",")`
pub fn commas(value: f64) -> String {
    group_digits(&js_number_string(js_round(value)))
}

/// `value.toString(16).padStart(8, "0")` -- the canonical address spelling.
///
/// PORT NOTE: `padStart` on a negative number yields `000000-2`, because it
/// pads the *string* including the sign. No caller passes a negative address,
/// but the formatting below reproduces the JavaScript rather than clamping,
/// so a future negative would surface as the same odd text instead of a panic.
pub fn hex8(value: i64) -> String {
    let text = if value < 0 {
        format!("-{:x}", value.unsigned_abs())
    } else {
        format!("{value:x}")
    };
    if text.len() >= 8 {
        text
    } else {
        format!("{}{}", "0".repeat(8 - text.len()), text)
    }
}

/// `Number.parseInt(text, 16)`.
///
/// PORT NOTE: radix 16 explicitly permits a `0x`/`0X` prefix, and `parseInt`
/// consumes the longest valid prefix rather than requiring the whole string.
/// Most call sites hand it bare digits captured by a regex, but
/// `asm/alignment.json` stores `"0x08002efa"`; a strict whole-string parse
/// silently yielded 0 for all 329 of those addresses, which dropped 329 region
/// boundaries and over-credited exact main coverage by 526 bytes.
pub fn parse_hex(text: &str) -> i64 {
    let body = text.trim_start_matches(is_js_space_char);
    let (negative, body) = match body.strip_prefix('-') {
        Some(rest) => (true, rest),
        None => (false, body.strip_prefix('+').unwrap_or(body)),
    };
    let body = body
        .strip_prefix("0x")
        .or_else(|| body.strip_prefix("0X"))
        .unwrap_or(body);
    let digits: String = body.chars().take_while(|c| c.is_ascii_hexdigit()).collect();
    if digits.is_empty() {
        return 0;
    }
    let magnitude = i64::from_str_radix(&digits, 16).unwrap_or(0);
    if negative {
        -magnitude
    } else {
        magnitude
    }
}

fn is_js_space_char(character: char) -> bool {
    crate::js::is_js_space(character)
}

/// `Number.parseInt(text, 0)`: radix 0 auto-detects a `0x`/`0X` prefix and
/// otherwise reads decimal. The `.space` reservation grammar admits both.
pub fn parse_int_auto(text: &str) -> Option<i64> {
    let trimmed = text.trim();
    let (body, radix) = match trimmed
        .strip_prefix("0x")
        .or_else(|| trimmed.strip_prefix("0X"))
    {
        Some(rest) => (rest, 16),
        None => (trimmed, 10),
    };
    // `parseInt` reads the longest valid prefix; the call sites here have
    // already matched a full-string regex, so a strict parse is equivalent.
    i64::from_str_radix(body, radix).ok()
}

/// String ordering by UTF-16 code unit, which is what `Array#sort` uses for
/// strings and what `readdirSync(...).sort()` therefore produces.
///
/// PORT NOTE: Rust's `str` ordering is by UTF-8 byte, which disagrees with
/// UTF-16 order once astral characters are involved: U+FFFD sorts *after*
/// U+10000 in UTF-16 code-unit order (0xFFFD vs the lead surrogate 0xD800) and
/// *before* it in UTF-8 byte order. Tracked paths here are ASCII, where the
/// two agree, but the
/// asset labels carry `·` (U+00B7) and are compared with the same helper, so
/// the correct comparator is used everywhere rather than assumed away.
pub fn utf16_cmp(left: &str, right: &str) -> std::cmp::Ordering {
    let mut a = left.encode_utf16();
    let mut b = right.encode_utf16();
    loop {
        match (a.next(), b.next()) {
            (None, None) => return std::cmp::Ordering::Equal,
            (None, Some(_)) => return std::cmp::Ordering::Less,
            (Some(_), None) => return std::cmp::Ordering::Greater,
            (Some(x), Some(y)) => {
                if x != y {
                    return x.cmp(&y);
                }
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn js_number_string_matches_ecmascript_tostring() {
        assert_eq!(js_number_string(1.0), "1");
        assert_eq!(js_number_string(0.0), "0");
        assert_eq!(js_number_string(-0.0), "0");
        assert_eq!(js_number_string(40.5), "40.5");
        assert_eq!(js_number_string(1e21), "1e+21");
        assert_eq!(js_number_string(1e-7), "1e-7");
        assert_eq!(js_number_string(0.000001), "0.000001");
        assert_eq!(js_number_string(-2.5), "-2.5");
        assert_eq!(js_number_string(8388608.0), "8388608");
    }

    #[test]
    fn js_round_is_half_toward_positive_infinity() {
        assert_eq!(js_round(0.5), 1.0);
        assert_eq!(js_round(1.5), 2.0);
        assert_eq!(js_round(-0.5), 0.0);
        assert_eq!(js_round(-1.5), -1.0);
        assert_eq!(js_round(2.4), 2.0);
        // Rust's own `round` disagrees on the negative half, which is the point.
        assert_ne!(js_round(-1.5), (-1.5f64).round());
    }

    #[test]
    fn to_fixed_rounds_half_up_not_half_to_even() {
        assert_eq!(to_fixed(0.25, 1), "0.3");
        assert_eq!(to_fixed(0.75, 1), "0.8");
        assert_eq!(to_fixed(2.5, 0), "3");
        assert_eq!(to_fixed(40.5, 1), "40.5");
        assert_eq!(to_fixed(0.93, 3), "0.930");
        assert_eq!(to_fixed(0.93 - 0.38, 3), "0.550");
        assert_eq!(to_fixed(8388608.0 / 1048576.0, 2), "8.00");
        assert_eq!(to_fixed(0.0, 1), "0.0");
        assert_eq!(to_fixed(99.99, 1), "100.0");
        assert_eq!(to_fixed(9.999, 2), "10.00");
        assert_eq!(to_fixed(-0.04, 1), "-0.0");
    }

    #[test]
    fn group_digits_and_commas() {
        assert_eq!(commas(0.0), "0");
        assert_eq!(commas(999.0), "999");
        assert_eq!(commas(1000.0), "1,000");
        assert_eq!(commas(1234567.0), "1,234,567");
        assert_eq!(commas(-1234.0), "-1,234");
        assert_eq!(commas(1234.4), "1,234");
        assert_eq!(commas(1234.5), "1,235");
        assert_eq!(group_digits("100"), "100");
    }

    #[test]
    fn parse_int_auto_detects_the_radix() {
        assert_eq!(parse_int_auto("0x20"), Some(32));
        assert_eq!(parse_int_auto("32"), Some(32));
        assert_eq!(parse_int_auto("0X10"), Some(16));
        assert_eq!(parse_hex("08000000"), 0x0800_0000);
    }

    #[test]
    fn utf16_cmp_orders_by_code_unit() {
        assert_eq!(utf16_cmp("a", "b"), std::cmp::Ordering::Less);
        assert_eq!(utf16_cmp("Z", "a"), std::cmp::Ordering::Less);
        assert_eq!(utf16_cmp("a", "a"), std::cmp::Ordering::Equal);
        assert_eq!(utf16_cmp("ab", "a"), std::cmp::Ordering::Greater);
        // U+FFFD (three UTF-8 bytes) sorts after a surrogate pair in UTF-8 byte
        // order but before it in UTF-16 code-unit order.
        assert_eq!(
            utf16_cmp("\u{fffd}", "\u{10000}"),
            std::cmp::Ordering::Greater
        );
        assert!("\u{fffd}" < "\u{10000}");
    }

    #[test]
    fn is_safe_integer_rejects_fractions_and_giants() {
        assert!(is_safe_integer(1.0));
        assert!(!is_safe_integer(1.5));
        assert!(!is_safe_integer(f64::NAN));
        assert!(!is_safe_integer(9_007_199_254_740_993.0));
    }
}
