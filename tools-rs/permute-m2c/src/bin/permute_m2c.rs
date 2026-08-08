//! `bun tools/lib/permute_m2c.ts` equivalent.

use permute_m2c::{parse_arguments, pipeline, ParseOutcome, USAGE};

fn main() {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    let parsed = match parse_arguments(&arguments) {
        Ok(value) => value,
        Err(message) => {
            // PORT NOTE: the TypeScript throws, and Bun prints the message with
            // no `error: ` prefix and exits 1. Same exit code, same payload.
            eprintln!("{message}");
            std::process::exit(1);
        }
    };
    let options = match parsed {
        ParseOutcome::Help => {
            println!("{USAGE}");
            std::process::exit(0);
        }
        ParseOutcome::Parsed(options) => options,
    };
    match pipeline::run(&options) {
        Ok(outcome) => {
            for line in &outcome.lines {
                println!("{line}");
            }
        }
        Err(message) => {
            eprintln!("{message}");
            std::process::exit(1);
        }
    }
}
