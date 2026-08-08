//! Native port of `tools/make/f0_archive.ts`.
//!
//! F0 is a 4bpp image archive with a little-endian offset table and a
//! move-to-front bitstream for each image.  This crate is the standalone
//! export/verify CLI and an independently tested implementation of the same
//! byte contract.

use alchemy_zlib::{deflate_sync, DeflateOptions};
use export_asset::chunk;
use generated_files::prune_files;
use import_asset::indexed_png;
use serde_json::{json, Value};
use std::collections::BTreeMap;
use std::fs;
use std::path::{Path, PathBuf};

pub const ROM_BASE: usize = 0x0800_0000;
pub const CODEC: &str = "golden-sun-mtf4-archive";

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);

impl std::fmt::Display for Error {
    fn fmt(&self, out: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        out.write_str(&self.0)
    }
}

impl std::error::Error for Error {}

fn err<T>(message: impl Into<String>) -> Result<T> {
    Err(Error(message.into()))
}

const PREFIX: [&str; 15] = [
    "00",
    "010",
    "011",
    "100",
    "101",
    "110",
    "11100",
    "11101",
    "11110",
    "1111100",
    "1111101",
    "1111110",
    "111111100",
    "111111101",
    "111111110",
];

struct BitReader<'a> {
    data: &'a [u8],
    offset: usize,
}

impl BitReader<'_> {
    fn get(&mut self) -> Result<u8> {
        if self.offset >= self.data.len() * 8 {
            return err("F0 image stream ended before its terminator");
        }
        let value = (self.data[self.offset / 8] >> (self.offset % 8)) & 1;
        self.offset += 1;
        Ok(value)
    }
}

fn selection(bits: &mut BitReader<'_>) -> Result<Option<usize>> {
    if bits.get()? == 0 {
        return Ok(Some(if bits.get()? == 0 {
            1
        } else if bits.get()? == 0 {
            2
        } else {
            3
        }));
    }
    if bits.get()? == 0 {
        return Ok(Some(if bits.get()? == 0 { 4 } else { 5 }));
    }
    if bits.get()? == 0 {
        return Ok(Some(6));
    }
    if bits.get()? == 0 {
        return Ok(Some(if bits.get()? == 0 { 7 } else { 8 }));
    }
    if bits.get()? == 0 {
        return Ok(Some(9));
    }
    if bits.get()? == 0 {
        return Ok(Some(if bits.get()? == 0 { 10 } else { 11 }));
    }
    if bits.get()? == 0 {
        return Ok(Some(12));
    }
    if bits.get()? == 0 {
        return Ok(Some(if bits.get()? == 0 { 13 } else { 14 }));
    }
    if bits.get()? == 0 {
        return Ok(Some(15));
    }
    Ok(None)
}

pub fn decode_pixels(data: &[u8]) -> Result<(Vec<u8>, usize)> {
    let mut bits = BitReader { data, offset: 0 };
    let mut move_to_front: Vec<u8> = (0..16).collect();
    let mut pixels = Vec::new();
    loop {
        while bits.get()? == 0 {
            pixels.push(move_to_front[0]);
        }
        match selection(&mut bits)? {
            None => return Ok((pixels, bits.offset)),
            Some(index) => {
                let value = move_to_front.remove(index);
                move_to_front.insert(0, value);
                pixels.push(value);
            }
        }
    }
}

pub fn encode_pixels(pixels: &[u8]) -> Result<Vec<u8>> {
    let mut move_to_front: Vec<u8> = (0..16).collect();
    let mut bits = Vec::new();
    for value in pixels {
        if *value >= 16 {
            return err("F0 image contains a non-4bpp pixel");
        }
        if *value == move_to_front[0] {
            bits.push(0);
            continue;
        }
        let index = move_to_front
            .iter()
            .position(|item| item == value)
            .expect("4bpp value is in the move-to-front table");
        bits.push(1);
        bits.extend(PREFIX[index - 1].bytes().map(|byte| byte - b'0'));
        let value = move_to_front.remove(index);
        move_to_front.insert(0, value);
    }
    bits.extend(std::iter::repeat_n(1, 10));
    bits.extend(std::iter::repeat_n(1, (8 - bits.len() % 8) % 8));
    let mut output = vec![0u8; bits.len() / 8];
    for (index, bit) in bits.into_iter().enumerate() {
        output[index / 8] |= bit << (index % 8);
    }
    Ok(output)
}

fn png_header(width: usize, height: usize, depth: u8, color_type: u8) -> Vec<u8> {
    let mut header = vec![0u8; 13];
    header[0..4].copy_from_slice(&(width as u32).to_be_bytes());
    header[4..8].copy_from_slice(&(height as u32).to_be_bytes());
    header[8..13].copy_from_slice(&[depth, color_type, 0, 0, 0]);
    header
}

fn png(parts: &[Vec<u8>]) -> Vec<u8> {
    let mut result = b"\x89PNG\r\n\x1a\n".to_vec();
    for part in parts {
        result.extend_from_slice(part);
    }
    result
}

pub fn image_png(pixels: &[u8], palette: &[u8]) -> Result<Vec<u8>> {
    if pixels.len() != 32 * 32 || palette.len() != 32 {
        return err("F0 images must be 32x32 with 16 colors");
    }
    let mut colors = vec![0u8; 48];
    for index in 0..16 {
        let value = u16::from_le_bytes([palette[index * 2], palette[index * 2 + 1]]);
        if value & 0x8000 != 0 {
            return err("F0 palette uses an unsupported high bit");
        }
        colors[index * 3] = ((value & 31) << 3) as u8;
        colors[index * 3 + 1] = (((value >> 5) & 31) << 3) as u8;
        colors[index * 3 + 2] = (((value >> 10) & 31) << 3) as u8;
    }
    let mut rows = Vec::with_capacity(32 * 17);
    for y in 0..32 {
        rows.push(0);
        for x in (0..32).step_by(2) {
            rows.push((pixels[y * 32 + x] << 4) | pixels[y * 32 + x + 1]);
        }
    }
    Ok(png(&[
        chunk(b"IHDR", &png_header(32, 32, 4, 3)),
        chunk(b"PLTE", &colors),
        chunk(
            b"IDAT",
            &deflate_sync(&rows, DeflateOptions { level: Some(9) }),
        ),
        chunk(b"IEND", &[]),
    ]))
}

fn read_image(path: &Path) -> Result<(Vec<u8>, Vec<u8>)> {
    let image =
        indexed_png(&fs::read(path).map_err(|e| Error(e.to_string()))?).map_err(|e| Error(e.0))?;
    if image.width != 32 || image.height != 32 || image.palette.len() != 16 {
        return err("F0 source PNG must be 32x32 with 16 colors");
    }
    let mut palette = vec![0u8; 32];
    for (index, [red, green, blue]) in image.palette.iter().enumerate() {
        if (red & 7) != 0 || (green & 7) != 0 || (blue & 7) != 0 {
            return err("F0 palette channels must be multiples of eight");
        }
        let value =
            u16::from(red >> 3) | (u16::from(green >> 3) << 5) | (u16::from(blue >> 3) << 10);
        palette[index * 2..index * 2 + 2].copy_from_slice(&value.to_le_bytes());
    }
    let pixels = image.pixels.into_iter().map(|pixel| pixel as u8).collect();
    Ok((pixels, palette))
}

fn image_path(directory: &Path, index: usize) -> PathBuf {
    directory.join(format!("image_{index:02}.png"))
}

fn source_image_path(directory: &Path, index: usize) -> PathBuf {
    PathBuf::from(format!(
        "{}_images_image_{index:02}.png",
        directory.display()
    ))
}

fn preview_png(directory: &Path, count: usize, columns: usize, scale: usize) -> Result<Vec<u8>> {
    let rows = count.div_ceil(columns);
    let width = columns * 32 * scale;
    let height = rows * 32 * scale;
    let mut output = vec![255u8; width * height * 3];
    for index in 0..count {
        let (pixels, palette) = read_image(&image_path(directory, index))?;
        let mut colors = [[0u8; 3]; 16];
        for color_index in 0..16 {
            let value =
                u16::from_le_bytes([palette[color_index * 2], palette[color_index * 2 + 1]]);
            colors[color_index] = [
                ((value & 31) << 3 | ((value & 31) >> 2)) as u8,
                (((value >> 5) & 31) << 3 | (((value >> 5) & 31) >> 2)) as u8,
                (((value >> 10) & 31) << 3 | (((value >> 10) & 31) >> 2)) as u8,
            ];
        }
        let left = index % columns * 32 * scale;
        let top = index / columns * 32 * scale;
        for y in 0..32 {
            for x in 0..32 {
                let color = colors[pixels[y * 32 + x] as usize];
                for sy in 0..scale {
                    for sx in 0..scale {
                        let target = ((top + y * scale + sy) * width + left + x * scale + sx) * 3;
                        output[target..target + 3].copy_from_slice(&color);
                    }
                }
            }
        }
    }
    let mut scanlines = Vec::with_capacity(height * (1 + width * 3));
    for y in 0..height {
        scanlines.push(0);
        scanlines.extend_from_slice(&output[y * width * 3..(y + 1) * width * 3]);
    }
    Ok(png(&[
        chunk(b"IHDR", &png_header(width, height, 8, 2)),
        chunk(
            b"IDAT",
            &deflate_sync(&scanlines, DeflateOptions { level: Some(9) }),
        ),
        chunk(b"IEND", &[]),
    ]))
}

fn package_image(path: &Path) -> Result<Vec<u8>> {
    let (pixels, palette) = read_image(path)?;
    let encoded = encode_pixels(&pixels)?;
    let mut data = palette;
    data.extend_from_slice(&encoded);
    data.resize((data.len() + 3) & !3, 0);
    Ok(data)
}

fn package_source(directory: &Path, index: usize) -> PathBuf {
    let conventional = source_image_path(directory, index);
    if conventional.exists() {
        conventional
    } else {
        image_path(directory, index)
    }
}

#[derive(Debug, Clone)]
struct Plan {
    images: usize,
    entries: Vec<Option<usize>>,
}

fn parse_plan(value: &Value) -> Result<Plan> {
    if value.get("format").and_then(Value::as_u64) != Some(1)
        || value.get("codec").and_then(Value::as_str) != Some(CODEC)
    {
        return err("unsupported F0 archive plan");
    }
    let images = value
        .get("images")
        .and_then(Value::as_u64)
        .and_then(|n| usize::try_from(n).ok())
        .ok_or_else(|| Error("F0 archive plan images must be an integer".into()))?;
    let entries = value
        .get("entries")
        .and_then(Value::as_array)
        .ok_or_else(|| Error("F0 archive plan entries must be an array".into()))?
        .iter()
        .map(|entry| {
            entry
                .as_u64()
                .map(|n| {
                    usize::try_from(n).map_err(|_| Error("F0 image index is too large".into()))
                })
                .transpose()
        })
        .collect::<Result<Vec<_>>>()?;
    Ok(Plan { images, entries })
}

pub fn build_archive(plan_value: &Value, directory: &Path) -> Result<Vec<u8>> {
    let plan = parse_plan(plan_value)?;
    let mut packages = Vec::with_capacity(plan.images);
    for index in 0..plan.images {
        packages.push(package_image(&package_source(directory, index))?);
    }
    let mut offset = plan.entries.len() * 2;
    let mut offsets = Vec::with_capacity(packages.len());
    for package in &packages {
        offsets.push(offset);
        offset += package.len();
    }
    if offsets.iter().any(|value| *value >= 0x10000) {
        return err("F0 archive offset exceeds 16 bits");
    }
    let mut result = vec![0u8; plan.entries.len() * 2];
    for (index, entry) in plan.entries.iter().enumerate() {
        let value = match entry {
            None => 0,
            Some(image) => *offsets
                .get(*image)
                .ok_or_else(|| Error("F0 archive entry references a missing image".into()))?,
        };
        result[index * 2..index * 2 + 2].copy_from_slice(&(value as u16).to_le_bytes());
    }
    for package in packages {
        result.extend_from_slice(&package);
    }
    Ok(result)
}

fn archive_slice(rom: &[u8], address: usize, input_end: usize) -> Result<&[u8]> {
    if address < ROM_BASE || input_end < address {
        return err("archive address lies below ROM base");
    }
    let start = address - ROM_BASE;
    let end = input_end
        .checked_sub(ROM_BASE)
        .ok_or_else(|| Error("archive address lies below ROM base".into()))?;
    rom.get(start..end)
        .ok_or_else(|| Error("archive range lies outside the ROM".into()))
}

pub fn extract_archive(
    rom: &[u8],
    address: usize,
    input_end: usize,
    plan_path: &Path,
    images: &Path,
    preview: Option<&Path>,
) -> Result<(usize, usize, usize)> {
    let archive = archive_slice(rom, address, input_end)?;
    if archive.len() < 2 {
        return err("F0 archive has no valid offset table");
    }
    let first = u16::from_le_bytes([archive[0], archive[1]]) as usize;
    if first == 0 || first % 2 != 0 || first > archive.len() {
        return err("F0 archive has no valid offset table");
    }
    let entries: Vec<usize> = archive[..first]
        .chunks_exact(2)
        .map(|bytes| u16::from_le_bytes([bytes[0], bytes[1]]) as usize)
        .collect();
    let mut unique: Vec<usize> = entries
        .iter()
        .copied()
        .filter(|value| *value != 0)
        .collect();
    unique.sort_unstable();
    unique.dedup();
    if unique.first().copied() != Some(first) {
        return err("F0 first image does not follow its offset table");
    }
    fs::create_dir_all(images).map_err(|e| Error(e.to_string()))?;
    let index_by_offset: BTreeMap<usize, usize> = unique
        .iter()
        .copied()
        .enumerate()
        .map(|(index, offset)| (offset, index))
        .collect();
    for (index, offset) in unique.iter().copied().enumerate() {
        let bound = unique.get(index + 1).copied().unwrap_or(archive.len());
        if offset + 32 > bound || bound > archive.len() {
            return err("F0 image package lies outside the archive");
        }
        let palette = &archive[offset..offset + 32];
        let (pixels, _) = decode_pixels(&archive[offset + 32..bound])?;
        if pixels.len() != 32 * 32 {
            return err("F0 image does not decode to 32x32");
        }
        let encoded = encode_pixels(&pixels)?;
        let mut rebuilt = palette.to_vec();
        rebuilt.extend_from_slice(&encoded);
        rebuilt.resize((rebuilt.len() + 3) & !3, 0);
        if rebuilt != archive[offset..bound] {
            return err("F0 image package does not re-encode exactly");
        }
        fs::write(image_path(images, index), image_png(&pixels, palette)?)
            .map_err(|e| Error(e.to_string()))?;
    }
    let plan = json!({
        "format": 1,
        "codec": CODEC,
        "images": unique.len(),
        "entries": entries.iter().map(|offset| {
            match offset {
                0 => Value::Null,
                value => Value::from(*index_by_offset.get(value).unwrap()),
            }
        }).collect::<Vec<_>>(),
    });
    if build_archive(&plan, images)? != archive {
        return err("rebuilt F0 archive differs");
    }
    let keep: Vec<String> = (0..unique.len())
        .map(|index| format!("image_{index:02}.png"))
        .collect();
    prune_files(images, "image_*.png", keep).map_err(|e| Error(e.to_string()))?;
    if let Some(parent) = plan_path.parent() {
        fs::create_dir_all(parent).map_err(|e| Error(e.to_string()))?;
    }
    fs::write(
        plan_path,
        format!("{}\n", serde_json::to_string(&plan).unwrap()),
    )
    .map_err(|e| Error(e.to_string()))?;
    if let Some(preview) = preview {
        if let Some(parent) = preview.parent() {
            fs::create_dir_all(parent).map_err(|e| Error(e.to_string()))?;
        }
        fs::write(preview, preview_png(images, unique.len(), 8, 2)?)
            .map_err(|e| Error(e.to_string()))?;
    }
    Ok((entries.len(), unique.len(), archive.len()))
}

pub fn verify_archive(
    rom: &[u8],
    address: usize,
    input_end: usize,
    plan_path: &Path,
    images: &Path,
) -> Result<usize> {
    let archive = archive_slice(rom, address, input_end)?;
    let plan: Value =
        serde_json::from_slice(&fs::read(plan_path).map_err(|e| Error(e.to_string()))?)
            .map_err(|e| Error(e.to_string()))?;
    let built = build_archive(&plan, images)?;
    if built != archive {
        return err("F0 archive differs from ROM");
    }
    Ok(built.len())
}

pub fn self_test() -> Result<()> {
    let pixels: Vec<u8> = (0..1024).map(|index| ((index * 7) & 15) as u8).collect();
    let encoded = encode_pixels(&pixels)?;
    if decode_pixels(&encoded)?.0 != pixels {
        return err("F0 pixel self-test failed");
    }
    let palette: Vec<u8> = (0..16u16)
        .flat_map(|index| (index * 0x421).to_le_bytes())
        .collect();
    let image = image_png(&pixels, &palette)?;
    let decoded = indexed_png(&image).map_err(|e| Error(e.0))?;
    if decoded.width != 32
        || decoded.height != 32
        || decoded
            .pixels
            .iter()
            .map(|value| *value as u8)
            .collect::<Vec<_>>()
            != pixels
    {
        return err("F0 PNG self-test failed");
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn mtf_round_trip() {
        self_test().unwrap();
    }

    #[test]
    fn plan_and_png_shape_are_stable() {
        let plan = json!({"format": 1, "codec": CODEC, "images": 0, "entries": []});
        assert_eq!(parse_plan(&plan).unwrap().entries.len(), 0);
        assert_eq!(
            &image_png(&vec![0; 1024], &vec![0; 32]).unwrap()[..8],
            b"\x89PNG\r\n\x1a\n"
        );
    }
}
