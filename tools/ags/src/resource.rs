//! Resource directory files built from editable inputs. A built file's name
//! is its recipe, as in pret's graphics rules: the stem names the input, the
//! first extension its form, and an optional second extension the resource
//! packer's codec. Image forms read the indexed image `STEM.PNG`:
//!
//! - `.gbapal`: the image's palette as little-endian BGR555.
//! - `.bitmap`: the pixels row by row, one palette index per byte.
//! - `.4bpp`, `.8bpp`: the pixels as GBA tiles, row-major.
//! - `.4bppWxH`, `.8bppWxH`: the pixels as GBA tiles cut into metatiles of
//!   one OBJ shape, W by H pixels, as pret's gbagfx -mwidth and -mheight:
//!   each metatile's tiles row-major before the next one's. A sheet of 1D
//!   sprites is drawn this way so each sprite reads whole.
//! - `.glyphs`: 1-bit glyphs. The image is one glyph, 8 pixels, wide with its
//!   glyphs stacked; each row is a byte, its leftmost pixel in bit 0 and any
//!   non-zero index inked.
//! - `.icons`: an icon bank. The image is one 32x32 icon wide with its icons
//!   stacked. The bank is a table of each icon's halfword offset, then each
//!   icon as 8-bit tiles in the packer's palette LZ without its tag,
//!   its stream zero-padded to a multiple of 32 bytes.
//! - `.frames`: a sprite bank. The image is one square frame wide with its
//!   frames stacked, a front and a back pose in turn. The bank is a table of
//!   each frame's offset in that order, ending 0, then the back frames and
//!   then the front frames, each zero-skip coded.
//!
//! A data form reads the identified table `STEM.BIN`:
//!
//! - `.bin`: the table's bytes as they are.
//!
//! A table reads its text `STEM.TSV`:
//!
//! - `.table`: records of little-endian fields. The first line names each
//!   column and its type, such as `x:s16\ty:s16` (`u8`, `s8`, `u16`, `s16`,
//!   `u32` or `s32`); each further line is a record of decimal or `0x` hex
//!   values in those columns. Only the last record may stop short.
//! - `.plane`: a grid plane of cells, such as a map's 128x128 grid. The
//!   first line gives the cell type, the grid's size, the fill of every
//!   cell not written and the top-left cell of the written rows:
//!   `cell:u16\tgrid:128x128\tfill:fff\tat:2,0`. Each further line is one
//!   grid row from that cell, its cells in hex separated by spaces; the rest
//!   of the row is fill, and an empty line a row of fill.
//! - `.delta0`, `.delta1`, `.delta2`: the table's bytes as 16-bit tilemap
//!   entries, delta-coded in that mode (Psynergy's tilemap delta).
//! - `.script`: a halfword command script, one command per line, its values
//!   decimal or `0x` hex: `channel N` (0xfd00 | N), `jump N` (0xfe00 | N),
//!   `stop` (0xfeff), `end` (0xffff), and the commands `frame SOURCE COUNT
//!   DESTINATION DELAY`, `control BLDCNT` and `level VALUE DELAY`, written
//!   as their halfwords.
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
//! A field sprite bank reads its record table `STEM.TSV` and each sprite's
//! PNG and TSV beside it (see ags::sprite):
//!
//! - `.sprites`: the bank as assembler source, which a data source
//!   `.include`s; its lists name the labels of the frames and scripts.
//! - `.spriteblocks`: the same, laid out sprite by sprite in the order
//!   `LAYOUT.TSV` beside it gives.
//!
//! A picture made of several reads its part list `STEM.TSV`:
//!
//! - `.parts`: each line names a part image beside the list and that part's
//!   form, such as `BLUE_FLAME_COLUMN\tbitmap`, or `table` for a table beside
//!   it; the parts are built in turn and joined.
//!
//! - `.icons4`: an icon bank of 4-bit icons, each with its own palette. Each
//!   line names a 32x32 icon image beside the list, or `-` for an empty
//!   slot. The bank is a table of each slot's halfword offset, 0 when empty,
//!   then each icon's 16-colour palette and its pixels, row by row, in the
//!   4-bit icon coder (Psynergy's icon4), zero-padded to a word, or to the
//!   hex boundary an `align` line gives (the Japanese ☀️ bank pads to 20).
//!
//! A block map reads its grid `STEM.TSV`, one line per row of hex words:
//!
//! - `.blocks`: the grid cut into 16x16 blocks, left to right and top to
//!   bottom. The bank is a table of each block's word offset, then each
//!   block's words, little-endian, in the packer's palette LZ without its tag.
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
use psynergy::assets::compression::{
    encode_delta7, encode_icon4, encode_tilemap_delta, encode_zero_skip,
};
use psynergy::assets::image::{bgr555_palette_of, indexed_bitmap_png, GbaBpp};

/// The resource packer's compressor, as the streams in both games show: the
/// general ring's window, read-ahead and reach, and the palette ring's
/// read-ahead. It packed every code overlay and every LZ data resource.
pub const PACKER: LzMachine = LzMachine::new(4123, 485, 4126, 272);

/// Whether a form reads a table or tilemap rather than an image.
fn data_form(form: &str) -> bool {
    matches!(
        form,
        "bin"
            | "delta0"
            | "delta1"
            | "delta2"
            | "parts"
            | "table"
            | "plane"
            | "icons4"
            | "blocks"
            | "script"
            | "sprites"
            | "spriteblocks"
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
        "bin" => "BIN",
        form if data_form(form) => "TSV",
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
            "plane" => plane(built, input)?,
            "icons4" => icon4_bank(built, input, sibling)?,
            "blocks" => block_map(built, input)?,
            "script" => script(built, input)?,
            "sprites" => crate::sprite::bank(built, input, sibling)?
                .source()?
                .into_bytes(),
            "spriteblocks" => crate::sprite::blocks_source(built, input, sibling)?.into_bytes(),
            _ => encode_tilemap_delta(&table(built, input)?, form.as_bytes()[5] - b'0')
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
        if (data_form(form) && !matches!(form, "table" | "plane")) || form == "font" {
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

/// A halfword command script, one command per line.
fn script(built: &str, text: &[u8]) -> Result<Vec<u8>, String> {
    let text = std::str::from_utf8(text).map_err(|_| format!("{built}: script is not text"))?;
    let mut words: Vec<u16> = Vec::new();
    for line in text.lines().filter(|line| !line.trim().is_empty()) {
        let mut fields = line.split('\t').map(str::trim);
        let command = fields.next().unwrap_or_default();
        let values = fields
            .map(|field| {
                match field.strip_prefix("0x") {
                    Some(hex) => u16::from_str_radix(hex, 16),
                    None => field.parse::<u16>(),
                }
                .map_err(|_| format!("{built}: {field:?} is not a halfword"))
            })
            .collect::<Result<Vec<u16>, _>>()?;
        let (arity, prefix) = match command {
            "channel" => (1, Some(0xfd00)),
            "jump" => (1, Some(0xfe00)),
            "stop" => (0, Some(0xfeff)),
            "end" => (0, Some(0xffff)),
            "frame" => (4, None),
            "control" => (1, None),
            "level" => (2, None),
            _ => return Err(format!("{built}: unknown command {command:?}")),
        };
        if values.len() != arity {
            return Err(format!("{built}: {line:?} needs {arity} values"));
        }
        match prefix {
            Some(_) if arity == 1 && values[0] > 0xff => {
                return Err(format!("{built}: {line:?} takes a byte"));
            }
            Some(base) => words.push(base | values.first().copied().unwrap_or(0)),
            None => words.extend(values),
        }
    }
    Ok(words.iter().flat_map(|word| word.to_le_bytes()).collect())
}

/// A grid plane of cells, cropped to where it differs from its fill.
fn plane(built: &str, text: &[u8]) -> Result<Vec<u8>, String> {
    let text = std::str::from_utf8(text).map_err(|_| format!("{built}: plane is not text"))?;
    // Rows follow the header line by line; an empty row is all fill.
    let mut lines = text.lines();
    let header = lines
        .next()
        .ok_or_else(|| format!("{built}: plane has no header"))?;
    let field = |key: &str| {
        header
            .split('\t')
            .find_map(|pair| pair.strip_prefix(key)?.strip_prefix(':'))
            .ok_or_else(|| format!("{built}: header needs {key}"))
    };
    let number = |value: &str| {
        value
            .parse::<usize>()
            .map_err(|_| format!("{built}: {value:?} is not a number"))
    };
    let pair = |value: &str, separator: char| -> Result<(usize, usize), String> {
        let (a, b) = value
            .split_once(separator)
            .ok_or_else(|| format!("{built}: {value:?} needs two numbers"))?;
        Ok((number(a)?, number(b)?))
    };
    let size = match field("cell")? {
        "u8" => 1,
        "u16" => 2,
        "u32" => 4,
        other => return Err(format!("{built}: cell type {other:?}")),
    };
    let (width, height) = pair(field("grid")?, 'x')?;
    let (left, top) = pair(field("at")?, ',')?;
    let hex = |word: &str| match u32::from_str_radix(word, 16) {
        Ok(value) if word.len() <= 2 * size && !word.starts_with('+') => Ok(value),
        _ => Err(format!("{built}: {word:?} is not a hex cell")),
    };
    let fill = hex(field("fill")?)?;
    let mut cells = vec![fill; width * height];
    for (y, line) in lines.enumerate() {
        let row = line.split_whitespace().collect::<Vec<_>>();
        if top + y >= height || left + row.len() > width {
            return Err(format!("{built}: row {y} leaves the grid"));
        }
        for (x, cell) in row.iter().enumerate() {
            cells[(top + y) * width + left + x] = hex(cell)?;
        }
    }
    Ok(cells
        .iter()
        .flat_map(|cell| cell.to_le_bytes().into_iter().take(size))
        .collect())
}

/// A bank of 16x16 blocks of words cut from a grid, each packed.
fn block_map(built: &str, grid: &[u8]) -> Result<Vec<u8>, String> {
    let text = std::str::from_utf8(grid).map_err(|_| format!("{built}: grid is not text"))?;
    let rows = text
        .lines()
        .filter(|line| !line.trim().is_empty())
        .map(|line| {
            line.split('\t')
                .map(|word| {
                    u32::from_str_radix(word.trim(), 16)
                        .map_err(|_| format!("{built}: {word:?} is not a hex word"))
                })
                .collect::<Result<Vec<_>, _>>()
        })
        .collect::<Result<Vec<_>, _>>()?;
    let width = rows.first().map_or(0, Vec::len);
    if width == 0
        || !width.is_multiple_of(16)
        || !rows.len().is_multiple_of(16)
        || rows.iter().any(|row| row.len() != width)
    {
        return Err(format!("{built}: the grid must be whole 16x16 blocks"));
    }
    let count = width / 16 * (rows.len() / 16);
    let mut output = vec![0u8; 4 * count];
    for index in 0..count {
        let (x, y) = (index % (width / 16) * 16, index / (width / 16) * 16);
        let words: Vec<u8> = rows[y..y + 16]
            .iter()
            .flat_map(|row| row[x..x + 16].iter().flat_map(|word| word.to_le_bytes()))
            .collect();
        let at = output.len() as u32;
        output[4 * index..4 * index + 4].copy_from_slice(&at.to_le_bytes());
        output.extend(compress_palette(&words, &PACKER)?);
    }
    Ok(output)
}

/// A bank of 4-bit icons with their own palettes from its slot list.
fn icon4_bank(
    built: &str,
    list: &[u8],
    sibling: &dyn Fn(&str) -> Result<Vec<u8>, String>,
) -> Result<Vec<u8>, String> {
    let text = std::str::from_utf8(list).map_err(|_| format!("{built}: icon list is not text"))?;
    let mut align = 4;
    let mut slots: Vec<&str> = Vec::new();
    for line in text.lines().filter(|line| !line.trim().is_empty()) {
        match line.strip_prefix("align\t") {
            Some(value) => {
                align = usize::from_str_radix(value.trim(), 16)
                    .ok()
                    .filter(|align| align.is_power_of_two())
                    .ok_or_else(|| format!("{built}: align {value} is not a power of two"))?
            }
            None => slots.push(line),
        }
    }
    let mut output = vec![0u8; 2 * slots.len()];
    for (index, slot) in slots.iter().enumerate() {
        if *slot == "-" {
            continue;
        }
        let at = u16::try_from(output.len()).map_err(|_| format!("{built}: bank too large"))?;
        output[2 * index..2 * index + 2].copy_from_slice(&at.to_le_bytes());
        let image = indexed_bitmap_png(&sibling(&format!("{slot}.PNG"))?)
            .map_err(|error| format!("{built}: {slot}: {}", error.0))?;
        let palette =
            bgr555_palette_of(&image).map_err(|error| format!("{built}: {slot}: {}", error.0))?;
        if (image.width, image.height) != (32, 32) || palette.len() != 32 {
            return Err(format!("{built}: {slot} must be 32x32 in 16 colours"));
        }
        output.extend(palette);
        output.extend(
            encode_icon4(&indices(&image))
                .map_err(|error| format!("{built}: {slot}: {}", error.0))?,
        );
        output.resize(output.len().next_multiple_of(align), 0);
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

/// The OBJ shape `WxH` in pixels names, in tiles: one of the twelve.
fn obj_shape(name: &str) -> Option<(usize, usize)> {
    let (wide, high) = name.split_once('x')?;
    let shape = (
        wide.parse::<usize>().ok()? / 8,
        high.parse::<usize>().ok()? / 8,
    );
    (psynergy::assets::image::OBJ_SHAPES.contains(&shape)
        && format!("{}x{}", shape.0 * 8, shape.1 * 8) == name)
        .then_some(shape)
}

fn image_form(built: &str, form: &str, png: &[u8]) -> Result<Vec<u8>, String> {
    let image = indexed_bitmap_png(png).map_err(|error| format!("{built}: {}", error.0))?;
    let (width, height) = (image.width as usize, image.height as usize);
    Ok(match form {
        "gbapal" => bgr555_palette_of(&image).map_err(|error| format!("{built}: {}", error.0))?,
        "bitmap" => indices(&image),
        "4bpp" => metatiles(&indices(&image), width, height, GbaBpp::Bpp4, 1, 1)?,
        "8bpp" => metatiles(&indices(&image), width, height, GbaBpp::Bpp8, 1, 1)?,
        form if form.starts_with("4bpp") || form.starts_with("8bpp") => {
            let bpp = if form.starts_with('4') {
                GbaBpp::Bpp4
            } else {
                GbaBpp::Bpp8
            };
            let (wide, high) = obj_shape(&form[4..])
                .ok_or_else(|| format!("{built}: .{form} names no OBJ shape"))?;
            metatiles(&indices(&image), width, height, bpp, wide, high)?
        }
        "frames" => sprite_bank(built, &indices(&image), width, height)?,
        "icons" => icon_bank(built, &indices(&image), width, height)?,
        "glyphs" if width == 8 => indices(&image)
            .chunks(8)
            .map(|row| {
                row.iter()
                    .enumerate()
                    .fold(0u8, |byte, (x, &pixel)| byte | u8::from(pixel != 0) << x)
            })
            .collect(),
        "glyphs" => return Err(format!("{built}: glyphs are 8 pixels wide, stacked")),
        other => return Err(format!("{built}: unknown form .{other}")),
    })
}

#[cfg(test)]
mod tests {
    use super::*;
    use psynergy::assets::image::{png_from_bitmap, png_from_gba_tiles};

    #[test]
    fn metatile_forms_cut_the_sheet_into_their_obj_shape() {
        // A 16x16 sheet whose four tiles are filled 1, 2, 3 and 4 row-major.
        let pixels: Vec<u8> = (0..256)
            .map(|i| 1 + (i % 16 / 8 + i / 128 * 2) as u8)
            .collect();
        let palette = [0u8; 32];
        let sheet = png_from_bitmap(&pixels, &palette, 16).unwrap();
        let first = |built: Vec<u8>| {
            built
                .chunks(32)
                .map(|tile| tile[0] & 15)
                .collect::<Vec<_>>()
        };
        assert_eq!(first(build_file("T.4bpp", &sheet).unwrap()), [1, 2, 3, 4]);
        assert_eq!(
            first(build_file("T.4bpp8x16", &sheet).unwrap()),
            [1, 3, 2, 4]
        );
        assert_eq!(
            first(build_file("T.4bpp16x16", &sheet).unwrap()),
            [1, 2, 3, 4]
        );
        assert!(build_file("T.4bpp24x8", &sheet).is_err());
        assert!(build_file("T.4bpp32x32", &sheet).is_err());
        assert!(build_file("T.4bpp08x16", &sheet).is_err());
    }

    #[test]
    fn names_read_their_image_and_follow_their_recipe() {
        assert_eq!(
            input_name("GRAPHICS/FX/STAR.bitmap.lz").unwrap(),
            "GRAPHICS/FX/STAR.PNG"
        );
        assert!(input_name("GRAPHICS/FX/STAR").is_err());
        assert_eq!(
            input_name("MAP/M/CELLS.delta1.lz").unwrap(),
            "MAP/M/CELLS.TSV"
        );
        assert_eq!(input_name("M/T.bin").unwrap(), "M/T.BIN");
        assert_eq!(build_file("T.bin", &[1, 2, 3]).unwrap(), [1, 2, 3]);
        assert_eq!(
            build_file("T.delta2", b"a:u16\tb:u16\n1\t3\n").unwrap(),
            [2, 1, 0, 2, 0]
        );
        assert_eq!(
            build_file("T.delta1.lz", b"a:u16\n1\n").unwrap()[0] <= 1,
            true
        );
        assert!(build_file("T.delta3", b"a:u16\n1\n").is_err());
        // A script writes each command as its halfwords.
        assert_eq!(
            build_file(
                "T.script",
                b"channel\t0x84\nframe\t0x600\t2\t0x480\t0\njump\t0\nstop\nend\n"
            )
            .unwrap(),
            [0x84, 0xfd, 0, 6, 2, 0, 0x80, 4, 0, 0, 0, 0xfe, 0xff, 0xfe, 0xff, 0xff]
        );
        assert_eq!(
            build_file("T.script", b"control\t0x3f44\nlevel\t0x1008\t10\n").unwrap(),
            [0x44, 0x3f, 8, 0x10, 10, 0]
        );
        assert!(build_file("T.script", b"frame\t1\t2\n").is_err());
        assert!(build_file("T.script", b"jump\t0x100\n").is_err());
        assert!(build_file("T.script", b"wait\t1\n").is_err());
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
        // A 4-bit icon bank: an empty slot is 0; an icon is its palette and
        // its stream, padded to a word.
        let bank = build_file_with("B.icons4", b"-\nA\n", &|name| {
            assert_eq!(name, "A.PNG");
            Ok(png_from_bitmap(&[0; 32 * 32], &[0; 32], 32).unwrap())
        })
        .unwrap();
        assert_eq!(&bank[..4], [0, 0, 4, 0]);
        assert_eq!(bank.len(), 4 + 32 + 132);
        assert_eq!(&bank[36..40], [0, 0, 0, 0]);
        let wide = build_file_with("B.icons4", b"align\t20\n-\nA\n", &|_| {
            Ok(png_from_bitmap(&[0; 32 * 32], &[0; 32], 32).unwrap())
        })
        .unwrap();
        assert_eq!(wide.len(), 192);
        assert!(build_file_with("B.icons4", b"align\t3\nA\n", &|_| Ok(Vec::new())).is_err());
        // A block map: two blocks side by side, a word offset each.
        let row = format!("{}\n", ["c80"; 32].join("\t"));
        let blocks = build_file("M.blocks", row.repeat(16).as_bytes()).unwrap();
        assert_eq!(&blocks[..4], [8, 0, 0, 0]);
        let second = u32::from_le_bytes(blocks[4..8].try_into().unwrap()) as usize;
        assert_eq!(blocks[8..second], blocks[second..]);
        assert!(build_file("M.blocks", b"c80\t0\n").is_err());
        assert!(build_file("M.blocks", b"x\n").is_err());
        // Parts join in list order.
        let joined = build_file_with("P.parts", b"A\tbitmap\nB\tbitmap\n", &|name| {
            Ok(match name {
                "A.PNG" => png_from_bitmap(&[1, 2], &[0, 0, 1, 0, 2, 0, 3, 0], 2).unwrap(),
                _ => png_from_bitmap(&[3], &[0, 0, 1, 0, 2, 0, 3, 0], 1).unwrap(),
            })
        })
        .unwrap();
        assert_eq!(joined, [1, 2, 3]);
        // 1-bit glyph rows: the leftmost pixel in bit 0.
        let mut rows = vec![0u8; 16];
        rows[0] = 1;
        rows[15] = 2;
        let glyphs = png_from_bitmap(&rows, &[0, 0, 0xff, 0x7f, 0x1f, 0], 8).unwrap();
        assert_eq!(build_file("G.glyphs", &glyphs).unwrap(), [1, 0x80]);
        assert!(build_file("G.glyphs", &png_from_bitmap(&[0; 32], &[0, 0], 16).unwrap()).is_err());
        // A table packs each record's fields in their columns' types.
        assert_eq!(input_name("M/PATH.table.lz").unwrap(), "M/PATH.TSV");
        assert_eq!(
            build_file("T.table", b"x:s8\ty:u16\n-1\t0x102\n2\n").unwrap(),
            [0xff, 2, 1, 2]
        );
        assert!(build_file("T.table", b"x:s8\n128\n").is_err());
        assert!(build_file("T.table", b"x:u8\ty:u8\n1\n2\t3\n").is_err());
        assert!(build_file("T.table", b"x\n1\n").is_err());
        // A plane fills around its written rows; cells are hex, space-separated.
        assert_eq!(
            input_name("M/GRID_SHAPE.plane").unwrap(),
            "M/GRID_SHAPE.TSV"
        );
        assert_eq!(
            build_file(
                "P.plane",
                b"cell:u16\tgrid:3x2\tfill:fff\tat:1,1\n001 0a2\n"
            )
            .unwrap(),
            [0xff, 0xf, 0xff, 0xf, 0xff, 0xf, 0xff, 0xf, 1, 0, 0xa2, 0]
        );
        assert_eq!(
            build_file("P.plane", b"cell:u8\tgrid:2x1\tfill:7\tat:0,0\n").unwrap(),
            [7, 7]
        );
        assert!(build_file("P.plane", b"cell:u8\tgrid:2x1\tfill:0\tat:1,0\n01 02\n").is_err());
        // A short row ends in fill; an empty row is all fill.
        assert_eq!(
            build_file(
                "P.plane",
                b"cell:u8\tgrid:2x3\tfill:9\tat:0,0\n01\n\n02 03\n"
            )
            .unwrap(),
            [1, 9, 9, 9, 2, 3]
        );
        assert!(build_file("P.plane", b"cell:u8\tgrid:2x1\tfill:0\tat:0,0\n010\n").is_err());
        assert!(build_file("P.plane", b"cell:u8\tgrid:1x1\tfill:0\tat:0,0\n\n\n").is_err());
        assert!(build_file("P.plane", b"cell:u8\tgrid:2x2\tfill:100\tat:0,0\n").is_err());
        assert!(build_file("P.plane", b"cell:u8\tgrid:2x2\tat:0,0\n").is_err());
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
