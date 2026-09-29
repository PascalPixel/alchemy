//! po2ags: an edition's PO catalog to its message archive and message
//! numbers as assembly, the step pret's preproc and charmap take for
//! Pokémon's text. Every pointer inside the archive names its own labels.
use std::path::Path;
use std::process::ExitCode;

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    if args.len() < 2 || args[0] == "-h" || args[0] == "--help" {
        eprintln!("usage: po2ags CATALOG.PO OUTPUT.inc [--label NAME]");
        return ExitCode::FAILURE;
    }
    let label = args
        .iter()
        .position(|arg| arg == "--label")
        .and_then(|index| args.get(index + 1))
        .map_or("Text_Message", String::as_str);
    let result = ags::text::read_source(Path::new(&args[0]))
        .and_then(|source| ags::text::messages_include(&source, label, &args[0]))
        .and_then(|text| {
            std::fs::write(&args[1], text).map_err(|error| format!("{}: {error}", args[1]))
        });
    match result {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
    }
}
