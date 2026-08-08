//! Native implementation of `tools/make/indexed_still.ts`.
//!
//! Pre-rendered backgrounds are 256x120 indexed images. Their ROM records
//! begin with 128 little-endian BGR555 palette entries and continue with a
//! seven-bit delta stream, padded to a 16-bit boundary.

use std::collections::BTreeMap;
use std::fs;
use std::path::{Path, PathBuf};

use export_asset::tile_png;
use generated_files::prune_files;
use import_asset::{indexed_png, Report, Rgb};
use serde_json::Value;

pub const STILL_WIDTH: usize = 256;
pub const STILL_HEIGHT: usize = 120;
pub const STILL_PIXELS: usize = STILL_WIDTH * STILL_HEIGHT;
pub const STILL_PALETTE_ENTRIES: usize = 128;
pub const STILL_PALETTE_BYTES: usize = STILL_PALETTE_ENTRIES * 2;
pub const RESOURCE_TABLE: u32 = 0x0832_0000;
pub const ROM_BASE: u32 = 0x0800_0000;

type Result<T> = std::result::Result<T, Error>;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);

impl std::fmt::Display for Error {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        formatter.write_str(&self.0)
    }
}

impl std::error::Error for Error {}

fn err<T>(message: impl Into<String>) -> Result<T> {
    Err(Error(message.into()))
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct StillIndexEntry {
    pub id: String,
    pub file: String,
    pub scene: String,
    pub location: Option<String>,
}

pub type StillIndex = BTreeMap<String, StillIndexEntry>;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Delta7Decoded {
    pub pixels: Vec<u8>,
    pub bits: usize,
    pub bytes: usize,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct StillDecoded {
    pub palette: Vec<u8>,
    pub pixels: Vec<u8>,
    pub bits: usize,
    pub bytes: usize,
    pub size: usize,
}

struct BitReader<'a> {
    data: &'a [u8],
    cursor: usize,
}

impl BitReader<'_> {
    fn read(&mut self, count: usize) -> Result<u32> {
        if !(1..=16).contains(&count) {
            return err("invalid bit count");
        }
        if self.cursor + count > self.data.len().saturating_mul(8) {
            return err("delta7 stream is truncated");
        }
        let mut value = 0u32;
        for index in 0..count {
            let bit = (self.data[(self.cursor + index) / 8] >> ((self.cursor + index) & 7)) & 1;
            value |= u32::from(bit) << index;
        }
        self.cursor += count;
        Ok(value)
    }
}

fn put(bits: &mut Vec<u8>, value: u32, count: usize) {
    for index in 0..count {
        bits.push(((value >> index) & 1) as u8);
    }
}

pub fn decode_delta7(data: &[u8], count: usize) -> Result<Delta7Decoded> {
    let mut reader = BitReader { data, cursor: 0 };
    let mut pixels = vec![0u8; count];
    let mut previous = 0i32;
    for pixel in &mut pixels {
        if reader.read(1)? == 0 {
            if reader.read(1)? != 0 {
                if reader.read(1)? == 0 {
                    let negative = reader.read(1)? != 0;
                    let magnitude = 11 + reader.read(4)? as i32;
                    previous += if negative { -magnitude } else { magnitude };
                    previous &= 0x7f;
                } else {
                    previous = reader.read(7)? as i32;
                }
            }
        } else if reader.read(1)? == 0 {
            let negative = reader.read(1)? != 0;
            let magnitude = 1 + reader.read(1)? as i32;
            previous += if negative { -magnitude } else { magnitude };
            previous &= 0x7f;
        } else {
            let negative = reader.read(1)? != 0;
            let magnitude = 3 + reader.read(3)? as i32;
            previous += if negative { -magnitude } else { magnitude };
            previous &= 0x7f;
        }
        *pixel = previous as u8;
    }
    let bytes = reader.cursor.div_ceil(16) * 2;
    for bit in reader.cursor..bytes * 8 {
        let value = data.get(bit / 8).copied().unwrap_or(0);
        if (value >> (bit & 7)) & 1 != 0 {
            return err("delta7 padding bits are not zero");
        }
    }
    Ok(Delta7Decoded {
        pixels,
        bits: reader.cursor,
        bytes,
    })
}

pub fn encode_delta7(pixels: &[u8]) -> Result<Vec<u8>> {
    let mut bits = Vec::new();
    let mut previous = 0u8;
    for &pixel in pixels {
        if pixel > 0x7f {
            return err("delta7 pixel exceeds seven bits");
        }
        let delta = pixel.wrapping_sub(previous) & 0x7f;
        if delta == 0 {
            put(&mut bits, 0, 2);
        } else if delta <= 2 {
            put(&mut bits, 1, 1);
            put(&mut bits, 0, 1);
            put(&mut bits, 0, 1);
            put(&mut bits, u32::from(delta - 1), 1);
        } else if delta <= 10 {
            put(&mut bits, 1, 1);
            put(&mut bits, 1, 1);
            put(&mut bits, 0, 1);
            put(&mut bits, u32::from(delta - 3), 3);
        } else if delta <= 26 {
            put(&mut bits, 0, 1);
            put(&mut bits, 1, 1);
            put(&mut bits, 0, 1);
            put(&mut bits, 0, 1);
            put(&mut bits, u32::from(delta - 11), 4);
        } else if delta <= 101 {
            put(&mut bits, 0, 1);
            put(&mut bits, 1, 1);
            put(&mut bits, 1, 1);
            put(&mut bits, u32::from(pixel), 7);
        } else if delta <= 117 {
            put(&mut bits, 0, 1);
            put(&mut bits, 1, 1);
            put(&mut bits, 0, 1);
            put(&mut bits, 1, 1);
            put(&mut bits, u32::from(117 - delta), 4);
        } else if delta <= 125 {
            put(&mut bits, 1, 1);
            put(&mut bits, 1, 1);
            put(&mut bits, 1, 1);
            put(&mut bits, u32::from(125 - delta), 3);
        } else {
            put(&mut bits, 1, 1);
            put(&mut bits, 0, 1);
            put(&mut bits, 1, 1);
            put(&mut bits, u32::from(127 - delta), 1);
        }
        previous = pixel;
    }
    let mut result = vec![0u8; bits.len().div_ceil(16) * 2];
    for (index, bit) in bits.into_iter().enumerate() {
        result[index / 8] |= bit << (index & 7);
    }
    Ok(result)
}

fn palette_rgb(data: &[u8]) -> Result<Vec<Rgb>> {
    if data.len() != STILL_PALETTE_BYTES {
        return err("still palette must contain 128 colors");
    }
    let mut palette = Vec::with_capacity(STILL_PALETTE_ENTRIES);
    for entry in data.chunks_exact(2) {
        let value = u16::from_le_bytes([entry[0], entry[1]]);
        if value & 0x8000 != 0 {
            return err("still palette contains a non-BGR555 high bit");
        }
        palette.push([
            ((value & 31) << 3) as u8,
            (((value >> 5) & 31) << 3) as u8,
            (((value >> 10) & 31) << 3) as u8,
        ]);
    }
    Ok(palette)
}

fn palette_bytes(palette: &[Rgb]) -> Result<Vec<u8>> {
    if palette.len() != STILL_PALETTE_ENTRIES {
        return err("still PNG must contain exactly 128 palette entries");
    }
    let mut result = Vec::with_capacity(STILL_PALETTE_BYTES);
    for &[red, green, blue] in palette {
        if red & 7 != 0 || green & 7 != 0 || blue & 7 != 0 {
            return err("still palette channels must be exact five-bit values");
        }
        let value =
            u16::from(red >> 3) | (u16::from(green >> 3) << 5) | (u16::from(blue >> 3) << 10);
        result.extend_from_slice(&value.to_le_bytes());
    }
    Ok(result)
}

pub fn decode_still(data: &[u8]) -> Result<StillDecoded> {
    if data.len() <= STILL_PALETTE_BYTES {
        return err("still record is truncated");
    }
    let palette = data[..STILL_PALETTE_BYTES].to_vec();
    palette_rgb(&palette)?;
    let decoded = decode_delta7(&data[STILL_PALETTE_BYTES..], STILL_PIXELS)?;
    Ok(StillDecoded {
        palette,
        pixels: decoded.pixels,
        bits: decoded.bits,
        bytes: decoded.bytes,
        size: STILL_PALETTE_BYTES + decoded.bytes,
    })
}

fn raster_tiles(pixels: &[u8]) -> Result<Vec<u8>> {
    if pixels.len() != STILL_PIXELS {
        return err("still raster has the wrong pixel count");
    }
    let mut result = Vec::with_capacity(STILL_PIXELS);
    for top in (0..STILL_HEIGHT).step_by(8) {
        for left in (0..STILL_WIDTH).step_by(8) {
            for y in 0..8 {
                result.extend_from_slice(
                    &pixels[(top + y) * STILL_WIDTH + left..(top + y) * STILL_WIDTH + left + 8],
                );
            }
        }
    }
    Ok(result)
}

pub fn export_still(data: &[u8]) -> Result<(Vec<u8>, Report)> {
    let decoded = decode_still(data)?;
    let raster = raster_tiles(&decoded.pixels)?;
    let palette = palette_rgb(&decoded.palette)?;
    tile_png(&raster, 8.0, (STILL_WIDTH / 8) as f64, Some(&palette)).map_err(|error| Error(error.0))
}

pub fn build_still(image: &[u8]) -> Result<(Vec<u8>, Report)> {
    let decoded = indexed_png(image).map_err(|error| Error(error.0))?;
    if decoded.width as usize != STILL_WIDTH || decoded.height as usize != STILL_HEIGHT {
        return err("still PNG must be 256x120");
    }
    let colors = palette_bytes(&decoded.palette)?;
    let pixels: Vec<u8> = decoded
        .pixels
        .into_iter()
        .map(|pixel| pixel as u8)
        .collect();
    let encoded = encode_delta7(&pixels)?;
    let replay = decode_delta7(&encoded, STILL_PIXELS)?;
    if replay.pixels != pixels {
        return err("delta7 encoder replay differs");
    }
    let mut result = colors;
    result.extend_from_slice(&encoded);
    let mut report = Report::default();
    report.set("width", STILL_WIDTH as f64);
    report.set("height", STILL_HEIGHT as f64);
    report.set("palette_entries", decoded.palette.len() as f64);
    report.set("bits", replay.bits as f64);
    report.set("encoded_bytes", encoded.len() as f64);
    Ok((result, report))
}

pub fn diagnose_still(data: &[u8]) -> Option<Report> {
    let decoded = decode_still(data).ok()?;
    if decoded.size.div_ceil(4) * 4 != data.len() {
        return None;
    }
    let replay = encode_delta7(&decoded.pixels).ok()?;
    if replay != data[STILL_PALETTE_BYTES..decoded.size] {
        return None;
    }
    let mut report = Report::default();
    report.set("width", STILL_WIDTH as f64);
    report.set("height", STILL_HEIGHT as f64);
    report.set("palette_entries", STILL_PALETTE_ENTRIES as f64);
    report.set("bits", decoded.bits as f64);
    report.set("source_bytes", decoded.size as f64);
    report.set("alignment_bytes", (data.len() - decoded.size) as f64);
    Some(report)
}

fn string(value: Option<&Value>) -> Option<&str> {
    value.and_then(Value::as_str)
}

fn valid_file(id: &str, file: &str) -> bool {
    let prefix = format!("resource_{id}_");
    let suffix = ".8bpp.png";
    if !file.starts_with(&prefix) || !file.ends_with(suffix) || file.contains('/') {
        return false;
    }
    let middle = &file[prefix.len()..file.len() - suffix.len()];
    !middle.is_empty()
        && middle
            .bytes()
            .all(|byte| byte.is_ascii_lowercase() || byte.is_ascii_digit() || byte == b'_')
}

pub fn read_still_index(path: &Path) -> Result<StillIndex> {
    let data = fs::read(path).map_err(|error| Error(format!("{}: {error}", path.display())))?;
    let document: Value = serde_json::from_slice(&data)
        .map_err(|error| Error(format!("{}: {error}", path.display())))?;
    if document.get("format").and_then(Value::as_u64) != Some(1)
        || string(document.get("kind")) != Some("golden-sun-pre-rendered-background-index")
    {
        return err("invalid pre-rendered background index");
    }
    let resources = match document.get("resources").and_then(Value::as_array) {
        Some(resources) => resources,
        None => return err("invalid pre-rendered background index"),
    };
    let mut result = BTreeMap::new();
    for (offset, entry) in resources.iter().enumerate() {
        let object = match entry.as_object() {
            Some(object) => object,
            None => {
                return err(format!(
                    "invalid pre-rendered background index entry {offset}"
                ))
            }
        };
        let expected = format!("{:x}", 0x1d + offset);
        let id = string(object.get("id"));
        let file = string(object.get("file"));
        let scene = string(object.get("scene"));
        let location = match object.get("location") {
            Some(Value::Null) => None,
            Some(Value::String(value)) => Some(value.clone()),
            _ => {
                return err(format!(
                    "invalid pre-rendered background index entry {offset}"
                ))
            }
        };
        if id != Some(expected.as_str())
            || file.is_none()
            || !valid_file(id.unwrap_or_default(), file.unwrap_or_default())
            || scene.is_none_or(str::is_empty)
        {
            return err(format!(
                "invalid pre-rendered background index entry {offset}"
            ));
        }
        let entry = StillIndexEntry {
            id: id.unwrap().to_string(),
            file: file.unwrap().to_string(),
            scene: scene.unwrap().to_string(),
            location,
        };
        result.insert(entry.id.clone(), entry);
    }
    if result.len() != 0x3e - 0x1d + 1 {
        return err("pre-rendered background index is incomplete");
    }
    Ok(result)
}

fn absolute(path: &Path) -> PathBuf {
    if path.is_absolute() {
        path.to_path_buf()
    } else {
        std::env::current_dir().unwrap_or_default().join(path)
    }
}

pub fn same_paths(left: &Path, right: &Path) -> bool {
    match (fs::canonicalize(left), fs::canonicalize(right)) {
        (Ok(left), Ok(right)) => left == right,
        _ => absolute(left) == absolute(right),
    }
}

fn resource_span(rom: &[u8], id: usize) -> Result<&[u8]> {
    let table = (RESOURCE_TABLE - ROM_BASE) as usize;
    if table + (id + 1) * 4 + 4 > rom.len() {
        return err("resource ID is outside the pointer table");
    }
    let start = u32::from_le_bytes(rom[table + id * 4..table + id * 4 + 4].try_into().unwrap());
    let end = u32::from_le_bytes(
        rom[table + (id + 1) * 4..table + (id + 1) * 4 + 4]
            .try_into()
            .unwrap(),
    );
    let start = start
        .checked_sub(ROM_BASE)
        .ok_or_else(|| Error("resource span is outside the ROM".into()))? as usize;
    let end = end
        .checked_sub(ROM_BASE)
        .ok_or_else(|| Error("resource span is outside the ROM".into()))? as usize;
    if start >= end || end > rom.len() {
        return err("resource span is outside the ROM");
    }
    Ok(&rom[start..end])
}

pub fn export_series(rom_path: &Path, index_path: &Path, directory: &Path) -> Result<usize> {
    if same_paths(rom_path, directory) {
        return err("refusing to overwrite the input ROM");
    }
    let names = read_still_index(index_path)?;
    let rom =
        fs::read(rom_path).map_err(|error| Error(format!("{}: {error}", rom_path.display())))?;
    let mut bytes = 0usize;
    let mut outputs = Vec::new();
    for id in 0x1d..=0x3e {
        let record = resource_span(&rom, id)?;
        let decoded = decode_still(record)?;
        let (image, _) = export_still(record)?;
        let name = format!("{id:x}");
        let entry = names.get(&name).ok_or_else(|| {
            Error(format!(
                "pre-rendered background index has no resource {name}"
            ))
        })?;
        let output = directory.join(&entry.file);
        if let Some(parent) = output.parent() {
            fs::create_dir_all(parent)
                .map_err(|error| Error(format!("{}: {error}", parent.display())))?;
        }
        fs::write(&output, &image)
            .map_err(|error| Error(format!("{}: {error}", output.display())))?;
        outputs.push(output);
        let rebuilt = build_still(&image)?.0;
        if rebuilt != record[..decoded.size] {
            return err(format!("resource {name} round trip differs"));
        }
        bytes += decoded.size;
    }
    prune_files(directory, "resource_*.8bpp.png", &outputs)
        .map_err(|error| Error(error.to_string()))?;
    Ok(bytes)
}

pub fn self_test() -> Result<()> {
    let deltas = [
        0u8, 1, 2, 3, 10, 11, 26, 27, 64, 101, 102, 117, 118, 125, 126, 127,
    ];
    let mut pixels = vec![0u8; STILL_PIXELS];
    let mut value = 0u8;
    for (index, pixel) in pixels.iter_mut().enumerate() {
        value = value.wrapping_add(deltas[index % deltas.len()]) & 0x7f;
        *pixel = value;
    }
    let encoded = encode_delta7(&pixels)?;
    let decoded = decode_delta7(&encoded, STILL_PIXELS)?;
    if decoded.pixels != pixels || decoded.bytes != encoded.len() {
        return err("delta7 self-test failed");
    }
    let mut palette = vec![0u8; STILL_PALETTE_BYTES];
    for index in 0..STILL_PALETTE_ENTRIES {
        let value = (index as u16 & 31)
            | (((index as u16 >> 2) & 31) << 5)
            | (((index as u16 >> 4) & 31) << 10);
        palette[index * 2..index * 2 + 2].copy_from_slice(&value.to_le_bytes());
    }
    let mut record = palette;
    record.extend_from_slice(&encoded);
    let (image, _) = export_still(&record)?;
    let (rebuilt, _) = build_still(&image)?;
    if rebuilt != record || diagnose_still(&record).is_none() {
        return err("still-image self-test failed");
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn delta7_boundaries_round_trip() {
        let deltas = [
            0u8, 1, 2, 3, 10, 11, 26, 27, 64, 101, 102, 117, 118, 125, 126, 127,
        ];
        let mut pixels = Vec::with_capacity(512);
        let mut value = 0u8;
        for index in 0..512 {
            value = value.wrapping_add(deltas[index % deltas.len()]) & 0x7f;
            pixels.push(value);
        }
        let encoded = encode_delta7(&pixels).unwrap();
        assert_eq!(
            decode_delta7(&encoded, pixels.len()).unwrap().pixels,
            pixels
        );
    }

    #[test]
    fn tracked_index_is_complete() {
        let root = Path::new(env!("CARGO_MANIFEST_DIR"))
            .parent()
            .and_then(Path::parent)
            .unwrap();
        let index = read_still_index(&root.join("assets/graphics/backgrounds_index.json")).unwrap();
        assert_eq!(index.len(), 34);
        assert_eq!(index["1d"].file, "resource_1d_rocky_highlands.8bpp.png");
    }

    #[test]
    fn synthetic_self_test_passes() {
        self_test().unwrap();
    }
}
