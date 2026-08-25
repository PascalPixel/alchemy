use std::io::{self, Write};
use std::path::Path;
use std::process::ExitCode;
const USAGE: &str = "usage: overlay-disasm --build-source FILE [BASE_HEX]\n       overlay-disasm --assemble FILE [BASE_HEX]\n       overlay-disasm --c-spans FILE [BASE_HEX]";
fn base_argument(arguments: &[String], index: usize) -> Result<i64, String> {
    arguments.get(index).map(|text| i64::from_str_radix(text.trim_start_matches("0x"), 16)).transpose().map_err(|_| "base must be hex".to_string()).map(|value| value.unwrap_or(crate::OVERLAY_BASE))
}
fn validate_arguments(arguments: &[String]) -> Result<bool, String> {
    if arguments.len() == 1 && matches!(arguments[0].as_str(), "-h" | "--help") {
        println!("{USAGE}");
        return Ok(true);
    }
    let Some(mode) = arguments.first().map(String::as_str) else {
        return Err(USAGE.to_string());
    };
    if !matches!(mode, "--build-source" | "--assemble" | "--c-spans") {
        return Err(format!("unknown option: {mode}\n{USAGE}"));
    }
    if !(arguments.len() == 2 || arguments.len() == 3) {
        return Err(USAGE.to_string());
    }
    if arguments.len() == 3 && arguments[2].starts_with('-') {
        return Err(format!("base must be hex\n{USAGE}"));
    }
    Ok(false)
}
pub fn entry(arguments: &[String]) -> ExitCode {
    let arguments: Vec<String> = arguments.to_vec();
    match validate_arguments(&arguments) {
        Ok(true) => return ExitCode::SUCCESS,
        Ok(false) => {}
        Err(error) => {
            eprintln!("{error}");
            return ExitCode::FAILURE;
        }
    }
    if arguments.len() >= 2 && arguments[0] == "--build-source" {
        let base = match base_argument(&arguments, 2) {
            Ok(base) => base,
            Err(error) => {
                eprintln!("{error}");
                return ExitCode::FAILURE;
            }
        };
        return match crate::disasm::build_overlay_source_from_file(Path::new(&arguments[1]), base) {
            Ok(text) => {
                print!("{text}");
                ExitCode::SUCCESS
            }
            Err(error) => {
                eprintln!("{error}");
                ExitCode::FAILURE
            }
        };
    }
    if arguments.len() >= 2 && (arguments[0] == "--assemble" || arguments[0] == "--c-spans") {
        let base = match base_argument(&arguments, 2) {
            Ok(base) => base,
            Err(error) => {
                eprintln!("{error}");
                return ExitCode::FAILURE;
            }
        };
        let source = crate::OverlaySource::path(arguments[1].clone());
        if arguments[0] == "--assemble" {
            return match crate::assemble_overlay(&source, base) {
                Ok(bytes) => match io::stdout().write_all(&bytes) {
                    Ok(()) => ExitCode::SUCCESS,
                    Err(error) => {
                        eprintln!("{error}");
                        ExitCode::FAILURE
                    }
                },
                Err(error) => {
                    eprintln!("{error}");
                    ExitCode::FAILURE
                }
            };
        }
        match crate::overlay_c_spans(&source, base) {
            Ok(spans) => {
                for span in spans {
                    println!("{},{}", span.start, span.end);
                }
            }
            Err(error) => {
                eprintln!("{error}");
                return ExitCode::FAILURE;
            }
        }
        return ExitCode::SUCCESS;
    }
    eprintln!("{USAGE}");
    ExitCode::FAILURE
}
