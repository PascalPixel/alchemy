//! The source no rewrite touches: what a `FAKEMATCH` tag covers and every
//! statement that holds asm. A tag on its own line covers the statement after
//! it, or the whole block when it is the block's first line; a tag after code
//! on the same line covers that statement, or the block that line opens.

use crate::permute::lex::{lex, Tok, Token};
use std::ops::Range;

fn punct(token: &Token) -> Option<&'static str> {
    match token.tok {
        Tok::Punct(p) => Some(p),
        _ => None,
    }
}

/// The index of the bracket matching the one at `at`, either way.
fn matching(tokens: &[Token], at: usize) -> Option<usize> {
    let forward = matches!(punct(&tokens[at]), Some("(" | "[" | "{"));
    let mut depth = 0i32;
    let mut k = at;
    loop {
        match punct(&tokens[k]) {
            Some("(" | "[" | "{") => depth += if forward { 1 } else { -1 },
            Some(")" | "]" | "}") => depth += if forward { -1 } else { 1 },
            _ => {}
        }
        if depth == 0 {
            return Some(k);
        }
        if forward {
            k += 1;
            if k >= tokens.len() {
                return None;
            }
        } else {
            k = k.checked_sub(1)?;
        }
    }
}

/// The first token of the statement holding `at`.
fn statement_start(tokens: &[Token], at: usize) -> usize {
    let mut k = at;
    if matches!(punct(&tokens[k]), Some("}" | ")" | "]")) {
        k = matching(tokens, k).unwrap_or(k);
    }
    let mut depth = 0i32;
    while k > 0 {
        let before = &tokens[k - 1];
        match punct(before) {
            Some(")" | "]") => depth += 1,
            Some("(" | "[") => depth -= 1,
            Some(";" | "{" | "}") if depth <= 0 => return k,
            _ => {}
        }
        k -= 1;
    }
    0
}

/// The last token of the statement starting at `at`, with its blocks, `else`
/// branches and a `do` loop's `while`.
fn statement_end(tokens: &[Token], at: usize) -> usize {
    let looping = matches!(&tokens[at].tok, Tok::Ident(n) if n == "do");
    let mut depth = 0i32;
    let mut k = at;
    while k < tokens.len() {
        match punct(&tokens[k]) {
            Some("(" | "[") => depth += 1,
            Some(")" | "]") => depth -= 1,
            Some(";") if depth <= 0 => return k,
            Some("}") if depth <= 0 => return k.saturating_sub(1).max(at),
            Some("{") if depth <= 0 => {
                let Some(close) = matching(tokens, k) else {
                    return tokens.len() - 1;
                };
                match tokens.get(close + 1) {
                    Some(t) if matches!(&t.tok, Tok::Ident(n) if n == "else") => k = close,
                    Some(t) if punct(t) == Some(";") => return close + 1,
                    _ if looping => k = close,
                    _ => return close,
                }
            }
            _ => {}
        }
        k += 1;
    }
    tokens.len().saturating_sub(1)
}

/// The byte ranges no rewrite may change.
pub(crate) fn ranges(text: &str) -> Vec<Range<usize>> {
    let Ok(all) = lex(text) else {
        return vec![0..text.len()];
    };
    let mut tags = Vec::new();
    let mut code: Vec<Token> = Vec::with_capacity(all.len());
    for token in all {
        match &token.tok {
            Tok::Comment(body) if body.contains("FAKEMATCH") => tags.push((token.start, code.len())),
            Tok::Comment(_) => {}
            _ => code.push(token),
        }
    }
    let mut out = Vec::new();
    let mut cover = |from: usize, to: usize| out.push(code[from].start..code[to].end);
    for (at, next) in tags {
        let before = next.checked_sub(1);
        let same_line = before.is_some_and(|k| !text[code[k].end..at].contains('\n'));
        match before {
            Some(k) if punct(&code[k]) == Some("{") => {
                let from = statement_start(&code, k);
                let to = if same_line { statement_end(&code, from) } else { matching(&code, k).unwrap_or(code.len() - 1) };
                cover(from.min(k), to);
            }
            Some(k) if same_line => cover(statement_start(&code, k), k),
            _ if next < code.len() => cover(next, statement_end(&code, next)),
            _ => {}
        }
    }
    for k in 0..code.len() {
        if matches!(&code[k].tok, Tok::Ident(n) if matches!(n.as_str(), "asm" | "__asm" | "__asm__")) {
            let from = statement_start(&code, k);
            cover(from, statement_end(&code, from));
        }
    }
    out
}

/// Whether `start..end` touches any of `ranges`.
pub(crate) fn covered(ranges: &[Range<usize>], start: usize, end: usize) -> bool {
    ranges.iter().any(|r| r.start < end.max(start + 1) && start < r.end)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn spans(text: &str) -> Vec<&str> {
        ranges(text).into_iter().map(|r| &text[r]).collect()
    }

    #[test]
    fn a_leading_tag_covers_the_next_statement() {
        let text = "{\n    a = 1;\n    /* FAKEMATCH: order */\n    b = (u8)c;\n    d = 2;\n}\n";
        assert_eq!(spans(text), vec!["b = (u8)c;"]);
        let loop_ = "{\n    x;\n    /* FAKEMATCH: one pass */\n    do {\n        y = 0x10;\n    } while (0);\n    z;\n}\n";
        assert_eq!(spans(loop_), vec!["do {\n        y = 0x10;\n    } while (0);"]);
    }

    #[test]
    fn a_trailing_tag_covers_its_statement() {
        let text = "{\n    a = 1;\n    b = c +\n        0x10; /* FAKEMATCH: r4 */\n    d = 2;\n}\n";
        assert_eq!(spans(text), vec!["b = c +\n        0x10;"]);
    }

    #[test]
    fn a_tag_that_opens_a_block_covers_the_block() {
        let first = "void F(void)\n{\n    /* FAKEMATCH: frame */\n    u8 a;\n    a = 0x10;\n}\nvoid G(void)\n{\n    b = 0x10;\n}\n";
        assert_eq!(spans(first), vec!["void F(void)\n{\n    /* FAKEMATCH: frame */\n    u8 a;\n    a = 0x10;\n}"]);
        let opens = "{\n    if (a) { /* FAKEMATCH: order */\n        b = 0x10;\n    } else {\n        c = 1;\n    }\n    d;\n}\n";
        assert_eq!(spans(opens), vec!["if (a) { /* FAKEMATCH: order */\n        b = 0x10;\n    } else {\n        c = 1;\n    }"]);
    }

    #[test]
    fn statements_holding_asm_are_covered() {
        let text = "{\n    register u32 r asm(\"r4\") = (u32)x;\n    asm(\"\" : : \"r\"(r));\n    y = (u8)z;\n}\n";
        assert_eq!(spans(text), vec!["register u32 r asm(\"r4\") = (u32)x;", "asm(\"\" : : \"r\"(r));"]);
    }
}
