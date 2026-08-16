//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

use std::io::{self, Write};
use std::path::Path;
use std::process::ExitCode;

use crate::{build_staff_roll, self_test, verify_staff_roll, Error};

const USAGE: &str = "usage: staff_roll build-stdout INDEX | verify ROM INDEX | --self-test";

fn run(args: &[String]) -> Result<(), Error> {
    match args {
        [flag] if matches!(flag.as_str(), "-h" | "--help") => {
            println!("{USAGE}");
            Ok(())
        }
        [flag] if flag == "--self-test" => {
            println!("staff_roll: self-test bytes={}", self_test()?);
            Ok(())
        }
        [command, index] if command == "build-stdout" => {
            let bytes = build_staff_roll(Path::new(index))?;
            io::stdout()
                .write_all(&bytes)
                .map_err(|e| Error(e.to_string()))?;
            Ok(())
        }
        [command, rom, index] if command == "verify" => {
            println!(
                "staff_roll: verified {} bytes",
                verify_staff_roll(Path::new(rom), Path::new(index))?
            );
            Ok(())
        }
        _ => Err(Error(USAGE.into())),
    }
}

pub fn entry(arguments: &[String]) -> std::process::ExitCode {
    match run(&arguments.to_vec()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
