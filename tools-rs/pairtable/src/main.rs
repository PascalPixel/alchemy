use std::fs;
use std::path::Path;
use std::process::ExitCode;

use pairtable::{export_pairs, import_pairs};

fn self_test() -> Result<(), String> {
    let raw: Vec<u8> =
        [0u16, 1, 0x1234, 0xabcd, 0xffff, 2].iter().flat_map(|w| w.to_le_bytes()).collect();
    let text = export_pairs(&raw).map_err(|e| e.to_string())?;
    if import_pairs(&text).map_err(|e| e.to_string())? != raw {
        return Err("pair-table round-trip failed".to_string());
    }
    println!("self-test=ok");
    Ok(())
}

fn run(mut args: Vec<String>) -> Result<(), String> {
    if args.iter().any(|arg| arg == "-h" || arg == "--help") {
        println!("usage: pairtable [--self-test] {{export,build}} INPUT OUTPUT");
        return Ok(());
    }
    if args.iter().any(|arg| arg == "--self-test") {
        self_test()?;
        args.retain(|arg| arg != "--self-test");
        if args.is_empty() {
            return Ok(());
        }
    }
    let (command, input, output) = match args.as_slice() {
        [command, input, output] if command == "export" || command == "build" => {
            (command, input, output)
        }
        _ => return Err("a pair-table command is required".to_string()),
    };
    if let Some(parent) = Path::new(output).parent() {
        fs::create_dir_all(parent).map_err(|e| e.to_string())?;
    }
    if command == "export" {
        let data = fs::read(input).map_err(|e| e.to_string())?;
        fs::write(output, export_pairs(&data).map_err(|e| e.to_string())?).map_err(|e| e.to_string())
    } else {
        let text = fs::read_to_string(input).map_err(|e| e.to_string())?;
        fs::write(output, import_pairs(&text).map_err(|e| e.to_string())?).map_err(|e| e.to_string())
    }
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
