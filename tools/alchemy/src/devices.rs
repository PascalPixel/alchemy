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
    // Macros whose body holds a device count at each use, by that device
    // (S2: every device is counted, a macro's too). A macro a header
    // defines reaches its own game and, from games/common, both; a macro
    // a C file defines reaches only that file.
    let mut header_macros: BTreeMap<&str, BTreeMap<String, Vec<usize>>> = BTreeMap::new();
    for (path, text) in &sources {
        if !path.ends_with(".h") {
            continue;
        }
        for (game, folder, _) in &games {
            if path.starts_with(folder) || path.starts_with("games/common") {
                header_macros.entry(game).or_default().extend(tagged_macros(text));
            }
        }
    }
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
            let tagged = header_macros.get(game).cloned().unwrap_or_default();
            let local = if path.ends_with(".c") { definitions(text) } else { Vec::new() };
            let found = devices_in(&without_proven(&code_only(text)), &emitting, &tagged, &local);
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
/// device, that the C file at `path` can use: those its own game's headers
/// and games/common's headers define (a games/common file sees both games'
/// headers), and those it defines itself. Headers are read once per run.
pub(crate) fn tagged_macro_names(root: &Path, path: &str, text: &str) -> BTreeSet<String> {
    type ByFolder = BTreeMap<String, BTreeSet<String>>;
    static HEADERS: std::sync::OnceLock<std::sync::Mutex<BTreeMap<std::path::PathBuf, ByFolder>>> = std::sync::OnceLock::new();
    let mut cache = HEADERS.get_or_init(Default::default).lock().unwrap_or_else(|e| e.into_inner());
    let headers = cache.entry(root.to_path_buf()).or_insert_with(|| {
        let mut by_folder: BTreeMap<String, BTreeSet<String>> = BTreeMap::new();
        for entry in walkdir::WalkDir::new(root.join("games")).into_iter().flatten() {
            let file = entry.path();
            if file.extension().and_then(|e| e.to_str()) != Some("h") {
                continue;
            }
            let relative = file.strip_prefix(root).unwrap_or(file).to_string_lossy().into_owned();
            let folder = relative.split('/').take(2).collect::<Vec<_>>().join("/");
            if let Ok(bytes) = fs::read(file) {
                by_folder
                    .entry(folder)
                    .or_default()
                    .extend(tagged_macros(&String::from_utf8_lossy(&bytes)).into_iter().map(|(name, _)| name));
            }
        }
        by_folder
    });
    let own = path.split('/').take(2).collect::<Vec<_>>().join("/");
    let mut names: BTreeSet<String> = headers
        .iter()
        .filter(|(folder, _)| own == "games/common" || folder.as_str() == own || folder.as_str() == "games/common")
        .flat_map(|(_, names)| names.iter().cloned())
        .collect();
    names.extend(tagged_macros(text).into_iter().map(|(name, _)| name));
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

/// Each macro whose body holds a device, with the KINDS indexes of those
/// devices, so a device counts at every use (S2). A FAKEMATCH tag in the
/// comment right above its #define, or in its body, explains it; a tagged
/// body with no device the count knows by kind (a forced temporary, a
/// cast) is a tagged-macro use. An untagged macro counts its pins, barriers
/// and one-pass blocks all the same: dropping a tag hides nothing.
fn tagged_macros(text: &str) -> Vec<(String, Vec<usize>)> {
    definitions(text)
        .into_iter()
        .filter_map(|(_, name, kinds)| kinds.map(|kinds| (name, kinds)))
        .collect()
}

/// Every #define and #undef of a text, in order: its line, its name, and
/// the device kinds of a #define whose body holds a device (None for an
/// #undef or a plain #define). A C file's macro counts from the #define
/// that holds its device to the next #define or #undef of its name.
fn definitions(text: &str) -> Vec<(usize, String, Option<Vec<usize>>)> {
    let lines: Vec<&str> = text.lines().collect();
    let define = regex::Regex::new(r"^\s*#\s*define\s+([A-Za-z_][A-Za-z0-9_]*)").expect("static pattern");
    let undef = regex::Regex::new(r"^\s*#\s*undef\s+([A-Za-z_][A-Za-z0-9_]*)").expect("static pattern");
    let comment = regex::Regex::new(r"^\s*(/\*|\*|//)|\*/\s*$").expect("static pattern");
    let mut found = Vec::new();
    for (i, line) in lines.iter().enumerate() {
        if let Some(name) = undef.captures(line).map(|c| c[1].to_string()) {
            found.push((i, name, None));
            continue;
        }
        let Some(name) = define.captures(line).map(|c| c[1].to_string()) else { continue };
        let mut body = line.to_string();
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
        let mut kinds = Vec::new();
        if templates.iter().any(|t| register(t.trim())) {
            kinds.push(0);
        }
        if templates.iter().any(|t| t.trim().is_empty()) {
            kinds.push(1);
        }
        let volatile = code.matches("volatile").count() > templates.len();
        if tagged && volatile {
            kinds.push(3);
        }
        if one_pass(&without_statement_wrapper(&code)) > 0 {
            kinds.push(5);
        }
        if tagged && kinds.is_empty() {
            kinds.push(6);
        }
        found.push((i, name, (!kinds.is_empty()).then_some(kinds)));
    }
    found
}

/// One-pass blocks: `do { ... } while (0)`, which ends CSE's block and is a
/// scheduling barrier (S2 names it a device).
fn one_pass(code: &str) -> usize {
    regex::Regex::new(r"\}\s*while\s*\(\s*0\s*\)").expect("static pattern").find_iter(code).count()
}

/// Counts in KINDS order over one file, outside #define bodies (a macro's
/// devices count where it is used).
type Definitions = [(usize, String, Option<Vec<usize>>)];

#[cfg(test)]
fn devices(code: &str, emitting: &BTreeSet<String>, tagged: &BTreeMap<String, Vec<usize>>) -> [usize; 7] {
    devices_in(code, emitting, tagged, &[])
}

/// As `devices`, where `local` holds the file's own #define and #undef
/// lines: a use counts by the definition in force on its line.
fn devices_in(code: &str, emitting: &BTreeSet<String>, tagged: &BTreeMap<String, Vec<usize>>, local: &Definitions) -> [usize; 7] {
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
            let in_force = local.iter().rev().find(|(at, name, _)| *at < number && name == found.as_str());
            let kinds = match in_force {
                Some((_, _, kinds)) => kinds.as_ref(),
                None => tagged.get(found.as_str()),
            };
            if let Some(kinds) = kinds {
                for kind in kinds {
                    macro_uses[*kind] += 1;
                }
            }
        }
        if line.contains("volatile") && !hardware(line) {
            volatile += line.matches("volatile").count()
                - line.matches("asm volatile").count()
                - line.matches("__asm__ volatile").count()
                - line.matches("asm __volatile__").count()
                - line.matches("__asm__ __volatile__").count();
        }
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
        let tagged: BTreeMap<String, Vec<usize>> = tagged_macros(header).into_iter().collect();
        // SET's only block is the statement macro's own wrapper: its tag
        // still counts it, as a tagged macro, not as a one-pass block.
        assert_eq!(tagged.get("SET"), Some(&vec![6]));
        assert_eq!(tagged.get("TEMP"), Some(&vec![6]));
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
        let found: BTreeMap<String, Vec<usize>> = tagged_macros(header).into_iter().collect();
        assert!(!found.contains_key("LAYER"), "the wrapper alone is no device");
        assert_eq!(found.get("INNER"), Some(&vec![5]), "an untagged macro's inner block still counts");
        assert_eq!(found.get("HALF"), Some(&vec![5]), "a macro closing another's blocks is no wrapper");
        assert_eq!(found.get("BARRIER"), Some(&vec![1]));
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
        let local = definitions(text);
        let found = devices_in(&code_only(text), &BTreeSet::new(), &BTreeMap::new(), &local);
        assert_eq!(found, [0, 1, 0, 0, 0, 0, 0], "only the use under the tagged definition counts");
    }
}
