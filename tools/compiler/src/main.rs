use std::process::ExitCode;

mod aggregate_mine;
mod cross_edition;
mod families;
mod twins;
mod waves;
mod workbench;

const USAGE: &str = "usage: compiler <aggregate-mine|candidate-show|cross-edition|families|permute|twins|waves|workbench|dashboard-server> [args]";

fn main() -> ExitCode {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    let Some(command) = arguments.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let rest = &arguments[1..];
    match command {
        "aggregate-mine" => result(aggregate_mine::run(rest)),
        "candidate-show" => {
            candidate_show::entrypoint::entry(rest);
            ExitCode::SUCCESS
        }
        "cross-edition" => result(cross_edition::run(rest)),
        "twins" => result(twins::run(rest)),
        "families" => result(families::run(rest)),
        "waves" => result(waves::run(rest)),
        "permute" => match permuter::run(rest.to_vec()) {
            Ok(()) => ExitCode::SUCCESS,
            Err(error) => {
                eprintln!("error: {error}");
                ExitCode::FAILURE
            }
        },
        "workbench" => result(workbench::run(rest)),
        "__workbench-step" => result(workbench::run_step(rest)),
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
