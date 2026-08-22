use std::process::ExitCode;

const USAGE: &str = "usage: build-stage <asm|claimed|full|rom> [args]";

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    let Some(command) = args.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let rest: Vec<String> = args[1..].to_vec();
    match command {
        "asm" => {
            build_asm::cli::entry(&rest);
            ExitCode::SUCCESS
        }
        "claimed" => {
            build_claimed::cli::entry(&rest);
            ExitCode::SUCCESS
        }
        "full" => match build_full::cli::run(&rest) {
            0 => ExitCode::SUCCESS,
            _ => ExitCode::FAILURE,
        },
        "rom" => match build_full::cli::run(&rest) {
            0 => ExitCode::SUCCESS,
            _ => ExitCode::FAILURE,
        },
        "-h" | "--help" => {
            println!("{USAGE}");
            ExitCode::SUCCESS
        }
        other => {
            eprintln!("unknown build stage: {other}\n{USAGE}");
            ExitCode::from(2)
        }
    }
}
