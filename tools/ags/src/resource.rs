//! Resource directory files built from editable inputs. A built file's name
//! is its recipe, as in pret's graphics rules: the stem names the input, the
//! first extension its form, and an optional second extension the resource
//! packer's codec. Image forms read the indexed image `STEM.PNG`:
//!
//! - `.gbapal`: the image's palette as little-endian BGR555.
//! - `.bitmap`: the pixels row by row, one palette index per byte.
//! - `.4bpp`, `.8bpp`: the pixels as GBA tiles, row-major.
//! - `.icons`: an icon bank. The image is one 32x32 icon wide with its icons
//!   stacked. The bank is a table of each icon's halfword offset, then each
//!   icon as 8-bit tiles in the packer's palette LZ without its tag,
//!   its stream zero-padded to a multiple of 32 bytes.
//! - `.frames`: a sprite bank. The image is one square frame wide with its
//!   frames stacked, a front and a back pose in turn. The bank is a table of
//!   each frame's offset in that order, ending 0, then the back frames and
//!   then the front frames, each zero-skip coded.
//!
//! Data forms read the identified table or tilemap `STEM.BIN`:
//!
//! - `.bin`: the table's bytes as they are.
//! - `.delta0`, `.delta1`, `.delta2`: the tilemap's 16-bit entries
//!   delta-coded in that mode (Psynergy's tilemap delta).
//!
//! A table reads its text `STEM.TSV`:
//!
//! - `.table`: records of little-endian fields. The first line names each
//!   column and its type, such as `x:s16\ty:s16` (`u8`, `s8`, `u16`, `s16`,
//!   `u32` or `s32`); each further line is a record of decimal or `0x` hex
//!   values in those columns. Only the last record may stop short.
//!
//! A font reads two inputs, `STEM.PNG` and its table `STEM.TSV`:
//!
//! - `.font`: one record per glyph, cut from the sheet's 16x16 cells in
//!   code order: the table's halfwords for that glyph, the advance width
//!   first, then its rows of sixteen pixels as little-endian halfwords, the
//!   leftmost pixel highest. The
//!   table names `first` (the first code), `top` (the cell row of the
//!   record's first row) and `rows`, then one line per glyph: its code in
//!   hex and its halfwords.
//!
//! A picture made of several reads its part list `STEM.TSV`:
//!
//! - `.parts`: each line names a part image beside the list and that part's
//!   form, such as `BLUE_FLAME_COLUMN\tbitmap`, or `table` for a table beside
//!   it; the parts are built in turn and joined.
//!
//! Either may then be packed:
//!
//! - `.lz`: the packer's LZ, the smaller of its general (tag 0) and
//!   palette (tag 1) encodings, the palette one on ties.
//! - or `.plz`: the packer's palette LZ without its tag byte.
//! - or `.mtf`: the tag-2 tile compressor.
//! - or `.d7`: the backdrop codec, each 7-bit pixel as a delta from the one
//!   before it.
use crate::graphics::{indices, metatiles};
use crate::lz::{compress_mtf4, compress_palette, compress_tagged, LzMachine};
use psynergy::assets::compression::{encode_delta7, encode_tilemap_delta, encode_zero_skip};
use psynergy::assets::image::{bgr555_palette_of, indexed_bitmap_png, GbaBpp};

/// The resource packer's compressor, as the streams in both games show: the
/// general ring's window, read-ahead and reach, and the palette ring's
/// read-ahead. It packed every code overlay and every LZ data resource.
pub const PACKER: LzMachine = LzMachine::new(4123, 485, 4126, 272);

/// Whether a form reads a table or tilemap rather than an image.
fn data_form(form: &str) -> bool {
    matches!(
        form,
        "bin" | "delta0" | "delta1" | "delta2" | "parts" | "table"
    )
}

/// The input a built file name reads, relative to the same directory: the
/// name up to its first extension, then `.BIN` for a data form or `.PNG`.
pub fn input_name(built: &str) -> Result<String, String> {
    let (directory, file) = built.rsplit_once('/').unwrap_or(("", built));
    let (stem, rest) = file
        .split_once('.')
        .ok_or_else(|| format!("{built} names no form"))?;
    let form = rest.split('.').next().unwrap_or_default();
    let extension = match form {
        "parts" | "table" => "TSV",
        form if data_form(form) => "BIN",
        _ => "PNG",
    };
    Ok(if directory.is_empty() {
        format!("{stem}.{extension}")
    } else {
        format!("{directory}/{stem}.{extension}")
    })
}

/// Build the file `built` names from the bytes of its input.
pub fn build_file(built: &str, input: &[u8]) -> Result<Vec<u8>, String> {
    build_file_with(built, input, &|name| Err(format!("{built} needs {name}")))
}

/// Build the file `built` names from its input, reading any second input,
/// such as a font's table, by its file name beside the first.
pub fn build_file_with(
    built: &str,
    input: &[u8],
    sibling: &dyn Fn(&str) -> Result<Vec<u8>, String>,
) -> Result<Vec<u8>, String> {
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
            "parts" => parts(built, input, sibling)?,
            "table" => table(built, input)?,
            _ => encode_tilemap_delta(input, form.as_bytes()[5] - b'0')
                .map_err(|error| format!("{built}: {}", error.0))?,
        }
    } else if form == "font" {
        let stem = input_name(built)?;
        let stem = stem.rsplit('/').next().unwrap_or(&stem);
        let table = sibling(&stem.replace(".PNG", ".TSV"))?;
        font(built, input, &table)?
    } else {
        image_form(built, form, input)?
    };
    match codec {
        None => Ok(pixels),
        Some("lz") => compress_tagged(&pixels, &PACKER),
        Some("plz") => compress_palette(&pixels, &PACKER),
        Some("mtf") => compress_mtf4(&pixels),
        Some("d7") => encode_delta7(&pixels).map_err(|error| format!("{built}: {}", error.0)),
        Some(other) => Err(format!("{built}: unknown codec .{other}")),
    }
}

/// The joined parts a part list names, each built from its image beside it.
fn parts(
    built: &str,
    list: &[u8],
    sibling: &dyn Fn(&str) -> Result<Vec<u8>, String>,
) -> Result<Vec<u8>, String> {
    let text = std::str::from_utf8(list).map_err(|_| format!("{built}: part list is not text"))?;
    let mut output = Vec::new();
    for line in text.lines().filter(|line| !line.trim().is_empty()) {
        let (part, form) = line
            .split_once('\t')
            .ok_or_else(|| format!("{built}: {line:?} needs a part and its form"))?;
        if (data_form(form) && form != "table") || form == "font" {
            return Err(format!(
                "{built}: part {part} must be an image form or a table"
            ));
        }
        let name = format!("{part}.{form}");
        output.extend(build_file(&name, &sibling(&input_name(&name)?)?)?);
    }
    Ok(output)
}

/// A table's records, each field little-endian in its column's type.
fn table(built: &str, text: &[u8]) -> Result<Vec<u8>, String> {
    let text = std::str::from_utf8(text).map_err(|_| format!("{built}: table is not text"))?;
    let mut lines = text.lines().filter(|line| !line.trim().is_empty());
    let columns = lines
        .next()
        .ok_or_else(|| format!("{built}: table names no columns"))?
        .split('\t')
        .map(
            |column| match column.rsplit_once(':').map(|(_, kind)| kind) {
                Some("u8") => Ok((1, false)),
                Some("s8") => Ok((1, true)),
                Some("u16") => Ok((2, false)),
                Some("s16") => Ok((2, true)),
                Some("u32") => Ok((4, false)),
                Some("s32") => Ok((4, true)),
                _ => Err(format!("{built}: column {column:?} needs NAME:TYPE")),
            },
        )
        .collect::<Result<Vec<(usize, bool)>, _>>()?;
    let mut output = Vec::new();
    let mut short = false;
    for line in lines {
        let fields: Vec<&str> = line.split('\t').collect();
        if short || fields.len() > columns.len() {
            return Err(format!("{built}: record {line:?} does not fit the columns"));
        }
        short = fields.len() < columns.len();
        for (field, &(size, signed)) in fields.iter().zip(&columns) {
            let field = field.trim();
            let (negative, digits) = match field.strip_prefix('-') {
                Some(digits) => (true, digits),
                None => (false, field),
            };
            let magnitude = match digits.strip_prefix("0x") {
                Some(hex) => i64::from_str_radix(hex, 16),
                None => digits.parse::<i64>(),
            }
            .map_err(|_| format!("{built}: {field:?} is not a number"))?;
            let value = if negative { -magnitude } else { magnitude };
            let bits = 8 * size as u32;
            let (low, high) = if signed {
                (-(1i64 << (bits - 1)), (1i64 << (bits - 1)) - 1)
            } else {
                (0, (1i64 << bits) - 1)
            };
            if value < low || value > high {
                return Err(format!("{built}: {field} does not fit its column"));
            }
            output.extend(&value.to_le_bytes()[..size]);
        }
    }
    Ok(output)
}

/// A font's glyph records from its sheet and table.
fn font(built: &str, png: &[u8], table: &[u8]) -> Result<Vec<u8>, String> {
    let image = indexed_bitmap_png(png).map_err(|error| format!("{built}: {}", error.0))?;
    let text = std::str::from_utf8(table).map_err(|_| format!("{built}: table is not text"))?;
    let (mut first, mut top, mut rows) = (None, None, None);
    let mut glyphs = Vec::new();
    for line in text.lines().filter(|line| !line.trim().is_empty()) {
        let fields: Vec<&str> = line.split('\t').collect();
        let hex = |field: &str| {
            u32::from_str_radix(field.trim(), 16)
                .map_err(|_| format!("{built}: {field} is not hex"))
        };
        match fields[0] {
            "first" => first = Some(hex(fields.get(1).copied().unwrap_or(""))? as usize),
            "top" => top = Some(hex(fields.get(1).copied().unwrap_or(""))? as usize),
            "rows" => rows = Some(hex(fields.get(1).copied().unwrap_or(""))? as usize),
            code => glyphs.push((
                hex(code)? as usize,
                fields[1..]
                    .iter()
                    .map(|field| hex(field))
                    .collect::<Result<Vec<_>, _>>()?,
            )),
        }
    }
    let (first, top, rows) = match (first, top, rows) {
        (Some(first), Some(top), Some(rows)) if top + rows <= 16 => (first, top, rows),
        _ => {
            return Err(format!(
                "{built}: the table needs first, top and rows within a cell"
            ))
        }
    };
    let (width, columns) = (image.width as usize, image.width as usize / 16);
    let mut output = Vec::new();
    for (index, (code, words)) in glyphs.iter().enumerate() {
        if *code != first + index {
            return Err(format!("{built}: glyph {code:x} is out of order"));
        }
        let (cell_x, cell_y) = (index % columns * 16, index / columns * 16);
        if cell_y + 16 > image.height as usize {
            return Err(format!("{built}: glyph {code:x} lies outside the sheet"));
        }
        for word in words {
            let word = u16::try_from(*word)
                .map_err(|_| format!("{built}: {word:x} exceeds a halfword"))?;
            output.extend(word.to_le_bytes());
        }
        for y in 0..rows {
            let row = (0..16).fold(0u16, |row, x| {
                let pixel = image.pixels[(cell_y + top + y) * width + cell_x + x];
                row | u16::from(pixel != 0) << (15 - x)
            });
            output.extend(row.to_le_bytes());
        }
    }
    Ok(output)
}

/// An icon bank of 32x32 icons stacked in one column.
fn icon_bank(built: &str, pixels: &[u8], width: usize, height: usize) -> Result<Vec<u8>, String> {
    if width != 32 || !height.is_multiple_of(32) {
        return Err(format!("{built}: icons are 32 pixels square, stacked"));
    }
    let count = height / 32;
    let mut output = vec![0u8; 2 * count];
    for (index, icon) in pixels.chunks(32 * 32).enumerate() {
        let at = u16::try_from(output.len()).map_err(|_| format!("{built}: bank too large"))?;
        output[2 * index..2 * index + 2].copy_from_slice(&at.to_le_bytes());
        let mut stream = compress_palette(&metatiles(icon, 32, 32, GbaBpp::Bpp8, 1, 1)?, &PACKER)?;
        stream.resize(stream.len().next_multiple_of(32), 0);
        output.extend(stream);
    }
    Ok(output)
}

/// A sprite bank of square frames stacked in one column.
fn sprite_bank(built: &str, pixels: &[u8], width: usize, height: usize) -> Result<Vec<u8>, String> {
    if width == 0 || !height.is_multiple_of(width) {
        return Err(format!("{built}: frames must be square and stacked"));
    }
    let frames: Vec<&[u8]> = pixels.chunks(width * width).collect();
    let table = 4 * (frames.len() + 1);
    let mut offsets = vec![0u32; frames.len()];
    let mut body = Vec::new();
    let back = (1..frames.len()).step_by(2);
    let front = (0..frames.len()).step_by(2);
    for index in back.chain(front) {
        offsets[index] = (table + body.len()) as u32;
        body.extend(
            encode_zero_skip(frames[index]).map_err(|error| format!("{built}: {}", error.0))?,
        );
    }
    let mut output: Vec<u8> = offsets
        .iter()
        .chain([&0])
        .flat_map(|offset| offset.to_le_bytes())
        .collect();
    output.extend(body);
    Ok(output)
}

fn image_form(built: &str, form: &str, png: &[u8]) -> Result<Vec<u8>, String> {
    let image = indexed_bitmap_png(png).map_err(|error| format!("{built}: {}", error.0))?;
    let (width, height) = (image.width as usize, image.height as usize);
    Ok(match form {
        "gbapal" => bgr555_palette_of(&image).map_err(|error| format!("{built}: {}", error.0))?,
        "bitmap" => indices(&image),
        "4bpp" => metatiles(&indices(&image), width, height, GbaBpp::Bpp4, 1, 1)?,
        "8bpp" => metatiles(&indices(&image), width, height, GbaBpp::Bpp8, 1, 1)?,
        "frames" => sprite_bank(built, &indices(&image), width, height)?,
        "icons" => icon_bank(built, &indices(&image), width, height)?,
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
        // A font: one 16x16 cell whose row 1 inks the leftmost and rightmost pixels.
        let mut pixels = vec![0u8; 256];
        pixels[16] = 1;
        pixels[31] = 1;
        let sheet = png_from_bitmap(&pixels, &[0, 0, 0xff, 0x7f], 16).unwrap();
        let table = b"first\t41\ntop\t1\nrows\t2\n41\t8\t0\n";
        let built = build_file_with("F.font", &sheet, &|name| {
            assert_eq!(name, "F.TSV");
            Ok(table.to_vec())
        })
        .unwrap();
        assert_eq!(built, [8, 0, 0, 0, 1, 0x80, 0, 0]);
        assert!(build_file("F.font", &sheet).is_err());
        // An icon bank: a halfword offset per icon, streams padded to 32 bytes.
        let icons = build_file(
            "I.icons",
            &png_from_bitmap(&[1; 32 * 64], &[0, 0, 1, 0], 32).unwrap(),
        )
        .unwrap();
        assert_eq!(&icons[..4], [4, 0, 36, 0]);
        assert_eq!((icons.len() - 4) % 32, 0);
        assert!(build_file(
            "I.icons",
            &png_from_bitmap(&[0; 16 * 32], &[0, 0], 16).unwrap()
        )
        .is_err());
        // Parts join in list order.
        let joined = build_file_with("P.parts", b"A\tbitmap\nB\tbitmap\n", &|name| {
            Ok(match name {
                "A.PNG" => png_from_bitmap(&[1, 2], &[0, 0, 1, 0, 2, 0, 3, 0], 2).unwrap(),
                _ => png_from_bitmap(&[3], &[0, 0, 1, 0, 2, 0, 3, 0], 1).unwrap(),
            })
        })
        .unwrap();
        assert_eq!(joined, [1, 2, 3]);
        // A table packs each record's fields in their columns' types.
        assert_eq!(input_name("M/PATH.table.lz").unwrap(), "M/PATH.TSV");
        assert_eq!(
            build_file("T.table", b"x:s8\ty:u16\n-1\t0x102\n2\n").unwrap(),
            [0xff, 2, 1, 2]
        );
        assert!(build_file("T.table", b"x:s8\n128\n").is_err());
        assert!(build_file("T.table", b"x:u8\ty:u8\n1\n2\t3\n").is_err());
        assert!(build_file("T.table", b"x\n1\n").is_err());
        // A part list may join a table after an image.
        let mixed = build_file_with("P.parts", b"A\tbitmap\nM\ttable\n", &|name| {
            Ok(match name {
                "A.PNG" => png_from_bitmap(&[1, 2], &[0, 0, 1, 0, 2, 0, 3, 0], 2).unwrap(),
                _ => b"v:u16\n0x304\n".to_vec(),
            })
        })
        .unwrap();
        assert_eq!(mixed, [1, 2, 4, 3]);
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
        assert_eq!(build_file("A.bitmap.d7", &png).unwrap().len() % 2, 0);
        assert!(build_file("A.4bpp.zip", &png).is_err());
        // Two 2x2 frames: the back one (1) is stored before the front one (0).
        let bank = png_from_bitmap(&[1, 0, 0, 2, 3, 3, 3, 3], &palette, 2).unwrap();
        assert_eq!(
            build_file("A.frames", &bank).unwrap(),
            [17, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 1, 0xe1, 2, 0]
        );
        // A bitmap may be any size; tiles need whole tiles.
        let odd = png_from_bitmap(&[3; 60], &palette, 12).unwrap();
        assert_eq!(build_file("A.bitmap", &odd).unwrap().len(), 60);
        assert!(build_file("A.4bpp", &odd).is_err());
    }
}
