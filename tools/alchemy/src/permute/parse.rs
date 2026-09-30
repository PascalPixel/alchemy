//! Parse the function to permute from the draft as written, and the
//! declarations the preprocessed translation unit makes visible to it.

use super::ast::*;
use super::lex::{lex, Tok, Token};
use std::collections::{BTreeMap, BTreeSet};

pub type Result<T> = std::result::Result<T, String>;

/// Words that start a declaration or a type name.
const SPEC_WORDS: &[&str] = &[
    "void",
    "char",
    "short",
    "int",
    "long",
    "float",
    "double",
    "signed",
    "unsigned",
    "__signed__",
    "struct",
    "union",
    "enum",
    "const",
    "__const",
    "volatile",
    "__volatile__",
    "__volatile",
    "register",
    "static",
    "extern",
    "auto",
    "typedef",
    "inline",
    "__inline__",
    "__inline",
];

const QUALIFIERS: &[&str] = &["const", "__const", "volatile", "__volatile__", "__volatile"];

/// A struct or union, or the constants of an enum, seen while scanning.
#[derive(Clone, Debug, Default)]
pub struct Unit {
    pub typedefs: BTreeMap<String, (Vec<String>, Declarator)>,
    pub records: BTreeMap<String, Vec<(Vec<String>, Declarator)>>,
    pub globals: BTreeMap<String, (Vec<String>, Declarator)>,
    pub enum_constants: BTreeSet<String>,
}

impl Unit {
    pub fn typedef_names(&self) -> BTreeSet<String> {
        self.typedefs.keys().cloned().collect()
    }
}

pub struct Parser<'a> {
    tokens: Vec<Token>,
    position: usize,
    typedefs: &'a mut BTreeSet<String>,
    comments: Vec<String>,
    trailing: Vec<String>,
    anonymous: usize,
    pub unit: Unit,
}

impl<'a> Parser<'a> {
    pub fn new(tokens: Vec<Token>, typedefs: &'a mut BTreeSet<String>) -> Self {
        Parser {
            tokens,
            position: 0,
            typedefs,
            comments: Vec::new(),
            trailing: Vec::new(),
            anonymous: 0,
            unit: Unit::default(),
        }
    }

    fn skip_comments(&mut self) {
        while let Some(Token {
            tok: Tok::Comment(text),
            ..
        }) = self.tokens.get(self.position)
        {
            self.comments.push(text.clone());
            self.position += 1;
        }
    }

    fn peek(&mut self) -> Option<&Tok> {
        self.skip_comments();
        self.tokens.get(self.position).map(|token| &token.tok)
    }

    fn peek_at(&mut self, ahead: usize) -> Option<&Tok> {
        self.skip_comments();
        self.tokens[self.position..]
            .iter()
            .filter(|token| !matches!(token.tok, Tok::Comment(_)))
            .nth(ahead)
            .map(|token| &token.tok)
    }

    fn line(&self) -> usize {
        self.tokens
            .get(self.position)
            .or(self.tokens.last())
            .map_or(0, |token| token.line)
    }

    fn error<T>(&self, message: &str) -> Result<T> {
        Err(format!("line {}: {message}", self.line()))
    }

    fn next(&mut self) -> Result<Tok> {
        self.skip_comments();
        let token = self
            .tokens
            .get(self.position)
            .map(|token| token.tok.clone())
            .ok_or_else(|| "unexpected end of input".to_string())?;
        self.position += 1;
        Ok(token)
    }

    fn is_punct(&mut self, punct: &str) -> bool {
        matches!(self.peek(), Some(Tok::Punct(p)) if *p == punct)
    }

    fn is_word(&mut self, word: &str) -> bool {
        matches!(self.peek(), Some(Tok::Ident(w)) if w == word)
    }

    fn eat(&mut self, punct: &str) -> bool {
        if self.is_punct(punct) {
            self.position += 1;
            true
        } else {
            false
        }
    }

    fn expect(&mut self, punct: &str) -> Result<()> {
        if self.eat(punct) {
            Ok(())
        } else {
            let found = self.peek_owned();
            self.error(&format!("expected {punct:?}, found {found:?}"))
        }
    }

    fn peek_owned(&mut self) -> Option<Tok> {
        self.peek().cloned()
    }

    fn ident(&mut self) -> Result<String> {
        match self.next()? {
            Tok::Ident(name) => Ok(name),
            other => self.error(&format!("expected an identifier, found {other:?}")),
        }
    }

    fn take_comments(&mut self) -> Vec<Stmt> {
        std::mem::take(&mut self.comments)
            .into_iter()
            .map(Stmt::Comment)
            .collect()
    }

    fn starts_type(&mut self, ahead: usize) -> bool {
        let typedefs = self.typedefs.clone();
        match self.peek_at(ahead) {
            Some(Tok::Ident(word)) => {
                (SPEC_WORDS.contains(&word.as_str()) && word != "typedef")
                    || typedefs.contains(word)
            }
            _ => false,
        }
    }

    fn starts_declaration(&mut self) -> bool {
        let typedefs = self.typedefs.clone();
        match self.peek_at(0).cloned() {
            Some(Tok::Ident(word)) if SPEC_WORDS.contains(&word.as_str()) => true,
            Some(Tok::Ident(word)) if typedefs.contains(&word) => !matches!(
                self.peek_at(1),
                Some(Tok::Punct(p)) if !matches!(*p, "*" | "(")
            ),
            _ => false,
        }
    }

    fn skip_attribute(&mut self) -> Result<()> {
        self.next()?;
        self.expect("(")?;
        let mut depth = 1;
        while depth > 0 {
            match self.next()? {
                Tok::Punct("(") => depth += 1,
                Tok::Punct(")") => depth -= 1,
                _ => {}
            }
        }
        Ok(())
    }

    /// Storage class, qualifiers and type specifiers. A struct, union or enum
    /// body is recorded in `unit` and replaced by its tag.
    fn specifiers(&mut self) -> Result<Vec<String>> {
        let mut specs = Vec::new();
        let mut typed = false;
        loop {
            let word = match self.peek() {
                Some(Tok::Ident(word)) => word.clone(),
                _ => break,
            };
            if word == "__attribute__" {
                self.skip_attribute()?;
                continue;
            }
            if matches!(word.as_str(), "struct" | "union" | "enum") {
                self.next()?;
                specs.push(word.clone());
                let tag = if let Some(Tok::Ident(tag)) = self.peek() {
                    let tag = tag.clone();
                    self.next()?;
                    tag
                } else {
                    self.anonymous += 1;
                    format!("__anonymous_{}", self.anonymous)
                };
                if self.is_punct("{") {
                    if word == "enum" {
                        self.enum_body()?;
                    } else {
                        let members = self.record_body()?;
                        self.unit.records.insert(tag.clone(), members);
                    }
                }
                specs.push(tag);
                typed = true;
                continue;
            }
            if SPEC_WORDS.contains(&word.as_str()) {
                if !QUALIFIERS.contains(&word.as_str())
                    && !matches!(
                        word.as_str(),
                        "register"
                            | "static"
                            | "extern"
                            | "auto"
                            | "typedef"
                            | "inline"
                            | "__inline__"
                            | "__inline"
                    )
                {
                    typed = true;
                }
                self.next()?;
                specs.push(word);
                continue;
            }
            if !typed && self.typedefs.contains(&word) {
                self.next()?;
                specs.push(word);
                typed = true;
                continue;
            }
            break;
        }
        Ok(specs)
    }

    fn enum_body(&mut self) -> Result<()> {
        self.expect("{")?;
        while !self.eat("}") {
            let name = self.ident()?;
            self.unit.enum_constants.insert(name);
            if self.eat("=") {
                self.conditional()?;
            }
            if !self.eat(",") {
                self.expect("}")?;
                break;
            }
        }
        Ok(())
    }

    fn record_body(&mut self) -> Result<Vec<(Vec<String>, Declarator)>> {
        self.expect("{")?;
        let mut members = Vec::new();
        while !self.eat("}") {
            let specs = self.specifiers()?;
            if specs.is_empty() {
                return self.error("expected a member declaration");
            }
            loop {
                if self.is_punct(":") {
                    // An unnamed bit-field.
                    self.next()?;
                    self.conditional()?;
                } else if !self.is_punct(";") {
                    let item = self.declarator(false)?;
                    if self.eat(":") {
                        self.conditional()?;
                    }
                    members.push((specs.clone(), item));
                }
                if !self.eat(",") {
                    break;
                }
            }
            self.expect(";")?;
        }
        Ok(members)
    }

    /// Declarator tokens up to `,`, `;`, `=`, `:` or a function body; the
    /// name is the first identifier that is not a qualifier and not inside a
    /// parameter list, so `(*)(s32)` in a cast stays abstract.
    fn declarator(&mut self, abstract_allowed: bool) -> Result<Declarator> {
        let mut before = Vec::new();
        let mut after = Vec::new();
        let mut name = None;
        let mut depth = 0usize;
        // For each open bracket, whether it opens a parameter list: a `(`
        // after a name or a closing bracket.
        let mut parameters: Vec<bool> = Vec::new();
        let mut previous: Option<Tok> = None;
        loop {
            let tok = match self.peek() {
                Some(tok) => tok.clone(),
                None => break,
            };
            match &tok {
                Tok::Punct(p) if depth == 0 && matches!(*p, "," | ";" | "=" | ":" | "{") => break,
                Tok::Punct(")") if depth == 0 => break,
                Tok::Punct(p) if matches!(*p, "(" | "[") => {
                    depth += 1;
                    parameters.push(
                        *p == "("
                            && matches!(
                                &previous,
                                Some(Tok::Ident(_)) | Some(Tok::Punct(")")) | Some(Tok::Punct("]"))
                            ),
                    );
                }
                Tok::Punct(p) if matches!(*p, ")" | "]") => {
                    depth -= 1;
                    parameters.pop();
                }
                _ => {}
            }
            self.next()?;
            let text = token_text(&tok);
            if text == "__attribute__" {
                self.position -= 1;
                self.skip_attribute()?;
                continue;
            }
            previous = Some(tok.clone());
            let in_parameters = parameters.iter().any(|&list| list);
            match (&tok, &name) {
                (Tok::Ident(word), None)
                    if !QUALIFIERS.contains(&word.as_str()) && !in_parameters =>
                {
                    name = Some(word.clone())
                }
                (_, None) => before.push(text),
                (_, Some(_)) => after.push(text),
            }
        }
        match name {
            Some(name) => Ok(Declarator {
                before,
                name,
                after,
                init: None,
            }),
            None if abstract_allowed => Ok(Declarator {
                before,
                name: String::new(),
                after,
                init: None,
            }),
            None => self.error("expected a declarator"),
        }
    }

    fn initializer(&mut self) -> Result<Init> {
        if self.eat("{") {
            let mut items = Vec::new();
            while !self.eat("}") {
                items.push(self.initializer()?);
                if !self.eat(",") {
                    self.expect("}")?;
                    break;
                }
            }
            Ok(Init::List(items))
        } else {
            Ok(Init::Expr(self.assignment()?))
        }
    }

    fn declaration(&mut self) -> Result<Decl> {
        let specs = self.specifiers()?;
        let typedef = specs.iter().any(|spec| spec == "typedef");
        let mut items = Vec::new();
        if !self.is_punct(";") {
            loop {
                let mut item = self.declarator(false)?;
                if self.eat("=") {
                    item.init = Some(self.initializer()?);
                }
                if typedef {
                    self.typedefs.insert(item.name.clone());
                }
                items.push(item);
                if !self.eat(",") {
                    break;
                }
            }
        }
        self.expect(";")?;
        Ok(Decl { specs, items })
    }

    fn type_name(&mut self) -> Result<TypeName> {
        let mut tokens = self.specifiers()?;
        let declarator = self.declarator(true)?;
        if !declarator.name.is_empty() {
            return self.error("a type name has no identifier");
        }
        tokens.extend(declarator.before);
        tokens.extend(declarator.after);
        Ok(TypeName { tokens })
    }

    // Statements.

    pub fn block_items(&mut self) -> Result<Vec<Stmt>> {
        let mut items = Vec::new();
        loop {
            self.skip_comments();
            items.extend(self.take_comments());
            if self.is_punct("}") || self.peek().is_none() {
                break;
            }
            let stmt = self.statement()?;
            items.extend(self.take_comments());
            items.push(stmt);
            items.extend(
                std::mem::take(&mut self.trailing)
                    .into_iter()
                    .map(Stmt::Comment),
            );
        }
        Ok(items)
    }

    fn body(&mut self) -> Result<Stmt> {
        self.statement()
    }

    pub fn statement(&mut self) -> Result<Stmt> {
        let tok = self
            .peek_owned()
            .ok_or_else(|| "unexpected end of function".to_string())?;
        match &tok {
            Tok::Punct("{") => {
                self.next()?;
                let items = self.block_items()?;
                self.expect("}")?;
                return Ok(Stmt::Block(items));
            }
            Tok::Punct(";") => {
                self.next()?;
                return Ok(Stmt::Empty);
            }
            Tok::Ident(word) => {
                if matches!(self.peek_at(1), Some(Tok::Punct(":")))
                    && !matches!(word.as_str(), "default" | "case")
                    && !self.typedefs.contains(word)
                {
                    self.next()?;
                    self.next()?;
                    return Ok(Stmt::Label(word.clone()));
                }
                match word.as_str() {
                    "if" => {
                        self.next()?;
                        self.expect("(")?;
                        let cond = self.expression()?;
                        self.expect(")")?;
                        let then = self.body()?;
                        let before = self.comments.len();
                        let other = if self.is_word("else") {
                            self.next()?;
                            Some(Box::new(self.body()?))
                        } else {
                            // Comments after a lone `if` belong after it.
                            let after = self.comments.split_off(before);
                            self.trailing.splice(0..0, after);
                            None
                        };
                        return Ok(Stmt::If(cond, Box::new(then), other));
                    }
                    "while" => {
                        self.next()?;
                        self.expect("(")?;
                        let cond = self.expression()?;
                        self.expect(")")?;
                        return Ok(Stmt::While(cond, Box::new(self.body()?)));
                    }
                    "do" => {
                        self.next()?;
                        let body = self.body()?;
                        if !self.is_word("while") {
                            return self.error("expected while after do");
                        }
                        self.next()?;
                        self.expect("(")?;
                        let cond = self.expression()?;
                        self.expect(")")?;
                        self.expect(";")?;
                        return Ok(Stmt::DoWhile(Box::new(body), cond));
                    }
                    "for" => {
                        self.next()?;
                        self.expect("(")?;
                        let init = if self.is_punct(";") {
                            None
                        } else {
                            Some(self.expression()?)
                        };
                        self.expect(";")?;
                        let cond = if self.is_punct(";") {
                            None
                        } else {
                            Some(self.expression()?)
                        };
                        self.expect(";")?;
                        let step = if self.is_punct(")") {
                            None
                        } else {
                            Some(self.expression()?)
                        };
                        self.expect(")")?;
                        return Ok(Stmt::For(init, cond, step, Box::new(self.body()?)));
                    }
                    "switch" => {
                        self.next()?;
                        self.expect("(")?;
                        let expr = self.expression()?;
                        self.expect(")")?;
                        return Ok(Stmt::Switch(expr, Box::new(self.body()?)));
                    }
                    "case" => {
                        self.next()?;
                        let expr = self.conditional()?;
                        self.expect(":")?;
                        return Ok(Stmt::Case(expr));
                    }
                    "default" => {
                        self.next()?;
                        self.expect(":")?;
                        return Ok(Stmt::Default);
                    }
                    "break" => {
                        self.next()?;
                        self.expect(";")?;
                        return Ok(Stmt::Break);
                    }
                    "continue" => {
                        self.next()?;
                        self.expect(";")?;
                        return Ok(Stmt::Continue);
                    }
                    "return" => {
                        self.next()?;
                        let value = if self.is_punct(";") {
                            None
                        } else {
                            Some(self.expression()?)
                        };
                        self.expect(";")?;
                        return Ok(Stmt::Return(value));
                    }
                    "goto" => {
                        self.next()?;
                        let label = self.ident()?;
                        self.expect(";")?;
                        return Ok(Stmt::Goto(label));
                    }
                    "asm" | "__asm__" | "__asm" => {
                        return self.error("inline assembly is not permuted");
                    }
                    _ => {}
                }
                if self.starts_declaration() {
                    return Ok(Stmt::Decl(self.declaration()?));
                }
            }
            _ => {}
        }
        let expr = self.expression()?;
        self.expect(";")?;
        Ok(Stmt::Expr(expr))
    }

    // Expressions.

    pub fn expression(&mut self) -> Result<Expr> {
        let mut expr = self.assignment()?;
        while self.eat(",") {
            let right = self.assignment()?;
            expr = Expr::Comma(Box::new(expr), Box::new(right));
        }
        Ok(expr)
    }

    fn assignment(&mut self) -> Result<Expr> {
        let target = self.conditional()?;
        let op = match self.peek() {
            Some(Tok::Punct(p)) => match *p {
                "=" => Some(None),
                "+=" | "-=" | "*=" | "/=" | "%=" | "<<=" | ">>=" | "&=" | "^=" | "|=" => {
                    Some(BinOp::from_text(&p[..p.len() - 1]))
                }
                _ => None,
            },
            _ => None,
        };
        match op {
            Some(op) => {
                self.next()?;
                let value = self.assignment()?;
                Ok(Expr::Assign(op, Box::new(target), Box::new(value)))
            }
            None => Ok(target),
        }
    }

    fn conditional(&mut self) -> Result<Expr> {
        let cond = self.binary(4)?;
        if self.eat("?") {
            let then = self.expression()?;
            self.expect(":")?;
            let other = self.conditional()?;
            return Ok(Expr::Cond(Box::new(cond), Box::new(then), Box::new(other)));
        }
        Ok(cond)
    }

    fn binary(&mut self, minimum: u8) -> Result<Expr> {
        let mut left = self.cast()?;
        loop {
            let op = match self.peek() {
                Some(Tok::Punct(p)) => BinOp::from_text(p),
                _ => None,
            };
            let Some(op) = op else { break };
            if op.precedence() < minimum {
                break;
            }
            self.next()?;
            let right = self.binary(op.precedence() + 1)?;
            left = Expr::binary(op, left, right);
        }
        Ok(left)
    }

    fn cast(&mut self) -> Result<Expr> {
        if self.is_punct("(") && self.starts_type(1) {
            self.next()?;
            let name = self.type_name()?;
            self.expect(")")?;
            let operand = self.cast()?;
            return Ok(Expr::Cast(name, Box::new(operand)));
        }
        self.unary()
    }

    fn unary(&mut self) -> Result<Expr> {
        let op = match self.peek() {
            Some(Tok::Punct(p)) => match *p {
                "-" => Some(UnOp::Neg),
                "+" => Some(UnOp::Plus),
                "!" => Some(UnOp::Not),
                "~" => Some(UnOp::BitNot),
                "*" => Some(UnOp::Deref),
                "&" => Some(UnOp::AddrOf),
                "++" => Some(UnOp::PreInc),
                "--" => Some(UnOp::PreDec),
                _ => None,
            },
            _ => None,
        };
        if let Some(op) = op {
            self.next()?;
            let operand = if matches!(op, UnOp::PreInc | UnOp::PreDec) {
                self.unary()?
            } else {
                self.cast()?
            };
            return Ok(Expr::Unary(op, Box::new(operand)));
        }
        if self.is_word("sizeof") {
            self.next()?;
            if self.is_punct("(") && self.starts_type(1) {
                self.next()?;
                let name = self.type_name()?;
                self.expect(")")?;
                return Ok(Expr::SizeofType(name));
            }
            return Ok(Expr::SizeofExpr(Box::new(self.unary()?)));
        }
        self.postfix()
    }

    fn postfix(&mut self) -> Result<Expr> {
        let mut expr = self.primary()?;
        loop {
            if self.eat("[") {
                let index = self.expression()?;
                self.expect("]")?;
                expr = Expr::Index(Box::new(expr), Box::new(index));
            } else if self.eat("(") {
                let mut arguments = Vec::new();
                if !self.eat(")") {
                    loop {
                        arguments.push(self.assignment()?);
                        if self.eat(")") {
                            break;
                        }
                        self.expect(",")?;
                    }
                }
                expr = Expr::Call(Box::new(expr), arguments);
            } else if self.eat(".") {
                expr = Expr::Member(Box::new(expr), self.ident()?, false);
            } else if self.eat("->") {
                expr = Expr::Member(Box::new(expr), self.ident()?, true);
            } else if self.eat("++") {
                expr = Expr::PostInc(Box::new(expr));
            } else if self.eat("--") {
                expr = Expr::PostDec(Box::new(expr));
            } else {
                break;
            }
        }
        Ok(expr)
    }

    fn primary(&mut self) -> Result<Expr> {
        match self.next()? {
            Tok::Ident(name) => Ok(Expr::Ident(name)),
            Tok::Number(text) => Ok(Expr::Number(text)),
            Tok::Char(text) => Ok(Expr::Char(text)),
            Tok::Str(text) => {
                let mut parts = vec![text];
                while let Some(Tok::Str(next)) = self.peek() {
                    parts.push(next.clone());
                    self.next()?;
                }
                Ok(Expr::Str(parts))
            }
            Tok::Punct("(") => {
                if self.is_punct("{") {
                    return self.error("statement expressions are not permuted");
                }
                let expr = self.expression()?;
                self.expect(")")?;
                Ok(expr)
            }
            other => self.error(&format!("unexpected {other:?}")),
        }
    }

    // Translation units.

    /// Record every typedef, record, enum constant and global declaration,
    /// skipping function bodies and anything this scanner cannot read.
    pub fn scan_unit(&mut self) {
        while self.peek().is_some() {
            let start = self.position;
            if self.scan_external().is_err() {
                self.position = start.max(self.position);
                self.recover();
            }
            self.comments.clear();
        }
    }

    fn recover(&mut self) {
        let mut depth = 0i32;
        while let Ok(tok) = self.next() {
            match tok {
                Tok::Punct("{") => depth += 1,
                Tok::Punct("}") => {
                    depth -= 1;
                    if depth <= 0 {
                        if self.is_punct(";") {
                            self.position += 1;
                        }
                        return;
                    }
                }
                Tok::Punct(";") if depth <= 0 => return,
                _ => {}
            }
        }
    }

    fn scan_external(&mut self) -> Result<()> {
        if self.eat(";") {
            return Ok(());
        }
        let specs = self.specifiers()?;
        if specs.is_empty() {
            return self.error("expected a declaration");
        }
        let typedef = specs.iter().any(|spec| spec == "typedef");
        if self.eat(";") {
            return Ok(());
        }
        loop {
            let item = self.declarator(false)?;
            if self.is_punct("{") {
                self.skip_braces()?;
                self.unit.globals.insert(item.name.clone(), (specs, item));
                return Ok(());
            }
            if self.eat("=") {
                self.initializer()?;
            }
            if typedef {
                self.typedefs.insert(item.name.clone());
                self.unit
                    .typedefs
                    .insert(item.name.clone(), (specs.clone(), item));
            } else {
                self.unit
                    .globals
                    .insert(item.name.clone(), (specs.clone(), item));
            }
            if !self.eat(",") {
                break;
            }
        }
        self.expect(";")
    }

    fn skip_braces(&mut self) -> Result<()> {
        self.expect("{")?;
        let mut depth = 1;
        while depth > 0 {
            match self.next()? {
                Tok::Punct("{") => depth += 1,
                Tok::Punct("}") => depth -= 1,
                _ => {}
            }
        }
        Ok(())
    }
}

pub fn token_text(tok: &Tok) -> String {
    match tok {
        Tok::Ident(text)
        | Tok::Number(text)
        | Tok::Char(text)
        | Tok::Str(text)
        | Tok::Comment(text) => text.clone(),
        Tok::Punct(p) => p.to_string(),
    }
}

/// Scan a preprocessed translation unit for the declarations it makes.
pub fn scan_unit(preprocessed: &str) -> Result<Unit> {
    let tokens = lex(preprocessed)?;
    let mut typedefs = BTreeSet::new();
    let mut parser = Parser::new(tokens, &mut typedefs);
    parser.scan_unit();
    Ok(parser.unit)
}

/// Where the function's definition sits in the draft's text.
pub struct Located {
    /// Byte range of the whole definition, from its first specifier to `}`.
    pub start: usize,
    pub end: usize,
    pub function: Function,
}

/// The functions a source defines, in order, each with whether it is
/// declared inline.
pub fn definitions(source: &str) -> Result<Vec<(String, bool)>> {
    let tokens = lex(source)?;
    Ok(scan_definitions(&tokens)
        .into_iter()
        .map(|(name, boundary, name_index, _, _)| {
            let inline = tokens[boundary..name_index].iter().any(|token| {
                matches!(&token.tok, Tok::Ident(word) if word == "inline" || word == "__inline" || word == "__inline__")
            });
            (name, inline)
        })
        .collect())
}

/// Find and parse the definition of `name`; `None` picks the draft's only
/// function definition.
pub fn locate(source: &str, name: Option<&str>, typedefs: &BTreeSet<String>) -> Result<Located> {
    let tokens = lex(source)?;
    let mut found = scan_definitions(&tokens);
    let (ident, boundary, name_index, close, open) = match name {
        Some(name) => found
            .into_iter()
            .find(|entry| entry.0 == name)
            .ok_or_else(|| format!("the draft does not define {name}"))?,
        None => match found.len() {
            1 => found.pop().expect("one definition"),
            0 => return Err("the draft defines no function".into()),
            _ => {
                return Err(format!(
                    "the draft defines {}; choose one with --function",
                    found
                        .iter()
                        .map(|entry| entry.0.as_str())
                        .collect::<Vec<_>>()
                        .join(", ")
                ))
            }
        },
    };
    locate_at(
        source, &tokens, typedefs, ident, boundary, name_index, close, open,
    )
}

/// Every top-level function definition: name, the token index where its
/// declaration starts, its name, its parameter list's close and its body's
/// open brace.
fn scan_definitions(tokens: &[Token]) -> Vec<(String, usize, usize, usize, usize)> {
    let mut depth = 0usize;
    let mut boundary = 0usize;
    let mut found = Vec::new();
    let mut index = 0;
    while index < tokens.len() {
        match &tokens[index].tok {
            Tok::Punct("{") => depth += 1,
            Tok::Punct("}") => {
                depth = depth.saturating_sub(1);
                if depth == 0 {
                    boundary = index + 1;
                }
            }
            Tok::Punct(";") if depth == 0 => boundary = index + 1,
            Tok::Ident(ident) if depth == 0 => {
                if let Some(Tok::Punct("(")) = tokens.get(index + 1).map(|token| &token.tok) {
                    if let Some(close) = matching(&tokens, index + 1) {
                        let open = tokens[close + 1..]
                            .iter()
                            .position(|token| !matches!(token.tok, Tok::Comment(_)))
                            .map(|at| close + 1 + at);
                        if let Some(open) = open.filter(|open| {
                            matches!(tokens[*open].tok, Tok::Punct("{"))
                                && !tokens[boundary..index]
                                    .iter()
                                    .any(|token| matches!(token.tok, Tok::Punct("=")))
                        }) {
                            found.push((ident.clone(), boundary, index, close, open));
                        }
                    }
                }
            }
            _ => {}
        }
        index += 1;
    }
    found
}

/// Parse the definition `scan_definitions` found at these token indices.
#[allow(clippy::too_many_arguments)]
fn locate_at(
    source: &str,
    tokens: &[Token],
    typedefs: &BTreeSet<String>,
    ident: String,
    boundary: usize,
    name_index: usize,
    close: usize,
    open: usize,
) -> Result<Located> {
    let first = tokens[boundary..name_index]
        .iter()
        .position(|token| !matches!(token.tok, Tok::Comment(_)))
        .map_or(name_index, |at| boundary + at);
    let header = source[tokens[first].start..tokens[open].start].to_string();
    let close_body = matching(&tokens, open).ok_or("unterminated function body")?;
    let mut names = typedefs.clone();
    let params = parameters(&tokens[name_index + 2..close], &mut names)?;
    let mut parser = Parser::new(tokens[open + 1..close_body].to_vec(), &mut names);
    let body = parser.block_items()?;
    if parser.peek().is_some() {
        return parser.error("unexpected text after the function body");
    }
    Ok(Located {
        start: tokens[first].start,
        end: tokens[close_body].end,
        function: Function {
            name: ident,
            header,
            params,
            body,
        },
    })
}

fn parameters(tokens: &[Token], typedefs: &mut BTreeSet<String>) -> Result<Vec<Decl>> {
    let tokens: Vec<Token> = tokens
        .iter()
        .filter(|token| !matches!(token.tok, Tok::Comment(_)))
        .cloned()
        .collect();
    if tokens.is_empty()
        || (tokens.len() == 1 && matches!(&tokens[0].tok, Tok::Ident(word) if word == "void"))
    {
        return Ok(Vec::new());
    }
    let mut parser = Parser::new(tokens, typedefs);
    let mut params = Vec::new();
    loop {
        let specs = parser.specifiers()?;
        let item = parser.declarator(true)?;
        params.push(Decl {
            specs,
            items: vec![item],
        });
        if !parser.eat(",") {
            break;
        }
    }
    if parser.peek().is_some() {
        return parser.error("unreadable parameter list");
    }
    Ok(params)
}

fn matching(tokens: &[Token], open: usize) -> Option<usize> {
    let (opening, closing) = match tokens[open].tok {
        Tok::Punct("(") => ("(", ")"),
        Tok::Punct("{") => ("{", "}"),
        Tok::Punct("[") => ("[", "]"),
        _ => return None,
    };
    let mut depth = 0usize;
    for (index, token) in tokens.iter().enumerate().skip(open) {
        match &token.tok {
            Tok::Punct(p) if *p == opening => depth += 1,
            Tok::Punct(p) if *p == closing => {
                depth -= 1;
                if depth == 0 {
                    return Some(index);
                }
            }
            _ => {}
        }
    }
    None
}

/// Parse a statement list, for tests and for mutations that build code.
#[cfg(test)]
pub fn parse_body(source: &str, typedefs: &[&str]) -> Result<Vec<Stmt>> {
    let mut names: BTreeSet<String> = typedefs.iter().map(|name| name.to_string()).collect();
    let mut parser = Parser::new(lex(source)?, &mut names);
    let body = parser.block_items()?;
    if parser.peek().is_some() {
        return parser.error("unexpected text");
    }
    Ok(body)
}

#[cfg(test)]
mod tests {
    use super::*;

    const DRAFT: &str = "#include \"TYPES.H\"\n/* header */\nextern u16 gTable[];\n\ns32 Find_Slot(s32 key)\n{\n    s32 index;\n    int entry;\n    for (index = 0; ; index += 1) {\n        entry = gTable[index];\n        /* note */\n        if (key == (entry & 0x1FF))\n            return *(u16 *)((u8 *)gTable + index * 2) >> 9;\n        if (((s16)entry) == -1)\n            break;\n    }\n    return 6;\n}\n";

    #[test]
    fn locates_parses_and_reprints_a_function() {
        let typedefs: BTreeSet<String> = ["u8", "u16", "s16", "s32"]
            .iter()
            .map(|name| name.to_string())
            .collect();
        let located = locate(DRAFT, None, &typedefs).unwrap();
        assert_eq!(located.function.name, "Find_Slot");
        assert_eq!(located.function.header.trim(), "s32 Find_Slot(s32 key)");
        assert_eq!(&DRAFT[located.end..], "\n");
        assert_eq!(located.function.params.len(), 1);
        let printed = located.function.print();
        assert_eq!(
            printed,
            "s32 Find_Slot(s32 key)\n{\n    s32 index;\n    int entry;\n\n    for (index = 0;; index += 1) {\n        entry = gTable[index];\n        /* note */\n        if (key == (entry & 0x1FF))\n            return *(u16 *)((u8 *)gTable + index * 2) >> 9;\n        if ((s16)entry == -1)\n            break;\n    }\n    return 6;\n}\n"
        );
        let again = locate(&printed, None, &typedefs).unwrap();
        assert_eq!(again.function.body, located.function.body);
    }

    #[test]
    fn scans_typedefs_records_and_prototypes() {
        let unit = scan_unit(
            "typedef unsigned short u16;\ntypedef struct { u16 a; u16 b : 3; } Pair;\nstruct Node { struct Node *next; u16 value[4]; };\nenum { KIND_A, KIND_B = 3 };\nextern struct Node *gHead;\nu16 Get(int);\nstatic int Helper(int x) { return x + 1; }\nint gCount = 2, gOther;\n",
        )
        .unwrap();
        assert!(unit.typedefs.contains_key("u16") && unit.typedefs.contains_key("Pair"));
        assert_eq!(unit.records["Node"].len(), 2);
        assert!(unit.enum_constants.contains("KIND_B"));
        for name in ["gHead", "Get", "Helper", "gCount", "gOther"] {
            assert!(unit.globals.contains_key(name), "{name}");
        }
    }

    #[test]
    fn function_pointer_casts_stay_abstract() {
        let body = parse_body(
            "d = ((s32 (*)(s32))0x030001d8)(v);\nf((void (**)(void))0x030000f4);\n{ s32 (*fn)(s32 a) = 0; }",
            &["s32"],
        )
        .unwrap();
        let function = Function {
            name: "F".into(),
            header: "void F(void)".into(),
            params: Vec::new(),
            body,
        };
        let printed = function.print();
        assert!(
            printed.contains("((s32 (*)(s32))0x030001d8)(v);"),
            "{printed}"
        );
        assert!(
            printed.contains("f((void (**)(void))0x030000f4);"),
            "{printed}"
        );
        assert!(printed.contains("s32 (*fn)(s32 a) = 0;"), "{printed}");
    }

    #[test]
    fn switch_labels_and_loops_round_trip() {
        let body = parse_body(
            "switch (x & 3) { case 0: y = 1; break; default: y = 2; }\ndo { x--; } while (x);\ndone:\nreturn;",
            &[],
        )
        .unwrap();
        let function = Function {
            name: "F".into(),
            header: "void F(void)".into(),
            params: Vec::new(),
            body,
        };
        assert_eq!(
            function.print(),
            "void F(void)\n{\n    switch (x & 3) {\n    case 0:\n        y = 1;\n        break;\n    default:\n        y = 2;\n    }\n    do {\n        x--;\n    } while (x);\ndone:\n    return;\n}\n"
        );
    }
}
