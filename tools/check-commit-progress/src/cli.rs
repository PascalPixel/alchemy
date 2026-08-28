//! Commit-subject validation against the current verified production build.

use std::path::{Path, PathBuf};
use std::process::{Command, ExitCode};

fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap()
        .to_path_buf()
}

fn progress_subject(root: &Path, target: &str) -> Result<String, String> {
    let output = Command::new("cargo")
        .args([
            "run",
            "--offline",
            "--quiet",
            "--release",
            "--manifest-path",
        ])
        .arg(root.join("tools/check/Cargo.toml"))
        .args(["--", "progress", "--target", target, "--subject"])
        .current_dir(root)
        .output()
        .map_err(|error| format!("could not calculate progress: {error}"))?;
    if !output.status.success() {
        return Err(String::from_utf8_lossy(&output.stderr).trim().to_string());
    }
    String::from_utf8(output.stdout)
        .map(|value| value.trim().to_string())
        .map_err(|error| error.to_string())
}

fn git_output(root: &Path, arguments: &[&str]) -> Result<std::process::Output, String> {
    Command::new("git")
        .args(arguments)
        .current_dir(root)
        .output()
        .map_err(|error| format!("could not run git: {error}"))
}

fn require_verified_index(root: &Path, target: &str) -> Result<(), String> {
    if !git_output(root, &["diff", "--quiet", "--ignore-submodules", "--"])?
        .status
        .success()
    {
        return Err("unstaged tracked changes found; stage them and rerun make verify".into());
    }
    let untracked = git_output(root, &["ls-files", "--others", "--exclude-standard"])?;
    if !untracked.status.success() {
        return Err("could not inspect untracked files".into());
    }
    let untracked = String::from_utf8_lossy(&untracked.stdout);
    if !untracked.trim().is_empty() {
        return Err("untracked files found; stage or ignore them and rerun make verify".into());
    }
    let attestation = root.join("out").join(target).join("reports/verified-tree");
    let expected = std::fs::read_to_string(&attestation).map_err(|_| {
        format!(
            "verified-tree attestation is missing; stage the tree and run make verify ({})",
            attestation.display()
        )
    })?;
    let tree = git_output(root, &["write-tree"])?;
    if !tree.status.success() {
        return Err(String::from_utf8_lossy(&tree.stderr).trim().to_string());
    }
    let actual = String::from_utf8_lossy(&tree.stdout);
    if actual.trim() != expected.trim() {
        return Err("staged tree changed after verification; rerun make verify".into());
    }
    Ok(())
}

fn check(message: &str, expected: &str) -> Result<(), String> {
    let subject = message.lines().next().unwrap_or("");
    let Some(title) = subject.strip_prefix(expected) else {
        return Err(format!("commit subject must start with {expected}"));
    };
    if !title.starts_with(' ') || title.trim().is_empty() {
        return Err(format!("commit subject needs a title after {expected}"));
    }
    Ok(())
}

fn self_test() -> Result<(), String> {
    let expected = "☀️ 52% –";
    check("☀️ 52% – Name the owner", expected)?;
    for bad in [
        "missing",
        "☀️ 51% – stale",
        "☀️ 52% - wrong dash",
        "☀️ 52% –",
    ] {
        if check(bad, expected).is_ok() {
            return Err(format!("invalid subject accepted: {bad}"));
        }
    }
    println!("self-test=ok prefix=done-percent");
    Ok(())
}

fn run(arguments: &[String]) -> Result<(), String> {
    let mut args = arguments.to_vec();
    if args.iter().any(|arg| arg == "--self-test") {
        return self_test();
    }
    if args.iter().any(|arg| arg == "--help" || arg == "-h") {
        println!("usage: check-commit-progress [--target TARGET] COMMIT_MESSAGE");
        return Ok(());
    }
    let mut target = "gs1-en".to_string();
    if let Some(index) = args.iter().position(|arg| arg == "--target") {
        if index + 1 >= args.len() {
            return Err("--target requires a value".into());
        }
        target = args.remove(index + 1);
        args.remove(index);
    }
    if target != "gs1-en" && target != "gs2-en" {
        return Err(format!("unknown target: {target}"));
    }
    let message = std::fs::read_to_string(
        args.first()
            .ok_or("usage: check-commit-progress [--target TARGET] COMMIT_MESSAGE")?,
    )
    .map_err(|error| error.to_string())?;
    let repository = root();
    require_verified_index(&repository, &target)?;
    check(&message, &progress_subject(&repository, &target)?)
}

pub fn entry(arguments: &[String]) -> ExitCode {
    match run(arguments) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
