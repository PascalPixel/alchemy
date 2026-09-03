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

struct Lifter<'a> {
    ins: &'a [Ins],
    value_sites: BTreeSet<usize>,
    by_addr: HashMap<u32, usize>,
    targets: BTreeSet<u32>,
    spill_slots: BTreeSet<u32>,
    regs: Vec<Option<Val>>,
    out: Vec<String>,
    indent: String,
    pending_name: Option<String>,
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
    cursor_max: usize,
}

fn push_unique(list: &mut Vec<String>, name: &str) {
    if !list.iter().any(|n| n == name) {
        list.push(name.to_string());
    }
}

impl<'a> Lifter<'a> {
    fn new(ins: &'a [Ins]) -> Self {
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
            value_sites: crate::sched::value_calls(ins),
            by_addr,
            targets,
            spill_slots,
            regs: vec![None; 16],
            out: Vec::new(),
            indent: "    ".to_string(),
            pending_name: None,
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
            cursor_max: 0,
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
        self.out.push(format!("{}{}", self.indent, s.as_ref()));
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
        if self.inline_calls.contains(&call) {
            if let Some(&oi) = self.call_stmt.get(&call) {
                if oi == self.out.len() - 1 && assigned_prefix(&self.out[oi]).is_none() {
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
        let lit = if (0x0200_0000..0x0201_0000).contains(&unsigned) && unsigned % 2 == 1 {
            format!("(s32){}", func_name(unsigned))
        } else if unsigned < 0x0200_0000 {
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
            self.maybe_label(i);
            if let Some((from, cond)) = self.back_branch(i) {
                if from < end {
                    i = self.loop_at(i, from, cond, end);
                    continue;
                }
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
                    self.run(i, tail_start);
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
        self.run_filtered(i, tail_start, counter);
        self.indent = saved;
        self.emit("}");
        self.clear_scratch();
        from + 1
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
                    self.emit("none = 0;");
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
                            let name = format!("p{rd}");
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
                let v = self.val_of(rn, Some(rn));
                let a = self.fmt_raw(&v);
                self.last_cmp = Some((a, imm.to_string()));
            }
            Kind::CmpReg { rn, rm } => {
                let v = self.val_of(rn, Some(rn));
                let w = self.val_of(rm, Some(rn));
                let a = self.fmt_raw(&v);
                let b = self.fmt(&w);
                self.last_cmp = Some((a, b));
            }
            Kind::Bl { target } => self.call(i, target),
            Kind::B { target } => return self.branch(i, target, end),
            Kind::Bcond { cond, target } => return self.conditional(i, cond, target),
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
            let e = format!("*({c_type} *)0x{:08x}", (c as u32).wrapping_add(off));
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

    fn park_load(&mut self, rd: u8, e: &str, c_type: &str) -> bool {
        if !(4..=11).contains(&rd) {
            return false;
        }
        let name = format!("p{rd}");
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
            Some(c) => format!("*({c_type} *)0x{:08x}", (c as u32).wrapping_add(off)),
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
                if op == "|" {
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
            let amount = rest.trim_end_matches(')');
            self.emit(format!("{lhs} += {amount};"));
            return;
        }
        if width == Width::Half && v.is_const() && !v.pool {
            // A halfword store of a literal is a HImode constant, which this
            // compiler loads from the pool; through an int local it is SImode
            // and a `movs`.
            let (record, target) = match hoisted_call(&lhs) {
                Some((call, rest)) => (Some(call), rest),
                None => (None, lhs.clone()),
            };
            self.emit("{");
            if let Some(call) = record {
                self.emit(format!("    u8 *record = {call};"));
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
        let wrap = (costly || count >= 5) && count > 0 && count != 5;
        let callee = func_name(target);
        let call = if self.value_sites.contains(&i) && count != 5 {
            if count == 0 {
                format!("Value0({callee})")
            } else {
                format!("Value{count}({callee}, {})", args.join(", "))
            }
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
            let mut text = text.trim().trim_end_matches(';').to_string();
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
            self.emit(format!("goto {};", label_name(target)));
            self.goto_targets.insert(target);
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
                self.run(l, t);
                self.indent = saved;
                self.emit("}");
                self.clear_scratch();
                return from + 1;
            }
            return t;
        }
        self.emit(format!("goto {};", label_name(target)));
        self.goto_targets.insert(target);
        i + 1
    }

    /// Formats a value without resolving a call result, so the caller can
    /// decide whether the call statement folds into a condition.
    fn fmt_raw(&mut self, v: &Val) -> String {
        if v.call_index().is_some() {
            return v.e.clone().unwrap();
        }
        self.fmt(v)
    }

    fn conditional(&mut self, i: usize, cond: Cond, target: u32) -> usize {
        let t = self.index_of(target);
        let (a, b) = self
            .last_cmp
            .clone()
            .unwrap_or_else(|| ("?".to_string(), "?".to_string()));
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
            let else_end = match last.kind {
                Kind::B { target } => self.index_of(target).filter(|e| *e > t),
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
        self.emit(format!("    goto {};", label_name(target)));
        self.emit("}");
        self.goto_targets.insert(target);
        i + 1
    }
}

/// Whether `movs` plus at most one `lsls` builds the word.
fn cheap(word: u32) -> bool {
    (0..32).any(|shift| (word >> shift) < 256 && (word >> shift) << shift == word)
}

fn is_plain_local(e: &str) -> bool {
    if e == "record" || e == "i" {
        return true;
    }
    if let Some(rest) = e.strip_prefix("rec") {
        return rest.bytes().all(|b| b.is_ascii_digit());
    }
    if let Some(rest) = e.strip_prefix('p') {
        return !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit());
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
    rewrite_scene_work(&mut lines);
    let mut consts = lifter.consts.clone();
    consts.extend(lifter.slots.iter().cloned());
    consts.extend(
        lifter
            .parked_loads
            .iter()
            .map(|entry| entry.split(':').next().unwrap().to_string()),
    );
    Draft {
        lines,
        params: lifter.params.iter().cloned().collect(),
        consts,
    }
}

/// The scene work record at 0x03001ebc: its step counter bump becomes the
/// `bump_step` helper and its other fields read through a byte pointer.
fn rewrite_scene_work(lines: &mut [String]) {
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
}
