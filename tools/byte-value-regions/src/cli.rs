//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

use crate::{build_byte_value_regions, self_test};
use serde_json::json;
use std::io::Write;
use std::path::Path;

const USAGE: &str = "usage: byte-value-regions build-region-stdout SOURCE ADDRESS SIZE\n       byte-value-regions --self-test\n\nBuild one byte-value region from a canonical index.\n  -h, --help     show this help\n      --self-test validate the reader and writer without repository work";

#[derive(Debug, PartialEq, Eq)]
enum Action {
    Help,
    SelfTest,
    Build {
        source: String,
        address: String,
        size: String,
    },
}

fn parse_args(args: &[String]) -> Result<Action, String> {
    match args {
        [arg] if arg == "-h" || arg == "--help" => Ok(Action::Help),
        [arg] if arg == "--self-test" => Ok(Action::SelfTest),
        [command, source, address, size]
            if command == "build-region-stdout"
                && !source.starts_with('-')
                && !address.starts_with('-')
                && !size.starts_with('-') =>
        {
            Ok(Action::Build {
                source: source.clone(),
                address: address.clone(),
                size: size.clone(),
            })
        }
        _ => Err(USAGE.to_string()),
    }
}

fn parse_u32(text: &str) -> Result<u32, String> {
    let digits = text.strip_prefix("0x").unwrap_or(text);
    u32::from_str_radix(digits, 16).map_err(|_| format!("invalid address: {text}"))
}

pub fn entry(arguments: &[String]) {
    if let Err(error) = run(arguments) {
        eprintln!("{error}");
        std::process::exit(1);
    }
}

fn run(arguments: &[String]) -> Result<(), String> {
    let args: Vec<String> = arguments.to_vec();
    let action = parse_args(&args)?;
    let Action::Build {
        source,
        address,
        size,
    } = action
    else {
        if matches!(action, Action::Help) {
            println!("{USAGE}");
            return Ok(());
        }
        println!("{}", self_test()?);
        return Ok(());
    };
    let address = parse_u32(&address)?;
    let size: usize = size.parse().map_err(|_| format!("invalid size: {size}"))?;
    let regions =
        build_byte_value_regions(Path::new(&source)).map_err(|error| error.to_string())?;
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
