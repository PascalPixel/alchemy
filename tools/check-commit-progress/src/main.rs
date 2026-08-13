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

fn cargo_command(root: &Path, crate_name: &str) -> Command {
    let mut command = Command::new("cargo");
    command
        .args([
            "run",
            "--offline",
            "--quiet",
            "--release",
            "--manifest-path",
        ])
        .arg(root.join("tools").join(crate_name).join("Cargo.toml"))
        .arg("--")
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

fn exact_c_percent(report: &Report) -> Result<u64, String> {
    let scaled = report
        .full_c_bytes
        .checked_mul(100)
        .ok_or("Exact-C percentage arithmetic overflow")?;
    scaled
        .checked_add(report.executable_bytes / 2)
        .ok_or_else(|| "Exact-C percentage arithmetic overflow".to_string())
        .map(|rounded| rounded / report.executable_bytes)
}

fn format_subject(report: &Report) -> Result<String, String> {
    Ok(format!("☀️ {}% –", exact_c_percent(report)?))
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

fn check(message: &str, report: &Report) -> Result<(), String> {
    let subject = message.lines().next().unwrap_or("");
    let expected = format_subject(report)?;
    let parsed = parse_subject(subject)
        .ok_or_else(|| format!("commit subject must start with {expected}"))?;
    if parsed.exact_c_percent != exact_c_percent(report)? {
        return Err(format!(
            "commit Exact-C percentage is stale; expected {}",
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
    let output = cargo_output(root, "full-c-progress", &["--target", target, "--json"])?;
    serde_json::from_str(&output).map_err(|e| format!("invalid full-c-progress output: {e}"))
}

fn self_test() -> Result<(), String> {
    let report = Report {
        full_c_bytes: 123456,
        executable_bytes: 1234567,
        remaining_bytes: 1111111,
    };
    check("☀️ 10% – valid Exact-C prefix", &report)?;
    for bad in [
        "missing",
        "☀️ 1000% – too wide",
        "☀️ 9% – stale",
        "☀️ 10% - wrong dash",
        "☀️ 10% –",
        "old [ ☀️ 123 / 1,234 ]",
    ] {
        if check(bad, &report).is_ok() {
            return Err(format!("invalid subject accepted: {bad}"));
        }
    }
    println!("self-test=ok prefix=exact-c-percent");
    Ok(())
}

fn run() -> Result<(), String> {
    let mut args: Vec<String> = std::env::args().skip(1).collect();
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
    check(&message, &metric)
}

fn main() -> ExitCode {
    match run() {
        Ok(()) => ExitCode::SUCCESS,
        Err(e) => {
            eprintln!("error: {e}");
            ExitCode::FAILURE
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn progress_child_is_cargo_authoritative() {
        let command = cargo_command(Path::new("/repo"), "full-c-progress");
        assert_eq!(command.get_program(), "cargo");
        let args: Vec<_> = command
            .get_args()
            .map(|arg| arg.to_string_lossy().into_owned())
            .collect();
        assert_eq!(
            args[0..6],
            [
                "run",
                "--offline",
                "--quiet",
                "--release",
                "--manifest-path",
                "/repo/tools/full-c-progress/Cargo.toml",
            ]
        );
        assert_eq!(args[6], "--");
    }
}
