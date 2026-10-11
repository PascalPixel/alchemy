//! `alchemy units evidence`: what the merged source and its own compile
//! show at every gap between two adjacent functions of one C file.
//!
//! The image analysis judges a gap from the linked bytes. This table adds
//! what only the source of a merged file shows while it is merged: which
//! file-scope names tie the two sides of the gap, what sits in the source
//! there, and whether the functions after the gap compile the same when the
//! file starts there. Every row is evidence for a later cut; none names a
//! file or recovers a boundary. Nothing here reads a ROM or a listing: the
//! inputs are the sources under games/, the edition's built images and the
//! compiler.

use super::model::Lang;
use super::Judged;
use crate::c_items::{self, Item, Kind, Space};
use crate::compiler::c_syntax::{lex, Tok};
use crate::compiler::routing::CompilerFamily;
use crate::targets::DecompTarget;
use std::collections::{BTreeMap, BTreeSet, HashMap};
use std::fmt::Write as _;
use std::fs;
use std::path::{Path, PathBuf};
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::Mutex;

pub const USAGE: &str = "usage: alchemy units evidence [--build EDITION] [--out FILE] [--devices FILE]
Writes one row for every gap between two adjacent functions of each C file
of one built edition (default tbs-en) to FILE (default
recon/<game>/floor_evidence.tsv). Build the edition first. Columns:
  file, image, gap, left, right   the file, and the gap as `alchemy units` numbers it
  left_at, right_at               where each function is defined: line, or fragment:line
  verdict, p                      the image analysis's verdict and boundary probability
  at_gap                          what the source holds between the two definitions:
                                  a fragment's edge, includes, counted declarations and
                                  definitions, comment lines with the first that says something
  between                         names defined there that the file uses, each with the
                                  side that uses it
  shared, shared_kinds,           names defined in the file that functions on both sides
  shared_names                    use: how many, of which kinds, and the names with the
                                  nearest pair of users first: name(kind,functions apart)
  left_only, right_only           how many such names only one side uses
  left_last, right_first          names whose last user is the left function, whose
                                  first user is the right one
  pad                             bytes the object leaves between the left function's
                                  end and the right one's start: 0, or length:hex
  label                           the right function's first .L number as the file
                                  compiles / when the file starts at this gap
  fresh                           the file compiled with every function body before the
                                  gap replaced by its prototype (inline ones stay): same N
                                  when the N functions after it emit the same assembly
                                  (labels and numbered local names renumbered), else how
                                  many change and the nearest one
  fakematch_left, fakematch_right FAKEMATCH tags of each function, or of an inline
                                  helper it uses, whose reason names file context (a
                                  label count, inlining, declarations, a pool, a file)
  devices                         with --devices FILE, when it exists: its rows that hold
                                  either function's name as a field
A name is a macro (define; NAME#2 is its second definition), type,
enumerator (enum), datum (static or data), inline helper (inline), function
the object does not hold (function) or static function (fn) that the file,
or a fragment or local header it includes (anything under a src folder),
defines. A use counts through the macros, helpers and data that mention the
name, a macro only between its #define and its #undef, and a name's mention
of a static function counts as that function's use. A fragment's functions
stand where it is included.";

/// Names a cell lists before it counts the rest.
const LISTED: usize = 6;

struct Options {
    build: String,
    out: Option<PathBuf>,
    devices: Option<PathBuf>,
}

fn parse(arguments: &[String]) -> Result<Options, String> {
    let mut options = Options { build: "tbs-en".into(), out: None, devices: None };
    let mut i = 0;
    while i < arguments.len() {
        let flag = arguments[i].as_str();
        let value = arguments.get(i + 1).ok_or_else(|| format!("{flag} needs a value\n{USAGE}"))?.clone();
        match flag {
            "--build" => options.build = value,
            "--out" => options.out = Some(PathBuf::from(value)),
            "--devices" => options.devices = Some(PathBuf::from(value)),
            _ => return Err(format!("unknown option {flag}\n{USAGE}")),
        }
        i += 2;
    }
    Ok(options)
}

fn word_byte(byte: u8) -> bool {
    byte.is_ascii_alphanumeric() || byte == b'_'
}

/// Identifiers of text the lexer cannot read, or of a directive line.
fn words(text: &str) -> BTreeSet<String> {
    let bytes = text.as_bytes();
    let mut out = BTreeSet::new();
    let mut i = 0;
    while i < bytes.len() {
        if !word_byte(bytes[i]) {
            i += 1;
            continue;
        }
        let start = i;
        while i < bytes.len() && word_byte(bytes[i]) {
            i += 1;
        }
        if !bytes[start].is_ascii_digit() {
            out.insert(text[start..i].to_string());
        }
    }
    out
}

/// Identifiers and comments of C text, literals and directive lines aside.
fn read(text: &str) -> (BTreeSet<String>, Vec<String>) {
    let Ok(tokens) = lex(text) else {
        return (words(text), Vec::new());
    };
    let mut identifiers = BTreeSet::new();
    let mut comments = Vec::new();
    for token in tokens {
        match token.tok {
            Tok::Ident(name) => {
                identifiers.insert(name);
            }
            Tok::Comment(text) => comments.push(text),
            _ => {}
        }
    }
    (identifiers, comments)
}

/// `# 12 "file" 1`, the preprocessor's note of where the next line is from.
fn marker(line: &str) -> Option<(usize, &str, &str)> {
    let rest = line.strip_prefix('#')?.trim_start();
    let rest = rest.strip_prefix("line").unwrap_or(rest).trim_start();
    let digits = rest.find(|c: char| !c.is_ascii_digit()).unwrap_or(rest.len());
    let number = rest[..digits].parse().ok()?;
    let quoted = rest[digits..].trim_start().strip_prefix('"')?;
    let end = quoted.find('"')?;
    Some((number, &quoted[..end], quoted[end + 1..].trim()))
}

/// Where each line of a preprocessed text came from.
struct Origins {
    files: Vec<String>,
    /// Byte offset of each output line.
    starts: Vec<usize>,
    /// Source file and line of each output line; none for a marker.
    lines: Vec<Option<(usize, usize)>>,
    /// Includes: the output line, the file entered and the file including it.
    entered: Vec<(usize, usize, usize)>,
}

impl Origins {
    fn new(text: &str, root: &str) -> Origins {
        let mut origins = Origins { files: vec![String::new()], starts: Vec::new(), lines: Vec::new(), entered: Vec::new() };
        let (mut file, mut line, mut offset) = (0, 1, 0);
        for (index, text_line) in text.split('\n').enumerate() {
            origins.starts.push(offset);
            offset += text_line.len() + 1;
            let Some((number, name, flags)) = marker(text_line) else {
                origins.lines.push(Some((file, line)));
                line += 1;
                continue;
            };
            let name = name.strip_prefix(root).map_or(name, |tail| tail.trim_start_matches('/'));
            let from = file;
            file = origins.files.iter().position(|f| f == name).unwrap_or_else(|| {
                origins.files.push(name.to_string());
                origins.files.len() - 1
            });
            if flags.split_whitespace().any(|flag| flag == "1") {
                origins.entered.push((index, file, from));
            }
            line = number;
            origins.lines.push(None);
        }
        origins
    }

    fn line_of(&self, offset: usize) -> usize {
        self.starts.partition_point(|start| *start <= offset).saturating_sub(1)
    }

    fn at(&self, offset: usize) -> (usize, usize) {
        self.lines[self.line_of(offset)].unwrap_or((0, 0))
    }
}

/// A file of the unit itself: its source, or a fragment or local header it
/// includes. Shared headers live under an include folder.
fn own(path: &str) -> bool {
    path.starts_with("games/") && path.contains("/src/")
}

fn short(path: &str) -> &str {
    path.split_once("/src/").map_or(path, |(_, tail)| tail)
}

fn inline_head(head: &str) -> bool {
    words(head).iter().any(|w| matches!(w.as_str(), "inline" | "__inline" | "__inline__"))
}

/// The classes of file context a FAKEMATCH reason can name.
const CONTEXT: &[(&str, &[&str])] = &[
    ("label", &["label count", "label number", "constant label", ".lc"]),
    ("inline", &["inlin"]),
    ("declaration", &["declaration order", "declared", "prototype", "definition order", "defined before", "defined after", "defined earlier", "defined later"]),
    ("pool", &["pool"]),
    ("file", &[" file", "translation unit"]),
];

/// The FAKEMATCH tags among `comments` whose reason names file context.
fn context_tags(comments: &[String], via: Option<&str>) -> Vec<String> {
    let mut found = Vec::new();
    for comment in comments {
        let Some((_, reason)) = comment.split_once("FAKEMATCH:") else {
            continue;
        };
        let reason = reason.trim_end_matches("*/").split_whitespace().collect::<Vec<_>>().join(" ");
        let lower = reason.to_lowercase();
        let classes: Vec<&str> = CONTEXT.iter().filter(|(_, keys)| keys.iter().any(|key| lower.contains(key))).map(|(class, _)| *class).collect();
        if classes.is_empty() {
            continue;
        }
        let text: String = reason.chars().take(160).collect();
        let via = via.map_or_else(String::new, |helper| format!(" via {helper}"));
        found.push(format!("[{}{via}] {text}{}", classes.join(","), if reason.chars().count() > 160 { "..." } else { "" }));
    }
    found
}

/// One function the object holds, in the object's order.
#[derive(Clone, Debug)]
pub struct Emitted {
    pub name: String,
    pub is_static: bool,
}

/// What the source shows at the gap after one emitted function.
#[derive(Clone, Debug, Default, PartialEq)]
pub struct SourceGap {
    pub left_at: String,
    pub right_at: String,
    pub at_gap: String,
    pub between: String,
    pub shared: usize,
    pub shared_kinds: String,
    pub shared_names: String,
    pub left_only: usize,
    pub right_only: usize,
    pub left_last: String,
    pub right_first: String,
    pub fakematch_left: String,
    pub fakematch_right: String,
}

/// The unit's source as the compiler read it.
pub struct Source {
    pub gaps: Vec<SourceGap>,
    /// The top-level definition holding each emitted function: itself, or
    /// the parent of a nested function.
    pub owner: Vec<Option<String>>,
}

/// A function definition of the unit's own files.
struct Defined {
    item: usize,
    name: String,
    inline: bool,
    mentions: BTreeSet<String>,
    comments: Vec<String>,
}

/// One definition of a name in the unit's own files.
struct Definition {
    name: String,
    kind: &'static str,
    item: usize,
    /// For a macro, the item that ends it: its `#undef` or its next definition.
    until: usize,
    /// Every identifier its defining item mentions.
    mentions: BTreeSet<String>,
    comments: Vec<String>,
}

/// What the unit's own files define under one name: each definition of a
/// macro by itself (`NAME#2` is its second), everything else of the name
/// together.
struct Entity {
    label: String,
    kind: &'static str,
    definitions: Vec<usize>,
}

fn listed(mut names: Vec<String>) -> String {
    let more = names.len().saturating_sub(LISTED);
    names.truncate(LISTED);
    if more > 0 {
        names.push(format!("+{more}"));
    }
    names.join(" ")
}

/// The files' own text by line, read once.
struct Raw<'a> {
    read: &'a dyn Fn(&str) -> Option<String>,
    lines: HashMap<String, Option<Vec<String>>>,
}

impl Raw<'_> {
    /// Lines `first..=last` of a file, numbered from 1.
    fn span(&mut self, file: &str, first: usize, last: usize) -> String {
        let read = self.read;
        let lines = self.lines.entry(file.to_string()).or_insert_with(|| read(file).map(|text| text.lines().map(str::to_string).collect()));
        match lines {
            Some(lines) if first > 0 && first <= last => lines.get(first - 1..last.min(lines.len())).unwrap_or(&[]).join("\n"),
            _ => String::new(),
        }
    }
}

/// The names of a unit's own files and who uses them.
struct Names {
    definitions: Vec<Definition>,
    entities: Vec<Entity>,
    by_name: BTreeMap<String, Vec<usize>>,
    /// The entity of each definition.
    entity_of: Vec<usize>,
}

impl Names {
    fn new(mut definitions: Vec<Definition>, undefined: &[(String, usize)]) -> Names {
        definitions.sort_by_key(|d| d.item);
        let mut by_name: BTreeMap<String, Vec<usize>> = BTreeMap::new();
        for (d, definition) in definitions.iter().enumerate() {
            by_name.entry(definition.name.clone()).or_default().push(d);
        }
        let mut entities: Vec<Entity> = Vec::new();
        let mut entity_of = vec![0; definitions.len()];
        for (name, indices) in &by_name {
            let macros: Vec<usize> = indices.iter().copied().filter(|&d| definitions[d].kind == "define").collect();
            for (n, &d) in macros.iter().enumerate() {
                let ends = macros.get(n + 1).map(|&next| definitions[next].item).into_iter().chain(undefined.iter().filter(|(undefined, item)| undefined == name && *item > definitions[d].item).map(|(_, item)| *item));
                definitions[d].until = ends.min().unwrap_or(usize::MAX);
                entity_of[d] = entities.len();
                entities.push(Entity { label: if n == 0 { name.clone() } else { format!("{name}#{}", n + 1) }, kind: "define", definitions: vec![d] });
            }
            let others: Vec<usize> = indices.iter().copied().filter(|&d| definitions[d].kind != "define").collect();
            if let Some(&first) = others.first() {
                for &d in &others {
                    entity_of[d] = entities.len();
                }
                entities.push(Entity { label: name.clone(), kind: definitions[first].kind, definitions: others });
            }
        }
        Names { definitions, entities, by_name, entity_of }
    }

    /// What `name` means at item `at`: a macro only between its definition
    /// and its end.
    fn resolve(&self, name: &str, at: usize) -> impl Iterator<Item = usize> + '_ {
        self.by_name.get(name).into_iter().flatten().filter(move |&&d| self.definitions[d].kind != "define" || (self.definitions[d].item < at && at < self.definitions[d].until)).map(|&d| self.entity_of[d])
    }

    /// Everything identifiers read at item `at` reach through the unit's
    /// own names. A macro's text is read where it is used, anything else
    /// where it is defined.
    fn reach(&self, mentions: &BTreeSet<String>, at: usize) -> BTreeSet<usize> {
        let mut seen = BTreeSet::new();
        let mut queue: Vec<(usize, usize)> = mentions.iter().flat_map(|name| self.resolve(name, at)).map(|entity| (entity, at)).collect();
        while let Some((entity, at)) = queue.pop() {
            if !seen.insert(entity) {
                continue;
            }
            for &d in &self.entities[entity].definitions {
                let definition = &self.definitions[d];
                let at = if definition.kind == "define" { at } else { definition.item };
                queue.extend(definition.mentions.iter().flat_map(|name| self.resolve(name, at)).map(|entity| (entity, at)));
            }
        }
        seen
    }

    fn show(&self, entity: usize, more: impl std::fmt::Display) -> String {
        format!("{}({}{more})", self.entities[entity].label, self.entities[entity].kind)
    }
}

/// Reads a unit preprocessed with its `#define` lines kept (`-dD`). `raw`
/// gives a source file's own text: the macros a function names before they
/// expand, and the comments.
pub fn source(text: &str, root: &str, emitted: &[Emitted], raw: &dyn Fn(&str) -> Option<String>) -> Source {
    let origins = Origins::new(text, root);
    let mut raw = Raw { read: raw, lines: HashMap::new() };
    let items: Vec<Item> = c_items::items(text);
    let place: Vec<(usize, usize)> = items.iter().map(|item| origins.at(item.start)).collect();
    let is_own = |index: usize| own(&origins.files[place[index].0]);
    let main_file = origins.files.iter().position(|file| own(file));
    // Function definitions of the unit's own files, in source order.
    let mut defined: Vec<Defined> = Vec::new();
    for (index, item) in items.iter().enumerate() {
        let Kind::Function { body } = item.kind else {
            continue;
        };
        let Some(name) = c_items::names(item).into_iter().next().map(|n| n.name).filter(|_| is_own(index)) else {
            continue;
        };
        let (mut mentions, _) = read(item.text);
        // The same lines of the file itself: macro names before expansion.
        let (file, first) = place[index];
        let (last_file, last) = origins.at(item.end.saturating_sub(1));
        let (before, comments) = if file == last_file { read(&raw.span(&origins.files[file], first, last)) } else { Default::default() };
        mentions.extend(before);
        defined.push(Defined { item: index, name, inline: inline_head(&item.text[..body]), mentions, comments });
    }
    // Each emitted function's definition: its own, or for a nested function
    // (`name.3`) the next definition that mentions it.
    let mut holder: Vec<Option<usize>> = Vec::new();
    let mut from = 0;
    for function in emitted {
        let base = unnumbered(&function.name);
        let nested = base != function.name;
        let found = (from..defined.len()).find(|&d| if nested { defined[d].name != base && defined[d].mentions.contains(base) } else { defined[d].name == base });
        if let Some(found) = found {
            from = found;
        }
        holder.push(found);
    }
    let emitting: BTreeSet<usize> = holder.iter().flatten().copied().collect();
    // Names the unit's own files define.
    let mut definitions: Vec<Definition> = Vec::new();
    let mut undefined: Vec<(String, usize)> = Vec::new();
    let mut define = |name: String, kind: &'static str, item: usize, mentions: BTreeSet<String>, comments: Vec<String>| definitions.push(Definition { name, kind, item, until: usize::MAX, mentions, comments });
    for (index, item) in items.iter().enumerate().filter(|(index, _)| is_own(*index)) {
        match item.kind {
            Kind::Directive => {
                if let Some(name) = item.text.trim_start_matches('#').trim_start().strip_prefix("undef") {
                    undefined.push((name.trim().to_string(), index));
                }
                for name in c_items::names(item) {
                    let body = item.text.split_once(&name.name).map_or("", |(_, tail)| tail);
                    define(name.name, "define", index, words(body), Vec::new());
                }
            }
            Kind::Declaration => {
                let declared = c_items::declared(item);
                let (mentions, _) = read(item.text);
                for name in c_items::names(item).into_iter().filter(|n| n.defines) {
                    let kind = match (name.space, declared.iter().find(|d| d.name == name.name)) {
                        (Space::Tag, _) => "type",
                        (_, Some(d)) if d.typedef => "type",
                        (_, Some(d)) if d.function => continue,
                        (_, Some(d)) if d.local => "static",
                        (_, Some(_)) => "data",
                        (_, None) => "enum",
                    };
                    define(name.name, kind, index, mentions.clone(), Vec::new());
                }
            }
            _ => {}
        }
    }
    for (_, function) in defined.iter().enumerate().filter(|(d, _)| !emitting.contains(d)) {
        define(function.name.clone(), if function.inline { "inline" } else { "function" }, function.item, function.mentions.clone(), function.comments.clone());
    }
    for (k, function) in emitted.iter().enumerate() {
        if let Some(d) = holder[k].filter(|&d| function.is_static && defined[d].name == function.name) {
            define(function.name.clone(), "fn", defined[d].item, BTreeSet::new(), Vec::new());
        }
    }
    let names = Names::new(definitions, &undefined);
    // Users of each name: the emitted functions that reach it, a static
    // function itself, and the static functions the name's own text reaches.
    let mut users: BTreeMap<usize, BTreeSet<usize>> = BTreeMap::new();
    let mut reached: Vec<BTreeSet<usize>> = Vec::new();
    let mut statics: BTreeMap<usize, usize> = BTreeMap::new();
    for (k, function) in emitted.iter().enumerate() {
        let found = holder[k].map_or_else(BTreeSet::new, |d| names.reach(&defined[d].mentions, defined[d].item));
        for &entity in &found {
            users.entry(entity).or_default().insert(k);
        }
        if let Some(entity) = names.resolve(&function.name, 0).find(|&e| names.entities[e].kind == "fn") {
            users.entry(entity).or_default().insert(k);
            statics.insert(entity, k);
        }
        reached.push(found);
    }
    for (entity, described) in names.entities.iter().enumerate().filter(|(_, e)| !matches!(e.kind, "fn" | "define")) {
        for &d in &described.definitions {
            let tied: Vec<usize> = names.reach(&names.definitions[d].mentions, names.definitions[d].item).iter().filter_map(|e| statics.get(e).copied()).collect();
            if !tied.is_empty() {
                users.entry(entity).or_default().extend(tied);
            }
        }
    }
    let spans: BTreeMap<usize, (usize, usize)> = users.iter().map(|(entity, set)| (*entity, (*set.iter().next().expect("a user"), *set.iter().next_back().expect("a user")))).collect();
    let where_at = |d: usize| {
        let (file, line) = place[defined[d].item];
        if Some(file) == main_file {
            line.to_string()
        } else {
            format!("{}:{line}", short(&origins.files[file]))
        }
    };
    let tags = |k: usize| -> String {
        let Some(d) = holder[k] else {
            return String::new();
        };
        let mut found = context_tags(&defined[d].comments, None);
        for &entity in &reached[k] {
            for &helper in &names.entities[entity].definitions {
                found.extend(context_tags(&names.definitions[helper].comments, Some(&names.entities[entity].label)));
            }
        }
        found.join(" | ")
    };
    let mut gaps = Vec::new();
    for k in 0..emitted.len().saturating_sub(1) {
        let (Some(left), Some(right)) = (holder[k], holder[k + 1]) else {
            gaps.push(SourceGap { at_gap: "?".into(), ..SourceGap::default() });
            continue;
        };
        let mut gap = SourceGap { left_at: where_at(left), right_at: where_at(right), fakematch_left: tags(k), fakematch_right: tags(k + 1), ..SourceGap::default() };
        let mut shared: Vec<(usize, String)> = Vec::new();
        let mut kinds: BTreeMap<&str, usize> = BTreeMap::new();
        let (mut last, mut first) = (Vec::new(), Vec::new());
        for (&entity, &(low, high)) in &spans {
            if high <= k {
                gap.left_only += 1;
                if high == k {
                    last.push(names.show(entity, ""));
                }
            } else if low > k {
                gap.right_only += 1;
                if low == k + 1 {
                    first.push(names.show(entity, ""));
                }
            } else {
                let below = *users[&entity].range(..=k).next_back().expect("a user before");
                let above = *users[&entity].range(k + 1..).next().expect("a user after");
                shared.push((above - below, names.show(entity, format!(",{}", above - below))));
                *kinds.entry(names.entities[entity].kind).or_default() += 1;
            }
        }
        shared.sort();
        last.sort();
        first.sort();
        gap.shared = shared.len();
        gap.shared_kinds = kinds.iter().map(|(kind, n)| format!("{kind} {n}")).collect::<Vec<_>>().join(", ");
        gap.shared_names = listed(shared.into_iter().map(|(_, shown)| shown).collect());
        gap.left_last = listed(last);
        gap.right_first = listed(first);
        // What sits between the two definitions.
        let (a, b) = (defined[left].item, defined[right].item);
        if a == b {
            gap.at_gap = "one definition holds both".into();
            gaps.push(gap);
            continue;
        }
        let mut parts: Vec<String> = Vec::new();
        let (file_a, file_b) = (place[a].0, place[b].0);
        let name = |file: usize| short(&origins.files[file]).to_string();
        if file_a != file_b {
            parts.push(match (Some(file_a) == main_file, Some(file_b) == main_file) {
                (true, _) => format!("enters fragment {}", name(file_b)),
                (_, true) => format!("leaves fragment {}", name(file_a)),
                _ => format!("fragment {} to fragment {}", name(file_a), name(file_b)),
            });
        }
        // The source lines between them, file by file as the compiler read them.
        let (line_a, line_b) = (origins.line_of(items[a].end.saturating_sub(1)), origins.line_of(items[b].start));
        let mut runs: Vec<(usize, usize, usize)> = Vec::new();
        for &(file, line) in origins.lines[line_a..=line_b].iter().flatten() {
            match runs.last_mut() {
                Some(run) if run.0 == file => run.2 = line,
                _ => runs.push((file, line, line)),
            }
        }
        if let Some(run) = runs.first_mut() {
            run.1 += 1;
        }
        if let Some(run) = runs.last_mut() {
            run.2 = run.2.saturating_sub(1);
        }
        let local: BTreeSet<usize> = runs.iter().map(|run| run.0).filter(|&file| own(&origins.files[file]) && file != file_a && file != file_b && Some(file) != main_file).collect();
        if !local.is_empty() {
            parts.push(format!("includes local {}", local.iter().map(|&file| name(file)).collect::<Vec<_>>().join(" ")));
        }
        let headers: Vec<String> = origins
            .entered
            .iter()
            .filter(|(line, file, from)| *line > line_a && *line < line_b && own(&origins.files[*from]) && !own(&origins.files[*file]))
            .map(|(_, file, _)| origins.files[*file].rsplit('/').next().unwrap_or("").to_string())
            .collect();
        if !headers.is_empty() {
            parts.push(format!("includes {}: {}", headers.len(), listed(headers)));
        }
        let mut counts: BTreeMap<&str, usize> = BTreeMap::new();
        let mut between: Vec<String> = Vec::new();
        for index in (a + 1..b).filter(|&i| is_own(i) && c_items::line_marker(&items[i]).is_none()) {
            let here: Vec<usize> = (0..names.definitions.len()).filter(|&d| names.definitions[d].item == index).collect();
            if here.is_empty() {
                *counts.entry(if items[index].kind == Kind::Declaration { "declaration" } else { "directive" }).or_default() += 1;
            }
            for d in here {
                *counts.entry(names.definitions[d].kind).or_default() += 1;
                let side = match spans.get(&names.entity_of[d]) {
                    None => continue,
                    Some((_, high)) if *high <= k => ",before",
                    Some((low, _)) if *low > k => ",after",
                    Some(_) => ",both",
                };
                between.push(names.show(names.entity_of[d], side));
            }
        }
        if !counts.is_empty() {
            parts.push(counts.iter().map(|(kind, n)| format!("{kind} {n}")).collect::<Vec<_>>().join(", "));
        }
        // Comments standing alone there, from the files themselves.
        let mut comments: Vec<String> = Vec::new();
        for &(file, first, last) in runs.iter().filter(|run| own(&origins.files[run.0])) {
            let span = raw.span(&origins.files[file], first, last);
            comments.extend(c_items::items(&span).iter().filter(|item| item.kind == Kind::Comment).flat_map(|item| item.text.lines()).map(|line| line.trim_matches(|c: char| c.is_whitespace() || matches!(c, '/' | '*')).to_string()));
        }
        if !comments.is_empty() {
            let says = comments.iter().find(|line| line.chars().any(char::is_alphanumeric)).map_or_else(String::new, |line| format!(": {}", line.chars().take(60).collect::<String>()));
            parts.push(format!("comment lines {}{says}", comments.len()));
        }
        gap.at_gap = if parts.is_empty() { "nothing".into() } else { parts.join("; ") };
        gap.between = listed(between);
        gaps.push(gap);
    }
    let owner = holder.iter().map(|d| d.map(|d| defined[d].name.clone())).collect();
    Source { gaps, owner }
}

/// One function of the compiler's assembly.
#[derive(Clone, Debug, PartialEq)]
pub struct Chunk {
    pub name: String,
    /// Its lines with every local label, and every name the file numbers
    /// (`helper.3`), renumbered from the function's start.
    pub text: String,
    /// The lowest `.L` number it defines.
    pub first_label: Option<u32>,
}

/// A name without the number the file gives a nested function or a local
/// static (`helper.3` is `helper`).
fn unnumbered(name: &str) -> &str {
    match name.rsplit_once('.') {
        Some((base, number)) if !base.is_empty() && !number.is_empty() && number.bytes().all(|b| b.is_ascii_digit()) => base,
        _ => name,
    }
}

/// The functions of an assembly file as the compiler writes them: from the
/// label after `.type NAME,function` to the function's end label.
pub fn chunks(assembly: &str) -> Vec<Chunk> {
    let mut out = Vec::new();
    let mut lines = assembly.lines();
    while let Some(line) = lines.next() {
        let Some(name) = line.trim().strip_prefix(".type").and_then(|rest| rest.trim().strip_suffix(",function")) else {
            continue;
        };
        let mut numbers: HashMap<String, usize> = HashMap::new();
        let mut text = String::new();
        let mut first_label: Option<u32> = None;
        for line in lines.by_ref() {
            if line.starts_with(".Lfe") || line.trim_start().starts_with(".size") {
                break;
            }
            if let Some(number) = line.strip_prefix(".L").and_then(|rest| rest.strip_suffix(':')).and_then(|n| n.parse::<u32>().ok()) {
                first_label = Some(first_label.map_or(number, |f| f.min(number)));
            }
            let bytes = line.as_bytes();
            let mut i = 0;
            while i < bytes.len() {
                let rest = &line[i..];
                // A local label, or a word with the file's number after it.
                let label = [".LCB", ".LC", ".L"].into_iter().find(|p| rest.starts_with(p)).map(str::len);
                let word = (word_byte(bytes[i]) && !bytes[i].is_ascii_digit() && (i == 0 || !(word_byte(bytes[i - 1]) || bytes[i - 1] == b'.'))).then(|| rest.bytes().take_while(|b| word_byte(*b)).count() + 1).filter(|end| rest.as_bytes().get(end - 1) == Some(&b'.'));
                let digits = |from: usize| rest.as_bytes().get(from..).map_or(0, |tail| tail.iter().take_while(|b| b.is_ascii_digit()).count());
                match label.or(word).map(|head| (head, digits(head))) {
                    Some((head, count)) if count > 0 && !rest.as_bytes().get(head + count).is_some_and(|b| word_byte(*b)) => {
                        let next = numbers.len();
                        let _ = write!(text, "{}#{}", &rest[..head], numbers.entry(rest[..head + count].to_string()).or_insert(next));
                        i += head + count;
                    }
                    _ => {
                        let step = rest.chars().next().map_or(1, char::len_utf8);
                        text.push_str(&rest[..step]);
                        i += step;
                    }
                }
            }
            text.push('\n');
        }
        out.push(Chunk { name: name.trim().to_string(), text, first_label });
    }
    out
}

/// Each chunk's name without the file's number, with its place among the
/// chunks of that name counted from the end: what stays the same when
/// functions before it leave the file.
fn keys(chunks: &[Chunk]) -> Vec<(&str, usize)> {
    let mut seen: HashMap<&str, usize> = HashMap::new();
    let mut keys: Vec<(&str, usize)> = chunks
        .iter()
        .rev()
        .map(|chunk| {
            let count = seen.entry(unnumbered(&chunk.name)).or_default();
            *count += 1;
            (unnumbered(&chunk.name), *count - 1)
        })
        .collect();
    keys.reverse();
    keys
}

/// A top-level function definition of a preprocessed text: its name, start,
/// opening brace, end and whether it is declared inline.
pub fn definitions(text: &str) -> Vec<(String, usize, usize, usize, bool)> {
    c_items::items(text)
        .iter()
        .filter_map(|item| {
            let Kind::Function { body } = item.kind else {
                return None;
            };
            let name = c_items::names(item).into_iter().next()?.name;
            Some((name, item.start, item.start + body, item.end, inline_head(&item.text[..body])))
        })
        .collect()
}

/// The file as it would start at a gap: every named definition's body gone,
/// its prototype left in its place. An inline definition stays.
pub fn without(text: &str, definitions: &[(String, usize, usize, usize, bool)], remove: &BTreeSet<&str>) -> String {
    let mut out = String::with_capacity(text.len());
    let mut at = 0;
    for (name, start, body, end, inline) in definitions {
        if *inline || !remove.contains(name.as_str()) || *start < at {
            continue;
        }
        out.push_str(&text[at..*start]);
        out.push_str(text[*start..*body].trim_end());
        out.push_str(";\n");
        at = *end;
    }
    out.push_str(&text[at..]);
    out
}

/// What became of the functions after a gap, nearest first: their names and
/// whether each changed.
pub fn outcome(after: &[(&str, bool)]) -> String {
    let changed = after.iter().filter(|(_, changed)| *changed).count();
    match after.iter().position(|(_, changed)| *changed) {
        None => format!("same {}", after.len()),
        Some(nearest) => format!("changes {changed} of {}, nearest +{} {}", after.len(), nearest + 1, after[nearest].0),
    }
}

/// `work` over `0..count` on every processor, results in order.
fn parallel<T: Send>(count: usize, work: impl Fn(usize) -> T + Sync) -> Vec<T> {
    let next = AtomicUsize::new(0);
    let found: Mutex<Vec<(usize, T)>> = Mutex::new(Vec::new());
    let workers = std::thread::available_parallelism().map_or(1, |n| n.get()).min(count.max(1));
    std::thread::scope(|scope| {
        for _ in 0..workers {
            scope.spawn(|| loop {
                let index = next.fetch_add(1, Ordering::Relaxed);
                if index >= count {
                    break;
                }
                let result = work(index);
                found.lock().expect("found").push((index, result));
            });
        }
    });
    let mut found = found.into_inner().expect("found");
    found.sort_by_key(|(index, _)| *index);
    found.into_iter().map(|(_, result)| result).collect()
}

/// One C file with several functions, as one image links it.
struct Unit {
    image: usize,
    object: String,
    source: String,
    /// Its functions' indices in the image, in order.
    functions: Vec<usize>,
    emitted: Vec<Emitted>,
}

/// A unit's compile: its source read, and the whole file's assembly.
struct Prepared {
    source: Source,
    plain: String,
    definitions: Vec<(String, usize, usize, usize, bool)>,
    whole: Vec<Chunk>,
    family: CompilerFamily,
    /// Why nothing was measured, when the whole file's compile is not the build's.
    problem: Option<String>,
}

struct Workshop<'a> {
    root: &'a Path,
    target: DecompTarget,
    directory: PathBuf,
}

impl Workshop<'_> {
    fn stem(&self, unit: &Unit, tag: &str) -> PathBuf {
        self.directory.join(format!("{}-{tag}", unit.object.trim_end_matches(".o").replace('/', "_")))
    }

    /// The unit preprocessed as its build does, with `extra` options.
    fn preprocessed(&self, unit: &Unit, tag: &str, extra: &[&str]) -> Result<String, String> {
        let stem = self.stem(unit, tag);
        let output = stem.with_extension("i");
        let steps = crate::build_rom::c_steps(self.target, Path::new(&unit.source), &stem.with_extension("o"))?;
        let mut command = crate::build_rom::preprocessor_only(&steps, &unit.source, &output)?;
        command.splice(1..1, extra.iter().map(|flag| flag.to_string()));
        crate::process::run(&command, self.root)?;
        let text = fs::read(&output).map(|bytes| String::from_utf8_lossy(&bytes).into_owned()).map_err(|e| format!("{}: {e}", output.display()));
        let _ = fs::remove_file(&output);
        text
    }

    /// The assembly the unit's compiler writes for preprocessed `text`;
    /// `assemble` also leaves its object.
    fn assembly(&self, unit: &Unit, tag: &str, text: &str, family: CompilerFamily, assemble: bool) -> Result<String, String> {
        let stem = self.stem(unit, tag);
        let input = stem.with_extension("c");
        fs::write(&input, text).map_err(|e| format!("{}: {e}", input.display()))?;
        let steps = crate::build_rom::c_steps_routed(self.target, &input, &stem.with_extension("o"), Some(family))?;
        let compile = steps.len() - usize::from(!assemble);
        let result = steps[..compile].iter().try_for_each(|step| crate::process::run(step, self.root).map(|_| ())).and_then(|()| fs::read_to_string(stem.with_extension("s")).map_err(|e| e.to_string()));
        for extension in ["c", "i", "s"] {
            let _ = fs::remove_file(stem.with_extension(extension));
        }
        result
    }

    fn prepare(&self, unit: &Unit) -> Result<Prepared, String> {
        let family = crate::compiler::routing::family_for_source(self.target.compiler, &unit.source);
        let plain = self.preprocessed(unit, "plain", &[])?;
        let listing = self.preprocessed(unit, "defines", &["-dD"])?;
        let raw = |file: &str| fs::read(self.root.join(file)).ok().map(|bytes| String::from_utf8_lossy(&bytes).into_owned());
        let source = source(&listing, &self.root.to_string_lossy(), &unit.emitted, &raw);
        let definitions = definitions(&plain);
        let whole = chunks(&self.assembly(unit, "whole", &plain, family, true)?);
        // The measured compile must be the build's own.
        let object = self.stem(unit, "whole").with_extension("o");
        let built = text_section(&self.root.join(self.target.output_dir).join("obj").join(&unit.object));
        let problem = (built.is_none() || text_section(&object) != built).then(|| "unmeasured: the preprocessed file does not assemble to the built object's code".to_string());
        let _ = fs::remove_file(&object);
        Ok(Prepared { source, plain, definitions, whole, family, problem })
    }

    /// The gap after function `k`: the right function's first label in the
    /// whole file and in the file started here, and how the rest compares.
    fn trial(&self, unit: &Unit, prepared: &Prepared, k: usize) -> (String, String) {
        if let Some(problem) = &prepared.problem {
            return ("-".into(), problem.clone());
        }
        let owner = &prepared.source.owner;
        // Definitions whose every function lies before the gap.
        let remove: BTreeSet<&str> = owner[..=k].iter().flatten().map(String::as_str).filter(|name| !owner[k + 1..].iter().flatten().any(|later| later == name)).collect();
        let fresh = match self.assembly(unit, &k.to_string(), &without(&prepared.plain, &prepared.definitions, &remove), prepared.family, false) {
            Ok(assembly) => chunks(&assembly),
            Err(error) => return ("-".into(), format!("does not compile: {}", error.lines().find(|l| l.contains("rror")).or_else(|| error.lines().next()).unwrap_or(""))),
        };
        let fresh_keys = keys(&fresh);
        let fresh: HashMap<(&str, usize), &Chunk> = fresh_keys.into_iter().zip(&fresh).collect();
        let whole_keys = keys(&prepared.whole);
        let whole: HashMap<&str, usize> = prepared.whole.iter().enumerate().map(|(i, chunk)| (chunk.name.as_str(), i)).collect();
        // Each function after the gap in both compiles.
        let pairs: Vec<(Option<&Chunk>, Option<&Chunk>)> = unit.emitted[k + 1..]
            .iter()
            .map(|function| {
                let at = whole.get(function.name.as_str()).copied();
                (at.map(|i| &prepared.whole[i]), at.and_then(|i| fresh.get(&whole_keys[i]).copied()))
            })
            .collect();
        let number = |chunk: Option<&Chunk>| chunk.and_then(|c| c.first_label).map_or_else(|| "-".to_string(), |n| n.to_string());
        let label = format!("{}/{}", number(pairs[0].0), number(pairs[0].1));
        let after: Vec<(&str, bool)> = unit.emitted[k + 1..].iter().zip(&pairs).map(|(function, (was, is))| (function.name.as_str(), was.map(|c| &c.text) != is.map(|c| &c.text))).collect();
        (label, outcome(&after))
    }
}

/// The bytes of an object's `.text`.
fn text_section(path: &Path) -> Option<Vec<u8>> {
    use object::{Object, ObjectSection};
    let bytes = fs::read(path).ok()?;
    let file = object::File::parse(&*bytes).ok()?;
    let section = file.section_by_name(".text")?;
    section.data().ok().map(<[u8]>::to_vec)
}

fn clean(text: &str) -> String {
    text.replace(['\t', '\n', '\r'], " ")
}

/// The rows of `devices` that name one of two functions, as one cell.
fn device_rows(devices: &str, names: [&str; 2]) -> String {
    devices
        .lines()
        .filter(|line| line.split('\t').any(|field| names.contains(&field.trim())))
        .map(|line| line.split('\t').map(str::trim).filter(|f| !f.is_empty()).collect::<Vec<_>>().join(" "))
        .collect::<Vec<_>>()
        .join(" | ")
}

pub const HEADER: &str = "file\timage\tgap\tleft\tright\tleft_at\tright_at\tverdict\tp\tat_gap\tbetween\tshared\tshared_kinds\tshared_names\tleft_only\tright_only\tleft_last\tright_first\tpad\tlabel\tfresh\tfakematch_left\tfakematch_right\tdevices";

pub fn run(arguments: &[String]) -> Result<(), String> {
    if arguments.iter().any(|a| a == "-h" || a == "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    let options = parse(arguments)?;
    let root = crate::compiler::routing::root().to_path_buf();
    let (target, judged): (DecompTarget, Vec<Judged>) = super::flatten::judged(&root, &options.build)?;
    let devices = options.devices.as_ref().and_then(|path| fs::read_to_string(path).map_err(|e| println!("{}: {e}; the devices column stays empty", path.display())).ok());
    // Every C file with several functions, where an image links it.
    let mut units: Vec<Unit> = Vec::new();
    for (index, j) in judged.iter().enumerate() {
        let image = &j.loaded.image;
        let mut by_object: BTreeMap<&str, Vec<usize>> = BTreeMap::new();
        for (i, f) in image.functions.iter().enumerate() {
            by_object.entry(&f.object).or_default().push(i);
        }
        for (object, functions) in by_object {
            let source = format!("{}.c", object.trim_end_matches(".o"));
            if functions.len() < 2 || !functions.iter().any(|&i| image.functions[i].lang == Lang::C) || !root.join(&source).is_file() {
                continue;
            }
            let emitted = functions.iter().map(|&i| Emitted { name: image.functions[i].name.clone(), is_static: image.functions[i].is_static }).collect();
            units.push(Unit { image: index, object: object.to_string(), source, functions, emitted });
        }
    }
    let workshop = Workshop { root: &root, target, directory: root.join(target.output_dir).join("obj/units/floor") };
    fs::create_dir_all(&workshop.directory).map_err(|e| format!("{}: {e}", workshop.directory.display()))?;
    let prepared: Vec<Result<Prepared, String>> = parallel(units.len(), |u| workshop.prepare(&units[u]));
    let mut trials: Vec<(usize, usize)> = Vec::new();
    for (u, unit) in units.iter().enumerate() {
        match &prepared[u] {
            Ok(_) => trials.extend((0..unit.emitted.len() - 1).map(|k| (u, k))),
            Err(error) => println!("{}: not read: {}", unit.source, error.lines().next().unwrap_or("")),
        }
    }
    // The longest compiles first, so no worker is left alone with them.
    let mut order: Vec<usize> = (0..trials.len()).collect();
    order.sort_by_key(|&t| std::cmp::Reverse(units[trials[t].0].emitted.len() - trials[t].1));
    let measured = parallel(order.len(), |n| {
        let (u, k) = trials[order[n]];
        workshop.trial(&units[u], prepared[u].as_ref().expect("prepared"), k)
    });
    let results: HashMap<(usize, usize), &(String, String)> = order.iter().map(|&t| trials[t]).zip(&measured).collect();
    let _ = fs::remove_dir(&workshop.directory);

    let command = format!("alchemy units evidence{}{}", if options.build == "tbs-en" { String::new() } else { format!(" --build {}", options.build) }, if devices.is_some() { " --devices FILE" } else { "" });
    let mut out = format!("# Evidence at every gap between two adjacent functions of one C file of {}, read from the merged source and its own compile; regenerate with `{command}` after a build (--help explains the columns). Every row is evidence for a later cut, not a recovered boundary.\n{HEADER}\n", options.build);
    let mut rows = 0;
    let mut tally: BTreeMap<&str, usize> = BTreeMap::new();
    for (u, unit) in units.iter().enumerate() {
        let Ok(prepared) = &prepared[u] else {
            continue;
        };
        let j = &judged[unit.image];
        let image = &j.loaded.image;
        for (k, pair) in unit.functions.windows(2).enumerate() {
            let Some(g) = j.gaps.iter().find(|g| g.index == pair[0]).filter(|_| pair[1] == pair[0] + 1) else {
                continue;
            };
            let (a, b) = (&image.functions[pair[0]], &image.functions[pair[1]]);
            let s = &prepared.source.gaps[k];
            let (label, fresh) = results.get(&(u, k)).map_or(("-", "-"), |r| (r.0.as_str(), r.1.as_str()));
            let pad = if a.pad.is_empty() { "0".to_string() } else { format!("{}:{}", a.pad.len(), a.pad.iter().map(|b| format!("{b:02x}")).collect::<String>()) };
            let device = devices.as_deref().map_or_else(String::new, |d| device_rows(d, [&a.name, &b.name]));
            let _ = writeln!(
                out,
                "{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{:.3}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}",
                unit.source,
                image.name,
                g.index,
                a.name,
                b.name,
                s.left_at,
                s.right_at,
                g.verdict.label(),
                g.probability,
                clean(&s.at_gap),
                clean(&s.between),
                s.shared,
                s.shared_kinds,
                clean(&s.shared_names),
                s.left_only,
                s.right_only,
                clean(&s.left_last),
                clean(&s.right_first),
                pad,
                label,
                clean(fresh),
                clean(&s.fakematch_left),
                clean(&s.fakematch_right),
                clean(&device),
            );
            rows += 1;
            let mut count = |what: &'static str, holds: bool| *tally.entry(what).or_default() += usize::from(holds);
            count("no name shared across the gap", s.shared == 0 && s.at_gap != "?");
            count("a name shared across the gap", s.shared > 0);
            count("no shared name and names of its own on both sides", s.shared == 0 && s.left_only > 0 && s.right_only > 0);
            count("something in the source at the gap", !matches!(s.at_gap.as_str(), "nothing" | "?"));
            count("a fragment's edge at the gap", s.at_gap.contains("fragment"));
            count("includes at the gap", s.at_gap.contains("includes "));
            count("comment lines at the gap", s.at_gap.contains("comment lines"));
            count("names defined at the gap", !s.between.is_empty());
            count("padding between the functions", !a.pad.is_empty());
            count("a later function changes when the file starts at the gap", fresh.starts_with("changes"));
            count("the right function changes when the file starts at the gap", fresh.contains("nearest +1 "));
            count("the trial was not measured", !fresh.starts_with("changes") && !fresh.starts_with("same"));
            count("a FAKEMATCH tag naming file context on either side", !s.fakematch_left.is_empty() || !s.fakematch_right.is_empty());
            count("a devices row", !device.is_empty());
            count("a function the source does not show", s.at_gap == "?");
        }
    }
    let path = options.out.unwrap_or_else(|| Path::new("recon").join(Path::new(target.game_dir()).file_name().unwrap_or_default()).join("floor_evidence.tsv"));
    let full = root.join(&path);
    if let Some(parent) = full.parent() {
        fs::create_dir_all(parent).map_err(|e| format!("{}: {e}", parent.display()))?;
    }
    fs::write(&full, out).map_err(|e| format!("{}: {e}", full.display()))?;
    println!("{rows} gaps in {} files: {}", units.len(), path.display());
    for (what, n) in tally {
        println!("{n:>6}  {what}");
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    fn emitted(names: &[(&str, bool)]) -> Vec<Emitted> {
        names.iter().map(|(name, is_static)| Emitted { name: name.to_string(), is_static: *is_static }).collect()
    }

    /// A unit as `-dD` leaves it: a shared header, the file's own names, a
    /// fragment with a function of its own and a macro that ends.
    const LISTING: &str = "# 1 \"games/tbs/src/menu/unit.c\"
# 1 \"/root/games/tbs/include/types.h\" 1
typedef int s32;
static inline s32 Shared(s32 a) { return a; }
# 2 \"games/tbs/src/menu/unit.c\" 2
#define LIMIT 3
static const s32 Table[] = { 1, 2 };
static inline s32 Twice(s32 a) { return a + Table[0]; }
s32 First(s32 a)
{
    return Twice(a) + 3;
}

static s32 Second(s32 a)
{
    return Table[a];
}
# 20 \"games/tbs/src/menu/unit.c\"
#undef LIMIT
# 1 \"/root/games/tbs/include/other.h\" 1
extern s32 gOther;
# 22 \"games/tbs/src/menu/unit.c\" 2
struct Pair { s32 a; };
#define LIMIT 4
static void (*const Handlers[])(void) = { (void (*)(void))Second };
# 1 \"games/tbs/src/menu/part.c\" 1
s32 Third(struct Pair *pair)
{
    return pair->a + 4 + Shared(1);
}
# 26 \"games/tbs/src/menu/unit.c\" 2
s32 Fourth(void)
{
    return Handlers[0] != 0;
}
";

    fn raw(file: &str) -> Option<String> {
        match file {
            "games/tbs/src/menu/unit.c" => Some(
                "#include \"types.h\"\n#define LIMIT 3\nstatic const s32 Table[] = { 1, 2 };\nstatic inline s32 Twice(s32 a) { return a + Table[0]; }\ns32 First(s32 a)\n{\n    return Twice(a) + LIMIT; /* FAKEMATCH: the inline helper keeps the constant pool apart */\n}\n\nstatic s32 Second(s32 a)\n{\n    return Table[a];\n}\n\n\n\n\n// ------\n// part.c\n#undef LIMIT\n#include \"other.h\"\nstruct Pair { s32 a; };\n#define LIMIT 4\nstatic void (*const Handlers[])(void) = { (void (*)(void))Second };\n#include \"part.c\"\ns32 Fourth(void)\n{\n    return Handlers[0] != 0;\n}\n"
                    .to_string(),
            ),
            "games/tbs/src/menu/part.c" => Some("s32 Third(struct Pair *pair)\n{\n    return pair->a + LIMIT + Shared(1);\n}\n".to_string()),
            _ => None,
        }
    }

    #[test]
    fn a_gap_shows_the_names_that_tie_its_sides_and_what_stands_there() {
        let read = source(LISTING, "/root", &emitted(&[("First", false), ("Second", true), ("Third", false), ("Fourth", false)]), &raw);
        assert_eq!(read.owner, ["First", "Second", "Third", "Fourth"].map(|name| Some(name.to_string())));
        let [first, second, third] = &read.gaps[..] else {
            panic!("three gaps");
        };
        // Table ties First (through its helper) to Second; the first LIMIT is First's alone.
        assert_eq!((first.left_at.as_str(), first.right_at.as_str(), first.at_gap.as_str()), ("5", "10", "nothing"));
        assert_eq!((first.shared, first.shared_kinds.as_str(), first.shared_names.as_str()), (1, "static 1", "Table(static,1)"));
        assert_eq!((first.left_only, first.left_last.as_str()), (2, "LIMIT(define) Twice(inline)"));
        assert_eq!(first.fakematch_left, "[inline,pool] the inline helper keeps the constant pool apart");
        assert_eq!(first.fakematch_right, "");
        // Handlers names the static Second and Fourth reads it: both tie Second to
        // Fourth across the fragment. The second LIMIT is another name than the first.
        assert_eq!(second.right_at, "menu/part.c:1");
        assert_eq!(second.at_gap, "enters fragment menu/part.c; includes 1: other.h; define 1, directive 1, static 1, type 1; comment lines 2: part.c");
        assert_eq!(second.between, "Pair(type,after) LIMIT#2(define,after) Handlers(static,both)");
        assert_eq!((second.shared, second.shared_kinds.as_str(), second.shared_names.as_str()), (2, "fn 1, static 1", "Handlers(static,2) Second(fn,2)"));
        assert_eq!((second.left_only, second.right_only), (3, 2));
        assert_eq!((second.left_last.as_str(), second.right_first.as_str()), ("Table(static)", "LIMIT#2(define) Pair(type)"));
        assert_eq!((third.at_gap.as_str(), third.shared, third.right_only), ("leaves fragment menu/part.c", 2, 0));
    }

    #[test]
    fn a_nested_function_stands_in_the_definition_that_holds_it() {
        let listing = "# 1 \"games/tbs/src/a.c\"\nint Outer(int a)\n{\n    int Inner(int b) { return b; }\n    return Inner(a);\n}\nint Next(void)\n{\n    return 0;\n}\n";
        let read = source(listing, "/root", &emitted(&[("Inner.0", true), ("Outer", false), ("Next", false)]), &|_| None);
        assert_eq!(read.owner, [Some("Outer".to_string()), Some("Outer".to_string()), Some("Next".to_string())]);
        assert_eq!(read.gaps[0].at_gap, "one definition holds both");
        assert_eq!(read.gaps[1].at_gap, "nothing");
    }

    #[test]
    fn functions_compare_by_their_code_not_by_the_file_s_numbering() {
        let whole = "\t.text\n\t.type\t First,function\nFirst:\n\tbx\tlr\n.Lfe1:\n\t.size\t First,.Lfe1-First\n\t.type\t Helper.3,function\nHelper.3:\n\tldr\tr0, .L40\n\tbne\t.LCB12\n\tbl\tHelper.3\n.L41:\n\t.align\t2\n.L40:\n\t.word\tcount.7\n.Lfe2:\n";
        let fresh = "\t.type\t Helper.0,function\nHelper.0:\n\tldr\tr0, .L9\n\tbne\t.LCB12\n\tbl\tHelper.0\n.L10:\n\t.align\t2\n.L9:\n\t.word\tcount.2\n.Lfe1:\n";
        let (whole, fresh) = (chunks(whole), chunks(fresh));
        assert_eq!(whole.iter().map(|c| (c.name.as_str(), c.first_label)).collect::<Vec<_>>(), [("First", None), ("Helper.3", Some(40))]);
        assert_eq!(fresh[0].first_label, Some(9));
        assert_eq!(whole[1].text, fresh[0].text);
        assert_eq!(whole[1].text, "Helper.#0:\n\tldr\tr0, .L#1\n\tbne\t.LCB#2\n\tbl\tHelper.#0\n.L#3:\n\t.align\t2\n.L#1:\n\t.word\tcount.#4\n");
        assert_eq!(keys(&whole), [("First", 0), ("Helper", 0)]);
        assert_ne!(chunks("\t.type\t F,function\nF:\n\tmov\tr0, #1\n.Lfe1:\n")[0].text, chunks("\t.type\t F,function\nF:\n\tmov\tr0, #2\n.Lfe1:\n")[0].text);
    }

    #[test]
    fn a_file_started_at_a_gap_keeps_prototypes_and_inline_bodies() {
        let text = "static inline int Helper(void) { return 1; }\nint First(int a)\n{\n    return a;\n}\nint Second(void)\n{\n    return First(Helper());\n}\n";
        let found = definitions(text);
        assert_eq!(found.iter().map(|d| (d.0.as_str(), d.4)).collect::<Vec<_>>(), [("Helper", true), ("First", false), ("Second", false)]);
        let started = without(text, &found, &BTreeSet::from(["Helper", "First"]));
        assert_eq!(started, "static inline int Helper(void) { return 1; }\nint First(int a);\n\nint Second(void)\n{\n    return First(Helper());\n}\n");
    }

    #[test]
    fn an_outcome_counts_what_changes_and_names_the_nearest() {
        assert_eq!(outcome(&[("A", false), ("B", false)]), "same 2");
        assert_eq!(outcome(&[("A", false), ("B", true), ("C", true)]), "changes 2 of 3, nearest +2 B");
    }

    #[test]
    fn tags_and_device_rows_are_picked_by_what_they_name() {
        let comments = ["/* FAKEMATCH: r9 is pinned */".to_string(), "/* FAKEMATCH: this wrapper only moves the compiler's constant label count */".to_string()];
        assert_eq!(context_tags(&comments, Some("Wrap")), ["[label via Wrap] this wrapper only moves the compiler's constant label count"]);
        assert_eq!(device_rows("name\tdevice\nFirst\tpin r9\nOther\tbarrier\nSecond\t\tvolatile\n", ["First", "Second"]), "First pin r9 | Second volatile");
    }
}
