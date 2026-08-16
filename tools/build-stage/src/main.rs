//! `build-stage`: one executable for every build stage.
//!
//! Eleventh consolidation, and the one on the ROM build path, so every stage
//! here is invoked by a Makefile target. See docs/ENTRY-POINTS.md.
//!
//! `build-assets` is deliberately absent: it is a binary-only crate with no
//! [lib] target, so there is nothing to link. Giving it one is a real edit on
//! the ROM build path and belongs in its own change.
//!
//! Named `build-stage` and not `build` because cargo forbids a binary target
//! called `build`; it collides with cargo's own build directory names.

use std::process::ExitCode;

const USAGE: &str = "usage: build-stage <stage> [args]\n       build-stage --list";
const COMMANDS: &[(&str, &str)] = &[
    ("asm", "rebuild the assembled stage"),
    ("claimed", "rebuild the claimed C stage"),
    ("full", "compose claimed C, retained asm and assets"),
    ("rom", "rebuild the ROM"),
    ("semantic", "compile the semantic sources"),
];

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    let Some(command) = args.first().map(String::as_str) else {
        eprintln!("{USAGE}"); list(); return ExitCode::from(2);
    };
    if matches!(command, "-h" | "--help" | "--list") {
        println!("{USAGE}\n\nstages:"); list(); return ExitCode::SUCCESS;
    }
    let rest: Vec<String> = args[1..].to_vec();
    match command {
        "asm" => { build_asm::cli::entry(&rest); ExitCode::SUCCESS }
        "claimed" => { build_claimed::cli::entry(&rest); ExitCode::SUCCESS }
        "full" => match build_full::cli::run(&rest) { 0 => ExitCode::SUCCESS, _ => ExitCode::FAILURE },
        "rom" => { build_rom::cli::entry(&rest); ExitCode::SUCCESS }
        "semantic" => { build_semantic::cli::entry(&rest); ExitCode::SUCCESS }
        other => { eprintln!("unknown build stage: {other}\n\n{USAGE}"); list(); ExitCode::from(2) }
    }
}

fn list() { for (n, s) in COMMANDS { println!("  {n:<10} {s}"); } }
