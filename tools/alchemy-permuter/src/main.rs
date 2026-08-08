use std::process::ExitCode;

fn main() -> ExitCode {
    match alchemy_permuter::run(std::env::args().skip(1).collect()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("{error}");
            ExitCode::from(1)
        }
    }
}
