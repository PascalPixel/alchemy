// CLI for the encounter-data extents. `verify` is the gate build_assets leans
// on: it rebuilds every region from the tracked JSON and compares it to the
// ROM, so a silent drift in the JSON cannot pass. Ported from
// tools/make/encounter_data.ts.

use std::io::Write;
use std::process::ExitCode;

use serde_json::json;

use encounter_data::{
    build_encounter_regions, export_encounter_data, matches_rom, option, self_test, Result,
};

fn run(args: &[String]) -> Result<()> {
    if let [command, directory] = args {
        if command == "list-regions" {
            let regions = build_encounter_regions(directory)?;
            println!(
                "{}",
                serde_json::to_string(
                    &regions
                        .iter()
                        .map(|region| json!({
                            "address": region.address,
                            "size": region.size,
                            "source": region.source,
                        }))
                        .collect::<Vec<_>>()
                )
                .map_err(|error| error.to_string())?
            );
            return Ok(());
        }
    }
    if let [command, directory, address, source, size] = args {
        if command == "build-region-stdout" {
            let digits = address.strip_prefix("0x").unwrap_or(address);
            let address = usize::from_str_radix(digits, 16)
                .map_err(|_| format!("invalid address: {address}"))?;
            let size: usize = size.parse().map_err(|_| format!("invalid size: {size}"))?;
            let regions = build_encounter_regions(directory)?;
            let region = regions
                .iter()
                .find(|region| {
                    region.address == address && region.source == source && region.size == size
                })
                .ok_or_else(|| "encounter-data region differs from manifest".to_string())?;
            eprintln!("{}", json!({ "source_bytes": region.data.len() }));
            std::io::stdout()
                .write_all(&region.data)
                .map_err(|error| error.to_string())?;
            return Ok(());
        }
    }
    if args.iter().any(|arg| arg == "--self-test") {
        self_test()?;
        println!("self-test=ok");
        return Ok(());
    }
    if args.iter().any(|arg| arg == "-h" || arg == "--help") {
        println!(
            "usage: encounter-data export ROM --directory DIR | verify ROM --directory DIR"
        );
        return Ok(());
    }
    let command = args.first().map(String::as_str);
    // PORT NOTE: `!args[1]` in the original is falsy for both a missing
    // argument and an empty string, so both land on the same error.
    let rom_path = args.get(1).filter(|path| !path.is_empty());
    let (Some(command @ ("export" | "verify")), Some(rom_path)) = (command, rom_path) else {
        return Err("an encounter-data command and ROM are required".to_string());
    };
    let rom = std::fs::read(rom_path).map_err(|e| format!("{rom_path}: {e}"))?;
    let directory = option(args, "--directory")?;
    let regions = if command == "export" {
        export_encounter_data(&rom, &directory)?
    } else {
        build_encounter_regions(&directory)?
    };
    for region in &regions {
        if !matches_rom(&rom, region) {
            return Err(format!("{}: verification differs", region.source));
        }
    }
    // A gate that scanned nothing would still print `identical=true`, so an
    // empty region set is a hard failure rather than a quiet pass.
    if regions.is_empty() {
        return Err("no encounter regions were verified".to_string());
    }
    let bytes: usize = regions.iter().map(|region| region.size).sum();
    println!(
        "regions={} bytes={bytes} unclaimed=8 identical=true",
        regions.len()
    );
    Ok(())
}

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    match run(&args) {
        Ok(()) => ExitCode::SUCCESS,
        Err(message) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}
