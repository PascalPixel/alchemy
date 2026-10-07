//! Hex literals that are quantities, written in decimal. Only code below
//! 0x1000 is converted: from 0x1000 up the authors wrote packed tile
//! entries, fixed point and sizes in hex. Hex also stays where C programmers
//! keep it: bit operations and the masks and flags they test or store, every
//! argument of a call or macro whose name says flag, the values beside a
//! fixed-point constant, and the all-ones limits of a width. Preprocessor
//! lines (every `#define` body among them), strings, comments, asm and
//! FAKEMATCH-tagged statements are never touched. Every converted value is
//! below 0x1000, so it has type int (or its suffix's type) in either spelling
//! and the bytes cannot change.

use super::tagged;
use crate::permute::lex::{lex, Tok, Token};

const ADDRESS: u64 = 0x0200_0000;
/// Values from here up stay hex in code.
const QUANTITY_LIMIT: u64 = 0x1000;
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
        Some("]") => {
            let mut depth = 0;
            for k in (0..=last).rev() {
                match punct(&tokens[k]) {
                    Some("]") => depth += 1,
                    Some("[") => {
                        depth -= 1;
                        if depth == 0 {
                            return k.checked_sub(1).and_then(|n| ident(&tokens[n])).is_some_and(flag_name);
                        }
                    }
                    _ => {}
                }
            }
            false
        }
        _ => ident(&tokens[last]).is_some_and(flag_name),
    }
}

fn says_flag(name: &str) -> bool {
    name.to_ascii_lowercase().contains("flag")
}

/// Whether a bracket group around token `i` belongs to a flag: the
/// arguments of a call or macro whose name says flag (`GameFlag_TestFar`,
/// `FLAG_SET`), an index into a flag-named array, or a list that passes a
/// flag-named function or value (`Call1(Engine_SetTriggerFlag, 0x220)`).
fn flag_argument(tokens: &[Token], groups: &[Option<usize>], i: usize) -> bool {
    let mut group = groups[i];
    while let Some(open) = group {
        let callee = open.checked_sub(1).and_then(|k| ident(&tokens[k]));
        let list = matches!(punct(&tokens[open]), Some("(" | "["));
        let passes = || (open + 1..tokens.len()).take_while(|&k| groups[k].is_some_and(|g| g >= open)).any(|k| groups[k] == Some(open) && ident(&tokens[k]).is_some_and(says_flag));
        if list && (callee.is_some_and(says_flag) || passes()) {
            return true;
        }
        group = groups[open];
    }
    false
}

/// The index of the token before the operand at `i`, past any casts.
fn before_operand(tokens: &[Token], i: usize) -> Option<usize> {
    let mut k = i.checked_sub(1)?;
    while punct(&tokens[k]) == Some(")") {
        let open = (0..k).rev().find(|&j| punct(&tokens[j]) == Some("("))?;
        let inside = &tokens[open + 1..k];
        let cast = !inside.is_empty()
            && inside.iter().all(|t| ident(t).is_some() || punct(t) == Some("*"))
            && ident(&inside[0]).is_some()
            && open.checked_sub(1).is_none_or(|j| ident(&tokens[j]).is_none() && punct(&tokens[j]) != Some(")"));
        if !cast {
            break;
        }
        k = open.checked_sub(1)?;
    }
    Some(k)
}

fn keeps_hex(tokens: &[Token], groups: &[Option<usize>], i: usize, value: u64) -> bool {
    if value >= QUANTITY_LIMIT || value >= ADDRESS || fixed_point(value) || LIMITS.contains(&value) {
        return true;
    }
    if flag_argument(tokens, groups, i) {
        return true;
    }
    let operand = before_operand(tokens, i);
    if operand.and_then(|k| punct(&tokens[k])).is_some_and(|p| BITWISE.contains(&p)) {
        return true;
    }
    if operand.is_some_and(|k| {
        matches!(punct(&tokens[k]), Some("==" | "!=" | "=" | "<" | "<=" | ">" | ">=" | "+=" | "-=")) && masked_operand(tokens, k)
    }) {
        return true;
    }
    let after = tokens.get(i + 1);
    if after.and_then(punct).is_some_and(|p| matches!(p, "&" | "|" | "^" | "<<" | ">>")) {
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
    let tagged = tagged::ranges(text);
    let mut edits: Vec<(usize, usize, String)> = Vec::new();
    for (i, token) in tokens.iter().enumerate() {
        let Tok::Number(number) = &token.tok else {
            continue;
        };
        let Some(h) = hex(number) else {
            continue;
        };
        if keeps_hex(&tokens, &groups, i, h.value) || tagged::covered(&tagged, token.start, token.end) {
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
        assert_eq!(rewrite("Walk(9, 0x15f8, 0x6f8);\n").0, "Walk(9, 0x15f8, 1784);\n");
    }

    #[test]
    fn values_from_0x1000_up_stay_hex() {
        same("tile = 0x5001;\nsize = 0x1000;\nx = 0xf128 + y;\nSet(0x4014, 0xc00f);\n");
        assert_eq!(rewrite("n = 0xfff;\n").0, "n = 4095;\n");
    }

    #[test]
    fn flag_arguments_stay_hex() {
        same("GameFlag_TestFar(0x16b);\nFLAG_SET(0x943);\nif (FLAG_CHK(0x214)) {}\nEngine_GameFlagSet(0xf2a);\n");
        same("x = GameFlag_Test(a + 0x10, 0x2);\nIF(FLAG_CHK(0x20)) {}\n");
        same("Call1(Engine_SetTriggerFlag, 0x220);\nCall2(GameFlag_SetByte, (s << 4) + 0x370, 1);\nf = gPlacementsFlag[0x14e];\n");
        same("for (flag = 0x200; flag <= 0x2ff; flag++) {}\nif (flag_bytes[flag] <= 0xfe) {}\n");
    }

    #[test]
    fn masks_behind_a_cast_stay_hex() {
        same("actor->flags &= (u8)0x7e;\nx = y | (u16)0x10;\nz = w & (u8 *)0x10;\n");
        assert_eq!(rewrite("n = f(a) + (u8)0x10;\n").0, "n = f(a) + (u8)16;\n");
        assert_eq!(rewrite("Wait(0x10);\n").0, "Wait(16);\n");
    }

    #[test]
    fn fakematch_tagged_statements_and_asm_stay_hex() {
        same("{\n    /* FAKEMATCH: order */\n    a = 0x10;\n}\n");
        same("{\n    a = 0x10; /* FAKEMATCH: order */\n}\n");
        same("void F(void)\n{\n    /* FAKEMATCH: frame */\n    u8 a;\n    a = 0x10;\n}\n");
        same("{\n    register u32 r asm(\"r4\") = 0x10;\n}\n");
        let text = "{\n    x = 1;\n    /* FAKEMATCH: order */\n    a = 0x10;\n    b = 0x10;\n}\n";
        assert_eq!(rewrite(text).0, "{\n    x = 1;\n    /* FAKEMATCH: order */\n    a = 0x10;\n    b = 16;\n}\n");
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
        same("#if VERSION == 0x10\n#endif\nchar *s = \"0x10\";\n/* 0x10 */\nasm(\"mov r0, #0x10\" : : \"i\"(0x10));\n");
    }

    #[test]
    fn definitions_are_never_rewritten() {
        same("#define COUNT 0x30\n#define SHEET(w) ((w)->sheet + 0xC56)\n#define LONG (0x20 + \\\n    0x8)\n");
        same("#define DJINN_ENTRY_OWNER 0xf00\n#define DJINN_ENTRY_ELEMENT 0xe0\n#define SOUNDCNT_X_ALL_TONES_ON 0x0f\n");
        same("#define KEYCNT_SOFT_RESET 0xc00f\n#define WAITCNT_GAME 0x4014\n#define HEX_DIGIT 0xf0e0\n");
        same("#define PAGE_CAP_LEFT 0xf128\n#define ELEMENT_TILE 0x5001\n");
        assert_eq!(rewrite("#define COUNT 0x30\nx = 0x10;\n").0, "#define COUNT 0x30\nx = 16;\n");
    }

    #[test]
    fn decimal_and_octal_literals_are_left_alone() {
        same("x = 16 + 010 + 'a';\n");
    }
}
