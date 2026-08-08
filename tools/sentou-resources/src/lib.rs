//! Native build and verification support for the sentou resource series.
//!
//! Export is intentionally not part of this narrow port. The tracked plans
//! are the build input: PNGs are decoded by `import-asset`, and their audited
//! token streams are encoded by `extract-resource`.

use alignment_tail::{build_alignment_tail, parse_alignment_tail};
use extract_resource::{
    encode_general_prefill, encode_palette, GeneralToken, PaletteGroup, PaletteOperation,
};
use import_asset::{gba_graphics, gba_palette_rgba, indexed_png};
use serde_json::{Map, Value};
use std::fs;
use std::io::{self, Write};
use std::path::{Path, PathBuf};

pub type Result<T> = std::result::Result<T, String>;
const PREFILL: usize = 0x1000;
const ROM_BASE: usize = 0x0800_0000;

fn fail<T>(message: impl Into<String>) -> Result<T> {
    Err(message.into())
}

fn object<'a>(value: &'a Value, label: &str) -> Result<&'a Map<String, Value>> {
    value
        .as_object()
        .ok_or_else(|| format!("{label} must be an object"))
}

fn field<'a>(value: &'a Map<String, Value>, name: &str) -> Result<&'a Value> {
    value
        .get(name)
        .ok_or_else(|| format!("missing field {name}"))
}

fn number(value: &Value, label: &str) -> Result<usize> {
    if let Some(text) = value.as_str() {
        let text = text
            .strip_prefix("0x")
            .ok_or_else(|| format!("{label} must be an integer"))?;
        return usize::from_str_radix(text, 16).map_err(|_| format!("{label} must be an integer"));
    }
    value
        .as_u64()
        .map(|n| n as usize)
        .ok_or_else(|| format!("{label} must be an integer"))
}

fn json_file(path: &Path) -> Result<Value> {
    let bytes = fs::read(path).map_err(|e| format!("{}: {e}", path.display()))?;
    serde_json::from_slice(&bytes).map_err(|e| format!("{}: {e}", path.display()))
}

fn token_number(value: &Value, label: &str) -> Result<u32> {
    number(value, label).and_then(|n| u32::try_from(n).map_err(|_| format!("{label} is too large")))
}

fn general_tokens(value: &Value) -> Result<Vec<GeneralToken>> {
    let list = value
        .as_array()
        .ok_or_else(|| "sentou general tokens must be an array".to_string())?;
    list.iter()
        .enumerate()
        .map(|(index, token)| {
            let token = token
                .as_array()
                .ok_or_else(|| format!("sentou general token {index} is invalid"))?;
            match token.first().and_then(Value::as_str) {
                Some("l") if token.len() == 2 => Ok(GeneralToken::Literal(token_number(
                    &token[1],
                    "sentou literal length",
                )?)),
                Some("c") if token.len() == 3 => Ok(GeneralToken::Copy {
                    length: token_number(&token[1], "sentou copy length")?,
                    distance: token_number(&token[2], "sentou copy distance")?,
                }),
                _ => fail(format!("sentou general token {index} is invalid")),
            }
        })
        .collect()
}

fn palette_operation(value: &Value) -> Result<PaletteOperation> {
    let token = value
        .as_array()
        .ok_or_else(|| "sentou palette token is invalid".to_string())?;
    match token.first().and_then(Value::as_str) {
        Some("l") if token.len() == 1 => Ok(PaletteOperation::Literal),
        Some("e") if token.len() == 1 => Ok(PaletteOperation::End),
        Some("c") if token.len() == 3 => Ok(PaletteOperation::Copy {
            length: token_number(&token[1], "sentou palette copy length")?,
            distance: token_number(&token[2], "sentou palette copy distance")?,
        }),
        _ => fail("sentou palette token is invalid"),
    }
}

fn palette_groups(value: &Value) -> Result<Vec<PaletteGroup>> {
    let list = value
        .as_array()
        .ok_or_else(|| "sentou palette groups must be an array".to_string())?;
    list.iter()
        .map(|group| {
            let group = group
                .as_array()
                .ok_or_else(|| "sentou palette group is invalid".to_string())?;
            match group.first().and_then(Value::as_str) {
                Some("z") if group.len() == 1 => Ok(PaletteGroup::Zeros),
                Some("g") if group.len() == 2 => {
                    let operations = group[1]
                        .as_array()
                        .ok_or_else(|| "sentou palette group is invalid".to_string())?;
                    Ok(PaletteGroup::Group(
                        operations
                            .iter()
                            .map(palette_operation)
                            .collect::<Result<Vec<_>>>()?,
                    ))
                }
                _ => fail("sentou palette group is invalid"),
            }
        })
        .collect()
}

fn sibling(plan: &Path, suffix: &str) -> PathBuf {
    let file = plan
        .file_name()
        .and_then(|name| name.to_str())
        .unwrap_or_default();
    let prefix = file.strip_suffix("stream.json").unwrap_or_default();
    plan.parent()
        .unwrap_or_else(|| Path::new("."))
        .join(format!("{prefix}{suffix}"))
}

fn build_decoded(
    plan: &Map<String, Value>,
    plan_path: &Path,
    decoded_size: usize,
    canvas_size: usize,
) -> Result<Vec<u8>> {
    let image = object(field(plan, "image")?, "sentou image")?;
    let source = field(image, "source")?
        .as_str()
        .ok_or_else(|| "sentou image source must be a string".to_string())?;
    let image_path = sibling(plan_path, source);
    let image_bytes =
        fs::read(&image_path).map_err(|e| format!("{}: {e}", image_path.display()))?;
    let encoding = field(image, "encoding")?.as_str().unwrap_or_default();
    let data = if encoding == "naiyou" {
        let image = indexed_png(&image_bytes).map_err(|e| e.0)?;
        if image.width != 64 {
            return fail("sentou data image must be 64 bytes wide");
        }
        image
            .pixels
            .into_iter()
            .map(|pixel| pixel as u8)
            .collect::<Vec<_>>()
    } else {
        let bpp = if encoding == "koma-4bpp" { 4.0 } else { 8.0 };
        let (pixels, _, report) = gba_graphics(&image_bytes, bpp).map_err(|e| e.0)?;
        if report.get("width") != Some(240.0) {
            return fail("sentou graphics source must be 240 pixels wide");
        }
        pixels
    };
    if data.len() != canvas_size {
        return fail("sentou source canvas has the wrong size");
    }
    if data[decoded_size..].iter().any(|byte| *byte != 0) {
        return fail("sentou source canvas has nonzero data outside the decoded stream");
    }
    Ok(data[..decoded_size].to_vec())
}

/// Build one audited stream and return its source paths for manifest callers.
pub fn build_sentou_resource(plan_path: &Path) -> Result<(Vec<u8>, Vec<PathBuf>)> {
    let document = json_file(plan_path)?;
    let plan = object(&document, "sentou plan")?;
    if field(plan, "kind")?.as_str() != Some("golden-sun-sentou-resource") {
        return fail("unsupported sentou plan");
    }
    let stream = object(field(plan, "stream")?, "sentou stream")?;
    let decoded_size = number(field(stream, "decoded_size")?, "sentou decoded size")?;
    let image = object(field(plan, "image")?, "sentou image")?;
    let canvas_size = number(field(image, "canvas_size")?, "sentou canvas size")?;
    let decoded = build_decoded(plan, plan_path, decoded_size, canvas_size)?;
    let codec = field(stream, "codec")?.as_str().unwrap_or_default();
    let encoded = if codec == "general-lz-prefill" {
        encode_general_prefill(
            &decoded,
            &general_tokens(field(stream, "tokens")?)?,
            PREFILL,
            1,
        )
        .map_err(|e| e.0)?
    } else if codec == "palette-lz" {
        let mut encoded = vec![1u8];
        encoded.extend_from_slice(
            &encode_palette(&decoded, &palette_groups(field(stream, "tokens")?)?)
                .map_err(|e| e.0)?,
        );
        encoded
    } else {
        return fail("unsupported sentou codec");
    };
    let prefix = match field(plan, "prefix_palette")? {
        Value::Null => Vec::new(),
        value => {
            let palette = object(value, "sentou prefix palette")?;
            let source = field(palette, "source")?.as_str().unwrap_or("iro.rgba.png");
            let image = fs::read(sibling(plan_path, source)).map_err(|e| e.to_string())?;
            gba_palette_rgba(&image).map_err(|e| e.0)?.0
        }
    };
    let source_size = number(field(plan, "source_size")?, "sentou source size")?;
    let boundary_size = number(
        field(plan, "resource_boundary_size")?,
        "sentou boundary size",
    )?;
    let mut result = prefix;
    result.extend_from_slice(&encoded);
    if result.len() != source_size {
        return fail("sentou resource differs from its audited source size");
    }
    if let Some(tail) = plan.get("boundary_suffix").filter(|value| !value.is_null()) {
        let tail = parse_alignment_tail(
            tail,
            boundary_size - source_size,
            3,
            "sentou boundary suffix",
        )
        .map_err(|e| e.0)?;
        result.extend_from_slice(&build_alignment_tail(&tail));
    }
    if result.len() != source_size && result.len() != boundary_size {
        return fail("sentou resource differs from its audited boundary size");
    }
    let image_source = field(image, "source")?.as_str().unwrap_or_default();
    let mut sources = vec![plan_path.to_path_buf(), sibling(plan_path, image_source)];
    if !matches!(field(plan, "prefix_palette")?, Value::Null) {
        sources.push(sibling(plan_path, "iro.rgba.png"));
    }
    Ok((result, sources))
}

pub fn build_sentou_series(index_path: &Path) -> Result<Vec<(usize, Vec<u8>, Vec<PathBuf>)>> {
    let document = json_file(index_path)?;
    let index = object(&document, "sentou index")?;
    let resources = field(index, "resources")?
        .as_array()
        .ok_or_else(|| "sentou index resources must be an array".to_string())?;
    let file = index_path
        .file_name()
        .and_then(|name| name.to_str())
        .unwrap_or_default();
    let prefix = file.strip_suffix("index.json").unwrap_or_default();
    resources
        .iter()
        .map(|entry| {
            let entry = object(entry, "sentou index entry")?;
            let address = number(field(entry, "address")?, "sentou address")?;
            let expected = number(field(entry, "size")?, "sentou index size")?;
            let source = field(entry, "source")?
                .as_str()
                .ok_or_else(|| "sentou index source must be a string".to_string())?;
            let plan = index_path
                .parent()
                .unwrap_or_else(|| Path::new("."))
                .join(format!("{prefix}{source}"));
            let (data, sources) = build_sentou_resource(&plan)?;
            if data.len() != expected {
                return fail("sentou index size differs from its canonical source");
            }
            Ok((address, data, sources))
        })
        .collect()
}

pub fn verify_sentou_resources(rom_path: &Path, directory: &Path) -> Result<String> {
    let index_path = PathBuf::from(format!("{}_index.json", directory.display()));
    let built = build_sentou_series(&index_path)?;
    let rom = fs::read(rom_path).map_err(|e| e.to_string())?;
    let index_document = json_file(&index_path)?;
    let index = object(&index_document, "sentou index")?;
    let resources = field(index, "resources")?.as_array().unwrap();
    let mut claimed = 0usize;
    let mut boundary = 0usize;
    for (position, (address, data, _)) in built.iter().enumerate() {
        let start = address
            .checked_sub(ROM_BASE)
            .ok_or_else(|| "sentou address lies before ROM base".to_string())?;
        if start + data.len() > rom.len() || rom[start..start + data.len()] != *data {
            return fail(format!("sentou resource {position} differs from ROM"));
        }
        claimed += data.len();
        boundary += number(
            field(
                object(&resources[position], "sentou index entry")?,
                "resource_boundary_size",
            )?,
            "sentou boundary",
        )?;
    }
    Ok(format!(
        "identical=true resources={} claimed_bytes={} boundary_bytes={} suffix_fallback={}",
        built.len(),
        claimed,
        boundary,
        boundary - claimed
    ))
}

pub fn self_test() -> Result<()> {
    let encoded = encode_general_prefill(b"TEST", &[GeneralToken::Literal(4)], PREFILL, 1)
        .map_err(|e| e.0)?;
    if encoded.is_empty() {
        return fail("sentou general self-test failed");
    }
    let mut operations = vec![PaletteOperation::Literal; 7];
    operations.push(PaletteOperation::End);
    let groups = [PaletteGroup::Group(operations)];
    if encode_palette(&[0u8; 7], &groups)
        .map_err(|e| e.0)?
        .is_empty()
    {
        return fail("sentou palette self-test failed");
    }
    println!("self-test=ok");
    Ok(())
}

pub fn run(args: &[String]) -> Result<()> {
    if args == ["--self-test"] {
        return self_test();
    }
    if args.len() == 2 && args[0] == "build-stdout" {
        let (data, _) = build_sentou_resource(Path::new(&args[1]))?;
        io::stdout().write_all(&data).map_err(|e| e.to_string())?;
        return Ok(());
    }
    if args.len() == 2 && args[0] == "series-stdout" {
        for (_, data, _) in build_sentou_series(Path::new(&args[1]))? {
            io::stdout().write_all(&data).map_err(|e| e.to_string())?;
        }
        return Ok(());
    }
    if args.len() == 4 && args[0] == "verify" && args[2] == "--directory" {
        println!(
            "{}",
            verify_sentou_resources(Path::new(&args[1]), Path::new(&args[3]))?
        );
        return Ok(());
    }
    if args.len() == 1 && matches!(args[0].as_str(), "-h" | "--help") {
        println!("usage: sentou-resources build-stdout PLAN | series-stdout INDEX | verify ROM --directory DIRECTORY | --self-test");
        return Ok(());
    }
    fail("invalid sentou-resources arguments")
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn native_self_test() {
        self_test().unwrap();
    }
}
