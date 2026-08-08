//! PNG encoders for GBA asset data -- the export half of the round trip whose
//! import half lives in `import-asset`.
//!
//! WHY THIS FILE EXISTS: every asset this repo tracks has to survive a full
//! round trip, ROM bytes -> PNG -> ROM bytes, unchanged. That is only testable
//! if the encoder and the decoder are separate implementations that agree, so
//! the decoders live in `import-asset` and the encoders live here, and the
//! self-test drives real data through both directions rather than asserting
//! that either one alone "looks right".
//!
//! Every compressed byte written here goes through `alchemy-zlib`'s
//! deterministic DEFLATE rather than a host zlib, for the reason that crate's
//! header gives: a PNG this repo tracks must be reproducible byte-for-byte on
//! darwin and linux, arm64 and x86_64. Compression ratio loses to determinism.
//!
//! Port of `tools/lib/export_asset.ts`.

use alchemy_zlib::{crc32, deflate_sync, DeflateOptions};
use import_asset::{js_number_json, Report, Rgb};

pub const ROM_BASE: u32 = 0x0800_0000;

const PNG_SIGNATURE: [u8; 8] = [0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a];

/// PORT NOTE: every `throw new Error(...)` in the TypeScript becomes one of
/// these, with the message preserved character for character -- callers and
/// the parity harness compare the text.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct ExportError(pub String);

impl std::fmt::Display for ExportError {
    fn fmt(&self, out: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        out.write_str(&self.0)
    }
}

impl std::error::Error for ExportError {}

fn err<T>(message: &str) -> Result<T, ExportError> {
    Err(ExportError(message.to_string()))
}

/// `Buffer#set` / `Buffer.from(array)` coerce each element with ToUint8: a
/// non-integral value truncates toward zero and the result is taken mod 256,
/// and a NaN becomes 0.
///
/// PORT NOTE: this is reachable. `--bpp 7` and `--bpp 260` pass the CLI's
/// `Number.isInteger` check and land in the IHDR depth byte, so the depth
/// written for `--bpp 260` is 4, not a clamp to 255 and not an error. Rust's
/// `as u8` saturates, which would write 255 instead.
fn to_uint8(value: f64) -> u8 {
    if !value.is_finite() {
        return 0;
    }
    (value.trunc() as i64).rem_euclid(256) as u8
}

/// A PNG chunk: big-endian payload length, the four-byte kind, the payload,
/// and a CRC-32 over kind+payload (not over the length).
pub fn chunk(kind: &[u8], payload: &[u8]) -> Vec<u8> {
    let mut body = Vec::with_capacity(kind.len() + payload.len());
    body.extend_from_slice(kind);
    body.extend_from_slice(payload);
    let mut out = Vec::with_capacity(8 + body.len());
    out.extend_from_slice(&(payload.len() as u32).to_be_bytes());
    out.extend_from_slice(&body);
    out.extend_from_slice(&crc32(&body).to_be_bytes());
    out
}

fn png(parts: &[Vec<u8>]) -> Vec<u8> {
    let mut out = PNG_SIGNATURE.to_vec();
    for part in parts {
        out.extend_from_slice(part);
    }
    out
}

fn ihdr(width: u32, height: u32, depth: f64, color: u8) -> Vec<u8> {
    let mut value = vec![0u8; 13];
    value[0..4].copy_from_slice(&width.to_be_bytes());
    value[4..8].copy_from_slice(&height.to_be_bytes());
    value[8] = to_uint8(depth);
    value[9] = color;
    value
}

fn idat(scanlines: &[u8]) -> Vec<u8> {
    chunk(
        b"IDAT",
        &deflate_sync(scanlines, DeflateOptions { level: Some(9) }),
    )
}

/// JavaScript's `x <= 0`, which is *false* for NaN -- so a NaN dimension falls
/// through this guard and is caught by the divisibility check instead.
///
/// PORT NOTE: written as a helper rather than `!(x > 0.0)` because clippy
/// rejects negated comparisons on partially ordered types, and the NaN
/// behaviour is exactly the subtlety it is warning about.
fn js_not_positive(value: f64) -> bool {
    matches!(
        value.partial_cmp(&0.0),
        Some(std::cmp::Ordering::Less | std::cmp::Ordering::Equal)
    )
}

/// JavaScript's `%` on numbers is a truncated remainder over f64, not integer
/// modulo: it is defined for negative and non-integral operands, and yields
/// NaN when the divisor is zero. The dimension checks below rely on that
/// (`count % columns !== 0` must be *true*, not a panic, when `columns` is 0).
fn js_remainder(left: f64, right: f64) -> f64 {
    if right == 0.0 || !left.is_finite() || !right.is_finite() {
        return f64::NAN;
    }
    left % right
}

/// Encodes 4bpp or 8bpp GBA tile data as an indexed PNG, laying tiles out
/// left to right in `columns`-wide rows.
///
/// `palette_colors` of `None` is the TypeScript's `undefined`/`null`: a
/// synthetic ramp stands in so the PNG is viewable, and the indices -- the
/// only part that round-trips -- are unaffected.
pub fn tile_png(
    raw: &[u8],
    bpp: f64,
    columns: f64,
    palette_colors: Option<&[Rgb]>,
) -> Result<(Vec<u8>, Report), ExportError> {
    // PORT NOTE: the TypeScript tests `bpp === 4`; every other value, 7 and
    // NaN included, takes the 8bpp path while still being echoed into the
    // report and the IHDR depth byte.
    let four = bpp == 4.0;
    let unit = if four { 32usize } else { 64 };
    if raw.is_empty() || !raw.len().is_multiple_of(unit) {
        return err("tile data must contain whole nonempty tiles");
    }
    let count = (raw.len() / unit) as f64;
    if js_not_positive(columns) || js_remainder(count, columns) != 0.0 {
        return err("columns must divide the tile count exactly");
    }
    let columns_count = columns as usize;
    let rows = count as usize / columns_count;
    let width = columns_count * 8;
    let height = rows * 8;

    let mut pixels = vec![0u8; width * height];
    for tile in 0..count as usize {
        let source = &raw[tile * unit..(tile + 1) * unit];
        let indices: Vec<u8> = if four {
            source
                .iter()
                .flat_map(|byte| [byte & 15, byte >> 4])
                .collect()
        } else {
            source.to_vec()
        };
        let left = tile % columns_count * 8;
        let top = tile / columns_count * 8;
        for y in 0..8 {
            let start = (top + y) * width + left;
            pixels[start..start + 8].copy_from_slice(&indices[y * 8..y * 8 + 8]);
        }
    }

    let mut scanlines: Vec<u8> =
        Vec::with_capacity(height * (width / if four { 2 } else { 1 } + 1));
    for y in 0..height {
        scanlines.push(0);
        let row = &pixels[y * width..(y + 1) * width];
        if four {
            for index in (0..width).step_by(2) {
                scanlines.push(row[index] << 4 | row[index + 1]);
            }
        } else {
            scanlines.extend_from_slice(row);
        }
    }

    let colors = if four { 16usize } else { 256 };
    let generated: Vec<Rgb> = if four {
        (0..16u8)
            .map(|index| [index * 16, index * 16, index * 16])
            .collect()
    } else {
        (0..256u32)
            .map(|index| [((index & 31) * 8) as u8, ((index >> 5) * 8) as u8, 0])
            .collect()
    };
    let palette: &[Rgb] = palette_colors.unwrap_or(&generated);
    if palette.is_empty() || palette.len() > colors {
        return err("palette does not fit the requested tile depth");
    }
    // PORT NOTE: the TypeScript writes `Math.max(...pixels)`, which spreads
    // every pixel as a call argument and throws a RangeError once the image
    // passes bun's argument limit (around 100k pixels, i.e. a ~1600-tile
    // export). Rust has no such ceiling, so large exports that fail there
    // succeed here. Diverging toward "works" is the right direction; the
    // check itself is identical below the limit.
    if pixels.iter().copied().max().unwrap_or(0) as usize >= palette.len() {
        return err("tile pixels reference a missing palette entry");
    }
    let palette_bytes: Vec<u8> = palette.iter().flatten().copied().collect();

    let image = png(&[
        chunk(b"IHDR", &ihdr(width as u32, height as u32, bpp, 3)),
        chunk(b"PLTE", &palette_bytes),
        idat(&scanlines),
        chunk(b"IEND", &[]),
    ]);
    let mut report = Report::default();
    report.set("width", width as f64);
    report.set("height", height as f64);
    report.set("bpp", bpp);
    report.set("tiles", count);
    report.set("columns", columns);
    Ok((image, report))
}

/// Encodes a BGR555 palette as a 16x16 indexed swatch grid, one pixel per
/// entry, padded with index 0 so the image is always 16x16 regardless of how
/// many entries the palette actually has.
pub fn palette_png(raw: &[u8]) -> Result<(Vec<u8>, Report), ExportError> {
    if raw.is_empty() || !raw.len().is_multiple_of(2) || raw.len() > 512 {
        return err("palette must contain 1..256 BGR555 entries");
    }
    let entries = raw.len() / 2;
    let mut palette: Vec<u8> = Vec::with_capacity(entries * 3);
    for entry in raw.chunks_exact(2) {
        let value = u16::from_le_bytes([entry[0], entry[1]]);
        if value & 0x8000 != 0 {
            return err("palette contains a non-BGR555 high bit");
        }
        palette.push(((value & 31) << 3) as u8);
        palette.push((((value >> 5) & 31) << 3) as u8);
        palette.push((((value >> 10) & 31) << 3) as u8);
    }
    let mut pixels: Vec<u8> = (0..entries).map(|index| index as u8).collect();
    pixels.resize(256, 0);
    let mut scanlines: Vec<u8> = Vec::with_capacity(16 * 17);
    for row in 0..16 {
        scanlines.push(0);
        scanlines.extend_from_slice(&pixels[row * 16..(row + 1) * 16]);
    }
    let image = png(&[
        chunk(b"IHDR", &ihdr(16, 16, 8.0, 3)),
        chunk(b"PLTE", &palette),
        idat(&scanlines),
        chunk(b"IEND", &[]),
    ]);
    let mut report = Report::default();
    report.set("width", 16.0);
    report.set("height", 16.0);
    report.set("palette_entries", entries as f64);
    Ok((image, report))
}

/// Encodes arbitrary bytes as a grayscale-palette indexed PNG, so a byte
/// region can be eyeballed for structure without pretending it is tile data.
pub fn byte_png(raw: &[u8], width: f64) -> Result<(Vec<u8>, Report), ExportError> {
    let length = raw.len() as f64;
    if raw.is_empty()
        || js_not_positive(width)
        || js_remainder(length, width) != 0.0
        || js_remainder(width, 8.0) != 0.0
        || js_remainder(length / width, 8.0) != 0.0
    {
        return err("byte image dimensions must be nonzero multiples of eight");
    }
    let width_count = width as usize;
    let height = raw.len() / width_count;
    let palette: Vec<u8> = (0..256u32).flat_map(|index| [index as u8; 3]).collect();
    let mut rows: Vec<u8> = Vec::with_capacity(height * (width_count + 1));
    for row in 0..height {
        rows.push(0);
        rows.extend_from_slice(&raw[row * width_count..(row + 1) * width_count]);
    }
    let image = png(&[
        chunk(b"IHDR", &ihdr(width_count as u32, height as u32, 8.0, 3)),
        chunk(b"PLTE", &palette),
        idat(&rows),
        chunk(b"IEND", &[]),
    ]);
    let mut report = Report::default();
    report.set("width", width);
    report.set("height", height as f64);
    report.set("bytes", raw.len() as f64);
    Ok((image, report))
}

/// Encodes raw RGBA8888 bytes as a truecolour-with-alpha PNG.
pub fn rgba_image(raw: &[u8], width: f64) -> Result<(Vec<u8>, Report), ExportError> {
    if raw.is_empty() || !raw.len().is_multiple_of(4) || js_not_positive(width) {
        return err("RGBA input must contain whole nonempty pixels");
    }
    let pixels = (raw.len() / 4) as f64;
    if js_remainder(pixels, width) != 0.0 {
        return err("width must divide the RGBA pixel count");
    }
    let width_count = width as usize;
    let height = pixels as usize / width_count;
    let mut rows: Vec<u8> = Vec::with_capacity(height * (width_count * 4 + 1));
    for row in 0..height {
        rows.push(0);
        rows.extend_from_slice(&raw[row * width_count * 4..(row + 1) * width_count * 4]);
    }
    let image = png(&[
        chunk(b"IHDR", &ihdr(width_count as u32, height as u32, 8.0, 6)),
        idat(&rows),
        chunk(b"IEND", &[]),
    ]);
    let mut report = Report::default();
    report.set("width", width);
    report.set("height", height as f64);
    report.set("pixels", pixels);
    Ok((image, report))
}

/// Encodes a BGR555 palette as an RGBA image, one pixel per colour, carrying
/// the GBA's high bit through as alpha 254 so it survives the round trip
/// instead of being silently dropped the way a 0x8000 rejection would.
pub fn palette_rgba_image(raw: &[u8], width: f64) -> Result<(Vec<u8>, Report), ExportError> {
    if raw.is_empty()
        || !raw.len().is_multiple_of(2)
        || js_not_positive(width)
        || js_remainder((raw.len() / 2) as f64, width) != 0.0
    {
        return err("palette RGBA dimensions must divide whole colors");
    }
    let mut pixels = vec![0u8; raw.len() * 2];
    for (index, entry) in raw.chunks_exact(2).enumerate() {
        let value = u16::from_le_bytes([entry[0], entry[1]]);
        let target = index * 4;
        pixels[target] = ((value & 31) << 3) as u8;
        pixels[target + 1] = (((value >> 5) & 31) << 3) as u8;
        pixels[target + 2] = (((value >> 10) & 31) << 3) as u8;
        pixels[target + 3] = if value & 0x8000 != 0 { 254 } else { 255 };
    }
    let (image, mut report) = rgba_image(&pixels, width)?;
    report.set("palette_entries", (raw.len() / 2) as f64);
    Ok((image, report))
}

/// `Number(text)` over the ECMAScript StringNumericLiteral grammar, which is
/// not `f64::from_str`: `Number("0x10")` is 16, `Number("")` is 0,
/// `Number("12abc")` is NaN, and `Number("inf")` is NaN where Rust's parser
/// would accept it.
pub fn js_number(text: &str) -> f64 {
    let trimmed = text.trim_matches(|c: char| {
        matches!(c, '\u{9}'..='\u{d}' | ' ' | '\u{a0}' | '\u{1680}' | '\u{2000}'..='\u{200a}'
            | '\u{2028}' | '\u{2029}' | '\u{202f}' | '\u{205f}' | '\u{3000}' | '\u{feff}')
    });
    if trimmed.is_empty() {
        return 0.0;
    }
    let (sign, body) = match trimmed.as_bytes()[0] {
        b'+' => (1.0, &trimmed[1..]),
        b'-' => (-1.0, &trimmed[1..]),
        _ => (1.0, trimmed),
    };
    if body == "Infinity" {
        return sign * f64::INFINITY;
    }
    if sign > 0.0 && body.len() > 2 {
        let radix = match &body[..2] {
            "0x" | "0X" => Some(16),
            "0o" | "0O" => Some(8),
            "0b" | "0B" => Some(2),
            _ => None,
        };
        if let Some(radix) = radix {
            return match u64::from_str_radix(&body[2..], radix) {
                Ok(value) => value as f64,
                Err(_) => f64::NAN,
            };
        }
    }
    if body.eq_ignore_ascii_case("inf")
        || body.eq_ignore_ascii_case("infinity")
        || body.eq_ignore_ascii_case("nan")
    {
        return f64::NAN;
    }
    match body.parse::<f64>() {
        Ok(value) => sign * value,
        Err(_) => f64::NAN,
    }
}

/// `Number.isInteger` after `Number(text)`, which is how the CLI validates
/// every numeric flag.
pub fn cli_number(text: &str) -> Result<f64, ExportError> {
    let value = js_number(text);
    if !value.is_finite() || value.fract() != 0.0 {
        return Err(ExportError(format!("invalid integer: {text}")));
    }
    Ok(value)
}

/// Drives real data through both directions of every encoder, decoding with
/// `import-asset` rather than re-deriving the expected bytes here: an encoder
/// checked only against its own idea of the right answer proves nothing.
pub fn self_test() -> Result<String, ExportError> {
    let decode = |error: import_asset::AssetError| ExportError(error.0);

    for (bpp, size, columns) in [(4.0f64, 32 * 7usize, 7.0f64), (8.0, 64 * 4, 4.0)] {
        let raw: Vec<u8> = (0..size)
            .map(|index| ((index * 37 + 11) & 255) as u8)
            .collect();
        let (image, _) = tile_png(&raw, bpp, columns, None)?;
        if import_asset::gba_graphics(&image, bpp).map_err(decode)?.0 != raw {
            return Err(ExportError(format!(
                "{}bpp tile round-trip failed",
                js_number_json(bpp)
            )));
        }
    }

    let colors: Vec<Rgb> = (0..256u32)
        .map(|index| {
            [
                ((index & 31) << 3) as u8,
                (((index >> 3) & 31) << 3) as u8,
                (((index >> 6) & 3) << 3) as u8,
            ]
        })
        .collect();
    let raw: Vec<u8> = (0..64 * 4usize)
        .map(|index| ((index * 53 + 7) & 255) as u8)
        .collect();
    let (image, _) = tile_png(&raw, 8.0, 4.0, Some(&colors))?;
    let (tiles, palette, _) = import_asset::gba_graphics(&image, 8.0).map_err(decode)?;
    let mut expected_palette = vec![0u8; 512];
    for (index, [red, green, blue]) in colors.iter().copied().enumerate() {
        let value =
            u16::from(red >> 3) | (u16::from(green >> 3) << 5) | (u16::from(blue >> 3) << 10);
        expected_palette[index * 2..index * 2 + 2].copy_from_slice(&value.to_le_bytes());
    }
    if tiles != raw || palette != expected_palette {
        return err("palette-correct tile round-trip failed");
    }

    let mut raw = vec![0u8; 448];
    for index in 0..224usize {
        let value = ((index * 109) & 0x7fff) as u16;
        raw[index * 2..index * 2 + 2].copy_from_slice(&value.to_le_bytes());
    }
    let (image, _) = palette_png(&raw)?;
    if import_asset::gba_graphics(&image, 8.0).map_err(decode)?.1 != raw {
        return err("palette round-trip failed");
    }

    let raw: Vec<u8> = (0..128 * 64usize)
        .map(|index| ((index * 53 + 7) & 255) as u8)
        .collect();
    let (image, _) = byte_png(&raw, 128.0)?;
    let decoded = import_asset::indexed_png(&image).map_err(decode)?;
    if decoded
        .pixels
        .iter()
        .map(|value| *value as u8)
        .collect::<Vec<u8>>()
        != raw
    {
        return err("indexed byte-image round-trip failed");
    }

    let raw: Vec<u8> = (0..11 * 7 * 4usize)
        .map(|index| ((index * 29 + 3) & 255) as u8)
        .collect();
    let (image, _) = rgba_image(&raw, 11.0)?;
    let rgba = import_asset::rgba_png(&image).map_err(decode)?;
    if rgba.width != 11 || rgba.height != 7 || rgba.pixels != raw {
        return err("RGBA image round-trip failed");
    }

    let mut raw = vec![0u8; 256];
    for index in 0..128usize {
        let value = ((index * 109) & 0xffff) as u16;
        raw[index * 2..index * 2 + 2].copy_from_slice(&value.to_le_bytes());
    }
    let (image, _) = palette_rgba_image(&raw, 16.0)?;
    if import_asset::gba_palette_rgba(&image).map_err(decode)?.0 != raw {
        return err("RGBA palette round-trip failed");
    }

    Ok("self-test=ok".to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    fn ramp(length: usize, step: usize, offset: usize) -> Vec<u8> {
        (0..length)
            .map(|index| ((index * step + offset) & 255) as u8)
            .collect()
    }

    #[test]
    fn the_ported_self_test_passes() {
        assert_eq!(self_test().expect("self-test"), "self-test=ok");
    }

    #[test]
    fn a_chunk_carries_a_crc_over_kind_and_payload_but_not_the_length() {
        let encoded = chunk(b"IEND", &[]);
        assert_eq!(
            encoded,
            vec![0, 0, 0, 0, b'I', b'E', b'N', b'D', 0xae, 0x42, 0x60, 0x82]
        );
        let sized = chunk(b"IDAT", &[1, 2, 3]);
        assert_eq!(&sized[..4], &[0, 0, 0, 3]);
        assert_eq!(&sized[4..11], b"IDAT\x01\x02\x03");
        assert_eq!(&sized[11..], &crc32(b"IDAT\x01\x02\x03").to_be_bytes());
    }

    #[test]
    fn every_encoder_emits_the_png_signature_and_ends_with_iend() {
        let (image, _) = byte_png(&ramp(64, 1, 0), 8.0).expect("byte png");
        assert_eq!(&image[..8], &PNG_SIGNATURE);
        assert_eq!(&image[image.len() - 8..], b"IEND\xae\x42\x60\x82");
    }

    #[test]
    fn tile_dimensions_are_rejected_before_anything_is_encoded() {
        assert_eq!(
            tile_png(&[], 4.0, 1.0, None),
            err("tile data must contain whole nonempty tiles")
        );
        assert_eq!(
            tile_png(&ramp(31, 1, 0), 4.0, 1.0, None),
            err("tile data must contain whole nonempty tiles")
        );
        // 8bpp tiles are 64 bytes, so a 32-byte input is a whole 4bpp tile and
        // a partial 8bpp one.
        assert!(tile_png(&ramp(32, 1, 0), 4.0, 1.0, None).is_ok());
        assert_eq!(
            tile_png(&ramp(32, 1, 0), 8.0, 1.0, None),
            err("tile data must contain whole nonempty tiles")
        );
    }

    #[test]
    fn a_zero_or_indivisible_column_count_is_an_error_not_a_division_by_zero() {
        // JS `count % 0` is NaN, which is `!== 0`, so this is an error rather
        // than a panic -- the reason the check goes through js_remainder.
        assert_eq!(
            tile_png(&ramp(64, 1, 0), 4.0, 0.0, None),
            err("columns must divide the tile count exactly")
        );
        assert_eq!(
            tile_png(&ramp(96, 1, 0), 4.0, 2.0, None),
            err("columns must divide the tile count exactly")
        );
        assert!(tile_png(&ramp(96, 1, 0), 4.0, 3.0, None).is_ok());
    }

    #[test]
    fn any_bpp_other_than_four_takes_the_eight_bit_path_and_is_echoed_verbatim() {
        // `--bpp 7` passes the CLI's integer check and reaches here.
        let (image, report) = tile_png(&ramp(64, 5, 1), 7.0, 1.0, None).expect("odd bpp");
        assert_eq!(report.get("bpp"), Some(7.0));
        // IHDR depth is byte 24 of the file: 8 signature + 8 chunk header + 8.
        assert_eq!(image[8 + 8 + 8], 7);
        assert_eq!(report.get("tiles"), Some(1.0));
    }

    #[test]
    fn the_ihdr_depth_byte_wraps_mod_256_the_way_buffer_set_does() {
        // `as u8` would saturate to 255 here; ToUint8 wraps to 4.
        assert_eq!(to_uint8(260.0), 4);
        assert_eq!(to_uint8(-1.0), 255);
        assert_eq!(to_uint8(f64::NAN), 0);
        assert_eq!(to_uint8(7.9), 7);
        let (image, _) = tile_png(&ramp(64, 5, 1), 260.0, 1.0, None).expect("wrapping bpp");
        assert_eq!(image[8 + 8 + 8], 4);
    }

    #[test]
    fn a_supplied_palette_must_cover_every_index_used() {
        let raw = ramp(64, 1, 0);
        let two = [[0u8, 0, 0], [8, 8, 8]];
        assert_eq!(
            tile_png(&raw, 8.0, 1.0, Some(&two)),
            err("tile pixels reference a missing palette entry")
        );
        let empty: [Rgb; 0] = [];
        assert_eq!(
            tile_png(&raw, 8.0, 1.0, Some(&empty)),
            err("palette does not fit the requested tile depth")
        );
        // 4bpp caps the palette at 16 entries even though the PNG could hold
        // more.
        let seventeen: Vec<Rgb> = (0..17u8).map(|index| [index, index, index]).collect();
        assert_eq!(
            tile_png(&ramp(32, 0, 0), 4.0, 1.0, Some(&seventeen)),
            err("palette does not fit the requested tile depth")
        );
    }

    #[test]
    fn four_bpp_nibble_order_is_low_nibble_first() {
        // One tile of 0x10 bytes decodes to pixel pairs (0, 1), which repack
        // into the scanline as 0x01 -- the swap is the whole point.
        let (image, _) = tile_png(&[0x10u8; 32], 4.0, 1.0, None).expect("nibble tile");
        let decoded = import_asset::indexed_png(&image).expect("decode");
        assert_eq!(&decoded.pixels[..4], &[0, 1, 0, 1]);
    }

    #[test]
    fn palette_png_rejects_the_gba_high_bit_and_oversized_palettes() {
        assert_eq!(
            palette_png(&[]),
            err("palette must contain 1..256 BGR555 entries")
        );
        assert_eq!(
            palette_png(&[0]),
            err("palette must contain 1..256 BGR555 entries")
        );
        assert_eq!(
            palette_png(&vec![0u8; 514]),
            err("palette must contain 1..256 BGR555 entries")
        );
        assert_eq!(
            palette_png(&[0x00, 0x80]),
            err("palette contains a non-BGR555 high bit")
        );
        let (_, report) = palette_png(&[0x1f, 0x00, 0xe0, 0x03]).expect("two colors");
        // The image is always the full 16x16 grid, however few entries it has.
        assert_eq!(report.get("width"), Some(16.0));
        assert_eq!(report.get("palette_entries"), Some(2.0));
    }

    #[test]
    fn byte_png_requires_both_dimensions_to_be_multiples_of_eight() {
        assert!(byte_png(&ramp(64, 1, 0), 8.0).is_ok());
        for width in [0.0, 4.0, 16.0] {
            assert_eq!(
                byte_png(&ramp(64, 1, 0), width),
                err("byte image dimensions must be nonzero multiples of eight"),
                "width {width}"
            );
        }
        assert_eq!(
            byte_png(&[], 8.0),
            err("byte image dimensions must be nonzero multiples of eight")
        );
    }

    #[test]
    fn rgba_reports_carry_pixel_counts_and_palette_alpha_survives_the_high_bit() {
        let (_, report) = rgba_image(&ramp(11 * 7 * 4, 29, 3), 11.0).expect("rgba");
        assert_eq!(report.get("pixels"), Some(77.0));
        assert_eq!(report.get("height"), Some(7.0));
        // 0x8000 set -> alpha 254, clear -> 255; that is what makes the GBA
        // high bit round-trip instead of being dropped.
        let (image, report) = palette_rgba_image(&[0x1f, 0x00, 0x1f, 0x80], 2.0).expect("palette");
        assert_eq!(report.get("palette_entries"), Some(2.0));
        let decoded = import_asset::rgba_png(&image).expect("decode");
        assert_eq!(decoded.pixels[3], 255);
        assert_eq!(decoded.pixels[7], 254);
    }

    #[test]
    fn report_keys_are_the_ones_the_cli_prints() {
        let (_, report) = tile_png(&ramp(64, 1, 0), 8.0, 1.0, None).expect("tiles");
        assert_eq!(
            import_asset::sorted_json(&report),
            "{\"bpp\": 8, \"columns\": 1, \"height\": 8, \"tiles\": 1, \"width\": 8}"
        );
    }

    #[test]
    fn cli_numbers_follow_the_ecmascript_grammar() {
        assert_eq!(js_number("0x10"), 16.0);
        assert_eq!(js_number(""), 0.0);
        assert!(js_number("12abc").is_nan());
        assert!(js_number("inf").is_nan());
        assert_eq!(cli_number("0x800").expect("hex"), 2048.0);
        assert_eq!(cli_number("-8").expect("negative"), -8.0);
        assert_eq!(cli_number("1.5"), err("invalid integer: 1.5"));
        assert_eq!(cli_number("abc"), err("invalid integer: abc"));
        // `Number("")` is 0, and 0 is an integer, so an empty flag value is
        // accepted here and rejected later by the dimension checks.
        assert_eq!(cli_number("").expect("empty"), 0.0);
    }
}
