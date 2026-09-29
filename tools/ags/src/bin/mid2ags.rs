//! mid2ags: a MIDI song to the sound driver's sequence assembly, as pret's
//! mid2agb is for Pokémon. Every pointer in the output is a label.
use std::fs;
use std::process::ExitCode;

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    if args.len() != 2 || args[0] == "-h" || args[0] == "--help" {
        eprintln!("usage: mid2ags INPUT.MID OUTPUT.s");
        return ExitCode::FAILURE;
    }
    let result = fs::read(&args[0])
        .map_err(|error| format!("{}: {error}", args[0]))
        .and_then(|midi| ags::sound::sequence_assembly(&midi))
        .and_then(|text| {
            fs::write(&args[1], text).map_err(|error| format!("{}: {error}", args[1]))
        });
    match result {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
    }
}
