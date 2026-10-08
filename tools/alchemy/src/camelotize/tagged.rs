//! The source no rewrite touches: what a `FAKEMATCH` tag covers and every
//! statement that holds asm. A tag covers the whole innermost block holding
//! it, with the statement that owns that block, or the whole function when
//! it sits in the function's top-level block (S2: a tag covers what its
//! reason names, and a reason may name several statements).

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
            // An `if` branch closes before `else`, and a `do` body before its
            // `while`: the statement goes on before that block.
            Some("}") if depth <= 0 && continues(tokens, k) => {
                k = matching(tokens, k - 1).unwrap_or(k - 1);
                continue;
            }
            Some(";") if depth <= 0 && is(&tokens[k], "else") => {}
            Some(";" | "{" | "}") if depth <= 0 => return k,
            _ => {}
        }
        k -= 1;
    }
    0
}

fn is(token: &Token, word: &str) -> bool {
    matches!(&token.tok, Tok::Ident(n) if n == word)
}

/// Whether the statement before `at` goes on past the `}` at `at - 1`: an
/// `else` follows it, or it closes a `do` body and `while` follows.
fn continues(tokens: &[Token], at: usize) -> bool {
    is(&tokens[at], "else")
        || (is(&tokens[at], "while")
            && matching(tokens, at - 1)
                .and_then(|open| open.checked_sub(1))
                .is_some_and(|before| is(&tokens[before], "do")))
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
            // A braceless branch ends at its `;`, but an `else` carries on.
            Some(";") if depth <= 0 && tokens.get(k + 1).is_some_and(|t| is(t, "else")) => {}
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

/// The innermost `{` left open before each token position: `inside[k]` is
/// the block that tokens `..k` leave open, so a tag before token `k` sits in
/// it.
fn inside(tokens: &[Token]) -> Vec<Option<usize>> {
    let mut stack: Vec<usize> = Vec::new();
    let mut out = Vec::with_capacity(tokens.len() + 1);
    out.push(None);
    for (k, token) in tokens.iter().enumerate() {
        match punct(token) {
            Some("{") => stack.push(k),
            Some("}") => {
                stack.pop();
            }
            _ => {}
        }
        out.push(stack.last().copied());
    }
    out
}

/// The byte ranges no rewrite may change. A tag's reason may name several
/// statements (PRIORITY.C's "these flag updates"), so a tag covers the whole
/// innermost block holding it, with the statement that owns that block (its
/// `if`, `else` branches and loop head); a tag in a function's top-level
/// block covers the whole function. A tag outside every block covers the
/// statement it trails, or else the one after it.
#[allow(clippy::single_range_in_vec_init)] // one range: the whole text
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
    let blocks = inside(&code);
    let mut out = Vec::new();
    let mut cover = |from: usize, to: usize| out.push(code[from].start..code[to].end);
    for (at, next) in tags {
        let last = code.len().saturating_sub(1);
        if let Some(open) = blocks[next] {
            let close = matching(&code, open).unwrap_or(last);
            let from = statement_start(&code, open).min(open);
            if blocks[open].is_none() {
                // The function's own block: the whole definition.
                cover(from, close);
            } else {
                cover(from, statement_end(&code, from).max(close));
            }
            continue;
        }
        let before = next.checked_sub(1);
        let same_line = before.is_some_and(|k| !text[code[k].end..at].contains('\n'));
        match before {
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

    fn covers(text: &str, needle: &str, nth: usize) -> bool {
        let at = text.match_indices(needle).nth(nth).unwrap().0;
        covered(&ranges(text), at, at + needle.len())
    }

    #[test]
    fn a_tag_in_a_function_block_covers_the_function() {
        let leading = "void F(void)\n{\n    a = 1;\n    /* FAKEMATCH: order */\n    b = (u8)c;\n    d = 2;\n}\nvoid G(void)\n{\n    e = (u8)f;\n}\n";
        assert_eq!(spans(leading), vec!["void F(void)\n{\n    a = 1;\n    /* FAKEMATCH: order */\n    b = (u8)c;\n    d = 2;\n}"]);
        let trailing = "void F(void)\n{\n    a = 1;\n    b = c +\n        0x10; /* FAKEMATCH: r4 */\n    d = 2;\n}\n";
        assert_eq!(spans(trailing), vec![trailing.trim_end()]);
        let first = "void F(void)\n{\n    /* FAKEMATCH: frame */\n    u8 a;\n    a = 0x10;\n}\nvoid G(void)\n{\n    b = 0x10;\n}\n";
        assert_eq!(spans(first), vec!["void F(void)\n{\n    /* FAKEMATCH: frame */\n    u8 a;\n    a = 0x10;\n}"]);
    }

    #[test]
    fn a_tag_in_an_inner_block_covers_the_statement_that_owns_it() {
        let leading = "void F(void)\n{\n    x;\n    if (a) {\n        b = 1;\n        /* FAKEMATCH: order */\n        c = 0x10;\n        d = 0x10;\n    } else {\n        e = 1;\n    }\n    z = 0x10;\n}\n";
        assert_eq!(spans(leading), vec!["if (a) {\n        b = 1;\n        /* FAKEMATCH: order */\n        c = 0x10;\n        d = 0x10;\n    } else {\n        e = 1;\n    }"]);
        let opens = "void F(void)\n{\n    if (a) { /* FAKEMATCH: order */\n        b = 0x10;\n    } else {\n        c = 1;\n    }\n    d;\n}\n";
        assert_eq!(spans(opens), vec!["if (a) { /* FAKEMATCH: order */\n        b = 0x10;\n    } else {\n        c = 1;\n    }"]);
        let looped = "void F(void)\n{\n    while (q) {\n        x;\n        do { Io_SetBlendControl(0x3f42); } while (0); /* FAKEMATCH: order */\n        y = 0x10;\n    }\n    w = 0x10;\n}\n";
        assert_eq!(spans(looped), vec!["while (q) {\n        x;\n        do { Io_SetBlendControl(0x3f42); } while (0); /* FAKEMATCH: order */\n        y = 0x10;\n    }"]);
        let chain = "void F(void)\n{\n    if (q) {\n        if (v == 1)\n            Call3(f, 0x10, 1);\n        else if (v == 2)\n            Call3(f, 0x20, 2); /* FAKEMATCH: r4 */\n    } else if (r) {\n        e = 0x30;\n    }\n    g = 0x30;\n}\n";
        assert!(covers(chain, "Call3(f, 0x10, 1)", 0) && covers(chain, "e = 0x30", 0) && !covers(chain, "g = 0x30", 0));
    }

    /// The reviewer's cases: each tag's reason names several statements.
    #[test]
    fn a_tag_protects_every_statement_its_reason_names() {
        // FIELD/TAKARA_HASHIRA/PRIORITY.C:334-355 (tags at 337 and 345)
        let priority = "void F(void)\n{\n        if (*flags & 1) {
            if (GetMapCellCollision(2, actor->x, actor->z) == 50) {
                Engine_AudioPlayCue(189);
                /* FAKEMATCH: the byte view keeps these flag updates out of the record's alias set; field stores reallocate the loop (score 2573). */
                *(u8 *)&actor->priority_flags &= 254;
                TakaraHashira_LowerActorToLedge(id, 1);
                *(u8 *)&actor->priority_flags |= 1;
            } else if (GetMapCellCollision(2, actor->x, actor->z) == 51) {
                SceneActor_WaitHeightBelowLimit((struct FieldActor *)actor, 0);
                Engine_AudioPlayCue(189);
                actor->y = 0;
                /* FAKEMATCH: the byte view keeps these flag updates out of the record's alias set; field stores reallocate the loop (score 2573). */
                *(u8 *)&actor->priority_flags &= 254;
                StagedActor_StepDownUntilClamp(id);
                actor->x = 0;
                actor->y = 0;
                actor->z = 0;
                *(u8 *)&actor->priority_flags |= 1;
            } else {
                OverlayObject_WaitUntilSettledAndReset((struct FieldActor *)actor);
            }
            *flags = 0;
        }\n}\n";
        for nth in 0..4 {
            assert!(covers(priority, "*(u8 *)&actor->priority_flags", nth), "{nth}");
        }
        assert!(covers(priority, "(struct FieldActor *)actor, 0", 0));
        assert!(!covers(priority, "*flags = 0", 0));
        // FIELD/HAIDIA_MURA/MURA.C:2569-2584 (tag at 2570)
        let mura = "s32 F(void)\n{\n    if (Runtime_ComputeFixedPointDistance(&target->x.fixed, &obj->x.fixed) < range || force != 0) {
        if (angle == dir || right == dir || left == dir || force != 0) {
            /* FAKEMATCH: plain-byte publication avoids synthetic QI masks. */
            *(u8 *)&obj->paused = 1;
            Object_SetMode(obj, 1);
            result = 1;
            *(u8 *)&obj->rise_counter = result;
        } else {
            *(u8 *)&obj->paused = 0;
            Object_SetMode(obj, 2);
            *(u8 *)&obj->rise_counter = 0;
        }
    } else {
        *(u8 *)&obj->paused = 0;
        Object_SetMode(obj, 2);
        *(u8 *)&obj->rise_counter = 0;
    }
    return result;\n}\n";
        for nth in 0..2 {
            assert!(covers(mura, "*(u8 *)&obj->paused", nth) && covers(mura, "*(u8 *)&obj->rise_counter", nth), "{nth}");
        }
        assert!(!covers(mura, "*(u8 *)&obj->paused", 2) && !covers(mura, "return result", 0));
    }

    #[test]
    fn a_tag_outside_every_block_covers_its_statement() {
        let before = "/* FAKEMATCH: frame */\nvoid F(void)\n{\n    a = 0x10;\n}\nvoid G(void)\n{\n    b = 0x10;\n}\n";
        assert_eq!(spans(before), vec!["void F(void)\n{\n    a = 0x10;\n}"]);
        let after = "u8 gValue = 0x10; /* FAKEMATCH: data */\nu8 gOther = 0x10;\n";
        assert_eq!(spans(after), vec!["u8 gValue = 0x10;"]);
    }

    #[test]
    fn statements_holding_asm_are_covered() {
        let text = "{\n    register u32 r asm(\"r4\") = (u32)x;\n    asm(\"\" : : \"r\"(r));\n    y = (u8)z;\n}\n";
        assert_eq!(spans(text), vec!["register u32 r asm(\"r4\") = (u32)x;", "asm(\"\" : : \"r\"(r));"]);
    }
}
