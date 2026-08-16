//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

use crate::{
    export_resource_byte_canvases, self_test, verify_resource_byte_canvases, Error, RESOURCES,
    SOURCE_BYTES,
};
use std::path::Path;
use std::io::{self, Write};
use std::process::ExitCode;

const USAGE: &str = "usage: resource-byte-canvases export ROM --directory DIR | verify ROM --directory DIR | --self-test";

fn run(args: &[String]) -> Result<(), Error> {
    if args.len() == 1 && args[0] == "--self-test" {
        self_test()?;
        println!(
            "self-test=ok resources={} source_bytes={SOURCE_BYTES}",
            RESOURCES.len()
        );
        return Ok(());
    }
    if args.len() == 1 && matches!(args[0].as_str(), "-h" | "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    if args.len() == 3 && args[0] == "build-stdout" {
        let resource = crate::build_resource_byte_canvases(Path::new(&args[1]))?
            .into_iter()
            .find(|item| item.id == args[2].to_lowercase())
            .ok_or_else(|| Error(format!("resource {} is absent", args[2])))?;
        io::stdout().write_all(&resource.data).map_err(|e| Error(e.to_string()))?;
        return Ok(());
    }
    if args.len() == 4 && args[0] == "export" && args[2] == "--directory" {
        export_resource_byte_canvases(Path::new(&args[1]), Path::new(&args[3]))?;
        return Ok(());
    }
    if args.len() == 4 && args[0] == "verify" && args[2] == "--directory" {
        verify_resource_byte_canvases(Path::new(&args[1]), Path::new(&args[3]))?;
        println!(
            "identical=true resources={} source_bytes={SOURCE_BYTES}",
            RESOURCES.len()
        );
        return Ok(());
    }
    Err(Error(USAGE.into()))
}

pub fn entry(arguments: &[String]) -> std::process::ExitCode {
    match run(&arguments.to_vec()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(Error(message)) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}
