use byte_value_regions::build_byte_value_regions;
use serde_json::json;
use std::io::Write;
use std::path::Path;

fn parse_u32(text: &str) -> Result<u32, String> {
    let digits = text.strip_prefix("0x").unwrap_or(text);
    u32::from_str_radix(digits, 16).map_err(|_| format!("invalid address: {text}"))
}

fn main() {
    if let Err(error) = run() {
        eprintln!("{error}");
        std::process::exit(1);
    }
}

fn run() -> Result<(), String> {
    let args: Vec<String> = std::env::args().skip(1).collect();
    let [command, source, address, size] = args.as_slice() else {
        return Err("usage: byte-value-regions build-region-stdout SOURCE ADDRESS SIZE".into());
    };
    if command != "build-region-stdout" {
        return Err("usage: byte-value-regions build-region-stdout SOURCE ADDRESS SIZE".into());
    }
    let address = parse_u32(address)?;
    let size: usize = size.parse().map_err(|_| format!("invalid size: {size}"))?;
    let regions = build_byte_value_regions(Path::new(source)).map_err(|error| error.to_string())?;
    let region = regions
        .iter()
        .find(|region| region.address == address)
        .filter(|region| region.data.len() == size)
        .ok_or_else(|| "byte-value region differs from manifest".to_string())?;
    eprintln!(
        "{}",
        json!({
            "representation": "structured byte values",
            "region_address": format!("0x{address:08x}"),
        })
    );
    std::io::stdout()
        .write_all(&region.data)
        .map_err(|error| error.to_string())?;
    Ok(())
}
