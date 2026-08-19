//! Draft C for one owner from its own disassembly.
//!
//! WHAT THIS IS FOR. A call-dense owner -- a scene or cutscene driver -- is
//! hundreds of calls with literal arguments and very little else. Transcribing
//! that by hand is a day of typing during which the interesting question, which
//! is what the source SAID, never gets asked. This writes the mechanical part so
//! the reader can spend the time on the rest.
//!
//! WHAT IT IS NOT FOR. It is not a decompiler and its output is not a
//! reconstruction. It emits what the bytes plainly show -- a call, its
//! arguments, a loop's bounds, a field written through a pointer -- and it is
//! silent about everything else. A draft that scores badly is a draft, and the
//! adoption gate refuses it exactly as it refuses anything else that does not
//! reproduce. Output goes to `work/`, which is gitignored, because a generator
//! pointed at every owner is precisely the machine that produced 862,856 bytes
//! of C nobody could use.
//!
//! WHAT IT MODELS, all of it read off the instruction stream:
//!
//!   * calls, with the arguments freshly loaded into r0-r3 and the stack slots
//!   * literal pools, located from the pc-relative targets objdump resolves,
//!     and skipped rather than decoded as instructions
//!   * a value loaded once into a callee-saved register and used as `base + N`,
//!     which is a local and not a literal
//!   * loads and stores through a held pointer, including register-offset
//!     addressing and the read-modify-write shape
//!   * counted loops, from the backward branch and the comparison before it
//!
//! Registers r4-r7 survive a call and r0-r3 do not, and the simulation follows
//! that. Getting it wrong drops every argument that was already in a register.

use std::collections::{BTreeMap, BTreeSet};

/// One decoded listing row.
struct Row {
    addr: i64,
    text: String,
    lo: u16,
    hi: Option<u16>,
}

/// A value the simulation is tracking: a number, or a named local.
#[derive(Clone, Debug, PartialEq)]
enum Val {
    Num(i64),
    Named(String),
}

impl Val {
    fn render(&self) -> String {
        match self {
            Val::Num(n) => n.to_string(),
            Val::Named(s) => s.clone(),
        }
    }
}

/// Drop a `}` that would close the function, and close any block still open
/// at the end. A large owner with mixed loops and ifs otherwise emits
/// `if (} > 0)`-adjacent imbalance that is not C.
fn balance_braces(lines: Vec<String>) -> Vec<String> {
    let mut depth = 0i32;
    let mut kept = Vec::with_capacity(lines.len());
    for line in lines {
        let opens = line.matches('{').count() as i32;
        let closes = line.matches('}').count() as i32;
        if depth - closes + opens < 0 {
            continue;
        }
        depth = depth - closes + opens;
        kept.push(line);
    }
    while depth > 0 {
        kept.push("}".to_string());
        depth -= 1;
    }
    kept
}

fn is_call_stmt(line: &str) -> bool {
    let t = line.trim();
    let t = t.strip_suffix(';').unwrap_or(t);
    let Some(name) = t.split('(').next() else { return false };
    name.starts_with("Func_") && t.ends_with(')') && !t.contains('{') && !t.contains('}')
}

fn hex(s: &str) -> Option<i64> {
    i64::from_str_radix(s.trim_start_matches("0x"), 16).ok()
}

/// `` 2003054:\tb520      \tpush\t{r5, lr}`` -> address, halfwords, text.
fn parse_row(line: &str) -> Option<Row> {
    let (addr_part, rest) = line.split_once(':')?;
    let addr = hex(addr_part.trim())?;
    let mut fields = rest.split('\t').filter(|f| !f.trim().is_empty());
    let bytes = fields.next()?.trim();
    let mut words = bytes.split_whitespace();
    let lo = u16::from_str_radix(words.next()?, 16).ok()?;
    let hi = words.next().and_then(|w| u16::from_str_radix(w, 16).ok());
    let text: Vec<&str> = fields.collect();
    let text = text.join("\t");
    let text = text.split('@').next().unwrap_or("").trim().to_string();
    if text.is_empty() {
        return None;
    }
    Some(Row { addr, text, lo, hi })
}

/// Operands after the mnemonic, split on commas.
fn operands(text: &str) -> (String, Vec<String>) {
    let mut parts = text.splitn(2, |c: char| c.is_whitespace());
    let op = parts.next().unwrap_or("").to_string();
    let rest = parts.next().unwrap_or("").trim();
    // Split on commas OUTSIDE brackets. `ldr r0, [pc, #4]` is two operands,
    // not three, and splitting naively loses every addressing mode in the file.
    let mut args: Vec<String> = Vec::new();
    let mut cur = String::new();
    let mut depth = 0i32;
    for c in rest.chars() {
        match c {
            '[' | '{' => { depth += 1; cur.push(c); }
            ']' | '}' => { depth -= 1; cur.push(c); }
            ',' if depth == 0 => {
                if !cur.trim().is_empty() { args.push(cur.trim().to_string()); }
                cur.clear();
            }
            _ => cur.push(c),
        }
    }
    if !cur.trim().is_empty() { args.push(cur.trim().to_string()); }
    (op, args)
}

fn immediate(token: &str) -> Option<i64> {
    let t = token.strip_prefix('#')?;
    if let Some(h) = t.strip_prefix("0x") {
        i64::from_str_radix(h, 16).ok()
    } else {
        t.parse().ok()
    }
}

/// A pool word: two halfwords, little end first.
fn word_at(rows: &BTreeMap<i64, u16>, addr: i64) -> Option<i64> {
    let lo = *rows.get(&addr)? as i64;
    let hi = *rows.get(&(addr + 2))? as i64;
    Some((hi << 16) | lo)
}

pub struct Draft {
    pub lines: Vec<String>,
    pub calls: usize,
    pub loops: usize,
    pub memory: usize,
}

/// Reconstruct a draft from an objdump listing.
pub fn draft(listing: &[String], func: &str) -> Draft {
    let rows: Vec<Row> = listing.iter().filter_map(|l| parse_row(l)).collect();

    // Halfword map, for reading pool words.
    let mut halfwords: BTreeMap<i64, u16> = BTreeMap::new();
    for r in &rows {
        halfwords.insert(r.addr, r.lo);
        if let Some(h) = r.hi {
            halfwords.insert(r.addr + 2, h);
        }
    }

    // Pools, from the targets objdump already resolved in its `@ (0x...)`
    // comments. A byte-pattern scan misses a pool word whose high half is not
    // zero, and then decodes four bytes of data as two instructions.
    let mut pool_words: BTreeSet<i64> = BTreeSet::new();
    for line in listing {
        if let Some(at) = line.find("@ (0x") {
            let tail = &line[at + 5..];
            if let Some(end) = tail.find(')') {
                if let Some(a) = hex(tail[..end].trim()) {
                    pool_words.insert(a);
                    pool_words.insert(a + 2);
                }
            }
        }
    }

    // An unconditional forward branch skips a region nothing can fall into. If
    // no other branch targets an address inside it, that region is data the
    // compiler placed mid-function, whatever objdump's `@ (0x...)` comments
    // happened to reference -- a pool word nothing loads is still a pool word.
    let mut branch_targets: BTreeSet<i64> = BTreeSet::new();
    for r in &rows {
        let (op, args) = operands(&r.text);
        if op.starts_with('b') && op != "bx" && args.len() == 1 {
            if let Some(x) = hex(&args[0]) {
                branch_targets.insert(x);
            }
        }
    }
    let mut pool_skip: BTreeSet<i64> = BTreeSet::new();
    for r in &rows {
        let (op, args) = operands(&r.text);
        if (op == "b" || op == "b.n") && args.len() == 1 {
            if let Some(target) = hex(&args[0]) {
                if target > r.addr
                    && !rows
                        .iter()
                        .any(|x| x.addr > r.addr && x.addr < target && branch_targets.contains(&x.addr))
                {
                    pool_skip.insert(r.addr);
                    for x in rows.iter().filter(|x| x.addr > r.addr && x.addr < target) {
                        pool_words.insert(x.addr);
                    }
                }
            }
        }
    }

    // A backward branch is a loop. Its bound comes from the comparison just
    // before it and its start from the last `movs` to that register above the
    // top; assuming a start of zero turns a countdown into a single pass.
    let mut loop_top: BTreeMap<i64, (i64, i64, String)> = BTreeMap::new();
    let mut loop_end: BTreeSet<i64> = BTreeSet::new();
    for (i, r) in rows.iter().enumerate() {
        let (op, args) = operands(&r.text);
        if !op.starts_with('b') || op == "bx" || op == "bl" || args.len() != 1 {
            continue;
        }
        let Some(target) = hex(&args[0]) else { continue };
        if target >= r.addr {
            continue;
        }
        let mut counter = String::new();
        let mut bound = 0;
        for back in rows[..i].iter().rev().take(6) {
            let (cop, cargs) = operands(&back.text);
            if cop == "cmp" && cargs.len() == 2 {
                if let Some(v) = immediate(&cargs[1]) {
                    counter = cargs[0].clone();
                    bound = v;
                }
                break;
            }
        }
        let mut start = 0;
        if !counter.is_empty() {
            for before in rows.iter().take_while(|x| x.addr < target) {
                let (mop, margs) = operands(&before.text);
                if mop == "movs" && margs.len() == 2 && margs[0] == counter {
                    if let Some(v) = immediate(&margs[1]) {
                        start = v;
                    }
                }
            }
        }
        loop_top.insert(target, (start, bound, op.clone()));
        loop_end.insert(r.addr);
    }

    // A forward CONDITIONAL branch is an `if`. The comparison before it gives
    // the test; the branch is taken when the body should be SKIPPED, so the C
    // condition is its inverse. If the instruction just before the branch
    // target is an unconditional forward branch, that is the `else` arm's jump
    // over the join and the shape is if/else.
    //
    // Only the call whose result is tested is expressed. `cmp r0, #0` after a
    // call is `if (f(...))`; anything else keeps its registers and is left for
    // the reader, because guessing a condition is worse than leaving a hole.
    let mut if_at: BTreeMap<i64, (String, i64, Option<i64>)> = BTreeMap::new();
    let mut close_at: BTreeMap<i64, usize> = BTreeMap::new();
    let mut else_at: BTreeMap<i64, i64> = BTreeMap::new();
    // second and later tests of a short-circuit chain -> the `cmp` that owns it
    let mut and_join: BTreeMap<i64, String> = BTreeMap::new();
    for (i, r) in rows.iter().enumerate() {
        let (op, args) = operands(&r.text);
        let cond = matches!(
            op.as_str(),
            "beq" | "beq.n" | "bne" | "bne.n" | "blt" | "blt.n" | "bgt" | "bgt.n"
                | "bge" | "bge.n" | "ble" | "ble.n"
        );
        if !cond || args.len() != 1 {
            continue;
        }
        let Some(target) = hex(&args[0]) else { continue };
        if target <= r.addr || pool_words.contains(&r.addr) {
            continue;
        }
        // The comparison immediately before, and the call before that.
        let Some(prev) = rows.get(i.wrapping_sub(1)) else { continue };
        let (pop_, pargs) = operands(&prev.text);
        if pop_ != "cmp" || pargs.len() != 2 || pargs[0] != "r0" || immediate(&pargs[1]) != Some(0) {
            continue;
        }
        // taken == skip the body, so invert.
        let test = match op.trim_end_matches(".n") {
            "beq" => "!= 0",
            "bne" => "== 0",
            "blt" => ">= 0",
            "bge" => "< 0",
            "bgt" => "<= 0",
            "ble" => "> 0",
            _ => continue,
        };
        // if/else when the body ends in an unconditional jump past the target
        let mut join = None;
        if let Some(last) = rows.iter().filter(|x| x.addr < target).last() {
            let (lop, largs) = operands(&last.text);
            if (lop == "b" || lop == "b.n") && largs.len() == 1 && !pool_skip.contains(&last.addr) {
                if let Some(j) = hex(&largs[0]) {
                    if j > target {
                        join = Some(j);
                        else_at.insert(target, j);
                    }
                }
            }
        }
        // Consecutive tests that skip to the SAME address are one short-circuit
        // condition, not two ifs: `if (f() == 0 && g() == 0)`. Emitting them
        // separately gives the first branch the wrong target and nests a block
        // that never closes where the reference has none.
        if let Some(prior) = if_at
            .iter()
            .find(|(_, v)| v.1 == target && v.2.is_none())
            .map(|(k, _)| *k)
        {
            if join.is_some() {
                let existing = if_at.remove(&prior).unwrap();
                if_at.insert(prior, (existing.0, target, join));
                if let Some(n) = close_at.get_mut(&target) {
                    if *n > 0 { *n -= 1; }
                }
                *close_at.entry(join.unwrap_or(target)).or_insert(0) += 1;
            }
            and_join.insert(prev.addr, test.to_string());
            continue;
        }
        if_at.insert(prev.addr, (test.to_string(), target, join));
        *close_at.entry(join.unwrap_or(target)).or_insert(0) += 1;
    }

    let mut reg: BTreeMap<String, Val> = BTreeMap::new();
    let mut fresh: BTreeMap<String, Val> = BTreeMap::new();
    let mut ptr: BTreeMap<String, String> = BTreeMap::new();
    let mut loaded: BTreeMap<String, String> = BTreeMap::new();
    let mut pending: BTreeMap<String, (String, String, i64)> = BTreeMap::new();
    let mut stack: BTreeMap<i64, Val> = BTreeMap::new();

    let mut out: Vec<String> = Vec::new();
    let mut calls = 0usize;
    let mut memory = 0usize;
    let mut depth = 1usize;
    let mut loop_n = 0usize;
    let mut names: BTreeSet<String> = BTreeSet::new();
    // callees whose result is tested cannot be void
    let mut tested: BTreeSet<String> = BTreeSet::new();
    // A callee whose result is held as a pointer cannot be declared void.
    let mut returns_pointer: BTreeSet<String> = BTreeSet::new();
    let mut tmp_n = 0usize;
    // temp name -> the line that produced it, so a temp that is actually used
    // can be promoted to `pN = f(...)` and declared. A temp nothing reads stays
    // a plain call.
    let mut temp_line: BTreeMap<String, usize> = BTreeMap::new();
    let mut temp_fn: BTreeMap<String, String> = BTreeMap::new();

    let indent = |d: usize| "    ".repeat(d);

    // A pool word loaded once into a callee-saved register and then used as
    // `base + N` is a local. Collect the candidates first.
    let mut base_values: BTreeSet<i64> = BTreeSet::new();
    for r in &rows {
        let (op, args) = operands(&r.text);
        if op == "ldr" && args.len() == 2 && args[0].starts_with("r") {
            let n: u32 = args[0][1..].parse().unwrap_or(0);
            if (4..=7).contains(&n) && args[1].starts_with("[pc") {
                if let Some(off) = args.get(1).and_then(|a| {
                    a.split('#').nth(1).and_then(|s| s.trim_end_matches(']').parse::<i64>().ok())
                }) {
                    if let Some(v) = word_at(&halfwords, ((r.addr + 4) & !3) + off) {
                        if v > 0 && v < 0x10000 {
                            base_values.insert(v);
                        }
                    }
                }
            }
        }
    }

    let deref = |base: &str, off: i64, width: &str| -> String {
        let cast = match width {
            "ldrb" | "strb" => "u8",
            "ldrh" | "strh" => "u16",
            "ldrsh" => "s16",
            "ldrsb" => "s8",
            _ => "s32",
        };
        let inner = if base.contains(' ') { format!("({base})") } else { base.to_string() };
        if off == 0 {
            format!("*({cast} *){inner}")
        } else {
            format!("*({cast} *)({inner} + {off})")
        }
    };

    for r in &rows {
        if pool_words.contains(&r.addr) {
            continue;
        }
        if let Some((start, bound, kind)) = loop_top.get(&r.addr) {
            loop_n += 1;
            // `bne` is `!=`, not `<`, and the difference is visible in the
            // bytes. A counted loop written `i < 4` whose counter is dead in the
            // body gets reversed by gcc into a countdown to zero, because
            // comparing against zero is free; written `i != 4` it cannot be
            // reversed and the reference's `adds`/`cmp #4`/`bne` survives. On
            // resource_37a:1380 that one spelling took 64 wrong instructions to
            // 24 across five loops.
            let cmp = if kind.starts_with("ble") {
                "<="
            } else if kind.starts_with("bge") {
                ">="
            } else if kind.starts_with("bne") {
                "!="
            } else {
                "<"
            };
            let step = if cmp == ">=" { "--" } else { "++" };
            out.push(format!(
                "{}for (i{loop_n} = {start}; i{loop_n} {cmp} {bound}; i{loop_n}{step}) {{",
                indent(depth)
            ));
            depth += 1;
        }
        // Close any block whose join is here, then open the else arm.
        while let Some(n) = close_at.get_mut(&r.addr) {
            if *n == 0 { break }
            *n -= 1;
            depth = depth.saturating_sub(1).max(1);
            out.push(format!("{}}}", indent(depth)));
            if *n == 0 { close_at.remove(&r.addr); break }
        }
        if else_at.contains_key(&r.addr) {
            depth = depth.saturating_sub(1).max(1);
            out.push(format!("{}}} else {{", indent(depth)));
            depth += 1;
        }
        if loop_end.contains(&r.addr) {
            depth = depth.saturating_sub(1).max(1);
            out.push(format!("{}}}", indent(depth)));
            continue;
        }

        let (op, args) = operands(&r.text);

        if let Some(test) = and_join.get(&r.addr).cloned() {
            // The call for this arm was just emitted; fold it into the open
            // condition instead of testing it separately.
            if out.last().is_some_and(|l| is_call_stmt(l)) {
                if let Some(prev) = out.pop() {
                    let call = prev.trim().trim_end_matches(';').to_string();
                    if let Some(name) = call.split('(').next() {
                        if name.starts_with("Func_") { tested.insert(name.to_string()); }
                    }
                    if let Some(open) = out.iter_mut().rev().find(|l| l.trim_start().starts_with("if (")) {
                        let closed = open.trim_end().trim_end_matches('{').trim_end().to_string();
                        let inner = closed.trim_start().trim_start_matches("if (").trim_end_matches(')');
                        let lead: String = open.chars().take_while(|c| c.is_whitespace()).collect();
                        *open = format!("{lead}if ({inner} && {call} {test}) {{");
                    }
                }
            }
            continue;
        }
        if let Some((test, _target, _join)) = if_at.get(&r.addr).cloned() {
            // The previous statement is the tested call. Fold it into the `if`
            // rather than emitting it and then testing nothing. A close-brace
            // or a loop head is not a call -- folding those produced
            // `if (} > 0)` on large main-image owners.
            if out.last().is_some_and(|l| is_call_stmt(l)) {
                if let Some(prev) = out.pop() {
                    let call = prev.trim().trim_end_matches(';').to_string();
                    if let Some(name) = call.split('(').next() {
                        if name.starts_with("Func_") {
                            tested.insert(name.to_string());
                        }
                    }
                    out.push(format!("{}if ({call} {test}) {{", indent(depth)));
                    depth += 1;
                    continue;
                }
            }
        }

        // A load or store through a held pointer.
        if let Some(rest) = args.get(1) {
            if rest.starts_with('[') && args.len() == 2 {
                let inner = rest.trim_start_matches('[').trim_end_matches(']');
                let mut bits = inner.split(',').map(|s| s.trim());
                let base = bits.next().unwrap_or("");
                let off_tok = bits.next().unwrap_or("#0");
                let off = immediate(off_tok).or_else(|| match reg.get(off_tok) {
                    Some(Val::Num(n)) => Some(*n),
                    _ => None,
                });
                if let (Some(pbase), Some(off)) = (ptr.get(base).cloned(), off) {
                    let place = deref(&pbase, off, &op);
                    if op.starts_with("ldr") {
                        loaded.insert(args[0].clone(), place.clone());
                        reg.remove(&args[0]);
                        fresh.remove(&args[0]);
                        if op == "ldr" {
                            ptr.insert(args[0].clone(), format!("*(void **)({pbase} + {off})"));
                        }
                        memory += 1;
                        continue;
                    }
                    if op.starts_with("str") {
                        memory += 1;
                        if let Some((lv, sym, mask)) = pending.remove(&args[0]) {
                            if lv == place {
                                out.push(format!("{}{place} {sym}= {mask:#x};", indent(depth)));
                                continue;
                            }
                        }
                        let v = loaded
                            .get(&args[0])
                            .cloned()
                            .or_else(|| reg.get(&args[0]).map(|v| v.render()))
                            .unwrap_or_else(|| "0".to_string());
                        out.push(format!("{}{place} = {v};", indent(depth)));
                        continue;
                    }
                }
            }
        }

        match op.as_str() {
            "movs" if args.len() == 2 => {
                let v = immediate(&args[1]).map(Val::Num).or_else(|| reg.get(&args[1]).cloned());
                match v {
                    Some(v) => {
                        reg.insert(args[0].clone(), v.clone());
                        fresh.insert(args[0].clone(), v);
                    }
                    None => {
                        reg.remove(&args[0]);
                        fresh.remove(&args[0]);
                    }
                }
                ptr.remove(&args[0]);
            }
            "lsls" if args.len() >= 2 => {
                let (src, shift) = if args.len() == 3 {
                    (args[1].clone(), immediate(&args[2]))
                } else {
                    (args[0].clone(), immediate(&args[1]))
                };
                if let (Some(Val::Num(n)), Some(s)) = (reg.get(&src).cloned(), shift) {
                    reg.insert(args[0].clone(), Val::Num(n << s));
                    fresh.insert(args[0].clone(), Val::Num(n << s));
                } else {
                    reg.remove(&args[0]);
                    fresh.remove(&args[0]);
                }
            }
            "adds" => {
                // `adds rN, rP, #0` copies a pointer; `adds rP, #K` offsets one.
                if args.len() == 2 && ptr.contains_key(&args[0]) {
                    if let Some(k) = immediate(&args[1]) {
                        let base = ptr[&args[0]].clone();
                        ptr.insert(args[0].clone(), format!("(u8 *)({base}) + {k}"));
                        continue;
                    }
                }
                if args.len() == 3 && ptr.contains_key(&args[1]) && immediate(&args[2]) == Some(0) {
                    let base = ptr[&args[1]].clone();
                    ptr.insert(args[0].clone(), base);
                    reg.remove(&args[0]);
                    continue;
                }
                let (a, b) = if args.len() == 3 {
                    (reg.get(&args[1]).cloned(), immediate(&args[2]).map(Val::Num).or_else(|| reg.get(&args[2]).cloned()))
                } else {
                    (reg.get(&args[0]).cloned(), immediate(&args[1]).map(Val::Num))
                };
                let sum = match (a, b) {
                    (Some(Val::Num(x)), Some(Val::Num(y))) => Some(Val::Num(x + y)),
                    (Some(Val::Named(n)), Some(Val::Num(y))) => Some(if y == 0 {
                        Val::Named(n)
                    } else {
                        Val::Named(format!("{n} + {y}"))
                    }),
                    _ => None,
                };
                match sum {
                    Some(v) => {
                        reg.insert(args[0].clone(), v.clone());
                        fresh.insert(args[0].clone(), v);
                    }
                    None => {
                        reg.remove(&args[0]);
                        fresh.remove(&args[0]);
                    }
                }
            }
            "negs" if args.len() == 2 => {
                if let Some(Val::Num(n)) = reg.get(&args[1]).cloned() {
                    reg.insert(args[0].clone(), Val::Num(-n));
                    fresh.insert(args[0].clone(), Val::Num(-n));
                } else {
                    reg.remove(&args[0]);
                    fresh.remove(&args[0]);
                }
            }
            "ands" | "orrs" | "bics" | "eors" if args.len() == 2 => {
                let sym = match op.as_str() {
                    "ands" => "&",
                    "orrs" => "|",
                    "bics" => "&~",
                    _ => "^",
                };
                let place = loaded.get(&args[1]).cloned().or_else(|| loaded.get(&args[0]).cloned());
                let mask = if loaded.contains_key(&args[1]) {
                    reg.get(&args[0]).cloned()
                } else {
                    reg.get(&args[1]).cloned()
                };
                if let (Some(place), Some(Val::Num(m))) = (place, mask) {
                    pending.insert(args[0].clone(), (place, sym.to_string(), m));
                }
                reg.remove(&args[0]);
                fresh.remove(&args[0]);
            }
            "ldr" if args.len() == 2 && args[1].starts_with("[pc") => {
                let off = args[1]
                    .split('#')
                    .nth(1)
                    .and_then(|s| s.trim_end_matches(']').parse::<i64>().ok())
                    .unwrap_or(0);
                match word_at(&halfwords, ((r.addr + 4) & !3) + off) {
                    Some(v) if base_values.contains(&v) => {
                        reg.insert(args[0].clone(), Val::Named("base".into()));
                        fresh.insert(args[0].clone(), Val::Named("base".into()));
                        out.push(format!("{}base = {v};", indent(depth)));
                    }
                    Some(v) => {
                        reg.insert(args[0].clone(), Val::Num(v));
                        fresh.insert(args[0].clone(), Val::Num(v));
                    }
                    None => {
                        reg.remove(&args[0]);
                        fresh.remove(&args[0]);
                    }
                }
            }
            "str" if args.len() == 2 && args[1].starts_with("[sp") => {
                let off = args[1]
                    .split('#')
                    .nth(1)
                    .and_then(|s| s.trim_end_matches(']').parse::<i64>().ok())
                    .unwrap_or(0);
                if let Some(v) = reg.get(&args[0]).cloned() {
                    stack.insert(off, v);
                }
            }
            _ if op.starts_with("bl") && args.len() == 1 => {
                let Some(target) = hex(&args[0]) else { continue };
                let name = format!("Func_{target:08x}");
                names.insert(name.clone());
                let mut got: Vec<String> = Vec::new();
                for i in 0..4 {
                    match fresh.get(&format!("r{i}")) {
                        Some(v) => got.push(v.render()),
                        None => break,
                    }
                }
                for (_, v) in stack.iter() {
                    got.push(v.render());
                }
                tmp_n += 1;
                temp_line.insert(format!("p{tmp_n}"), out.len());
                temp_fn.insert(format!("p{tmp_n}"), name.clone());
                out.push(format!("{}{name}({});", indent(depth), got.join(", ")));
                calls += 1;
                // r0 dies (it carries the return); r1-r3 keep what the caller
                // put there, and r4-r7 survive entirely.
                for i in 0..4 {
                    let k = format!("r{i}");
                    if i == 0 {
                        reg.remove(&k);
                        ptr.remove(&k);
                    }
                    fresh.remove(&k);
                }
                stack.clear();
                loaded.clear();
                pending.clear();
                ptr.insert("r0".to_string(), format!("p{tmp_n}"));
            }
            // A forward unconditional branch over nothing but pool words is
            // the compiler stepping around a pool it placed mid-function, not
            // control flow. Clearing registers on it loses every argument the
            // caller had already loaded -- which is exactly what the reference
            // does, setting r1 before the skip and r0 after it.
            _ if pool_skip.contains(&r.addr) => {}
            _ if op.starts_with('b') && op != "bx" => {
                for i in 0..4 {
                    let k = format!("r{i}");
                    reg.remove(&k);
                    fresh.remove(&k);
                    ptr.remove(&k);
                }
                stack.clear();
                loaded.clear();
                pending.clear();
            }
            _ => {}
        }
    }

    while depth > 1 {
        depth -= 1;
        out.push(format!("{}}}", indent(depth)));
    }

    // Promote only the temps something later reads, and only when the
    // originating line is still a bare call. Folding that call into an `if`
    // leaves `if (f() != 0) {` in the slot; prefixing `pN =` makes it not C.
    let mut used: BTreeSet<String> = BTreeSet::new();
    for (name, at) in &temp_line {
        let referenced = out
            .iter()
            .enumerate()
            .any(|(i, l)| i != *at && l.contains(name.as_str()));
        if referenced && out.get(*at).is_some_and(|l| is_call_stmt(l)) {
            used.insert(name.clone());
            if let Some(f) = temp_fn.get(name) {
                returns_pointer.insert(f.clone());
            }
            let line = out[*at].clone();
            let lead: String = line.chars().take_while(|c| c.is_whitespace()).collect();
            let call = line.trim_start().trim_end_matches(';');
            out[*at] = format!("{lead}{name} = {call};");
        }
    }

    let mut lines = Vec::new();
    lines.push("#include \"types.h\"".to_string());
    lines.push(String::new());
    lines.push(format!(
        "/* DRAFT for {func}: {calls} calls, {loops} loops, {memory} memory operations.",
        loops = loop_n
    ));
    lines.push(" * Written by reconstruct from the owner's own disassembly.".to_string());
    lines.push(" * It is a starting point, not a reconstruction: read the assembly and".to_string());
    lines.push(" * fix it. Score it before believing any of it. */".to_string());
    lines.push(String::new());
    for n in &names {
        if returns_pointer.contains(n) {
            lines.push(format!("void *{n}();"));
        } else if tested.contains(n) {
            lines.push(format!("s32 {n}();"));
        } else {
            lines.push(format!("void {n}();"));
        }
    }
    lines.push(String::new());
    lines.push(format!("void {func}(void)"));
    lines.push("{".to_string());
    if !base_values.is_empty() {
        lines.push("    s32 base;".to_string());
    }
    for name in &used {
        lines.push(format!("    void *{name};"));
    }
    for k in 1..=loop_n {
        lines.push(format!("    s32 i{k};"));
    }
    if !base_values.is_empty() || loop_n > 0 || !used.is_empty() {
        lines.push(String::new());
    }
    lines.extend(balance_braces(out));
    lines.push("}".to_string());

    Draft { lines, calls, loops: loop_n, memory }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn listing(rows: &[&str]) -> Vec<String> {
        rows.iter().map(|s| s.to_string()).collect()
    }

    #[test]
    fn extra_close_braces_are_dropped_and_open_blocks_are_closed() {
        let kept = balance_braces(vec![
            "    if (1) {".into(),
            "        f();".into(),
            "    }".into(),
            "    }".into(),
            "    if (2) {".into(),
        ]);
        assert_eq!(
            kept,
            vec![
                "    if (1) {".to_string(),
                "        f();".to_string(),
                "    }".to_string(),
                "    if (2) {".to_string(),
                "}".to_string(),
            ]
        );
    }

    #[test]
    fn a_call_takes_the_arguments_freshly_loaded_into_r0_r3() {
        let d = draft(
            &listing(&[
                " 2000000:\t2004      \tmovs\tr0, #4",
                " 2000002:\t2101      \tmovs\tr1, #1",
                " 2000004:\tf000 f800 \tbl\t0x2000100",
            ]),
            "Func_02000000",
        );
        assert_eq!(d.calls, 1);
        assert!(d.lines.iter().any(|l| l.contains("Func_02000100(4, 1);")), "{:?}", d.lines);
    }

    #[test]
    fn a_backward_branch_becomes_a_loop_with_its_real_start() {
        let d = draft(
            &listing(&[
                " 2000000:\t251d      \tmovs\tr5, #29",
                " 2000002:\t2000      \tmovs\tr0, #0",
                " 2000004:\tf000 f800 \tbl\t0x2000100",
                " 2000008:\t2d00      \tcmp\tr5, #0",
                " 200000a:\tdafa      \tbge.n\t0x2000002",
            ]),
            "Func_02000000",
        );
        assert_eq!(d.loops, 1);
        assert!(
            d.lines.iter().any(|l| l.contains("for (i1 = 29; i1 >= 0; i1--)")),
            "a countdown must not be read as starting at zero: {:?}",
            d.lines
        );
    }

    #[test]
    fn r4_to_r7_survive_a_call_and_r0_does_not() {
        // r5 is loaded once and used as an argument after an intervening call.
        let d = draft(
            &listing(&[
                " 2000000:\t2507      \tmovs\tr5, #7",
                " 2000002:\tf000 f800 \tbl\t0x2000100",
                " 2000006:\t1c28      \tadds\tr0, r5, #0",
                " 2000008:\tf000 f800 \tbl\t0x2000200",
            ]),
            "Func_02000000",
        );
        assert!(
            d.lines.iter().any(|l| l.contains("Func_02000200(7);")),
            "a value parked in r5 must survive the call: {:?}",
            d.lines
        );
    }

    #[test]
    fn a_pool_word_is_not_decoded_as_an_instruction() {
        // The `@ (0x...)` comment is how the pool is located; without it the
        // two halfwords below would read as instructions.
        let d = draft(
            &listing(&[
                " 2000000:\t4801      \tldr\tr0, [pc, #4]\t@ (0x2000008)",
                " 2000002:\tf000 f800 \tbl\t0x2000100",
                " 2000008:\t0301      \tlsls\tr1, r0, #12",
                " 200000a:\t0000      \tmovs\tr0, r0",
            ]),
            "Func_02000000",
        );
        assert_eq!(d.calls, 1, "the pool must not add calls: {:?}", d.lines);
        assert!(d.lines.iter().any(|l| l.contains("Func_02000100(769);")), "{:?}", d.lines);
    }

    #[test]
    fn a_branch_over_a_pool_does_not_clear_the_arguments_behind_it() {
        // The compiler sets one argument, steps around a pool it placed
        // mid-function, sets the other, and calls. Treating that branch as
        // control flow drops the argument loaded before it -- which is how a
        // 1,574-byte owner came out two bytes short with one wrong
        // instruction, when it was otherwise exact.
        let d = draft(
            &listing(&[
                " 2000000:\t21ec      \tmovs\tr1, #236",
                " 2000002:\te002      \tb.n\t0x2000008",
                " 2000004:\t0000      \tmovs\tr0, r0",
                " 2000006:\t0c1e      \tlsrs\tr6, r3, #16",
                " 2000008:\t2002      \tmovs\tr0, #2",
                " 200000a:\tf001 f800 \tbl\t0x2001000",
            ]),
            "Func_02000000",
        );
        assert!(
            d.lines.iter().any(|l| l.contains("Func_02001000(2, 236);")),
            "the argument set before the pool skip must survive it: {:?}",
            d.lines
        );
    }

    #[test]
    fn an_if_does_not_fold_a_close_brace_or_a_loop_head() {
        let d = draft(
            &listing(&[
                " 2000000:\t2000      \tmovs\tr0, #0",
                " 2000002:\t2800      \tcmp\tr0, #0",
                " 2000004:\td001      \tbeq.n\t0x200000a",
                " 2000006:\te7fe      \tb.n\t0x2000006",
                " 2000008:\t2000      \tmovs\tr0, #0",
                " 200000a:\tf000 f800 \tbl\t0x2000100",
                " 200000e:\t2800      \tcmp\tr0, #0",
                " 2000010:\td000      \tbeq.n\t0x2000014",
                " 2000014:\t4770      \tbx\tlr",
            ]),
            "Func_02000000",
        );
        assert!(
            d.lines.iter().all(|l| !l.contains("if (}") && !l.contains("= if (")),
            "folding must not eat braces: {:?}",
            d.lines
        );
    }

    #[test]
    fn a_read_modify_write_through_a_pointer_is_one_statement() {
        let d = draft(
            &listing(&[
                " 2000000:\tf000 f800 \tbl\t0x2000100",
                " 2000004:\t305a      \tadds\tr0, #90",
                " 2000006:\t7802      \tldrb\tr2, [r0, #0]",
                " 2000008:\t25fe      \tmovs\tr5, #254",
                " 200000a:\t402a      \tands\tr2, r5",
                " 200000c:\t7002      \tstrb\tr2, [r0, #0]",
            ]),
            "Func_02000000",
        );
        assert!(
            d.lines.iter().any(|l| l.contains("&= 0xfe;")),
            "the load, mask and store are one compound assignment: {:?}",
            d.lines
        );
    }

    #[test]
    fn a_dot_s_path_is_assembly_and_an_overlay_row_is_not() {
        assert!(looks_like_assembly("asm/080bbb0c.s"));
        assert!(looks_like_assembly("080bbb0c"));
        assert!(looks_like_assembly("0x080bbb0c"));
        assert!(!looks_like_assembly("resource_3bd:13f8"));
        assert!(!looks_like_assembly("--out"));
    }

    #[test]
    fn a_whole_overlay_listing_is_refused() {
        let err = resolve_assembly("assets/code/resource_373_overlay.s").unwrap_err();
        assert!(err.contains("<overlay>:<offsetHex>"), "{err}");
    }

    #[test]
    fn drafts_cannot_leave_work() {
        let d = Draft { lines: vec!["int x;".into()], calls: 0, loops: 0, memory: 0 };
        let err = write_draft("exact/080bbb0c.c", &d).unwrap_err();
        assert!(err.contains("scratch/"), "{err}");
    }
}

/// `overlay reconstruct <overlay>:<offsetHex> [--span BYTES] [--out PATH]`
/// `overlay reconstruct <asm/addr.s> [--out PATH]`
/// `compiler reconstruct <asm/addr.s> [--out PATH]`
///
/// Writes the draft to `work/` and prints what it modelled. It never writes
/// anywhere else: the tree has no home for C that does not reproduce, and this
/// produces C that has not been read yet, which is further from reproducing
/// than usual. A `.s` path is the main-image form: assemble the file, objdump
/// it, and lift the listing the same way an overlay row is lifted.
pub fn run(argv: &[String]) -> Result<Vec<String>, String> {
    if argv.iter().any(|a| a == "-h" || a == "--help") {
        return Ok(vec![
            "usage: reconstruct <overlay>:<offsetHex> [--span BYTES] [--out PATH]".to_string(),
            "       reconstruct <asm/addr.s> [--out PATH]".to_string(),
            String::new(),
            "Drafts C for one owner from its own disassembly, into scratch/.".to_string(),
            "A .s file is assembled and lifted; an overlay row is shown and lifted.".to_string(),
            "It is a starting point. Score it before believing any of it.".to_string(),
        ]);
    }
    let value_after = |flag: &str| -> Option<String> {
        argv.iter().position(|a| a == flag).and_then(|i| argv.get(i + 1)).cloned()
    };

    if let Some(path) = assembly_arg(argv) {
        return run_assembly(&path, value_after("--out"));
    }

    let target = argv
        .iter()
        .find(|a| a.contains(':') && !a.starts_with('-'))
        .ok_or("give an owner as <overlay>:<offsetHex> or a path to asm/<addr>.s")?;
    let (overlay, offset) = target.split_once(':').ok_or("expected <overlay>:<offsetHex>")?;

    let mut show: Vec<String> = vec![overlay.to_string(), offset.to_string()];
    if let Some(span) = value_after("--span") {
        show.push("-n".to_string());
        show.push(span);
    }
    let listing = match crate::run(&show)? {
        crate::Outcome::Lines(lines) => lines,
        _ => return Err("overlay show produced no listing".to_string()),
    };
    if listing.is_empty() {
        return Err(format!("{target}: no disassembly; check the overlay and offset"));
    }

    let addr = i64::from_str_radix(offset.trim_start_matches("0x"), 16)
        .map_err(|_| format!("{offset}: not a hex offset"))?;
    let func = format!("Func_{:08x}", 0x0200_0000 + addr);
    let d = draft(&listing, &func);

    let out = value_after("--out").unwrap_or_else(|| {
        format!("scratch/{}_c_{:08x}.c", overlay, 0x0200_0000 + addr)
    });
    write_draft(&out, &d)?;

    Ok(vec![
        format!(
            "{target}: {} calls, {} loops, {} memory operations -> {out}",
            d.calls, d.loops, d.memory
        ),
        "score it before believing any of it".to_string(),
    ])
}

fn assembly_arg(argv: &[String]) -> Option<String> {
    argv.iter()
        .find(|a| !a.starts_with('-') && looks_like_assembly(a))
        .cloned()
}

fn looks_like_assembly(arg: &str) -> bool {
    if arg.ends_with(".s") || arg.ends_with(".S") {
        return true;
    }
    let trimmed = arg.strip_prefix("0x").or_else(|| arg.strip_prefix("0X")).unwrap_or(arg);
    !trimmed.is_empty()
        && trimmed.len() <= 8
        && trimmed.bytes().all(|b| b.is_ascii_hexdigit())
}

fn resolve_assembly(arg: &str) -> Result<std::path::PathBuf, String> {
    let given = std::path::PathBuf::from(arg);
    if given.extension().and_then(|e| e.to_str()) == Some("s")
        || given.extension().and_then(|e| e.to_str()) == Some("S")
    {
        if arg.contains("_overlay.s") {
            return Err(format!(
                "{arg}: a whole overlay listing is not one owner -- use <overlay>:<offsetHex>"
            ));
        }
        if given.is_file() {
            return Ok(given);
        }
        let rooted = repo_root().join(&given);
        if rooted.is_file() {
            return Ok(rooted);
        }
        return Err(format!("{arg}: no such assembly file"));
    }
    let digits = arg.strip_prefix("0x").or_else(|| arg.strip_prefix("0X")).unwrap_or(arg);
    let addr = u32::from_str_radix(digits, 16).map_err(|_| format!("{arg}: not a hex address"))?;
    let name = format!("{addr:08x}.s");
    let relative = std::path::Path::new("asm").join(&name);
    if relative.is_file() {
        return Ok(relative);
    }
    let rooted = repo_root().join("asm").join(&name);
    if rooted.is_file() {
        return Ok(rooted);
    }
    Err(format!("asm/{name}: no such owner assembly"))
}

fn repo_root() -> std::path::PathBuf {
    std::path::Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(|p| p.parent())
        .expect("overlay-show sits under tools/")
        .to_path_buf()
}

fn run_assembly(arg: &str, out: Option<String>) -> Result<Vec<String>, String> {
    let path = resolve_assembly(arg)?;
    let stem = path
        .file_stem()
        .and_then(|s| s.to_str())
        .ok_or_else(|| format!("{}: cannot read stem", path.display()))?
        .to_string();
    let work = std::path::PathBuf::from("scratch").join("reconstruct").join(&stem);
    let assembled = crate::gas::assemble_path(&path, &work)?;
    let d = draft(&assembled.listing, &assembled.func);
    let out = out.unwrap_or_else(|| format!("scratch/{stem}.c"));
    write_draft(&out, &d)?;
    Ok(vec![
        format!(
            "{}: {} calls, {} loops, {} memory operations -> {out}",
            path.display(),
            d.calls,
            d.loops,
            d.memory
        ),
        "score it before believing any of it".to_string(),
    ])
}

fn write_draft(out: &str, d: &Draft) -> Result<(), String> {
    if !out.starts_with("scratch/") {
        return Err(format!(
            "{out}: drafter output goes under scratch/, which is gitignored -- an unread draft is not an asset"
        ));
    }
    if let Some(parent) = std::path::Path::new(out).parent() {
        std::fs::create_dir_all(parent).map_err(|e| e.to_string())?;
    }
    std::fs::write(out, format!("{}\n", d.lines.join("\n"))).map_err(|e| e.to_string())
}
