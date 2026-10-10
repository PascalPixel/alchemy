//! C type queries and declaration lookup for candidate generation.

use super::Node;
use std::collections::{BTreeMap, BTreeSet};

pub fn basic(name: &str) -> Node {
    Node::TypeDecl { declname: None, quals: vec![], align: vec![], ty: Node::IdentifierType { names: name.split_whitespace().map(str::to_string).collect() }.boxed() }
}

pub fn pointer(ty: Node) -> Node { Node::PtrDecl { quals: vec![], ty: ty.boxed() } }

pub fn set_name(ty: &mut Node, name: Option<&str>) {
    match ty {
        Node::TypeDecl { declname, .. } => *declname = name.map(str::to_string),
        Node::PtrDecl { ty, .. } | Node::ArrayDecl { ty, .. } | Node::FuncDecl { ty, .. } => set_name(ty, name),
        _ => {}
    }
}

pub fn declaration(name: &str, mut ty: Node) -> Node {
    set_name(&mut ty, Some(name));
    Node::Decl { name: Some(name.into()), quals: vec![], align: vec![], storage: vec![], funcspec: vec![], attributes: vec![], ty: ty.boxed(), init: None, bitsize: None, asmlabel: None }
}

pub fn typename(mut ty: Node) -> Node {
    set_name(&mut ty, None);
    Node::Typename { name: None, quals: vec![], align: vec![], attributes: vec![], ty: ty.boxed() }
}

#[derive(Default)]
pub struct Types {
    aliases: BTreeMap<String, Node>,
    pub vars: BTreeMap<String, Node>,
    pub locals: BTreeSet<String>,
    records: BTreeMap<(bool, String), Node>,
}

impl Types {
    pub fn new(unit: &Node, target: &str) -> Self {
        let mut result = Self::default();
        result.collect(unit, target, false);
        result
    }

    fn collect(&mut self, node: &Node, target: &str, local: bool) {
        match node {
            Node::Typedef { name, ty, .. } => { self.aliases.insert(name.clone(), *ty.clone()); self.collect(ty, target, local); }
            Node::Struct { name: Some(name), decls: Some(_), .. } | Node::Union { name: Some(name), decls: Some(_), .. } => {
                self.records.insert((matches!(node, Node::Union { .. }), name.clone()), node.clone());
            }
            Node::Struct { .. } | Node::Union { .. } => {}
            Node::FuncDef { decl, body, .. } => {
                self.collect(decl, target, false);
                if let Node::Decl { name: Some(name), ty, .. } = decl.as_ref() {
                    if name == target {
                        if let Node::FuncDecl { args: Some(args), .. } = ty.as_ref() { self.collect(args, target, true); }
                        self.collect(body, target, true);
                    }
                }
            }
            Node::Decl { name, ty, .. } => {
                if let Some(name) = name {
                    self.vars.insert(name.clone(), *ty.clone());
                    if local { self.locals.insert(name.clone()); }
                }
                self.collect(ty, target, local);
            }
            Node::FuncDecl { ty, .. } => self.collect(ty, target, local),
            Node::Enumerator { name, .. } => { self.vars.insert(name.clone(), basic("int")); }
            _ => for child in node.children() { self.collect(child, target, local); }
        }
    }

    pub fn resolve<'a>(&'a self, mut ty: &'a Node) -> Option<&'a Node> {
        // Malformed/cyclic aliases must not hang a worker.
        for _ in 0..64 {
            if let Node::TypeDecl { ty: inner, .. } = ty {
                if let Node::IdentifierType { names } = inner.as_ref() {
                    if names.len() == 1 {
                        if let Some(next) = self.aliases.get(&names[0]) { ty = next; continue; }
                    }
                }
            }
            return Some(ty);
        }
        None
    }

    pub fn decay(&self, ty: &Node) -> Option<Node> {
        Some(match self.resolve(ty)? {
            Node::ArrayDecl { ty, .. } => pointer(*ty.clone()),
            Node::FuncDecl { .. } => pointer(ty.clone()),
            Node::TypeDecl { ty, .. } if matches!(ty.as_ref(), Node::Enum { .. }) => basic("int"),
            _ => ty.clone(),
        })
    }

    pub fn arithmetic(&self, ty: &Node) -> bool {
        self.allowed_basic(ty, &["int", "short", "long", "char", "signed", "unsigned", "float", "double"])
    }

    pub fn integral(&self, ty: &Node) -> bool {
        self.allowed_basic(ty, &["int", "short", "long", "char", "signed", "unsigned"])
    }

    pub fn allowed_basic(&self, ty: &Node, allowed: &[&str]) -> bool {
        matches!(self.resolve(ty), Some(Node::TypeDecl { ty, .. }) if matches!(ty.as_ref(), Node::IdentifierType { names } if names.iter().any(|n| allowed.contains(&n.as_str()))))
    }

    fn float(&self, ty: &Node) -> bool {
        matches!(self.resolve(ty), Some(Node::TypeDecl { ty, .. }) if matches!(ty.as_ref(), Node::IdentifierType { names } if names.iter().any(|n| n == "float" || n == "double")))
    }

    pub fn similar(&self, a: &Node, b: &Node) -> bool {
        self.same(a, b, true)
    }

    pub fn same(&self, a: &Node, b: &Node, similar: bool) -> bool {
        match (self.resolve(a), self.resolve(b)) {
            (Some(Node::PtrDecl { ty: a, .. }), Some(Node::PtrDecl { ty: b, .. })) | (Some(Node::ArrayDecl { ty: a, .. }), Some(Node::ArrayDecl { ty: b, .. })) => self.same(a, b, similar),
            (Some(Node::TypeDecl { ty: a, .. }), Some(Node::TypeDecl { ty: b, .. })) => match (a.as_ref(), b.as_ref()) {
                (Node::Struct { name: a, .. }, Node::Struct { name: b, .. }) | (Node::Union { name: a, .. }, Node::Union { name: b, .. }) => a == b,
                (Node::IdentifierType { .. } | Node::Enum { .. }, Node::IdentifierType { .. } | Node::Enum { .. }) if similar => true,
                (Node::Enum { name: a, .. }, Node::Enum { name: b, .. }) => a == b,
                (Node::IdentifierType { names: a }, Node::IdentifierType { names: b }) => { let (mut a, mut b) = (a.clone(), b.clone()); a.sort(); b.sort(); a == b }
                _ => false,
            },
            (Some(Node::FuncDecl { args: a, ty: ar }), Some(Node::FuncDecl { args: b, ty: br })) => {
                let params = |n: &Option<Box<Node>>| if let Some(Node::ParamList { params }) = n.as_deref() { params.clone() } else { vec![] };
                let (a, b) = (params(a), params(b));
                a.len() == b.len() && a.iter().zip(&b).all(|(a, b)| match (a, b) {
                    (Node::EllipsisParam {}, Node::EllipsisParam {}) => true,
                    (Node::Id { name: a }, Node::Id { name: b }) => a == b,
                    (Node::Decl { ty: a, .. } | Node::Typename { ty: a, .. }, Node::Decl { ty: b, .. } | Node::Typename { ty: b, .. }) => self.same(a, b, false),
                    _ => false,
                }) && self.same(ar, br, false)
            }
            _ => false
        }
    }

    fn deref(&self, ty: &Node) -> Option<Node> {
        match self.resolve(ty)? { Node::PtrDecl { ty, .. } | Node::ArrayDecl { ty, .. } => Some(*ty.clone()), _ => None }
    }

    fn field(&self, node: &Node, field: &str, depth: usize) -> Option<Node> {
        if depth > 32 { return None; }
        let (name, decls) = match node { Node::Struct { name, decls, .. } | Node::Union { name, decls, .. } => (name, decls), _ => return None };
        let Some(decls) = decls else { return self.field(self.records.get(&(matches!(node, Node::Union { .. }), name.as_ref()?.clone()))?, field, depth + 1); };
        for decl in decls {
            if let Node::Decl { name, ty, .. } = decl {
                if name.as_deref() == Some(field) { return Some(*ty.clone()); }
                if name.is_none() {
                    let ty = if let Node::TypeDecl { ty, .. } = ty.as_ref() { ty.as_ref() } else { ty.as_ref() };
                    if let Some(found) = self.field(ty, field, depth + 1) { return Some(found); }
                }
            }
        }
        None
    }

    pub fn expr(&self, expr: &Node) -> Option<Node> {
        let ty = match expr {
            Node::Id { name } => self.vars.get(name)?.clone(),
            Node::Constant { ty, .. } => match ty.as_str() { "string" => pointer(basic("char")), "char" => basic("int"), _ => basic(ty) },
            Node::Cast { to_type, .. } | Node::CompoundLiteral { ty: to_type, .. } => match to_type.as_ref() { Node::Typename { ty, .. } => *ty.clone(), _ => return None },
            Node::Assignment { lvalue, .. } => self.expr(lvalue)?,
            Node::UnaryOp { op, expr } => match op.as_str() {
                "&" => pointer(self.expr(expr)?),
                "*" => self.deref(&self.expr(expr)?)?,
                "p++" | "p--" | "++" | "--" => self.expr(expr)?,
                "-" | "+" if self.float(&self.expr(expr)?) => if self.allowed_basic(&self.expr(expr)?, &["double"]) { basic("double") } else { basic("float") },
                "sizeof" | "-" | "+" | "~" | "!" => basic("int"),
                _ => return None,
            },
            Node::ArrayRef { name, .. } => self.deref(&self.expr(name)?)?,
            Node::StructRef { name, op, field } => {
                let Node::Id { name: field } = field.as_ref() else { return None };
                let mut ty = self.expr(name)?;
                if op == "->" { ty = self.deref(&ty)?; }
                let Node::TypeDecl { ty, .. } = self.resolve(&ty)? else { return None };
                self.field(ty, field, 0)?
            }
            Node::BinaryOp { op, left, right } => {
                let a = self.decayed(left)?;
                let b = self.decayed(right)?;
                if matches!(op.as_str(), "<<" | ">>") { a }
                else if matches!(op.as_str(), "+" | "-") && matches!(self.resolve(&a)?, Node::PtrDecl { .. }) {
                    if matches!(self.resolve(&b)?, Node::PtrDecl { .. }) { if op == "+" { return None; } basic("int") } else { a }
                } else if op == "+" && matches!(self.resolve(&b)?, Node::PtrDecl { .. }) { b }
                else if matches!(op.as_str(), "+" | "-" | "*" | "/") && (self.float(&a) || self.float(&b)) {
                    if a.print().contains("double") || b.print().contains("double") { basic("double") } else { basic("float") }
                } else { basic("int") }
            }
            Node::FuncCall { name, .. } => {
                let ty = self.expr(name)?;
                let mut ty = self.resolve(&ty)?;
                if let Node::PtrDecl { ty: inner, .. } = ty { ty = self.resolve(inner)?; }
                let Node::FuncDecl { ty, .. } = ty else { return None };
                *ty.clone()
            }
            Node::TernaryOp { iftrue, .. } => self.expr(iftrue)?,
            Node::ExprList { exprs } => self.expr(exprs.last()?)?,
            _ => return None,
        };
        Some(ty)
    }

    pub fn decayed(&self, expr: &Node) -> Option<Node> { self.decay(&self.expr(expr)?) }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn expressions_resolve_aliases_members_function_pointers_and_arrays() {
        let unit = super::super::parse("typedef unsigned short U; struct S { U x; }; typedef struct S S; S *p; U a[4]; U (*call)(int); int target(int n) { return n; }").unwrap();
        let types = Types::new(&unit, "target");
        let expr = Node::StructRef { name: Node::id("p").boxed(), op: "->".into(), field: Node::id("x").boxed() };
        assert_eq!(types.resolve(&types.expr(&expr).unwrap()).unwrap().print(), "unsigned short");
        assert!(matches!(types.decayed(&Node::id("a")), Some(Node::PtrDecl { .. })));
        let call = Node::FuncCall { name: Node::id("call").boxed(), args: None };
        assert!(types.integral(&types.expr(&call).unwrap()));
        assert!(types.expr(&Node::id("unknown")).is_none());
        let unit = super::super::parse("int target(int n) { { short n; n = 1; } return n; }").unwrap();
        assert_eq!(Types::new(&unit, "target").expr(&Node::id("n")).unwrap().print(), "short");
    }

    #[test]
    fn type_comparison_ignores_qualifiers_but_keeps_signatures_strict() {
        let unit = super::super::parse("int first(int x); int second(volatile int y); unsigned int third(int x); int fourth(short x); int target(void) { return 0; }").unwrap();
        let types = Types::new(&unit, "target");
        assert!(types.similar(&basic("float"), &basic("int")));
        assert!(!types.same(&basic("float"), &basic("int"), false));
        assert!(types.similar(&basic("void"), &basic("int")));
        assert!(types.same(&types.vars["first"], &types.vars["second"], true));
        assert!(!types.same(&types.vars["first"], &types.vars["third"], true));
        assert!(!types.same(&types.vars["first"], &types.vars["fourth"], true));
        let array = |n| Node::ArrayDecl { ty: basic("int").boxed(), dim: Some(Node::Constant { ty: "int".into(), value: n }.boxed()), dim_quals: vec![] };
        assert!(types.same(&array("2".into()), &array("7".into()), false));
    }
}
