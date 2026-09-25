//! `alchemy verify`: the landing gate, run as waves of Makefile gates.
//!
//! Every gate is an ordinary `make` target, so each stays runnable alone.
//! Gates within a wave share no pending prerequisite and run concurrently;
//! each later invocation marks every finished gate old (`make -o`), so no
//! prerequisite (the full build, input preparation) runs twice. Every gate
//! calls this executable directly instead of `cargo run`. A passing gate
//! prints one line; a failing gate prints its whole output. Every gate's
//! output is kept in `out/verify/<gate>.log`.

use std::path::{Path, PathBuf};
use std::process::{Command, ExitCode, Stdio};
use std::time::Instant;

/// The gates of `make verify`, in dependency waves.
pub const WAVES: &[&[&str]] = &[
    &[
        "toolchain-check",
        "native-format-check",
        "index-sync-check",
        "publication-tree-check",
        "corpus-check",
        "language-check",
        "lint-production",
        "tooling-index-check",
        "prepare-inputs",
    ],
    &["source-tracking-check", "build-full", "compare-tla"],
    &[
        "full-rom-check",
        "overlay-check",
        "check-owners",
        "coverage-check",
    ],
    &[
        "declared-tu-check",
        "owner-inventory-check",
        "siblings-check",
    ],
    &["strict-tu-check"],
];

const USAGE: &str = "usage: alchemy verify\n\
Runs the landing gate (make verify): every gate in dependency waves, concurrently within a wave,\n\
one line per passing gate and the whole output of a failing one. Logs: out/verify/<gate>.log.";

pub fn entry(arguments: &[String]) -> ExitCode {
    if arguments == ["--help"] || arguments == ["-h"] {
        println!("{USAGE}");
        return ExitCode::SUCCESS;
    }
    if !arguments.is_empty() {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    }
    match run(crate::compiler::routing::root()) {
        Ok(true) => ExitCode::SUCCESS,
        Ok(false) => ExitCode::FAILURE,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}

struct Outcome {
    gate: &'static str,
    passed: bool,
    seconds: f64,
    log: PathBuf,
}

fn run(root: &Path) -> Result<bool, String> {
    let executable = std::env::current_exe().map_err(|error| error.to_string())?;
    let logs = root.join("out/verify");
    std::fs::create_dir_all(&logs).map_err(|error| format!("{}: {error}", logs.display()))?;
    let started = Instant::now();
    let mut finished: Vec<&str> = Vec::new();
    for wave in WAVES {
        let outcomes = std::thread::scope(|scope| {
            let handles = wave
                .iter()
                .map(|gate| {
                    let arguments = make_arguments(&executable, &finished, gate);
                    let log = logs.join(format!("{gate}.log"));
                    scope.spawn(move || run_gate(root, gate, &arguments, log))
                })
                .collect::<Vec<_>>();
            handles
                .into_iter()
                .map(|handle| handle.join().expect("gate thread"))
                .collect::<Result<Vec<_>, String>>()
        })?;
        let mut passed = true;
        for outcome in &outcomes {
            if outcome.passed {
                println!("ok   {:<24} {:>6.1}s", outcome.gate, outcome.seconds);
            } else {
                passed = false;
                let text = std::fs::read_to_string(&outcome.log).unwrap_or_default();
                println!(
                    "FAIL {:<24} {:>6.1}s  ({})\n{text}",
                    outcome.gate,
                    outcome.seconds,
                    outcome
                        .log
                        .strip_prefix(root)
                        .unwrap_or(&outcome.log)
                        .display()
                );
            }
        }
        if !passed {
            println!(
                "verify failed after {:.1}s",
                started.elapsed().as_secs_f64()
            );
            return Ok(false);
        }
        finished.extend(wave.iter().copied());
    }
    println!(
        "verify ok: {} gates in {:.1}s",
        finished.len(),
        started.elapsed().as_secs_f64()
    );
    Ok(true)
}

/// `make` for one gate: this executable for every tool call and every
/// finished gate marked old, so none of them (or their prerequisites) runs
/// again.
fn make_arguments(executable: &Path, finished: &[&str], gate: &str) -> Vec<String> {
    let mut arguments = vec![
        "--no-print-directory".to_string(),
        format!("ALCHEMY={}", executable.display()),
    ];
    for done in finished {
        arguments.push("-o".into());
        arguments.push((*done).into());
    }
    arguments.push(gate.into());
    arguments
}

fn run_gate(
    root: &Path,
    gate: &'static str,
    arguments: &[String],
    log: PathBuf,
) -> Result<Outcome, String> {
    let file =
        std::fs::File::create(&log).map_err(|error| format!("{}: {error}", log.display()))?;
    let error_file = file.try_clone().map_err(|error| error.to_string())?;
    let started = Instant::now();
    let status = Command::new("make")
        .current_dir(root)
        .args(arguments)
        .stdin(Stdio::null())
        .stdout(file)
        .stderr(error_file)
        .status()
        .map_err(|error| format!("cannot run make {gate}: {error}"))?;
    Ok(Outcome {
        gate,
        passed: status.success(),
        seconds: started.elapsed().as_secs_f64(),
        log,
    })
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::collections::{BTreeMap, BTreeSet};

    /// Each Makefile rule's prerequisites, from `target: prerequisites`
    /// lines (order-only prerequisites included).
    fn rules(makefile: &str) -> BTreeMap<String, Vec<String>> {
        let mut rules = BTreeMap::new();
        let mut lines = makefile.lines().peekable();
        while let Some(line) = lines.next() {
            if line.starts_with(['\t', '#', '.', ' ']) || line.contains(":=") || line.contains("?=")
            {
                continue;
            }
            let Some((targets, prerequisites)) = line.split_once(':') else {
                continue;
            };
            let mut prerequisites = prerequisites.to_string();
            while prerequisites.ends_with('\\') {
                prerequisites.pop();
                prerequisites.push(' ');
                prerequisites.push_str(lines.next().unwrap_or_default());
            }
            for target in targets.split_whitespace() {
                rules.insert(
                    target.to_string(),
                    prerequisites
                        .split_whitespace()
                        .filter(|name| *name != "|")
                        .map(str::to_owned)
                        .collect(),
                );
            }
        }
        rules
    }

    /// What `make -o FINISHED... gate` runs besides the gate: its
    /// prerequisites, never descending past a finished gate.
    fn reachable(
        rules: &BTreeMap<String, Vec<String>>,
        finished: &BTreeSet<String>,
        gate: &str,
    ) -> BTreeSet<String> {
        let mut seen = BTreeSet::new();
        let mut pending = vec![gate.to_string()];
        while let Some(name) = pending.pop() {
            for prerequisite in rules.get(&name).into_iter().flatten() {
                if !finished.contains(prerequisite) && seen.insert(prerequisite.clone()) {
                    pending.push(prerequisite.clone());
                }
            }
        }
        seen
    }

    #[test]
    fn waves_keep_every_gate_and_run_each_prerequisite_once() {
        let makefile =
            std::fs::read_to_string(crate::compiler::routing::root().join("Makefile")).unwrap();
        let rules = rules(&makefile);
        let gates = WAVES
            .iter()
            .flat_map(|wave| wave.iter())
            .collect::<Vec<_>>();
        assert_eq!(
            gates.iter().collect::<BTreeSet<_>>().len(),
            gates.len(),
            "a gate appears twice"
        );
        let (mut finished, mut ran) = (BTreeSet::new(), BTreeSet::new());
        for wave in WAVES {
            let mut claimed = BTreeMap::new();
            for gate in *wave {
                assert!(rules.contains_key(*gate), "{gate} is not a Makefile target");
                for prerequisite in reachable(&rules, &finished, gate) {
                    assert!(
                        !ran.contains(&prerequisite),
                        "{gate} runs {prerequisite} again"
                    );
                    assert!(
                        !wave.contains(&prerequisite.as_str()),
                        "{gate} needs {prerequisite} from its own wave"
                    );
                    if let Some(other) = claimed.insert(prerequisite.clone(), *gate) {
                        panic!("{other} and {gate} would both run {prerequisite}");
                    }
                }
            }
            ran.extend(claimed.into_keys());
            finished.extend(wave.iter().map(|gate| gate.to_string()));
        }
        // The `verify` rule itself runs nothing but this executable.
        assert!(rules["verify"].is_empty(), "{:?}", rules["verify"]);
    }

    #[test]
    fn later_gates_mark_finished_gates_old_and_call_this_executable() {
        let arguments = make_arguments(
            Path::new("/repo/alchemy"),
            &["prepare-inputs", "build-full"],
            "full-rom-check",
        );
        assert_eq!(
            arguments,
            [
                "--no-print-directory",
                "ALCHEMY=/repo/alchemy",
                "-o",
                "prepare-inputs",
                "-o",
                "build-full",
                "full-rom-check"
            ]
        );
    }
}
