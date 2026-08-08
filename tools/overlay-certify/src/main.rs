use std::env;
use std::process::ExitCode;

fn main() -> ExitCode {
    match overlay_certify::run(&env::args().skip(1).collect::<Vec<_>>()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(message) => {
            eprintln!("{message}");
            ExitCode::FAILURE
        }
    }
}
