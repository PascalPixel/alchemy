use f0_archive::{build_archive, extract_archive, self_test, verify_archive, Error};
use std::fs;
use std::io::{self, Write};
use std::path::Path;
use std::process::ExitCode;

const USAGE: &str = "usage: f0_archive.ts export ROM --address N --input-end N --plan FILE --images BASE [--preview FILE] | verify ROM --address N --input-end N --plan FILE --images BASE | --self-test";

fn option(args: &[String], name: &str) -> Result<String, Error> {
    let index = args
        .iter()
        .position(|arg| arg == name)
        .ok_or_else(|| Error(format!("{name} is required")))?;
    args.get(index + 1)
        .cloned()
        .ok_or_else(|| Error(format!("{name} is required")))
}

fn number(args: &[String], name: &str) -> Result<usize, Error> {
    let text = option(args, name)?;
    let value = if let Some(hex) = text.strip_prefix("0x").or_else(|| text.strip_prefix("0X")) {
        usize::from_str_radix(hex, 16)
    } else {
        text.parse::<usize>()
    };
    value.map_err(|_| Error(format!("{name} must be an integer")))
}

fn run(args: &[String]) -> Result<(), Error> {
    if args.len() == 1 && args[0] == "--self-test" {
        self_test()?;
        println!("self-test=ok");
        return Ok(());
    }
    if args.len() == 1 && matches!(args[0].as_str(), "-h" | "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    if args.first().map(String::as_str) == Some("build-stdout") {
        let plan_path = args.get(1).ok_or_else(|| Error(USAGE.into()))?;
        let directory = args.get(2).ok_or_else(|| Error(USAGE.into()))?;
        let plan: serde_json::Value = serde_json::from_slice(
            &fs::read(plan_path).map_err(|e| Error(e.to_string()))?,
        )
        .map_err(|e| Error(e.to_string()))?;
        let built = build_archive(&plan, Path::new(directory))?;
        io::stdout().write_all(&built).map_err(|e| Error(e.to_string()))?;
        return Ok(());
    }
    let (mode, rest) = if args
        .first()
        .is_some_and(|arg| arg == "export" || arg == "verify")
    {
        (args[0].as_str(), &args[1..])
    } else {
        ("export", args)
    };
    let rom_path = rest.first().ok_or_else(|| Error(USAGE.into()))?;
    let address = number(rest, "--address")?;
    let input_end = number(rest, "--input-end")?;
    let plan = option(rest, "--plan")?;
    let images = option(rest, "--images")?;
    let rom = fs::read(rom_path).map_err(|e| Error(e.to_string()))?;
    if mode == "verify" {
        let bytes = verify_archive(
            &rom,
            address,
            input_end,
            Path::new(&plan),
            Path::new(&images),
        )?;
        println!("identical=true bytes={bytes}");
        return Ok(());
    }
    let preview = rest
        .iter()
        .position(|arg| arg == "--preview")
        .map(|index| {
            rest.get(index + 1)
                .cloned()
                .ok_or_else(|| Error("--preview requires a path".into()))
        })
        .transpose()?;
    let (entries, images_count, encoded) = extract_archive(
        &rom,
        address,
        input_end,
        Path::new(&plan),
        Path::new(&images),
        preview.as_deref().map(Path::new),
    )?;
    println!("entries={entries} images={images_count} encoded={encoded}");
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
