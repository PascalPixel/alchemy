pub mod cli;

use import_asset::indexed_png;
use serde_json::{Map, Value};
use std::fs;
use std::path::{Path, PathBuf};

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);

impl std::fmt::Display for Error {
    fn fmt(&self, output: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        output.write_str(&self.0)
    }
}

fn fail<T>(message: impl Into<String>) -> Result<T> {
    Err(Error(message.into()))
}

fn integer(value: Option<&Value>, name: &str) -> Result<usize> {
    let value = value.ok_or_else(|| Error(format!("missing {name}")))?;
    if let Some(number) = value.as_u64() {
        return usize::try_from(number).map_err(|_| Error(format!("{name} is too large")));
    }
    if let Some(number) = value.as_i64() {
        return usize::try_from(number)
            .map_err(|_| Error(format!("{name} must be a non-negative integer")));
    }
    fail(format!("{name} must be an integer"))
}

fn plan_field<'a>(plan: &'a Map<String, Value>, name: &str) -> Result<&'a Value> {
    plan.get(name)
        .ok_or_else(|| Error(format!("missing {name}")))
}

pub fn encode_stream(pixels: &[u8]) -> Result<Vec<u8>> {
    let mut output = Vec::new();
    let mut cursor = 0usize;
    while cursor < pixels.len() {
        let value = pixels[cursor];
        if value != 0 {
            if value > 0xdf {
                return fail("sprite pixel exceeds the literal range");
            }
            output.push(value);
            cursor += 1;
            continue;
        }
        let mut end = cursor + 1;
        while end < pixels.len() && pixels[end] == 0 {
            end += 1;
        }
        let mut remaining = end - cursor;
        while remaining != 0 {
            let count = remaining.min(32);
            output.push(0xdf + count as u8);
            remaining -= count;
        }
        cursor = end;
    }
    output.push(0);
    Ok(output)
}

pub type Rgb = [u8; 3];

pub fn read_palette(path: &Path, offset: usize, entries: usize) -> Result<Vec<Rgb>> {
    let image = indexed_png(&fs::read(path).map_err(|error| Error(error.to_string()))?)
        .map_err(|error| Error(error.0))?;
    if offset + entries > image.pixels.len() {
        return fail("shared palette PNG is too small");
    }
    image.pixels[offset..offset + entries]
        .iter()
        .map(|pixel| {
            image
                .palette
                .get(*pixel as usize)
                .copied()
                .ok_or_else(|| Error("shared palette PNG references a missing color".into()))
        })
        .collect()
}

fn read_image(path: &Path, width: usize, height: usize, palette: &[Rgb]) -> Result<Vec<u8>> {
    let image = indexed_png(&fs::read(path).map_err(|error| Error(error.to_string()))?)
        .map_err(|error| Error(error.0))?;
    if image.width as usize != width || image.height as usize != height {
        return fail(format!("{}: expected {width}x{height}", path.display()));
    }
    if image.palette != palette {
        return fail(format!(
            "{}: palette differs from the proven shared palette",
            path.display()
        ));
    }
    if image.pixels.iter().any(|value| *value > 0xdf) {
        return fail(format!(
            "{}: pixel exceeds the archive literal range",
            path.display()
        ));
    }
    Ok(image.pixels.into_iter().map(|value| value as u8).collect())
}

fn parse_plan(value: &Value) -> Result<(usize, usize, usize, usize, usize, Vec<usize>)> {
    let plan = value
        .as_object()
        .ok_or_else(|| Error("unsupported zero-skip sprite plan".into()))?;
    if plan_field(plan, "format")?.as_u64() != Some(1)
        || plan_field(plan, "codec")?.as_str() != Some("zero-skip-sprite-archive")
    {
        return fail("unsupported zero-skip sprite plan");
    }
    let width = integer(plan.get("width"), "width")?;
    let height = integer(plan.get("height"), "height")?;
    let images = integer(plan.get("images"), "images")?;
    let palette_offset = integer(plan.get("palette_offset"), "palette_offset")?;
    let palette_entries = integer(plan.get("palette_entries"), "palette_entries")?;
    let order = plan_field(plan, "stream_order")?
        .as_array()
        .ok_or_else(|| Error("stream_order must be an array".into()))?
        .iter()
        .map(|value| integer(Some(value), "stream_order entry"))
        .collect::<Result<Vec<_>>>()?;
    let mut sorted = order.clone();
    sorted.sort_unstable();
    if sorted != (0..images).collect::<Vec<_>>() {
        return fail("sprite stream order is not a permutation");
    }
    Ok((
        width,
        height,
        images,
        palette_offset,
        palette_entries,
        order,
    ))
}

pub fn build_archive(plan_value: &Value, directory: &Path, palette_path: &Path) -> Result<Vec<u8>> {
    let (width, height, count, palette_offset, palette_entries, order) = parse_plan(plan_value)?;
    let palette = read_palette(palette_path, palette_offset, palette_entries)?;
    let mut streams = Vec::with_capacity(count);
    for index in 0..count {
        let path = PathBuf::from(format!(
            "{}_images_frame_{index:02}.png",
            directory.display()
        ));
        streams.push(encode_stream(&read_image(&path, width, height, &palette)?)?);
    }
    let header_size = (count + 1) * 4;
    let mut offsets = vec![0usize; count];
    let mut cursor = header_size;
    for index in order.iter().copied() {
        offsets[index] = cursor;
        cursor += streams[index].len();
    }
    let mut header = vec![0u8; header_size];
    for (index, offset) in offsets.iter().copied().enumerate() {
        header[index * 4..index * 4 + 4].copy_from_slice(&(offset as u32).to_le_bytes());
    }
    for index in order {
        header.extend_from_slice(&streams[index]);
    }
    Ok(header)
}
