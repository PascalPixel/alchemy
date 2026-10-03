use std::process::ExitCode;

mod commit_progress;
mod layout;
mod no_asm;
mod publication;
#[cfg(test)]
pub(crate) use publication::figure_test_reason;

const USAGE: &str =
    "usage: alchemy check <publication|commit-progress|coverage|layout|no-asm|progress|routes> [args]";

/// Run a check body and report its error the way every check does.
fn report(result: Result<(), String>) -> ExitCode {
    match result {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}

fn routes(arguments: &[String]) -> ExitCode {
    if arguments == ["--standard"] {
        for flag in crate::compiler::routing::cflags_for_target(
            crate::compiler::routing::CompilerTarget::Tbs,
        ) {
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
        "layout" => layout::entry(rest),
        "coverage" => {
            crate::coverage::entry(rest);
            ExitCode::SUCCESS
        }
        "no-asm" => no_asm::entry(rest),
        "progress" => {
            crate::coverage::progress::entry(rest);
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
