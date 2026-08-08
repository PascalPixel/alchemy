//! Native source-to-ROM builder for `sentou_menu_index.json`.
//! The ROM-to-source exporter remains owned by the existing asset worker.

use canonical_json::is_canonical_json_text;
use import_asset::indexed_png;
use serde_json::Value;
use std::fs;
use std::path::{Path, PathBuf};

pub const ROM_BASE: usize = 0x0800_0000;
pub const ADDRESS: usize = 0x080b_3940;
pub const END: usize = 0x080b_5000;
pub const SIZE: usize = END - ADDRESS;

const GRAPHICS: &[(usize, usize, &str, usize, usize, usize, usize)] = &[
    (0x080b_3940, 0x300, "hyouji_00.4bpp.png", 6, 2, 2, 6),
    (0x080b_3c40, 0x100, "hyouji_01.4bpp.png", 2, 2, 2, 2),
    (0x080b_3d40, 0x140, "moji.4bpp.png", 10, 1, 1, 10),
    (0x080b_3e80, 0x100, "hyouji_02.4bpp.png", 2, 2, 2, 2),
    (0x080b_3f80, 0x180, "hyouji_03.4bpp.png", 3, 2, 2, 3),
];

pub type Result<T> = std::result::Result<T, Error>;
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);
impl std::fmt::Display for Error { fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result { f.write_str(&self.0) } }
impl std::error::Error for Error {}
fn fail<T>(message: impl Into<String>) -> Result<T> { Err(Error(message.into())) }

fn object<'a>(value: &'a Value, name: &str) -> Result<&'a serde_json::Map<String, Value>> {
    value.as_object().ok_or_else(|| Error(format!("{name} must be an object")))
}
fn array<'a>(value: Option<&'a Value>, count: usize, name: &str) -> Result<&'a [Value]> {
    let values = value.and_then(Value::as_array).ok_or_else(|| Error(format!("{name} requires {count} entries")))?;
    if values.len() != count { return fail(format!("{name} requires {count} entries")); }
    Ok(values)
}
fn integer(value: Option<&Value>, minimum: i64, maximum: i64, name: &str) -> Result<i64> {
    let number = value.and_then(Value::as_i64).ok_or_else(|| Error(format!("{name} is outside its range")))?;
    if number < minimum || number > maximum { return fail(format!("{name} is outside its range")); }
    Ok(number)
}
fn expected_keys(value: &Value, expected: &[&str], name: &str) -> Result<()> {
    let actual = object(value, name)?;
    let mut actual_keys = actual.keys().map(String::as_str).collect::<Vec<_>>();
    let mut expected_keys = expected.to_vec();
    actual_keys.sort_unstable(); expected_keys.sort_unstable();
    if actual_keys != expected_keys { return fail(format!("{name} has unexpected fields")); }
    Ok(())
}
fn read_json(path: &Path) -> Result<Value> {
    let text = fs::read_to_string(path).map_err(|e| Error(format!("{}: {e}", path.display())))?;
    let value: Value = serde_json::from_str(&text).map_err(|e| Error(format!("{}: {e}", path.display())))?;
    if !is_canonical_json_text(&text, &value) { return fail("battle-menu source is not canonical JSON"); }
    Ok(value)
}
fn hex(address: usize) -> String { format!("0x{address:08x}") }

fn atlas(path: &Path, spec: &(usize, usize, &str, usize, usize, usize, usize)) -> Result<Vec<u8>> {
    let (_, size, _, frames, tiles_wide, tiles_high, columns) = *spec;
    let image = indexed_png(&fs::read(path).map_err(|e| Error(format!("{}: {e}", path.display())))?)
        .map_err(|e| Error(e.0))?;
    let frame_width = tiles_wide * 8;
    let frame_height = tiles_high * 8;
    if image.width as usize != columns * frame_width || image.height as usize != frames.div_ceil(columns) * frame_height {
        return fail(format!("{}: atlas dimensions differ", path.display()));
    }
    let palette = (0..16).map(|value| { let value = (value * 8) as u8; [value, value, value] }).collect::<Vec<_>>();
    if image.palette != palette || image.pixels.iter().any(|value| *value >= 16) {
        return fail(format!("{}: symbolic palette differs", path.display()));
    }
    let mut output = Vec::with_capacity(size);
    for frame in 0..frames {
        let left = (frame % columns) * frame_width;
        let top = (frame / columns) * frame_height;
        for tile_y in 0..tiles_high { for tile_x in 0..tiles_wide { for y in 0..8 { for x in (0..8).step_by(2) {
            let offset = (top + tile_y * 8 + y) * image.width as usize + left + tile_x * 8 + x;
            output.push(image.pixels[offset] as u8 | (image.pixels[offset + 1] as u8) << 4);
        }}}}
    }
    if output.len() != size { return fail("battle-menu atlas size differs"); }
    Ok(output)
}

fn unsigned_halfwords(values: Option<&Value>, count: usize, maximum: i64, name: &str) -> Result<Vec<u8>> {
    let mut output = Vec::with_capacity(count * 2);
    for (index, value) in array(values, count, name)?.iter().enumerate() {
        output.extend_from_slice(&(integer(Some(value), 0, maximum, &format!("{name} {index}"))? as u16).to_le_bytes());
    }
    Ok(output)
}
fn signed_halfwords(values: Option<&Value>, count: usize, name: &str) -> Result<Vec<u8>> {
    let mut output = Vec::with_capacity(count * 2);
    for (index, value) in array(values, count, name)?.iter().enumerate() {
        output.extend_from_slice(&(integer(Some(value), -0x8000, 0x7fff, &format!("{name} {index}"))? as i16).to_le_bytes());
    }
    Ok(output)
}
fn signed_bytes(values: Option<&Value>, count: usize, name: &str) -> Result<Vec<u8>> {
    array(values, count, name)?.iter().enumerate().map(|(index, value)| {
        Ok(integer(Some(value), -0x80, 0x7f, &format!("{name} {index}"))? as i8 as u8)
    }).collect()
}
fn selector_ids(value: Option<&Value>) -> Result<Vec<u8>> {
    let mut ids = Vec::with_capacity(20);
    for (index, item) in array(value, 20, "selector IDs")?.iter().enumerate() {
        ids.push(integer(Some(item), 1, 0x7fff, &format!("selector ID {index}"))? as u16);
    }
    for left in 0..ids.len() { if ids[left + 1..].contains(&ids[left]) { return fail("selector IDs must be unique before their terminator"); } }
    Ok(ids.into_iter().flat_map(u16::to_le_bytes).collect())
}

fn loadouts(value: Option<&Value>) -> Result<Vec<u8>> {
    let entries = array(value, 35, "loadouts")?;
    let mut output = vec![0u8; 35 * 66];
    for (index, entry) in entries.iter().enumerate() {
        expected_keys(entry, &["main_ids", "extra_ids", "group"], &format!("loadout {index}"))?;
        let item = object(entry, "loadout")?;
        let main = item.get("main_ids").and_then(Value::as_array).ok_or_else(|| Error(format!("loadout {index} main IDs invalid")))?;
        let extra = item.get("extra_ids").and_then(Value::as_array).ok_or_else(|| Error(format!("loadout {index} extra IDs invalid")))?;
        if main.len() > 23 || extra.len() > 7 { return fail(format!("loadout {index} exceeds its terminated fields")); }
        let mut main_seen = Vec::new();
        for (position, value) in main.iter().enumerate() {
            let id = integer(Some(value), 1, 0x7fff, &format!("loadout {index} main ID {position}"))? as u16;
            if main_seen.contains(&id) { return fail(format!("loadout {index} contains duplicate IDs")); }
            main_seen.push(id);
            let offset = index * 66 + position * 2;
            output[offset..offset + 2].copy_from_slice(&id.to_le_bytes());
        }
        let mut extra_seen = Vec::new();
        for (position, value) in extra.iter().enumerate() {
            let id = integer(Some(value), 1, 0x7fff, &format!("loadout {index} extra ID {position}"))? as u16;
            if extra_seen.contains(&id) { return fail(format!("loadout {index} contains duplicate IDs")); }
            extra_seen.push(id);
            let offset = index * 66 + 48 + position * 2;
            output[offset..offset + 2].copy_from_slice(&id.to_le_bytes());
        }
        let group = integer(item.get("group"), 0, 2, &format!("loadout {index} group"))? as u16;
        output[index * 66 + 64..index * 66 + 66].copy_from_slice(&group.to_le_bytes());
    }
    Ok(output)
}

pub fn build_sentou_menu_data(index: &Path) -> Result<Vec<u8>> {
    let source = read_json(index)?;
    expected_keys(&source, &["format", "kind", "address", "end", "size", "graphics", "cell_offsets", "row_offsets", "thresholds", "selector_ids", "loadouts", "sound_ids", "multipliers", "alignment"], "battle-menu source")?;
    let item = object(&source, "battle-menu source")?;
    if item.get("format").and_then(Value::as_i64) != Some(1)
        || item.get("kind").and_then(Value::as_str) != Some("golden-sun-sentou-menu-data")
        || item.get("address").and_then(Value::as_str) != Some(&hex(ADDRESS))
        || item.get("end").and_then(Value::as_str) != Some(&hex(END))
        || item.get("size").and_then(Value::as_u64) != Some(SIZE as u64)
    { return fail("battle-menu extent differs"); }
    let graphics = array(item.get("graphics"), GRAPHICS.len(), "battle-menu graphics")?;
    let prefix = index.file_name().and_then(|name| name.to_str()).unwrap_or("").strip_suffix("index.json").unwrap_or("");
    let root = index.parent().unwrap_or_else(|| Path::new("."));
    let mut output = Vec::with_capacity(SIZE);
    for (position, entry) in graphics.iter().enumerate() {
        expected_keys(entry, &["address", "size", "source", "frames", "frame_tiles_wide", "frame_tiles_high", "columns"], &format!("battle-menu graphic {position}"))?;
        let graphic = object(entry, "battle-menu graphic")?;
        let spec = GRAPHICS[position];
        if graphic.get("address").and_then(Value::as_str) != Some(&hex(spec.0))
            || graphic.get("size").and_then(Value::as_u64) != Some(spec.1 as u64)
            || graphic.get("source").and_then(Value::as_str) != Some(spec.2)
            || graphic.get("frames").and_then(Value::as_u64) != Some(spec.3 as u64)
            || graphic.get("frame_tiles_wide").and_then(Value::as_u64) != Some(spec.4 as u64)
            || graphic.get("frame_tiles_high").and_then(Value::as_u64) != Some(spec.5 as u64)
            || graphic.get("columns").and_then(Value::as_u64) != Some(spec.6 as u64)
        { return fail(format!("battle-menu graphic {position} layout differs")); }
        let source_name = graphic.get("source").and_then(Value::as_str).unwrap();
        output.extend(atlas(&root.join(format!("{prefix}{source_name}")), &spec)?);
    }
    output.extend(unsigned_halfwords(item.get("cell_offsets"), 30, 231, "cell offsets")?);
    output.extend(unsigned_halfwords(item.get("row_offsets"), 5, 978, "row offsets")?);
    for row in array(item.get("thresholds"), 6, "threshold rows")? { output.extend(signed_halfwords(Some(row), 5, "threshold row")?); }
    output.extend(selector_ids(item.get("selector_ids"))?);
    output.extend([0, 0]);
    output.extend(loadouts(item.get("loadouts"))?);
    for (index, value) in array(item.get("sound_ids"), 4, "sound IDs")?.iter().enumerate() {
        output.push(integer(Some(value), 1, 0x7f, &format!("sound ID {index}"))? as u8);
    }
    output.extend(signed_bytes(item.get("multipliers"), 13, "multipliers")?);
    let alignment = object(item.get("alignment").unwrap(), "battle-menu alignment")?;
    if alignment.get("address").and_then(Value::as_str) != Some("0x080b4ac3")
        || alignment.get("end").and_then(Value::as_str) != Some(&hex(END))
        || alignment.get("fill").and_then(Value::as_i64) != Some(0)
    { return fail("battle-menu alignment differs"); }
    output.resize(SIZE, 0);
    if output.len() != SIZE { return fail("battle-menu package size differs"); }
    Ok(output)
}

pub fn verify_sentou_menu_data(rom: &[u8], index: &Path) -> Result<()> {
    let start = ADDRESS.checked_sub(ROM_BASE).ok_or_else(|| Error("ROM address is invalid".into()))?;
    let expected = rom.get(start..start + SIZE).ok_or_else(|| Error("ROM is too small for battle-menu data".into()))?;
    if build_sentou_menu_data(index)? != expected { return fail("battle-menu package differs from ROM"); }
    Ok(())
}

pub fn self_test() -> Result<()> {
    let index = PathBuf::from(env!("CARGO_MANIFEST_DIR")).join("../../assets/data/sentou_menu_index.json");
    if index.exists() && build_sentou_menu_data(&index)?.len() != SIZE { return fail("battle-menu self-test size differs"); }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn tracked_package_builds() {
        let index = PathBuf::from(env!("CARGO_MANIFEST_DIR")).join("../../assets/data/sentou_menu_index.json");
        assert_eq!(build_sentou_menu_data(&index).unwrap().len(), SIZE);
    }
    #[test]
    fn constants_match_source_extent() { assert_eq!(END - ADDRESS, 5824); }
}
