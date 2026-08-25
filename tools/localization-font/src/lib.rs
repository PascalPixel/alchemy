pub mod cli;

use std::collections::BTreeSet;
use std::fs;
use std::path::{Path, PathBuf};

use extract_resource::{encode_palette, PaletteGroup, PaletteOperation};
use import_asset::indexed_png;
use serde_json::{Map, Value};

pub const PREFIX_ADDRESS: u32 = 0x0802_9910;
const PREFIX_END: u32 = 0x0803_3e60;
const MTF_LAYOUT: [(u32, u32, usize, bool); 7] =
    [(0x0802_9a10, 0x0802_9acc, 3, true), (0x0802_9acc, 0x0802_9b68, 3, false), (0x0802_9b68, 0x0802_9e00, 13, true), (0x0802_9e00, 0x0802_9ee4, 43, false), (0x0802_9ee4, 0x0802_de88, 255, true), (0x0802_de88, 0x0803_08a0, 160, true), (0x0803_08a0, 0x0803_10a4, 32, true)];
const TILE0_ADDRESS: u32 = 0x0802_9910;
const TILE0_END: u32 = 0x0802_9a10;
const UI_TILE_ADDRESS: u32 = 0x0803_10a4;
const UI_TILE_END: u32 = 0x0803_1864;
const PACKED_ADDRESS: u32 = 0x0803_1864;
const PACKED_END: u32 = 0x0803_2224;
const FONT_ADDRESS: u32 = 0x0803_2224;
const FONT_END: u32 = 0x0803_3e24;
const ARTICLE_DATA_ADDRESS: u32 = 0x0803_3e24;
const ARTICLE_TABLE_ADDRESS: u32 = 0x0803_3e40;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);
impl std::fmt::Display for Error {
    fn fmt(&self, out: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        out.write_str(&self.0)
    }
}
impl std::error::Error for Error {}
fn err<T>(message: impl Into<String>) -> Result<T, Error> {
    Err(Error(message.into()))
}
fn object<'a>(value: &'a Value, label: &str) -> Result<&'a Map<String, Value>, Error> {
    value.as_object().ok_or_else(|| Error(format!("{label} must be an object")))
}
fn exact_keys(value: &Map<String, Value>, keys: &[&str], label: &str) -> Result<(), Error> {
    let mut actual: Vec<&str> = value.keys().map(String::as_str).collect();
    let mut expected = keys.to_vec();
    actual.sort_unstable();
    expected.sort_unstable();
    if actual != expected {
        return err(format!("{label} has unknown fields"));
    }
    Ok(())
}
fn usize_field(value: &Map<String, Value>, name: &str, label: &str) -> Result<usize, Error> {
    value.get(name).and_then(Value::as_u64).and_then(|n| usize::try_from(n).ok()).ok_or_else(|| Error(format!("{label} metadata differs")))
}
fn bool_field(value: &Map<String, Value>, name: &str, label: &str) -> Result<bool, Error> {
    value.get(name).and_then(Value::as_bool).ok_or_else(|| Error(format!("{label} metadata differs")))
}
fn string_field<'a>(value: &'a Map<String, Value>, name: &str, label: &str) -> Result<&'a str, Error> {
    value.get(name).and_then(Value::as_str).ok_or_else(|| Error(format!("{label} metadata differs")))
}
fn array<'a>(value: &'a Map<String, Value>, name: &str, label: &str) -> Result<&'a [Value], Error> {
    value.get(name).and_then(Value::as_array).map(Vec::as_slice).ok_or_else(|| Error(format!("{label} collections differ")))
}
fn address(value: &Value, label: &str) -> Result<u32, Error> {
    value.as_str().and_then(|text| text.strip_prefix("0x")).and_then(|text| u32::from_str_radix(text, 16).ok()).ok_or_else(|| Error(format!("{label} address differs")))
}
fn path(root: &Path, source: &str) -> Result<PathBuf, Error> {
    let tail = source.strip_prefix("graphics/fonts/localization_font/").or_else(|| source.strip_prefix("graphics/fonts_localization_font_")).ok_or_else(|| Error("localization-font image path differs".into()))?;
    if tail.is_empty() || tail.contains('/') || !tail.bytes().all(|b| b.is_ascii_lowercase() || b.is_ascii_digit() || b"_.-".contains(&b)) {
        return err("localization-font image path differs");
    }
    Ok(if source.starts_with("graphics/fonts/localization_font/") { root.join(format!("graphics/fonts_localization_font_{tail}")) } else { root.join(source) })
}

fn atlas_frames(path: &Path, count: usize, width: usize, height: usize, columns: usize, depth: u8) -> Result<Vec<Vec<u8>>, Error> {
    let image = indexed_png(&fs::read(path).map_err(|e| Error(format!("{}: {e}", path.display())))?).map_err(|e| Error(e.0))?;
    let rows = count.div_ceil(columns);
    if columns == 0 || image.width as usize != columns * width || image.height as usize != rows * height {
        return err(format!("{}: atlas dimensions differ", path.display()));
    }
    if image.pixels.iter().any(|pixel| *pixel >= 1u32 << depth) {
        return err(format!("{}: atlas exceeds {depth}bpp", path.display()));
    }
    let image_width = image.width as usize;
    Ok((0..count)
        .map(|index| {
            let left = index % columns * width;
            let top = index / columns * height;
            let mut frame = vec![0; width * height];
            for y in 0..height {
                for x in 0..width {
                    frame[y * width + x] = image.pixels[(top + y) * image_width + left + x] as u8;
                }
            }
            frame
        })
        .collect())
}

fn pack_tiles(pixels: &[u8], width: usize, height: usize) -> Result<Vec<u8>, Error> {
    if !width.is_multiple_of(8) || !height.is_multiple_of(8) || pixels.len() != width * height || pixels.iter().any(|pixel| *pixel >= 16) {
        return err("4bpp pixels differ");
    }
    let mut output = Vec::with_capacity(width * height / 2);
    for top in (0..height).step_by(8) {
        for left in (0..width).step_by(8) {
            for y in 0..8 {
                for x in (0..8).step_by(2) {
                    output.push(pixels[(top + y) * width + left + x] | pixels[(top + y) * width + left + x + 1] << 4);
                }
            }
        }
    }
    Ok(output)
}

fn encode_pixels(pixels: &[u8]) -> Result<Vec<u8>, Error> {
    let mut mtf: Vec<u8> = (0..16).collect();
    let prefixes = ["00", "010", "011", "100", "101", "110", "11100", "11101", "11110", "1111100", "1111101", "1111110", "111111100", "111111101", "111111110"];
    let mut bits = Vec::new();
    for value in pixels {
        if *value >= 16 {
            return err("F0 image contains a non-4bpp pixel");
        }
        if *value == mtf[0] {
            bits.push(0);
            continue;
        }
        let index = mtf.iter().position(|item| item == value).unwrap();
        bits.push(1);
        bits.extend(prefixes[index - 1].bytes().map(|byte| byte - b'0'));
        let value = mtf.remove(index);
        mtf.insert(0, value);
    }
    bits.extend(std::iter::repeat_n(1, 10));
    bits.extend(std::iter::repeat_n(1, (8 - bits.len() % 8) % 8));
    let mut output = vec![0; bits.len() / 8];
    for (index, bit) in bits.into_iter().enumerate() {
        output[index / 8] |= bit << (index % 8);
    }
    Ok(output)
}

fn parse_source(value: &Value) -> Result<&Map<String, Value>, Error> {
    let source = object(value, "localization-font source")?;
    exact_keys(source, &["format", "kind", "address", "size", "direct_tiles", "mtf_banks", "packed_images", "font", "articles"], "localization-font source")?;
    if source.get("format").and_then(Value::as_u64) != Some(1)
        || source.get("kind").and_then(Value::as_str) != Some("golden-sun-localization-font")
        || address(source.get("address").unwrap(), "source")? != PREFIX_ADDRESS
        || address(source.get("size").unwrap(), "source")? != PREFIX_END - PREFIX_ADDRESS
    {
        return err("localization-font source layout differs");
    }
    let direct = array(source, "direct_tiles", "localization-font source")?;
    let banks = array(source, "mtf_banks", "localization-font source")?;
    if direct.len() != 2 || banks.len() != MTF_LAYOUT.len() {
        return err("localization-font source collections differ");
    }
    for item in direct {
        exact_keys(object(item, "direct-tile source")?, &["address", "end", "source", "columns"], "direct-tile source")?;
    }
    for item in banks {
        exact_keys(object(item, "MTF-bank source")?, &["address", "end", "entries", "terminal", "images", "columns", "tail_zeros", "source"], "MTF-bank source")?;
    }
    exact_keys(object(source.get("packed_images").unwrap(), "packed-image source")?, &["address", "end", "images", "columns", "tail_zeros", "source", "tokens"], "packed-image source")?;
    exact_keys(object(source.get("font").unwrap(), "font source")?, &["address", "first_code", "glyphs", "columns", "source", "advances"], "font source")?;
    let articles = object(source.get("articles").unwrap(), "article source")?;
    exact_keys(articles, &["data_address", "table_address", "data_size", "records", "entries"], "article source")?;
    for item in array(articles, "records", "article source")? {
        exact_keys(object(item, "article record")?, &["offset", "text"], "article record")?;
    }
    Ok(source)
}

fn numbers(value: &Map<String, Value>, name: &str, label: &str) -> Result<Vec<usize>, Error> {
    array(value, name, label)?.iter().map(|item| item.as_u64().and_then(|n| usize::try_from(n).ok()).ok_or_else(|| Error(format!("{label} differs")))).collect()
}

fn build_direct(source: &Map<String, Value>, root: &Path) -> Result<Vec<Vec<u8>>, Error> {
    let expected = [(TILE0_ADDRESS, TILE0_END, 2), (UI_TILE_ADDRESS, UI_TILE_END, 2)];
    let mut output = Vec::new();
    for (index, item) in array(source, "direct_tiles", "localization-font source")?.iter().enumerate() {
        let item = object(item, "direct-tile source")?;
        let (start, end, columns) = expected[index];
        if address(item.get("address").unwrap(), "direct tile")? != start || address(item.get("end").unwrap(), "direct tile end")? != end || usize_field(item, "columns", "direct-tile")? != columns {
            return err("direct-tile source extent differs");
        }
        let bytes = fs::read(path(root, string_field(item, "source", "direct-tile")?)?).map_err(|e| Error(e.to_string()))?;
        let image = indexed_png(&bytes).map_err(|e| Error(e.0))?;
        let raw = import_asset::gba_graphics(&bytes, 4.0).map_err(|e| Error(e.0))?.0;
        if image.width % 8 != 0 || raw.len() != (end - start) as usize {
            return err("direct-tile source extent differs");
        }
        output.push(raw);
    }
    Ok(output)
}

fn palette_group(value: &Value) -> Result<PaletteGroup, Error> {
    let items = value.as_array().ok_or_else(|| Error("packed-image tokens differ".into()))?;
    match (items.first().and_then(Value::as_str), items.len()) {
        (Some("z"), 1) => Ok(PaletteGroup::Zeros),
        (Some("g"), 2) => {
            let mut operations = Vec::new();
            for item in items[1].as_array().ok_or_else(|| Error("packed-image tokens differ".into()))? {
                let item = item.as_array().ok_or_else(|| Error("packed-image tokens differ".into()))?;
                match (item.first().and_then(Value::as_str), item.get(1).and_then(Value::as_u64), item.get(2).and_then(Value::as_u64)) {
                    (Some("l"), None, None) => operations.push(PaletteOperation::Literal),
                    (Some("e"), None, None) => operations.push(PaletteOperation::End),
                    (Some("c"), Some(length), Some(distance)) => operations.push(PaletteOperation::Copy { length: length as u32, distance: distance as u32 }),
                    _ => return err("packed-image tokens differ"),
                }
            }
            Ok(PaletteGroup::Group(operations))
        }
        _ => err("packed-image tokens differ"),
    }
}

fn build_mtf(source: &Map<String, Value>, root: &Path) -> Result<Vec<Vec<u8>>, Error> {
    let mut output = Vec::new();
    for (index, item) in array(source, "mtf_banks", "localization-font source")?.iter().enumerate() {
        let item = object(item, "MTF-bank source")?;
        let (start, end, entry_count, terminal) = MTF_LAYOUT[index];
        let entries = numbers(item, "entries", "MTF-bank")?;
        let images = usize_field(item, "images", "MTF-bank")?;
        let columns = usize_field(item, "columns", "MTF-bank")?;
        let tail = usize_field(item, "tail_zeros", "MTF-bank")?;
        if address(item.get("address").unwrap(), "MTF bank")? != start
            || address(item.get("end").unwrap(), "MTF bank end")? != end
            || entries.len() != entry_count
            || bool_field(item, "terminal", "MTF-bank")? != terminal
            || images == 0
            || entries.iter().any(|entry| *entry >= images)
            || entries.iter().collect::<BTreeSet<_>>().len() != images
        {
            return err(format!("MTF bank {index} metadata differs"));
        }
        let frames = atlas_frames(&path(root, string_field(item, "source", "MTF-bank")?)?, images, 16, 16, columns, 4)?;
        let streams: Vec<Vec<u8>> = frames.iter().map(|frame| encode_pixels(frame)).collect::<Result<_, _>>()?;
        let header_size = entries.len() * 4 + usize::from(terminal) * 4;
        let mut pointers = Vec::new();
        let mut cursor = start + header_size as u32;
        for stream in &streams {
            pointers.push(cursor);
            cursor += stream.len() as u32;
        }
        let mut header = vec![0; header_size];
        for (slot, entry) in entries.iter().enumerate() {
            header[slot * 4..slot * 4 + 4].copy_from_slice(&pointers[*entry].to_le_bytes());
        }
        if terminal {
            header[entries.len() * 4..entries.len() * 4 + 4].copy_from_slice(&u32::MAX.to_le_bytes());
        }
        let mut built = header;
        for stream in streams {
            built.extend(stream);
        }
        built.extend(std::iter::repeat_n(0, tail));
        if built.len() != (end - start) as usize || cursor + tail as u32 != end {
            return err(format!("MTF bank {index} extent differs"));
        }
        output.push(built);
    }
    Ok(output)
}

fn build_packed(source: &Map<String, Value>, root: &Path) -> Result<Vec<u8>, Error> {
    let packed = object(source.get("packed_images").unwrap(), "packed-image source")?;
    let images = usize_field(packed, "images", "packed-image")?;
    let columns = usize_field(packed, "columns", "packed-image")?;
    let tail = usize_field(packed, "tail_zeros", "packed-image")?;
    if address(packed.get("address").unwrap(), "packed image")? != PACKED_ADDRESS || address(packed.get("end").unwrap(), "packed image end")? != PACKED_END || images != 8 {
        return err("packed-image metadata differs");
    }
    let tokens: Vec<Vec<PaletteGroup>> = array(packed, "tokens", "packed-image")?.iter().map(|value| value.as_array().ok_or_else(|| Error("packed-image tokens differ".into()))?.iter().map(palette_group).collect()).collect::<Result<_, _>>()?;
    if tokens.len() != images {
        return err("packed-image metadata differs");
    }
    let frames = atlas_frames(&path(root, string_field(packed, "source", "packed-image")?)?, images, 32, 32, columns, 4)?;
    let mut streams = Vec::new();
    for (index, frame) in frames.iter().enumerate() {
        streams.push(encode_palette(&pack_tiles(frame, 32, 32)?, &tokens[index]).map_err(|e| Error(e.0))?);
    }
    let mut header = vec![0; images * 4];
    let mut cursor = PACKED_ADDRESS + 32;
    for (index, stream) in streams.iter().enumerate() {
        header[index * 4..index * 4 + 4].copy_from_slice(&cursor.to_le_bytes());
        cursor += stream.len() as u32;
    }
    let mut output = header;
    for stream in streams {
        output.extend(stream);
    }
    output.extend(std::iter::repeat_n(0, tail));
    if output.len() != (PACKED_END - PACKED_ADDRESS) as usize || cursor + tail as u32 != PACKED_END {
        return err("packed-image extent differs");
    }
    Ok(output)
}

fn build_font(source: &Map<String, Value>, root: &Path) -> Result<Vec<u8>, Error> {
    let font = object(source.get("font").unwrap(), "font source")?;
    let advances = numbers(font, "advances", "font")?;
    if address(font.get("address").unwrap(), "font")? != FONT_ADDRESS || usize_field(font, "first_code", "font")? != 32 || usize_field(font, "glyphs", "font")? != 224 || advances.len() != 224 || advances.iter().any(|value| *value > 0xffff) {
        return err("font metadata differs");
    }
    let frames = atlas_frames(&path(root, string_field(font, "source", "font")?)?, 224, 16, 16, usize_field(font, "columns", "font")?, 1)?;
    let mut output = vec![0; 224 * 32];
    for (index, frame) in frames.iter().enumerate() {
        if frame[15 * 16..].iter().any(|pixel| *pixel != 0) {
            return err("font atlas padding row is not blank");
        }
        output[index * 32..index * 32 + 2].copy_from_slice(&(advances[index] as u16).to_le_bytes());
        for y in 0..15 {
            let mut row = 0u16;
            for x in 0..16 {
                row |= u16::from(frame[y * 16 + x]) << (15 - x);
            }
            output[index * 32 + 2 + y * 2..index * 32 + 4 + y * 2].copy_from_slice(&row.to_le_bytes());
        }
    }
    if output.len() != (FONT_END - FONT_ADDRESS) as usize {
        return err("font extent differs");
    }
    Ok(output)
}

fn build_articles(source: &Map<String, Value>) -> Result<Vec<u8>, Error> {
    let article = object(source.get("articles").unwrap(), "article source")?;
    let data_size = (ARTICLE_TABLE_ADDRESS - ARTICLE_DATA_ADDRESS) as usize;
    if address(article.get("data_address").unwrap(), "article data")? != ARTICLE_DATA_ADDRESS || address(article.get("table_address").unwrap(), "article table")? != ARTICLE_TABLE_ADDRESS || usize_field(article, "data_size", "article")? != data_size {
        return err("article metadata differs");
    }
    let records = array(article, "records", "article")?;
    let mut data = vec![0; data_size];
    for record in records {
        let record = object(record, "article record")?;
        let offset = usize_field(record, "offset", "article record")?;
        let text = string_field(record, "text", "article record")?;
        if !text.bytes().all(|byte| (0x20..=0x7e).contains(&byte)) || offset + text.len() + 1 > data.len() {
            return err("article record differs");
        }
        data[offset..offset + text.len()].copy_from_slice(text.as_bytes());
    }
    let entries = numbers(article, "entries", "article")?;
    if entries.len() != 8 {
        return err("article metadata differs");
    }
    let mut table = vec![0; entries.len() * 4];
    for (index, entry) in entries.iter().enumerate() {
        if *entry >= records.len() {
            return err("article entry differs");
        }
        let offset = usize_field(object(&records[*entry], "article record")?, "offset", "article record")?;
        table[index * 4..index * 4 + 4].copy_from_slice(&(ARTICLE_DATA_ADDRESS + offset as u32).to_le_bytes());
    }
    data.extend(table);
    Ok(data)
}

pub fn build_localization_font(value: &Value, root: &Path) -> Result<Vec<u8>, Error> {
    let source = parse_source(value)?;
    let direct = build_direct(source, root)?;
    let mtf = build_mtf(source, root)?;
    let packed = build_packed(source, root)?;
    let font = build_font(source, root)?;
    let articles = build_articles(source)?;
    let mut result = direct[0].clone();
    for bank in mtf {
        result.extend(bank);
    }
    result.extend(&direct[1]);
    result.extend(packed);
    result.extend(font);
    result.extend(articles);
    if result.len() != (PREFIX_END - PREFIX_ADDRESS) as usize {
        return err("localization-font package extent differs");
    }
    Ok(result)
}
