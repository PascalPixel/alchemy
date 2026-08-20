use std::process::ExitCode;

const USAGE: &str = "usage: compiler <candidate-show|permute|dashboard-server> [args]";

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
