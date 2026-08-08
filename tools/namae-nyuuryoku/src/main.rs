use namae_nyuuryoku::{
    build_namae_nyuuryoku, self_test, verify_namae_nyuuryoku, Error, ADDRESS, SIZE,
};
use std::io::{self, Write};
use std::path::Path;
use std::process::ExitCode;

const USAGE: &str = "usage: namae_nyuuryoku build SOURCE --output FILE | build-stdout SOURCE | verify ROM SOURCE | --self-test";

fn run(args: &[String]) -> Result<(), Error> {
    match args {
        [flag] if flag == "--self-test" => {
            self_test()?;
            println!("self-test=ok");
            Ok(())
        }
        [flag] if matches!(flag.as_str(), "--help" | "-h") => {
            println!("{USAGE}");
            Ok(())
        }
        [command, source, flag, output] if command == "build" && flag == "--output" => {
            let bytes = build_namae_nyuuryoku(Path::new(source))?;
            if let Some(parent) = Path::new(output).parent() {
                std::fs::create_dir_all(parent).map_err(|e| Error(e.to_string()))?;
            }
            std::fs::write(output, bytes).map_err(|e| Error(e.to_string()))?;
            println!("address=0x{ADDRESS:08x} bytes={SIZE}");
            Ok(())
        }
        [command, source] if command == "build-stdout" => {
            io::stdout()
                .write_all(&build_namae_nyuuryoku(Path::new(source))?)
                .map_err(|e| Error(e.to_string()))?;
            Ok(())
        }
        [command, rom, source] if command == "verify" => {
            verify_namae_nyuuryoku(
                &std::fs::read(rom).map_err(|e| Error(e.to_string()))?,
                Path::new(source),
            )?;
            println!("identical=true source_bytes={SIZE}");
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
