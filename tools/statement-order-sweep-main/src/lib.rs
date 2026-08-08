// Statement-order sweep: pure analysis half.
//
// Port of tools/compiler/statement_order_sweep_main.ts. See src/main.rs for the
// driver and for the PORT NOTEs covering deliberate behavioural differences.
//
// Every string index in this module is a *character* index over a `&[char]`,
// mirroring the TypeScript original's UTF-16 code-unit indices. The two agree on
// every code point below U+10000 and, more importantly, agree with each other
// end to end: the indices produced by `function_body` are only ever fed back
// into slices of the same character vector, so the rendered text is identical
// even where a source file contains non-ASCII (these drafts carry Japanese
// comments). Rust byte indices would NOT be interchangeable with the TS ones,
// which is why `&[char]` is used throughout rather than `&str`.

use std::collections::HashSet;

/// One depth-1 statement of a function body.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Piece {
    pub text: String,
    pub simple: bool,
}

/// JS `\s` (and, identically, the set trimmed by `String.prototype.trim`):
/// Unicode space separators plus the ASCII whitespace controls, BOM, and the
/// two line separators. Rust's `char::is_whitespace` differs (it excludes
/// U+FEFF and includes U+0085), so it cannot be substituted.
pub fn js_is_space(c: char) -> bool {
    matches!(
        c,
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

/// JS `\w` / the character class `[A-Za-z0-9_]`.
fn is_word(c: char) -> bool {
    c.is_ascii_alphanumeric() || c == '_'
}

fn is_ident_start(c: char) -> bool {
    c.is_ascii_alphabetic() || c == '_'
}

fn skip_space(text: &[char], mut at: usize) -> usize {
    while at < text.len() && js_is_space(text[at]) {
        at += 1;
    }
    at
}

/// JS `String.prototype.trim`.
pub fn js_trim(text: &str) -> String {
    let characters: Vec<char> = text.chars().collect();
    let mut start = 0;
    while start < characters.len() && js_is_space(characters[start]) {
        start += 1;
    }
    let mut end = characters.len();
    while end > start && js_is_space(characters[end - 1]) {
        end -= 1;
    }
    characters[start..end].iter().collect()
}

fn matches_literal(text: &[char], at: usize, literal: &str) -> bool {
    for (cursor, wanted) in (at..).zip(literal.chars()) {
        if cursor >= text.len() || text[cursor] != wanted {
            return false;
        }
    }
    true
}

// 関数本体を切り出す。最初の `Func_` 定義の `{` から対応する `}` まで。
//
// Hand-rolled stand-in for `/\bFunc_[0-9a-f]{8}\s*\(/g`. `\b` before `F` means
// the preceding character must not be a word character.
pub fn function_body(source: &[char]) -> Option<(usize, usize)> {
    for index in 0..source.len() {
        if index > 0 && is_word(source[index - 1]) {
            continue;
        }
        if !matches_literal(source, index, "Func_") {
            continue;
        }
        let mut at = index + 5;
        let mut digits = 0;
        while digits < 8 && at < source.len() && matches!(source[at], '0'..='9' | 'a'..='f') {
            at += 1;
            digits += 1;
        }
        if digits != 8 {
            continue;
        }
        at = skip_space(source, at);
        if at >= source.len() || source[at] != '(' {
            continue;
        }

        // `source.indexOf("(", signature.index)` finds this same paren: nothing
        // between the match start and it can be one.
        let open = at;
        let mut parameter_depth: i64 = 0;
        let mut brace: i64 = -1;
        let mut cursor = open;
        while cursor < source.len() {
            if source[cursor] == '(' {
                parameter_depth += 1;
            } else if source[cursor] == ')' {
                parameter_depth -= 1;
                if parameter_depth == 0 {
                    let after = skip_space(source, cursor + 1);
                    if after < source.len() && source[after] == '{' {
                        brace = after as i64;
                    }
                    break;
                }
            }
            cursor += 1;
        }
        if brace < 0 {
            continue;
        }
        let start = brace as usize + 1;
        let mut body_depth = 1;
        let mut scan = start;
        while scan < source.len() {
            if source[scan] == '{' {
                body_depth += 1;
            } else if source[scan] == '}' {
                body_depth -= 1;
                if body_depth == 0 {
                    return Some((start, scan));
                }
            }
            scan += 1;
        }
    }
    None
}

// 本体を深さ1の「文」に割る。ブロックや制御構文は一つの塊として扱う。
pub fn top_level_statements(body: &str) -> Vec<Piece> {
    let mut pieces: Vec<Piece> = Vec::new();
    let mut depth = 0i64;
    let mut current = String::new();
    for character in body.chars() {
        current.push(character);
        if character == '{' {
            depth += 1;
        } else if character == '}' {
            depth -= 1;
            if depth == 0 {
                pieces.push(Piece { text: std::mem::take(&mut current), simple: false });
            }
        } else if character == ';' && depth == 0 {
            pieces.push(Piece { text: std::mem::take(&mut current), simple: true });
        }
    }
    // Preserve trailing whitespace before the function's closing brace. Dropping
    // it made an otherwise valid reordered source end in the needlessly ugly `}}`.
    if !current.is_empty() {
        pieces.push(Piece { text: current, simple: false });
    }
    pieces
}

const KEYWORDS: &[&str] = &[
    "if", "else", "for", "while", "do", "switch", "case", "default", "return", "goto", "break",
    "continue", "sizeof", "struct", "union", "enum", "typedef", "const", "volatile", "signed",
    "unsigned", "int", "char", "short", "long", "void", "float", "double", "s8", "u8", "s16",
    "u16", "s32", "u32", "s64", "u64",
];

fn is_keyword(name: &str) -> bool {
    KEYWORDS.contains(&name)
}

/// `/[A-Za-z_][A-Za-z0-9_]*/g` minus the C keywords.
fn identifiers(text: &[char]) -> Vec<String> {
    let mut found = Vec::new();
    let mut at = 0;
    while at < text.len() {
        if is_ident_start(text[at]) {
            let start = at;
            while at < text.len() && is_word(text[at]) {
                at += 1;
            }
            let name: String = text[start..at].iter().collect();
            if !is_keyword(&name) {
                found.push(name);
            }
        } else {
            at += 1;
        }
    }
    found
}

/// `(?:\[[^\]]*\]|->[A-Za-z0-9_]+|\.[A-Za-z0-9_]+)` at `at`.
fn assignment_suffix(text: &[char], at: usize) -> Option<usize> {
    if at >= text.len() {
        return None;
    }
    if text[at] == '[' {
        let mut cursor = at + 1;
        while cursor < text.len() && text[cursor] != ']' {
            cursor += 1;
        }
        return if cursor < text.len() { Some(cursor + 1) } else { None };
    }
    if text[at] == '-' && at + 1 < text.len() && text[at + 1] == '>' {
        let mut cursor = at + 2;
        while cursor < text.len() && is_word(text[cursor]) {
            cursor += 1;
        }
        return if cursor > at + 2 { Some(cursor) } else { None };
    }
    if text[at] == '.' {
        let mut cursor = at + 1;
        while cursor < text.len() && is_word(text[cursor]) {
            cursor += 1;
        }
        return if cursor > at + 1 { Some(cursor) } else { None };
    }
    None
}

/// `(?:=[^=]|\+=|-=|\*=|\/=|%=|&=|\|=|\^=|<<=|>>=|\+\+|--)` at `at`.
fn assignment_operator(text: &[char], at: usize) -> bool {
    if at >= text.len() {
        return false;
    }
    if text[at] == '=' {
        // `[^=]` requires a character to be present, and it may be any
        // character other than `=` (including a newline).
        return at + 1 < text.len() && text[at + 1] != '=';
    }
    for pair in ["+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "++", "--"] {
        if matches_literal(text, at, pair) {
            return true;
        }
    }
    matches_literal(text, at, "<<=") || matches_literal(text, at, ">>=")
}

/// Leftmost match of the compound-assignment regex; returns the captured name.
fn assignment_target(text: &[char]) -> Option<String> {
    for start in 0..text.len() {
        if !is_ident_start(text[start]) {
            continue;
        }
        let mut end = start + 1;
        while end < text.len() && is_word(text[end]) {
            end += 1;
        }
        let name: String = text[start..end].iter().collect();
        // `\s*` then a greedy repetition of suffixes, then `\s*` then the
        // operator. The repetition needs real backtracking, so record the
        // position after every repetition count and try them longest-first.
        let mut stops = vec![skip_space(text, end)];
        while let Some(next) = assignment_suffix(text, *stops.last().unwrap()) {
            stops.push(next);
        }
        for stop in stops.iter().rev() {
            if assignment_operator(text, skip_space(text, *stop)) {
                return Some(name);
            }
        }
    }
    None
}

/// `^\s*(?:const\s+|volatile\s+)*(?:[A-Za-z_][A-Za-z0-9_]*)\s+\**\s*([A-Za-z_][A-Za-z0-9_]*)\s*=`
fn initialised_declaration_target(text: &[char]) -> Option<String> {
    let mut stops = vec![skip_space(text, 0)];
    loop {
        let at = *stops.last().unwrap();
        let mut next = None;
        for qualifier in ["const", "volatile"] {
            if matches_literal(text, at, qualifier) {
                let after = at + qualifier.chars().count();
                let spaced = skip_space(text, after);
                if spaced > after {
                    next = Some(spaced);
                }
                break;
            }
        }
        match next {
            Some(position) => stops.push(position),
            None => break,
        }
    }
    for stop in stops.iter().rev() {
        let at = *stop;
        if at >= text.len() || !is_ident_start(text[at]) {
            continue;
        }
        let mut cursor = at + 1;
        while cursor < text.len() && is_word(text[cursor]) {
            cursor += 1;
        }
        let spaced = skip_space(text, cursor);
        if spaced == cursor {
            continue; // `\s+` needs at least one space.
        }
        cursor = spaced;
        while cursor < text.len() && text[cursor] == '*' {
            cursor += 1;
        }
        cursor = skip_space(text, cursor);
        if cursor >= text.len() || !is_ident_start(text[cursor]) {
            continue;
        }
        let name_start = cursor;
        cursor += 1;
        while cursor < text.len() && is_word(text[cursor]) {
            cursor += 1;
        }
        let name: String = text[name_start..cursor].iter().collect();
        cursor = skip_space(text, cursor);
        if cursor < text.len() && text[cursor] == '=' {
            return Some(name);
        }
    }
    None
}

// 代入対象と参照名をおおまかに拾う。判定は必ず安全側に倒す。
pub fn writes_of(text: &str) -> HashSet<String> {
    let characters: Vec<char> = text.chars().collect();
    let mut written = HashSet::new();
    if let Some(name) = assignment_target(&characters) {
        if !is_keyword(&name) {
            written.insert(name);
        }
    }
    if let Some(name) = initialised_declaration_target(&characters) {
        written.insert(name);
    }
    written
}

fn touches_memory_or_calls(text: &str) -> bool {
    if is_plain_declaration(text) {
        return false;
    }
    let characters: Vec<char> = text.chars().collect();
    // `text.replace(/^\s*[A-Za-z_][A-Za-z0-9_]*\s*=/, "")`
    let mut stripped = characters.as_slice();
    let at = skip_space(&characters, 0);
    if at < characters.len() && is_ident_start(characters[at]) {
        let mut cursor = at + 1;
        while cursor < characters.len() && is_word(characters[cursor]) {
            cursor += 1;
        }
        cursor = skip_space(&characters, cursor);
        if cursor < characters.len() && characters[cursor] == '=' {
            stripped = &characters[cursor + 1..];
        }
    }
    // `/\*|\[|->|\w\s*\(/`
    for at in 0..stripped.len() {
        let c = stripped[at];
        if c == '*' || c == '[' {
            return true;
        }
        if c == '-' && at + 1 < stripped.len() && stripped[at + 1] == '>' {
            return true;
        }
        if is_word(c) {
            let after = skip_space(stripped, at + 1);
            if after < stripped.len() && stripped[after] == '(' {
                return true;
            }
        }
    }
    false
}

/// `(?:\s*\[[^\]]+\])?\s*;$` and everything before it, i.e. the closing tail of
/// the plain-declaration pattern, starting after the declared name.
fn plain_declaration_tail(text: &[char], at: usize) -> bool {
    // The optional array suffix is greedy, so try it first.
    let spaced = skip_space(text, at);
    if spaced < text.len() && text[spaced] == '[' {
        let mut cursor = spaced + 1;
        while cursor < text.len() && text[cursor] != ']' {
            cursor += 1;
        }
        if cursor < text.len() && cursor > spaced + 1 {
            let end = skip_space(text, cursor + 1);
            if end + 1 == text.len() && text[end] == ';' {
                return true;
            }
        }
    }
    let end = skip_space(text, at);
    end + 1 == text.len() && text[end] == ';'
}

fn plain_declaration_after_qualifiers(text: &[char], at: usize) -> bool {
    // `(?:(?:struct|union|enum)\s+[A-Za-z_][A-Za-z0-9_]*|[A-Za-z_][A-Za-z0-9_]*)`
    let mut type_ends: Vec<usize> = Vec::new();
    for tag in ["struct", "union", "enum"] {
        if matches_literal(text, at, tag) {
            let after = at + tag.chars().count();
            let spaced = skip_space(text, after);
            if spaced > after && spaced < text.len() && is_ident_start(text[spaced]) {
                let mut cursor = spaced + 1;
                while cursor < text.len() && is_word(text[cursor]) {
                    cursor += 1;
                }
                type_ends.push(cursor);
            }
            break;
        }
    }
    if at < text.len() && is_ident_start(text[at]) {
        let mut cursor = at + 1;
        while cursor < text.len() && is_word(text[cursor]) {
            cursor += 1;
        }
        type_ends.push(cursor);
    }
    for type_end in type_ends {
        let spaced = skip_space(text, type_end);
        if spaced == type_end {
            continue; // `\s+`
        }
        // `(?:\*+\s*)?` is greedy: prefer taking it.
        let mut starts = Vec::new();
        if spaced < text.len() && text[spaced] == '*' {
            let mut cursor = spaced;
            while cursor < text.len() && text[cursor] == '*' {
                cursor += 1;
            }
            starts.push(skip_space(text, cursor));
        }
        starts.push(spaced);
        for start in starts {
            if start >= text.len() || !is_ident_start(text[start]) {
                continue;
            }
            let mut cursor = start + 1;
            while cursor < text.len() && is_word(text[cursor]) {
                cursor += 1;
            }
            if plain_declaration_tail(text, cursor) {
                return true;
            }
        }
    }
    false
}

// An uninitialised, fixed-size local declaration has no run-time effect. Reject
// variable-length arrays and anything with an initializer; those can evaluate
// expressions and must retain their source position.
pub fn is_plain_declaration(text: &str) -> bool {
    let declaration = js_trim(text);
    if declaration.contains('=') || !declaration.ends_with(';') {
        return false;
    }
    let characters: Vec<char> = declaration.chars().collect();
    // `/\[[^\]]*[A-Za-z_][A-Za-z0-9_]*[^\]]*\]/`: a bracketed span, containing
    // no `]`, that holds at least one identifier character.
    for at in 0..characters.len() {
        if characters[at] != '[' {
            continue;
        }
        let mut cursor = at + 1;
        let mut named = false;
        while cursor < characters.len() && characters[cursor] != ']' {
            if is_ident_start(characters[cursor]) {
                named = true;
            }
            cursor += 1;
        }
        if cursor < characters.len() && named {
            return false;
        }
    }
    // `^(?:(?:register|const|volatile|signed|unsigned|short|long)\s+)*` then the
    // rest; the qualifier repetition backtracks, so try every count.
    let mut stops = vec![0usize];
    loop {
        let at = *stops.last().unwrap();
        let mut next = None;
        for qualifier in ["register", "const", "volatile", "signed", "unsigned", "short", "long"] {
            if matches_literal(&characters, at, qualifier) {
                let after = at + qualifier.chars().count();
                let spaced = skip_space(&characters, after);
                if spaced > after {
                    next = Some(spaced);
                }
                break;
            }
        }
        match next {
            Some(position) => stops.push(position),
            None => break,
        }
    }
    stops
        .iter()
        .rev()
        .any(|stop| plain_declaration_after_qualifiers(&characters, *stop))
}

pub fn independent(a: &str, b: &str) -> bool {
    let writes_a = writes_of(a);
    let writes_b = writes_of(b);
    let reads_a: HashSet<String> = identifiers(&a.chars().collect::<Vec<char>>()).into_iter().collect();
    let reads_b: HashSet<String> = identifiers(&b.chars().collect::<Vec<char>>()).into_iter().collect();
    for name in &writes_a {
        if reads_b.contains(name) {
            return false;
        }
    }
    for name in &writes_b {
        if reads_a.contains(name) {
            return false;
        }
    }
    // 両方が記憶域を触るなら順序を保つ。別名かどうかを源から証明できない。
    if touches_memory_or_calls(a) && touches_memory_or_calls(b) {
        return false;
    }
    true
}

// 相互に独立な単純文の極大連続列。
//
// `to` is an `f64` because the TypeScript computes it as `Math.min(end, index +
// maxRun - 1)` with a `Number`-parsed `--max-run`; a non-integral or NaN value
// there is observable, so it is preserved rather than rounded away here.
pub fn independent_runs(pieces: &[Piece], max_run: f64) -> Vec<(usize, f64)> {
    let mut runs: Vec<(usize, f64)> = Vec::new();
    let mut index = 0usize;
    while index < pieces.len() {
        if !pieces[index].simple {
            index += 1;
            continue;
        }
        let mut end = index;
        while end + 1 < pieces.len() && pieces[end + 1].simple {
            let mut mutual = true;
            for earlier in index..=end {
                if !independent(&pieces[earlier].text, &pieces[end + 1].text) {
                    mutual = false;
                    break;
                }
            }
            if !mutual {
                break;
            }
            end += 1;
        }
        if end - index + 1 >= 2 {
            let limit = index as f64 + max_run - 1.0;
            let capped = if limit.is_nan() { f64::NAN } else { (end as f64).min(limit) };
            runs.push((index, capped));
        }
        index = end + 1;
    }
    runs
}

/// The TypeScript generator's yield order: for each index in turn, that element
/// followed by every permutation of the rest.
pub fn permutations(items: &[usize]) -> Vec<Vec<usize>> {
    if items.len() <= 1 {
        return vec![items.to_vec()];
    }
    let mut out = Vec::new();
    for index in 0..items.len() {
        let mut rest = items[..index].to_vec();
        rest.extend_from_slice(&items[index + 1..]);
        for tail in permutations(&rest) {
            let mut ordering = vec![items[index]];
            ordering.extend(tail);
            out.push(ordering);
        }
    }
    out
}

#[cfg(test)]
mod tests {
    use super::*;

    fn body_of(source: &str) -> Option<(String, usize, usize)> {
        let characters: Vec<char> = source.chars().collect();
        function_body(&characters)
            .map(|(start, end)| (characters[start..end].iter().collect(), start, end))
    }

    // --- the TypeScript selfTest(), transcribed ---

    #[test]
    fn self_test_accepts_plain_declarations() {
        for declaration in [
            "s16 *cursor;",
            "volatile s32 *cursor;",
            "s32 values[4];",
            "struct Position *position;",
        ] {
            assert!(is_plain_declaration(declaration), "rejected: {declaration}");
        }
    }

    #[test]
    fn self_test_rejects_effectful_statements() {
        for statement in ["s32 value = 0;", "s32 values[count];", "*cursor = value;"] {
            assert!(!is_plain_declaration(statement), "accepted: {statement}");
        }
    }

    #[test]
    fn self_test_preserves_trailing_whitespace() {
        let pieces = top_level_statements("\n    s16 *cursor;\n    volatile s32 *other;\n");
        assert_eq!(pieces.len(), 3);
        assert!(pieces[0].simple);
        assert!(pieces[1].simple);
        assert_eq!(pieces[2].text, "\n");
    }

    #[test]
    fn self_test_exposes_pointer_declarations() {
        let pieces = top_level_statements("\n    s16 *cursor;\n    volatile s32 *other;\n");
        let runs = independent_runs(&pieces, 6.0);
        assert_eq!(runs.len(), 1);
        assert_eq!(runs[0].0, 0);
        assert_eq!(runs[0].1, 1.0);
    }

    #[test]
    fn self_test_parses_pointer_to_array_parameter() {
        let source = "void Func_08000000(s32 *value, u16 (*table)[2]) {\n    s32 index;\n}\n";
        let (body, _, _) = body_of(source).expect("no body");
        assert_eq!(body, "\n    s32 index;\n");
    }

    // --- edge cases the TypeScript self-test missed ---

    #[test]
    fn signature_needs_a_word_boundary_and_exactly_eight_lowercase_hex() {
        // `\b` rejects a name glued to a preceding word character.
        assert!(body_of("void xFunc_08000000(void) {\n}\n").is_none());
        // Nine hex digits: `{8}` cannot backtrack, so `\s*\(` fails.
        assert!(body_of("void Func_080000000(void) {\n}\n").is_none());
        // Seven, and uppercase hex, both fail too.
        assert!(body_of("void Func_0800000(void) {\n}\n").is_none());
        assert!(body_of("void Func_0800000A(void) {\n}\n").is_none());
        // A non-word character before the name is fine.
        assert!(body_of("void *Func_08000000(void) {\n}\n").is_some());
    }

    #[test]
    fn signature_whitespace_uses_js_semantics_not_rust() {
        // U+00A0 is JS `\s` but is not matched by Rust's `char::is_whitespace`
        // for `\s`-equivalence purposes in some ports; both gaps here are JS
        // whitespace and must be skipped.
        let source = "void Func_08000000\u{a0}(void)\u{2028}{\n    s32 index;\n}\n";
        let (body, _, _) = body_of(source).expect("no body");
        assert_eq!(body, "\n    s32 index;\n");
        // U+0085 (NEL) is Rust whitespace but NOT JS `\s`; it must not be skipped.
        assert!(body_of("void Func_08000000\u{85}(void) {\n}\n").is_none());
    }

    #[test]
    fn body_indices_are_character_indices_over_the_whole_source() {
        // A multi-byte comment ahead of the function: byte offsets would differ
        // from the TypeScript's UTF-16 offsets, character offsets do not.
        let source = "// 関数本体を切り出す\nvoid Func_08000000(void) {\n    s32 index;\n}\n";
        let characters: Vec<char> = source.chars().collect();
        let (start, end) = function_body(&characters).expect("no body");
        assert_eq!(characters[start..end].iter().collect::<String>(), "\n    s32 index;\n");
        // Rendering through the same character vector round-trips exactly.
        let rendered: String = characters[..start]
            .iter()
            .chain(characters[start..end].iter())
            .chain(characters[end..].iter())
            .collect();
        assert_eq!(rendered, source);
        assert_ne!(start, source.find('{').unwrap() + 1); // bytes != characters here
    }

    #[test]
    fn body_takes_the_first_function_with_a_brace() {
        // A prototype has no `{`; the scan must fall through to the definition.
        let source = "void Func_08000000(void);\nvoid Func_08000004(void) {\n    s32 a;\n}\n";
        let (body, _, _) = body_of(source).expect("no body");
        assert_eq!(body, "\n    s32 a;\n");
    }

    #[test]
    fn top_level_statements_treats_blocks_as_one_piece() {
        let pieces = top_level_statements(" a = 1; if (a) { b = 2; c = 3; } d = 4;");
        assert_eq!(pieces.len(), 3);
        assert_eq!(pieces[0].text, " a = 1;");
        assert!(pieces[0].simple);
        assert_eq!(pieces[1].text, " if (a) { b = 2; c = 3; }");
        assert!(!pieces[1].simple);
        assert_eq!(pieces[2].text, " d = 4;");
        assert!(pieces[2].simple);
        // The trailing empty remainder is dropped, not pushed.
        assert_eq!(top_level_statements("a;").len(), 1);
        // A trailing non-empty remainder is kept and marked non-simple.
        let tail = top_level_statements("a;\n  ");
        assert_eq!(tail.len(), 2);
        assert_eq!(tail[1].text, "\n  ");
        assert!(!tail[1].simple);
    }

    #[test]
    fn writes_of_finds_compound_and_declaration_targets() {
        assert_eq!(writes_of("a = 1;"), HashSet::from(["a".to_string()]));
        assert_eq!(writes_of("a += 1;"), HashSet::from(["a".to_string()]));
        assert_eq!(writes_of("a++;"), HashSet::from(["a".to_string()]));
        assert_eq!(writes_of("a[i].b->c = 1;"), HashSet::from(["a".to_string()]));
        assert_eq!(writes_of("a <<= 1;"), HashSet::from(["a".to_string()]));
        // `=[^=]` must not fire on a comparison.
        assert!(writes_of("if (a == b) return;").is_empty());
        // Keyword targets are dropped.
        assert!(writes_of("return = x;").is_empty());
        // The declaration branch backtracks past a leading qualifier.
        assert_eq!(writes_of("const s16 value = 1;"), HashSet::from(["value".to_string()]));
        assert_eq!(writes_of("s16 *cursor = 0;"), HashSet::from(["cursor".to_string()]));
        // Both branches can contribute, and they agree here.
        assert_eq!(writes_of("s32 total = 0;"), HashSet::from(["total".to_string()]));
    }

    #[test]
    fn writes_of_needs_a_character_after_the_bare_equals() {
        // `=[^=]` requires a following character: a trailing `=` matches nothing.
        assert!(writes_of("a =").is_empty());
        assert_eq!(writes_of("a =b"), HashSet::from(["a".to_string()]));
    }

    #[test]
    fn plain_declaration_rejects_variable_length_arrays_and_initialisers() {
        assert!(is_plain_declaration("register unsigned long counter;"));
        assert!(is_plain_declaration("  u16 table[16];  "));
        // A hex subscript is rejected: `0x10` contains `x`, which the
        // variable-length-array guard reads as an identifier. Faithful to the
        // TypeScript, which behaves the same way.
        assert!(!is_plain_declaration("u16 table[0x10];"));
        assert!(is_plain_declaration("struct Position position;"));
        assert!(is_plain_declaration("s32 **handle;"));
        assert!(is_plain_declaration("s32 ** handle;"));
        // `(?:\*+\s*)?` occurs once, so a space *between* stars does not parse.
        assert!(!is_plain_declaration("s32 * * handle;"));
        assert!(!is_plain_declaration("s32 values[count];"));
        assert!(!is_plain_declaration("s32 value = 0;"));
        assert!(!is_plain_declaration("s32 value"));
        assert!(!is_plain_declaration("Func_08000000();"));
        // Backtracking: a variable actually named after a qualifier prefix.
        assert!(is_plain_declaration("unsigned counter;"));
        assert!(is_plain_declaration("constant value;"));
        // An empty subscript has no `[^\]]+` to match.
        assert!(!is_plain_declaration("s32 values[];"));
    }

    #[test]
    fn plain_declaration_trims_js_whitespace() {
        assert!(is_plain_declaration("\u{feff}s16 cursor;\u{a0}"));
        // U+0085 is not JS whitespace, so it stays and breaks the anchor.
        assert!(!is_plain_declaration("\u{85}s16 cursor;"));
    }

    #[test]
    fn independence_keeps_two_memory_touching_statements_ordered() {
        assert!(!independent("*a = 1;", "*b = 2;"));
        assert!(!independent("f();", "g();"));
        // Plain declarations are not memory accesses despite the `*`.
        assert!(independent("s16 *cursor;", "volatile s32 *other;"));
        // A read/write conflict blocks the swap.
        assert!(!independent("a = 1;", "b = a;"));
        assert!(independent("a = 1;", "b = 2;"));
    }

    #[test]
    fn runs_are_capped_by_max_run_but_the_scan_advances_past_the_cap() {
        let pieces = top_level_statements("s16 *a;s16 *b;s16 *c;s16 *d;");
        let runs = independent_runs(&pieces, 2.0);
        assert_eq!(runs, vec![(0usize, 1.0f64)]);
        let runs = independent_runs(&pieces, 6.0);
        assert_eq!(runs, vec![(0usize, 3.0f64)]);
        // A single-statement run is never emitted.
        let pieces = top_level_statements("s16 *a;*a = 1;");
        assert!(independent_runs(&pieces, 6.0).is_empty());
    }

    #[test]
    fn a_nan_max_run_yields_a_nan_bound() {
        let pieces = top_level_statements("s16 *a;s16 *b;");
        let runs = independent_runs(&pieces, f64::NAN);
        assert_eq!(runs.len(), 1);
        assert!(runs[0].1.is_nan());
    }

    #[test]
    fn permutation_order_matches_the_generator() {
        assert_eq!(permutations(&[0]), vec![vec![0]]);
        assert_eq!(
            permutations(&[0, 1, 2]),
            vec![
                vec![0, 1, 2],
                vec![0, 2, 1],
                vec![1, 0, 2],
                vec![1, 2, 0],
                vec![2, 0, 1],
                vec![2, 1, 0],
            ]
        );
        assert_eq!(permutations(&[]).len(), 1);
    }
}
