use std::fs;
use std::process::ExitCode;

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    if args.len() != 2 || args[0] == "-h" || args[0] == "--help" {
        eprintln!("usage: wav2ags INPUT.pcm8.wav|INPUT.pcm4 OUTPUT.bin");
        return ExitCode::FAILURE;
    }
    let result = fs::read(&args[0])
        .map_err(|error| format!("{}: {error}", args[0]))
        .and_then(|input| ags::sound::build_sound_file(&args[0], &input))
        .and_then(|bytes| {
            ags::write_whole(&args[1], &bytes)
        });
    match result {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
    }
}
