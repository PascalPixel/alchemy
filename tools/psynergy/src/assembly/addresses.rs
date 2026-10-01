//! Address uses in assembly source. Values come from source operands; no
//! image, placement, expected bytes or generated report enters the analysis.

use std::collections::{BTreeMap, BTreeSet, VecDeque};

#[derive(Clone, Debug, Default)]
pub struct Types {
    /// Pointer arguments in the first four ARM ABI registers.
    pub arguments: BTreeMap<String, u16>,
    /// Source declarations of arrays whose elements are pointers.
    pub tables: BTreeSet<String>,
    /// Pointer-valued source objects, distinct from pointer arrays.
    pub objects: BTreeSet<String>,
}

#[derive(Clone, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub struct Site {
    pub source: usize,
    pub line: usize,
    pub value: u32,
}

/// An integer assembler expression, never a symbol's address. Arithmetic is
/// evaluated as a 32-bit word, just as GAS emits it in a word directive.
pub fn integer(text: &str) -> Option<u32> {
    fn atom(text: &[u8], at: &mut usize) -> Option<u32> {
        space(text, at);
        let unary = text.get(*at).copied();
        if matches!(unary, Some(b'+' | b'-' | b'~')) {
            *at += 1;
            let value = atom(text, at)?;
            return Some(match unary? {
                b'-' => value.wrapping_neg(),
                b'~' => !value,
                _ => value,
            });
        }
        if unary == Some(b'(') {
            *at += 1;
            let value = expr(text, at, 0)?;
            space(text, at);
            if text.get(*at) != Some(&b')') {
                return None;
            }
            *at += 1;
            return Some(value);
        }
        let start = *at;
        while text.get(*at).is_some_and(u8::is_ascii_alphanumeric) {
            *at += 1;
        }
        let number = std::str::from_utf8(&text[start..*at]).ok()?;
        if let Some(hex) = number
            .strip_prefix("0x")
            .or_else(|| number.strip_prefix("0X"))
        {
            u32::from_str_radix(hex, 16).ok()
        } else {
            number.parse().ok()
        }
    }
    fn space(text: &[u8], at: &mut usize) {
        while text.get(*at).is_some_and(u8::is_ascii_whitespace) {
            *at += 1;
        }
    }
    fn expr(text: &[u8], at: &mut usize, minimum: u8) -> Option<u32> {
        let mut value = atom(text, at)?;
        loop {
            space(text, at);
            let rest = &text[*at..];
            let (operator, precedence, width) = if rest.starts_with(b"<<") {
                ('<', 4, 2)
            } else if rest.starts_with(b">>") {
                ('>', 4, 2)
            } else {
                match rest.first().copied() {
                    Some(b'|') => ('|', 1, 1),
                    Some(b'^') => ('^', 2, 1),
                    Some(b'&') => ('&', 3, 1),
                    Some(b'+') => ('+', 5, 1),
                    Some(b'-') => ('-', 5, 1),
                    Some(b'*') => ('*', 6, 1),
                    Some(b'/') => ('/', 6, 1),
                    Some(b'%') => ('%', 6, 1),
                    _ => break,
                }
            };
            if precedence < minimum {
                break;
            }
            *at += width;
            let right = expr(text, at, precedence + 1)?;
            value = match operator {
                '+' => value.wrapping_add(right),
                '-' => value.wrapping_sub(right),
                '*' => value.wrapping_mul(right),
                '/' => value.checked_div(right)?,
                '%' => value.checked_rem(right)?,
                '<' => value.checked_shl(right)?,
                '>' => value.checked_shr(right)?,
                '|' => value | right,
                '^' => value ^ right,
                '&' => value & right,
                _ => unreachable!(),
            };
        }
        Some(value)
    }
    let mut at = 0;
    let value = expr(text.as_bytes(), &mut at, 0)?;
    space(text.as_bytes(), &mut at);
    (at == text.len()).then_some(value)
}

pub fn memory_address(word: u32) -> bool {
    (0x0200_0000..0x0204_0000).contains(&word)
        || (0x0300_0000..0x0300_8000).contains(&word)
        || (0x0800_0000..0x0e00_0000).contains(&word)
}
/// A source-spelled immediate call, including a numeric expression. Register
/// calls and symbol expressions remain references rather than absolute calls.
pub fn numeric_calls(source: &str) -> Vec<(usize, u32)> {
    source
        .lines()
        .enumerate()
        .filter_map(|(line, text)| {
            let text = text.split_once(':').map_or(text, |(_, rest)| rest).trim();
            let (word, operands) = text.split_once(char::is_whitespace)?;
            let (word, _) = condition(word);
            matches!(word, "bl" | "blx")
                .then(|| {
                    integer(operands.trim().trim_start_matches('#')).map(|value| (line + 1, value))
                })
                .flatten()
        })
        .collect()
}

fn register(text: &str) -> Option<usize> {
    match text.trim() {
        "ip" => Some(12),
        "sp" => Some(13),
        "lr" => Some(14),
        "pc" => Some(15),
        text => text
            .strip_prefix('r')?
            .parse::<usize>()
            .ok()
            .filter(|&r| r < 16),
    }
}

fn symbol(text: &str) -> Option<(String, i32)> {
    let text = text.trim().trim_start_matches('=').trim();
    let end = text
        .find(|c: char| !(c.is_ascii_alphanumeric() || matches!(c, '_' | '.' | '$')))
        .unwrap_or(text.len());
    let name = &text[..end];
    if !name.starts_with(|c: char| c.is_ascii_alphabetic() || matches!(c, '_' | '.' | '$')) {
        return None;
    }
    let rest = text[end..].trim();
    let offset = if rest.is_empty() {
        0
    } else {
        integer(rest)? as i32
    };
    Some((name.to_string(), offset))
}

#[derive(Clone, Debug, PartialEq, Eq, PartialOrd, Ord)]
enum Origin {
    Word(usize),
    Symbol(String, Option<i32>),
    Argument(usize),
}
type Value = BTreeSet<Origin>;

#[derive(Clone, Debug)]
enum Op {
    Instruction(String, Vec<String>),
    Word(String),
    Barrier,
}
#[derive(Clone, Debug)]
struct Node {
    source: usize,
    line: usize,
    op: Op,
}
#[derive(Default)]
struct Program {
    nodes: Vec<Node>,
    labels: BTreeMap<String, Vec<usize>>,
    labelled: BTreeSet<usize>,
    entries: BTreeMap<String, Vec<usize>>,
    roots: BTreeSet<usize>,
    locals: BTreeSet<(usize, String)>,
}

impl Program {
    fn name(&self, source: usize, name: &str) -> String {
        if name.starts_with('.') || self.locals.contains(&(source, name.to_string())) {
            format!("__source{source}{name}")
        } else {
            name.to_string()
        }
    }
    fn qualify(&self, source: usize, operand: &str) -> String {
        symbol(operand).map_or_else(
            || operand.to_string(),
            |(name, offset)| {
                let name = self.name(source, &name);
                if offset == 0 {
                    name
                } else {
                    format!("{name} + {offset}")
                }
            },
        )
    }
    fn append(&mut self, source: usize, text: &str) {
        let exported = text
            .lines()
            .filter_map(|line| {
                let (word, operands) = line.trim().split_once(char::is_whitespace)?;
                matches!(word, ".global" | ".globl").then_some(operands)
            })
            .flat_map(|operands| operands.split(',').map(|name| name.trim().to_string()))
            .collect::<BTreeSet<_>>();
        for name in text
            .lines()
            .filter_map(|line| line.trim().split_once(':').map(|(name, _)| name.trim()))
        {
            if !exported.contains(name) && symbol(name).is_some() {
                self.locals.insert((source, name.to_string()));
            }
        }
        self.nodes.push(Node {
            source,
            line: 0,
            op: Op::Barrier,
        });
        let mut entry = false;
        let mut first = true;
        let mut labels = Vec::new();
        for (line, raw) in text.lines().enumerate() {
            let mut text = raw.trim();
            if let Some((name, rest)) = text.split_once(':') {
                if symbol(name).is_some_and(|(_, offset)| offset == 0) {
                    let name = self.name(source, name.trim());
                    self.labels
                        .entry(name.clone())
                        .or_default()
                        .push(self.nodes.len());
                    self.labelled.insert(self.nodes.len());
                    labels.push(name);
                    text = rest.trim();
                }
            }
            if text.is_empty() || text.starts_with('#') {
                continue;
            }
            let (word, operands) = text.split_once(char::is_whitespace).unwrap_or((text, ""));
            if matches!(
                word,
                ".section"
                    | ".text"
                    | ".data"
                    | ".bss"
                    | ".pushsection"
                    | ".popsection"
                    | ".previous"
                    | ".thumb_func"
                    | ".arm"
            ) {
                self.nodes.push(Node {
                    source,
                    line: line + 1,
                    op: Op::Barrier,
                });
                labels.clear();
                entry = word == ".thumb_func" || word == ".arm";
                continue;
            }
            if matches!(word, ".4byte" | ".word" | ".long" | ".int") {
                for operand in operands.split(',') {
                    self.nodes.push(Node {
                        source,
                        line: line + 1,
                        op: Op::Word(self.qualify(source, operand.trim())),
                    });
                }
                labels.clear();
                continue;
            }
            if matches!(word, ".2byte" | ".hword" | ".short") && integer(operands) == Some(0xf800) {
                self.nodes.push(Node {
                    source,
                    line: line + 1,
                    op: Op::Instruction("blx".into(), vec!["lr".into()]),
                });
                continue;
            }
            if word.starts_with('.') {
                if matches!(
                    word,
                    ".byte"
                        | ".2byte"
                        | ".hword"
                        | ".short"
                        | ".space"
                        | ".skip"
                        | ".incbin"
                        | ".ascii"
                        | ".asciz"
                        | ".string"
                        | ".fill"
                        | ".zero"
                ) {
                    self.nodes.push(Node {
                        source,
                        line: line + 1,
                        op: Op::Barrier,
                    });
                    labels.clear();
                }
                continue;
            }
            if entry || first {
                self.roots.insert(self.nodes.len());
                for name in &labels {
                    self.entries
                        .entry(name.clone())
                        .or_default()
                        .push(self.nodes.len());
                }
                first = false;
                entry = false;
            }
            if word.starts_with("ldr") && !operands.contains('[') {
                self.roots.insert(self.nodes.len());
            }
            let operands = operands
                .split(',')
                .map(|operand| operand.trim().to_string())
                .collect();
            self.nodes.push(Node {
                source,
                line: line + 1,
                op: Op::Instruction(word.into(), operands),
            });
            labels.clear();
        }
    }
    fn words(&self, name: &str, offset: i32, indexed: bool) -> Vec<usize> {
        let mut words = Vec::new();
        for &start in self.labels.get(name).into_iter().flatten() {
            if offset < 0 || offset % 4 != 0 {
                continue;
            }
            let start = start + offset as usize / 4;
            for at in start..self.nodes.len() {
                if !matches!(self.nodes[at].op, Op::Word(_)) {
                    break;
                }
                if at != start && self.labelled.contains(&at) {
                    break;
                }
                words.push(at);
                if !indexed {
                    break;
                }
            }
        }
        words
    }
    fn value(&self, at: usize) -> Value {
        match &self.nodes[at].op {
            Op::Word(operand) if integer(operand).is_some() => {
                if integer(operand).is_some_and(memory_address) {
                    BTreeSet::from([Origin::Word(at)])
                } else {
                    Value::new()
                }
            }
            Op::Word(operand) => symbol(operand)
                .map(|(name, offset)| BTreeSet::from([Origin::Symbol(name, Some(offset))]))
                .unwrap_or_default(),
            _ => Value::new(),
        }
    }
}

#[derive(Clone, Debug, Default, PartialEq, Eq)]
struct Summary {
    arguments: u16,
    returned: Value,
}
#[derive(Clone, Debug, Default, PartialEq, Eq)]
struct State {
    registers: [Value; 16],
    stack: BTreeMap<i32, Value>,
}
fn widen(value: &mut Value) {
    let mut offsets = BTreeMap::<String, BTreeSet<Option<i32>>>::new();
    for origin in value.iter() {
        if let Origin::Symbol(name, offset) = origin {
            offsets.entry(name.clone()).or_default().insert(*offset);
        }
    }
    for (name, offsets) in offsets {
        if offsets.len() > 1 {
            value.retain(|origin| !matches!(origin, Origin::Symbol(symbol, _) if symbol == &name));
            value.insert(Origin::Symbol(name, None));
        }
    }
}
impl State {
    fn merge(&mut self, other: &State) -> bool {
        let mut changed = false;
        for (value, incoming) in self.registers.iter_mut().zip(&other.registers) {
            if incoming.is_empty() {
                continue;
            }
            let before = value.clone();
            value.extend(incoming.iter().cloned());
            widen(value);
            changed |= *value != before;
        }
        for (&slot, incoming) in &other.stack {
            if incoming.is_empty() {
                continue;
            }
            let value = self.stack.entry(slot).or_default();
            let before = value.clone();
            value.extend(incoming.iter().cloned());
            widen(value);
            changed |= *value != before;
        }
        changed
    }
}

fn condition(word: &str) -> (&str, bool) {
    let word = word
        .strip_suffix(".n")
        .or_else(|| word.strip_suffix(".w"))
        .unwrap_or(word);
    for suffix in [
        "eq", "ne", "cs", "hs", "cc", "lo", "mi", "pl", "vs", "vc", "hi", "ls", "ge", "lt", "gt",
        "le", "al",
    ] {
        if let Some(base) = word.strip_suffix(suffix) {
            if matches!(
                base,
                "b" | "bl"
                    | "blx"
                    | "bx"
                    | "ldr"
                    | "str"
                    | "ldrb"
                    | "strb"
                    | "ldrh"
                    | "strh"
                    | "ldrsb"
                    | "ldrsh"
                    | "ldm"
                    | "stm"
                    | "ldmia"
                    | "stmia"
                    | "mov"
                    | "mvn"
                    | "add"
                    | "sub"
                    | "and"
                    | "orr"
                    | "eor"
                    | "bic"
                    | "cmp"
                    | "cmn"
                    | "tst"
                    | "teq"
                    | "push"
                    | "pop"
            ) {
                return (base, suffix != "al");
            }
        }
    }
    (word, false)
}

fn demand(value: &Value, summary: &mut Summary, used: &mut BTreeSet<usize>) {
    for origin in value {
        match origin {
            Origin::Word(at) => {
                used.insert(*at);
            }
            Origin::Argument(argument) => {
                summary.arguments |= 1 << argument;
            }
            Origin::Symbol(_, _) => {}
        }
    }
}

fn call(
    target: &str,
    state: &mut State,
    summaries: &BTreeMap<String, Summary>,
    summary: &mut Summary,
    used: &mut BTreeSet<usize>,
    dependencies: &mut BTreeSet<String>,
) {
    dependencies.insert(target.to_string());
    let Some(callee) = summaries.get(target) else {
        for r in [0, 1, 2, 3, 4, 12, 14] {
            state.registers[r].clear();
        }
        return;
    };
    for r in 0..4 {
        if callee.arguments & (1 << r) != 0 {
            demand(&state.registers[r], summary, used);
        }
    }
    let mut returned = Value::new();
    for origin in &callee.returned {
        if let Origin::Argument(r) = origin {
            returned.extend(state.registers[*r].iter().cloned());
        } else {
            returned.insert(origin.clone());
        }
    }
    for r in [0, 1, 2, 3, 4, 12, 14] {
        state.registers[r].clear();
    }
    state.registers[0] = returned;
}

fn run(
    program: &Program,
    start: usize,
    arguments: bool,
    summaries: &BTreeMap<String, Summary>,
    used: &mut BTreeSet<usize>,
    dependencies: &mut BTreeSet<String>,
    covered: &mut BTreeSet<usize>,
) -> Summary {
    let mut summary = Summary::default();
    let mut initial = State::default();
    if arguments {
        for r in 0..4 {
            initial.registers[r].insert(Origin::Argument(r));
        }
    }
    let mut states = BTreeMap::from([(start, initial)]);
    let mut queue = VecDeque::from([start]);
    while let Some(at) = queue.pop_front() {
        let Some(Node {
            source,
            op: Op::Instruction(word, operands),
            ..
        }) = program.nodes.get(at)
        else {
            continue;
        };
        covered.insert(at);
        let (word, conditional) = condition(word);
        let mut state = states[&at].clone();
        let before = conditional.then(|| state.clone());
        let destination = operands.first().and_then(|operand| register(operand));
        let mut successors = vec![at + 1];
        if word == "b" {
            successors = operands
                .first()
                .into_iter()
                .flat_map(|target| {
                    program
                        .labels
                        .get(&program.qualify(*source, target))
                        .into_iter()
                        .flatten()
                        .copied()
                })
                .collect();
            if conditional {
                successors.push(at + 1);
            }
            if successors.is_empty() {
                if let Some(target) = operands.first() {
                    call(
                        &program.qualify(*source, target),
                        &mut state,
                        summaries,
                        &mut summary,
                        used,
                        dependencies,
                    );
                }
            }
        } else if matches!(word, "bl" | "blx" | "bx") {
            if let Some(target) = operands.first() {
                let indirect = register(target)
                    .or_else(|| target.strip_prefix("_call_via_").and_then(register));
                if let Some(r) = indirect {
                    if word == "bx" && r == 14 {
                        summary.returned.extend(state.registers[0].iter().cloned());
                    } else {
                        demand(&state.registers[r], &mut summary, used);
                        let targets = state.registers[r].clone();
                        for target in targets {
                            if let Origin::Symbol(name, _) = target {
                                call(
                                    &name,
                                    &mut state,
                                    summaries,
                                    &mut summary,
                                    used,
                                    dependencies,
                                );
                            }
                        }
                    }
                    if word == "bx" {
                        successors.clear();
                    } else {
                        for r in [0, 1, 2, 3, 4, 12, 14] {
                            state.registers[r].clear();
                        }
                    }
                } else {
                    call(
                        &program.qualify(*source, target),
                        &mut state,
                        summaries,
                        &mut summary,
                        used,
                        dependencies,
                    );
                }
            }
        } else if word.starts_with("ldr") || word.starts_with("str") {
            let load = word.starts_with("ldr");
            if let Some(bracket) = operands.iter().position(|operand| operand.starts_with('[')) {
                let base = register(
                    operands[bracket]
                        .trim_start_matches('[')
                        .trim_end_matches(']'),
                );
                let offset = operands
                    .get(bracket + 1)
                    .and_then(|operand| {
                        integer(operand.trim_start_matches('#').trim_end_matches(']'))
                    })
                    .unwrap_or(0) as i32;
                let indexed = operands
                    .get(bracket + 1)
                    .is_some_and(|operand| register(operand.trim_end_matches(']')).is_some());
                if let Some(base) = base {
                    if base == 13 {
                        if let Some(r) = destination {
                            if load {
                                state.registers[r] =
                                    state.stack.get(&offset).cloned().unwrap_or_default();
                            } else if state.registers[r].is_empty() {
                                state.stack.remove(&offset);
                            } else {
                                state.stack.insert(offset, state.registers[r].clone());
                            }
                        }
                    } else {
                        demand(&state.registers[base], &mut summary, used);
                        let mut value = Value::new();
                        if word == "ldr" {
                            for origin in &state.registers[base] {
                                if let Origin::Symbol(name, displacement) = origin {
                                    for word in program.words(
                                        name,
                                        displacement.map_or(0, |displacement| {
                                            displacement.wrapping_add(offset)
                                        }),
                                        indexed || displacement.is_none(),
                                    ) {
                                        value.extend(program.value(word));
                                    }
                                }
                            }
                        }
                        if load {
                            if let Some(r) = destination {
                                state.registers[r] = value;
                            }
                        }
                    }
                }
            } else if load {
                if let (Some(r), Some(operand)) = (destination, operands.get(1)) {
                    let operand = program.qualify(*source, operand);
                    if let Some((name, offset)) = symbol(&operand) {
                        if operands[1].starts_with('=') {
                            state.registers[r] =
                                BTreeSet::from([Origin::Symbol(name, Some(offset))]);
                        } else {
                            state.registers[r] = program
                                .words(&name, offset, false)
                                .into_iter()
                                .flat_map(|at| program.value(at))
                                .collect();
                        }
                    } else {
                        state.registers[r] = if integer(operand.trim_start_matches('='))
                            .is_some_and(memory_address)
                        {
                            BTreeSet::from([Origin::Word(at)])
                        } else {
                            Value::new()
                        };
                    }
                }
            }
        } else if word.starts_with("ldm") || word.starts_with("stm") {
            if let Some(r) = destination {
                demand(&state.registers[r], &mut summary, used);
            }
        } else if word == "adr" {
            if let (Some(r), Some(operand)) = (destination, operands.get(1)) {
                state.registers[r] = symbol(&program.qualify(*source, operand))
                    .map(|(name, offset)| BTreeSet::from([Origin::Symbol(name, Some(offset))]))
                    .unwrap_or_default();
            }
        } else if word.starts_with("mov") {
            if let Some(r) = destination {
                let value = operands
                    .get(1)
                    .and_then(|operand| register(operand))
                    .map(|source| state.registers[source].clone())
                    .unwrap_or_default();
                if r == 15 {
                    demand(&value, &mut summary, used);
                    successors.clear();
                } else {
                    state.registers[r] = value;
                }
            }
        } else if matches!(word, "add" | "adds" | "sub" | "subs") {
            if let Some(r) = destination {
                let source = if operands.len() > 2 {
                    operands.get(1).and_then(|operand| register(operand))
                } else {
                    Some(r)
                };
                let mut value = source
                    .map(|r| state.registers[r].clone())
                    .unwrap_or_default();
                let last = operands.last().map(String::as_str).unwrap_or("");
                if let Some(offset) = integer(last.trim_start_matches('#')) {
                    let offset = if word.starts_with("sub") {
                        (offset as i32).wrapping_neg()
                    } else {
                        offset as i32
                    };
                    value = value
                        .into_iter()
                        .map(|origin| match origin {
                            Origin::Symbol(name, previous) => Origin::Symbol(
                                name,
                                previous.map(|previous| previous.wrapping_add(offset)),
                            ),
                            origin => origin,
                        })
                        .collect();
                    if r == 13 {
                        state.stack = state
                            .stack
                            .into_iter()
                            .map(|(slot, value)| (slot.wrapping_sub(offset), value))
                            .collect();
                    }
                } else if let Some(other) = register(last) {
                    value.extend(state.registers[other].iter().cloned());
                }
                state.registers[r] = value;
            }
        } else if matches!(word, "lsl" | "lsls" | "lsr" | "lsrs" | "asr" | "asrs") {
            if let Some(r) = destination {
                state.registers[r] = if operands
                    .last()
                    .and_then(|operand| integer(operand.trim_start_matches('#')))
                    == Some(0)
                {
                    operands
                        .get(1)
                        .and_then(|operand| register(operand))
                        .map(|r| state.registers[r].clone())
                        .unwrap_or_default()
                } else {
                    Value::new()
                };
            }
        } else if word == "pop" {
            let registers = operands.join(",");
            if registers.contains("pc") || registers.contains("r15") {
                summary.returned.extend(state.registers[0].iter().cloned());
                successors.clear();
            }
            for operand in operands {
                if let Some(r) = register(operand.trim_matches(['{', '}'])) {
                    state.registers[r].clear();
                }
            }
        } else if !matches!(word, "push" | "cmp" | "cmn" | "tst" | "teq" | "nop") {
            if let Some(r) = destination {
                state.registers[r].clear();
            }
        }
        if let Some(before) = before {
            state.merge(&before);
        }
        for next in successors {
            if !matches!(
                program.nodes.get(next).map(|node| &node.op),
                Some(Op::Instruction(..))
            ) {
                continue;
            }
            if let Some(old) = states.get_mut(&next) {
                if old.merge(&state) {
                    queue.push_back(next);
                }
            } else {
                states.insert(next, state.clone());
                queue.push_back(next);
            }
        }
    }
    summary
}

/// Sources must already have strings and comments removed. Every source in a
/// call/table context belongs to the same source tree and target game.
pub fn sites(sources: &[&str], types: &Types) -> Vec<Site> {
    let mut program = Program::default();
    for (index, source) in sources.iter().enumerate() {
        program.append(index, source);
    }
    let mut summaries = types
        .arguments
        .iter()
        .map(|(name, &arguments)| {
            (
                name.clone(),
                Summary {
                    arguments,
                    ..Summary::default()
                },
            )
        })
        .collect::<BTreeMap<_, _>>();
    let mut used = BTreeSet::new();
    let mut covered = BTreeSet::new();
    let mut callers = BTreeMap::<String, BTreeSet<String>>::new();
    let mut scheduled = program.entries.keys().cloned().collect::<BTreeSet<_>>();
    let mut queue = scheduled.iter().cloned().collect::<VecDeque<_>>();
    while let Some(name) = queue.pop_front() {
        scheduled.remove(&name);
        let mut found = Summary::default();
        let mut dependencies = BTreeSet::new();
        for &start in &program.entries[&name] {
            let summary = run(
                &program,
                start,
                true,
                &summaries,
                &mut used,
                &mut dependencies,
                &mut covered,
            );
            found.arguments |= summary.arguments;
            found.returned.extend(summary.returned);
        }
        for dependency in dependencies {
            callers.entry(dependency).or_default().insert(name.clone());
        }
        let current = summaries.entry(name.clone()).or_default();
        let before = current.clone();
        current.arguments |= found.arguments;
        current.returned.extend(found.returned);
        widen(&mut current.returned);
        if *current != before {
            for caller in callers.get(&name).into_iter().flatten() {
                if scheduled.insert(caller.clone()) {
                    queue.push_back(caller.clone());
                }
            }
        }
    }
    for &start in &program.roots {
        if !covered.contains(&start) {
            run(
                &program,
                start,
                false,
                &summaries,
                &mut used,
                &mut BTreeSet::new(),
                &mut covered,
            );
        }
    }
    for table in &types.tables {
        used.extend(program.words(table, 0, true));
        for source in 0..sources.len() {
            used.extend(program.words(&program.name(source, table), 0, true));
        }
    }
    for object in &types.objects {
        used.extend(program.words(object, 0, false));
        for source in 0..sources.len() {
            used.extend(program.words(&program.name(source, object), 0, false));
        }
    }
    used.into_iter()
        .filter_map(|at| {
            let node = &program.nodes[at];
            let operand = match &node.op {
                Op::Word(operand) => operand.as_str(),
                Op::Instruction(_, operands) => operands.get(1)?.trim_start_matches('='),
                _ => return None,
            };
            let value = integer(operand)?;
            memory_address(value).then_some(Site {
                source: node.source,
                line: node.line,
                value,
            })
        })
        .collect()
}

#[cfg(test)]
mod tests {
    use super::*;
    fn values(source: &str) -> Vec<u32> {
        sites(&[source], &Types::default())
            .into_iter()
            .map(|site| site.value)
            .collect()
    }
    #[test]
    fn numeric_expressions_never_resolve_symbols() {
        assert_eq!(integer("(0x02000000 + (4 << 2))"), Some(0x02000010));
        assert_eq!(integer("0x08000000 | 1"), Some(0x08000001));
        assert_eq!(integer("Buffer + 4"), None);
        assert_eq!(integer("1 / 0"), None);
    }
    #[test]
    fn branches_and_conditional_overwrites_preserve_pointer_paths() {
        for source in [
            "ldr r0, pool\nbne clear\nb use\nclear:\nmovs r0, #0\nb done\nuse:\nldr r1, [r0]\ndone:\nbx lr\npool:\n.word 0x02001000",
            "ldr r0, pool\nmoveq r0, #0\nldr r1, [r0]\nbx lr\npool:\n.word 0x02001000",
            "ldr r0, pool\nb again\nagain:\nmov r5, r0\nadds r5, #4\nldr r1, [r5]\nb again\npool:\n.word 0x02001000",
        ] { assert_eq!(values(source), vec![0x02001000], "{source}"); }
    }
    #[test]
    fn a_loaded_jump_table_entry_is_an_address_without_a_pool_xref() {
        let source = "adr r1, table\nldr r0, [r1, r2]\nbx r0\ntable:\n.word 0x08000101, Handler, 0x08000201\n";
        assert_eq!(values(source), vec![0x08000101, 0x08000201]);
        assert!(values("adr r1, table\nldr r0, [r1, r2]\nlsrs r0, #16\nbx lr\ntable:\n.word 0x08000101, 0x08000201").is_empty());
    }
    #[test]
    fn source_callee_summaries_follow_arguments_and_returns() {
        let source = ".thumb_func\ncaller:\nldr r0, pool\nbl adapter\nbx lr\npool:\n.word 0x03000100\n.thumb_func\nadapter:\nmov r2, r0\nbl reader\nbx lr\n.thumb_func\nreader:\nldr r1, [r2]\nbx lr\n";
        assert_eq!(values(source), vec![0x03000100]);
        let source = ".thumb_func\ncaller:\nbl getter\nldr r1, [r0]\nbx lr\n.thumb_func\ngetter:\nldr r0, pool\nbx lr\npool:\n.word 0x02000100\n";
        assert_eq!(values(source), vec![0x02000100]);
    }
    #[test]
    fn scalar_arguments_and_clobbered_values_stay_scalar() {
        for source in [
            "ldr r0, pool\nbl cam_bounds\nbx lr\npool:\n.word 0x08040000\n",
            "ldr r0, pool\nbl unknown\nldr r1, [r0]\nbx lr\npool:\n.word 0x02000100\n",
            "ldr r0, pool\nmovs r0, #0\nldr r1, [r0]\npool:\n.word 0x02000100\n",
        ] {
            assert!(values(source).is_empty(), "{source}");
        }
        let types = Types {
            arguments: BTreeMap::from([("reader".into(), 1)]),
            ..Types::default()
        };
        assert_eq!(
            sites(
                &["ldr r0, pool\nbl reader\nbx lr\npool:\n.word 0x02000100\n"],
                &types
            )
            .len(),
            1
        );
    }
}
