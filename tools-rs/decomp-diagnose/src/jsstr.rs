// JavaScript string and regex semantics that the diagnosis port depends on.
//
// WHY THIS FILE EXISTS
// The TypeScript normalizer leans on JS regex details that Rust's standard
// library does not share: `\s` includes U+FEFF and excludes U+0085, `\b` is
// ASCII-only, `.` excludes line terminators, and `String#trim` strips both
// whitespace and line terminators. Reimplementing them here keeps the
// classification identical instead of merely similar.

/// JS `\s`: WhiteSpace + LineTerminator. Note U+FEFF is included and U+0085 is
/// not — the exact inverse of Rust's `char::is_whitespace` on those two.
pub fn is_js_space(character: char) -> bool {
    matches!(
        character,
        '\u{9}'
            | '\u{a}'
            | '\u{b}'
            | '\u{c}'
            | '\u{d}'
            | '\u{20}'
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

/// JS line terminators, which `.` never matches.
pub fn is_line_terminator(character: char) -> bool {
    matches!(character, '\u{a}' | '\u{d}' | '\u{2028}' | '\u{2029}')
}

/// JS `String#trim`.
pub fn js_trim(value: &str) -> String {
    value
        .trim_matches(|character: char| is_js_space(character))
        .to_string()
}

fn is_word(character: char) -> bool {
    character.is_ascii_alphanumeric() || character == '_'
}

fn boundary_at(chars: &[char], index: usize) -> bool {
    let before = index > 0 && is_word(chars[index - 1]);
    let after = index < chars.len() && is_word(chars[index]);
    before != after
}

/// `value.replace(/\s*[;@].*$/, "")` — a single, non-global replacement whose
/// `$` is a true end-of-string because the source regex has no `m` flag.
pub fn strip_comment_tail(value: &str) -> String {
    let chars: Vec<char> = value.chars().collect();
    let mut start = 0usize;
    while start < chars.len() {
        let mut cursor = start;
        while cursor < chars.len() && is_js_space(chars[cursor]) {
            cursor += 1;
        }
        if cursor < chars.len() && (chars[cursor] == ';' || chars[cursor] == '@') {
            // `.*$` only succeeds when no line terminator follows.
            if !chars[cursor + 1..].iter().copied().any(is_line_terminator) {
                return chars[..start].iter().collect();
            }
        }
        start += 1;
    }
    value.to_string()
}

/// `value.replace(/\s+/g, " ")`.
pub fn replace_all_ws_runs(value: &str) -> String {
    let mut out = String::new();
    let mut in_run = false;
    for character in value.chars() {
        if is_js_space(character) {
            if !in_run {
                out.push(' ');
                in_run = true;
            }
        } else {
            in_run = false;
            out.push(character);
        }
    }
    out
}

/// `value.replace(/0x[0-9a-f]+\s+<[^>]+>/gi, "<address>")`.
pub fn substitute_addresses(value: &str) -> String {
    let chars: Vec<char> = value.chars().collect();
    let mut out = String::new();
    let mut index = 0usize;
    while index < chars.len() {
        if let Some(end) = match_address(&chars, index) {
            out.push_str("<address>");
            index = end;
            continue;
        }
        out.push(chars[index]);
        index += 1;
    }
    out
}

fn match_address(chars: &[char], start: usize) -> Option<usize> {
    if chars.get(start)? != &'0' {
        return None;
    }
    if !matches!(chars.get(start + 1), Some('x') | Some('X')) {
        return None;
    }
    let mut cursor = start + 2;
    let digits = cursor;
    while cursor < chars.len() && chars[cursor].is_ascii_hexdigit() {
        cursor += 1;
    }
    if cursor == digits {
        return None;
    }
    let spaces = cursor;
    while cursor < chars.len() && is_js_space(chars[cursor]) {
        cursor += 1;
    }
    if cursor == spaces {
        return None;
    }
    if chars.get(cursor) != Some(&'<') {
        return None;
    }
    cursor += 1;
    let inner = cursor;
    while cursor < chars.len() && chars[cursor] != '>' {
        cursor += 1;
    }
    if cursor == inner || cursor >= chars.len() {
        return None;
    }
    Some(cursor + 1)
}

const REGISTER_NAMES: [&str; 6] = ["sp", "lr", "pc", "ip", "fp", "sl"];

/// `text.replace(/\b(?:r(?:1[0-2]|[0-9])|sp|lr|pc|ip|fp|sl)\b/gi, "<reg>")`.
///
/// PORT NOTE: alternation order matters. `r1[0-2]` is tried before `r[0-9]`, so
/// `r10` matches whole while `r13` matches nothing at all (the trailing `\b`
/// rejects the `r1` prefix).
pub fn substitute_registers(text: &str) -> String {
    let chars: Vec<char> = text.chars().collect();
    let mut out = String::new();
    let mut index = 0usize;
    while index < chars.len() {
        if let Some(end) = match_register(&chars, index) {
            out.push_str("<reg>");
            index = end;
            continue;
        }
        out.push(chars[index]);
        index += 1;
    }
    out
}

fn match_register(chars: &[char], start: usize) -> Option<usize> {
    if !boundary_at(chars, start) {
        return None;
    }
    let mut lengths: Vec<usize> = Vec::new();
    if matches!(chars.get(start), Some('r') | Some('R')) {
        if matches!(chars.get(start + 1), Some('1'))
            && matches!(chars.get(start + 2), Some('0'..='2'))
        {
            lengths.push(3);
        }
        if chars.get(start + 1).is_some_and(char::is_ascii_digit) {
            lengths.push(2);
        }
    }
    let pair: String = chars
        .iter()
        .skip(start)
        .take(2)
        .collect::<String>()
        .to_lowercase();
    if REGISTER_NAMES.contains(&pair.as_str()) {
        lengths.push(2);
    }
    for length in lengths {
        if boundary_at(chars, start + length) {
            return Some(start + length);
        }
    }
    None
}

/// `new RegExp(`\\b${word}\\b`, "i").test(value)` for an ASCII word.
pub fn has_word(value: &str, word: &str) -> bool {
    let chars: Vec<char> = value.chars().collect();
    let needle: Vec<char> = word.chars().collect();
    for start in 0..=chars.len().saturating_sub(needle.len()) {
        if !boundary_at(&chars, start) {
            continue;
        }
        let matched = needle
            .iter()
            .enumerate()
            .all(|(offset, expected)| {
                chars
                    .get(start + offset)
                    .is_some_and(|actual| actual.eq_ignore_ascii_case(expected))
            });
        if matched && boundary_at(&chars, start + needle.len()) {
            return true;
        }
    }
    false
}

pub fn contains_ignore_ascii_case(haystack: &str, needle: &str) -> bool {
    haystack.to_ascii_lowercase().contains(&needle.to_ascii_lowercase())
}

/// `/0x08[0-9a-f]{6}/i.test(value)`.
pub fn has_rom_address_literal(value: &str) -> bool {
    let chars: Vec<char> = value.chars().collect();
    for start in 0..chars.len() {
        if chars[start] != '0' {
            continue;
        }
        if !matches!(chars.get(start + 1), Some('x') | Some('X')) {
            continue;
        }
        if chars.get(start + 2) != Some(&'0') || chars.get(start + 3) != Some(&'8') {
            continue;
        }
        if (0..6).all(|offset| {
            chars
                .get(start + 4 + offset)
                .is_some_and(char::is_ascii_hexdigit)
        }) {
            return true;
        }
    }
    false
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn js_whitespace_disagrees_with_rust_on_two_code_points() {
        assert!(is_js_space('\u{feff}'));
        assert!(!'\u{feff}'.is_whitespace());
        assert!(!is_js_space('\u{85}'));
        assert!('\u{85}'.is_whitespace());
    }

    #[test]
    fn comment_tails_are_cut_at_the_whitespace_run_before_the_marker() {
        assert_eq!(strip_comment_tail("r0, r1 \t@ 0x98"), "r0, r1");
        assert_eq!(strip_comment_tail("r0, r1; note"), "r0, r1");
        assert_eq!(strip_comment_tail("r0, r1"), "r0, r1");
        // `.*$` cannot cross a line terminator, so the match fails entirely.
        assert_eq!(strip_comment_tail("r0 @c\nmore"), "r0 @c\nmore");
    }

    #[test]
    fn whitespace_runs_collapse_to_one_space() {
        assert_eq!(replace_all_ws_runs("a \t\u{feff} b"), "a b");
        assert_eq!(replace_all_ws_runs("ab"), "ab");
    }

    #[test]
    fn address_operands_collapse_only_with_a_symbol_suffix() {
        assert_eq!(substitute_addresses("0x8003e58 <Func>"), "<address>");
        assert_eq!(substitute_addresses("0X8003E58 <F>!"), "<address>!");
        assert_eq!(substitute_addresses("0x8003e58"), "0x8003e58");
        assert_eq!(substitute_addresses("0x8003e58 <>"), "0x8003e58 <>");
    }

    #[test]
    fn register_substitution_respects_alternation_order_and_boundaries() {
        assert_eq!(substitute_registers("r0, r10, r12"), "<reg>, <reg>, <reg>");
        assert_eq!(substitute_registers("r13"), "r13");
        assert_eq!(substitute_registers("r1x"), "r1x");
        assert_eq!(substitute_registers("[sp, #4]"), "[<reg>, #4]");
        assert_eq!(substitute_registers("SPX"), "SPX");
        assert_eq!(substitute_registers("spam sp"), "spam <reg>");
    }

    #[test]
    fn word_and_literal_probes_are_ascii_boundary_aware() {
        assert!(has_word("[pc, #20]", "pc"));
        assert!(has_word("PC", "pc"));
        assert!(!has_word("pcs", "pc"));
        assert!(has_rom_address_literal("0x08003e58"));
        assert!(!has_rom_address_literal("0x0800"));
        assert!(!has_rom_address_literal("0x09003e58"));
    }

    #[test]
    fn trim_strips_line_terminators_and_bom() {
        assert_eq!(js_trim("\u{feff} a \n"), "a");
    }
}
