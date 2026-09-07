//! Build offset-table palette-LZ archives from an authored plan and PNG atlas.

use std::fmt;

use alignment_tail::{build_alignment_tail, AlignmentTail};
use extract_resource::{encode_palette, PaletteGroup};
use import_asset::{indexed_png, rgba_png};

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum PixelFormat {
    Rgba,
    Indexed8,
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

fn pixel_size(pixel_format: PixelFormat) -> usize {
    match pixel_format {
        PixelFormat::Rgba => 4,
        PixelFormat::Indexed8 => 1,
    }
}

fn png_error(message: impl fmt::Display) -> ArchiveError {
    error(message.to_string())
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

/// Rebuild an archive from an atlas and its authored plan.
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

#[cfg(test)]
mod tests {
    use super::*;
    use extract_resource::PaletteOperation;

    fn indexed_atlas(pixels: &[u8]) -> Vec<u8> {
        let mut atlas = Vec::new();
        let mut encoder = png::Encoder::new(&mut atlas, 8, 8);
        encoder.set_color(png::ColorType::Indexed);
        encoder.set_depth(png::BitDepth::Eight);
        encoder.set_palette(vec![0, 0, 0]);
        encoder
            .write_header()
            .unwrap()
            .write_image_data(pixels)
            .unwrap();
        atlas
    }

    #[test]
    fn rebuilds_an_authored_archive_from_its_atlas() {
        let pixels = vec![0; 64];
        let atlas = indexed_atlas(&pixels);
        let tokens = vec![
            PaletteGroup::Zeros,
            PaletteGroup::Group(vec![
                PaletteOperation::Copy {
                    length: 56,
                    distance: 8,
                },
                PaletteOperation::End,
            ]),
        ];
        let payload = encode_palette(&pixels, &tokens).unwrap();
        let plan = ArchivePlan {
            chunk_width: 8,
            chunk_height: 8,
            columns: 1,
            pixel_format: PixelFormat::Indexed8,
            offset_width: 2,
            stream_alignment: 4,
            streams: vec![ArchiveStream {
                decoded_size: pixels.len(),
                encoded_size: payload.len(),
                tokens,
                lookahead: vec![],
            }],
            alignment_tail: None,
        };

        assert_eq!(
            read_atlas(&atlas, 1, 8, 8, 1, PixelFormat::Indexed8).unwrap(),
            vec![pixels]
        );
        let rebuilt = build_archive(&atlas, &plan).unwrap();
        assert_eq!(&rebuilt[..2], &[2, 0]);
        assert_eq!(&rebuilt[2..2 + payload.len()], payload);
        assert_eq!(&rebuilt[2 + payload.len()..], &[0]);
    }
}
