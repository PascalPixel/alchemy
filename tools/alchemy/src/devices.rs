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
a macro), and volatile on objects that are not hardware (a heuristic), and\n\
fails when a count rises above tools/alchemy/devices.tsv. --update writes\n\
the current counts there; lower them whenever a device goes.";

pub(crate) const KINDS: [&str; 4] = ["register-pin", "asm-barrier", "inline-asm", "volatile"];

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
        "; devices: {} register pins, {} asm barriers, {} inline asm, {} volatile",
        get("register-pin"),
        get("asm-barrier"),
        get("inline-asm"),
        get("volatile")
    )
}

pub(crate) fn count(root: &Path) -> Result<Counts, String> {
    let games = [("tbs", "games/THE BROKEN SEAL", CompilerTarget::Tbs), ("tla", "games/THE LOST AGE", CompilerTarget::Tla)];
    let mut sources: Vec<(String, String)> = Vec::new();
    for entry in walkdir::WalkDir::new(root.join("games")).sort_by_file_name() {
        let entry = entry.map_err(|e| e.to_string())?;
        let path = entry.path().strip_prefix(root).unwrap_or(entry.path()).to_string_lossy().into_owned();
        if !(path.ends_with(".C") || path.ends_with(".H")) {
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
            if !proven.contains(&name) && asm_statements(&body).iter().any(|t| instruction(t)) {
                emitting.insert(name);
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
            if !(path.starts_with(folder) || path.starts_with("games/COMMON")) {
                continue;
            }
            if path.ends_with(".C")
                && matches!(
                    family_for_source(*target, path),
                    CompilerFamily::Agbcc | CompilerFamily::AgbccFlash | CompilerFamily::AgbccArm
                )
            {
                continue;
            }
            let found = devices(&without_proven(&code_only(text)), &emitting);
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

/// The end of the `CAMELOT_ASM(...)` marker starting at `start`, and where the
/// code it marks begins.
fn marker_end(code: &str, start: usize) -> usize {
    let mut depth = 0;
    for (offset, c) in code[start..].char_indices() {
        match c {
            '(' => depth += 1,
            ')' => {
                depth -= 1;
                if depth == 0 {
                    return start + offset + 1;
                }
            }
            _ => {}
        }
    }
    code.len()
}

/// Names of the macros a `CAMELOT_ASM` proof marks: proven handwritten asm
/// helpers (S2), not devices.
fn proven_macros(code: &str) -> BTreeSet<String> {
    let mut found = BTreeSet::new();
    for (start, _) in code.match_indices("CAMELOT_ASM(") {
        let rest = code[marker_end(code, start)..].trim_start();
        if let Some(define) = rest.strip_prefix('#').map(str::trim_start).and_then(|r| r.strip_prefix("define")) {
            let define = define.trim_start();
            let end = define.find(|c: char| !c.is_ascii_alphanumeric() && c != '_').unwrap_or(define.len());
            found.insert(define[..end].to_string());
        }
    }
    found
}

/// The code with what each `CAMELOT_ASM` proof marks blanked: a `static`
/// helper's whole definition, or else the one statement that follows.
fn without_proven(code: &str) -> String {
    let mut out: Vec<char> = code.chars().collect();
    let byte_to_char: Vec<usize> = {
        let mut map = vec![0; code.len() + 1];
        let mut n = 0;
        for (i, c) in code.char_indices() {
            for slot in map.iter_mut().skip(i).take(c.len_utf8()) {
                *slot = n;
            }
            n += 1;
        }
        map[code.len()] = n;
        map
    };
    for (start, _) in code.match_indices("CAMELOT_ASM(") {
        let from = marker_end(code, start);
        let rest = &code[from..];
        let lead = rest.len() - rest.trim_start().len();
        let body = from + lead;
        if code[body..].starts_with('#') {
            continue;
        }
        let end = if code[body..].starts_with("static") {
            let mut depth = 0;
            let mut seen = false;
            let mut end = code.len();
            for (offset, c) in code[body..].char_indices() {
                match c {
                    '{' => {
                        depth += 1;
                        seen = true;
                    }
                    '}' => {
                        depth -= 1;
                        if seen && depth == 0 {
                            end = body + offset + 1;
                            break;
                        }
                    }
                    ';' if !seen => {
                        end = body + offset + 1;
                        break;
                    }
                    _ => {}
                }
            }
            end
        } else {
            code[body..].find(';').map_or(code.len(), |e| body + e + 1)
        };
        for slot in &mut out[byte_to_char[body]..byte_to_char[end]] {
            if *slot != '\n' {
                *slot = ' ';
            }
        }
    }
    out.into_iter().collect()
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
        && !template.bytes().all(|b| b.is_ascii_alphanumeric() || b == b'_' || b == b'.')
        && !template.split(|c: char| !c.is_ascii_alphanumeric()).any(|word| word == "swi")
}

/// Counts in KINDS order over one file, outside #define bodies (a macro's
/// devices count where it is used).
fn devices(code: &str, emitting: &BTreeSet<String>) -> [usize; 4] {
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
    for line in plain.lines() {
        for found in word.find_iter(line) {
            if emitting.contains(found.as_str()) {
                inline += 1;
            }
        }
        if line.contains("volatile") && !hardware(line) {
            volatile += line.matches("volatile").count()
                - line.matches("asm volatile").count()
                - line.matches("__asm__ volatile").count()
                - line.matches("asm __volatile__").count();
        }
    }
    [pins, barriers, inline, volatile]
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
        assert_eq!(devices(&code, &emitting), [1, 2, 2, 1]);
    }

    #[test]
    fn a_macro_body_counts_where_it_is_used() {
        let code = code_only("#define STEER(x) \\\n    asm(\"mov %0, %0\" : \"+r\"(x))\n");
        let bodies = macro_bodies(&code);
        assert_eq!(bodies[0].0, "STEER");
        assert!(asm_statements(&bodies[0].1).iter().any(|t| instruction(t)));
        assert_eq!(devices(&code, &BTreeSet::new()), [0, 0, 0, 0]);
    }

    #[test]
    fn a_proven_helper_is_not_a_device() {
        let code = code_only(
            "CAMELOT_ASM(\"stmia\")\n\
             static __inline__ void Set(u32 a)\n{\n    register u32 r __asm__(\"r0\") = a;\n    __asm__ volatile(\"stmia %0!, {%0}\" : \"+r\"(r));\n}\n\
             CAMELOT_ASM(\"identity\")\n#define IDENT() asm(\"stmia r0!, {r1}\")\n\
             void f(void)\n{\n    CAMELOT_ASM(\"stack switch\")\n    register u8 *sp __asm__(\"sp\");\n    IDENT();\n    asm(\"\");\n}\n",
        );
        assert!(proven_macros(&code).contains("IDENT"));
        assert_eq!(devices(&without_proven(&code), &BTreeSet::new()), [0, 1, 0, 0]);
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
}
