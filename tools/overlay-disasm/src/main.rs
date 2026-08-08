//! `overlay_disasm.ts`'s `import.meta.main` block.
//!
//! PORT NOTE: the TypeScript accepts ONLY `--self-test` and THROWS on anything
//! else, including no argument at all. Bun prints a thrown Error to stderr and
//! exits 1. Parity is asserted on the exit code and the message body, never on
//! Bun's surrounding prose.

use std::io::{self, Write};
use std::path::Path;
use std::process::ExitCode;

fn base_argument(arguments: &[String], index: usize) -> Result<i64, String> {
    arguments
        .get(index)
        .map(|text| i64::from_str_radix(text.trim_start_matches("0x"), 16))
        .transpose()
        .map_err(|_| "base must be hex".to_string())
        .map(|value| value.unwrap_or(overlay_disasm::OVERLAY_BASE))
}

fn main() -> ExitCode {
    let argument = std::env::args().nth(1);
    if argument.as_deref() == Some("--self-test") {
        return match overlay_disasm::selftest::self_test() {
            Ok(message) => {
                println!("{message}");
                ExitCode::SUCCESS
            }
            Err(error) => {
                eprintln!("{error}");
                ExitCode::FAILURE
            }
        };
    }
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    if arguments.len() >= 2 && arguments[0] == "--build-source" {
        let base = match base_argument(&arguments, 2) {
            Ok(base) => base,
            Err(error) => {
                eprintln!("{error}");
                return ExitCode::FAILURE;
            }
        };
        return match overlay_disasm::disasm::build_overlay_source_from_file(
            Path::new(&arguments[1]),
            base,
        ) {
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
        let source = overlay_disasm::OverlaySource::path(arguments[1].clone());
        if arguments[0] == "--assemble" {
            return match overlay_disasm::assemble_overlay(&source, base) {
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
        for span in overlay_disasm::overlay_c_spans(&source, base) {
            println!("{},{}", span.start, span.end);
        }
        return ExitCode::SUCCESS;
    }
    eprintln!("usage: overlay-disasm --self-test");
    ExitCode::FAILURE
}
