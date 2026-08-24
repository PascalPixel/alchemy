pub mod cli;

use std::collections::{BTreeMap, BTreeSet};
use std::fs;

use serde_json::{Map, Value};

pub const ROM_BASE: i64 = 0x0800_0000;
pub const CHARACTER_DESCRIPTOR_COUNT: usize = 0x200;
pub const CHARACTER_DESCRIPTOR_SIZE: usize = 20;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);

impl std::fmt::Display for Error {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        formatter.write_str(&self.0)
    }
}

type Result<T> = std::result::Result<T, Error>;

fn fail<T>(message: impl Into<String>) -> Result<T> {
    Err(Error(message.into()))
}

fn integer_value(value: &Value, name: &str) -> Result<i64> {
    match value {
        Value::String(text) => parse_integer(text, name),
        Value::Number(number) => number
            .as_i64()
            .or_else(|| number.as_u64().and_then(|value| i64::try_from(value).ok()))
            .or_else(|| {
                number
                    .as_f64()
                    .filter(|value| {
                        value.is_finite()
                            && value.fract() == 0.0
                            && value.abs() <= 9_007_199_254_740_991.0
                    })
                    .map(|value| value as i64)
            })
            .ok_or_else(|| Error(format!("invalid {name}"))),
        _ => fail(format!("invalid {name}")),
    }
}

fn parse_integer(text: &str, name: &str) -> Result<i64> {
    let parsed = if let Some(digits) = text.strip_prefix("0x").or_else(|| text.strip_prefix("0X")) {
        u64::from_str_radix(digits, 16)
            .ok()
            .and_then(|value| i64::try_from(value).ok())
    } else {
        text.parse::<f64>().ok().and_then(|value| {
            if value.is_finite() && value.fract() == 0.0 && value.abs() <= 9_007_199_254_740_991.0 {
                Some(value as i64)
            } else {
                None
            }
        })
    };
    parsed.ok_or_else(|| Error(format!("invalid {name}")))
}

fn bounded(value: &Value, minimum: i64, maximum: i64, name: &str) -> Result<i64> {
    let value = integer_value(value, name)?;
    if !(minimum..=maximum).contains(&value) {
        return fail(format!("invalid {name}"));
    }
    Ok(value)
}

fn text<'a>(value: &'a Value, name: &str) -> Result<&'a str> {
    match value {
        Value::String(value) if !value.is_empty() => Ok(value),
        _ => fail(format!("invalid {name}")),
    }
}

fn object<'a>(value: &'a Value, name: &str) -> Result<&'a Map<String, Value>> {
    value
        .as_object()
        .ok_or_else(|| Error(format!("invalid {name}")))
}

fn field<'a>(value: &'a Value, name: &str) -> Result<&'a Value> {
    object(value, "object")?
        .get(name)
        .ok_or_else(|| Error(format!("missing {name}")))
}

fn array<'a>(value: &'a Value, name: &str) -> Result<&'a Vec<Value>> {
    value
        .as_array()
        .ok_or_else(|| Error(format!("invalid {name}")))
}

fn write_u16(data: &mut [u8], offset: usize, value: u16) {
    data[offset..offset + 2].copy_from_slice(&value.to_le_bytes());
}

fn write_u32(data: &mut [u8], offset: usize, value: u32) {
    data[offset..offset + 4].copy_from_slice(&value.to_le_bytes());
}

fn pair(value: &Value, minimum: i64, maximum: i64, name: &str) -> Result<[i64; 2]> {
    let values = array(value, name)?;
    if values.len() != 2 {
        return fail(format!("invalid {name}"));
    }
    Ok([
        bounded(&values[0], minimum, maximum, &format!("{name} first value"))?,
        bounded(
            &values[1],
            minimum,
            maximum,
            &format!("{name} second value"),
        )?,
    ])
}

fn encode_command(value: &Value) -> Result<(u8, u8)> {
    let values = array(value, "animation command")?;
    if values.len() < 2 {
        return fail("invalid animation command");
    }
    let operation = text(&values[0], "animation operation")?;
    if operation == "frame" {
        if values.len() != 3 {
            return fail("invalid frame command");
        }
        return Ok((
            bounded(&values[1], 0, 0xee, "frame number")? as u8,
            bounded(&values[2], 0, 0xff, "frame time")? as u8,
        ));
    }
    if values.len() != 2 {
        return fail("invalid control command");
    }
    let operand = bounded(&values[1], 0, 0xff, "control operand")? as u8;
    let opcode = match operation {
        "end" => 0xef,
        "set_draw" => 0xf0,
        "hold" => 0xf1,
        "jump" => 0xfd,
        "select" => 0xfe,
        "blank" => 0xff,
        _ => return fail(format!("unknown animation operation {operation}")),
    };
    Ok((opcode, operand))
}

type AnimationBuild = (Vec<u8>, BTreeMap<String, u32>, BTreeMap<String, usize>);

fn build_animations(index: &Value) -> Result<AnimationBuild> {
    let start = integer_value(field(index, "animation_address")?, "animation address")?;
    let groups = array(field(index, "animation_groups")?, "animation groups")?;
    let mut data = Vec::new();
    let mut symbols = BTreeMap::new();
    let mut table_counts = BTreeMap::new();
    let mut cursor = start;
    for group in groups {
        let group_name = text(field(group, "name")?, "animation symbol")?;
        let commands = array(field(group, "commands")?, "animation commands")?;
        for command_value in commands {
            let (opcode, operand) = encode_command(command_value)?;
            data.extend_from_slice(&[opcode, operand]);
        }
        let entry_commands = array(field(group, "entry_commands")?, "animation entry commands")?;
        let mut label_indexes = BTreeSet::new();
        let mut entry_addresses = Vec::with_capacity(entry_commands.len());
        for (entry_index, command_value) in entry_commands.iter().enumerate() {
            let command = bounded(
                command_value,
                0,
                commands.len() as i64 - 1,
                "animation entry command",
            )? as usize;
            if !label_indexes.insert(command) {
                return fail(format!("duplicate command label in {group_name}"));
            }
            let label_name = format!("{group_name}_{entry_index:03x}");
            let address = u32::try_from(cursor + (command * 2) as i64).expect("animation address");
            if symbols.insert(label_name.clone(), address).is_some() {
                return fail(format!("duplicate animation symbol {label_name}"));
            }
            entry_addresses.push(address);
        }
        cursor += i64::try_from(commands.len() * 2).expect("command size");
        if symbols
            .insert(
                group_name.to_string(),
                u32::try_from(cursor).expect("animation address"),
            )
            .is_some()
        {
            return fail(format!("duplicate animation symbol {group_name}"));
        }
        let table_entries = object(group, "animation group")?
            .get("table_entries")
            .map(|value| array(value, "animation table entries"))
            .transpose()?;
        let table_count = table_entries.map_or(entry_addresses.len(), Vec::len);
        table_counts.insert(group_name.to_string(), table_count);
        if let Some(table_entries) = table_entries {
            for entry in table_entries {
                let entry = bounded(
                    entry,
                    0,
                    entry_addresses.len() as i64 - 1,
                    "animation table entry",
                )? as usize;
                data.extend_from_slice(&entry_addresses[entry].to_le_bytes());
            }
        } else {
            for address in entry_addresses {
                data.extend_from_slice(&address.to_le_bytes());
            }
        }
        cursor += i64::try_from(table_count * 4).expect("table size");
        if let Some(extras) = object(group, "animation group")?.get("extra_tables") {
            for extra in array(extras, "extra animation tables")? {
                let extra = object(extra, "extra animation table")?;
                let name = text(
                    extra
                        .get("name")
                        .ok_or_else(|| Error("invalid extra animation table".into()))?,
                    "animation symbol",
                )?;
                if symbols
                    .insert(
                        name.to_string(),
                        u32::try_from(cursor).expect("animation address"),
                    )
                    .is_some()
                {
                    return fail(format!("duplicate animation symbol {name}"));
                }
                let count = bounded(
                    extra
                        .get("count")
                        .ok_or_else(|| Error("invalid extra animation table".into()))?,
                    1,
                    0xffff,
                    "extra animation table count",
                )? as usize;
                let address = symbols[name];
                for _ in 0..count {
                    data.extend_from_slice(&address.to_le_bytes());
                }
                cursor += i64::try_from(count * 4).expect("table size");
            }
        }
    }
    Ok((data, symbols, table_counts))
}

pub fn build_character_catalog(index: &Value) -> Result<Vec<u8>> {
    if integer_value(field(index, "format")?, "format")? != 2
        || text(field(index, "codec")?, "codec")? != "golden-sun-character-catalog"
    {
        return fail("unsupported character catalog format");
    }
    let address = integer_value(field(index, "address")?, "catalog address")?;
    let size = integer_value(field(index, "size")?, "catalog size")?;
    let descriptor_count = integer_value(field(index, "descriptor_count")?, "descriptor count")?;
    let descriptor_size = integer_value(field(index, "descriptor_size")?, "descriptor size")?;
    let animation_address = integer_value(field(index, "animation_address")?, "animation address")?;
    if descriptor_count != CHARACTER_DESCRIPTOR_COUNT as i64
        || descriptor_size != CHARACTER_DESCRIPTOR_SIZE as i64
        || animation_address != address + descriptor_count * descriptor_size
    {
        return fail("character descriptor layout differs");
    }
    let mut frames = BTreeMap::new();
    for item in array(field(index, "frame_directories")?, "frame directories")? {
        let item = array(item, "frame directory")?;
        if item.len() < 2 {
            return fail("invalid frame directory");
        }
        let name = text(&item[0], "frame directory name")?.to_string();
        if frames
            .insert(
                name.clone(),
                bounded(&item[1], ROM_BASE, 0xffff_ffff, "frame directory address")? as u32,
            )
            .is_some()
        {
            return fail(format!("duplicate frame directory {name}"));
        }
    }
    let (animation_data, symbols, table_counts) = build_animations(index)?;
    if animation_address + animation_data.len() as i64 != address + size {
        return fail("character animation size differs");
    }
    let mut descriptors = vec![0u8; CHARACTER_DESCRIPTOR_COUNT * CHARACTER_DESCRIPTOR_SIZE];
    let descriptor_object = object(field(index, "descriptors")?, "descriptors")?;
    let mut used = BTreeSet::new();
    for (key, item) in descriptor_object {
        if key.len() != 3
            || !key
                .bytes()
                .all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(&byte))
        {
            return fail(format!("invalid descriptor id {key}"));
        }
        let record_id = usize::from_str_radix(key, 16)
            .map_err(|_| Error(format!("invalid descriptor id {key}")))?;
        if record_id >= CHARACTER_DESCRIPTOR_COUNT || !used.insert(record_id) {
            return fail(format!("duplicate descriptor id {key}"));
        }
        let offset = record_id * CHARACTER_DESCRIPTOR_SIZE;
        let size_pair = pair(field(item, "size")?, 0, 0xff, "descriptor size")?;
        let adjust = pair(field(item, "adjust")?, -0x80, 0x7f, "descriptor adjustment")?;
        let anchor = pair(field(item, "anchor")?, -0x80, 0xff, "descriptor anchor")?;
        descriptors[offset] = size_pair[0] as u8;
        descriptors[offset + 1] = size_pair[1] as u8;
        write_u16(
            &mut descriptors,
            offset + 2,
            bounded(field(item, "scale")?, 0, 0xffff, "descriptor scale")? as u16,
        );
        descriptors[offset + 4] =
            bounded(field(item, "draw_kind")?, 0, 0xff, "descriptor draw kind")? as u8;
        let animation_count = bounded(
            field(item, "animation_count")?,
            0,
            0xff,
            "descriptor animation count",
        )? as u8;
        descriptors[offset + 5] = animation_count;
        descriptors[offset + 6] = adjust[0] as i8 as u8;
        descriptors[offset + 7] = adjust[1] as i8 as u8;
        descriptors[offset + 8] = anchor[0] as i8 as u8;
        descriptors[offset + 9] =
            bounded(&Value::from(anchor[1]), 0, 0xff, "descriptor anchor y")? as u8;
        let codec = bounded(field(item, "frame_codec")?, 0, 3, "descriptor frame codec")? as u8;
        if !matches!(codec, 0 | 1 | 3) {
            return fail("unsupported descriptor frame codec");
        }
        descriptors[offset + 10] = codec;
        if !field(item, "frames")?.is_null() {
            let frame_name = text(field(item, "frames")?, "descriptor frame directory")?;
            let frame_address = frames
                .get(frame_name)
                .copied()
                .ok_or_else(|| Error(format!("undefined frame directory {frame_name}")))?;
            write_u32(&mut descriptors, offset + 12, frame_address);
        }
        let animation_name = text(field(item, "animation")?, "descriptor animation table")?;
        let animation_directory = symbols
            .get(animation_name)
            .copied()
            .ok_or_else(|| Error(format!("undefined animation table {animation_name}")))?;
        let table_count = table_counts
            .get(animation_name)
            .copied()
            .ok_or_else(|| Error(format!("undefined animation table {animation_name}")))?;
        if usize::from(animation_count) > table_count {
            return fail(format!("descriptor {key} exceeds its animation table"));
        }
        write_u32(&mut descriptors, offset + 16, animation_directory);
    }
    descriptors.extend_from_slice(&animation_data);
    Ok(descriptors)
}

pub fn read_json(path: &str) -> Result<Value> {
    let bytes = fs::read(path).map_err(|error| Error(format!("{path}: {error}")))?;
    serde_json::from_str(&String::from_utf8_lossy(&bytes))
        .map_err(|error| Error(format!("{path}: {error}")))
}
