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
        "usage: gba_header.ts export ROM (--output FILE | --directory DIR) [--template FILE] | \
build SOURCE --output FILE | verify ROM SOURCE | --self-test"
    );
    ExitCode::SUCCESS
}

fn run(args: &[String]) -> Result<ExitCode, String> {
    if args.len() == 1 && args[0] == "--self-test" {
        println!("{}", gba_header::self_test()?);
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
        let source = gba_header::export_gba_header(
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
        let source = gba_header::read_gba_header_source(source_path)?;
        let root = repository_root();
        let logo_source = root.join(source["standard"]["logo"]["source"].as_str().unwrap());
        let built = gba_header::build_gba_header(
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
        let built = gba_header::verify_gba_header(
            &std::fs::read(&args[1]).map_err(|error| error.to_string())?,
            Path::new(&args[2]),
        )?;
        println!("address=0x08000000 bytes={} exact=true", built.len());
        return Ok(ExitCode::SUCCESS);
    }
    if args.iter().any(|arg| arg == "-h" || arg == "--help") {
        return Ok(usage());
    }
    Ok(usage())
}

fn main() -> ExitCode {
    match run(&std::env::args().skip(1).collect::<Vec<_>>()) {
        Ok(code) => code,
        Err(message) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}
