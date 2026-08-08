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

use check_sanctum::{corpus_guard, parse_sealed, queue, stems, violations, QueueScan, SealedEntry};

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
    let good = "\n## Sealed\n\n- `08011568` floor=2hw axes=compiler,shape — scheduler tie the reference wins\n\n## Next\n";
    let set = |items: &[&str]| -> HashSet<String> {
        items.iter().map(|s| (*s).to_string()).collect()
    };
    let checks: Vec<(&str, bool)> = {
        let parsed: Vec<SealedEntry> = match parse_sealed(good) {
            Ok(entries) => entries,
            Err(error) => return fail(&error),
        };
        let beyond = parse_sealed(&format!("{good}\n- `08099999` some prose bullet\n"));
        let one_axis =
            parse_sealed("\n## Sealed\n\n- `08011568` floor=2hw axes=compiler — only flags tried\n");
        vec![
            (
                "parseSealed did not read a well-formed entry",
                parsed.len() == 1 && parsed[0].owner == "08011568" && parsed[0].floor == 2,
            ),
            (
                "parseSealed read beyond its own section",
                beyond.map(|e| e.len()) == Ok(1),
            ),
            (
                "a sealed owner that is now exact must be a violation",
                !violations(&parsed, &set(&["08011568"]), &set(&[])).is_empty(),
            ),
            (
                "a sealed owner with no semantic source must be a violation",
                !violations(&parsed, &set(&[]), &set(&[])).is_empty(),
            ),
            (
                "a well-formed sealed owner must pass",
                violations(&parsed, &set(&[]), &set(&["08011568"])).is_empty(),
            ),
            (
                "sealing on one axis must be refused",
                one_axis
                    .map(|e| !violations(&e, &set(&[]), &set(&["08011568"])).is_empty())
                    == Ok(true),
            ),
            (
                "a malformed entry must be an error",
                parse_sealed("\n## Sealed\n\n- `08011568` missing the rest\n").is_err(),
            ),
        ]
    };
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
    let ledger = root.join("SANCTUM.md");
    let text = match std::fs::read_to_string(&ledger) {
        Ok(text) => text,
        // PORT NOTE: the TS lets readFileSync throw; the message here is the
        // same failure with a readable face.
        Err(error) => return fail(&format!("cannot read {}: {error}", ledger.display())),
    };
    let entries = match parse_sealed(&text) {
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

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    if args.iter().any(|a| a == "--self-test") {
        return self_test();
    }
    if args.iter().any(|a| a == "--queue") {
        return run_queue();
    }
    gate()
}
