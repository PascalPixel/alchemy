//! `alchemy verify`: the landing gate, run as waves of Makefile gates.
//!
//! Every gate is an ordinary `make` target, so each stays runnable alone.
//! Gates within a wave share no pending prerequisite and run concurrently;
//! each later invocation marks every finished gate old (`make -o`), so no
//! prerequisite (a ROM link, input preparation) runs twice. Every gate
//! calls this executable directly instead of `cargo run`. A passing gate
//! prints one line; a failing gate prints its whole output. Every gate's
//! output is kept in `out/verify/<gate>.log`.

use std::io::{Read, Write};
use std::path::{Path, PathBuf};
use std::process::{Command, ExitCode, Stdio};
use std::time::Instant;

/// The gates of `make verify`, in dependency waves.
pub const WAVES: &[&[&str]] = &[
    &[
        "toolchain-check",
        "native-format-check",
        "index-sync-check",
        "untracked-check",
        "publication-tree-check",
        "corpus-check",
        "language-check",
        "lint-production",
        "tooling-index-check",
        "prepare-inputs",
    ],
    &["compare", "compare-tla", "compare-other-editions"],
    &["coverage-report"],
];

const USAGE: &str = "usage: alchemy verify\n\
Runs every gate (make verify) in dependency waves, concurrently within a wave, one line per\n\
passing gate and the whole output of a failing one. Logs: out/verify/<gate>.log.\n\
       alchemy verify --land\n\
On main, before committing a landing (make land): the staged checks, the tests, all twelve editions\n\
built and compared, and the progress measurement and decomp.dev report prepared for the push.\n\
       alchemy verify --pre-commit\n\
The commit hooks' staged checks, on every branch; they build and publish nothing.\n\
       alchemy verify --pre-push\n\
Checks commits absent from remotes and each pushed tree. For a push of main to PascalPixel/alchemy,\n\
uploads the measurement make land prepared for its tip; CI draws the progress figures from it.";

const STAGED: &[&str] = &[
    "index-sync-check",
    "native-format-check",
    "language-check",
    "lint-staged",
    "tooling-index-check",
    "publication-staged-check",
];

pub fn entry(arguments: &[String]) -> ExitCode {
    if arguments == ["--help"] || arguments == ["-h"] {
        println!("{USAGE}");
        return ExitCode::SUCCESS;
    }
    if arguments == ["--pre-push"] {
        return match pre_push(crate::compiler::routing::root()) {
            Ok(()) => ExitCode::SUCCESS,
            Err(error) => {
                eprintln!("error: {error}");
                ExitCode::FAILURE
            }
        };
    }
    let mode = match arguments {
        [] => Mode::Verify,
        [flag] if flag == "--pre-commit" => Mode::Staged,
        [flag] if flag == "--land" => Mode::Land,
        _ => {
            eprintln!("{USAGE}");
            return ExitCode::from(2);
        }
    };
    match run(crate::compiler::routing::root(), mode) {
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

pub(crate) fn is_main(root: &Path) -> Result<bool, String> {
    let output = Command::new("git")
        .args(["symbolic-ref", "--quiet", "HEAD"])
        .current_dir(root)
        .output()
        .map_err(|error| format!("cannot read current branch: {error}"))?;
    if output.status.code() == Some(1) {
        return Ok(false);
    }
    if !output.status.success() {
        return Err(format!(
            "cannot read current branch: {}",
            String::from_utf8_lossy(&output.stderr).trim()
        ));
    }
    Ok(output.stdout == b"refs/heads/main\n")
}

fn outgoing_main(updates: &str) -> Result<Option<&str>, String> {
    let mut main = None;
    for line in updates.lines().filter(|line| !line.trim().is_empty()) {
        let fields = line.split_whitespace().collect::<Vec<_>>();
        if fields.len() != 4 {
            return Err(format!("invalid pre-push update: {line}"));
        }
        if fields[2] == "refs/heads/main"
            && (!matches!(fields[1].len(), 40 | 64)
                || !fields[1].bytes().all(|byte| byte.is_ascii_hexdigit()))
        {
            return Err("outgoing main has an invalid object id".into());
        }
        if fields[2] == "refs/heads/main" && !fields[1].bytes().all(|byte| byte == b'0') {
            if main.replace(fields[1]).is_some() {
                return Err("main appears twice in pre-push updates".into());
            }
        }
    }
    Ok(main)
}

fn pre_push(root: &Path) -> Result<(), String> {
    let mut updates = String::new();
    std::io::stdin()
        .read_to_string(&mut updates)
        .map_err(|error| error.to_string())?;
    let executable = std::env::current_exe().map_err(|error| error.to_string())?;
    let mut publication = Command::new(executable)
        .args(["check", "publication", "--pre-push"])
        .current_dir(root)
        .stdin(Stdio::piped())
        .spawn()
        .map_err(|error| format!("cannot inspect outgoing publication history: {error}"))?;
    publication
        .stdin
        .take()
        .ok_or("publication stdin is unavailable")?
        .write_all(updates.as_bytes())
        .map_err(|error| error.to_string())?;
    if !publication
        .wait()
        .map_err(|error| error.to_string())?
        .success()
    {
        return Err("outgoing publication history failed".into());
    }
    if let Some(tip) = outgoing_main(&updates)? {
        let remote = std::env::var("ALCHEMY_PUSH_URL").unwrap_or_default();
        crate::coverage::publish::upload(root, tip, &remote)?;
    }
    Ok(())
}

fn commit_waves(main: bool) -> Vec<&'static [&'static str]> {
    let mut waves = vec![STAGED];
    if main {
        waves.push(WAVES[0]);
        waves.push(&["test"]);
        waves.extend_from_slice(&WAVES[1..]);
    }
    waves
}

/// What a run of the gates is for.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
enum Mode {
    /// Every gate, publishing nothing.
    Verify,
    /// The commit hooks: staged checks only.
    Staged,
    /// A landing on main, run before its commit: every gate, the tests and
    /// the publication, staged for the commit.
    Land,
}

fn run(root: &Path, mode: Mode) -> Result<bool, String> {
    let executable = std::env::current_exe().map_err(|error| error.to_string())?;
    match mode {
        Mode::Verify => run_waves(root, &executable, false, true),
        Mode::Staged => run_waves(root, &executable, true, false),
        Mode::Land if !is_main(root)? => {
            Err("make land publishes main's progress: run it on main".into())
        }
        Mode::Land => {
            if !run_waves(root, &executable, true, true)? {
                return Ok(false);
            }
            crate::coverage::publish::prepare(root)?;
            Ok(true)
        }
    }
}

fn run_waves(root: &Path, executable: &Path, pre_commit: bool, main: bool) -> Result<bool, String> {
    let logs = root.join("out/verify");
    std::fs::create_dir_all(&logs).map_err(|error| format!("{}: {error}", logs.display()))?;
    let started = Instant::now();
    let mut finished: Vec<&str> = Vec::new();
    let waves = if pre_commit {
        commit_waves(main)
    } else {
        WAVES.to_vec()
    };
    for wave in waves {
        let wave = wave
            .iter()
            .copied()
            .filter(|gate| !finished.contains(gate))
            .collect::<Vec<_>>();
        let outcomes = std::thread::scope(|scope| {
            let handles = wave
                .iter()
                .map(|gate| {
                    let arguments =
                        make_arguments(&executable, &finished, gate, pre_commit && main);
                    let log = logs.join(format!("{gate}.log"));
                    scope.spawn(move || run_gate(root, gate, &arguments, log))
                })
                .collect::<Vec<_>>();
            handles
                .into_iter()
                .map(|handle| handle.join().expect("gate thread"))
                .collect::<Result<Vec<_>, String>>()
        })?;
        if !report(&outcomes, root) {
            println!(
                "verify failed after {:.1}s",
                started.elapsed().as_secs_f64()
            );
            return Ok(false);
        }
        finished.extend(wave);
    }
    if pre_commit && main {
        for gate in [
            "untracked-check",
            "index-sync-check",
            "publication-staged-check",
        ] {
            let arguments = make_arguments(&executable, &[], gate, true);
            let outcome = run_gate(root, gate, &arguments, logs.join(format!("{gate}.log")))?;
            if !report(&[outcome], root) {
                return Ok(false);
            }
        }
    }
    println!(
        "verify ok: {} gates in {:.1}s",
        finished.len(),
        started.elapsed().as_secs_f64()
    );
    Ok(true)
}

fn report(outcomes: &[Outcome], root: &Path) -> bool {
    let mut passed = true;
    for outcome in outcomes {
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
    passed
}

/// `make` for one gate: this executable for every tool call and every
/// finished gate marked old, so none of them (or their prerequisites) runs
/// again.
fn make_arguments(
    executable: &Path,
    finished: &[&str],
    gate: &str,
    main_commit: bool,
) -> Vec<String> {
    let mut arguments = vec![
        "--no-print-directory".to_string(),
        format!("ALCHEMY={}", executable.display()),
    ];
    if main_commit {
        arguments.push("TARGET=tbs-ja".into());
    }
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
    let mut command = Command::new("make");
    if isolated(gate) {
        // A commit hook's git variables name the repository being committed;
        // tests that make their own repositories must never inherit them.
        for (key, _) in std::env::vars_os() {
            if key.to_string_lossy().starts_with("GIT_") {
                command.env_remove(key);
            }
        }
    }
    let status = command
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

/// Gates that run outside the commit being made: the tool tests.
fn isolated(gate: &str) -> bool {
    matches!(gate, "test" | "tool-tests")
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn only_the_tests_leave_the_commits_repository() {
        assert!(isolated("test"));
        assert!(!isolated("index-sync-check"));
        assert!(!isolated("publication-staged-check"));
        assert!(!isolated("compare"));
    }
    use std::collections::{BTreeMap, BTreeSet};

    #[test]
    fn push_selection_uses_outgoing_destination_and_tip() {
        let tip = "0123456789012345678901234567890123456789";
        let zero = "0".repeat(40);
        let branch = format!("refs/heads/main {tip} refs/heads/wf/draft {zero}\n");
        assert_eq!(outgoing_main(&branch).unwrap(), None);
        let main = format!("refs/heads/wf/draft {tip} refs/heads/main {zero}\n");
        assert_eq!(
            outgoing_main(&format!("{branch}{main}")).unwrap(),
            Some(tip)
        );
        assert_eq!(
            outgoing_main(&format!("(delete) {zero} refs/heads/main {tip}\n")).unwrap(),
            None
        );
        assert!(outgoing_main("refs/heads/main --help refs/heads/main anything").is_err());
    }

    #[test]
    fn main_default_gates_override_an_ambient_tla_target() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        std::fs::write(root.join("Makefile"), "build-full coverage:\n\t@printf '%s\\n' '$(TARGET)'\ncompare-tla:\n\t@printf '%s\\n' tla-ja\n").unwrap();
        for gate in ["build-full", "coverage", "compare-tla"] {
            let arguments = make_arguments(Path::new("/unused/alchemy"), &[], gate, true);
            let output = Command::new("make")
                .args(arguments)
                .env("TARGET", "tla-en")
                .current_dir(root)
                .output()
                .unwrap();
            assert!(output.status.success());
            let expected = if gate == "compare-tla" {
                "tla-ja\n"
            } else {
                "tbs-ja\n"
            };
            assert_eq!(String::from_utf8(output.stdout).unwrap(), expected);
        }
    }

    #[test]
    fn branch_commits_stop_before_tests_builds_and_publication() {
        assert_eq!(commit_waves(false), [STAGED]);
        let main = commit_waves(true);
        let gates = main
            .iter()
            .flat_map(|wave| wave.iter())
            .copied()
            .collect::<BTreeSet<_>>();
        for gate in WAVES.iter().flat_map(|wave| wave.iter()) {
            assert!(gates.contains(gate));
        }
        assert!(gates.contains("test"));
        let builds = main
            .iter()
            .position(|wave| wave.contains(&"compare"))
            .unwrap();
        let publication = main
            .iter()
            .position(|wave| wave.contains(&"coverage-report"))
            .unwrap();
        assert!(builds < publication);
        assert!(main[builds].contains(&"compare-tla"));
        assert!(main[builds].contains(&"compare-other-editions"));
    }

    #[test]
    fn only_the_main_ref_selects_the_landing_gate() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        assert!(Command::new("git")
            .args(["init", "--quiet", "--initial-branch=main"])
            .arg(root)
            .status()
            .unwrap()
            .success());
        assert!(is_main(root).unwrap());
        assert!(Command::new("git")
            .args(["symbolic-ref", "HEAD", "refs/heads/wf/main-draft"])
            .current_dir(root)
            .status()
            .unwrap()
            .success());
        assert!(!is_main(root).unwrap());
        std::fs::write(
            root.join(".git/HEAD"),
            "0123456789012345678901234567890123456789\n",
        )
        .unwrap();
        assert!(!is_main(root).unwrap());
    }

    /// A repository whose every gate records its name and whose `failing`
    /// gate fails.
    fn landing_fixture(failing: &str) -> tempfile::TempDir {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        assert!(Command::new("git")
            .args(["init", "--quiet", "--initial-branch=wf/landing"])
            .arg(root)
            .status()
            .unwrap()
            .success());
        let mut makefile = String::new();
        let gates = STAGED
            .iter()
            .chain(WAVES.iter().flat_map(|wave| wave.iter()))
            .chain(["test"].iter())
            .copied()
            .collect::<BTreeSet<_>>();
        for gate in gates {
            makefile.push_str(&format!("{gate}:\n\t@echo {gate} >> gates.log\n"));
            if gate == failing {
                makefile.push_str("\t@false\n");
            }
        }
        std::fs::write(root.join("Makefile"), makefile).unwrap();
        std::fs::create_dir_all(root.join("recon/tbs/metrics")).unwrap();
        // Neither game has a build yet, so both are pending.
        std::fs::write(
            root.join("rom.sha1"),
            "5c4695205413df7db52b9a184815a07783999971  out/tbs-en/tbs-en.gba\n\
             b500663220cb9bf56b9f8e8c0c544f1d6fa3a824  out/tla-en/tla-en.gba\n",
        )
        .unwrap();
        directory
    }

    fn gates_run(root: &Path) -> Vec<String> {
        std::fs::read_to_string(root.join("gates.log"))
            .unwrap_or_default()
            .lines()
            .map(str::to_owned)
            .collect()
    }

    #[test]
    fn main_landing_builds_compares_then_measures_without_staging() {
        let directory = landing_fixture("none");
        let root = directory.path();
        assert!(run_waves(root, Path::new("/unused/alchemy"), true, true).unwrap());
        let ran = gates_run(root);
        let at = |gate: &str| ran.iter().position(|name| name == gate).unwrap();
        for gate in ["compare", "compare-tla"] {
            assert!(at("test") < at(gate) && at(gate) < at("coverage-report"));
        }
        assert_eq!(
            &ran[ran.len() - 2..],
            ["index-sync-check", "publication-staged-check"]
        );
        let staged = Command::new("git")
            .args(["diff", "--cached", "--name-only"])
            .current_dir(root)
            .output()
            .unwrap();
        assert!(staged.stdout.is_empty());
    }

    #[test]
    fn a_game_whose_build_differs_stops_the_landing_before_publication() {
        let directory = landing_fixture("compare-tla");
        let root = directory.path();
        assert!(!run_waves(root, Path::new("/unused/alchemy"), true, true).unwrap());
        let ran = gates_run(root);
        assert!(ran.iter().any(|gate| gate == "compare-tla"));
        assert!(!ran.iter().any(|gate| gate == "coverage-report"));
        assert!(!root.join("README.md").exists());
        // A branch commit runs only the staged checks.
        let directory = landing_fixture("compare-tla");
        let root = directory.path();
        assert!(run_waves(root, Path::new("/unused/alchemy"), true, false).unwrap());
        assert_eq!(gates_run(root).len(), STAGED.len());
    }

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
            &["prepare-inputs", "compare"],
            "coverage-report",
            false,
        );
        assert_eq!(
            arguments,
            [
                "--no-print-directory",
                "ALCHEMY=/repo/alchemy",
                "-o",
                "prepare-inputs",
                "-o",
                "compare",
                "coverage-report"
            ]
        );
    }
}
