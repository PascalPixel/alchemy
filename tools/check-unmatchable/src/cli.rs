//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

//! check-unmatchable --- gate for the owner status registers.
//!
//!   check-unmatchable              # gate the registers
//!   check-unmatchable --queue      # owners with the compiler axis spent
//!   check-unmatchable --self-test
//!
//! Exit codes: 0 clean, 1 any violation, malformed entry, or a scan that looked
//! at nothing.

use std::collections::HashSet;
use std::path::Path;
use std::process::ExitCode;

use crate::{corpus_guard, provisional_violations, parse_provisional_file, parse_unmatchable_file,
    queue, read_recon, recon_violations, stems, violations, QueueScan};

const USAGE: &str = "Usage: check-unmatchable [--queue | --self-test]\n\nModes:\n  (default)      Validate the registers against current owners.\n  --queue        List owners with compiler search spent but shape search unrun.\n  --self-test    Run the gate's internal checks.\n  -h, --help     Show this help.";

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
    let entry = |axes: &[&str]| crate::UnmatchableEntry {
        owner: "08011568".to_string(),
        floor: 2,
        axes: axes.iter().map(|axis| (*axis).to_string()).collect(),
        reason: "scheduler tie".to_string(),
    };
    let unmatchable = vec![entry(&["compiler", "shape"])];
    let checks: Vec<(&str, bool)> = vec![
        (
            "the tracked unmatchable set must parse",
            parse_unmatchable_file(root()).is_ok(),
        ),
        (
            "a sealed owner that is now exact must be a violation",
            !violations(&unmatchable, &set(&["08011568"]), &set(&[])).is_empty(),
        ),
        (
            "a sealed owner with no semantic source must be a violation",
            !violations(&unmatchable, &set(&[]), &set(&[])).is_empty(),
        ),
        (
            "a well-formed sealed owner must pass",
            violations(&unmatchable, &set(&[]), &set(&["08011568"])).is_empty(),
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
    println!("check_unmatchable self-test ok ({} invariants)", checks.len());
    ExitCode::SUCCESS
}

/// Owner stems from `semantic/regions.json`: `<overlay>_c_<address>`.
fn audited_owner_stems(root: &std::path::Path) -> std::collections::HashSet<String> {
    let mut out = std::collections::HashSet::new();
    let Ok(text) = std::fs::read_to_string(root.join("semantic/regions.json")) else {
        return out;
    };
    let Ok(value) = serde_json::from_str::<serde_json::Value>(&text) else {
        return out;
    };
    for row in value
        .get("manual_regions")
        .and_then(|node| node.as_array())
        .unwrap_or(&Vec::new())
    {
        let (Some(overlay), Some(entry)) = (
            row.get("overlay").and_then(|n| n.as_str()),
            row.get("entry").and_then(|n| n.as_str()),
        ) else {
            continue;
        };
        if let Ok(address) = u32::from_str_radix(entry.trim_start_matches("0x"), 16) {
            out.insert(format!("{overlay}_c_{address:08x}"));
        }
    }
    out
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
    let entries = match parse_unmatchable_file(root) {
        Ok(entries) => entries,
        Err(error) => return fail(&error),
    };

    let exact = stems(root, "exact");
    // The audited owner boundaries, not a directory of C. This read the
    // `semantic/` source tree, so once that tier was deleted every entry failed
    // as "names no owner" and the register could not be used at all -- which
    // silently removed the only way to withdraw a blocked owner from the queue.
    let semantic = audited_owner_stems(root);
    // PORT NOTE: added, no TypeScript counterpart. Scanning nothing is not
    // passing; see `corpus_guard`.
    if let Some(message) = corpus_guard(root, &exact, &semantic) {
        return fail(&message);
    }

    let provisional = match parse_provisional_file(root) {
        Ok(entries) => entries,
        Err(error) => return fail(&error),
    };
    let mut problems = violations(&entries, &exact, &semantic);
    // The reconstruction records live or die by the same rule as the registers:
    // an owner that reached exact must not keep a record of how it did not.
    let recon = match read_recon(root) {
        Ok(list) => list,
        Err(message) => return fail(&message),
    };
    problems.extend(recon_violations(&recon, &exact));
    problems.extend(provisional_violations(&provisional, &exact));
    if !problems.is_empty() {
        for problem in &problems {
            eprintln!("error: {problem}");
        }
        return ExitCode::from(1);
    }
    println!(
        "owner registers ok: {} unmatchable owner(s), {} provisional source(s)",
        entries.len(),
        provisional.len()
    );
    eprintln!(
        "draft records: {}\nchecked against {} audited and {} exact owner(s)",
        recon.len(),
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
