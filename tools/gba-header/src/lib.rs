//! GBA cartridge-header codec, ported from tools/make/gba_header.ts.

use std::fs;
use std::path::{Path, PathBuf};

use alchemy_zlib::{deflate_sync, DeflateOptions};
use canonical_json::{canonical_json, is_canonical_json_text};
use export_asset::chunk;
use import_asset::indexed_png;
use serde_json::{json, Map, Value};

pub const GBA_HEADER_ADDRESS: u32 = 0x0800_0000;
pub const GBA_HEADER_SIZE: usize = 0xc0;
pub const GBA_LOGO_ADDRESS: u32 = GBA_HEADER_ADDRESS + 0x04;
pub const GBA_LOGO_SIZE: usize = 0x9c;
pub const GBA_FIXED_ADDRESS: u32 = GBA_HEADER_ADDRESS + 0xb2;
pub const GBA_FIXED_SIZE: usize = 0x0a;
pub const GBA_RESERVED_END_ADDRESS: u32 = GBA_HEADER_ADDRESS + 0xbe;
pub const GBA_RESERVED_END_SIZE: usize = 0x02;
pub const GBA_LOGO_WIDTH: u32 = 104;
pub const GBA_LOGO_HEIGHT: u32 = 16;

const LOGO_SHA256: &str = "08a0153cfd6b0ea54b938f7d209933fa849da0d56f5a34c481060c9ff2fad818";
const CODEWORDS: [&str; 16] = [
    "1", "0110", "01010", "0100", "00010", "011110", "010110", "000110", "00110", "011111",
    "010111", "000111", "0010", "01110", "00111", "0000",
];
const EXPECTED_RESERVED: [(&str, usize); 2] = [("0x080000b5", 7), ("0x080000be", 2)];
const EXPECTED_UNRESOLVED: [(&str, &str, usize); 6] = [
    ("entry_branch", "0x08000000", 4),
    ("title", "0x080000a0", 12),
    ("game_code", "0x080000ac", 4),
    ("maker_code", "0x080000b0", 2),
    ("software_version", "0x080000bc", 1),
    ("complement_checksum", "0x080000bd", 1),
];

#[derive(Clone, Debug)]
struct Edition {
    target: u32,
    title: String,
    game_code: String,
    maker_code: String,
    software_version: u8,
}

fn object<'a>(value: &'a Value, label: &str) -> Result<&'a Map<String, Value>, String> {
    value
        .as_object()
        .ok_or_else(|| format!("{label} must be an object"))
}

fn exact_keys(value: &Map<String, Value>, expected: &[&str], label: &str) -> Result<(), String> {
    let mut actual: Vec<&str> = value.keys().map(String::as_str).collect();
    actual.sort_unstable();
    let mut wanted = expected.to_vec();
    wanted.sort_unstable();
    if actual != wanted {
        return Err(format!("{label} fields differ"));
    }
    Ok(())
}

fn number_equals(value: Option<&Value>, expected: u64) -> bool {
    let Some(value) = value else { return false };
    if let Some(actual) = value.as_u64() {
        return actual == expected;
    }
    value
        .as_f64()
        .is_some_and(|actual| actual.is_finite() && actual == expected as f64)
}

fn safe_positive_integer(value: Option<&Value>) -> Option<usize> {
    let number = value?.as_f64()?;
    if !number.is_finite()
        || number.fract() != 0.0
        || number <= 0.0
        || number > 9_007_199_254_740_991.0
    {
        return None;
    }
    Some(number as usize)
}

fn safe_integer(value: Option<&Value>) -> Option<i64> {
    let number = value?.as_f64()?;
    if !number.is_finite() || number.fract() != 0.0 || number.abs() > 9_007_199_254_740_991.0 {
        return None;
    }
    Some(number as i64)
}

fn canonical_address(value: Option<&Value>, label: &str) -> Result<(String, u32), String> {
    let text = value
        .and_then(Value::as_str)
        .ok_or_else(|| format!("{label} is not a canonical address"))?;
    if text.len() != 10
        || !text.starts_with("0x")
        || !text[2..]
            .bytes()
            .all(|byte| byte.is_ascii_hexdigit() && !byte.is_ascii_uppercase())
    {
        return Err(format!("{label} is not a canonical address"));
    }
    let address = u32::from_str_radix(&text[2..], 16)
        .map_err(|_| format!("{label} is not a canonical address"))?;
    Ok((text.to_string(), address))
}

fn parse_record(
    value: &Value,
    label: &str,
    with_name: bool,
) -> Result<(Option<String>, String, usize), String> {
    let record = object(value, label)?;
    let keys = if with_name {
        &["name", "address", "size"][..]
    } else {
        &["address", "size"][..]
    };
    exact_keys(record, keys, label)?;
    let name = if with_name {
        Some(
            record
                .get("name")
                .and_then(Value::as_str)
                .ok_or_else(|| format!("{label} name differs"))?
                .to_string(),
        )
    } else {
        None
    };
    let (address, _) = canonical_address(record.get("address"), &format!("{label} address"))?;
    let size = safe_positive_integer(record.get("size"));
    if size.is_none() {
        return Err(format!("{label} size differs"));
    }
    Ok((name, address, size.unwrap()))
}

fn parse_ascii(value: Option<&Value>, length: usize, label: &str) -> Result<String, String> {
    let text = value
        .and_then(Value::as_str)
        .ok_or_else(|| format!("{label} differs"))?;
    if text.len() != length
        || !text
            .bytes()
            .all(|byte| byte.is_ascii_uppercase() || byte.is_ascii_digit())
    {
        return Err(format!("{label} differs"));
    }
    Ok(text.to_string())
}

fn title_bytes(title: &str) -> Result<[u8; 12], String> {
    let bytes = title.as_bytes();
    if bytes.len() > 12 || bytes.iter().any(|byte| !(*byte >= 0x20 && *byte <= 0x7e)) {
        return Err("GBA title must contain at most twelve printable ASCII bytes".to_string());
    }
    let mut output = [0u8; 12];
    output[..bytes.len()].copy_from_slice(bytes);
    Ok(output)
}

fn parse_edition(value: &Value) -> Result<Edition, String> {
    let edition = object(value, "GBA header edition")?;
    exact_keys(
        edition,
        &[
            "entry_branch",
            "title",
            "game_code",
            "maker_code",
            "software_version",
            "complement_checksum",
        ],
        "GBA header edition",
    )?;
    let entry = object(
        edition
            .get("entry_branch")
            .ok_or("GBA entry branch must be an object")?,
        "GBA entry branch",
    )?;
    exact_keys(
        entry,
        &["instruction_set", "operation", "target"],
        "GBA entry branch",
    )?;
    if entry.get("instruction_set").and_then(Value::as_str) != Some("arm")
        || entry.get("operation").and_then(Value::as_str) != Some("b")
    {
        return Err("GBA entry branch differs".to_string());
    }
    let (_, target) = canonical_address(entry.get("target"), "GBA entry target")?;
    encode_arm_branch(GBA_HEADER_ADDRESS, target)?;

    let title = object(
        edition.get("title").ok_or("GBA title must be an object")?,
        "GBA title",
    )?;
    exact_keys(title, &["text", "padding", "field_bytes"], "GBA title")?;
    let title_text = title
        .get("text")
        .and_then(Value::as_str)
        .ok_or("GBA title encoding differs")?;
    if title.get("padding").and_then(Value::as_str) != Some("nul")
        || !number_equals(title.get("field_bytes"), 12)
    {
        return Err("GBA title encoding differs".to_string());
    }
    title_bytes(title_text)?;
    let game_code = parse_ascii(edition.get("game_code"), 4, "GBA game code")?;
    let maker_code = parse_ascii(edition.get("maker_code"), 2, "GBA maker code")?;
    let version = safe_integer(edition.get("software_version"));
    if !version.is_some_and(|value| (0..=0xff).contains(&value)) {
        return Err("GBA software version differs".to_string());
    }
    if edition.get("complement_checksum").and_then(Value::as_str) != Some("derived") {
        return Err("GBA checksum policy differs".to_string());
    }
    Ok(Edition {
        target,
        title: title_text.to_string(),
        game_code,
        maker_code,
        software_version: version.unwrap() as u8,
    })
}

pub fn parse_gba_header_source(value: &Value) -> Result<(), String> {
    let source = object(value, "GBA header source")?;
    exact_keys(
        source,
        &[
            "format",
            "kind",
            "address",
            "standard",
            "edition",
            "unresolved_fields",
        ],
        "GBA header source",
    )?;
    if !number_equals(source.get("format"), 2)
        || source.get("kind").and_then(Value::as_str)
            != Some("gba-cartridge-header-standard-fields")
        || source.get("address").and_then(Value::as_str) != Some("0x08000000")
    {
        return Err("unsupported GBA header source".to_string());
    }
    let standard = object(
        source
            .get("standard")
            .ok_or("GBA header standard fields must be an object")?,
        "GBA header standard fields",
    )?;
    exact_keys(
        standard,
        &[
            "logo",
            "fixed_value",
            "unit_code",
            "device_type",
            "reserved_zero_ranges",
        ],
        "GBA header standard fields",
    )?;
    let logo = object(
        standard
            .get("logo")
            .ok_or("GBA header logo must be an object")?,
        "GBA header logo",
    )?;
    exact_keys(
        logo,
        &["codec", "source", "width", "height", "bpp"],
        "GBA header logo",
    )?;
    if logo.get("codec").and_then(Value::as_str) != Some("gba-bios-huffman-logo")
        || logo
            .get("source")
            .and_then(Value::as_str)
            .is_none_or(str::is_empty)
        || !number_equals(logo.get("width"), GBA_LOGO_WIDTH as u64)
        || !number_equals(logo.get("height"), GBA_LOGO_HEIGHT as u64)
        || !number_equals(logo.get("bpp"), 1)
        || standard.get("fixed_value").and_then(Value::as_str) != Some("0x96")
        || standard.get("unit_code").and_then(Value::as_str) != Some("0x00")
        || standard.get("device_type").and_then(Value::as_str) != Some("0x00")
    {
        return Err("GBA header standard values differ".to_string());
    }
    let reserved = source_array(
        standard.get("reserved_zero_ranges"),
        "GBA header reserved ranges differ",
    )?;
    let parsed_reserved: Vec<(Option<String>, String, usize)> = reserved
        .iter()
        .enumerate()
        .map(|(index, value)| {
            parse_record(value, &format!("GBA header reserved range {index}"), false)
        })
        .collect::<Result<_, _>>()?;
    if parsed_reserved.len() != EXPECTED_RESERVED.len()
        || parsed_reserved.iter().zip(EXPECTED_RESERVED).any(
            |((_, address, size), (expected_address, expected_size))| {
                address != expected_address || *size != expected_size
            },
        )
    {
        return Err("GBA header reserved ranges differ".to_string());
    }

    let unresolved = source_array(
        source.get("unresolved_fields"),
        "GBA header unresolved fields differ",
    )?;
    let parsed_unresolved: Vec<(Option<String>, String, usize)> = unresolved
        .iter()
        .enumerate()
        .map(|(index, value)| {
            parse_record(value, &format!("GBA header unresolved field {index}"), true)
        })
        .collect::<Result<_, _>>()?;
    match source.get("edition") {
        Some(Value::Null) => {
            if parsed_unresolved.len() != EXPECTED_UNRESOLVED.len()
                || parsed_unresolved.iter().zip(EXPECTED_UNRESOLVED).any(
                    |((name, address, size), (expected_name, expected_address, expected_size))| {
                        name.as_deref() != Some(expected_name)
                            || address != expected_address
                            || *size != expected_size
                    },
                )
            {
                return Err("GBA header unresolved fields differ".to_string());
            }
        }
        Some(edition) => {
            parse_edition(edition)?;
            if !unresolved.is_empty() {
                return Err("resolved GBA header still names unresolved fields".to_string());
            }
        }
        None => return Err("GBA header edition must be an object".to_string()),
    }
    Ok(())
}

fn source_array<'a>(value: Option<&'a Value>, message: &str) -> Result<&'a Vec<Value>, String> {
    value
        .and_then(Value::as_array)
        .ok_or_else(|| message.to_string())
}

pub fn read_gba_header_source(path: &Path) -> Result<Value, String> {
    let text = fs::read_to_string(path).map_err(|error| error.to_string())?;
    let value: Value = serde_json::from_str(&text).map_err(|error| error.to_string())?;
    if !is_canonical_json_text(&text, &value) {
        return Err(format!("{}: source is not canonical JSON", path.display()));
    }
    parse_gba_header_source(&value)?;
    Ok(value)
}

fn png_header(width: u32, height: u32) -> Vec<u8> {
    let mut header = vec![0u8; 13];
    header[0..4].copy_from_slice(&width.to_be_bytes());
    header[4..8].copy_from_slice(&height.to_be_bytes());
    header[8..13].copy_from_slice(&[1, 3, 0, 0, 0]);
    header
}

fn logo_png(pixels: &[u8]) -> Vec<u8> {
    let mut rows = vec![0u8; GBA_LOGO_HEIGHT as usize * (GBA_LOGO_WIDTH as usize / 8 + 1)];
    let mut cursor = 0;
    for y in 0..GBA_LOGO_HEIGHT as usize {
        cursor += 1;
        for x in (0..GBA_LOGO_WIDTH as usize).step_by(8) {
            let mut value = 0u8;
            for bit in 0..8 {
                value |= pixels[y * GBA_LOGO_WIDTH as usize + x + bit] << (7 - bit);
            }
            rows[cursor] = value;
            cursor += 1;
        }
    }
    let mut output = b"\x89PNG\r\n\x1a\n".to_vec();
    output.extend_from_slice(&chunk(
        b"IHDR",
        &png_header(GBA_LOGO_WIDTH, GBA_LOGO_HEIGHT),
    ));
    output.extend_from_slice(&chunk(b"PLTE", &[0xff, 0xff, 0xff, 0, 0, 0]));
    output.extend_from_slice(&chunk(
        b"IDAT",
        &deflate_sync(&rows, DeflateOptions { level: Some(9) }),
    ));
    output.extend_from_slice(&chunk(b"IEND", &[]));
    output
}

fn tiled_logo_bits(pixels: &[u8]) -> Vec<u8> {
    let mut decoded = vec![0u8; GBA_LOGO_WIDTH as usize * GBA_LOGO_HEIGHT as usize / 8];
    let tiles_wide = GBA_LOGO_WIDTH as usize / 8;
    for y in 0..GBA_LOGO_HEIGHT as usize {
        for x in 0..GBA_LOGO_WIDTH as usize {
            let tile = (y / 8) * tiles_wide + x / 8;
            let bit = tile * 64 + (y & 7) * 8 + (x & 7);
            decoded[bit >> 3] |= pixels[y * GBA_LOGO_WIDTH as usize + x] << (bit & 7);
        }
    }
    decoded
}

fn addition_deltas(decoded: &[u8]) -> Vec<u8> {
    let mut output = vec![0u8; decoded.len() + 4];
    let header = ((decoded.len() as u32) << 8) | 0x82;
    output[0..4].copy_from_slice(&header.to_le_bytes());
    let mut previous = 0u16;
    for (index, bytes) in decoded.chunks_exact(2).enumerate() {
        let current = u16::from_le_bytes([bytes[0], bytes[1]]);
        let delta = current.wrapping_sub(previous);
        output[4 + index * 2..6 + index * 2].copy_from_slice(&delta.to_le_bytes());
        previous = current;
    }
    output
}

fn huffman_logo(data: &[u8]) -> Result<Vec<u8>, String> {
    let mut output = vec![0u8; GBA_LOGO_SIZE];
    let mut position = 0usize;
    for value in data {
        for nibble in [value & 0x0f, value >> 4] {
            for character in CODEWORDS[nibble as usize].bytes() {
                if position >= 0x4ce {
                    return Err("GBA logo exceeds its fixed Huffman field".to_string());
                }
                if character == b'1' {
                    let word_offset = (position / 32) * 4;
                    let shift = 31 - position % 32;
                    let mut word = u32::from_le_bytes(
                        output[word_offset..word_offset + 4].try_into().unwrap(),
                    );
                    word |= 1u32 << shift;
                    output[word_offset..word_offset + 4].copy_from_slice(&word.to_le_bytes());
                }
                position += 1;
            }
        }
    }
    output[0x98] = 0x21;
    output[0x99] = 0xd4;
    Ok(output)
}

pub fn encode_gba_logo(image: &[u8]) -> Result<Vec<u8>, String> {
    let decoded = indexed_png(image).map_err(|error| error.0)?;
    let pixels: Vec<u8> = decoded.pixels.iter().map(|pixel| *pixel as u8).collect();
    if decoded.width != GBA_LOGO_WIDTH
        || decoded.height != GBA_LOGO_HEIGHT
        || decoded.palette != vec![[255, 255, 255], [0, 0, 0]]
        || image != logo_png(&pixels)
    {
        return Err("GBA logo source must be the canonical 104x16 monochrome PNG".to_string());
    }
    let output = huffman_logo(&addition_deltas(&tiled_logo_bits(&pixels)))?;
    if sha256_hex(&output) != LOGO_SHA256 {
        return Err("GBA logo source does not encode the standard firmware logo".to_string());
    }
    Ok(output)
}

pub fn encode_arm_branch(address: u32, target: u32) -> Result<Vec<u8>, String> {
    if address > 0xffff_fffc || target > 0xffff_fffc || address % 4 != 0 || target % 4 != 0 {
        return Err("ARM branch addresses must be aligned u32 values".to_string());
    }
    let words = (target as i64 - (address as i64 + 8)) / 4;
    if !(-0x80_0000..=0x7f_ffff).contains(&words) {
        return Err("ARM branch target is out of range".to_string());
    }
    let instruction = 0xea00_0000u32 | (words as u32 & 0x00ff_ffff);
    Ok(instruction.to_le_bytes().to_vec())
}

pub fn decode_arm_branch(data: &[u8], address: u32) -> Result<u32, String> {
    if data.len() != 4 {
        return Err("GBA entry branch must contain one ARM instruction".to_string());
    }
    let instruction = u32::from_le_bytes(data.try_into().unwrap());
    if instruction & 0xff00_0000 != 0xea00_0000 {
        return Err("GBA entry instruction is not an unconditional ARM B".to_string());
    }
    let mut displacement = instruction & 0x00ff_ffff;
    if displacement & 0x0080_0000 != 0 {
        displacement |= 0xff00_0000;
    }
    let target = address as i64 + 8 + (displacement as i32 as i64) * 4;
    if !(0..=0xffff_fffc).contains(&target) || target % 4 != 0 {
        return Err("GBA entry target is invalid".to_string());
    }
    Ok(target as u32)
}

pub fn gba_complement_checksum(header: &[u8]) -> Result<u8, String> {
    if header.len() < 0xbd {
        return Err("GBA header is too short for its complement checksum".to_string());
    }
    let mut checksum: i64 = -0x19;
    for byte in &header[0xa0..=0xbc] {
        checksum -= i64::from(*byte);
    }
    Ok(checksum.rem_euclid(256) as u8)
}

fn edition(source: &Value) -> Result<Edition, String> {
    let source_object = object(source, "GBA header source")?;
    match source_object.get("edition") {
        Some(Value::Null) => Err("GBA header edition is unresolved".to_string()),
        Some(value) => parse_edition(value),
        None => Err("GBA header edition is unresolved".to_string()),
    }
}

fn normalize_path(path: &Path) -> PathBuf {
    if path.is_absolute() {
        path.to_path_buf()
    } else {
        std::env::current_dir().unwrap().join(path)
    }
}

fn repository_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .unwrap()
        .to_path_buf()
}

fn logo_source_path(source: &Value) -> Result<PathBuf, String> {
    let source_object = object(source, "GBA header source")?;
    let standard = object(
        source_object.get("standard").unwrap(),
        "GBA header standard fields",
    )?;
    let logo = object(standard.get("logo").unwrap(), "GBA header logo")?;
    let relative = logo.get("source").and_then(Value::as_str).unwrap();
    let root = normalize_path(&repository_root());
    let path = normalize_path(&root.join(relative));
    if path != root && !path.starts_with(&root) {
        return Err("GBA logo source escaped the repository".to_string());
    }
    Ok(path)
}

pub fn build_gba_header(source: &Value, logo_image: &[u8]) -> Result<Vec<u8>, String> {
    parse_gba_header_source(source)?;
    let edition = edition(source)?;
    let mut output = vec![0u8; GBA_HEADER_SIZE];
    output[0..4].copy_from_slice(&encode_arm_branch(GBA_HEADER_ADDRESS, edition.target)?);
    output[0x04..0xa0].copy_from_slice(&encode_gba_logo(logo_image)?);
    output[0xa0..0xac].copy_from_slice(&title_bytes(&edition.title)?);
    output[0xac..0xb0].copy_from_slice(edition.game_code.as_bytes());
    output[0xb0..0xb2].copy_from_slice(edition.maker_code.as_bytes());
    output[0xb2] = 0x96;
    output[0xbc] = edition.software_version;
    output[0xbd] = gba_complement_checksum(&output)?;
    Ok(output)
}

pub fn build_gba_header_component(
    source: &Value,
    logo_image: &[u8],
    address: u32,
    size: usize,
) -> Result<Vec<u8>, String> {
    parse_gba_header_source(source)?;
    if address == GBA_LOGO_ADDRESS && size == GBA_LOGO_SIZE {
        return encode_gba_logo(logo_image);
    }
    if address == GBA_FIXED_ADDRESS && size == GBA_FIXED_SIZE {
        return Ok(vec![0x96, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
    }
    if address == GBA_RESERVED_END_ADDRESS && size == GBA_RESERVED_END_SIZE {
        return Ok(vec![0, 0]);
    }
    if object(source, "GBA header source")?
        .get("edition")
        .is_some_and(|value| !value.is_null())
        && size > 0
        && address >= GBA_HEADER_ADDRESS
        && (address as u64 + size as u64) <= GBA_HEADER_ADDRESS as u64 + GBA_HEADER_SIZE as u64
    {
        let header = build_gba_header(source, logo_image)?;
        let offset = (address - GBA_HEADER_ADDRESS) as usize;
        return Ok(header[offset..offset + size].to_vec());
    }
    Err("GBA header component is not independently determined".to_string())
}

fn decode_title(field: &[u8]) -> Result<String, String> {
    if field.len() != 12 {
        return Err("GBA title field size differs".to_string());
    }
    let terminator = field.iter().position(|byte| *byte == 0);
    let length = terminator.unwrap_or(field.len());
    if terminator.is_some_and(|index| field[index..].iter().any(|byte| *byte != 0)) {
        return Err("GBA title has data after NUL padding".to_string());
    }
    if field[..length]
        .iter()
        .any(|byte| !(*byte >= 0x20 && *byte <= 0x7e))
    {
        return Err("GBA title is not printable ASCII".to_string());
    }
    Ok(String::from_utf8(field[..length].to_vec()).unwrap())
}

fn decode_code(field: &[u8], label: &str, length: usize) -> Result<String, String> {
    let text =
        String::from_utf8(field.to_vec()).map_err(|_| format!("{label} is not canonical ASCII"))?;
    if text.len() != length
        || !text
            .bytes()
            .all(|byte| byte.is_ascii_uppercase() || byte.is_ascii_digit())
    {
        return Err(format!("{label} is not canonical ASCII"));
    }
    Ok(text)
}

fn standard_clone(source: &Value) -> Value {
    object(source, "GBA header source")
        .unwrap()
        .get("standard")
        .unwrap()
        .clone()
}

pub fn derive_gba_header_source(header: &[u8], template: &Value) -> Result<Value, String> {
    if header.len() != GBA_HEADER_SIZE {
        return Err("GBA header extent differs".to_string());
    }
    parse_gba_header_source(template)?;
    let logo_path = logo_source_path(template)?;
    let logo = encode_gba_logo(&fs::read(logo_path).map_err(|error| error.to_string())?)?;
    if header[0x04..0xa0] != logo {
        return Err("GBA firmware logo differs".to_string());
    }
    if header[0xb2] != 0x96
        || header[0xb3] != 0
        || header[0xb4] != 0
        || header[0xb5..0xbc].iter().any(|byte| *byte != 0)
        || header[0xbe..0xc0].iter().any(|byte| *byte != 0)
    {
        return Err("GBA fixed or reserved fields differ".to_string());
    }
    let target = decode_arm_branch(&header[0..4], GBA_HEADER_ADDRESS)?;
    let source = json!({
        "format": 2,
        "kind": "gba-cartridge-header-standard-fields",
        "address": "0x08000000",
        "standard": standard_clone(template),
        "edition": {
            "entry_branch": {
                "instruction_set": "arm",
                "operation": "b",
                "target": format!("0x{target:08x}"),
            },
            "title": {
                "text": decode_title(&header[0xa0..0xac])?,
                "padding": "nul",
                "field_bytes": 12,
            },
            "game_code": decode_code(&header[0xac..0xb0], "GBA game code", 4)?,
            "maker_code": decode_code(&header[0xb0..0xb2], "GBA maker code", 2)?,
            "software_version": header[0xbc],
            "complement_checksum": "derived",
        },
        "unresolved_fields": [],
    });
    parse_gba_header_source(&source)?;
    if header[0xbd] != gba_complement_checksum(header)? {
        return Err("GBA complement checksum differs".to_string());
    }
    let source_logo = fs::read(logo_source_path(&source)?).map_err(|error| error.to_string())?;
    let rebuilt = build_gba_header(&source, &source_logo)?;
    if rebuilt != header {
        return Err("derived GBA header source does not round-trip".to_string());
    }
    Ok(source)
}

fn same_path(left: &Path, right: &Path) -> bool {
    match (fs::canonicalize(left), fs::canonicalize(right)) {
        (Ok(left), Ok(right)) => left == right,
        _ => normalize_path(left) == normalize_path(right),
    }
}

fn temporary_directory(parent: &Path, prefix: &str) -> Result<PathBuf, String> {
    let pid = std::process::id();
    for attempt in 0..1000u32 {
        let candidate = parent.join(format!(".{prefix}{pid}-{attempt}"));
        if fs::create_dir(&candidate).is_ok() {
            return Ok(candidate);
        }
    }
    Err("unable to create temporary export directory".to_string())
}

pub fn export_gba_header(
    rom: &[u8],
    output: &Path,
    template_path: &Path,
    input_path: Option<&Path>,
) -> Result<Value, String> {
    if rom.len() < GBA_HEADER_SIZE {
        return Err("ROM is too small for a GBA header".to_string());
    }
    if input_path.is_some_and(|input| same_path(input, output)) {
        return Err("refusing to overwrite the input ROM".to_string());
    }
    let template = read_gba_header_source(template_path)?;
    let source = derive_gba_header_source(&rom[..GBA_HEADER_SIZE], &template)?;
    let destination = normalize_path(output);
    let parent = destination.parent().unwrap();
    fs::create_dir_all(parent).map_err(|error| error.to_string())?;
    let transaction = temporary_directory(parent, "gba-header-export-")?;
    let staged = transaction.join(destination.file_name().unwrap());
    let result = (|| {
        fs::write(&staged, format!("{}\n", canonical_json(&source)))
            .map_err(|error| error.to_string())?;
        let staged_source = read_gba_header_source(&staged)?;
        let logo =
            fs::read(logo_source_path(&staged_source)?).map_err(|error| error.to_string())?;
        if build_gba_header(&staged_source, &logo)? != rom[..GBA_HEADER_SIZE] {
            return Err("staged GBA header source does not round-trip".to_string());
        }
        fs::rename(&staged, &destination).map_err(|error| error.to_string())?;
        Ok(source.clone())
    })();
    let _ = fs::remove_dir_all(&transaction);
    result
}

pub fn verify_gba_header(rom: &[u8], source_path: &Path) -> Result<Vec<u8>, String> {
    if rom.len() < GBA_HEADER_SIZE {
        return Err("ROM is too small for a GBA header".to_string());
    }
    let source = read_gba_header_source(source_path)?;
    let logo = fs::read(logo_source_path(&source)?).map_err(|error| error.to_string())?;
    let built = build_gba_header(&source, &logo)?;
    if built != rom[..GBA_HEADER_SIZE] {
        let offset = built
            .iter()
            .zip(&rom[..GBA_HEADER_SIZE])
            .position(|(left, right)| left != right)
            .unwrap_or(0);
        return Err(format!(
            "GBA header differs at 0x{:08x}",
            GBA_HEADER_ADDRESS + offset as u32
        ));
    }
    Ok(built)
}

fn synthetic_source(template: &Value) -> Value {
    json!({
        "format": 2,
        "kind": "gba-cartridge-header-standard-fields",
        "address": "0x08000000",
        "standard": standard_clone(template),
        "edition": {
            "entry_branch": { "instruction_set": "arm", "operation": "b", "target": "0x080003c0" },
            "title": { "text": "HEADER TEST", "padding": "nul", "field_bytes": 12 },
            "game_code": "TST1",
            "maker_code": "01",
            "software_version": 7,
            "complement_checksum": "derived",
        },
        "unresolved_fields": [],
    })
}

fn expected_unresolved() -> Value {
    Value::Array(
        EXPECTED_UNRESOLVED
            .iter()
            .map(|(name, address, size)| json!({ "name": name, "address": address, "size": size }))
            .collect(),
    )
}

fn reject<F: FnOnce() -> Result<(), String>>(callback: F) -> bool {
    callback().is_err()
}

pub fn self_test() -> Result<String, String> {
    let default_source = repository_root().join("assets/data/gba_header.json");
    let canonical = read_gba_header_source(&default_source)?;
    let canonical_object = object(&canonical, "GBA header source")?;
    if canonical_object.get("edition").is_none_or(Value::is_null)
        || !canonical_object
            .get("unresolved_fields")
            .unwrap()
            .as_array()
            .unwrap()
            .is_empty()
    {
        return Err(
            "canonical GBA header source must be fully resolved after byte closure".to_string(),
        );
    }
    let mut template = canonical.clone();
    let template_object = template.as_object_mut().unwrap();
    template_object.insert("edition".to_string(), Value::Null);
    template_object.insert("unresolved_fields".to_string(), expected_unresolved());
    parse_gba_header_source(&template)?;
    let logo_image = fs::read(logo_source_path(&template)?).map_err(|error| error.to_string())?;
    let source = synthetic_source(&template);
    let built = build_gba_header(&source, &logo_image)?;
    if built.len() != GBA_HEADER_SIZE
        || built[0..4] != [0xee, 0, 0, 0xea]
        || decode_arm_branch(&built[0..4], GBA_HEADER_ADDRESS)? != 0x0800_03c0
        || built[0xbd] != gba_complement_checksum(&built)?
    {
        return Err("complete GBA header build self-test failed".to_string());
    }
    if derive_gba_header_source(&built, &template)? != source {
        return Err("GBA header semantic round-trip failed".to_string());
    }
    let temporary_root = std::env::var_os("TMPDIR")
        .or_else(|| std::env::var_os("TMP"))
        .or_else(|| std::env::var_os("TEMP"))
        .map(PathBuf::from)
        .unwrap_or_else(|| PathBuf::from("/tmp"));
    let temporary = temporary_directory(&temporary_root, "gba-header-self-test-")?;
    let result = (|| {
        let output = temporary.join("header.json");
        export_gba_header(&built, &output, &default_source, None)?;
        if build_gba_header(&read_gba_header_source(&output)?, &logo_image)? != built {
            return Err("exported GBA header differs".to_string());
        }
        verify_gba_header(&built, &output)?;
        let sentinel = b"preserve\n";
        fs::write(&output, sentinel).map_err(|error| error.to_string())?;
        let mut corrupt_checksum = built.clone();
        corrupt_checksum[0xbd] ^= 1;
        if !reject(|| {
            export_gba_header(&corrupt_checksum, &output, &default_source, None).map(|_| ())
        }) || fs::read(&output).map_err(|error| error.to_string())? != sentinel
        {
            return Err("failed export replaced its destination".to_string());
        }
        let mut corrupt_branch = built.clone();
        corrupt_branch[3] = 0xeb;
        if !reject(|| derive_gba_header_source(&corrupt_branch, &template).map(|_| ())) {
            return Err("ARM BL entry was accepted".to_string());
        }
        let mut corrupt_logo = built.clone();
        corrupt_logo[0x20] ^= 1;
        if !reject(|| derive_gba_header_source(&corrupt_logo, &template).map(|_| ())) {
            return Err("nonstandard logo was accepted".to_string());
        }
        let mut corrupt_fixed = built.clone();
        corrupt_fixed[0xb5] = 1;
        if !reject(|| derive_gba_header_source(&corrupt_fixed, &template).map(|_| ())) {
            return Err("reserved data was accepted".to_string());
        }
        let mut corrupt_title = built.clone();
        corrupt_title[0xa2] = 0;
        corrupt_title[0xbd] = gba_complement_checksum(&corrupt_title)?;
        if !reject(|| derive_gba_header_source(&corrupt_title, &template).map(|_| ())) {
            return Err("embedded title NUL was accepted".to_string());
        }
        let mut corrupt_code = built.clone();
        corrupt_code[0xac] = b'a';
        corrupt_code[0xbd] = gba_complement_checksum(&corrupt_code)?;
        if !reject(|| derive_gba_header_source(&corrupt_code, &template).map(|_| ())) {
            return Err("lowercase game code was accepted".to_string());
        }
        let mut unknown = source.clone();
        unknown
            .as_object_mut()
            .unwrap()
            .get_mut("edition")
            .unwrap()
            .as_object_mut()
            .unwrap()
            .insert("raw_checksum".to_string(), json!("0x00"));
        if parse_gba_header_source(&unknown).is_ok() {
            return Err("unknown edition field was accepted".to_string());
        }
        let mut explicit_checksum = source.clone();
        explicit_checksum
            .as_object_mut()
            .unwrap()
            .get_mut("edition")
            .unwrap()
            .as_object_mut()
            .unwrap()
            .insert("complement_checksum".to_string(), json!("0x00"));
        if parse_gba_header_source(&explicit_checksum).is_ok() {
            return Err("explicit checksum byte was accepted".to_string());
        }
        let mut bad_version = source.clone();
        bad_version
            .as_object_mut()
            .unwrap()
            .get_mut("edition")
            .unwrap()
            .as_object_mut()
            .unwrap()
            .insert("software_version".to_string(), json!(256));
        if parse_gba_header_source(&bad_version).is_ok() {
            return Err("oversized software version was accepted".to_string());
        }
        if !reject(|| build_gba_header(&template, &logo_image).map(|_| ())) {
            return Err("unresolved header built as complete".to_string());
        }
        fs::write(&output, serde_json::to_string(&source).unwrap())
            .map_err(|error| error.to_string())?;
        if !reject(|| read_gba_header_source(&output).map(|_| ())) {
            return Err("noncanonical JSON was accepted".to_string());
        }
        let rom = temporary.join("rom.gba");
        fs::write(&rom, &built).map_err(|error| error.to_string())?;
        if !reject(|| export_gba_header(&built, &rom, &default_source, Some(&rom)).map(|_| ())) {
            return Err("input ROM overwrite was accepted".to_string());
        }
        Ok(())
    })();
    let _ = fs::remove_dir_all(&temporary);
    result?;
    Ok("self-test=ok bytes=192 unresolved=0".to_string())
}

mod sha256 {
    const K: [u32; 64] = [
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4,
        0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe,
        0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f,
        0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
        0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc,
        0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
        0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116,
        0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7,
        0xc67178f2,
    ];

    pub fn hex(data: &[u8]) -> String {
        let mut state = [
            0x6a09e667u32,
            0xbb67ae85,
            0x3c6ef372,
            0xa54ff53a,
            0x510e527f,
            0x9b05688c,
            0x1f83d9ab,
            0x5be0cd19,
        ];
        let bit_length = (data.len() as u64) * 8;
        let mut message = data.to_vec();
        message.push(0x80);
        while message.len() % 64 != 56 {
            message.push(0);
        }
        message.extend_from_slice(&bit_length.to_be_bytes());
        for block in message.chunks_exact(64) {
            let mut words = [0u32; 64];
            for (index, word) in words[..16].iter_mut().enumerate() {
                *word = u32::from_be_bytes(block[index * 4..index * 4 + 4].try_into().unwrap());
            }
            for index in 16..64 {
                let s0 = words[index - 15].rotate_right(7)
                    ^ words[index - 15].rotate_right(18)
                    ^ (words[index - 15] >> 3);
                let s1 = words[index - 2].rotate_right(17)
                    ^ words[index - 2].rotate_right(19)
                    ^ (words[index - 2] >> 10);
                words[index] = words[index - 16]
                    .wrapping_add(s0)
                    .wrapping_add(words[index - 7])
                    .wrapping_add(s1);
            }
            let (mut a, mut b, mut c, mut d, mut e, mut f, mut g, mut h) = (
                state[0], state[1], state[2], state[3], state[4], state[5], state[6], state[7],
            );
            for index in 0..64 {
                let s1 = e.rotate_right(6) ^ e.rotate_right(11) ^ e.rotate_right(25);
                let choice = (e & f) ^ ((!e) & g);
                let temp1 = h
                    .wrapping_add(s1)
                    .wrapping_add(choice)
                    .wrapping_add(K[index])
                    .wrapping_add(words[index]);
                let s0 = a.rotate_right(2) ^ a.rotate_right(13) ^ a.rotate_right(22);
                let majority = (a & b) ^ (a & c) ^ (b & c);
                let temp2 = s0.wrapping_add(majority);
                (h, g, f, e, d, c, b, a) = (
                    g,
                    f,
                    e,
                    d.wrapping_add(temp1),
                    c,
                    b,
                    a,
                    temp1.wrapping_add(temp2),
                );
            }
            for (value, add) in state.iter_mut().zip([a, b, c, d, e, f, g, h]) {
                *value = value.wrapping_add(add);
            }
        }
        state.iter().map(|word| format!("{word:08x}")).collect()
    }
}

fn sha256_hex(data: &[u8]) -> String {
    sha256::hex(data)
}
