use std::process::ExitCode;

fn main() -> ExitCode {
    match audio_engine_data::run(std::env::args().skip(1).collect()) {
        Ok(Some(line)) => {
            println!("{line}");
            ExitCode::SUCCESS
        }
        Ok(None) => ExitCode::SUCCESS,
        Err(message) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}
