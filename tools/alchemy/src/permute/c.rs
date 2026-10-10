//! C translation-unit tree for candidate generation.
//! Mutation numbering visits scalar children before lists in declaration order.
//! Alignment and enumerator attributes are metadata, outside that traversal.
//! The parser adapts lang-c; candidates use this tree's deterministic printer.

mod parser;
mod print;
pub(super) mod types;

pub use parser::parse;

trait Edges {
    fn edges<'a>(&'a self, single: &mut Vec<&'a Node>, lists: &mut Vec<&'a Node>);
    fn edges_mut<'a>(&'a mut self, single: &mut Vec<&'a mut Node>, lists: &mut Vec<&'a mut Node>);
}

impl Edges for String {
    fn edges<'a>(&'a self, _: &mut Vec<&'a Node>, _: &mut Vec<&'a Node>) {}
    fn edges_mut<'a>(&'a mut self, _: &mut Vec<&'a mut Node>, _: &mut Vec<&'a mut Node>) {}
}

impl<T: Edges> Edges for Option<T> {
    fn edges<'a>(&'a self, single: &mut Vec<&'a Node>, lists: &mut Vec<&'a Node>) {
        if let Some(value) = self { value.edges(single, lists); }
    }
    fn edges_mut<'a>(&'a mut self, single: &mut Vec<&'a mut Node>, lists: &mut Vec<&'a mut Node>) {
        if let Some(value) = self { value.edges_mut(single, lists); }
    }
}

impl Edges for Vec<Node> {
    fn edges<'a>(&'a self, _: &mut Vec<&'a Node>, lists: &mut Vec<&'a Node>) { lists.extend(self); }
    fn edges_mut<'a>(&'a mut self, _: &mut Vec<&'a mut Node>, lists: &mut Vec<&'a mut Node>) { lists.extend(self); }
}

impl Edges for Vec<String> {
    fn edges<'a>(&'a self, _: &mut Vec<&'a Node>, _: &mut Vec<&'a Node>) {}
    fn edges_mut<'a>(&'a mut self, _: &mut Vec<&'a mut Node>, _: &mut Vec<&'a mut Node>) {}
}

impl Edges for Box<Node> {
    fn edges<'a>(&'a self, single: &mut Vec<&'a Node>, _: &mut Vec<&'a Node>) { single.push(self); }
    fn edges_mut<'a>(&'a mut self, single: &mut Vec<&'a mut Node>, _: &mut Vec<&'a mut Node>) { single.push(self); }
}

macro_rules! nodes {
    ($($kind:ident { $($field:ident: $ty:ty),* $(,)? }),* $(,)?) => {
        #[derive(Clone, Debug, PartialEq, Eq)]
        pub enum Node { $($kind { $($field: $ty),* }),* }

        impl Node {
            #[cfg(test)]
            fn kind(&self) -> &'static str {
                match self { $(Self::$kind { .. } => stringify!($kind)),* }
            }
            pub fn children(&self) -> Vec<&Node> {
                let mut result = Vec::new();
                let mut lists = Vec::new();
                // Enumerator attributes remain metadata, outside traversal.
                if let Self::Enumerator { value, .. } = self {
                    value.edges(&mut result, &mut lists);
                    return result;
                }
                match self { $(Self::$kind { $($field),* } => {
                    $(if stringify!($field) != "align" { $field.edges(&mut result, &mut lists); })*
                }),* }
                result.extend(lists);
                result
            }
            pub fn children_mut(&mut self) -> Vec<&mut Node> {
                let mut result = Vec::new();
                let mut lists = Vec::new();
                if let Self::Enumerator { value, .. } = self {
                    value.edges_mut(&mut result, &mut lists);
                    return result;
                }
                match self { $(Self::$kind { $($field),* } => {
                    $(if stringify!($field) != "align" { $field.edges_mut(&mut result, &mut lists); })*
                }),* }
                result.extend(lists);
                result
            }
        }
    };
}

pub type Child = Box<Node>;
pub type Optional = Option<Child>;

nodes! {
    ArrayDecl { ty: Child, dim: Optional, dim_quals: Vec<String> },
    ArrayRef { name: Child, subscript: Child },
    Asm { quals: Vec<String>, asm: Child, outputs: Vec<Node>, inputs: Vec<Node>, clobbers: Vec<Node>, gotos: Vec<String> },
    AsmOperand { symbolic_name: Option<String>, constraint: Child, expr: Child },
    Assignment { op: String, lvalue: Child, rvalue: Child },
    Alignas { alignment: Child },
    BinaryOp { op: String, left: Child, right: Child },
    Break {},
    Case { expr: Child, stmts: Vec<Node> },
    Cast { to_type: Child, expr: Child },
    Compound { block_items: Vec<Node> },
    CompoundLiteral { ty: Child, init: Child },
    Constant { ty: String, value: String },
    Continue {},
    Decl { name: Option<String>, quals: Vec<String>, align: Vec<Node>, storage: Vec<String>, funcspec: Vec<String>, attributes: Vec<Node>, ty: Child, init: Optional, bitsize: Optional, asmlabel: Optional },
    DeclList { decls: Vec<Node> },
    Default { stmts: Vec<Node> },
    DoWhile { cond: Child, stmt: Child },
    EllipsisParam {},
    EmptyStatement {},
    Enum { name: Option<String>, attributes: Vec<Node>, values: Optional },
    Enumerator { name: String, attributes: Vec<Node>, value: Optional },
    EnumeratorList { enumerators: Vec<Node> },
    ExprList { exprs: Vec<Node> },
    FileAst { ext: Vec<Node> },
    For { init: Optional, cond: Optional, next: Optional, stmt: Child },
    FuncCall { name: Child, args: Optional },
    FuncDecl { args: Optional, ty: Child },
    FuncDef { decl: Child, param_decls: Vec<Node>, body: Child },
    Attribute { name: String, args: Option<Vec<Node>> },
    AttributeStatement { attrs: Vec<Node> },
    Goto { name: String },
    Id { name: String },
    IdentifierType { names: Vec<String> },
    If { cond: Child, iftrue: Child, iffalse: Optional },
    InitList { exprs: Vec<Node> },
    Label { name: String, stmt: Child },
    NamedInitializer { name: Vec<Node>, expr: Child },
    ParamList { params: Vec<Node> },
    PtrDecl { quals: Vec<String>, ty: Child },
    Range { first: Child, last: Child },
    Return { expr: Optional },
    StaticAssert { cond: Child, message: Optional },
    Struct { name: Option<String>, attributes: Vec<Node>, decls: Option<Vec<Node>> },
    StructRef { name: Child, op: String, field: Child },
    Switch { cond: Child, stmt: Child },
    TernaryOp { cond: Child, iftrue: Child, iffalse: Child },
    TypeDecl { declname: Option<String>, quals: Vec<String>, align: Vec<Node>, ty: Child },
    Typedef { name: String, quals: Vec<String>, storage: Vec<String>, attributes: Vec<Node>, ty: Child },
    Typename { name: Option<String>, quals: Vec<String>, align: Vec<Node>, attributes: Vec<Node>, ty: Child },
    Typeof { expr: Child },
    UnaryOp { op: String, expr: Child },
    Union { name: Option<String>, attributes: Vec<Node>, decls: Option<Vec<Node>> },
    While { cond: Child, stmt: Child },
    Pragma { string: String },
}

impl Node {
    pub fn boxed(self) -> Child { Box::new(self) }

    pub fn id(name: impl Into<String>) -> Self { Self::Id { name: name.into() } }

    pub fn print(&self) -> String { print::Generator::default().visit(self) }

    /// Keep unrelated declarations as prototypes; normalize only the target.
    pub fn extract_function(&mut self, name: &str) -> Result<usize, String> {
        let Self::FileAst { ext } = self else { return Err("expected a C translation unit".into()); };
        let mut index = None;
        for (i, item) in ext.iter_mut().enumerate() {
            if let Self::FuncDef { decl, .. } = item {
                if let Self::Decl { name: Some(n), funcspec, .. } = decl.as_ref() {
                    if n == name {
                        if index.replace(i).is_some() { return Err(format!("multiple definitions of {name}")); }
                        item.normalize();
                    } else if !funcspec.iter().any(|s| s == "inline") {
                        *item = *decl.clone();
                    }
                }
            }
            if let Self::Decl { storage, .. } = item { storage.retain(|s| s != "static"); }
        }
        index.ok_or_else(|| format!("function {name} not found"))
    }

    fn normalize(&mut self) {
        fn brace(stmt: &mut Child) {
            if !matches!(stmt.as_ref(), Node::Compound { .. } | Node::Case { .. } | Node::Default { .. }) {
                let old = std::mem::replace(stmt.as_mut(), Node::EmptyStatement {});
                **stmt = Node::Compound { block_items: vec![old] };
            }
        }
        match self {
            Self::FuncDef { body, .. } => body.normalize(),
            Self::Compound { .. } => {
                for child in self.children_mut() { child.normalize(); }
            }
            Self::Case { stmts, .. } | Self::Default { stmts } => {
                for stmt in stmts { stmt.normalize(); }
            }
            Self::For { stmt, .. } | Self::While { stmt, .. } | Self::DoWhile { stmt, .. } | Self::Switch { stmt, .. } => {
                brace(stmt);
                stmt.normalize();
            }
            Self::If { iftrue, iffalse, .. } => {
                brace(iftrue);
                iftrue.normalize();
                if let Some(stmt) = iffalse { brace(stmt); stmt.normalize(); }
            }
            Self::Label { stmt, .. } => stmt.normalize(),
            _ => {}
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn decode(text: &str) -> String {
        let mut result = String::new();
        let mut chars = text.chars();
        while let Some(c) = chars.next() {
            result.push(if c == '\\' { match chars.next().unwrap() { 'n' => '\n', 'r' => '\r', 't' => '\t', '\\' => '\\', other => panic!("unexpected escape {other}") } } else { c });
        }
        result
    }

    #[test]
    fn independent_parser_invalid_inputs_are_rejected() {
        let mut accepted = Vec::new();
        for row in include_str!("c/fixtures/parser-invalid.tsv").lines() {
            let c: Vec<_> = row.split('\t').map(decode).collect();
            if let Ok(tree) = parse(&c[1]) { accepted.push(format!("{}\nsource {}\nparsed {}", c[0], c[1], tree.print())); }
        }
        assert!(accepted.is_empty(), "{} unexpected acceptances\n{}", accepted.len(), accepted.join("\n"));
    }

    #[test]
    fn parse_error_shows_the_preprocessed_line_that_failed() {
        let error = parse("int F(void) {\n    return +;\n}\n").unwrap_err();
        assert!(error.contains("C parse error at 2:"), "{error}");
        assert!(error.ends_with("\n    return +;"), "{error}");
    }

    #[test]
    fn empty_for_clauses_survive_an_enclosing_struct_cast() {
        let source = "struct S { int x; }; void F(void *p) { if (((struct S *)p)->x) { for (;;) { break; } } }";
        assert!(parse(source).unwrap().print().contains("for (;;)"));
        assert!(parse(&source.replace("for (;;)", "for (;)")).is_err());
    }

    #[test]
    fn adapter_and_printer_failures_do_not_become_valid_candidates() {
        for row in include_str!("c/fixtures/parser-unsupported.tsv").lines() {
            let c: Vec<_> = row.split('\t').map(decode).collect();
            assert!(parse(&c[1]).is_err(), "{}: unsupported input {}", c[0], c[2]);
        }
    }

    #[test]
    fn offsetof_bridge_does_not_accept_the_unsupported_builtin() {
        assert!(parse("int f(void) { return __builtin_offsetof(struct S, p); }").is_err());
        let source = "int f(void) { return offsetof(struct S, p); }";
        assert!(parse(source).unwrap().print().contains("offsetof(struct S, p)"));
    }

    #[test]
    fn independent_parser_inputs_match_printing_and_traversal() {
        let mut failures = Vec::new();
        fn topology(node: &Node, out: &mut String) {
            out.push_str(match node.kind() { "FileAst" => "FileAST", "Id" => "ID", "Attribute" => "GccAttribute", "AttributeStatement" => "GccAttributeStatement", name => name });
            if let Node::Constant { ty, .. } = node { out.push('\t'); out.push_str(ty); }
            out.push('\n');
            for child in node.children() { topology(child, out); }
        }
        for (index, row) in include_str!("c/fixtures/parser-cases.tsv").lines().enumerate() {
            let c: Vec<_> = row.split('\t').map(decode).collect();
            match parse(&c[1]) {
                Ok(tree) if tree.print() == c[2] => {
                    let mut nodes = String::new();
                    topology(&tree, &mut nodes);
                    if nodes != c[3] { failures.push(format!("row {index} {}\nsource {}\nactual topology {}\nexpected topology {}", c[0], c[1], nodes, c[3])); }
                },
                Ok(tree) => failures.push(format!("row {index} {}\nsource {}\nactual {}\nexpected {}", c[0], c[1], tree.print(), c[2])),
                Err(error) => failures.push(format!("row {index} {}\nsource {}\nparse error {error}", c[0], c[1])),
            }
        }
        assert!(failures.is_empty(), "{} discrepancies\n{}", failures.len(), failures.join("\n"));
    }

    #[test]
    fn translation_unit_fixtures() {
        for (source, expected, nodes) in [
            (include_str!("c/fixtures/declarations.c"), include_str!("c/fixtures/declarations.expected"), include_str!("c/fixtures/declarations.nodes")),
            (include_str!("c/fixtures/statements.c"), include_str!("c/fixtures/statements.expected"), include_str!("c/fixtures/statements.nodes")),
            (include_str!("c/fixtures/expressions.c"), include_str!("c/fixtures/expressions.expected"), include_str!("c/fixtures/expressions.nodes")),
            (include_str!("c/fixtures/sizeof.c"), include_str!("c/fixtures/sizeof.expected"), include_str!("c/fixtures/sizeof.nodes")),
        ] {
            let mut unit = parse(source).unwrap();
            unit.extract_function("target").unwrap();
            assert_eq!(unit.print(), expected.replace("\\x20\n", " \n").replace("\\x0a\n", "\n"));
            fn topology(node: &Node, out: &mut String) {
                out.push_str(match node.kind() { "FileAst" => "FileAST", "Id" => "ID", "Attribute" => "GccAttribute", "AttributeStatement" => "GccAttributeStatement", name => name });
                if let Node::Constant { ty, .. } = node { out.push('\t'); out.push_str(ty); }
                out.push('\n');
                for child in node.children() { topology(child, out); }
            }
            let mut actual_nodes = String::new();
            topology(&unit, &mut actual_nodes);
            assert_eq!(actual_nodes, nodes);
            let mut reparsed = parse(&unit.print()).unwrap();
            reparsed.extract_function("target").unwrap();
            assert_eq!(reparsed, unit);
        }
    }

    #[test]
    fn traversal_matches_field_order() {
        let unit = parse("int target(int n) { for (n = 1; n < 4; n++) n += 2; }").unwrap();
        fn collect(node: &Node, names: &mut Vec<String>) {
            if let Node::Id { name } = node { names.push(name.clone()); }
            if let Node::Constant { value, .. } = node { names.push(value.clone()); }
            for child in node.children() { collect(child, names); }
        }
        let mut order = Vec::new();
        collect(&unit, &mut order);
        assert_eq!(order, ["n", "1", "n", "4", "n", "n", "2"]);
        let Node::FileAst { ext } = unit else { panic!() };
        let Node::FuncDef { decl, .. } = &ext[0] else { panic!() };
        let Node::Decl { ty, .. } = decl.as_ref() else { panic!() };
        assert!(matches!(ty.children()[0], Node::ParamList { .. }));
        assert!(matches!(ty.children()[1], Node::TypeDecl { .. }));
    }
}
