use executable_gap_sources::{build_executable_gap_data, build_section, self_test, verify_rom};
use std::io::{self, Write};
use std::path::Path;
use std::process::ExitCode;

const USAGE: &str =
    "usage: executable-gap-sources build-stdout INDEX [ADDRESS] | verify ROM INDEX | --self-test";

fn parse_address(text: &str) -> Result<u64, String> {
    let digits = text
        .strip_prefix("0x")
        .ok_or_else(|| "address must be 0x-prefixed".to_string())?;
    u64::from_str_radix(digits, 16).map_err(|_| "invalid address".to_string())
}

fn run(args: &[String]) -> Result<(), String> {
    if args.len() == 1 && args[0] == "--self-test" {
        println!(
            "{}",
            self_test(Path::new("assets/data/executable_gap_plan.json"))?
        );
        return Ok(());
    }
    if args.len() == 1 && matches!(args[0].as_str(), "-h" | "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    if args.len() == 3 && args[0] == "build-stdout" {
        let bytes = build_section(Path::new(&args[1]), parse_address(&args[2])?)?;
        io::stdout().write_all(&bytes).map_err(|e| e.to_string())?;
        return Ok(());
    }
    if args.len() == 2 && args[0] == "build-stdout" {
        let sections = build_executable_gap_data(Path::new(&args[1]))?;
        if sections.len() != 1 {
            return Err(
                "build-stdout requires ADDRESS when the package has multiple typed sections".into(),
            );
        }
        io::stdout()
            .write_all(&sections[0].data)
            .map_err(|e| e.to_string())?;
        return Ok(());
    }
    if args.len() == 3 && args[0] == "verify" {
        let (sections, bytes) = verify_rom(Path::new(&args[1]), Path::new(&args[2]))?;
        println!("sections={sections} bytes={bytes} exact=true");
        return Ok(());
    }
    Err(USAGE.into())
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
