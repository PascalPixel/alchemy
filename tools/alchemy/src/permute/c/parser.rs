use super::{Node, Optional};
use lang_c::{ast as c, driver, span::Node as Spanned};
use std::collections::{HashMap, HashSet};

type Result<T> = std::result::Result<T, String>;

/// lang-c discards directives. Preserve them as unique placeholder nodes, and
/// control statement's body with the following statement into a compound.
pub fn parse(source: &str) -> Result<Node> {
    let tokens = crate::permute::lex::lex(source)?;
    let mut prefix = "__alchemy_pragma_".to_string();
    while source.contains(&prefix) { prefix.push('_'); }
    let mut pragmas = HashMap::new();
    let mut input = String::new();
    let mut preceding = Vec::new();
    let mut position = 0;
    let mut tokens = tokens.iter().peekable();
    for line in source.split_inclusive('\n') {
        while tokens.peek().is_some_and(|t| t.start < position) {
            let token = tokens.next().unwrap();
            if !matches!(token.tok, crate::permute::lex::Tok::Comment(_)) { preceding.push(&token.tok); }
        }
        let directive = line.trim_start().strip_prefix('#').map(str::trim_start);
        if let Some(pragma) = directive.and_then(|s| s.strip_prefix("pragma")).filter(|s| s.is_empty() || s.starts_with(char::is_whitespace)) {
            let name = format!("{prefix}{}", pragmas.len());
            pragmas.insert(name.clone(), pragma.trim().to_string());
            input += &if decorates_statement(&preceding) { format!("{name}:\n") } else { format!("int {name};\n") };
        } else if directive.is_some_and(|s| s.starts_with("ident") || s.starts_with("line") || s.starts_with(|c: char| c.is_ascii_digit())) {
            input.push('\n');
        } else {
            // Comments have already served their purpose before preprocessing.
            // Accept them in fixtures too, without changing source offsets.
            input += line;
        }
        position += line.len();
    }
    let comments = crate::permute::lex::lex(&input)?;
    for token in comments.iter().rev() {
        if matches!(token.tok, crate::permute::lex::Tok::Comment(_)) {
            let blank: String = input[token.start..token.end].chars().map(|c| if c == '\n' { '\n' } else { ' ' }).collect();
            input.replace_range(token.start..token.end, &blank);
        }
    }
    let global_asm = preserve_global_asm(&mut input, &prefix)?;
    let aliases = permissive_input(&mut input, &prefix)?;
    let mut parsed = parse_input(input)?;
    while let Some((open, close)) = sizeof_cast_ambiguity(&parsed)? {
        let mut input = parsed.source;
        input.insert(close, ')');
        input.insert(open, '(');
        parsed = parse_input(input)?;
    }
    let mut adapter = Adapter { source: parsed.source, pragmas, global_asm, anonymous: 0, prefix: prefix.clone(), typedefs: HashSet::new(), shadowed: HashSet::new(), aliases: aliases.clone() };
    let mut ext = Vec::new();
    for item in &parsed.unit.0 {
        match &item.node {
            c::ExternalDeclaration::Declaration(d) => ext.extend(adapter.declaration(&d.node)?.into_iter().filter(|n| !matches!(n, Node::Typedef { name, .. } if name == &format!("{prefix}int128")))),
            c::ExternalDeclaration::StaticAssert(a) => ext.push(adapter.assertion(&a.node)?),
            c::ExternalDeclaration::FunctionDefinition(f) => {
                let f = &f.node;
                let specs = adapter.specifiers(&f.specifiers)?;
                let decl = adapter.decl(&specs, Some(&f.declarator), None, None)?.boxed();
                let mut param_decls = Vec::new();
                for d in &f.declarations { param_decls.extend(adapter.declaration(&d.node)?); }
                ext.push(Node::FuncDef { decl, param_decls, body: adapter.statement(&f.statement.node)?.boxed() });
            }
        }
    }
    fn restore(node: &mut Node, aliases: &HashMap<String, String>) {
        let value = match node {
            Node::Id { name } | Node::Label { name, .. } | Node::Goto { name } | Node::Typedef { name, .. } | Node::Enumerator { name, .. } | Node::Attribute { name, .. } => Some(name),
            Node::Decl { name, .. } | Node::TypeDecl { declname: name, .. } | Node::Struct { name, .. } | Node::Union { name, .. } | Node::Enum { name, .. } | Node::Typename { name, .. } => name.as_mut(),
            _ => None,
        };
        if let Some(value) = value { if let Some(original) = aliases.get(value) { *value = original.clone(); } }
        if let Node::IdentifierType { names } = node { for name in names { if let Some(original) = aliases.get(name) { *name = original.clone(); } } }
        // Restore metadata explicitly: it is outside mutation traversal.
        match node {
            Node::Decl { align, .. } | Node::TypeDecl { align, .. } | Node::Typename { align, .. } => for child in align { restore(child, aliases); },
            Node::Enumerator { attributes, .. } => for child in attributes { restore(child, aliases); },
            _ => {}
        }
        for child in node.children_mut() { restore(child, aliases); }
    }
    let mut unit = Node::FileAst { ext };
    restore(&mut unit, &aliases);
    validate_names(&unit, &mut Names::default())?;
    Ok(unit)
}

/// permitting either kind to shadow a name in an enclosing scope.
#[derive(Default)]
struct Names { visible: HashMap<String, bool>, local: HashSet<String> }

impl Names {
    fn child(&self) -> Self { Self { visible: self.visible.clone(), local: HashSet::new() } }
}

fn validate_names(node: &Node, scope: &mut Names) -> Result<()> {
    fn bind(scope: &mut Names, name: &str, typedef: bool) -> Result<()> {
        if scope.local.contains(name) && scope.visible.get(name).is_some_and(|old| *old != typedef) {
            return Err(format!("{name} redeclared as a different kind of symbol"));
        }
        scope.visible.insert(name.to_string(), typedef);
        scope.local.insert(name.to_string());
        Ok(())
    }
    fn enum_names(node: &Node, scope: &mut Names) -> Result<()> {
        match node {
            Node::Enumerator { name, .. } => bind(scope, name, false)?,
            Node::FuncDecl { ty, .. } => enum_names(ty, scope)?,
            _ => { for child in node.children() { enum_names(child, scope)?; } },
        }
        Ok(())
    }
    fn check_types(node: &Node, scope: &Names) -> Result<()> {
        if let Node::IdentifierType { names } = node {
            if names.len() == 1 && scope.visible.get(&names[0]) == Some(&false) { return Err(format!("{} is no longer a typedef in this scope", names[0])); }
        }
        for child in node.children() { check_types(child, scope)?; }
        Ok(())
    }
    match node {
        Node::FileAst { ext } => { for item in ext { validate_names(item, scope)?; } },
        Node::Typedef { name, ty, .. } => { check_types(ty, scope)?; enum_names(ty, scope)?; bind(scope, name, true)?; },
        Node::Decl { name, init, ty, .. } => {
            check_types(ty, scope)?;
            enum_names(ty, scope)?;
            if let Some(init) = init { validate_names(init, scope)?; }
            if let Some(name) = name { bind(scope, name, false)?; }
        },
        Node::FuncDef { decl, param_decls, body } => {
            validate_names(decl, scope)?;
            let mut local = scope.child();
            if let Node::Decl { ty, .. } = decl.as_ref() {
                let mut ty = ty.as_ref();
                while let Node::PtrDecl { ty: inner, .. } = ty { ty = inner; }
                if let Node::FuncDecl { args: Some(args), .. } = ty {
                    if let Node::ParamList { params } = args.as_ref() {
                        for param in params { match param {
                            Node::Decl { name: Some(name), .. } | Node::Id { name } => bind(&mut local, name, false)?,
                            _ => {},
                        } }
                    }
                }
            }
            for decl in param_decls { validate_names(decl, &mut local)?; }
            if let Node::Compound { block_items } = body.as_ref() {
                for item in block_items { validate_names(item, &mut local)?; }
            } else { validate_names(body, &mut local)?; }
        },
        Node::Compound { block_items } => {
            let mut local = scope.child();
            for item in block_items { validate_names(item, &mut local)?; }
        },
        Node::For { init, cond, next, stmt } => {
            for child in [init.as_deref(), cond.as_deref(), next.as_deref(), Some(stmt.as_ref())].into_iter().flatten() { validate_names(child, scope)?; }
        },
        Node::Typename { .. } | Node::TypeDecl { .. } | Node::PtrDecl { .. } | Node::ArrayDecl { .. } | Node::FuncDecl { .. } | Node::ParamList { .. } => {},
        _ => { for child in node.children() { validate_names(child, scope)?; } },
    }
    Ok(())
}

/// Bridge grammar differences while preserving the candidate tree and names.
fn permissive_input(input: &mut String, prefix: &str) -> Result<HashMap<String, String>> {
    use crate::permute::lex::{lex, Tok};
    let tokens = lex(input)?;
    let mut patches = Vec::new();
    let mut aliases = HashMap::new();
    // Keep a reused typedef name visible until the parameter list is complete.
    let mut typedefs = HashSet::new();
    for (i, token) in tokens.iter().enumerate() {
        if matches!(&token.tok, Tok::Ident(s) if s == "typedef") {
            if let Some(end) = tokens[i..].iter().position(|t| t.tok == Tok::Punct(";")) {
                if let Some(Tok::Ident(name)) = i.checked_add(end).and_then(|end| end.checked_sub(1)).map(|end| &tokens[end].tok) { typedefs.insert(name.clone()); }
            }
        }
    }
    let mut brace_depth = 0usize;
    let mut paren_depth = 0usize;
    for (i, token) in tokens.iter().enumerate() {
        if let Tok::Ident(name) = &token.tok {
            if brace_depth > 0 && typedefs.contains(name) && i > 0
                && matches!(&tokens[i - 1].tok, Tok::Ident(s) if typedefs.contains(s) || matches!(s.as_str(), "int" | "char" | "unsigned" | "signed" | "long" | "short" | "float" | "double"))
                && tokens.get(i + 1).is_some_and(|t| t.tok == Tok::Punct("="))
                && tokens.get(i + 2).is_some_and(|t| t.tok == Tok::Punct("("))
                && tokens.get(i + 3).is_some_and(|t| t.tok == Tok::Punct("{")) {
                let alias = format!("{prefix}deferred_value_{}", aliases.len());
                aliases.insert(alias.clone(), name.clone());
                patches.push((token.start, token.end, alias));
                let (mut parentheses, mut braces) = (0usize, 0usize);
                for end in tokens.iter().skip(i + 2) {
                    match end.tok {
                        Tok::Punct("(") => parentheses += 1,
                        Tok::Punct(")") => parentheses = parentheses.saturating_sub(1),
                        Tok::Punct("{") => braces += 1,
                        Tok::Punct("}") => braces = braces.saturating_sub(1),
                        Tok::Punct(";") if parentheses == 0 && braces == 0 => {
                            patches.push((end.start, end.start, format!(", {name} = {prefix}deferred_binding()")));
                            break;
                        },
                        _ => {},
                    }
                }
            }
        }
        if token.tok == Tok::Punct("(") && brace_depth == 0 && paren_depth == 0
            && i > 0 && matches!(&tokens[i - 1].tok, Tok::Ident(s) if !matches!(s.as_str(), "_Atomic" | "_Alignas" | "_Static_assert" | "__attribute__" | "asm" | "__asm__" | "typeof" | "__typeof__")) {
            let mut depth = 1;
            let mut renamed = HashMap::new();
            let mut end = i + 1;
            while end < tokens.len() && depth > 0 {
                let t = &tokens[end];
                if depth == 1 {
                    if let Tok::Ident(name) = &t.tok {
                        if typedefs.contains(name) && matches!(&tokens[end - 1].tok, Tok::Ident(s) if typedefs.contains(s) || matches!(s.as_str(), "int" | "unsigned" | "signed" | "long" | "short" | "char" | "float" | "double" | "_Bool"))
                            && tokens.get(end + 1).is_some_and(|t| matches!(t.tok, Tok::Punct("," | ")" | "["))) {
                            let alias = format!("{prefix}parameter_{}", aliases.len());
                            patches.push((t.start, t.end, alias.clone()));
                            aliases.insert(alias.clone(), name.clone());
                            renamed.insert(name.clone(), alias);
                        }
                    }
                }
                match t.tok { Tok::Punct("(") => depth += 1, Tok::Punct(")") => depth -= 1, _ => {} }
                end += 1;
            }
            if !renamed.is_empty() && tokens.get(end).is_some_and(|t| t.tok == Tok::Punct("{")) {
                let mut depth = 1;
                end += 1;
                while end < tokens.len() && depth > 0 {
                    let t = &tokens[end];
                    if let Tok::Ident(name) = &t.tok { if let Some(alias) = renamed.get(name) { patches.push((t.start, t.end, alias.clone())); } }
                    match t.tok { Tok::Punct("{") => depth += 1, Tok::Punct("}") => depth -= 1, _ => {} }
                    end += 1;
                }
            }
        }
        match token.tok { Tok::Punct("{") => brace_depth += 1, Tok::Punct("}") => brace_depth = brace_depth.saturating_sub(1), Tok::Punct("(") => paren_depth += 1, Tok::Punct(")") => paren_depth = paren_depth.saturating_sub(1), _ => {} }
    }
    let mut braces = Vec::new();
    let mut record = false;
    let mut int128 = false;
    let mut parentheses = 0usize;
    for (i, token) in tokens.iter().enumerate() {
        match &token.tok {
            Tok::Punct("(") => parentheses += 1,
            Tok::Punct(")") => parentheses = parentheses.saturating_sub(1),
            Tok::Ident(s) if matches!(s.as_str(), "struct" | "union") => record = true,
            Tok::Punct("{") => { braces.push(record); record = false; },
            Tok::Punct("}") => { braces.pop(); },
            // Empty declarations never occur inside a for header. A struct
            // cast in an enclosing condition can also set the record hint.
            Tok::Punct(";") if parentheses == 0 && (braces.is_empty() || braces.last() == Some(&true)) && (i == 0 || matches!(tokens[i - 1].tok, Tok::Punct(";" | "{"))) => patches.push((token.start, token.end, String::new())),
            Tok::Punct(":") if tokens.get(i + 1).is_some_and(|t| t.tok == Tok::Punct("}")) => patches.push((token.end, token.end, ";".into())),
            Tok::Ident(s) if s == "__int128" => { patches.push((token.start, token.end, format!("{prefix}int128"))); int128 = true; },
            Tok::Ident(s) if s == "__builtin_offsetof" => return Err("__builtin_offsetof is not supported by the C parser".into()),
            Tok::Ident(s) if s == "offsetof" && tokens.get(i + 1).is_some_and(|t| t.tok == Tok::Punct("(")) => patches.push((token.start, token.end, "__builtin_offsetof".into())),
            Tok::Ident(s) if matches!(s.as_str(), "_Alignas" | "_Static_assert") => {
                let mut depth = 0;
                let mut comma = false;
                for end in i + 1..tokens.len() {
                    match tokens[end].tok {
                        Tok::Punct("(") => depth += 1,
                        Tok::Punct(")") => depth -= 1,
                        Tok::Punct(",") if depth == 1 => comma = true,
                        _ => {},
                    }
                    if depth == 0 {
                        if s == "_Alignas" {
                            if braces.last() != Some(&true) { break; }
                            let args = &input[tokens[i + 1].end..tokens[end].start];
                            patches.push((token.start, tokens[end].end, format!("__attribute__(({prefix}align(sizeof({args}))))")));
                        } else if !comma { patches.push((tokens[end].start, tokens[end].start, format!(",\"{prefix}assert\""))); }
                        break;
                    }
                }
            },
            Tok::Ident(s) if braces.is_empty() && parentheses == 0
                && !typedefs.contains(s)
                && tokens.get(i + 1).is_some_and(|t| t.tok == Tok::Punct("("))
                && (i == 0 || match &tokens[i - 1].tok { Tok::Punct(";" | "}") => true, Tok::Ident(s) => matches!(s.as_str(), "extern" | "static" | "inline"), _ => false })
                && !matches!(s.as_str(), "int" | "void" | "char" | "short" | "long" | "float" | "double" | "signed" | "unsigned" | "_Bool" | "asm" | "__asm" | "__asm__" | "_Atomic" | "typeof" | "__typeof__" | "__typeof" | "__attribute__" | "__extension__") => patches.push((token.start, token.start, "int ".into())),
            _ => {},
        }
        if token.tok == Tok::Punct(";") { record = false; }
    }
    patches.sort_by_key(|p| (p.0, p.1));
    for (start, end, text) in patches.into_iter().rev() { input.replace_range(start..end, &text); }
    if int128 { input.insert_str(0, &format!("typedef int {prefix}int128;\n")); }
    Ok(aliases)
}

fn preserve_global_asm(input: &mut String, prefix: &str) -> Result<HashMap<String, Node>> {
    use crate::permute::lex::{lex, Tok};
    let tokens: Vec<_> = lex(input)?.into_iter().filter(|t| !matches!(t.tok, Tok::Comment(_))).collect();
    let mut depth = 0;
    let mut patches = Vec::new();
    let mut statements = HashMap::new();
    let mut index = 0;
    while index < tokens.len() {
        let token = &tokens[index];
        if depth == 0 && matches!(&token.tok, Tok::Ident(s) if matches!(s.as_str(), "asm" | "__asm" | "__asm__")) && (index == 0 || matches!(tokens[index - 1].tok, Tok::Punct(";" | "}"))) {
            let start = index;
            index += 1;
            if tokens.get(index).is_none_or(|t| t.tok != Tok::Punct("(")) { return Err("invalid global asm".into()); }
            index += 1;
            let mut strings = Vec::new();
            while let Some(token) = tokens.get(index) {
                let Tok::Str(s) = &token.tok else { break };
                strings.push(s.clone());
                index += 1;
            }
            if strings.is_empty() || tokens.get(index).is_none_or(|t| t.tok != Tok::Punct(")")) || tokens.get(index + 1).is_none_or(|t| t.tok != Tok::Punct(";")) { return Err("invalid global asm".into()); }
            let name = format!("{prefix}asm{}", statements.len());
            statements.insert(name.clone(), Node::Asm { quals: Vec::new(), asm: string_literal(&strings).boxed(), outputs: Vec::new(), inputs: Vec::new(), clobbers: Vec::new(), gotos: Vec::new() });
            patches.push((tokens[start].start, tokens[index + 1].end, format!("int {name};")));
            index += 2;
            continue;
        }
        match token.tok { Tok::Punct("{") => depth += 1, Tok::Punct("}") => depth -= 1, _ => {} }
        index += 1;
    }
    for (start, end, replacement) in patches.into_iter().rev() { input.replace_range(start..end, &replacement); }
    Ok(statements)
}

fn string_literal(strings: &[String]) -> Node {
    let mut value = String::new();
    for s in strings {
        if value.is_empty() { value = s.clone(); } else {
            value.pop();
            value += s.split_once('"').map_or(s.as_str(), |(_, tail)| tail);
        }
    }
    Node::Constant { ty: "string".into(), value }
}

fn parse_input(input: String) -> Result<driver::Parse> {
    driver::parse_preprocessed(&driver::Config::with_gcc(), input).map_err(|e| format!("C parse error at {}:{}: expected {}\n    {}", e.line, e.column, {
        let mut expected: Vec<_> = e.expected.iter().copied().collect();
        expected.sort(); expected.join(", ")
    }, e.source.lines().nth(e.line.saturating_sub(1)).unwrap_or("<end of input>").trim()))
}

/// `sizeof((T) +x)` (likewise -, * and &). Preserve that observable behavior,
/// including its precedence, instead of silently correcting the user's C.
fn sizeof_cast_ambiguity(parsed: &driver::Parse) -> Result<Option<(usize, usize)>> {
    use lang_c::{span::Span, visit::{self, Visit}};
    use crate::permute::lex::Tok;
    #[derive(Default)]
    struct Expressions<'a> { nodes: Vec<(&'a c::Expression, Span)>, for_names: HashSet<String> }
    impl<'a> Visit<'a> for Expressions<'a> {
        fn visit_statement(&mut self, s: &'a c::Statement, span: &'a Span) {
            let enclosing = matches!(s, c::Statement::Compound(_)).then(|| self.for_names.clone());
            if let c::Statement::For(f) = s {
                if let c::ForInitializer::Declaration(d) = &f.node.initializer.node {
                    fn name(d: &c::Declarator) -> Option<&str> {
                        match &d.kind.node {
                            c::DeclaratorKind::Identifier(id) => Some(&id.node.name),
                            c::DeclaratorKind::Declarator(d) => name(&d.node),
                            _ => None,
                        }
                    }
                    for d in &d.node.declarators {
                        if let Some(name) = name(&d.node.declarator.node) { self.for_names.insert(name.into()); }
                    }
                }
            }
            visit::visit_statement(self, s, span);
            if let Some(enclosing) = enclosing { self.for_names = enclosing; }
        }
        fn visit_expression(&mut self, e: &'a c::Expression, span: &'a Span) {
            let shadowed = match e {
                c::Expression::SizeOfTy(t) => t.node.0.node.specifiers.iter().any(|s| matches!(&s.node,
                    c::SpecifierQualifier::TypeSpecifier(t) if matches!(&t.node,
                        c::TypeSpecifier::TypedefName(id) if self.for_names.contains(&id.node.name)))),
                _ => false,
            };
            if !shadowed { self.nodes.push((e, *span)); }
            visit::visit_expression(self, e, span);
        }
    }
    let mut expressions = Expressions::default();
    visit::visit_translation_unit(&mut expressions, &parsed.unit);
    let tokens = crate::permute::lex::lex(&parsed.source)?;
    let mut matches = Vec::new();
    for (expr, span) in &expressions.nodes {
        if !matches!(expr, c::Expression::SizeOfTy(_)) || !parsed.source[span.start..span.end].starts_with("sizeof") { continue; }
        let next = tokens.partition_point(|t| t.start < span.end);
        if !tokens.get(next).is_some_and(|t| matches!(t.tok, Tok::Punct("+" | "-" | "*" | "&"))) { continue; }
        let Some(operand) = tokens.get(next + 1) else { continue };
        let mut end = expressions.nodes.iter().filter(|(node, s)| {
            if s.start != operand.start { return false; }
            let is_binary = matches!(node, c::Expression::Comma(_) | c::Expression::Conditional(_)) || matches!(node, c::Expression::BinaryOperator(b) if b.node.operator.node != c::BinaryOperator::Index);
            if !is_binary { return true; }
            // Parenthesized expressions are unary operands regardless of the
            // expression kind contained inside their parentheses.
            let text = &parsed.source[s.start..s.end];
            let mut depth = 0isize;
            let mut fully_wrapped = text.starts_with('(');
            for t in tokens.iter().filter(|t| t.start >= s.start && t.end <= s.end) {
                match t.tok { Tok::Punct("(") => depth += 1, Tok::Punct(")") => depth -= 1, _ => {} }
                if depth == 0 && t.end < s.end { fully_wrapped = false; }
            }
            fully_wrapped
        }).map(|(_, s)| s.end).max();
        if operand.tok == Tok::Punct("(") {
            let mut depth = 0;
            for t in &tokens[next + 1..] {
                match t.tok { Tok::Punct("(") => depth += 1, Tok::Punct(")") => depth -= 1, _ => {} }
                if depth == 0 { end = Some(end.unwrap_or(0).max(t.end)); break; }
            }
        }
        if let Some(end) = end {
            let open = span.start + parsed.source[span.start..span.end].find('(').expect("sizeof type has parentheses");
            matches.push((open, end));
        }
    }
    // Resolve inner/rightmost ambiguity first; a later parse then sees the
    // complete unary operand when ambiguities are nested or chained.
    Ok(matches.into_iter().max_by_key(|(start, _)| *start))
}

fn decorates_statement(tokens: &[&crate::permute::lex::Tok]) -> bool {
    use crate::permute::lex::Tok;
    match tokens.last() {
        Some(Tok::Ident(s)) if s == "else" || s == "do" => true,
        Some(Tok::Punct(":")) => true,
        Some(Tok::Punct(")")) => {
            let mut depth = 0;
            for (index, token) in tokens.iter().enumerate().rev() {
                match token {
                    Tok::Punct(")") => depth += 1,
                    Tok::Punct("(") => {
                        depth -= 1;
                        if depth == 0 { return index > 0 && matches!(tokens[index - 1], Tok::Ident(s) if matches!(s.as_str(), "if" | "for" | "while" | "switch")); }
                    }
                    _ => {}
                }
            }
            false
        }
        _ => false,
    }
}

#[derive(Clone)]
struct Specs {
    base: Node,
    quals: Vec<String>,
    storage: Vec<String>,
    funcspec: Vec<String>,
    align: Vec<Node>,
    attributes: Vec<Node>,
    asmlabel: Optional,
}

struct Adapter { source: String, pragmas: HashMap<String, String>, global_asm: HashMap<String, Node>, anonymous: usize, prefix: String, typedefs: HashSet<String>, shadowed: HashSet<String>, aliases: HashMap<String, String> }

fn name(decl: &c::Declarator) -> Option<String> {
    match &decl.kind.node {
        c::DeclaratorKind::Abstract => None,
        c::DeclaratorKind::Identifier(id) => Some(id.node.name.clone()),
        c::DeclaratorKind::Declarator(d) => name(&d.node),
    }
}

fn qualifier(q: &c::TypeQualifier) -> &'static str {
    match q {
        c::TypeQualifier::Const => "const", c::TypeQualifier::Restrict => "restrict",
        c::TypeQualifier::Volatile => "volatile", c::TypeQualifier::Atomic => "_Atomic",
        c::TypeQualifier::Nonnull => "_Nonnull", c::TypeQualifier::NullUnspecified => "_Null_unspecified", c::TypeQualifier::Nullable => "_Nullable",
    }
}

impl Adapter {
    fn text<T>(&self, n: &Spanned<T>) -> &str { &self.source[n.span.start..n.span.end] }

    fn string(&self, s: &c::StringLiteral) -> Node {
        string_literal(s)
    }

    fn expression(&mut self, expression: &Spanned<c::Expression>) -> Result<Node> {
        use c::Expression as E;
        Ok(match &expression.node {
            E::Identifier(n) => Node::id(&n.node.name),
            E::Constant(n) => {
                let ty = match &n.node {
                    c::Constant::Integer(i) => {
                        if i.suffix.imaginary { return Err("imaginary integer constants are not supported".into()); }
                        let size = match i.suffix.size { c::IntegerSize::Int => "int", c::IntegerSize::Long => "long int", c::IntegerSize::LongLong => "long long int" };
                        if i.suffix.unsigned { format!("unsigned {size}") } else { size.into() }
                    }
                    c::Constant::Float(f) => match f.suffix.format {
                        c::FloatFormat::Float => "float".into(), c::FloatFormat::Double => "double".into(), c::FloatFormat::LongDouble => "long double".into(),
                        _ => return Err("unsupported floating constant format".into()),
                    },
                    c::Constant::Character(_) => "char".into(),
                };
                Node::Constant { ty, value: self.text(n).into() }
            }
            E::StringLiteral(n) => self.string(&n.node),
            E::Member(n) => Node::StructRef {
                name: self.expression(&n.node.expression)?.boxed(),
                op: if n.node.operator.node == c::MemberOperator::Direct { "." } else { "->" }.into(),
                field: Node::id(&n.node.identifier.node.name).boxed(),
            },
            E::Call(n) => {
                let args = n.node.arguments.iter().map(|n| self.expression(n)).collect::<Result<Vec<_>>>()?;
                Node::FuncCall { name: self.expression(&n.node.callee)?.boxed(), args: (!args.is_empty()).then(|| Node::ExprList { exprs: args }.boxed()) }
            }
            E::CompoundLiteral(n) => Node::CompoundLiteral { ty: self.typename(&n.node.type_name.node)?.boxed(), init: self.initializer_list(&n.node.initializer_list)?.boxed() },
            E::SizeOfTy(n) => {
                let mut expr = self.typename(&n.node.0.node)?;
                if n.node.0.node.declarator.is_none() && n.node.0.node.specifiers.len() == 1 {
                    if let c::SpecifierQualifier::TypeSpecifier(t) = &n.node.0.node.specifiers[0].node {
                        if let c::TypeSpecifier::TypedefName(id) = &t.node {
                            if self.shadowed.contains(&id.node.name) { expr = Node::id(&id.node.name); }
                        }
                    }
                }
                Node::UnaryOp { op: "sizeof".into(), expr: expr.boxed() }
            },
            E::SizeOfVal(n) => {
                let expr = if let E::Identifier(id) = &n.node.0.node {
                    if self.typedefs.contains(&id.node.name) && !self.shadowed.contains(&id.node.name) {
                        Node::Typename { name: None, quals: Vec::new(), align: Vec::new(), attributes: Vec::new(), ty: Node::TypeDecl { declname: None, quals: Vec::new(), align: Vec::new(), ty: Node::IdentifierType { names: vec![id.node.name.clone()] }.boxed() }.boxed() }
                    } else { self.expression(&n.node.0)? }
                } else { self.expression(&n.node.0)? };
                Node::UnaryOp { op: "sizeof".into(), expr: expr.boxed() }
            },
            E::AlignOf(n) => Node::UnaryOp { op: "_Alignof".into(), expr: self.typename(&n.node.0.node)?.boxed() },
            E::UnaryOperator(n) => {
                use c::UnaryOperator as U;
                let op = match n.node.operator.node { U::PostIncrement => "p++", U::PostDecrement => "p--", U::PreIncrement => "++", U::PreDecrement => "--", U::Address => "&", U::Indirection => "*", U::Plus => "+", U::Minus => "-", U::Complement => "~", U::Negate => "!" };
                Node::UnaryOp { op: op.into(), expr: self.expression(&n.node.operand)?.boxed() }
            }
            E::Cast(n) => Node::Cast { to_type: self.typename(&n.node.type_name.node)?.boxed(), expr: self.expression(&n.node.expression)?.boxed() },
            E::BinaryOperator(n) => {
                use c::BinaryOperator as B;
                let left = self.expression(&n.node.lhs)?.boxed();
                let right = self.expression(&n.node.rhs)?.boxed();
                let (op, assign) = match n.node.operator.node {
                    B::Index => return Ok(Node::ArrayRef { name: left, subscript: right }),
                    B::Multiply => ("*", false), B::Divide => ("/", false), B::Modulo => ("%", false), B::Plus => ("+", false), B::Minus => ("-", false),
                    B::ShiftLeft => ("<<", false), B::ShiftRight => (">>", false), B::Less => ("<", false), B::Greater => (">", false), B::LessOrEqual => ("<=", false), B::GreaterOrEqual => (">=", false),
                    B::Equals => ("==", false), B::NotEquals => ("!=", false), B::BitwiseAnd => ("&", false), B::BitwiseXor => ("^", false), B::BitwiseOr => ("|", false), B::LogicalAnd => ("&&", false), B::LogicalOr => ("||", false),
                    B::Assign => ("=", true), B::AssignMultiply => ("*=", true), B::AssignDivide => ("/=", true), B::AssignModulo => ("%=", true), B::AssignPlus => ("+=", true), B::AssignMinus => ("-=", true),
                    B::AssignShiftLeft => ("<<=", true), B::AssignShiftRight => (">>=", true), B::AssignBitwiseAnd => ("&=", true), B::AssignBitwiseXor => ("^=", true), B::AssignBitwiseOr => ("|=", true),
                };
                if assign { Node::Assignment { op: op.into(), lvalue: left, rvalue: right } } else { Node::BinaryOp { op: op.into(), left, right } }
            }
            E::Conditional(n) => Node::TernaryOp { cond: self.expression(&n.node.condition)?.boxed(), iftrue: self.expression(&n.node.then_expression)?.boxed(), iffalse: self.expression(&n.node.else_expression)?.boxed() },
            E::Comma(nodes) => Node::ExprList { exprs: nodes.iter().map(|n| self.expression(n)).collect::<Result<_>>()? },
            E::Statement(n) => self.statement(&n.node)?,
            E::OffsetOf(o) => {
                let mut field = Node::id(&o.node.designator.node.base.node.name);
                for member in &o.node.designator.node.members { field = match &member.node {
                    c::OffsetMember::Member(id) => Node::StructRef { name: field.boxed(), op: ".".into(), field: Node::id(&id.node.name).boxed() },
                    c::OffsetMember::IndirectMember(id) => Node::StructRef { name: field.boxed(), op: "->".into(), field: Node::id(&id.node.name).boxed() },
                    c::OffsetMember::Index(index) => Node::ArrayRef { name: field.boxed(), subscript: self.expression(index)?.boxed() },
                }; }
                Node::FuncCall { name: Node::id("offsetof").boxed(), args: Some(Node::ExprList { exprs: vec![self.typename(&o.node.type_name.node)?, field] }.boxed()) }
            },
            E::GenericSelection(_) | E::VaArg(_) => return Err("C expression is not supported by the C tree".into()),
        })
    }

    fn extensions(&mut self, extensions: &[Spanned<c::Extension>]) -> Result<(Vec<Node>, Optional)> {
        let mut attributes = Vec::new();
        let mut label = None;
        for extension in extensions {
            match &extension.node {
                c::Extension::Attribute(a) => {
                    let args = if a.arguments.is_empty() && !self.text(extension).contains('(') { None } else { Some(a.arguments.iter().map(|n| self.expression(n)).collect::<Result<_>>()?) };
                    attributes.push(Node::Attribute { name: a.name.node.clone(), args });
                }
                c::Extension::AsmLabel(s) => label = Some(self.string(&s.node).boxed()),
                c::Extension::AvailabilityAttribute(_) => return Err("Clang availability attributes are not supported".into()),
            }
        }
        Ok((attributes, label))
    }

    fn specifiers(&mut self, specifiers: &[Spanned<c::DeclarationSpecifier>]) -> Result<Specs> {
        let mut result = Specs { base: Node::IdentifierType { names: Vec::new() }, quals: Vec::new(), storage: Vec::new(), funcspec: Vec::new(), align: Vec::new(), attributes: Vec::new(), asmlabel: None };
        let mut record_attributes = false;
        for spec in specifiers {
            if !matches!(spec.node, c::DeclarationSpecifier::Extension(_)) {
                record_attributes = matches!(&spec.node, c::DeclarationSpecifier::TypeSpecifier(t) if matches!(t.node, c::TypeSpecifier::Struct(_) | c::TypeSpecifier::Enum(_)));
            }
            match &spec.node {
                c::DeclarationSpecifier::TypeSpecifier(t) => {
                    let ty = self.type_specifier(t)?;
                    match (&mut result.base, ty) {
                        (Node::IdentifierType { names }, Node::IdentifierType { names: other }) => names.extend(other),
                        (slot, ty) => *slot = ty,
                    }
                }
                c::DeclarationSpecifier::TypeQualifier(q) => result.quals.push(qualifier(&q.node).into()),
                c::DeclarationSpecifier::StorageClass(s) => result.storage.push(match s.node { c::StorageClassSpecifier::Typedef => "typedef", c::StorageClassSpecifier::Extern => "extern", c::StorageClassSpecifier::Static => "static", c::StorageClassSpecifier::ThreadLocal => "_Thread_local", c::StorageClassSpecifier::Auto => "auto", c::StorageClassSpecifier::Register => "register" }.into()),
                c::DeclarationSpecifier::Function(f) => result.funcspec.push(match f.node { c::FunctionSpecifier::Inline => "inline", c::FunctionSpecifier::Noreturn => "_Noreturn" }.into()),
                c::DeclarationSpecifier::Alignment(a) => result.align.push(Node::Alignas { alignment: match &a.node { c::AlignmentSpecifier::Type(t) => self.typename(&t.node)?.boxed(), c::AlignmentSpecifier::Constant(e) => self.expression(e)?.boxed() } }),
                c::DeclarationSpecifier::Extension(e) => {
                    let (attrs, label) = self.extensions(e)?;
                    let mut ordinary = Vec::new();
                    for attr in attrs {
                        if let Node::Attribute { name, args: Some(args) } = &attr {
                            if name == &format!("{}align", self.prefix) {
                                let Node::UnaryOp { op, expr } = &args[0] else { unreachable!() };
                                assert_eq!(op, "sizeof");
                                result.align.push(Node::Alignas { alignment: expr.clone() });
                                continue;
                            }
                        }
                        ordinary.push(attr);
                    }
                    if record_attributes {
                        match &mut result.base {
                            Node::Struct { attributes, .. } | Node::Union { attributes, .. } | Node::Enum { attributes, .. } => attributes.extend(ordinary),
                            _ => unreachable!(),
                        }
                    } else {
                        result.attributes.extend(ordinary);
                    }
                    if label.is_some() { result.asmlabel = label; }
                }
            }
        }
        if let Node::IdentifierType { names } = &mut result.base { if names.is_empty() { names.push("int".into()); } }
        Ok(result)
    }

    fn qualifiers(&mut self, qualifiers: &[Spanned<c::SpecifierQualifier>]) -> Result<Specs> {
        let specs: Vec<_> = qualifiers.iter().map(|q| Spanned::new(match &q.node {
            c::SpecifierQualifier::TypeSpecifier(t) => c::DeclarationSpecifier::TypeSpecifier(t.clone()),
            c::SpecifierQualifier::TypeQualifier(t) => c::DeclarationSpecifier::TypeQualifier(t.clone()),
            c::SpecifierQualifier::Extension(e) => c::DeclarationSpecifier::Extension(e.clone()),
        }, q.span)).collect();
        self.specifiers(&specs)
    }

    fn type_specifier(&mut self, t: &Spanned<c::TypeSpecifier>) -> Result<Node> {
        use c::TypeSpecifier as T;
        let keyword = match &t.node {
            T::Void => "void", T::Char => "char", T::Short => "short", T::Int => "int", T::Long => "long", T::Float => "float", T::Double => "double", T::Signed => "signed", T::Unsigned => "unsigned", T::Bool => "_Bool", T::Complex => "_Complex",
            T::TypedefName(n) => return Ok(Node::IdentifierType { names: vec![if n.node.name == format!("{}int128", self.prefix) { "__int128".into() } else { n.node.name.clone() }] }),
            T::Struct(s) => {
                let mut decls = None;
                if let Some(fields) = &s.node.declarations {
                    let mut declarations = Vec::new();
                    for f in fields {
                        match &f.node {
                            c::StructDeclaration::StaticAssert(a) => declarations.push(self.assertion(&a.node)?),
                            c::StructDeclaration::Field(f) => {
                                let mut specs = self.qualifiers(&f.node.specifiers)?;
                                if f.node.declarators.is_empty() {
                                    if matches!(specs.base, Node::IdentifierType { .. }) {
                                        return Err("unnamed scalar structure members are unsupported".into());
                                    }
                                    declarations.push(self.decl(&specs, None, None, None)?);
                                }
                                self.name_shared_type(&mut specs.base, f.node.declarators.len());
                                for (i, d) in f.node.declarators.iter().enumerate() {
                                    if i > 0 { Self::forward_type(&mut specs.base); }
                                    if let Some(pragma) = d.node.declarator.as_ref().and_then(|d| name(&d.node)).and_then(|n| self.pragmas.get(&n)) {
                                        declarations.push(Node::Pragma { string: pragma.clone() });
                                    } else {
                                        declarations.push(self.decl(&specs, d.node.declarator.as_ref(), None, d.node.bit_width.as_deref())?);
                                    }
                                }
                            }
                        }
                    }
                    decls = Some(declarations);
                }
                let name = s.node.identifier.as_ref().map(|n| n.node.name.clone());
                return Ok(if s.node.kind.node == c::StructKind::Struct { Node::Struct { name, attributes: Vec::new(), decls } } else { Node::Union { name, attributes: Vec::new(), decls } });
            }
            T::Enum(e) => {
                let mut enumerators = Vec::new();
                for en in &e.node.enumerators {
                    let (attributes, _) = self.extensions(&en.node.extensions)?;
                    enumerators.push(Node::Enumerator { name: en.node.identifier.node.name.clone(), attributes, value: self.optional_expression(en.node.expression.as_deref())? });
                }
                let values = self.text(t).contains('{').then(|| Node::EnumeratorList { enumerators }.boxed());
                return Ok(Node::Enum { name: e.node.identifier.as_ref().map(|n| n.node.name.clone()), attributes: Vec::new(), values });
            }
            T::TypeOf(t) => return Ok(Node::Typeof { expr: match &t.node { c::TypeOf::Expression(e) => self.expression(e)?.boxed(), c::TypeOf::Type(t) => self.typename(&t.node)?.boxed() } }),
            T::Atomic(t) => {
                let Node::Typename { ty, .. } = self.typename(&t.node)? else { unreachable!() };
                let mut ty = *ty;
                match &mut ty {
                    Node::TypeDecl { quals, .. } | Node::PtrDecl { quals, .. } => {
                        if !quals.iter().any(|q| q == "_Atomic") { quals.push("_Atomic".into()); }
                    }
                    _ => return Err("_Atomic requires a qualified type".into()),
                }
                return Ok(ty);
            }
            T::TS18661Float(_) => return Err("unsupported C type specifier".into()),
        };
        Ok(Node::IdentifierType { names: vec![keyword.into()] })
    }

    fn name_shared_type(&mut self, ty: &mut Node, count: usize) {
        if count < 2 { return; }
        let name = match ty {
            Node::Struct { name, decls: Some(_), .. } | Node::Union { name, decls: Some(_), .. } | Node::Enum { name, values: Some(_), .. } => name,
            _ => return,
        };
        if name.is_none() { self.anonymous += 1; *name = Some(format!("_PermuterAnon{}", self.anonymous)); }
    }

    fn forward_type(ty: &mut Node) {
        match ty {
            Node::Struct { decls, .. } | Node::Union { decls, .. } => *decls = None,
            Node::Enum { values, .. } => *values = None,
            _ => {}
        }
    }

    fn declaration(&mut self, declaration: &c::Declaration) -> Result<Vec<Node>> {
        let mut specs = self.specifiers(&declaration.specifiers)?;
        if declaration.declarators.is_empty() {
            if !specs.attributes.is_empty() && declaration.specifiers.iter().all(|s| matches!(s.node, c::DeclarationSpecifier::Extension(_))) {
                return Ok(vec![Node::AttributeStatement { attrs: specs.attributes }]);
            }
            return Ok(vec![self.decl(&specs, None, None, None)?]);
        }
        self.name_shared_type(&mut specs.base, declaration.declarators.len());
        let mut base = &mut specs.base;
        while let Node::PtrDecl { ty, .. } | Node::ArrayDecl { ty, .. } | Node::FuncDecl { ty, .. } = base { base = ty; }
        if let Node::TypeDecl { declname, .. } = base { *declname = name(&declaration.declarators[0].node.declarator.node); }
        let mut result = Vec::new();
        for (i, d) in declaration.declarators.iter().enumerate() {
            if let Some(initializer) = &d.node.initializer {
                if let c::Initializer::Expression(expr) = &initializer.node {
                    if let c::Expression::Call(call) = &expr.node {
                        if matches!(&call.node.callee.node, c::Expression::Identifier(id) if id.node.name == format!("{}deferred_binding", self.prefix)) { continue; }
                    }
                }
            }
            if i > 0 { Self::forward_type(&mut specs.base); }
            if let Some(pragma) = name(&d.node.declarator.node).and_then(|n| self.pragmas.get(&n)) {
                result.push(Node::Pragma { string: pragma.clone() });
            } else if let Some(asm) = name(&d.node.declarator.node).and_then(|n| self.global_asm.get(&n)) {
                result.push(asm.clone());
            } else {
                result.push(self.decl(&specs, Some(&d.node.declarator), d.node.initializer.as_ref(), None)?);
            }
        }
        for node in &result { match node {
            Node::Typedef { name, .. } => { self.typedefs.insert(name.clone()); self.shadowed.remove(name); },
            Node::Decl { name: Some(name), .. } => { self.shadowed.insert(self.aliases.get(name).unwrap_or(name).clone()); },
            _ => {},
        } }
        Ok(result)
    }

    fn decl(&mut self, specs: &Specs, declarator: Option<&Spanned<c::Declarator>>, initializer: Option<&Spanned<c::Initializer>>, bitsize: Option<&Spanned<c::Expression>>) -> Result<Node> {
        let name = declarator.and_then(|d| name(&d.node));
        let mut ty = Self::base_type(specs, name.clone());
        let mut attributes = specs.attributes.clone();
        let mut asmlabel = specs.asmlabel.clone();
        if let Some(d) = declarator {
            ty = self.declarator(&d.node, ty)?;
            let (attrs, label) = self.extensions(&d.node.extensions)?;
            attributes.extend(attrs);
            if label.is_some() { asmlabel = label; }
        } else if matches!(specs.base, Node::Struct { .. } | Node::Union { .. } | Node::Enum { .. }) {
            ty = specs.base.clone();
        }
        if specs.storage.iter().any(|s| s == "typedef") {
            return Ok(Node::Typedef { name: name.ok_or("unnamed typedef")?, quals: specs.quals.clone(), storage: specs.storage.clone(), attributes, ty: ty.boxed() });
        }
        let mut quals = specs.quals.clone();
        let mut inner = &ty;
        while let Node::PtrDecl { ty, .. } | Node::ArrayDecl { ty, .. } | Node::FuncDecl { ty, .. } = inner { inner = ty; }
        if matches!(inner, Node::TypeDecl { quals, .. } if quals.iter().any(|q| q == "_Atomic")) && !quals.iter().any(|q| q == "_Atomic") { quals.push("_Atomic".into()); }
        Ok(Node::Decl { name, quals, align: specs.align.clone(), storage: specs.storage.clone(), funcspec: specs.funcspec.clone(), attributes, ty: ty.boxed(), init: initializer.map(|i| self.initializer(&i.node).map(Node::boxed)).transpose()?, bitsize: self.optional_expression(bitsize)?, asmlabel })
    }

    fn base_type(specs: &Specs, name: Option<String>) -> Node {
        if matches!(specs.base, Node::TypeDecl { .. } | Node::PtrDecl { .. } | Node::ArrayDecl { .. } | Node::FuncDecl { .. }) {
            let mut ty = specs.base.clone();
            let mut inner = &mut ty;
            while let Node::PtrDecl { ty, .. } | Node::ArrayDecl { ty, .. } | Node::FuncDecl { ty, .. } = inner { inner = ty; }
            if let Node::TypeDecl { declname, .. } = inner { if declname.is_none() { *declname = name; } }
            ty
        } else {
            Node::TypeDecl { declname: name, quals: specs.quals.clone(), align: specs.align.clone(), ty: specs.base.clone().boxed() }
        }
    }

    fn declarator(&mut self, d: &c::Declarator, mut ty: Node) -> Result<Node> {
        for derived in &d.derived {
            if let c::DerivedDeclarator::Pointer(qs) = &derived.node {
                let mut quals = Vec::new();
                for q in qs { match &q.node {
                    c::PointerQualifier::TypeQualifier(q) => quals.push(qualifier(&q.node).into()),
                    c::PointerQualifier::Extension(_) => return Err("attributes on pointer qualifiers are not supported".into()),
                } }
                ty = Node::PtrDecl { quals, ty: ty.boxed() };
            }
        }
        for derived in d.derived.iter().rev() {
            match &derived.node {
                c::DerivedDeclarator::Pointer(_) => {}
                c::DerivedDeclarator::Array(a) => {
                    let mut dim_quals: Vec<String> = a.node.qualifiers.iter().map(|q| qualifier(&q.node).into()).collect();
                    let dim = match &a.node.size {
                        c::ArraySize::Unknown => None,
                        c::ArraySize::VariableUnknown => Some(Node::id("*").boxed()),
                        c::ArraySize::VariableExpression(e) => Some(self.expression(e)?.boxed()),
                        c::ArraySize::StaticExpression(e) => {
                            let at = self.text(a).find("static").unwrap();
                            let before = a.node.qualifiers.iter().filter(|q| q.span.start < a.span.start + at).count();
                            dim_quals.insert(before, "static".into()); Some(self.expression(e)?.boxed())
                        }
                    };
                    ty = Node::ArrayDecl { ty: ty.boxed(), dim, dim_quals };
                }
                c::DerivedDeclarator::Function(f) => {
                    let mut params = Vec::new();
                    for p in &f.node.parameters {
                        let specs = self.specifiers(&p.node.specifiers)?;
                        let mut decl = self.decl(&specs, p.node.declarator.as_ref(), None, None)?;
                        if let Node::Decl { name: Some(name), ty, .. } = &decl {
                            if self.typedefs.contains(name) && !self.shadowed.contains(name) && p.node.declarator.as_ref().is_some_and(|d| matches!(d.node.kind.node, c::DeclaratorKind::Declarator(_))) {
                                let mut argument_type = *ty.clone();
                                let mut core = &mut argument_type;
                                while let Node::PtrDecl { ty, .. } | Node::ArrayDecl { ty, .. } | Node::FuncDecl { ty, .. } = core { core = ty; }
                                if let Node::TypeDecl { declname, ty, .. } = core { *declname = None; **ty = Node::IdentifierType { names: vec![name.clone()] }; }
                                let argument = Node::Typename { name: None, quals: Vec::new(), align: Vec::new(), attributes: Vec::new(), ty: argument_type.boxed() };
                                decl = Node::Typename { name: None, quals: specs.quals.clone(), align: specs.align.clone(), attributes: specs.attributes.clone(), ty: Node::FuncDecl { args: Some(Node::ParamList { params: vec![argument] }.boxed()), ty: Self::base_type(&specs, None).boxed() }.boxed() };
                            }
                        }
                        params.push(if let Node::Decl { name: None, quals, align, attributes, ty, .. } = decl { Node::Typename { name: None, quals, align, attributes, ty } } else { decl });
                    }
                    if f.node.ellipsis == c::Ellipsis::Some { params.push(Node::EllipsisParam {}); }
                    let args = (!params.is_empty()).then(|| Node::ParamList { params }.boxed());
                    ty = Node::FuncDecl { args, ty: ty.boxed() };
                }
                c::DerivedDeclarator::KRFunction(ids) => {
                    let params: Vec<_> = ids.iter().map(|i| Node::id(&i.node.name)).collect();
                    ty = Node::FuncDecl { args: (!params.is_empty()).then(|| Node::ParamList { params }.boxed()), ty: ty.boxed() };
                }
                c::DerivedDeclarator::Block(_) => return Err("Clang blocks are not supported".into()),
            }
        }
        if let c::DeclaratorKind::Declarator(nested) = &d.kind.node { ty = self.declarator(&nested.node, ty)?; }
        Ok(ty)
    }

    fn typename(&mut self, t: &c::TypeName) -> Result<Node> {
        let specs = self.qualifiers(&t.specifiers)?;
        let ty = Self::base_type(&specs, None);
        let ty = if let Some(d) = &t.declarator { self.declarator(&d.node, ty)? } else { ty };
        Ok(Node::Typename { name: None, quals: specs.quals, align: specs.align, attributes: specs.attributes, ty: ty.boxed() })
    }

    fn optional_expression(&mut self, e: Option<&Spanned<c::Expression>>) -> Result<Optional> { e.map(|e| self.expression(e).map(Node::boxed)).transpose() }

    fn initializer(&mut self, i: &c::Initializer) -> Result<Node> {
        match i { c::Initializer::Expression(e) => self.expression(e), c::Initializer::List(items) => self.initializer_list(items) }
    }

    fn initializer_list(&mut self, items: &[Spanned<c::InitializerListItem>]) -> Result<Node> {
        let mut exprs = Vec::new();
        for item in items {
            let expr = self.initializer(&item.node.initializer.node)?;
            if item.node.designation.is_empty() { exprs.push(expr); } else {
                let mut name = Vec::new();
                for d in &item.node.designation { name.push(match &d.node {
                    c::Designator::Index(e) => self.expression(e)?,
                    c::Designator::Member(id) => Node::id(&id.node.name),
                    c::Designator::Range(r) => Node::Range { first: self.expression(&r.node.from)?.boxed(), last: self.expression(&r.node.to)?.boxed() },
                }); }
                exprs.push(Node::NamedInitializer { name, expr: expr.boxed() });
            }
        }
        Ok(Node::InitList { exprs })
    }

    fn assertion(&mut self, a: &c::StaticAssert) -> Result<Node> {
        let message = self.string(&a.message.node);
        let message = if matches!(&message, Node::Constant { value, .. } if value == &format!("\"{}assert\"", self.prefix)) { None } else { Some(message.boxed()) };
        Ok(Node::StaticAssert { cond: self.expression(&a.expression)?.boxed(), message })
    }

    fn statement(&mut self, s: &c::Statement) -> Result<Node> {
        use c::Statement as S;
        Ok(match s {
            S::Compound(items) => {
                let typedefs = self.typedefs.clone();
                let shadowed = self.shadowed.clone();
                let mut block_items = Vec::new();
                for item in items { match &item.node {
                    c::BlockItem::Declaration(d) => block_items.extend(self.declaration(&d.node)?),
                    c::BlockItem::StaticAssert(a) => { block_items.push(self.assertion(&a.node)?); block_items.push(Node::EmptyStatement {}); },
                    c::BlockItem::Statement(s) => block_items.push(self.statement(&s.node)?),
                } }
                self.typedefs = typedefs;
                self.shadowed = shadowed;
                Node::Compound { block_items }
            }
            S::Expression(None) => Node::EmptyStatement {},
            S::Expression(Some(e)) => {
                let node = self.expression(e)?;
                if let Node::FuncCall { name, args: None } = &node {
                    if let Node::Id { name } = name.as_ref() {
                        if let Some(string) = self.pragmas.get(name) { return Ok(Node::Pragma { string: string.clone() }); }
                    }
                }
                node
            }
            S::If(i) => Node::If { cond: self.expression(&i.node.condition)?.boxed(), iftrue: self.statement(&i.node.then_statement.node)?.boxed(), iffalse: i.node.else_statement.as_ref().map(|s| self.statement(&s.node).map(Node::boxed)).transpose()? },
            S::Switch(s) => {
                let mut stmt = self.statement(&s.node.statement.node)?;
                fix_switch_cases(&mut stmt);
                Node::Switch { cond: self.expression(&s.node.expression)?.boxed(), stmt: stmt.boxed() }
            }
            S::While(s) => Node::While { cond: self.expression(&s.node.expression)?.boxed(), stmt: self.statement(&s.node.statement.node)?.boxed() },
            S::DoWhile(s) => Node::DoWhile { cond: self.expression(&s.node.expression)?.boxed(), stmt: self.statement(&s.node.statement.node)?.boxed() },
            S::For(f) => {
                let init = match &f.node.initializer.node {
                    c::ForInitializer::Empty => None,
                    c::ForInitializer::Expression(e) => Some(self.expression(e)?.boxed()),
                    c::ForInitializer::Declaration(d) => Some(Node::DeclList { decls: self.declaration(&d.node)? }.boxed()),
                    c::ForInitializer::StaticAssert(a) => Some(self.assertion(&a.node)?.boxed()),
                };
                Node::For { init, cond: self.optional_expression(f.node.condition.as_deref())?, next: self.optional_expression(f.node.step.as_deref())?, stmt: self.statement(&f.node.statement.node)?.boxed() }
            }
            S::Labeled(l) => {
                let stmt = self.statement(&l.node.statement.node)?;
                match &l.node.label.node {
                    c::Label::Identifier(i) => {
                        if let Some(pragma) = self.pragmas.get(&i.node.name) {
                            let mut block_items = vec![Node::Pragma { string: pragma.clone() }];
                            let next_is_pragma = matches!(&l.node.statement.node, S::Labeled(next) if matches!(&next.node.label.node, c::Label::Identifier(id) if self.pragmas.contains_key(&id.node.name)));
                            if next_is_pragma {
                                let Node::Compound { block_items: rest } = stmt else { unreachable!() };
                                block_items.extend(rest);
                            } else { block_items.push(stmt); }
                            Node::Compound { block_items }
                        } else { Node::Label { name: i.node.name.clone(), stmt: stmt.boxed() } }
                    }
                    c::Label::Case(e) => Node::Case { expr: self.expression(e)?.boxed(), stmts: vec![stmt] },
                    c::Label::CaseRange(r) => Node::Case { expr: Node::Range { first: self.expression(&r.node.low)?.boxed(), last: self.expression(&r.node.high)?.boxed() }.boxed(), stmts: vec![stmt] },
                    c::Label::Default => Node::Default { stmts: vec![stmt] },
                }
            }
            S::Goto(id) => Node::Goto { name: id.node.name.clone() },
            S::Continue => Node::Continue {}, S::Break => Node::Break {},
            S::Return(e) => Node::Return { expr: self.optional_expression(e.as_deref())? },
            S::Asm(a) => match &a.node {
                c::AsmStatement::GnuBasic(s) => Node::Asm { quals: Vec::new(), asm: self.string(&s.node).boxed(), outputs: Vec::new(), inputs: Vec::new(), clobbers: Vec::new(), gotos: Vec::new() },
                c::AsmStatement::GnuExtended(a) => {
                    let outputs = a.outputs.iter().map(|o| self.asm_operand(&o.node)).collect::<Result<_>>()?;
                    let inputs = a.inputs.iter().map(|o| self.asm_operand(&o.node)).collect::<Result<_>>()?;
                    Node::Asm { quals: a.qualifier.iter().map(|q| qualifier(&q.node).into()).collect(), asm: self.string(&a.template.node).boxed(), outputs, inputs, clobbers: a.clobbers.iter().map(|s| self.string(&s.node)).collect(), gotos: Vec::new() }
                }
            },
        })
    }

    fn asm_operand(&mut self, a: &c::GnuAsmOperand) -> Result<Node> {
        Ok(Node::AsmOperand { symbolic_name: a.symbolic_name.as_ref().map(|i| i.node.name.clone()), constraint: self.string(&a.constraints.node).boxed(), expr: self.expression(&a.variable_name)?.boxed() })
    }
}

fn fix_switch_cases(stmt: &mut Node) {
    let Node::Compound { block_items } = stmt else { return };
    let mut result = Vec::new();
    let mut last_case = None;
    for mut child in std::mem::take(block_items) {
        if matches!(child, Node::Case { .. } | Node::Default { .. }) {
            loop {
                let stmts = match &mut child { Node::Case { stmts, .. } | Node::Default { stmts } => stmts, _ => unreachable!() };
                let nested = if stmts.first().is_some_and(|n| matches!(n, Node::Case { .. } | Node::Default { .. })) { stmts.pop() } else { None };
                result.push(child);
                if let Some(next) = nested { child = next; } else { break; }
            }
            last_case = Some(result.len() - 1);
        } else if let Some(index) = last_case {
            match &mut result[index] { Node::Case { stmts, .. } | Node::Default { stmts } => stmts.push(child), _ => unreachable!() }
        } else { result.push(child); }
    }
    *block_items = result;
}
