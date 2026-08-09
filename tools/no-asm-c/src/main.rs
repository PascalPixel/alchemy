use no_asm_c::{find_forbidden, self_test, source_files, SOURCE_ROOTS};
use std::fs;
use std::path::{Path, PathBuf};
use std::process::ExitCode;

const USAGE: &str = "usage: no-asm-c [--self-test]\n\nScan C and headers for inline-assembly escape hatches.\nWith no option, scan the repository source roots.\n  -h, --help     show this help\n      --self-test validate the scanner without repository work";

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
    // The binary lives in tools/target/<profile>/, so the root is the
    // workspace's parent. CARGO_MANIFEST_DIR is tools/no-asm-c at compile
    // time, which is stable regardless of where the binary is invoked from.
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap()
        .to_path_buf()
}

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
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
    let mut files = Vec::new();
    for source_root in SOURCE_ROOTS {
        match source_files(&root.join(source_root)) {
            Ok(found) => files.extend(found),
            Err(error) => {
                eprintln!("{source_root}: {error}");
                return ExitCode::FAILURE;
            }
        }
    }
    // Scanning nothing is not passing: `forbidden=0` over an empty list reads
    // exactly like a clean run.
    if files.is_empty() {
        eprintln!("C/H assembly escape gate scanned no files — this is a FAILURE, not a pass.");
        return ExitCode::FAILURE;
    }

    let mut findings = Vec::new();
    for path in &files {
        let text = match fs::read_to_string(path) {
            Ok(text) => text,
            Err(error) => {
                eprintln!("{}: {error}", path.display());
                return ExitCode::FAILURE;
            }
        };
        let name = path
            .strip_prefix(&root)
            .unwrap_or(path)
            .to_string_lossy()
            .into_owned();
        findings.extend(find_forbidden(&name, &text));
    }
    for finding in &findings {
        eprintln!(
            "{}:{}: forbidden {} construct",
            finding.file, finding.line, finding.token
        );
    }
    println!("scanned={} forbidden={}", files.len(), findings.len());
    if findings.is_empty() {
        return ExitCode::SUCCESS;
    }
    eprintln!(
        "C/H ASSEMBLY ESCAPE — hard failure. Use ordinary C or retain the owner as a .s source."
    );
    ExitCode::FAILURE
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
