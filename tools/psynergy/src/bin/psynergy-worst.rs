//! `psynergy-worst`: maintained C ranked by how mechanical it reads, worst
//! first, into a TSV for people choosing what to rewrite next.

use psynergy::worst;
use std::fmt::Write as _;
use std::path::PathBuf;

const USAGE: &str = "usage: psynergy-worst --out FILE [--root DIR] [--show N]\n\
Ranks every function in ROOT/games/**/*.C by weighted mechanical signals\n\
(FAKEMATCH devices, inline asm, byte-offset pointer arithmetic, placeholder\n\
names, dereferenced casts, one-pass blocks, goto, volatile, hex literals,\n\
value casts), writes them worst first to FILE and prints the first N.";

fn main() {
    if let Err(error) = run() {
        eprintln!("{error}");
        std::process::exit(1);
    }
}

fn run() -> Result<(), String> {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    let (mut root, mut out, mut show) = (PathBuf::from("."), None, 30usize);
    let mut i = 0;
    while i < arguments.len() {
        let value = arguments.get(i + 1).ok_or_else(|| USAGE.to_string())?;
        match arguments[i].as_str() {
            "--root" => root = PathBuf::from(value),
            "--out" => out = Some(PathBuf::from(value)),
            "--show" => show = value.parse().map_err(|_| USAGE.to_string())?,
            _ => return Err(USAGE.to_string()),
        }
        i += 2;
    }
    let out = out.ok_or_else(|| USAGE.to_string())?;
    let ranked = worst::rank_tree(&root)?;
    let mut tsv = String::from("rank\tscore\tdensity\tlines\tfunction\tsource\tsignals\n");
    for (rank, r) in ranked.iter().enumerate() {
        let signals: Vec<String> = r.signals.iter().filter(|(_, n)| *n > 0).map(|(s, n)| format!("{s}={n}")).collect();
        let _ = writeln!(tsv, "{}\t{}\t{:.1}\t{}\t{}\t{}:{}\t{}", rank + 1, r.score, r.density(), r.lines, r.name, r.file, r.line, signals.join(","));
    }
    if let Some(dir) = out.parent() {
        std::fs::create_dir_all(dir).map_err(|e| e.to_string())?;
    }
    std::fs::write(&out, &tsv).map_err(|e| format!("{}: {e}", out.display()))?;
    for line in tsv.lines().take(show + 1) {
        println!("{line}");
    }
    let total: u32 = ranked.iter().map(|r| r.score).sum();
    println!("{} functions, total score {total}; {}", ranked.len(), out.display());
    // Camelotized: the share of functions, and of their lines, with no
    // mechanical signal at all, for each game and the shared code.
    for (label, needle) in [("☀️", "games/THE BROKEN SEAL/"), ("⚓️", "games/THE LOST AGE/"), ("COMMON", "games/COMMON/")] {
        let part: Vec<_> = ranked.iter().filter(|r| r.file.starts_with(needle)).collect();
        let lines: usize = part.iter().map(|r| r.lines).sum();
        let clean: Vec<_> = part.iter().filter(|r| r.score == 0).collect();
        let clean_lines: usize = clean.iter().map(|r| r.lines).sum();
        println!(
            "{label} camelotized: {:.1}% of {} functions, {:.1}% of their lines",
            100.0 * clean.len() as f64 / part.len().max(1) as f64,
            part.len(),
            100.0 * clean_lines as f64 / lines.max(1) as f64
        );
    }
    Ok(())
}
