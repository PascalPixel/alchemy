use crate::{build_late_runtime_residual, SOURCE_BYTES};
use std::io::Write;
use std::path::{Path, PathBuf};
use std::process::ExitCode;

const USAGE: &str = "usage: late-runtime-residual build-region-stdout INDEX ADDRESS";

fn catalog_path() -> PathBuf {
    std::env::var_os("ALCHEMY_ROOT")
        .map(PathBuf::from)
        .unwrap_or_else(|| Path::new(env!("CARGO_MANIFEST_DIR")).ancestors().nth(2).unwrap().to_path_buf())
        .join("games/gs1/assets/data/late_runtime_catalog.json")
}

pub fn entry(args: &[String]) -> ExitCode {
    let result = (|| -> Result<(), String> {
        if args.len() == 1 && matches!(args[0].as_str(), "-h" | "--help") {
            println!("{USAGE}");
            return Ok(());
        }
        if args.len() != 3 || args[0] != "build-region-stdout" {
            return Err(USAGE.into());
        }
        let wanted = u32::from_str_radix(args[2].strip_prefix("0x").ok_or("invalid address")?, 16)
            .map_err(|_| "invalid address".to_string())?;
        let built = build_late_runtime_residual(Path::new(&args[1]), &catalog_path())?;
        let (_, data) = built
            .regions
            .iter()
            .find(|(address, _)| *address == wanted)
            .ok_or("late-runtime asset address is not a produced region")?;
        if built.source_bytes != SOURCE_BYTES {
            return Err("late residual source-byte total differs".into());
        }
        eprintln!("{{\"source_bytes\":{}}}", built.source_bytes);
        std::io::stdout().write_all(data).map_err(|e| e.to_string())
    })();
    match result {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
