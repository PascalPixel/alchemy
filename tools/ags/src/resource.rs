//! Resource directory files built from editable inputs. A built file's name
//! is its recipe, as in pret's graphics rules: the stem names the input, the
//! first extension its form, and an optional second extension the resource
//! packer's codec. Image forms read the indexed image `STEM.PNG`:
//!
//! - `.gbapal`: the image's palette as little-endian BGR555.
//! - `.bitmap`: the pixels row by row, one palette index per byte.
//! - `.4bpp`, `.8bpp`: the pixels as GBA tiles, row-major.
//!
//! Data forms read the identified table or tilemap `STEM.BIN`:
//!
//! - `.bin`: the table's bytes as they are.
//! - `.delta0`, `.delta1`, `.delta2`: the tilemap's 16-bit entries
//!   delta-coded in that mode (Psynergy's tilemap delta).
//!
//! Either may then be packed:
//!
//! - `.lz`: the packer's LZ, the smaller of its general (tag 0) and
//!   palette (tag 1) encodings, the palette one on ties.
//! - or `.mtf`: the tag-2 tile compressor.
use crate::graphics::{indices, metatiles};
use crate::lz::{compress_mtf4, compress_tagged, LzMachine};
use psynergy::assets::compression::encode_tilemap_delta;
use psynergy::assets::image::{bgr555_palette_of, indexed_bitmap_png, GbaBpp};

/// The resource packer's compressor, as the streams in both games show: the
/// general ring's window, read-ahead and reach, and the palette ring's
/// read-ahead. It packed every code overlay and every LZ data resource.
pub const PACKER: LzMachine = LzMachine::new(4123, 485, 4126, 272);

/// Whether a form reads a table or tilemap rather than an image.
fn data_form(form: &str) -> bool {
    matches!(form, "bin" | "delta0" | "delta1" | "delta2")
}

/// The input a built file name reads, relative to the same directory: the
/// name up to its first extension, then `.BIN` for a data form or `.PNG`.
pub fn input_name(built: &str) -> Result<String, String> {
    let (directory, file) = built.rsplit_once('/').unwrap_or(("", built));
    let (stem, rest) = file
        .split_once('.')
        .ok_or_else(|| format!("{built} names no form"))?;
    let form = rest.split('.').next().unwrap_or_default();
    let extension = if data_form(form) { "BIN" } else { "PNG" };
    Ok(if directory.is_empty() {
        format!("{stem}.{extension}")
    } else {
        format!("{directory}/{stem}.{extension}")
    })
}

/// Build the file `built` names from the bytes of its input.
pub fn build_file(built: &str, input: &[u8]) -> Result<Vec<u8>, String> {
    let file = built.rsplit('/').next().unwrap_or(built);
    let extensions: Vec<&str> = file.split('.').skip(1).collect();
    let (form, codec) = match extensions.as_slice() {
        [form] => (*form, None),
        [form, codec] => (*form, Some(*codec)),
        _ => return Err(format!("{built}: expected STEM.FORM or STEM.FORM.CODEC")),
    };
    let pixels = if data_form(form) {
        match form {
            "bin" => input.to_vec(),
            _ => encode_tilemap_delta(input, form.as_bytes()[5] - b'0')
                .map_err(|error| format!("{built}: {}", error.0))?,
        }
    } else {
        image_form(built, form, input)?
    };
    match codec {
        None => Ok(pixels),
        Some("lz") => compress_tagged(&pixels, &PACKER),
        Some("mtf") => compress_mtf4(&pixels),
        Some(other) => Err(format!("{built}: unknown codec .{other}")),
    }
}

fn image_form(built: &str, form: &str, png: &[u8]) -> Result<Vec<u8>, String> {
    let image = indexed_bitmap_png(png).map_err(|error| format!("{built}: {}", error.0))?;
    let (width, height) = (image.width as usize, image.height as usize);
    Ok(match form {
        "gbapal" => bgr555_palette_of(&image).map_err(|error| format!("{built}: {}", error.0))?,
        "bitmap" => indices(&image),
        "4bpp" => metatiles(&indices(&image), width, height, GbaBpp::Bpp4, 1, 1)?,
        "8bpp" => metatiles(&indices(&image), width, height, GbaBpp::Bpp8, 1, 1)?,
        other => return Err(format!("{built}: unknown form .{other}")),
    })
}

#[cfg(test)]
mod tests {
    use super::*;
    use psynergy::assets::image::{png_from_bitmap, png_from_gba_tiles};

    #[test]
    fn names_read_their_image_and_follow_their_recipe() {
        assert_eq!(
            input_name("GRAPHICS/FX/STAR.bitmap.lz").unwrap(),
            "GRAPHICS/FX/STAR.PNG"
        );
        assert!(input_name("GRAPHICS/FX/STAR").is_err());
        assert_eq!(
            input_name("MAP/M/CELLS.delta1.lz").unwrap(),
            "MAP/M/CELLS.BIN"
        );
        assert_eq!(build_file("T.bin", &[1, 2, 3]).unwrap(), [1, 2, 3]);
        assert_eq!(
            build_file("T.delta2", &[1, 0, 3, 0]).unwrap(),
            [2, 1, 0, 2, 0]
        );
        assert_eq!(build_file("T.delta1.lz", &[1, 0]).unwrap()[0] <= 1, true);
        assert!(build_file("T.delta3", &[1, 0]).is_err());
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
        // A bitmap may be any size; tiles need whole tiles.
        let odd = png_from_bitmap(&[3; 60], &palette, 12).unwrap();
        assert_eq!(build_file("A.bitmap", &odd).unwrap().len(), 60);
        assert!(build_file("A.4bpp", &odd).is_err());
    }
}
