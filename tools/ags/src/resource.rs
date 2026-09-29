//! Resource directory files built from editable images. A built file's name
//! is its recipe, as in pret's graphics rules: the stem names the image
//! `STEM.PNG`, the first extension its pixel form, and an optional second
//! extension the resource packer's codec.
//!
//! - `.gbapal`: the image's palette as little-endian BGR555.
//! - `.bitmap`: the pixels row by row, one palette index per byte.
//! - `.4bpp`, `.8bpp`: the pixels as GBA tiles, row-major.
//! - then `.lz`: the packer's LZ, the smaller of its general (tag 0) and
//!   palette (tag 1) encodings, the palette one on ties.
//! - or `.mtf`: the tag-2 tile compressor.
use crate::graphics::{indices, metatiles};
use crate::lz::{compress_mtf4, compress_tagged, LzMachine};
use psynergy::assets::image::{bgr555_palette_from_png, indexed_png, GbaBpp};

/// The resource packer's compressor, as the streams in both games show: the
/// general ring's window, read-ahead and reach, and the palette ring's
/// read-ahead. It packed every code overlay and every LZ data resource.
pub const PACKER: LzMachine = LzMachine::new(4123, 485, 4126, 272);

/// The image a built file name reads, relative to the same directory: the
/// name up to its first extension, then `.PNG`.
pub fn image_name(built: &str) -> Result<String, String> {
    let (directory, file) = built.rsplit_once('/').unwrap_or(("", built));
    let (stem, _) = file
        .split_once('.')
        .ok_or_else(|| format!("{built} names no pixel form"))?;
    Ok(if directory.is_empty() {
        format!("{stem}.PNG")
    } else {
        format!("{directory}/{stem}.PNG")
    })
}

/// Build the file `built` names from the bytes of its PNG.
pub fn build_file(built: &str, png: &[u8]) -> Result<Vec<u8>, String> {
    let file = built.rsplit('/').next().unwrap_or(built);
    let extensions: Vec<&str> = file.split('.').skip(1).collect();
    let (form, codec) = match extensions.as_slice() {
        [form] => (*form, None),
        [form, codec] => (*form, Some(*codec)),
        _ => return Err(format!("{built}: expected STEM.FORM or STEM.FORM.CODEC")),
    };
    let image = indexed_png(png).map_err(|error| format!("{built}: {}", error.0))?;
    let (width, height) = (image.width as usize, image.height as usize);
    let pixels = match form {
        "gbapal" => {
            bgr555_palette_from_png(png).map_err(|error| format!("{built}: {}", error.0))?
        }
        "bitmap" => indices(&image),
        "4bpp" => metatiles(&indices(&image), width, height, GbaBpp::Bpp4, 1, 1)?,
        "8bpp" => metatiles(&indices(&image), width, height, GbaBpp::Bpp8, 1, 1)?,
        other => return Err(format!("{built}: unknown pixel form .{other}")),
    };
    match codec {
        None => Ok(pixels),
        Some("lz") => compress_tagged(&pixels, &PACKER),
        Some("mtf") => compress_mtf4(&pixels),
        Some(other) => Err(format!("{built}: unknown codec .{other}")),
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use psynergy::assets::image::png_from_gba_tiles;

    #[test]
    fn names_read_their_image_and_follow_their_recipe() {
        assert_eq!(
            image_name("GRAPHICS/FX/STAR.bitmap.lz").unwrap(),
            "GRAPHICS/FX/STAR.PNG"
        );
        assert!(image_name("GRAPHICS/FX/STAR").is_err());
        let palette: Vec<u8> = (0..16u16)
            .flat_map(|color| (color * 0x421).to_le_bytes())
            .collect();
        let tiles: Vec<u8> = (0..64).map(|index| (index % 16) as u8 * 0x11).collect();
        let png = png_from_gba_tiles(&tiles, &palette, GbaBpp::Bpp4, 2).unwrap();
        assert_eq!(build_file("A.gbapal", &png).unwrap(), palette);
        assert_eq!(build_file("A.4bpp", &png).unwrap(), tiles);
        assert_eq!(build_file("A.bitmap", &png).unwrap().len(), 16 * 8);
        let packed = build_file("A.4bpp.lz", &png).unwrap();
        assert!(packed[0] <= 1);
        assert_eq!(build_file("A.4bpp.mtf", &png).unwrap()[0], 2);
        assert!(build_file("A.4bpp.zip", &png).is_err());
    }
}
