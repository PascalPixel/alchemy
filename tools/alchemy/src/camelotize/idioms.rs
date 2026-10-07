//! Spellings a C programmer would not write: `x = x + 1` for `x++`,
//! `x = x | n` for `x |= n`, `!(a == b)` for `a != b`, parentheses around a
//! single name or number, and empty statements. Found on the token stream;
//! each is proved by `trial`.

use super::casts::{after_callee, cast_at, ident, operand_start, punct};
use super::trial::{self, Edit};
use crate::permute::lex::{lex, Tok, Token};
use crate::targets::DecompTarget;
use std::path::Path;

const COMPOUND: &[&str] = &["+", "-", "*", "|", "&", "^", "<<", ">>"];

/// The index of the bracket matching the opener at `open`.
fn closing(tokens: &[Token], open: usize) -> Option<usize> {
    let mut depth = 0;
    for (k, token) in tokens.iter().enumerate().skip(open) {
        match punct(token) {
            Some("(" | "[" | "{") => depth += 1,
            Some(")" | "]" | "}") => {
                depth -= 1;
                if depth == 0 {
                    return Some(k);
                }
            }
            _ => {}
        }
    }
    None
}

/// The operators at the top level of `tokens[from..to]`, or none when the
/// brackets there do not balance.
fn top_level(tokens: &[Token], from: usize, to: usize) -> Option<Vec<&'static str>> {
    let mut depth = 0;
    let mut found = Vec::new();
    for token in &tokens[from..to] {
        match punct(token) {
            Some("(" | "[" | "{") => depth += 1,
            Some(")" | "]" | "}") => {
                depth -= 1;
                if depth < 0 {
                    return None;
                }
            }
            Some(p) if depth == 0 => found.push(p),
            _ => {}
        }
    }
    (depth == 0).then_some(found)
}

fn same(tokens: &[Token], a: std::ops::Range<usize>, b: std::ops::Range<usize>) -> bool {
    a.len() == b.len() && tokens[a].iter().zip(&tokens[b]).all(|(x, y)| x.tok == y.tok)
}

/// `L = L op R;` as `L op= R;`, or `L++;` / `L--;` for a step of one.
fn compound(text: &str, tokens: &[Token], found: &mut Vec<Edit>) {
    for start in 0..tokens.len() {
        if start > 0 && !matches!(punct(&tokens[start - 1]), Some(";" | "{" | "}")) {
            continue;
        }
        let Some(assign) = (start..tokens.len()).find(|&k| matches!(punct(&tokens[k]), Some("=" | ";" | "{" | "}"))) else {
            continue;
        };
        if punct(&tokens[assign]) != Some("=") || assign == start {
            continue;
        }
        let left = start..assign;
        if tokens[left.clone()].iter().any(|t| matches!(punct(t), Some("(" | "*" | "++" | "--")) || ident(t).is_some_and(|n| n == "return")) {
            continue;
        }
        let right = assign + 1..assign + 1 + left.len();
        if right.end >= tokens.len() || !same(tokens, left.clone(), right.clone()) {
            continue;
        }
        let Some(op) = punct(&tokens[right.end]).filter(|p| COMPOUND.contains(p)) else {
            continue;
        };
        let Some(end) = (right.end..tokens.len()).find(|&k| matches!(punct(&tokens[k]), Some(";" | "{" | "}"))) else {
            continue;
        };
        if punct(&tokens[end]) != Some(";") {
            continue;
        }
        let rest = right.end + 1..end;
        if rest.is_empty() {
            continue;
        }
        let Some(operators) = top_level(tokens, rest.start, rest.end) else {
            continue;
        };
        let tight = |p: &&str| matches!(*p, "*" | "/" | "%" | "." | "->");
        let ok = match op {
            "+" | "-" => operators.iter().all(tight),
            _ => operators.iter().all(|p| matches!(*p, "." | "->")),
        };
        if !ok {
            continue;
        }
        let target = &text[tokens[start].start..tokens[assign - 1].end];
        let value = &text[tokens[rest.start].start..tokens[rest.end - 1].end];
        let replacement = match (op, value) {
            ("+", "1") => format!("{target}++"),
            ("-", "1") => format!("{target}--"),
            _ => format!("{target} {op}= {value}"),
        };
        found.push(Edit { start: tokens[start].start, end: tokens[end - 1].end, text: replacement });
    }
}

/// `!(a == b)` as `a != b`, where nothing around binds tighter than `!=`.
fn negations(text: &str, tokens: &[Token], found: &mut Vec<Edit>) {
    for bang in 0..tokens.len() {
        if punct(&tokens[bang]) != Some("!") || tokens.get(bang + 1).and_then(punct) != Some("(") {
            continue;
        }
        let Some(close) = closing(tokens, bang + 1) else {
            continue;
        };
        let Some(operators) = top_level(tokens, bang + 2, close) else {
            continue;
        };
        let [op @ ("==" | "!=")] = operators.as_slice() else {
            continue;
        };
        let loose_before = bang.checked_sub(1).is_none_or(|k| {
            matches!(punct(&tokens[k]), Some("(" | "," | ";" | "&&" | "||" | "?" | ":" | "=" | "{"))
                || ident(&tokens[k]).is_some_and(|n| n == "return")
        });
        let loose_after = tokens.get(close + 1).is_none_or(|t| matches!(punct(t), Some(")" | "," | ";" | "&&" | "||" | "?" | ":")));
        if !loose_before || !loose_after {
            continue;
        }
        let at = (bang + 2..close).find(|&k| punct(&tokens[k]) == Some(*op)).expect("operator");
        let flipped = if *op == "==" { "!=" } else { "==" };
        let inner = format!(
            "{}{}{}",
            &text[tokens[bang + 2].start..tokens[at].start],
            flipped,
            &text[tokens[at].end..tokens[close - 1].end]
        );
        found.push(Edit { start: tokens[bang].start, end: tokens[close].end, text: inner });
    }
}

/// Whether the `)` at `close` ends a cast.
fn is_cast_end(tokens: &[Token], close: usize) -> bool {
    if punct(&tokens[close]) != Some(")") {
        return false;
    }
    let mut depth = 0;
    for k in (0..=close).rev() {
        match punct(&tokens[k]) {
            Some(")") => depth += 1,
            Some("(") => {
                depth -= 1;
                if depth == 0 {
                    return cast_at(tokens, k) == Some(close) && !after_callee(tokens, k);
                }
            }
            _ => {}
        }
    }
    false
}

/// `(x)` around a name, number or member chain (`(a->b)`, `(a[i].c)`),
/// where it is neither a call nor a cast.
fn parentheses(text: &str, tokens: &[Token], found: &mut Vec<Edit>) {
    for open in 0..tokens.len().saturating_sub(2) {
        if punct(&tokens[open]) != Some("(") {
            continue;
        }
        let after_cast = open > 0 && is_cast_end(tokens, open - 1);
        if after_callee(tokens, open) && !after_cast {
            continue;
        }
        let Some(close) = chain_end(tokens, open + 1).filter(|&k| punct(&tokens[k]) == Some(")")) else {
            continue;
        };
        if tokens.get(close + 1).is_some_and(|t| operand_start(t) && !matches!(punct(t), Some("-" | "+" | "*" | "&"))) {
            continue;
        }
        if open > 0 && ident(&tokens[open - 1]).is_some_and(|n| n == "sizeof") {
            continue;
        }
        let inner = &text[tokens[open + 1].start..tokens[close - 1].end];
        found.push(Edit { start: tokens[open].start, end: tokens[close].end, text: inner.to_string() });
    }
}

/// The index just past a name or number with its `->` / `.` members and
/// `[...]` indexes, starting at `from`, which binds tighter than any operator
/// around it.
fn chain_end(tokens: &[Token], from: usize) -> Option<usize> {
    let first = tokens.get(from)?;
    if matches!(first.tok, Tok::Number(_)) {
        return Some(from + 1);
    }
    if matches!(ident(first)?, "sizeof" | "defined") {
        return None;
    }
    let mut k = from + 1;
    loop {
        match tokens.get(k).and_then(punct) {
            Some("->" | ".") if tokens.get(k + 1).and_then(ident).is_some() => k += 2,
            // A call binds as tightly: `(Object_GetById(0))->field`.
            Some("[" | "(") => k = closing(tokens, k)? + 1,
            _ => return Some(k),
        }
    }
}

/// The indentation of the line holding byte `at`.
fn indentation(text: &str, at: usize) -> &str {
    let line = text[..at].rfind('\n').map_or(0, |k| k + 1);
    let width = text[line..].bytes().take_while(|b| matches!(b, b' ' | b'\t')).count();
    &text[line..line + width]
}

/// Whether the block `{ ... }` closing at `close` ends in a statement that
/// leaves it: `return`, `break`, `continue` or `goto`.
fn leaves(tokens: &[Token], open: usize, close: usize) -> bool {
    if close < open + 2 || punct(&tokens[close - 1]) != Some(";") {
        return false;
    }
    let mut depth = 0;
    let mut k = close - 1;
    while k > open + 1 {
        k -= 1;
        match punct(&tokens[k]) {
            Some(")" | "]") => depth += 1,
            Some("(" | "[") => depth -= 1,
            Some(";" | "{" | "}") if depth == 0 => {
                k += 1;
                break;
            }
            _ => {}
        }
    }
    ident(&tokens[k]).is_some_and(|n| matches!(n, "return" | "break" | "continue" | "goto"))
}

/// Whether the block opening at `open` starts with a declaration.
fn declares(tokens: &[Token], open: usize) -> bool {
    const TYPES: &[&str] = &[
        "struct", "union", "enum", "const", "volatile", "register", "static", "extern", "unsigned", "signed",
        "int", "char", "short", "long", "void", "typedef",
    ];
    let first = tokens.get(open + 1);
    let second = tokens.get(open + 2);
    first.and_then(ident).is_some_and(|n| TYPES.contains(&n))
        || (first.and_then(ident).is_some() && (second.and_then(ident).is_some() || second.and_then(punct) == Some("*")))
}

/// `if (x) { ...; return ...; } else { body }` as `if (x) { ...; return
/// ...; }` followed by the body, one level less indented; `else if` the
/// same way. Only an `if` that starts a statement, so no outer `else`
/// claims it.
fn flatten(text: &str, tokens: &[Token], found: &mut Vec<Edit>) {
    for at in 0..tokens.len() {
        if ident(&tokens[at]) != Some("if") {
            continue;
        }
        if at > 0 && !matches!(punct(&tokens[at - 1]), Some(";" | "{" | "}")) {
            continue;
        }
        let Some(condition) = tokens.get(at + 1).filter(|t| punct(t) == Some("(")).and_then(|_| closing(tokens, at + 1)) else {
            continue;
        };
        let open = condition + 1;
        if tokens.get(open).and_then(punct) != Some("{") {
            continue;
        }
        let Some(close) = closing(tokens, open) else {
            continue;
        };
        if tokens.get(close + 1).and_then(ident) != Some("else") || !leaves(tokens, open, close) {
            continue;
        }
        let indent = indentation(text, tokens[close].start);
        let after = close + 2;
        match tokens.get(after) {
            Some(t) if ident(t) == Some("if") => {
                found.push(Edit { start: tokens[close].end, end: t.start, text: format!("\n{indent}") });
            }
            Some(t) if punct(t) == Some("{") => {
                let Some(end) = closing(tokens, after) else {
                    continue;
                };
                if declares(tokens, after) || end == after + 1 {
                    continue;
                }
                let inner = text[tokens[after].end..tokens[end].start].trim_end();
                if !inner.starts_with('\n') || indentation(text, tokens[end].start) != indent {
                    continue;
                }
                let deeper = indentation(text, tokens[after + 1].start);
                let Some(step) = deeper.strip_prefix(indent).map(str::len).filter(|&n| n > 0) else {
                    continue;
                };
                let body: Vec<String> = inner
                    .split('\n')
                    .map(|line| {
                        let blank = line.bytes().take_while(|b| matches!(b, b' ' | b'\t')).count();
                        line[blank.min(step)..].to_string()
                    })
                    .collect();
                found.push(Edit { start: tokens[close].end, end: tokens[end].end, text: body.join("\n") });
            }
            _ => {}
        }
    }
}

/// A second `;` after a statement, outside a `for` header.
fn empty_statements(tokens: &[Token], found: &mut Vec<Edit>) {
    let mut depth = 0i32;
    for k in 0..tokens.len() {
        match punct(&tokens[k]) {
            Some("(") => depth += 1,
            Some(")") => depth -= 1,
            Some(";") if depth == 0 && k > 0 && punct(&tokens[k - 1]) == Some(";") => {
                found.push(Edit { start: tokens[k].start, end: tokens[k].end, text: String::new() });
            }
            _ => {}
        }
    }
}

pub(crate) fn candidates(text: &str) -> Vec<Edit> {
    let Ok(tokens) = lex(text) else {
        return Vec::new();
    };
    let plain: Vec<Token> = tokens.iter().filter(|t| !matches!(t.tok, Tok::Comment(_))).cloned().collect();
    let tokens = super::numbers::code(tokens);
    let mut found = Vec::new();
    compound(text, &tokens, &mut found);
    negations(text, &tokens, &mut found);
    parentheses(text, &tokens, &mut found);
    flatten(text, &tokens, &mut found);
    empty_statements(&plain, &mut found);
    trial::untagged(text, found)
}

pub(crate) fn rewrite(root: &Path, file: &Path, text: &str, games: &[DecompTarget]) -> Result<(String, usize), String> {
    trial::proved(root, file, text, games, candidates(text))
}

#[cfg(test)]
mod tests {
    use super::*;

    fn rewritten(text: &str) -> String {
        trial::apply(text, &trial::disjoint(candidates(text)))
    }

    #[test]
    fn steps_and_updates_become_compound() {
        assert_eq!(
            rewritten("{\n    i = i + 1;\n    p->n = p->n - 1;\n    x = x + y * 2;\n    f = f | 0x10;\n}\n"),
            "{\n    i++;\n    p->n--;\n    x += y * 2;\n    f |= 0x10;\n}\n"
        );
    }

    #[test]
    fn updates_that_would_regroup_stay() {
        let text = "{\n    x = x - a + b;\n    y = y * a + b;\n    z = z + 1 << 2;\n    *p = *p + 1;\n}\n";
        assert_eq!(rewritten(text), text);
    }

    #[test]
    fn negated_comparisons_and_single_parentheses() {
        assert_eq!(rewritten("if (!(a == b) && c) {}\nr = (x) + (3);\n"), "if (a != b && c) {}\nr = x + 3;\n");
        assert_eq!(rewritten("(s->sprite)->n = -(a[i + 1].b);\n"), "s->sprite->n = -a[i + 1].b;\n");
        let calls = "f(a->b);\nif (p->q) {}\nr = (a->b + 1);\n";
        assert_eq!(rewritten(calls), calls);
        let text = "n = !(a == b) + 1;\ny = f(x);\nz = (u8)(w);\ns = sizeof(v);\n";
        assert_eq!(rewritten(text), "n = !(a == b) + 1;\ny = f(x);\nz = (u8)w;\ns = sizeof(v);\n");
    }

    /// The reviewer's finding: the parentheses the casts pass leaves around
    /// a call operand.
    #[test]
    fn parentheses_around_a_call_operand_go() {
        assert_eq!(rewritten("    (Object_GetById(0))->field = 1;\n"), "    Object_GetById(0)->field = 1;\n");
        assert_eq!(rewritten("n = (f(a, b)) + 1;\nreturn (g(x)[2]);\n"), "n = f(a, b) + 1;\nreturn g(x)[2];\n");
        let kept = "n = (f(a))(b);\ns = (sizeof(v));\nwhile (f(x)) {}\n";
        assert_eq!(rewritten(kept), kept);
    }

    #[test]
    fn else_after_a_leaving_block_is_flattened() {
        let text = "void F(void)\n{\n    if (a) {\n        x = 1;\n        return;\n    } else {\n        y = 2;\n        if (b) {\n            z = 3;\n        }\n    }\n    w = 4;\n}\n";
        assert_eq!(
            rewritten(text),
            "void F(void)\n{\n    if (a) {\n        x = 1;\n        return;\n    }\n    y = 2;\n    if (b) {\n        z = 3;\n    }\n    w = 4;\n}\n"
        );
        let chain = "{\n    if (a) {\n        return f(x);\n    } else if (b) {\n        g();\n    }\n}\n";
        assert_eq!(rewritten(chain), "{\n    if (a) {\n        return f(x);\n    }\n    if (b) {\n        g();\n    }\n}\n");
    }

    #[test]
    fn else_that_the_block_can_reach_or_that_declares_stays() {
        let falls = "{\n    if (a) {\n        if (b) {\n            return;\n        }\n        x = 1;\n    } else {\n        y = 2;\n    }\n}\n";
        assert_eq!(rewritten(falls), falls);
        let declares = "{\n    if (a) {\n        return;\n    } else {\n        u8 i;\n        i = 2;\n    }\n}\n";
        assert_eq!(rewritten(declares), declares);
        let claimed = "{\n    if (c) {\n        n();\n    } else if (a) {\n        return;\n    } else {\n        y = 2;\n    }\n}\n";
        assert_eq!(rewritten(claimed), claimed);
        let leaves_inside = "{\n    if (a) {\n        while (b) {\n            return;\n        }\n    } else {\n        y = 2;\n    }\n}\n";
        assert_eq!(rewritten(leaves_inside), leaves_inside);
    }

    #[test]
    fn fakematch_tagged_statements_and_asm_stay() {
        let lead = "void F(void)\n{\n    if (q) {\n        k = 0;\n        /* FAKEMATCH: one pass */\n        do {\n            i = i + 1;\n        } while (0);\n    }\n    j = j + 1;\n}\n";
        assert_eq!(rewritten(lead), lead.replace("j = j + 1", "j++"));
        let trail = "void F(void)\n{\n    {\n        i = i + 1; /* FAKEMATCH: r4 */\n    }\n    r = (x);\n}\n";
        assert_eq!(rewritten(trail), trail.replace("(x)", "x"));
        let whole = "void F(void)\n{\n    i = i + 1; /* FAKEMATCH: r4 */\n    r = (x);\n}\n";
        assert_eq!(rewritten(whole), whole);
        let block = "{\n    if (a) { /* FAKEMATCH: order */\n        return;\n    } else {\n        y = 2;\n    }\n}\n";
        assert_eq!(rewritten(block), block);
        let asm = "{\n    asm(\"\" : \"+r\"((x)));\n    register u32 r asm(\"r4\") = (x);\n}\n";
        assert_eq!(rewritten(asm), asm);
    }

    #[test]
    fn empty_statements_go_but_for_headers_and_do_loops_stay() {
        assert_eq!(rewritten("{\n    x = 1;;\n    for (;;) {}\n}\n"), "{\n    x = 1;\n    for (;;) {}\n}\n");
        let text = "do { x++; } while (x);\nx = 1;\nasm(\"\" : \"+r\"(x));\n";
        assert_eq!(rewritten(text), text);
    }
}
