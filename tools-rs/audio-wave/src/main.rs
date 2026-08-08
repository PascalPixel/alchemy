//! CLI mirror of `tools/make/audio_wave.ts`.
//!
//! `--self-test` anywhere in the arguments runs the self-test and prints
//! `self-test=ok`; anything else prints the usage line. Both exit 0. A failing
//! self-test writes the thrown message to stderr and exits 1, matching Bun's
//! behaviour for an uncaught `Error` (Bun also prints a stack trace, which the
//! Rust port omits).

use std::process::ExitCode;

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    if args.iter().any(|arg| arg == "--self-test") {
        match audio_wave::self_test() {
            Ok(()) => ExitCode::SUCCESS,
            Err(message) => {
                eprintln!("error: {message}");
                ExitCode::FAILURE
            }
        }
    } else {
        println!("usage: audio_wave.ts --self-test");
        ExitCode::SUCCESS
    }
}
