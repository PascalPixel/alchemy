use std::fs;
use std::io::{self, Write};
use std::path::Path;
use std::process::ExitCode;

use static_sprite_series::{
    build_series, export_series, read_json, self_test, verify_series, Error, Options,
    STATIC_DESCRIPTOR_COUNT, STATIC_DESCRIPTOR_TABLE, STATIC_PALETTE_ENTRIES,
    STATIC_PALETTE_OFFSET, STATIC_SERIES_ADDRESS, STATIC_SERIES_END,
};

const USAGE: &str = "usage: static-sprite-series export-series ROM --directory DIR --palette PNG [--address N --end N] [--descriptor-table N --descriptor-count N] [--palette-offset N --palette-entries N] [--suffix-zeros N] | build INDEX --palette PNG --output FILE | verify ROM INDEX --palette PNG | --self-test";

fn option(args: &[String], name: &str) -> Result<String, Error> {
    let index = args.iter().position(|arg| arg == name)
        .ok_or_else(|| Error(format!("{name} is required")))?;
    args.get(index + 1).cloned().ok_or_else(|| Error(format!("{name} is required")))
}

fn optional(args: &[String], name: &str) -> Result<Option<String>, Error> {
    match args.iter().position(|arg| arg == name) {
        Some(index) => args.get(index + 1).cloned().map(Some)
            .ok_or_else(|| Error(format!("{name} requires a value"))),
        None => Ok(None),
    }
}

fn path_same(a: &str, b: &str) -> bool {
    let left = Path::new(a).canonicalize().unwrap_or_else(|_| Path::new(a).to_path_buf());
    let right = Path::new(b).canonicalize().unwrap_or_else(|_| Path::new(b).to_path_buf());
    left == right
}

fn number(text: Option<String>, default: i64) -> Result<i64, Error> {
    match text {
        None => Ok(default),
        Some(text) => static_sprite_series::parse_integer(&text, "integer"),
    }
}

fn run(args: &[String]) -> Result<(), Error> {
    if args.iter().any(|arg| arg == "--self-test") {
        self_test()?;
        if args.len() == 1 { return Ok(()); }
    }
    match args.first().map(String::as_str) {
        Some("export-series") => {
            let rom_path = args.get(1).ok_or_else(|| Error(USAGE.into()))?;
            let directory = option(args, "--directory")?;
            let palette = option(args, "--palette")?;
            if path_same(rom_path, &palette) { return Err(Error("refusing to overwrite an input".into())); }
            let address = number(optional(args, "--address")?, STATIC_SERIES_ADDRESS)?;
            let end = number(optional(args, "--end")?, STATIC_SERIES_END)?;
            let descriptor_table = number(optional(args, "--descriptor-table")?, STATIC_DESCRIPTOR_TABLE)?;
            let descriptor_count = number(optional(args, "--descriptor-count")?, STATIC_DESCRIPTOR_COUNT)?;
            let palette_offset = number(optional(args, "--palette-offset")?, STATIC_PALETTE_OFFSET)?;
            let palette_entries = number(optional(args, "--palette-entries")?, STATIC_PALETTE_ENTRIES)?;
            let suffix_zeros = number(optional(args, "--suffix-zeros")?, 0)?;
            let rom = fs::read(rom_path).map_err(|e| Error(format!("{rom_path}: {e}")))?;
            let index = export_series(&rom, Path::new(&directory), Path::new(&palette), Options {
                address, end, descriptor_table, descriptor_count, palette_offset, palette_entries, suffix_zeros,
            })?;
            let frames = index["packages"].as_array().unwrap().iter().map(|item| {
                let path = Path::new(&directory).join(item["plan"].as_str().unwrap());
                read_json(&path).unwrap()["frames"].as_array().unwrap().len()
            }).sum::<usize>();
            println!("packages={} frames={} bytes={}", index["packages"].as_array().unwrap().len(), frames, number_field(&index, "size")?);
            Ok(())
        }
        Some("build") => {
            let index_path = args.get(1).ok_or_else(|| Error(USAGE.into()))?;
            let palette = option(args, "--palette")?;
            let output = option(args, "--output")?;
            let index = read_json(Path::new(index_path))?;
            let bytes = build_series(&index, Path::new(index_path), Path::new(&palette))?;
            if let Some(parent) = Path::new(&output).parent() { fs::create_dir_all(parent).map_err(|e| Error(e.to_string()))?; }
            fs::write(&output, bytes).map_err(|e| Error(e.to_string()))?;
            Ok(())
        }
        Some("build-stdout") => {
            let index_path = args.get(1).ok_or_else(|| Error(USAGE.into()))?;
            let palette = option(args, "--palette")?;
            let index = read_json(Path::new(index_path))?;
            let bytes = build_series(&index, Path::new(index_path), Path::new(&palette))?;
            io::stdout().write_all(&bytes).map_err(|e| Error(e.to_string()))?;
            Ok(())
        }
        Some("verify") => {
            let rom_path = args.get(1).ok_or_else(|| Error(USAGE.into()))?;
            let index_path = args.get(2).ok_or_else(|| Error(USAGE.into()))?;
            let palette = option(args, "--palette")?;
            let index = read_json(Path::new(index_path))?;
            let rom = fs::read(rom_path).map_err(|e| Error(e.to_string()))?;
            verify_series(&rom, &index, Path::new(index_path), Path::new(&palette))?;
            println!("identical=true bytes={}", number_field(&index, "size")?);
            Ok(())
        }
        Some("--help") | Some("-h") => { println!("{USAGE}"); Ok(()) }
        _ => Err(Error(USAGE.into())),
    }
}

fn number_field(value: &serde_json::Value, name: &str) -> Result<i64, Error> {
    match value.get(name) {
        Some(serde_json::Value::String(text)) => static_sprite_series::parse_integer(text, name),
        Some(serde_json::Value::Number(number)) => number.as_i64().ok_or_else(|| Error(format!("invalid {name}"))),
        _ => Err(Error(format!("invalid {name}"))),
    }
}

fn main() -> ExitCode {
    match run(&std::env::args().skip(1).collect::<Vec<_>>()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(Error(message)) => { eprintln!("error: {message}"); ExitCode::FAILURE }
    }
}
