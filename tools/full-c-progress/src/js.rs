// Hand-written equivalents of the JavaScript regular expressions and string
// helpers retained for exact compatibility with the metric's historical output.
//
// WHY: the original is a `bun` script whose behaviour is defined by JS regex
// and JS string semantics, and those differ from anything Rust gives by
// default. The ones that bite here:
//
//   * `\s` in JS is WhiteSpace ∪ LineTerminator: U+FEFF is IN and U+0085 is
//     OUT, which is exactly the reverse of Rust's `char::is_whitespace`.
//   * `\b`, `\w` and `\d` are ASCII-only in JS.
//   * JS regexes are unanchored unless `^`/`$` say otherwise, alternation is
//     ordered leftmost, and quantifiers are greedy with backtracking.
//   * `.` excludes \n, \r, U+2028 and U+2029; `$` without `m` is end-of-string.
//   * `Number.parseInt(text, 0)` is auto-radix (`0x` → 16, otherwise 10) and
//     `Number.parseInt` stops at the first invalid character.
//
// Each matcher below is written against the exact pattern it replaces, and the
// pattern is quoted above it. Several were lifted from
// `tools/full-c-history/src/js.rs`, which inlined the same helpers; they are
// copied rather than shared because these crates are independently buildable.

/// JS `\s`: WhiteSpace ∪ LineTerminator. Note U+FEFF is in and U+0085 is out,
/// which is the opposite of Rust's `White_Space` property.
pub fn is_js_space(character: char) -> bool {
    matches!(
        character,
        '\t' | '\n' | '\u{b}' | '\u{c}' | '\r' | ' ' | '\u{a0}' | '\u{1680}' | '\u{2000}'
            ..='\u{200a}'
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

/// JS `.`: any code point except the four line terminators.
fn is_js_dot(character: char) -> bool {
    !matches!(character, '\n' | '\r' | '\u{2028}' | '\u{2029}')
}

fn word_boundary_before(text: &str, at: usize) -> bool {
    text[..at]
        .chars()
        .next_back()
        .is_none_or(|previous| !is_js_word(previous))
}

fn word_boundary_after(text: &str, at: usize) -> bool {
    text[at..]
        .chars()
        .next()
        .is_none_or(|next| !is_js_word(next))
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

/// Split on `/\r?\n/`, the JS line split every reader in the tool uses.
///
/// PORT NOTE: this is NOT `str::lines` — `lines` also drops a trailing `\n`
/// and treats a lone `\r` as nothing, whereas `String.split(/\r?\n/)` keeps a
/// trailing empty element and leaves a lone `\r` inside the line.
pub fn split_lines(text: &str) -> Vec<&str> {
    let mut lines = Vec::new();
    let bytes = text.as_bytes();
    let mut start = 0usize;
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
        } else {
            cursor += 1;
        }
    }
    lines.push(&text[start..]);
    lines
}

// ---------------------------------------------------------------------------
// canonicalCSource
// ---------------------------------------------------------------------------

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

/// `canonicalCSource`: true when none of the four non-canonical shapes appear.
pub fn canonical_c_source(source: &str) -> bool {
    !(register_pinned(source)
        || inline_assembly(source)
        || incbin(source)
        || !find_word(source, "M2C_ERROR").is_empty())
}

// ---------------------------------------------------------------------------
// Number formatting and parsing
// ---------------------------------------------------------------------------

/// `commas`: the `\B(?=(\d{3})+(?!\d))` thousands grouping over `String(value)`.
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

/// `Number.parseInt(text, 0)` restricted to the shapes the two call sites can
/// produce: `0x`-prefixed hex, or a decimal run. Both are already validated by
/// the regex that captured them, so this cannot see a stray sign or suffix.
fn parse_int_auto(text: &str) -> Option<i64> {
    match strip_prefix_ci(text, "0x") {
        Some(hex) => i64::from_str_radix(hex, 16).ok(),
        None => text.parse::<i64>().ok(),
    }
}

// ---------------------------------------------------------------------------
// Commit-subject grammar
// ---------------------------------------------------------------------------

/// `[0-9]{1,3}(?:,[0-9]{3})*`, greedy.
///
/// PORT NOTE: the pattern is always followed by a literal that shares no
/// character with `[0-9,]`, so the greedy run is forced to the whole
/// digit/comma run and backtracking can only shorten it into a position the
/// following literal rejects. Requiring the WHOLE run to parse is therefore
/// exactly equivalent, and it is what makes `1234 / 1,234` fail to match.
fn canonical_number_run(text: &str) -> Option<(&str, &str)> {
    let taken = text
        .find(|character: char| !(character.is_ascii_digit() || character == ','))
        .unwrap_or(text.len());
    let (run, rest) = text.split_at(taken);
    let mut groups = run.split(',');
    let head = groups.next()?;
    if head.is_empty() || head.len() > 3 || !head.chars().all(|c| c.is_ascii_digit()) {
        return None;
    }
    for group in groups {
        if group.len() != 3 || !group.chars().all(|c| c.is_ascii_digit()) {
            return None;
        }
    }
    Some((run, rest))
}

const SUBJECT_OPEN: &str = "[ \u{2600}\u{fe0f} ";

/// `/\[ ☀️ ([0-9]{1,3}(?:,[0-9]{3})*) \/ ([0-9]{1,3}(?:,[0-9]{3})*) \]$/`
///
/// Unanchored at the start (leftmost match wins) and anchored at the end.
pub fn subject_match(subject: &str) -> Option<(&str, &str)> {
    let mut from = 0usize;
    while let Some(offset) = subject[from..].find(SUBJECT_OPEN) {
        let at = from + offset;
        from = at + 1;
        let rest = &subject[at + SUBJECT_OPEN.len()..];
        let Some((numerator, rest)) = canonical_number_run(rest) else {
            continue;
        };
        let Some(rest) = rest.strip_prefix(" / ") else {
            continue;
        };
        let Some((denominator, rest)) = canonical_number_run(rest) else {
            continue;
        };
        if rest == " ]" {
            return Some((numerator, denominator));
        }
    }
    None
}

// ---------------------------------------------------------------------------
// Assembler listing and source-line grammar
// ---------------------------------------------------------------------------

pub struct ListingMatch<'a> {
    pub source_line: i64,
    pub address_hex: &'a str,
    /// `match[3].length`, before the `/ 2` the caller applies.
    pub encoding_digits: usize,
}

fn take_run(text: &str, accept: impl Fn(char) -> bool) -> (&str, &str) {
    let taken = text
        .find(|character: char| !accept(character))
        .unwrap_or(text.len());
    text.split_at(taken)
}

/// `/^\s*(\d+)\s+([0-9a-f]+)\s+([0-9a-f]{4,})\s+(.*)$/i`
///
/// PORT NOTE: every run in this pattern is bounded by a character class the
/// next atom cannot accept (digits/hex against whitespace), so the greedy match
/// is deterministic and no backtracking is possible. The trailing `(.*)$`
/// still has to reject an embedded line terminator, because JS `.` excludes
/// them and `$` without the `m` flag only matches the end of the string.
pub fn listing_row(line: &str) -> Option<ListingMatch<'_>> {
    let rest = trim_js_start(line);
    let (digits, rest) = take_run(rest, |c| c.is_ascii_digit());
    if digits.is_empty() {
        return None;
    }
    let trimmed = trim_js_start(rest);
    if trimmed.len() == rest.len() {
        return None;
    }
    let (address, rest) = take_run(trimmed, is_hex);
    if address.is_empty() {
        return None;
    }
    let trimmed = trim_js_start(rest);
    if trimmed.len() == rest.len() {
        return None;
    }
    let (encoding, rest) = take_run(trimmed, is_hex);
    if encoding.len() < 4 {
        return None;
    }
    let trimmed = trim_js_start(rest);
    if trimmed.len() == rest.len() {
        return None;
    }
    if !trimmed.chars().all(is_js_dot) {
        return None;
    }
    Some(ListingMatch {
        source_line: digits.parse::<i64>().ok()?,
        address_hex: address,
        encoding_digits: encoding.len(),
    })
}

/// `/^\s*\.4byte\s+(0x[0-9a-f]+|\d+)\s*$/i`
pub fn directive_value(line: &str) -> Option<i64> {
    let rest = strip_prefix_ci(trim_js_start(line), ".4byte")?;
    number_operand(rest)
}

/// `/^\s*\.space\s+(0x[0-9a-f]+|\d+)\s*$/i`
pub fn space_directive(line: &str) -> Option<i64> {
    let rest = strip_prefix_ci(trim_js_start(line), ".space")?;
    number_operand(rest)
}

/// The shared `\s+(0x[0-9a-f]+|\d+)\s*$` tail. The alternation is ordered, so
/// the hex branch is tried first; when it fails the decimal branch runs from
/// the same position.
fn number_operand(rest: &str) -> Option<i64> {
    let trimmed = trim_js_start(rest);
    if trimmed.len() == rest.len() {
        return None; // `\s+` needs at least one space
    }
    if let Some(after) = strip_prefix_ci(trimmed, "0x") {
        let (hex, tail) = take_run(after, is_hex);
        if !hex.is_empty() && all_js_space(tail) {
            return parse_int_auto(&format!("0x{hex}"));
        }
    }
    let (decimal, tail) = take_run(trimmed, |c| c.is_ascii_digit());
    if decimal.is_empty() || !all_js_space(tail) {
        return None;
    }
    parse_int_auto(decimal)
}

/// `/^\s*AlchemyC_([0-9a-f]{8}):\s*$/i` — the capture, with its original case.
pub fn alchemy_c_label(line: &str) -> Option<&str> {
    let rest = strip_prefix_ci(trim_js_start(line), "AlchemyC_")?;
    if rest.len() < 8 || !rest.is_char_boundary(8) || !rest[..8].chars().all(is_hex) {
        return None;
    }
    let (address, rest) = rest.split_at(8);
    all_js_space(rest.strip_prefix(':')?).then_some(address)
}

fn local_label_body(text: &str) -> Option<(usize, &str)> {
    let rest = strip_prefix_ci(text, ".L_")?;
    let (body, tail) = take_run(rest, |c: char| {
        c.is_ascii_alphanumeric() || matches!(c, '_' | '.' | '$')
    });
    if body.is_empty() {
        return None;
    }
    Some((3 + body.len(), tail))
}

/// `/^\s*(\.L_[0-9a-z_.$]+):\s*$/i` — the capture, with its original case.
pub fn local_label(line: &str) -> Option<&str> {
    let text = trim_js_start(line);
    let (length, tail) = local_label_body(text)?;
    all_js_space(tail.strip_prefix(':')?).then(|| &text[..length])
}

/// `/^\s*$/`
pub fn blank_line(line: &str) -> bool {
    all_js_space(line)
}

/// `/^\s*b(?:l?x?|[a-z]{2})?(?:\.[nw])?\s+(\.L_[0-9a-z_.$]+)\s*$/i`
///
/// PORT NOTE: the alternation is ordered and the whole group is optional, so
/// the engine tries, in order: `l?x?` matching the longest of `lx`/`l`/`x`/``,
/// then `[a-z]{2}`, then the group absent. `beq .L_x` only matches through the
/// `[a-z]{2}` branch, after `l?x?` has matched empty and failed at `\s+`.
pub fn branch_label(line: &str) -> Option<&str> {
    let text = trim_js_start(line);
    let rest = strip_prefix_ci(text, "b")?;
    let mut candidates: Vec<usize> = Vec::new();
    let lower: Vec<char> = rest
        .chars()
        .take(2)
        .map(|c| c.to_ascii_lowercase())
        .collect();
    let mut mnemonic = 0usize;
    if lower.first() == Some(&'l') {
        mnemonic += 1;
    }
    if lower.get(mnemonic) == Some(&'x') {
        mnemonic += 1;
    }
    // `l?x?` backtracks from the longest match down to empty.
    for length in (0..=mnemonic).rev() {
        candidates.push(length);
    }
    if lower.len() == 2 && lower.iter().all(|c| c.is_ascii_lowercase()) {
        candidates.push(2);
    }
    if !candidates.contains(&0) {
        candidates.push(0);
    }
    for skip in candidates {
        let after = &rest[skip..];
        for suffix in [2usize, 0] {
            let after = if suffix == 2 {
                let mut characters = after.chars();
                if characters.next() != Some('.') {
                    continue;
                }
                match characters.next().map(|c| c.to_ascii_lowercase()) {
                    Some('n') | Some('w') => &after[2..],
                    _ => continue,
                }
            } else {
                after
            };
            let trimmed = trim_js_start(after);
            if trimmed.len() == after.len() {
                continue; // `\s+`
            }
            let Some((length, tail)) = local_label_body(trimmed) else {
                continue;
            };
            if all_js_space(tail) {
                return Some(&trimmed[..length]);
            }
        }
    }
    None
}

/// `/^\s*\.(?:2byte|hword)\b/i`
pub fn halfword_directive(line: &str) -> bool {
    directive_word(line, &["2byte", "hword"])
}

/// `/^\s*\.(?:2byte|byte|hword|word|space)\b/i`
pub fn data_directive(line: &str) -> bool {
    directive_word(line, &["2byte", "byte", "hword", "word", "space"])
}

fn directive_word(line: &str, words: &[&str]) -> bool {
    let Some(rest) = trim_js_start(line).strip_prefix('.') else {
        return false;
    };
    words.iter().any(|word| match strip_prefix_ci(rest, word) {
        Some(tail) => word_boundary_after(tail, 0),
        None => false,
    })
}

/// `/^\s*\./`
pub fn any_directive(line: &str) -> bool {
    trim_js_start(line).starts_with('.')
}

/// `/^\s*[a-z][a-z0-9.]*\s/i`
pub fn mnemonic_line(line: &str) -> bool {
    let text = trim_js_start(line);
    let mut characters = text.chars();
    if !characters.next().is_some_and(|c| c.is_ascii_alphabetic()) {
        return false;
    }
    // `[a-z0-9.]*` under `/i` is exactly "ASCII alphanumeric or dot".
    let (_, tail) = take_run(&text[1..], |c: char| c.is_ascii_alphanumeric() || c == '.');
    tail.chars().next().is_some_and(is_js_space)
}

/// `/\bldr\w*\s+r\d+,\s*\[pc,\s*#(0x[0-9a-f]+|\d+)\]/i` — leftmost match.
pub fn pc_relative_literal(line: &str) -> Option<i64> {
    let lowered = line.to_ascii_lowercase();
    let mut from = 0usize;
    while let Some(offset) = lowered[from..].find("ldr") {
        let at = from + offset;
        from = at + 1;
        if !word_boundary_before(&lowered, at) {
            continue;
        }
        let (_, rest) = take_run(&lowered[at + 3..], is_js_word);
        let trimmed = trim_js_start(rest);
        if trimmed.len() == rest.len() {
            continue;
        }
        let Some(rest) = trimmed.strip_prefix('r') else {
            continue;
        };
        let (digits, rest) = take_run(rest, |c: char| c.is_ascii_digit());
        if digits.is_empty() {
            continue;
        }
        let Some(rest) = rest.strip_prefix(',') else {
            continue;
        };
        let rest = trim_js_start(rest);
        let Some(rest) = rest.strip_prefix("[pc,") else {
            continue;
        };
        let rest = trim_js_start(rest);
        let Some(rest) = rest.strip_prefix('#') else {
            continue;
        };
        if let Some(after) = strip_prefix_ci(rest, "0x") {
            let (hex, tail) = take_run(after, is_hex);
            if !hex.is_empty() && tail.starts_with(']') {
                return parse_int_auto(&format!("0x{hex}"));
            }
        }
        let (decimal, tail) = take_run(rest, |c: char| c.is_ascii_digit());
        if !decimal.is_empty() && tail.starts_with(']') {
            return parse_int_auto(decimal);
        }
    }
    None
}

/// `/^resource_.+_overlay\.s$/` over a directory entry name.
///
/// PORT NOTE: `.` excludes line terminators, so a filename holding one does not
/// match. `.+` is greedy but the suffix is a literal, so any split works.
pub fn overlay_source_name(name: &str) -> bool {
    let Some(rest) = name.strip_prefix("resource_") else {
        return false;
    };
    let Some(stem) = rest.strip_suffix("_overlay.s") else {
        return false;
    };
    !stem.is_empty() && stem.chars().all(is_js_dot)
}

/// `basename.replace(/_overlay\.s$/, "")`
pub fn strip_overlay_suffix(name: &str) -> String {
    match name.strip_suffix("_overlay.s") {
        Some(stem) => stem.to_string(),
        None => name.to_string(),
    }
}

/// `basename.replace(/overlay\.s$/, replacement)`
pub fn replace_overlay_suffix(name: &str, replacement: &str) -> String {
    match name.strip_suffix("overlay.s") {
        Some(head) => format!("{head}{replacement}"),
        None => name.to_string(),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn canonical_c_source_rejects_the_four_shapes() {
        assert!(!canonical_c_source(r#"register int x asm("r4");"#));
        assert!(!canonical_c_source("__asm__(\"nop\");"));
        assert!(!canonical_c_source("asm  volatile (\"nop\");"));
        assert!(!canonical_c_source("/* .incbin \"x.bin\" */"));
        assert!(!canonical_c_source("return M2C_ERROR(unknown);"));
        assert!(canonical_c_source("int Func_02000030(void) { return 1; }"));
        // JS `\b` is ASCII-only, so a leading word character suppresses it.
        assert!(canonical_c_source("fasm volatile"));
        assert!(canonical_c_source(".incbinary"));
    }

    #[test]
    fn split_lines_is_not_str_lines() {
        assert_eq!(split_lines("a\r\nb\n"), vec!["a", "b", ""]);
        assert_eq!(split_lines(""), vec![""]);
        assert_eq!(split_lines("a\rb"), vec!["a\rb"]);
    }

    #[test]
    fn js_space_disagrees_with_rust_whitespace() {
        assert!(blank_line("\u{feff} \t"));
        assert!(!blank_line("\u{85}"));
        assert!(!char::is_whitespace('\u{feff}'));
        assert!(char::is_whitespace('\u{85}'));
    }

    #[test]
    fn subject_grammar_matches_the_checked_examples() {
        assert_eq!(
            subject_match("decomp: x [ \u{2600}\u{fe0f} 123 / 1,234 ]"),
            Some(("123", "1,234"))
        );
        assert_eq!(subject_match("x [ \u{2600}\u{fe0f} 1234 / 1,234 ]"), None);
        assert_eq!(subject_match("x [ \u{2600}\u{fe0f} 123/1,234 ]"), None);
        assert_eq!(
            subject_match("x [ \u{2600}\u{fe0f} 123 / 1,234 bytes]"),
            None
        );
        assert_eq!(subject_match("x [C 123,456/1,234,567 bytes]"), None);
        assert_eq!(subject_match("x [123 of 456]"), None);
        assert_eq!(
            subject_match("x [ \u{2600}\u{fe0f} 2 / 1 ]"),
            Some(("2", "1"))
        );
        // Anchored at the end only: trailing text after `]` kills the match.
        assert_eq!(subject_match("[ \u{2600}\u{fe0f} 1 / 2 ] tail"), None);
        // Leftmost of two candidates wins; the first one here cannot reach `$`.
        assert_eq!(
            subject_match("[ \u{2600}\u{fe0f} 9 / 9 ] [ \u{2600}\u{fe0f} 1 / 2 ]"),
            Some(("1", "2"))
        );
    }

    #[test]
    fn commas_group_by_three() {
        assert_eq!(commas(0), "0");
        assert_eq!(commas(999), "999");
        assert_eq!(commas(1000), "1,000");
        assert_eq!(commas(1345890), "1,345,890");
    }

    #[test]
    fn listing_rows_need_four_encoding_digits() {
        let row = listing_row("   7 0004 4770     \tbx lr").unwrap();
        assert_eq!(row.source_line, 7);
        assert_eq!(row.address_hex, "0004");
        assert_eq!(row.encoding_digits, 4);
        assert!(listing_row("   7 0004 47     bx lr").is_none());
        assert!(listing_row("   7 0004 4770").is_none());
        assert!(listing_row(" x 0004 4770 bx lr").is_none());
        // Uppercase hex is accepted (the /i flag).
        assert_eq!(listing_row(" 1 000A ABCD  x").unwrap().address_hex, "000A");
        // JS `.` excludes U+2028, and `$` is end-of-string.
        assert!(listing_row(" 1 0000 4770 a\u{2028}b").is_none());
    }

    #[test]
    fn directive_operands_are_auto_radix() {
        assert_eq!(directive_value("\t.4byte 0x08000101"), Some(0x0800_0101));
        assert_eq!(directive_value("  .4BYTE   12  "), Some(12));
        assert_eq!(directive_value(".4byte 012"), Some(12));
        assert_eq!(directive_value(".4byte 0x"), None);
        assert_eq!(directive_value(".4byte 1 2"), None);
        assert_eq!(space_directive("\t.space 0x10"), Some(16));
        assert_eq!(space_directive(".space"), None);
    }

    #[test]
    fn label_shapes() {
        assert_eq!(alchemy_c_label("AlchemyC_0200ABCD:  "), Some("0200ABCD"));
        assert_eq!(alchemy_c_label("  alchemyc_02000010:"), Some("02000010"));
        assert_eq!(alchemy_c_label("AlchemyC_0200001:"), None);
        assert_eq!(local_label("\t.L_020022bc:"), Some(".L_020022bc"));
        assert_eq!(local_label(".L_:"), None);
    }

    #[test]
    fn branch_alternation_is_ordered_and_backtracks() {
        assert_eq!(branch_label("\tb .L_02000010"), Some(".L_02000010"));
        assert_eq!(branch_label("\tbl .L_02000010"), Some(".L_02000010"));
        assert_eq!(branch_label("\tblx .L_02000010"), Some(".L_02000010"));
        assert_eq!(branch_label("\tbx .L_02000010"), Some(".L_02000010"));
        assert_eq!(branch_label("\tbeq .L_02000010"), Some(".L_02000010"));
        assert_eq!(branch_label("\tb.n .L_020022bc"), Some(".L_020022bc"));
        assert_eq!(branch_label("\tbne.w .L_02000010"), Some(".L_02000010"));
        assert_eq!(branch_label("\tbic r0, r1"), None);
        assert_eq!(branch_label("\tbl 0x02000010"), None);
        assert_eq!(branch_label("\tbeq .L_02000010 ; note"), None);
        // Three letters after `b` cannot be spelled by either alternative.
        assert_eq!(branch_label("\tbxyz .L_02000010"), None);
    }

    #[test]
    fn directive_prefixes_respect_word_boundaries() {
        assert!(halfword_directive("\t.2byte 0x4770"));
        assert!(halfword_directive("  .HWORD 1"));
        assert!(!halfword_directive("\t.4byte 1"));
        assert!(!halfword_directive("\t.hwords 1"));
        assert!(data_directive("\t.word 1"));
        assert!(data_directive("\t.byte 1"));
        assert!(data_directive("\t.space 4"));
        assert!(!data_directive("\t.ascii \"x\""));
        assert!(any_directive("   .thumb"));
        assert!(!any_directive("bx lr"));
    }

    #[test]
    fn mnemonic_lines_need_a_trailing_space() {
        assert!(mnemonic_line("\tbx lr"));
        assert!(mnemonic_line("\tldr.w r0, [pc, #4]"));
        assert!(!mnemonic_line("\tbx"));
        assert!(!mnemonic_line("\t.thumb x"));
        assert!(!mnemonic_line("\t1abc x"));
    }

    #[test]
    fn pc_relative_literals_are_found_anywhere_in_the_line() {
        assert_eq!(pc_relative_literal("\tldr r0, [pc, #0x10]"), Some(16));
        assert_eq!(pc_relative_literal("\tldrb r12, [pc,#20]"), Some(20));
        assert_eq!(pc_relative_literal("\t/* x */ LDR R1, [PC, #4]"), Some(4));
        assert_eq!(pc_relative_literal("\tldr r0, [r1, #4]"), None);
        assert_eq!(pc_relative_literal("\txldr r0, [pc, #4]"), None);
    }

    #[test]
    fn overlay_name_helpers() {
        assert!(overlay_source_name("resource_36f_overlay.s"));
        assert!(!overlay_source_name("resource_overlay.s"));
        assert!(!overlay_source_name("other_36f_overlay.s"));
        assert_eq!(
            strip_overlay_suffix("resource_36f_overlay.s"),
            "resource_36f"
        );
        assert_eq!(
            replace_overlay_suffix("resource_36f_overlay.s", "c_02000030.c"),
            "resource_36f_c_02000030.c"
        );
    }
}
