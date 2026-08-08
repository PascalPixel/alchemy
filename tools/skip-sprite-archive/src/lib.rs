//! Native port of `tools/make/skip_sprite_archive.ts`.
//!
//! The archive is intentionally simple: a little-endian pointer table followed
//! by zero-skip encoded indexed frames.  Plans and PNGs remain the interchange
//! format, while this crate owns the standalone codec and its parity checks.

use alchemy_zlib::{deflate_sync, DeflateOptions};
use export_asset::chunk;
use generated_files::prune_files;
use import_asset::indexed_png;
use serde_json::{json, Map, Value};
use std::collections::BTreeSet;
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

impl std::error::Error for Error {}

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

/// Decode one frame, returning the decoded pixels and the first byte after it.
pub fn decode_stream(data: &[u8], offset: usize, pixels: usize) -> Result<(Vec<u8>, usize)> {
    let mut output = vec![0u8; pixels];
    let mut cursor = offset;
    let mut position = 0usize;
    while cursor < data.len() {
        let value = data[cursor];
        cursor += 1;
        if value == 0 {
            if position != pixels {
                return fail("sprite stream ended before its image boundary");
            }
            return Ok((output, cursor));
        }
        if value <= 0xdf {
            if position >= pixels {
                return fail("sprite literal crossed its image boundary");
            }
            output[position] = value;
            position += 1;
        } else {
            position += value as usize - 0xdf;
            if position > pixels {
                return fail("sprite skip crossed its image boundary");
            }
        }
    }
    fail("sprite stream has no terminator")
}

/// Encode one indexed frame using the archive's zero-run representation.
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

fn png_header(width: usize, height: usize, depth: u8, color_type: u8) -> Vec<u8> {
    let mut header = vec![0u8; 13];
    header[0..4].copy_from_slice(&(width as u32).to_be_bytes());
    header[4..8].copy_from_slice(&(height as u32).to_be_bytes());
    header[8..13].copy_from_slice(&[depth, color_type, 0, 0, 0]);
    header
}

pub type Rgb = [u8; 3];

/// Construct the exact indexed PNG emitted by the TypeScript helper.
pub fn png(pixels: &[u8], width: usize, height: usize, palette: &[Rgb]) -> Result<Vec<u8>> {
    if pixels.len() != width * height || palette.is_empty() || palette.len() > 256 {
        return fail("invalid indexed sprite image");
    }
    if pixels.iter().any(|pixel| *pixel as usize >= palette.len()) {
        return fail("indexed sprite pixel references a missing palette entry");
    }
    let mut colors = Vec::with_capacity(palette.len() * 3);
    for color in palette {
        colors.extend_from_slice(color);
    }
    let mut rows = Vec::with_capacity(height * (width + 1));
    for row in pixels.chunks_exact(width) {
        rows.push(0);
        rows.extend_from_slice(row);
    }
    let transparency = {
        let mut value = vec![0u8];
        value.resize(palette.len(), 0xff);
        value
    };
    let mut output = b"\x89PNG\r\n\x1a\n".to_vec();
    output.extend_from_slice(&chunk(b"IHDR", &png_header(width, height, 8, 3)));
    output.extend_from_slice(&chunk(b"PLTE", &colors));
    output.extend_from_slice(&chunk(b"tRNS", &transparency));
    output.extend_from_slice(&chunk(
        b"IDAT",
        &deflate_sync(&rows, DeflateOptions { level: Some(9) }),
    ));
    output.extend_from_slice(&chunk(b"IEND", &[]));
    Ok(output)
}

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

fn preview(images: &[Vec<u8>], width: usize, height: usize, palette: &[Rgb]) -> Result<Vec<u8>> {
    let columns = 4usize;
    let scale = 2usize;
    let rows = images.len().div_ceil(columns);
    let out_width = columns * width * scale;
    let out_height = rows * height * scale;
    let mut output = vec![32u8; out_width * out_height * 3];
    for (index, pixels) in images.iter().enumerate() {
        let left = index % columns * width * scale;
        let top = index / columns * height * scale;
        for y in 0..height {
            for x in 0..width {
                let value = pixels[y * width + x] as usize;
                let color = if value != 0 {
                    *palette.get(value).ok_or_else(|| {
                        Error("sprite pixel references a missing palette entry".into())
                    })?
                } else {
                    [32, 32, 32]
                };
                for sy in 0..scale {
                    for sx in 0..scale {
                        let target =
                            ((top + y * scale + sy) * out_width + left + x * scale + sx) * 3;
                        output[target..target + 3].copy_from_slice(&color);
                    }
                }
            }
        }
    }
    let mut rows_data = Vec::with_capacity(out_height * (out_width * 3 + 1));
    for row in output.chunks_exact(out_width * 3) {
        rows_data.push(0);
        rows_data.extend_from_slice(row);
    }
    let mut result = b"\x89PNG\r\n\x1a\n".to_vec();
    result.extend_from_slice(&chunk(b"IHDR", &png_header(out_width, out_height, 8, 2)));
    result.extend_from_slice(&chunk(
        b"IDAT",
        &deflate_sync(&rows_data, DeflateOptions { level: Some(9) }),
    ));
    result.extend_from_slice(&chunk(b"IEND", &[]));
    Ok(result)
}

pub fn export_archive(
    decoded: &[u8],
    directory: &Path,
    plan_path: &Path,
    palette_path: &Path,
    preview_path: Option<&Path>,
    width: usize,
    height: usize,
) -> Result<usize> {
    let mut entries = Vec::new();
    let limit = decoded.len().min(256 * 4);
    let mut offset = 0usize;
    while offset < limit {
        if offset + 4 > decoded.len() {
            return fail("archive pointer table lacks its null terminator");
        }
        let value = u32::from_le_bytes(decoded[offset..offset + 4].try_into().unwrap()) as usize;
        entries.push(value);
        offset += 4;
        if value == 0 {
            break;
        }
    }
    if entries.last().copied() != Some(0) {
        return fail("archive pointer table lacks its null terminator");
    }
    let offsets = &entries[..entries.len() - 1];
    let first = entries.len() * 4;
    if offsets.is_empty() || offsets.iter().copied().min() != Some(first) {
        return fail("archive streams do not follow its pointer table");
    }
    let mut seen = BTreeSet::new();
    if offsets
        .iter()
        .any(|value| !seen.insert(*value) || *value < first || *value >= decoded.len())
    {
        return fail("archive contains an invalid sprite pointer");
    }
    let mut images = Vec::with_capacity(offsets.len());
    let mut ends = Vec::with_capacity(offsets.len());
    for offset in offsets {
        let (image, end) = decode_stream(decoded, *offset, width * height)?;
        images.push(image);
        ends.push((*offset, end));
    }
    let mut order: Vec<usize> = (0..offsets.len()).collect();
    order.sort_by_key(|index| offsets[*index]);
    for (position, index) in order.iter().copied().enumerate() {
        let expected = order
            .get(position + 1)
            .map(|next| offsets[*next])
            .unwrap_or(decoded.len());
        let end = ends[index].1;
        if end != expected {
            return fail("sprite streams are not contiguous");
        }
    }
    let palette = read_palette(palette_path, 16, 224)?;
    fs::create_dir_all(directory).map_err(|error| Error(error.to_string()))?;
    for (index, image) in images.iter().enumerate() {
        let path = PathBuf::from(format!(
            "{}_images_frame_{index:02}.png",
            directory.display()
        ));
        fs::write(path, png(image, width, height, &palette)?)
            .map_err(|error| Error(error.to_string()))?;
    }
    let plan = json!({
        "format": 1,
        "codec": "zero-skip-sprite-archive",
        "width": width,
        "height": height,
        "images": images.len(),
        "palette_offset": 16,
        "palette_entries": 224,
        "stream_order": order,
    });
    if let Some(parent) = plan_path.parent() {
        fs::create_dir_all(parent).map_err(|error| Error(error.to_string()))?;
    }
    fs::write(
        plan_path,
        format!("{}\n", serde_json::to_string(&plan).unwrap()),
    )
    .map_err(|error| Error(error.to_string()))?;
    if build_archive(&plan, directory, palette_path)? != decoded {
        return fail("exported sprite archive does not round-trip");
    }
    let keep = (0..images.len())
        .map(|index| format!("images_frame_{index:02}.png"))
        .collect::<Vec<_>>();
    prune_files(directory, "images_frame_*.png", keep).map_err(|error| Error(error.to_string()))?;
    if let Some(path) = preview_path {
        if let Some(parent) = path.parent() {
            fs::create_dir_all(parent).map_err(|error| Error(error.to_string()))?;
        }
        fs::write(path, preview(&images, width, height, &palette)?)
            .map_err(|error| Error(error.to_string()))?;
    }
    Ok(images.len())
}

pub fn self_test() -> Result<()> {
    let mut unit = vec![0u8; 65];
    unit.extend([1, 2, 3]);
    unit.extend(std::iter::repeat_n(0, 31));
    let mut source = Vec::new();
    for _ in 0..8 {
        source.extend_from_slice(&unit);
    }
    let encoded = encode_stream(&source)?;
    let (decoded, end) = decode_stream(&encoded, 0, source.len())?;
    if decoded != source || end != encoded.len() {
        return fail("zero-skip stream round-trip failed");
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn zero_skip_round_trip() {
        self_test().unwrap();
    }

    #[test]
    fn long_zero_runs_are_split_at_thirty_two() {
        let encoded = encode_stream(&[0; 65]).unwrap();
        assert_eq!(encoded, vec![0xff, 0xff, 0xe0, 0]);
        assert_eq!(decode_stream(&encoded, 0, 65).unwrap().0, vec![0; 65]);
    }

    #[test]
    fn indexed_png_has_the_expected_alpha_palette() {
        let image = png(&[0, 1, 0, 1], 2, 2, &[[1, 2, 3], [4, 5, 6]]).unwrap();
        assert!(image.windows(4).any(|window| window == b"tRNS"));
        assert_eq!(&image[0..8], b"\x89PNG\r\n\x1a\n");
    }
}
