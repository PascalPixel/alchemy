use crate::{build_still, Error};
use std::fs;
use std::io::{self, Write};
use std::process::ExitCode;

const USAGE: &str = "usage: indexed-still build-stdout IMAGE";

fn run(args: &[String]) -> Result<(), Error> {
    if args.first().map(String::as_str) != Some("build-stdout") {
        return Err(Error(USAGE.into()));
    }
    let image = args.get(1).ok_or_else(|| Error(USAGE.into()))?;
    let bytes = fs::read(image).map_err(|e| Error(e.to_string()))?;
    let (built, _) = build_still(&bytes)?;
    io::stdout().write_all(&built).map_err(|e| Error(e.to_string()))?;
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
