//! PNG images, GBA tile packing and palette conversion.
use super::{err, AssetError};
use std::io::Cursor;
pub type Rgb = [u8; 3];
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct ImageSize {
    pub width: u32,
    pub height: u32,
}

pub const PNG_SIGNATURE: [u8; 8] = [0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a];

fn png_error(error: impl std::fmt::Display) -> AssetError {
    AssetError(error.to_string())
}

fn decode(data: &[u8]) -> Result<(png::OutputInfo, Vec<u8>, png::Info<'static>), AssetError> {
    if !data.starts_with(&PNG_SIGNATURE) {
        return err("invalid file signature");
    }
    let mut decoder = png::Decoder::new(Cursor::new(data));
    decoder.set_transformations(png::Transformations::IDENTITY);
    let mut reader = decoder.read_info().map_err(png_error)?;
    let info = reader.info().clone();
    let mut bytes = vec![0; reader.output_buffer_size()];
    let output = reader.next_frame(&mut bytes).map_err(png_error)?;
    bytes.truncate(output.buffer_size());
    Ok((output, bytes, info))
}

#[derive(Debug)]
pub struct IndexedImage {
    pub width: u32,
    pub height: u32,
    pub pixels: Vec<u32>,
    pub palette: Vec<Rgb>,
    pub has_transparency: bool,
}

pub fn indexed_png(data: &[u8]) -> Result<IndexedImage, AssetError> {
    let (output, bytes, info) = decode(data)?;
    if output.color_type != png::ColorType::Indexed
        || !matches!(
            output.bit_depth,
            png::BitDepth::One | png::BitDepth::Two | png::BitDepth::Four | png::BitDepth::Eight
        )
    {
        return err("PNG must use an indexed 1/2/4/8-bit palette");
    }
    let (width, height) = (output.width, output.height);
    if width == 0 || height == 0 || !width.is_multiple_of(8) || !height.is_multiple_of(8) {
        return err("PNG dimensions must be nonzero multiples of eight");
    }
    let depth = output.bit_depth as u32;
    let raw_palette = info
        .palette
        .as_deref()
        .ok_or_else(|| AssetError("PNG lacks IHDR or PLTE".into()))?;
    if raw_palette.len() < 3 || raw_palette.len() > 768 || !raw_palette.len().is_multiple_of(3) {
        return err("invalid PLTE");
    }
    let palette: Vec<Rgb> = raw_palette
        .chunks_exact(3)
        .map(|rgb| [rgb[0], rgb[1], rgb[2]])
        .collect();
    if palette.len() as u32 > 1 << depth {
        return err("palette exceeds indexed bit depth");
    }
    let has_transparency = info
        .trns
        .as_deref()
        .is_some_and(|alpha| alpha.iter().any(|value| *value != 255));
    if let Some(alpha) = info.trns.as_deref() {
        if alpha.len() > palette.len() || alpha.iter().any(|value| *value != 0 && *value != 255) {
            return err("transparency must contain only binary alpha");
        }
    }
    let mut pixels = Vec::with_capacity(width as usize * height as usize);
    let mask = (1u32 << depth) - 1;
    let row_size = (width as usize * depth as usize).div_ceil(8);
    for row in bytes.chunks_exact(output.line_size).take(height as usize) {
        for x in 0..width as usize {
            let bit = x * depth as usize;
            let index = (u32::from(row[bit / 8]) >> (8 - depth - (bit % 8) as u32)) & mask;
            if index as usize >= palette.len() {
                return err("pixel references a missing palette entry");
            }
            pixels.push(index);
        }
    }
    if bytes.len() != row_size * height as usize {
        return err("unexpected decompressed PNG size");
    }
    Ok(IndexedImage {
        width,
        height,
        pixels,
        palette,
        has_transparency,
    })
}

#[derive(Debug)]
pub struct RgbaImage {
    pub width: u32,
    pub height: u32,
    pub pixels: Vec<u8>,
}

/// Read linear palette indices, allowing only zero padding after the payload.
pub fn indexed_bytes(data: &[u8], size: usize) -> Result<Vec<u8>, AssetError> {
    let image = indexed_png(data)?;
    if size > image.pixels.len() || image.pixels[size..].iter().any(|pixel| *pixel != 0) {
        return err("indexed PNG payload size or zero padding differs");
    }
    Ok(image.pixels[..size]
        .iter()
        .map(|pixel| *pixel as u8)
        .collect())
}

pub fn rgba_png(data: &[u8]) -> Result<RgbaImage, AssetError> {
    let (output, bytes, _) = decode(data)?;
    if output.color_type != png::ColorType::Rgba || output.bit_depth != png::BitDepth::Eight {
        return err("PNG must use 8-bit RGBA pixels");
    }
    if output.width == 0 || output.height == 0 {
        return err("PNG lacks a nonempty IHDR");
    }
    if output.line_size != output.width as usize * 4 || bytes.len() != output.buffer_size() {
        return err("unexpected decompressed PNG size");
    }
    Ok(RgbaImage {
        width: output.width,
        height: output.height,
        pixels: bytes,
    })
}

pub fn gba_palette_rgba(data: &[u8]) -> Result<(Vec<u8>, ImageSize), AssetError> {
    let image = rgba_png(data)?;
    let mut palette = Vec::with_capacity(image.pixels.len() / 2);
    for rgba in image.pixels.chunks_exact(4) {
        let (r, g, b, a) = (rgba[0], rgba[1], rgba[2], rgba[3]);
        if r & 7 != 0 || g & 7 != 0 || b & 7 != 0 {
            return err("RGBA palette colors must be multiples of eight");
        }
        if a != 254 && a != 255 {
            return err("RGBA palette alpha must be 254 or 255");
        }
        palette.extend_from_slice(
            &(u16::from(r >> 3)
                | u16::from(g >> 3) << 5
                | u16::from(b >> 3) << 10
                | u16::from(255 - a) << 15)
                .to_le_bytes(),
        );
    }
    Ok((
        palette,
        ImageSize {
            width: image.width,
            height: image.height,
        },
    ))
}

pub fn gba_graphics(data: &[u8], bpp: GbaBpp) -> Result<(Vec<u8>, Vec<u8>, ImageSize), AssetError> {
    let (tiles, palette, width, height) = gba_graphics_from_png(data, bpp, false)?;
    Ok((tiles, palette, ImageSize { width, height }))
}

/// Pack an indexed PNG into 8x8 tiles of one bit per pixel: each tile is
/// eight bytes, one row each from the top, with the leftmost pixel in the most
/// significant bit; tiles follow in row-major order.  Palette index 0 clears a
/// bit and index 1 sets it, whatever colours the palette holds.
pub fn one_bit_tiles(data: &[u8]) -> Result<(Vec<u8>, ImageSize), AssetError> {
    let image = indexed_png(data)?;
    if image.palette.len() > 2 || image.pixels.iter().any(|pixel| *pixel > 1) {
        return err("image does not fit 1bpp");
    }
    let width = image.width as usize;
    let mut tiles = Vec::with_capacity(width * image.height as usize / 8);
    for top in (0..image.height as usize).step_by(8) {
        for left in (0..width).step_by(8) {
            for y in 0..8 {
                let row = &image.pixels[(top + y) * width + left..][..8];
                tiles.push(row.iter().fold(0, |bits, pixel| bits << 1 | *pixel as u8));
            }
        }
    }
    Ok((
        tiles,
        ImageSize {
            width: image.width,
            height: image.height,
        },
    ))
}

fn encode_png(
    width: usize,
    height: usize,
    color: png::ColorType,
    depth: png::BitDepth,
    palette: Option<&[u8]>,
    pixels: &[u8],
) -> Result<Vec<u8>, AssetError> {
    let mut output = Vec::new();
    let mut encoder = png::Encoder::new(&mut output, width as u32, height as u32);
    encoder.set_color(color);
    encoder.set_depth(depth);
    if let Some(palette) = palette {
        encoder.set_palette(palette);
    }
    let mut writer = encoder.write_header().map_err(png_error)?;
    writer.write_image_data(pixels).map_err(png_error)?;
    writer.finish().map_err(png_error)?;
    Ok(output)
}

#[cfg(test)]
fn rgba_image(raw: &[u8], width: usize) -> Result<Vec<u8>, AssetError> {
    if raw.is_empty()
        || !raw.len().is_multiple_of(4)
        || width == 0
        || !(raw.len() / 4).is_multiple_of(width)
    {
        return err("RGBA input must contain whole nonempty pixels");
    }
    encode_png(
        width,
        raw.len() / width / 4,
        png::ColorType::Rgba,
        png::BitDepth::Eight,
        None,
        raw,
    )
}

#[test]
fn image_round_trips() -> Result<(), AssetError> {
    let mut indexed = Vec::new();
    let mut encoder = png::Encoder::new(&mut indexed, 8, 8);
    encoder.set_color(png::ColorType::Indexed);
    encoder.set_depth(png::BitDepth::One);
    encoder.set_palette(vec![0, 0, 0, 248, 0, 0]);
    encoder
        .write_header()
        .map_err(png_error)?
        .write_image_data(&[0x55; 8])
        .map_err(png_error)?;
    let (tiles, palette, _) = gba_graphics(&indexed, GbaBpp::Bpp4)?;
    if tiles != [0x10; 32] || palette != [0, 0, 0x1f, 0] {
        return err("GBA graphics self-test failed");
    }
    let mut rgba = vec![0; 8 * 8 * 4];
    rgba[4] = 248;
    let image = rgba_image(&rgba, 8)?;
    if rgba_png(&image)?.pixels != rgba {
        return err("RGBA self-test failed");
    }
    Ok(())
}

/// The GBA's packed, tile-major indexed-pixel formats.
#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum GbaBpp {
    Bpp4,
    Bpp8,
}

impl GbaBpp {
    fn tile_bytes(self) -> usize {
        match self {
            Self::Bpp4 => 32,
            Self::Bpp8 => 64,
        }
    }

    fn palette_entries(self) -> usize {
        match self {
            Self::Bpp4 => 16,
            Self::Bpp8 => 256,
        }
    }

    fn png_depth(self) -> png::BitDepth {
        match self {
            Self::Bpp4 => png::BitDepth::Four,
            Self::Bpp8 => png::BitDepth::Eight,
        }
    }

    fn name(self) -> &'static str {
        match self {
            Self::Bpp4 => "4bpp",
            Self::Bpp8 => "8bpp",
        }
    }
}

fn check_image(
    image: &IndexedImage,
    bpp: GbaBpp,
    reject_transparency: bool,
) -> Result<(), AssetError> {
    if reject_transparency && image.has_transparency {
        return err("GBA BGR555 palettes cannot represent transparent PNG entries");
    }
    if image.palette.len() > bpp.palette_entries()
        || image
            .pixels
            .iter()
            .any(|pixel| *pixel as usize >= bpp.palette_entries())
    {
        return err(format!("image does not fit {}", bpp.name()));
    }
    Ok(())
}

fn bgr555_palette(image: &IndexedImage) -> Result<Vec<u8>, AssetError> {
    let mut palette = Vec::with_capacity(image.palette.len() * 2);
    for [red, green, blue] in &image.palette {
        if red & 7 != 0 || green & 7 != 0 || blue & 7 != 0 {
            return err("palette channels must be exact five-bit values (multiples of 8)");
        }
        let value =
            u16::from(*red >> 3) | u16::from(*green >> 3) << 5 | u16::from(*blue >> 3) << 10;
        palette.extend_from_slice(&value.to_le_bytes());
    }
    Ok(palette)
}

fn tiles_from_image(image: &IndexedImage, bpp: GbaBpp) -> Vec<u8> {
    let width = image.width as usize;
    let mut tiles =
        Vec::with_capacity(width * image.height as usize / if bpp == GbaBpp::Bpp4 { 2 } else { 1 });
    for top in (0..image.height as usize).step_by(8) {
        for left in (0..width).step_by(8) {
            for y in 0..8 {
                for x in (0..8).step_by(if bpp == GbaBpp::Bpp4 { 2 } else { 1 }) {
                    let at = (top + y) * width + left + x;
                    if bpp == GbaBpp::Bpp4 {
                        tiles.push((image.pixels[at] | image.pixels[at + 1] << 4) as u8);
                    } else {
                        tiles.push(image.pixels[at] as u8);
                    }
                }
            }
        }
    }
    tiles
}

/// Shared encoder for the strict portable converters and the older asset builder.
/// The legacy builder permits its existing binary-alpha PNG inputs; the public
/// converter refuses them because BGR555 cannot retain that state.
fn gba_graphics_from_png(
    data: &[u8],
    bpp: GbaBpp,
    reject_transparency: bool,
) -> Result<(Vec<u8>, Vec<u8>, u32, u32), AssetError> {
    let image = indexed_png(data)?;
    check_image(&image, bpp, reject_transparency)?;
    let tiles = tiles_from_image(&image, bpp);
    let palette = bgr555_palette(&image)?;
    Ok((tiles, palette, image.width, image.height))
}

/// Convert an opaque indexed PNG to GBA tile-major 4bpp or 8bpp pixels.
pub fn gba_tiles_from_png(data: &[u8], bpp: GbaBpp) -> Result<Vec<u8>, AssetError> {
    gba_graphics_from_png(data, bpp, true).map(|(tiles, _, _, _)| tiles)
}

/// Convert an opaque indexed PNG palette to little-endian, 15-bit BGR555 words.
pub fn bgr555_palette_from_png(data: &[u8]) -> Result<Vec<u8>, AssetError> {
    let image = indexed_png(data)?;
    if image.has_transparency {
        return err("GBA BGR555 palettes cannot represent transparent PNG entries");
    }
    bgr555_palette(&image)
}

fn palette_rgb(palette: &[u8], bpp: GbaBpp) -> Result<Vec<u8>, AssetError> {
    if palette.is_empty() || !palette.len().is_multiple_of(2) {
        return err("BGR555 palette must contain a nonempty whole number of words");
    }
    if palette.len() / 2 > bpp.palette_entries() {
        return err(format!("palette exceeds {} entry limit", bpp.name()));
    }
    let mut rgb = Vec::with_capacity(palette.len() / 2 * 3);
    for word in palette.chunks_exact(2) {
        let value = u16::from_le_bytes([word[0], word[1]]);
        if value & 0x8000 != 0 {
            return err("BGR555 palette uses unsupported transparent bit 15");
        }
        rgb.extend_from_slice(&[
            ((value & 0x1f) << 3) as u8,
            (((value >> 5) & 0x1f) << 3) as u8,
            (((value >> 10) & 0x1f) << 3) as u8,
        ]);
    }
    Ok(rgb)
}

fn image_shape(
    tiles: &[u8],
    bpp: GbaBpp,
    tiles_wide: usize,
) -> Result<(usize, usize, usize), AssetError> {
    if tiles.is_empty() || !tiles.len().is_multiple_of(bpp.tile_bytes()) {
        return err("tile input must contain a nonempty whole number of GBA tiles");
    }
    if tiles_wide == 0 {
        return err("--tiles-wide must be greater than zero");
    }
    let count = tiles.len() / bpp.tile_bytes();
    if !count.is_multiple_of(tiles_wide) {
        return err("tile count must divide evenly into --tiles-wide rows");
    }
    let tiles_high = count / tiles_wide;
    let width = tiles_wide
        .checked_mul(8)
        .ok_or_else(|| AssetError("image width overflows platform bounds".into()))?;
    let height = tiles_high
        .checked_mul(8)
        .ok_or_else(|| AssetError("image height overflows platform bounds".into()))?;
    if width > u32::MAX as usize || height > u32::MAX as usize {
        return err("image dimensions exceed PNG bounds");
    }
    Ok((tiles_high, width, height))
}

/// Convert GBA tile-major 4bpp or 8bpp pixels and a LE BGR555 palette to PNG.
pub fn png_from_gba_tiles(
    tiles: &[u8],
    palette: &[u8],
    bpp: GbaBpp,
    tiles_wide: usize,
) -> Result<Vec<u8>, AssetError> {
    let rgb = palette_rgb(palette, bpp)?;
    let entries = rgb.len() / 3;
    let (tiles_high, width, height) = image_shape(tiles, bpp, tiles_wide)?;
    let mut pixels = vec![0u8; width * height];
    for tile_y in 0..tiles_high {
        for tile_x in 0..tiles_wide {
            let offset = (tile_y * tiles_wide + tile_x) * bpp.tile_bytes();
            for y in 0..8 {
                for x in 0..8 {
                    let index = match bpp {
                        GbaBpp::Bpp4 => {
                            let byte = tiles[offset + y * 4 + x / 2];
                            if x.is_multiple_of(2) {
                                byte & 0x0f
                            } else {
                                byte >> 4
                            }
                        }
                        GbaBpp::Bpp8 => tiles[offset + y * 8 + x],
                    };
                    if index as usize >= entries {
                        return err("tile references an index outside the BGR555 palette");
                    }
                    pixels[(tile_y * 8 + y) * width + tile_x * 8 + x] = index;
                }
            }
        }
    }
    let encoded = if bpp == GbaBpp::Bpp4 {
        let mut packed = Vec::with_capacity(pixels.len() / 2);
        for pair in pixels.chunks_exact(2) {
            packed.push(pair[0] << 4 | pair[1]);
        }
        packed
    } else {
        pixels
    };
    encode_png(
        width,
        height,
        png::ColorType::Indexed,
        bpp.png_depth(),
        Some(&rgb),
        &encoded,
    )
}

#[cfg(test)]
mod tile_tests {
    use super::*;

    fn fixture_png(bpp: GbaBpp, width: usize, height: usize) -> Vec<u8> {
        let entries = bpp.palette_entries();
        let palette: Vec<u8> = (0..entries)
            .flat_map(|index| {
                [
                    ((index & 31) as u8) << 3,
                    (((index >> 3) & 31) as u8) << 3,
                    (((index >> 5) & 31) as u8) << 3,
                ]
            })
            .collect();
        let pixels: Vec<u8> = (0..width * height)
            .map(|index| (index % entries) as u8)
            .collect();
        let source = if bpp == GbaBpp::Bpp4 {
            pixels
                .chunks_exact(2)
                .map(|pair| pair[0] << 4 | pair[1])
                .collect()
        } else {
            pixels
        };
        encode_png(
            width,
            height,
            png::ColorType::Indexed,
            bpp.png_depth(),
            Some(&palette),
            &source,
        )
        .unwrap()
    }

    #[test]
    fn gba_tile_and_palette_round_trips_are_lossless() {
        for (bpp, width, height) in [(GbaBpp::Bpp4, 16, 16), (GbaBpp::Bpp8, 8, 16)] {
            let source = fixture_png(bpp, width, height);
            let tiles = gba_tiles_from_png(&source, bpp).unwrap();
            let palette = bgr555_palette_from_png(&source).unwrap();
            let output = png_from_gba_tiles(&tiles, &palette, bpp, width / 8).unwrap();
            assert_eq!(gba_tiles_from_png(&output, bpp).unwrap(), tiles);
            assert_eq!(bgr555_palette_from_png(&output).unwrap(), palette);
        }
    }

    #[test]
    fn gba_tile_decoder_rejects_malformed_inputs() {
        let palette = [0, 0];
        assert!(png_from_gba_tiles(&[0; 31], &palette, GbaBpp::Bpp4, 1).is_err());
        assert!(png_from_gba_tiles(&[0; 32], &palette, GbaBpp::Bpp4, 0).is_err());
        assert!(png_from_gba_tiles(&[1; 32], &palette, GbaBpp::Bpp4, 1).is_err());
        assert!(png_from_gba_tiles(&[0; 32], &[0], GbaBpp::Bpp4, 1).is_err());
        assert!(png_from_gba_tiles(&[0; 32], &[0, 0x80], GbaBpp::Bpp4, 1).is_err());
        assert!(png_from_gba_tiles(&[0; 32], &palette, GbaBpp::Bpp4, 2).is_err());
    }

    #[test]
    fn portable_converters_reject_png_transparency() {
        let mut source = Vec::new();
        let mut encoder = png::Encoder::new(&mut source, 8, 8);
        encoder.set_color(png::ColorType::Indexed);
        encoder.set_depth(png::BitDepth::One);
        encoder.set_palette(&[0, 0, 0, 248, 0, 0]);
        encoder.set_trns(&[0, 255]);
        encoder
            .write_header()
            .unwrap()
            .write_image_data(&[0; 8])
            .unwrap();
        assert!(indexed_png(&source).unwrap().has_transparency);
        assert!(gba_tiles_from_png(&source, GbaBpp::Bpp4).is_err());
        assert!(bgr555_palette_from_png(&source).is_err());
        assert!(gba_graphics(&source, GbaBpp::Bpp4).is_ok());
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    fn indexed(depth: png::BitDepth) -> Vec<u8> {
        let mut out = Vec::new();
        let mut encoder = png::Encoder::new(&mut out, 8, 8);
        encoder.set_color(png::ColorType::Indexed);
        encoder.set_depth(depth);
        encoder.set_palette(vec![0, 0, 0, 248, 0, 0]);
        let data = vec![0; (8 * depth as usize).div_ceil(8) * 8];
        encoder
            .write_header()
            .unwrap()
            .write_image_data(&data)
            .unwrap();
        out
    }

    #[test]
    fn png_crate_preserves_all_indexed_depths() {
        for depth in [
            png::BitDepth::One,
            png::BitDepth::Two,
            png::BitDepth::Four,
            png::BitDepth::Eight,
        ] {
            assert_eq!(indexed_png(&indexed(depth)).unwrap().pixels.len(), 64);
        }
    }

    #[test]
    fn one_bit_tiles_pack_rows_most_significant_bit_first() {
        let mut out = Vec::new();
        let mut encoder = png::Encoder::new(&mut out, 16, 8);
        encoder.set_color(png::ColorType::Indexed);
        encoder.set_depth(png::BitDepth::One);
        encoder.set_palette(vec![0, 0, 0, 255, 255, 255]);
        let rows: Vec<u8> = (0..8).flat_map(|y| [0x80 >> y, 0x01 << y]).collect();
        encoder
            .write_header()
            .unwrap()
            .write_image_data(&rows)
            .unwrap();
        let (tiles, size) = one_bit_tiles(&out).unwrap();
        assert_eq!(tiles[..8], [0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01]);
        assert_eq!(tiles[8..], [0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80]);
        assert_eq!(
            size,
            ImageSize {
                width: 16,
                height: 8
            }
        );
        let mut wide = Vec::new();
        let mut encoder = png::Encoder::new(&mut wide, 8, 8);
        encoder.set_color(png::ColorType::Indexed);
        encoder.set_depth(png::BitDepth::Two);
        encoder.set_palette(vec![0, 0, 0, 8, 8, 8, 16, 16, 16]);
        encoder
            .write_header()
            .unwrap()
            .write_image_data(&[0xaa; 16])
            .unwrap();
        assert!(one_bit_tiles(&wide).is_err());
    }

    #[test]
    fn indexed_payload_requires_zero_padding_and_complete_input() {
        let image = indexed(png::BitDepth::Eight);
        assert_eq!(indexed_bytes(&image, 17).unwrap(), vec![0; 17]);
        assert_eq!(indexed_bytes(&image, 64).unwrap(), vec![0; 64]);
        assert!(indexed_bytes(&image, 65).is_err());
        let mut pixels = vec![0; 64];
        pixels[63] = 1;
        let image = encode_png(
            8,
            8,
            png::ColorType::Indexed,
            png::BitDepth::Eight,
            Some(&[0, 0, 0, 248, 0, 0]),
            &pixels,
        )
        .unwrap();
        assert!(indexed_bytes(&image, 63).is_err());
        assert_eq!(indexed_bytes(&image, 64).unwrap()[63], 1);
    }
}
