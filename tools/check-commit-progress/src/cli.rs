//! Commit-subject validation against the current verified production build.

use std::path::Path;
use std::process::{Command, ExitCode};

const USAGE: &str = "usage: check commit-progress COMMIT_MESSAGE";

fn command(program: &Path, arguments: &[&str]) -> Result<String, String> {
    let output = Command::new(program)
        .args(arguments)
        .output()
        .map_err(|error| format!("could not run {}: {error}", program.display()))?;
    if !output.status.success() {
        return Err(String::from_utf8_lossy(&output.stderr).trim().into());
    }
    String::from_utf8(output.stdout)
        .map(|text| text.trim().into())
        .map_err(|error| error.to_string())
}

fn progress_subject() -> Result<String, String> {
    let executable = std::env::current_exe().map_err(|error| error.to_string())?;
    command(&executable, &["progress", "--subject"])
}

fn require_verified_index() -> Result<(), String> {
    command(
        Path::new("make"),
        &["--no-print-directory", "index-sync-check"],
    )?;
    let attestation = Path::new("out/gs1-en/reports/verified-tree");
    let expected = std::fs::read_to_string(attestation)
        .map_err(|_| "verified-tree attestation is missing; stage the tree and run make verify")?;
    if command(Path::new("git"), &["write-tree"])? != expected.trim() {
        return Err("staged tree changed after verification; rerun make verify".into());
    }
    Ok(())
}

fn valid(message: &str, expected: &str) -> bool {
    let subject = message.lines().next().unwrap_or("");
    subject
        .strip_prefix(expected)
        .is_some_and(|title| title.starts_with(' ') && !title.trim().is_empty())
}

fn self_test() -> Result<(), String> {
    let expected = "☀️ 52% –";
    let bad = [
        "missing",
        "☀️ 51% – stale",
        "☀️ 52% - wrong dash",
        "☀️ 52% –",
    ];
    if !valid("☀️ 52% – Name the owner", expected)
        || bad.into_iter().any(|message| valid(message, expected))
    {
        return Err("subject prefix self-test failed".into());
    }
    println!("self-test=ok prefix=done-percent");
    Ok(())
}

fn run(arguments: &[String]) -> Result<(), String> {
    match arguments {
        [flag] if flag == "--self-test" => self_test(),
        [message] => {
            let message = std::fs::read_to_string(message).map_err(|error| error.to_string())?;
            require_verified_index()?;
            let expected = progress_subject()?;
            if valid(&message, &expected) {
                Ok(())
            } else {
                Err(format!("commit subject must start with {expected}"))
            }
        }
        _ => Err(USAGE.into()),
    }
}

pub fn entry(arguments: &[String]) -> ExitCode {
    if let Err(error) = run(arguments) {
        eprintln!("error: {error}");
        return ExitCode::FAILURE;
    }
    ExitCode::SUCCESS
}
