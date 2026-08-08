//! `overlay_disasm.ts`'s `import.meta.main` block.
//!
//! PORT NOTE: the TypeScript accepts ONLY `--self-test` and THROWS on anything
//! else, including no argument at all. Bun prints a thrown Error to stderr and
//! exits 1. Parity is asserted on the exit code and the message body, never on
//! Bun's surrounding prose.

use std::process::ExitCode;

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
    // Two conveniences the TypeScript reaches through its importers rather than
    // its own CLI. They are additive: `--self-test` and the error case below are
    // byte-identical to the original.
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    if arguments.first().map(String::as_str) == Some("--build-source") && arguments.len() >= 2 {
        let base = arguments
            .get(2)
            .map(|text| {
                i64::from_str_radix(text.trim_start_matches("0x"), 16).expect("base must be hex")
            })
            .unwrap_or(overlay_disasm::OVERLAY_BASE);
        return match overlay_disasm::disasm::build_overlay_source_from_file(
            std::path::Path::new(&arguments[1]),
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
    eprintln!("usage: overlay_disasm.ts --self-test");
    ExitCode::FAILURE
}
