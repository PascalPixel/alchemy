//! alchemy dead: what no edition builds or uses.
//!
//! In a matching decomp everything a ROM places stays, called or not; dead
//! means only what none of the twelve editions links, includes or selects.
//! The cheap part reads the linker scripts and include lines (`alchemy check
//! dead` runs it as a hard check); the full part reads the preprocessor's
//! output of every linked C source in all twelve editions.

use crate::c_items::{items, names, Item, Kind};
use crate::targets::{target_for, DecompTarget, TARGET_IDS};
use std::collections::{BTreeMap, BTreeSet, HashMap, HashSet};
use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::Mutex;

const USAGE: &str = "usage: alchemy dead [--check] [--apply PATH...] [--skip PATH...]\n\
  (none)        report unlinked sources, unincluded headers and linker scripts,\n\
                then preprocess every linked C source in all twelve editions\n\
                (after make compare-editions) and report never-selected #if\n\
                arms, never-included headers and header declarations nothing uses\n\
  --check       only the cheap part, failing on any unlinked source or header\n\
  --apply PATH  remove the dead arms, headers and declarations under PATH\n\
                (a file or folder prefix; repeatable)\n\
  --skip PATH   leave files under PATH untouched by --apply (repeatable)\n\
Arms of a chain that tests MODERN count as live. Declarations used only by\n\
recon drafts are reported apart and kept.";

pub fn run(args: &[String]) -> Result<(), String> {
    let mut check = false;
    let mut apply = Vec::new();
    let mut skip = Vec::new();
    let mut args = args.iter();
    while let Some(arg) = args.next() {
        match arg.as_str() {
            "-h" | "--help" => {
                println!("{USAGE}");
                return Ok(());
            }
            "--check" => check = true,
            "--apply" => apply.push(args.next().ok_or(USAGE)?.clone()),
            "--skip" => skip.push(args.next().ok_or(USAGE)?.clone()),
            _ => return Err(USAGE.into()),
        }
    }
    let root = crate::compiler::routing::root();
    let linked = Linked::read(root)?;
    let unlinked = linked.unlinked(root);
    for (kind, paths) in &unlinked {
        for path in paths {
            println!("{kind}\t{path}");
        }
    }
    if check {
        let count: usize = unlinked.iter().map(|(_, paths)| paths.len()).sum();
        if count > 0 {
            return Err(format!(
                "{count} file(s) under games/ no edition links or includes; move their work to recon/ (S4)"
            ));
        }
        return Ok(());
    }
    let report = Report::read(root, &linked)?;
    report.print();
    if !apply.is_empty() {
        let within = |path: &str| {
            apply.iter().any(|prefix| path.starts_with(prefix.as_str()))
                && !skip.iter().any(|prefix| path.starts_with(prefix.as_str()))
        };
        report.apply(root, &within)?;
        // Headers no include line reaches go with the rest; sources stay
        // for their work to move to recon/ first (S4).
        for (kind, paths) in &unlinked {
            for path in paths.iter().filter(|path| *kind == "unincluded-header" && within(path)) {
                fs::remove_file(root.join(path)).map_err(|error| format!("{path}: {error}"))?;
                eprintln!("removed {path}");
            }
        }
    }
    Ok(())
}

/// The cheap hard check: every source and header under games/ is linked by
/// some edition or included by a file that is.
pub fn check_entry(args: &[String]) -> std::process::ExitCode {
    let mut args = args.to_vec();
    args.insert(0, "--check".into());
    match run(&args) {
        Ok(()) => std::process::ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            std::process::ExitCode::FAILURE
        }
    }
}

// ---------------------------------------------------------------------------
// What each edition links

pub(crate) struct Linked {
    /// Every file each edition's link reaches: objects' sources, overlay
    /// scripts and data, and what they include, by raw include lines.
    pub files: Vec<(DecompTarget, BTreeSet<String>)>,
    /// The C sources each edition compiles as translation units.
    pub units: Vec<(DecompTarget, Vec<String>)>,
    /// Headers the assembler or linker includes, which no C unit enters.
    pub assembled: BTreeSet<String>,
}

fn include_pattern() -> regex::Regex {
    regex::Regex::new(r#"(?m)^[ \t]*(?:#[ \t]*include[ \t]*[<"]([^">]+)[">]|\.include[ \t]+"([^"]+)"|INCLUDE[ \t]+"([^"]+)")"#)
        .expect("static pattern")
}

fn overlay_pattern() -> regex::Regex {
    regex::Regex::new(r#"(?m)^[ \t]*\.incbin[ \t]+"overlays/resource_([0-9a-f]+)\.lz""#)
        .expect("static pattern")
}

fn relative(root: &Path, path: &Path) -> String {
    path.strip_prefix(root).unwrap_or(path).to_string_lossy().into_owned()
}

/// Where an include line's name resolves: the including file's folder, the
/// checkout root, then the game's and COMMON's include folders.
fn resolve(root: &Path, target: DecompTarget, from: &str, name: &str) -> Option<String> {
    let mut candidates = vec![
        Path::new(from).parent().unwrap_or(Path::new("")).join(name),
        PathBuf::from(name),
        Path::new(target.game_dir()).join("include").join(name),
        Path::new("games/common/include").join(name),
    ];
    for module in crate::compiler::routing::COMMON_INCLUDE_MODULES {
        candidates.push(Path::new("games/common/include").join(module).join(name));
    }
    candidates
        .into_iter()
        .find(|path| root.join(path).is_file())
        .map(|path| normalize(&path.to_string_lossy()))
}

fn normalize(path: &str) -> String {
    let mut parts: Vec<&str> = Vec::new();
    for part in path.split('/') {
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

/// Headers a tool reads directly: the fixed-address lists (O2).
fn tool_headers(target: DecompTarget) -> Vec<String> {
    ["iwram_call.h", "ram_buffer.h", "cartboot.h"]
        .iter()
        .map(|name| format!("{}/include/{name}", target.game_dir()))
        .collect()
}

impl Linked {
    pub(crate) fn read(root: &Path) -> Result<Linked, String> {
        let includes = include_pattern();
        let overlays = overlay_pattern();
        let mut files = Vec::new();
        let mut units = Vec::new();
        let mut assembled = BTreeSet::new();
        for id in TARGET_IDS {
            let target = target_for(id);
            let main = target.script();
            let statements =
                crate::edition::linked_statements(root, target, crate::edition::Image::Main)?;
            let mut reached = BTreeSet::new();
            let mut scripts = vec![normalize(&main.to_string_lossy())];
            if let Some(edition) = target.edition_script() {
                scripts.push(normalize(&edition.to_string_lossy()));
                scripts.push(normalize(
                    &crate::targets::edition_script_path(
                        target.id.as_str().split_once('-').unwrap().0,
                        "en",
                    )
                    .to_string_lossy(),
                ));
            }
            // A game with an object list links by it and the one script.
            if let Some(list) = target.object_list() {
                scripts.push(normalize(&list.to_string_lossy()));
            }
            let mut queue = scripts;
            let mut compiled = Vec::new();
            for object in crate::edition::built_objects(&statements) {
                let source = crate::build_rom::source_for(root, &object)?;
                queue.push(normalize(&source.to_string_lossy()));
            }
            for header in tool_headers(target) {
                if root.join(&header).is_file() {
                    queue.push(header);
                }
            }
            let mut direct: BTreeSet<String> = queue.iter().cloned().collect();
            while let Some(file) = queue.pop() {
                if !reached.insert(file.clone()) {
                    continue;
                }
                let Ok(bytes) = fs::read(root.join(&file)) else {
                    continue;
                };
                let text = String::from_utf8_lossy(&bytes);
                if file.ends_with(".c")
                    && direct.contains(&file) {
                        compiled.push(file.clone());
                    }
                if !(file.ends_with(".c")
                    || file.ends_with(".h")
                    || file.ends_with(".S")
                    || file.ends_with(".s")
                    || file.ends_with(".inc")
                    || file.ends_with(".ld"))
                {
                    continue;
                }
                for capture in includes.captures_iter(&text) {
                    let name = (1..=3).find_map(|group| capture.get(group)).unwrap().as_str();
                    if let Some(path) = resolve(root, target, &file, name) {
                        if !(file.ends_with(".c") || file.ends_with(".h")) {
                            assembled.insert(path.clone());
                        }
                        queue.push(path);
                    }
                }
                for capture in overlays.captures_iter(&text) {
                    let id = &capture[1];
                    let script = target.overlay_script(id);
                    if !root.join(&script).is_file() {
                        continue;
                    }
                    let statements = crate::edition::linked_statements(
                        root,
                        target,
                        crate::edition::Image::Overlay(id),
                    )?;
                    queue.push(normalize(&script.to_string_lossy()));
                    let own = format!("resource_{id}_overlay");
                    for object in crate::edition::built_objects(&statements) {
                        if object == own {
                            continue;
                        }
                        let source =
                            normalize(&crate::build_rom::source_for(root, &object)?.to_string_lossy());
                        direct.insert(source.clone());
                        queue.push(source);
                    }
                    let data = target.overlay_data(id);
                    if root.join(&data).is_file() {
                        queue.push(normalize(&data.to_string_lossy()));
                    }
                }
            }
            compiled.sort();
            files.push((target, reached));
            units.push((target, compiled));
        }
        Ok(Linked { files, units, assembled })
    }

    pub(crate) fn reached(&self) -> BTreeSet<&str> {
        self.files
            .iter()
            .flat_map(|(_, files)| files.iter().map(String::as_str))
            .collect()
    }

    /// Files under games/ no edition reaches, by kind.
    pub(crate) fn unlinked(&self, root: &Path) -> Vec<(&'static str, Vec<String>)> {
        let reached = self.reached();
        let mut sources = Vec::new();
        let mut headers = Vec::new();
        let mut scripts = Vec::new();
        for entry in walkdir::WalkDir::new(root.join("games")).sort_by_file_name() {
            let Ok(entry) = entry else { continue };
            if !entry.file_type().is_file() {
                continue;
            }
            let path = relative(root, entry.path());
            if reached.contains(path.as_str()) {
                continue;
            }
            match path.rsplit('.').next().unwrap_or("") {
                "c" | "s" | "S" => sources.push(path),
                "h" | "inc" => headers.push(path),
                "ld" => scripts.push(path),
                _ => {}
            }
        }
        vec![
            ("unlinked-source", sources),
            ("unincluded-header", headers),
            ("unlinked-script", scripts),
        ]
    }
}

// ---------------------------------------------------------------------------
// The preprocessor's view
//
// Every arm of every C source and header is marked with a token of its own
// on a new line after its directive, in a copy of games/; an arm is live
// when its token reaches some edition's preprocessed output, and a header
// is used when the output's line markers enter it.

const MARK: &str = "ALCHEMY_DEAD_ARM_";

/// What the preprocessor emitted, over every unit of every edition.
#[derive(Default)]
struct Live {
    arms: HashSet<usize>,
    files: HashSet<String>,
}

impl Live {
    fn merge(&mut self, other: Live) {
        self.arms.extend(other.arms);
        self.files.extend(other.files);
    }
}

fn scan(output: &str, copy: &str) -> Live {
    let mut live = Live::default();
    for row in output.lines() {
        if let Some(marker) = row.strip_prefix("# ") {
            if let Some((_, rest)) = marker.split_once(" \"") {
                if let Some((name, _)) = rest.split_once('"') {
                    let name = name.strip_prefix(copy).unwrap_or(name).trim_start_matches('/');
                    live.files.insert(normalize(name));
                }
            }
            continue;
        }
        let mut rest = row;
        while let Some(at) = rest.find(MARK) {
            rest = &rest[at + MARK.len()..];
            let digits = rest.find(|c: char| !c.is_ascii_digit()).unwrap_or(rest.len());
            if let Ok(id) = rest[..digits].parse() {
                live.arms.insert(id);
            }
        }
    }
    live
}

/// The arms of every C file and header under games/, and a marked copy of
/// the checkout to preprocess them in.
struct Marked {
    copy: tempfile::TempDir,
    texts: BTreeMap<String, Text>,
    chains: BTreeMap<String, Vec<Chain>>,
    /// Marker id -> (file, chain, arm).
    ids: Vec<(String, usize, usize)>,
}

impl Marked {
    fn new(root: &Path) -> Result<Marked, String> {
        let copy = tempfile::Builder::new()
            .prefix("alchemy-dead-")
            .tempdir_in(root.join("out"))
            .map_err(|error| error.to_string())?;
        for entry in fs::read_dir(root).map_err(|error| error.to_string())? {
            let entry = entry.map_err(|error| error.to_string())?;
            let name = entry.file_name();
            if name == "games" || name == "out" {
                continue;
            }
            std::os::unix::fs::symlink(entry.path(), copy.path().join(&name))
                .map_err(|error| error.to_string())?;
        }
        let mut texts = BTreeMap::new();
        let mut all = BTreeMap::new();
        let mut ids = Vec::new();
        for entry in walkdir::WalkDir::new(root.join("games")).sort_by_file_name() {
            let entry = entry.map_err(|error| error.to_string())?;
            let path = relative(root, entry.path());
            let target = copy.path().join(&path);
            if entry.file_type().is_dir() {
                fs::create_dir_all(&target).map_err(|error| error.to_string())?;
                continue;
            }
            let extension = path.rsplit('.').next().unwrap_or("");
            if !matches!(extension, "c" | "h" | "inc") {
                std::os::unix::fs::symlink(entry.path(), &target).map_err(|error| error.to_string())?;
                continue;
            }
            let bytes = fs::read(entry.path()).map_err(|error| format!("{path}: {error}"))?;
            let text = Text::new(String::from_utf8_lossy(&bytes).into_owned());
            let found = chains(&text);
            let mut after: BTreeMap<u32, Vec<usize>> = BTreeMap::new();
            for (number, chain) in found.iter().enumerate() {
                for (arm_number, arm) in chain.arms.iter().enumerate() {
                    after.entry(arm.body.0 - 1).or_default().push(ids.len());
                    ids.push((path.clone(), number, arm_number));
                }
            }
            let mut marked = Vec::with_capacity(bytes.len() + after.len() * 32);
            for (index, row) in bytes.split_inclusive(|byte| *byte == b'\n').enumerate() {
                marked.extend_from_slice(row);
                if let Some(list) = after.get(&(index as u32 + 1)) {
                    if !row.ends_with(b"\n") {
                        marked.push(b'\n');
                    }
                    for id in list {
                        marked.extend_from_slice(format!("{MARK}{id}\n").as_bytes());
                    }
                }
            }
            fs::write(&target, marked).map_err(|error| format!("{path}: {error}"))?;
            texts.insert(path.clone(), text);
            all.insert(path, found);
        }
        Ok(Marked { copy, texts, chains: all, ids })
    }

    fn preprocess(&self, root: &Path, target: DecompTarget, source: &str) -> Result<Live, String> {
        let real = root.to_string_lossy().into_owned();
        let copy = self.copy.path().to_string_lossy().into_owned();
        let mut command = crate::compiler::preprocess::command(target, source, false)?;
        for argument in command.iter_mut().skip(1) {
            if argument.contains(&real) && argument.contains("/games/") {
                *argument = argument.replacen(&real, &copy, 1);
            }
        }
        command.insert(1, format!("-I{}", root.join(target.output_dir).display()));
        command.push(source.into());
        let output = Command::new(&command[0])
            .args(&command[1..])
            .current_dir(self.copy.path())
            .output()
            .map_err(|error| format!("{source}: {error}"))?;
        if !output.status.success() {
            return Err(format!(
                "{} {source}: {}",
                target.id,
                String::from_utf8_lossy(&output.stderr).trim()
            ));
        }
        Ok(scan(&String::from_utf8_lossy(&output.stdout), &copy))
    }
}
// ---------------------------------------------------------------------------
// Source text: code without comments, line by line, and conditional chains

struct Text {
    raw: String,
    /// Each physical line with comments and literals blanked.
    code: Vec<String>,
    /// Byte offset of each line's start in raw.
    starts: Vec<usize>,
}

impl Text {
    fn new(raw: String) -> Text {
        let mut code = String::with_capacity(raw.len());
        let bytes = raw.as_bytes();
        let mut i = 0;
        while i < bytes.len() {
            if bytes[i..].starts_with(b"/*") {
                let end = raw[i + 2..].find("*/").map_or(raw.len(), |e| i + 2 + e + 2);
                for &byte in &bytes[i..end] {
                    code.push(if byte == b'\n' { '\n' } else { ' ' });
                }
                i = end;
            } else if bytes[i..].starts_with(b"//") {
                let end = raw[i..].find('\n').map_or(raw.len(), |e| i + e);
                code.extend(std::iter::repeat_n(' ', end - i));
                i = end;
            } else if bytes[i] == b'"' || bytes[i] == b'\'' {
                let quote = bytes[i];
                let mut j = i + 1;
                while j < bytes.len() && bytes[j] != quote && bytes[j] != b'\n' {
                    j += if bytes[j] == b'\\' { 2 } else { 1 };
                }
                let end = (j + 1).min(bytes.len());
                code.push(quote as char);
                code.extend(std::iter::repeat_n(' ', end.saturating_sub(i + 2)));
                if end > i + 1 {
                    code.push(if bytes[end - 1] == b'\n' { '\n' } else { quote as char });
                }
                i = end;
            } else {
                let width = raw[i..].chars().next().map_or(1, char::len_utf8);
                code.push(if bytes[i] < 0x80 { bytes[i] as char } else { ' ' });
                i += width;
            }
        }
        let mut starts = vec![0];
        for (at, byte) in bytes.iter().enumerate() {
            if *byte == b'\n' {
                starts.push(at + 1);
            }
        }
        if starts.last() == Some(&bytes.len()) && !bytes.is_empty() {
            starts.pop();
        }
        Text { code: code.lines().map(str::to_owned).collect(), raw, starts }
    }

    fn line_of(&self, offset: usize) -> u32 {
        (self.starts.partition_point(|start| *start <= offset)) as u32
    }

    /// The directive on a 1-based line, with continuation lines joined, and
    /// the last line it spans.
    fn directive(&self, line: u32) -> Option<(String, String, u32)> {
        let text = self.code.get(line as usize - 1)?;
        let rest = text.trim_start().strip_prefix('#')?;
        let mut joined = rest.to_string();
        let mut last = line;
        while joined.trim_end().ends_with('\\') {
            let trimmed = joined.trim_end();
            joined = trimmed[..trimmed.len() - 1].to_string();
            last += 1;
            match self.code.get(last as usize - 1) {
                Some(next) => joined.push_str(next),
                None => break,
            }
        }
        let joined = joined.trim_start();
        let end = joined
            .find(|c: char| !c.is_ascii_alphanumeric() && c != '_')
            .unwrap_or(joined.len());
        Some((joined[..end].to_string(), joined[end..].trim().to_string(), last))
    }
}

#[derive(Clone, Debug)]
struct Arm {
    /// The line of its directive (#if, #elif or #else).
    line: u32,
    directive: String,
    condition: String,
    /// Lines of its body, exclusive of the next directive.
    body: (u32, u32),
    children: Vec<usize>,
}

#[derive(Clone, Debug)]
struct Chain {
    arms: Vec<Arm>,
    endif: u32,
    parent: Option<(usize, usize)>,
}

fn chains(text: &Text) -> Vec<Chain> {
    let mut chains: Vec<Chain> = Vec::new();
    let mut open: Vec<usize> = Vec::new();
    let mut line = 1u32;
    let count = text.code.len() as u32;
    while line <= count {
        let Some((name, argument, last)) = text.directive(line) else {
            line += 1;
            continue;
        };
        match name.as_str() {
            "if" | "ifdef" | "ifndef" => {
                let parent = open.last().map(|&chain| (chain, chains[chain].arms.len() - 1));
                let index = chains.len();
                if let Some((chain, arm)) = parent {
                    chains[chain].arms[arm].children.push(index);
                }
                chains.push(Chain {
                    arms: vec![Arm {
                        line,
                        directive: name,
                        condition: argument,
                        body: (last + 1, last),
                        children: Vec::new(),
                    }],
                    endif: 0,
                    parent,
                });
                open.push(index);
            }
            "elif" | "else" => {
                if let Some(&chain) = open.last() {
                    let arms = &mut chains[chain].arms;
                    arms.last_mut().unwrap().body.1 = line - 1;
                    arms.push(Arm {
                        line,
                        directive: name,
                        condition: argument,
                        body: (last + 1, last),
                        children: Vec::new(),
                    });
                }
            }
            "endif" => {
                if let Some(chain) = open.pop() {
                    chains[chain].arms.last_mut().unwrap().body.1 = line - 1;
                    chains[chain].endif = line;
                }
            }
            _ => {}
        }
        line = last + 1;
    }
    chains.retain(|chain| chain.endif != 0);
    chains
}

/// An arm's verdict: live when the preprocessor emitted one of its own
/// lines or a nested arm is live; None when it holds nothing to emit.
/// An arm that only stops the build (#error) guards against a configuration
/// no edition has; it is not dead code.
fn only_error(text: &Text, arm: &Arm) -> bool {
    let mut line = arm.body.0;
    let mut error = false;
    while line <= arm.body.1 {
        let code = text.code.get(line as usize - 1).map_or("", String::as_str);
        if let Some((name, _, last)) = text.directive(line) {
            if name != "error" && name != "warning" {
                return false;
            }
            error = true;
            line = last + 1;
            continue;
        }
        if !code.trim().is_empty() {
            return false;
        }
        line += 1;
    }
    error
}

fn tests_modern(chain: &Chain) -> bool {
    chain
        .arms
        .iter()
        .any(|arm| identifiers(&arm.condition).any(|word| word == "MODERN"))
}

fn identifiers(text: &str) -> impl Iterator<Item = &str> {
    text.split(|c: char| !c.is_ascii_alphanumeric() && c != '_')
        .filter(|word| word.starts_with(|c: char| c.is_ascii_alphabetic() || c == '_'))
}

// ---------------------------------------------------------------------------
// The report

#[derive(Clone, Debug)]
struct DeadArm {
    file: String,
    chain: usize,
    arm: usize,
    line: u32,
    directive: String,
    condition: String,
}

#[derive(Clone, Debug)]
struct Declaration {
    file: String,
    lines: (u32, u32),
    names: Vec<String>,
    describe: String,
}

struct Report {
    texts: BTreeMap<String, Text>,
    chains: BTreeMap<String, Vec<Chain>>,
    arms: Vec<DeadArm>,
    headers: Vec<String>,
    declarations: Vec<Declaration>,
    drafts_only: Vec<Declaration>,
    definitions: Vec<Declaration>,
}

impl Report {
    fn read(root: &Path, linked: &Linked) -> Result<Report, String> {
        for (target, _) in &linked.units {
            if !root.join(target.output_dir).is_dir() {
                return Err(format!(
                    "{} is not built: run make compare-editions first",
                    target.id
                ));
            }
        }
        let marked = Marked::new(root)?;
        let jobs: Vec<(DecompTarget, &String)> = linked
            .units
            .iter()
            .flat_map(|(target, units)| units.iter().map(move |unit| (*target, unit)))
            .collect();
        let live = Mutex::new(Live::default());
        let errors = Mutex::new(Vec::new());
        let next = AtomicUsize::new(0);
        let workers = std::thread::available_parallelism().map_or(4, |count| count.get());
        std::thread::scope(|scope| {
            for _ in 0..workers {
                scope.spawn(|| {
                    let mut own = Live::default();
                    loop {
                        let index = next.fetch_add(1, Ordering::Relaxed);
                        let Some((target, unit)) = jobs.get(index) else { break };
                        match marked.preprocess(root, *target, unit) {
                            Ok(found) => own.merge(found),
                            Err(error) => errors.lock().unwrap().push(error),
                        }
                    }
                    live.lock().unwrap().merge(own);
                });
            }
        });
        let errors = errors.into_inner().unwrap();
        if !errors.is_empty() {
            return Err(errors.join("\n"));
        }
        let live = live.into_inner().unwrap();
        eprintln!("preprocessed {} translation units", jobs.len());

        // Headers: reached by a raw include line but entered by no unit.
        let reached = linked.reached();
        let tool: BTreeSet<String> = linked
            .units
            .iter()
            .flat_map(|(target, _)| tool_headers(*target))
            .collect();
        let headers: Vec<String> = reached
            .iter()
            .filter(|file| file.starts_with("games/") && file.ends_with(".h"))
            .filter(|file| !live.files.contains(**file) && !tool.contains(**file) && !linked.assembled.contains(**file))
            .map(|file| file.to_string())
            .collect();

        // Arms of every file a unit entered.
        let mut verdict: HashMap<(&str, usize, usize), bool> = HashMap::new();
        for (id, (file, chain, arm)) in marked.ids.iter().enumerate() {
            verdict.insert((file.as_str(), *chain, *arm), live.arms.contains(&id));
        }
        let mut texts = BTreeMap::new();
        let mut all_chains = BTreeMap::new();
        let mut arms = Vec::new();
        for (file, found) in &marked.chains {
            if !live.files.contains(file) {
                continue;
            }
            let text = &marked.texts[file];
            for (index, chain) in found.iter().enumerate() {
                if tests_modern(chain) || inside_modern(found, chain) {
                    continue;
                }
                // A chain inside a dead arm is reported through that arm.
                if let Some((parent, arm)) = chain.parent {
                    if verdict.get(&(file.as_str(), parent, arm)) == Some(&false) {
                        continue;
                    }
                }
                for (number, arm) in chain.arms.iter().enumerate() {
                    if verdict[&(file.as_str(), index, number)] || only_error(text, arm) {
                        continue;
                    }
                    arms.push(DeadArm {
                        file: file.clone(),
                        chain: index,
                        arm: number,
                        line: arm.line,
                        directive: arm.directive.clone(),
                        condition: arm.condition.clone(),
                    });
                }
            }
        }
        let Marked { texts: marked_texts, chains: marked_chains, .. } = marked;
        for (file, text) in marked_texts {
            if live.files.contains(&file) {
                texts.insert(file.clone(), text);
            }
        }
        for (file, found) in marked_chains {
            if live.files.contains(&file) {
                all_chains.insert(file, found);
            }
        }

        let (declarations, drafts_only, definitions) =
            declarations(root, linked, &texts, &all_chains, &arms)?;
        Ok(Report {
            texts,
            chains: all_chains,
            arms,
            headers,
            declarations,
            drafts_only,
            definitions,
        })
    }

    fn print(&self) {
        for arm in &self.arms {
            println!(
                "dead-arm\t{}:{}\t#{} {}",
                arm.file, arm.line, arm.directive, arm.condition
            );
        }
        for header in &self.headers {
            println!("dead-header\t{header}");
        }
        for (kind, list) in [
            ("dead-declaration", &self.declarations),
            ("drafts-only-declaration", &self.drafts_only),
            ("kept-unused", &self.definitions),
        ] {
            for declaration in list {
                println!(
                    "{kind}\t{}:{}\t{}",
                    declaration.file, declaration.lines.0, declaration.describe
                );
            }
        }
        let mut counts: BTreeMap<&str, usize> = BTreeMap::new();
        counts.insert("dead-arm", self.arms.len());
        counts.insert("dead-header", self.headers.len());
        counts.insert("dead-declaration", self.declarations.len());
        counts.insert("drafts-only-declaration", self.drafts_only.len());
        counts.insert("kept-unused", self.definitions.len());
        let summary: Vec<String> = counts.iter().map(|(k, v)| format!("{k}={v}")).collect();
        eprintln!("{}", summary.join(" "));
    }

    fn apply(&self, root: &Path, within: &dyn Fn(&str) -> bool) -> Result<(), String> {
        let mut edits: BTreeMap<&str, Vec<Edit>> = BTreeMap::new();
        let mut by_chain: BTreeMap<(&str, usize), Vec<usize>> = BTreeMap::new();
        for arm in &self.arms {
            if within(&arm.file) && !self.headers.contains(&arm.file) {
                by_chain.entry((arm.file.as_str(), arm.chain)).or_default().push(arm.arm);
            }
        }
        for ((file, chain), dead) in &by_chain {
            let chain = &self.chains[*file][*chain];
            let text = &self.texts[*file];
            edits.entry(file).or_default().extend(chain_edits(text, chain, dead));
        }
        for declaration in &self.declarations {
            if within(&declaration.file) && !self.headers.contains(&declaration.file) {
                let text = &self.texts[declaration.file.as_str()];
                edits
                    .entry(declaration.file.as_str())
                    .or_default()
                    .push(Edit::Remove(attached_comment(text, declaration.lines.0), declaration.lines.1));
            }
        }
        let mut changed = 0;
        for (file, edits) in edits {
            let text = &self.texts[file];
            let rewritten = rewrite(text, edits)?;
            fs::write(root.join(file), rewritten).map_err(|error| format!("{file}: {error}"))?;
            changed += 1;
        }
        let mut removed = 0;
        for header in &self.headers {
            if within(header) {
                fs::remove_file(root.join(header)).map_err(|error| format!("{header}: {error}"))?;
                removed += 1;
            }
        }
        eprintln!("rewrote {changed} file(s), removed {removed} header(s)");
        Ok(())
    }
}

fn inside_modern(chains: &[Chain], chain: &Chain) -> bool {
    let mut parent = chain.parent;
    while let Some((index, _)) = parent {
        if tests_modern(&chains[index]) {
            return true;
        }
        parent = chains[index].parent;
    }
    false
}

// ---------------------------------------------------------------------------
// Declarations

type Declarations = (Vec<Declaration>, Vec<Declaration>, Vec<Declaration>);

fn declarations(
    root: &Path,
    linked: &Linked,
    texts: &BTreeMap<String, Text>,
    chains: &BTreeMap<String, Vec<Chain>>,
    dead_arms: &[DeadArm],
) -> Result<Declarations, String> {
    // Lines removed by dead arms do not use anything.
    let mut dead_lines: HashMap<&str, Vec<(u32, u32)>> = HashMap::new();
    for arm in dead_arms {
        let chain = &chains[&arm.file][arm.chain];
        let body = chain.arms[arm.arm].body;
        dead_lines.entry(arm.file.as_str()).or_default().push(body);
    }
    let is_dead_line = |file: &str, line: u32| {
        dead_lines
            .get(file)
            .is_some_and(|ranges| ranges.iter().any(|(a, b)| (*a..=*b).contains(&line)))
    };

    // Candidates: each declaration item of a header a unit entered.
    struct Candidate {
        declaration: Declaration,
        definition: bool,
        /// For a macro, the first line of its block of non-blank lines.
        block: Option<u32>,
    }
    let mut candidates: Vec<Candidate> = Vec::new();
    // The fixed-address headers are read by the build itself (O2).
    let tool: HashSet<String> =
        linked.units.iter().flat_map(|(target, _)| tool_headers(*target)).collect();
    for (file, text) in texts {
        if !file.ends_with(".h") || tool.contains(file) {
            continue;
        }
        let blanked = blank_conditionals(text);
        for item in items(&blanked) {
            let found = names(&item);
            if found.is_empty() || guard_invocation(&item) || constraint_typedef(&item) {
                continue;
            }
            let first = text.line_of(item.start);
            let last = text.line_of(item.end.saturating_sub(1).max(item.start));
            if is_dead_line(file, first) {
                continue;
            }
            let definition = match &item.kind {
                Kind::Function { .. } => !is_inline(&item),
                Kind::Declaration => found.iter().any(|name| {
                    name.space == crate::c_items::Space::Ordinary
                        && name.defines
                        && !is_typedef(&item)
                        && !is_enumerator(&item, &name.name)
                }),
                _ => false,
            };
            let block = (item.kind == Kind::Directive).then(|| block_start(text, first));
            let describe = found.iter().map(|name| name.describe()).collect::<Vec<_>>().join(", ");
            candidates.push(Candidate {
                declaration: Declaration {
                    file: file.clone(),
                    lines: (first, last),
                    names: found.into_iter().map(|name| name.name).collect(),
                    describe,
                },
                definition,
                block,
            });
        }
    }
    let wanted: HashSet<&str> = candidates
        .iter()
        .flat_map(|candidate| candidate.declaration.names.iter().map(String::as_str))
        .collect();

    // Occurrences: live C lines, every linked assembly file and script.
    let mut uses: HashMap<String, Vec<(String, u32)>> = HashMap::new();
    let mut record = |word: &str, file: &str, line: u32| {
        if wanted.contains(word) {
            uses.entry(word.to_string()).or_default().push((file.to_string(), line));
        }
    };
    for (file, text) in texts {
        for (index, code) in text.code.iter().enumerate() {
            let line = index as u32 + 1;
            if is_dead_line(file, line) {
                continue;
            }
            for word in identifiers(code) {
                record(word, file, line);
            }
        }
    }
    let reached = linked.reached();
    for file in &reached {
        let assembly = file.ends_with(".S") || file.ends_with(".s") || file.ends_with(".ld")
            || file.ends_with(".inc");
        if !assembly {
            continue;
        }
        let Ok(bytes) = fs::read(root.join(file)) else { continue };
        let text = String::from_utf8_lossy(&bytes);
        for (index, row) in text.lines().enumerate() {
            for word in identifiers(row) {
                record(word, file, index as u32 + 1);
            }
        }
    }
    // Drafts: kept as work (S4); what only they use is reported apart.
    let mut draft_words: HashSet<String> = HashSet::new();
    for entry in walkdir::WalkDir::new(root.join("recon")) {
        let Ok(entry) = entry else { continue };
        let path = entry.path();
        if path.extension().is_some_and(|ext| ext == "c" || ext == "h") {
            if let Ok(bytes) = fs::read(path) {
                let text = String::from_utf8_lossy(&bytes);
                for word in identifiers(&text) {
                    if wanted.contains(word) {
                        draft_words.insert(word.to_string());
                    }
                }
            }
        }
    }

    // A use inside a dead candidate does not count: iterate to a fixpoint.
    let mut dead = vec![false; candidates.len()];
    let mut ranges: HashMap<&str, Vec<(usize, u32, u32)>> = HashMap::new();
    for (index, candidate) in candidates.iter().enumerate() {
        let d = &candidate.declaration;
        ranges.entry(d.file.as_str()).or_default().push((index, d.lines.0, d.lines.1));
    }
    loop {
        let mut changed = false;
        for (index, candidate) in candidates.iter().enumerate() {
            if dead[index] || candidate.definition {
                continue;
            }
            let d = &candidate.declaration;
            let used = d.names.iter().any(|name| {
                uses.get(name).is_some_and(|sites| {
                    sites.iter().any(|(file, line)| {
                        let inside = ranges.get(file.as_str()).is_some_and(|ranges| {
                            ranges.iter().any(|(other, a, b)| {
                                (*a..=*b).contains(line) && (*other == index || dead[*other])
                            })
                        });
                        !inside
                    })
                })
            });
            if !used {
                dead[index] = true;
                changed = true;
            }
        }
        if !changed {
            break;
        }
    }
    // A macro of a block whose other members are used is part of a
    // vocabulary (opcodes, characters, registers): it stays, as pret keeps
    // whole constant lists.
    let live_blocks: HashSet<(&str, u32)> = candidates
        .iter()
        .enumerate()
        .filter(|(index, candidate)| !dead[*index] && candidate.block.is_some())
        .map(|(_, candidate)| (candidate.declaration.file.as_str(), candidate.block.unwrap()))
        .collect();
    let vocabulary: Vec<bool> = candidates
        .iter()
        .map(|candidate| {
            candidate.block.is_some_and(|block| {
                live_blocks.contains(&(candidate.declaration.file.as_str(), block))
            })
        })
        .collect();
    let mut out = (Vec::new(), Vec::new(), Vec::new());
    for (index, candidate) in candidates.into_iter().enumerate() {
        let drafts = candidate.declaration.names.iter().any(|name| draft_words.contains(name));
        if candidate.definition {
            let d = &candidate.declaration;
            let used = d.names.iter().any(|name| {
                uses.get(name).is_some_and(|sites| {
                    sites.iter().any(|(file, line)| {
                        !(file == &d.file && (d.lines.0..=d.lines.1).contains(line))
                    })
                })
            });
            if !used {
                let mut declaration = candidate.declaration;
                declaration.describe = format!("definition {}", declaration.describe);
                out.2.push(declaration);
            }
        } else if dead[index] {
            if vocabulary[index] {
                let mut declaration = candidate.declaration;
                declaration.describe = format!("vocabulary {}", declaration.describe);
                out.2.push(declaration);
            } else if drafts {
                out.1.push(candidate.declaration);
            } else {
                out.0.push(candidate.declaration);
            }
        }
    }
    Ok(out)
}

/// The first line of the run of non-blank lines that holds `line`.
fn block_start(text: &Text, line: u32) -> u32 {
    let mut start = line;
    while start > 1 && !text.raw_line(start - 1).trim().is_empty() {
        start -= 1;
    }
    start
}

/// A top-level macro invocation (LAYOUT_OFFSET_GUARD and the like) is a
/// compile-time check, not a declaration anything has to use.
fn guard_invocation(item: &Item) -> bool {
    let text = item.text.trim_start();
    let end = text.find(|c: char| !c.is_ascii_alphanumeric() && c != '_').unwrap_or(0);
    end > 0
        && text[..end].bytes().all(|b| b.is_ascii_uppercase() || b.is_ascii_digit() || b == b'_')
        && text[end..].trim_start().starts_with('(')
}

fn is_inline(item: &Item) -> bool {
    let head = &item.text[..item.text.find('{').unwrap_or(item.text.len())];
    identifiers(head).any(|word| word == "inline" || word == "__inline__" || word == "__inline")
}

fn is_typedef(item: &Item) -> bool {
    identifiers(item.text).any(|word| word == "typedef")
}

/// Conditional array bounds in a typedef can reject a bad layout even when
/// nothing uses the typedef's name. Keep that check independent of formatting.
fn constraint_typedef(item: &Item) -> bool {
    if !is_typedef(item) {
        return false;
    }
    let Ok(tokens) = crate::compiler::c_syntax::lex(item.text) else {
        return true;
    };
    let mut depth = 0;
    for token in tokens {
        match token.tok {
            crate::compiler::c_syntax::Tok::Punct("[") => depth += 1,
            crate::compiler::c_syntax::Tok::Punct("]") => depth -= 1,
            crate::compiler::c_syntax::Tok::Punct("?") if depth > 0 => return true,
            _ => {}
        }
    }
    false
}

fn is_enumerator(item: &Item, name: &str) -> bool {
    identifiers(item.text).any(|word| word == "enum") && {
        // An enumerator is named inside the braces.
        let open = item.text.find('{').unwrap_or(item.text.len());
        let close = item.text.rfind('}').unwrap_or(0);
        open < close && identifiers(&item.text[open..close]).any(|word| word == name)
    }
}

/// The header with its conditional directives blanked, so that the items
/// inside an #if read as items of their own at their own offsets.
fn blank_conditionals(text: &Text) -> String {
    let mut out = String::with_capacity(text.raw.len());
    for (index, row) in text.raw.split_inclusive('\n').enumerate() {
        let line = index as u32 + 1;
        let conditional = text.directive(line).is_some_and(|(name, _, _)| {
            matches!(name.as_str(), "if" | "ifdef" | "ifndef" | "elif" | "else" | "endif")
        });
        if conditional {
            for byte in row.bytes() {
                out.push(if byte == b'\n' { '\n' } else { ' ' });
            }
            // Keep byte offsets: multibyte characters blank to one space
            // each byte above, which pushes the same count.
        } else {
            out.push_str(row);
        }
    }
    out
}

// ---------------------------------------------------------------------------
// Rewriting

#[derive(Debug, Clone, PartialEq)]
enum Edit {
    /// Remove lines a..=b.
    Remove(u32, u32),
    /// Replace line a with text.
    Replace(u32, String),
}

/// The edits that drop a chain's dead arms while every edition that reaches
/// it selects the same lines it did.
fn chain_edits(text: &Text, chain: &Chain, dead: &[usize]) -> Vec<Edit> {
    let kept: Vec<usize> = (0..chain.arms.len()).filter(|arm| !dead.contains(arm)).collect();
    let directive_end = |line: u32| text.directive(line).map_or(line, |(_, _, last)| last);
    if kept.is_empty() {
        return vec![Edit::Remove(chain.arms[0].line, chain.endif)];
    }
    let mut edits = Vec::new();
    for &arm in dead {
        let start = chain.arms[arm].line;
        let end = chain.arms[arm].body.1;
        edits.push(Edit::Remove(start, end));
    }
    let first = &chain.arms[kept[0]];
    if kept[0] != 0 {
        if first.directive == "else" {
            // Every arm before it was never taken: it always is.
            edits.push(Edit::Remove(first.line, directive_end(first.line)));
            edits.push(Edit::Remove(chain.endif, chain.endif));
        } else {
            let row = &text.raw[text.starts[first.line as usize - 1]..];
            let row = &row[..row.find('\n').unwrap_or(row.len())];
            edits.push(Edit::Replace(first.line, row.replacen("elif", "if", 1)));
        }
    }
    edits
}

/// A comment alone on the lines directly above a removed declaration goes
/// with it.
fn attached_comment(text: &Text, first: u32) -> u32 {
    let mut line = first;
    while line > 1 {
        let above = &text.raw_line(line - 1);
        let trimmed = above.trim();
        let comment = trimmed.starts_with("//")
            || (trimmed.starts_with("/*") && trimmed.ends_with("*/") && !trimmed.contains("FAKEMATCH"));
        if !comment {
            break;
        }
        line -= 1;
    }
    line
}

impl Text {
    fn raw_line(&self, line: u32) -> &str {
        let start = self.starts[line as usize - 1];
        let rest = &self.raw[start..];
        &rest[..rest.find('\n').unwrap_or(rest.len())]
    }
}

fn rewrite(text: &Text, edits: Vec<Edit>) -> Result<String, String> {
    let mut removed = BTreeSet::new();
    let mut replaced = BTreeMap::new();
    for edit in edits {
        match edit {
            Edit::Remove(a, b) => removed.extend(a..=b),
            Edit::Replace(line, row) => {
                replaced.insert(line, row);
            }
        }
    }
    let mut out = String::with_capacity(text.raw.len());
    let mut blank_run = 0;
    let mut previous_removed = false;
    for (index, row) in text.raw.split_inclusive('\n').enumerate() {
        let line = index as u32 + 1;
        if removed.contains(&line) {
            previous_removed = true;
            continue;
        }
        let blank = row.trim().is_empty();
        // Removing a block leaves no double blank line behind.
        if blank && previous_removed && blank_run > 0 {
            continue;
        }
        if blank && previous_removed && out.is_empty() {
            continue;
        }
        previous_removed = false;
        blank_run = if blank { blank_run + 1 } else { 0 };
        match replaced.get(&line) {
            Some(new) => {
                out.push_str(new);
                if row.ends_with('\n') {
                    out.push('\n');
                }
            }
            None => out.push_str(row),
        }
    }
    Ok(out)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn unused_layout_checks_survive_formatting_and_comments() {
        for source in [
            "typedef char Check[sizeof(struct Packet) == 12 ? 1 : -1];",
            "typedef char Check[sizeof(struct Packet)==12?1:-1];",
            "typedef char Check[\n  OFFSET(struct Packet, field) == 8\n  ? 1\n  : -1];",
            "typedef char Check[(sizeof(struct Packet) != 12) ? -7 /* reject */ : 1];",
        ] {
            assert!(constraint_typedef(&items(source)[0]), "{source}");
        }
        for source in [
            "typedef char Scratch[64];",
            "extern int Removed(void);",
            "typedef int (*Callback)(char text[8]);",
            "typedef char Scratch[64]; /* ? 1 : -1 */",
        ] {
            assert!(!constraint_typedef(&items(source)[0]), "{source}");
        }
    }

    #[test]
    fn the_output_names_the_arms_and_headers_it_reached() {
        let output = "# 1 \"games/a.c\"\nALCHEMY_DEAD_ARM_3\nint a;\n# 1 \"/c/games/b.h\" 1\nx ALCHEMY_DEAD_ARM_12 y\n";
        let live = scan(output, "/c");
        assert_eq!(live.arms, HashSet::from([3, 12]));
        assert_eq!(live.files, HashSet::from(["games/a.c".to_string(), "games/b.h".to_string()]));
    }

    #[test]
    fn a_never_selected_arm_is_dead_and_its_chain_collapses() {
        let text = Text::new("#if A\nint a;\n#else\nint b;\n#endif\nint c;\n".into());
        let found = chains(&text);
        assert_eq!(found.len(), 1);
        let edits = chain_edits(&text, &found[0], &[0]);
        assert_eq!(rewrite(&text, edits).unwrap(), "int b;\nint c;\n");
    }

    #[test]
    fn a_dead_first_arm_hands_its_place_to_the_next_condition() {
        let text = Text::new("#if A\nint a;\n#elif B\nint b;\n#endif\n".into());
        let found = chains(&text);
        let edits = chain_edits(&text, &found[0], &[0]);
        assert_eq!(rewrite(&text, edits).unwrap(), "#if B\nint b;\n#endif\n");
    }

    #[test]
    fn a_dead_else_goes_alone() {
        let text = Text::new("#ifdef A\nint a;\n#else\nint b;\n#endif\n".into());
        let found = chains(&text);
        let edits = chain_edits(&text, &found[0], &[1]);
        assert_eq!(rewrite(&text, edits).unwrap(), "#ifdef A\nint a;\n#endif\n");
    }

    #[test]
    fn an_arm_that_only_stops_the_build_is_no_dead_code() {
        let text = Text::new("#if A\nint a;\n#else\n#error no edition\n#endif\n".into());
        let found = chains(&text);
        assert!(only_error(&text, &found[0].arms[1]));
        assert!(!only_error(&text, &found[0].arms[0]));
    }

    #[test]
    fn modern_chains_stay() {
        let text = Text::new("#ifdef MODERN\nint a;\n#else\nint b;\n#endif\n".into());
        assert!(tests_modern(&chains(&text)[0]));
    }

    #[test]
    fn conditionals_blank_without_moving_items() {
        let text = Text::new("#if A\nint a;\n#endif\n".into());
        let blanked = blank_conditionals(&text);
        assert_eq!(blanked.len(), text.raw.len());
        let found = items(&blanked);
        assert_eq!(found.len(), 1);
        assert_eq!(text.line_of(found[0].start), 2);
    }

    #[test]
    fn includes_resolve_beside_the_file_first() {
        assert_eq!(normalize("games/a/../b/./c.h"), "games/b/c.h");
    }

    #[test]
    fn removing_a_block_leaves_one_blank_line() {
        let text = Text::new("int a;\n\n// note\nint b;\n\nint c;\n".into());
        let first = attached_comment(&text, 4);
        assert_eq!(first, 3);
        assert_eq!(rewrite(&text, vec![Edit::Remove(first, 4)]).unwrap(), "int a;\n\nint c;\n");
    }
}
