//! Manual-permutation trees and mixed-radix choice enumeration.

use super::ast::{Function, Stmt};
use super::mutate::Rng;
use std::collections::{BTreeMap, HashSet};

const NAMES: &[&str] = &[
    "PERM_GENERAL",
    "PERM_ONEOF",
    "PERM_LINESWAP",
    "PERM_LINESWAP_TEXT",
    "PERM_RANDOMIZE",
    "PERM_IGNORE",
    "PERM_PRETEND",
    "PERM_FORCE_SAMELINE",
    "PERM_INT",
    "PERM_ONCE",
    "PERM_VAR",
];

#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Node {
    Text(String),
    OneOf(Vec<Vec<Node>>),
    LineSwap(Vec<Vec<Node>>),
    LineSwapText(Vec<Vec<Node>>),
    Randomize(Vec<Node>),
    Ignore(Vec<Node>),
    Pretend(Vec<Node>),
    SameLine(Vec<Node>),
    Int(i64, i64),
    Once(String, Vec<Node>),
    Var(Vec<Node>, Option<Vec<Node>>),
}

/// One expansion: the C text with each ignored region as a placeholder
/// call, the placeholders' code, and the randomized regions' text.
#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub struct Expanded {
    pub text: String,
    pub ignored: Vec<(String, String)>,
    pub randomize: Vec<String>,
    ast_swaps: Vec<Vec<usize>>,
    /// Preprocessed and normalized once per manual expansion. Shared by the
    /// search and replay; mutations always work on their own tree.
    pub unit: Option<std::sync::Arc<super::c::Node>>,
}

impl Expanded {
    /// Preserve the control markers through the real C preprocessor.
    pub fn preprocess_text(&self) -> String {
        let mut out = self.text.clone();
        if let Ok(tokens) = super::lex::lex(&self.text) {
            for token in tokens.iter().rev() {
                if let super::lex::Tok::Comment(text) = &token.tok {
                    if let Some(marker) = text.strip_prefix("/* ").and_then(|s| s.strip_suffix(" */")).filter(|s| s.starts_with("_permuter ")) {
                        out.replace_range(token.start..token.end, &format!("\n#pragma {marker}\n"));
                    }
                }
            }
        }
        out
    }

    pub fn key(&self) -> [u8; 32] {
        // AST orders and ignored text can differ while placeholder C is the
        // same. They must not share a prepared tree or a replay baseline.
        super::text_key(&format!("{:?}{:?}{:?}", self.text, self.ignored, self.ast_swaps))
    }

    pub fn apply_c_ast(&self, unit: &mut super::c::Node) {
        use super::c::Node as C;
        fn visit(unit: &mut C, swaps: &[Vec<usize>]) -> bool {
            if let C::Compound { block_items } = unit {
                let order = match block_items.first() {
                    Some(C::Pragma { string }) => string.strip_prefix("_permuter ast ").and_then(|n| n.parse::<usize>().ok()).and_then(|i| swaps.get(i)),
                    _ => None,
                };
                if let Some(order) = order {
                    for item in block_items.iter_mut().skip(1) { visit(item, swaps); }
                    *block_items = order.iter().flat_map(|&i| match &block_items[i + 1] { C::Compound { block_items } => block_items.clone(), _ => unreachable!("line swap arguments are blocks") }).collect();
                    return true;
                }
                let mut out = Vec::new();
                for mut item in std::mem::take(block_items) {
                    if visit(&mut item, swaps) {
                        let C::Compound { block_items } = item else { unreachable!() };
                        out.extend(block_items);
                    } else { out.push(item); }
                }
                *block_items = out;
            } else {
                // An unbraced control body keeps its compound after swapping.
                for child in unit.children_mut() { visit(child, swaps); }
            }
            false
        }
        visit(unit, &self.ast_swaps);
    }

    /// Return preprocessor-preserved markers to the body parser's comments.
    pub fn comment_markers(&self, text: &str) -> String {
        let mut out = text.to_string();
        // Both printers use the same markers; the full tree prints pragmas.
        for (index, _) in self.ignored.iter().enumerate() {
            out = out.replace(&format!("#pragma _permuter ignored {index}"), &format!("/* _permuter ignored {index} */"));
        }
        for kind in ["sameline", "pretend", "randomizer"] {
            for edge in ["start", "end"] {
                out = out.replace(&format!("#pragma _permuter {kind} {edge}"), &format!("/* _permuter {kind} {edge} */"));
            }
        }
        out
    }

    /// The text with every placeholder replaced by its code.
    pub fn restore(&self, text: &str) -> String {
        let mut out = self.comment_markers(text);
        for (placeholder, code) in &self.ignored {
            out = out.replace(placeholder, code);
            let call = placeholder.trim_end_matches(';');
            out = out.replace(call, code.trim_end_matches(';'));
        }
        // Markers survive AST printing, so sameline and pretend take effect
        let (mut same, mut pretend) = (0usize, 0usize);
        let mut result = String::new();
        for line in out.lines() {
            let trimmed = line.trim();
            match trimmed {
                "/* _permuter sameline start */" => same += 1,
                "/* _permuter sameline end */" => same -= 1,
                "/* _permuter pretend start */" => pretend += 1,
                "/* _permuter pretend end */" => pretend -= 1,
                "/* _permuter randomizer start */" | "/* _permuter randomizer end */" => {}
                _ if pretend > 0 && trimmed.starts_with("#pragma _permuter define ") => {
                    result.push('#');
                    result.push_str(trimmed.strip_prefix("#pragma _permuter ").unwrap());
                }
                _ if pretend == 0 => {
                    if same > 0 && !result.ends_with('\n') && !result.is_empty() {
                        result.push(' ');
                        result.push_str(line.trim_start());
                    } else {
                        result.push_str(line);
                    }
                }
                _ => {}
            }
            if same == 0 {
                result.push('\n');
            }
        }
        result
    }

    /// Apply line swaps after parsing each line as a compound statement.
    /// This is observably different from text swapping for declarations,
    /// dangling else, and a macro used as an unbraced control-flow body.
    pub fn apply_ast(&self, function: &mut Function) {
        fn block(stmts: &mut Vec<Stmt>, swaps: &[Vec<usize>]) {
            for stmt in stmts.iter_mut() {
                nested(stmt, swaps);
            }
            let mut out = Vec::new();
            for stmt in std::mem::take(stmts) {
                if let Stmt::Block(mut items) = stmt {
                    if let Some(order) = marker(&items, swaps) {
                        items.remove(0);
                        for &i in order {
                            if let Stmt::Block(inner) = &items[i] {
                                out.extend(inner.clone());
                            }
                        }
                    } else {
                        out.push(Stmt::Block(items));
                    }
                } else {
                    out.push(stmt);
                }
            }
            *stmts = out;
        }
        fn marker<'a>(items: &[Stmt], swaps: &'a [Vec<usize>]) -> Option<&'a Vec<usize>> {
            let Stmt::Comment(text) = items.first()? else { return None };
            let id = text.strip_prefix("/* _permuter ast ")?.strip_suffix(" */")?.parse::<usize>().ok()?;
            swaps.get(id)
        }
        fn nested(stmt: &mut Stmt, swaps: &[Vec<usize>]) {
            match stmt {
                Stmt::Block(items) => {
                    // Leave this block's marker for its parent to flatten.
                    block(items, swaps);
                }
                Stmt::If(_, body, other) => {
                    body_block(body, swaps);
                    if let Some(other) = other {
                        body_block(other, swaps);
                    }
                }
                Stmt::While(_, body) | Stmt::DoWhile(body, _) | Stmt::For(_, _, _, body) | Stmt::Switch(_, body) => body_block(body, swaps),
                _ => {}
            }
        }
        fn body_block(body: &mut Stmt, swaps: &[Vec<usize>]) {
            nested(body, swaps);
            if let Stmt::Block(items) = body {
                if let Some(order) = marker(items, swaps) {
                    let out = order
                        .iter()
                        .flat_map(|&i| match &items[i + 1] {
                            Stmt::Block(inner) => inner.clone(),
                            _ => unreachable!("line swap arguments are blocks"),
                        })
                        .collect();
                    *items = out;
                }
            }
        }
        block(&mut function.body, &self.ast_swaps);
    }
}

/// Byte index just past the parenthesis that closes the one at `open`,
/// skipping strings, characters and comments.
/// Braces and brackets do not nest macro arguments: IGNORE/PRETEND may
/// carry fragments that open or close a C scope outside the macro call.
fn closing(text: &str, open: usize) -> Option<usize> {
    let bytes = text.as_bytes();
    let mut depth = 0usize;
    let mut index = open;
    while index < bytes.len() {
        match bytes[index] {
            b'(' => depth += 1,
            b')' => {
                depth = depth.checked_sub(1)?;
                if depth == 0 {
                    return Some(index + 1);
                }
            }
            quote @ (b'"' | b'\'') => {
                index += 1;
                while index < bytes.len() && bytes[index] != quote {
                    index += if bytes[index] == b'\\' { 2 } else { 1 };
                }
            }
            b'/' if bytes.get(index + 1) == Some(&b'*') => {
                index = text[index + 2..].find("*/").map(|at| index + 2 + at + 1)?;
            }
            b'/' if bytes.get(index + 1) == Some(&b'/') => {
                index = text[index..].find('\n').map_or(bytes.len(), |at| index + at);
            }
            _ => {}
        }
        index += 1;
    }
    None
}

/// The next macro call at or after `from`, outside comments: (start, name).
fn next_macro(text: &str, from: usize) -> Option<(usize, &str)> {
    let bytes = text.as_bytes();
    let mut index = from;
    while index < bytes.len() {
        if bytes[index..].starts_with(b"/*") {
            index = text[index + 2..].find("*/").map_or(bytes.len(), |at| index + 2 + at + 2);
            continue;
        }
        if bytes[index..].starts_with(b"//") {
            index = text[index..].find('\n').map_or(bytes.len(), |at| index + at);
            continue;
        }
        if bytes[index] == b'"' || bytes[index] == b'\'' {
            let quote = bytes[index];
            index += 1;
            while index < bytes.len() && bytes[index] != quote {
                index += if bytes[index] == b'\\' { 2 } else { 1 };
            }
            index += 1;
            continue;
        }
        let boundary = index == 0 || !(bytes[index - 1].is_ascii_alphanumeric() || bytes[index - 1] == b'_');
        if boundary && bytes[index..].starts_with(b"PERM_") {
            let mut end = index + 5;
            while bytes.get(end).is_some_and(|b| b.is_ascii_alphanumeric() || *b == b'_') {
                end += 1;
            }
            if text[end..].trim_start().starts_with('(') {
                return Some((index, &text[index..end]));
            }
        }
        index += 1;
    }
    None
}

/// Whether `text` still uses a PERM macro outside comments and strings.
pub fn has_macros(text: &str) -> bool {
    next_macro(text, 0).is_some()
}

/// Split a macro's arguments at top-level commas.
fn arguments(inner: &str) -> Vec<&str> {
    let bytes = inner.as_bytes();
    let (mut depth, mut start, mut out) = (0i32, 0, Vec::new());
    let mut index = 0;
    while index < bytes.len() {
        match bytes[index] {
            b'(' => depth += 1,
            b')' => depth -= 1,
            b',' if depth == 0 => {
                out.push(&inner[start..index]);
                start = index + 1;
            }
            quote @ (b'"' | b'\'') => {
                index += 1;
                while index < bytes.len() && bytes[index] != quote {
                    index += if bytes[index] == b'\\' { 2 } else { 1 };
                }
            }
            b'/' if bytes.get(index + 1) == Some(&b'*') => {
                index = inner[index + 2..].find("*/").map_or(bytes.len(), |at| index + 2 + at + 1);
            }
            b'/' if bytes.get(index + 1) == Some(&b'/') => {
                index = inner[index..].find('\n').map_or(bytes.len(), |at| index + at);
            }
            _ => {}
        }
        index += 1;
    }
    out.push(&inner[start..]);
    out
}

/// AST swap arguments are lines, but a comment or quoted token may span
/// several lines. Splitting inside it can swallow generated block braces.
fn ast_lines(inner: &str) -> Result<Vec<&str>, String> {
    let tokens = super::lex::lex(inner)?;
    let (mut start, mut token, mut lines) = (0, 0, Vec::new());
    let (mut has_code, mut last_start) = (false, 0);
    for (at, c) in inner.char_indices() {
        if c != '\n' { continue; }
        while token < tokens.len() && tokens[token].end <= at {
            has_code |= !matches!(tokens[token].tok, super::lex::Tok::Comment(_));
            token += 1;
        }
        if tokens.get(token).is_some_and(|t| t.start <= at && at < t.end) { continue; }
        // Comment-only lines belong to the next argument, rather than
        // multiplying the search by permutations of empty blocks.
        if has_code {
            lines.push(&inner[start..at]);
            last_start = start;
            start = at + 1;
            has_code = false;
        }
    }
    has_code |= tokens[token..].iter().any(|t| !matches!(t.tok, super::lex::Tok::Comment(_)));
    if has_code { lines.push(&inner[start..]); }
    else if let Some(last) = lines.last_mut() { *last = &inner[last_start..]; }
    Ok(lines)
}

pub fn parse(text: &str) -> Result<Vec<Node>, String> {
    let nodes = parse_inner(text)?;
    count(&nodes)?;
    Ok(nodes)
}

fn parse_inner(text: &str) -> Result<Vec<Node>, String> {
    let mut nodes = Vec::new();
    let mut at = 0;
    while let Some((start, name)) = next_macro(text, at) {
        if !NAMES.contains(&name) {
            return Err(format!("Unrecognized PERM macro: {name}"));
        }
        if start > at {
            nodes.push(Node::Text(text[at..start].replace("(,)", ",")));
        }
        let open = start + name.len() + text[start + name.len()..].find('(').expect("macro has (");
        let end = closing(text, open).ok_or_else(|| format!("{name}: unbalanced parentheses"))?;
        let inner = &text[open + 1..end - 1];
        nodes.push(match name {
            "PERM_GENERAL" => Node::OneOf(arguments(inner).into_iter().map(parse_inner).collect::<Result<_, _>>()?),
            // Alchemy's older alias trimmed its alternatives.
            "PERM_ONEOF" => Node::OneOf(arguments(inner).into_iter().map(|arg| parse_inner(arg.trim())).collect::<Result<_, _>>()?),
            "PERM_LINESWAP" | "PERM_LINESWAP_TEXT" => {
                let lines = if name == "PERM_LINESWAP" { ast_lines(inner)? } else { inner.lines().filter(|line| !line.trim().is_empty()).collect() };
                let lines = lines.into_iter()
                    .map(parse_inner)
                    .collect::<Result<_, _>>()?;
                if name == "PERM_LINESWAP" {
                    Node::LineSwap(lines)
                } else {
                    Node::LineSwapText(lines)
                }
            }
            "PERM_RANDOMIZE" => Node::Randomize(parse_inner(inner)?),
            "PERM_IGNORE" => Node::Ignore(parse_inner(inner)?),
            "PERM_PRETEND" => Node::Pretend(parse_inner(inner)?),
            "PERM_FORCE_SAMELINE" => Node::SameLine(parse_inner(inner)?),
            "PERM_INT" => {
                let args = arguments(inner);
                if args.len() != 2 {
                    return Err("PERM_INT takes 2 integer arguments".into());
                }
                let low = args[0].trim().parse::<i64>().map_err(|_| "PERM_INT: invalid lower bound")?;
                let high = args[1].trim().parse::<i64>().map_err(|_| "PERM_INT: invalid upper bound")?;
                if low > high {
                    return Err("PERM_INT: lower bound exceeds upper bound".into());
                }
                Node::Int(low, high)
            }
            "PERM_ONCE" | "PERM_VAR" => {
                let args = arguments(inner);
                if !(1..=2).contains(&args.len()) {
                    return Err(format!("{name} takes 1 or 2 arguments"));
                }
                if name == "PERM_ONCE" {
                    Node::Once(args[0].trim().into(), parse_inner(args[args.len() - 1])?)
                } else {
                    Node::Var(parse_inner(args[0])?, args.get(1).map(|v| parse_inner(v)).transpose()?)
                }
            }
            _ => unreachable!(),
        });
        at = end;
    }
    if at < text.len() {
        nodes.push(Node::Text(text[at..].replace("(,)", ",")));
    }
    Ok(nodes)
}

/// Whether any macro offers a choice (alternatives or line orders).
pub fn has_choices(nodes: &[Node]) -> bool {
    count(nodes).is_ok_and(|n| n > 1)
}

pub fn randomizes(nodes: &[Node]) -> bool {
    nodes.iter().any(|node| match node {
        Node::Randomize(_) => true,
        Node::OneOf(options) | Node::LineSwap(options) | Node::LineSwapText(options) => options.iter().any(|o| randomizes(o)),
        Node::Ignore(inner) | Node::Pretend(inner) | Node::SameLine(inner) | Node::Once(_, inner) => randomizes(inner),
        Node::Var(name, value) => randomizes(name) || value.as_ref().is_some_and(|v| randomizes(v)),
        _ => false,
    })
}

/// Expand with the first alternative and the written line order, or with
/// choices drawn from `rng`.
pub fn expand(nodes: &[Node], rng: Option<&mut Rng>) -> Result<Expanded, String> {
    let total = count(nodes)?;
    expand_seed(nodes, rng.map_or(0, |r| r.below(total as usize) as u64))
}

fn product(mut counts: impl Iterator<Item = Result<u64, String>>) -> Result<u64, String> {
    counts.try_fold(1u64, |a, b| a.checked_mul(b?).ok_or_else(|| "PERM search space exceeds 64-bit seeds".into()))
}

fn factorial(n: usize) -> Result<u64, String> {
    product((1..=n as u64).map(Ok))
}
fn inner_count(nodes: &[Node]) -> Result<u64, String> {
    product(nodes.iter().map(node_count))
}
fn node_count(node: &Node) -> Result<u64, String> {
    match node {
        Node::Text(_) => Ok(1),
        Node::Int(low, high) => u64::try_from(i128::from(*high) - i128::from(*low) + 1).map_err(|_| "PERM_INT range exceeds 64-bit seeds".into()),
        Node::OneOf(options) => options.iter().try_fold(0u64, |a, b| {
            a.checked_add(inner_count(b)?).ok_or_else(|| "PERM search space exceeds 64-bit seeds".into())
        }),
        Node::LineSwap(lines) | Node::LineSwapText(lines) => product(std::iter::once(factorial(lines.len())).chain(lines.iter().map(|l| inner_count(l)))),
        Node::Randomize(inner) | Node::Ignore(inner) | Node::Pretend(inner) | Node::SameLine(inner) | Node::Once(_, inner) => inner_count(inner),
        Node::Var(name, value) => product(std::iter::once(inner_count(name)).chain(value.iter().map(|v| inner_count(v)))),
    }
}

fn once_groups(nodes: &[Node]) -> Vec<(&str, Vec<&Node>)> {
    fn visit<'a>(nodes: &'a [Node], groups: &mut Vec<(&'a str, Vec<&'a Node>)>) {
        for node in nodes {
            if let Node::Once(key, _) = node {
                if let Some((_, entries)) = groups.iter_mut().find(|(k, _)| *k == key) {
                    entries.push(node);
                } else {
                    groups.push((key, vec![node]));
                }
            }
            match node {
                Node::OneOf(options) | Node::LineSwap(options) | Node::LineSwapText(options) => {
                    for inner in options {
                        visit(inner, groups);
                    }
                }
                Node::Randomize(inner) | Node::Ignore(inner) | Node::Pretend(inner) | Node::SameLine(inner) | Node::Once(_, inner) => visit(inner, groups),
                Node::Var(name, value) => {
                    visit(name, groups);
                    if let Some(v) = value {
                        visit(v, groups);
                    }
                }
                _ => {}
            }
        }
    }
    let mut groups = Vec::new();
    visit(nodes, &mut groups);
    groups
}

pub fn count(nodes: &[Node]) -> Result<u64, String> {
    let groups = once_groups(nodes);
    for (key, options) in &groups {
        if options.len() == 1 {
            return Err(format!("PERM_ONCE({key}) is used once; a key names choices shared by two or more places"));
        }
    }
    product(std::iter::once(inner_count(nodes)).chain(groups.iter().map(|(_, options)| Ok(options.len() as u64))))
}

fn shuffle<T>(mut items: Vec<T>, mut seed: u64) -> Vec<T> {
    let mut out = Vec::new();
    while !items.is_empty() {
        let i = (seed % items.len() as u64) as usize;
        seed /= items.len() as u64;
        out.push(items.remove(i));
    }
    out
}

pub fn expand_seed(nodes: &[Node], mut seed: u64) -> Result<Expanded, String> {
    if seed >= count(nodes)? {
        return Err("PERM seed outside search space".into());
    }
    let mut once = Vec::new();
    for (_, options) in once_groups(nodes) {
        once.push(options[(seed % options.len() as u64) as usize]);
        seed /= options.len() as u64;
    }
    let mut out = Expanded::default();
    out.text = evaluate(nodes, seed, &once, &mut BTreeMap::new(), &mut out)?;
    Ok(out)
}

fn evaluate(nodes: &[Node], mut seed: u64, once: &[&Node], vars: &mut BTreeMap<String, String>, out: &mut Expanded) -> Result<String, String> {
    let mut text = String::new();
    for node in nodes {
        let count = node_count(node)?;
        let sub = seed % count;
        seed /= count;
        text.push_str(&eval_node(node, sub, once, vars, out)?);
    }
    Ok(text)
}

fn eval_node(node: &Node, mut seed: u64, once: &[&Node], vars: &mut BTreeMap<String, String>, out: &mut Expanded) -> Result<String, String> {
    Ok(match node {
        Node::Text(text) => text.clone(),
        Node::Int(low, _) => (i128::from(*low) + i128::from(seed)).to_string(),
        Node::OneOf(options) => {
            let mut chosen = None;
            for option in options {
                let count = inner_count(option)?;
                if seed < count {
                    chosen = Some(evaluate(option, seed, once, vars, out)?);
                    break;
                }
                seed -= count;
            }
            chosen.expect("validated seed")
        }
        Node::LineSwap(lines) | Node::LineSwapText(lines) => {
            let orders = factorial(lines.len())?;
            let order = shuffle((0..lines.len()).collect(), seed % orders);
            seed /= orders;
            // Evaluate children in source order BEFORE shuffling: PERM_VAR
            // writes and reads must not change order with their output lines.
            let mut texts = Vec::new();
            for line in lines {
                let count = inner_count(line)?;
                texts.push(evaluate(line, seed % count, once, vars, out)?);
                seed /= count;
            }
            if matches!(node, Node::LineSwap(_)) {
                let id = out.ast_swaps.len();
                out.ast_swaps.push(order);
                format!(
                    "{{\n/* _permuter ast {id} */\n{}\n}}",
                    texts.iter().map(|s| format!("{{{s}\n}}")).collect::<Vec<_>>().join("\n")
                )
            } else {
                order.iter().map(|&i| texts[i].as_str()).collect::<Vec<_>>().join("\n")
            }
        }
        Node::Once(_, inner) => {
            if once.iter().any(|chosen| std::ptr::eq(*chosen, node)) {
                evaluate(inner, seed, once, vars, out)?
            } else {
                String::new()
            }
        }
        Node::Var(name, value) => {
            let count = inner_count(name)?;
            let name = evaluate(name, seed % count, once, vars, out)?.trim().to_string();
            if let Some(value) = value {
                let text = evaluate(value, seed / count, once, vars, out)?;
                vars.insert(name, text);
                String::new()
            } else {
                vars.get(&name).cloned().ok_or_else(|| format!("Tried to read undefined PERM_VAR {name}"))?
            }
        }
        Node::Ignore(inner) => {
            let text = evaluate(inner, seed, once, vars, out)?;
            if text.is_empty() {
                String::new()
            } else {
                let placeholder = format!("/* _permuter ignored {} */", out.ignored.len());
                out.ignored.push((placeholder.clone(), text));
                format!("\n{placeholder}\n")
            }
        }
        Node::Randomize(inner) | Node::SameLine(inner) | Node::Pretend(inner) => {
            let text = evaluate(inner, seed, once, vars, out)?;
            let kind = match node {
                Node::Randomize(_) => {
                    if text.contains("/* _permuter randomizer start */") {
                        return Err("PERM_RANDOMIZE cannot hold another PERM_RANDOMIZE".into());
                    }
                    out.randomize.push(text.clone());
                    "randomizer"
                }
                Node::SameLine(_) => "sameline",
                _ => "pretend",
            };
            format!("\n/* _permuter {kind} start */\n{text}\n/* _permuter {kind} end */\n")
        }
    })
}

/// visited, then shuffle the remainder. A finite pass never repeats a seed.
pub struct Seeds {
    total: u64,
    seen: HashSet<u64>,
    remaining: Option<Vec<u64>>,
    random: bool,
    rng: Rng,
}

impl Seeds {
    pub fn new(total: u64, random: bool, seed: u64) -> Self {
        Self {
            total,
            seen: HashSet::new(),
            remaining: None,
            random,
            rng: Rng::new(seed),
        }
    }
}

impl Iterator for Seeds {
    type Item = u64;
    fn next(&mut self) -> Option<u64> {
        loop {
            if (self.seen.len() as u64) < self.total / 2 {
                let seed = self.rng.below(self.total as usize) as u64;
                if self.seen.insert(seed) {
                    return Some(seed);
                }
            } else {
                if self.remaining.is_none() {
                    let mut remaining: Vec<_> = (0..self.total).filter(|s| !self.seen.contains(s)).collect();
                    for i in (1..remaining.len()).rev() {
                        remaining.swap(i, self.rng.below(i + 1));
                    }
                    remaining.reverse(); // pop in the shuffled list's forward order
                    self.remaining = Some(remaining);
                }
                if let Some(seed) = self.remaining.as_mut().unwrap().pop() {
                    return Some(seed);
                }
                if !self.random {
                    return None;
                }
                self.seen.clear();
                self.remaining = None;
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    const DRAFT: &str = "void f(int *p)\n{\n    PERM_LINESWAP(\n    p[0] = 1;\n    p[1] = 2;\n    )\n    p[2] = PERM_GENERAL(3, 4 + PERM_ONEOF(5, 6));\n    PERM_IGNORE(asm(\"\" ::: \"r5\");)\n    PERM_RANDOMIZE(p[3] = 7;)\n    /* PERM_GENERAL(x) in a comment stays */\n}\n";

    #[test]
    fn default_expansion_takes_first_choices_and_written_order() {
        let nodes = parse(DRAFT).unwrap();
        assert!(has_choices(&nodes) && randomizes(&nodes));
        let expanded = expand(&nodes, None).unwrap();
        let mut function = super::super::parse::locate(&expanded.text, None, &Default::default()).unwrap().function;
        expanded.apply_ast(&mut function);
        assert!(function.print().contains("    p[0] = 1;\n    p[1] = 2;\n"));
        assert!(expanded.text.contains("p[2] = 3;"));
        assert!(expanded.text.contains("/* _permuter ignored 0 */"));
        assert_eq!(expanded.randomize, ["p[3] = 7;"]);
        let restored = expanded.restore(&expanded.text);
        assert!(restored.contains("asm(\"\" ::: \"r5\");"));
        assert!(!has_macros(&restored));
        assert!(restored.contains("/* PERM_GENERAL(x) in a comment stays */"));
    }

    #[test]
    fn random_expansions_reach_every_choice_and_repeat_under_a_seed() {
        let nodes = parse(DRAFT).unwrap();
        let mut seen = std::collections::BTreeSet::new();
        let mut rng = Rng::new(7);
        for _ in 0..200 {
            let expanded = expand(&nodes, Some(&mut rng)).unwrap();
            let mut function = super::super::parse::locate(&expanded.text, None, &Default::default()).unwrap().function;
            expanded.apply_ast(&mut function);
            seen.insert(function.print());
        }
        // Two line orders times three values.
        assert_eq!(seen.len(), 6);
        assert!(seen.iter().any(|text| text.contains("p[2] = 4 + 6;")));
        assert!(seen.iter().any(|text| text.contains("    p[1] = 2;\n    p[0] = 1;\n")));
        let (mut a, mut b) = (Rng::new(3), Rng::new(3));
        for _ in 0..20 {
            assert_eq!(expand(&nodes, Some(&mut a)), expand(&nodes, Some(&mut b)));
        }
    }

    #[test]
    fn macros_are_found_only_as_calls_outside_comments_and_strings() {
        assert!(has_macros("x = PERM_ONEOF(1, 2);"));
        assert!(!has_macros("/* PERM_ONEOF(1, 2) */ x = \"PERM_IGNORE(a)\";"));
        assert!(parse("int PERM_GENERALLY(int);").is_err());
        assert!(parse("PERM_GENERAL(a, (b").is_err());
    }

    fn expansions(source: &str) -> Vec<String> {
        let nodes = parse(source).unwrap();
        (0..count(&nodes).unwrap())
            .map(|seed| {
                let e = expand_seed(&nodes, seed).unwrap();
                e.restore(&e.text).trim().to_string()
            })
            .collect()
    }

    #[test]
    fn seed_order_uses_sum_for_choices_and_product_for_siblings() {
        assert_eq!(
            expansions("PERM_GENERAL(a,PERM_GENERAL(b,c)) PERM_INT(-1,1)"),
            ["a -1", "b -1", "c -1", "a 0", "b 0", "c 0", "a 1", "b 1", "c 1",]
        );
        assert_eq!(expansions("PERM_GENERAL((,),x)"), [",", "x"]);
        assert!(parse("PERM_INT(3,2)").is_err());
        assert!(parse("PERM_INT(1)").is_err());
        assert!(parse("PERM_INT(0,18446744073709551615)").is_err());
    }

    #[test]
    fn once_selects_occurrences_and_variables_share_the_selected_expansion() {
        assert!(parse("PERM_ONCE(k,a)XPERM_ONCE(k,b)").is_err()); // only one real occurrence
    }

    #[test]
    fn shared_fragments_and_once_choices_compose_with_surrounding_text() {
        assert_eq!(expansions("begin;PERM_ONCE(note,record(3);)middle;PERM_ONCE(note,record(8);)end;"), ["begin;record(3);middle;end;", "begin;middle;record(8);end;"]);
        assert_eq!(expansions("PERM_VAR(mask,PERM_GENERAL(15,63))return value & PERM_VAR(mask);"), ["return value & 15;", "return value & 63;"]);
        assert!(parse("prefix;PERM_ONCE(record(4);)suffix;").is_err());
        assert!(expand(&parse("return PERM_VAR(unbound_mask);").unwrap(), None).is_err());
        assert_eq!(expansions("PERM_LINESWAP_TEXT(\nPERM_VAR(step,19)start();\nfinish(PERM_VAR(step));\n)"), ["start();\nfinish(19);", "finish(19);\nstart();"]);
    }

    #[test]
    fn ignored_macros_expand_and_pretend_only_reaches_the_parser() {
        assert_eq!(expansions("PERM_IGNORE(PERM_INT(2,3))"), ["2", "3"]);
        assert_eq!(expansions("PERM_PRETEND(typedef int T;)int x;"), ["int x;"]);
        let expanded = expand(&parse("PERM_PRETEND(typedef int T;)int x;").unwrap(), None).unwrap();
        assert!(expanded.text.contains("typedef int T;"));
        assert_eq!(expansions("PERM_FORCE_SAMELINE(a;\nb;)"), ["a; b;"]);
        assert_eq!(expansions("/* 日本語 */ PERM_INT(1,1)"), ["/* 日本語 */ 1"]);
        assert!(expand(&parse("PERM_RANDOMIZE(PERM_RANDOMIZE(x();))").unwrap(), None).is_err());
        assert_eq!(expansions("PERM_PRETEND(\n#pragma _permuter define ID 7\ntypedef int T;\n)"), ["#define ID 7"]);
    }

    #[test]
    fn ast_lineswap_keeps_unbraced_bodies_together_and_flattens_declarations() {
        let source = "void f(int a) { if (a) PERM_LINESWAP(\nx(); y();\nz();\n) }";
        let nodes = parse(source).unwrap();
        for seed in 0..2 {
            let expanded = expand_seed(&nodes, seed).unwrap();
            let mut f = super::super::parse::locate(&expanded.text, None, &Default::default()).unwrap().function;
            expanded.apply_ast(&mut f);
            let Stmt::If(_, body, _) = &f.body[0] else { panic!() };
            let Stmt::Block(stmts) = body.as_ref() else { panic!("must remain a block") };
            assert_eq!(stmts.len(), 3);
            assert!(!f.print().contains("_permuter ast"));
        }
        let source = "void f(void) { PERM_LINESWAP(\nint a;\nint b;\n) a = b = 0; }";
        let expanded = expand(&parse(source).unwrap(), None).unwrap();
        let mut f = super::super::parse::locate(&expanded.text, None, &Default::default()).unwrap().function;
        expanded.apply_ast(&mut f);
        assert!(matches!(f.body[0], Stmt::Decl(_)) && matches!(f.body[1], Stmt::Decl(_)));
    }

    #[test]
    fn ast_lineswap_keeps_multiline_and_trailing_comments_whole() {
        let source = "void target(int *a, int *b) { PERM_LINESWAP(\n/* first\n * second */\n*a = PERM_GENERAL(1,2); // trailing\n*b = 3;\n) }";
        let nodes = parse(source).unwrap();
        assert_eq!(count(&nodes).unwrap(), 4);
        let mut results = std::collections::BTreeSet::new();
        for seed in 0..4 {
            let expanded = expand_seed(&nodes, seed).unwrap();
            for text in [expanded.text.clone(), expanded.preprocess_text()] {
                let mut unit = super::super::c::parse(&text).unwrap();
                expanded.apply_c_ast(&mut unit);
                let printed = unit.print();
                assert!(!printed.contains("_permuter ast"));
                assert!(printed.contains("*b = 3;"));
                assert!(printed.contains("*a = 1;") || printed.contains("*a = 2;"));
                assert_eq!(printed.matches("*a =").count(), 1);
                assert_eq!(printed.matches("*b =").count(), 1);
                super::super::c::parse(&printed).unwrap();
                results.insert((printed.contains("*a = 1;"), printed.find("*a =").unwrap() < printed.find("*b =").unwrap()));
            }
        }
        assert_eq!(results.len(), 4);
    }

    #[test]
    fn finite_seeds_are_exhaustive_and_random_seeds_repeat_whole_passes() {
        for count in 1..24 {
            let seeds: Vec<_> = Seeds::new(count, false, 12).collect();
            assert_eq!(seeds.len(), count as usize);
            assert_eq!(seeds.iter().copied().collect::<std::collections::BTreeSet<_>>(), (0..count).collect());
            let infinite: Vec<_> = Seeds::new(count, true, 12).take(count as usize * 3).collect();
            for cycle in infinite.chunks(count as usize) {
                assert_eq!(cycle.iter().collect::<HashSet<_>>().len(), count as usize);
            }
        }
    }
}
