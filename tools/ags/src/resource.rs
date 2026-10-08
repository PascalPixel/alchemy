use crate::graphics::{indices, metatiles, MapDrawing};
use crate::lz::{
    compress_mtf4_packed, compress_palette, compress_palette_packed, compress_tagged_packed,
    LzMachine,
};
use crate::patch::{patch_name, Patch};
use crate::assets::compression::{
    encode_delta7, encode_halfword_lz, encode_icon4, encode_tilemap_delta, encode_zero_skip,
};
use crate::assets::image::{bgr555_palette_of, indexed_bitmap_png, GbaBpp};

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
    let tiles = ["4bpp", "8bpp"].iter().any(|bpp| {
        form.strip_prefix(bpp)
            .is_some_and(|shape| shape.is_empty() || obj_shape(shape).is_some())
    });
    let form = tiles
        || data_form(form)
        || matches!(
            form,
            "font" | "gbapal" | "bitmap" | "frames" | "icons" | "icon4" | "glyphs" | "1bpp"
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
}
