use crate::{build_executable_gap_data, build_section};
use std::io::Write;
use std::path::Path;
use std::process::ExitCode;

const USAGE: &str = "usage: executable-gap-sources build-stdout INDEX [ADDRESS]";

pub fn entry(args: &[String]) -> ExitCode {
    let result = (|| -> Result<(), String> {
        if args.len() == 1 && matches!(args[0].as_str(), "-h" | "--help") {
            println!("{USAGE}");
            return Ok(());
        }
        if args.first().map(String::as_str) != Some("build-stdout")
            || args.len() < 2
            || args.len() > 3
        {
            return Err(USAGE.into());
        }
        let path = Path::new(&args[1]);
        let sections = build_executable_gap_data(path)?;
        let data = if let Some(text) = args.get(2) {
            let address =
                u64::from_str_radix(text.strip_prefix("0x").ok_or("invalid address")?, 16)
                    .map_err(|_| "invalid address".to_string())?;
            build_section(path, address)?
        } else if sections.len() == 1 {
            sections[0].data.clone()
        } else {
            return Err(
                "build-stdout requires ADDRESS when the package has multiple typed sections".into(),
            );
        };
        std::io::stdout()
            .write_all(&data)
            .map_err(|e| e.to_string())
    })();
    match result {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
