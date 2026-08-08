use simple_resources::{build_simple_resource, export_simple_resources, self_test, verify_simple_resources, Error};
use std::io::{self, Write};
use std::path::Path;
use std::process::ExitCode;

const USAGE: &str =
    "usage: simple_resources.ts [--self-test] {export|verify} ROM --directory ASSETS";
fn option(args: &[String], name: &str) -> Result<String, Error> {
    let i = args
        .iter()
        .position(|x| x == name)
        .ok_or_else(|| Error(format!("{name} is required")))?;
    args.get(i + 1)
        .cloned()
        .ok_or_else(|| Error(format!("{name} is required")))
}
fn run(mut args: Vec<String>) -> Result<(), Error> {
    if args.iter().any(|x| x == "-h" || x == "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    if args.iter().any(|x| x == "--self-test") {
        self_test()?;
        println!("self-test=ok");
        args.retain(|x| x != "--self-test");
        if args.is_empty() {
            return Ok(());
        }
    }
    let command = args
        .first()
        .map(String::as_str)
        .ok_or_else(|| Error("export or verify and a ROM are required".into()))?;
    if command == "build-stdout" {
        let root = args
            .get(1)
            .ok_or_else(|| Error("build-stdout requires an assets root".into()))?;
        let id = args
            .get(2)
            .ok_or_else(|| Error("build-stdout requires a resource id".into()))?
            .trim_start_matches("0x");
        let id = u32::from_str_radix(id, 16).map_err(|_| Error("invalid resource id".into()))?;
        let bytes = build_simple_resource(id, Path::new(root))?;
        io::stdout().write_all(&bytes).map_err(|e| Error(e.to_string()))?;
        return Ok(());
    }
    let rom = args
        .get(1)
        .ok_or_else(|| Error("export or verify and a ROM are required".into()))?;
    if !matches!(command, "export" | "verify") {
        return Err(Error("export or verify and a ROM are required".into()));
    }
    let root = option(&args, "--directory")?;
    let rom = Path::new(rom);
    let root = Path::new(&root);
    if simple_resources::same_paths(rom, root) {
        return Err(Error("refusing to overwrite the input ROM".into()));
    }
    let line = if command == "export" {
        export_simple_resources(rom, root)?
    } else {
        verify_simple_resources(rom, root)?
    };
    println!("{line}");
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
