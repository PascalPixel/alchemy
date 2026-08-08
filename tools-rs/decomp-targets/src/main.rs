//! CLI mirror of the TS `if (import.meta.main) selfTest()` entry point.

use std::process::ExitCode;

fn main() -> ExitCode {
    match decomp_targets::self_test() {
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
