pub mod cli;

use serde_json::{Map, Value};

pub type Result<T> = std::result::Result<T, String>;

pub const BRIGHTNESS_CURVE_ADDRESS: usize = 0x080c_5c10;
pub const BRIGHTNESS_CURVE_SIZE: usize = 0x20;
pub const ENCOUNTER_TABLE_ADDRESS: usize = 0x080c_5c38;
pub const ENCOUNTER_TABLE_SIZE: usize = 0x1d48;
pub const ALIGNMENT_ADDRESS: usize = 0x080c_7980;
pub const ALIGNMENT_SIZE: usize = 0x1680;
pub const ALIGNMENT_END: usize = 0x080c_9000;
pub const FORMATION_COUNT: usize = 380;
pub const PRELOAD_COUNT: usize = 20;
pub const METADATA_COUNT: usize = 172;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct EncounterRegion {
    pub address: usize,
    pub size: usize,
    pub source: &'static str,
    pub data: Vec<u8>,
}

fn is_primitive(value: &Value) -> bool {
    !matches!(value, Value::Array(_) | Value::Object(_))
}

pub fn integer(value: &Value, minimum: i64, maximum: i64, name: &str) -> Result<i64> {
    let number = match value {
        Value::Number(number) => number.as_f64(),
        _ => None,
    };
    let Some(number) = number else {
        return Err(format!("invalid {name}"));
    };
    if number.fract() != 0.0 || number.abs() > 9_007_199_254_740_991.0 {
        return Err(format!("invalid {name}"));
    }
    let number = number as i64;
    if number < minimum || number > maximum {
        return Err(format!("invalid {name}"));
    }
    Ok(number)
}

pub fn js_number(value: &Value) -> f64 {
    match value {
        Value::Null => 0.0,
        Value::Bool(flag) => {
            if *flag {
                1.0
            } else {
                0.0
            }
        }
        Value::Number(number) => number.as_f64().unwrap_or(f64::NAN),
        Value::String(text) => js_string_to_number(text),
        Value::Array(items) => match items.as_slice() {
            [] => 0.0,
            [only] if is_primitive(only) => js_number(only),
            _ => f64::NAN,
        },
        Value::Object(_) => f64::NAN,
    }
}

fn js_string_to_number(text: &str) -> f64 {
    let trimmed = text.trim_matches(|c: char| {
        c == '\u{feff}'
            || c == '\u{0009}'
            || c == '\u{000a}'
            || c == '\u{000b}'
            || c == '\u{000c}'
            || c == '\u{000d}'
            || c == '\u{0020}'
            || c == '\u{00a0}'
            || c == '\u{1680}'
            || ('\u{2000}'..='\u{200a}').contains(&c)
            || c == '\u{2028}'
            || c == '\u{2029}'
            || c == '\u{202f}'
            || c == '\u{205f}'
            || c == '\u{3000}'
    });
    if trimmed.is_empty() {
        return 0.0;
    }
    let radix = |prefix: &str, radix: u32| -> Option<f64> {
        let digits = trimmed.strip_prefix(prefix)?;
        if digits.is_empty() || !digits.chars().all(|c| c.is_digit(radix)) {
            return Some(f64::NAN);
        }
        let mut total = 0.0_f64;
        for digit in digits.chars() {
            total = total * f64::from(radix) + f64::from(digit.to_digit(radix).unwrap_or(0));
        }
        Some(total)
    };
    for (prefix, base) in [
        ("0x", 16),
        ("0X", 16),
        ("0o", 8),
        ("0O", 8),
        ("0b", 2),
        ("0B", 2),
    ] {
        if let Some(parsed) = radix(prefix, base) {
            return parsed;
        }
    }
    match trimmed {
        "Infinity" | "+Infinity" => return f64::INFINITY,
        "-Infinity" => return f64::NEG_INFINITY,
        _ => {}
    }
    if trimmed
        .chars()
        .any(|c| !matches!(c, '0'..='9' | '+' | '-' | '.' | 'e' | 'E'))
    {
        return f64::NAN;
    }
    trimmed.parse::<f64>().unwrap_or(f64::NAN)
}

fn document(path: &str) -> Result<Map<String, Value>> {
    let bytes = std::fs::read(path).map_err(|e| format!("{path}: {e}"))?;
    let text = String::from_utf8_lossy(&bytes);
    let parsed: Value =
        serde_json::from_str(&text).map_err(|e| format!("{path}: invalid JSON: {e}"))?;
    let Value::Object(parsed) = parsed else {
        return Err(format!("{path}: source must be an object"));
    };
    if parsed.get("format").map(js_strict_one) != Some(true) {
        return Err(format!("{path}: unsupported source format"));
    }
    Ok(parsed)
}

fn js_strict_one(value: &Value) -> bool {
    matches!(value, Value::Number(number) if number.as_f64() == Some(1.0))
}

fn js_strict_int(value: Option<&Value>, expected: f64) -> bool {
    matches!(value, Some(Value::Number(number)) if number.as_f64() == Some(expected))
}

fn exact_keys(value: &Map<String, Value>, keys: &[&str], name: &str) -> Result<()> {
    let mut actual: Vec<&str> = value.keys().map(String::as_str).collect();
    actual.sort_unstable();
    let mut expected: Vec<&str> = keys.to_vec();
    expected.sort_unstable();
    if actual != expected {
        return Err(format!("{name} has unknown fields"));
    }
    Ok(())
}

fn checked_extent(
    source: &Map<String, Value>,
    address: usize,
    size: usize,
    name: &str,
) -> Result<()> {
    let actual_address = source.get("address").map_or(f64::NAN, js_number);
    let actual_size = source.get("size").map_or(f64::NAN, js_number);
    if actual_address != address as f64 || actual_size != size as f64 {
        return Err(format!("{name} extent differs"));
    }
    Ok(())
}

fn flag_byte(value: Option<&Value>, name: &str) -> Result<u8> {
    let Some(Value::Object(source)) = value else {
        return Err(format!("{name} is not an object"));
    };
    exact_keys(source, &["bit0", "bits_1_4", "bits_5_7"], name)?;
    let Some(Value::Bool(bit0)) = source.get("bit0") else {
        return Err(format!("{name} bit0 is not boolean"));
    };
    let middle = integer(
        source.get("bits_1_4").unwrap_or(&Value::Null),
        0,
        0x0f,
        &format!("{name} bits 1..4"),
    )?;
    let high = integer(
        source.get("bits_5_7").unwrap_or(&Value::Null),
        0,
        0x07,
        &format!("{name} bits 5..7"),
    )?;
    Ok((u8::from(*bit0)) | (middle as u8) << 1 | (high as u8) << 5)
}

pub fn build_brightness_curve(path: &str) -> Result<Vec<u8>> {
    let source = document(path)?;
    exact_keys(
        &source,
        &["format", "address", "size", "phases"],
        "brightness curve",
    )?;
    checked_extent(
        &source,
        BRIGHTNESS_CURVE_ADDRESS,
        BRIGHTNESS_CURVE_SIZE,
        "brightness curve",
    )?;
    let phases = match source.get("phases") {
        Some(Value::Array(phases))
            if phases.len() == 2
                && phases
                    .iter()
                    .all(|phase| matches!(phase, Value::Array(steps) if steps.len() == 16)) =>
        {
            phases
        }
        _ => return Err("brightness curve requires two sixteen-step phases".to_string()),
    };
    let mut output = Vec::with_capacity(32);
    for phase in phases {
        let Value::Array(steps) = phase else {
            unreachable!("shape checked above");
        };
        for step in steps {
            let value = integer(step, -128, 127, "brightness coefficient")?;
            output.push((value & 0xff) as u8);
        }
    }
    Ok(output)
}

fn slot_triple(slot: &Value, name: &str) -> Result<(i64, i64, i64)> {
    let Value::Array(items) = slot else {
        return Err(format!("{name} slot is not iterable"));
    };
    let at = |index: usize| items.get(index).unwrap_or(&Value::Null);
    let member = integer(at(0), 1, 0xff, &format!("{name} member"))?;
    let minimum = integer(at(1), 0, 0xff, &format!("{name} minimum"))?;
    let maximum = integer(at(2), minimum, 0xff, &format!("{name} maximum"))?;
    Ok((member, minimum, maximum))
}

pub fn build_encounter_tables(path: &str) -> Result<Vec<u8>> {
    let source = document(path)?;
    exact_keys(
        &source,
        &[
            "format",
            "address",
            "size",
            "formation_record_size",
            "metadata_record_size",
            "formations",
            "preload_ids",
            "metadata",
        ],
        "encounter tables",
    )?;
    checked_extent(
        &source,
        ENCOUNTER_TABLE_ADDRESS,
        ENCOUNTER_TABLE_SIZE,
        "encounter tables",
    )?;
    if !js_strict_int(source.get("formation_record_size"), 16.0)
        || !js_strict_int(source.get("metadata_record_size"), 8.0)
    {
        return Err("encounter record sizes differ".to_string());
    }

    let source_formations = match source.get("formations") {
        Some(Value::Array(items)) if items.len() == FORMATION_COUNT => items,
        _ => {
            return Err(format!(
                "encounter source requires {FORMATION_COUNT} formations"
            ))
        }
    };
    let mut formations = vec![0u8; FORMATION_COUNT * 16];
    for (index, formation) in source_formations.iter().enumerate() {
        if formation.is_null() {
            continue;
        }
        let items = match formation {
            Value::Array(items) if !items.is_empty() && items.len() <= 6 => items,
            _ => return Err(format!("formation {index} has invalid slots")),
        };
        let offset = index * 16;
        formations[offset] =
            integer(&items[0], 0, 0xff, &format!("formation {index} layout"))? as u8;
        for (slot_index, slot) in items[1..].iter().enumerate() {
            let (member, minimum, maximum) = slot_triple(slot, &format!("formation {index}"))?;
            formations[offset + 1 + slot_index] = member as u8;
            formations[offset + 6 + slot_index] = minimum as u8;
            formations[offset + 11 + slot_index] = maximum as u8;
        }
    }

    let source_preload = match source.get("preload_ids") {
        Some(Value::Array(items)) if items.len() == PRELOAD_COUNT => items,
        _ => {
            return Err(format!(
                "encounter source requires {PRELOAD_COUNT} preload IDs"
            ))
        }
    };
    let mut preload = vec![0u8; PRELOAD_COUNT * 2];
    for (index, value) in source_preload.iter().enumerate() {
        let id = integer(value, 0, 0xffff, &format!("preload ID {index}"))? as u16;
        preload[index * 2..index * 2 + 2].copy_from_slice(&id.to_le_bytes());
    }

    let source_metadata = match source.get("metadata") {
        Some(Value::Array(items)) if items.len() == METADATA_COUNT => items,
        _ => {
            return Err(format!(
                "encounter source requires {METADATA_COUNT} metadata records"
            ))
        }
    };
    let mut metadata = vec![0u8; METADATA_COUNT * 8];
    for (index, entry) in source_metadata.iter().enumerate() {
        let Value::Array(items) = entry else {
            return Err(format!("metadata {index} is not iterable"));
        };
        let at = |slot: usize| items.get(slot);
        let offset = index * 8;
        let value = integer(
            at(0).unwrap_or(&Value::Null),
            0,
            0xffff,
            &format!("metadata {index} value"),
        )? as u16;
        metadata[offset..offset + 2].copy_from_slice(&value.to_le_bytes());
        metadata[offset + 2] = flag_byte(at(1), &format!("metadata {index} flags 0"))?;
        metadata[offset + 3] = flag_byte(at(2), &format!("metadata {index} flags 1"))?;
        metadata[offset + 4] = integer(
            at(3).unwrap_or(&Value::Null),
            0,
            0xff,
            &format!("metadata {index} attribute 4"),
        )? as u8;
    }

    let mut output = formations;
    output.extend_from_slice(&preload);
    output.extend_from_slice(&metadata);
    if output.len() != ENCOUNTER_TABLE_SIZE {
        return Err("encounter table size differs".to_string());
    }
    Ok(output)
}

pub fn build_alignment(path: &str) -> Result<Vec<u8>> {
    let source = document(path)?;
    exact_keys(
        &source,
        &["format", "address", "size", "end", "fill"],
        "encounter alignment",
    )?;
    checked_extent(
        &source,
        ALIGNMENT_ADDRESS,
        ALIGNMENT_SIZE,
        "encounter alignment",
    )?;
    if source.get("end").map_or(f64::NAN, js_number) != ALIGNMENT_END as f64
        || !js_strict_int(source.get("fill"), 0.0)
    {
        return Err("encounter alignment boundary differs".to_string());
    }
    Ok(vec![0u8; ALIGNMENT_SIZE])
}

pub fn build_encounter_regions(directory: &str) -> Result<Vec<EncounterRegion>> {
    type Builder = fn(&str) -> Result<Vec<u8>>;
    let sources: [(usize, usize, &'static str, Builder); 3] = [
        (
            BRIGHTNESS_CURVE_ADDRESS,
            BRIGHTNESS_CURVE_SIZE,
            "brightness_curve.json",
            build_brightness_curve,
        ),
        (
            ENCOUNTER_TABLE_ADDRESS,
            ENCOUNTER_TABLE_SIZE,
            "encounter_tables.json",
            build_encounter_tables,
        ),
        (
            ALIGNMENT_ADDRESS,
            ALIGNMENT_SIZE,
            "alignment.json",
            build_alignment,
        ),
    ];
    let mut regions = Vec::with_capacity(3);
    for (address, size, source, builder) in sources {
        let data = builder(&format!("{directory}_{source}"))?;
        if data.len() != size {
            return Err(format!("{source}: built size differs"));
        }
        regions.push(EncounterRegion {
            address,
            size,
            source,
            data,
        });
    }
    Ok(regions)
}
