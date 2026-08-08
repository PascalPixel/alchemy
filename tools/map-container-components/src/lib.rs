//! Native builders and exporters for Golden Sun map-container components.
//!
//! This is the Rust equivalent of `tools/make/map_container_components.ts`.
//! The TypeScript module remains available because the asset builders still
//! import its library functions during the migration.

use canonical_json::canonical_json;
use extract_resource::{
    decode_general_trace, decode_palette_trace, encode_general, encode_palette, GeneralToken,
    PaletteGroup, PaletteOperation,
};
use serde_json::{json, Map, Value};
use std::collections::BTreeMap;
use std::fs;
use std::path::Path;
use tilemap::{export_tilemap, import_tilemap};

pub type Result<T> = std::result::Result<T, String>;
pub type OffsetChecks = BTreeMap<usize, usize>;

const MAX_DECODED: u64 = 0x20_0000;

fn err<T>(message: impl Into<String>) -> Result<T> {
    Err(message.into())
}

fn read(path: &Path) -> Result<Vec<u8>> {
    fs::read(path).map_err(|error| format!("{}: {error}", path.display()))
}

fn write(path: &Path, data: &[u8]) -> Result<()> {
    if let Some(parent) = path.parent() {
        fs::create_dir_all(parent).map_err(|error| format!("{}: {error}", parent.display()))?;
    }
    fs::write(path, data).map_err(|error| format!("{}: {error}", path.display()))
}

fn json_file(path: &Path) -> Result<Value> {
    let text =
        String::from_utf8(read(path)?).map_err(|error| format!("{}: {error}", path.display()))?;
    serde_json::from_str(&text).map_err(|error| format!("{}: {error}", path.display()))
}

fn canonical_file(path: &Path, value: &Value) -> Result<()> {
    write(path, format!("{}\n", canonical_json(value)).as_bytes())
}

fn compact_json(value: &Value) -> Result<Vec<u8>> {
    let mut output = serde_json::to_vec(value).map_err(|error| error.to_string())?;
    output.push(b'\n');
    Ok(output)
}

fn object<'a>(value: &'a Value, label: &str) -> Result<&'a Map<String, Value>> {
    value
        .as_object()
        .ok_or_else(|| format!("{label} must be an object"))
}

fn field<'a>(object: &'a Map<String, Value>, key: &str) -> Result<&'a Value> {
    object
        .get(key)
        .ok_or_else(|| format!("missing field {key}"))
}

/// JavaScript `Number(value)` for the integer-shaped values accepted by this
/// tool. Plans use both JSON numbers and hexadecimal strings.
fn number(value: &Value, label: &str) -> Result<u64> {
    if let Some(value) = value.as_u64() {
        return Ok(value);
    }
    if let Some(value) = value.as_i64() {
        return if value >= 0 {
            Ok(value as u64)
        } else {
            err(format!("{label} must be an integer"))
        };
    }
    if let Some(value) = value.as_str() {
        let value = value.trim();
        let parsed = if let Some(value) = value
            .strip_prefix("0x")
            .or_else(|| value.strip_prefix("0X"))
        {
            u64::from_str_radix(value, 16)
        } else {
            value.parse::<u64>()
        };
        return parsed.map_err(|_| format!("{label} must be an integer"));
    }
    err(format!("{label} must be an integer"))
}

fn byte(value: &Value, label: &str) -> Result<u8> {
    let value = number(value, label)?;
    if value > u8::MAX as u64 {
        return err(format!("{label} is outside u8"));
    }
    Ok(value as u8)
}

fn word(value: &Value, label: &str) -> Result<u16> {
    let value = number(value, label)?;
    if value > u16::MAX as u64 {
        return err(format!("{label} is outside u16"));
    }
    Ok(value as u16)
}

fn hex(data: &[u8]) -> String {
    data.iter().map(|value| format!("{value:02x}")).collect()
}

fn parse_hex(value: &str) -> Result<Vec<u8>> {
    if value.len() % 2 != 0 {
        return err("hex data has an odd length");
    }
    (0..value.len())
        .step_by(2)
        .map(|index| {
            u8::from_str_radix(&value[index..index + 2], 16)
                .map_err(|_| "invalid hex data".to_string())
        })
        .collect()
}

fn u16s(data: &[u8]) -> Result<Vec<u16>> {
    if data.len() % 2 != 0 {
        return err("partial u16");
    }
    Ok(data
        .chunks_exact(2)
        .map(|chunk| u16::from_le_bytes([chunk[0], chunk[1]]))
        .collect())
}

fn pack_u16(words: &[u16]) -> Vec<u8> {
    words.iter().flat_map(|word| word.to_le_bytes()).collect()
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Codec {
    General,
    TaggedPalette,
}

impl Codec {
    fn name(self) -> &'static str {
        match self {
            Self::General => "golden-sun-general-lz",
            Self::TaggedPalette => "golden-sun-tagged-palette-lz",
        }
    }
}

#[derive(Debug, Clone)]
pub struct DecodedComponent {
    pub decoded: Vec<u8>,
    pub used: usize,
    codec: Codec,
    general_tokens: Vec<GeneralToken>,
    palette_groups: Vec<PaletteGroup>,
}

pub fn decode_component(encoded: &[u8]) -> Result<DecodedComponent> {
    match encoded.first().copied() {
        Some(0) => {
            let (decoded, used, tokens) =
                decode_general_trace(encoded, 0, encoded.len(), MAX_DECODED)
                    .map_err(|error| error.0)?;
            Ok(DecodedComponent {
                decoded,
                used,
                codec: Codec::General,
                general_tokens: tokens,
                palette_groups: Vec::new(),
            })
        }
        Some(1) => {
            let (decoded, used, groups) =
                decode_palette_trace(encoded, 1, encoded.len(), MAX_DECODED)
                    .map_err(|error| error.0)?;
            Ok(DecodedComponent {
                decoded,
                used,
                codec: Codec::TaggedPalette,
                general_tokens: Vec::new(),
                palette_groups: groups,
            })
        }
        _ => err("unsupported map-component compression tag"),
    }
}

fn general_token_value(token: &GeneralToken) -> Value {
    match token {
        GeneralToken::Literal(count) => json!(["l", count]),
        GeneralToken::Copy { length, distance } => json!(["c", length, distance]),
    }
}

fn palette_operation_value(operation: &PaletteOperation) -> Value {
    match operation {
        PaletteOperation::Literal => json!(["l"]),
        PaletteOperation::End => json!(["e"]),
        PaletteOperation::Copy { length, distance } => json!(["c", length, distance]),
    }
}

fn palette_group_value(group: &PaletteGroup) -> Value {
    match group {
        PaletteGroup::Zeros => json!(["z"]),
        PaletteGroup::Group(operations) => json!([
            "g",
            operations
                .iter()
                .map(palette_operation_value)
                .collect::<Vec<_>>()
        ]),
    }
}

fn plan_for(component: &DecodedComponent, encoded: &[u8], extra: Value) -> Result<Value> {
    let mut output = Map::new();
    output.insert("format".into(), json!(1));
    output.insert("codec".into(), json!(component.codec.name()));
    if component.codec == Codec::TaggedPalette {
        output.insert("tag".into(), json!(1));
    }
    output.insert("decoded_size".into(), json!(component.decoded.len()));
    output.insert("encoded_size".into(), json!(encoded.len()));
    output.insert(
        "tokens".into(),
        match component.codec {
            Codec::General => Value::Array(
                component
                    .general_tokens
                    .iter()
                    .map(general_token_value)
                    .collect(),
            ),
            Codec::TaggedPalette => Value::Array(
                component
                    .palette_groups
                    .iter()
                    .map(palette_group_value)
                    .collect(),
            ),
        },
    );
    let replay = match component.codec {
        Codec::General => encode_general(&component.decoded, &component.general_tokens)
            .map_err(|error| error.0)?,
        Codec::TaggedPalette => {
            let mut replay = vec![1];
            replay.extend(
                encode_palette(&component.decoded, &component.palette_groups)
                    .map_err(|error| error.0)?,
            );
            replay
        }
    };
    if replay.len() > encoded.len()
        || encoded[..replay.len()] != replay
        || component.used > encoded.len()
    {
        return err(match component.codec {
            Codec::General => "general-LZ token replay differs from component span",
            Codec::TaggedPalette => "tagged palette-LZ token replay differs from component span",
        });
    }
    output.insert("lookahead".into(), json!(hex(&encoded[replay.len()..])));
    let extra = object(&extra, "plan metadata")?;
    for (key, value) in extra {
        output.insert(key.clone(), value.clone());
    }
    Ok(Value::Object(output))
}

fn general_tokens(value: &Value) -> Result<Vec<GeneralToken>> {
    let tokens = value
        .as_array()
        .ok_or_else(|| "tokens must be an array".to_string())?;
    tokens
        .iter()
        .enumerate()
        .map(|(index, value)| {
            let token = value
                .as_array()
                .ok_or_else(|| format!("token {index} is invalid"))?;
            match token.first().and_then(Value::as_str) {
                Some("l") if token.len() == 2 => {
                    Ok(GeneralToken::Literal(
                        number(&token[1], "literal count")? as u32
                    ))
                }
                Some("c") if token.len() == 3 => Ok(GeneralToken::Copy {
                    length: number(&token[1], "copy length")? as u32,
                    distance: number(&token[2], "copy distance")? as u32,
                }),
                _ => err(format!("token {index} is invalid")),
            }
        })
        .collect()
}

fn palette_operations(value: &Value) -> Result<Vec<PaletteOperation>> {
    let values = value
        .as_array()
        .ok_or_else(|| "palette operations must be an array".to_string())?;
    values
        .iter()
        .enumerate()
        .map(|(index, value)| {
            let operation = value
                .as_array()
                .ok_or_else(|| format!("palette operation {index} is invalid"))?;
            match operation.first().and_then(Value::as_str) {
                Some("l") if operation.len() == 1 => Ok(PaletteOperation::Literal),
                Some("e") if operation.len() == 1 => Ok(PaletteOperation::End),
                Some("c") if operation.len() == 3 => Ok(PaletteOperation::Copy {
                    length: number(&operation[1], "palette copy length")? as u32,
                    distance: number(&operation[2], "palette copy distance")? as u32,
                }),
                _ => err(format!("palette operation {index} is invalid")),
            }
        })
        .collect()
}

fn palette_groups(value: &Value) -> Result<Vec<PaletteGroup>> {
    let values = value
        .as_array()
        .ok_or_else(|| "palette tokens must be an array".to_string())?;
    values
        .iter()
        .enumerate()
        .map(|(index, value)| {
            let group = value
                .as_array()
                .ok_or_else(|| format!("palette group {index} is invalid"))?;
            match group.first().and_then(Value::as_str) {
                Some("z") if group.len() == 1 => Ok(PaletteGroup::Zeros),
                Some("g") if group.len() == 2 => {
                    Ok(PaletteGroup::Group(palette_operations(&group[1])?))
                }
                _ => err(format!("palette group {index} is invalid")),
            }
        })
        .collect()
}

pub fn encode_plan(decoded: &[u8], plan: &Value) -> Result<Vec<u8>> {
    let plan = object(plan, "map-component LZ plan")?;
    if number(field(plan, "format")?, "format")? != 1 {
        return err("unsupported map-component LZ plan");
    }
    let codec = field(plan, "codec")?
        .as_str()
        .ok_or_else(|| "unsupported map-component LZ plan".to_string())?;
    let decoded_size = number(field(plan, "decoded_size")?, "decoded size")?;
    if decoded.len() as u64 != decoded_size {
        return err("map-component decoded size differs from plan");
    }
    let tokens = field(plan, "tokens")?;
    let mut encoded = match codec {
        "golden-sun-general-lz" => {
            encode_general(decoded, &general_tokens(tokens)?).map_err(|error| error.0)?
        }
        "golden-sun-tagged-palette-lz" => {
            if plan
                .get("tag")
                .map(|value| number(value, "tag"))
                .transpose()?
                .unwrap_or(0)
                != 1
            {
                return err("tagged palette-LZ plan is missing tag 1");
            }
            let mut output = vec![1];
            output.extend(
                encode_palette(decoded, &palette_groups(tokens)?).map_err(|error| error.0)?,
            );
            output
        }
        _ => return err("unsupported map-component LZ plan"),
    };
    let lookahead = match plan.get("lookahead") {
        Some(Value::String(value)) => parse_hex(value)?,
        Some(_) => return err("map-component plan lookahead must be hexadecimal"),
        None => Vec::new(),
    };
    encoded.extend(lookahead);
    Ok(encoded)
}

pub fn export_header(container: &[u8], source: &Path) -> Result<Vec<usize>> {
    if container.len() < 0x3c {
        return err("map container is shorter than its 0x3c header");
    }
    let header = &container[..0x3c];
    let parameters = header[..0x0c].to_vec();
    let records: Vec<Value> = (0..3)
        .map(|index| {
            Value::Array(
                (0..4)
                    .map(|field| {
                        json!(u16::from_le_bytes([
                            header[0x0c + index * 8 + field * 2],
                            header[0x0d + index * 8 + field * 2]
                        ]))
                    })
                    .collect(),
            )
        })
        .collect();
    let offsets: Vec<Value> = (0..6)
        .map(|index| {
            json!(format!(
                "0x{:x}",
                u32::from_le_bytes([
                    header[0x24 + index * 4],
                    header[0x25 + index * 4],
                    header[0x26 + index * 4],
                    header[0x27 + index * 4]
                ])
            ))
        })
        .collect();
    let document = json!({
        "format": 1,
        "parameters": parameters,
        "records": records,
        "component_offsets": offsets,
    });
    canonical_file(source, &document)?;
    if build_header(source, None)? != header {
        return err("container header does not round-trip");
    }
    Ok((0..6)
        .map(|index| {
            u32::from_le_bytes([
                header[0x24 + index * 4],
                header[0x25 + index * 4],
                header[0x26 + index * 4],
                header[0x27 + index * 4],
            ]) as usize
        })
        .collect())
}

pub fn build_header(source: &Path, offsets_check: Option<&OffsetChecks>) -> Result<Vec<u8>> {
    let document = json_file(source)?;
    let object = object(&document, "map container header")?;
    if number(field(object, "format")?, "format")? != 1 {
        return err("unsupported map container header source");
    }
    let parameters = field(object, "parameters")?
        .as_array()
        .ok_or_else(|| "container header requires twelve parameter bytes".to_string())?;
    if parameters.len() != 12 {
        return err("container header requires twelve parameter bytes");
    }
    let parameters: Vec<u8> = parameters
        .iter()
        .enumerate()
        .map(|(i, value)| byte(value, &format!("container parameter {i}")))
        .collect::<Result<_>>()?;
    let records = field(object, "records")?
        .as_array()
        .ok_or_else(|| "container header requires three four-u16 records".to_string())?;
    if records.len() != 3 {
        return err("container header requires three four-u16 records");
    }
    let records: Vec<Vec<u16>> = records
        .iter()
        .enumerate()
        .map(|(index, record)| {
            let record = record
                .as_array()
                .ok_or_else(|| "container header requires three four-u16 records".to_string())?;
            if record.len() != 4 {
                return err("container header requires three four-u16 records");
            }
            record
                .iter()
                .enumerate()
                .map(|(field, value)| word(value, &format!("record {index} field {field}")))
                .collect()
        })
        .collect::<Result<_>>()?;
    let offsets = field(object, "component_offsets")?
        .as_array()
        .ok_or_else(|| "container header requires six component offsets".to_string())?;
    if offsets.len() != 6 {
        return err("container header requires six component offsets");
    }
    let offsets: Vec<usize> = offsets
        .iter()
        .enumerate()
        .map(|(index, value)| {
            let value = number(value, &format!("component offset {index}"))?;
            if value > u32::MAX as u64 {
                return err("component offset is outside u32");
            }
            Ok(value as usize)
        })
        .collect::<Result<_>>()?;
    if let Some(checks) = offsets_check {
        for slot in 0..6 {
            let expected = checks.get(&slot).copied().unwrap_or(0);
            if offsets[slot] != expected {
                return err(format!(
                    "header offset {slot} differs from the claimed component span"
                ));
            }
        }
    }
    let mut output = vec![0u8; 0x3c];
    output[..0x0c].copy_from_slice(&parameters);
    for (index, record) in records.iter().enumerate() {
        for (field, value) in record.iter().enumerate() {
            output[0x0c + index * 8 + field * 2..0x0e + index * 8 + field * 2]
                .copy_from_slice(&value.to_le_bytes());
        }
    }
    for (index, value) in offsets.iter().enumerate() {
        output[0x24 + index * 4..0x28 + index * 4].copy_from_slice(&(*value as u32).to_le_bytes());
    }
    Ok(output)
}

pub fn decode_metatiles(decoded: &[u8]) -> Result<(u8, Vec<u16>)> {
    if decoded.is_empty() || !matches!(decoded[0], 0..=2) || (decoded.len() - 1) % 8 != 0 {
        return err("invalid 2x2-metatile component");
    }
    let mode = decoded[0];
    let count = (decoded.len() - 1) / 2;
    if mode == 0 || mode == 2 {
        let mut entries = u16s(&decoded[1..])?;
        if mode == 2 {
            let mut previous = 0;
            for value in &mut entries {
                *value ^= previous;
                previous = *value;
            }
        }
        return Ok((mode, entries));
    }
    let mut previous = 0;
    let mut entries = Vec::with_capacity(count);
    for index in 0..count {
        let mut value = u16::from_be_bytes([decoded[1 + index], decoded[1 + count + index]]);
        value ^= previous;
        previous = value;
        entries.push(value);
    }
    Ok((mode, entries))
}

pub fn encode_metatiles(entries: &[u16], mode: u8) -> Result<Vec<u8>> {
    if !matches!(mode, 0..=2) || entries.is_empty() || entries.len() % 4 != 0 {
        return err("metatiles require mode 0/1/2 and groups of four u16 entries");
    }
    if mode == 0 || mode == 2 {
        let mut transformed = Vec::with_capacity(entries.len());
        let mut previous = 0;
        for &value in entries {
            transformed.push(if mode == 2 { value ^ previous } else { value });
            previous = value;
        }
        let mut output = vec![mode];
        output.extend(pack_u16(&transformed));
        return Ok(output);
    }
    let mut planar = Vec::with_capacity(entries.len());
    let mut previous = 0;
    for &value in entries {
        let transformed = value ^ previous;
        planar.push(transformed);
        previous = value;
    }
    let mut output = vec![mode];
    output.extend(planar.iter().map(|value| (value >> 8) as u8));
    output.extend(planar.iter().map(|value| *value as u8));
    Ok(output)
}

pub fn export_metatiles(encoded: &[u8], source: &Path, plan_path: &Path) -> Result<(usize, u8)> {
    let component = decode_component(encoded)?;
    let (mode, entries) = decode_metatiles(&component.decoded)?;
    let tilemap = export_tilemap(&pack_u16(&entries), 4).map_err(|error| error.to_string())?;
    write(source, tilemap.as_bytes())?;
    let extra = json!({"component": "map-metatiles-2x2", "transform_mode": mode, "metatiles": entries.len() / 4});
    let plan = plan_for(&component, encoded, extra)?;
    write(plan_path, &compact_json(&plan)?)?;
    if build_metatiles(source, plan_path)? != encoded {
        return err("metatile component does not round-trip");
    }
    Ok((entries.len() / 4, mode))
}

pub fn build_metatiles(source: &Path, plan_path: &Path) -> Result<Vec<u8>> {
    let raw = import_tilemap(&String::from_utf8(read(source)?).map_err(|error| error.to_string())?)
        .map_err(|error| error.to_string())?;
    let plan = json_file(plan_path)?;
    let object = object(&plan, "metatile plan")?;
    let mode = byte(field(object, "transform_mode")?, "transform mode")?;
    let entries = u16s(&raw)?;
    let decoded = encode_metatiles(&entries, mode)?;
    encode_plan(&decoded, &plan)
}

pub fn export_descriptors(encoded: &[u8], source: &Path, plan_path: &Path) -> Result<usize> {
    let component = decode_component(encoded)?;
    if component.decoded.len() % 4 != 0 {
        return err("descriptor component is not a sequence of four-byte records");
    }
    let records: Vec<Value> = component
        .decoded
        .chunks_exact(4)
        .map(|record| json!(record.to_vec()))
        .collect();
    canonical_file(
        source,
        &json!({"format": 1, "record_size": 4, "records": records}),
    )?;
    let plan = plan_for(
        &component,
        encoded,
        json!({"component": "map-descriptors-4byte", "records": component.decoded.len() / 4}),
    )?;
    write(plan_path, &compact_json(&plan)?)?;
    if build_descriptors(source, plan_path)? != encoded {
        return err("descriptor component does not round-trip");
    }
    Ok(component.decoded.len() / 4)
}

pub fn build_descriptors(source: &Path, plan_path: &Path) -> Result<Vec<u8>> {
    let document = json_file(source)?;
    let object = object(&document, "map descriptor source")?;
    if number(field(object, "format")?, "format")? != 1
        || number(field(object, "record_size")?, "record size")? != 4
    {
        return err("unsupported map descriptor source");
    }
    let records = field(object, "records")?
        .as_array()
        .ok_or_else(|| "map descriptors must contain four byte values".to_string())?;
    let mut decoded = Vec::new();
    for record in records {
        let record = record
            .as_array()
            .ok_or_else(|| "map descriptors must contain four byte values".to_string())?;
        if record.len() != 4 {
            return err("map descriptors must contain four byte values");
        }
        for value in record {
            decoded.push(byte(value, "map descriptor byte")?);
        }
    }
    encode_plan(&decoded, &json_file(plan_path)?)
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct AnimationQueue {
    pub header: String,
    pub commands: Vec<[u16; 2]>,
}

pub fn decode_queues(decoded: &[u8]) -> Result<Vec<AnimationQueue>> {
    let words = u16s(decoded)?;
    let mut queues = Vec::new();
    let mut cursor = 0;
    while cursor < words.len() && words[cursor] != 0xffff {
        let header = words[cursor];
        if header >> 8 != 0xfd {
            return err("animation queue is missing its FDxx header");
        }
        cursor += 1;
        let mut commands = Vec::new();
        while cursor < words.len() && words[cursor] != 0xfe00 {
            if cursor + 1 >= words.len() {
                return err("animation queue has a partial command pair");
            }
            commands.push([words[cursor], words[cursor + 1]]);
            cursor += 2;
        }
        if cursor >= words.len() {
            return err("animation queue is missing FE00");
        }
        cursor += 1;
        queues.push(AnimationQueue {
            header: format!("0x{header:04x}"),
            commands,
        });
    }
    if cursor != words.len().saturating_sub(1) || words.get(cursor) != Some(&0xffff) {
        return err("animation queue stream is missing final FFFF");
    }
    Ok(queues)
}

pub fn encode_queues(queues: &[AnimationQueue]) -> Result<Vec<u8>> {
    let mut words = Vec::new();
    for queue in queues {
        let header = parse_number_text(&queue.header, "animation queue header")?;
        if header >> 8 != 0xfd {
            return err("animation queue header must be FDxx");
        }
        words.push(header as u16);
        words.extend(
            queue
                .commands
                .iter()
                .flat_map(|command| [command[0], command[1]]),
        );
        words.push(0xfe00);
    }
    words.push(0xffff);
    Ok(pack_u16(&words))
}

fn parse_number_text(value: &str, label: &str) -> Result<u64> {
    let value = value.trim();
    let parsed = if let Some(value) = value
        .strip_prefix("0x")
        .or_else(|| value.strip_prefix("0X"))
    {
        u64::from_str_radix(value, 16)
    } else {
        value.parse::<u64>()
    };
    parsed.map_err(|_| format!("{label} must be an integer"))
}

fn word_list_document(decoded: &[u8]) -> Result<Value> {
    Ok(
        json!({"format": 1, "word_size": 2, "words": u16s(decoded)?.iter().map(|word| format!("0x{word:04x}")).collect::<Vec<_>>() }),
    )
}

fn encode_word_list(document: &Value) -> Result<Vec<u8>> {
    let object = object(document, "component word source")?;
    let words = field(object, "words")?
        .as_array()
        .ok_or_else(|| "component word source must contain words".to_string())?;
    words
        .iter()
        .map(|value| word(value, "component word"))
        .collect::<Result<Vec<_>>>()
        .map(|words| pack_u16(&words))
}

pub fn export_queues(encoded: &[u8], source: &Path, plan_path: &Path) -> Result<(String, usize)> {
    let component = decode_component(encoded)?;
    let (document, report, count) = match decode_queues(&component.decoded) {
        Ok(queues) => {
            let commands = queues
                .iter()
                .map(|queue| queue.commands.len())
                .sum::<usize>();
            let queues_json: Vec<Value> = queues.iter().map(|queue| json!({"header": queue.header, "commands": queue.commands.iter().map(|command| json!([command[0], command[1]])).collect::<Vec<_>>() })).collect();
            (
                json!({"format": 1, "terminators": ["0xfe00", "0xffff"], "queues": queues_json}),
                json!({"component": "map-animation-queues", "queues": queues.len(), "commands": commands}),
                (queues.len().to_string(), commands),
            )
        }
        Err(_) => {
            let document = word_list_document(&component.decoded)?;
            let words = document
                .get("words")
                .and_then(Value::as_array)
                .map_or(0, Vec::len);
            (
                document,
                json!({"component": "map-animation-words", "words": words}),
                ("raw".to_string(), words),
            )
        }
    };
    canonical_file(source, &document)?;
    let plan = plan_for(&component, encoded, report)?;
    write(plan_path, &compact_json(&plan)?)?;
    if build_queues(source, plan_path)? != encoded {
        return err("animation queue component does not round-trip");
    }
    Ok(count)
}

pub fn build_queues(source: &Path, plan_path: &Path) -> Result<Vec<u8>> {
    let document = json_file(source)?;
    let source_object = object(&document, "animation queue source")?;
    if number(field(source_object, "format")?, "format")? != 1 {
        return err("unsupported animation queue source");
    }
    let decoded = if source_object.contains_key("queues") {
        let queues = field(source_object, "queues")?
            .as_array()
            .ok_or_else(|| "animation queues must be an array".to_string())?;
        let queues: Vec<AnimationQueue> = queues
            .iter()
            .enumerate()
            .map(|(index, value)| {
                let queue = object(value, &format!("queue {index}"))?;
                let header = field(queue, "header")?
                    .as_str()
                    .ok_or_else(|| "animation queue header must be text".to_string())?
                    .to_string();
                let commands = field(queue, "commands")?
                    .as_array()
                    .ok_or_else(|| "animation commands must be an array".to_string())?;
                let commands = commands
                    .iter()
                    .map(|command| {
                        let command = command.as_array().ok_or_else(|| {
                            "animation command must contain two u16 values".to_string()
                        })?;
                        if command.len() != 2 {
                            return err("animation command must contain two u16 values");
                        }
                        Ok([
                            word(&command[0], "animation command")?,
                            word(&command[1], "animation command")?,
                        ])
                    })
                    .collect::<Result<Vec<_>>>()?;
                Ok(AnimationQueue { header, commands })
            })
            .collect::<Result<Vec<_>>>()?;
        encode_queues(&queues)?
    } else {
        encode_word_list(&document)?
    };
    encode_plan(&decoded, &json_file(plan_path)?)
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum BlendCommand {
    Reset,
    Stop,
    Jump { target_pair: u8 },
    SetBlendControl { value: u16 },
    WriteBlendValue { value: u16, duration: u16 },
}

pub fn decode_blend_commands(decoded: &[u8]) -> Result<Vec<BlendCommand>> {
    let words = u16s(decoded)?;
    let mut commands = Vec::new();
    let mut cursor = 0;
    while cursor < words.len() {
        let value = words[cursor];
        cursor += 1;
        if value == 0xffff {
            commands.push(BlendCommand::Reset);
        } else if value >> 8 == 0xfe {
            if value == 0xfeff {
                commands.push(BlendCommand::Stop);
            } else {
                commands.push(BlendCommand::Jump {
                    target_pair: value as u8,
                });
            }
        } else if value & 0xf000 == 0x3000 {
            commands.push(BlendCommand::SetBlendControl { value });
        } else {
            if cursor >= words.len() {
                return err("blend value is missing its duration");
            }
            commands.push(BlendCommand::WriteBlendValue {
                value,
                duration: words[cursor],
            });
            cursor += 1;
        }
    }
    Ok(commands)
}

pub fn encode_blend_commands(commands: &[BlendCommand]) -> Result<Vec<u8>> {
    let mut words = Vec::new();
    for command in commands {
        match *command {
            BlendCommand::Reset => words.push(0xffff),
            BlendCommand::Stop => words.push(0xfeff),
            BlendCommand::Jump { target_pair } => {
                if target_pair == u8::MAX {
                    return err("blend jump target pair is outside 0..254");
                }
                words.push(0xfe00 | u16::from(target_pair));
            }
            BlendCommand::SetBlendControl { value } => {
                if value & 0xf000 != 0x3000 {
                    return err("blend-control value must be a 3xxx u16");
                }
                words.push(value);
            }
            BlendCommand::WriteBlendValue { value, duration } => {
                if value == 0xffff || value >> 8 == 0xfe || value & 0xf000 == 0x3000 {
                    return err("blend value collides with a control command");
                }
                words.extend([value, duration]);
            }
        }
    }
    Ok(pack_u16(&words))
}

fn blend_value(value: &Value, label: &str) -> Result<u16> {
    word(value, label)
}

fn parse_blend_commands(value: &Value) -> Result<Vec<BlendCommand>> {
    let values = value
        .as_array()
        .ok_or_else(|| "blend commands must be an array".to_string())?;
    values
        .iter()
        .map(|value| {
            let object = object(value, "blend command")?;
            let op = field(object, "op")?
                .as_str()
                .ok_or_else(|| "blend command op must be text".to_string())?;
            match op {
                "reset" => Ok(BlendCommand::Reset),
                "stop" => Ok(BlendCommand::Stop),
                "jump" => Ok(BlendCommand::Jump {
                    target_pair: byte(field(object, "target_pair")?, "blend jump target")?,
                }),
                "set_blend_control" => Ok(BlendCommand::SetBlendControl {
                    value: blend_value(field(object, "value")?, "blend control")?,
                }),
                "write_blend_value" => Ok(BlendCommand::WriteBlendValue {
                    value: blend_value(field(object, "value")?, "blend value")?,
                    duration: word(field(object, "duration")?, "blend duration")?,
                }),
                _ => err(format!("unsupported blend animation operation: {op}")),
            }
        })
        .collect()
}

fn blend_command_value(command: &BlendCommand) -> Value {
    match command {
        BlendCommand::Reset => json!({"op": "reset"}),
        BlendCommand::Stop => json!({"op": "stop"}),
        BlendCommand::Jump { target_pair } => json!({"op": "jump", "target_pair": target_pair}),
        BlendCommand::SetBlendControl { value } => {
            json!({"op": "set_blend_control", "value": format!("0x{value:04x}")})
        }
        BlendCommand::WriteBlendValue { value, duration } => {
            json!({"op": "write_blend_value", "value": format!("0x{value:04x}"), "duration": duration})
        }
    }
}

pub fn export_blend_animation(
    encoded: &[u8],
    source: &Path,
    plan_path: &Path,
) -> Result<(usize, String)> {
    let component = decode_component(encoded)?;
    let (document, report, count) = match decode_blend_commands(&component.decoded) {
        Ok(commands) => (
            json!({"format": 1, "word_size": 2, "commands": commands.iter().map(blend_command_value).collect::<Vec<_>>() }),
            json!({"component": "map-blend-animation", "commands": commands.len()}),
            commands.len(),
        ),
        Err(_) => {
            let document = word_list_document(&component.decoded)?;
            let words = document
                .get("words")
                .and_then(Value::as_array)
                .map_or(0, Vec::len);
            (
                document,
                json!({"component": "map-blend-words", "words": words}),
                words,
            )
        }
    };
    canonical_file(source, &document)?;
    let plan = plan_for(&component, encoded, report)?;
    let codec = component.codec.name().to_string();
    write(plan_path, &compact_json(&plan)?)?;
    if build_blend_animation(source, plan_path)? != encoded {
        return err("blend animation component does not round-trip");
    }
    Ok((count, codec))
}

pub fn build_blend_animation(source: &Path, plan_path: &Path) -> Result<Vec<u8>> {
    let document = json_file(source)?;
    let object = object(&document, "blend animation source")?;
    if number(field(object, "format")?, "format")? != 1
        || number(field(object, "word_size")?, "word size")? != 2
    {
        return err("unsupported blend animation source");
    }
    let decoded = if object.contains_key("commands") {
        pack_u16(&decode_blend_commands_from_json(field(
            object, "commands",
        )?)?)
    } else {
        encode_word_list(&document)?
    };
    encode_plan(&decoded, &json_file(plan_path)?)
}

fn decode_blend_commands_from_json(value: &Value) -> Result<Vec<u16>> {
    let commands = parse_blend_commands(value)?;
    let encoded = encode_blend_commands(&commands)?;
    u16s(&encoded)
}

pub fn export_sparse(encoded: &[u8], source: &Path) -> Result<usize> {
    let terminator = encoded
        .windows(3)
        .position(|window| window == [0xff, 0xff, 0xff])
        .ok_or_else(|| "invalid sparse-cell triple stream".to_string())?;
    if terminator % 3 != 0 || encoded[terminator + 3..].iter().any(|value| *value != 0) {
        return err("invalid sparse-cell triple stream");
    }
    let records: Vec<Value> = encoded[..terminator]
        .chunks_exact(3)
        .map(|record| json!(record.to_vec()))
        .collect();
    let document = json!({"format": 1, "record_size": 3, "terminator": "ffffff", "alignment_zeros": encoded.len() - terminator - 3, "records": records});
    canonical_file(source, &document)?;
    if build_sparse(source)? != encoded {
        return err("sparse-cell component does not round-trip");
    }
    Ok(terminator / 3)
}

pub fn build_sparse(source: &Path) -> Result<Vec<u8>> {
    let document = json_file(source)?;
    let object = object(&document, "sparse-cell source")?;
    if number(field(object, "format")?, "format")? != 1
        || number(field(object, "record_size")?, "record size")? != 3
        || field(object, "terminator")?.as_str() != Some("ffffff")
    {
        return err("unsupported sparse-cell source");
    }
    let records = field(object, "records")?
        .as_array()
        .ok_or_else(|| "sparse-cell records must contain three byte values".to_string())?;
    let mut output = Vec::new();
    for record in records {
        let record = record
            .as_array()
            .ok_or_else(|| "sparse-cell records must contain three byte values".to_string())?;
        if record.len() != 3 {
            return err("sparse-cell records must contain three byte values");
        }
        for value in record {
            output.push(byte(value, "sparse-cell value")?);
        }
    }
    output.extend([0xff, 0xff, 0xff]);
    let padding = number(field(object, "alignment_zeros")?, "sparse-cell alignment")?;
    if padding > 3 {
        return err("sparse-cell alignment is outside 0..3");
    }
    output.extend(std::iter::repeat_n(0, padding as usize));
    Ok(output)
}

pub fn self_test() -> Result<()> {
    let encoded = extract_resource::synthetic_general();
    let component = decode_component(&encoded)?;
    if component.decoded != b"ABAB" {
        return err("general decoder self-test failed");
    }
    let plan = plan_for(&component, &encoded, json!({"component": "test"}))?;
    let rebuilt = encode_plan(&component.decoded, &plan)?;
    if rebuilt != encoded {
        return err("general component self-test failed");
    }
    let entries = [0x1234, 0x5678, 0x9abc, 0xdef0];
    for mode in 0..=2 {
        let decoded = encode_metatiles(&entries, mode)?;
        let (actual_mode, actual_entries) = decode_metatiles(&decoded)?;
        if actual_mode != mode || actual_entries != entries {
            return err("metatile self-test failed");
        }
    }
    let sparse = vec![1, 2, 3, 0xff, 0xff, 0xff, 0];
    let directory =
        std::env::temp_dir().join(format!("alchemy-map-components-{}", std::process::id()));
    fs::create_dir_all(&directory).map_err(|error| error.to_string())?;
    let source = directory.join("sparse.json");
    let count = export_sparse(&sparse, &source)?;
    if count != 1 || build_sparse(&source)? != sparse {
        return err("sparse component self-test failed");
    }
    let _ = fs::remove_dir_all(directory);
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn synthetic_compression_round_trips() {
        self_test().expect("map container self-test");
    }

    #[test]
    fn header_round_trips() {
        let source =
            std::env::temp_dir().join(format!("alchemy-map-header-{}.json", std::process::id()));
        let mut header = vec![0u8; 0x3c];
        for (index, value) in header.iter_mut().enumerate() {
            *value = index as u8;
        }
        let offsets = export_header(&header, &source).expect("export header");
        assert_eq!(build_header(&source, None).unwrap(), header);
        assert_eq!(offsets.len(), 6);
        let _ = fs::remove_file(source);
    }

    #[test]
    fn all_component_builders_reject_invalid_sources() {
        let path =
            std::env::temp_dir().join(format!("alchemy-map-invalid-{}.json", std::process::id()));
        fs::write(&path, b"{}\n").unwrap();
        assert!(build_sparse(&path).is_err());
        assert!(build_header(&path, None).is_err());
        let _ = fs::remove_file(path);
    }
}
