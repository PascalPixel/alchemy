use map_container_components::{
    build_blend_animation, build_descriptors, build_header, build_metatiles, build_queues,
    build_sparse, export_blend_animation, export_descriptors, export_header, export_metatiles,
    export_queues, export_sparse, self_test, Result,
};
use std::path::Path;
use std::io::{self, Write};
use std::process::ExitCode;

const USAGE: &str = "usage: map-container-components [--self-test] <export|build|verify> <header|metatiles|descriptors|queues|blend|sparse> ...";

fn option(args: &[String], name: &str) -> Result<String> {
    let index = args
        .iter()
        .position(|arg| arg == name)
        .ok_or_else(|| format!("{name} is required"))?;
    args.get(index + 1)
        .cloned()
        .ok_or_else(|| format!("{name} is required"))
}

fn kind(args: &[String]) -> Result<&str> {
    args.get(1)
        .map(String::as_str)
        .ok_or_else(|| USAGE.to_string())
}

fn verify_bytes(actual: &[u8], expected: &Path) -> Result<()> {
    let expected =
        std::fs::read(expected).map_err(|error| format!("{}: {error}", expected.display()))?;
    if actual != expected {
        return Err("component bytes differ".into());
    }
    println!("identical=true bytes={}", actual.len());
    Ok(())
}

fn run(mut args: Vec<String>) -> Result<()> {
    if args.iter().any(|arg| arg == "-h" || arg == "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    if args.iter().any(|arg| arg == "--self-test") {
        self_test()?;
        args.retain(|arg| arg != "--self-test");
        if args.is_empty() {
            println!("self-test=ok");
            return Ok(());
        }
    }
    let command = args
        .first()
        .map(String::as_str)
        .ok_or_else(|| USAGE.to_string())?;
    let component = kind(&args)?;
    match command {
        "export" => {
            let input = option(&args, "--input")?;
            let source = option(&args, "--source")?;
            let plan = args
                .iter()
                .position(|arg| arg == "--plan")
                .map(|index| args.get(index + 1).cloned())
                .flatten()
                .ok_or_else(|| "--plan is required for compressed components".to_string());
            match component {
                "header" => {
                    let offsets = export_header(
                        &std::fs::read(&input).map_err(|e| e.to_string())?,
                        Path::new(&source),
                    )?;
                    println!("offsets={}", offsets.len());
                }
                "metatiles" => {
                    let (count, mode) = export_metatiles(
                        &std::fs::read(&input).map_err(|e| e.to_string())?,
                        Path::new(&source),
                        Path::new(&plan?),
                    )?;
                    println!("metatiles={} mode={}", count, mode);
                }
                "descriptors" => println!(
                    "records={}",
                    export_descriptors(
                        &std::fs::read(&input).map_err(|e| e.to_string())?,
                        Path::new(&source),
                        Path::new(&plan?)
                    )?
                ),
                "queues" => {
                    let (count, commands) = export_queues(
                        &std::fs::read(&input).map_err(|e| e.to_string())?,
                        Path::new(&source),
                        Path::new(&plan?),
                    )?;
                    println!("queues={} commands={}", count, commands);
                }
                "blend" => {
                    let (count, codec) = export_blend_animation(
                        &std::fs::read(&input).map_err(|e| e.to_string())?,
                        Path::new(&source),
                        Path::new(&plan?),
                    )?;
                    println!("commands={} codec={}", count, codec);
                }
                "sparse" => println!(
                    "records={}",
                    export_sparse(
                        &std::fs::read(&input).map_err(|e| e.to_string())?,
                        Path::new(&source)
                    )?
                ),
                _ => return Err(USAGE.into()),
            }
        }
        "build" | "build-stdout" | "verify" => {
            let source = option(&args, "--source")?;
            let output = args
                .iter()
                .position(|arg| arg == "--output")
                .and_then(|index| args.get(index + 1))
                .cloned();
            let plan = args
                .iter()
                .position(|arg| arg == "--plan")
                .and_then(|index| args.get(index + 1))
                .cloned();
            let built = match component {
                "header" => build_header(Path::new(&source), None)?,
                "metatiles" => build_metatiles(
                    Path::new(&source),
                    Path::new(&plan.ok_or("--plan is required")?),
                )?,
                "descriptors" => build_descriptors(
                    Path::new(&source),
                    Path::new(&plan.ok_or("--plan is required")?),
                )?,
                "queues" => build_queues(
                    Path::new(&source),
                    Path::new(&plan.ok_or("--plan is required")?),
                )?,
                "blend" => build_blend_animation(
                    Path::new(&source),
                    Path::new(&plan.ok_or("--plan is required")?),
                )?,
                "sparse" => build_sparse(Path::new(&source))?,
                _ => return Err(USAGE.into()),
            };
            if command == "build-stdout" {
                io::stdout().write_all(&built).map_err(|e| e.to_string())?;
            } else if command == "verify" {
                verify_bytes(&built, Path::new(&option(&args, "--expected")?))?;
            } else {
                let output = output.ok_or("--output is required")?;
                std::fs::write(&output, &built).map_err(|e| e.to_string())?;
                println!("bytes={}", built.len());
            }
        }
        _ => return Err(USAGE.into()),
    }
    Ok(())
}

fn main() -> ExitCode {
    match run(std::env::args().skip(1).collect()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(message) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}
