use skip_sprite_archive::{build_archive, export_archive, self_test, Error};
use std::fs;
use std::io::{self, Write};
use std::path::Path;
use std::process::ExitCode;

const USAGE: &str = "usage: skip-sprite-archive [--self-test] [--decoded FILE --images DIR --plan FILE --palette FILE] [--preview FILE] [--width N] [--height N]";

fn option(args: &[String], name: &str) -> Result<String, Error> {
    let index = args
        .iter()
        .position(|arg| arg == name)
        .ok_or_else(|| Error(format!("{name} is required")))?;
    args.get(index + 1)
        .cloned()
        .ok_or_else(|| Error(format!("{name} is required")))
}

fn optional(args: &[String], name: &str) -> Option<String> {
    args.iter()
        .position(|arg| arg == name)
        .and_then(|index| args.get(index + 1))
        .cloned()
}

fn number(args: &[String], name: &str, default: usize) -> Result<usize, Error> {
    match optional(args, name) {
        None => Ok(default),
        Some(text) => {
            let parsed = text
                .strip_prefix("0x")
                .or_else(|| text.strip_prefix("0X"))
                .map(|digits| usize::from_str_radix(digits, 16))
                .unwrap_or_else(|| text.parse::<usize>());
            parsed.map_err(|_| Error(format!("{name} must be an integer")))
        }
    }
}

fn run(args: &[String]) -> Result<(), Error> {
    if args.iter().any(|arg| arg == "-h" || arg == "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    if args.iter().any(|arg| arg == "--self-test") {
        self_test()?;
        println!("self-test=ok");
        if optional(args, "--decoded").is_none() {
            return Ok(());
        }
    }
    if args.first().map(String::as_str) == Some("build-stdout") {
        let directory = args.get(1).ok_or_else(|| Error(USAGE.into()))?;
        let plan_path = args.get(2).ok_or_else(|| Error(USAGE.into()))?;
        let palette = args.get(3).ok_or_else(|| Error(USAGE.into()))?;
        let plan: serde_json::Value = serde_json::from_slice(
            &fs::read(plan_path).map_err(|error| Error(error.to_string()))?,
        )
        .map_err(|error| Error(error.to_string()))?;
        let built = build_archive(&plan, Path::new(directory), Path::new(palette))?;
        io::stdout().write_all(&built).map_err(|error| Error(error.to_string()))?;
        return Ok(());
    }
    let decoded = option(args, "--decoded")?;
    let images = option(args, "--images")?;
    let plan = option(args, "--plan")?;
    let palette = option(args, "--palette")?;
    let preview = optional(args, "--preview");
    let width = number(args, "--width", 64)?;
    let height = number(args, "--height", 64)?;
    let bytes = fs::read(&decoded).map_err(|error| Error(error.to_string()))?;
    let count = export_archive(
        &bytes,
        Path::new(&images),
        Path::new(&plan),
        Path::new(&palette),
        preview.as_deref().map(Path::new),
        width,
        height,
    )?;
    println!("images={count} decoded={}", bytes.len());
    Ok(())
}

fn main() -> ExitCode {
    match run(&std::env::args().skip(1).collect::<Vec<_>>()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(Error(message)) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}
