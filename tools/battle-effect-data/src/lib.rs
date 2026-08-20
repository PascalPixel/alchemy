use extract_resource::{encode_palette, PaletteGroup, PaletteOperation};
use import_asset::gba_graphics;
use serde_json::Value;
use std::io::Write;
use std::path::{Path, PathBuf};
pub const BATTLE_DATA_ADDRESS: u32 = 0x0809_c410;
pub const BATTLE_DATA_END: u32 = 0x080a_1000;
const DIRECT_GRAPHICS_END: u32 = 0x0809_c610;
const WEIGHTED_RECORDS_END: u32 = 0x0809_d170;
const RULE_TABLES_END: u32 = 0x0809_e4ce;
const HALFWORD_GRAPHIC_END: u32 = 0x0809_e680;
const SINE_TABLE_ADDRESS: u32 = 0x0809_ed84;
const SINE_TABLE_END: u32 = 0x0809_ef84;
const GRADIENT_ADDRESS: u32 = 0x0809_f840;
const GRADIENT_END: u32 = 0x0809_fbc0;
const SPARSE_TABLE_ADDRESS: u32 = 0x080a_0138;
const SPARSE_TABLE_END: u32 = 0x080a_0288;
pub type Res<T> = Result<T, String>;
fn err<T>(message: &str) -> Res<T> {
    Err(message.to_string())
}
type TableRow = (u32, u32, &'static [&'static str], &'static [&'static str]);
const RULE_LAYOUT: &[TableRow] = &[
    (
        0x0809_d170,
        0x0809_d7a8,
        &[
            "selector_a:s16",
            "selector_b:s16",
            "condition:s16",
            "result:s16",
        ],
        &["Func_0808ace0"],
    ),
    (
        0x0809_d7a8,
        0x0809_d8b0,
        &[
            "selector_a:s16",
            "selector_b:s16",
            "condition:s16",
            "result:s16",
        ],
        &["Func_0808adf0"],
    ),
    (0x0809_d8b0, 0x0809_d9f0, &["value:u16", "phase:u16"], &[]),
    (
        0x0809_d9f0,
        0x0809_ddd8,
        &[
            "selector_a:s16",
            "selector_b:s16",
            "condition:s16",
            "result:s16",
        ],
        &["Func_0808b090"],
    ),
    (
        0x0809_ddd8,
        0x0809_e1d8,
        &["selector_a:s16", "selector_b:s16", "result:u32"],
        &["Func_0808b158"],
    ),
    (
        0x0809_e1d8,
        0x0809_e270,
        &[
            "selector_a:s16",
            "selector_b:s16",
            "value_a:u16",
            "value_b:u16",
        ],
        &["Func_0808b1d8"],
    ),
    (0x0809_e270, 0x0809_e488, &["value:u32"], &["Func_0808b25c"]),
    (0x0809_e488, 0x0809_e4ce, &["value:u16"], &["Func_0808b320"]),
];
const TAIL_TABLE_LAYOUT: &[TableRow] = &[
    (0x0809_e680, 0x0809_e686, &["phase:u8"], &["Func_0808d9a4"]),
    (0x0809_e6b8, 0x0809_e6c0, &["phase:u8"], &["Func_0808e0b0"]),
    (
        0x0809_e8ac,
        0x0809_e8ce,
        &["intensity:u8"],
        &["Func_0808f52c"],
    ),
    (0x0809_e8ce, 0x0809_e8ee, &["curve:u8"], &["Func_0808f52c"]),
    (0x0809_e8ee, 0x0809_e92e, &["order:u8"], &["Func_08090658"]),
    (0x0809_e92e, 0x0809_e96e, &["color:u16"], &["Func_08090a5c"]),
    (0x0809_e96e, 0x0809_e9ae, &["color:u16"], &["Func_08090a5c"]),
    (0x0809_e9ae, 0x0809_e9ee, &["color:u16"], &["Func_08090a5c"]),
    (
        0x0809_e9f0,
        0x0809_ebfc,
        &["id:u16", "value:s16"],
        &["Func_08091560"],
    ),
    (0x0809_ed80, 0x0809_ed84, &["phase:u16"], &["Func_08092980"]),
    (
        0x0809_ef84,
        0x0809_f024,
        &[
            "x:s16",
            "y:s16",
            "tile:u16",
            "attribute_a:u16",
            "attribute_b:u16",
        ],
        &["Func_08094820"],
    ),
    (
        0x0809_f024,
        0x0809_f0b0,
        &["y_offset:s16", "tile_offset:u16"],
        &["Func_08094e7c"],
    ),
    (0x0809_f0f8, 0x0809_f118, &["phase:u16"], &["Func_08097b54"]),
    (0x0809_f118, 0x0809_f11c, &["order:u8"], &["Func_08097c3c"]),
    (0x0809_f128, 0x0809_f12c, &["order:u8"], &[]),
    (0x0809_f160, 0x0809_f168, &["slot:s8"], &["Func_0809ad70"]),
    (0x0809_f168, 0x0809_f188, &["curve:u8"], &["Func_0809bcf8"]),
    (
        0x0809_f188,
        0x0809_f1a8,
        &["offset:s16"],
        &["Func_0809bcf8"],
    ),
    (
        0x0809_f1a8,
        0x0809_f810,
        &[
            "resource_id:s16",
            "group:s8",
            "variant:s8",
            "effect_id:u16",
            "reserved:u16",
        ],
        &["Func_0808a8e4", "Func_0808ab48", "Func_0808ab74"],
    ),
    (
        0x0809_f810,
        0x0809_f840,
        &[
            "word_0:u32",
            "script:ptr",
            "word_2:u32",
            "word_3:u32",
            "word_4:u32",
            "word_5:u32",
        ],
        &["FunctionHead_0808b674"],
    ),
    (0x0809_fbc0, 0x0809_fbd0, &["packed_config:u32"], &[]),
    (0x0809_fc28, 0x0809_fc2c, &["value:u8"], &["Func_08093304"]),
    (
        0x080a_0108,
        0x080a_0128,
        &["palette_code:u32"],
        &["Func_08097384"],
    ),
];
#[rustfmt::skip]
const OPCODE_LAYOUT: &[(u32, &str)] = &[(1,"Func_0800d654"),(0,"Func_0800d674"),(3,"Func_0800d9f0"),(3,"Func_0800da18"),(3,"Func_0800da40"),(0,"Func_0800da78"),(0,"Func_0800daa0"),(2,"Func_0800dac0"),(3,"Func_0800dd70"),(3,"Func_0800df04"),(0,"Func_0800dcdc"),(0,"Func_0800daf0"),(2,"Func_0800d710"),(1,"Func_0800d760"),(1,"Func_0800d780"),(1,"Func_0800d7b4"),(0,"Func_0800d7e8"),(1,"Func_0800d7f8"),(1,"Func_0800d820"),(1,"Func_0800d850"),(1,"Func_0800d880"),(2,"Func_0800e9a0"),(2,"Func_0800e9dc"),(2,"Func_0800ea18"),(0,"FunctionHead_0800ebec"),(0,"Func_0800f7f4"),(0,"Func_0800f2f8"),(0,"Func_0800d8e8"),(1,"Func_0800d8c4"),(1,"Func_0800d8f4"),(1,"Func_0800d900"),(0,"Func_0800ca2c"),(0,"Func_0800ca44"),(0,"Func_0800ca58"),(1,"Func_0800d6a4"),(0,"Func_0800f7dc"),(0,"Func_0800d7e8"),(0,"Func_0800d7e8"),(0,"Func_0800d7e8"),(0,"Func_0800d7e8")];
#[rustfmt::skip]
const SCRIPT_LAYOUT: &[(u32,u32,&[u32])] = &[(0x0809_e6c0,0x0809_e75c,&[0x0809_e6c0]),(0x0809_e75c,0x0809_e87c,&[0x0809_e75c]),(0x0809_e87c,0x0809_e8a0,&[0x0809_e87c]),(0x0809_e8a0,0x0809_e8ac,&[0x0809_e8a0]),(0x0809_ebfc,0x0809_ed80,&[0x0809_ebfc,0x0809_ec7c,0x0809_ecfc]),(0x0809_f0b0,0x0809_f0b4,&[0x0809_f0b0]),(0x0809_f0b4,0x0809_f0bc,&[0x0809_f0b4]),(0x0809_f0bc,0x0809_f0f8,&[0x0809_f0bc,0x0809_f0d4]),(0x0809_f11c,0x0809_f128,&[0x0809_f11c]),(0x0809_f12c,0x0809_f13c,&[0x0809_f12c]),(0x0809_f13c,0x0809_f148,&[0x0809_f13c]),(0x0809_f148,0x0809_f154,&[0x0809_f148]),(0x0809_f154,0x0809_f160,&[0x0809_f154]),(0x0809_f80c,0x0809_f810,&[0x0809_f80c]),(0x0809_fbcc,0x0809_fbec,&[0x0809_fbcc]),(0x0809_fbec,0x0809_fc04,&[0x0809_fbec]),(0x0809_fc04,0x0809_fc1c,&[0x0809_fc04]),(0x0809_fc1c,0x0809_fc28,&[0x0809_fc1c]),(0x0809_fc2c,0x0809_fd38,&[0x0809_fc2c]),(0x0809_fd38,0x0809_fd44,&[0x0809_fd38]),(0x0809_fd44,0x0809_fe00,&[0x0809_fd44]),(0x0809_fe00,0x0809_fe04,&[0x0809_fe00]),(0x0809_fe04,0x0809_fe10,&[0x0809_fe04]),(0x0809_fe10,0x0809_fecc,&[0x0809_fe10]),(0x0809_fecc,0x0809_ff18,&[0x0809_fecc]),(0x0809_ff18,0x0809_ff2c,&[0x0809_ff18]),(0x0809_ff2c,0x0809_ff40,&[0x0809_ff2c]),(0x0809_ff40,0x0809_ff58,&[0x0809_ff40]),(0x080a_0128,0x080a_012c,&[0x080a_0128])];
#[rustfmt::skip]
const CALLBACK_SYMBOLS: &[(u32,&str)] = &[(0x0808_f305,"Func_0808f304"),(0x0809_2a75,"Func_08092a74"),(0x0809_3965,"Func_08093964"),(0x0809_397d,"Func_0809397c"),(0x0809_3a15,"Func_08093a14"),(0x0809_8c09,"Func_08098c08")];
fn hex(value: u32) -> String {
    format!("0x{value:08x}")
}
fn read_u16(data: &[u8], offset: usize) -> u32 {
    u32::from(u16::from_le_bytes([data[offset], data[offset + 1]]))
}
fn write_u16(output: &mut [u8], offset: usize, value: u16) {
    output[offset..offset + 2].copy_from_slice(&value.to_le_bytes());
}
fn write_u32(output: &mut [u8], offset: usize, value: u32) {
    output[offset..offset + 4].copy_from_slice(&value.to_le_bytes());
}
fn is_integer(value: f64) -> bool {
    value.is_finite() && value.fract() == 0.0
}
fn integer(value: Option<&Value>, minimum: f64, maximum: f64, label: &str) -> Res<f64> {
    let number = match value {
        Some(value) => value.as_f64().unwrap_or(f64::NAN),
        _ => f64::NAN,
    };
    let integral = value.is_some_and(|value| value.as_f64().is_some()) && is_integer(number);
    if !integral || number < minimum || number > maximum {
        return Err(format!("{label} is out of range"));
    }
    Ok(number)
}
fn array(value: Option<&Value>) -> Option<&Vec<Value>> {
    match value {
        Some(Value::Array(items)) => Some(items),
        _ => None,
    }
}
fn text_is(value: Option<&Value>, expected: &str) -> bool {
    matches!(value.and_then(Value::as_str), Some(text) if text == expected)
}
fn number_is(value: Option<&Value>, expected: f64) -> bool {
    matches!(value.and_then(Value::as_f64), Some(number) if number == expected)
}
fn graphic_path(root: &Path, source: &str) -> Res<PathBuf> {
    const PREFIX: &str = "graphics/battle/effects/";
    let accepted = match source.strip_prefix(PREFIX) {
        Some(rest) => {
            rest.len() > 4
                && rest.ends_with(".png")
                && rest
                    .chars()
                    .all(|ch| ch.is_ascii_lowercase() || ch.is_ascii_digit() || "_.-".contains(ch))
        }
        None => false,
    };
    if !accepted {
        return err("battle-effect graphic path differs");
    }
    Ok(root.join(format!(
        "graphics/battle_effects_{}",
        &source[PREFIX.len()..]
    )))
}
fn read_file(path: &Path) -> Res<Vec<u8>> {
    std::fs::read(path).map_err(|error| format!("ENOENT: {}: {error}", path.display()))
}
fn build_direct_graphics(source: &Value, root: &Path) -> Res<Vec<u8>> {
    let expected = [
        (BATTLE_DATA_ADDRESS, BATTLE_DATA_ADDRESS + 0x100),
        (BATTLE_DATA_ADDRESS + 0x100, DIRECT_GRAPHICS_END),
    ];
    let items = array(source.get("direct_graphics")).ok_or("direct-graphics collection differs")?;
    let mut output: Vec<u8> = Vec::new();
    for (index, item) in items.iter().enumerate() {
        let (start, end) = expected[index];
        if !text_is(item.get("address"), &hex(start))
            || !text_is(item.get("end"), &hex(end))
            || !number_is(item.get("bpp"), 4.0)
            || !number_is(item.get("columns"), 4.0)
            || !text_is(item.get("role"), "obj_tiles")
        {
            return err("direct-graphic layout differs");
        }
        let name = item.get("source").and_then(Value::as_str).unwrap_or("");
        let path = graphic_path(root, name)?;
        let (tiles, _, report) = gba_graphics(&read_file(&path)?, 4.0).map_err(|error| error.0)?;
        if report.get("width") != Some(32.0)
            || report.get("height") != Some(16.0)
            || tiles.len() as u32 != end - start
        {
            return err("direct-graphic dimensions differ");
        }
        output.extend_from_slice(&tiles);
    }
    Ok(output)
}
fn graphic_tiles(graphic: &Value, root: &Path) -> Res<Vec<u8>> {
    if !number_is(graphic.get("bpp"), 4.0) {
        return err("compressed graphic depth differs");
    }
    let name = graphic.get("source").and_then(Value::as_str).unwrap_or("");
    let path = graphic_path(root, name)?;
    let (tiles, _, report) = gba_graphics(&read_file(&path)?, 4.0).map_err(|error| error.0)?;
    let columns = graphic
        .get("columns")
        .and_then(Value::as_f64)
        .unwrap_or(f64::NAN);
    let decoded = graphic
        .get("decoded_bytes")
        .and_then(Value::as_f64)
        .unwrap_or(f64::NAN);
    if report.get("width") != Some(columns * 8.0) || tiles.len() as f64 != decoded {
        return err("compressed graphic dimensions differ");
    }
    Ok(tiles)
}
fn build_weighted_records(source: &Value) -> Res<Vec<u8>> {
    let table = source
        .get("weighted_records")
        .ok_or("weighted-record layout differs")?;
    let records = array(table.get("records"));
    let fields = Value::Array(
        [
            "base_result:u16",
            "rank_origin:u16",
            "results:u16[8]",
            "weights:u8[8]",
        ]
        .iter()
        .map(|text| Value::String((*text).to_string()))
        .collect(),
    );
    let consumers = Value::Array(
        ["Func_0808ae74", "Func_0808b05c"]
            .iter()
            .map(|text| Value::String((*text).to_string()))
            .collect(),
    );
    if !text_is(table.get("address"), &hex(DIRECT_GRAPHICS_END))
        || !text_is(table.get("end"), &hex(WEIGHTED_RECORDS_END))
        || !number_is(table.get("stride"), 28.0)
        || table.get("fields") != Some(&fields)
        || table.get("consumers") != Some(&consumers)
        || records.is_none_or(|items| items.len() != 104)
    {
        return err("weighted-record layout differs");
    }
    let records = records.unwrap();
    let mut output = vec![0u8; records.len() * 28];
    for (index, record) in records.iter().enumerate() {
        let offset = index * 28;
        write_u16(
            &mut output,
            offset,
            integer(record.get("base_result"), 0.0, 65535.0, "base result")? as u16,
        );
        write_u16(
            &mut output,
            offset + 2,
            integer(record.get("rank_origin"), 0.0, 65535.0, "rank origin")? as u16,
        );
        let results = array(record.get("results"));
        let weights = array(record.get("weights"));
        if results.is_none_or(|items| items.len() != 8)
            || weights.is_none_or(|items| items.len() != 8)
        {
            return err("weighted-record array extent differs");
        }
        for (slot, value) in results.unwrap().iter().enumerate() {
            let value = integer(Some(value), 0.0, 65535.0, "weighted result")?;
            write_u16(&mut output, offset + 4 + slot * 2, value as u16);
        }
        for (slot, value) in weights.unwrap().iter().enumerate() {
            let value = integer(Some(value), 0.0, 255.0, "result weight")?;
            output[offset + 20 + slot] = value as u8;
        }
    }
    Ok(output)
}
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum FieldKind {
    S8,
    U8,
    S16,
    U16,
    S32,
    U32,
    Ptr,
}
fn field_kind(field: &str) -> Res<FieldKind> {
    let (name, kind) = match field.split_once(':') {
        Some(parts) => parts,
        None => return err("typed-table field differs"),
    };
    let mut characters = name.chars();
    let named = match characters.next() {
        Some(first) if first.is_ascii_lowercase() => {
            characters.all(|ch| ch.is_ascii_lowercase() || ch.is_ascii_digit() || ch == '_')
        }
        _ => false,
    };
    if !named {
        return err("typed-table field differs");
    }
    match kind {
        "s8" => Ok(FieldKind::S8),
        "u8" => Ok(FieldKind::U8),
        "s16" => Ok(FieldKind::S16),
        "u16" => Ok(FieldKind::U16),
        "s32" => Ok(FieldKind::S32),
        "u32" => Ok(FieldKind::U32),
        "ptr" => Ok(FieldKind::Ptr),
        _ => err("typed-table field differs"),
    }
}
fn field_size(kind: FieldKind) -> usize {
    match kind {
        FieldKind::S8 | FieldKind::U8 => 1,
        FieldKind::S16 | FieldKind::U16 => 2,
        FieldKind::S32 | FieldKind::U32 | FieldKind::Ptr => 4,
    }
}
fn write_field(
    output: &mut [u8],
    offset: usize,
    kind: FieldKind,
    value: Option<&Value>,
) -> Res<()> {
    match kind {
        FieldKind::S8 => {
            let number = integer(value, -128.0, 127.0, "signed byte table value")?;
            output[offset] = number as i64 as u8;
        }
        FieldKind::U8 => {
            output[offset] = integer(value, 0.0, 255.0, "byte table value")? as u8;
        }
        FieldKind::S16 => {
            let number = integer(value, -32768.0, 32767.0, "signed table value")?;
            write_u16(output, offset, number as i64 as u16);
        }
        FieldKind::U16 => {
            write_u16(
                output,
                offset,
                integer(value, 0.0, 65535.0, "table value")? as u16,
            );
        }
        FieldKind::S32 => {
            let number = integer(
                value,
                -2147483648.0,
                2147483647.0,
                "signed word table value",
            )?;
            write_u32(output, offset, number as i64 as u32);
        }
        FieldKind::U32 => {
            let number = integer(value, 0.0, 4294967295.0, "word table value")?;
            write_u32(output, offset, number as u32);
        }
        FieldKind::Ptr => match value {
            Some(Value::Null) => write_u32(output, offset, 0),
            Some(Value::String(name)) => match script_symbol(name) {
                Some(address) => write_u32(output, offset, address),
                None => return err("symbolic table pointer differs"),
            },
            _ => return err("symbolic table pointer differs"),
        },
    }
    Ok(())
}
fn string_list(items: &[&str]) -> Value {
    Value::Array(
        items
            .iter()
            .map(|text| Value::String((*text).to_string()))
            .collect(),
    )
}
fn build_table_collection(
    tables: &[Value],
    layout: &[TableRow],
    label: &str,
) -> Res<Vec<(u32, u32, Vec<u8>)>> {
    let mut built = Vec::with_capacity(tables.len());
    for (table_index, table) in tables.iter().enumerate() {
        let (start, end, fields, consumers) = layout[table_index];
        if !text_is(table.get("address"), &hex(start))
            || !text_is(table.get("end"), &hex(end))
            || table.get("fields") != Some(&string_list(fields))
            || table.get("consumers") != Some(&string_list(consumers))
        {
            return Err(format!("{label} layout differs"));
        }
        let declared = array(table.get("fields")).ok_or(format!("{label} layout differs"))?;
        let mut kinds = Vec::with_capacity(declared.len());
        for entry in declared {
            kinds.push(field_kind(entry.as_str().unwrap_or(""))?);
        }
        let stride: usize = kinds.iter().copied().map(field_size).sum();
        let records = array(table.get("records"));
        if !number_is(table.get("stride"), stride as f64)
            || records.is_none_or(|items| items.len() * stride != (end - start) as usize)
        {
            return Err(format!("{label} extent differs"));
        }
        let mut output = vec![0u8; (end - start) as usize];
        for (record_index, record) in records.unwrap().iter().enumerate() {
            let values = array(Some(record));
            if values.is_none_or(|items| items.len() != kinds.len()) {
                return Err(format!("{label} record extent differs"));
            }
            let mut offset = record_index * stride;
            for (field_index, value) in values.unwrap().iter().enumerate() {
                let kind = kinds[field_index];
                write_field(&mut output, offset, kind, Some(value))?;
                offset += field_size(kind);
            }
        }
        built.push((start, end, output));
    }
    Ok(built)
}
fn build_typed_tables(source: &Value) -> Res<Vec<u8>> {
    let tables = array(source.get("typed_tables")).ok_or("typed-table collection differs")?;
    let built = build_table_collection(tables, RULE_LAYOUT, "typed-table")?;
    Ok(built.into_iter().flat_map(|(_, _, bytes)| bytes).collect())
}
fn build_tail_tables(source: &Value) -> Res<Vec<(u32, u32, Vec<u8>)>> {
    let tables = array(source.get("tail_tables")).ok_or("tail-table collection differs")?;
    build_table_collection(tables, TAIL_TABLE_LAYOUT, "tail-table")
}
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum HalfwordToken {
    Literal(u32),
    Copy { length: u32, distance: u32 },
    End,
}
fn append_halfword_copy(output: &mut Vec<u32>, distance: u32, length: u32) -> Res<()> {
    if distance < 1 || distance as usize > output.len() || !(2..=33).contains(&length) {
        return err("halfword copy differs");
    }
    for _ in 0..length {
        output.push(output[output.len() - distance as usize]);
    }
    Ok(())
}
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum HalfwordOperation {
    Literal,
    Copy { length: u32, distance: u32 },
    End,
}
pub fn encode_halfword(decoded: &[u8], tokens: &[HalfwordToken]) -> Res<Vec<u8>> {
    if !decoded.len().is_multiple_of(2) {
        return err("halfword pixels have an odd size");
    }
    let mut operations: Vec<HalfwordOperation> = Vec::new();
    for token in tokens {
        match *token {
            HalfwordToken::Literal(count) => {
                let count = integer(
                    Some(&Value::from(count)),
                    1.0,
                    decoded.len() as f64 / 2.0,
                    "halfword literal count",
                )? as u32;
                for _ in 0..count {
                    operations.push(HalfwordOperation::Literal);
                }
            }
            HalfwordToken::Copy { length, distance } => {
                let length = integer(
                    Some(&Value::from(length)),
                    2.0,
                    33.0,
                    "halfword copy length",
                )? as u32;
                let distance = integer(
                    Some(&Value::from(distance)),
                    1.0,
                    2047.0,
                    "halfword copy distance",
                )? as u32;
                operations.push(HalfwordOperation::Copy { length, distance });
            }
            HalfwordToken::End => operations.push(HalfwordOperation::End),
        }
    }
    let mut encoded: Vec<u8> = Vec::new();
    let mut replay: Vec<u32> = Vec::new();
    let mut operation = 0usize;
    let mut cursor = 0usize;
    let mut ended = false;
    while operation < operations.len() {
        let mut flags: u32 = 0;
        let mut words: Vec<u32> = Vec::new();
        let mut index = 0u32;
        while index < 16 && operation < operations.len() {
            let token = operations[operation];
            if ended {
                return err("halfword plan has data after terminator");
            }
            match token {
                HalfwordOperation::Literal => {
                    if cursor + 2 > decoded.len() {
                        return err("halfword literal crossed decoded pixels");
                    }
                    let value = read_u16(decoded, cursor);
                    cursor += 2;
                    replay.push(value);
                    words.push(value);
                }
                HalfwordOperation::Copy { length, distance } => {
                    flags |= 1 << (15 - index);
                    if distance as usize > replay.len() {
                        return err("halfword copy crossed replay prefix");
                    }
                    words.push((distance << 5) | (length - 2));
                    append_halfword_copy(&mut replay, distance, length)?;
                    cursor += length as usize * 2;
                }
                HalfwordOperation::End => {
                    flags |= 1 << (15 - index);
                    words.push(0);
                    ended = true;
                    if operation + 1 != operations.len() {
                        return err("halfword terminator is not final");
                    }
                }
            }
            index += 1;
            operation += 1;
        }
        let mut group = vec![0u8; 2 + words.len() * 2];
        write_u16(&mut group, 0, flags as u16);
        for (slot, value) in words.iter().enumerate() {
            write_u16(&mut group, 2 + slot * 2, *value as u16);
        }
        encoded.extend_from_slice(&group);
    }
    let mut replay_bytes = vec![0u8; replay.len() * 2];
    for (index, value) in replay.iter().enumerate() {
        write_u16(&mut replay_bytes, index * 2, *value as u16);
    }
    if !ended || cursor != decoded.len() || replay_bytes != decoded {
        return err("halfword plan does not reconstruct decoded pixels");
    }
    Ok(encoded)
}
fn halfword_token_from(value: &Value) -> Res<HalfwordToken> {
    let items = match value {
        Value::Array(items) if !items.is_empty() => items,
        _ => return err("halfword plan token differs"),
    };
    match items[0].as_str() {
        Some("l") => {
            if items.len() != 2 {
                return err("halfword literal token differs");
            }
            let count = integer(items.get(1), 1.0, f64::INFINITY, "halfword literal count")?;
            Ok(HalfwordToken::Literal(count as u32))
        }
        Some("c") => {
            if items.len() != 3 {
                return err("halfword copy token differs");
            }
            let length = integer(items.get(1), 2.0, 33.0, "halfword copy length")?;
            let distance = integer(items.get(2), 1.0, 2047.0, "halfword copy distance")?;
            Ok(HalfwordToken::Copy {
                length: length as u32,
                distance: distance as u32,
            })
        }
        Some("e") if items.len() == 1 => Ok(HalfwordToken::End),
        _ => err("halfword plan token differs"),
    }
}
fn build_halfword_graphic(source: &Value, root: &Path) -> Res<Vec<u8>> {
    let graphic = source
        .get("halfword_graphic")
        .ok_or("halfword graphic layout differs")?;
    if !text_is(graphic.get("address"), &hex(RULE_TABLES_END))
        || !text_is(graphic.get("end"), &hex(HALFWORD_GRAPHIC_END))
        || !number_is(graphic.get("decoded_bytes"), 1536.0)
        || !number_is(graphic.get("columns"), 12.0)
        || !text_is(graphic.get("codec"), "halfword-lz")
    {
        return err("halfword graphic layout differs");
    }
    let tiles = graphic_tiles(graphic, root)?;
    let mut tokens = Vec::new();
    for token in array(graphic.get("tokens")).ok_or("halfword plan token differs")? {
        tokens.push(halfword_token_from(token)?);
    }
    let encoded = encode_halfword(&tiles, &tokens)?;
    if encoded.len() as u32 != HALFWORD_GRAPHIC_END - RULE_TABLES_END {
        return err("halfword graphic encoded size differs");
    }
    Ok(encoded)
}
fn palette_number(value: Option<&Value>) -> f64 {
    match value {
        Some(value) => value.as_f64().unwrap_or_else(|| match value {
            Value::Bool(flag) => f64::from(u8::from(*flag)),
            Value::Null => 0.0,
            _ => f64::NAN,
        }),
        _ => f64::NAN,
    }
}
fn palette_group_from(value: &Value) -> Res<PaletteGroup> {
    let items = match value {
        Value::Array(items) => items,
        _ => return err("invalid palette token group"),
    };
    if items.first().and_then(Value::as_str) == Some("z") {
        return Ok(PaletteGroup::Zeros);
    }
    let inner = match (items.len(), items.first().and_then(Value::as_str)) {
        (2, Some("g")) => array(items.get(1)).ok_or("invalid palette token group")?,
        _ => return err("invalid palette token group"),
    };
    if inner.is_empty() {
        return err("invalid palette token group");
    }
    let mut operations = Vec::with_capacity(inner.len());
    for token in inner {
        let parts = match token {
            Value::Array(parts) if !parts.is_empty() => parts,
            _ => return err("invalid palette copy token"),
        };
        match parts[0].as_str() {
            Some("l") => operations.push(PaletteOperation::Literal),
            Some("e") => operations.push(PaletteOperation::End),
            _ => {
                if parts.len() != 3 || parts[0].as_str() != Some("c") {
                    return err("invalid palette copy token");
                }
                let length = palette_number(parts.get(1));
                let distance = palette_number(parts.get(2));
                if !is_integer(length) || !is_integer(distance) || length < 0.0 || distance < 0.0 {
                    return err("palette copy length is invalid");
                }
                operations.push(PaletteOperation::Copy {
                    length: length as u32,
                    distance: distance as u32,
                });
            }
        }
    }
    Ok(PaletteGroup::Group(operations))
}
fn build_palette_graphics(source: &Value, root: &Path) -> Res<Vec<(u32, u32, Vec<u8>)>> {
    let expected: [(u32, u32, f64); 3] = [
        (0x0809_ff58, 0x080a_001e, 768.0),
        (0x080a_001e, 0x080a_00b8, 768.0),
        (0x080a_00b8, 0x080a_0107, 512.0),
    ];
    let graphics =
        array(source.get("palette_graphics")).ok_or("palette-graphic collection differs")?;
    let mut built = Vec::with_capacity(graphics.len());
    for (index, graphic) in graphics.iter().enumerate() {
        let (start, end, decoded_bytes) = expected[index];
        if !text_is(graphic.get("address"), &hex(start))
            || !text_is(graphic.get("end"), &hex(end))
            || !number_is(graphic.get("decoded_bytes"), decoded_bytes)
            || !number_is(graphic.get("columns"), 2.0)
            || !text_is(graphic.get("codec"), "palette-lz")
        {
            return err("palette graphic layout differs");
        }
        let tiles = graphic_tiles(graphic, root)?;
        let mut groups = Vec::new();
        for token in array(graphic.get("tokens")).ok_or("invalid palette token group")? {
            groups.push(palette_group_from(token)?);
        }
        let encoded = encode_palette(&tiles, &groups).map_err(|error| error.0)?;
        if encoded.len() as u32 != end - start {
            return err("palette graphic encoded size differs");
        }
        built.push((start, end, encoded));
    }
    Ok(built)
}
fn build_sentinel_lookup(source: &Value) -> Res<Vec<u8>> {
    let table = source
        .get("sentinel_lookup")
        .ok_or("sentinel lookup layout differs")?;
    let records = array(table.get("records"));
    if !text_is(table.get("address"), &hex(0x0809_e686))
        || !text_is(table.get("end"), &hex(0x0809_e6b8))
        || table.get("fields") != Some(&string_list(&["key:s16", "value:s16"]))
        || table.get("consumers") != Some(&string_list(&["Func_0808ddb8"]))
        || records.is_none_or(|items| items.len() != 12)
        || !number_is(table.get("sentinel"), -1.0)
    {
        return err("sentinel lookup layout differs");
    }
    let mut output = vec![0u8; 50];
    for (index, record) in records.unwrap().iter().enumerate() {
        let values = array(Some(record));
        if values.is_none_or(|items| items.len() != 2) {
            return err("sentinel lookup record differs");
        }
        let values = values.unwrap();
        let key = integer(values.first(), -32768.0, 32767.0, "lookup key")?;
        let value = integer(values.get(1), -32768.0, 32767.0, "lookup value")?;
        write_u16(&mut output, index * 4, key as i64 as u16);
        write_u16(&mut output, index * 4 + 2, value as i64 as u16);
    }
    write_u16(&mut output, 48, 0xffff);
    Ok(output)
}
fn script_name(address: u32) -> String {
    format!("ObjectScript_{address:08x}")
}
fn script_symbol(name: &str) -> Option<u32> {
    for (_, _, entries) in SCRIPT_LAYOUT {
        for address in *entries {
            if script_name(*address) == name {
                return Some(*address);
            }
        }
    }
    CALLBACK_SYMBOLS
        .iter()
        .find(|(_, symbol)| *symbol == name)
        .map(|(address, _)| *address)
}
fn script_word(value: Option<&Value>) -> Res<u32> {
    if let Some(Value::String(name)) = value {
        return match script_symbol(name) {
            Some(address) => Ok(address),
            None => Err(format!("unknown script symbol: {name}")),
        };
    }
    let number = integer(value, -2147483648.0, 4294967295.0, "script word")?;
    Ok(number as i64 as u32)
}
fn opcode_number(name: &str) -> Res<usize> {
    let digits = match name.strip_prefix("op_") {
        Some(digits) if !digits.is_empty() && digits.bytes().all(|byte| byte.is_ascii_digit()) => {
            digits
        }
        _ => return err("object-script opcode differs"),
    };
    let value = digits
        .parse::<f64>()
        .map_err(|_| "object-script opcode differs".to_string())?;
    let value = integer(
        Some(&Value::from(value)),
        0.0,
        (OPCODE_LAYOUT.len() - 1) as f64,
        "object-script opcode",
    )?;
    Ok(value as usize)
}
fn build_object_scripts(source: &Value) -> Res<Vec<(u32, u32, Vec<u8>)>> {
    let scripts = array(source.get("object_scripts")).ok_or("object-script collection differs")?;
    let mut built = Vec::with_capacity(scripts.len());
    for (index, script) in scripts.iter().enumerate() {
        let (start, end, entries) = SCRIPT_LAYOUT[index];
        let entry_points =
            array(script.get("entry_points")).ok_or("object-script layout differs")?;
        if !text_is(script.get("address"), &hex(start))
            || !text_is(script.get("end"), &hex(end))
            || entry_points.len() != entries.len()
        {
            return err("object-script layout differs");
        }
        let mut chunks: Vec<u8> = Vec::new();
        let mut boundaries: Vec<u32> = Vec::new();
        let mut size = 0u32;
        for command in array(script.get("commands")).ok_or("object-script layout differs")? {
            boundaries.push(start + size);
            if let Some(word) = command.get("skip_word") {
                let word = script_word(Some(word))?;
                if word <= 63 {
                    return err("object-script skip word is dispatchable");
                }
                chunks.extend_from_slice(&word.to_le_bytes());
                size += 4;
                continue;
            }
            let opcode =
                opcode_number(command.get("opcode").and_then(Value::as_str).unwrap_or(""))?;
            let (arity, handler) = OPCODE_LAYOUT[opcode];
            let arguments = array(command.get("arguments"));
            if !text_is(command.get("handler"), handler)
                || arguments.is_none_or(|items| items.len() as u32 != arity)
            {
                return err("object-script command layout differs");
            }
            let mut output = vec![0u8; (1 + arity as usize) * 4];
            write_u32(&mut output, 0, opcode as u32);
            for (slot, argument) in arguments.unwrap().iter().enumerate() {
                write_u32(&mut output, 4 + slot * 4, script_word(Some(argument))?);
            }
            size += output.len() as u32;
            chunks.extend_from_slice(&output);
        }
        if start + size != end {
            return err("object-script extent differs");
        }
        for (entry_index, entry) in entry_points.iter().enumerate() {
            let expected = entries[entry_index];
            if !text_is(entry.get("name"), &script_name(expected))
                || !text_is(entry.get("address"), &hex(expected))
                || !boundaries.contains(&expected)
            {
                return err("object-script entry point differs");
            }
        }
        built.push((start, end, chunks));
    }
    Ok(built)
}
fn build_sine_table(source: &Value) -> Res<Vec<u8>> {
    let table = source
        .get("sine_table")
        .ok_or("sine-table formula differs")?;
    if !text_is(table.get("address"), &hex(SINE_TABLE_ADDRESS))
        || !text_is(table.get("end"), &hex(SINE_TABLE_END))
        || !number_is(table.get("count"), 256.0)
        || !text_is(table.get("type"), "s16")
        || !text_is(table.get("formula"), "trunc(sin(index*pi/128)*0x1000)")
    {
        return err("sine-table formula differs");
    }
    let mut output = vec![0u8; 512];
    for index in 0..256u32 {
        let value = (f64::from(index) * std::f64::consts::PI / 128.0).sin() * 4096.0;
        write_u16(&mut output, index as usize * 2, value.trunc() as i64 as u16);
    }
    Ok(output)
}
fn build_gradients(source: &Value) -> Res<Vec<u8>> {
    let table = source.get("gradients").ok_or("gradient layout differs")?;
    let records = array(table.get("records"));
    if !text_is(table.get("address"), &hex(GRADIENT_ADDRESS))
        || !text_is(table.get("end"), &hex(GRADIENT_END))
        || !number_is(table.get("stride"), 28.0)
        || table.get("consumers") != Some(&string_list(&["Func_0808f52c"]))
        || records.is_none_or(|items| items.len() != 32)
    {
        return err("gradient layout differs");
    }
    let mut output = vec![0u8; (GRADIENT_END - GRADIENT_ADDRESS) as usize];
    for (record_index, record) in records.unwrap().iter().enumerate() {
        let runs = array(record.get("runs")).ok_or("gradient run count differs")?;
        if runs.len() != 3 {
            return err("gradient run count differs");
        }
        let offset = record_index * 28;
        let first = integer(
            record.get("first_color"),
            0.0,
            65535.0,
            "gradient first color",
        )?;
        let second = integer(
            record.get("second_color"),
            0.0,
            65535.0,
            "gradient second color",
        )?;
        write_u16(&mut output, offset, first as u16);
        write_u16(&mut output, offset + 2, second as u16);
        for (run_index, run) in runs.iter().enumerate() {
            let run_offset = offset + 4 + run_index * 8;
            let length = integer(run.get("length"), 0.0, 65535.0, "gradient run length")?;
            let color = integer(run.get("color"), 0.0, 65535.0, "gradient color")?;
            write_u16(&mut output, run_offset, length as u16);
            write_u16(&mut output, run_offset + 2, color as u16);
            output[run_offset + 4] =
                integer(run.get("start_low"), 0.0, 255.0, "gradient low start")? as u8;
            output[run_offset + 5] =
                integer(run.get("end_low"), 0.0, 255.0, "gradient low end")? as u8;
            output[run_offset + 6] =
                integer(run.get("start_high"), 0.0, 255.0, "gradient high start")? as u8;
            output[run_offset + 7] =
                integer(run.get("end_high"), 0.0, 255.0, "gradient high end")? as u8;
        }
    }
    Ok(output)
}
fn build_symbolic_pointers(source: &Value) -> Res<Vec<u8>> {
    let table = source
        .get("symbolic_pointers")
        .ok_or("symbolic pointer table differs")?;
    let expected = Value::Array(
        [0x0809_f13cu32, 0x0809_f148, 0x0809_f154]
            .iter()
            .map(|address| Value::String(script_name(*address)))
            .collect(),
    );
    if !text_is(table.get("address"), &hex(0x080a_012c))
        || !text_is(table.get("end"), &hex(0x080a_0138))
        || table.get("pointers") != Some(&expected)
    {
        return err("symbolic pointer table differs");
    }
    let mut output = vec![0u8; 12];
    for (index, name) in array(table.get("pointers")).unwrap().iter().enumerate() {
        let address = name.as_str().and_then(script_symbol).unwrap_or(0);
        write_u32(&mut output, index * 4, address);
    }
    Ok(output)
}
fn build_sparse_table(source: &Value) -> Res<Vec<u8>> {
    let table = source
        .get("sparse_table")
        .ok_or("sparse-table layout differs")?;
    let sentinel = Value::Array(vec![Value::from(-1), Value::from(0)]);
    if !text_is(table.get("address"), &hex(SPARSE_TABLE_ADDRESS))
        || !text_is(table.get("end"), &hex(SPARSE_TABLE_END))
        || !number_is(table.get("count"), 41.0)
        || table.get("fields") != Some(&string_list(&["key:s32", "value:s32"]))
        || table.get("consumers") != Some(&string_list(&["Func_0809bcf8"]))
        || table.get("sentinel") != Some(&sentinel)
    {
        return err("sparse-table layout differs");
    }
    let mut output = vec![0u8; (SPARSE_TABLE_END - SPARSE_TABLE_ADDRESS) as usize];
    let mut indices: Vec<u32> = Vec::new();
    for record in array(table.get("records")).ok_or("sparse-table layout differs")? {
        let index = integer(record.get("index"), 0.0, 40.0, "sparse-table index")? as u32;
        if indices.contains(&index) {
            return err("sparse-table index is duplicated");
        }
        indices.push(index);
        let key = integer(
            record.get("key"),
            -2147483648.0,
            2147483647.0,
            "sparse-table key",
        )?;
        let value = integer(
            record.get("value"),
            -2147483648.0,
            2147483647.0,
            "sparse-table value",
        )?;
        write_u32(&mut output, index as usize * 8, key as i64 as u32);
        write_u32(&mut output, index as usize * 8 + 4, value as i64 as u32);
    }
    write_u32(&mut output, 41 * 8, 0xffff_ffff);
    Ok(output)
}
fn build_zero_fills(source: &Value) -> Res<Vec<(u32, u32, Vec<u8>)>> {
    let expected = [
        (0x0809_e9eeu32, 0x0809_e9f0u32),
        (0x080a_0107, 0x080a_0108),
        (0x080a_0288, BATTLE_DATA_END),
    ];
    let fills = array(source.get("zero_fills")).ok_or("zero-fill collection differs")?;
    let mut built = Vec::with_capacity(fills.len());
    for (index, fill) in fills.iter().enumerate() {
        let (start, end) = expected[index];
        if !text_is(fill.get("address"), &hex(start)) || !text_is(fill.get("end"), &hex(end)) {
            return err("zero-fill layout differs");
        }
        built.push((start, end, vec![0u8; (end - start) as usize]));
    }
    Ok(built)
}
fn assemble_tail(segments: &[(u32, u32, Vec<u8>)]) -> Res<Vec<u8>> {
    let span = (BATTLE_DATA_END - RULE_TABLES_END) as usize;
    let mut output = vec![0u8; span];
    let mut written = vec![0u8; span];
    for (start, end, bytes) in segments {
        if *start < RULE_TABLES_END
            || *end > BATTLE_DATA_END
            || start >= end
            || bytes.len() as u32 != end - start
        {
            return err("battle-effect tail segment differs");
        }
        let offset = (start - RULE_TABLES_END) as usize;
        for (index, byte) in bytes.iter().enumerate() {
            if written[offset + index] != 0 && output[offset + index] != *byte {
                return Err(format!(
                    "battle-effect overlap differs at {}",
                    hex(start + index as u32)
                ));
            }
            output[offset + index] = *byte;
            written[offset + index] = 1;
        }
    }
    if let Some(hole) = written.iter().position(|flag| *flag == 0) {
        return Err(format!(
            "battle-effect tail has a hole at {}",
            hex(RULE_TABLES_END + hole as u32)
        ));
    }
    Ok(output)
}
pub fn build_battle_effect_data(value: &Value, root: &Path) -> Res<Vec<u8>> {
    let mut prefix = build_direct_graphics(value, root)?;
    prefix.extend_from_slice(&build_weighted_records(value)?);
    prefix.extend_from_slice(&build_typed_tables(value)?);
    if prefix.len() as u32 != RULE_TABLES_END - BATTLE_DATA_ADDRESS {
        return err("battle-effect table prefix extent differs");
    }
    let mut segments: Vec<(u32, u32, Vec<u8>)> = vec![(
        RULE_TABLES_END,
        HALFWORD_GRAPHIC_END,
        build_halfword_graphic(value, root)?,
    )];
    segments.extend(build_tail_tables(value)?);
    segments.push((0x0809_e686, 0x0809_e6b8, build_sentinel_lookup(value)?));
    segments.extend(build_object_scripts(value)?);
    segments.push((SINE_TABLE_ADDRESS, SINE_TABLE_END, build_sine_table(value)?));
    segments.push((GRADIENT_ADDRESS, GRADIENT_END, build_gradients(value)?));
    segments.extend(build_palette_graphics(value, root)?);
    segments.push((0x080a_012c, 0x080a_0138, build_symbolic_pointers(value)?));
    segments.push((
        SPARSE_TABLE_ADDRESS,
        SPARSE_TABLE_END,
        build_sparse_table(value)?,
    ));
    segments.extend(build_zero_fills(value)?);
    let mut output = prefix;
    output.extend_from_slice(&assemble_tail(&segments)?);
    if output.len() as u32 != BATTLE_DATA_END - BATTLE_DATA_ADDRESS {
        return err("battle-effect source extent differs");
    }
    Ok(output)
}
const USAGE: &str = "usage: battle-effect-data build-stdout SOURCE --root ASSETS";
fn option(args: &[String], name: &str) -> Option<String> {
    args.iter()
        .position(|arg| arg == name)
        .and_then(|index| args.get(index + 1))
        .cloned()
}
fn validate_options(args: &[String]) -> Res<()> {
    if args.len() != 4
        || args[0] != "build-stdout"
        || args[1].is_empty()
        || args[2] != "--root"
        || args[3].is_empty()
    {
        return err(USAGE);
    }
    Ok(())
}
pub fn run(args: Vec<String>) -> Res<()> {
    if args.is_empty() || args == ["-h"] || args == ["--help"] {
        println!("{USAGE}");
        return Ok(());
    }
    validate_options(&args)?;
    let input = Path::new(&args[1]);
    let root = option(&args, "--root").ok_or(USAGE)?;
    let text = String::from_utf8_lossy(&read_file(input)?).into_owned();
    let document: Value = serde_json::from_str(&text).map_err(|error| error.to_string())?;
    let built = build_battle_effect_data(&document, Path::new(&root))?;
    std::io::stdout()
        .write_all(&built)
        .map_err(|error| error.to_string())?;
    eprintln!("{{}}");
    Ok(())
}
