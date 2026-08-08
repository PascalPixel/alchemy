// Golden Sun's encounter data lives in three fixed ROM extents: a 32-byte
// brightness curve, the 0x1d48-byte encounter table (formations, preload IDs,
// per-encounter metadata), and a 0x1680-byte zero-fill run that pads the block
// out to 0x080c9000. This file exists so that block can be held in the tree as
// reviewable JSON rather than as an opaque byte blob: `build_*` rebuilds each
// extent from JSON and the byte-for-byte comparison against the ROM is what
// proves the JSON is a faithful description and not a plausible-looking guess.
// build_assets.ts calls build_encounter_regions for the same reason.
//
// Ported from tools/make/encounter_data.ts. The JSON here is a validation
// boundary: every field is proven, never assumed, so the parse helpers take
// `&Value` and reject anything they have not checked.

use std::path::Path;

use serde_json::{Map, Value};

pub type Result<T> = std::result::Result<T, String>;

const ROM_BASE: usize = 0x0800_0000;
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

// ---------------------------------------------------------------------------
// canonical JSON
// ---------------------------------------------------------------------------
// PORT NOTE: tools/lib/canonical_json.ts is imported by the TypeScript
// original, but tools-rs/canonical-json inherits `version.workspace`, so a path
// dependency on it would drag this crate into tools-rs/Cargo.toml and stop it
// building on its own. The 20 lines are reproduced here instead; the tests pin
// the two shapes this file actually emits.

fn is_primitive(value: &Value) -> bool {
    !matches!(value, Value::Array(_) | Value::Object(_))
}

fn encode_scalar(value: &Value) -> String {
    serde_json::to_string(value).expect("a scalar always serializes")
}

fn reflow(value: &Value, indent: &str) -> String {
    if is_primitive(value) {
        return encode_scalar(value);
    }
    let inner = format!("{indent}  ");
    match value {
        Value::Array(items) => {
            if items.is_empty() {
                return "[]".to_string();
            }
            if items.iter().all(is_primitive) {
                let parts: Vec<String> = items.iter().map(encode_scalar).collect();
                return format!("[{}]", parts.join(", "));
            }
            let parts: Vec<String> = items
                .iter()
                .map(|item| format!("{inner}{}", reflow(item, &inner)))
                .collect();
            format!("[\n{}\n{indent}]", parts.join(",\n"))
        }
        Value::Object(record) => {
            if record.is_empty() {
                return "{}".to_string();
            }
            let parts: Vec<String> = record
                .iter()
                .map(|(key, item)| {
                    format!(
                        "{inner}{}: {}",
                        encode_scalar(&Value::String(key.clone())),
                        reflow(item, &inner)
                    )
                })
                .collect();
            format!("{{\n{}\n{indent}}}", parts.join(",\n"))
        }
        _ => unreachable!("primitives are handled above"),
    }
}

pub fn canonical_json(value: &Value) -> String {
    reflow(value, "")
}

fn pretty(value: &Value) -> String {
    format!("{}\n", canonical_json(value))
}

fn object(pairs: Vec<(&str, Value)>) -> Value {
    let mut map = Map::new();
    for (key, item) in pairs {
        map.insert(key.to_string(), item);
    }
    Value::Object(map)
}

// ---------------------------------------------------------------------------
// JavaScript value semantics
// ---------------------------------------------------------------------------

/// `Number.isSafeInteger(value)` plus the range test, on a decoded JSON value.
///
/// PORT NOTE: JSON has one number type, so `1.0` and `1` are the same value and
/// both pass here, exactly as they do under `Number.isSafeInteger`. A string
/// such as `"1"` is rejected: `typeof value !== "number"` in the original.
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

/// ECMAScript `Number(value)` over a decoded JSON value.
///
/// PORT NOTE: this is the StringNumericLiteral grammar, not `f64::from_str`:
/// `Number("0x080c5c38")` is 134634552 (the tracked files store addresses as
/// hex strings) and `Number("")` is 0, both of which `from_str` rejects. Only
/// the conversions this file can meet are modelled; objects and multi-element
/// arrays are NaN, which is what `checkedExtent` needs from them anyway.
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
    // PORT NOTE: JS trims by its own whitespace set, which includes U+FEFF and
    // excludes U+0085. Rust's `char::is_whitespace` is the reverse on both, so
    // the set is spelled out rather than borrowed.
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
        // Radix literals never carry a sign, so accumulating in f64 matches the
        // spec's "mathematical value, rounded to a Number" for oversized input.
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
    // `f64::from_str` accepts "inf"/"nan"/"1_0"-free decimals; the spec's
    // StrDecimalLiteral does not accept the alphabetic spellings, so they are
    // filtered out before parsing.
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
    // PORT NOTE: `readFileSync(path, "utf8")` replaces malformed sequences
    // rather than failing, so this is `from_utf8_lossy`, not `from_utf8`.
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

/// PORT NOTE: the original sorts both key lists and compares element-wise. JS
/// sorts strings by UTF-16 code unit and Rust by byte, but the two orders only
/// disagree above U+FFFF, and even there the comparison is an equality test
/// between two lists sorted by the *same* comparator, so the verdict is
/// identical for any input. A byte sort is used.
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

fn hexadecimal(value: usize) -> String {
    format!("0x{value:08x}")
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
        // PORT NOTE: `typeof null === "object"` in JS, but the original tests
        // `value === null` separately, so null lands on the same message.
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

fn split_flags(value: u8) -> Value {
    object(vec![
        ("bit0", Value::Bool(value & 1 != 0)),
        ("bits_1_4", Value::from(value >> 1 & 0x0f)),
        ("bits_5_7", Value::from(value >> 5)),
    ])
}

// ---------------------------------------------------------------------------
// builders
// ---------------------------------------------------------------------------

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
    // PORT NOTE: the original array-destructures the slot. Destructuring a
    // non-array throws a TypeError instead of the domain error below; the
    // message differs, the rejection does not.
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
        formations[offset] = integer(&items[0], 0, 0xff, &format!("formation {index} layout"))? as u8;
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

/// PORT NOTE: `directory` is a *prefix*, not a directory: the original joins it
/// to each file name with an underscore (`${directory}_${source}`), so the
/// tracked sources are `assets/data/encounter_data_brightness_curve.json`.
/// `export_encounter_data` disagrees with this and writes into a directory of
/// that name; see the note there. Both behaviours are reproduced as they stand.
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

// ---------------------------------------------------------------------------
// export
// ---------------------------------------------------------------------------

/// PORT NOTE: kept faithful to a broken original. `export_encounter_data`
/// writes `<directory>/brightness_curve.json` while `build_encounter_regions`
/// reads `<directory>_brightness_curve.json`, and it emits formations as
/// `{layout, slots:[{member_id,…}]}` objects while the builder demands
/// `[layout, [member,min,max], …]` tuples. The round-trip check at the end of
/// this function therefore cannot pass in either the TypeScript or the Rust
/// version; measured, `bun … export` fails with ENOENT on the underscore path
/// and this port reproduces that. Fixing it is a behaviour change and belongs
/// in its own commit against the TypeScript first.
pub fn export_encounter_data(rom: &[u8], directory: &str) -> Result<Vec<EncounterRegion>> {
    if rom.len() < ALIGNMENT_END - ROM_BASE {
        return Err("ROM is too small for encounter data".to_string());
    }
    std::fs::create_dir_all(directory).map_err(|e| format!("{directory}: {e}"))?;

    let curve_start = BRIGHTNESS_CURVE_ADDRESS - ROM_BASE;
    let curve = &rom[curve_start..curve_start + BRIGHTNESS_CURVE_SIZE];
    let phases: Vec<Value> = (0..2)
        .map(|phase| {
            Value::Array(
                curve[phase * 16..phase * 16 + 16]
                    .iter()
                    .map(|&value| Value::from(i32::from(value as i8)))
                    .collect(),
            )
        })
        .collect();
    write(
        directory,
        "brightness_curve.json",
        &object(vec![
            ("format", Value::from(1)),
            ("address", Value::from(hexadecimal(BRIGHTNESS_CURVE_ADDRESS))),
            ("size", Value::from(BRIGHTNESS_CURVE_SIZE)),
            ("phases", Value::Array(phases)),
        ]),
    )?;

    let table_start = ENCOUNTER_TABLE_ADDRESS - ROM_BASE;
    let encounter = &rom[table_start..table_start + ENCOUNTER_TABLE_SIZE];
    let mut formations = Vec::with_capacity(FORMATION_COUNT);
    for index in 0..FORMATION_COUNT {
        let record = &encounter[index * 16..index * 16 + 16];
        if record.iter().all(|&value| value == 0) {
            formations.push(Value::Null);
            continue;
        }
        let mut slots = Vec::new();
        let mut ended = false;
        for slot in 0..5 {
            let member = record[1 + slot];
            let minimum = record[6 + slot];
            let maximum = record[11 + slot];
            if member == 0 {
                ended = true;
                if minimum != 0 || maximum != 0 {
                    return Err(format!("formation {index} has data without a member"));
                }
                continue;
            }
            if ended || minimum > maximum {
                return Err(format!("formation {index} has invalid member ordering"));
            }
            slots.push(object(vec![
                ("member_id", Value::from(member)),
                ("minimum", Value::from(minimum)),
                ("maximum", Value::from(maximum)),
            ]));
        }
        formations.push(object(vec![
            ("layout", Value::from(record[0])),
            ("slots", Value::Array(slots)),
        ]));
    }
    let preload_offset = FORMATION_COUNT * 16;
    let metadata_offset = preload_offset + PRELOAD_COUNT * 2;
    let preload_ids: Vec<Value> = (0..PRELOAD_COUNT)
        .map(|index| Value::from(read_u16_le(encounter, preload_offset + index * 2)))
        .collect();
    let mut metadata = Vec::with_capacity(METADATA_COUNT);
    for index in 0..METADATA_COUNT {
        let offset = metadata_offset + index * 8;
        if encounter[offset + 5..offset + 8].iter().any(|&v| v != 0) {
            return Err(format!("metadata {index} has nonzero reserved bytes"));
        }
        metadata.push(object(vec![
            ("value", Value::from(read_u16_le(encounter, offset))),
            ("flags_0", split_flags(encounter[offset + 2])),
            ("flags_1", split_flags(encounter[offset + 3])),
            ("attribute_4", Value::from(encounter[offset + 4])),
        ]));
    }
    write(
        directory,
        "encounter_tables.json",
        &object(vec![
            ("format", Value::from(1)),
            ("address", Value::from(hexadecimal(ENCOUNTER_TABLE_ADDRESS))),
            ("size", Value::from(ENCOUNTER_TABLE_SIZE)),
            ("formation_record_size", Value::from(16)),
            ("metadata_record_size", Value::from(8)),
            ("formations", Value::Array(formations)),
            ("preload_ids", Value::Array(preload_ids)),
            ("metadata", Value::Array(metadata)),
        ]),
    )?;

    let alignment = &rom[ALIGNMENT_ADDRESS - ROM_BASE..ALIGNMENT_END - ROM_BASE];
    if alignment.iter().any(|&value| value != 0) {
        return Err("encounter alignment is not zero-filled".to_string());
    }
    write(
        directory,
        "alignment.json",
        &object(vec![
            ("format", Value::from(1)),
            ("address", Value::from(hexadecimal(ALIGNMENT_ADDRESS))),
            ("size", Value::from(ALIGNMENT_SIZE)),
            ("end", Value::from(hexadecimal(ALIGNMENT_END))),
            ("fill", Value::from(0)),
        ]),
    )?;

    let built = build_encounter_regions(directory)?;
    for region in &built {
        if !matches_rom(rom, region) {
            return Err(format!("{}: round trip differs", region.source));
        }
    }
    Ok(built)
}

fn write(directory: &str, name: &str, value: &Value) -> Result<()> {
    let path = Path::new(directory).join(name);
    std::fs::write(&path, pretty(value)).map_err(|e| format!("{}: {e}", path.display()))
}

fn read_u16_le(bytes: &[u8], offset: usize) -> u16 {
    u16::from_le_bytes([bytes[offset], bytes[offset + 1]])
}

/// PORT NOTE: `Buffer#subarray` clamps out-of-range ends rather than throwing,
/// and `Buffer#equals` on a short slice returns false. Slicing is clamped here
/// to keep a truncated ROM a mismatch instead of a panic.
pub fn matches_rom(rom: &[u8], region: &EncounterRegion) -> bool {
    let start = region.address - ROM_BASE;
    let start = start.min(rom.len());
    let end = (start + region.size).min(rom.len());
    rom[start..end] == region.data[..]
}

// ---------------------------------------------------------------------------
// CLI helpers
// ---------------------------------------------------------------------------

pub fn option(args: &[String], name: &str) -> Result<String> {
    match args.iter().position(|arg| arg == name) {
        Some(index) if index + 1 < args.len() => Ok(args[index + 1].clone()),
        _ => Err(format!("{name} is required")),
    }
}

pub fn self_test() -> Result<()> {
    let flags = split_flags(0xad);
    if flag_byte(Some(&flags), "flags")? != 0xad {
        return Err("encounter flag self-test failed".to_string());
    }
    if integer(&Value::String("1".to_string()), 0, 1, "string value").is_ok() {
        return Err("encounter integer self-test accepted a string".to_string());
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    use serde_json::json;

    fn scratch(name: &str) -> std::path::PathBuf {
        let dir = std::env::temp_dir().join(format!("encounter-data-test-{name}"));
        let _ = std::fs::remove_dir_all(&dir);
        std::fs::create_dir_all(&dir).expect("scratch directory");
        dir
    }

    fn write_source(prefix: &std::path::Path, name: &str, value: &Value) -> String {
        let path = format!("{}_{name}", prefix.display());
        std::fs::write(&path, pretty(value)).expect("fixture written");
        path
    }

    fn curve_source() -> Value {
        json!({
            "format": 1,
            "address": "0x080c5c10",
            "size": 32,
            "phases": [
                [15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0],
                [4, 6, 7, 8, 8, 8, 8, 8, 4, 2, 1, 0, 0, 0, 0, -1],
            ],
        })
    }

    fn table_source() -> Value {
        let mut formations = vec![Value::Null; FORMATION_COUNT];
        formations[1] = json!([0, [1, 1, 1]]);
        formations[4] = json!([2, [117, 1, 3], [114, 0, 2]]);
        let preload: Vec<Value> = (0..PRELOAD_COUNT).map(|i| json!(0x1100 + i)).collect();
        let metadata: Vec<Value> = (0..METADATA_COUNT)
            .map(|i| {
                json!([
                    453 + i,
                    {"bit0": false, "bits_1_4": 1, "bits_5_7": 5},
                    {"bit0": true, "bits_1_4": 0, "bits_5_7": 0},
                    7,
                ])
            })
            .collect();
        json!({
            "format": 1,
            "address": "0x080c5c38",
            "size": 7496,
            "formation_record_size": 16,
            "metadata_record_size": 8,
            "formations": formations,
            "preload_ids": preload,
            "metadata": metadata,
        })
    }

    fn alignment_source() -> Value {
        json!({
            "format": 1,
            "address": "0x080c7980",
            "size": 5760,
            "end": "0x080c9000",
            "fill": 0,
        })
    }

    #[test]
    fn brightness_curve_encodes_signed_coefficients() {
        let dir = scratch("curve");
        let prefix = dir.join("p");
        let path = write_source(&prefix, "brightness_curve.json", &curve_source());
        let built = build_brightness_curve(&path).expect("built");
        assert_eq!(built.len(), BRIGHTNESS_CURVE_SIZE);
        assert_eq!(built[0], 15);
        // -1 must land as 0xff, not as a saturating clamp.
        assert_eq!(built[31], 0xff);
    }

    #[test]
    fn brightness_curve_rejects_out_of_range_coefficient() {
        let dir = scratch("curve-range");
        let prefix = dir.join("p");
        let mut source = curve_source();
        source["phases"][0][0] = json!(128);
        let path = write_source(&prefix, "brightness_curve.json", &source);
        assert_eq!(
            build_brightness_curve(&path).unwrap_err(),
            "invalid brightness coefficient"
        );
    }

    #[test]
    fn encounter_tables_lay_out_columns_not_rows() {
        let dir = scratch("tables");
        let prefix = dir.join("p");
        let path = write_source(&prefix, "encounter_tables.json", &table_source());
        let built = build_encounter_tables(&path).expect("built");
        assert_eq!(built.len(), ENCOUNTER_TABLE_SIZE);
        // Formation 4: layout at +0, members at +1.., minima at +6.., maxima at +11..
        let base = 4 * 16;
        assert_eq!(&built[base..base + 3], &[2, 117, 114]);
        assert_eq!(&built[base + 6..base + 8], &[1, 0]);
        assert_eq!(&built[base + 11..base + 13], &[3, 2]);
        // Preload IDs are little-endian u16 immediately after the formations.
        let preload = FORMATION_COUNT * 16;
        assert_eq!(&built[preload..preload + 2], &[0x00, 0x11]);
        // Metadata: value u16 LE, two packed flag bytes, attribute, 3 reserved.
        let metadata = preload + PRELOAD_COUNT * 2;
        assert_eq!(&built[metadata..metadata + 2], &453u16.to_le_bytes());
        assert_eq!(built[metadata + 2], 0xa2);
        assert_eq!(built[metadata + 3], 0x01);
        assert_eq!(built[metadata + 4], 7);
        assert_eq!(&built[metadata + 5..metadata + 8], &[0, 0, 0]);
    }

    #[test]
    fn regions_are_read_from_an_underscore_prefix_not_a_directory() {
        let dir = scratch("regions");
        let prefix = dir.join("encounter_data");
        write_source(&prefix, "brightness_curve.json", &curve_source());
        write_source(&prefix, "encounter_tables.json", &table_source());
        write_source(&prefix, "alignment.json", &alignment_source());
        let regions = build_encounter_regions(&prefix.display().to_string()).expect("regions");
        assert_eq!(regions.len(), 3);
        assert_eq!(
            regions.iter().map(|r| r.size).sum::<usize>(),
            BRIGHTNESS_CURVE_SIZE + ENCOUNTER_TABLE_SIZE + ALIGNMENT_SIZE
        );
        assert!(regions[2].data.iter().all(|&b| b == 0));
    }

    #[test]
    fn maximum_below_minimum_is_rejected() {
        let dir = scratch("ordering");
        let prefix = dir.join("p");
        let mut source = table_source();
        source["formations"][1] = json!([0, [1, 5, 4]]);
        let path = write_source(&prefix, "encounter_tables.json", &source);
        assert_eq!(
            build_encounter_tables(&path).unwrap_err(),
            "invalid formation 1 maximum"
        );
    }

    #[test]
    fn a_seventh_formation_element_is_rejected() {
        let dir = scratch("slots");
        let prefix = dir.join("p");
        let mut source = table_source();
        source["formations"][1] = json!([0, [1, 1, 1], [1, 1, 1], [1, 1, 1], [1, 1, 1], [1, 1, 1], [1, 1, 1]]);
        let path = write_source(&prefix, "encounter_tables.json", &source);
        assert_eq!(
            build_encounter_tables(&path).unwrap_err(),
            "formation 1 has invalid slots"
        );
    }

    #[test]
    fn wrong_format_and_wrong_extent_are_distinct_failures() {
        let dir = scratch("format");
        let prefix = dir.join("p");
        let mut source = alignment_source();
        source["format"] = json!(2);
        let path = write_source(&prefix, "alignment.json", &source);
        assert!(build_alignment(&path)
            .unwrap_err()
            .ends_with("unsupported source format"));

        let mut source = alignment_source();
        source["size"] = json!(5761);
        let path = write_source(&prefix, "alignment2.json", &source);
        assert_eq!(
            build_alignment(&path).unwrap_err(),
            "encounter alignment extent differs"
        );
    }

    #[test]
    fn addresses_are_read_with_ecmascript_number_semantics() {
        // The tracked sources store addresses as hex strings; f64::from_str
        // would reject every one of them.
        assert_eq!(js_number(&json!("0x080c5c38")), ENCOUNTER_TABLE_ADDRESS as f64);
        assert_eq!(js_number(&json!("0x080c5c38")), 135_027_768.0);
        assert_eq!(js_number(&json!("  0x20  ")), 32.0);
        assert_eq!(js_number(&json!("")), 0.0);
        assert_eq!(js_number(&json!("\u{feff}8")), 8.0);
        assert!(js_number(&json!("\u{0085}8")).is_nan());
        assert!(js_number(&json!("12abc")).is_nan());
        assert!(js_number(&json!("0x")).is_nan());
        assert_eq!(js_number(&json!(null)), 0.0);
        assert_eq!(js_number(&json!(true)), 1.0);
        assert_eq!(js_number(&json!([])), 0.0);
        assert!(js_number(&json!({})).is_nan());
        assert_eq!(js_number(&json!("1e3")), 1000.0);
    }

    #[test]
    fn integer_accepts_a_whole_float_and_rejects_a_numeric_string() {
        assert_eq!(integer(&json!(1.0), 0, 1, "v").unwrap(), 1);
        assert_eq!(integer(&json!("1"), 0, 1, "v").unwrap_err(), "invalid v");
        assert_eq!(integer(&json!(1.5), 0, 2, "v").unwrap_err(), "invalid v");
        assert_eq!(integer(&json!(9.007199254740993e15), 0, i64::MAX, "v").unwrap_err(), "invalid v");
    }

    #[test]
    fn flag_bytes_round_trip_every_value() {
        for value in 0u16..=255 {
            let value = value as u8;
            let split = split_flags(value);
            assert_eq!(flag_byte(Some(&split), "flags").unwrap(), value);
        }
        assert_eq!(
            flag_byte(Some(&json!({"bit0": false, "bits_1_4": 1})), "f").unwrap_err(),
            "f has unknown fields"
        );
        assert_eq!(flag_byte(Some(&json!(null)), "f").unwrap_err(), "f is not an object");
    }

    #[test]
    fn canonical_form_inlines_primitive_arrays() {
        let value = json!({"a": [1, 2], "b": [{"c": 1}]});
        assert_eq!(
            canonical_json(&value),
            "{\n  \"a\": [1, 2],\n  \"b\": [\n    {\n      \"c\": 1\n    }\n  ]\n}"
        );
    }

    #[test]
    fn export_round_trip_check_compares_against_the_rom() {
        // A synthetic ROM whose encounter block is exactly what the fixtures
        // describe, proving matches_rom is doing real byte work.
        let dir = scratch("export");
        let prefix = dir.join("p");
        write_source(&prefix, "brightness_curve.json", &curve_source());
        write_source(&prefix, "encounter_tables.json", &table_source());
        write_source(&prefix, "alignment.json", &alignment_source());
        let regions = build_encounter_regions(&prefix.display().to_string()).expect("regions");
        let mut rom = vec![0u8; ALIGNMENT_END - ROM_BASE];
        for region in &regions {
            let start = region.address - ROM_BASE;
            rom[start..start + region.size].copy_from_slice(&region.data);
        }
        assert!(regions.iter().all(|r| matches_rom(&rom, r)));
        rom[ENCOUNTER_TABLE_ADDRESS - ROM_BASE] ^= 1;
        assert!(!matches_rom(&rom, &regions[1]));
        // A truncated ROM must be a mismatch, not a panic.
        assert!(!matches_rom(&rom[..8], &regions[0]));
    }

    #[test]
    fn self_test_passes() {
        self_test().expect("self-test");
    }
}
