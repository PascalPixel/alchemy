use std::io::{Read, Write};
use std::path::{Path, PathBuf};
use std::process::{Command, ExitCode, Stdio};
use std::time::Instant;

const USAGE: &str = "usage: alchemy verify [--pre-commit | --land | --pre-push]\n\
  (none)        every gate, all twelve editions built and compared, publishing nothing\n\
  --pre-commit  the staged checks every commit runs; they build and publish nothing\n\
  --land        on main, before committing a landing: every gate, then one count writes\n\
                the measurement and decomp.dev report the push uploads for CI\n\
  --pre-push    checks outgoing commits; a push of main uploads its tip's measurement";

pub const STAGED: &[&str] = &[
    "language-check",
    "compiler-source-check",
    "publication-staged-check",
    "lint-staged",
];

pub const GATES: &[&str] = &[
    "language-check",
    "compiler-source-check",
    "publication-check",
    "lint-staged",
    "test",
    "compare-editions",
    "modern-check",
];

pub fn entry(arguments: &[String]) -> ExitCode {
    let root = crate::compiler::routing::root();
    let result = match arguments {
        [] => run(root, GATES, None),
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

fn pre_push(root: &Path) -> Result<(), String> {
    let mut updates = String::new();
    std::io::stdin()
        .read_to_string(&mut updates)
        .map_err(|error| error.to_string())?;
    let executable = std::env::current_exe().map_err(|error| error.to_string())?;
    let main = outgoing_main(&updates)?;
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

fn land(root: &Path) -> Result<bool, String> {
    if !is_main(root)? {
        return Err("make land: run it on main".into());
    }
    if !run(root, GATES, None)? {
        return Ok(false);
    }
    println!("{}", crate::coverage::report(root)?);
    crate::coverage::publish::prepare(root)?;
    Ok(true)
}

fn run(root: &Path, gates: &[&'static str], last: Option<&'static str>) -> Result<bool, String> {
    let executable = std::env::current_exe().map_err(|error| error.to_string())?;
    let logs = root.join("out/verify");
    std::fs::create_dir_all(&logs).map_err(|error| format!("{}: {error}", logs.display()))?;
    // Cargo tests and Clippy can replace/unlink the release executable while
    // comparison launches twelve children. Keep this verified invocation's
    // executable alive under a private path for the entire gate run.
    let pinned = pin_executable(&executable, &logs)?;
    let executable = pinned.path().join("alchemy");
    let started = Instant::now();
    let outcomes = schedule(gates, &|gate| run_gate(root, &executable, gate, &logs))?;
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

fn pin_executable(executable: &Path, logs: &Path) -> Result<tempfile::TempDir, String> {
    let directory = tempfile::Builder::new()
        .prefix("executable-")
        .tempdir_in(logs)
        .map_err(|error| format!("{}: {error}", logs.display()))?;
    std::fs::copy(executable, directory.path().join("alchemy"))
        .map_err(|error| format!("cannot pin {}: {error}", executable.display()))?;
    Ok(directory)
}

/// Publication includes alias checks that read every linked ELF. Comparison
/// replaces those files and removes intermediate symbol images, so join its
/// workers before inspecting output. A failed comparison still runs every
/// gate; it remains a failure in the original reporting order.
fn schedule<T: Send>(
    gates: &[&'static str],
    task: &(impl Fn(&'static str) -> Result<T, String> + Sync),
) -> Result<Vec<T>, String> {
    let publication = gates
        .contains(&"compare-editions")
        .then(|| gates.iter().position(|gate| *gate == "publication-check"))
        .flatten();
    let mut outcomes = std::thread::scope(|scope| {
        let handles = gates
            .iter()
            .enumerate()
            .filter(|(index, _)| Some(*index) != publication)
            .map(|(index, &gate)| {
                scope.spawn(move || {
                    let mut results = vec![(index, task(gate))];
                    // Only comparison writes the files publication reads. Tests
                    // and lint can continue while publication inspects them.
                    if gate == "compare-editions" {
                        if let Some(index) = publication {
                            results.push((index, task("publication-check")));
                        }
                    }
                    results
                })
            })
            .collect::<Vec<_>>();
        handles
            .into_iter()
            .flat_map(|handle| handle.join().expect("gate thread"))
            .collect::<Vec<_>>()
    });
    outcomes.sort_unstable_by_key(|(index, _)| *index);
    outcomes.into_iter().map(|(_, result)| result).collect()
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

fn make_arguments(executable: &Path, gate: &str) -> Vec<String> {
    let mut arguments = vec![
        "--no-print-directory".to_string(),
        format!("ALCHEMY={}", executable.display()),
        format!("ALCHEMY_BIN={}", executable.display()),
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
        for (key, _) in std::env::vars_os() {
            if key.to_string_lossy().starts_with("GIT_") {
                command.env_remove(key);
            }
        }
    }
    // A gate proves its builds by content: no build under it may stop early
    // because its files merely look unchanged (settled.rs).
    command.env("ALCHEMY_REVERIFY", "1");
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
    fn gate_executable_survives_cargo_replacing_the_release_binary() {
        let directory = tempfile::tempdir().unwrap();
        let original = directory.path().join("release-alchemy");
        std::fs::write(&original, "verified executable").unwrap();
        let pinned = pin_executable(&original, directory.path()).unwrap();
        let executable = pinned.path().join("alchemy");
        std::fs::remove_file(&original).unwrap();
        std::fs::write(&original, "replacement executable").unwrap();
        assert_eq!(
            std::fs::read_to_string(&executable).unwrap(),
            "verified executable"
        );
        let arguments = make_arguments(&executable, "compare-editions");
        assert!(arguments.contains(&format!("ALCHEMY={}", executable.display())));
        assert!(arguments.contains(&format!("ALCHEMY_BIN={}", executable.display())));
        assert!(arguments.contains(&"-j12".into()));
        drop(pinned);
        assert!(!executable.exists());
    }

    #[test]
    fn output_readers_wait_for_comparison_even_when_it_fails() {
        use std::sync::atomic::{AtomicBool, Ordering};
        use std::sync::{mpsc, Condvar, Mutex};
        use std::time::Duration;

        for comparison in [Ok(true), Ok(false), Err("comparison launch failed".into())] {
            let (writer_started, writer_start) = mpsc::channel();
            let (independent_started, independent_start) = mpsc::channel();
            let (reader_started, reader_start) = mpsc::channel();
            let state = (Mutex::new((false, false)), Condvar::new());
            let finished = AtomicBool::new(false);
            std::thread::scope(|scope| {
                let worker = scope.spawn(|| {
                    schedule(
                        &["publication-check", "test", "compare-editions"],
                        &|gate| match gate {
                            "compare-editions" => {
                                let mut state_lock = state.0.lock().unwrap();
                                state_lock.0 = true;
                                state.1.notify_all();
                                writer_started.send(()).unwrap();
                                while !state_lock.1 {
                                    state_lock = state.1.wait(state_lock).unwrap();
                                }
                                finished.store(true, Ordering::Release);
                                comparison.clone().map(|passed| (gate, passed))
                            }
                            "test" => {
                                let (state_lock, _) = state
                                    .1
                                    .wait_timeout_while(
                                        state.0.lock().unwrap(),
                                        Duration::from_secs(2),
                                        |state| !state.0,
                                    )
                                    .unwrap();
                                if !state_lock.0 {
                                    return Err("independent gate was serialized".into());
                                }
                                independent_started.send(()).unwrap();
                                Ok((gate, true))
                            }
                            "publication-check" => {
                                reader_started.send(()).unwrap();
                                if !finished.load(Ordering::Acquire) {
                                    return Err("reader saw comparison's unfinished output".into());
                                }
                                Ok((gate, true))
                            }
                            _ => unreachable!(),
                        },
                    )
                });
                let writer_ready = writer_start.recv_timeout(Duration::from_secs(2));
                let independent_ready = independent_start.recv_timeout(Duration::from_secs(2));
                // This bounded observation supplements the reader callback's
                // completion check while comparison is explicitly blocked.
                let premature = reader_start
                    .recv_timeout(Duration::from_millis(100))
                    .is_ok();
                // Always release before any assertion can unwind the scope,
                // including a startup timeout or serialization regression.
                state.0.lock().unwrap().1 = true;
                state.1.notify_all();
                let result = worker.join().unwrap();
                writer_ready.unwrap();
                independent_ready.unwrap();
                assert!(!premature, "publication began while comparison was writing");
                reader_start.recv_timeout(Duration::from_secs(2)).unwrap();
                match &comparison {
                    Ok(passed) => assert_eq!(
                        result.unwrap(),
                        vec![
                            ("publication-check", true),
                            ("test", true),
                            ("compare-editions", *passed)
                        ]
                    ),
                    Err(error) => assert_eq!(result.unwrap_err(), *error),
                }
            });
        }
    }

    #[test]
    fn publication_does_not_wait_for_independent_gates() {
        use std::sync::{mpsc, Condvar, Mutex};
        use std::time::Duration;

        let (reader_finished, reader) = mpsc::channel();
        let release = (Mutex::new(false), Condvar::new());
        std::thread::scope(|scope| {
            let worker = scope.spawn(|| {
                schedule(
                    &["test", "publication-check", "compare-editions"],
                    &|gate| {
                        if gate == "test" {
                            let mut released = release.0.lock().unwrap();
                            while !*released {
                                released = release.1.wait(released).unwrap();
                            }
                        } else if gate == "publication-check" {
                            reader_finished.send(()).unwrap();
                        }
                        Ok(gate)
                    },
                )
            });
            let publication = reader.recv_timeout(Duration::from_secs(2));
            // Release the worker even if a scheduling regression timed out.
            *release.0.lock().unwrap() = true;
            release.1.notify_all();
            let outcomes = worker.join().unwrap().unwrap();
            publication.expect("publication waited for the blocked test gate");
            assert_eq!(outcomes, ["test", "publication-check", "compare-editions"]);
        });
    }

    #[test]
    fn staged_gates_still_dispatch_together_without_comparison() {
        use std::sync::{Condvar, Mutex};
        use std::time::Duration;

        let state = (Mutex::new(0), Condvar::new());
        let outcomes = schedule(STAGED, &|gate| {
            let mut count = state.0.lock().unwrap();
            *count += 1;
            state.1.notify_all();
            let (count, _) = state
                .1
                .wait_timeout_while(count, Duration::from_secs(2), |count| *count < STAGED.len())
                .unwrap();
            if *count != STAGED.len() {
                return Err("staged gates were deferred or another gate was added".into());
            }
            Ok(gate)
        })
        .unwrap();
        assert_eq!(outcomes, STAGED);
    }

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

    #[test]
    fn every_gate_is_a_makefile_target() {
        let makefile =
            std::fs::read_to_string(crate::compiler::routing::root().join("Makefile")).unwrap();
        for gate in STAGED.iter().chain(GATES).chain(&["coverage-report"]) {
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
        assert!(!GATES.contains(&"coverage-report"));
    }

    #[test]
    fn make_builds_tools_once_and_keeps_the_verified_executable_in_children() {
        let root = crate::compiler::routing::root();
        let dry_run = |args: &[&str]| {
            let output = Command::new("make")
                .args(["--no-print-directory", "--dry-run"])
                .args(args)
                .env_remove("ALCHEMY")
                .env_remove("ALCHEMY_BIN")
                .env_remove("MAKEFLAGS")
                .env_remove("MFLAGS")
                .current_dir(root)
                .output()
                .unwrap();
            assert!(
                output.status.success(),
                "{}",
                String::from_utf8_lossy(&output.stderr)
            );
            String::from_utf8(output.stdout).unwrap()
        };
        let ordinary = dry_run(&["compare-tbs-en", "progress"]);
        assert_eq!(ordinary.matches("cargo build").count(), 1);
        assert!(!ordinary.contains("cargo run"));
        let verified = dry_run(&[
            "ALCHEMY=/private/tmp/verified-alchemy",
            "compare-editions",
            "publication-check",
        ]);
        assert!(!verified.contains("cargo build") && !verified.contains("cargo run"));
        assert_eq!(
            verified
                .matches("/private/tmp/verified-alchemy build rom --target ")
                .count(),
            12
        );
        assert!(verified.contains("/private/tmp/verified-alchemy check publication --tree"));
    }

    #[test]
    fn publication_runs_every_check_and_propagates_a_child_failure() {
        let root = crate::compiler::routing::root();
        let makefile = std::fs::read_to_string(root.join("Makefile")).unwrap();
        let directory = tempfile::tempdir().unwrap();
        let run = |makefile: &str| {
            std::fs::write(directory.path().join("Makefile"), makefile).unwrap();
            Command::new("make")
                .args([
                    "--no-print-directory",
                    "ALCHEMY=unused-verified-executable",
                    "CHECK=echo",
                    "publication-check",
                ])
                .env_remove("MAKEFLAGS")
                .env_remove("MFLAGS")
                .current_dir(directory.path())
                .output()
                .unwrap()
        };
        let expected = [
            "publication --tree",
            "layout",
            "dead",
            "aliases",
            "devices",
            "headers",
            "hygiene",
            "object-list",
            "packer",
        ];
        let passed = run(&makefile);
        assert!(
            passed.status.success(),
            "{}",
            String::from_utf8_lossy(&passed.stderr)
        );
        let output = String::from_utf8(passed.stdout).unwrap();
        for check in expected {
            assert_eq!(
                output.lines().filter(|line| *line == check).count(),
                1,
                "{output}"
            );
        }

        // Fail the first child: keep-going must still execute the others,
        // and the parent must report failure rather than publish a count.
        let failure = makefile.replace("\t$(CHECK) publication --tree\n", "\t@false\n");
        assert_ne!(failure, makefile);
        let failed = run(&failure);
        assert!(!failed.status.success());
        let output = String::from_utf8(failed.stdout).unwrap();
        for check in &expected[1..] {
            assert_eq!(
                output.lines().filter(|line| line == check).count(),
                1,
                "{output}"
            );
        }
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
}
