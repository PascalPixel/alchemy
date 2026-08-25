//! GBA cartridge-header codec, ported from tools/make/gba_header.ts.

use std::fs;
use std::path::Path;

use canonical_json::is_canonical_json_text;
use import_asset::indexed_png;
use serde_json::{Map, Value};
use sha2::{Digest, Sha256};

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
const LOGO_PNG_SHA256: &str = "060df97f1ea5afefd2c32a471614d116ef855b545800695a451915d8f5f350ba";
const CODEWORDS: [&str; 16] = ["1", "0110", "01010", "0100", "00010", "011110", "010110", "000110", "00110", "011111", "010111", "000111", "0010", "01110", "00111", "0000"];
const EXPECTED_RESERVED: [(&str, usize); 2] = [("0x080000b5", 7), ("0x080000be", 2)];
const EXPECTED_UNRESOLVED: [(&str, &str, usize); 6] = [("entry_branch", "0x08000000", 4), ("title", "0x080000a0", 12), ("game_code", "0x080000ac", 4), ("maker_code", "0x080000b0", 2), ("software_version", "0x080000bc", 1), ("complement_checksum", "0x080000bd", 1)];

fn sha256_hex(data: &[u8]) -> String {
    format!("{:x}", Sha256::digest(data))
}

#[derive(Clone, Debug)]
struct Edition {
    target: u32,
    title: String,
    game_code: String,
    maker_code: String,
    software_version: u8,
}

fn object<'a>(value: &'a Value, label: &str) -> Result<&'a Map<String, Value>, String> {
    value.as_object().ok_or_else(|| format!("{label} must be an object"))
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
    value.as_f64().is_some_and(|actual| actual.is_finite() && actual == expected as f64)
}

fn safe_positive_integer(value: Option<&Value>) -> Option<usize> {
    let number = value?.as_f64()?;
    if !number.is_finite() || number.fract() != 0.0 || number <= 0.0 || number > 9_007_199_254_740_991.0 {
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
    let text = value.and_then(Value::as_str).ok_or_else(|| format!("{label} is not a canonical address"))?;
    if text.len() != 10 || !text.starts_with("0x") || !text[2..].bytes().all(|byte| byte.is_ascii_hexdigit() && !byte.is_ascii_uppercase()) {
        return Err(format!("{label} is not a canonical address"));
    }
    let address = u32::from_str_radix(&text[2..], 16).map_err(|_| format!("{label} is not a canonical address"))?;
    Ok((text.to_string(), address))
}

fn parse_record(value: &Value, label: &str, with_name: bool) -> Result<(Option<String>, String, usize), String> {
    let record = object(value, label)?;
    let keys = if with_name { &["name", "address", "size"][..] } else { &["address", "size"][..] };
    exact_keys(record, keys, label)?;
    let name = if with_name { Some(record.get("name").and_then(Value::as_str).ok_or_else(|| format!("{label} name differs"))?.to_string()) } else { None };
    let (address, _) = canonical_address(record.get("address"), &format!("{label} address"))?;
    let size = safe_positive_integer(record.get("size"));
    if size.is_none() {
        return Err(format!("{label} size differs"));
    }
    Ok((name, address, size.unwrap()))
}

fn parse_ascii(value: Option<&Value>, length: usize, label: &str) -> Result<String, String> {
    let text = value.and_then(Value::as_str).ok_or_else(|| format!("{label} differs"))?;
    if text.len() != length || !text.bytes().all(|byte| byte.is_ascii_uppercase() || byte.is_ascii_digit()) {
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
    exact_keys(edition, &["entry_branch", "title", "game_code", "maker_code", "software_version", "complement_checksum"], "GBA header edition")?;
    let entry = object(edition.get("entry_branch").ok_or("GBA entry branch must be an object")?, "GBA entry branch")?;
    exact_keys(entry, &["instruction_set", "operation", "target"], "GBA entry branch")?;
    if entry.get("instruction_set").and_then(Value::as_str) != Some("arm") || entry.get("operation").and_then(Value::as_str) != Some("b") {
        return Err("GBA entry branch differs".to_string());
    }
    let (_, target) = canonical_address(entry.get("target"), "GBA entry target")?;
    encode_arm_branch(GBA_HEADER_ADDRESS, target)?;

    let title = object(edition.get("title").ok_or("GBA title must be an object")?, "GBA title")?;
    exact_keys(title, &["text", "padding", "field_bytes"], "GBA title")?;
    let title_text = title.get("text").and_then(Value::as_str).ok_or("GBA title encoding differs")?;
    if title.get("padding").and_then(Value::as_str) != Some("nul") || !number_equals(title.get("field_bytes"), 12) {
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
    Ok(Edition { target, title: title_text.to_string(), game_code, maker_code, software_version: version.unwrap() as u8 })
}

pub fn parse_gba_header_source(value: &Value) -> Result<(), String> {
    let source = object(value, "GBA header source")?;
    exact_keys(source, &["format", "kind", "address", "standard", "edition", "unresolved_fields"], "GBA header source")?;
    if !number_equals(source.get("format"), 2) || source.get("kind").and_then(Value::as_str) != Some("gba-cartridge-header-standard-fields") || source.get("address").and_then(Value::as_str) != Some("0x08000000") {
        return Err("unsupported GBA header source".to_string());
    }
    let standard = object(source.get("standard").ok_or("GBA header standard fields must be an object")?, "GBA header standard fields")?;
    exact_keys(standard, &["logo", "fixed_value", "unit_code", "device_type", "reserved_zero_ranges"], "GBA header standard fields")?;
    let logo = object(standard.get("logo").ok_or("GBA header logo must be an object")?, "GBA header logo")?;
    exact_keys(logo, &["codec", "source", "width", "height", "bpp"], "GBA header logo")?;
    if logo.get("codec").and_then(Value::as_str) != Some("gba-bios-huffman-logo")
        || logo.get("source").and_then(Value::as_str).is_none_or(str::is_empty)
        || !number_equals(logo.get("width"), GBA_LOGO_WIDTH as u64)
        || !number_equals(logo.get("height"), GBA_LOGO_HEIGHT as u64)
        || !number_equals(logo.get("bpp"), 1)
        || standard.get("fixed_value").and_then(Value::as_str) != Some("0x96")
        || standard.get("unit_code").and_then(Value::as_str) != Some("0x00")
        || standard.get("device_type").and_then(Value::as_str) != Some("0x00")
    {
        return Err("GBA header standard values differ".to_string());
    }
    let reserved = source_array(standard.get("reserved_zero_ranges"), "GBA header reserved ranges differ")?;
    let parsed_reserved: Vec<(Option<String>, String, usize)> = reserved.iter().enumerate().map(|(index, value)| parse_record(value, &format!("GBA header reserved range {index}"), false)).collect::<Result<_, _>>()?;
    if parsed_reserved.len() != EXPECTED_RESERVED.len() || parsed_reserved.iter().zip(EXPECTED_RESERVED).any(|((_, address, size), (expected_address, expected_size))| address != expected_address || *size != expected_size) {
        return Err("GBA header reserved ranges differ".to_string());
    }

    let unresolved = source_array(source.get("unresolved_fields"), "GBA header unresolved fields differ")?;
    let parsed_unresolved: Vec<(Option<String>, String, usize)> = unresolved.iter().enumerate().map(|(index, value)| parse_record(value, &format!("GBA header unresolved field {index}"), true)).collect::<Result<_, _>>()?;
    match source.get("edition") {
        Some(Value::Null) => {
            if parsed_unresolved.len() != EXPECTED_UNRESOLVED.len() || parsed_unresolved.iter().zip(EXPECTED_UNRESOLVED).any(|((name, address, size), (expected_name, expected_address, expected_size))| name.as_deref() != Some(expected_name) || address != expected_address || *size != expected_size) {
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
    value.and_then(Value::as_array).ok_or_else(|| message.to_string())
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
                    let mut word = u32::from_le_bytes(output[word_offset..word_offset + 4].try_into().unwrap());
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
    if decoded.width != GBA_LOGO_WIDTH || decoded.height != GBA_LOGO_HEIGHT || decoded.palette != vec![[255, 255, 255], [0, 0, 0]] || sha256_hex(image) != LOGO_PNG_SHA256 {
        return Err("GBA logo source must be the canonical 104x16 monochrome PNG".to_string());
    }
    let output = huffman_logo(&addition_deltas(&tiled_logo_bits(&pixels)))?;
    if sha256_hex(&output) != LOGO_SHA256 {
        return Err("GBA logo source does not encode the standard firmware logo".to_string());
    }
    Ok(output)
}

pub fn encode_arm_branch(address: u32, target: u32) -> Result<Vec<u8>, String> {
    if address > 0xffff_fffc || target > 0xffff_fffc || !address.is_multiple_of(4) || !target.is_multiple_of(4) {
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

pub fn build_gba_header_component(source: &Value, logo_image: &[u8], address: u32, size: usize) -> Result<Vec<u8>, String> {
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
    if object(source, "GBA header source")?.get("edition").is_some_and(|value| !value.is_null()) && size > 0 && address >= GBA_HEADER_ADDRESS && (address as u64 + size as u64) <= GBA_HEADER_ADDRESS as u64 + GBA_HEADER_SIZE as u64 {
        let header = build_gba_header(source, logo_image)?;
        let offset = (address - GBA_HEADER_ADDRESS) as usize;
        return Ok(header[offset..offset + size].to_vec());
    }
    Err("GBA header component is not independently determined".to_string())
}
