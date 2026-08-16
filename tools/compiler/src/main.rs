//! `compiler`: one executable for the compiler-side searches.
//!
//! Tenth consolidation. See docs/ENTRY-POINTS.md.

use std::process::ExitCode;

const USAGE: &str = "usage: compiler <command> [args]\n       compiler --list";
const COMMANDS: &[(&str, &str)] = &[
    ("mode-sweep", "plan, score and report compiler-mode sweeps"),
    ("search-modes", "search compiler mode combinations"),
    ("shape-sweep", "bounded source-shape search, with the iterative descent driver"),
];

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    let Some(command) = args.first().map(String::as_str) else {
        eprintln!("{USAGE}"); list(); return ExitCode::from(2);
    };
    if matches!(command, "-h" | "--help" | "--list") {
        println!("{USAGE}\n\ncommands:"); list(); return ExitCode::SUCCESS;
    }
    let rest: Vec<String> = args[1..].to_vec();
    match command {
        "mode-sweep" => mode_sweep::cli::entry(&rest),
        "search-modes" => search_compiler_modes::cli::entry(&rest),
        "shape-sweep" => match shape_sweep::run(rest) {
            Ok(()) => ExitCode::SUCCESS,
            Err(message) => { eprintln!("error: {message}"); ExitCode::FAILURE }
        },
        other => {
            eprintln!("unknown compiler command: {other}\n\n{USAGE}"); list(); ExitCode::from(2)
        }
    }
}

fn list() { for (n, s) in COMMANDS { println!("  {n:<16} {s}"); } }
