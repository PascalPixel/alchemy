//! Deterministic C formatting for candidate identity and saved source.

use super::Node;

#[derive(Default)]
pub(super) struct Generator { indent: usize }

impl Generator {
    fn indentation(&self) -> String { " ".repeat(self.indent) }

    fn optional(&mut self, node: &Option<Box<Node>>) -> String {
        node.as_deref().map_or_else(String::new, |n| self.visit(n))
    }

    fn joined(&mut self, nodes: &[Node], separator: &str) -> String {
        nodes.iter().map(|n| self.visit(n)).collect::<Vec<_>>().join(separator)
    }

    fn expression(&mut self, node: &Node) -> String {
        match node {
            Node::InitList { .. } => format!("{{{}}}", self.visit(node)),
            Node::ExprList { .. } | Node::Compound { .. } => format!("({})", self.visit(node)),
            _ => self.visit(node),
        }
    }

    fn parenthesize(&mut self, node: &Node, condition: bool) -> String {
        let s = self.expression(node);
        if condition { format!("({s})") } else { s }
    }

    fn simple(&mut self, node: &Node) -> String {
        self.parenthesize(node, !matches!(node, Node::Constant { .. } | Node::Id { .. } | Node::ArrayRef { .. } | Node::StructRef { .. } | Node::FuncCall { .. }))
    }

    fn attributes(&mut self, attrs: &[Node]) -> String {
        if attrs.is_empty() { String::new() } else { format!(" __attribute__(({}))", self.joined(attrs, ", ")) }
    }

    fn declaration(&mut self, node: &Node, no_type: bool) -> String {
        let Node::Decl { name, funcspec, storage, align, ty, asmlabel, attributes, bitsize, init, .. } = node else { unreachable!() };
        let mut s = if no_type { name.clone().unwrap_or_default() } else {
            let mut s = String::new();
            if !funcspec.is_empty() { s += &(funcspec.join(" ") + " "); }
            if !storage.is_empty() { s += &(storage.join(" ") + " "); }
            if let Some(a) = align.first() { s += &(self.visit(a) + " "); }
            s + &self.ty(ty, &[], true)
        };
        if let Some(label) = asmlabel { s += &format!(" asm({})", self.visit(label)); }
        s += &self.attributes(attributes);
        if let Some(bits) = bitsize { s += &format!(" : {}", self.visit(bits)); }
        if let Some(init) = init { s += &format!(" = {}", self.expression(init)); }
        s
    }

    fn ty<'a>(&mut self, node: &'a Node, modifiers: &[&'a Node], emit_name: bool) -> String {
        match node {
            Node::TypeDecl { declname, quals, ty, .. } => {
                let mut s = if quals.is_empty() { String::new() } else { quals.join(" ") + " " };
                s += &self.visit(ty);
                let mut name = if emit_name { declname.clone().unwrap_or_default() } else { String::new() };
                for (i, modifier) in modifiers.iter().enumerate() {
                    match modifier {
                        Node::ArrayDecl { dim, dim_quals, .. } => {
                            if i > 0 && matches!(modifiers[i - 1], Node::PtrDecl { .. }) { name = format!("({name})"); }
                            name += "[";
                            if !dim_quals.is_empty() { name += &(dim_quals.join(" ") + " "); }
                            name += &(self.optional(dim) + "]");
                        }
                        Node::FuncDecl { args, .. } => {
                            if i > 0 && matches!(modifiers[i - 1], Node::PtrDecl { .. }) { name = format!("({name})"); }
                            name += &format!("({})", self.optional(args));
                        }
                        Node::PtrDecl { quals, .. } => {
                            name = if quals.is_empty() { "*".to_string() + &name } else {
                                format!("* {}{}", quals.join(" "), if name.is_empty() { name } else { " ".to_string() + &name })
                            };
                        }
                        _ => unreachable!(),
                    }
                }
                if !name.is_empty() { s += &(" ".to_string() + &name); }
                s
            }
            Node::Typename { ty, .. } => self.ty(ty, &[], emit_name),
            Node::IdentifierType { names } => names.join(" ") + " ",
            Node::ArrayDecl { ty, .. } | Node::PtrDecl { ty, .. } | Node::FuncDecl { ty, .. } => {
                let mut modifiers = modifiers.to_vec();
                modifiers.push(node);
                self.ty(ty, &modifiers, emit_name)
            }
            _ => self.visit(node),
        }
    }

    fn record(&mut self, keyword: &str, name: &Option<String>, attrs: &[Node], members: Option<&[Node]>) -> String {
        let mut s = format!("{keyword} {}", name.as_deref().unwrap_or(""));
        if let Some(members) = members {
            s += &format!("\n{}{{\n", self.indentation());
            self.indent += 2;
            if keyword == "enum" {
                let mut body = self.joined(members, "");
                body.truncate(body.len().saturating_sub(2));
                s += &(body + "\n");
            } else {
                for member in members { s += &self.statement(member, false); }
            }
            self.indent -= 2;
            s += &(self.indentation() + "}");
        }
        s + &self.attributes(attrs)
    }

    fn statement(&mut self, node: &Node, add_indent: bool) -> String {
        let indent = " ".repeat(self.indent + if add_indent { 2 } else { 0 });
        let s = self.visit(node);
        match node {
            Node::Decl { .. } | Node::Assignment { .. } | Node::Cast { .. } | Node::UnaryOp { .. } | Node::BinaryOp { .. } | Node::TernaryOp { .. } | Node::FuncCall { .. } | Node::ArrayRef { .. } | Node::StructRef { .. } | Node::Constant { .. } | Node::Id { .. } | Node::Typedef { .. } | Node::ExprList { .. } => format!("{indent}{s};\n"),
            Node::Compound { .. } => s,
            Node::If { .. } => indent + &s,
            _ => format!("{indent}{s}\n"),
        }
    }

    pub fn visit(&mut self, node: &Node) -> String {
        match node {
            Node::Constant { value, .. } => value.clone(),
            Node::Id { name } => name.clone(),
            Node::Pragma { string } => if string.is_empty() { "#pragma".into() } else { format!("#pragma {string}") },
            Node::ArrayRef { name, subscript } => format!("{}[{}]", self.simple(name), self.visit(subscript)),
            Node::StructRef { name, op, field } => format!("{}{}{}", self.simple(name), op, self.visit(field)),
            Node::FuncCall { name, args } => format!("{}({})", self.simple(name), self.optional(args)),
            Node::Typeof { expr } => format!("typeof({})", self.visit(expr)),
            Node::UnaryOp { op, expr } if op == "sizeof" => format!("sizeof({})", self.visit(expr)),
            Node::UnaryOp { op, expr } if op == "p++" || op == "p--" => format!("{}{}", self.simple(expr), &op[1..]),
            Node::UnaryOp { op, expr } => format!("{op}{}", self.simple(expr)),
            Node::BinaryOp { op, left, right } => format!("{} {op} {}", self.simple(left), self.simple(right)),
            Node::Assignment { op, lvalue, rvalue } => format!("{} {op} {}", self.visit(lvalue), self.parenthesize(rvalue, matches!(rvalue.as_ref(), Node::Assignment { .. }))),
            Node::IdentifierType { names } => names.join(" "),
            Node::Decl { .. } => self.declaration(node, false),
            Node::DeclList { decls } => decls.iter().enumerate().map(|(i, n)| self.declaration(n, i != 0)).collect::<Vec<_>>().join(", "),
            Node::Typedef { storage, ty, .. } => {
                let s = if storage.is_empty() { String::new() } else { storage.join(" ") + " " };
                s + &self.ty(ty, &[], true)
            }
            Node::Cast { to_type, expr } => format!("({}) {}", self.ty(to_type, &[], false), self.simple(expr)),
            Node::ExprList { exprs } | Node::InitList { exprs } => exprs.iter().map(|n| self.expression(n)).collect::<Vec<_>>().join(", "),
            Node::Alignas { alignment } => format!("_Alignas({})", self.visit(alignment)),
            Node::Enumerator { name, attributes, value } => {
                let mut s = self.indentation() + name + &self.attributes(attributes);
                if let Some(value) = value { s += &format!(" = {}", self.visit(value)); }
                s + ",\n"
            }
            Node::FuncDef { decl, param_decls, body } => {
                let decl = self.visit(decl);
                let params = if param_decls.is_empty() { String::new() } else { self.joined(param_decls, ";\n") + ";\n" };
                format!("{decl}\n{params}{}\n", self.visit(body))
            }
            Node::FileAst { ext } => {
                let mut s = String::new();
                for n in ext {
                    s += &self.visit(n);
                    s += match n { Node::FuncDef { .. } => "", Node::Pragma { .. } => "\n", _ => ";\n" };
                }
                s
            }
            Node::Compound { block_items } => {
                let mut s = self.indentation() + "{\n";
                self.indent += 2;
                for n in block_items { s += &self.statement(n, false); }
                self.indent -= 2;
                s + &self.indentation() + "}\n"
            }
            Node::CompoundLiteral { ty, init } => format!("({}){{{}}}", self.visit(ty), self.visit(init)),
            Node::AttributeStatement { attrs } => self.attributes(attrs).trim_start().to_string() + ";",
            Node::EmptyStatement {} => ";".into(),
            Node::ParamList { params } => self.joined(params, ", "),
            Node::Return { expr } => if let Some(expr) = expr { format!("return {};", self.visit(expr)) } else { "return;".into() },
            Node::Break {} => "break;".into(),
            Node::Continue {} => "continue;".into(),
            Node::TernaryOp { cond, iftrue, iffalse } => format!("({}) ? ({}) : ({})", self.expression(cond), self.expression(iftrue), self.expression(iffalse)),
            Node::If { cond, iftrue, iffalse } => {
                let mut s = format!("if ({})\n{}", self.visit(cond), self.statement(iftrue, true));
                if let Some(n) = iffalse {
                    let n = match n.as_ref() { Node::Compound { block_items } if block_items.len() == 1 && matches!(block_items[0], Node::If { .. }) => &block_items[0], _ => n };
                    s += &(self.indentation() + "else\n" + &self.statement(n, true));
                }
                s
            }
            Node::For { init, cond, next, stmt } => {
                let init = self.optional(init);
                let cond = if cond.is_some() { " ".to_string() + &self.optional(cond) } else { String::new() };
                let next = if next.is_some() { " ".to_string() + &self.optional(next) } else { String::new() };
                format!("for ({init};{cond};{next})\n{}", self.statement(stmt, true))
            }
            Node::While { cond, stmt } => format!("while ({})\n{}", self.visit(cond), self.statement(stmt, true)),
            Node::DoWhile { cond, stmt } => format!("do\n{}{}while ({});", self.statement(stmt, true), self.indentation(), self.visit(cond)),
            Node::StaticAssert { cond, message } => format!("_Static_assert({}{})", self.visit(cond), if let Some(message) = message { ",".to_string() + &self.visit(message) } else { String::new() }),
            Node::Switch { cond, stmt } => format!("switch ({})\n{}", self.visit(cond), self.statement(stmt, true)),
            Node::Case { expr, stmts } => {
                let mut s = format!("case {}:\n", self.visit(expr));
                for stmt in stmts { s += &self.statement(stmt, true); }
                s
            }
            Node::Default { stmts } => {
                let mut s = "default:\n".to_string();
                for stmt in stmts { s += &self.statement(stmt, true); }
                s
            }
            Node::Label { name, stmt } => format!("{name}:\n{}", self.statement(stmt, false)),
            Node::Goto { name } => format!("goto {name};"),
            Node::EllipsisParam {} => "...".into(),
            Node::Struct { name, attributes, decls } => self.record("struct", name, attributes, decls.as_deref()),
            Node::Union { name, attributes, decls } => self.record("union", name, attributes, decls.as_deref()),
            Node::Enum { name, attributes, values } => {
                let members = values.as_deref().map(|v| match v { Node::EnumeratorList { enumerators } => enumerators.as_slice(), _ => unreachable!() });
                self.record("enum", name, attributes, members)
            }
            Node::Typename { ty, .. } => self.ty(ty, &[], true),
            Node::NamedInitializer { name, expr } => {
                let mut s = String::new();
                for n in name { s += &if let Node::Id { name } = n { format!(".{name}") } else { format!("[{}]", self.visit(n)) }; }
                s + " = " + &self.expression(expr)
            }
            Node::FuncDecl { .. } => self.ty(node, &[], true),
            Node::ArrayDecl { .. } | Node::TypeDecl { .. } | Node::PtrDecl { .. } => self.ty(node, &[], false),
            Node::Range { first, last } => format!("{} ... {}", self.visit(first), self.visit(last)),
            Node::Attribute { name, args } => if let Some(args) = args { format!("{name}({})", self.joined(args, ", ")) } else { name.clone() },
            Node::Asm { quals, asm, outputs, inputs, clobbers, gotos } => {
                let mut s = "asm".to_string();
                for q in quals { s += &(" ".to_string() + q); }
                s += &format!("({}", self.visit(asm));
                let mut parts = vec![self.joined(outputs, ", "), self.joined(inputs, ", "), self.joined(clobbers, ", "), gotos.join(", ")];
                if !quals.iter().any(|q| q == "goto") { while parts.last().is_some_and(String::is_empty) { parts.pop(); } }
                for part in parts { s += " :"; if !part.is_empty() { s += &(" ".to_string() + &part); } }
                s + ");"
            }
            Node::AsmOperand { symbolic_name, constraint, expr } => format!("{}{}({})", symbolic_name.as_ref().map_or_else(String::new, |n| format!("[{n}]")), self.visit(constraint), self.visit(expr)),
            Node::EnumeratorList { .. } => node.children().iter().map(|n| self.visit(n)).collect(),
        }
    }
}
