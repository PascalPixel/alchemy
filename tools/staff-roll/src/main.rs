use std::io::{self, Write};
use std::path::Path;
use std::process::ExitCode;

use staff_roll::{build_staff_roll, self_test, verify_staff_roll, Error};

const USAGE: &str = "usage: staff_roll build-stdout INDEX | verify ROM INDEX | --self-test";

fn run(args: &[String]) -> Result<(), Error> {
    match args {
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

fn main() -> ExitCode {
    match run(&std::env::args().skip(1).collect::<Vec<_>>()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
