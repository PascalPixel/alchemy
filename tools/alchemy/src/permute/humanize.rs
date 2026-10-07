//! `alchemy humanize`: the permuter's search with its objective turned
//! around. For a function that already matches, it tries readability
//! rewrites and keeps one only when the whole object compiles to exactly the
//! same bytes as the current spelling (code, literal pools, data, relocations
//! and symbols) with the same compiler warnings in every edition that
//! compiles the file, and `alchemy worst`'s score of the function falls. No
//! listing and nothing from the ROM is read: the current source's own
//! objects are the reference. Number bases are `alchemy camelotize
//! numbers`'s, never this command's.
//! FAKEMATCH-tagged statements, asm, volatile and register variables are
//! never touched. The file is written once, at the end, in its own lines
//! where the rewrites carry into them; then a bare `alchemy keep` judges all
//! twelve editions.

use super::ast::{BinOp, Decl, Expr, Function, Init, Stmt, UnOp};
use super::mutate::Rng;
use super::parse::{locate, scan_unit, Unit};
use super::types::Env;
use crate::compiler::routing::root;
use crate::targets::{decomp_target, DecompTarget};
use std::path::{Path, PathBuf};
use std::time::{Duration, Instant};

const USAGE: &str = r#"usage: alchemy humanize FILE.C [--function NAME] [--minutes N] [--jobs J] [--seed S] [--dry-run]
Rewrites a matching function toward plainer C without changing one byte of
its object: each candidate is one rewrite of the current best, compiled with
alchemy diff's steps for FILE.C in every edition of every game that compiles
it (editions whose preprocessed file is the same are compiled once), and
kept only when each object file is byte-identical to the current
spelling's, the compiler's warnings are the same (sorted, without line
numbers), and alchemy worst's score of the function falls (then narrow
locals, lines and characters break ties). An integer cast whose operand is
a string, an address, or declared a pointer, array or function stays,
whatever the callee's prototype, decided by the operand's declared type in
the preprocessed file (an unknown type stays too). Rewrites: drop any
other value cast or a pointer cast, fold a
single-use temporary into its use, merge a
declaration with its first assignment, widen a u8, s8, u16 or s16 local to
s32, compound assignment, ++ and --, a negated comparison, a
do { } while (0) block back to plain statements, a goto over statements to
an if, a goto back to its label to a do-while. Numbers keep their base:
that is alchemy camelotize numbers. Left as written: tagged
code as alchemy camelotize defines it for both commands (the statement a
FAKEMATCH tag stands before or ends the line of, the whole block whose first
line carries one, every statement holding asm), any statement with volatile
or register, statements inside #if, #define bodies, and a function defined
more than once across #if branches.
Candidates are tried in an order drawn from --seed, J at a time; the first
in that order that passes is kept, so a seed repeats exactly. Without
--function every function in FILE.C is humanized in turn. FILE.C is written
once at the end (not with --dry-run); run a bare alchemy keep next, which
checks all twelve editions.
Defaults: --minutes 5 per function, --jobs cores - 2, --seed 1."#;

struct Options {
    file: PathBuf,
    function: Option<String>,
    minutes: f64,
    jobs: usize,
    seed: u64,
    dry_run: bool,
}

fn parse_options(arguments: &[String]) -> Result<Options, String> {
    let (mut file, mut function) = (None, None);
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
            "--dry-run" => dry_run = true,
            other if other.starts_with('-') => return Err(format!("unknown humanize option: {other}\n{USAGE}")),
            other if file.is_none() => file = Some(PathBuf::from(other)),
            _ => return Err(USAGE.into()),
        }
        index += 1;
    }
    let file = file.ok_or_else(|| USAGE.to_string())?;
    Ok(Options { file, function, minutes, jobs, seed, dry_run })
}

/// Every edition that may compile `file`: all six of its game's, both
/// games' twelve for shared or unplaced files.
fn editions_for(file: &Path) -> Vec<DecompTarget> {
    let text = file.to_string_lossy();
    let game = |id: &str| decomp_target(Some(id)).map(|t| t.editions().to_vec()).unwrap_or_default();
    if text.contains("THE BROKEN SEAL") || text.contains("/tbs/") {
        game("tbs-ja")
    } else if text.contains("THE LOST AGE") || text.contains("/tla/") {
        game("tla-ja")
    } else {
        [game("tbs-ja"), game("tla-ja")].concat()
    }
}

/// How many top-level definitions of `name` the text holds; each edition's
/// #if branch counts.
fn definitions(text: &str, name: &str) -> usize {
    super::lex::lex(text).map_or(0, |tokens| super::parse::scan_definitions(&tokens).iter().filter(|entry| entry.0 == name).count())
}

fn next_step(file: &Path) -> String {
    format!("wrote {}; next: a bare alchemy keep, which checks all twelve editions", file.display())
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
}

impl Rewrite {
    pub const ALL: [Rewrite; 10] = [
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

/// A statement the rewrites leave alone: anything that names asm, volatile
/// or register. FAKEMATCH tags are `camelotize`'s tagged ranges, marked by
/// `mark_tagged`.
fn guarded(stmt: &Stmt) -> bool {
    let mut found = false;
    stmt.each(&mut |inner| {
        if let Stmt::Decl(decl) = inner {
            if decl.specs.iter().any(|s| s == "volatile" || s == "register") || decl.items.iter().any(|item| item.before.iter().chain(&item.after).any(|t| t == "volatile" || t.contains(PLACEHOLDER))) {
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

/// Tagged code, as `camelotize`'s tagged module defines it for both commands:
/// `Mask::new` puts `TAGGED` before the first statement of each range, or
/// `TAGGED_ALL` inside the opening brace of a definition a range covers
/// whole, and `unmask` takes them out again.
const TAGGED: &str = "/*__humanize_tagged__*/";
const TAGGED_ALL: &str = "/*__humanize_tagged_all__*/";

/// Which statements of a block are open to rewriting: not guarded, not
/// inside an edition's #if, not a statement `TAGGED` stands before (with the
/// labels in front of it), and nothing in a block that holds `TAGGED_ALL`.
fn open_mask(stmts: &[Stmt]) -> Vec<bool> {
    let mut mask = vec![true; stmts.len()];
    let (mut leading, mut depth) = (false, 0);
    for (i, stmt) in stmts.iter().enumerate() {
        if let Some(step) = directive_step(stmt) {
            depth += step;
            mask[i] = false;
            continue;
        }
        if depth > 0 {
            // Inside an edition's #if: only one branch compiles here.
            mask[i] = false;
            continue;
        }
        match stmt {
            Stmt::Comment(text) => {
                mask[i] = false;
                if text.contains(TAGGED_ALL) {
                    return vec![false; stmts.len()];
                }
                leading |= text.contains(TAGGED);
            }
            Stmt::Label(_) | Stmt::Case(_) | Stmt::Default if leading => mask[i] = false,
            other => {
                if leading || guarded(other) {
                    mask[i] = false;
                }
                leading = false;
            }
        }
    }
    mask
}

/// Marks the ranges `camelotize::tagged` protects in `text`: `TAGGED` at the
/// first statement of each, or `TAGGED_ALL` after the opening brace of a
/// top-level definition a range starts.
fn mark_tagged(text: &str) -> String {
    use super::lex::Tok;
    let Ok(tokens) = super::lex::lex(text) else { return text.to_string() };
    let mut inserts: Vec<(usize, &str)> = Vec::new();
    for range in crate::camelotize::tagged::ranges(text) {
        let depth: i32 = tokens
            .iter()
            .take_while(|t| t.start < range.start)
            .map(|t| match t.tok {
                Tok::Punct("{") => 1,
                Tok::Punct("}") => -1,
                _ => 0,
            })
            .sum();
        if depth > 0 {
            inserts.push((range.start, TAGGED));
        } else if let Some(open) = tokens.iter().find(|t| t.start >= range.start && t.start < range.end && matches!(t.tok, Tok::Punct("{"))) {
            inserts.push((open.end, TAGGED_ALL));
        }
    }
    inserts.sort();
    inserts.dedup();
    let mut out = text.to_string();
    for (at, marker) in inserts.into_iter().rev() {
        let spaced = if marker == TAGGED { format!("{marker} ") } else { marker.to_string() };
        out.insert_str(at, &spaced);
    }
    out
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
fn walk_exprs(stmts: &mut [Stmt], visit: &mut dyn FnMut(&mut Expr) -> bool, cursor: &Cursor) {
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

fn walk_expr(expr: &mut Expr, visit: &mut dyn FnMut(&mut Expr) -> bool, cursor: &Cursor) {
    if cursor.done() {
        return;
    }
    if !visit(expr) {
        return;
    }
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

fn expr_rewrite(kind: Rewrite, expr: &mut Expr, cursor: &mut Cursor, types: Option<&Env>) {
    match kind {
        Rewrite::ValueCast | Rewrite::PointerCast => {
            if let Expr::Cast(ty, inner) = expr {
                let pointer = ty.tokens.iter().any(|t| t == "*");
                // An address, string, array or function made a number keeps its
                // cast, decided by the operand's declared type (Env::number_of_address).
                let address = match types {
                    Some(env) => env.number_of_address(inner),
                    None => matches!(**inner, Expr::Unary(UnOp::AddrOf, _) | Expr::Str(_)),
                };
                let fits = if kind == Rewrite::PointerCast { pointer } else { !pointer && !address && is_value_type(&ty.tokens) };
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

/// Remove the declarator of `name` wherever an open statement declares it
/// without an initializer; drop its declaration when nothing is left in it.
/// A declaration a FAKEMATCH tag or a guard protects is never touched: the
/// result is false, and the caller must not fold.
fn remove_declarator(stmts: &mut Vec<Stmt>, name: &str) -> bool {
    let mask = open_mask(stmts);
    for i in 0..stmts.len() {
        if let Stmt::Decl(decl) = &mut stmts[i] {
            if let Some(at) = decl.items.iter().position(|item| item.name == name) {
                if !mask[i] || decl.items[at].init.is_some() {
                    return false;
                }
                decl.items.remove(at);
                if decl.items.is_empty() {
                    stmts.remove(i);
                }
                return true;
            }
        }
    }
    for (stmt, open) in stmts.iter_mut().zip(mask) {
        for block in nested_mut(stmt) {
            if declares(block, name) {
                return open && remove_declarator(block, name);
            }
        }
    }
    false
}

fn declares(stmts: &[Stmt], name: &str) -> bool {
    stmts.iter().any(|stmt| {
        let mut found = false;
        stmt.each(&mut |s| found |= matches!(s, Stmt::Decl(decl) if decl.items.iter().any(|item| item.name == name)));
        found
    })
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
                // The temporary's declaration must go with it, and may be
                // one a FAKEMATCH tag protects.
                if !from_decl && declares(body, &name) && !remove_declarator(&mut body.to_vec(), &name) {
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
pub fn apply(kind: Rewrite, function: &mut Function, target: usize, types: Option<&Env>) -> usize {
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
                    expr_rewrite(kind, expr, &mut cursor, types);
                }
                true
            },
            &Cursor { target: usize::MAX, seen: 0 },
        );
    }
    cursor.seen
}

/// Every (rewrite, site) of a function, in an order drawn from `rng`.
pub fn candidates(function: &Function, types: Option<&Env>, rng: &mut Rng) -> Vec<(Rewrite, usize)> {
    let mut all = Vec::new();
    for kind in Rewrite::ALL {
        let sites = apply(kind, &mut function.clone(), usize::MAX, types);
        all.extend((0..sites).map(|site| (kind, site)));
    }
    for i in (1..all.len()).rev() {
        let j = rng.below(i + 1);
        all.swap(i, j);
    }
    all
}

// ---------------------------------------------------------------- objective

/// How mechanical a spelling reads: alchemy worst's score first, then
/// narrow locals, then lines and characters.
#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub struct Reading {
    pub worst: u32,
    pub narrow: usize,
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
        let text = &mark_tagged(&Mask::directives(text, &mut mask));
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
        // A marker the printer put on its own line goes with its line; one
        // still where `mark_tagged` put it goes with the space it added.
        let lines = regex::Regex::new(r"(?m)^[ \t]*/\*__humanize_tagged(_all)?__\*/[ \t]*\n").unwrap();
        let untagged = lines.replace_all(text, "").replace(&format!("{TAGGED} "), "").replace(TAGGED, "").replace(TAGGED_ALL, "");
        let text = untagged.as_str();
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
    Reading { worst, narrow, lines: printed.lines().count(), chars: printed.split_whitespace().map(str::len).sum() }
}

// ---------------------------------------------------------------- compile

/// What one compile shows the judge: the object's bytes, and the compiler's
/// and assembler's messages sorted and without line numbers, as camelotize's
/// trials compare them. A rewrite that keeps the bytes but adds or changes
/// a warning (an integer made from a pointer, an array or a function
/// without its cast, which C89 asks for) is refused.
#[derive(Clone, Debug, Default, PartialEq, Eq)]
struct Compiled {
    bytes: Vec<u8>,
    warnings: String,
}

struct Job {
    candidate: PathBuf,
    object: PathBuf,
    steps: Vec<Vec<String>>,
    reference: Compiled,
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
        let mut job = Job { candidate, object, steps, reference: Compiled::default() };
        job.reference = job.compile(base)?;
        Ok(job)
    }

    fn compile(&self, text: &str) -> Result<Compiled, String> {
        std::fs::write(&self.candidate, text).map_err(|e| format!("{}: {e}", self.candidate.display()))?;
        let _ = std::fs::remove_file(&self.object);
        let mut messages = String::new();
        for step in &self.steps {
            messages.push_str(&crate::process::run_with_messages(step, root())?.1);
        }
        let bytes = std::fs::read(&self.object).map_err(|e| format!("{}: {e}", self.object.display()))?;
        Ok(Compiled { bytes, warnings: crate::camelotize::trial::unnumbered(&messages) })
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

    /// The candidate keeps every byte of the object and every warning.
    fn same(&self, text: &str) -> bool {
        self.compile(text).is_ok_and(|compiled| compiled == self.reference)
    }
}

/// One worker's judge: the file compiled for each distinct edition build, so
/// a function only one edition compiles (inside its #if) is checked there.
struct Check {
    jobs: Vec<Job>,
}

impl Check {
    fn same(&self, text: &str) -> bool {
        self.jobs.iter().all(|job| job.same(text))
    }

    fn rebase(&mut self, text: &str) -> Result<(), String> {
        for job in &mut self.jobs {
            job.reference = job.compile(text)?;
        }
        Ok(())
    }
}

/// The editions whose builds of `file` differ: each edition that compiles
/// it, less those whose preprocessed file and compiler repeat an earlier
/// one's (the same unit compiles to the same object, whatever the rewrite).
fn distinct_editions(file: &Path, directory: &Path, text: &str) -> Result<(Vec<DecompTarget>, String), String> {
    let (mut kept, mut seen, mut first) = (Vec::new(), Vec::new(), None);
    for target in editions_for(file) {
        // An edition that cannot compile the file cannot judge a rewrite:
        // nothing is humanized rather than leave it unchecked.
        let job = Job::new(target, file, &directory.join(target.id.as_str()), text).map_err(|error| format!("{}: {}: {error}; left as written", file.display(), target.id))?;
        let unit = job.preprocess()?;
        let key: String = unit.lines().filter(|line| !line.starts_with('#')).collect::<Vec<_>>().join("\n");
        first.get_or_insert_with(|| unit.clone());
        let key = (target.compiler, key);
        if !seen.contains(&key) {
            seen.push(key);
            kept.push(target);
        }
    }
    let unit = first.ok_or_else(|| format!("{}: no edition compiles it", file.display()))?;
    Ok((kept, unit))
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
/// Whether the text that changes from `before` to `after` lies outside
/// every range `camelotize`'s tagged module protects in `before`: the one
/// definition of tagged code (a tag's statement, or the whole block a tag on
/// its first line opens, and every statement holding asm), read in the
/// file's own lines.
fn outside_tagged(before: &str, after: &str) -> bool {
    let (a, b) = (before.as_bytes(), after.as_bytes());
    let prefix = a.iter().zip(b).take_while(|(x, y)| x == y).count();
    let suffix = a[prefix..].iter().rev().zip(b[prefix..].iter().rev()).take_while(|(x, y)| x == y).count();
    let (start, end) = (prefix, a.len() - suffix);
    !crate::camelotize::tagged::covered(&crate::camelotize::tagged::ranges(before), start, end)
}

fn humanize_function(options: &Options, jobs: &[Check], unit: &Unit, text: &str, name: &str) -> Result<(String, Reading, Reading, Vec<Rewrite>), String> {
    let typedefs = unit.typedef_names();
    let (masked, mask) = Mask::new(text);
    if definitions(&masked, name) > 1 {
        return Err(format!("{name}: defined more than once across #if branches; left as written"));
    }
    let located = locate(&masked, Some(name), &typedefs).map_err(|e| format!("{name}: {e}"))?;
    let (start, end) = (located.start, located.end);
    let base = located.function.clone();
    let source = |function: &Function| mask.unmask(&splice(&masked, start, end, function));
    // The printer's own spelling must already keep the bytes.
    if !jobs[0].same(&source(&base)) {
        return Err(format!("{name}: the printed function does not compile to the same object and warnings; left as written"));
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
        let types = Env::new(unit, &best);
        for (kind, site) in candidates(&best, Some(&types), &mut rng) {
            let mut candidate = best.clone();
            apply(kind, &mut candidate, site, Some(&types));
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
            .filter(|next| outside_tagged(&mask.unmask(&carried), &mask.unmask(next)))
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
    let (editions, preprocessed) = distinct_editions(&file, &scratch.0.join("editions"), &text)?;
    println!(
        "{}: judged in {} ({} distinct edition build(s))",
        options.file.display(),
        editions.iter().map(|t| t.id.as_str()).collect::<Vec<_>>().join(", "),
        editions.len()
    );
    let mut jobs = Vec::new();
    for index in 0..options.jobs {
        let mut check = Check { jobs: Vec::new() };
        for target in &editions {
            check.jobs.push(Job::new(*target, &file, &scratch.0.join(format!("job-{index:02}-{}", target.id)), &text)?);
        }
        jobs.push(check);
    }
    let unit = scan_unit(&preprocessed)?;
    let mut names: Vec<String> = match &options.function {
        Some(name) => vec![name.clone()],
        None => crate::worst::rank_source("", &text).into_iter().map(|r| r.name).collect(),
    };
    let mut seen = std::collections::BTreeSet::new();
    names.retain(|name| seen.insert(name.clone()));
    let mut changed = false;
    for name in &names {
        match humanize_function(&options, &jobs, &unit, &text, name) {
            Ok((out, before, after, kept)) => {
                println!(
                    "{name}: worst {} -> {}, narrow locals {} -> {}, lines {} -> {}; {} rewrite(s){}",
                    before.worst, after.worst, before.narrow, after.narrow, before.lines, after.lines, kept.len(),
                    if kept.is_empty() { String::new() } else { format!(": {}", kept.iter().map(|k| k.name()).collect::<Vec<_>>().join(", ")) }
                );
                if out != text {
                    // Later functions are judged against the new text.
                    text = out;
                    for check in &mut jobs {
                        check.rebase(&text)?;
                    }
                    changed = true;
                }
            }
            Err(error) => println!("{error}"),
        }
    }
    if changed && !options.dry_run {
        std::fs::write(&file, &text).map_err(|e| format!("{}: {e}", file.display()))?;
        println!("{}", next_step(&options.file));
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::permute::parse::locate;
    use std::collections::BTreeSet;

    fn typedefs() -> BTreeSet<String> {
        ["u8", "s8", "u16", "s16", "u32", "s32"].iter().map(|s| s.to_string()).collect()
    }

    fn parse(text: &str) -> Function {
        locate(text, None, &typedefs()).unwrap().function
    }

    /// The search without a compiler: `same` stands in for the object check.
    fn climb(function: &Function, seed: u64, same: &dyn Fn(&Function) -> bool) -> (Function, Vec<Rewrite>) {
        let mut rng = Rng::new(seed);
        let (mut best, mut kept) = (function.clone(), Vec::new());
        'climb: loop {
            let current = reading(&best);
            for (kind, site) in candidates(&best, None, &mut rng) {
                let mut candidate = best.clone();
                apply(kind, &mut candidate, site, None);
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

    /// Reviewers' lines (FUNE_HEYA/ROWING.C, TOREBI_IZUMI/PLAY.C): the
    /// callees are declared with empty parentheses, so GCC says nothing
    /// when the cast goes; the operand's declared type keeps it.
    #[test]
    fn integer_casts_of_addresses_stay_by_declared_type() {
        let unit = scan_unit(
            "typedef int s32;\ntypedef unsigned char u8;\nextern const u8 MsgFuneWhereGoingSlay[];\nextern const u8 *MsgTorebiStepRightTry;\nextern s32 FuneHeya_ProgressTableA();\nvoid UiText_ShowCenteredMessage();\n",
        )
        .unwrap();
        let text = "void F(void)\n{\n    u8 n;\n    UiText_ShowCenteredMessage((s32)MsgFuneWhereGoingSlay, 1, 8);\n    G(3, (s32)FuneHeya_ProgressTableA, (s32)MsgTorebiStepRightTry);\n    G((s32)\"s\", (s32)&n, (s32)Unknown, (s32)n);\n}\n";
        let function = parse(text);
        let env = Env::new(&unit, &function);
        assert_eq!(apply(Rewrite::ValueCast, &mut function.clone(), usize::MAX, Some(&env)), 1);
        let mut dropped = function.clone();
        apply(Rewrite::ValueCast, &mut dropped, 0, Some(&env));
        assert_ne!(dropped, function);
    }

    /// One definition of tagged code: camelotize's, read in the file's own
    /// lines. A tag on a block's first line covers the block.
    #[test]
    fn carried_rewrites_stay_outside_tagged_code() {
        let before = "void F(void)\n{\n    a = (u8)b;\n    if (c) {\n        /* FAKEMATCH: order */\n        d = (u8)e;\n        f = (u8)g;\n    }\n    h = (u8)i; /* FAKEMATCH: r4 */\n}\n";
        assert!(outside_tagged(before, &before.replacen("a = (u8)b", "a = b", 1)));
        assert!(!outside_tagged(before, &before.replacen("f = (u8)g", "f = g", 1)));
        assert!(!outside_tagged(before, &before.replacen("d = (u8)e", "d = e", 1)));
        assert!(!outside_tagged(before, &before.replacen("h = (u8)i", "h = i", 1)));
    }

    #[test]
    fn a_removable_cast_and_temporary_go() {
        let function = parse(REMOVABLE);
        let (best, kept) = climb(&function, 1, &|_| true);
        let printed = best.print();
        assert!(!printed.contains("(s32)"), "{printed}");
        assert!(!printed.contains(" t"), "{printed}");
        assert!(printed.contains("Work_Set(work, work->count + 0x1c);"), "{printed}");
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

    /// The whole path a file takes: masked, parsed, climbed with every
    /// rewrite accepted, printed and unmasked.
    fn humanized(text: &str, seed: u64) -> String {
        let (masked, mask) = Mask::new(text);
        let (best, _) = climb(&parse(&masked), seed, &|_| true);
        mask.unmask(&best.print())
    }

    #[test]
    fn a_trailing_fakematch_tag_protects_its_own_statement() {
        // Reviewers' probe: the tagged line lost its cast and the next,
        // untagged line was shielded instead.
        for seed in 1..6 {
            let printed = humanized(
                "\
void Work_Pin(struct Work *work)
{
    work->low = (u8)work->value; /* FAKEMATCH: the native keeps the cast. */
    work->high = (u8)work->other;
}
",
                seed,
            );
            assert!(printed.contains("work->low = (u8)work->value;"), "{printed}");
            assert!(printed.contains("work->high = work->other;"), "{printed}");
            assert!(!printed.contains("__humanize"), "{printed}");
        }
    }

    #[test]
    fn a_trailing_fakematch_tag_keeps_a_one_pass_block() {
        // Reviewers' probe: the tagged do { } while (0) was unwrapped.
        for seed in 1..6 {
            let printed = humanized(
                "\
void Work_Once(struct Work *work)
{
    do {
        work->a = 1;
    } while (0); /* FAKEMATCH: the block orders the store. */
    work->b = (u8)work->c;
}
",
                seed,
            );
            assert!(printed.contains("} while (0);"), "{printed}");
            assert!(printed.contains("work->b = work->c;"), "{printed}");
        }
    }

    #[test]
    fn a_tag_on_a_block_opening_or_inside_a_statement_protects_it() {
        for seed in 1..6 {
            let printed = humanized(
                "\
void Work_Block(struct Work *work)
{
    if (work->a) { /* FAKEMATCH: the whole branch. */
        work->b = (u8)work->c;
        work->d = (u8)work->e;
    }
    Work_Set(work, /* FAKEMATCH: the argument's cast. */
             (u8)work->f);
    if (work->x) {
        work->y = (u8)work->z;
    } /* FAKEMATCH: the branch. */
    else {
        work->w = (u8)work->v;
    }
    work->g = (u8)work->h;
}
",
                seed,
            );
            assert!(printed.contains("work->b = (u8)work->c;") && printed.contains("work->d = (u8)work->e;"), "{printed}");
            assert!(printed.contains("(u8)work->f"), "{printed}");
            assert!(printed.contains("work->y = (u8)work->z;") && printed.contains("work->w = (u8)work->v;"), "{printed}");
            assert!(printed.contains("work->g = work->h;"), "{printed}");
        }
    }

    #[test]
    fn a_fold_never_removes_a_protected_declaration() {
        // remove_declarator ignored the mask and took a tagged local.
        for text in [
            "\
void Work_Frame(struct Work *work)
{
    s32 t; /* FAKEMATCH: the native frame. */
    t = work->a;
    work->b = t;
}
",
            "\
void Work_Frame(struct Work *work)
{
    /* FAKEMATCH: the native frame. */
    s32 t;
    t = work->a;
    work->b = t;
}
",
        ] {
            for seed in 1..6 {
                let printed = humanized(text, seed);
                assert!(printed.contains("s32 t;"), "{printed}");
                assert!(printed.contains("t = work->a;") && printed.contains("work->b = t;"), "{printed}");
            }
        }
    }

    #[test]
    fn numbers_keep_their_base() {
        // Reviewers' probes: humanize decimalized mask locals
        // (SCALE_RGB555_BUFFER.C mask_green = 0x3e0 beside mask_blue =
        // 0x7c00, OBJECT.C offset_mask = 0x3E, CONTEXT.C flag = 0x121).
        // Number bases are alchemy camelotize numbers' alone.
        for seed in 1..6 {
            let printed = humanized(
                "\
void Work_Flags(struct Work *work)
{
    u32 mask_green = 0x3e0;
    u32 mask_blue = 0x7c00;
    s32 offset_mask = 0x3E;
    s32 flag;
    flag = 0x121;
    FLAG_SET(flag);
    work->a = work->b + 0x1234;
    work->c = work->d + 0x123;
    work->e = (work->f & mask_green) | (work->g & mask_blue) | offset_mask;
#define LIMIT 0x123
    work->h = LIMIT;
}
",
                seed,
            );
            for spelling in ["0x3e0", "0x7c00", "0x3E", "0x121", "0x1234", "0x123;", "\n#define LIMIT 0x123\n"] {
                assert!(printed.contains(spelling), "{spelling}: {printed}");
            }
            assert!(!Rewrite::ALL.iter().any(|kind| kind.name().contains("hex")));
        }
    }

    /// A probe file the Broken Seal's English build compiles as game C.
    fn probe_check(directory: &Path, text: &str) -> Option<Job> {
        let target = decomp_target(Some("tbs-en")).ok()?;
        std::fs::create_dir_all(directory).ok()?;
        let file = directory.join("PROBE.C");
        std::fs::write(&file, text).ok()?;
        match Job::new(target, &file, &directory.join("job"), text) {
            Ok(job) => Some(job),
            Err(error) => {
                // No pinned compiler in this checkout: nothing can judge.
                eprintln!("humanize probe skipped: {error}");
                None
            }
        }
    }

    #[test]
    fn a_cast_c89_needs_is_kept_by_its_warning() {
        // Reviewers' probes from PLAY.C: s32 message = MsgTorebiStepRightTry;
        // and Engine_EventSetMessage((s32)Msg...) lost their casts: the
        // bytes stay, but GCC warns of an integer made from a pointer.
        // The same holds for a pointer local and a function's address.
        let header = "\
typedef int s32;
typedef unsigned char u8;
struct Work { s32 a; };
extern const u8 MsgTorebiStepRightTry[];
extern const u8 MsgTorebiGetRidItems[];
void Engine_EventSetMessage(s32 message);
void Work_Callback(void);
void Work_Take(s32 value);
";
        let probes = [
            ("void Probe(void)\n{\n    s32 message = (s32)MsgTorebiStepRightTry;\n    Engine_EventSetMessage(message);\n}\n", "(s32)MsgTorebiStepRightTry"),
            ("void Probe(void)\n{\n    Engine_EventSetMessage((s32)MsgTorebiGetRidItems);\n}\n", "(s32)MsgTorebiGetRidItems"),
            ("void Probe(struct Work *work)\n{\n    Work_Take((s32)work);\n}\n", "(s32)work"),
            ("void Probe(void)\n{\n    Work_Take((s32)Work_Callback);\n}\n", "(s32)Work_Callback"),
        ];
        let scratch = Scratch(root().join("out/humanize").join(format!("probe-{}", std::process::id())));
        for (index, (body, cast)) in probes.iter().enumerate() {
            let text = format!("{header}{body}");
            let Some(job) = probe_check(&scratch.0.join(index.to_string()), &text) else { return };
            // Every value cast rewrite the search would try drops this cast.
            let (masked, mask) = Mask::new(&text);
            let function = parse(&masked);
            let sites = apply(Rewrite::ValueCast, &mut function.clone(), usize::MAX, None);
            assert_eq!(sites, 1, "{body}");
            let mut dropped = function.clone();
            apply(Rewrite::ValueCast, &mut dropped, 0, None);
            let located = locate(&masked, Some("Probe"), &typedefs()).unwrap();
            let candidate = mask.unmask(&splice(&masked, located.start, located.end, &dropped));
            assert!(!candidate.contains(cast), "{candidate}");
            let compiled = job.compile(&candidate).expect("the candidate compiles");
            assert_eq!(compiled.bytes, job.reference.bytes, "{cast}: the bytes alone would pass it");
            assert_ne!(compiled.warnings, job.reference.warnings, "{cast}: a new warning");
            assert!(!job.same(&candidate), "{cast}: the judge refuses it");
            assert!(job.same(&text), "the current spelling passes");
        }
    }

    #[test]
    fn warnings_compare_without_line_numbers() {
        let at = |line: u32, what: &str| Compiled { bytes: vec![1, 2], warnings: crate::camelotize::trial::unnumbered(&format!("P.C: In function `Probe':\nP.C:{line}: warning: {what}\n")) };
        assert_eq!(at(9, "unused variable `t'"), at(8, "unused variable `t'"));
        assert_ne!(at(9, "unused variable `t'"), at(9, "initialization makes integer from pointer without a cast"));
    }

    #[test]
    fn an_address_made_a_number_keeps_its_cast() {
        // Demonstration on PLAY.C: (s32)&SceneId_TorebiIzumi1 lost its cast.
        let printed = humanized(
            "\
void Work_Scene(struct Work *work)
{
    Party_Set((s32)&Scene_Id, (s32)work->a);
}
",
            1,
        );
        assert!(printed.contains("Party_Set((s32)&Scene_Id, work->a);"), "{printed}");
    }

    #[test]
    fn a_function_defined_in_several_editions_is_counted() {
        let text = "\
#if TBS_EDITION_JA
void NameEntry_Count(u8 *name)
{
    name[0] = (u8)(name[1] - 0xde);
}
#else
void NameEntry_Count(u8 *name)
{
    name[0] = 0;
}
#endif
void NameEntry_Other(void)
{
}
";
        let (masked, _) = Mask::new(text);
        assert_eq!(definitions(&masked, "NameEntry_Count"), 2);
        assert_eq!(definitions(&masked, "NameEntry_Other"), 1);
    }

    #[test]
    fn every_edition_of_the_files_game_judges() {
        // NAME.C's NameEntry_CountLetters exists only in the Japanese build;
        // an English-only check passed a change of its behaviour.
        let tbs: Vec<_> = editions_for(Path::new("games/THE BROKEN SEAL/SRC/MENU/NAME.C")).iter().map(|t| t.id.as_str()).collect();
        assert_eq!(tbs, ["tbs-ja", "tbs-en", "tbs-de", "tbs-es", "tbs-fr", "tbs-it"]);
        assert_eq!(editions_for(Path::new("games/THE LOST AGE/SRC/X.C")).len(), 6);
        assert_eq!(editions_for(Path::new("games/COMMON/SRC/SOUND/TAIL.C")).len(), 12);
    }

    #[test]
    fn the_next_step_is_a_bare_keep() {
        let message = next_step(Path::new("games/THE BROKEN SEAL/SRC/MENU/NAME.C"));
        assert!(message.contains("next: a bare alchemy keep"), "{message}");
        assert!(!message.contains("alchemy keep games"), "{message}");
    }

    #[test]
    fn fakematch_statements_are_left_alone() {
        let printed = humanized(
            "\
void Work_Pin(struct Work *work)
{
    work->first = (u8)work->zero;
    /* FAKEMATCH: the native keeps the cast. */
    work->low = (u8)work->value;
    work->high = (u8)work->other;
}
",
            3,
        );
        assert!(printed.contains("work->first = work->zero;"), "{printed}");
        assert!(printed.contains("work->low = (u8)work->value;"), "{printed}");
        assert!(printed.contains("work->high = work->other;"), "{printed}");
    }

    /// One definition of tagged code for the search as for the carry: a tag
    /// on the first line of a block, the function's own included, covers
    /// the whole block, as camelotize's tagged module says.
    #[test]
    fn a_tag_on_a_blocks_first_line_covers_the_block() {
        let texts = [
            "void Work_All(struct Work *work)\n{\n    /* FAKEMATCH: the native frame. */\n    work->a = (u8)work->b;\n    work->c = (u8)work->d;\n}\n",
            "void Work_Nest(struct Work *work)\n{\n    if (work->x) {\n        /* FAKEMATCH: order */\n        work->a = (u8)work->b;\n        work->c = (u8)work->d;\n    }\n    work->e = (u8)work->f;\n}\n",
            "void Work_Mid(struct Work *work)\n{\n    work->e = (u8)work->f; work->a = (u8)work->b; /* FAKEMATCH: r4 */\n    work->c = (u8)work->d;\n}\n",
        ];
        for text in texts {
            for seed in 1..6 {
                let printed = humanized(text, seed);
                assert!(!printed.contains("__humanize"), "{printed}");
                assert!(printed.contains("work->a = (u8)work->b;"), "{printed}");
                let (masked, mask) = Mask::new(text);
                assert_eq!(mask.unmask(&masked), text);
            }
        }
        let nested = humanized(texts[1], 1);
        assert!(nested.contains("work->c = (u8)work->d;") && nested.contains("work->e = work->f;"), "{nested}");
        let mid = humanized(texts[2], 1);
        assert!(mid.contains("work->e = work->f;") && mid.contains("work->c = work->d;"), "{mid}");
        assert!(humanized(texts[0], 1).contains("work->c = (u8)work->d;"));
    }

    #[test]
    fn inline_assembly_is_masked_kept_and_restored() {
        let text = "\
void Work_Asm(struct Work *work)
{
    register u32 v asm(\"r4\"); /* FAKEMATCH: r4 as the native allocates it. */
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
        let first = candidates(&function, None, &mut rng);
        let mut rng = Rng::new(7);
        assert_eq!(first, candidates(&function, None, &mut rng));
        let printed = a.print();
        assert!(!printed.contains("while (0)"), "{printed}");
        assert!(printed.contains("work->d != 0x123"), "{printed}");
        assert!(printed.contains("i++;"), "{printed}");
        // Numbers keep their base.
        assert!(printed.contains("work->b + 0x40"), "{printed}");
        assert!(printed.contains("REG_BLDALPHA = 0x1010;"), "{printed}");
        assert!(printed.contains("work->y += 0x500000;"), "{printed}");
    }
}
