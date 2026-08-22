//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

use crate::{build, parse_args, repository_root, ParseOutcome};

pub fn run(arguments: &[String]) -> i32 {
    if arguments == ["--self-test"] {
        crate::self_test().unwrap_or_else(|error| fail(&error));
        println!("self-test=ok");
        return 0;
    }
    let options = match parse_args(arguments) {
        Ok(ParseOutcome::Help) => {
            println!("usage: build-full [-h] [--target GAME-EDITION] [-o OUTPUT] [--claimed-output CLAIMED_OUTPUT] [--asm-output ASM_OUTPUT] [--asset-manifest ASSET_MANIFEST] [--asset-output ASSET_OUTPUT] [--jobs JOBS] [--source-only] [rom] | --self-test");
            return 0;
        }
        Ok(ParseOutcome::Run(options)) => options,
        Err(error) => fail(&error),
    };
    let cwd = std::env::current_dir().unwrap_or_else(|error| fail(&format!("cwd: {error}")));
    let summary = build(&repository_root(), &cwd, &options).unwrap_or_else(|error| fail(&error));
    println!("{summary}");
    0
}

fn fail(message: &str) -> ! {
    eprintln!("error: {message}");
    std::process::exit(1);
}
