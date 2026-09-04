//! Greedy source-shape search against the real scorer.
//!
//! The scorer costs about 0.08s when its release binary is invoked directly,
//! so a few hundred legal respellings of an owner cost seconds, not minutes.
//! This searches that space automatically and keeps only what lowers
//! differing_halfwords. No forced registers, no scheduling flags: every
//! candidate is a legal respelling.
//!
//!   shape-search <owner-hex> [rounds]

use regex::Regex;
use std::collections::BTreeSet;
use std::fs;
use std::path::PathBuf;
use std::process::{Command, ExitCode};

struct Variant {
    label: String,
    text: String,
}

fn main() -> ExitCode {
    match run() {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
    }
}

fn run() -> Result<(), String> {
    let args: Vec<String> = std::env::args().skip(1).collect();
    let owner = args
        .first()
        .cloned()
        .ok_or("usage: shape-search <owner-hex> [rounds]")?;
    let rounds: usize = args.get(1).and_then(|r| r.parse().ok()).unwrap_or(4);
    let repo = std::env::current_dir().map_err(|e| e.to_string())?;
    let relative = format!("games/gs1/recon/en/main/{owner}.c");
    let file: PathBuf = repo.join(&relative);
    let scorer = repo.join("out/cargo-target/release/compiler");
    let score = |_: &()| -> u64 {
        let output = Command::new(&scorer)
            .current_dir(&repo)
            .args(["candidate-show", &relative, "--owner", &owner])
            .output();
        let Ok(output) = output else { return u64::MAX };
        let text = String::from_utf8_lossy(&output.stdout);
        text.lines()
            .find_map(|line| line.split("differing_halfwords=").nth(1))
            .and_then(|rest| rest.split_whitespace().next())
            .and_then(|n| n.parse().ok())
            .unwrap_or(u64::MAX)
    };
    let original = fs::read_to_string(&file).map_err(|e| format!("{}: {e}", file.display()))?;
    let mut best = original.clone();
    let mut best_score = score(&());
    println!("{owner} baseline dh={}", show(best_score));
    if best_score == u64::MAX {
        println!("  baseline does not compile; nothing to search");
        return Ok(());
    }
    let mut tried = 0;
    for round in 0..rounds {
        if best_score == 0 {
            break;
        }
        let mut variants = Vec::new();
        variants.extend(swaps(&best));
        variants.extend(distant_swaps(&best));
        variants.extend(guard_forms(&best));
        variants.extend(compound_forms(&best));
        variants.extend(retypes(&best));
        variants.extend(commutes(&best));
        let mut improved = false;
        for v in variants {
            if v.text == best {
                continue;
            }
            fs::write(&file, &v.text).map_err(|e| e.to_string())?;
            tried += 1;
            let s = score(&());
            if s < best_score {
                println!(
                    "  round {round}: {} -> dh={s} (was {})",
                    v.label,
                    show(best_score)
                );
                best_score = s;
                best = v.text;
                improved = true;
                break; // restart generation from the improved source
            }
        }
        if !improved {
            break;
        }
    }
    fs::write(&file, &best).map_err(|e| e.to_string())?;
    println!(
        "{owner} FINAL dh={} after {tried} variants",
        show(best_score)
    );
    Ok(())
}

fn show(score: u64) -> String {
    if score == u64::MAX {
        "inf".into()
    } else {
        score.to_string()
    }
}

// ---- variant generators -----------------------------------------------

fn body_range(src: &str) -> Option<(usize, usize)> {
    let from = src.rfind("\n)").unwrap_or(0);
    let open = from + src[from..].find('{')?;
    let mut depth = 0i32;
    for (i, c) in src[open..].char_indices() {
        if c == '{' {
            depth += 1;
        } else if c == '}' {
            depth -= 1;
            if depth == 0 {
                return Some((open + 1, open + i));
            }
        }
    }
    None
}

/// Top-level statements inside the function body, as (start, end) spans.
fn statements(src: &str, from: usize, to: usize) -> Vec<(usize, usize)> {
    let mut spans = Vec::new();
    let mut depth = 0i32;
    let mut start = from;
    let bytes = src.as_bytes();
    for i in from..to {
        let c = bytes[i] as char;
        if c == '{' {
            depth += 1;
        } else if c == '}' {
            depth -= 1;
            if depth == 0 {
                spans.push((start, i + 1));
                start = i + 1;
            }
        } else if c == ';' && depth == 0 {
            spans.push((start, i + 1));
            start = i + 1;
        }
    }
    spans
        .into_iter()
        .filter(|(a, b)| !src[*a..*b].trim().is_empty())
        .collect()
}

fn is_decl(t: &str) -> bool {
    let re = Regex::new(r"^\s*(?:const\s+)?(?:struct\s+\w+\s*\*?|unsigned\s+\w+|signed\s+\w+|[su]\d+|u8|void|int|char|short|long)\s+\**\w+").unwrap();
    re.is_match(t)
}

fn swaps(src: &str) -> Vec<Variant> {
    let Some((from, to)) = body_range(src) else {
        return Vec::new();
    };
    let spans = statements(src, from, to);
    let mut out = Vec::new();
    for i in 0..spans.len().saturating_sub(1) {
        let (a1, b1) = spans[i];
        let (a2, b2) = spans[i + 1];
        let first = &src[a1..b1];
        let second = &src[a2..b2];
        // never reorder across a declaration/statement boundary illegally in
        // C89: a declaration may not follow a statement.
        if is_decl(second) && !is_decl(first) {
            continue;
        }
        out.push(Variant {
            label: format!("swap@{i}"),
            text: format!("{}{}{}{}", &src[..a1], second, first, &src[b2..]),
        });
    }
    out
}

fn retypes(src: &str) -> Vec<Variant> {
    let mut out = Vec::new();
    let pairs = [
        ("s32", "u32"),
        ("u32", "s32"),
        ("s16", "u16"),
        ("u16", "s16"),
        ("s32", "s16"),
    ];
    for (from, to) in pairs {
        let re = Regex::new(&format!(r"\b{from} (\w+);")).unwrap();
        let names: BTreeSet<String> = re.captures_iter(src).map(|c| c[1].to_string()).collect();
        for n in names {
            let decl = format!("{from} {n};");
            out.push(Variant {
                label: format!("retype {n}:{from}->{to}"),
                text: src.replacen(&decl, &format!("{to} {n};"), 1),
            });
        }
    }
    out
}

fn commutes(src: &str) -> Vec<Variant> {
    let mut out = Vec::new();
    let re =
        Regex::new(r"([A-Za-z_][\w.\->\[\]]*) (\*|\+|\||&|\^) ([A-Za-z_][\w.\->\[\]]*)").unwrap();
    let mut seen = BTreeSet::new();
    for m in re.captures_iter(src) {
        let whole = m[0].to_string();
        if !seen.insert(whole.clone()) {
            continue;
        }
        let flipped = format!("{} {} {}", &m[3], &m[2], &m[1]);
        out.push(Variant {
            label: format!("commute {whole}"),
            text: src.replace(&whole, &flipped),
        });
    }
    out
}

fn distant_swaps(src: &str) -> Vec<Variant> {
    let Some((from, to)) = body_range(src) else {
        return Vec::new();
    };
    let spans = statements(src, from, to);
    let mut out = Vec::new();
    for i in 0..spans.len() {
        for j in (i + 2)..spans.len().min(i + 6) {
            let a = &src[spans[i].0..spans[i].1];
            let b = &src[spans[j].0..spans[j].1];
            if is_decl(a) != is_decl(b) {
                continue;
            }
            let text = format!(
                "{}{}{}{}{}",
                &src[..spans[i].0],
                b,
                &src[spans[i].1..spans[j].0],
                a,
                &src[spans[j].1..]
            );
            out.push(Variant {
                label: format!("move@{i}<->{j}"),
                text,
            });
        }
    }
    out
}

fn guard_forms(src: &str) -> Vec<Variant> {
    let mut out = Vec::new();
    // Invert simple equality guards with an else arm: `if (a == b) {A} else {B}`
    // at one indent becomes `if (a != b) {B} else {A}`.
    let head = Regex::new(r"if \(([^()]+?) == ([^()]+?)\) \{").unwrap();
    for m in head.captures_iter(src) {
        let start = m.get(0).unwrap().start();
        let open = m.get(0).unwrap().end() - 1;
        let Some(close_then) = matching_brace(src, open) else {
            continue;
        };
        let after = &src[close_then + 1..];
        let Some(rest) = after.strip_prefix(" else {") else {
            continue;
        };
        let open_else = close_then + 1 + " else {".len() - 1;
        let Some(close_else) = matching_brace(src, open_else) else {
            continue;
        };
        let then_body = &src[open + 1..close_then];
        let else_body = &src[open_else + 1..close_else];
        let _ = rest;
        let flipped = format!(
            "if ({} != {}) {{{}}} else {{{}}}",
            &m[1], &m[2], else_body, then_body
        );
        out.push(Variant {
            label: "invert if/else".into(),
            text: format!("{}{}{}", &src[..start], flipped, &src[close_else + 1..]),
        });
    }
    // Relational mirror.
    for (a, b) in [("<", ">"), (">", "<"), ("<=", ">="), (">=", "<=")] {
        let rel = Regex::new(&format!(
            r"if \((\w[\w.\->\[\]]*) {} (\w[\w.\->\[\]]*)\)",
            regex::escape(a)
        ))
        .unwrap();
        for k in rel.captures_iter(src) {
            out.push(Variant {
                label: format!("mirror {}", &k[0]),
                text: src.replacen(&k[0], &format!("if ({} {b} {})", &k[2], &k[1]), 1),
            });
        }
    }
    out
}

fn matching_brace(src: &str, open: usize) -> Option<usize> {
    let mut depth = 0i32;
    for (i, c) in src[open..].char_indices() {
        if c == '{' {
            depth += 1;
        } else if c == '}' {
            depth -= 1;
            if depth == 0 {
                return Some(open + i);
            }
        }
    }
    None
}

fn compound_forms(src: &str) -> Vec<Variant> {
    let mut out = Vec::new();
    let re = Regex::new(r"(\s+)([\w.\->\[\]]+) (\+|\||&|\^|\*|-)= ([^;]+);").unwrap();
    for m in re.captures_iter(src) {
        out.push(Variant {
            label: format!("expand {} {}=", &m[2], &m[3]),
            text: src.replacen(
                &m[0],
                &format!("{}{} = {} {} {};", &m[1], &m[2], &m[2], &m[3], &m[4]),
                1,
            ),
        });
    }
    // `x = x op y;` contracts to `x op= y;` when the left operand repeats.
    let re2 =
        Regex::new(r"(\s+)([\w.\->\[\]]+) = ([\w.\->\[\]]+) (\+|\||&|\^|\*) ([^;]+);").unwrap();
    for m in re2.captures_iter(src) {
        if m[2] != m[3] {
            continue;
        }
        out.push(Variant {
            label: format!("contract {}", &m[2]),
            text: src.replacen(
                &m[0],
                &format!("{}{} {}= {};", &m[1], &m[2], &m[4], &m[5]),
                1,
            ),
        });
    }
    out
}
