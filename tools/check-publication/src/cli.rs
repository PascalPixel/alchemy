//! Command-line boundary for the publication gate.

use std::{io::Read, path::Path, process::ExitCode};

const USAGE: &str = "Usage: check-publication [--staged | --pre-push | --self-test]\n\nModes:\n  --staged       Check staged files before committing.\n  --pre-push     Check outgoing history using update lines on stdin.\n  --self-test    Run the publication gate's internal checks.\n  -h, --help     Show this help.";

fn root() -> &'static Path {
    Path::new(concat!(env!("CARGO_MANIFEST_DIR"), "/../.."))
}

fn fail(message: &str) -> ExitCode {
    eprintln!("error: {message}");
    ExitCode::FAILURE
}

pub fn entry(arguments: &[String]) -> ExitCode {
    match arguments {
        [argument] if argument == "-h" || argument == "--help" => {
            println!("{USAGE}");
            ExitCode::SUCCESS
        }
        [argument] if argument == "--staged" => {
            crate::check_staged(root()).map_or_else(|error| fail(&error), |_| ExitCode::SUCCESS)
        }
        [argument] if argument == "--pre-push" => {
            let mut updates = String::new();
            if let Err(error) = std::io::stdin().read_to_string(&mut updates) {
                return fail(&format!("pre-push stdin failed: {error}"));
            }
            crate::check_push(root(), &updates)
                .map_or_else(|error| fail(&error), |_| ExitCode::SUCCESS)
        }
        [argument] if argument == "--self-test" => crate::self_test().map_or_else(
            |error| fail(&error),
            |_| {
                println!("self-test=ok");
                ExitCode::SUCCESS
            },
        ),
        _ => fail(USAGE),
    }
}
