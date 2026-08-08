use std::process::ExitCode;

use overlay_show::{run, Outcome};

fn main() -> ExitCode {
    let argv: Vec<String> = std::env::args().skip(1).collect();
    match run(&argv) {
        Ok(Outcome::Usage) => ExitCode::SUCCESS,
        Ok(Outcome::SelfTest(message)) => {
            println!("{message}");
            ExitCode::SUCCESS
        }
        Ok(Outcome::Lines(lines)) => {
            for line in lines {
                println!("{line}");
            }
            ExitCode::SUCCESS
        }
        Err(message) => {
            eprintln!("{message}");
            ExitCode::FAILURE
        }
    }
}
