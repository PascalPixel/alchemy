//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

use std::fs;
use std::io::Write;
use std::path::Path;
use std::process::ExitCode;

use crate::{export_words, import_words, self_test};

fn run(args: Vec<String>) -> Result<(), String> {
    if args.as_slice() == ["--self-test"] {
        println!("{}", self_test()?);
        return Ok(());
    }
    if args.iter().any(|arg| arg == "-h" || arg == "--help") {
        println!("usage: wordstream {{export,build}} INPUT OUTPUT");
        return Ok(());
    }
    if let [command, input] = args.as_slice() {
        if command == "build-stdout" {
            let text = fs::read_to_string(input).map_err(|e| e.to_string())?;
            let bytes = import_words(&text).map_err(|e| e.to_string())?;
            std::io::stdout()
                .write_all(&bytes)
                .map_err(|e| e.to_string())?;
            return Ok(());
        }
    }
    let (command, input, output) = match args.as_slice() {
        [command, input, output] if command == "export" || command == "build" => {
            (command, input, output)
        }
        _ => return Err("a word-stream command is required".to_string()),
    };
    if let Some(parent) = Path::new(output).parent() {
        fs::create_dir_all(parent).map_err(|e| e.to_string())?;
    }
    if command == "export" {
        let data = fs::read(input).map_err(|e| e.to_string())?;
        fs::write(output, export_words(&data).map_err(|e| e.to_string())?)
            .map_err(|e| e.to_string())
    } else {
        let text = fs::read_to_string(input).map_err(|e| e.to_string())?;
        fs::write(output, import_words(&text).map_err(|e| e.to_string())?)
            .map_err(|e| e.to_string())
    }
}

pub fn entry(arguments: &[String]) -> std::process::ExitCode {
    match run(arguments.to_vec()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(message) => {
            eprintln!("{message}");
            ExitCode::FAILURE
        }
    }
}
