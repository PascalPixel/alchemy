//! What an expression or statement reads and writes, for deciding whether two
//! pieces of code may trade places. Named locals are exact; globals are named
//! but may alias memory reached through pointers; a call touches everything.

use super::ast::*;
use super::types::{CType, Env};
use std::collections::BTreeSet;

#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub struct Effects {
    pub reads: BTreeSet<String>,
    pub writes: BTreeSet<String>,
    pub global_reads: BTreeSet<String>,
    pub global_writes: BTreeSet<String>,
    pub memory_read: bool,
    pub memory_write: bool,
    pub call: bool,
    pub volatile: bool,
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum Mode {
    Read,
    Write,
    ReadWrite,
    Address,
}

impl Effects {
    /// No writes, calls or volatile accesses: evaluating it twice, or not at
    /// all, is unobservable.
    pub fn pure(&self) -> bool {
        self.writes.is_empty()
            && self.global_writes.is_empty()
            && !self.memory_write
            && !self.call
            && !self.volatile
    }

    fn touches_memory(&self) -> bool {
        self.memory_read
            || self.memory_write
            || self.call
            || !self.global_reads.is_empty()
            || !self.global_writes.is_empty()
    }

    pub fn merge(&mut self, other: Effects) {
        self.reads.extend(other.reads);
        self.writes.extend(other.writes);
        self.global_reads.extend(other.global_reads);
        self.global_writes.extend(other.global_writes);
        self.memory_read |= other.memory_read;
        self.memory_write |= other.memory_write;
        self.call |= other.call;
        self.volatile |= other.volatile;
    }

    /// Whether running `self` and `other` in the other order could differ.
    pub fn conflicts(&self, other: &Effects) -> bool {
        fn one_way(a: &Effects, b: &Effects) -> bool {
            if a.call && b.touches_memory() {
                return true;
            }
            if a.memory_write && b.touches_memory() {
                return true;
            }
            if !a.global_writes.is_empty() && (b.memory_read || b.memory_write) {
                return true;
            }
            if a.global_writes
                .iter()
                .any(|name| b.global_reads.contains(name) || b.global_writes.contains(name))
            {
                return true;
            }
            a.writes
                .iter()
                .any(|name| b.reads.contains(name) || b.writes.contains(name))
        }
        (self.volatile && other.volatile) || one_way(self, other) || one_way(other, self)
    }
}

pub fn expr_effects(expr: &Expr, env: &Env) -> Effects {
    let mut effects = Effects::default();
    walk(expr, env, Mode::Read, &mut effects);
    effects
}

/// The effects of evaluating an assignment's value and target, without the
/// store itself: what happens before the stored value is known.
pub fn before_store(expr: &Expr, env: &Env) -> Effects {
    match expr {
        Expr::Assign(None, target, value) => {
            let mut effects = expr_effects(value, env);
            walk(target, env, Mode::Address, &mut effects);
            effects
        }
        other => expr_effects(other, env),
    }
}

pub fn stmt_effects(stmt: &Stmt, env: &Env) -> Effects {
    let mut effects = Effects::default();
    stmt.each(&mut |stmt| {
        if let Stmt::Decl(decl) = stmt {
            for item in &decl.items {
                if item.init.is_some() {
                    effects.writes.insert(item.name.clone());
                }
            }
        }
        for expr in stmt.own_exprs() {
            walk(expr, env, Mode::Read, &mut effects);
        }
    });
    effects
}

fn volatile_type(env: &Env, expr: &Expr) -> bool {
    env.type_of(expr).is_some_and(|ty| ty.is_volatile())
}

fn access(effects: &mut Effects, mode: Mode, local: bool, name: &str) {
    let (reads, writes) = if local {
        (&mut effects.reads, &mut effects.writes)
    } else {
        (&mut effects.global_reads, &mut effects.global_writes)
    };
    match mode {
        Mode::Read => {
            reads.insert(name.to_string());
        }
        Mode::Write => {
            writes.insert(name.to_string());
        }
        Mode::ReadWrite => {
            reads.insert(name.to_string());
            writes.insert(name.to_string());
        }
        Mode::Address => {}
    }
}

fn memory(effects: &mut Effects, mode: Mode) {
    match mode {
        Mode::Read => effects.memory_read = true,
        Mode::Write => effects.memory_write = true,
        Mode::ReadWrite => {
            effects.memory_read = true;
            effects.memory_write = true;
        }
        Mode::Address => {}
    }
}

fn walk(expr: &Expr, env: &Env, mode: Mode, effects: &mut Effects) {
    if mode != Mode::Address
        && matches!(
            expr,
            Expr::Ident(_) | Expr::Member(..) | Expr::Index(..) | Expr::Unary(UnOp::Deref, _)
        )
        && volatile_type(env, expr)
    {
        effects.volatile = true;
    }
    match expr {
        Expr::Ident(name) => {
            if env.is_enum_constant(name) {
                return;
            }
            if let Some(CType::Func(..)) = env.variable(name).map(CType::strip) {
                return;
            }
            let local = env.is_local(name) && !env.addressed.contains(name);
            access(effects, mode, local, name);
        }
        Expr::Number(_)
        | Expr::Char(_)
        | Expr::Str(_)
        | Expr::SizeofType(_)
        | Expr::SizeofExpr(_) => {}
        Expr::Member(base, _, true) => {
            walk(base, env, Mode::Read, effects);
            memory(effects, mode);
        }
        Expr::Member(base, _, false) => walk(base, env, mode, effects),
        Expr::Index(base, index) => {
            walk(index, env, Mode::Read, effects);
            let array = env
                .type_of(base)
                .is_some_and(|ty| matches!(ty.strip(), CType::Array(_)));
            if array {
                walk(base, env, mode, effects);
            } else {
                walk(base, env, Mode::Read, effects);
                memory(effects, mode);
            }
        }
        Expr::Unary(UnOp::Deref, pointer) => {
            walk(pointer, env, Mode::Read, effects);
            memory(effects, mode);
        }
        Expr::Unary(UnOp::AddrOf, operand) => walk(operand, env, Mode::Address, effects),
        Expr::Unary(UnOp::PreInc | UnOp::PreDec, operand)
        | Expr::PostInc(operand)
        | Expr::PostDec(operand) => walk(operand, env, Mode::ReadWrite, effects),
        Expr::Unary(_, operand) | Expr::Cast(_, operand) => walk(operand, env, Mode::Read, effects),
        Expr::Binary(_, left, right) | Expr::Comma(left, right) => {
            walk(left, env, Mode::Read, effects);
            walk(right, env, Mode::Read, effects);
        }
        Expr::Assign(op, target, value) => {
            walk(value, env, Mode::Read, effects);
            let target_mode = if op.is_some() {
                Mode::ReadWrite
            } else {
                Mode::Write
            };
            walk(target, env, target_mode, effects);
        }
        Expr::Cond(cond, then, other) => {
            walk(cond, env, Mode::Read, effects);
            walk(then, env, Mode::Read, effects);
            walk(other, env, Mode::Read, effects);
        }
        Expr::Call(callee, arguments) => {
            effects.call = true;
            walk(callee, env, Mode::Read, effects);
            for argument in arguments {
                walk(argument, env, Mode::Read, effects);
            }
        }
    }
}

/// Whether control can leave `stmt` other than by falling through: a return,
/// a goto, a label to jump in by, or a break or continue that is not caught
/// by a loop or switch inside it.
pub fn escapes(stmt: &Stmt) -> bool {
    fn inner(stmt: &Stmt, in_loop: bool, in_switch: bool) -> bool {
        match stmt {
            Stmt::Return(_) | Stmt::Goto(_) | Stmt::Label(_) => true,
            Stmt::Case(_) | Stmt::Default => !in_switch,
            Stmt::Break => !(in_loop || in_switch),
            Stmt::Continue => !in_loop,
            Stmt::Block(stmts) => stmts.iter().any(|stmt| inner(stmt, in_loop, in_switch)),
            Stmt::If(_, then, other) => {
                inner(then, in_loop, in_switch)
                    || other
                        .as_ref()
                        .is_some_and(|other| inner(other, in_loop, in_switch))
            }
            Stmt::While(_, body) | Stmt::DoWhile(body, _) | Stmt::For(_, _, _, body) => {
                inner(body, true, in_switch)
            }
            Stmt::Switch(_, body) => inner(body, in_loop, true),
            _ => false,
        }
    }
    inner(stmt, false, false)
}

/// Whether a `continue` in `body` belongs to the loop `body` is the body of.
pub fn has_continue(body: &Stmt) -> bool {
    match body {
        Stmt::Continue => true,
        Stmt::Block(stmts) => stmts.iter().any(has_continue),
        Stmt::If(_, then, other) => {
            has_continue(then) || other.as_ref().is_some_and(|other| has_continue(other))
        }
        Stmt::Switch(_, body) => has_continue(body),
        _ => false,
    }
}
