use crate::{build_resource_directory, Res};
use serde_json::Value;
use std::fs;
use std::io::{self, Write};
use std::process::ExitCode;

const USAGE: &str = "usage: resource-directory build-stdout SOURCE";

fn run(args: &[String]) -> Res<()> {
    if args.first().map(String::as_str) != Some("build-stdout") {
        return Err(USAGE.into());
    }
    let source = args.get(1).ok_or_else(|| USAGE.to_string())?;
    let value: Value =
        serde_json::from_slice(&fs::read(source).map_err(|e| e.to_string())?).map_err(|e| e.to_string())?;
    let bytes = build_resource_directory(&value)?;
    io::stdout().write_all(&bytes).map_err(|e| e.to_string())
}

pub fn entry(arguments: &[String]) -> ExitCode {
    match run(arguments) {
        Ok(()) => ExitCode::SUCCESS,
        Err(message) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}
