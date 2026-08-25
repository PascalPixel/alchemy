use std::io::Write;
use std::path::Path;
use std::process::ExitCode;

use crate::build_localization_font;

pub fn entry(arguments: &[String]) -> ExitCode {
    let result = match arguments {
        [command, source, root_flag, root] if command == "build-stdout" && root_flag == "--root" => (|| {
            let source = serde_json::from_slice(&std::fs::read(source).map_err(|e| e.to_string())?)
                .map_err(|e| e.to_string())?;
            let bytes = build_localization_font(&source, Path::new(root)).map_err(|e| e.0)?;
            std::io::stdout().write_all(&bytes).map_err(|e| e.to_string())
        })(),
        [argument] if matches!(argument.as_str(), "-h" | "--help") => {
            println!("usage: localization-font build-stdout SOURCE --root ASSETS");
            Ok(())
        }
        _ => Err("usage: localization-font build-stdout SOURCE --root ASSETS".into()),
    };
    match result {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
