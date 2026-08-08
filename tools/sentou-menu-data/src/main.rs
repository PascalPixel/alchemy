use sentou_menu_data::{build_sentou_menu_data, self_test, verify_sentou_menu_data, Error, ADDRESS, SIZE};
use std::io::{self, Write};
use std::path::Path;
use std::process::ExitCode;

const USAGE: &str = "usage: sentou_menu_data build INDEX --output FILE | build-stdout INDEX | verify ROM INDEX | --self-test";

fn run(args: &[String]) -> Result<(), Error> {
    match args {
        [flag] if flag == "--self-test" => { self_test()?; println!("self-test=ok"); Ok(()) }
        [flag] if matches!(flag.as_str(), "--help" | "-h") => { println!("{USAGE}"); Ok(()) }
        [command, index, flag, output] if command == "build" && flag == "--output" => {
            let bytes = build_sentou_menu_data(Path::new(index))?;
            if let Some(parent) = Path::new(output).parent() { std::fs::create_dir_all(parent).map_err(|e| Error(e.to_string()))?; }
            std::fs::write(output, bytes).map_err(|e| Error(e.to_string()))?;
            println!("address=0x{ADDRESS:08x} bytes={SIZE}");
            Ok(())
        }
        [command, index] if command == "build-stdout" => {
            let bytes = build_sentou_menu_data(Path::new(index))?;
            io::stdout().write_all(&bytes).map_err(|e| Error(e.to_string()))?;
            Ok(())
        }
        [command, rom, index] if command == "verify" => {
            let rom = std::fs::read(rom).map_err(|e| Error(e.to_string()))?;
            verify_sentou_menu_data(&rom, Path::new(index))?;
            println!("identical=true source_bytes={SIZE}");
            Ok(())
        }
        _ => Err(Error(USAGE.into())),
    }
}

fn main() -> ExitCode {
    match run(&std::env::args().skip(1).collect::<Vec<_>>()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => { eprintln!("error: {error}"); ExitCode::FAILURE }
    }
}
