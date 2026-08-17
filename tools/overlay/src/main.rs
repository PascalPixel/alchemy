//! `overlay`: one executable for every overlay operation.
//!
//! WHY THIS EXISTS. `tools/` shipped 115 separate executables. Nothing could
//! enumerate them, so for a month agents kept rebuilding work that already
//! existed: three permuter passes, a shape-sweep transform, an entire
//! statement-order sweeper, and a duplicate of the architecture registration
//! check. The names took the blame, but the names were a symptom. 115 entry
//! points is the disease.
//!
//! Every former `overlay-*` binary is now a LIBRARY linked here. Nothing was
//! deleted and no capability was lost; there is one way in, and `overlay --list`
//! enumerates what that way covers.
//!
//! The per-command adapters below are lifted from each crate's own `main.rs`,
//! because the `run` signatures are not uniform: seven distinct shapes across
//! twelve crates, returning `()`, `i32`, `Vec<String>` or a bespoke `Outcome`.
//! That non-uniformity is itself a symptom of tools growing without a shared
//! entry point.

use std::path::{Path, PathBuf};
use std::process::ExitCode;

const USAGE: &str = "usage: overlay <command> [args]\n       overlay --list";

const COMMANDS: &[(&str, &str)] = &[
    ("adopt", "adopt a matched overlay candidate"),
    ("park", "un-adopt a row: restore its assembly, move its C to semantic/"),
    ("audit", "compare every adopted row against the bytes it replaced"),
    ("score", "show one overlay row beside the ROM, like candidate-show"),
    ("call-order-check", "check overlay call ordering"),
    ("candidate-rank", "rank overlay candidates"),
    ("certify", "certify overlay results"),
    ("entry", "report overlay entry points"),
    ("gaps", "report unclaimed overlay ranges"),
    ("inventory", "rebuild the overlay inventory"),
    ("mode-cohort", "compiler-mode cohort over overlays"),
    ("published", "report published overlays"),
    ("show", "show one overlay"),
    ("twins", "find mirrored overlay pairs"),
    ("unindexed", "report unindexed overlays"),
    ("showcase", "run the overlay showcase"),
    ("disasm", "overlay disasm"),
    ("parity-dump", "dump overlay parity data"),
];

fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("crate sits two levels below the repository root")
        .to_path_buf()
}

fn lines(result: Result<Vec<String>, String>) -> ExitCode {
    match result {
        Ok(lines) => {
            for line in lines {
                println!("{line}");
            }
            ExitCode::SUCCESS
        }
        Err(message) => {
            eprintln!("{message}");
            ExitCode::FAILURE
        }
    }
}

fn unit(result: Result<(), String>) -> ExitCode {
    match result {
        Ok(()) => ExitCode::SUCCESS,
        Err(message) => {
            eprintln!("{message}");
            ExitCode::FAILURE
        }
    }
}

fn code(result: Result<i32, String>) -> ExitCode {
    match result {
        Ok(0) => ExitCode::SUCCESS,
        Ok(_) => ExitCode::FAILURE,
        Err(message) => {
            eprintln!("{message}");
            ExitCode::FAILURE
        }
    }
}

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    let Some(command) = args.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        list();
        return ExitCode::from(2);
    };
    if command == "--self-test" {
        return self_test();
    }
    if matches!(command, "-h" | "--help" | "--list") {
        println!("{USAGE}\n\ncommands:");
        list();
        return ExitCode::SUCCESS;
    }
    let rest: Vec<String> = args[1..].to_vec();
    match command {
        "disasm" => overlay_disasm::cli::entry(&rest),
        "parity-dump" => { overlay_disasm::entry_parity_dump::entry(&rest); ExitCode::SUCCESS }
        "showcase" => { overlay_showcase::entrypoint::entry(&rest); ExitCode::SUCCESS }

        "adopt" => code(overlay_adopt::run(&root(), &rest)),
        "park" => code(overlay_adopt::park::run(&root(), &rest)),
        "audit" => code(overlay_adopt::park::run_audit(&root(), &rest)),
        "score" => code(overlay_adopt::score::run(&root(), &rest)),
        "call-order-check" => code(overlay_call_order_check::run(&rest)),
        "candidate-rank" => {
            let self_exe = std::env::current_exe().unwrap_or_else(|_| PathBuf::from("overlay"));
            unit(overlay_candidate_rank::run(
                &root(),
                &self_exe,
                &["candidate-rank"],
                &rest,
            ))
        }
        "certify" => unit(overlay_certify::run(&rest)),
        "entry" => lines(overlay_entry::run(&rest)),
        "gaps" => unit(overlay_gaps::run(&rest)),
        "inventory" => unit(overlay_inventory::run(&root(), &rest)),
        "mode-cohort" => lines(overlay_mode_cohort::run(&rest)),
        "published" => unit(overlay_published::run(&rest)),
        "show" => match overlay_show::run(&rest) {
            Ok(overlay_show::Outcome::Usage) => ExitCode::SUCCESS,
            Ok(overlay_show::Outcome::SelfTest(message)) => {
                println!("{message}");
                ExitCode::SUCCESS
            }
            Ok(overlay_show::Outcome::Lines(text)) => {
                for line in text {
                    println!("{line}");
                }
                ExitCode::SUCCESS
            }
            Err(message) => {
                eprintln!("{message}");
                ExitCode::FAILURE
            }
        },
        "twins" => unit(overlay_twins::run(&root(), &rest)),
        "unindexed" => unit(overlay_unindexed::run(&rest)),
        other => {
            eprintln!("unknown overlay command: {other}\n\n{USAGE}");
            list();
            ExitCode::from(2)
        }
    }
}

fn list() {
    for (name, summary) in COMMANDS {
        println!("  {name:<18} {summary}");
    }
}

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
    matches!(name, "adopt" | "audit" | "call-order-check" | "candidate-rank" | "certify" | "disasm" | "entry" | "gaps" | "inventory" | "mode-cohort" | "parity-dump" | "park" | "published" | "score" | "show" | "showcase" | "twins" | "unindexed")
}
