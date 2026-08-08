//! A small, general-purpose parser for the Lisp-like S-expression syntax gcc's
//! RTL dumps (`-d<letter>`, `-fdump-rtl-*`) print. This module knows nothing
//! about RTL semantics -- it only turns the text into a typed tree so consumers
//! can pattern-match structurally instead of scraping strings.
//!
//! gcc's RTL print format is close to Lisp but not quite: tags carry a
//! `/flag` and/or `:MODE` suffix glued directly onto the head atom with no
//! space (`reg/v:SI`, `plus:SI`), `[...]` brackets group PARALLEL bodies and
//! bracketed value hints (`(const_int 3 [0x3])`), and quoted strings hold
//! symbol names. `;;` lines are human-readable trace/commentary, never part
//! of the S-expression content, and are stripped before tokenizing.
//!
//! Port of `tools/lib/rtl_sexpr.ts`. Behaviour is identical, including the
//! handling of malformed input (unterminated strings, unbalanced parens,
//! stray closers).

/// A parsed S-expression node.
///
/// PORT NOTE: the TypeScript original models this as a discriminated union
/// `{ kind: "atom" } | { kind: "list"; bracket: boolean; items }`. A Rust enum
/// carries exactly the same information; `bracket` is preserved as a field so
/// `[` and `(` lists stay distinguishable and `render` round-trips.
#[derive(Debug, Clone, PartialEq, Eq)]
pub enum SExpr {
    Atom(String),
    List { bracket: bool, items: Vec<SExpr> },
}

/// Convenience constructor mirroring the TS `atom()` export.
pub fn atom(value: impl Into<String>) -> SExpr {
    SExpr::Atom(value.into())
}

/// The character class JavaScript's `\s` matches.
///
/// PORT NOTE: this is deliberately *not* `char::is_whitespace`. JS `\s` is
/// `Zs ∪ {\t \n \v \f \r     ﻿}`, whereas Rust's
/// `char::is_whitespace` is the Unicode `White_Space` property: it excludes
/// U+FEFF and includes U+0085 (NEL). Both of those can plausibly appear in a
/// dump file read with the wrong encoding, so the JS set is reproduced exactly
/// to keep tokenization byte-for-byte identical.
fn is_js_space(ch: char) -> bool {
    matches!(
        ch,
        '\t' | '\n'
            | '\u{0b}'
            | '\u{0c}'
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

fn is_delimiter(ch: char) -> bool {
    is_js_space(ch) || matches!(ch, '(' | ')' | '[' | ']')
}

/// Equivalent of JS `String.prototype.trim().startsWith(";;")`: only the
/// leading run of whitespace matters for the test.
fn trimmed_starts_with_double_semi(line: &str) -> bool {
    line.trim_start_matches(is_js_space).starts_with(";;")
}

/// Splits RTL dump text into tokens: parens, brackets, quoted strings, atoms.
///
/// `;;` comment lines are dropped first. Unterminated strings run to the end
/// of the input and are emitted without a closing quote (matching the TS,
/// which slices past the end and gets a clamped substring).
pub fn tokenize(text: &str) -> Vec<String> {
    let stripped: String = text
        .split('\n')
        .filter(|line| !trimmed_starts_with_double_semi(line))
        .collect::<Vec<_>>()
        .join("\n");

    // PORT NOTE: the TS indexes UTF-16 code units; we index `char`s. The two
    // agree for every slice this tokenizer produces, because every boundary is
    // drawn at an ASCII delimiter or quote, never inside a multi-unit
    // character.
    let chars: Vec<char> = stripped.chars().collect();
    let mut tokens: Vec<String> = Vec::new();
    let mut index = 0usize;
    while index < chars.len() {
        let ch = chars[index];
        if is_js_space(ch) {
            index += 1;
            continue;
        }
        if matches!(ch, '(' | ')' | '[' | ']') {
            tokens.push(ch.to_string());
            index += 1;
            continue;
        }
        if ch == '"' {
            let mut end = index + 1;
            // PORT NOTE: no escape processing, exactly as in the TS. A `\"`
            // inside a string terminates it; that is gcc's behaviour for the
            // names it prints, and consumers rely on the raw quoted text.
            while end < chars.len() && chars[end] != '"' {
                end += 1;
            }
            let stop = (end + 1).min(chars.len());
            tokens.push(chars[index..stop].iter().collect());
            index = end + 1;
            continue;
        }
        let mut end = index;
        while end < chars.len() && !is_delimiter(chars[end]) {
            end += 1;
        }
        tokens.push(chars[index..end].iter().collect());
        index = end;
    }
    tokens
}

/// Parses every top-level form in the token stream, tolerating an unbalanced
/// trailing fragment (a dump file can be truncated by a compiler crash or a
/// mid-write read) by returning what parsed cleanly rather than erroring.
/// Stray closing tokens at top level are skipped.
pub fn parse_sexprs(tokens: &[String]) -> Vec<SExpr> {
    fn parse_one(tokens: &[String], position: &mut usize) -> SExpr {
        let token = tokens[*position].as_str();
        if token == "(" || token == "[" {
            let bracket = token == "[";
            *position += 1;
            let close = if bracket { "]" } else { ")" };
            let mut items: Vec<SExpr> = Vec::new();
            while *position < tokens.len() && tokens[*position] != close {
                items.push(parse_one(tokens, position));
            }
            if *position < tokens.len() && tokens[*position] == close {
                *position += 1;
            }
            return SExpr::List { bracket, items };
        }
        let value = token.to_string();
        *position += 1;
        SExpr::Atom(value)
    }

    let mut position = 0usize;
    let mut results: Vec<SExpr> = Vec::new();
    while position < tokens.len() {
        if tokens[position] == ")" || tokens[position] == "]" {
            position += 1;
            continue;
        }
        results.push(parse_one(tokens, &mut position));
    }
    results
}

/// Tokenize then parse.
pub fn parse_all(text: &str) -> Vec<SExpr> {
    parse_sexprs(&tokenize(text))
}

/// The head of a list, e.g. `(plus:SI (reg ...) (reg ...))` -> `"plus:SI"`. A
/// non-list, an empty list, or a list whose first item is itself a list has no
/// head.
pub fn head(expr: &SExpr) -> Option<&str> {
    match expr {
        SExpr::Atom(_) => None,
        SExpr::List { items, .. } => match items.first() {
            Some(SExpr::Atom(value)) => Some(value.as_str()),
            _ => None,
        },
    }
}

/// The `tag`/`flags`/`mode` split of a raw head atom.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Tag {
    pub tag: String,
    pub flags: Vec<String>,
    pub mode: Option<String>,
}

/// gcc glues `/flags` and `:MODE` onto the head atom with no separator:
/// `reg/v:SI`, `insn_list:REG_DEP_ANTI`, `mem/f/c:SI`. Split it back apart.
/// The split is on the *first* `:`; everything after it is the mode.
pub fn parse_tag(raw_head: &str) -> Tag {
    let (before_mode, mode) = match raw_head.find(':') {
        Some(at) => (&raw_head[..at], Some(raw_head[at + 1..].to_string())),
        None => (raw_head, None),
    };
    let mut parts = before_mode.split('/');
    // `str::split` always yields at least one element, matching JS `split`.
    let tag = parts.next().unwrap_or("").to_string();
    let flags = parts.map(|part| part.to_string()).collect();
    Tag { tag, flags, mode }
}

/// Descends into nested single-child lists to find the first real atom --
/// handles gcc's occasional extra wrapping paren, e.g. the symbol name in
/// `(symbol_ref:SI ("Func_02001508"))` is one list deep from where a bare
/// `(symbol_ref:SI "Func_02001508")` would put it.
pub fn first_atom_deep(expr: &SExpr) -> Option<&str> {
    match expr {
        SExpr::Atom(value) => Some(value.as_str()),
        SExpr::List { items, .. } => items.iter().find_map(first_atom_deep),
    }
}

/// Renders a node back to text. Round-trips any tree this module produced,
/// modulo whitespace and stripped comments.
pub fn render(expr: &SExpr) -> String {
    match expr {
        SExpr::Atom(value) => value.clone(),
        SExpr::List { bracket, items } => {
            let (open, close) = if *bracket { ('[', ']') } else { ('(', ')') };
            let mut out = String::new();
            out.push(open);
            for (position, item) in items.iter().enumerate() {
                if position > 0 {
                    out.push(' ');
                }
                out.push_str(&render(item));
            }
            out.push(close);
            out
        }
    }
}

/// Helpers shared by the binary and the tests.
impl SExpr {
    pub fn items(&self) -> &[SExpr] {
        match self {
            SExpr::Atom(_) => &[],
            SExpr::List { items, .. } => items,
        }
    }

    pub fn is_list(&self) -> bool {
        matches!(self, SExpr::List { .. })
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    // Port of the TS `selfTest()`.
    #[test]
    fn self_test_insn_from_a_real_dump() {
        let parsed = parse_all(concat!(
            ";; a trace comment, not content\n",
            "(insn 15 10 17 (set (reg/v:SI 3 r3)\n",
            "        (plus:SI (reg:SI 3 r3)\n",
            "            (const_int 3 [0x3]))) 5 {*thumb_addsi3} (nil))\n",
        ));
        assert_eq!(parsed.len(), 1, "expected one top-level list");
        assert!(parsed[0].is_list());
        assert_eq!(head(&parsed[0]), Some("insn"));

        let pattern = &parsed[0].items()[4];
        assert_eq!(head(pattern), Some("set"));

        let dest = &pattern.items()[1];
        let dest_head = head(dest).expect("dest should have a head");
        assert_eq!(dest_head, "reg/v:SI");

        let Tag { tag, flags, mode } = parse_tag(dest_head);
        assert_eq!(tag, "reg");
        assert_eq!(flags, vec!["v".to_string()]);
        assert_eq!(mode.as_deref(), Some("SI"));
    }

    #[test]
    fn self_test_first_atom_deep_descends_extra_paren() {
        let wrapped = &parse_all("(symbol_ref:SI (\"Func_02001508\"))")[0];
        let argument = &wrapped.items()[1];
        assert_eq!(first_atom_deep(argument), Some("\"Func_02001508\""));
    }

    #[test]
    fn self_test_truncated_input_yields_partial_form() {
        let truncated = parse_all("(insn 1 0 2 (set (reg:SI 3");
        assert_eq!(truncated.len(), 1);
        assert_eq!(head(&truncated[0]), Some("insn"));
        // The unterminated inner lists are still built, just short.
        let set = &truncated[0].items()[4];
        assert_eq!(head(set), Some("set"));
        assert_eq!(render(&truncated[0]), "(insn 1 0 2 (set (reg:SI 3)))");
    }

    // --- Edge cases the TS self-test did not cover ---

    #[test]
    fn comment_lines_are_stripped_only_when_leading() {
        // A `;;` that is not at the start of the trimmed line is content.
        let tokens = tokenize("   ;; dropped\n(a ;;kept)\n");
        assert_eq!(tokens, vec!["(", "a", ";;kept", ")"]);
    }

    #[test]
    fn empty_input_parses_to_nothing() {
        assert!(tokenize("").is_empty());
        assert!(parse_all("").is_empty());
        assert!(parse_all("   \n\t\n").is_empty());
        assert!(parse_all(";; only a comment\n").is_empty());
    }

    #[test]
    fn empty_list_has_no_head_and_renders_back() {
        let parsed = parse_all("() []");
        assert_eq!(parsed.len(), 2);
        assert_eq!(head(&parsed[0]), None);
        assert_eq!(head(&parsed[1]), None);
        assert_eq!(first_atom_deep(&parsed[0]), None);
        assert_eq!(render(&parsed[0]), "()");
        assert_eq!(render(&parsed[1]), "[]");
    }

    #[test]
    fn stray_closers_are_skipped_at_top_level() {
        let parsed = parse_all(") ] (a) ) (b)");
        assert_eq!(parsed.len(), 2);
        assert_eq!(head(&parsed[0]), Some("a"));
        assert_eq!(head(&parsed[1]), Some("b"));
    }

    #[test]
    fn bare_atom_after_a_closed_list_is_its_own_top_level_form() {
        let parsed = parse_all("(a) b");
        assert_eq!(parsed.len(), 2);
        assert_eq!(parsed[1], SExpr::Atom("b".to_string()));
    }

    #[test]
    fn mismatched_bracket_kind_becomes_an_atom_inside_the_list() {
        // PORT NOTE: identical to the TS. Inside a `(`-list only `)` closes,
        // and `parse_one` has no special case for a wrong-kind closer, so `]`
        // falls through to the atom branch and the list runs to EOF.
        let parsed = parse_all("(a ] b");
        assert_eq!(parsed.len(), 1);
        assert_eq!(render(&parsed[0]), "(a ] b)");
    }

    #[test]
    fn brackets_nest_and_keep_their_kind() {
        let parsed = parse_all("(parallel [(set (reg:SI 0 r0) (const_int 1 [0x1]))])");
        assert_eq!(parsed.len(), 1);
        assert_eq!(
            render(&parsed[0]),
            "(parallel [(set (reg:SI 0 r0) (const_int 1 [0x1]))])"
        );
        let body = &parsed[0].items()[1];
        assert!(matches!(body, SExpr::List { bracket: true, .. }));
    }

    #[test]
    fn strings_keep_their_quotes_and_inner_delimiters() {
        let tokens = tokenize("(symbol_ref:SI \"a b (c) [d]\")");
        assert_eq!(
            tokens,
            vec!["(", "symbol_ref:SI", "\"a b (c) [d]\"", ")"]
        );
    }

    #[test]
    fn backslash_does_not_escape_a_quote() {
        // PORT NOTE: matches the TS exactly -- there is no escape handling, so
        // the string ends at the quote after the backslash.
        // The string token ends at the quote following the backslash; the
        // trailing `b"` is then scanned as a plain atom, because `"` is not a
        // token delimiter once we are outside a string.
        let tokens = tokenize("(\"a\\\"b\")");
        assert_eq!(tokens, vec!["(", "\"a\\\"", "b\"", ")"]);
    }

    #[test]
    fn unterminated_string_runs_to_end_of_input() {
        let tokens = tokenize("(symbol_ref:SI \"Func_0800");
        assert_eq!(tokens, vec!["(", "symbol_ref:SI", "\"Func_0800"]);
        let parsed = parse_all("(symbol_ref:SI \"Func_0800");
        assert_eq!(parsed.len(), 1);
        assert_eq!(first_atom_deep(&parsed[0]), Some("symbol_ref:SI"));
    }

    #[test]
    fn empty_string_token() {
        assert_eq!(tokenize("\"\""), vec!["\"\""]);
    }

    #[test]
    fn parse_tag_splits_on_the_first_colon_only() {
        assert_eq!(
            parse_tag("mem/f/c:SI"),
            Tag {
                tag: "mem".into(),
                flags: vec!["f".into(), "c".into()],
                mode: Some("SI".into()),
            }
        );
        assert_eq!(
            parse_tag("insn_list:REG_DEP_ANTI"),
            Tag {
                tag: "insn_list".into(),
                flags: vec![],
                mode: Some("REG_DEP_ANTI".into()),
            }
        );
        assert_eq!(
            parse_tag("plus"),
            Tag {
                tag: "plus".into(),
                flags: vec![],
                mode: None,
            }
        );
        // Trailing colon yields an empty (but present) mode, as in JS.
        assert_eq!(parse_tag("reg:").mode.as_deref(), Some(""));
        // A colon inside the mode stays in the mode.
        assert_eq!(parse_tag("a:b:c").mode.as_deref(), Some("b:c"));
        // Leading slash gives an empty tag and one flag, as in JS.
        assert_eq!(parse_tag("/v").tag, "");
        assert_eq!(parse_tag("/v").flags, vec!["v".to_string()]);
        // Empty input: JS `"".split("/")` is `[""]`.
        assert_eq!(
            parse_tag(""),
            Tag {
                tag: String::new(),
                flags: vec![],
                mode: None,
            }
        );
    }

    #[test]
    fn head_of_atom_and_of_list_headed_list_is_none() {
        assert_eq!(head(&atom("x")), None);
        let parsed = parse_all("((a) b)");
        assert_eq!(head(&parsed[0]), None);
        // ...but first_atom_deep still finds the atom.
        assert_eq!(first_atom_deep(&parsed[0]), Some("a"));
    }

    #[test]
    fn first_atom_deep_skips_empty_lists() {
        let parsed = parse_all("(() [] (( x )))");
        assert_eq!(first_atom_deep(&parsed[0]), Some("x"));
    }

    #[test]
    fn atoms_are_split_on_delimiters_without_whitespace() {
        assert_eq!(tokenize("(a)(b)"), vec!["(", "a", ")", "(", "b", ")"]);
        assert_eq!(tokenize("3[0x3]"), vec!["3", "[", "0x3", "]"]);
    }

    #[test]
    fn non_ascii_whitespace_separates_atoms() {
        // U+00A0 and U+FEFF are both in JS `\s`.
        assert_eq!(tokenize("a\u{a0}b\u{feff}c"), vec!["a", "b", "c"]);
    }

    #[test]
    fn render_round_trips_a_multiline_insn() {
        let text = "(insn 15 10 17 (set (reg/v:SI 3 r3)\n        (plus:SI (reg:SI 3 r3)\n            (const_int 3 [0x3]))) 5 {*thumb_addsi3} (nil))";
        let parsed = parse_all(text);
        let rendered = render(&parsed[0]);
        assert_eq!(parse_all(&rendered), parsed, "render output must re-parse to the same tree");
        assert!(rendered.contains("{*thumb_addsi3}"));
    }

    #[test]
    fn real_dump_files_parse_into_top_level_forms() {
        let root = std::path::Path::new(env!("CARGO_MANIFEST_DIR"))
            .parent()
            .unwrap()
            .parent()
            .unwrap();
        let dump = root.join("work/rtl/08006088.c.18.greg");
        let Ok(text) = std::fs::read_to_string(&dump) else {
            // The dump is a work artifact; skip rather than fail if absent.
            return;
        };
        let parsed = parse_all(&text);
        assert!(parsed.len() > 5, "expected many top-level forms, got {}", parsed.len());
        let insn_count = parsed
            .iter()
            .filter(|expr| matches!(head(expr), Some("insn") | Some("jump_insn") | Some("call_insn")))
            .count();
        assert!(insn_count > 0, "expected at least one insn form");
    }
}
