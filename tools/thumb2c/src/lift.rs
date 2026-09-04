//! Symbolic lifting of one Thumb function into a C body that GCC 2.96 at
//! -O2 recompiles to the same bytes. Values are tracked per register as
//! constants or C expressions; calls, stores, loops, and compares become
//! statements in the spellings the compiler's idioms require.

use crate::decode::{Alu, Cond, Ins, Kind, Offset, Shift, Width};
use std::collections::{BTreeMap, BTreeSet, HashMap};

#[derive(Clone, Debug, Default)]
pub struct Val {
    pub param: bool,
    pub c: Option<i32>,
    pub e: Option<String>,
    pub mem: bool,
    pub shared: bool,
    pub def_out: Option<usize>,
    pub name: Option<String>,
    pub reg: Option<u8>,
    pub pool: bool,
    /// A constant formed by shifting a constant left: `(k, shift)`.
    pub shifted: Option<(i32, u8)>,
    /// A constant formed by adding a shifted constant to a pool address:
    /// `(base, k, shift)`, the element `k` of a table of `1 << shift`-byte
    /// rows at `base`.
    pub table: Option<(u32, i32, u8)>,
}

impl Val {
    fn constant(c: i32) -> Self {
        Val {
            c: Some(c),
            ..Default::default()
        }
    }

    fn expr(e: impl Into<String>) -> Self {
        Val {
            e: Some(e.into()),
            ..Default::default()
        }
    }

    fn memory(e: String) -> Self {
        Val {
            e: Some(e),
            mem: true,
            ..Default::default()
        }
    }

    fn is_const(&self) -> bool {
        self.c.is_some()
    }

    fn call_index(&self) -> Option<usize> {
        self.e
            .as_deref()
            .and_then(|e| e.strip_prefix("@call:"))
            .and_then(|n| n.parse().ok())
    }
}

/// The lifted body plus the locals it needs declared.
pub struct Draft {
    pub lines: Vec<String>,
    pub params: Vec<String>,
    /// Declared widths of the parameters the prologue narrows.
    pub param_types: BTreeMap<String, &'static str>,
    pub consts: Vec<String>,
    /// Stack objects whose address is taken (`add rN, sp, #start`): each
    /// spans to the next taken address or the frame end, in bytes.
    pub frames: Vec<(u32, u32)>,
}

fn reg_var(n: u8) -> Option<&'static str> {
    Some(match n {
        7 => "rec",
        8 => "rec2",
        9 => "rec3",
        10 => "rec4",
        11 => "rec5",
        4 => "rec6",
        5 => "rec7",
        6 => "rec8",
        _ => return None,
    })
}

fn is_ident(s: &str) -> bool {
    let mut chars = s.chars();
    matches!(chars.next(), Some(c) if c.is_ascii_alphabetic() || c == '_')
        && chars.all(|c| c.is_ascii_alphanumeric() || c == '_')
}

fn is_word_char(c: char) -> bool {
    c.is_ascii_alphanumeric() || c == '_'
}

/// `(<inner> + <n>)` or `(<n> + <inner>)` split into its parts.
fn split_plus(e: &str) -> Option<(String, i64)> {
    let inner = e.strip_prefix('(')?.strip_suffix(')')?;
    if let Some(at) = inner.rfind(" + ") {
        let (left, right) = (&inner[..at], &inner[at + 3..]);
        if let Ok(n) = right.parse::<i64>() {
            if balanced(left) {
                return Some((left.to_string(), n));
            }
        }
    }
    if let Some(at) = inner.find(" + ") {
        let (left, right) = (&inner[..at], &inner[at + 3..]);
        if let Ok(n) = left.parse::<i64>() {
            if balanced(right) {
                return Some((right.to_string(), n));
            }
        }
    }
    None
}

fn balanced(s: &str) -> bool {
    let mut depth = 0i32;
    for c in s.chars() {
        match c {
            '(' => depth += 1,
            ')' => {
                depth -= 1;
                if depth < 0 {
                    return false;
                }
            }
            _ => {}
        }
    }
    depth == 0
}

fn replace_word(line: &str, word: &str, with: &str) -> String {
    let mut result = String::new();
    let mut rest = line;
    while let Some(at) = rest.find(word) {
        let before_ok = at == 0 || !is_word_char(rest[..at].chars().last().unwrap());
        result.push_str(&rest[..at]);
        if before_ok {
            result.push_str(with);
        } else {
            result.push_str(word);
        }
        rest = &rest[at + word.len()..];
    }
    result.push_str(rest);
    result
}

fn func_name(target: u32) -> String {
    format!("Func_{target:08x}")
}

fn label_name(target: u32) -> String {
    format!("L_{target:08x}")
}

fn contains_call(line: &str) -> bool {
    let mut rest = line;
    while let Some(at) = rest.find("Func_") {
        let tail = &rest[at + 5..];
        if tail.len() >= 9
            && tail[..8].bytes().all(|b| b.is_ascii_hexdigit())
            && tail.as_bytes()[8] == b'('
        {
            return true;
        }
        rest = &rest[at + 5..];
    }
    false
}

fn assigned_prefix(line: &str) -> Option<&str> {
    let trimmed = line.trim_start();
    let at = trimmed.find(" = ")?;
    let name = &trimmed[..at];
    is_ident(name).then_some(name)
}

fn cond_text(cond: Cond, a: &str, b: &str) -> String {
    let op = match cond {
        Cond::Eq => "==",
        Cond::Ne => "!=",
        Cond::Lt => "<",
        Cond::Ge => ">=",
        Cond::Gt => ">",
        Cond::Le => "<=",
        // The unsigned conditions compare the left operand as u32; a
        // signed local needs the cast, a u32 counter already is one.
        Cond::Ls => return format!("{} <= {b}", unsigned_operand(a)),
        Cond::Hi => return format!("{} > {b}", unsigned_operand(a)),
        Cond::Cc => return format!("{} < {b}", unsigned_operand(a)),
        Cond::Cs => return format!("{} >= {b}", unsigned_operand(a)),
        // The sign flag of a result compared against nothing: negative or not.
        Cond::Mi if b == "0" => return format!("{a} < 0"),
        Cond::Pl if b == "0" => return format!("{a} >= 0"),
        other => return format!("{a} /* {} */ {b}", other.mnemonic()),
    };
    format!("{a} {op} {b}")
}

/// The left operand of an unsigned comparison: the loop counter `i` is a
/// u32 already, every other operand is cast.
fn unsigned_operand(a: &str) -> String {
    if a == "i" || a.starts_with("(u32)") {
        a.to_string()
    } else {
        format!("(u32){a}")
    }
}

fn negate(cond: Cond) -> Cond {
    match cond {
        Cond::Eq => Cond::Ne,
        Cond::Ne => Cond::Eq,
        Cond::Lt => Cond::Ge,
        Cond::Ge => Cond::Lt,
        Cond::Gt => Cond::Le,
        Cond::Le => Cond::Gt,
        Cond::Ls => Cond::Hi,
        Cond::Hi => Cond::Ls,
        Cond::Cc => Cond::Cs,
        Cond::Cs => Cond::Cc,
        Cond::Mi => Cond::Pl,
        Cond::Pl => Cond::Mi,
        Cond::Vs => Cond::Vc,
        Cond::Vc => Cond::Vs,
    }
}

thread_local! {
    /// Whether the unit being lifted belongs to the main image, where every
    /// pool word that is a ROM or RAM address is a symbol.
    static MAIN_MODE: std::cell::Cell<bool> = const { std::cell::Cell::new(false) };
}

/// Switches the lifter between the overlay and main-image spellings.
pub fn set_main_mode(on: bool) {
    MAIN_MODE.with(|m| m.set(on));
}

pub fn main_mode() -> bool {
    MAIN_MODE.with(|m| m.get())
}

thread_local! {
    /// Typed table views the lifted bodies index, by symbol name: the
    /// declaration each needs at file scope.
    static TABLES: std::cell::RefCell<BTreeMap<String, String>> =
        const { std::cell::RefCell::new(BTreeMap::new()) };
}

/// The typed table declarations the bodies lifted so far need, drained.
pub fn take_tables() -> Vec<String> {
    TABLES.with(|t| t.borrow_mut().split_off("").into_values().collect())
}

fn width_bytes(c_type: &str) -> i64 {
    match c_type {
        "u8" | "s8" => 1,
        "u16" | "s16" => 2,
        _ => 4,
    }
}

/// In the main image, a word inside the ROM or the work RAM is a relocated
/// symbol: code when odd, data when even.
fn main_symbol(c: i32) -> Option<String> {
    let word = c as u32;
    if !main_mode() {
        return None;
    }
    let rom = (0x0800_0000..0x0a00_0000).contains(&word);
    let ram =
        (0x0200_0000..0x0204_0000).contains(&word) || (0x0300_0000..0x0300_8000).contains(&word);
    if rom && word % 2 == 1 {
        Some(format!("(s32)Func_{:08x}", word & !1))
    } else if rom || ram {
        Some(format!("(s32)Data_{word:08x}"))
    } else {
        None
    }
}

fn format_const(c: i32) -> String {
    if let Some(symbol) = main_symbol(c) {
        return symbol;
    }
    if c < 0 {
        if c >= -256 {
            format!("{c}")
        } else {
            format!("-0x{:x}", -(c as i64))
        }
    } else if c < 256 {
        format!("{c}")
    } else {
        format!("0x{c:x}")
    }
}

/// A lowered `switch`: the compared register, the (value, body index) pairs, the distinct bodies in address
/// order, and the shared exit index.
struct SwitchTree {
    subject: u8,
    /// The tree compared with `bhi`/`bls`: the subject is unsigned.
    unsigned: bool,
    cases: Vec<(u32, usize)>,
    /// Case values whose own body was a jump to another body: the source
    /// repeated that body and the compiler cross-jumped it.
    aliases: Vec<(u32, usize)>,
    bodies: Vec<usize>,
    exit: usize,
}

struct Lifter<'a> {
    ins: &'a [Ins],
    value_sites: BTreeSet<usize>,
    direct_sites: BTreeSet<usize>,
    by_addr: HashMap<u32, usize>,
    targets: BTreeSet<u32>,
    spill_slots: BTreeSet<u32>,
    /// Stack objects whose address is taken, with their byte sizes.
    frames: Vec<(u32, u32)>,
    /// Instructions the run stepped through; the rest were dropped.
    visited: BTreeSet<usize>,
    /// Register state at every `goto` to a label not yet emitted, with the
    /// output index of the goto and its indent: the label joins them with
    /// the fall-through state, as an if/else join does.
    goto_states: BTreeMap<u32, Vec<(Vec<Option<Val>>, usize, String)>>,
    regs: Vec<Option<Val>>,
    out: Vec<String>,
    indent: String,
    pending_name: Option<String>,
    /// A definition that fell inside a read-modify-write statement: it lands
    /// after that statement, where the original defines it.
    deferred: Option<String>,
    stack_args: BTreeMap<u32, Val>,
    written: BTreeSet<u8>,
    call_stmt: HashMap<usize, usize>,
    inline_calls: BTreeSet<usize>,
    parked: BTreeMap<String, (String, i64)>,
    consts: Vec<String>,
    slots: Vec<String>,
    parked_loads: Vec<String>,
    goto_targets: BTreeSet<u32>,
    emitted_labels: BTreeSet<u32>,
    params: BTreeSet<String>,
    /// Parameters the prologue narrows in place, `u16` for a `lsls`/`lsrs`
    /// pair by 16: the original declared them with that width.
    param_types: BTreeMap<String, &'static str>,
    last_cmp: Option<(String, String)>,
    /// The register the last `cmp rN, #imm` compared.
    last_cmp_reg: Option<u8>,
    /// Instructions an idiom already consumed; the run skips them.
    consumed: BTreeSet<usize>,
    /// Registers that are variables inside the loop being lifted: a write
    /// to one assigns the variable at once, so the loop's later reads and
    /// its test read the variable.
    loop_vars: BTreeMap<u8, String>,
    /// Whether such an assignment is being spelled; the hook does not nest.
    assigning: bool,
    /// The exits of the switch statements being emitted, innermost last: a
    /// jump to one is a `break`.
    switch_exits: Vec<usize>,
    /// The states the innermost switch's explicit breaks leave, with the
    /// lines of the body they broke out of, for the join at its exit.
    switch_breaks: Vec<Vec<(Vec<Option<Val>>, usize, usize, String)>>,
    /// Where the switch body being emitted began in the output.
    switch_body_start: usize,
    cursor_max: usize,
    /// The instruction being lifted.
    cursor: usize,
    /// Enclosing loops as (tail start, back-branch index).
    loops: Vec<(usize, usize)>,
}

fn push_unique(list: &mut Vec<String>, name: &str) {
    if !list.iter().any(|n| n == name) {
        list.push(name.to_string());
    }
}

impl<'a> Lifter<'a> {
    fn new(ins: &'a [Ins]) -> Self {
        let sites = crate::sched::value_calls(ins);
        let by_addr = ins.iter().enumerate().map(|(i, x)| (x.addr, i)).collect();
        let mut targets = BTreeSet::new();
        let mut spill_slots = BTreeSet::new();
        let mut taken = BTreeSet::new();
        let mut frame_size: u32 = 0;
        for x in ins {
            match x.kind {
                Kind::B { target } | Kind::Bcond { target, .. } => {
                    targets.insert(target);
                }
                Kind::LdrSp { imm, .. } => {
                    spill_slots.insert(imm);
                }
                Kind::AddSp { imm, .. } => {
                    spill_slots.insert(imm);
                    taken.insert(imm);
                }
                Kind::SpAdjust(k) if k < 0 && frame_size == 0 => {
                    frame_size = k.unsigned_abs();
                }
                _ => {}
            }
        }
        // A taken stack address starts an object that runs to the next
        // taken address or the frame end.
        let taken: Vec<u32> = taken.into_iter().collect();
        let frames: Vec<(u32, u32)> = taken
            .iter()
            .enumerate()
            .filter_map(|(k, &start)| {
                let end = taken
                    .get(k + 1)
                    .copied()
                    .unwrap_or(frame_size.max(start + 4));
                (end > start).then_some((start, end - start))
            })
            .collect();
        let mut lifter = Lifter {
            ins,
            value_sites: sites.value,
            direct_sites: sites.direct,
            by_addr,
            targets,
            spill_slots,
            frames,
            visited: BTreeSet::new(),
            goto_states: BTreeMap::new(),
            regs: vec![None; 16],
            out: Vec::new(),
            indent: "    ".to_string(),
            pending_name: None,
            deferred: None,
            stack_args: BTreeMap::new(),
            written: BTreeSet::new(),
            call_stmt: HashMap::new(),
            inline_calls: BTreeSet::new(),
            parked: BTreeMap::new(),
            consts: Vec::new(),
            slots: Vec::new(),
            parked_loads: Vec::new(),
            goto_targets: BTreeSet::new(),
            emitted_labels: BTreeSet::new(),
            params: BTreeSet::new(),
            param_types: BTreeMap::new(),
            last_cmp: None,
            last_cmp_reg: None,
            consumed: BTreeSet::new(),
            loop_vars: BTreeMap::new(),
            assigning: false,
            switch_exits: Vec::new(),
            switch_breaks: Vec::new(),
            switch_body_start: 0,
            cursor_max: 0,
            cursor: 0,
            loops: Vec::new(),
        };
        lifter.reset();
        lifter
    }

    fn reset(&mut self) {
        self.params.clear();
        self.param_types.clear();
        self.visited.clear();
        self.goto_states.clear();
        self.inline_calls.clear();
        self.cursor_max = 0;
        self.regs = vec![None; 16];
        for k in 0..4u8 {
            self.regs[k as usize] = Some(Val {
                e: Some(format!("a{k}")),
                param: true,
                ..Default::default()
            });
        }
        self.out.clear();
        self.indent = "    ".to_string();
        self.pending_name = None;
        self.stack_args.clear();
        self.written.clear();
        self.call_stmt.clear();
        self.parked.clear();
        self.consts.clear();
        self.slots.clear();
        self.parked_loads.clear();
        self.emitted_labels.clear();
        self.last_cmp = None;
    }

    /// Inserts a line, keeping every recorded output index past it valid.
    fn insert_line(&mut self, at: usize, line: String) {
        self.out.insert(at, line);
        for index in self.call_stmt.values_mut() {
            if *index >= at {
                *index += 1;
            }
        }
        for value in self.regs.iter_mut().flatten() {
            if let Some(def) = value.def_out.as_mut() {
                if *def >= at {
                    *def += 1;
                }
            }
        }
        // States recorded at earlier gotos move with the lines around them.
        for states in self.goto_states.values_mut() {
            for (regs, stop, _) in states.iter_mut() {
                if *stop >= at {
                    *stop += 1;
                }
                for value in regs.iter_mut().flatten() {
                    if let Some(def) = value.def_out.as_mut() {
                        if *def >= at {
                            *def += 1;
                        }
                    }
                }
            }
        }
    }

    fn emit(&mut self, s: impl AsRef<str>) {
        let line = s.as_ref();
        if let Some(deferred) = self.deferred.take() {
            // A block opener or a declaration comes first: the deferred
            // statement lands after them, where a statement may stand.
            let opener = line.trim_end().ends_with('{');
            let declaration = ["s32 ", "u8 ", "u16 ", "u32 "]
                .iter()
                .any(|t| line.trim_start().starts_with(t));
            if opener || declaration {
                self.out.push(format!("{}{line}", self.indent));
                self.deferred = Some(deferred);
                return;
            }
            // The definition fell between the load and the store of a byte
            // mask: the statement opens up so it lands there, where the
            // compiler picks the temporary the loaded byte no longer needs.
            if let Some((lhs, mask)) = line.strip_suffix(';').and_then(|l| l.split_once(" &= ")) {
                let (record_line, name, index) = match call_indexed(lhs) {
                    Some((call, index)) => (
                        Some(format!("u8 *record = {call};")),
                        "record".to_string(),
                        index,
                    ),
                    None => match indexed(lhs) {
                        Some((name, index)) => (None, name, index),
                        None => {
                            self.out.push(format!("{}{line}", self.indent));
                            self.out.push(format!("{}{deferred}", self.indent));
                            return;
                        }
                    },
                };
                let indent = self.indent.clone();
                self.out.push(format!("{indent}{{"));
                if let Some(record_line) = record_line {
                    self.out.push(format!("{indent}    {record_line}"));
                }
                self.out.push(format!(
                    "{indent}    u8 value = *(volatile u8 *)&{name}[{index}];"
                ));
                self.out
                    .push(format!("{indent}    s32 masked = value & {mask};"));
                self.out.push(String::new());
                self.out.push(format!("{indent}    {deferred}"));
                self.out
                    .push(format!("{indent}    {name}[{index}] = (u8)masked;"));
                self.out.push(format!("{indent}}}"));
                return;
            }
            self.out.push(format!("{}{line}", self.indent));
            self.out.push(format!("{}{deferred}", self.indent));
            return;
        }
        self.out.push(format!("{}{line}", self.indent));
    }

    /// Whether a low register holds a memory read that no statement has
    /// consumed yet: a read-modify-write is in flight.
    fn read_in_flight(&self, except: u8) -> bool {
        (0..4u8).filter(|&r| r != except).any(|r| {
            self.regs[r as usize]
                .as_ref()
                .and_then(|v| v.e.as_deref())
                .is_some_and(|e| e.contains("*("))
        })
    }

    /// A parameter register shifted left and back in place by 16 or 24 is
    /// the callee re-narrowing a `u16`, `s16`, `u8`, or `s8` parameter: the
    /// declaration carries the width and the body reads the plain name.
    fn narrow_parameter(&mut self, i: usize, shift: Shift, rd: u8, rm: u8, imm: u32) -> bool {
        if shift != Shift::Lsl || rd != rm || rd > 3 || !(imm == 16 || imm == 24) {
            return false;
        }
        let Some(v) = self.regs[rd as usize].as_ref() else {
            return false;
        };
        if !v.param || v.name.is_some() {
            return false;
        }
        let Some(name) = v.e.clone() else {
            return false;
        };
        if !name.starts_with('a') || name.len() != 2 {
            return false;
        }
        let Some(next) = self.ins.get(i + 1) else {
            return false;
        };
        let Kind::ShiftImm {
            shift: back,
            rd: rd2,
            rm: rm2,
            imm: imm2,
        } = next.kind
        else {
            return false;
        };
        if rd2 != rd || rm2 != rd || imm2 != imm {
            return false;
        }
        let c_type = match (imm, back) {
            (16, Shift::Lsr) => "u16",
            (16, Shift::Asr) => "s16",
            (24, Shift::Lsr) => "u8",
            (24, Shift::Asr) => "s8",
            _ => return false,
        };
        self.param_types.insert(name.clone(), c_type);
        self.params.insert(name);
        self.consumed.insert(i + 1);
        true
    }

    fn index_of(&self, target: u32) -> Option<usize> {
        self.by_addr.get(&target).copied()
    }

    fn fmt(&mut self, v: &Val) -> String {
        if v.param {
            if let Some(e) = &v.e {
                self.params.insert(e.clone());
            }
        }
        if let Some(name) = &v.name {
            return name.clone();
        }
        if v.call_index().is_some() {
            let mut v = v.clone();
            return self.ensure_result_var(&mut v);
        }
        if let Some(c) = v.c {
            return format_const(c);
        }
        v.e.clone().unwrap_or_else(|| "?".to_string())
    }

    fn ensure_result_var(&mut self, v: &mut Val) -> String {
        let Some(call) = v.call_index() else {
            return self.fmt(v);
        };
        if let Some(&oi) = self.call_stmt.get(&call) {
            // A result that gets used makes the call value-returning after all:
            // the void cast comes off.
            let line = &self.out[oi];
            if line.trim_start().starts_with("((void (*)())") {
                self.out[oi] = strip_void_cast(line);
            }
        }
        if self.inline_calls.contains(&call) {
            if let Some(&oi) = self.call_stmt.get(&call) {
                if oi + 1 == self.out.len()
                    && assigned_prefix(&self.out[oi]).is_none()
                    && !self.call_aliased(call)
                {
                    let text = self.out.pop().unwrap();
                    let text = text.trim().trim_end_matches(';').to_string();
                    self.call_stmt.remove(&call);
                    v.e = Some(text.clone());
                    return text;
                }
            }
        }
        let Some(&oi) = self.call_stmt.get(&call) else {
            eprintln!(
                "missing call statement for @call:{call} {} at {:x}",
                self.ins[call].text, self.ins[call].addr
            );
            return "MISSING".to_string();
        };
        if self.out[oi].trim_start().starts_with("(void)") {
            let line = &self.out[oi];
            let lead = line.len() - line.trim_start().len();
            self.out[oi] = format!("{}{}", &line[..lead], &line[lead + 6..]);
        }
        let existing = assigned_prefix(&self.out[oi]).map(str::to_string);
        let name = existing.clone().unwrap_or_else(|| {
            self.pending_name
                .take()
                .unwrap_or_else(|| "record".to_string())
        });
        self.pending_name = None;
        if existing.is_none() {
            let line = &self.out[oi];
            let lead = line.len() - line.trim_start().len();
            let mut rewritten = format!("{}{} = {}", &line[..lead], name, &line[lead..]);
            if let Some(at) = rewritten.find("= Call") {
                rewritten.replace_range(at..at + 6, "= Value");
            }
            self.out[oi] = rewritten;
        }
        v.e = Some(name.clone());
        name
    }

    /// A typed table element, `Data_<base>_t[k][column]`, when the address
    /// was formed as a pool address plus a scaled constant index and the
    /// access fits the row: the compiler scales a constant index into a
    /// typed array at run time, where a folded address would sit in the
    /// pool. The table is declared once per unit; a second shape for the
    /// same symbol keeps the folded spelling.
    fn table_expr(&mut self, base: &Val, off: i64, width: Width) -> Option<String> {
        let (address, k, shift) = base.table?;
        let c_type = width.c_type();
        let w = width_bytes(c_type);
        let stride = 1i64 << shift;
        if off < 0 || off >= stride || off % w != 0 || stride % w != 0 {
            return None;
        }
        let name = format!("Data_{address:08x}_t");
        let declaration = format!("extern {c_type} {name}[][{}];", stride / w);
        let accepted = TABLES.with(|t| {
            let mut t = t.borrow_mut();
            match t.get(&name) {
                Some(existing) => *existing == declaration,
                None => {
                    t.insert(name.clone(), declaration);
                    true
                }
            }
        });
        accepted.then(|| format!("{name}[{k}][{}]", off / w))
    }

    fn addr_expr(&mut self, base: &Val, off: i64, width: Width) -> String {
        let mut base = base.clone();
        let mut b = self.ensure_result_var(&mut base);
        let mut total = off;
        if let Some((inner, n)) = split_plus(&b) {
            b = inner;
            total += n;
        }
        let width = width.c_type();
        let simple = is_ident(&b);
        if self.parked.contains_key(&b) && total == 0 {
            return if width == "u8" {
                format!("*{b}")
            } else {
                format!("*({width} *){b}")
            };
        }
        if width == "u8" && simple {
            return format!("{b}[{total}]");
        }
        if total != 0 {
            format!("*({width} *)({b} + {total})")
        } else {
            format!("*({width} *)({b})")
        }
    }

    fn park(&mut self, name: &str, expr: &str) {
        let (base, off) = match split_plus(expr) {
            Some((inner, n)) if is_ident(&inner) => (inner, n),
            _ => (String::new(), 0),
        };
        let decl;
        let use_word;
        if !base.is_empty() {
            decl = format!("{name} = {base} + {off};");
            use_word = Some(format!("{base}[{off}]"));
        } else {
            let pointer = expr
                .strip_prefix("(*(s32 *)(")
                .and_then(|rest| rest.rsplit_once(") + "))
                .and_then(|(inner, n)| n.strip_suffix(')').map(|n| (inner, n)));
            decl = match pointer {
                Some((inner, n)) => format!("{name} = *(u8 **)({inner}) + {n};"),
                None => format!("{name} = {expr};"),
            };
            use_word = None;
        }
        let mut first = None;
        for k in (0..self.out.len()).rev() {
            let line = self.out[k].clone();
            let trimmed = line.trim_start();
            if contains_call(&line) || trimmed.starts_with('}') || trimmed.starts_with('{') {
                break;
            }
            let replaced = match &use_word {
                Some(word) => replace_word(&line, word, &format!("*{name}")),
                None => line.replace(&format!("*(u8 *){expr}"), &format!("*{name}")),
            };
            if replaced != line {
                self.out[k] = replaced;
                first = Some(k);
            }
        }
        let line = format!("{}{decl}", self.indent);
        match first {
            Some(k) => self.insert_line(k, line),
            None => self.out.push(line),
        }
        self.parked.insert(name.to_string(), (base, off));
    }

    fn name_shared(&mut self, v: &Val) -> String {
        if let Some(name) = &v.name {
            return name.clone();
        }
        let c = v.c.unwrap_or(0);
        let unsigned = c as u32;
        let name = format!("base{}_{:x}", v.reg.unwrap_or(0), unsigned);
        push_unique(&mut self.consts, &name);
        // Overlay addresses are relocated symbols: odd ones name code, even ones
        // data; a literal would be a plain pool word the scheduler treats
        // differently.
        // A constant formed by `movs`/`adds` is a literal; only a pool word
        // is a relocation.
        let lit = if (0x0200_0000..0x0201_0000).contains(&unsigned) && unsigned % 2 == 1 {
            format!("(s32){}", func_name(unsigned))
        } else if v.pool && unsigned < 0x0201_0000 {
            format!("(s32)Data_{unsigned:08x}")
        } else {
            format_const(c)
        };
        let line = format!("{}{name} = {lit};", self.indent);
        match v.def_out {
            Some(at) if at <= self.out.len() => self.insert_line(at, line),
            _ => self.out.push(line),
        }
        for slot in self.regs.iter_mut().flatten() {
            if slot.shared && slot.c == v.c && slot.reg == v.reg {
                slot.name = Some(name.clone());
            }
        }
        name
    }

    /// The name of a constant address a callee-saved register keeps across
    /// its uses: the original holds such an address in one pointer local, so
    /// every access goes through the same pseudo. The scene work pointer
    /// keeps its literal spelling, which the step helper recognises.
    fn shared_base(&mut self, base: &Val) -> Option<String> {
        let c = base.c? as u32;
        let callee_saved = base.reg.is_some_and(|r| (4..=7).contains(&r));
        if !base.shared || !callee_saved || c == 0x0300_1ebc {
            return None;
        }
        Some(self.name_shared(base))
    }

    /// A pointer-typed local read as an integer operand.
    fn arith(&mut self, v: &Val) -> String {
        let text = self.fmt(v);
        let parked = self.parked.contains_key(&text)
            || self
                .parked_loads
                .iter()
                .any(|p| p.split(':').next() == Some(text.as_str()));
        if parked {
            return format!("(s32){text}");
        }
        // A compound expression over a parked pointer, `(p5b + 1)`, is an
        // integer operand too.
        let mentions = self
            .parked
            .keys()
            .cloned()
            .chain(
                self.parked_loads
                    .iter()
                    .filter_map(|p| p.split(':').next().map(str::to_string)),
            )
            .any(|name| text.starts_with('(') && contains_word(&text, &name));
        if mentions {
            format!("(s32){text}")
        } else {
            text
        }
    }

    fn val_of(&mut self, r: u8, dest: Option<u8>) -> Val {
        if dest != Some(r) && r <= 3 {
            self.written.remove(&r);
        }
        self.regs[r as usize]
            .clone()
            .unwrap_or_else(|| Val::expr(format!("r{r}?")))
    }

    fn set_reg(&mut self, r: u8, mut v: Val) {
        if let Some(name) = self.loop_vars.get(&r).cloned() {
            if !self.assigning && v.e.as_deref() != Some(name.as_str()) {
                self.assigning = true;
                let text = self.fmt(&v);
                self.emit(format!("{name} = {text};"));
                self.assigning = false;
                v = Val::expr(name);
            }
        }
        if (5..=7).contains(&r) && v.is_const() && !v.shared {
            v.shared = true;
            v.def_out = Some(self.out.len());
            v.reg = Some(r);
        }
        // Every value remembers where it was defined: a join assigns a
        // variable there, not at the end of the path.
        if v.def_out.is_none() {
            v.def_out = Some(self.out.len());
        }
        self.regs[r as usize] = Some(v);
        if r <= 3 {
            self.written.insert(r);
        }
    }

    /// The conditional branch that jumps back to instruction `i`, if any.
    fn back_branch(&self, i: usize) -> Option<(usize, Cond)> {
        let addr = self.ins[i].addr;
        if !self.targets.contains(&addr) {
            return None;
        }
        self.ins[i + 1..]
            .iter()
            .enumerate()
            .find_map(|(k, x)| match x.kind {
                Kind::Bcond { cond, target } if target == addr => Some((i + 1 + k, cond)),
                _ => None,
            })
    }

    /// Whether r0 is read on either path out of the branch at `i` before it
    /// is written: the fall-through path and the taken path both count, so a
    /// call result an else branch stores is named rather than folded into
    /// the condition.
    fn r0_read_after(&self, i: usize) -> bool {
        if self.r0_read_from(i + 1) {
            return true;
        }
        match self.ins[i].kind {
            Kind::Bcond { target, .. } => self
                .index_of(target)
                .is_some_and(|t| t > i && self.r0_read_from(t)),
            _ => false,
        }
    }

    fn r0_read_from(&self, start: usize) -> bool {
        for x in &self.ins[start.min(self.ins.len())..] {
            let (sources, dest) = operands(&x.kind);
            if matches!(x.kind, Kind::Bl { .. }) {
                return false;
            }
            if sources.contains(&0) {
                return true;
            }
            if dest == Some(0) && writes_only(&x.kind) {
                return false;
            }
        }
        false
    }

    fn maybe_label(&mut self, i: usize) {
        let addr = self.ins[i].addr;
        if self.goto_targets.contains(&addr) && !self.emitted_labels.contains(&addr) {
            // A label a later jump comes back to heads a loop the structure
            // did not recover: the registers that loop reads before writing
            // are variables from here on, initialised before the label.
            let back = self.ins[i + 1..]
                .iter()
                .enumerate()
                .filter(|(_, x)| match x.kind {
                    Kind::B { target } | Kind::Bcond { target, .. } => target == addr,
                    _ => false,
                })
                .map(|(k, _)| i + 1 + k)
                .max();
            if let Some(back) = back {
                let whole = self.ins.len();
                self.loop_enter(i, back + 1, back + 1, whole);
            }
            self.emit(format!("{}:;", label_name(addr)));
            self.emitted_labels.insert(addr);
            // Every goto that reaches this label brought its own state;
            // a register that differs between them becomes a variable
            // assigned on each path, as at an if/else join.
            if let Some(states) = self.goto_states.remove(&addr) {
                let mut all = states;
                all.push((self.regs.clone(), self.out.len(), self.indent.clone()));
                let borrowed: Vec<(&[Option<Val>], usize, usize, &str)> = all
                    .iter()
                    .map(|(regs, stop, indent)| (regs.as_slice(), 0usize, *stop, indent.as_str()))
                    .collect();
                let end = self.ins.len();
                self.join_many(i, end, &borrowed);
            }
        }
    }

    fn is_pool_jump(&self, k: usize) -> bool {
        match self.ins[k].kind {
            Kind::B { target } => self.index_of(target) == Some(k + 1),
            _ => false,
        }
    }

    fn clear_scratch(&mut self) {
        for r in 0..4 {
            self.regs[r] = None;
        }
    }

    fn run(&mut self, mut i: usize, end: usize) {
        while i < end {
            if i < self.cursor_max {
                i = self.cursor_max;
                continue;
            }
            if self.consumed.contains(&i) {
                i += 1;
                continue;
            }
            self.maybe_label(i);
            if let Some((from, cond)) = self.back_branch(i) {
                if from < end {
                    i = self.loop_at(i, from, cond, end);
                    continue;
                }
            }
            if let Some(tree) = self.switch_at(i, end) {
                i = self.emit_switch(tree, end);
                continue;
            }
            i = self.step(i, end);
        }
    }

    fn loop_at(&mut self, i: usize, from: usize, cond: Cond, end: usize) -> usize {
        let Kind::CmpImm {
            rn: counter,
            imm: limit,
        } = self.ins[from - 1].kind
        else {
            if matches!(self.ins[from - 1].kind, Kind::CmpReg { .. }) {
                return self.do_while(i, from, cond, end);
            }
            let tail: Vec<&str> = self.ins[i.max(from.saturating_sub(6))..=from]
                .iter()
                .map(|z| z.text.as_str())
                .collect();
            self.emit(format!(
                "/* loop head {} ; tail {} */",
                self.ins[i].text,
                tail.join(" ; ")
            ));
            return self.step(i, end);
        };
        let mut tail_start = from - 1;
        while tail_start > i
            && !matches!(self.ins[tail_start - 1].kind, Kind::Bl { .. })
            && !matches!(
                self.ins[tail_start - 1].kind,
                Kind::Store { .. } | Kind::StrSp { .. }
            )
            && !self.is_pool_jump(tail_start - 1)
        {
            tail_start -= 1;
        }
        if self.is_pool_jump(tail_start) {
            tail_start += 1;
        }
        let tail_ins: Vec<usize> = (tail_start..from)
            .filter(|k| !self.is_pool_jump(*k))
            .collect();
        if tail_ins.len() == 1 {
            if let Kind::Load {
                width: Width::Word,
                rd,
                rn,
                offset: Offset::Imm(off),
            } = self.ins[tail_ins[0]].kind
            {
                if rd == counter {
                    let bv = self.val_of(rn, None);
                    let mem = match bv.c {
                        Some(c) => format!("*(s32 *)0x{:08x}", (c as u32).wrapping_add(off)),
                        None => format!("*(s32 *)({} + {off})", self.fmt(&bv)),
                    };
                    let text = match cond {
                        Cond::Ne => format!("{mem} != {limit}"),
                        Cond::Eq => format!("{mem} == {limit}"),
                        other => format!("{mem} /* {} */ {limit}", other.mnemonic()),
                    };
                    self.emit(format!("while ({text}) {{"));
                    let saved = self.indent.clone();
                    self.indent.push_str("    ");
                    self.loops.push((tail_start, from));
                    self.run(i, tail_start);
                    self.loops.pop();
                    self.indent = saved;
                    self.emit("}");
                    self.clear_scratch();
                    return from + 1;
                }
            }
        }
        let simple = tail_ins.len() == 1
            || (tail_ins.len() == 2 && matches!(self.ins[tail_ins[0]].kind, Kind::AddImm8 { .. }));
        let head = if simple {
            let c = if cond == Cond::Ls {
                format!("i < {}", limit + 1)
            } else {
                format!("i != {limit}")
            };
            format!("for (i = 0; {c}; i++) {{")
        } else if counter == 3 && cond == Cond::Ne {
            format!("for (i = 0; (i >> 16) != {limit}; i += 0x10000) {{")
        } else {
            return self.do_while(i, from, cond, end);
        };
        // The registers the body carries around are loop variables; the
        // counter is `i`. A tail instruction that is not the counter's
        // update, `adds r3, #2` walking a pointer, belongs to the body.
        let outer_vars = std::mem::take(&mut self.loop_vars);
        let whole = self.ins.len();
        let pre = self.loop_enter_except(i, from, from + 1, whole, Some(counter));
        self.emit(head);
        let saved = self.indent.clone();
        self.indent.push_str("    ");
        let inner = self.indent.clone();
        self.regs[counter as usize] = Some(Val::expr("i"));
        self.loops.push((tail_start, from));
        let body_out = self.out.len();
        self.run_filtered(i, tail_start, counter);
        for k in tail_ins {
            let counter_update = match self.ins[k].kind {
                Kind::AddImm8 { rd, .. } | Kind::SubImm8 { rd, .. } => rd == counter,
                Kind::CmpImm { rn, .. } => rn == counter,
                _ => false,
            };
            if !counter_update {
                self.step(k, from);
            }
        }
        self.loops.pop();
        self.loop_exit(&pre, body_out, &inner, from + 1, whole);
        self.loop_vars = outer_vars;
        self.indent = saved;
        self.emit("}");
        self.regs[counter as usize] = None;
        self.cursor_max = self.cursor_max.max(from + 1);
        from + 1
    }

    /// A `switch` the compiler lowered to a comparison tree: compares of one
    /// register against constants, `beq` to case bodies, and one shared exit
    /// every body branches to. Bodies follow the tree in source order.
    /// The `lo..=hi` a range node at branch `j` covers: `cmp rn, #lo; blt
    /// exit; cmp rn, #hi; bgt exit`, both branches to the same exit, and a
    /// range small enough to spell as adjacent cases.
    fn range_node(&self, j: usize, rn: u8, lo: Option<u32>, exit: usize) -> Option<(u32, u32)> {
        let lo = lo?;
        let Kind::CmpImm { rn: r, imm: hi } = self.ins.get(j + 1)?.kind else {
            return None;
        };
        let Kind::Bcond { cond, target } = self.ins.get(j + 2)?.kind else {
            return None;
        };
        if r != rn || !matches!(cond, Cond::Gt | Cond::Hi) || self.index_of(target)? != exit {
            return None;
        }
        (hi >= lo && hi - lo <= 8).then_some((lo, hi))
    }

    fn switch_at(&self, i: usize, end: usize) -> Option<SwitchTree> {
        let Kind::CmpImm { rn, .. } = self.ins[i].kind else {
            return None;
        };
        let mut j = i;
        let mut cases: Vec<(u32, usize)> = Vec::new();
        let mut aliases: Vec<(u32, usize)> = Vec::new();
        let mut exits: Vec<usize> = Vec::new();
        let mut last_value = None;
        let mut unsigned = false;
        while j < end {
            match self.ins[j].kind {
                Kind::CmpImm { rn: r, imm } if r == rn => last_value = Some(imm),
                Kind::Bcond { cond, target } => {
                    let t = self.index_of(target)?;
                    match cond {
                        Cond::Eq => cases.push((last_value?, t)),
                        Cond::Ne => {
                            cases.push((last_value?, j + 1));
                            exits.push(t);
                        }
                        // `cmp lo; blt exit; cmp hi; bgt exit` is a range node:
                        // adjacent cases sharing the body that follows it.
                        Cond::Lt | Cond::Cc if self.range_node(j, rn, last_value, t).is_some() => {
                            let (lo, hi) = self.range_node(j, rn, last_value, t)?;
                            for value in lo..=hi {
                                if !cases.iter().any(|(v, _)| *v == value) {
                                    cases.push((value, j + 3));
                                }
                            }
                            exits.push(t);
                            unsigned |= cond == Cond::Cc;
                            j += 2;
                        }
                        Cond::Hi | Cond::Ls | Cond::Gt | Cond::Le | Cond::Ge | Cond::Lt => {
                            if t <= j || t >= end {
                                return None;
                            }
                            unsigned |= matches!(cond, Cond::Hi | Cond::Ls);
                        }
                        _ => return None,
                    }
                }
                Kind::B { target } => {
                    let t = self.index_of(target)?;
                    if self.is_pool_jump(j) {
                        return None;
                    }
                    // A jump to another case's body shares that body; any other
                    // jump is the exit.
                    if cases.iter().any(|(_, b)| *b == t) {
                        for (value, _) in cases.iter().filter(|(_, b)| *b == j) {
                            aliases.push((*value, t));
                        }
                        cases.retain(|(_, b)| *b != j);
                    } else {
                        exits.push(t);
                    }
                }
                _ => break,
            }
            j += 1;
        }
        let tree_end = j;
        // Three or more compares with a shared exit is a switch; fewer is an if.
        let compares = (i..tree_end)
            .filter(|k| matches!(self.ins[*k].kind, Kind::CmpImm { .. }))
            .count();
        if compares < 3 || cases.len() < 2 || exits.is_empty() {
            return None;
        }
        let exit = *exits.iter().max()?;
        if cases
            .iter()
            .any(|(_, body)| *body < tree_end || *body > exit)
            || exit > end
        {
            return None;
        }
        // A case body reached through the tree's fall-through (`bne exit`)
        // starts right at the tree end; every other body must lie between
        // the tree and the exit.
        let mut bodies: Vec<usize> = cases.iter().map(|(_, b)| *b).collect();
        bodies.sort_unstable();
        bodies.dedup();
        Some(SwitchTree {
            subject: rn,
            unsigned,
            cases,
            aliases,
            bodies,
            exit,
        })
    }

    fn emit_switch(&mut self, tree: SwitchTree, end: usize) -> usize {
        let value = self.val_of(tree.subject, None);
        let subject = self.fmt(&value);
        let subject = if tree.unsigned {
            format!("(u32){subject}")
        } else {
            subject
        };
        let switch_at = self.out.len();
        self.emit(format!("switch ({subject}) {{"));
        let saved = self.indent.clone();
        // Every case starts from the state at the switch; the states the
        // cases leave, and the default path, meet at the exit.
        let before = self.regs.clone();
        let mut paths: Vec<(Vec<Option<Val>>, usize, usize, String)> = Vec::new();
        self.switch_exits.push(tree.exit);
        self.switch_breaks.push(Vec::new());
        // Cross-jumped cases come first: their stub sits inside the tree.
        for (value, body) in tree.aliases.clone() {
            self.indent = saved.clone();
            self.emit(format!("case {value}:"));
            self.indent = format!("{saved}    ");
            let body_end = tree
                .bodies
                .iter()
                .find(|b| **b > body)
                .copied()
                .unwrap_or(tree.exit);
            let mut stop = body_end;
            if stop > body {
                if let Kind::B { target } = self.ins[stop - 1].kind {
                    if self.index_of(target) == Some(tree.exit) {
                        stop -= 1;
                    }
                }
            }
            let mark = self.cursor_max;
            self.cursor_max = 0;
            self.regs = before.clone();
            let start = self.out.len();
            self.switch_body_start = start;
            self.run(body, stop);
            self.cursor_max = mark;
            paths.push((
                self.regs.clone(),
                start,
                self.out.len(),
                format!("{saved}    "),
            ));
            self.emit("break;");
            self.clear_scratch();
        }
        let mut fresh = true;
        for (n, body) in tree.bodies.iter().enumerate() {
            let body_end = tree.bodies.get(n + 1).copied().unwrap_or(tree.exit);
            let mut labels: Vec<u32> = tree
                .cases
                .iter()
                .filter(|(_, b)| b == body)
                .map(|(v, _)| *v)
                .collect();
            labels.sort_unstable();
            labels.dedup();
            self.indent = saved.clone();
            for label in labels {
                self.emit(format!("case {label}:"));
            }
            self.indent = format!("{saved}    ");
            // The closing jump to the exit is the `break`; a body that ends
            // without one falls through into the next case's labels.
            let mut stop = body_end;
            let mut breaks = body_end == tree.exit;
            if stop > *body {
                if let Kind::B { target } = self.ins[stop - 1].kind {
                    if self.index_of(target) == Some(tree.exit) {
                        stop -= 1;
                        breaks = true;
                    }
                }
            }
            self.cursor_max = self.cursor_max.max(*body);
            // A body entered by fall-through continues the previous state.
            if fresh {
                self.regs = before.clone();
            }
            let start = self.out.len();
            self.switch_body_start = start;
            self.run(*body, stop);
            if breaks {
                paths.push((
                    self.regs.clone(),
                    start,
                    self.out.len(),
                    format!("{saved}    "),
                ));
                self.emit("break;");
            } else {
                self.emit("/* fall through */");
            }
            fresh = breaks;
            self.clear_scratch();
        }
        self.switch_exits.pop();
        if let Some(breaks) = self.switch_breaks.pop() {
            paths.extend(breaks);
        }
        self.indent = saved.clone();
        self.emit("}");
        paths.push((before, 0, switch_at, saved.clone()));
        let borrowed: Vec<(&[Option<Val>], usize, usize, &str)> = paths
            .iter()
            .map(|(state, start, stop, indent)| (state.as_slice(), *start, *stop, indent.as_str()))
            .collect();
        self.join_many(tree.exit, end, &borrowed);
        self.cursor_max = self.cursor_max.max(tree.exit);
        tree.exit
    }

    fn run_filtered(&mut self, mut i: usize, end: usize, counter: u8) {
        while i < end {
            let skip = match self.ins[i].kind {
                Kind::AddImm8 { rd, .. } => rd == counter,
                Kind::CmpImm { rn, .. } => rn == counter,
                _ => false,
            };
            if skip {
                i += 1;
                continue;
            }
            i = self.step(i, end);
        }
    }

    fn call_result_name_for(&mut self, dest: u8, v: &Val) {
        if let Some(name) = reg_var(dest) {
            if v.call_index().is_some() {
                self.pending_name = Some(name.to_string());
                let mut v = v.clone();
                self.ensure_result_var(&mut v);
            }
        }
    }

    fn value_name_for_shift(&mut self, rm: u8) {
        if let Some(v0) = self.regs[rm as usize].clone() {
            if v0.call_index().is_some() {
                self.pending_name = Some("value".to_string());
                let mut v0 = v0;
                self.ensure_result_var(&mut v0);
            }
        }
    }

    fn binary(&mut self, rd: u8, rn: u8, rm: u8, op: &str, fold: fn(i32, i32) -> i32) {
        let v = self.val_of(rn, Some(rd));
        let w = self.val_of(rm, Some(rd));
        let value = match (v.c, w.c) {
            (Some(a), Some(b)) => {
                let mut value = Val::constant(fold(a, b));
                // A pool address plus a shifted constant is a typed table
                // element: the compiler scales a constant index into a typed
                // array at run time rather than folding it into the pool.
                if op == "+" {
                    let pair = if v.pool && w.shifted.is_some() {
                        Some((a as u32, w.shifted))
                    } else if w.pool && v.shifted.is_some() {
                        Some((b as u32, v.shifted))
                    } else {
                        None
                    };
                    if let Some((base, Some((k, shift)))) = pair {
                        value.table = Some((base, k, shift));
                    }
                }
                value
            }
            _ => {
                let a = self.arith(&v);
                let b = self.arith(&w);
                Val::expr(format!("({a} {op} {b})"))
            }
        };
        self.set_reg(rd, value);
    }

    fn step(&mut self, i: usize, end: usize) -> usize {
        self.cursor = i;
        self.visited.insert(i);
        self.cursor_max = self.cursor_max.max(i + 1);
        self.maybe_label(i);
        let x = &self.ins[i];
        let addr = x.addr;
        let flags = flag_register(&x.kind);
        match x.kind.clone() {
            // The return address popped into r1 rather than r0 leaves r0
            // for the value the function returns.
            Kind::Pop {
                pc: false,
                list: 0b10,
            } if matches!(self.ins.get(i + 1).map(|n| &n.kind), Some(Kind::Bx(1))) => {
                if self.regs[0].is_some() {
                    let mut v = self.val_of(0, None);
                    let text = if v.call_index().is_some() {
                        self.ensure_result_var(&mut v)
                    } else {
                        self.fmt(&v)
                    };
                    self.emit(format!("return {text};"));
                }
            }
            Kind::Push { .. }
            | Kind::Pop { .. }
            | Kind::Bx(_)
            | Kind::SpAdjust(_)
            | Kind::Nop
            | Kind::Unknown(_) => {}
            Kind::AddSp { rd, imm } => self.set_reg(rd, Val::expr(format!("&slot{imm}"))),
            Kind::AddPc { rd, imm } => {
                let target = (addr & !3).wrapping_add(4).wrapping_add(imm);
                self.set_reg(rd, Val::expr(format!("(s32)Data_{target:08x}")));
            }
            Kind::AddHi { rd, rm } => {
                if rd == 13 || rm == 13 {
                    return i + 1;
                }
                let v = self.val_of(rd, Some(rd));
                let w = self.val_of(rm, Some(rd));
                let value = match (v.c, w.c) {
                    (Some(a), Some(b)) => Val::constant(a.wrapping_add(b)),
                    _ => {
                        let mut v = v;
                        let a = self.ensure_result_var(&mut v);
                        let b = self.fmt(&w);
                        Val::expr(format!("({a} + {b})"))
                    }
                };
                self.set_reg(rd, value);
            }
            Kind::MovImm { rd, imm } => {
                // A constant in a callee-saved register is a local the
                // original defined before the preceding call and used after
                // it; the scheduler moved the move below the call. The
                // assignment lands above that call so the local is live
                // across it and takes the same register.
                let previous_call = (5..=7).contains(&rd).then(|| {
                    let indent = self.indent.clone();
                    self.out.iter().rposition(|line| {
                        line.starts_with(&indent)
                            && !line[indent.len()..].starts_with(' ')
                            && contains_call(line)
                    })
                });
                match previous_call.flatten() {
                    Some(at) if at + 1 == self.out.len() && self.read_straight(i + 1, rd) => {
                        let name = format!("v{rd}");
                        push_unique(&mut self.consts, &name);
                        let line = format!("{}{name} = {};", self.indent, format_const(imm as i32));
                        self.insert_line(at, line);
                        self.set_reg(rd, Val::expr(name));
                    }
                    _ => self.set_reg(rd, Val::constant(imm as i32)),
                }
            }
            Kind::Movs { rd, rm } => {
                let v = self.val_of(rm, Some(rd));
                self.set_reg(rd, v);
            }
            Kind::MovHi { rd, rm } => {
                if rd == rm {
                    return i + 1;
                }
                let v = self.val_of(rm, Some(rd));
                if (8..=11).contains(&rd) && v.c == Some(0) {
                    if self.read_in_flight(rm) {
                        self.deferred = Some("none = 0;".to_string());
                    } else {
                        self.emit("none = 0;");
                    }
                    push_unique(&mut self.consts, "none");
                    self.regs[rm as usize] = Some(Val::expr("none"));
                    self.set_reg(rd, Val::expr("none"));
                    return i + 1;
                }
                if (4..=11).contains(&rd) && v.is_const() {
                    let sv = Val {
                        c: v.c,
                        shared: true,
                        def_out: Some(self.out.len()),
                        reg: Some(rd),
                        ..Default::default()
                    };
                    self.regs[rm as usize] = Some(sv.clone());
                    self.set_reg(rd, sv);
                    return i + 1;
                }
                if (4..=11).contains(&rd) {
                    if let Some(e) = v.e.clone() {
                        if v.call_index().is_some() {
                            self.pending_name = reg_var(rd).map(str::to_string);
                            let mut v2 = v.clone();
                            self.ensure_result_var(&mut v2);
                        } else if !is_plain_local(&e) && !e.ends_with('?') {
                            let name = self.fresh_park_name(rd, "u8 *");
                            self.park(&name, &e);
                            self.regs[rm as usize] = Some(Val::expr(name.clone()));
                            self.set_reg(rd, Val::expr(name));
                            return i + 1;
                        }
                    }
                }
                self.set_reg(rd, v);
            }
            Kind::ShiftImm { shift, rd, rm, imm } => {
                if self.narrow_parameter(i, shift, rd, rm, imm) {
                    return i + 1;
                }
                self.value_name_for_shift(rm);
                let v = self.val_of(rm, Some(rd));
                let n = imm;
                let value = match (shift, v.c) {
                    (Shift::Lsl, Some(c)) => {
                        let mut value = Val::constant(c.wrapping_shl(n));
                        if (1..=4).contains(&n) && c > 0 {
                            value.shifted = Some((c, n as u8));
                        }
                        value
                    }
                    (Shift::Lsr, Some(c)) => Val::constant(((c as u32) >> n) as i32),
                    (Shift::Asr, Some(c)) => Val::constant(c >> n),
                    (Shift::Lsl, None) => Val::expr(format!("({} << {n})", self.arith(&v))),
                    // A logical shift right is unsigned in the original.
                    (Shift::Lsr, None) => {
                        let text = self.arith(&v);
                        Val::expr(format!("({} >> {n})", unsigned_operand(&text)))
                    }
                    (Shift::Asr, None) => Val::expr(format!("({} >> {n})", self.arith(&v))),
                };
                self.set_reg(rd, value);
            }
            Kind::Alu { op, rd, rm } => match op {
                Alu::Mul => self.binary(rd, rd, rm, "*", |a, b| a.wrapping_mul(b)),
                Alu::Neg => {
                    let v = self.val_of(rm, Some(rd));
                    let value = match v.c {
                        Some(c) => Val::constant(c.wrapping_neg()),
                        None => Val::expr(format!("-{}", self.fmt(&v))),
                    };
                    self.set_reg(rd, value);
                }
                Alu::Orr => self.binary_expr(rd, rm, "|"),
                Alu::And => self.binary_expr(rd, rm, "&"),
                Alu::Eor => self.binary_expr(rd, rm, "^"),
                Alu::Lsl => self.binary_expr(rd, rm, "<<"),
                Alu::Asr => self.binary_expr(rd, rm, ">>"),
                Alu::Lsr => {
                    let a = self.val_of(rd, Some(rd));
                    let b = self.val_of(rm, Some(rd));
                    let left = self.arith(&a);
                    let text = format!("({} >> {})", unsigned_operand(&left), self.arith(&b));
                    self.set_reg(rd, Val::expr(text));
                }
                // Bit clear: `a & ~b`, with a constant mask folded.
                Alu::Bic => {
                    let a = self.val_of(rd, Some(rd));
                    let b = self.val_of(rm, Some(rd));
                    let left = self.arith(&a);
                    let text = match b.c {
                        Some(c) => format!("({left} & {})", format_const(!c)),
                        None => format!("({left} & ~{})", self.arith(&b)),
                    };
                    self.set_reg(rd, Val::expr(text));
                }
                Alu::Mvn => {
                    let v = self.val_of(rm, Some(rd));
                    let text = self.fmt(&v);
                    self.set_reg(rd, Val::expr(format!("~{text}")));
                }
                Alu::Tst | Alu::Cmn => {
                    let v = self.val_of(rd, None);
                    let w = self.val_of(rm, None);
                    let a = self.arith(&v);
                    let b = self.arith(&w);
                    let op = if op == Alu::Tst { "&" } else { "+" };
                    self.last_cmp = Some((format!("({a} {op} {b})"), "0".to_string()));
                }
                _ => self.emit(format!("/* {} */", x.text.clone())),
            },
            Kind::AddImm8 { rd, imm } => {
                if rd == 0 {
                    if let Some(call) = self.regs[0].as_ref().and_then(Val::call_index) {
                        self.inline_calls.insert(call);
                    }
                }
                let v = self.val_of(rd, None);
                let n = imm as i32;
                if v.shared && v.is_const() {
                    let name = self.name_shared(&v);
                    self.set_reg(rd, Val::expr(format!("({name} + {n})")));
                } else if let Some(c) = v.c {
                    self.set_reg(rd, Val::constant(c.wrapping_add(n)));
                } else {
                    let mut v = v;
                    let text = self.ensure_result_var(&mut v);
                    self.set_reg(rd, Val::expr(format!("({text} + {n})")));
                }
            }
            Kind::AddImm3 { rd, rn, imm } => {
                let n = imm as i32;
                let source = self.regs[rn as usize].clone();
                if n != 0 && source.as_ref().is_some_and(|s| s.shared && s.is_const()) {
                    let name = self.name_shared(source.as_ref().unwrap());
                    self.set_reg(rd, Val::expr(format!("({name} + {n})")));
                    return i + 1;
                }
                let v = self.val_of(rn, Some(rd));
                if n == 0 && (4..=11).contains(&rd) {
                    self.call_result_name_for(rd, &v);
                }
                let value = if n == 0 {
                    v
                } else if let Some(c) = v.c {
                    Val::constant(c.wrapping_add(n))
                } else {
                    let mut v = v;
                    let text = self.ensure_result_var(&mut v);
                    Val::expr(format!("({text} + {n})"))
                };
                self.set_reg(rd, value);
            }
            Kind::AddReg { rd, rn, rm } => {
                let (v, w) = (
                    self.regs[rn as usize].clone(),
                    self.regs[rm as usize].clone(),
                );
                // Two pool words added together: one is a relocated symbol, the
                // other its offset; folding them would lose a pool entry.
                if let (Some(v), Some(w)) = (v, w) {
                    if v.pool && w.pool && v.is_const() && w.is_const() {
                        let (a, b) = (v.c.unwrap() as u32, w.c.unwrap() as u32);
                        let (symbol, offset) = if a >= b { (a, b) } else { (b, a) };
                        self.val_of(rn, Some(rd));
                        self.val_of(rm, Some(rd));
                        // Through a local the symbol stays a register operand; as a
                        // direct sum the assembler folds symbol and offset into one
                        // relocation and the second pool word disappears.
                        let name = format!("base{rd}_{symbol:x}");
                        push_unique(&mut self.consts, &name);
                        self.emit(format!("{name} = (s32)Data_{symbol:08x};"));
                        self.set_reg(rd, Val::expr(format!("({name} + 0x{offset:x})")));
                        return i + 1;
                    }
                }
                self.binary(rd, rn, rm, "+", |a, b| a.wrapping_add(b))
            }
            Kind::SubImm8 { rd, imm } => {
                let v = self.val_of(rd, None);
                let n = imm as i32;
                let value = match v.c {
                    Some(c) => Val::constant(c.wrapping_sub(n)),
                    None => Val::expr(format!("({} - {n})", self.fmt(&v))),
                };
                self.set_reg(rd, value);
            }
            Kind::SubImm3 { rd, rn, imm } => {
                let v = self.val_of(rn, Some(rd));
                let n = imm as i32;
                let value = match v.c {
                    Some(c) => Val::constant(c.wrapping_sub(n)),
                    None => Val::expr(format!("({} - {n})", self.fmt(&v))),
                };
                self.set_reg(rd, value);
            }
            Kind::SubReg { rd, rn, rm } => {
                let v = self.val_of(rn, Some(rd));
                let w = self.val_of(rm, Some(rd));
                let a = self.arith(&v);
                let b = self.arith(&w);
                self.set_reg(rd, Val::expr(format!("({a} - {b})")));
            }
            Kind::LdrPool { rd, word } => {
                let c = word as i32;
                // A word the assembler could have built with `movs` (and one
                // shift) only reaches the pool as a relocated data symbol.
                if cheap(word) {
                    self.set_reg(
                        rd,
                        Val {
                            e: Some(format!("(s32)Data_{word:08x}")),
                            pool: true,
                            ..Default::default()
                        },
                    );
                    return i + 1;
                }
                if (4..=11).contains(&rd) {
                    self.set_reg(
                        rd,
                        Val {
                            c: Some(c),
                            shared: true,
                            def_out: Some(self.out.len()),
                            reg: Some(rd),
                            pool: true,
                            ..Default::default()
                        },
                    );
                } else {
                    self.set_reg(
                        rd,
                        Val {
                            c: Some(c),
                            pool: true,
                            ..Default::default()
                        },
                    );
                }
            }
            Kind::LdrSp { rd, imm } => self.set_reg(rd, Val::expr(format!("slot{imm}"))),
            Kind::Load {
                width,
                rd,
                rn,
                offset,
            } => self.load(rd, rn, offset, width),
            Kind::StrSp { rd, imm } => {
                let v = self.val_of(rd, None);
                if self.spill_slots.contains(&imm) {
                    let name = format!("slot{imm}");
                    let text = self.fmt(&v);
                    self.emit(format!("{name} = {text};"));
                    push_unique(&mut self.slots, &name);
                } else {
                    self.stack_args.insert(imm / 4, v);
                }
            }
            Kind::Store {
                width,
                rd,
                rn,
                offset,
            } => self.store(rd, rn, offset, width),
            // A multiple transfer with writeback is `x = *p; p += 4` (the
            // compiler's auto-increment pass folds that back into ldmia):
            // the loaded values are variables at once, since the base moves.
            Kind::Ldmia { rn, list } => {
                let mut base = self.val_of(rn, None);
                let b = self.ensure_result_var(&mut base);
                let regs: Vec<u8> = (0..8u8).filter(|r| list & (1 << r) != 0).collect();
                for (k, r) in regs.iter().enumerate() {
                    let e = if k == 0 {
                        format!("*(s32 *)({b})")
                    } else {
                        format!("*(s32 *)({b} + {})", 4 * k)
                    };
                    let name = format!("v{r}");
                    push_unique(&mut self.consts, &name);
                    self.emit(format!("{name} = {e};"));
                    self.set_reg(*r, Val::expr(name));
                }
                if !regs.contains(&rn) {
                    let text = format!("({b} + {})", 4 * regs.len());
                    self.set_reg(rn, Val::expr(text));
                }
            }
            Kind::Stmia { rn, list } => {
                let mut base = self.val_of(rn, None);
                let b = self.ensure_result_var(&mut base);
                let regs: Vec<u8> = (0..8u8).filter(|r| list & (1 << r) != 0).collect();
                for (k, r) in regs.iter().enumerate() {
                    let v = self.val_of(*r, None);
                    let value = self.fmt(&v);
                    let lhs = if k == 0 {
                        format!("*(s32 *)({b})")
                    } else {
                        format!("*(s32 *)({b} + {})", 4 * k)
                    };
                    self.emit(format!("{lhs} = {value};"));
                }
                let text = format!("({b} + {})", 4 * regs.len());
                self.set_reg(rn, Val::expr(text));
            }
            Kind::CmpImm { rn, imm } => {
                // A compared register is consumed: it is not a call argument.
                let v = self.val_of(rn, None);
                let a = self.fmt_raw(&v);
                self.last_cmp = Some((a, imm.to_string()));
                self.last_cmp_reg = Some(rn);
            }
            Kind::CmpReg { rn, rm } => {
                let v = self.val_of(rn, None);
                let w = self.val_of(rm, None);
                let a = self.fmt_raw(&v);
                let b = self.fmt(&w);
                self.last_cmp = Some((a, b));
            }
            Kind::Bl { target } => self.call(i, target),
            Kind::B { target } => return self.branch(i, target, end),
            Kind::Bcond { cond, target } => return self.conditional(i, cond, target, end),
        }
        if let Some(r) = flags {
            if let Some(v) = self.regs[r as usize].clone() {
                let a = self.fmt_raw(&v);
                self.last_cmp = Some((a, "0".to_string()));
            }
        }
        i + 1
    }

    fn binary_expr(&mut self, rd: u8, rm: u8, op: &str) {
        let v = self.val_of(rd, Some(rd));
        let w = self.val_of(rm, Some(rd));
        let a = self.arith(&v);
        let b = self.arith(&w);
        self.set_reg(rd, Val::expr(format!("({a} {op} {b})")));
    }

    fn load(&mut self, rd: u8, rn: u8, offset: Offset, width: Width) {
        if let Offset::Reg(rm) = offset {
            let index = self.val_of(rm, None);
            let base = self.val_of(rn, None);
            let c_type = width.c_type();
            let e = match index.c {
                Some(c) => match self.table_expr(&base, i64::from(c), width) {
                    Some(e) => e,
                    None => self.addr_expr(&base, i64::from(c), width),
                },
                // A register index that is not a constant stays in the address.
                None => {
                    let b = match base.c {
                        Some(c) => format!("0x{:08x}", c as u32),
                        None => self.arith(&base),
                    };
                    let ix = self.arith(&index);
                    format!("*({c_type} *)({b} + {ix})")
                }
            };
            if index.c.is_none() && self.park_load(rd, &e, c_type) {
                return;
            }
            self.set_reg(rd, Val::memory(e));
            return;
        }
        let Offset::Imm(off) = offset else { return };
        if let Some(slot) = self.regs[rn as usize]
            .as_ref()
            .and_then(|v| v.e.as_deref())
            .and_then(|e| e.strip_prefix("&slot"))
        {
            let e = format!("slot{slot}");
            self.set_reg(rd, Val::memory(e));
            return;
        }
        let base = self.val_of(rn, None);
        let c_type = width.c_type();
        if let Some(c) = base.c {
            let e = match self.table_expr(&base, i64::from(off), width) {
                Some(e) => e,
                None => match self.shared_base(&base) {
                    Some(name) => deref_named_at(c_type, &name, off, c as u32),
                    None => absolute_deref(c_type, (c as u32).wrapping_add(off)),
                },
            };
            // A load into a callee-saved register is a value kept across calls,
            // a local in the original: the scene work pointer read once at entry
            // and used from r5 is the common case.
            if self.park_load(rd, &e, c_type) {
                return;
            }
            self.set_reg(rd, Val::memory(e));
            return;
        }
        let e = self.addr_expr(&base, i64::from(off), width);
        if self.park_load(rd, &e, c_type) {
            return;
        }
        self.set_reg(rd, Val::memory(e));
    }

    /// `pN`, or `pNb`, `pNc`, when the register was already parked with
    /// another type: one local cannot be both a pointer and an integer.
    fn fresh_park_name(&self, rd: u8, c_type: &str) -> String {
        for suffix in ["", "b", "c", "d", "e"] {
            let candidate = format!("p{rd}{suffix}");
            let conflict = self.parked.contains_key(&candidate)
                || self.parked_loads.iter().any(|p| {
                    p.split(':').next() == Some(candidate.as_str())
                        && p.split(':').nth(1) != Some(c_type)
                });
            if !conflict {
                return candidate;
            }
        }
        format!("p{rd}")
    }

    fn park_load(&mut self, rd: u8, e: &str, c_type: &str) -> bool {
        if !(4..=11).contains(&rd) {
            return false;
        }
        let name = self.fresh_park_name(rd, c_type);
        self.emit(format!("{name} = {e};"));
        push_unique(&mut self.parked_loads, &format!("{name}:{c_type}"));
        self.set_reg(rd, Val::memory(name));
        true
    }

    fn store(&mut self, rd: u8, rn: u8, offset: Offset, width: Width) {
        let v = self.val_of(rd, None);
        let c_type = width.c_type();
        let Offset::Imm(off) = offset else {
            let Offset::Reg(rm) = offset else { return };
            let mut base = self.val_of(rn, None);
            let index = self.val_of(rm, None);
            if let Some(e) = index
                .c
                .and_then(|c| self.table_expr(&base, i64::from(c), width))
            {
                let value = self.fmt(&v);
                self.emit(format!("{e} = {value};"));
                return;
            }
            let b = self.ensure_result_var(&mut base);
            let ix = self.fmt(&index);
            let value = self.fmt(&v);
            self.emit(format!("*({c_type} *)({b} + {ix}) = {value};"));
            return;
        };
        let base = self.val_of(rn, None);
        let lhs = match base.c {
            Some(c) => match self.table_expr(&base, i64::from(off), width) {
                Some(e) => e,
                None => match self.shared_base(&base) {
                    Some(name) => deref_named_at(c_type, &name, off, c as u32),
                    None => absolute_deref(c_type, (c as u32).wrapping_add(off)),
                },
            },
            None => self.addr_expr(&base, i64::from(off), width),
        };
        // A register still holding a lazy read of the location this store
        // overwrites, and read again later, is captured first: the original
        // read it before writing.
        let mut captured: Option<String> = None;
        for r in 0..13usize {
            let Some(held) = self.regs[r].clone() else {
                continue;
            };
            if !held.mem || held.name.is_some() || self.fmt_raw(&held) != lhs {
                continue;
            }
            if r as u8 == rd || !self.live_after(self.cursor + 1, self.ins.len(), r as u8) {
                continue;
            }
            let name = match &captured {
                Some(name) => name.clone(),
                None => {
                    let name = format!("v{r}");
                    push_unique(&mut self.consts, &name);
                    self.emit(format!("{name} = {lhs};"));
                    captured = Some(name.clone());
                    name
                }
            };
            self.regs[r] = Some(Val::expr(name));
        }
        let rhs = self.fmt(&v);
        let parts = rhs
            .strip_prefix('(')
            .and_then(|r| r.strip_suffix(')'))
            .and_then(|inner| {
                inner
                    .find(" | ")
                    .map(|at| (&inner[..at], "|", &inner[at + 3..]))
                    .or_else(|| {
                        inner
                            .find(" & ")
                            .map(|at| (&inner[..at], "&", &inner[at + 3..]))
                    })
            });
        if let Some((left, op, right)) = parts {
            let other = if left == lhs {
                Some(right)
            } else if right == lhs {
                Some(left)
            } else {
                None
            };
            if let Some(other) = other {
                let other = other.to_string();
                // A single-bit set is a compound OR: the loaded byte stays the first
                // operand, so the result lands in its register, not the constant's.
                // The constant in the destination register means the original
                // spelled the constant first, the byte-mode form; the loaded byte
                // in the destination is a compound OR.
                if op == "|" && right == lhs {
                    if let Some((record, index)) = indexed(&lhs) {
                        let previous = self.out.last().cloned();
                        let call = previous.as_deref().and_then(|line| {
                            let trimmed = line.trim();
                            (record == "record")
                                .then(|| trimmed.strip_prefix("record = "))
                                .flatten()
                                .filter(|rest| rest.starts_with("Func_") && rest.ends_with(");"))
                                .map(|rest| rest.trim_end_matches(';').to_string())
                        });
                        if call.is_some() {
                            self.out.pop();
                        }
                        self.emit("{");
                        if let Some(call) = call {
                            self.emit(format!("    u8 *record = {call};"));
                        }
                        self.emit(format!(
                            "    u8 value = *(volatile u8 *)&{record}[{index}];"
                        ));
                        self.emit("");
                        self.emit(format!("    {lhs} = (u8)(value | {other});"));
                        self.emit("}");
                        return;
                    }
                    if let Some((call, index)) = call_indexed(&lhs) {
                        self.emit("{");
                        self.emit(format!("    u8 *record = {call};"));
                        self.emit(format!("    u8 value = *(volatile u8 *)&record[{index}];"));
                        self.emit("");
                        self.emit(format!("    record[{index}] = (u8)(value | {other});"));
                        self.emit("}");
                        return;
                    }
                }
                self.emit(format!("{lhs} {op}= {other};"));
                return;
            }
        }
        if let Some(rest) = rhs.strip_prefix(&format!("({lhs} + ")) {
            let amount = rest.strip_suffix(')').unwrap_or(rest);
            self.emit(format!("{lhs} += {amount};"));
            return;
        }
        // A quotient stored as a halfword goes through an int local too: the
        // store address is then computed after the division, as the original
        // orders it.
        let quotient = rhs.contains(" / ");
        if width == Width::Half && ((v.is_const() && !v.pool) || quotient) {
            // A halfword store of a literal is a HImode constant, which this
            // compiler loads from the pool; through an int local it is SImode
            // and a `movs`.
            let (hoist, target) = match hoisted_call(&lhs) {
                Some((call, rest)) => (Some(format!("u8 *record = {call};")), rest),
                None => match hoisted_load(&lhs) {
                    Some((load, rest)) => (Some(format!("s32 target = {load};")), rest),
                    // A computed address goes into a pointer first: the int
                    // local's pseudo then lives only across the store, so
                    // the address keeps the register the original gives it
                    // (395:02000158 went from 4 differing halfwords to exact).
                    None => match half_store_address(&lhs) {
                        Some(address) => (
                            Some(format!("u16 *target = (u16 *)({address});")),
                            "*target".to_string(),
                        ),
                        None => (None, lhs.clone()),
                    },
                },
            };
            self.emit("{");
            if let Some(hoist) = hoist {
                self.emit(format!("    {hoist}"));
            }
            self.emit(format!("    s32 shown = {rhs};"));
            self.emit("");
            self.emit(format!("    {target} = shown;"));
            self.emit("}");
            return;
        }
        self.emit(format!("{lhs} = {rhs};"));
    }

    fn call(&mut self, i: usize, target: u32) {
        let mut n = 0u8;
        for r in 0..4u8 {
            if self.written.contains(&r) {
                n = r + 1;
            }
        }
        for r in 0..n {
            if self.regs[r as usize].is_none() && !self.written.contains(&r) {
                n = r;
                break;
            }
        }
        let mut args: Vec<String> = Vec::new();
        let mut shared_index = BTreeSet::new();
        for r in 0..n {
            let written = self.written.contains(&r);
            let v = self.val_of(r, None);
            if v.shared {
                shared_index.insert(r as usize);
            }
            let text =
                if v.shared && v.is_const() && (v.name.is_some() || (v.pool && v.reg.is_some())) {
                    self.name_shared(&v)
                } else if written {
                    self.fmt(&v)
                } else {
                    let mut v = v;
                    self.ensure_result_var(&mut v)
                };
            args.push(text);
        }
        let stack: Vec<Val> = self.stack_args.values().cloned().collect();
        for v in &stack {
            let text = self.fmt(v);
            args.push(text);
        }
        for arg in args.iter_mut().take(n as usize) {
            if let Some(hex) = arg.strip_prefix("0x0200") {
                if hex.len() == 4 && hex.bytes().all(|b| b.is_ascii_hexdigit()) {
                    let value = u32::from_str_radix(&arg[2..], 16).unwrap_or(0);
                    if value % 2 == 1 {
                        *arg = format!("(s32){}", func_name(value));
                    }
                }
            }
        }
        let costly = args.iter().take(n as usize).enumerate().any(|(k, a)| {
            if shared_index.contains(&k) {
                return false;
            }
            if a.starts_with("-0x") || a.starts_with("(s32)Func_") {
                return true;
            }
            if let Some(digits) = a.strip_prefix('-') {
                return digits.bytes().all(|b| b.is_ascii_digit()) && !digits.is_empty();
            }
            if let Some(hex) = a.strip_prefix("0x") {
                return i64::from_str_radix(hex, 16).is_ok_and(|v| v >= 256);
            }
            a.bytes().all(|b| b.is_ascii_digit()) && a.parse::<i64>().is_ok_and(|v| v >= 256)
        });
        let count = args.len();
        let direct = self.direct_sites.contains(&i);
        let wrap = (costly || count >= 5) && count > 0 && count != 5 && !direct;
        let callee = func_name(target);
        let value = self.value_sites.contains(&i);
        let callee_returns_value = self
            .value_sites
            .iter()
            .any(|s| matches!(self.ins[*s].kind, Kind::Bl { target: t } if t == target));
        let call = if value && direct && count != 5 {
            // A direct call whose result the reference discarded still sets r0.
            format!("(void){callee}({})", args.join(", "))
        } else if value && count != 5 {
            if count == 0 {
                format!("Value0({callee})")
            } else {
                format!("Value{count}({callee}, {})", args.join(", "))
            }
        } else if !wrap && callee_returns_value {
            // A void site of a callee declared as returning a value keeps the
            // direct call shape through a cast.
            format!("((void (*)()){callee})({})", args.join(", "))
        } else if wrap {
            format!("Call{count}({callee}, {})", args.join(", "))
        } else {
            format!("{callee}({})", args.join(", "))
        };
        self.emit(format!("{call};"));
        self.call_stmt.insert(i, self.out.len() - 1);
        self.regs[0] = Some(Val::expr(format!("@call:{i}")));
        for r in 1..4 {
            self.regs[r] = None;
        }
        self.written.clear();
        self.stack_args.clear();
    }

    /// Whether any register other than r0 still refers to call `call`.
    fn call_aliased(&self, call: usize) -> bool {
        self.regs
            .iter()
            .enumerate()
            .skip(1)
            .any(|(_, v)| v.as_ref().and_then(Val::call_index) == Some(call))
            || self
                .stack_args
                .values()
                .any(|v| v.call_index() == Some(call))
    }

    fn pop_call_condition(&mut self, a: &str, extra_guard: bool) -> String {
        let Some(call) = a
            .strip_prefix("@call:")
            .and_then(|n| n.parse::<usize>().ok())
        else {
            return a.to_string();
        };
        let oi = self.call_stmt.get(&call).copied();
        if oi == Some(self.out.len() - 1)
            && assigned_prefix(&self.out[oi.unwrap()]).is_none()
            && extra_guard
            && !self.call_aliased(call)
        {
            let text = self.out.pop().unwrap();
            let mut text = strip_void_cast(text.trim().trim_end_matches(';'));
            if text.starts_with("Call") {
                text.replace_range(0..4, "Value");
            }
            self.call_stmt.remove(&call);
            return text;
        }
        let mut v = Val::expr(a);
        self.ensure_result_var(&mut v)
    }

    fn branch(&mut self, i: usize, target: u32, end: usize) -> usize {
        let Some(t) = self.index_of(target) else {
            self.jump(target);
            return i + 1;
        };
        if t == i + 1 {
            return i + 1;
        }
        if t > i && t <= end {
            for l in i + 1..t {
                let Some((from, cond)) = self.back_branch(l) else {
                    continue;
                };
                if from < t {
                    continue;
                }
                let outer_vars = std::mem::take(&mut self.loop_vars);
                let whole = self.ins.len();
                let pre = self.loop_enter(l, from, from + 1, whole);
                let mark = self.out.len();
                // The test sits past the body: lifting it first must not
                // move the cursor beyond the body, which comes next.
                let cursor = self.cursor_max;
                self.run(t, from);
                self.cursor_max = cursor;
                let (ca, cb) = match self.ins[from - 1].kind {
                    Kind::CmpImm { rn, imm } => {
                        let v = self.val_of(rn, None);
                        (self.fmt_raw(&v), imm.to_string())
                    }
                    Kind::CmpReg { rn, rm } => {
                        let v = self.val_of(rn, None);
                        let w = self.val_of(rm, None);
                        let b = self.fmt(&w);
                        (self.fmt_raw(&v), b)
                    }
                    _ => ("?".to_string(), "?".to_string()),
                };
                let ca = self.pop_call_condition(&ca, self.out.len() == mark + 1);
                let text = cond_text(cond, &ca, &cb);
                self.emit(format!("while ({text}) {{"));
                let saved = self.indent.clone();
                self.indent.push_str("    ");
                let inner = self.indent.clone();
                self.loops.push((t, from));
                let body_out = self.out.len();
                self.run(l, t);
                self.loops.pop();
                self.loop_exit(&pre, body_out, &inner, from + 1, whole);
                self.loop_vars = outer_vars;
                self.indent = saved;
                self.emit("}");
                self.cursor_max = self.cursor_max.max(from + 1);
                return from + 1;
            }
            // Code the jump skips that a goto elsewhere reaches is lifted in
            // place under its label, entered with no fall-through state.
            if (i + 1..t).any(|k| self.goto_targets.contains(&self.ins[k].addr)) {
                self.jump(target);
                self.regs = vec![None; 16];
                self.last_cmp = None;
                return i + 1;
            }
            return t;
        }
        self.jump(target);
        i + 1
    }

    /// `continue` into the enclosing loop's tail, `break` past it, else a goto.
    fn jump(&mut self, target: u32) {
        let index = self.index_of(target);
        if index.is_some() && index == self.switch_exits.last().copied() {
            // The state at an explicit break reaches the switch's exit.
            let path = (
                self.regs.clone(),
                self.switch_body_start,
                self.out.len(),
                self.indent.clone(),
            );
            if let Some(breaks) = self.switch_breaks.last_mut() {
                breaks.push(path);
            }
            self.emit("break;");
            return;
        }
        if let (Some(index), Some(&(tail_start, from))) = (index, self.loops.last()) {
            if index >= tail_start && index <= from {
                self.emit("continue;");
                return;
            }
            if index == from + 1 {
                self.emit("break;");
                return;
            }
        }
        // The label will merge this state with the others reaching it.
        if !self.emitted_labels.contains(&target) {
            self.goto_states.entry(target).or_default().push((
                self.regs.clone(),
                self.out.len(),
                self.indent.clone(),
            ));
        }
        self.emit(format!("goto {};", label_name(target)));
        self.goto_targets.insert(target);
    }

    /// Formats a value without resolving a call result, so the caller can
    /// decide whether the call statement folds into a condition.
    fn fmt_raw(&mut self, v: &Val) -> String {
        if v.call_index().is_some() {
            return v.e.clone().unwrap();
        }
        self.fmt(v)
    }

    /// `cmp rX, #0; bge L; adds rX, (1 << k) - 1; L: asrs rX, rX, #k` is the
    /// compiler's signed division by a power of two: the quotient replaces
    /// the register and nothing is emitted.
    fn signed_division(
        &mut self,
        i: usize,
        cond: Cond,
        t: Option<usize>,
        a: &str,
        b: &str,
    ) -> Option<usize> {
        let t = t?;
        let rx = self.last_cmp_reg?;
        if cond != Cond::Ge || b != "0" || t <= i + 1 || t >= self.ins.len() {
            return None;
        }
        // The shift may sit a few independent instructions past the join.
        let mention = format!("r{rx}");
        let mut shift_at = None;
        for j in t..(t + 4).min(self.ins.len()) {
            if let Kind::ShiftImm {
                shift: Shift::Asr,
                rd,
                rm,
                imm,
            } = &self.ins[j].kind
            {
                if *rd == rx && *rm == rx && *imm != 0 {
                    shift_at = Some((j, *imm));
                    break;
                }
            }
            if self.ins[j].text.contains(&mention) {
                return None;
            }
        }
        let (shift_at, k) = shift_at?;
        let rounding = (1u64 << k) - 1;
        let body = &self.ins[i + 1..t];
        let added = match body {
            [one] => match &one.kind {
                Kind::AddImm8 { rd, imm } if *rd == rx => u64::from(*imm),
                _ => return None,
            },
            [pool, add] => match (&pool.kind, &add.kind) {
                (Kind::LdrPool { rd: rc, word }, Kind::AddReg { rd, rn, rm })
                    if *rd == rx && *rn == rx && rm == rc =>
                {
                    u64::from(*word)
                }
                _ => return None,
            },
            _ => return None,
        };
        if added != rounding {
            return None;
        }
        let dividend = self.arith(&Val::expr(a.to_string()));
        self.set_reg(
            rx,
            Val::expr(format!("({dividend} / {})", format_const(1i32 << k))),
        );
        self.consumed.insert(shift_at);
        Some(t)
    }

    fn conditional(&mut self, i: usize, cond: Cond, target: u32, end: usize) -> usize {
        let t = self.index_of(target);
        let (a, b) = self
            .last_cmp
            .clone()
            .unwrap_or_else(|| ("0".to_string(), "0".to_string()));
        if let Some(next) = self.signed_division(i, cond, t, &a, &b) {
            return next;
        }
        // A branch past the region's end leaves it: that is a break, continue
        // or goto under the condition, never a body that swallows what
        // follows the region.
        if let Some(t) = t.filter(|t| *t > i && *t <= end) {
            let a = if a.starts_with("@call:") {
                let guard = !self.r0_read_after(i);
                self.pop_call_condition(&a, guard)
            } else {
                a
            };
            let text = cond_text(negate(cond), &a, &b);
            let if_at = self.out.len();
            self.emit(format!("if ({text}) {{"));
            let saved = self.indent.clone();
            self.indent.push_str("    ");
            let body_indent = self.indent.clone();
            let last = &self.ins[t - 1];
            // An else block ends inside the current region; a jump beyond it
            // is a break, continue, or goto handled when the body is lifted.
            let else_end = match last.kind {
                Kind::B { target } => self.index_of(target).filter(|e| *e > t && *e <= end),
                _ => None,
            };
            let before = self.regs.clone();
            self.run(i + 1, if else_end.is_some() { t - 1 } else { t });
            let then_state = self.regs.clone();
            let then_at = self.out.len();
            if let Some(else_end) = else_end {
                self.indent = saved.clone();
                self.emit("} else {");
                let else_start = self.out.len();
                self.indent.push_str("    ");
                self.regs = before;
                self.run(t, else_end);
                let else_state = self.regs.clone();
                let else_at = self.out.len();
                self.indent = saved;
                self.emit("}");
                self.join(
                    else_end,
                    end,
                    (&then_state, if_at + 1, then_at, &body_indent),
                    (&else_state, else_start, else_at, &body_indent),
                );
                return else_end;
            }
            self.indent = saved.clone();
            self.emit("}");
            self.join(
                t,
                end,
                (&then_state, if_at + 1, then_at, &body_indent),
                (&before, 0, if_at, &saved),
            );
            return t;
        }
        let a = if a.starts_with("@call:") {
            let mut v = Val::expr(a);
            self.ensure_result_var(&mut v)
        } else {
            a
        };
        let text = cond_text(cond, &a, &b);
        self.emit(format!("if ({text}) {{"));
        let saved = self.indent.clone();
        self.indent.push_str("    ");
        self.jump(target);
        self.indent = saved;
        self.emit("}");
        i + 1
    }

    /// Merges the register states of two paths meeting at instruction `at`.
    /// A register the paths leave with different values, and that is read
    /// again before it is written, is a variable the original assigned on
    /// each path: the assignments land at the end of each path (for the
    /// fall-through path, before the `if`), and the merged state reads the
    /// variable. A register nothing reads again is simply unknown.
    fn join(
        &mut self,
        at: usize,
        end: usize,
        then: (&[Option<Val>], usize, usize, &str),
        other: (&[Option<Val>], usize, usize, &str),
    ) {
        self.join_many(at, end, &[then, other]);
    }

    /// The N-path form of `join`: every path that reaches `at` contributes
    /// its state, the lines it emitted (`start..stop`), and its indent.
    fn join_many(&mut self, at: usize, end: usize, paths: &[(&[Option<Val>], usize, usize, &str)]) {
        // The assignment lands where the path defined the value when that
        // lies inside the path at its own block level; otherwise at the
        // path's end.
        let place = |out: &[String], v: &Val, start: usize, stop: usize, indent: &str| {
            let at_level =
                |line: &String| line.starts_with(indent) && !line[indent.len()..].starts_with(' ');
            match v.def_out {
                Some(def) if def > start && def <= stop && out.get(def).is_some_and(at_level) => {
                    def
                }
                _ => stop,
            }
        };
        let unnamed_call = |v: &Option<Val>| {
            v.as_ref()
                .is_some_and(|v| v.e.as_deref().is_some_and(|e| e.starts_with("@call:")))
        };
        let mut inserts: Vec<(usize, String)> = Vec::new();
        for r in 0..13u8 {
            let values: Vec<&Option<Val>> = paths.iter().map(|p| &p.0[r as usize]).collect();
            let first = values[0];
            if values.iter().all(|v| same_val(v, first)) {
                self.regs[r as usize] = first.clone();
                continue;
            }
            // A register one path leaves undefined cannot be read after the
            // join in a correct original; keep a defined value. A call result
            // is named where it is consumed, not here.
            let defined = values
                .iter()
                .find(|v| v.is_some())
                .copied()
                .cloned()
                .flatten();
            // An unnamed call result on one path has a statement the other
            // paths may already have consumed: the merged register is unknown.
            if values.iter().any(|v| unnamed_call(v)) {
                self.regs[r as usize] = None;
                continue;
            }
            if values.iter().any(|v| v.is_none()) || !self.live_after(at, end, r) {
                self.regs[r as usize] = defined;
                continue;
            }
            let name = format!("v{r}");
            push_unique(&mut self.consts, &name);
            let texts: Vec<String> = values
                .iter()
                .map(|v| self.fmt_raw(v.as_ref().expect("defined on every path")))
                .collect();
            // A path value that contains another path's value derives from
            // it, `-x` from `x`: it reads the variable rather than repeating
            // the expression, which a volatile access could not.
            for (k, path) in paths.iter().enumerate() {
                let mut text = texts[k].clone();
                for (j, from) in texts.iter().enumerate() {
                    if j != k && from.len() > 2 && text != *from && contains_word(&text, from) {
                        text = replace_word(&text, from, &name);
                        break;
                    }
                }
                let v = values[k].as_ref().expect("defined on every path");
                let position = place(&self.out, v, path.1, path.2, path.3);
                inserts.push((position, format!("{}{name} = {text};", path.3)));
            }
            self.regs[r as usize] = Some(Val::expr(name));
        }
        // Later positions first, so earlier insertions do not shift them;
        // equal positions keep their discovery order.
        inserts.sort_by(|x, y| y.0.cmp(&x.0));
        let mut k = 0;
        while k < inserts.len() {
            let position = inserts[k].0;
            let group_end = inserts[k..]
                .iter()
                .position(|(p, _)| *p != position)
                .map(|n| k + n)
                .unwrap_or(inserts.len());
            for (_, line) in inserts[k..group_end].iter().rev() {
                self.insert_line(position, line.clone());
            }
            k = group_end;
        }
    }

    /// Registers a loop body writes; a call inside it writes r0–r3.
    fn loop_writes(&self, start: usize, stop: usize) -> BTreeSet<u8> {
        let mut written = BTreeSet::new();
        for k in start..stop.min(self.ins.len()) {
            written.extend(writes(&self.ins[k].kind));
            if matches!(self.ins[k].kind, Kind::Bl { .. }) {
                written.extend(0..4u8);
            }
        }
        written
    }

    /// Whether the body reads `r` before it writes it: the value that
    /// enters the loop is carried around it.
    fn read_before_write(&self, start: usize, stop: usize, r: u8) -> bool {
        for k in start..stop.min(self.ins.len()) {
            let kind = &self.ins[k].kind;
            if crate::sched::reads(kind).contains(&r) {
                return true;
            }
            // A call's arguments are set inside the body before it; the
            // call itself reads nothing that entered the loop.
            if matches!(kind, Kind::Bl { .. }) {
                if r <= 3 {
                    return false;
                }
                continue;
            }
            if writes(kind).contains(&r) {
                return false;
            }
        }
        false
    }

    /// Before a loop body: a register the body writes and either reads first
    /// or leaves live past the loop is a variable, `vN`, holding the value
    /// that enters the loop. A parameter register keeps its own name. The
    /// state that enters the loop is returned for `loop_exit`.
    fn loop_enter(
        &mut self,
        start: usize,
        stop: usize,
        after: usize,
        end: usize,
    ) -> Vec<Option<Val>> {
        self.loop_enter_except(start, stop, after, end, None)
    }

    /// `loop_enter` with one register left alone: the counter a counted
    /// loop spells as `i`.
    fn loop_enter_except(
        &mut self,
        start: usize,
        stop: usize,
        after: usize,
        end: usize,
        skip: Option<u8>,
    ) -> Vec<Option<Val>> {
        for r in self.loop_writes(start, stop) {
            if r > 12 || Some(r) == skip {
                continue;
            }
            let Some(v) = self.regs[r as usize].clone() else {
                continue;
            };
            let variable = format!("v{r}");
            if v.e.as_deref() == Some(variable.as_str()) {
                continue;
            }
            // Only a value the body reads before it writes is carried
            // around the loop; a register the body merely leaves live is
            // assigned where the body defines it, on the way out.
            let _ = (after, end);
            if !self.read_before_write(start, stop, r) {
                continue;
            }
            if v.param
                && v.e
                    .as_deref()
                    .is_some_and(|e| e.len() == 2 && e.starts_with('a'))
            {
                self.loop_vars.insert(r, v.e.clone().unwrap_or_default());
                continue;
            }
            // A constant a callee-saved register already held is initialised
            // where the register was set, as the original did (`count = 0;`
            // before the calls that precede the loop).
            if v.shared && v.is_const() {
                let name = self.name_shared(&v);
                self.loop_vars.insert(r, name.clone());
                self.regs[r as usize] = Some(Val::expr(name));
                continue;
            }
            let text = self.fmt(&v);
            push_unique(&mut self.consts, &variable);
            self.emit(format!("{variable} = {text};"));
            self.loop_vars.insert(r, variable.clone());
            self.regs[r as usize] = Some(Val::expr(variable));
        }
        self.regs.clone()
    }

    /// After a loop body: a register the body left holding another value
    /// than the one that entered is assigned where the body defined it,
    /// when the loop reads the variable again or the code after it does.
    fn loop_exit(
        &mut self,
        pre: &[Option<Val>],
        body_out: usize,
        indent: &str,
        after: usize,
        end: usize,
    ) {
        let mut inserts: Vec<(usize, String)> = Vec::new();
        for r in 0..13u8 {
            let now = self.regs[r as usize].clone();
            let before = &pre[r as usize];
            if same_val(&now, before) {
                continue;
            }
            let Some(now) = now else {
                self.regs[r as usize] = before.clone();
                continue;
            };
            let variable = match before {
                Some(b) if b.param && b.e.as_deref().is_some_and(|e| e.len() == 2) => {
                    b.e.clone().unwrap_or_default()
                }
                _ => format!("v{r}"),
            };
            let entered = before
                .as_ref()
                .is_some_and(|b| b.e.as_deref() == Some(variable.as_str()));
            if !entered && !self.live_after(after, end, r) {
                continue;
            }
            if now.e.as_deref() == Some(variable.as_str()) {
                continue;
            }
            // A call result is named by the formatter, `record`, so the
            // variable takes it where the call happened.
            let text = self.fmt(&now);
            let at_level =
                |line: &String| line.starts_with(indent) && !line[indent.len()..].starts_with(' ');
            let position = match now.def_out {
                Some(def)
                    if def > body_out
                        && def <= self.out.len()
                        && self.out.get(def).is_some_and(at_level) =>
                {
                    def
                }
                _ => self.out.len(),
            };
            inserts.push((position, format!("{indent}{variable} = {text};")));
            push_unique(&mut self.consts, &variable);
            self.regs[r as usize] = Some(Val::expr(variable));
        }
        inserts.sort_by(|x, y| y.0.cmp(&x.0));
        let mut k = 0;
        while k < inserts.len() {
            let position = inserts[k].0;
            let group_end = inserts[k..]
                .iter()
                .position(|(p, _)| *p != position)
                .map(|n| k + n)
                .unwrap_or(inserts.len());
            for (_, line) in inserts[k..group_end].iter().rev() {
                self.insert_line(position, line.clone());
            }
            k = group_end;
        }
    }

    /// A loop tested at its bottom that is not a counted `for`: the body
    /// runs once before the test, so it is a `do` loop over its variables.
    fn do_while(&mut self, i: usize, from: usize, cond: Cond, _end: usize) -> usize {
        let outer_vars = std::mem::take(&mut self.loop_vars);
        // Liveness past the loop follows the code wherever it continues.
        let whole = self.ins.len();
        let pre = self.loop_enter(i, from, from + 1, whole);
        self.emit("do {");
        let saved = self.indent.clone();
        self.indent.push_str("    ");
        let inner = self.indent.clone();
        self.loops.push((from, from));
        let body_out = self.out.len();
        self.run(i, from - 1);
        self.loops.pop();
        self.loop_exit(&pre, body_out, &inner, from + 1, whole);
        let (ca, cb) = match self.ins[from - 1].kind {
            Kind::CmpImm { rn, imm } => {
                let v = self.val_of(rn, None);
                (self.fmt_raw(&v), imm.to_string())
            }
            Kind::CmpReg { rn, rm } => {
                let v = self.val_of(rn, None);
                let w = self.val_of(rm, None);
                let b = self.fmt(&w);
                (self.fmt_raw(&v), b)
            }
            _ => ("?".to_string(), "?".to_string()),
        };
        self.indent = saved;
        let text = cond_text(cond, &ca, &cb);
        self.emit(format!("}} while ({text});"));
        self.loop_vars = outer_vars;
        self.cursor_max = self.cursor_max.max(from + 1);
        from + 1
    }

    /// Whether register `r` is read on the straight line from `at` before
    /// any write to it, call, or branch.
    fn read_straight(&self, at: usize, r: u8) -> bool {
        let mut j = at;
        while j < self.ins.len() {
            let kind = &self.ins[j].kind;
            if crate::sched::reads(kind).contains(&r) {
                return true;
            }
            if writes(kind).contains(&r)
                || matches!(
                    kind,
                    Kind::Bl { .. }
                        | Kind::B { .. }
                        | Kind::Bcond { .. }
                        | Kind::Bx(_)
                        | Kind::Pop { pc: true, .. }
                )
            {
                return false;
            }
            j += 1;
        }
        false
    }

    /// Whether register `r` is read at or after instruction `at` before it
    /// is written, scanning the straight line until the region ends. A call
    /// may read an argument register; a jump leaves the answer unknown, so
    /// both count as live.
    fn live_after(&self, at: usize, _end: usize, r: u8) -> bool {
        // The code after a region is whatever the machine executes next:
        // the walk follows every branch, both ways for a conditional one,
        // and stops at a return or at a write to the register.
        let mut pending = vec![at];
        let mut visited = BTreeSet::new();
        while let Some(start) = pending.pop() {
            let mut j = start;
            while j < self.ins.len() && visited.insert(j) {
                let kind = &self.ins[j].kind;
                if crate::sched::reads(kind).contains(&r) {
                    return true;
                }
                match kind {
                    Kind::Pop { pc: true, .. } | Kind::Bx(_) => break,
                    Kind::B { target } => match self.index_of(*target) {
                        Some(next) => {
                            j = next;
                            continue;
                        }
                        None => return true,
                    },
                    Kind::Bcond { target, .. } => match self.index_of(*target) {
                        Some(taken) => pending.push(taken),
                        None => return true,
                    },
                    _ => {}
                }
                if writes(kind).contains(&r) {
                    break;
                }
                j += 1;
            }
        }
        false
    }
}

/// Whether two register states hold the same value.
fn same_val(a: &Option<Val>, b: &Option<Val>) -> bool {
    match (a, b) {
        (None, None) => true,
        (Some(x), Some(y)) => x.c == y.c && x.e == y.e && x.name == y.name && x.mem == y.mem,
        _ => false,
    }
}

/// Registers an instruction writes.
fn writes(kind: &Kind) -> Vec<u8> {
    match *kind {
        Kind::MovImm { rd, .. }
        | Kind::MovHi { rd, .. }
        | Kind::Movs { rd, .. }
        | Kind::LdrPool { rd, .. }
        | Kind::LdrSp { rd, .. }
        | Kind::Load { rd, .. }
        | Kind::AddImm3 { rd, .. }
        | Kind::AddImm8 { rd, .. }
        | Kind::AddReg { rd, .. }
        | Kind::AddHi { rd, .. }
        | Kind::SubImm3 { rd, .. }
        | Kind::SubImm8 { rd, .. }
        | Kind::SubReg { rd, .. }
        | Kind::ShiftImm { rd, .. }
        | Kind::Alu { rd, .. } => vec![rd],
        Kind::Bl { .. } => vec![0, 1, 2, 3, 12, 14],
        Kind::Ldmia { list, .. } => (0..8u8).filter(|b| list & (1 << b) != 0).collect(),
        _ => vec![],
    }
}

/// Whether `movs` plus at most one `lsls` builds the word.
fn cheap(word: u32) -> bool {
    (0..32).any(|shift| (word >> shift) < 256 && (word >> shift) << shift == word)
}

/// `((void (*)())Func_x)(args)` back to `Func_x(args)`.
fn strip_void_cast(line: &str) -> String {
    let Some(at) = line.find("((void (*)())") else {
        return line.to_string();
    };
    let rest = &line[at + 13..];
    let Some(close) = rest.find(')') else {
        return line.to_string();
    };
    format!("{}{}{}", &line[..at], &rest[..close], &rest[close + 1..])
}

fn is_plain_local(e: &str) -> bool {
    if e == "record" || e == "i" {
        return true;
    }
    if let Some(rest) = e.strip_prefix("rec") {
        return rest.bytes().all(|b| b.is_ascii_digit());
    }
    if let Some(rest) = e.strip_prefix('p') {
        let digits = rest.trim_end_matches(|c: char| c.is_ascii_lowercase());
        return !digits.is_empty()
            && digits.bytes().all(|b| b.is_ascii_digit())
            && rest.len() - digits.len() <= 1;
    }
    false
}

/// `*(W *)(Func_x(...) + n)` as the call and the same store through `record`.
fn hoisted_call(lhs: &str) -> Option<(String, String)> {
    let (prefix, inner) = lhs.split_once("*)(")?;
    let inner = inner.strip_suffix(')')?;
    let (call, offset) = inner.rsplit_once(" + ")?;
    let is_call =
        call.starts_with("Func_") || call.starts_with("Call") || call.starts_with("Value");
    if !is_call || !call.ends_with(')') || !balanced(call) {
        return None;
    }
    Some((call.to_string(), format!("{prefix}*)(record + {offset})")))
}

/// `*(u16 *)(*(s32 *)(record + 80) + 30)` split into the loaded base and
/// `*(u16 *)(target + 30)`. The base loads before the constant is born, so
/// the constant has the shorter life and takes the first allocation
/// register, as the compiler orders them.
fn hoisted_load(lhs: &str) -> Option<(String, String)> {
    let (prefix, inner) = lhs.split_once("*)(")?;
    let inner = inner.strip_suffix(')')?;
    let (base, offset) = inner.rsplit_once(" + ")?;
    if !base.starts_with("*(") || !balanced(base) {
        return None;
    }
    Some((base.to_string(), format!("{prefix}*)(target + {offset})")))
}

/// `*(s32 *)(base7_200c41c + 4)`, or without the offset.
fn deref_named(c_type: &str, name: &str, off: u32) -> String {
    if off == 0 {
        format!("*({c_type} *){name}")
    } else {
        format!("*({c_type} *)({name} + {off})")
    }
}

/// Whether `name` occurs in `text` as a whole identifier.
fn contains_word(text: &str, name: &str) -> bool {
    let boundary = |c: Option<char>| !c.is_some_and(|c| c.is_ascii_alphanumeric() || c == '_');
    let mut rest = text;
    while let Some(at) = rest.find(name) {
        let before = rest[..at].chars().last();
        let after = rest[at + name.len()..].chars().next();
        if boundary(before) && boundary(after) {
            return true;
        }
        rest = &rest[at + name.len()..];
    }
    false
}

/// An absolute access. An address inside the overlay image is a relocated
/// data symbol: two such addresses stay two pool words, where two literals
/// become one base and an offset.
/// The computed address of a halfword store, `*(u16 *)(<expr>)`, when the
/// expression is more than a name.
fn half_store_address(lhs: &str) -> Option<String> {
    let inner = lhs.strip_prefix("*(u16 *)(")?.strip_suffix(')')?;
    // A named base plus an offset stays inline (the scene modules match
    // that way); an address that itself loads a pointer is hoisted.
    let loads = inner.contains("*(");
    loads.then(|| inner.to_string())
}

/// `deref_named` through a base kept in a register, volatile when the base
/// is a hardware register block.
fn deref_named_at(c_type: &str, name: &str, off: u32, address: u32) -> String {
    let text = deref_named(c_type, name, off);
    if (0x0400_0000..0x0500_0000).contains(&address) {
        text.replacen(
            &format!("*({c_type} *)"),
            &format!("*(volatile {c_type} *)"),
            1,
        )
    } else {
        text
    }
}

fn absolute_deref(c_type: &str, address: u32) -> String {
    if main_mode() && main_symbol(address as i32).is_some() {
        return format!("*({c_type} *)Data_{address:08x}");
    }
    // A hardware register is volatile in the original: every access stays.
    if (0x0400_0000..0x0500_0000).contains(&address) {
        return format!("*(volatile {c_type} *)0x{address:08x}");
    }
    if (0x0200_0000..0x0201_0000).contains(&address) {
        format!("*({c_type} *)Data_{address:08x}")
    } else {
        format!("*({c_type} *)0x{address:08x}")
    }
}

/// `name[index]` split into its parts.
fn indexed(lhs: &str) -> Option<(String, u32)> {
    let (name, rest) = lhs.split_once('[')?;
    let index = rest.strip_suffix(']')?.parse().ok()?;
    is_ident(name).then(|| (name.to_string(), index))
}

/// `*(u8 *)(Func_xxxxxxxx(...) + index)` split into call and index.
fn call_indexed(lhs: &str) -> Option<(String, u32)> {
    let inner = lhs.strip_prefix("*(u8 *)(")?.strip_suffix(')')?;
    let (call, index) = inner.rsplit_once(" + ")?;
    if !call.starts_with("Func_") || !call.ends_with(')') || !balanced(call) {
        return None;
    }
    Some((call.to_string(), index.parse().ok()?))
}

/// The register whose new value sets the flags a following branch tests.
fn flag_register(kind: &Kind) -> Option<u8> {
    match *kind {
        Kind::MovImm { rd, .. }
        | Kind::Movs { rd, .. }
        | Kind::AddImm3 { rd, .. }
        | Kind::AddImm8 { rd, .. }
        | Kind::AddReg { rd, .. }
        | Kind::SubImm3 { rd, .. }
        | Kind::SubImm8 { rd, .. }
        | Kind::SubReg { rd, .. }
        | Kind::ShiftImm { rd, .. } => Some(rd),
        Kind::Alu { op, rd, .. } if !matches!(op, Alu::Tst | Alu::Cmp | Alu::Cmn) => Some(rd),
        _ => None,
    }
}

/// Registers an instruction reads, and the register it writes.
fn operands(kind: &Kind) -> (Vec<u8>, Option<u8>) {
    match *kind {
        Kind::MovImm { rd, .. } | Kind::LdrPool { rd, .. } | Kind::LdrSp { rd, .. } => {
            (vec![], Some(rd))
        }
        Kind::MovHi { rd, rm } | Kind::Movs { rd, rm } => (vec![rm], Some(rd)),
        Kind::AddImm3 { rd, rn, .. } | Kind::SubImm3 { rd, rn, .. } => (vec![rn], Some(rd)),
        Kind::AddImm8 { rd, .. } | Kind::SubImm8 { rd, .. } => (vec![], Some(rd)),
        Kind::AddReg { rd, rn, rm } | Kind::SubReg { rd, rn, rm } => (vec![rn, rm], Some(rd)),
        Kind::AddHi { rd, rm } => (vec![rm], Some(rd)),
        Kind::ShiftImm { rd, rm, .. } => (vec![rm], Some(rd)),
        Kind::Alu { rd, rm, .. } => (vec![rm], Some(rd)),
        Kind::CmpImm { rn, .. } => (vec![rn], None),
        Kind::CmpReg { rn, rm } => (vec![rn, rm], None),
        Kind::Load { rd, rn, offset, .. } => {
            let mut sources = vec![rn];
            if let Offset::Reg(rm) = offset {
                sources.push(rm);
            }
            (sources, Some(rd))
        }
        Kind::Store { rd, rn, offset, .. } => {
            let mut sources = vec![rd, rn];
            if let Offset::Reg(rm) = offset {
                sources.push(rm);
            }
            (sources, None)
        }
        Kind::StrSp { rd, .. } => (vec![rd], None),
        Kind::AddSp { rd, .. } | Kind::AddPc { rd, .. } => (vec![], Some(rd)),
        _ => (vec![], None),
    }
}

fn writes_only(kind: &Kind) -> bool {
    matches!(
        kind,
        Kind::MovImm { .. }
            | Kind::MovHi { .. }
            | Kind::Movs { .. }
            | Kind::LdrPool { .. }
            | Kind::LdrSp { .. }
            | Kind::Load { .. }
            | Kind::AddImm3 { .. }
            | Kind::AddImm8 { .. }
            | Kind::AddReg { .. }
            | Kind::SubImm3 { .. }
            | Kind::SubImm8 { .. }
            | Kind::SubReg { .. }
            | Kind::ShiftImm { .. }
            | Kind::Alu { op: Alu::Neg, .. }
    )
}

/// Lifts one function. Two passes: the first discovers goto targets so the
/// second can place labels before their first use.
pub fn lift(ins: &[Ins]) -> Draft {
    let mut lifter = Lifter::new(ins);
    lifter.run(0, ins.len());
    lifter.reset();
    lifter.run(0, ins.len());
    let mut lines = lifter.out.clone();
    for target in lifter.goto_targets.difference(&lifter.emitted_labels) {
        lines.push(format!("    {}:;", label_name(*target)));
    }
    // Instructions the run never stepped through and no idiom consumed are
    // code the draft dropped; the ranges are reported at the top so a batch
    // can measure what the structure misses.
    let structural = |kind: &Kind| {
        matches!(
            kind,
            Kind::B { .. }
                | Kind::Bcond { .. }
                | Kind::CmpImm { .. }
                | Kind::CmpReg { .. }
                | Kind::Push { .. }
                | Kind::Pop { .. }
                | Kind::Bx(_)
                | Kind::SpAdjust(_)
                | Kind::MovHi { .. }
        )
    };
    let mut dropped: Vec<(u32, u32, usize)> = Vec::new();
    for (k, x) in ins.iter().enumerate() {
        if lifter.visited.contains(&k) || lifter.consumed.contains(&k) || structural(&x.kind) {
            continue;
        }
        match dropped.last_mut() {
            Some((_, end, count)) if *end == x.addr => {
                *end = x.addr + x.size;
                *count += 1;
            }
            _ => dropped.push((x.addr, x.addr + x.size, 1)),
        }
    }
    let dropped: Vec<String> = dropped
        .iter()
        .filter(|(_, _, count)| *count >= 2)
        .map(|(start, end, count)| format!("0x{start:08x}..0x{end:08x} ({count})"))
        .collect();
    if !dropped.is_empty() {
        // At the end: the unit's by-value struct detection reads the first
        // lines of the body.
        lines.push(format!("    /* unlifted: {} */", dropped.join(", ")));
    }
    if !main_mode() {
        rewrite_scene_work(&mut lines);
    }
    let mut consts = lifter.consts.clone();
    for slot in &lifter.slots {
        push_unique(&mut consts, slot);
    }
    for entry in &lifter.parked_loads {
        push_unique(&mut consts, entry.split(':').next().unwrap());
    }
    Draft {
        lines,
        params: lifter.params.iter().cloned().collect(),
        param_types: lifter.param_types.clone(),
        consts,
        frames: lifter.frames.clone(),
    }
}

/// The scene work record at 0x03001ebc: its step counter bump becomes the
/// `bump_step` helper and its other fields read through a byte pointer.
fn rewrite_scene_work(lines: &mut Vec<String>) {
    const STEP: &str = "*(u16 *)((*(s32 *)0x03001ebc + 0x1d8))";
    for line in lines.iter_mut() {
        let trimmed = line.trim_start();
        let lead = line.len() - trimmed.len();
        if let Some(rest) = trimmed.strip_prefix(STEP) {
            if let Some(rest) = rest.strip_prefix(" = (") {
                if let Some(rest) = rest.strip_prefix(STEP) {
                    if let Some(amount) =
                        rest.strip_prefix(" + ").and_then(|r| r.strip_suffix(");"))
                    {
                        if amount.bytes().all(|b| b.is_ascii_digit()) {
                            *line = format!("{}bump_step({amount});", &line[..lead]);
                            continue;
                        }
                    }
                }
            }
        }
        if let Some(rest) = trimmed.strip_prefix(STEP) {
            if let Some(amount) = rest.strip_prefix(" += ").and_then(|r| r.strip_suffix(';')) {
                if amount.bytes().all(|b| b.is_ascii_digit()) {
                    *line = format!("{}bump_step({amount});", &line[..lead]);
                    continue;
                }
            }
        }
        *line = line.replace("(*(s32 *)0x03001ebc + 0x", "(*(u8 **)0x03001ebc + 0x");
    }
    if !main_mode() {
        share_scene_work(lines);
    }
}

/// Consecutive statements through the scene work pointer read it once: the
/// reference loads it into one register for the run, and a volatile read
/// cannot be merged by the compiler, so the run names it.
fn share_scene_work(lines: &mut Vec<String>) {
    const WORK: &str = "(*(u8 **)0x03001ebc + 0x";
    let uses_work = |line: &str| {
        line.contains(WORK) && !contains_call(line) && !line.trim_start().starts_with("bump_step")
    };
    let mut at = 0;
    while at < lines.len() {
        if !uses_work(&lines[at]) {
            at += 1;
            continue;
        }
        let lead = lines[at].len() - lines[at].trim_start().len();
        let mut end = at + 1;
        while end < lines.len()
            && uses_work(&lines[end])
            && lines[end].len() - lines[end].trim_start().len() == lead
        {
            end += 1;
        }
        if end - at >= 2 {
            for line in &mut lines[at..end] {
                *line = line.replace(WORK, "(work + 0x");
            }
            lines.insert(
                at,
                format!("{}work = *(u8 **)0x03001ebc;", " ".repeat(lead)),
            );
            end += 1;
        }
        at = end;
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::decode::{decode_window, OVERLAY_BASE};

    fn lifted(halves: &[u16]) -> String {
        let image: Vec<u8> = halves.iter().flat_map(|h| h.to_le_bytes()).collect();
        let ins = decode_window(&image, OVERLAY_BASE, image.len() as u32);
        lift(&ins).lines.join("\n")
    }

    /// `ldr r0, [r1]; ldr r2, [r1, #4]; adds r0, r0, r2; str r0, [r1]; bx lr`
    #[test]
    fn compound_assignment_keeps_inner_parentheses() {
        let text = lifted(&[0x6808, 0x684a, 0x1880, 0x6008, 0x4770]);
        assert!(text.contains("*(s32 *)(a1) += *(s32 *)(a1 + 4);"), "{text}");
    }

    /// `subs r0, #1; bne +0; movs r0, #0; bx lr`: the branch tests the
    /// subtraction's flags, not a compare.
    #[test]
    fn arithmetic_flags_feed_the_branch() {
        let text = lifted(&[0x3801, 0xd100, 0x2000, 0x4770]);
        assert!(text.contains("if ((a0 - 1) == 0) {"), "{text}");
    }

    /// `L: bl X; cmp r0, #0; bne cont; bl Y; b exit; cont: bl Z; adds r5, #1;
    /// cmp r5, #4; bne L; exit: bx lr`: the jump past the loop is `break`,
    /// and the `b exit` is not mistaken for an else block.
    #[test]
    fn jump_past_the_loop_is_break() {
        let text = lifted(&[
            0xf000, 0xf80a, 0x2800, 0xd102, 0xf000, 0xf80a, 0xe004, 0xf000, 0xf80a, 0x3501, 0x2d04,
            0xd1f3, 0x4770,
        ]);
        assert!(text.contains("for (i = 0; i != 4; i++) {"), "{text}");
        assert!(text.contains("break;"), "{text}");
        assert!(!text.contains("goto"), "{text}");
        assert!(!text.contains("else"), "{text}");
    }
}
