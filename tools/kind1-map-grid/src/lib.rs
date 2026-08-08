//! Native kind-1 map-grid exporter and verifier.
//!
//! A kind-1 map contains two byte planes, two attribute planes, and a
//! palette-compressed 16-bit value plane. Values whose low twelve bits are
//! `0xfff` are represented in the exported images by a separate binary
//! sentinel mask and are assigned consecutive ordinals while exporting.
//!
//! The TypeScript implementation remains in place because the asset builder
//! imports its library functions. This crate provides the standalone CLI and
//! keeps the exact same stream, PNG, and plan representations for direct
//! export and verification.

use std::fs;
use std::path::{Path, PathBuf};

use alchemy_zlib::{deflate_sync, DeflateOptions};
use export_asset::{byte_png, chunk};
use extract_resource::{decode_palette_trace, encode_palette, PaletteGroup, PaletteOperation};
use import_asset::indexed_png;
use serde_json::{json, Map, Value};

pub const ROM_BASE: usize = 0x0800_0000;
pub const PLANE: usize = 0x4000;
pub const DECODED_SIZE: usize = 4 * PLANE;
pub const WIDTH: usize = 128;
pub const HEIGHT: usize = 128;

const FILES: [&str; 4] = [
    "value_low.png",
    "value_high.png",
    "attribute_a.png",
    "attribute_b.png",
];

pub type Result<T> = std::result::Result<T, String>;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct ExportStats {
    pub tokens: usize,
    pub sentinels: usize,
    pub encoded: usize,
}

fn err(message: impl Into<String>) -> String {
    message.into()
}

fn read(path: &Path) -> Result<Vec<u8>> {
    fs::read(path).map_err(|error| format!("{}: {error}", path.display()))
}

fn write(path: &Path, data: &[u8]) -> Result<()> {
    if let Some(parent) = path.parent() {
        fs::create_dir_all(parent).map_err(|error| format!("{}: {error}", parent.display()))?;
    }
    fs::write(path, data).map_err(|error| format!("{}: {error}", path.display()))
}

fn grid_path(directory: &Path, suffix: &str) -> PathBuf {
    PathBuf::from(format!("{}_grid_{suffix}", directory.display()))
}

fn plan_path(directory: &Path) -> PathBuf {
    PathBuf::from(format!("{}_grid_grid.kind1.json", directory.display()))
}

/// Split the decoded kind-1 payload into its four exported byte planes and
/// the one-bit sentinel mask.
pub fn transform(decoded: &[u8]) -> Result<([Vec<u8>; 4], Vec<u8>)> {
    if decoded.len() != DECODED_SIZE {
        return Err(err("kind-1 map input must contain three 128x128 planes"));
    }
    let plane_a = &decoded[..PLANE];
    let plane_b = &decoded[PLANE..2 * PLANE];
    let values = &decoded[2 * PLANE..];
    let mut planes = std::array::from_fn(|_| vec![0u8; PLANE]);
    let mut mask = vec![0u8; PLANE];
    let mut ordinal = 0usize;
    for index in 0..PLANE {
        let value_offset = index * 2;
        let mut value = u16::from_le_bytes([values[value_offset], values[value_offset + 1]]);
        if value & 0x0fff == 0x0fff {
            mask[index] = 1;
            if ordinal < 0x0fff {
                value = (value & 0xf000) | ordinal as u16;
                ordinal += 1;
            }
        }
        planes[0][index] = value as u8;
        planes[1][index] = (value >> 8) as u8;
        planes[2][index] = plane_a[index];
        planes[3][index] = plane_b[index];
    }
    Ok((planes, mask))
}

/// Reconstruct the original four-plane decoded payload from exported PNG
/// planes and its binary sentinel mask.
pub fn inverse(planes: &[Vec<u8>], mask: &[u8]) -> Result<Vec<u8>> {
    if planes.len() != 4 || planes.iter().any(|plane| plane.len() != PLANE) {
        return Err(err("map source must contain four 128x128 byte planes"));
    }
    if mask.len() != PLANE || mask.iter().any(|value| *value != 0 && *value != 1) {
        return Err(err("map sentinel mask is invalid"));
    }
    let mut values = vec![0u8; 2 * PLANE];
    let mut ordinal = 0usize;
    for index in 0..PLANE {
        let value = u16::from(planes[0][index]) | (u16::from(planes[1][index]) << 8);
        let mut restored = value;
        if mask[index] != 0 {
            let expected = if ordinal < 0x0fff {
                (value & 0xf000) | ordinal as u16
            } else {
                value
            };
            if value != expected {
                return Err(err("sentinel mask disagrees with transformed value"));
            }
            restored = (value & 0xf000) | 0x0fff;
            if ordinal < 0x0fff {
                ordinal += 1;
            }
        } else if value & 0x0fff == 0x0fff {
            return Err(err("unmasked source value collides with sentinel form"));
        }
        values[index * 2..index * 2 + 2].copy_from_slice(&restored.to_le_bytes());
    }
    let mut decoded = Vec::with_capacity(DECODED_SIZE);
    decoded.extend_from_slice(&planes[2]);
    decoded.extend_from_slice(&planes[3]);
    decoded.extend_from_slice(&values);
    Ok(decoded)
}

/// Encode the sentinel mask in the same 1-bit indexed PNG form as the
/// TypeScript `mask_png` helper.
pub fn mask_png(mask: &[u8]) -> Result<Vec<u8>> {
    if mask.len() != PLANE || mask.iter().any(|value| *value != 0 && *value != 1) {
        return Err(err("sentinel mask must contain 128x128 binary pixels"));
    }
    let mut rows = vec![0u8; HEIGHT * 17];
    let mut cursor = 0usize;
    for y in 0..HEIGHT {
        cursor += 1;
        for x in (0..WIDTH).step_by(8) {
            let mut value = 0u8;
            for bit in 0..8 {
                value |= mask[y * WIDTH + x + bit] << (7 - bit);
            }
            rows[cursor] = value;
            cursor += 1;
        }
    }
    let mut header = [0u8; 13];
    header[0..4].copy_from_slice(&(WIDTH as u32).to_be_bytes());
    header[4..8].copy_from_slice(&(HEIGHT as u32).to_be_bytes());
    header[8] = 1;
    header[9] = 3;
    let compressed = deflate_sync(&rows, DeflateOptions { level: Some(9) });
    let mut output = b"\x89PNG\r\n\x1a\n".to_vec();
    output.extend_from_slice(&chunk(b"IHDR", &header));
    output.extend_from_slice(&chunk(b"PLTE", b"\0\0\0\xff\xff\xff"));
    output.extend_from_slice(&chunk(b"IDAT", &compressed));
    output.extend_from_slice(&chunk(b"IEND", &[]));
    Ok(output)
}

fn read_plane(path: &Path) -> Result<Vec<u8>> {
    let image = indexed_png(&read(path)?).map_err(|error| error.0)?;
    if image.width != WIDTH as u32 || image.height != HEIGHT as u32 || image.pixels.len() != PLANE {
        return Err(err(format!(
            "{}: expected one 128x128 byte plane",
            path.display()
        )));
    }
    Ok(image.pixels.into_iter().map(|value| value as u8).collect())
}

fn read_mask(path: &Path) -> Result<Vec<u8>> {
    let image = indexed_png(&read(path)?).map_err(|error| error.0)?;
    if image.pixels.len() != PLANE {
        return Err(err("map sentinel mask is invalid"));
    }
    image
        .pixels
        .into_iter()
        .map(|value| {
            if value == 0 || value == 1 {
                Ok(value as u8)
            } else {
                Err(err("map sentinel mask is invalid"))
            }
        })
        .collect()
}

fn token_value(token: &PaletteGroup) -> Value {
    match token {
        PaletteGroup::Zeros => json!(["z"]),
        PaletteGroup::Group(operations) => json!([
            "g",
            operations.iter().map(operation_value).collect::<Vec<_>>()
        ]),
    }
}

fn operation_value(operation: &PaletteOperation) -> Value {
    match operation {
        PaletteOperation::Literal => json!(["l"]),
        PaletteOperation::End => json!(["e"]),
        PaletteOperation::Copy { length, distance } => json!(["c", length, distance]),
    }
}

fn number(value: &Value, label: &str) -> Result<u64> {
    value
        .as_u64()
        .ok_or_else(|| err(format!("invalid {label}")))
}

fn text<'a>(value: &'a Value, label: &str) -> Result<&'a str> {
    value
        .as_str()
        .ok_or_else(|| err(format!("invalid {label}")))
}

fn parse_operation(value: &Value, index: usize) -> Result<PaletteOperation> {
    let operation = value
        .as_array()
        .ok_or_else(|| err(format!("palette token operation {index} must be an array")))?;
    match operation.first().and_then(Value::as_str) {
        Some("l") if operation.len() == 1 => Ok(PaletteOperation::Literal),
        Some("e") if operation.len() == 1 => Ok(PaletteOperation::End),
        Some("c") if operation.len() == 3 => {
            let length = number(&operation[1], "palette copy length")?;
            let distance = number(&operation[2], "palette copy distance")?;
            Ok(PaletteOperation::Copy {
                length: length as u32,
                distance: distance as u32,
            })
        }
        _ => Err(err(format!("palette token operation {index} is invalid"))),
    }
}

fn parse_tokens(value: &Value) -> Result<Vec<PaletteGroup>> {
    let tokens = value
        .as_array()
        .ok_or_else(|| err("palette tokens must be an array"))?;
    tokens
        .iter()
        .enumerate()
        .map(|(index, token)| {
            let token = token
                .as_array()
                .ok_or_else(|| err(format!("palette token {index} must be an array")))?;
            match token.first().and_then(Value::as_str) {
                Some("z") if token.len() == 1 => Ok(PaletteGroup::Zeros),
                Some("g") if token.len() == 2 => {
                    let operations = token[1].as_array().ok_or_else(|| {
                        err(format!("palette token group {index} must be an array"))
                    })?;
                    Ok(PaletteGroup::Group(
                        operations
                            .iter()
                            .enumerate()
                            .map(|(operation_index, operation)| {
                                parse_operation(operation, operation_index)
                            })
                            .collect::<Result<Vec<_>>>()?,
                    ))
                }
                _ => Err(err(format!("palette token {index} is invalid"))),
            }
        })
        .collect()
}

fn plan_value(
    decoded_size: usize,
    encoded_size: usize,
    tokens: &[PaletteGroup],
    lookahead: &str,
) -> Value {
    json!({
        "format": 1,
        "codec": "golden-sun-kind1-grid",
        "decoded_size": decoded_size,
        "encoded_size": encoded_size,
        "tokens": tokens.iter().map(token_value).collect::<Vec<_>>(),
        "lookahead": lookahead,
    })
}

fn plan_parts(plan: &Value) -> Result<(Vec<PaletteGroup>, String)> {
    let object: &Map<String, Value> = plan
        .as_object()
        .ok_or_else(|| err("kind-1 grid plan must be an object"))?;
    if number(
        object.get("format").ok_or_else(|| err("invalid format"))?,
        "format",
    )? != 1
        || text(
            object.get("codec").ok_or_else(|| err("invalid codec"))?,
            "codec",
        )? != "golden-sun-kind1-grid"
    {
        return Err(err("unsupported kind-1 grid plan"));
    }
    let tokens = parse_tokens(object.get("tokens").ok_or_else(|| err("invalid tokens"))?)?;
    let lookahead = object
        .get("lookahead")
        .map(|value| text(value, "lookahead").map(str::to_owned))
        .transpose()?
        .unwrap_or_default();
    Ok((tokens, lookahead))
}

pub fn build_grid(plan: &Value, directory: &Path) -> Result<Vec<u8>> {
    let (tokens, lookahead) = plan_parts(plan)?;
    let planes = FILES.map(|name| read_plane(&grid_path(directory, name)));
    let planes = planes.into_iter().collect::<Result<Vec<_>>>()?;
    let mask = read_mask(&grid_path(directory, "sentinels.png"))?;
    let decoded = inverse(&planes, &mask)?;
    let mut encoded = vec![1u8];
    encoded.extend_from_slice(&encode_palette(&decoded, &tokens).map_err(|error| error.0)?);
    encoded.extend_from_slice(
        &hex::decode(&lookahead).map_err(|_| err("kind-1 lookahead is not hexadecimal"))?,
    );
    Ok(encoded)
}

pub fn export_grid(data: &[u8], directory: &Path) -> Result<ExportStats> {
    if data.is_empty() || data[0] != 1 {
        return Err(err("kind-1 map grid must begin with byte 1"));
    }
    let (decoded, used, tokens) =
        decode_palette_trace(data, 1, data.len(), 0x10000).map_err(|error| error.0)?;
    let (planes, mask) = transform(&decoded)?;
    fs::create_dir_all(directory).map_err(|error| format!("{}: {error}", directory.display()))?;
    for (index, name) in FILES.iter().enumerate() {
        let image = byte_png(&planes[index], WIDTH as f64)
            .map_err(|error| error.0)?
            .0;
        write(&grid_path(directory, name), &image)?;
    }
    write(&grid_path(directory, "sentinels.png"), &mask_png(&mask)?)?;
    let lookahead = import_asset::hex(&data[used..]);
    let plan = plan_value(decoded.len(), data.len(), &tokens, &lookahead);
    let plan_text = serde_json::to_string(&plan).map_err(|error| error.to_string())?;
    write(&plan_path(directory), format!("{plan_text}\n").as_bytes())?;
    let rebuilt = build_grid(&plan, directory)?;
    if rebuilt != data {
        return Err(err("exported kind-1 grid does not round-trip"));
    }
    Ok(ExportStats {
        tokens: tokens.len(),
        sentinels: mask.iter().filter(|value| **value != 0).count(),
        encoded: data.len(),
    })
}

pub fn verify_grid(
    rom: &[u8],
    address: usize,
    size: usize,
    directory: &Path,
) -> Result<ExportStats> {
    let data = rom_range(rom, address, size)?;
    let plan_text = fs::read_to_string(plan_path(directory)).map_err(|error| error.to_string())?;
    let plan: Value = serde_json::from_str(&plan_text).map_err(|error| error.to_string())?;
    let rebuilt = build_grid(&plan, directory)?;
    if rebuilt != data {
        return Err(err("kind-1 grid source differs from ROM"));
    }
    let (decoded, _, tokens) =
        decode_palette_trace(&data, 1, data.len(), 0x10000).map_err(|error| error.0)?;
    let (_, mask) = transform(&decoded)?;
    Ok(ExportStats {
        tokens: tokens.len(),
        sentinels: mask.iter().filter(|value| **value != 0).count(),
        encoded: data.len(),
    })
}

fn rom_range(rom: &[u8], address: usize, size: usize) -> Result<&[u8]> {
    if address < ROM_BASE {
        return Err(err("kind-1 grid range is outside the ROM"));
    }
    let start = address - ROM_BASE;
    let end = start
        .checked_add(size)
        .ok_or_else(|| err("kind-1 grid range is outside the ROM"))?;
    if end > rom.len() {
        return Err(err("kind-1 grid range is outside the ROM"));
    }
    Ok(&rom[start..end])
}

pub fn self_test() -> Result<()> {
    let mut decoded = vec![0u8; DECODED_SIZE];
    for (index, value) in [0x0fff_u16, 0x2fff, 0x1234, 0x4fff].into_iter().enumerate() {
        decoded[2 * PLANE + index * 2..2 * PLANE + index * 2 + 2]
            .copy_from_slice(&value.to_le_bytes());
    }
    let (planes, mask) = transform(&decoded)?;
    if inverse(&planes.to_vec(), &mask)? != decoded
        || mask.iter().filter(|value| **value != 0).count() != 3
    {
        return Err(err("kind-1 map transform self-test failed"));
    }
    if mask_png(&mask)?.is_empty() {
        return Err(err("kind-1 mask PNG self-test failed"));
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn transform_round_trip_matches_typescript_contract() {
        self_test().unwrap();
    }

    #[test]
    fn mask_png_has_expected_indexed_dimensions() {
        let mask = vec![0u8; PLANE];
        let png = mask_png(&mask).unwrap();
        let image = indexed_png(&png).unwrap();
        assert_eq!((image.width, image.height), (128, 128));
        assert!(image.pixels.iter().all(|value| *value == 0));
    }
}

mod hex {
    pub fn decode(value: &str) -> Result<Vec<u8>, ()> {
        if value.len() % 2 != 0 {
            return Err(());
        }
        (0..value.len())
            .step_by(2)
            .map(|index| u8::from_str_radix(&value[index..index + 2], 16).map_err(|_| ()))
            .collect()
    }
}
