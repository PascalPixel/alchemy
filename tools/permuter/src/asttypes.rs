//! Pragmatic C type inference over the `lang-c` AST.
//!
//! A port of pret decomp-permuter's `ast_types.py` (MIT), reduced to what
//! the randomization passes consult. Like the original it makes simplifying
//! assumptions: qualifiers do not matter for comparisons, arithmetic
//! promotes int-like types to `int`, and variable names are assumed unique
//! across scopes. Failures are reported as `Fail` so a pass can abort and
//! another can be tried, mirroring `RandomizationFailure`.

use std::collections::{BTreeMap, BTreeSet};

use lang_c::ast::*;
use lang_c::span::Node;

/// Aborts the current randomization pass; the engine picks another.
#[derive(Debug, Clone, Copy)]
pub struct Fail;

pub fn ensure(cond: bool) -> Result<(), Fail> {
    if cond {
        Ok(())
    } else {
        Err(Fail)
    }
}

/// A resolved-enough C type. `Basic` covers builtin keyword types and
/// typedef names (a single name found in the typedef table resolves).
#[derive(Debug, Clone, PartialEq)]
pub enum CType {
    Ptr(Box<CType>),
    Array(Box<CType>),
    /// A function type carrying only its return type; parameter lists are
    /// not compared (the passes ported here never need them).
    Func(Box<CType>),
    Basic {
        names: Vec<String>,
        volatile: bool,
    },
    Struct {
        is_union: bool,
        name: String,
    },
    Enum(String),
}

impl CType {
    pub fn basic(name: &str) -> CType {
        CType::Basic {
            names: vec![name.to_string()],
            volatile: false,
        }
    }
    pub fn int() -> CType {
        CType::basic("int")
    }
    pub fn ptr(inner: CType) -> CType {
        CType::Ptr(Box::new(inner))
    }
}

#[derive(Debug, Default, Clone)]
pub struct TypeMap {
    pub typedefs: BTreeMap<String, CType>,
    pub var_types: BTreeMap<String, CType>,
    pub local_vars: BTreeSet<String>,
    /// struct/union tag -> ordered fields (None name = anonymous member).
    pub fields: BTreeMap<String, Vec<(Option<String>, CType)>>,
}

pub fn is_local_var(name: &str, tm: &TypeMap) -> bool {
    tm.local_vars.contains(name)
}

pub fn resolve_typedefs(mut t: CType, tm: &TypeMap) -> CType {
    let mut guard = 0;
    loop {
        match &t {
            CType::Basic { names, .. } if names.len() == 1 => {
                if let Some(next) = tm.typedefs.get(&names[0]) {
                    t = next.clone();
                    guard += 1;
                    if guard > 64 {
                        return t;
                    }
                    continue;
                }
            }
            _ => {}
        }
        return t;
    }
}

pub fn pointer_decay(t: &CType, tm: &TypeMap) -> CType {
    let real = resolve_typedefs(t.clone(), tm);
    match real {
        CType::Array(inner) => CType::Ptr(inner),
        CType::Func(_) => CType::ptr(real),
        CType::Enum(_) => CType::int(),
        _ => t.clone(),
    }
}

pub fn deref_type(t: &CType, tm: &TypeMap) -> Result<CType, Fail> {
    match resolve_typedefs(t.clone(), tm) {
        CType::Ptr(inner) | CType::Array(inner) => Ok(*inner),
        _ => Err(Fail),
    }
}

fn struct_member_type(tag: &str, field: &str, tm: &TypeMap) -> Result<CType, Fail> {
    let fields = tm.fields.get(tag).ok_or(Fail)?;
    for (name, t) in fields {
        match name {
            Some(n) if n == field => return Ok(t.clone()),
            None => {
                // Anonymous nested struct/union member: search inside.
                if let CType::Struct { name, .. } = t {
                    if let Ok(found) = struct_member_type(name, field, tm) {
                        return Ok(found);
                    }
                }
            }
            _ => {}
        }
    }
    Err(Fail)
}

/// Whether a resolved type is a basic type carrying one of the allowed
/// keywords (port of `allowed_basic_type`).
pub fn allowed_basic_type(t: &CType, tm: &TypeMap, allowed: &[&str]) -> bool {
    match resolve_typedefs(t.clone(), tm) {
        CType::Basic { names, .. } => names.iter().any(|n| allowed.contains(&n.as_str())),
        _ => false,
    }
}

/// Port of `same_type`. With `allow_similar`, all int-ish (identifier or
/// enum) type pairs compare equal, exactly as pret's version does.
pub fn same_type(a: &CType, b: &CType, tm: &TypeMap, allow_similar: bool) -> bool {
    let mut a = resolve_typedefs(a.clone(), tm);
    let mut b = resolve_typedefs(b.clone(), tm);
    let mut allow_similar = allow_similar;
    loop {
        match (a, b) {
            (CType::Array(x), CType::Array(y)) | (CType::Ptr(x), CType::Ptr(y)) => {
                a = resolve_typedefs(*x, tm);
                b = resolve_typedefs(*y, tm);
            }
            (CType::Func(x), CType::Func(y)) => {
                a = resolve_typedefs(*x, tm);
                b = resolve_typedefs(*y, tm);
                allow_similar = false;
            }
            (
                CType::Struct {
                    is_union: u1,
                    name: n1,
                },
                CType::Struct {
                    is_union: u2,
                    name: n2,
                },
            ) => return u1 == u2 && n1 == n2,
            (CType::Enum(n1), CType::Enum(n2)) => {
                return allow_similar || n1 == n2;
            }
            (CType::Enum(_), CType::Basic { .. })
            | (CType::Basic { .. }, CType::Enum(_)) => return allow_similar,
            (CType::Basic { names: n1, .. }, CType::Basic { names: n2, .. }) => {
                if allow_similar {
                    return true;
                }
                let mut s1 = n1.clone();
                let mut s2 = n2.clone();
                s1.sort();
                s2.sort();
                return s1 == s2;
            }
            _ => return false,
        }
    }
}

// ----------------------------------------------------------- type building

fn wrap_derived(mut t: CType, derived: &[Node<DerivedDeclarator>]) -> CType {
    for d in derived {
        t = match &d.node {
            DerivedDeclarator::Pointer(_) | DerivedDeclarator::Block(_) => {
                CType::Ptr(Box::new(t))
            }
            DerivedDeclarator::Array(_) => CType::Array(Box::new(t)),
            DerivedDeclarator::Function(_) | DerivedDeclarator::KRFunction(_) => {
                CType::Func(Box::new(t))
            }
        };
    }
    t
}

/// Applies a declarator to a base type; returns the type and the declared
/// name if any.
pub fn apply_declarator(base: CType, d: &Declarator) -> (CType, Option<String>) {
    let t = wrap_derived(base, &d.derived);
    match &d.kind.node {
        DeclaratorKind::Abstract => (t, None),
        DeclaratorKind::Identifier(id) => (t, Some(id.node.name.clone())),
        DeclaratorKind::Declarator(inner) => apply_declarator(t, &inner.node),
    }
}

fn base_from_type_specifier(ts: &TypeSpecifier, names: &mut Vec<String>) -> Option<CType> {
    let name = match ts {
        TypeSpecifier::Void => "void",
        TypeSpecifier::Char => "char",
        TypeSpecifier::Short => "short",
        TypeSpecifier::Int => "int",
        TypeSpecifier::Long => "long",
        TypeSpecifier::Float => "float",
        TypeSpecifier::Double => "double",
        TypeSpecifier::Signed => "signed",
        TypeSpecifier::Unsigned => "unsigned",
        TypeSpecifier::Bool => "_Bool",
        TypeSpecifier::TypedefName(id) => {
            names.push(id.node.name.clone());
            return None;
        }
        TypeSpecifier::Struct(st) => {
            return Some(CType::Struct {
                is_union: matches!(st.node.kind.node, StructKind::Union),
                name: st
                    .node
                    .identifier
                    .as_ref()
                    .map(|i| i.node.name.clone())
                    .unwrap_or_default(),
            });
        }
        TypeSpecifier::Enum(et) => {
            return Some(CType::Enum(
                et.node
                    .identifier
                    .as_ref()
                    .map(|i| i.node.name.clone())
                    .unwrap_or_default(),
            ));
        }
        _ => return None,
    };
    names.push(name.to_string());
    None
}

pub fn ctype_from_decl_specifiers(specs: &[Node<DeclarationSpecifier>]) -> Result<CType, Fail> {
    let mut names = Vec::new();
    let mut volatile = false;
    let mut tagged: Option<CType> = None;
    for s in specs {
        match &s.node {
            DeclarationSpecifier::TypeSpecifier(ts) => {
                if let Some(t) = base_from_type_specifier(&ts.node, &mut names) {
                    tagged = Some(t);
                }
            }
            DeclarationSpecifier::TypeQualifier(q) => {
                if matches!(q.node, TypeQualifier::Volatile) {
                    volatile = true;
                }
            }
            _ => {}
        }
    }
    if let Some(t) = tagged {
        return Ok(t);
    }
    ensure(!names.is_empty())?;
    Ok(CType::Basic { names, volatile })
}

pub fn ctype_from_specifier_qualifiers(
    specs: &[Node<SpecifierQualifier>],
) -> Result<CType, Fail> {
    let mut names = Vec::new();
    let mut volatile = false;
    let mut tagged: Option<CType> = None;
    for s in specs {
        match &s.node {
            SpecifierQualifier::TypeSpecifier(ts) => {
                if let Some(t) = base_from_type_specifier(&ts.node, &mut names) {
                    tagged = Some(t);
                }
            }
            SpecifierQualifier::TypeQualifier(q) => {
                if matches!(q.node, TypeQualifier::Volatile) {
                    volatile = true;
                }
            }
            SpecifierQualifier::Extension(_) => {}
        }
    }
    if let Some(t) = tagged {
        return Ok(t);
    }
    ensure(!names.is_empty())?;
    Ok(CType::Basic { names, volatile })
}

pub fn ctype_from_type_name(tn: &TypeName) -> Result<CType, Fail> {
    let base = ctype_from_specifier_qualifiers(&tn.specifiers)?;
    match &tn.declarator {
        Some(d) => Ok(apply_declarator(base, &d.node).0),
        None => Ok(base),
    }
}

// ---------------------------------------------------------- typemap build

fn register_type_specifier(ts: &TypeSpecifier, tm: &mut TypeMap) {
    match ts {
        TypeSpecifier::Struct(st) => {
            if let Some(decls) = &st.node.declarations {
                let mut fields = Vec::new();
                for d in decls {
                    if let StructDeclaration::Field(f) = &d.node {
                        // Recurse into nested definitions first.
                        for sq in &f.node.specifiers {
                            if let SpecifierQualifier::TypeSpecifier(inner) = &sq.node {
                                register_type_specifier(&inner.node, tm);
                            }
                        }
                        let base = match ctype_from_specifier_qualifiers(&f.node.specifiers) {
                            Ok(b) => b,
                            Err(_) => continue,
                        };
                        if f.node.declarators.is_empty() {
                            fields.push((None, base.clone()));
                        }
                        for sd in &f.node.declarators {
                            match &sd.node.declarator {
                                Some(decl) => {
                                    let (t, name) = apply_declarator(base.clone(), &decl.node);
                                    fields.push((name, t));
                                }
                                None => fields.push((None, base.clone())),
                            }
                        }
                    }
                }
                if let Some(id) = &st.node.identifier {
                    tm.fields.insert(id.node.name.clone(), fields);
                }
            }
        }
        TypeSpecifier::Enum(et) => {
            for e in &et.node.enumerators {
                tm.var_types
                    .insert(e.node.identifier.node.name.clone(), CType::int());
            }
        }
        _ => {}
    }
}

fn register_decl_specifiers(specs: &[Node<DeclarationSpecifier>], tm: &mut TypeMap) {
    for s in specs {
        if let DeclarationSpecifier::TypeSpecifier(ts) = &s.node {
            register_type_specifier(&ts.node, tm);
        }
    }
}

fn is_typedef(specs: &[Node<DeclarationSpecifier>]) -> bool {
    specs.iter().any(|s| {
        matches!(
            &s.node,
            DeclarationSpecifier::StorageClass(sc)
                if matches!(sc.node, StorageClassSpecifier::Typedef)
        )
    })
}

fn record_declaration(d: &Declaration, tm: &mut TypeMap, local: bool) {
    register_decl_specifiers(&d.specifiers, tm);
    let base = match ctype_from_decl_specifiers(&d.specifiers) {
        Ok(b) => b,
        Err(_) => return,
    };
    let typedef = is_typedef(&d.specifiers);
    for init in &d.declarators {
        let (t, name) = apply_declarator(base.clone(), &init.node.declarator.node);
        if let Some(name) = name {
            if typedef {
                tm.typedefs.insert(name, t);
            } else {
                tm.var_types.insert(name.clone(), t);
                if local {
                    tm.local_vars.insert(name);
                }
            }
        }
    }
}

fn record_body_decls(stmt: &Node<Statement>, tm: &mut TypeMap) {
    match &stmt.node {
        Statement::Compound(items) => {
            for item in items {
                match &item.node {
                    BlockItem::Declaration(d) => record_declaration(&d.node, tm, true),
                    BlockItem::Statement(s) => record_body_decls(s, tm),
                    BlockItem::StaticAssert(_) => {}
                }
            }
        }
        Statement::If(i) => {
            record_body_decls(&i.node.then_statement, tm);
            if let Some(e) = &i.node.else_statement {
                record_body_decls(e, tm);
            }
        }
        Statement::Switch(s) => record_body_decls(&s.node.statement, tm),
        Statement::While(w) => record_body_decls(&w.node.statement, tm),
        Statement::DoWhile(d) => record_body_decls(&d.node.statement, tm),
        Statement::For(f) => {
            if let ForInitializer::Declaration(d) = &f.node.initializer.node {
                record_declaration(&d.node, tm, true);
            }
            record_body_decls(&f.node.statement, tm);
        }
        Statement::Labeled(l) => record_body_decls(&l.node.statement, tm),
        _ => {}
    }
}

/// Port of `build_typemap`: typedefs and globals from the whole unit,
/// locals from the target function only.
pub fn build_typemap(unit: &TranslationUnit, target_fn: &str) -> TypeMap {
    let mut tm = TypeMap::default();
    for ext in &unit.0 {
        match &ext.node {
            ExternalDeclaration::Declaration(d) => record_declaration(&d.node, &mut tm, false),
            ExternalDeclaration::FunctionDefinition(f) => {
                register_decl_specifiers(&f.node.specifiers, &mut tm);
                let base = match ctype_from_decl_specifiers(&f.node.specifiers) {
                    Ok(b) => b,
                    Err(_) => continue,
                };
                let (t, name) = apply_declarator(base, &f.node.declarator.node);
                let name = match name {
                    Some(n) => n,
                    None => continue,
                };
                tm.var_types.insert(name.clone(), t);
                if name == target_fn {
                    for der in &f.node.declarator.node.derived {
                        if let DerivedDeclarator::Function(fd) = &der.node {
                            for p in &fd.node.parameters {
                                let pbase =
                                    match ctype_from_decl_specifiers(&p.node.specifiers) {
                                        Ok(b) => b,
                                        Err(_) => continue,
                                    };
                                if let Some(pd) = &p.node.declarator {
                                    let (pt, pname) = apply_declarator(pbase, &pd.node);
                                    if let Some(pname) = pname {
                                        tm.var_types.insert(pname.clone(), pt);
                                        tm.local_vars.insert(pname);
                                    }
                                }
                            }
                        }
                    }
                    record_body_decls(&f.node.statement, &mut tm);
                }
            }
            ExternalDeclaration::StaticAssert(_) => {}
        }
    }
    tm
}

// ----------------------------------------------------------- expr typing

/// Port of `expr_type`. Errors abort the pass instead of crashing.
pub fn expr_type(e: &Expression, tm: &TypeMap) -> Result<CType, Fail> {
    match e {
        Expression::Identifier(id) => tm.var_types.get(&id.node.name).cloned().ok_or(Fail),
        Expression::Constant(c) => match &c.node {
            Constant::Integer(_) => Ok(CType::int()),
            Constant::Character(_) => Ok(CType::int()),
            Constant::Float(f) => Ok(match f.suffix.format {
                FloatFormat::Float => CType::basic("float"),
                _ => CType::basic("double"),
            }),
        },
        Expression::StringLiteral(_) => Ok(CType::ptr(CType::basic("char"))),
        Expression::Member(m) => {
            let mut lhs = expr_type(&m.node.expression.node, tm)?;
            if matches!(m.node.operator.node, MemberOperator::Indirect) {
                lhs = deref_type(&lhs, tm)?;
            }
            match resolve_typedefs(lhs, tm) {
                CType::Struct { name, .. } => {
                    struct_member_type(&name, &m.node.identifier.node.name, tm)
                }
                _ => Err(Fail),
            }
        }
        Expression::Cast(c) => ctype_from_type_name(&c.node.type_name.node),
        Expression::UnaryOperator(u) => {
            let sub = &u.node.operand.node;
            match u.node.operator.node {
                UnaryOperator::PostIncrement
                | UnaryOperator::PostDecrement
                | UnaryOperator::PreIncrement
                | UnaryOperator::PreDecrement => expr_type(sub, tm),
                UnaryOperator::Address => Ok(CType::ptr(expr_type(sub, tm)?)),
                UnaryOperator::Indirection => deref_type(&expr_type(sub, tm)?, tm),
                UnaryOperator::Minus | UnaryOperator::Plus => {
                    let t = pointer_decay(&expr_type(sub, tm)?, tm);
                    if allowed_basic_type(&t, tm, &["double"]) {
                        Ok(CType::basic("double"))
                    } else if allowed_basic_type(&t, tm, &["float"]) {
                        Ok(CType::basic("float"))
                    } else {
                        Ok(CType::int())
                    }
                }
                UnaryOperator::Complement | UnaryOperator::Negate => Ok(CType::int()),
            }
        }
        Expression::SizeOfTy(_) | Expression::SizeOfVal(_) | Expression::AlignOf(_) => {
            Ok(CType::int())
        }
        Expression::BinaryOperator(b) => {
            let op = &b.node.operator.node;
            if is_assign_op(op) {
                return expr_type(&b.node.lhs.node, tm);
            }
            if matches!(op, BinaryOperator::Index) {
                let lhs = pointer_decay(&expr_type(&b.node.lhs.node, tm)?, tm);
                return deref_type(&lhs, tm);
            }
            let lhs = pointer_decay(&expr_type(&b.node.lhs.node, tm)?, tm);
            match op {
                BinaryOperator::ShiftLeft | BinaryOperator::ShiftRight => return Ok(lhs),
                BinaryOperator::Less
                | BinaryOperator::Greater
                | BinaryOperator::LessOrEqual
                | BinaryOperator::GreaterOrEqual
                | BinaryOperator::Equals
                | BinaryOperator::NotEquals
                | BinaryOperator::LogicalAnd
                | BinaryOperator::LogicalOr
                | BinaryOperator::BitwiseAnd
                | BinaryOperator::BitwiseOr
                | BinaryOperator::BitwiseXor
                | BinaryOperator::Modulo => return Ok(CType::int()),
                _ => {}
            }
            let rhs = pointer_decay(&expr_type(&b.node.rhs.node, tm)?, tm);
            let real_l = resolve_typedefs(lhs.clone(), tm);
            let real_r = resolve_typedefs(rhs.clone(), tm);
            if matches!(op, BinaryOperator::Plus | BinaryOperator::Minus) {
                let lptr = matches!(real_l, CType::Ptr(_));
                let rptr = matches!(real_r, CType::Ptr(_));
                if lptr && rptr {
                    return Ok(CType::int());
                }
                if lptr {
                    return Ok(lhs);
                }
                if rptr {
                    return Ok(rhs);
                }
            }
            let float = |t: &CType, name: &str| allowed_basic_type(t, tm, &[name]);
            if float(&real_l, "double") || float(&real_r, "double") {
                Ok(CType::basic("double"))
            } else if float(&real_l, "float") || float(&real_r, "float") {
                Ok(CType::basic("float"))
            } else {
                Ok(CType::int())
            }
        }
        Expression::Call(c) => {
            let mut t = resolve_typedefs(expr_type(&c.node.callee.node, tm)?, tm);
            if let CType::Ptr(inner) = t {
                t = resolve_typedefs(*inner, tm);
            }
            match t {
                CType::Func(ret) => Ok(*ret),
                _ => Err(Fail),
            }
        }
        Expression::Comma(items) => {
            let last = items.last().ok_or(Fail)?;
            expr_type(&last.node, tm)
        }
        Expression::Conditional(c) => expr_type(&c.node.then_expression.node, tm),
        Expression::CompoundLiteral(cl) => ctype_from_type_name(&cl.node.type_name.node),
        _ => Err(Fail),
    }
}

pub fn decayed_expr_type(e: &Expression, tm: &TypeMap) -> Result<CType, Fail> {
    Ok(pointer_decay(&expr_type(e, tm)?, tm))
}

pub fn is_assign_op(op: &BinaryOperator) -> bool {
    matches!(
        op,
        BinaryOperator::Assign
            | BinaryOperator::AssignMultiply
            | BinaryOperator::AssignDivide
            | BinaryOperator::AssignModulo
            | BinaryOperator::AssignPlus
            | BinaryOperator::AssignMinus
            | BinaryOperator::AssignShiftLeft
            | BinaryOperator::AssignShiftRight
            | BinaryOperator::AssignBitwiseAnd
            | BinaryOperator::AssignBitwiseXor
            | BinaryOperator::AssignBitwiseOr
    )
}
