//! Load-bearing JavaScript string semantics for nm parsing and file ordering.

/// ECMAScript `WhiteSpace` + `LineTerminator`, which is what a bare `\s` in a
/// regular expression and what `String.prototype.trim` both match.
///
/// Do not use `char::is_whitespace`: Rust includes U+0085 but omits JS U+FEFF.
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

pub fn js_trim(text: &str) -> &str {
    text.trim_matches(is_js_space)
}

/// `text.split(/\s+/)`.
///
/// Unlike `split_whitespace`, JS retains boundary empty fields. Callers must trim
/// first before selecting the final nm field.
pub fn js_split_whitespace_runs(text: &str) -> Vec<&str> {
    if text.is_empty() {
        // JS returns [""] for this pattern on empty input.
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
/// Not `str::lines`: JS preserves a final empty field and only absorbs CR in
/// CRLF pairs.
pub fn js_split_lines(text: &str) -> Vec<&str> {
    let mut fields = Vec::new();
    let mut start = 0usize;
    let bytes = text.as_bytes();
    let mut i = 0usize;
    while i < bytes.len() {
        if bytes[i] == b'\n' {
            let end = if i > start && bytes[i - 1] == b'\r' { i - 1 } else { i };
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
/// This differs from Rust UTF-8 order across U+E000..U+FFFF and astral values;
/// it is not locale comparison.
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
