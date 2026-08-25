use crate::{build_archive, Error};
use std::fs;
use std::io::{self, Write};
use std::path::Path;
use std::process::ExitCode;

const USAGE: &str = "usage: skip-sprite-archive build-stdout DIRECTORY PLAN PALETTE";

fn run(args: &[String]) -> Result<(), Error> {
    if args.first().map(String::as_str) != Some("build-stdout") {
        return Err(Error(USAGE.into()));
    }
    let directory = args.get(1).ok_or_else(|| Error(USAGE.into()))?;
    let plan = args.get(2).ok_or_else(|| Error(USAGE.into()))?;
    let palette = args.get(3).ok_or_else(|| Error(USAGE.into()))?;
    let value = serde_json::from_slice(&fs::read(plan).map_err(|e| Error(e.to_string()))?).map_err(|e| Error(e.to_string()))?;
    let bytes = build_archive(&value, Path::new(directory), Path::new(palette))?;
    io::stdout().write_all(&bytes).map_err(|e| Error(e.to_string()))?;
    Ok(())
}

pub fn entry(arguments: &[String]) -> ExitCode {
    match run(arguments) {
        Ok(()) => ExitCode::SUCCESS,
        Err(Error(message)) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}
