// Hand-written equivalents of the JavaScript regular expressions and string
// helpers the TypeScript ledger relies on.
//
// WHY: the original is a `bun` script whose behaviour is defined by JS regex
// semantics, and those differ from anything Rust would give by default. The
// two that bite here are the character class `\s` (JS includes U+FEFF and
// excludes U+0085, the reverse of Rust's `char::is_whitespace`) and `\b`
// (ASCII-only in JS). Each matcher below is written against the exact pattern
// it replaces, and the pattern is quoted above it.

/// JS `\s`: WhiteSpace ∪ LineTerminator. Note U+FEFF is in and U+0085 is out,
/// which is the opposite of Rust's `White_Space` property.
pub fn is_js_space(character: char) -> bool {
    matches!(
        character,
        '\t' | '\n'
            | '\u{b}'
            | '\u{c}'
            | '\r'
            | ' '
            | '\u{a0}'
            | '\u{1680}'
            | '\u{2000}'..='\u{200a}'
            | '\u{2028}'
            | '\u{2029}'
            | '\u{202f}'
            | '\u{205f}'
            | '\u{3000}'
            | '\u{feff}'
    )
}

/// JS `\w`: `[A-Za-z0-9_]`, ASCII only.
fn is_js_word(character: char) -> bool {
    character.is_ascii_alphanumeric() || character == '_'
}

fn is_hex(character: char) -> bool {
    character.is_ascii_hexdigit()
}

/// JS `\b` before `at`: the previous character is a non-word character (or the
/// match is at the start of the subject) while the next one is a word char.
fn word_boundary_before(text: &str, at: usize) -> bool {
    text[..at].chars().next_back().is_none_or(|previous| !is_js_word(previous))
}

fn word_boundary_after(text: &str, at: usize) -> bool {
    text[at..].chars().next().is_none_or(|next| !is_js_word(next))
}

fn find_word(text: &str, needle: &str) -> Vec<usize> {
    let mut hits = Vec::new();
    let mut from = 0usize;
    while let Some(offset) = text[from..].find(needle) {
        let at = from + offset;
        if word_boundary_before(text, at) && word_boundary_after(text, at + needle.len()) {
            hits.push(at);
        }
        from = at + 1;
    }
    hits
}

/// `/\bregister\b[^;\n]*\basm\s*\(/`
fn register_pinned(source: &str) -> bool {
    for start in find_word(source, "register") {
        let mut cursor = start + "register".len();
        let bytes = source.as_bytes();
        while cursor <= source.len() {
            if source.is_char_boundary(cursor)
                && source[cursor..].starts_with("asm")
                && word_boundary_before(source, cursor)
            {
                let mut after = cursor + 3;
                while let Some(character) = source[after..].chars().next() {
                    if is_js_space(character) {
                        after += character.len_utf8();
                    } else {
                        break;
                    }
                }
                if source[after..].starts_with('(') {
                    return true;
                }
            }
            if cursor >= source.len() || bytes[cursor] == b';' || bytes[cursor] == b'\n' {
                break;
            }
            cursor += 1;
        }
    }
    false
}

/// `/\b__asm__\b|\basm\s+volatile\b/`
fn inline_assembly(source: &str) -> bool {
    if !find_word(source, "__asm__").is_empty() {
        return true;
    }
    let mut from = 0usize;
    while let Some(offset) = source[from..].find("asm") {
        let at = from + offset;
        from = at + 1;
        if !word_boundary_before(source, at) {
            continue;
        }
        let mut cursor = at + 3;
        let mut spaces = 0usize;
        while let Some(character) = source[cursor..].chars().next() {
            if is_js_space(character) {
                cursor += character.len_utf8();
                spaces += 1;
            } else {
                break;
            }
        }
        if spaces == 0 {
            continue;
        }
        if source[cursor..].starts_with("volatile") && word_boundary_after(source, cursor + 8) {
            return true;
        }
    }
    false
}

/// `/\.incbin\b/`
fn incbin(source: &str) -> bool {
    let mut from = 0usize;
    while let Some(offset) = source[from..].find(".incbin") {
        let at = from + offset;
        if word_boundary_after(source, at + ".incbin".len()) {
            return true;
        }
        from = at + 1;
    }
    false
}

/// The four exclusion patterns of `acceptableHistoricalC`.
pub fn acceptable_historical_c(source: &str) -> bool {
    !(register_pinned(source)
        || inline_assembly(source)
        || incbin(source)
        || !find_word(source, "M2C_ERROR").is_empty())
}

fn trim_js_start(text: &str) -> &str {
    text.trim_start_matches(is_js_space)
}

fn all_js_space(text: &str) -> bool {
    text.chars().all(is_js_space)
}

fn strip_prefix_ci<'a>(text: &'a str, prefix: &str) -> Option<&'a str> {
    let end = prefix.len();
    if text.len() >= end && text.is_char_boundary(end) && text[..end].eq_ignore_ascii_case(prefix) {
        Some(&text[end..])
    } else {
        None
    }
}

fn take_hex8(text: &str) -> Option<(&str, &str)> {
    if text.len() >= 8 && text.is_char_boundary(8) && text[..8].chars().all(is_hex) {
        Some((&text[..8], &text[8..]))
    } else {
        None
    }
}

/// `/^\s*AlchemyC_([0-9a-f]{8}):\s*$/i` — returns the lowercased address.
pub fn alchemy_c_label(line: &str) -> Option<String> {
    let rest = strip_prefix_ci(trim_js_start(line), "AlchemyC_")?;
    let (address, rest) = take_hex8(rest)?;
    let rest = rest.strip_prefix(':')?;
    all_js_space(rest).then(|| address.to_ascii_lowercase())
}

/// `/^\s*$/`
pub fn blank_line(line: &str) -> bool {
    all_js_space(line)
}

/// `/^\s*\.L_[0-9a-z_.$]+:\s*$/i`
pub fn local_label(line: &str) -> bool {
    let Some(rest) = strip_prefix_ci(trim_js_start(line), ".L_") else { return false };
    let taken = rest
        .find(|character: char| {
            !(character.is_ascii_alphanumeric() || matches!(character, '_' | '.' | '$'))
        })
        .unwrap_or(rest.len());
    if taken == 0 {
        return false;
    }
    let rest = &rest[taken..];
    match rest.strip_prefix(':') {
        Some(tail) => all_js_space(tail),
        None => false,
    }
}

/// `/^\s*\.space\s+(0x[0-9a-f]+|\d+)\s*$/i` with `Number.parseInt(_, 0)`.
pub fn space_directive(line: &str) -> Option<i64> {
    let rest = strip_prefix_ci(trim_js_start(line), ".space")?;
    let trimmed = trim_js_start(rest);
    if trimmed.len() == rest.len() {
        return None; // `\s+` needs at least one space
    }
    let (digits, radix, tail) = match strip_prefix_ci(trimmed, "0x") {
        Some(after) => {
            let taken = after.find(|c: char| !is_hex(c)).unwrap_or(after.len());
            (&after[..taken], 16u32, &after[taken..])
        }
        None => {
            let taken = trimmed
                .find(|c: char| !c.is_ascii_digit())
                .unwrap_or(trimmed.len());
            (&trimmed[..taken], 10u32, &trimmed[taken..])
        }
    };
    if digits.is_empty() || !all_js_space(tail) {
        return None;
    }
    i64::from_str_radix(digits, radix).ok()
}

/// `/^(?:src|exact)\/([0-9a-f]{8})\.c$/i` — returns the lowercased address.
pub fn main_c_path(path: &str) -> Option<String> {
    let rest = strip_prefix_ci(path, "src/").or_else(|| strip_prefix_ci(path, "exact/"))?;
    let (address, rest) = take_hex8(rest)?;
    rest.eq_ignore_ascii_case(".c").then(|| address.to_ascii_lowercase())
}

/// `/([0-9a-f]{8})\.(?:c|s)$/i` on a manifest `source` — lowercased address.
pub fn region_stem(source: &str) -> Option<String> {
    let rest = source.strip_suffix(|c: char| c == 'c' || c == 'C' || c == 's' || c == 'S')?;
    let rest = rest.strip_suffix('.')?;
    if rest.len() < 8 || !rest.is_char_boundary(rest.len() - 8) {
        return None;
    }
    let address = &rest[rest.len() - 8..];
    address
        .chars()
        .all(is_hex)
        .then(|| address.to_ascii_lowercase())
}

/// `/^assets\/code\/(.+)_overlay\.s$/` (case-sensitive, greedy `.+`).
pub fn overlay_container(path: &str) -> Option<&str> {
    let rest = path.strip_prefix("assets/code/")?;
    let stem = rest.strip_suffix("_overlay.s")?;
    (!stem.is_empty() && !stem.contains('\n')).then_some(stem)
}

/// `/_c_([0-9a-f]{8})\.c$/i` — lowercased address.
pub fn overlay_c_address(path: &str) -> Option<String> {
    let rest = strip_suffix_ci(path, ".c")?;
    if rest.len() < 8 || !rest.is_char_boundary(rest.len() - 8) {
        return None;
    }
    let (head, address) = rest.split_at(rest.len() - 8);
    if !address.chars().all(is_hex) {
        return None;
    }
    strip_suffix_ci(head, "_c_")?;
    Some(address.to_ascii_lowercase())
}

fn strip_suffix_ci<'a>(text: &'a str, suffix: &str) -> Option<&'a str> {
    if text.len() < suffix.len() {
        return None;
    }
    let split = text.len() - suffix.len();
    if !text.is_char_boundary(split) || !text[split..].eq_ignore_ascii_case(suffix) {
        return None;
    }
    Some(&text[..split])
}

/// `/^[0-9]+\s+\w+\s+([0-9a-f]+)\t(.+)$/` over one `git ls-tree -r` row.
pub fn ls_tree_row(line: &str) -> Option<(String, String)> {
    let (head, path) = line.split_once('\t')?;
    if path.is_empty() {
        return None;
    }
    let mut rest = head;
    let digits = rest.find(|c: char| !c.is_ascii_digit()).unwrap_or(rest.len());
    if digits == 0 {
        return None;
    }
    rest = &rest[digits..];
    let spaces = rest.len() - trim_js_start(rest).len();
    if spaces == 0 {
        return None;
    }
    rest = trim_js_start(rest);
    let word = rest.find(|c: char| !is_js_word(c)).unwrap_or(rest.len());
    if word == 0 {
        return None;
    }
    rest = &rest[word..];
    let spaces = rest.len() - trim_js_start(rest).len();
    if spaces == 0 {
        return None;
    }
    rest = trim_js_start(rest);
    if rest.is_empty() || !rest.chars().all(is_hex) {
        return None;
    }
    Some((rest.to_string(), path.to_string()))
}

/// `csvCell`: quote only when the cell holds `"`, `,`, CR, or LF.
pub fn csv_cell(value: &str) -> String {
    if value.contains(['"', ',', '\r', '\n']) {
        format!("\"{}\"", value.replace('"', "\"\""))
    } else {
        value.to_string()
    }
}

/// `commas`, the `\B(?=(\d{3})+(?!\d))` thousands grouping over a decimal
/// integer rendering.
pub fn commas(value: i64) -> String {
    let text = value.to_string();
    let (sign, digits) = match text.strip_prefix('-') {
        Some(rest) => ("-", rest),
        None => ("", text.as_str()),
    };
    let mut out = String::new();
    for (index, character) in digits.chars().enumerate() {
        if index > 0 && (digits.len() - index) % 3 == 0 {
            out.push(',');
        }
        out.push(character);
    }
    format!("{sign}{out}")
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn excludes_the_four_noncanonical_c_shapes() {
        assert!(!acceptable_historical_c(r#"register int x asm("r4");"#));
        assert!(!acceptable_historical_c("__asm__(\"nop\");"));
        assert!(!acceptable_historical_c("asm  volatile (\"nop\");"));
        assert!(!acceptable_historical_c("/* .incbin \"x.bin\" */"));
        assert!(!acceptable_historical_c("return M2C_ERROR(unknown);"));
        assert!(acceptable_historical_c("int f(void) { return 1; }"));
    }

    #[test]
    fn register_pin_does_not_cross_a_semicolon_or_newline() {
        assert!(acceptable_historical_c("register int x; int y = asm ();"));
        // `[^;\n]*` cannot span the newline, so this stays acceptable.
        assert!(acceptable_historical_c("register int x\nint y = asm ();"));
        // A register with an asm operand later on the same statement is pinned.
        assert!(!acceptable_historical_c("register int x, y asm(\"r5\");"));
    }

    #[test]
    fn word_boundaries_are_ascii_only_like_js() {
        // `\basm\s+volatile\b`: the trailing boundary rejects `volatiles`.
        assert!(acceptable_historical_c("asm volatiles"));
        assert!(!acceptable_historical_c("asm volatile"));
        // A leading word char suppresses `\b`, so `fasm volatile` is fine.
        assert!(acceptable_historical_c("fasm volatile"));
        assert!(acceptable_historical_c("xM2C_ERROR"));
    }

    #[test]
    fn incbin_needs_a_trailing_word_boundary() {
        assert!(acceptable_historical_c(".incbinary"));
        assert!(!acceptable_historical_c(".incbin "));
    }

    #[test]
    fn placeholder_line_shapes() {
        assert_eq!(alchemy_c_label("AlchemyC_02000010:").as_deref(), Some("02000010"));
        assert_eq!(alchemy_c_label("  ALCHEMYC_0200ABCD:  ").as_deref(), Some("0200abcd"));
        assert_eq!(alchemy_c_label("AlchemyC_0200001:"), None);
        assert!(local_label("\t.L_02000012:"));
        assert!(local_label(".l_a.b$c:"));
        assert!(!local_label(".L_:"));
        assert!(blank_line("\u{feff} \t"));
        assert!(!blank_line("\u{85}")); // U+0085 is not JS \s
        assert_eq!(space_directive("\t.space 0x10"), Some(16));
        assert_eq!(space_directive("  .SPACE   12  "), Some(12));
        assert_eq!(space_directive(".space"), None);
        assert_eq!(space_directive(".space 12 x"), None);
    }

    #[test]
    fn path_shapes() {
        assert_eq!(main_c_path("src/0200ABCD.c").as_deref(), Some("0200abcd"));
        assert_eq!(main_c_path("exact/0200abcd.C").as_deref(), Some("0200abcd"));
        assert_eq!(main_c_path("other/0200abcd.c"), None);
        assert_eq!(region_stem("asm/0200abcd.s").as_deref(), Some("0200abcd"));
        assert_eq!(region_stem("x/0200ABCD.c").as_deref(), Some("0200abcd"));
        assert_eq!(region_stem("x/0200abcd.h"), None);
        assert_eq!(overlay_container("assets/code/kind1_overlay.s"), Some("kind1"));
        assert_eq!(overlay_container("assets/code/a_overlay_overlay.s"), Some("a_overlay"));
        assert_eq!(overlay_container("exact/kind1_overlay.s"), None);
        assert_eq!(
            overlay_c_address("exact/kind1_c_02000010.c").as_deref(),
            Some("02000010")
        );
        assert_eq!(overlay_c_address("exact/kind1_02000010.c"), None);
    }

    #[test]
    fn ls_tree_rows_split_on_the_first_tab() {
        let (oid, path) = ls_tree_row("100644 blob 0123abcd\tsrc/dir/a b.c").unwrap();
        assert_eq!(oid, "0123abcd");
        assert_eq!(path, "src/dir/a b.c");
        let (_, path) = ls_tree_row("100644 blob 0123abcd\ta\tb.c").unwrap();
        assert_eq!(path, "a\tb.c");
        assert!(ls_tree_row("100644 blob 0123abcd src/a.c").is_none());
    }

    #[test]
    fn csv_and_comma_helpers() {
        assert_eq!(csv_cell("plain"), "plain");
        assert_eq!(csv_cell("a,b"), "\"a,b\"");
        assert_eq!(csv_cell("say \"hi\""), "\"say \"\"hi\"\"\"");
        assert_eq!(commas(0), "0");
        assert_eq!(commas(999), "999");
        assert_eq!(commas(1000), "1,000");
        assert_eq!(commas(1345722), "1,345,722");
    }
}
