use std::path::{Path, PathBuf};
use std::process::ExitCode;

const USAGE: &str = "usage: overlay <adopt|park|audit|score|show|reconstruct|twins|disasm> [args]";

fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("overlay is under tools")
        .to_path_buf()
}

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

fn lines(result: Result<Vec<String>, String>) -> ExitCode {
    match result {
        Ok(lines) => {
            for line in lines {
                println!("{line}");
            }
            ExitCode::SUCCESS
        }
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
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
        "adopt" => code(overlay_adopt::run(&root(), rest)),
        "park" => code(overlay_adopt::park::run(&root(), rest)),
        "audit" => code(overlay_adopt::park::run_audit(&root(), rest)),
        "score" => code(overlay_adopt::score::run(&root(), rest)),
        "disasm" => overlay_disasm::cli::entry(rest),
        "reconstruct" => lines(overlay_show::reconstruct::run(rest)),
        "twins" => code(overlay_adopt::twins::run(&root(), rest)),
        "show" => match overlay_show::run(rest) {
            Ok(overlay_show::Outcome::Usage) => ExitCode::SUCCESS,
            Ok(overlay_show::Outcome::SelfTest(message)) => {
                println!("{message}");
                ExitCode::SUCCESS
            }
            Ok(overlay_show::Outcome::Lines(lines)) => {
                for line in lines {
                    println!("{line}");
                }
                ExitCode::SUCCESS
            }
            Err(error) => {
                eprintln!("{error}");
                ExitCode::FAILURE
            }
        },
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
