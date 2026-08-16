//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

use std::fs;
use std::io::{self, Write};
use std::path::Path;
use std::process::ExitCode;

use crate::{
    canonical_catalog, catalog_size, descriptor_count, export_character_catalog, read_json,
    self_test, verify_character_catalog, Error, ROM_BASE,
};

const USAGE: &str = "usage: character-catalog export ROM --output INDEX | verify ROM INDEX | build-stdout SOURCE | verify-unit ROM --catalog INDEX --series INDEX --palette PNG | --self-test";

fn option(args: &[String], start: usize, name: &str, allowed: &[&str]) -> Result<String, Error> {
    let mut found = None;
    let mut index = start;
    while index < args.len() {
        let option = args[index].as_str();
        if !allowed.contains(&option) {
            return Err(Error(USAGE.into()));
        }
        if option == name && found.is_some() {
            return Err(Error(format!("duplicate {option}")));
        }
        let value = args
            .get(index + 1)
            .filter(|value| !value.starts_with('-'))
            .cloned()
            .ok_or_else(|| Error(format!("missing {option}")))?;
        if option == name {
            found = Some(value);
        }
        index += 2;
    }
    found.ok_or_else(|| Error(format!("missing {name}")))
}

fn resolve(path: &str) -> String {
    Path::new(path)
        .canonicalize()
        .unwrap_or_else(|_| Path::new(path).to_path_buf())
        .to_string_lossy()
        .into_owned()
}

fn run(args: &[String]) -> Result<(), Error> {
    match args.first().map(String::as_str) {
        Some(flag) if args.len() == 1 && matches!(flag, "-h" | "--help") => {
            println!("{USAGE}");
            Ok(())
        }
        Some("--self-test") if args.len() == 1 => {
            self_test()?;
            println!("self-test=ok");
            Ok(())
        }
        Some("export") => {
            if args.len() != 4 {
                return Err(Error(USAGE.into()));
            }
            let rom_path = args
                .get(1)
                .ok_or_else(|| Error("missing ROM path".into()))?;
            let output = resolve(&option(args, 2, "--output", &["--output"])?);
            if resolve(rom_path) == output {
                return Err(Error("refusing to overwrite the ROM".into()));
            }
            let rom = fs::read(rom_path).map_err(|error| Error(format!("{rom_path}: {error}")))?;
            let catalog = export_character_catalog(&rom)?;
            if let Some(parent) = Path::new(&output).parent() {
                fs::create_dir_all(parent)
                    .map_err(|error| Error(format!("{}: {error}", parent.display())))?;
            }
            fs::write(&output, format!("{}\n", canonical_catalog(&catalog)))
                .map_err(|error| Error(format!("{output}: {error}")))?;
            verify_character_catalog(&rom, &catalog)?;
            println!(
                "descriptors={} groups={} bytes={}",
                descriptor_count(&catalog)?,
                array_len(&catalog, "animation_groups")?,
                catalog_size(&catalog)?
            );
            Ok(())
        }
        Some("verify") => {
            if args.len() != 3 {
                return Err(Error(USAGE.into()));
            }
            let rom_path = args
                .get(1)
                .ok_or_else(|| Error("usage: character-catalog verify ROM INDEX".into()))?;
            let index_path = args
                .get(2)
                .ok_or_else(|| Error("usage: character-catalog verify ROM INDEX".into()))?;
            let catalog = read_json(index_path)?;
            let rom = fs::read(rom_path).map_err(|error| Error(format!("{rom_path}: {error}")))?;
            verify_character_catalog(&rom, &catalog)?;
            println!("identical=true bytes={}", catalog_size(&catalog)?);
            Ok(())
        }
        Some("build-stdout") => {
            if args.len() != 2 {
                return Err(Error(USAGE.into()));
            }
            let index_path = args
                .get(1)
                .ok_or_else(|| Error("usage: character-catalog build-stdout SOURCE".into()))?;
            let catalog = read_json(index_path)?;
            let bytes = crate::build_character_catalog(&catalog)?;
            io::stdout()
                .write_all(&bytes)
                .map_err(|error| Error(format!("stdout: {error}")))?;
            Ok(())
        }
        Some("verify-unit") => {
            if args.len() != 8 {
                return Err(Error(USAGE.into()));
            }
            let rom_path = args
                .get(1)
                .ok_or_else(|| Error("missing ROM path".into()))?;
            let catalog_path = option(args, 2, "--catalog", &["--catalog", "--series", "--palette"])?;
            let series_path = option(args, 2, "--series", &["--catalog", "--series", "--palette"])?;
            let _palette_path = option(args, 2, "--palette", &["--catalog", "--series", "--palette"])?;
            let catalog = read_json(&catalog_path)?;
            let catalog_data = crate::build_character_catalog(&catalog)?;
            let series = read_json(&series_path)?;
            let series_address = integer_field(&series, "address")?;
            let series_size = integer_field(&series, "size")?;
            let catalog_address = integer_field(&catalog, "address")?;
            if catalog_address + catalog_data.len() as i64 != series_address || series_size < 0 {
                return Err(Error("character unit extents are not adjacent".into()));
            }
            let rom = fs::read(rom_path).map_err(|error| Error(format!("{rom_path}: {error}")))?;
            let offset = catalog_address - ROM_BASE;
            if offset < 0 {
                return Err(Error("character unit differs from the ROM".into()));
            }
            let offset = offset as usize;
            let total = catalog_data.len() + series_size as usize;
            if offset.checked_add(total).is_none_or(|end| end > rom.len())
                || rom[offset..offset + catalog_data.len()] != catalog_data
            {
                return Err(Error("character unit differs from the ROM".into()));
            }
            println!(
                "identical=true descriptors={} bytes={}",
                descriptor_count(&catalog)?,
                total
            );
            Ok(())
        }
        _ => Err(Error(USAGE.into())),
    }
}

fn integer_field(value: &serde_json::Value, name: &str) -> Result<i64, Error> {
    match value.get(name) {
        Some(serde_json::Value::String(text)) => {
            let digits = text.strip_prefix("0x").unwrap_or(text);
            i64::from_str_radix(digits, 16).map_err(|_| Error(format!("invalid {name}")))
        }
        Some(serde_json::Value::Number(number)) => number
            .as_i64()
            .ok_or_else(|| Error(format!("invalid {name}"))),
        _ => Err(Error(format!("invalid {name}"))),
    }
}

fn array_len(value: &serde_json::Value, name: &str) -> Result<usize, Error> {
    value
        .get(name)
        .and_then(serde_json::Value::as_array)
        .map(Vec::len)
        .ok_or_else(|| Error(format!("invalid {name}")))
}

pub fn entry(arguments: &[String]) -> std::process::ExitCode {
    match run(&std::env::args().skip(1).collect::<Vec<_>>()) {
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
    fn option_parser_rejects_unknown_and_missing_values() {
        let args = vec!["--output".into(), "index.json".into()];
        assert_eq!(
            option(&args, 0, "--output", &["--output"]).unwrap(),
            "index.json"
        );
        assert!(option(
            &["--other".into(), "value".into()],
            0,
            "--output",
            &["--output"]
        )
        .is_err());
        assert!(USAGE.contains("build-stdout"));
    }
}
