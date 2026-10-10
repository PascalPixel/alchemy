//! Weighted search passes over complete C translation units.
//! These explore source hypotheses; compilation determines whether they match.
//! Collected paths identify mutation sites, including moved inline arguments.

use super::c::{Node, types::{self, Types}};
use super::rng::Rng;
use std::collections::{BTreeMap, BTreeSet};

mod lifetime;
mod remaining;

#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash, PartialOrd, Ord)]
pub enum Kind {
    Temporary, Expand, ReorderStatements, ReorderDeclarations, Cast,
    LocalType, GlobalType, FunctionType, SplitAssignment, Block, StructRef,
    FactorMultiply, FactorShift, Commutative, AddSub, Inequality,
    CompoundAssignment, Remove, ChainAssignment, LongChainAssignment,
    Inline, ConditionalBlock, AliasArray, RemoveVariable, ReverseCondition,
    AddMask, XorZero, ReferVariable, FloatLiteral, SameLine, EmptyStatement,
    Condition, MultiplyZero, DummyComma, SelfAssignment, DuplicateAssignment,
    PadDeclaration,
}

impl Kind {
    pub const GCC: [(Self, f64); 37] = [
        (Self::Temporary, 100.0), (Self::Expand, 20.0),
        (Self::ReorderStatements, 10.0), (Self::ReorderDeclarations, 10.0),
        (Self::AddMask, 0.5), (Self::XorZero, 0.5),
        (Self::Cast, 10.0), (Self::ReferVariable, 0.5), (Self::FloatLiteral, 0.5),
        (Self::LocalType, 10.0), (Self::GlobalType, 5.0),
        (Self::FunctionType, 5.0), (Self::SplitAssignment, 10.0),
        (Self::SameLine, 0.5), (Self::Block, 10.0), (Self::StructRef, 10.0),
        (Self::EmptyStatement, 0.5), (Self::Condition, 0.5), (Self::MultiplyZero, 0.5),
        (Self::FactorMultiply, 5.0), (Self::FactorShift, 5.0),
        (Self::DummyComma, 0.5), (Self::SelfAssignment, 0.5),
        (Self::Commutative, 5.0), (Self::AddSub, 5.0), (Self::Inequality, 5.0),
        (Self::CompoundAssignment, 5.0), (Self::Remove, 5.0),
        (Self::DuplicateAssignment, 0.5), (Self::ChainAssignment, 5.0),
        (Self::LongChainAssignment, 3.0), (Self::PadDeclaration, 0.5),
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
            Self::AddMask => "add integer masks",
            Self::XorZero => "add xor-zero or multiply-one",
            Self::ReferVariable => "insert a variable reference",
            Self::FloatLiteral => "change a float literal",
            Self::SameLine => "put statements on the same line",
            Self::EmptyStatement => "insert empty control flow",
            Self::Condition => "change a zero comparison",
            Self::MultiplyZero => "multiply an expression by zero",
            Self::DummyComma => "insert a comma expression",
            Self::SelfAssignment => "insert a self assignment",
            Self::DuplicateAssignment => "duplicate an assignment",
            Self::PadDeclaration => "insert a padding declaration",
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
struct Sites { nodes: Vec<Site>, regions: Vec<(usize, usize)>, region: Option<(usize, usize)>, choice: Option<usize>, body: Path }
impl Sites {
    fn new(unit: &Node, target: &str, rng: &mut Rng) -> Option<Self> {
        let mut sites = Self::build(unit, target)?;
        sites.choice = choose(rng, &sites.regions);
        sites.region = sites.choice.map(|i| sites.regions[i]);
        Some(sites)
    }
    fn build(unit: &Node, target: &str) -> Option<Self> {
        let body = vec![target_index(unit, target)?, 1];
        let mut nodes = Vec::new();
        fn walk(node: &Node, path: Path, nodes: &mut Vec<Site>, index: &mut usize) {
            let slot = nodes.len();
            nodes.push(Site { path: path.clone(), start: *index, end: 0 });
            *index += 2;
            for (i, n) in node.children().iter().enumerate() { walk(n, child(&path, i), nodes, index); }
            nodes[slot].end = *index;
            *index += 2;
        }
        let root = vec![body[0]];
        walk(at(unit, &root), root, &mut nodes, &mut 1);
        let (mut start, mut regions) = (None, Vec::new());
        for site in &nodes {
            if let Node::Pragma { string } = at(unit, &site.path) {
                if string == "_permuter randomizer start" && start.replace(site.end).is_some() { return None; }
                if string == "_permuter randomizer end" { regions.push((start.take()? + 1, site.start - 1)); }
            }
        }
        if start.is_some() { return None; }
        Some(Self { nodes, regions, region: None, choice: None, body })
    }
    fn allows(&self, path: &[usize]) -> bool {
        self.region.is_none_or(|(start, end)| self.nodes.iter().any(|s| s.path == path && s.start > start && s.end < end))
    }
    fn allows_pre(&self, path: &[usize]) -> bool {
        self.region.is_none_or(|(start, end)| self.nodes.iter().any(|s| s.path == path && s.start > start && s.start < end))
    }
    fn select(&self, unit: &Node, predicate: impl Fn(&Node) -> bool) -> Vec<Path> {
        self.nodes.iter().filter(|s| self.allows(&s.path) && predicate(at(unit, &s.path))).map(|s| s.path.clone()).collect()
    }
    fn blocks(&self, unit: &Node) -> Vec<Path> {
        fn block(unit: &Node, path: Path, out: &mut Vec<Path>) {
            out.push(path.clone());
            let node = at(unit, &path);
            for (i, stmt) in statements(node).unwrap().iter().enumerate() { nested(unit, stmt, statement_path(&path, node, i), out); }
        }
        fn nested(unit: &Node, node: &Node, path: Path, out: &mut Vec<Path>) {
            match node {
                Node::Compound { .. } | Node::Case { .. } | Node::Default { .. } => block(unit, path, out),
                Node::If { iffalse, .. } => { block(unit, child(&path, 1), out); if iffalse.is_some() { block(unit, child(&path, 2), out); } }
                Node::While { .. } | Node::DoWhile { .. } | Node::Switch { .. } => block(unit, child(&path, 1), out),
                Node::For { .. } => block(unit, child(&path, node.children().len() - 1), out),
                Node::Label { stmt, .. } => nested(unit, stmt, child(&path, 0), out),
                _ => {}
            }
        }
        let mut out = Vec::new();
        block(unit, self.body.clone(), &mut out);
        out
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
    out.extend(expression_nodes(node, path, value).into_iter().filter_map(|(path, expr)| expr.then_some(path)));
}

fn expression_nodes(node: &Node, path: &[usize], value: bool) -> Vec<(Path, bool)> {
    fn visit(node: &Node, path: &[usize], top: bool, lvalue: bool, out: &mut Vec<(Path, bool)>) {
        out.push((path.to_vec(), !top && !lvalue));
        let children = node.children();
        match node {
            Node::Assignment { .. } => { visit(children[0], &child(path, 0), false, true, out); visit(children[1], &child(path, 1), false, false, out); }
            Node::StructRef { op, .. } => visit(children[0], &child(path, 0), false, lvalue && op == ".", out),
            Node::Cast { .. } => visit(children[1], &child(path, 1), false, false, out),
            Node::UnaryOp { op, .. } if op != "sizeof" => visit(children[0], &child(path, 0), false, ["p++", "p--", "++", "--", "&"].contains(&op.as_str()), out),
            Node::BinaryOp { .. } => for (i, n) in children.iter().enumerate() { visit(n, &child(path, i), false, false, out); },
            Node::FuncCall { name, args } => {
                if !matches!(name.as_ref(), Node::Id { .. }) { visit(name, &child(path, 0), false, false, out); }
                if let Some(args) = args { visit(args, &child(path, 1), true, false, out); }
            }
            Node::ExprList { exprs } => for (i, n) in exprs.iter().enumerate() { if !matches!(n, Node::Typename { .. }) { visit(n, &child(path, i), false, false, out); } },
            Node::ArrayRef { .. } => { visit(children[0], &child(path, 0), false, lvalue, out); visit(children[1], &child(path, 1), false, false, out); }
            Node::TernaryOp { .. } => for (i, n) in children.iter().enumerate() { visit(n, &child(path, i), i != 0, false, out); },
            Node::Return { expr: Some(expr) } => visit(expr, &child(path, 0), false, false, out),
            Node::Decl { init: Some(init), .. } => visit(init, &child(path, 1), matches!(init.as_ref(), Node::InitList { .. }), false, out),
            Node::For { init, cond, next, stmt } => {
                let mut i = 0;
                for (n, top) in [(init.as_deref(), true), (cond.as_deref(), false), (next.as_deref(), true), (Some(stmt.as_ref()), true)] {
                    if let Some(n) = n { visit(n, &child(path, i), top, false, out); i += 1; }
                }
            }
            Node::Compound { block_items } | Node::Default { stmts: block_items } => for (i, n) in block_items.iter().enumerate() { visit(n, &child(path, i), true, false, out); },
            Node::Case { stmts, .. } => for (i, n) in stmts.iter().enumerate() { visit(n, &child(path, i + 1), true, false, out); },
            Node::DoWhile { .. } => { visit(children[1], &child(path, 1), true, false, out); visit(children[0], &child(path, 0), false, false, out); }
            Node::While { .. } | Node::If { .. } | Node::Switch { .. } => for (i, n) in children.iter().enumerate() { visit(n, &child(path, i), i != 0, false, out); },
            Node::Label { stmt, .. } => visit(stmt, &child(path, 0), true, false, out),
            _ => {}
        }
    }
    let mut out = Vec::new();
    visit(node, path, !value, false, &mut out);
    out
}

fn effectful(n: &Node) -> bool {
    match n {
        Node::FuncCall { .. } | Node::Assignment { .. } => true,
        Node::UnaryOp { op, expr } => {
            matches!(op.as_str(), "++" | "--" | "p++" | "p--")
                || expr.children().into_iter().any(effectful)
        }
        _ => n.children().into_iter().any(effectful),
    }
}
/// Unlike a C literal reader, it rejects long suffixes and old-style octal.
fn factor_constant(n: &Node) -> Option<i64> {
    let Node::Constant { value, .. } = n else { return None };
    let value = value.trim_matches(['u', 'U']);
    let (negative, value) = if let Some(v) = value.strip_prefix('-') { (true, v) } else { (false, value.strip_prefix('+').unwrap_or(value)) };
    let value = if let Some(v) = value.strip_prefix("0x").or_else(|| value.strip_prefix("0X")) { i64::from_str_radix(v, 16).ok()? }
    else if let Some(v) = value.strip_prefix("0o").or_else(|| value.strip_prefix("0O")) { i64::from_str_radix(v, 8).ok()? }
    else if let Some(v) = value.strip_prefix("0b").or_else(|| value.strip_prefix("0B")) { i64::from_str_radix(v, 2).ok()? }
    else { if value.starts_with('0') && !value.chars().all(|c| c == '0') { return None; } value.parse().ok()? };
    if negative { value.checked_neg() } else { Some(value) }
}
fn random_type(rng: &mut Rng) -> Node {
    let sign = if rng.random() < 0.5 { "unsigned " } else { "" };
    let names = ["char", "short", "int", "long", "long long"];
    let name = names[weighted(rng, [1.0, 1.0, 2.0, 0.5, 0.5].into_iter())];
    let mut ty = types::basic(&format!("{sign}{name}"));
    if rng.random() < 0.5 { if let Node::TypeDecl { quals, .. } = &mut ty { quals.push("volatile".into()); } }
    ty
}

/// Retry failed passes with the same weights. Block insertion and padding
/// declarations remain applicable even when the function has no expressions.
pub fn mutate(unit: &mut Node, target: &str, rng: &mut Rng) -> Option<super::mutate::Step> {
    let sites = Sites::new(unit, target, rng)?;
    loop {
        let kind = Kind::GCC[weighted(rng, Kind::GCC.iter().map(|(_, w)| *w))].0;
        let before = rng.clone();
        if apply_sites(unit, target, kind, rng, &sites) {
            return Some(super::mutate::Step { kind: super::mutate::Kind::Full(kind), rng: before, region: sites.choice });
        }
    }
}

#[cfg(test)]
pub fn apply(unit: &mut Node, target: &str, kind: Kind, rng: &mut Rng) -> bool {
    let Some(sites) = Sites::new(unit, target, rng) else { return false };
    apply_sites(unit, target, kind, rng, &sites)
}

pub fn replay(unit: &mut Node, target: &str, kind: Kind, rng: &mut Rng, region: Option<usize>) -> bool {
    let Some(mut sites) = Sites::build(unit, target) else { return false };
    if let Some(i) = region { let Some(bounds) = sites.regions.get(i) else { return false }; sites.region = Some(*bounds); }
    apply_sites(unit, target, kind, rng, &sites)
}

fn apply_sites(unit: &mut Node, target: &str, kind: Kind, rng: &mut Rng, sites: &Sites) -> bool {
    // Each pass either completes or leaves the input untouched, so failed
    // attempts and minimizer replays cannot leak half a transformation.
    let mut trial = unit.clone();
    let result = match kind {
        Kind::Temporary => lifetime::temporary(&mut trial, target, sites, rng),
        Kind::Expand => lifetime::expand(&mut trial, sites, rng),
        Kind::LocalType | Kind::GlobalType | Kind::FunctionType => change_type(&mut trial, target, sites, kind, rng),
        Kind::ReorderStatements | Kind::ReorderDeclarations => reorder(&mut trial, sites, kind, rng),
        Kind::SplitAssignment | Kind::ChainAssignment | Kind::LongChainAssignment => assignments(&mut trial, sites, kind, rng),
        Kind::Block => wrap(&mut trial, sites, rng),
        Kind::ConditionalBlock => remaining::conditional(&mut trial, target, sites, rng),
        Kind::Inline => remaining::inline(&mut trial, target, sites, rng),
        Kind::StructRef => remaining::struct_ref(&mut trial, sites, rng),
        Kind::AliasArray | Kind::RemoveVariable => remaining::variables(&mut trial, target, sites, kind, rng),
        Kind::AddMask | Kind::XorZero | Kind::ReferVariable | Kind::FloatLiteral | Kind::SameLine | Kind::EmptyStatement | Kind::Condition | Kind::MultiplyZero | Kind::DummyComma | Kind::SelfAssignment | Kind::DuplicateAssignment | Kind::PadDeclaration => auxiliary(&mut trial, target, sites, kind, rng),
        _ => expression(&mut trial, target, sites, kind, rng),
    };
    if result.is_some() { *unit = trial; true } else { false }
}

/// Statement boundaries in source order. Declarations are not valid following
/// statements for inserted executable code in the target's C89 dialect.
fn insertion_points(unit: &Node, sites: &Sites, bounded: bool) -> Vec<(Path, usize)> {
    insertion_points_with_decls(unit, sites, bounded, false)
}

fn insertion_points_with_decls(unit: &Node, sites: &Sites, bounded: bool, allow_decls: bool) -> Vec<(Path, usize)> {
    let mut points = Vec::new();
    for path in sites.blocks(unit) {
        let block = at(unit, &path);
        let items = statements(block).unwrap();
        for (i, item) in items.iter().enumerate() {
            let p = statement_path(&path, block, i);
            if (allow_decls || !matches!(item, Node::Decl { .. })) && (!bounded || sites.allows_pre(&p)) {
                points.push((path.clone(), i));
            }
        }
        let last = items.len().checked_sub(1).map(|i| statement_path(&path, block, i)).unwrap_or_else(|| path.clone());
        if !bounded || sites.allows(&last) { points.push((path, items.len())); }
    }
    points.sort_by_key(|(path, i)| statement_path(path, at(unit, path), *i));
    points
}

fn auxiliary(unit: &mut Node, target: &str, sites: &Sites, kind: Kind, rng: &mut Rng) -> Option<()> {
    let values = sites.expressions(unit);
    let types = Types::new(unit, target);
    match kind {
        Kind::AddMask | Kind::XorZero | Kind::DummyComma | Kind::MultiplyZero => {
            let path = &values[choose(rng, &values)?];
            let expr = at(unit, path).clone();
            let replacement = match kind {
                Kind::DummyComma => Node::ExprList { exprs: vec![integer(0), expr] },
                Kind::AddMask => {
                    if !types.integral(&types.decayed(&expr)?) { return None; }
                    let mask = ["0xFF", "0xFFFF", "0xFFFFFFFF", "0xFFFFFFFFFFFFFFFF"][rng.below(4)];
                    let suffix = ["", "u"][rng.below(2)];
                    let mask = Node::Constant { ty: "int".into(), value: format!("{mask}{suffix}") };
                    let mut result = binary("&", expr, mask.clone());
                    if rng.random() < 0.3 { for _ in 0..rng.below(12) { result = binary("&", result, mask.clone()); } }
                    result
                }
                Kind::XorZero => {
                    let ty = types.decayed(&expr)?;
                    if types.integral(&ty) { binary("^", expr, integer(0)) }
                    else {
                        let resolved = types.resolve(&ty)?;
                        let resolved = if let Node::TypeDecl { ty, .. } = resolved { ty.as_ref() } else { resolved };
                        let Node::IdentifierType { names } = resolved else { return None };
                        let (ty, value) = if names == &["float"] { ("float", "1.0f") } else if names == &["double"] { ("double", "1.0") } else { return None };
                        binary("*", expr, Node::Constant { ty: ty.into(), value: value.into() })
                    }
                }
                Kind::MultiplyZero => {
                    if effectful(&expr) || !types.arithmetic(&types.decayed(&expr)?) { return None; }
                    let zeros: Vec<_> = values.iter().filter(|p| matches!(at(unit, p), Node::Constant { value, .. } if ["0", "0.0", "0.0f"].contains(&value.as_str()))).collect();
                    let zero = zeros[choose(rng, &zeros)?];
                    let result = binary("*", expr, at(unit, zero).clone());
                    *at_mut(unit, zero) = result;
                    return Some(());
                }
                _ => unreachable!(),
            };
            *at_mut(unit, path) = replacement;
        }
        Kind::FloatLiteral => {
            let paths = sites.select(unit, |n| matches!(n, Node::Constant { ty, .. } if ty == "float"));
            let path = &paths[choose(rng, &paths)?];
            let Node::Constant { value, .. } = at(unit, path) else { return None };
            let value = value.to_lowercase();
            let mut choices = vec![value.trim_end_matches('f').to_string()];
            if let Some(v) = value.strip_suffix(".0f").or_else(|| value.strip_suffix(".f")) { choices.push(if v.is_empty() { "0".into() } else { v.into() }); }
            if value.starts_with("0.") { choices.push(value[1..].into()); }
            else if value.starts_with('.') { choices.push(format!("0{value}")); }
            choices.push(if let Some(v) = value.strip_suffix(".0f") { format!("{}.f", if v.is_empty() { "0" } else { v }) } else { format!("{}0f", value.trim_end_matches('f')) });
            let value = choices.swap_remove(rng.below(choices.len()));
            let ty = if value.ends_with('f') { "float" } else if value.contains('.') { "double" } else { "int" };
            *at_mut(unit, path) = Node::Constant { ty: ty.into(), value };
        }
        Kind::PadDeclaration => {
            let names: BTreeSet<_> = sites.nodes.iter().filter(|s| s.path.starts_with(&sites.body)).filter_map(|s| name(at(unit, &s.path))).collect();
            let var = if !names.contains("pad") { "pad".into() } else { (2..).map(|i| format!("pad{i}")).find(|n| !names.contains(n.as_str())).unwrap() };
            let decl = types::declaration(&var, random_type(rng));
            let items = statements_mut(at_mut(unit, &sites.body));
            let leading = items.iter().take_while(|n| matches!(n, Node::Decl { .. })).count();
            items.insert(rng.below(leading + 1), decl);
        }
        Kind::Condition => {
            let paths: Vec<_> = sites.nodes.iter().filter(|s| s.path.starts_with(&sites.body) && matches!(at(unit, &s.path), Node::If { .. } | Node::While { .. } | Node::DoWhile { .. } | Node::For { .. })).map(|s| s.path.clone()).collect();
            let node = at_mut(unit, &paths[choose(rng, &paths)?]);
            let cond = match node { Node::If { cond, .. } | Node::While { cond, .. } | Node::DoWhile { cond, .. } => cond, Node::For { cond, .. } => cond.as_mut()?, _ => unreachable!() };
            if let Node::BinaryOp { op, left, right } = cond.as_ref() {
                if ["==", "!=", "<", ">", "<=", ">="].contains(&op.as_str()) && rng.random() < 0.9 {
                    if !["==", "!="].contains(&op.as_str()) || !matches!(right.as_ref(), Node::Constant { value, .. } if ["0", "0U", "0.0", "0.0f"].contains(&value.as_str())) { return None; }
                    *cond = if op == "==" { unary("!", *left.clone()).boxed() } else { left.clone() };
                    return Some(());
                }
            }
            let mut expr = *cond.clone();
            let mut op = "!=";
            if let Node::UnaryOp { op: unary_op, expr: inner } = &expr { if unary_op == "!" && rng.random() < 0.9 { op = "=="; expr = *inner.clone(); } }
            let (ty, value) = [("int", "0"), ("unsigned int", "0U"), ("float", "0.0f")][weighted(rng, [0.8, 0.2, 0.05].into_iter())];
            *cond = binary(op, expr, Node::Constant { ty: ty.into(), value: value.into() }).boxed();
        }
        Kind::SameLine => {
            let points = insertion_points(unit, sites, true);
            if points.len() < 3 { return None; }
            let mut length = (points.len() - 2) as f64;
            for _ in 0..4 { length *= rng.random(); }
            let length = length as usize + 2;
            let i = rng.below(points.len() - length);
            for (index, marker) in [(i + length, "end"), (i, "start")] {
                let (path, offset) = &points[index];
                statements_mut(at_mut(unit, path)).insert(*offset, Node::Pragma { string: format!("_permuter sameline {marker}") });
            }
        }
        Kind::DuplicateAssignment | Kind::ReferVariable | Kind::SelfAssignment | Kind::EmptyStatement => {
            let mut inserts;
            let bounded = kind == Kind::SelfAssignment;
            let points = insertion_points(unit, sites, bounded);
            if points.is_empty() { return None; }
            let mut selected = None;
            if kind == Kind::EmptyStatement {
                let label = format!("dummy_label_{}", rng.below(1_000_000) + 1);
                inserts = match rng.below(6) {
                    0 => { let n = rng.below(5) + 2; let n = [1, n][rng.below(2)]; (0..n).map(|_| Node::If { cond: integer(1).boxed(), iftrue: compound(vec![]).boxed(), iffalse: None }).collect() }
                    1 => vec![Node::If { cond: integer(0).boxed(), iftrue: compound(vec![]).boxed(), iffalse: None }],
                    2 => vec![Node::DoWhile { cond: integer(0).boxed(), stmt: compound(vec![]).boxed() }],
                    3 => vec![Node::Label { name: label, stmt: Node::EmptyStatement {}.boxed() }],
                    4 => vec![Node::Goto { name: label.clone() }, Node::Label { name: label, stmt: Node::EmptyStatement {}.boxed() }],
                    _ => vec![Node::EmptyStatement {}],
                };
                inserts.insert(0, Node::Pragma { string: "_permuter sameline start".into() });
                inserts.push(Node::Pragma { string: "_permuter sameline end".into() });
            } else if kind == Kind::DuplicateAssignment {
                let paths = sites.select(unit, |n| matches!(n, Node::Assignment { op, .. } if op == "="));
                inserts = vec![at(unit, &paths[choose(rng, &paths)?]).clone()];
            } else {
                let mut seen = BTreeSet::new();
                let paths: Vec<_> = values.iter().filter(|p| matches!(at(unit, p), Node::Id { .. } | Node::StructRef { .. }) && (kind != Kind::SelfAssignment || seen.insert(at(unit, p).print()))).collect();
                let mut expr = at(unit, paths[choose(rng, &paths)?]).clone();
                if effectful(&expr) { return None; }
                if kind == Kind::ReferVariable {
                    let ty = types.decayed(&expr)?;
                    let resolved = types.resolve(&ty)?;
                    let resolved = if let Node::TypeDecl { ty, .. } = resolved { ty.as_ref() } else { resolved };
                    if matches!(resolved, Node::Struct { .. } | Node::Union { .. }) { expr = unary("&", expr); }
                    if rng.random() < 0.5 { expr = unary("!", expr); }
                    let mut cond = expr.clone();
                    for _ in 0..[0, 0, 0, 0, 0, 1, 2, 2][rng.below(8)] { cond = binary("&&", cond, expr.clone()); }
                    inserts = vec![Node::If { cond: cond.boxed(), iftrue: compound(vec![]).boxed(), iffalse: None }];
                } else {
                    selected = choose(rng, &points);
                    inserts = match rng.below(3) {
                        0 => vec![assign(expr.clone(), expr)],
                        1 => vec![Node::Assignment { op: "+=".into(), lvalue: expr.boxed(), rvalue: integer(0).boxed() }],
                        _ => vec![unary("p++", expr.clone()), unary("p--", expr)],
                    };
                }
            }
            let (path, offset) = &points[selected.or_else(|| choose(rng, &points))?];
            statements_mut(at_mut(unit, path)).splice(*offset..*offset, inserts);
        }
        _ => unreachable!(),
    }
    Some(())
}

fn expression(unit: &mut Node, target: &str, sites: &Sites, kind: Kind, rng: &mut Rng) -> Option<()> {
    if kind == Kind::Remove { return remove_ast(unit, sites, rng); }
    let value_only = matches!(kind, Kind::Cast | Kind::FactorMultiply | Kind::FactorShift);
    let paths = if value_only { sites.expressions(unit) } else { sites.nodes.iter().filter(|s| s.path.starts_with(&sites.body) && (kind == Kind::ReverseCondition || sites.allows(&s.path))).map(|s| s.path.clone()).collect() };
    let candidates: Vec<_> = paths.into_iter().filter(|path| { let n = at(unit, path); match kind {
        Kind::Cast => true,
        Kind::Commutative => matches!(n, Node::BinaryOp { op, .. } if ["+", "*", "&", "|", "^", "<", ">", "<=", ">=", "==", "!="].contains(&op.as_str())),
        Kind::AddSub => matches!(n, Node::BinaryOp { op, .. } if op == "+" || op == "-"),
        Kind::Inequality => matches!(n, Node::BinaryOp { op, .. } if ["<", ">", "<=", ">="].contains(&op.as_str())),
        Kind::FactorMultiply => matches!(n, Node::BinaryOp { op, left, right } if op == "*" && (matches!(left.as_ref(), Node::Constant { .. }) || matches!(right.as_ref(), Node::Constant { .. }))),
        Kind::FactorShift => matches!(n, Node::BinaryOp { op, right, .. } if (op == "<<" || op == ">>") && matches!(right.as_ref(), Node::Constant { .. })),
        Kind::CompoundAssignment => matches!(n, Node::Assignment { op, lvalue, rvalue } if op != "=" || matches!(rvalue.as_ref(), Node::BinaryOp { op, left, .. } if left == lvalue && ["+", "-", "*", "/", "<<", ">>", "&", "|", "^"].contains(&op.as_str()))),
        Kind::ReverseCondition => matches!(n, Node::If { iffalse: Some(_), .. } | Node::TernaryOp { .. }),
        _ => false,
    }}).collect();
    let path = &candidates[choose(rng, &candidates)?];
    let node = at(unit, path).clone();
    let replacement = match (kind, node) {
        (Kind::Cast, expr) => {
            let types = Types::new(unit, target);
            if !types.arithmetic(&types.decayed(&expr)?) { return None; }
            let ty = if rng.random() < 0.5 {
                let sign = ["", "unsigned "][rng.below(2)];
                let name = ["int", "char", "long", "short", "long long"][rng.below(5)];
                types::basic(&format!("{sign}{name}"))
            } else { types::basic(["float", "double"][rng.below(2)]) };
            Node::Cast { to_type: types::typename(ty).boxed(), expr: expr.boxed() }
        }
        (Kind::Commutative, Node::BinaryOp { op, left, right }) => {
            let op = match op.as_str() { "<" => ">", ">" => "<", "<=" => ">=", ">=" => "<=", _ => &op };
            binary(op, *right, *left)
        }
        (Kind::AddSub, Node::BinaryOp { op, left, right }) => {
            let types = Types::new(unit, target);
            let right = match *right {
                Node::Constant { ty, value } => Node::Constant { ty, value: if let Some(v) = value.strip_prefix('-') { v.into() } else { format!("-{value}") } },
                Node::UnaryOp { op, expr } if op == "-" => *expr,
                right => {
                    let ty = types.decayed(&binary(&op, *left.clone(), right.clone()))?;
                    if matches!(types.resolve(&ty)?, Node::TypeDecl { .. }) { return None; }
                    unary("-", right)
                }
            };
            binary(if op == "+" { "-" } else { "+" }, *left, right)
        }
        (Kind::Inequality, Node::BinaryOp { op, left, right }) => {
            if rng.random() < 0.25 {
                let change = ["+", "-"][rng.below(2)];
                binary(&op, binary(change, *left, integer(1)), binary(change, *right, integer(1)))
            } else {
                let lower = op == "<" || op == ">=";
                let op = match op.as_str() { "<" => "<=", ">=" => ">", ">" => ">=", "<=" => "<", _ => unreachable!() };
                if rng.random() < 0.5 { binary(op, binary(if lower { "+" } else { "-" }, *left, integer(1)), *right) }
                else { binary(op, *left, binary(if lower { "-" } else { "+" }, *right, integer(1))) }
            }
        }
        (Kind::FactorShift, Node::BinaryOp { op, left, right }) => {
            let n = factor_constant(&right)?;
            if n < 2 { return None; }
            let first = 1 + rng.below(n as usize - 1) as i64;
            binary(&op, binary(&op, *left, integer(first)), integer(n - first))
        }
        (Kind::FactorMultiply, Node::BinaryOp { left, right, .. }) => {
            let (n, expr) = if matches!(left.as_ref(), Node::Constant { .. }) { (factor_constant(&left)?, *right) } else { (factor_constant(&right)?, *left) };
            let limit = n.checked_abs()?.min(21);
            let mut factors = BTreeSet::new();
            for i in 2..limit { if n % i == 0 { factors.insert(i); factors.insert(n / i); } }
            let mut factors: Vec<_> = factors.into_iter().collect();
            if n < 0 { factors.extend(factors.clone().into_iter().map(|f| -f)); }
            let a = factors[choose(rng, &factors)?];
            let mut inner = binary("*", expr, integer(a));
            if rng.below(2) != 0 { if let Node::BinaryOp { left, right, .. } = &mut inner { std::mem::swap(left, right); } }
            let mut outer = binary("*", inner, integer(n / a));
            if rng.below(2) != 0 { if let Node::BinaryOp { left, right, .. } = &mut outer { std::mem::swap(left, right); } }
            outer
        }
        (Kind::CompoundAssignment, Node::Assignment { op, lvalue, rvalue }) => {
            if op == "=" {
                let Node::BinaryOp { op, right, .. } = *rvalue else { return None };
                Node::Assignment { op: format!("{op}="), lvalue, rvalue: right }
            } else { assign(*lvalue.clone(), binary(op.trim_end_matches('='), *lvalue, *rvalue)) }
        }
        (Kind::ReverseCondition, Node::If { cond, iftrue, iffalse: Some(iffalse) }) => Node::If { cond: negate(*cond).boxed(), iftrue: iffalse, iffalse: Some(iftrue) },
        (Kind::ReverseCondition, Node::TernaryOp { cond, iftrue, iffalse }) => Node::TernaryOp { cond: negate(*cond).boxed(), iftrue: iffalse, iffalse: iftrue },
        _ => return None,
    };
    *at_mut(unit, path) = replacement;
    Some(())
}
fn negate(n: Node) -> Node { if let Node::UnaryOp { op, expr } = &n { if op == "!" { return *expr.clone(); } } unary("!", n) }

/// into type dimensions, sizeof operands or initializer-list elements.
fn replacement_reachable(unit: &Node, body: &[usize], path: &[usize]) -> bool {
    for depth in body.len()..path.len() {
        let node = at(unit, &path[..depth]);
        let i = path[depth];
        let visits = match node {
            Node::StructRef { .. } => i == 0,
            Node::Cast { .. } => i == 1,
            Node::UnaryOp { op, .. } => op != "sizeof",
            Node::FuncCall { name, .. } => i != 0 || !matches!(name.as_ref(), Node::Id { .. }),
            Node::ExprList { exprs } => !matches!(exprs[i], Node::Typename { .. }),
            Node::Decl { .. } => i == 1,
            Node::Case { .. } => i != 0,
            Node::Assignment { .. } | Node::BinaryOp { .. } | Node::ArrayRef { .. } | Node::TernaryOp { .. } | Node::Return { .. } | Node::For { .. } | Node::Compound { .. } | Node::Default { .. } | Node::DoWhile { .. } | Node::While { .. } | Node::If { .. } | Node::Switch { .. } | Node::Label { .. } => true,
            _ => false,
        };
        if !visits { return false; }
    }
    true
}

fn remove_ast(unit: &mut Node, sites: &Sites, rng: &mut Rng) -> Option<()> {
    let mut candidates = Vec::new();
    for site in &sites.nodes {
        if !site.path.starts_with(&sites.body) || !sites.allows(&site.path) { continue; }
        match at(unit, &site.path) {
            Node::Cast { expr, .. } => candidates.push((site.path.clone(), *expr.clone())),
            Node::BinaryOp { op, left, right } if op == "&" => {
                if matches!(left.as_ref(), Node::Constant { .. }) { candidates.push((site.path.clone(), *right.clone())); }
                if matches!(right.as_ref(), Node::Constant { .. }) { candidates.push((site.path.clone(), *left.clone())); }
            }
            Node::If { iftrue, iffalse: None, .. } => candidates.push((site.path.clone(), *iftrue.clone())),
            Node::While { stmt, .. } | Node::DoWhile { stmt, .. } => candidates.push((site.path.clone(), *stmt.clone())),
            _ => {}
        }
    }
    let (path, replacement) = candidates.get(choose(rng, &candidates)?)?;
    if replacement_reachable(unit, &sites.body, path) { *at_mut(unit, path) = replacement.clone(); }
    Some(())
}

fn mentioned_in_region(unit: &Node, sites: &Sites) -> Vec<String> {
    let mut seen = BTreeSet::new();
    sites.nodes.iter().filter_map(|s| {
        if !sites.allows(&s.path) { return None; }
        let Node::Id { name } = at(unit, &s.path) else { return None };
        if s.path.last() == Some(&1) && matches!(at(unit, &s.path[..s.path.len() - 1]), Node::StructRef { .. }) { return None; }
        seen.insert(name.clone()).then(|| name.clone())
    }).collect()
}

fn randomize_innermost(ty: &mut Node, types: &Types, rng: &mut Rng) -> Option<()> {
    match ty {
        Node::TypeDecl { .. } if types.integral(ty) => { *ty = random_type(rng); Some(()) }
        Node::PtrDecl { ty, .. } | Node::ArrayDecl { ty, .. } | Node::FuncDecl { ty, .. } => randomize_innermost(ty, types, rng),
        _ => None,
    }
}

fn change_type(unit: &mut Node, target: &str, sites: &Sites, kind: Kind, rng: &mut Rng) -> Option<()> {
    let types = Types::new(unit, target);
    let names = mentioned_in_region(unit, sites);
    if kind == Kind::LocalType {
        let candidates = sites.nodes.iter().filter(|s| matches!(at(unit, &s.path), Node::Decl { name: Some(name), ty, .. } if names.contains(name) && matches!(ty.as_ref(), Node::TypeDecl { .. }))).map(|s| s.path.clone()).collect::<Vec<_>>();
        let path = &candidates[choose(rng, &candidates)?];
        let Node::Decl { name, ty, .. } = at_mut(unit, path) else { return None };
        if !types.integral(ty) { return None; }
        **ty = random_type(rng); types::set_name(ty, name.as_deref());
        return Some(());
    }
    let mut called = vec![target.to_string()];
    for site in &sites.nodes {
        if sites.allows(&site.path) { if let Node::FuncCall { name, .. } = at(unit, &site.path) { if let Node::Id { name } = name.as_ref() { called.push(name.clone()); } } }
    }
    let Node::FileAst { ext } = unit else { return None };
    if kind == Kind::GlobalType {
        let selected = &names[choose(rng, &names)?];
        let candidates: Vec<_> = ext.iter().enumerate().filter_map(|(i, n)| matches!(n, Node::Decl { name: Some(name), .. } if name == selected).then_some(i)).collect();
        let Node::Decl { ty, .. } = &ext[candidates[choose(rng, &candidates)?]] else { return None };
        let mut new = *ty.clone(); randomize_innermost(&mut new, &types, rng)?; types::set_name(&mut new, Some(selected));
        for n in ext { if let Node::Decl { name: Some(name), ty, .. } = n { if name == selected { **ty = new.clone(); } } }
        return Some(());
    }
    let mut seen = BTreeSet::new(); called.retain(|n| seen.insert(n.clone()));
    let selected = &called[choose(rng, &called)?];
    let decl_type = |n: &Node| match n { Node::Decl { ty, .. } => Some(*ty.clone()), Node::FuncDef { decl, .. } => if let Node::Decl { ty, .. } = decl.as_ref() { Some(*ty.clone()) } else { None }, _ => None };
    let candidates: Vec<_> = ext.iter().enumerate().filter_map(|(i, n)| (name(n) == Some(selected) && matches!(decl_type(n), Some(Node::FuncDecl { .. }))).then_some(i)).collect();
    let main = candidates.iter().rev().copied().find(|&i| matches!(ext[i], Node::FuncDef { .. })).or_else(|| choose(rng, &candidates).map(|i| candidates[i]))?;
    let mut new = decl_type(&ext[main])?;
    let Node::FuncDecl { args, ty: ret } = &mut new else { return None };
    if rng.random() < 0.5 {
        let old = types.decay(ret)?;
        let void = matches!(types.resolve(&old)?, Node::TypeDecl { ty, .. } if matches!(ty.as_ref(), Node::IdentifierType { names } if names.contains(&"void".into())));
        **ret = if void { random_type(rng) } else if rng.random() < 0.2 { types::basic("void") } else { if !types.integral(&old) { return None; } random_type(rng) };
        types::set_name(&mut new, Some(selected));
    } else {
        let Node::ParamList { params } = args.as_deref_mut()? else { return None };
        let i = choose(rng, params)?;
        match &mut params[i] {
            Node::Decl { name, ty, .. } => { let old = types.decay(ty)?; if !types.integral(&old) { return None; } **ty = random_type(rng); types::set_name(ty, name.as_deref()); }
            Node::Typename { ty, .. } => { let old = types.decay(ty)?; if !types.integral(&old) { return None; } **ty = random_type(rng); }
            _ => return None,
        }
    }
    for item in ext {
        if name(item) != Some(selected) { continue; }
        let decl = if let Node::FuncDef { decl, .. } = item { decl.as_mut() } else { item };
        let Node::Decl { ty, .. } = decl else { continue };
        if matches!(ty.as_ref(), Node::FuncDecl { .. }) { **ty = new.clone(); }
    }
    Some(())
}

fn reorder(unit: &mut Node, sites: &Sites, kind: Kind, rng: &mut Rng) -> Option<()> {
    let decl = kind == Kind::ReorderDeclarations;
    let candidates: Vec<_> = insertion_points_with_decls(unit, sites, true, true).into_iter().filter(|(path, i)| {
        let stmts = statements(at(unit, path)).unwrap();
        let before = i.checked_sub(1).and_then(|j| stmts.get(j));
        let after = stmts.get(*i);
        if decl { *i == 0 || matches!(before, Some(Node::Decl { .. })) || matches!(after, Some(Node::Decl { .. })) }
        else { !matches!(after, Some(Node::Decl { .. })) || matches!(after, Some(Node::Decl { name: Some(_), init: Some(init), .. }) if !matches!(init.as_ref(), Node::InitList { .. })) }
    }).collect();
    let sources: Vec<_> = candidates.iter().enumerate().filter_map(|(slot, (path, i))| {
        let stmt = statements(at(unit, path)).unwrap().get(*i)?;
        let movable = if decl { matches!(stmt, Node::Decl { .. }) }
            else { !matches!(stmt, Node::Pragma { .. } | Node::Compound { .. } | Node::For { .. } | Node::While { .. } | Node::DoWhile { .. } | Node::If { .. } | Node::Switch { .. } | Node::Case { .. } | Node::Default { .. }) };
        movable.then_some(slot)
    }).collect();
    let source = sources[choose(rng, &sources)?];
    let (from_block, from_index) = &candidates[source];
    let source_path = statement_path(from_block, at(unit, from_block), *from_index);
    let stmt = at(unit, &source_path).clone();
    let mut after = source + 1;
    if !decl && (matches!(stmt, Node::Decl { .. }) || candidates.get(after).is_none_or(|(p, i)| p != from_block || *i != from_index + 1)) { after -= 1; }
    let mut destinations = Vec::new();
    for (i, (path, index)) in candidates.iter().enumerate() {
        let distance = (source as isize - i as isize).max(i as isize - after as isize);
        if distance == 0 || (!decl && matches!(statements(at(unit, path)).unwrap().get(*index), Some(Node::Decl { .. }))) { continue; }
        let probability = (distance as f64 + 1.0).powf(-1.5) + if decl { 1.0 } else { 0.5 } / candidates.len() as f64;
        destinations.push((i, probability));
    }
    if destinations.is_empty() { return None; }
    let selected = destinations[weighted(rng, destinations.iter().map(|(_, w)| *w))].0;
    let (mut to_block, mut to_index) = candidates[selected].clone();
    if to_block == *from_block && (to_index == *from_index || to_index == from_index + 1) { return None; }
    if decl {
        let Node::Decl { name: Some(var), .. } = &stmt else { return None };
        let to_stmt = statements(at(unit, &to_block))?.get(to_index);
        let to_path = if to_stmt.is_some() { statement_path(&to_block, at(unit, &to_block), to_index) } else { from_block.clone() };
        let site = sites.nodes.iter().find(|s| s.path == to_path)?;
        let limit = if to_stmt.is_some() { site.start } else { site.end };
        let uses = sites.nodes.iter().filter(|s| s.path.starts_with(&sites.body) && s.start < limit && match at(unit, &s.path) { Node::Id { name } => name == var, Node::TypeDecl { declname: Some(name), .. } => name == var, _ => false }).count();
        if uses > 1 { return None; }
    } else if let Node::Decl { name: Some(name), init: Some(init), .. } = stmt {
        // Extract the initializer without removing its declaration. Changing
        // the initializer before insertion keeps an enclosing path stable.
        let Node::Decl { init: old, .. } = at_mut(unit, &source_path) else { return None };
        *old = None;
        statements_mut(at_mut(unit, &to_block)).insert(to_index, assign(Node::id(name), *init));
        return Some(());
    }
    if to_block.starts_with(&source_path) { return None; }
    if to_block == *from_block && *from_index < to_index { to_index -= 1; }
    if to_block.starts_with(from_block) && to_block.len() > from_block.len() && to_block[from_block.len()] > source_path[from_block.len()] { to_block[from_block.len()] -= 1; }
    let stmt = statements_mut(at_mut(unit, from_block)).remove(*from_index);
    statements_mut(at_mut(unit, &to_block)).insert(to_index, stmt);
    Some(())
}

fn assignments(unit: &mut Node, sites: &Sites, kind: Kind, rng: &mut Rng) -> Option<()> {
    if kind == Kind::SplitAssignment {
        let candidates: Vec<_> = sites.select(unit, |n| matches!(n, Node::Assignment { op, rvalue, .. } if op == "=" && matches!(rvalue.as_ref(), Node::BinaryOp { .. }))).into_iter().filter(|p| p.starts_with(&sites.body)).collect();
        let path = &candidates[choose(rng, &candidates)?];
        let (block, index) = insertion_points(unit, sites, true).into_iter().find(|(block, i)| statement_path(block, at(unit, block), *i) == *path)?;
        fn binops(unit: &Node, path: Path, out: &mut Vec<Path>) {
            let Node::BinaryOp { left, right, .. } = at(unit, &path) else { return };
            if matches!(left.as_ref(), Node::BinaryOp { .. }) { binops(unit, child(&path, 0), out); }
            out.push(path.clone());
            if matches!(right.as_ref(), Node::BinaryOp { .. }) { binops(unit, child(&path, 1), out); }
        }
        let mut splits = Vec::new(); binops(unit, child(path, 1), &mut splits);
        let split = &splits[choose(rng, &splits)?];
        let types = Types::new(unit, name(at(unit, &[sites.body[0]]))?);
        let Node::Assignment { lvalue, .. } = at(unit, path) else { return None };
        let var = *lvalue.clone();
        let side_path = child(split, usize::from(rng.random() >= 0.5));
        let side = at(unit, &side_path).clone();
        if !types.similar(&types.decayed(&var)?, &types.decayed(&side)?) { return None; }
        *at_mut(unit, &side_path) = var.clone();
        statements_mut(at_mut(unit, &block)).insert(index, assign(var, side));
        return Some(());
    }
    let mut candidates = Vec::new();
    for path in sites.blocks(unit) {
        let block = at(unit, &path);
        let stmts = statements(block)?;
        let mut i = 0;
        while i < stmts.len() {
            let stmt = &stmts[i];
            let mut next_i = i + 1;
            if kind == Kind::LongChainAssignment {
                if let Node::Assignment { rvalue, .. } = stmt {
                    if sites.allows(&statement_path(&path, block, i)) && !effectful(rvalue) {
                        while next_i < stmts.len() && sites.allows(&statement_path(&path, block, next_i)) && matches!(&stmts[next_i], Node::Assignment { rvalue: rhs, .. } if rhs == rvalue) { next_i += 1; }
                        if next_i > i + 2 { candidates.push((path.clone(), i, next_i)); }
                    }
                }
                i = next_i;
                continue;
            }
            let first_i = i;
            i += 1;
            let i = first_i;
            if !sites.allows(&statement_path(&path, block, i)) { continue; }
            let Node::Assignment { lvalue, rvalue, .. } = stmt else { continue };
            for (j, next) in stmts.iter().enumerate().skip(i + 1).take(4) {
                if sites.allows(&statement_path(&path, block, j)) && matches!(next, Node::Assignment { rvalue: rhs, .. } if (rhs == rvalue || rhs == lvalue) && !effectful(rhs)) { candidates.push((path.clone(), i, j)); }
            }
        }
    }
    let (path, mut i, mut j) = candidates[choose(rng, &candidates)?].clone();
    if kind == Kind::LongChainAssignment {
        let a = i + rng.below(j - i + 1);
        let b = i + rng.below(j - i + 1);
        if a + 3 <= b { i = a; j = b; }
    }
    let stmts = statements_mut(at_mut(unit, &path));
    let Node::Assignment { lvalue, rvalue, .. } = stmts[i].clone() else { return None };
    if kind == Kind::ChainAssignment {
        let Node::Assignment { lvalue: next, .. } = stmts[j].clone() else { return None };
        stmts[i] = if rng.random() < 0.5 { assign(*next, stmts[i].clone()) } else { assign(*lvalue, assign(*next, *rvalue)) };
        stmts.remove(j);
    } else {
        let mut new = stmts[i].clone();
        for stmt in stmts.iter().take(j).skip(i + 1) {
            let Node::Assignment { lvalue, .. } = stmt.clone() else { return None };
            let op = if let Node::Assignment { op, .. } = &new { op.clone() } else { return None };
            new = Node::Assignment { op, lvalue, rvalue: new.boxed() };
        }
        stmts[i] = new;
        stmts.drain(i + 1..j);
    }
    Some(())
}

fn wrap(unit: &mut Node, sites: &Sites, rng: &mut Rng) -> Option<()> {
        let blocks = sites.blocks(unit);
        let path = &blocks[choose(rng, &blocks)?];
        let block = at(unit, path);
        let items = statements(block)?;
        let leading = items.iter().take_while(|n| matches!(n, Node::Decl { .. } | Node::Pragma { .. })).count();
        let mut lo = leading + rng.below(items.len() + 1 - leading);
        let mut hi = leading + rng.below(items.len() + 1 - leading);
        if hi < lo { std::mem::swap(&mut lo, &mut hi); }
        let same_line = rng.random() < 0.5 && (lo..hi).all(|i| sites.allows(&statement_path(path, block, i)));
        let items = statements_mut(at_mut(unit, path));
        let body = compound(items.drain(lo..hi).collect()).boxed();
        let replacement = if same_line {
            vec![Node::Pragma { string: "_permuter sameline start".into() }, Node::DoWhile { cond: integer(0).boxed(), stmt: body }, Node::Pragma { string: "_permuter sameline end".into() }]
        } else { vec![Node::If { cond: integer(1).boxed(), iftrue: body, iffalse: None }] };
        items.splice(lo..lo, replacement);
        Some(())
}

#[cfg(test)]
mod tests {
    use super::*;

    const SOURCE: &str = "typedef unsigned short U; struct S { int member; }; extern int global; extern int called(int);\nint target(int a, int b, struct S *p) { int x; int y; int z; int spare; float f = 1.0f; x = 0; x = a + b; y = a + b; z = a + b; spare = 3; x = x + 2; if (a < b) { y = x * 12; } else { y = x << 4; } z = (int)(y & 255); x = called(z); return p->member + x + global; }\n";

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
                count += usize::from(unit != base);
                let mut replay = base.clone();
                assert!(apply(&mut replay, "target", kind, &mut Rng::new(seed)));
                assert_eq!(unit, replay, "{kind:?}, seed {seed}");
                super::super::c::parse(&unit.print()).unwrap_or_else(|e| panic!("{kind:?}, seed {seed}: {e}\n{}", unit.print()));
                match kind {
                    Kind::Inline => assert!(unit.print().contains("inline_fn")),
                    Kind::AliasArray => assert!(unit.print().contains("new_var[2]")),
                    Kind::GlobalType => assert_eq!(at(&unit, &[target_index(&unit, "target").unwrap()]), at(&base, &[target_index(&base, "target").unwrap()])),
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
                assert!(super::replay(&mut replay, "target", kind, &mut step.rng.clone(), step.region));
            }
            assert_eq!(unit, replay);
        }
        let mut empty = parsed("void target(void) {}");
        // Block insertion and declarations also cover empty functions.
        for _ in 0..5 { mutate(&mut empty, "target", &mut Rng::new(5)); }
    }

    #[test]
    fn saved_regions_replay_without_consuming_a_second_region_draw() {
        let base = parsed("int target(int a, int b) { int x;\n#pragma _permuter randomizer start\nx = a + b;\n#pragma _permuter randomizer end\n#pragma _permuter randomizer start\nx = a * b;\n#pragma _permuter randomizer end\nreturn x; }");
        for seed in 0..20 {
            let mut unit = base.clone();
            let mut rng = Rng::new(seed);
            let mut steps = Vec::new();
            for _ in 0..12 { if let Some(step) = mutate(&mut unit, "target", &mut rng) { steps.push(step); } }
            let mut restored = base.clone();
            for step in steps {
                let super::super::mutate::Kind::Full(kind) = step.kind else { panic!() };
                assert!(replay(&mut restored, "target", kind, &mut step.rng.clone(), step.region));
            }
            assert_eq!(unit, restored, "seed {seed}");
        }
    }

    #[test]
    fn float_neutral_expression_and_unevaluated_operands_are_supported() {
        let base = parsed("float target(float f) { return f + sizeof(f); }");
        let sites = Sites::build(&base, "target").unwrap();
        let paths = sites.expressions(&base);
        assert_eq!(paths.iter().filter(|p| matches!(at(&base, p), Node::Id { name } if name == "f")).count(), 1);
        let mut found = false;
        for seed in 0..20 {
            let mut unit = base.clone();
            if apply(&mut unit, "target", Kind::XorZero, &mut Rng::new(seed)) && unit.print().contains("1.0f") { found = true; }
        }
        assert!(found);
    }

    #[test]
    fn auxiliary_passes_match_seeded_text_and_rng() {
        let base = parsed("int target(int a, int b) { float f = 1.0f; int x; x = a + b; return x + a; }");
        for row in include_str!("c/fixtures/mutations.tsv").lines() {
            let columns: Vec<_> = row.split('\t').collect();
            let kind = match columns[0] {
                "perm_add_mask" => Kind::AddMask,
                "perm_xor_zero" => Kind::XorZero,
                "perm_dummy_comma_expr" => Kind::DummyComma,
                "perm_float_literal" => Kind::FloatLiteral,
                "perm_pad_var_decl" => Kind::PadDeclaration,
                _ => panic!("unknown fixture pass"),
            };
            let mut rng = Rng::new(columns[1].parse().unwrap());
            let mut unit = base.clone();
            let changed = apply(&mut unit, "target", kind, &mut rng);
            assert_eq!(changed, columns[2] == "1", "{kind:?}, seed {}", columns[1]);
            assert_eq!(unit.print(), columns[4].replace("\\n", "\n"), "{kind:?}, seed {}", columns[1]);
            assert_eq!(rng.random(), columns[3].parse::<f64>().unwrap(), "{kind:?}, seed {}", columns[1]);
        }
    }

    fn fixture_source(case: &str) -> &'static str {
        match case {
                "flow" => include_str!("c/fixtures/mutations-flow.c"),
                "nested" => include_str!("c/fixtures/mutations-nested.c"),
                "regions" => include_str!("c/fixtures/mutations-regions.c"),
                "arithmetic" => include_str!("c/fixtures/mutations-arithmetic.c"),
                "types" => include_str!("c/fixtures/mutations-types.c"),
                "assignments" => include_str!("c/fixtures/mutations-assignments.c"),
                "shadow" => include_str!("c/fixtures/mutations-shadow.c"),
                "lifetime" => include_str!("c/fixtures/mutations-lifetime.c"),
                "structs" => include_str!("c/fixtures/mutations-structs.c"),
                "manyvars" => include_str!("c/fixtures/mutations-manyvars.c"),
                "inline" => include_str!("c/fixtures/mutations-inline.c"),
                _ => panic!("unknown fixture input"),
        }
    }

    fn fixture_kind(method: &str) -> Kind {
        match method {
                "perm_add_mask" => Kind::AddMask,
                "perm_xor_zero" => Kind::XorZero,
                "perm_dummy_comma_expr" => Kind::DummyComma,
                "perm_float_literal" => Kind::FloatLiteral,
                "perm_pad_var_decl" => Kind::PadDeclaration,
                "perm_refer_to_var" => Kind::ReferVariable,
                "perm_empty_stmt" => Kind::EmptyStatement,
                "perm_sameline" => Kind::SameLine,
                "perm_condition" => Kind::Condition,
                "perm_mult_zero" => Kind::MultiplyZero,
                "perm_add_self_assignment" => Kind::SelfAssignment,
                "perm_duplicate_assignment" => Kind::DuplicateAssignment,
                "perm_cast_simple" => Kind::Cast,
                "perm_commutative" => Kind::Commutative,
                "perm_add_sub" => Kind::AddSub,
                "perm_inequalities" => Kind::Inequality,
                "perm_factor_mult" => Kind::FactorMultiply,
                "perm_factor_shift" => Kind::FactorShift,
                "perm_compound_assignment" => Kind::CompoundAssignment,
                "perm_reverse_cond" => Kind::ReverseCondition,
                "perm_ins_block" => Kind::Block,
                "perm_randomize_internal_type" => Kind::LocalType,
                "perm_randomize_external_type" => Kind::GlobalType,
                "perm_randomize_function_type" => Kind::FunctionType,
                "perm_reorder_decls" => Kind::ReorderDeclarations,
                "perm_reorder_stmts" => Kind::ReorderStatements,
                "perm_split_assignment" => Kind::SplitAssignment,
                "perm_chain_assignment" => Kind::ChainAssignment,
                "perm_long_chain_assignment" => Kind::LongChainAssignment,
                "perm_remove_ast" => Kind::Remove,
                "perm_temp_for_expr" => Kind::Temporary,
                "perm_expand_expr" => Kind::Expand,
                "perm_inline" => Kind::Inline,
                "perm_var_cond_block" => Kind::ConditionalBlock,
                "perm_struct_ref" => Kind::StructRef,
                "perm_alias_array" => Kind::AliasArray,
                "perm_remove_var" => Kind::RemoveVariable,
                _ => panic!("unknown fixture pass"),
        }
    }

    #[test]
    fn passes_match_in_nested_blocks_and_regions() {
        let mut failures = Vec::new();
        for row in include_str!("c/fixtures/mutations-auxiliary.tsv").lines().chain(include_str!("c/fixtures/mutations-expression.tsv").lines()).chain(include_str!("c/fixtures/mutations-types-assignments.tsv").lines()).chain(include_str!("c/fixtures/mutations-lifetime.tsv").lines()).chain(include_str!("c/fixtures/mutations-final.tsv").lines()) {
            let c: Vec<_> = row.split('\t').collect();
            let kind = fixture_kind(c[1]);
            let mut unit = parsed(fixture_source(c[0]));
            let mut rng = Rng::new(c[2].parse().unwrap());
            let changed = apply(&mut unit, "target", kind, &mut rng);
            let text = unit.print();
            let expected = c[5].replace("\\n", "\n");
            let next = rng.random();
            if changed != (c[3] == "1") || text != expected || next != c[4].parse::<f64>().unwrap() {
                failures.push(format!("{} {kind:?} seed {}: changed {changed} expected {}; rng {next} expected {}\nactual:\n{text}\nexpected:\n{expected}", c[0], c[2], c[3], c[4]));
            }
        }
        assert!(failures.is_empty(), "{} mismatches\n{}", failures.len(), failures.into_iter().take(8).collect::<Vec<_>>().join("\n"));
    }

    #[test]
    fn successive_mutations_match_source_hashes_retries_and_rng() {
        check_walks(include_str!("c/fixtures/mutations-integer-seeds.tsv"), &[include_str!("c/fixtures/mutations-auxiliary.tsv"), include_str!("c/fixtures/mutations-expression.tsv"), include_str!("c/fixtures/mutations-types-assignments.tsv"), include_str!("c/fixtures/mutations-lifetime.tsv"), include_str!("c/fixtures/mutations-final.tsv")], true);
        check_walks(include_str!("c/fixtures/mutations-final-walks.tsv"), &[include_str!("c/fixtures/mutations-auxiliary.tsv"), include_str!("c/fixtures/mutations-expression.tsv"), include_str!("c/fixtures/mutations-types-assignments.tsv"), include_str!("c/fixtures/mutations-lifetime.tsv"), include_str!("c/fixtures/mutations-final.tsv")], true);
        check_walks(include_str!("c/fixtures/mutations-walks.tsv"), &[include_str!("c/fixtures/mutations-auxiliary.tsv"), include_str!("c/fixtures/mutations-expression.tsv")], false);
        check_walks(include_str!("c/fixtures/mutations-types-walks.tsv"), &[include_str!("c/fixtures/mutations-auxiliary.tsv"), include_str!("c/fixtures/mutations-expression.tsv"), include_str!("c/fixtures/mutations-types-assignments.tsv")], true);
        check_walks(include_str!("c/fixtures/mutations-lifetime-walks.tsv"), &[include_str!("c/fixtures/mutations-auxiliary.tsv"), include_str!("c/fixtures/mutations-expression.tsv"), include_str!("c/fixtures/mutations-types-assignments.tsv"), include_str!("c/fixtures/mutations-lifetime.tsv")], true);
    }

    fn check_walks(walks: &str, fixtures: &[&str], full_text: bool) {
        use sha2::{Digest, Sha256};
        let methods: BTreeSet<_> = fixtures.iter().flat_map(|f| f.lines()).map(|row| fixture_kind(row.split('\t').nth(1).unwrap())).collect();
        let weights: Vec<_> = Kind::GCC.into_iter().filter(|(kind, _)| methods.contains(kind)).collect();
        let mut unit = parsed(fixture_source("flow"));
        let mut rng = Rng::new(0);
        for row in walks.lines() {
            let c: Vec<_> = row.split('\t').collect();
            if c[2] == "0" { unit = parsed(fixture_source(c[0])); rng = Rng::from_decimal(c[1]).unwrap(); }
            let before = (methods.len() == Kind::GCC.len()).then(|| (unit.clone(), rng.clone()));
            let sites = Sites::new(&unit, "target", &mut rng).unwrap();
            let mut attempts = 0;
            let kind = loop {
                attempts += 1;
                assert!(attempts < 1000, "no successful mutation");
                let kind = weights[weighted(&mut rng, weights.iter().map(|(_, w)| *w))].0;
                let success = apply_sites(&mut unit, "target", kind, &mut rng, &sites);
                if success { break kind; }
            };
            let context = format!("{} seed {} step {}", c[0], c[1], c[2]);
            assert_eq!(kind, fixture_kind(c[4]), "{context}: selected pass");
            assert_eq!(attempts, c[3].parse::<usize>().unwrap(), "{context}: retry count");
            if full_text { assert_eq!(unit.print(), c[5].replace("\\n", "\n"), "{context}: source"); }
            else { assert_eq!(format!("{:x}", Sha256::digest(unit.print().as_bytes())), c[5], "{context}: source\n{}", unit.print()); }
            assert_eq!(rng.clone().random(), c[6].parse::<f64>().unwrap(), "{context}: random state");
            if let Some((mut driven, mut driver_rng)) = before {
                let mut restored = driven.clone();
                let step = mutate(&mut driven, "target", &mut driver_rng).unwrap();
                assert_eq!(driven, unit, "{context}: production mutation driver");
                assert_eq!(driver_rng.random(), c[6].parse::<f64>().unwrap(), "{context}: driver random state");
                let super::super::mutate::Kind::Full(kind) = step.kind else { panic!() };
                assert!(replay(&mut restored, "target", kind, &mut step.rng.clone(), step.region));
                assert_eq!(restored, unit, "{context}: replay");
            }
        }
    }

    #[test]
    fn unchanged_success_does_not_retry_another_pass() {
        let mut unit = parsed("int target(int a) { return a + a; }");
        let base = unit.clone();
        assert!(apply(&mut unit, "target", Kind::Commutative, &mut Rng::new(0)));
        assert_eq!(unit, base);
    }

    #[test]
    fn side_effect_detection_matches_unary_traversal() {
        for row in include_str!("c/fixtures/mutations-effects.tsv").lines() {
            let (expr, expected) = row.split_once('\t').unwrap();
            let unit = parsed(&format!("int target(int a,int b) {{ return {expr}; }}"));
            let body = [target_index(&unit, "target").unwrap(), 1];
            let Node::Return { expr: Some(value) } = &statements(at(&unit, &body)).unwrap()[0] else { panic!() };
            assert_eq!(effectful(value), expected == "1", "{expr}");
        }
    }
}
