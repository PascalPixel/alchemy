//! Hex literals that are quantities, written in decimal. A literal is
//! converted only where the code shows it is a quantity: an argument of a
//! scene command from EVENT.H (whose coordinates are pixels, S3), a loop
//! bound compared with the loop counter, a constant array index, or an
//! operand of plain arithmetic (+ - * /) in a statement with no bit
//! operation, shift, register or fixed address, among names that never meet
//! one. Everything else stays as written: case labels, register and control
//! words, blend values, packed byte pairs, flags and masks, values from
//! 0x1000 up, fixed point, the all-ones limits of a width, and Nintendo's
//! library files (the caller skips those). Preprocessor lines (every
//! `#define` body among them), strings, comments, asm and FAKEMATCH-tagged
//! statements are never touched. Every converted value is below 0x1000, so
//! it has type int (or its suffix's type) in either spelling and the bytes
//! cannot change.

use super::tagged;
use crate::permute::lex::{lex, Tok, Token};
use std::collections::HashSet;
use std::ops::Range;
use std::path::Path;

const ADDRESS: u64 = 0x0200_0000;
/// Values from here up stay hex in code.
const QUANTITY_LIMIT: u64 = 0x1000;
const BITWISE: &[&str] = &["&", "|", "^", "~", "<<", ">>", "&=", "|=", "^=", "<<=", ">>="];
const LIMITS: &[u64] = &[0x7F, 0xFF, 0x7FFF, 0xFFFF, 0xFF_FFFF];
const COMPARISONS: &[&str] = &["<", "<=", ">", ">=", "!="];
const TYPES: &[&str] = &[
    "u8", "s8", "u16", "s16", "u32", "s32", "int", "unsigned", "signed", "char", "short", "long", "void", "const", "volatile", "struct",
    "union", "return", "sizeof", "if", "while", "for", "do", "else", "switch", "case",
];

struct Hex {
    value: u64,
    digits: usize,
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
    Some(Hex { value, digits, suffix: suffix.to_string() })
}

/// A Q-format constant: whole units (a multiple of 0x100 from 0x1000 up), or
/// a binary fraction such as 0x6666 (0.4) or 0x19999 (1.6), whose low three
/// hex digits repeat.
fn fixed_point(value: u64) -> bool {
    let low = value & 0xFFF;
    value >= 0x1000 && (value % 0x100 == 0 || (low >> 8 == low & 0xF && (low >> 4) & 0xF == low & 0xF))
}

/// Two small bytes packed in one word: a blend pair (0x80c) or a sprite
/// shape (0x0808).
fn packed(digits: usize, value: u64) -> bool {
    let (high, low) = (value >> 8, value & 0xFF);
    value >= 0x100 && low != 0 && ((digits == 4 && high <= 0x1F && low <= 0x1F) || ((2..=0x10).contains(&high) && low <= 0x10))
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
    if name.starts_with("REG") {
        return true;
    }
    let lower = name.to_ascii_lowercase();
    ["flag", "mask", "bits", "attr", "blend", "control", "cnt"].iter().any(|word| lower.contains(word))
}

/// A name that writes hardware: an Io_ service, a register, a DMA or a
/// queued register write.
fn hardware(name: &str) -> bool {
    name.starts_with("Io_") || name.starts_with("REG") || name.starts_with("Dma_") || name == "QueueIoWrite" || flag_name(name)
}

fn says_flag(name: &str) -> bool {
    name.to_ascii_lowercase().contains("flag")
}

/// Scene command parameters that hold quantities: actor numbers, pixel
/// coordinates and frame counts. Emotes, messages, sounds, modes,
/// directions and speeds are ids or units and keep their base.
const QUANTITY_PARAMETERS: &[&str] = &["no", "to", "x", "z", "t"];

/// The quantity arguments of the scene commands, as `NAME/index`: the
/// parameters of the all-caps function-like macros an EVENT.H defines that
/// hold actor numbers, coordinates and frames (every argument of `WAIT`).
/// The flag commands, whose arguments are flag ids, have none.
pub(crate) fn commands(event_h: &str) -> Vec<String> {
    let mut out = Vec::new();
    for line in event_h.lines() {
        let Some(rest) = line.trim_start().strip_prefix('#').and_then(|r| r.trim_start().strip_prefix("define")).map(str::trim_start) else {
            continue;
        };
        let name: String = rest.chars().take_while(|c| c.is_ascii_uppercase() || c.is_ascii_digit() || *c == '_').collect();
        let Some(parameters) = rest[name.len()..].strip_prefix('(').and_then(|r| r.split_once(')')).map(|(p, _)| p) else {
            continue;
        };
        if name.is_empty() || says_flag(&name) {
            continue;
        }
        for (index, parameter) in parameters.split(',').map(str::trim).enumerate() {
            if name == "WAIT" || QUANTITY_PARAMETERS.contains(&parameter) {
                out.push(format!("{name}/{index}"));
            }
        }
    }
    out
}

/// The scene commands of ☀️'s EVENT.H.
pub(crate) fn scene_commands(root: &Path) -> Vec<String> {
    std::fs::read_to_string(root.join("games/THE BROKEN SEAL/INCLUDE/EVENT.H")).map(|text| commands(&text)).unwrap_or_default()
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

/// The statement around token `i`: between the `;`, `{` or `}` before it and
/// the one after it.
fn statement(tokens: &[Token], i: usize) -> Range<usize> {
    let boundary = |t: &Token| matches!(punct(t), Some(";" | "{" | "}"));
    let start = (0..i).rev().find(|&k| boundary(&tokens[k])).map_or(0, |k| k + 1);
    let end = (i..tokens.len()).find(|&k| boundary(&tokens[k])).unwrap_or(tokens.len());
    start..end
}

/// Whether a span holds a bit operation, a hardware name or a fixed address.
fn touches_bits(tokens: &[Token], span: Range<usize>) -> bool {
    tokens[span].iter().any(|t| {
        punct(t).is_some_and(|p| BITWISE.contains(&p))
            || ident(t).is_some_and(hardware)
            || matches!(&t.tok, Tok::Number(n) if hex(n).is_some_and(|h| h.value >= ADDRESS))
    })
}

/// Whether a span holds a hardware name or a fixed address.
fn touches_hardware(tokens: &[Token], span: Range<usize>) -> bool {
    tokens[span].iter().any(|t| {
        ident(t).is_some_and(|n| n.starts_with("Io_") || n.starts_with("REG") || n.starts_with("Dma_") || n == "QueueIoWrite")
            || matches!(&t.tok, Tok::Number(n) if hex(n).is_some_and(|h| h.value >= ADDRESS))
    })
}

/// The variables and fields of a span: names that are not called, types or
/// keywords.
fn names(tokens: &[Token], span: Range<usize>) -> impl Iterator<Item = &str> + '_ {
    span.filter_map(move |k| {
        let name = ident(&tokens[k])?;
        let called = tokens.get(k + 1).and_then(punct) == Some("(");
        (!called && !TYPES.contains(&name)).then_some(name)
    })
}

/// Every variable or field that meets a bit operation, a register or a fixed
/// address anywhere in the file: it holds a word, not a quantity.
fn tainted(tokens: &[Token]) -> HashSet<String> {
    let mut out = HashSet::new();
    let mut start = 0;
    for k in 0..=tokens.len() {
        if k == tokens.len() || matches!(punct(&tokens[k]), Some(";" | "{" | "}")) {
            if touches_bits(tokens, start..k) {
                out.extend(names(tokens, start..k).map(str::to_string));
            }
            start = k + 1;
        }
    }
    out
}

fn operand_end(token: &Token) -> bool {
    matches!(&token.tok, Tok::Ident(_) | Tok::Number(_) | Tok::Char(_)) || matches!(punct(token), Some(")" | "]"))
}

/// Whether token `i` sits in a `case` label.
fn in_case_label(tokens: &[Token], span: Range<usize>, i: usize) -> bool {
    (span.start..i).rev().take_while(|&k| punct(&tokens[k]) != Some(":")).any(|k| matches!(ident(&tokens[k]), Some("case" | "default")))
}

/// A whole argument of a scene command: `OBJ_POS_SET(8, 0xC0, -0x108)`.
fn command_argument(tokens: &[Token], groups: &[Option<usize>], i: usize, commands: &[String]) -> bool {
    let Some(open) = groups[i] else {
        return false;
    };
    let Some(name) = open.checked_sub(1).and_then(|k| ident(&tokens[k])) else {
        return false;
    };
    if punct(&tokens[open]) != Some("(") {
        return false;
    }
    let first = if i >= 1 && punct(&tokens[i - 1]) == Some("-") { i - 1 } else { i };
    let opens = first.checked_sub(1).is_some_and(|k| k == open || (groups[k] == Some(open) && punct(&tokens[k]) == Some(",")));
    let index = (open + 1..first).filter(|&k| groups[k] == Some(open) && punct(&tokens[k]) == Some(",")).count();
    opens && tokens.get(i + 1).and_then(punct).is_some_and(|p| matches!(p, "," | ")")) && commands.iter().any(|c| *c == format!("{name}/{index}"))
}

/// A loop bound compared with the counter its `for` steps:
/// `for (i = 0; i < 0x10; i++)`.
fn loop_bound(tokens: &[Token], groups: &[Option<usize>], i: usize) -> bool {
    let Some(open) = groups[i] else {
        return false;
    };
    if punct(&tokens[open]) != Some("(") || open.checked_sub(1).and_then(|k| ident(&tokens[k])) != Some("for") {
        return false;
    }
    let inside: Vec<usize> = (open + 1..tokens.len()).take_while(|&k| groups[k].is_some_and(|g| g >= open)).collect();
    let semis: Vec<usize> = inside.iter().copied().filter(|&k| groups[k] == Some(open) && punct(&tokens[k]) == Some(";")).collect();
    let (Some(&first), Some(&second), Some(&last)) = (semis.first(), semis.get(1), inside.last()) else {
        return false;
    };
    if !(first < i && i < second) {
        return false;
    }
    let ends = |t: Option<&Token>| t.and_then(punct).is_some_and(|p| matches!(p, ";" | "&&" | "||" | ")"));
    let counter = if i >= 2 && tokens[i - 1].tok != Tok::Punct("-") && punct(&tokens[i - 1]).is_some_and(|p| COMPARISONS.contains(&p)) && ends(tokens.get(i + 1)) {
        ident(&tokens[i - 2]).filter(|_| i >= 3 && matches!(punct(&tokens[i - 3]), Some(";" | "&&" | "||" | "(")))
    } else if tokens.get(i + 1).and_then(punct).is_some_and(|p| COMPARISONS.contains(&p)) && ends(tokens.get(i + 3)) && matches!(punct(&tokens[i - 1]), Some(";" | "&&" | "||" | "(")) {
        tokens.get(i + 2).and_then(ident)
    } else {
        None
    };
    let Some(counter) = counter.filter(|c| !flag_name(c)) else {
        return false;
    };
    let step = second + 1..last + 1;
    let steps = step.clone().any(|k| {
        ident(&tokens[k]) == Some(counter)
            && (tokens.get(k + 1).and_then(punct).is_some_and(|p| matches!(p, "++" | "--" | "+=" | "-=" | "="))
                || k.checked_sub(1).and_then(|j| punct(&tokens[j])).is_some_and(|p| matches!(p, "++" | "--")))
    });
    steps && !touches_bits(tokens, step)
}

/// Whether token `i` sits in the brackets of a declarator (`u8 pad[0x24]`,
/// `u16 pages[2][0x3c0]`): a size, written as the layout's offsets are.
fn in_declarator(tokens: &[Token], groups: &[Option<usize>], i: usize) -> bool {
    let mut group = groups[i];
    while let Some(open) = group {
        if punct(&tokens[open]) == Some("[") {
            let mut k = open;
            while k >= 1 && punct(&tokens[k - 1]) == Some("]") {
                let mut depth = 0;
                let mut j = k - 1;
                loop {
                    match punct(&tokens[j]) {
                        Some("]") => depth += 1,
                        Some("[") => {
                            depth -= 1;
                            if depth == 0 {
                                break;
                            }
                        }
                        _ => {}
                    }
                    if j == 0 {
                        return true;
                    }
                    j -= 1;
                }
                k = j;
            }
            if k >= 2 && ident(&tokens[k - 1]).is_some() && (ident(&tokens[k - 2]).is_some() || punct(&tokens[k - 2]) == Some("*")) {
                return true;
            }
        }
        group = groups[open];
    }
    false
}

/// A statement that casts to a pointer: its offsets are byte offsets into a
/// layout, written in hex as the layout is.
fn address_arithmetic(tokens: &[Token], span: Range<usize>) -> bool {
    span.clone().any(|k| punct(&tokens[k]) == Some("*") && tokens.get(k + 1).and_then(punct) == Some(")"))
}

/// A name whose values are in fixed-point or angle units, written in hex.
fn in_units(name: &str) -> bool {
    let lower = name.to_ascii_lowercase();
    ["scale", "angle", "rot", "speed", "vel", "accel", "dir", "alpha", "fixed", "frac"].iter().any(|word| lower.contains(word))
}

/// A constant index into a named array: `a[0x1F]`, `b[2][0x3]`.
fn constant_index(tokens: &[Token], i: usize) -> bool {
    if i < 2 || punct(&tokens[i - 1]) != Some("[") || tokens.get(i + 1).and_then(punct) != Some("]") {
        return false;
    }
    let named = ident(&tokens[i - 2]).is_some_and(|n| !flag_name(n) && !hardware(n)) || punct(&tokens[i - 2]) == Some("]");
    named && !touches_hardware(tokens, statement(tokens, i))
}

/// An operand of plain arithmetic in a statement no bit operation, register
/// or fixed address touches, among variables that never meet one.
fn arithmetic(tokens: &[Token], i: usize, tainted: &HashSet<String>) -> bool {
    let before = i.checked_sub(1).and_then(|k| punct(&tokens[k]));
    let binary_before = match before {
        Some("+=" | "-=" | "*=" | "/=") => true,
        Some("+" | "-" | "*" | "/") => i >= 2 && operand_end(&tokens[i - 2]),
        _ => false,
    };
    let binary_after = tokens.get(i + 1).and_then(punct).is_some_and(|p| matches!(p, "+" | "-" | "*" | "/")) && before != Some(")");
    if !(binary_before || binary_after) {
        return false;
    }
    let span = statement(tokens, i);
    if in_case_label(tokens, span.clone(), i) || touches_bits(tokens, span.clone()) || address_arithmetic(tokens, span.clone()) {
        return false;
    }
    !names(tokens, span).any(|n| tainted.contains(n) || flag_name(n) || in_units(n))
}

/// Values beside a fixed-point constant in the same argument or initializer
/// list are fixed point too.
fn beside_fixed_point(tokens: &[Token], groups: &[Option<usize>], i: usize) -> bool {
    let Some(group) = groups[i] else {
        return false;
    };
    let list = match punct(&tokens[group]) {
        Some("(") => true,
        Some("{") => group.checked_sub(1).and_then(|k| punct(&tokens[k])).is_some_and(|p| matches!(p, "=" | "," | "{")),
        _ => false,
    };
    list && tokens.iter().enumerate().any(|(k, t)| {
        k != i && groups[k] == Some(group) && matches!(&t.tok, Tok::Number(n) if hex(n).is_some_and(|h| fixed_point(h.value) && h.value < ADDRESS))
    })
}

fn quantity(tokens: &[Token], groups: &[Option<usize>], i: usize, h: &Hex, commands: &[String], tainted: &HashSet<String>) -> bool {
    if h.value >= QUANTITY_LIMIT || fixed_point(h.value) || LIMITS.contains(&h.value) || packed(h.digits, h.value) {
        return false;
    }
    if flag_argument(tokens, groups, i) || beside_fixed_point(tokens, groups, i) {
        return false;
    }
    if in_declarator(tokens, groups, i) {
        return false;
    }
    // Arithmetic only below 0x400 and off whole multiples of 0x100, which
    // are fixed-point steps.
    let small = h.value < 0x400 && h.value % 0x100 != 0;
    command_argument(tokens, groups, i, commands) || loop_bound(tokens, groups, i) || constant_index(tokens, i) || (small && arithmetic(tokens, i, tainted))
}

/// The text with its quantities in decimal, and how many changed. `commands`
/// are the scene commands whose arguments are quantities.
pub(crate) fn rewrite(text: &str, commands: &[String]) -> (String, usize) {
    let Ok(tokens) = lex(text) else {
        return (text.to_string(), 0);
    };
    let tokens = code(tokens);
    let groups = groups(&tokens);
    let tainted = tainted(&tokens);
    let tagged = tagged::ranges(text);
    let mut edits: Vec<(usize, usize, String)> = Vec::new();
    for (i, token) in tokens.iter().enumerate() {
        let Tok::Number(number) = &token.tok else {
            continue;
        };
        let Some(h) = hex(number) else {
            continue;
        };
        if !quantity(&tokens, &groups, i, &h, commands, &tainted) || tagged::covered(&tagged, token.start, token.end) {
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

    fn scene() -> Vec<String> {
        commands(include_str!("../../../../games/THE BROKEN SEAL/INCLUDE/EVENT.H"))
    }

    fn convert(text: &str) -> (String, usize) {
        rewrite(text, &scene())
    }

    fn same(text: &str) {
        assert_eq!(convert(text), (text.to_string(), 0), "{text}");
    }

    #[test]
    fn scene_commands_are_event_h_macros_but_the_flag_ones() {
        let found = scene();
        for quantity in ["OBJ_POS_SET/0", "OBJ_POS_SET/1", "OBJ_POS_SET/2", "WAIT/0", "OBJ_EMO/2", "OBJ_FACE/1"] {
            assert!(found.iter().any(|c| c == quantity), "{quantity} {found:?}");
        }
        assert!(!found.iter().any(|c| c.starts_with("FLAG") || c.starts_with("EVENT_H") || c.starts_with("SE/")), "{found:?}");
        assert!(!found.iter().any(|c| matches!(c.as_str(), "OBJ_EMO/1" | "OBJ_SPD/1" | "OBJ_DIR/1" | "MES_SET/0")), "{found:?}");
        same("OBJ_EMO(14, 0x100, 0);\nSE(0x7B);\nOBJ_DIR(13, 0x20, 0);\n");
        assert_eq!(convert("OBJ_EMO(14, 0x100, 0x3c);\n").0, "OBJ_EMO(14, 0x100, 60);\n");
    }

    #[test]
    fn quantities_become_decimal_with_their_sign_and_suffix() {
        let text = "void F(void)\n{\n    OBJ_POS_SET(8, 0xC0, -0x108);\n    a[0x1F] = b + 0x10u;\n    for (i = 0; i < 0x2A; i++) {}\n}\n";
        let (out, count) = convert(text);
        assert_eq!(count, 5);
        assert_eq!(out, "void F(void)\n{\n    OBJ_POS_SET(8, 192, -264);\n    a[31] = b + 16u;\n    for (i = 0; i < 42; i++) {}\n}\n");
    }

    #[test]
    fn plain_stores_calls_and_case_labels_stay_as_written() {
        same("x = 0x10;\nWait(0x10);\nAudio_PlayCue(0x7B);\nswitch (x) { case 0x2A: break; case 0x10 + 1: break; }\n");
    }

    #[test]
    fn bit_operations_masks_and_their_variables_stay_hex() {
        same("x = y & 0x3F;\nx |= 0x40;\nx &= ~0x4;\nz = 0x3 << 4;\nw = v >> 0x8;\nif ((a & 0xF0) == 0x80) {}\n");
        same("if (0x20 == (b & 0x30)) {}\nactor->flags = 0x204;\nif (gFlags != 0x11) {}\nt = 0xC | u;\n");
        same("n = m + 0x10;\nREG_BG0CNT = n;\n");
        same("k = j * 0x20;\nk |= 0x4;\n");
        same("for (bit = 1; bit < 0x80; bit <<= 1) {}\nfor (flag = 0x200; flag <= 0x2ff; flag++) {}\n");
    }

    #[test]
    fn addresses_fixed_point_packed_pairs_and_limits_stay_hex() {
        same("p = (u16 *)0x04000000 + 0x10;\nq = 0x03007FF0;\nspeed = s + 0x18000;\nx = y + 0xFF;\nREG_BLDALPHA = 0x1010;\n");
        same("OBJ_SPD(8, 0x19999, 0x6666);\nOBJ_SPD(12, 0x10000, 0x8);\nOBJ_WALK(9, 0x80c, 0x0808);\n");
        assert_eq!(convert("OBJ_WALK(9, 0x15f8, 0x6f8);\n").0, "OBJ_WALK(9, 0x15f8, 1784);\n");
    }

    #[test]
    fn layout_sizes_offsets_and_units_stay_hex() {
        same("struct S {\n    u8 pad24[0x50 - 0x24];\n    u16 pages[2][0x3c0];\n};\nu8 *unknown_534[0x10];\n");
        same("slot = (u16 *)(state + 0x534);\n*(s32 *)(base[19] + 0x1c0) = 0x209;\n");
        same("o->scale_y -= 0x80;\nrecord->angle -= 0x80;\nsize += 0xc00;\ntransfer += 0x100;\n");
        assert_eq!(convert("work[0x294] = 8;\nid += 0x78;\n").0, "work[660] = 8;\nid += 120;\n");
    }

    #[test]
    fn flag_ids_stay_hex() {
        same("GameFlag_TestFar(0x16b);\nFLAG_SET(0x943);\nif (FLAG_CHK(0x214)) {}\nEngine_GameFlagSet(0xf2a);\n");
        same("x = GameFlag_Test(a + 0x10, 0x2);\nf = gPlacementsFlag[0x14e];\nCall2(GameFlag_SetByte, s + 0x370, 1);\n");
    }

    /// The lines reviewers showed decimalized by the earlier rule, as they
    /// stand in the tree.
    #[test]
    fn reviewed_words_stay_hex() {
        // BATTLE/EFFECT/DESCRIPT.C:179-194
        same("range = flags & 0x600;\nswitch (range) {\ncase 0:\n    accepted = 1;\n    break;\ncase 0x200:\n    break;\ncase 0x400:\n    flags &= 0xf000;\n    accepted = (u16)((flags - reference) + 0x1fff) <= 0x3ffe;\n    break;\ncase 0x600:\n    break;\n}\n");
        // BATTLE/EFFECT/DESCRIPT.C:380-384
        same("kind = desc2->flags & 0x30;\nswitch (kind) {\ncase 0: Audio_PlayCue(0x7B); break;\ncase 32: Audio_PlayCue(0x80); break;\n}\n");
        // MENU/ITEM/STATUS.C:182,203,215,227
        same("switch (key) {\ncase 0x40:\n    break;\ncase 0x80:\n    break;\ncase 0x20:\n    break;\ncase 0x10:\n    break;\n}\n");
        // MAP/LOADGFX.C:32,61; MAP/PERSPECT.C:152; SYSTEM/IO.C:82
        same("control = 0x682;\nmode = 0x501;\nIo_Set16(0x501, &REG_BG1CNT);\nu32 control = 0x84;\n");
        // SYSTEM/MEMORY/MEMORY.C:37
        same("{\n    s32 control = 0x400;\n\n    REG_BG0CNT = control;\n}\n");
        // BATTLE/EFFECT/OBJECT_ROW.C:90
        same("REG16(0x0400000c) = 0x784;\nREG16(0x0400000c) = 0x84 + n;\n");
        // TOREBI_IZUMI/IZUMI.C:144; WORLD_MAP/STORY.C:573-619
        same("do { Io_SetBlendAlpha(0x80c); } while (0);\ngWorldMapBlend = 0xe00;\nQueueFrame(buffer, 0x3480)\ngWorldMapBlend = 0xd00;\n");
        // GRAPHICS/DISPLAY/BLEND.C:18-48; MINIGAME/REEL_INIT_TITLE.C:108
        same("gBlendLayers = 0x3e;\ngBlendTargetLevel = 0x10;\nblend = 0x400;\nblend = blend + 0x10;\n");
        // SYSTEM/LINK/SIO_INTR.C:179; SOUND/ENGINE.C:522
        same("state->status = 0x80;\n*reg0 = 0x40;\nREG_WAVE_RAM[0] = note->wave[0];\n");
        // SYSTEM/RESOURCE/RESOBJ.C:71-72
        same("switch (size) {\ncase 0x0808: shape = 0; break;\ncase 0x0810: shape = 0x8000; break;\n}\n");
    }

    #[test]
    fn fakematch_tagged_statements_and_asm_stay_hex() {
        same("{\n    /* FAKEMATCH: order */\n    a = b + 0x10;\n}\n");
        same("{\n    a = b + 0x10; /* FAKEMATCH: order */\n}\n");
        same("void F(void)\n{\n    /* FAKEMATCH: frame */\n    u8 a;\n    a = b + 0x10;\n}\n");
        same("{\n    register u32 r asm(\"r4\") = 0x10;\n}\n");
        let text = "{\n    x = 1;\n    /* FAKEMATCH: order */\n    a = c + 0x10;\n    b = c + 0x10;\n}\n";
        assert_eq!(convert(text).0, "{\n    x = 1;\n    /* FAKEMATCH: order */\n    a = c + 0x10;\n    b = c + 16;\n}\n");
    }

    #[test]
    fn directives_strings_comments_and_asm_are_untouched() {
        same("#if VERSION == 0x10\n#endif\nchar *s = \"0x10\";\n/* 0x10 */\nasm(\"mov r0, #0x10\" : : \"i\"(0x10));\n");
        same("#define COUNT 0x30\n#define SHEET(w) ((w)->sheet + 0xC56)\n#define LONG (0x20 + \\\n    0x8)\n");
        assert_eq!(convert("#define COUNT 0x30\nx = y + 0x10;\n").0, "#define COUNT 0x30\nx = y + 16;\n");
    }

    #[test]
    fn decimal_and_octal_literals_are_left_alone() {
        same("x = 16 + 010 + 'a';\n");
    }
}
