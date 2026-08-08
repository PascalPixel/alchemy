//! `if (import.meta.main) await main();`

use compiler_corpus_regression::cli::{parse_arguments, ParseOutcome, USAGE};
use compiler_corpus_regression::pipeline::run;
use compiler_corpus_regression::selftest::self_test;

fn main() {
    let argv: Vec<String> = std::env::args().skip(1).collect();
    // `Bun.argv.includes("--self-test")` scans the WHOLE argv, including
    // argv[0] and argv[1], and wins over every other argument no matter where
    // it appears -- `--flags --self-test` runs the self-test.
    if argv.iter().any(|argument| argument == "--self-test") {
        match self_test() {
            Ok(message) => println!("{message}"),
            Err(message) => fail(&message),
        }
        return;
    }
    let options = match parse_arguments(&argv) {
        Ok(ParseOutcome::Help) => {
            println!("{USAGE}");
            return;
        }
        Ok(ParseOutcome::Options(options)) => options,
        Err(message) => fail(&message),
    };
    match run(&options) {
        Ok(outcome) => {
            for line in &outcome.lines {
                println!("{line}");
            }
            std::process::exit(outcome.exit_code);
        }
        Err(message) => fail(&message),
    }
}

/// PORT NOTE -- an uncaught throw in Bun prints the message to stderr with NO
/// `error: ` prefix and exits 1. Parity is asserted on the exit code and the
/// offending path in the message, never on the surrounding prose, because the
/// two runtimes format the frame differently.
fn fail(message: &str) -> ! {
    eprintln!("{message}");
    std::process::exit(1);
}
