//! Portable GBA tile and palette container conversions.
use crate::{encode_png, err, indexed_png, AssetError};

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
    image: &crate::IndexedImage,
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

fn bgr555_palette(image: &crate::IndexedImage) -> Result<Vec<u8>, AssetError> {
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

fn tiles_from_image(image: &crate::IndexedImage, bpp: GbaBpp) -> Vec<u8> {
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
pub(crate) fn gba_graphics_from_png(
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
mod tests {
    use super::*;

    fn indexed_png(bpp: GbaBpp, width: usize, height: usize) -> Vec<u8> {
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
            let source = indexed_png(bpp, width, height);
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
        assert!(crate::indexed_png(&source).unwrap().has_transparency);
        assert!(gba_tiles_from_png(&source, GbaBpp::Bpp4).is_err());
        assert!(bgr555_palette_from_png(&source).is_err());
        assert!(crate::gba_graphics(&source, 4.0).is_ok());
    }
}
