//! Discover and run the native Rust self-tests.
//!
//! Each runnable command is classified by the dispatch registry and Cargo
//! metadata. This runner executes those classifications with bounded
//! parallelism and refuses to pass when discovery finds nothing.
//!
//! ```text
//! cargo run --manifest-path tools/self-test/Cargo.toml
//! cargo run --manifest-path tools/self-test/Cargo.toml -- --list
//! cargo run --manifest-path tools/self-test/Cargo.toml -- --jobs 4
//! cargo run --manifest-path tools/self-test/Cargo.toml -- --self-test
//! ```

use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::{Path, PathBuf};
use std::process::{Command, Stdio};
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::{Arc, Mutex};
use std::time::Instant;

const JOB_SHARE_NUMERATOR: usize = 4;
const JOB_SHARE_DENOMINATOR: usize = 5;
const MAX_CARGO_WORKERS: usize = 4;

fn jobs_for(cores: usize) -> usize {
    cores
        .saturating_mul(JOB_SHARE_NUMERATOR)
        .checked_div(JOB_SHARE_DENOMINATOR)
        .unwrap_or(0)
        .clamp(1, MAX_CARGO_WORKERS)
}

fn available_parallelism() -> usize {
    std::thread::available_parallelism()
        .map(|count| count.get())
        .unwrap_or(1)
}

fn parse_jobs(args: &[String], cores: usize) -> Result<usize, String> {
    let cap = jobs_for(cores);
    let mut requested = None;
    let mut index = 0;
    while index < args.len() {
        let argument = &args[index];
        let raw = if argument == "--jobs" {
            index += 1;
            args.get(index)
                .ok_or_else(|| "--jobs requires a positive integer".to_string())?
        } else if let Some(value) = argument.strip_prefix("--jobs=") {
            value
        } else {
            index += 1;
            continue;
        };

        if requested.is_some() {
            return Err("--jobs may only be specified once".to_string());
        }
        let value = raw
            .parse::<usize>()
            .map_err(|_| format!("invalid --jobs value: {raw}"))?;
        if value == 0 {
            return Err("--jobs must be at least 1".to_string());
        }
        requested = Some(value.min(cap));
        index += 1;
    }
    Ok(requested.unwrap_or(cap))
}

#[derive(Debug, Clone)]
struct CargoBinary {
    crate_name: String,
    binary: String,
    manifest: PathBuf,
}

fn cargo_binary_targets(root: &Path) -> Result<Vec<CargoBinary>, String> {
    let tools = root.join("tools");
    let mut manifests = fs::read_dir(&tools)
        .map_err(|error| format!("could not scan {}: {error}", tools.display()))?
        .flatten()
        .map(|entry| entry.path().join("Cargo.toml"))
        .filter(|path| path.is_file())
        .collect::<Vec<_>>();
    manifests.sort_by(|left, right| left.as_os_str().cmp(right.as_os_str()));

    let mut targets = Vec::new();
    for manifest in manifests {
        let manifest = fs::canonicalize(&manifest)
            .map_err(|error| format!("{}: {error}", manifest.display()))?;
        let output = Command::new("cargo")
            .args([
                "metadata",
                "--offline",
                "--no-deps",
                "--format-version",
                "1",
                "--manifest-path",
            ])
            .arg(&manifest)
            .current_dir(root)
            .output()
            .map_err(|error| format!("cargo metadata {}: {error}", manifest.display()))?;
        if !output.status.success() {
            return Err(format!(
                "cargo metadata {} failed: {}",
                manifest.display(),
                String::from_utf8_lossy(&output.stderr).trim()
            ));
        }
        let metadata: serde_json::Value =
            serde_json::from_slice(&output.stdout).map_err(|error| {
                format!(
                    "cargo metadata {} was invalid JSON: {error}",
                    manifest.display()
                )
            })?;
        let package = metadata
            .get("packages")
            .and_then(serde_json::Value::as_array)
            .and_then(|packages| {
                packages.iter().find(|package| {
                    package
                        .get("manifest_path")
                        .and_then(serde_json::Value::as_str)
                        == Some(manifest.to_string_lossy().as_ref())
                })
            })
            .ok_or_else(|| format!("cargo metadata omitted {}", manifest.display()))?;
        let crate_name = manifest
            .parent()
            .and_then(Path::file_name)
            .and_then(|name| name.to_str())
            .ok_or_else(|| format!("{} has no native crate directory", manifest.display()))?;
        for target in package
            .get("targets")
            .and_then(serde_json::Value::as_array)
            .into_iter()
            .flatten()
        {
            let is_binary = target
                .get("kind")
                .and_then(serde_json::Value::as_array)
                .is_some_and(|kinds| kinds.iter().any(|kind| kind.as_str() == Some("bin")));
            if !is_binary {
                continue;
            }
            let binary = target
                .get("name")
                .and_then(serde_json::Value::as_str)
                .ok_or_else(|| {
                    format!(
                        "cargo metadata gave {} an unnamed binary",
                        manifest.display()
                    )
                })?;
            targets.push(CargoBinary {
                crate_name: crate_name.to_string(),
                binary: binary.to_string(),
                manifest: manifest.clone(),
            });
        }
    }
    targets.sort_by(|left, right| {
        (&left.crate_name, &left.binary).cmp(&(&right.crate_name, &right.binary))
    });
    Ok(targets)
}

fn native_root(root: &Path) -> Result<PathBuf, String> {
    let directory = root.join("tools");
    directory
        .join("self-test/Cargo.toml")
        .is_file()
        .then_some(directory)
        .ok_or_else(|| format!("could not find the commands root under {}", root.display()))
}

#[derive(Debug, Clone)]
struct Invocation {
    name: String,
    program: PathBuf,
    arguments: Vec<String>,
    resource_group: Option<&'static str>,
}

/// These two self-tests both cold-populate the same resource_380 overlay-C
/// entries. Keep that one shared population transaction together; all other
/// native self-tests retain the worker pool's normal parallelism.
fn resource_group(crate_name: &str) -> Option<&'static str> {
    match crate_name {
        "overlay-driver" | "overlay-published" => Some("overlay-c/resource_380"),
        _ => None,
    }
}

fn cargo_run_arguments(manifest: &Path, binary_name: &str) -> Vec<String> {
    vec![
        "run".to_string(),
        "--offline".to_string(),
        "--quiet".to_string(),
        "--release".to_string(),
        "--manifest-path".to_string(),
        manifest.to_string_lossy().into_owned(),
        "--bin".to_string(),
        binary_name.to_string(),
        "--".to_string(),
        "--self-test".to_string(),
    ]
}

fn native_tools(root: &Path) -> Result<Vec<Invocation>, String> {
    native_root(root)?;
    let mut by_crate: BTreeMap<String, Vec<(String, PathBuf, Vec<String>)>> = BTreeMap::new();

    for target in cargo_binary_targets(root)? {
        if !dispatch::should_self_test(&target.crate_name, &target.binary) {
            continue;
        }
        by_crate
            .entry(target.crate_name.clone())
            .or_default()
            .push((
                target.binary.clone(),
                PathBuf::from("cargo"),
                cargo_run_arguments(&target.manifest, &target.binary),
            ));
    }

    if by_crate.is_empty() {
        return Ok(Vec::new());
    }

    let mut tools = Vec::new();
    for (crate_name, mut binaries) in by_crate {
        binaries.sort_by(|left, right| left.0.as_bytes().cmp(right.0.as_bytes()));
        let multiple = binaries.len() > 1;
        for (binary_name, program, arguments) in binaries {
            let name = if multiple {
                format!("rust/{crate_name}/{binary_name}")
            } else {
                format!("rust/{crate_name}")
            };
            tools.push(Invocation {
                name,
                program,
                arguments,
                resource_group: resource_group(&crate_name),
            });
        }
    }
    tools.sort_by(|left, right| left.name.as_bytes().cmp(right.name.as_bytes()));
    Ok(tools)
}

#[derive(Debug)]
struct Outcome {
    tool: String,
    ok: bool,
    output: String,
    elapsed_ms: u128,
}

/// Classified commands use one of the established self-test success signals.
///
/// The machine-readable form is the `self-test=ok` token.  Older commands use
/// a human-readable `self-test ok` or `self-test passed` token pair, and the
/// staff-roll command reports its validated positive output size as
/// `self-test bytes=<positive integer>`.  Requiring one of these explicit
/// signals makes a zero exit status necessary but insufficient; arbitrary
/// prose, including an empty or failure-only report, cannot pass.
#[derive(Debug, Clone, Copy, Eq, PartialEq)]
enum SuccessSignal {
    KeyValue,
    HumanReadable,
    PositiveByteCount(usize),
}

fn positive_byte_count(token: &str) -> Option<usize> {
    token
        .strip_prefix("bytes=")
        .and_then(|value| value.parse::<usize>().ok())
        .filter(|value| *value > 0)
}

fn self_test_success_signal(output: &str) -> Option<SuccessSignal> {
    let mut signal = None;
    for line in output.lines() {
        let tokens: Vec<_> = line.split_whitespace().collect();
        if tokens
            .iter()
            .any(|token| matches!(*token, "self-test=failed" | "self-test=failure"))
            || tokens
                .windows(2)
                .any(|window| matches!(window, ["self-test", "failed" | "failure"]))
        {
            return None;
        }

        if tokens.contains(&"self-test=ok") {
            signal = Some(SuccessSignal::KeyValue);
        }
        for window in tokens.windows(2) {
            match window {
                ["self-test", "ok" | "passed"] => {
                    signal = Some(SuccessSignal::HumanReadable);
                }
                ["self-test", bytes] => {
                    if let Some(count) = positive_byte_count(bytes) {
                        signal = Some(SuccessSignal::PositiveByteCount(count));
                    }
                }
                _ => {}
            }
        }
    }
    signal
}

fn validate_self_test(exit_success: bool, output: &str) -> Result<(), String> {
    if !exit_success {
        return Err("process exited unsuccessfully".to_string());
    }
    if self_test_success_signal(output).is_none() {
        return Err(
            "process exited 0 without a recognized self-test success signal "
                .to_string()
                + "(`self-test=ok`, `self-test ok|passed`, or `self-test bytes=<positive integer>`)",
        );
    }
    Ok(())
}

fn combined_output(stdout: &[u8], stderr: &[u8]) -> String {
    let stdout = String::from_utf8_lossy(stdout);
    let stderr = String::from_utf8_lossy(stderr);
    match (stdout.is_empty(), stderr.is_empty()) {
        (true, true) => String::new(),
        (false, true) => stdout.into_owned(),
        (true, false) => stderr.into_owned(),
        (false, false) => format!("{stdout}\n{stderr}"),
    }
}

fn run_one(root: &Path, tool: &Invocation) -> Outcome {
    let started = Instant::now();
    let spawned = Command::new(&tool.program)
        .args(&tool.arguments)
        .current_dir(root)
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
        .output();
    let elapsed_ms = started.elapsed().as_millis();
    match spawned {
        Ok(output) => {
            let report = combined_output(&output.stdout, &output.stderr);
            let validation = validate_self_test(output.status.success(), &report);
            let ok = validation.is_ok();
            let mut detail = report.trim().to_string();
            if let Err(reason) = validation {
                detail = if detail.is_empty() && !output.status.success() {
                    format!("{reason}: process exited with {}", output.status)
                } else if detail.is_empty() {
                    reason
                } else {
                    format!("{reason}\n{detail}")
                };
            }
            Outcome {
                tool: tool.name.clone(),
                ok,
                output: detail,
                elapsed_ms,
            }
        }
        Err(error) => Outcome {
            tool: tool.name.clone(),
            ok: false,
            output: format!("could not spawn {}: {error}", tool.program.display()),
            elapsed_ms,
        },
    }
}

fn run_all(root: &Path, tools: &[Invocation], jobs: usize) -> Vec<Outcome> {
    let next = Arc::new(AtomicUsize::new(0));
    let results = Arc::new(Mutex::new(Vec::with_capacity(tools.len())));
    let resource_locks: Arc<BTreeMap<&'static str, Arc<Mutex<()>>>> = Arc::new(
        tools
            .iter()
            .filter_map(|tool| tool.resource_group)
            .collect::<BTreeSet<_>>()
            .into_iter()
            .map(|resource| (resource, Arc::new(Mutex::new(()))))
            .collect(),
    );
    let workers = jobs.min(tools.len());
    std::thread::scope(|scope| {
        for _ in 0..workers {
            let next = Arc::clone(&next);
            let results = Arc::clone(&results);
            let resource_locks = Arc::clone(&resource_locks);
            scope.spawn(move || loop {
                let index = next.fetch_add(1, Ordering::Relaxed);
                if index >= tools.len() {
                    break;
                }
                let _resource_guard = tools[index]
                    .resource_group
                    .and_then(|resource| resource_locks.get(resource))
                    .map(|lock| lock.lock().expect("self-test resource mutex"));
                let outcome = run_one(root, &tools[index]);
                results
                    .lock()
                    .expect("self-test results mutex")
                    .push(outcome);
            });
        }
    });
    Arc::try_unwrap(results)
        .expect("self-test workers still hold results")
        .into_inner()
        .expect("self-test results mutex")
}

fn tail_lines(output: &str, count: usize) -> impl Iterator<Item = &str> {
    let lines: Vec<&str> = output.lines().collect();
    let start = lines.len().saturating_sub(count);
    lines.into_iter().skip(start)
}

fn repo_root() -> PathBuf {
    if let Ok(explicit) = std::env::var("ALCHEMY_ROOT") {
        return PathBuf::from(explicit);
    }
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .map(Path::to_path_buf)
        .unwrap_or_else(|| PathBuf::from("."))
}

fn self_test() {
    assert_eq!(jobs_for(18), 4);
    assert_eq!(jobs_for(1), 1);
    assert_eq!(parse_jobs(&["--jobs".into(), "4".into()], 18), Ok(4));
    assert_eq!(parse_jobs(&["--jobs=64".into()], 18), Ok(4));
    assert!(parse_jobs(&["--jobs".into()], 18).is_err());
    assert!(parse_jobs(&["--jobs".into(), "0".into()], 18).is_err());
    assert!(dispatch::should_self_test("build-rom", "build-rom"));
    assert!(dispatch::should_self_test("discover", "discover"));
    assert!(dispatch::should_self_test(
        "candidate-explain",
        "candidate-explain"
    ));
    assert!(!dispatch::should_self_test(
        "compiler-corpus-regression",
        "compiler-corpus-regression-bench"
    ));
    assert!(!dispatch::should_self_test("text-bg", "text-bg"));
    assert_eq!(
        tail_lines("1\n2\n3\n4\n5\n6\n7", 6).collect::<Vec<_>>(),
        vec!["2", "3", "4", "5", "6", "7"]
    );
    println!("self_test self-test ok");
}

fn main() {
    let args: Vec<String> = std::env::args().skip(1).collect();
    if args.iter().any(|argument| argument == "--self-test") {
        self_test();
        return;
    }

    let root = repo_root();
    let jobs = match parse_jobs(&args, available_parallelism()) {
        Ok(jobs) => jobs,
        Err(error) => {
            eprintln!("self-test: {error}");
            std::process::exit(2);
        }
    };
    let tools = match native_tools(&root) {
        Ok(tools) => tools,
        Err(error) => {
            eprintln!("self-test: {error}");
            std::process::exit(1);
        }
    };

    // A successful empty run is a vacuous pass.  Keep this guard immediately
    // after discovery so both normal execution and `--list` reject a broken
    // root or an accidentally unregistered native crate.
    if tools.is_empty() {
        eprintln!(
            "no classified command exposes a self-test under {} -- refusing to pass without scanning anything",
            native_root(&root)
                .map(|path| path.display().to_string())
                .unwrap_or_else(|_| "the repository".to_string())
        );
        std::process::exit(1);
    }

    if args.iter().any(|argument| argument == "--list") {
        for tool in &tools {
            println!("  {}", tool.name);
        }
        println!("{} commands expose a self-test", tools.len());
        return;
    }

    let started = Instant::now();
    let mut results = run_all(&root, &tools, jobs);
    results.sort_by(|left, right| left.tool.as_bytes().cmp(right.tool.as_bytes()));
    let failed: Vec<&Outcome> = results.iter().filter(|result| !result.ok).collect();
    for result in &failed {
        eprintln!("FAIL {} ({}ms)", result.tool, result.elapsed_ms);
        for line in tail_lines(&result.output, 6) {
            eprintln!("     {line}");
        }
    }
    let seconds = started.elapsed().as_secs_f64();
    println!(
        "self-tests: {}/{} passed in {seconds:.1}s (jobs={jobs})",
        results.len() - failed.len(),
        results.len()
    );
    if !failed.is_empty() {
        std::process::exit(1);
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn policy_exposes_public_and_non_public_self_tests() {
        assert!(dispatch::public_target("build-rom", "build-rom"));
        assert!(dispatch::should_self_test("build-rom", "build-rom"));
        assert!(dispatch::public_target("discover", "discover"));
        assert!(dispatch::should_self_test("discover", "discover"));
        assert_eq!(
            dispatch::non_public_target("rtl-insn", "rtl-insn").map(|target| target.kind),
            Some(dispatch::NonPublicKind::InternalDiagnostic)
        );
        assert!(!dispatch::should_self_test(
            "integrate-matches",
            "integrate-matches-bench"
        ));
    }

    #[test]
    fn jobs_are_bounded_and_invalid_values_are_rejected() {
        assert_eq!(jobs_for(18), 4);
        assert_eq!(jobs_for(2), 1);
        assert_eq!(parse_jobs(&[], 18), Ok(4));
        assert_eq!(parse_jobs(&["--jobs".into(), "4".into()], 18), Ok(4));
        assert_eq!(parse_jobs(&["--jobs=64".into()], 18), Ok(4));
        assert!(parse_jobs(&["--jobs".into(), "nope".into()], 18).is_err());
        assert!(parse_jobs(&["--jobs".into(), "0".into()], 18).is_err());
    }

    #[test]
    fn cargo_command_plan_is_offline_and_selects_exactly_one_binary() {
        let arguments = cargo_run_arguments(Path::new("tools/example/Cargo.toml"), "example");
        assert_eq!(arguments[0], "run");
        assert_eq!(arguments[1], "--offline");
        assert_eq!(arguments[2], "--quiet");
        assert_eq!(
            arguments
                .iter()
                .filter(|argument| argument.as_str() == "--bin")
                .count(),
            1
        );
        assert_eq!(arguments[arguments.len() - 2], "--");
        assert_eq!(arguments[arguments.len() - 1], "--self-test");
    }

    #[test]
    fn success_signal_contract_accepts_all_inventory_variants() {
        assert_eq!(
            self_test_success_signal("self-test=ok resources=8"),
            Some(SuccessSignal::KeyValue)
        );
        assert_eq!(
            self_test_success_signal("mode sweep self-test passed (16 checks)"),
            Some(SuccessSignal::HumanReadable)
        );
        assert_eq!(
            self_test_success_signal("staff_roll: self-test bytes=5540"),
            Some(SuccessSignal::PositiveByteCount(5540))
        );
    }

    #[test]
    fn success_signal_contract_rejects_arbitrary_and_contradictory_output() {
        for output in [
            "",
            "all checks completed",
            "self-test=okay",
            "not-self-test=ok",
            "self-test bytes=0",
            "self-test bytes=not-a-number",
            "self-test failed",
            "self-test=ok\nself-test=failed",
        ] {
            assert_eq!(
                self_test_success_signal(output),
                None,
                "unexpected success signal in {output:?}"
            );
        }
        assert!(validate_self_test(true, "all checks completed").is_err());
        assert!(validate_self_test(false, "self-test=ok").is_err());
    }

    #[test]
    fn success_signal_may_be_emitted_on_either_output_stream() {
        let output = combined_output(b"diagnostic output", b"self-test=ok\n");
        assert_eq!(output, "diagnostic output\nself-test=ok\n");
        assert!(validate_self_test(true, &output).is_ok());
    }

    #[test]
    fn zero_exit_without_a_signal_cannot_pass() {
        let invocation = Invocation {
            name: "fixture/empty".to_string(),
            program: PathBuf::from("sh"),
            arguments: vec!["-c".to_string(), "exit 0".to_string()],
            resource_group: None,
        };
        let outcome = run_one(Path::new("."), &invocation);
        assert!(!outcome.ok);
        assert!(outcome
            .output
            .contains("without a recognized self-test success signal"));
    }

    #[test]
    fn nonempty_non_signal_output_cannot_pass() {
        let invocation = Invocation {
            name: "fixture/prose".to_string(),
            program: PathBuf::from("sh"),
            arguments: vec![
                "-c".to_string(),
                "printf 'all checks completed\\n'".to_string(),
            ],
            resource_group: None,
        };
        let outcome = run_one(Path::new("."), &invocation);
        assert!(!outcome.ok);
        assert!(outcome.output.contains("all checks completed"));
    }

    #[test]
    fn resource_group_serializes_the_shared_overlay_population() {
        assert_eq!(
            resource_group("overlay-driver"),
            Some("overlay-c/resource_380")
        );
        assert_eq!(
            resource_group("overlay-published"),
            Some("overlay-c/resource_380")
        );
        assert_eq!(resource_group("overlay-show"), None);
    }

    #[test]
    fn failure_reports_are_limited_to_the_last_six_lines() {
        assert_eq!(
            tail_lines("1\n2\n3\n4\n5\n6\n7", 6).collect::<Vec<_>>(),
            vec!["2", "3", "4", "5", "6", "7"]
        );
        assert_eq!(tail_lines("only", 6).collect::<Vec<_>>(), vec!["only"]);
    }

    #[test]
    fn native_root_is_selected_without_a_tool_manifest_registry() {
        let root = Path::new(env!("CARGO_MANIFEST_DIR"))
            .parent()
            .and_then(Path::parent)
            .expect("repository root");
        assert!(native_root(root).is_ok());
    }
}
