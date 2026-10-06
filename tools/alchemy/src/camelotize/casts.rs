//! Casts the compiler does not need. Candidates are found on the token
//! stream: a value cast to one of the six integer types, or a pointer cast
//! `(T *)`, in front of an operand. Each removal is proved by `trial`.

use super::trial::{self, Edit};
use crate::permute::lex::{lex, Tok, Token};
use crate::targets::DecompTarget;
use std::path::Path;

const VALUES: &[&str] = &["u8", "s8", "u16", "s16", "u32", "s32"];

pub(super) fn punct(token: &Token) -> Option<&'static str> {
    match token.tok {
        Tok::Punct(p) => Some(p),
        _ => None,
    }
}

pub(super) fn ident(token: &Token) -> Option<&str> {
    match &token.tok {
        Tok::Ident(name) => Some(name),
        _ => None,
    }
}

pub(super) fn operand_start(token: &Token) -> bool {
    match &token.tok {
        Tok::Ident(_) | Tok::Number(_) | Tok::Char(_) | Tok::Str(_) => true,
        Tok::Punct(p) => matches!(*p, "(" | "*" | "-" | "+" | "~" | "!" | "&" | "++" | "--"),
        Tok::Comment(_) => false,
    }
}

/// Whether the `(` at `open` follows a callee, an array or a declarator
/// rather than starting an operand.
pub(super) fn after_callee(tokens: &[Token], open: usize) -> bool {
    let Some(before) = open.checked_sub(1).map(|k| &tokens[k]) else {
        return false;
    };
    match &before.tok {
        Tok::Ident(name) => !matches!(name.as_str(), "return" | "case" | "else"),
        Tok::Punct(p) => matches!(*p, ")" | "]"),
        _ => true,
    }
}

/// The index of the `)` closing a cast at `open`: a value type, or a type
/// with one or more `*`.
pub(super) fn cast_at(tokens: &[Token], open: usize) -> Option<usize> {
    let mut k = open + 1;
    while matches!(tokens.get(k).and_then(ident), Some("const" | "volatile")) {
        k += 1;
    }
    let name = ident(tokens.get(k)?)?;
    if matches!(name, "struct" | "union") {
        ident(tokens.get(k + 1)?)?;
        k += 1;
    }
    k += 1;
    if VALUES.contains(&name) && punct(tokens.get(k)?) == Some(")") {
        return Some(k);
    }
    let mut stars = 0;
    while matches!(tokens.get(k).and_then(punct), Some("*")) || matches!(tokens.get(k).and_then(ident), Some("const" | "volatile")) {
        if punct(&tokens[k]) == Some("*") {
            stars += 1;
        }
        k += 1;
    }
    (stars > 0 && punct(tokens.get(k)?) == Some(")")).then_some(k)
}

pub(crate) fn candidates(text: &str) -> Vec<Edit> {
    let Ok(tokens) = lex(text) else {
        return Vec::new();
    };
    let tokens = super::numbers::code(tokens);
    let mut found = Vec::new();
    for open in 0..tokens.len() {
        if punct(&tokens[open]) != Some("(") || after_callee(&tokens, open) {
            continue;
        }
        let Some(close) = cast_at(&tokens, open) else {
            continue;
        };
        if tokens.get(close + 1).is_some_and(operand_start) {
            found.push(Edit { start: tokens[open].start, end: tokens[close].end, text: String::new() });
        }
    }
    found
}

pub(crate) fn rewrite(root: &Path, file: &Path, text: &str, games: &[DecompTarget]) -> Result<(String, usize), String> {
    trial::proved(root, file, text, games, candidates(text))
}

#[cfg(test)]
mod tests {
    use super::*;

    fn spans(text: &str) -> Vec<&str> {
        candidates(text).iter().map(|c| &text[c.start..c.end]).collect()
    }

    #[test]
    fn value_and_pointer_casts_in_front_of_operands_are_found() {
        let text = "x = (u8)y + (s16)-z;\np = (struct Foo *)q;\nr = (const u16 *)(s + 2);\nreturn (u32)f(1);\n";
        assert_eq!(spans(text), vec!["(u8)", "(s16)", "(struct Foo *)", "(const u16 *)", "(u32)"]);
    }

    #[test]
    fn calls_declarators_sizeof_and_asm_are_not_casts() {
        let text = "void F(u8);\ntypedef void (*Fn)(u8 *);\nn = sizeof(u16);\nG(u8) ;\nasm(\"\" : : \"r\"((u32)x));\n/* (u8)x */\n";
        assert!(spans(text).is_empty(), "{:?}", spans(text));
    }

    #[test]
    fn removal_keeps_tokens_apart() {
        let text = "a = b -(s16)-c;\nreturn(u8)x;\nd = (u8) e;\nf((u8)g, (u16 *)h);\n";
        assert_eq!(trial::apply(text, &candidates(text)), "a = b - -c;\nreturn x;\nd = e;\nf(g, h);\n");
    }
}
