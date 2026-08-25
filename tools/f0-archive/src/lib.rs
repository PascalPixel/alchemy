pub mod cli;

use import_asset::indexed_png;
use serde_json::Value;
use std::fs;
use std::path::{Path, PathBuf};

pub const CODEC: &str = "golden-sun-mtf4-archive";

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);

impl std::fmt::Display for Error {
    fn fmt(&self, out: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        out.write_str(&self.0)
    }
}

fn err<T>(message: impl Into<String>) -> Result<T> {
    Err(Error(message.into()))
}

const PREFIX: [&str; 15] = ["00", "010", "011", "100", "101", "110", "11100", "11101", "11110", "1111100", "1111101", "1111110", "111111100", "111111101", "111111110"];

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
        let index = move_to_front.iter().position(|item| item == value).expect("4bpp value is in the move-to-front table");
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

fn read_image(path: &Path) -> Result<(Vec<u8>, Vec<u8>)> {
    let image = indexed_png(&fs::read(path).map_err(|e| Error(e.to_string()))?).map_err(|e| Error(e.0))?;
    if image.width != 32 || image.height != 32 || image.palette.len() != 16 {
        return err("F0 source PNG must be 32x32 with 16 colors");
    }
    let mut palette = vec![0u8; 32];
    for (index, [red, green, blue]) in image.palette.iter().enumerate() {
        if (red & 7) != 0 || (green & 7) != 0 || (blue & 7) != 0 {
            return err("F0 palette channels must be multiples of eight");
        }
        let value = u16::from(red >> 3) | (u16::from(green >> 3) << 5) | (u16::from(blue >> 3) << 10);
        palette[index * 2..index * 2 + 2].copy_from_slice(&value.to_le_bytes());
    }
    let pixels = image.pixels.into_iter().map(|pixel| pixel as u8).collect();
    Ok((pixels, palette))
}

fn image_path(directory: &Path, index: usize) -> PathBuf {
    directory.join(format!("image_{index:02}.png"))
}

fn source_image_path(directory: &Path, index: usize) -> PathBuf {
    PathBuf::from(format!("{}_images_image_{index:02}.png", directory.display()))
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
    if value.get("format").and_then(Value::as_u64) != Some(1) || value.get("codec").and_then(Value::as_str) != Some(CODEC) {
        return err("unsupported F0 archive plan");
    }
    let images = value.get("images").and_then(Value::as_u64).and_then(|n| usize::try_from(n).ok()).ok_or_else(|| Error("F0 archive plan images must be an integer".into()))?;
    let entries =
        value.get("entries").and_then(Value::as_array).ok_or_else(|| Error("F0 archive plan entries must be an array".into()))?.iter().map(|entry| entry.as_u64().map(|n| usize::try_from(n).map_err(|_| Error("F0 image index is too large".into()))).transpose()).collect::<Result<Vec<_>>>()?;
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
            Some(image) => *offsets.get(*image).ok_or_else(|| Error("F0 archive entry references a missing image".into()))?,
        };
        result[index * 2..index * 2 + 2].copy_from_slice(&(value as u16).to_le_bytes());
    }
    for package in packages {
        result.extend_from_slice(&package);
    }
    Ok(result)
}
