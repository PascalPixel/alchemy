pub mod cli;
use extract_resource::{
    encode_general, encode_palette, GeneralToken, PaletteGroup, PaletteOperation,
};
use serde_json::{Map, Value};
use std::collections::BTreeMap;
use std::fs;
use std::path::Path;
use tilemap::import_tilemap;
pub type Result<T> = std::result::Result<T, String>;
pub type OffsetChecks = BTreeMap<usize, usize>;
fn err<T>(message: impl Into<String>) -> Result<T> {
    Err(message.into())
}
fn read(path: &Path) -> Result<Vec<u8>> {
    fs::read(path).map_err(|error| format!("{}: {error}", path.display()))
}
fn json_file(path: &Path) -> Result<Value> {
    let text =
        String::from_utf8(read(path)?).map_err(|error| format!("{}: {error}", path.display()))?;
    serde_json::from_str(&text).map_err(|error| format!("{}: {error}", path.display()))
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
fn parse_hex(value: &str) -> Result<Vec<u8>> {
    if !value.len().is_multiple_of(2) {
        return err("hex data has an odd length");
    }
    (0..value.len())
        .step_by(2)
        .map(|i| {
            u8::from_str_radix(&value[i..i + 2], 16).map_err(|_| "invalid hex data".to_string())
        })
        .collect()
}
fn u16s(data: &[u8]) -> Result<Vec<u16>> {
    if !data.len().is_multiple_of(2) {
        return err("partial u16");
    }
    Ok(data
        .chunks_exact(2)
        .map(|c| u16::from_le_bytes([c[0], c[1]]))
        .collect())
}
fn pack_u16(words: &[u16]) -> Vec<u8> {
    words.iter().flat_map(|word| word.to_le_bytes()).collect()
}
fn general_tokens(value: &Value) -> Result<Vec<GeneralToken>> {
    value
        .as_array()
        .ok_or_else(|| "tokens must be an array".to_string())?
        .iter()
        .enumerate()
        .map(|(i, token)| {
            let token = token
                .as_array()
                .ok_or_else(|| format!("token {i} is invalid"))?;
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
                _ => err(format!("token {i} is invalid")),
            }
        })
        .collect()
}
fn palette_operations(value: &Value) -> Result<Vec<PaletteOperation>> {
    value
        .as_array()
        .ok_or_else(|| "palette operations must be an array".to_string())?
        .iter()
        .enumerate()
        .map(|(i, value)| {
            let operation = value
                .as_array()
                .ok_or_else(|| format!("palette operation {i} is invalid"))?;
            match operation.first().and_then(Value::as_str) {
                Some("l") if operation.len() == 1 => Ok(PaletteOperation::Literal),
                Some("e") if operation.len() == 1 => Ok(PaletteOperation::End),
                Some("c") if operation.len() == 3 => Ok(PaletteOperation::Copy {
                    length: number(&operation[1], "palette copy length")? as u32,
                    distance: number(&operation[2], "palette copy distance")? as u32,
                }),
                _ => err(format!("palette operation {i} is invalid")),
            }
        })
        .collect()
}
fn palette_groups(value: &Value) -> Result<Vec<PaletteGroup>> {
    value
        .as_array()
        .ok_or_else(|| "palette tokens must be an array".to_string())?
        .iter()
        .enumerate()
        .map(|(i, value)| {
            let group = value
                .as_array()
                .ok_or_else(|| format!("palette group {i} is invalid"))?;
            match (group.first().and_then(Value::as_str), group.len()) {
                (Some("z"), 1) => Ok(PaletteGroup::Zeros),
                (Some("g"), 2) => Ok(PaletteGroup::Group(palette_operations(&group[1])?)),
                _ => err(format!("palette group {i} is invalid")),
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
        for (slot, offset) in offsets.iter().enumerate().take(6) {
            let expected = checks.get(&slot).copied().unwrap_or(0);
            if *offset != expected {
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
pub fn encode_metatiles(entries: &[u16], mode: u8) -> Result<Vec<u8>> {
    if !matches!(mode, 0..=2) || entries.is_empty() || !entries.len().is_multiple_of(4) {
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
    let tokens = vec![
        GeneralToken::Literal(2),
        GeneralToken::Copy {
            length: 2,
            distance: 2,
        },
    ];
    let expected = encode_general(b"ABAB", &tokens).map_err(|error| error.0)?;
    let plan = serde_json::json!({
        "format": 1,
        "codec": "golden-sun-general-lz",
        "decoded_size": 4,
        "tokens": [["l", 2], ["c", 2, 2]],
        "lookahead": ""
    });
    if encode_plan(b"ABAB", &plan)? != expected {
        return err("general component self-test failed");
    }
    let blend = encode_blend_commands(&[
        BlendCommand::SetBlendControl { value: 0x3f40 },
        BlendCommand::WriteBlendValue {
            value: 0x0010,
            duration: 3,
        },
        BlendCommand::Stop,
    ])?;
    if blend != [0x40, 0x3f, 0x10, 0x00, 0x03, 0x00, 0xff, 0xfe] {
        return err("blend component self-test failed");
    }
    Ok(())
}
