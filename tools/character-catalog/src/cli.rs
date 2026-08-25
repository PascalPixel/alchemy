use crate::{build_character_catalog, read_json, Error};
use std::io::{self, Write};
use std::process::ExitCode;

const USAGE: &str = "usage: character-catalog build-stdout SOURCE";

fn run(args: &[String]) -> Result<(), Error> {
    if args.first().map(String::as_str) != Some("build-stdout") {
        return Err(Error(USAGE.into()));
    }
    let source = args.get(1).ok_or_else(|| Error(USAGE.into()))?;
    let bytes = build_character_catalog(&read_json(source)?)?;
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
