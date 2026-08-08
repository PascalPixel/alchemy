//! Rust port of `tools/make/character_catalog.ts`.
//!
//! The catalog is a small structured byte region: a descriptor table followed
//! by animation command streams and pointer tables.  The JSON representation
//! is deliberately kept dynamic here because the tracked schema uses tuples
//! for commands, labels, and directory entries.

use std::collections::{BTreeMap, BTreeSet};
use std::fs;

use serde_json::{Map, Value};

pub const ROM_BASE: i64 = 0x0800_0000;
pub const CHARACTER_DESCRIPTOR_ADDRESS: i64 = 0x0818_5024;
pub const CHARACTER_DESCRIPTOR_COUNT: usize = 0x200;
pub const CHARACTER_DESCRIPTOR_SIZE: usize = 20;
pub const CHARACTER_ANIMATION_ADDRESS: i64 = 0x0818_7824;
pub const CHARACTER_ANIMATION_END: i64 = 0x0818_d554;

const EXTRA_POINTER_TABLES: [(i64, usize); 3] =
    [(0x0818_8acc, 4), (0x0818_97ec, 5), (0x0818_9800, 5)];

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);

impl std::fmt::Display for Error {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        formatter.write_str(&self.0)
    }
}

impl std::error::Error for Error {}

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

fn hex(value: i64, width: usize) -> String {
    format!("0x{value:0width$x}")
}

fn id(value: usize) -> String {
    format!("{value:03x}")
}

/// `JSON.stringify` and `Object.keys` enumerate canonical JavaScript array
/// index keys before the remaining string keys. Descriptor IDs such as `100`
/// are array-index keys even though they are written in hexadecimal; IDs such
/// as `01a` are ordinary strings and retain insertion order.
fn js_object(entries: Vec<(String, Value)>) -> Map<String, Value> {
    let mut indexed = Vec::new();
    let mut strings = Vec::new();
    for (key, value) in entries {
        let index = if key == "0" {
            Some(0)
        } else if !key.is_empty() && !key.starts_with('0') {
            key.parse::<u32>()
                .ok()
                .filter(|index| *index < u32::MAX && index.to_string() == key)
        } else {
            None
        };
        if let Some(index) = index {
            indexed.push((index, key, value));
        } else {
            strings.push((key, value));
        }
    }
    indexed.sort_by_key(|(index, _, _)| *index);
    let mut result = Map::new();
    for (_, key, value) in indexed
        .into_iter()
        .chain(strings.into_iter().map(|(key, value)| (0, key, value)))
    {
        result.insert(key, value);
    }
    result
}

fn signed_byte(value: u8) -> i64 {
    if value < 0x80 {
        i64::from(value)
    } else {
        i64::from(value) - 0x100
    }
}

fn read_u16(data: &[u8], offset: usize) -> Result<u16> {
    data.get(offset..offset + 2)
        .and_then(|bytes| bytes.try_into().ok())
        .map(u16::from_le_bytes)
        .ok_or_else(|| Error("character catalog is outside the ROM".into()))
}

fn read_u32(data: &[u8], offset: usize) -> Result<u32> {
    data.get(offset..offset + 4)
        .and_then(|bytes| bytes.try_into().ok())
        .map(u32::from_le_bytes)
        .ok_or_else(|| Error("character catalog is outside the ROM".into()))
}

fn write_u16(data: &mut [u8], offset: usize, value: u16) {
    data[offset..offset + 2].copy_from_slice(&value.to_le_bytes());
}

fn write_u32(data: &mut [u8], offset: usize, value: u32) {
    data[offset..offset + 4].copy_from_slice(&value.to_le_bytes());
}

#[derive(Debug, Clone)]
struct DescriptorRecord {
    id: usize,
    offset: usize,
    frame_directory: u32,
    animation_directory: u32,
    animation_count: u8,
}

#[derive(Debug, Clone)]
struct NamedDirectory {
    name: String,
    ids: Vec<usize>,
}

#[derive(Debug, Clone)]
struct DirectoryGroup {
    address: u32,
    count: usize,
    ids: Vec<usize>,
    name: String,
}

fn records(rom: &[u8]) -> Result<Vec<DescriptorRecord>> {
    let table = usize::try_from(CHARACTER_DESCRIPTOR_ADDRESS - ROM_BASE).expect("table offset");
    let end = table + CHARACTER_DESCRIPTOR_COUNT * CHARACTER_DESCRIPTOR_SIZE;
    if end > rom.len() {
        return fail("character descriptor table is outside the ROM");
    }
    let mut output = Vec::new();
    for record_id in 0..CHARACTER_DESCRIPTOR_COUNT {
        let offset = table + record_id * CHARACTER_DESCRIPTOR_SIZE;
        if rom[offset..offset + CHARACTER_DESCRIPTOR_SIZE]
            .iter()
            .all(|value| *value == 0)
        {
            continue;
        }
        output.push(DescriptorRecord {
            id: record_id,
            offset,
            frame_directory: read_u32(rom, offset + 12)?,
            animation_directory: read_u32(rom, offset + 16)?,
            animation_count: rom[offset + 5],
        });
    }
    Ok(output)
}

fn named_directories(records: &[DescriptorRecord], frame: bool) -> BTreeMap<u32, NamedDirectory> {
    let mut grouped: BTreeMap<u32, Vec<usize>> = BTreeMap::new();
    for record in records {
        let address = if frame {
            record.frame_directory
        } else {
            record.animation_directory
        };
        if address != 0 {
            grouped.entry(address).or_default().push(record.id);
        }
    }
    grouped
        .into_iter()
        .map(|(address, mut ids)| {
            ids.sort_unstable();
            let prefix = if frame { "chr" } else { "anm" };
            let name = format!("{prefix}_{}", id(ids[0]));
            (address, NamedDirectory { name, ids })
        })
        .collect()
}

fn animation_groups(
    records: &[DescriptorRecord],
    names: &BTreeMap<u32, NamedDirectory>,
) -> Result<Vec<DirectoryGroup>> {
    let mut grouped: BTreeMap<u32, DirectoryGroup> = BTreeMap::new();
    for record in records {
        if record.animation_directory == 0 {
            return fail(format!("descriptor {} lacks animation data", id(record.id)));
        }
        let named = names
            .get(&record.animation_directory)
            .ok_or_else(|| Error(format!("descriptor {} lacks animation data", id(record.id))))?;
        let group = grouped
            .entry(record.animation_directory)
            .or_insert_with(|| DirectoryGroup {
                address: record.animation_directory,
                count: 0,
                ids: named.ids.clone(),
                name: named.name.clone(),
            });
        group.count = group.count.max(usize::from(record.animation_count));
    }
    Ok(grouped.into_values().collect())
}

fn read_pointer_table(rom: &[u8], address: u32, count: usize) -> Result<Vec<u32>> {
    let offset = i64::from(address) - ROM_BASE;
    if offset < 0 {
        return fail("animation pointer table is outside the ROM");
    }
    let offset = usize::try_from(offset).expect("nonnegative offset");
    if offset
        .checked_add(count * 4)
        .is_none_or(|end| end > rom.len())
    {
        return fail("animation pointer table is outside the ROM");
    }
    (0..count)
        .map(|index| read_u32(rom, offset + index * 4))
        .collect()
}

fn command(operation: &str, first: i64, second: Option<i64>) -> Value {
    let mut values = vec![Value::String(operation.to_string()), Value::from(first)];
    if let Some(second) = second {
        values.push(Value::from(second));
    }
    Value::Array(values)
}

fn decode_command(opcode: u8, operand: u8) -> Result<Value> {
    if opcode < 0xef {
        return Ok(command(
            "frame",
            i64::from(opcode),
            Some(i64::from(operand)),
        ));
    }
    let operation = match opcode {
        0xef => "end",
        0xf0 => "set_draw",
        0xf1 => "hold",
        0xfd => "jump",
        0xfe => "select",
        0xff => "blank",
        _ => {
            return fail(format!(
                "unsupported animation opcode {}",
                hex(i64::from(opcode), 2)
            ))
        }
    };
    Ok(command(operation, i64::from(operand), None))
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

pub fn export_character_catalog(rom: &[u8]) -> Result<Value> {
    let source_records = records(rom)?;
    let frame_names = named_directories(&source_records, true);
    let animation_names = named_directories(&source_records, false);
    let groups = animation_groups(&source_records, &animation_names)?;
    if source_records.len() != 367 || groups.len() != 364 {
        return fail("character catalog structure differs");
    }

    let mut descriptor_entries = Vec::new();
    for item in &source_records {
        let offset = item.offset;
        let codec = rom[offset + 10];
        if !matches!(codec, 0 | 1 | 3) {
            return fail("unsupported character frame codec");
        }
        if rom[offset + 11] != 0 {
            return fail("character descriptor reserve byte is not zero");
        }
        let frame = if item.frame_directory == 0 {
            Value::Null
        } else {
            Value::String(
                frame_names
                    .get(&item.frame_directory)
                    .ok_or_else(|| Error("character frame directory is unnamed".into()))?
                    .name
                    .clone(),
            )
        };
        let animation = animation_names
            .get(&item.animation_directory)
            .ok_or_else(|| Error("character animation directory is unnamed".into()))?;
        let mut descriptor = Map::new();
        descriptor.insert(
            "size".into(),
            Value::Array(vec![Value::from(rom[offset]), Value::from(rom[offset + 1])]),
        );
        descriptor.insert("scale".into(), Value::from(read_u16(rom, offset + 2)?));
        descriptor.insert("draw_kind".into(), Value::from(rom[offset + 4]));
        descriptor.insert("animation_count".into(), Value::from(item.animation_count));
        descriptor.insert(
            "adjust".into(),
            Value::Array(vec![
                Value::from(signed_byte(rom[offset + 6])),
                Value::from(signed_byte(rom[offset + 7])),
            ]),
        );
        descriptor.insert(
            "anchor".into(),
            Value::Array(vec![
                Value::from(signed_byte(rom[offset + 8])),
                Value::from(rom[offset + 9]),
            ]),
        );
        descriptor.insert("frame_codec".into(), Value::from(codec));
        descriptor.insert("frames".into(), frame);
        descriptor.insert("animation".into(), Value::String(animation.name.clone()));
        descriptor_entries.push((id(item.id), Value::Object(descriptor)));
    }
    let descriptors = js_object(descriptor_entries);

    let frame_directories = frame_names
        .iter()
        .map(|(address, named)| {
            Value::Array(vec![
                Value::String(named.name.clone()),
                Value::String(hex(i64::from(*address), 8)),
                Value::Array(
                    named
                        .ids
                        .iter()
                        .map(|value| Value::String(id(*value)))
                        .collect(),
                ),
            ])
        })
        .collect();

    let mut output_groups = Vec::new();
    let mut cursor = CHARACTER_ANIMATION_ADDRESS;
    for group in groups {
        if cursor > i64::from(group.address) || (i64::from(group.address) - cursor) % 2 != 0 {
            return fail(format!(
                "invalid animation command extent before {}",
                group.name
            ));
        }
        let mut commands = Vec::new();
        let mut address = cursor;
        while address < i64::from(group.address) {
            let offset = usize::try_from(address - ROM_BASE).expect("animation offset");
            commands.push(decode_command(rom[offset], rom[offset + 1])?);
            address += 2;
        }
        let pointers = read_pointer_table(rom, group.address, group.count)?;
        if pointers.iter().any(|pointer| {
            let pointer = i64::from(*pointer);
            pointer < cursor || pointer >= i64::from(group.address) || (pointer - cursor) % 2 != 0
        }) {
            return fail(format!(
                "animation table {} points outside its commands",
                group.name
            ));
        }
        let unique: BTreeSet<u32> = pointers.iter().copied().collect();
        let first = unique.iter().next().copied();
        if first != Some(cursor as u32) {
            return fail(format!(
                "animation table {} leaves an unlabeled prefix",
                group.name
            ));
        }
        let mut labels = Vec::new();
        let mut label_by_address = BTreeMap::new();
        for (index, address) in unique.iter().enumerate() {
            let name = format!("{}_{index:03x}", group.name);
            label_by_address.insert(*address, name.clone());
            labels.push(Value::Array(vec![
                Value::String(name),
                Value::from((i64::from(*address) - cursor) / 2),
            ]));
        }
        let entries = pointers
            .iter()
            .map(|pointer| {
                Value::String(
                    label_by_address
                        .get(pointer)
                        .expect("validated pointer")
                        .clone(),
                )
            })
            .collect();
        let mut item = Map::new();
        item.insert("name".into(), Value::String(group.name.clone()));
        item.insert(
            "ids".into(),
            Value::Array(
                group
                    .ids
                    .iter()
                    .map(|value| Value::String(id(*value)))
                    .collect(),
            ),
        );
        item.insert("commands".into(), Value::Array(commands));
        item.insert("labels".into(), Value::Array(labels));
        item.insert("entries".into(), Value::Array(entries));
        cursor = i64::from(group.address) + i64::try_from(group.count * 4).expect("table size");
        let mut extras = Vec::new();
        for (extra_address, count) in EXTRA_POINTER_TABLES {
            if cursor != extra_address {
                continue;
            }
            let pointers = read_pointer_table(rom, cursor as u32, count)?;
            if pointers.iter().any(|pointer| i64::from(*pointer) != cursor) {
                return fail(format!(
                    "extra animation table anm_{cursor:08x} is not self-referential"
                ));
            }
            let name = format!("anm_{cursor:08x}");
            let mut extra = Map::new();
            extra.insert("name".into(), Value::String(name.clone()));
            extra.insert(
                "entries".into(),
                Value::Array(
                    pointers
                        .iter()
                        .map(|_| Value::String(name.clone()))
                        .collect(),
                ),
            );
            extras.push(Value::Object(extra));
            cursor += i64::try_from(count * 4).expect("table size");
        }
        if !extras.is_empty() {
            item.insert("extra_tables".into(), Value::Array(extras));
        }
        output_groups.push(Value::Object(item));
    }
    if cursor != CHARACTER_ANIMATION_END {
        return fail("character animation extent differs");
    }

    let mut result = Map::new();
    result.insert("format".into(), Value::from(1));
    result.insert(
        "codec".into(),
        Value::String("golden-sun-character-catalog".into()),
    );
    result.insert(
        "address".into(),
        Value::String(hex(CHARACTER_DESCRIPTOR_ADDRESS, 8)),
    );
    result.insert(
        "size".into(),
        Value::String(hex(
            CHARACTER_ANIMATION_END - CHARACTER_DESCRIPTOR_ADDRESS,
            8,
        )),
    );
    result.insert(
        "descriptor_count".into(),
        Value::from(CHARACTER_DESCRIPTOR_COUNT),
    );
    result.insert(
        "descriptor_size".into(),
        Value::from(CHARACTER_DESCRIPTOR_SIZE),
    );
    result.insert(
        "animation_address".into(),
        Value::String(hex(CHARACTER_ANIMATION_ADDRESS, 8)),
    );
    result.insert("frame_directories".into(), Value::Array(frame_directories));
    result.insert("descriptors".into(), Value::Object(descriptors));
    result.insert("animation_groups".into(), Value::Array(output_groups));
    Ok(Value::Object(result))
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

fn build_animations(
    index: &Value,
) -> Result<(Vec<u8>, BTreeMap<String, u32>, BTreeMap<String, usize>)> {
    let start = integer_value(field(index, "animation_address")?, "animation address")?;
    let groups = array(field(index, "animation_groups")?, "animation groups")?;
    let mut data = Vec::new();
    let mut symbols = BTreeMap::new();
    let mut table_counts = BTreeMap::new();
    let mut cursor = start;
    for group in groups {
        let commands = array(field(group, "commands")?, "animation commands")?;
        for command_value in commands {
            let (opcode, operand) = encode_command(command_value)?;
            data.extend_from_slice(&[opcode, operand]);
        }
        let labels = array(field(group, "labels")?, "animation labels")?;
        let mut label_indexes = BTreeSet::new();
        let group_name = text(field(group, "name")?, "animation symbol")?;
        for label in labels {
            let label = array(label, "animation label")?;
            if label.len() < 2 {
                return fail("invalid animation label");
            }
            let label_name = text(&label[0], "animation symbol")?;
            let command = bounded(
                &label[1],
                0,
                commands.len() as i64 - 1,
                "animation label command",
            )? as usize;
            if !label_indexes.insert(command) {
                return fail(format!("duplicate command label in {group_name}"));
            }
            if symbols
                .insert(
                    label_name.to_string(),
                    u32::try_from(cursor + (command * 2) as i64).expect("animation address"),
                )
                .is_some()
            {
                return fail(format!("duplicate animation symbol {label_name}"));
            }
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
        let entries = array(field(group, "entries")?, "animation entries")?;
        table_counts.insert(group_name.to_string(), entries.len());
        for entry in entries {
            let entry = text(entry, "animation entry")?;
            let address = symbols
                .get(entry)
                .copied()
                .ok_or_else(|| Error(format!("undefined animation entry {entry}")))?;
            data.extend_from_slice(&address.to_le_bytes());
        }
        cursor += i64::try_from(entries.len() * 4).expect("table size");
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
                let entries = array(
                    extra
                        .get("entries")
                        .ok_or_else(|| Error("invalid extra animation table".into()))?,
                    "extra animation entries",
                )?;
                for entry in entries {
                    let entry = text(entry, "extra animation entry")?;
                    let address = symbols
                        .get(entry)
                        .copied()
                        .ok_or_else(|| Error(format!("undefined extra animation entry {entry}")))?;
                    data.extend_from_slice(&address.to_le_bytes());
                }
                cursor += i64::try_from(entries.len() * 4).expect("table size");
            }
        }
    }
    Ok((data, symbols, table_counts))
}

pub fn build_character_catalog(index: &Value) -> Result<Vec<u8>> {
    if integer_value(field(index, "format")?, "format")? != 1
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

pub fn verify_character_catalog(rom: &[u8], index: &Value) -> Result<()> {
    let built = build_character_catalog(index)?;
    let address = integer_value(field(index, "address")?, "catalog address")? - ROM_BASE;
    if address < 0
        || usize::try_from(address)
            .ok()
            .and_then(|offset| offset.checked_add(built.len()))
            .is_none_or(|end| end > rom.len())
    {
        return fail("character catalog is outside the ROM");
    }
    let offset = address as usize;
    if rom[offset..offset + built.len()] != built {
        return fail("character catalog differs from the ROM");
    }
    Ok(())
}

pub fn catalog_size(index: &Value) -> Result<i64> {
    integer_value(field(index, "size")?, "catalog size")
}

pub fn descriptor_count(index: &Value) -> Result<usize> {
    Ok(object(field(index, "descriptors")?, "descriptors")?.len())
}

pub fn canonical_catalog(index: &Value) -> String {
    canonical_json::canonical_json(index)
}

pub fn self_test() -> Result<()> {
    let commands = [
        command("frame", 0xee, Some(0x55)),
        command("end", 1, None),
        command("set_draw", 2, None),
        command("hold", 3, None),
        command("jump", 4, None),
        command("select", 5, None),
        command("blank", 6, None),
    ];
    for expected in commands {
        let (opcode, operand) = encode_command(&expected)?;
        if decode_command(opcode, operand)? != expected {
            return fail("character command self-test failed");
        }
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn command_round_trip() {
        self_test().expect("command self-test");
    }

    #[test]
    fn canonical_catalog_uses_tuple_arrays() {
        let value = Value::Array(vec![
            Value::String("frame".into()),
            Value::from(1),
            Value::from(2),
        ]);
        assert_eq!(canonical_catalog(&value), "[\"frame\", 1, 2]");
    }
}
