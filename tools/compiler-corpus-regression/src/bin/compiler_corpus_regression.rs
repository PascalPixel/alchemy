//! `if (import.meta.main) await main();`

use compiler_corpus_regression::cli::{parse_arguments, ParseOutcome, USAGE};
use compiler_corpus_regression::pipeline::run;
use compiler_corpus_regression::selftest::self_test;

fn main() {
    let argv: Vec<String> = std::env::args().skip(1).collect();
    // `native process.argv.includes("--self-test")` scans the WHOLE argv, including
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
    // Native addition, ahead of the ported pipeline because it answers a
    // different question against a different corpus. See `crate::overlays`.
    if options.overlays {
        let root = alchemy_routing::routing::root();
        let cache = std::path::Path::new(&options.cache).join("overlays");
        match compiler_corpus_regression::overlays::run(root, &cache) {
            Ok(verdicts) => {
                let mut load_bearing = 0usize;
                let mut unmeasured = 0usize;
                for verdict in &verdicts {
                    if let Some(note) = &verdict.note {
                        unmeasured += 1;
                        println!("UNMEASURED {} {note}", verdict.source);
                        continue;
                    }
                    if verdict.load_bearing {
                        load_bearing += 1;
                        println!(
                            "LOAD-BEARING {} size={}/{} first_diff={} flags={}",
                            verdict.source,
                            verdict.routed_len,
                            verdict.stripped_len,
                            verdict
                                .first_difference
                                .map(|v| format!("0x{v:x}"))
                                .unwrap_or_else(|| "-".to_string()),
                            verdict.extras.join(" ")
                        );
                    }
                }
                let removable = verdicts.len() - load_bearing - unmeasured;
                println!(
                    "overlay routing: owners={} load_bearing={load_bearing} removable={removable} unmeasured={unmeasured}",
                    verdicts.len()
                );
                std::process::exit(0);
            }
            Err(message) => fail(&message),
        }
    }
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

/// PORT NOTE -- an uncaught throw in native process prints the message to stderr with NO
/// `error: ` prefix and exits 1. differential comparison is asserted on the exit code and the
/// offending path in the message, never on the surrounding prose, because the
/// two runtimes format the frame differently.
fn fail(message: &str) -> ! {
    eprintln!("{message}");
    std::process::exit(1);
}
