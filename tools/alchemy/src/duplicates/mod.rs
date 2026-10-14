//! `alchemy duplicates`: the functions ☀️'s C writes out more than once.
//! Reads the C units the object list links, with the C files they include,
//! and the objects the build made in each edition; changes nothing.

mod code;
mod text;

use crate::c_items::{flat, Kind};
use crate::object_list::LANGUAGES;
use crate::targets::{target_for, DecompTargetId};
use code::Code;
use std::collections::{BTreeMap, BTreeSet, HashMap};
use std::fmt::Write as _;
use std::fs;
use std::path::{Path, PathBuf};
use text::Text;

const USAGE: &str = "usage: alchemy duplicates [--out-dir DIR] [--near-lines N]
Reports the functions ☀️'s C source writes out more than once. Reads the C
units games/tbs/objects.tsv links, each with the C files it includes at the
place it includes them, and the objects the build made of every row
(out/tbs-<language>/obj); build the six editions first. Changes nothing.
A function's code is its bytes with every relocated field cleared and what
each relocation names; two names are the same when they are spelled the
same, or when the functions they name in their own images are the same
code (a far-call veneer two overlays name differently). A function's text
is its tokens without comments, its own name and the names it declares
(parameters, locals, labels) by their order of appearance.
Functions are grouped when they are the same code:
  EXACT  the same code in every edition that links both, and the same text:
         these can be written once as they are
  SAME   the same code, written differently: one text has to be chosen
  NEAR   the same bytes apart from what the relocations name, or the same
         text apart from one or two words (a constant, a called function, a
         table); only functions of N lines or more (default 8). These need
         a parameter: a macro argument or a #define before the include
A run is a row of groups that stand together in every unit, in one order
and with the same text between them: one shared file. Writes into DIR
(default out/reports/duplicates):
  groups.tsv   one row for each written copy, with its group and its run
  trivial.tsv  the same for empty bodies and single returns, outside the totals
  names.tsv    the same code under several names, in any object the build
               links: each name, its functions, their bytes and what they name
  summary.md   totals, the largest runs and what stands out
Columns: group, kind, run, place (the group's place in its run), of (the
run's groups), name, file, line, lines, bytes (the English build's), images
(the overlays or main that link it), units (how many compile this text),
shared (yes when the file is already compiled by several units), text and
code (which of the group's texts and codes this copy is), differs (its
words where the copies differ), renames (its names for code the copies
name differently), saves (the lines that go when the group is written
once; a copy keeps a line for each word it gives), names (same or differ
over the group, or apart for a copy whose image links another copy of the
group, so that their names have to stay apart), fragment (the shared files
that hold the group's code).";

/// One function definition as a file writes it.
#[derive(Clone, Debug, Default)]
struct Definition {
    file: String,
    line: usize,
    lines: usize,
    /// The `#if` conditions it stands under.
    conditions: String,
    text: Text,
}

/// One function of one built object.
#[derive(Clone, Debug, Default)]
struct Function {
    name: String,
    /// Its code in each edition; `None` where the edition does not have it.
    code: Vec<Option<Code>>,
    /// The functions its relocations name, by the name they use: one of
    /// its own object, else one its image links.
    names: BTreeMap<String, usize>,
}

/// One function a unit's text defines and its object holds.
#[derive(Clone, Debug, Default)]
struct Written {
    unit: usize,
    function: usize,
    /// Its definitions in the unit's text; more than one are the arms of a
    /// conditional.
    definitions: Vec<usize>,
    /// The unit's text between the function before it and this one, less
    /// comments and includes.
    gap: String,
}

struct Unit {
    file: String,
    images: BTreeSet<String>,
}

#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord)]
enum Sameness {
    Exact,
    Same,
    Near,
}

impl Sameness {
    fn label(self) -> &'static str {
        match self {
            Sameness::Exact => "EXACT",
            Sameness::Same => "SAME",
            Sameness::Near => "NEAR",
        }
    }
}

#[derive(Clone, Debug)]
struct Group {
    kind: Sameness,
    copies: Vec<usize>,
    /// For each copy, which of the group's texts and which of its codes it is.
    text: Vec<usize>,
    code: Vec<usize>,
    /// For each copy, the words it has where the group's copies differ, and
    /// its names for the code they name differently.
    differs: Vec<String>,
    renames: Vec<String>,
    /// How many words a shared text would take from each unit.
    parameters: usize,
    /// For each copy, whether an image links it and another of the copies.
    twice: Vec<bool>,
    /// The run it stands in, its place there and how many groups the run has.
    run: usize,
    place: usize,
    of: usize,
}

// ---------------------------------------------------------------------------
// Reading

enum Piece {
    Function(usize),
    Include(String),
    Other(String),
}

#[derive(Default)]
struct Sources {
    files: BTreeMap<String, Vec<Piece>>,
    definitions: Vec<Definition>,
    /// Included C files that are not where the include line says.
    missing: BTreeSet<String>,
    /// Function items whose head could not be read.
    unread: Vec<String>,
}

fn normalize(path: &Path) -> String {
    let mut parts: Vec<&str> = Vec::new();
    for part in path.to_str().unwrap_or("").split('/') {
        match part {
            "" | "." => {}
            ".." => {
                parts.pop();
            }
            part => parts.push(part),
        }
    }
    parts.join("/")
}

/// A function's text with each file its body includes in the include's
/// place: found beside `file`, else among the game's headers.
fn inlined(root: &Path, file: &str, text: &str, depth: usize) -> String {
    let mut out = String::new();
    for line in text.split_inclusive('\n') {
        let directive = line.trim_start().strip_prefix('#').map(str::trim_start).and_then(|rest| rest.strip_prefix("include"));
        let name = directive.and_then(|rest| rest.trim_start().strip_prefix('"')).and_then(|rest| rest.split('"').next());
        let beside = Path::new(file).parent().unwrap_or(Path::new("")).to_path_buf();
        let places = [beside, PathBuf::from("games/tbs/include"), PathBuf::from("games/common/include")];
        let found = name.filter(|_| depth < 4).and_then(|name| places.iter().map(|place| normalize(&place.join(name))).find(|path| root.join(path).is_file()));
        match found.and_then(|path| fs::read(root.join(&path)).ok().map(|bytes| (path, bytes))) {
            Some((path, bytes)) => {
                out.push_str(&inlined(root, &path, &String::from_utf8_lossy(&bytes), depth + 1));
                out.push('\n');
            }
            None => out.push_str(line),
        }
    }
    out
}

impl Sources {
    /// A C file's pieces in order: its function definitions, the C files
    /// it includes and everything else that is not a comment or a header.
    fn pieces(&mut self, root: &Path, file: &str, source: &str) -> Vec<Piece> {
        let mut pieces = Vec::new();
        for flat in flat(source) {
            let item = &flat.item;
            let line = || source[..item.start].matches('\n').count() + 1;
            match &item.kind {
                Kind::Comment => {}
                Kind::Include(name) if crate::c_items::c_text_file(&root.join(normalize(&Path::new(file).parent().unwrap_or(Path::new("")).join(name)))) => {
                    let directory = Path::new(file).parent().unwrap_or(Path::new(""));
                    pieces.push(Piece::Include(normalize(&directory.join(name))));
                }
                Kind::Include(_) => {}
                Kind::Function { .. } => match text::read(&inlined(root, file, item.text, 0)) {
                    Some(text) => {
                        let lines = item.text.trim_end().matches('\n').count() + 1;
                        pieces.push(Piece::Function(self.definitions.len()));
                        self.definitions.push(Definition { file: file.to_string(), line: line(), lines, conditions: flat.conditions.join(" && "), text });
                    }
                    None => {
                        self.unread.push(format!("{file}:{}", line()));
                        pieces.push(Piece::Other(text::plain(item.text)));
                    }
                },
                _ => pieces.push(Piece::Other(text::plain(item.text))),
            }
        }
        pieces
    }

    /// A unit's definitions in the order of its text, each with the text
    /// that stands before it since the last one.
    fn expand(&mut self, root: &Path, file: &str, found: &mut Vec<(usize, String)>, gap: &mut String, depth: usize) -> Result<(), String> {
        if !self.files.contains_key(file) {
            let source = fs::read(root.join(file)).map_err(|e| format!("{file}: {e}"))?;
            let pieces = self.pieces(root, file, &String::from_utf8_lossy(&source));
            self.files.insert(file.to_string(), pieces);
        }
        for index in 0..self.files[file].len() {
            match &self.files[file][index] {
                Piece::Function(definition) => found.push((*definition, std::mem::take(gap))),
                Piece::Other(text) => {
                    gap.push_str(text);
                    gap.push('\n');
                }
                Piece::Include(included) => {
                    let included = included.clone();
                    if depth < 8 && root.join(&included).is_file() {
                        self.expand(root, &included, found, gap, depth + 1)?;
                    } else {
                        self.missing.insert(format!("{file}: {included}"));
                    }
                }
            }
        }
        Ok(())
    }
}

/// A unit's copies in the order of its text: each name its object holds,
/// with the definitions of that name. `compiled` is the object's functions.
fn copies(unit: usize, definitions: &[Definition], text: &[(usize, String)], compiled: &HashMap<&str, usize>) -> Vec<Written> {
    let mut found: Vec<Written> = Vec::new();
    let mut at: HashMap<&str, usize> = HashMap::new();
    let mut gap = String::new();
    for (definition, before) in text {
        let name = definitions[*definition].text.name.as_str();
        gap.push_str(before);
        match (at.get(name), compiled.get(name)) {
            (Some(copy), _) => found[*copy].definitions.push(*definition),
            (None, Some(function)) => {
                at.insert(name, found.len());
                found.push(Written { unit, function: *function, definitions: vec![*definition], gap: std::mem::take(&mut gap) });
            }
            // A definition no object holds is text between its neighbours.
            (None, None) => {
                gap.push_str(&definitions[*definition].text.tokens.join(" "));
                gap.push('\n');
            }
        }
    }
    found
}

struct Read {
    units: Vec<Unit>,
    definitions: Vec<Definition>,
    functions: Vec<Function>,
    copies: Vec<Written>,
    notes: Vec<String>,
}

fn read(root: &Path) -> Result<Read, String> {
    let game = target_for(DecompTargetId::TbsJa);
    let rows = crate::object_list::read(root, game)?.ok_or("☀️ has no object list")?;
    // Every object the build makes, with the images and languages that link it.
    let mut objects: Vec<(&str, BTreeSet<String>, BTreeSet<&str>)> = Vec::new();
    for row in rows.iter().filter(|row| !row.object.contains([':', '*'])) {
        let at = objects.iter().position(|(object, _, _)| *object == row.object).unwrap_or_else(|| {
            objects.push((&row.object, BTreeSet::new(), BTreeSet::new()));
            objects.len() - 1
        });
        objects[at].1.insert(row.image.clone());
        objects[at].2.extend(row.editions.iter().copied());
    }
    let mut functions: Vec<Function> = Vec::new();
    let mut own: Vec<HashMap<String, usize>> = Vec::new();
    let mut linked: HashMap<(&str, String), Vec<usize>> = HashMap::new();
    for (object, images, languages) in &objects {
        let unit = root.join(format!("{object}.c")).is_file();
        let mut held: HashMap<String, usize> = HashMap::new();
        for (edition, (target, language)) in game.editions().iter().zip(LANGUAGES).enumerate() {
            let path = root.join(target.output_dir).join("obj").join(format!("{object}.o"));
            let bytes = match fs::read(&path) {
                Ok(bytes) if languages.contains(language) => bytes,
                Err(e) if unit && languages.contains(language) => return Err(format!("{}: {e}; build the six editions first", path.display())),
                _ => continue,
            };
            for function in code::functions(&bytes).map_err(|e| format!("{}: {e}", path.display()))? {
                let at = *held.entry(function.name.clone()).or_insert_with(|| {
                    functions.push(Function { name: function.name.clone(), code: vec![None; LANGUAGES.len()], names: BTreeMap::new() });
                    if function.global {
                        for image in images {
                            linked.entry((image.as_str(), function.name.clone())).or_default().push(functions.len() - 1);
                        }
                    }
                    functions.len() - 1
                });
                functions[at].code[edition] = Some(function.code);
            }
        }
        own.push(held);
    }
    // What each name a function's relocations use means where it is linked.
    for (index, (_, images, _)) in objects.iter().enumerate() {
        for function in own[index].values() {
            let used: BTreeSet<String> = functions[*function].code.iter().flatten().flat_map(|code| code.targets.iter().cloned()).collect();
            for name in used {
                let meant = own[index].get(&name).copied().or_else(|| {
                    let found: BTreeSet<Option<&Vec<usize>>> = images.iter().map(|image| linked.get(&(image.as_str(), name.clone()))).collect();
                    (found.len() == 1).then(|| found.into_iter().next().flatten()).flatten().filter(|of| of.len() == 1).map(|of| of[0])
                });
                if let Some(meant) = meant {
                    functions[*function].names.insert(name, meant);
                }
            }
        }
    }
    let mut sources = Sources::default();
    let (mut units, mut all, mut notes) = (Vec::new(), Vec::new(), Vec::new());
    let (mut unwritten, mut uncompiled) = (0usize, 0usize);
    for (index, (object, images, _)) in objects.iter().enumerate() {
        let file = format!("{object}.c");
        if !root.join(&file).is_file() {
            continue;
        }
        let mut text = Vec::new();
        sources.expand(root, &file, &mut text, &mut String::new(), 0)?;
        let compiled: HashMap<&str, usize> = own[index].iter().map(|(name, function)| (name.as_str(), *function)).collect();
        let written: BTreeSet<&str> = text.iter().map(|(definition, _)| sources.definitions[*definition].text.name.as_str()).collect();
        unwritten += compiled.keys().filter(|name| !written.contains(*name)).count();
        uncompiled += written.iter().filter(|name| !compiled.contains_key(*name)).count();
        all.extend(copies(units.len(), &sources.definitions, &text, &compiled));
        units.push(Unit { file, images: images.clone() });
    }
    if unwritten + uncompiled > 0 {
        notes.push(format!("{unwritten} functions in the units' objects have no definition of their name in their unit's text (a macro defines them, or they are nested), and {uncompiled} definitions have no function in any edition's object (inline, or under a condition no edition takes); neither is compared"));
    }
    if !sources.missing.is_empty() {
        notes.push(format!("included C files not found: {}", sources.missing.iter().cloned().collect::<Vec<_>>().join(", ")));
    }
    if !sources.unread.is_empty() {
        notes.push(format!("function definitions whose head could not be read: {}", sources.unread.join(", ")));
    }
    Ok(Read { units, definitions: sources.definitions, functions, copies: all, notes })
}

// ---------------------------------------------------------------------------
// Grouping

struct Options {
    out_dir: PathBuf,
    /// The fewest lines of a function that is grouped as near another.
    near_lines: usize,
    /// The most words two near texts differ in.
    near_words: usize,
}

fn intern<T: std::hash::Hash + Eq>(known: &mut HashMap<T, usize>, value: T) -> usize {
    let next = known.len();
    *known.entry(value).or_insert(next)
}

/// Each function's code in each edition as the number of what `part` takes of it.
fn keys<'a, T: std::hash::Hash + Eq>(functions: impl Iterator<Item = &'a Function>, part: impl Fn(&'a Code) -> T) -> Vec<Vec<Option<usize>>> {
    let mut known = HashMap::new();
    functions.map(|function| function.code.iter().map(|code| code.as_ref().map(|code| intern(&mut known, part(code)))).collect()).collect()
}

/// Classes of items whose keys agree in every edition both have, and that
/// have one in common: `keys[item][edition]`.
fn classes(keys: &[Vec<Option<usize>>]) -> Vec<usize> {
    let mut class = Vec::with_capacity(keys.len());
    let mut first: Vec<usize> = Vec::new();
    let mut holding: HashMap<(usize, usize), Vec<usize>> = HashMap::new();
    for (item, key) in keys.iter().enumerate() {
        let agrees = |other: &[Option<usize>]| {
            let both = || key.iter().zip(other).filter_map(|(a, b)| a.zip(*b));
            both().next().is_some() && both().all(|(a, b)| a == b)
        };
        let present = || key.iter().enumerate().filter_map(|(edition, key)| key.map(|key| (edition, key)));
        let known = present().filter_map(|at| holding.get(&at)).flatten().copied().find(|class| agrees(&keys[first[*class]]));
        class.push(known.unwrap_or_else(|| {
            first.push(item);
            for at in present() {
                holding.entry(at).or_default().push(first.len() - 1);
            }
            first.len() - 1
        }));
    }
    class
}

/// Disjoint sets, each known by its smallest member.
struct Sets(Vec<usize>);

impl Sets {
    fn find(&mut self, item: usize) -> usize {
        let mut root = item;
        while self.0[root] != root {
            root = self.0[root];
        }
        let mut at = item;
        while self.0[at] != root {
            at = std::mem::replace(&mut self.0[at], root);
        }
        root
    }

    /// Whether the two were apart.
    fn join(&mut self, a: usize, b: usize) -> bool {
        let (a, b) = (self.find(a), self.find(b));
        self.0[a.max(b)] = a.min(b);
        a != b
    }

    /// Joins the items that have the same key.
    fn join_by(&mut self, key: impl Fn(usize) -> Option<usize>) {
        let mut held: HashMap<usize, usize> = HashMap::new();
        for item in 0..self.0.len() {
            if let Some(key) = key(item) {
                self.join(*held.entry(key).or_insert(item), item);
            }
        }
    }
}

/// What a function's relocations name in each edition: each name, and the
/// set of the function it names there.
type Named<'a> = Vec<Option<Vec<(&'a str, Option<usize>)>>>;

/// The functions that are the same code, each by the first of them: the
/// same bytes and relocated fields in every edition both have, and every
/// relocation naming the same thing. Two names are the same when they are
/// spelled the same, or when the functions they name are the same code:
/// found from the leaves up, until nothing more joins.
fn same(functions: &[Function]) -> Vec<usize> {
    let apart = classes(&keys(functions.iter(), |code| (&code.bytes, &code.fields)));
    let mut alike: BTreeMap<usize, Vec<usize>> = BTreeMap::new();
    for (function, class) in apart.iter().enumerate() {
        alike.entry(*class).or_default().push(function);
    }
    alike.retain(|_, of| of.len() > 1);
    let mut sets = Sets((0..functions.len()).collect());
    loop {
        let mut joined = false;
        for of in alike.values() {
            let mut named: HashMap<Named, usize> = HashMap::new();
            for function in of {
                let meant = |edition: usize, name: &String| functions[*function].names.get(name).filter(|other| functions[**other].code[edition].is_some()).copied();
                let names = functions[*function].code.iter().enumerate().map(|(edition, code)| code.as_ref().map(|code| code.targets.iter().map(|name| (name.as_str(), meant(edition, name).map(|other| sets.find(other)))).collect()));
                let first = *named.entry(names.collect()).or_insert(*function);
                joined |= sets.join(first, *function);
            }
            let named: Vec<_> = named.into_iter().collect();
            for (at, (names, function)) in named.iter().enumerate() {
                for (others, other) in &named[..at] {
                    let agree = names.iter().zip(others).all(|pair| match pair {
                        (Some(a), Some(b)) => a.len() == b.len() && a.iter().zip(b).all(|(a, b)| a.0 == b.0 || (a.1.is_some() && a.1 == b.1)),
                        _ => true,
                    });
                    joined |= agree && sets.join(*function, *other);
                }
            }
        }
        if !joined {
            return (0..functions.len()).map(|function| sets.find(function)).collect();
        }
    }
}

/// A word as a text writes it and as what it means: a function's name
/// stands for its code, which another text may call by another name.
type Word<'a> = (&'a str, &'a str);

/// Where rows differ, as written: for each distinct way of differing,
/// each row's word there, in the order of the first place it shows. First
/// the places where the rows mean different things, then the places where
/// they only name the same thing differently.
fn columns(rows: &[Vec<Word>]) -> (Vec<Vec<String>>, Vec<Vec<String>>) {
    let (mut differs, mut renames): (Vec<Vec<String>>, Vec<Vec<String>>) = (Vec::new(), Vec::new());
    for place in 0..rows.iter().map(Vec::len).min().unwrap_or(0) {
        if rows.iter().all(|row| row[place].0 == rows[0][place].0) {
            continue;
        }
        let found = if rows.iter().all(|row| row[place].1 == rows[0][place].1) { &mut renames } else { &mut differs };
        let column: Vec<String> = rows.iter().map(|row| row[place].0.to_string()).collect();
        if !found.contains(&column) {
            found.push(column);
        }
    }
    (differs, renames)
}

/// What the words of a text mean: the same code under several names goes
/// by the first of them.
struct Meaning<'a> {
    same: &'a [usize],
    called: HashMap<usize, &'a str>,
}

impl<'a> Meaning<'a> {
    /// A row of the words of `function`'s text, or of what its relocations name.
    fn of(&self, function: &'a Function, row: Vec<&'a str>) -> Vec<Word<'a>> {
        row.into_iter().map(|word| (word, function.names.get(word).map_or(word, |other| self.called[&self.same[*other]]))).collect()
    }
}

/// Each item's number among the distinct values, from 1 in the order they show.
fn numbered(values: impl Iterator<Item = usize>) -> Vec<usize> {
    let mut known = HashMap::new();
    values.map(|value| intern(&mut known, value) + 1).collect()
}

fn group(units: &[Unit], functions: &[Function], same: &[usize], copies: &[Written], definitions: &[Definition], options: &Options) -> (Vec<Group>, usize) {
    // The same code under several names goes by the first of them.
    let mut called: HashMap<usize, &str> = HashMap::new();
    for (function, set) in same.iter().enumerate() {
        let name = called.entry(*set).or_insert(&functions[function].name);
        *name = (*name).min(&functions[function].name);
    }
    let meaning = Meaning { same, called };
    let function = |copy: usize| &functions[copies[copy].function];
    let first = |copy: usize| &definitions[copies[copy].definitions[0]];
    // A copy's text is every definition of its name with its condition.
    let written = |copy: usize| -> Vec<&str> {
        let parts = copies[copy].definitions.iter().map(|d| &definitions[*d]);
        parts.flat_map(|d| std::iter::once(d.conditions.as_str()).chain(d.text.tokens.iter().map(String::as_str))).collect()
    };
    let words = |copy: usize| meaning.of(function(copy), written(copy));
    let targets = |copy: usize| meaning.of(function(copy), function(copy).code.iter().flatten().next().map_or(Vec::new(), |code| code.targets.iter().map(String::as_str).collect()));
    let exact: Vec<usize> = copies.iter().map(|copy| same[copy.function]).collect();
    let apart = classes(&keys(copies.iter().map(|copy| &functions[copy.function]), |code| (&code.bytes, &code.fields)));
    let (mut texts, mut shapes) = (HashMap::new(), HashMap::new());
    let text: Vec<usize> = (0..copies.len()).map(|copy| intern(&mut texts, written(copy))).collect();
    let shape: Vec<usize> = copies.iter().map(|copy| intern(&mut shapes, copy.definitions.iter().map(|d| (&definitions[*d].conditions, definitions[*d].text.shape())).collect::<Vec<_>>())).collect();

    let mut sets = Sets((0..copies.len()).collect());
    sets.join_by(|copy| Some(exact[copy]));
    // Near: the same bytes apart from what the relocations name, or the
    // same shape of text with few words apart.
    let near = |copy: usize| first(copy).lines >= options.near_lines && !first(copy).text.trivial;
    sets.join_by(|copy| near(copy).then_some(apart[copy]));
    let mut shaped: BTreeMap<usize, Vec<usize>> = BTreeMap::new();
    for copy in (0..copies.len()).filter(|copy| near(*copy)) {
        let of_shape = shaped.entry(shape[copy]).or_default();
        if !of_shape.iter().any(|other| text[*other] == text[copy]) {
            of_shape.push(copy);
        }
    }
    let mut cluster_of: HashMap<usize, usize> = HashMap::new();
    for of_shape in shaped.values().filter(|of_shape| of_shape.len() > 1) {
        let rows: Vec<Vec<Word>> = of_shape.iter().map(|copy| words(*copy)).collect();
        let mut clusters: Vec<Vec<usize>> = Vec::new();
        for row in 0..rows.len() {
            let fits = |cluster: &Vec<usize>| columns(&cluster.iter().chain([&row]).map(|r| rows[*r].clone()).collect::<Vec<_>>()).0.len() <= options.near_words;
            match clusters.iter().position(fits) {
                Some(cluster) => clusters[cluster].push(row),
                None => clusters.push(vec![row]),
            }
        }
        for cluster in clusters.iter().filter(|cluster| cluster.len() > 1) {
            cluster_of.extend(cluster.iter().map(|row| (text[of_shape[*row]], of_shape[cluster[0]])));
        }
    }
    sets.join_by(|copy| near(copy).then(|| cluster_of.get(&text[copy]).copied()).flatten());

    let mut members: BTreeMap<usize, Vec<usize>> = BTreeMap::new();
    for copy in 0..copies.len() {
        members.entry(sets.find(copy)).or_default().push(copy);
    }
    let mut groups = Vec::new();
    for members in members.into_values() {
        let distinct = |of: &dyn Fn(usize) -> usize| members.iter().map(|copy| of(*copy)).collect::<BTreeSet<_>>().len();
        if distinct(&|copy| copies[copy].definitions[0]) < 2 {
            continue;
        }
        // Two texts are the same when no word of them means another thing.
        let all: Vec<Vec<Word>> = members.iter().map(|copy| words(*copy)).collect();
        let mut alike: Vec<usize> = Vec::new();
        for (member, row) in all.iter().enumerate() {
            let known = (0..member).find(|other| all[*other].len() == row.len() && columns(&[all[*other].clone(), row.clone()]).0.is_empty());
            alike.push(known.map_or(member, |other| alike[other]));
        }
        let kind = match (distinct(&|copy| exact[copy]), alike.iter().collect::<BTreeSet<_>>().len()) {
            (1, 1) => Sameness::Exact,
            (1, _) => Sameness::Same,
            _ => Sameness::Near,
        };
        // What differs: the words of the texts when they have one shape,
        // else what the relocations name when the bytes are the same.
        let rows: Vec<Vec<Word>> = match (distinct(&|copy| shape[copy]) == 1, distinct(&|copy| apart[copy]) == 1) {
            (true, _) => all,
            (false, true) => members.iter().map(|copy| targets(*copy)).collect(),
            (false, false) => Vec::new(),
        };
        let (differs, renames) = columns(&rows);
        let of = |columns: &[Vec<String>], member: usize| columns.iter().map(|column| column[member].as_str()).collect::<Vec<_>>().join(" ");
        // One image cannot link two functions of one name: copies that
        // share an image keep their own names.
        let images = |copy: &usize| &units[copies[*copy].unit].images;
        let twice = members.iter().map(|copy| members.iter().any(|other| other != copy && !images(other).is_disjoint(images(copy)))).collect();
        let parameters = if kind != Sameness::Near { 0 } else if rows.is_empty() { options.near_words } else { differs.len() };
        groups.push(Group {
            kind,
            text: numbered(alike.into_iter()),
            code: numbered(members.iter().map(|copy| exact[*copy])),
            differs: (0..members.len()).map(|member| of(&differs, member)).collect(),
            renames: (0..members.len()).map(|member| of(&renames, member)).collect(),
            copies: members,
            parameters,
            twice,
            run: 0,
            place: 0,
            of: 0,
        });
    }
    let parted = runs(copies, &mut groups);
    (groups, parted)
}

/// Numbers the runs: two groups stand in one when every copy of the first
/// is followed in its unit by a copy of the second, every copy of the
/// second follows one of the first, and the text between is the same.
/// Gives how many pairs only the text between keeps apart.
fn runs(copies: &[Written], groups: &mut [Group]) -> usize {
    let mut of: HashMap<usize, usize> = HashMap::new();
    for (index, group) in groups.iter().enumerate() {
        of.extend(group.copies.iter().map(|copy| (*copy, index)));
    }
    let beside = |copy: usize, other: Option<usize>| other.filter(|other| copies.get(*other).is_some_and(|c| c.unit == copies[copy].unit)).and_then(|other| of.get(&other).copied());
    let mut next: Vec<Option<usize>> = vec![None; groups.len()];
    let mut follows = vec![false; groups.len()];
    let mut parted = 0;
    for (index, group) in groups.iter().enumerate() {
        let after: BTreeSet<Option<usize>> = group.copies.iter().map(|copy| beside(*copy, Some(copy + 1))).collect();
        let Some(Some(other)) = (after.len() == 1).then(|| after.into_iter().next()).flatten() else {
            continue;
        };
        let second = &groups[other].copies;
        if other == index || !second.iter().all(|copy| beside(*copy, copy.checked_sub(1)) == Some(index)) {
            continue;
        }
        if second.iter().all(|copy| copies[*copy].gap == copies[second[0]].gap) {
            next[index] = Some(other);
            follows[other] = true;
        } else {
            parted += 1;
        }
    }
    let mut run = 0;
    for (start, _) in follows.iter().enumerate().filter(|(_, follows)| !**follows) {
        run += 1;
        let mut row = vec![start];
        while let Some(index) = next[row[row.len() - 1]] {
            row.push(index);
        }
        for (place, index) in row.iter().enumerate() {
            (groups[*index].run, groups[*index].place, groups[*index].of) = (run, place + 1, row.len());
        }
    }
    parted
}

// ---------------------------------------------------------------------------
// Reporting

const HEADER: &str = "group\tkind\trun\tplace\tof\tname\tfile\tline\tlines\tbytes\timages\tunits\tshared\ttext\tcode\tdiffers\trenames\tsaves\tnames\tfragment";

/// One written copy of a group: a definition and the units that compile it there.
struct Row {
    definition: usize,
    name: String,
    bytes: usize,
    images: BTreeSet<String>,
    units: usize,
    shared: bool,
    text: usize,
    code: usize,
    differs: String,
    renames: String,
    /// An image links this copy and another of the group.
    apart: bool,
    saves: i64,
}

struct Reported {
    number: usize,
    kind: Sameness,
    trivial: bool,
    renamed: bool,
    rows: Vec<Row>,
    /// The row whose text stays when the group is written once.
    kept: usize,
    run: (usize, usize, usize),
    fragment: Vec<String>,
}

impl Reported {
    fn saves(&self) -> i64 {
        self.rows.iter().map(|row| row.saves).sum()
    }
}

fn rows(read: &Read, groups: &[Group]) -> Vec<Reported> {
    // A file two units compile, or one that is no unit, is already shared.
    let mut users: BTreeMap<&str, BTreeSet<usize>> = BTreeMap::new();
    for copy in &read.copies {
        for definition in &copy.definitions {
            users.entry(read.definitions[*definition].file.as_str()).or_default().insert(copy.unit);
        }
    }
    let shared = |file: &str| users[file].len() > 1 || !read.units.iter().any(|unit| unit.file == file);
    let mut reported = Vec::new();
    for (index, group) in groups.iter().enumerate() {
        let mut rows: Vec<Row> = Vec::new();
        for (member, copy) in group.copies.iter().enumerate() {
            let copy = &read.copies[*copy];
            let (definition, function) = (copy.definitions[0], &read.functions[copy.function]);
            let row = rows.iter().position(|row| row.definition == definition).unwrap_or_else(|| {
                let code = function.code.get(1).and_then(Option::as_ref).or_else(|| function.code.iter().flatten().next());
                rows.push(Row {
                    definition,
                    name: function.name.clone(),
                    bytes: code.map_or(0, |code| code.bytes.len()),
                    images: BTreeSet::new(),
                    units: 0,
                    shared: shared(&read.definitions[definition].file),
                    text: group.text[member],
                    code: group.code[member],
                    differs: group.differs[member].clone(),
                    renames: group.renames[member].clone(),
                    apart: false,
                    saves: 0,
                });
                rows.len() - 1
            });
            rows[row].units += 1;
            rows[row].apart |= group.twice[member];
            rows[row].images.extend(read.units[copy.unit].images.iter().cloned());
        }
        // Written once, the text stays where it is already shared, else in
        // the first place of the way most copies write it; a copy still
        // gives the words it differs in, and its name where its image links
        // another of the copies.
        let written = |row: &Row| rows.iter().filter(|other| other.text == row.text).count();
        let kept = (0..rows.len()).max_by_key(|row| (rows[*row].shared, rows[*row].units, written(&rows[*row]), std::cmp::Reverse(*row))).unwrap_or(0);
        for (at, row) in rows.iter_mut().enumerate() {
            let lines = read.definitions[row.definition].lines as i64;
            row.saves = if at == kept { 0 } else { lines } - group.parameters as i64 - i64::from(row.apart);
        }
        let fragment: BTreeSet<String> = rows.iter().filter(|row| row.shared).map(|row| read.definitions[row.definition].file.clone()).collect();
        reported.push(Reported {
            number: index + 1,
            kind: group.kind,
            trivial: rows.iter().all(|row| read.definitions[row.definition].text.trivial),
            renamed: rows.iter().any(|row| row.name != rows[0].name),
            rows,
            kept,
            run: (group.run, group.place, group.of),
            fragment: fragment.into_iter().collect(),
        });
    }
    reported
}

fn table(read: &Read, reported: &[Reported], trivial: bool) -> String {
    let mut out = format!("{HEADER}\n");
    let or_none = |text: &str| if text.trim().is_empty() { "-".to_string() } else { text.to_string() };
    for group in reported.iter().filter(|group| group.trivial == trivial) {
        for row in &group.rows {
            let definition = &read.definitions[row.definition];
            let fields = [
                group.number.to_string(),
                group.kind.label().to_string(),
                group.run.0.to_string(),
                group.run.1.to_string(),
                group.run.2.to_string(),
                row.name.clone(),
                definition.file.clone(),
                definition.line.to_string(),
                definition.lines.to_string(),
                row.bytes.to_string(),
                row.images.iter().cloned().collect::<Vec<_>>().join(","),
                row.units.to_string(),
                if row.shared { "yes" } else { "no" }.to_string(),
                row.text.to_string(),
                row.code.to_string(),
                or_none(&row.differs),
                or_none(&row.renames),
                row.saves.to_string(),
                if row.apart { "apart" } else if group.renamed { "differ" } else { "same" }.to_string(),
                or_none(&group.fragment.join(",")),
            ];
            let _ = writeln!(out, "{}", fields.join("\t"));
        }
    }
    out
}

/// The same code under several names, in any object the build links: each
/// name, how many functions have it, their size and what they name.
fn names(functions: &[Function], same: &[usize]) -> String {
    let mut sets: BTreeMap<usize, BTreeMap<&str, (usize, &Function)>> = BTreeMap::new();
    for (function, set) in functions.iter().zip(same) {
        sets.entry(*set).or_default().entry(&function.name).or_insert((0, function)).0 += 1;
    }
    let mut out = String::from("set\tname\tfunctions\tbytes\tnames\n");
    for (number, names) in sets.values().filter(|names| names.len() > 1).enumerate() {
        for (name, (count, function)) in names {
            let code = function.code.iter().flatten().next();
            let _ = writeln!(out, "{}\t{name}\t{count}\t{}\t{}", number + 1, code.map_or(0, |code| code.bytes.len()), code.map_or(String::new(), |code| code.targets.join(" ")));
        }
    }
    out
}

fn short(file: &str) -> &str {
    file.split_once("/src/").map_or(file, |(_, tail)| tail)
}

fn summary(read: &Read, same: &[usize], reported: &[Reported], options: &Options) -> String {
    let counted: Vec<&Reported> = reported.iter().filter(|group| !group.trivial).collect();
    // A run is as same as its least same group.
    let mut runs: BTreeMap<usize, Vec<&Reported>> = BTreeMap::new();
    for group in &counted {
        runs.entry(group.run.0).or_default().push(group);
    }
    let kind_of = |run: &[&Reported]| run.iter().map(|group| group.kind).max().unwrap_or(Sameness::Exact);
    let saved = |groups: &[&Reported]| groups.iter().map(|group| group.saves()).sum::<i64>();
    let mut out = String::from("# Functions ☀️ writes out more than once\n\n");
    let _ = writeln!(out, "{} C units, {} function definitions, {} compiled functions compared.\n", read.units.len(), read.definitions.len(), read.copies.len());
    let _ = writeln!(out, "| kind | groups | runs | written copies | copies that go | lines saved | copies in a shared image | their lines saved |\n|---|---:|---:|---:|---:|---:|---:|---:|");
    for kind in [Sameness::Exact, Sameness::Same, Sameness::Near] {
        let of: Vec<&Reported> = counted.iter().copied().filter(|group| group.kind == kind).collect();
        let written: usize = of.iter().map(|group| group.rows.len()).sum();
        let apart: Vec<&Row> = of.iter().flat_map(|group| group.rows.iter().filter(|row| row.apart)).collect();
        let runs = runs.values().filter(|run| kind_of(run) == kind).count();
        let _ = writeln!(out, "| {} | {} | {runs} | {written} | {} | {} | {} | {} |", kind.label(), of.len(), written - of.len(), saved(&of), apart.len(), apart.iter().map(|row| row.saves).sum::<i64>());
    }
    let trivial: Vec<&Reported> = reported.iter().filter(|group| group.trivial).collect();
    let _ = writeln!(
        out,
        "\nEXACT: the same code in every edition and the same text. SAME: the same code, written differently. NEAR: the same bytes apart from what the relocations name, or the same text apart from at most {} words, for functions of {} lines or more. Lines saved are the lines of every written copy but one, less one line for each word a copy has to give the shared text (a near copy's words, and its own name where its image links another of the copies). A run counts under its least same group. The last two columns are the written copies, among those before, whose image links another copy of their group: their names have to stay apart, so the shared text takes the name as a word too (a macro in one file rather than an include). Outside these totals: {} groups of trivial functions (empty bodies, single returns) with {} written copies, in trivial.tsv.\n",
        options.near_words,
        options.near_lines,
        trivial.len(),
        trivial.iter().map(|group| group.rows.len()).sum::<usize>()
    );

    let as_kept: i64 = counted.iter().filter(|group| group.kind == Sameness::Same).flat_map(|group| group.rows.iter().filter(|row| row.text == group.rows[group.kept].text)).map(|row| row.saves).sum();
    let _ = writeln!(out, "Of SAME's lines saved, {as_kept} are of copies written as the copy that stays (the way most copies of their group are written); the others are of copies written another way.\n");
    let words = |group: &Reported| group.rows[0].differs.split_whitespace().count();
    let by_words = |keep: &dyn Fn(usize) -> bool| {
        let of: Vec<&Reported> = counted.iter().copied().filter(|group| group.kind == Sameness::Near && keep(words(group))).collect();
        format!("{} groups, {} lines", of.len(), saved(&of))
    };
    let _ = writeln!(
        out,
        "NEAR groups by the words each copy would give besides its name: one word, {}; two, {}; more (the same bytes, with relocations that name other things), {}; not told from one shape of text or one shape of code, {}.\n",
        by_words(&|count| count == 1),
        by_words(&|count| count == 2),
        by_words(&|count| count > 2),
        by_words(&|count| count == 0)
    );
    // A run written once costs each file that writes it out today the
    // comment that marks the place and the include.
    let net = |run: &[&Reported]| {
        let files: BTreeSet<&str> = run.iter().flat_map(|group| group.rows.iter().filter(|row| !row.shared).map(|row| read.definitions[row.definition].file.as_str())).collect();
        saved(run) - 2 * files.len() as i64
    };
    let _ = writeln!(out, "By run, where the net is the lines saved less two (the comment that marks the place and the include) for each file that writes the run out today:\n\n| runs | count | groups | lines saved | net | with a net gain | their net |\n|---|---:|---:|---:|---:|---:|---:|");
    for kind in [Sameness::Exact, Sameness::Same, Sameness::Near] {
        let of: Vec<&Vec<&Reported>> = runs.values().filter(|run| kind_of(run) == kind).collect();
        let gains: Vec<i64> = of.iter().map(|run| net(run)).filter(|net| *net > 0).collect();
        let _ = writeln!(
            out,
            "| {} | {} | {} | {} | {} | {} | {} |",
            kind.label(),
            of.len(),
            of.iter().map(|run| run.len()).sum::<usize>(),
            of.iter().map(|run| saved(run)).sum::<i64>(),
            of.iter().map(|run| net(run)).sum::<i64>(),
            gains.len(),
            gains.iter().sum::<i64>()
        );
    }
    out.push('\n');

    let _ = writeln!(out, "## The twenty largest runs\n\n| run | kind | groups | copies of each | lines saved | bytes | from | to | files |\n|---:|---|---:|---:|---:|---:|---|---|---|");
    let mut largest: Vec<(&usize, &Vec<&Reported>)> = runs.iter().collect();
    largest.sort_by_key(|(_, run)| std::cmp::Reverse(saved(run)));
    for (run, groups) in largest.into_iter().take(20) {
        let mut ordered = groups.clone();
        ordered.sort_by_key(|group| group.run.1);
        let files: BTreeSet<&str> = ordered.iter().flat_map(|group| group.rows.iter().map(|row| short(&read.definitions[row.definition].file))).collect();
        let _ = writeln!(
            out,
            "| {run} | {} | {} | {} | {} | {} | {} | {} | {} |",
            kind_of(&ordered).label(),
            ordered.len(),
            ordered[0].rows.iter().map(|row| row.units).sum::<usize>(),
            saved(&ordered),
            ordered.iter().map(|group| group.rows[0].bytes).sum::<usize>(),
            ordered[0].rows[0].name,
            ordered[ordered.len() - 1].rows[0].name,
            files.into_iter().collect::<Vec<_>>().join(" ")
        );
    }

    let _ = writeln!(out, "\n## What stands out\n");
    let mut named: BTreeMap<usize, BTreeSet<&str>> = BTreeMap::new();
    for (set, function) in same.iter().zip(&read.functions) {
        named.entry(*set).or_default().insert(&function.name);
    }
    let several = named.values().filter(|names| names.len() > 1).count();
    let _ = writeln!(out, "- {several} sets of functions are the same code under several names (names.tsv): far-call veneers the overlays name differently, and helpers.");
    let mut list = |title: &str, of: Vec<&Reported>| {
        let _ = writeln!(out, "- {title}: {} groups, {} lines saved.", of.len(), saved(&of));
        let mut of = of;
        of.sort_by_key(|group| std::cmp::Reverse(group.saves()));
        for group in of.into_iter().take(8) {
            let names: BTreeSet<&str> = group.rows.iter().map(|row| row.name.as_str()).collect();
            let files: BTreeSet<&str> = group.rows.iter().map(|row| short(&read.definitions[row.definition].file)).collect();
            let _ = writeln!(out, "  - group {} ({}, {} lines): {} in {}", group.number, group.kind.label(), group.saves(), names.into_iter().collect::<Vec<_>>().join(", "), files.into_iter().collect::<Vec<_>>().join(" "));
        }
    };
    let of = |keep: &dyn Fn(&Reported) -> bool| counted.iter().copied().filter(|group| keep(group)).collect::<Vec<_>>();
    list("The same code under different names in different images (one name would do)", of(&|group| group.renamed && group.kind != Sameness::Near && !group.rows.iter().any(|row| row.apart)));
    list("The same code twice in one image (the names have to stay apart)", of(&|group| group.kind != Sameness::Near && group.rows.iter().any(|row| row.apart)));
    list("Copies that name the same code differently (a far-call veneer, a helper)", of(&|group| group.rows.iter().any(|row| !row.renames.trim().is_empty())));
    list("The same text compiled to different code (the units give it different macros, types or neighbours)", of(&|group| group.kind == Sameness::Near && group.rows.iter().all(|row| row.text == 1)));
    list("Code a shared file holds and another file writes out again", of(&|group| !group.fragment.is_empty() && group.rows.iter().any(|row| !row.shared)));
    for note in &read.notes {
        let _ = writeln!(out, "- {note}.");
    }
    out
}

fn parse(arguments: &[String]) -> Result<Options, String> {
    let mut options = Options { out_dir: PathBuf::from("out/reports/duplicates"), near_lines: 8, near_words: 2 };
    let mut rest = arguments.iter();
    while let Some(flag) = rest.next() {
        let mut value = || rest.next().ok_or_else(|| format!("{flag} needs a value\n{USAGE}"));
        match flag.as_str() {
            "--out-dir" => options.out_dir = PathBuf::from(value()?),
            "--near-lines" => options.near_lines = value()?.parse().map_err(|_| format!("--near-lines takes a number\n{USAGE}"))?,
            _ => return Err(format!("unknown option {flag}\n{USAGE}")),
        }
    }
    Ok(options)
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    if arguments.iter().any(|a| a == "-h" || a == "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    let options = parse(arguments)?;
    let root = crate::compiler::routing::root();
    let mut read = read(root)?;
    let same = same(&read.functions);
    let (groups, parted) = group(&read.units, &read.functions, &same, &read.copies, &read.definitions, &options);
    if parted > 0 {
        read.notes.push(format!("{parted} pairs of groups stand together in every unit and are in two runs because the text between them (data, declarations, a definition no object holds) differs from unit to unit"));
    }
    let reported = rows(&read, &groups);
    let out = root.join(&options.out_dir);
    fs::create_dir_all(&out).map_err(|e| format!("{}: {e}", out.display()))?;
    let summary = summary(&read, &same, &reported, &options);
    let tables = [("groups.tsv", table(&read, &reported, false)), ("trivial.tsv", table(&read, &reported, true)), ("names.tsv", names(&read.functions, &same)), ("summary.md", summary.clone())];
    for (name, text) in tables {
        fs::write(out.join(name), text).map_err(|e| format!("{}: {e}", out.join(name).display()))?;
    }
    let totals = summary.find("\n\n## ").unwrap_or(summary.len());
    println!("{}\n\nreports: {}/groups.tsv, trivial.tsv, names.tsv, summary.md", &summary[..totals], options.out_dir.display());
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    fn code(bytes: &[u8], targets: &[&str]) -> Vec<Option<Code>> {
        let fields = (0..targets.len()).map(|at| (at as u32 * 4, 10, 0)).collect();
        vec![Some(Code { bytes: bytes.to_vec(), fields, targets: targets.iter().map(|t| t.to_string()).collect() })]
    }

    fn function(name: &str, bytes: &[u8], targets: &[&str], names: &[(&str, usize)]) -> Function {
        Function { name: name.to_string(), code: code(bytes, targets), names: names.iter().map(|(name, at)| (name.to_string(), *at)).collect() }
    }

    fn definition(file: &str, line: usize, source: &str) -> Definition {
        Definition { file: file.to_string(), line, lines: source.lines().count(), conditions: String::new(), text: text::read(source).unwrap() }
    }

    #[test]
    fn a_name_is_the_same_when_it_is_spelled_the_same_or_names_the_same_code() {
        let functions = [
            // Two overlays' veneers to one far function, named differently.
            function("Blend", &[1], &["FarBlend"], &[]),
            function("SetBlend", &[1], &["FarBlend"], &[]),
            function("Hide", &[1], &["FarHide"], &[]),
            // Their callers: the same code through either name of the veneer.
            function("Make", &[2], &["Blend"], &[("Blend", 0)]),
            function("Create", &[2], &["SetBlend"], &[("SetBlend", 1)]),
            // Not through another veneer, and not with other bytes.
            function("Lose", &[2], &["Hide"], &[("Hide", 2)]),
            function("Other", &[3], &["Blend"], &[("Blend", 0)]),
            // Each unit's own Start is other code under one name: the
            // callers still name the same thing.
            function("Start", &[4], &[], &[]),
            function("Start", &[5], &[], &[]),
            function("Open", &[6], &["Start", "Make"], &[("Start", 7), ("Make", 3)]),
            function("Begin", &[6], &["Start", "Create"], &[("Start", 8), ("Create", 4)]),
        ];
        assert_eq!(same(&functions), [0, 0, 2, 3, 3, 5, 6, 7, 8, 9, 9]);
    }

    #[test]
    fn keys_agree_over_the_editions_both_have() {
        let keys = [vec![Some(1), Some(2)], vec![Some(1), None], vec![None, Some(3)], vec![Some(4), Some(2)], vec![None, None]];
        assert_eq!(classes(&keys), [0, 0, 1, 2, 3]);
        // The second place differs twice in one way, the fourth only in name.
        let rows = [vec![("a", "a"), ("x", "x"), ("x", "x"), ("p", "s"), ("c", "c")], vec![("a", "a"), ("y", "y"), ("y", "y"), ("q", "s"), ("d", "d")], vec![("a", "a"), ("x", "x"), ("x", "x"), ("p", "s"), ("c", "c")]];
        let (differs, renames) = columns(&rows);
        assert_eq!(differs, [["x", "y", "x"], ["c", "d", "c"]]);
        assert_eq!(renames, [["p", "q", "p"]]);
    }

    #[test]
    fn copies_are_grouped_by_code_and_text_and_stand_in_runs() {
        let functions = [
            function("Blend", &[1], &["FarBlend"], &[]),
            function("SetBlend", &[1], &["FarBlend"], &[]),
            // Unit a.
            function("Make", &[2], &["Blend"], &[("Blend", 0)]),
            function("Tick", &[3], &["TableA"], &[]),
            function("Only", &[9], &[], &[]),
            // Unit b: the same Make under another name through its own
            // veneer, Tick with its own table, and Tick again as Tock.
            function("Create", &[2], &["SetBlend"], &[("SetBlend", 1)]),
            function("Tick", &[3], &["TableB"], &[]),
            function("Tock", &[3], &["TableB"], &[]),
            // Unit c: Make written another way, and nothing after it.
            function("Make", &[2], &["Blend"], &[]),
            function("Zero", &[7], &[], &[]),
            function("Zero", &[7], &[], &[]),
        ];
        let make = "void Make(s32 mode)\n{\n\ts32 *work = Find(mode);\n\n\tBlend(work, 1); // 半透明\n}\n";
        let tick = |table: &str, name: &str| format!("s32 {name}(void)\n{{\n\treturn {table}[gFrame & 3];\n\t;\n}}\n");
        let definitions = [
            definition("a.c", 10, make),
            definition("a.c", 20, &tick("TableA", "Tick")),
            definition("a.c", 30, "void Only(void)\n{\n\tRun();\n}\n"),
            definition("b.c", 5, &make.replace("Make", "Create").replace("Blend", "SetBlend").replace("work", "w")),
            definition("b.c", 15, &tick("TableB", "Tick")),
            definition("b.c", 25, &tick("TableB", "Tock")),
            definition("c.c", 1, "void Make(s32 mode)\n{\n\tBlend(Find(mode), 1);\n}\n"),
            definition("a.c", 40, "s32 Zero(void)\n{\n\treturn 0;\n}\n"),
            definition("b.c", 40, "s32 Zero(void)\n{\n\treturn 0;\n}\n"),
        ];
        let copy = |unit: usize, function: usize, definition: usize| Written { unit, function, definitions: vec![definition], gap: String::new() };
        let copies = [copy(0, 2, 0), copy(0, 3, 1), copy(0, 4, 2), copy(0, 9, 7), copy(1, 5, 3), copy(1, 6, 4), copy(1, 7, 5), copy(1, 10, 8), copy(2, 8, 6)];
        let units: Vec<Unit> = ["a", "b", "c"].iter().map(|image| Unit { file: format!("{image}.c"), images: BTreeSet::from([image.to_string()]) }).collect();
        let options = Options { out_dir: PathBuf::new(), near_lines: 5, near_words: 2 };
        let same = same(&functions);
        let (groups, parted) = group(&units, &functions, &same, &copies, &definitions, &options);
        let seen: Vec<(Sameness, &[usize], usize)> = groups.iter().map(|group| (group.kind, group.copies.as_slice(), group.parameters)).collect();
        // Make is the same code in three units, written two ways; Tick and
        // Tock differ in the table they name, and unit b links two of them,
        // so those two keep their names; Zero is the same in both.
        assert_eq!(seen, [(Sameness::Same, &[0, 4, 8][..], 0), (Sameness::Near, &[1, 5, 6][..], 1), (Sameness::Exact, &[3, 7][..], 0)]);
        assert_eq!(groups[0].text, [1, 1, 2]);
        assert_eq!(groups[0].renames[..2], ["Blend", "SetBlend"]);
        assert_eq!(groups[1].differs, ["TableA", "TableB", "TableB"]);
        assert_eq!((groups[1].code.as_slice(), groups[1].twice.as_slice()), (&[1, 2, 2][..], &[false, true, true][..]));
        // Unit c has Make with no Tick after it: no run of the two.
        assert_eq!(groups.iter().map(|group| (group.run, group.place, group.of)).collect::<Vec<_>>(), [(1, 1, 1), (2, 1, 1), (3, 1, 1)]);
        assert_eq!(parted, 0);

        // Without unit c, and without Tock, Make and Tick stand together.
        let (groups, _) = group(&units, &functions, &same, &[copies[0].clone(), copies[1].clone(), copies[4].clone(), copies[5].clone()], &definitions, &options);
        assert_eq!(groups.iter().map(|group| (group.kind, group.run, group.place, group.of)).collect::<Vec<_>>(), [(Sameness::Exact, 1, 1, 2), (Sameness::Near, 1, 2, 2)]);
        // With other text before one Tick they do not.
        let mut moved = [copies[0].clone(), copies[1].clone(), copies[4].clone(), copies[5].clone()];
        moved[3].gap = "s32 TableB [ 4 ] ;\n".to_string();
        assert_eq!(group(&units, &functions, &same, &moved, &definitions, &options).1, 1);
    }

    #[test]
    fn a_units_text_takes_the_files_it_includes_at_their_place() {
        let root = tempfile::tempdir().unwrap();
        let write = |file: &str, text: &str| {
            let path = root.path().join(file);
            fs::create_dir_all(path.parent().unwrap()).unwrap();
            fs::write(path, text).unwrap();
        };
        write("games/tbs/src/a/unit.c", "#include \"types.h\"\n// 前置き\nvoid First(void)\n{\n\tRun();\n}\n\n// ここに共通ソースが入る\n#include \"../common/shared.c\"\n\ns32 gCount;\n\nvoid Last(s32 mode)\n{\n#include \"body.inc\"\n}\n");
        write("games/tbs/src/common/shared.c", "#include \"types.h\"\n#define STEP 2\n\nvoid Shared(void)\n{\n\tRun();\n}\n");
        write("games/tbs/include/body.inc", "\ts32 kept = mode;\n\n\tApply(kept);\n");
        let mut sources = Sources::default();
        let mut text = Vec::new();
        sources.expand(root.path(), "games/tbs/src/a/unit.c", &mut text, &mut String::new(), 0).unwrap();
        let seen: Vec<(&str, &str, usize, usize, &str)> = text.iter().map(|(d, gap)| (sources.definitions[*d].text.name.as_str(), short(&sources.definitions[*d].file), sources.definitions[*d].line, sources.definitions[*d].lines, gap.as_str())).collect();
        // Comments and headers are no text between; a definition and another directive are.
        assert_eq!(seen, [("First", "a/unit.c", 3, 4, ""), ("Shared", "common/shared.c", 4, 4, "__hash__ define STEP 2 __eol__\n"), ("Last", "a/unit.c", 13, 4, "s32 gCount ;\n")]);
        // The body a function includes is its text.
        assert_eq!(sources.definitions[text[2].0].text.tokens.join(" "), "void $0 ( s32 $1 ) { s32 $2 = $1 ; Apply ( $2 ) ; }");
        assert!(sources.missing.is_empty() && sources.unread.is_empty());
    }

    #[test]
    fn a_units_copies_are_the_functions_its_object_holds_in_the_order_of_its_text() {
        let definitions = [
            definition("a.c", 1, "static inline s32 Half(s32 x)\n{\n\treturn x / 2;\n}\n"),
            definition("a.c", 6, "s32 Run(void)\n{\n\treturn Half(4);\n}\n"),
            definition("f.c", 1, "void Draw(void)\n{\n}\n"),
            definition("a.c", 12, "s32 Run(void)\n{\n\treturn Half(6);\n}\n"),
        ];
        let text = [(0, "# define SIZE 4\n".to_string()), (1, String::new()), (2, "s32 gCount ;\n".to_string()), (3, String::new())];
        let compiled = HashMap::from([("Run", 7), ("Draw", 8)]);
        let found = copies(3, &definitions, &text, &compiled);
        assert_eq!(found.iter().map(|copy| (copy.unit, copy.function, copy.definitions.as_slice())).collect::<Vec<_>>(), [(3, 7, &[1, 3][..]), (3, 8, &[2][..])]);
        // The inline function no object holds is text before Run.
        assert_eq!(found[0].gap, "# define SIZE 4\nstatic inline s32 $0 ( s32 $1 ) { return $1 / 2 ; }\n");
        assert_eq!(found[1].gap, "s32 gCount ;\n");
    }
}
