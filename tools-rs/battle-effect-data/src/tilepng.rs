// `tile_png` from tools/lib/export_asset.ts, and nothing else from that file.
//
// Why this exists: `export_battle_effect_data` writes six indexed PNGs, and the
// bytes of those PNGs are compared against the tracked ones. The whole encoder
// is thirty lines plus a deflate call, and the deflate is already ported in
// `alchemy-zlib`, so copying the one function is cheaper and far less
// entangled than pulling a whole `export-asset` crate that does not yet exist.

use alchemy_zlib::{deflate_sync, DeflateOptions};
use import_asset::{make_chunk, AssetError, Report, Rgb, PNG_SIGNATURE};

fn err<T>(message: &str) -> Result<T, AssetError> {
    Err(AssetError(message.to_string()))
}

fn ihdr(width: u32, height: u32, depth: u8, color: u8) -> Vec<u8> {
    let mut value = Vec::with_capacity(13);
    value.extend_from_slice(&width.to_be_bytes());
    value.extend_from_slice(&height.to_be_bytes());
    value.extend_from_slice(&[depth, color, 0, 0, 0]);
    value
}

/// PORT NOTE: the TypeScript takes `bpp` and `columns` as JS numbers and only
/// ever tests `bpp === 4`. This port keeps `bpp` as an `f64` so the "not four
/// means eight" behaviour survives verbatim, but takes `columns` as a `usize`:
/// the `columns <= 0` guard below is exactly what a non-positive JS value would
/// hit, and every caller in this crate passes a positive integer literal.
pub fn tile_png(
    raw: &[u8],
    bpp: f64,
    columns: usize,
    palette_colors: Option<&[Rgb]>,
) -> Result<(Vec<u8>, Report), AssetError> {
    let four = bpp == 4.0;
    let unit = if four { 32 } else { 64 };
    if raw.is_empty() || !raw.len().is_multiple_of(unit) {
        return err("tile data must contain whole nonempty tiles");
    }
    let count = raw.len() / unit;
    if columns == 0 || !count.is_multiple_of(columns) {
        return err("columns must divide the tile count exactly");
    }
    let rows = count / columns;
    let width = columns * 8;
    let height = rows * 8;
    let mut pixels = vec![0u8; width * height];
    for tile in 0..count {
        let source = &raw[tile * unit..(tile + 1) * unit];
        let indices: Vec<u8> = if four {
            source.iter().flat_map(|byte| [byte & 15, byte >> 4]).collect()
        } else {
            source.to_vec()
        };
        let left = (tile % columns) * 8;
        let top = (tile / columns) * 8;
        for y in 0..8 {
            let target = (top + y) * width + left;
            pixels[target..target + 8].copy_from_slice(&indices[y * 8..y * 8 + 8]);
        }
    }
    let mut scanlines: Vec<u8> = Vec::with_capacity(height * (1 + width));
    for y in 0..height {
        scanlines.push(0);
        let row = &pixels[y * width..(y + 1) * width];
        if four {
            for index in (0..width).step_by(2) {
                scanlines.push((row[index] << 4) | row[index + 1]);
            }
        } else {
            scanlines.extend_from_slice(row);
        }
    }
    let colors = if four { 16usize } else { 256 };
    let default_palette: Vec<Rgb> = if four {
        (0..16u8).map(|index| [index * 16, index * 16, index * 16]).collect()
    } else {
        (0..256u32)
            .map(|index| [((index & 31) * 8) as u8, ((index >> 5) * 8) as u8, 0])
            .collect()
    };
    let palette: &[Rgb] = palette_colors.unwrap_or(&default_palette);
    if palette.is_empty() || palette.len() > colors {
        return err("palette does not fit the requested tile depth");
    }
    // `pixels` is never empty here, so `Math.max(...pixels)` always has a value.
    if usize::from(*pixels.iter().max().unwrap()) >= palette.len() {
        return err("tile pixels reference a missing palette entry");
    }
    let palette_bytes: Vec<u8> = palette.iter().flatten().copied().collect();
    let mut image = Vec::from(PNG_SIGNATURE);
    image.extend_from_slice(&make_chunk(
        b"IHDR",
        &ihdr(width as u32, height as u32, bpp as u8, 3),
    ));
    image.extend_from_slice(&make_chunk(b"PLTE", &palette_bytes));
    image.extend_from_slice(&make_chunk(
        b"IDAT",
        &deflate_sync(&scanlines, DeflateOptions { level: Some(9) }),
    ));
    image.extend_from_slice(&make_chunk(b"IEND", &[]));
    let mut report = Report::default();
    report.set("width", width as f64);
    report.set("height", height as f64);
    report.set("bpp", bpp);
    report.set("tiles", count as f64);
    report.set("columns", columns as f64);
    Ok((image, report))
}

#[cfg(test)]
mod tests {
    use super::*;
    use import_asset::gba_graphics;

    #[test]
    fn tiles_round_trip_through_the_png_reader() {
        for (bpp, size, columns) in [(4.0f64, 32 * 7usize, 7usize), (8.0, 64 * 4, 4)] {
            let raw: Vec<u8> = (0..size).map(|index| ((index * 37 + 11) & 255) as u8).collect();
            let (image, report) = tile_png(&raw, bpp, columns, None).unwrap();
            assert_eq!(gba_graphics(&image, bpp).unwrap().0, raw, "{bpp}bpp round trip");
            assert_eq!(report.get("columns"), Some(columns as f64));
        }
    }

    #[test]
    fn ragged_input_is_refused_the_way_the_typescript_refuses_it() {
        assert_eq!(
            tile_png(&[], 4.0, 1, None).unwrap_err().0,
            "tile data must contain whole nonempty tiles"
        );
        assert_eq!(
            tile_png(&[0u8; 31], 4.0, 1, None).unwrap_err().0,
            "tile data must contain whole nonempty tiles"
        );
        assert_eq!(
            tile_png(&[0u8; 96], 4.0, 5, None).unwrap_err().0,
            "columns must divide the tile count exactly"
        );
    }

    #[test]
    fn a_pixel_outside_the_supplied_palette_is_refused() {
        let mut raw = vec![0u8; 32];
        raw[0] = 0x30;
        let palette: Vec<Rgb> = vec![[0, 0, 0], [8, 8, 8]];
        assert_eq!(
            tile_png(&raw, 4.0, 1, Some(&palette)).unwrap_err().0,
            "tile pixels reference a missing palette entry"
        );
    }
}
