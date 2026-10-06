//! `alchemy humanize`: the permuter's search with its objective turned
//! around. For a function that already matches, it tries readability
//! rewrites and keeps one only when the whole object compiles to exactly the
//! same bytes as the current spelling (code, literal pools, data, relocations
//! and symbols) and `alchemy worst`'s score of the function falls. No listing
//! and nothing from the ROM is read: the current source's own object is the
//! reference. FAKEMATCH-tagged statements, asm, volatile and register
//! variables are never touched. The file is written once, at the end, in its
//! own lines where the rewrites carry into them; then `alchemy keep` judges
//! all twelve editions.

use super::ast::{BinOp, Decl, Expr, Function, Init, Stmt, UnOp};
use super::mutate::Rng;
use super::parse::{locate, scan_unit, Unit};
use crate::compiler::routing::root;
use crate::targets::{decomp_target, DecompTarget};
use std::path::{Path, PathBuf};
use std::time::{Duration, Instant};

const USAGE: &str = r#"usage: alchemy humanize FILE.C [--function NAME] [--minutes N] [--jobs J] [--seed S] [--edition EDITION] [--dry-run]
Rewrites a matching function toward plainer C without changing one byte of
its object: each candidate is one rewrite of the current best, compiled with
alchemy diff's steps for FILE.C, and kept only when the object file is
byte-identical to the current spelling's and alchemy worst's score of the
function falls (then hex numbers, narrow locals, lines and characters break
ties). Rewrites: drop a value or pointer cast, fold a single-use temporary
into its use, merge a declaration with its first assignment, widen a u8, s8,
u16 or s16 local to s32, compound assignment, ++ and --, a negated
comparison, a do { } while (0) block back to plain statements, a goto
over statements to an if, a goto back to its label to a do-while, a hex
quantity outside bit operations to decimal. Statements after a FAKEMATCH
comment, and any with asm, volatile or register, are left as written.
Candidates are tried in an order drawn from --seed, J at a time; the first
in that order that passes is kept, so a seed repeats exactly. Without
--function every function in FILE.C is humanized in turn. FILE.C is written
once at the end (not with --dry-run); run alchemy keep FILE.C next.
Defaults: --minutes 5 per function, --jobs cores - 2, --seed 1, the English
edition of the file's game."#;

struct Options {
    file: PathBuf,
    function: Option<String>,
    minutes: f64,
    jobs: usize,
    seed: u64,
    target: DecompTarget,
    dry_run: bool,
}

fn parse_options(arguments: &[String]) -> Result<Options, String> {
    let (mut file, mut function, mut target) = (None, None, None);
    let mut minutes = 5.0;
    let mut jobs = std::thread::available_parallelism().map_or(1, |n| n.get().saturating_sub(2).max(1));
    let (mut seed, mut dry_run) = (1, false);
    let mut index = 0;
    let value = |index: usize| arguments.get(index).cloned().ok_or_else(|| USAGE.to_string());
    while index < arguments.len() {
        match arguments[index].as_str() {
            "-h" | "--help" => return Err(USAGE.into()),
            "--function" => {
                index += 1;
                function = Some(value(index)?);
            }
            "--minutes" => {
                index += 1;
                minutes = value(index)?.parse().map_err(|_| format!("--minutes: a number\n{USAGE}"))?;
            }
            "--jobs" => {
                index += 1;
                jobs = value(index)?.parse::<usize>().map_err(|_| format!("--jobs: a count\n{USAGE}"))?.max(1);
            }
            "--seed" => {
                index += 1;
                seed = value(index)?.parse().map_err(|_| format!("--seed: a number\n{USAGE}"))?;
            }
            "--edition" => {
                index += 1;
                target = Some(decomp_target(Some(&value(index)?))?);
            }
            "--dry-run" => dry_run = true,
            other if other.starts_with('-') => return Err(format!("unknown humanize option: {other}\n{USAGE}")),
            other if file.is_none() => file = Some(PathBuf::from(other)),
            _ => return Err(USAGE.into()),
        }
        index += 1;
    }
    let file = file.ok_or_else(|| USAGE.to_string())?;
    let text = file.to_string_lossy().into_owned();
    let target = match target {
        Some(target) => target,
        None if text.contains("THE LOST AGE") || text.contains("/tla/") => decomp_target(Some("tla-en"))?,
        None => decomp_target(Some("tbs-en"))?,
    };
    Ok(Options { file, function, minutes, jobs, seed, target, dry_run })
}

// ---------------------------------------------------------------- rewrites

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum Rewrite {
    ValueCast,
    PointerCast,
    FoldTemporary,
    MergeDeclaration,
    WidenLocal,
    Compound,
    Increment,
    NegatedComparison,
    OnePass,
    Goto,
    HexQuantity,
}

impl Rewrite {
    pub const ALL: [Rewrite; 11] = [
        Rewrite::ValueCast,
        Rewrite::PointerCast,
        Rewrite::FoldTemporary,
        Rewrite::MergeDeclaration,
        Rewrite::WidenLocal,
        Rewrite::Compound,
        Rewrite::Increment,
        Rewrite::NegatedComparison,
        Rewrite::OnePass,
        Rewrite::Goto,
        Rewrite::HexQuantity,
    ];

    pub fn name(self) -> &'static str {
        match self {
            Rewrite::ValueCast => "drop a value cast",
            Rewrite::PointerCast => "drop a pointer cast",
            Rewrite::FoldTemporary => "fold a single-use temporary",
            Rewrite::MergeDeclaration => "merge a declaration with its first assignment",
            Rewrite::WidenLocal => "widen a narrow local to s32",
            Rewrite::Compound => "compound assignment",
            Rewrite::Increment => "increment or decrement",
            Rewrite::NegatedComparison => "negated comparison",
            Rewrite::OnePass => "do { } while (0) to plain statements",
            Rewrite::Goto => "goto to if or do-while",
            Rewrite::HexQuantity => "hex quantity to decimal",
        }
    }
}

/// Counts sites in a walk and fires at the chosen one.
struct Cursor {
    target: usize,
    seen: usize,
}

impl Cursor {
    fn hit(&mut self) -> bool {
        let fire = self.seen == self.target;
        self.seen += 1;
        fire
    }

    fn done(&self) -> bool {
        self.seen > self.target
    }
}

/// A statement the rewrites leave alone: a FAKEMATCH device or anything
/// that names asm, volatile or register.
fn guarded(stmt: &Stmt) -> bool {
    let mut found = false;
    stmt.each(&mut |inner| {
        if let Stmt::Decl(decl) = inner {
            if decl.specs.iter().any(|s| s == "volatile" || s == "register") || decl.items.iter().any(|item| item.before.iter().chain(&item.after).any(|t| t == "volatile" || t.contains(PLACEHOLDER))) {
                found = true;
            }
        }
        if let Stmt::Comment(text) = inner {
            if text.contains("FAKEMATCH") {
                found = true;
            }
        }
    });
    stmt.each_expr(&mut |expr| match expr {
        Expr::Ident(name) if name.starts_with(PLACEHOLDER) || name == "volatile" || name == "__volatile__" => found = true,
        Expr::Cast(ty, _) if ty.tokens.iter().any(|t| t == "volatile") => found = true,
        _ => {}
    });
    found
}

/// Which statements of a block are open to rewriting: not guarded, and not
/// the statement a FAKEMATCH comment stands before.
fn open_mask(stmts: &[Stmt]) -> Vec<bool> {
    let mut mask = Vec::with_capacity(stmts.len());
    let (mut tagged, mut depth) = (false, 0);
    for stmt in stmts {
        if let Some(step) = directive_step(stmt) {
            depth += step;
            mask.push(false);
            continue;
        }
        if depth > 0 {
            // Inside an edition's #if: only one branch compiles here.
            mask.push(false);
            continue;
        }
        match stmt {
            Stmt::Comment(text) => {
                tagged |= text.contains("FAKEMATCH");
                mask.push(false);
            }
            other => {
                mask.push(!tagged && !guarded(other));
                tagged = false;
            }
        }
    }
    mask
}

fn children_mut(expr: &mut Expr) -> Vec<&mut Expr> {
    match expr {
        Expr::Ident(_) | Expr::Number(_) | Expr::Char(_) | Expr::Str(_) | Expr::SizeofType(_) => Vec::new(),
        Expr::Unary(_, a) | Expr::PostInc(a) | Expr::PostDec(a) | Expr::Member(a, ..) | Expr::Cast(_, a) | Expr::SizeofExpr(a) => vec![a],
        Expr::Binary(_, a, b) | Expr::Assign(_, a, b) | Expr::Comma(a, b) | Expr::Index(a, b) => vec![a, b],
        Expr::Cond(a, b, c) => vec![a, b, c],
        Expr::Call(callee, arguments) => {
            let mut all = vec![callee.as_mut()];
            all.extend(arguments.iter_mut());
            all
        }
    }
}

fn own_exprs_mut(stmt: &mut Stmt) -> Vec<&mut Expr> {
    match stmt {
        Stmt::Expr(e) | Stmt::If(e, ..) | Stmt::While(e, _) | Stmt::DoWhile(_, e) | Stmt::Switch(e, _) | Stmt::Return(Some(e)) => vec![e],
        Stmt::For(a, b, c, _) => [a, b, c].into_iter().filter_map(Option::as_mut).collect(),
        Stmt::Decl(decl) => {
            let mut all = Vec::new();
            for item in &mut decl.items {
                if let Some(init) = &mut item.init {
                    init_exprs_mut(init, &mut all);
                }
            }
            all
        }
        _ => Vec::new(),
    }
}

fn init_exprs_mut<'a>(init: &'a mut Init, all: &mut Vec<&'a mut Expr>) {
    match init {
        Init::Expr(e) => all.push(e),
        Init::List(items) => items.iter_mut().for_each(|item| init_exprs_mut(item, all)),
    }
}

fn nested_mut(stmt: &mut Stmt) -> Vec<&mut Vec<Stmt>> {
    let mut out = Vec::new();
    match stmt {
        Stmt::Block(stmts) => out.push(stmts),
        Stmt::If(_, then, other) => {
            if let Stmt::Block(stmts) = &mut **then {
                out.push(stmts);
            }
            if let Some(other) = other {
                if let Stmt::Block(stmts) = &mut **other {
                    out.push(stmts);
                }
            }
        }
        Stmt::While(_, body) | Stmt::DoWhile(body, _) | Stmt::For(_, _, _, body) | Stmt::Switch(_, body) => {
            if let Stmt::Block(stmts) = &mut **body {
                out.push(stmts);
            }
        }
        _ => {}
    }
    out
}

/// Pre-order walk of every expression in open statements.
fn walk_exprs(stmts: &mut [Stmt], visit: &mut dyn FnMut(&mut Expr), cursor: &Cursor) {
    let mask = open_mask(stmts);
    for (stmt, open) in stmts.iter_mut().zip(mask) {
        if cursor.done() {
            return;
        }
        if !open {
            continue;
        }
        for expr in own_exprs_mut(stmt) {
            walk_expr(expr, visit, cursor);
        }
        for block in nested_mut(stmt) {
            walk_exprs(block, visit, cursor);
        }
        // A lone statement body (if (x) y = 1;) is not a block.
        let lone: Vec<&mut Stmt> = match stmt {
            Stmt::If(_, then, other) => {
                let mut v: Vec<&mut Stmt> = Vec::new();
                if !matches!(**then, Stmt::Block(_)) {
                    v.push(then);
                }
                if let Some(other) = other {
                    if !matches!(**other, Stmt::Block(_)) {
                        v.push(other);
                    }
                }
                v
            }
            Stmt::While(_, b) | Stmt::DoWhile(b, _) | Stmt::For(_, _, _, b) | Stmt::Switch(_, b) if !matches!(**b, Stmt::Block(_)) => vec![b],
            _ => Vec::new(),
        };
        for inner in lone {
            walk_exprs(std::slice::from_mut(inner), visit, cursor);
        }
    }
}

fn walk_expr(expr: &mut Expr, visit: &mut dyn FnMut(&mut Expr), cursor: &Cursor) {
    if cursor.done() {
        return;
    }
    visit(expr);
    for child in children_mut(expr) {
        walk_expr(child, visit, cursor);
    }
}

/// Every block of open statements, the function body first.
fn walk_blocks(stmts: &mut Vec<Stmt>, visit: &mut dyn FnMut(&mut Vec<Stmt>, &[bool]), cursor: &Cursor) {
    if cursor.done() {
        return;
    }
    let mask = open_mask(stmts);
    visit(stmts, &mask);
    let mask = open_mask(stmts);
    for (stmt, open) in stmts.iter_mut().zip(mask) {
        if !open || cursor.done() {
            continue;
        }
        for block in nested_mut(stmt) {
            walk_blocks(block, visit, cursor);
        }
    }
}

const VALUE_TYPES: [&str; 12] = ["u8", "s8", "u16", "s16", "u32", "s32", "int", "unsigned", "char", "short", "long", "signed"];

fn is_value_type(tokens: &[String]) -> bool {
    !tokens.is_empty() && tokens.iter().all(|t| VALUE_TYPES.contains(&t.as_str()))
}

fn hex_value(text: &str) -> Option<u64> {
    let digits = text.strip_prefix("0x").or_else(|| text.strip_prefix("0X"))?;
    let digits = digits.trim_end_matches(['u', 'U', 'l', 'L']);
    u64::from_str_radix(digits, 16).ok()
}

/// A hex number that reads as a quantity: not a mask (all ones), not a
/// single bit, not an address, and not round in hex (three trailing zero
/// digits, as 16.16 fixed point 0x18000 and angles 0x3000 are), whose unit
/// the hex spelling shows.
fn hex_quantity(expr: &Expr) -> bool {
    let Expr::Number(text) = expr else { return false };
    let Some(value) = hex_value(text) else { return false };
    value >= 10 && value < 0x0200_0000 && (value + 1) & value != 0 && value & (value - 1) != 0 && value & 0xfff != 0
}

/// A hardware register takes fields, not quantities.
fn register_place(expr: &Expr) -> bool {
    matches!(expr, Expr::Ident(name) if name.starts_with("REG_"))
}

fn bitwise(op: BinOp) -> bool {
    matches!(op, BinOp::BitAnd | BinOp::BitXor | BinOp::BitOr | BinOp::Shl | BinOp::Shr)
}

/// A side-effect-free place: a name, a member of one, or an index by names
/// and numbers.
fn pure_place(expr: &Expr) -> bool {
    match expr {
        Expr::Ident(_) => true,
        Expr::Member(base, ..) => pure_place(base),
        Expr::Index(base, index) => pure_place(base) && matches!(**index, Expr::Ident(_) | Expr::Number(_)),
        Expr::Unary(UnOp::Deref, inner) => pure_place(inner),
        _ => false,
    }
}

fn expr_rewrite(kind: Rewrite, expr: &mut Expr, cursor: &mut Cursor) {
    match kind {
        Rewrite::ValueCast | Rewrite::PointerCast => {
            if let Expr::Cast(ty, inner) = expr {
                let pointer = ty.tokens.iter().any(|t| t == "*");
                let fits = if kind == Rewrite::PointerCast { pointer } else { !pointer && is_value_type(&ty.tokens) };
                if fits && cursor.hit() {
                    *expr = (**inner).clone();
                }
            }
        }
        Rewrite::Compound => {
            if let Expr::Assign(None, place, value) = expr {
                if let Expr::Binary(op, left, right) = &**value {
                    if pure_place(place) && **left == **place && !matches!(op, BinOp::And | BinOp::Or) && !op.is_comparison() && cursor.hit() {
                        *expr = Expr::Assign(Some(*op), place.clone(), right.clone());
                    }
                }
            }
        }
        Rewrite::Increment => {
            let one = |e: &Expr| matches!(e, Expr::Number(n) if n == "1");
            let step = match expr {
                Expr::Assign(Some(BinOp::Add), place, value) if one(value) && pure_place(place) => Some((true, place.clone())),
                Expr::Assign(Some(BinOp::Sub), place, value) if one(value) && pure_place(place) => Some((false, place.clone())),
                Expr::Unary(UnOp::PreInc, place) => Some((true, place.clone())),
                Expr::Unary(UnOp::PreDec, place) => Some((false, place.clone())),
                _ => None,
            };
            if let Some((up, place)) = step {
                if cursor.hit() {
                    *expr = if up { Expr::PostInc(place) } else { Expr::PostDec(place) };
                }
            }
        }
        Rewrite::NegatedComparison => {
            if let Expr::Unary(UnOp::Not, inner) = expr {
                if matches!(**inner, Expr::Binary(op, ..) if op.is_comparison()) && cursor.hit() {
                    *expr = super::mutate::negate((**inner).clone());
                }
            }
        }
        Rewrite::HexQuantity => {
            let convert = |e: &mut Expr, cursor: &mut Cursor| {
                if hex_quantity(e) && cursor.hit() {
                    if let Expr::Number(text) = e {
                        let suffix: String = text.chars().rev().take_while(|c| matches!(c, 'u' | 'U' | 'l' | 'L')).collect::<Vec<_>>().into_iter().rev().collect();
                        *text = format!("{}{suffix}", hex_value(text).unwrap());
                    }
                }
            };
            match expr {
                Expr::Binary(op, a, b) if !bitwise(*op) => {
                    convert(a, cursor);
                    convert(b, cursor);
                }
                Expr::Assign(op, place, b) if op.is_none_or(|op| !bitwise(op)) && !register_place(place) => convert(b, cursor),
                Expr::Call(_, arguments) => arguments.iter_mut().for_each(|a| convert(a, cursor)),
                Expr::Index(_, b) => convert(b, cursor),
                _ => {}
            }
        }
        _ => {}
    }
}

fn mentions(stmts: &[Stmt], name: &str) -> usize {
    let mut count = 0;
    for stmt in stmts {
        stmt.each_expr(&mut |e| {
            if matches!(e, Expr::Ident(n) if n == name) {
                count += 1;
            }
        });
    }
    count
}

fn replace_ident(expr: &mut Expr, name: &str, with: &Expr) {
    if matches!(expr, Expr::Ident(n) if n == name) {
        *expr = with.clone();
        return;
    }
    for child in children_mut(expr) {
        replace_ident(child, name, with);
    }
}

fn scalar(decl: &Decl, index: usize) -> bool {
    let item = &decl.items[index];
    item.after.is_empty() && !item.before.iter().any(|t| t != "*") && !decl.specs.iter().any(|s| ["static", "extern", "register", "volatile", "struct", "union"].contains(&s.as_str()))
}

/// Remove the declarator of `name` wherever it is declared without an
/// initializer; drop its declaration when nothing is left in it.
fn remove_declarator(stmts: &mut Vec<Stmt>, name: &str) -> bool {
    for i in 0..stmts.len() {
        if let Stmt::Decl(decl) = &mut stmts[i] {
            if let Some(at) = decl.items.iter().position(|item| item.name == name && item.init.is_none()) {
                decl.items.remove(at);
                if decl.items.is_empty() {
                    stmts.remove(i);
                }
                return true;
            }
        }
    }
    for stmt in stmts.iter_mut() {
        for block in nested_mut(stmt) {
            if remove_declarator(block, name) {
                return true;
            }
        }
    }
    false
}

fn breaks_out(stmt: &Stmt) -> bool {
    match stmt {
        Stmt::Break | Stmt::Continue => true,
        Stmt::Block(stmts) => stmts.iter().any(breaks_out),
        Stmt::If(_, then, other) => breaks_out(then) || other.as_deref().is_some_and(breaks_out),
        _ => false,
    }
}

/// Statement-level rewrites over one block; `everywhere` is the function body
/// for whole-function mention counts.
fn block_rewrite(kind: Rewrite, stmts: &mut Vec<Stmt>, mask: &[bool], body: &[Stmt], cursor: &mut Cursor) -> Option<String> {
    match kind {
        Rewrite::FoldTemporary => {
            for i in 0..stmts.len().saturating_sub(1) {
                if !mask[i] || !mask[i + 1] {
                    continue;
                }
                let (name, value, from_decl) = match &stmts[i] {
                    Stmt::Expr(Expr::Assign(None, place, value)) => match &**place {
                        Expr::Ident(name) => (name.clone(), (**value).clone(), false),
                        _ => continue,
                    },
                    Stmt::Decl(decl) if decl.items.len() == 1 && scalar(decl, 0) => match &decl.items[0].init {
                        Some(Init::Expr(value)) => (decl.items[0].name.clone(), value.clone(), true),
                        _ => continue,
                    },
                    _ => continue,
                };
                if matches!(stmts[i + 1], Stmt::Decl(_)) {
                    continue;
                }
                let total = mentions(body, &name);
                let wanted = if from_decl { 1 } else { 2 };
                if total != wanted || mentions(std::slice::from_ref(&stmts[i + 1]), &name) != 1 {
                    continue;
                }
                if cursor.hit() {
                    for expr in own_exprs_mut(&mut stmts[i + 1]) {
                        replace_ident(expr, &name, &value);
                    }
                    stmts.remove(i);
                    return (!from_decl).then_some(name);
                }
            }
            None
        }
        Rewrite::MergeDeclaration => {
            let Some(first) = stmts.iter().position(|s| !matches!(s, Stmt::Decl(_) | Stmt::Comment(_))) else { return None };
            if !mask[first] {
                return None;
            }
            let Stmt::Expr(Expr::Assign(None, place, value)) = &stmts[first] else { return None };
            let Expr::Ident(name) = &**place else { return None };
            let (name, value) = (name.clone(), (**value).clone());
            if value.mentions(&name) > 0 {
                return None;
            }
            // The declaration must be the last one before the assignment, so
            // nothing moves past another declaration's initializer.
            let at = (0..first).rev().find(|&k| matches!(stmts[k], Stmt::Decl(_)))?;
            let Stmt::Decl(decl) = &stmts[at] else { return None };
            if !mask[at] || decl.items.len() != 1 || decl.items[0].name != name || decl.items[0].init.is_some() || !scalar(decl, 0) {
                return None;
            }
            if cursor.hit() {
                if let Stmt::Decl(decl) = &mut stmts[at] {
                    decl.items[0].init = Some(Init::Expr(value));
                }
                stmts.remove(first);
            }
            None
        }
        Rewrite::WidenLocal => {
            for (i, stmt) in stmts.iter_mut().enumerate() {
                if !mask[i] {
                    continue;
                }
                if let Stmt::Decl(decl) = stmt {
                    let narrow = decl.specs.len() == 1 && ["u8", "s8", "u16", "s16"].contains(&decl.specs[0].as_str());
                    if narrow && (0..decl.items.len()).all(|k| scalar(decl, k) && decl.items[k].before.is_empty()) && cursor.hit() {
                        decl.specs[0] = "s32".into();
                        return None;
                    }
                }
            }
            None
        }
        Rewrite::Goto => {
            // if (c) goto L; S...; L:   becomes   if (!c) { S... }
            // L: S...; if (c) goto L;   becomes   do { S... } while (c);
            let jumps = |label: &str| {
                let mut count = 0;
                for stmt in body {
                    stmt.each(&mut |s| count += usize::from(matches!(s, Stmt::Goto(l) if l == label)));
                }
                count
            };
            let goto_of = |stmt: &Stmt| -> Option<(Expr, String)> {
                let Stmt::If(cond, then, None) = stmt else { return None };
                match &**then {
                    Stmt::Goto(label) => Some((cond.clone(), label.clone())),
                    Stmt::Block(items) if items.len() == 1 => match &items[0] {
                        Stmt::Goto(label) => Some((cond.clone(), label.clone())),
                        _ => None,
                    },
                    _ => None,
                }
            };
            let plain = |range: &[Stmt]| range.iter().all(|s| !matches!(s, Stmt::Label(_) | Stmt::Decl(_) | Stmt::Case(_) | Stmt::Default));
            for i in 0..stmts.len() {
                if !mask[i] {
                    continue;
                }
                if let Some((cond, label)) = goto_of(&stmts[i]) {
                    let Some(j) = (i + 1..stmts.len()).find(|&j| matches!(&stmts[j], Stmt::Label(l) if *l == label)) else { continue };
                    if j == i + 1 || !mask[i..j].iter().all(|m| *m) || !plain(&stmts[i + 1..j]) || jumps(&label) != 1 {
                        continue;
                    }
                    if cursor.hit() {
                        let inner: Vec<Stmt> = stmts[i + 1..j].to_vec();
                        stmts.splice(i..=j, [Stmt::If(super::mutate::negate(cond), Box::new(Stmt::Block(inner)), None)]);
                        return None;
                    }
                }
                if let Stmt::Label(label) = &stmts[i] {
                    let label = label.clone();
                    let Some(j) = (i + 1..stmts.len()).find(|&j| goto_of(&stmts[j]).is_some_and(|(_, l)| l == label)) else { continue };
                    if j == i + 1 || !mask[i + 1..=j].iter().all(|m| *m) || !plain(&stmts[i + 1..j]) || jumps(&label) != 1 {
                        continue;
                    }
                    if cursor.hit() {
                        let (cond, _) = goto_of(&stmts[j]).unwrap();
                        let inner: Vec<Stmt> = stmts[i + 1..j].to_vec();
                        stmts.splice(i..=j, [Stmt::DoWhile(Box::new(Stmt::Block(inner)), cond)]);
                        return None;
                    }
                }
            }
            None
        }
        Rewrite::OnePass => {
            for i in 0..stmts.len() {
                if !mask[i] {
                    continue;
                }
                let Stmt::DoWhile(inner, Expr::Number(zero)) = &stmts[i] else { continue };
                if zero != "0" || breaks_out(inner) {
                    continue;
                }
                if cursor.hit() {
                    let inner = (**inner).clone();
                    let items = match inner {
                        Stmt::Block(items) if !items.iter().any(|s| matches!(s, Stmt::Decl(_))) => items,
                        other => vec![other],
                    };
                    stmts.splice(i..=i, items);
                    return None;
                }
            }
            None
        }
        _ => None,
    }
}

fn statement_level(kind: Rewrite) -> bool {
    matches!(kind, Rewrite::FoldTemporary | Rewrite::MergeDeclaration | Rewrite::WidenLocal | Rewrite::OnePass | Rewrite::Goto)
}

/// Apply rewrite `kind` at its `target`th site; returns how many sites
/// the walk saw (the count, when `target` is past them all).
pub fn apply(kind: Rewrite, function: &mut Function, target: usize) -> usize {
    let mut cursor = Cursor { target, seen: 0 };
    if statement_level(kind) {
        let body = function.body.clone();
        let mut folded = None;
        let cell = std::cell::RefCell::new(&mut cursor);
        walk_blocks(
            &mut function.body,
            &mut |stmts, mask| {
                let mut cursor = cell.borrow_mut();
                if !cursor.done() {
                    if let Some(name) = block_rewrite(kind, stmts, mask, &body, &mut cursor) {
                        folded = Some(name);
                    }
                }
            },
            &Cursor { target: usize::MAX, seen: 0 },
        );
        if let Some(name) = folded {
            remove_declarator(&mut function.body, &name);
        }
    } else {
        let cell = std::cell::RefCell::new(&mut cursor);
        walk_exprs(
            &mut function.body,
            &mut |expr| {
                let mut cursor = cell.borrow_mut();
                if !cursor.done() {
                    expr_rewrite(kind, expr, &mut cursor);
                }
            },
            &Cursor { target: usize::MAX, seen: 0 },
        );
    }
    cursor.seen
}

/// Every (rewrite, site) of a function, in an order drawn from `rng`.
pub fn candidates(function: &Function, rng: &mut Rng) -> Vec<(Rewrite, usize)> {
    let mut all = Vec::new();
    for kind in Rewrite::ALL {
        let sites = apply(kind, &mut function.clone(), usize::MAX);
        all.extend((0..sites).map(|site| (kind, site)));
    }
    for i in (1..all.len()).rev() {
        let j = rng.below(i + 1);
        all.swap(i, j);
    }
    all
}

// ---------------------------------------------------------------- objective

/// How mechanical a spelling reads: alchemy worst's score first, then hex
/// numbers and narrow locals, then lines and characters.
#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub struct Reading {
    pub worst: u32,
    pub plain: usize,
    pub lines: usize,
    pub chars: usize,
}

#[cfg(test)]
pub fn reading(function: &Function) -> Reading {
    reading_with(function, &Mask::default())
}

/// Inline assembly, which the permuter's parser refuses, stands in the
/// parsed text as placeholders the rewrites never touch: a statement as a
/// call `__humanize_asm_N()`, a register variable's `asm("rN")` as an array
/// bound `[__humanize_asm_N]`. `unmask` puts every one back verbatim.
#[derive(Default)]
pub struct Mask {
    pieces: Vec<String>,
}

const PLACEHOLDER: &str = "__humanize_asm_";

impl Mask {
    /// Preprocessor lines, which the parser's lexer drops, become comments
    /// `/*__humanize_pp_N_KIND*/` so a function keeps every edition's
    /// branch in place; statements inside a conditional are left as written.
    fn directives(text: &str, mask: &mut Mask) -> String {
        let mut out = String::new();
        let mut lines = text.split_inclusive('\n');
        while let Some(line) = lines.next() {
            let trimmed = line.trim_start();
            if !trimmed.starts_with('#') {
                out.push_str(line);
                continue;
            }
            let mut piece = line.to_string();
            while piece.trim_end_matches(['\n', '\r']).ends_with('\\') {
                match lines.next() {
                    Some(next) => piece.push_str(next),
                    None => break,
                }
            }
            let word: String = trimmed[1..].trim_start().chars().take_while(|c| c.is_ascii_alphabetic()).collect();
            let kind = match word.as_str() {
                "if" | "ifdef" | "ifndef" => "if",
                "else" | "elif" => "else",
                "endif" => "endif",
                _ => "other",
            };
            let newline = if piece.ends_with('\n') { "\n" } else { "" };
            let body = piece.strip_suffix('\n').unwrap_or(&piece).to_string();
            out.push_str(&format!("/*{PP}{}_{kind}*/{newline}", mask.pieces.len()));
            mask.pieces.push(body);
        }
        out
    }

    pub fn new(text: &str) -> (String, Mask) {
        let mut mask = Mask::default();
        let text = &Mask::directives(text, &mut mask);
        let bytes = text.as_bytes();
        let (mut out, mut i, mut last) = (String::new(), 0, 0);
        while i < bytes.len() {
            match bytes[i] {
                b'/' if bytes.get(i + 1) == Some(&b'*') => i = text[i + 2..].find("*/").map_or(bytes.len(), |end| i + 4 + end),
                b'/' if bytes.get(i + 1) == Some(&b'/') => i = text[i..].find('\n').map_or(bytes.len(), |end| i + end),
                b'"' | b'\'' => i = skip_quoted(bytes, i),
                c if c.is_ascii_alphabetic() || c == b'_' => {
                    let start = i;
                    while i < bytes.len() && (bytes[i].is_ascii_alphanumeric() || bytes[i] == b'_') {
                        i += 1;
                    }
                    if !matches!(&text[start..i], "asm" | "__asm" | "__asm__") {
                        continue;
                    }
                    let mut j = i;
                    loop {
                        while j < bytes.len() && bytes[j].is_ascii_whitespace() {
                            j += 1;
                        }
                        let word_end = (j..bytes.len()).find(|&k| !(bytes[k].is_ascii_alphanumeric() || bytes[k] == b'_')).unwrap_or(bytes.len());
                        if matches!(&text[j..word_end], "volatile" | "__volatile__" | "__volatile") {
                            j = word_end;
                        } else {
                            break;
                        }
                    }
                    if bytes.get(j) != Some(&b'(') {
                        continue;
                    }
                    let Some(close) = matching_paren(bytes, j) else { continue };
                    let before = text[..start].trim_end();
                    let declarator = before.ends_with(|c: char| c.is_ascii_alphanumeric() || c == '_');
                    let index = mask.pieces.len();
                    mask.pieces.push(text[start..=close].to_string());
                    out.push_str(&text[last..start]);
                    if declarator {
                        out.push_str(&format!("[{PLACEHOLDER}{index}]"));
                    } else {
                        out.push_str(&format!("{PLACEHOLDER}{index}()"));
                    }
                    i = close + 1;
                    last = i;
                }
                _ => i += 1,
            }
        }
        out.push_str(&text[last..]);
        (out, mask)
    }

    pub fn unmask(&self, text: &str) -> String {
        if self.pieces.is_empty() {
            return text.to_string();
        }
        let bound = regex::Regex::new(&format!(r"\s*\[\s*{PLACEHOLDER}(\d+)\s*\]")).unwrap();
        let call = regex::Regex::new(&format!(r"{PLACEHOLDER}(\d+)\s*\(\s*\)")).unwrap();
        let piece = |caps: &regex::Captures| self.pieces[caps[1].parse::<usize>().unwrap()].clone();
        let directive = regex::Regex::new(&format!(r"[ \t]*/\*{PP}(\d+)_[a-z]+\*/")).unwrap();
        let text = bound.replace_all(text, |caps: &regex::Captures| format!(" {}", piece(caps)));
        let text = call.replace_all(&text, |caps: &regex::Captures| piece(caps));
        directive.replace_all(&text, |caps: &regex::Captures| piece(caps)).into_owned()
    }
}

const PP: &str = "__humanize_pp_";

/// How a statement changes the depth of preprocessor conditionals.
fn directive_step(stmt: &Stmt) -> Option<i32> {
    let Stmt::Comment(text) = stmt else { return None };
    let at = text.find(PP)?;
    let rest = &text[at + PP.len()..];
    Some(if rest.contains("_if*/") {
        1
    } else if rest.contains("_endif*/") {
        -1
    } else {
        0
    })
}

fn skip_quoted(bytes: &[u8], start: usize) -> usize {
    let quote = bytes[start];
    let mut i = start + 1;
    while i < bytes.len() && bytes[i] != quote {
        i += if bytes[i] == b'\\' { 2 } else { 1 };
    }
    i + 1
}

fn matching_paren(bytes: &[u8], open: usize) -> Option<usize> {
    let (mut depth, mut i) = (0usize, open);
    while i < bytes.len() {
        match bytes[i] {
            b'"' | b'\'' => {
                i = skip_quoted(bytes, i);
                continue;
            }
            b'(' => depth += 1,
            b')' => {
                depth -= 1;
                if depth == 0 {
                    return Some(i);
                }
            }
            _ => {}
        }
        i += 1;
    }
    None
}

pub fn reading_with(function: &Function, mask: &Mask) -> Reading {
    let printed = mask.unmask(&function.print());
    let ranked = crate::worst::rank_source("HUMANIZE.C", &printed);
    let worst = ranked.iter().map(|r| r.score).sum();
    let magic = ranked.iter().flat_map(|r| r.signals.iter()).filter(|(name, _)| *name == "magic").map(|(_, n)| n).sum::<usize>();
    let mut narrow = 0;
    for stmt in &function.body {
        stmt.each(&mut |s| {
            if let Stmt::Decl(decl) = s {
                if decl.specs.iter().any(|t| ["u8", "s8", "u16", "s16"].contains(&t.as_str())) {
                    narrow += decl.items.len();
                }
            }
        });
    }
    Reading { worst, plain: magic + narrow, lines: printed.lines().count(), chars: printed.split_whitespace().map(str::len).sum() }
}

// ---------------------------------------------------------------- compile

struct Job {
    candidate: PathBuf,
    object: PathBuf,
    steps: Vec<Vec<String>>,
    reference: Vec<u8>,
}

impl Job {
    fn new(target: DecompTarget, file: &Path, directory: &Path, base: &str) -> Result<Job, String> {
        let stem = file.file_stem().unwrap_or_default().to_string_lossy().into_owned();
        let object = directory.join("obj").join(format!("{stem}.o"));
        std::fs::create_dir_all(object.parent().unwrap()).map_err(|e| format!("{}: {e}", directory.display()))?;
        let candidate = directory.join(file.file_name().unwrap_or_default());
        let mut steps = crate::diff::draft_steps(target, file, &object, false)?;
        let source = file.to_string_lossy().into_owned();
        let candidate_text = candidate.to_string_lossy().into_owned();
        let import_from = format!("-I{}", directory.display());
        let import_to = format!("-I{}", root().join(target.output_dir).display());
        let own = format!("-I{}", file.parent().unwrap_or(Path::new(".")).display());
        for step in &mut steps {
            let mut added = false;
            let mut out = Vec::with_capacity(step.len() + 1);
            for argument in step.drain(..) {
                let argument = if argument == import_from {
                    import_to.clone()
                } else if argument.contains(&source) {
                    argument.replace(&source, &candidate_text)
                } else {
                    argument
                };
                // Quoted includes beside the file still resolve from the copy.
                if !added && argument.starts_with("-I") {
                    out.push(own.clone());
                    added = true;
                }
                out.push(argument);
            }
            *step = out;
        }
        let mut job = Job { candidate, object, steps, reference: Vec::new() };
        job.reference = job.compile(base)?;
        Ok(job)
    }

    fn compile(&self, text: &str) -> Result<Vec<u8>, String> {
        std::fs::write(&self.candidate, text).map_err(|e| format!("{}: {e}", self.candidate.display()))?;
        let _ = std::fs::remove_file(&self.object);
        for step in &self.steps {
            crate::process::run(step, root())?;
        }
        std::fs::read(&self.object).map_err(|e| format!("{}: {e}", self.object.display()))
    }

    fn preprocess(&self) -> Result<String, String> {
        // As the permuter does: the compile step with -E for -S, when the
        // route has not already written the preprocessed unit.
        let preprocessed = self.object.with_extension("i");
        if !preprocessed.exists() {
            let assembly = self.object.with_extension("s").to_string_lossy().into_owned();
            let mut step = self.steps[0].clone();
            if !step.iter().any(|argument| argument == "-S") {
                return Err("no compile step to preprocess with".into());
            }
            for argument in step.iter_mut() {
                if argument == "-S" {
                    *argument = "-E".into();
                } else if *argument == assembly {
                    *argument = preprocessed.to_string_lossy().into_owned();
                }
            }
            crate::process::run(&step, root())?;
        }
        std::fs::read_to_string(&preprocessed).map_err(|e| format!("{}: {e}", preprocessed.display()))
    }

    /// The candidate keeps every byte of the object.
    fn same(&self, text: &str) -> bool {
        self.compile(text).is_ok_and(|bytes| bytes == self.reference)
    }
}

struct Scratch(PathBuf);

impl Drop for Scratch {
    fn drop(&mut self) {
        let _ = std::fs::remove_dir_all(&self.0);
    }
}

fn splice(text: &str, start: usize, end: usize, function: &Function) -> String {
    format!("{}{}{}", &text[..start], function.print().trim_end(), &text[end..])
}

/// Humanize one function of `text`; returns the new file text and the
/// readings before and after.
fn humanize_function(options: &Options, jobs: &[Job], unit: &Unit, text: &str, name: &str) -> Result<(String, Reading, Reading, Vec<Rewrite>), String> {
    let typedefs = unit.typedef_names();
    let (masked, mask) = Mask::new(text);
    let located = locate(&masked, Some(name), &typedefs).map_err(|e| format!("{name}: {e}"))?;
    let (start, end) = (located.start, located.end);
    let base = located.function.clone();
    let source = |function: &Function| mask.unmask(&splice(&masked, start, end, function));
    // The printer's own spelling must already keep the bytes.
    if !jobs[0].same(&source(&base)) {
        return Err(format!("{name}: the printed function does not compile to the same object; left as written"));
    }
    let reading = |function: &Function| reading_with(function, &mask);
    let before = reading(&base);
    let mut best = base.clone();
    let mut best_reading = before;
    let mut kept = Vec::new();
    let mut trail: Vec<Function> = Vec::new();
    let mut rng = Rng::new(options.seed);
    let deadline = Instant::now() + Duration::from_secs_f64(options.minutes * 60.0);
    'climb: while Instant::now() < deadline {
        let mut tries: Vec<(Rewrite, Function, Reading)> = Vec::new();
        for (kind, site) in candidates(&best, &mut rng) {
            let mut candidate = best.clone();
            apply(kind, &mut candidate, site);
            let r = reading(&candidate);
            if candidate != best && r < best_reading {
                tries.push((kind, candidate, r));
            }
        }
        if tries.is_empty() {
            break;
        }
        // J at a time; the first in seed order that keeps the bytes wins.
        for batch in tries.chunks(jobs.len()) {
            if Instant::now() >= deadline {
                break 'climb;
            }
            let verdicts: Vec<bool> = std::thread::scope(|scope| {
                let handles: Vec<_> = batch.iter().zip(jobs).map(|((_, candidate, _), job)| {
                    let candidate_text = source(candidate);
                    scope.spawn(move || job.same(&candidate_text))
                }).collect();
                handles.into_iter().map(|h| h.join().unwrap_or(false)).collect()
            });
            if let Some(at) = verdicts.iter().position(|v| *v) {
                let (kind, candidate, r) = &batch[at];
                best = candidate.clone();
                best_reading = *r;
                kept.push(*kind);
                trail.push(candidate.clone());
                continue 'climb;
            }
        }
        break;
    }
    if kept.is_empty() {
        return Ok((text.to_string(), before, before, kept));
    }
    // Carry the rewrites into the file's own lines one at a time, as the
    // permuter respells its best; a rewrite whose lines the printer spells
    // differently from the file is left out, and every text carried is
    // compiled again and must keep the bytes. The file keeps its layout.
    let (mut carried, mut previous, mut written) = (masked.clone(), splice(&masked, start, end, &base), Vec::new());
    for (kind, state) in kept.iter().zip(&trail) {
        let printed = splice(&masked, start, end, state);
        let next = super::transplant(&carried, &previous, &printed)
            .filter(|next| locate(next, Some(name), &typedefs).is_ok() && jobs[0].same(&mask.unmask(next)));
        if let Some(next) = next {
            carried = next;
            written.push(*kind);
        }
        previous = printed;
    }
    if written.len() < kept.len() {
        println!("{name}: {} of {} rewrites carry into the file's own lines; the rest are left out", written.len(), kept.len());
    }
    let after = locate(&carried, Some(name), &typedefs).map_or(before, |l| reading(&l.function));
    Ok((mask.unmask(&carried), before, after, written))
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let options = parse_options(arguments)?;
    let file = std::fs::canonicalize(&options.file).map_err(|e| format!("{}: {e}", options.file.display()))?;
    let mut text = std::fs::read_to_string(&file).map_err(|e| format!("{}: {e}", file.display()))?;
    let stem = file.file_stem().unwrap_or_default().to_string_lossy().into_owned();
    let scratch = Scratch(root().join("out/humanize").join(format!("{stem}-{}", std::process::id())));
    let mut jobs = Vec::new();
    for index in 0..options.jobs {
        jobs.push(Job::new(options.target, &file, &scratch.0.join(format!("job-{index:02}")), &text)?);
    }
    let unit = scan_unit(&jobs[0].preprocess()?)?;
    let names: Vec<String> = match &options.function {
        Some(name) => vec![name.clone()],
        None => crate::worst::rank_source("", &text).into_iter().map(|r| r.name).collect(),
    };
    let mut changed = false;
    for name in &names {
        match humanize_function(&options, &jobs, &unit, &text, name) {
            Ok((out, before, after, kept)) => {
                println!(
                    "{name}: worst {} -> {}, hex and narrow {} -> {}, lines {} -> {}; {} rewrite(s){}",
                    before.worst, after.worst, before.plain, after.plain, before.lines, after.lines, kept.len(),
                    if kept.is_empty() { String::new() } else { format!(": {}", kept.iter().map(|k| k.name()).collect::<Vec<_>>().join(", ")) }
                );
                if out != text {
                    // Later functions are judged against the new text.
                    text = out;
                    for job in &mut jobs {
                        job.reference = job.compile(&text)?;
                    }
                    changed = true;
                }
            }
            Err(error) => println!("{error}"),
        }
    }
    if changed && !options.dry_run {
        std::fs::write(&file, &text).map_err(|e| format!("{}: {e}", file.display()))?;
        println!("wrote {}; next: alchemy keep {}", options.file.display(), options.file.display());
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::permute::parse::locate;
    use std::collections::BTreeSet;

    fn parse(text: &str) -> Function {
        let typedefs: BTreeSet<String> = ["u8", "s8", "u16", "s16", "u32", "s32"].iter().map(|s| s.to_string()).collect();
        locate(text, None, &typedefs).unwrap().function
    }

    /// The search without a compiler: `same` stands in for the object check.
    fn climb(function: &Function, seed: u64, same: &dyn Fn(&Function) -> bool) -> (Function, Vec<Rewrite>) {
        let mut rng = Rng::new(seed);
        let (mut best, mut kept) = (function.clone(), Vec::new());
        'climb: loop {
            let current = reading(&best);
            for (kind, site) in candidates(&best, &mut rng) {
                let mut candidate = best.clone();
                apply(kind, &mut candidate, site);
                if candidate != best && reading(&candidate) < current && same(&candidate) {
                    best = candidate;
                    kept.push(kind);
                    continue 'climb;
                }
            }
            return (best, kept);
        }
    }

    const REMOVABLE: &str = "\
void Work_Step(struct Work *work)
{
    s32 t;
    t = (s32)work->count;
    Work_Set(work, t + 0x1c);
}
";

    #[test]
    fn a_removable_cast_and_temporary_go() {
        let function = parse(REMOVABLE);
        let (best, kept) = climb(&function, 1, &|_| true);
        let printed = best.print();
        assert!(!printed.contains("(s32)"), "{printed}");
        assert!(!printed.contains(" t"), "{printed}");
        assert!(printed.contains("Work_Set(work, work->count + 28);"), "{printed}");
        assert!(kept.contains(&Rewrite::ValueCast) && kept.contains(&Rewrite::FoldTemporary), "{kept:?}");
        assert!(reading(&best) < reading(&function));
    }

    #[test]
    fn a_load_bearing_cast_stays() {
        // Stand-in for the compiler: dropping the u8 truncation changes code.
        let function = parse(
            "\
void Work_Low(struct Work *work)
{
    work->low = (u8)work->value;
}
",
        );
        let (best, kept) = climb(&function, 1, &|candidate| candidate.print().contains("(u8)"));
        assert!(best.print().contains("(u8)work->value"), "{}", best.print());
        assert!(kept.is_empty());
    }

    #[test]
    fn fakematch_statements_are_left_alone() {
        let function = parse(
            "\
void Work_Pin(struct Work *work)
{
    /* FAKEMATCH: the native keeps the cast. */
    work->low = (u8)work->value;
    work->high = (u8)work->other;
}
",
        );
        let (best, _) = climb(&function, 3, &|_| true);
        let printed = best.print();
        assert!(printed.contains("work->low = (u8)work->value;"), "{printed}");
        assert!(printed.contains("work->high = work->other;"), "{printed}");
    }

    #[test]
    fn inline_assembly_is_masked_kept_and_restored() {
        let text = "\
void Work_Asm(struct Work *work)
{
    /* FAKEMATCH: r4 as the native allocates it. */
    register u32 v asm(\"r4\");
    v = (u32)work->a;
    asm volatile(\"\" : : \"r\"(v));
    work->b = (u16)work->c;
#if EDITION_INTERNATIONAL
    work->d = (u16)work->e;
#else
    work->d = (u8)work->e;
#endif
}
";
        let (masked, mask) = Mask::new(text);
        assert!(!masked.contains("asm(") && !masked.contains("asm volatile"), "{masked}");
        assert_eq!(mask.unmask(&masked), text);
        let function = parse(&masked);
        let (best, _) = climb(&function, 1, &|_| true);
        let printed = mask.unmask(&best.print());
        assert!(printed.contains("register u32 v asm(\"r4\");"), "{printed}");
        assert!(printed.contains("asm volatile(\"\" : : \"r\"(v));"), "{printed}");
        assert!(printed.contains("work->b = work->c;"), "{printed}");
        // Each edition's branch stays in place and as written.
        assert!(printed.contains("\n#if EDITION_INTERNATIONAL\n    work->d = (u16)work->e;\n#else\n    work->d = (u8)work->e;\n#endif\n"), "{printed}");
    }

    #[test]
    fn gotos_become_an_if_and_a_do_while() {
        let function = parse(
            "\
void Work_Jump(struct Work *work)
{
    if (work->a == 0)
        goto skip;
    work->b = 1;
skip:
again:
    work->c++;
    if (work->c < 4)
        goto again;
}
",
        );
        let (best, kept) = climb(&function, 1, &|_| true);
        let printed = best.print();
        assert!(!printed.contains("goto"), "{printed}");
        assert!(printed.contains("if (work->a != 0) {"), "{printed}");
        assert!(printed.contains("} while (work->c < 4);"), "{printed}");
        assert_eq!(kept.iter().filter(|k| **k == Rewrite::Goto).count(), 2, "{kept:?}");
    }

    #[test]
    fn a_seed_repeats_exactly() {
        let text = "\
void Work_Many(struct Work *work)
{
    u8 i;
    s32 t;
    i = 0;
    do {
        work->a = (u16)(work->b + 0x40);
        work->c = !(work->d == 0x123);
    } while (0);
    t = (s32)work->e;
    work->f = work->f + t;
    i += 1;
    REG_BLDALPHA = 0x1010;
    work->y = work->y + 0x500000;
}
";
        let function = parse(text);
        let run = |seed| climb(&function, seed, &|_| true);
        let (a, ka) = run(7);
        let (b, kb) = run(7);
        assert_eq!(a, b);
        assert_eq!(ka, kb);
        let mut rng = Rng::new(7);
        let first = candidates(&function, &mut rng);
        let mut rng = Rng::new(7);
        assert_eq!(first, candidates(&function, &mut rng));
        let printed = a.print();
        assert!(!printed.contains("while (0)"), "{printed}");
        assert!(printed.contains("work->d != 291"), "{printed}");
        assert!(printed.contains("i++;"), "{printed}");
        // Register fields and 16.16 fixed point keep their hex.
        assert!(printed.contains("REG_BLDALPHA = 0x1010;"), "{printed}");
        assert!(printed.contains("work->y += 0x500000;"), "{printed}");
    }
}
