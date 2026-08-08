//! `bun tools/overlay/overlay_mode_cohort.ts` in Rust.
//!
//! PORT NOTE: the TypeScript checks `Bun.argv.includes("--self-test")` against
//! the WHOLE argv, including `bun` and the script path, and then parses
//! `Bun.argv.slice(2)`. The Rust mirror hands `run` everything after the
//! executable, which is the same slice, and `run` does the same whole-slice
//! `--self-test` scan.

use std::process::ExitCode;

fn main() -> ExitCode {
    let argv: Vec<String> = std::env::args().skip(1).collect();
    match overlay_mode_cohort::run(&argv) {
        Ok(lines) => {
            for line in lines {
                println!("{line}");
            }
            ExitCode::SUCCESS
        }
        // A thrown Error in Bun goes to stderr with a source frame and exits 1.
        // The frame is runtime decoration and will differ; the exit code and the
        // message text are what the parity harness asserts.
        Err(message) => {
            eprintln!("{message}");
            ExitCode::FAILURE
        }
    }
}
