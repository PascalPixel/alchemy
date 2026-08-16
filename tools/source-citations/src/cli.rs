//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

//! Port of the `main()` in `tools/check/source_citations.ts`.
//!
//!   source-citations
//!   source-citations --self-test

use std::path::{Path, PathBuf};
use std::process::ExitCode;

use crate::{check, self_test};

const USAGE: &str = "Usage: source-citations [--self-test]\n\nModes:\n  (default)      Validate clean-room source citations and retirements.\n  --self-test    Run the citation check's internal checks.\n  -h, --help     Show this help.";

#[derive(Debug, PartialEq, Eq)]
enum Command {
    Help,
    Check,
    SelfTest,
}

fn parse_args(args: &[String]) -> Result<Command, &'static str> {
    match args {
        [] => Ok(Command::Check),
        [argument] if argument == "--self-test" => Ok(Command::SelfTest),
        [argument] if argument == "-h" || argument == "--help" => Ok(Command::Help),
        _ => Err(USAGE),
    }
}

/// The TypeScript resolves ROOT from import.meta.url; the binary resolves it
/// from its own manifest, which sits two levels below the repository root.
fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("crate lives two levels below the repository root")
        .to_path_buf()
}

pub fn entry(arguments: &[String]) -> std::process::ExitCode {
    let root = root();
    let args: Vec<String> = arguments.to_vec();
    let command = match parse_args(&args) {
        Ok(command) => command,
        Err(message) => {
            eprintln!("error: {message}");
            return ExitCode::FAILURE;
        }
    };
    if command == Command::Help {
        println!("{USAGE}");
        return ExitCode::SUCCESS;
    }
    if command == Command::SelfTest {
        return match self_test(&root) {
            Ok(()) => {
                println!("source_citations self-test ok");
                ExitCode::SUCCESS
            }
            Err(problem) => {
                eprintln!("error: {problem}");
                ExitCode::FAILURE
            }
        };
    }
    match check(&root) {
        Ok(summary) => {
            println!("{summary}");
            ExitCode::SUCCESS
        }
        Err(problems) => {
            for problem in &problems {
                eprintln!("error: {problem}");
            }
            ExitCode::FAILURE
        }
    }
}

#[cfg(test)]
mod parser_tests {
    use super::{parse_args, Command};

    fn args(items: &[&str]) -> Vec<String> {
        items.iter().map(|item| (*item).to_string()).collect()
    }

    #[test]
    fn preserves_default_and_self_test_modes() {
        assert_eq!(parse_args(&args(&[])), Ok(Command::Check));
        assert_eq!(parse_args(&args(&["--self-test"])), Ok(Command::SelfTest));
        assert_eq!(parse_args(&args(&["--help"])), Ok(Command::Help));
    }

    #[test]
    fn rejects_unknown_options() {
        assert!(parse_args(&args(&["--bogus"])).is_err());
        assert!(parse_args(&args(&["--self-test", "--bogus"])).is_err());
    }
}
