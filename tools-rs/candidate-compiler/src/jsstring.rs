//! JavaScript string primitives that Rust's standard library gets subtly wrong.
//!
//! Candidate compilation parses the stdout of `arm-none-eabi-nm` with `trim()` and
//! `split(/\s+/)`, and it orders a directory listing with `Array#sort()`. All
//! three have JavaScript-specific definitions that differ from the obvious Rust
//! spelling, and all three are load-bearing here.

/// ECMAScript `WhiteSpace` + `LineTerminator`, which is what a bare `\s` in a
/// regular expression and what `String.prototype.trim` both match.
///
/// PORT NOTE -- this is deliberately NOT `char::is_whitespace`. The two sets
/// disagree in both directions:
///   * Rust's `White_Space` contains U+0085 NEXT LINE; JavaScript's `\s` does
///     not, so `"a\u{85}b".split(/\s+/)` is one element in JavaScript and two
///     under a naive port.
///   * JavaScript's `\s` contains U+FEFF ZERO WIDTH NO-BREAK SPACE; Rust's
///     `White_Space` does not.
///
/// `nm` output is ASCII in practice, so neither difference has ever been
/// observed here -- which is exactly why a "simplification" back to
/// `is_whitespace` would pass every functional test. `tests/js_traps.rs` pins
/// both directions so it cannot.
pub fn is_js_space(c: char) -> bool {
    matches!(
        c,
        '\u{9}'      // TAB
            | '\u{A}'    // LF (LineTerminator)
            | '\u{B}'    // VT
            | '\u{C}'    // FF
            | '\u{D}'    // CR (LineTerminator)
            | '\u{20}'   // SPACE
            | '\u{A0}'   // NBSP
            | '\u{1680}' // OGHAM SPACE MARK
            | '\u{2000}'
            ..='\u{200A}'
            | '\u{2028}' // LINE SEPARATOR (LineTerminator)
            | '\u{2029}' // PARAGRAPH SEPARATOR (LineTerminator)
            | '\u{202F}' // NARROW NBSP
            | '\u{205F}' // MEDIUM MATHEMATICAL SPACE
            | '\u{3000}' // IDEOGRAPHIC SPACE
            | '\u{FEFF}' // ZWNBSP
    )
}

/// `String.prototype.trim`.
pub fn js_trim(text: &str) -> &str {
    text.trim_matches(is_js_space)
}

/// `text.split(/\s+/)`.
///
/// PORT NOTE -- this is NOT `str::split_whitespace`, which drops the leading
/// and trailing empty fields. ECMAScript `String.prototype.split` with a
/// regular expression keeps them: `" a".split(/\s+/)` is `["", "a"]`. The call
/// site takes `.at(-1)`, so a trailing empty field would change the answer from
/// the symbol name to the empty string. It is only safe because `trim()` runs
/// first, and this function is written to make that dependency real rather than
/// accidental.
pub fn js_split_whitespace_runs(text: &str) -> Vec<&str> {
    if text.is_empty() {
        // ECMAScript special case: splitting the empty string on a pattern that
        // matches the empty string at position 0 yields [""], and `/\s+/` does
        // not match empty, so the result is [""] either way.
        return vec![""];
    }
    let mut fields = Vec::new();
    let mut field_start = 0usize;
    let mut index = 0usize;
    let bytes = text.len();
    let mut chars = text.char_indices().peekable();
    while let Some((offset, c)) = chars.next() {
        index = offset;
        if !is_js_space(c) {
            continue;
        }
        fields.push(&text[field_start..offset]);
        // Consume the whole run.
        let mut end = offset + c.len_utf8();
        while let Some(&(next_offset, next_char)) = chars.peek() {
            if !is_js_space(next_char) {
                break;
            }
            end = next_offset + next_char.len_utf8();
            chars.next();
        }
        field_start = end;
    }
    let _ = index;
    fields.push(&text[field_start..bytes]);
    fields
}

/// `lines.split(/\r?\n/)`.
///
/// PORT NOTE -- not `str::lines`, which also swallows a trailing `\r` that is
/// not followed by `\n` on some inputs and, more importantly, drops the final
/// empty field. `"a\n".split(/\r?\n/)` is `["a", ""]` in JavaScript and
/// `["a"]` under `str::lines`. The call sites filter falsy values or use
/// `.find`, so the difference is invisible today and would become visible the
/// moment someone indexes the result.
pub fn js_split_lines(text: &str) -> Vec<&str> {
    let mut fields = Vec::new();
    let mut start = 0usize;
    let bytes = text.as_bytes();
    let mut i = 0usize;
    while i < bytes.len() {
        if bytes[i] == b'\n' {
            let end = if i > start && bytes[i - 1] == b'\r' {
                i - 1
            } else {
                i
            };
            fields.push(&text[start..end]);
            start = i + 1;
        }
        i += 1;
    }
    fields.push(&text[start..]);
    fields
}

/// Compare two strings the way `Array.prototype.sort()` without a comparator
/// does: by UTF-16 code unit, not by UTF-8 byte and not by `char`.
///
/// PORT NOTE -- Rust's `Ord for str` compares UTF-8 bytes. The two orders
/// disagree for any pair straddling U+E000..U+FFFF versus the astral planes: a
/// surrogate pair encodes as 0xD800..0xDBFF, which sorts *below* U+E000, while
/// the UTF-8 encoding of an astral character sorts *above* every BMP character.
/// This is also explicitly not `localeCompare`, which the TypeScript does not
/// call here.
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
