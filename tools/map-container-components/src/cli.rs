use crate::{
    build_blend_animation, build_descriptors, build_header, build_metatiles, build_queues, build_sparse, Result,
};
use std::io::{self, Write};
use std::path::Path;
use std::process::ExitCode;
const USAGE: &str = "usage: map-container-components build-stdout KIND --source FILE [--plan FILE]";
fn option(args: &[String], name: &str) -> Result<String> {
    let index = args.iter().position(|arg| arg == name).ok_or_else(|| format!("{name} is required"))?;
    args.get(index + 1).cloned().ok_or_else(|| format!("{name} is required"))
}
fn build(args: &[String]) -> Result<Vec<u8>> {
    if args.len() < 4 || args[0] != "build-stdout" {
        return Err(USAGE.into());
    }
    let source = option(args, "--source")?;
    let plan = args.iter().position(|arg| arg == "--plan").and_then(|index| args.get(index + 1)).map(Path::new);
    match args[1].as_str() {
        "header" => build_header(Path::new(&source), None),
        "metatiles" => build_metatiles(Path::new(&source), plan.ok_or("--plan is required")?),
        "descriptors" => build_descriptors(Path::new(&source), plan.ok_or("--plan is required")?),
        "queues" => build_queues(Path::new(&source), plan.ok_or("--plan is required")?),
        "blend" => build_blend_animation(Path::new(&source), plan.ok_or("--plan is required")?),
        "sparse" => build_sparse(Path::new(&source)),
        _ => Err(USAGE.into()),
    }
}
pub fn entry(arguments: &[String]) -> ExitCode {
    if arguments.is_empty() || arguments.iter().any(|arg| arg == "-h" || arg == "--help") {
        println!("{USAGE}");
        return if arguments.is_empty() { ExitCode::from(2) } else { ExitCode::SUCCESS };
    }
    match build(arguments).and_then(|bytes| io::stdout().write_all(&bytes).map_err(|e| e.to_string())) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
