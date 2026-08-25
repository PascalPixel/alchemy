use crate::{build_early_runtime_data, default_catalog_path, Error};
use std::io::Write;
use std::process::ExitCode;
const USAGE: &str = "usage: early-runtime-data build-region-stdout SOURCE ADDRESS";
pub fn entry(arguments: &[String]) -> ExitCode {
    if arguments.len() == 1 && matches!(arguments[0].as_str(), "-h" | "--help") {
        println!("{USAGE}");
        return ExitCode::SUCCESS;
    }
    if arguments.len() != 3 || arguments[0] != "build-region-stdout" {
        eprintln!("error: {USAGE}");
        return ExitCode::FAILURE;
    }
    let address = arguments[2].strip_prefix("0x").unwrap_or(&arguments[2]);
    let address = match i64::from_str_radix(address, 16) {
        Ok(address) => address,
        Err(_) => {
            eprintln!("error: invalid address: {}", arguments[2]);
            return ExitCode::FAILURE;
        }
    };
    match build_region(&arguments[1], address) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
fn build_region(source: &str, address: i64) -> Result<(), Error> {
    let catalog = default_catalog_path();
    let built = build_early_runtime_data(source, &catalog)?;
    let region = built
        .regions
        .iter()
        .find(|(start, _)| *start == address)
        .ok_or_else(|| Error("early-runtime asset address is not a produced region".into()))?;
    eprintln!(
        "{{\"source_bytes\":{},\"region_address\":\"0x{address:08x}\"}}",
        built.source_bytes
    );
    std::io::stdout()
        .write_all(&region.1)
        .map_err(|error| Error(error.to_string()))?;
    Ok(())
}
