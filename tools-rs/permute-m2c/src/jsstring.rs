//! JavaScript string and character-class semantics.
//!
//! Every module in this crate that needs to know what `\s`, `\w`, `\d`,
//! `String#trim`, `String#split(/\r?\n/)` or the default `Array#sort` order
//! actually mean asks here, so that no other module has to remember it.

/// PORT NOTE (trap: JS `\s` is not Rust `char::is_whitespace`).
///
/// ECMAScript `WhiteSpace ∪ LineTerminator` is: TAB, LF, VT, FF, CR, SP,
/// NBSP (U+00A0), ZWNBSP/BOM (U+FEFF), LS (U+2028), PS (U+2029) and every
/// Unicode `Zs`. It **excludes** U+0085 NEL, which Rust's `White_Space`
/// *includes*. Using `char::is_whitespace` here would make `\s+` match a NEL
/// that JavaScript would not, and would miss the BOM that JavaScript does.
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
            | '\u{2000}'..='\u{200A}'
            | '\u{2028}'
            | '\u{2029}'
            | '\u{202F}'
            | '\u{205F}'
            | '\u{3000}'
            | '\u{FEFF}'
    )
}

/// PORT NOTE (trap: JS `\w` is ASCII-only). `char::is_alphanumeric` is
/// Unicode-aware and would match `é` or `０`, which `\w` never does.
pub fn is_js_word(c: char) -> bool {
    c.is_ascii_alphanumeric() || c == '_'
}

/// PORT NOTE (trap: JS `\d` is ASCII-only). `char::is_numeric` matches Devanagari
/// digits; `\d` does not.
pub fn is_js_digit(c: char) -> bool {
    c.is_ascii_digit()
}

/// `\b` at byte offset `at`: a word/non-word transition, ASCII-only.
pub fn is_word_boundary(text: &str, at: usize) -> bool {
    let before = text[..at].chars().next_back().is_some_and(is_js_word);
    let after = text[at..].chars().next().is_some_and(is_js_word);
    before != after
}

/// `String#trim`: strips the ECMAScript whitespace set from both ends.
pub fn js_trim(text: &str) -> &str {
    text.trim_matches(is_js_space)
}

/// `text.split(/\r?\n/)`.
///
/// PORT NOTE (trap: `str::lines` is not the same function). `str::lines` drops
/// a trailing empty final field and treats a lone `\r` as part of the line; the
/// JavaScript split keeps the trailing empty field, which is what makes
/// `.filter(Boolean)` load-bearing in `retainedAssemblyStems`.
pub fn split_crlf_lines(text: &str) -> Vec<&str> {
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

/// PORT NOTE (trap: three distinct string orders). The TypeScript calls a bare
/// `Array#sort()`, which is UTF-16 **code unit** order, not `localeCompare`
/// (ICU, case as a tertiary weight) and not Rust's `str::cmp` (UTF-8 byte
/// order). The three agree on ASCII and disagree above U+FFFF: `str::cmp` puts
/// astral characters after U+E000, code-unit order puts them before, because a
/// surrogate pair starts at U+D800.
pub fn code_unit_cmp(left: &str, right: &str) -> std::cmp::Ordering {
    let mut a = left.encode_utf16();
    let mut b = right.encode_utf16();
    loop {
        match (a.next(), b.next()) {
            (None, None) => return std::cmp::Ordering::Equal,
            (None, Some(_)) => return std::cmp::Ordering::Less,
            (Some(_), None) => return std::cmp::Ordering::Greater,
            (Some(x), Some(y)) if x != y => return x.cmp(&y),
            _ => {}
        }
    }
}

/// `String#replaceAll(needle, replacement)` with a literal needle.
///
/// PORT NOTE: an empty needle in JavaScript splices the replacement between
/// every code point and at both ends. `str::replace` agrees, so this is a thin
/// wrapper kept for the name; the empty case is pinned by a test regardless.
pub fn replace_all(text: &str, needle: &str, replacement: &str) -> String {
    if needle.is_empty() {
        // `"ab".replaceAll("", "-")` is `"-a-b-"`.
        let mut out = String::from(replacement);
        for c in text.chars() {
            out.push(c);
            out.push_str(replacement);
        }
        return out;
    }
    text.replace(needle, replacement)
}

/// `String#replace(needle, replacement)` with a literal needle: first
/// occurrence only.
pub fn replace_first(text: &str, needle: &str, replacement: &str) -> String {
    if needle.is_empty() {
        return format!("{replacement}{text}");
    }
    match text.find(needle) {
        Some(at) => format!("{}{}{}", &text[..at], replacement, &text[at + needle.len()..]),
        None => text.to_string(),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn js_space_excludes_nel_and_includes_bom() {
        // The exact pair Rust gets backwards.
        assert!(!is_js_space('\u{0085}'));
        assert!('\u{0085}'.is_whitespace());
        assert!(is_js_space('\u{FEFF}'));
        assert!(!'\u{FEFF}'.is_whitespace());
        assert!(is_js_space('\u{00A0}'));
        assert!(is_js_space('\u{3000}'));
    }

    #[test]
    fn js_word_and_digit_are_ascii_only() {
        assert!(!is_js_word('é'));
        assert!('é'.is_alphanumeric());
        assert!(!is_js_digit('٣'));
        assert!('٣'.is_numeric());
    }

    #[test]
    fn split_keeps_the_trailing_empty_field() {
        assert_eq!(split_crlf_lines("a\nb\n"), vec!["a", "b", ""]);
        assert_eq!("a\nb\n".lines().collect::<Vec<_>>(), vec!["a", "b"]);
        assert_eq!(split_crlf_lines("a\r\nb"), vec!["a", "b"]);
        assert_eq!(split_crlf_lines(""), vec![""]);
        // A lone CR is not a separator for `/\r?\n/`.
        assert_eq!(split_crlf_lines("a\rb"), vec!["a\rb"]);
    }

    #[test]
    fn code_unit_order_differs_from_utf8_order() {
        let astral = "\u{1F600}";
        let bmp = "\u{E000}";
        assert_eq!(code_unit_cmp(astral, bmp), std::cmp::Ordering::Less);
        assert_eq!(astral.cmp(bmp), std::cmp::Ordering::Greater);
    }

    #[test]
    fn trim_uses_the_js_whitespace_set() {
        assert_eq!(js_trim("\u{FEFF}x\u{00A0}"), "x");
        assert_eq!(js_trim("\u{0085}x"), "\u{0085}x");
    }

    #[test]
    fn empty_needle_replacements_match_js() {
        assert_eq!(replace_all("ab", "", "-"), "-a-b-");
        assert_eq!(replace_first("ab", "", "-"), "-ab");
    }
}
