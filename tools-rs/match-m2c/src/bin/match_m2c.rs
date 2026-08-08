//! `match_m2c.ts`'s `if (import.meta.main) await main()`.

use match_m2c::cli::{parse_arguments, ParseOutcome, USAGE};
use match_m2c::pipeline::run_pipeline;

fn main() {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    match parse_arguments(&arguments) {
        Ok(ParseOutcome::Help) => {
            println!("{USAGE}");
        }
        Ok(ParseOutcome::Options(options)) => match run_pipeline(&options) {
            Ok(summary) => println!("{summary}"),
            Err(error) => fail(&error),
        },
        Err(error) => fail(&error),
    }
}

/// Bun prints an uncaught throw to stderr and exits 1. The prose differs (Bun
/// has no `error: ` prefix and prints a stack); the exit code and the offending
/// value do not, and those are what the parity harness compares.
fn fail(message: &str) -> ! {
    eprintln!("{message}");
    std::process::exit(1);
}
