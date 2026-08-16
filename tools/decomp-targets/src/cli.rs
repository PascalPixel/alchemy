//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

//! CLI mirror of the TS `if (import.meta.main) selfTest()` entry point.

use std::process::ExitCode;

pub fn entry(arguments: &[String]) -> std::process::ExitCode {
    match crate::self_test() {
        Ok(summary) => {
            println!("{summary}");
            ExitCode::SUCCESS
        }
        // PORT NOTE: the TS threw, which Bun surfaced on stderr with a non-zero
        // exit. We print the same message and exit 1 rather than panicking, so
        // the output stays readable in CI logs.
        Err(message) => {
            eprintln!("{message}");
            ExitCode::FAILURE
        }
    }
}
