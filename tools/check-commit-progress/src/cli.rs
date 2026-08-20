//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

use serde_json::Value;
use std::path::{Path, PathBuf};
use std::process::{Command, ExitCode};

#[derive(Clone, Debug)]
struct Report {
    full_c_bytes: u64,
    executable_bytes: u64,
    remaining_bytes: u64,
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
struct Subject {
    exact_c_percent: u64,
}

fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap()
        .to_path_buf()
}

fn command_output(program: &Path, args: &[&str], cwd: &Path) -> Result<String, String> {
    let output = Command::new(program)
        .args(args)
        .current_dir(cwd)
        .output()
        .map_err(|e| format!("could not run {}: {e}", program.display()))?;
    if !output.status.success() {
        return Err(String::from_utf8_lossy(&output.stderr).trim().to_string());
    }
    String::from_utf8(output.stdout).map_err(|e| e.to_string())
}

/// `cargo run` for the progress reporter.
///
/// `full-c-progress` is a `check` subcommand now, not a binary of its own: the
/// entry-point consolidation linked it in and stripped its `[[bin]]`, but this
/// gate went on naming its manifest, so every commit died on cargo's "a bin
/// target must be available" -- a commit hook that could not let any commit
/// through.
fn cargo_command(root: &Path, subcommand: &str) -> Command {
    let mut command = Command::new("cargo");
    command
        .args([
            "run",
            "--offline",
            "--quiet",
            "--release",
            "--manifest-path",
        ])
        .arg(root.join("tools").join("check").join("Cargo.toml"))
        .arg("--")
        .arg(subcommand)
        .current_dir(root);
    command
}

fn cargo_output(root: &Path, crate_name: &str, args: &[&str]) -> Result<String, String> {
    let output = cargo_command(root, crate_name)
        .args(args)
        .output()
        .map_err(|e| format!("could not run cargo for {crate_name}: {e}"))?;
    if !output.status.success() {
        return Err(String::from_utf8_lossy(&output.stderr).trim().to_string());
    }
    String::from_utf8(output.stdout).map_err(|e| e.to_string())
}

fn git(root: &Path, args: &[&str]) -> Result<String, String> {
    command_output(Path::new("git"), args, root)
}

/// Permanent-assembly bytes, from the tracked coverage map.
///
/// The subject prefix is DONE -- exact C plus permanent assembly -- and the
/// progress report only carries the first term.
fn permanent_asm_bytes(root: &Path, target: &str) -> Result<u64, String> {
    // The INDEX copy, like the progress report above it. Reading the working
    // tree would let an unstaged map set the percentage of a commit that does
    // not contain it.
    let path = format!("metrics/{target}-coverage-map.json");
    let text = git(root, &["show", &format!(":{path}")])
        .map_err(|_| format!("stage {path} before committing"))?;
    let document: serde_json::Value =
        serde_json::from_str(&text).map_err(|error| format!("{path}: {error}"))?;
    document["categories"]["retained_asm"]["bytes"]
        .as_u64()
        .ok_or_else(|| format!("{path} has no categories.retained_asm.bytes"))
}

/// The nearest whole DONE share. It was the Exact-C share alone until 2026-08;
/// the sun is the project's headline and the headline is DONE.
fn done_percent(report: &Report, permanent: u64) -> Result<u64, String> {
    let done = report
        .full_c_bytes
        .checked_add(permanent)
        .ok_or("DONE percentage arithmetic overflow")?;
    if done > report.executable_bytes {
        return Err("DONE numerator exceeds executable denominator".to_string());
    }
    let scaled = done
        .checked_mul(100)
        .ok_or("DONE percentage arithmetic overflow")?;
    scaled
        .checked_add(report.executable_bytes / 2)
        .ok_or_else(|| "DONE percentage arithmetic overflow".to_string())
        .map(|rounded| rounded / report.executable_bytes)
}

fn format_subject(report: &Report, permanent: u64) -> Result<String, String> {
    Ok(format!("☀️ {}% –", done_percent(report, permanent)?))
}

fn parse_subject(text: &str) -> Option<Subject> {
    let rest = text.strip_prefix("☀️ ")?;
    let count = rest
        .find(|character: char| !character.is_ascii_digit())
        .unwrap_or(rest.len());
    if count == 0 || count > 3 {
        return None;
    }
    let (percent, rest) = rest.split_at(count);
    if !rest.starts_with("% – ") || rest.len() == "% – ".len() {
        return None;
    }
    let exact_c_percent = percent.parse::<u64>().ok()?;
    (exact_c_percent <= 100).then_some(Subject { exact_c_percent })
}

fn report(value: &Value, target: &str) -> Result<Report, String> {
    let object = value
        .as_object()
        .ok_or("staged Full-C report is not an object")?;
    let string = |key: &str| object.get(key).and_then(Value::as_str);
    if object.get("format").and_then(Value::as_u64) != Some(1)
        || string("metric") != Some("full-c-byte-share")
        || string("target") != Some(target)
        || string("audit") != Some("complete")
    {
        return Err(format!(
            "staged {target} Full-C report is missing, incomplete, or has the wrong format"
        ));
    }
    let count = |key: &str| {
        object
            .get(key)
            .and_then(Value::as_u64)
            .ok_or_else(|| format!("staged report {key} is invalid"))
    };
    let result = Report {
        full_c_bytes: count("full_c_bytes")?,
        executable_bytes: count("executable_bytes")?,
        remaining_bytes: count("remaining_bytes")?,
    };
    if result.executable_bytes == 0
        || result.full_c_bytes > result.executable_bytes
        || result.full_c_bytes.checked_add(result.remaining_bytes) != Some(result.executable_bytes)
    {
        return Err("staged report arithmetic is invalid".into());
    }
    Ok(result)
}

fn check(message: &str, report: &Report, permanent: u64) -> Result<(), String> {
    let subject = message.lines().next().unwrap_or("");
    let expected = format_subject(report, permanent)?;
    let parsed = parse_subject(subject)
        .ok_or_else(|| format!("commit subject must start with {expected}"))?;
    if parsed.exact_c_percent != done_percent(report, permanent)? {
        return Err(format!(
            "commit DONE percentage is stale; expected {}",
            expected
        ));
    }
    Ok(())
}

fn staged_paths(root: &Path) -> Result<Vec<String>, String> {
    if !git(root, &["ls-files", "-u"])?.trim().is_empty() {
        return Err("cannot validate Full-C progress with unmerged index entries".into());
    }
    Ok(git(
        root,
        &["diff", "--cached", "--name-only", "--diff-filter=ACMRT"],
    )?
    .lines()
    .map(str::to_string)
    .collect())
}

fn report_required(paths: &[String], target: &str) -> bool {
    paths.iter().any(|path| {
        if target == "gs1-en" {
            path.starts_with("src/")
                || path.starts_with("asm/")
                || path.starts_with("include/")
                || path == "metrics/gs1-en-executable.json"
                || (path.starts_with("assets/code/resource_")
                    && (path.ends_with("_overlay.s")
                        || path
                            .rsplit('/')
                            .next()
                            .is_some_and(|n| n.starts_with("c_") && n.ends_with(".c"))))
        } else {
            path.starts_with("games/gs2/src/")
                || path.starts_with("games/gs2/asm/")
                || path.starts_with("games/gs2/include/")
                || path == "metrics/gs2-en-executable.json"
        }
    })
}

fn current_report(root: &Path, target: &str) -> Result<Value, String> {
    let output = cargo_output(root, "progress", &["--target", target, "--json"])?;
    serde_json::from_str(&output).map_err(|e| format!("invalid full-c-progress output: {e}"))
}

fn self_test() -> Result<(), String> {
    let report = Report {
        full_c_bytes: 123456,
        executable_bytes: 1234567,
        remaining_bytes: 1111111,
    };
    check("☀️ 10% – valid DONE prefix", &report, 0)?;
    for bad in [
        "missing",
        "☀️ 1000% – too wide",
        "☀️ 9% – stale",
        "☀️ 10% - wrong dash",
        "☀️ 10% –",
        "old [ ☀️ 123 / 1,234 ]",
    ] {
        if check(bad, &report, 0).is_ok() {
            return Err(format!("invalid subject accepted: {bad}"));
        }
    }
    println!("self-test=ok prefix=done-percent");
    Ok(())
}

fn run(arguments: &[String]) -> Result<(), String> {
    let mut args: Vec<String> = arguments.to_vec();
    if args.iter().any(|a| a == "--self-test") {
        return self_test();
    }
    if args.iter().any(|a| a == "--help" || a == "-h") {
        println!("usage: check-commit-progress [--target TARGET] COMMIT_MESSAGE");
        return Ok(());
    }
    let mut target = "gs1-en".to_string();
    if let Some(i) = args.iter().position(|a| a == "--target") {
        if i + 1 >= args.len() {
            return Err("--target requires a value".into());
        }
        target = args.remove(i + 1);
        args.remove(i);
    }
    if target != "gs1-en" && target != "gs2-en" {
        return Err(format!("unknown target: {target}"));
    }
    let message_path = args
        .first()
        .ok_or("usage: check-commit-progress [--target TARGET] COMMIT_MESSAGE")?;
    let root = root();
    let paths = staged_paths(&root)?;
    let report_path = format!("metrics/{target}-progress.json");
    let staged_text = git(&root, &["show", &format!(":{report_path}")])
        .map_err(|_| format!("stage {report_path} before committing"))?;
    let staged_value: Value = serde_json::from_str(&staged_text).map_err(|e| e.to_string())?;
    let metric = report(&staged_value, &target)?;
    if report_required(&paths, &target) && current_report(&root, &target)? != staged_value {
        return Err(format!("{report_path} is stale: regenerate it with the Rust full-c-progress tool and stage it with executable-source changes"));
    }
    let message = std::fs::read_to_string(message_path).map_err(|e| e.to_string())?;
    check(&message, &metric, permanent_asm_bytes(&root, &target)?)
}

pub fn entry(arguments: &[String]) -> ExitCode {
    match run(arguments) {
        Ok(()) => ExitCode::SUCCESS,
        Err(e) => {
            eprintln!("error: {e}");
            ExitCode::FAILURE
        }
    }
}
