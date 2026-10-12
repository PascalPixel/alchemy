//! `alchemy unit split`: the inverse of `unit flatten`. One C file is cut
//! in two before a function; what every file can see stays as it was, and
//! the six editions judge the cut.
//!
//! The head keeps the file's name. The tail starts with the head's include
//! block. A file-scope macro, type, declaration or inline function of the
//! head goes where it is used: to the tail, or, when both pieces use it,
//! to the head's unit header; where the header cannot hold it, each piece
//! keeps a copy. The unit header's declarations of what the tail defines
//! move to a unit header of the tail's own, which every includer of the
//! head's header gains. The link list gains the tail's row after each row
//! of the head.

use crate::c_items::{self, Item, Kind, Space};
use crate::headers::model::{include_names, spelling, unit_header, Model, SOURCE};
use crate::headers::plan::identifiers;
use crate::headers::scan::{entries, What};
use crate::headers::{generate, undeclared};
use crate::object_list::{self, Row};
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::Path;

const USAGE: &str = "usage: alchemy unit split FILE --before FUNCTION [--name NEW] [--trial]
Cuts the ☀️ C file FILE in two before the definition of FUNCTION, with the
comment block directly above it. The head stays FILE. The tail becomes NEW
in the same folder; without --name it is FILE's name with the next free
number (window.c gives window2.c).
The tail starts with the head's include block. Each file-scope macro,
type (typedef, struct, union, enum), declaration (extern or prototype)
and inline function written before the cut goes where it is used. Used
only after the cut, it moves to the tail. Used on both sides, it moves to
the head's unit header (games/tbs/include/unit/<path>.h), which both
pieces read. Used only before the cut, it stays. A #if block of such
items moves whole.
The header is read by other files, so it cannot hold everything. An item
both sides use stays in the head and is copied to the tail when another
header or C file declares or defines its name too (the file's own view of
a name, such as an extern under another type), or when it is not a macro
and names something from a header the unit header does not read (no
include is added: other files would see it). Such a copy is a second
declaration, and alchemy check headers counts it.
The unit header's declarations of the functions and data the tail defines
move to a unit header of NEW, and every file that includes the head's
unit header gains an include of the new one right after it: no file sees
less than before, and no prototype is hidden. A function of the head that
its header leaves out (called with no prototype in scope) and the tail
names gets its prototype written in the tail, where its definition stood
above before. The game's object list gains a row for NEW after each row
of FILE, with its place, sections and editions.
Then the six ☀️ editions are built and compared (make compare-tbs-<lang>).
If one differs or does not build, every file is put back and the command
fails with the first difference. --trial always puts every file back and
prints `same` or the first difference. It prints how many macros, types,
externs and inlines went to the tail, were copied to it, and went to the
header.
Refused, with the names: FUNCTION is not a function of FILE, or stands
inside a #if block, or is the file's first; a static function or static
datum of one side is named on the other; the tail names a macro or type
that cannot move (it shares a #if block with code, or it is #undef'd).
Limits. ☀️ game files of the default Thumb route only. Use is read from
names, not from scopes: a local or a field that shares a static's name
refuses the cut, and one that shares a macro's or type's name moves it. A
macro or type moved to the header is seen by every file that reads it,
and earlier than where it stood; only the build tells whether that is
harmless. alchemy headers apply would write the header again without the
macros put there. The cut keeps the bytes, not the counts of alchemy
check headers: each copied declaration, each prototype written in the
tail and each left-out function the tail names is one more finding, and
taking those out again is a later trial of its own. One cut a run; the
number in a placeholder name is the next free one, not the file's place
in ROM order. A build cut short leaves the cut in the tree: git checkout
and removing the two new files puts it back. `same` is six ☀️ editions;
run make compare-editions for all twelve before a commit. The tree itself
must build identical first.";

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
enum Class {
    Macro,
    Type,
    Extern,
    Inline,
}

const CLASSES: [(Class, &str); 4] = [(Class::Macro, "macros"), (Class::Type, "types"), (Class::Extern, "externs"), (Class::Inline, "inlines")];

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
enum Place {
    Head,
    Tail,
    Header,
    Both,
}

/// One file-scope item before the cut, with the comment block above it.
#[derive(Debug, Default)]
struct Part {
    start: usize,
    end: usize,
    /// What the item holds that can move: one class for each macro, type,
    /// declaration or inline function.
    classes: Vec<Class>,
    /// The names those define.
    names: BTreeSet<String>,
    /// The static functions and static data it defines.
    statics: BTreeSet<String>,
    /// Each function and datum it defines, with the range of its text that
    /// says its type.
    shows: Vec<(String, usize, usize)>,
    /// It holds code, data or a directive that must stay where it is.
    fixed: bool,
}

impl Part {
    fn moves(&self) -> bool {
        !self.fixed && !self.classes.is_empty()
    }
}

fn directive_word(text: &str) -> &str {
    let rest = text.trim_start().trim_start_matches('#').trim_start();
    &rest[..rest.find(|c: char| !c.is_ascii_alphabetic()).unwrap_or(rest.len())]
}

fn has_word(text: &str, words: &[&str]) -> bool {
    text.split(|c: char| !(c.is_ascii_alphanumeric() || c == '_')).any(|word| words.contains(&word))
}

/// What one item adds to its part.
fn describe(item: &Item, part: &mut Part, base: usize) {
    let named = |part: &mut Part, class: Class| {
        part.classes.push(class);
        part.names.extend(c_items::names(item).into_iter().map(|name| name.name));
    };
    match &item.kind {
        Kind::Comment => {}
        Kind::Include(_) => part.fixed = true,
        Kind::Directive => match directive_word(item.text) {
            "define" if c_items::names(item).iter().any(|name| name.space == Space::Macro) => named(part, Class::Macro),
            "if" | "ifdef" | "ifndef" | "elif" | "else" | "endif" => {}
            word => {
                part.fixed = true;
                if word == "undef" {
                    part.names.extend(item.text.split_whitespace().nth(1).map(str::to_string));
                }
            }
        },
        Kind::Conditional => {
            for piece in c_items::flat(item.text) {
                describe(&piece.item, part, base + item.start);
            }
        }
        Kind::Declaration => {
            let declared = c_items::declared(item);
            let first = item.text.split_whitespace().next().unwrap_or("");
            if declared.iter().all(|d| d.typedef) && matches!(first, "typedef" | "struct" | "union" | "enum") {
                named(part, Class::Type);
            } else if !declared.is_empty() && declared.iter().all(|d| d.function || (d.external && !d.initialized)) {
                named(part, Class::Extern);
            } else {
                part.fixed = true;
                let end = base + item.start + item.text.find('=').unwrap_or(item.text.len());
                part.shows.extend(declared.iter().map(|d| (d.name.clone(), base + item.start, end)));
                part.statics.extend(declared.into_iter().filter(|d| d.local).map(|d| d.name));
            }
        }
        Kind::Function { body } => {
            let head = &item.text[..*body];
            if has_word(head, &["inline", "__inline__", "__inline"]) && has_word(head, &["static", "extern"]) {
                named(part, Class::Inline);
            } else {
                part.fixed = true;
                let name = c_items::names(item).into_iter().next().map(|name| name.name);
                part.shows.extend(name.clone().map(|name| (name, base + item.start, base + item.start + body)));
                if has_word(head, &["static"]) {
                    part.statics.extend(name);
                }
            }
        }
    }
}

/// Where an item's comment block starts: the comments directly above it,
/// with no blank line between.
fn lead(text: &str, items: &[Item], at: usize) -> usize {
    let mut first = at;
    while first > 0 && items[first - 1].kind == Kind::Comment && text[items[first - 1].end..items[first].start].matches('\n').count() < 2 {
        first -= 1;
    }
    text[..items[first].start].rfind('\n').map_or(0, |n| n + 1)
}

/// The names a piece reaches: those its own text names, and those named
/// by each moving item it reaches.
fn reach(seed: &[&str], text: &str, parts: &[Part]) -> BTreeSet<String> {
    let mut found: BTreeSet<String> = seed.iter().flat_map(|piece| identifiers(piece)).map(str::to_string).collect();
    let mut taken = vec![false; parts.len()];
    let mut shown = BTreeSet::new();
    loop {
        let mut grew = false;
        for (i, part) in parts.iter().enumerate() {
            if !taken[i] && part.moves() && part.names.iter().any(|name| found.contains(name)) {
                taken[i] = true;
                grew = true;
                found.extend(identifiers(&text[part.start..part.end]).into_iter().map(str::to_string));
            }
            // Who names a function or datum is shown its type, as its
            // header's declaration shows it.
            for (name, start, end) in &part.shows {
                if found.contains(name) && shown.insert(*start) {
                    grew = true;
                    found.extend(identifiers(&text[*start..*end]).into_iter().map(str::to_string));
                }
            }
        }
        if !grew {
            return found;
        }
    }
}

/// The text without the ranges, each taken out through the end of its last
/// line; one blank line stays where two would meet.
fn without(text: &str, ranges: &[(usize, usize)]) -> String {
    let mut out = String::with_capacity(text.len());
    let mut at = 0;
    for &(start, end) in ranges {
        out.push_str(&text[at..start]);
        at = text[end..].find('\n').map_or(text.len(), |n| end + n + 1);
        if out.is_empty() || out.ends_with("\n\n") {
            at += text[at..].len() - text[at..].trim_start_matches('\n').len();
        }
    }
    out.push_str(&text[at..]);
    out
}

fn listed(names: impl IntoIterator<Item = String>) -> String {
    names.into_iter().collect::<Vec<_>>().join(", ")
}

/// A file cut in two.
#[derive(Debug)]
struct Cut {
    head: String,
    /// The tail's include block and the items moved or copied to it, and
    /// its code.
    top: String,
    rest: String,
    /// The items both pieces use, for the head's unit header.
    shared: String,
    /// How many items of each class went to the tail, went to the header,
    /// and were copied to the tail.
    moved: [[usize; 4]; 3],
    /// The functions and data only the tail defines for other code.
    defines: BTreeSet<String>,
    /// The functions of the head and of the tail.
    functions: (usize, usize),
}

/// Cut `text` before `function`. `own` is how an include line spells the
/// head's unit header; `elsewhere` tells whether another header, or another file
/// that reads the head's header, declares a name too; `behind` tells whether
/// a name is defined (or, for an inline function's body, declared) by a
/// header that itself reads the head's unit header.
fn cut(text: &str, function: &str, own: &str, elsewhere: &dyn Fn(&str) -> bool, behind: &dyn Fn(&str, bool) -> bool) -> Result<Cut, String> {
    let items = c_items::items(text);
    let is = |item: &Item| matches!(item.kind, Kind::Function { .. }) && c_items::names(item).first().is_some_and(|name| name.name == function);
    let Some(at) = items.iter().position(is) else {
        let inside = c_items::flat(text).iter().any(|piece| is(&piece.item));
        return Err(if inside { format!("{function} stands inside a #if block: a cut there would divide the block") } else { format!("{function} is not a function of the file") });
    };
    if !items[..at].iter().any(|item| matches!(item.kind, Kind::Function { .. })) {
        return Err(format!("{function} is the file's first function: nothing would stay"));
    }
    // The include block: from the first include to the last one before any code.
    let code = |item: &Item| matches!(item.kind, Kind::Declaration | Kind::Function { .. } | Kind::Conditional);
    let first_code = items.iter().position(code).unwrap_or(0);
    let includes: Vec<usize> = (0..first_code).filter(|&i| matches!(items[i].kind, Kind::Include(_))).collect();
    let (block_start, block_end, body) = match (includes.first(), includes.last()) {
        (Some(&first), Some(&last)) => (items[first].start, items[last].end, last + 1),
        _ => (0, 0, 0),
    };
    let cut_at = lead(text, &items, at);
    let mut parts: Vec<Part> = Vec::new();
    for i in body..at {
        if items[i].kind == Kind::Comment {
            continue;
        }
        let mut part = Part { start: lead(text, &items, i).max(block_end), end: items[i].end, ..Part::default() };
        describe(&items[i], &mut part, 0);
        parts.push(part);
    }
    // A macro that is #undef'd means different things at different lines.
    let undefined: BTreeSet<String> = parts.iter().filter(|part| part.fixed && part.classes.is_empty()).flat_map(|part| part.names.iter().cloned()).collect();
    for part in parts.iter_mut().filter(|part| part.names.iter().any(|name| undefined.contains(name))) {
        part.fixed = true;
    }
    let mut after = Part::default();
    for item in &items[at..] {
        describe(item, &mut after, 0);
    }

    let stays: Vec<&str> = parts.iter().filter(|part| !part.moves()).map(|part| &text[part.start..part.end]).collect();
    let head_reach = reach(&stays, text, &parts);
    let tail_reach = reach(&[&text[cut_at..]], text, &parts);
    let across: BTreeSet<String> = parts.iter().flat_map(|part| &part.statics).filter(|name| tail_reach.contains(*name)).cloned().collect();
    if !across.is_empty() {
        return Err(format!("static before the cut and named after it: {}", listed(across)));
    }
    let back: BTreeSet<String> = after.statics.iter().filter(|name| head_reach.contains(*name)).cloned().collect();
    if !back.is_empty() {
        return Err(format!("static after the cut and named before it: {}", listed(back)));
    }
    let held: BTreeSet<String> = parts.iter().filter(|part| part.fixed).flat_map(|part| &part.names).filter(|name| tail_reach.contains(*name) && !after.names.contains(*name)).cloned().collect();
    if !held.is_empty() {
        return Err(format!("named after the cut and unable to move (in a #if block with code, or #undef'd): {}", listed(held)));
    }

    // Where each moving item goes. One both pieces use goes to the header,
    // unless its name is declared elsewhere too: such a declaration is the
    // file's own view of the name, and each piece keeps a copy.
    let names = |part: &Part, reach: &BTreeSet<String>| part.names.iter().any(|name| reach.contains(name));
    let mut places: Vec<Place> = parts
        .iter()
        .map(|part| match (part.moves() && names(part, &tail_reach), names(part, &head_reach)) {
            (false, _) => Place::Head,
            (true, false) => Place::Tail,
            (true, true) if part.names.iter().any(|name| elsewhere(name)) => Place::Both,
            (true, true) => Place::Header,
        })
        .collect();
    // A type, declaration or inline function cannot stand in the header
    // when it names a copied item, or a name whose header reads the unit
    // header itself (the header would be read before that name is defined).
    while let Some(at) = (0..parts.len()).find(|&i| {
        let words = identifiers(&text[parts[i].start..parts[i].end]);
        let inline = parts[i].classes.contains(&Class::Inline);
        places[i] == Place::Header
            && parts[i].classes.iter().any(|class| *class != Class::Macro)
            && (words.iter().any(|word| behind(word, inline)) || (0..parts.len()).any(|k| places[k] == Place::Both && parts[k].names.iter().any(|name| words.contains(name.as_str()))))
    }) {
        places[at] = Place::Both;
    }
    let mut moved = [[0; 4]; 3];
    let (mut tail_items, mut shared, mut gone) = (String::new(), String::new(), Vec::new());
    for (part, place) in parts.iter().zip(&places) {
        let (to, counts) = match place {
            Place::Head => continue,
            Place::Tail => (&mut tail_items, &mut moved[0]),
            Place::Header => (&mut shared, &mut moved[1]),
            Place::Both => (&mut tail_items, &mut moved[2]),
        };
        to.push_str(text[part.start..part.end].trim_end());
        to.push_str("\n\n");
        for class in &part.classes {
            counts[CLASSES.iter().position(|(known, _)| known == class).expect("a class")] += 1;
        }
        if *place != Place::Both {
            gone.push((part.start, part.end));
        }
    }

    let mut block = text[block_start..block_end].to_string();
    let mut head = without(&text[..cut_at], &gone).trim_end().to_string() + "\n";
    if !shared.is_empty() && !include_names(&block).iter().any(|name| name == own) {
        let line = format!("{}#include \"{own}\"", if block.is_empty() { "" } else { "\n" });
        block.push_str(&line);
        head.insert_str(block_end, &if block_end == 0 { format!("{line}\n\n") } else { line });
    }
    let block = if block.is_empty() { block } else { block + "\n\n" };
    let (top, rest) = (format!("{block}{tail_items}"), text[cut_at..].to_string());
    let defined = |text: &str| -> BTreeSet<String> { entries(text).into_iter().filter(|entry| matches!(entry.what, What::Definition { .. } | What::Object) && !entry.local).map(|entry| entry.name).collect() };
    let kept = defined(&head);
    let defines = defined(&text[cut_at..]).into_iter().filter(|name| !kept.contains(name)).collect();
    let count = |text: &str| c_items::items(text).iter().filter(|item| matches!(item.kind, Kind::Function { .. })).count();
    let functions = (count(&head), count(&tail_items) + count(&rest));
    Ok(Cut { head, top, rest, shared: shared.trim_end().to_string(), moved, defines, functions })
}

/// The prototypes the tail needs written out: of each function the head
/// defines for other code that its header leaves out and the tail names.
/// Before the cut the definition itself stood above the tail.
fn left_out_prototypes(head: &str, left_out: &BTreeSet<&str>, rest: &str) -> (Vec<String>, String) {
    let named = identifiers(rest);
    let (mut names, mut out, mut open) = (Vec::new(), String::new(), String::new());
    let all = entries(head);
    for entry in &all {
        let What::Definition { prototype } = &entry.what else {
            continue;
        };
        if entry.local || !left_out.contains(entry.name.as_str()) || !named.contains(entry.name.as_str()) || out.contains(&format!("{prototype}\n")) {
            continue;
        }
        // One wording in every arm that defines it needs no condition.
        let one = all.iter().filter(|other| other.name == entry.name).all(|other| other.what == entry.what);
        let condition = if one { String::new() } else { entry.conditions.join(" && ") };
        if condition != open {
            out.push_str(if open.is_empty() { "" } else { "#endif\n" });
            out.push_str(&if condition.is_empty() { String::new() } else { format!("#if {condition}\n") });
            open = condition;
        }
        out.push_str(&format!("{prototype}\n"));
        if !names.contains(&entry.name) {
            names.push(entry.name.clone());
        }
    }
    out.push_str(if open.is_empty() { "" } else { "#endif\n" });
    out.push_str(if out.is_empty() { "" } else { "\n" });
    (names, out)
}

/// The head's unit header without its declarations of what the tail
/// defines, and the tail's unit header with them, if there are any.
fn split_header(header: &str, tail: &BTreeSet<String>, new_path: &str, own: &str) -> Result<(String, Option<String>), String> {
    let guard = |condition: &&String| !(condition.starts_with("!defined(") && condition.ends_with("_H)"));
    let mut moved: BTreeMap<(usize, usize), String> = BTreeMap::new();
    let mut kept: BTreeSet<(usize, usize)> = BTreeSet::new();
    for entry in entries(header) {
        if !matches!(entry.what, What::Prototype | What::Extern) {
            continue;
        }
        if tail.contains(&entry.name) {
            moved.insert((entry.start, entry.end), entry.conditions.iter().filter(guard).cloned().collect::<Vec<_>>().join(" && "));
        } else {
            kept.insert((entry.start, entry.end));
        }
    }
    if let Some((start, end)) = moved.keys().find(|range| kept.contains(range)) {
        return Err(format!("the unit header declares names of both pieces in one declaration: {}", header[*start..*end].trim()));
    }
    for (name, start, end) in undeclared::left_out(header) {
        if tail.contains(name) {
            moved.insert((start, end), String::new());
        }
    }
    if moved.is_empty() {
        return Ok((header.to_string(), None));
    }
    let lines: Vec<(String, String)> = moved.iter().map(|((start, end), condition)| (condition.clone(), header[*start..*end].trim().to_string())).collect();
    let said: BTreeSet<&str> = lines.iter().flat_map(|(_, line)| identifiers(line)).collect();
    let tags: BTreeSet<String> = header
        .lines()
        .filter(|line| {
            let words: Vec<&str> = line.trim_end_matches(';').split_whitespace().collect();
            line.ends_with(';') && matches!(words[..], ["struct" | "union" | "enum", tag] if said.contains(tag))
        })
        .map(str::to_string)
        .collect();
    let mut includes: BTreeSet<String> = include_names(header).into_iter().collect();
    // The types the head's header holds are in reach of the tail's too.
    if !generate::held(header).is_empty() {
        includes.insert(own.to_string());
    }
    let new = generate::render(new_path, tags, includes, &[], &lines);
    // An arm of a conditional left with nothing goes with it.
    let rest = without(header, &moved.keys().copied().collect::<Vec<_>>());
    let mut rest: Vec<&str> = rest.lines().collect();
    while let Some(at) = (1..rest.len()).find(|&i| rest[i].trim_start().starts_with("#endif") && rest[i - 1].trim_start().starts_with("#if")) {
        rest.drain(at - 1..=at);
    }
    Ok((rest.join("\n") + "\n", Some(new)))
}

/// A unit header with the shared items before its closing line.
fn with_shared(header: &str, shared: &str) -> String {
    let close = header.rfind("\n#endif").map_or(header.len(), |n| n + 1);
    format!("{}\n\n{shared}\n\n{}", header[..close].trim_end(), &header[close..])
}

/// The headers, as an include spells them, that a header reads: those it
/// includes, and those they include.
fn visible(headers: &BTreeMap<String, Vec<String>>, includes: &[String]) -> BTreeSet<String> {
    let mut found: BTreeSet<String> = BTreeSet::new();
    let mut next: Vec<&String> = includes.iter().collect();
    while let Some(header) = next.pop() {
        if found.insert(header.clone()) {
            next.extend(headers.get(header).into_iter().flatten());
        }
    }
    found
}

/// The text with an include of `new` on the line after its include of `old`.
fn include_after(text: &str, old: &str, new: &str) -> Option<String> {
    let is = |line: &str, name: &str| line.trim_start().strip_prefix("#include").is_some_and(|rest| rest.trim_start().strip_prefix('"').and_then(|rest| rest.split('"').next()) == Some(name));
    if text.lines().any(|line| is(line, new)) {
        return None;
    }
    let mut found = false;
    let out: String = text
        .split_inclusive('\n')
        .map(|line| {
            if is(line, old) && !found {
                found = true;
                let indent = &line[..line.find('#').unwrap_or(0)];
                format!("{}\n{indent}#include \"{new}\"\n", line.trim_end_matches('\n'))
            } else {
                line.to_string()
            }
        })
        .collect();
    found.then_some(out)
}

/// The list with a row of `new` after each row of `old`: its image,
/// place, sections and editions.
fn with_rows(rows: &[Row], old: &str, new: &str) -> Vec<Row> {
    rows.iter().flat_map(|row| if row.object == old { vec![row.clone(), Row { object: new.to_string(), ..row.clone() }] } else { vec![row.clone()] }).collect()
}

/// The next free numbered name beside a file: window.c gives window2.c,
/// and so does window2.c while window.c is there.
fn placeholder(file: &str, exists: &dyn Fn(&str) -> bool) -> Result<String, String> {
    let (folder, name) = file.rsplit_once('/').unwrap_or(("", file));
    let stem = name.trim_end_matches(".c");
    let bare = stem.trim_end_matches(|c: char| c.is_ascii_digit());
    let base = if bare != stem && !bare.is_empty() && exists(&format!("{folder}/{bare}.c")) { bare } else { stem };
    let taken = |stem: &str| ["c", "s", "S", "h", "mid"].iter().any(|extension| exists(&format!("{folder}/{stem}.{extension}")));
    let found = (2..1000).map(|n| format!("{base}{n}")).find(|stem| !taken(stem)).ok_or("no free number")?;
    Ok(format!("{folder}/{found}.c"))
}

fn eight_three(file: &str) -> Result<(), String> {
    let stem = file.rsplit_once('/').map_or(file, |(_, name)| name).trim_end_matches(".c");
    let fits = !stem.is_empty() && stem.len() <= 8 && stem.chars().all(|c| c.is_ascii_lowercase() || c.is_ascii_digit() || c == '_');
    if fits && file.ends_with(".c") {
        Ok(())
    } else {
        Err(format!("{file}: not a lower-case 8.3 C file name; pass --name"))
    }
}

/// What a cut writes, and what was there: dropped, it puts every file back
/// unless the cut was kept.
struct Written<'a> {
    root: &'a Path,
    before: Vec<(String, Option<Vec<u8>>)>,
    keep: bool,
}

impl Written<'_> {
    fn write(&mut self, file: &str, text: &str) -> Result<(), String> {
        let path = self.root.join(file);
        self.before.push((file.to_string(), fs::read(&path).ok()));
        fs::write(&path, text).map_err(|e| format!("{file}: {e}"))
    }
}

impl Drop for Written<'_> {
    fn drop(&mut self) {
        if self.keep {
            return;
        }
        for (file, before) in self.before.iter().rev() {
            let path = self.root.join(file);
            let done = match before {
                Some(bytes) => fs::write(&path, bytes),
                None => fs::remove_file(&path),
            };
            if let Err(error) = done {
                eprintln!("{file}: not put back: {error}");
            }
        }
    }
}

/// Build and compare the six ☀️ editions as `make compare-tbs-<lang>` does.
/// The first difference, or none.
fn compare(root: &Path) -> Result<Option<String>, String> {
    let alchemy = std::env::current_exe().map_err(|e| e.to_string())?.to_string_lossy().into_owned();
    let results: Vec<Result<String, String>> = std::thread::scope(|scope| {
        let runs: Vec<_> = object_list::LANGUAGES
            .iter()
            .map(|language| {
                let alchemy = &alchemy;
                scope.spawn(move || crate::process::run(&["make", "--no-print-directory", &format!("compare-tbs-{language}"), &format!("ALCHEMY={alchemy}")], root))
            })
            .collect();
        runs.into_iter().map(|run| run.join().unwrap_or_else(|_| Err("the build stopped".into()))).collect()
    });
    for (language, result) in object_list::LANGUAGES.iter().zip(results) {
        let Err(error) = result else {
            continue;
        };
        let edition = format!("tbs-{language}");
        // The hash differs: `alchemy objects` names what differs. Else the build's own first error.
        let detail = if error.contains("did NOT match") || error.contains("FAILED") {
            let found = crate::process::run(&[alchemy.as_str(), "objects", &edition], root).unwrap_or_else(|e| e);
            found.lines().map(str::trim).find(|line| !line.is_empty()).unwrap_or("the ROM differs").to_string()
        } else {
            let first = crate::headers::objects::first_error(&error).replace(&format!("{}/", root.display()), "");
            if first.is_empty() {
                error.lines().next().unwrap_or("the build failed").to_string()
            } else {
                first
            }
        };
        return Ok(Some(format!("{edition}: {detail}")));
    }
    Ok(None)
}

/// Each name the file's headers give, with the header as an include spells
/// it and whether the header defines the name or only declares it.
fn index(model: &Model, file: &str, own: &str) -> BTreeMap<String, (String, bool)> {
    let mut found = BTreeMap::new();
    for header in model.reach.get(file).into_iter().flatten().filter(|header| header.ends_with(".h")) {
        let Some(spelled) = spelling(header).filter(|spelled| spelled != own) else {
            continue;
        };
        for piece in c_items::flat(&model.text[header]) {
            for name in c_items::names(&piece.item) {
                if name.space == Space::Tag && !name.defines {
                    continue;
                }
                let defines = name.defines || name.space == Space::Macro;
                found.entry(name.name).or_insert((spelled.clone(), defines));
            }
        }
    }
    found
}

/// The names declared or defined outside the file: by any header the
/// game's files read, its own unit header among them, and by any other C
/// file. The file's declaration of such a name is its own view of it, and
/// its definition of one would meet the other where both are read.
fn elsewhere(model: &Model, file: &str) -> BTreeSet<String> {
    let files = model.headers.iter().chain(&model.units).chain(&model.fragments).filter(|other| *other != file);
    files.flat_map(|other| c_items::flat(&model.text[other])).flat_map(|piece| c_items::names(&piece.item)).filter(|name| name.defines || name.space != Space::Tag).map(|name| name.name).collect()
}

fn counts(counts: &[usize; 4]) -> String {
    CLASSES.iter().zip(counts).map(|((_, name), count)| format!("{name} {count}")).collect::<Vec<_>>().join(", ")
}

fn split(arguments: &[String]) -> Result<(), String> {
    let (mut file, mut before, mut name, mut trial) = (None, None, None, false);
    let mut rest = arguments.iter();
    while let Some(argument) = rest.next() {
        match argument.as_str() {
            "--trial" => trial = true,
            flag @ ("--before" | "--name") => {
                let value = rest.next().ok_or_else(|| format!("{flag} needs a value\n{USAGE}"))?.clone();
                *(if flag == "--before" { &mut before } else { &mut name }) = Some(value);
            }
            path if !path.starts_with('-') && file.is_none() => file = Some(path.trim_start_matches("./").to_string()),
            other => return Err(format!("unknown argument {other}\n{USAGE}")),
        }
    }
    let (Some(file), Some(function)) = (file, before) else {
        return Err(USAGE.into());
    };
    crate::compiler::routing::prefer_installed_binutils();
    let root = crate::compiler::routing::root();
    let exists = |path: &str| root.join(path).exists();
    let header = unit_header(&file).filter(|_| root.join(&file).is_file()).ok_or_else(|| format!("{file}: not a C file under {SOURCE}"))?;
    let own = spelling(&header).ok_or("the unit header has no spelling")?;
    let folder = file.rsplit_once('/').map_or("", |(folder, _)| folder);
    let new = match name {
        Some(name) if !name.contains('/') => format!("{folder}/{name}"),
        Some(name) => return Err(format!("{name}: NEW is a file name; the tail goes beside {file}")),
        None => placeholder(&file, &exists)?,
    };
    eight_three(&new)?;
    let new_header = unit_header(&new).ok_or("no unit header for the tail")?;
    if let Some(taken) = [&new, &new_header].into_iter().find(|path| exists(path)) {
        return Err(format!("{taken} already exists; pass --name"));
    }
    let target = crate::targets::target_for(crate::targets::DecompTargetId::TbsJa);
    let family = |source: &str| format!("{:?}", crate::compiler::routing::family_for_source(target.compiler, source));
    if family(&file) != family(&new) || family(&new) != family(&format!("{SOURCE}x/x.c")) {
        return Err(format!("{file}: its compiler is chosen by its path ({}), which the tail would not share", family(&file)));
    }

    let text = fs::read_to_string(root.join(&file)).map_err(|e| format!("{file}: {e}"))?;
    let model = Model::read(root)?;
    let declared = elsewhere(&model, &file);
    let index = index(&model, &file, &own);
    // What the unit header reads is all a declaration in it can name.
    let spelled: BTreeMap<String, Vec<String>> = model.headers.iter().filter_map(|header| Some((spelling(header)?, include_names(&model.text[header])))).collect();
    let seen = visible(&spelled, &fs::read_to_string(root.join(&header)).map(|text| include_names(&text)).unwrap_or_default());
    let behind = |word: &str, any: bool| index.get(word).is_some_and(|(header, defines)| (*defines || any) && !seen.contains(header));
    let made = cut(&text, &function, &own, &|name| declared.contains(name), &behind)?;
    if !model.units.contains(&file) {
        return Err(format!("{file}: no ☀️ edition compiles it as a file of its own"));
    }
    let rows = object_list::read(root, target)?.ok_or("☀️ has no object list")?;
    let listed = with_rows(&rows, file.trim_end_matches(".c"), new.trim_end_matches(".c"));
    let added = listed.len() - rows.len();
    if added == 0 {
        return Err(format!("{file}: no row of the object list links it"));
    }
    let list = target.object_list().ok_or("☀️ has no object list")?.to_string_lossy().into_owned();
    let list_text = object_list::written(root, target, listed)?;

    let old_header = fs::read_to_string(root.join(&header)).ok();
    let (mut head_header, tail_header) = match &old_header {
        Some(found) => split_header(found, &made.defines, &new_header, &own)?,
        None => (generate::render(&header, BTreeSet::new(), BTreeSet::new(), &[], &[]), None),
    };
    if !made.shared.is_empty() {
        head_header = with_shared(&head_header, &made.shared);
    }
    let new_own = spelling(&new_header).ok_or("the tail's unit header has no spelling")?;

    println!("{file} before {function}: {} functions stay, {} go to {new}", made.functions.0, made.functions.1);
    println!("  to the tail: {}", counts(&made.moved[0]));
    println!("  copied to the tail (a view of a name declared elsewhere too, or what the header cannot hold): {}", counts(&made.moved[2]));
    println!("  to the header {header}: {}", counts(&made.moved[1]));

    let mut written = Written { root, before: Vec::new(), keep: false };
    let mut includers = 0;
    let left_out: BTreeSet<&str> = old_header.as_deref().map(undeclared::left_out).unwrap_or_default().into_iter().map(|(name, _, _)| name).collect();
    let (prototyped, prototypes) = left_out_prototypes(&made.head, &left_out, &made.rest);
    if !prototyped.is_empty() {
        println!("  prototypes written in the tail, of functions the header leaves out: {}", prototyped.join(", "));
    }
    let (mut head, mut tail) = (made.head, format!("{}{prototypes}{}", made.top, made.rest));
    if let Some(tail_header) = &tail_header {
        // Every file that includes the head's header sees the tail's too.
        let found = crate::process::run(&["git", "grep", "-l", "-F", &format!("\"{own}\""), "--", "games", "recon"], root).unwrap_or_default();
        for other in found.lines().filter(|other| *other != file) {
            let before = fs::read_to_string(root.join(other)).map_err(|e| format!("{other}: {e}"))?;
            if let Some(after) = include_after(&before, &own, &new_own) {
                written.write(other, &after)?;
                includers += 1;
            }
        }
        head = include_after(&head, &own, &new_own).unwrap_or(head);
        tail = include_after(&tail, &own, &new_own).unwrap_or(tail);
        written.write(&new_header, tail_header)?;
        let declarations = entries(tail_header).len() + undeclared::left_out(tail_header).len();
        println!("  {declarations} declarations to {new_header}, which {includers} other files now include");
    }
    println!("  {added} row{} in {list}", if added == 1 { "" } else { "s" });
    if old_header.as_deref() != Some(head_header.as_str()) && (old_header.is_some() || !made.shared.is_empty()) {
        written.write(&header, &head_header)?;
    }
    written.write(&file, &head)?;
    written.write(&new, &tail)?;
    written.write(&list, &list_text)?;

    let difference = compare(root)?;
    match (difference, trial) {
        (None, false) => {
            written.keep = true;
            println!("same in the six ☀️ editions: the cut is in the tree ({} files written)", written.before.len());
            Ok(())
        }
        (None, true) => {
            println!("same");
            Ok(())
        }
        (Some(difference), true) => {
            println!("{difference}");
            Ok(())
        }
        (Some(difference), false) => Err(format!("every file is put back: {difference}")),
    }
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    match arguments.split_first() {
        Some((command, rest)) if command == "split" => {
            if rest.iter().any(|a| a == "-h" || a == "--help") {
                println!("{USAGE}");
                return Ok(());
            }
            split(rest)
        }
        _ => super::flatten::run(arguments),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    const OWN: &str = "unit/a/file.h";
    const NOWHERE: fn(&str) -> bool = |_| false;
    const NEVER: fn(&str, bool) -> bool = |_, _| false;

    const SOURCE_TEXT: &str = "// banner\n\n#include \"unit/a/file.h\"\n#include \"types.h\"\n\n#define HEAD_ONLY 1\n#define TAIL_ONLY 2\n// both use it\n#define BOTH (gShared + 3)\n\nextern s32 gShared;\nextern s32 gTail;\n\nstruct Pair\n{\n\ts32 a;\n};\n\n#if EDITION_INTERNATIONAL\n#define WIDE 4\n#else\n#define WIDE 5\n#endif\n\nstatic inline s32 Twice(s32 x)\n{\n\treturn x * TAIL_ONLY;\n}\n\nstatic s32 sCount;\n\ns32 First(void)\n{\n\treturn HEAD_ONLY + BOTH + sCount;\n}\n\n// the second\n// function\ns32 Second(struct Pair *pair)\n{\n\treturn Twice(pair->a) + BOTH + gTail + WIDE;\n}\n\ns32 Third(void)\n{\n\treturn Second(0);\n}\n";

    #[test]
    fn a_file_scope_item_goes_where_it_is_used() {
        let made = cut(SOURCE_TEXT, "Second", OWN, &NOWHERE, &NEVER).unwrap();
        // The head keeps what only it uses, and loses the rest.
        assert!(made.head.contains("#define HEAD_ONLY 1") && made.head.contains("static s32 sCount;") && made.head.ends_with("return HEAD_ONLY + BOTH + sCount;\n}\n"));
        for moved in ["TAIL_ONLY 2", "#define BOTH", "gShared;", "gTail;", "struct Pair", "WIDE", "Twice", "Second", "both use it"] {
            assert!(!made.head.contains(moved), "{moved} stays in\n{}", made.head);
        }
        // The tail starts with the include block, then what only it uses, then the function under its comments.
        assert!(made.top.starts_with("#include \"unit/a/file.h\"\n#include \"types.h\"\n\n#define TAIL_ONLY 2\n\nextern s32 gTail;\n\nstruct Pair\n"), "{}", made.top);
        assert!(made.top.contains("#if EDITION_INTERNATIONAL\n#define WIDE 4\n#else\n#define WIDE 5\n#endif\n\nstatic inline s32 Twice"));
        assert!(made.top.ends_with("}\n\n") && made.rest.starts_with("// the second\n// function\ns32 Second(") && !made.top.contains("banner"));
        // A macro both use goes to the header with what it names, under its comment.
        assert_eq!(made.shared, "// both use it\n#define BOTH (gShared + 3)\n\nextern s32 gShared;");
        assert_eq!(made.moved, [[3, 1, 1, 1], [1, 0, 1, 0], [0; 4]]);
        assert_eq!(made.functions, (1, 3));
        assert_eq!(made.defines.iter().map(String::as_str).collect::<Vec<_>>(), ["Second", "Third"]);
    }

    #[test]
    fn a_view_of_a_name_declared_elsewhere_is_copied_not_shared() {
        // Another header declares gShared with another type: the file's own
        // declaration cannot stand where other files read it.
        let made = cut(SOURCE_TEXT, "Second", OWN, &|name| name == "gShared", &NEVER).unwrap();
        assert!(made.head.contains("extern s32 gShared;\n") && made.top.contains("#define TAIL_ONLY 2\n\nextern s32 gShared;\n\nextern s32 gTail;"), "{}", made.top);
        assert_eq!(made.shared, "// both use it\n#define BOTH (gShared + 3)");
        assert_eq!(made.moved, [[3, 1, 1, 1], [1, 0, 0, 0], [0, 0, 1, 0]]);
        // A shared type that names a copied one is copied with it.
        let text = "struct Row\n{\n\ts32 a;\n};\nstruct Table\n{\n\tstruct Row rows[2];\n};\n#define COUNT 2\nvoid A(struct Table *t)\n{\n\tCOUNT;\n}\nvoid B(struct Table *t)\n{\n\tCOUNT;\n}\n";
        let made = cut(text, "B", OWN, &|name| name == "Row", &NEVER).unwrap();
        assert_eq!((made.shared.as_str(), made.moved), ("#define COUNT 2", [[0; 4], [1, 0, 0, 0], [0, 2, 0, 0]]));
        assert!(made.head.contains("struct Table") && made.top.starts_with("#include \"unit/a/file.h\"\n\nstruct Row\n{\n\ts32 a;\n};\n\nstruct Table"), "{}", made.top);
    }

    #[test]
    fn a_type_that_names_what_the_header_does_not_read_is_copied() {
        // The unit header reads types.h and, through it, base.h; battle.h it does not read.
        let headers: BTreeMap<String, Vec<String>> = [("types.h", vec!["base.h"]), ("battle.h", vec!["types.h"])].into_iter().map(|(header, includes)| (header.to_string(), includes.into_iter().map(str::to_string).collect())).collect();
        assert_eq!(visible(&headers, &["types.h".to_string()]).iter().map(String::as_str).collect::<Vec<_>>(), ["base.h", "types.h"]);
        // A struct that holds battle.h's Vector cannot stand in the unit header.
        let text = "struct Object\n{\n\tstruct Vector pos;\n};\nstruct Item\n{\n\ts32 id;\n};\nvoid A(struct Object *o, struct Item *i)\n{\n}\nvoid B(struct Object *o, struct Item *i)\n{\n}\n";
        let made = cut(text, "B", OWN, &NOWHERE, &|word, _| word == "Vector").unwrap();
        assert_eq!((made.shared.as_str(), made.moved), ("struct Item\n{\n\ts32 id;\n};", [[0; 4], [0, 1, 0, 0], [0, 1, 0, 0]]));
        assert!(made.head.contains("struct Object") && !made.head.contains("struct Item\n") && made.top.contains("struct Object\n{\n\tstruct Vector pos;\n};\n"));
    }

    #[test]
    fn a_type_reaches_the_tail_through_the_function_that_returns_it() {
        let text = "struct Row\n{\n\ts32 a;\n};\nstruct Row *Find(void)\n{\n\treturn 0;\n}\nvoid B(void)\n{\n\tFind()->a = 1;\n}\n";
        let made = cut(text, "B", OWN, &NOWHERE, &NEVER).unwrap();
        assert_eq!((made.shared.as_str(), made.moved[1]), ("struct Row\n{\n\ts32 a;\n};", [0, 1, 0, 0]));
        assert_eq!(with_shared("#ifndef G_H\n#define G_H\n\nvoid F(void);\n\n#endif\n", &made.shared), "#ifndef G_H\n#define G_H\n\nvoid F(void);\n\nstruct Row\n{\n\ts32 a;\n};\n\n#endif\n");
    }

    #[test]
    fn a_left_out_function_the_tail_names_is_declared_there() {
        let head = "s32 Put(s32 x)\n{\n\treturn x;\n}\n#if EDITION_INTERNATIONAL\nvoid Wide(void)\n{\n}\n#endif\nvoid Plain(void)\n{\n}\nstatic void Own(void)\n{\n}\n";
        let left_out: BTreeSet<&str> = ["Put", "Wide", "Unused", "Own"].into_iter().collect();
        let (names, text) = left_out_prototypes(head, &left_out, "void B(void)\n{\n\t((s32 (*)())Put)(1);\n\tWide();\n\tPlain();\n\tOwn();\n}\n");
        assert_eq!((names, text.as_str()), (vec!["Put".to_string(), "Wide".to_string()], "s32 Put(s32 x);\nvoid Wide(void);\n\n"));
        assert_eq!(left_out_prototypes(head, &left_out, "void B(void)\n{\n}\n"), (Vec::new(), String::new()));
        // Defined alike in both arms of a conditional: declared once, under no condition.
        let arms = "#if EDITION_INTERNATIONAL\nvoid Put(void)\n{\n}\n#else\nvoid Put(void)\n{\n\tPut();\n}\n#endif\n";
        assert_eq!(left_out_prototypes(arms, &left_out, "void B(void)\n{\n\tPut();\n}\n").1, "void Put(void);\n\n");
        // Worded otherwise in each arm: each wording under its arm's condition.
        let worded = arms.replacen("void Put(void)", "void Put(s32 x)", 1);
        assert_eq!(left_out_prototypes(&worded, &left_out, "void B(void)\n{\n\tPut();\n}\n").1, "#if EDITION_INTERNATIONAL\nvoid Put(s32 x);\n#endif\n#if !(EDITION_INTERNATIONAL)\nvoid Put(void);\n#endif\n\n");
    }

    #[test]
    fn the_head_gains_its_header_when_something_is_shared() {
        let made = cut("#include \"types.h\"\n\n#define BOTH 1\n\ns32 A(void)\n{\n\treturn BOTH;\n}\n\ns32 B(void)\n{\n\treturn BOTH;\n}\n", "B", OWN, &NOWHERE, &NEVER).unwrap();
        assert!(made.head.starts_with("#include \"types.h\"\n#include \"unit/a/file.h\"\n\ns32 A(void)"), "{}", made.head);
        assert_eq!((made.top.as_str(), &made.rest[..11]), ("#include \"types.h\"\n#include \"unit/a/file.h\"\n\n", "s32 B(void)"));
    }

    #[test]
    fn a_cut_that_cannot_be_neutral_is_refused_with_the_names() {
        let refused = |text: &str, function: &str| cut(text, function, OWN, &NOWHERE, &NEVER).unwrap_err();
        let statics = "static s32 sCount;\nstatic void Step(void)\n{\n}\nvoid A(void)\n{\n\tStep();\n}\nvoid B(void)\n{\n\tStep();\n\tsCount = 1;\n}\n";
        assert_eq!(refused(statics, "B"), "static before the cut and named after it: Step, sCount");
        let back = "static void Late(void);\nvoid A(void)\n{\n\tLate();\n}\nvoid B(void)\n{\n}\nstatic void Late(void)\n{\n}\n";
        assert_eq!(refused(back, "B"), "static after the cut and named before it: Late");
        let inside = "void A(void)\n{\n}\n#if EDITION_INTERNATIONAL\nvoid B(void)\n{\n}\n#endif\n";
        assert!(refused(inside, "B").starts_with("B stands inside a #if block"));
        assert_eq!(refused(inside, "C"), "C is not a function of the file");
        assert!(refused(inside, "A").contains("first function"));
        let mixed = "#if EDITION_INTERNATIONAL\n#define WIDE 1\nvoid A(void)\n{\n}\n#endif\nvoid B(void)\n{\n}\nvoid C(void)\n{\n\tB();\n\tWIDE;\n}\n";
        assert!(refused(mixed, "C").ends_with("#undef'd): WIDE"));
        let undefined = "#define N 1\nvoid A(void)\n{\n}\n#undef N\n#define N 2\nvoid B(void)\n{\n}\nvoid C(void)\n{\n\tN;\n}\n";
        assert!(refused(undefined, "C").ends_with("#undef'd): N"));
        // A static named on its own side only divides nothing.
        assert!(cut(&statics.replace("\tStep();\n\tsCount = 1;\n", ""), "B", OWN, &NOWHERE, &NEVER).is_ok());
    }

    const HEADER: &str = "#ifndef ALCHEMY_UNIT_A_FILE_H\n#define ALCHEMY_UNIT_A_FILE_H\n\n#include \"types.h\"\n#include \"edition.h\"\n\nstruct Pair;\nstruct Other;\n\ns32 First(void);\n#if EDITION_INTERNATIONAL\ns32 Second(struct Pair *pair);\n#endif\n/* Third is called before its definition with no prototype in scope. */\nextern const u8 gTable[];\nvoid Last(struct Other *other);\n\n#endif\n";

    #[test]
    fn the_header_is_split_as_the_file_is() {
        let tail: BTreeSet<String> = ["Second", "Third", "gTable"].iter().map(|name| name.to_string()).collect();
        let (head, new) = split_header(HEADER, &tail, "games/tbs/include/unit/a/file2.h", OWN).unwrap();
        assert_eq!(head, "#ifndef ALCHEMY_UNIT_A_FILE_H\n#define ALCHEMY_UNIT_A_FILE_H\n\n#include \"types.h\"\n#include \"edition.h\"\n\nstruct Pair;\nstruct Other;\n\ns32 First(void);\nvoid Last(struct Other *other);\n\n#endif\n");
        assert_eq!(
            new.unwrap(),
            "#ifndef ALCHEMY_UNIT_A_FILE2_H\n#define ALCHEMY_UNIT_A_FILE2_H\n\n#include \"types.h\"\n#include \"edition.h\"\n\nstruct Pair;\n\n#if EDITION_INTERNATIONAL\ns32 Second(struct Pair *pair);\n#endif\n/* Third is called before its definition with no prototype in scope. */\nextern const u8 gTable[];\n\n#endif\n"
        );
        // Nothing of the tail's: the header stays, and no header is made.
        assert_eq!(split_header(HEADER, &BTreeSet::new(), "x.h", OWN).unwrap(), (HEADER.to_string(), None));
    }

    #[test]
    fn an_includer_of_the_head_gains_the_tail() {
        let text = "#include \"types.h\"\n#include \"unit/a/file.h\"\n#include \"window.h\"\n";
        assert_eq!(include_after(text, OWN, "unit/a/file2.h").unwrap(), "#include \"types.h\"\n#include \"unit/a/file.h\"\n#include \"unit/a/file2.h\"\n#include \"window.h\"\n");
        assert_eq!(include_after("#include \"types.h\"\n", OWN, "unit/a/file2.h"), None);
        assert_eq!(include_after("#include \"unit/a/file.h\"\n#include \"unit/a/file2.h\"\n", OWN, "unit/a/file2.h"), None);
    }

    #[test]
    fn the_list_gains_a_row_after_each_row_of_the_head() {
        let text = format!("{}\nmain\trom_main_02\tgames/tbs/src/a/before\nmain\trom_main_02\tgames/tbs/src/a/file\t.text\nmain\trom_main_02\tgames/tbs/src/a/after\nmain\trom_main_05\tgames/tbs/src/a/file\t.rodata\tja en\n", object_list::HEADER);
        let rows = object_list::parse(&text).unwrap();
        let out = object_list::format(&with_rows(&rows, "games/tbs/src/a/file", "games/tbs/src/a/file2"));
        assert_eq!(
            out,
            format!("{}\nmain\trom_main_02\tgames/tbs/src/a/before\nmain\trom_main_02\tgames/tbs/src/a/file\t.text\nmain\trom_main_02\tgames/tbs/src/a/file2\t.text\nmain\trom_main_02\tgames/tbs/src/a/after\nmain\trom_main_05\tgames/tbs/src/a/file\t.rodata\tja en\nmain\trom_main_05\tgames/tbs/src/a/file2\t.rodata\tja en\n", object_list::HEADER)
        );
    }

    #[test]
    fn a_placeholder_is_the_next_free_number() {
        let files = |names: &'static [&'static str]| move |path: &str| names.contains(&path);
        assert_eq!(placeholder("a/window.c", &files(&["a/window.c"])).unwrap(), "a/window2.c");
        assert_eq!(placeholder("a/window2.c", &files(&["a/window.c", "a/window2.c"])).unwrap(), "a/window3.c");
        assert_eq!(placeholder("a/window.c", &files(&["a/window.c", "a/window2.h"])).unwrap(), "a/window3.c");
        assert_eq!(placeholder("a/b5.c", &files(&["a/b5.c"])).unwrap(), "a/b52.c");
        assert!(eight_three("a/window2.c").is_ok() && eight_three("a/windowing2.c").is_err() && eight_three("a/Window.c").is_err());
    }
}
