// Rust port of tools/make/localization_tables.ts.
//
// Why this file exists: the three localization table regions in the main image
// are data, not code, and the branch tracks them as reviewable JSON records
// rather than as a ROM span. This tool is the only thing that converts between
// the two directions, so it has to be exact in BOTH: `export` must produce the
// records byte-for-byte as they are already tracked, and `build` must produce
// the ROM bytes back. Its own export path proves the round trip before it
// writes anything, and `verify` re-proves it against the ROM afterwards.
//
// The port keeps the region layout table, the field-order of the emitted
// records, and every error message verbatim, because all three are compared
// against the TypeScript in the parity run and against the tracked files by the
// rest of the toolchain.

mod jsnum;
mod json;

use std::io::Write;
use std::path::{Path, PathBuf};

use jsnum::{is_safe_integer, to_number};
use json::{canonical_json, minified, Value};

pub const ROM_BASE: u32 = 0x0800_0000;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum ElementKind {
    U8,
    S8,
    U16,
    U32,
    AsciiFixed,
}

impl ElementKind {
    fn name(self) -> &'static str {
        match self {
            ElementKind::U8 => "u8",
            ElementKind::S8 => "s8",
            ElementKind::U16 => "u16",
            ElementKind::U32 => "u32",
            ElementKind::AsciiFixed => "ascii-fixed",
        }
    }

    fn width(self) -> usize {
        match self {
            ElementKind::U8 | ElementKind::S8 | ElementKind::AsciiFixed => 1,
            ElementKind::U16 => 2,
            ElementKind::U32 => 4,
        }
    }
}

struct SegmentLayout {
    start: u32,
    end: u32,
    element: ElementKind,
    stride: u32,
    consumers: &'static [&'static str],
}

struct RegionLayout {
    start: u32,
    end: u32,
    segments: &'static [SegmentLayout],
}

const fn s(
    start: u32,
    end: u32,
    element: ElementKind,
    stride: u32,
    consumers: &'static [&'static str],
) -> SegmentLayout {
    SegmentLayout {
        start,
        end,
        element,
        stride,
        consumers,
    }
}

use ElementKind::{AsciiFixed, S8, U16, U32, U8};

const REGION_0: [SegmentLayout; 4] = [
    s(0x0803_3e60, 0x0803_3eb0, U8, 1, &["Func_080191cc"]),
    s(0x0803_3eb0, 0x0803_3ed8, S8, 2, &["Func_080191cc"]),
    s(0x0803_3ed8, 0x0803_3ee8, U8, 1, &[]),
    s(0x0803_3ee8, 0x0803_3ef8, S8, 1, &["Func_080191cc"]),
];

const REGION_1: [SegmentLayout; 13] = [
    s(
        0x0803_66f8,
        0x0803_6738,
        U16,
        2,
        &["Func_0801908c", "Func_08021e6c", "Func_08028194"],
    ),
    s(0x0803_6738, 0x0803_6740, S8, 1, &["Func_0801908c"]),
    s(0x0803_6740, 0x0803_6750, S8, 1, &["Func_0801a98c"]),
    s(
        0x0803_6750,
        0x0803_67b0,
        U8,
        1,
        &["Func_0801ca1c", "Func_0801ccc0"],
    ),
    s(0x0803_67b0, 0x0803_67c9, U8, 1, &[]),
    s(
        0x0803_67c9,
        0x0803_67cc,
        S8,
        1,
        &["Func_0801d108", "Func_0801d4cc"],
    ),
    s(
        0x0803_67cc,
        0x0803_67ce,
        S8,
        1,
        &["Func_0801d108", "Func_0801d4cc"],
    ),
    s(
        0x0803_67ce,
        0x0803_67d0,
        S8,
        1,
        &["Func_0801d108", "Func_0801d4cc"],
    ),
    s(0x0803_67d0, 0x0803_67d6, S8, 1, &["Func_0801d9d4"]),
    s(0x0803_67d6, 0x0803_67dc, S8, 1, &["Func_0801d9d4"]),
    s(0x0803_67dc, 0x0803_67e4, S8, 1, &["Func_0801d9d4"]),
    s(
        0x0803_67e4,
        0x0803_680c,
        U16,
        2,
        &["Func_08019d2c", "Func_0802977c"],
    ),
    s(
        0x0803_680c,
        0x0803_68d4,
        U16,
        4,
        &["Func_08019d2c", "Func_0802977c"],
    ),
];

const REGION_2: [SegmentLayout; 35] = [
    s(0x0803_70d4, 0x0803_71b4, U8, 1, &["Func_0801de5c"]),
    s(0x0803_71b4, 0x0803_71c4, U8, 1, &["Func_0801de5c"]),
    s(0x0803_71c4, 0x0803_71e0, U8, 1, &["Func_0801ef68"]),
    s(0x0803_71e0, 0x0803_71f6, U8, 1, &["Func_08020198"]),
    s(0x0803_71f6, 0x0803_71fe, S8, 1, &["Func_08020bd8"]),
    s(0x0803_71fe, 0x0803_7206, U16, 2, &["Func_08021228"]),
    s(0x0803_7206, 0x0803_7216, U16, 2, &["Func_08021360"]),
    s(0x0803_7216, 0x0803_7226, U16, 2, &["Func_08021360"]),
    s(0x0803_7226, 0x0803_7230, S8, 1, &["Func_080216b4"]),
    s(0x0803_7230, 0x0803_7250, U32, 4, &["Func_080217a4"]),
    s(0x0803_7250, 0x0803_7280, U8, 8, &["Func_08021848"]),
    s(0x0803_7280, 0x0803_72c0, U8, 4, &["Func_080219c8"]),
    s(0x0803_72c0, 0x0803_7300, U16, 2, &["Func_08021a18"]),
    s(0x0803_7300, 0x0803_7308, AsciiFixed, 1, &["Func_08021c34"]),
    s(0x0803_7308, 0x0803_7328, U8, 1, &["Func_08023e70"]),
    s(0x0803_7328, 0x0803_73a8, U8, 8, &["Func_08023178"]),
    s(0x0803_73a8, 0x0803_73b8, U8, 4, &["Func_08023178"]),
    s(0x0803_73b8, 0x0803_73d8, U8, 4, &["Func_08023178"]),
    s(0x0803_73d8, 0x0803_73dc, AsciiFixed, 1, &["Func_08023178"]),
    s(0x0803_73dc, 0x0803_73e0, AsciiFixed, 1, &["Func_08023178"]),
    s(0x0803_73e0, 0x0803_73e4, AsciiFixed, 1, &["Func_08023178"]),
    s(0x0803_73e4, 0x0803_73e7, AsciiFixed, 1, &["Func_08023178"]),
    s(0x0803_73e7, 0x0803_73eb, U8, 2, &["Func_08023e70"]),
    s(0x0803_73eb, 0x0803_73ef, U8, 2, &["Func_08023e70"]),
    s(0x0803_73ef, 0x0803_73f7, U8, 1, &["Func_080286a0"]),
    s(0x0803_73f7, 0x0803_7403, U8, 1, &["Func_08028920"]),
    s(0x0803_7403, 0x0803_740f, U8, 1, &["Func_08028920"]),
    s(0x0803_740f, 0x0803_7428, U8, 1, &["Func_080289e8"]),
    s(
        0x0803_7428,
        0x0803_742c,
        AsciiFixed,
        1,
        &["Func_08028ef0", "Func_080292c4"],
    ),
    s(0x0803_742c, 0x0803_7440, AsciiFixed, 1, &["Func_080292c4"]),
    s(0x0803_7440, 0x0803_7448, AsciiFixed, 1, &["Func_08029554"]),
    s(0x0803_7448, 0x0803_7450, AsciiFixed, 1, &["Func_08029554"]),
    s(0x0803_7450, 0x0803_7458, AsciiFixed, 1, &["Func_08029554"]),
    s(0x0803_7458, 0x0803_7460, AsciiFixed, 1, &["Func_08029554"]),
    s(0x0803_7460, 0x0803_7464, AsciiFixed, 1, &["Func_08029554"]),
];

const REGIONS: [RegionLayout; 3] = [
    RegionLayout {
        start: 0x0803_3e60,
        end: 0x0803_3ef8,
        segments: &REGION_0,
    },
    RegionLayout {
        start: 0x0803_66f8,
        end: 0x0803_68d4,
        segments: &REGION_1,
    },
    RegionLayout {
        start: 0x0803_70d4,
        end: 0x0803_7464,
        segments: &REGION_2,
    },
];

fn hex(value: u32) -> String {
    format!("0x{value:08x}")
}

fn integer(value: Option<f64>, label: &str) -> Result<f64, String> {
    match value {
        Some(number) if is_safe_integer(number) => Ok(number),
        _ => Err(format!("{label} must be an integer")),
    }
}

/// JS sorts strings by UTF-16 code unit, not by byte. Every key here is ASCII,
/// where the two agree, but the comparator is written the JS way so that a
/// future non-ASCII field cannot silently reorder.
fn utf16_cmp(left: &str, right: &str) -> std::cmp::Ordering {
    left.encode_utf16().cmp(right.encode_utf16())
}

fn exact_keys(value: &Value, keys: &[&str], label: &str) -> Result<(), String> {
    let mut present = value.own_keys();
    present.sort_by(|a, b| utf16_cmp(a, b));
    let mut wanted: Vec<String> = keys.iter().map(|k| (*k).to_string()).collect();
    wanted.sort_by(|a, b| utf16_cmp(a, b));
    if present.join(",") != wanted.join(",") {
        return Err(format!("{label} has unknown fields"));
    }
    Ok(())
}

fn numeric(source: &Value, element: ElementKind, size: usize) -> Result<Vec<u8>, String> {
    let width = element.width();
    let stride = source
        .get("stride")
        .and_then(Value::as_num)
        .unwrap_or(f64::NAN);
    let values = match source.get("values") {
        Some(Value::Arr(items)) => items.clone(),
        _ => Vec::new(),
    };
    let width_f = width as f64;
    let size_f = size as f64;
    // Written as `<` rather than `!(>=)` so a NaN stride behaves as it does in
    // JS: the comparison is false, and the modulo below is what rejects it.
    if stride < width_f
        || stride % width_f != 0.0
        || size_f % stride != 0.0
        || values.len() as f64 != size_f / width_f
    {
        return Err("numeric table layout differs".to_string());
    }
    let mut output = vec![0u8; size];
    for (index, raw) in values.iter().enumerate() {
        let value = integer(raw.as_num(), "table value")?;
        match element {
            ElementKind::U8 => {
                if !(0.0..=255.0).contains(&value) {
                    return Err("u8 table value is outside its range".to_string());
                }
                output[index] = value as u8;
            }
            ElementKind::S8 => {
                if !(-128.0..=127.0).contains(&value) {
                    return Err("s8 table value is outside its range".to_string());
                }
                output[index] = (value as i8) as u8;
            }
            ElementKind::U16 => {
                if !(0.0..=65535.0).contains(&value) {
                    return Err("u16 table value is outside its range".to_string());
                }
                output[index * 2..index * 2 + 2].copy_from_slice(&(value as u16).to_le_bytes());
            }
            ElementKind::U32 => {
                if !(0.0..=4_294_967_295.0).contains(&value) {
                    return Err("u32 table value is outside its range".to_string());
                }
                output[index * 4..index * 4 + 4].copy_from_slice(&(value as u32).to_le_bytes());
            }
            ElementKind::AsciiFixed => unreachable!("ascii-fixed never reaches numeric()"),
        }
    }
    Ok(output)
}

fn is_printable_ascii(text: &str) -> bool {
    text.chars().all(|ch| ('\u{20}'..='\u{7e}').contains(&ch))
}

fn text_segment(source: &Value, size: usize) -> Result<Vec<u8>, String> {
    let stride = source.get("stride").and_then(Value::as_num);
    let value = match source.get("text") {
        Some(Value::Str(text)) => text.clone(),
        // A non-string `text` stringifies before the regex sees it; every such
        // spelling fails the printable test, so it lands on the same error.
        _ => "\u{0}".to_string(),
    };
    if stride != Some(1.0) || !is_printable_ascii(&value) {
        return Err("fixed text differs".to_string());
    }
    let encoded = value.into_bytes();
    if encoded.len() >= size {
        return Err("fixed text lacks room for its terminator".to_string());
    }
    let mut output = vec![0u8; size];
    output[..encoded.len()].copy_from_slice(&encoded);
    Ok(output)
}

fn layout_for(source: &Value) -> Result<&'static RegionLayout, String> {
    let address = source.get("address").and_then(Value::as_str);
    let size = source.get("size").and_then(Value::as_str);
    let region = REGIONS
        .iter()
        .find(|item| address == Some(hex(item.start).as_str()));
    match region {
        Some(region) if size == Some(hex(region.end - region.start).as_str()) => Ok(region),
        _ => Err("localization-table region differs".to_string()),
    }
}

pub fn build_localization_tables(value: &Value) -> Result<Vec<u8>, String> {
    // `typeof null === "object"`, so the TypeScript rules null out explicitly;
    // arrays pass this check and are caught by exactKeys instead.
    if !matches!(value, Value::Obj(_) | Value::Arr(_)) {
        return Err("localization-table source must be an object".to_string());
    }
    exact_keys(
        value,
        &["format", "kind", "address", "size", "segments"],
        "localization-table source",
    )?;
    let segments = match value.get("segments") {
        Some(Value::Arr(items)) => items.clone(),
        _ => return Err("localization-table source differs".to_string()),
    };
    if value.get("format").and_then(Value::as_num) != Some(1.0)
        || value.get("kind").and_then(Value::as_str) != Some("golden-sun-localization-tables")
    {
        return Err("localization-table source differs".to_string());
    }
    let region = layout_for(value)?;
    if segments.len() != region.segments.len() {
        return Err("localization-table segment count differs".to_string());
    }
    let mut result = Vec::new();
    for (index, segment) in segments.iter().enumerate() {
        let expected = &region.segments[index];
        let is_text = segment.get("element").and_then(Value::as_str) == Some("ascii-fixed");
        exact_keys(
            segment,
            if is_text {
                &["address", "end", "element", "stride", "consumers", "text"]
            } else {
                &["address", "end", "element", "stride", "consumers", "values"]
            },
            &format!("localization-table segment {index}"),
        )?;
        let consumers = Value::Arr(
            expected
                .consumers
                .iter()
                .map(|name| Value::Str((*name).to_string()))
                .collect(),
        );
        let given_consumers = segment.get("consumers").cloned().unwrap_or(Value::Null);
        if segment.get("address").and_then(Value::as_str) != Some(hex(expected.start).as_str())
            || segment.get("end").and_then(Value::as_str) != Some(hex(expected.end).as_str())
            || segment.get("element").and_then(Value::as_str) != Some(expected.element.name())
            || segment.get("stride").and_then(Value::as_num) != Some(f64::from(expected.stride))
            || minified(&given_consumers) != minified(&consumers)
        {
            return Err(format!("localization-table segment {index} differs"));
        }
        let size = (expected.end - expected.start) as usize;
        let part = if is_text {
            text_segment(segment, size)?
        } else {
            numeric(segment, expected.element, size)?
        };
        result.extend_from_slice(&part);
    }
    if result.len() != (region.end - region.start) as usize {
        return Err("localization-table extent differs".to_string());
    }
    Ok(result)
}

fn range(rom: &[u8], start: u32, end: u32) -> Result<&[u8], String> {
    let begin = i64::from(start) - i64::from(ROM_BASE);
    let finish = i64::from(end) - i64::from(ROM_BASE);
    if begin < 0 || finish > rom.len() as i64 || begin > finish {
        return Err("ROM range differs".to_string());
    }
    Ok(&rom[begin as usize..finish as usize])
}

fn export_segment(rom: &[u8], layout: &SegmentLayout) -> Result<Value, String> {
    let source = range(rom, layout.start, layout.end)?;
    let mut fields: Vec<(String, Value)> = vec![
        ("address".to_string(), Value::Str(hex(layout.start))),
        ("end".to_string(), Value::Str(hex(layout.end))),
        ("stride".to_string(), Value::Num(f64::from(layout.stride))),
        (
            "consumers".to_string(),
            Value::Arr(
                layout
                    .consumers
                    .iter()
                    .map(|name| Value::Str((*name).to_string()))
                    .collect(),
            ),
        ),
    ];
    if layout.element == ElementKind::AsciiFixed {
        let zero = match source.iter().position(|byte| *byte == 0) {
            Some(at) => at,
            None => return Err("fixed text padding differs".to_string()),
        };
        if source[zero..].iter().any(|byte| *byte != 0) {
            return Err("fixed text padding differs".to_string());
        }
        // PORT NOTE: `Buffer#toString("ascii")` masks every byte with 0x7f
        // before decoding, so 0xc1 decodes as 'A' and passes the printable
        // check. Reproduce the mask; a plain UTF-8 decode would reject bytes
        // the TypeScript accepts (and a lossy one would emit U+FFFD).
        let value: String = source[..zero]
            .iter()
            .map(|byte| char::from(byte & 0x7f))
            .collect();
        if !is_printable_ascii(&value) {
            return Err("fixed text is not printable ASCII".to_string());
        }
        // `stride: 1` is respelled after `element` in the TypeScript, but the
        // key already exists, so it keeps its original position.
        fields.push(("element".to_string(), Value::Str("ascii-fixed".to_string())));
        fields.push(("text".to_string(), Value::Str(value)));
        return Ok(Value::Obj(fields));
    }
    let mut values: Vec<Value> = Vec::new();
    match layout.element {
        ElementKind::U8 => values.extend(source.iter().map(|b| Value::Num(f64::from(*b)))),
        ElementKind::S8 => {
            values.extend(source.iter().map(|b| Value::Num(f64::from(*b as i8))));
        }
        ElementKind::U16 => {
            for pair in source.chunks_exact(2) {
                values.push(Value::Num(f64::from(u16::from_le_bytes([
                    pair[0], pair[1],
                ]))));
            }
        }
        ElementKind::U32 => {
            for quad in source.chunks_exact(4) {
                values.push(Value::Num(f64::from(u32::from_le_bytes([
                    quad[0], quad[1], quad[2], quad[3],
                ]))));
            }
        }
        ElementKind::AsciiFixed => unreachable!(),
    }
    fields.push((
        "element".to_string(),
        Value::Str(layout.element.name().to_string()),
    ));
    fields.push(("values".to_string(), Value::Arr(values)));
    Ok(Value::Obj(fields))
}

pub fn export_localization_tables(rom: &[u8]) -> Result<Vec<Value>, String> {
    let mut sources = Vec::new();
    for region in REGIONS.iter() {
        let mut segments = Vec::new();
        for segment in region.segments {
            segments.push(export_segment(rom, segment)?);
        }
        let source = Value::obj(vec![
            ("format", Value::Num(1.0)),
            (
                "kind",
                Value::Str("golden-sun-localization-tables".to_string()),
            ),
            ("address", Value::Str(hex(region.start))),
            ("size", Value::Str(hex(region.end - region.start))),
            ("segments", Value::Arr(segments)),
        ]);
        if build_localization_tables(&source)? != range(rom, region.start, region.end)? {
            return Err(format!(
                "exported table region {} differs",
                hex(region.start)
            ));
        }
        sources.push(source);
    }
    Ok(sources)
}

pub fn self_test() -> Result<(), String> {
    // PORT NOTE: added here, with no counterpart in the TypeScript. The
    // TypeScript self-test builds whatever REGIONS happens to contain, so an
    // empty or gappy layout table would sail through it and then make `export`
    // print `regions=0 bytes=0` and exit 0 — a gate that passes without
    // scanning anything. Refuse that outright. This can only fire on a layout
    // edit, so it never changes the output of a healthy run.
    if REGIONS.is_empty() {
        return Err("localization-table layout covers nothing".to_string());
    }
    for region in REGIONS.iter() {
        let mut cursor = region.start;
        for segment in region.segments {
            if segment.start != cursor || segment.end <= segment.start {
                return Err("localization-table layout covers nothing".to_string());
            }
            cursor = segment.end;
        }
        if cursor != region.end {
            return Err("localization-table layout covers nothing".to_string());
        }
    }
    for region in REGIONS.iter() {
        let segments: Vec<Value> = region
            .segments
            .iter()
            .map(|segment| {
                let mut fields: Vec<(String, Value)> = vec![
                    ("address".to_string(), Value::Str(hex(segment.start))),
                    ("end".to_string(), Value::Str(hex(segment.end))),
                    ("stride".to_string(), Value::Num(f64::from(segment.stride))),
                    (
                        "consumers".to_string(),
                        Value::Arr(
                            segment
                                .consumers
                                .iter()
                                .map(|name| Value::Str((*name).to_string()))
                                .collect(),
                        ),
                    ),
                ];
                fields.push((
                    "element".to_string(),
                    Value::Str(segment.element.name().to_string()),
                ));
                if segment.element == ElementKind::AsciiFixed {
                    fields.push(("text".to_string(), Value::Str(String::new())));
                } else {
                    let count = (segment.end - segment.start) as usize / segment.element.width();
                    fields.push((
                        "values".to_string(),
                        Value::Arr(vec![Value::Num(0.0); count]),
                    ));
                }
                Value::Obj(fields)
            })
            .collect();
        let source = Value::obj(vec![
            ("format", Value::Num(1.0)),
            (
                "kind",
                Value::Str("golden-sun-localization-tables".to_string()),
            ),
            ("address", Value::Str(hex(region.start))),
            ("size", Value::Str(hex(region.end - region.start))),
            ("segments", Value::Arr(segments)),
        ]);
        if build_localization_tables(&source)?.len() != (region.end - region.start) as usize {
            return Err("localization-table self-test failed".to_string());
        }
    }
    println!("self-test=ok");
    Ok(())
}

fn option(args: &[String], name: &str) -> Option<String> {
    let index = args.iter().position(|item| item == name)?;
    args.get(index + 1).cloned()
}

fn source_name(source: &Value) -> String {
    let address = source.get("address").and_then(Value::as_str).unwrap_or("");
    format!("table_{}.json", &address[2.min(address.len())..])
}

fn read(path: &str) -> Result<Vec<u8>, String> {
    std::fs::read(path).map_err(|error| format!("{path}: {error}"))
}

fn base_name(path: &str) -> String {
    Path::new(path)
        .file_name()
        .map(|name| name.to_string_lossy().into_owned())
        .unwrap_or_else(|| path.to_string())
}

fn truthy(value: Option<&String>) -> Option<&String> {
    value.filter(|text| !text.is_empty())
}

const USAGE: &str = "usage: localization-tables {export ROM --directory DIR|build SOURCE --output FILE|verify ROM SOURCE}";

fn run(mut args: Vec<String>) -> Result<(), String> {
    if args.iter().any(|item| item == "--self-test") {
        self_test()?;
        args.retain(|item| item != "--self-test");
        if args.is_empty() {
            return Ok(());
        }
    }
    if let [command, source] = args.as_slice() {
        if command == "build-stdout" {
            let text = String::from_utf8_lossy(&read(source)?).into_owned();
            let built = build_localization_tables(&json::parse(&text)?)?;
            std::io::stdout()
                .write_all(&built)
                .map_err(|error| error.to_string())?;
            return Ok(());
        }
    }
    let command = args.first().map(String::as_str).unwrap_or("");
    if command == "export" {
        let rom_path = truthy(args.get(1)).cloned();
        let directory = option(&args, "--directory").filter(|text| !text.is_empty());
        let (rom_path, directory) = match (rom_path, directory) {
            (Some(rom), Some(dir)) => (rom, dir),
            _ => return Err("usage: localization-tables export ROM --directory DIR".to_string()),
        };
        let sources = export_localization_tables(&read(&rom_path)?)?;
        std::fs::create_dir_all(&directory).map_err(|error| format!("{directory}: {error}"))?;
        let mut total = 0.0f64;
        for source in &sources {
            let path = PathBuf::from(&directory).join(source_name(source));
            std::fs::write(&path, format!("{}\n", canonical_json(source)))
                .map_err(|error| format!("{}: {error}", path.display()))?;
            total += to_number(source.get("size").and_then(Value::as_str).unwrap_or(""));
        }
        println!(
            "identical=true regions={} bytes={}",
            sources.len(),
            json::number_text(total)
        );
        return Ok(());
    }
    if command == "build" {
        let input = truthy(args.get(1)).cloned();
        let output = option(&args, "--output").filter(|text| !text.is_empty());
        let (input, output) = match (input, output) {
            (Some(input), Some(output)) => (input, output),
            _ => return Err("usage: localization-tables build SOURCE --output FILE".to_string()),
        };
        let text = String::from_utf8_lossy(&read(&input)?).into_owned();
        // PORT NOTE: which inputs `json::parse` accepts matches `JSON.parse`
        // exactly, but the refusal text does not. Bun's message is
        // JavaScriptCore's ("JSON Parse error: Expected '}'"), which is neither
        // stable across engines nor read by any caller; the exit code and the
        // accept/reject decision are what the parity run pins.
        let built = build_localization_tables(&json::parse(&text)?)?;
        if let Some(parent) = Path::new(&output).parent() {
            if !parent.as_os_str().is_empty() {
                std::fs::create_dir_all(parent)
                    .map_err(|error| format!("{}: {error}", parent.display()))?;
            }
        }
        std::fs::write(&output, &built).map_err(|error| format!("{output}: {error}"))?;
        println!("bytes={}", built.len());
        return Ok(());
    }
    if command == "verify" {
        let rom_path = truthy(args.get(1)).cloned();
        let input = truthy(args.get(2)).cloned();
        let (rom_path, input) = match (rom_path, input) {
            (Some(rom), Some(input)) => (rom, input),
            _ => return Err("usage: localization-tables verify ROM SOURCE".to_string()),
        };
        let text = String::from_utf8_lossy(&read(&input)?).into_owned();
        let source = json::parse(&text)?;
        let built = build_localization_tables(&source)?;
        let start = to_number(source.get("address").and_then(Value::as_str).unwrap_or(""));
        let rom = read(&rom_path)?;
        // `start` came out of the record, so it can be NaN or out of range;
        // both land in `range`'s refusal exactly as the arithmetic does in JS.
        if !start.is_finite() || start < 0.0 || start > f64::from(u32::MAX) {
            return Err("ROM range differs".to_string());
        }
        let start = start as u32;
        let end = start.wrapping_add(built.len() as u32);
        if range(&rom, start, end)? != built {
            return Err(format!("{} differs from ROM", base_name(&input)));
        }
        println!("identical=true bytes={}", built.len());
        return Ok(());
    }
    Err(USAGE.to_string())
}

fn main() {
    let args: Vec<String> = std::env::args().skip(1).collect();
    if let Err(message) = run(args) {
        // PORT NOTE: Bun prints a source excerpt and a stack trace around this
        // line. The `error: <message>` line itself is identical, and that is
        // what the parity run compares; the excerpt is noise no caller reads.
        eprintln!("error: {message}");
        std::process::exit(1);
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn zeroed_rom() -> Vec<u8> {
        vec![0u8; 0x0080_0000]
    }

    fn segment_source(layout: &SegmentLayout, values: Vec<Value>) -> Value {
        Value::obj(vec![
            ("address", Value::Str(hex(layout.start))),
            ("end", Value::Str(hex(layout.end))),
            ("stride", Value::Num(f64::from(layout.stride))),
            (
                "consumers",
                Value::Arr(
                    layout
                        .consumers
                        .iter()
                        .map(|name| Value::Str((*name).to_string()))
                        .collect(),
                ),
            ),
            ("element", Value::Str(layout.element.name().to_string())),
            (
                if layout.element == ElementKind::AsciiFixed {
                    "text"
                } else {
                    "values"
                },
                if layout.element == ElementKind::AsciiFixed {
                    Value::Str(String::new())
                } else {
                    Value::Arr(values)
                },
            ),
        ])
    }

    fn zero_source(region: &RegionLayout) -> Value {
        let segments: Vec<Value> = region
            .segments
            .iter()
            .map(|segment| {
                let count = (segment.end - segment.start) as usize / segment.element.width();
                segment_source(segment, vec![Value::Num(0.0); count])
            })
            .collect();
        Value::obj(vec![
            ("format", Value::Num(1.0)),
            (
                "kind",
                Value::Str("golden-sun-localization-tables".to_string()),
            ),
            ("address", Value::Str(hex(region.start))),
            ("size", Value::Str(hex(region.end - region.start))),
            ("segments", Value::Arr(segments)),
        ])
    }

    #[test]
    fn a_zero_rom_round_trips_through_export_and_build() {
        let rom = zeroed_rom();
        let sources = export_localization_tables(&rom).expect("zeroed ROM exports");
        assert_eq!(sources.len(), 3);
        for (source, region) in sources.iter().zip(REGIONS.iter()) {
            let built = build_localization_tables(source).expect("rebuilds");
            assert_eq!(built.len(), (region.end - region.start) as usize);
            assert!(built.iter().all(|byte| *byte == 0));
        }
    }

    #[test]
    fn exported_records_keep_the_typescript_field_order() {
        let rom = zeroed_rom();
        let sources = export_localization_tables(&rom).unwrap();
        assert_eq!(
            sources[0].own_keys(),
            ["format", "kind", "address", "size", "segments"]
        );
        let Value::Arr(segments) = sources[0].get("segments").unwrap() else {
            panic!("segments is an array");
        };
        assert_eq!(
            segments[0].own_keys(),
            ["address", "end", "stride", "consumers", "element", "values"],
            "stride precedes element because the TypeScript spreads `common` first"
        );
        // The ascii-fixed spelling re-states `stride`, which must NOT move it.
        let Value::Arr(third) = sources[2].get("segments").unwrap() else {
            panic!("segments is an array");
        };
        let ascii = third
            .iter()
            .find(|segment| segment.get("element").and_then(Value::as_str) == Some("ascii-fixed"))
            .expect("region 3 has ascii-fixed segments");
        assert_eq!(
            ascii.own_keys(),
            ["address", "end", "stride", "consumers", "element", "text"]
        );
    }

    #[test]
    fn signed_and_wide_elements_decode_little_endian() {
        let mut rom = zeroed_rom();
        // 0x08033eb0 is the s8/stride-2 segment; 0x08037230 is u32/stride-4.
        let at = (0x0803_3eb0 - ROM_BASE) as usize;
        rom[at] = 0xff;
        let at32 = (0x0803_7230 - ROM_BASE) as usize;
        rom[at32..at32 + 4].copy_from_slice(&[0x78, 0x56, 0x34, 0x12]);
        let sources = export_localization_tables(&rom).unwrap();
        let Value::Arr(segments) = sources[0].get("segments").unwrap() else {
            panic!()
        };
        let Value::Arr(values) = segments[1].get("values").unwrap() else {
            panic!()
        };
        assert_eq!(values[0].as_num(), Some(-1.0), "s8 sign-extends");
        let Value::Arr(segments) = sources[2].get("segments").unwrap() else {
            panic!()
        };
        let u32_segment = segments
            .iter()
            .find(|segment| segment.get("address").and_then(Value::as_str) == Some("0x08037230"))
            .unwrap();
        let Value::Arr(values) = u32_segment.get("values").unwrap() else {
            panic!()
        };
        assert_eq!(values[0].as_num(), Some(0x1234_5678 as f64));
    }

    #[test]
    fn ascii_text_is_masked_to_seven_bits_like_buffer_tostring() {
        let mut rom = zeroed_rom();
        // 0x08037300 is an 8-byte ascii-fixed segment.
        let at = (0x0803_7300 - ROM_BASE) as usize;
        rom[at..at + 3].copy_from_slice(&[b'H', b'i', 0xc1]);
        let layout = REGIONS[2]
            .segments
            .iter()
            .find(|segment| segment.start == 0x0803_7300)
            .unwrap();
        let record = export_segment(&rom, layout).expect("a high byte still decodes");
        assert_eq!(
            record.get("text").and_then(Value::as_str),
            Some("HiA"),
            "Buffer#toString(\"ascii\") masks 0xc1 down to 'A'"
        );
        // Masking is lossy, so the round trip that guards the real export path
        // must refuse this ROM rather than write a record that rebuilds wrong.
        assert_eq!(
            export_localization_tables(&rom).unwrap_err(),
            "exported table region 0x080370d4 differs"
        );
    }

    #[test]
    fn text_without_a_terminator_is_refused() {
        let mut rom = zeroed_rom();
        let at = (0x0803_7300 - ROM_BASE) as usize;
        rom[at..at + 8].copy_from_slice(b"12345678");
        assert_eq!(
            export_localization_tables(&rom).unwrap_err(),
            "fixed text padding differs"
        );
    }

    #[test]
    fn trailing_bytes_after_the_terminator_are_refused() {
        let mut rom = zeroed_rom();
        let at = (0x0803_7300 - ROM_BASE) as usize;
        rom[at] = b'A';
        rom[at + 3] = b'B';
        assert_eq!(
            export_localization_tables(&rom).unwrap_err(),
            "fixed text padding differs"
        );
    }

    #[test]
    fn build_refuses_a_wrong_shape_before_it_reads_any_value() {
        assert_eq!(
            build_localization_tables(&Value::Num(1.0)).unwrap_err(),
            "localization-table source must be an object"
        );
        assert_eq!(
            build_localization_tables(&Value::Null).unwrap_err(),
            "localization-table source must be an object"
        );
        // An array is `typeof "object"`, so it reaches exactKeys.
        assert_eq!(
            build_localization_tables(&Value::Arr(vec![])).unwrap_err(),
            "localization-table source has unknown fields"
        );
        let mut source = zero_source(&REGIONS[0]);
        if let Value::Obj(pairs) = &mut source {
            pairs.push(("extra".to_string(), Value::Num(0.0)));
        }
        assert_eq!(
            build_localization_tables(&source).unwrap_err(),
            "localization-table source has unknown fields"
        );
    }

    #[test]
    fn build_refuses_an_unknown_region_and_a_wrong_size() {
        let mut source = zero_source(&REGIONS[0]);
        if let Value::Obj(pairs) = &mut source {
            for (key, value) in pairs.iter_mut() {
                if key == "size" {
                    *value = Value::Str("0x00000099".to_string());
                }
            }
        }
        assert_eq!(
            build_localization_tables(&source).unwrap_err(),
            "localization-table region differs"
        );
    }

    #[test]
    fn build_refuses_out_of_range_and_non_integer_values() {
        let mut source = zero_source(&REGIONS[0]);
        let set_first = |source: &mut Value, value: Value| {
            let Value::Obj(pairs) = source else { panic!() };
            for (key, slot) in pairs.iter_mut() {
                if key == "segments" {
                    let Value::Arr(segments) = slot else { panic!() };
                    let Value::Obj(fields) = &mut segments[0] else {
                        panic!()
                    };
                    for (name, field) in fields.iter_mut() {
                        if name == "values" {
                            let Value::Arr(values) = field else { panic!() };
                            values[0] = value.clone();
                        }
                    }
                }
            }
        };
        set_first(&mut source, Value::Num(256.0));
        assert_eq!(
            build_localization_tables(&source).unwrap_err(),
            "u8 table value is outside its range"
        );
        set_first(&mut source, Value::Num(1.5));
        assert_eq!(
            build_localization_tables(&source).unwrap_err(),
            "table value must be an integer"
        );
        // One number type: 1.0 is an integer and must be accepted.
        set_first(&mut source, Value::Num(1.0));
        assert!(build_localization_tables(&source).is_ok());
    }

    #[test]
    fn build_refuses_a_changed_consumer_list() {
        let mut source = zero_source(&REGIONS[0]);
        let Value::Obj(pairs) = &mut source else {
            panic!()
        };
        for (key, slot) in pairs.iter_mut() {
            if key == "segments" {
                let Value::Arr(segments) = slot else { panic!() };
                let Value::Obj(fields) = &mut segments[0] else {
                    panic!()
                };
                for (name, field) in fields.iter_mut() {
                    if name == "consumers" {
                        *field = Value::Arr(vec![Value::Str("Func_deadbeef".to_string())]);
                    }
                }
            }
        }
        assert_eq!(
            build_localization_tables(&source).unwrap_err(),
            "localization-table segment 0 differs"
        );
    }

    #[test]
    fn text_longer_than_its_slot_is_refused() {
        let region = &REGIONS[2];
        let mut source = zero_source(region);
        let Value::Obj(pairs) = &mut source else {
            panic!()
        };
        for (key, slot) in pairs.iter_mut() {
            if key == "segments" {
                let Value::Arr(segments) = slot else { panic!() };
                for segment in segments.iter_mut() {
                    if segment.get("element").and_then(Value::as_str) == Some("ascii-fixed") {
                        let Value::Obj(fields) = segment else {
                            panic!()
                        };
                        for (name, field) in fields.iter_mut() {
                            if name == "text" {
                                *field = Value::Str("0123456789".to_string());
                            }
                        }
                        break;
                    }
                }
            }
        }
        assert_eq!(
            build_localization_tables(&source).unwrap_err(),
            "fixed text lacks room for its terminator"
        );
    }

    #[test]
    fn a_short_rom_is_refused_rather_than_panicking() {
        assert_eq!(
            export_localization_tables(&[0u8; 16]).unwrap_err(),
            "ROM range differs"
        );
    }

    #[test]
    fn the_self_test_covers_every_region() {
        self_test().expect("self-test passes");
    }

    #[test]
    fn source_names_come_from_the_address_field() {
        let rom = zeroed_rom();
        let sources = export_localization_tables(&rom).unwrap();
        let names: Vec<String> = sources.iter().map(source_name).collect();
        assert_eq!(
            names,
            [
                "table_08033e60.json",
                "table_080366f8.json",
                "table_080370d4.json"
            ]
        );
    }

    #[test]
    fn the_region_layout_tiles_each_region_without_a_gap() {
        for region in REGIONS.iter() {
            let mut cursor = region.start;
            for segment in region.segments {
                assert_eq!(segment.start, cursor, "segments must be contiguous");
                assert!(segment.end > segment.start);
                cursor = segment.end;
            }
            assert_eq!(cursor, region.end);
        }
    }
}
