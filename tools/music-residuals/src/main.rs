use std::io::Write;
use std::path::Path;
use std::process::ExitCode;

use music_residuals::{build_region, self_test, verify_music_residuals, Result};

const USAGE: &str =
    "usage: music_residuals.ts build-stdout INDEX ADDRESS | verify ROM --index INDEX | --self-test";

fn run(args: &[String]) -> Result<()> {
    if args == ["--self-test"] {
        self_test()?;
        println!("self-test=ok");
        return Ok(());
    }
    if args.len() == 3 && args[0] == "build-stdout" {
        let address = {
            let text = args[2].strip_prefix("0x").unwrap_or(&args[2]);
            u32::from_str_radix(text, 16)
                .map_err(|_| "invalid music residual address".to_string())?
        };
        let bytes = build_region(Path::new(&args[1]), address)?;
        std::io::stdout()
            .write_all(&bytes)
            .map_err(|error| error.to_string())?;
        return Ok(());
    }
    if args.len() == 4 && args[0] == "verify" && args[2] == "--index" {
        println!(
            "{}",
            verify_music_residuals(Path::new(&args[1]), Path::new(&args[3]))?
        );
        return Ok(());
    }
    if args == ["-h"] || args == ["--help"] {
        println!("{USAGE}");
        return Ok(());
    }
    Err(USAGE.into())
}

fn main() -> ExitCode {
    match run(&std::env::args().skip(1).collect::<Vec<_>>()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(message) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}
