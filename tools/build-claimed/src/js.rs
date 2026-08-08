//! JavaScript semantics that Rust does not share, extracted as named helpers.
//!
//! Every function here exists because the obvious Rust spelling is a different
//! function. They are named rather than inlined so clippy's suggestions (which
//! are about Rust, not about ECMAScript) cannot quietly rewrite them into
//! something that means something else, and so that a "simplification" has to
//! delete a test to land.

use serde_json::Value;

// ---------------------------------------------------------------------------
// Whitespace
// ---------------------------------------------------------------------------

/// The ECMAScript `\s` class, exactly.
///
/// It is NOT `char::is_whitespace` and it is NOT Rust's `White_Space`. It adds
/// U+FEFF (ZWNBSP), and it EXCLUDES U+0085 (NEL), which Rust's `White_Space`
/// contains. Both directions are load-bearing: `nm` output is ASCII today, but
/// a symbol name is attacker-shaped data as far as this tool is concerned, and
/// the two classes disagree on which bytes end a field.
pub fn is_js_space(ch: char) -> bool {
    matches!(
        ch,
        '\u{9}'      // TAB
            | '\u{a}'  // LF
            | '\u{b}'  // VT
            | '\u{c}'  // FF
            | '\u{d}'  // CR
            | '\u{20}' // SPACE
            | '\u{a0}' // NBSP
            | '\u{1680}'
            | '\u{2000}'..='\u{200a}'
            | '\u{2028}' // LINE SEPARATOR
            | '\u{2029}' // PARAGRAPH SEPARATOR
            | '\u{202f}'
            | '\u{205f}'
            | '\u{3000}'
            | '\u{feff}' // ZWNBSP
    )
}

/// `String.prototype.trim()`. Trims the ECMAScript whitespace set, so it is not
/// `str::trim`.
pub fn js_trim(text: &str) -> &str {
    text.trim_matches(is_js_space)
}

/// `text.trim().split(/\s+/)`.
///
/// After the trim there are no leading or trailing separators, so this never
/// yields the empty leading element that a bare `split` on an untrimmed string
/// would. An all-whitespace input trims to `""` and `"".split(/\s+/)` is
/// `[""]` -- one empty string, not zero elements. That case is reachable and is
/// pinned by a test.
pub fn js_trim_split_whitespace(text: &str) -> Vec<&str> {
    let trimmed = js_trim(text);
    if trimmed.is_empty() {
        return vec![""];
    }
    trimmed.split(is_js_space).filter(|part| !part.is_empty()).collect()
}

/// `text.split(/\r?\n/)`.
///
/// `.` is not involved, so this is line splitting proper: a lone `\r` is NOT a
/// separator here (it stays inside the field and is removed later by `trim`),
/// and a trailing newline produces a final empty element, which every caller
/// then drops with `.filter(Boolean)`.
pub fn js_split_lines(text: &str) -> Vec<&str> {
    let mut out = Vec::new();
    let bytes = text.as_bytes();
    let mut start = 0usize;
    let mut index = 0usize;
    while index < bytes.len() {
        if bytes[index] == b'\n' {
            let end = if index > start && bytes[index - 1] == b'\r' {
                index - 1
            } else {
                index
            };
            out.push(&text[start..end]);
            start = index + 1;
        }
        index += 1;
    }
    out.push(&text[start..]);
    out
}

// ---------------------------------------------------------------------------
// Numbers
// ---------------------------------------------------------------------------

/// `Number.parseInt(text, radix)` for radix 10 and 16, returning `None` for
/// `NaN`.
///
/// THIS IS THE ONLY INTEGER PARSE PATH IN THE CRATE. `from_str_radix` differs
/// from it in three ways that have each cost bytes in this repository:
///
///   * `parseInt("0x1f", 16)` is 31; `from_str_radix("0x1f", 16)` is an error.
///   * `parseInt("3.9", 10)` is 3 and then PASSES `Number.isInteger`;
///     `from_str_radix("3.9", 10)` is an error.
///   * `parseInt("12abc", 10)` is 12; `from_str_radix` rejects the whole string.
///
/// Leading ECMAScript whitespace is skipped and a leading sign is honoured.
/// Parsing stops at the first character that is not a digit in the radix, and
/// the result is `NaN` only when no digit was consumed at all.
pub fn js_parse_int(text: &str, radix: u32) -> Option<f64> {
    debug_assert!(radix == 10 || radix == 16, "only the radices this tool uses");
    let mut chars = text.chars().peekable();
    while chars.peek().copied().is_some_and(is_js_space) {
        chars.next();
    }
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
    if radix == 16 {
        // `parseInt` strips an `0x`/`0X` prefix when the radix is 16. This is
        // the difference that `from_str_radix` does not have.
        let mut lookahead = chars.clone();
        if lookahead.next() == Some('0')
            && matches!(lookahead.peek(), Some('x') | Some('X'))
        {
            chars.next();
            chars.next();
        }
    }
    let mut seen = false;
    let mut value = 0f64;
    for ch in chars {
        let digit = match ch.to_digit(radix) {
            Some(digit) => digit,
            None => break,
        };
        seen = true;
        value = value * f64::from(radix) + f64::from(digit);
    }
    if !seen {
        return None;
    }
    Some(if negative { -value } else { value })
}

/// `Number.isInteger(value)`. `NaN` and the infinities are not integers.
pub fn js_is_integer(value: f64) -> bool {
    value.is_finite() && value.fract() == 0.0
}

/// `Math.max(...values)` over an already-materialised list.
///
/// `f64::max` is NOT this function: `f64::max(NaN, 1.0)` is `1.0`, but
/// `Math.max(NaN, 1)` is `NaN`. The propagation is the point -- a `NaN` extent
/// arriving from a malformed `nm` line has to poison the module end the same
/// way it does in the original, or the two mirrors disagree on a corrupt input.
/// An empty list is `-Infinity`, as in JavaScript.
pub fn js_max(values: &[f64]) -> f64 {
    let mut best = f64::NEG_INFINITY;
    for &value in values {
        if value.is_nan() {
            return f64::NAN;
        }
        if value > best {
            best = value;
        }
    }
    best
}

/// `Math.min(...values)`, with the same `NaN` propagation and an empty list of
/// `+Infinity`.
pub fn js_min(values: &[f64]) -> f64 {
    let mut best = f64::INFINITY;
    for &value in values {
        if value.is_nan() {
            return f64::NAN;
        }
        if value < best {
            best = value;
        }
    }
    best
}

/// `left - right` used as a numeric sort comparator.
///
/// A comparator that returns `NaN` is treated by the specification as 0, i.e.
/// "equal", so the sort SILENTLY leaves the pair in its existing relative
/// order rather than throwing. `partial_cmp` returns `None` there and
/// `total_cmp` invents an ordering in which `NaN` is larger than everything;
/// both model this wrongly, so neither is used.
pub fn js_numeric_compare(left: f64, right: f64) -> std::cmp::Ordering {
    let difference = left - right;
    if difference.is_nan() || difference == 0.0 {
        return std::cmp::Ordering::Equal;
    }
    if difference < 0.0 {
        std::cmp::Ordering::Less
    } else {
        std::cmp::Ordering::Greater
    }
}

/// `value.toString(16).padStart(8, "0")` for a non-negative integral `f64`.
///
/// `padStart` pads and never truncates, so a value wider than eight hex digits
/// widens the field instead of wrapping; `{:08x}` agrees. `NaN` stringifies as
/// `"NaN"` and is then padded to `"00000NaN"`, which is genuinely what the
/// original prints in its failure message, so that path is reproduced rather
/// than hidden behind a Rust integer cast.
pub fn js_hex_pad8(value: f64) -> String {
    let body = if value.is_nan() {
        "NaN".to_string()
    } else if value.is_infinite() {
        if value > 0.0 { "Infinity".into() } else { "-Infinity".into() }
    } else if value < 0.0 {
        format!("-{:x}", (-value) as u64)
    } else {
        format!("{:x}", value as u64)
    };
    if body.len() >= 8 {
        body
    } else {
        format!("{}{}", "0".repeat(8 - body.len()), body)
    }
}

/// A number on its way into JSON.
///
/// `JSON.stringify` writes `null` for `NaN` and both infinities. Integral
/// values inside the safe range are emitted as integers, which is what every
/// value this tool produces actually is (addresses, sizes and byte counts):
/// `serde_json`'s float path would write `1.0` where JavaScript writes `1`.
///
/// PORT NOTE -- the non-integral fallback uses Rust's shortest-roundtrip
/// formatting, which diverges from `Number.prototype.toString` above 1e21 and
/// below 1e-6. No value here can reach either bound: every number in the
/// manifest is a difference or sum of ROM addresses. A test walks the real
/// corpus and asserts that no non-integral value is ever emitted, so the
/// divergence stays unreachable rather than merely unlikely.
pub fn js_number(value: f64) -> Value {
    if !value.is_finite() {
        return Value::Null;
    }
    if js_is_integer(value) && value.abs() <= 9_007_199_254_740_991.0 {
        return Value::from(value as i64);
    }
    Value::from(value)
}

// ---------------------------------------------------------------------------
// Strings and arrays
// ---------------------------------------------------------------------------

/// `Array.prototype.sort()` with no comparator: UTF-16 code-unit order over the
/// default string conversion.
///
/// This is a THIRD order, distinct from both `str::cmp` (UTF-8 byte order) and
/// `localeCompare` (ICU). It agrees with `str::cmp` on ASCII and disagrees
/// above U+FFFF, where a surrogate pair sorts below U+E000. The sort is stable,
/// so `sort_by` is used and `sort_unstable_by` is not.
pub fn js_default_sort(items: &mut [String]) {
    items.sort_by(|left, right| js_string_compare(left, right));
}

/// The comparison underneath [`js_default_sort`].
pub fn js_string_compare(left: &str, right: &str) -> std::cmp::Ordering {
    let mut left_units = left.encode_utf16();
    let mut right_units = right.encode_utf16();
    loop {
        match (left_units.next(), right_units.next()) {
            (None, None) => return std::cmp::Ordering::Equal,
            (None, Some(_)) => return std::cmp::Ordering::Less,
            (Some(_), None) => return std::cmp::Ordering::Greater,
            (Some(a), Some(b)) => {
                if a != b {
                    return a.cmp(&b);
                }
            }
        }
    }
}

/// `TypedArray.prototype.subarray(start, end)` over a byte buffer.
///
/// Rust's `&buffer[start..end]` PANICS where this clamps. Both arguments go
/// through ToIntegerOrInfinity first, so `NaN` becomes 0, a negative index
/// counts from the end, and an end before the start yields an empty slice.
pub fn js_subarray(buffer: &[u8], start: f64, end: f64) -> &[u8] {
    let length = buffer.len() as f64;
    let resolve = |raw: f64| -> usize {
        let integral = if raw.is_nan() {
            0.0
        } else if raw.is_infinite() {
            if raw > 0.0 { length } else { -length - 1.0 }
        } else {
            raw.trunc()
        };
        let positioned = if integral < 0.0 { length + integral } else { integral };
        positioned.clamp(0.0, length) as usize
    };
    let from = resolve(start);
    let to = resolve(end);
    if to <= from {
        return &buffer[0..0];
    }
    &buffer[from..to]
}

/// `left || right` where both sides are strings: the empty string is falsy.
///
/// `str::is_empty` spelled out, so the intent survives a reader who assumes
/// `unwrap_or`.
pub fn js_or_string<'a>(left: &'a str, right: &'a str) -> &'a str {
    if left.is_empty() {
        right
    } else {
        left
    }
}

/// `/^Func_[0-9a-f]{8}$/` -- WITHOUT the `i` flag.
///
/// Written out because the missing `i` is the whole content of the check: an
/// uppercase `FUNC_` or an uppercase hex digit must FAIL. `[0-9a-f]` is also
/// ASCII-only, which a Rust `is_ascii_hexdigit` would widen to include
/// uppercase.
pub fn is_lowercase_func_symbol(name: &str) -> bool {
    let Some(rest) = name.strip_prefix("Func_") else {
        return false;
    };
    rest.len() == 8
        && rest
            .bytes()
            .all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(&byte))
}

/// `/^[0-9a-f]{8}\.c$/i` -- WITH the `i` flag.
///
/// The sibling pattern above has no `i`; this one does, and the pair is the
/// reason each pattern's flags are checked individually rather than assumed to
/// match. An `0801A0B4.C` file IS claimed by this tool.
pub fn is_main_image_source_name(name: &str) -> bool {
    let bytes = name.as_bytes();
    if bytes.len() != 10 {
        return false;
    }
    if !bytes[..8].iter().all(u8::is_ascii_hexdigit) {
        return false;
    }
    bytes[8] == b'.' && (bytes[9] == b'c' || bytes[9] == b'C')
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn js_whitespace_is_not_rust_whitespace() {
        assert!(is_js_space('\u{feff}'), "ZWNBSP is JS whitespace");
        assert!(!'\u{feff}'.is_whitespace(), "and is not Rust whitespace");
        assert!(!is_js_space('\u{85}'), "NEL is not JS whitespace");
        assert!('\u{85}'.is_whitespace(), "but is Rust whitespace");
        assert!(is_js_space('\u{a0}') && is_js_space('\u{2028}') && is_js_space('\u{3000}'));
    }

    #[test]
    fn trim_split_matches_the_regex_form() {
        assert_eq!(js_trim_split_whitespace("  a   b\tc "), vec!["a", "b", "c"]);
        assert_eq!(js_trim_split_whitespace("only"), vec!["only"]);
        // `"".split(/\s+/)` is `[""]`, not `[]`.
        assert_eq!(js_trim_split_whitespace("   "), vec![""]);
        assert_eq!(js_trim_split_whitespace(""), vec![""]);
    }

    #[test]
    fn line_split_keeps_a_lone_carriage_return() {
        assert_eq!(js_split_lines("a\r\nb\nc\n"), vec!["a", "b", "c", ""]);
        assert_eq!(js_split_lines("a\rb"), vec!["a\rb"]);
        assert_eq!(js_split_lines(""), vec![""]);
    }

    #[test]
    fn parse_int_accepts_what_from_str_radix_rejects() {
        assert_eq!(js_parse_int("0x1f", 16), Some(31.0));
        assert!(u32::from_str_radix("0x1f", 16).is_err());
        assert_eq!(js_parse_int("1f", 16), Some(31.0));
        assert_eq!(js_parse_int("3.9", 10), Some(3.0));
        assert!(js_is_integer(js_parse_int("3.9", 10).unwrap()));
        assert_eq!(js_parse_int("12abc", 10), Some(12.0));
        assert_eq!(js_parse_int("  8", 10), Some(8.0));
        assert_eq!(js_parse_int("-4", 10), Some(-4.0));
        assert_eq!(js_parse_int("abc", 10), None);
        assert_eq!(js_parse_int("", 16), None);
        assert_eq!(js_parse_int("0801c0c8", 16), Some(134_332_616.0));
        // Uppercase hex parses; the `i`-less symbol regex still rejects it.
        assert_eq!(js_parse_int("0801C0C8", 16), Some(134_332_616.0));
    }

    #[test]
    fn max_propagates_nan_where_f64_max_swallows_it() {
        assert!(js_max(&[1.0, f64::NAN, 3.0]).is_nan());
        assert_eq!(f64::NAN.max(3.0), 3.0, "the trap this helper exists for");
        assert_eq!(js_max(&[1.0, 5.0, 3.0]), 5.0);
        assert_eq!(js_max(&[]), f64::NEG_INFINITY);
        assert!(js_min(&[1.0, f64::NAN]).is_nan());
        assert_eq!(js_min(&[4.0, 2.0]), 2.0);
    }

    #[test]
    fn nan_comparator_is_treated_as_equal_and_falls_through() {
        assert_eq!(js_numeric_compare(f64::NAN, 1.0), std::cmp::Ordering::Equal);
        // Stability plus "equal" means the input order survives.
        let mut names = vec!["b".to_string(), "a".to_string()];
        names.sort_by(|_, _| js_numeric_compare(f64::NAN, f64::NAN));
        assert_eq!(names, vec!["b".to_string(), "a".to_string()]);
    }

    #[test]
    fn hex_pad_widens_but_never_truncates() {
        assert_eq!(js_hex_pad8(0x0801_c0c8u32 as f64), "0801c0c8");
        assert_eq!(js_hex_pad8(0.0), "00000000");
        assert_eq!(js_hex_pad8(0x1_0000_0000u64 as f64), "100000000");
        assert_eq!(js_hex_pad8(f64::NAN), "00000NaN");
    }

    #[test]
    fn json_numbers_match_javascript() {
        assert_eq!(js_number(1.0).to_string(), "1");
        assert_eq!(js_number(0.0).to_string(), "0");
        assert_eq!(js_number(f64::NAN), Value::Null);
        assert_eq!(js_number(f64::INFINITY), Value::Null);
        assert_eq!(js_number(134_397_640.0).to_string(), "134397640");
    }

    #[test]
    fn default_sort_is_code_unit_order_and_stable() {
        let mut items = vec!["b".into(), "A".into(), "a".into(), "B".into()];
        js_default_sort(&mut items);
        assert_eq!(items, vec!["A", "B", "a", "b"]);
        // Above the BMP the code-unit order and the code-point order disagree.
        assert_eq!(
            js_string_compare("\u{10000}", "\u{e000}"),
            std::cmp::Ordering::Less
        );
        assert_eq!("\u{10000}".cmp("\u{e000}"), std::cmp::Ordering::Greater);
    }

    #[test]
    fn subarray_clamps_where_indexing_panics() {
        let buffer = [1u8, 2, 3, 4];
        assert_eq!(js_subarray(&buffer, 2.0, 99.0), &[3, 4]);
        assert_eq!(js_subarray(&buffer, 9.0, 12.0), &[] as &[u8]);
        assert_eq!(js_subarray(&buffer, 3.0, 1.0), &[] as &[u8]);
        assert_eq!(js_subarray(&buffer, f64::NAN, f64::NAN), &[] as &[u8]);
        assert_eq!(js_subarray(&buffer, -2.0, 4.0), &[3, 4]);
    }

    #[test]
    fn func_symbol_pattern_has_no_ignore_case_flag() {
        assert!(is_lowercase_func_symbol("Func_0801c0c8"));
        assert!(!is_lowercase_func_symbol("Func_0801C0C8"));
        assert!(!is_lowercase_func_symbol("FUNC_0801c0c8"));
        assert!(!is_lowercase_func_symbol("Func_0801c0c"));
        assert!(!is_lowercase_func_symbol("Data_0801c0c8"));
    }

    #[test]
    fn source_name_pattern_does_have_the_ignore_case_flag() {
        assert!(is_main_image_source_name("0801c0c8.c"));
        assert!(is_main_image_source_name("0801C0C8.C"));
        assert!(!is_main_image_source_name("resource_39c_c_0200a000.c"));
        assert!(!is_main_image_source_name("0801c0c8.s"));
        assert!(!is_main_image_source_name("0801c0c.c"));
    }

    #[test]
    fn empty_string_is_falsy() {
        assert_eq!(js_or_string("", "fallback"), "fallback");
        assert_eq!(js_or_string("value", "fallback"), "value");
    }
}
