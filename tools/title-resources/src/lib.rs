use alignment_tail::{build_alignment_tail, parse_alignment_tail};
use extract_resource::{encode_palette, PaletteGroup, PaletteOperation};
use import_asset::gba_graphics;
use serde_json::Value;
use std::fs;
use std::io::{self, Write};
use std::path::{Path, PathBuf};

const USAGE: &str = "usage: title_resources build-stdout PLAN";

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);

impl std::fmt::Display for Error {
    fn fmt(&self, output: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        output.write_str(&self.0)
    }
}

fn err(message: impl Into<String>) -> Error {
    Error(message.into())
}

fn read(path: &Path) -> Result<Vec<u8>, Error> {
    fs::read(path).map_err(|e| err(format!("{}: {e}", path.display())))
}

fn json(path: &Path) -> Result<Value, Error> {
    let data = read(path)?;
    serde_json::from_slice(&data).map_err(|e| err(format!("{}: {e}", path.display())))
}

fn field<'a>(value: &'a Value, name: &str) -> Result<&'a Value, Error> {
    value
        .get(name)
        .ok_or_else(|| err(format!("missing {name}")))
}

fn number(value: &Value, name: &str) -> Result<usize, Error> {
    value
        .as_u64()
        .and_then(|n| usize::try_from(n).ok())
        .ok_or_else(|| err(format!("{name} must be an integer")))
}

fn string<'a>(value: &'a Value, name: &str) -> Result<&'a str, Error> {
    value
        .as_str()
        .ok_or_else(|| err(format!("{name} must be a string")))
}

fn component_size(bpp: u8, width: usize, height: usize) -> Result<usize, Error> {
    let bits = width
        .checked_mul(height)
        .and_then(|n| n.checked_mul(bpp as usize))
        .ok_or_else(|| err("component size overflow"))?;
    if bits % 8 != 0 {
        return Err(err("component size is fractional"));
    }
    Ok(bits / 8)
}

fn validate_components(components: &[Value], decoded_size: usize) -> Result<(), Error> {
    let mut cursor = 0;
    for component in components {
        let offset = number(field(component, "offset")?, "component offset")?;
        let bpp = number(field(component, "bpp")?, "component bpp")?;
        let width = number(field(component, "width")?, "component width")?;
        let height = number(field(component, "height")?, "component height")?;
        let size = number(field(component, "size")?, "component size")?;
        if offset != cursor || size != component_size(bpp as u8, width, height)? {
            return Err(err("title components are not a contiguous native layout"));
        }
        if width % 8 != 0 || height % 8 != 0 {
            return Err(err("title component dimensions are not tile aligned"));
        }
        cursor += size;
    }
    if cursor != decoded_size {
        return Err(err("title components do not cover decoded output"));
    }
    Ok(())
}

fn expand_groups(value: &Value) -> Result<Vec<PaletteGroup>, Error> {
    let groups = value
        .as_array()
        .ok_or_else(|| err("invalid compact title groups"))?;
    let mut ended = false;
    let mut result = Vec::new();
    for group in groups {
        if ended {
            return Err(err("title groups contain data after terminator"));
        }
        if group.as_u64() == Some(0) {
            result.push(PaletteGroup::Zeros);
            continue;
        }
        let values = group
            .as_array()
            .ok_or_else(|| err("invalid compact title group"))?;
        if values.len() < 3 || values.iter().any(|v| v.as_u64().is_none()) {
            return Err(err("invalid compact title group"));
        }
        let flags = values[0].as_u64().unwrap();
        if flags == 0 {
            return Err(err("compact title flags are invalid"));
        }
        let mut cursor = 1;
        let mut operations = Vec::new();
        for bit in (0..8).rev() {
            if flags & (1 << bit) == 0 {
                operations.push(PaletteOperation::Literal);
                continue;
            }
            if cursor + 1 >= values.len() {
                return Err(err("compact title copy is truncated"));
            }
            let length = values[cursor].as_u64().unwrap() as u32;
            let distance = values[cursor + 1].as_u64().unwrap() as u32;
            cursor += 2;
            if length == 0 && distance == 0 {
                operations.push(PaletteOperation::End);
                ended = true;
                break;
            }
            operations.push(PaletteOperation::Copy { length, distance });
        }
        if cursor != values.len() {
            return Err(err("compact title group has unused values"));
        }
        result.push(PaletteGroup::Group(operations));
    }
    if !ended {
        return Err(err("compact title groups lack a terminator"));
    }
    Ok(result)
}

fn plan_components(plan: &Value) -> Result<Vec<Value>, Error> {
    plan.get("components")
        .and_then(Value::as_array)
        .cloned()
        .ok_or_else(|| err("title components must be an array"))
}

fn source_for(plan_path: &Path, source: &str) -> PathBuf {
    let text = plan_path.to_string_lossy();
    let base = text.strip_suffix("container.json").unwrap_or(&text);
    let flat = PathBuf::from(format!("{}{}", base, source.replace('/', "_")));
    if flat.is_file() {
        flat
    } else {
        plan_path
            .parent()
            .unwrap_or_else(|| Path::new("."))
            .join(source)
    }
}

pub fn build_title_resource(plan_path: &Path) -> Result<Vec<u8>, Error> {
    let plan = json(plan_path)?;
    if number(field(&plan, "format")?, "format")? != 1
        || string(field(&plan, "codec")?, "codec")? != "golden-sun-title-lz"
    {
        return Err(err("unsupported title-resource plan"));
    }
    let decoded_size = number(field(&plan, "decoded_size")?, "decoded_size")?;
    let source_size = number(field(&plan, "source_size")?, "source_size")?;
    let container_size = number(field(&plan, "container_size")?, "container_size")?;
    let palette_entries = number(field(&plan, "palette_entries")?, "palette_entries")?;
    let components = plan_components(&plan)?;
    validate_components(&components, decoded_size)?;
    let mut decoded = vec![0u8; decoded_size];
    let mut full_palette: Option<Vec<u8>> = None;
    let mut graphics = Vec::new();
    for component in &components {
        let source = string(field(component, "source")?, "component source")?;
        let bpp = number(field(component, "bpp")?, "component bpp")? as f64;
        let width = number(field(component, "width")?, "component width")?;
        let height = number(field(component, "height")?, "component height")?;
        let size = number(field(component, "size")?, "component size")?;
        let image = read(&source_for(plan_path, source))?;
        let (tiles, palette, report) = gba_graphics(&image, bpp).map_err(|e| err(e.0))?;
        if report.get("width") != Some(width as f64)
            || report.get("height") != Some(height as f64)
            || tiles.len() != size
        {
            return Err(err(format!("title image dimensions differ: {source}")));
        }
        if palette.len() == palette_entries * 2 && full_palette.is_none() {
            full_palette = Some(palette.clone());
        }
        graphics.push((tiles, palette));
    }
    let palette = full_palette.ok_or_else(|| err("full title palette is missing"))?;
    for (index, component) in components.iter().enumerate() {
        let (tiles, component_palette) = &graphics[index];
        if palette.get(..component_palette.len()) != Some(component_palette.as_slice()) {
            return Err(err(format!(
                "title image palette differs: {}",
                string(field(component, "source")?, "component source")?
            )));
        }
        let offset = number(field(component, "offset")?, "component offset")?;
        decoded[offset..offset + tiles.len()].copy_from_slice(tiles);
    }
    if palette.len() != palette_entries * 2 {
        return Err(err("title palette has the wrong size"));
    }
    let groups = expand_groups(field(&plan, "groups")?)?;
    let encoded_stream = encode_palette(&decoded, &groups).map_err(|e| err(e.0))?;
    let mut encoded = palette;
    encoded.extend(encoded_stream);
    if encoded.len() != source_size {
        return Err(err("title source size differs from its plan"));
    }
    let tail_value = field(&plan, "tail")?;
    let tail_size = number(field(tail_value, "size")?, "title tail size")?;
    if tail_size != container_size.saturating_sub(source_size) || container_size < source_size {
        return Err(err("title tail size differs from its plan"));
    }
    if let Some(policy) = tail_value.get("policy").and_then(Value::as_str) {
        match policy {
            "zero" => encoded.extend(std::iter::repeat_n(0, tail_size)),
            "fallback" => {}
            _ => return Err(err("unsupported title tail policy")),
        }
    } else {
        let tail =
            parse_alignment_tail(tail_value, tail_size, 3, "title tail").map_err(|e| err(e.0))?;
        encoded.extend(build_alignment_tail(&tail));
    }
    Ok(encoded)
}

pub fn run(args: Vec<String>) -> Result<(), Error> {
    if args.first().map(String::as_str) != Some("build-stdout") {
        return Err(err(USAGE));
    }
    let plan = args.get(1).ok_or_else(|| err(USAGE))?;
    let built = build_title_resource(Path::new(plan))?;
    io::stdout()
        .write_all(&built)
        .map_err(|e| err(e.to_string()))
}
