//! alchemy headers static: an overlay's function that no other object of
//! the overlay names is `static`.
//!
//! An overlay is one C file beside its two stub tables. The entry table
//! names the functions the engine calls; every other function is reached
//! from inside the file, by a call or through the scene's own tables, and
//! is `static` (S3). Its file's header loses the prototype, and where the
//! file names the function above its definition the prototype stands
//! once, `static`, in one block after the file's includes.

use super::generate::{editions_only, header, needs, Types};
use super::lint::place;
use super::model::{include_names, normalize, spelling, unit_header, Model};
use super::objects::{self, Bench, Built};
use super::plan::{apply, identifiers, places, removal};
use super::scan::{bodies, entries, Entry, Scope, What};
use crate::c_items::{flat, items, Kind};
use crate::camelotize::source::{mask, next, opening, previous};
use crate::camelotize::trial::Edit;
use std::collections::{BTreeMap, BTreeSet};
use std::path::Path;

/// The comment over a file's block of forward declarations.
pub(crate) const FORWARD: &str = "// 前方宣言";

/// The overlays' side of the object list.
#[derive(Default)]
pub(crate) struct Overlays {
    /// Each overlay C unit's images.
    pub image: BTreeMap<String, Vec<String>>,
    /// The identifiers each image's other objects name.
    pub named: BTreeMap<String, BTreeSet<String>>,
}

/// The identifiers an assembly source and what it includes name. A row
/// of the overlays' import list, `import NAME, TARGET`, names nothing of
/// the overlay: it is the stub the overlay's code calls when no object of
/// the overlay defines NAME.
fn assembly_names(root: &Path, file: &str, seen: &mut BTreeSet<String>, out: &mut BTreeSet<String>) {
    if !seen.insert(file.to_string()) {
        return;
    }
    let Ok(text) = std::fs::read_to_string(root.join(file)) else {
        return;
    };
    let folder = file.rsplit_once('/').map_or("", |(folder, _)| folder);
    let mut code = String::new();
    for line in text.lines() {
        let trimmed = line.trim();
        let included = [".include", "#include"].iter().find_map(|word| trimmed.strip_prefix(word)).and_then(|rest| rest.trim().strip_prefix('"')).and_then(|rest| rest.split('"').next());
        if let Some(name) = included {
            let beside = normalize(&format!("{folder}/{name}"));
            let found = if root.join(&beside).is_file() { beside } else { normalize(name) };
            assembly_names(root, &found, seen, out);
        } else if !trimmed.strip_prefix("import").is_some_and(|rest| rest.starts_with(char::is_whitespace)) {
            code.push_str(line);
            code.push('\n');
        }
    }
    out.extend(identifiers(&code).into_iter().map(str::to_string));
}

impl Overlays {
    pub(crate) fn read(root: &Path, model: &Model) -> Result<Overlays, String> {
        let game = objects::editions()[0];
        let rows = crate::object_list::read(root, game)?.unwrap_or_default();
        let mut out = Overlays::default();
        for row in rows.iter().filter(|row| row.image != crate::object_list::MAIN) {
            let named = out.named.entry(row.image.clone()).or_default();
            // A member of the compiler's library, or every object's section.
            if row.object.contains([':', '*']) {
                continue;
            }
            let source = if row.object == format!("resource_{}_overlay", row.image) {
                let data = game.overlay_data(&row.image);
                if !root.join(&data).is_file() {
                    continue;
                }
                data.to_string_lossy().into_owned()
            } else {
                crate::build_rom::source_for(root, &row.object)?.to_string_lossy().into_owned()
            };
            if model.units.contains(&source) {
                let images = out.image.entry(source).or_default();
                if !images.contains(&row.image) {
                    images.push(row.image.clone());
                }
            } else {
                assembly_names(root, &source, &mut BTreeSet::new(), named);
            }
        }
        Ok(out)
    }
}

/// What becomes of a function a file defines for other code.
#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) enum Verdict {
    /// `static`, with its forward declaration's lines (condition and
    /// prototype) when the file names it above its definition.
    Static(Vec<(String, String)>),
    /// Left as it is, and why.
    Kept(String),
}

/// A file's function definitions: each entry's index with where its head
/// begins, after any macro call that stands before it, and its body's
/// brace. A head this does not read gives no place.
pub(crate) fn heads(text: &str, all: &[Entry]) -> Vec<(usize, Option<usize>, usize)> {
    let mask = mask(text);
    let mut found = Vec::new();
    for piece in flat(text) {
        let Kind::Function { body } = piece.item.kind else {
            continue;
        };
        let Some(index) = all.iter().position(|entry| entry.start == piece.item.start && matches!(entry.what, What::Definition { .. })) else {
            continue;
        };
        let brace = piece.item.start + body;
        let close = previous(&mask, brace).filter(|&at| mask[at] == b')');
        let head = close.and_then(|close| opening(&mask, close)).and_then(|open| {
            let before = mask[piece.item.start..open].iter().rposition(|&b| b == b')').map_or(piece.item.start, |at| piece.item.start + at + 1);
            next(&mask, before).filter(|&at| at < open)
        });
        found.push((index, head, brace));
    }
    found
}

/// The structs and unions a signature only points to, each as its
/// declaration: `struct Tag;`.
pub(crate) fn pointed(signature: &str) -> Vec<String> {
    let found: Vec<&str> = signature.split(' ').filter(|t| !t.is_empty()).collect();
    (1..found.len()).filter(|&i| matches!(found[i - 1], "struct" | "union") && found.get(i + 1) == Some(&"*")).map(|i| format!("{} {};", found[i - 1], found[i])).collect()
}

/// The tags a text declares or defines at any depth, `struct Tag;` and
/// `struct Tag {`: each as `struct Tag;`, where it stands, and whether
/// that is its definition.
pub(crate) fn tag_places(text: &str) -> Vec<(String, usize, bool)> {
    let bytes = text.as_bytes();
    let all = places(text);
    let mut found = Vec::new();
    for pair in all.windows(2) {
        let ((start, keyword), (at, tag)) = (pair[0], pair[1]);
        if !matches!(keyword, "struct" | "union") {
            continue;
        }
        let after = (at + tag.len()..bytes.len()).find(|&i| !bytes[i].is_ascii_whitespace());
        if let Some(byte) = after.map(|i| bytes[i]).filter(|byte| matches!(byte, b';' | b'{')) {
            found.push((format!("{keyword} {tag};"), start, byte == b'{'));
        }
    }
    found
}

/// The tags a text declares or defines.
pub(crate) fn tags(text: &str) -> BTreeSet<String> {
    tag_places(text).into_iter().map(|(tag, _, _)| tag).collect()
}

/// Where a file's block of forward declarations stands: the line after
/// its leading header includes, or before its first line of code when it
/// includes none. `c_text` tells an include of C text, which is code, by
/// the name the include line spells.
pub(crate) fn block_place(text: &str, c_text: &dyn Fn(&str) -> bool) -> usize {
    let line_end = |at: usize| text[at..].find('\n').map_or(text.len(), |n| at + n + 1);
    let code = |text: &str| flat(text).iter().any(|piece| matches!(&piece.item.kind, Kind::Declaration | Kind::Function { .. }) || matches!(&piece.item.kind, Kind::Include(name) if c_text(name)));
    let (mut after, mut first) = (None, text.len());
    for item in items(text) {
        match &item.kind {
            Kind::Comment | Kind::Directive => continue,
            Kind::Include(name) if !c_text(name) => after = Some(line_end(item.end)),
            Kind::Conditional if !code(item.text) => {
                if !include_names(item.text).is_empty() {
                    after = Some(line_end(item.end));
                }
            }
            _ => {
                first = text[..item.start].rfind('\n').map_or(0, |n| n + 1);
                break;
            }
        }
    }
    after.unwrap_or(first)
}

/// A text without its block of forward declarations, and the block's
/// place in what is left.
pub(crate) fn without_block(text: &str) -> String {
    let Some(start) = text.match_indices(FORWARD).map(|(at, _)| at).find(|&at| text[..at].ends_with('\n') || at == 0) else {
        return text.to_string();
    };
    let end = text[start..].find("\n\n").map_or(text.len(), |n| start + n + 1);
    // The blank line before the block goes with it; at the head of a
    // file, the one after it.
    if text[..start].trim().is_empty() {
        return format!("{}{}", &text[..start], text[end..].strip_prefix('\n').unwrap_or(&text[end..]));
    }
    let start = if text[..start].ends_with("\n\n") { start - 1 } else { start };
    format!("{}{}", &text[..start], &text[end..])
}

/// The block's text: the comment, the tags its prototypes point to, and
/// each prototype under its condition.
pub(crate) fn block(tags: &BTreeSet<String>, lines: &[(String, String)]) -> String {
    let mut out = format!("{FORWARD}\n");
    for tag in tags {
        out.push_str(tag);
        out.push('\n');
    }
    let mut open = "";
    for (condition, declaration) in lines {
        if condition != open {
            if !open.is_empty() {
                out.push_str("#endif\n");
            }
            if !condition.is_empty() {
                out.push_str(&format!("#if {condition}\n"));
            }
            open = condition;
        }
        out.push_str(&format!("static {declaration}\n"));
    }
    if !open.is_empty() {
        out.push_str("#endif\n");
    }
    out
}

/// A text with a block at its place, a blank line on each side.
pub(crate) fn with_block(text: &str, block: &str, c_text: &dyn Fn(&str) -> bool) -> String {
    let at = block_place(text, c_text);
    let before = if text[..at].trim().is_empty() || text[..at].ends_with("\n\n") { "" } else { "\n" };
    let after = if text[at..].starts_with('\n') || at == text.len() { "" } else { "\n" };
    format!("{}{before}{block}{after}{}", &text[..at], &text[at..])
}

/// What the pass knows of every file before it judges one.
pub(crate) struct Scene<'a> {
    pub model: &'a Model,
    pub overlays: &'a Overlays,
    pub types: &'a Types,
    /// Each C file's identifiers by name, with where they stand.
    uses: BTreeMap<&'a str, BTreeMap<&'a str, Vec<usize>>>,
    /// Each header's identifiers.
    named: BTreeMap<&'a str, BTreeSet<&'a str>>,
    /// The names each C file's functions declare for themselves.
    inside: BTreeMap<&'a str, BTreeSet<String>>,
}

impl<'a> Scene<'a> {
    pub(crate) fn new(model: &'a Model, overlays: &'a Overlays, types: &'a Types) -> Scene<'a> {
        let mut scene = Scene { model, overlays, types, uses: BTreeMap::new(), named: BTreeMap::new(), inside: BTreeMap::new() };
        for file in model.units.iter().chain(&model.fragments) {
            let text = model.text[file].as_str();
            let found = scene.uses.entry(file).or_default();
            for (at, word) in places(text) {
                found.entry(word).or_default().push(at);
            }
            let locals = bodies(text, &|_| false).into_iter().flat_map(|body| body.locals).filter(|local| local.kind != Scope::Variable).map(|local| local.name);
            scene.inside.insert(file, locals.collect());
        }
        for header in &model.headers {
            scene.named.insert(header, identifiers(&model.text[header]));
        }
        scene
    }

    /// Whether a file is an overlay's alone: every unit that compiles it
    /// is one.
    pub(crate) fn overlay(&self, file: &str) -> bool {
        let users = self.model.users(file);
        !users.is_empty() && users.iter().all(|unit| self.overlays.image.contains_key(*unit))
    }

    /// Whether a unit names a function of `file` above a place of the
    /// file: in its own text or in a C file it includes before.
    fn above(&self, unit: &str, file: &str, name: &str, limit: usize, skip: &[(usize, usize)]) -> Option<bool> {
        let limit = place(self.model, unit, file, limit)?;
        let found = self.model.parts(unit).any(|part| {
            let Some(found) = self.uses.get(part).and_then(|uses| uses.get(name)) else {
                return false;
            };
            found.iter().any(|&at| {
                let own = part == file && skip.iter().any(|(start, end)| (*start..*end).contains(&at));
                !own && place(self.model, unit, part, at).is_some_and(|at| at < limit)
            })
        });
        Some(found)
    }

    /// What becomes of each function a file defines for other code, in
    /// the file's order.
    pub(crate) fn verdicts(&self, file: &str) -> Vec<(String, Verdict)> {
        let model = self.model;
        let text = model.text[file].as_str();
        let all = &model.entries[file];
        let own = unit_header(file);
        let existing = own.as_ref().and_then(|header| model.text.get(header)).map_or("", String::as_str);
        let left: BTreeSet<&str> = super::undeclared::left_out(existing).into_iter().map(|(name, _, _)| name).collect();
        let users = model.users(file);
        let heads = heads(text, all);
        // The types in reach where the block stands: the headers', and
        // those the file's own header holds. One the file defines for
        // itself stands below the block.
        let mut near = Types::default();
        for held in super::generate::held(existing) {
            near.add(own.as_deref().unwrap_or(file), held);
        }
        near.add("its own file", text);
        let plain = |entry: &Entry| !entry.signature.split(' ').any(|word| matches!(word, "inline" | "__inline" | "__inline__" | "__attribute__"));
        let mut order: Vec<&str> = Vec::new();
        for entry in all.iter().filter(|entry| matches!(entry.what, What::Definition { .. })) {
            if !order.contains(&entry.name.as_str()) {
                order.push(&entry.name);
            }
        }
        let mut out = Vec::new();
        for name in order {
            let places: Vec<&(usize, Option<usize>, usize)> = heads.iter().filter(|(index, _, _)| all[*index].name == name).collect();
            let kept = |why: String| (name.to_string(), Verdict::Kept(why));
            let hidden = places.iter().filter(|(index, _, _)| all[*index].local).count();
            if places.iter().any(|(index, head, _)| head.is_none() || !plain(&all[*index])) {
                // An inline function is none of this pass's.
                if hidden < places.len() {
                    out.push(kept("its definition's head is not a plain one".into()));
                }
                continue;
            }
            // A function that is static already keeps its place in the
            // block, or leaves it: the block is written anew.
            if hidden == 0 {
                if let Some(why) = self.outside(file, name, &left, own.as_deref()) {
                    out.push(kept(why));
                    continue;
                }
            } else if hidden < places.len() {
                out.push(kept("its file defines it static too".into()));
                continue;
            }
            // The file names it above its definition: in an edition whose
            // definition is the last one, every earlier naming is above.
            let last = places.iter().map(|(index, _, _)| all[*index].start).max().unwrap_or(0);
            let skip: Vec<(usize, usize)> = places.iter().map(|(index, _, brace)| (all[*index].start, *brace)).collect();
            let forward = users.iter().any(|unit| self.above(unit, file, name, last, &skip) != Some(false));
            if !forward {
                out.push((name.to_string(), Verdict::Static(Vec::new())));
                continue;
            }
            // Each wording once, under its condition when there are several.
            let mut wordings: Vec<(&str, &Entry)> = Vec::new();
            for (index, _, _) in &places {
                if let What::Definition { prototype } = &all[*index].what {
                    if !wordings.iter().any(|(text, _)| *text == prototype.as_str()) {
                        wordings.push((prototype, &all[*index]));
                    }
                }
            }
            let mut lines = Vec::new();
            let mut problem = None;
            for (prototype, entry) in &wordings {
                let condition = if wordings.len() == 1 { String::new() } else { entry.conditions.join(" && ") };
                if !editions_only(&condition) {
                    problem = Some(format!("it has several wordings under `{condition}`, which the block cannot test"));
                } else if let Err(why) = needs(&entry.signature, name, self.types, &near) {
                    problem = Some(format!("its forward declaration {why}"));
                }
                lines.push((condition, prototype.to_string()));
            }
            out.push(match problem.filter(|_| hidden == 0) {
                Some(why) => kept(why),
                None => (name.to_string(), Verdict::Static(lines)),
            });
        }
        out
    }

    /// Why a function a file defines for other code stays so, if it does.
    fn outside(&self, file: &str, name: &str, left: &BTreeSet<&str>, own: Option<&str>) -> Option<String> {
        let model = self.model;
        let all = &model.entries[file];
        let users = model.users(file);
        if left.contains(name) {
            return Some("it is called before its definition with no prototype in scope".into());
        }
        if all.iter().any(|entry| entry.name == name && entry.what == What::Prototype) {
            return Some("its file declares it too".into());
        }
        if all.iter().any(|entry| entry.name == name && matches!(entry.what, What::ShapeMacro { .. } | What::ViewMacro)) {
            return Some("a macro of its file casts it".into());
        }
        let image = users.iter().flat_map(|unit| self.overlays.image.get(*unit).into_iter().flatten()).find(|image| self.overlays.named.get(*image).is_some_and(|named| named.contains(name)));
        if let Some(image) = image {
            return Some(format!("another object of overlay {image} names it"));
        }
        let inside = users.iter().flat_map(|unit| model.parts(unit)).find(|part| self.inside.get(part).is_some_and(|names| names.contains(name)));
        if let Some(part) = inside {
            return Some(format!("a function of {part} declares it for itself"));
        }
        let header = users.iter().flat_map(|unit| model.reach[*unit].iter()).find(|reached| Some(reached.as_str()) != own && self.named.get(reached.as_str()).is_some_and(|named| named.contains(name)));
        if let Some(header) = header {
            return Some(format!("{header} names it"));
        }
        if !model.units.iter().any(|unit| unit == file) {
            let early = users.iter().find(|unit| self.above(unit, file, name, 0, &[]) != Some(false));
            if let Some(unit) = early {
                return Some(format!("{unit} names it above the line that includes its file"));
            }
        }
        None
    }

    /// A file's text with its verdicts carried out: `static` before each
    /// definition that takes it, and the block of the forward declarations
    /// in definition order. `declared` are the tags the headers the file
    /// reads declare.
    pub(crate) fn rewrite(&self, file: &str, verdicts: &[(String, Verdict)], declared: &BTreeSet<String>) -> String {
        let text = self.model.text[file].as_str();
        let all = &self.model.entries[file];
        let heads = heads(text, all);
        let taken: BTreeMap<&str, &Vec<(String, String)>> = verdicts.iter().filter_map(|(name, verdict)| match verdict {
            Verdict::Static(lines) => Some((name.as_str(), lines)),
            Verdict::Kept(_) => None,
        }).collect();
        let mut edits = Vec::new();
        let (mut lines, mut tags) = (Vec::new(), BTreeSet::new());
        let mut told = BTreeSet::new();
        // Where each tag is first needed: at the block when a forward
        // declaration points to it, else at the first head that does.
        let mut needed: BTreeMap<String, usize> = BTreeMap::new();
        for (index, head, _) in &heads {
            let entry = &all[*index];
            let (Some(forward), Some(head)) = (taken.get(entry.name.as_str()), head) else {
                continue;
            };
            if !entry.local {
                edits.push(Edit { start: *head, end: *head, text: "static ".to_string() });
            }
            let at = if forward.is_empty() { *head } else { 0 };
            for tag in pointed(&entry.signature) {
                let first = needed.entry(tag).or_insert(at);
                *first = (*first).min(at);
            }
            if told.insert(entry.name.as_str()) {
                lines.extend(forward.iter().cloned());
            }
        }
        // A tag a header in reach declares, or the file defines above its
        // first need, is known there already.
        let mut defined: BTreeMap<String, usize> = BTreeMap::new();
        for (tag, at, defines) in tag_places(text) {
            if defines {
                defined.entry(tag).or_insert(at);
            }
        }
        for (tag, at) in needed {
            if !declared.contains(&tag) && !defined.get(&tag).is_some_and(|start| *start < at) {
                tags.insert(tag);
            }
        }
        let made = apply(text, &edits);
        if lines.is_empty() && tags.is_empty() {
            return made;
        }
        let model = self.model;
        let exists = |path: &str| model.text.contains_key(path);
        let c_text = |name: &str| !name.ends_with(".h") || model.resolve(&exists, file, file, name).is_some_and(|found| model.is_fragment(&found));
        with_block(&made, &block(&tags, &lines), &c_text)
    }
}

/// A header's text without the prototypes of these functions, and
/// without the tags it declared for them alone: the way out for a header
/// the generator does not write as it stands.
pub(crate) fn without_prototypes(text: &str, names: &BTreeSet<&str>) -> String {
    let edits: Vec<Edit> = entries(text).iter().filter(|entry| entry.what == What::Prototype && names.contains(entry.name.as_str()) && entry.declarators == 1).map(|entry| removal(text, entry.start, entry.end)).collect();
    let made = apply(text, &edits);
    let count = |text: &str, tag: &str| places(text).into_iter().filter(|(_, word)| *word == tag).count();
    let mut edits = Vec::new();
    let mut at = 0;
    for line in made.split_inclusive('\n') {
        let declared = line.trim().strip_suffix(';').and_then(|rest| rest.strip_prefix("struct ").or_else(|| rest.strip_prefix("union ")));
        if let Some(tag) = declared.filter(|tag| !tag.is_empty() && tag.bytes().all(|b| b.is_ascii_alphanumeric() || b == b'_')) {
            if count(&made, tag) == 1 && count(text, tag) > 1 {
                edits.push(removal(&made, at, at + line.trim_end().len()));
            }
        }
        at += line.len();
    }
    apply(&made, &edits)
}

/// The parts of a unit's objects that differ otherwise than by a symbol's
/// scope: `static` makes a global symbol local and nothing else.
pub(crate) fn differing(before: &Built, now: &Built) -> BTreeSet<String> {
    let mut found = objects::differing(before, now);
    let scopeless = |built: &Built| -> Vec<Option<Vec<String>>> {
        built.iter().map(|edition| edition.as_ref().ok().and_then(|parts| parts.get("symbols")).map(|part| part.relocations.lines().map(|row| row.rsplit_once(' ').map_or(row, |(rest, _)| rest).to_string()).collect())).collect()
    };
    if scopeless(before) == scopeless(now) {
        found.remove("symbols");
    }
    found
}

/// The whole pass's rewrite: each changed file's text, each header that
/// goes, and what was decided of each function.
#[derive(Default)]
pub(crate) struct Rewrite {
    pub texts: BTreeMap<String, String>,
    pub gone: BTreeSet<String>,
    pub verdicts: BTreeMap<String, Vec<(String, Verdict)>>,
    /// The headers the generator does not write as they stand: these
    /// lose their prototypes line by line.
    pub unfaithful: Vec<String>,
    pub problems: Vec<String>,
}

/// The include line of a header on the search path.
fn include_line(name: &str) -> String {
    format!("#include \"{name}\"\n")
}

/// Whether a file's text reads a header, by its own lines or through the
/// headers they name. `text_of` gives each header's text as the pass
/// leaves it, none for one that goes.
fn reads<'a>(model: &Model, text_of: &dyn Fn(&str) -> Option<&'a str>, file: &str, text: &str, wanted: &str) -> bool {
    let exists = |path: &str| model.text.contains_key(path);
    let mut seen = BTreeSet::new();
    let mut queue: Vec<(String, String)> = include_names(text).into_iter().map(|name| (file.to_string(), name)).collect();
    while let Some((from, name)) = queue.pop() {
        let Some(found) = model.resolve(&exists, file, &from, &name) else {
            continue;
        };
        if model.is_fragment(&found) || !seen.insert(found.clone()) {
            continue;
        }
        let Some(text) = text_of(&found) else {
            continue;
        };
        if name == wanted || spelling(&found).as_deref() == Some(wanted) {
            return true;
        }
        queue.extend(include_names(text).into_iter().map(|name| (found.clone(), name)));
    }
    false
}

/// The rewrite of every overlay C file but those left as they are.
pub(crate) fn plan(scene: &Scene, leave: &BTreeSet<String>) -> Rewrite {
    let model = scene.model;
    let mut out = Rewrite::default();
    let files: Vec<&String> = model.units.iter().chain(&model.fragments).filter(|file| scene.overlay(file) && !leave.contains(*file)).collect();
    // The headers first: what each still declares decides the tags a
    // file's block has to declare.
    let mut headers: BTreeMap<String, Option<String>> = BTreeMap::new();
    let mut lost: BTreeMap<String, Vec<String>> = BTreeMap::new();
    for file in &files {
        let verdicts = scene.verdicts(file);
        let all = &model.entries[*file];
        let taken: BTreeSet<&str> = verdicts.iter().filter(|(name, verdict)| matches!(verdict, Verdict::Static(_)) && all.iter().any(|entry| entry.name == *name && !entry.local)).map(|(name, _)| name.as_str()).collect();
        if let (Some(path), false) = (unit_header(file), taken.is_empty()) {
            if let Some(existing) = model.text.get(&path) {
                let text = model.text[*file].as_str();
                // What the header could not say before is no news.
                let (mut known, mut problems) = (Vec::new(), Vec::new());
                let faithful = header(&path, text, Some(existing), all, scene.types, &mut known).is_some_and(|(written, _)| written == *existing);
                let left: Vec<Entry> = all.iter().filter(|entry| !(matches!(entry.what, What::Definition { .. }) && taken.contains(entry.name.as_str()))).cloned().collect();
                let new = if faithful {
                    let new = header(&path, text, Some(existing), &left, scene.types, &mut problems).map(|(written, _)| written);
                    out.problems.extend(problems.into_iter().filter(|problem| !known.contains(problem)));
                    new
                } else {
                    out.unfaithful.push(path.clone());
                    Some(without_prototypes(existing, &taken))
                };
                let kept = new.as_deref().map(include_names).unwrap_or_default();
                lost.insert(path.clone(), include_names(existing).into_iter().filter(|name| !kept.contains(name)).collect());
                headers.insert(path, new);
            }
        }
        out.verdicts.insert((*file).clone(), verdicts);
    }
    let text_of = |header: &str| -> Option<&str> {
        match headers.get(header) {
            Some(new) => new.as_deref(),
            None => model.text.get(header).map(String::as_str),
        }
    };
    for file in &files {
        let verdicts = &out.verdicts[*file];
        if !verdicts.iter().any(|(_, verdict)| matches!(verdict, Verdict::Static(_))) {
            continue;
        }
        // The tags a header has declared for every unit that compiles the
        // file.
        let mut declared: Option<BTreeSet<String>> = None;
        for unit in model.users(file) {
            let here: BTreeSet<String> = model.reach[unit].iter().filter(|reached| !model.is_fragment(reached)).filter_map(|reached| text_of(reached)).flat_map(tags).collect();
            declared = Some(match declared {
                Some(all) => all.intersection(&here).cloned().collect(),
                None => here,
            });
        }
        out.texts.insert((*file).clone(), scene.rewrite(file, verdicts, &declared.unwrap_or_default()));
    }
    // What a header no longer includes, each file that includes the
    // header includes for itself; a header that goes leaves no line.
    let mut readers: BTreeMap<String, Vec<&String>> = BTreeMap::new();
    for file in model.units.iter().chain(&model.fragments).chain(&model.headers) {
        for name in include_names(&model.text[file]) {
            readers.entry(name).or_default().push(file);
        }
    }
    for (path, names) in &lost {
        let gone = headers.get(path).is_some_and(|new| new.is_none());
        let Some(own) = spelling(path).filter(|_| gone || !names.is_empty()) else {
            continue;
        };
        for file in readers.get(&own).into_iter().flatten().filter(|file| **file != path) {
            let written = out.texts.get(*file).or_else(|| headers.get(*file).and_then(|new| new.as_ref()));
            let Some(text) = written.or_else(|| model.text.get(*file).filter(|_| !headers.contains_key(*file))) else {
                continue;
            };
            // The file as it reads without this header's own includes.
            let line = include_line(&own);
            let bare = if gone { text.replacen(&line, "", 1) } else { text.clone() };
            let after = |header: &str| -> Option<&str> {
                match headers.get(header) {
                    Some(new) => new.as_deref(),
                    None => model.text.get(header).map(String::as_str),
                }
            };
            let missing: String = names.iter().filter(|name| !reads(model, &after, file, &bare, name)).map(|name| include_line(name)).collect();
            let new = if gone { text.replacen(&line, &missing, 1) } else { text.replacen(&line, &format!("{line}{missing}"), 1) };
            if new == *text {
                continue;
            }
            if headers.contains_key(*file) {
                headers.insert((*file).clone(), Some(new));
            } else {
                out.texts.insert((*file).clone(), new);
            }
        }
    }
    for (path, new) in headers {
        match new {
            Some(text) => {
                out.texts.insert(path, text);
            }
            None => {
                out.gone.insert(path);
            }
        }
    }
    out
}

/// A reason without the names it gives: the kind of cause, for the count.
fn kind(why: &str) -> String {
    let games = |word: &str| word.starts_with("games/") || word.len() == 3 && word.bytes().all(|b| b.is_ascii_hexdigit());
    why.split(' ').map(|word| if games(word) || word.starts_with('`') || word.ends_with('`') { "…" } else { word }).collect::<Vec<_>>().join(" ")
}

pub(crate) fn run(root: &Path, dry: bool, mut leave: BTreeSet<String>, report: Option<&str>) -> Result<(), String> {
    let mut model = Model::read(root)?;
    // The tree as it stands; the pass reads each C file without its
    // block, which it writes anew.
    let tree = model.text.clone();
    for file in model.units.iter().chain(&model.fragments) {
        let bare = without_block(&model.text[file]);
        if bare != model.text[file] {
            model.entries.insert(file.clone(), entries(&bare));
            model.text.insert(file.clone(), bare);
        }
    }
    let overlays = Overlays::read(root, &model)?;
    let types = Types::read(&model);
    let scene = Scene::new(&model, &overlays, &types);
    let mut units: Vec<&String> = overlays.image.keys().filter(|unit| !model.arm.contains(*unit)).collect();
    units.sort_by_key(|unit| std::cmp::Reverse(tree[*unit].len()));
    let before = super::parallel(&units, || Bench::new(root), |bench, unit| bench.build(root, unit, &tree[*unit]))?;
    let made = loop {
        let made = plan(&scene, &leave);
        let now = super::parallel(&units, || Bench::new(root), |bench, unit| {
            let text = made.texts.get(*unit).unwrap_or(&tree[*unit]);
            let others: Vec<(String, String)> = model.reach[*unit].iter().filter_map(|file| made.texts.get(file).map(|text| (file.clone(), text.clone()))).collect();
            bench.build_among(root, unit, text, &others)
        })?;
        // A unit whose objects differ is left as it was: its own file
        // first, then the C files it includes.
        let mut more = BTreeSet::new();
        for ((unit, before), now) in units.iter().zip(&before).zip(&now) {
            let parts = differing(before, now);
            let Some(first) = parts.iter().find(|part| before.iter().flatten().any(|parts| parts.get(*part).is_some_and(|part| part.code))).or_else(|| parts.iter().next()) else {
                continue;
            };
            let how = match objects::difference(before, now, first) {
                Some(how) => how,
                None => now.iter().find_map(|edition| edition.as_ref().err()).map_or_else(|| "is not what it was".to_string(), |messages| format!("does not compile: {}", objects::first_error(messages))),
            };
            println!("differs: {unit}: {first} {how} ({} parts)", parts.len());
            let own = leave.contains(*unit);
            more.extend(model.parts(unit).filter(|part| (*part == unit.as_str()) != own).map(str::to_string));
        }
        if more.is_empty() {
            break made;
        }
        if more.iter().all(|file| leave.contains(file)) {
            return Err("units differ with every file they compile left as it was".into());
        }
        for file in &more {
            println!("left as it was: {file}");
        }
        leave.extend(more);
    };
    for problem in &made.problems {
        println!("problem: {problem}");
    }
    for header in &made.unfaithful {
        println!("line by line: {header}");
    }
    let count = |text: &str| entries(text).iter().filter(|entry| entry.what == What::Prototype && !entry.local).count();
    let (mut taken, mut forward, mut kept, mut prototypes) = (0, 0, 0, 0);
    let mut causes: BTreeMap<String, usize> = BTreeMap::new();
    let mut rows = String::from("file\tfunction\tverdict\tcause\n");
    for (file, verdicts) in &made.verdicts {
        let all = &model.entries[file];
        for (name, verdict) in verdicts {
            let was = all.iter().any(|entry| entry.name == *name && entry.local);
            match verdict {
                Verdict::Static(lines) => {
                    taken += usize::from(!was);
                    forward += usize::from(!lines.is_empty());
                    rows.push_str(&format!("{file}\t{name}\t{}\t{}\n", if was { "is static" } else { "static" }, if lines.is_empty() { "" } else { "declared in the block" }));
                }
                Verdict::Kept(why) => {
                    kept += 1;
                    *causes.entry(kind(why)).or_default() += 1;
                    rows.push_str(&format!("{file}\t{name}\tkept\t{why}\n"));
                }
            }
        }
    }
    // A file that differs from what the pass writes by its spacing alone
    // stays as it is: the formatter aligns a block's names its own way.
    let spaced = |text: &str| text.split_whitespace().collect::<Vec<_>>().join(" ");
    let mut changed = 0;
    for (file, text) in &made.texts {
        if tree.get(file).is_some_and(|old| old == text || spaced(old) == spaced(text)) {
            continue;
        }
        changed += 1;
        if file.ends_with(".h") {
            prototypes += count(tree.get(file).map_or("", String::as_str)).saturating_sub(count(text));
        }
        if !dry {
            super::write(root, file, text)?;
        }
    }
    for file in &made.gone {
        prototypes += count(tree.get(file).map_or("", String::as_str));
        if !dry {
            std::fs::remove_file(root.join(file)).map_err(|e| format!("{file}: {e}"))?;
        }
    }
    if let Some(path) = report {
        std::fs::write(path, &rows).map_err(|e| format!("{path}: {e}"))?;
    }
    for (cause, count) in &causes {
        println!("kept: {count}: {cause}");
    }
    println!(
        "{taken} functions {} static in {} files of {} overlay units ({forward} declared in a block); {kept} kept; {prototypes} prototypes out of the headers, {} headers gone; {changed} files {}",
        if dry { "would become" } else { "became" },
        made.verdicts.len(),
        units.len(),
        made.gone.len(),
        if dry { "would change" } else { "changed" },
    );
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn a_head_begins_after_a_macro_call_before_it() {
        let text = "// 判定\nconst struct P *\nGet(s32 a)\n{\n}\nMAKE(Other) void Run(void)\n{\n}\n";
        let all = entries(text);
        let found: Vec<(&str, Option<&str>)> = heads(text, &all).into_iter().map(|(index, head, brace)| (all[index].name.as_str(), head.map(|head| &text[head..brace]))).collect();
        assert_eq!(found, [("Get", Some("const struct P *\nGet(s32 a)\n")), ("Run", Some("void Run(void)\n"))]);
    }

    #[test]
    fn a_block_stands_after_the_leading_includes() {
        let text = "/* 村 */\n#include \"a.h\"\n\n#define NULL 0\n#include \"b.h\" // 型\n#if X\n#include \"c.h\"\n#endif\n\n// 共通\n#include \"f.c\"\n\nvoid Run(void)\n{\n}\n#include \"late.h\"\n";
        let named_c = |name: &str| !name.ends_with(".h");
        let at = block_place(text, &named_c);
        assert!(text[at..].starts_with("\n// 共通\n"));
        // An include of a header that defines code is code as well.
        let shared = text.replace("\"f.c\"", "\"common/f.h\"");
        assert_eq!(block_place(&shared, &|name| name == "common/f.h"), at);
        assert!(block_place(&shared, &named_c) > at);
        let lines = [(String::new(), "void A(void);".to_string()), ("X".to_string(), "s32 B(s32 a);".to_string()), ("!(X)".to_string(), "s32 B(void);".to_string())];
        let tags: BTreeSet<String> = ["struct P;".to_string()].into();
        let block = block(&tags, &lines);
        assert_eq!(block, format!("{FORWARD}\nstruct P;\nstatic void A(void);\n#if X\nstatic s32 B(s32 a);\n#endif\n#if !(X)\nstatic s32 B(void);\n#endif\n"));
        let made = with_block(text, &block, &named_c);
        assert_eq!(made, text.replacen("#endif\n\n", &format!("#endif\n\n{block}\n"), 1));
        assert_eq!(without_block(&made), text);
        // With no include the block stands before the first line of code.
        assert_eq!(with_block("// 部品\nvoid Run(void)\n{\n}\n", &block, &named_c), format!("// 部品\n\n{block}\nvoid Run(void)\n{{\n}}\n"));
        assert_eq!(with_block("#include \"a.h\"\nvoid Run(void);\n", "X\n", &named_c), "#include \"a.h\"\n\nX\n\nvoid Run(void);\n");
    }

    #[test]
    fn tags_are_read_from_signatures_and_headers() {
        assert_eq!(pointed("void F ( struct P * p , union Q q , const struct R * * r , s32 * n )"), ["struct P;", "struct R;"]);
        let text = "struct P;\nunion Q\n{\n\ts32 a;\n};\nvoid F(struct R *r);\ntypedef struct S { s32 a; } S;\n// struct T;\n";
        assert_eq!(tags(text).into_iter().collect::<Vec<_>>(), ["struct P;", "struct S;", "union Q;"]);
        assert_eq!(tag_places(text).into_iter().filter(|(_, _, defines)| *defines).map(|(tag, at, _)| (tag, &text[at..at + 5])).collect::<Vec<_>>(), [("union Q;".to_string(), "union"), ("struct S;".to_string(), "struc")]);
    }

    const A: &str = "games/tbs/src/o/a.c";
    const B: &str = "games/tbs/src/o/b.c";
    const F: &str = "games/tbs/src/o/f.c";
    const A_H: &str = "games/tbs/include/unit/o/a.h";
    const F_H: &str = "games/tbs/include/unit/o/f.h";
    const TYPES: &str = "typedef signed int s32;\ntypedef unsigned int u32;\n";

    fn files() -> Vec<(&'static str, &'static str)> {
        vec![
            (
                A,
                "#include \"unit/o/a.h\"\n#include \"unit/o/f.h\"\n#include \"shared.h\"\n\nstruct Local\n{\n\ts32 x;\n};\n\ns32 Entry(void)\n{\n\tLate(1);\n\tShared(0);\n\tHeld(0);\n\treturn Early(0);\n}\n\ns32 Early(struct Local *p)\n{\n\treturn 0;\n}\n\nvoid Late(s32 a)\n{\n\tImplicit();\n}\n\nvoid Implicit(void)\n{\n}\n\nvoid Other(struct Far *far, struct Local *p)\n{\n\tvoid Inner();\n\tInner();\n}\n\nvoid Inner(void)\n{\n}\n\n#include \"f.c\"\n\nvoid Typed(Own *own)\n{\n}\n\nvoid Caller(void)\n{\n\tTyped(0);\n\tPart(0);\n}\n\nconst u32 Table[] = { (u32)Other, (u32)Deep };\n",
            ),
            (F, "void Part(struct Far *far)\n{\n\tDeep();\n}\n\nvoid Deep(void)\n{\n}\n\nvoid Shared(s32 a)\n{\n}\n\nvoid Held(s32 a)\n{\n}\n"),
            (B, "#include \"unit/o/f.h\"\n\n#include \"f.c\"\n\nvoid Second(void)\n{\n\tPart(0);\n}\n"),
            (
                A_H,
                "#ifndef ALCHEMY_UNIT_O_A_H\n#define ALCHEMY_UNIT_O_A_H\n\n#include \"types.h\"\n\nstruct Far;\nstruct Local;\n\ns32 Entry(void);\ns32 Early(struct Local *p);\nvoid Late(s32 a);\n/* Implicit is called before its definition with no prototype in scope. */\nvoid Other(struct Far *far, struct Local *p);\nvoid Inner(void);\nvoid Caller(void);\nextern const u32 Table[];\n\n#endif\n",
            ),
            (F_H, "#ifndef ALCHEMY_UNIT_O_F_H\n#define ALCHEMY_UNIT_O_F_H\n\n#include \"types.h\"\n\nstruct Far;\n\nvoid Part(struct Far *far);\nvoid Deep(void);\nvoid Shared(s32 a);\nvoid Held(s32 a);\n\n#endif\n"),
            ("games/tbs/include/shared.h", "#define HOLD(x) Held(x)\n"),
            ("games/tbs/include/types.h", TYPES),
        ]
    }

    fn model(files: &[(&str, &str)]) -> (Model, Overlays, Types) {
        let reach: &[&str] = &[F, A_H, F_H, "games/tbs/include/shared.h", "games/tbs/include/types.h"];
        let model = Model::of(files, &[(A, reach), (B, &[F, F_H, "games/tbs/include/types.h"])]);
        let mut overlays = Overlays::default();
        overlays.image.insert(A.to_string(), vec!["3a0".to_string()]);
        overlays.image.insert(B.to_string(), vec!["3a1".to_string()]);
        overlays.named.insert("3a0".to_string(), ["Entry".to_string()].into());
        overlays.named.insert("3a1".to_string(), ["Second".to_string(), "Deep".to_string()].into());
        let mut types = Types::default();
        types.add("games/tbs/include/types.h", TYPES);
        types.add(A, "typedef struct Own Own;\n");
        (model, overlays, types)
    }

    fn said(scene: &Scene, file: &str) -> Vec<(String, String)> {
        scene
            .verdicts(file)
            .into_iter()
            .map(|(name, verdict)| match verdict {
                Verdict::Static(lines) => (name, lines.into_iter().map(|(_, line)| line).collect::<Vec<_>>().join(" ")),
                Verdict::Kept(why) => (name, format!("kept: {why}")),
            })
            .collect()
    }

    fn pairs(rows: &[(&str, &str)]) -> Vec<(String, String)> {
        rows.iter().map(|(name, what)| (name.to_string(), what.to_string())).collect()
    }

    #[test]
    fn a_function_only_its_own_file_names_is_static() {
        let files = files();
        let (model, overlays, types) = model(&files);
        let scene = Scene::new(&model, &overlays, &types);
        assert!(scene.overlay(A) && scene.overlay(F));
        assert_eq!(
            said(&scene, A),
            pairs(&[
                ("Entry", "kept: another object of overlay 3a0 names it"),
                ("Early", "s32 Early(struct Local *p);"),
                ("Late", "void Late(s32 a);"),
                ("Implicit", "kept: it is called before its definition with no prototype in scope"),
                ("Other", ""),
                ("Inner", "kept: a function of games/tbs/src/o/a.c declares it for itself"),
                ("Typed", ""),
                ("Caller", ""),
            ])
        );
        // Deep is named by the other overlay's entry table, Shared above
        // the line that includes the file, and Held by a header's macro.
        assert_eq!(
            said(&scene, F),
            pairs(&[
                ("Part", ""),
                ("Deep", "kept: another object of overlay 3a1 names it"),
                ("Shared", "kept: games/tbs/src/o/a.c names it above the line that includes its file"),
                ("Held", "kept: games/tbs/include/shared.h names it"),
            ])
        );
        let made = plan(&scene, &BTreeSet::new());
        let text = &made.texts[A];
        // No header declares struct Far any more: a.c's block does, for
        // the head that points to it. struct Local is defined above the
        // heads that point to it and below the block's prototype.
        assert!(text.contains(&format!("#include \"shared.h\"\n\n{FORWARD}\nstruct Far;\nstruct Local;\nstatic s32 Early(struct Local *p);\nstatic void Late(s32 a);\n\nstruct Local\n")), "{text}");
        for head in ["\nstatic s32 Early(struct Local *p)\n{", "\nstatic void Other(struct Far *far, struct Local *p)\n{", "\nvoid Implicit(void)\n{", "\ns32 Entry(void)\n{", "\nstatic void Typed(Own *own)\n{"] {
            assert!(text.contains(head), "{head}");
        }
        assert_eq!(made.texts[F], format!("{FORWARD}\nstruct Far;\n\n{}", files[1].1.replacen("void Part(", "static void Part(", 1)));
        assert_eq!(
            made.texts[A_H],
            "#ifndef ALCHEMY_UNIT_O_A_H\n#define ALCHEMY_UNIT_O_A_H\n\n#include \"types.h\"\n\ns32 Entry(void);\n/* Implicit is called before its definition with no prototype in scope. */\nvoid Inner(void);\nextern const u32 Table[];\n\n#endif\n"
        );
        assert_eq!(made.texts[F_H], files[4].1.replacen("struct Far;\n\nvoid Part(struct Far *far);\n", "", 1));
        assert!(made.gone.is_empty() && made.unfaithful.is_empty() && !made.texts.contains_key(B));
        // A file left as it is keeps its header whole.
        let left = plan(&scene, &[A.to_string()].into());
        assert!(!left.texts.contains_key(A) && !left.texts.contains_key(A_H) && left.texts.contains_key(F));
    }

    #[test]
    fn what_the_pass_wrote_it_writes_again() {
        let files = files();
        let (first, overlays, types) = model(&files);
        let made = plan(&Scene::new(&first, &overlays, &types), &BTreeSet::new());
        // The second pass reads the files the first wrote, each C file
        // without its block.
        let bare: Vec<(String, String)> = files.iter().map(|(file, text)| (file.to_string(), without_block(made.texts.get(*file).map_or(text, String::as_str)))).collect();
        let again: Vec<(&str, &str)> = bare.iter().map(|(file, text)| (file.as_str(), text.as_str())).collect();
        let (second, overlays, types) = model(&again);
        let scene = Scene::new(&second, &overlays, &types);
        assert_eq!(said(&scene, A)[1], ("Early".to_string(), "s32 Early(struct Local *p);".to_string()));
        let rewritten = plan(&scene, &BTreeSet::new());
        for file in [A, F, A_H, F_H] {
            assert_eq!(rewritten.texts.get(file).or(made.texts.get(file)), made.texts.get(file), "{file}");
        }
    }

    #[test]
    fn a_header_with_nothing_left_goes_with_its_include_lines() {
        let mut files = files();
        files[1].1 = "void Part(struct Far *far)\n{\n}\n";
        files[4].1 = "#ifndef ALCHEMY_UNIT_O_F_H\n#define ALCHEMY_UNIT_O_F_H\n\n#include \"types.h\"\n#include \"objdisp.h\"\n\nstruct Far;\n\nvoid Part(struct Far *far, Handle h);\n\n#endif\n";
        files[1].1 = "void Part(struct Far *far, Handle h)\n{\n}\n";
        let (model, overlays, mut types) = model(&files);
        types.add("games/common/include/field/objdisp.h", "typedef struct Q Handle;\n");
        let made = plan(&Scene::new(&model, &overlays, &types), &BTreeSet::new());
        assert_eq!(made.gone.iter().collect::<Vec<_>>(), [F_H]);
        // Each file that read the header reads what the header read for it.
        // a.c reads types.h through its own header still; b.c read it
        // through this one alone.
        assert!(made.texts[A].starts_with("#include \"unit/o/a.h\"\n#include \"objdisp.h\"\n#include \"shared.h\"\n"), "{}", made.texts[A]);
        assert!(made.texts[B].starts_with("#include \"types.h\"\n#include \"objdisp.h\"\n\n#include \"f.c\"\n"), "{}", made.texts[B]);
    }

    #[test]
    fn a_static_function_changes_a_symbols_scope_alone() {
        use objects::Part;
        let built = |scope: &str, byte: u8| -> Built {
            let symbols = Part { relocations: format!("Run .text 4 Text {scope}\nTable .data 8 Data Linkage"), ..Part::default() };
            vec![Ok([("Run".to_string(), Part { bytes: vec![byte], ..Part::default() }), ("symbols".to_string(), symbols)].into())]
        };
        assert!(differing(&built("Linkage", 1), &built("Compilation", 1)).is_empty());
        assert_eq!(differing(&built("Linkage", 1), &built("Compilation", 2)).into_iter().collect::<Vec<_>>(), ["Run"]);
    }

    #[test]
    fn a_header_loses_the_prototypes_of_what_became_static() {
        let text = "#ifndef A_H\n#define A_H\n\nstruct Far;\nstruct Near;\nstruct Stale;\n\nvoid Run(struct Near *near);\ns32 Get(struct Far *far, struct Near *near);\nextern u8 gTable[];\n\n#endif\n";
        // The tag no line named before stays: it is not this pass's.
        assert_eq!(without_prototypes(text, &["Get"].into()), text.replacen("s32 Get(struct Far *far, struct Near *near);\n", "", 1).replacen("struct Far;\n", "", 1));
    }

    #[test]
    fn a_reason_is_counted_by_its_kind() {
        assert_eq!(kind("another object of overlay 3a0 names it"), "another object of overlay … names it");
        assert_eq!(kind("games/tbs/include/shared.h names it"), "… names it");
    }
}
