use std::fs;
use std::path::Path;
use std::process::ExitCode;

use tilemap::{export_tilemap, import_tilemap};

fn value(args: &[String], name: &str) -> Result<String, String> {
    let index = args
        .iter()
        .position(|arg| arg == name)
        .ok_or(format!("{name} is required"))?;
    args.get(index + 1)
        .cloned()
        .ok_or(format!("{name} is required"))
}

fn self_test() -> Result<(), String> {
    let raw: Vec<u8> = (0..64u16)
        .flat_map(|index| index.wrapping_mul(257).to_le_bytes())
        .collect();
    let text = export_tilemap(&raw, 8).map_err(|e| e.to_string())?;
    if import_tilemap(&text).map_err(|e| e.to_string())? != raw {
        return Err("tilemap round-trip failed".to_string());
    }
    println!("self-test=ok");
    Ok(())
}

fn run(mut args: Vec<String>) -> Result<(), String> {
    if args.iter().any(|arg| arg == "-h" || arg == "--help") {
        println!("usage: tilemap [--self-test] {{export,import}} <input> -o <output> [--width N]");
        return Ok(());
    }
    if args.iter().any(|arg| arg == "--self-test") {
        self_test()?;
        args.retain(|arg| arg != "--self-test");
        if args.is_empty() {
            return Ok(());
        }
    }
    let command = args
        .first()
        .cloned()
        .ok_or("a tilemap command is required")?;
    let input = args.get(1).cloned().ok_or("an input path is required")?;
    let flag = if args.iter().any(|arg| arg == "-o") {
        "-o"
    } else {
        "--output"
    };
    let output = value(&args, flag)?;
    if let Some(parent) = Path::new(&output).parent() {
        fs::create_dir_all(parent).map_err(|e| e.to_string())?;
    }
    match command.as_str() {
        "export" => {
            let width: usize = value(&args, "--width")?
                .parse()
                .map_err(|_| "--width must be a positive integer")?;
            let data = fs::read(&input).map_err(|e| e.to_string())?;
            let result = export_tilemap(&data, width).map_err(|e| e.to_string())?;
            fs::write(&output, &result).map_err(|e| e.to_string())?;
            println!("entries={}", result.split_whitespace().count());
        }
        "import" => {
            let text = fs::read_to_string(&input).map_err(|e| e.to_string())?;
            let result = import_tilemap(&text).map_err(|e| e.to_string())?;
            fs::write(&output, &result).map_err(|e| e.to_string())?;
            println!("bytes={}", result.len());
        }
        _ => return Err("a tilemap command is required".to_string()),
    }
    Ok(())
}

fn main() -> ExitCode {
    match run(std::env::args().skip(1).collect()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(message) => {
            eprintln!("{message}");
            ExitCode::FAILURE
        }
    }
}
