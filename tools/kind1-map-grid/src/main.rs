use std::process::ExitCode;
use std::io::{self, Write};

use kind1_map_grid::{export_grid, self_test, verify_grid, Result, ROM_BASE};

const USAGE: &str = "usage: kind1-map-grid [export|verify] ROM --address N --size N --directory DIR | --self-test";

fn option(args: &[String], name: &str) -> Result<String> {
    let index = args
        .iter()
        .position(|arg| arg == name)
        .ok_or_else(|| format!("{name} is required"))?;
    args.get(index + 1)
        .cloned()
        .ok_or_else(|| format!("{name} is required"))
}

fn number(value: &str) -> Result<usize> {
    let (digits, radix) = value
        .strip_prefix("0x")
        .map_or((value, 10), |digits| (digits, 16));
    usize::from_str_radix(digits, radix).map_err(|_| format!("invalid number: {value}"))
}

fn run(mut args: Vec<String>) -> Result<()> {
    if args.iter().any(|arg| arg == "--self-test") {
        self_test()?;
        args.retain(|arg| arg != "--self-test");
        if args.is_empty() {
            println!("self-test=ok");
            return Ok(());
        }
    }
    if args.iter().any(|arg| arg == "-h" || arg == "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    if args.first().map(String::as_str) == Some("build-stdout") {
        let plan = args
            .get(1)
            .ok_or_else(|| "build-stdout requires a plan".to_string())?;
        let directory = option(&args, "--directory")?;
        let source = std::fs::read_to_string(plan).map_err(|e| e.to_string())?;
        let plan: serde_json::Value = serde_json::from_str(&source).map_err(|e| e.to_string())?;
        let built = kind1_map_grid::build_grid(&plan, std::path::Path::new(&directory))?;
        io::stdout().write_all(&built).map_err(|e| e.to_string())?;
        return Ok(());
    }

    let command = match args.first().map(String::as_str) {
        Some("export") | Some("verify") => args.remove(0),
        _ => "export".to_string(),
    };
    let rom_path = args
        .iter()
        .find(|arg| !arg.starts_with('-'))
        .ok_or_else(|| USAGE.to_string())?;
    let address = number(&option(&args, "--address")?)?;
    let size = number(&option(&args, "--size")?)?;
    let directory = option(&args, "--directory")?;
    let rom = std::fs::read(rom_path).map_err(|error| format!("{rom_path}: {error}"))?;

    if command == "verify" {
        let stats = verify_grid(&rom, address, size, std::path::Path::new(&directory))?;
        println!(
            "identical=true tokens={} sentinels={} encoded=0x{:x}",
            stats.tokens, stats.sentinels, stats.encoded
        );
    } else {
        let start = address
            .checked_sub(ROM_BASE)
            .ok_or_else(|| "kind-1 grid range is outside the ROM".to_string())?;
        let end = start
            .checked_add(size)
            .ok_or_else(|| "kind-1 grid range is outside the ROM".to_string())?;
        if end > rom.len() {
            return Err("kind-1 grid range is outside the ROM".to_string());
        }
        let stats = export_grid(&rom[start..end], std::path::Path::new(&directory))?;
        println!(
            "tokens={} sentinels={} encoded=0x{:x}",
            stats.tokens, stats.sentinels, stats.encoded
        );
    }
    Ok(())
}

fn main() -> ExitCode {
    match run(std::env::args().skip(1).collect()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(message) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}
