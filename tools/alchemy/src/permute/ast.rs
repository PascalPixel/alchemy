//! The function body the permuter edits, and its printer.
//!
//! Types stay spelled as their tokens; `types` resolves them when a mutation
//! needs to know a width or a signedness.

#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub enum BinOp {
    Mul,
    Div,
    Mod,
    Add,
    Sub,
    Shl,
    Shr,
    Lt,
    Gt,
    Le,
    Ge,
    Eq,
    Ne,
    BitAnd,
    BitXor,
    BitOr,
    And,
    Or,
}

impl BinOp {
    pub fn text(self) -> &'static str {
        match self {
            BinOp::Mul => "*",
            BinOp::Div => "/",
            BinOp::Mod => "%",
            BinOp::Add => "+",
            BinOp::Sub => "-",
            BinOp::Shl => "<<",
            BinOp::Shr => ">>",
            BinOp::Lt => "<",
            BinOp::Gt => ">",
            BinOp::Le => "<=",
            BinOp::Ge => ">=",
            BinOp::Eq => "==",
            BinOp::Ne => "!=",
            BinOp::BitAnd => "&",
            BinOp::BitXor => "^",
            BinOp::BitOr => "|",
            BinOp::And => "&&",
            BinOp::Or => "||",
        }
    }

    pub fn from_text(text: &str) -> Option<BinOp> {
        Some(match text {
            "*" => BinOp::Mul,
            "/" => BinOp::Div,
            "%" => BinOp::Mod,
            "+" => BinOp::Add,
            "-" => BinOp::Sub,
            "<<" => BinOp::Shl,
            ">>" => BinOp::Shr,
            "<" => BinOp::Lt,
            ">" => BinOp::Gt,
            "<=" => BinOp::Le,
            ">=" => BinOp::Ge,
            "==" => BinOp::Eq,
            "!=" => BinOp::Ne,
            "&" => BinOp::BitAnd,
            "^" => BinOp::BitXor,
            "|" => BinOp::BitOr,
            "&&" => BinOp::And,
            "||" => BinOp::Or,
            _ => return None,
        })
    }

    pub fn precedence(self) -> u8 {
        match self {
            BinOp::Or => 4,
            BinOp::And => 5,
            BinOp::BitOr => 6,
            BinOp::BitXor => 7,
            BinOp::BitAnd => 8,
            BinOp::Eq | BinOp::Ne => 9,
            BinOp::Lt | BinOp::Gt | BinOp::Le | BinOp::Ge => 10,
            BinOp::Shl | BinOp::Shr => 11,
            BinOp::Add | BinOp::Sub => 12,
            BinOp::Mul | BinOp::Div | BinOp::Mod => 13,
        }
    }

    pub fn is_comparison(self) -> bool {
        matches!(
            self,
            BinOp::Lt | BinOp::Gt | BinOp::Le | BinOp::Ge | BinOp::Eq | BinOp::Ne
        )
    }

    fn is_bitwise_or_shift(self) -> bool {
        matches!(
            self,
            BinOp::BitAnd | BinOp::BitXor | BinOp::BitOr | BinOp::Shl | BinOp::Shr
        )
    }
}

#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub enum UnOp {
    Neg,
    Plus,
    Not,
    BitNot,
    Deref,
    AddrOf,
    PreInc,
    PreDec,
}

impl UnOp {
    fn text(self) -> &'static str {
        match self {
            UnOp::Neg => "-",
            UnOp::Plus => "+",
            UnOp::Not => "!",
            UnOp::BitNot => "~",
            UnOp::Deref => "*",
            UnOp::AddrOf => "&",
            UnOp::PreInc => "++",
            UnOp::PreDec => "--",
        }
    }
}

/// `None` is plain `=`; otherwise the compound operator.
pub type AssignOp = Option<BinOp>;

#[derive(Clone, Debug, PartialEq, Eq, Hash)]
pub struct TypeName {
    pub tokens: Vec<String>,
}

impl TypeName {
    pub fn text(&self) -> String {
        join_tokens(&self.tokens)
    }
}

#[derive(Clone, Debug, PartialEq, Eq, Hash)]
pub enum Expr {
    Ident(String),
    Number(String),
    Char(String),
    Str(Vec<String>),
    Unary(UnOp, Box<Expr>),
    PostInc(Box<Expr>),
    PostDec(Box<Expr>),
    Binary(BinOp, Box<Expr>, Box<Expr>),
    Assign(AssignOp, Box<Expr>, Box<Expr>),
    Cond(Box<Expr>, Box<Expr>, Box<Expr>),
    Comma(Box<Expr>, Box<Expr>),
    Call(Box<Expr>, Vec<Expr>),
    Index(Box<Expr>, Box<Expr>),
    Member(Box<Expr>, String, bool),
    Cast(TypeName, Box<Expr>),
    SizeofExpr(Box<Expr>),
    SizeofType(TypeName),
}

impl Expr {
    pub fn ident(name: &str) -> Expr {
        Expr::Ident(name.to_string())
    }

    pub fn binary(op: BinOp, left: Expr, right: Expr) -> Expr {
        Expr::Binary(op, Box::new(left), Box::new(right))
    }

    pub fn precedence(&self) -> u8 {
        match self {
            Expr::Comma(..) => 1,
            Expr::Assign(..) => 2,
            Expr::Cond(..) => 3,
            Expr::Binary(op, ..) => op.precedence(),
            Expr::Unary(..) | Expr::Cast(..) | Expr::SizeofExpr(..) | Expr::SizeofType(..) => 14,
            _ => 15,
        }
    }

    /// Every direct subexpression, in source order.
    pub fn children(&self) -> Vec<&Expr> {
        match self {
            Expr::Ident(_)
            | Expr::Number(_)
            | Expr::Char(_)
            | Expr::Str(_)
            | Expr::SizeofType(_) => Vec::new(),
            Expr::Unary(_, a)
            | Expr::PostInc(a)
            | Expr::PostDec(a)
            | Expr::Member(a, ..)
            | Expr::Cast(_, a)
            | Expr::SizeofExpr(a) => vec![a],
            Expr::Binary(_, a, b)
            | Expr::Assign(_, a, b)
            | Expr::Comma(a, b)
            | Expr::Index(a, b) => vec![a, b],
            Expr::Cond(a, b, c) => vec![a, b, c],
            Expr::Call(callee, arguments) => {
                let mut all = vec![callee.as_ref()];
                all.extend(arguments.iter());
                all
            }
        }
    }

    /// Calls `visit` on this expression and every subexpression.
    pub fn each(&self, visit: &mut dyn FnMut(&Expr)) {
        visit(self);
        for child in self.children() {
            child.each(visit);
        }
    }

    pub fn mentions(&self, name: &str) -> usize {
        let mut count = 0;
        self.each(&mut |expr| {
            if matches!(expr, Expr::Ident(ident) if ident == name) {
                count += 1;
            }
        });
        count
    }
}

#[derive(Clone, Debug, PartialEq, Eq, Hash)]
pub enum Init {
    Expr(Expr),
    List(Vec<Init>),
}

#[derive(Clone, Debug, PartialEq, Eq, Hash)]
pub struct Declarator {
    /// Tokens before the name: pointer stars, qualifiers, an opening paren.
    pub before: Vec<String>,
    pub name: String,
    /// Tokens after the name: array bounds, parameter lists, a closing paren.
    pub after: Vec<String>,
    pub init: Option<Init>,
}

#[derive(Clone, Debug, PartialEq, Eq, Hash)]
pub struct Decl {
    /// Storage class, qualifiers and type specifiers, in source order.
    pub specs: Vec<String>,
    pub items: Vec<Declarator>,
}

#[derive(Clone, Debug, PartialEq, Eq, Hash)]
pub enum Stmt {
    Expr(Expr),
    Decl(Decl),
    Block(Vec<Stmt>),
    If(Expr, Box<Stmt>, Option<Box<Stmt>>),
    While(Expr, Box<Stmt>),
    DoWhile(Box<Stmt>, Expr),
    For(Option<Expr>, Option<Expr>, Option<Expr>, Box<Stmt>),
    Switch(Expr, Box<Stmt>),
    Case(Expr),
    Default,
    Label(String),
    Break,
    Continue,
    Return(Option<Expr>),
    Goto(String),
    Empty,
    Comment(String),
}

impl Stmt {
    /// Calls `visit` on every statement nested in this one, this one first.
    pub fn each(&self, visit: &mut dyn FnMut(&Stmt)) {
        visit(self);
        match self {
            Stmt::Block(stmts) => stmts.iter().for_each(|stmt| stmt.each(visit)),
            Stmt::If(_, then, other) => {
                then.each(visit);
                if let Some(other) = other {
                    other.each(visit);
                }
            }
            Stmt::While(_, body)
            | Stmt::DoWhile(body, _)
            | Stmt::For(_, _, _, body)
            | Stmt::Switch(_, body) => body.each(visit),
            _ => {}
        }
    }

    /// The expressions this statement evaluates directly, not in nested statements.
    pub fn own_exprs(&self) -> Vec<&Expr> {
        match self {
            Stmt::Expr(expr)
            | Stmt::If(expr, ..)
            | Stmt::While(expr, _)
            | Stmt::DoWhile(_, expr)
            | Stmt::Switch(expr, _)
            | Stmt::Case(expr)
            | Stmt::Return(Some(expr)) => vec![expr],
            Stmt::For(init, cond, step, _) => [init, cond, step]
                .into_iter()
                .filter_map(Option::as_ref)
                .collect(),
            Stmt::Decl(decl) => {
                let mut all = Vec::new();
                for item in &decl.items {
                    if let Some(init) = &item.init {
                        init_exprs(init, &mut all);
                    }
                }
                all
            }
            _ => Vec::new(),
        }
    }

    /// Every expression in this statement and the statements nested in it.
    pub fn each_expr(&self, visit: &mut dyn FnMut(&Expr)) {
        self.each(&mut |stmt| {
            for expr in stmt.own_exprs() {
                expr.each(visit);
            }
        });
    }
}

fn init_exprs<'a>(init: &'a Init, all: &mut Vec<&'a Expr>) {
    match init {
        Init::Expr(expr) => all.push(expr),
        Init::List(items) => items.iter().for_each(|item| init_exprs(item, all)),
    }
}

/// A parsed function: its header as written, and its body.
#[derive(Clone, Debug, PartialEq, Eq, Hash)]
pub struct Function {
    pub name: String,
    pub header: String,
    pub params: Vec<Decl>,
    pub body: Vec<Stmt>,
}

impl Function {
    pub fn print(&self) -> String {
        let mut out = String::new();
        out.push_str(self.header.trim_end());
        out.push_str("\n{\n");
        print_block_items(&self.body, 1, false, &mut out);
        out.push_str("}\n");
        out
    }
}

pub fn join_tokens(tokens: &[String]) -> String {
    let mut out = String::new();
    for (index, token) in tokens.iter().enumerate() {
        if index > 0 {
            let previous = tokens[index - 1].as_str();
            let tight_after = matches!(previous, "(" | "[" | "*");
            let tight_before = matches!(token.as_str(), ")" | "]" | "," | "[");
            let call = token == "(" && previous == ")";
            if !(tight_after || tight_before || call) {
                out.push(' ');
            }
        }
        out.push_str(token);
    }
    out
}

pub fn print_expr(expr: &Expr) -> String {
    let mut out = String::new();
    write_expr(expr, 0, &mut out);
    out
}

fn write_paren(expr: &Expr, minimum: u8, out: &mut String) {
    if expr.precedence() < minimum {
        out.push('(');
        write_expr(expr, 0, out);
        out.push(')');
    } else {
        write_expr(expr, minimum, out);
    }
}

/// Operands whose grouping is legal without parentheses but easy to misread.
fn clarify(parent: BinOp, child: &Expr) -> bool {
    match child {
        Expr::Binary(op, ..) if *op != parent => {
            (parent.is_bitwise_or_shift() || op.is_bitwise_or_shift())
                || (parent == BinOp::Or && *op == BinOp::And)
        }
        _ => false,
    }
}

fn write_operand(parent: BinOp, child: &Expr, minimum: u8, out: &mut String) {
    if clarify(parent, child) {
        out.push('(');
        write_expr(child, 0, out);
        out.push(')');
    } else {
        write_paren(child, minimum, out);
    }
}

fn write_expr(expr: &Expr, _minimum: u8, out: &mut String) {
    match expr {
        Expr::Ident(name) | Expr::Number(name) | Expr::Char(name) => out.push_str(name),
        Expr::Str(parts) => out.push_str(&parts.join(" ")),
        Expr::Unary(op, operand) => {
            out.push_str(op.text());
            let doubled = matches!(
                (op, operand.as_ref()),
                (UnOp::Neg, Expr::Unary(UnOp::Neg | UnOp::PreDec, _))
                    | (UnOp::Plus, Expr::Unary(UnOp::Plus | UnOp::PreInc, _))
                    | (UnOp::AddrOf, Expr::Unary(UnOp::AddrOf, _))
            ) || matches!(operand.as_ref(), Expr::Number(text) if text.starts_with('-'));
            if doubled {
                out.push('(');
                write_expr(operand, 0, out);
                out.push(')');
            } else {
                write_paren(operand, 14, out);
            }
        }
        Expr::PostInc(operand) => {
            write_paren(operand, 15, out);
            out.push_str("++");
        }
        Expr::PostDec(operand) => {
            write_paren(operand, 15, out);
            out.push_str("--");
        }
        Expr::Binary(op, left, right) => {
            let precedence = op.precedence();
            write_operand(*op, left, precedence, out);
            out.push(' ');
            out.push_str(op.text());
            out.push(' ');
            write_operand(*op, right, precedence + 1, out);
        }
        Expr::Assign(op, target, value) => {
            write_paren(target, 14, out);
            out.push(' ');
            if let Some(op) = op {
                out.push_str(op.text());
            }
            out.push_str("= ");
            write_paren(value, 2, out);
        }
        Expr::Cond(cond, then, other) => {
            write_paren(cond, 4, out);
            out.push_str(" ? ");
            write_paren(then, 0, out);
            out.push_str(" : ");
            write_paren(other, 3, out);
        }
        Expr::Comma(left, right) => {
            write_paren(left, 1, out);
            out.push_str(", ");
            write_paren(right, 2, out);
        }
        Expr::Call(callee, arguments) => {
            write_paren(callee, 15, out);
            out.push('(');
            for (index, argument) in arguments.iter().enumerate() {
                if index > 0 {
                    out.push_str(", ");
                }
                write_paren(argument, 2, out);
            }
            out.push(')');
        }
        Expr::Index(base, index) => {
            write_paren(base, 15, out);
            out.push('[');
            write_expr(index, 0, out);
            out.push(']');
        }
        Expr::Member(base, member, arrow) => {
            write_paren(base, 15, out);
            out.push_str(if *arrow { "->" } else { "." });
            out.push_str(member);
        }
        Expr::Cast(name, operand) => {
            out.push('(');
            out.push_str(&name.text());
            out.push(')');
            write_paren(operand, 14, out);
        }
        Expr::SizeofExpr(operand) => {
            out.push_str("sizeof(");
            write_expr(operand, 0, out);
            out.push(')');
        }
        Expr::SizeofType(name) => {
            out.push_str("sizeof(");
            out.push_str(&name.text());
            out.push(')');
        }
    }
}

fn print_init(init: &Init) -> String {
    match init {
        Init::Expr(expr) => {
            let mut out = String::new();
            write_paren(expr, 2, &mut out);
            out
        }
        Init::List(items) => format!(
            "{{ {} }}",
            items.iter().map(print_init).collect::<Vec<_>>().join(", ")
        ),
    }
}

pub fn print_decl(decl: &Decl) -> String {
    let items: Vec<String> = decl
        .items
        .iter()
        .map(|item| {
            let mut text = join_tokens(&item.before);
            text.push_str(&item.name);
            text.push_str(&join_tokens(&item.after));
            if let Some(init) = &item.init {
                text.push_str(" = ");
                text.push_str(&print_init(init));
            }
            text
        })
        .collect();
    format!("{} {};", decl.specs.join(" "), items.join(", "))
}

fn indent(level: usize, out: &mut String) {
    for _ in 0..level {
        out.push_str("    ");
    }
}

fn print_block_items(stmts: &[Stmt], level: usize, switch_body: bool, out: &mut String) {
    // The function's own declarations end with a blank line.
    let declarations = stmts
        .iter()
        .position(|stmt| !matches!(stmt, Stmt::Decl(_) | Stmt::Comment(_)))
        .filter(|end| level == 1 && *end > 0 && matches!(stmts[end - 1], Stmt::Decl(_)));
    for (index, stmt) in stmts.iter().enumerate() {
        if Some(index) == declarations {
            out.push('\n');
        }
        match stmt {
            Stmt::Case(_) | Stmt::Default if switch_body => {
                print_stmt(stmt, level.saturating_sub(1), out)
            }
            Stmt::Label(_) => print_stmt(stmt, level.saturating_sub(1), out),
            _ => print_stmt(stmt, level, out),
        }
    }
}

/// A statement used as the body of `if`, `else` or a loop: braces stay on the
/// header line, a single statement goes on its own indented line.
fn print_body(body: &Stmt, level: usize, switch_body: bool, out: &mut String) {
    match body {
        Stmt::Block(stmts) => {
            out.push_str(" {\n");
            print_block_items(stmts, level + 1, switch_body, out);
            indent(level, out);
            out.push('}');
        }
        other => {
            out.push('\n');
            print_stmt(other, level + 1, out);
        }
    }
}

fn print_stmt(stmt: &Stmt, level: usize, out: &mut String) {
    indent(level, out);
    match stmt {
        Stmt::Expr(expr) => {
            out.push_str(&print_expr(expr));
            out.push_str(";\n");
        }
        Stmt::Decl(decl) => {
            out.push_str(&print_decl(decl));
            out.push('\n');
        }
        Stmt::Block(stmts) => {
            out.push_str("{\n");
            print_block_items(stmts, level + 1, false, out);
            indent(level, out);
            out.push_str("}\n");
        }
        Stmt::If(cond, then, other) => {
            out.push_str("if (");
            out.push_str(&print_expr(cond));
            out.push(')');
            print_body(then, level, false, out);
            if let Some(other) = other {
                if matches!(then.as_ref(), Stmt::Block(_)) {
                    out.push_str(" else");
                } else {
                    indent(level, out);
                    out.push_str("else");
                }
                if let Stmt::If(..) = other.as_ref() {
                    out.push(' ');
                    let mut nested = String::new();
                    print_stmt(other, level, &mut nested);
                    out.push_str(nested.trim_start());
                    return;
                }
                print_body(other, level, false, out);
            }
            if matches!(other.as_deref().unwrap_or(then.as_ref()), Stmt::Block(_)) {
                out.push('\n');
            }
        }
        Stmt::While(cond, body) => {
            out.push_str("while (");
            out.push_str(&print_expr(cond));
            out.push(')');
            if matches!(body.as_ref(), Stmt::Empty) {
                out.push_str(";\n");
                return;
            }
            print_body(body, level, false, out);
            if matches!(body.as_ref(), Stmt::Block(_)) {
                out.push('\n');
            }
        }
        Stmt::DoWhile(body, cond) => {
            out.push_str("do");
            print_body(body, level, false, out);
            if matches!(body.as_ref(), Stmt::Block(_)) {
                out.push(' ');
            } else {
                indent(level, out);
            }
            out.push_str("while (");
            out.push_str(&print_expr(cond));
            out.push_str(");\n");
        }
        Stmt::For(init, cond, step, body) => {
            out.push_str("for (");
            if let Some(init) = init {
                out.push_str(&print_expr(init));
            }
            out.push(';');
            if let Some(cond) = cond {
                out.push(' ');
                out.push_str(&print_expr(cond));
            }
            out.push(';');
            if let Some(step) = step {
                out.push(' ');
                out.push_str(&print_expr(step));
            }
            out.push(')');
            if matches!(body.as_ref(), Stmt::Empty) {
                out.push_str(";\n");
                return;
            }
            print_body(body, level, false, out);
            if matches!(body.as_ref(), Stmt::Block(_)) {
                out.push('\n');
            }
        }
        Stmt::Switch(expr, body) => {
            out.push_str("switch (");
            out.push_str(&print_expr(expr));
            out.push(')');
            print_body(body, level, true, out);
            if matches!(body.as_ref(), Stmt::Block(_)) {
                out.push('\n');
            }
        }
        Stmt::Case(expr) => {
            out.push_str("case ");
            out.push_str(&print_expr(expr));
            out.push_str(":\n");
        }
        Stmt::Default => out.push_str("default:\n"),
        Stmt::Label(name) => {
            out.push_str(name);
            out.push_str(":\n");
        }
        Stmt::Break => out.push_str("break;\n"),
        Stmt::Continue => out.push_str("continue;\n"),
        Stmt::Return(None) => out.push_str("return;\n"),
        Stmt::Return(Some(expr)) => {
            out.push_str("return ");
            out.push_str(&print_expr(expr));
            out.push_str(";\n");
        }
        Stmt::Goto(label) => {
            out.push_str("goto ");
            out.push_str(label);
            out.push_str(";\n");
        }
        Stmt::Empty => out.push_str(";\n"),
        Stmt::Comment(text) => {
            let mut lines = text.lines();
            if let Some(first) = lines.next() {
                out.push_str(first.trim());
            }
            for line in lines {
                out.push('\n');
                indent(level, out);
                out.push_str("   ");
                out.push_str(line.trim());
            }
            out.push('\n');
        }
    }
}
