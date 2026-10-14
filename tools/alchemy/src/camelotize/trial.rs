//! Source edits proved one file at a time: each set of edits is compiled and
//! assembled with each edition's compiler, flags and assembler, and kept only
//! when the object (every section's bytes, every relocation and symbol) is
//! identical and the compiler says nothing new in every edition of the game
//! that builds the file. Sets are tried together and bisected
//! when one fails, so a file costs a handful of compiles, not one per edit.
//! A file other files include (a scene/common header) is proved in every C
//! file that reaches it, directly or through other headers (`including`):
//! each is compiled in a copy of the tree where every header on the way is
//! copied beside the edited file, and a C file that compiles without
//! reading the file refuses the judge (`Judge`). Edits that match only
//! beside each other are kept together (`proved_groups`).

use crate::compiler::bundle::compiler_command;
use crate::compiler::routing::{cflags_for_target_source, compiler_assembly_command, is_arm};
use crate::targets::{target_for, DecompTarget, TARGET_IDS};
use std::path::{Path, PathBuf};
use std::process::Command;

/// Replace `start..end` with `text`.
#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Edit {
    pub start: usize,
    pub end: usize,
    pub text: String,
}

fn joins(a: u8, b: u8) -> bool {
    let word = |c: u8| c.is_ascii_alphanumeric() || c == b'_';
    let operator = |c: u8| b"+-*/%<>=!&|^~.".contains(&c);
    (word(a) && word(b)) || (operator(a) && operator(b))
}

/// The text with the edits made, overlapping ones after the first dropped.
/// An edit that takes text out also takes the blanks after it, and keeps one
/// space wherever the tokens on either side would otherwise run together.
pub(crate) fn apply(text: &str, edits: &[Edit]) -> String {
    let mut sorted = edits.to_vec();
    sorted.sort_by_key(|e| e.start);
    let bytes = text.as_bytes();
    let mut out = String::with_capacity(text.len());
    let mut at = 0;
    for edit in sorted {
        if edit.start < at {
            continue;
        }
        out.push_str(&text[at..edit.start]);
        let mut end = edit.end;
        if edit.text.is_empty() {
            while end < bytes.len() && matches!(bytes[end], b' ' | b'\t') {
                end += 1;
            }
        }
        let next = edit.text.bytes().next().or_else(|| bytes.get(end).copied());
        if let (Some(&a), Some(b)) = (out.as_bytes().last(), next) {
            if joins(a, b) {
                out.push(' ');
            }
        }
        out.push_str(&edit.text);
        if let (Some(a), Some(&b)) = (edit.text.bytes().last(), bytes.get(end)) {
            if joins(a, b) {
                out.push(' ');
            }
        }
        at = end;
    }
    out.push_str(&text[at..]);
    out
}

/// The candidates outside FAKEMATCH-tagged statements and asm.
pub(crate) fn untagged(text: &str, found: Vec<Edit>) -> Vec<Edit> {
    let tagged = super::tagged::ranges(text);
    found.into_iter().filter(|e| !super::tagged::covered(&tagged, e.start, e.end)).collect()
}

/// What an object file gives the linker: every section's name, size and
/// bytes, its relocations by target name, and its symbols. Debug sections
/// and the local `.L` labels, which only number GCC's jumps, are left out.
pub(crate) fn object_contents(bytes: &[u8]) -> Result<String, String> {
    use object::{Object, ObjectSection, ObjectSymbol, RelocationTarget, SectionKind};
    use std::fmt::Write;
    let file = object::File::parse(bytes).map_err(|e| e.to_string())?;
    let section_name = |index| file.section_by_index(index).ok().and_then(|s| s.name().ok().map(str::to_string)).unwrap_or_default();
    let mut out = String::new();
    for section in file.sections() {
        let name = section.name().unwrap_or("");
        if matches!(section.kind(), SectionKind::Metadata | SectionKind::Debug | SectionKind::DebugString)
            || name.starts_with(".stab")
            || name.starts_with(".debug")
        {
            continue;
        }
        let data = section.data().map_err(|e| e.to_string())?;
        let _ = writeln!(out, "section {name} {:?} size {} align {} {data:02x?}", section.kind(), section.size(), section.align());
        for (offset, relocation) in section.relocations() {
            let target = match relocation.target() {
                RelocationTarget::Symbol(index) => match file.symbol_by_index(index) {
                    Ok(symbol) if !symbol.name().unwrap_or("").is_empty() => symbol.name().unwrap_or("").to_string(),
                    Ok(symbol) => symbol.section_index().map(section_name).unwrap_or_default(),
                    Err(e) => return Err(e.to_string()),
                },
                RelocationTarget::Section(index) => section_name(index),
                _ => String::new(),
            };
            let _ = writeln!(out, "  reloc {offset:#x} {:?} {} {target}", relocation.flags(), relocation.addend());
        }
    }
    let mut symbols: Vec<String> = file
        .symbols()
        .filter(|s| !s.name().unwrap_or("").starts_with(".L"))
        .map(|s| {
            let section = s.section_index().map(section_name).unwrap_or_default();
            format!("symbol {} {section} {:#x} {} {:?} {:?}", s.name().unwrap_or(""), s.address(), s.size(), s.kind(), s.scope())
        })
        .collect();
    symbols.sort();
    out.push_str(&symbols.join("\n"));
    Ok(out)
}

/// The compiler's messages without their line numbers, sorted, so an edit
/// that moves lines (an else flattened) leaves them the same unless a
/// message is new. The include chain GCC prints above a message from a
/// header is left out: an include added above it (event.h) moves where the
/// message comes from, it does not make it new.
pub(crate) fn unnumbered(messages: &str) -> String {
    let number = regex::Regex::new(r":\d+:").expect("pattern");
    let chain = |line: &&str| line.starts_with("In file included from ") || line.trim_start().starts_with("from ");
    let mut lines: Vec<String> = messages.lines().filter(|line| !chain(line)).map(|line| number.replace(line, ":").into_owned()).collect();
    lines.sort();
    lines.join("\n")
}

/// Candidates that overlap an earlier one are dropped.
pub(crate) fn disjoint(mut edits: Vec<Edit>) -> Vec<Edit> {
    edits.sort_by_key(|e| e.start);
    let mut out: Vec<Edit> = Vec::with_capacity(edits.len());
    for edit in edits {
        if out.last().is_none_or(|last| last.end <= edit.start) {
            out.push(edit);
        }
    }
    out
}

struct Build {
    target: DecompTarget,
    imports: Option<std::sync::Arc<tempfile::TempDir>>,
    directory: tempfile::TempDir,
}

impl Build {
    /// The object's contents and the compiler's messages for this text in
    /// place of the file. Assembly text is not compared: an edit that only
    /// renumbers GCC's `.L` labels leaves the object identical.
    fn compile(&self, root: &Path, file: &Path, text: &str) -> Result<(String, String), String> {
        let (bytes, messages) = self.assemble(root, file, text)?;
        Ok((object_contents(&bytes)?, unnumbered(&messages)))
    }

    /// The object file itself and the messages as the tools gave them.
    fn assemble(&self, root: &Path, file: &Path, text: &str) -> Result<(Vec<u8>, String), String> {
        self.assemble_among(root, file, text, &[])
    }

    /// As `assemble`, with other files of the tree read as these texts:
    /// each is a path under the root and what stands in its place. The
    /// file and the others are laid out in a copy of the tree's folders,
    /// with every file of the tree on a path of includes from the file to
    /// a replaced one, and what each copied file includes from beside
    /// itself: so an include reaches a replaced file however many headers
    /// lie between, as GCC finds a quoted include beside the file that
    /// includes it first. Each folder of the search path is searched in
    /// the copy just before the tree.
    fn assemble_among(&self, root: &Path, file: &Path, text: &str, others: &[(String, String)]) -> Result<(Vec<u8>, String), String> {
        let name = file.file_name().and_then(|n| n.to_str()).unwrap_or("source.c");
        let assembly = self.directory.path().join("out.s");
        let object = self.directory.path().join("OUT.O");
        // The file's own folder, then the flags.
        let flags: Vec<String> = std::iter::once(format!("-I{}", file.parent().unwrap_or(root).display())).chain(cflags_for_target_source(self.target.compiler, name)).collect();
        let mut arguments = Vec::new();
        let source = if others.is_empty() {
            arguments.extend(flags);
            self.directory.path().join(name)
        } else {
            let copy = self.directory.path().join("tree");
            if copy.exists() {
                std::fs::remove_dir_all(&copy).map_err(|e| e.to_string())?;
            }
            let place = |path: &Path, text: &str| -> Result<std::path::PathBuf, String> {
                let to = copy.join(path.strip_prefix(root).unwrap_or(path));
                std::fs::create_dir_all(to.parent().unwrap_or(&copy)).map_err(|e| e.to_string())?;
                std::fs::write(&to, text).map_err(|e| e.to_string())?;
                Ok(to)
            };
            for (path, text) in others {
                place(Path::new(path), text)?;
            }
            let mut search: Vec<PathBuf> = flags.iter().filter_map(|flag| flag.strip_prefix("-I")).map(PathBuf::from).collect();
            search.extend(self.imports.iter().map(|imports| imports.path().to_path_buf()));
            for (path, text) in between(root, file, text, others, &search)? {
                place(Path::new(&path), &text)?;
            }
            let source = place(file, "")?;
            for flag in flags {
                let shadow = flag.strip_prefix("-I").and_then(|folder| Path::new(folder).strip_prefix(root).ok()).map(|folder| copy.join(folder));
                if let Some(shadow) = shadow.filter(|shadow| shadow.is_dir()) {
                    arguments.push(format!("-I{}", shadow.display()));
                }
                arguments.push(flag);
            }
            source
        };
        std::fs::write(&source, text).map_err(|e| e.to_string())?;
        // What a replaced C file, or header of C text, includes beside
        // itself is still found beside it, after everything the search
        // path offers.
        for (path, _) in others.iter().filter(|(path, text)| crate::c_items::c_text(path, text)) {
            let beside = format!("-I{}", root.join(path).parent().unwrap_or(root).display());
            if !arguments.contains(&beside) {
                arguments.push(beside);
            }
        }
        arguments.push(format!("-D{}=1", self.target.edition_define));
        if let Some(imports) = &self.imports {
            arguments.push(format!("-I{}", imports.path().display()));
        }
        arguments.extend(["-S".into(), "-o".into(), assembly.to_string_lossy().into_owned()]);
        arguments.extend(["-x".into(), "c".into(), source.to_string_lossy().into_owned()]);
        let command = compiler_command(&arguments)?;
        let output = Command::new(&command[0]).args(&command[1..]).current_dir(root).output().map_err(|e| e.to_string())?;
        let mut messages = String::from_utf8_lossy(&output.stderr).into_owned();
        if !output.status.success() {
            return Err(messages);
        }
        let arm = is_arm(self.target.compiler, name);
        let command = compiler_assembly_command(&assembly.to_string_lossy(), &object.to_string_lossy(), arm);
        let output = Command::new(&command[0]).args(&command[1..]).current_dir(root).output().map_err(|e| e.to_string())?;
        messages.push_str(&String::from_utf8_lossy(&output.stderr));
        if !output.status.success() {
            return Err(messages);
        }
        let bytes = std::fs::read(&object).map_err(|e| e.to_string())?;
        Ok((bytes, messages))
    }
}

/// The files of the tree a compile of `file` (read as `text`) among
/// `others` reads from the copy, as paths under the root with their tree
/// texts: each file on a path of includes from `file` to a replaced one,
/// and, from each of those and each replaced file, what it includes from
/// beside itself. `search` is the compile's search path, in order. A file
/// outside the root has no place in the copy, and is read where it is.
fn between(root: &Path, file: &Path, text: &str, others: &[(String, String)], search: &[PathBuf]) -> Result<Vec<(String, String)>, String> {
    let canonical = |path: PathBuf| path.canonicalize().unwrap_or(path);
    let base = canonical(root.to_path_buf());
    let unit = canonical(file.to_path_buf());
    let replaced: std::collections::BTreeMap<PathBuf, &str> = others.iter().map(|(path, text)| (canonical(root.join(path)), text.as_str())).collect();
    let stand_in = |path: &Path| if path == unit { Some(text.to_string()) } else { replaced.get(path).map(|text| (*text).to_string()) };
    let graph = super::source::include_graph(&unit, search, &stand_in);
    let mut copied: std::collections::BTreeSet<PathBuf> = replaced.keys().cloned().collect();
    loop {
        let before = copied.len();
        for (path, included) in &graph {
            if included.iter().any(|(to, _)| copied.contains(to)) {
                copied.insert(path.clone());
            }
        }
        for (path, included) in &graph {
            if copied.contains(path) {
                copied.extend(included.iter().filter(|(_, beside)| *beside).map(|(to, _)| to.clone()));
            }
        }
        if copied.len() == before {
            break;
        }
    }
    let mut out = Vec::new();
    for path in copied.iter().filter(|path| **path != unit && !replaced.contains_key(*path)) {
        if let Ok(relative) = path.strip_prefix(&base) {
            let text = std::fs::read_to_string(path).map_err(|e| format!("{}: {e}", path.display()))?;
            out.push((relative.to_string_lossy().into_owned(), text));
        }
    }
    Ok(out)
}

/// Every edition of these games.
pub(crate) fn editions(games: &[DecompTarget]) -> Vec<DecompTarget> {
    games
        .iter()
        .flat_map(|&game| TARGET_IDS.iter().map(|&id| target_for(id)).filter(move |t| t.compiler == game.compiler))
        .collect()
}

/// The edits that leave the file's assembly and messages unchanged in every
/// game it builds in, applied, and how many there were.
pub(crate) fn proved(root: &Path, file: &Path, text: &str, games: &[DecompTarget], found: Vec<Edit>) -> Result<(String, usize), String> {
    let found = disjoint(found);
    if found.is_empty() {
        return Ok((text.to_string(), 0));
    }
    // Every edition of the game proves every edit: an edition's headers and
    // defines can change what an edit outside any #if means (script.c's
    // casts kept English and broke tbs-ja). A file one edition cannot
    // compile is skipped whole.
    let mut builds = Vec::new();
    for target in editions(games) {
        let imports = crate::build_text::fresh_c_imports(root, target)?;
        let directory = tempfile::Builder::new().prefix("alchemy-trial-").tempdir().map_err(|e| e.to_string())?;
        let build = Build { target, imports: imports.map(std::sync::Arc::new), directory };
        let baseline = build.compile(root, file, text)?;
        builds.push((build, baseline));
    }
    let (kept, _) = crate::keep::bisect(&found, |set: &[Edit]| -> Result<bool, String> {
        let candidate = apply(text, set);
        for (build, baseline) in &builds {
            match build.compile(root, file, &candidate) {
                Ok(result) if &result == baseline => {}
                _ => return Ok(false),
            }
        }
        Ok(true)
    })?;
    Ok((apply(text, &kept), kept.len()))
}

/// The C files of these games, under `games/<game>/src` and
/// `games/common/src`, that include `header`, directly or through other
/// headers, as a trial compile's include path finds it: the C file's own
/// folder, then the game's.
pub(crate) fn including(root: &Path, header: &Path, games: &[DecompTarget]) -> Vec<PathBuf> {
    let Ok(header) = header.canonicalize() else {
        return Vec::new();
    };
    let mut found = std::collections::BTreeSet::new();
    for game in games {
        let flags: Vec<PathBuf> =
            cflags_for_target_source(game.compiler, "unit.c").iter().filter_map(|flag| flag.strip_prefix("-I")).map(PathBuf::from).collect();
        for folder in [root.join(game.source_dir), root.join("games/common/src")] {
            for entry in walkdir::WalkDir::new(&folder).into_iter().filter_map(Result::ok) {
                let path = entry.path();
                if path.extension().is_none_or(|e| e != "c") {
                    continue;
                }
                let search: Vec<PathBuf> = path.parent().map(Path::to_path_buf).into_iter().chain(flags.iter().cloned()).collect();
                if super::source::reachable(path, &search).contains(&header) {
                    found.insert(path.to_path_buf());
                }
            }
        }
    }
    found.into_iter().collect()
}

/// `work` of each index below `count`, run side by side on up to `threads`
/// threads, in index order.
fn side_by_side<T: Send>(threads: usize, count: usize, work: impl Fn(usize) -> T + Sync) -> Vec<T> {
    use std::sync::atomic::{AtomicUsize, Ordering};
    let next = AtomicUsize::new(0);
    let slots: Vec<std::sync::Mutex<Option<T>>> = (0..count).map(|_| std::sync::Mutex::new(None)).collect();
    std::thread::scope(|scope| {
        for _ in 0..threads.clamp(1, count.max(1)) {
            scope.spawn(|| loop {
                let k = next.fetch_add(1, Ordering::Relaxed);
                if k >= count {
                    break;
                }
                let done = work(k);
                *slots[k].lock().expect("slot") = Some(done);
            });
        }
    });
    slots.into_iter().map(|slot| slot.into_inner().expect("slot").expect("every index is worked")).collect()
}

/// A compile's object contents and messages, or why it failed.
type Outcome = Result<(String, String), String>;

/// One compile a judge repeats: an edition, the C file it compiles (the
/// file itself, or one that includes it), that file's text, and its object
/// and messages with the file as the tree has it.
struct Trial {
    build: Build,
    unit: PathBuf,
    unit_text: String,
    baseline: (String, String),
}

/// The compiles that judge a file's edits: the file itself in every edition
/// of its games, or, for an included file, every C file that includes it in
/// every edition, with the file read as the candidate text. A candidate
/// passes when every object (each section's bytes, relocation and symbol)
/// and the compiler's messages are unchanged in all of them.
pub(crate) struct Judge {
    /// The included file as a path under the root, when the compiled
    /// units are the files that include it.
    included: Option<String>,
    file: PathBuf,
    trials: Vec<Trial>,
    threads: usize,
    /// Headers, as paths under the root, whose own messages are not
    /// compared: those an added include brings in for the first time.
    quiet: Vec<String>,
}

impl Judge {
    /// A judge of `file`, whose tree text is `text`: compiled itself when
    /// `units` is empty, else each of `units` with the file in it. A unit
    /// one edition cannot compile is an error, and so is a unit that no
    /// edition's compile reads the file in: each unit is compiled once with
    /// an `#error` above the file's text, and must fail.
    pub(crate) fn new(root: &Path, file: &Path, text: &str, games: &[DecompTarget], units: &[PathBuf], threads: usize) -> Result<Judge, String> {
        let included = if units.is_empty() {
            None
        } else {
            let relative = file.strip_prefix(root).map_err(|_| format!("{} is outside {}", file.display(), root.display()))?;
            Some(relative.to_string_lossy().into_owned())
        };
        let compiled: Vec<PathBuf> = if units.is_empty() { vec![file.to_path_buf()] } else { units.to_vec() };
        let mut judge = Judge { included, file: file.to_path_buf(), trials: Vec::new(), threads: threads.max(1), quiet: Vec::new() };
        for target in editions(games) {
            let imports = crate::build_text::fresh_c_imports(root, target)?;
            let imports = imports.map(std::sync::Arc::new);
            for unit in &compiled {
                let unit_text = if units.is_empty() {
                    text.to_string()
                } else {
                    std::fs::read_to_string(unit).map_err(|e| format!("{}: {e}", unit.display()))?
                };
                let directory = tempfile::Builder::new().prefix("alchemy-trial-").tempdir().map_err(|e| e.to_string())?;
                let build = Build { target, imports: imports.clone(), directory };
                judge.trials.push(Trial { build, unit: unit.clone(), unit_text, baseline: Default::default() });
            }
        }
        let outcomes: Vec<Outcome> = side_by_side(judge.threads, judge.trials.len(), |k| judge.outcome(root, &judge.trials[k], text));
        for (trial, outcome) in judge.trials.iter_mut().zip(outcomes) {
            trial.baseline = outcome.map_err(|e| format!("{} ({}): {e}", trial.unit.display(), trial.build.target.id))?;
        }
        if let Some(relative) = &judge.included {
            let probe = format!("#error alchemy trial probe: {relative} is read here\n{text}");
            let unread: Vec<bool> = side_by_side(judge.threads, units.len(), |u| {
                judge.trials.iter().filter(|trial| trial.unit == units[u]).all(|trial| judge.outcome(root, trial, &probe).is_ok())
            });
            if let Some(unit) = units.iter().zip(unread).find_map(|(unit, unread)| unread.then_some(unit)) {
                return Err(format!("{} compiles without reading {relative}, so its trials would prove nothing", unit.display()));
            }
        }
        Ok(judge)
    }

    fn outcome(&self, root: &Path, trial: &Trial, text: &str) -> Outcome {
        match &self.included {
            None => trial.build.compile(root, &self.file, text),
            Some(relative) => {
                let (bytes, messages) = trial.build.assemble_among(root, &trial.unit, &trial.unit_text, &[(relative.clone(), text.to_string())])?;
                Ok((object_contents(&bytes)?, unnumbered(&messages)))
            }
        }
    }

    /// Leaves out of the comparison the messages these headers (paths under
    /// the root) give about their own lines. A header an added include
    /// brings in for the first time may warn about its own inlines; that
    /// says nothing about the file's code, whose messages are still
    /// compared.
    pub(crate) fn quiet(&mut self, headers: Vec<String>) {
        self.quiet = headers;
    }

    /// The messages, less those about the quiet headers' own lines.
    fn heard(&self, messages: &str) -> String {
        let quiet = |line: &str| line.split_once(':').is_some_and(|(place, _)| self.quiet.iter().any(|h| Path::new(place).ends_with(h)));
        messages.lines().filter(|line| !quiet(line)).collect::<Vec<_>>().join("\n")
    }

    /// Whether a compile's outcome is its baseline.
    fn same(&self, outcome: &(String, String), baseline: &(String, String)) -> bool {
        outcome.0 == baseline.0 && self.heard(&outcome.1) == self.heard(&baseline.1)
    }

    /// Whether this text in place of the file leaves every compile as it
    /// was. The compiles run side by side and stop at the first difference.
    pub(crate) fn passes(&self, root: &Path, text: &str) -> bool {
        use std::sync::atomic::{AtomicBool, AtomicUsize, Ordering};
        let next = AtomicUsize::new(0);
        let failed = AtomicBool::new(false);
        std::thread::scope(|scope| {
            for _ in 0..self.threads.min(self.trials.len()) {
                scope.spawn(|| loop {
                    if failed.load(Ordering::Relaxed) {
                        break;
                    }
                    let Some(trial) = self.trials.get(next.fetch_add(1, Ordering::Relaxed)) else {
                        break;
                    };
                    if !self.outcome(root, trial, text).is_ok_and(|outcome| self.same(&outcome, &trial.baseline)) {
                        failed.store(true, Ordering::Relaxed);
                    }
                });
            }
        });
        !failed.load(Ordering::Relaxed)
    }

    /// The first compile this text changes, said for a person: the unit,
    /// the edition and what changed.
    pub(crate) fn difference(&self, root: &Path, text: &str) -> Option<String> {
        self.trials.iter().find_map(|trial| {
            let place = format!("{} ({})", trial.unit.strip_prefix(root).unwrap_or(&trial.unit).display(), trial.build.target.id);
            match self.outcome(root, trial, text) {
                Err(messages) => Some(format!("{place} does not compile: {}", messages.lines().find(|l| l.contains("error")).unwrap_or(""))),
                Ok((object, _)) if object != trial.baseline.0 => Some(format!("{place}: the object changes")),
                Ok((_, messages)) if self.heard(&messages) != self.heard(&trial.baseline.1) => {
                    let before = self.heard(&trial.baseline.1);
                    let after = self.heard(&messages);
                    let new = after.lines().find(|l| !before.lines().any(|b| b == *l)).unwrap_or("");
                    Some(format!("{place}: a new message: {new}"))
                }
                Ok(_) => None,
            }
        })
    }
}

/// The fewest of `edits` that must stay out for the others to pass, as
/// indices: Zeller's ddmin over the set left out, which starts as all of
/// them (adding nothing passes) and shrinks while a smaller set left out
/// still passes. Edits that match only beside each other stay in together.
pub(crate) fn fewest_out<T: Clone>(edits: &[T], mut passes: impl FnMut(&[T]) -> Result<bool, String>) -> Result<Vec<usize>, String> {
    let mut rest = |out: &[usize]| -> Result<bool, String> {
        let rest: Vec<T> = edits.iter().enumerate().filter(|(i, _)| !out.contains(i)).map(|(_, e)| e.clone()).collect();
        passes(&rest)
    };
    let mut out: Vec<usize> = (0..edits.len()).collect();
    let mut n = 2;
    while out.len() >= 2 {
        let chunks: Vec<Vec<usize>> = out.chunks(out.len().div_ceil(n)).map(<[usize]>::to_vec).collect();
        let mut reduced = false;
        for chunk in &chunks {
            if rest(chunk)? {
                out = chunk.clone();
                n = 2;
                reduced = true;
                break;
            }
        }
        if !reduced && chunks.len() > 2 {
            for chunk in &chunks {
                let others: Vec<usize> = out.iter().copied().filter(|i| !chunk.contains(i)).collect();
                if rest(&others)? {
                    out = others;
                    n = (chunks.len() - 1).max(2);
                    reduced = true;
                    break;
                }
            }
        }
        if !reduced {
            if chunks.len() >= out.len() {
                break;
            }
            n = (2 * chunks.len()).min(out.len());
        }
    }
    Ok(out)
}

/// The edits that pass, applied, and how many there were; none when the
/// required edits (an include the others need) alone fail. `passes` judges
/// a candidate text of the whole file (`Judge::passes`). The required edits
/// are made under every set tried, and kept only beside another edit.
/// Each group holds edits that may stand or fall together (one function's
/// calls): the groups are tried together and bisected, and a failing group
/// is bisected by its edits; then the edits that failed alone are tried
/// again together, less the fewest that must stay out, because some calls
/// match only beside each other.
pub(crate) fn proved_groups(text: &str, required: &[Edit], groups: Vec<Vec<Edit>>, passes: &dyn Fn(&str) -> bool) -> Result<Option<(String, usize)>, String> {
    let candidate = |set: &[Edit]| -> String { apply(text, &[required, set].concat()) };
    if !required.is_empty() && !passes(&candidate(&[])) {
        return Ok(None);
    }
    let groups: Vec<Vec<Edit>> = groups.into_iter().filter(|group| !group.is_empty()).collect();
    let (good, bad) = crate::keep::bisect(&groups, |set: &[Vec<Edit>]| Ok::<bool, String>(passes(&candidate(&set.concat()))))?;
    let mut kept: Vec<Edit> = good.concat();
    for group in bad {
        let base = kept.clone();
        let (alone, failed) = crate::keep::bisect(&group, |set: &[Edit]| Ok::<bool, String>(passes(&candidate(&[&base[..], set].concat()))))?;
        kept.extend(alone);
        if failed.len() >= 2 {
            let base = kept.clone();
            let out = fewest_out(&failed, |rest| Ok(passes(&candidate(&[&base[..], rest].concat()))))?;
            kept.extend(failed.iter().enumerate().filter(|(i, _)| !out.contains(i)).map(|(_, e)| e.clone()));
        }
    }
    if kept.is_empty() {
        return Ok(Some((text.to_string(), 0)));
    }
    Ok(Some((candidate(&kept), kept.len())))
}

/// One edition's compiler for whole-file comparisons outside `proved`.
pub(crate) struct Compiler {
    build: Build,
}

impl Compiler {
    pub(crate) fn new(root: &Path, target: DecompTarget) -> Result<Compiler, String> {
        let imports = crate::build_text::fresh_c_imports(root, target)?;
        let directory = tempfile::Builder::new().prefix("alchemy-trial-").tempdir().map_err(|e| e.to_string())?;
        Ok(Compiler { build: Build { target, imports: imports.map(std::sync::Arc::new), directory } })
    }

    /// The object file of this text in place of the file, or the
    /// compiler's messages when it does not compile.
    pub(crate) fn object(&self, root: &Path, file: &Path, text: &str) -> Result<Vec<u8>, String> {
        self.build.assemble(root, file, text).map(|(object, _)| object)
    }

    /// The object file of this text in place of the file, compiled among
    /// other replaced files of the tree: each a path under the root and
    /// the text read in its place.
    pub(crate) fn object_among(&self, root: &Path, file: &Path, text: &str, others: &[(String, String)]) -> Result<Vec<u8>, String> {
        self.build.assemble_among(root, file, text, others).map(|(object, _)| object)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn cut(start: usize, end: usize) -> Edit {
        Edit { start, end, text: String::new() }
    }

    #[test]
    fn removals_keep_tokens_apart_and_overlaps_go() {
        let text = "a = b -(s16)-c;\nreturn(u8)x;\n";
        assert_eq!(apply(text, &[cut(7, 12), cut(22, 26)]), "a = b - -c;\nreturn x;\n");
        let edits = disjoint(vec![cut(0, 4), cut(2, 6), cut(6, 8)]);
        assert_eq!(edits, vec![cut(0, 4), cut(6, 8)]);
    }

    #[test]
    fn messages_compare_without_line_numbers() {
        let before = "a.c: In function `F':\nA.C:12: warning: unused variable `x'\n";
        let after = "a.c: In function `F':\nA.C:11: warning: unused variable `x'\n";
        assert_eq!(unnumbered(before), unnumbered(after));
        assert_ne!(unnumbered(before), unnumbered("a.c: In function `F':\nA.C:11: warning: unused variable `y'\n"));
        // An include added above moves the chain GCC prints above a
        // header's message; the message itself is the same.
        let before = "In file included from b.h:4,\n                 from a.c:3:\nc.h: In function `G':\nc.h:9: warning: return makes integer from pointer without a cast\n";
        let after = "In file included from b.h:5,\n                 from a.c:4:\nc.h: In function `G':\nc.h:9: warning: return makes integer from pointer without a cast\n";
        assert_eq!(unnumbered(before), unnumbered(after));
        assert_eq!(unnumbered(&before.replace("from a.c:3:", "from a.c:4:")), unnumbered(before));
        let new = format!("{after}a.c:20: warning: unused variable `y'\n");
        assert_ne!(unnumbered(before), unnumbered(&new));
    }

    fn tbs() -> DecompTarget {
        target_for(crate::targets::DecompTargetId::TbsJa)
    }

    /// A tree of its own: `one.c` includes `common/value.h`; `two.c` reaches
    /// it only through `common/staged.h`, which includes it from beside
    /// itself; `three.c` includes neither.
    fn scene_tree() -> (tempfile::TempDir, PathBuf) {
        let folder = tempfile::tempdir().unwrap();
        let root = folder.path().canonicalize().unwrap();
        let scene = root.join("games/tbs/src/scene");
        std::fs::create_dir_all(scene.join("common")).unwrap();
        for (name, text) in [
            ("one.c", "#include \"common/value.h\"\n\nint One(void)\n{\n\treturn VALUE;\n}\n"),
            ("two.c", "#include \"common/staged.h\"\n\nint Two(void)\n{\n\treturn Staged();\n}\n"),
            ("three.c", "int Three(void)\n{\n\treturn 3;\n}\n"),
            ("common/staged.h", "#include \"value.h\"\n\nstatic int Staged(void)\n{\n\treturn VALUE + 1;\n}\n"),
            ("common/value.h", "#define VALUE 2\n"),
        ] {
            std::fs::write(scene.join(name), text).unwrap();
        }
        (folder, root)
    }

    #[test]
    fn a_header_is_included_by_the_files_that_reach_it_through_others() {
        let (_folder, root) = scene_tree();
        let scene = root.join("games/tbs/src/scene");
        assert_eq!(including(&root, &scene.join("common/value.h"), &[tbs()]), [scene.join("one.c"), scene.join("two.c")]);
        assert_eq!(including(&root, &scene.join("common/staged.h"), &[tbs()]), [scene.join("two.c")]);
    }

    /// A header's candidate is compiled in every file that includes it,
    /// through every header on the way: a change to one file's object or
    /// messages fails, and a file that never reads the header refuses the
    /// judge.
    #[test]
    fn a_header_is_judged_in_every_file_that_reaches_it() {
        let (_folder, root) = scene_tree();
        let scene = root.join("games/tbs/src/scene");
        let header = scene.join("common/value.h");
        let text = "#define VALUE 2\n";
        let judge = |units: &[&str]| {
            let units: Vec<PathBuf> = units.iter().map(|unit| scene.join(unit)).collect();
            Judge::new(&root, &header, text, &[tbs()], &units, 4)
        };
        let both = judge(&["one.c", "two.c"]).unwrap();
        assert!(both.passes(&root, "/* two */\n#define VALUE 2\n"));
        assert!(!both.passes(&root, "#define VALUE 3\n"));
        // two.c reads the candidate through staged.h.
        let through = judge(&["two.c"]).unwrap();
        assert!(!through.passes(&root, "#define VALUE 3\n"));
        assert!(!through.passes(&root, &format!("#error the candidate\n{text}")));
        assert!(through.passes(&root, "#define VALUE (1 + 1)\n"));
        // The same object with a new warning fails.
        let one = judge(&["one.c"]).unwrap();
        let warned = "#define VALUE (char *)2\n";
        assert!(!one.passes(&root, warned));
        assert!(one.difference(&root, warned).is_some_and(|why| why.contains("a new message")), "{:?}", one.difference(&root, warned));
        let refused = judge(&["one.c", "three.c"]).err().unwrap_or_default();
        assert!(refused.contains("three.c compiles without reading"), "{refused}");
    }

    /// The messages a quiet header gives about its own lines are not
    /// compared; the file's own still are.
    #[test]
    fn a_quiet_header_warns_only_about_itself() {
        let folder = tempfile::tempdir().unwrap();
        std::fs::write(folder.path().join("noisy.h"), "static inline int Noisy(void)\n{\n\treturn (char *)1;\n}\n").unwrap();
        let file = folder.path().join("scene.c");
        let text = "int F(void)\n{\n\treturn 2;\n}\n";
        let root = crate::compiler::routing::root();
        let mut judge = Judge::new(root, &file, text, &[tbs()], &[], 4).unwrap();
        let included = format!("#include \"noisy.h\"\n{text}");
        assert!(!judge.passes(root, &included));
        judge.quiet(vec!["noisy.h".to_string()]);
        assert!(judge.passes(root, &included));
        assert!(!judge.passes(root, &included.replace("return 2;", "return (char *)2;")));
        // A name that only ends the same way is another header.
        judge.quiet(vec!["isy.h".to_string()]);
        assert!(!judge.passes(root, &included));
    }

    #[test]
    fn replacements_are_spaced_only_when_needed() {
        assert_eq!(apply("x=(y);", &[Edit { start: 2, end: 5, text: "y".into() }]), "x=y;");
        assert_eq!(apply("return(y);", &[Edit { start: 6, end: 9, text: "y".into() }]), "return y;");
    }
}
