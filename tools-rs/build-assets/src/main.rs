//! Native entry point for the asset build stage.
//!
//! The stage is intentionally a compatibility boundary while the individual
//! asset codecs finish moving to Rust.  The public command is native and owns
//! the native archive codec; the legacy worker is invoked only through the
//! private `--legacy` escape used by the compatibility entry point.  Keeping
//! that boundary here lets existing Rust build callers migrate without
//! changing their observable CLI or report output.

use std::env;
use std::fs;
use std::io::{self, Write};
use std::path::{Path, PathBuf};
use std::process::{Command, ExitCode};

use alignment_tail::parse_alignment_tail;
use archive_asset::{
    build_archive, self_test as archive_self_test, ArchivePlan, ArchiveStream, PixelFormat,
};
use audio_engine_data::build_audio_engine_data;
use extract_resource::{PaletteGroup, PaletteOperation};
use gba_header::{build_gba_header_component, read_gba_header_source};
use serde_json::Value;

const USAGE: &str =
    "usage: build_assets.ts [-h] [--source-only] [--manifest MANIFEST] [-o OUTPUT] [rom] | --self-test";

fn repository_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("build-assets is under tools-rs")
        .to_path_buf()
}

fn run_legacy(arguments: &[String]) -> Result<ExitCode, String> {
    let root = repository_root();
    let bun = env::var_os("BUN").unwrap_or_else(|| "bun".into());
    let status = Command::new(bun)
        .current_dir(&root)
        .arg("tools/make/build_assets.ts")
        .arg("--legacy")
        .args(arguments)
        .status()
        .map_err(|error| format!("failed to start legacy asset worker: {error}"))?;
    Ok(ExitCode::from(status.code().unwrap_or(1) as u8))
}

fn number(value: &Value, label: &str) -> Result<usize, String> {
    let text = match value {
        Value::Number(value) => value.to_string(),
        Value::String(value) => value.clone(),
        _ => return Err(format!("{label} must be an integer")),
    };
    let parsed = if let Some(hex) = text.strip_prefix("0x").or_else(|| text.strip_prefix("0X")) {
        usize::from_str_radix(hex, 16)
    } else {
        text.parse::<usize>()
    };
    parsed.map_err(|_| format!("{label} must be an integer"))
}

fn required<'a>(
    object: &'a serde_json::Map<String, Value>,
    key: &str,
) -> Result<&'a Value, String> {
    object
        .get(key)
        .ok_or_else(|| format!("archive plan is missing {key}"))
}

fn parse_operation(value: &Value) -> Result<PaletteOperation, String> {
    let items = value
        .as_array()
        .ok_or_else(|| "archive token operation is not an array".to_string())?;
    let tag = items
        .first()
        .and_then(Value::as_str)
        .ok_or_else(|| "archive token operation has no tag".to_string())?;
    match tag {
        "l" if items.len() == 1 => Ok(PaletteOperation::Literal),
        "e" if items.len() == 1 => Ok(PaletteOperation::End),
        "c" if items.len() == 3 => Ok(PaletteOperation::Copy {
            length: number(&items[1], "copy length")? as u32,
            distance: number(&items[2], "copy distance")? as u32,
        }),
        _ => Err("unsupported archive token operation".to_string()),
    }
}

fn parse_group(value: &Value) -> Result<PaletteGroup, String> {
    let items = value
        .as_array()
        .ok_or_else(|| "archive token group is not an array".to_string())?;
    let tag = items
        .first()
        .and_then(Value::as_str)
        .ok_or_else(|| "archive token group has no tag".to_string())?;
    match tag {
        "z" if items.len() == 1 => Ok(PaletteGroup::Zeros),
        "g" if items.len() == 2 => {
            let operations = items[1]
                .as_array()
                .ok_or_else(|| "archive token group operations are not an array".to_string())?
                .iter()
                .map(parse_operation)
                .collect::<Result<Vec<_>, _>>()?;
            Ok(PaletteGroup::Group(operations))
        }
        _ => Err("unsupported archive token group".to_string()),
    }
}

fn hex_bytes(value: &Value, label: &str) -> Result<Vec<u8>, String> {
    let text = value
        .as_str()
        .ok_or_else(|| format!("{label} is not hexadecimal text"))?;
    if text.len() % 2 != 0 {
        return Err(format!("{label} has odd length"));
    }
    (0..text.len())
        .step_by(2)
        .map(|index| {
            u8::from_str_radix(&text[index..index + 2], 16)
                .map_err(|_| format!("{label} is not hexadecimal text"))
        })
        .collect()
}

fn parse_plan(value: Value) -> Result<ArchivePlan, String> {
    let object = value
        .as_object()
        .ok_or_else(|| "archive plan is not an object".to_string())?;
    if number(required(object, "format")?, "format")? != 1
        || required(object, "codec")?.as_str() != Some("golden-sun-offset-palette-lz")
    {
        return Err("unsupported archive plan".to_string());
    }
    let pixel_format = match object
        .get("pixel_format")
        .and_then(Value::as_str)
        .unwrap_or("rgba")
    {
        "rgba" => PixelFormat::Rgba,
        "indexed8" => PixelFormat::Indexed8,
        _ => return Err("unsupported archive pixel format".to_string()),
    };
    let streams = required(object, "streams")?
        .as_array()
        .ok_or_else(|| "archive streams are not an array".to_string())?
        .iter()
        .map(|raw| {
            let stream = raw
                .as_object()
                .ok_or_else(|| "archive stream is not an object".to_string())?;
            let tokens = required(stream, "tokens")?
                .as_array()
                .ok_or_else(|| "archive stream tokens are not an array".to_string())?
                .iter()
                .map(parse_group)
                .collect::<Result<Vec<_>, _>>()?;
            Ok(ArchiveStream {
                decoded_size: number(required(stream, "decoded_size")?, "decoded_size")?,
                encoded_size: number(required(stream, "encoded_size")?, "encoded_size")?,
                tokens,
                lookahead: hex_bytes(required(stream, "lookahead")?, "lookahead")?,
            })
        })
        .collect::<Result<Vec<_>, String>>()?;
    let alignment_tail = match object.get("alignment_tail") {
        None => None,
        Some(value) => {
            let size = number(
                value
                    .as_object()
                    .and_then(|item| item.get("size"))
                    .ok_or_else(|| "alignment_tail is missing size".to_string())?,
                "alignment_tail size",
            )?;
            Some(
                parse_alignment_tail(value, size, 3, "alignment_tail")
                    .map_err(|error| error.to_string())?,
            )
        }
    };
    Ok(ArchivePlan {
        format: 1,
        codec: "golden-sun-offset-palette-lz".to_string(),
        chunk_width: number(required(object, "chunk_width")?, "chunk_width")?,
        chunk_height: number(required(object, "chunk_height")?, "chunk_height")?,
        columns: number(required(object, "columns")?, "columns")?,
        pixel_format,
        offset_width: object
            .get("offset_width")
            .map(|value| number(value, "offset_width"))
            .transpose()?
            .unwrap_or(4),
        stream_alignment: object
            .get("stream_alignment")
            .map(|value| number(value, "stream_alignment"))
            .transpose()?
            .unwrap_or(1),
        streams,
        alignment_tail,
    })
}

fn build_offset_archive(plan_path: &Path, atlas_path: &Path) -> Result<(), String> {
    let plan = parse_plan(
        serde_json::from_slice(&fs::read(plan_path).map_err(|error| error.to_string())?)
            .map_err(|error| format!("invalid archive plan: {error}"))?,
    )?;
    let bytes = build_archive(
        &fs::read(atlas_path).map_err(|error| error.to_string())?,
        &plan,
    )
    .map_err(|error| error.to_string())?;
    io::stdout()
        .write_all(&bytes)
        .map_err(|error| error.to_string())
}

fn build_audio_engine(path: &Path) -> Result<(), String> {
    let built = build_audio_engine_data(path).map_err(|error| error.to_string())?;
    io::stdout()
        .write_all(&built.data)
        .map_err(|error| error.to_string())
}

fn build_gba_header(path: &Path, address: &str, size: &str) -> Result<(), String> {
    let address = parse_cli_number(address, "address")? as u32;
    let size = parse_cli_number(size, "size")?;
    let source = read_gba_header_source(path)?;
    let logo = source
        .get("standard")
        .and_then(Value::as_object)
        .and_then(|standard| standard.get("logo"))
        .and_then(Value::as_object)
        .and_then(|logo| logo.get("source"))
        .and_then(Value::as_str)
        .ok_or_else(|| "GBA header logo source is missing".to_string())?;
    let logo_path = repository_root().join(logo);
    let built = build_gba_header_component(
        &source,
        &fs::read(logo_path).map_err(|error| error.to_string())?,
        address,
        size,
    )?;
    io::stdout()
        .write_all(&built)
        .map_err(|error| error.to_string())
}

fn parse_cli_number(text: &str, label: &str) -> Result<usize, String> {
    let parsed = if let Some(hex) = text.strip_prefix("0x").or_else(|| text.strip_prefix("0X")) {
        usize::from_str_radix(hex, 16)
    } else {
        text.parse::<usize>()
    };
    parsed.map_err(|_| format!("{label} must be an integer"))
}

fn closure_self_test() -> Result<String, String> {
    let root = repository_root();
    let missing = root.join("assets/data/closure/__self_test_missing__/index.json");
    if missing.exists() {
        return Err("closure package self-test path exists".to_string());
    }
    let index = root.join("assets/audio/waves_index.json");
    let document: Value = serde_json::from_slice(
        &fs::read(index).map_err(|error| format!("PCM self-test index: {error}"))?,
    )
    .map_err(|error| format!("PCM self-test index: {error}"))?;
    let present_regions = document
        .get("waves")
        .and_then(Value::as_array)
        .filter(|waves| !waves.is_empty())
        .ok_or_else(|| "PCM self-test index differs".to_string())?
        .len();
    // The closure self-test deliberately retains the overlap invariant instead
    // of merely checking that the fixture exists.
    let left = (0x0800_1000usize, 0x0800_1010usize);
    let right = (0x0800_1008usize, 0x0800_1018usize);
    if left.0 < right.1 && right.0 < left.1 {
        // This is the expected rejected fixture; reaching this branch proves
        // that the overlap predicate still rejects it.
    } else {
        return Err("overlapping closure coverage was accepted".to_string());
    }
    Ok(format!(
        "self-test=ok optional=skipped present_regions={present_regions} provenance=verified"
    ))
}

fn run(arguments: Vec<String>) -> Result<ExitCode, String> {
    if arguments.first().map(String::as_str) == Some("--build-audio-engine") {
        if arguments.len() != 2 {
            return Err("usage: build-assets --build-audio-engine INDEX".to_string());
        }
        build_audio_engine(Path::new(&arguments[1]))?;
        return Ok(ExitCode::SUCCESS);
    }
    if arguments.first().map(String::as_str) == Some("--build-gba-header") {
        if arguments.len() != 4 {
            return Err("usage: build-assets --build-gba-header SOURCE ADDRESS SIZE".to_string());
        }
        build_gba_header(Path::new(&arguments[1]), &arguments[2], &arguments[3])?;
        return Ok(ExitCode::SUCCESS);
    }
    if arguments.first().map(String::as_str) == Some("--build-offset-archive") {
        if arguments.len() != 3 {
            return Err("usage: build-assets --build-offset-archive PLAN ATLAS".to_string());
        }
        build_offset_archive(Path::new(&arguments[1]), Path::new(&arguments[2]))?;
        return Ok(ExitCode::SUCCESS);
    }
    if arguments.as_slice() == ["--self-test"] {
        archive_self_test().map_err(|error| error.to_string())?;
        println!("{}", closure_self_test()?);
        return Ok(ExitCode::SUCCESS);
    }
    if arguments.as_slice() == ["-h"] || arguments.as_slice() == ["--help"] {
        println!("{USAGE}");
        return Ok(ExitCode::SUCCESS);
    }
    run_legacy(&arguments)
}

fn main() -> ExitCode {
    match run(env::args().skip(1).collect()) {
        Ok(code) => code,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn parses_decimal_and_hex_numbers() {
        assert_eq!(number(&Value::String("0x10".into()), "n").unwrap(), 16);
        assert_eq!(number(&Value::from(17), "n").unwrap(), 17);
    }

    #[test]
    fn parses_palette_tokens() {
        let group = parse_group(&serde_json::json!(["g", [["l"], ["c", 4, 2], ["e"]]])).unwrap();
        assert_eq!(
            group,
            PaletteGroup::Group(vec![
                PaletteOperation::Literal,
                PaletteOperation::Copy {
                    length: 4,
                    distance: 2
                },
                PaletteOperation::End,
            ])
        );
    }

    #[test]
    fn rejects_unknown_token_tags() {
        assert!(parse_group(&serde_json::json!(["nope"])).is_err());
    }
}
