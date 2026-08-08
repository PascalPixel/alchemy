//! JavaScript semantics `candidate_show.ts` depends on.
//!
//! Every numeric parse in this crate routes through [`js_parse_int_radix`].
//! There are exactly two call sites (the candidate stem, and the two `nm`
//! fields in [`crate::extent`]) and they must not each grow their own
//! implementation: a sibling port lost 526 bytes and then 980 more because a
//! correct helper existed and one call site did not use it.

/// `Number.parseInt(text, radix)` for radix 10 and 16.
///
/// PORT NOTE -- this is NOT `i64::from_str_radix`.
///
/// * `parseInt` skips leading JavaScript whitespace, accepts a leading `+`/`-`,
///   and for radix 16 accepts an optional `0x`/`0X` prefix. `from_str_radix`
///   REJECTS `0x` outright.
/// * `parseInt` consumes the longest valid prefix and ignores the rest:
///   `parseInt("8x", 10)` is `8`, `parseInt("1.5")` is `1`,
///   `parseInt("080000c0.txt", 16)` is `0x080000c0`. `from_str_radix` errors on
///   all three.
/// * An empty digit run is `NaN`, not an error.
///
/// The result is `f64` because that is the only numeric type JavaScript has and
/// callers compare it with `Number.isSafeInteger`, which a `u64` port would
/// silently make vacuous.
pub fn js_parse_int_radix(text: &str, radix: u32) -> f64 {
    debug_assert!(radix == 10 || radix == 16);
    let bytes = text.as_bytes();
    let mut i = 0usize;
    // `StringToNumber` leading whitespace: JavaScript's `\s` plus the line
    // terminators. `char::is_whitespace` is close but disagrees at U+0085
    // (whitespace in Rust, not in JavaScript) and U+200B, so the set is
    // written out.
    while i < bytes.len() {
        let rest = &text[i..];
        let c = match rest.chars().next() {
            Some(c) => c,
            None => break,
        };
        if is_js_space(c) {
            i += c.len_utf8();
        } else {
            break;
        }
    }
    let mut sign = 1.0f64;
    if i < bytes.len() && (bytes[i] == b'+' || bytes[i] == b'-') {
        if bytes[i] == b'-' {
            sign = -1.0;
        }
        i += 1;
    }
    if radix == 16
        && i + 1 < bytes.len()
        && bytes[i] == b'0'
        && (bytes[i + 1] == b'x' || bytes[i + 1] == b'X')
    {
        i += 2;
    }
    let start = i;
    let mut acc: u128 = 0;
    let mut overflowed = false;
    let mut acc_f = 0.0f64;
    while i < bytes.len() {
        let digit = match digit_value(bytes[i]) {
            Some(d) if u32::from(d) < radix => d,
            _ => break,
        };
        if overflowed {
            acc_f = acc_f * f64::from(radix) + f64::from(digit);
        } else {
            match acc
                .checked_mul(u128::from(radix))
                .and_then(|v| v.checked_add(u128::from(digit)))
            {
                Some(next) => acc = next,
                None => {
                    overflowed = true;
                    acc_f = acc as f64 * f64::from(radix) + f64::from(digit);
                }
            }
        }
        i += 1;
    }
    if i == start {
        return f64::NAN;
    }
    let magnitude = if overflowed { acc_f } else { acc as f64 };
    sign * magnitude
}

/// JavaScript's `\s` / `StringToNumber` whitespace set.
///
/// PORT NOTE -- it INCLUDES U+00A0, U+FEFF, U+2028, U+2029 and U+3000 and
/// EXCLUDES U+0085. Rust's `char::is_whitespace` (the `White_Space` property)
/// is the reverse on both ends.
pub fn is_js_space(c: char) -> bool {
    matches!(
        c,
        '\u{0009}'
            | '\u{000A}'
            | '\u{000B}'
            | '\u{000C}'
            | '\u{000D}'
            | '\u{0020}'
            | '\u{00A0}'
            | '\u{1680}'
            | '\u{2000}'
            ..='\u{200A}'
                | '\u{2028}'
                | '\u{2029}'
                | '\u{202F}'
                | '\u{205F}'
                | '\u{3000}'
                | '\u{FEFF}'
    )
}

/// `text.replace(/\s+$/, "")`.
///
/// PORT NOTE -- `str::trim_end` uses `White_Space`, which differs from
/// JavaScript's `\s` at U+0085 and U+FEFF. `is_js_space` is the authority.
pub fn trim_js_trailing_space(text: &str) -> &str {
    let mut end = text.len();
    while end > 0 {
        let c = text[..end].chars().next_back().expect("non-empty");
        if is_js_space(c) {
            end -= c.len_utf8();
        } else {
            break;
        }
    }
    &text[..end]
}

/// `text.padEnd(width)` -- pad with U+0020 to `width` UTF-16 code units.
///
/// PORT NOTE -- `padEnd` PADS BUT NEVER TRUNCATES, and `String.length` counts
/// UTF-16 code units, not bytes and not `char`s. `format!("{:<30}")` counts
/// `char`s, which differs for any astral character (2 code units, 1 `char`) and
/// for any non-ASCII byte sequence.
pub fn pad_end(text: &str, width: usize) -> String {
    let length = utf16_len(text);
    if length >= width {
        return text.to_string();
    }
    let mut out = text.to_string();
    out.extend(std::iter::repeat_n(' ', width - length));
    out
}

/// `text.padStart(width, "0")`.
///
/// PORT NOTE -- same never-truncates rule. A value wider than `width` widens
/// the column rather than wrapping.
pub fn pad_start_zero(text: &str, width: usize) -> String {
    let length = utf16_len(text);
    if length >= width {
        return text.to_string();
    }
    let mut out = String::new();
    out.extend(std::iter::repeat_n('0', width - length));
    out.push_str(text);
    out
}

/// `text.slice(0, end)` measured in UTF-16 code units.
///
/// PORT NOTE -- `slice` CLAMPS where `&text[..n]` panics, and it cuts on code
/// units, so it can split a surrogate pair. Splitting a pair yields a lone
/// surrogate, which is not representable in a Rust `String`; the replacement
/// character is substituted and the case is pinned by a test so a later reader
/// knows it was considered rather than missed. No objdump output reaches it.
pub fn slice_utf16(text: &str, end: usize) -> String {
    if utf16_len(text) <= end {
        return text.to_string();
    }
    let units: Vec<u16> = text.encode_utf16().take(end).collect();
    String::from_utf16_lossy(&units)
}

/// `String.prototype.length`.
pub fn utf16_len(text: &str) -> usize {
    text.chars().map(char::len_utf16).sum()
}

fn digit_value(b: u8) -> Option<u8> {
    match b {
        b'0'..=b'9' => Some(b - b'0'),
        b'a'..=b'z' => Some(b - b'a' + 10),
        b'A'..=b'Z' => Some(b - b'A' + 10),
        _ => None,
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn parse_int_accepts_the_hex_prefix_from_str_radix_rejects() {
        assert_eq!(js_parse_int_radix("0x10", 16), 16.0);
        assert!(i64::from_str_radix("0x10", 16).is_err());
    }

    #[test]
    fn parse_int_takes_the_longest_valid_prefix() {
        assert_eq!(js_parse_int_radix("8x", 10), 8.0);
        assert_eq!(js_parse_int_radix("1.5", 10), 1.0);
        assert_eq!(js_parse_int_radix("080000c0.txt", 16), 134_217_920.0);
        assert_eq!(js_parse_int_radix("  -ff", 16), -255.0);
    }

    #[test]
    fn parse_int_returns_nan_not_an_error() {
        assert!(js_parse_int_radix("", 16).is_nan());
        assert!(js_parse_int_radix("zz", 16).is_nan());
        assert!(js_parse_int_radix("0x", 16).is_nan());
    }

    #[test]
    fn js_space_set_differs_from_rust_white_space() {
        assert!(is_js_space('\u{FEFF}') && !'\u{FEFF}'.is_whitespace());
        assert!(!is_js_space('\u{0085}') && '\u{0085}'.is_whitespace());
        assert_eq!(trim_js_trailing_space("bx lr\u{FEFF}"), "bx lr");
        assert_eq!(trim_js_trailing_space("bx lr\u{0085}"), "bx lr\u{0085}");
    }

    #[test]
    fn padding_never_truncates_and_counts_utf16_units() {
        assert_eq!(pad_end("ab", 4), "ab  ");
        let long = "x".repeat(40);
        assert_eq!(pad_end(&long, 30), long);
        assert_eq!(pad_start_zero("1f", 4), "001f");
        assert_eq!(pad_start_zero("12345", 4), "12345");
        // U+1D400 is one `char` but two UTF-16 code units.
        assert_eq!(utf16_len("\u{1D400}"), 2);
        assert_eq!(pad_end("\u{1D400}", 4), "\u{1D400}  ");
    }

    #[test]
    fn slice_clamps_and_cuts_on_code_units() {
        assert_eq!(slice_utf16("abc", 30), "abc");
        assert_eq!(slice_utf16("abcdef", 3), "abc");
        // A cut through a surrogate pair; JavaScript yields a lone surrogate.
        assert_eq!(slice_utf16("\u{1D400}", 1), "\u{FFFD}");
    }
}
