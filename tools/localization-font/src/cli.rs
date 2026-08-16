//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

use std::fs;
use std::io::{self, Write};
use std::path::Path;
use std::process::ExitCode;

use crate::{
    build_localization_font, canonical_source, export_localization_font, self_test, Error,
    PREFIX_ADDRESS, PREFIX_END,
};
use serde_json::Value;

const USAGE: &str = "usage: localization-font {export ROM --root ASSETS --output SOURCE|build SOURCE --root ASSETS --output FILE|build-stdout SOURCE --root ASSETS|verify ROM SOURCE --root ASSETS|--self-test}";

fn option(args: &[String], start: usize, name: &str, allowed: &[&str]) -> Result<String, Error> {
    let mut found = None;
    let mut index = start;
    while index < args.len() {
        let option = args[index].as_str();
        if !allowed.contains(&option) {
            return Err(Error(USAGE.into()));
        }
        let value = args
            .get(index + 1)
            .filter(|value| !value.starts_with('-'))
            .cloned()
            .ok_or_else(|| Error(format!("{option} is required")))?;
        if option == name {
            if found.is_some() {
                return Err(Error(format!("duplicate {option}")));
            }
            found = Some(value);
        }
        index += 2;
    }
    found.ok_or_else(|| Error(format!("{name} is required")))
}

fn run(args: Vec<String>) -> Result<(), Error> {
    if args.len() == 1 && matches!(args[0].as_str(), "-h" | "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    if args.len() == 1 && args[0] == "--self-test" {
        self_test()?;
        println!("self-test=ok");
        return Ok(());
    }
    match args.first().map(String::as_str) {
        Some("export") => {
            if args.len() != 6 {
                return Err(Error(USAGE.into()));
            }
            let rom_path = args.get(1).ok_or_else(|| Error(USAGE.into()))?;
            let root = option(&args, 2, "--root", &["--root", "--output"])?;
            let output = option(&args, 2, "--output", &["--root", "--output"])?;
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
            if args.len() != 6 {
                return Err(Error(USAGE.into()));
            }
            let input = args.get(1).ok_or_else(|| Error(USAGE.into()))?;
            let root = option(&args, 2, "--root", &["--root", "--output"])?;
            let output = option(&args, 2, "--output", &["--root", "--output"])?;
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
            if args.len() != 4 {
                return Err(Error(USAGE.into()));
            }
            let input = args.get(1).ok_or_else(|| Error(USAGE.into()))?;
            let root = option(&args, 2, "--root", &["--root"])?;
            let source: Value = serde_json::from_slice(
                &fs::read(input).map_err(|e| Error(e.to_string()))?,
            )
            .map_err(|e| Error(e.to_string()))?;
            let built = build_localization_font(&source, Path::new(&root))?;
            io::stdout().write_all(&built).map_err(|error| Error(error.to_string()))?;
            Ok(())
        }
        Some("verify") => {
            if args.len() != 5 {
                return Err(Error(USAGE.into()));
            }
            let rom_path = args.get(1).ok_or_else(|| Error(USAGE.into()))?;
            let input = args.get(2).ok_or_else(|| Error(USAGE.into()))?;
            let root = option(&args, 3, "--root", &["--root"])?;
            let source: Value =
                serde_json::from_slice(&fs::read(input).map_err(|e| Error(e.to_string()))?)
                    .map_err(|e| Error(e.to_string()))?;
            let built = build_localization_font(&source, Path::new(&root))?;
            let rom = fs::read(rom_path).map_err(|e| Error(e.to_string()))?;
            let start = (PREFIX_ADDRESS - crate::ROM_BASE) as usize;
            let end = (PREFIX_END - crate::ROM_BASE) as usize;
            if end > rom.len() || built != rom[start..end] {
                return Err(Error("localization-font source differs from ROM".into()));
            }
            println!("identical=true bytes={}", built.len());
            Ok(())
        }
        _ => Err(Error(USAGE.into())),
    }
}

pub fn entry(arguments: &[String]) -> std::process::ExitCode {
    match run(std::env::args().skip(1).collect()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(Error(message)) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}

#[cfg(test)]
mod tests {
    use super::{option, USAGE};

    #[test]
    fn option_parser_rejects_unknown_options() {
        let args = vec![
            "--root".into(),
            "assets".into(),
            "--output".into(),
            "source".into(),
        ];
        assert_eq!(
            option(&args, 0, "--root", &["--root", "--output"]).unwrap(),
            "assets"
        );
        assert!(option(
            &["--unknown".into(), "value".into()],
            0,
            "--root",
            &["--root"]
        )
        .is_err());
        assert!(USAGE.contains("build-stdout"));
    }
}
