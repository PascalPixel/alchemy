//! One translation unit from several of today's C files, in ROM order.
//!
//! Each file keeps its own region, in order, so every function still sees
//! what it saw alone: includes are gathered once at the top, an item another
//! file already wrote identically appears once, and the rest is reconciled
//! name by name:
//! - an external declaration two files (or a file and the merged headers or
//!   definitions) give different shapes stays at block scope inside the
//!   functions of the file that wrote it, exactly as that file compiled it;
//! - a private macro a later file uses or redefines is `#undef`ined at the
//!   start of that file's region;
//! - a static definition, record tag or typedef two files define differently
//!   keeps its first spelling; the later file's copy takes a numbered name
//!   in that file's region (`Name_2`), which says nothing new about it;
//! - a `#define OWN_x` before the includes (a file declaring a header's
//!   object its own way) is dropped unless every file has it; that file's
//!   own declaration then stays at block scope by the first rule.
//!
//! Anything else that cannot be reconciled is reported, never guessed.

use crate::adopt_into::Known;
use crate::c_items::{blank_line_before, items, names, normalized, Item, Kind, Space};
use crate::compiler::c_syntax::{lex, Tok};
use std::collections::{BTreeMap, BTreeSet};

pub struct Source {
    /// Repository-relative path, for messages.
    pub path: String,
    pub text: String,
    /// What the file's own headers declare, when known: calls to names
    /// they leave undeclared were implicit `int f()` in the file alone.
    pub own: Option<Known>,
}

#[derive(Debug)]
pub struct Merged {
    pub text: String,
    pub notes: Vec<String>,
}

/// The parts of one file.
struct Parsed<'a> {
    path: &'a str,
    text: &'a str,
    lead: Vec<&'a Item<'a>>,
    pre_defines: Vec<&'a Item<'a>>,
    includes: Vec<&'a Item<'a>>,
    /// Body items with the comments written above them.
    body: Vec<(Vec<&'a Item<'a>>, &'a Item<'a>)>,
    trailing: Vec<&'a Item<'a>>,
}

fn parse<'a>(source: &'a Source, list: &'a [Item<'a>]) -> Parsed<'a> {
    let mut parsed = Parsed { path: &source.path, text: &source.text, lead: Vec::new(), pre_defines: Vec::new(), includes: Vec::new(), body: Vec::new(), trailing: Vec::new() };
    let first_code = list.iter().position(|i| matches!(i.kind, Kind::Declaration | Kind::Function { .. } | Kind::Conditional)).unwrap_or(list.len());
    let last_top_include = list[..first_code].iter().rposition(|i| matches!(i.kind, Kind::Include(_)));
    let first_other = list.iter().position(|i| i.kind != Kind::Comment).unwrap_or(list.len());
    let mut pending: Vec<&Item> = Vec::new();
    for (n, item) in list.iter().enumerate() {
        match &item.kind {
            Kind::Include(_) => parsed.includes.push(item),
            Kind::Comment if n < first_other && first_other < list.len() && !matches!(list[first_other].kind, Kind::Function { .. } | Kind::Declaration) => parsed.lead.push(item),
            Kind::Directive if is_define(item) && last_top_include.is_some_and(|l| n < l) => parsed.pre_defines.push(item),
            Kind::Comment => pending.push(item),
            _ => parsed.body.push((std::mem::take(&mut pending), item)),
        }
    }
    parsed.trailing = pending;
    parsed
}

fn is_define(item: &Item) -> bool {
    item.text.trim_start().trim_start_matches('#').trim_start().starts_with("define")
}

fn macro_name(item: &Item) -> Option<String> {
    names(item).into_iter().find(|n| n.space == Space::Macro).map(|n| n.name)
}

fn identifiers(text: &str) -> BTreeSet<String> {
    let mut out = BTreeSet::new();
    let bytes = text.as_bytes();
    let mut i = 0;
    while i < bytes.len() {
        if bytes[i].is_ascii_alphabetic() || bytes[i] == b'_' {
            let start = i;
            while i < bytes.len() && (bytes[i].is_ascii_alphanumeric() || bytes[i] == b'_') {
                i += 1;
            }
            out.insert(text[start..i].to_string());
        } else {
            i += 1;
        }
    }
    out
}

fn uses(item: &Item, name: &str) -> bool {
    match lex(item.text) {
        Ok(tokens) => tokens.iter().any(|t| matches!(&t.tok, Tok::Ident(s) if s == name)),
        Err(_) => identifiers(item.text).contains(name),
    }
}

/// `word` replaced by `with` wherever it stands as a whole identifier
/// outside comments and literals; `after_tag` limits it to `struct word`,
/// `union word` and `enum word`.
pub fn rename(text: &str, word: &str, with: &str, after_tag: bool) -> String {
    let bytes = text.as_bytes();
    let mut out = String::with_capacity(text.len());
    let mut i = 0;
    let mut last_word = String::new();
    while i < bytes.len() {
        let c = bytes[i];
        if c == b'/' && bytes.get(i + 1) == Some(&b'*') {
            let end = text[i + 2..].find("*/").map_or(text.len(), |e| i + 2 + e + 2);
            out.push_str(&text[i..end]);
            i = end;
        } else if c == b'/' && bytes.get(i + 1) == Some(&b'/') {
            let end = text[i..].find('\n').map_or(text.len(), |e| i + e);
            out.push_str(&text[i..end]);
            i = end;
        } else if c == b'"' || c == b'\'' {
            let mut j = i + 1;
            while j < bytes.len() && bytes[j] != c && bytes[j] != b'\n' {
                j += if bytes[j] == b'\\' { 2 } else { 1 };
            }
            let end = (j + 1).min(bytes.len());
            out.push_str(&text[i..end]);
            i = end;
        } else if c.is_ascii_alphabetic() || c == b'_' {
            let start = i;
            while i < bytes.len() && (bytes[i].is_ascii_alphanumeric() || bytes[i] == b'_') {
                i += 1;
            }
            let found = &text[start..i];
            if found == word && (!after_tag || matches!(last_word.as_str(), "struct" | "union" | "enum")) {
                out.push_str(with);
            } else {
                out.push_str(found);
            }
            last_word = found.to_string();
        } else {
            if !c.is_ascii_whitespace() {
                last_word.clear();
            }
            let next = text[i..].chars().next().map_or(1, char::len_utf8);
            out.push_str(&text[i..i + next]);
            i += next;
        }
    }
    out
}

/// What one file says about one ordinary name at file scope.
#[derive(Default)]
struct Usage {
    /// Declaring items' signatures, by file.
    declared: BTreeMap<usize, String>,
    /// The defining file and signature.
    defined: Option<(usize, String, bool)>,
}

fn is_static(item: &Item) -> bool {
    match lex(item.text) {
        Ok(tokens) => tokens.iter().filter(|t| !matches!(t.tok, Tok::Comment(_))).take_while(|t| !matches!(t.tok, Tok::Punct("(" | "=" | "{" | ";"))).any(|t| matches!(&t.tok, Tok::Ident(s) if s == "static")),
        Err(_) => false,
    }
}

/// Insert block-scope declarations at the head of a function body.
fn with_block_declarations(item: &Item, body: usize, declarations: &[String]) -> String {
    if declarations.is_empty() {
        return item.text.to_string();
    }
    let open = body + 1;
    let mut lines = String::new();
    for d in declarations {
        lines.push_str("\n\t");
        lines.push_str(d);
    }
    let rest = &item.text[open..];
    // A body that opens with other declarations keeps them after ours.
    format!("{}{lines}{}", &item.text[..open], rest)
}

fn one_line(item: &Item) -> String {
    let code = normalized(item);
    // normalized() spaces every token; keep the original spelling when it is one line.
    let text = item.text.trim();
    if !text.contains('\n') && !text.contains("/*") && !text.contains("//") {
        text.to_string()
    } else {
        code
    }
}

/// Enumerators an earlier file already defined with the same explicit value
/// leave a later file's enum: one constant, one definition. A clash of
/// values, or an implicit value that would move, is left for the merge to
/// report.
fn shared_enumerators(sources: &[Source], notes: &mut Vec<String>) -> Vec<Source> {
    let mut seen: BTreeMap<String, String> = BTreeMap::new();
    let mut out = Vec::new();
    for source in sources {
        let mut text = source.text.clone();
        let list = items(&source.text);
        let mut edits: Vec<(usize, usize, String)> = Vec::new();
        for item in &list {
            if item.kind != Kind::Declaration {
                continue;
            }
            let Some((open, close)) = enum_body(item.text) else {
                continue;
            };
            let body = &item.text[open + 1..close];
            let members: Vec<&str> = body.split(',').collect();
            let parsed: Vec<Option<(String, String)>> = members
                .iter()
                .map(|m| {
                    let code = strip_line_comments(m);
                    let (name, value) = code.split_once('=')?;
                    Some((name.trim().to_string(), value.split_whitespace().collect::<Vec<_>>().join(" ")))
                })
                .collect();
            let explicit = members.iter().zip(&parsed).all(|(m, p)| p.is_some() || strip_line_comments(m).trim().is_empty());
            if !explicit {
                for (name, value) in parsed.into_iter().flatten() {
                    seen.entry(name).or_insert(value);
                }
                continue;
            }
            let mut kept: Vec<&str> = Vec::new();
            let mut gone = Vec::new();
            for (m, p) in members.iter().zip(&parsed) {
                match p {
                    Some((name, value)) if seen.get(name) == Some(value) => gone.push(name.clone()),
                    Some((name, value)) => {
                        seen.insert(name.clone(), value.clone());
                        kept.push(m);
                    }
                    None => kept.push(m),
                }
            }
            if gone.is_empty() {
                continue;
            }
            let tagged = !item.text[..open].trim().trim_start_matches("extern").trim().trim_start_matches("enum").trim().is_empty();
            let left = kept.iter().any(|m| !strip_line_comments(m).trim().is_empty());
            let replacement = if left {
                let mut body = kept.join(",");
                if !body.ends_with('\n') {
                    body.push('\n');
                }
                format!("{}{{{}}}{}", &item.text[..open], body, &item.text[close + 1..])
            } else if tagged && identifiers(&source.text.replacen(item.text, "", 1)).contains(item.text[..open].trim().trim_start_matches("extern").trim().trim_start_matches("enum").trim()) {
                continue;
            } else {
                String::new()
            };
            notes.push(format!("{}: {} enumerator(s) an earlier file defines identically left its enum", source.path, gone.len()));
            edits.push((item.start, item.end, replacement));
        }
        for (start, end, replacement) in edits.into_iter().rev() {
            text.replace_range(start..end, &replacement);
        }
        out.push(Source { path: source.path.clone(), text, own: source.own.clone() });
    }
    out
}

fn strip_line_comments(text: &str) -> String {
    let mut out = String::new();
    for line in text.lines() {
        let line = line.split("//").next().unwrap_or("");
        let mut rest = line;
        while let Some(at) = rest.find("/*") {
            out.push_str(&rest[..at]);
            rest = rest[at..].find("*/").map_or("", |e| &rest[at + e + 2..]);
        }
        out.push_str(rest);
        out.push('\n');
    }
    out
}

/// The braces of `enum [Tag] { ... };` with no declarator.
fn enum_body(text: &str) -> Option<(usize, usize)> {
    let code = text.trim_start();
    let code = code.strip_prefix("extern").map_or(code, str::trim_start);
    if !code.starts_with("enum") {
        return None;
    }
    let open = text.find('{')?;
    let close = text.rfind('}')?;
    if text[close + 1..].split("/*").next().unwrap_or("").trim().trim_end_matches(';').trim().is_empty() && text[open + 1..close].find('{').is_none() {
        Some((open, close))
    } else {
        None
    }
}

#[derive(Clone, Copy, Debug, Default)]
pub struct Options {
    /// Keep each differing private declaration as a cast of the name over
    /// its own file's region instead of giving way to the governing shape.
    pub views: bool,
    /// With views, also keep unprototyped `T f()` declarations as casts.
    pub unprototyped: bool,
}

/// Single-line declarations inside an `#if` block, with their names.
fn conditional_declarations(item: &Item) -> Vec<(String, Vec<crate::c_items::Name>)> {
    let mut out = Vec::new();
    for line in item.text.lines() {
        let trimmed = line.trim();
        if trimmed.starts_with('#') || !trimmed.ends_with(';') {
            continue;
        }
        let list = items(trimmed);
        if list.len() == 1 && list[0].kind == Kind::Declaration {
            let found: Vec<_> = names(&list[0]).into_iter().filter(|n| n.space == Space::Ordinary && !n.defines).collect();
            if !found.is_empty() {
                out.push((trimmed.to_string(), found));
            }
        }
    }
    out
}

/// `#define NAME (cast of NAME)`: the file's own view of a name another
/// shape governs. `extern u8 gX[4];` -> `((u8 (*))&gX)`, `s32 f(u8);` ->
/// `((s32 (*)(u8))f)`, `extern struct W gW;` -> `(*(struct W (*))&gW)`.
pub fn view(declaration: &str, name: &str) -> Option<String> {
    let mut text = declaration.trim().trim_end_matches(';').trim().to_string();
    for word in ["extern ", "static "] {
        if let Some(rest) = text.strip_prefix(word) {
            text = rest.trim().to_string();
        }
    }
    let at = text.match_indices(name).find(|(at, _)| {
        let before = text[..*at].chars().last().is_none_or(|c| !(c.is_ascii_alphanumeric() || c == '_'));
        let after = text[at + name.len()..].chars().next().is_none_or(|c| !(c.is_ascii_alphanumeric() || c == '_'));
        before && after
    })?.0;
    let rest = text[at + name.len()..].trim_start();
    let head = &text[..at];
    let (cast, function) = if rest.starts_with('[') {
        // The whole array type, so indexing stays indexing of an array.
        (format!("{head}(*){rest}"), false)
    } else if rest.starts_with('(') {
        (format!("{head}(*){rest}"), true)
    } else if rest.is_empty() {
        (format!("{head}(*)"), false)
    } else {
        return None;
    };
    let array = rest.starts_with('[');
    let cast = cast.split_whitespace().collect::<Vec<_>>().join(" ");
    Some(if function {
        format!("#define {name} (({cast}){name})")
    } else if array {
        format!("#define {name} (*({cast})&{name})")
    } else {
        format!("#define {name} (*({cast})&{name})")
    })
}

pub fn merge(sources: &[Source], headers: &Known, options: &Options) -> Result<Merged, Vec<String>> {
    let mut notes = Vec::new();
    let owned = shared_enumerators(sources, &mut notes);
    let sources = &owned[..];
    let lists: Vec<Vec<Item>> = sources.iter().map(|s| items(&s.text)).collect();
    let mut files: Vec<Parsed> = sources.iter().zip(&lists).map(|(s, l)| parse(s, l)).collect();
    let mut errors = Vec::new();

    // Defines written before the includes.
    let mut pre: Vec<String> = Vec::new();
    let mut dropped_pre: BTreeSet<String> = BTreeSet::new();
    let mut moved: Vec<(usize, usize)> = Vec::new();
    for (k, f) in files.iter().enumerate() {
        for d in &f.pre_defines {
            let text = normalized(d);
            if moved.iter().any(|m| m.0 == k && m.1 == d.start) {
                continue;
            }
            if pre.contains(&text) || dropped_pre.contains(&text) {
                continue;
            }
            let everywhere = files.iter().all(|g| g.pre_defines.iter().any(|e| normalized(e) == text));
            let name = macro_name(d).unwrap_or_default();
            if everywhere {
                pre.push(text);
            } else if name.starts_with("OWN_") {
                dropped_pre.insert(text);
                notes.push(format!("{name}: only {} defines it; its own declaration stays at block scope", f.path));
            } else if d.text.split_whitespace().count() == 2 {
                // A flag a header may test: every region sees it, as one file did.
                pre.push(text);
                notes.push(format!("{name}: only {} defines it before its includes; now the whole unit does", f.path));
            } else {
                moved.push((k, d.start));
            }
        }
    }
    // Other defines before the includes open their own file's region.
    for (k, start) in moved.into_iter().rev() {
        let list = &lists[k];
        if let Some(item) = list.iter().find(|i| i.start == start) {
            files[k].body.insert(0, (Vec::new(), item));
        }
    }
    let mut includes: Vec<String> = Vec::new();
    for f in &files {
        for i in &f.includes {
            let line = i.text.trim().to_string();
            if !includes.iter().any(|have| normalized_include(have) == normalized_include(&line)) {
                includes.push(line);
            }
        }
    }

    // Ordinary names: who declares and who defines them.
    let mut ordinary: BTreeMap<String, Usage> = BTreeMap::new();
    for (k, f) in files.iter().enumerate() {
        for (_, item) in &f.body {
            for n in names(item).into_iter().filter(|n| n.space == Space::Ordinary) {
                let usage = ordinary.entry(n.name.clone()).or_default();
                let function = matches!(item.kind, Kind::Function { .. });
                if n.defines {
                    if let Some((other, signature, _)) = &usage.defined {
                        if *other != k && *signature != n.signature {
                            // Handled below as a static or typedef clash.
                        }
                    } else {
                        usage.defined = Some((k, n.signature.clone(), function));
                    }
                } else if item.kind == Kind::Declaration {
                    usage.declared.entry(k).or_insert(n.signature.clone());
                }
            }
        }
    }

    // Renames for clashing static definitions, tags and typedefs.
    let mut renames: Vec<BTreeMap<String, (String, bool)>> = vec![BTreeMap::new(); files.len()];
    let mut taken: BTreeSet<String> = BTreeSet::new();
    for f in &files {
        for (_, item) in &f.body {
            for n in names(item) {
                taken.insert(n.name);
            }
        }
    }
    let mut first_definition: BTreeMap<(Space, String), (usize, String)> = BTreeMap::new();
    for (k, f) in files.iter().enumerate() {
        for (_, item) in &f.body {
            let text = normalized(item);
            for n in names(item).into_iter().filter(|n| n.defines && n.space != Space::Macro) {
                let key = (n.space, n.name.clone());
                let header_clash = headers.defined.contains_key(&key) && !headers.texts.contains(&text);
                match first_definition.get(&key) {
                    Some((other, earlier)) if *other != k && *earlier != text => {}
                    Some(_) => continue,
                    None if header_clash => {}
                    None => {
                        first_definition.insert(key, (k, text.clone()));
                        continue;
                    }
                }
                // A clash: renamable only for statics, tags and typedefs.
                let tag = n.space == Space::Tag;
                let local = tag || is_static(item) || n.signature.starts_with("typedef");
                if !local {
                    errors.push(format!("{}: {} is defined differently in another file or header", f.path, n.describe()));
                    continue;
                }
                if renames[k].contains_key(&n.name) {
                    continue;
                }
                let mut index = 2;
                let mut fresh = format!("{}_{index}", n.name);
                while taken.contains(&fresh) {
                    index += 1;
                    fresh = format!("{}_{index}", n.name);
                }
                taken.insert(fresh.clone());
                notes.push(format!("{}: {} -> {fresh} (another file or header defines it differently)", f.path, n.describe()));
                renames[k].insert(n.name.clone(), (fresh, tag));
            }
        }
    }

    // Every private declaration's text, by file and name, top level or
    // inside an edition's #if.
    let mut declaration_text: BTreeMap<(usize, String), (String, bool)> = BTreeMap::new();
    for (k, f) in files.iter().enumerate() {
        for (_, item) in &f.body {
            match item.kind {
                Kind::Declaration => {
                    for n in names(item).into_iter().filter(|n| n.space == Space::Ordinary && !n.defines) {
                        declaration_text.entry((k, n.name)).or_insert((one_line(item), false));
                    }
                }
                Kind::Conditional => {
                    for (line, line_names) in conditional_declarations(item) {
                        for n in line_names {
                            ordinary.entry(n.name.clone()).or_default().declared.entry(k).or_insert(n.signature.clone());
                            declaration_text.entry((k, n.name)).or_insert((line.clone(), true));
                        }
                    }
                }
                _ => {}
            }
        }
    }

    // Declarations kept at block scope: (file, name) -> declaration text.
    let mut block: BTreeMap<usize, BTreeMap<String, String>> = BTreeMap::new();
    let mut lifted: BTreeSet<(usize, String)> = BTreeSet::new();
    let mut dropped: BTreeMap<(usize, String), (String, bool)> = BTreeMap::new();
    let mut hoisted: Vec<String> = Vec::new();
    let mut hoisted_names: BTreeSet<String> = BTreeSet::new();
    for (name, usage) in &ordinary {
        if usage.declared.is_empty() {
            continue;
        }
        let header = headers.declared.get(&(Space::Ordinary, name.clone())).cloned();
        let definition = usage.defined.as_ref().filter(|(k, _, _)| !renames[*k].contains_key(name)).map(|(_, s, _)| s.clone());
        let mut shapes: BTreeSet<&String> = usage.declared.values().collect();
        shapes.extend(header.iter());
        shapes.extend(definition.iter());
        if shapes.len() <= 1 {
            continue;
        }
        // GCC 2.96 refuses a block-scope declaration that differs from a
        // file-scope one already seen. So: with neither header nor definition
        // every file keeps its own inside its functions; otherwise the
        // header's shape, or the definition's declared once after the
        // includes, governs, and a differing private shape gives way to it
        // (with `views`, it stays as a cast of the name over its own region).
        let mut governing = header.clone().or(definition.clone());
        if governing.is_none() {
            // Block scope only where it adds no copies: one function per file.
            let block_ok = usage.declared.keys().all(|&k| {
                let conditional = declaration_text.get(&(k, name.clone())).is_some_and(|(_, c)| *c);
                let users = files[k].body.iter().filter(|(_, item)| matches!(item.kind, Kind::Function { .. }) && uses(item, name)).count();
                !conditional && users <= 1
            });
            if block_ok {
                for &k in usage.declared.keys() {
                    lifted.insert((k, name.clone()));
                    notes.push(format!("{}: {name} stays at block scope ({})", files[k].path, usage.declared[&k]));
                }
                continue;
            }
            // Otherwise the first file's shape governs where it stands.
            governing = usage.declared.values().next().cloned();
        }
        if header.is_none() {
            if let Some((k, _, true)) = &usage.defined {
                // The defining file's own declaration, when it has one, is what its
                // calls before the definition saw: that one goes first.
                if let (Some(own), Some((text, false))) = (usage.declared.get(k), declaration_text.get(&(*k, name.clone()))) {
                    hoisted.push(text.clone());
                    hoisted_names.insert(name.clone());
                    governing = Some(own.clone());
                    dropped.insert((*k, name.clone()), (text.clone(), false));
                } else if let Some(head) = files[*k].body.iter().find_map(|(_, item)| match item.kind {
                    Kind::Function { body } if names(item).first().is_some_and(|n| n.name == *name) => Some(item.text[..body].split_whitespace().collect::<Vec<_>>().join(" ")),
                    _ => None,
                }) {
                    hoisted.push(format!("{head};"));
                    hoisted_names.insert(name.clone());
                }
            }
        }
        for (&k, signature) in &usage.declared {
            if renames[k].contains_key(name) || Some(signature) == governing.as_ref() {
                continue;
            }
            let text = declaration_text.get(&(k, name.clone())).map(|(t, _)| t.clone()).unwrap_or_default();
            let defining_here = usage.defined.as_ref().is_some_and(|(d, _, _)| *d == k);
            // A header's static inline wrapper is called, never cast: a cast call
            // would not inline.
            let header_static = header.as_ref().is_some_and(|h| h.split_whitespace().any(|w| w == "static"));
            let as_view = options.views && !defining_here && !header_static && (options.unprototyped || !signature.ends_with("( )"));
            if as_view {
                notes.push(format!("{}: {name} keeps its own shape as a cast ({signature})", files[k].path));
            } else if !signature.ends_with("( )") {
                notes.push(format!("{}: {name} takes the {} shape ({signature} dropped)", files[k].path, if header.is_some() { "header's" } else if definition.is_some() { "definition's" } else { "first file's" }));
            }
            dropped.insert((k, name.clone()), (text, as_view));
        }
    }
    // The declaring items to lift, and where they go.
    for (k, f) in files.iter().enumerate() {
        for (_, item) in &f.body {
            if item.kind != Kind::Declaration {
                continue;
            }
            let item_names: Vec<String> = names(item).into_iter().filter(|n| n.space == Space::Ordinary && !n.defines).map(|n| n.name).collect();
            let moving: Vec<&String> = item_names.iter().filter(|n| lifted.contains(&(k, (*n).clone()))).collect();
            let handled = item_names.iter().filter(|n| lifted.contains(&(k, (*n).clone())) || dropped.contains_key(&(k, (*n).clone()))).count();
            if handled == 0 {
                continue;
            }
            if handled != item_names.len() {
                errors.push(format!("{}: `{}` declares several names and only some must stay at block scope", f.path, one_line(item)));
                continue;
            }
            let text = one_line(item);
            for name in moving {
                block.entry(k).or_default().insert(name.clone(), text.clone());
            }
        }
    }

    // Emit.
    let mut out = String::new();
    if let Some(first) = files.first() {
        for c in &first.lead {
            out.push_str(c.text);
            out.push('\n');
        }
        if !first.lead.is_empty() {
            out.push('\n');
        }
    }
    for p in &pre {
        out.push_str(p);
        out.push('\n');
    }
    for i in &includes {
        out.push_str(i);
        out.push('\n');
    }
    if !hoisted.is_empty() {
        out.push('\n');
        for h in &hoisted {
            out.push_str(h);
            out.push('\n');
        }
    }
    let mut emitted: BTreeSet<String> = BTreeSet::new();
    // Private macros live from their region on: name -> definition.
    let mut active: BTreeMap<String, String> = BTreeMap::new();
    let mut shadows: BTreeSet<String> = BTreeSet::new();
    for (k, f) in files.iter().enumerate() {
        let rename_all = |text: &str| -> String {
            let mut t = text.to_string();
            for (from, (to, tag)) in &renames[k] {
                t = rename(&t, from, to, *tag);
            }
            t
        };
        out.push('\n');
        if k > 0 {
            for c in &f.lead {
                out.push_str(c.text);
                out.push_str("\n\n");
            }
        }
        // Macros of earlier regions this region uses or defines otherwise.
        let region_words = identifiers(f.text);
        let mut own_first: BTreeMap<String, String> = BTreeMap::new();
        for (_, item) in &f.body {
            if item.kind == Kind::Directive {
                if let Some(name) = macro_name(item) {
                    own_first.entry(name).or_insert_with(|| normalized(item));
                }
            }
        }
        let mut undefs = Vec::new();
        for (name, definition) in active.clone() {
            if !region_words.contains(&name) {
                continue;
            }
            if own_first.get(&name) == Some(&definition) {
                continue;
            }
            if shadows.contains(&name) && !own_first.contains_key(&name) {
                errors.push(format!("{}: uses the header's macro {name}, which an earlier file redefines", f.path));
            }
            undefs.push(name.clone());
            active.remove(&name);
        }
        for name in &undefs {
            out.push_str(&format!("#undef {name}\n"));
            notes.push(format!("{}: #undef {name}, an earlier file's private macro", f.path));
        }
        if !undefs.is_empty() {
            out.push('\n');
        }
        let mut views: Vec<String> = Vec::new();
        // Calls the file made with no declaration in sight were `int f()`;
        // with views they stay so where the unit now shows a prototype.
        if options.views {
            if let Some(own) = &sources[k].own {
                let mut lines = String::new();
                for name in implicit_calls(f, own) {
                    if headers.defined.contains_key(&(Space::Macro, name.clone())) {
                        continue;
                    }
                    let earlier = ordinary.get(&name).is_some_and(|u| u.declared.keys().any(|&j| j < k) || u.defined.as_ref().is_some_and(|(j, _, _)| *j < k));
                    if headers.declared.contains_key(&(Space::Ordinary, name.clone())) || earlier || hoisted_names.contains(&name) {
                        lines.push_str(&format!("#define {name} ((int (*)()){name})\n"));
                        notes.push(format!("{}: {name} keeps its implicit `int {name}()` as a cast", f.path));
                        views.push(name);
                    }
                }
                if !lines.is_empty() {
                    out.push_str(&lines);
                    out.push('\n');
                }
            }
        }
        let mut first_in_region = true;
        for (comments, item) in &f.body {
            // An item already written identically appears once.
            let key = normalized(item);
            let mut undef_before = String::new();
            let defines_static_or_decl = !matches!(item.kind, Kind::Function { .. }) || is_static(item);
            if item.kind == Kind::Directive {
                if let Some(name) = macro_name(item) {
                    let header = headers.defined.contains_key(&(Space::Macro, name.clone())) && !headers.texts.contains(&key);
                    if active.get(&name) == Some(&key) {
                        continue;
                    }
                    if header {
                        shadows.insert(name.clone());
                        notes.push(format!("{}: #undef {name} before its own definition, which differs from a header's", f.path));
                    }
                    if active.contains_key(&name) || header {
                        undef_before = format!("#undef {name}\n");
                    }
                    active.insert(name, key.clone());
                }
            } else if defines_static_or_decl && rename_all(item.text) == item.text && emitted.contains(&key) && item.kind != Kind::Conditional {
                continue;
            } else if item.kind == Kind::Declaration {
                let declared: Vec<String> = names(item).into_iter().filter(|n| n.space == Space::Ordinary && !n.defines).map(|n| n.name).collect();
                if !declared.is_empty() && declared.iter().all(|n| lifted.contains(&(k, n.clone())) || dropped.contains_key(&(k, n.clone()))) {
                    let defines = view_lines(&declared, k, &dropped, options, &mut views, &mut errors, f.path);
                    if defines.is_empty() {
                        continue;
                    }
                    if !first_in_region {
                        out.push('\n');
                    }
                    first_in_region = false;
                    out.push_str(&defines);
                    continue;
                }
            }
            if !first_in_region {
                out.push_str(if blank_line_before(f.text, comments.first().map_or(item.start, |c| c.start)) { "\n\n" } else { "\n" });
            }
            first_in_region = false;
            for c in comments {
                out.push_str(&rename_all(c.text));
                out.push_str(if blank_line_before(f.text, item.start) && std::ptr::eq(*c, *comments.last().unwrap()) { "\n\n" } else { "\n" });
            }
            out.push_str(&undef_before);
            let text = match item.kind {
                Kind::Function { body } => {
                    let wanted: Vec<String> = block.get(&k).map(|m| m.iter().filter(|(n, _)| uses(item, n)).map(|(_, d)| d.clone()).collect()).unwrap_or_default();
                    with_block_declarations(item, body, &wanted)
                }
                Kind::Conditional => conditional_text(item, k, &dropped, options, &mut views, &mut errors, f.path),
                _ => item.text.to_string(),
            };
            out.push_str(&rename_all(&text));
            emitted.insert(key);
        }
        for c in &f.trailing {
            out.push_str("\n\n");
            out.push_str(c.text);
        }
        if !views.is_empty() {
            out.push_str("\n\n");
            for name in &views {
                out.push_str(&format!("#undef {name}\n"));
            }
            out.pop();
        }
        out.push('\n');
    }
    if !errors.is_empty() {
        errors.sort();
        errors.dedup();
        return Err(errors);
    }
    Ok(Merged { text: out, notes })
}

/// The `#define` lines that stand for a dropped declaration in views mode.
fn view_lines(declared: &[String], k: usize, dropped: &BTreeMap<(usize, String), (String, bool)>, options: &Options, views: &mut Vec<String>, errors: &mut Vec<String>, path: &str) -> String {
    if !options.views {
        return String::new();
    }
    let mut out = String::new();
    for name in declared {
        let Some((text, true)) = dropped.get(&(k, name.clone())) else {
            continue;
        };
        if views.contains(name) {
            continue;
        }
        match view(text, name) {
            Some(line) => {
                out.push_str(&line);
                out.push('\n');
                views.push(name.clone());
            }
            None => errors.push(format!("{path}: no cast spells `{text}`")),
        }
    }
    out.trim_end_matches('\n').to_string()
}

/// An `#if` block with its dropped declarations removed, or turned into views.
fn conditional_text(item: &Item, k: usize, dropped: &BTreeMap<(usize, String), (String, bool)>, options: &Options, views: &mut Vec<String>, errors: &mut Vec<String>, path: &str) -> String {
    let mut out = String::new();
    for line in item.text.split_inclusive('\n') {
        let trimmed = line.trim();
        let list = if !trimmed.starts_with('#') && trimmed.ends_with(';') { items(trimmed) } else { Vec::new() };
        let declared: Vec<String> = if list.len() == 1 && list[0].kind == Kind::Declaration {
            names(&list[0]).into_iter().filter(|n| n.space == Space::Ordinary && !n.defines).map(|n| n.name).collect()
        } else {
            Vec::new()
        };
        if !declared.is_empty() && declared.iter().all(|n| dropped.contains_key(&(k, n.clone()))) {
            let defines = view_lines(&declared, k, dropped, options, views, errors, path);
            if !defines.is_empty() {
                out.push_str(&defines);
                out.push('\n');
            }
            continue;
        }
        out.push_str(line);
    }
    out
}

/// Names a file calls without declaring them itself or in its own headers.
fn implicit_calls(f: &Parsed, own: &Known) -> BTreeSet<String> {
    const KEYWORDS: &[&str] = &["if", "while", "for", "switch", "return", "sizeof", "do", "else", "case", "__asm__", "asm", "__attribute__", "__builtin_va_arg"];
    let mut known: BTreeSet<String> = BTreeSet::new();
    for (_, item) in &f.body {
        for n in names(item) {
            known.insert(n.name);
        }
    }
    let mut out = BTreeSet::new();
    for (_, item) in &f.body {
        if !matches!(item.kind, Kind::Function { .. }) {
            continue;
        }
        let Ok(tokens) = lex(item.text) else {
            continue;
        };
        let code: Vec<&Tok> = tokens.iter().map(|t| &t.tok).filter(|t| !matches!(t, Tok::Comment(_))).collect();
        for w in code.windows(3) {
            let (Tok::Ident(name), Tok::Punct("(")) = (w[1], w[2]) else {
                continue;
            };
            // A declaration or cast, `T (*name)(` or `(T)name(`, is not a call site we judge.
            if matches!(w[0], Tok::Punct("." | "->")) || KEYWORDS.contains(&name.as_str()) || known.contains(name) {
                continue;
            }
            if own.declared.contains_key(&(Space::Ordinary, name.clone())) || own.defined.contains_key(&(Space::Macro, name.clone())) || own.defined.contains_key(&(Space::Ordinary, name.clone())) {
                continue;
            }
            out.insert(name.clone());
        }
    }
    // Locals and parameters are identifiers too; only file-scope names matter,
    // and those the caller checks against what the unit declares.
    out
}

fn normalized_include(line: &str) -> String {
    line.split_whitespace().collect::<Vec<_>>().join(" ")
}

#[cfg(test)]
mod tests {
    use super::*;

    fn source(path: &str, text: &str) -> Source {
        Source { path: path.into(), text: text.into(), own: None }
    }

    #[test]
    fn identical_items_once_and_regions_in_order() {
        let a = source("A.C", "/* A. */\n#include \"TYPES.H\"\n\nstruct S { u8 a; };\nvoid g(void);\n\n// 最初\nvoid f(void)\n{\n\tg();\n}\n");
        let b = source("B.C", "#include \"TYPES.H\"\n#include \"B.H\"\n\nstruct S { u8 a; };\nvoid g(void);\n\nvoid g(void)\n{\n}\n");
        let merged = merge(&[a, b], &Known::default(), &Options::default()).unwrap();
        assert_eq!(merged.text.matches("#include \"TYPES.H\"").count(), 1);
        assert!(merged.text.starts_with("/* A. */\n\n#include \"TYPES.H\"\n#include \"B.H\"\n"));
        assert_eq!(merged.text.matches("struct S").count(), 1);
        assert_eq!(merged.text.matches("void g(void);").count(), 1);
        assert!(merged.text.contains("// 最初\nvoid f(void)"));
        assert!(merged.text.find("void f(void)").unwrap() < merged.text.find("void g(void)\n{").unwrap());
    }

    #[test]
    fn conflicting_declarations_stay_in_their_own_functions() {
        let a = source("A.C", "#include \"TYPES.H\"\nextern u8 gX[];\nvoid h(u32 a);\nvoid f(void)\n{\n\th(gX[0]);\n}\n");
        let b = source("B.C", "#include \"TYPES.H\"\nextern u16 gX[];\nvoid h(u32 a);\nvoid k(void)\n{\n\th(gX[1]);\n}\n");
        let merged = merge(&[a, b], &Known::default(), &Options::default()).unwrap();
        // No header or definition: each file's shape goes inside its own functions.
        assert!(merged.text.contains("void f(void)\n{\n\textern u8 gX[];\n\th(gX[0]);"), "{}", merged.text);
        assert!(merged.text.contains("void k(void)\n{\n\textern u16 gX[];\n\th(gX[1]);"));
        // A definition governs: its prototype comes first, and a differing
        // shape gives way to it, or stays as a cast with views.
        let e = || source("E.C", "void p(u8 a);\nvoid q(void)\n{\n\tp(1);\n}\n");
        let d = || source("D.C", "void p(s32 a)\n{\n}\n");
        let plain = merge(&[e(), d()], &Known::default(), &Options::default()).unwrap();
        assert!(plain.text.contains("void p(s32 a);\n") && !plain.text.contains("u8 a"), "{}", plain.text);
        let cast = merge(&[e(), d()], &Known::default(), &Options { views: true, unprototyped: false }).unwrap();
        assert!(cast.text.contains("#define p ((void (*)(u8 a))p)\nvoid q(void)"), "{}", cast.text);
        assert!(cast.text.contains("#undef p\n"), "{}", cast.text);
        assert_eq!(merged.text.matches("void h(u32 a);").count(), 1);
    }

    #[test]
    fn a_header_shape_wins_and_private_ones_go_inside() {
        let mut headers = Known::default();
        headers.declared.insert((Space::Ordinary, "h".into()), "void h ( u32 )".into());
        let a = source("A.C", "#include \"H.H\"\nvoid f(void)\n{\n\th(1);\n}\n");
        let b = source("B.C", "s32 h(void);\nvoid k(void)\n{\n\th();\n}\n");
        let merged = merge(&[a, b], &headers, &Options::default()).unwrap();
        assert!(!merged.text.contains("s32 h(void);"), "{}", merged.text);
        assert!(merged.notes.iter().any(|n| n.contains("takes the header's shape")));
    }

    #[test]
    fn private_macros_end_where_a_later_file_needs_the_name() {
        let a = source("A.C", "#define Go ((void (*)(int))Go)\nvoid Go(void);\nvoid f(void)\n{\n\tGo(1);\n}\n");
        let b = source("B.C", "void Go(void);\nvoid k(void)\n{\n\tGo();\n}\n");
        let c = source("C.C", "#define N 4\nvoid m(void)\n{\n}\n");
        let d = source("D.C", "#define N 4\n#define M 5\nvoid n(void)\n{\n}\n");
        let merged = merge(&[a, b, c, d], &Known::default(), &Options::default()).unwrap();
        assert!(merged.text.contains("#undef Go\n\nvoid k(void)"), "{}", merged.text);
        assert_eq!(merged.text.matches("#define N 4").count(), 1);
        let redefined = merge(&[source("A.C", "#define N 4\nint a = N;\n"), source("B.C", "#define N 5\nint b = N;\n")], &Known::default(), &Options::default()).unwrap();
        assert!(redefined.text.contains("#undef N\n\n#define N 5"), "{}", redefined.text);
    }

    #[test]
    fn clashing_statics_and_tags_take_numbered_names() {
        let a = source("A.C", "struct T { u8 a; };\nstatic u8 sCount;\nvoid f(struct T *t)\n{\n\tsCount = t->a;\n}\n");
        let b = source("B.C", "struct T { u16 b; };\nstatic u16 sCount;\nvoid k(struct T *t)\n{\n\tsCount = t->b;\n}\n");
        let merged = merge(&[a, b], &Known::default(), &Options::default()).unwrap();
        assert!(merged.text.contains("struct T_2 { u16 b; };\nstatic u16 sCount_2;\nvoid k(struct T_2 *t)\n{\n\tsCount_2 = t->b;"), "{}", merged.text);
        let error = merge(&[source("A.C", "u8 gA = 1;\n"), source("B.C", "u8 gA = 2;\n")], &Known::default(), &Options::default()).unwrap_err();
        assert!(error[0].contains("gA is defined differently"), "{error:?}");
    }

    #[test]
    fn own_defines_drop_unless_every_file_has_them() {
        let a = source("A.C", "#define OWN_gState\n#include \"STATE.H\"\nextern u8 gState[];\nvoid f(void)\n{\n\tgState[0] = 1;\n}\n");
        let b = source("B.C", "#include \"STATE.H\"\nvoid k(void)\n{\n\tgState.a = 1;\n}\n");
        let mut headers = Known::default();
        headers.declared.insert((Space::Ordinary, "gState".into()), "struct State gState".into());
        let merged = merge(&[a, b], &headers, &Options::default()).unwrap();
        assert!(!merged.text.contains("OWN_gState"));
        // Under the header the file's own shape gives way; the bytes judge whether it may.
        assert!(!merged.text.contains("extern u8 gState[];"), "{}", merged.text);
    }

    #[test]
    fn enumerators_defined_identically_appear_once() {
        let a = source("A.C", "enum A {\n    MSG_X = 0x10,\n    MSG_Y = 0x11\n};\n");
        let b = source("B.C", "enum {\n    MSG_Y = 0x11,\n    MSG_Z = 0x12\n};\n");
        let c = source("C.C", "enum {\n    MSG_X = 0x10\n};\n");
        let merged = merge(&[a, b, c], &Known::default(), &Options::default()).unwrap();
        assert_eq!(merged.text.matches("MSG_Y").count(), 1, "{}", merged.text);
        assert!(merged.text.contains("enum {\n    MSG_Z = 0x12\n};"), "{}", merged.text);
        assert_eq!(merged.text.matches("MSG_X").count(), 1);
        let clash = merge(&[source("A.C", "enum { M = 1 };\n"), source("B.C", "enum { M = 2 };\n")], &Known::default(), &Options::default());
        assert!(clash.is_err());
    }

    #[test]
    fn views_cast_the_name_to_the_files_own_shape() {
        assert_eq!(view("extern u8 gX[4];", "gX").unwrap(), "#define gX (*(u8 (*)[4])&gX)");
        assert_eq!(view("extern u8 gY[][8];", "gY").unwrap(), "#define gY (*(u8 (*)[][8])&gY)");
        assert_eq!(view("struct A *Get(s32 id);", "Get").unwrap(), "#define Get ((struct A *(*)(s32 id))Get)");
        assert_eq!(view("extern struct W gW;", "gW").unwrap(), "#define gW (*(struct W (*))&gW)");
        assert_eq!(view("void Run();", "Run").unwrap(), "#define Run ((void (*)())Run)");
    }

    #[test]
    fn implicit_calls_keep_their_unprototyped_shape_under_views() {
        let mut headers = Known::default();
        headers.declared.insert((Space::Ordinary, "Run".into()), "void Run ( s32 )".into());
        let a = Source { path: "A.C".into(), text: "void f(void)\n{\n\tRun(1);\n}\n".into(), own: Some(Known::default()) };
        let views = merge(&[a], &headers, &Options { views: true, unprototyped: false }).unwrap();
        assert!(views.text.contains("#define Run ((int (*)())Run)\n"), "{}", views.text);
        let a = Source { path: "A.C".into(), text: "void f(void)\n{\n\tRun(1);\n}\n".into(), own: Some(Known::default()) };
        assert!(!merge(&[a], &headers, &Options::default()).unwrap().text.contains("#define"));
    }

    #[test]
    fn block_scope_only_where_it_adds_no_copies() {
        let a = source("A.C", "extern u16 gS;\nvoid f(void)\n{\n\tgS = 1;\n}\nvoid g(void)\n{\n\tgS = 2;\n}\n");
        let b = source("B.C", "extern volatile u16 gS;\nvoid k(void)\n{\n\tgS = 3;\n}\n");
        let plain = merge(&[a, b], &Known::default(), &Options::default()).unwrap();
        assert_eq!(plain.text.matches("u16 gS;").count(), 1, "{}", plain.text);
        assert!(plain.notes.iter().any(|n| n.contains("first file's shape")));
    }

    #[test]
    fn rename_touches_identifiers_only() {
        assert_eq!(rename("struct T t; T x; /* T */ \"T\" Tx", "T", "T_2", false), "struct T_2 t; T_2 x; /* T */ \"T\" Tx");
        assert_eq!(rename("struct T t; T x;", "T", "T_2", true), "struct T_2 t; T x;");
    }
}
