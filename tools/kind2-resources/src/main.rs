use kind2_resources::{
    self_test, verify_kind2_resource, verify_kind2_series, write_build_stdout, write_encode_stdout,
    Result,
};
use std::path::Path;
use std::process::ExitCode;

const USAGE: &str = "usage: kind2_resources [--self-test] build-stdout PLAN | encode-stdout PLAN | verify ROM --index INDEX | verify-resource ROM --plan PLAN";
fn option(args: &[String], name: &str) -> Result<String> {
    let index = args
        .iter()
        .position(|arg| arg == name)
        .ok_or_else(|| format!("{name} is required"))?;
    args.get(index + 1)
        .cloned()
        .ok_or_else(|| format!("{name} is required"))
}
fn run(mut args: Vec<String>) -> Result<()> {
    if args.iter().any(|arg| arg == "-h" || arg == "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    if args.iter().any(|arg| arg == "--self-test") {
        self_test()?;
        println!("self-test=ok");
        args.retain(|arg| arg != "--self-test");
        if args.is_empty() {
            return Ok(());
        }
    }
    match args.first().map(String::as_str) {
        Some("build-stdout") => write_build_stdout(Path::new(
            args.get(1)
                .ok_or_else(|| "build-stdout requires a plan".to_string())?,
        )),
        Some("encode-stdout") => write_encode_stdout(Path::new(
            args.get(1)
                .ok_or_else(|| "encode-stdout requires a plan".to_string())?,
        )),
        Some("verify") => {
            let rom = args.get(1).ok_or_else(|| USAGE.to_string())?;
            println!(
                "{}",
                verify_kind2_series(Path::new(rom), Path::new(&option(&args, "--index")?))?
            );
            Ok(())
        }
        Some("verify-resource") => {
            let rom = args.get(1).ok_or_else(|| USAGE.to_string())?;
            println!(
                "{}",
                verify_kind2_resource(Path::new(rom), Path::new(&option(&args, "--plan")?))?
            );
            Ok(())
        }
        _ => Err(USAGE.into()),
    }
}
fn main() -> ExitCode {
    match run(std::env::args().skip(1).collect()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
