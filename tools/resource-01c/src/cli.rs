use crate::{build_resource_01c, Error};
use std::io::{self, Write};
use std::path::Path;
use std::process::ExitCode;

const USAGE: &str = "usage: resource_01c build-stdout PLAN";

fn run(args: &[String]) -> Result<(), Error> {
    if args.first().map(String::as_str) != Some("build-stdout") {
        return Err(Error(USAGE.into()));
    }
    let plan = args.get(1).ok_or_else(|| Error(USAGE.into()))?;
    let (bytes, _) = build_resource_01c(Path::new(plan))?;
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
