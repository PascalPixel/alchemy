//! Turns lifted bodies into a translation unit: one C function per Thumb
//! function in the window, local declarations, and the declarations and
//! inline wrappers the candidate needs to compile against the overlay route.

use crate::decode::{Ins, Kind};
use crate::lift::{lift, Draft};
use std::collections::{BTreeMap, BTreeSet};

/// Splits the window at every `push {.., lr}` that follows a return.
pub fn split_functions(ins: &[Ins]) -> Vec<(u32, Vec<Ins>)> {
    let mut functions: Vec<(u32, Vec<Ins>)> = Vec::new();
    let mut current: Vec<Ins> = Vec::new();
    let mut returned = false;
    for x in ins {
        let prologue = matches!(x.kind, Kind::Push { lr: true });
        if prologue && returned && !current.is_empty() {
            functions.push((current[0].addr, std::mem::take(&mut current)));
            returned = false;
        }
        if matches!(x.kind, Kind::Bx(_) | Kind::Pop { pc: true }) {
            returned = true;
        }
        current.push(x.clone());
    }
    if !current.is_empty() {
        functions.push((current[0].addr, current));
    }
    functions
}

fn is_local_assignment(line: &str) -> Option<&str> {
    let trimmed = line.trim_start();
    let (name, _) = trimmed.split_once(" = ")?;
    let plain = name == "record"
        || name == "value"
        || name
            .strip_prefix("rec")
            .is_some_and(|rest| rest.bytes().all(|b| b.is_ascii_digit()))
        || is_park_name(name);
    plain.then_some(name)
}

/// A byte store of a constant that an earlier flag update OR-ed in shares
/// its byte-mode pseudo with that update; route it through an int local.
fn shown_pass(lines: &mut Vec<String>) {
    let mut constants = BTreeSet::new();
    for line in lines.iter() {
        let mut rest = line.as_str();
        while let Some(at) = rest.find("(u8)(value | ") {
            let tail = &rest[at + 13..];
            if let Some(end) = tail.find(')') {
                constants.insert(tail[..end].to_string());
            }
            rest = &rest[at + 13..];
        }
    }
    if constants.is_empty() {
        return;
    }
    let mut result = Vec::with_capacity(lines.len());
    for line in lines.drain(..) {
        let trimmed = line.trim_start();
        let lead = &line[..line.len() - trimmed.len()];
        let store = trimmed
            .strip_suffix(';')
            .and_then(|s| s.split_once(" = "))
            .filter(|(lhs, rhs)| {
                constants.contains(*rhs)
                    && (lhs.starts_with('*')
                        && lhs[1..]
                            .bytes()
                            .all(|b| b.is_ascii_alphanumeric() || b == b'_')
                        || indexed_store(lhs))
            });
        match store {
            Some((lhs, rhs)) => {
                result.push(format!("{lead}{{"));
                result.push(format!("{lead}    s32 shown = {rhs};"));
                result.push(String::new());
                result.push(format!("{lead}    {lhs} = shown;"));
                result.push(format!("{lead}}}"));
            }
            None => result.push(line),
        }
    }
    *lines = result;
}

/// Whether the local is ever indexed, which makes it a byte pointer;
/// every other local is an integer the address casts accept.
pub fn indexed_anywhere(lines: &[String], name: &str) -> bool {
    let deref = format!("*{name}");
    if lines.iter().any(|line| {
        let mut rest = line.as_str();
        while let Some(at) = rest.find(&deref) {
            let after = rest[at + deref.len()..].chars().next();
            let before = rest[..at].chars().last();
            if !after.is_some_and(|c| c.is_ascii_alphanumeric() || c == '_')
                && !before.is_some_and(|c| c == '*' || c == ')' || c.is_ascii_alphanumeric())
            {
                return true;
            }
            rest = &rest[at + deref.len()..];
        }
        false
    }) {
        return true;
    }
    let needle = format!("{name}[");
    lines.iter().any(|line| {
        let mut rest = line.as_str();
        while let Some(at) = rest.find(&needle) {
            let before_ok = at == 0
                || !rest[..at]
                    .chars()
                    .last()
                    .is_some_and(|c| c.is_ascii_alphanumeric() || c == '_');
            if before_ok {
                return true;
            }
            rest = &rest[at + needle.len()..];
        }
        false
    })
}

/// A register read before any write in the window (`rN?`) becomes an
/// uninitialised integer local; the compiler still assigns it a register.
fn unknown_registers(lines: &mut [String]) -> Vec<String> {
    let mut found = Vec::new();
    for line in lines.iter_mut() {
        let mut out = String::new();
        let mut rest = line.as_str();
        while let Some(at) = rest.find('?') {
            let head = &rest[..at];
            let digits: String = head
                .chars()
                .rev()
                .take_while(|c| c.is_ascii_digit())
                .collect();
            let start = head.len() - digits.len();
            if !digits.is_empty()
                && head[..start].ends_with('r')
                && !head[..start - 1].ends_with(|c: char| c.is_ascii_alphanumeric() || c == '_')
            {
                let name = format!("r{}", digits.chars().rev().collect::<String>());
                out.push_str(&head[..start - 1]);
                out.push_str(&name);
                if !found.contains(&name) {
                    found.push(name);
                }
            } else {
                out.push_str(head);
                out.push('?');
            }
            rest = &rest[at + 1..];
        }
        out.push_str(rest);
        *line = out;
    }
    found
}

/// `pN` with an optional disambiguating letter.
fn is_park_name(name: &str) -> bool {
    let Some(rest) = name.strip_prefix('p') else {
        return false;
    };
    let digits = rest.trim_end_matches(|c: char| c.is_ascii_lowercase());
    !digits.is_empty()
        && digits.bytes().all(|b| b.is_ascii_digit())
        && rest.len() - digits.len() <= 1
}

/// `name[digits]`.
fn indexed_store(lhs: &str) -> bool {
    let Some((name, rest)) = lhs.split_once('[') else {
        return false;
    };
    !name.is_empty()
        && name.bytes().all(|b| b.is_ascii_alphanumeric() || b == b'_')
        && rest
            .strip_suffix(']')
            .is_some_and(|d| !d.is_empty() && d.bytes().all(|b| b.is_ascii_digit()))
}

fn slots_in(lines: &[String]) -> Vec<String> {
    let mut found = Vec::new();
    for line in lines {
        let mut rest = line.as_str();
        while let Some(at) = rest.find("slot") {
            let before_ok = at == 0
                || !rest[..at]
                    .chars()
                    .last()
                    .is_some_and(|c| c.is_ascii_alphanumeric() || c == '_');
            let digits: String = rest[at + 4..]
                .chars()
                .take_while(|c| c.is_ascii_digit())
                .collect();
            if before_ok && !digits.is_empty() {
                let name = format!("slot{digits}");
                if !found.contains(&name) {
                    found.push(name);
                }
            }
            rest = &rest[at + 4..];
        }
    }
    found
}

fn params_in(lines: &[String]) -> Vec<String> {
    let mut found = Vec::new();
    for line in lines {
        for (at, _) in line.match_indices('a') {
            let before_ok = at == 0
                || !line[..at]
                    .chars()
                    .last()
                    .is_some_and(|c| c.is_ascii_alphanumeric() || c == '_');
            let after: Vec<char> = line[at + 1..].chars().take(2).collect();
            if before_ok
                && after.first().is_some_and(|c| ('0'..='3').contains(c))
                && !after
                    .get(1)
                    .is_some_and(|c| c.is_ascii_alphanumeric() || *c == '_')
            {
                let name = format!("a{}", after[0]);
                if !found.contains(&name) {
                    found.push(name);
                }
            }
        }
    }
    found
}

/// One C function from a lifted draft.
pub fn function_source(entry: u32, draft: &Draft) -> String {
    let mut lines = draft.lines.clone();
    shown_pass(&mut lines);
    let unknown = unknown_registers(&mut lines);
    let mut vars: Vec<String> = vec!["record".to_string()];
    for line in &lines {
        if let Some(name) = is_local_assignment(line) {
            if !vars.iter().any(|v| v == name) {
                vars.push(name.to_string());
            }
        }
    }
    vars.sort();
    let mut consts = draft.consts.clone();
    for slot in slots_in(&lines) {
        if !consts.contains(&slot) {
            consts.push(slot);
        }
    }
    let mut params = draft.params.clone();
    for param in params_in(&lines) {
        if !params.contains(&param) {
            params.push(param);
        }
    }
    params.sort();
    let signature = if params.is_empty() {
        "void".to_string()
    } else {
        params
            .iter()
            .map(|p| {
                if indexed_anywhere(&lines, p) {
                    format!("u8 *{p}")
                } else {
                    format!("s32 {p}")
                }
            })
            .collect::<Vec<_>>()
            .join(", ")
    };
    let mut text = format!("void Func_{entry:08x}({signature})\n{{\n    u32 i;\n");
    for v in vars.iter().filter(|v| !consts.contains(v)) {
        if indexed_anywhere(&lines, v) {
            text.push_str(&format!("    u8 *{v};\n"));
        } else {
            text.push_str(&format!("    s32 {v};\n"));
        }
    }
    for r in &unknown {
        text.push_str(&format!("    s32 {r};\n"));
    }
    for c in &consts {
        let pointer = is_park_name(c) || (c.starts_with("slot") && indexed_anywhere(&lines, c));
        if pointer {
            text.push_str(&format!("    u8 *{c};\n"));
        } else {
            text.push_str(&format!("    s32 {c};\n"));
        }
    }
    text.push('\n');
    for line in &lines {
        text.push_str(line);
        text.push('\n');
    }
    text.push_str("}\n");
    text
}

/// Lifts every function of a decoded window into C bodies.
pub fn bodies(ins: &[Ins]) -> String {
    split_functions(ins)
        .iter()
        .map(|(entry, function)| function_source(*entry, &lift(function)))
        .collect::<Vec<_>>()
        .join("\n")
}

fn matching_paren(text: &str) -> Option<usize> {
    let mut depth = 0i32;
    for (i, c) in text.char_indices() {
        match c {
            '(' => depth += 1,
            ')' => {
                depth -= 1;
                if depth == 0 {
                    return Some(i);
                }
            }
            _ => {}
        }
    }
    None
}

fn wrapper_prefix(before: &str) -> Option<&'static str> {
    let trimmed = before.strip_suffix('(')?;
    let digits = trimmed.trim_end_matches(|c: char| c.is_ascii_digit());
    if digits.len() == trimmed.len() {
        return None;
    }
    if digits.ends_with("Call") {
        Some("void")
    } else if digits.ends_with("Value") {
        Some("s32")
    } else {
        None
    }
}

fn symbols<'a>(body: &'a str, prefix: &str) -> Vec<(usize, &'a str)> {
    let mut found = Vec::new();
    let mut rest = body;
    let mut base = 0;
    while let Some(at) = rest.find(prefix) {
        let start = base + at;
        let tail = &body[start + prefix.len()..];
        if tail.len() >= 8 && tail[..8].bytes().all(|b| b.is_ascii_hexdigit()) {
            found.push((start, &body[start..start + prefix.len() + 8]));
        }
        rest = &rest[at + prefix.len()..];
        base = start + prefix.len();
    }
    found
}

/// Composes the candidate unit around the lifted bodies. The entry function
/// carries `name`; other functions keep their address names.
pub fn compose(entry: u32, name: &str, body: &str) -> String {
    let this = format!("Func_{entry:08x}");
    let body_lines: Vec<String> = body.lines().map(str::to_string).collect();
    let mut categories: BTreeMap<String, BTreeSet<&'static str>> = BTreeMap::new();
    fn add(map: &mut BTreeMap<String, BTreeSet<&'static str>>, symbol: &str, kind: &'static str) {
        map.entry(symbol.to_string()).or_default().insert(kind);
    }
    for (at, symbol) in symbols(body, "Func_") {
        let before = &body[..at];
        let after = &body[at + symbol.len()..];
        if let Some(kind) = wrapper_prefix(before) {
            add(&mut categories, symbol, kind);
        }
        if before.ends_with("(s32)") {
            add(&mut categories, symbol, "void");
        }
        if before.ends_with("(void)") {
            add(&mut categories, symbol, "s32");
        }
        if before.ends_with("((void (*)())") {
            add(&mut categories, symbol, "s32");
            continue;
        }
        let at_statement_start =
            before.trim_end_matches(' ').ends_with('\n') || before.trim().is_empty();
        if at_statement_start && after.starts_with('(') {
            add(&mut categories, symbol, "void");
        }
        if after.starts_with('(') {
            if let Some(end) = matching_paren(after) {
                let following = &after[end + 1..];
                if let Some(head) = before.strip_suffix(" = ") {
                    let ident = head
                        .rsplit(|c: char| !(c.is_ascii_alphanumeric() || c == '_'))
                        .next()
                        .unwrap_or("");
                    let pointer = indexed_anywhere(&body_lines, ident);
                    add(&mut categories, symbol, if pointer { "ptr" } else { "s32" });
                }
                if following.starts_with('[') {
                    add(&mut categories, symbol, "ptr");
                }
                if following.starts_with(" +")
                    || following.starts_with(" *")
                    || following.starts_with(" -")
                {
                    add(&mut categories, symbol, "s32");
                }
                let condition = before.ends_with("if (")
                    || before.ends_with("while (")
                    || before.ends_with("if ((")
                    || before.ends_with("while ((");
                if condition
                    || before.ends_with('(')
                    || before.ends_with(", ")
                    || before.ends_with(',')
                {
                    add(&mut categories, symbol, "s32");
                }
            }
        }
        categories.entry(symbol.to_string()).or_default();
    }
    categories.remove(&this);
    let defined: Vec<String> = body
        .lines()
        .filter_map(|line| line.strip_prefix("void Func_"))
        .map(|rest| format!("Func_{}", &rest[..8]))
        .collect();
    for symbol in &defined {
        if categories
            .get(symbol)
            .is_some_and(|k| !k.contains("ptr") && !k.contains("s32"))
        {
            categories.remove(symbol);
        }
    }
    let mut body = body.replacen(&format!("void {this}("), &format!("void {name}("), 1);
    let mut declarations = String::new();
    let mut data: Vec<&str> = symbols(&body, "Data_")
        .into_iter()
        .map(|(_, s)| s)
        .collect();
    data.sort();
    data.dedup();
    for symbol in data {
        declarations.push_str(&format!("extern u8 {symbol}[];\n"));
    }
    for (symbol, kinds) in &categories {
        let return_type = if kinds.contains("ptr") {
            "u8 *"
        } else if kinds.contains("s32") {
            "s32 "
        } else {
            "void "
        };
        declarations.push_str(&format!("{return_type}{symbol}();\n"));
        if kinds.contains("void") && (kinds.contains("ptr") || kinds.contains("s32")) {
            body = cast_direct_void_calls(&body, symbol);
        }
        if kinds.contains("ptr") && kinds.contains("void") {
            let mut rewritten = String::new();
            let mut rest = body.as_str();
            let needle = format!("({symbol},");
            while let Some(at) = rest.find(&needle) {
                let head = &rest[..at];
                let wrapper = head
                    .rsplit(|c: char| !(c.is_ascii_alphanumeric()))
                    .next()
                    .unwrap_or("");
                rewritten.push_str(head);
                if wrapper.starts_with("Call") {
                    rewritten.push_str(&format!("((void (*)()){symbol},"));
                } else {
                    rewritten.push_str(&needle);
                }
                rest = &rest[at + needle.len()..];
            }
            rewritten.push_str(rest);
            body = rewritten;
        }
    }
    let mut wrappers = String::new();
    if body.contains("Value0(") {
        wrappers.push_str("\nstatic __inline__ s32 Value0(s32 (*f)())\n{\n    return f();\n}\n");
    }
    if body.contains("Call0(") {
        wrappers.push_str("\nstatic __inline__ void Call0(void (*f)())\n{\n    f();\n}\n");
    }
    for n in 1..=12usize {
        let parameters = (0..n)
            .map(|i| format!("s32 a{i}"))
            .collect::<Vec<_>>()
            .join(", ");
        let arguments = (0..n)
            .map(|i| format!("a{i}"))
            .collect::<Vec<_>>()
            .join(", ");
        if body.contains(&format!("Call{n}(")) {
            wrappers.push_str(&format!(
                "\nstatic __inline__ void Call{n}(void (*f)(), {parameters})\n{{\n    f({arguments});\n}}\n"
            ));
        }
        if body.contains(&format!("Value{n}(")) {
            wrappers.push_str(&format!(
                "\nstatic __inline__ s32 Value{n}(s32 (*f)(), {parameters})\n{{\n    return f({arguments});\n}}\n"
            ));
        }
    }
    format!(
        "#include \"types.h\"\n\n#define {name} {this}\n\n\
/* Loader-relocated overlay calls: each symbol names the pre-relocation call\n\
\x20* word the image holds. */\n{declarations}\n\
/* Call sites spelled through these wrappers pass their constants straight\n\
\x20* into the argument registers; a direct call precomputes a costly constant\n\
\x20* into a pseudo that the compiler then shares with later uses in the block.\n\
\x20* A value-returning call also sets r0 last of its arguments. */\n{wrappers}\n\
/* The scene step counter at 0x1d8 of the shared scene work record. */\n\
static __inline__ void bump_step(s32 amount)\n{{\n    u8 *work = *(u8 **)0x03001ebc;\n\n    \
*(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);\n}}\n\n{body}"
    )
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn declarations_follow_use() {
        let body = "void Func_02000100(void)\n{\n    record = Func_02000200(1);\n    Call2(Func_02000300, 0x1000, 0);\n    if (Func_02000400() != 0) {\n    }\n}\n";
        let unit = compose(0x02000100, "Scene_Run", body);
        assert!(unit.contains("s32 Func_02000200();"));
        assert!(unit.contains("void Func_02000300();"));
        assert!(unit.contains("s32 Func_02000400();"));
        assert!(unit.contains("void Scene_Run(void)"));
        assert!(unit.contains("static __inline__ void Call2("));
        assert!(!unit.contains("Call1("));
    }
}

/// A callee declared as value-returning still needs a plain call insn at
/// the sites where the scheduler saw a clobber: route those through the
/// void wrapper with a cast.
fn cast_direct_void_calls(body: &str, symbol: &str) -> String {
    body.lines()
        .map(|line| {
            let trimmed = line.trim_start();
            let lead = &line[..line.len() - trimmed.len()];
            let Some(args) = trimmed
                .strip_prefix(symbol)
                .and_then(|rest| rest.strip_prefix('('))
                .and_then(|rest| rest.strip_suffix(");"))
            else {
                return line.to_string();
            };
            let n = if args.is_empty() {
                0
            } else {
                args.split(", ").count()
            };
            if n == 0 {
                format!("{lead}Call0((void (*)()){symbol});")
            } else {
                format!("{lead}Call{n}((void (*)()){symbol}, {args});")
            }
        })
        .collect::<Vec<_>>()
        .join("\n")
        + "\n"
}
