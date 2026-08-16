//! CLI moved out of src/bin so this command can be linked.

//! `if (import.meta.main) await main();`

use crate::cli::{parse_arguments, ParseOutcome, USAGE};
use crate::pipeline::run;
use crate::selftest::self_test;

pub fn entry(arguments: &[String]) {
    let argv: Vec<String> = arguments.to_vec();
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
        // --per-flag asks the narrower question: of a multi-flag owner's
        // routed flags, which ones does it not actually need?
        if options.per_flag {
            match crate::overlays::run_per_flag(root, &cache) {
                Ok(verdicts) => {
                    let mut redundant = 0usize;
                    let mut unmeasured = 0usize;
                    for (flag, verdict) in &verdicts {
                        if verdict.note.is_some() {
                            unmeasured += 1;
                            continue;
                        }
                        if !verdict.load_bearing {
                            redundant += 1;
                            println!("REDUNDANT {} {flag}", verdict.source);
                        }
                    }
                    println!(
                        "overlay per-flag: tested={} redundant={redundant} load_bearing={} unmeasured={unmeasured}",
                        verdicts.len(),
                        verdicts.len() - redundant - unmeasured
                    );
                    std::process::exit(0);
                }
                Err(message) => fail(&message),
            }
        }
        // With --flags, the question flips from "is this owner's routing
        // load-bearing" to "would promoting these flags into the overlay
        // baseline break an owner that does not carry them".
        if !options.flags.is_empty() {
            match crate::overlays::run_addition(root, &cache, &options.flags) {
                Ok(verdicts) => {
                    let mut broken = 0usize;
                    let mut unmeasured = 0usize;
                    for verdict in &verdicts {
                        if let Some(note) = &verdict.note {
                            unmeasured += 1;
                            println!("UNMEASURED {} {note}", verdict.source);
                            continue;
                        }
                        if verdict.load_bearing {
                            broken += 1;
                            println!(
                                "WOULD-BREAK {} size={}/{} first_diff={}",
                                verdict.source,
                                verdict.routed_len,
                                verdict.stripped_len,
                                verdict
                                    .first_difference
                                    .map(|v| format!("0x{v:x}"))
                                    .unwrap_or_else(|| "-".to_string()),
                            );
                        }
                    }
                    let unaffected = verdicts.len() - broken - unmeasured;
                    println!(
                        "overlay promotion of [{}]: tested={} would_break={broken} unaffected={unaffected} unmeasured={unmeasured}",
                        options.flags.join(" "),
                        verdicts.len()
                    );
                    std::process::exit(if broken == 0 { 0 } else { 1 });
                }
                Err(message) => fail(&message),
            }
        }
        match crate::overlays::run(root, &cache) {
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
