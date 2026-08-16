//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

use crate::{find_violations, scannable_files, self_test, Finding};
use std::fs;
use std::path::{Path, PathBuf};
use std::process::ExitCode;

const USAGE: &str = "usage: cache-key-lint [--self-test]\n\nScan native Rust sources for hand-maintained cache-key versions.\nWith no option, scan the repository tools tree.\n  -h, --help     show this help\n      --self-test validate the lint without repository work";

#[derive(Debug, PartialEq, Eq)]
enum Action {
    Help,
    SelfTest,
    Scan,
}

fn parse_args(args: &[String]) -> Result<Action, String> {
    match args {
        [] => Ok(Action::Scan),
        [arg] if arg == "-h" || arg == "--help" => Ok(Action::Help),
        [arg] if arg == "--self-test" => Ok(Action::SelfTest),
        _ => Err(USAGE.to_string()),
    }
}

fn repository_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap()
        .to_path_buf()
}

fn scan(root: &Path, directory: &str) -> Result<(usize, Vec<Finding>), String> {
    let base = root.join(directory);
    let names = scannable_files(&base).map_err(|error| format!("{directory}: {error}"))?;
    let mut findings = Vec::new();
    for name in &names {
        let text = fs::read_to_string(base.join(name))
            .map_err(|error| format!("{directory}/{name}: {error}"))?;
        findings.extend(find_violations(&format!("{directory}/{name}"), &text));
    }
    Ok((names.len(), findings))
}

pub fn entry(arguments: &[String]) -> std::process::ExitCode {
    let args: Vec<String> = arguments.to_vec();
    let action = match parse_args(&args) {
        Ok(action) => action,
        Err(message) => {
            eprintln!("{message}");
            return ExitCode::FAILURE;
        }
    };
    match action {
        Action::Help => {
            println!("{USAGE}");
            ExitCode::SUCCESS
        }
        Action::SelfTest => match self_test() {
            Ok(line) => {
                println!("{line}");
                ExitCode::SUCCESS
            }
            Err(message) => {
                eprintln!("{message}");
                ExitCode::FAILURE
            }
        },
        Action::Scan => scan_repository(),
    }
}

fn scan_repository() -> ExitCode {
    let root = repository_root();
    let mut scanned = 0;
    let mut findings = Vec::new();
    match scan(&root, "tools") {
        Ok((count, found)) => {
            scanned += count;
            findings.extend(found);
        }
        Err(message) => {
            eprintln!("{message}");
            return ExitCode::FAILURE;
        }
    }

    for finding in &findings {
        println!("{}:{}  {}", finding.file, finding.line, finding.text);
    }
    println!("\nscanned={scanned} violations={}", findings.len());

    // SCANNING NOTHING IS NOT PASSING. `violations=0` over an empty file list
    // reads exactly like a clean run, and a lint that can pass without looking
    // is the defect it polices.
    if scanned == 0 {
        println!(
            "NOTHING SCANNED — this is a FAILURE, not a pass.\n  No Rust sources under {}.",
            root.display()
        );
        return ExitCode::FAILURE;
    }
    if !findings.is_empty() {
        println!(
            "HAND-MAINTAINED CACHE KEY — this is a FAILURE.\n\
             \x20 A cache key must derive from its inputs, not from a literal describing\n\
             \x20 the logic. Mix in a digest of the tool's own source instead:\n\
             \x20     hasher.update(format!(\"name:{{}}:...\", self_digest()).as_bytes());\n\
             \x20 See the native overlay-disasm crate and HANDOVER §5h for why."
        );
        return ExitCode::FAILURE;
    }
    ExitCode::SUCCESS
}

#[cfg(test)]
mod tests {
    use super::*;

    fn args(values: &[&str]) -> Vec<String> {
        values.iter().map(|value| (*value).to_string()).collect()
    }

    #[test]
    fn cli_contract_has_help_and_rejects_unknown_options() {
        assert_eq!(parse_args(&args(&[])), Ok(Action::Scan));
        assert_eq!(parse_args(&args(&["-h"])), Ok(Action::Help));
        assert_eq!(parse_args(&args(&["--help"])), Ok(Action::Help));
        assert_eq!(parse_args(&args(&["--self-test"])), Ok(Action::SelfTest));
        assert!(parse_args(&args(&["--unknown"])).is_err());
    }
}
