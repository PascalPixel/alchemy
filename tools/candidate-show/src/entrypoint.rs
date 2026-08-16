//! CLI for this crate, moved out of src/bin so the command can be linked
//! into a shared entry point. Named `entrypoint` because this crate already
//! owns a `cli` module.

//! `candidate_show.ts`'s `if (import.meta.main) await main()`.

use alchemy_routing::routing::root;
use crate::cli::{options_of, ParseOutcome, USAGE};
use crate::diff::self_test;
use crate::render::render;

pub fn entry(arguments: &[String]) {
    let arguments: Vec<String> = arguments.to_vec();
    // `--self-test` is checked before `optionsOf`, so it wins over an
    // otherwise invalid argv.
    if arguments.iter().any(|argument| argument == "--self-test") {
        match self_test() {
            Ok(line) => println!("{line}"),
            Err(error) => fail(&error),
        }
        return;
    }
    let root = root();
    match options_of(root, &arguments) {
        Ok(ParseOutcome::Help) => println!("{USAGE}"),
        Ok(ParseOutcome::Options(options)) => match render(root, &options) {
            Ok(output) => print!("{}", output.stdout),
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
