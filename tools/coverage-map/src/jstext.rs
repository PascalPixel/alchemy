// JavaScript string semantics the renderers depend on.
//
// PORT NOTE: `String#length` and `String#slice` count UTF-16 code units, not
// Unicode scalars and not bytes. Every label that reaches these helpers is BMP
// today (`·` U+00B7, `–` U+2013, `…` U+2026), where code units and scalars
// agree, but `slice` on a byte index would panic mid-character on any of them,
// so the code-unit definition is implemented rather than assumed.

/// `text.length`.
pub fn utf16_len(text: &str) -> usize {
    text.chars().map(char::len_utf16).sum()
}

/// `text.slice(0, end)` with `end` in UTF-16 code units.
///
/// A cut that would land inside a surrogate pair takes the whole pair's
/// preceding boundary, which is what an unpaired surrogate would render as
/// anyway; no tracked label can reach that case.
pub fn utf16_slice_to(text: &str, end: usize) -> String {
    let mut out = String::new();
    let mut units = 0usize;
    for character in text.chars() {
        let width = character.len_utf16();
        if units + width > end {
            break;
        }
        out.push(character);
        units += width;
    }
    out
}

/// `value.replace(/[&<>"']/g, ...)` with the five SVG entities.
pub fn escape_text(value: &str) -> String {
    let mut out = String::with_capacity(value.len());
    for character in value.chars() {
        match character {
            '&' => out.push_str("&amp;"),
            '<' => out.push_str("&lt;"),
            '>' => out.push_str("&gt;"),
            '"' => out.push_str("&quot;"),
            '\'' => out.push_str("&apos;"),
            _ => out.push(character),
        }
    }
    out
}

/// `text.replace(/[.*+?^${}()|[\]\\]/g, "\\$&")` -- the escape both README
/// rewriters and `leafDisplayName` use before building a `RegExp`.
pub fn escape_regex(text: &str) -> String {
    let mut out = String::with_capacity(text.len());
    for character in text.chars() {
        if matches!(
            character,
            '.' | '*' | '+' | '?' | '^' | '$' | '{' | '}' | '(' | ')' | '|' | '[' | ']' | '\\'
        ) {
            out.push('\\');
        }
        out.push(character);
    }
    out
}

#[cfg(test)]
mod tests {
    use super::{escape_regex, escape_text, utf16_len, utf16_slice_to};

    #[test]
    fn length_counts_code_units_not_bytes_or_scalars() {
        assert_eq!(utf16_len("a·b"), 3, "U+00B7 is one code unit and two bytes");
        assert_eq!(utf16_len("…"), 1);
        assert_eq!(
            utf16_len("\u{1f600}"),
            2,
            "an astral scalar is a surrogate pair"
        );
    }

    #[test]
    fn slicing_never_cuts_a_character_in_half() {
        assert_eq!(utf16_slice_to("a·b", 2), "a·");
        assert_eq!(
            utf16_slice_to("a·b", 9),
            "a·b",
            "slice clamps, it does not panic"
        );
        assert_eq!(utf16_slice_to("abc", 0), "");
    }

    #[test]
    fn the_five_svg_entities_are_all_replaced() {
        assert_eq!(
            escape_text("a&b<c>d\"e'f"),
            "a&amp;b&lt;c&gt;d&quot;e&apos;f"
        );
    }

    #[test]
    fn regex_metacharacters_are_escaped_and_hyphen_is_not() {
        assert_eq!(escape_regex("a.b*c-d"), "a\\.b\\*c-d");
        assert_eq!(escape_regex("x[1]{2}"), "x\\[1\\]\\{2\\}");
    }
}
