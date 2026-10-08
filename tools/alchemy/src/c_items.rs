use crate::compiler::c_syntax::{lex, scan_definitions, Tok};

#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) enum Kind {
    Comment,
    Include(String),
    Directive,
    Conditional,
    Declaration,
    Function { body: usize },
}

#[derive(Clone, Debug)]
pub(crate) struct Item<'a> {
    pub start: usize,
    pub end: usize,
    pub text: &'a str,
    pub kind: Kind,
}

#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash, PartialOrd, Ord)]
pub(crate) enum Space {
    Ordinary,
    Tag,
    Macro,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Name {
    pub space: Space,
    pub name: String,
    pub defines: bool,
    pub signature: String,
}

impl Name {
    fn new(space: Space, name: &str, defines: bool, signature: String) -> Name {
        Name { space, name: name.to_string(), defines, signature }
    }

    pub(crate) fn describe(&self) -> String {
        match self.space {
            Space::Tag => self.signature.clone(),
            Space::Macro => format!("macro {}", self.name),
            Space::Ordinary => self.name.clone(),
        }
    }
}

fn line_start(bytes: &[u8], at: usize) -> bool {
    bytes[..at].iter().rev().take_while(|b| **b != b'\n').all(|b| *b == b' ' || *b == b'\t')
}

fn comment_end(source: &str, at: usize) -> Option<usize> {
    let rest = &source[at..];
    if let Some(after) = rest.strip_prefix("/*") {
        Some(after.find("*/").map_or(source.len(), |e| at + 2 + e + 2))
    } else if rest.starts_with("//") {
        Some(rest.find('\n').map_or(source.len(), |e| at + e))
    } else {
        None
    }
}

fn literal_end(bytes: &[u8], at: usize) -> usize {
    let quote = bytes[at];
    let mut i = at + 1;
    while i < bytes.len() && bytes[i] != quote && bytes[i] != b'\n' {
        i += if bytes[i] == b'\\' { 2 } else { 1 };
    }
    (i + 1).min(bytes.len())
}

fn directive_end(source: &str, at: usize) -> usize {
    let bytes = source.as_bytes();
    let mut i = at;
    while i < bytes.len() && bytes[i] != b'\n' {
        if let Some(end) = comment_end(source, i) {
            i = end;
        } else if bytes[i] == b'"' || bytes[i] == b'\'' {
            i = literal_end(bytes, i);
        } else if bytes[i] == b'\\' && bytes.get(i + 1) == Some(&b'\n') {
            i += 2;
        } else {
            i += 1;
        }
    }
    i
}

fn directive_name(line: &str) -> &str {
    let rest = line.trim_start().trim_start_matches('#').trim_start();
    let end = rest.find(|c: char| !c.is_ascii_alphanumeric() && c != '_').unwrap_or(rest.len());
    &rest[..end]
}

fn conditional_end(source: &str, at: usize) -> usize {
    let bytes = source.as_bytes();
    let mut depth = 0usize;
    let mut i = at;
    while i < bytes.len() {
        if let Some(end) = comment_end(source, i) {
            i = end;
        } else if bytes[i] == b'"' || bytes[i] == b'\'' {
            i = literal_end(bytes, i);
        } else if bytes[i] == b'#' && line_start(bytes, i) {
            let end = directive_end(source, i);
            match directive_name(&source[i..end]) {
                "if" | "ifdef" | "ifndef" => depth += 1,
                "endif" => {
                    depth = depth.saturating_sub(1);
                    if depth == 0 {
                        return end;
                    }
                }
                _ => {}
            }
            i = end;
        } else {
            i += 1;
        }
    }
    source.len()
}

fn statement_end(source: &str, at: usize) -> (usize, Option<usize>) {
    let bytes = source.as_bytes();
    let (mut depth, mut i) = (0usize, at);
    let (mut last, mut assigned, mut body) = (0u8, false, None);
    while i < bytes.len() {
        let byte = bytes[i];
        if let Some(end) = comment_end(source, i) {
            i = end;
            continue;
        }
        if byte == b'#' && line_start(bytes, i) {
            i = directive_end(source, i);
            continue;
        }
        if byte == b'"' || byte == b'\'' {
            i = literal_end(bytes, i);
            last = byte;
            continue;
        }
        match byte {
            b'(' | b'[' => depth += 1,
            b'{' => {
                if depth == 0 && last == b')' && !assigned {
                    body = Some(i);
                }
                depth += 1;
            }
            b')' | b']' => depth = depth.saturating_sub(1),
            b'}' => {
                depth = depth.saturating_sub(1);
                if depth == 0 && body.is_some() {
                    return (i + 1, body);
                }
            }
            b';' if depth == 0 => return (i + 1, None),
            b'=' if depth == 0 => assigned = true,
            _ => {}
        }
        if !byte.is_ascii_whitespace() {
            last = byte;
        }
        i += 1;
    }
    (source.len(), None)
}

fn trailing_comments(source: &str, mut end: usize) -> usize {
    let bytes = source.as_bytes();
    loop {
        let mut i = end;
        while i < bytes.len() && (bytes[i] == b' ' || bytes[i] == b'\t') {
            i += 1;
        }
        match comment_end(source, i) {
            Some(after) => end = after,
            None => return end,
        }
    }
}

fn include_header(line: &str) -> String {
    let rest = line.trim_start().trim_start_matches('#').trim_start();
    let rest = rest.strip_prefix("include").unwrap_or(rest).trim();
    rest.trim_start_matches(['"', '<']).split(['"', '>']).next().unwrap_or("").to_string()
}

pub(crate) fn items(source: &str) -> Vec<Item<'_>> {
    let bytes = source.as_bytes();
    let mut out = Vec::new();
    let mut i = 0;
    while i < bytes.len() {
        if bytes[i].is_ascii_whitespace() {
            i += 1;
            continue;
        }
        let start = i;
        let (end, kind) = if let Some(end) = comment_end(source, i) {
            (end, Kind::Comment)
        } else if bytes[i] == b'#' && line_start(bytes, i) {
            let line_end = directive_end(source, i);
            let line = &source[i..line_end];
            match directive_name(line) {
                "include" => (trailing_comments(source, line_end), Kind::Include(include_header(line))),
                "if" | "ifdef" | "ifndef" => (conditional_end(source, i), Kind::Conditional),
                _ => (line_end, Kind::Directive),
            }
        } else {
            let (end, body) = statement_end(source, i);
            let kind = body.map_or(Kind::Declaration, |b| Kind::Function { body: b - start });
            (trailing_comments(source, end), kind)
        };
        out.push(Item { start, end, text: &source[start..end], kind });
        i = end.max(start + 1);
    }
    out
}

pub(crate) fn blank_line_before(source: &str, at: usize) -> bool {
    let gap: &str = source[..at].trim_end_matches([' ', '\t']);
    let gap = &gap[gap.trim_end().len()..];
    gap.matches('\n').count() >= 2
}

fn token_text(tok: &Tok) -> &str {
    match tok {
        Tok::Ident(s) | Tok::Number(s) | Tok::Char(s) | Tok::Str(s) | Tok::Comment(s) => s,
        Tok::Punct(p) => p,
    }
}

pub(crate) fn normalized(item: &Item) -> String {
    match item.kind {
        Kind::Declaration | Kind::Function { .. } => match lex(item.text) {
            Ok(tokens) => tokens
                .iter()
                .filter(|t| !matches!(t.tok, Tok::Comment(_)))
                .map(|t| token_text(&t.tok))
                .collect::<Vec<_>>()
                .join(" "),
            Err(_) => collapse(item.text),
        },
        _ => {
            let joined = without_comments(item.text).replace("\\\n", " ");
            let lines: Vec<String> = joined
                .lines()
                .map(|line| match line.trim_start().strip_prefix('#') {
                    Some(rest) => format!("#{}", collapse(rest)),
                    None => collapse(line),
                })
                .filter(|line| !line.is_empty())
                .collect();
            lines.join("\n")
        }
    }
}

fn collapse(text: &str) -> String {
    text.split_whitespace().collect::<Vec<_>>().join(" ")
}

fn without_comments(text: &str) -> String {
    let bytes = text.as_bytes();
    let (mut out, mut i) = (String::new(), 0);
    while i < bytes.len() {
        if let Some(end) = comment_end(text, i) {
            out.push(' ');
            i = end;
        } else if bytes[i] == b'"' || bytes[i] == b'\'' {
            let end = literal_end(bytes, i);
            out.push_str(&text[i..end]);
            i = end;
        } else {
            let next = text[i..].chars().next().map_or(1, char::len_utf8);
            out.push_str(&text[i..i + next]);
            i += next;
        }
    }
    out
}

const STORAGE: &[&str] = &[
    "typedef", "extern", "static", "auto", "register", "inline", "__inline", "__inline__",
    "__extension__", "const", "volatile", "__const", "__volatile", "__volatile__",
];
const BASIC: &[&str] = &[
    "void", "char", "short", "int", "long", "float", "double", "signed", "unsigned", "__signed__",
];

fn close(toks: &[&Tok], open: usize) -> usize {
    let mut depth = 0usize;
    for (i, tok) in toks.iter().enumerate().skip(open) {
        match tok {
            Tok::Punct("(" | "[" | "{") => depth += 1,
            Tok::Punct(")" | "]" | "}") => {
                depth -= 1;
                if depth == 0 {
                    return i;
                }
            }
            _ => {}
        }
    }
    toks.len().saturating_sub(1)
}

fn is(tok: Option<&&Tok>, punct: &str) -> bool {
    matches!(tok, Some(Tok::Punct(p)) if *p == punct)
}

fn ident<'t>(tok: Option<&&'t Tok>) -> Option<&'t str> {
    match tok {
        Some(Tok::Ident(s)) => Some(s),
        _ => None,
    }
}

struct Specifiers {
    end: usize,
    typedef: bool,
    external: bool,
    bare_tag: Option<String>,
}

fn specifiers(toks: &[&Tok]) -> Specifiers {
    let mut spec = Specifiers { end: toks.len(), typedef: false, external: false, bare_tag: None };
    let (mut typed, mut i) = (false, 0);
    while i < toks.len() {
        let Some(word) = ident(toks.get(i)) else {
            spec.end = i;
            return spec;
        };
        match word {
            "typedef" => spec.typedef = true,
            "extern" => spec.external = true,
            "__attribute__" if is(toks.get(i + 1), "(") => i = close(toks, i + 1),
            "struct" | "union" | "enum" => {
                typed = true;
                let tag = ident(toks.get(i + 1));
                let mut next = i + 1 + usize::from(tag.is_some());
                if is(toks.get(next), "{") {
                    next = close(toks, next) + 1;
                } else {
                    spec.bare_tag = tag.map(str::to_string);
                }
                i = next;
                continue;
            }
            w if STORAGE.contains(&w) => {}
            w if BASIC.contains(&w) => typed = true,
            _ if !typed => typed = true,
            _ => {
                spec.end = i;
                return spec;
            }
        }
        i += 1;
    }
    spec
}

fn name_index(toks: &[&Tok]) -> Option<usize> {
    let mut i = 0;
    while i < toks.len() {
        match toks[i] {
            Tok::Punct("*" | "(") => {}
            Tok::Ident(w) if w == "__attribute__" => i = close(toks, i + 1),
            Tok::Ident(w) if STORAGE.contains(&w.as_str()) => {}
            Tok::Ident(_) => return Some(i),
            _ => return None,
        }
        i += 1;
    }
    None
}

fn declarator(toks: &[&Tok]) -> Option<(String, bool, bool)> {
    let at = name_index(toks)?;
    let Tok::Ident(name) = toks[at] else {
        return None;
    };
    let function = is(toks.get(at + 1), "(");
    let initialized = toks.iter().any(|t| matches!(t, Tok::Punct("=")));
    Some((name.clone(), function, initialized))
}

fn signature(specs: &[&Tok], declarator: &[&Tok]) -> String {
    let mut out: Vec<&str> = specs
        .iter()
        .map(|t| token_text(t))
        .filter(|t| *t != "extern")
        .collect();
    let end = declarator.iter().position(|t| matches!(t, Tok::Punct("="))).unwrap_or(declarator.len());
    let d = &declarator[..end];
    let texts: Vec<&str> = d.iter().map(|t| token_text(t)).collect();
    match name_index(d).filter(|&at| is(d.get(at + 1), "(")) {
        Some(at) => {
            let close_at = close(d, at + 1);
            out.extend(&texts[..at + 2]);
            for (n, parameter) in split_commas(&d[at + 2..close_at]).into_iter().enumerate() {
                if n > 0 {
                    out.push(",");
                }
                let spec = specifiers(parameter);
                let name = name_index(&parameter[spec.end..]).map(|i| spec.end + i);
                out.extend(
                    parameter
                        .iter()
                        .enumerate()
                        .filter(|(i, _)| Some(*i) != name)
                        .map(|(_, t)| token_text(t)),
                );
            }
            out.extend(&texts[close_at..]);
        }
        None => out.extend(texts),
    }
    out.join(" ")
}

fn split_commas<'s, 't>(toks: &'s [&'t Tok]) -> Vec<&'s [&'t Tok]> {
    let (mut parts, mut depth, mut from) = (Vec::new(), 0usize, 0);
    for (i, tok) in toks.iter().enumerate() {
        match tok {
            Tok::Punct("(" | "[" | "{") => depth += 1,
            Tok::Punct(")" | "]" | "}") => depth = depth.saturating_sub(1),
            Tok::Punct(",") if depth == 0 => {
                parts.push(&toks[from..i]);
                from = i + 1;
            }
            _ => {}
        }
    }
    parts.push(&toks[from..]);
    parts.into_iter().filter(|p| !p.is_empty()).collect()
}

fn declarators<'s, 't>(rest: &'s [&'t Tok]) -> Vec<&'s [&'t Tok]> {
    match rest.last() {
        Some(Tok::Punct(";")) => split_commas(&rest[..rest.len() - 1]),
        _ => split_commas(rest),
    }
}

fn tag_definitions(toks: &[&Tok], names: &mut Vec<Name>) {
    for i in 0..toks.len() {
        let Some(keyword @ ("struct" | "union" | "enum")) = ident(toks.get(i)) else {
            continue;
        };
        let tag = ident(toks.get(i + 1));
        let open = i + 1 + usize::from(tag.is_some());
        if !is(toks.get(open), "{") {
            continue;
        }
        if let Some(tag) = tag {
            names.push(Name::new(Space::Tag, tag, true, format!("{keyword} {tag}")));
        }
        if keyword == "enum" {
            let end = close(toks, open);
            let body = &toks[open + 1..end];
            for part in split_commas(body) {
                if let Some(constant) = ident(part.first()) {
                    names.push(Name::new(Space::Ordinary, constant, true, constant.to_string()));
                }
            }
        }
    }
}

fn declaration_names(toks: &[&Tok]) -> Vec<Name> {
    let mut names = Vec::new();
    tag_definitions(toks, &mut names);
    let spec = specifiers(toks);
    let specs = &toks[..spec.end.min(toks.len())];
    let declarators = declarators(&toks[specs.len()..]);
    if declarators.is_empty() {
        if let Some(tag) = &spec.bare_tag {
            let keyword = specs.iter().find_map(|t| ident(Some(t)).filter(|w| matches!(*w, "struct" | "union" | "enum")));
            names.push(Name::new(Space::Tag, tag, false, format!("{} {tag}", keyword.unwrap_or("struct"))));
        }
    }
    for part in declarators {
        if let Some((name, function, initialized)) = declarator(part) {
            let defines = spec.typedef || (!function && (initialized || !spec.external));
            names.push(Name::new(Space::Ordinary, &name, defines, signature(specs, part)));
        }
    }
    names
}

pub(crate) fn names(item: &Item) -> Vec<Name> {
    match &item.kind {
        Kind::Comment | Kind::Include(_) => Vec::new(),
        Kind::Directive => {
            let rest = item.text.trim_start().trim_start_matches('#').trim_start();
            match rest.strip_prefix("define") {
                Some(macro_text) if macro_text.starts_with([' ', '\t']) => {
                    let macro_text = macro_text.trim_start();
                    let end = macro_text
                        .find(|c: char| !c.is_ascii_alphanumeric() && c != '_')
                        .unwrap_or(macro_text.len());
                    vec![Name::new(Space::Macro, &macro_text[..end], true, normalized(item))]
                }
                _ => Vec::new(),
            }
        }
        Kind::Conditional => {
            let inner: String = item
                .text
                .lines()
                .map(|line| match directive_name(line) {
                    "if" | "ifdef" | "ifndef" | "elif" | "else" | "endif" if line.trim_start().starts_with('#') => "",
                    _ => line,
                })
                .collect::<Vec<_>>()
                .join("\n");
            items(&inner).iter().flat_map(names).collect()
        }
        Kind::Declaration | Kind::Function { .. } => {
            let Ok(tokens) = lex(item.text) else {
                return Vec::new();
            };
            let code = |tokens: &[crate::compiler::c_syntax::Token]| -> Vec<Tok> {
                tokens.iter().map(|t| t.tok.clone()).filter(|t| !matches!(t, Tok::Comment(_))).collect()
            };
            if matches!(item.kind, Kind::Function { .. }) {
                let Some((name, boundary, _, _, open)) = scan_definitions(&tokens).into_iter().next() else {
                    return Vec::new();
                };
                let head = code(&tokens[boundary..open]);
                let toks: Vec<&Tok> = head.iter().collect();
                let spec = specifiers(&toks);
                let end = spec.end.min(toks.len());
                return vec![Name::new(Space::Ordinary, &name, true, signature(&toks[..end], &toks[end..]))];
            }
            let all = code(&tokens);
            let toks: Vec<&Tok> = all.iter().collect();
            declaration_names(&toks)
        }
    }
}

pub(crate) fn line_marker(item: &Item) -> Option<String> {
    if item.kind != Kind::Directive {
        return None;
    }
    let rest = item.text.trim_start().strip_prefix('#')?.trim_start();
    let rest = rest.strip_prefix("line").unwrap_or(rest).trim_start();
    let digits = rest.find(|c: char| !c.is_ascii_digit()).unwrap_or(rest.len());
    if digits == 0 {
        return None;
    }
    let quoted = rest[digits..].trim_start().strip_prefix('"')?;
    Some(quoted[..quoted.find('"')?].to_string())
}

pub(crate) fn aggregate_types(item: &Item) -> Vec<String> {
    if item.kind != Kind::Declaration {
        return Vec::new();
    }
    let Ok(tokens) = lex(item.text) else {
        return Vec::new();
    };
    let toks: Vec<&Tok> = tokens.iter().map(|t| &t.tok).filter(|t| !matches!(t, Tok::Comment(_))).collect();
    let mut found = Vec::new();
    for i in 0..toks.len() {
        let Some(keyword @ ("struct" | "union")) = ident(toks.get(i)) else {
            continue;
        };
        match (ident(toks.get(i + 1)), is(toks.get(i + 1), "{"), is(toks.get(i + 2), "{")) {
            (Some(tag), _, true) => found.push(format!("{keyword} {tag}")),
            (None, true, _) if i == 0 || ident(toks.get(i - 1)) == Some("typedef") => {
                let spec = specifiers(&toks);
                if spec.typedef {
                    for part in declarators(&toks[spec.end.min(toks.len())..]) {
                        if let Some((name, false, _)) = declarator(part) {
                            if !part.iter().any(|t| matches!(t, Tok::Punct("*" | "["))) {
                                found.push(name);
                            }
                        }
                    }
                }
            }
            _ => {}
        }
    }
    found
}

#[cfg(test)]
mod tests {
    use super::*;

    fn kinds(source: &str) -> Vec<(Kind, &str)> {
        items(source).into_iter().map(|i| (i.kind, i.text)).collect()
    }

    #[test]
    fn splits_top_level_items_with_their_exact_text() {
        let source = "/* head */\n#include \"TYPES.H\"\n#define MASK(x) \\\n  ((x) & 1)\nstruct A { u8 a; }; /* note */\ns32 f(void);\n#if defined(X)\nu8 gX;\n#endif\nvoid g(void)\n{\n#if Y\n    f();\n#endif\n}\n";
        let found = kinds(source);
        assert_eq!(found[0], (Kind::Comment, "/* head */"));
        assert_eq!(found[1], (Kind::Include("TYPES.H".into()), "#include \"TYPES.H\""));
        assert_eq!(found[2], (Kind::Directive, "#define MASK(x) \\\n  ((x) & 1)"));
        assert_eq!(found[3], (Kind::Declaration, "struct A { u8 a; }; /* note */"));
        assert_eq!(found[4], (Kind::Declaration, "s32 f(void);"));
        assert_eq!(found[5], (Kind::Conditional, "#if defined(X)\nu8 gX;\n#endif"));
        assert_eq!(found[6].1, "void g(void)\n{\n#if Y\n    f();\n#endif\n}");
        assert_eq!(found[6].0, Kind::Function { body: 13 });
        assert_eq!(found.len(), 7);
    }

    fn named(source: &str) -> Vec<(Space, String, bool)> {
        items(source).iter().flat_map(names).map(|n| (n.space, n.name, n.defines)).collect()
    }

    #[test]
    fn names_say_what_is_declared_and_what_is_defined() {
        use Space::*;
        assert_eq!(
            named("struct P;\nstruct Q { struct R { u8 a; } r; };\ntypedef unsigned short u16;\nextern u8 *gW, gV[];\nconst struct Q *Get(s32 a);\nstatic u8 gT[2] = { 1, 2 };\nvoid (*gHook)(void);\nenum { A, B = 2 };\n#define M 1\ns32 Run(void) { return 0; }\n"),
            vec![
                (Tag, "P".into(), false),
                (Tag, "Q".into(), true),
                (Tag, "R".into(), true),
                (Ordinary, "u16".into(), true),
                (Ordinary, "gW".into(), false),
                (Ordinary, "gV".into(), false),
                (Ordinary, "Get".into(), false),
                (Ordinary, "gT".into(), true),
                (Ordinary, "gHook".into(), true),
                (Ordinary, "A".into(), true),
                (Ordinary, "B".into(), true),
                (Macro, "M".into(), true),
                (Ordinary, "Run".into(), true),
            ]
        );
    }

    #[test]
    fn signatures_leave_out_extern_and_parameter_names() {
        let signature = |source: &str| names(&items(source)[0]).remove(0).signature;
        let declared = signature("extern void f(u32 a, void (*done)(void));");
        assert_eq!(declared, "void f ( u32 , void ( * ) ( void ) )");
        assert_eq!(signature("void f(u32, void (*)(void));"), declared);
        assert_eq!(signature("void f(u32 value, void (*cb)(void))\n{\n}"), declared);
        assert_ne!(signature("void f(s32, void (*)(void));"), declared);
        assert_eq!(signature("struct Q;"), "struct Q");
    }

    #[test]
    fn normalizes_spacing_and_comments_and_reads_line_markers() {
        let a = items("struct A {\n    u8 a; /* x */\n};");
        let b = items("struct A { u8 a; };");
        assert_eq!(normalized(&a[0]), normalized(&b[0]));
        let m = items("#define  X  1 /* c */");
        assert_eq!(normalized(&m[0]), "#define X 1");
        let marker = items("# 12 \"games/COMMON/INCLUDE/GAME/A.H\" 2\n");
        assert_eq!(line_marker(&marker[0]).as_deref(), Some("games/COMMON/INCLUDE/GAME/A.H"));
        assert!(blank_line_before("a;\n\n  b;", 6));
        assert!(!blank_line_before("a;\n  b;", 5));
    }

    #[test]
    fn aggregates_are_tags_and_untagged_typedefs() {
        let found: Vec<String> = items("struct A { struct B { u8 b; } b; };\ntypedef struct { u8 c; } C, *PC;\nunion U { u8 u; };\nstruct A *p;\nvoid f(void) { struct L { u8 l; } l; }\n")
            .iter()
            .flat_map(aggregate_types)
            .collect();
        assert_eq!(found, ["struct A", "struct B", "C", "union U"]);
    }
}
