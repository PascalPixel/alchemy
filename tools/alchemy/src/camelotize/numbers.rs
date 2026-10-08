//! Hex literals that are quantities, written in decimal. A literal is
//! converted only where the code shows it is a quantity: an argument of a
//! scene command from EVENT.H (whose coordinates are pixels, S3), a loop
//! bound compared with the loop counter, or an operand of plain arithmetic
//! (+ - * /) in a statement with no bit operation, shift, register, fixed
//! address or subscript, among names that never meet one, beside operands
//! every edition types as numbers. Everything else stays as written: case
//! labels, register and control words, blend values, packed byte pairs,
//! flags and masks, values from 0x1000 up, fixed point, angles, subscripts,
//! byte offsets and layout sizes, pointer offsets, character codes, the
//! all-ones limits of a width, and Nintendo's library files (the caller
//! skips those). One quantity keeps one base in a function. Preprocessor
//! lines (every `#define` body among them), strings, comments, asm and
//! FAKEMATCH-tagged code are never touched. Every converted value is below 0x1000, so
//! it has type int (or its suffix's type) in either spelling and the bytes
//! cannot change.

use super::tagged;
use crate::permute::lex::{lex, Tok, Token};
use crate::permute::parse::{matching, scan_definitions};
use crate::targets::DecompTarget;
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
    value >= 0x1000 && (value.is_multiple_of(0x100) || (low >> 8 == low & 0xF && (low >> 4) & 0xF == low & 0xF))
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
    ["scale", "ang", "rot", "speed", "vel", "accel", "dir", "alpha", "fixed", "frac", "theta", "yaw", "turn", "facing", "heading"]
        .iter()
        .any(|word| lower.contains(word))
}

/// The words of a name, lowercase, split at underscores and case steps
/// (`object_id`, `objId` and `OBJ_ID` are each `obj(ect)` and `id`).
fn words(name: &str) -> Vec<String> {
    let mut words: Vec<String> = Vec::new();
    let mut word = String::new();
    let mut lower_before = false;
    for ch in name.chars() {
        if ch == '_' || (ch.is_ascii_uppercase() && lower_before) {
            words.push(std::mem::take(&mut word));
        }
        lower_before = ch.is_ascii_lowercase() || ch.is_ascii_digit();
        if ch != '_' {
            word.push(ch.to_ascii_lowercase());
        }
    }
    words.push(word);
    words
}

/// A name for a character or text, by its words: its operands are
/// character codes (`c - 0xde` dakuten, `ch == 0x20`), written in hex.
fn says_character(name: &str) -> bool {
    words(name).iter().any(|w| {
        matches!(
            w.as_str(),
            "c" | "ch" | "chr" | "char" | "chars" | "character" | "characters" | "glyph" | "glyphs" | "kana" | "letter" | "font" | "ascii" | "digit" | "digits" | "text" | "str" | "string" | "msg" | "code" | "codes"
        )
    })
}

/// A name for a byte offset or an id, by its words: its operands are
/// offsets into a layout (`off = (n * 4) + 0x14` before `work + off`) or
/// the bases of id ranges (`id += 0x78` maps 8 onto the enemies' 0x80),
/// written in hex.
fn says_offset_or_id(name: &str) -> bool {
    words(name).iter().any(|w| matches!(w.as_str(), "off" | "offs" | "offset" | "offsets" | "ofs" | "ofst" | "adr" | "addr" | "address" | "id" | "ids"))
}

/// Whether token `i` is in the arguments of a call that plays a sound
/// (`Engine_AudioPlayCue(mode + 0x5a)`): a cue id, not a quantity.
fn sound_argument(tokens: &[Token], groups: &[Option<usize>], i: usize) -> bool {
    let mut group = groups[i];
    while let Some(open) = group {
        let callee = open.checked_sub(1).and_then(|k| ident(&tokens[k]));
        if punct(&tokens[open]) == Some("(")
            && callee.is_some_and(|n| words(n).iter().any(|w| matches!(w.as_str(), "audio" | "sound" | "snd" | "song" | "bgm" | "se" | "sfx" | "cue" | "music" | "voice")))
        {
            return true;
        }
        group = groups[open];
    }
    false
}

/// The names a statement assigns to before token `i` (`step` in
/// `step = object->action * 0x50`): the literal is part of their value.
fn targets(tokens: &[Token], i: usize) -> Vec<&str> {
    let span = statement(tokens, i);
    let Some(assign) = (span.start..i).find(|&k| matches!(punct(&tokens[k]), Some("=" | "+=" | "-=" | "*=" | "/="))) else {
        return Vec::new();
    };
    heads(tokens, span.start..assign)
}

/// The names that hold character codes: those named for characters, and
/// every name assigned from one (`idx = c - 0xde;` makes `idx` a code).
fn characters(tokens: &[Token]) -> HashSet<String> {
    let mut out: HashSet<String> = tokens.iter().filter_map(ident).filter(|n| says_character(n)).map(str::to_string).collect();
    let mut spans = Vec::new();
    let mut start = 0;
    for k in 0..=tokens.len() {
        if k == tokens.len() || matches!(punct(&tokens[k]), Some(";" | "{" | "}")) {
            spans.push(start..k);
            start = k + 1;
        }
    }
    loop {
        let before = out.len();
        for span in &spans {
            let Some(assign) = span.clone().find(|&k| matches!(punct(&tokens[k]), Some("=" | "+=" | "-="))) else {
                continue;
            };
            if names(tokens, assign + 1..span.end).any(|n| out.contains(n)) {
                out.extend(names(tokens, span.start..assign).map(str::to_string));
            }
        }
        if out.len() == before {
            return out;
        }
    }
}

/// Whether token `i` sits inside brackets: a subscript or a declarator's
/// size, a byte offset into a layout (`state[0xcca]`, `pad[0x24]`).
fn in_brackets(tokens: &[Token], groups: &[Option<usize>], i: usize) -> bool {
    let mut group = groups[i];
    while let Some(open) = group {
        if punct(&tokens[open]) == Some("[") {
            return true;
        }
        group = groups[open];
    }
    false
}

/// The `(` or `[` that the closer at `close` matches.
fn opener(tokens: &[Token], close: usize) -> Option<usize> {
    let mut depth = 0i32;
    let mut k = close;
    loop {
        match punct(&tokens[k]) {
            Some(")" | "]" | "}") => depth += 1,
            Some("(" | "[" | "{") => depth -= 1,
            _ => {}
        }
        if depth == 0 {
            return Some(k);
        }
        k = k.checked_sub(1)?;
    }
}

/// The first token of the postfix operand ending at `end` (`a->b[2].c`,
/// `f(x)`, `(a + b)`), or none when it is not one.
fn operand_start(tokens: &[Token], end: usize) -> Option<usize> {
    let mut k = end;
    loop {
        match &tokens[k].tok {
            Tok::Ident(_) | Tok::Number(_) | Tok::Char(_) => {}
            Tok::Punct("]" | ")") => {
                let open = opener(tokens, k)?;
                let callee = open.checked_sub(1).is_some_and(|j| {
                    ident(&tokens[j]).is_some_and(|n| !TYPES.contains(&n)) || matches!(punct(&tokens[j]), Some("]" | ")"))
                });
                if punct(&tokens[k]) == Some(")") && !callee {
                    return Some(open);
                }
                k = open.checked_sub(1)?;
                continue;
            }
            _ => return None,
        }
        match k.checked_sub(1).and_then(|j| punct(&tokens[j])) {
            Some("." | "->") if k >= 2 => k -= 2,
            _ => return Some(k),
        }
    }
}

/// The operands token `i` is added to, subtracted from, compared with or
/// assigned to, as token ranges.
fn partners(tokens: &[Token], i: usize) -> Vec<Range<usize>> {
    const OPERATORS: &[&str] = &["+", "-", "*", "/", "%", "<", "<=", ">", ">=", "==", "!=", "=", "+=", "-=", "*=", "/="];
    let mut out = Vec::new();
    if i >= 2 && punct(&tokens[i - 1]).is_some_and(|p| OPERATORS.contains(&p)) && operand_end(&tokens[i - 2]) {
        if let Some(start) = operand_start(tokens, i - 2) {
            out.push(start..i - 1);
        }
    }
    if tokens.get(i + 1).and_then(punct).is_some_and(|p| OPERATORS.contains(&p) && !matches!(p, "=" | "+=" | "-=" | "*=" | "/=")) {
        if let Some(end) = super::casts::unary_end(tokens, i + 2) {
            out.push(i + 2..end + 1);
        }
    }
    out
}

/// The names an operand is about: its variables and fields outside any
/// subscript or argument list (`a` in `a[i]`, `p` and `x` in `p->x`).
fn heads(tokens: &[Token], range: Range<usize>) -> Vec<&str> {
    let mut depth = 0i32;
    let mut out = Vec::new();
    for k in range {
        match punct(&tokens[k]) {
            Some("(" | "[") => depth += 1,
            Some(")" | "]") => depth -= 1,
            _ => {}
        }
        if let Some(name) = ident(&tokens[k]).filter(|n| depth == 0 && !TYPES.contains(n) && tokens.get(k + 1).and_then(punct) != Some("(")) {
            out.push(name);
        }
    }
    out
}

/// An operand of plain arithmetic in a statement no bit operation, register,
/// fixed address, subscript or character code touches, among variables that
/// never meet one.
fn arithmetic(tokens: &[Token], i: usize, tainted: &HashSet<String>, characters: &HashSet<String>) -> bool {
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
    // A subscript in the statement makes it byte-addressed state or a store
    // of a code (`out[0] = val + 0x30`).
    if tokens[span.clone()].iter().any(|t| punct(t) == Some("[")) {
        return false;
    }
    !names(tokens, span).any(|n| {
        tainted.contains(n) || flag_name(n) || in_units(n) || says_offset_or_id(n) || characters.contains(n) || n.starts_with("unknown") || n.starts_with("pad")
    })
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

struct Context<'a> {
    tokens: &'a [Token],
    groups: Vec<Option<usize>>,
    commands: &'a [String],
    tainted: HashSet<String>,
    characters: HashSet<String>,
}

fn quantity(c: &Context, i: usize, h: &Hex) -> bool {
    let (tokens, groups) = (c.tokens, &c.groups[..]);
    if h.value >= QUANTITY_LIMIT || fixed_point(h.value) || LIMITS.contains(&h.value) || packed(h.digits, h.value) {
        return false;
    }
    if flag_argument(tokens, groups, i) || sound_argument(tokens, groups, i) || beside_fixed_point(tokens, groups, i) {
        return false;
    }
    if in_declarator(tokens, groups, i) || in_brackets(tokens, groups, i) {
        return false;
    }
    // A loop over character codes or units keeps its base.
    let plain = |span: Range<usize>| !names(tokens, span).any(|n| c.characters.contains(n) || in_units(n) || says_offset_or_id(n) || n.starts_with("unknown"));
    // Arithmetic only below 0x400 and off whole multiples of 0x100, which
    // are fixed-point steps.
    let small = h.value < 0x400 && !h.value.is_multiple_of(0x100);
    command_argument(tokens, groups, i, c.commands)
        || (loop_bound(tokens, groups, i) && plain(statement(tokens, i)))
        || (small && arithmetic(tokens, i, &c.tainted, &c.characters))
}

/// The top-level block each token sits in, as the index of its `{`: one
/// function.
fn functions(tokens: &[Token]) -> Vec<Option<usize>> {
    let mut depth = 0usize;
    let mut open = None;
    let mut out = Vec::with_capacity(tokens.len());
    for (k, token) in tokens.iter().enumerate() {
        match punct(token) {
            Some("{") => {
                if depth == 0 {
                    open = Some(k);
                }
                depth += 1;
                out.push(open);
            }
            Some("}") => {
                depth = depth.saturating_sub(1);
                out.push(open);
                if depth == 0 {
                    open = None;
                }
            }
            _ => out.push(open),
        }
    }
    out
}

/// A literal found to be a quantity: its byte range and decimal spelling,
/// and the operands it is added to or subtracted from (byte ranges), which
/// must be numbers by their declared type, not addresses (`state + 0x1c0`).
pub(crate) struct Found {
    start: usize,
    end: usize,
    decimal: String,
    operands: Vec<Range<usize>>,
    /// The function it sits in and its value: the same value kept hex in
    /// that function keeps it hex.
    value: (Option<usize>, u64),
}

/// The quantities of `text`, before typing, and the values that stay hex in
/// each function, tagged code aside. One quantity keeps one base within a
/// function: a literal stays hex when a name it meets also meets a literal
/// that stays hex there (`len > 0x10` keeps `len - 0x10`), or when its value
/// stays hex there (`id = 0x80;` keeps `limit += 0x80;`, `BumpAllocate(0x400)`
/// keeps `i < 0x400`).
pub(crate) fn found(text: &str, commands: &[String]) -> (Vec<Found>, HashSet<(Option<usize>, u64)>) {
    let Ok(tokens) = lex(text) else {
        return (Vec::new(), HashSet::new());
    };
    let tokens = code(tokens);
    let context = Context { groups: groups(&tokens), commands, tainted: tainted(&tokens), characters: characters(&tokens), tokens: &tokens };
    let tagged = tagged::ranges(text);
    let functions = functions(&tokens);
    let related = |i: usize| -> HashSet<&str> { partners(&tokens, i).into_iter().flat_map(|r| heads(&tokens, r)).collect() };
    // Each hex literal: its token, value, whether a tag covers it, and the
    // literal while it is still a candidate.
    let mut hexes: Vec<(usize, u64, bool, Option<Hex>)> = Vec::new();
    for (i, token) in tokens.iter().enumerate() {
        let Tok::Number(number) = &token.tok else {
            continue;
        };
        let Some(h) = hex(number) else {
            continue;
        };
        let tag = tagged::covered(&tagged, token.start, token.end);
        let keep = tag || !quantity(&context, i, &h);
        hexes.push((i, h.value, tag, (!keep).then_some(h)));
    }
    // The values kept hex in each function; a tag keeps its own literals
    // only.
    let kept_values = |hexes: &[(usize, u64, bool, Option<Hex>)]| -> HashSet<(Option<usize>, u64)> {
        hexes.iter().filter(|(_, _, tag, h)| h.is_none() && !tag).map(|(i, value, _, _)| (functions[*i], *value)).collect()
    };
    loop {
        let mut kept: HashSet<(Option<usize>, &str)> = HashSet::new();
        for (i, _, _, h) in &hexes {
            if h.is_none() {
                kept.extend(related(*i).into_iter().map(|n| (functions[*i], n)));
            }
        }
        let values = kept_values(&hexes);
        let mut changed = false;
        for (i, value, _, h) in hexes.iter_mut() {
            // A candidate also meets the names its statement assigns: in
            // `step = object->action * 0x50; object->angle += step + 0x1000;`
            // the step is an angle, kept hex.
            if h.is_some()
                && (values.contains(&(functions[*i], *value))
                    || related(*i).into_iter().chain(targets(&tokens, *i)).any(|n| kept.contains(&(functions[*i], n))))
            {
                *h = None;
                changed = true;
            }
        }
        if !changed {
            break;
        }
    }
    let values = kept_values(&hexes);
    let found = hexes
        .into_iter()
        .filter_map(|(i, value, _, h)| {
            let h = h?;
            let added = matches!(i.checked_sub(1).and_then(|k| punct(&tokens[k])), Some("+" | "-" | "+=" | "-="))
                || matches!(tokens.get(i + 1).and_then(punct), Some("+" | "-"));
            let operands = if added { partners(&tokens, i).into_iter().map(|r| tokens[r.start].start..tokens[r.end - 1].end).collect() } else { Vec::new() };
            Some(Found { start: tokens[i].start, end: tokens[i].end, decimal: format!("{}{}", h.value, h.suffix), operands, value: (functions[i], value) })
        })
        .collect();
    (found, values)
}

/// The text with its quantities in decimal, and how many changed. `address`
/// says whether the operand at a byte range (its start, its text) is an
/// address or cannot be typed: a literal beside one stays hex, and so does
/// its value everywhere in that function (`buffer + 0x480` keeps
/// `i < 0x480`).
pub(crate) fn rewrite_with(text: &str, commands: &[String], address: &mut dyn FnMut(usize, &str) -> bool) -> (String, usize) {
    let (found, mut values) = found(text, commands);
    let (mut edits, beside): (Vec<Found>, Vec<Found>) = found.into_iter().partition(|f| !f.operands.iter().any(|r| address(r.start, &text[r.clone()])));
    values.extend(beside.iter().map(|f| f.value));
    edits.retain(|f| !values.contains(&f.value));
    let count = edits.len();
    let mut out = text.to_string();
    for f in edits.into_iter().rev() {
        out.replace_range(f.start..f.end, &f.decimal);
    }
    (out, count)
}

/// As `rewrite_with`, typing each operand in every edition of the games the
/// file builds in: a literal stays hex beside an operand any edition types
/// as an address, or none can type.
pub(crate) fn rewrite(root: &Path, file: &Path, text: &str, games: &[DecompTarget], commands: &[String]) -> Result<(String, usize), String> {
    let (candidates, _) = found(text, commands);
    if candidates.is_empty() {
        return Ok((text.to_string(), 0));
    }
    let mut editions = Vec::new();
    if candidates.iter().any(|f| !f.operands.is_empty()) {
        let relative = file.strip_prefix(root).unwrap_or(file).to_string_lossy().into_owned();
        for target in super::trial::editions(games) {
            let preprocessed = crate::compiler::preprocess::Expansion::new(root, target)?.fresh(&relative)?;
            editions.push(super::casts::Edition::new(preprocessed)?);
        }
    }
    let tokens = code(lex(text).map_err(|e| e.to_string())?);
    let bodies: Vec<(String, usize, usize)> = scan_definitions(&tokens)
        .into_iter()
        .filter_map(|(name, _, _, _, open)| Some((name, tokens[open].start, tokens[matching(&tokens, open)?].end)))
        .collect();
    Ok(rewrite_with(text, commands, &mut |at, operand| {
        let function = bodies.iter().find(|(_, start, stop)| *start <= at && at <= *stop).map_or("", |b| b.0.as_str());
        let mut read = false;
        for edition in editions.iter_mut() {
            match edition.address(function, operand) {
                Some(true) => return true,
                Some(false) => read = true,
                None => {}
            }
        }
        !read
    }))
}

#[cfg(test)]
mod tests {
    use super::*;

    fn scene() -> Vec<String> {
        commands(include_str!("../../../../games/THE BROKEN SEAL/INCLUDE/EVENT.H"))
    }

    /// The names these tests declare addresses: pointers into state and
    /// tables.
    fn convert(text: &str) -> (String, usize) {
        rewrite_with(text, &scene(), &mut |_, operand| {
            operand.split(|c: char| !c.is_ascii_alphanumeric() && c != '_').any(|w| matches!(w, "state" | "table" | "cursor" | "p"))
        })
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
        let text = "void F(void)\n{\n    OBJ_POS_SET(8, 0xC0, -0x108);\n    a = b + 0x10u;\n    for (i = 0; i < 0x2A; i++) {}\n}\n";
        let (out, count) = convert(text);
        assert_eq!(count, 4);
        assert_eq!(out, "void F(void)\n{\n    OBJ_POS_SET(8, 192, -264);\n    a = b + 16u;\n    for (i = 0; i < 42; i++) {}\n}\n");
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
        assert_eq!(convert("work[0x294] = 8;\nn += 0x78;\n").0, "work[0x294] = 8;\nn += 120;\n");
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
        // A tag covers its whole block, and the whole function at its top.
        same("void F(void)\n{\n    x = 1;\n    /* FAKEMATCH: order */\n    a = e + 0x10;\n    b = d + 0x10;\n}\n");
        let text = "void F(void)\n{\n    if (x) {\n        /* FAKEMATCH: order */\n        a = e + 0x10;\n        f = e + 0x10;\n    }\n    b = d + 0x10;\n}\n";
        assert_eq!(convert(text).0, text.replace("b = d + 0x10", "b = d + 16"));
    }

    /// The reviewer's findings on the rejected corpus run, verbatim.
    #[test]
    fn byte_offsets_subscripts_and_layout_stay_hex() {
        same("void F(u8 *state)\n{\n    state[0xcca] = 1;\n    state[0xcca] = 0;\n    cursor = state + 0x12f;\n    state[0x131] = 0;\n}\n");
        same("x = object[0x15];\ny = gPartyState.unknown_000[0xc];\nz = state + 0x1c0;\n");
        same("            if (ratio > value) {\n                result |= 0x400;\n                table += 0x10;\n            }\n");
        same("struct S {\n    u8 unknown_534[0xcb8 - 0x534];\n    u8 pad[0x3];\n};\n");
        same("n = a[i + 0x10];\n");
    }

    #[test]
    fn character_codes_stay_hex() {
        same("                idx = c - 0xDE;\n                if (idx > 1U) {\n                    idx += 0xBE;\n                }\n");
        same("                if ((u8)(character - 0xde) <= 1)\n");
        same("if (c == 0x20) {}\nout[0] = val + 0x30;\n");
        same("for (c = 0xa1; c < 0xe0; c++) {}\nfor (ch = 0x30; ch <= 0x39; ch++) {}\nn = glyph + 0x20;\n");
    }

    #[test]
    fn one_quantity_keeps_one_base_in_a_function() {
        // MOTION_AIM_APPROACH_LINKED_OBJECT.C:29-32
        same("s32 F(void)\n{\n  if (len > 0x10)\n  {\n    dx2 = dx;\n    n = len - 0x10;\n  }\n}\n");
        let other = "s32 F(void)\n{\n  if (len > 0x10)\n  {\n  }\n}\ns32 G(void)\n{\n    n = len - 0x10;\n}\n";
        assert_eq!(convert(other).0, other.replace("len - 0x10", "len - 16"));
    }

    /// The four lines the session-2 dry run left for a reviewer, verbatim:
    /// the enemy-id base and two buffer sizes, each also written hex
    /// elsewhere in its function.
    #[test]
    fn one_value_keeps_one_base_in_a_function() {
        // ☀️ BATTLE/PARTY/PARTY.C:396-397
        same("void F(void)\n{\n        id = 0x80;\n        entry_limit += 0x80;\n        for (; id < entry_limit; id += 1) {\n        }\n}\n");
        // ☀️ BATTLE/PARTY/PARTY.C:454-455; ⚓️ BATTLE/PARTY/LIST_LIVING_UNITS.C:81
        same("void F(void)\n{\n        remaining = 0x80;\n        enemy_limit = enemy_capacity + 0x80;\n        if (remaining < enemy_limit) {\n        }\n}\n");
        // ☀️ GRAPHICS/TEXT/GLYPH.C:59-62
        same("void F(void)\n{\n    remapped = Runtime_BumpAllocate(0x400);\n    src = decoded->input;\n    dst = remapped;\n    for (i = 0; i < 0x400; i++) {\n        index = *src++;\n    }\n}\n");
        // ☀️ FIELD/RAMAKAN_SABAKU/STORM.C:143-144
        same("void F(void)\n{\n    destination = buffer;\n    source = buffer + 0x480;\n    for (i = 0; i < 0x480; i++) {\n        u8 value = *source++;\n    }\n}\n");
        // A literal kept hex beside an address keeps its value hex too.
        same("void F(void)\n{\n    cursor = p + 0x80;\n    for (i = 0; i < 0x80; i++) {}\n}\n");
        // Another function's hex, or a tagged literal, keeps nothing.
        let other = "void F(void)\n{\n    id = 0x80;\n}\nvoid G(void)\n{\n    for (i = 0; i < 0x80; i++) {}\n}\n";
        assert_eq!(convert(other).0, other.replace("i < 0x80", "i < 128"));
        let tagged = "void F(void)\n{\n    if (x) {\n        /* FAKEMATCH: order */\n        id = 0x80;\n    }\n    for (i = 0; i < 0x80; i++) {}\n}\n";
        assert_eq!(convert(tagged).0, tagged.replace("i < 0x80", "i < 128"));
    }

    #[test]
    fn angles_stay_hex() {
        same("angle = angle + 0x10;\nobj->facing = obj->facing - 0x40;\nyaw += 0x20;\n");
        // ⚓️ BATTLE/EFFECT/ADVANCE_ANGLE_WITH_RAMP.C:8-13, RADPART.C:25-30:
        // the step a literal assigns meets the angle's hex.
        same("void BattleFx_AdvanceAngleWithRamp(struct ObjectRuntime *object)\n{\n    u32 step;\n\n    step = object->action * 0x50;\n    object->angle += step + 0x1000;\n    if (step < 0x1000)\n        object->action++;\n}\n");
        same("void BattleFx_SpinUpAngle(void *obj)\n{\n    u32 step;\n\n    step = FIELD_AT_OFFSET(obj, s16 *, 0x64) * 0x50;\n    FIELD_AT_OFFSET(obj, u16 *, 6) = (u16)(FIELD_AT_OFFSET(obj, u16 *, 6) + step + 0x1000);\n    if (step < 0x1000U) {\n        FIELD_AT_OFFSET(obj, s16 *, 0x64) = (s16)((u16)FIELD_AT_OFFSET(obj, s16 *, 0x64) + 1);\n    }\n}\n");
    }

    /// Lines the session-2 dry run decimalized, verbatim.
    #[test]
    fn offsets_ids_and_sound_cues_stay_hex() {
        // ☀️ BATTLE/EFFECT/EMITTER.C:256; FIELD/COMMON/OBJECT/SNAPSHOT.C:42
        same("    s32 work = (s32)gEventWork;\n    u32 off = (obj_id * 4) + 0x14;\n\n    s32 src_z = (s32)*(struct EfxSrc **)(work + off);\n");
        same("    if (index > 0xbf)\n        return 0;\n    offset = (index * 4) + 0x14;\n    return *(void **)(base + offset);\n");
        // COMMON BATTLE/PARTY/CONTAINS_ID.C:8; ☀️ BATTLE/MOTION/SLOTS.C; ⚓️ BATTLE/SUMMON/FIND_SLOT.C
        same("    if (id > 7)\n        id += 0x78;\n");
        same("        object_id -= 0x78;\n");
        same("        id = i + 0x80;\n");
        // ☀️ FIELD/KOROSSEO_KABE/TRANSITION.C
        same("    Engine_AudioPlayCue(mode + 0x5a);\n");
        // Quantities beside them still change.
        assert_eq!(convert("    object->y = (index / columns) * 0x10 + origin_y;\n").0, "    object->y = (index / columns) * 16 + origin_y;\n");
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
