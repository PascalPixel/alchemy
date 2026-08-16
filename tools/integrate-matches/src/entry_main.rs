//! CLI moved out of src/bin so this command can be linked.

//! `integrate-matches`'s `main()`.
//!
//! PORT NOTE -- the legacy implementation lets an `Error` escape `main`, so native process prints its
//! own uncaught-exception report and exits 1. native process's report has no `error: `
//! prefix and includes a stack trace; matching that prose is neither possible
//! nor useful, so this prints the message to stderr and exits 1. The differential comparison
//! harness compares exit status and the stdout payload, which is the artifact.

use std::process::ExitCode;

use crate::cli::{parse_arguments, ParseOutcome, USAGE};
use crate::pipeline::run_pipeline;
use crate::selftest::self_test;

pub fn entry(arguments: &[String]) -> ExitCode {
    let arguments: Vec<String> = arguments.to_vec();
    match parse_arguments(&arguments) {
        Err(message) => {
            eprintln!("{message}");
            ExitCode::FAILURE
        }
        Ok(ParseOutcome::Help) => {
            println!("{USAGE}");
            ExitCode::SUCCESS
        }
        Ok(ParseOutcome::Parsed(options)) => {
            let result = if options.self_test {
                self_test().map(|line| vec![line])
            } else {
                run_pipeline(&options.directory, options.apply).map(|report| report.lines)
            };
            match result {
                Ok(lines) => {
                    for line in lines {
                        println!("{line}");
                    }
                    ExitCode::SUCCESS
                }
                Err(message) => {
                    eprintln!("{message}");
                    ExitCode::FAILURE
                }
            }
        }
    }
}
