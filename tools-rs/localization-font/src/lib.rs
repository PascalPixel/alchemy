//! Native exporter and builder for the Golden Sun localization font package.
//!
//! The source format deliberately stays a JSON `Value`: this keeps the Rust
//! tool able to validate the same extensible document shape as the TypeScript
//! implementation while preserving the tracked field order on export.

use std::collections::BTreeMap;
use std::fs;
use std::path::{Path, PathBuf};

use alchemy_zlib::{deflate_sync, DeflateOptions};
use canonical_json::canonical_json;
use export_asset::chunk;
use extract_resource::{decode_palette_trace, encode_palette, PaletteGroup, PaletteOperation};
use import_asset::indexed_png;
use serde_json::{Map, Value};

pub const ROM_BASE: u32 = 0x0800_0000;
pub const PREFIX_ADDRESS: u32 = 0x0802_9910;
pub const PREFIX_END: u32 = 0x0803_3e60;

const MTF_LAYOUT: [(u32, u32, usize, bool); 7] = [
    (0x0802_9a10, 0x0802_9acc, 3, true),
    (0x0802_9acc, 0x0802_9b68, 3, false),
    (0x0802_9b68, 0x0802_9e00, 13, true),
    (0x0802_9e00, 0x0802_9ee4, 43, false),
    (0x0802_9ee4, 0x0802_de88, 255, true),
    (0x0802_de88, 0x0803_08a0, 160, true),
    (0x0803_08a0, 0x0803_10a4, 32, true),
];
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

fn hex(value: u32) -> String {
    format!("0x{value:08x}")
}

fn parse_hex(value: &Value, label: &str) -> Result<u32, Error> {
    let text = value
        .as_str()
        .ok_or_else(|| Error(format!("{label} address differs")))?;
    let digits = text
        .strip_prefix("0x")
        .ok_or_else(|| Error(format!("{label} address differs")))?;
    u32::from_str_radix(digits, 16).map_err(|_| Error(format!("{label} address differs")))
}

fn object<'a>(value: &'a Value, label: &str) -> Result<&'a Map<String, Value>, Error> {
    value
        .as_object()
        .ok_or_else(|| Error(format!("{label} must be an object")))
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
    value
        .get(name)
        .and_then(Value::as_u64)
        .and_then(|n| usize::try_from(n).ok())
        .ok_or_else(|| Error(format!("{label} metadata differs")))
}

fn bool_field(value: &Map<String, Value>, name: &str, label: &str) -> Result<bool, Error> {
    value
        .get(name)
        .and_then(Value::as_bool)
        .ok_or_else(|| Error(format!("{label} metadata differs")))
}

fn string_field<'a>(
    value: &'a Map<String, Value>,
    name: &str,
    label: &str,
) -> Result<&'a str, Error> {
    value
        .get(name)
        .and_then(Value::as_str)
        .ok_or_else(|| Error(format!("{label} metadata differs")))
}

fn array<'a>(
    value: &'a Map<String, Value>,
    name: &str,
    label: &str,
) -> Result<&'a Vec<Value>, Error> {
    value
        .get(name)
        .and_then(Value::as_array)
        .ok_or_else(|| Error(format!("{label} collections differ")))
}

fn source_path(root: &Path, source: &str) -> Result<PathBuf, Error> {
    let prefix = "graphics/fonts/localization_font/";
    let (path, tail) = if let Some(tail) = source.strip_prefix(prefix) {
        (
            root.join(format!("graphics/fonts_localization_font_{tail}")),
            tail,
        )
    } else if let Some(tail) = source.strip_prefix("graphics/fonts_localization_font_") {
        (root.join(source), tail)
    } else {
        return err("localization-font image path differs");
    };
    if tail.is_empty()
        || tail.contains('/')
        || !tail
            .bytes()
            .all(|b| b.is_ascii_lowercase() || b.is_ascii_digit() || b"_.-".contains(&b))
    {
        return err("localization-font image path differs");
    }
    Ok(path)
}

fn read_range(rom: &[u8], start: u32, end: u32) -> Result<Vec<u8>, Error> {
    let first = start
        .checked_sub(ROM_BASE)
        .ok_or_else(|| Error("ROM range differs".into()))? as usize;
    let last = end
        .checked_sub(ROM_BASE)
        .ok_or_else(|| Error("ROM range differs".into()))? as usize;
    if end < start || last > rom.len() {
        return err("ROM range differs");
    }
    Ok(rom[first..last].to_vec())
}

fn pointer(rom: &[u8], address: u32) -> Result<u32, Error> {
    let bytes = read_range(rom, address, address + 4)?;
    Ok(u32::from_le_bytes(bytes.try_into().unwrap()))
}

fn png_indexed(pixels: &[u8], width: usize, height: usize, depth: u8) -> Result<Vec<u8>, Error> {
    if width == 0
        || height == 0
        || width % 8 != 0
        || height % 8 != 0
        || pixels.len() != width * height
    {
        return err("indexed image dimensions differ");
    }
    let limit = 1usize << depth;
    if pixels.iter().any(|pixel| usize::from(*pixel) >= limit) {
        return err("indexed image pixel exceeds its depth");
    }
    let palette: Vec<u8> = if depth == 1 {
        vec![0, 0, 0, 255, 255, 255]
    } else {
        (0..16)
            .flat_map(|index| [index * 17, index * 17, index * 17])
            .collect()
    };
    let bytes_per_row = width * usize::from(depth) / 8;
    let mut rows = vec![0u8; height * (bytes_per_row + 1)];
    for y in 0..height {
        let mut target = y * (bytes_per_row + 1) + 1;
        let pixels_per_byte = 8 / usize::from(depth);
        for x in (0..width).step_by(pixels_per_byte) {
            let mut byte = 0u8;
            for part in 0..pixels_per_byte {
                byte |= pixels[y * width + x + part] << (8 - usize::from(depth) * (part + 1));
            }
            rows[target] = byte;
            target += 1;
        }
    }
    let mut ihdr = vec![0u8; 13];
    ihdr[0..4].copy_from_slice(&(width as u32).to_be_bytes());
    ihdr[4..8].copy_from_slice(&(height as u32).to_be_bytes());
    ihdr[8] = depth;
    ihdr[9] = 3;
    let mut result = b"\x89PNG\r\n\x1a\n".to_vec();
    result.extend_from_slice(&chunk(b"IHDR", &ihdr));
    result.extend_from_slice(&chunk(b"PLTE", &palette));
    result.extend_from_slice(&chunk(
        b"IDAT",
        &deflate_sync(&rows, DeflateOptions { level: Some(9) }),
    ));
    result.extend_from_slice(&chunk(b"IEND", &[]));
    Ok(result)
}

fn atlas(
    frames: &[Vec<u8>],
    frame_width: usize,
    frame_height: usize,
    columns: usize,
) -> Result<Vec<u8>, Error> {
    if frames.is_empty()
        || columns == 0
        || frames
            .iter()
            .any(|frame| frame.len() != frame_width * frame_height)
    {
        return err("atlas frame layout differs");
    }
    let rows = frames.len().div_ceil(columns);
    let width = columns * frame_width;
    let height = rows * frame_height;
    let mut pixels = vec![0u8; width * height];
    for (index, frame) in frames.iter().enumerate() {
        let left = index % columns * frame_width;
        let top = index / columns * frame_height;
        for y in 0..frame_height {
            let source = &frame[y * frame_width..(y + 1) * frame_width];
            pixels[(top + y) * width + left..(top + y) * width + left + frame_width]
                .copy_from_slice(source);
        }
    }
    png_indexed(&pixels, width, height, 4)
}

fn atlas_frames(
    path: &Path,
    count: usize,
    frame_width: usize,
    frame_height: usize,
    columns: usize,
    depth: u8,
) -> Result<Vec<Vec<u8>>, Error> {
    let image =
        indexed_png(&fs::read(path).map_err(|e| Error(format!("{}: {e}", path.display())))?)
            .map_err(|e| Error(e.0))?;
    let rows = count.div_ceil(columns);
    if columns == 0
        || image.width as usize != columns * frame_width
        || image.height as usize != rows * frame_height
    {
        return err(format!("{}: atlas dimensions differ", path.display()));
    }
    if image.pixels.iter().any(|pixel| *pixel >= (1u32 << depth)) {
        return err(format!("{}: atlas exceeds {depth}bpp", path.display()));
    }
    let width = image.width as usize;
    Ok((0..count)
        .map(|index| {
            let left = index % columns * frame_width;
            let top = index / columns * frame_height;
            let mut frame = vec![0u8; frame_width * frame_height];
            for y in 0..frame_height {
                for x in 0..frame_width {
                    frame[y * frame_width + x] = image.pixels[(top + y) * width + left + x] as u8;
                }
            }
            frame
        })
        .collect())
}

fn pack_tiles(pixels: &[u8], width: usize, height: usize) -> Result<Vec<u8>, Error> {
    if width % 8 != 0
        || height % 8 != 0
        || pixels.len() != width * height
        || pixels.iter().any(|pixel| *pixel >= 16)
    {
        return err("4bpp pixels differ");
    }
    let mut result = Vec::with_capacity(width * height / 2);
    for top in (0..height).step_by(8) {
        for left in (0..width).step_by(8) {
            for y in 0..8 {
                for x in (0..8).step_by(2) {
                    result.push(
                        pixels[(top + y) * width + left + x]
                            | pixels[(top + y) * width + left + x + 1] << 4,
                    );
                }
            }
        }
    }
    Ok(result)
}

fn unpack_tiles(source: &[u8], width: usize, height: usize) -> Result<Vec<u8>, Error> {
    if width % 8 != 0 || height % 8 != 0 || source.len() != width * height / 2 {
        return err("4bpp tile extent differs");
    }
    let mut pixels = vec![0u8; width * height];
    let mut cursor = 0;
    for top in (0..height).step_by(8) {
        for left in (0..width).step_by(8) {
            for y in 0..8 {
                for x in (0..8).step_by(2) {
                    let byte = source[cursor];
                    cursor += 1;
                    pixels[(top + y) * width + left + x] = byte & 15;
                    pixels[(top + y) * width + left + x + 1] = byte >> 4;
                }
            }
        }
    }
    Ok(pixels)
}

struct BitReader<'a> {
    data: &'a [u8],
    offset: usize,
}
impl<'a> BitReader<'a> {
    fn get(&mut self) -> Result<u8, Error> {
        if self.offset >= self.data.len() * 8 {
            return err("F0 image stream ended before its terminator");
        }
        let value = (self.data[self.offset / 8] >> (self.offset % 8)) & 1;
        self.offset += 1;
        Ok(value)
    }
}

fn selection(bits: &mut BitReader<'_>) -> Result<Option<usize>, Error> {
    if bits.get()? == 0 {
        return Ok(Some(if bits.get()? == 0 {
            1
        } else if bits.get()? == 0 {
            2
        } else {
            3
        }));
    }
    if bits.get()? == 0 {
        return Ok(Some(if bits.get()? == 0 { 4 } else { 5 }));
    }
    if bits.get()? == 0 {
        return Ok(Some(6));
    }
    if bits.get()? == 0 {
        return Ok(Some(if bits.get()? == 0 { 7 } else { 8 }));
    }
    if bits.get()? == 0 {
        return Ok(Some(9));
    }
    if bits.get()? == 0 {
        return Ok(Some(if bits.get()? == 0 { 10 } else { 11 }));
    }
    if bits.get()? == 0 {
        return Ok(Some(12));
    }
    if bits.get()? == 0 {
        return Ok(Some(if bits.get()? == 0 { 13 } else { 14 }));
    }
    if bits.get()? == 0 {
        return Ok(Some(15));
    }
    Ok(None)
}

fn decode_pixels(data: &[u8]) -> Result<(Vec<u8>, usize), Error> {
    let mut bits = BitReader { data, offset: 0 };
    let mut mtf: Vec<u8> = (0..16).collect();
    let mut pixels = Vec::new();
    loop {
        while bits.get()? == 0 {
            pixels.push(mtf[0]);
        }
        match selection(&mut bits)? {
            None => return Ok((pixels, bits.offset)),
            Some(index) => {
                let value = mtf.remove(index);
                mtf.insert(0, value);
                pixels.push(value);
            }
        }
    }
}

fn encode_pixels(pixels: &[u8]) -> Result<Vec<u8>, Error> {
    let mut mtf: Vec<u8> = (0..16).collect();
    let mut bits = Vec::new();
    let prefixes = [
        "00",
        "010",
        "011",
        "100",
        "101",
        "110",
        "11100",
        "11101",
        "11110",
        "1111100",
        "1111101",
        "1111110",
        "111111100",
        "111111101",
        "111111110",
    ];
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
    let mut output = vec![0u8; bits.len() / 8];
    for (index, bit) in bits.into_iter().enumerate() {
        output[index / 8] |= bit << (index % 8);
    }
    Ok(output)
}

fn parse_source(value: &Value) -> Result<&Map<String, Value>, Error> {
    let source = object(value, "localization-font source")?;
    exact_keys(
        source,
        &[
            "format",
            "kind",
            "address",
            "size",
            "direct_tiles",
            "mtf_banks",
            "packed_images",
            "font",
            "articles",
        ],
        "localization-font source",
    )?;
    if source.get("format").and_then(Value::as_u64) != Some(1)
        || source.get("kind").and_then(Value::as_str) != Some("golden-sun-localization-font")
        || parse_hex(source.get("address").unwrap(), "source")? != PREFIX_ADDRESS
        || parse_hex(source.get("size").unwrap(), "source")? != PREFIX_END - PREFIX_ADDRESS
    {
        return err("localization-font source layout differs");
    }
    let direct = array(source, "direct_tiles", "localization-font source")?;
    let banks = array(source, "mtf_banks", "localization-font source")?;
    if direct.len() != 2 || banks.len() != MTF_LAYOUT.len() {
        return err("localization-font source collections differ");
    }
    for item in direct {
        exact_keys(
            object(item, "direct-tile source")?,
            &["address", "end", "source", "columns"],
            "direct-tile source",
        )?;
    }
    for item in banks {
        exact_keys(
            object(item, "MTF-bank source")?,
            &[
                "address",
                "end",
                "entries",
                "terminal",
                "images",
                "columns",
                "tail_zeros",
                "source",
            ],
            "MTF-bank source",
        )?;
    }
    exact_keys(
        object(source.get("packed_images").unwrap(), "packed-image source")?,
        &[
            "address",
            "end",
            "images",
            "columns",
            "tail_zeros",
            "source",
            "tokens",
        ],
        "packed-image source",
    )?;
    exact_keys(
        object(source.get("font").unwrap(), "font source")?,
        &[
            "address",
            "first_code",
            "glyphs",
            "columns",
            "source",
            "advances",
        ],
        "font source",
    )?;
    let articles = object(source.get("articles").unwrap(), "article source")?;
    exact_keys(
        articles,
        &[
            "data_address",
            "table_address",
            "data_size",
            "records",
            "entries",
        ],
        "article source",
    )?;
    for item in array(articles, "records", "article source")? {
        exact_keys(
            object(item, "article record")?,
            &["offset", "text"],
            "article record",
        )?;
    }
    Ok(source)
}

fn field_array_usize(
    value: &Map<String, Value>,
    name: &str,
    label: &str,
) -> Result<Vec<usize>, Error> {
    array(value, name, label)?
        .iter()
        .map(|item| {
            item.as_u64()
                .and_then(|value| usize::try_from(value).ok())
                .ok_or_else(|| Error(format!("{label} differs")))
        })
        .collect()
}

fn build_direct(source: &Map<String, Value>, root: &Path) -> Result<Vec<Vec<u8>>, Error> {
    let expected = [
        (TILE0_ADDRESS, TILE0_END, 2usize),
        (UI_TILE_ADDRESS, UI_TILE_END, 2usize),
    ];
    let mut result = Vec::new();
    for (index, item) in array(source, "direct_tiles", "localization-font source")?
        .iter()
        .enumerate()
    {
        let item = object(item, "direct-tile source")?;
        let (start, end, columns) = expected[index];
        if parse_hex(item.get("address").unwrap(), "direct tile")? != start
            || parse_hex(item.get("end").unwrap(), "direct tile end")? != end
            || usize_field(item, "columns", "direct-tile")? != columns
        {
            return err("direct-tile source extent differs");
        }
        let path = source_path(root, string_field(item, "source", "direct-tile")?)?;
        let bytes = fs::read(&path).map_err(|e| Error(e.to_string()))?;
        let image = indexed_png(&bytes).map_err(|e| Error(e.0))?;
        let raw = import_asset::gba_graphics(&bytes, 4.0)
            .map_err(|e| Error(e.0))?
            .0;
        if image.width % 8 != 0 || raw.len() != (end - start) as usize {
            return err("direct-tile source extent differs");
        }
        result.push(raw);
    }
    Ok(result)
}

fn json_palette_group(value: &Value) -> Result<PaletteGroup, Error> {
    let array = value
        .as_array()
        .ok_or_else(|| Error("packed-image tokens differ".into()))?;
    match array.first().and_then(Value::as_str) {
        Some("z") if array.len() == 1 => Ok(PaletteGroup::Zeros),
        Some("g") if array.len() == 2 => {
            let ops = array[1]
                .as_array()
                .ok_or_else(|| Error("packed-image tokens differ".into()))?;
            let mut result = Vec::new();
            for op in ops {
                let op = op
                    .as_array()
                    .ok_or_else(|| Error("packed-image tokens differ".into()))?;
                match (
                    op.first().and_then(Value::as_str),
                    op.get(1).and_then(Value::as_u64),
                    op.get(2).and_then(Value::as_u64),
                ) {
                    (Some("l"), None, None) => result.push(PaletteOperation::Literal),
                    (Some("e"), None, None) => result.push(PaletteOperation::End),
                    (Some("c"), Some(length), Some(distance)) => {
                        result.push(PaletteOperation::Copy {
                            length: length as u32,
                            distance: distance as u32,
                        })
                    }
                    _ => return err("packed-image tokens differ"),
                }
            }
            Ok(PaletteGroup::Group(result))
        }
        _ => err("packed-image tokens differ"),
    }
}

fn json_palette_tokens(value: &[PaletteGroup]) -> Value {
    Value::Array(
        value
            .iter()
            .map(|group| match group {
                PaletteGroup::Zeros => Value::Array(vec![Value::String("z".into())]),
                PaletteGroup::Group(ops) => Value::Array(vec![
                    Value::String("g".into()),
                    Value::Array(
                        ops.iter()
                            .map(|op| match op {
                                PaletteOperation::Literal => {
                                    Value::Array(vec![Value::String("l".into())])
                                }
                                PaletteOperation::End => {
                                    Value::Array(vec![Value::String("e".into())])
                                }
                                PaletteOperation::Copy { length, distance } => Value::Array(vec![
                                    Value::String("c".into()),
                                    Value::from(*length),
                                    Value::from(*distance),
                                ]),
                            })
                            .collect(),
                    ),
                ]),
            })
            .collect(),
    )
}

fn build_mtf(source: &Map<String, Value>, root: &Path) -> Result<Vec<Vec<u8>>, Error> {
    let mut result = Vec::new();
    for (index, item) in array(source, "mtf_banks", "localization-font source")?
        .iter()
        .enumerate()
    {
        let item = object(item, "MTF-bank source")?;
        let (start, end, entry_count, terminal) = MTF_LAYOUT[index];
        let entries = field_array_usize(item, "entries", "MTF-bank")?;
        let images = usize_field(item, "images", "MTF-bank")?;
        let columns = usize_field(item, "columns", "MTF-bank")?;
        let tail = usize_field(item, "tail_zeros", "MTF-bank")?;
        if parse_hex(item.get("address").unwrap(), "MTF bank")? != start
            || parse_hex(item.get("end").unwrap(), "MTF bank end")? != end
            || entries.len() != entry_count
            || bool_field(item, "terminal", "MTF-bank")? != terminal
            || images == 0
            || entries.iter().any(|entry| *entry >= images)
            || entries
                .iter()
                .collect::<std::collections::BTreeSet<_>>()
                .len()
                != images
        {
            return err(format!("MTF bank {index} metadata differs"));
        }
        let frames = atlas_frames(
            &source_path(root, string_field(item, "source", "MTF-bank")?)?,
            images,
            16,
            16,
            columns,
            4,
        )?;
        let streams: Vec<Vec<u8>> = frames
            .iter()
            .map(|frame| encode_pixels(frame))
            .collect::<Result<_, _>>()?;
        let header_size = entries.len() * 4 + if terminal { 4 } else { 0 };
        let mut pointers = Vec::new();
        let mut cursor = start + header_size as u32;
        for stream in &streams {
            pointers.push(cursor);
            cursor += stream.len() as u32;
        }
        let mut header = vec![0u8; header_size];
        for (slot, entry) in entries.iter().enumerate() {
            header[slot * 4..slot * 4 + 4].copy_from_slice(&pointers[*entry].to_le_bytes());
        }
        if terminal {
            header[entries.len() * 4..entries.len() * 4 + 4]
                .copy_from_slice(&u32::MAX.to_le_bytes());
        }
        let mut built = header;
        for stream in streams {
            built.extend(stream);
        }
        built.extend(std::iter::repeat_n(0, tail));
        if built.len() != (end - start) as usize || cursor + tail as u32 != end {
            return err(format!("MTF bank {index} extent differs"));
        }
        result.push(built);
    }
    Ok(result)
}

fn build_packed(source: &Map<String, Value>, root: &Path) -> Result<Vec<u8>, Error> {
    let packed = object(source.get("packed_images").unwrap(), "packed-image source")?;
    let images = usize_field(packed, "images", "packed-image")?;
    let columns = usize_field(packed, "columns", "packed-image")?;
    let tail = usize_field(packed, "tail_zeros", "packed-image")?;
    if parse_hex(packed.get("address").unwrap(), "packed image")? != PACKED_ADDRESS
        || parse_hex(packed.get("end").unwrap(), "packed image end")? != PACKED_END
        || images != 8
    {
        return err("packed-image metadata differs");
    }
    let tokens: Vec<Vec<PaletteGroup>> = array(packed, "tokens", "packed-image")?
        .iter()
        .map(|value| {
            value
                .as_array()
                .ok_or_else(|| Error("packed-image tokens differ".into()))?
                .iter()
                .map(json_palette_group)
                .collect()
        })
        .collect::<Result<_, _>>()?;
    if tokens.len() != images {
        return err("packed-image metadata differs");
    }
    let frames = atlas_frames(
        &source_path(root, string_field(packed, "source", "packed-image")?)?,
        images,
        32,
        32,
        columns,
        4,
    )?;
    let mut streams = Vec::new();
    for (index, frame) in frames.iter().enumerate() {
        streams.push(
            encode_palette(&pack_tiles(frame, 32, 32)?, &tokens[index]).map_err(|e| Error(e.0))?,
        );
    }
    let mut header = vec![0u8; images * 4];
    let mut cursor = PACKED_ADDRESS + 32;
    for (index, stream) in streams.iter().enumerate() {
        header[index * 4..index * 4 + 4].copy_from_slice(&cursor.to_le_bytes());
        cursor += stream.len() as u32;
    }
    let mut built = header;
    for stream in streams {
        built.extend(stream);
    }
    built.extend(std::iter::repeat_n(0, tail));
    if built.len() != (PACKED_END - PACKED_ADDRESS) as usize || cursor + tail as u32 != PACKED_END {
        return err("packed-image extent differs");
    }
    Ok(built)
}

fn build_font(source: &Map<String, Value>, root: &Path) -> Result<Vec<u8>, Error> {
    let font = object(source.get("font").unwrap(), "font source")?;
    if parse_hex(font.get("address").unwrap(), "font")? != FONT_ADDRESS
        || usize_field(font, "first_code", "font")? != 32
        || usize_field(font, "glyphs", "font")? != 224
    {
        return err("font metadata differs");
    }
    let advances = field_array_usize(font, "advances", "font")?;
    if advances.len() != 224 || advances.iter().any(|value| *value > 0xffff) {
        return err("font metadata differs");
    }
    let frames = atlas_frames(
        &source_path(root, string_field(font, "source", "font")?)?,
        224,
        16,
        16,
        usize_field(font, "columns", "font")?,
        1,
    )?;
    let mut output = vec![0u8; 224 * 32];
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
            output[index * 32 + 2 + y * 2..index * 32 + 4 + y * 2]
                .copy_from_slice(&row.to_le_bytes());
        }
    }
    if output.len() != (FONT_END - FONT_ADDRESS) as usize {
        return err("font extent differs");
    }
    Ok(output)
}

fn build_articles(source: &Map<String, Value>) -> Result<Vec<u8>, Error> {
    let article = object(source.get("articles").unwrap(), "article source")?;
    if parse_hex(article.get("data_address").unwrap(), "article data")? != ARTICLE_DATA_ADDRESS
        || parse_hex(article.get("table_address").unwrap(), "article table")?
            != ARTICLE_TABLE_ADDRESS
        || usize_field(article, "data_size", "article")?
            != (ARTICLE_TABLE_ADDRESS - ARTICLE_DATA_ADDRESS) as usize
    {
        return err("article metadata differs");
    }
    let records = array(article, "records", "article")?;
    let mut data = vec![0u8; (ARTICLE_TABLE_ADDRESS - ARTICLE_DATA_ADDRESS) as usize];
    for record in records {
        let record = object(record, "article record")?;
        let offset = usize_field(record, "offset", "article record")?;
        let text = string_field(record, "text", "article record")?;
        if !text.bytes().all(|byte| (0x20..=0x7e).contains(&byte)) {
            return err("article record differs");
        }
        let bytes = text.as_bytes();
        if offset + bytes.len() + 1 > data.len() {
            return err("article record exceeds its data area");
        }
        data[offset..offset + bytes.len()].copy_from_slice(bytes);
    }
    let entries = field_array_usize(article, "entries", "article")?;
    if entries.len() != 8 {
        return err("article metadata differs");
    }
    let mut table = vec![0u8; entries.len() * 4];
    for (index, entry) in entries.iter().enumerate() {
        if *entry >= records.len() {
            return err("article entry differs");
        }
        let offset = usize_field(
            object(&records[*entry], "article record")?,
            "offset",
            "article record",
        )?;
        table[index * 4..index * 4 + 4]
            .copy_from_slice(&(ARTICLE_DATA_ADDRESS + offset as u32).to_le_bytes());
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

fn relative(path: &Path, root: &Path) -> Result<String, Error> {
    path.strip_prefix(root)
        .map_err(|_| Error("source path is outside its root".into()))
        .map(|path| {
            path.to_string_lossy()
                .replace(std::path::MAIN_SEPARATOR, "/")
        })
}

pub fn export_localization_font(rom: &[u8], root: &Path) -> Result<Value, Error> {
    let image_root = root.join("graphics/fonts_localization_font");
    fs::create_dir_all(root.join("graphics")).map_err(|e| Error(e.to_string()))?;
    let mut direct = Vec::new();
    for (name, start, end, columns) in [
        ("tile_08029910.4bpp.png", TILE0_ADDRESS, TILE0_END, 2usize),
        (
            "tile_080310a4.4bpp.png",
            UI_TILE_ADDRESS,
            UI_TILE_END,
            2usize,
        ),
    ] {
        let path = PathBuf::from(format!("{}_{}", image_root.display(), name));
        let (image, _) =
            export_asset::tile_png(&read_range(rom, start, end)?, 4.0, columns as f64, None)
                .map_err(|e| Error(e.0))?;
        fs::write(&path, image).map_err(|e| Error(e.to_string()))?;
        direct.push(Value::Object(Map::from_iter([
            (String::from("address"), Value::String(hex(start))),
            (String::from("end"), Value::String(hex(end))),
            (
                String::from("source"),
                Value::String(relative(&path, root)?),
            ),
            (String::from("columns"), Value::from(columns)),
        ])));
    }
    let mut banks = Vec::new();
    for (index, (start, end, count, terminal)) in MTF_LAYOUT.iter().enumerate() {
        let slots: Vec<u32> = (0..*count)
            .map(|slot| pointer(rom, *start + slot as u32 * 4))
            .collect::<Result<_, _>>()?;
        if *terminal && pointer(rom, *start + *count as u32 * 4)? != u32::MAX {
            return err(format!("MTF bank {index} lacks its terminal entry"));
        }
        let mut pointers: Vec<u32> = slots.clone();
        pointers.sort_unstable();
        pointers.dedup();
        let first = *start + *count as u32 * 4 + if *terminal { 4 } else { 0 };
        if pointers.first().copied() != Some(first)
            || pointers
                .iter()
                .any(|value| *value < first || *value >= *end)
        {
            return err(format!("MTF bank {index} pointers differ"));
        }
        let mut frames = Vec::new();
        let mut encoded_end = first;
        for image in 0..pointers.len() {
            let value = pointers[image];
            let bound = pointers.get(image + 1).copied().unwrap_or(*end);
            let (pixels, _) = decode_pixels(&read_range(rom, value, bound)?)?;
            if pixels.len() != 256 {
                return err(format!("MTF bank {index} glyph dimensions differ"));
            }
            let encoded = encode_pixels(&pixels)?;
            if encoded != read_range(rom, value, value + encoded.len() as u32)? {
                return err(format!("MTF bank {index} glyph replay differs"));
            }
            if image + 1 < pointers.len() && value + encoded.len() as u32 != bound {
                return err(format!("MTF bank {index} has an interior gap"));
            }
            frames.push(pixels);
            encoded_end = value + encoded.len() as u32;
        }
        let tail = read_range(rom, encoded_end, *end)?;
        if tail.iter().any(|byte| *byte != 0) {
            return err(format!("MTF bank {index} has nonzero tail data"));
        }
        let columns = frames.len().min(16);
        let path = PathBuf::from(format!(
            "{}_mtf_{index:02}.indexed.png",
            image_root.display()
        ));
        fs::write(&path, atlas(&frames, 16, 16, columns)?).map_err(|e| Error(e.to_string()))?;
        let map: BTreeMap<u32, usize> = pointers
            .iter()
            .copied()
            .enumerate()
            .map(|(image, value)| (value, image))
            .collect();
        let entries: Vec<Value> = slots
            .iter()
            .map(|value| Value::from(*map.get(value).unwrap() as u64))
            .collect();
        banks.push(Value::Object(Map::from_iter([
            (String::from("address"), Value::String(hex(*start))),
            (String::from("end"), Value::String(hex(*end))),
            (String::from("entries"), Value::Array(entries)),
            (String::from("terminal"), Value::Bool(*terminal)),
            (String::from("images"), Value::from(frames.len())),
            (String::from("columns"), Value::from(columns)),
            (String::from("tail_zeros"), Value::from(tail.len())),
            (
                String::from("source"),
                Value::String(relative(&path, root)?),
            ),
        ])));
    }
    let packed_pointers: Vec<u32> = (0..8)
        .map(|index| pointer(rom, PACKED_ADDRESS + index * 4))
        .collect::<Result<_, _>>()?;
    if packed_pointers[0] != PACKED_ADDRESS + 32
        || packed_pointers.iter().enumerate().any(|(index, value)| {
            *value < PACKED_ADDRESS + 32
                || *value >= PACKED_END
                || (index > 0 && *value <= packed_pointers[index - 1])
        })
    {
        return err("packed-image pointers differ");
    }
    let mut packed_frames = Vec::new();
    let mut packed_tokens = Vec::new();
    let mut packed_used = PACKED_ADDRESS + 32;
    for (index, value) in packed_pointers.iter().enumerate() {
        let bound = packed_pointers
            .get(index + 1)
            .copied()
            .unwrap_or(PACKED_END);
        let (decoded, used, tokens) = decode_palette_trace(
            rom,
            (*value - ROM_BASE) as usize,
            (bound - ROM_BASE) as usize,
            512,
        )
        .map_err(|e| Error(e.0))?;
        let used_address = used as u32 + ROM_BASE;
        if decoded.len() != 512
            || encode_palette(&decoded, &tokens).map_err(|e| Error(e.0))?
                != read_range(rom, *value, used_address)?
        {
            return err(format!("packed image {index} differs"));
        }
        if index + 1 < packed_pointers.len() && used_address != bound {
            return err("packed images have an interior gap");
        }
        packed_frames.push(unpack_tiles(&decoded, 32, 32)?);
        packed_tokens.push(json_palette_tokens(&tokens));
        packed_used = used_address;
    }
    let packed_tail = read_range(rom, packed_used, PACKED_END)?;
    if packed_tail.iter().any(|byte| *byte != 0) {
        return err("packed images have a nonzero tail");
    }
    let packed_path = root.join("graphics/fonts_localization_font_packed_32x32.indexed.png");
    fs::write(&packed_path, atlas(&packed_frames, 32, 32, 4)?).map_err(|e| Error(e.to_string()))?;
    let font_raw = read_range(rom, FONT_ADDRESS, FONT_END)?;
    let mut font_frames = Vec::new();
    let mut advances = Vec::new();
    for glyph in 0..224 {
        advances.push(u16::from_le_bytes([font_raw[glyph * 32], font_raw[glyph * 32 + 1]]) as u64);
        let mut pixels = vec![0u8; 256];
        for y in 0..15 {
            let row = u16::from_le_bytes([
                font_raw[glyph * 32 + 2 + y * 2],
                font_raw[glyph * 32 + 3 + y * 2],
            ]);
            for x in 0..16 {
                pixels[y * 16 + x] = ((row >> (15 - x)) & 1) as u8;
            }
        }
        font_frames.push(pixels);
    }
    let font_path = root.join("graphics/fonts_localization_font_glyphs_0020_00ff.1bpp.png");
    fs::write(&font_path, {
        let columns = 16;
        let rows = font_frames.len().div_ceil(columns);
        let width = columns * 16;
        let height = rows * 16;
        let mut pixels = vec![0u8; width * height];
        for (index, frame) in font_frames.iter().enumerate() {
            let left = index % columns * 16;
            let top = index / columns * 16;
            for y in 0..16 {
                pixels[(top + y) * width + left..(top + y) * width + left + 16]
                    .copy_from_slice(&frame[y * 16..y * 16 + 16]);
            }
        }
        png_indexed(&pixels, width, height, 1)?
    })
    .map_err(|e| Error(e.to_string()))?;
    let article_entries: Vec<u32> = (0..8)
        .map(|index| pointer(rom, ARTICLE_TABLE_ADDRESS + index * 4))
        .collect::<Result<_, _>>()?;
    let mut unique = article_entries.clone();
    unique.sort_unstable();
    unique.dedup();
    let mut records = Vec::new();
    for address in &unique {
        if *address < ARTICLE_DATA_ADDRESS || *address >= ARTICLE_TABLE_ADDRESS {
            return err("article pointer differs");
        }
        let mut end = *address;
        while end < ARTICLE_TABLE_ADDRESS && rom[(end - ROM_BASE) as usize] != 0 {
            end += 1;
        }
        if end >= ARTICLE_TABLE_ADDRESS {
            return err("article text lacks a terminator");
        }
        let text = rom[(address - ROM_BASE) as usize..(end - ROM_BASE) as usize].to_vec();
        if text.iter().any(|byte| !(0x20..=0x7e).contains(byte)) {
            return err("article text is not printable ASCII");
        }
        let mut record = Map::new();
        record.insert(
            "offset".into(),
            Value::from(*address - ARTICLE_DATA_ADDRESS),
        );
        record.insert(
            "text".into(),
            Value::String(String::from_utf8(text).unwrap()),
        );
        records.push(Value::Object(record));
    }
    let record_map: BTreeMap<u32, usize> = unique
        .iter()
        .copied()
        .enumerate()
        .map(|(index, value)| (value, index))
        .collect();
    let source = Value::Object(Map::from_iter([
        (String::from("format"), Value::from(1)),
        (
            String::from("kind"),
            Value::String("golden-sun-localization-font".into()),
        ),
        (String::from("address"), Value::String(hex(PREFIX_ADDRESS))),
        (
            String::from("size"),
            Value::String(hex(PREFIX_END - PREFIX_ADDRESS)),
        ),
        (String::from("direct_tiles"), Value::Array(direct)),
        (String::from("mtf_banks"), Value::Array(banks)),
        (
            String::from("packed_images"),
            Value::Object(Map::from_iter([
                (String::from("address"), Value::String(hex(PACKED_ADDRESS))),
                (String::from("end"), Value::String(hex(PACKED_END))),
                (String::from("images"), Value::from(8)),
                (String::from("columns"), Value::from(4)),
                (String::from("tail_zeros"), Value::from(packed_tail.len())),
                (
                    String::from("source"),
                    Value::String(relative(&packed_path, root)?),
                ),
                (String::from("tokens"), Value::Array(packed_tokens)),
            ])),
        ),
        (
            String::from("font"),
            Value::Object(Map::from_iter([
                (String::from("address"), Value::String(hex(FONT_ADDRESS))),
                (String::from("first_code"), Value::from(32)),
                (String::from("glyphs"), Value::from(224)),
                (String::from("columns"), Value::from(16)),
                (
                    String::from("source"),
                    Value::String(relative(&font_path, root)?),
                ),
                (
                    String::from("advances"),
                    Value::Array(advances.into_iter().map(Value::from).collect()),
                ),
            ])),
        ),
        (
            String::from("articles"),
            Value::Object(Map::from_iter([
                (
                    String::from("data_address"),
                    Value::String(hex(ARTICLE_DATA_ADDRESS)),
                ),
                (
                    String::from("table_address"),
                    Value::String(hex(ARTICLE_TABLE_ADDRESS)),
                ),
                (
                    String::from("data_size"),
                    Value::from(ARTICLE_TABLE_ADDRESS - ARTICLE_DATA_ADDRESS),
                ),
                (String::from("records"), Value::Array(records)),
                (
                    String::from("entries"),
                    Value::Array(
                        article_entries
                            .iter()
                            .map(|value| Value::from(*record_map.get(value).unwrap()))
                            .collect(),
                    ),
                ),
            ])),
        ),
    ]));
    if build_localization_font(&source, root)? != read_range(rom, PREFIX_ADDRESS, PREFIX_END)? {
        return err("exported localization-font package does not round-trip");
    }
    Ok(source)
}

pub fn self_test() -> Result<(), Error> {
    for depth in [1u8, 4] {
        let pixels: Vec<u8> = (0..32 * 16)
            .map(|index| (index * 7 % (1usize << depth)) as u8)
            .collect();
        let png = png_indexed(&pixels, 32, 16, depth)?;
        let image = indexed_png(&png).map_err(|e| Error(e.0))?;
        if image.width != 32
            || image.height != 16
            || image
                .pixels
                .iter()
                .map(|pixel| *pixel as u8)
                .collect::<Vec<_>>()
                != pixels
        {
            return err(format!("{depth}bpp indexed-image self-test failed"));
        }
    }
    let pixels: Vec<u8> = (0..32 * 32)
        .map(|index| ((index * 11) & 15) as u8)
        .collect();
    if unpack_tiles(&pack_tiles(&pixels, 32, 32)?, 32, 32)? != pixels {
        return err("localization tile self-test failed");
    }
    if source_path(
        Path::new("assets"),
        "graphics/fonts/localization_font/../private.png",
    )
    .is_ok()
    {
        return err("localization-font path escape was accepted");
    }
    let sample: Vec<u8> = (0..256).map(|index| (index % 16) as u8).collect();
    if decode_pixels(&encode_pixels(&sample)?)?.0 != sample {
        return err("F0 pixel self-test failed");
    }
    Ok(())
}

pub fn canonical_source(value: &Value) -> String {
    canonical_json(value)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn self_test_passes() {
        self_test().unwrap();
    }

    #[test]
    fn f0_replay_preserves_mtf_stream() {
        let pixels: Vec<u8> = (0..512).map(|index| ((index * 3) & 15) as u8).collect();
        let encoded = encode_pixels(&pixels).unwrap();
        assert_eq!(decode_pixels(&encoded).unwrap().0, pixels);
    }

    #[test]
    fn indexed_png_has_expected_dimensions() {
        let pixels = vec![0u8; 32 * 16];
        let image = indexed_png(&png_indexed(&pixels, 32, 16, 1).unwrap()).unwrap();
        assert_eq!((image.width, image.height), (32, 16));
    }
}
