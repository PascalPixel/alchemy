mod json;

use json::{dumps, Json};
use std::collections::BTreeSet;
use std::fs;
use std::path::Path;
use tree_sitter::{Node, Parser};

#[derive(Debug)]
struct Statement {
    line: usize,
    end_line: usize,
    kind: String,
    anchor: bool,
    governed_by: Option<usize>,
    writes: BTreeSet<String>,
    reads: BTreeSet<String>,
}

fn text<'a>(node: Node<'_>, source: &'a [u8]) -> &'a str {
    std::str::from_utf8(&source[node.byte_range()]).unwrap_or("")
}

fn identifier(node: Node<'_>, source: &[u8]) -> Option<String> {
    if node.kind() == "identifier" {
        return Some(text(node, source).to_string());
    }
    let mut cursor = node.walk();
    for child in node.named_children(&mut cursor) {
        if let Some(name) = identifier(child, source) {
            return Some(name);
        }
    }
    None
}

fn effects(node: Node<'_>, source: &[u8], writes: &mut BTreeSet<String>, reads: &mut BTreeSet<String>) {
    match node.kind() {
        "assignment_expression" => {
            if let Some(left) = node.child_by_field_name("left") {
                if left.kind() == "identifier" {
                    let name = text(left, source).to_string();
                    writes.insert(name.clone());
                    let operator = node.child_by_field_name("operator")
                        .map(|item| text(item, source))
                        .unwrap_or("=");
                    if operator != "=" {
                        reads.insert(name);
                    }
                } else {
                    effects(left, source, writes, reads);
                }
            }
            if let Some(right) = node.child_by_field_name("right") {
                effects(right, source, writes, reads);
            }
            return;
        }
        "update_expression" => {
            if let Some(argument) = node.named_child(0) {
                if argument.kind() == "identifier" {
                    let name = text(argument, source).to_string();
                    writes.insert(name.clone());
                    reads.insert(name);
                    return;
                }
            }
        }
        "declaration" => {
            let mut cursor = node.walk();
            for child in node.named_children(&mut cursor) {
                if matches!(child.kind(), "init_declarator" | "identifier" | "pointer_declarator" | "array_declarator") {
                    if let Some(name) = identifier(child, source) {
                        writes.insert(name);
                    }
                    if child.kind() == "init_declarator" {
                        if let Some(value) = child.child_by_field_name("value") {
                            effects(value, source, writes, reads);
                        }
                    }
                }
            }
            return;
        }
        "identifier" => {
            reads.insert(text(node, source).to_string());
            return;
        }
        _ => {}
    }
    let mut cursor = node.walk();
    for child in node.named_children(&mut cursor) {
        effects(child, source, writes, reads);
    }
}

fn is_anchor(kind: &str) -> bool {
    matches!(kind, "labeled_statement" | "goto_statement" | "return_statement" |
        "break_statement" | "continue_statement" | "case_statement")
}

fn is_control(kind: &str) -> bool {
    matches!(kind, "if_statement" | "for_statement" | "while_statement" |
        "do_statement" | "switch_statement")
}

fn children_of_block(node: Node<'_>) -> Vec<Node<'_>> {
    let mut cursor = node.walk();
    node.named_children(&mut cursor)
        .filter(|child| !matches!(child.kind(), "comment" | "preproc_if" | "preproc_def" | "preproc_include"))
        .collect()
}

fn semantic_end_line(node: Node<'_>) -> usize {
    if matches!(node.kind(), "compound_statement" | "else_clause") {
        let mut cursor = node.walk();
        return node.named_children(&mut cursor)
            .map(semantic_end_line)
            .max()
            .unwrap_or(node.start_position().row + 1);
    }
    if is_control(node.kind()) {
        let mut cursor = node.walk();
        return node.named_children(&mut cursor)
            .filter(|child| child.kind() != "comment")
            .map(semantic_end_line)
            .max()
            .unwrap_or(node.start_position().row + 1);
    }
    node.end_position().row + 1
}

fn collect(block: Node<'_>, governed_by: Option<usize>, source: &[u8], out: &mut Vec<Statement>) {
    let items = if block.kind() == "compound_statement" || block.kind() == "case_statement" {
        children_of_block(block)
    } else {
        vec![block]
    };
    for item in items {
        if item.kind() == "else_clause" {
            if let Some(body) = item.named_child(0) {
                let owner = if body.kind() == "compound_statement" { None } else { governed_by };
                collect(body, owner, source, out);
            }
            continue;
        }
        if block.kind() == "case_statement" && item == block.named_child(0).unwrap_or(item) {
            continue;
        }
        let line = item.start_position().row + 1;
        let mut writes = BTreeSet::new();
        let mut reads = BTreeSet::new();
        effects(item, source, &mut writes, &mut reads);
        let control = is_control(item.kind());
        out.push(Statement {
            line,
            end_line: semantic_end_line(item),
            kind: item.kind().to_string(),
            anchor: is_anchor(item.kind()) || control,
            governed_by,
            writes,
            reads,
        });

        if item.kind() == "case_statement" {
            collect(item, None, source, out);
            continue;
        }
        if control {
            for field in ["consequence", "alternative", "body"] {
                if let Some(body) = item.child_by_field_name(field) {
                    let body = if body.kind() == "else_clause" {
                        body.named_child(0).unwrap_or(body)
                    } else {
                        body
                    };
                    let owner = if body.kind() == "compound_statement" { None } else { Some(line) };
                    collect(body, owner, source, out);
                }
            }
        }
    }
}

fn find_function<'a>(node: Node<'a>, source: &[u8], wanted: &str) -> Option<Node<'a>> {
    if node.kind() == "function_definition" {
        if let Some(declarator) = node.child_by_field_name("declarator") {
            if identifier(declarator, source).as_deref() == Some(wanted) {
                return Some(node);
            }
        }
    }
    let mut cursor = node.walk();
    for child in node.named_children(&mut cursor) {
        if let Some(found) = find_function(child, source, wanted) {
            return Some(found);
        }
    }
    None
}

fn analyse(path: &Path, function: &str) -> Result<Json, String> {
    let source = fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?;
    let mut parser = Parser::new();
    parser.set_language(&tree_sitter_c::LANGUAGE.into())
        .map_err(|error| format!("C parser: {error}"))?;
    let tree = parser.parse(&source, None).ok_or("C parser returned no tree")?;
    if tree.root_node().has_error() {
        return Err(format!("could not parse {}", path.display()));
    }
    let function_node = find_function(tree.root_node(), &source, function)
        .ok_or_else(|| format!("function not found: {function}"))?;
    let body = function_node.child_by_field_name("body").ok_or("function has no body")?;
    let mut statements = Vec::new();
    collect(body, None, &source, &mut statements);
    Ok(Json::Obj(vec![
        ("function".into(), Json::Str(function.into())),
        ("statements".into(), Json::Arr(statements.into_iter().map(statement_json).collect())),
        ("declarations".into(), Json::Obj(Vec::new())),
    ]))
}

fn statement_json(item: Statement) -> Json {
    Json::Obj(vec![
        ("line".into(), Json::Int(item.line as i64)),
        ("end_line".into(), Json::Int(item.end_line as i64)),
        ("kind".into(), Json::Str(item.kind)),
        ("anchor".into(), Json::Bool(item.anchor)),
        ("governed_by".into(), item.governed_by.map_or(Json::Null, |line| Json::Int(line as i64))),
        ("writes".into(), Json::Arr(item.writes.into_iter().map(Json::Str).collect())),
        ("reads".into(), Json::Arr(item.reads.into_iter().map(Json::Str).collect())),
    ])
}

fn self_test() -> Result<(), String> {
    let source = b"void Func_02000500(void) { int a; a = 1; if (a) a = 2; if (a) return; }";
    let path = std::env::temp_dir().join(format!("alchemy-c-structure-{}.c", std::process::id()));
    fs::write(&path, source).map_err(|error| error.to_string())?;
    let result = analyse(&path, "Func_02000500");
    let _ = fs::remove_file(path);
    let rendered = dumps(&result?);
    if !rendered.contains("\"governed_by\": 1") || !rendered.contains("\"return_statement\"") {
        return Err(format!("structural self-test failed: {rendered}"));
    }
    println!("self-test=ok tool=c-structure");
    Ok(())
}

fn main() {
    let args: Vec<String> = std::env::args().skip(1).collect();
    let result = if args.iter().any(|arg| arg == "--self-test") {
        self_test()
    } else if args.len() == 2 {
        match analyse(Path::new(&args[0]), &args[1]) {
            Ok(value) => { println!("{}", dumps(&value)); Ok(()) }
            Err(error) => { println!("{}", dumps(&Json::Obj(vec![("error".into(), Json::Str(error.clone()))]))); Err(error) }
        }
    } else {
        Err("usage: c-structure SOURCE.c FUNCTION | --self-test".into())
    };
    if let Err(error) = result {
        eprintln!("error: {error}");
        std::process::exit(2);
    }
}
