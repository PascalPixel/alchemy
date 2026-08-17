//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

//! CLI mirror of the TS `if (import.meta.main) selfTest()` entry point.

use std::process::ExitCode;

pub fn entry(arguments: &[String]) -> std::process::ExitCode {
    // This command runs one thing. Silently ignoring an argument is worse than
    // refusing it: `decomp-targets --json` would print the self-test and exit
    // 0, reading as though the flag had been honoured.
    for argument in arguments {
        if argument != "--self-test" {
            eprintln!("usage: decomp-targets [--self-test]");
            eprintln!("unrecognised argument: {argument}");
            return ExitCode::FAILURE;
        }
    }
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
