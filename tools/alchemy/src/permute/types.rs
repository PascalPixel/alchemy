//! Just enough C typing for the mutations that need a width, a signedness or
//! a spelling: temporaries, casts and pointer forms. Anything unknown stays
//! unknown and the mutation declines the site.

use super::ast::*;
use super::parse::Unit;
use std::collections::{BTreeMap, BTreeSet};

#[derive(Clone, Debug, PartialEq, Eq)]
pub enum CType {
    Void,
    Int { bits: u8, signed: bool },
    Float,
    Ptr(Box<CType>),
    Array(Box<CType>),
    Record(String),
    Func(Box<CType>, Vec<CType>),
    Named(String, Box<CType>),
    Volatile(Box<CType>),
}

pub const INT: CType = CType::Int {
    bits: 32,
    signed: true,
};

impl CType {
    /// The type without typedef names or qualifiers.
    pub fn strip(&self) -> &CType {
        match self {
            CType::Named(_, inner) | CType::Volatile(inner) => inner.strip(),
            other => other,
        }
    }

    pub fn is_volatile(&self) -> bool {
        match self {
            CType::Volatile(_) => true,
            CType::Named(_, inner) => inner.is_volatile(),
            _ => false,
        }
    }

    pub fn int(&self) -> Option<(u8, bool)> {
        match self.strip() {
            CType::Int { bits, signed } => Some((*bits, *signed)),
            _ => None,
        }
    }

    pub fn is_pointer_like(&self) -> bool {
        matches!(self.strip(), CType::Ptr(_) | CType::Array(_))
    }

    pub fn pointee(&self) -> Option<&CType> {
        match self.strip() {
            CType::Ptr(inner) | CType::Array(inner) => Some(inner),
            _ => None,
        }
    }

    /// Arrays and functions used as values.
    pub fn decay(&self) -> CType {
        match self.strip() {
            CType::Array(inner) => CType::Ptr(inner.clone()),
            CType::Func(..) => CType::Ptr(Box::new(self.clone())),
            _ => self.clone(),
        }
    }

    pub fn promoted(&self) -> CType {
        match self.strip() {
            CType::Int { bits, .. } if *bits < 32 => INT,
            _ => self.decay(),
        }
    }

    /// The same type after typedef resolution, ignoring qualifiers.
    pub fn same(&self, other: &CType) -> bool {
        match (self.strip(), other.strip()) {
            (CType::Ptr(a), CType::Ptr(b)) | (CType::Array(a), CType::Array(b)) => a.same(b),
            (a, b) => a == b,
        }
    }
}

/// Declarations visible to the function: the unit's and its own.
pub struct Env {
    typedefs: BTreeMap<String, (Vec<String>, Declarator)>,
    records: BTreeMap<String, Vec<(Vec<String>, Declarator)>>,
    globals: BTreeMap<String, CType>,
    enum_constants: BTreeSet<String>,
    pub locals: BTreeMap<String, CType>,
    /// Locals whose address is taken; they behave like memory.
    pub addressed: BTreeSet<String>,
    pub return_type: Option<CType>,
    spellings: BTreeMap<(u8, bool), String>,
    /// Rewrites only touch statements whose own text matches.
    pub focus: Option<regex::Regex>,
}

impl Env {
    pub fn new(unit: &Unit, function: &Function) -> Env {
        let mut env = Env {
            typedefs: unit.typedefs.clone(),
            records: unit.records.clone(),
            globals: BTreeMap::new(),
            enum_constants: unit.enum_constants.clone(),
            locals: BTreeMap::new(),
            addressed: BTreeSet::new(),
            return_type: None,
            spellings: BTreeMap::new(),
            focus: None,
        };
        for (name, (specs, item)) in &unit.globals {
            let ty = env.declared(specs, item);
            env.globals.insert(name.clone(), ty);
        }
        for (bits, signed, preferred, fallback) in [
            (32, true, "s32", "int"),
            (32, false, "u32", "unsigned int"),
            (16, true, "s16", "short"),
            (16, false, "u16", "unsigned short"),
            (8, true, "s8", "signed char"),
            (8, false, "u8", "unsigned char"),
        ] {
            let spelled = env.typedefs.contains_key(preferred)
                && *env.resolve(&[preferred.to_string()]).strip() == CType::Int { bits, signed };
            env.spellings.insert(
                (bits, signed),
                if spelled { preferred } else { fallback }.to_string(),
            );
        }
        env.refresh(function);
        env
    }

    /// Re-read the function's parameters and locals after a mutation.
    pub fn refresh(&mut self, function: &Function) {
        self.locals.clear();
        self.addressed.clear();
        if let Some(ty) = self.globals.get(&function.name) {
            if let CType::Func(ret, _) = ty.strip() {
                self.return_type = Some((**ret).clone());
            }
        }
        if self.return_type.is_none() {
            let prefix = function
                .header
                .find(&format!("{}(", function.name))
                .or_else(|| function.header.find(&function.name))
                .map_or("", |at| &function.header[..at]);
            let specs: Vec<String> = prefix
                .split(|c: char| c.is_whitespace() || c == '*')
                .filter(|word| !word.is_empty())
                .map(str::to_string)
                .collect();
            let mut ty = self.resolve(&specs);
            for _ in 0..prefix.matches('*').count() {
                ty = CType::Ptr(Box::new(ty));
            }
            self.return_type = Some(ty);
        }
        for param in &function.params {
            for item in &param.items {
                let ty = self.declared(&param.specs, item);
                self.locals.insert(item.name.clone(), ty);
            }
        }
        for stmt in &function.body {
            stmt.each(&mut |stmt| {
                if let Stmt::Decl(decl) = stmt {
                    for item in &decl.items {
                        let ty = self.declared(&decl.specs, item);
                        self.locals.insert(item.name.clone(), ty);
                    }
                }
            });
            stmt.each_expr(&mut |expr| {
                if let Expr::Unary(UnOp::AddrOf, operand) = expr {
                    if let Some(name) = root_name(operand) {
                        self.addressed.insert(name.to_string());
                    }
                }
            });
        }
    }

    pub fn is_local(&self, name: &str) -> bool {
        self.locals.contains_key(name)
    }

    pub fn is_enum_constant(&self, name: &str) -> bool {
        !self.locals.contains_key(name) && self.enum_constants.contains(name)
    }

    pub fn variable(&self, name: &str) -> Option<&CType> {
        self.locals.get(name).or_else(|| self.globals.get(name))
    }

    pub fn is_typedef(&self, name: &str) -> bool {
        self.typedefs.contains_key(name)
    }

    /// Resolve declaration specifiers to a type.
    pub fn resolve(&self, specs: &[String]) -> CType {
        self.resolve_depth(specs, 0)
    }

    fn resolve_depth(&self, specs: &[String], depth: usize) -> CType {
        let mut volatile = false;
        let mut unsigned = false;
        let mut explicit_signed = false;
        let (mut chars, mut shorts, mut longs, mut ints) = (0, 0, 0, 0);
        let mut base = None;
        let mut words = specs.iter();
        while let Some(word) = words.next() {
            match word.as_str() {
                "volatile" | "__volatile__" | "__volatile" => volatile = true,
                "unsigned" => unsigned = true,
                "signed" | "__signed__" => explicit_signed = true,
                "char" => chars += 1,
                "short" => shorts += 1,
                "long" => longs += 1,
                "int" => ints += 1,
                "void" => base = Some(CType::Void),
                "float" | "double" => base = Some(CType::Float),
                "struct" | "union" => {
                    base = words.next().map(|tag| CType::Record(tag.clone()));
                }
                "enum" => {
                    words.next();
                    base = Some(INT);
                }
                "const" | "__const" | "register" | "static" | "extern" | "auto" | "typedef"
                | "inline" | "__inline__" | "__inline" => {}
                name => {
                    if let Some((specs, item)) = self.typedefs.get(name) {
                        if depth < 16 {
                            let inner = self.resolve_depth(specs, depth + 1);
                            let inner = self.apply(inner, item);
                            base = Some(CType::Named(name.to_string(), Box::new(inner)));
                        }
                    }
                }
            }
        }
        let _ = ints;
        let ty = match base {
            Some(ty) => ty,
            None if chars > 0 => CType::Int {
                bits: 8,
                signed: explicit_signed,
            },
            None if shorts > 0 => CType::Int {
                bits: 16,
                signed: !unsigned,
            },
            None if longs > 1 => CType::Int {
                bits: 64,
                signed: !unsigned,
            },
            None => CType::Int {
                bits: 32,
                signed: !unsigned,
            },
        };
        if volatile {
            CType::Volatile(Box::new(ty))
        } else {
            ty
        }
    }

    /// The type a declarator gives its name.
    pub fn declared(&self, specs: &[String], item: &Declarator) -> CType {
        let base = self.resolve(specs);
        self.apply(base, item)
    }

    fn apply(&self, base: CType, item: &Declarator) -> CType {
        let grouped = item.before.iter().position(|token| token == "(");
        let stars = |tokens: &[String]| tokens.iter().filter(|token| *token == "*").count();
        let suffix = |mut ty: CType, after: &[String]| {
            let mut arrays = 0;
            let mut function = false;
            let mut depth = 0;
            for token in after {
                match token.as_str() {
                    "[" if depth == 0 => {
                        arrays += 1;
                        depth += 1
                    }
                    "(" if depth == 0 => {
                        function = true;
                        depth += 1
                    }
                    "[" | "(" => depth += 1,
                    "]" | ")" => depth -= 1,
                    _ => {}
                }
            }
            if function {
                ty = CType::Func(Box::new(ty), Vec::new());
            }
            for _ in 0..arrays {
                ty = CType::Array(Box::new(ty));
            }
            ty
        };
        let mut ty = base;
        match grouped {
            None => {
                for _ in 0..stars(&item.before) {
                    ty = CType::Ptr(Box::new(ty));
                }
                suffix(ty, &item.after)
            }
            Some(at) => {
                for _ in 0..stars(&item.before[..at]) {
                    ty = CType::Ptr(Box::new(ty));
                }
                let close = item
                    .after
                    .iter()
                    .position(|token| token == ")")
                    .unwrap_or(0);
                ty = suffix(ty, &item.after[(close + 1).min(item.after.len())..]);
                ty = suffix(ty, &item.after[..close]);
                for _ in 0..stars(&item.before[at..]) {
                    ty = CType::Ptr(Box::new(ty));
                }
                ty
            }
        }
    }

    pub fn type_name(&self, name: &TypeName) -> CType {
        let split = name
            .tokens
            .iter()
            .position(|token| matches!(token.as_str(), "*" | "(" | "["))
            .unwrap_or(name.tokens.len());
        let item = Declarator {
            before: name.tokens[split..].to_vec(),
            name: String::new(),
            after: Vec::new(),
            init: None,
        };
        self.declared(&name.tokens[..split], &item)
    }

    fn member(&self, record: &CType, member: &str) -> Option<CType> {
        let CType::Record(tag) = record.strip() else {
            return None;
        };
        let members = self.records.get(tag)?;
        members
            .iter()
            .find(|(_, item)| item.name == member)
            .map(|(specs, item)| self.declared(specs, item))
    }

    /// The type of `expr`, when every part of it is known.
    pub fn type_of(&self, expr: &Expr) -> Option<CType> {
        Some(match expr {
            Expr::Ident(name) => {
                if let Some(ty) = self.variable(name) {
                    ty.clone()
                } else if self.is_enum_constant(name) {
                    INT
                } else {
                    return None;
                }
            }
            Expr::Number(text) => number_type(text)?,
            Expr::Char(_) => INT,
            Expr::Str(_) => CType::Ptr(Box::new(CType::Int {
                bits: 8,
                signed: false,
            })),
            Expr::Unary(op, operand) => match op {
                UnOp::Neg | UnOp::Plus | UnOp::BitNot => self.type_of(operand)?.promoted(),
                UnOp::Not => INT,
                UnOp::Deref => self.type_of(operand)?.decay().pointee()?.clone(),
                UnOp::AddrOf => CType::Ptr(Box::new(self.type_of(operand)?)),
                UnOp::PreInc | UnOp::PreDec => self.type_of(operand)?,
            },
            Expr::PostInc(operand) | Expr::PostDec(operand) => self.type_of(operand)?,
            Expr::Binary(op, left, right) => {
                if op.is_comparison() || matches!(op, BinOp::And | BinOp::Or) {
                    return Some(INT);
                }
                let left = self.type_of(left)?;
                let right = self.type_of(right)?;
                match op {
                    BinOp::Shl | BinOp::Shr => left.promoted(),
                    BinOp::Add | BinOp::Sub if left.is_pointer_like() => {
                        if right.is_pointer_like() {
                            INT
                        } else {
                            left.decay()
                        }
                    }
                    BinOp::Add if right.is_pointer_like() => right.decay(),
                    _ => usual(&left, &right)?,
                }
            }
            Expr::Assign(_, target, _) => self.type_of(target)?,
            Expr::Cond(_, then, other) => {
                let then = self.type_of(then)?;
                let other = self.type_of(other)?;
                if then.int().is_some() && other.int().is_some() {
                    usual(&then, &other)?
                } else {
                    then.decay()
                }
            }
            Expr::Comma(_, right) => self.type_of(right)?,
            Expr::Call(callee, _) => match self.type_of(callee)?.strip() {
                CType::Func(ret, _) => (**ret).clone(),
                CType::Ptr(inner) => match inner.strip() {
                    CType::Func(ret, _) => (**ret).clone(),
                    _ => return None,
                },
                _ => return None,
            },
            Expr::Index(base, index) => {
                let base = self.type_of(base)?;
                if base.is_pointer_like() {
                    base.pointee()?.clone()
                } else {
                    self.type_of(index)?.pointee()?.clone()
                }
            }
            Expr::Member(base, member, arrow) => {
                let base = self.type_of(base)?;
                let record = if *arrow {
                    base.pointee()?.clone()
                } else {
                    base
                };
                self.member(&record, member)?
            }
            Expr::Cast(name, _) => self.type_name(name),
            Expr::SizeofExpr(_) | Expr::SizeofType(_) => CType::Int {
                bits: 32,
                signed: false,
            },
        })
    }

    /// Declaration specifiers and stars that spell `ty`, if it can be spelled.
    pub fn spell(&self, ty: &CType) -> Option<String> {
        match ty {
            CType::Named(name, inner) => {
                if inner.is_volatile() {
                    None
                } else {
                    Some(name.clone())
                }
            }
            CType::Volatile(_) | CType::Func(..) | CType::Float => None,
            CType::Void => Some("void".into()),
            CType::Int { bits, signed } => self.spellings.get(&(*bits, *signed)).cloned(),
            CType::Ptr(inner) | CType::Array(inner) => {
                if matches!(inner.strip(), CType::Func(..)) {
                    return None;
                }
                let base = self.spell(inner)?;
                Some(if base.ends_with('*') {
                    format!("{base}*")
                } else {
                    format!("{base} *")
                })
            }
            CType::Record(tag) => {
                if tag.starts_with("__anonymous_") {
                    None
                } else {
                    Some(format!("struct {tag}"))
                }
            }
        }
    }

    /// The spelling of an integer type of this width and signedness.
    pub fn int_spelling(&self, bits: u8, signed: bool) -> Option<&str> {
        self.spellings.get(&(bits, signed)).map(String::as_str)
    }
}

/// The variable an lvalue names, through members and indexing of arrays.
pub fn root_name(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Ident(name) => Some(name),
        Expr::Member(base, _, false) | Expr::Index(base, _) => root_name(base),
        _ => None,
    }
}

fn number_type(text: &str) -> Option<CType> {
    let lower = text.to_ascii_lowercase();
    if lower.contains('.') || (!lower.starts_with("0x") && lower.contains('e')) {
        return Some(CType::Float);
    }
    let digits = lower.trim_end_matches(['u', 'l']);
    let unsigned_suffix = lower[digits.len()..].contains('u');
    let value = if let Some(hex) = digits.strip_prefix("0x") {
        u64::from_str_radix(hex, 16).ok()?
    } else if digits.len() > 1 && digits.starts_with('0') {
        u64::from_str_radix(&digits[1..], 8).ok()?
    } else {
        digits.parse::<u64>().ok()?
    };
    let signed = !unsigned_suffix
        && (value <= 0x7fff_ffff || !(lower.starts_with("0x") || lower.starts_with('0')));
    Some(CType::Int { bits: 32, signed })
}

fn usual(left: &CType, right: &CType) -> Option<CType> {
    let left = left.promoted();
    let right = right.promoted();
    if matches!(left.strip(), CType::Float) || matches!(right.strip(), CType::Float) {
        return Some(CType::Float);
    }
    let (left_bits, left_signed) = left.int()?;
    let (right_bits, right_signed) = right.int()?;
    let bits = left_bits.max(right_bits);
    let signed = if left_bits == right_bits {
        left_signed && right_signed
    } else if left_bits > right_bits {
        left_signed
    } else {
        right_signed
    };
    Some(CType::Int { bits, signed })
}

#[cfg(test)]
mod tests {
    use super::super::parse::{locate, scan_unit};
    use super::*;

    #[test]
    fn expressions_type_through_typedefs_records_and_promotions() {
        let header = "typedef unsigned short u16;\ntypedef signed int s32;\ntypedef unsigned char u8;\nstruct Unit { u8 pad[4]; u16 hp; s32 *link; };\nextern struct Unit *gUnits[4];\nu16 Unit_Get(s32);\n";
        let source = format!("{header}s32 F(s32 index)\n{{\n    u8 small;\n    return 0;\n}}\n");
        let unit = scan_unit(header).unwrap();
        let located = locate(&source, None, &unit.typedef_names()).unwrap();
        let env = Env::new(&unit, &located.function);
        let expr = |text: &str| {
            let body = super::super::parse::parse_body(&format!("{text};"), &["u8", "u16", "s32"])
                .unwrap();
            match body.into_iter().next().unwrap() {
                Stmt::Expr(expr) => expr,
                other => panic!("{other:?}"),
            }
        };
        let spell = |text: &str| env.spell(&env.type_of(&expr(text)).unwrap());
        assert_eq!(spell("gUnits[index]->hp").as_deref(), Some("u16"));
        assert_eq!(spell("gUnits[index]->hp + small").as_deref(), Some("s32"));
        assert_eq!(spell("gUnits[1]").as_deref(), Some("struct Unit *"));
        assert_eq!(spell("*gUnits[1]->link").as_deref(), Some("s32"));
        assert_eq!(spell("Unit_Get(index) >> 1").as_deref(), Some("s32"));
        assert_eq!(spell("(u8)index").as_deref(), Some("u8"));
        assert_eq!(spell("0xFFFFFFFF").as_deref(), Some("unsigned int"));
        assert_eq!(env.type_of(&expr("Unknown(1)")), None);
        assert_eq!(
            env.return_type,
            Some(CType::Named("s32".into(), Box::new(INT)))
        );
    }
}
