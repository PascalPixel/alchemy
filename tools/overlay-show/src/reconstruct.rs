use std::collections::{BTreeMap, BTreeSet};
struct Row {
    addr: i64,
    text: String,
    lo: u16,
    hi: Option<u16>,
}
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
    let Some(name) = t.split('(').next() else {
        return false;
    };
    name.starts_with("Func_") && t.ends_with(')') && !t.contains('{') && !t.contains('}')
}
fn hex(s: &str) -> Option<i64> {
    i64::from_str_radix(s.trim_start_matches("0x"), 16).ok()
}
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
fn operands(text: &str) -> (String, Vec<String>) {
    let mut parts = text.splitn(2, |c: char| c.is_whitespace());
    let op = parts.next().unwrap_or("").to_string();
    let rest = parts.next().unwrap_or("").trim();
    let mut args: Vec<String> = Vec::new();
    let mut cur = String::new();
    let mut depth = 0i32;
    for c in rest.chars() {
        match c {
            '[' | '{' => {
                depth += 1;
                cur.push(c);
            }
            ']' | '}' => {
                depth -= 1;
                cur.push(c);
            }
            ',' if depth == 0 => {
                if !cur.trim().is_empty() {
                    args.push(cur.trim().to_string());
                }
                cur.clear();
            }
            _ => cur.push(c),
        }
    }
    if !cur.trim().is_empty() {
        args.push(cur.trim().to_string());
    }
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
pub fn draft(listing: &[String], func: &str) -> Draft {
    let rows: Vec<Row> = listing.iter().filter_map(|l| parse_row(l)).collect();
    let mut halfwords: BTreeMap<i64, u16> = BTreeMap::new();
    for r in &rows {
        halfwords.insert(r.addr, r.lo);
        if let Some(h) = r.hi {
            halfwords.insert(r.addr + 2, h);
        }
    }
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
                if target > r.addr && !rows.iter().any(|x| x.addr > r.addr && x.addr < target && branch_targets.contains(&x.addr)) {
                    pool_skip.insert(r.addr);
                    for x in rows.iter().filter(|x| x.addr > r.addr && x.addr < target) {
                        pool_words.insert(x.addr);
                    }
                }
            }
        }
    }
    let mut loop_top: BTreeMap<i64, (i64, i64, String)> = BTreeMap::new();
    let mut loop_end: BTreeSet<i64> = BTreeSet::new();
    for (i, r) in rows.iter().enumerate() {
        let (op, args) = operands(&r.text);
        if !op.starts_with('b') || op == "bx" || op == "bl" || args.len() != 1 {
            continue;
        }
        let Some(target) = hex(&args[0]) else {
            continue;
        };
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
    let mut if_at: BTreeMap<i64, (String, i64, Option<i64>)> = BTreeMap::new();
    let mut close_at: BTreeMap<i64, usize> = BTreeMap::new();
    let mut else_at: BTreeMap<i64, i64> = BTreeMap::new();
    let mut and_join: BTreeMap<i64, String> = BTreeMap::new();
    for (i, r) in rows.iter().enumerate() {
        let (op, args) = operands(&r.text);
        let cond = matches!(op.as_str(), "beq" | "beq.n" | "bne" | "bne.n" | "blt" | "blt.n" | "bgt" | "bgt.n" | "bge" | "bge.n" | "ble" | "ble.n");
        if !cond || args.len() != 1 {
            continue;
        }
        let Some(target) = hex(&args[0]) else {
            continue;
        };
        if target <= r.addr || pool_words.contains(&r.addr) {
            continue;
        }
        let Some(prev) = rows.get(i.wrapping_sub(1)) else {
            continue;
        };
        let (pop_, pargs) = operands(&prev.text);
        if pop_ != "cmp" || pargs.len() != 2 || pargs[0] != "r0" || immediate(&pargs[1]) != Some(0) {
            continue;
        }
        let test = match op.trim_end_matches(".n") {
            "beq" => "!= 0",
            "bne" => "== 0",
            "blt" => ">= 0",
            "bge" => "< 0",
            "bgt" => "<= 0",
            "ble" => "> 0",
            _ => continue,
        };
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
        if let Some(prior) = if_at.iter().find(|(_, v)| v.1 == target && v.2.is_none()).map(|(k, _)| *k) {
            if join.is_some() {
                let existing = if_at.remove(&prior).unwrap();
                if_at.insert(prior, (existing.0, target, join));
                if let Some(n) = close_at.get_mut(&target) {
                    if *n > 0 {
                        *n -= 1;
                    }
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
    let mut tested: BTreeSet<String> = BTreeSet::new();
    let mut returns_pointer: BTreeSet<String> = BTreeSet::new();
    let mut tmp_n = 0usize;
    let mut temp_line: BTreeMap<String, usize> = BTreeMap::new();
    let mut temp_fn: BTreeMap<String, String> = BTreeMap::new();
    let indent = |d: usize| "    ".repeat(d);
    let mut base_values: BTreeSet<i64> = BTreeSet::new();
    for r in &rows {
        let (op, args) = operands(&r.text);
        if op == "ldr" && args.len() == 2 && args[0].starts_with("r") {
            let n: u32 = args[0][1..].parse().unwrap_or(0);
            if (4..=7).contains(&n) && args[1].starts_with("[pc") {
                if let Some(off) = args.get(1).and_then(|a| a.split('#').nth(1).and_then(|s| s.trim_end_matches(']').parse::<i64>().ok())) {
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
            out.push(format!("{}for (i{loop_n} = {start}; i{loop_n} {cmp} {bound}; i{loop_n}{step}) {{", indent(depth)));
            depth += 1;
        }
        while let Some(n) = close_at.get_mut(&r.addr) {
            if *n == 0 {
                break;
            }
            *n -= 1;
            depth = depth.saturating_sub(1).max(1);
            out.push(format!("{}}}", indent(depth)));
            if *n == 0 {
                close_at.remove(&r.addr);
                break;
            }
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
            if out.last().is_some_and(|l| is_call_stmt(l)) {
                if let Some(prev) = out.pop() {
                    let call = prev.trim().trim_end_matches(';').to_string();
                    if let Some(name) = call.split('(').next() {
                        if name.starts_with("Func_") {
                            tested.insert(name.to_string());
                        }
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
                        let v = loaded.get(&args[0]).cloned().or_else(|| reg.get(&args[0]).map(|v| v.render())).unwrap_or_else(|| "0".to_string());
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
                let (src, shift) = if args.len() == 3 { (args[1].clone(), immediate(&args[2])) } else { (args[0].clone(), immediate(&args[1])) };
                if let (Some(Val::Num(n)), Some(s)) = (reg.get(&src).cloned(), shift) {
                    reg.insert(args[0].clone(), Val::Num(n << s));
                    fresh.insert(args[0].clone(), Val::Num(n << s));
                } else {
                    reg.remove(&args[0]);
                    fresh.remove(&args[0]);
                }
            }
            "adds" => {
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
                let (a, b) = if args.len() == 3 { (reg.get(&args[1]).cloned(), immediate(&args[2]).map(Val::Num).or_else(|| reg.get(&args[2]).cloned())) } else { (reg.get(&args[0]).cloned(), immediate(&args[1]).map(Val::Num)) };
                let sum = match (a, b) {
                    (Some(Val::Num(x)), Some(Val::Num(y))) => Some(Val::Num(x + y)),
                    (Some(Val::Named(n)), Some(Val::Num(y))) => Some(if y == 0 { Val::Named(n) } else { Val::Named(format!("{n} + {y}")) }),
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
                let mask = if loaded.contains_key(&args[1]) { reg.get(&args[0]).cloned() } else { reg.get(&args[1]).cloned() };
                if let (Some(place), Some(Val::Num(m))) = (place, mask) {
                    pending.insert(args[0].clone(), (place, sym.to_string(), m));
                }
                reg.remove(&args[0]);
                fresh.remove(&args[0]);
            }
            "ldr" if args.len() == 2 && args[1].starts_with("[pc") => {
                let off = args[1].split('#').nth(1).and_then(|s| s.trim_end_matches(']').parse::<i64>().ok()).unwrap_or(0);
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
                let off = args[1].split('#').nth(1).and_then(|s| s.trim_end_matches(']').parse::<i64>().ok()).unwrap_or(0);
                if let Some(v) = reg.get(&args[0]).cloned() {
                    stack.insert(off, v);
                }
            }
            _ if op.starts_with("bl") && args.len() == 1 => {
                let Some(target) = hex(&args[0]) else {
                    continue;
                };
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
    let mut used: BTreeSet<String> = BTreeSet::new();
    for (name, at) in &temp_line {
        let referenced = out.iter().enumerate().any(|(i, l)| i != *at && l.contains(name.as_str()));
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
    lines.push(format!("/* DRAFT for {func}: {calls} calls, {loops} loops, {memory} memory operations.", loops = loop_n));
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

/// Draft one overlay owner from its disassembly into the ignored scratch tree.
pub fn run(argv: &[String]) -> Result<Vec<String>, String> {
    if argv.iter().any(|argument| matches!(argument.as_str(), "-h" | "--help")) {
        return Ok(vec!["usage: overlay reconstruct <overlay>:<offsetHex> [--span BYTES] [--out PATH]".to_string(), "Drafts one owner into scratch/. Score and read it before promotion.".to_string()]);
    }
    let value_after = |flag: &str| argv.iter().position(|argument| argument == flag).and_then(|index| argv.get(index + 1)).cloned();
    let target = argv.iter().find(|argument| argument.contains(':') && !argument.starts_with('-')).ok_or("give an owner as <overlay>:<offsetHex>")?;
    let (overlay, offset) = target.split_once(':').ok_or("expected <overlay>:<offsetHex>")?;
    let mut show = vec![overlay.to_string(), offset.to_string()];
    if let Some(span) = value_after("--span") {
        show.extend(["-n".to_string(), span]);
    }
    let listing = match crate::run(&show)? {
        crate::Outcome::Lines(lines) => lines,
        _ => return Err("overlay show produced no listing".to_string()),
    };
    if listing.is_empty() {
        return Err(format!("{target}: no disassembly; check the overlay and offset"));
    }
    let address = i64::from_str_radix(offset.trim_start_matches("0x"), 16).map_err(|_| format!("{offset}: not a hex offset"))?;
    let draft = draft(&listing, &format!("Func_{:08x}", 0x0200_0000 + address));
    let output = value_after("--out").unwrap_or_else(|| format!("scratch/{overlay}_c_{:08x}.c", 0x0200_0000 + address));
    write_draft(&output, &draft)?;
    Ok(vec![format!("{target}: {} calls, {} loops, {} memory operations -> {output}", draft.calls, draft.loops, draft.memory), "score it before believing any of it".to_string()])
}

fn write_draft(output: &str, draft: &Draft) -> Result<(), String> {
    if !output.starts_with("scratch/") {
        return Err(format!("{output}: drafter output belongs under scratch/, which is gitignored"));
    }
    if let Some(parent) = std::path::Path::new(output).parent() {
        std::fs::create_dir_all(parent).map_err(|error| error.to_string())?;
    }
    std::fs::write(output, format!("{}\n", draft.lines.join("\n"))).map_err(|error| error.to_string())
}
