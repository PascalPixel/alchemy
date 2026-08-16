//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

use std::env;
use std::io::Write;
use std::path::{Path, PathBuf};
use std::process::ExitCode;

use crate::{build_byte_henkan_tables, self_test};

fn run(args: &[String]) -> Result<(), String> {
    if args.iter().any(|arg| arg == "--self-test") {
        return self_test();
    }
    if args.iter().any(|arg| arg == "--help" || arg == "-h") {
        println!("usage: byte-henkan [TABLE.json]  (default assets/data/byte_henkan_hyou.json)");
        return Ok(());
    }
    if let [command, source] = args {
        if command == "build-stdout" {
            let bytes = build_byte_henkan_tables(Path::new(source))?;
            std::io::stdout()
                .write_all(&bytes)
                .map_err(|error| error.to_string())?;
            return Ok(());
        }
    }
    // PORT NOTE: the TypeScript resolves the default source relative to the
    // current directory. A compiled binary is not run from the repository root,
    // so the default is anchored to the repository instead. An explicit path
    // argument is still used verbatim, exactly as in the TypeScript.
    let source: PathBuf = match args.first() {
        Some(path) => PathBuf::from(path),
        None => Path::new(env!("CARGO_MANIFEST_DIR"))
            .parent()
            .expect("crate directory has a parent")
            .parent()
            .expect("tools has a parent")
            .join("assets/data/byte_henkan_hyou.json"),
    };
    println!("bytes={}", build_byte_henkan_tables(&source)?.len());
    Ok(())
}

pub fn entry(arguments: &[String]) -> std::process::ExitCode {
    let args: Vec<String> = arguments.to_vec();
    match run(&args) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
    }
}
