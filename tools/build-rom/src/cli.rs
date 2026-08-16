//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

use crate::{main_pipeline, parse_args, self_test, ParseOutcome, USAGE};

pub fn entry(arguments: &[String]) {
    let argv: Vec<String> = arguments.to_vec();
    if argv.as_slice() == ["--self-test"] {
        match self_test() {
            Ok(line) => println!("{line}"),
            Err(message) => fail(&message),
        }
        return;
    }
    let options = match parse_args(&argv) {
        Ok(ParseOutcome::Help) => {
            println!("{USAGE}");
            std::process::exit(0);
        }
        Ok(ParseOutcome::Options(options)) => options,
        Err(message) => fail(&message),
    };
    let cwd = match std::env::current_dir() {
        Ok(cwd) => cwd,
        Err(error) => fail(&format!("cwd: {error}")),
    };
    match main_pipeline(&options, &cwd) {
        Ok(line) => println!("{line}"),
        Err(message) => fail(&message),
    }
}

// PORT NOTE: Bun prints a stack trace for an uncaught `throw` and exits 1.
// The port prints `error: <message>` on stderr and exits 1; the message text
// itself is identical.
fn fail(message: &str) -> ! {
    eprintln!("error: {message}");
    std::process::exit(1);
}
