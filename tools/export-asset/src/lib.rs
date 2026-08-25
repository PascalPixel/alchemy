//! GBA graphics exporters.
//!
//! The maintained `png` crate owns PNG framing, filtering and compression.
//! This module owns only the reversible GBA tile/palette layouts.

use std::io::Write;

use flate2::{write::ZlibEncoder, Compression};
use png::{BitDepth, ColorType, Encoder};

use import_asset::{Report, Rgb};

pub const ROM_BASE: u32 = 0x0800_0000;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct ExportError(pub String);

impl std::fmt::Display for ExportError {
    fn fmt(&self, out: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        out.write_str(&self.0)
    }
}

impl std::error::Error for ExportError {}

fn err<T>(message: impl Into<String>) -> Result<T, ExportError> {
    Err(ExportError(message.into()))
}

fn png_error(error: impl std::fmt::Display) -> ExportError {
    ExportError(format!("PNG: {error}"))
}

/// Raw PNG chunk construction remains public for the few fixed-format source
/// images that carry project-specific chunks around the generic encoder.
pub fn chunk(kind: &[u8], payload: &[u8]) -> Vec<u8> {
    let mut body = Vec::with_capacity(kind.len() + payload.len());
    body.extend_from_slice(kind);
    body.extend_from_slice(payload);
    let mut output = Vec::with_capacity(body.len() + 8);
    output.extend_from_slice(&(payload.len() as u32).to_be_bytes());
    output.extend_from_slice(&body);
    output.extend_from_slice(&crc32fast::hash(&body).to_be_bytes());
    output
}

pub fn zlib(data: &[u8]) -> Vec<u8> {
    let mut encoder = ZlibEncoder::new(Vec::new(), Compression::best());
    encoder.write_all(data).expect("writing to memory cannot fail");
    encoder.finish().expect("writing to memory cannot fail")
}

fn encode_png(width: usize, height: usize, color: ColorType, depth: BitDepth, palette: Option<&[u8]>, pixels: &[u8]) -> Result<Vec<u8>, ExportError> {
    let mut output = Vec::new();
    {
        let mut encoder = Encoder::new(&mut output, width as u32, height as u32);
        encoder.set_color(color);
        encoder.set_depth(depth);
        if let Some(palette) = palette {
            encoder.set_palette(palette.to_vec());
        }
        let mut writer = encoder.write_header().map_err(png_error)?;
        writer.write_image_data(pixels).map_err(png_error)?;
    }
    Ok(output)
}

fn dimensions(count: usize, width: f64, message: &str) -> Result<(usize, usize), ExportError> {
    if !width.is_finite() || width <= 0.0 || width.fract() != 0.0 {
        return err(message);
    }
    let width = width as usize;
    if count == 0 || !count.is_multiple_of(width) {
        return err(message);
    }
    Ok((width, count / width))
}

pub fn tile_png(raw: &[u8], bpp: f64, columns: f64, palette_colors: Option<&[Rgb]>) -> Result<(Vec<u8>, Report), ExportError> {
    let four = match bpp {
        4.0 => true,
        8.0 => false,
        _ => return err("tile depth must be 4 or 8bpp"),
    };
    let tile_bytes = if four { 32 } else { 64 };
    if raw.is_empty() || !raw.len().is_multiple_of(tile_bytes) {
        return err("tile data must contain whole nonempty tiles");
    }
    let tiles = raw.len() / tile_bytes;
    let (columns, rows) = dimensions(tiles, columns, "columns must divide the tile count exactly")?;
    let (width, height) = (columns * 8, rows * 8);
    let mut pixels = vec![0; width * height];
    for tile in 0..tiles {
        let source = &raw[tile * tile_bytes..(tile + 1) * tile_bytes];
        let (left, top) = (tile % columns * 8, tile / columns * 8);
        for y in 0..8 {
            for x in 0..8 {
                let index = y * 8 + x;
                pixels[(top + y) * width + left + x] = if four {
                    let byte = source[index / 2];
                    if index & 1 == 0 {
                        byte & 15
                    } else {
                        byte >> 4
                    }
                } else {
                    source[index]
                };
            }
        }
    }
    let generated: Vec<Rgb> = if four { (0..16).map(|i| [i * 16, i * 16, i * 16]).collect() } else { (0..256u16).map(|i| [((i & 31) * 8) as u8, (((i >> 5) & 7) * 8) as u8, 0]).collect() };
    let palette = palette_colors.unwrap_or(&generated);
    let limit = if four { 16 } else { 256 };
    if palette.is_empty() || palette.len() > limit || pixels.iter().any(|pixel| *pixel as usize >= palette.len()) {
        return err("tile pixels reference a missing palette entry");
    }
    let palette_bytes: Vec<u8> = palette.iter().flatten().copied().collect();
    let data = if four { pixels.chunks_exact(2).map(|pair| pair[0] << 4 | pair[1]).collect() } else { pixels };
    let image = encode_png(width, height, ColorType::Indexed, if four { BitDepth::Four } else { BitDepth::Eight }, Some(&palette_bytes), &data)?;
    let mut report = Report::default();
    report.set("width", width as f64);
    report.set("height", height as f64);
    report.set("bpp", bpp);
    report.set("tiles", tiles as f64);
    report.set("columns", columns as f64);
    Ok((image, report))
}

pub fn palette_png(raw: &[u8]) -> Result<(Vec<u8>, Report), ExportError> {
    if raw.is_empty() || !raw.len().is_multiple_of(2) || raw.len() > 512 {
        return err("palette must contain 1..256 BGR555 entries");
    }
    let mut palette = Vec::with_capacity(raw.len() / 2 * 3);
    for bytes in raw.chunks_exact(2) {
        let value = u16::from_le_bytes([bytes[0], bytes[1]]);
        if value & 0x8000 != 0 {
            return err("palette contains a non-BGR555 high bit");
        }
        palette.extend([((value & 31) << 3) as u8, (((value >> 5) & 31) << 3) as u8, (((value >> 10) & 31) << 3) as u8]);
    }
    let entries = raw.len() / 2;
    let mut pixels: Vec<u8> = (0..entries).map(|index| index as u8).collect();
    pixels.resize(256, 0);
    let image = encode_png(16, 16, ColorType::Indexed, BitDepth::Eight, Some(&palette), &pixels)?;
    let mut report = Report::default();
    report.set("width", 16.0);
    report.set("height", 16.0);
    report.set("palette_entries", entries as f64);
    Ok((image, report))
}

pub fn byte_png(raw: &[u8], width: f64) -> Result<(Vec<u8>, Report), ExportError> {
    let (width, height) = dimensions(raw.len(), width, "byte image dimensions must be nonzero multiples of eight")?;
    if !width.is_multiple_of(8) || !height.is_multiple_of(8) {
        return err("byte image dimensions must be nonzero multiples of eight");
    }
    let palette: Vec<u8> = (0..=255).flat_map(|value| [value, value, value]).collect();
    let image = encode_png(width, height, ColorType::Indexed, BitDepth::Eight, Some(&palette), raw)?;
    let mut report = Report::default();
    report.set("width", width as f64);
    report.set("height", height as f64);
    report.set("bytes", raw.len() as f64);
    Ok((image, report))
}

pub fn rgba_image(raw: &[u8], width: f64) -> Result<(Vec<u8>, Report), ExportError> {
    if !raw.len().is_multiple_of(4) {
        return err("RGBA input must contain whole nonempty pixels");
    }
    let pixels = raw.len() / 4;
    let (width, height) = dimensions(pixels, width, "width must divide the RGBA pixel count")?;
    let image = encode_png(width, height, ColorType::Rgba, BitDepth::Eight, None, raw)?;
    let mut report = Report::default();
    report.set("width", width as f64);
    report.set("height", height as f64);
    report.set("pixels", pixels as f64);
    Ok((image, report))
}

pub fn palette_rgba_image(raw: &[u8], width: f64) -> Result<(Vec<u8>, Report), ExportError> {
    if raw.is_empty() || !raw.len().is_multiple_of(2) {
        return err("palette RGBA dimensions must divide whole colors");
    }
    let mut pixels = Vec::with_capacity(raw.len() * 2);
    for bytes in raw.chunks_exact(2) {
        let value = u16::from_le_bytes([bytes[0], bytes[1]]);
        pixels.extend([((value & 31) << 3) as u8, (((value >> 5) & 31) << 3) as u8, (((value >> 10) & 31) << 3) as u8, if value & 0x8000 == 0 { 255 } else { 254 }]);
    }
    let (image, mut report) = rgba_image(&pixels, width).map_err(|_| ExportError("palette RGBA dimensions must divide whole colors".into()))?;
    report.set("palette_entries", (raw.len() / 2) as f64);
    Ok((image, report))
}

pub fn js_number(text: &str) -> f64 {
    let text = text.trim();
    if text.is_empty() {
        return 0.0;
    }
    if let Some(hex) = text.strip_prefix("0x").or_else(|| text.strip_prefix("0X")) {
        return u64::from_str_radix(hex, 16).map_or(f64::NAN, |value| value as f64);
    }
    text.parse().unwrap_or(f64::NAN)
}

pub fn cli_number(text: &str) -> Result<f64, ExportError> {
    let value = js_number(text);
    if value.is_finite() && value.fract() == 0.0 {
        Ok(value)
    } else {
        err(format!("invalid integer: {text}"))
    }
}

pub fn self_test() -> Result<String, ExportError> {
    for (bpp, raw, columns) in [(4.0, vec![0x10; 32], 1.0), (8.0, (0..64).collect(), 1.0)] {
        let image = tile_png(&raw, bpp, columns, None)?.0;
        if import_asset::gba_graphics(&image, bpp).map_err(|e| ExportError(e.0))?.0 != raw {
            return err("tile PNG round-trip failed");
        }
    }
    Ok("self-test=ok".into())
}
