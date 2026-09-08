use std::process::ExitCode;

mod commit_progress;
mod integrate;
mod no_asm;
mod owners;
mod publication;
mod retained;

const USAGE: &str = "usage: alchemy check <publication|commit-progress|owners|retained|coverage|integrate|no-asm|progress|routes> [args]";

fn routes(arguments: &[String]) -> ExitCode {
    if arguments == ["--standard"] {
        for flag in
            compiler_core::routing::cflags_for_target(compiler_core::routing::CompilerTarget::Gs1)
        {
            println!("{flag}");
        }
        ExitCode::SUCCESS
    } else {
        eprintln!("usage: alchemy check routes --standard");
        ExitCode::from(2)
    }
}

pub fn entry(arguments: &[String]) -> ExitCode {
    let Some(command) = arguments.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let rest = &arguments[1..];
    match command {
        "publication" => publication::entry(rest),
        "commit-progress" => commit_progress::entry(rest),
        "owners" => owners::entry(rest),
        "retained" => retained::entry(rest),
        "coverage" => {
            coverage_map::entrypoint::entry(rest);
            ExitCode::SUCCESS
        }
        "integrate" => integrate::entry(rest),
        "no-asm" => no_asm::entry(rest),
        "progress" => {
            coverage_map::progress::entry(rest);
            ExitCode::SUCCESS
        }
        "routes" => routes(rest),
        "-h" | "--help" => {
            println!("{USAGE}");
            ExitCode::SUCCESS
        }
        _ => {
            eprintln!("unknown check command: {command}\n{USAGE}");
            ExitCode::from(2)
        }
    }
}
