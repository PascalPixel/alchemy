// JavaScript number and string semantics the queue port depends on.
//
// WHY THIS FILE EXISTS
// `decomp_queue.ts` reads its numeric flags with `Number(...)`, prints its
// ranking with `toFixed(1)`, and stamps the report with `toISOString()`. Each
// of those has a specification that Rust's nearest equivalent does not share:
// `Number("0x10")` is 16 while `f64::from_str` rejects it, `Number("inf")` is
// NaN while `f64::from_str` returns infinity, and `toFixed` breaks ties toward
// larger n where Rust's `{:.1}` breaks them toward even. Getting these wrong
// would not crash anything — it would just quietly print different rankings and
// accept flags the TypeScript rejects.

use decomp_diagnose::jsstr::js_trim;

/// `Number(string)` per the StringNumericLiteral grammar.
///
/// PORT NOTE: `f64::from_str` disagrees in both directions — it rejects `0x10`,
/// `0b101` and `.5e1`'s cousins that JS accepts, and accepts `inf`, `nan`,
/// `1_0` and `1e5f` that JS turns into NaN. Everything is validated here before
/// the final `parse` so only JS-legal spellings reach it.
pub fn js_number(text: &str) -> f64 {
    let trimmed = js_trim(text);
    if trimmed.is_empty() {
        return 0.0;
    }
    for (prefix, radix) in [("0x", 16u32), ("0X", 16), ("0o", 8), ("0O", 8), ("0b", 2), ("0B", 2)] {
        if let Some(digits) = trimmed.strip_prefix(prefix) {
            return radix_value(digits, radix);
        }
    }
    let (sign, body) = match trimmed.strip_prefix('-') {
        Some(rest) => (-1.0f64, rest),
        None => (1.0f64, trimmed.strip_prefix('+').unwrap_or(&trimmed)),
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

/// Digits-only radix literal; empty or malformed is NaN, as in JS.
fn radix_value(digits: &str, radix: u32) -> f64 {
    if digits.is_empty() || !digits.chars().all(|item| item.is_digit(radix)) {
        return f64::NAN;
    }
    // PORT NOTE: JS computes the exact mathematical value and then rounds once.
    // u128 covers every literal that fits 128 bits exactly; beyond that the
    // running accumulation rounds per digit. No flag this tool parses comes
    // anywhere near that, and the fallback still lands on infinity for
    // genuinely huge literals.
    let mut exact: Option<u128> = Some(0);
    let mut approximate = 0.0f64;
    for character in digits.chars() {
        let digit = character.to_digit(radix).expect("checked above") as u128;
        exact = exact.and_then(|value| value.checked_mul(radix as u128)?.checked_add(digit));
        approximate = approximate * f64::from(radix) + digit as f64;
    }
    match exact {
        Some(value) => value as f64,
        None => approximate,
    }
}

/// `StrUnsignedDecimalLiteral` without the `Infinity` alternative.
fn is_decimal_literal(body: &str) -> bool {
    let chars: Vec<char> = body.chars().collect();
    let mut cursor = 0usize;
    let integer = take_digits(&chars, &mut cursor);
    let mut fraction = 0usize;
    if chars.get(cursor) == Some(&'.') {
        cursor += 1;
        fraction = take_digits(&chars, &mut cursor);
    }
    if integer == 0 && fraction == 0 {
        return false;
    }
    if matches!(chars.get(cursor), Some('e') | Some('E')) {
        cursor += 1;
        if matches!(chars.get(cursor), Some('+') | Some('-')) {
            cursor += 1;
        }
        if take_digits(&chars, &mut cursor) == 0 {
            return false;
        }
    }
    cursor == chars.len()
}

fn take_digits(chars: &[char], cursor: &mut usize) -> usize {
    let start = *cursor;
    while chars.get(*cursor).is_some_and(char::is_ascii_digit) {
        *cursor += 1;
    }
    *cursor - start
}

/// `Number.isInteger(value)`.
pub fn is_integer(value: f64) -> bool {
    value.is_finite() && value.trunc() == value
}

/// `ToInt32(value)` — the coercion `&` applies to its operands.
pub fn to_int32(value: f64) -> i32 {
    if !value.is_finite() {
        return 0;
    }
    let truncated = value.trunc();
    let wrapped = truncated.rem_euclid(4294967296.0);
    if wrapped >= 2147483648.0 {
        (wrapped - 4294967296.0) as i32
    } else {
        wrapped as i32
    }
}

/// `Number#toFixed(1)`.
///
/// PORT NOTE: the tie rule is "pick the larger n" applied to the magnitude
/// after the sign is split off, i.e. half away from zero — `(0.25).toFixed(1)`
/// is `"0.3"` and `(-0.25).toFixed(1)` is `"-0.3"`. Rust's `{:.1}` rounds
/// half-to-even and would print `"0.2"` for the first. Formatting to 1100
/// places first is exact for every finite double, so the tie test below sees
/// the real value rather than a pre-rounded one.
pub fn to_fixed_1(value: f64) -> String {
    if value.is_nan() {
        return "NaN".to_string();
    }
    if value.is_infinite() {
        return if value > 0.0 { "Infinity" } else { "-Infinity" }.to_string();
    }
    if value.abs() >= 1e21 {
        return number_to_string(value);
    }
    let negative = value < 0.0;
    let magnitude = value.abs();
    let exact = format!("{magnitude:.1100}");
    let (integer, fraction) = exact.split_once('.').expect("1100 places always emits a point");
    let kept: u8 = fraction.as_bytes()[0] - b'0';
    let tail = &fraction[1..];
    let round_up = match tail.as_bytes()[0] {
        digit if digit > b'5' => true,
        b'5' => true, // Either strictly above the tie, or the tie itself; both round up.
        _ => false,
    };
    let mut digits: Vec<u8> = integer.bytes().chain(std::iter::once(b'0' + kept)).collect();
    if round_up {
        let mut index = digits.len();
        loop {
            if index == 0 {
                digits.insert(0, b'1');
                break;
            }
            index -= 1;
            if digits[index] == b'9' {
                digits[index] = b'0';
            } else {
                digits[index] += 1;
                break;
            }
        }
    }
    let text = String::from_utf8(digits).expect("decimal digits are ASCII");
    let (whole, last) = text.split_at(text.len() - 1);
    let whole = whole.trim_start_matches('0');
    let whole = if whole.is_empty() { "0" } else { whole };
    format!("{}{whole}.{last}", if negative { "-" } else { "" })
}

/// `String(number)`.
///
/// PORT NOTE: at magnitudes of 1e21 and above JS switches to exponential form
/// spelled `1e+21`, where Rust's `{}` prints all 22 digits and its `{:e}` omits
/// the `+`. Below that threshold this defers to the shared `JSON.stringify`
/// number formatter, which is already pinned against JS in decomp-diagnose.
pub fn number_to_string(value: f64) -> String {
    if value.is_nan() {
        return "NaN".to_string();
    }
    if value.is_infinite() {
        return if value > 0.0 { "Infinity" } else { "-Infinity" }.to_string();
    }
    // PORT NOTE: JS leaves fixed notation at 1e21 and above and below 1e-6.
    // `decomp_diagnose::json::number` covers only the upper switch, because the
    // fractions it formats never get that small (they are counts over an
    // instruction total, so the smallest non-zero value is around 1e-3). This
    // handles both sides so the console line is right for any input.
    if value != 0.0 && (value.abs() >= 1e21 || value.abs() < 1e-6) {
        let exponential = format!("{value:e}");
        let (mantissa, exponent) = exponential
            .split_once('e')
            .expect("LowerExp always emits an exponent");
        return if exponent.starts_with('-') {
            format!("{mantissa}e{exponent}")
        } else {
            format!("{mantissa}e+{exponent}")
        };
    }
    decomp_diagnose::json::number(value)
}

/// `new Date(millis).toISOString()`.
pub fn iso_timestamp(millis: i64) -> String {
    let (days, remainder) = (millis.div_euclid(86_400_000), millis.rem_euclid(86_400_000));
    let (year, month, day) = civil_from_days(days);
    let second = remainder / 1000;
    format!(
        "{year:04}-{month:02}-{day:02}T{:02}:{:02}:{:02}.{:03}Z",
        second / 3600,
        (second / 60) % 60,
        second % 60,
        remainder % 1000
    )
}

/// Howard Hinnant's `civil_from_days`, for days since 1970-01-01.
fn civil_from_days(days: i64) -> (i64, i64, i64) {
    let shifted = days + 719_468;
    let era = if shifted >= 0 { shifted } else { shifted - 146_096 } / 146_097;
    let day_of_era = shifted - era * 146_097;
    let year_of_era =
        (day_of_era - day_of_era / 1460 + day_of_era / 36524 - day_of_era / 146_096) / 365;
    let year = year_of_era + era * 400;
    let day_of_year = day_of_era - (365 * year_of_era + year_of_era / 4 - year_of_era / 100);
    let shifted_month = (5 * day_of_year + 2) / 153;
    let day = day_of_year - (153 * shifted_month + 2) / 5 + 1;
    let month = if shifted_month < 10 {
        shifted_month + 3
    } else {
        shifted_month - 9
    };
    (if month <= 2 { year + 1 } else { year }, month, day)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn number_follows_the_string_numeric_literal_grammar() {
        assert_eq!(js_number("0x10"), 16.0);
        assert_eq!(js_number("0X1f"), 31.0);
        assert_eq!(js_number("0b101"), 5.0);
        assert_eq!(js_number("0o17"), 15.0);
        assert_eq!(js_number(""), 0.0);
        assert_eq!(js_number("   "), 0.0);
        assert_eq!(js_number(" 12 "), 12.0);
        assert_eq!(js_number(".5"), 0.5);
        assert_eq!(js_number("5."), 5.0);
        assert_eq!(js_number("+3e2"), 300.0);
        assert_eq!(js_number("Infinity"), f64::INFINITY);
        assert_eq!(js_number("-Infinity"), f64::NEG_INFINITY);
        // Everything Rust's parser would have accepted but JS does not.
        for rejected in ["inf", "nan", "NaN", "1_0", "1e", "0x", "infinity", "1e5f", "12abc", "--1"] {
            assert!(js_number(rejected).is_nan(), "{rejected} should be NaN");
        }
    }

    #[test]
    fn integer_and_int32_coercions_match_javascript() {
        assert!(is_integer(1.0));
        assert!(!is_integer(1.5));
        assert!(!is_integer(f64::NAN));
        assert!(!is_integer(f64::INFINITY));
        assert_eq!(to_int32(0x0800_0001u32 as f64), 0x0800_0001u32 as i32);
        assert_eq!(to_int32(4294967297.0), 1);
        assert_eq!(to_int32(-1.0), -1);
        assert_eq!(to_int32(f64::NAN), 0);
    }

    /// Expectations captured from Bun 1.3.14.
    #[test]
    fn to_fixed_matches_the_spec_tie_rule() {
        let cases: &[(f64, &str)] = &[
            (0.25, "0.3"),
            (-0.25, "-0.3"),
            (0.35, "0.3"),
            (-0.35, "-0.3"),
            (277.142_857_142_857_1, "277.1"),
            (-0.04, "-0.0"),
            (0.05, "0.1"),
            (1.05, "1.1"),
            (2.675, "2.7"),
            (0.0, "0.0"),
            (-0.0, "0.0"),
            (1234.05, "1234.0"),
            (8.45, "8.4"),
            (9.99, "10.0"),
            (1e21, "1e+21"),
        ];
        for (value, expected) in cases {
            assert_eq!(&to_fixed_1(*value), expected, "toFixed(1) of {value}");
        }
        assert_eq!(to_fixed_1(f64::NAN), "NaN");
        assert_eq!(to_fixed_1(f64::INFINITY), "Infinity");
        assert_eq!(number_to_string(1e21), "1e+21");
        assert_eq!(number_to_string(-1e21), "-1e+21");
        assert_eq!(number_to_string(1.5e21), "1.5e+21");
        assert_eq!(number_to_string(1e-7), "1e-7");
        assert_eq!(number_to_string(41.0), "41");
    }

    #[test]
    fn iso_timestamp_matches_date_to_iso_string() {
        // `new Date("2026-08-07T11:32:41.163Z").getTime()`
        assert_eq!(iso_timestamp(1_786_102_361_163), "2026-08-07T11:32:41.163Z");
        assert_eq!(iso_timestamp(0), "1970-01-01T00:00:00.000Z");
        // A leap day, to exercise the civil-date arithmetic.
        assert_eq!(iso_timestamp(1_709_164_800_000), "2024-02-29T00:00:00.000Z");
    }
}
