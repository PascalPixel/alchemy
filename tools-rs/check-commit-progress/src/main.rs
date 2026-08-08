use serde_json::Value;
use std::path::{Path, PathBuf};
use std::process::{Command, ExitCode};

#[derive(Clone, Debug)]
struct Report {
    full_c_bytes: u64,
    executable_bytes: u64,
    remaining_bytes: u64,
}

#[derive(Clone, Copy, Debug)]
struct Subject {
    full_c_kb: u64,
    executable_kb: u64,
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

fn git(root: &Path, args: &[&str]) -> Result<String, String> {
    command_output(Path::new("git"), args, root)
}

fn canonical_count(text: &str) -> Option<u64> {
    if text.is_empty() || !text.bytes().all(|b| b.is_ascii_digit() || b == b',') {
        return None;
    }
    let value = text.replace(',', "").parse::<u64>().ok()?;
    if commas(value) == text {
        Some(value)
    } else {
        None
    }
}

fn commas(value: u64) -> String {
    let digits = value.to_string();
    let mut out = String::new();
    for (i, ch) in digits.chars().enumerate() {
        if i > 0 && (digits.len() - i) % 3 == 0 {
            out.push(',');
        }
        out.push(ch);
    }
    out
}

fn format_subject(report: &Report) -> String {
    format!(
        "[ ☀️ {} / {} ]",
        commas(report.full_c_bytes / 1000),
        commas(report.executable_bytes / 1000)
    )
}

fn parse_subject(text: &str) -> Option<Subject> {
    let marker = "[ ☀️ ";
    let start = text.rfind(marker)?;
    if !text.ends_with(" ]") {
        return None;
    }
    let body = &text[start + marker.len()..text.len() - 2];
    let (left, right) = body.split_once(" / ")?;
    if right.contains(" / ") {
        return None;
    }
    let full_c_kb = canonical_count(left)?;
    let executable_kb = canonical_count(right)?;
    (full_c_kb <= executable_kb).then_some(Subject {
        full_c_kb,
        executable_kb,
    })
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
    if result.full_c_bytes > result.executable_bytes
        || result.full_c_bytes.checked_add(result.remaining_bytes) != Some(result.executable_bytes)
    {
        return Err("staged report arithmetic is invalid".into());
    }
    Ok(result)
}

fn legacy_subject(subject: &str) -> bool {
    let suffix = subject.rsplit_once('[').map(|(_, s)| s).unwrap_or("");
    suffix.ends_with(']')
        && (suffix.contains(" of ") || suffix.starts_with("C ") || suffix.contains(" C bytes]"))
}

fn check(message: &str, report: &Report, previous: Option<&str>) -> Result<(), String> {
    let subject = message.lines().next().unwrap_or("");
    let parsed = parse_subject(subject)
        .ok_or_else(|| format!("commit subject must end with {}", format_subject(report)))?;
    if parsed.full_c_kb != report.full_c_bytes / 1000
        || parsed.executable_kb != report.executable_bytes / 1000
    {
        return Err(format!(
            "commit suffix is stale; expected {}",
            format_subject(report)
        ));
    }
    let Some(previous) = previous else {
        return Ok(());
    };
    if let Some(old) = parse_subject(previous) {
        if parsed.executable_kb != old.executable_kb
            && !subject.starts_with("metrics: correct executable denominator")
        {
            return Err(format!("executable denominator changed from {} to {}; use an explicit metrics: correct executable denominator commit", old.executable_kb, parsed.executable_kb));
        }
        if parsed.full_c_kb < old.full_c_kb
            && !subject.starts_with("metrics: revert reduces Full-C")
        {
            return Err(format!("Full-C bytes regressed from {} to {}; if deliberate use a 'metrics: revert reduces Full-C' commit carrying the true current suffix", old.full_c_kb, parsed.full_c_kb));
        }
    } else if !legacy_subject(previous)
        && !subject.starts_with("metrics: restore Full-C counter chain")
    {
        return Err("previous commit has neither canonical Full-C nor recognized transition syntax; resume the chain with a 'metrics: restore Full-C counter chain' commit carrying the current suffix".into());
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
    let binary = root.join("tools-rs/full-c-progress/target/release/full-c-progress");
    let output = command_output(&binary, &["--target", target, "--json"], root)?;
    serde_json::from_str(&output).map_err(|e| format!("invalid full-c-progress output: {e}"))
}

fn self_test() -> Result<(), String> {
    let report = Report {
        full_c_bytes: 123456,
        executable_bytes: 1234567,
        remaining_bytes: 1111111,
    };
    check(
        "ok [ ☀️ 123 / 1,234 ]",
        &report,
        Some("old [C 123,456/1,234,567 bytes]"),
    )?;
    check(
        "metrics: restore Full-C counter chain [ ☀️ 123 / 1,234 ]",
        &report,
        Some("docs"),
    )?;
    check(
        "metrics: revert reduces Full-C [ ☀️ 123 / 1,234 ]",
        &report,
        Some("old [ ☀️ 200 / 1,234 ]"),
    )?;
    for bad in [
        "missing",
        "bad [ ☀️ 1234 / 1,234 ]",
        "stale [ ☀️ 122 / 1,234 ]",
    ] {
        if check(bad, &report, None).is_ok() {
            return Err(format!("invalid subject accepted: {bad}"));
        }
    }
    if check(
        "regress [ ☀️ 123 / 1,234 ]",
        &report,
        Some("old [ ☀️ 200 / 1,234 ]"),
    )
    .is_ok()
    {
        return Err("unannounced regression accepted".into());
    }
    println!("self-test=ok suffix=full-c-byte-share");
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
    let previous = git(&root, &["log", "-1", "--format=%s"])?
        .trim()
        .to_string();
    if let Some(old) = parse_subject(&previous) {
        if metric.executable_bytes / 1000 != old.executable_kb
            && !paths.contains(&format!("metrics/{target}-executable.json"))
        {
            return Err(format!(
                "denominator correction requires staged metrics/{target}-executable.json"
            ));
        }
    }
    let message = std::fs::read_to_string(message_path).map_err(|e| e.to_string())?;
    check(
        &message,
        &metric,
        (!previous.is_empty()).then_some(previous.as_str()),
    )
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
