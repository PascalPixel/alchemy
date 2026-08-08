use map_resources::{run, SeriesKind};
use std::process::ExitCode;

fn main() -> ExitCode {
    match run(std::env::args().skip(1).collect(), SeriesKind::Chiiki) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
