//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

use std::path::{Path, PathBuf};
use std::process::ExitCode;

fn repository_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .unwrap()
        .to_path_buf()
}

fn default_source() -> PathBuf {
    repository_root().join("assets/data/gba_header.json")
}

fn option(args: &[String], names: &[&str]) -> Result<String, String> {
    let index = args
        .iter()
        .position(|argument| names.contains(&argument.as_str()));
    match index {
        Some(index) if index + 1 < args.len() => Ok(args[index + 1].clone()),
        _ => Err(format!("{} is required", names.last().unwrap())),
    }
}

fn usage() -> ExitCode {
    println!(
        "usage: gba-header export ROM (--output FILE | --directory DIR) [--template FILE] | \
build SOURCE --output FILE | verify ROM SOURCE | --self-test"
    );
    ExitCode::SUCCESS
}

fn validate_options(args: &[String]) -> Result<(), String> {
    let valued = ["-o", "--output", "--directory", "--template"];
    let mut index = 0;
    while index < args.len() {
        let argument = args[index].as_str();
        if valued.contains(&argument) {
            if index + 1 >= args.len() {
                return Err(format!("{argument} requires a value"));
            }
            index += 2;
            continue;
        }
        if matches!(argument, "-h" | "--help" | "--self-test") {
            index += 1;
            continue;
        }
        if argument.starts_with('-') {
            return Err(format!("unknown option: {argument}"));
        }
        index += 1;
    }
    Ok(())
}

fn run(args: &[String]) -> Result<ExitCode, String> {
    validate_options(args)?;
    if args.iter().any(|arg| arg == "-h" || arg == "--help") {
        return Ok(usage());
    }
    if args.len() == 1 && args[0] == "--self-test" {
        println!("{}", crate::self_test()?);
        return Ok(ExitCode::SUCCESS);
    }
    if args.first().is_some_and(|arg| arg == "export") && args.get(1).is_some() {
        let rom_path = Path::new(&args[1]);
        let output = if args.iter().any(|arg| arg == "--directory") {
            PathBuf::from(option(args, &["--directory"])?).join("gba_header.json")
        } else {
            PathBuf::from(option(args, &["-o", "--output"])?)
        };
        let template = if args.iter().any(|arg| arg == "--template") {
            PathBuf::from(option(args, &["--template"])?)
        } else {
            default_source()
        };
        let source = crate::export_gba_header(
            &std::fs::read(rom_path).map_err(|error| error.to_string())?,
            &output,
            &template,
            Some(rom_path),
        )?;
        let target = source["edition"]["entry_branch"]["target"]
            .as_str()
            .unwrap();
        println!("target={target} bytes=192 exact=true");
        return Ok(ExitCode::SUCCESS);
    }
    if args.first().is_some_and(|arg| arg == "build") && args.get(1).is_some() {
        let source_path = Path::new(&args[1]);
        let source = crate::read_gba_header_source(source_path)?;
        let root = repository_root();
        let logo_source = root.join(source["standard"]["logo"]["source"].as_str().unwrap());
        let built = crate::build_gba_header(
            &source,
            &std::fs::read(logo_source).map_err(|error| error.to_string())?,
        )?;
        let output = PathBuf::from(option(args, &["-o", "--output"])?);
        if let Some(parent) = output.parent() {
            std::fs::create_dir_all(parent).map_err(|error| error.to_string())?;
        }
        std::fs::write(&output, &built).map_err(|error| error.to_string())?;
        println!("address=0x08000000 bytes={}", built.len());
        return Ok(ExitCode::SUCCESS);
    }
    if args.first().is_some_and(|arg| arg == "verify") && args.len() == 3 {
        let built = crate::verify_gba_header(
            &std::fs::read(&args[1]).map_err(|error| error.to_string())?,
            Path::new(&args[2]),
        )?;
        println!("address=0x08000000 bytes={} exact=true", built.len());
        return Ok(ExitCode::SUCCESS);
    }
    Ok(usage())
}

pub fn entry(arguments: &[String]) -> std::process::ExitCode {
    match run(&arguments.to_vec()) {
        Ok(code) => code,
        Err(message) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn args(values: &[&str]) -> Vec<String> {
        values.iter().map(|value| (*value).to_string()).collect()
    }

    #[test]
    fn help_aliases_succeed_before_file_access() {
        assert_eq!(run(&args(&["-h"])).unwrap(), ExitCode::SUCCESS);
        assert_eq!(run(&args(&["--help"])).unwrap(), ExitCode::SUCCESS);
    }

    #[test]
    fn unknown_option_is_rejected_before_file_access() {
        assert_eq!(
            run(&args(&["verify", "missing.gba", "missing.json", "--bogus"])).unwrap_err(),
            "unknown option: --bogus"
        );
    }
}
