//! Small, semantics-preserving rewrites of one function body.
//!
//! Every mutation keeps the function's meaning under C89 as GCC 2.96 compiles
//! it for this target: it only exchanges spellings that denote the same
//! computation, or moves code across code it cannot observe. A site the
//! analysis cannot prove safe is declined, never guessed.

use super::ast::*;
use super::effects::{before_store, escapes, expr_effects, has_continue, stmt_effects, Effects};
use super::types::{root_name, CType, Env};

/// A deterministic generator (SplitMix64), so a seed names a search.
#[derive(Clone, Debug)]
pub struct Rng(u64);

impl Rng {
    pub fn new(seed: u64) -> Rng {
        Rng(seed)
    }

    pub fn next(&mut self) -> u64 {
        self.0 = self.0.wrapping_add(0x9e37_79b9_7f4a_7c15);
        let mut z = self.0;
        z = (z ^ (z >> 30)).wrapping_mul(0xbf58_476d_1ce4_e5b9);
        z = (z ^ (z >> 27)).wrapping_mul(0x94d0_49bb_1331_11eb);
        z ^ (z >> 31)
    }

    pub fn below(&mut self, bound: usize) -> usize {
        (self.next() % bound.max(1) as u64) as usize
    }

    pub fn chance(&mut self, numerator: u64, denominator: u64) -> bool {
        self.next() % denominator < numerator
    }
}

#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash, PartialOrd, Ord)]
pub enum Kind {
    SwapOperands,
    ReorderStatements,
    ReorderDeclarations,
    IntroduceTemporary,
    ShareTemporary,
    RemoveTemporary,
    AddCast,
    DropCast,
    LoopForm,
    PointerIndex,
    CompoundAssignment,
    ConditionAssignment,
    Register,
    InvertIf,
    ZeroTest,
}

impl Kind {
    pub const ALL: [(Kind, u64); 15] = [
        (Kind::SwapOperands, 10),
        (Kind::ReorderStatements, 10),
        (Kind::ReorderDeclarations, 8),
        (Kind::IntroduceTemporary, 8),
        (Kind::ShareTemporary, 4),
        (Kind::RemoveTemporary, 6),
        (Kind::AddCast, 5),
        (Kind::DropCast, 4),
        (Kind::LoopForm, 6),
        (Kind::PointerIndex, 4),
        (Kind::CompoundAssignment, 4),
        (Kind::ConditionAssignment, 4),
        (Kind::Register, 3),
        (Kind::InvertIf, 4),
        (Kind::ZeroTest, 4),
    ];

    pub fn name(self) -> &'static str {
        match self {
            Kind::SwapOperands => "swap commutative operands",
            Kind::ReorderStatements => "reorder independent statements",
            Kind::ReorderDeclarations => "reorder local declarations",
            Kind::IntroduceTemporary => "introduce a temporary",
            Kind::ShareTemporary => "share one temporary between two statements",
            Kind::RemoveTemporary => "remove a temporary",
            Kind::AddCast => "add a same-width cast",
            Kind::DropCast => "drop a same-width cast",
            Kind::LoopForm => "change loop form",
            Kind::PointerIndex => "pointer arithmetic or indexing",
            Kind::CompoundAssignment => "split or join a compound assignment",
            Kind::ConditionAssignment => "move an assignment into or out of a condition or call",
            Kind::Register => "toggle register",
            Kind::InvertIf => "invert an if/else",
            Kind::ZeroTest => "test truth or compare with zero",
        }
    }
}

/// Apply one random mutation; `None` when no kind has a site.
pub fn mutate(function: &mut Function, env: &mut Env, rng: &mut Rng) -> Option<Kind> {
    let mut kinds: Vec<(Kind, u64)> = Kind::ALL.to_vec();
    while !kinds.is_empty() {
        let total: u64 = kinds.iter().map(|(_, weight)| weight).sum();
        let mut pick = rng.next() % total;
        let index = kinds
            .iter()
            .position(|(_, weight)| {
                if pick < *weight {
                    true
                } else {
                    pick -= weight;
                    false
                }
            })
            .expect("a weighted pick");
        let (kind, _) = kinds.remove(index);
        if apply(kind, function, env, rng) {
            env.refresh(function);
            return Some(kind);
        }
    }
    None
}

pub fn apply(kind: Kind, function: &mut Function, env: &Env, rng: &mut Rng) -> bool {
    match kind {
        Kind::SwapOperands => expr_mutation(function, env, rng, &swap_site, &swap_apply),
        Kind::AddCast => expr_mutation(function, env, rng, &add_cast_site, &add_cast_apply),
        Kind::DropCast => expr_mutation(function, env, rng, &drop_cast_site, &drop_cast_apply),
        Kind::PointerIndex => expr_mutation(function, env, rng, &pointer_site, &pointer_apply),
        Kind::CompoundAssignment => {
            expr_mutation(function, env, rng, &compound_site, &compound_apply)
        }
        Kind::ReorderStatements => {
            block_mutation(function, env, rng, &reorder_sites, &reorder_apply)
        }
        Kind::ReorderDeclarations => {
            block_mutation(function, env, rng, &declaration_sites, &declaration_apply)
        }
        Kind::IntroduceTemporary => introduce_temporary(function, env, rng),
        Kind::ShareTemporary => share_temporary(function, env, rng),
        Kind::RemoveTemporary => remove_temporary(function, env, rng),
        Kind::LoopForm => block_mutation(function, env, rng, &loop_sites, &loop_apply),
        Kind::ConditionAssignment => {
            block_mutation(function, env, rng, &condition_sites, &condition_apply)
        }
        Kind::Register => block_mutation(function, env, rng, &register_sites, &register_apply),
        Kind::InvertIf => block_mutation(function, env, rng, &invert_sites, &invert_apply),
        Kind::ZeroTest => expr_mutation(function, env, rng, &zero_site, &zero_apply),
    }
}

// Expression contexts.

/// How the value of an expression is consumed above it.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum Sink {
    /// Discarded.
    Unused,
    /// Only compared against zero.
    Truth,
    /// Only its low bits reach anything observable.
    Bits(u8),
    Full,
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum Role {
    Value,
    /// Stored to, or read and stored by `++`, `--` and compound assignment.
    Target,
    /// The operand of `&`.
    Address,
    /// Inside `sizeof` or a case label: never evaluated.
    Unevaluated,
    Callee,
}

#[derive(Clone, Copy, Debug)]
pub struct Ctx {
    pub sink: Sink,
    pub role: Role,
    /// Evaluated only on some paths: right of `&&` or `||`, or a `?:` arm.
    pub guarded: bool,
}

type Visit<'v> = dyn FnMut(&mut Expr, Ctx) -> bool + 'v;

fn width(ty: Option<CType>) -> Option<u8> {
    match ty.as_ref().map(CType::strip) {
        Some(CType::Int { bits, .. }) => Some(*bits),
        Some(CType::Ptr(_)) => Some(32),
        _ => None,
    }
}

fn low_bits(sink: Sink) -> Sink {
    match sink {
        Sink::Unused => Sink::Unused,
        Sink::Truth => Sink::Bits(32),
        other => other,
    }
}

fn walk_expr(expr: &mut Expr, ctx: Ctx, env: &Env, visit: &mut Visit) -> bool {
    if visit(expr, ctx) {
        return true;
    }
    let value = |sink| Ctx {
        sink,
        role: Role::Value,
        guarded: ctx.guarded,
    };
    let assigned_width = |target: &Expr| width(env.type_of(target));
    match expr {
        Expr::Ident(_) | Expr::Number(_) | Expr::Char(_) | Expr::Str(_) | Expr::SizeofType(_) => {
            false
        }
        Expr::SizeofExpr(operand) => walk_expr(
            operand,
            Ctx {
                sink: Sink::Full,
                role: Role::Unevaluated,
                guarded: ctx.guarded,
            },
            env,
            visit,
        ),
        Expr::Unary(op, operand) => {
            let child = match op {
                UnOp::Neg | UnOp::Plus | UnOp::BitNot => value(low_bits(ctx.sink)),
                UnOp::Not => value(Sink::Truth),
                UnOp::Deref => value(Sink::Full),
                UnOp::AddrOf => Ctx {
                    sink: Sink::Full,
                    role: Role::Address,
                    guarded: ctx.guarded,
                },
                UnOp::PreInc | UnOp::PreDec => Ctx {
                    sink: Sink::Full,
                    role: Role::Target,
                    guarded: ctx.guarded,
                },
            };
            walk_expr(operand, child, env, visit)
        }
        Expr::PostInc(operand) | Expr::PostDec(operand) => walk_expr(
            operand,
            Ctx {
                sink: Sink::Full,
                role: Role::Target,
                guarded: ctx.guarded,
            },
            env,
            visit,
        ),
        Expr::Binary(op, left, right) => {
            let (left_ctx, right_ctx) = match op {
                BinOp::Add
                | BinOp::Sub
                | BinOp::Mul
                | BinOp::BitAnd
                | BinOp::BitOr
                | BinOp::BitXor => (value(low_bits(ctx.sink)), value(low_bits(ctx.sink))),
                BinOp::Shl => (value(low_bits(ctx.sink)), value(Sink::Full)),
                BinOp::Eq | BinOp::Ne => (value(Sink::Bits(32)), value(Sink::Bits(32))),
                BinOp::And | BinOp::Or => (
                    value(Sink::Truth),
                    Ctx {
                        sink: Sink::Truth,
                        role: Role::Value,
                        guarded: true,
                    },
                ),
                _ => (value(Sink::Full), value(Sink::Full)),
            };
            walk_expr(left, left_ctx, env, visit) || walk_expr(right, right_ctx, env, visit)
        }
        Expr::Assign(op, target, value_expr) => {
            let bits = assigned_width(target);
            let value_sink = match (op, bits) {
                (
                    None
                    | Some(
                        BinOp::Add
                        | BinOp::Sub
                        | BinOp::Mul
                        | BinOp::BitAnd
                        | BinOp::BitOr
                        | BinOp::BitXor,
                    ),
                    Some(bits),
                ) => Sink::Bits(bits),
                _ => Sink::Full,
            };
            let target_ctx = Ctx {
                sink: Sink::Full,
                role: Role::Target,
                guarded: ctx.guarded,
            };
            walk_expr(value_expr, value(value_sink), env, visit)
                || walk_expr(target, target_ctx, env, visit)
        }
        Expr::Cond(cond, then, other) => {
            let arm = Ctx {
                sink: ctx.sink,
                role: Role::Value,
                guarded: true,
            };
            walk_expr(cond, value(Sink::Truth), env, visit)
                || walk_expr(then, arm, env, visit)
                || walk_expr(other, arm, env, visit)
        }
        Expr::Comma(left, right) => {
            walk_expr(left, value(Sink::Unused), env, visit) || walk_expr(right, ctx, env, visit)
        }
        Expr::Call(callee, arguments) => {
            let callee_ctx = Ctx {
                sink: Sink::Full,
                role: Role::Callee,
                guarded: ctx.guarded,
            };
            if walk_expr(callee, callee_ctx, env, visit) {
                return true;
            }
            arguments
                .iter_mut()
                .any(|argument| walk_expr(argument, value(Sink::Full), env, visit))
        }
        Expr::Index(base, index) => {
            walk_expr(base, value(Sink::Full), env, visit)
                || walk_expr(index, value(Sink::Bits(32)), env, visit)
        }
        Expr::Member(base, _, arrow) => {
            let base_ctx = if *arrow {
                value(Sink::Full)
            } else {
                Ctx {
                    sink: Sink::Full,
                    role: ctx.role,
                    guarded: ctx.guarded,
                }
            };
            walk_expr(base, base_ctx, env, visit)
        }
        Expr::Cast(name, operand) => {
            let target = env.type_name(name);
            let sink = match target.strip() {
                CType::Void => Sink::Unused,
                CType::Int { bits, .. } => Sink::Bits(*bits),
                _ => Sink::Full,
            };
            walk_expr(operand, value(sink), env, visit)
        }
    }
}

fn root(sink: Sink) -> Ctx {
    Ctx {
        sink,
        role: Role::Value,
        guarded: false,
    }
}

fn walk_init(init: &mut Init, sink: Sink, env: &Env, visit: &mut Visit) -> bool {
    match init {
        Init::Expr(expr) => walk_expr(expr, root(sink), env, visit),
        Init::List(items) => items
            .iter_mut()
            .any(|item| walk_init(item, Sink::Full, env, visit)),
    }
}

/// Visit the expressions a statement evaluates itself, with their contexts.
fn walk_own(stmt: &mut Stmt, env: &Env, visit: &mut Visit) -> bool {
    match stmt {
        Stmt::Expr(expr) => walk_expr(expr, root(Sink::Unused), env, visit),
        Stmt::Decl(decl) => {
            let specs = decl.specs.clone();
            decl.items.iter_mut().any(|item| {
                let bits = width(Some(env.declared(&specs, item)));
                let sink = bits.map_or(Sink::Full, Sink::Bits);
                match &mut item.init {
                    Some(init) => walk_init(init, sink, env, visit),
                    None => false,
                }
            })
        }
        Stmt::If(cond, ..) | Stmt::While(cond, _) | Stmt::DoWhile(_, cond) => {
            walk_expr(cond, root(Sink::Truth), env, visit)
        }
        Stmt::For(init, cond, step, _) => {
            init.as_mut()
                .is_some_and(|init| walk_expr(init, root(Sink::Unused), env, visit))
                || cond
                    .as_mut()
                    .is_some_and(|cond| walk_expr(cond, root(Sink::Truth), env, visit))
                || step
                    .as_mut()
                    .is_some_and(|step| walk_expr(step, root(Sink::Unused), env, visit))
        }
        Stmt::Switch(expr, _) => walk_expr(expr, root(Sink::Full), env, visit),
        Stmt::Return(Some(expr)) => {
            let sink = width(env.return_type.clone()).map_or(Sink::Full, Sink::Bits);
            walk_expr(expr, root(sink), env, visit)
        }
        _ => false,
    }
}

fn walk_stmt(stmt: &mut Stmt, env: &Env, visit: &mut Visit) -> bool {
    if focused(env, stmt) && walk_own(stmt, env, visit) {
        return true;
    }
    match stmt {
        Stmt::Block(stmts) => stmts.iter_mut().any(|stmt| walk_stmt(stmt, env, visit)),
        Stmt::If(_, then, other) => {
            walk_stmt(then, env, visit)
                || other
                    .as_mut()
                    .is_some_and(|other| walk_stmt(other, env, visit))
        }
        Stmt::While(_, body)
        | Stmt::DoWhile(body, _)
        | Stmt::For(_, _, _, body)
        | Stmt::Switch(_, body) => walk_stmt(body, env, visit),
        _ => false,
    }
}

fn walk_function(function: &mut Function, env: &Env, visit: &mut Visit) -> bool {
    function
        .body
        .iter_mut()
        .any(|stmt| walk_stmt(stmt, env, visit))
}

type ExprSite = dyn Fn(&Expr, Ctx, &Env) -> bool;
type ExprApply = dyn Fn(&mut Expr, Ctx, &Env, &mut Rng) -> bool;

fn expr_mutation(
    function: &mut Function,
    env: &Env,
    rng: &mut Rng,
    site: &ExprSite,
    apply: &ExprApply,
) -> bool {
    let mut count = 0;
    walk_function(function, env, &mut |expr, ctx| {
        if site(expr, ctx, env) {
            count += 1;
        }
        false
    });
    if count == 0 {
        return false;
    }
    let target = rng.below(count);
    let mut seen = 0;
    let mut done = false;
    walk_function(function, env, &mut |expr, ctx| {
        if !site(expr, ctx, env) {
            return false;
        }
        if seen == target {
            done = apply(expr, ctx, env, rng);
            return true;
        }
        seen += 1;
        false
    });
    done
}

// Statement lists.

fn walk_blocks(stmts: &mut Vec<Stmt>, visit: &mut dyn FnMut(&mut Vec<Stmt>) -> bool) -> bool {
    if visit(stmts) {
        return true;
    }
    stmts.iter_mut().any(|stmt| walk_nested_blocks(stmt, visit))
}

fn walk_nested_blocks(stmt: &mut Stmt, visit: &mut dyn FnMut(&mut Vec<Stmt>) -> bool) -> bool {
    match stmt {
        Stmt::Block(stmts) => walk_blocks(stmts, visit),
        Stmt::If(_, then, other) => {
            walk_nested_blocks(then, visit)
                || other
                    .as_mut()
                    .is_some_and(|other| walk_nested_blocks(other, visit))
        }
        Stmt::While(_, body)
        | Stmt::DoWhile(body, _)
        | Stmt::For(_, _, _, body)
        | Stmt::Switch(_, body) => walk_nested_blocks(body, visit),
        _ => false,
    }
}

type BlockSites<S> = dyn Fn(&[Stmt], &Env) -> Vec<S>;
type BlockApply<S> = dyn Fn(&mut Vec<Stmt>, S, &Env, &mut Rng) -> bool;

fn block_mutation<S: Clone>(
    function: &mut Function,
    env: &Env,
    rng: &mut Rng,
    sites: &BlockSites<S>,
    apply: &BlockApply<S>,
) -> bool {
    let mut count = 0;
    walk_blocks(&mut function.body, &mut |stmts| {
        count += sites(stmts, env).len();
        false
    });
    if count == 0 {
        return false;
    }
    let mut target = rng.below(count);
    let mut done = false;
    walk_blocks(&mut function.body, &mut |stmts| {
        let found = sites(stmts, env);
        if target < found.len() {
            done = apply(stmts, found[target].clone(), env, rng);
            return true;
        }
        target -= found.len();
        false
    });
    done
}

fn leading_declarations(stmts: &[Stmt]) -> usize {
    stmts
        .iter()
        .position(|stmt| !matches!(stmt, Stmt::Decl(_) | Stmt::Comment(_)))
        .unwrap_or(stmts.len())
}

/// Whether `--focus` admits the statement: its own text (a loop's or an
/// if's header, not its body) matches the pattern.
fn focused(env: &Env, stmt: &Stmt) -> bool {
    let Some(focus) = &env.focus else {
        return true;
    };
    let text = match stmt {
        Stmt::Decl(decl) => print_decl(decl),
        other => other
            .own_exprs()
            .iter()
            .map(|expr| print_expr(expr))
            .collect::<Vec<_>>()
            .join("; "),
    };
    focus.is_match(&text)
}

/// Whether a focused statement mentions `name`.
fn mentioned_in_focus(function: &Function, env: &Env, name: &str) -> bool {
    if env.focus.is_none() {
        return true;
    }
    let mut found = false;
    for stmt in &function.body {
        stmt.each(&mut |stmt| {
            let declares =
                matches!(stmt, Stmt::Decl(decl) if decl.items.iter().any(|item| item.name == name));
            let mentions = stmt.own_exprs().iter().any(|expr| expr.mentions(name) > 0);
            if (declares || mentions) && focused(env, stmt) {
                found = true;
            }
        });
    }
    found
}

// Swap commutative operands.

fn swap_site(expr: &Expr, ctx: Ctx, env: &Env) -> bool {
    if ctx.role == Role::Unevaluated {
        return false;
    }
    match expr {
        Expr::Binary(op, left, right) => {
            matches!(
                op,
                BinOp::Add
                    | BinOp::Mul
                    | BinOp::BitAnd
                    | BinOp::BitOr
                    | BinOp::BitXor
                    | BinOp::Eq
                    | BinOp::Ne
                    | BinOp::Lt
                    | BinOp::Gt
                    | BinOp::Le
                    | BinOp::Ge
            ) && !expr_effects(left, env).conflicts(&expr_effects(right, env))
        }
        _ => false,
    }
}

fn mirrored(op: BinOp) -> BinOp {
    match op {
        BinOp::Lt => BinOp::Gt,
        BinOp::Gt => BinOp::Lt,
        BinOp::Le => BinOp::Ge,
        BinOp::Ge => BinOp::Le,
        other => other,
    }
}

fn associative(op: BinOp) -> bool {
    matches!(
        op,
        BinOp::Add | BinOp::Mul | BinOp::BitAnd | BinOp::BitOr | BinOp::BitXor
    )
}

/// `(a op b) op c` and `a op (b op c)` for an associative integer operator,
/// when the three operands cannot observe each other.
fn regroupable(op: BinOp, left: &Expr, right: &Expr, env: &Env) -> bool {
    if !associative(op) {
        return false;
    }
    let parts: Vec<&Expr> = match (left, right) {
        (Expr::Binary(inner, a, b), c) if *inner == op => vec![a, b, c],
        (a, Expr::Binary(inner, b, c)) if *inner == op => vec![a, b, c],
        _ => return false,
    };
    let effects: Vec<_> = parts.iter().map(|part| expr_effects(part, env)).collect();
    (0..3).all(|i| (i + 1..3).all(|j| !effects[i].conflicts(&effects[j])))
}

fn swap_apply(expr: &mut Expr, _: Ctx, env: &Env, rng: &mut Rng) -> bool {
    let Expr::Binary(op, left, right) = expr else {
        return false;
    };
    let op = *op;
    if regroupable(op, left, right, env) && rng.chance(1, 2) {
        let taken = std::mem::replace(expr, Expr::Number("0".into()));
        let Expr::Binary(_, left, right) = taken else {
            unreachable!("a binary expression");
        };
        *expr = match (*left, *right) {
            (Expr::Binary(inner, a, b), c) if inner == op => {
                Expr::binary(op, *a, Expr::Binary(op, b, Box::new(c)))
            }
            (a, Expr::Binary(_, b, c)) => Expr::binary(op, Expr::Binary(op, Box::new(a), b), *c),
            (a, c) => Expr::binary(op, a, c),
        };
        return true;
    }
    if let Expr::Binary(op, left, right) = expr {
        std::mem::swap(left, right);
        *op = mirrored(*op);
    }
    true
}

// Comparisons with zero.

fn zero(expr: &Expr) -> bool {
    matches!(expr, Expr::Number(text) if text == "0")
}

fn zero_site(expr: &Expr, ctx: Ctx, env: &Env) -> bool {
    if ctx.sink != Sink::Truth || ctx.role != Role::Value {
        return false;
    }
    match expr {
        Expr::Binary(BinOp::Eq | BinOp::Ne, left, right) => zero(left) || zero(right),
        Expr::Unary(UnOp::Not, _) => true,
        Expr::Binary(op, ..) if op.is_comparison() || matches!(op, BinOp::And | BinOp::Or) => false,
        other => env
            .type_of(other)
            .is_some_and(|ty| ty.int().is_some() || ty.is_pointer_like()),
    }
}

fn zero_apply(expr: &mut Expr, _: Ctx, _: &Env, _: &mut Rng) -> bool {
    let taken = std::mem::replace(expr, Expr::Number("0".into()));
    *expr = match taken {
        Expr::Binary(op @ (BinOp::Eq | BinOp::Ne), left, right) => {
            let tested = if zero(&right) { *left } else { *right };
            if op == BinOp::Ne {
                tested
            } else {
                Expr::Unary(UnOp::Not, Box::new(tested))
            }
        }
        Expr::Unary(UnOp::Not, operand) => {
            Expr::Binary(BinOp::Eq, operand, Box::new(Expr::Number("0".into())))
        }
        other => Expr::binary(BinOp::Ne, other, Expr::Number("0".into())),
    };
    true
}

// Same-width casts.

fn is_cast(expr: &Expr) -> bool {
    matches!(expr, Expr::Cast(..))
}

/// Whether reinterpreting a `bits`-wide value with the other signedness is
/// invisible to how the value is consumed.
fn sign_invisible(sink: Sink, bits: u8) -> bool {
    match sink {
        Sink::Unused | Sink::Truth => true,
        Sink::Bits(consumed) => consumed <= bits,
        Sink::Full => false,
    }
}

fn add_cast_site(expr: &Expr, ctx: Ctx, env: &Env) -> bool {
    if ctx.role != Role::Value || is_cast(expr) || matches!(expr, Expr::Str(_)) {
        return false;
    }
    matches!(
        env.type_of(expr).as_ref().and_then(CType::int),
        Some((bits, _)) if bits <= 32
    )
}

fn cast_to(env: &Env, bits: u8, signed: bool, operand: Expr) -> Option<Expr> {
    let spelling = env.int_spelling(bits, signed)?;
    Some(Expr::Cast(
        TypeName {
            tokens: spelling.split_whitespace().map(str::to_string).collect(),
        },
        Box::new(operand),
    ))
}

fn add_cast_apply(expr: &mut Expr, ctx: Ctx, env: &Env, rng: &mut Rng) -> bool {
    let Some((bits, signed)) = env.type_of(expr).as_ref().and_then(CType::int) else {
        return false;
    };
    let flip = sign_invisible(ctx.sink, bits) && rng.chance(2, 3);
    let operand = std::mem::replace(expr, Expr::Number("0".into()));
    match cast_to(env, bits, signed != flip, operand.clone()) {
        Some(cast) => {
            *expr = cast;
            true
        }
        None => {
            *expr = operand;
            false
        }
    }
}

fn drop_cast_site(expr: &Expr, ctx: Ctx, env: &Env) -> bool {
    let Expr::Cast(name, operand) = expr else {
        return false;
    };
    if ctx.role != Role::Value {
        return false;
    }
    let target = env.type_name(name);
    let Some(source) = env.type_of(operand) else {
        return false;
    };
    if target.same(&source) {
        return true;
    }
    match (target.int(), source.int()) {
        (Some((to_bits, _)), Some((from_bits, _))) => {
            to_bits == from_bits && sign_invisible(ctx.sink, to_bits)
        }
        _ => false,
    }
}

fn drop_cast_apply(expr: &mut Expr, _: Ctx, _: &Env, _: &mut Rng) -> bool {
    if let Expr::Cast(_, operand) = expr {
        let operand = std::mem::replace(operand.as_mut(), Expr::Number("0".into()));
        *expr = operand;
        return true;
    }
    false
}

// Pointer arithmetic and indexing.

fn pointer_to_object(env: &Env, expr: &Expr) -> bool {
    env.type_of(expr).is_some_and(|ty| {
        ty.decay()
            .pointee()
            .is_some_and(|inner| !matches!(inner.strip(), CType::Func(..) | CType::Void))
    })
}

fn pointer_site(expr: &Expr, ctx: Ctx, env: &Env) -> bool {
    if ctx.role == Role::Unevaluated {
        return false;
    }
    match expr {
        Expr::Index(..) => true,
        Expr::Unary(UnOp::Deref, pointer) => {
            matches!(pointer.as_ref(), Expr::Binary(BinOp::Add, ..))
                || pointer_to_object(env, pointer)
        }
        Expr::Unary(UnOp::AddrOf, operand) => {
            ctx.role == Role::Value && matches!(operand.as_ref(), Expr::Index(..))
        }
        Expr::Binary(BinOp::Add, left, right) => {
            ctx.role == Role::Value
                && pointer_to_object(env, left)
                && env.type_of(right).is_some_and(|ty| ty.int().is_some())
        }
        Expr::Member(base, _, true) => !matches!(base.as_ref(), Expr::Unary(UnOp::Deref, _)),
        Expr::Member(base, _, false) => matches!(base.as_ref(), Expr::Unary(UnOp::Deref, _)),
        _ => false,
    }
}

fn is_zero(expr: &Expr) -> bool {
    matches!(expr, Expr::Number(text) if text == "0")
}

fn pointer_apply(expr: &mut Expr, _: Ctx, _: &Env, _: &mut Rng) -> bool {
    let taken = std::mem::replace(expr, Expr::Number("0".into()));
    *expr = match taken {
        Expr::Index(base, index) => {
            if is_zero(&index) {
                Expr::Unary(UnOp::Deref, base)
            } else {
                Expr::Unary(UnOp::Deref, Box::new(Expr::Binary(BinOp::Add, base, index)))
            }
        }
        Expr::Unary(UnOp::Deref, pointer) => match *pointer {
            Expr::Binary(BinOp::Add, base, index) => Expr::Index(base, index),
            other => Expr::Index(Box::new(other), Box::new(Expr::Number("0".into()))),
        },
        Expr::Unary(UnOp::AddrOf, operand) => match *operand {
            Expr::Index(base, index) => Expr::Binary(BinOp::Add, base, index),
            other => Expr::Unary(UnOp::AddrOf, Box::new(other)),
        },
        Expr::Binary(BinOp::Add, base, index) => {
            Expr::Unary(UnOp::AddrOf, Box::new(Expr::Index(base, index)))
        }
        Expr::Member(base, member, true) => {
            Expr::Member(Box::new(Expr::Unary(UnOp::Deref, base)), member, false)
        }
        Expr::Member(base, member, false) => match *base {
            Expr::Unary(UnOp::Deref, pointer) => Expr::Member(pointer, member, true),
            other => Expr::Member(Box::new(other), member, false),
        },
        other => other,
    };
    true
}

// Compound assignment.

fn compound_op(op: BinOp) -> bool {
    matches!(
        op,
        BinOp::Add
            | BinOp::Sub
            | BinOp::Mul
            | BinOp::Div
            | BinOp::Mod
            | BinOp::Shl
            | BinOp::Shr
            | BinOp::BitAnd
            | BinOp::BitOr
            | BinOp::BitXor
    )
}

fn commutative(op: BinOp) -> bool {
    matches!(
        op,
        BinOp::Add | BinOp::Mul | BinOp::BitAnd | BinOp::BitOr | BinOp::BitXor
    )
}

fn joinable(target: &Expr, value: &Expr) -> bool {
    match value {
        Expr::Binary(op, left, right) if compound_op(*op) => {
            left.as_ref() == target || (commutative(*op) && right.as_ref() == target)
        }
        _ => false,
    }
}

fn compound_site(expr: &Expr, ctx: Ctx, env: &Env) -> bool {
    if ctx.role == Role::Unevaluated {
        return false;
    }
    match expr {
        Expr::Assign(Some(_), target, _) => expr_effects(target, env).pure(),
        Expr::Assign(None, target, value) => {
            joinable(target, value) && expr_effects(target, env).pure()
        }
        Expr::PostInc(_) | Expr::PostDec(_) | Expr::Unary(UnOp::PreInc | UnOp::PreDec, _) => {
            ctx.sink == Sink::Unused
        }
        _ => false,
    }
}

fn one() -> Box<Expr> {
    Box::new(Expr::Number("1".into()))
}

fn compound_apply(expr: &mut Expr, ctx: Ctx, _: &Env, rng: &mut Rng) -> bool {
    let taken = std::mem::replace(expr, Expr::Number("0".into()));
    *expr = match taken {
        Expr::Assign(Some(op), target, value) => {
            let unit = matches!(value.as_ref(), Expr::Number(text) if text == "1");
            if unit
                && ctx.sink == Sink::Unused
                && matches!(op, BinOp::Add | BinOp::Sub)
                && rng.chance(1, 2)
            {
                if op == BinOp::Add {
                    Expr::PostInc(target)
                } else {
                    Expr::PostDec(target)
                }
            } else {
                let copy = target.clone();
                Expr::Assign(None, target, Box::new(Expr::Binary(op, copy, value)))
            }
        }
        Expr::Assign(None, target, value) => match *value {
            Expr::Binary(op, left, right) => {
                let rest = if left == target { right } else { left };
                Expr::Assign(Some(op), target, rest)
            }
            other => Expr::Assign(None, target, Box::new(other)),
        },
        Expr::PostInc(target) if rng.chance(1, 2) => Expr::Unary(UnOp::PreInc, target),
        Expr::Unary(UnOp::PreInc, target) if rng.chance(1, 2) => Expr::PostInc(target),
        Expr::PostInc(target) | Expr::Unary(UnOp::PreInc, target) => {
            Expr::Assign(Some(BinOp::Add), target, one())
        }
        Expr::PostDec(target) if rng.chance(1, 2) => Expr::Unary(UnOp::PreDec, target),
        Expr::Unary(UnOp::PreDec, target) if rng.chance(1, 2) => Expr::PostDec(target),
        Expr::PostDec(target) | Expr::Unary(UnOp::PreDec, target) => {
            Expr::Assign(Some(BinOp::Sub), target, one())
        }
        other => other,
    };
    true
}

// Reorder statements.

/// Statements that may be moved as a unit.
fn movable(stmt: &Stmt) -> bool {
    !matches!(
        stmt,
        Stmt::Decl(_)
            | Stmt::Comment(_)
            | Stmt::Case(_)
            | Stmt::Default
            | Stmt::Label(_)
            | Stmt::Empty
            | Stmt::Break
            | Stmt::Continue
            | Stmt::Return(_)
            | Stmt::Goto(_)
    ) && !escapes(stmt)
}

fn reorder_sites(stmts: &[Stmt], env: &Env) -> Vec<(usize, usize)> {
    let start = leading_declarations(stmts);
    let effects: Vec<Option<Effects>> = stmts
        .iter()
        .map(|stmt| movable(stmt).then(|| stmt_effects(stmt, env)))
        .collect();
    let mut sites = Vec::new();
    for from in start..stmts.len() {
        let Some(moved) = &effects[from] else {
            continue;
        };
        if !focused(env, &stmts[from]) {
            continue;
        }
        for step in 1..=4usize {
            for to in [from.checked_sub(step), Some(from + step)]
                .into_iter()
                .flatten()
            {
                if to < start || to >= stmts.len() {
                    continue;
                }
                let crossed = if to < from {
                    to..from
                } else {
                    from + 1..to + 1
                };
                let clear = crossed.clone().all(|index| match &effects[index] {
                    Some(other) => !moved.conflicts(other),
                    None => matches!(stmts[index], Stmt::Comment(_)),
                });
                let code = crossed
                    .clone()
                    .any(|index| !matches!(stmts[index], Stmt::Comment(_)));
                if clear && code {
                    sites.push((from, to));
                }
            }
        }
    }
    sites
}

fn reorder_apply(stmts: &mut Vec<Stmt>, (from, to): (usize, usize), _: &Env, _: &mut Rng) -> bool {
    let stmt = stmts.remove(from);
    stmts.insert(to, stmt);
    true
}

// Reorder declarations.

#[derive(Clone)]
enum DeclSite {
    Swap(usize, usize),
    Items(usize, usize),
}

fn declaration_depends(first: &Decl, second: &Decl) -> bool {
    let names = |decl: &Decl| {
        decl.items
            .iter()
            .map(|item| item.name.clone())
            .collect::<Vec<_>>()
    };
    let mentions = |decl: &Decl, names: &[String]| {
        let stmt = Stmt::Decl(decl.clone());
        let mut found = false;
        stmt.each_expr(&mut |expr| {
            if let Expr::Ident(name) = expr {
                found |= names.contains(name);
            }
        });
        found
    };
    mentions(second, &names(first)) || mentions(first, &names(second))
}

fn declaration_sites(stmts: &[Stmt], env: &Env) -> Vec<DeclSite> {
    let end = leading_declarations(stmts);
    let decls: Vec<usize> = (0..end)
        .filter(|index| matches!(stmts[*index], Stmt::Decl(_)))
        .collect();
    let mut sites = Vec::new();
    for pair in decls.windows(2) {
        let (Stmt::Decl(a), Stmt::Decl(b)) = (&stmts[pair[0]], &stmts[pair[1]]) else {
            continue;
        };
        let effects_a = stmt_effects(&stmts[pair[0]], env);
        let effects_b = stmt_effects(&stmts[pair[1]], env);
        let near = focused(env, &stmts[pair[0]]) || focused(env, &stmts[pair[1]]);
        if near && !declaration_depends(a, b) && !effects_a.conflicts(&effects_b) {
            sites.push(DeclSite::Swap(pair[0], pair[1]));
        }
    }
    for index in decls {
        if let Stmt::Decl(decl) = &stmts[index] {
            if decl.items.len() > 1
                && decl.items.iter().all(|item| item.init.is_none())
                && focused(env, &stmts[index])
            {
                sites.push(DeclSite::Items(index, decl.items.len()));
            }
        }
    }
    sites
}

fn declaration_apply(stmts: &mut Vec<Stmt>, site: DeclSite, _: &Env, rng: &mut Rng) -> bool {
    match site {
        DeclSite::Swap(a, b) => stmts.swap(a, b),
        DeclSite::Items(index, count) => {
            if let Stmt::Decl(decl) = &mut stmts[index] {
                let first = rng.below(count);
                let second = (first + 1 + rng.below(count - 1)) % count;
                decl.items.swap(first, second);
            }
        }
    }
    true
}

// Temporaries.

fn used_names(function: &Function) -> std::collections::BTreeSet<String> {
    let mut names = std::collections::BTreeSet::new();
    for stmt in &function.body {
        stmt.each(&mut |stmt| {
            if let Stmt::Decl(decl) = stmt {
                names.extend(decl.items.iter().map(|item| item.name.clone()));
            }
        });
        stmt.each_expr(&mut |expr| {
            if let Expr::Ident(name) = expr {
                names.insert(name.clone());
            }
        });
    }
    for param in &function.params {
        names.extend(param.items.iter().map(|item| item.name.clone()));
    }
    names
}

fn fresh_name(function: &Function, env: &Env) -> String {
    let used = used_names(function);
    (1..)
        .map(|n| {
            if n == 1 {
                "tmp".to_string()
            } else {
                format!("tmp{n}")
            }
        })
        .find(|name| {
            !used.contains(name)
                && env.variable(name).is_none()
                && !env.is_typedef(name)
                && !env.is_enum_constant(name)
        })
        .expect("an unused name")
}

/// Candidate subexpressions of a statement's own expression that could be
/// computed into a temporary just before the statement.
fn hoistable(stmt: &mut Stmt, env: &Env, visit: &mut dyn FnMut(&mut Expr) -> bool) -> bool {
    if !focused(env, stmt) {
        return false;
    }
    let whole_value = |expr: &Expr| -> Option<*const Expr> {
        match expr {
            Expr::Assign(None, target, value) if expr_effects(target, env).pure() => {
                Some(value.as_ref() as *const Expr)
            }
            _ => None,
        }
    };
    let (root_expr, whole) = match stmt {
        Stmt::Expr(expr) => {
            let whole = whole_value(expr);
            (expr, whole)
        }
        Stmt::Return(Some(expr)) | Stmt::If(expr, ..) | Stmt::Switch(expr, _) => {
            let whole = Some(&*expr as *const Expr);
            (expr, whole)
        }
        _ => return false,
    };
    let snapshot = root_expr.clone();
    walk_expr(root_expr, root(Sink::Full), env, &mut |expr, ctx| {
        if ctx.role != Role::Value || ctx.guarded {
            return false;
        }
        if matches!(
            expr,
            Expr::Str(_) | Expr::Number(_) | Expr::Char(_) | Expr::Assign(..) | Expr::Comma(..)
        ) {
            return false;
        }
        if let Expr::Ident(name) = expr {
            if env.is_local(name) {
                return false;
            }
        }
        let is_whole = whole.is_some_and(|whole| std::ptr::eq(whole, expr as *const Expr));
        let effects = expr_effects(expr, env);
        if !effects.pure() && !is_whole {
            return false;
        }
        if !is_whole {
            // Whatever else the statement evaluates must not disturb it.
            let rest = rest_effects(&snapshot, expr, env);
            if rest.conflicts(&effects) {
                return false;
            }
        }
        match env.type_of(expr) {
            Some(ty) => {
                let ty = ty.decay();
                if env.spell(&ty).is_none()
                    || ty.is_volatile()
                    || matches!(ty.strip(), CType::Void | CType::Record(_))
                {
                    return false;
                }
            }
            None => return false,
        }
        visit(expr)
    })
}

/// Effects of `whole` other than evaluating `part` and the final store.
fn rest_effects(whole: &Expr, part: &Expr, env: &Env) -> Effects {
    let mut copy = whole.clone();
    let mut replaced = false;
    replace_first(&mut copy, part, &Expr::Number("0".into()), &mut replaced);
    before_store(&copy, env)
}

fn replace_first(expr: &mut Expr, part: &Expr, with: &Expr, replaced: &mut bool) {
    if *replaced {
        return;
    }
    if expr == part {
        *expr = with.clone();
        *replaced = true;
        return;
    }
    match expr {
        Expr::Unary(_, a)
        | Expr::PostInc(a)
        | Expr::PostDec(a)
        | Expr::Member(a, ..)
        | Expr::Cast(_, a)
        | Expr::SizeofExpr(a) => replace_first(a, part, with, replaced),
        Expr::Binary(_, a, b) | Expr::Assign(_, a, b) | Expr::Comma(a, b) | Expr::Index(a, b) => {
            replace_first(a, part, with, replaced);
            replace_first(b, part, with, replaced);
        }
        Expr::Cond(a, b, c) => {
            replace_first(a, part, with, replaced);
            replace_first(b, part, with, replaced);
            replace_first(c, part, with, replaced);
        }
        Expr::Call(callee, arguments) => {
            replace_first(callee, part, with, replaced);
            for argument in arguments {
                replace_first(argument, part, with, replaced);
            }
        }
        _ => {}
    }
}

fn introduce_temporary(function: &mut Function, env: &Env, rng: &mut Rng) -> bool {
    let mut count = 0;
    walk_blocks(&mut function.body, &mut |stmts| {
        let start = leading_declarations(stmts);
        for stmt in &mut stmts[start..] {
            hoistable(stmt, env, &mut |_| {
                count += 1;
                false
            });
        }
        false
    });
    if count == 0 {
        return false;
    }
    let name = fresh_name(function, env);
    let mut target = rng.below(count);
    let mut done = false;
    walk_blocks(&mut function.body, &mut |stmts| {
        let start = leading_declarations(stmts);
        for index in start..stmts.len() {
            let mut hoisted = None;
            let found = hoistable(&mut stmts[index], env, &mut |expr| {
                if target > 0 {
                    target -= 1;
                    return false;
                }
                let ty = env.type_of(expr).map(|ty| ty.decay());
                let value = std::mem::replace(expr, Expr::ident(&name));
                hoisted = Some((value, ty));
                true
            });
            if found {
                let Some((value, Some(ty))) = hoisted else {
                    return true;
                };
                let spelling = env.spell(&ty).expect("spellable temporary");
                let (specs, stars) = split_spelling(&spelling);
                stmts.insert(
                    index,
                    Stmt::Expr(Expr::Assign(
                        None,
                        Box::new(Expr::ident(&name)),
                        Box::new(value),
                    )),
                );
                stmts.insert(
                    start,
                    Stmt::Decl(Decl {
                        specs,
                        items: vec![Declarator {
                            before: vec!["*".to_string(); stars],
                            name: name.clone(),
                            after: Vec::new(),
                            init: None,
                        }],
                    }),
                );
                done = true;
                return true;
            }
        }
        false
    });
    done
}

/// Hoist two same-typed subexpressions of two statements in one block into
/// one temporary assigned just before each, as a programmer reuses a scratch
/// variable. Its two lifetimes never overlap, but they are one variable to
/// the compiler, and local allocation then cannot tie either value to the
/// register of an operand that dies computing it.
fn share_temporary(function: &mut Function, env: &Env, rng: &mut Rng) -> bool {
    // Every hoistable site, by block, statement and ordinal, with its type.
    let mut sites: Vec<(usize, usize, usize, String)> = Vec::new();
    let mut block = 0;
    walk_blocks(&mut function.body, &mut |stmts| {
        let start = leading_declarations(stmts);
        for index in start..stmts.len() {
            let mut ordinal = 0;
            hoistable(&mut stmts[index], env, &mut |expr| {
                if let Some(spelling) = env
                    .type_of(expr)
                    .map(|ty| ty.decay())
                    .and_then(|ty| env.spell(&ty))
                {
                    sites.push((block, index, ordinal, spelling));
                }
                ordinal += 1;
                false
            });
        }
        block += 1;
        false
    });
    let mut pairs = Vec::new();
    for (first, a) in sites.iter().enumerate() {
        for b in &sites[first + 1..] {
            if a.0 == b.0 && a.1 < b.1 && a.3 == b.3 {
                pairs.push((a.clone(), b.clone()));
            }
        }
    }
    if pairs.is_empty() {
        return false;
    }
    let (first, second) = pairs.swap_remove(rng.below(pairs.len()));
    let name = fresh_name(function, env);
    let mut block = 0;
    let mut done = false;
    walk_blocks(&mut function.body, &mut |stmts| {
        if block != first.0 {
            block += 1;
            return false;
        }
        let start = leading_declarations(stmts);
        let mut shared = stmts.clone();
        // The later statement first, so the earlier one keeps its index.
        for (index, ordinal) in [(second.1, second.2), (first.1, first.2)] {
            let mut skip = ordinal;
            let mut hoisted = None;
            hoistable(&mut shared[index], env, &mut |expr| {
                if skip > 0 {
                    skip -= 1;
                    return false;
                }
                hoisted = Some(std::mem::replace(expr, Expr::ident(&name)));
                true
            });
            let Some(value) = hoisted else {
                return true;
            };
            shared.insert(
                index,
                Stmt::Expr(Expr::Assign(
                    None,
                    Box::new(Expr::ident(&name)),
                    Box::new(value),
                )),
            );
        }
        *stmts = shared;
        let (specs, stars) = split_spelling(&first.3);
        stmts.insert(
            start,
            Stmt::Decl(Decl {
                specs,
                items: vec![Declarator {
                    before: vec!["*".to_string(); stars],
                    name: name.clone(),
                    after: Vec::new(),
                    init: None,
                }],
            }),
        );
        done = true;
        true
    });
    done
}

fn split_spelling(spelling: &str) -> (Vec<String>, usize) {
    let stars = spelling.matches('*').count();
    let specs = spelling
        .split(|c: char| c.is_whitespace() || c == '*')
        .filter(|word| !word.is_empty())
        .map(str::to_string)
        .collect();
    (specs, stars)
}

/// A local assigned once and read once later in the same statement list.
#[derive(Clone, Debug)]
struct Single {
    name: String,
}

fn remove_candidates(function: &Function, env: &Env) -> Vec<Single> {
    let mut candidates = Vec::new();
    let mut declared: Vec<(String, bool)> = Vec::new();
    for stmt in &function.body {
        stmt.each(&mut |stmt| {
            if let Stmt::Decl(decl) = stmt {
                let storage = decl
                    .specs
                    .iter()
                    .any(|spec| matches!(spec.as_str(), "static" | "extern" | "typedef"));
                for item in &decl.items {
                    let scalar = item.after.is_empty() && !item.before.iter().any(|t| t == "(");
                    declared.push((item.name.clone(), !storage && scalar));
                }
            }
        });
    }
    for (name, ok) in declared {
        if !ok
            || env.addressed.contains(&name)
            || env.variable(&name).is_some_and(CType::is_volatile)
            || candidates.iter().any(|c: &Single| c.name == name)
            || !mentioned_in_focus(function, env, &name)
        {
            continue;
        }
        let mut probe = function.clone();
        if remove_one(&mut probe, &name, env) {
            candidates.push(Single { name });
        }
    }
    candidates
}

fn remove_temporary(function: &mut Function, env: &Env, rng: &mut Rng) -> bool {
    let candidates = remove_candidates(function, env);
    if candidates.is_empty() {
        return false;
    }
    let chosen = &candidates[rng.below(candidates.len())];
    remove_one(function, &chosen.name, env)
}

fn count_mentions(function: &Function, name: &str) -> usize {
    let mut count = 0;
    for stmt in &function.body {
        stmt.each_expr(&mut |expr| {
            if matches!(expr, Expr::Ident(ident) if ident == name) {
                count += 1;
            }
        });
    }
    count
}

fn is_loop(stmt: &Stmt) -> bool {
    matches!(stmt, Stmt::While(..) | Stmt::DoWhile(..) | Stmt::For(..))
}

/// Whether `name` is used in `stmt` outside any loop and outside nested
/// statements' own control (only its own expressions and plain branches).
fn use_outside_loops(stmt: &Stmt, name: &str) -> bool {
    if is_loop(stmt) {
        return false;
    }
    let own = stmt.own_exprs().iter().any(|expr| expr.mentions(name) > 0);
    if own {
        return true;
    }
    match stmt {
        Stmt::Block(stmts) => stmts.iter().any(|stmt| use_outside_loops(stmt, name)),
        Stmt::If(_, then, other) => {
            use_outside_loops(then, name)
                || other
                    .as_ref()
                    .is_some_and(|other| use_outside_loops(other, name))
        }
        _ => false,
    }
}

fn mentions_stmt(stmt: &Stmt, name: &str) -> usize {
    let mut count = 0;
    stmt.each_expr(&mut |expr| {
        if matches!(expr, Expr::Ident(ident) if ident == name) {
            count += 1;
        }
    });
    count
}

fn declared_type_name(function: &Function, name: &str) -> Option<TypeName> {
    let mut found = None;
    for stmt in &function.body {
        stmt.each(&mut |stmt| {
            if let Stmt::Decl(decl) = stmt {
                for item in &decl.items {
                    if item.name == name {
                        let mut tokens: Vec<String> = decl
                            .specs
                            .iter()
                            .filter(|spec| !matches!(spec.as_str(), "register" | "auto"))
                            .cloned()
                            .collect();
                        tokens.extend(item.before.iter().cloned());
                        found = Some(TypeName { tokens });
                    }
                }
            }
        });
    }
    found
}

/// Replace the single read of `name` with the value it was assigned, and
/// delete the assignment and the declaration.
fn remove_one(function: &mut Function, name: &str, env: &Env) -> bool {
    let total = count_mentions(function, name);
    let declared_type = declared_type_name(function, name);
    let Some(declared_type) = declared_type else {
        return false;
    };
    let var_type = env.variable(name).cloned();
    let mut done = false;
    walk_blocks(&mut function.body, &mut |stmts| {
        // Find the definition in this list.
        let mut definition = None;
        for (index, stmt) in stmts.iter().enumerate() {
            match stmt {
                Stmt::Decl(decl) => {
                    if let Some(item) = decl.items.iter().find(|item| item.name == name) {
                        if let Some(Init::Expr(value)) = &item.init {
                            if total == 1 {
                                definition = Some((index, value.clone(), true));
                            }
                        }
                    }
                }
                Stmt::Expr(Expr::Assign(None, target, value)) if matches!(target.as_ref(), Expr::Ident(t) if t == name) => {
                    if total == 2 && value.mentions(name) == 0 {
                        definition = Some((index, value.as_ref().clone(), false));
                    }
                }
                _ => {}
            }
            if definition.is_some() {
                break;
            }
        }
        let Some((def_index, value, by_init)) = definition else {
            return false;
        };
        let value_effects = expr_effects(&value, env);
        let Some(use_index) =
            (def_index + 1..stmts.len()).find(|index| mentions_stmt(&stmts[*index], name) > 0)
        else {
            return false;
        };
        if !use_outside_loops(&stmts[use_index], name) {
            return true;
        }
        for between in &stmts[def_index + 1..use_index] {
            if matches!(between, Stmt::Label(_) | Stmt::Case(_) | Stmt::Default) {
                return true;
            }
            if value_effects.pure() {
                if stmt_effects(between, env).conflicts(&value_effects) {
                    return true;
                }
            } else if !matches!(between, Stmt::Comment(_) | Stmt::Decl(_)) {
                return true;
            }
        }
        // The use statement's other work must not disturb the value either.
        let use_stmt = &stmts[use_index];
        let mut rest = Effects::default();
        let mut guarded_use = false;
        let mut probe = use_stmt.clone();
        walk_own_probe(&mut probe, env, name, &mut guarded_use);
        for expr in use_stmt.own_exprs() {
            let mut copy = expr.clone();
            let mut replaced = false;
            replace_first(
                &mut copy,
                &Expr::ident(name),
                &Expr::Number("0".into()),
                &mut replaced,
            );
            rest.merge(before_store(&copy, env));
        }
        if !use_stmt
            .own_exprs()
            .iter()
            .any(|expr| expr.mentions(name) > 0)
        {
            // The read is in a nested branch: the whole condition runs first.
            if value_effects.pure() {
                if stmt_effects(use_stmt, env).conflicts(&value_effects) {
                    return true;
                }
            } else {
                return true;
            }
        } else if value_effects.pure() {
            if rest.conflicts(&value_effects) {
                return true;
            }
        } else if use_index != def_index + 1 || !rest.pure() || guarded_use {
            return true;
        }
        // Build the replacement, converting as the assignment did.
        let same = match (&var_type, env.type_of(&value)) {
            (Some(var), Some(value_type)) => var.same(&value_type),
            _ => false,
        };
        let replacement = if same {
            value.clone()
        } else {
            Expr::Cast(declared_type.clone(), Box::new(value.clone()))
        };
        replace_stmt_ident(&mut stmts[use_index], name, &replacement);
        if !by_init {
            stmts.remove(def_index);
        }
        done = true;
        true
    });
    if done {
        walk_blocks(&mut function.body, &mut |stmts| {
            remove_declarator(stmts, name)
        });
    }
    done
}

fn walk_own_probe(stmt: &mut Stmt, env: &Env, name: &str, guarded: &mut bool) {
    walk_own(stmt, env, &mut |expr, ctx| {
        if matches!(expr, Expr::Ident(ident) if ident == name) && ctx.guarded {
            *guarded = true;
        }
        false
    });
}

fn replace_stmt_ident(stmt: &mut Stmt, name: &str, with: &Expr) {
    fn in_expr(expr: &mut Expr, name: &str, with: &Expr) {
        if matches!(expr, Expr::Ident(ident) if ident == name) {
            *expr = with.clone();
            return;
        }
        match expr {
            Expr::Unary(_, a)
            | Expr::PostInc(a)
            | Expr::PostDec(a)
            | Expr::Member(a, ..)
            | Expr::Cast(_, a)
            | Expr::SizeofExpr(a) => in_expr(a, name, with),
            Expr::Binary(_, a, b)
            | Expr::Assign(_, a, b)
            | Expr::Comma(a, b)
            | Expr::Index(a, b) => {
                in_expr(a, name, with);
                in_expr(b, name, with);
            }
            Expr::Cond(a, b, c) => {
                in_expr(a, name, with);
                in_expr(b, name, with);
                in_expr(c, name, with);
            }
            Expr::Call(callee, arguments) => {
                in_expr(callee, name, with);
                arguments.iter_mut().for_each(|a| in_expr(a, name, with));
            }
            _ => {}
        }
    }
    fn in_init(init: &mut Init, name: &str, with: &Expr) {
        match init {
            Init::Expr(expr) => in_expr(expr, name, with),
            Init::List(items) => items.iter_mut().for_each(|item| in_init(item, name, with)),
        }
    }
    match stmt {
        Stmt::Expr(expr)
        | Stmt::If(expr, ..)
        | Stmt::While(expr, _)
        | Stmt::DoWhile(_, expr)
        | Stmt::Switch(expr, _)
        | Stmt::Case(expr)
        | Stmt::Return(Some(expr)) => in_expr(expr, name, with),
        Stmt::For(init, cond, step, _) => {
            for part in [init, cond, step].into_iter().flatten() {
                in_expr(part, name, with);
            }
        }
        Stmt::Decl(decl) => {
            for item in &mut decl.items {
                if let Some(init) = &mut item.init {
                    in_init(init, name, with);
                }
            }
        }
        _ => {}
    }
    match stmt {
        Stmt::Block(stmts) => stmts
            .iter_mut()
            .for_each(|stmt| replace_stmt_ident(stmt, name, with)),
        Stmt::If(_, then, other) => {
            replace_stmt_ident(then, name, with);
            if let Some(other) = other {
                replace_stmt_ident(other, name, with);
            }
        }
        Stmt::While(_, body)
        | Stmt::DoWhile(body, _)
        | Stmt::For(_, _, _, body)
        | Stmt::Switch(_, body) => replace_stmt_ident(body, name, with),
        _ => {}
    }
}

fn remove_declarator(stmts: &mut Vec<Stmt>, name: &str) -> bool {
    for index in 0..stmts.len() {
        if let Stmt::Decl(decl) = &mut stmts[index] {
            if let Some(at) = decl.items.iter().position(|item| item.name == name) {
                decl.items.remove(at);
                if decl.items.is_empty() {
                    stmts.remove(index);
                }
                return true;
            }
        }
    }
    false
}

// Loop forms.

#[derive(Clone, Copy, Debug)]
enum LoopSite {
    ForToWhile(usize),
    WhileToGuardedDo(usize),
    GuardedDoToWhile(usize),
    WhileToFor(usize),
    ForToGuardedDo(usize),
    DoToBreak(usize),
    BreakToDo(usize),
}

fn same_expr(a: &Expr, b: &Expr) -> bool {
    a == b
}

fn guarded_do(stmt: &Stmt) -> Option<(&Expr, &Stmt, &Expr)> {
    let Stmt::If(cond, then, None) = stmt else {
        return None;
    };
    let inner = match then.as_ref() {
        Stmt::Block(items) => {
            let code: Vec<&Stmt> = items
                .iter()
                .filter(|stmt| !matches!(stmt, Stmt::Comment(_)))
                .collect();
            match code.as_slice() {
                [only] => *only,
                _ => return None,
            }
        }
        other => other,
    };
    match inner {
        Stmt::DoWhile(body, again) => Some((cond, body, again)),
        _ => None,
    }
}

fn block_items_of(body: &Stmt) -> Vec<Stmt> {
    match body {
        Stmt::Block(items) => items.clone(),
        Stmt::Empty => Vec::new(),
        other => vec![other.clone()],
    }
}

fn last_code(items: &[Stmt]) -> Option<usize> {
    items
        .iter()
        .rposition(|stmt| !matches!(stmt, Stmt::Comment(_)))
}

fn is_true(expr: &Expr) -> bool {
    matches!(expr, Expr::Number(text) if text != "0" && text.chars().all(|c| c.is_ascii_digit()))
}

fn break_tail(items: &[Stmt]) -> Option<(usize, &Expr)> {
    let last = last_code(items)?;
    match &items[last] {
        Stmt::If(cond, then, None) if matches!(then.as_ref(), Stmt::Break) => Some((last, cond)),
        Stmt::If(cond, then, None) if matches!(then.as_ref(), Stmt::Block(inner) if matches!(inner.as_slice(), [Stmt::Break])) => {
            Some((last, cond))
        }
        _ => None,
    }
}

fn loop_sites(stmts: &[Stmt], env: &Env) -> Vec<LoopSite> {
    let mut sites = Vec::new();
    for (index, stmt) in stmts.iter().enumerate() {
        if !focused(env, stmt) {
            continue;
        }
        match stmt {
            Stmt::For(_, _, _, body) if !has_continue(body) => {
                sites.push(LoopSite::ForToWhile(index));
                sites.push(LoopSite::ForToGuardedDo(index));
            }
            Stmt::While(cond, body) => {
                if !is_true(cond) {
                    sites.push(LoopSite::WhileToGuardedDo(index));
                }
                let items = block_items_of(body);
                if !has_continue(body)
                    && last_code(&items).is_some_and(|last| matches!(items[last], Stmt::Expr(_)))
                {
                    sites.push(LoopSite::WhileToFor(index));
                }
                if is_true(cond) && !has_continue(body) && break_tail(&items).is_some() {
                    sites.push(LoopSite::BreakToDo(index));
                }
            }
            Stmt::DoWhile(body, _) if !has_continue(body) => {
                sites.push(LoopSite::DoToBreak(index));
            }
            _ => {}
        }
        if let Some((cond, _, again)) = guarded_do(stmt) {
            if same_expr(cond, again) {
                sites.push(LoopSite::GuardedDoToWhile(index));
            }
        }
    }
    sites
}

pub fn negate(expr: Expr) -> Expr {
    match expr {
        Expr::Binary(op, left, right) if op.is_comparison() => {
            let flipped = match op {
                BinOp::Eq => BinOp::Ne,
                BinOp::Ne => BinOp::Eq,
                BinOp::Lt => BinOp::Ge,
                BinOp::Ge => BinOp::Lt,
                BinOp::Gt => BinOp::Le,
                _ => BinOp::Gt,
            };
            Expr::Binary(flipped, left, right)
        }
        Expr::Unary(UnOp::Not, operand) => *operand,
        other => Expr::Unary(UnOp::Not, Box::new(other)),
    }
}

fn with_step(body: &Stmt, step: Option<&Expr>) -> Stmt {
    let mut items = block_items_of(body);
    if let Some(step) = step {
        items.push(Stmt::Expr(step.clone()));
    }
    Stmt::Block(items)
}

fn loop_apply(stmts: &mut Vec<Stmt>, site: LoopSite, _: &Env, _: &mut Rng) -> bool {
    match site {
        LoopSite::ForToWhile(index) | LoopSite::ForToGuardedDo(index) => {
            let Stmt::For(init, cond, step, body) = stmts[index].clone() else {
                return false;
            };
            let cond = cond.unwrap_or(Expr::Number("1".into()));
            let body = with_step(&body, step.as_ref());
            let replacement = if matches!(site, LoopSite::ForToWhile(_)) {
                Stmt::While(cond, Box::new(body))
            } else {
                Stmt::If(
                    cond.clone(),
                    Box::new(Stmt::Block(vec![Stmt::DoWhile(Box::new(body), cond)])),
                    None,
                )
            };
            stmts[index] = replacement;
            if let Some(init) = init {
                stmts.insert(index, Stmt::Expr(init));
            }
        }
        LoopSite::WhileToGuardedDo(index) => {
            let Stmt::While(cond, body) = stmts[index].clone() else {
                return false;
            };
            let body = Stmt::Block(block_items_of(&body));
            stmts[index] = Stmt::If(
                cond.clone(),
                Box::new(Stmt::Block(vec![Stmt::DoWhile(Box::new(body), cond)])),
                None,
            );
        }
        LoopSite::GuardedDoToWhile(index) => {
            let Some((cond, body, _)) = guarded_do(&stmts[index]) else {
                return false;
            };
            stmts[index] = Stmt::While(cond.clone(), Box::new(Stmt::Block(block_items_of(body))));
        }
        LoopSite::WhileToFor(index) => {
            let Stmt::While(cond, body) = stmts[index].clone() else {
                return false;
            };
            let mut items = block_items_of(&body);
            let Some(last) = last_code(&items) else {
                return false;
            };
            let Stmt::Expr(step) = items.remove(last) else {
                return false;
            };
            // Take the preceding assignment as the initializer when it sets
            // something the condition reads.
            let init = index
                .checked_sub(1)
                .and_then(|previous| match &stmts[previous] {
                    Stmt::Expr(expr @ Expr::Assign(_, target, _)) => root_name(target)
                        .filter(|name| cond.mentions(name) > 0)
                        .map(|_| (previous, expr.clone())),
                    _ => None,
                });
            let cond = if is_true(&cond) { None } else { Some(cond) };
            match init {
                Some((previous, init)) => {
                    stmts[index] =
                        Stmt::For(Some(init), cond, Some(step), Box::new(Stmt::Block(items)));
                    stmts.remove(previous);
                }
                None => {
                    stmts[index] = Stmt::For(None, cond, Some(step), Box::new(Stmt::Block(items)));
                }
            }
        }
        LoopSite::DoToBreak(index) => {
            let Stmt::DoWhile(body, cond) = stmts[index].clone() else {
                return false;
            };
            let mut items = block_items_of(&body);
            items.push(Stmt::If(negate(cond), Box::new(Stmt::Break), None));
            stmts[index] = Stmt::While(Expr::Number("1".into()), Box::new(Stmt::Block(items)));
        }
        LoopSite::BreakToDo(index) => {
            let Stmt::While(_, body) = stmts[index].clone() else {
                return false;
            };
            let mut items = block_items_of(&body);
            let Some((last, cond)) = break_tail(&items).map(|(last, cond)| (last, cond.clone()))
            else {
                return false;
            };
            items.remove(last);
            stmts[index] = Stmt::DoWhile(Box::new(Stmt::Block(items)), negate(cond));
        }
    }
    true
}

// Assignments in conditions.

#[derive(Clone, Copy, Debug)]
enum ConditionSite {
    Into(usize),
    OutOf(usize),
}

/// A call statement, or the assignment of a call's result: its arguments
/// can take an assignment the way a condition can.
fn call_statement(expr: &Expr) -> bool {
    match expr {
        Expr::Call(..) => true,
        Expr::Assign(_, _, value) => matches!(value.as_ref(), Expr::Call(..)),
        _ => false,
    }
}

fn condition_of(stmt: &Stmt) -> Option<&Expr> {
    match stmt {
        Stmt::If(cond, ..) | Stmt::Switch(cond, _) => Some(cond),
        Stmt::Expr(expr) if call_statement(expr) => Some(expr),
        _ => None,
    }
}

fn condition_of_mut(stmt: &mut Stmt) -> Option<&mut Expr> {
    match stmt {
        Stmt::If(cond, ..) | Stmt::Switch(cond, _) => Some(cond),
        Stmt::Expr(expr) if call_statement(expr) => Some(expr),
        _ => None,
    }
}

/// Where an assignment can sit embedded: a condition, or a call. A call's
/// result assignment is the statement itself, so only its call counts.
fn embedding_of(stmt: &Stmt) -> Option<&Expr> {
    match condition_of(stmt)? {
        Expr::Assign(_, _, value) if matches!(stmt, Stmt::Expr(_)) => Some(value),
        cond => Some(cond),
    }
}

fn embedding_of_mut(stmt: &mut Stmt) -> Option<&mut Expr> {
    if matches!(stmt, Stmt::Expr(Expr::Assign(..))) {
        let Some(Expr::Assign(_, _, value)) = condition_of_mut(stmt) else {
            return None;
        };
        return Some(value);
    }
    condition_of_mut(stmt)
}

/// The only unguarded, evaluated occurrence of `name` in `cond`.
fn single_use(cond: &Expr, name: &str, env: &Env) -> bool {
    if cond.mentions(name) != 1 {
        return false;
    }
    let mut ok = false;
    let mut copy = cond.clone();
    walk_expr(&mut copy, root(Sink::Truth), env, &mut |expr, ctx| {
        if matches!(expr, Expr::Ident(ident) if ident == name) {
            ok = ctx.role == Role::Value && !ctx.guarded;
            return true;
        }
        false
    });
    ok
}

fn embedded_assignment(cond: &Expr, env: &Env) -> Option<(String, Expr)> {
    let mut found = Vec::new();
    let mut copy = cond.clone();
    walk_expr(&mut copy, root(Sink::Truth), env, &mut |expr, ctx| {
        if let Expr::Assign(None, target, value) = expr {
            if let Expr::Ident(name) = target.as_ref() {
                found.push((name.clone(), value.as_ref().clone(), ctx.guarded));
            }
        }
        false
    });
    let [(name, value, false)] = found.as_slice() else {
        return None;
    };
    (cond.mentions(name) == 1 && env.is_local(name)).then(|| (name.clone(), value.clone()))
}

fn condition_sites(stmts: &[Stmt], env: &Env) -> Vec<ConditionSite> {
    let mut sites = Vec::new();
    for index in 0..stmts.len() {
        let near = focused(env, &stmts[index])
            || stmts.get(index + 1).is_some_and(|next| focused(env, next));
        if !near {
            continue;
        }
        if let (Stmt::Expr(Expr::Assign(None, target, value)), Some(next)) =
            (&stmts[index], stmts.get(index + 1).and_then(condition_of))
        {
            if let Expr::Ident(name) = target.as_ref() {
                let value_effects = expr_effects(value, env);
                let mut copy = next.clone();
                let mut replaced = false;
                replace_first(
                    &mut copy,
                    &Expr::ident(name),
                    &Expr::Number("0".into()),
                    &mut replaced,
                );
                let rest = expr_effects(&copy, env);
                if env.is_local(name)
                    && !matches!(value.as_ref(), Expr::Number(_) | Expr::Char(_))
                    && value.mentions(name) == 0
                    && single_use(next, name, env)
                    && !rest.conflicts(&value_effects)
                    && (value_effects.pure() || rest.pure())
                {
                    sites.push(ConditionSite::Into(index));
                }
            }
        }
        if let Some(cond) = embedding_of(&stmts[index]) {
            if let Some((name, value)) = embedded_assignment(cond, env) {
                let mut copy = cond.clone();
                let mut replaced = false;
                let assignment =
                    Expr::Assign(None, Box::new(Expr::ident(&name)), Box::new(value.clone()));
                replace_first(
                    &mut copy,
                    &assignment,
                    &Expr::Number("0".into()),
                    &mut replaced,
                );
                let rest = expr_effects(&copy, env);
                let value_effects = expr_effects(&value, env);
                if replaced
                    && !rest.conflicts(&value_effects)
                    && (value_effects.pure() || rest.pure())
                {
                    sites.push(ConditionSite::OutOf(index));
                }
            }
        }
    }
    sites
}

fn condition_apply(stmts: &mut Vec<Stmt>, site: ConditionSite, env: &Env, _: &mut Rng) -> bool {
    match site {
        ConditionSite::Into(index) => {
            let Stmt::Expr(assignment) = stmts.remove(index) else {
                return false;
            };
            let Expr::Assign(None, target, _) = &assignment else {
                return false;
            };
            let Some(cond) = condition_of_mut(&mut stmts[index]) else {
                return false;
            };
            let mut replaced = false;
            let target = target.as_ref().clone();
            replace_first(cond, &target, &assignment, &mut replaced);
            replaced
        }
        ConditionSite::OutOf(index) => {
            let Some((name, value)) =
                embedding_of(&stmts[index]).and_then(|cond| embedded_assignment(cond, env))
            else {
                return false;
            };
            let assignment = Expr::Assign(None, Box::new(Expr::ident(&name)), Box::new(value));
            let Some(cond) = embedding_of_mut(&mut stmts[index]) else {
                return false;
            };
            let mut replaced = false;
            replace_first(cond, &assignment, &Expr::ident(&name), &mut replaced);
            if replaced {
                stmts.insert(index, Stmt::Expr(assignment));
            }
            replaced
        }
    }
}

// register.

fn register_sites(stmts: &[Stmt], env: &Env) -> Vec<usize> {
    (0..leading_declarations(stmts))
        .filter(|index| match &stmts[*index] {
            Stmt::Decl(decl) => {
                focused(env, &stmts[*index])
                    && !decl
                        .specs
                        .iter()
                        .any(|spec| matches!(spec.as_str(), "static" | "extern" | "typedef"))
            }
            _ => false,
        })
        .collect()
}

fn register_apply(stmts: &mut Vec<Stmt>, index: usize, _: &Env, _: &mut Rng) -> bool {
    let Stmt::Decl(decl) = &mut stmts[index] else {
        return false;
    };
    if let Some(at) = decl.specs.iter().position(|spec| spec == "register") {
        decl.specs.remove(at);
    } else {
        decl.specs.insert(0, "register".into());
    }
    true
}

// if/else inversion.

fn invert_sites(stmts: &[Stmt], env: &Env) -> Vec<usize> {
    (0..stmts.len())
        .filter(|index| {
            matches!(&stmts[*index], Stmt::If(_, _, Some(_))) && focused(env, &stmts[*index])
        })
        .collect()
}

fn braced(stmt: Stmt) -> Stmt {
    match stmt {
        Stmt::Block(_) => stmt,
        other => Stmt::Block(vec![other]),
    }
}

fn invert_apply(stmts: &mut Vec<Stmt>, index: usize, _: &Env, _: &mut Rng) -> bool {
    let Stmt::If(cond, then, Some(other)) = stmts[index].clone() else {
        return false;
    };
    stmts[index] = Stmt::If(
        negate(cond),
        Box::new(braced(*other)),
        Some(Box::new(braced(*then))),
    );
    true
}

/// Constructs no programmer would write for their own sake: casts to the
/// operand's own type, casts of casts and `(*p).member`.
pub fn unnatural(function: &Function, env: &Env) -> usize {
    let mut count = 0;
    for stmt in &function.body {
        stmt.each_expr(&mut |expr| match expr {
            Expr::Cast(name, operand) => {
                if is_cast(operand) {
                    count += 1;
                } else if let Some(ty) = env.type_of(operand) {
                    if env.type_name(name).same(&ty) && !matches!(operand.as_ref(), Expr::Number(_))
                    {
                        count += 1;
                    }
                }
            }
            Expr::Member(base, _, false)
                if matches!(base.as_ref(), Expr::Unary(UnOp::Deref, _)) =>
            {
                count += 1
            }
            _ => {}
        });
    }
    count
}

#[cfg(test)]
mod tests {
    use super::super::parse::{locate, scan_unit};
    use super::*;

    const HEADER: &str = "typedef signed char s8;\ntypedef unsigned char u8;\ntypedef signed short s16;\ntypedef unsigned short u16;\ntypedef signed int s32;\ntypedef unsigned int u32;\nstruct Unit { u16 hp; u16 max; u8 flags; };\nextern struct Unit *gUnit;\nextern s32 gX;\nextern s32 gY;\nextern u16 gTable[8];\ns32 Rand(void);\nvoid Use(s32);\ns32 Use2(s32);\n";

    fn load(body: &str) -> (Function, Env) {
        let source = format!("{HEADER}s32 F(s32 a, s32 b)\n{{\n{body}\n}}\n");
        let unit = scan_unit(HEADER).unwrap();
        let located = locate(&source, Some("F"), &unit.typedef_names()).unwrap();
        let env = Env::new(&unit, &located.function);
        (located.function, env)
    }

    fn body(function: &Function) -> String {
        let printed = function.print();
        let start = printed.find("{\n").unwrap() + 2;
        let end = printed.rfind('}').unwrap();
        printed[start..end]
            .lines()
            .map(str::trim)
            .filter(|line| !line.is_empty())
            .collect::<Vec<_>>()
            .join(" ")
    }

    /// Every distinct result one mutation kind can produce from `body`.
    fn outcomes(kind: Kind, source: &str) -> Vec<String> {
        let (function, env) = load(source);
        let mut seen = std::collections::BTreeSet::new();
        for seed in 0..200 {
            let mut candidate = function.clone();
            let mut rng = Rng::new(seed);
            if apply(kind, &mut candidate, &env, &mut rng) {
                seen.insert(body(&candidate));
            }
        }
        seen.into_iter().collect()
    }

    #[test]
    fn generator_is_deterministic_per_seed() {
        let mut a = Rng::new(7);
        let mut b = Rng::new(7);
        let first: Vec<u64> = (0..4).map(|_| a.next()).collect();
        assert_eq!(first, (0..4).map(|_| b.next()).collect::<Vec<_>>());
        assert_ne!(
            first,
            (0..4).map(|_| Rng::new(8).next()).collect::<Vec<_>>()
        );
        let (function, mut env) = load("s32 x;\nx = a + b;\ngX = x;\ngY = b;\nreturn x;");
        let run = |seed| {
            let mut candidate = function.clone();
            let mut env = Env::new(&scan_unit(HEADER).unwrap(), &candidate);
            let mut rng = Rng::new(seed);
            let kinds: Vec<Kind> = (0..6)
                .filter_map(|_| mutate(&mut candidate, &mut env, &mut rng))
                .collect();
            (body(&candidate), kinds)
        };
        assert_eq!(run(3), run(3));
        assert!(mutate(&mut function.clone(), &mut env, &mut Rng::new(1)).is_some());
    }

    #[test]
    fn commutative_operands_swap_and_comparisons_mirror() {
        let found = outcomes(Kind::SwapOperands, "return (a + b) < gX;");
        assert!(
            found.contains(&"return b + a < gX;".to_string()),
            "{found:?}"
        );
        assert!(
            found.contains(&"return gX > a + b;".to_string()),
            "{found:?}"
        );
        // Two calls may not trade places.
        assert!(outcomes(Kind::SwapOperands, "return Rand() + Rand();").is_empty());
        // Division is not commutative.
        assert!(outcomes(Kind::SwapOperands, "return a / b;").is_empty());
        // Associative operators regroup.
        let found = outcomes(Kind::SwapOperands, "return a + b + gX;");
        assert!(
            found.contains(&"return a + (b + gX);".to_string()),
            "{found:?}"
        );
    }

    #[test]
    fn truth_tests_and_zero_comparisons_trade_places() {
        let found = outcomes(
            Kind::ZeroTest,
            "if (a != 0 && !b) return 1;\nreturn gUnit == 0;",
        );
        for expected in [
            "if (a && !b) return 1; return gUnit == 0;",
            "if (a != 0 && b == 0) return 1; return gUnit == 0;",
        ] {
            assert!(
                found.contains(&expected.to_string()),
                "{expected}: {found:?}"
            );
        }
        // A returned comparison is a value, not a truth test.
        assert!(
            !found.iter().any(|body| body.contains("return !gUnit")),
            "{found:?}"
        );
    }

    #[test]
    fn statements_reorder_only_when_independent() {
        let found = outcomes(Kind::ReorderStatements, "gX = a;\ngY = b;\nreturn 0;");
        assert_eq!(found, vec!["gY = b; gX = a; return 0;".to_string()]);
        // A read after a write of the same global stays put.
        assert!(outcomes(Kind::ReorderStatements, "gX = a;\ngY = gX;\nreturn 0;").is_empty());
        // Calls may touch any global.
        assert!(outcomes(Kind::ReorderStatements, "gX = a;\nUse(b);\nreturn 0;").is_empty());
        // Stores through pointers may alias globals.
        assert!(outcomes(
            Kind::ReorderStatements,
            "gUnit->hp = a;\ngY = b;\nreturn 0;"
        )
        .is_empty());
        // Locals are exact.
        let found = outcomes(
            Kind::ReorderStatements,
            "s32 x;\ns32 y;\nx = a;\ny = b;\nreturn x + y;",
        );
        assert!(
            found.contains(&"s32 x; s32 y; y = b; x = a; return x + y;".to_string()),
            "{found:?}"
        );
        // Nothing moves across a return.
        assert!(outcomes(
            Kind::ReorderStatements,
            "gX = a;\nif (b) return 1;\ngY = b;\nreturn 0;"
        )
        .iter()
        .all(|body| body.starts_with("gX = a; if (b)")));
    }

    #[test]
    fn declarations_reorder_unless_initializers_depend() {
        let found = outcomes(
            Kind::ReorderDeclarations,
            "s32 x;\nu16 y;\nx = a;\ny = b;\nreturn x + y;",
        );
        assert!(
            found.contains(&"u16 y; s32 x; x = a; y = b; return x + y;".to_string()),
            "{found:?}"
        );
        assert!(outcomes(
            Kind::ReorderDeclarations,
            "s32 x = a;\ns32 y = x;\nreturn y;"
        )
        .is_empty());
        let found = outcomes(
            Kind::ReorderDeclarations,
            "s32 x, y;\nx = a;\ny = b;\nreturn x - y;",
        );
        assert!(
            found.contains(&"s32 y, x; x = a; y = b; return x - y;".to_string()),
            "{found:?}"
        );
    }

    #[test]
    fn temporaries_are_typed_introduced_and_removed() {
        let found = outcomes(Kind::IntroduceTemporary, "gX = gUnit->hp + b;\nreturn 0;");
        assert!(
            found.contains(&"u16 tmp; tmp = gUnit->hp; gX = tmp + b; return 0;".to_string()),
            "{found:?}"
        );
        assert!(
            found.contains(&"s32 tmp; tmp = gUnit->hp + b; gX = tmp; return 0;".to_string()),
            "{found:?}"
        );
        // A call may become a temporary only as the whole value.
        let found = outcomes(Kind::IntroduceTemporary, "gX = Rand() + 1;\nreturn 0;");
        assert!(
            found.contains(&"s32 tmp; tmp = Rand() + 1; gX = tmp; return 0;".to_string()),
            "{found:?}"
        );
        assert!(
            !found.iter().any(|body| body.contains("tmp = Rand();")),
            "{found:?}"
        );
        // Nothing is hoisted out of the guarded side of &&.
        let found = outcomes(
            Kind::IntroduceTemporary,
            "if (gUnit != 0 && gUnit->hp) return 1;\nreturn 0;",
        );
        assert!(
            !found.iter().any(|body| body.contains("tmp = gUnit->hp")),
            "{found:?}"
        );

        let found = outcomes(
            Kind::RemoveTemporary,
            "u16 t;\nt = gUnit->hp;\ngX = t + b;\nreturn 0;",
        );
        assert_eq!(found, vec!["gX = gUnit->hp + b; return 0;".to_string()]);
        let found = outcomes(Kind::RemoveTemporary, "u8 t;\nt = a;\nreturn t;");
        assert_eq!(found, vec!["return (u8)a;".to_string()]);
        // A store between the definition and the use can change the value.
        assert!(outcomes(
            Kind::RemoveTemporary,
            "u16 t;\nt = gUnit->hp;\ngUnit->hp = 0;\nreturn t;"
        )
        .is_empty());
        // A read inside a loop would repeat the computation.
        assert!(outcomes(
            Kind::RemoveTemporary,
            "s32 t;\nt = gX;\nwhile (b--) Use(t);\nreturn 0;"
        )
        .is_empty());
        // A call moves only into the next statement, unguarded.
        let found = outcomes(
            Kind::RemoveTemporary,
            "s32 t;\nt = Rand();\nif (t == 3) return 1;\nreturn 0;",
        );
        assert_eq!(
            found,
            vec!["if (Rand() == 3) return 1; return 0;".to_string()]
        );
    }

    #[test]
    fn one_temporary_serves_two_statements() {
        let found = outcomes(
            Kind::ShareTemporary,
            "Use(a + 49);\nUse(a + 32);\nreturn 0;",
        );
        assert_eq!(
            found,
            vec!["s32 tmp; tmp = a + 49; Use(tmp); tmp = a + 32; Use(tmp); return 0;".to_string()]
        );
        // Only values of one type share it, and only across statements.
        assert!(outcomes(
            Kind::ShareTemporary,
            "gX = gUnit->hp;\nUse(a + 1);\nreturn 0;"
        )
        .is_empty());
        assert!(outcomes(Kind::ShareTemporary, "Use((a + 1) * (b + 1));\nreturn 0;").is_empty());
    }

    #[test]
    fn same_width_casts_respect_how_the_value_is_used() {
        let found = outcomes(Kind::AddCast, "gTable[0] = gUnit->hp;\nreturn 0;");
        assert!(
            found.contains(&"gTable[0] = (s16)gUnit->hp; return 0;".to_string()),
            "{found:?}"
        );
        // A 16-bit value widened into a 32-bit store must keep its sign.
        let found = outcomes(Kind::AddCast, "gX = gUnit->hp;\nreturn 0;");
        assert!(
            !found.iter().any(|body| body.contains("(s16)gUnit->hp")),
            "{found:?}"
        );
        assert!(
            found.contains(&"gX = (u16)gUnit->hp; return 0;".to_string()),
            "{found:?}"
        );
        // Right shifts see the sign of 32-bit values.
        let found = outcomes(Kind::AddCast, "return a >> b;");
        assert!(
            !found.iter().any(|body| body.contains("(u32)a")),
            "{found:?}"
        );
        let found = outcomes(Kind::AddCast, "return a + b;");
        assert!(
            found.contains(&"return (u32)a + b;".to_string()),
            "{found:?}"
        );

        let found = outcomes(
            Kind::DropCast,
            "gTable[0] = (s16)gUnit->hp;\nreturn (s32)a;",
        );
        assert!(
            found.contains(&"gTable[0] = gUnit->hp; return (s32)a;".to_string()),
            "{found:?}"
        );
        assert!(
            found.contains(&"gTable[0] = (s16)gUnit->hp; return a;".to_string()),
            "{found:?}"
        );
        assert!(outcomes(Kind::DropCast, "gX = (s16)gUnit->hp;\nreturn 0;").is_empty());
        assert!(outcomes(Kind::DropCast, "return (u8)a;").is_empty());
    }

    #[test]
    fn loops_change_form_without_changing_iterations() {
        let found = outcomes(
            Kind::LoopForm,
            "s32 i;\nfor (i = 0; i < a; i++) Use(i);\nreturn 0;",
        );
        assert!(
            found.contains(&"s32 i; i = 0; while (i < a) { Use(i); i++; } return 0;".to_string()),
            "{found:?}"
        );
        assert!(
            found.contains(
                &"s32 i; i = 0; if (i < a) { do { Use(i); i++; } while (i < a); } return 0;"
                    .to_string()
            ),
            "{found:?}"
        );
        // continue would skip the moved step.
        assert!(outcomes(
            Kind::LoopForm,
            "s32 i;\nfor (i = 0; i < a; i++) { if (i == b) continue; Use(i); }\nreturn 0;"
        )
        .is_empty());
        let found = outcomes(Kind::LoopForm, "while (a < b) a++;\nreturn a;");
        assert!(
            found.contains(&"if (a < b) { do { a++; } while (a < b); } return a;".to_string()),
            "{found:?}"
        );
        assert!(
            found.contains(&"for (; a < b; a++) { } return a;".to_string()),
            "{found:?}"
        );
        let found = outcomes(
            Kind::LoopForm,
            "if (a < b) { do { a++; } while (a < b); }\nreturn a;",
        );
        assert!(
            found.contains(&"while (a < b) { a++; } return a;".to_string()),
            "{found:?}"
        );
        let found = outcomes(Kind::LoopForm, "do { a++; } while (a < b);\nreturn a;");
        assert_eq!(
            found,
            vec!["while (1) { a++; if (a >= b) break; } return a;".to_string()]
        );
        let found = outcomes(
            Kind::LoopForm,
            "while (1) { a++; if (a >= b) break; }\nreturn a;",
        );
        assert!(
            found.contains(&"do { a++; } while (a < b); return a;".to_string()),
            "{found:?}"
        );
    }

    #[test]
    fn pointer_forms_are_equivalent_spellings() {
        let found = outcomes(Kind::PointerIndex, "return gTable[a] + gUnit->hp;");
        assert!(
            found.contains(&"return *(gTable + a) + gUnit->hp;".to_string()),
            "{found:?}"
        );
        assert!(
            found.contains(&"return gTable[a] + (*gUnit).hp;".to_string()),
            "{found:?}"
        );
        let found = outcomes(Kind::PointerIndex, "return *(gTable + a) + *gTable;");
        assert!(
            found.contains(&"return gTable[a] + *gTable;".to_string()),
            "{found:?}"
        );
        assert!(
            found.contains(&"return *(gTable + a) + gTable[0];".to_string()),
            "{found:?}"
        );
        let found = outcomes(Kind::PointerIndex, "Use((s32)&gTable[a]);\nreturn 0;");
        assert!(
            found.contains(&"Use((s32)(gTable + a)); return 0;".to_string()),
            "{found:?}"
        );
    }

    #[test]
    fn compound_assignments_split_and_join() {
        let found = outcomes(
            Kind::CompoundAssignment,
            "gX += a;\ngUnit->hp = gUnit->hp - 1;\nb++;\nreturn b;",
        );
        for expected in [
            "gX = gX + a; gUnit->hp = gUnit->hp - 1; b++; return b;",
            "gX += a; gUnit->hp -= 1; b++; return b;",
            "gX += a; gUnit->hp = gUnit->hp - 1; b += 1; return b;",
            "gX += a; gUnit->hp = gUnit->hp - 1; ++b; return b;",
        ] {
            assert!(
                found.contains(&expected.to_string()),
                "{expected}: {found:?}"
            );
        }
        // A used increment keeps its form.
        assert!(outcomes(Kind::CompoundAssignment, "return b++;").is_empty());
        // The target is evaluated twice when split; it must be pure.
        assert!(outcomes(Kind::CompoundAssignment, "gTable[Rand()] += 1;\nreturn 0;").is_empty());
    }

    #[test]
    fn assignments_move_into_and_out_of_conditions() {
        let found = outcomes(
            Kind::ConditionAssignment,
            "s32 x;\nx = Rand();\nif (x == 3) return 1;\nreturn x;",
        );
        assert_eq!(
            found,
            vec!["s32 x; if ((x = Rand()) == 3) return 1; return x;".to_string()]
        );
        let found = outcomes(
            Kind::ConditionAssignment,
            "s32 x;\nif ((x = Rand()) != 0) return x;\nreturn 0;",
        );
        assert_eq!(
            found,
            vec!["s32 x; x = Rand(); if (x != 0) return x; return 0;".to_string()]
        );
        // Reading the variable twice in the condition would be unsequenced.
        assert!(outcomes(
            Kind::ConditionAssignment,
            "s32 x;\nx = Rand();\nif (x == x + 1) return 1;\nreturn 0;"
        )
        .is_empty());
        // A call's argument takes the assignment as a condition does.
        let found = outcomes(
            Kind::ConditionAssignment,
            "s32 x;\nx = a + 1;\nUse(x);\nreturn x;",
        );
        assert_eq!(found, vec!["s32 x; Use(x = a + 1); return x;".to_string()]);
        let found = outcomes(
            Kind::ConditionAssignment,
            "s32 x;\ngX = Use2(x = a + 1);\nreturn x;",
        );
        assert_eq!(
            found,
            vec!["s32 x; x = a + 1; gX = Use2(x); return x;".to_string()]
        );
        // The result assignment of a call is the statement, not embedded.
        assert!(outcomes(Kind::ConditionAssignment, "gX = Rand();\nreturn 0;").is_empty());
    }

    #[test]
    fn register_toggles_and_if_else_inverts() {
        assert_eq!(
            outcomes(Kind::Register, "s32 x;\nx = a;\nreturn x;"),
            vec!["register s32 x; x = a; return x;".to_string()]
        );
        assert_eq!(
            outcomes(Kind::InvertIf, "if (a < b) gX = 1; else gY = 1;\nreturn 0;"),
            vec!["if (a >= b) { gY = 1; } else { gX = 1; } return 0;".to_string()]
        );
    }

    #[test]
    fn focus_limits_rewrites_to_matching_statements() {
        let (function, mut env) = load("gX = gUnit->hp + b;\ngY = gUnit->max + a;\nreturn 0;");
        env.focus = Some(regex::Regex::new("gY").unwrap());
        let mut seen = std::collections::BTreeSet::new();
        for seed in 0..200 {
            let mut candidate = function.clone();
            if apply(
                Kind::IntroduceTemporary,
                &mut candidate,
                &env,
                &mut Rng::new(seed),
            ) {
                seen.insert(body(&candidate));
            }
        }
        assert!(!seen.is_empty());
        assert!(
            seen.iter().all(|body| body.contains("gX = gUnit->hp + b;")),
            "{seen:?}"
        );
    }

    #[test]
    fn unnatural_constructs_are_counted() {
        let (function, env) = load("return (s32)a + (u16)(u8)b + (*gUnit).hp + (u32)a;");
        assert_eq!(unnatural(&function, &env), 3);
    }
}
