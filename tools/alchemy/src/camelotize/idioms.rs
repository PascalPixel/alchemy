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

/// `(x)` around one name or number, where it is neither a call nor a cast.
fn parentheses(text: &str, tokens: &[Token], found: &mut Vec<Edit>) {
    for open in 0..tokens.len().saturating_sub(2) {
        let after_cast = open > 0 && is_cast_end(tokens, open - 1);
        if punct(&tokens[open]) != Some("(") || punct(&tokens[open + 2]) != Some(")") || (after_callee(tokens, open) && !after_cast) {
            continue;
        }
        let inner = &tokens[open + 1];
        if !matches!(inner.tok, Tok::Number(_) | Tok::Ident(_)) {
            continue;
        }
        if tokens.get(open + 3).is_some_and(|t| operand_start(t) && !matches!(punct(t), Some("-" | "+" | "*" | "&"))) {
            continue;
        }
        if open > 0 && ident(&tokens[open - 1]).is_some_and(|n| n == "sizeof") {
            continue;
        }
        found.push(Edit { start: tokens[open].start, end: tokens[open + 2].end, text: text[inner.start..inner.end].to_string() });
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
    empty_statements(&plain, &mut found);
    found
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
        let text = "n = !(a == b) + 1;\ny = f(x);\nz = (u8)(w);\ns = sizeof(v);\n";
        assert_eq!(rewritten(text), "n = !(a == b) + 1;\ny = f(x);\nz = (u8)w;\ns = sizeof(v);\n");
    }

    #[test]
    fn empty_statements_go_but_for_headers_and_do_loops_stay() {
        assert_eq!(rewritten("{\n    x = 1;;\n    for (;;) {}\n}\n"), "{\n    x = 1;\n    for (;;) {}\n}\n");
        let text = "do { x++; } while (x);\nx = 1;\nasm(\"\" : \"+r\"(x));\n";
        assert_eq!(rewritten(text), text);
    }
}
