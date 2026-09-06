use std::io::{self, Write};
use std::path::Path;
use std::process::ExitCode;
const USAGE: &str = "usage: disassemble --build-source FILE [BASE_HEX]\n       disassemble --assemble FILE [BASE_HEX]\n       disassemble --c-spans FILE [BASE_HEX]";
fn base_argument(arguments: &[String]) -> Result<i64, String> {
    arguments
        .get(2)
        .map(|text| i64::from_str_radix(text.trim_start_matches("0x"), 16))
        .transpose()
        .map_err(|_| "base must be hex".to_string())
        .map(|value| value.unwrap_or(crate::OVERLAY_BASE))
}
fn operation(arguments: &[String]) -> Result<(), String> {
    if arguments.len() == 1 && matches!(arguments[0].as_str(), "-h" | "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    let mode = arguments.first().map(String::as_str).ok_or(USAGE)?;
    if !matches!(mode, "--build-source" | "--assemble" | "--c-spans") {
        return Err(format!("unknown option: {mode}\n{USAGE}"));
    }
    if !(arguments.len() == 2 || arguments.len() == 3) {
        return Err(USAGE.into());
    }
    if arguments.get(2).is_some_and(|base| base.starts_with('-')) {
        return Err(format!("base must be hex\n{USAGE}"));
    }
    let base = base_argument(arguments)?;
    let path = &arguments[1];
    match mode {
        "--build-source" => print!(
            "{}",
            crate::disasm::build_overlay_source_from_file(Path::new(path), base)?
        ),
        "--assemble" => io::stdout()
            .write_all(&crate::assemble_overlay(
                &crate::OverlaySource::path(path.clone()),
                base,
            )?)
            .map_err(|error| error.to_string())?,
        "--c-spans" => {
            let source = crate::OverlaySource::path(path.clone());
            for span in crate::overlay_c_spans(&source, base)? {
                println!("{},{}", span.start, span.end);
            }
        }
        _ => unreachable!(),
    }
    Ok(())
}
pub fn entry(arguments: &[String]) -> ExitCode {
    match operation(arguments) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
    }
}
