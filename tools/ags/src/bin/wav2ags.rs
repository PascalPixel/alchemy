//! wav2ags: a sound input to the bytes a sound data source reads, as pret's
//! wav2agb is for Pokémon: an eight-bit WAV (NAME.PCM8.WAV) to its PCM wave
//! record, or a .PCM4 CGB wave pattern to its sixteen bytes.
use std::fs;
use std::process::ExitCode;

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    if args.len() != 2 || args[0] == "-h" || args[0] == "--help" {
        eprintln!("usage: wav2ags INPUT.PCM8.WAV|INPUT.PCM4 OUTPUT.bin");
        return ExitCode::FAILURE;
    }
    let result = fs::read(&args[0])
        .map_err(|error| format!("{}: {error}", args[0]))
        .and_then(|input| ags::sound::build_sound_file(&args[0], &input))
        .and_then(|bytes| {
            fs::write(&args[1], bytes).map_err(|error| format!("{}: {error}", args[1]))
        });
    match result {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
    }
}
