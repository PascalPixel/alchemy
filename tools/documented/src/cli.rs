//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

use std::path::Path;
use std::process::ExitCode;

use crate::{
    catalog_violations, cataloged, command_names, documented, entry_points, scanned_nothing,
    self_test, violations,
};

const USAGE: &str = "usage: documented [--self-test]";

pub fn entry(arguments: &[String]) -> std::process::ExitCode {
    let arguments = arguments.to_vec();
    match arguments.as_slice() {
        [argument] if matches!(argument.as_str(), "-h" | "--help") => {
            println!("{USAGE}");
            return ExitCode::SUCCESS;
        }
        [argument] if argument == "--self-test" => {
            return match self_test() {
                Ok(line) => {
                    println!("{line}");
                    ExitCode::SUCCESS
                }
                Err(message) => {
                    eprintln!("error: {message}");
                    ExitCode::FAILURE
                }
            }
        }
        [] => {}
        _ => {
            eprintln!("error: {USAGE}");
            return ExitCode::from(2);
        }
    }
    let root = Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("documented must live under tools");
    let tools = entry_points();
    let markdown = match std::fs::read_to_string(root.join("CONTRIBUTING.md")) {
        Ok(text) => text,
        Err(error) => {
            eprintln!("error: cannot read CONTRIBUTING.md: {error}");
            return ExitCode::FAILURE;
        }
    };
    let docs = documented(&markdown);
    if let Some(why) = scanned_nothing(tools.len(), docs.len()) {
        eprintln!("error: {why}");
        return ExitCode::FAILURE;
    }
    let mut problems = violations(&tools, &docs);
    let commands = command_names();
    let catalog = match std::fs::read_to_string(root.join("CONTRIBUTING.md")) {
        Ok(text) => cataloged(&text),
        Err(error) => {
            eprintln!("error: cannot read CONTRIBUTING.md: {error}");
            return ExitCode::FAILURE;
        }
    };
    problems.extend(catalog_violations(&commands, &catalog));
    if !problems.is_empty() {
        for problem in &problems {
            eprintln!("error: {problem}");
        }
        eprintln!(
            "\n{} native documentation violation(s). The dispatcher is the command registry.",
            problems.len()
        );
        return ExitCode::FAILURE;
    }
    println!(
        "documented ok: {} command groups in CONTRIBUTING.md, {} public commands in CONTRIBUTING.md",
        tools.len(),
        commands.len()
    );
    ExitCode::SUCCESS
}
