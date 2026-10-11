//! The rewrite: each C file's functions declared once, in the file's own
//! header; the declarations C files and ☀️'s headers repeat taken out; each
//! file made to include the headers of what it calls; and each call that
//! saw another declaration than the header's cast to the one it saw.

use super::expand::{Seen, Source, Stray};
use super::generate::{header, Types};
use super::model::{spelling, unit_header, Model, COMMON, INCLUDE, SOURCE, UNIT};
use super::scan::{comparable, is_function, view, Entry, What};
use crate::camelotize::trial::Edit;
use std::collections::{BTreeMap, BTreeSet, HashMap};

/// What one edition's expansion of a unit says.
#[derive(Default)]
pub(crate) struct View {
    pub defined: BTreeMap<String, String>,
    pub named: BTreeSet<String>,
    pub seen: Vec<Seen>,
    pub strays: Vec<Stray>,
}

/// Each unit's six editions.
pub(crate) type Views = BTreeMap<String, Vec<View>>;

/// A use of a name to write as a use of the declaration it saw, inside
/// `function`: a call as `((old)name)(...)`, which is a device and gets
/// its tag; an object under another type as a view of it, which is plain C.
#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Cast {
    pub at: usize,
    pub name: String,
    pub old: String,
    pub truth: String,
    pub function: String,
}

impl Cast {
    fn tagged(&self) -> bool {
        is_function(&self.old) && is_function(&self.truth)
    }
}

/// A file's rewrite: what leaves it and what it includes, and the casts
/// its calls may need.
#[derive(Default)]
pub(crate) struct Draft {
    pub edits: Vec<Edit>,
    pub casts: Vec<Cast>,
}

#[derive(Default)]
pub(crate) struct Plan {
    /// Every header to write, by path.
    pub headers: BTreeMap<String, String>,
    /// Every C file and ☀️ header to rewrite.
    pub drafts: BTreeMap<String, Draft>,
    pub problems: Vec<String>,
    pub removed: usize,
    pub guarded: usize,
}

/// The identifiers a file's code names: comments, literals and include
/// lines apart.
pub(crate) fn identifiers(text: &str) -> BTreeSet<&str> {
    let bytes = text.as_bytes();
    let mut found = BTreeSet::new();
    let mut i = 0;
    let mut line_start = true;
    while i < bytes.len() {
        let byte = bytes[i];
        if bytes[i..].starts_with(b"/*") {
            i = text[i + 2..].find("*/").map_or(bytes.len(), |at| i + 2 + at + 2);
        } else if bytes[i..].starts_with(b"//") {
            i = text[i..].find('\n').map_or(bytes.len(), |at| i + at);
        } else if byte == b'"' || byte == b'\'' {
            let mut end = i + 1;
            while end < bytes.len() && bytes[end] != byte && bytes[end] != b'\n' {
                end += if bytes[end] == b'\\' { 2 } else { 1 };
            }
            i = (end + 1).min(bytes.len());
        } else if byte == b'#' && line_start && text[i + 1..].trim_start_matches([' ', '\t']).starts_with("include") {
            i = text[i..].find('\n').map_or(bytes.len(), |at| i + at);
        } else if byte.is_ascii_alphabetic() || byte == b'_' {
            let end = (i..bytes.len()).find(|&k| !(bytes[k].is_ascii_alphanumeric() || bytes[k] == b'_')).unwrap_or(bytes.len());
            found.insert(&text[i..end]);
            i = end;
            line_start = false;
        } else {
            if byte == b'\n' {
                line_start = true;
            } else if !byte.is_ascii_whitespace() {
                line_start = false;
            }
            i += 1;
        }
    }
    found
}

/// The edit that takes a byte range out, with the rest of its lines when
/// nothing else is on them.
pub(crate) fn removal(text: &str, start: usize, end: usize) -> Edit {
    let line = text[..start].rfind('\n').map_or(0, |n| n + 1);
    let rest = text[end..].find('\n').map_or(text.len(), |n| end + n);
    if text[line..start].trim().is_empty() && text[end..rest].trim().is_empty() {
        Edit { start: line, end: (rest + 1).min(text.len()), text: String::new() }
    } else {
        Edit { start, end, text: String::new() }
    }
}

/// A removal that also takes the FAKEMATCH comment standing directly
/// above: the tag of a device goes with the device.
pub(crate) fn removal_with_tag(text: &str, start: usize, end: usize) -> Edit {
    let mut edit = removal(text, start, end);
    let above = text[..edit.start].trim_end_matches([' ', '\t']);
    let Some(above) = above.strip_suffix('\n') else {
        return edit;
    };
    let line = above.rfind('\n').map_or(0, |n| n + 1);
    let comment = if above[line..].trim_start().starts_with("//") {
        Some(line)
    } else if above.ends_with("*/") {
        above.rfind("/*").map(|open| above[..open].rfind('\n').map_or(0, |n| n + 1)).filter(|&from| above[from..].trim_start().starts_with("/*"))
    } else {
        None
    };
    if let Some(from) = comment.filter(|&from| above[from..].contains("FAKEMATCH") && edit.start == above.len() + 1) {
        edit.start = from;
    }
    edit
}

/// The text with the edits made, in order of place. Where a removal
/// leaves a blank line on both sides of the gap, one stays.
pub(crate) fn apply(text: &str, edits: &[Edit]) -> String {
    let mut sorted: Vec<&Edit> = edits.iter().collect();
    sorted.sort_by_key(|e| (e.start, e.end));
    let bytes = text.as_bytes();
    let mut out = String::with_capacity(text.len() + 256);
    let mut at = 0;
    for edit in sorted {
        if edit.start < at {
            continue;
        }
        out.push_str(&text[at..edit.start]);
        out.push_str(&edit.text);
        at = edit.end;
        if edit.text.is_empty() && edit.end > edit.start && (out.ends_with("\n\n") || out.is_empty()) {
            while bytes.get(at) == Some(&b'\n') {
                at += 1;
            }
        }
    }
    out.push_str(&text[at..]);
    out
}

/// The edit that adds the include lines a text lacks before its first
/// include, or before its first line of code when it includes nothing.
pub(crate) fn include_edit(text: &str, names: &[String]) -> Option<Edit> {
    let present = super::model::include_names(text);
    let missing: Vec<&String> = names.iter().filter(|name| !present.contains(name)).collect();
    if missing.is_empty() {
        return None;
    }
    let block: String = missing.iter().map(|name| format!("#include \"{name}\"\n")).collect();
    let pieces = crate::c_items::flat(text);
    let first = pieces.iter().find(|p| !matches!(p.item.kind, crate::c_items::Kind::Comment));
    let at = match first {
        Some(piece) => text[..piece.item.start].rfind('\n').map_or(0, |n| n + 1),
        None => text.len(),
    };
    // A header's includes go inside its guard.
    let guard = first.filter(|p| p.item.text.starts_with("#ifndef") || p.item.text.starts_with("#if !defined"));
    let (at, gap) = match guard {
        Some(_) => {
            let define = pieces.iter().find(|p| p.item.text.starts_with("#define")).map_or(at, |p| p.item.end);
            let next = pieces.iter().find(|p| p.item.start >= define && !matches!(p.item.kind, crate::c_items::Kind::Comment));
            let includes_follow = next.is_some_and(|p| matches!(p.item.kind, crate::c_items::Kind::Include(_)));
            match next {
                Some(piece) if includes_follow => (text[..piece.item.start].rfind('\n').map_or(0, |n| n + 1), ""),
                _ => (text[define..].find('\n').map_or(text.len(), |n| define + n + 1), "\n"),
            }
        }
        None => {
            let includes_follow = first.is_some_and(|p| matches!(p.item.kind, crate::c_items::Kind::Include(_)));
            (at, if includes_follow || first.is_none() { "" } else { "\n" })
        }
    };
    let lead = if guard.is_some() && gap == "\n" { "\n" } else { "" };
    Some(Edit { start: at, end: at, text: format!("{lead}{block}{}", if guard.is_some() { "" } else { gap }) })
}

/// Whether a header has an include guard. One without is a list other
/// files read their own way (the far-call banks, which the assembler
/// reads too), and takes no include.
fn guarded(text: &str) -> bool {
    let mut directives = text.lines().map(str::trim_start).filter(|line| line.starts_with('#'));
    let first = directives.next().unwrap_or("");
    (first.starts_with("#ifndef") || first.starts_with("#if !defined")) && directives.next().is_some_and(|second| second.starts_with("#define"))
}

fn plain_prototype(text: &str, entry: &Entry) -> bool {
    let item = &text[entry.start..entry.end];
    entry.declarators == 1 && !item.contains('{') && !item.contains('#') && !entry.local
}

/// The edits that write one call as a cast to the shape it saw, with the
/// tag that gives the measured reason on the line above.
pub(crate) fn cast_edits(text: &str, cast: &Cast, reason: &str) -> Vec<Edit> {
    let line = text[..cast.at].rfind('\n').map_or(0, |n| n + 1);
    let indent: String = text[line..].chars().take_while(|c| *c == ' ' || *c == '\t').collect();
    let end = cast.at + cast.name.len();
    let (before, after) = view(&cast.old);
    let mut edits = vec![Edit { start: cast.at, end: cast.at, text: before }, Edit { start: end, end, text: after.to_string() }];
    if cast.tagged() {
        edits.push(Edit { start: line, end: line, text: format!("{indent}/* FAKEMATCH: {} is called as {}; {reason} */\n", cast.name, cast.old) });
    }
    edits
}

/// What the headers and expansions say about where a name is declared.
pub(crate) struct Truths<'a> {
    model: &'a Model,
    views: &'a Views,
    /// Each unit's wanted names with the file whose header declares them.
    homes: BTreeMap<&'a str, HashMap<&'a str, &'a str>>,
    main: Vec<HashMap<&'a str, &'a str>>,
}

impl<'a> Truths<'a> {
    fn new(model: &'a Model, views: &'a Views, emitted: &BTreeMap<&'a str, BTreeSet<String>>) -> Truths<'a> {
        let definitions = model.definitions();
        let mut homes: BTreeMap<&str, HashMap<&str, &str>> = BTreeMap::new();
        for unit in &model.units {
            let found = homes.entry(unit.as_str()).or_default();
            for name in definitions.keys() {
                if let Some((home, _)) = model.home(&definitions, unit, name) {
                    if emitted.get(home).is_some_and(|names| names.contains(*name)) {
                        found.insert(name, home);
                    }
                }
            }
        }
        let mut main: Vec<HashMap<&str, &str>> = vec![HashMap::new(); 6];
        for (unit, editions) in views {
            if !model.main.contains(unit) {
                continue;
            }
            for (edition, view) in editions.iter().enumerate() {
                for (name, shape) in &view.defined {
                    main[edition].entry(name.as_str()).or_insert(shape.as_str());
                }
            }
        }
        Truths { model, views, homes, main }
    }

    /// The file whose header declares the name for a unit's code.
    pub(crate) fn home(&self, unit: &str, name: &str) -> Option<&'a str> {
        self.homes.get(unit)?.get(name).copied()
    }

    /// The shape that header gives the name in one edition.
    fn shape(&self, unit: &str, edition: usize, name: &str) -> Option<&'a str> {
        let home = self.home(unit, name)?;
        let own = self.model.parts(unit).any(|part| part == home);
        if own {
            self.views.get(unit)?.get(edition)?.defined.get(name).map(String::as_str)
        } else {
            self.main[edition].get(name).copied()
        }
    }

    fn names(&self, unit: &str, name: &str) -> bool {
        self.views.get(unit).is_some_and(|editions| editions.iter().any(|view| view.named.contains(name)))
    }

    /// A name's declarations can leave a file when every unit that
    /// compiles the file and names the name finds it in a header.
    fn settled(&self, file: &str, name: &str) -> bool {
        let users = self.model.users(file);
        users.iter().any(|unit| self.home(unit, name).is_some()) && users.iter().all(|unit| self.home(unit, name).is_some() || !self.names(unit, name))
    }
}

/// One use of a name in the source: the name, whether it is a call, and
/// what each unit's edition saw there (unit, edition, shape).
type Site<'a> = (&'a str, bool, Vec<(&'a str, usize, &'a str)>);

/// Whether a file is ☀️'s alone to rewrite.
fn own(file: &str) -> bool {
    file.starts_with(SOURCE) || (file.starts_with(INCLUDE) && !file.starts_with(&format!("{INCLUDE}{UNIT}")))
}

/// Whether a line of a shared header is ☀️'s alone: under `#if !GAME_TLA`.
fn ours_only(conditions: &[String]) -> bool {
    conditions.iter().any(|c| matches!(c.replace(' ', "").as_str(), "!GAME_TLA" | "!(GAME_TLA)"))
}

impl Plan {
    pub(crate) fn new(model: &Model, sources: &BTreeMap<String, Source>, views: &Views) -> Plan {
        let mut plan = Plan::default();
        let definitions = model.definitions();
        let types = Types::read(model);
        // The header of each C file that defines something callable.
        let mut generated: BTreeMap<&str, String> = BTreeMap::new();
        let mut emitted: BTreeMap<&str, BTreeSet<String>> = BTreeMap::new();
        for file in model.units.iter().chain(&model.fragments) {
            let Some(path) = unit_header(file) else {
                continue;
            };
            if let Some((text, names)) = header(&path, &model.entries[file], &types, &mut plan.problems) {
                generated.insert(file, path.clone());
                emitted.insert(file, names);
                plan.headers.insert(path, text);
            }
        }
        let truths = Truths::new(model, views, &emitted);
        let files: Vec<&String> = model.units.iter().chain(&model.fragments).chain(&model.headers).filter(|file| own(file) && !model.frozen.contains(*file)).collect();
        let shared: Vec<&String> = model.headers.iter().filter(|file| file.starts_with(COMMON) && !model.frozen.contains(*file)).collect();
        // What leaves each file: of a shared header, what is ☀️'s alone.
        for file in files.iter().chain(&shared) {
            let text = &model.text[*file];
            let draft = plan.drafts.entry(file.to_string()).or_default();
            let mut macros = BTreeSet::new();
            for entry in &model.entries[*file] {
                if !own(file) && !ours_only(&entry.conditions) {
                    continue;
                }
                let gone = match &entry.what {
                    What::Prototype | What::Extern => plain_prototype(text, entry) && truths.settled(file, &entry.name),
                    What::ShapeMacro { .. } => {
                        let gone = truths.settled(file, &entry.name);
                        if gone {
                            macros.insert(entry.name.as_str());
                        }
                        gone
                    }
                    What::Undef => macros.contains(entry.name.as_str()),
                    _ => false,
                };
                if gone {
                    draft.edits.push(removal_with_tag(text, entry.start, entry.end));
                }
            }
            for body in sources.get(*file).map(|s| s.bodies.as_slice()).unwrap_or(&[]) {
                for local in body.locals.iter().filter(|_| own(file)) {
                    if truths.settled(file, &local.name) {
                        draft.edits.push(removal(text, local.start, local.end));
                    }
                }
            }
            plan.removed += draft.edits.len();
        }
        // The calls that saw another declaration than the header's.
        // A use needs its old declaration when that says something else
        // than the header: a call's code depends on the whole shape, any
        // other use only on whether the name is a function or which type
        // of object it is.
        let apart = |old: &str, truth: &str, call: bool| comparable(old) != comparable(truth) && (call || !(is_function(old) && is_function(truth)));
        let mut sites: BTreeMap<(&str, usize), Site> = BTreeMap::new();
        for (unit, editions) in views {
            for (edition, view) in editions.iter().enumerate() {
                for seen in &view.seen {
                    sites.entry((seen.file.as_str(), seen.at)).or_insert((seen.name.as_str(), seen.call, Vec::new())).2.push((unit, edition, &seen.old));
                }
                let mut told = BTreeSet::new();
                for stray in &view.strays {
                    if truths.shape(unit, edition, &stray.name).is_some_and(|truth| apart(&stray.old, truth, stray.call)) && told.insert((&stray.file, stray.line, &stray.name)) && edition == 1 {
                        plan.problems.push(format!("{unit}: {}:{}: a macro uses {} as {}, not as its header declares it", stray.file, stray.line, stray.name, stray.old));
                    }
                }
            }
        }
        for ((file, at), (name, call, sights)) in sites {
            let mut olds = BTreeSet::new();
            let mut truth = None;
            for (unit, edition, old) in sights {
                if let Some(shape) = truths.shape(unit, edition, name) {
                    olds.insert(old);
                    if apart(old, shape, call) && truth.is_none() {
                        truth = Some(shape);
                    }
                }
            }
            let Some(truth) = truth else {
                continue;
            };
            let source = &sources[file];
            let line = source.line(at);
            if olds.len() > 1 || !own(file) || model.frozen.contains(file) {
                plan.problems.push(format!("{file}:{line}: {name} is called as {} by different editions or units, or in a shared header", olds.into_iter().collect::<Vec<_>>().join(" and as ")));
                continue;
            }
            let function = source.bodies.iter().find(|b| b.open < at && at < b.close).map_or(String::new(), |b| b.name.clone());
            let old = olds.into_iter().next().unwrap_or_default().to_string();
            plan.drafts.entry(file.to_string()).or_default().casts.push(Cast { at, name: name.to_string(), old, truth: truth.to_string(), function });
        }
        // What each file includes.
        for file in &files {
            let text = &model.text[*file];
            let mut includes: Vec<String> = Vec::new();
            let mut others = BTreeSet::new();
            if model.units.contains(*file) {
                for part in model.parts(file) {
                    if let Some(path) = generated.get(part) {
                        includes.extend(spelling(path));
                    }
                }
                for name in views.get(*file).into_iter().flatten().flat_map(|view| &view.named) {
                    if let Some(path) = truths.home(file, name).and_then(|home| generated.get(home)) {
                        others.extend(spelling(path));
                    }
                }
            } else if file.starts_with(INCLUDE) && guarded(text) {
                // A header on the search path includes the header of what
                // its own text calls, when that is one file for every unit.
                let gone: BTreeSet<&str> = plan.drafts[*file].edits.iter().flat_map(|e| identifiers(&text[e.start..e.end])).collect();
                let kept: BTreeSet<&str> = model.entries[*file].iter().map(|e| e.name.as_str()).filter(|name| !gone.contains(name)).collect();
                let users = model.users(file);
                for name in identifiers(text) {
                    if kept.contains(name) || !definitions.contains_key(name) {
                        continue;
                    }
                    let homes: BTreeSet<Option<&str>> = users.iter().map(|unit| truths.home(unit, name)).collect();
                    if let (1, Some(Some(home))) = (homes.len(), homes.iter().next()) {
                        if model.in_main(home) {
                            others.extend(generated.get(home).and_then(|path| spelling(path)));
                        }
                    }
                }
            }
            for name in others {
                if !includes.contains(&name) {
                    includes.push(name);
                }
            }
            if let Some(edit) = include_edit(text, &includes) {
                plan.drafts.entry(file.to_string()).or_default().edits.push(edit);
            }
        }
        // A shared header's prototype that says something else than ☀️'s
        // definition is ⚓️'s alone.
        for file in shared {
            let text = &model.text[file];
            let mut ranges: Vec<(usize, usize)> = Vec::new();
            for entry in &model.entries[file] {
                if !matches!(entry.what, What::Prototype | What::Extern) || !plain_prototype(text, entry) || entry.conditions.iter().any(|c| c.contains("GAME_TLA")) {
                    continue;
                }
                let users = model.users(file);
                let differs = users.iter().filter_map(|unit| truths.home(unit, &entry.name)).any(|home| {
                    definitions.get(entry.name.as_str()).into_iter().flatten().any(|(file, d)| *file == home && d.signature != entry.signature)
                });
                if differs {
                    ranges.push((entry.start, entry.end));
                }
            }
            plan.guarded += ranges.len();
            plan.drafts.entry(file.clone()).or_default().edits.extend(guard_edits(text, &ranges));
        }
        plan.drafts.retain(|_, draft| !draft.edits.is_empty() || !draft.casts.is_empty());
        plan
    }
}

/// The edits that put each range's lines under `#if GAME_TLA`, neighbours
/// under one.
pub(crate) fn guard_edits(text: &str, ranges: &[(usize, usize)]) -> Vec<Edit> {
    let mut ranges = ranges.to_vec();
    ranges.sort();
    let mut groups: Vec<(usize, usize)> = Vec::new();
    for (start, end) in ranges {
        let line = text[..start].rfind('\n').map_or(0, |n| n + 1);
        let rest = text[end..].find('\n').map_or(text.len(), |n| end + n + 1);
        match groups.last_mut() {
            Some((_, last)) if text[*last..line].trim().is_empty() => *last = rest,
            _ => groups.push((line, rest)),
        }
    }
    let mut edits = Vec::new();
    for (start, end) in &groups {
        edits.push(Edit { start: *start, end: *start, text: "#if GAME_TLA\n".to_string() });
        edits.push(Edit { start: *end, end: *end, text: "#endif\n".to_string() });
    }
    if !groups.is_empty() && !super::model::include_names(text).iter().any(|name| name == "edition.h") {
        // GAME_TLA is edition.h's: before the first include, or after the guard.
        let at = match text.find("#include") {
            Some(at) => text[..at].rfind('\n').map_or(0, |n| n + 1),
            None => text.find("#define").and_then(|d| text[d..].find('\n').map(|n| d + n + 1)).unwrap_or(0),
        };
        edits.push(Edit { start: at, end: at, text: "#include \"edition.h\"\n".to_string() });
    }
    edits
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn identifiers_leave_out_comments_literals_and_include_lines() {
        let found = identifiers("#include \"field_event.h\"\n#define Go(x) Run(x) // Not\ns32 a = Use('c', \"Str\"); /* Nor */\n");
        assert_eq!(found.into_iter().collect::<Vec<_>>(), ["Go", "Run", "Use", "a", "define", "s32", "x"]);
    }

    #[test]
    fn a_removed_item_takes_its_line_and_no_second_blank_line_stays() {
        let text = "#include \"a.h\"\n\nvoid F(void);\n\nvoid G(void)\n{\n}\ns32 x; void H(void); s32 y;\n";
        let f = text.find("void F").unwrap();
        let h = text.find("void H").unwrap();
        let edits = [removal(text, f, f + 13), removal(text, h, h + 13)];
        assert_eq!(apply(text, &edits), "#include \"a.h\"\n\nvoid G(void)\n{\n}\ns32 x;  s32 y;\n");
    }

    #[test]
    fn a_devices_tag_goes_with_it() {
        let text = "// keep\n/* FAKEMATCH: called as void:\n   two instructions move. */\n#define F ((void (*)(void))F)\n\n/* a note */\nvoid G(void);\n";
        let f = text.find("#define").unwrap();
        let g = text.find("void G").unwrap();
        let edits = [removal_with_tag(text, f, f + 29), removal_with_tag(text, g, g + 13)];
        assert_eq!(apply(text, &edits), "// keep\n\n/* a note */\n");
    }

    #[test]
    fn includes_go_first_and_inside_a_guard() {
        let names = vec!["unit/a.h".to_string(), "b.h".to_string()];
        let with = |text: &str, names: &[String]| apply(text, &include_edit(text, names).into_iter().collect::<Vec<_>>());
        assert_eq!(with("// head\n#include \"b.h\"\nvoid F(void)\n{\n}\n", &names), "// head\n#include \"unit/a.h\"\n#include \"b.h\"\nvoid F(void)\n{\n}\n");
        assert_eq!(with("void F(void)\n{\n}\n", &names[..1]), "#include \"unit/a.h\"\n\nvoid F(void)\n{\n}\n");
        assert_eq!(with("#ifndef H\n#define H\n\n#include \"b.h\"\n#endif\n", &names), "#ifndef H\n#define H\n\n#include \"unit/a.h\"\n#include \"b.h\"\n#endif\n");
        assert_eq!(with("#ifndef H\n#define H\nstruct P;\n#endif\n", &names[..1]), "#ifndef H\n#define H\n\n#include \"unit/a.h\"\nstruct P;\n#endif\n");
        assert_eq!(include_edit("#include \"b.h\"\n", &names[1..]), None);
    }

    #[test]
    fn a_call_is_cast_to_the_shape_it_saw_under_its_tag() {
        let text = "void Run(void)\n{\n\tif (Get(1))\n\t\tx = Get(2);\n}\n";
        let cast = Cast { at: text.rfind("Get").unwrap(), name: "Get".into(), old: "s32 (*)()".into(), truth: "u8 (*)(u8)".into(), function: "Run".into() };
        assert_eq!(
            apply(text, &cast_edits(text, &cast, "as u8 (*)(u8), its prototype, the function is 4 bytes longer")),
            "void Run(void)\n{\n\tif (Get(1))\n\t\t/* FAKEMATCH: Get is called as s32 (*)(); as u8 (*)(u8), its prototype, the function is 4 bytes longer */\n\t\tx = ((s32 (*)())Get)(2);\n}\n"
        );
    }

    #[test]
    fn contradicting_shared_prototypes_become_the_other_games() {
        let text = "#ifndef H\n#define H\n#include \"types.h\"\nvoid A(void);\nvoid B(void);\ns32 C(s32 a,\n\ts32 b);\n\nvoid D(void);\n#endif\n";
        let range = |needle: &str, len: usize| {
            let at = text.find(needle).unwrap();
            (at, at + len)
        };
        let out = apply(text, &guard_edits(text, &[range("void B", 13), range("s32 C", 22), range("void D", 13)]));
        assert_eq!(out, "#ifndef H\n#define H\n#include \"edition.h\"\n#include \"types.h\"\nvoid A(void);\n#if GAME_TLA\nvoid B(void);\ns32 C(s32 a,\n\ts32 b);\n\nvoid D(void);\n#endif\n#endif\n");
    }
}
