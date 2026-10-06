//! Hex literals that are quantities, written in decimal. Hex stays where C
//! programmers keep it: bit operations and the masks and flags they test or
//! store, addresses, Q-format fixed-point constants and the values beside
//! them, and the all-ones limits of a width. Preprocessor lines, strings,
//! comments and asm statements are never touched. Every converted value is
//! below 0x02000000, so it has type int (or its suffix's type) in either
//! spelling and the bytes cannot change.

use crate::permute::lex::{lex, Tok, Token};

const ADDRESS: u64 = 0x0200_0000;
const BITWISE: &[&str] = &["&", "|", "^", "~", "<<", ">>", "&=", "|=", "^=", "<<=", ">>="];
const LIMITS: &[u64] = &[0x7F, 0xFF, 0x7FFF, 0xFFFF, 0xFF_FFFF];

struct Hex {
    value: u64,
    suffix: String,
}

fn hex(text: &str) -> Option<Hex> {
    let rest = text.strip_prefix("0x").or_else(|| text.strip_prefix("0X"))?;
    let digits = rest.chars().take_while(char::is_ascii_hexdigit).count();
    let suffix = &rest[digits..];
    if digits == 0 || !suffix.chars().all(|c| matches!(c, 'u' | 'U' | 'l' | 'L')) {
        return None;
    }
    let value = u64::from_str_radix(&rest[..digits], 16).ok()?;
    Some(Hex { value, suffix: suffix.to_string() })
}

/// A Q-format constant: whole units (a multiple of 0x100 from 0x1000 up), or
/// a binary fraction such as 0x6666 (0.4) or 0x19999 (1.6), whose low three
/// hex digits repeat.
fn fixed_point(value: u64) -> bool {
    let low = value & 0xFFF;
    value >= 0x1000 && (value % 0x100 == 0 || (low >> 8 == low & 0xF && (low >> 4) & 0xF == low & 0xF))
}

fn punct(token: &Token) -> Option<&'static str> {
    match token.tok {
        Tok::Punct(p) => Some(p),
        _ => None,
    }
}

fn ident(token: &Token) -> Option<&str> {
    match &token.tok {
        Tok::Ident(name) => Some(name),
        _ => None,
    }
}

/// Code tokens: comments dropped, asm statements removed whole.
pub(super) fn code(tokens: Vec<Token>) -> Vec<Token> {
    let tokens: Vec<Token> = tokens.into_iter().filter(|t| !matches!(t.tok, Tok::Comment(_))).collect();
    let mut out = Vec::with_capacity(tokens.len());
    let mut i = 0;
    while i < tokens.len() {
        if matches!(ident(&tokens[i]), Some("asm" | "__asm__" | "__asm")) {
            let mut j = i + 1;
            while j < tokens.len() && matches!(ident(&tokens[j]), Some("volatile" | "__volatile__")) {
                j += 1;
            }
            if j < tokens.len() && punct(&tokens[j]) == Some("(") {
                let mut depth = 0;
                while j < tokens.len() {
                    match punct(&tokens[j]) {
                        Some("(") => depth += 1,
                        Some(")") => {
                            depth -= 1;
                            if depth == 0 {
                                break;
                            }
                        }
                        _ => {}
                    }
                    j += 1;
                }
                i = j + 1;
                continue;
            }
        }
        out.push(tokens[i].clone());
        i += 1;
    }
    out
}

/// The innermost bracket group each token sits in, as the index of its opener.
fn groups(tokens: &[Token]) -> Vec<Option<usize>> {
    let mut stack: Vec<usize> = Vec::new();
    let mut out = Vec::with_capacity(tokens.len());
    for (i, token) in tokens.iter().enumerate() {
        match punct(token) {
            Some("(" | "[" | "{") => {
                out.push(stack.last().copied());
                stack.push(i);
            }
            Some(")" | "]" | "}") => {
                stack.pop();
                out.push(stack.last().copied());
            }
            _ => out.push(stack.last().copied()),
        }
    }
    out
}

fn flag_name(name: &str) -> bool {
    if name.starts_with("REG_") {
        return true;
    }
    let lower = name.to_ascii_lowercase();
    ["flag", "mask", "bits", "attr"].iter().any(|word| lower.contains(word))
}

/// Whether the operand that ends just before `end` (exclusive) is a mask:
/// a bracketed expression with a bit operation inside, or a flag-named
/// variable or member.
fn masked_operand(tokens: &[Token], end: usize) -> bool {
    let Some(last) = end.checked_sub(1) else {
        return false;
    };
    match punct(&tokens[last]) {
        Some(")") => {
            let mut depth = 0;
            for k in (0..=last).rev() {
                match punct(&tokens[k]) {
                    Some(")") => depth += 1,
                    Some("(") => {
                        depth -= 1;
                        if depth == 0 {
                            return tokens[k..=last].iter().any(|t| punct(t).is_some_and(|p| BITWISE.contains(&p)));
                        }
                    }
                    _ => {}
                }
            }
            false
        }
        Some("]") => false,
        _ => ident(&tokens[last]).is_some_and(flag_name),
    }
}

fn keeps_hex(tokens: &[Token], groups: &[Option<usize>], i: usize, value: u64) -> bool {
    if value >= ADDRESS || fixed_point(value) || LIMITS.contains(&value) {
        return true;
    }
    let before = i.checked_sub(1).map(|k| &tokens[k]);
    let after = tokens.get(i + 1);
    if before.and_then(punct).is_some_and(|p| BITWISE.contains(&p)) {
        return true;
    }
    if after.and_then(punct).is_some_and(|p| matches!(p, "&" | "|" | "^" | "<<" | ">>")) {
        return true;
    }
    if matches!(before.and_then(punct), Some("==" | "!=" | "=")) && masked_operand(tokens, i - 1) {
        return true;
    }
    if after.and_then(punct).is_some_and(|p| matches!(p, "==" | "!=")) {
        let mut k = i + 2;
        while k < tokens.len() && !matches!(punct(&tokens[k]), Some(";" | ")" | "," | "&&" | "||")) {
            if punct(&tokens[k]).is_some_and(|p| BITWISE.contains(&p)) {
                return true;
            }
            k += 1;
        }
    }
    // Beside a fixed-point constant in the same argument or initializer list.
    let Some(group) = groups[i] else {
        return false;
    };
    let list = match punct(&tokens[group]) {
        Some("(") => true,
        Some("{") => group.checked_sub(1).and_then(|k| punct(&tokens[k])).is_some_and(|p| matches!(p, "=" | "," | "{")),
        _ => false,
    };
    list && tokens.iter().enumerate().any(|(k, t)| {
        k != i
            && groups[k] == Some(group)
            && matches!(&t.tok, Tok::Number(n) if hex(n).is_some_and(|h| fixed_point(h.value) && h.value < ADDRESS))
    })
}

pub(crate) fn rewrite(text: &str) -> (String, usize) {
    let Ok(tokens) = lex(text) else {
        return (text.to_string(), 0);
    };
    let tokens = code(tokens);
    let groups = groups(&tokens);
    let mut edits: Vec<(usize, usize, String)> = Vec::new();
    for (i, token) in tokens.iter().enumerate() {
        let Tok::Number(number) = &token.tok else {
            continue;
        };
        let Some(h) = hex(number) else {
            continue;
        };
        if keeps_hex(&tokens, &groups, i, h.value) {
            continue;
        }
        edits.push((token.start, token.end, format!("{}{}", h.value, h.suffix)));
    }
    let count = edits.len();
    let mut out = text.to_string();
    for (start, end, decimal) in edits.into_iter().rev() {
        out.replace_range(start..end, &decimal);
    }
    (out, count)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn same(text: &str) {
        assert_eq!(rewrite(text), (text.to_string(), 0), "{text}");
    }

    #[test]
    fn quantities_become_decimal_with_their_sign_and_suffix() {
        let text = "void F(void)\n{\n    OBJ_POS_SET(8, 0xC0, -0x108);\n    a[0x1F] = 0x10u;\n    switch (x) { case 0x2A: break; }\n}\n";
        let (out, count) = rewrite(text);
        assert_eq!(count, 5);
        assert_eq!(
            out,
            "void F(void)\n{\n    OBJ_POS_SET(8, 192, -264);\n    a[31] = 16u;\n    switch (x) { case 42: break; }\n}\n"
        );
    }

    #[test]
    fn bit_operations_and_their_masks_stay_hex() {
        same("x = y & 0x3F;\nx |= 0x40;\nx &= ~0x4;\nz = 0x3 << 4;\nw = v >> 0x8;\nif ((a & 0xF0) == 0x80) {}\n");
        same("if (0x20 == (b & 0x30)) {}\nactor->flags = 0x204;\nif (gFlags != 0x11) {}\nt = 0xC | u;\n");
    }

    #[test]
    fn addresses_fixed_point_and_limits_stay_hex() {
        same("p = (u16 *)0x04000000;\nq = 0x03007FF0;\nspeed = 0x18000;\nx = 0xFF;\ny = 0xFFFF;\nREG_BLDALPHA = 0x1010;\n");
        same("Actor_SetSpeed(8, 0x19999, 0x6666);\ngravity = 0x1999;\n");
        assert_eq!(rewrite("Walk(9, 0x15f8, 0x6f8);\n").0, "Walk(9, 5624, 1784);\n");
    }

    #[test]
    fn values_beside_fixed_point_stay_hex() {
        same("Move(a, 0x10, 0x8000);\n");
        let (out, count) = rewrite("Move(a, 0x10, 0x8000);\nWait(0x10);\n");
        assert_eq!(count, 1);
        assert!(out.ends_with("Wait(16);\n"));
    }

    #[test]
    fn directives_strings_comments_and_asm_are_untouched() {
        same("#define REG (*(u16 *)0x10)\n#define N 0x20\nchar *s = \"0x10\";\n/* 0x10 */\nasm(\"mov r0, #0x10\" : : \"i\"(0x10));\n");
    }

    #[test]
    fn decimal_and_octal_literals_are_left_alone() {
        same("x = 16 + 010 + 'a';\n");
    }
}
