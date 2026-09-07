//! Rebuild kind-1 map grids from tracked PNG planes and palette-codec plans.
//!
//! Both the five-layer atlas and the older split-plane representation retain
//! the binary sentinel mask needed to reverse exported ordinal values.

use std::fs;
use std::path::{Path, PathBuf};

use extract_resource::{encode_palette, PaletteGroup, PaletteOperation};
use import_asset::indexed_png;
use serde_json::{Map, Value};

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

fn err(message: impl Into<String>) -> String {
    message.into()
}

fn read(path: &Path) -> Result<Vec<u8>> {
    fs::read(path).map_err(|error| format!("{}: {error}", path.display()))
}

fn grid_path(directory: &Path, suffix: &str) -> PathBuf {
    PathBuf::from(format!("{}_grid_{suffix}", directory.display()))
}

/// Split the decoded kind-1 payload into its four exported byte planes and
/// the one-bit sentinel mask.
fn transform(decoded: &[u8]) -> Result<([Vec<u8>; 4], Vec<u8>)> {
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
    let (planes, mask) = if plan.get("atlas_layers").and_then(Value::as_u64) == Some(5) {
        let image =
            indexed_png(&read(&grid_path(directory, "layers.png"))?).map_err(|error| error.0)?;
        if image.width != WIDTH as u32
            || image.height != (HEIGHT * 5) as u32
            || image.pixels.len() != PLANE * 5
        {
            return Err(err(
                "map grid atlas must contain five stacked 128x128 layers",
            ));
        }
        let layers = image
            .pixels
            .chunks(PLANE)
            .map(|layer| layer.iter().map(|value| *value as u8).collect::<Vec<_>>())
            .collect::<Vec<_>>();
        (layers[..4].to_vec(), layers[4].clone())
    } else {
        let planes = FILES.map(|name| read_plane(&grid_path(directory, name)));
        (
            planes.into_iter().collect::<Result<Vec<_>>>()?,
            read_mask(&grid_path(directory, "sentinels.png"))?,
        )
    };
    let decoded = inverse(&planes, &mask)?;
    let mut encoded = vec![1u8];
    encoded.extend_from_slice(&encode_palette(&decoded, &tokens).map_err(|error| error.0)?);
    encoded.extend_from_slice(
        &hex::decode(&lookahead).map_err(|_| err("kind-1 lookahead is not hexadecimal"))?,
    );
    Ok(encoded)
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
    let mut invalid_mask = mask.clone();
    invalid_mask[0] = 2;
    if inverse(&planes, &invalid_mask).is_ok() || inverse(&planes, &mask[..PLANE - 1]).is_ok() {
        return Err(err("invalid kind-1 sentinel mask was accepted"));
    }
    Ok(())
}

#[test]
fn grid_transform_round_trip() -> Result<()> {
    self_test()
}
