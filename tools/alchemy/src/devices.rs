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

const USAGE: &str = "usage: alchemy check devices [--update]\n\
Counts, for each game, register pins (register ... asm(\"rN\")), empty asm\n\
barriers, instruction-emitting inline asm other than swi (directly or through\n\
a macro), volatile on objects that are not hardware (a heuristic), and\n\
goto statements (a device that needs no tag; library C apart), and\n\
fails when a count rises above tools/alchemy/devices.tsv. --update writes\n\
the current counts there; lower them whenever a device goes.";

pub(crate) const KINDS: [&str; 7] =
    ["register-pin", "asm-barrier", "inline-asm", "volatile", "goto", "one-pass", "tagged-macro"];

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
        "; devices: {} register pins, {} asm barriers, {} inline asm, {} volatile, {} goto, {} one-pass blocks, {} other tagged macro uses",
        get("register-pin"),
        get("asm-barrier"),
        get("inline-asm"),
        get("volatile"),
        get("goto"),
        get("one-pass"),
        get("tagged-macro")
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
        if !path.ends_with(".h") {
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
            let local = if path.ends_with(".c") { in_force(&definitions(text), &headers) } else { Vec::new() };
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
                by_folder.entry(folder).or_default().extend(definitions(&String::from_utf8_lossy(&bytes)));
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
type Kinds = [usize; 7];

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
        let mut own = [0; 7];
        own[0] = templates.iter().filter(|t| register(t.trim())).count();
        own[1] = templates.iter().filter(|t| t.trim().is_empty()).count();
        own[3] = code.lines().map(volatiles).sum();
        own[5] = one_pass(&without_statement_wrapper(&format!("#define {name}{code}")));
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
        let Some((own, words)) = bodies.get(name) else { return [0; 7] };
        // A macro that names itself, directly or through another, is not
        // expanded again (C stops there too).
        if !open.insert(name) {
            return [0; 7];
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
                    Some((_, _, kinds)) => kinds.unwrap_or([0; 7]),
                    None => headers.get(word).copied().unwrap_or([0; 7]),
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

/// A file's own macros, each with its devices from the line it is defined
/// (None from an #undef).
type InForce = [(usize, String, Option<Kinds>)];

#[cfg(test)]
fn devices(code: &str, emitting: &BTreeSet<String>, headers: &BTreeMap<String, Kinds>) -> [usize; 7] {
    devices_in(code, emitting, headers, &[])
}

/// Counts in KINDS order over one file, outside #define bodies: a macro's
/// devices count where it is used, by the file's own definition in force
/// on that line (`local`, from `in_force`) and otherwise by its headers'.
fn devices_in(code: &str, emitting: &BTreeSet<String>, headers: &BTreeMap<String, Kinds>, local: &InForce) -> [usize; 7] {
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
    let templates = asm_statements(&plain);
    let pins = templates.iter().filter(|t| register(t.trim())).count();
    let barriers = templates.iter().filter(|t| t.trim().is_empty()).count();
    let mut inline = templates.iter().filter(|t| instruction(t)).count();
    let word = regex::Regex::new(r"[A-Za-z_][A-Za-z0-9_]*").expect("static pattern");
    let mut volatile = 0;
    let mut gotos = 0;
    let mut macro_uses = [0usize; 7];
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
            for (kind, n) in macro_uses.iter_mut().zip(kinds.unwrap_or([0; 7])) {
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
    ]
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
        assert_eq!(devices("asm(\"swi 2; mov r0, r1\"); asm(\"nop\");", &BTreeSet::new(), &BTreeMap::new()), [0, 0, 2, 0, 0, 0, 0]);
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
        assert_eq!(devices(&code, &emitting, &BTreeMap::new()), [1, 2, 3, 1, 0, 0, 0]);
    }

    #[test]
    fn a_macro_body_counts_where_it_is_used() {
        let code = code_only("#define STEER(x) \\\n    asm(\"mov %0, %0\" : \"+r\"(x))\n");
        let bodies = macro_bodies(&code);
        assert_eq!(bodies[0].0, "STEER");
        assert!(asm_statements(&bodies[0].1).iter().any(|t| instruction(t)));
        assert_eq!(devices(&code, &BTreeSet::new(), &BTreeMap::new()), [0, 0, 0, 0, 0, 0, 0]);
    }

    #[test]
    fn gnu_asm_qualifiers_are_not_volatile_objects() {
        let code = code_only(
            "__asm__ __volatile__(\"\" : : \"r\"(value));\n\
             volatile s32 counter; __asm__ __volatile__(\"\" : : \"r\"(counter));\n",
        );
        assert_eq!(devices(&code, &BTreeSet::new(), &BTreeMap::new()), [0, 2, 0, 1, 0, 0, 0]);
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
        assert_eq!(devices(&without_proven(&code), &BTreeSet::new(), &BTreeMap::new()), [2, 1, 1, 0, 0, 0, 0]);
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
        assert_eq!(devices(&code, &BTreeSet::new(), &BTreeMap::new()), [0, 0, 0, 0, 2, 0, 0]);
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
        assert_eq!(tagged.get("SET"), Some(&[0, 0, 0, 0, 0, 0, 1]));
        assert_eq!(tagged.get("TEMP"), Some(&[0, 0, 0, 0, 0, 0, 1]));
        assert!(!tagged.contains_key("PLAIN"));
        let code = code_only("void f(void)\n{\n\tSET(1);\n\tSET(2);\n\tPLAIN(3);\n\tTEMP(4);\n\tdo\n\t{\n\t\tg = 5;\n\t} while (0);\n}\n");
        assert_eq!(devices(&code, &BTreeSet::new(), &tagged), [0, 0, 0, 0, 0, 1, 3]);
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
        assert_eq!(found.get("INNER"), Some(&[0, 0, 0, 0, 0, 1, 0]), "an untagged macro's inner block still counts");
        assert_eq!(found.get("HALF"), Some(&[0, 0, 0, 0, 0, 1, 0]), "a macro closing another's inner block counts it; its last close ends their wrapper");
        assert_eq!(found.get("BARRIER"), Some(&[0, 1, 0, 0, 0, 0, 0]));
        let code = code_only("void f(void)\n{\n\tLAYER(1);\n\tINNER(2);\n\tINNER(3);\n\tBARRIER();\n}\n");
        assert_eq!(devices(&code, &BTreeSet::new(), &found), [0, 1, 0, 0, 0, 2, 0]);
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
        assert_eq!(found, [0, 1, 0, 0, 0, 0, 0], "only the use under the tagged definition counts");
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
        assert_eq!(found.get("LOCK"), Some(&[0, 0, 0, 1, 0, 1, 0]), "its read block; its leading do opens the pair's wrapper");
        assert_eq!(found.get("UNLOCK"), Some(&[0, 0, 0, 0, 0, 1, 0]), "the guarded block; its last close ends the wrapper");
        assert_eq!(found.get("PUSH"), Some(&[0, 0, 0, 1, 0, 2, 0]), "an untagged macro counts the devices of those it uses");
        assert_eq!(found.get("TWICE"), Some(&[0, 0, 0, 2, 0, 4, 0]), "as far down as they go");
        assert!(!found.contains_key("LOOP"), "a macro that names itself expands once");
        let code = code_only("void f(void)\n{\n\tPUSH(1);\n\tPUSH(2);\n}\n");
        assert_eq!(devices(&code, &BTreeSet::new(), &found), [0, 0, 0, 2, 0, 4, 0]);
        // A C file's own macro over a header's counts at each of its uses.
        let text = "#define DEFINE_WRITE(name) void name(u32 v) { PUSH(v); }\nDEFINE_WRITE(WriteA)\nDEFINE_WRITE(WriteB)\n";
        let local = in_force(&definitions(text), &found);
        assert_eq!(devices_in(&code_only(text), &BTreeSet::new(), &found, &local), [0, 0, 0, 2, 0, 4, 0]);
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
        assert_eq!(found.get("FILL"), Some(&[0, 0, 0, 1, 0, 0, 0]), "the temporary counts, the hardware register does not");
        assert!(!found.contains_key("READ"), "a hardware register alone is no device");
        assert_eq!(found.get("TWO"), Some(&[0, 0, 0, 2, 0, 0, 0]), "each volatile counts");
        let written = code_only("void f(void)\n{\n\t{\n\t\tvolatile u32 fill_ = 0;\n\t\tDma_Set(&fill_, p, 0x85000000, (volatile u32 *)0x040000d4);\n\t}\n}\n");
        let used = code_only("void f(void)\n{\n\tFILL(0, p);\n}\n");
        assert_eq!(devices(&written, &BTreeSet::new(), &BTreeMap::new()), devices(&used, &BTreeSet::new(), &found));
    }
}
