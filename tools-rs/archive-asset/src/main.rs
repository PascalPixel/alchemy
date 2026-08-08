use std::fs;
use std::path::Path;
use std::process::ExitCode;

use archive_asset::{extract_archive, plan_json, self_test, PixelFormat, ROM_BASE};
use export_asset::js_number;

const USAGE: &str = "usage: archive_asset.ts [--self-test] ROM --address ADDRESS --input-end END --chunk-width WIDTH --chunk-height HEIGHT --columns COLUMNS --plan PLAN --atlas ATLAS [--source-end END] [options]";

fn option(args: &[String], name: &str) -> Result<String, String> {
    match args.iter().position(|argument| argument == name) {
        Some(index) if index + 1 < args.len() => Ok(args[index + 1].clone()),
        _ => Err(format!("{name} is required")),
    }
}

fn integer(args: &[String], name: &str, fallback: Option<usize>) -> Result<usize, String> {
    if !args.iter().any(|argument| argument == name) {
        return fallback.ok_or_else(|| format!("{name} is required"));
    }
    let text = option(args, name)?;
    let value = js_number(&text);
    if !value.is_finite() || value.fract() != 0.0 || value < 0.0 {
        return Err(format!("{name} must be an integer"));
    }
    if value > usize::MAX as f64 {
        return Err(format!("{name} must be an integer"));
    }
    Ok(value as usize)
}

fn pixel_format(args: &[String]) -> Result<PixelFormat, String> {
    let text = if args.iter().any(|argument| argument == "--pixel-format") {
        option(args, "--pixel-format")?
    } else {
        "rgba".to_string()
    };
    match text.as_str() {
        "rgba" => Ok(PixelFormat::Rgba),
        "indexed8" => Ok(PixelFormat::Indexed8),
        _ => Err("unsupported archive pixel format".to_string()),
    }
}

fn run(mut args: Vec<String>) -> Result<(), String> {
    if args.iter().any(|argument| argument == "--self-test") {
        self_test().map_err(|error| error.0)?;
        println!("self-test=ok");
        args.retain(|argument| argument != "--self-test");
        if args.is_empty() {
            return Ok(());
        }
    }
    if args
        .iter()
        .any(|argument| argument == "-h" || argument == "--help")
    {
        println!("{USAGE}");
        return Ok(());
    }
    let rom_path = args.first().ok_or_else(|| "ROM is required".to_string())?;
    let address = integer(&args, "--address", None)?;
    let input_end = integer(&args, "--input-end", None)?;
    let source_end = integer(&args, "--source-end", Some(input_end))?;
    let chunk_width = integer(&args, "--chunk-width", None)?;
    let chunk_height = integer(&args, "--chunk-height", None)?;
    let columns = integer(&args, "--columns", None)?;
    let format = pixel_format(&args)?;
    let offset_width = integer(&args, "--offset-width", Some(4))?;
    let stream_alignment = integer(&args, "--stream-alignment", Some(1))?;
    if !matches!(offset_width, 2 | 4) {
        return Err("offset width must be 2 or 4".to_string());
    }
    let plan_path = option(&args, "--plan")?;
    let atlas_path = option(&args, "--atlas")?;
    let rom = fs::read(rom_path).map_err(|error| error.to_string())?;
    let base = ROM_BASE as usize;
    if address < base || input_end < base || source_end < base {
        return Err("archive address lies below ROM base".to_string());
    }
    let address = address - base;
    let input_end = input_end - base;
    let source_end = source_end - base;
    let (plan, atlas, count) = extract_archive(
        &rom,
        address,
        input_end,
        source_end,
        chunk_width,
        chunk_height,
        columns,
        format,
        offset_width,
        stream_alignment,
    )
    .map_err(|error| error.0)?;
    let plan_parent = Path::new(&plan_path).parent().unwrap_or(Path::new("."));
    let atlas_parent = Path::new(&atlas_path).parent().unwrap_or(Path::new("."));
    fs::create_dir_all(plan_parent).map_err(|error| error.to_string())?;
    fs::create_dir_all(atlas_parent).map_err(|error| error.to_string())?;
    fs::write(&plan_path, format!("{}\n", plan_json(&plan))).map_err(|error| error.to_string())?;
    fs::write(&atlas_path, atlas).map_err(|error| error.to_string())?;
    println!(
        "streams={} decoded={} encoded={} alignment={}",
        count,
        plan.streams
            .iter()
            .map(|stream| stream.decoded_size)
            .sum::<usize>(),
        source_end - address,
        input_end - source_end,
    );
    Ok(())
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
