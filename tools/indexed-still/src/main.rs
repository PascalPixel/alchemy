use indexed_still::{export_series, read_still_index, self_test, Error};
use std::path::{Path, PathBuf};
use std::io::{self, Write};
use std::process::ExitCode;

const USAGE: &str =
    "usage: indexed_still.ts [--self-test] export-series ROM [--index FILE] [--directory DIR]";

fn optional_option(args: &[String], names: &[&str]) -> Result<Option<String>, Error> {
    match args
        .iter()
        .position(|argument| names.contains(&argument.as_str()))
    {
        Some(index) => args
            .get(index + 1)
            .cloned()
            .map(Some)
            .ok_or_else(|| Error(format!("{} is required", names[names.len() - 1]))),
        None => Ok(None),
    }
}

fn default_index() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .unwrap()
        .join("assets/graphics/backgrounds_index.json")
}

fn run(mut args: Vec<String>) -> Result<(), Error> {
    if args
        .iter()
        .any(|argument| argument == "-h" || argument == "--help")
    {
        println!("{USAGE}");
        return Ok(());
    }
    if args.iter().any(|argument| argument == "--self-test") {
        self_test()?;
        println!("self-test=ok");
        read_still_index(&default_index())?;
        args.retain(|argument| argument != "--self-test");
        if args.is_empty() {
            return Ok(());
        }
    }
    if args.first().map(String::as_str) == Some("build-stdout") {
        let source = args.get(1).ok_or_else(|| Error("build-stdout requires an image".into()))?;
        let image = std::fs::read(source).map_err(|e| Error(e.to_string()))?;
        let (built, _) = indexed_still::build_still(&image)?;
        io::stdout().write_all(&built).map_err(|e| Error(e.to_string()))?;
        return Ok(());
    }
    if args.first().map(String::as_str) != Some("export-series") || args.get(1).is_none() {
        return Err(Error(
            "an indexed-still command and ROM are required".into(),
        ));
    }
    let rom_path = Path::new(args.get(1).unwrap());
    let index_path = optional_option(&args, &["--index"])?
        .map(PathBuf::from)
        .unwrap_or_else(default_index);
    let directory = optional_option(&args, &["--directory"])?
        .map(PathBuf::from)
        .unwrap_or_else(|| {
            index_path
                .parent()
                .unwrap_or_else(|| Path::new("."))
                .to_path_buf()
        });
    let bytes = export_series(rom_path, &index_path, &directory)?;
    println!("images=34 bytes={bytes}");
    Ok(())
}

fn main() -> ExitCode {
    match run(std::env::args().skip(1).collect()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(Error(message)) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}
