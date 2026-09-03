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
    pub consts: Vec<String>,
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
        Cond::Le | Cond::Ls => "<=",
        Cond::Hi => ">",
        Cond::Cc => "<",
        Cond::Cs => ">=",
        other => return format!("{a} /* {} */ {b}", other.mnemonic()),
    };
    format!("{a} {op} {b}")
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

fn format_const(c: i32) -> String {
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
    last_cmp: Option<(String, String)>,
    /// The register the last `cmp rN, #imm` compared.
    last_cmp_reg: Option<u8>,
    /// Instructions an idiom already consumed; the run skips them.
    consumed: BTreeSet<usize>,
    cursor_max: usize,
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
        for x in ins {
            match x.kind {
                Kind::B { target } | Kind::Bcond { target, .. } => {
                    targets.insert(target);
                }
                Kind::LdrSp { imm, .. } | Kind::AddSp { imm, .. } => {
                    spill_slots.insert(imm);
                }
                _ => {}
            }
        }
        let mut lifter = Lifter {
            ins,
            value_sites: sites.value,
            direct_sites: sites.direct,
            by_addr,
            targets,
            spill_slots,
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
            last_cmp: None,
            last_cmp_reg: None,
            consumed: BTreeSet::new(),
            cursor_max: 0,
            loops: Vec::new(),
        };
        lifter.reset();
        lifter
    }

    fn reset(&mut self) {
        self.params.clear();
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

    fn emit(&mut self, s: impl AsRef<str>) {
        let line = s.as_ref();
        if let Some(deferred) = self.deferred.take() {
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
            Some(k) => self.out.insert(k, line),
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
        let lit = if (0x0200_0000..0x0201_0000).contains(&unsigned) && unsigned % 2 == 1 {
            format!("(s32){}", func_name(unsigned))
        } else if unsigned < 0x0201_0000 {
            format!("(s32)Data_{unsigned:08x}")
        } else {
            format_const(c)
        };
        let line = format!("{}{name} = {lit};", self.indent);
        match v.def_out {
            Some(at) if at <= self.out.len() => self.out.insert(at, line),
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
        if (5..=7).contains(&r) && v.is_const() && !v.shared {
            v.shared = true;
            v.def_out = Some(self.out.len());
            v.reg = Some(r);
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

    fn r0_read_after(&self, i: usize) -> bool {
        for x in &self.ins[i + 1..] {
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
            self.emit(format!("{}:;", label_name(addr)));
            self.emitted_labels.insert(addr);
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
                i = self.emit_switch(tree);
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
            let tail: Vec<&str> = self.ins[tail_start..=from]
                .iter()
                .map(|z| z.text.as_str())
                .collect();
            format!("for (i = 0; ; ) {{ /* tail: {} */", tail.join(" ; "))
        };
        self.emit(head);
        let saved = self.indent.clone();
        self.indent.push_str("    ");
        self.regs[counter as usize] = Some(Val::expr("i"));
        self.loops.push((tail_start, from));
        self.run_filtered(i, tail_start, counter);
        self.loops.pop();
        self.indent = saved;
        self.emit("}");
        self.clear_scratch();
        from + 1
    }

    /// A `switch` the compiler lowered to a comparison tree: compares of one
    /// register against constants, `beq` to case bodies, and one shared exit
    /// every body branches to. Bodies follow the tree in source order.
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

    fn emit_switch(&mut self, tree: SwitchTree) -> usize {
        let value = self.val_of(tree.subject, None);
        let subject = self.fmt(&value);
        let subject = if tree.unsigned {
            format!("(u32){subject}")
        } else {
            subject
        };
        self.emit(format!("switch ({subject}) {{"));
        let saved = self.indent.clone();
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
            self.run(body, stop);
            self.cursor_max = mark;
            self.emit("break;");
            self.clear_scratch();
        }
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
            // The closing jump to the exit is the `break`.
            let mut stop = body_end;
            if stop > *body {
                if let Kind::B { target } = self.ins[stop - 1].kind {
                    if self.index_of(target) == Some(tree.exit) {
                        stop -= 1;
                    }
                }
            }
            self.cursor_max = self.cursor_max.max(*body);
            self.run(*body, stop);
            self.emit("break;");
            self.clear_scratch();
        }
        self.indent = saved;
        self.emit("}");
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
            (Some(a), Some(b)) => Val::constant(fold(a, b)),
            _ => {
                let a = self.arith(&v);
                let b = self.arith(&w);
                Val::expr(format!("({a} {op} {b})"))
            }
        };
        self.set_reg(rd, value);
    }

    fn step(&mut self, i: usize, end: usize) -> usize {
        self.cursor_max = self.cursor_max.max(i + 1);
        self.maybe_label(i);
        let x = &self.ins[i];
        let addr = x.addr;
        let flags = flag_register(&x.kind);
        match x.kind.clone() {
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
            Kind::MovImm { rd, imm } => self.set_reg(rd, Val::constant(imm as i32)),
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
                self.value_name_for_shift(rm);
                let v = self.val_of(rm, Some(rd));
                let n = imm;
                let value = match (shift, v.c) {
                    (Shift::Lsl, Some(c)) => Val::constant(c.wrapping_shl(n)),
                    (Shift::Lsr, Some(c)) => Val::constant(((c as u32) >> n) as i32),
                    (Shift::Asr, Some(c)) => Val::constant(c >> n),
                    (Shift::Lsl, None) => Val::expr(format!("({} << {n})", self.arith(&v))),
                    (_, None) => Val::expr(format!("({} >> {n})", self.arith(&v))),
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
                Alu::Lsr | Alu::Asr => self.binary_expr(rd, rm, ">>"),
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
            Kind::Ldmia { .. } | Kind::Stmia { .. } => self.emit(format!("/* {} */", x.text)),
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
            let off = index.c.map(i64::from).unwrap_or(0);
            let e = self.addr_expr(&base, off, width);
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
            let e = match self.shared_base(&base) {
                Some(name) => deref_named(c_type, &name, off),
                None => format!("*({c_type} *)0x{:08x}", (c as u32).wrapping_add(off)),
            };
            if base.shared && self.park_load(rd, &e, c_type) {
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
            let b = self.ensure_result_var(&mut base);
            let ix = self.fmt(&index);
            let value = self.fmt(&v);
            self.emit(format!("*({c_type} *)({b} + {ix}) = {value};"));
            return;
        };
        let base = self.val_of(rn, None);
        let lhs = match base.c {
            Some(c) => match self.shared_base(&base) {
                Some(name) => deref_named(c_type, &name, off),
                None => format!("*({c_type} *)0x{:08x}", (c as u32).wrapping_add(off)),
            },
            None => self.addr_expr(&base, i64::from(off), width),
        };
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
                    None => (None, lhs.clone()),
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
                let mark = self.out.len();
                self.run(t, from);
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
                self.loops.push((t, from));
                self.run(l, t);
                self.loops.pop();
                self.indent = saved;
                self.emit("}");
                self.clear_scratch();
                return from + 1;
            }
            return t;
        }
        self.jump(target);
        i + 1
    }

    /// `continue` into the enclosing loop's tail, `break` past it, else a goto.
    fn jump(&mut self, target: u32) {
        let index = self.index_of(target);
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
        if let Some(t) = t.filter(|t| *t > i) {
            let a = if a.starts_with("@call:") {
                let guard = !self.r0_read_after(i);
                self.pop_call_condition(&a, guard)
            } else {
                a
            };
            let text = cond_text(negate(cond), &a, &b);
            self.emit(format!("if ({text}) {{"));
            let saved = self.indent.clone();
            self.indent.push_str("    ");
            let last = &self.ins[t - 1];
            // An else block ends inside the current region; a jump beyond it
            // is a break, continue, or goto handled when the body is lifted.
            let else_end = match last.kind {
                Kind::B { target } => self.index_of(target).filter(|e| *e > t && *e <= end),
                _ => None,
            };
            self.run(i + 1, if else_end.is_some() { t - 1 } else { t });
            if let Some(else_end) = else_end {
                self.indent = saved.clone();
                self.emit("} else {");
                self.indent.push_str("    ");
                self.run(t, else_end);
                self.indent = saved;
                self.emit("}");
                return else_end;
            }
            self.indent = saved;
            self.emit("}");
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
    rewrite_scene_work(&mut lines);
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
        consts,
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
    share_scene_work(lines);
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
