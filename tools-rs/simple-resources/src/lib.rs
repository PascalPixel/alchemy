use std::fs;
use std::path::{Path, PathBuf};

use export_asset::{palette_rgba_image, tile_png};
use extract_resource::{decode_palette_trace, encode_palette, PaletteGroup, PaletteOperation};
use import_asset::{gba_graphics, gba_palette_rgba, Rgb};
use serde_json::{Map, Value};

pub const ROM_BASE: u32 = 0x0800_0000;
pub const RESOURCE_TABLE: u32 = 0x0832_0000;
const R2: (u32, usize) = (0x0832_0fa0, 0x10);
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
fn write(path: &Path, bytes: &[u8]) -> Result<(), Error> {
    if let Some(parent) = path.parent() {
        fs::create_dir_all(parent).map_err(|e| err(format!("{}: {e}", parent.display())))?;
    }
    fs::write(path, bytes).map_err(|e| err(format!("{}: {e}", path.display())))
}
fn write_json(path: &Path, value: &Value) -> Result<(), Error> {
    let json = serde_json::to_string(value).map_err(|e| err(e.to_string()))?;
    write(path, format!("{json}\n").as_bytes())
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
fn offset(address: u32) -> Result<usize, Error> {
    address
        .checked_sub(ROM_BASE)
        .map(|x| x as usize)
        .ok_or_else(|| err("resource address is below ROM base"))
}
fn pointer(rom: &[u8], id: u32) -> Result<u32, Error> {
    let start = RESOURCE_TABLE
        .checked_sub(ROM_BASE)
        .and_then(|x| (x as usize).checked_add(id as usize * 4))
        .ok_or_else(|| err("resource pointer is outside the ROM"))?;
    let bytes = rom
        .get(start..start + 4)
        .ok_or_else(|| err("resource pointer is outside the ROM"))?;
    Ok(u32::from_le_bytes(bytes.try_into().unwrap()))
}
fn range(rom: &[u8], id: u32, address: u32, size: usize) -> Result<Vec<u8>, Error> {
    if pointer(rom, id)? != address {
        return Err(err(format!("resource {id:x} address differs")));
    }
    let start = offset(address)?;
    rom.get(start..start + size)
        .map(ToOwned::to_owned)
        .ok_or_else(|| err("resource range is outside the ROM"))
}
fn bgr555(data: &[u8]) -> Result<Vec<Rgb>, Error> {
    if !data.len().is_multiple_of(2) {
        return Err(err("palette has an odd byte count"));
    }
    Ok(data
        .chunks_exact(2)
        .map(|x| {
            let v = u16::from_le_bytes([x[0], x[1]]);
            [
                ((v & 31) << 3) as u8,
                (((v >> 5) & 31) << 3) as u8,
                (((v >> 10) & 31) << 3) as u8,
            ]
        })
        .collect())
}
fn font_palette(rom: &[u8]) -> Result<Vec<Rgb>, Error> {
    let start = offset(0x0800_777c)?;
    let mut values = rom
        .get(start..start + 32)
        .ok_or_else(|| err("font palette is outside the ROM"))?
        .to_vec();
    for (i, value) in [0x4180u16, 0x3960, 0x3140, 0x2920, 0x49a0, 0x51c0, 0x59e0]
        .into_iter()
        .enumerate()
    {
        values[(i + 4) * 2..(i + 5) * 2].copy_from_slice(&value.to_le_bytes());
    }
    bgr555(&values)
}
fn stamp_paths(root: &Path) -> (PathBuf, PathBuf) {
    let p = root.join("data/resource_2");
    (
        p.with_file_name("resource_2_build_stamp.txt"),
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

fn plan_json(tokens: &[PaletteGroup]) -> Value {
    let mut plan = Map::new();
    for (k, v) in [
        ("format", Value::from(1)),
        ("codec", Value::from("golden-sun-tagged-palette-screen")),
        ("resource_id", Value::from("18")),
        ("address", Value::from("0x083337a0")),
        ("container_size", Value::from("0x828")),
        ("decoded_size", Value::from("0x9600")),
        ("palette_entries", Value::from(224)),
        ("tag", Value::from(1)),
        ("width", Value::from(240)),
        ("height", Value::from(160)),
    ] {
        plan.insert(k.into(), v);
    }
    let mut consumer = Map::new();
    for (k, v) in [
        ("function", Value::from("Func_080f2b70")),
        ("palette_bytes", Value::from("0x1c0")),
        ("stream_offset", Value::from("0x1c0")),
        ("tile_depth", Value::from(8)),
    ] {
        consumer.insert(k.into(), v);
    }
    plan.insert("consumer".into(), Value::Object(consumer));
    let tokens = tokens
        .iter()
        .map(|group| match group {
            PaletteGroup::Zeros => Value::Array(vec![Value::from("z")]),
            PaletteGroup::Group(ops) => Value::Array(vec![
                Value::from("g"),
                Value::Array(
                    ops.iter()
                        .map(|op| match op {
                            PaletteOperation::Literal => Value::Array(vec![Value::from("l")]),
                            PaletteOperation::End => Value::Array(vec![Value::from("e")]),
                            PaletteOperation::Copy { length, distance } => Value::Array(vec![
                                Value::from("c"),
                                Value::from(*length),
                                Value::from(*distance),
                            ]),
                        })
                        .collect(),
                ),
            ]),
        })
        .collect();
    plan.insert("tokens".into(), Value::Array(tokens));
    Value::Object(plan)
}
pub fn export_simple_resources(rom_path: &Path, root: &Path) -> Result<String, Error> {
    let rom = read(rom_path)?;
    let stamp = range(&rom, 2, R2.0, R2.1)?;
    let (source, plan) = stamp_paths(root);
    write(
        &source,
        format!("{}\n", String::from_utf8_lossy(&stamp[..12])).as_bytes(),
    )?;
    let mut stamp_plan = Map::new();
    stamp_plan.insert("format".into(), Value::from(1));
    stamp_plan.insert("kind".into(), Value::from("golden-sun-build-stamp"));
    stamp_plan.insert("digits".into(), Value::from(12));
    stamp_plan.insert(
        "trailer".into(),
        Value::Array(stamp[12..].iter().map(|x| Value::from(*x)).collect()),
    );
    write_json(&plan, &Value::Object(stamp_plan))?;
    if build_resource_2(root)? != stamp {
        return Err(err("resource 2 round trip differs"));
    }
    let font = range(&rom, 0x13, R13.0, R13.1)?;
    write(
        &root.join("graphics/resource_13_font.4bpp.png"),
        &tile_png(&font, 4.0, 16.0, Some(&font_palette(&rom)?))
            .map_err(|e| err(e.0))?
            .0,
    )?;
    if build_resource_13(root)? != font {
        return Err(err("resource 13 round trip differs"));
    }
    let words = range(&rom, 0x14, R14.0, R14.1)?;
    write(
        &root.join("graphics/resource_14_words.rgba.png"),
        &palette_rgba_image(&words, 16.0).map_err(|e| err(e.0))?.0,
    )?;
    if build_resource_14(root)? != words {
        return Err(err("resource 14 round trip differs"));
    }
    let screen = range(&rom, 0x18, R18.0, R18.1)?;
    let start = offset(R18.0)?;
    if screen[0x1c0] != 1 {
        return Err(err("resource 18 lacks its tag-1 screen stream"));
    }
    let (decoded, cursor, tokens) =
        decode_palette_trace(&rom, start + 0x1c1, start + screen.len(), R18.2 as u64)
            .map_err(|e| err(e.0))?;
    if decoded.len() != R18.2 || cursor != start + screen.len() {
        return Err(err(
            "resource 18 screen stream differs from its consumer bounds",
        ));
    }
    write(
        &root.join("graphics/resource_18_screen.8bpp.png"),
        &tile_png(&decoded, 8.0, 30.0, Some(&bgr555(&screen[..0x1c0])?))
            .map_err(|e| err(e.0))?
            .0,
    )?;
    write_json(
        &root.join("graphics/resource_18_screen.lz.json"),
        &plan_json(&tokens),
    )?;
    if build_resource_18(root)? != screen {
        return Err(err("resource 18 screen round trip differs"));
    }
    Ok("resources=4 unique_bytes=0x30f8 screen_18=240x160 decoded_18=0x9600".into())
}
pub fn verify_simple_resources(rom_path: &Path, root: &Path) -> Result<String, Error> {
    let rom = read(rom_path)?;
    for (id, address, size) in [(2, R2.0, R2.1), (0x13, R13.0, R13.1), (0x14, R14.0, R14.1)] {
        if build_simple_resource(id, root)? != range(&rom, id, address, size)? {
            return Err(err(format!("resource {id:x} verification differs")));
        }
    }
    if build_resource_18(root)? != range(&rom, 0x18, R18.0, R18.1)? {
        return Err(err("resource 18 verification differs"));
    }
    Ok("identical=true resources=4 unique_bytes=0x30f8".into())
}
pub fn self_test() -> Result<(), Error> {
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

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn self_test_matches_typescript() {
        self_test().unwrap();
    }
    #[test]
    fn palette_plan_round_trips() {
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
        let encoded = encode_palette(&decoded, &groups).unwrap();
        let (output, cursor, parsed) = decode_palette_trace(&encoded, 0, encoded.len(), 4).unwrap();
        assert_eq!(output, decoded);
        assert_eq!(cursor, encoded.len());
        assert_eq!(parsed, groups);
    }
    #[test]
    fn plan_keeps_hex_fields() {
        let plan = plan_json(&[PaletteGroup::Zeros]);
        assert_eq!(number_field(&plan, "address").unwrap(), R18.0 as u64);
        assert_eq!(
            number_field(plan.get("consumer").unwrap(), "palette_bytes").unwrap(),
            0x1c0
        );
        assert!(validate_plan(&plan).is_ok());
    }
}
