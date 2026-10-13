use crate::graphics::{indices, metatiles, MapDrawing};
use crate::lz::{
    compress_mtf4_packed, compress_palette, compress_palette_packed, compress_tagged_packed,
    LzMachine,
};
use crate::patch::{patch_name, Patch};
use crate::assets::compression::{
    encode_delta7, encode_halfword_lz, encode_icon4, encode_tilemap_delta, encode_zero_skip,
};
use crate::assets::image::{
    bgr555_palette_of, indexed_bitmap_png, png_like, tile_sheet_pixels, tiles_from_metatiles,
    GbaBpp, TileLayout,
};

pub const PACKER: LzMachine = LzMachine::new(4123, 485, 4126, 272);

pub(crate) const CODECS: &[&str] = &["lz", "plz", "mtf", "d7", "hlz"];

pub fn is_recipe(built: &str) -> bool {
    let file = built.rsplit('/').next().unwrap_or(built);
    let mut parts = file.split('.');
    let stem = parts.next().unwrap_or_default();
    let extensions: Vec<&str> = parts.collect();
    let (form, codec) = match extensions.as_slice() {
        [form] => (*form, None),
        [form, codec] => (*form, Some(*codec)),
        _ => return false,
    };
    let form = tile_form(form).is_some()
        || data_form(form)
        || matches!(
            form,
            "font" | "gbapal" | "bitmap" | "frames" | "icons" | "icon4" | "glyphs" | "1bpp" | "strips"
        );
    !stem.is_empty() && form && codec.is_none_or(|codec| CODECS.contains(&codec))
}

fn data_form(form: &str) -> bool {
    matches!(
        form,
        "delta0"
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

pub fn input_name(built: &str) -> Result<String, String> {
    let (directory, file) = built.rsplit_once('/').unwrap_or(("", built));
    let (stem, rest) = file
        .split_once('.')
        .ok_or_else(|| format!("{built} names no form"))?;
    let form = rest.split('.').next().unwrap_or_default();
    let extension = match form {
        form if data_form(form) => "tsv",
        _ => "png",
    };
    Ok(if directory.is_empty() {
        format!("{stem}.{extension}")
    } else {
        format!("{directory}/{stem}.{extension}")
    })
}

/// The file a recipe's table names beside its `input`, or under common/.
/// Tables name files by their stems, which double as symbol names in upper
/// case; the files themselves are lower case.
pub fn sibling_path(input: &std::path::Path, name: &str) -> Result<std::path::PathBuf, String> {
    let name = &name.to_ascii_lowercase();
    let Some(shared) = name.strip_prefix("common/") else {
        return Ok(input.with_file_name(name));
    };
    if shared
        .split('/')
        .any(|part| part.is_empty() || part == ".." || part == "." || part.contains('\\'))
    {
        return Err(format!("{name} names no file in COMMON"));
    }
    let (games, folder) = input
        .parent()
        .and_then(|folder| {
            let text = folder.to_str()?;
            let at = text
                .find("/src/")
                .or_else(|| text.ends_with("/src").then(|| text.len() - 4))?;
            let games = std::path::Path::new(&text[..at]).parent()?;
            Some((games.to_path_buf(), text[at + 1..].to_owned()))
        })
        .ok_or_else(|| format!("{} lies under no game's SRC", input.display()))?;
    Ok(if shared.contains('/') {
        games.join("common/src").join(shared)
    } else {
        games.join("common").join(folder).join(shared)
    })
}

pub fn build_file(built: &str, input: &[u8]) -> Result<Vec<u8>, String> {
    build_file_with(built, input, &|name| Err(format!("{built} needs {name}")))
}

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
        let table = sibling(&stem.replace(".png", ".tsv"))?;
        font(built, input, &table)?
    } else {
        image_form(built, form, input)?
    };
    let packed = match codec {
        Some("lz" | "plz" | "mtf") => packed_bytes(built, &pixels, sibling)?,
        _ => None,
    };
    let packed = packed.as_deref().unwrap_or(&pixels);
    match codec {
        None => Ok(pixels),
        Some("lz") => compress_tagged_packed(&pixels, packed, &PACKER),
        Some("plz") => compress_palette_packed(&pixels, packed, &PACKER),
        Some("mtf") => compress_mtf4_packed(&pixels, packed),
        Some("d7") => encode_delta7(&pixels).map_err(|error| format!("{built}: {}", error.0)),
        Some("hlz") => encode_halfword_lz(&pixels).map_err(|error| format!("{built}: {}", error.0)),
        Some(other) => Err(format!("{built}: unknown codec .{other}")),
    }
}

fn parts(
    built: &str,
    list: &[u8],
    sibling: &dyn Fn(&str) -> Result<Vec<u8>, String>,
) -> Result<Vec<u8>, String> {
    let text = std::str::from_utf8(list).map_err(|_| format!("{built}: part list is not text"))?;
    let mut output = Vec::new();
    for line in text.lines().filter(|line| !line.trim().is_empty()) {
        let fields: Vec<&str> = line.split('\t').map(str::trim).collect();
        let [part, form, drawing @ ..] = fields.as_slice() else {
            return Err(format!("{built}: {line:?} needs a part and its form"));
        };
        if (data_form(form) && !matches!(*form, "table" | "plane")) || *form == "font" {
            return Err(format!(
                "{built}: part {part} must be an image form or a table"
            ));
        }
        let name = format!("{part}.{form}");
        let input = sibling(&input_name(&name)?)?;
        if drawing.is_empty() {
            output.extend(build_file(&name, &input)?);
            continue;
        }
        let drawn = MapPart::parse(built, drawing)?;
        let bpp = match *form {
            "4bpp" => GbaBpp::Bpp4,
            "8bpp" => GbaBpp::Bpp8,
            _ => {
                return Err(format!(
                    "{built}: part {part} drawn by a tilemap must be .4bpp or .8bpp"
                ))
            }
        };
        let layout = drawn.layout(built, sibling)?;
        let image = indexed_bitmap_png(&input).map_err(|error| format!("{name}: {}", error.0))?;
        output.extend(
            layout
                .tiles(
                    &indices(&image),
                    image.width as usize,
                    image.height as usize,
                    bpp,
                    drawn.frames,
                )
                .map_err(|error| format!("{name}: {error}"))?,
        );
    }
    Ok(output)
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct MapPart {
    pub map: String,
    pub first: usize,
    pub count: usize,
    pub frames: bool,
}

impl MapPart {
    pub fn parse(built: &str, fields: &[&str]) -> Result<Self, String> {
        let number = |field: &str| {
            match field.strip_prefix("0x") {
                Some(hex) => usize::from_str_radix(hex, 16),
                None => field.parse(),
            }
            .map_err(|_| format!("{built}: {field:?} is not a tile number"))
        };
        match fields {
            [map, first, count] | [map, first, count, "frames"] => Ok(Self {
                map: (*map).to_owned(),
                first: number(first)?,
                count: number(count)?,
                frames: fields.len() == 4,
            }),
            _ => Err(format!(
                "{built}: a drawn part reads MAP FIRST COUNT [frames]"
            )),
        }
    }

    pub fn layout(
        &self,
        built: &str,
        sibling: &dyn Fn(&str) -> Result<Vec<u8>, String>,
    ) -> Result<MapDrawing, String> {
        let text = sibling(&format!("{}.tsv", self.map))?;
        let (entries, wide) = tilemap(&format!("{}.table", self.map), &text)?;
        MapDrawing::new(&entries, wide, self.first, self.count)
            .map_err(|error| format!("{built}: {error}"))
    }
}

pub fn tilemap(built: &str, text: &[u8]) -> Result<(Vec<u16>, usize), String> {
    let header = std::str::from_utf8(text)
        .ok()
        .and_then(|text| text.lines().next())
        .ok_or_else(|| format!("{built}: table names no columns"))?;
    let wide = header.split('\t').count();
    if header.split('\t').any(|column| !column.ends_with(":u16")) {
        return Err(format!("{built}: a tilemap's columns are u16"));
    }
    let bytes = table(built, text)?;
    let entries: Vec<u16> = bytes
        .as_chunks::<2>().0.iter()
        .map(|pair| u16::from_le_bytes([pair[0], pair[1]]))
        .collect();
    if !entries.len().is_multiple_of(wide) {
        return Err(format!("{built}: a tilemap has whole rows"));
    }
    Ok((entries, wide))
}

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

fn plane(built: &str, text: &[u8]) -> Result<Vec<u8>, String> {
    let text = std::str::from_utf8(text).map_err(|_| format!("{built}: plane is not text"))?;
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

/// A block map: one offset word per 16x16 block of the grid, then each
/// block's words as a palette stream. A block the map leaves out is written
/// `-` in all its cells: its offset is zero and it has no stream.
fn block_map(built: &str, grid: &[u8]) -> Result<Vec<u8>, String> {
    let text = std::str::from_utf8(grid).map_err(|_| format!("{built}: grid is not text"))?;
    let rows = text
        .lines()
        .filter(|line| !line.trim().is_empty())
        .map(|line| {
            line.split('\t')
                .map(|word| match word.trim() {
                    "-" => Ok(None),
                    word => u32::from_str_radix(word, 16)
                        .map(Some)
                        .map_err(|_| format!("{built}: {word:?} is not a hex word")),
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
        let cells: Vec<Option<u32>> = rows[y..y + 16]
            .iter()
            .flat_map(|row| row[x..x + 16].iter().copied())
            .collect();
        if cells.iter().all(Option::is_none) {
            continue;
        }
        let words = cells
            .iter()
            .map(|cell| cell.map(u32::to_le_bytes))
            .collect::<Option<Vec<_>>>()
            .ok_or_else(|| format!("{built}: block {index} is left out in part"))?
            .concat();
        let at = output.len() as u32;
        output[4 * index..4 * index + 4].copy_from_slice(&at.to_le_bytes());
        output.extend(compress_palette(&words, &PACKER)?);
    }
    Ok(output)
}

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
        let image = indexed_bitmap_png(&sibling(&format!("{slot}.png"))?)
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

/// Square frames stacked in one sheet: an offset word for each and a zero,
/// then the odd frames' zero-skip streams and the even ones'. A frame drawn
/// wholly in index 255, which no stream can hold, is a slot the bank numbers
/// but never drew: its word points where the next stream starts.
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
        if frames[index].iter().all(|pixel| *pixel == 0xff) {
            continue;
        }
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

/// A picture kept as strips: one offset word for the palette and one for
/// each 8-pixel row band, then the PNG's palette as BGR555 and each band's
/// 8bpp tiles, left to right, as its own tagged stream.
fn strip_picture(
    built: &str,
    image: &crate::assets::image::IndexedImage,
) -> Result<Vec<u8>, String> {
    let (width, height) = (image.width as usize, image.height as usize);
    if !width.is_multiple_of(8) || !height.is_multiple_of(8) {
        return Err(format!("{built}: a strip picture is whole 8x8 tiles"));
    }
    let pixels = indices(image);
    let palette = bgr555_palette_of(image).map_err(|error| format!("{built}: {}", error.0))?;
    let bands = height / 8;
    let mut output = vec![0u8; 4 * (bands + 1)];
    let place = |output: &mut Vec<u8>, index: usize| {
        let at = output.len() as u32;
        output[4 * index..4 * index + 4].copy_from_slice(&at.to_le_bytes());
    };
    place(&mut output, 0);
    output.extend(palette);
    for (band, rows) in pixels.chunks(8 * width).enumerate() {
        place(&mut output, band + 1);
        let tiles = metatiles(rows, width, 8, GbaBpp::Bpp8, 1, 1)?;
        output.extend(compress_tagged_packed(&tiles, &tiles, &PACKER)?);
    }
    Ok(output)
}

/// The depth and OBJ shape, in tiles, a tile form names: `.4bpp` and `.8bpp`
/// read single 8x8 tiles, `.4bpp32x16` and the like whole sprites (A4).
pub fn tile_form(form: &str) -> Option<(GbaBpp, (usize, usize))> {
    let bpp = match form.get(..4)? {
        "4bpp" => GbaBpp::Bpp4,
        "8bpp" => GbaBpp::Bpp8,
        _ => return None,
    };
    match &form[4..] {
        "" => Some((bpp, (1, 1))),
        shape => obj_shape(shape).map(|shape| (bpp, shape)),
    }
}

fn depth_of(form: &str) -> Result<GbaBpp, String> {
    tile_form(form)
        .map(|(bpp, _)| bpp)
        .ok_or_else(|| format!(".{form} is not a tile form"))
}

/// A tile sheet drawn again: the PNG `png` read as the tile form `from`, its
/// tiles laid out as the OBJ sprites of the tile form `to`, `wide` sprites to
/// a row (every sprite in one row when `None`), in the PNG's own palette,
/// transparency and bit depth. Both forms must be tile forms of one depth:
/// a 4bpp stream read as 8bpp pixels is no drawing of it. The drawing moves
/// the PNG's own pixels, every palette index whole: a 4bpp build keeps only
/// each index's low four bits, so a sheet drawn in palette banks would lose
/// its banks if drawn again from the built tiles.
pub fn redraw(png: &[u8], from: &str, to: &str, wide: Option<usize>) -> Result<Vec<u8>, String> {
    if depth_of(from)? != depth_of(to)? {
        return Err(format!(".{from} and .{to} differ in depth"));
    }
    let tiles = build_file(&format!("SHEET.{from}"), png)?;
    let sheet = Picture::read("the sheet", png, from)?;
    let drawing = drawn_as(&sheet.tiles, png, to, wide)?;
    builds_the_same(&drawing, to, &tiles)?;
    keeps_the_pixels(&drawing, to, &[sheet])?;
    Ok(drawing)
}

/// The tiles a part list of tile parts builds, drawn again as one sheet of
/// the OBJ sprites of `to` (see `redraw`). Every part must be a plain tile
/// part of `to`'s depth, and the parts' PNGs must share one palette unless
/// `palette` names the PNG whose palette the sheet takes.
pub fn redraw_parts(
    list: &[u8],
    to: &str,
    wide: Option<usize>,
    palette: Option<&[u8]>,
    sibling: &dyn Fn(&str) -> Result<Vec<u8>, String>,
) -> Result<Vec<u8>, String> {
    let depth = depth_of(to)?;
    let text = std::str::from_utf8(list).map_err(|_| "the part list is not text".to_string())?;
    let mut pngs = Vec::new();
    for line in text.lines().filter(|line| !line.trim().is_empty()) {
        let fields: Vec<&str> = line.split('\t').map(str::trim).collect();
        let [part, form] = fields.as_slice() else {
            return Err(format!("{line:?} is no tile part; only tile parts redraw"));
        };
        match tile_form(form) {
            Some((bpp, _)) if bpp == depth => {}
            Some(_) => return Err(format!("part {part} is .{form}, not the depth of .{to}")),
            None => return Err(format!("{line:?} is no tile part; only tile parts redraw")),
        }
        pngs.push((*part, *form, sibling(&input_name(&format!("{part}.{form}"))?)?));
    }
    let (first_part, _, first) = pngs.first().ok_or("the part list names no part")?;
    let like = match palette {
        Some(png) => png,
        None => {
            let shared = crate::assets::image::png_palette(first).map_err(|error| error.0)?;
            for (part, _, png) in &pngs[1..] {
                if crate::assets::image::png_palette(png).map_err(|error| error.0)? != shared {
                    return Err(format!(
                        "parts {first_part} and {part} differ in palette; name the palette to draw in"
                    ));
                }
            }
            first.as_slice()
        }
    };
    let tiles = build_file_with("LIST.parts", list, sibling)?;
    let parts = pngs
        .iter()
        .map(|(part, form, png)| Picture::read(&format!("part {part}"), png, form))
        .collect::<Result<Vec<_>, _>>()?;
    let sheet: Vec<u8> = parts.iter().flat_map(|part| part.tiles.iter().copied()).collect();
    let drawing = drawn_as(&sheet, like, to, wide)?;
    builds_the_same(&drawing, to, &tiles)?;
    keeps_the_pixels(&drawing, to, &parts)?;
    Ok(drawing)
}

/// A picture read as a tile form: its own pixels, and the same pixels as
/// 8x8 tiles in the order the form reads them, one byte a pixel. Every
/// palette index stays whole, where a 4bpp build keeps only its low four
/// bits; the build of the form packs these tiles.
struct Picture<'a> {
    name: String,
    form: &'a str,
    width: usize,
    pixels: Vec<u8>,
    tiles: Vec<u8>,
}

impl<'a> Picture<'a> {
    fn read(name: &str, png: &[u8], form: &'a str) -> Result<Self, String> {
        let (_, (wide, high)) =
            tile_form(form).ok_or_else(|| format!(".{form} is not a tile form"))?;
        let image = indexed_bitmap_png(png).map_err(|error| format!("{name}: {}", error.0))?;
        let (width, height) = (image.width as usize, image.height as usize);
        let pixels = indices(&image);
        let tiles = metatiles(&pixels, width, height, GbaBpp::Bpp8, wide, high)
            .map_err(|error| format!("{name}: {error}"))?;
        Ok(Self {
            name: name.to_owned(),
            form,
            width,
            pixels,
            tiles,
        })
    }

    /// How many of the form's sprites the picture holds to a row.
    fn sprites_wide(&self) -> usize {
        let (_, (wide, _)) = tile_form(self.form).expect("a picture is read as a tile form");
        self.width / (wide * 8)
    }
}

/// `tiles`, 8x8 tiles of one byte a pixel, laid out as the OBJ sprites of
/// `form`, `wide` to a row (every sprite in one row when `None`): the pixels
/// and their width.
fn laid_out(tiles: &[u8], form: &str, wide: Option<usize>) -> Result<(Vec<u8>, usize), String> {
    let (_, (sprite_wide, sprite_high)) =
        tile_form(form).ok_or_else(|| format!(".{form} is not a tile form"))?;
    let count = tiles.len() / 64;
    if count == 0 || !tiles.len().is_multiple_of(64) || !count.is_multiple_of(sprite_wide * sprite_high) {
        return Err(format!(
            "{count} tiles are not whole {}x{} sprites",
            sprite_wide * 8,
            sprite_high * 8
        ));
    }
    let sprites = count / (sprite_wide * sprite_high);
    let wide = wide.unwrap_or(sprites);
    if wide == 0 || !sprites.is_multiple_of(wide) {
        return Err(format!("{sprites} sprites do not fill rows of {wide}"));
    }
    let layout = TileLayout {
        meta: (sprite_wide, sprite_high),
        tiles_wide: wide * sprite_wide,
    };
    let sheet = tiles_from_metatiles(tiles, GbaBpp::Bpp8, layout);
    let pixels =
        tile_sheet_pixels(&sheet, GbaBpp::Bpp8, layout.tiles_wide).map_err(|error| error.0)?;
    Ok((pixels, layout.tiles_wide * 8))
}

/// `tiles` (see `laid_out`) drawn as the OBJ sprites of `form`, `wide` to a
/// row, in the palette, transparency and bit depth of the PNG `like`.
fn drawn_as(tiles: &[u8], like: &[u8], form: &str, wide: Option<usize>) -> Result<Vec<u8>, String> {
    let (pixels, width) = laid_out(tiles, form, wide)?;
    png_like(like, &pixels, width).map_err(|error| error.0)
}

/// `form` must build exactly `tiles`, the source's own build, from a drawing.
fn builds_the_same(png: &[u8], form: &str, tiles: &[u8]) -> Result<(), String> {
    if build_file(&format!("SHEET.{form}"), png)? != tiles {
        return Err(format!("the drawing does not build the same tiles as .{form}"));
    }
    Ok(())
}

/// The last word on a drawing: read as `form` and laid out again as each
/// source picture was, in turn, it gives back every pixel of each, every
/// palette index whole.
fn keeps_the_pixels(png: &[u8], form: &str, sources: &[Picture]) -> Result<(), String> {
    let drawing = Picture::read("the drawing", png, form)?;
    let mut rest = drawing.tiles.as_slice();
    for source in sources {
        let (tiles, after) = rest.split_at(source.tiles.len().min(rest.len()));
        let kept = laid_out(tiles, source.form, Some(source.sprites_wide()))
            .is_ok_and(|(pixels, _)| pixels == source.pixels);
        if !kept {
            return Err(format!(
                "the drawing does not give back every pixel of {} as .{}",
                source.name, source.form
            ));
        }
        rest = after;
    }
    if !rest.is_empty() {
        return Err("the drawing holds more tiles than its source".into());
    }
    Ok(())
}

fn obj_shape(name: &str) -> Option<(usize, usize)> {
    let (wide, high) = name.split_once('x')?;
    let shape = (
        wide.parse::<usize>().ok()? / 8,
        high.parse::<usize>().ok()? / 8,
    );
    (crate::assets::image::OBJ_SHAPES.contains(&shape)
        && format!("{}x{}", shape.0 * 8, shape.1 * 8) == name)
        .then_some(shape)
}

fn image_form(built: &str, form: &str, png: &[u8]) -> Result<Vec<u8>, String> {
    let image = indexed_bitmap_png(png).map_err(|error| format!("{built}: {}", error.0))?;
    let (width, height) = (image.width as usize, image.height as usize);
    Ok(match form {
        "gbapal" => bgr555_palette_of(&image).map_err(|error| format!("{built}: {}", error.0))?,
        "bitmap" => indices(&image),
        "strips" => strip_picture(built, &image)?,
        form if form.starts_with("4bpp") || form.starts_with("8bpp") => {
            let (bpp, (wide, high)) = tile_form(form)
                .ok_or_else(|| format!("{built}: .{form} names no OBJ shape"))?;
            metatiles(&indices(&image), width, height, bpp, wide, high)?
        }
        "frames" => sprite_bank(built, &indices(&image), width, height)?,
        "icons" => icon_bank(built, &indices(&image), width, height)?,
        "icon4" => {
            encode_icon4(&indices(&image)).map_err(|error| format!("{built}: {}", error.0))?
        }
        "1bpp" => one_bpp(built, &indices(&image), width, height)?,
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

/// 8x8 tiles in sheet order, one byte per tile row with the leftmost pixel
/// in bit 7, as pret's gbagfx writes `.1bpp`; any non-zero index is set.
fn one_bpp(built: &str, pixels: &[u8], width: usize, height: usize) -> Result<Vec<u8>, String> {
    if !width.is_multiple_of(8) || !height.is_multiple_of(8) || pixels.len() != width * height {
        return Err(format!("{built}: 1bpp sheets are whole 8x8 tiles"));
    }
    let mut out = Vec::with_capacity(pixels.len() / 8);
    for tile_y in (0..height).step_by(8) {
        for tile_x in (0..width).step_by(8) {
            for y in tile_y..tile_y + 8 {
                let row = &pixels[y * width + tile_x..y * width + tile_x + 8];
                out.push(row.iter().fold(0u8, |byte, &pixel| byte << 1 | u8::from(pixel != 0)));
            }
        }
    }
    Ok(out)
}

/// The bytes Camelot's packer saw, when a patch file beside the asset lists
/// edits made after packing (W3); `None` when the asset has no patch.
fn packed_bytes(
    built: &str,
    pixels: &[u8],
    sibling: &dyn Fn(&str) -> Result<Vec<u8>, String>,
) -> Result<Option<Vec<u8>>, String> {
    let name = patch_name(&input_name(built)?);
    match sibling(&name) {
        Ok(text) => Ok(Some(Patch::parse(&name, &text)?.packed(built, pixels)?)),
        Err(_) => Ok(None),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn one_bpp_packs_tiles_in_sheet_order_leftmost_pixel_high() {
        let mut pixels = vec![0u8; 16 * 8];
        pixels[0] = 1; // tile 0, row 0, x 0
        pixels[7] = 3; // tile 0, row 0, x 7
        pixels[16 + 1] = 1; // tile 0, row 1, x 1
        pixels[7 * 16 + 8] = 2; // tile 1, row 7, x 0
        let bytes = one_bpp("T.1bpp", &pixels, 16, 8).unwrap();
        assert_eq!(bytes.len(), 16);
        assert_eq!(&bytes[..2], &[0x81, 0x40]);
        assert_eq!(bytes[15], 0x80);
        assert!(bytes[2..15].iter().all(|&byte| byte == 0));
        assert!(one_bpp("T.1bpp", &pixels[..120], 15, 8).is_err());
        assert!(is_recipe("graphics/display/staff.1bpp"));
        assert!(!is_recipe("graphics/display/staff.2bpp"));
    }
    #[test]
    fn a_frame_bank_numbers_a_slot_it_never_drew() {
        // Four 2x2 frames; the third is drawn wholly in index 255.
        let mut pixels = vec![1u8; 4];
        pixels.extend([2u8; 4]);
        pixels.extend([0xffu8; 4]);
        pixels.extend([0u8, 3, 3, 0]);
        let bank = sprite_bank("T.frames", &pixels, 2, 8).unwrap();
        let word =
            |index: usize| u32::from_le_bytes(bank[4 * index..4 * index + 4].try_into().unwrap());
        // odd frames first: 1 and 3, then 0; slot 2 points at the bank's end
        assert_eq!([word(1), word(3), word(0), word(2), word(4)], [20, 25, 30, 35, 0]);
        assert_eq!(&bank[20..], &[2, 2, 2, 2, 0, 0xe0, 3, 3, 0xe0, 0, 1, 1, 1, 1, 0]);
        // a frame that only touches index 255 is still refused
        pixels[8] = 1;
        assert!(sprite_bank("T.frames", &pixels, 2, 8).is_err());
    }
    #[test]
    fn a_strip_picture_packs_each_row_band_behind_its_offset() {
        // 16x16, three colours: the top band is colour 1, the bottom band
        // colour 2 with one pixel of colour 0 at the left of its last row.
        let mut pixels = vec![1u8; 16 * 8];
        pixels.extend(vec![2u8; 16 * 8]);
        pixels[16 * 15] = 0;
        let palette = [0u8, 0, 0x1f, 0, 0xe0, 0x03];
        let png = crate::assets::image::png_from_bitmap(&pixels, &palette, 16).unwrap();
        let built = build_file("P.strips", &png).unwrap();
        let word = |at: usize| u32::from_le_bytes(built[at..at + 4].try_into().unwrap()) as usize;
        assert_eq!(word(0), 12, "the palette follows three offsets");
        assert_eq!(built[12..18], palette);
        assert_eq!(word(4), 18);
        let top = compress_tagged_packed(&[1u8; 128], &[1u8; 128], &PACKER).unwrap();
        assert_eq!(built[18..18 + top.len()], top[..]);
        assert_eq!(word(8), 18 + top.len());
        // The bottom band's tiles are left to right: the odd pixel is byte 56.
        let mut tiles = vec![2u8; 128];
        tiles[56] = 0;
        let bottom = compress_tagged_packed(&tiles, &tiles, &PACKER).unwrap();
        assert_eq!(built[word(8)..], bottom[..]);
        assert!(is_recipe("graphics/title/picture.strips"));
        let odd = crate::assets::image::png_from_bitmap(&pixels[..16 * 12], &palette, 16).unwrap();
        assert!(build_file("P.strips", &odd).unwrap_err().contains("whole 8x8 tiles"));
    }

    /// Two 16x16 pictures, a box of colour 1 and a box of colour 2, drawn
    /// one above the other as a plain sheet.
    fn two_boxes_stacked() -> Vec<u8> {
        (0..16 * 32)
            .map(|index| {
                let (x, y) = (index % 16, index / 16 % 16);
                let colour = 1 + (index / (16 * 16)) as u8;
                if x == 0 || y == 0 || x == 15 || y == 15 { colour } else { 0 }
            })
            .collect()
    }

    /// An indexed PNG of `depth` bits, `width` wide, in sixteen colours whose
    /// blue rises by `blue` a step, colour 0 transparent when `clear`.
    fn indexed(pixels: &[u8], width: usize, depth: png::BitDepth, blue: u8, clear: bool) -> Vec<u8> {
        let rgb: Vec<u8> = (0..16u8).flat_map(|colour| [colour * 8, colour * 8, colour * blue]).collect();
        crate::assets::image::encode_png(
            width,
            pixels.len() / width,
            png::ColorType::Indexed,
            depth,
            Some(&rgb),
            clear.then_some(&[0u8][..]),
            &crate::assets::image::packed_rows(pixels, width, depth),
        )
        .unwrap()
    }

    fn bit_depth(png: &[u8]) -> png::BitDepth {
        png::Decoder::new(std::io::Cursor::new(png)).read_info().unwrap().info().bit_depth
    }

    #[test]
    fn tile_forms_name_their_depth_and_hardware_shape() {
        assert_eq!(tile_form("8bpp64x32"), Some((GbaBpp::Bpp8, (8, 4))));
        assert_eq!(tile_form("4bpp"), Some((GbaBpp::Bpp4, (1, 1))));
        assert_eq!(tile_form("4bpp24x8"), None, "no OBJ is 24 wide");
        assert_eq!(tile_form("4bpp08x08"), None, "a shape is written as the build reads it");
        assert_eq!(tile_form("bitmap"), None);
    }

    #[test]
    fn a_stacked_sheet_redraws_as_its_sprites_side_by_side() {
        let palette: Vec<u8> = (0..16u16).flat_map(|colour| (colour * 0x0421).to_le_bytes()).collect();
        let png = crate::assets::image::png_from_bitmap(&two_boxes_stacked(), &palette, 16).unwrap();
        let tiles = build_file("S.4bpp16x16", &png).unwrap();
        assert_eq!(tiles, build_file("S.4bpp", &png).unwrap(), "one sprite wide, both forms agree");
        let row = redraw(&png, "4bpp16x16", "4bpp16x16", None).unwrap();
        assert_eq!(redraw(&png, "4bpp", "4bpp16x16", None).unwrap(), row);
        let image = indexed_bitmap_png(&row).unwrap();
        assert_eq!((image.width, image.height), (32, 16));
        assert_eq!((image.pixels[0], image.pixels[16], image.pixels[17]), (1, 2, 2));
        assert_eq!(image.palette, indexed_bitmap_png(&png).unwrap().palette);
        assert_eq!(build_file("S.4bpp16x16", &row).unwrap(), tiles);
        assert_ne!(build_file("S.4bpp", &row).unwrap(), tiles, "a plain sheet reads rows across both");
        let column = redraw(&row, "4bpp16x16", "4bpp16x16", Some(1)).unwrap();
        assert_eq!(indexed_bitmap_png(&column).unwrap().pixels, indexed_bitmap_png(&png).unwrap().pixels);
    }

    #[test]
    fn a_4bpp_png_redraws_at_its_own_depth_and_back() {
        let png = indexed(&two_boxes_stacked(), 16, png::BitDepth::Four, 8, false);
        assert_eq!(bit_depth(&png), png::BitDepth::Four);
        let row = redraw(&png, "4bpp16x16", "4bpp16x16", None).unwrap();
        assert_eq!(bit_depth(&row), png::BitDepth::Four);
        assert_eq!(indexed_bitmap_png(&row).unwrap().width, 32);
        assert_eq!(build_file("S.4bpp16x16", &row).unwrap(), build_file("S.4bpp16x16", &png).unwrap());
        assert_eq!(redraw(&row, "4bpp16x16", "4bpp16x16", Some(1)).unwrap(), png, "and back, byte for byte");
    }

    #[test]
    fn a_redrawing_keeps_the_transparency() {
        let png = indexed(&two_boxes_stacked(), 16, png::BitDepth::Eight, 8, true);
        let row = redraw(&png, "4bpp16x16", "4bpp16x16", None).unwrap();
        let (plte, trns) = crate::assets::image::png_palette(&row).unwrap();
        assert_eq!(trns, [0]);
        assert_eq!((plte, trns), crate::assets::image::png_palette(&png).unwrap());
        assert!(indexed_bitmap_png(&row).unwrap().has_transparency);
    }

    #[test]
    fn a_png_redraws_only_from_a_tile_form_of_the_same_depth() {
        let png = indexed(&two_boxes_stacked(), 16, png::BitDepth::Eight, 8, false);
        assert!(redraw(&png, "bitmap", "4bpp16x16", None).unwrap_err().contains(".bitmap is not a tile form"));
        assert!(redraw(&png, "4bpp16x16", "frames", None).unwrap_err().contains(".frames is not a tile form"));
        assert!(redraw(&png, "4bpp16x16", "8bpp16x16", None).unwrap_err().contains("differ in depth"));
        assert!(redraw(&png, "8bpp", "4bpp16x16", None).unwrap_err().contains("differ in depth"));
        assert!(redraw(&png, "8bpp", "8bpp16x16", None).is_ok(), "an 8bpp sheet of the same picture");
    }

    /// The two boxes as the parts A and B of a part list, beside it.
    fn two_parts(b_blue: u8, b_clear: bool) -> impl Fn(&str) -> Result<Vec<u8>, String> {
        let pixels = two_boxes_stacked();
        let (top, bottom) = pixels.split_at(16 * 16);
        let a = indexed(top, 16, png::BitDepth::Eight, 8, false);
        let b = indexed(bottom, 16, png::BitDepth::Eight, b_blue, b_clear);
        move |name: &str| match name.to_ascii_lowercase().as_str() {
            "a.png" => Ok(a.clone()),
            "b.png" => Ok(b.clone()),
            _ => Err(format!("no {name}")),
        }
    }

    #[test]
    fn a_part_list_redraws_as_one_sheet_of_its_tiles() {
        let sibling = two_parts(8, false);
        let list = b"A\t4bpp16x16\nB\t4bpp\n";
        let row = redraw_parts(list, "4bpp16x16", None, None, &sibling).unwrap();
        assert_eq!(build_file("S.4bpp16x16", &row).unwrap(), build_file_with("L.parts", list, &sibling).unwrap());
        let sheet = indexed(&two_boxes_stacked(), 16, png::BitDepth::Eight, 8, false);
        assert_eq!(row, redraw(&sheet, "4bpp", "4bpp16x16", None).unwrap());
    }

    #[test]
    fn a_part_list_redraws_only_tile_parts_of_the_same_depth() {
        let sibling = two_parts(8, false);
        let refused = |list: &str, to: &str| redraw_parts(list.as_bytes(), to, None, None, &sibling).unwrap_err();
        assert!(refused("A\t4bpp16x16\nB\t8bpp\n", "4bpp16x16").contains("part B is .8bpp, not the depth of .4bpp16x16"));
        assert!(refused("A\t4bpp\nB\t4bpp\n", "8bpp16x16").contains("part A is .4bpp"));
        assert!(refused("A\t4bpp\nM\ttable\n", "4bpp16x16").contains("no tile part"));
        assert!(refused("A\t4bpp\tMAP\t0\t4\n", "4bpp16x16").contains("no tile part"));
        assert!(refused("A\t4bpp\n", "bitmap").contains(".bitmap is not a tile form"));
        assert!(refused("\n", "4bpp16x16").contains("names no part"));
    }

    #[test]
    fn parts_in_different_palettes_redraw_only_in_a_named_palette() {
        let list = b"A\t4bpp16x16\nB\t4bpp16x16\n";
        for sibling in [two_parts(16, false), two_parts(8, true)] {
            let error = redraw_parts(list, "4bpp16x16", None, None, &sibling).unwrap_err();
            assert!(error.contains("parts A and B differ in palette"), "{error}");
            let b = sibling("b.png").unwrap();
            let row = redraw_parts(list, "4bpp16x16", None, Some(&b), &sibling).unwrap();
            let palette = crate::assets::image::png_palette(&row).unwrap();
            assert_eq!(palette, crate::assets::image::png_palette(&b).unwrap());
        }
    }

    #[test]
    fn a_redrawing_that_cannot_build_the_same_tiles_is_refused() {
        let png = indexed(&two_boxes_stacked(), 16, png::BitDepth::Eight, 8, false);
        let tiles = build_file("S.4bpp16x16", &png).unwrap();
        assert!(redraw(&png, "4bpp16x16", "4bpp16x16", Some(3)).unwrap_err().contains("rows of 3"));
        assert!(redraw(&png, "4bpp16x16", "4bpp64x64", None).unwrap_err().contains("whole 64x64"));
        assert!(redraw(&png, "4bpp16x16", "4bpp24x8", None).unwrap_err().contains("not a tile form"));
        // The drawing is laid out from the sheet's pixels and judged against
        // the sheet's own build. A drawing laid out by the form that checks
        // it builds back, so the check is shown a drawing of another form:
        // the boxes side by side, which a plain sheet reads across both.
        let sheet = Picture::read("the sheet", &png, "4bpp16x16").unwrap();
        let row = drawn_as(&sheet.tiles, &png, "4bpp16x16", None).unwrap();
        assert!(builds_the_same(&row, "4bpp16x16", &tiles).is_ok());
        let error = builds_the_same(&row, "4bpp", &tiles).unwrap_err();
        assert_eq!(error, "the drawing does not build the same tiles as .4bpp");
    }

    /// The two boxes drawn in OBJ palette banks 10 and 13, as a sheet in
    /// 256 colours whose 4bpp tiles the game shows in those palettes: each
    /// index carries its bank in its high four bits, and the lower box is
    /// filled with its bank's colour 0, which a 4bpp build writes as 0.
    fn two_boxes_in_banks() -> Vec<u8> {
        two_boxes_stacked()
            .iter()
            .enumerate()
            .map(|(index, colour)| match (index < 16 * 16, colour) {
                (true, 0) => 0,
                (true, colour) => 0xa0 | colour,
                (false, colour) => 0xd0 | colour,
            })
            .collect()
    }

    /// An 8-bit indexed PNG in 256 colours, `width` wide.
    fn in_banks(pixels: &[u8], width: usize) -> Vec<u8> {
        let rgb: Vec<u8> = (0..=255u8).flat_map(|index| [index & 0xf0, index << 4, 0]).collect();
        crate::assets::image::encode_png(
            width,
            pixels.len() / width,
            png::ColorType::Indexed,
            png::BitDepth::Eight,
            Some(&rgb),
            None,
            pixels,
        )
        .unwrap()
    }

    #[test]
    fn a_sheet_in_palette_banks_redraws_and_back_keeping_every_index() {
        let pixels = two_boxes_in_banks();
        let png = in_banks(&pixels, 16);
        let tiles = build_file("S.4bpp16x16", &png).unwrap();
        let row = redraw(&png, "4bpp16x16", "4bpp16x16", None).unwrap();
        let drawn = indexed_bitmap_png(&row).unwrap();
        assert_eq!((drawn.width, drawn.height), (32, 16));
        assert_eq!((drawn.pixels[0], drawn.pixels[16], drawn.pixels[32 + 17]), (0xa1, 0xd2, 0xd0));
        assert_eq!(build_file("S.4bpp16x16", &row).unwrap(), tiles);
        assert_eq!(redraw(&row, "4bpp16x16", "4bpp16x16", Some(1)).unwrap(), png, "and back, every index");
        assert_eq!(redraw(&png, "4bpp", "4bpp", Some(2)).unwrap(), png, "and as itself");
        // The two boxes as parts of a part list keep their banks the same way.
        let (top, bottom) = pixels.split_at(16 * 16);
        let (a, b) = (in_banks(top, 16), in_banks(bottom, 16));
        let sibling = move |name: &str| match name.to_ascii_lowercase().as_str() {
            "a.png" => Ok(a.clone()),
            "b.png" => Ok(b.clone()),
            _ => Err(format!("no {name}")),
        };
        let list = b"A\t4bpp16x16\nB\t4bpp\n";
        assert_eq!(redraw_parts(list, "4bpp16x16", None, None, &sibling).unwrap(), row);
    }

    #[test]
    fn a_drawing_that_does_not_give_back_its_source_is_refused() {
        let png = in_banks(&two_boxes_in_banks(), 16);
        let tiles = build_file("S.4bpp16x16", &png).unwrap();
        let sheet = Picture::read("the sheet", &png, "4bpp16x16").unwrap();
        let row = drawn_as(&sheet.tiles, &png, "4bpp16x16", None).unwrap();
        assert!(keeps_the_pixels(&row, "4bpp16x16", std::slice::from_ref(&sheet)).is_ok());
        // The row drawn from the built tiles keeps only each index's low four
        // bits: it builds the same tiles, and the last check refuses it.
        let nibbles: Vec<u8> = sheet.tiles.iter().map(|index| index & 0x0f).collect();
        let lost = drawn_as(&nibbles, &png, "4bpp16x16", None).unwrap();
        assert!(builds_the_same(&lost, "4bpp16x16", &tiles).is_ok());
        let error = keeps_the_pixels(&lost, "4bpp16x16", std::slice::from_ref(&sheet)).unwrap_err();
        assert_eq!(error, "the drawing does not give back every pixel of the sheet as .4bpp16x16");
        // A drawing that holds more than its source is refused too.
        let top = Picture::read("the top box", &in_banks(&two_boxes_in_banks()[..16 * 16], 16), "4bpp16x16");
        let error = keeps_the_pixels(&row, "4bpp16x16", &[top.unwrap()]).unwrap_err();
        assert_eq!(error, "the drawing holds more tiles than its source");
    }

    #[test]
    fn a_block_map_leaves_out_the_blocks_written_as_dashes() {
        let row = |left: &str, right: &str| format!("{}\t{}\n", [left; 16].join("\t"), [right; 16].join("\t"));
        let grid = |left: &str, right: &str| row(left, right).repeat(16);
        let both = block_map("M.blocks", grid("1a", "2").as_bytes()).unwrap();
        let word = |bytes: &[u8], at: usize| u32::from_le_bytes(bytes[at..at + 4].try_into().unwrap());
        assert_eq!(word(&both, 0), 8);
        let second = word(&both, 4) as usize;
        // A block left out has offset zero and no stream; the others keep theirs.
        let right = block_map("M.blocks", grid("-", "2").as_bytes()).unwrap();
        assert_eq!((word(&right, 0), word(&right, 4)), (0, 8));
        assert_eq!(right[8..], both[second..]);
        let none = block_map("M.blocks", grid("-", "-").as_bytes()).unwrap();
        assert_eq!(none, [0u8; 8]);
        // A block is whole or left out, never half.
        let half = format!("{}{}", row("-", "2"), row("1", "2").repeat(15));
        assert!(block_map("M.blocks", half.as_bytes()).unwrap_err().contains("left out in part"));
        assert!(block_map("M.blocks", grid("x", "2").as_bytes()).is_err());
    }
}
