//! Native builder for the Golden Sun staff-roll package.
//!
//! The package is a fixed ROM extent: a preload table, aligned ASCII strings,
//! glyph widths, line pointers, and a one-bit font atlas.  This crate owns the
//! build and ROM-verification boundary used by the asset builder.

use std::fs;
use std::path::{Path, PathBuf};

use canonical_json::is_canonical_json_text;
use import_asset::indexed_png;
use serde_json::Value;

pub const ROM_BASE: u32 = 0x0800_0000;
pub const STAFF_ROLL_ADDRESS: u32 = 0x080f_0a5c;
pub const STAFF_ROLL_END: u32 = 0x080f_2000;
pub const STAFF_ROLL_SIZE: usize = (STAFF_ROLL_END - STAFF_ROLL_ADDRESS) as usize;
const RESOURCE_END: u32 = 0x080f_0ae0;
const TEXT_ADDRESS: u32 = 0x080f_0ae0;
const WIDTH_ADDRESS: u32 = 0x080f_11bd;
const WIDTH_END: u32 = 0x080f_121d;
const LINE_ADDRESS: u32 = 0x080f_1220;
const FONT_ADDRESS: u32 = 0x080f_1770;
const ALIGNMENT_ADDRESS: u32 = 0x080f_1a70;
const RESOURCE_COUNT: usize = 32;
const STRING_COUNT: usize = 110;
const LINE_COUNT: usize = 339;
const GLYPH_COUNT: usize = 96;
const FONT_COLUMNS: usize = 16;
const FONT_SOURCE: &str = "moji.1bpp.png";

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);

impl std::fmt::Display for Error {
    fn fmt(&self, output: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        output.write_str(&self.0)
    }
}
impl std::error::Error for Error {}

type Result<T> = std::result::Result<T, Error>;

fn err(message: impl Into<String>) -> Error {
    Error(message.into())
}

fn hex(value: u32) -> String {
    format!("0x{value:08x}")
}

fn object<'a>(value: &'a Value, label: &str) -> Result<&'a serde_json::Map<String, Value>> {
    value
        .as_object()
        .ok_or_else(|| err(format!("{label} must be an object")))
}

fn exact_keys(
    value: &serde_json::Map<String, Value>,
    expected: &[&str],
    label: &str,
) -> Result<()> {
    let mut actual: Vec<&str> = value.keys().map(String::as_str).collect();
    actual.sort_unstable();
    let mut wanted = expected.to_vec();
    wanted.sort_unstable();
    if actual != wanted {
        return Err(err(format!("{label} has unknown or missing fields")));
    }
    Ok(())
}

fn field<'a>(value: &'a Value, name: &str) -> Result<&'a Value> {
    value
        .get(name)
        .ok_or_else(|| err(format!("staff-roll field {name} is missing")))
}

fn text_field<'a>(value: &'a Value, name: &str) -> Result<&'a str> {
    field(value, name)?
        .as_str()
        .ok_or_else(|| err(format!("staff-roll field {name} is not a string")))
}

fn integer(value: &Value, minimum: u32, maximum: u32, name: &str) -> Result<u32> {
    let number = value
        .as_u64()
        .or_else(|| {
            text_field(value, name).ok().and_then(|text| {
                text.strip_prefix("0x")
                    .or_else(|| text.strip_prefix("0X"))
                    .map_or_else(
                        || text.parse().ok(),
                        |hex| u64::from_str_radix(hex, 16).ok(),
                    )
            })
        })
        .ok_or_else(|| err(format!("{name} is outside its range")))?;
    if number < u64::from(minimum) || number > u64::from(maximum) {
        return Err(err(format!("{name} is outside its range")));
    }
    Ok(number as u32)
}

fn read_document(path: &Path) -> Result<Value> {
    let text = fs::read_to_string(path).map_err(|e| err(format!("{}: {e}", path.display())))?;
    let value: Value =
        serde_json::from_str(&text).map_err(|e| err(format!("{}: {e}", path.display())))?;
    if !is_canonical_json_text(&text, &value) {
        return Err(err(format!(
            "{}: source is not canonical JSON",
            path.display()
        )));
    }
    let map = object(&value, "staff-roll source")?;
    exact_keys(
        map,
        &[
            "format",
            "kind",
            "address",
            "end",
            "size",
            "preload_table",
            "text_pool",
            "glyph_widths",
            "lines",
            "font",
            "alignment",
        ],
        "staff-roll source",
    )?;
    if field(&value, "format")?.as_u64() != Some(1)
        || text_field(&value, "kind")? != "golden-sun-staff-roll"
        || text_field(&value, "address")? != hex(STAFF_ROLL_ADDRESS)
        || text_field(&value, "end")? != hex(STAFF_ROLL_END)
        || field(&value, "size")?.as_u64() != Some(STAFF_ROLL_SIZE as u64)
    {
        return Err(err("staff-roll extent differs"));
    }
    Ok(value)
}

fn list<'a>(value: &'a Value, name: &str) -> Result<&'a Vec<Value>> {
    field(value, name)?
        .as_array()
        .ok_or_else(|| err(format!("{name} requires an array")))
}

fn build_inner(index_path: &Path) -> Result<Vec<u8>> {
    let source = read_document(index_path)?;
    let preload = field(&source, "preload_table")?;
    exact_keys(
        object(preload, "preload resource table")?,
        &["address", "end", "slots", "resource_ids", "terminator"],
        "preload resource table",
    )?;
    if text_field(preload, "address")? != hex(STAFF_ROLL_ADDRESS)
        || text_field(preload, "end")? != hex(RESOURCE_END)
        || field(preload, "slots")?.as_u64() != Some(33)
        || field(preload, "terminator")?.as_u64() != Some(0)
    {
        return Err(err("preload resource-table layout differs"));
    }
    let ids = list(preload, "resource_ids")?;
    if ids.len() != RESOURCE_COUNT {
        return Err(err("preload resource IDs requires 32 entries"));
    }
    let mut output = vec![0u8; STAFF_ROLL_SIZE];
    for (i, id) in ids.iter().enumerate() {
        output[i * 4..i * 4 + 4].copy_from_slice(
            &integer(id, 1, 0xffff, &format!("preload resource ID {i}"))?.to_le_bytes(),
        );
    }

    let text_pool = field(&source, "text_pool")?;
    exact_keys(
        object(text_pool, "staff-roll text pool")?,
        &["address", "end", "alignment", "strings"],
        "staff-roll text pool",
    )?;
    if text_field(text_pool, "address")? != hex(TEXT_ADDRESS)
        || text_field(text_pool, "end")? != hex(WIDTH_ADDRESS)
        || field(text_pool, "alignment")?.as_u64() != Some(4)
    {
        return Err(err("staff-roll text-pool layout differs"));
    }
    let strings = list(text_pool, "strings")?;
    if strings.len() != STRING_COUNT {
        return Err(err("staff-roll strings requires 110 entries"));
    }
    let mut addresses = Vec::with_capacity(STRING_COUNT);
    let mut cursor = TEXT_ADDRESS;
    for (i, item) in strings.iter().enumerate() {
        let string = item
            .as_str()
            .ok_or_else(|| err(format!("staff-roll string {i} is not printable ASCII")))?;
        if !string.bytes().all(|byte| (0x20..=0x7e).contains(&byte)) {
            return Err(err(format!("staff-roll string {i} is not printable ASCII")));
        }
        addresses.push(cursor);
        let start = (cursor - STAFF_ROLL_ADDRESS) as usize;
        output[start..start + string.len()].copy_from_slice(string.as_bytes());
        output[start + string.len()] = 0;
        cursor += string.len() as u32 + 1;
        if i + 1 != strings.len() {
            let padding = (4 - (cursor & 3)) & 3;
            cursor += padding;
        }
    }
    if cursor != WIDTH_ADDRESS {
        return Err(err("staff-roll text pool has the wrong encoded size"));
    }

    let widths = field(&source, "glyph_widths")?;
    let width_map = object(widths, "staff-roll glyph widths")?;
    exact_keys(
        width_map,
        &["address", "end", "count", "values", "padding"],
        "staff-roll glyph widths",
    )?;
    let padding = field(widths, "padding")?;
    exact_keys(
        object(padding, "staff-roll glyph-width padding")?,
        &["end", "size", "fill"],
        "staff-roll glyph-width padding",
    )?;
    if text_field(widths, "address")? != hex(WIDTH_ADDRESS)
        || text_field(widths, "end")? != hex(WIDTH_END)
        || field(widths, "count")?.as_u64() != Some(96)
        || text_field(padding, "end")? != hex(LINE_ADDRESS)
        || field(padding, "size")?.as_u64() != Some(3)
        || field(padding, "fill")?.as_u64() != Some(0)
    {
        return Err(err("staff-roll glyph-width layout differs"));
    }
    let values = list(widths, "values")?;
    if values.len() != GLYPH_COUNT {
        return Err(err("staff-roll glyph widths requires 96 entries"));
    }
    for (i, value) in values.iter().enumerate() {
        output[(WIDTH_ADDRESS - STAFF_ROLL_ADDRESS) as usize + i] =
            integer(value, 1, 8, &format!("staff-roll glyph width {i}"))? as u8;
    }

    let lines = field(&source, "lines")?;
    exact_keys(
        object(lines, "staff-roll lines")?,
        &["address", "count", "string_indices", "terminator"],
        "staff-roll lines",
    )?;
    if text_field(lines, "address")? != hex(LINE_ADDRESS)
        || field(lines, "count")?.as_u64() != Some(LINE_COUNT as u64)
        || field(lines, "terminator")?.as_u64() != Some(0)
    {
        return Err(err("staff-roll line-table layout differs"));
    }
    let indices = list(lines, "string_indices")?;
    if indices.len() != LINE_COUNT {
        return Err(err("staff-roll line indices requires 339 entries"));
    }
    for (i, item) in indices.iter().enumerate() {
        let index = integer(
            item,
            0,
            (STRING_COUNT - 1) as u32,
            &format!("staff-roll line index {i}"),
        )? as usize;
        output[(LINE_ADDRESS - STAFF_ROLL_ADDRESS) as usize + i * 4
            ..(LINE_ADDRESS - STAFF_ROLL_ADDRESS) as usize + i * 4 + 4]
            .copy_from_slice(&addresses[index].to_le_bytes());
    }

    let font = field(&source, "font")?;
    exact_keys(
        object(font, "staff-roll font")?,
        &[
            "address",
            "end",
            "source",
            "encoding",
            "first_code",
            "glyphs",
            "columns",
        ],
        "staff-roll font",
    )?;
    if text_field(font, "address")? != hex(FONT_ADDRESS)
        || text_field(font, "end")? != hex(ALIGNMENT_ADDRESS)
        || text_field(font, "source")? != FONT_SOURCE
        || text_field(font, "encoding")? != "gba-1bpp-msb-glyph-rows"
        || field(font, "first_code")?.as_u64() != Some(32)
        || field(font, "glyphs")?.as_u64() != Some(96)
        || field(font, "columns")?.as_u64() != Some(16)
    {
        return Err(err("staff-roll font layout differs"));
    }
    let prefix = index_path
        .file_name()
        .and_then(|name| name.to_str())
        .and_then(|name| name.strip_suffix("index.json"))
        .ok_or_else(|| err("staff-roll index filename differs"))?;
    let font_path = index_path
        .parent()
        .unwrap_or_else(|| Path::new("."))
        .join(format!("{prefix}{FONT_SOURCE}"));
    let image = indexed_png(
        &fs::read(&font_path).map_err(|e| err(format!("{}: {e}", font_path.display())))?,
    )
    .map_err(|e| err(e.0))?;
    if image.width != 128
        || image.height != 48
        || image.palette != vec![[0, 0, 0], [255, 255, 255]]
        || image.pixels.iter().any(|pixel| *pixel > 1)
    {
        return Err(err("staff-roll font differs"));
    }
    let font_offset = (FONT_ADDRESS - STAFF_ROLL_ADDRESS) as usize;
    for glyph in 0..GLYPH_COUNT {
        let left = glyph % FONT_COLUMNS * 8;
        let top = glyph / FONT_COLUMNS * 8;
        for y in 0..8 {
            let mut row = 0u8;
            for x in 0..8 {
                row |= (image.pixels[(top + y) * image.width as usize + left + x] as u8) << (7 - x);
            }
            output[font_offset + glyph * 8 + y] = row;
        }
    }

    let alignment = field(&source, "alignment")?;
    exact_keys(
        object(alignment, "staff-roll alignment")?,
        &["address", "end", "fill"],
        "staff-roll alignment",
    )?;
    if text_field(alignment, "address")? != hex(ALIGNMENT_ADDRESS)
        || text_field(alignment, "end")? != hex(STAFF_ROLL_END)
        || field(alignment, "fill")?.as_u64() != Some(0)
    {
        return Err(err("staff-roll alignment differs"));
    }
    Ok(output)
}

pub fn build_staff_roll(index_path: &Path) -> Result<Vec<u8>> {
    build_inner(index_path)
}

pub fn verify_staff_roll(rom_path: &Path, index_path: &Path) -> Result<usize> {
    let rom = fs::read(rom_path).map_err(|e| err(format!("{}: {e}", rom_path.display())))?;
    let offset = (STAFF_ROLL_ADDRESS - ROM_BASE) as usize;
    let end = offset + STAFF_ROLL_SIZE;
    if rom.len() < end {
        return Err(err("ROM is shorter than the staff-roll region"));
    }
    let built = build_inner(index_path)?;
    if built != rom[offset..end] {
        return Err(err("staff-roll source differs from ROM"));
    }
    Ok(STAFF_ROLL_SIZE)
}

pub fn self_test() -> Result<usize> {
    let root = PathBuf::from(env!("CARGO_MANIFEST_DIR")).join("../..");
    let index = root.join("assets/text/staff_roll_index.json");
    let bytes = build_inner(&index)?;
    if bytes.len() != STAFF_ROLL_SIZE {
        return Err(err("staff-roll self-test size differs"));
    }
    Ok(bytes.len())
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn canonical_staff_roll_builds() {
        assert_eq!(self_test().unwrap(), STAFF_ROLL_SIZE);
    }
}
