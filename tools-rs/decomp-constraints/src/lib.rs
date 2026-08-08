// Compiler-aware source constraints. Each mutation preserves
// the likely intent or ABI search space while changing type inference, live ranges,
// pointer scheduling, or expression lowering.
//
// Two halves live here:
//   - inferAssemblyConstraints, which reads a `Func_<stem>:` body out of an
//     asm/<stem>.s file and reports the shape facts (argument count, memory
//     widths, condition signedness, register live spans) that decide WHICH
//     mutation operators are worth trying on that function. This is the half
//     the CLI drives.
//   - CONSTRAINT_OPERATORS, the reusable mutations. They take the random source
//     as a parameter and must consume it in exactly the same order as the
//     established order -- skipping a `pick` call on a one-element
//     list would desynchronise every seeded search after that point.
//
// PORT NOTE: the regex patterns are transcribed verbatim into the small
// ECMAScript-semantics engine in regex.rs rather than rewritten, because the
// originals use negative lookahead and backreferences that no Rust regex crate
// offers, and because their exact greediness decides which text gets rewritten.

pub mod regex;

use std::sync::atomic::{AtomicUsize, Ordering};

use crate::regex::{replace_literal_first, Regex};

// ---------------------------------------------------------------------------
// canonical JSON
// ---------------------------------------------------------------------------

// PORT NOTE: tools/lib/canonical_json.ts is already ported to the
// tools-rs/canonical-json crate, but that crate is a serde_json consumer and
// this one is deliberately dependency-free. The single record shape written
// here is emitted through this tiny ordered writer instead; key order is
// insertion order, matching the JS object literal, and nothing sorts.
#[derive(Debug, Clone)]
pub enum Json {
    Number(f64),
    String(String),
    Array(Vec<Json>),
    Object(Vec<(String, Json)>),
}

fn is_primitive(value: &Json) -> bool {
    matches!(value, Json::Number(_) | Json::String(_))
}

fn encode_scalar(value: &Json) -> String {
    match value {
        Json::Number(n) => encode_number(*n),
        Json::String(s) => encode_string(s),
        _ => unreachable!("encode_scalar on a container"),
    }
}

/// `JSON.stringify` of a number: integral doubles print without a `.0` tail.
fn encode_number(n: f64) -> String {
    if n.is_finite() && n.fract() == 0.0 && n.abs() < 1e21 {
        format!("{}", n as i64)
    } else {
        format!("{n}")
    }
}

fn encode_string(s: &str) -> String {
    let mut out = String::with_capacity(s.len() + 2);
    out.push('"');
    for c in s.chars() {
        match c {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            '\u{8}' => out.push_str("\\b"),
            '\u{c}' => out.push_str("\\f"),
            c if (c as u32) < 0x20 => out.push_str(&format!("\\u{:04x}", c as u32)),
            c => out.push(c),
        }
    }
    out.push('"');
    out
}

pub fn canonical_json(value: &Json) -> String {
    reflow(value, "")
}

fn reflow(value: &Json, indent: &str) -> String {
    if is_primitive(value) {
        return encode_scalar(value);
    }
    let inner = format!("{indent}  ");
    match value {
        Json::Array(items) => {
            if items.is_empty() {
                return "[]".to_string();
            }
            if items.iter().all(is_primitive) {
                let parts: Vec<String> = items.iter().map(encode_scalar).collect();
                return format!("[{}]", parts.join(", "));
            }
            let parts: Vec<String> = items
                .iter()
                .map(|item| format!("{inner}{}", reflow(item, &inner)))
                .collect();
            format!("[\n{}\n{indent}]", parts.join(",\n"))
        }
        Json::Object(entries) => {
            if entries.is_empty() {
                return "{}".to_string();
            }
            let parts: Vec<String> = entries
                .iter()
                .map(|(key, item)| {
                    format!("{inner}{}: {}", encode_string(key), reflow(item, &inner))
                })
                .collect();
            format!("{{\n{}\n{indent}}}", parts.join(",\n"))
        }
        _ => unreachable!("primitives handled above"),
    }
}

// ---------------------------------------------------------------------------
// assembly constraints
// ---------------------------------------------------------------------------

#[derive(Debug, Clone, PartialEq)]
pub struct MemoryCounts {
    pub byte: u32,
    pub halfword: u32,
    pub word: u32,
    pub signed: u32,
}

#[derive(Debug, Clone, PartialEq)]
pub struct ConditionCounts {
    pub signed: u32,
    pub unsigned: u32,
    pub equality: u32,
}

#[derive(Debug, Clone, PartialEq)]
pub struct AssemblyConstraints {
    pub stem: String,
    pub instructions: usize,
    pub inferred_arguments: i64,
    pub calls: u32,
    pub memory: MemoryCounts,
    pub conditions: ConditionCounts,
    pub distinct_offsets: usize,
    pub longest_register_span: usize,
    pub stack_argument_accesses: u32,
    pub suggested_operators: Vec<String>,
}

impl AssemblyConstraints {
    pub fn to_json(&self) -> Json {
        Json::Object(vec![
            ("stem".into(), Json::String(self.stem.clone())),
            ("instructions".into(), Json::Number(self.instructions as f64)),
            (
                "inferred_arguments".into(),
                Json::Number(self.inferred_arguments as f64),
            ),
            ("calls".into(), Json::Number(self.calls as f64)),
            (
                "memory".into(),
                Json::Object(vec![
                    ("byte".into(), Json::Number(self.memory.byte as f64)),
                    ("halfword".into(), Json::Number(self.memory.halfword as f64)),
                    ("word".into(), Json::Number(self.memory.word as f64)),
                    ("signed".into(), Json::Number(self.memory.signed as f64)),
                ]),
            ),
            (
                "conditions".into(),
                Json::Object(vec![
                    ("signed".into(), Json::Number(self.conditions.signed as f64)),
                    (
                        "unsigned".into(),
                        Json::Number(self.conditions.unsigned as f64),
                    ),
                    (
                        "equality".into(),
                        Json::Number(self.conditions.equality as f64),
                    ),
                ]),
            ),
            (
                "distinct_offsets".into(),
                Json::Number(self.distinct_offsets as f64),
            ),
            (
                "longest_register_span".into(),
                Json::Number(self.longest_register_span as f64),
            ),
            (
                "stack_argument_accesses".into(),
                Json::Number(self.stack_argument_accesses as f64),
            ),
            (
                "suggested_operators".into(),
                Json::Array(
                    self.suggested_operators
                        .iter()
                        .map(|name| Json::String(name.clone()))
                        .collect(),
                ),
            ),
        ])
    }
}

/// `Number(text)` for the two literal shapes the offset regex can produce.
fn js_number(text: &str) -> f64 {
    let lower = text.to_ascii_lowercase();
    if let Some(digits) = lower.strip_prefix("0x") {
        let mut value = 0.0f64;
        for c in digits.chars() {
            value = value * 16.0 + f64::from(c.to_digit(16).expect("hex digit matched by regex"));
        }
        return value;
    }
    text.parse::<f64>().unwrap_or(f64::NAN)
}

// The mnemonics gcc's Thumb output writes its destination register first for.
// Transcribed from the writesFirst alternation; membership, not a prefix test.
const WRITES_FIRST: &[&str] = &[
    "mov", "movs", "mvn", "mvns", "add", "adds", "adc", "adcs", "sub", "subs", "sbc", "sbcs", "rsb",
    "rsbs", "and", "ands", "orr", "orrs", "eor", "eors", "bic", "bics", "lsl", "lsls", "lsr",
    "lsrs", "asr", "asrs", "ror", "rors", "neg", "negs", "mul", "muls", "ldr", "ldrb", "ldrh",
    "ldrsb", "ldrsh",
];

struct Row {
    mnemonic: String,
    operands: String,
}

/// An insertion-ordered set of strings. JS `Set` iteration order is insertion
/// order and it is load-bearing here: `suggested_operators` is serialised
/// straight out of the set, so a HashSet would reorder every emitted record.
#[derive(Default)]
struct OrderedSet {
    items: Vec<String>,
}

impl OrderedSet {
    fn add(&mut self, item: &str) {
        if !self.items.iter().any(|existing| existing == item) {
            self.items.push(item.to_string());
        }
    }
}

pub fn infer_assembly_constraints(stem: &str, source: &str) -> AssemblyConstraints {
    let line_split = Regex::new(r"\r?\n", "");
    let comment = Regex::new(r"@.*$", "");
    let token = Regex::new(r"^(\S+)\s*(.*)$", "");
    let suffix = Regex::new(r"\.(?:n|w)$", "");
    let register_pattern = Regex::new(r"\b(r(?:1[0-2]|[0-9])|sp|lr|pc|ip|fp|sl)\b", "");
    let immediate = Regex::new(r"#(0x[0-9a-f]+|\d+)", "i");
    let is_r0_to_r3 = Regex::new(r"^r[0-3]$", "");

    let mut rows: Vec<Row> = Vec::new();
    let mut started = false;
    let header = format!("Func_{stem}:");
    for raw in split_all(&line_split, source) {
        let line = regex::js_trim(&comment.replace_first(raw, "")).to_string();
        if line == header {
            started = true;
            continue;
        }
        if !started || line.is_empty() || line.starts_with('.') || line.ends_with(':') {
            continue;
        }
        if let Some(found) = token.exec(&line) {
            let head = found.group(&line, 1).unwrap_or("").to_lowercase();
            rows.push(Row {
                mnemonic: suffix.replace_first(&head, ""),
                operands: found.group(&line, 2).unwrap_or("").to_string(),
            });
        }
    }

    let mut written: Vec<String> = Vec::new();
    let mut maximum_argument: i64 = -1;
    let mut calls = 0u32;
    let mut stack = 0u32;
    let mut memory = MemoryCounts {
        byte: 0,
        halfword: 0,
        word: 0,
        signed: 0,
    };
    let mut conditions = ConditionCounts {
        signed: 0,
        unsigned: 0,
        equality: 0,
    };
    let mut offsets: Vec<f64> = Vec::new();
    let mut first: Vec<(String, usize)> = Vec::new();
    let mut last: Vec<(String, usize)> = Vec::new();

    for (index, row) in rows.iter().enumerate() {
        let mnemonic = row.mnemonic.as_str();
        if mnemonic == "bl" || mnemonic == "blx" {
            calls += 1;
        }
        if mnemonic.starts_with("ldr") || mnemonic.starts_with("str") {
            if mnemonic.ends_with('b') {
                memory.byte += 1;
            } else if mnemonic.ends_with('h') {
                memory.halfword += 1;
            } else {
                memory.word += 1;
            }
            if mnemonic.ends_with("sb") || mnemonic.ends_with("sh") {
                memory.signed += 1;
            }
            if row.operands.contains("[sp,") || row.operands.contains("[sp]") {
                stack += 1;
            }
            if let Some(found) = immediate.exec(&row.operands) {
                let value = js_number(found.group(&row.operands, 1).expect("group 1 participates"));
                if !offsets.contains(&value) {
                    offsets.push(value);
                }
            }
        }
        match mnemonic {
            "beq" | "bne" => conditions.equality += 1,
            "bgt" | "blt" | "bge" | "ble" | "bmi" | "bpl" => conditions.signed += 1,
            "bhi" | "bls" | "bcs" | "bcc" | "bhs" | "blo" => conditions.unsigned += 1,
            _ => {}
        }
        let registers: Vec<String> = register_pattern
            .find_iter(&row.operands)
            .iter()
            .map(|found| {
                found
                    .group(&row.operands, 1)
                    .expect("group 1 participates")
                    .to_string()
            })
            .collect();
        let destination = if WRITES_FIRST.contains(&mnemonic) {
            registers.first().cloned()
        } else {
            None
        };
        for (position, register) in registers.iter().enumerate() {
            if !first.iter().any(|(name, _)| name == register) {
                first.push((register.clone(), index));
            }
            match last.iter_mut().find(|(name, _)| name == register) {
                Some(slot) => slot.1 = index,
                None => last.push((register.clone(), index)),
            }
            let is_destination = position == 0 && destination.as_deref() == Some(register.as_str());
            if is_r0_to_r3.is_match(register)
                && !is_destination
                && !written.iter().any(|name| name == register)
            {
                let number = i64::from(
                    register[1..2]
                        .parse::<u8>()
                        .expect("r0-r3 has a digit at index 1"),
                );
                maximum_argument = maximum_argument.max(number);
            }
        }
        if let Some(destination) = destination {
            if !written.contains(&destination) {
                written.push(destination);
            }
        }
    }

    let longest = first
        .iter()
        .map(|(register, at)| {
            let end = last
                .iter()
                .find(|(name, _)| name == register)
                .map_or(*at, |(_, end)| *end);
            end - at
        })
        .max()
        .unwrap_or(0);

    let mut suggestions = OrderedSet::default();
    if memory.signed > 0 {
        suggestions.add("fieldtype");
    }
    if !offsets.is_empty() {
        suggestions.add("fieldsyntax");
        suggestions.add("splitload");
    }
    if stack > 0 || maximum_argument >= 3 {
        suggestions.add("argshift");
    }
    if calls > 0 {
        suggestions.add("signature");
    }
    if longest >= 8 {
        suggestions.add("declshuffle");
        suggestions.add("splitload");
        suggestions.add("postincrement");
    }
    if conditions.signed + conditions.unsigned + conditions.equality > 0 {
        suggestions.add("condinvert");
    }
    // 直後にロードが続くストアは揮発ストア柵の指紋(LAWS.md): スケジューラなら持ち上げる並び。
    if rows.iter().enumerate().any(|(at, row)| {
        row.mnemonic.starts_with("str")
            && at + 1 < rows.len()
            && rows[at + 1].mnemonic.starts_with("ldr")
    }) {
        suggestions.add("volatilize");
    }

    AssemblyConstraints {
        stem: stem.to_string(),
        instructions: rows.len(),
        inferred_arguments: maximum_argument + 1,
        calls,
        memory,
        conditions,
        distinct_offsets: offsets.len(),
        longest_register_span: longest,
        stack_argument_accesses: stack,
        suggested_operators: suggestions.items,
    }
}

/// `text.split(/\r?\n/)` -- a regex split, which keeps the empty trailing
/// field the way JS does.
fn split_all<'h>(pattern: &Regex, text: &'h str) -> Vec<&'h str> {
    let mut parts = Vec::new();
    let mut at = 0;
    for found in pattern.find_iter(text) {
        parts.push(&text[at..found.start()]);
        at = found.end();
    }
    parts.push(&text[at..]);
    parts
}

// ---------------------------------------------------------------------------
// constraint operators
// ---------------------------------------------------------------------------

pub type Random<'a> = &'a mut dyn FnMut() -> f64;

/// `items[Math.floor(random() * items.length)]`, including the JS behaviour on
/// an out-of-range draw (`undefined`, i.e. the caller bails).
fn pick_index(length: usize, random: Random) -> Option<usize> {
    if length == 0 {
        return None;
    }
    let index = (random() * length as f64).floor();
    if index < 0.0 || index >= length as f64 {
        return None;
    }
    Some(index as usize)
}

fn splice(body: &str, start: usize, length: usize, replacement: &str) -> String {
    format!("{}{}{}", &body[..start], replacement, &body[start + length..])
}

fn signature_type(body: &str, random: Random) -> Option<String> {
    let pattern = Regex::new(
        r"\b(s8|u8|s16|u16|s32|u32|void \*)\s+(Func_08[0-9a-f]{6})\s*\(([^)]*)\)",
        "",
    );
    let found = pattern.exec(body)?;
    let return_type = found.group(body, 1)?.to_string();
    let parameter_list = found.group(body, 3)?.to_string();

    let typed = Regex::new(r"^(s8|u8|s16|u16|s32|u32|void \*)\b", "");
    // (text, type) pairs; slot 0 is the return type, which is located by the
    // match index rather than by searching, since it is not a parameter.
    let mut slots: Vec<(String, String)> = vec![(return_type.clone(), return_type.clone())];
    for parameter in parameter_list.split(',') {
        let trimmed = regex::js_trim(parameter);
        if let Some(hit) = typed.exec(trimmed) {
            slots.push((
                hit.text(trimmed).to_string(),
                hit.group(trimmed, 1)?.to_string(),
            ));
        }
    }

    let slot_index = pick_index(slots.len(), random)?;
    let (slot_text, slot_type) = slots[slot_index].clone();
    let family: &[&str] = match slot_type.as_str() {
        "s8" => &["u8", "s32"],
        "u8" => &["s8", "u32"],
        "s16" => &["u16", "s32"],
        "u16" => &["s16", "u32"],
        "s32" => &["u32", "s16", "void *"],
        "u32" => &["s32", "u16", "void *"],
        "void *" => &["s32", "u32"],
        _ => &[],
    };
    let replacement = family[pick_index(family.len(), random)?];

    let offset = if slot_index == 0 {
        found.start()
    } else {
        let from = found.start() + return_type.len();
        // JS `indexOf` returns -1 here; the resulting slice arithmetic is
        // nonsense, and the case is unreachable because slot text always came
        // out of `body`. Bail rather than panic.
        from + body[from..].find(&slot_text)?
    };
    Some(splice(body, offset, slot_text.len(), replacement))
}

fn unused_leading_argument(body: &str, _random: Random) -> Option<String> {
    let pattern = Regex::new(r"(Func_08[0-9a-f]{6}\s*\()([^)]*)(\)\s*\{)", "");
    let found = pattern.exec(body)?;
    let parameters = found.group(body, 2)?.to_string();

    if Regex::new(r"\b_v1arg\b", "").is_match(&parameters) {
        let strip = Regex::new(r"^\s*s32 _v1arg\s*,?\s*", "");
        let blank = Regex::new(r"^\s*$", "");
        return Some(pattern.replace_first_with(body, |m| {
            let open = m.group(body, 1).unwrap_or("");
            let inner = m.group(body, 2).unwrap_or("");
            let close = m.group(body, 3).unwrap_or("");
            let stripped = blank.replace_first(&strip.replace_first(inner, ""), "void");
            format!("{open}{stripped}{close}")
        }));
    }

    let trimmed = regex::js_trim(&parameters).to_string();
    let count = if trimmed.is_empty() || trimmed == "void" {
        0
    } else {
        trimmed.split(',').count()
    };
    if count >= 4 {
        return None;
    }
    let tail = if count == 0 {
        String::new()
    } else {
        format!(", {trimmed}")
    };
    Some(pattern.replace_first(body, &format!("$1s32 _v1arg{tail}$3")))
}

fn pointer_step(body: &str, random: Random) -> Option<String> {
    let forms: [(&str, &str); 3] = [
        (r"\b(\w+) \+= 1;", "$1++;"),
        (r"\b(\w+)\+\+;", "$1 = $1 + 1;"),
        (r"\b(\w+) = \1 \+ 1;", "$1 += 1;"),
    ];
    let usable: Vec<(Regex, &str)> = forms
        .iter()
        .map(|(pattern, replacement)| (Regex::new(pattern, ""), *replacement))
        .filter(|(pattern, _)| pattern.is_match(body))
        .collect();
    let index = pick_index(usable.len(), random)?;
    let (pattern, replacement) = &usable[index];
    Some(pattern.replace_first(body, replacement))
}

fn fuse_post_increment(body: &str, _random: Random) -> Option<String> {
    let fuse = Regex::new(r"(\w+) = \*(\w+);\n(\s*)\2(?: \+= 1|\+\+| = \2 \+ 1);", "");
    if let Some(found) = fuse.exec(body) {
        let value = found.group(body, 1)?.to_string();
        let pointer = found.group(body, 2)?.to_string();
        return Some(fuse.replace_first_with(body, |_| format!("{value} = *{pointer}++;")));
    }
    let split = Regex::new(r"(\w+) = \*(\w+)\+\+;", "");
    let found = split.exec(body)?;
    let value = found.group(body, 1)?.to_string();
    let pointer = found.group(body, 2)?.to_string();
    Some(split.replace_first_with(body, |_| {
        format!("{value} = *{pointer};\n    {pointer} += 1;")
    }))
}

fn volatile_constraint(body: &str, random: Random) -> Option<String> {
    let pattern = Regex::new(
        r"^(\s+)(volatile )?(s8|u8|s16|u16|s32|u32|void \*+)\s+(\w+)\s*;",
        "m",
    );
    let declarations = pattern.find_iter(body);
    let index = pick_index(declarations.len(), random)?;
    let target = &declarations[index];
    let lead = target.group(body, 1).unwrap_or("");
    let already = target.group(body, 2).is_some();
    let kind = target.group(body, 3)?;
    let name = target.group(body, 4)?;
    let replacement = format!(
        "{lead}{}{kind} {name};",
        if already { "" } else { "volatile " }
    );
    Some(splice(
        body,
        target.start(),
        target.end() - target.start(),
        &replacement,
    ))
}

// 揮発ストア柵の法則(LAWS.md): 記憶アクセスの左辺値へ volatile を付与または除去する。
// スケジューラはvolatileストアを越えて後続ロードを持ち上げない(src/0809a65c.c)。
fn volatilize_access(body: &str, random: Random) -> Option<String> {
    if random() < 0.3 {
        let qualified = Regex::new(
            r"volatile (?=(?:s8|u8|s16|u16|s32|u32|s64|u64|void|[A-Za-z_][A-Za-z0-9_]*) \*[,)])",
            "",
        );
        let sites = qualified.find_iter(body);
        let index = pick_index(sites.len(), random)?;
        let target = &sites[index];
        return Some(splice(body, target.start(), "volatile ".len(), ""));
    }
    let pattern = Regex::new(
        r"\*\((?!volatile )((?:s8|u8|s16|u16|s32|u32|s64|u64|void|[A-Za-z_][A-Za-z0-9_]*)) \*\)|M2C_FIELD\(([^,()]+),\s*(?!volatile )((?:s8|u8|s16|u16|s32|u32))(\s*\*),",
        "",
    );
    let sites = pattern.find_iter(body);
    let index = pick_index(sites.len(), random)?;
    let target = &sites[index];
    let whole = target.text(body).to_string();
    let replacement = if whole.starts_with("M2C_FIELD") {
        Regex::new(r",\s*((?:s8|u8|s16|u16|s32|u32))(\s*\*),", "")
            .replace_first(&whole, ", volatile $1$2,")
    } else {
        replace_literal_first(&whole, "*(", "*(volatile ")
    };
    Some(splice(body, target.start(), whole.len(), &replacement))
}

fn field_type_constraint(body: &str, random: Random) -> Option<String> {
    let pattern = Regex::new(
        r"M2C_FIELD\(([^,()]+),\s*(s8|u8|s16|u16|s32|u32)(\s*\*?),\s*(0x[0-9a-fA-F]+|\d+)\)",
        "",
    );
    let matches = pattern.find_iter(body);
    let index = pick_index(matches.len(), random)?;
    let target = &matches[index];
    let flip: &[&str] = match target.group(body, 2)? {
        "s8" => &["u8"],
        "u8" => &["s8"],
        "s16" => &["u16"],
        "u16" => &["s16"],
        "s32" => &["u32"],
        "u32" => &["s32"],
        _ => &[],
    };
    // PORT NOTE: `pick` is called even on these one-element lists, so it draws
    // from the RNG; skipping the draw would shift every later decision in a
    // seeded run.
    let kind = flip[pick_index(flip.len(), random)?];
    let replacement = format!(
        "M2C_FIELD({}, {kind}{}, {})",
        target.group(body, 1)?,
        target.group(body, 3)?,
        target.group(body, 4)?
    );
    Some(splice(
        body,
        target.start(),
        target.end() - target.start(),
        &replacement,
    ))
}

fn field_syntax_constraint(body: &str, random: Random) -> Option<String> {
    let pattern = Regex::new(
        r"M2C_FIELD\(([^,()]+),\s*(s8|u8|s16|u16|s32|u32)\s*\*,\s*(0x[0-9a-fA-F]+|\d+)\)",
        "",
    );
    let matches = pattern.find_iter(body);
    let index = pick_index(matches.len(), random)?;
    let target = &matches[index];
    let replacement = format!(
        "*({} *)((u8 *)({}) + {})",
        target.group(body, 2)?,
        target.group(body, 1)?,
        target.group(body, 3)?
    );
    Some(splice(
        body,
        target.start(),
        target.end() - target.start(),
        &replacement,
    ))
}

// PORT NOTE: `let localCounter = 0` was module-level mutable state in the
// TypeScript, shared by every call in the process. An AtomicUsize keeps that
// exact property (names cycle _v1live0.._v1live96 across calls) instead of
// silently restarting the counter per invocation, which would make two
// mutations in one search collide on a local name.
static LOCAL_COUNTER: AtomicUsize = AtomicUsize::new(0);

pub fn reset_local_counter() {
    LOCAL_COUNTER.store(0, Ordering::SeqCst);
}

fn split_memory_load(body: &str, random: Random) -> Option<String> {
    let pattern = Regex::new(
        r"^(\s+)(\w+) = (M2C_FIELD\([^;]+\)|\*\([^;]+\));$",
        "m",
    );
    let matches = pattern.find_iter(body);
    let index = pick_index(matches.len(), random)?;
    let target = &matches[index];
    let name = format!("_v1live{}", LOCAL_COUNTER.fetch_add(1, Ordering::SeqCst) % 97);
    let declared = Regex::new(r"(Func_08[0-9a-f]{6}\s*\([^)]*\)\s*\{\n)", "")
        .replace_first(body, &format!("$1    s32 {name};\n"));
    let lead = target.group(body, 1)?;
    let destination = target.group(body, 2)?;
    let load = target.group(body, 3)?;
    Some(replace_literal_first(
        &declared,
        target.text(body),
        &format!("{lead}{name} = (s32)({load});\n{lead}{destination} = {name};"),
    ))
}

fn rounding_constraint(body: &str, _random: Random) -> Option<String> {
    let shift = Regex::new(r"\((\w+) \+ 3\) >> 2", "");
    if let Some(found) = shift.exec(body) {
        let name = found.group(body, 1)?.to_string();
        return Some(shift.replace_first_with(body, |_| format!("(({name} + 3) / 4)")));
    }
    let divide = Regex::new(r"\(\((\w+) \+ 3\) / 4\)", "");
    let found = divide.exec(body)?;
    let name = found.group(body, 1)?.to_string();
    Some(divide.replace_first_with(body, |_| format!("({name} + 3) >> 2")))
}

pub type ConstraintOperator = fn(&str, Random) -> Option<String>;

/// Stable names and order; seeded consumers treat the order as part of the
/// contract.
pub const CONSTRAINT_OPERATORS: &[(&str, ConstraintOperator)] = &[
    ("signature", signature_type),
    ("argshift", unused_leading_argument),
    ("pointerstep", pointer_step),
    ("postincrement", fuse_post_increment),
    ("volatile", volatile_constraint),
    ("volatilize", volatilize_access),
    ("fieldtype", field_type_constraint),
    ("fieldsyntax", field_syntax_constraint),
    ("splitload", split_memory_load),
    ("rounding", rounding_constraint),
];

#[cfg(test)]
mod tests {
    use super::*;

    /// A deterministic seeded RNG stand-in: draws are
    /// scripted so a test can pin both the choice AND the number of draws.
    fn scripted(values: &[f64]) -> impl FnMut() -> f64 + '_ {
        let mut at = 0usize;
        move || {
            let value = values.get(at).copied().unwrap_or(0.0);
            at += 1;
            value
        }
    }

    const SAMPLE: &str =
        "Func_08000000:\n\tmov r3, r1\n\tldrsh r2, [r0, r3]\n\tcmp r2, #0\n\tblt .L0\n\tbl Func_08000100\n.L0:\n\tbx lr\n";

    #[test]
    fn matches_the_upstream_self_test_expectations() {
        let result = infer_assembly_constraints("08000000", SAMPLE);
        assert_eq!(result.inferred_arguments, 2);
        assert_eq!(result.memory.signed, 1);
        assert_eq!(result.conditions.signed, 1);
        assert!(result
            .suggested_operators
            .iter()
            .any(|name| name == "fieldtype"));
    }

    #[test]
    fn rows_start_only_after_the_named_label_and_drop_comments() {
        // Everything before `Func_…:` is ignored, `@` comments are cut, and
        // directive/label lines never become rows.
        let source = concat!(
            "\t.text\n",
            "\tldr r0, [r1]      @ this load is OUTSIDE the function\n",
            "Func_08001234:\n",
            "\t.align 2\n",
            "\tldrb r0, [r1, #4] @ trailing comment\n",
            "other_label:\n",
            "\tbx lr\n"
        );
        let result = infer_assembly_constraints("08001234", source);
        assert_eq!(result.instructions, 2, "ldrb and bx only");
        assert_eq!(result.memory.byte, 1);
        assert_eq!(result.memory.word, 0, "the pre-label ldr must not count");
        assert_eq!(result.distinct_offsets, 1);
    }

    #[test]
    fn hex_and_decimal_offsets_that_denote_the_same_number_collapse() {
        // JS has one number type, so Set<number> holds 0x10 and 16 as ONE
        // entry. A port that de-duplicated on the literal text would report 2.
        let source =
            "Func_08000001:\n\tldr r0, [r1, #0x10]\n\tldr r2, [r1, #16]\n\tldr r3, [r1, #20]\n";
        let result = infer_assembly_constraints("08000001", source);
        assert_eq!(result.distinct_offsets, 2);
    }

    #[test]
    fn a_destination_register_is_not_counted_as_an_incoming_argument() {
        // `mov r3, r1` writes r3, so r3 is not evidence of a 4th argument;
        // r1 is. Reading r3 later, after it was written, still must not count.
        let source = "Func_08000002:\n\tmov r3, r1\n\tadd r0, r3\n\tbx lr\n";
        let result = infer_assembly_constraints("08000002", source);
        assert_eq!(result.inferred_arguments, 2, "r0 and r1");
        assert!(!result
            .suggested_operators
            .iter()
            .any(|name| name == "argshift"));
    }

    #[test]
    fn r13_is_not_a_register_token_but_r10_is() {
        let source = "Func_08000003:\n\tldr r10, [r13, #4]\n\tldr r0, [sp, #8]\n\tbx lr\n";
        let result = infer_assembly_constraints("08000003", source);
        assert_eq!(
            result.stack_argument_accesses, 1,
            "only the [sp, form counts"
        );
        assert!(result
            .suggested_operators
            .iter()
            .any(|name| name == "argshift"));
    }

    #[test]
    fn suggestion_order_is_insertion_order_not_alphabetical() {
        // splitload is first added by the offsets rule and must NOT move when
        // the live-span rule re-adds it.
        let mut source = String::from("Func_08000004:\n\tldrsh r0, [r1, #4]\n\tstr r0, [r1]\n");
        source.push_str("\tldr r2, [r1, #8]\n");
        for _ in 0..10 {
            source.push_str("\tadd r4, r5\n");
        }
        source.push_str("\tadd r1, r6\n\tbeq .L0\n\tbx lr\n");
        let result = infer_assembly_constraints("08000004", &source);
        assert_eq!(
            result.suggested_operators,
            vec![
                "fieldtype",
                "fieldsyntax",
                "splitload",
                "declshuffle",
                "postincrement",
                "condinvert",
                "volatilize"
            ]
        );
    }

    #[test]
    fn canonical_json_shape_matches_the_typescript_writer() {
        let result = infer_assembly_constraints("08000000", SAMPLE);
        let text = canonical_json(&result.to_json());
        assert!(text.starts_with("{\n  \"stem\": \"08000000\",\n  \"instructions\": 6,"));
        assert!(text.contains("\n  \"memory\": {\n    \"byte\": 0,"));
        assert!(
            text.contains("\"suggested_operators\": [\"fieldtype\", \"signature\", \"condinvert\"]"),
            "primitive arrays stay inline on one line: {text}"
        );
    }

    #[test]
    fn signature_flips_the_return_type_when_the_first_slot_is_drawn() {
        let body = "s32 Func_08001000(u8 a, void *b) {\n    return 0;\n}\n";
        let mut random = scripted(&[0.0, 0.0]);
        let out = signature_type(body, &mut random).expect("a signature exists");
        assert!(out.starts_with("u32 Func_08001000("), "got {out}");
    }

    #[test]
    fn signature_flips_a_parameter_when_a_later_slot_is_drawn() {
        let body = "s32 Func_08001000(u8 a, void *b) {\n    return 0;\n}\n";
        // slots = [s32, u8, void *]; draw index 1 (u8), then family[0] = s8.
        let mut random = scripted(&[1.0 / 3.0, 0.0]);
        let out = signature_type(body, &mut random).expect("a signature exists");
        assert_eq!(out, "s32 Func_08001000(s8 a, void *b) {\n    return 0;\n}\n");
    }

    #[test]
    fn signature_skips_a_spaced_pointer_parameter() {
        // `void * b` fails the `\b` after the `*`, so it is not a slot at all;
        // only the return type is offered and the draw must still land there.
        let body = "u16 Func_08001000(void * b) {\n    return 0;\n}\n";
        let mut random = scripted(&[0.9, 0.0]);
        let out = signature_type(body, &mut random).expect("a signature exists");
        assert!(out.starts_with("s16 Func_08001000(void * b)"), "got {out}");
    }

    #[test]
    fn argshift_adds_then_removes_the_synthetic_argument() {
        let body = "s32 Func_08001000(u8 a) {\n    return a;\n}\n";
        let mut random = scripted(&[]);
        let added = unused_leading_argument(body, &mut random).expect("room for one more");
        assert_eq!(
            added,
            "s32 Func_08001000(s32 _v1arg, u8 a) {\n    return a;\n}\n"
        );
        let removed = unused_leading_argument(&added, &mut random).expect("it is reversible");
        assert_eq!(removed, body);
    }

    #[test]
    fn argshift_leaves_a_full_argument_list_alone() {
        let body = "s32 Func_08001000(u8 a, u8 b, u8 c, u8 d) {\n    return a;\n}\n";
        let mut random = scripted(&[]);
        assert!(unused_leading_argument(body, &mut random).is_none());
    }

    #[test]
    fn argshift_removal_restores_void_for_an_emptied_list() {
        let body = "s32 Func_08001000(s32 _v1arg) {\n    return 0;\n}\n";
        let mut random = scripted(&[]);
        let removed = unused_leading_argument(body, &mut random).expect("removable");
        assert_eq!(removed, "s32 Func_08001000(void) {\n    return 0;\n}\n");
    }

    #[test]
    fn pointerstep_rewrites_only_the_forms_present() {
        let body = "    p += 1;\n";
        // Only form 0 is usable, so the single-entry pick must land on it.
        let mut random = scripted(&[0.0]);
        assert_eq!(pointer_step(body, &mut random).unwrap(), "    p++;\n");

        let assigned = "    q = q + 1;\n";
        let mut random = scripted(&[0.0]);
        assert_eq!(
            pointer_step(assigned, &mut random).unwrap(),
            "    q += 1;\n",
            "the backreference form must recognise q = q + 1"
        );

        let mismatched = "    q = r + 1;\n";
        let mut random = scripted(&[0.0]);
        assert!(
            pointer_step(mismatched, &mut random).is_none(),
            "q = r + 1 is not a self-increment"
        );
    }

    #[test]
    fn postincrement_fuses_and_unfuses() {
        let body = "    x = *p;\n    p += 1;\n";
        let mut random = scripted(&[]);
        let fused = fuse_post_increment(body, &mut random).expect("fusable");
        assert_eq!(fused, "    x = *p++;\n");
        let split = fuse_post_increment(&fused, &mut random).expect("splittable");
        assert_eq!(split, body);
    }

    #[test]
    fn volatile_toggles_a_declaration_in_place() {
        let body = "void Func_08001000(void) {\n    s32 x;\n    u8 y;\n}\n";
        let mut random = scripted(&[0.0]);
        let out = volatile_constraint(body, &mut random).expect("two declarations");
        assert_eq!(
            out,
            "void Func_08001000(void) {\n    volatile s32 x;\n    u8 y;\n}\n"
        );
        let mut random = scripted(&[0.0]);
        let back = volatile_constraint(&out, &mut random).expect("still matches");
        assert_eq!(back, body);
    }

    #[test]
    fn volatilize_adds_a_qualifier_to_a_cast_and_removes_it_again() {
        let body = "    x = *(s32 *)p;\n";
        // random() >= 0.3 takes the "add" branch; then draw site 0.
        let mut random = scripted(&[0.5, 0.0]);
        let out = volatilize_access(body, &mut random).expect("one cast site");
        assert_eq!(out, "    x = *(volatile s32 *)p;\n");

        // random() < 0.3 takes the "remove" branch.
        let mut random = scripted(&[0.1, 0.0]);
        let back = volatilize_access(&out, &mut random).expect("one qualified site");
        assert_eq!(back, body);
    }

    #[test]
    fn volatilize_qualifies_an_m2c_field_pointer() {
        let body = "    x = M2C_FIELD(base, s32 *, 0x8);\n";
        let mut random = scripted(&[0.5, 0.0]);
        let out = volatilize_access(body, &mut random).expect("one field site");
        assert_eq!(out, "    x = M2C_FIELD(base, volatile s32 *, 0x8);\n");
    }

    #[test]
    fn fieldtype_flips_signedness_and_still_draws_from_the_rng() {
        let body = "    x = M2C_FIELD(base, s16, 0x4);\n";
        let draws = std::cell::Cell::new(0usize);
        let mut random = || {
            draws.set(draws.get() + 1);
            0.0
        };
        let out = field_type_constraint(body, &mut random).expect("one field");
        assert_eq!(out, "    x = M2C_FIELD(base, u16, 0x4);\n");
        assert_eq!(
            draws.get(),
            2,
            "one draw for the site, one for the one-item flip"
        );
    }

    #[test]
    fn fieldsyntax_expands_a_pointer_field_to_a_cast() {
        let body = "    x = M2C_FIELD(base, u8 *, 0x10);\n";
        let mut random = scripted(&[0.0]);
        let out = field_syntax_constraint(body, &mut random).expect("one field");
        assert_eq!(out, "    x = *(u8 *)((u8 *)(base) + 0x10);\n");
    }

    #[test]
    fn splitload_declares_a_live_local_and_cycles_its_name() {
        reset_local_counter();
        let body = "void Func_08001000(void) {\n    x = M2C_FIELD(base, s32, 0x4);\n}\n";
        let mut random = scripted(&[0.0]);
        let out = split_memory_load(body, &mut random).expect("one load");
        assert_eq!(
            out,
            concat!(
                "void Func_08001000(void) {\n",
                "    s32 _v1live0;\n",
                "    _v1live0 = (s32)(M2C_FIELD(base, s32, 0x4));\n",
                "    x = _v1live0;\n}\n"
            )
        );
        let mut random = scripted(&[0.0]);
        let again = split_memory_load(body, &mut random).expect("one load");
        assert!(
            again.contains("_v1live1"),
            "the module-level counter advances across calls: {again}"
        );
    }

    #[test]
    fn rounding_swaps_between_shift_and_divide() {
        let mut random = scripted(&[]);
        let shifted = "    n = (count + 3) >> 2;\n";
        let divided = rounding_constraint(shifted, &mut random).expect("a shift exists");
        assert_eq!(divided, "    n = ((count + 3) / 4);\n");
        let back = rounding_constraint(&divided, &mut random).expect("a divide exists");
        assert_eq!(back, shifted);
    }

    #[test]
    fn operator_table_names_and_order_are_the_contract() {
        let names: Vec<&str> = CONSTRAINT_OPERATORS.iter().map(|(name, _)| *name).collect();
        assert_eq!(
            names,
            vec![
                "signature",
                "argshift",
                "pointerstep",
                "postincrement",
                "volatile",
                "volatilize",
                "fieldtype",
                "fieldsyntax",
                "splitload",
                "rounding"
            ]
        );
    }

    #[test]
    fn every_operator_declines_cleanly_on_unrelated_source() {
        let body = "int main(void) { return 0; }\n";
        for (name, operator) in CONSTRAINT_OPERATORS {
            let mut random = scripted(&[0.5, 0.5, 0.5]);
            assert!(
                operator(body, &mut random).is_none(),
                "{name} should decline on source with nothing to mutate"
            );
        }
    }
}
