//! Source-owned steering and inline dependencies (S2).
//! This analysis accepts fresh preprocessing, never compiled output or a report.
use crate::permute::ast::{Expr, Init, Stmt, UnOp};
use crate::permute::lex::{lex, Tok, Token};
use crate::permute::parse::{locate, matching, parameters, scan_definitions, scan_unit};
use std::collections::{BTreeMap, BTreeSet};

#[derive(Clone, Debug, Default)]
pub(crate) struct Analysis {
    pub steered: BTreeSet<String>,
    pub whole_owners: BTreeSet<String>,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Forwarder {
    pub file: String,
    pub line: usize,
    pub name: String,
}

#[derive(Clone)]
struct Definition {
    name: String,
    owner: String,
    line: usize,
    boundary: usize,
    name_index: usize,
    params_end: usize,
    open: usize,
    close: usize,
    inline: bool,
    static_: bool,
}

pub(crate) fn owner(path: &str) -> String {
    let path = std::path::Path::new(path);
    let root = super::routing::root();
    let absolute = if path.is_absolute() {
        path.to_path_buf()
    } else {
        root.join(path)
    };
    let normalized = std::fs::canonicalize(&absolute).unwrap_or(absolute);
    normalized
        .strip_prefix(root)
        .unwrap_or(&normalized)
        .to_string_lossy()
        .replace('\\', "/")
}

fn origins(text: &str, fallback: &str) -> Vec<(String, usize)> {
    let mut file = owner(fallback);
    let mut line = 1;
    text.lines()
        .map(|row| {
            if let Some(rest) = row.trim_start().strip_prefix('#') {
                let rest = rest
                    .trim_start()
                    .strip_prefix("line ")
                    .unwrap_or(rest.trim_start());
                if let Some((number, quoted)) = rest.split_once(' ') {
                    if let (Ok(next), Some(quoted)) = (
                        number.parse::<usize>(),
                        quoted.trim_start().strip_prefix('"'),
                    ) {
                        if let Some((path, _)) = quoted.split_once('"') {
                            file = owner(path);
                            line = next;
                            return (file.clone(), line);
                        }
                    }
                }
            }
            let location = (file.clone(), line);
            line += 1;
            location
        })
        .collect()
}

fn definitions(tokens: &[Token], origins: &[(String, usize)]) -> Result<Vec<Definition>, String> {
    Ok(scan_definitions(tokens).into_iter().filter_map(|(name, boundary, name_index, params_end, open)| {
        // Raw conditionals may spell alternative unmatched bodies; the fresh
        // compiler expansion supplies their active, balanced definitions.
        let close = matching(tokens, open)?;
        let (owner, line) = origins.get(tokens[name_index].line - 1).cloned().unwrap_or_default();
        let header = &tokens[boundary..name_index];
        Some(Definition {
            name, owner, line, boundary, name_index, params_end, open, close,
            inline: header.iter().any(|token| matches!(&token.tok, Tok::Ident(word) if ["inline", "__inline", "__inline__"].contains(&word.as_str()))),
            static_: header.iter().any(|token| matches!(&token.tok, Tok::Ident(word) if word == "static")),
        })
    }).collect())
}

/// Fresh expanded function definitions owned by one source file. Diagnostic
/// readers use this to distinguish an edition exclusion from an unlinked body.
pub(crate) fn owned_definitions(text: &str, source: &str) -> Result<BTreeSet<String>, String> {
    let tokens = lex(text)?;
    let locations = origins(text, source);
    let wanted = owner(source);
    Ok(definitions(&tokens, &locations)?
        .into_iter()
        .filter(|definition| definition.owner == wanted)
        .map(|definition| definition.name)
        .collect())
}

/// Mask GNU assembly/attributes for the read-only C parser. Operand calls are
/// retained in the original tokens and refused below if masking would hide one.
fn c_only(text: &str, tokens: &[Token]) -> String {
    let mut bytes = text.as_bytes().to_vec();
    let mut index = 0;
    while index < tokens.len() {
        let special = matches!(&tokens[index].tok, Tok::Ident(word) if ["asm", "__asm", "__asm__", "__attribute", "__attribute__"].contains(&word.as_str()));
        if special {
            let mut open = index + 1;
            while matches!(tokens.get(open).map(|token| &token.tok), Some(Tok::Ident(word)) if ["volatile", "__volatile", "__volatile__"].contains(&word.as_str()))
            {
                open += 1;
            }
            if matches!(
                tokens.get(open).map(|token| &token.tok),
                Some(Tok::Punct("("))
            ) {
                if let Some(close) = matching(tokens, open) {
                    for byte in &mut bytes[tokens[index].start..tokens[close].end] {
                        if *byte != b'\n' {
                            *byte = b' ';
                        }
                    }
                    index = close;
                }
            }
        }
        index += 1;
    }
    String::from_utf8(bytes).expect("masking preserves UTF-8")
}

fn ident(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Ident(name) => Some(name),
        Expr::Cast(_, value) | Expr::Unary(UnOp::Deref | UnOp::AddrOf, value) => ident(value),
        _ => None,
    }
}
fn argument_ident(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Ident(name) => Some(name),
        Expr::Cast(_, value) => argument_ident(value),
        _ => None,
    }
}

type Scope = BTreeMap<String, BTreeSet<String>>;
fn targets(name: &str, scopes: &[Scope], helpers: &BTreeSet<String>) -> BTreeSet<String> {
    scopes
        .iter()
        .rev()
        .find_map(|scope| scope.get(name).cloned())
        .unwrap_or_else(|| {
            helpers
                .contains(name)
                .then(|| name.to_string())
                .into_iter()
                .collect()
        })
}
fn value_targets(expr: &Expr, scopes: &[Scope], helpers: &BTreeSet<String>) -> BTreeSet<String> {
    ident(expr).map_or_else(BTreeSet::new, |name| targets(name, scopes, helpers))
}
fn expr_calls(
    expr: &Expr,
    scopes: &mut Vec<Scope>,
    helpers: &BTreeSet<String>,
    calls: &mut BTreeSet<String>,
) {
    match expr {
        Expr::SizeofExpr(_) | Expr::SizeofType(_) => return,
        Expr::Call(callee, _) => {
            calls.extend(value_targets(callee, scopes, helpers));
        }
        Expr::Assign(_, left, right) => {
            if let Expr::Ident(name) = left.as_ref() {
                let assigned = value_targets(right, scopes, helpers);
                if let Some(scope) = scopes
                    .iter_mut()
                    .rev()
                    .find(|scope| scope.contains_key(name))
                {
                    scope.insert(name.clone(), assigned);
                }
            }
        }
        _ => {}
    }
    for child in expr.children() {
        expr_calls(child, scopes, helpers, calls);
    }
}
fn init_calls(
    init: &Init,
    scopes: &mut Vec<Scope>,
    helpers: &BTreeSet<String>,
    calls: &mut BTreeSet<String>,
) {
    match init {
        Init::Expr(expr) => expr_calls(expr, scopes, helpers, calls),
        Init::List(items) => {
            for item in items {
                init_calls(item, scopes, helpers, calls);
            }
        }
    }
}
fn merge_scopes(scopes: &mut [Scope], other: &[Scope]) {
    for (scope, other) in scopes.iter_mut().zip(other) {
        for (name, values) in other {
            scope
                .entry(name.clone())
                .or_default()
                .extend(values.iter().cloned());
        }
    }
}
fn switch_calls(
    stmt: &Stmt,
    scopes: &mut Vec<Scope>,
    entry: &[Scope],
    helpers: &BTreeSet<String>,
    calls: &mut BTreeSet<String>,
) {
    if let Stmt::Block(items) = stmt {
        scopes.push(Scope::new());
        for item in items {
            if matches!(item, Stmt::Case(_) | Stmt::Default) {
                merge_scopes(scopes, entry);
            }
            body_calls(std::slice::from_ref(item), scopes, helpers, calls);
        }
        scopes.pop();
    } else {
        body_calls(std::slice::from_ref(stmt), scopes, helpers, calls);
    }
}
fn body_calls(
    body: &[Stmt],
    scopes: &mut Vec<Scope>,
    helpers: &BTreeSet<String>,
    calls: &mut BTreeSet<String>,
) {
    for stmt in body {
        match stmt {
            Stmt::Decl(decl) => {
                for item in &decl.items {
                    let assigned = match &item.init {
                        Some(Init::Expr(expr)) => value_targets(expr, scopes, helpers),
                        _ => BTreeSet::new(),
                    };
                    scopes
                        .last_mut()
                        .unwrap()
                        .insert(item.name.clone(), assigned);
                    if let Some(init) = &item.init {
                        init_calls(init, scopes, helpers, calls);
                    }
                }
            }
            Stmt::Block(items) => {
                scopes.push(Scope::new());
                body_calls(items, scopes, helpers, calls);
                scopes.pop();
            }
            Stmt::If(expr, yes, no) => {
                expr_calls(expr, scopes, helpers, calls);
                let original = scopes.clone();
                body_calls(std::slice::from_ref(yes.as_ref()), scopes, helpers, calls);
                let yes_scopes = scopes.clone();
                *scopes = original;
                if let Some(no) = no {
                    body_calls(std::slice::from_ref(no.as_ref()), scopes, helpers, calls);
                }
                merge_scopes(scopes, &yes_scopes);
            }
            Stmt::DoWhile(body, condition) => {
                let first = scopes.clone();
                let mut exits: Option<Vec<Scope>> = None;
                loop {
                    let entry = scopes.clone();
                    body_calls(std::slice::from_ref(body.as_ref()), scopes, helpers, calls);
                    expr_calls(condition, scopes, helpers, calls);
                    if let Some(previous) = &mut exits {
                        merge_scopes(previous, scopes);
                    } else {
                        exits = Some(scopes.clone());
                    }
                    let mut next = first.clone();
                    merge_scopes(&mut next, scopes);
                    if next == entry {
                        *scopes = exits.unwrap();
                        break;
                    }
                    *scopes = next;
                }
            }
            Stmt::While(_, body) | Stmt::For(_, _, _, body) => {
                if let Stmt::For(Some(init), _, _, _) = stmt {
                    expr_calls(init, scopes, helpers, calls);
                }
                loop {
                    let entry = scopes.clone();
                    match stmt {
                        Stmt::While(condition, _) | Stmt::For(_, Some(condition), _, _) => {
                            expr_calls(condition, scopes, helpers, calls)
                        }
                        _ => {}
                    }
                    body_calls(std::slice::from_ref(body.as_ref()), scopes, helpers, calls);
                    if let Stmt::For(_, _, Some(step), _) = stmt {
                        expr_calls(step, scopes, helpers, calls);
                    }
                    merge_scopes(scopes, &entry);
                    if *scopes == entry {
                        break;
                    }
                }
            }
            Stmt::Switch(expr, body) => {
                expr_calls(expr, scopes, helpers, calls);
                let entry = scopes.clone();
                switch_calls(body, scopes, &entry, helpers, calls);
                merge_scopes(scopes, &entry);
            }
            _ => {
                for expr in stmt.own_exprs() {
                    expr_calls(expr, scopes, helpers, calls);
                }
            }
        }
    }
}

/// Literal pools emitted at file scope name their complete owner in .size.
/// Data directives do not make neighboring plain functions steering code.
fn pool_owners(tokens: &[Token], tag: usize) -> Option<BTreeSet<String>> {
    let at = (tag + 1..tokens.len()).find(|&at| !matches!(tokens[at].tok, Tok::Comment(_)))?;
    if !matches!(&tokens[at].tok, Tok::Ident(word) if ["asm", "__asm", "__asm__"].contains(&word.as_str()))
    {
        return None;
    }
    let open = at + 1;
    if !matches!(
        tokens.get(open).map(|token| &token.tok),
        Some(Tok::Punct("("))
    ) {
        return None;
    }
    let close = matching(tokens, open)?;
    let mut assembly = String::new();
    for token in &tokens[open + 1..close] {
        let Tok::Str(value) = &token.tok else {
            return None;
        };
        assembly.push_str(
            &value[1..value.len() - 1]
                .replace("\\n", "\n")
                .replace("\\t", "\t")
                .replace("\\\"", "\"")
                .replace("\\\\", "\\"),
        );
    }
    let mut owners = BTreeSet::new();
    for row in assembly
        .lines()
        .map(str::trim)
        .filter(|row| !row.is_empty())
    {
        if row.ends_with(':') {
            continue;
        }
        let operation = row.split_whitespace().next()?;
        if ![
            ".align", ".balign", ".p2align", ".word", ".long", ".short", ".byte", ".size",
        ]
        .contains(&operation)
        {
            return None;
        }
        if operation == ".size" {
            let name = row[operation.len()..]
                .trim_start()
                .split(',')
                .next()?
                .trim();
            if name.is_empty() || !name.chars().all(|c| c.is_ascii_alphanumeric() || c == '_') {
                return None;
            }
            owners.insert(name.to_string());
        }
    }
    Some(owners)
}

/// Exact inline-call dependencies in one expanded translation unit. A header
/// preamble applies to that header's definitions, never to its including C file.
pub(crate) fn analyze(text: &str, source: &str) -> Result<Analysis, String> {
    let tokens = lex(text)?;
    let locations = origins(text, source);
    let defs = definitions(&tokens, &locations)?;
    let clean = c_only(text, &tokens);
    let typedefs = scan_unit(&clean)?.typedef_names();
    let helpers = defs
        .iter()
        .filter(|def| def.inline)
        .map(|def| def.name.clone())
        .collect::<BTreeSet<_>>();
    let mut result = Analysis::default();
    for (index, token) in tokens.iter().enumerate() {
        if matches!(&token.tok, Tok::Comment(comment) if comment.contains("FAKEMATCH")) {
            let tag_owner = locations.get(token.line - 1).map(|(owner, _)| owner);
            if let Some(def) = defs.iter().find(|def| {
                tag_owner == Some(&def.owner) && (def.boundary..=def.close).contains(&index)
            }) {
                result.steered.insert(def.name.clone());
            } else if let Some(owners) = pool_owners(&tokens, index) {
                result.steered.extend(owners);
            } else if let Some((owner, _)) = locations.get(token.line - 1) {
                result.whole_owners.insert(owner.clone());
            }
        }
    }
    for def in &defs {
        if result.whole_owners.contains(&def.owner)
            || std::path::Path::new(&def.owner).ends_with("games/COMMON/INCLUDE/LIB/CALL.H")
        {
            result.steered.insert(def.name.clone());
        }
    }
    let mut edges = BTreeMap::<String, BTreeSet<String>>::new();
    loop {
        let relevant = result
            .steered
            .intersection(&helpers)
            .cloned()
            .collect::<BTreeSet<_>>();
        for def in &defs {
            if result.steered.contains(&def.name)
                || edges.contains_key(&def.name)
                || !tokens[def.open + 1..def.close]
                    .iter()
                    .any(|token| matches!(&token.tok, Tok::Ident(name) if relevant.contains(name)))
            {
                continue;
            }
            if tokens[def.open + 1..def.close].iter().any(|token| {
                matches!(&token.tok, Tok::Ident(name) if helpers.contains(name))
                    && clean[token.start..token.end].trim().is_empty()
            }) {
                return Err(format!(
                    "{}:{} {}: inline helper in assembly operand needs source analysis",
                    def.owner, def.line, def.name
                ));
            }
            let located = locate(&clean, Some(&def.name), &typedefs).map_err(|error| {
                format!(
                    "{}:{} {} inline dependencies: {error}",
                    def.owner, def.line, def.name
                )
            })?;
            let mut scopes = vec![located
                .function
                .params
                .iter()
                .flat_map(|decl| &decl.items)
                .map(|item| (item.name.clone(), BTreeSet::new()))
                .collect()];
            let mut calls = BTreeSet::new();
            body_calls(&located.function.body, &mut scopes, &helpers, &mut calls);
            edges.insert(def.name.clone(), calls);
        }
        let mut changed = false;
        for (caller, calls) in &edges {
            if calls.iter().any(|name| result.steered.contains(name)) {
                changed |= result.steered.insert(caller.clone());
            }
        }
        if !changed {
            break;
        }
    }
    Ok(result)
}

fn syntax_types(tokens: &[Token], names: &mut BTreeSet<String>) {
    for token in tokens {
        let Tok::Ident(word) = &token.tok else {
            continue;
        };
        if [
            "static",
            "extern",
            "inline",
            "__inline",
            "__inline__",
            "const",
            "__const",
            "volatile",
            "__volatile__",
            "register",
        ]
        .contains(&word.as_str())
        {
            continue;
        }
        if ![
            "void", "char", "short", "int", "long", "float", "double", "signed", "unsigned",
            "struct", "union", "enum",
        ]
        .contains(&word.as_str())
        {
            names.insert(word.clone());
        }
        break;
    }
}
fn definition_types(
    tokens: &[Token],
    def: &Definition,
    known: &BTreeSet<String>,
) -> BTreeSet<String> {
    let mut names = known.clone();
    syntax_types(&tokens[def.boundary..def.name_index], &mut names);
    let mut start = def.name_index + 2;
    let mut at = start;
    while at < def.params_end {
        if matches!(tokens[at].tok, Tok::Punct("(")) {
            at = matching(tokens, at).unwrap_or(at);
        }
        if matches!(tokens[at].tok, Tok::Punct(",")) {
            syntax_types(&tokens[start..at], &mut names);
            start = at + 1;
        }
        at += 1;
    }
    syntax_types(&tokens[start..def.params_end], &mut names);
    names
}

fn has_reason(tokens: &[Token]) -> bool {
    tokens.iter().any(|token| match &token.tok {
        Tok::Comment(comment) => comment
            .split_once("FAKEMATCH:")
            .is_some_and(|(_, reason)| !reason.trim().trim_end_matches("*/").trim().is_empty()),
        _ => false,
    })
}

/// Pure call/return adapters are steering helpers; transformed computations
/// remain outside this deliberately bounded automatic convention.
pub(crate) fn forwarding_findings(text: &str, source: &str) -> Result<Vec<Forwarder>, String> {
    let tokens = lex(text)?;
    let locations = origins(text, source);
    let defs = definitions(&tokens, &locations)?;
    let clean = c_only(text, &tokens);
    let typedefs = scan_unit(&clean)?.typedef_names();
    let mut found = Vec::new();
    for def in defs.iter().filter(|def| def.inline && def.static_) {
        if has_reason(&tokens[def.open + 1..def.close]) {
            continue;
        }
        // Restrict parsing to this definition so a batch's repeated header
        // definitions cannot borrow the first translation unit's body/tag.
        let fragment = &clean[tokens[def.name_index].start..tokens[def.close].end];
        let fragment = format!("void {fragment}");
        // The parser uses parameter types and body, not the artificial result type.
        let types = definition_types(&tokens, def, &typedefs);
        let Ok(located) = locate(&fragment, Some(&def.name), &types) else {
            continue;
        };
        let statements = located
            .function
            .body
            .iter()
            .filter(|stmt| !matches!(stmt, Stmt::Comment(_) | Stmt::Empty))
            .collect::<Vec<_>>();
        let expression = match statements.as_slice() {
            [Stmt::Expr(expr)] | [Stmt::Return(Some(expr))] => expr,
            _ => continue,
        };
        let mut expression: &Expr = expression;
        while let Expr::Cast(_, value) = expression {
            expression = value.as_ref();
        }
        let Expr::Call(callee, args) = expression else {
            continue;
        };
        let Some(callee) = ident(callee) else {
            continue;
        };
        let mut names = parameters(
            &tokens[def.name_index + 2..def.params_end],
            &mut types.clone(),
        )?
        .into_iter()
        .flat_map(|decl| decl.items)
        .map(|item| item.name)
        .collect::<Vec<_>>();
        names.retain(|name| name != callee);
        names.sort();
        let forwarded = args.iter().map(argument_ident).collect::<Option<Vec<_>>>();
        if forwarded.is_some_and(|mut forwarded| {
            forwarded.sort();
            forwarded == names.iter().map(String::as_str).collect::<Vec<_>>()
        }) {
            found.push(Forwarder {
                file: def.owner.clone(),
                line: def.line,
                name: def.name.clone(),
            });
        }
    }
    Ok(found)
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn helper_graph_is_transitive_and_inclusion_alone_does_not_steer() {
        let text = "# 1 \"games/COMMON/INCLUDE/LIB/CALL.H\"\nstatic inline void Call1(void (*f)(), int x) { f(x); }\n# 1 \"games/THE BROKEN SEAL/SRC/A.C\"\nstatic inline void Adapter(int x) { Call1(Target, x); }\nvoid Caller(void) { Adapter(1); }\nvoid Plain(void) { Target(2); }\n";
        let analysis = analyze(text, "games/THE BROKEN SEAL/SRC/A.C").unwrap();
        assert_eq!(
            analysis.steered,
            ["Adapter", "Call1", "Caller"].map(String::from).into()
        );
    }
    #[test]
    fn bindings_sizeof_aliases_and_noninline_calls_are_distinct() {
        let text = "static inline int Help(int x) { /* FAKEMATCH: measured. */ return x; }\nint Tagged(int x) { /* FAKEMATCH: measured. */ return x; }\nint Shadow(int (*Help)(int)) { return Help(1); }\nint Local(void) { int (*Help)(int) = Other; return Help(1); }\nint Alias(void) { int (*f)(int) = Help; return f(1); }\nint Unevaluated(void) { return sizeof(Help(1)); }\nint Ordinary(void) { return Tagged(1); }\n";
        let analysis = analyze(text, "A.C").unwrap();
        assert_eq!(
            analysis.steered,
            ["Alias", "Help", "Tagged"].map(String::from).into()
        );
    }
    #[test]
    fn cycles_and_macro_bodies_remain_function_specific() {
        let text = "static inline void A(void) { /* FAKEMATCH: measured. */ B(); }\nstatic inline void B(void) { A(); }\nvoid Expanded(void) { /* FAKEMATCH: preserved macro. */ A(); }\nvoid Plain(void) {}";
        assert_eq!(
            analyze(text, "A.C").unwrap().steered,
            ["A", "B", "Expanded"].map(String::from).into()
        );
    }
    #[test]
    fn header_preamble_never_marks_unrelated_source_functions() {
        let text = "# 1 \"helper.h\"\n/* FAKEMATCH: helper only. */\ntypedef int T;\nstatic inline T Help(T x) { return Target(x); }\n# 1 \"source.c\"\nvoid Plain(void) {}\nvoid User(void) { Help(1); }";
        assert_eq!(
            analyze(text, "source.c").unwrap().steered,
            ["Help", "User"].map(String::from).into()
        );
    }
    #[test]
    fn asm_does_not_hide_inline_dependencies() {
        let text = "static inline int Help(int x) { /* FAKEMATCH: measured. */ return x; }\nint Caller(void) { register int r __asm__(\"r0\"); __asm__ volatile(\"\"); return Help(1); }";
        assert!(analyze(text, "A.C").unwrap().steered.contains("Caller"));
    }
    #[test]
    fn forwarders_need_reason_inside_their_own_body() {
        let text = "/* FAKEMATCH: preceding tag does not admit helper. */\nstatic inline int Forward(int x) { return (int)Target((int)x); }\nstatic inline void Pointer(void (*f)(int), int x) { f(x); }\nstatic inline int Tagged(int x) { /* FAKEMATCH: measured. */ return Target(x); }\nstatic inline int Compute(int x) { return Target(x + 1); }\nstatic inline int Fake(int x) { const char *s = \"FAKEMATCH: fake\"; return Target(x); }";
        let found = forwarding_findings(text, "A.C").unwrap();
        assert_eq!(
            found
                .iter()
                .map(|item| item.name.as_str())
                .collect::<Vec<_>>(),
            ["Forward", "Pointer"]
        );
    }
    #[test]
    fn zero_iterations_loop_backedges_and_switch_entries_preserve_targets() {
        let text = "static inline int Help(int x) { /* FAKEMATCH: measured. */ return x; }\nint Zero(int n) { int (*f)(int) = Help; while(n) f = Plain; return f(1); }\nint Back(int n) { int (*f)(int) = Plain; while(n) { f(1); f = Help; } return 0; }\nint Cases(int n) { int (*f)(int) = Help; switch(n) { case 0: f = Plain; break; case 1: return f(1); } return 0; }";
        let result = analyze(text, "A.C").unwrap();
        for name in ["Zero", "Back", "Cases"] {
            assert!(result.steered.contains(name), "{name}: {result:?}");
        }
    }
    #[test]
    fn raw_production_header_adapters_keep_zero_one_and_two_typed_arguments() {
        let source = "#include \"TYPES.H\"\nstatic inline void Event_Begin(void) { Engine_EventBegin(); }\nstatic inline void Event_Wait(s32 frames) { Engine_EventWait(frames); }\nstatic inline void Actor_SetSpeed(s32 channel, s32 speed) { Engine_ActorSetSpeed(channel, speed); }";
        assert_eq!(
            forwarding_findings(source, "FIELD_EVENT.H").unwrap().len(),
            3
        );
    }
    #[test]
    fn production_copy_adapter_parameter_permutations_are_forwarders() {
        let source = "static inline s32 CopyWords(const void *src, CopyFn copy, void *dst, s32 size) { return copy(dst, (const void *)src, size); }\nstatic inline s32 Duplicate(s32 a, s32 b) { return Target(a, a); }";
        let found = forwarding_findings(source, "MENU9.C").unwrap();
        assert_eq!(
            found
                .iter()
                .map(|item| item.name.as_str())
                .collect::<Vec<_>>(),
            ["CopyWords"]
        );
    }
    #[test]
    fn do_while_backedges_keep_later_targets_without_inventing_zero_iterations() {
        let text = "static inline int Help(int x) { /* FAKEMATCH: measured. */ return x; }\nint Back(int n) { int (*f)(int) = Plain; do { f(1); f = Help; } while(n); return 0; }\nint Killed(int n) { int (*f)(int) = Help; do { f = Plain; } while(n); return f(1); }";
        let result = analyze(text, "A.C").unwrap();
        assert!(result.steered.contains("Back"));
        assert!(!result.steered.contains("Killed"));
    }
    #[test]
    fn file_scope_literal_pool_tag_marks_only_its_sized_function() {
        let text = "void Steered(void) { /* FAKEMATCH: literal load. */ asm(\"ldr r0, .LPool\"); }\n/* FAKEMATCH: complete source-owned pool. */\nasm(\".align 2\\n.LPool:\\n.word Table\\n.size Steered,.-Steered\");\nvoid Plain(void) {}";
        let result = analyze(text, "A.C").unwrap();
        assert_eq!(result.steered, ["Steered".to_string()].into());
        assert!(result.whole_owners.is_empty());
    }
}
