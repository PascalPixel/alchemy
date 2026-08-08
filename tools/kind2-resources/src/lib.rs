//! Native build and ROM verification for Golden Sun Kind-2 resources.
//!
//! This crate owns the build-facing API and its verifier.

use import_asset::{gba_graphics, gba_palette_rgba, indexed_png};
use serde_json::{Map, Value};
use std::fs;
use std::io::{Read, Write};
use std::path::{Path, PathBuf};

pub type Result<T> = std::result::Result<T, String>;
pub const ROM_BASE: usize = 0x0800_0000;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct BuiltResource {
    pub id: usize,
    pub address: usize,
    pub data: Vec<u8>,
    pub prefix_palette_size: usize,
    pub presentation_status: String,
    pub sources: Vec<PathBuf>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Token {
    Literal(usize),
    Copy { distance: usize, length: usize },
}

fn err(message: impl Into<String>) -> String {
    message.into()
}
fn read(path: &Path) -> Result<Vec<u8>> {
    fs::read(path).map_err(|e| format!("{}: {e}", path.display()))
}
fn json(path: &Path) -> Result<Value> {
    let bytes = read(path)?;
    serde_json::from_slice(&bytes).map_err(|e| format!("{}: {e}", path.display()))
}
fn object<'a>(value: &'a Value, label: &str) -> Result<&'a Map<String, Value>> {
    value
        .as_object()
        .ok_or_else(|| err(format!("{label} must be an object")))
}
fn field<'a>(value: &'a Map<String, Value>, name: &str) -> Result<&'a Value> {
    value
        .get(name)
        .ok_or_else(|| err(format!("missing {name}")))
}
fn string(value: &Value, label: &str) -> Result<String> {
    value
        .as_str()
        .map(str::to_owned)
        .ok_or_else(|| err(format!("{label} must be a string")))
}
fn parse_hex(value: &str, label: &str) -> Result<usize> {
    let digits = value
        .strip_prefix("0x")
        .ok_or_else(|| err(format!("{label} is not canonical")))?;
    usize::from_str_radix(digits, 16).map_err(|_| err(format!("{label} is not canonical")))
}
fn number(value: &Value, label: &str) -> Result<usize> {
    value
        .as_u64()
        .map(|n| n as usize)
        .ok_or_else(|| err(format!("{label} must be an integer")))
}
fn decode_hex(value: &str) -> Result<Vec<u8>> {
    if value.len() % 2 != 0 || !value.bytes().all(|b| b.is_ascii_hexdigit()) {
        return Err("invalid kind-2 lookahead".into());
    }
    (0..value.len())
        .step_by(2)
        .map(|i| {
            u8::from_str_radix(&value[i..i + 2], 16).map_err(|_| err("invalid kind-2 lookahead"))
        })
        .collect()
}
fn child(root: &Path, name: &str) -> Result<PathBuf> {
    if name.contains('/') || name.contains('\\') || name == "." || name == ".." {
        return Err("kind-2 source name must be a direct child".into());
    }
    Ok(root.join(name))
}
fn indexed_child(root: &Path, name: &str, id: usize) -> Result<PathBuf> {
    let expected = format!("resource_{id:03x}/stream.json");
    if name != expected {
        return Err("kind-2 index source name is not canonical".into());
    }
    Ok(root.join(name))
}

struct BitWriter {
    bits: Vec<u8>,
}
impl BitWriter {
    fn new() -> Self {
        Self { bits: Vec::new() }
    }
    fn put(&mut self, value: bool) {
        self.bits.push(u8::from(value));
    }
    fn write(&mut self, value: usize, count: usize) {
        for index in 0..count {
            self.bits.push(((value >> index) & 1) as u8);
        }
    }
    fn finish(&self) -> Vec<u8> {
        let mut result = vec![0; self.bits.len().div_ceil(8)];
        for (index, bit) in self.bits.iter().enumerate() {
            result[index >> 3] |= bit << (index & 7);
        }
        result
    }
}
fn distance_width(produced: usize) -> usize {
    if produced < 33 || produced >= 2081 {
        12
    } else {
        (32 - (produced as u32 - 33).leading_zeros()) as usize
    }
}
fn put_length(bits: &mut BitWriter, length: usize) -> Result<()> {
    match length {
        2 => bits.put(false),
        3 => {
            bits.write(1, 1);
            bits.put(false);
        }
        4 => {
            bits.write(3, 2);
            bits.put(false);
        }
        5 => {
            bits.write(7, 3);
            bits.put(false);
        }
        6 | 7 => {
            bits.write(15, 4);
            bits.put(false);
            bits.put(length == 7);
        }
        8..=10 => {
            bits.write(31, 5);
            bits.write(length - 7, 2);
        }
        11..=137 => {
            bits.write(31, 5);
            bits.write(0, 2);
            bits.write(length - 10, 7);
        }
        _ => return Err("tag-2 copy length is outside 2..137".into()),
    }
    Ok(())
}
fn mtf_index(table: &mut Vec<u8>, value: u8) -> usize {
    let index = table
        .iter()
        .position(|candidate| *candidate == value)
        .unwrap_or(0);
    let selected = table.remove(index);
    table.insert(0, selected);
    index
}

pub fn encode_kind2(decoded: &[u8], tokens: &[Token], lookahead: &[u8]) -> Result<Vec<u8>> {
    let mut bits = BitWriter::new();
    let mut table: Vec<u8> = (0..16).collect();
    let mut cursor = 0usize;
    for token in tokens {
        match *token {
            Token::Literal(width) => {
                if !matches!(width, 2..=4) || cursor >= decoded.len() {
                    return Err("invalid tag-2 literal token".into());
                }
                bits.put(true);
                if width == 2 {
                    bits.put(true);
                } else {
                    bits.put(false);
                    bits.put(width == 3);
                }
                let value = decoded[cursor];
                let low = mtf_index(&mut table, value & 15);
                let high = mtf_index(&mut table, value >> 4);
                if low >= 1 << width || high >= 1 << width {
                    return Err("literal does not fit its recorded MTF width".into());
                }
                bits.write(low, width);
                bits.write(high, width);
                cursor += 1;
            }
            Token::Copy { distance, length } => {
                if distance > cursor || cursor + length > decoded.len() {
                    return Err("tag-2 copy is outside decoded data".into());
                }
                for index in 0..length {
                    if decoded[cursor + index] != decoded[cursor + index - distance] {
                        return Err("tag-2 copy token differs from source pixels".into());
                    }
                }
                bits.put(false);
                put_length(&mut bits, length)?;
                if distance <= 32 {
                    bits.put(true);
                    bits.write(distance - 1, 5);
                } else {
                    bits.put(false);
                    let width = distance_width(cursor);
                    let value = distance - 33;
                    if value >= 1 << width {
                        return Err("tag-2 long distance does not fit".into());
                    }
                    bits.write(value, width);
                }
                cursor += length;
            }
        }
    }
    if cursor != decoded.len() {
        return Err("tag-2 tokens do not cover decoded data".into());
    }
    bits.put(false);
    bits.write(31, 5);
    bits.write(0, 2);
    bits.write(0, 7);
    let mut result = vec![2];
    result.extend(bits.finish());
    result.extend(lookahead);
    Ok(result)
}

fn parse_tokens(value: &Value) -> Result<Vec<Token>> {
    value
        .as_array()
        .ok_or_else(|| err("tag-2 tokens must be an array"))?
        .iter()
        .enumerate()
        .map(|(index, token)| {
            if let Some(width) = token.as_u64() {
                return Ok(Token::Literal(width as usize));
            }
            let pair = token
                .as_array()
                .ok_or_else(|| err(format!("invalid tag-2 token {index}")))?;
            if pair.len() != 2 {
                return Err(format!("invalid tag-2 token {index}"));
            }
            Ok(Token::Copy {
                distance: number(&pair[0], "tag-2 copy distance")?,
                length: number(&pair[1], "tag-2 copy length")?,
            })
        })
        .collect()
}

fn parse_encoding_plan(path: &Path) -> Result<(usize, Vec<Token>, Vec<u8>, Option<usize>)> {
    let value = json(path)?;
    let plan = object(&value, "kind-2 encoding plan")?;
    if number(field(plan, "format")?, "format")? != 1
        || string(field(plan, "codec")?, "codec")? != "golden-sun-kind2-lz"
    {
        return Err("unsupported tag-2 plan".into());
    }
    let decoded_size = number(field(plan, "decoded_size")?, "decoded_size")?;
    let tokens = parse_tokens(field(plan, "tokens")?)?;
    let lookahead = decode_hex(plan.get("lookahead").and_then(Value::as_str).unwrap_or(""))?;
    if lookahead.len() > 3 {
        return Err("tag-2 lookahead is too long".into());
    }
    let encoded_size = plan
        .get("encoded_size")
        .map(|value| number(value, "encoded_size"))
        .transpose()?;
    Ok((decoded_size, tokens, lookahead, encoded_size))
}

pub fn encode_kind2_plan(decoded: &[u8], plan_path: &Path) -> Result<Vec<u8>> {
    let (decoded_size, tokens, lookahead, encoded_size) = parse_encoding_plan(plan_path)?;
    if decoded.len() != decoded_size {
        return Err("tag-2 decoded data has the wrong size".into());
    }
    let encoded = encode_kind2(decoded, &tokens, &lookahead)?;
    if encoded_size.is_some_and(|size| size != encoded.len()) {
        return Err("tag-2 stream has the wrong encoded size".into());
    }
    Ok(encoded)
}

struct Plan {
    id: usize,
    address: usize,
    size: usize,
    encoding: String,
    image: String,
    status: String,
    tokens: Vec<Token>,
    decoded_size: usize,
    encoded_size: usize,
    lookahead: Vec<u8>,
    prefix: usize,
    palette_image: Option<String>,
}
fn parse_plan(path: &Path) -> Result<Plan> {
    let value = json(path)?;
    let plan = object(&value, "kind-2 plan")?;
    let id = parse_hex(
        &string(field(plan, "resource_id")?, "resource_id")?,
        "resource_id",
    )?;
    let address = parse_hex(&string(field(plan, "address")?, "address")?, "address")?;
    let size = parse_hex(&string(field(plan, "size")?, "size")?, "size")?;
    let image = object(field(plan, "image")?, "kind-2 image")?;
    let encoding = string(field(image, "encoding")?, "encoding")?;
    let image_name = string(field(image, "source")?, "image source")?;
    let status = string(field(image, "status")?, "image status")?;
    let stream = object(field(plan, "stream")?, "kind-2 stream")?;
    let encoded_size = parse_hex(
        &string(field(stream, "encoded_size")?, "encoded_size")?,
        "encoded_size",
    )?;
    let decoded_size = parse_hex(
        &string(field(stream, "decoded_size")?, "decoded_size")?,
        "decoded_size",
    )?;
    let lookahead = decode_hex(&string(field(stream, "lookahead")?, "lookahead")?)?;
    let (prefix, palette_image) = match plan.get("prefix_palette") {
        Some(Value::Object(value)) => (
            parse_hex(
                &string(field(value, "size")?, "palette size")?,
                "palette size",
            )?,
            Some(string(field(value, "source")?, "palette source")?),
        ),
        _ => (0, None),
    };
    if encoded_size != size.saturating_sub(prefix) || lookahead.len() > 3 {
        return Err("kind-2 stream extent is invalid".into());
    }
    Ok(Plan {
        id,
        address,
        size,
        encoding,
        image: image_name,
        status,
        tokens: parse_tokens(field(stream, "tokens")?)?,
        decoded_size,
        encoded_size,
        lookahead,
        prefix,
        palette_image,
    })
}

pub fn build_kind2_resource(plan_path: &Path) -> Result<BuiltResource> {
    let plan = parse_plan(plan_path)?;
    let directory = plan_path
        .parent()
        .ok_or_else(|| err("kind-2 plan has no parent"))?;
    let image_path = child(directory, &plan.image)?;
    let image = read(&image_path)?;
    let decoded = if plan.encoding == "naiyou" {
        indexed_png(&image)
            .map_err(|e| e.0)?
            .pixels
            .into_iter()
            .map(|pixel| pixel as u8)
            .collect::<Vec<_>>()
    } else {
        let bpp = match plan.encoding.as_str() {
            "koma-4bpp" => 4.0,
            "haikei-8bpp" => 8.0,
            _ => return Err("unsupported kind-2 presentation".into()),
        };
        gba_graphics(&image, bpp).map_err(|e| e.0)?.0
    };
    if decoded.len() < plan.decoded_size
        || decoded[plan.decoded_size..].iter().any(|byte| *byte != 0)
    {
        return Err("kind-2 image has nonzero data outside its decoded stream".into());
    }
    let palette_path = plan
        .palette_image
        .as_deref()
        .map(|name| child(directory, name))
        .transpose()?;
    let prefix = if let Some(path) = &palette_path {
        gba_palette_rgba(&read(path)?).map_err(|e| e.0)?.0
    } else {
        Vec::new()
    };
    if prefix.len() != plan.prefix {
        return Err("kind-2 palette source has the wrong size".into());
    }
    let stream = encode_kind2(&decoded[..plan.decoded_size], &plan.tokens, &plan.lookahead)?;
    if stream.len() != plan.encoded_size {
        return Err("kind-2 stream has the wrong encoded size".into());
    }
    let mut data = prefix.clone();
    data.extend(stream);
    if data.len() != plan.size {
        return Err("kind-2 resource has the wrong size".into());
    }
    let mut sources = vec![plan_path.to_path_buf(), image_path];
    if let Some(path) = palette_path {
        sources.push(path);
    }
    Ok(BuiltResource {
        id: plan.id,
        address: plan.address,
        data,
        prefix_palette_size: prefix.len(),
        presentation_status: plan.status,
        sources,
    })
}

pub fn build_kind2_series(index_path: &Path) -> Result<Vec<BuiltResource>> {
    let value = json(index_path)?;
    let index = object(&value, "kind-2 index")?;
    if field(index, "kind")?.as_str() != Some("golden-sun-kind2-resource-series") {
        return Err("unsupported kind-2 series index".into());
    }
    let directory = index_path
        .parent()
        .ok_or_else(|| err("kind-2 index has no parent"))?;
    let entries = field(index, "resources")?
        .as_array()
        .ok_or_else(|| err("kind-2 index resources must be an array"))?;
    entries
        .iter()
        .map(|entry| {
            let entry = object(entry, "kind-2 index entry")?;
            let id = parse_hex(&string(field(entry, "id")?, "id")?, "id")?;
            let plan_path =
                indexed_child(directory, &string(field(entry, "source")?, "source")?, id)?;
            let built = build_kind2_resource(&plan_path)?;
            let address = parse_hex(&string(field(entry, "address")?, "address")?, "address")?;
            let size = parse_hex(&string(field(entry, "size")?, "size")?, "size")?;
            if built.id != id || built.address != address || built.data.len() != size {
                return Err("kind-2 resource differs from its series index".into());
            }
            Ok(built)
        })
        .collect()
}

fn range(rom: &[u8], address: usize, size: usize) -> Result<&[u8]> {
    rom.get(
        address
            .checked_sub(ROM_BASE)
            .ok_or_else(|| err("kind-2 range is outside the ROM"))?
            ..address.checked_sub(ROM_BASE).unwrap() + size,
    )
    .ok_or_else(|| err("kind-2 range is outside the ROM"))
}
pub fn verify_kind2_series(rom_path: &Path, index_path: &Path) -> Result<String> {
    let rom = read(rom_path)?;
    let built = build_kind2_series(index_path)?;
    let mut bytes = 0;
    for resource in &built {
        if range(&rom, resource.address, resource.data.len())? != resource.data {
            return Err(format!(
                "kind-2 resource 0x{:03x} differs from ROM",
                resource.id
            ));
        }
        bytes += resource.data.len();
    }
    Ok(format!(
        "identical=true resources={} source_bytes={bytes}",
        built.len()
    ))
}
pub fn verify_kind2_resource(rom_path: &Path, plan_path: &Path) -> Result<String> {
    let rom = read(rom_path)?;
    let built = build_kind2_resource(plan_path)?;
    if range(&rom, built.address, built.data.len())? != built.data {
        return Err(format!(
            "kind-2 resource 0x{:03x} differs from ROM",
            built.id
        ));
    }
    Ok(format!(
        "identical=true resource=0x{:03x} source_bytes={}",
        built.id,
        built.data.len()
    ))
}
pub fn self_test() -> Result<()> {
    let prefix: Vec<u8> = (0..32)
        .map(|index| ((index * 37 + 11) & 255) as u8)
        .collect();
    let mut decoded = prefix.clone();
    decoded.extend_from_slice(&prefix[16..]);
    decoded.extend_from_slice(&prefix[16..]);
    let mut tokens = vec![Token::Literal(4); 32];
    tokens.push(Token::Copy {
        distance: 16,
        length: 32,
    });
    let encoded = encode_kind2(&decoded, &tokens, &[0xa5, 0x5a])?;
    if encoded.first() != Some(&2) || encoded.len() <= 3 {
        return Err("kind-2 codec self-test failed".into());
    }
    Ok(())
}
pub fn write_build_stdout(plan: &Path) -> Result<()> {
    let built = build_kind2_resource(plan)?;
    std::io::stdout()
        .write_all(&built.data)
        .map_err(|e| e.to_string())
}

pub fn write_encode_stdout(plan: &Path) -> Result<()> {
    let mut decoded = Vec::new();
    std::io::stdin()
        .read_to_end(&mut decoded)
        .map_err(|e| e.to_string())?;
    let encoded = encode_kind2_plan(&decoded, plan)?;
    std::io::stdout()
        .write_all(&encoded)
        .map_err(|e| e.to_string())
}
