//! The eight regular expressions of `permute_m2c.ts`, hand-compiled.
//!
//! PORT NOTE (trap: the `regex` crate is not an ECMAScript engine). Two of
//! these patterns rely on greedy backtracking to pick *which* `Func_08xxxxxx`
//! in a run belongs to the signature, which a leftmost-longest DFA does not
//! reproduce, and one of them (`STATEMENT`) has an ordered alternation whose
//! first-match-wins behaviour distinguishes `void **` from `void *`. Rather
//! than depend on a general engine, each pattern is written out as an explicit
//! matcher with the same anchoring, greediness and alternation order, and each
//! carries the flags of its source pattern in its doc comment.
//!
//! PORT NOTE (trap: missing `i` flag). The flags are transcribed per pattern.
//! `HARDWARE` deliberately matches `0x04`/`0x0E` but **not** `0x0e`, and
//! `ADDRESS` deliberately matches both cases, because that is what the
//! TypeScript does. [`asm_literal_row`] is the one pattern that carries `i`.

use crate::jsstring::{is_js_digit, is_js_space, is_js_word};

// ---------------------------------------------------------------------------
// Small scanners.
// ---------------------------------------------------------------------------

fn run_while(text: &str, from: usize, predicate: impl Fn(char) -> bool) -> usize {
    let mut at = from;
    for c in text[from..].chars() {
        if !predicate(c) {
            break;
        }
        at += c.len_utf8();
    }
    at
}

fn hex_run(text: &str, from: usize) -> usize {
    run_while(text, from, |c| c.is_ascii_hexdigit())
}

fn literal(text: &str, at: usize, needle: &str) -> Option<usize> {
    if text[at..].starts_with(needle) {
        Some(at + needle.len())
    } else {
        None
    }
}

/// `0x[0-9A-Fa-f]+`, greedy, at a fixed position.
fn hex_literal(text: &str, at: usize) -> Option<(usize, &str)> {
    let after_prefix = literal(text, at, "0x")?;
    let end = hex_run(text, after_prefix);
    if end == after_prefix {
        return None;
    }
    Some((end, &text[at..end]))
}

/// The character class `[\w\[\]().*>-]`.
fn is_target_char(c: char) -> bool {
    is_js_word(c) || matches!(c, '[' | ']' | '(' | ')' | '.' | '*' | '>' | '-')
}

/// The character class `[\w* ]`.
fn is_signature_char(c: char) -> bool {
    is_js_word(c) || c == '*' || c == ' '
}

// ---------------------------------------------------------------------------
// STATEMENT
// ---------------------------------------------------------------------------

/// One match of the `STATEMENT` pattern (flags: `g`).
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct StatementMatch {
    pub start: usize,
    pub end: usize,
    pub indent: String,
    pub target: String,
    pub address: String,
    pub index: String,
    pub stride: String,
    pub kind: String,
    pub offset: String,
}

/// Ordered exactly as the source alternation. `void **` must precede `void *`.
const KINDS: [&str; 6] = ["s32 *", "void **", "u32 *", "s16 *", "u8 *", "void *"];

fn statement_at(text: &str, start: usize) -> Option<StatementMatch> {
    // `( *)` -- greedy, then backtracked. Nothing that can follow it begins
    // with a space, so only the longest run can succeed; the loop exists so a
    // later edit to the pattern cannot quietly break the semantics.
    let indent_end = run_while(text, start, |c| c == ' ');
    let mut indent_len = indent_end - start;
    loop {
        let after_indent = start + indent_len;
        if let Some(found) = statement_after_indent(text, start, after_indent) {
            return Some(found);
        }
        if indent_len == 0 {
            return None;
        }
        indent_len -= 1;
    }
}

fn statement_after_indent(text: &str, start: usize, at: usize) -> Option<StatementMatch> {
    // `((?:[\w\[\]().*>-]+ = )|(?:return ))`, ordered alternation.
    let mut targets: Vec<(usize, String)> = Vec::new();
    let class_end = run_while(text, at, is_target_char);
    let mut length = class_end - at;
    while length > 0 {
        if let Some(after) = literal(text, at + length, " = ") {
            targets.push((after, text[at..after].to_string()));
        }
        length -= 1;
    }
    if let Some(after) = literal(text, at, "return ") {
        targets.push((after, text[at..after].to_string()));
    }

    for (after_target, target) in targets {
        if let Some(found) = statement_body(text, start, at, &target, after_target) {
            return Some(found);
        }
    }
    None
}

fn statement_body(
    text: &str,
    start: usize,
    indent_end: usize,
    target: &str,
    at: usize,
) -> Option<StatementMatch> {
    let at = literal(text, at, "M2C_FIELD((*(s32 *)")?;
    let (at, address) = hex_literal(text, at)?;
    let at = literal(text, at, " + (")?;
    let index_end = run_while(text, at, is_js_word);
    if index_end == at {
        return None;
    }
    let index = &text[at..index_end];
    let at = literal(text, index_end, " * ")?;
    let stride_end = run_while(text, at, is_js_digit);
    if stride_end == at {
        return None;
    }
    let stride = &text[at..stride_end];
    let at = literal(text, stride_end, ")), ")?;

    for kind in KINDS {
        let Some(after_kind) = literal(text, at, kind) else {
            continue;
        };
        let Some(after_comma) = literal(text, after_kind, ", ") else {
            continue;
        };
        let Some((after_offset, offset)) = hex_literal(text, after_comma) else {
            continue;
        };
        let Some(end) = literal(text, after_offset, ");") else {
            continue;
        };
        return Some(StatementMatch {
            start,
            end,
            indent: text[start..indent_end].to_string(),
            target: target.to_string(),
            address: address.to_string(),
            index: index.to_string(),
            stride: stride.to_string(),
            kind: kind.to_string(),
            offset: offset.to_string(),
        });
    }
    None
}

/// All non-overlapping `STATEMENT` matches, left to right, as the `g` flag
/// produces them.
pub fn statement_matches(text: &str) -> Vec<StatementMatch> {
    let mut out = Vec::new();
    let mut at = 0usize;
    while at <= text.len() {
        if !text.is_char_boundary(at) {
            at += 1;
            continue;
        }
        if let Some(found) = statement_at(text, at) {
            at = found.end;
            out.push(found);
        } else {
            at += text[at..].chars().next().map_or(1, char::len_utf8);
        }
    }
    out
}

// ---------------------------------------------------------------------------
// ADDRESS
// ---------------------------------------------------------------------------

/// `0x0[238][0-9A-Fa-f]{6}` (flags: `g`). Case-sensitive on the `0x` prefix and
/// on the `[238]` selector, case-insensitive on the six digits because the
/// class spells both cases out.
pub fn address_matches(text: &str) -> Vec<&str> {
    let bytes = text.as_bytes();
    let mut out = Vec::new();
    let mut at = 0usize;
    while at + 10 <= bytes.len() {
        if bytes[at] == b'0'
            && bytes[at + 1] == b'x'
            && bytes[at + 2] == b'0'
            && matches!(bytes[at + 3], b'2' | b'3' | b'8')
            && bytes[at + 4..at + 10].iter().all(u8::is_ascii_hexdigit)
        {
            out.push(&text[at..at + 10]);
            at += 10;
        } else {
            at += 1;
        }
    }
    out
}

// ---------------------------------------------------------------------------
// `/\)\s*\{/`
// ---------------------------------------------------------------------------

/// The first `)` ... `{` opening. Returns `(start, end)` of the whole match.
pub fn opening_brace(text: &str) -> Option<(usize, usize)> {
    let mut at = 0usize;
    while at < text.len() {
        if text.as_bytes()[at] == b')' {
            let space_end = run_while(text, at + 1, is_js_space);
            let mut candidate = space_end;
            loop {
                if let Some(end) = literal(text, candidate, "{") {
                    return Some((at, end));
                }
                if candidate == at + 1 {
                    break;
                }
                candidate -= text[..candidate]
                    .chars()
                    .next_back()
                    .map_or(1, char::len_utf8);
            }
        }
        at += 1;
    }
    None
}

// ---------------------------------------------------------------------------
// The two `Func_08xxxxxx` patterns.
// ---------------------------------------------------------------------------

/// One match of `/(\w[\w* ]*)(Func_08[0-9a-f]{6})\(([^)]*)\) \{/` or of
/// `/(\w[\w* ]*)(Func_08[0-9a-f]{6})\(\);/`.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct FuncMatch {
    pub start: usize,
    pub end: usize,
    pub leading: String,
    pub name: String,
    pub parameters: String,
}

fn func_name_at(text: &str, at: usize) -> Option<usize> {
    let after = literal(text, at, "Func_08")?;
    let digits = &text.as_bytes()[after..];
    if digits.len() < 6 {
        return None;
    }
    if !digits[..6]
        .iter()
        .all(|b| b.is_ascii_digit() || (b'a'..=b'f').contains(b))
    {
        return None;
    }
    Some(after + 6)
}

fn func_match_at(text: &str, start: usize, definition: bool) -> Option<FuncMatch> {
    let first = text[start..].chars().next()?;
    if !is_js_word(first) {
        return None;
    }
    let leading_end = run_while(text, start + first.len_utf8(), is_signature_char);
    // `(\w[\w* ]*)` is greedy: the longest leading run wins, which selects the
    // LAST `Func_08xxxxxx` in a run of them, not the first.
    let mut split = leading_end;
    loop {
        if let Some(after_name) = func_name_at(text, split) {
            let name = text[split..after_name].to_string();
            if definition {
                if let Some(after_open) = literal(text, after_name, "(") {
                    let parameters_end = run_while(text, after_open, |c| c != ')');
                    if let Some(after_close) = literal(text, parameters_end, ")") {
                        if let Some(end) = literal(text, after_close, " {") {
                            return Some(FuncMatch {
                                start,
                                end,
                                leading: text[start..split].to_string(),
                                name,
                                parameters: text[after_open..parameters_end].to_string(),
                            });
                        }
                    }
                }
            } else if let Some(end) = literal(text, after_name, "();") {
                return Some(FuncMatch {
                    start,
                    end,
                    leading: text[start..split].to_string(),
                    name,
                    parameters: String::new(),
                });
            }
        }
        if split == start + first.len_utf8() {
            return None;
        }
        split -= text[..split].chars().next_back().map_or(1, char::len_utf8);
    }
}

fn first_func_match(text: &str, definition: bool) -> Option<FuncMatch> {
    let mut at = 0usize;
    while at < text.len() {
        if text.is_char_boundary(at) {
            if let Some(found) = func_match_at(text, at, definition) {
                return Some(found);
            }
        }
        at += 1;
    }
    None
}

/// `/(\w[\w* ]*)(Func_08[0-9a-f]{6})\(([^)]*)\) \{/.exec(body)`.
pub fn signature_match(text: &str) -> Option<FuncMatch> {
    first_func_match(text, true)
}

/// `/(\w[\w* ]*)(Func_08[0-9a-f]{6})\(\);/.exec(body)`.
pub fn bare_call_match(text: &str) -> Option<FuncMatch> {
    first_func_match(text, false)
}

// ---------------------------------------------------------------------------
// HARDWARE
// ---------------------------------------------------------------------------

/// Ordered as the source alternation.
const HARDWARE_TYPES: [&str; 6] = ["u8", "s8", "u16", "s16", "u32", "s32"];

/// One match of
/// `/\*\s*\((u8|s8|u16|s16|u32|s32) \*\)\s*(0x0[4E][0-9A-Fa-f]{6})/g`.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct HardwareMatch {
    pub start: usize,
    pub end: usize,
    pub kind: String,
    pub address: String,
}

fn hardware_at(text: &str, start: usize) -> Option<HardwareMatch> {
    let at = literal(text, start, "*")?;
    let at = run_while(text, at, is_js_space);
    let at = literal(text, at, "(")?;
    for kind in HARDWARE_TYPES {
        let Some(after_kind) = literal(text, at, kind) else {
            continue;
        };
        let Some(after_star) = literal(text, after_kind, " *)") else {
            continue;
        };
        let after_space = run_while(text, after_star, is_js_space);
        // PORT NOTE: `[4E]` is spelled uppercase-only in the source pattern and
        // the pattern carries no `i` flag, so `0x0e000000` is deliberately not
        // matched. Lower-casing here would be the missing-`i`-flag defect in
        // reverse.
        let Some(after_prefix) = literal(text, after_space, "0x0") else {
            continue;
        };
        let selector = text.as_bytes().get(after_prefix).copied();
        if selector != Some(b'4') && selector != Some(b'E') {
            continue;
        }
        let digits_start = after_prefix + 1;
        let digits = text.as_bytes().get(digits_start..digits_start + 6)?;
        if !digits.iter().all(u8::is_ascii_hexdigit) {
            continue;
        }
        let end = digits_start + 6;
        return Some(HardwareMatch {
            start,
            end,
            kind: kind.to_string(),
            address: text[after_space..end].to_string(),
        });
    }
    None
}

/// All non-overlapping hardware-access matches.
pub fn hardware_matches(text: &str) -> Vec<HardwareMatch> {
    let mut out = Vec::new();
    let mut at = 0usize;
    while at < text.len() {
        if text.is_char_boundary(at) {
            if let Some(found) = hardware_at(text, at) {
                at = found.end;
                out.push(found);
                continue;
            }
        }
        at += 1;
    }
    out
}

// ---------------------------------------------------------------------------
// POST_INCREMENT
// ---------------------------------------------------------------------------

/// One match of `/(\w+) = (\*\w+);\n(\s*)(\w+) \+= \d+;/g`.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct PostIncrementMatch {
    pub start: usize,
    pub end: usize,
    pub target: String,
    pub load: String,
    pub pointer: String,
}

fn post_increment_at(text: &str, start: usize) -> Option<PostIncrementMatch> {
    let target_end = run_while(text, start, is_js_word);
    if target_end == start {
        return None;
    }
    let at = literal(text, target_end, " = ")?;
    let load_start = at;
    let at = literal(text, at, "*")?;
    let load_end = run_while(text, at, is_js_word);
    if load_end == at {
        return None;
    }
    // PORT NOTE: `\n` in the pattern is a bare line feed, so a CRLF file never
    // matches here. `\s*` would happily eat the `\r`, but it comes *after* the
    // `\n`. Reproduced, not repaired.
    let at = literal(text, load_end, ";\n")?;
    let at = run_while(text, at, is_js_space);
    let pointer_end = run_while(text, at, is_js_word);
    if pointer_end == at {
        return None;
    }
    let after = literal(text, pointer_end, " += ")?;
    let digits_end = run_while(text, after, is_js_digit);
    if digits_end == after {
        return None;
    }
    let end = literal(text, digits_end, ";")?;
    Some(PostIncrementMatch {
        start,
        end,
        target: text[start..target_end].to_string(),
        load: text[load_start..load_end].to_string(),
        pointer: text[at..pointer_end].to_string(),
    })
}

/// All non-overlapping matches.
///
/// PORT NOTE: the TypeScript callback can return the whole match unchanged, but
/// the regular expression's cursor still advances past it, so a shorter match
/// nested inside a rejected one is never retried. This scan advances the same
/// way for the same reason.
pub fn post_increment_matches(text: &str) -> Vec<PostIncrementMatch> {
    let mut out = Vec::new();
    let mut at = 0usize;
    while at < text.len() {
        if text.is_char_boundary(at) {
            if let Some(found) = post_increment_at(text, at) {
                at = found.end;
                out.push(found);
                continue;
            }
        }
        at += 1;
    }
    out
}

// ---------------------------------------------------------------------------
// The assembly-row patterns.
// ---------------------------------------------------------------------------

/// `row.replace(/@.*$/, "")`, flags: none.
///
/// PORT NOTE (trap: JS `.` excludes the four line terminators, and `$` without
/// `m` anchors only at the very end of the input). A row containing a lone `\r`
/// after its `@` therefore keeps its comment, because `.*` stops at the `\r` and
/// `$` then fails. `split(';')`-style truncation at the first `@` would be
/// wrong. This reproduces the pattern.
pub fn strip_comment(row: &str) -> &str {
    let mut at = 0usize;
    while let Some(offset) = row[at..].find('@') {
        let start = at + offset;
        if !row[start..]
            .chars()
            .any(|c| matches!(c, '\n' | '\r' | '\u{2028}' | '\u{2029}'))
        {
            return &row[..start];
        }
        at = start + 1;
    }
    row
}

/// `/^ldr\s+r4,\s*\[pc,\s*#0\]$/`, flags: none.
pub fn is_veneer_load(row: &str) -> bool {
    let Some(at) = literal(row, 0, "ldr") else {
        return false;
    };
    let after = run_while(row, at, is_js_space);
    if after == at {
        return false;
    }
    let Some(at) = literal(row, after, "r4,") else {
        return false;
    };
    let at = run_while(row, at, is_js_space);
    let Some(at) = literal(row, at, "[pc,") else {
        return false;
    };
    let at = run_while(row, at, is_js_space);
    let Some(at) = literal(row, at, "#0]") else {
        return false;
    };
    at == row.len()
}

/// `/^bx\s+r4$/`, flags: none.
pub fn is_veneer_branch(row: &str) -> bool {
    let Some(at) = literal(row, 0, "bx") else {
        return false;
    };
    let after = run_while(row, at, is_js_space);
    if after == at {
        return false;
    }
    literal(row, after, "r4").is_some_and(|end| end == row.len())
}

/// `/^\.4byte\s+(0x[0-9a-f]+)$/i`, flags: `i`.
///
/// PORT NOTE (trap: missing `i` flag). This is the one pattern in the file that
/// carries `i`, so `.4BYTE 0XABC` matches and yields `"0XABC"`. Dropping the
/// flag is the exact defect class that has already been confirmed live three
/// times in this repository; the test below pins it.
pub fn asm_literal_row(row: &str) -> Option<&str> {
    if !row.to_ascii_lowercase().starts_with(".4byte") {
        return None;
    }
    let at = ".4byte".len();
    let after = run_while(row, at, is_js_space);
    if after == at {
        return None;
    }
    let lower = row[after..].to_ascii_lowercase();
    if !lower.starts_with("0x") {
        return None;
    }
    let digits_start = after + 2;
    let end = run_while(row, digits_start, |c| {
        c.is_ascii_digit() || ('a'..='f').contains(&c.to_ascii_lowercase())
    });
    if end == digits_start || end != row.len() {
        return None;
    }
    Some(&row[after..end])
}

/// `/^[0-9a-f]{8}\.s$/`, flags: none. Lowercase only, deliberately.
pub fn is_address_asm_name(name: &str) -> bool {
    let bytes = name.as_bytes();
    bytes.len() == 10
        && bytes[8] == b'.'
        && bytes[9] == b's'
        && bytes[..8]
            .iter()
            .all(|b| b.is_ascii_digit() || (b'a'..=b'f').contains(b))
}

/// `/\barg0\b/.test(text)`, flags: none.
pub fn mentions_arg0(text: &str) -> bool {
    let mut at = 0usize;
    while let Some(offset) = text[at..].find("arg0") {
        let start = at + offset;
        let end = start + 4;
        if crate::jsstring::is_word_boundary(text, start)
            && crate::jsstring::is_word_boundary(text, end)
        {
            return true;
        }
        at = start + 1;
    }
    false
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn statement_prefers_void_double_star() {
        let body = "  x = M2C_FIELD((*(s32 *)0x08001234 + (i * 4)), void **, 0x8);\n";
        let found = statement_matches(body);
        assert_eq!(found.len(), 1);
        assert_eq!(found[0].kind, "void **");
        assert_eq!(found[0].indent, "  ");
        assert_eq!(found[0].target, "x = ");
        assert_eq!(found[0].offset, "0x8");
    }

    #[test]
    fn statement_accepts_the_return_alternative() {
        let body = "    return M2C_FIELD((*(s32 *)0x0800ABcd + (n * 12)), u8 *, 0x0);";
        let found = statement_matches(body);
        assert_eq!(found.len(), 1);
        assert_eq!(found[0].target, "return ");
        assert_eq!(found[0].address, "0x0800ABcd");
        assert_eq!(found[0].stride, "12");
    }

    #[test]
    fn address_is_case_insensitive_in_its_digits_only() {
        assert_eq!(
            address_matches("a 0x08ABcd12 b 0x0812345 c 0x0100000000"),
            vec!["0x08ABcd12"]
        );
        // `0X` is not accepted: the pattern has no `i` flag.
        assert!(address_matches("0X08ABcd12").is_empty());
    }

    #[test]
    fn hardware_selector_is_case_sensitive() {
        assert_eq!(hardware_matches("*(u16 *)0x04000000").len(), 1);
        assert_eq!(hardware_matches("*(u16 *)0x0E000000").len(), 1);
        // The missing-`i`-flag behaviour, preserved on purpose.
        assert!(hardware_matches("*(u16 *)0x0e000000").is_empty());
    }

    #[test]
    fn hardware_eats_js_specific_whitespace() {
        // `\s` includes NBSP in JavaScript.
        assert_eq!(hardware_matches("*\u{00A0}(u32 *)\u{00A0}0x04000000").len(), 1);
        // ... and excludes NEL.
        assert!(hardware_matches("*\u{0085}(u32 *)0x04000000").is_empty());
    }

    #[test]
    fn signature_greediness_picks_the_last_candidate() {
        let body = "s32 Func_08001111 Func_08002222(void) {";
        let found = signature_match(body).expect("matches");
        assert_eq!(found.name, "Func_08002222");
        assert_eq!(found.leading, "s32 Func_08001111 ");
        assert_eq!(found.parameters, "void");
    }

    #[test]
    fn func_names_are_lowercase_hex_only() {
        assert!(signature_match("s32 Func_08ABCDEF(void) {").is_none());
        assert!(signature_match("s32 Func_08abcdef(void) {").is_some());
    }

    #[test]
    fn strip_comment_keeps_a_comment_containing_a_line_terminator() {
        assert_eq!(strip_comment("bx r4 @ tail"), "bx r4 ");
        assert_eq!(strip_comment("bx r4 @ ta\rail"), "bx r4 @ ta\rail");
    }

    #[test]
    fn literal_row_carries_the_i_flag() {
        assert_eq!(asm_literal_row(".4byte 0x08001235"), Some("0x08001235"));
        assert_eq!(asm_literal_row(".4BYTE 0X0800123D"), Some("0X0800123D"));
        assert_eq!(asm_literal_row(".4byte 08001235"), None);
    }

    #[test]
    fn asm_name_pattern_is_lowercase_only() {
        assert!(is_address_asm_name("080000e0.s"));
        assert!(!is_address_asm_name("080000E0.s"));
        assert!(!is_address_asm_name("080000e0.S"));
        assert!(!is_address_asm_name("80000e0.s"));
    }

    #[test]
    fn arg0_respects_word_boundaries() {
        assert!(mentions_arg0("s32 arg0, s32 b"));
        assert!(!mentions_arg0("s32 xarg0"));
        assert!(!mentions_arg0("s32 arg01"));
        assert!(mentions_arg0("(arg0)"));
    }

    #[test]
    fn post_increment_requires_a_bare_line_feed() {
        let body = "a = *p;\n  p += 4;";
        assert_eq!(post_increment_matches(body).len(), 1);
        let crlf = "a = *p;\r\n  p += 4;";
        assert!(post_increment_matches(crlf).is_empty());
    }

    #[test]
    fn opening_brace_is_leftmost() {
        assert_eq!(opening_brace("f() {\n g() {"), Some((2, 5)));
        assert_eq!(opening_brace("f()\n\t{"), Some((2, 6)));
        assert_eq!(opening_brace("no brace"), None);
    }

    #[test]
    fn veneer_rows() {
        assert!(is_veneer_load("ldr r4, [pc, #0]"));
        assert!(is_veneer_load("ldr\tr4,[pc,#0]"));
        assert!(!is_veneer_load("ldrr4, [pc, #0]"));
        assert!(!is_veneer_load("ldr r4, [pc, #0] x"));
        assert!(is_veneer_branch("bx r4"));
        assert!(!is_veneer_branch("bxr4"));
        assert!(!is_veneer_branch("bx r5"));
    }
}
