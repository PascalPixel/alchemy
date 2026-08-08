//! CLI entrypoint: port of `main()` in `tools/lib/verify.ts`.

use std::fs;
use std::process::ExitCode;

use verify::routing::root;
use verify::targets::{decomp_target, DEFAULT_TARGET};
use verify::{address_sources, verify, Result};

fn run() -> Result<bool> {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    if arguments.iter().any(|a| a == "-h" || a == "--help") {
        println!("usage: verify [-h] [--target gs1-en|gs2-en]");
        return Ok(true);
    }
    let mut target_id = DEFAULT_TARGET.to_string();
    let mut index = 0;
    while index < arguments.len() {
        let argument = &arguments[index];
        if argument == "--target" {
            index += 1;
            let value = arguments
                .get(index)
                .ok_or_else(|| "--target requires a value".to_string())?;
            target_id = verify::targets::parse_decomp_target(value)?.to_string();
        } else if let Some(value) = argument.strip_prefix("--target=") {
            target_id = verify::targets::parse_decomp_target(value)?.to_string();
        } else {
            return Err(format!("unrecognized argument: {argument}"));
        }
        index += 1;
    }

    let target = decomp_target(&target_id)?;
    let rom_path = root().join(target.rom);
    let rom = fs::read(&rom_path)
        .map_err(|error| format!("cannot read {}: {error}", rom_path.display()))?;
    verify::require_rom_size(rom.len(), target.rom_size, target.id)?;
    let output_dir = root().join(target.output_dir).join("verify");
    fs::create_dir_all(&output_dir)
        .map_err(|error| format!("cannot create {}: {error}", output_dir.display()))?;
    let output_dir = output_dir.to_string_lossy().into_owned();

    // verify() parses the filename itself as the absolute ROM address, so only
    // main-image translation units are eligible.
    let source_directory = root().join(target.source_dir);
    let sources = address_sources(&source_directory)?;

    // PORT NOTE (added hard failure): the TypeScript prints
    // `verified=0 failures=0 bytes=0` and exits 0 when the source directory
    // holds no address-named file. A verifier that compared nothing must not
    // report success.
    verify::require_nonempty_corpus(sources.len(), target.id, &source_directory)?;

    let mut failures: Vec<String> = Vec::new();
    let mut total: u64 = 0;
    for source in &sources {
        let path = source.to_string_lossy().into_owned();
        let (matched, size) = verify(&path, &rom, &output_dir, target.compiler)?;
        total += size as u64;
        if !matched {
            failures.push(
                source
                    .file_name()
                    .map(|name| name.to_string_lossy().into_owned())
                    .unwrap_or(path),
            );
        }
    }
    println!(
        "verified={} failures={} bytes={total}",
        sources.len() - failures.len(),
        failures.len()
    );
    Ok(failures.is_empty())
}

fn main() -> ExitCode {
    match run() {
        Ok(true) => ExitCode::SUCCESS,
        Ok(false) => ExitCode::FAILURE,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
