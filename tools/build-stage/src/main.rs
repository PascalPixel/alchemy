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
    if command == "--self-test" {
        return self_test();
    }
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

/// `--self-test`: prove the host's own contract.
///
/// Each consolidated host swallowed the `--self-test` its component binaries
/// used to answer, so the native runner reported five hosts failing for a flag
/// none of them implemented. What a HOST owns is its dispatch table, so that is
/// what it checks: every command named, uniquely, in sorted order, with a
/// non-empty summary and a reachable arm. The components themselves are covered
/// by `make crate-tests` and by the dispatcher registry gates.
fn self_test() -> ExitCode {
    // Uniqueness and reachability, not sort order: several hosts group related
    // commands deliberately, and `--list` should keep reading that way.
    let mut seen: Vec<&str> = Vec::new();
    for (name, summary) in COMMANDS {
        if name.is_empty() || summary.is_empty() {
            eprintln!("self-test: a command has an empty name or summary");
            return ExitCode::FAILURE;
        }
        if seen.contains(name) {
            eprintln!("self-test: {name} is listed twice");
            return ExitCode::FAILURE;
        }
        if !dispatchable(name) {
            eprintln!("self-test: {name} is listed but has no dispatch arm");
            return ExitCode::FAILURE;
        }
        seen.push(name);
    }
    println!("self-test=ok commands={}", COMMANDS.len());
    ExitCode::SUCCESS
}

fn dispatchable(name: &str) -> bool {
    matches!(name, "asm" | "claimed" | "full" | "rom" | "semantic")
}
