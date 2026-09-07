use compiler_core::routing::root;
use std::process::ExitCode;

const USAGE: &str = "usage: alchemy overlay <adopt|park|audit|score|disasm> [args]";

fn code(result: Result<i32, String>) -> ExitCode {
    match result {
        Ok(0) => ExitCode::SUCCESS,
        Ok(_) => ExitCode::FAILURE,
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
    }
}

pub fn entry(arguments: &[String]) -> ExitCode {
    let Some(command) = arguments.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let rest = &arguments[1..];
    match command {
        "adopt" => code(overlay_adopt::run(root(), rest)),
        "park" => code(overlay_adopt::park::run(root(), rest)),
        "audit" => code(overlay_adopt::park::run_audit(root(), rest)),
        "score" => code(overlay_adopt::score::run(root(), rest)),
        "disasm" => disassemble::cli::entry(rest),
        "-h" | "--help" => {
            println!("{USAGE}");
            ExitCode::SUCCESS
        }
        _ => {
            eprintln!("unknown overlay command: {command}\n{USAGE}");
            ExitCode::from(2)
        }
    }
}
