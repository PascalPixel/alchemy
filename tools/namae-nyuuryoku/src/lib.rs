//! Native builder for the fixed Golden Sun name-entry screen package.
//!
//! This deliberately implements only the source-to-bytes and ROM verification
//! surface needed by the asset build boundary. Export remains with the legacy
//! asset worker until that boundary is removed.

use extract_resource::{encode_general, GeneralToken};
use serde_json::{Map, Value};
use std::fs;
use std::path::{Path, PathBuf};

pub const ROM_BASE: usize = 0x0800_0000;
pub const ADDRESS: usize = 0x0807_3852;
pub const END: usize = 0x0807_7000;
pub const SIZE: usize = END - ADDRESS;
const RESOURCE_TABLE: usize = 0x0807_3854;
const TILEMAP: usize = 0x0807_3864;
const TILE_POINTER: usize = 0x0807_3968;
const UI_TILE: usize = 0x0803_10a4;
const WIDTH: usize = 24;
const HEIGHT: usize = 13;
const TILEMAP_BYTES: usize = WIDTH * HEIGHT * 2;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);

impl std::fmt::Display for Error {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(&self.0)
    }
}
impl std::error::Error for Error {}
type Result<T> = std::result::Result<T, Error>;

fn fail<T>(message: impl Into<String>) -> Result<T> {
    Err(Error(message.into()))
}
fn object<'a>(value: &'a Value, label: &str) -> Result<&'a Map<String, Value>> {
    value
        .as_object()
        .ok_or_else(|| Error(format!("{label} must be an object")))
}
fn field<'a>(value: &'a Map<String, Value>, name: &str) -> Result<&'a Value> {
    value
        .get(name)
        .ok_or_else(|| Error(format!("missing {name}")))
}
fn string<'a>(value: &'a Map<String, Value>, name: &str) -> Result<&'a str> {
    field(value, name)?
        .as_str()
        .ok_or_else(|| Error(format!("{name} must be a string")))
}
fn number(value: &Value, label: &str) -> Result<usize> {
    if let Some(n) = value.as_u64() {
        return usize::try_from(n).map_err(|_| Error(format!("{label} must be an integer")));
    }
    if let Some(s) = value.as_str() {
        let n = s
            .strip_prefix("0x")
            .or_else(|| s.strip_prefix("0X"))
            .map_or_else(|| s.parse::<u64>(), |h| u64::from_str_radix(h, 16))
            .map_err(|_| Error(format!("{label} must be an integer")))?;
        return usize::try_from(n).map_err(|_| Error(format!("{label} must be an integer")));
    }
    fail(format!("{label} must be an integer"))
}
fn exact_keys(value: &Map<String, Value>, expected: &[&str], label: &str) -> Result<()> {
    let mut actual = value.keys().map(String::as_str).collect::<Vec<_>>();
    let mut expected = expected.to_vec();
    actual.sort_unstable();
    expected.sort_unstable();
    if actual != expected {
        return fail(format!("{label} has unknown fields"));
    }
    Ok(())
}
fn hex(value: usize) -> String {
    format!("0x{value:08x}")
}
fn align4(value: usize) -> usize {
    (value + 3) & !3
}

fn coordinates(value: &Value, label: &str) -> Result<std::collections::HashSet<(usize, usize)>> {
    let values = value
        .as_array()
        .ok_or_else(|| Error(format!("{label} must be an array")))?;
    let mut result = std::collections::HashSet::new();
    for coordinate in values {
        let pair = coordinate
            .as_array()
            .ok_or_else(|| Error(format!("{label} coordinate differs")))?;
        if pair.len() != 2 {
            return fail(format!("{label} coordinate differs"));
        }
        let row = number(&pair[0], &format!("{label} row"))?;
        let column = number(&pair[1], &format!("{label} column"))?;
        if row >= HEIGHT || column >= WIDTH {
            return fail(format!("{label} coordinate is outside the tilemap"));
        }
        if !result.insert((row, column)) {
            return fail(format!("{label} has a duplicate coordinate"));
        }
    }
    Ok(result)
}

fn tokens(value: &Value) -> Result<Vec<GeneralToken>> {
    let values = value
        .as_array()
        .ok_or_else(|| Error("name-entry token plan length differs".into()))?;
    if values.is_empty() || values.len() > TILEMAP_BYTES {
        return fail("name-entry token plan length differs");
    }
    values
        .iter()
        .enumerate()
        .map(|(index, token)| {
            let pair = token
                .as_array()
                .ok_or_else(|| Error(format!("name-entry token {index} opcode differs")))?;
            match pair.first().and_then(Value::as_str) {
                Some("l") if pair.len() == 2 => {
                    let count =
                        number(&pair[1], &format!("name-entry token {index} literal count"))?;
                    if !(1..=TILEMAP_BYTES).contains(&count) {
                        return fail(format!(
                            "name-entry token {index} literal count is outside its range"
                        ));
                    }
                    Ok(GeneralToken::Literal(count as u32))
                }
                Some("c") if pair.len() == 3 => {
                    let length =
                        number(&pair[1], &format!("name-entry token {index} copy length"))?;
                    let distance =
                        number(&pair[2], &format!("name-entry token {index} copy distance"))?;
                    if !(2..=137).contains(&length) {
                        return fail(format!(
                            "name-entry token {index} copy length is outside its range"
                        ));
                    }
                    if !(1..=TILEMAP_BYTES).contains(&distance) {
                        return fail(format!(
                            "name-entry token {index} copy distance is outside its range"
                        ));
                    }
                    Ok(GeneralToken::Copy {
                        length: length as u32,
                        distance: distance as u32,
                    })
                }
                _ => fail(format!("name-entry token {index} opcode differs")),
            }
        })
        .collect()
}

fn tilemap(source: &Map<String, Value>) -> Result<Vec<u8>> {
    exact_keys(
        source,
        &[
            "address",
            "codec",
            "decoded_size",
            "width",
            "height",
            "copied_width",
            "copied_height",
            "palette",
            "tiles",
            "horizontal_flips",
            "vertical_flips",
            "tokens",
        ],
        "name-entry tilemap",
    )?;
    if string(source, "address")? != hex(TILEMAP)
        || string(source, "codec")? != "golden-sun-general-lz"
        || number(field(source, "decoded_size")?, "decoded_size")? != TILEMAP_BYTES
        || number(field(source, "width")?, "width")? != WIDTH
        || number(field(source, "height")?, "height")? != HEIGHT
        || number(field(source, "copied_width")?, "copied_width")? != WIDTH
        || number(field(source, "copied_height")?, "copied_height")? != 9
        || number(field(source, "palette")?, "palette")? != 15
    {
        return fail("name-entry tilemap layout differs");
    }
    let rows = field(source, "tiles")?
        .as_array()
        .ok_or_else(|| Error("name-entry tilemap row extents differ".into()))?;
    if rows.len() != HEIGHT {
        return fail("name-entry tilemap row extents differ");
    }
    let horizontal = coordinates(field(source, "horizontal_flips")?, "horizontal flips")?;
    let vertical = coordinates(field(source, "vertical_flips")?, "vertical flips")?;
    let mut decoded = Vec::with_capacity(TILEMAP_BYTES);
    for (row_index, row) in rows.iter().enumerate() {
        let values = row
            .as_array()
            .ok_or_else(|| Error("name-entry tilemap row extents differ".into()))?;
        if values.len() != WIDTH {
            return fail("name-entry tilemap row extents differ");
        }
        for (column, raw) in values.iter().enumerate() {
            let tile = number(raw, "tile index")?;
            if tile > 0x03ff {
                return fail("tile index is outside the screen-entry range");
            }
            let mut entry = tile | (15 << 12);
            if horizontal.contains(&(row_index, column)) {
                entry |= 0x0400;
            }
            if vertical.contains(&(row_index, column)) {
                entry |= 0x0800;
            }
            decoded.extend_from_slice(&(entry as u16).to_le_bytes());
        }
    }
    let encoded =
        encode_general(&decoded, &tokens(field(source, "tokens")?)?).map_err(|e| Error(e.0))?;
    if align4(TILEMAP + encoded.len()) != TILE_POINTER {
        return fail("name-entry compressed stream does not reach its aligned pointer table");
    }
    Ok(encoded)
}

pub fn build_namae_nyuuryoku(path: &Path) -> Result<Vec<u8>> {
    let text = fs::read_to_string(path).map_err(|e| Error(format!("{}: {e}", path.display())))?;
    let value: Value =
        serde_json::from_str(&text).map_err(|e| Error(format!("{}: {e}", path.display())))?;
    let source = object(&value, "name-entry source")?;
    exact_keys(
        source,
        &[
            "format",
            "kind",
            "address",
            "end",
            "resource_ids",
            "tilemap",
            "ui_tile_address",
            "next_code_address",
        ],
        "name-entry source",
    )?;
    if number(field(source, "format")?, "format")? != 1
        || string(source, "kind")? != "golden-sun-namae-nyuuryoku"
        || string(source, "address")? != hex(ADDRESS)
        || string(source, "end")? != hex(END)
        || string(source, "ui_tile_address")? != hex(UI_TILE)
        || string(source, "next_code_address")? != hex(END)
    {
        return fail("name-entry source metadata differs");
    }
    let ids = field(source, "resource_ids")?
        .as_array()
        .ok_or_else(|| Error("name-entry resource IDs differ".into()))?;
    let expected = ["0x05a", "0x05b", "0x05c", "0x05d"];
    if ids.len() != expected.len() || ids.iter().zip(expected).any(|(v, e)| v.as_str() != Some(e)) {
        return fail("name-entry source metadata differs");
    }
    let encoded = tilemap(object(field(source, "tilemap")?, "name-entry tilemap")?)?;
    let mut output = vec![0u8; SIZE];
    let table = RESOURCE_TABLE - ADDRESS;
    for (index, id) in [0x05a_u32, 0x05b, 0x05c, 0x05d].into_iter().enumerate() {
        output[table + index * 4..table + index * 4 + 4].copy_from_slice(&id.to_le_bytes());
    }
    let start = TILEMAP - ADDRESS;
    output[start..start + encoded.len()].copy_from_slice(&encoded);
    output[TILE_POINTER - ADDRESS..TILE_POINTER - ADDRESS + 4]
        .copy_from_slice(&(UI_TILE as u32).to_le_bytes());
    Ok(output)
}

pub fn verify_namae_nyuuryoku(rom: &[u8], source: &Path) -> Result<()> {
    let start = ADDRESS
        .checked_sub(ROM_BASE)
        .ok_or_else(|| Error("ROM address is invalid".into()))?;
    let expected = rom
        .get(start..start + SIZE)
        .ok_or_else(|| Error("ROM is too small for name-entry data".into()))?;
    let built = build_namae_nyuuryoku(source)?;
    if built != expected {
        return fail("name-entry package differs from ROM");
    }
    Ok(())
}

pub fn self_test() -> Result<()> {
    let path = PathBuf::from(env!("CARGO_MANIFEST_DIR"))
        .join("../../assets/graphics/fonts_namae_nyuuryoku_gamen.json");
    if path.exists() && build_namae_nyuuryoku(&path)?.len() != SIZE {
        return fail("name-entry package size differs");
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn tracked_source_builds() {
        let path = PathBuf::from(env!("CARGO_MANIFEST_DIR"))
            .join("../../assets/graphics/fonts_namae_nyuuryoku_gamen.json");
        assert_eq!(build_namae_nyuuryoku(&path).unwrap().len(), SIZE);
    }
    #[test]
    fn extent_is_fixed() {
        assert_eq!(SIZE, 0x37ae);
    }
}
