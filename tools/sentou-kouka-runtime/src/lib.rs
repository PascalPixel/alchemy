use serde_json::{Map, Value};
use std::fs;
use std::io::{self, Write};
use std::path::{Path, PathBuf};
use std::process::Command;
use std::time::{SystemTime, UNIX_EPOCH};
pub const ROM_BASE: u32 = 0x0800_0000;
pub const SENTOU_KOUKA_ADDRESS: u32 = 0x080e_da78;
pub const SENTOU_KOUKA_END: u32 = 0x080f_0254;
pub const SENTOU_KOUKA_SIZE: usize = (SENTOU_KOUKA_END - SENTOU_KOUKA_ADDRESS) as usize;
const KEISU_ADDRESS: u32 = 0x080e_da78;
const GOUSEI_ADDRESS: u32 = 0x080e_daf0;
const HYOU_A_ADDRESS: u32 = 0x080e_de48;
const KANSUU_ADDRESS: u32 = 0x080e_e2b4;
const HYOU_B_ADDRESS: u32 = 0x080e_e910;
const GOUSEI_IRO_ADDRESS: u32 = 0x080e_efa4;
const BIT_MASK_ADDRESS: u32 = 0x080e_f014;
const ZERO_FILL_ADDRESS: u32 = 0x080e_f054;
const TENKAI_ADDRESS: u32 = 0x080f_0000;
const CALLBACK_COUNT: usize = 407;
const USAGE: &str = "usage: sentou-kouka-runtime build-stdout INDEX";
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);
impl std::fmt::Display for Error {
    fn fmt(&self, output: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        output.write_str(&self.0)
    }
}
impl std::error::Error for Error {}
type Result<T> = std::result::Result<T, Error>;
fn fail<T>(message: impl Into<String>) -> Result<T> {
    Err(Error(message.into()))
}
fn hex(value: u32) -> String {
    format!("0x{value:08x}")
}
fn object<'a>(value: &'a Value, label: &str) -> Result<&'a Map<String, Value>> {
    value
        .as_object()
        .ok_or_else(|| Error(format!("{label} must be an object")))
}
fn field<'a>(value: &'a Map<String, Value>, name: &str, label: &str) -> Result<&'a Value> {
    value
        .get(name)
        .ok_or_else(|| Error(format!("{label} is missing")))
}
fn string<'a>(value: &'a Map<String, Value>, name: &str, label: &str) -> Result<&'a str> {
    field(value, name, label)?
        .as_str()
        .ok_or_else(|| Error(format!("{label} must be a string")))
}
fn signed(value: &Value, minimum: i64, maximum: i64, label: &str) -> Result<i64> {
    let number = value
        .as_i64()
        .ok_or_else(|| Error(format!("invalid {label}")))?;
    if !(minimum..=maximum).contains(&number) {
        return fail(format!("invalid {label}"));
    }
    Ok(number)
}
fn unsigned(value: &Value, maximum: u64, label: &str) -> Result<u64> {
    let number = value
        .as_u64()
        .ok_or_else(|| Error(format!("invalid {label}")))?;
    if number > maximum {
        return fail(format!("invalid {label}"));
    }
    Ok(number)
}
fn array<'a>(value: &'a Value, count: usize, label: &str) -> Result<&'a Vec<Value>> {
    let values = value
        .as_array()
        .ok_or_else(|| Error(format!("{label} requires {count} entries")))?;
    if values.len() != count {
        return fail(format!("{label} requires {count} entries"));
    }
    Ok(values)
}
fn read_document(path: &Path) -> Result<Value> {
    let text = fs::read_to_string(path).map_err(|e| Error(e.to_string()))?;
    let value: Value = serde_json::from_str(&text).map_err(|e| Error(e.to_string()))?;
    if !canonical_json::is_canonical_json_text(&text, &value) {
        return fail(format!("{}: source is not canonical JSON", path.display()));
    }
    Ok(value)
}
#[derive(Clone, Copy)]
enum TableType {
    U8,
    U16,
}
fn table_width(kind: TableType) -> usize {
    match kind {
        TableType::U8 => 1,
        TableType::U16 => 2,
    }
}
fn number(value: &Value, kind: TableType, label: &str) -> Result<u32> {
    let value = unsigned(
        value,
        if matches!(kind, TableType::U8) {
            0xff
        } else {
            0xffff
        },
        label,
    )?;
    Ok(value as u32)
}
fn put_u16(output: &mut [u8], at: usize, value: u16) {
    output[at..at + 2].copy_from_slice(&value.to_le_bytes());
}
fn put_u32(output: &mut [u8], at: usize, value: u32) {
    output[at..at + 4].copy_from_slice(&value.to_le_bytes());
}
fn encode_values(value: &Value, kind: TableType, count: usize, label: &str) -> Result<Vec<u8>> {
    let values = array(value, count, label)?;
    let mut output = vec![0; count * table_width(kind)];
    for (index, item) in values.iter().enumerate() {
        let value = number(item, kind, &format!("{label} value {index}"))?;
        if matches!(kind, TableType::U8) {
            output[index] = value as u8;
        } else {
            put_u16(&mut output, index * 2, value as u16);
        }
    }
    Ok(output)
}
fn build_keisu(path: &Path) -> Result<Vec<u8>> {
    let document = read_document(path)?;
    let source = object(&document, "effect coefficients")?;

    if source.get("format").and_then(Value::as_i64) != Some(1)
        || string(source, "address", "effect coefficients")? != hex(KEISU_ADDRESS)
        || source.get("size").and_then(Value::as_u64)
            != Some((GOUSEI_ADDRESS - KEISU_ADDRESS) as u64)
    {
        return fail("effect coefficient extent differs");
    }
    let mut output = Vec::new();
    for name in ["unit_q16", "negative_offsets"] {
        output.extend(encode_signed(
            field(source, name, "effect coefficients")?,
            4,
            name,
        )?);
    }
    let pairs = array(
        field(source, "coordinate_pairs", "coordinate pairs")?,
        6,
        "coordinate pairs",
    )?;
    for (index, pair) in pairs.iter().enumerate() {
        let pair = array(pair, 2, &format!("coordinate pair {index}"))?;
        for axis in 0..2 {
            let value = signed(&pair[axis], -0x8000, 0x7fff, "coordinate pair")? as i16;
            output.extend(value.to_le_bytes());
        }
    }
    let scales = object(
        field(source, "scales_q16", "effect q16 scales")?,
        "effect q16 scales",
    )?;

    for (name, count) in [
        ("base", 4),
        ("enhanced", 2),
        ("normal_a", 4),
        ("normal_b", 4),
        ("high", 2),
    ] {
        output.extend(encode_signed(
            field(scales, name, "effect q16 scales")?,
            count,
            name,
        )?);
    }
    if output.len() != (GOUSEI_ADDRESS - KEISU_ADDRESS) as usize {
        return fail("effect coefficient size differs");
    }
    Ok(output)
}
fn encode_signed(value: &Value, count: usize, label: &str) -> Result<Vec<u8>> {
    let values = array(value, count, label)?;
    let mut output = Vec::with_capacity(count * 4);
    for item in values {
        output.extend((signed(item, -0x8000_0000, 0x7fff_ffff, label)? as i32).to_le_bytes());
    }
    Ok(output)
}
fn build_table(path: &Path, address: u32, end: u32, first_u16: bool) -> Result<Vec<u8>> {
    let document = read_document(path)?;
    let source = object(&document, "effect table source")?;

    if source.get("format").and_then(Value::as_i64) != Some(1)
        || string(source, "address", "effect table")? != hex(address)
        || source.get("size").and_then(Value::as_u64) != Some((end - address) as u64)
    {
        return fail("effect table extent differs");
    }
    let tables = field(source, "tables", "effect tables")?
        .as_array()
        .ok_or_else(|| Error("effect tables must be an array".to_string()))?;
    if tables.is_empty() {
        return fail("effect tables must not be empty");
    }
    let mut output = Vec::new();
    let mut cursor = address;
    for (index, table) in tables.iter().enumerate() {
        let table = object(table, &format!("effect table {index}"))?;
        let kind = if first_u16 && index == 0 {
            TableType::U16
        } else {
            TableType::U8
        };
        let expected_name = format!(
            "{}_{}",
            if address == HYOU_A_ADDRESS {
                "hyou_a"
            } else {
                "hyou_b"
            },
            format!("{index:03}")
        );
        let expected_type = if matches!(kind, TableType::U16) {
            "u16"
        } else {
            "u8"
        };
        if string(table, "name", "effect table")? != expected_name
            || string(table, "address", "effect table")? != hex(cursor)
            || string(table, "type", "effect table")? != expected_type
        {
            return fail(format!("effect table {index} identity differs"));
        }
        let values = field(table, "values", "effect table")?;
        let count = values
            .as_array()
            .ok_or_else(|| Error(format!("{expected_name} requires an array")))?
            .len();
        let encoded = encode_values(values, kind, count, &expected_name)?;
        cursor = cursor
            .checked_add(encoded.len() as u32)
            .ok_or_else(|| Error("effect table extent overflowed".to_string()))?;
        output.extend(encoded);
    }
    if cursor != end || output.len() != (end - address) as usize {
        return fail("effect table output size differs");
    }
    Ok(output)
}
fn callback_word(value: &Value, index: usize) -> Result<u32> {
    if value.is_null() {
        return Ok(0);
    }
    let text = value
        .as_str()
        .ok_or_else(|| Error(format!("callback {index} must be symbolic or null")))?;
    if text.len() != 13 || !text.starts_with("Func_") {
        return fail(format!("callback {index} symbol differs"));
    }
    let address = u32::from_str_radix(&text[5..], 16)
        .map_err(|_| Error(format!("callback {index} symbol differs")))?;
    if address < ROM_BASE || address >= SENTOU_KOUKA_ADDRESS || address & 1 != 0 {
        return fail(format!("callback {index} target differs"));
    }
    Ok(address | 1)
}
fn build_callbacks(path: &Path) -> Result<Vec<u8>> {
    let document = read_document(path)?;
    let source = object(&document, "effect callbacks")?;

    if source.get("format").and_then(Value::as_i64) != Some(1)
        || string(source, "address", "effect callbacks")? != hex(KANSUU_ADDRESS)
        || source.get("size").and_then(Value::as_u64)
            != Some((HYOU_B_ADDRESS - KANSUU_ADDRESS) as u64)
        || string(source, "mode", "effect callbacks")? != "thumb"
        || source.get("index_bias").and_then(Value::as_i64) != Some(-1)
    {
        return fail("effect callback directory identity differs");
    }
    let entries = array(
        field(source, "entries", "effect callback directory")?,
        CALLBACK_COUNT,
        "effect callback directory",
    )?;
    let mut output = vec![0; CALLBACK_COUNT * 4];
    let mut nonnull = 0;
    let mut unique = std::collections::HashSet::new();
    for (index, entry) in entries.iter().enumerate() {
        let word = callback_word(entry, index)?;
        if word != 0 {
            nonnull += 1;
            unique.insert(word);
        }
        put_u32(&mut output, index * 4, word);
    }
    if nonnull != 162
        || CALLBACK_COUNT - nonnull != 245
        || unique.len() != 143
        || nonnull - unique.len() != 19
    {
        return fail("effect callback null or alias structure differs");
    }
    Ok(output)
}
fn build_masks(path: &Path) -> Result<Vec<u8>> {
    let document = read_document(path)?;
    let source = object(&document, "effect bit masks")?;

    if source.get("format").and_then(Value::as_i64) != Some(1)
        || string(source, "address", "effect masks")? != hex(BIT_MASK_ADDRESS)
        || source.get("size").and_then(Value::as_u64)
            != Some((ZERO_FILL_ADDRESS - BIT_MASK_ADDRESS) as u64)
        || string(source, "type", "effect masks")? != "u16"
        || source.get("repeat").and_then(Value::as_u64) != Some(2)
    {
        return fail("effect bit mask identity differs");
    }
    let values = array(
        field(source, "values", "effect bit masks")?,
        16,
        "effect bit masks",
    )?;
    let mut one = Vec::with_capacity(32);
    for (index, value) in values.iter().enumerate() {
        let value = unsigned(value, 0xffff, "effect bit mask")? as u16;
        let expected = if index < 8 {
            1 << index
        } else {
            ((12 - (index - 8) / 2) << 8 | 1 + (index - 8) % 2) as u64
        };
        if value as u64 != expected {
            return fail("effect bit mask order differs");
        }
        one.extend(value.to_le_bytes());
    }
    Ok([one.clone(), one].concat())
}
fn unique_temp(prefix: &str) -> Result<PathBuf> {
    let nanos = SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .map_err(|e| Error(e.to_string()))?
        .as_nanos();
    let path = std::env::temp_dir().join(format!("{prefix}-{}-{nanos}", std::process::id()));
    fs::create_dir(&path).map_err(|e| Error(e.to_string()))?;
    Ok(path)
}
fn command(program: &str, args: &[String]) -> Result<()> {
    let output = Command::new(program)
        .args(args)
        .output()
        .map_err(|e| Error(format!("{program}: {e}")))?;
    if !output.status.success() {
        let detail = String::from_utf8_lossy(&output.stderr).trim().to_string();
        return fail(if detail.is_empty() {
            format!("{program} failed")
        } else {
            detail
        });
    }
    Ok(())
}
fn assemble(path: &Path, base: u32, expected_size: usize) -> Result<Vec<u8>> {
    let work = unique_temp("sentou-kouka-assemble")?;
    let object = work.join("source.o");
    let elf = work.join("source.elf");
    let binary = work.join("source.bin");
    let result = (|| {
        command(
            "arm-none-eabi-as",
            &[
                "-mcpu=arm7tdmi".into(),
                "-mthumb-interwork".into(),
                "-o".into(),
                object.to_string_lossy().into_owned(),
                path.to_string_lossy().into_owned(),
            ],
        )?;
        command(
            "arm-none-eabi-ld",
            &[
                format!("-Ttext={}", hex(base)),
                "-o".into(),
                elf.to_string_lossy().into_owned(),
                object.to_string_lossy().into_owned(),
            ],
        )?;
        command(
            "arm-none-eabi-objcopy",
            &[
                "-O".into(),
                "binary".into(),
                "-j".into(),
                ".text".into(),
                elf.to_string_lossy().into_owned(),
                binary.to_string_lossy().into_owned(),
            ],
        )?;
        let bytes = fs::read(&binary).map_err(|e| Error(e.to_string()))?;
        if bytes.len() != expected_size {
            return fail(format!("{}: assembled size differs", path.display()));
        }
        Ok(bytes)
    })();
    let _ = fs::remove_dir_all(&work);
    result
}
#[derive(Debug, Clone)]
struct Sources {
    prefix: String,
    keisu: String,
    gousei: String,
    hyou_a: String,
    kansuu: String,
    hyou_b: String,
    gousei_iro: String,
    bit_mask: String,
    tenkai: String,
}
fn parse_index(path: &Path) -> Result<Sources> {
    let document = read_document(path)?;
    let index = object(&document, "battle effect runtime index")?;

    if index.get("format").and_then(Value::as_i64) != Some(1)
        || string(index, "kind", "runtime index")? != "golden-sun-sentou-kouka-runtime"
        || string(index, "address", "runtime index")? != hex(SENTOU_KOUKA_ADDRESS)
        || string(index, "end", "runtime index")? != hex(SENTOU_KOUKA_END)
        || index.get("size").and_then(Value::as_u64) != Some(SENTOU_KOUKA_SIZE as u64)
    {
        return fail("battle effect runtime index extent differs");
    }
    let sources = object(
        field(index, "sources", "runtime sources")?,
        "runtime sources",
    )?;

    let expected = [
        ("keisu", "keisu.json"),
        ("gousei", "gousei.s"),
        ("hyou_a", "hyou_a.json"),
        ("kansuu", "kansuu_hyou.json"),
        ("hyou_b", "hyou_b.json"),
        ("gousei_iro", "gousei_iro.s"),
        ("bit_mask", "bit_mask.json"),
        ("tenkai", "tenkai.s"),
    ];
    for (key, value) in expected {
        if string(sources, key, "runtime source")? != value {
            return fail(format!("battle effect runtime {key} source differs"));
        }
    }
    let name = path.file_name().and_then(|x| x.to_str()).unwrap_or("");
    let prefix = name
        .strip_suffix("index.json")
        .ok_or_else(|| Error("battle effect runtime index name differs".into()))?
        .to_string();
    Ok(Sources {
        prefix,
        keisu: "keisu.json".into(),
        gousei: "gousei.s".into(),
        hyou_a: "hyou_a.json".into(),
        kansuu: "kansuu_hyou.json".into(),
        hyou_b: "hyou_b.json".into(),
        gousei_iro: "gousei_iro.s".into(),
        bit_mask: "bit_mask.json".into(),
        tenkai: "tenkai.s".into(),
    })
}
fn source_path(directory: &Path, sources: &Sources, name: &str) -> PathBuf {
    directory.join(format!("{}{}", sources.prefix, name))
}
pub fn build_sentou_kouka_runtime(index_path: &Path) -> Result<Vec<u8>> {
    let sources = parse_index(index_path)?;
    let directory = index_path
        .parent()
        .ok_or_else(|| Error("index has no parent".into()))?;
    let mut output = Vec::new();
    output.extend(build_keisu(&source_path(
        directory,
        &sources,
        &sources.keisu,
    ))?);
    output.extend(assemble(
        &source_path(directory, &sources, &sources.gousei),
        GOUSEI_ADDRESS,
        (HYOU_A_ADDRESS - GOUSEI_ADDRESS) as usize,
    )?);
    output.extend(build_table(
        &source_path(directory, &sources, &sources.hyou_a),
        HYOU_A_ADDRESS,
        KANSUU_ADDRESS,
        true,
    )?);
    output.extend(build_callbacks(&source_path(
        directory,
        &sources,
        &sources.kansuu,
    ))?);
    output.extend(build_table(
        &source_path(directory, &sources, &sources.hyou_b),
        HYOU_B_ADDRESS,
        GOUSEI_IRO_ADDRESS,
        false,
    )?);
    output.extend(assemble(
        &source_path(directory, &sources, &sources.gousei_iro),
        GOUSEI_IRO_ADDRESS,
        (BIT_MASK_ADDRESS - GOUSEI_IRO_ADDRESS) as usize,
    )?);
    output.extend(build_masks(&source_path(
        directory,
        &sources,
        &sources.bit_mask,
    ))?);
    output.resize(
        output.len() + (TENKAI_ADDRESS - ZERO_FILL_ADDRESS) as usize,
        0,
    );
    output.extend(assemble(
        &source_path(directory, &sources, &sources.tenkai),
        TENKAI_ADDRESS,
        (SENTOU_KOUKA_END - TENKAI_ADDRESS) as usize,
    )?);
    if output.len() != SENTOU_KOUKA_SIZE {
        return fail("battle effect runtime output size differs");
    }
    Ok(output)
}
pub fn run(args: Vec<String>) -> Result<()> {
    if args.is_empty() || args == ["-h"] || args == ["--help"] {
        println!("{USAGE}");
        return Ok(());
    }
    match args.as_slice() {
        [command, index] if command == "build-stdout" => io::stdout()
            .write_all(&build_sentou_kouka_runtime(Path::new(index))?)
            .map_err(|error| Error(error.to_string())),
        _ => fail(USAGE),
    }
}
