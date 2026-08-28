use std::process::ExitCode;

mod aggregate_context;
mod cross_edition;
mod families;
mod family_m2c;
mod waves;

const USAGE: &str =
    "usage: compiler <candidate-show|cross-edition|families|permute|waves|dashboard-server> [args]";

fn main() -> ExitCode {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    let Some(command) = arguments.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let rest = &arguments[1..];
    match command {
        "candidate-show" => {
            candidate_show::entrypoint::entry(rest);
            ExitCode::SUCCESS
        }
        "cross-edition" => result(cross_edition::run(rest)),
        "families" => result(families::run(rest)),
        "waves" => result(waves::run(rest)),
        "permute" => match permuter::run(rest.to_vec()) {
            Ok(()) => ExitCode::SUCCESS,
            Err(error) => {
                eprintln!("error: {error}");
                ExitCode::FAILURE
            }
        },
        "dashboard-server" => {
            dashboard_server::cli::entry(rest);
            ExitCode::SUCCESS
        }
        "-h" | "--help" => {
            println!("{USAGE}");
            ExitCode::SUCCESS
        }
        _ => {
            eprintln!("unknown compiler command: {command}\n{USAGE}");
            ExitCode::from(2)
        }
    }
}

fn result(value: Result<(), String>) -> ExitCode {
    match value {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
