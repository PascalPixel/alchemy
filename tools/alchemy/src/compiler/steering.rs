use super::c_syntax::{lex, lex_branches, matching, matching_in, scan_definitions, scan_definitions_in, Tok, Token};
use std::collections::{BTreeMap, BTreeSet};

#[derive(Clone, Debug, Default)]
pub(crate) struct Analysis {
    pub steered: BTreeSet<String>,
    pub whole_owners: BTreeSet<String>,
    /// Why each steered function is steered: a tag as `file:line`, a
    /// file-scope tag as `file`, or `via HELPER` for a steered inline helper.
    pub reasons: BTreeMap<String, BTreeSet<String>>,
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

fn definitions(tokens: &[Token], origins: &[(String, usize)]) -> Vec<Definition> {
    scan_definitions(tokens).into_iter().filter_map(|(name, boundary, name_index, params_end, open)| {
        let close = matching(tokens, open)?;
        let (owner, line) = origins.get(tokens[name_index].line - 1).cloned().unwrap_or_default();
        let header = &tokens[boundary..name_index];
        Some(Definition {
            name, owner, line, boundary, name_index, params_end, open, close,
            inline: header.iter().any(|token| matches!(&token.tok, Tok::Ident(word) if ["inline", "__inline", "__inline__"].contains(&word.as_str()))),
            static_: header.iter().any(|token| matches!(&token.tok, Tok::Ident(word) if word == "static")),
        })
    }).collect()
}

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

fn names_used(body: &[Token], names: &BTreeSet<String>) -> BTreeSet<String> {
    body.iter()
        .enumerate()
        .filter_map(|(at, token)| match &token.tok {
            Tok::Ident(name) if names.contains(name) && is_use(body, at) => Some(name.clone()),
            _ => None,
        })
        .collect()
}

fn is_use(body: &[Token], at: usize) -> bool {
    match body[..at]
        .iter()
        .rev()
        .find(|token| !matches!(token.tok, Tok::Comment(_)))
        .map(|token| &token.tok)
    {
        Some(Tok::Punct("." | "->")) => false,
        Some(Tok::Ident(word)) => ["return", "else", "do", "case", "sizeof"].contains(&word.as_str()),
        _ => true,
    }
}

pub(crate) fn analyze(text: &str, source: &str) -> Result<Analysis, String> {
    let tokens = lex(text)?;
    let locations = origins(text, source);
    let defs = definitions(&tokens, &locations);
    let helpers = defs
        .iter()
        .filter(|def| def.inline)
        .map(|def| def.name.clone())
        .collect::<BTreeSet<_>>();
    let mut result = Analysis::default();
    for (index, token) in tokens.iter().enumerate() {
        if matches!(&token.tok, Tok::Comment(comment) if comment.contains("FAKEMATCH")) {
            let location = locations.get(token.line - 1);
            let tag_owner = location.map(|(owner, _)| owner);
            let tag = location.map_or_else(String::new, |(owner, line)| format!("{owner}:{line}"));
            if let Some(def) = defs.iter().find(|def| {
                tag_owner == Some(&def.owner) && (def.boundary..=def.close).contains(&index)
            }) {
                result.steered.insert(def.name.clone());
                result.reasons.entry(def.name.clone()).or_default().insert(tag);
            } else if let Some(owners) = pool_owners(&tokens, index) {
                for owner in &owners {
                    result.reasons.entry(owner.clone()).or_default().insert(tag.clone());
                }
                result.steered.extend(owners);
            } else if let Some((owner, _)) = location {
                result.whole_owners.insert(owner.clone());
            }
        }
    }
    for def in &defs {
        if result.whole_owners.contains(&def.owner)
            || std::path::Path::new(&def.owner).ends_with("games/common/include/lib/call.h")
        {
            result.steered.insert(def.name.clone());
            result.reasons.entry(def.name.clone()).or_default().insert(def.owner.clone());
        }
    }
    loop {
        let relevant = result
            .steered
            .intersection(&helpers)
            .cloned()
            .collect::<BTreeSet<_>>();
        let reached = defs
            .iter()
            .filter(|def| !result.steered.contains(&def.name))
            .map(|def| (def.name.clone(), names_used(&tokens[def.open + 1..def.close], &relevant)))
            .filter(|(_, used)| !used.is_empty())
            .collect::<Vec<_>>();
        if reached.is_empty() {
            break;
        }
        for (name, used) in reached {
            let reasons = result.reasons.entry(name.clone()).or_default();
            reasons.extend(used.into_iter().map(|helper| format!("via {helper}")));
            result.steered.insert(name);
        }
    }
    Ok(result)
}

/// Each function of a C file, read before preprocessing, whose body uses
/// a macro a FAKEMATCH tag explains, with the macros it uses: the tag sits
/// above the macro's #define, so it is in no function the preprocessed
/// text holds, and the functions that expand it are the steered ones.
/// Read as the C preprocessor reads one edition, so a brace opened in both
/// arms of an #if/#else does not hide the functions after it.
pub(crate) fn macro_users(raw: &str, tagged: &BTreeSet<String>) -> BTreeMap<String, BTreeSet<String>> {
    let mut found: BTreeMap<String, BTreeSet<String>> = BTreeMap::new();
    let Ok((tokens, branches)) = lex_branches(raw) else { return found };
    for (name, _, _, _, open) in scan_definitions_in(&tokens, &branches) {
        let Some(close) = matching_in(&tokens, open, &branches) else { continue };
        for token in &tokens[open + 1..close] {
            if let Tok::Ident(word) = &token.tok {
                if tagged.contains(word) {
                    found.entry(name.clone()).or_default().insert(word.clone());
                }
            }
        }
    }
    found
}

/// Whether these tokens hold a FAKEMATCH tag with its reason.
pub(crate) fn has_reason(tokens: &[Token]) -> bool {
    tokens.iter().any(|token| match &token.tok {
        Tok::Comment(comment) => comment
            .split_once("FAKEMATCH:")
            .is_some_and(|(_, reason)| !reason.trim().trim_end_matches("*/").trim().is_empty()),
        _ => false,
    })
}

fn close_of(tokens: &[&Tok], open: usize) -> Option<usize> {
    let mut depth = 0usize;
    for (index, tok) in tokens.iter().enumerate().skip(open) {
        match tok {
            Tok::Punct("(" | "[" | "{") => depth += 1,
            Tok::Punct(")" | "]" | "}") => {
                depth = depth.checked_sub(1)?;
                if depth == 0 {
                    return Some(index);
                }
            }
            _ => {}
        }
    }
    None
}

fn pieces<'a>(tokens: &'a [&'a Tok]) -> Option<Vec<&'a [&'a Tok]>> {
    let mut pieces = Vec::new();
    let (mut start, mut at) = (0, 0);
    while at < tokens.len() {
        match tokens[at] {
            Tok::Punct("(" | "[" | "{") => at = close_of(tokens, at)?,
            Tok::Punct(",") => {
                pieces.push(&tokens[start..at]);
                start = at + 1;
            }
            _ => {}
        }
        at += 1;
    }
    pieces.push(&tokens[start..]);
    Some(pieces)
}

fn operand(mut tokens: &[&Tok], unary: bool) -> Option<String> {
    loop {
        match tokens {
            [Tok::Ident(name)] => return Some(name.clone()),
            [Tok::Punct("*" | "&"), rest @ ..] if unary => tokens = rest,
            [Tok::Punct("("), ..] => {
                let close = close_of(tokens, 0)?;
                tokens = if close + 1 == tokens.len() {
                    &tokens[1..close]
                } else {
                    &tokens[close + 1..]
                };
            }
            _ => return None,
        }
    }
}

/// The callee and the names passed, through casts and parentheses, of a
/// function body (comments left out) that is one call, returned or not.
pub(crate) fn forwarded_call(body: &[&Tok]) -> Option<(String, Vec<String>)> {
    let statements = body
        .split(|tok| matches!(tok, Tok::Punct(";")))
        .filter(|statement| !statement.is_empty())
        .collect::<Vec<_>>();
    let ([statement], Some(Tok::Punct(";"))) = (statements.as_slice(), body.last()) else {
        return None;
    };
    let expression = match statement {
        [Tok::Ident(word), rest @ ..] if word == "return" => rest,
        _ => statement,
    };
    if expression
        .iter()
        .any(|tok| matches!(tok, Tok::Punct("{" | "}" | ":" | "?" | "=")))
        || matches!(expression.first(), Some(Tok::Ident(word)) if [
            "if", "else", "while", "for", "do", "switch", "goto", "break", "continue", "case",
            "default", "return",
        ]
        .contains(&word.as_str()))
    {
        return None;
    }
    let [callee @ .., Tok::Punct(")")] = expression else {
        return None;
    };
    let mut depth = 0usize;
    let open = (0..expression.len()).rev().find(|&at| {
        match expression[at] {
            Tok::Punct(")") => depth += 1,
            Tok::Punct("(") => depth -= 1,
            _ => {}
        }
        depth == 0
    })?;
    let callee = operand(&callee[..open], true)?;
    let args = &expression[open + 1..expression.len() - 1];
    let args = if args.is_empty() {
        Vec::new()
    } else {
        pieces(args)?
            .into_iter()
            .map(|arg| operand(arg, false))
            .collect::<Option<Vec<_>>>()?
    };
    Some((callee, args))
}

/// The names a parameter list declares, the one in `(*name)` for a
/// function pointer; none for `(void)`.
pub(crate) fn parameter_names(tokens: &[&Tok]) -> Option<Vec<String>> {
    if matches!(tokens, [] | [Tok::Ident(_)]) {
        return Some(Vec::new());
    }
    pieces(tokens)?
        .into_iter()
        .map(|piece| {
            let mut name = None;
            let mut at = 0;
            while at < piece.len() {
                match piece[at] {
                    Tok::Punct("(") => {
                        let close = close_of(piece, at)?;
                        if matches!(piece.get(at + 1), Some(Tok::Punct("*"))) {
                            return piece[at..close].iter().rev().find_map(|tok| match tok {
                                Tok::Ident(name) => Some(name.clone()),
                                _ => None,
                            });
                        }
                        at = close;
                    }
                    Tok::Punct("[") => at = close_of(piece, at)?,
                    Tok::Ident(word) => name = Some(word.clone()),
                    _ => {}
                }
                at += 1;
            }
            name
        })
        .collect()
}

/// Headers whose forwarding inlines are a layer of the whole game (S2), not
/// devices: the scene script layer; the IWRAM call layer, the one inline per
/// resident IWRAM routine that every caller of that routine goes through; and
/// the ☀️ engine call layer, the one inline per main-image routine it names,
/// which every overlay call of that routine goes through (Pascal, 2026-10-13).
pub(crate) const SCRIPT_LAYER: &[&str] = &[
    "games/tbs/include/field_event.h",
    "games/tbs/include/field_service.h",
    "games/tbs/src/scene/runpa_jo.h",
    "games/tbs/include/iwram_call.h",
    "games/tbs/include/engcall.h",
];

pub(crate) fn forwarding_findings(text: &str, source: &str) -> Result<Vec<Forwarder>, String> {
    let tokens = lex(text)?;
    let locations = origins(text, source);
    let defs = definitions(&tokens, &locations);
    let code = |range: std::ops::Range<usize>| {
        tokens[range]
            .iter()
            .map(|token| &token.tok)
            .filter(|tok| !matches!(tok, Tok::Comment(_)))
            .collect::<Vec<_>>()
    };
    let mut found = Vec::new();
    for def in defs
        .iter()
        .filter(|def| def.inline && def.static_ && !SCRIPT_LAYER.contains(&def.owner.as_str()))
    {
        if has_reason(&tokens[def.open + 1..def.close]) {
            continue;
        }
        let Some((callee, mut args)) = forwarded_call(&code(def.open + 1..def.close)) else {
            continue;
        };
        let Some(mut names) = parameter_names(&code(def.name_index + 2..def.params_end)) else {
            continue;
        };
        names.retain(|name| *name != callee);
        names.sort();
        args.sort();
        if args == names {
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
        let text = "# 1 \"games/common/include/lib/call.h\"\nstatic inline void Call1(void (*f)(), int x) { f(x); }\n# 1 \"games/tbs/src/a.c\"\nstatic inline void Adapter(int x) { Call1(Target, x); }\nvoid Caller(void) { Adapter(1); }\nvoid Plain(void) { Target(2); }\n";
        let analysis = analyze(text, "games/tbs/src/a.c").unwrap();
        assert_eq!(
            analysis.steered,
            ["Adapter", "Call1", "Caller"].map(String::from).into()
        );
    }
    #[test]
    fn functions_that_expand_a_tagged_macro_are_steered() {
        let raw = "#define SET(v) do { g = (v); } while (0)\nvoid Uses(void)\n{\n\tSET(1);\n}\nvoid Plain(void)\n{\n\tg = 2;\n}\n";
        let tagged = BTreeSet::from(["SET".to_string()]);
        let users = macro_users(raw, &tagged);
        assert_eq!(users.keys().collect::<Vec<_>>(), vec!["Uses"]);
    }

    #[test]
    fn a_brace_opened_in_both_arms_hides_no_later_macro_user() {
        // window.c: UiText_AppendArticleName opens `if (...) {` in both arms;
        // the option menu after it still expands the tagged Dma_Fill32.
        let raw = "\
#define SET(v) do { g = (v); } while (0)
u32 Article(s32 no)
{
#if defined(TBS_EDITION_ES)
\tif (count == 0)
\t{
#else
\tif (no == 0)
\t{
#endif
\t\tSET(1);
\t}
\treturn 0;
}
void Option(void)
{
\tSET(2);
}
";
        let tagged = BTreeSet::from(["SET".to_string()]);
        let users = macro_users(raw, &tagged);
        assert_eq!(users.keys().collect::<Vec<_>>(), vec!["Article", "Option"]);
    }

    #[test]
    fn named_helpers_steer_and_noninline_calls_and_members_do_not() {
        let text = "static inline int Help(int x) { /* FAKEMATCH: measured. */ return x; }\nint Tagged(int x) { /* FAKEMATCH: measured. */ return x; }\nint Alias(void) { int (*f)(int) = Help; return f(1); }\nint Ordinary(void) { return Tagged(1); }\nint Member(struct S *s) { return s->Help; }\n";
        let analysis = analyze(text, "a.c").unwrap();
        assert_eq!(
            analysis.steered,
            ["Alias", "Help", "Tagged"].map(String::from).into()
        );
    }
    #[test]
    fn reasons_name_the_tag_or_the_steered_helper() {
        let text = "static inline int Help(int x) { /* FAKEMATCH: measured. */ return x; }\nint Caller(void) { return Help(1); }\nint Tagged(void) {\n /* FAKEMATCH: own. */ return 2; }\n";
        let result = analyze(text, "a.c").unwrap();
        assert_eq!(result.reasons["Caller"], BTreeSet::from(["via Help".to_string()]));
        assert_eq!(result.reasons["Tagged"].len(), 1);
        assert!(result.reasons["Tagged"].iter().next().unwrap().ends_with("a.c:4"));
    }

    #[test]
    fn cycles_and_macro_bodies_remain_function_specific() {
        let text = "static inline void A(void) { /* FAKEMATCH: measured. */ B(); }\nstatic inline void B(void) { A(); }\nvoid Expanded(void) { /* FAKEMATCH: preserved macro. */ A(); }\nvoid Plain(void) {}";
        assert_eq!(
            analyze(text, "a.c").unwrap().steered,
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
        assert!(analyze(text, "a.c").unwrap().steered.contains("Caller"));
    }
    #[test]
    fn forwarders_need_reason_inside_their_own_body() {
        let text = "/* FAKEMATCH: preceding tag does not admit helper. */\nstatic inline int Forward(int x) { return (int)Target((int)x); }\nstatic inline void Pointer(void (*f)(int), int x) { f(x); }\nstatic inline int Tagged(int x) { /* FAKEMATCH: measured. */ return Target(x); }\nstatic inline int Compute(int x) { return Target(x + 1); }\nstatic inline int Fake(int x) { const char *s = \"FAKEMATCH: fake\"; return Target(x); }";
        let found = forwarding_findings(text, "a.c").unwrap();
        assert_eq!(
            found
                .iter()
                .map(|item| item.name.as_str())
                .collect::<Vec<_>>(),
            ["Forward", "Pointer"]
        );
    }
    #[test]
    fn raw_production_header_adapters_keep_zero_one_and_two_typed_arguments() {
        let source = "#include \"types.h\"\nstatic inline void Event_Begin(void) { Engine_EventBegin(); }\nstatic inline void Event_Wait(s32 frames) { Engine_EventWait(frames); }\nstatic inline void Actor_SetSpeed(s32 channel, s32 speed) { Engine_ActorSetSpeed(channel, speed); }";
        assert_eq!(
            forwarding_findings(source, "field_event.h").unwrap().len(),
            3
        );
    }
    #[test]
    fn iwram_call_layer_is_admitted_only_in_its_own_header() {
        let layer = "static __inline__ s32 Iwram_CopyWords(void *destination, const void *source, s32 size) { return Iwram_CopyWordsEntry(destination, source, size); }";
        assert!(forwarding_findings(layer, "games/tbs/include/iwram_call.h")
            .unwrap()
            .is_empty());
        for elsewhere in ["games/tbs/src/menu/backdrop.c", "games/tla/include/iwram_call.h"] {
            assert_eq!(forwarding_findings(layer, elsewhere).unwrap().len(), 1);
        }
        let wrapper = "# 1 \"games/tbs/include/iwram_call.h\"\nstatic __inline__ s32 Iwram_CopyWords(void *d, const void *s, s32 n) { return Iwram_CopyWordsEntry(d, s, n); }\n# 1 \"games/tbs/src/menu/backdrop.c\"\nstatic __inline__ s32 CopyWords(void *d, const void *s, s32 n) { return Iwram_CopyWords(d, s, n); }\n";
        let found = forwarding_findings(wrapper, "games/tbs/src/menu/backdrop.c").unwrap();
        assert_eq!(
            found
                .iter()
                .map(|item| item.name.as_str())
                .collect::<Vec<_>>(),
            ["CopyWords"]
        );
    }
    #[test]
    fn engine_call_layer_is_admitted_only_in_its_own_header() {
        let layer = "static inline s32 FieldGameFlag_GetByte(s32 flag) { return GameFlag_GetByte(flag); }";
        assert!(forwarding_findings(layer, "games/tbs/include/engcall.h")
            .unwrap()
            .is_empty());
        for elsewhere in ["games/tbs/src/scene/world_map.c", "games/tla/include/engcall.h"] {
            assert_eq!(forwarding_findings(layer, elsewhere).unwrap().len(), 1);
        }
    }
    #[test]
    fn production_copy_adapter_parameter_permutations_are_forwarders() {
        let source = "static inline s32 CopyWords(const void *src, CopyFn copy, void *dst, s32 size) { return copy(dst, (const void *)src, size); }\nstatic inline s32 Duplicate(s32 a, s32 b) { return Target(a, a); }";
        let found = forwarding_findings(source, "cancel.c").unwrap();
        assert_eq!(
            found
                .iter()
                .map(|item| item.name.as_str())
                .collect::<Vec<_>>(),
            ["CopyWords"]
        );
    }
    #[test]
    fn file_scope_literal_pool_tag_marks_only_its_sized_function() {
        let text = "void Steered(void) { /* FAKEMATCH: literal load. */ asm(\"ldr r0, .LPool\"); }\n/* FAKEMATCH: complete source-owned pool. */\nasm(\".align 2\\n.LPool:\\n.word Table\\n.size Steered,.-Steered\");\nvoid Plain(void) {}";
        let result = analyze(text, "a.c").unwrap();
        assert_eq!(result.steered, ["Steered".to_string()].into());
        assert!(result.whole_owners.is_empty());
    }
}
