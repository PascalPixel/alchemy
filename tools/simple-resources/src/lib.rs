use std::fs;
use std::path::{Path, PathBuf};

#[cfg(test)]
use extract_resource::decode_palette_trace;
use extract_resource::{encode_palette, PaletteGroup, PaletteOperation};
use import_asset::{gba_graphics, gba_palette_rgba};
use serde_json::Value;

const R13: (u32, usize) = (0x0832_0fb0, 0x2000);
const R14: (u32, usize) = (0x0832_2fb0, 0x8c0);
const R18: (u32, usize, usize) = (0x0833_37a0, 0x828, 0x9600);

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);
impl std::fmt::Display for Error {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(&self.0)
    }
}
impl std::error::Error for Error {}
fn err(value: impl Into<String>) -> Error {
    Error(value.into())
}

fn read(path: &Path) -> Result<Vec<u8>, Error> {
    fs::read(path).map_err(|e| err(format!("{}: {e}", path.display())))
}
fn text(path: &Path) -> Result<String, Error> {
    fs::read_to_string(path).map_err(|e| err(format!("{}: {e}", path.display())))
}
fn absolute(path: &Path) -> PathBuf {
    if path.is_absolute() {
        path.to_path_buf()
    } else {
        std::env::current_dir().unwrap_or_default().join(path)
    }
}
fn same(left: &Path, right: &Path) -> bool {
    match (fs::canonicalize(left), fs::canonicalize(right)) {
        (Ok(a), Ok(b)) => a == b,
        _ => absolute(left) == absolute(right),
    }
}
pub fn same_paths(left: &Path, right: &Path) -> bool {
    same(left, right)
}
fn json(path: &Path) -> Result<Value, Error> {
    serde_json::from_str(&text(path)?).map_err(|e| err(format!("{}: {e}", path.display())))
}
fn field<'a>(value: &'a Value, name: &str) -> Result<&'a Value, Error> {
    value
        .get(name)
        .ok_or_else(|| err(format!("missing {name}")))
}
fn string_field<'a>(value: &'a Value, name: &str) -> Result<&'a str, Error> {
    field(value, name)?
        .as_str()
        .ok_or_else(|| err(format!("invalid {name}")))
}
fn number(value: &Value) -> Option<u64> {
    value.as_u64().or_else(|| {
        value.as_str().and_then(|s| {
            if let Some(hex) = s.strip_prefix("0x").or_else(|| s.strip_prefix("0X")) {
                u64::from_str_radix(hex, 16).ok()
            } else {
                s.parse().ok()
            }
        })
    })
}
fn number_field(value: &Value, name: &str) -> Result<u64, Error> {
    number(field(value, name)?).ok_or_else(|| err(format!("invalid {name}")))
}
fn stamp_paths(root: &Path) -> (PathBuf, PathBuf) {
    let p = root.join("data/resource_2");
    (
        p.with_file_name("resource_2_build_stamp.stamp"),
        p.with_file_name("resource_2_layout.json"),
    )
}

pub fn build_resource_2(root: &Path) -> Result<Vec<u8>, Error> {
    let (source, plan_path) = stamp_paths(root);
    let stamp = text(&source)?.trim_end().to_string();
    let plan = json(&plan_path)?;
    if number_field(&plan, "format")? != 1
        || string_field(&plan, "kind")? != "golden-sun-build-stamp"
        || number_field(&plan, "digits")? != 12
        || stamp.len() != 12
        || !stamp.bytes().all(|x| x.is_ascii_digit())
    {
        return Err(err("invalid build-stamp source"));
    }
    let trailer = field(&plan, "trailer")?
        .as_array()
        .ok_or_else(|| err("invalid build-stamp trailer"))?;
    if trailer.len() != 4 || trailer.iter().any(|x| number(x).is_none_or(|n| n > 255)) {
        return Err(err("invalid build-stamp trailer"));
    }
    Ok(stamp
        .into_bytes()
        .into_iter()
        .chain(trailer.iter().map(|x| number(x).unwrap() as u8))
        .collect())
}
pub fn build_resource_13(root: &Path) -> Result<Vec<u8>, Error> {
    let (tiles, _, report) = gba_graphics(
        &read(&root.join("graphics/resource_13_font.4bpp.png"))?,
        4.0,
    )
    .map_err(|e| err(e.0))?;
    if report.get("width") != Some(128.0)
        || report.get("height") != Some(128.0)
        || tiles.len() != R13.1
    {
        return Err(err("font source must be a 16 by 16 grid of 8 by 8 glyphs"));
    }
    Ok(tiles)
}
pub fn build_resource_14(root: &Path) -> Result<Vec<u8>, Error> {
    let (words, report) =
        gba_palette_rgba(&read(&root.join("graphics/resource_14_words.rgba.png"))?)
            .map_err(|e| err(e.0))?;
    if report.get("width") != Some(16.0)
        || report.get("height") != Some(70.0)
        || words.len() != R14.1
    {
        return Err(err(
            "resource 14 word image must contain 1120 BGR555-compatible values",
        ));
    }
    Ok(words)
}

fn operation(value: &Value) -> Result<PaletteOperation, Error> {
    let a = value
        .as_array()
        .ok_or_else(|| err("invalid palette token"))?;
    match a.first().and_then(Value::as_str) {
        Some("l") if a.len() == 1 => Ok(PaletteOperation::Literal),
        Some("e") if a.len() == 1 => Ok(PaletteOperation::End),
        Some("c") if a.len() == 3 => Ok(PaletteOperation::Copy {
            length: number(&a[1]).ok_or_else(|| err("invalid palette token length"))? as u32,
            distance: number(&a[2]).ok_or_else(|| err("invalid palette token distance"))? as u32,
        }),
        _ => Err(err("invalid palette token")),
    }
}
fn groups(value: &Value) -> Result<Vec<PaletteGroup>, Error> {
    value
        .as_array()
        .ok_or_else(|| err("invalid palette tokens"))?
        .iter()
        .map(|value| {
            let a = value
                .as_array()
                .ok_or_else(|| err("invalid palette token group"))?;
            match a.first().and_then(Value::as_str) {
                Some("z") if a.len() == 1 => Ok(PaletteGroup::Zeros),
                Some("g") if a.len() == 2 => Ok(PaletteGroup::Group(
                    a[1].as_array()
                        .ok_or_else(|| err("invalid palette token group"))?
                        .iter()
                        .map(operation)
                        .collect::<Result<_, _>>()?,
                )),
                _ => Err(err("invalid palette token group")),
            }
        })
        .collect()
}
fn validate_plan(plan: &Value) -> Result<Vec<PaletteGroup>, Error> {
    if number_field(plan, "format")? != 1
        || string_field(plan, "codec")? != "golden-sun-tagged-palette-screen"
        || string_field(plan, "resource_id")? != "18"
        || number_field(plan, "address")? != R18.0 as u64
        || number_field(plan, "container_size")? != R18.1 as u64
        || number_field(plan, "decoded_size")? != R18.2 as u64
        || number_field(plan, "palette_entries")? != 224
        || number_field(plan, "tag")? != 1
        || number_field(plan, "width")? != 240
        || number_field(plan, "height")? != 160
    {
        return Err(err(
            "resource 18 screen plan differs from its consumer layout",
        ));
    }
    let consumer = field(plan, "consumer")?;
    if string_field(consumer, "function")? != "Func_080f2b70"
        || number_field(consumer, "palette_bytes")? != 0x1c0
        || number_field(consumer, "stream_offset")? != 0x1c0
        || number_field(consumer, "tile_depth")? != 8
    {
        return Err(err(
            "resource 18 screen plan differs from its consumer layout",
        ));
    }
    groups(field(plan, "tokens")?)
}
pub fn build_resource_18(root: &Path) -> Result<Vec<u8>, Error> {
    let prefix = root.join("graphics/resource_18");
    let plan = json(&prefix.with_file_name("resource_18_screen.lz.json"))?;
    let tokens = validate_plan(&plan)?;
    let (tiles, palette, report) = gba_graphics(
        &read(&prefix.with_file_name("resource_18_screen.8bpp.png"))?,
        8.0,
    )
    .map_err(|e| err(e.0))?;
    if report.get("width") != Some(240.0)
        || report.get("height") != Some(160.0)
        || tiles.len() != R18.2
        || palette.len() != 0x1c0
    {
        return Err(err(
            "resource 18 source must be a 240 by 160 screen with 224 colors",
        ));
    }
    let encoded = encode_palette(&tiles, &tokens).map_err(|e| err(e.0))?;
    let mut result = palette;
    result.push(1);
    result.extend(encoded);
    if result.len() != R18.1 {
        return Err(err("resource 18 encoded size differs"));
    }
    Ok(result)
}
pub fn build_simple_resource(id: u32, root: &Path) -> Result<Vec<u8>, Error> {
    match id {
        2 => build_resource_2(root),
        0x13 => build_resource_13(root),
        0x14 => build_resource_14(root),
        0x18 => build_resource_18(root),
        _ => Err(err(format!("unsupported simple resource {id:x}"))),
    }
}

#[test]
fn codec_round_trip() -> Result<(), Error> {
    let digits = "012345678901";
    if digits.len() != 12 || !digits.bytes().all(|x| x.is_ascii_digit()) {
        return Err(err("build-stamp validation self-test failed"));
    }
    let decoded = [1u8, 2, 1, 2];
    let groups = [PaletteGroup::Group(vec![
        PaletteOperation::Literal,
        PaletteOperation::Literal,
        PaletteOperation::Copy {
            length: 2,
            distance: 2,
        },
        PaletteOperation::End,
    ])];
    let encoded = encode_palette(&decoded, &groups).map_err(|e| err(e.0))?;
    if decode_palette_trace(&encoded, 0, encoded.len(), 4)
        .map_err(|e| err(e.0))?
        .0
        != decoded
    {
        return Err(err("tagged screen codec self-test failed"));
    }
    Ok(())
}
