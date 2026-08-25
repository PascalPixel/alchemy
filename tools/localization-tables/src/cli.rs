use std::io::Write;

use serde_json::Value;

#[derive(Clone, Copy, PartialEq, Eq)]
enum Element {
    U8,
    S8,
    U16,
    U32,
    Ascii,
}

impl Element {
    fn name(self) -> &'static str {
        match self {
            Self::U8 => "u8",
            Self::S8 => "s8",
            Self::U16 => "u16",
            Self::U32 => "u32",
            Self::Ascii => "ascii-fixed",
        }
    }
    fn width(self) -> usize {
        match self {
            Self::U8 | Self::S8 | Self::Ascii => 1,
            Self::U16 => 2,
            Self::U32 => 4,
        }
    }
}

struct Segment {
    start: u32,
    end: u32,
    element: Element,
    stride: u32,
    consumers: &'static [&'static str],
}

const fn segment(start: u32, end: u32, element: Element, stride: u32, consumers: &'static [&'static str]) -> Segment {
    Segment { start, end, element, stride, consumers }
}

struct Region {
    start: u32,
    end: u32,
    segments: &'static [Segment],
}

use Element::{Ascii, S8, U16, U32, U8};

const REGION_0: [Segment; 4] = [
    segment(0x0803_3e60, 0x0803_3eb0, U8, 1, &["Func_080191cc"]),
    segment(0x0803_3eb0, 0x0803_3ed8, S8, 2, &["Func_080191cc"]),
    segment(0x0803_3ed8, 0x0803_3ee8, U8, 1, &[]),
    segment(0x0803_3ee8, 0x0803_3ef8, S8, 1, &["Func_080191cc"]),
];

const REGION_1: [Segment; 13] = [
    segment(0x0803_66f8, 0x0803_6738, U16, 2, &["Func_0801908c", "Func_08021e6c", "Func_08028194"]),
    segment(0x0803_6738, 0x0803_6740, S8, 1, &["Func_0801908c"]),
    segment(0x0803_6740, 0x0803_6750, S8, 1, &["Func_0801a98c"]),
    segment(0x0803_6750, 0x0803_67b0, U8, 1, &["Func_0801ca1c", "Func_0801ccc0"]),
    segment(0x0803_67b0, 0x0803_67c9, U8, 1, &[]),
    segment(0x0803_67c9, 0x0803_67cc, S8, 1, &["Func_0801d108", "Func_0801d4cc"]),
    segment(0x0803_67cc, 0x0803_67ce, S8, 1, &["Func_0801d108", "Func_0801d4cc"]),
    segment(0x0803_67ce, 0x0803_67d0, S8, 1, &["Func_0801d108", "Func_0801d4cc"]),
    segment(0x0803_67d0, 0x0803_67d6, S8, 1, &["Func_0801d9d4"]),
    segment(0x0803_67d6, 0x0803_67dc, S8, 1, &["Func_0801d9d4"]),
    segment(0x0803_67dc, 0x0803_67e4, S8, 1, &["Func_0801d9d4"]),
    segment(0x0803_67e4, 0x0803_680c, U16, 2, &["Func_08019d2c", "Func_0802977c"]),
    segment(0x0803_680c, 0x0803_68d4, U16, 4, &["Func_08019d2c", "Func_0802977c"]),
];

const REGION_2: [Segment; 35] = [
    segment(0x0803_70d4, 0x0803_71b4, U8, 1, &["Func_0801de5c"]),
    segment(0x0803_71b4, 0x0803_71c4, U8, 1, &["Func_0801de5c"]),
    segment(0x0803_71c4, 0x0803_71e0, U8, 1, &["Func_0801ef68"]),
    segment(0x0803_71e0, 0x0803_71f6, U8, 1, &["Func_08020198"]),
    segment(0x0803_71f6, 0x0803_71fe, S8, 1, &["Func_08020bd8"]),
    segment(0x0803_71fe, 0x0803_7206, U16, 2, &["Func_08021228"]),
    segment(0x0803_7206, 0x0803_7216, U16, 2, &["Func_08021360"]),
    segment(0x0803_7216, 0x0803_7226, U16, 2, &["Func_08021360"]),
    segment(0x0803_7226, 0x0803_7230, S8, 1, &["Func_080216b4"]),
    segment(0x0803_7230, 0x0803_7250, U32, 4, &["Func_080217a4"]),
    segment(0x0803_7250, 0x0803_7280, U8, 8, &["Func_08021848"]),
    segment(0x0803_7280, 0x0803_72c0, U8, 4, &["Func_080219c8"]),
    segment(0x0803_72c0, 0x0803_7300, U16, 2, &["Func_08021a18"]),
    segment(0x0803_7300, 0x0803_7308, Ascii, 1, &["Func_08021c34"]),
    segment(0x0803_7308, 0x0803_7328, U8, 1, &["Func_08023e70"]),
    segment(0x0803_7328, 0x0803_73a8, U8, 8, &["Func_08023178"]),
    segment(0x0803_73a8, 0x0803_73b8, U8, 4, &["Func_08023178"]),
    segment(0x0803_73b8, 0x0803_73d8, U8, 4, &["Func_08023178"]),
    segment(0x0803_73d8, 0x0803_73dc, Ascii, 1, &["Func_08023178"]),
    segment(0x0803_73dc, 0x0803_73e0, Ascii, 1, &["Func_08023178"]),
    segment(0x0803_73e0, 0x0803_73e4, Ascii, 1, &["Func_08023178"]),
    segment(0x0803_73e4, 0x0803_73e7, Ascii, 1, &["Func_08023178"]),
    segment(0x0803_73e7, 0x0803_73eb, U8, 2, &["Func_08023e70"]),
    segment(0x0803_73eb, 0x0803_73ef, U8, 2, &["Func_08023e70"]),
    segment(0x0803_73ef, 0x0803_73f7, U8, 1, &["Func_080286a0"]),
    segment(0x0803_73f7, 0x0803_7403, U8, 1, &["Func_08028920"]),
    segment(0x0803_7403, 0x0803_740f, U8, 1, &["Func_08028920"]),
    segment(0x0803_740f, 0x0803_7428, U8, 1, &["Func_080289e8"]),
    segment(0x0803_7428, 0x0803_742c, Ascii, 1, &["Func_08028ef0", "Func_080292c4"]),
    segment(0x0803_742c, 0x0803_7440, Ascii, 1, &["Func_080292c4"]),
    segment(0x0803_7440, 0x0803_7448, Ascii, 1, &["Func_08029554"]),
    segment(0x0803_7448, 0x0803_7450, Ascii, 1, &["Func_08029554"]),
    segment(0x0803_7450, 0x0803_7458, Ascii, 1, &["Func_08029554"]),
    segment(0x0803_7458, 0x0803_7460, Ascii, 1, &["Func_08029554"]),
    segment(0x0803_7460, 0x0803_7464, Ascii, 1, &["Func_08029554"]),
];

const REGIONS: [Region; 3] = [
    Region { start: 0x0803_3e60, end: 0x0803_3ef8, segments: &REGION_0 },
    Region { start: 0x0803_66f8, end: 0x0803_68d4, segments: &REGION_1 },
    Region { start: 0x0803_70d4, end: 0x0803_7464, segments: &REGION_2 },
];

fn hex(value: u32) -> String {
    format!("0x{value:08x}")
}

fn object<'a>(value: &'a Value, label: &str) -> Result<&'a serde_json::Map<String, Value>, String> {
    value.as_object().ok_or_else(|| format!("{label} must be an object"))
}

fn exact_keys(value: &serde_json::Map<String, Value>, keys: &[&str], label: &str) -> Result<(), String> {
    let mut actual: Vec<&str> = value.keys().map(String::as_str).collect();
    let mut expected = keys.to_vec();
    actual.sort_unstable();
    expected.sort_unstable();
    if actual != expected {
        return Err(format!("{label} has unknown fields"));
    }
    Ok(())
}

fn number(value: Option<&Value>, label: &str) -> Result<f64, String> {
    value
        .and_then(Value::as_f64)
        .filter(|n| n.is_finite() && n.fract() == 0.0)
        .ok_or_else(|| format!("{label} must be an integer"))
}

fn array<'a>(value: &'a serde_json::Map<String, Value>, name: &str, label: &str) -> Result<&'a [Value], String> {
    value.get(name).and_then(Value::as_array).map(Vec::as_slice).ok_or_else(|| format!("{label} differs"))
}

fn numeric(source: &serde_json::Map<String, Value>, element: Element, size: usize) -> Result<Vec<u8>, String> {
    let stride = number(source.get("stride"), "table stride")?;
    let values = array(source, "values", "numeric table")?;
    let width = element.width() as f64;
    if stride < width
        || stride % width != 0.0
        || size as f64 % stride != 0.0
        || values.len() as f64 != size as f64 / width
    {
        return Err("numeric table layout differs".into());
    }
    let mut output = vec![0; size];
    for (index, raw) in values.iter().enumerate() {
        let value = number(Some(raw), "table value")?;
        match element {
            Element::U8 if (0.0..=255.0).contains(&value) => output[index] = value as u8,
            Element::S8 if (-128.0..=127.0).contains(&value) => output[index] = (value as i8) as u8,
            Element::U16 if (0.0..=65535.0).contains(&value) => {
                output[index * 2..index * 2 + 2].copy_from_slice(&(value as u16).to_le_bytes())
            }
            Element::U32 if (0.0..=4_294_967_295.0).contains(&value) => {
                output[index * 4..index * 4 + 4].copy_from_slice(&(value as u32).to_le_bytes())
            }
            Element::U8 => return Err("u8 table value is outside its range".into()),
            Element::S8 => return Err("s8 table value is outside its range".into()),
            Element::U16 => return Err("u16 table value is outside its range".into()),
            Element::U32 => return Err("u32 table value is outside its range".into()),
            Element::Ascii => unreachable!(),
        }
    }
    Ok(output)
}

fn text(source: &serde_json::Map<String, Value>, size: usize) -> Result<Vec<u8>, String> {
    if number(source.get("stride"), "text stride")? != 1.0 {
        return Err("fixed text differs".into());
    }
    let value = source.get("text").and_then(Value::as_str).unwrap_or("");
    if !value.bytes().all(|byte| (0x20..=0x7e).contains(&byte)) || value.len() >= size {
        return Err("fixed text differs".into());
    }
    let mut output = vec![0; size];
    output[..value.len()].copy_from_slice(value.as_bytes());
    Ok(output)
}

fn build(value: &Value) -> Result<Vec<u8>, String> {
    let source = object(value, "localization-table source")?;
    exact_keys(source, &["format", "kind", "address", "size", "segments"], "localization-table source")?;
    if number(source.get("format"), "format")? != 1.0
        || source.get("kind").and_then(Value::as_str) != Some("golden-sun-localization-tables")
    {
        return Err("localization-table source differs".into());
    }
    let address = source.get("address").and_then(Value::as_str).unwrap_or("");
    let size = source.get("size").and_then(Value::as_str).unwrap_or("");
    let region = REGIONS
        .iter()
        .find(|region| address == hex(region.start) && size == hex(region.end - region.start))
        .ok_or_else(|| "localization-table region differs".to_string())?;
    let segments = source
        .get("segments")
        .and_then(Value::as_array)
        .ok_or_else(|| "localization-table source differs".to_string())?;
    if segments.len() != region.segments.len() {
        return Err("localization-table segment count differs".into());
    }
    let mut result = Vec::new();
    for (index, value) in segments.iter().enumerate() {
        let expected = &region.segments[index];
        let segment = object(value, &format!("localization-table segment {index}"))?;
        let keys = if segment.get("element").and_then(Value::as_str) == Some("ascii-fixed") {
            &["address", "end", "element", "stride", "consumers", "text"][..]
        } else {
            &["address", "end", "element", "stride", "consumers", "values"][..]
        };
        exact_keys(segment, keys, &format!("localization-table segment {index}"))?;
        let consumers = Value::Array(expected.consumers.iter().map(|name| Value::String((*name).into())).collect());
        if segment.get("address").and_then(Value::as_str) != Some(hex(expected.start).as_str())
            || segment.get("end").and_then(Value::as_str) != Some(hex(expected.end).as_str())
            || segment.get("element").and_then(Value::as_str) != Some(expected.element.name())
            || number(segment.get("stride"), "segment stride")? != f64::from(expected.stride)
            || serde_json::to_string(segment.get("consumers").unwrap()).unwrap()
                != serde_json::to_string(&consumers).unwrap()
        {
            return Err(format!("localization-table segment {index} differs"));
        }
        let bytes = if expected.element == Element::Ascii {
            text(segment, (expected.end - expected.start) as usize)?
        } else {
            numeric(segment, expected.element, (expected.end - expected.start) as usize)?
        };
        result.extend(bytes);
    }
    if result.len() != (region.end - region.start) as usize {
        return Err("localization-table extent differs".into());
    }
    Ok(result)
}

pub fn entry(arguments: &[String]) {
    let result = match arguments {
        [command, source] if command == "build-stdout" => (|| {
            let bytes = std::fs::read(source).map_err(|error| format!("{source}: {error}"))?;
            let value: Value = serde_json::from_slice(&bytes).map_err(|error| error.to_string())?;
            std::io::stdout().write_all(&build(&value)?).map_err(|error| error.to_string())
        })(),
        [argument] if matches!(argument.as_str(), "-h" | "--help") => {
            println!("usage: localization-tables build-stdout SOURCE");
            Ok(())
        }
        _ => Err("usage: localization-tables build-stdout SOURCE".into()),
    };
    if let Err(error) = result {
        eprintln!("error: {error}");
        std::process::exit(1);
    }
}
