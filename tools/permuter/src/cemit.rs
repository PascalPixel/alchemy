//! C source emitter over the `lang-c` AST.
//!
//! `lang-c` parses C but does not print it; this module is the inverse. It
//! emits compilable C89-style source for the constructs used by this
//! repository's reconstructed sources: typedefs, struct/union/enum
//! definitions with bitfields, extern object and array declarations,
//! function declarations and definitions, pointers with qualifier placement
//! (`struct Rec *volatile f80;`), function pointers, casts, sizeof,
//! ternaries, comma expressions, and all C89 statements. Formatting is not
//! preserved; expressions are parenthesized conservatively so that the
//! emitted source has exactly the parse of the input AST.

use lang_c::ast::*;
use lang_c::span::Node;

pub fn emit_translation_unit(unit: &TranslationUnit) -> String {
    let mut out = String::new();
    for decl in &unit.0 {
        match &decl.node {
            ExternalDeclaration::Declaration(d) => {
                emit_declaration(&mut out, &d.node, 0);
            }
            ExternalDeclaration::FunctionDefinition(f) => {
                emit_function_definition(&mut out, &f.node);
            }
            ExternalDeclaration::StaticAssert(s) => {
                emit_static_assert(&mut out, &s.node, 0);
            }
        }
    }
    out
}

fn indent(out: &mut String, level: usize) {
    for _ in 0..level {
        out.push_str("    ");
    }
}

// ---------------------------------------------------------------- external

fn emit_function_definition(out: &mut String, f: &FunctionDefinition) {
    let specs = declaration_specifiers(&f.specifiers);
    out.push_str(&specs);
    if !specs.is_empty() {
        out.push(' ');
    }
    out.push_str(&declarator_string(&f.declarator.node));
    out.push('\n');
    for d in &f.declarations {
        emit_declaration(out, &d.node, 0);
    }
    emit_statement(out, &f.statement.node, 0);
    out.push('\n');
}

fn emit_static_assert(out: &mut String, s: &StaticAssert, level: usize) {
    indent(out, level);
    out.push_str("_Static_assert(");
    out.push_str(&expr(&s.expression.node));
    out.push_str(", ");
    out.push_str(&string_literal(&s.message.node));
    out.push_str(");\n");
}

fn emit_declaration(out: &mut String, d: &Declaration, level: usize) {
    indent(out, level);
    out.push_str(&declaration_string(d));
    out.push_str(";\n");
}

fn declaration_string(d: &Declaration) -> String {
    let mut s = declaration_specifiers(&d.specifiers);
    let mut first = true;
    for init in &d.declarators {
        if first {
            if !s.is_empty() {
                s.push(' ');
            }
            first = false;
        } else {
            s.push_str(", ");
        }
        s.push_str(&declarator_string(&init.node.declarator.node));
        if let Some(init) = &init.node.initializer {
            s.push_str(" = ");
            s.push_str(&initializer(&init.node));
        }
    }
    s
}

// ---------------------------------------------------------------- specifiers

fn declaration_specifiers(specs: &[Node<DeclarationSpecifier>]) -> String {
    let mut parts = Vec::new();
    for s in specs {
        match &s.node {
            DeclarationSpecifier::StorageClass(sc) => parts.push(storage_class(&sc.node)),
            DeclarationSpecifier::TypeSpecifier(ts) => parts.push(type_specifier(&ts.node)),
            DeclarationSpecifier::TypeQualifier(tq) => {
                parts.push(type_qualifier(&tq.node).to_string())
            }
            DeclarationSpecifier::Function(fs) => parts.push(
                match fs.node {
                    FunctionSpecifier::Inline => "inline",
                    FunctionSpecifier::Noreturn => "_Noreturn",
                }
                .to_string(),
            ),
            DeclarationSpecifier::Alignment(_) => {}
            DeclarationSpecifier::Extension(_) => {}
        }
    }
    parts.join(" ")
}

fn specifier_qualifiers(specs: &[Node<SpecifierQualifier>]) -> String {
    let mut parts = Vec::new();
    for s in specs {
        match &s.node {
            SpecifierQualifier::TypeSpecifier(ts) => parts.push(type_specifier(&ts.node)),
            SpecifierQualifier::TypeQualifier(tq) => {
                parts.push(type_qualifier(&tq.node).to_string())
            }
            SpecifierQualifier::Extension(_) => {}
        }
    }
    parts.join(" ")
}

fn storage_class(sc: &StorageClassSpecifier) -> String {
    match sc {
        StorageClassSpecifier::Typedef => "typedef",
        StorageClassSpecifier::Extern => "extern",
        StorageClassSpecifier::Static => "static",
        StorageClassSpecifier::ThreadLocal => "_Thread_local",
        StorageClassSpecifier::Auto => "auto",
        StorageClassSpecifier::Register => "register",
    }
    .to_string()
}

fn type_qualifier(tq: &TypeQualifier) -> &'static str {
    match tq {
        TypeQualifier::Const => "const",
        TypeQualifier::Restrict => "restrict",
        TypeQualifier::Volatile => "volatile",
        TypeQualifier::Atomic => "_Atomic",
        _ => "",
    }
}

fn type_specifier(ts: &TypeSpecifier) -> String {
    match ts {
        TypeSpecifier::Void => "void".to_string(),
        TypeSpecifier::Char => "char".to_string(),
        TypeSpecifier::Short => "short".to_string(),
        TypeSpecifier::Int => "int".to_string(),
        TypeSpecifier::Long => "long".to_string(),
        TypeSpecifier::Float => "float".to_string(),
        TypeSpecifier::Double => "double".to_string(),
        TypeSpecifier::Signed => "signed".to_string(),
        TypeSpecifier::Unsigned => "unsigned".to_string(),
        TypeSpecifier::Bool => "_Bool".to_string(),
        TypeSpecifier::Complex => "_Complex".to_string(),
        TypeSpecifier::Atomic(t) => format!("_Atomic({})", type_name(&t.node)),
        TypeSpecifier::Struct(st) => struct_type(&st.node),
        TypeSpecifier::Enum(et) => enum_type(&et.node),
        TypeSpecifier::TypedefName(id) => id.node.name.clone(),
        TypeSpecifier::TypeOf(_) => "/* typeof unsupported */".to_string(),
        TypeSpecifier::TS18661Float(_) => "/* ts18661 unsupported */".to_string(),
    }
}

fn struct_type(st: &StructType) -> String {
    let mut s = match st.kind.node {
        StructKind::Struct => "struct".to_string(),
        StructKind::Union => "union".to_string(),
    };
    if let Some(id) = &st.identifier {
        s.push(' ');
        s.push_str(&id.node.name);
    }
    if let Some(decls) = &st.declarations {
        s.push_str(" {\n");
        for d in decls {
            match &d.node {
                StructDeclaration::Field(f) => {
                    s.push_str("    ");
                    s.push_str(&struct_field(&f.node));
                    s.push_str(";\n");
                }
                StructDeclaration::StaticAssert(sa) => {
                    let mut t = String::new();
                    emit_static_assert(&mut t, &sa.node, 1);
                    s.push_str(&t);
                }
            }
        }
        s.push('}');
    }
    s
}

fn struct_field(f: &StructField) -> String {
    let mut s = specifier_qualifiers(&f.specifiers);
    let mut first = true;
    for d in &f.declarators {
        if first {
            if !s.is_empty() {
                s.push(' ');
            }
            first = false;
        } else {
            s.push_str(", ");
        }
        if let Some(decl) = &d.node.declarator {
            s.push_str(&declarator_string(&decl.node));
        }
        if let Some(width) = &d.node.bit_width {
            s.push_str(" : ");
            s.push_str(&expr(&width.node));
        }
    }
    s
}

fn enum_type(et: &EnumType) -> String {
    let mut s = "enum".to_string();
    if let Some(id) = &et.identifier {
        s.push(' ');
        s.push_str(&id.node.name);
    }
    if !et.enumerators.is_empty() {
        s.push_str(" {\n");
        for (i, e) in et.enumerators.iter().enumerate() {
            s.push_str("    ");
            s.push_str(&e.node.identifier.node.name);
            if let Some(v) = &e.node.expression {
                s.push_str(" = ");
                s.push_str(&expr(&v.node));
            }
            if i + 1 < et.enumerators.len() {
                s.push(',');
            }
            s.push('\n');
        }
        s.push('}');
    }
    s
}

// ---------------------------------------------------------------- declarators

fn declarator_string(d: &Declarator) -> String {
    // Pointers come first in `derived` (source order), suffixes after.
    let mut prefix = String::new();
    let mut suffix = String::new();
    for der in &d.derived {
        match &der.node {
            DerivedDeclarator::Pointer(quals) | DerivedDeclarator::Block(quals) => {
                prefix.push('*');
                for q in quals {
                    if let PointerQualifier::TypeQualifier(tq) = &q.node {
                        prefix.push_str(type_qualifier(&tq.node));
                        prefix.push(' ');
                    }
                }
            }
            DerivedDeclarator::Array(a) => {
                suffix.push('[');
                for q in &a.node.qualifiers {
                    suffix.push_str(type_qualifier(&q.node));
                    suffix.push(' ');
                }
                match &a.node.size {
                    ArraySize::Unknown => {}
                    ArraySize::VariableUnknown => suffix.push('*'),
                    ArraySize::VariableExpression(e) => suffix.push_str(&expr(&e.node)),
                    ArraySize::StaticExpression(e) => {
                        suffix.push_str("static ");
                        suffix.push_str(&expr(&e.node));
                    }
                }
                suffix.push(']');
            }
            DerivedDeclarator::Function(f) => {
                suffix.push('(');
                let mut parts = Vec::new();
                for p in &f.node.parameters {
                    parts.push(parameter(&p.node));
                }
                if matches!(f.node.ellipsis, Ellipsis::Some) {
                    parts.push("...".to_string());
                }
                suffix.push_str(&parts.join(", "));
                suffix.push(')');
            }
            DerivedDeclarator::KRFunction(ids) => {
                suffix.push('(');
                let names: Vec<&str> = ids.iter().map(|i| i.node.name.as_str()).collect();
                suffix.push_str(&names.join(", "));
                suffix.push(')');
            }
        }
    }
    let core = match &d.kind.node {
        DeclaratorKind::Abstract => String::new(),
        DeclaratorKind::Identifier(id) => id.node.name.clone(),
        DeclaratorKind::Declarator(inner) => format!("({})", declarator_string(&inner.node)),
    };
    format!("{prefix}{core}{suffix}")
}

fn parameter(p: &ParameterDeclaration) -> String {
    let mut s = declaration_specifiers(&p.specifiers);
    if let Some(d) = &p.declarator {
        let ds = declarator_string(&d.node);
        if !ds.is_empty() {
            if !s.is_empty() {
                s.push(' ');
            }
            s.push_str(&ds);
        }
    }
    s
}

fn type_name(t: &TypeName) -> String {
    let mut s = specifier_qualifiers(&t.specifiers);
    if let Some(d) = &t.declarator {
        let ds = declarator_string(&d.node);
        if !ds.is_empty() {
            if !s.is_empty() {
                s.push(' ');
            }
            s.push_str(&ds);
        }
    }
    s
}

// ---------------------------------------------------------------- initializers

fn initializer(i: &Initializer) -> String {
    match i {
        Initializer::Expression(e) => expr(&e.node),
        Initializer::List(items) => {
            let mut parts = Vec::new();
            for item in items {
                let mut s = String::new();
                for d in &item.node.designation {
                    match &d.node {
                        Designator::Index(e) => {
                            s.push('[');
                            s.push_str(&expr(&e.node));
                            s.push(']');
                        }
                        Designator::Member(id) => {
                            s.push('.');
                            s.push_str(&id.node.name);
                        }
                        Designator::Range(r) => {
                            s.push('[');
                            s.push_str(&expr(&r.node.from.node));
                            s.push_str(" ... ");
                            s.push_str(&expr(&r.node.to.node));
                            s.push(']');
                        }
                    }
                }
                if !item.node.designation.is_empty() {
                    s.push_str(" = ");
                }
                s.push_str(&initializer(&item.node.initializer.node));
                parts.push(s);
            }
            format!("{{{}}}", parts.join(", "))
        }
    }
}

// ---------------------------------------------------------------- statements

fn emit_statement(out: &mut String, s: &Statement, level: usize) {
    match s {
        Statement::Labeled(l) => {
            // Labels stand at the parent's indentation; the labeled
            // statement follows on its own line.
            indent(out, level);
            match &l.node.label.node {
                Label::Identifier(id) => {
                    out.push_str(&id.node.name);
                    out.push(':');
                }
                Label::Case(e) => {
                    out.push_str("case ");
                    out.push_str(&expr(&e.node));
                    out.push(':');
                }
                Label::CaseRange(r) => {
                    out.push_str("case ");
                    out.push_str(&expr(&r.node.low.node));
                    out.push_str(" ... ");
                    out.push_str(&expr(&r.node.high.node));
                    out.push(':');
                }
                Label::Default => out.push_str("default:"),
            }
            out.push('\n');
            emit_statement(out, &l.node.statement.node, level + 1);
        }
        Statement::Compound(items) => {
            indent(out, level);
            out.push_str("{\n");
            for item in items {
                match &item.node {
                    BlockItem::Declaration(d) => emit_declaration(out, &d.node, level + 1),
                    BlockItem::StaticAssert(sa) => emit_static_assert(out, &sa.node, level + 1),
                    BlockItem::Statement(st) => emit_statement(out, &st.node, level + 1),
                }
            }
            indent(out, level);
            out.push_str("}\n");
        }
        Statement::Expression(e) => {
            indent(out, level);
            if let Some(e) = e {
                out.push_str(&expr(&e.node));
            }
            out.push_str(";\n");
        }
        Statement::If(i) => {
            indent(out, level);
            out.push_str("if (");
            out.push_str(&expr(&i.node.condition.node));
            out.push_str(")\n");
            emit_substatement(out, &i.node.then_statement.node, level);
            if let Some(e) = &i.node.else_statement {
                indent(out, level);
                out.push_str("else\n");
                emit_substatement(out, &e.node, level);
            }
        }
        Statement::Switch(sw) => {
            indent(out, level);
            out.push_str("switch (");
            out.push_str(&expr(&sw.node.expression.node));
            out.push_str(")\n");
            emit_substatement(out, &sw.node.statement.node, level);
        }
        Statement::While(w) => {
            indent(out, level);
            out.push_str("while (");
            out.push_str(&expr(&w.node.expression.node));
            out.push_str(")\n");
            emit_substatement(out, &w.node.statement.node, level);
        }
        Statement::DoWhile(d) => {
            indent(out, level);
            out.push_str("do\n");
            emit_substatement(out, &d.node.statement.node, level);
            indent(out, level);
            out.push_str("while (");
            out.push_str(&expr(&d.node.expression.node));
            out.push_str(");\n");
        }
        Statement::For(f) => {
            indent(out, level);
            out.push_str("for (");
            match &f.node.initializer.node {
                ForInitializer::Empty => out.push(';'),
                ForInitializer::Expression(e) => {
                    out.push_str(&expr(&e.node));
                    out.push(';');
                }
                ForInitializer::Declaration(d) => {
                    out.push_str(&declaration_string(&d.node));
                    out.push(';');
                }
                ForInitializer::StaticAssert(_) => out.push(';'),
            }
            out.push(' ');
            if let Some(c) = &f.node.condition {
                out.push_str(&expr(&c.node));
            }
            out.push_str("; ");
            if let Some(st) = &f.node.step {
                out.push_str(&expr(&st.node));
            }
            out.push_str(")\n");
            emit_substatement(out, &f.node.statement.node, level);
        }
        Statement::Goto(id) => {
            indent(out, level);
            out.push_str("goto ");
            out.push_str(&id.node.name);
            out.push_str(";\n");
        }
        Statement::Continue => {
            indent(out, level);
            out.push_str("continue;\n");
        }
        Statement::Break => {
            indent(out, level);
            out.push_str("break;\n");
        }
        Statement::Return(e) => {
            indent(out, level);
            out.push_str("return");
            if let Some(e) = e {
                out.push(' ');
                out.push_str(&expr(&e.node));
            }
            out.push_str(";\n");
        }
        Statement::Asm(_) => {
            // Inline assembly is forbidden in this repository (`no_asm_c`);
            // an AST that carries it is emitted as an error marker so the
            // compile step fails loudly rather than silently dropping it.
            indent(out, level);
            out.push_str("#error asm statement unsupported\n");
        }
    }
}

/// A sub-statement of if/while/for/switch: always braced so the printed
/// nesting matches the AST regardless of dangling-else concerns.
fn emit_substatement(out: &mut String, s: &Statement, level: usize) {
    match s {
        Statement::Compound(_) => emit_statement(out, s, level),
        _ => {
            indent(out, level);
            out.push_str("{\n");
            emit_statement(out, s, level + 1);
            indent(out, level);
            out.push_str("}\n");
        }
    }
}

// ---------------------------------------------------------------- expressions

/// Expressions are printed with conservative parentheses: any operand that
/// is itself compound is wrapped, so the emitted text has exactly the parse
/// of the AST. Formatting is not a goal; correctness is.
fn expr(e: &Expression) -> String {
    match e {
        Expression::Identifier(id) => id.node.name.clone(),
        Expression::Constant(c) => constant(&c.node),
        Expression::StringLiteral(s) => string_literal(&s.node),
        Expression::Member(m) => {
            let op = match m.node.operator.node {
                MemberOperator::Direct => ".",
                MemberOperator::Indirect => "->",
            };
            format!(
                "{}{}{}",
                sub(&m.node.expression.node),
                op,
                m.node.identifier.node.name
            )
        }
        Expression::Call(c) => {
            let args: Vec<String> = c
                .node
                .arguments
                .iter()
                .map(|a| comma_safe(&a.node))
                .collect();
            format!("{}({})", sub(&c.node.callee.node), args.join(", "))
        }
        Expression::CompoundLiteral(cl) => {
            let mut s = format!("({})", type_name(&cl.node.type_name.node));
            let items = Initializer::List(cl.node.initializer_list.clone());
            s.push_str(&initializer(&items));
            s
        }
        Expression::SizeOfTy(t) => format!("sizeof({})", type_name(&t.node.0.node)),
        Expression::SizeOfVal(v) => format!("sizeof({})", expr(&v.node.0.node)),
        Expression::AlignOf(t) => format!("_Alignof({})", type_name(&t.node.0.node)),
        Expression::UnaryOperator(u) => {
            let operand = sub(&u.node.operand.node);
            match u.node.operator.node {
                UnaryOperator::PostIncrement => format!("{operand}++"),
                UnaryOperator::PostDecrement => format!("{operand}--"),
                UnaryOperator::PreIncrement => format!("++{operand}"),
                UnaryOperator::PreDecrement => format!("--{operand}"),
                UnaryOperator::Address => format!("&{operand}"),
                UnaryOperator::Indirection => format!("*{operand}"),
                UnaryOperator::Plus => format!("+{operand}"),
                UnaryOperator::Minus => format!("-{operand}"),
                UnaryOperator::Complement => format!("~{operand}"),
                UnaryOperator::Negate => format!("!{operand}"),
            }
        }
        Expression::Cast(c) => format!(
            "({}){}",
            type_name(&c.node.type_name.node),
            sub(&c.node.expression.node)
        ),
        Expression::BinaryOperator(b) => {
            if b.node.operator.node == BinaryOperator::Index {
                return format!("{}[{}]", sub(&b.node.lhs.node), expr(&b.node.rhs.node));
            }
            let op = binary_operator(&b.node.operator.node);
            format!("{} {} {}", sub(&b.node.lhs.node), op, sub(&b.node.rhs.node))
        }
        Expression::Conditional(c) => format!(
            "{} ? {} : {}",
            sub(&c.node.condition.node),
            sub(&c.node.then_expression.node),
            sub(&c.node.else_expression.node)
        ),
        Expression::Comma(items) => {
            let parts: Vec<String> = items.iter().map(|i| comma_safe(&i.node)).collect();
            parts.join(", ")
        }
        Expression::OffsetOf(o) => {
            let mut s = format!(
                "__builtin_offsetof({}, {}",
                type_name(&o.node.type_name.node),
                o.node.designator.node.base.node.name
            );
            for m in &o.node.designator.node.members {
                match &m.node {
                    OffsetMember::Member(id) => {
                        s.push('.');
                        s.push_str(&id.node.name);
                    }
                    OffsetMember::IndirectMember(id) => {
                        s.push_str("->");
                        s.push_str(&id.node.name);
                    }
                    OffsetMember::Index(e) => {
                        s.push('[');
                        s.push_str(&expr(&e.node));
                        s.push(']');
                    }
                }
            }
            s.push(')');
            s
        }
        Expression::GenericSelection(_) | Expression::VaArg(_) | Expression::Statement(_) => {
            "/* unsupported expression */".to_string()
        }
    }
}

/// An operand position: wrap anything compound in parentheses.
fn sub(e: &Expression) -> String {
    match e {
        Expression::Identifier(_)
        | Expression::Constant(_)
        | Expression::StringLiteral(_)
        | Expression::Call(_)
        | Expression::Member(_) => expr(e),
        Expression::BinaryOperator(b) if b.node.operator.node == BinaryOperator::Index => expr(e),
        _ => format!("({})", expr(e)),
    }
}

/// An argument or comma-list element: a nested comma expression must keep
/// its own parentheses; everything else may stand bare.
fn comma_safe(e: &Expression) -> String {
    match e {
        Expression::Comma(_) => format!("({})", expr(e)),
        _ => expr(e),
    }
}

fn binary_operator(op: &BinaryOperator) -> &'static str {
    match op {
        BinaryOperator::Index => "[]",
        BinaryOperator::Multiply => "*",
        BinaryOperator::Divide => "/",
        BinaryOperator::Modulo => "%",
        BinaryOperator::Plus => "+",
        BinaryOperator::Minus => "-",
        BinaryOperator::ShiftLeft => "<<",
        BinaryOperator::ShiftRight => ">>",
        BinaryOperator::Less => "<",
        BinaryOperator::Greater => ">",
        BinaryOperator::LessOrEqual => "<=",
        BinaryOperator::GreaterOrEqual => ">=",
        BinaryOperator::Equals => "==",
        BinaryOperator::NotEquals => "!=",
        BinaryOperator::BitwiseAnd => "&",
        BinaryOperator::BitwiseXor => "^",
        BinaryOperator::BitwiseOr => "|",
        BinaryOperator::LogicalAnd => "&&",
        BinaryOperator::LogicalOr => "||",
        BinaryOperator::Assign => "=",
        BinaryOperator::AssignMultiply => "*=",
        BinaryOperator::AssignDivide => "/=",
        BinaryOperator::AssignModulo => "%=",
        BinaryOperator::AssignPlus => "+=",
        BinaryOperator::AssignMinus => "-=",
        BinaryOperator::AssignShiftLeft => "<<=",
        BinaryOperator::AssignShiftRight => ">>=",
        BinaryOperator::AssignBitwiseAnd => "&=",
        BinaryOperator::AssignBitwiseXor => "^=",
        BinaryOperator::AssignBitwiseOr => "|=",
    }
}

fn constant(c: &Constant) -> String {
    match c {
        Constant::Integer(i) => {
            let prefix = match i.base {
                IntegerBase::Decimal => "",
                IntegerBase::Octal => "0",
                IntegerBase::Hexadecimal => "0x",
                IntegerBase::Binary => "0b",
            };
            let mut s = format!("{}{}", prefix, i.number);
            if i.suffix.unsigned {
                s.push('u');
            }
            match i.suffix.size {
                IntegerSize::Int => {}
                IntegerSize::Long => s.push('l'),
                IntegerSize::LongLong => s.push_str("ll"),
            }
            s
        }
        Constant::Float(f) => {
            let prefix = match f.base {
                FloatBase::Decimal => "",
                FloatBase::Hexadecimal => "0x",
            };
            let mut s = format!("{}{}", prefix, f.number);
            match f.suffix.format {
                FloatFormat::Float => s.push('f'),
                FloatFormat::Double => {}
                FloatFormat::LongDouble => s.push('l'),
                FloatFormat::TS18661Format(_) => {}
            }
            s
        }
        Constant::Character(c) => c.clone(),
    }
}

fn string_literal(s: &StringLiteral) -> String {
    s.join(" ")
}

/// Renders one expression; used by the AST randomizer for structural
/// equality (`equal_ast`) via canonical text.
pub fn emit_expression(e: &Expression) -> String {
    expr(e)
}

pub fn self_test() -> Result<(), String> {
    let src = "int f(int a) { return a * 2 + 1; }";
    let mut cfg = lang_c::driver::Config::default();
    cfg.flavor = lang_c::driver::Flavor::GnuC11;
    let parsed = lang_c::driver::parse_preprocessed(&cfg, src.to_string())
        .map_err(|e| format!("cemit self-test parse failed: {e}"))?;
    let emitted = emit_translation_unit(&parsed.unit);
    if !emitted.contains("return") {
        return Err("cemit self-test: emitted source lost the return".to_string());
    }
    Ok(())
}
