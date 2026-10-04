//! `alchemy verify`: the gates, each an ordinary `make` target run
//! concurrently with the others. A passing gate prints one line, a failing
//! gate its whole output; every gate's output is kept in
//! `out/verify/<gate>.log`.

use std::io::{Read, Write};
use std::path::{Path, PathBuf};
use std::process::{Command, ExitCode, Stdio};
use std::time::Instant;

const USAGE: &str = "usage: alchemy verify [--pre-commit | --land | --pre-push]\n\
  (none)        every gate, all twelve editions built and compared, publishing nothing\n\
  --pre-commit  the staged checks every commit runs; they build and publish nothing\n\
  --land        on main, before committing a landing: every gate, then one count writes\n\
                README's progress line (staged) and the decomp.dev report\n\
  --pre-push    checks outgoing commits and the publication of an outgoing main tip";

/// The checks every commit runs on what it stages.
pub const STAGED: &[&str] = &[
    "language-check",
    "compiler-source-check",
    "publication-staged-check",
    "lint-staged",
];

/// `make verify` and `make land`: the checks over the whole tree, the tests
/// and all twelve editions built and compared, all at once.
pub const GATES: &[&str] = &[
    "language-check",
    "compiler-source-check",
    "publication-check",
    "lint-staged",
    "test",
    "compare-editions",
];

const PUBLICATION_FILES: &[&str] = &["README.md"];

pub fn entry(arguments: &[String]) -> ExitCode {
    let root = crate::compiler::routing::root();
    let result = match arguments {
        [] => run(root, GATES, Some("coverage-check")),
        [flag] if flag == "--pre-commit" => run(root, STAGED, None),
        [flag] if flag == "--land" => land(root),
        [flag] if flag == "--pre-push" => pre_push(root).map(|()| true),
        [flag] if flag == "-h" || flag == "--help" => {
            println!("{USAGE}");
            return ExitCode::SUCCESS;
        }
        _ => {
            eprintln!("{USAGE}");
            return ExitCode::from(2);
        }
    };
    match result {
        Ok(true) => ExitCode::SUCCESS,
        Ok(false) => ExitCode::FAILURE,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
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

/// Main's subject prefix as `make land` published it in README, in the
/// index (`revision` empty) or in a commit: each game's DONE or `pending`.
pub(crate) fn published_subject(root: &Path, revision: &str) -> Result<String, String> {
    let output = Command::new("git")
        .args(["show", &format!("{revision}:README.md")])
        .current_dir(root)
        .output()
        .map_err(|error| format!("cannot read README: {error}"))?;
    String::from_utf8_lossy(&output.stdout)
        .lines()
        .find_map(|line| {
            let inner = line.trim().strip_prefix("**☀️ ")?.strip_suffix("**")?;
            let (sun, anchor) = inner.split_once(" · ⚓️ ")?;
            Some(format!("☀️ {sun} ⚓️ {anchor} –"))
        })
        .ok_or_else(|| "README carries no progress line: run make land".into())
}

pub(crate) fn valid_subject(message: &str, expected: &str) -> bool {
    message
        .lines()
        .next()
        .unwrap_or("")
        .strip_prefix(expected)
        .is_some_and(|title| title.starts_with(' ') && !title.trim().is_empty())
}

fn outgoing_main(updates: &str) -> Result<Option<&str>, String> {
    let mut main = None;
    for line in updates.lines().filter(|line| !line.trim().is_empty()) {
        let fields = line.split_whitespace().collect::<Vec<_>>();
        if fields.len() != 4 {
            return Err(format!("invalid pre-push update: {line}"));
        }
        if fields[2] != "refs/heads/main" {
            continue;
        }
        if !matches!(fields[1].len(), 40 | 64)
            || !fields[1].bytes().all(|byte| byte.is_ascii_hexdigit())
        {
            return Err("outgoing main has an invalid object id".into());
        }
        if !fields[1].bytes().all(|byte| byte == b'0') && main.replace(fields[1]).is_some() {
            return Err("main appears twice in pre-push updates".into());
        }
    }
    Ok(main)
}

fn check_main_tip(root: &Path, executable: &Path, tip: &str) -> Result<(), String> {
    let repair = "create or amend a commit on main after make land";
    let unchanged = Command::new("git")
        .args(["diff", "--quiet", "--ignore-submodules=none", tip, "--"])
        .current_dir(root)
        .status()
        .map_err(|error| format!("cannot inspect outgoing main {tip}: {error}"))?;
    if !unchanged.success() {
        return Err(format!(
            "outgoing main {tip} is not the current verified tree; {repair}"
        ));
    }
    let expected = published_subject(root, tip).map_err(|error| format!("{error}; {repair}"))?;
    let message = Command::new("git")
        .args(["show", "-s", "--format=%B", tip])
        .current_dir(root)
        .output()
        .map_err(|error| format!("cannot read outgoing main subject: {error}"))?;
    if !message.status.success()
        || !valid_subject(&String::from_utf8_lossy(&message.stdout), &expected)
    {
        return Err(format!(
            "outgoing main {tip} lacks its verified {expected} prefix; {repair}"
        ));
    }
    let coverage = Command::new(executable)
        .args(["check", "coverage", "--check"])
        .current_dir(root)
        .status()
        .map_err(|error| format!("cannot check outgoing main publication: {error}"))?;
    if !coverage.success() {
        return Err(format!(
            "outgoing main {tip} publication is stale; {repair}"
        ));
    }
    Ok(())
}

fn pre_push(root: &Path) -> Result<(), String> {
    let mut updates = String::new();
    std::io::stdin()
        .read_to_string(&mut updates)
        .map_err(|error| error.to_string())?;
    let executable = std::env::current_exe().map_err(|error| error.to_string())?;
    let main = outgoing_main(&updates)?;
    if let Some(tip) = main {
        check_main_tip(root, &executable, tip)?;
    }
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
    if let Some(tip) = main {
        let remote = std::env::var("ALCHEMY_PUSH_URL").unwrap_or_default();
        crate::coverage::publish::upload(root, tip, &remote)?;
    }
    Ok(())
}

/// A landing on main, run before its commit: every gate, then the count
/// measured once into README's progress line, staged for the commit, and the
/// decomp.dev report.
fn land(root: &Path) -> Result<bool, String> {
    if !is_main(root)? {
        return Err("make land publishes main's progress: run it on main".into());
    }
    if !run(root, GATES, None)? {
        return Ok(false);
    }
    println!("{}", crate::coverage::publish_count(root)?);
    stage(root, PUBLICATION_FILES)?;
    crate::coverage::publish::prepare(root)?;
    Ok(true)
}

fn stage(root: &Path, paths: &[&str]) -> Result<(), String> {
    let output = Command::new("git")
        .args(["add", "--"])
        .args(paths)
        .current_dir(root)
        .output()
        .map_err(|error| format!("cannot stage publication: {error}"))?;
    if !output.status.success() {
        return Err(format!(
            "cannot stage publication: {}",
            String::from_utf8_lossy(&output.stderr).trim()
        ));
    }
    Ok(())
}

/// Every gate at once, then `last` once they all pass.
fn run(root: &Path, gates: &[&'static str], last: Option<&'static str>) -> Result<bool, String> {
    let executable = std::env::current_exe().map_err(|error| error.to_string())?;
    let logs = root.join("out/verify");
    std::fs::create_dir_all(&logs).map_err(|error| format!("{}: {error}", logs.display()))?;
    let started = Instant::now();
    let outcomes = std::thread::scope(|scope| {
        let handles = gates
            .iter()
            .map(|&gate| {
                let executable = &executable;
                let logs = &logs;
                scope.spawn(move || run_gate(root, executable, gate, logs))
            })
            .collect::<Vec<_>>();
        handles
            .into_iter()
            .map(|handle| handle.join().expect("gate thread"))
            .collect::<Result<Vec<_>, String>>()
    })?;
    let mut passed = report(&outcomes, root);
    if passed {
        if let Some(gate) = last {
            passed = report(&[run_gate(root, &executable, gate, &logs)?], root);
        }
    }
    println!(
        "verify {}: {:.1}s",
        if passed { "ok" } else { "failed" },
        started.elapsed().as_secs_f64()
    );
    Ok(passed)
}

struct Outcome {
    gate: &'static str,
    passed: bool,
    seconds: f64,
    log: PathBuf,
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

/// `make GATE` with this executable for every tool call; the edition builds
/// run side by side.
fn make_arguments(executable: &Path, gate: &str) -> Vec<String> {
    let mut arguments = vec![
        "--no-print-directory".to_string(),
        format!("ALCHEMY={}", executable.display()),
        "TARGET=tbs-ja".into(),
    ];
    if gate == "compare-editions" {
        arguments.push("-j12".into());
    }
    arguments.push(gate.into());
    arguments
}

fn run_gate(
    root: &Path,
    executable: &Path,
    gate: &'static str,
    logs: &Path,
) -> Result<Outcome, String> {
    let log = logs.join(format!("{gate}.log"));
    let file =
        std::fs::File::create(&log).map_err(|error| format!("{}: {error}", log.display()))?;
    let error_file = file.try_clone().map_err(|error| error.to_string())?;
    let started = Instant::now();
    let mut command = Command::new("make");
    if gate == "test" {
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
        .args(make_arguments(executable, gate))
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
        assert!(land(root).is_err());
    }

    /// Every gate is a Makefile target, and the count runs once, after them.
    #[test]
    fn every_gate_is_a_makefile_target() {
        let makefile =
            std::fs::read_to_string(crate::compiler::routing::root().join("Makefile")).unwrap();
        for gate in STAGED
            .iter()
            .chain(GATES)
            .chain(&["coverage", "coverage-check"])
        {
            assert!(
                makefile
                    .lines()
                    .any(|line| line.split(':').next().is_some_and(|targets| targets
                        .split_whitespace()
                        .any(|target| target == *gate))
                        && line.contains(':')
                        && !line.starts_with('\t')),
                "{gate} is not a Makefile target"
            );
        }
        assert!(!GATES.contains(&"coverage") && !GATES.contains(&"coverage-check"));
    }

    #[test]
    fn a_failing_gate_stops_before_the_count() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        std::fs::write(
            root.join("Makefile"),
            "pass:\n\t@echo pass >> gates.log\nfail:\n\t@false\ncount:\n\t@echo count >> gates.log\n",
        )
        .unwrap();
        assert!(!run(root, &["pass", "fail"], Some("count")).unwrap());
        assert_eq!(
            std::fs::read_to_string(root.join("gates.log")).unwrap(),
            "pass\n"
        );
        assert!(run(root, &["pass"], Some("count")).unwrap());
        assert_eq!(
            std::fs::read_to_string(root.join("gates.log")).unwrap(),
            "pass\npass\ncount\n"
        );
    }

    #[test]
    fn publication_stages_the_readme_without_staging_other_work() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        assert!(Command::new("git")
            .args(["init", "--quiet", "--initial-branch=main"])
            .arg(root)
            .status()
            .unwrap()
            .success());
        for path in PUBLICATION_FILES.iter().copied().chain(["pending.c"]) {
            let path = root.join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, "fixture\n").unwrap();
        }
        stage(root, PUBLICATION_FILES).unwrap();
        let staged = Command::new("git")
            .args(["diff", "--cached", "--name-only"])
            .current_dir(root)
            .output()
            .unwrap();
        let mut paths = String::from_utf8(staged.stdout)
            .unwrap()
            .lines()
            .map(str::to_owned)
            .collect::<Vec<_>>();
        paths.sort();
        let mut expected = PUBLICATION_FILES
            .iter()
            .map(|path| path.to_string())
            .collect::<Vec<_>>();
        expected.sort();
        assert_eq!(paths, expected);
    }
}
