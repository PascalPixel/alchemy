//! Weighted search passes over complete C translation units.
//! These explore source hypotheses; compilation determines whether they match.
//! Collected paths identify mutation sites, including moved inline arguments.

use super::c::{Node, types::{self, Types}};
use super::rng::Rng;
use std::collections::BTreeSet;

#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash, PartialOrd, Ord)]
pub enum Kind {
    Temporary, Expand, ReorderStatements, ReorderDeclarations, Cast,
    LocalType, GlobalType, FunctionType, SplitAssignment, Block, StructRef,
    FactorMultiply, FactorShift, Commutative, AddSub, Inequality,
    CompoundAssignment, Remove, ChainAssignment, LongChainAssignment,
    Inline, ConditionalBlock, AliasArray, RemoveVariable, ReverseCondition,
}

impl Kind {
    /// are omitted; weights of the supported passes keep their proportions.
    pub const GCC: [(Self, f64); 25] = [
        (Self::Temporary, 100.0), (Self::Expand, 20.0),
        (Self::ReorderStatements, 10.0), (Self::ReorderDeclarations, 10.0),
        (Self::Cast, 10.0), (Self::LocalType, 10.0), (Self::GlobalType, 5.0),
        (Self::FunctionType, 5.0), (Self::SplitAssignment, 10.0),
        (Self::Block, 10.0), (Self::StructRef, 10.0),
        (Self::FactorMultiply, 5.0), (Self::FactorShift, 5.0),
        (Self::Commutative, 5.0), (Self::AddSub, 5.0), (Self::Inequality, 5.0),
        (Self::CompoundAssignment, 5.0), (Self::Remove, 5.0),
        (Self::ChainAssignment, 5.0), (Self::LongChainAssignment, 3.0),
        (Self::Inline, 10.0), (Self::ConditionalBlock, 5.0),
        (Self::AliasArray, 3.0), (Self::RemoveVariable, 5.0),
        (Self::ReverseCondition, 1.0),
    ];

    pub fn name(self) -> &'static str {
        match self {
            Self::Temporary => "introduce or reuse a temporary",
            Self::Expand => "expand a variable's value",
            Self::ReorderStatements => "move a statement",
            Self::ReorderDeclarations => "move a declaration",
            Self::Cast => "cast an expression",
            Self::LocalType => "change a local type",
            Self::GlobalType => "change a global type",
            Self::FunctionType => "change a function signature",
            Self::SplitAssignment => "split an assignment",
            Self::Block => "wrap statements in a block",
            Self::StructRef => "change pointer or member access",
            Self::FactorMultiply => "factor a multiplication",
            Self::FactorShift => "factor a shift",
            Self::Commutative => "exchange operands",
            Self::AddSub => "exchange addition and subtraction",
            Self::Inequality => "adjust an inequality",
            Self::CompoundAssignment => "split or join a compound assignment",
            Self::Remove => "remove a cast, mask or control wrapper",
            Self::ChainAssignment => "chain assignments",
            Self::LongChainAssignment => "chain consecutive assignments",
            Self::Inline => "extract an inline helper",
            Self::ConditionalBlock => "wrap statements in a variable condition",
            Self::AliasArray => "combine locals into an array",
            Self::RemoveVariable => "reuse another variable",
            Self::ReverseCondition => "reverse a condition",
        }
    }
}

type Path = Vec<usize>;
fn at<'a>(node: &'a Node, path: &[usize]) -> &'a Node {
    if let Some((first, rest)) = path.split_first() { at(node.children()[*first], rest) } else { node }
}
fn at_mut<'a>(node: &'a mut Node, path: &[usize]) -> &'a mut Node {
    if let Some((first, rest)) = path.split_first() { at_mut(node.children_mut().swap_remove(*first), rest) } else { node }
}
fn child(path: &[usize], i: usize) -> Path { let mut p = path.to_vec(); p.push(i); p }
fn choose<T>(rng: &mut Rng, items: &[T]) -> Option<usize> { (!items.is_empty()).then(|| rng.below(items.len())) }
fn weighted(rng: &mut Rng, weights: impl Iterator<Item = f64> + Clone) -> usize {
    let mut pick = rng.random() * weights.clone().sum::<f64>();
    for (i, weight) in weights.enumerate() { pick -= weight; if pick < 0.0 { return i; } }
    0
}
fn integer(n: i64) -> Node { Node::Constant { ty: "int".into(), value: n.to_string() } }
fn unary(op: &str, expr: Node) -> Node { Node::UnaryOp { op: op.into(), expr: expr.boxed() } }
fn binary(op: &str, left: Node, right: Node) -> Node { Node::BinaryOp { op: op.into(), left: left.boxed(), right: right.boxed() } }
fn assign(left: Node, right: Node) -> Node { Node::Assignment { op: "=".into(), lvalue: left.boxed(), rvalue: right.boxed() } }
fn compound(items: Vec<Node>) -> Node { Node::Compound { block_items: items } }
fn statements(node: &Node) -> Option<&Vec<Node>> {
    match node { Node::Compound { block_items } => Some(block_items), Node::Case { stmts, .. } | Node::Default { stmts } => Some(stmts), _ => None }
}
fn statements_mut(node: &mut Node) -> &mut Vec<Node> {
    match node { Node::Compound { block_items } => block_items, Node::Case { stmts, .. } | Node::Default { stmts } => stmts, _ => unreachable!("a statement list") }
}
fn statement_path(path: &[usize], node: &Node, index: usize) -> Path { child(path, index + usize::from(matches!(node, Node::Case { .. }))) }
fn name(node: &Node) -> Option<&str> {
    match node { Node::Decl { name, .. } => name.as_deref(), Node::FuncDef { decl, .. } => name(decl), _ => None }
}
pub fn target_index(unit: &Node, target: &str) -> Option<usize> {
    let Node::FileAst { ext } = unit else { return None };
    ext.iter().position(|n| matches!(n, Node::FuncDef { .. }) && name(n) == Some(target))
}

struct Site { path: Path, start: usize, end: usize }
struct Sites { nodes: Vec<Site>, region: Option<(usize, usize)>, body: Path }
impl Sites {
    fn new(unit: &Node, target: &str, rng: &mut Rng) -> Option<Self> {
        let body = vec![target_index(unit, target)?, 1];
        let mut nodes = Vec::new();
        fn walk(node: &Node, path: Path, nodes: &mut Vec<Site>) {
            let start = nodes.len();
            nodes.push(Site { path: path.clone(), start, end: 0 });
            for (i, n) in node.children().iter().enumerate() { walk(n, child(&path, i), nodes); }
            nodes[start].end = nodes.len();
        }
        walk(at(unit, &body), body.clone(), &mut nodes);
        let (mut start, mut regions) = (None, Vec::new());
        for site in &nodes {
            if let Node::Pragma { string } = at(unit, &site.path) {
                if string == "_permuter randomizer start" { start = Some(site.end); }
                if string == "_permuter randomizer end" { if let Some(start) = start.take() { regions.push((start, site.start)); } }
            }
        }
        let region = choose(rng, &regions).map(|i| regions[i]);
        Some(Self { nodes, region, body })
    }
    fn allows(&self, path: &[usize]) -> bool {
        self.region.is_none_or(|(start, end)| self.nodes.iter().any(|s| s.path == path && s.start >= start && s.end <= end))
    }
    fn select(&self, unit: &Node, predicate: impl Fn(&Node) -> bool) -> Vec<Path> {
        self.nodes.iter().filter(|s| self.allows(&s.path) && predicate(at(unit, &s.path))).map(|s| s.path.clone()).collect()
    }
    fn blocks(&self, unit: &Node) -> Vec<Path> {
        self.nodes.iter().filter(|s| statements(at(unit, &s.path)).is_some()).map(|s| s.path.clone()).collect()
    }
    fn expressions(&self, unit: &Node) -> Vec<Path> {
        let mut paths = Vec::new();
        expressions(at(unit, &self.body), &self.body, false, &mut paths);
        paths.retain(|p| self.allows(p));
        paths
    }
}

/// Visit evaluated values, keeping assignment targets, field names, type
/// dimensions, sizeof operands and asm output constraints intact.
fn expressions(node: &Node, path: &[usize], value: bool, out: &mut Vec<Path>) {
    let is_expr = matches!(node, Node::Id { .. } | Node::Constant { .. } | Node::UnaryOp { .. } | Node::BinaryOp { .. } | Node::Cast { .. } | Node::TernaryOp { .. } | Node::Assignment { .. } | Node::ArrayRef { .. } | Node::StructRef { .. } | Node::FuncCall { .. } | Node::ExprList { .. });
    if value && is_expr { out.push(path.to_vec()); }
    match node {
        Node::Decl { init: Some(init), .. } => {
            // Decl's single children begin with type, then initializer.
            if !matches!(init.as_ref(), Node::InitList { .. }) { expressions(init, &child(path, 1), true, out); }
        }
        Node::Decl { .. } | Node::Typedef { .. } | Node::Typename { .. } | Node::TypeDecl { .. } | Node::Case { .. } | Node::Asm { .. } => {
            if let Node::Case { stmts, .. } = node { for (i, n) in stmts.iter().enumerate() { expressions(n, &child(path, i + 1), false, out); } }
        }
        Node::UnaryOp { op, .. } if op == "sizeof" => {}
        Node::StructRef { name, op, .. } => expressions(name, &child(path, 0), op == "->", out),
        Node::Cast { expr, .. } => expressions(expr, &child(path, 1), true, out),
        Node::FuncCall { name, args } => {
            expressions(name, &child(path, 0), false, out);
            if let Some(args) = args { expressions(args, &child(path, 1), false, out); }
        }
        _ => for (i, n) in node.children().iter().enumerate() {
            let val = match node {
                Node::Assignment { .. } => i == 1,
                Node::UnaryOp { op, .. } => !matches!(op.as_str(), "&" | "++" | "--" | "p++" | "p--"),
                Node::Compound { .. } | Node::Default { .. } | Node::Label { .. } => false,
                Node::If { .. } | Node::While { .. } | Node::DoWhile { .. } | Node::Switch { .. } => i == 0,
                Node::For { stmt, .. } => !std::ptr::eq(*n, stmt.as_ref()),
                _ => true,
            };
            expressions(n, &child(path, i), val, out);
        }
    }
}

fn mentioned(node: &Node) -> BTreeSet<String> {
    let mut paths = Vec::new();
    // Includes IDs used as lvalues, excluding the field part of a member.
    fn visit(node: &Node, out: &mut Vec<String>) {
        if let Node::Id { name } = node { out.push(name.clone()); }
        if let Node::StructRef { name, .. } = node { visit(name, out); }
        else { for n in node.children() { visit(n, out); } }
    }
    visit(node, &mut paths);
    paths.into_iter().collect()
}
fn fresh(unit: &Node, stem: &str) -> String {
    fn collect(n: &Node, out: &mut BTreeSet<String>) {
        match n {
            Node::Id { name } | Node::Typedef { name, .. } | Node::Label { name, .. } | Node::Enumerator { name, .. } => { out.insert(name.clone()); }
            Node::Decl { name: Some(name), .. } => { out.insert(name.clone()); }
            _ => {}
        }
        for n in n.children() { collect(n, out); }
    }
    let mut names = BTreeSet::new(); collect(unit, &mut names);
    if !names.contains(stem) { return stem.into(); }
    (2..).map(|i| format!("{stem}{i}")).find(|n| !names.contains(n)).unwrap()
}
fn effectful(n: &Node) -> bool {
    matches!(n, Node::FuncCall { .. } | Node::Assignment { .. })
        || matches!(n, Node::UnaryOp { op, .. } if matches!(op.as_str(), "++" | "--" | "p++" | "p--"))
        || n.children().into_iter().any(effectful)
}
fn constant(n: &Node) -> Option<i64> {
    let Node::Constant { value, .. } = n else { return None };
    let value = value.trim_end_matches(['u', 'U', 'l', 'L']);
    if let Some(hex) = value.strip_prefix("0x").or_else(|| value.strip_prefix("0X")) { i64::from_str_radix(hex, 16).ok() }
    else if value.starts_with('0') && value.len() > 1 { i64::from_str_radix(&value[1..], 8).ok() }
    else { value.parse().ok() }
}
fn random_type(rng: &mut Rng) -> Node {
    let sign = if rng.random() < 0.5 { "unsigned " } else { "" };
    let names = ["char", "short", "int", "long", "long long"];
    let name = names[weighted(rng, [1.0, 1.0, 2.0, 0.5, 0.5].into_iter())];
    let mut ty = types::basic(&format!("{sign}{name}"));
    if rng.random() < 0.5 { if let Node::TypeDecl { quals, .. } = &mut ty { quals.push("volatile".into()); } }
    ty
}

/// Retry failed passes with the same weights. A bound prevents an empty or
/// opaque function from trapping a worker forever.
pub fn mutate(unit: &mut Node, target: &str, rng: &mut Rng) -> Option<super::mutate::Step> {
    for _ in 0..128 {
        let kind = Kind::GCC[weighted(rng, Kind::GCC.iter().map(|(_, w)| *w))].0;
        let before = rng.clone();
        if apply(unit, target, kind, rng) {
            return Some(super::mutate::Step { kind: super::mutate::Kind::Full(kind), rng: before });
        }
    }
    None
}

pub fn apply(unit: &mut Node, target: &str, kind: Kind, rng: &mut Rng) -> bool {
    let Some(sites) = Sites::new(unit, target, rng) else { return false };
    // Each pass either completes or leaves the input untouched, so failed
    // attempts and minimizer replays cannot leak half a transformation.
    let mut trial = unit.clone();
    let result = match kind {
        Kind::Temporary => temporary(&mut trial, target, &sites, rng),
        Kind::Expand => expand(&mut trial, &sites, rng),
        Kind::LocalType | Kind::GlobalType | Kind::FunctionType => change_type(&mut trial, target, &sites, kind, rng),
        Kind::ReorderStatements | Kind::ReorderDeclarations => reorder(&mut trial, &sites, kind, rng),
        Kind::SplitAssignment | Kind::ChainAssignment | Kind::LongChainAssignment => assignments(&mut trial, &sites, kind, rng),
        Kind::Block | Kind::ConditionalBlock => wrap(&mut trial, &sites, kind, rng),
        Kind::Inline => inline(&mut trial, target, &sites, rng),
        Kind::AliasArray | Kind::RemoveVariable => variables(&mut trial, target, &sites, kind, rng),
        _ => expression(&mut trial, target, &sites, kind, rng),
    };
    if result.is_some() && trial != *unit { *unit = trial; true } else { false }
}

fn expression(unit: &mut Node, target: &str, sites: &Sites, kind: Kind, rng: &mut Rng) -> Option<()> {
    let candidates = if kind == Kind::Cast { sites.expressions(unit) } else { sites.select(unit, |n| match kind {
        Kind::Commutative => matches!(n, Node::BinaryOp { op, .. } if ["+", "*", "&", "|", "^", "<", ">", "<=", ">=", "==", "!="].contains(&op.as_str())),
        Kind::AddSub => matches!(n, Node::BinaryOp { op, .. } if op == "+" || op == "-"),
        Kind::Inequality => matches!(n, Node::BinaryOp { op, .. } if ["<", ">", "<=", ">="].contains(&op.as_str())),
        Kind::FactorMultiply => matches!(n, Node::BinaryOp { op, left, right } if op == "*" && (constant(left).is_some() || constant(right).is_some())),
        Kind::FactorShift => matches!(n, Node::BinaryOp { op, right, .. } if (op == "<<" || op == ">>") && constant(right).is_some()),
        Kind::CompoundAssignment => matches!(n, Node::Assignment { op, lvalue, rvalue } if op != "=" || matches!(rvalue.as_ref(), Node::BinaryOp { op, left, .. } if left == lvalue && ["+", "-", "*", "/", "%", "<<", ">>", "&", "|", "^"].contains(&op.as_str()))),
        Kind::StructRef => matches!(n, Node::StructRef { .. } | Node::ArrayRef { .. }) || matches!(n, Node::UnaryOp { op, expr } if op == "*" && matches!(expr.as_ref(), Node::BinaryOp { op, .. } if op == "+")),
        Kind::ReverseCondition => matches!(n, Node::If { iffalse: Some(_), .. } | Node::TernaryOp { .. }),
        Kind::Remove => matches!(n, Node::Cast { .. } | Node::If { iffalse: None, .. } | Node::While { .. } | Node::DoWhile { .. }) || matches!(n, Node::BinaryOp { op, left, right } if op == "&" && (constant(left).is_some() || constant(right).is_some())),
        _ => false,
    }) };
    let path = &candidates[choose(rng, &candidates)?];
    let node = at(unit, path).clone();
    let replacement = match (kind, node) {
        (Kind::Cast, expr) => {
            let types = Types::new(unit, target);
            if !types.arithmetic(&types.decayed(&expr)?) { return None; }
            let mut ty = random_type(rng);
            if let Node::TypeDecl { quals, .. } = &mut ty { quals.clear(); }
            Node::Cast { to_type: types::typename(ty).boxed(), expr: expr.boxed() }
        }
        (Kind::Commutative, Node::BinaryOp { op, left, right }) => {
            let op = match op.as_str() { "<" => ">", ">" => "<", "<=" => ">=", ">=" => "<=", _ => &op };
            binary(op, *right, *left)
        }
        (Kind::AddSub, Node::BinaryOp { op, left, right }) => {
            let types = Types::new(unit, target);
            if !types.arithmetic(&types.decayed(&right)?) { return None; }
            let right = if let Node::UnaryOp { op, expr } = right.as_ref() { if op == "-" { *expr.clone() } else { unary("-", *right) } } else { unary("-", *right) };
            binary(if op == "+" { "-" } else { "+" }, *left, right)
        }
        (Kind::Inequality, Node::BinaryOp { op, left, right }) => {
            let types = Types::new(unit, target);
            if !types.integral(&types.decayed(&left)?) || !types.integral(&types.decayed(&right)?) { return None; }
            let (op, delta) = match op.as_str() { "<" => ("<=", -1), ">=" => (">", -1), ">" => (">=", 1), "<=" => ("<", 1), _ => unreachable!() };
            binary(op, *left, binary(if delta == 1 { "+" } else { "-" }, *right, integer(1)))
        }
        (Kind::FactorShift, Node::BinaryOp { op, left, right }) => {
            let n = constant(&right)?;
            if !(2..64).contains(&n) { return None; }
            let first = 1 + rng.below(n as usize - 1) as i64;
            binary(&op, binary(&op, *left, integer(first)), integer(n - first))
        }
        (Kind::FactorMultiply, Node::BinaryOp { left, right, .. }) => {
            let (n, expr) = if let Some(n) = constant(&left) { (n, *right) } else { (constant(&right)?, *left) };
            let limit = n.checked_abs()?.min(21);
            let factors: Vec<_> = (2..limit).filter(|f| n % f == 0).collect();
            let a = factors[choose(rng, &factors)?];
            binary("*", binary("*", expr, integer(a)), integer(n / a))
        }
        (Kind::CompoundAssignment, Node::Assignment { op, lvalue, rvalue }) => {
            if op == "=" {
                let Node::BinaryOp { op, right, .. } = *rvalue else { return None };
                Node::Assignment { op: format!("{op}="), lvalue, rvalue: right }
            } else { assign(*lvalue.clone(), binary(op.trim_end_matches('='), *lvalue, *rvalue)) }
        }
        (Kind::StructRef, Node::StructRef { name, op, field }) => {
            if op == "->" { Node::StructRef { name: unary("*", *name).boxed(), op: ".".into(), field } }
            else if let Node::UnaryOp { op, expr } = name.as_ref() {
                if op == "*" { Node::StructRef { name: expr.clone(), op: "->".into(), field } }
                else { Node::StructRef { name: unary("&", *name).boxed(), op: "->".into(), field } }
            } else { Node::StructRef { name: unary("&", *name).boxed(), op: "->".into(), field } }
        }
        (Kind::StructRef, Node::ArrayRef { name, subscript }) => unary("*", binary("+", *name, *subscript)),
        (Kind::StructRef, Node::UnaryOp { expr, .. }) => {
            let Node::BinaryOp { left, right, .. } = *expr else { return None };
            Node::ArrayRef { name: left, subscript: right }
        }
        (Kind::ReverseCondition, Node::If { cond, iftrue, iffalse: Some(iffalse) }) => Node::If { cond: negate(*cond).boxed(), iftrue: iffalse, iffalse: Some(iftrue) },
        (Kind::ReverseCondition, Node::TernaryOp { cond, iftrue, iffalse }) => Node::TernaryOp { cond: negate(*cond).boxed(), iftrue: iffalse, iffalse: iftrue },
        (Kind::Remove, Node::Cast { expr, .. }) => *expr,
        (Kind::Remove, Node::BinaryOp { left, right, .. }) => if constant(&left).is_some() { *right } else { *left },
        (Kind::Remove, Node::If { iftrue, .. }) => *iftrue,
        (Kind::Remove, Node::While { stmt, .. } | Node::DoWhile { stmt, .. }) => *stmt,
        _ => return None,
    };
    *at_mut(unit, path) = replacement;
    Some(())
}
fn negate(n: Node) -> Node { if let Node::UnaryOp { op, expr } = &n { if op == "!" { return *expr.clone(); } } unary("!", n) }

fn change_type(unit: &mut Node, target: &str, sites: &Sites, kind: Kind, rng: &mut Rng) -> Option<()> {
    let types = Types::new(unit, target);
    let mut names = BTreeSet::new();
    for site in &sites.nodes { if sites.allows(&site.path) { if let Node::Id { name } = at(unit, &site.path) { names.insert(name.clone()); } } }
    if kind == Kind::LocalType {
        let candidates = sites.nodes.iter().filter(|s| matches!(at(unit, &s.path), Node::Decl { name: Some(name), ty, .. } if names.contains(name) && types.integral(ty))).map(|s| s.path.clone()).collect::<Vec<_>>();
        let path = &candidates[choose(rng, &candidates)?];
        let Node::Decl { name, ty, quals, .. } = at_mut(unit, path) else { return None };
        **ty = random_type(rng); types::set_name(ty, name.as_deref()); quals.clear();
        return Some(());
    }
    let mut called = vec![target.to_string()];
    for site in &sites.nodes {
        if sites.allows(&site.path) { if let Node::FuncCall { name, .. } = at(unit, &site.path) { if let Node::Id { name } = name.as_ref() { called.push(name.clone()); } } }
    }
    let Node::FileAst { ext } = unit else { return None };
    if kind == Kind::GlobalType {
        let candidates: Vec<_> = ext.iter().filter_map(|n| match n {
            Node::Decl { name: Some(name), ty, .. } if names.contains(name) && !types.locals.contains(name) && !matches!(ty.as_ref(), Node::FuncDecl { .. }) => Some(name.clone()), _ => None
        }).collect();
        let selected = &candidates[choose(rng, &candidates)?];
        fn inner(ty: &mut Node, types: &Types, rng: &mut Rng) -> Option<()> {
            match ty {
                Node::TypeDecl { .. } if types.integral(ty) => { *ty = random_type(rng); Some(()) }
                Node::PtrDecl { ty, .. } | Node::ArrayDecl { ty, .. } => inner(ty, types, rng),
                _ => None,
            }
        }
        let Node::Decl { ty, .. } = ext.iter().find(|n| name(n) == Some(selected))? else { return None };
        let mut new = *ty.clone(); inner(&mut new, &types, rng)?; types::set_name(&mut new, Some(selected));
        for n in ext { if let Node::Decl { name: Some(name), ty, quals, .. } = n { if name == selected { **ty = new.clone(); quals.clear(); } } }
        return Some(());
    }
    called.sort(); called.dedup();
    let selected = &called[choose(rng, &called)?];
    let decl_type = |n: &Node| match n { Node::Decl { ty, .. } => Some(*ty.clone()), Node::FuncDef { decl, .. } => if let Node::Decl { ty, .. } = decl.as_ref() { Some(*ty.clone()) } else { None }, _ => None };
    let original = ext.iter().find(|n| name(n) == Some(selected) && matches!(n, Node::FuncDef { .. })).or_else(|| ext.iter().find(|n| name(n) == Some(selected)))?;
    let Node::FuncDecl { args, ty: ret } = decl_type(original)? else { return None };
    let param = if rng.random() < 0.5 { None } else {
        let Node::ParamList { params } = args.as_deref()? else { return None };
        Some(choose(rng, params)?)
    };
    let old = if let Some(i) = param {
        let Node::ParamList { params } = args.as_deref()? else { return None };
        match &params[i] { Node::Decl { ty, .. } | Node::Typename { ty, .. } => ty.as_ref(), _ => return None }
    } else { ret.as_ref() };
    if !types.integral(old) && !(param.is_none() && matches!(types.resolve(old)?, Node::TypeDecl { ty, .. } if matches!(ty.as_ref(), Node::IdentifierType { names } if names == &["void"]))) { return None; }
    let new = random_type(rng);
    for item in ext {
        if name(item) != Some(selected) { continue; }
        let decl = if let Node::FuncDef { decl, .. } = item { decl.as_mut() } else { item };
        let Node::Decl { ty, name, .. } = decl else { continue };
        let Node::FuncDecl { args, ty: ret } = ty.as_mut() else { continue };
        if let Some(i) = param {
            let Node::ParamList { params } = args.as_deref_mut()? else { return None };
            match params.get_mut(i)? {
                Node::Decl { name, ty, quals, .. } => { **ty = new.clone(); types::set_name(ty, name.as_deref()); quals.clear(); }
                Node::Typename { ty, .. } => **ty = new.clone(),
                _ => return None,
            }
        } else { **ret = new.clone(); types::set_name(ret, name.as_deref()); }
    }
    Some(())
}

fn reorder(unit: &mut Node, sites: &Sites, kind: Kind, rng: &mut Rng) -> Option<()> {
    let mut candidates = Vec::new();
    for path in sites.blocks(unit) {
        let block = at(unit, &path);
        let stmts = statements(block)?;
        for (i, stmt) in stmts.iter().enumerate() {
            if !sites.allows(&statement_path(&path, block, i)) { continue; }
            let decl = matches!(stmt, Node::Decl { .. });
            if decl != (kind == Kind::ReorderDeclarations) || matches!(stmt, Node::Pragma { .. } | Node::Case { .. } | Node::Default { .. }) { continue; }
            for j in 0..stmts.len() {
                if i == j || !sites.allows(&statement_path(&path, block, j)) { continue; }
                let range = &stmts[i.min(j)..=i.max(j)];
                if range.iter().any(|n| matches!(n, Node::Pragma { .. })) { continue; }
                if decl {
                    if !range.iter().all(|n| matches!(n, Node::Decl { init: None, .. })) { continue; }
                } else if range.iter().any(|n| matches!(n, Node::Decl { .. } | Node::Case { .. } | Node::Default { .. })) { continue; }
                candidates.push((path.clone(), i, j));
            }
        }
    }
    if candidates.is_empty() { return None; }
    let index = weighted(rng, candidates.iter().map(|(_, a, b)| ((*a as f64 - *b as f64).abs() + 1.0).powf(-1.5)));
    let (path, from, to) = &candidates[index];
    let stmts = statements_mut(at_mut(unit, path));
    let stmt = stmts.remove(*from); stmts.insert(*to, stmt);
    Some(())
}

fn assignments(unit: &mut Node, sites: &Sites, kind: Kind, rng: &mut Rng) -> Option<()> {
    let mut candidates = Vec::new();
    for path in sites.blocks(unit) {
        let block = at(unit, &path);
        let stmts = statements(block)?;
        for (i, stmt) in stmts.iter().enumerate() {
            if !sites.allows(&statement_path(&path, block, i)) { continue; }
            let Node::Assignment { op, lvalue, rvalue } = stmt else { continue };
            if op != "=" { continue; }
            if kind == Kind::SplitAssignment {
                if matches!(lvalue.as_ref(), Node::Id { .. }) && matches!(rvalue.as_ref(), Node::BinaryOp { .. }) { candidates.push((path.clone(), i, i)); }
                continue;
            }
            if effectful(rvalue) { continue; }
            for (j, next) in stmts.iter().enumerate().skip(i + 1).take(if kind == Kind::LongChainAssignment { stmts.len() } else { 4 }) {
                if !sites.allows(&statement_path(&path, block, j)) || stmts[i + 1..=j].iter().any(|n| matches!(n, Node::Pragma { .. })) { break; }
                if matches!(next, Node::Assignment { op, rvalue: rhs, .. } if op == "=" && (rhs == rvalue || (kind == Kind::ChainAssignment && rhs == lvalue))) {
                    if kind != Kind::LongChainAssignment || j > i + 1 { candidates.push((path.clone(), i, j)); }
                } else if kind == Kind::LongChainAssignment { break; }
            }
        }
    }
    let (path, i, j) = &candidates[choose(rng, &candidates)?];
    let stmts = statements_mut(at_mut(unit, path));
    let Node::Assignment { lvalue, rvalue, .. } = stmts[*i].clone() else { return None };
    if kind == Kind::SplitAssignment {
        let Node::BinaryOp { op, left, right } = *rvalue else { return None };
        let (first, rest) = if rng.random() < 0.5 { (*left, binary(&op, *lvalue.clone(), *right)) } else { (*right, binary(&op, *left, *lvalue.clone())) };
        stmts[*i] = assign(*lvalue.clone(), rest);
        stmts.insert(*i, assign(*lvalue, first));
    } else {
        let mut new = stmts[*i].clone();
        let indices = if kind == Kind::LongChainAssignment { ((*i + 1)..=*j).collect::<Vec<_>>() } else { vec![*j] };
        for &index in &indices {
            let Node::Assignment { lvalue, .. } = stmts[index].clone() else { return None };
            new = assign(*lvalue, new);
        }
        stmts[*i] = new;
        for &index in indices.iter().rev() { stmts.remove(index); }
    }
    Some(())
}

/// Closest statement list containing an expression. Expression paths are
/// consumed before changing the list, so insertion cannot invalidate them.
fn place(unit: &Node, path: &[usize]) -> Option<(Path, usize)> {
    for len in (0..path.len()).rev() {
        let block = at(unit, &path[..len]);
        if statements(block).is_some() {
            let i = path[len].checked_sub(usize::from(matches!(block, Node::Case { .. })))?;
            return Some((path[..len].to_vec(), i));
        }
    }
    None
}

fn writes(node: &Node) -> BTreeSet<String> {
    let mut out = BTreeSet::new();
    fn visit(n: &Node, out: &mut BTreeSet<String>) {
        match n {
            Node::Assignment { lvalue, .. } => out.extend(mentioned(lvalue)),
            Node::UnaryOp { op, expr } if ["++", "--", "p++", "p--", "&"].contains(&op.as_str()) => out.extend(mentioned(expr)),
            Node::Decl { name: Some(name), .. } => { out.insert(name.clone()); }
            _ => {}
        }
        for n in n.children() { visit(n, out); }
    }
    visit(node, &mut out); out
}

fn temporary(unit: &mut Node, target: &str, sites: &Sites, rng: &mut Rng) -> Option<()> {
    let types = Types::new(unit, target);
    let candidates = sites.expressions(unit);
    if candidates.is_empty() { return None; }
    let index = weighted(rng, candidates.iter().map(|p| if matches!(at(unit, p), Node::Id { .. } | Node::Constant { .. }) { 0.5 } else { 1.0 }));
    let path = &candidates[index];
    let original = at(unit, path).clone();
    let (block_path, stmt_index) = place(unit, path)?;
    let block = at(unit, &block_path);
    let stmts = statements(block)?;
    // Declarations belong at the front of a compound, never directly after
    // a case label. Skip initialized declarations here; expand handles them.
    if !matches!(block, Node::Compound { .. }) || matches!(stmts[stmt_index], Node::Decl { .. }) { return None; }
    let mut ty = types.decayed(&original)?;
    if matches!(types.resolve(&ty)?, Node::TypeDecl { ty, .. } if matches!(ty.as_ref(), Node::IdentifierType { names } if names == &["void"])) { return None; }
    let lvalue = matches!(&original, Node::Id { .. } | Node::StructRef { .. } | Node::ArrayRef { .. }) || matches!(&original, Node::UnaryOp { op, .. } if op == "*");
    let pointer = lvalue && (rng.random() < 0.05 || matches!(types.resolve(&ty)?, Node::TypeDecl { ty, .. } if matches!(ty.as_ref(), Node::Struct { .. } | Node::Union { .. })));
    let expr = if pointer { ty = types::pointer(types.expr(&original)?); unary("&", original.clone()) } else { original.clone() };
    let at_use = effectful(&original) || rng.random() < 0.1;
    let dependencies = mentioned(&expr);
    let mut insertions = vec![stmt_index];
    if !at_use {
        for i in (0..stmt_index).rev() {
            if matches!(stmts[i], Node::Decl { .. } | Node::Pragma { .. }) || !sites.allows(&statement_path(&block_path, block, i)) { break; }
            if !writes(&stmts[i]).is_disjoint(&dependencies) || effectful(&stmts[i]) { break; }
            insertions.push(i);
        }
    }
    let insert = insertions[weighted(rng, insertions.iter().enumerate().map(|(i, _)| 1.0 / (i + 1) as f64))];
    let mut reuse = Vec::new();
    for n in &stmts[..insert] {
        let Node::Decl { name: Some(name), ty: var_type, .. } = n else { continue };
        if dependencies.contains(name) || !types.similar(var_type, &ty) || !types.locals.contains(name) { continue; }
        // The previous value must be dead until the next plain write.
        let mut dead = true;
        for stmt in &stmts[insert..] {
            if let Node::Assignment { op, lvalue, rvalue } = stmt {
                if op == "=" && matches!(lvalue.as_ref(), Node::Id { name: n } if n == name) && !mentioned(rvalue).contains(name) { break; }
            }
            if mentioned(stmt).contains(name) { dead = false; break; }
        }
        if dead { reuse.push(name.clone()); }
    }
    let reused = rng.random() < 0.5 && !reuse.is_empty();
    let var = if reused { reuse[rng.below(reuse.len())].clone() } else { fresh(unit, "new_var") };
    let replacement = if at_use { assign(Node::id(&var), expr.clone()) } else { Node::id(&var) };
    let replacement = if pointer { unary("*", replacement) } else { replacement };
    let mut replacements = vec![path.clone()];
    if !at_use && !effectful(&original) && rng.random() < 0.4 {
        // Share a value inside this statement; no cross-statement lifetime
        // guesses are needed to make repeated expressions reachable.
        for p in sites.expressions(unit) {
            if p != *path && place(unit, &p) == Some((block_path.clone(), stmt_index)) && at(unit, &p) == &original { replacements.push(p); }
        }
    }
    for p in replacements { *at_mut(unit, &p) = replacement.clone(); }
    let stmts = statements_mut(at_mut(unit, &block_path));
    if !at_use { stmts.insert(insert, assign(Node::id(&var), expr)); }
    if !reused {
        if types.integral(&ty) && rng.random() < 0.3 { ty = random_type(rng); }
        let end = stmts.iter().take_while(|n| matches!(n, Node::Decl { .. } | Node::Pragma { .. })).count();
        // Insert after any existing local typedefs/type definitions. This
        // matters when the expression's type is declared in this block.
        stmts.insert(end, types::declaration(&var, ty));
    }
    Some(())
}

fn expand(unit: &mut Node, sites: &Sites, rng: &mut Rng) -> Option<()> {
    let mut candidates = Vec::new();
    for path in sites.blocks(unit) {
        let block = at(unit, &path);
        let stmts = statements(block)?;
        for (i, stmt) in stmts.iter().enumerate() {
            let (name, expr) = match stmt {
                Node::Decl { name: Some(name), init: Some(expr), .. } if !matches!(expr.as_ref(), Node::InitList { .. }) => (name, expr.as_ref()),
                Node::Assignment { op, lvalue, rvalue } if op == "=" => { let Node::Id { name } = lvalue.as_ref() else { continue }; (name, rvalue.as_ref()) }
                _ => continue,
            };
            if !sites.allows(&statement_path(&path, block, i)) || effectful(expr) { continue; }
            let mut dependencies = mentioned(expr); dependencies.insert(name.clone());
            let mut reads = Vec::new();
            for (j, stmt) in stmts.iter().enumerate().skip(i + 1) {
                if !writes(stmt).is_disjoint(&dependencies) || matches!(stmt, Node::Pragma { .. }) { break; }
                let mut paths = Vec::new(); expressions(stmt, &statement_path(&path, block, j), false, &mut paths);
                reads.extend(paths.into_iter().filter(|p| sites.allows(p) && matches!(at(unit, p), Node::Id { name: n } if n == name)));
                if effectful(stmt) { break; }
            }
            if !reads.is_empty() { candidates.push((path.clone(), i, expr.clone(), reads)); }
        }
    }
    let (path, i, expr, reads) = &candidates[choose(rng, &candidates)?];
    let all = rng.random() < 0.3;
    let chosen = rng.below(reads.len());
    for (index, p) in reads.iter().enumerate() { if all || index == chosen { *at_mut(unit, p) = expr.clone(); } }
    // Keep assignments unless every use in the rest of the function was
    // replaced. The remove-variable pass explores broader reuse separately.
    if all && rng.random() < 0.8 {
        let block = at(unit, path);
        let stmt = &statements(block)?[*i];
        let var = match stmt { Node::Decl { name: Some(name), .. } => name.clone(), Node::Assignment { lvalue, .. } => if let Node::Id { name } = lvalue.as_ref() { name.clone() } else { return Some(()) }, _ => return Some(()) };
        let later = &statements(block)?[*i + 1..];
        if !later.iter().any(|n| mentioned(n).contains(&var)) {
            let stmts = statements_mut(at_mut(unit, path));
            if let Node::Decl { init, .. } = &mut stmts[*i] { *init = None; } else { stmts.remove(*i); }
        }
    }
    Some(())
}

fn wrap(unit: &mut Node, sites: &Sites, kind: Kind, rng: &mut Rng) -> Option<()> {
    let mut candidates = Vec::new();
    for path in sites.blocks(unit) {
        let block = at(unit, &path);
        for (i, stmt) in statements(block)?.iter().enumerate() {
            if sites.allows(&statement_path(&path, block, i)) && !matches!(stmt, Node::Decl { .. } | Node::Pragma { .. } | Node::Case { .. } | Node::Default { .. }) { candidates.push((path.clone(), i)); }
        }
    }
    let (path, start) = &candidates[choose(rng, &candidates)?];
    let block = at(unit, path);
    let stmts = statements(block)?;
    let last = stmts.iter().enumerate().skip(*start).take_while(|(i, n)| sites.allows(&statement_path(path, block, *i)) && !matches!(n, Node::Decl { .. } | Node::Pragma { .. } | Node::Case { .. } | Node::Default { .. })).count();
    let end = *start + 1 + rng.below(last);
    let cond = if kind == Kind::ConditionalBlock {
        let mut names = BTreeSet::new();
        for stmt in &stmts[*start..end] { names.extend(mentioned(stmt)); }
        let names: Vec<_> = names.into_iter().collect();
        Node::id(&names[choose(rng, &names)?])
    } else { integer(1) };
    let stmts = statements_mut(at_mut(unit, path));
    let body = compound(stmts.drain(*start..end).collect()).boxed();
    let wrapper = if kind == Kind::Block && rng.random() < 0.5 { Node::DoWhile { cond: integer(0).boxed(), stmt: body } }
        else { Node::If { cond: cond.boxed(), iftrue: body, iffalse: None } };
    stmts.insert(*start, wrapper);
    Some(())
}

fn inline(unit: &mut Node, target: &str, sites: &Sites, rng: &mut Rng) -> Option<()> {
    let types = Types::new(unit, target);
    let candidates = sites.expressions(unit);
    if candidates.is_empty() { return None; }
    let selected = weighted(rng, candidates.iter().map(|p| match at(unit, p) { Node::Id { .. } => 0.05, Node::Constant { .. } => 0.01, _ => 1.0 }));
    let path = &candidates[selected];
    let expr = at(unit, path).clone();
    if effectful(&expr) { return None; }
    let ret = types.decayed(&expr)?;
    let helper = fresh(unit, "inline_fn");
    let mut params = Vec::new();
    let mut args = Vec::new();
    let mut body = expr.clone();
    let mut values = Vec::new(); expressions(&body, &[], true, &mut values);
    // Cut pure subexpressions into parameters, including every local ID.
    // A path prefix already cut must never be visited again.
    let mut cuts: Vec<Path> = Vec::new();
    for p in values {
        if cuts.iter().any(|c| p.starts_with(c)) { continue; }
        let n = at(&expr, &p);
        let local = matches!(n, Node::Id { name } if types.locals.contains(name));
        if !local && (p.is_empty() || rng.random() >= 0.25) { continue; }
        let ty = types.decayed(n)?;
        let arg = fresh(unit, &format!("arg{}", params.len()));
        params.push(types::declaration(&arg, ty)); args.push(n.clone());
        *at_mut(&mut body, &p) = Node::id(arg); cuts.push(p);
    }
    if params.is_empty() { return None; }
    // Declared locals hidden inside an lvalue/address expression may not
    // have been cut by the value visitor. Decline instead of emitting an
    // out-of-scope name in the helper.
    if !mentioned(&body).is_disjoint(&types.locals) { return None; }
    let call = Node::FuncCall { name: Node::id(&helper).boxed(), args: Some(Node::ExprList { exprs: args }.boxed()) };
    *at_mut(unit, path) = call;
    let mut decl = types::declaration(&helper, Node::FuncDecl { args: Some(Node::ParamList { params }.boxed()), ty: ret.boxed() });
    if let Node::Decl { storage, funcspec, .. } = &mut decl { storage.push("static".into()); funcspec.push("inline".into()); }
    let helper = Node::FuncDef { decl: decl.boxed(), param_decls: vec![], body: compound(vec![Node::Return { expr: Some(body.boxed()) }]).boxed() };
    let index = target_index(unit, target)?;
    let Node::FileAst { ext } = unit else { return None };
    ext.insert(index, helper);
    Some(())
}

fn variables(unit: &mut Node, target: &str, sites: &Sites, kind: Kind, rng: &mut Rng) -> Option<()> {
    let types = Types::new(unit, target);
    let body = at(unit, &sites.body);
    let stmts = statements(body)?;
    let mut mentioned_names = BTreeSet::new();
    for path in sites.select(unit, |n| matches!(n, Node::Id { .. })) { if let Node::Id { name } = at(unit, &path) { mentioned_names.insert(name.clone()); } }
    let mut candidates = Vec::new();
    for (i, decl) in stmts.iter().enumerate() {
        let Node::Decl { name: Some(a), ty: ta, init: None, .. } = decl else { continue };
        if !mentioned_names.contains(a) || matches!(types.resolve(ta)?, Node::ArrayDecl { .. }) { continue; }
        for (j, decl) in stmts.iter().enumerate().skip(i + 1) {
            let Node::Decl { name: Some(b), ty: tb, init: None, .. } = decl else { continue };
            if mentioned_names.contains(b) && types.similar(ta, tb) { candidates.push((i, j, a.clone(), b.clone(), *ta.clone())); }
        }
    }
    let (i, j, a, b, ty) = candidates.get(choose(rng, &candidates)?)?.clone();
    let var = if kind == Kind::AliasArray { fresh(unit, "new_var") } else { b.clone() };
    fn replace(node: &mut Node, a: &str, b: &str, var: &str, array: bool) {
        if let Node::Id { name } = node {
            if name == a || (array && name == b) {
                *node = if array { Node::ArrayRef { name: Node::id(var).boxed(), subscript: integer(i64::from(name == b)).boxed() } } else { Node::id(var) };
                return;
            }
        }
        // The field of a member is a name, not a variable reference.
        if let Node::StructRef { name, .. } = node { replace(name, a, b, var, array); }
        else { for n in node.children_mut() { replace(n, a, b, var, array); } }
    }
    let body = at_mut(unit, &sites.body);
    replace(body, &a, &b, &var, kind == Kind::AliasArray);
    let stmts = statements_mut(body);
    if kind == Kind::AliasArray {
        stmts[i] = types::declaration(&var, Node::ArrayDecl { ty: ty.boxed(), dim: Some(integer(2).boxed()), dim_quals: vec![] }); stmts.remove(j);
    } else { stmts.remove(i); }
    Some(())
}

#[cfg(test)]
mod tests {
    use super::*;

    const SOURCE: &str = "typedef unsigned short U; struct S { int member; }; extern int global; extern int called(int);\nint target(int a, int b, struct S *p) { int x; int y; int z; int spare; x = a + b; y = a + b; z = a + b; spare = 3; x = x + 2; if (a < b) { y = x * 12; } else { y = x << 4; } z = (int)(y & 255); x = called(z); return p->member + x + global; }\n";

    fn parsed(source: &str) -> Node {
        let mut unit = super::super::c::parse(source).unwrap();
        unit.extract_function("target").unwrap(); unit
    }

    #[test]
    fn supported_families_generate_replayable_c() {
        let base = parsed(SOURCE);
        for (kind, _) in Kind::GCC {
            let mut count = 0;
            for seed in 0..40 {
                let mut unit = base.clone();
                if !apply(&mut unit, "target", kind, &mut Rng::new(seed)) { assert_eq!(unit, base); continue; }
                count += 1;
                let mut replay = base.clone();
                assert!(apply(&mut replay, "target", kind, &mut Rng::new(seed)));
                assert_eq!(unit, replay, "{kind:?}, seed {seed}");
                super::super::c::parse(&unit.print()).unwrap_or_else(|e| panic!("{kind:?}, seed {seed}: {e}\n{}", unit.print()));
                match kind {
                    Kind::Inline => assert!(unit.print().contains("inline_fn")),
                    Kind::AliasArray => assert!(unit.print().contains("new_var[2]")),
                    Kind::GlobalType => assert_ne!(at(&unit, &[2]), at(&base, &[2])),
                    _ => {}
                }
            }
            assert!(count > 0, "{kind:?} never found a site");
        }
    }

    #[test]
    fn region_identity_survives_repeated_expressions_and_tree_edits() {
        let base = parsed("int target(int a, int b) { int x; int y; x = a + b;\n#pragma _permuter randomizer start\ny = a + b;\n#pragma _permuter randomizer end\nreturn a + b + x + y; }");
        for seed in 0..40 {
            let mut unit = base.clone();
            assert!(apply(&mut unit, "target", Kind::Commutative, &mut Rng::new(seed)));
            let text = unit.print();
            assert!(text.contains("x = a + b;") && text.contains("y = b + a;") && text.contains("return ((a + b) + x) + y;"), "{text}");
        }
        let mut unit = base.clone();
        let mut rng = Rng::new(17);
        for _ in 0..40 { mutate(&mut unit, "target", &mut rng); }
        let text = unit.print();
        // Type/lifetime hypotheses can affect declarations; raw code outside
        // the region is protected from expression mutations.
        assert!(text.contains("_permuter randomizer start") && text.contains("_permuter randomizer end"));
    }

    #[test]
    fn signature_and_global_changes_update_every_declaration() {
        let source = "extern int global; extern int global; int called(int x); int called(int y); int target(int a) { return called(a) + global; }";
        for kind in [Kind::GlobalType, Kind::FunctionType] {
            for seed in 0..30 {
                let mut unit = parsed(source);
                if !apply(&mut unit, "target", kind, &mut Rng::new(seed)) { continue; }
                let Node::FileAst { ext } = &unit else { panic!() };
                let types = Types::new(&unit, "target");
                for (a, b) in [(0, 1), (2, 3)] {
                    let (Node::Decl { ty: a, .. }, Node::Decl { ty: b, .. }) = (&ext[a], &ext[b]) else { panic!() };
                    // Parameter spellings may differ; their emitted base
                    // types and return types must agree.
                    let canonical = |n: &Node| n.print().replace(" x", "").replace(" y", "");
                    assert!(types.similar(a, b) || canonical(a) == canonical(b));
                }
            }
        }
    }

    #[test]
    fn seeded_walks_replay_after_helpers_and_declarations_are_inserted() {
        let base = parsed(SOURCE);
        for seed in 0..8 {
            let mut unit = base.clone();
            let mut rng = Rng::new(seed);
            let mut steps = Vec::new();
            for _ in 0..24 { if let Some(step) = mutate(&mut unit, "target", &mut rng) { steps.push(step); } }
            let mut replay = base.clone();
            for step in steps {
                let super::super::mutate::Kind::Full(kind) = step.kind else { panic!() };
                assert!(apply(&mut replay, "target", kind, &mut step.rng.clone()));
            }
            assert_eq!(unit, replay);
        }
        let mut empty = parsed("void target(void) {}");
        // The retry bound also covers trees with no applicable expression.
        for _ in 0..5 { mutate(&mut empty, "target", &mut Rng::new(5)); }
    }
}
