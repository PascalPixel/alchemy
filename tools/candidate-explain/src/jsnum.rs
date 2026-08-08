//! JavaScript numeric and string primitives this port needs and that no
//! sibling crate already exports.
//!
//! Everything numeric that `candidate_explain.ts` parses goes through exactly
//! one function in this module. That is deliberate: the confirmed regression
//! pattern in this repository is a crate that owns a correct `parseInt` helper
//! and then has one call site reach for `i64::from_str_radix` instead.

/// `Number.parseInt(text, 16)`.
///
/// PORT NOTE -- `i64::from_str_radix(text, 16)` is NOT this function and must
/// never be substituted for it. Three documented divergences, each of which
/// has cost bytes elsewhere in this tree:
///
///   * `parseInt` accepts an optional `0x` / `0X` prefix at radix 16;
///     `from_str_radix` rejects it outright.
///   * `parseInt` is a *prefix* parse: it consumes the longest leading run of
///     valid digits and ignores the rest, so `parseInt("ffz", 16)` is `255`
///     and `parseInt("8x", 10)` is `8`. `from_str_radix` errors.
///   * `parseInt` with no valid leading digit is `NaN`, not an error.
///
/// Leading whitespace is JavaScript's `StrWhiteSpace`, which is why the trim
/// is spelled out rather than delegated to `str::trim`: Rust's `White_Space`
/// property includes U+0085 (which JavaScript excludes) and omits U+FEFF
/// (which JavaScript includes).
pub fn parse_int_16(text: &str) -> f64 {
    let mut chars = text.chars().peekable();
    while let Some(&c) = chars.peek() {
        if is_js_whitespace(c) {
            chars.next();
        } else {
            break;
        }
    }
    let rest: String = chars.collect();
    let (sign, body) = match rest.strip_prefix('-') {
        Some(body) => (-1.0f64, body),
        None => (1.0f64, rest.strip_prefix('+').unwrap_or(&rest)),
    };
    let body = body
        .strip_prefix("0x")
        .or_else(|| body.strip_prefix("0X"))
        .unwrap_or(body);
    let digits: String = body.chars().take_while(|c| c.is_ascii_hexdigit()).collect();
    if digits.is_empty() {
        return f64::NAN;
    }
    // Accumulate in f64 exactly as ECMAScript specifies (the value is the
    // mathematical value rounded once); the inputs here are at most eight hex
    // digits, so no rounding occurs in practice.
    let mut value = 0.0f64;
    for c in digits.chars() {
        value = value * 16.0 + f64::from(c.to_digit(16).expect("ascii hex digit"));
    }
    sign * value
}

/// JavaScript's `StrWhiteSpace` character set.
///
/// PORT NOTE -- this is NOT `char::is_whitespace`. JavaScript includes
/// U+00A0, U+FEFF, U+2028 and U+2029 and excludes U+0085; Rust's
/// `White_Space` property is the mirror image on both counts.
pub fn is_js_whitespace(c: char) -> bool {
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
            | '\u{2000}'..='\u{200A}'
            | '\u{2028}'
            | '\u{2029}'
            | '\u{202F}'
            | '\u{205F}'
            | '\u{3000}'
            | '\u{FEFF}'
    )
}

/// The four characters JavaScript's `.` excludes and `$` treats as a line
/// terminator.
pub fn is_js_line_terminator(c: char) -> bool {
    matches!(c, '\u{000A}' | '\u{000D}' | '\u{2028}' | '\u{2029}')
}

/// `Number.prototype.toString(16)` for an integral value, including the
/// negative case, which prints a `-` followed by the magnitude rather than a
/// two's-complement pattern.
pub fn to_string_16(value: f64) -> String {
    if value.is_nan() {
        return "NaN".to_string();
    }
    if value.is_infinite() {
        return (if value > 0.0 { "Infinity" } else { "-Infinity" }).to_string();
    }
    let magnitude = value.abs() as u64;
    let body = format!("{magnitude:x}");
    if value < 0.0 {
        format!("-{body}")
    } else {
        body
    }
}

/// `String.prototype.padStart(width, fill)`.
///
/// PORT NOTE -- the width is counted in UTF-16 code units, not `char`s and not
/// bytes. Every value padded here is ASCII, but the helper is written honestly
/// so a later caller with non-ASCII input does not silently drift.
pub fn pad_start(text: &str, width: usize, fill: char) -> String {
    let length = utf16_len(text);
    if length >= width {
        return text.to_string();
    }
    let mut out = String::new();
    for _ in 0..(width - length) {
        out.push(fill);
    }
    out.push_str(text);
    out
}

/// `String.prototype.padEnd(width, " ")`.
pub fn pad_end(text: &str, width: usize) -> String {
    let length = utf16_len(text);
    let mut out = text.to_string();
    if length >= width {
        return out;
    }
    for _ in 0..(width - length) {
        out.push(' ');
    }
    out
}

/// `String.prototype.slice(0, width)` in UTF-16 code units.
///
/// PORT NOTE -- JavaScript slices code units and will happily split a
/// surrogate pair. Rust cannot hold a lone surrogate in a `String`, so a split
/// pair is replaced with U+FFFD rather than panicking. `&text[..width]` would
/// panic on any non-ASCII boundary, and `chars().take(width)` would count
/// astral characters as one where JavaScript counts two.
pub fn slice_utf16(text: &str, width: usize) -> String {
    if utf16_len(text) <= width {
        return text.to_string();
    }
    let units: Vec<u16> = text.encode_utf16().take(width).collect();
    String::from_utf16_lossy(&units)
}

/// `String.prototype.length`.
pub fn utf16_len(text: &str) -> usize {
    text.encode_utf16().count()
}

/// `String.prototype.trimEnd()`, using JavaScript's whitespace set plus its
/// line terminators.
pub fn trim_end_js(text: &str) -> &str {
    text.trim_end_matches(|c| is_js_whitespace(c) || is_js_line_terminator(c))
}
