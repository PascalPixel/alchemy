use crate::{build_encounter_regions, Result};
use serde_json::json;
use std::io::Write;
use std::process::ExitCode;

fn run(args: &[String]) -> Result<()> {
    match args {
        [command, directory] if command == "list-regions" => {
            let regions = build_encounter_regions(directory)?;
            println!(
                "{}",
                serde_json::to_string(
                    &regions
                        .iter()
                        .map(|region| json!({
                            "address": region.address, "size": region.size, "source": region.source,
                        }))
                        .collect::<Vec<_>>()
                )
                .map_err(|e| e.to_string())?
            );
            Ok(())
        }
        [command, directory, address, source, size] if command == "build-region-stdout" => {
            let digits = address.strip_prefix("0x").unwrap_or(address);
            let address = usize::from_str_radix(digits, 16).map_err(|_| format!("invalid address: {address}"))?;
            let size: usize = size.parse().map_err(|_| format!("invalid size: {size}"))?;
            let region = build_encounter_regions(directory)?.into_iter().find(|region| region.address == address && region.source == *source && region.size == size).ok_or_else(|| "encounter-data region differs from manifest".to_string())?;
            eprintln!("{}", json!({"source_bytes": region.data.len()}));
            std::io::stdout().write_all(&region.data).map_err(|e| e.to_string())
        }
        _ => Err("usage: encounter-data list-regions DIRECTORY | build-region-stdout DIRECTORY ADDRESS SOURCE SIZE".into()),
    }
}

pub fn entry(arguments: &[String]) -> ExitCode {
    match run(arguments) {
        Ok(()) => ExitCode::SUCCESS,
        Err(message) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}
