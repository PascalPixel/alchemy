//! AST utilities over `lang-c`: traversal, node identity, statement
//! insertion, and expression equality.
//!
//! A port of pret decomp-permuter's `ast_util.py` (MIT), adapted to
//! `lang-c`'s `Node<T>` representation. Node identity is the source span of
//! the node in the current candidate text: the engine re-emits and re-parses
//! after every mutation, so every pass starts from a tree whose spans are
//! genuine and unique. Nodes created during a mutation carry `Span::none()`
//! and are never targeted again within the same pass.
//!
//! pycparser's `Case`/`Default` blocks do not exist in `lang-c`: a case
//! label is a `Labeled` statement whose body (in this repository's emitted
//! style) is always a compound, so `Block` here is always a compound
//! statement.

use lang_c::ast::*;
use lang_c::span::{Node, Span};

use crate::asttypes::{is_assign_op, TypeMap};
use crate::cemit;

/// Node identity: (span.start, span.end) in the current candidate text.
pub type Nid = (usize, usize);

pub fn nid<T>(n: &Node<T>) -> Nid {
    (n.span.start, n.span.end)
}

pub fn n<T>(t: T) -> Node<T> {
    Node::new(t, Span::none())
}

// ---------------------------------------------------------- constructors

pub fn id_expr(name: &str) -> Expression {
    Expression::Identifier(Box::new(n(Identifier { name: name.to_string() })))
}

pub fn int_expr(number: &str) -> Expression {
    int_expr_full(number, IntegerBase::Decimal, false)
}

pub fn int_expr_full(number: &str, base: IntegerBase, unsigned: bool) -> Expression {
    Expression::Constant(Box::new(n(Constant::Integer(Integer {
        base,
        number: number.into(),
        suffix: IntegerSuffix { size: IntegerSize::Int, unsigned, imaginary: false },
    }))))
}

pub fn float_expr(number: &str, single: bool) -> Expression {
    Expression::Constant(Box::new(n(Constant::Float(Float {
        base: FloatBase::Decimal,
        number: number.into(),
        suffix: FloatSuffix { format: if single { FloatFormat::Float } else { FloatFormat::Double }, imaginary: false },
    }))))
}

pub fn binop(op: BinaryOperator, lhs: Expression, rhs: Expression) -> Expression {
    Expression::BinaryOperator(Box::new(n(BinaryOperatorExpression {
        operator: n(op),
        lhs: Box::new(n(lhs)),
        rhs: Box::new(n(rhs)),
    })))
}

pub fn unary(op: UnaryOperator, operand: Expression) -> Expression {
    Expression::UnaryOperator(Box::new(n(UnaryOperatorExpression { operator: n(op), operand: Box::new(n(operand)) })))
}

pub fn expr_stmt(e: Expression) -> Node<BlockItem> {
    n(BlockItem::Statement(n(Statement::Expression(Some(Box::new(n(e)))))))
}

pub fn empty_compound() -> Node<Statement> {
    n(Statement::Compound(Vec::new()))
}

pub fn equal_expr(a: &Expression, b: &Expression) -> bool {
    cemit::emit_expression(a) == cemit::emit_expression(b)
}

// --------------------------------------------------------- classification

pub fn is_lvalue(e: &Expression, tm: &TypeMap) -> bool {
    match e {
        Expression::Identifier(identifier) => !tm.enum_constants.contains(&identifier.node.name),
        Expression::Member(_) => true,
        Expression::BinaryOperator(b) => {
            matches!(b.node.operator.node, BinaryOperator::Index)
        }
        Expression::UnaryOperator(u) => {
            matches!(u.node.operator.node, UnaryOperator::Indirection)
        }
        _ => false,
    }
}

pub fn is_effectful(e: &Expression) -> bool {
    let mut found = false;
    for_each_expr(e, &mut |sub| match sub {
        Expression::UnaryOperator(u) => {
            if matches!(
                u.node.operator.node,
                UnaryOperator::PostIncrement
                    | UnaryOperator::PostDecrement
                    | UnaryOperator::PreIncrement
                    | UnaryOperator::PreDecrement
            ) {
                found = true;
            }
        }
        Expression::Call(_) => found = true,
        Expression::BinaryOperator(b) if is_assign_op(&b.node.operator.node) => found = true,
        _ => {}
    });
    found
}

/// Full recursion over every sub-expression, including lvalues.
pub fn for_each_expr(e: &Expression, f: &mut dyn FnMut(&Expression)) {
    f(e);
    match e {
        Expression::Member(m) => for_each_expr(&m.node.expression.node, f),
        Expression::Call(c) => {
            for_each_expr(&c.node.callee.node, f);
            for a in &c.node.arguments {
                for_each_expr(&a.node, f);
            }
        }
        Expression::UnaryOperator(u) => for_each_expr(&u.node.operand.node, f),
        Expression::Cast(c) => for_each_expr(&c.node.expression.node, f),
        Expression::BinaryOperator(b) => {
            for_each_expr(&b.node.lhs.node, f);
            for_each_expr(&b.node.rhs.node, f);
        }
        Expression::Conditional(c) => {
            for_each_expr(&c.node.condition.node, f);
            for_each_expr(&c.node.then_expression.node, f);
            for_each_expr(&c.node.else_expression.node, f);
        }
        Expression::Comma(items) => {
            for i in items.iter() {
                for_each_expr(&i.node, f);
            }
        }
        Expression::SizeOfVal(v) => for_each_expr(&v.node.0.node, f),
        _ => {}
    }
}

// ------------------------------------------------- visit_replace traversal
//
// Port of pret's `visit_replace`/`replace_subexprs`: visits every
// expression position, reporting whether the position is a replaceable
// expression (`is_expr` = not a statement-level expression and not an
// lvalue). The mutable callback returns true when it replaced the node,
// which stops descent into it.

pub fn walk_stmt_exprs(stmt: &mut Node<Statement>, f: &mut dyn FnMut(&mut Node<Expression>, bool) -> bool) {
    match &mut stmt.node {
        Statement::Labeled(l) => walk_stmt_exprs(&mut l.node.statement, f),
        Statement::Compound(items) => {
            for item in items.iter_mut() {
                walk_block_item_exprs(item, f);
            }
        }
        Statement::Expression(Some(e)) => walk_expr(e, true, false, f),
        Statement::Expression(None) => {}
        Statement::If(i) => {
            walk_expr(&mut i.node.condition, false, false, f);
            walk_stmt_exprs(&mut i.node.then_statement, f);
            if let Some(e) = &mut i.node.else_statement {
                walk_stmt_exprs(e, f);
            }
        }
        Statement::Switch(s) => {
            walk_expr(&mut s.node.expression, false, false, f);
            walk_stmt_exprs(&mut s.node.statement, f);
        }
        Statement::While(w) => {
            walk_expr(&mut w.node.expression, false, false, f);
            walk_stmt_exprs(&mut w.node.statement, f);
        }
        Statement::DoWhile(d) => {
            walk_stmt_exprs(&mut d.node.statement, f);
            walk_expr(&mut d.node.expression, false, false, f);
        }
        Statement::For(fo) => {
            match &mut fo.node.initializer.node {
                ForInitializer::Expression(e) => walk_expr(e, true, false, f),
                ForInitializer::Declaration(d) => walk_decl_exprs(d, f),
                _ => {}
            }
            if let Some(c) = &mut fo.node.condition {
                walk_expr(c, false, false, f);
            }
            if let Some(s) = &mut fo.node.step {
                walk_expr(s, true, false, f);
            }
            walk_stmt_exprs(&mut fo.node.statement, f);
        }
        Statement::Return(Some(e)) => walk_expr(e, false, false, f),
        _ => {}
    }
}

fn walk_block_item_exprs(item: &mut Node<BlockItem>, f: &mut dyn FnMut(&mut Node<Expression>, bool) -> bool) {
    match &mut item.node {
        BlockItem::Declaration(d) => walk_decl_exprs(d, f),
        BlockItem::Statement(s) => walk_stmt_exprs(s, f),
        BlockItem::StaticAssert(_) => {}
    }
}

fn walk_decl_exprs(d: &mut Node<Declaration>, f: &mut dyn FnMut(&mut Node<Expression>, bool) -> bool) {
    for init in d.node.declarators.iter_mut() {
        if let Some(i) = &mut init.node.initializer {
            if let Initializer::Expression(e) = &mut i.node {
                walk_expr(e, false, false, f);
            }
        }
    }
}

fn walk_expr(
    e: &mut Node<Expression>,
    toplevel: bool,
    lvalue: bool,
    f: &mut dyn FnMut(&mut Node<Expression>, bool) -> bool,
) {
    let is_expr = !toplevel && !lvalue;
    if f(e, is_expr) {
        return;
    }
    match &mut e.node {
        Expression::BinaryOperator(b) => {
            let op = b.node.operator.node.clone();
            if is_assign_op(&op) {
                walk_expr(&mut b.node.lhs, false, true, f);
                walk_expr(&mut b.node.rhs, false, false, f);
            } else if matches!(op, BinaryOperator::Index) {
                walk_expr(&mut b.node.lhs, false, lvalue, f);
                walk_expr(&mut b.node.rhs, false, false, f);
            } else {
                walk_expr(&mut b.node.lhs, false, false, f);
                walk_expr(&mut b.node.rhs, false, false, f);
            }
        }
        Expression::Member(m) => {
            let direct = matches!(m.node.operator.node, MemberOperator::Direct);
            walk_expr(&mut m.node.expression, false, lvalue && direct, f);
        }
        Expression::Cast(c) => walk_expr(&mut c.node.expression, false, false, f),
        Expression::UnaryOperator(u) => match u.node.operator.node {
            UnaryOperator::PostIncrement
            | UnaryOperator::PostDecrement
            | UnaryOperator::PreIncrement
            | UnaryOperator::PreDecrement
            | UnaryOperator::Address => walk_expr(&mut u.node.operand, false, true, f),
            _ => walk_expr(&mut u.node.operand, false, false, f),
        },
        Expression::Call(c) => {
            if !matches!(&c.node.callee.node, Expression::Identifier(_)) {
                walk_expr(&mut c.node.callee, false, false, f);
            }
            for a in c.node.arguments.iter_mut() {
                walk_expr(a, false, false, f);
            }
        }
        Expression::Comma(items) => {
            for i in items.iter_mut() {
                walk_expr(i, false, false, f);
            }
        }
        Expression::Conditional(c) => {
            walk_expr(&mut c.node.condition, false, false, f);
            walk_expr(&mut c.node.then_expression, true, false, f);
            walk_expr(&mut c.node.else_expression, true, false, f);
        }
        _ => {}
    }
}

/// Immutable variant of the same traversal, for candidate enumeration.
pub fn scan_stmt_exprs(stmt: &Node<Statement>, f: &mut dyn FnMut(&Node<Expression>, bool)) {
    // Reuse the mutable walker through an adapter is impossible without
    // aliasing; duplicate the dispatch immutably via a tiny shim: clone-free
    // read-only recursion.
    scan_stmt(stmt, f);
}

fn scan_stmt(stmt: &Node<Statement>, f: &mut dyn FnMut(&Node<Expression>, bool)) {
    match &stmt.node {
        Statement::Labeled(l) => scan_stmt(&l.node.statement, f),
        Statement::Compound(items) => {
            for item in items {
                match &item.node {
                    BlockItem::Declaration(d) => scan_decl(d, f),
                    BlockItem::Statement(s) => scan_stmt(s, f),
                    BlockItem::StaticAssert(_) => {}
                }
            }
        }
        Statement::Expression(Some(e)) => scan_expr(e, true, false, f),
        Statement::Expression(None) => {}
        Statement::If(i) => {
            scan_expr(&i.node.condition, false, false, f);
            scan_stmt(&i.node.then_statement, f);
            if let Some(e) = &i.node.else_statement {
                scan_stmt(e, f);
            }
        }
        Statement::Switch(s) => {
            scan_expr(&s.node.expression, false, false, f);
            scan_stmt(&s.node.statement, f);
        }
        Statement::While(w) => {
            scan_expr(&w.node.expression, false, false, f);
            scan_stmt(&w.node.statement, f);
        }
        Statement::DoWhile(d) => {
            scan_stmt(&d.node.statement, f);
            scan_expr(&d.node.expression, false, false, f);
        }
        Statement::For(fo) => {
            match &fo.node.initializer.node {
                ForInitializer::Expression(e) => scan_expr(e, true, false, f),
                ForInitializer::Declaration(d) => scan_decl(d, f),
                _ => {}
            }
            if let Some(c) = &fo.node.condition {
                scan_expr(c, false, false, f);
            }
            if let Some(s) = &fo.node.step {
                scan_expr(s, true, false, f);
            }
            scan_stmt(&fo.node.statement, f);
        }
        Statement::Return(Some(e)) => scan_expr(e, false, false, f),
        _ => {}
    }
}

/// Expression scan over a single block item (statement or declaration).
pub fn scan_item_exprs(item: &Node<BlockItem>, f: &mut dyn FnMut(&Node<Expression>, bool)) {
    match &item.node {
        BlockItem::Declaration(d) => scan_decl(d, f),
        BlockItem::Statement(s) => scan_stmt(s, f),
        BlockItem::StaticAssert(_) => {}
    }
}

fn scan_decl(d: &Node<Declaration>, f: &mut dyn FnMut(&Node<Expression>, bool)) {
    for init in &d.node.declarators {
        if let Some(i) = &init.node.initializer {
            if let Initializer::Expression(e) = &i.node {
                scan_expr(e, false, false, f);
            }
        }
    }
}

fn scan_expr(e: &Node<Expression>, toplevel: bool, lvalue: bool, f: &mut dyn FnMut(&Node<Expression>, bool)) {
    let is_expr = !toplevel && !lvalue;
    f(e, is_expr);
    match &e.node {
        Expression::BinaryOperator(b) => {
            let op = &b.node.operator.node;
            if is_assign_op(op) {
                scan_expr(&b.node.lhs, false, true, f);
                scan_expr(&b.node.rhs, false, false, f);
            } else if matches!(op, BinaryOperator::Index) {
                scan_expr(&b.node.lhs, false, lvalue, f);
                scan_expr(&b.node.rhs, false, false, f);
            } else {
                scan_expr(&b.node.lhs, false, false, f);
                scan_expr(&b.node.rhs, false, false, f);
            }
        }
        Expression::Member(m) => {
            let direct = matches!(m.node.operator.node, MemberOperator::Direct);
            scan_expr(&m.node.expression, false, lvalue && direct, f);
        }
        Expression::Cast(c) => scan_expr(&c.node.expression, false, false, f),
        Expression::UnaryOperator(u) => match u.node.operator.node {
            UnaryOperator::PostIncrement
            | UnaryOperator::PostDecrement
            | UnaryOperator::PreIncrement
            | UnaryOperator::PreDecrement
            | UnaryOperator::Address => scan_expr(&u.node.operand, false, true, f),
            _ => scan_expr(&u.node.operand, false, false, f),
        },
        Expression::Call(c) => {
            if !matches!(&c.node.callee.node, Expression::Identifier(_)) {
                scan_expr(&c.node.callee, false, false, f);
            }
            for a in &c.node.arguments {
                scan_expr(a, false, false, f);
            }
        }
        Expression::Comma(items) => {
            for i in items.iter() {
                scan_expr(i, false, false, f);
            }
        }
        Expression::Conditional(c) => {
            scan_expr(&c.node.condition, false, false, f);
            scan_expr(&c.node.then_expression, true, false, f);
            scan_expr(&c.node.else_expression, true, false, f);
        }
        _ => {}
    }
}

// -------------------------------------------------------- statement visits

pub fn visit_stmts(stmt: &Node<Statement>, f: &mut dyn FnMut(&Node<Statement>)) {
    f(stmt);
    match &stmt.node {
        Statement::Labeled(l) => visit_stmts(&l.node.statement, f),
        Statement::Compound(items) => {
            for item in items {
                if let BlockItem::Statement(s) = &item.node {
                    visit_stmts(s, f);
                }
            }
        }
        Statement::If(i) => {
            visit_stmts(&i.node.then_statement, f);
            if let Some(e) = &i.node.else_statement {
                visit_stmts(e, f);
            }
        }
        Statement::Switch(s) => visit_stmts(&s.node.statement, f),
        Statement::While(w) => visit_stmts(&w.node.statement, f),
        Statement::DoWhile(d) => visit_stmts(&d.node.statement, f),
        Statement::For(fo) => visit_stmts(&fo.node.statement, f),
        _ => {}
    }
}

pub fn visit_stmts_mut(stmt: &mut Node<Statement>, f: &mut dyn FnMut(&mut Node<Statement>)) {
    f(stmt);
    match &mut stmt.node {
        Statement::Labeled(l) => visit_stmts_mut(&mut l.node.statement, f),
        Statement::Compound(items) => {
            for item in items.iter_mut() {
                if let BlockItem::Statement(s) = &mut item.node {
                    visit_stmts_mut(s, f);
                }
            }
        }
        Statement::If(i) => {
            visit_stmts_mut(&mut i.node.then_statement, f);
            if let Some(e) = &mut i.node.else_statement {
                visit_stmts_mut(e, f);
            }
        }
        Statement::Switch(s) => visit_stmts_mut(&mut s.node.statement, f),
        Statement::While(w) => visit_stmts_mut(&mut w.node.statement, f),
        Statement::DoWhile(d) => visit_stmts_mut(&mut d.node.statement, f),
        Statement::For(fo) => visit_stmts_mut(&mut fo.node.statement, f),
        _ => {}
    }
}

/// Mutable walk over every declaration statement in a function body.
pub fn visit_decls_mut(stmt: &mut Node<Statement>, f: &mut dyn FnMut(&mut Node<Declaration>)) {
    match &mut stmt.node {
        Statement::Labeled(l) => visit_decls_mut(&mut l.node.statement, f),
        Statement::Compound(items) => {
            for item in items.iter_mut() {
                match &mut item.node {
                    BlockItem::Declaration(d) => f(d),
                    BlockItem::Statement(s) => visit_decls_mut(s, f),
                    BlockItem::StaticAssert(_) => {}
                }
            }
        }
        Statement::If(i) => {
            visit_decls_mut(&mut i.node.then_statement, f);
            if let Some(e) = &mut i.node.else_statement {
                visit_decls_mut(e, f);
            }
        }
        Statement::Switch(s) => visit_decls_mut(&mut s.node.statement, f),
        Statement::While(w) => visit_decls_mut(&mut w.node.statement, f),
        Statement::DoWhile(d) => visit_decls_mut(&mut d.node.statement, f),
        Statement::For(fo) => {
            if let ForInitializer::Declaration(d) = &mut fo.node.initializer.node {
                f(d);
            }
            visit_decls_mut(&mut fo.node.statement, f);
        }
        _ => {}
    }
}

// ------------------------------------------------------------- blocks

/// Nested blocks of one statement, in pret's `for_nested_blocks` order.
/// After the engine's normalization every returned statement is a Compound.
pub fn nested_blocks(stmt: &Node<Statement>) -> Vec<&Node<Statement>> {
    let mut out = Vec::new();
    collect_nested(stmt, &mut out);
    out
}

fn collect_nested<'a>(stmt: &'a Node<Statement>, out: &mut Vec<&'a Node<Statement>>) {
    match &stmt.node {
        Statement::Compound(_) => out.push(stmt),
        Statement::For(f) => out.push(&f.node.statement),
        Statement::While(w) => out.push(&w.node.statement),
        Statement::DoWhile(d) => out.push(&d.node.statement),
        Statement::If(i) => {
            out.push(&i.node.then_statement);
            if let Some(e) = &i.node.else_statement {
                out.push(e);
            }
        }
        Statement::Switch(s) => out.push(&s.node.statement),
        Statement::Labeled(l) => collect_nested(&l.node.statement, out),
        _ => {}
    }
}

pub fn has_nested_block(item: &BlockItem) -> bool {
    match item {
        BlockItem::Statement(s) => matches!(
            &s.node,
            Statement::Compound(_)
                | Statement::For(_)
                | Statement::While(_)
                | Statement::DoWhile(_)
                | Statement::If(_)
                | Statement::Switch(_)
                | Statement::Labeled(_)
        ),
        _ => false,
    }
}

pub fn block_items(block: &Node<Statement>) -> &Vec<Node<BlockItem>> {
    match &block.node {
        Statement::Compound(items) => items,
        _ => panic!("block is not a compound; the engine normalizes candidates"),
    }
}

/// Finds the compound statement with the given id and hands its item list
/// to the callback. Returns None when the block was not found.
pub fn with_block<R>(
    root: &mut Node<Statement>,
    target: Nid,
    f: &mut dyn FnMut(&mut Vec<Node<BlockItem>>) -> R,
) -> Option<R> {
    let root_id = nid(root);
    if let Statement::Compound(items) = &mut root.node {
        if root_id == target {
            return Some(f(items));
        }
    }
    match &mut root.node {
        Statement::Labeled(l) => with_block(&mut l.node.statement, target, f),
        Statement::Compound(items) => {
            for item in items.iter_mut() {
                if let BlockItem::Statement(s) = &mut item.node {
                    if let Some(r) = with_block(s, target, f) {
                        return Some(r);
                    }
                }
            }
            None
        }
        Statement::If(i) => {
            if let Some(r) = with_block(&mut i.node.then_statement, target, f) {
                return Some(r);
            }
            if let Some(e) = &mut i.node.else_statement {
                return with_block(e, target, f);
            }
            None
        }
        Statement::Switch(s) => with_block(&mut s.node.statement, target, f),
        Statement::While(w) => with_block(&mut w.node.statement, target, f),
        Statement::DoWhile(d) => with_block(&mut d.node.statement, target, f),
        Statement::For(fo) => with_block(&mut fo.node.statement, target, f),
        _ => None,
    }
}

pub fn insert_block_item(root: &mut Node<Statement>, block: Nid, index: usize, item: Node<BlockItem>) -> bool {
    let mut item = Some(item);
    with_block(root, block, &mut |items| {
        let at = index.min(items.len());
        if let Some(item) = item.take() {
            items.insert(at, item);
        }
    })
    .is_some()
}

// ----------------------------------------------------- insertion points

#[derive(Debug, Clone)]
pub struct DeclInfo {
    pub names: Vec<String>,
    /// Set when the declaration declares exactly one name: (name, init
    /// present and not an initializer list).
    pub single: Option<(String, bool)>,
}

#[derive(Debug, Clone)]
pub struct StmtInfo {
    pub id: Nid,
    pub start: usize,
    pub is_decl: bool,
    pub has_nested_block: bool,
    /// Control transfers (break/continue/return/goto) must never move or
    /// vanish: relocating one changes which statements execute, and the
    /// search's biggest false gains have all been exactly that.
    pub is_jump: bool,
    pub decl: Option<DeclInfo>,
}

#[derive(Debug, Clone)]
pub struct InsPoint {
    pub block: Nid,
    pub index: usize,
    pub before: Option<StmtInfo>,
    pub after: Option<StmtInfo>,
}

fn stmt_info(item: &Node<BlockItem>) -> StmtInfo {
    let (is_decl, decl) = match &item.node {
        BlockItem::Declaration(d) => {
            let names: Vec<String> = d
                .node
                .declarators
                .iter()
                .filter_map(|i| {
                    crate::asttypes::apply_declarator(crate::asttypes::CType::int(), &i.node.declarator.node).1
                })
                .collect();
            let single = if d.node.declarators.len() == 1 && names.len() == 1 {
                let init = &d.node.declarators[0].node.initializer;
                let ok_init = matches!(
                    init,
                    Some(node) if matches!(node.node, Initializer::Expression(_))
                );
                Some((names[0].clone(), ok_init))
            } else {
                None
            };
            (true, Some(DeclInfo { names, single }))
        }
        _ => (false, None),
    };
    let is_jump = matches!(
        &item.node,
        BlockItem::Statement(st) if matches!(
            st.node,
            Statement::Break | Statement::Continue | Statement::Return(_) | Statement::Goto(_)
        )
    );
    StmtInfo {
        id: item_nid(item),
        start: item_nid(item).0,
        is_decl,
        has_nested_block: has_nested_block(&item.node),
        is_jump,
        decl,
    }
}

pub fn item_nid(item: &Node<BlockItem>) -> Nid {
    nid(item)
}

/// Port of `get_insertion_points`. The randomization region is always
/// unbounded in this port (PERM pragma regions are a stub), so every point
/// is included.
pub fn insertion_points(body: &Node<Statement>, allow_within_decl: bool) -> Vec<InsPoint> {
    let mut out = Vec::new();
    rec_points(body, &mut out);
    if !allow_within_decl {
        out.retain(|p| !p.after.as_ref().map(|a| a.is_decl).unwrap_or(false));
    }
    out
}

fn rec_points(block: &Node<Statement>, out: &mut Vec<InsPoint>) {
    let items = block_items(block);
    let block_id = nid(block);
    let mut last: Option<StmtInfo> = None;
    for (i, item) in items.iter().enumerate() {
        let info = stmt_info(item);
        out.push(InsPoint { block: block_id, index: i, before: last.clone(), after: Some(info.clone()) });
        if let BlockItem::Statement(s) = &item.node {
            for nb in nested_blocks(s) {
                rec_points(nb, out);
            }
        }
        last = Some(info);
    }
    out.push(InsPoint { block: block_id, index: items.len(), before: last, after: None });
}

// ------------------------------------------------- reads / writes / decls

/// Port of `compute_write_locations`: declaration sites, `++`/`--` on plain
/// identifiers, and assignments whose lvalue is a plain identifier, keyed by
/// span start (monotone in traversal order).
pub fn compute_write_locations(fdef: &FunctionDefinition) -> std::collections::BTreeMap<String, Vec<usize>> {
    let mut writes: std::collections::BTreeMap<String, Vec<usize>> = Default::default();
    let mut add = |name: &str, loc: usize| {
        writes.entry(name.to_string()).or_default().push(loc);
    };
    // Parameters count as writes at their declaration.
    for der in &fdef.declarator.node.derived {
        if let DerivedDeclarator::Function(fd) = &der.node {
            for p in &fd.node.parameters {
                if let Some(d) = &p.node.declarator {
                    if let (_, Some(name)) = crate::asttypes::apply_declarator(crate::asttypes::CType::int(), &d.node) {
                        add(&name, p.span.start);
                    }
                }
            }
        }
    }
    collect_writes(&fdef.statement, &mut add);
    for locs in writes.values_mut() {
        locs.sort_unstable();
        locs.dedup();
    }
    writes
}

fn collect_writes(stmt: &Node<Statement>, add: &mut dyn FnMut(&str, usize)) {
    // Declarations.
    fn decl_writes(d: &Node<Declaration>, add: &mut dyn FnMut(&str, usize)) {
        for init in &d.node.declarators {
            if let (_, Some(name)) =
                crate::asttypes::apply_declarator(crate::asttypes::CType::int(), &init.node.declarator.node)
            {
                add(&name, d.span.start);
            }
            if let Some(i) = &init.node.initializer {
                if let Initializer::Expression(e) = &i.node {
                    expr_writes(e, add);
                }
            }
        }
    }
    fn expr_writes(e: &Node<Expression>, add: &mut dyn FnMut(&str, usize)) {
        let start = e.span.start;
        match &e.node {
            Expression::UnaryOperator(u) => {
                if matches!(
                    u.node.operator.node,
                    UnaryOperator::PostIncrement
                        | UnaryOperator::PostDecrement
                        | UnaryOperator::PreIncrement
                        | UnaryOperator::PreDecrement
                ) {
                    if let Expression::Identifier(id) = &u.node.operand.node {
                        add(&id.node.name, start);
                    }
                }
                expr_writes(&u.node.operand, add);
            }
            Expression::BinaryOperator(b) => {
                if is_assign_op(&b.node.operator.node) {
                    if let Expression::Identifier(id) = &b.node.lhs.node {
                        add(&id.node.name, start);
                    }
                }
                expr_writes(&b.node.lhs, add);
                expr_writes(&b.node.rhs, add);
            }
            Expression::Member(m) => expr_writes(&m.node.expression, add),
            Expression::Call(c) => {
                expr_writes(&c.node.callee, add);
                for a in &c.node.arguments {
                    expr_writes(a, add);
                }
            }
            Expression::Cast(c) => expr_writes(&c.node.expression, add),
            Expression::Conditional(c) => {
                expr_writes(&c.node.condition, add);
                expr_writes(&c.node.then_expression, add);
                expr_writes(&c.node.else_expression, add);
            }
            Expression::Comma(items) => {
                for i in items.iter() {
                    expr_writes(i, add);
                }
            }
            Expression::SizeOfVal(v) => expr_writes(&v.node.0, add),
            _ => {}
        }
    }
    match &stmt.node {
        Statement::Labeled(l) => collect_writes(&l.node.statement, add),
        Statement::Compound(items) => {
            for item in items {
                match &item.node {
                    BlockItem::Declaration(d) => decl_writes(d, add),
                    BlockItem::Statement(s) => collect_writes(s, add),
                    BlockItem::StaticAssert(_) => {}
                }
            }
        }
        Statement::Expression(Some(e)) => expr_writes(e, add),
        Statement::If(i) => {
            expr_writes(&i.node.condition, add);
            collect_writes(&i.node.then_statement, add);
            if let Some(e) = &i.node.else_statement {
                collect_writes(e, add);
            }
        }
        Statement::Switch(s) => {
            expr_writes(&s.node.expression, add);
            collect_writes(&s.node.statement, add);
        }
        Statement::While(w) => {
            expr_writes(&w.node.expression, add);
            collect_writes(&w.node.statement, add);
        }
        Statement::DoWhile(d) => {
            collect_writes(&d.node.statement, add);
            expr_writes(&d.node.expression, add);
        }
        Statement::For(f) => {
            match &f.node.initializer.node {
                ForInitializer::Expression(e) => expr_writes(e, add),
                ForInitializer::Declaration(d) => decl_writes(d, add),
                _ => {}
            }
            if let Some(c) = &f.node.condition {
                expr_writes(c, add);
            }
            if let Some(s) = &f.node.step {
                expr_writes(s, add);
            }
            collect_writes(&f.node.statement, add);
        }
        Statement::Return(Some(e)) => expr_writes(e, add),
        _ => {}
    }
}

/// Finds variable reads used by the data-flow-sensitive AST passes. `&id` is
/// not a value read and a plain assignment does not read its lvalue, but its
/// right-hand side must still be visited. The old pret-compatible traversal
/// skipped both sides of `copy = Func(size)`, which let `perm_expand_expr`
/// erase the definition of `size` while leaving that use uninitialized.
pub fn find_var_reads(fdef: &FunctionDefinition) -> Vec<(String, usize)> {
    let mut out = Vec::new();
    reads_stmt(&fdef.statement, &mut out);
    out
}

pub fn find_var_reads_in_expr(e: &Node<Expression>) -> Vec<(String, usize)> {
    let mut out = Vec::new();
    reads_expr(e, &mut out);
    out
}

fn reads_stmt(stmt: &Node<Statement>, out: &mut Vec<(String, usize)>) {
    match &stmt.node {
        Statement::Labeled(l) => reads_stmt(&l.node.statement, out),
        Statement::Compound(items) => {
            for item in items {
                match &item.node {
                    BlockItem::Declaration(d) => {
                        for init in &d.node.declarators {
                            if let Some(i) = &init.node.initializer {
                                if let Initializer::Expression(e) = &i.node {
                                    reads_expr(e, out);
                                }
                            }
                        }
                    }
                    BlockItem::Statement(s) => reads_stmt(s, out),
                    BlockItem::StaticAssert(_) => {}
                }
            }
        }
        Statement::Expression(Some(e)) => reads_expr(e, out),
        Statement::If(i) => {
            reads_expr(&i.node.condition, out);
            reads_stmt(&i.node.then_statement, out);
            if let Some(e) = &i.node.else_statement {
                reads_stmt(e, out);
            }
        }
        Statement::Switch(s) => {
            reads_expr(&s.node.expression, out);
            reads_stmt(&s.node.statement, out);
        }
        Statement::While(w) => {
            reads_expr(&w.node.expression, out);
            reads_stmt(&w.node.statement, out);
        }
        Statement::DoWhile(d) => {
            reads_stmt(&d.node.statement, out);
            reads_expr(&d.node.expression, out);
        }
        Statement::For(f) => {
            match &f.node.initializer.node {
                ForInitializer::Expression(e) => reads_expr(e, out),
                ForInitializer::Declaration(d) => {
                    for init in &d.node.declarators {
                        if let Some(i) = &init.node.initializer {
                            if let Initializer::Expression(e) = &i.node {
                                reads_expr(e, out);
                            }
                        }
                    }
                }
                _ => {}
            }
            if let Some(c) = &f.node.condition {
                reads_expr(c, out);
            }
            if let Some(s) = &f.node.step {
                reads_expr(s, out);
            }
            reads_stmt(&f.node.statement, out);
        }
        Statement::Return(Some(e)) => reads_expr(e, out),
        _ => {}
    }
}

fn reads_expr(e: &Node<Expression>, out: &mut Vec<(String, usize)>) {
    match &e.node {
        Expression::Identifier(id) => out.push((id.node.name.clone(), e.span.start)),
        Expression::UnaryOperator(u) => {
            if matches!(u.node.operator.node, UnaryOperator::Address)
                && matches!(&u.node.operand.node, Expression::Identifier(_))
            {
                return;
            }
            reads_expr(&u.node.operand, out);
        }
        Expression::Member(m) => reads_expr(&m.node.expression, out),
        Expression::BinaryOperator(b) => {
            if is_assign_op(&b.node.operator.node) {
                if b.node.operator.node != BinaryOperator::Assign
                    || !matches!(&b.node.lhs.node, Expression::Identifier(_))
                {
                    reads_expr(&b.node.lhs, out);
                }
                reads_expr(&b.node.rhs, out);
                return;
            }
            reads_expr(&b.node.lhs, out);
            reads_expr(&b.node.rhs, out);
        }
        Expression::Call(c) => {
            reads_expr(&c.node.callee, out);
            for a in &c.node.arguments {
                reads_expr(a, out);
            }
        }
        Expression::Cast(c) => reads_expr(&c.node.expression, out),
        Expression::Conditional(c) => {
            reads_expr(&c.node.condition, out);
            reads_expr(&c.node.then_expression, out);
            reads_expr(&c.node.else_expression, out);
        }
        Expression::Comma(items) => {
            for i in items.iter() {
                reads_expr(i, out);
            }
        }
        Expression::SizeOfVal(v) => reads_expr(&v.node.0, out),
        _ => {}
    }
}

pub fn compute_read_locations(fdef: &FunctionDefinition) -> std::collections::BTreeMap<String, Vec<usize>> {
    let mut reads: std::collections::BTreeMap<String, Vec<usize>> = Default::default();
    for (name, loc) in find_var_reads(fdef) {
        reads.entry(name).or_default().push(loc);
    }
    for locs in reads.values_mut() {
        locs.sort_unstable();
        locs.dedup();
    }
    reads
}

// --------------------------------------------------------------- decls

/// Builds `type name;` from a CType for a new temporary. Supports basic
/// types (with volatile), struct/union/enum references, and pointer chains
/// over those.
pub fn make_decl(name: &str, t: &crate::asttypes::CType) -> Option<Node<Declaration>> {
    use crate::asttypes::CType;
    let mut ptrs = 0usize;
    let mut cur = t;
    while let CType::Ptr(inner) = cur {
        ptrs += 1;
        cur = inner;
    }
    let mut specifiers: Vec<Node<DeclarationSpecifier>> = Vec::new();
    match cur {
        CType::Basic { names, volatile } => {
            if *volatile {
                specifiers.push(n(DeclarationSpecifier::TypeQualifier(n(TypeQualifier::Volatile))));
            }
            for name in names {
                let ts = match name.as_str() {
                    "void" => TypeSpecifier::Void,
                    "char" => TypeSpecifier::Char,
                    "short" => TypeSpecifier::Short,
                    "int" => TypeSpecifier::Int,
                    "long" => TypeSpecifier::Long,
                    "float" => TypeSpecifier::Float,
                    "double" => TypeSpecifier::Double,
                    "signed" => TypeSpecifier::Signed,
                    "unsigned" => TypeSpecifier::Unsigned,
                    other => TypeSpecifier::TypedefName(n(Identifier { name: other.to_string() })),
                };
                specifiers.push(n(DeclarationSpecifier::TypeSpecifier(n(ts))));
            }
        }
        CType::Struct { is_union, name: tag } => {
            if tag.is_empty() {
                return None;
            }
            specifiers.push(n(DeclarationSpecifier::TypeSpecifier(n(TypeSpecifier::Struct(n(StructType {
                kind: n(if *is_union { StructKind::Union } else { StructKind::Struct }),
                identifier: Some(n(Identifier { name: tag.clone() })),
                declarations: None,
            }))))));
        }
        CType::Enum(tag) => {
            if tag.is_empty() {
                return None;
            }
            specifiers.push(n(DeclarationSpecifier::TypeSpecifier(n(TypeSpecifier::Enum(n(EnumType {
                identifier: Some(n(Identifier { name: tag.clone() })),
                enumerators: Vec::new(),
            }))))));
        }
        _ => return None,
    }
    let mut derived = Vec::new();
    for _ in 0..ptrs {
        derived.push(n(DerivedDeclarator::Pointer(Vec::new())));
    }
    let declarator = Declarator {
        kind: n(DeclaratorKind::Identifier(n(Identifier { name: name.to_string() }))),
        derived,
        extensions: Vec::new(),
    };
    Some(n(Declaration {
        specifiers,
        declarators: vec![n(InitDeclarator { declarator: n(declarator), initializer: None })],
    }))
}
