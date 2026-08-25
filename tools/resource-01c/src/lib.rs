pub mod cli;

use extract_resource::{encode_general, GeneralToken};
use import_asset::gba_graphics;
use serde_json::{Map, Value};
use sha2::{Digest, Sha256};
use std::fs;
use std::path::{Path, PathBuf};

pub const RESOURCE_ADDRESS: u32 = 0x0833_ac08;
pub const RESOURCE_SIZE: usize = 0x02b4;
pub const RESOURCE_END: u32 = RESOURCE_ADDRESS + RESOURCE_SIZE as u32;
pub const PALETTE_BYTES: usize = 0x20;
pub const GLYPHS: usize = 36;
pub const ATLAS_COLUMNS: usize = 9;
const TILE_BYTES: usize = 0x20;
const ATLAS_SHA256: &str = "9fa7a5102e3babea665d7bd1430f2f11397630c8bff35d272482bb82bd049d0a";

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

#[derive(Debug, Clone)]
struct Plan {
    tokens: Vec<GeneralToken>,
    source: String,
}

fn read(path: &Path) -> Result<Vec<u8>, Error> {
    fs::read(path).map_err(|error| err(format!("{}: {error}", path.display())))
}

fn object<'a>(value: &'a Value, label: &str) -> Result<&'a Map<String, Value>, Error> {
    value.as_object().ok_or_else(|| err(format!("{label} must be an object")))
}

fn exact_keys(value: &Map<String, Value>, expected: &[&str], label: &str) -> Result<(), Error> {
    let mut actual: Vec<&str> = value.keys().map(String::as_str).collect();
    actual.sort_unstable();
    let mut expected = expected.to_vec();
    expected.sort_unstable();
    if actual != expected {
        return Err(err(format!("{label} has unexpected fields")));
    }
    Ok(())
}

fn string<'a>(value: &'a Value, name: &str) -> Result<&'a str, Error> {
    value.get(name).and_then(Value::as_str).ok_or_else(|| err(format!("invalid {name}")))
}

fn number(value: &Value) -> Option<u64> {
    value.as_u64().or_else(|| value.as_str().and_then(|text| text.strip_prefix("0x").or_else(|| text.strip_prefix("0X")).map_or_else(|| text.parse().ok(), |hex| u64::from_str_radix(hex, 16).ok())))
}

fn number_field(value: &Value, name: &str) -> Result<u64, Error> {
    value.get(name).and_then(number).ok_or_else(|| err(format!("invalid {name}")))
}

fn json_document(path: &Path, label: &str) -> Result<Value, Error> {
    let text = fs::read_to_string(path).map_err(|error| err(format!("{}: {error}", path.display())))?;
    let value: Value = serde_json::from_str(&text).map_err(|error| err(format!("{label}: {error}")))?;
    Ok(value)
}

fn parse_tokens(value: &Value) -> Result<Vec<GeneralToken>, Error> {
    let values = value.as_array().ok_or_else(|| err("kana tokens must be an array"))?;
    let mut tokens = Vec::with_capacity(values.len());
    for (index, value) in values.iter().enumerate() {
        let token = value.as_array().ok_or_else(|| err(format!("kana token {index} must be an array")))?;
        match token.first().and_then(Value::as_str) {
            Some("l") if token.len() == 2 => {
                let count = token[1].as_u64();
                if count.is_none_or(|count| count == 0) {
                    return Err(err(format!("kana token {index} has an invalid literal shape")));
                }
                tokens.push(GeneralToken::Literal(count.unwrap() as u32));
            }
            Some("c") if token.len() == 3 => {
                let length = token[1].as_u64();
                let distance = token[2].as_u64();
                if length.is_none_or(|value| !(2..=137).contains(&value)) || distance.is_none_or(|value| !(1..=0x1020).contains(&value)) {
                    return Err(err(format!("kana token {index} has an invalid copy shape")));
                }
                tokens.push(GeneralToken::Copy { length: length.unwrap() as u32, distance: distance.unwrap() as u32 });
            }
            _ => return Err(err(format!("kana token {index} has an unsupported operation"))),
        }
    }
    Ok(tokens)
}

fn hex(value: u32, width: usize) -> String {
    format!("0x{value:0width$x}")
}

fn read_plan(path: &Path) -> Result<Plan, Error> {
    let value = json_document(path, "kana plan")?;
    let plan = object(&value, "kana plan")?;
    exact_keys(plan, &["format", "kind", "codec", "resource_id", "address", "size", "end", "decoded_size", "palette_entries", "glyph_width", "glyph_height", "glyphs", "atlas_columns", "source", "tokens"], "kana plan")?;
    if number_field(&value, "format")? != 1
        || string(&value, "kind")? != "golden-sun-kana-glyph-bank"
        || string(&value, "codec")? != "golden-sun-general-lz"
        || string(&value, "resource_id")? != "01c"
        || string(&value, "address")? != hex(RESOURCE_ADDRESS, 8)
        || string(&value, "size")? != hex(RESOURCE_SIZE as u32, 1)
        || string(&value, "end")? != hex(RESOURCE_END, 8)
        || number_field(&value, "decoded_size")? != (PALETTE_BYTES + GLYPHS * TILE_BYTES) as u64
        || number_field(&value, "palette_entries")? != 16
        || number_field(&value, "glyph_width")? != 8
        || number_field(&value, "glyph_height")? != 8
        || number_field(&value, "glyphs")? != GLYPHS as u64
        || number_field(&value, "atlas_columns")? != ATLAS_COLUMNS as u64
        || string(&value, "source")? != "kana.4bpp.png"
    {
        return Err(err("kana plan differs from the audited layout"));
    }
    Ok(Plan { tokens: parse_tokens(plan.get("tokens").unwrap())?, source: string(&value, "source")?.to_string() })
}

fn physical_path(path: &Path) -> PathBuf {
    let absolute = if path.is_absolute() { path.to_path_buf() } else { std::env::current_dir().unwrap_or_default().join(path) };
    let mut existing = absolute.clone();
    let mut suffix = Vec::new();
    while !existing.exists() {
        let Some(name) = existing.file_name() else {
            return absolute;
        };
        suffix.push(name.to_owned());
        let Some(parent) = existing.parent() else {
            return absolute;
        };
        existing = parent.to_path_buf();
    }
    let mut result = fs::canonicalize(existing).unwrap_or_else(|_| absolute.clone());
    for name in suffix.iter().rev() {
        result.push(name);
    }
    result
}

fn source_path(plan_path: &Path, name: &str) -> Result<PathBuf, Error> {
    if name != "kana.4bpp.png" {
        return Err(err("kana source name differs"));
    }
    let basename = plan_path.file_name().and_then(|name| name.to_str()).ok_or_else(|| err("kana source name differs"))?;
    let prefix = if basename.bytes().all(|byte| byte.is_ascii_lowercase() || byte.is_ascii_digit() || byte == b'_' || byte == b'.') { basename.rfind('_').map_or_else(String::new, |index| basename[..=index].to_string()) } else { String::new() };
    let root = fs::canonicalize(plan_path.parent().ok_or_else(|| err("kana plan has no parent"))?).map_err(|error| err(error.to_string()))?;
    let path = fs::canonicalize(root.join(format!("{prefix}{name}"))).map_err(|error| err(error.to_string()))?;
    let relative = path.strip_prefix(&root).map_err(|_| err("kana source escaped its directory"))?;
    if relative != Path::new(&format!("{prefix}{name}")) {
        return Err(err("kana source escaped its directory"));
    }
    Ok(path)
}

fn read_kana_image(path: &Path) -> Result<(Vec<u8>, Vec<u8>), Error> {
    let encoded = read(path)?;
    let (tiles, palette, report) = gba_graphics(&encoded, 4.0).map_err(|error| err(error.0))?;
    if format!("{:x}", Sha256::digest(&encoded)) != ATLAS_SHA256 {
        return Err(err("kana atlas is not a canonical source PNG"));
    }
    if report.get("width") != Some((ATLAS_COLUMNS * 8) as f64) || report.get("height") != Some((GLYPHS / ATLAS_COLUMNS * 8) as f64) || report.get("tiles") != Some(GLYPHS as f64) || report.get("palette_entries") != Some(16.0) || palette.len() != PALETTE_BYTES || tiles.len() != GLYPHS * TILE_BYTES {
        return Err(err("kana atlas differs from the audited dimensions"));
    }
    Ok((tiles, palette))
}

pub fn build_resource_01c(plan_path: &Path) -> Result<(Vec<u8>, Vec<PathBuf>), Error> {
    let plan = read_plan(plan_path)?;
    let image_path = source_path(plan_path, &plan.source)?;
    let (tiles, palette) = read_kana_image(&image_path)?;
    let mut decoded = palette;
    decoded.extend_from_slice(&tiles);
    let encoded = encode_general(&decoded, &plan.tokens).map_err(|error| err(error.0))?;
    if encoded.len() != RESOURCE_SIZE {
        return Err(err("kana resource has the wrong encoded size"));
    }
    Ok((encoded, vec![physical_path(plan_path), image_path]))
}
