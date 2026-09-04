use std::process::ExitCode;

const USAGE: &str = "usage: check <publication|commit-progress|owners|retained|coverage|integrate|no-asm|progress|routes> [args]";

fn routes(arguments: &[String]) -> ExitCode {
    if arguments == ["--standard"] {
        for flag in
            compiler_core::routing::cflags_for_target(compiler_core::routing::CompilerTarget::Gs1)
        {
            println!("{flag}");
        }
        ExitCode::SUCCESS
    } else {
        eprintln!("usage: check routes --standard");
        ExitCode::from(2)
    }
}

fn main() -> ExitCode {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    let Some(command) = arguments.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let rest = &arguments[1..];
    match command {
        "publication" | "check-publication" => check_publication::cli::entry(rest),
        "commit-progress" | "check-commit-progress" => check_commit_progress::cli::entry(rest),
        "owners" | "check-unmatchable" => check_unmatchable::cli::entry(rest),
        "retained" | "core-retained-audit" => core_retained_audit::cli::entry(rest),
        "coverage" | "coverage-map" => {
            coverage_map::entrypoint::entry(rest);
            ExitCode::SUCCESS
        }
        "integrate" | "integrate-matches" => integrate_matches::entry(rest),
        "retire" => integrate_matches::retire::entry(rest),
        "no-asm" | "no-asm-c" => no_asm_c::cli::entry(rest),
        "progress" => {
            full_c_progress::entry(rest);
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
