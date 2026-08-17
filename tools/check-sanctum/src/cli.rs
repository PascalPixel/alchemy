//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

//! check-sanctum --- gate for SANCTUM.md, the sealed-owner ledger.
//!
//!   check-sanctum              # gate the ledger
//!   check-sanctum --queue      # owners with the compiler axis spent
//!   check-sanctum --self-test
//!
//! Exit codes: 0 clean, 1 any violation, malformed entry, or a scan that looked
//! at nothing.

use std::collections::HashSet;
use std::path::Path;
use std::process::ExitCode;

use crate::{corpus_guard, parse_sealed_file, queue, stems, violations, QueueScan};

const USAGE: &str = "Usage: check-sanctum [--queue | --self-test]\n\nModes:\n  (default)      Validate SANCTUM.md against current owners.\n  --queue        List owners with compiler search spent but shape search unrun.\n  --self-test    Run the ledger gate's internal checks.\n  -h, --help     Show this help.";

#[derive(Debug, PartialEq, Eq)]
enum Command {
    Help,
    Gate,
    Queue,
    SelfTest,
}

fn parse_args(args: &[String]) -> Result<Command, &'static str> {
    match args {
        [] => Ok(Command::Gate),
        [argument] if argument == "--queue" => Ok(Command::Queue),
        [argument] if argument == "--self-test" => Ok(Command::SelfTest),
        [argument] if argument == "-h" || argument == "--help" => Ok(Command::Help),
        _ => Err(USAGE),
    }
}

fn root() -> &'static Path {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap()
}

fn fail(message: &str) -> ExitCode {
    eprintln!("error: {message}");
    ExitCode::from(1)
}

/// The gate's own load-bearing invariants, kept runnable outside `cargo test` so
/// a deployed binary can prove itself. `cargo test` covers far more.
fn self_test() -> ExitCode {
    let set = |items: &[&str]| -> HashSet<String> {
        items.iter().map(|s| (*s).to_string()).collect()
    };
    let entry = |axes: &[&str]| crate::SealedEntry {
        owner: "08011568".to_string(),
        floor: 2,
        axes: axes.iter().map(|axis| (*axis).to_string()).collect(),
        reason: "scheduler tie".to_string(),
    };
    let sealed = vec![entry(&["compiler", "shape"])];
    let checks: Vec<(&str, bool)> = vec![
        (
            "the tracked sealed set must parse",
            parse_sealed_file(root()).is_ok(),
        ),
        (
            "a sealed owner that is now exact must be a violation",
            !violations(&sealed, &set(&["08011568"]), &set(&[])).is_empty(),
        ),
        (
            "a sealed owner with no semantic source must be a violation",
            !violations(&sealed, &set(&[]), &set(&[])).is_empty(),
        ),
        (
            "a well-formed sealed owner must pass",
            violations(&sealed, &set(&[]), &set(&["08011568"])).is_empty(),
        ),
        (
            "sealing on one axis must be refused",
            !violations(&vec![entry(&["compiler"])], &set(&[]), &set(&["08011568"])).is_empty(),
        ),
    ];
    for (message, ok) in &checks {

        if !ok {
            return fail(message);
        }
    }
    println!("check_sanctum self-test ok ({} invariants)", checks.len());
    ExitCode::SUCCESS
}

fn run_queue() -> ExitCode {
    match queue(root()) {
        // PORT NOTE: the TypeScript returns an empty list when out/modesweep is
        // missing and prints `queue=0`, which is indistinguishable from a real
        // empty queue. A scan that read no records is not a result.
        QueueScan::RootMissing(path) => fail(&format!(
            "{} does not exist; nothing was scanned, so the queue is unknown, not empty",
            path.display()
        )),
        QueueScan::RootEmpty(path) => fail(&format!(
            "{} holds no sweep directories; nothing was scanned",
            path.display()
        )),
        QueueScan::NoRecords(path) => fail(&format!(
            "no readable floor.json under {}; nothing was scanned",
            path.display()
        )),
        QueueScan::Scanned { records, rows } => {
            for row in &rows {
                println!("  {} floor={}hw", row.owner, row.floor);
            }
            println!(
                "queue={} owners with the compiler axis spent and the shape axis unrun",
                rows.len()
            );
            eprintln!("scanned {records} floor record(s)");
            ExitCode::SUCCESS
        }
    }
}

fn gate() -> ExitCode {
    let root = root();
    let entries = match parse_sealed_file(root) {
        Ok(entries) => entries,
        Err(error) => return fail(&error),
    };

    let exact = stems(root, "exact");
    let semantic = stems(root, "semantic");
    // PORT NOTE: added, no TypeScript counterpart. Scanning nothing is not
    // passing; see `corpus_guard`.
    if let Some(message) = corpus_guard(root, &exact, &semantic) {
        return fail(&message);
    }

    let problems = violations(&entries, &exact, &semantic);
    if !problems.is_empty() {
        for problem in &problems {
            eprintln!("error: {problem}");
        }
        return ExitCode::from(1);
    }
    println!("sanctum ok: {} sealed owner(s)", entries.len());
    eprintln!(
        "checked against {} semantic and {} exact owner(s)",
        semantic.len(),
        exact.len()
    );
    ExitCode::SUCCESS
}

pub fn entry(arguments: &[String]) -> std::process::ExitCode {
    let args: Vec<String> = arguments.to_vec();
    match parse_args(&args) {
        Ok(Command::Help) => {
            println!("{USAGE}");
            ExitCode::SUCCESS
        }
        Ok(Command::SelfTest) => self_test(),
        Ok(Command::Queue) => run_queue(),
        Ok(Command::Gate) => gate(),
        Err(message) => fail(message),
    }
}

#[cfg(test)]
mod parser_tests {
    use super::{parse_args, Command};

    fn args(items: &[&str]) -> Vec<String> {
        items.iter().map(|item| (*item).to_string()).collect()
    }

    #[test]
    fn preserves_default_and_named_modes() {
        assert_eq!(parse_args(&args(&[])), Ok(Command::Gate));
        assert_eq!(parse_args(&args(&["--queue"])), Ok(Command::Queue));
        assert_eq!(parse_args(&args(&["--self-test"])), Ok(Command::SelfTest));
        assert_eq!(parse_args(&args(&["-h"])), Ok(Command::Help));
    }

    #[test]
    fn rejects_unknown_or_combined_options() {
        assert!(parse_args(&args(&["--bogus"])).is_err());
        assert!(parse_args(&args(&["--queue", "--bogus"])).is_err());
    }
}
