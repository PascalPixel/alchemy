//! CLI for this crate, moved out of src/bin so the command can be linked
//! into a shared entry point. Named `entrypoint` because this crate already
//! owns a `cli` module.

//! `bun tools/lib/candidate_explain.ts` -- the CLI entry point.

use std::io::Write;

use crate::cli::{parse_arguments, Options, ParseOutcome, USAGE};
use crate::run::{run_main_image, run_overlay};
use crate::self_test;

pub fn entry(arguments: &[String]) {
    // Must consume the PASSED slice: as a `compiler` subcommand, env::args()
    // still carries the host binary and the subcommand name.
    let argv: Vec<String> = arguments.to_vec();

    // `Bun.argv.includes("--self-test")` scans the WHOLE argv, including
    // argv[0] and argv[1], and runs before any option parsing.
    if argv.iter().any(|argument| argument == "--self-test") {
        match self_test() {
            Ok(line) => {
                println!("{line}");
                return;
            }
            Err(message) => fail(&message),
        }
    }

    let cwd = std::env::current_dir().unwrap_or_else(|error| fail(&error.to_string()));
    let options = match parse_arguments(&argv, &cwd) {
        ParseOutcome::Help => {
            println!("{USAGE}");
            return;
        }
        ParseOutcome::Error(message) => fail(&message),
        ParseOutcome::Parsed(options) => *options,
    };

    let work = match &options {
        Options::Overlay { work, .. } | Options::Main { work, .. } => work.clone(),
    };
    if let Err(error) = std::fs::create_dir_all(&work) {
        fail(&format!("{}: {error}", work.display()));
    }

    let outcome = match &options {
        Options::Overlay { .. } => run_overlay(&options),
        Options::Main { source, work } => run_main_image(source, work),
    };
    match outcome {
        Ok(text) => {
            let mut stdout = std::io::stdout().lock();
            let _ = stdout.write_all(text.as_bytes());
            let _ = stdout.flush();
        }
        Err(message) => fail(&message),
    }
}

/// A thrown `Error` in Bun prints a stack trace and exits 1. Only the exit code
/// and the fact of failure are contracted; the prose is not, so the message is
/// written plainly to stderr with no `error: ` prefix (Bun does not emit one
/// either).
fn fail(message: &str) -> ! {
    eprintln!("{message}");
    std::process::exit(1);
}
