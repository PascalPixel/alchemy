//! `check symbols` -- validate the symbol map, and emit the header from it.

use std::process::ExitCode;

pub fn entry(arguments: &[String]) -> ExitCode {
    let root = std::path::Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(std::path::Path::parent)
        .expect("crate sits two levels below the repository root")
        .to_path_buf();
    if arguments.iter().any(|a| a == "--self-test") {
        return match crate::self_test() {
            Ok(()) => {
                println!("symbols self-test ok");
                ExitCode::SUCCESS
            }
            Err(error) => {
                eprintln!("error: {error}");
                ExitCode::FAILURE
            }
        };
    }
    let path = crate::map_path(&root);
    let text = match std::fs::read_to_string(&path) {
        Ok(text) => text,
        Err(error) => {
            eprintln!("error: cannot read {}: {error}", path.display());
            return ExitCode::FAILURE;
        }
    };
    let entries = match crate::parse(&text) {
        Ok(entries) => entries,
        Err(error) => {
            eprintln!("error: {error}");
            return ExitCode::FAILURE;
        }
    };
    let problems = crate::violations(&entries);
    if !problems.is_empty() {
        for problem in &problems {
            eprintln!("error: {problem}");
        }
        return ExitCode::FAILURE;
    }
    if arguments.iter().any(|a| a == "--emit") {
        let header = crate::header(&entries);
        let out = root.join("include/symbols.h");
        if let Err(error) = std::fs::write(&out, header) {
            eprintln!("error: cannot write {}: {error}", out.display());
            return ExitCode::FAILURE;
        }
        println!("symbols ok: {} entries -> include/symbols.h", entries.len());
        return ExitCode::SUCCESS;
    }
    println!("symbols ok: {} entries", entries.len());
    ExitCode::SUCCESS
}
