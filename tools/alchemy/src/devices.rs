//! Compiler-steering devices in game C, counted per game, with a ratchet:
//! a count may stay or fall against the tracked baseline, never rise.
//!
//! A game's count covers its own C and headers and the COMMON source both
//! games build; Nintendo's library files (routed to agbcc) are not game C.

use crate::compiler::routing::{family_for_source, CompilerFamily, CompilerTarget};
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::Path;

pub(crate) const BASELINE: &str = "tools/alchemy/devices.tsv";

const USAGE: &str = "usage: alchemy check devices [--update | --list]\n\
Counts, for each game, register pins (register ... asm(\"rN\")), empty asm\n\
barriers, instruction-emitting inline asm other than swi (directly or through\n\
a macro), volatile on objects that are not hardware (a heuristic), goto\n\
statements (a device that needs no tag; library C apart), one-pass blocks,\n\
tagged macro uses and once-only loops (a while, for or do loop whose body\n\
always leaves on its first pass), and fails when a count rises above\n\
tools/alchemy/devices.tsv. --update writes the current counts there; lower\n\
them whenever a device goes. --list prints each once-only loop written in\n\
game C, by file and line.";

pub(crate) const KINDS: [&str; 8] =
    ["register-pin", "asm-barrier", "inline-asm", "volatile", "goto", "one-pass", "tagged-macro", "once-loop"];

pub(crate) type Counts = BTreeMap<(String, String), usize>;

pub fn entry(args: &[String]) -> std::process::ExitCode {
    match run(args) {
        Ok(()) => std::process::ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            std::process::ExitCode::FAILURE
        }
    }
}

fn run(args: &[String]) -> Result<(), String> {
    let update = match args {
        [] => false,
        [flag] if flag == "--update" => true,
        [flag] if flag == "--list" => return list(crate::compiler::routing::root()),
        [flag] if flag == "-h" || flag == "--help" => {
            println!("{USAGE}");
            return Ok(());
        }
        _ => return Err(USAGE.into()),
    };
    let root = crate::compiler::routing::root();
    let counts = count(root)?;
    if update {
        fs::write(root.join(BASELINE), render(&counts)).map_err(|e| format!("{BASELINE}: {e}"))?;
        print!("{}", render(&counts));
        return Ok(());
    }
    let baseline = fs::read_to_string(root.join(BASELINE)).map_err(|e| format!("{BASELINE}: {e}"))?;
    let failures = ratchet(&parse(&baseline)?, &counts);
    if !failures.is_empty() {
        return Err(format!("{}\nremove the device, or explain it to Pascal", failures.join("\n")));
    }
    let lower: Vec<String> = counts
        .iter()
        .filter(|(key, value)| parse(&baseline).ok().and_then(|b| b.get(*key).copied()).is_some_and(|b| **value < b))
        .map(|((game, kind), value)| format!("{game} {kind} fell to {value}"))
        .collect();
    if !lower.is_empty() {
        eprintln!("{}; run alchemy check devices --update to lock it in", lower.join(", "));
    }
    Ok(())
}

pub(crate) fn render(counts: &Counts) -> String {
    let mut text = String::from("game\tdevice\tcount\n");
    for ((game, kind), value) in counts {
        text.push_str(&format!("{game}\t{kind}\t{value}\n"));
    }
    text
}

/// Each once-only loop written in game C, as `path:line`, so each can be
/// made plain or tagged. Macro bodies are left to their uses' counts.
fn list(root: &Path) -> Result<(), String> {
    for entry in walkdir::WalkDir::new(root.join("games")).sort_by_file_name() {
        let entry = entry.map_err(|e| e.to_string())?;
        let path = entry.path().strip_prefix(root).unwrap_or(entry.path()).to_string_lossy().into_owned();
        if !(path.ends_with(".c") || path.ends_with(".h")) || library(&path) {
            continue;
        }
        let text = String::from_utf8_lossy(&fs::read(entry.path()).map_err(|e| format!("{path}: {e}"))?).into_owned();
        let plain = without_proven(&code_only(&text));
        for (line, _) in once_loops_in(&plain) {
            println!("{path}:{line}");
        }
    }
    Ok(())
}

/// Nintendo's library C, routed to agbcc: not game C.
fn library(path: &str) -> bool {
    path.ends_with(".c")
        && [CompilerTarget::Tbs, CompilerTarget::Tla]
            .into_iter()
            .any(|target| matches!(family_for_source(target, path), CompilerFamily::Agbcc | CompilerFamily::AgbccFlash))
}

fn parse(text: &str) -> Result<Counts, String> {
    let mut counts = Counts::new();
    for (number, row) in text.lines().enumerate().skip(1) {
        let fields: Vec<&str> = row.split('\t').collect();
        let [game, kind, value] = fields[..] else {
            return Err(format!("{BASELINE}:{}: three fields expected", number + 1));
        };
        let value = value.parse().map_err(|_| format!("{BASELINE}:{}: bad count", number + 1))?;
        counts.insert((game.into(), kind.into()), value);
    }
    Ok(counts)
}

fn ratchet(baseline: &Counts, counts: &Counts) -> Vec<String> {
    counts
        .iter()
        .filter_map(|(key, value)| {
            let allowed = baseline.get(key).copied().unwrap_or(0);
            (*value > allowed).then(|| format!("{} {}: {value}, baseline {allowed}", key.0, key.1))
        })
        .collect()
}

/// One line for make progress.
pub(crate) fn summary(counts: &Counts, game: &str) -> String {
    let get = |kind: &str| counts.get(&(game.to_string(), kind.to_string())).copied().unwrap_or(0);
    format!(
        "; devices: {} register pins, {} asm barriers, {} inline asm, {} volatile, {} goto, {} one-pass blocks, {} other tagged macro uses, {} once-only loops",
        get("register-pin"),
        get("asm-barrier"),
        get("inline-asm"),
        get("volatile"),
        get("goto"),
        get("one-pass"),
        get("tagged-macro"),
        get("once-loop")
    )
}

pub(crate) fn count(root: &Path) -> Result<Counts, String> {
    let games = [("tbs", "games/tbs", CompilerTarget::Tbs), ("tla", "games/tla", CompilerTarget::Tla)];
    let mut sources: Vec<(String, String)> = Vec::new();
    for entry in walkdir::WalkDir::new(root.join("games")).sort_by_file_name() {
        let entry = entry.map_err(|e| e.to_string())?;
        let path = entry.path().strip_prefix(root).unwrap_or(entry.path()).to_string_lossy().into_owned();
        if !(path.ends_with(".c") || path.ends_with(".h")) {
            continue;
        }
        let text = fs::read(entry.path()).map_err(|e| format!("{path}: {e}"))?;
        sources.push((path, String::from_utf8_lossy(&text).into_owned()));
    }
    // Macros whose body emits instructions count at each use.
    let mut emitting: BTreeSet<String> = BTreeSet::new();
    for (_, text) in &sources {
        let code = code_only(text);
        let proven = proven_macros(&code);
        for (name, body) in macro_bodies(&code) {
            if !proven.contains(&name) && asm_statements(&without_proven(&body)).iter().any(|t| instruction(t)) {
                emitting.insert(name);
            }
        }
    }
    // Macros whose body holds a device count at each use, by that device,
    // with the devices of the macros their body uses (S2: every device is
    // counted, a macro's too). A macro a header defines reaches its own
    // game and, from games/common, both; a macro a C file defines reaches
    // only that file.
    let mut header_definitions: BTreeMap<&str, Vec<Definition>> = BTreeMap::new();
    for (path, text) in &sources {
        // A header that is C text keeps its macros to itself, as a C file does.
        if !path.ends_with(".h") || crate::c_items::c_text(path, text) {
            continue;
        }
        let found = definitions(text);
        for (game, folder, _) in &games {
            if path.starts_with(folder) || path.starts_with("games/common") {
                header_definitions.entry(game).or_default().extend(found.iter().cloned());
            }
        }
    }
    let header_macros: BTreeMap<&str, BTreeMap<String, Kinds>> =
        header_definitions.iter().map(|(game, found)| (*game, resolve(found))).collect();
    let mut counts = Counts::new();
    for (game, _, _) in &games {
        for kind in KINDS {
            counts.insert((game.to_string(), kind.to_string()), 0);
        }
    }
    for (path, text) in &sources {
        for (game, folder, target) in &games {
            if !(path.starts_with(folder) || path.starts_with("games/common")) {
                continue;
            }
            if path.ends_with(".c")
                && matches!(
                    family_for_source(*target, path),
                    CompilerFamily::Agbcc | CompilerFamily::AgbccFlash
                )
            {
                continue;
            }
            let headers = header_macros.get(game).cloned().unwrap_or_default();
            let local = if crate::c_items::c_text(path, text) { in_force(&definitions(text), &headers) } else { Vec::new() };
            let found = devices_in(&without_proven(&code_only(text)), &emitting, &headers, &local);
            for (kind, value) in KINDS.iter().zip(found) {
                *counts.get_mut(&(game.to_string(), kind.to_string())).unwrap() += value;
            }
        }
    }
    Ok(counts)
}

/// Source with comments and string contents blanked except asm templates,
/// which keep their text.
fn code_only(text: &str) -> String {
    let bytes = text.as_bytes();
    let mut out = String::with_capacity(text.len());
    let mut i = 0;
    while i < bytes.len() {
        if bytes[i..].starts_with(b"/*") {
            let end = text[i + 2..].find("*/").map_or(text.len(), |e| i + 2 + e + 2);
            out.extend(text[i..end].chars().map(|c| if c == '\n' { '\n' } else { ' ' }));
            i = end;
        } else if bytes[i..].starts_with(b"//") {
            let end = text[i..].find('\n').map_or(text.len(), |e| i + e);
            out.extend(std::iter::repeat_n(' ', end - i));
            i = end;
        } else {
            let c = text[i..].chars().next().unwrap();
            out.push(c);
            i += c.len_utf8();
        }
    }
    out
}

/// Only macros whose complete body has reviewed evidence are exempt.
fn proven_macros(code: &str) -> BTreeSet<String> {
    crate::evidence::approved_spans(code).into_iter()
        .flat_map(|span| macro_bodies(&code[span]))
        .map(|(name, _)| name).collect()
}

fn without_proven(code: &str) -> String {
    crate::evidence::mask_inline(code)
}

/// Each #define's name and body, continuation lines joined.
fn macro_bodies(code: &str) -> Vec<(String, String)> {
    let mut found = Vec::new();
    let mut lines = code.lines();
    while let Some(line) = lines.next() {
        let Some(rest) = line.trim_start().strip_prefix('#') else { continue };
        let Some(rest) = rest.trim_start().strip_prefix("define") else { continue };
        let mut body = rest.to_string();
        while body.trim_end().ends_with('\\') {
            body = body.trim_end().trim_end_matches('\\').to_string();
            match lines.next() {
                Some(next) => body.push_str(next),
                None => break,
            }
        }
        let body = body.trim_start();
        let end = body.find(|c: char| !c.is_ascii_alphanumeric() && c != '_').unwrap_or(body.len());
        found.push((body[..end].to_string(), body[end..].to_string()));
    }
    found
}

/// The template of every asm statement or asm label: the concatenated
/// string literals after `asm (` / `__asm__ volatile (`.
fn asm_statements(code: &str) -> Vec<String> {
    let pattern = regex::Regex::new(r#"\b(?:asm|__asm__|__asm)\s*(?:volatile|__volatile__)?\s*\(\s*((?:"(?:[^"\\]|\\.)*"\s*)*)"#)
        .expect("static pattern");
    pattern
        .captures_iter(code)
        .map(|capture| {
            let literals = regex::Regex::new(r#""((?:[^"\\]|\\.)*)""#).expect("static pattern");
            literals.captures_iter(&capture[1]).map(|l| l[1].to_string()).collect::<String>()
        })
        .collect()
}

fn register(template: &str) -> bool {
    matches!(template, "ip" | "sp" | "lr" | "fp" | "sl" | "sb")
        || template.strip_prefix('r').is_some_and(|n| !n.is_empty() && n.bytes().all(|b| b.is_ascii_digit()))
}

/// A template that emits instructions: anything but a register name, a
/// symbol name (an asm label) or nothing, and not a software interrupt.
fn instruction(template: &str) -> bool {
    let template = template.trim();
    !template.is_empty()
        && !register(template)
        && (!template.bytes().all(|b| b.is_ascii_alphanumeric() || b == b'_' || b == b'.')
            || matches!(template, "nop" | "ret" | "eret" | "wfi" | "wfe" | "sev" | "yield"))
}

/// The names of the macros a FAKEMATCH tag explains, or whose body holds a
/// device or uses a macro that does, that the C file at `path` can use:
/// those its own game's headers
/// and games/common's headers define (a games/common file sees both games'
/// headers), and those it defines itself. Headers are read once per run.
pub(crate) fn tagged_macro_names(root: &Path, path: &str, text: &str) -> BTreeSet<String> {
    // Each top folder's header definitions, and the macros each folder's
    // files can use, resolved once per run.
    #[derive(Default)]
    struct Headers {
        by_folder: BTreeMap<String, Vec<Definition>>,
        by_scope: BTreeMap<String, BTreeMap<String, Kinds>>,
    }
    static HEADERS: std::sync::OnceLock<std::sync::Mutex<BTreeMap<std::path::PathBuf, Headers>>> = std::sync::OnceLock::new();
    let mut cache = HEADERS.get_or_init(Default::default).lock().unwrap_or_else(|e| e.into_inner());
    let headers = cache.entry(root.to_path_buf()).or_insert_with(|| {
        let mut by_folder: BTreeMap<String, Vec<Definition>> = BTreeMap::new();
        for entry in walkdir::WalkDir::new(root.join("games")).sort_by_file_name().into_iter().flatten() {
            let file = entry.path();
            if file.extension().and_then(|e| e.to_str()) != Some("h") {
                continue;
            }
            let relative = file.strip_prefix(root).unwrap_or(file).to_string_lossy().into_owned();
            let folder = relative.split('/').take(2).collect::<Vec<_>>().join("/");
            if let Ok(bytes) = fs::read(file) {
                let text = String::from_utf8_lossy(&bytes);
                if !crate::c_items::c_text(&relative, &text) {
                    by_folder.entry(folder).or_default().extend(definitions(&text));
                }
            }
        }
        Headers { by_folder, by_scope: BTreeMap::new() }
    });
    let own = path.split('/').take(2).collect::<Vec<_>>().join("/");
    if !headers.by_scope.contains_key(&own) {
        let reached = headers
            .by_folder
            .iter()
            .filter(|(folder, _)| own == "games/common" || folder.as_str() == own || folder.as_str() == "games/common")
            .flat_map(|(_, found)| found.iter());
        let resolved = resolve(reached);
        headers.by_scope.insert(own.clone(), resolved);
    }
    let reached = &headers.by_scope[&own];
    let mut names: BTreeSet<String> = reached.keys().cloned().collect();
    let local = in_force(&definitions(text), reached);
    names.extend(local.into_iter().filter(|(_, _, kinds)| kinds.is_some_and(|kinds| kinds.iter().any(|n| *n > 0))).map(|(_, name, _)| name));
    names
}

/// A macro body without the outer `do { ... } while (0)` that wraps a
/// statement macro's whole body: C's idiom for a macro used as one
/// statement, the source's own layer (S2), not a device. Blocks inside it
/// still count.
fn without_statement_wrapper(code: &str) -> String {
    let joined = code.replace("\\\n", " ");
    let wrapper = regex::Regex::new(r"(?s)^(\s*#\s*define\s+[A-Za-z_][A-Za-z0-9_]*(?:\([^)]*\))?)\s*do\s*\{(.*)\}\s*while\s*\(\s*0\s*\)\s*$")
        .expect("static pattern");
    match wrapper.captures(&joined) {
        Some(c) => format!("{} {{{}}}", &c[1], &c[2]),
        None => joined,
    }
}

/// The devices of one use of a macro, in KINDS order.
type Kinds = [usize; 8];

/// One #define or #undef of a text.
#[derive(Clone)]
struct Definition {
    /// Its line, from 0.
    line: usize,
    name: String,
    /// The devices its own body holds, each as often as it occurs (None
    /// for an #undef). A FAKEMATCH tag in the comment right above the
    /// #define, or in its body, explains them; a tagged body with no
    /// device the count knows by kind (a forced temporary, a cast) is one
    /// tagged-macro use. An untagged body counts its devices all the same:
    /// dropping a tag hides nothing.
    own: Option<Kinds>,
    /// The identifiers its body uses: the devices of a macro among them
    /// count at each use of this one.
    words: Vec<String>,
}

/// Every #define and #undef of a text, in order.
fn definitions(text: &str) -> Vec<Definition> {
    let lines: Vec<&str> = text.lines().collect();
    let define = regex::Regex::new(r"^\s*#\s*define\s+([A-Za-z_][A-Za-z0-9_]*)").expect("static pattern");
    let undef = regex::Regex::new(r"^\s*#\s*undef\s+([A-Za-z_][A-Za-z0-9_]*)").expect("static pattern");
    let comment = regex::Regex::new(r"^\s*(/\*|\*|//)|\*/\s*$").expect("static pattern");
    let word = regex::Regex::new(r"[A-Za-z_][A-Za-z0-9_]*").expect("static pattern");
    let open = regex::Regex::new(r"\bdo\s*\{").expect("static pattern");
    let close_last = regex::Regex::new(r"\}\s*while\s*\(\s*0\s*\)\s*$").expect("static pattern");
    let mut found = Vec::new();
    for (i, line) in lines.iter().enumerate() {
        if let Some(name) = undef.captures(line).map(|c| c[1].to_string()) {
            found.push(Definition { line: i, name, own: None, words: Vec::new() });
            continue;
        }
        let Some(head) = define.captures(line) else { continue };
        let name = head[1].to_string();
        let mut body = line[head.get(0).map_or(0, |m| m.end())..].to_string();
        let mut j = i;
        while lines[j].trim_end().ends_with('\\') && j + 1 < lines.len() {
            j += 1;
            body.push('\n');
            body.push_str(lines[j]);
        }
        let mut above = String::new();
        let mut k = i;
        while k > 0 && comment.is_match(lines[k - 1]) {
            k -= 1;
            above.insert_str(0, lines[k]);
        }
        let tagged = above.contains("FAKEMATCH") || body.contains("FAKEMATCH");
        let code = code_only(&body);
        let templates = asm_statements(&code);
        let mut own = [0; 8];
        own[0] = templates.iter().filter(|t| register(t.trim())).count();
        own[1] = templates.iter().filter(|t| t.trim().is_empty()).count();
        own[3] = code.lines().map(volatiles).sum();
        own[5] = one_pass(&without_statement_wrapper(&format!("#define {name}{code}")));
        own[7] = once_loops(&code.replace("\\\n", "  "));
        // A body that ends by closing more blocks than it opens ends a
        // statement wrapper another macro opened (IRQ_UNLOCK closing
        // IRQ_LOCK's `do {`): that close is the pair's wrapper, no device.
        let joined = code.replace("\\\n", " ");
        let opens = open.find_iter(&joined).count();
        let closes_last = close_last.is_match(&joined);
        if closes_last && own[5] > opens {
            own[5] -= 1;
        }
        if tagged && own.iter().all(|n| *n == 0) {
            own[6] = 1;
        }
        let words = word.find_iter(&code).map(|w| w.as_str().to_string()).filter(|w| *w != name).collect();
        found.push(Definition { line: i, name, own: Some(own), words });
    }
    found
}

/// Each macro of `definitions` that holds a device, with the devices of the
/// macros its body uses, as far down as they go (S2: a device inside a
/// macro counts at each use, inside a macro another macro uses too). A
/// later #define or #undef of a name replaces an earlier one.
fn resolve<'a>(definitions: impl IntoIterator<Item = &'a Definition>) -> BTreeMap<String, Kinds> {
    let mut bodies: BTreeMap<&str, (&Kinds, &[String])> = BTreeMap::new();
    for definition in definitions {
        match &definition.own {
            Some(own) => bodies.insert(&definition.name, (own, &definition.words)),
            None => bodies.remove(definition.name.as_str()),
        };
    }
    fn total<'a>(name: &'a str, bodies: &BTreeMap<&'a str, (&Kinds, &'a [String])>, done: &mut BTreeMap<&'a str, Kinds>, open: &mut BTreeSet<&'a str>) -> Kinds {
        if let Some(kinds) = done.get(name) {
            return *kinds;
        }
        let Some((own, words)) = bodies.get(name) else { return [0; 8] };
        // A macro that names itself, directly or through another, is not
        // expanded again (C stops there too).
        if !open.insert(name) {
            return [0; 8];
        }
        let mut sum = **own;
        for word in words.iter() {
            let inner = total(word, bodies, done, open);
            for (kind, n) in sum.iter_mut().zip(inner) {
                *kind += n;
            }
        }
        open.remove(name);
        done.insert(name, sum);
        sum
    }
    let mut done = BTreeMap::new();
    let mut found = BTreeMap::new();
    for name in bodies.keys() {
        let sum = total(name, &bodies, &mut done, &mut BTreeSet::new());
        if sum.iter().any(|n| *n > 0) {
            found.insert(name.to_string(), sum);
        }
    }
    found
}

/// A C file's own #define and #undef lines, in order: its line, its name
/// and, for a #define, its devices with those of the macros its body uses,
/// the file's own as defined above it and otherwise its headers'. A use
/// counts by the definition in force on its line, from its #define to the
/// next #define or #undef of its name.
fn in_force(definitions: &[Definition], headers: &BTreeMap<String, Kinds>) -> Vec<(usize, String, Option<Kinds>)> {
    let mut found: Vec<(usize, String, Option<Kinds>)> = Vec::new();
    for definition in definitions {
        let kinds = definition.own.map(|own| {
            let mut sum = own;
            for word in &definition.words {
                let inner = match found.iter().rev().find(|(_, name, _)| name == word) {
                    Some((_, _, kinds)) => kinds.unwrap_or([0; 8]),
                    None => headers.get(word).copied().unwrap_or([0; 8]),
                };
                for (kind, n) in sum.iter_mut().zip(inner) {
                    *kind += n;
                }
            }
            sum
        });
        found.push((definition.line, definition.name.clone(), kinds));
    }
    found
}

/// One-pass blocks: `do { ... } while (0)`, which ends CSE's block and is a
/// scheduling barrier (S2 names it a device).
fn one_pass(code: &str) -> usize {
    regex::Regex::new(r"\}\s*while\s*\(\s*0\s*\)").expect("static pattern").find_iter(code).count()
}

/// Once-only loops: a `while`, `for` or `do` loop whose body always leaves
/// on its first pass (its last statement is a break, return or goto, or an
/// if whose arms both leave, and no continue of its own can start a second
/// pass). Such a loop repeats nothing: it only shapes the code's layout, as
/// a goto or a one-pass block would. `do { } while (0)` is a one-pass block
/// and counts there, not here.
fn once_loops(code: &str) -> usize {
    once_loop_offsets(code).len()
}

/// Once-only loops of a C text with its directives: each edition's branch
/// of an #if can open a brace another closes, so the loops are read in two
/// whole views, every conditional's first branch and every conditional's
/// last, each balanced as one edition sees it, and counted once each: by
/// line and column, from 1 and 0.
fn once_loops_in(code: &str) -> Vec<(usize, usize)> {
    let mut found: BTreeSet<(usize, usize)> = BTreeSet::new();
    for last in [false, true] {
        let view = without_directives(&one_branch(code, last));
        found.extend(once_loop_offsets(&view).into_iter().map(|offset| {
            let line_start = view[..offset].rfind('\n').map_or(0, |n| n + 1);
            (view[..offset].matches('\n').count() + 1, offset - line_start)
        }));
    }
    found.into_iter().collect()
}

/// The text with every #if group's branches but one blanked to empty
/// lines: its first branch, or its last. Lines keep their places.
fn one_branch(code: &str, last: bool) -> String {
    let lines: Vec<&str> = code.lines().collect();
    let mut blank = vec![false; lines.len()];
    // Each open group's branch starts, by line.
    let mut groups: Vec<Vec<usize>> = Vec::new();
    for (number, line) in lines.iter().enumerate() {
        let Some(directive) = line.trim_start().strip_prefix('#') else { continue };
        let word: String = directive.trim_start().chars().take_while(|c| c.is_ascii_alphabetic()).collect();
        match word.as_str() {
            "if" | "ifdef" | "ifndef" => groups.push(vec![number]),
            "elif" | "else" => {
                if let Some(group) = groups.last_mut() {
                    group.push(number);
                }
            }
            "endif" => {
                let Some(starts) = groups.pop() else { continue };
                let keep = if last { starts.len() - 1 } else { 0 };
                for (branch, start) in starts.iter().enumerate() {
                    let end = starts.get(branch + 1).copied().unwrap_or(number);
                    if branch != keep {
                        blank[*start + 1..end].iter_mut().for_each(|b| *b = true);
                    }
                }
            }
            _ => {}
        }
    }
    let mut text = String::with_capacity(code.len());
    for (line, blanked) in lines.iter().zip(blank) {
        if !blanked {
            text.push_str(line);
        }
        text.push('\n');
    }
    text
}

/// Where each once-only loop of `code` starts, in bytes.
pub(crate) fn once_loop_offsets(code: &str) -> Vec<usize> {
    let keyword = regex::Regex::new(r"\b(?:while|for|do)\b").expect("static pattern");
    let bytes = code.as_bytes();
    // Keywords inside string or character literals are not code.
    let mut literal = vec![false; bytes.len()];
    let mut i = 0;
    while i < bytes.len() {
        if bytes[i] == b'"' || bytes[i] == b'\'' {
            let end = skip_literal(bytes, i);
            literal[i..end].iter_mut().for_each(|b| *b = true);
            i = end;
        } else {
            i += 1;
        }
    }
    keyword
        .find_iter(code)
        .filter(|found| !literal[found.start()])
        .filter(|found| matches!(Statement::parse(bytes, found.start()), Some((Statement::Loop { once: true }, _))))
        .map(|found| found.start())
        .collect()
}

/// The end of the string or character literal that starts at `start`.
fn skip_literal(bytes: &[u8], start: usize) -> usize {
    let quote = bytes[start];
    let mut i = start + 1;
    while i < bytes.len() && bytes[i] != quote && bytes[i] != b'\n' {
        i += if bytes[i] == b'\\' { 2 } else { 1 };
    }
    (i + 1).min(bytes.len())
}

/// The little of a C statement once_loops needs.
enum Statement {
    /// break, return or goto: the loop around it ends here.
    Leave,
    Continue,
    Block(Vec<Statement>),
    If(Box<Statement>, Option<Box<Statement>>),
    /// A switch: a break inside it is its own, a continue the loop's.
    Switch(Box<Statement>),
    /// A loop: a break or continue inside it is its own.
    Loop { once: bool },
    Label,
    Other,
}

impl Statement {
    /// The statement that starts at `at` (after any space), and its end.
    fn parse(bytes: &[u8], at: usize) -> Option<(Statement, usize)> {
        let i = skip_space(bytes, at);
        let word_end = i + bytes[i..].iter().take_while(|b| b.is_ascii_alphanumeric() || **b == b'_').count();
        let word = std::str::from_utf8(&bytes[i..word_end]).unwrap_or("");
        if bytes.get(i) == Some(&b'{') {
            let mut inner = Vec::new();
            let mut j = i + 1;
            loop {
                let k = skip_space(bytes, j);
                match bytes.get(k) {
                    None => return None,
                    Some(b'}') => return Some((Statement::Block(inner), k + 1)),
                    Some(b';') => j = k + 1,
                    _ => {
                        let (statement, end) = Statement::parse(bytes, k)?;
                        inner.push(statement);
                        j = end;
                    }
                }
            }
        }
        match word {
            "if" => {
                let close = matching(bytes, skip_space(bytes, word_end))?;
                let (then, end) = Statement::parse(bytes, close)?;
                let after = skip_space(bytes, end);
                if bytes[after..].starts_with(b"else") && !bytes.get(after + 4).is_some_and(|b| b.is_ascii_alphanumeric() || *b == b'_') {
                    let (otherwise, end) = Statement::parse(bytes, after + 4)?;
                    return Some((Statement::If(Box::new(then), Some(Box::new(otherwise))), end));
                }
                Some((Statement::If(Box::new(then), None), end))
            }
            "switch" => {
                let close = matching(bytes, skip_space(bytes, word_end))?;
                let (body, end) = Statement::parse(bytes, close)?;
                Some((Statement::Switch(Box::new(body)), end))
            }
            "while" | "for" => {
                let close = matching(bytes, skip_space(bytes, word_end))?;
                // `while (c);` has an empty body: it repeats, or it is the
                // tail of a do loop, read with its do.
                let next = skip_space(bytes, close);
                if bytes.get(next) == Some(&b';') {
                    return Some((Statement::Loop { once: false }, next + 1));
                }
                let (body, end) = Statement::parse(bytes, close)?;
                Some((Statement::Loop { once: body.leaves() && !body.continues() }, end))
            }
            "do" => {
                let (body, end) = Statement::parse(bytes, word_end)?;
                let tail = skip_space(bytes, end);
                if !bytes[tail..].starts_with(b"while") {
                    return None;
                }
                let open = skip_space(bytes, tail + 5);
                let close = matching(bytes, open)?;
                let condition: String =
                    std::str::from_utf8(&bytes[open + 1..close - 1]).unwrap_or("").split_whitespace().collect();
                let next = skip_space(bytes, close);
                let end = if bytes.get(next) == Some(&b';') { next + 1 } else { close };
                // `do { } while (0)` is a one-pass block, counted as such.
                let once = condition != "0" && body.leaves() && !body.continues();
                Some((Statement::Loop { once }, end))
            }
            "break" | "return" | "goto" => Some((Statement::Leave, statement_end(bytes, word_end)?)),
            "continue" => Some((Statement::Continue, statement_end(bytes, word_end)?)),
            "case" | "default" => {
                let colon = word_end + bytes[word_end..].iter().position(|b| *b == b':')?;
                Some((Statement::Label, colon + 1))
            }
            _ if !word.is_empty() && !word.as_bytes()[0].is_ascii_digit() && {
                let k = skip_space(bytes, word_end);
                bytes.get(k) == Some(&b':') && bytes.get(k + 1) != Some(&b':')
            } =>
            {
                Some((Statement::Label, skip_space(bytes, word_end) + 1))
            }
            _ => Some((Statement::Other, statement_end(bytes, i)?)),
        }
    }

    /// Whether every pass through it leaves the loop around it.
    fn leaves(&self) -> bool {
        match self {
            Statement::Leave => true,
            Statement::Block(inner) => inner.last().is_some_and(Statement::leaves),
            Statement::If(then, Some(otherwise)) => then.leaves() && otherwise.leaves(),
            _ => false,
        }
    }

    /// Whether it holds a continue of the loop around it.
    fn continues(&self) -> bool {
        match self {
            Statement::Continue => true,
            Statement::Block(inner) => inner.iter().any(Statement::continues),
            Statement::If(then, otherwise) => then.continues() || otherwise.as_ref().is_some_and(|o| o.continues()),
            Statement::Switch(body) => body.continues(),
            _ => false,
        }
    }
}

/// Past spaces and macro line continuations.
fn skip_space(bytes: &[u8], mut i: usize) -> usize {
    while i < bytes.len() && (bytes[i].is_ascii_whitespace() || bytes[i] == b'\\') {
        i += 1;
    }
    i
}

/// After the parentheses that open at `open`, or None.
fn matching(bytes: &[u8], open: usize) -> Option<usize> {
    if bytes.get(open) != Some(&b'(') {
        return None;
    }
    let mut depth = 0;
    let mut i = open;
    while i < bytes.len() {
        match bytes[i] {
            b'"' | b'\'' => {
                i = skip_literal(bytes, i);
                continue;
            }
            b'(' => depth += 1,
            b')' => {
                depth -= 1;
                if depth == 0 {
                    return Some(i + 1);
                }
            }
            _ => {}
        }
        i += 1;
    }
    None
}

/// After the `;` that ends the expression or declaration at `i`, brackets
/// and literals skipped; None at a `}` that closes an outer block.
fn statement_end(bytes: &[u8], mut i: usize) -> Option<usize> {
    let mut depth = 0i32;
    while i < bytes.len() {
        match bytes[i] {
            b'"' | b'\'' => {
                i = skip_literal(bytes, i);
                continue;
            }
            b'(' | b'[' | b'{' => depth += 1,
            b')' | b']' | b'}' => {
                depth -= 1;
                if depth < 0 {
                    return None;
                }
            }
            b';' if depth == 0 => return Some(i + 1),
            _ => {}
        }
        i += 1;
    }
    None
}

/// A file's own macros, each with its devices from the line it is defined
/// (None from an #undef).
type InForce = [(usize, String, Option<Kinds>)];

#[cfg(test)]
fn devices(code: &str, emitting: &BTreeSet<String>, headers: &BTreeMap<String, Kinds>) -> [usize; 8] {
    devices_in(code, emitting, headers, &[])
}

/// Counts in KINDS order over one file, outside #define bodies: a macro's
/// devices count where it is used, by the file's own definition in force
/// on that line (`local`, from `in_force`) and otherwise by its headers'.
fn devices_in(code: &str, emitting: &BTreeSet<String>, headers: &BTreeMap<String, Kinds>, local: &InForce) -> [usize; 8] {
    let plain = without_directives(code);
    let templates = asm_statements(&plain);
    let pins = templates.iter().filter(|t| register(t.trim())).count();
    let barriers = templates.iter().filter(|t| t.trim().is_empty()).count();
    let mut inline = templates.iter().filter(|t| instruction(t)).count();
    let word = regex::Regex::new(r"[A-Za-z_][A-Za-z0-9_]*").expect("static pattern");
    let mut volatile = 0;
    let mut gotos = 0;
    let mut macro_uses = [0usize; 8];
    for (number, line) in plain.lines().enumerate() {
        for found in word.find_iter(line) {
            if emitting.contains(found.as_str()) {
                inline += 1;
            }
            if found.as_str() == "goto" {
                gotos += 1;
            }
            let kinds = match local.iter().rev().find(|(at, name, _)| *at < number && name == found.as_str()) {
                Some((_, _, kinds)) => *kinds,
                None => headers.get(found.as_str()).copied(),
            };
            for (kind, n) in macro_uses.iter_mut().zip(kinds.unwrap_or([0; 8])) {
                *kind += n;
            }
        }
        volatile += volatiles(line);
    }
    [
        pins + macro_uses[0],
        barriers + macro_uses[1],
        inline,
        volatile + macro_uses[3],
        gotos,
        one_pass(&plain) + macro_uses[5],
        macro_uses[6],
        once_loops_in(code).len() + macro_uses[7],
    ]
}

/// Code with each preprocessor directive, continuation lines too, blanked to
/// an empty line: a #define body counts where the macro is used.
fn without_directives(code: &str) -> String {
    let mut plain = String::with_capacity(code.len());
    let mut lines = code.lines();
    while let Some(line) = lines.next() {
        let directive = line.trim_start().starts_with('#');
        let mut continued = directive && line.trim_end().ends_with('\\');
        if !directive {
            plain.push_str(line);
        }
        plain.push('\n');
        while continued {
            match lines.next() {
                Some(next) => {
                    continued = next.trim_end().ends_with('\\');
                    plain.push('\n');
                }
                None => break,
            }
        }
    }
    plain
}

/// The volatile objects a line declares or reads through, outside asm
/// qualifiers and lines about the hardware.
fn volatiles(line: &str) -> usize {
    if !line.contains("volatile") || hardware(line) {
        return 0;
    }
    line.matches("volatile").count()
        - line.matches("asm volatile").count()
        - line.matches("__asm__ volatile").count()
        - line.matches("asm __volatile__").count()
        - line.matches("__asm__ __volatile__").count()
}

/// A line about the hardware: registers, I/O or video memory addresses.
fn hardware(line: &str) -> bool {
    ["REG_", "0x04", "0x4000", "0x05", "0x06", "0x07", "IO_REG", "Dma", "DMA", "VRAM", "OAM", "PLTT", "IRQ", "Irq"]
        .iter()
        .any(|needle| line.contains(needle))
        || line.trim_start().starts_with("typedef")
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn a_software_interrupt_word_does_not_hide_other_instructions() {
        assert!(instruction("swi 2; mov r0, r1"));
        assert!(instruction("nop"));
        assert!(!instruction("Resource_Table"));
        assert_eq!(devices("asm(\"swi 2; mov r0, r1\"); asm(\"nop\");", &BTreeSet::new(), &BTreeMap::new()), [0, 0, 2, 0, 0, 0, 0, 0]);
    }

    #[test]
    fn each_kind_is_told_apart() {
        let code = code_only(
            "register s32 a asm(\"r4\") = 0;\n\
             asm(\"\");\n__asm__ volatile(\"\" ::: \"memory\");\n\
             asm(\"ldrh %0, [%1]\" : \"=r\"(a));\n\
             asm volatile(\"swi 3\");\n\
             extern u8 x[] __asm__(\"Data_03001ebc\");\n\
             volatile s32 counter; // asm(\"mov r0, r0\")\n\
             *(volatile u16 *)REG_DISPCNT = 0;\n\
             STEER(a);\n",
        );
        let emitting = BTreeSet::from(["STEER".to_string()]);
        assert_eq!(devices(&code, &emitting, &BTreeMap::new()), [1, 2, 3, 1, 0, 0, 0, 0]);
    }

    #[test]
    fn a_macro_body_counts_where_it_is_used() {
        let code = code_only("#define STEER(x) \\\n    asm(\"mov %0, %0\" : \"+r\"(x))\n");
        let bodies = macro_bodies(&code);
        assert_eq!(bodies[0].0, "STEER");
        assert!(asm_statements(&bodies[0].1).iter().any(|t| instruction(t)));
        assert_eq!(devices(&code, &BTreeSet::new(), &BTreeMap::new()), [0, 0, 0, 0, 0, 0, 0, 0]);
    }

    #[test]
    fn gnu_asm_qualifiers_are_not_volatile_objects() {
        let code = code_only(
            "__asm__ __volatile__(\"\" : : \"r\"(value));\n\
             volatile s32 counter; __asm__ __volatile__(\"\" : : \"r\"(counter));\n",
        );
        assert_eq!(devices(&code, &BTreeSet::new(), &BTreeMap::new()), [0, 2, 0, 1, 0, 0, 0, 0]);
    }

    #[test]
    fn a_marker_alone_does_not_exempt_a_helper() {
        let code = code_only(
            "CAMELOT_ASM(\"stmia\")\n\
             static __inline__ void Set(u32 a)\n{\n    register u32 r __asm__(\"r0\") = a;\n    __asm__ volatile(\"stmia %0!, {%0}\" : \"+r\"(r));\n}\n\
             CAMELOT_ASM(\"identity\")\n#define IDENT() asm(\"stmia r0!, {r1}\")\n\
             void f(void)\n{\n    CAMELOT_ASM(\"stack switch\")\n    register u8 *sp __asm__(\"sp\");\n    IDENT();\n    asm(\"\");\n}\n",
        );
        assert!(!proven_macros(&code).contains("IDENT"));
        assert_eq!(without_proven(&code), code);
        assert_eq!(devices(&without_proven(&code), &BTreeSet::new(), &BTreeMap::new()), [2, 1, 1, 0, 0, 0, 0, 0]);
    }

    #[test]
    fn the_ratchet_refuses_a_rise_only() {
        let key = ("tbs".to_string(), "volatile".to_string());
        let baseline = Counts::from([(key.clone(), 3)]);
        assert!(ratchet(&baseline, &Counts::from([(key.clone(), 3)])).is_empty());
        assert!(ratchet(&baseline, &Counts::from([(key.clone(), 2)])).is_empty());
        assert_eq!(ratchet(&baseline, &Counts::from([(key, 4)])).len(), 1);
        assert_eq!(parse(&render(&baseline)).unwrap(), baseline);
    }

    #[test]
    fn gotos_are_counted_where_code_jumps_not_where_a_comment_or_macro_names_them() {
        let code = code_only("void f(void)\n{\n\tif (x)\n\t\tgoto done; // goto in a comment\n\tgoto done;\ndone:\n\treturn;\n}\n#define LEAVE goto done\n");
        assert_eq!(devices(&code, &BTreeSet::new(), &BTreeMap::new()), [0, 0, 0, 0, 2, 0, 0, 0]);
    }

    #[test]
    fn one_pass_blocks_and_tagged_macros_count_at_each_use() {
        let header = "// 説明
/* FAKEMATCH: measured. */
#define SET(v) \\
	do \\
	{ \\
		g = (v); \\
	} while (0)
#define PLAIN(v) do { g = (v); } while (0)
#define TEMP(v) { u32 t_ = (v); g = t_; } /* FAKEMATCH: temporary. */
";
        let tagged = resolve(&definitions(header));
        // SET's only block is the statement macro's own wrapper: its tag
        // still counts it, as a tagged macro, not as a one-pass block.
        assert_eq!(tagged.get("SET"), Some(&[0, 0, 0, 0, 0, 0, 1, 0]));
        assert_eq!(tagged.get("TEMP"), Some(&[0, 0, 0, 0, 0, 0, 1, 0]));
        assert!(!tagged.contains_key("PLAIN"));
        let code = code_only("void f(void)\n{\n\tSET(1);\n\tSET(2);\n\tPLAIN(3);\n\tTEMP(4);\n\tdo\n\t{\n\t\tg = 5;\n\t} while (0);\n}\n");
        assert_eq!(devices(&code, &BTreeSet::new(), &tagged), [0, 0, 0, 0, 0, 1, 3, 0]);
    }

    #[test]
    fn a_statement_macros_wrapper_is_its_layer_and_a_block_inside_it_a_device() {
        let header = "#define LAYER(v) \\
	do \\
	{ \\
		g = (v); \\
	} while (0)
#define INNER(v) \\
	do \\
	{ \\
		u32 s_; \\
		do \\
		{ \\
			s_ = (v); \\
		} while (0); \\
		g = s_; \\
	} while (0)
#define HALF() } while (0); g = 0; } while (0)
#define BARRIER() do { asm(\"\"); } while (0)
";
        let found = resolve(&definitions(header));
        assert!(!found.contains_key("LAYER"), "the wrapper alone is no device");
        assert_eq!(found.get("INNER"), Some(&[0, 0, 0, 0, 0, 1, 0, 0]), "an untagged macro's inner block still counts");
        assert_eq!(found.get("HALF"), Some(&[0, 0, 0, 0, 0, 1, 0, 0]), "a macro closing another's inner block counts it; its last close ends their wrapper");
        assert_eq!(found.get("BARRIER"), Some(&[0, 1, 0, 0, 0, 0, 0, 0]));
        let code = code_only("void f(void)\n{\n\tLAYER(1);\n\tINNER(2);\n\tINNER(3);\n\tBARRIER();\n}\n");
        assert_eq!(devices(&code, &BTreeSet::new(), &found), [0, 1, 0, 0, 0, 2, 0, 0]);
    }

    #[test]
    fn a_macro_counts_only_in_the_game_and_file_that_can_use_it() {
        let root = tempfile::tempdir().unwrap();
        let write = |path: &str, text: &str| {
            let file = root.path().join(path);
            std::fs::create_dir_all(file.parent().unwrap()).unwrap();
            std::fs::write(file, text).unwrap();
        };
        write(
            "games/tla/include/local.h",
            "/* FAKEMATCH: measured. */\n#define STEP(v) do { g = (v); asm(\"\"); } while (0)\n",
        );
        write("games/tbs/include/plain.h", "#define WAIT() do { } while (0)\n");
        write("games/tbs/src/a.c", "void f(void)\n{\n\tSTEP(1);\n\tWAIT();\n}\n");
        write("games/tla/src/b.c", "void g(void)\n{\n\tSTEP(1);\n}\n");
        let counts = count(root.path()).unwrap();
        let get = |game: &str, kind: &str| counts[&(game.to_string(), kind.to_string())];
        assert_eq!(get("tbs", "asm-barrier"), 0, "a tla header's macro is not counted in tbs");
        assert_eq!(get("tla", "asm-barrier"), 1);
        assert_eq!(get("tbs", "one-pass"), 0, "a wrapper alone is no device");
        assert!(!tagged_macro_names(root.path(), "games/tbs/src/a.c", "").contains("STEP"));
        assert!(tagged_macro_names(root.path(), "games/tla/src/b.c", "").contains("STEP"));
    }

    #[test]
    fn a_c_files_macro_counts_by_the_definition_in_force_on_each_line() {
        let text = "#define PUT(w) n++
void count(void)
{
\tPUT(1);
\tPUT(2);
}
#undef PUT
/* FAKEMATCH: measured. */
#define PUT(w) do { g = (w); asm(\"\"); } while (0)
void write(void)
{
\tPUT(1);
}
#undef PUT
void later(void)
{
\tPUT(3);
}
";
        let local = in_force(&definitions(text), &BTreeMap::new());
        let found = devices_in(&code_only(text), &BTreeSet::new(), &BTreeMap::new(), &local);
        assert_eq!(found, [0, 1, 0, 0, 0, 0, 0, 0], "only the use under the tagged definition counts");
    }

    #[test]
    fn a_macro_counts_the_devices_of_the_macros_inside_it() {
        // A lock pair's two meaningful blocks and its volatile, as
        // irqlock.h writes them, and a push that wraps the pair.
        let header = "/* FAKEMATCH: measured. */
#define LOCK(saved) \\
\tdo \\
\t{ \\
\t\tvolatile u16 *ime_; \\
\t\tdo \\
\t\t{ \\
\t\t\time_ = &g_ime; \\
\t\t\tsaved = *ime_; \\
\t\t} while (0); \\
\t\t*ime_ = 0; \\
\t\tdo \\
\t\t{
/* FAKEMATCH: closes LOCK's blocks. */
#define UNLOCK(saved) \\
\t} \\
\twhile (0) \\
\t\t; \\
\tg_ime = saved; \\
\t} \\
\twhile (0)
#define PUSH(v) \\
\t{ \\
\t\tu32 saved; \\
\t\tLOCK(saved); \\
\t\tg = (v); \\
\t\tUNLOCK(saved); \\
\t}
#define TWICE(v) { PUSH(v); PUSH(v); }
#define LOOP() LOOP()
";
        let found = resolve(&definitions(header));
        assert_eq!(found.get("LOCK"), Some(&[0, 0, 0, 1, 0, 1, 0, 0]), "its read block; its leading do opens the pair's wrapper");
        assert_eq!(found.get("UNLOCK"), Some(&[0, 0, 0, 0, 0, 1, 0, 0]), "the guarded block; its last close ends the wrapper");
        assert_eq!(found.get("PUSH"), Some(&[0, 0, 0, 1, 0, 2, 0, 0]), "an untagged macro counts the devices of those it uses");
        assert_eq!(found.get("TWICE"), Some(&[0, 0, 0, 2, 0, 4, 0, 0]), "as far down as they go");
        assert!(!found.contains_key("LOOP"), "a macro that names itself expands once");
        let code = code_only("void f(void)\n{\n\tPUSH(1);\n\tPUSH(2);\n}\n");
        assert_eq!(devices(&code, &BTreeSet::new(), &found), [0, 0, 0, 2, 0, 4, 0, 0]);
        // A C file's own macro over a header's counts at each of its uses.
        let text = "#define DEFINE_WRITE(name) void name(u32 v) { PUSH(v); }\nDEFINE_WRITE(WriteA)\nDEFINE_WRITE(WriteB)\n";
        let local = in_force(&definitions(text), &found);
        assert_eq!(devices_in(&code_only(text), &BTreeSet::new(), &found, &local), [0, 0, 0, 2, 0, 4, 0, 0]);
    }

    #[test]
    fn a_volatile_in_a_macro_counts_as_it_would_written_out_tagged_or_not() {
        let header = "#define FILL(value, to) \\
\t{ \\
\t\tvolatile u32 fill_ = (value); \\
\t\tDma_Set(&fill_, (to), 0x85000000, (volatile u32 *)0x040000d4); \\
\t}
#define READ(x) (*(volatile u16 *)0x04000130 & (x))
#define TWO() { volatile u8 a_; volatile u8 b_; a_ = b_; }
";
        let found = resolve(&definitions(header));
        assert_eq!(found.get("FILL"), Some(&[0, 0, 0, 1, 0, 0, 0, 0]), "the temporary counts, the hardware register does not");
        assert!(!found.contains_key("READ"), "a hardware register alone is no device");
        assert_eq!(found.get("TWO"), Some(&[0, 0, 0, 2, 0, 0, 0, 0]), "each volatile counts");
        let written = code_only("void f(void)\n{\n\t{\n\t\tvolatile u32 fill_ = 0;\n\t\tDma_Set(&fill_, p, 0x85000000, (volatile u32 *)0x040000d4);\n\t}\n}\n");
        let used = code_only("void f(void)\n{\n\tFILL(0, p);\n}\n");
        assert_eq!(devices(&written, &BTreeSet::new(), &BTreeMap::new()), devices(&used, &BTreeSet::new(), &found));
    }

    #[test]
    fn a_loop_whose_body_always_leaves_on_its_first_pass_is_a_once_only_loop() {
        let once = [
            "while (c) { x(); break; }",
            "for (;;) { x(); break; }",
            "for (k = r; k == r;) k = r, x(); for (k = r; k == r;) { k = r; break; }",
            "while (id >= 0) { if (a) return 1; break; }",
            "for (;;) { if (a) { x(); break; } else return 2; }",
            "for (i = 0; i < 4; i++) { if (a) break; x(); goto out; }",
            "do { x(); break; } while (c);",
            "while (c) { switch (x) { case 1: y(); continue; } break; } while (d) { L: x(); break; }",
        ];
        let counted: Vec<usize> = once.iter().map(|code| once_loops_in(&code_only(code)).len()).collect();
        // The first for of the third repeats; a continue inside a switch
        // belongs to the loop, so that loop of the last can run again.
        assert_eq!(counted, [1, 1, 1, 1, 1, 1, 1, 1]);
        let code = code_only("void f(void)\n{\n\tfor (;;)\n\t{\n\t\tx();\n\t\tbreak;\n\t}\n\tdo\n\t{\n\t\tx();\n\t} while (0);\n}\n");
        assert_eq!(devices(&code, &BTreeSet::new(), &BTreeMap::new()), [0, 0, 0, 0, 0, 1, 0, 1], "do {{ }} while (0) is a one-pass block, counted once");
    }

    #[test]
    fn a_loop_that_can_run_again_is_no_device() {
        let real = [
            "for (;;) { if (a) break; x(); }",
            "while (c) { if (a) break; }",
            "for (;;) { if (a) continue; break; }",
            "while (c) { switch (x) { case 1: y(); break; } }",
            "for (;;) { while (c) { if (a) break; } }",
            "while (c) { if (a) return 1; }",
            "do { x(); if (a) break; } while (c);",
            "while (c);",
            "do x(); while (c);",
            "s = \"for (;;) { break; }\";",
            "for (;;) { if (a) { if (b) break; } else break; }",
        ];
        for code in real {
            assert!(once_loops_in(&code_only(code)).is_empty(), "{}", code);
        }
    }

    #[test]
    fn a_statement_macros_wrapper_is_no_once_only_loop_and_a_macros_loop_counts_at_each_use() {
        let header = "#define LAYER(v) \\
	do \\
	{ \\
		g = (v); \\
	} while (0)
#define ONCE(v) \\
	for (;;) \\
	{ \\
		g = (v); \\
		break; \\
	}
";
        let found = resolve(&definitions(header));
        assert!(!found.contains_key("LAYER"));
        assert_eq!(found.get("ONCE"), Some(&[0, 0, 0, 0, 0, 0, 0, 1]));
        let code = code_only("void f(void)\n{\n\tLAYER(1);\n\tONCE(2);\n\tONCE(3);\n}\n");
        assert_eq!(devices(&code, &BTreeSet::new(), &found), [0, 0, 0, 0, 0, 0, 0, 2]);
    }

    #[test]
    fn each_editions_branch_is_read_whole() {
        // Each branch opens the brace the shared code closes: read as one
        // text the braces would not balance, and the real loop would be
        // taken for a once-only one.
        let text = "void f(void)\n{\n\tfor (;;)\n\t{\n\t\tif (a)\n\t\t\tbreak;\n#if EDITION_INTERNATIONAL\n\t\tif (b)\n\t\t{\n#else\n\t\tif (c)\n\t\t{\n#endif\n\t\t\tx();\n\t\t}\n\t\tWaitFrames(1);\n\t}\n\twhile (d)\n\t{\n#if EDITION_JA\n\t\treturn 1;\n#else\n\t\tbreak;\n#endif\n\t}\n}\n";
        assert_eq!(once_loops_in(&code_only(text)), [(18, 1)]);
        assert_eq!(devices(&code_only(text), &BTreeSet::new(), &BTreeMap::new())[7], 1);
    }
}
