use resource_01c::{
    export_resource_01c, self_test, verify_resource_01c, Error, GLYPHS, RESOURCE_SIZE,
};
use std::path::Path;
use std::io::{self, Write};
use std::process::ExitCode;

const USAGE: &str =
    "usage: resource_01c export ROM --directory DIR | verify ROM --plan PLAN | --self-test";

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
    if args.len() == 4 && args[0] == "export" && args[2] == "--directory" {
        export_resource_01c(Path::new(&args[1]), Path::new(&args[3]))?;
        println!("glyphs={GLYPHS} source_bytes={RESOURCE_SIZE}");
        return Ok(());
    }
    if args.len() == 4 && args[0] == "verify" && args[2] == "--plan" {
        println!(
            "{}",
            verify_resource_01c(Path::new(&args[1]), Path::new(&args[3]))?
        );
        return Ok(());
    }
    if args.len() == 2 && args[0] == "build-stdout" {
        let (bytes, _) = resource_01c::build_resource_01c(Path::new(&args[1]))?;
        io::stdout().write_all(&bytes).map_err(|e| Error(e.to_string()))?;
        return Ok(());
    }
    Err(Error(USAGE.into()))
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
