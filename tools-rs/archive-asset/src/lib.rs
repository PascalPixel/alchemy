//! Port of `tools/make/archive_asset.ts`.
//!
//! An archive is an offset table followed by palette-LZ streams.  This tool
//! extracts those streams to a plan and a PNG atlas, then replays both before
//! it writes anything.  The PNG and codec implementations are shared with the
//! other native asset tools, so the round trip remains byte exact.

use std::fmt;

use alignment_tail::{
    build_alignment_tail, inspect_alignment_tail, parse_alignment_tail, AlignmentTail,
};
use export_asset::{byte_png, rgba_image};
use extract_resource::{decode_palette_trace, encode_palette, PaletteGroup, PaletteOperation};
use import_asset::{indexed_png, rgba_png, subarray};
use serde_json::{json, Map, Value};

pub const ROM_BASE: u32 = 0x0800_0000;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum PixelFormat {
    Rgba,
    Indexed8,
}

impl PixelFormat {
    pub fn as_str(self) -> &'static str {
        match self {
            Self::Rgba => "rgba",
            Self::Indexed8 => "indexed8",
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct ArchiveStream {
    pub decoded_size: usize,
    pub encoded_size: usize,
    pub tokens: Vec<PaletteGroup>,
    pub lookahead: Vec<u8>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct ArchivePlan {
    pub format: usize,
    pub codec: String,
    pub chunk_width: usize,
    pub chunk_height: usize,
    pub columns: usize,
    pub pixel_format: PixelFormat,
    pub offset_width: usize,
    pub stream_alignment: usize,
    pub streams: Vec<ArchiveStream>,
    pub alignment_tail: Option<AlignmentTail>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct ArchiveError(pub String);

impl fmt::Display for ArchiveError {
    fn fmt(&self, out: &mut fmt::Formatter<'_>) -> fmt::Result {
        out.write_str(&self.0)
    }
}

impl std::error::Error for ArchiveError {}

fn error(message: impl Into<String>) -> ArchiveError {
    ArchiveError(message.into())
}

fn checked_product(values: &[usize], message: &str) -> Result<usize, ArchiveError> {
    values
        .iter()
        .try_fold(1usize, |value, next| value.checked_mul(*next))
        .ok_or_else(|| error(message))
}

pub fn pixel_size(pixel_format: PixelFormat) -> usize {
    match pixel_format {
        PixelFormat::Rgba => 4,
        PixelFormat::Indexed8 => 1,
    }
}

fn png_error(message: impl fmt::Display) -> ArchiveError {
    error(message.to_string())
}

/// Lay the decoded chunks into a row-major atlas and encode it as PNG.
pub fn make_atlas(
    chunks: &[Vec<u8>],
    chunk_width: usize,
    chunk_height: usize,
    columns: usize,
    pixel_format: PixelFormat,
) -> Result<Vec<u8>, ArchiveError> {
    let depth = pixel_size(pixel_format);
    let unit = checked_product(
        &[chunk_width, chunk_height, depth],
        "archive chunks have inconsistent RGBA dimensions",
    )?;
    if chunks.is_empty() || chunks.iter().any(|chunk| chunk.len() != unit) || columns == 0 {
        return Err(error("archive chunks have inconsistent RGBA dimensions"));
    }
    let rows = chunks.len().div_ceil(columns);
    let width = columns
        .checked_mul(chunk_width)
        .ok_or_else(|| error("archive chunks have inconsistent RGBA dimensions"))?;
    let atlas_len = checked_product(
        &[width, rows, chunk_height, depth],
        "archive chunks have inconsistent RGBA dimensions",
    )?;
    let mut atlas = vec![0u8; atlas_len];
    for (index, chunk) in chunks.iter().enumerate() {
        let left = index % columns * chunk_width;
        let top = index / columns * chunk_height;
        for y in 0..chunk_height {
            let source = y * chunk_width * depth;
            let target = ((top + y) * width + left) * depth;
            atlas[target..target + chunk_width * depth]
                .copy_from_slice(&chunk[source..source + chunk_width * depth]);
        }
    }
    match pixel_format {
        PixelFormat::Rgba => rgba_image(&atlas, width as f64)
            .map(|(image, _)| image)
            .map_err(png_error),
        PixelFormat::Indexed8 => byte_png(&atlas, width as f64)
            .map(|(image, _)| image)
            .map_err(png_error),
    }
}

/// Split a PNG atlas back into its original chunks.
pub fn read_atlas(
    data: &[u8],
    count: usize,
    chunk_width: usize,
    chunk_height: usize,
    columns: usize,
    pixel_format: PixelFormat,
) -> Result<Vec<Vec<u8>>, ArchiveError> {
    if columns == 0 {
        return Err(error("archive atlas dimensions differ from its plan"));
    }
    let depth = pixel_size(pixel_format);
    let (width, height, pixels) = match pixel_format {
        PixelFormat::Rgba => {
            let image = rgba_png(data).map_err(png_error)?;
            (image.width as usize, image.height as usize, image.pixels)
        }
        PixelFormat::Indexed8 => {
            let image = indexed_png(data).map_err(png_error)?;
            let pixels = image
                .pixels
                .into_iter()
                .map(|pixel| pixel as u8)
                .collect::<Vec<_>>();
            (image.width as usize, image.height as usize, pixels)
        }
    };
    let rows = count.div_ceil(columns);
    if width != columns * chunk_width || height != rows * chunk_height {
        return Err(error("archive atlas dimensions differ from its plan"));
    }
    let mut chunks = Vec::with_capacity(count);
    for index in 0..count {
        let left = index % columns * chunk_width;
        let top = index / columns * chunk_height;
        let mut chunk = Vec::with_capacity(chunk_width * chunk_height * depth);
        for y in 0..chunk_height {
            let start = ((top + y) * width + left) * depth;
            chunk.extend_from_slice(&pixels[start..start + chunk_width * depth]);
        }
        chunks.push(chunk);
    }
    Ok(chunks)
}

/// Rebuild an archive from an atlas and its extracted plan.
pub fn build_archive(atlas: &[u8], plan: &ArchivePlan) -> Result<Vec<u8>, ArchiveError> {
    let chunks = read_atlas(
        atlas,
        plan.streams.len(),
        plan.chunk_width,
        plan.chunk_height,
        plan.columns,
        plan.pixel_format,
    )?;
    let mut encoded = Vec::with_capacity(chunks.len());
    for (chunk, stream) in chunks.iter().zip(&plan.streams) {
        if chunk.len() != stream.decoded_size {
            return Err(error("archive chunk size differs from its plan"));
        }
        let mut payload = encode_palette(chunk, &stream.tokens).map_err(png_error)?;
        payload.extend_from_slice(&stream.lookahead);
        if payload.len() != stream.encoded_size {
            return Err(error("archive stream size differs from its plan"));
        }
        encoded.push(payload);
    }

    let offset_width = plan.offset_width;
    let alignment = plan.stream_alignment;
    if !matches!(offset_width, 2 | 4) {
        return Err(error("offset width must be 2 or 4"));
    }
    if alignment == 0 {
        return Err(error("stream alignment must be positive"));
    }
    let mut offset = encoded
        .len()
        .checked_mul(offset_width)
        .ok_or_else(|| error("archive offsets do not fit their configured width"))?;
    let mut offsets = Vec::with_capacity(encoded.len());
    let mut slots = Vec::with_capacity(encoded.len());
    for payload in encoded {
        offsets.push(offset);
        let padding = (alignment - payload.len() % alignment) % alignment;
        let mut slot = payload;
        slot.resize(slot.len() + padding, 0);
        offset = offset
            .checked_add(slot.len())
            .ok_or_else(|| error("archive offsets do not fit their configured width"))?;
        slots.push(slot);
    }
    let limit = 1usize << (offset_width * 8);
    if offsets.iter().any(|value| *value >= limit) {
        return Err(error("archive offsets do not fit their configured width"));
    }
    let mut archive =
        Vec::with_capacity(offset + plan.alignment_tail.as_ref().map_or(0, AlignmentTail::size));
    for value in offsets {
        match offset_width {
            2 => archive.extend_from_slice(&(value as u16).to_le_bytes()),
            4 => archive.extend_from_slice(&(value as u32).to_le_bytes()),
            _ => unreachable!(),
        }
    }
    for slot in slots {
        archive.extend_from_slice(&slot);
    }
    if let Some(tail) = &plan.alignment_tail {
        archive.extend_from_slice(&build_alignment_tail(tail));
    }
    Ok(archive)
}

fn palette_operation_value(operation: PaletteOperation) -> Value {
    match operation {
        PaletteOperation::Literal => json!(["l"]),
        PaletteOperation::End => json!(["e"]),
        PaletteOperation::Copy { length, distance } => json!(["c", length, distance]),
    }
}

fn palette_group_value(group: &PaletteGroup) -> Value {
    match group {
        PaletteGroup::Zeros => json!(["z"]),
        PaletteGroup::Group(operations) => Value::Array(vec![
            Value::String("g".to_string()),
            Value::Array(
                operations
                    .iter()
                    .copied()
                    .map(palette_operation_value)
                    .collect(),
            ),
        ]),
    }
}

fn hex(bytes: &[u8]) -> String {
    let mut result = String::with_capacity(bytes.len() * 2);
    for byte in bytes {
        result.push(char::from_digit((byte >> 4) as u32, 16).unwrap());
        result.push(char::from_digit((byte & 15) as u32, 16).unwrap());
    }
    result
}

fn plan_value(plan: &ArchivePlan) -> Value {
    let mut value = Map::new();
    value.insert("format".to_string(), json!(plan.format));
    value.insert("codec".to_string(), json!(plan.codec));
    value.insert("chunk_width".to_string(), json!(plan.chunk_width));
    value.insert("chunk_height".to_string(), json!(plan.chunk_height));
    value.insert("columns".to_string(), json!(plan.columns));
    value.insert(
        "pixel_format".to_string(),
        json!(plan.pixel_format.as_str()),
    );
    value.insert("offset_width".to_string(), json!(plan.offset_width));
    value.insert("stream_alignment".to_string(), json!(plan.stream_alignment));
    value.insert(
        "streams".to_string(),
        Value::Array(
            plan.streams
                .iter()
                .map(|stream| {
                    let mut item = Map::new();
                    item.insert("decoded_size".to_string(), json!(stream.decoded_size));
                    item.insert("encoded_size".to_string(), json!(stream.encoded_size));
                    item.insert(
                        "tokens".to_string(),
                        Value::Array(stream.tokens.iter().map(palette_group_value).collect()),
                    );
                    item.insert("lookahead".to_string(), json!(hex(&stream.lookahead)));
                    Value::Object(item)
                })
                .collect(),
        ),
    );
    if let Some(tail) = &plan.alignment_tail {
        value.insert(
            "alignment_tail".to_string(),
            serde_json::to_value(tail).expect("alignment tail is serializable"),
        );
    }
    Value::Object(value)
}

pub fn extract_archive(
    rom: &[u8],
    address: usize,
    input_end: usize,
    source_end: usize,
    chunk_width: usize,
    chunk_height: usize,
    columns: usize,
    pixel_format: PixelFormat,
    offset_width: usize,
    stream_alignment: usize,
) -> Result<(ArchivePlan, Vec<u8>, usize), ArchiveError> {
    if source_end < address || source_end > input_end {
        return Err(error("archive source end lies outside its boundary"));
    }
    let archive = subarray(rom, address, source_end);
    let tail = subarray(rom, source_end, input_end);
    if tail.len() > 3 {
        return Err(error("archive alignment tail exceeds its bounded extent"));
    }
    if archive.len() < 4 {
        return Err(error("archive is empty"));
    }
    if stream_alignment == 0 {
        return Err(error("stream alignment must be positive"));
    }
    if !matches!(offset_width, 2 | 4) {
        return Err(error("offset width must be 2 or 4"));
    }
    let read_offset = |data: &[u8], index: usize| -> Result<usize, ArchiveError> {
        let start = index
            .checked_mul(offset_width)
            .ok_or_else(|| error("archive has no valid offset table"))?;
        let bytes = data
            .get(start..start + offset_width)
            .ok_or_else(|| error("archive has no valid offset table"))?;
        Ok(match offset_width {
            2 => u16::from_le_bytes([bytes[0], bytes[1]]) as usize,
            4 => u32::from_le_bytes([bytes[0], bytes[1], bytes[2], bytes[3]]) as usize,
            _ => unreachable!(),
        })
    };
    let first = read_offset(archive, 0)?;
    if first == 0 || first % offset_width != 0 || first > archive.len() {
        return Err(error("archive has no valid offset table"));
    }
    let count = first / offset_width;
    let offsets = (0..count)
        .map(|index| read_offset(archive, index))
        .collect::<Result<Vec<_>, _>>()?;
    if offsets[0] != first || offsets.windows(2).any(|pair| pair[0] > pair[1]) {
        return Err(error("archive offsets are not monotonic"));
    }

    let maximum = checked_product(
        &[chunk_width, chunk_height, pixel_size(pixel_format)],
        "archive stream has an unexpected decoded size",
    )?;
    let mut chunks = Vec::with_capacity(count);
    let mut streams = Vec::with_capacity(count);
    for (index, offset) in offsets.iter().copied().enumerate() {
        let stream_end = offsets.get(index + 1).copied().unwrap_or(archive.len());
        let (decoded, cursor, tokens) =
            decode_palette_trace(archive, offset, stream_end, maximum as u64).map_err(png_error)?;
        if decoded.len() != maximum {
            return Err(error("archive stream has an unexpected decoded size"));
        }
        let replay = encode_palette(&decoded, &tokens).map_err(png_error)?;
        let original = subarray(archive, offset, cursor);
        if subarray(original, 0, replay.len()) != replay {
            return Err(error("archive token replay differs"));
        }
        let lookahead = subarray(original, replay.len(), original.len()).to_vec();
        let padding = subarray(archive, cursor, stream_end);
        if padding.len()
            != (stream_alignment - original.len() % stream_alignment) % stream_alignment
            || padding.iter().any(|byte| *byte != 0)
        {
            return Err(error("archive stream padding differs from its alignment"));
        }
        chunks.push(decoded.clone());
        streams.push(ArchiveStream {
            decoded_size: decoded.len(),
            encoded_size: original.len(),
            tokens,
            lookahead,
        });
    }

    let alignment_tail = if tail.is_empty() {
        None
    } else {
        Some(inspect_alignment_tail(tail, 3).map_err(png_error)?)
    };
    let plan = ArchivePlan {
        format: 1,
        codec: "golden-sun-offset-palette-lz".to_string(),
        chunk_width,
        chunk_height,
        columns,
        pixel_format,
        offset_width,
        stream_alignment,
        streams,
        alignment_tail,
    };
    let atlas = make_atlas(&chunks, chunk_width, chunk_height, columns, pixel_format)?;
    if build_archive(&atlas, &plan)? != rom[address..input_end] {
        return Err(error("rebuilt archive differs"));
    }
    Ok((plan, atlas, count))
}

pub fn plan_json(plan: &ArchivePlan) -> String {
    serde_json::to_string(&plan_value(plan)).expect("archive plan is serializable")
}

pub fn self_test() -> Result<(), ArchiveError> {
    let fill = inspect_alignment_tail(&[0xa5, 0xa5, 0xa5], 3).map_err(png_error)?;
    if build_alignment_tail(&fill) != [0xa5, 0xa5, 0xa5] {
        return Err(error("alignment tail self-test failed"));
    }
    let bytes = inspect_alignment_tail(&[0x12, 0x34], 3).map_err(png_error)?;
    let value = serde_json::to_value(&bytes).map_err(png_error)?;
    if parse_alignment_tail(&value, 2, 3, "tail").map_err(png_error)? != bytes {
        return Err(error("alignment tail self-test failed"));
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn archive_plan_tokens_keep_the_tracked_shape() {
        let plan = ArchivePlan {
            format: 1,
            codec: "golden-sun-offset-palette-lz".into(),
            chunk_width: 8,
            chunk_height: 8,
            columns: 1,
            pixel_format: PixelFormat::Indexed8,
            offset_width: 2,
            stream_alignment: 1,
            streams: vec![ArchiveStream {
                decoded_size: 64,
                encoded_size: 4,
                tokens: vec![PaletteGroup::Group(vec![
                    PaletteOperation::Literal,
                    PaletteOperation::Copy {
                        length: 2,
                        distance: 1,
                    },
                    PaletteOperation::End,
                ])],
                lookahead: vec![0xab],
            }],
            alignment_tail: None,
        };
        let text = plan_json(&plan);
        assert!(text.contains(r#""tokens":[["g",[["l"],["c",2,1],["e"]]]]"#));
        assert!(text.ends_with(r#""lookahead":"ab"}]}"#));
    }

    #[test]
    fn self_test_passes() {
        self_test().expect("self-test");
    }
}
