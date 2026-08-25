//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

use crate::{build_sentou_gamen_data, self_test, verify_sentou_gamen_data, Error, SIZE};
use std::io::Write;
use std::path::Path;
use std::process::ExitCode;

const USAGE: &str = "usage: sentou_gamen_data build INDEX --output FILE | verify ROM INDEX | --self-test";

fn run(args: &[String]) -> Result<(), Error> {
    match args {
        [x] if x == "--self-test" => {
            self_test()?;
            println!("self-test=ok");
            Ok(())
        }
        [cmd, index, flag, output] if cmd == "build" && flag == "--output" => {
            let (bytes, _) = build_sentou_gamen_data(Path::new(index))?;
            std::fs::write(output, bytes).map_err(|e| Error(e.to_string()))?;
            println!("source_bytes={SIZE}");
            Ok(())
        }
        [cmd, index] if cmd == "build-stdout" => {
            let (bytes, _) = build_sentou_gamen_data(Path::new(index))?;
            std::io::stdout().write_all(&bytes).map_err(|e| Error(e.to_string()))?;
            Ok(())
        }
        [cmd, rom, index] if cmd == "verify" => {
            let data = std::fs::read(rom).map_err(|e| Error(e.to_string()))?;
            verify_sentou_gamen_data(&data, Path::new(index))?;
            println!("identical=true source_bytes={SIZE}");
            Ok(())
        }
        [x] if x == "--help" || x == "-h" => {
            println!("{USAGE}");
            Ok(())
        }
        _ => Err(Error(USAGE.into())),
    }
}

pub fn entry(arguments: &[String]) -> std::process::ExitCode {
    match run(&arguments.to_vec()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(e) => {
            let _ = writeln!(std::io::stderr(), "error: {e}");
            ExitCode::FAILURE
        }
    }
}
