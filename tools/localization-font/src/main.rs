use std::fs;
use std::io::{self, Write};
use std::path::Path;
use std::process::ExitCode;

use localization_font::{
    build_localization_font, canonical_source, export_localization_font, self_test, Error,
    PREFIX_ADDRESS, PREFIX_END,
};
use serde_json::Value;

const USAGE: &str = "usage: localization_font.ts {export ROM --root ASSETS --output SOURCE|build SOURCE --root ASSETS --output FILE|verify ROM SOURCE --root ASSETS|--self-test}";

fn option(args: &[String], name: &str) -> Result<String, Error> {
    let index = args
        .iter()
        .position(|arg| arg == name)
        .ok_or_else(|| Error(format!("{name} is required")))?;
    args.get(index + 1)
        .cloned()
        .ok_or_else(|| Error(format!("{name} is required")))
}

fn run(mut args: Vec<String>) -> Result<(), Error> {
    if args.iter().any(|arg| arg == "--self-test") {
        self_test()?;
        println!("self-test=ok");
        args.retain(|arg| arg != "--self-test");
        if args.is_empty() {
            return Ok(());
        }
    }
    match args.first().map(String::as_str) {
        Some("export") => {
            let rom_path = args.get(1).ok_or_else(|| Error(USAGE.into()))?;
            let root = option(&args, "--root")?;
            let output = option(&args, "--output")?;
            let source = export_localization_font(
                &fs::read(rom_path).map_err(|e| Error(e.to_string()))?,
                Path::new(&root),
            )?;
            if let Some(parent) = Path::new(&output).parent() {
                fs::create_dir_all(parent).map_err(|e| Error(e.to_string()))?;
            }
            fs::write(&output, format!("{}\n", canonical_source(&source)))
                .map_err(|e| Error(e.to_string()))?;
            let banks = source["mtf_banks"]
                .as_array()
                .unwrap()
                .iter()
                .map(|bank| bank["images"].as_u64().unwrap())
                .sum::<u64>();
            println!(
                "identical=true bytes={} glyphs=224 mtf_images={banks}",
                PREFIX_END - PREFIX_ADDRESS
            );
            Ok(())
        }
        Some("build") => {
            let input = args.get(1).ok_or_else(|| Error(USAGE.into()))?;
            let root = option(&args, "--root")?;
            let output = option(&args, "--output")?;
            let source: Value =
                serde_json::from_slice(&fs::read(input).map_err(|e| Error(e.to_string()))?)
                    .map_err(|e| Error(e.to_string()))?;
            let built = build_localization_font(&source, Path::new(&root))?;
            if let Some(parent) = Path::new(&output).parent() {
                fs::create_dir_all(parent).map_err(|e| Error(e.to_string()))?;
            }
            fs::write(output, built).map_err(|e| Error(e.to_string()))?;
            println!("bytes={}", PREFIX_END - PREFIX_ADDRESS);
            Ok(())
        }
        Some("build-stdout") => {
            let input = args.get(1).ok_or_else(|| Error(USAGE.into()))?;
            let root = option(&args, "--root")?;
            let source: Value = serde_json::from_slice(
                &fs::read(input).map_err(|e| Error(e.to_string()))?,
            )
            .map_err(|e| Error(e.to_string()))?;
            let built = build_localization_font(&source, Path::new(&root))?;
            io::stdout().write_all(&built).map_err(|error| Error(error.to_string()))?;
            Ok(())
        }
        Some("verify") => {
            let rom_path = args.get(1).ok_or_else(|| Error(USAGE.into()))?;
            let input = args.get(2).ok_or_else(|| Error(USAGE.into()))?;
            let root = option(&args, "--root")?;
            let source: Value =
                serde_json::from_slice(&fs::read(input).map_err(|e| Error(e.to_string()))?)
                    .map_err(|e| Error(e.to_string()))?;
            let built = build_localization_font(&source, Path::new(&root))?;
            let rom = fs::read(rom_path).map_err(|e| Error(e.to_string()))?;
            let start = (PREFIX_ADDRESS - localization_font::ROM_BASE) as usize;
            let end = (PREFIX_END - localization_font::ROM_BASE) as usize;
            if end > rom.len() || built != rom[start..end] {
                return Err(Error("localization-font source differs from ROM".into()));
            }
            println!("identical=true bytes={}", built.len());
            Ok(())
        }
        _ => Err(Error(USAGE.into())),
    }
}

fn main() -> ExitCode {
    match run(std::env::args().skip(1).collect()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(Error(message)) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}
