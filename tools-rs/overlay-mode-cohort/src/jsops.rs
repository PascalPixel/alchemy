//! JavaScript primitives that Rust spells differently, gathered in one place.
//!
//! Every one of these exists because the obvious Rust equivalent has different
//! behaviour on some input `tools/overlay/overlay_mode_cohort.ts` can actually
//! see. Each is pinned by a test.

/// JavaScript's `StrWhiteSpace` / regex `\s` class.
///
/// PORT NOTE -- THIS IS NOT `char::is_whitespace`. JavaScript includes U+00A0,
/// U+FEFF, U+2028 and U+2029 and EXCLUDES U+0085; Rust's `White_Space` property
/// is the other way round on both counts. `arm-none-eabi-nm` output is ASCII in
/// practice, but the split and trim below run on compiler and linker text that
/// this tool does not control.
pub fn is_js_whitespace(character: char) -> bool {
    matches!(
        character,
        '\u{9}' | '\u{a}' | '\u{b}' | '\u{c}' | '\u{d}'
            | '\u{20}' | '\u{a0}' | '\u{feff}'
            | '\u{1680}' | '\u{2000}'..='\u{200a}'
            | '\u{2028}' | '\u{2029}' | '\u{202f}' | '\u{205f}' | '\u{3000}'
    )
}

/// `String#trim`.
pub fn js_trim(text: &str) -> &str {
    text.trim_matches(is_js_whitespace)
}

/// `text.split(/\s+/)`.
///
/// PORT NOTE: this keeps JavaScript's leading empty field. `" a".split(/\s+/)`
/// is `["", "a"]`, not `["a"]`, because the regex matches at offset 0 and the
/// text before it is an empty field. `str::split_whitespace` drops it. Every
/// call site here trims first, so the field cannot appear in practice; it is
/// reproduced anyway so a future caller that forgets to trim diverges loudly
/// rather than quietly.
pub fn js_split_whitespace(text: &str) -> Vec<&str> {
    let mut fields: Vec<&str> = Vec::new();
    let mut field_start = 0usize;
    let mut cursor = 0usize;
    let bytes = text.as_bytes();
    while cursor < text.len() {
        // `cursor` always sits on a character boundary: it is only ever advanced
        // past whole characters.
        let character = text[cursor..].chars().next().expect("character boundary");
        if is_js_whitespace(character) {
            fields.push(&text[field_start..cursor]);
            let mut run_end = cursor;
            while run_end < text.len() {
                let next = text[run_end..].chars().next().expect("character boundary");
                if !is_js_whitespace(next) {
                    break;
                }
                run_end += next.len_utf8();
            }
            cursor = run_end;
            field_start = run_end;
            continue;
        }
        cursor += character.len_utf8();
    }
    let _ = bytes;
    fields.push(&text[field_start..]);
    fields
}

/// `text.split(/\r?\n/)`.
pub fn js_split_lines(text: &str) -> Vec<&str> {
    let mut lines: Vec<&str> = Vec::new();
    let mut start = 0usize;
    let bytes = text.as_bytes();
    let mut cursor = 0usize;
    while cursor < bytes.len() {
        if bytes[cursor] == b'\n' {
            let end = if cursor > start && bytes[cursor - 1] == b'\r' {
                cursor - 1
            } else {
                cursor
            };
            lines.push(&text[start..end]);
            cursor += 1;
            start = cursor;
            continue;
        }
        cursor += 1;
    }
    lines.push(&text[start..]);
    lines
}

/// `Number.parseInt(text, radix)`.
///
/// PORT NOTE -- THIS IS NOT `i64::from_str_radix`. Four measured differences,
/// every one of which this tool can hit:
///
///  * `parseInt("0x1f", 16)` is 31; `from_str_radix` REJECTS the `0x` prefix.
///    `arm-none-eabi-nm -S` prints bare hex today, but the same helper parses
///    the `--pairs` / `--jobs` / `--top` decimal arguments and the address
///    captured out of a candidate filename. Routing every numeric parse through
///    one function is the whole point: a repository-local incident cost 980
///    bytes at a second call site that had a correct helper available and did
///    not use it.
///  * `parseInt("1.5")` is 1: digits simply stop at the `.`.
///  * `parseInt("12abc")` is 12, where `from_str_radix` errors.
///  * `parseInt("")`, `parseInt("abc")` and `parseInt("-")` are NaN, which is
///    neither an error nor a number -- hence the `f64` return, so the
///    `Number.isInteger` guards downstream see exactly what JavaScript sees.
pub fn js_parse_int(text: &str, radix: u32) -> f64 {
    let trimmed = js_trim(text);
    let mut chars = trimmed.chars().peekable();
    let mut negative = false;
    match chars.peek() {
        Some('+') => {
            chars.next();
        }
        Some('-') => {
            negative = true;
            chars.next();
        }
        _ => {}
    }
    let rest: String = chars.collect();
    let mut digits = rest.as_str();
    if radix == 16 && (digits.starts_with("0x") || digits.starts_with("0X")) {
        digits = &digits[2..];
    }
    let mut value = 0f64;
    let mut seen = 0usize;
    for character in digits.chars() {
        match character.to_digit(36) {
            Some(digit) if digit < radix => {
                value = value * f64::from(radix) + f64::from(digit);
                seen += 1;
            }
            _ => break,
        }
    }
    if seen == 0 {
        return f64::NAN;
    }
    if negative {
        -value
    } else {
        value
    }
}

/// `Number.isInteger(value)`.
pub fn js_is_integer(value: f64) -> bool {
    value.is_finite() && value.fract() == 0.0
}

/// `text.slice(start)` on UTF-16 code units.
///
/// PORT NOTE: `String#length` counts UTF-16 code units, not `char`s and not
/// bytes, and `String#slice` CLAMPS where Rust's `&text[start..]` panics. Both
/// matter: `candidate.source.slice(ROOT.length + 1)` runs on the error path
/// without the `isAbsolute` guard the success path has, so a relative source
/// shorter than the root produces `""` in JavaScript rather than a crash.
pub fn js_slice_from(text: &str, start: usize) -> String {
    let units: Vec<u16> = text.encode_utf16().collect();
    let start = start.min(units.len());
    String::from_utf16_lossy(&units[start..])
}

/// `text.slice(0, end)` on UTF-16 code units, clamping like JavaScript.
pub fn js_slice_to(text: &str, end: usize) -> String {
    let units: Vec<u16> = text.encode_utf16().collect();
    let end = end.min(units.len());
    String::from_utf16_lossy(&units[..end])
}

/// `text.length`.
pub fn js_length(text: &str) -> usize {
    text.encode_utf16().count()
}

/// `buffer.subarray(0, end)`.
///
/// PORT NOTE: `Buffer#subarray` CLAMPS an over-long end and treats a NaN end as
/// 0, where Rust's `&buffer[..end]` panics on both. `size` here comes straight
/// out of `parseInt` on linker output, so NaN is reachable from a malformed
/// `nm -S` row rather than hypothetical.
pub fn js_subarray_to(buffer: &[u8], end: f64) -> &[u8] {
    if end.is_nan() || end <= 0.0 {
        return &buffer[..0];
    }
    let end = if end >= buffer.len() as f64 {
        buffer.len()
    } else {
        end as usize
    };
    &buffer[..end]
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn parse_int_accepts_the_hex_prefix_from_str_radix_rejects() {
        assert_eq!(js_parse_int("0x1f", 16), 31.0);
        assert!(i64::from_str_radix("0x1f", 16).is_err());
        assert_eq!(js_parse_int("1f", 16), 31.0);
    }

    #[test]
    fn parse_int_truncates_and_yields_nan_rather_than_erroring() {
        assert_eq!(js_parse_int("1.5", 10), 1.0);
        assert_eq!(js_parse_int("12abc", 10), 12.0);
        assert_eq!(js_parse_int("  7  ", 10), 7.0);
        assert_eq!(js_parse_int("-3", 10), -3.0);
        assert!(js_parse_int("", 10).is_nan());
        assert!(js_parse_int("abc", 10).is_nan());
        assert!(js_parse_int("-", 10).is_nan());
    }

    #[test]
    fn is_integer_rejects_nan_and_infinity() {
        assert!(js_is_integer(4.0));
        assert!(!js_is_integer(f64::NAN));
        assert!(!js_is_integer(f64::INFINITY));
        assert!(!js_is_integer(1.5));
    }

    #[test]
    fn whitespace_class_is_javascripts_not_rusts() {
        // U+FEFF is whitespace in JavaScript and not in Rust.
        assert!(is_js_whitespace('\u{feff}'));
        // U+0085 is whitespace in Rust and not in JavaScript.
        assert!(!is_js_whitespace('\u{85}'));
        assert!('\u{85}'.is_whitespace());
        assert!(!'\u{feff}'.is_whitespace());
        // U+2028 and U+2029 are JavaScript whitespace and JavaScript line
        // terminators at once; `.` excludes them but `\s` includes them.
        assert!(is_js_whitespace('\u{2028}'));
    }

    #[test]
    fn split_whitespace_keeps_the_leading_empty_field() {
        assert_eq!(js_split_whitespace(" a b"), vec!["", "a", "b"]);
        assert_eq!(js_split_whitespace("a  b"), vec!["a", "b"]);
        assert_eq!(js_split_whitespace(""), vec![""]);
    }

    #[test]
    fn split_lines_handles_crlf_and_a_trailing_newline() {
        assert_eq!(js_split_lines("a\r\nb\n"), vec!["a", "b", ""]);
        assert_eq!(js_split_lines("a"), vec!["a"]);
    }

    #[test]
    fn slice_clamps_where_rust_indexing_panics() {
        assert_eq!(js_slice_from("abc", 10), "");
        assert_eq!(js_slice_to("abc", 10), "abc");
        assert_eq!(js_slice_from("abcdef", 2), "cdef");
    }

    #[test]
    fn length_counts_utf16_units() {
        assert_eq!(js_length("a\u{1f600}"), 3);
        assert_eq!("a\u{1f600}".chars().count(), 2);
    }

    #[test]
    fn subarray_clamps_and_treats_nan_as_zero() {
        let buffer = [1u8, 2, 3];
        assert_eq!(js_subarray_to(&buffer, 99.0), &buffer[..]);
        assert_eq!(js_subarray_to(&buffer, f64::NAN), &[] as &[u8]);
        assert_eq!(js_subarray_to(&buffer, 2.0), &[1u8, 2][..]);
    }
}
