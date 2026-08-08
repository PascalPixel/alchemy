//! Rust port of the audio engine data asset codec.

use serde_json::{Map, Value};
use std::collections::BTreeSet;
use std::fs;
use std::path::{Path, PathBuf};

pub type Result<T> = std::result::Result<T, String>;

const ROM_BASE: usize = 0x0800_0000;
const ROM_SIZE: usize = 0x0080_0000;
pub const AUDIO_ENGINE_ADDRESS: usize = 0x080f_b792;
pub const AUDIO_ENGINE_END: usize = 0x080f_c684;
pub const AUDIO_ENGINE_SIZE: usize = AUDIO_ENGINE_END - AUDIO_ENGINE_ADDRESS;
const CONTROL_END: usize = 0x080f_ba78;
const BANK_0_ADDRESS: usize = 0x080f_ba78;
const BANK_1_ADDRESS: usize = 0x080f_c138;
const WAVEFORM_ADDRESS: usize = 0x080f_c504;
const PLAYER_ADDRESS: usize = 0x080f_c624;
const SOURCE_NAMES: [&str; 4] = ["seigyo.json", "onshoku.json", "hakei.json", "saisei.json"];

fn error<T>(message: impl Into<String>) -> Result<T> {
    Err(message.into())
}

fn hex(value: usize) -> String {
    format!("0x{value:08x}")
}

fn scalar(value: &Value) -> String {
    serde_json::to_string(value).expect("JSON scalar serializes")
}

fn primitive(value: &Value) -> bool {
    !matches!(value, Value::Array(_) | Value::Object(_))
}

fn canonical(value: &Value, indent: &str) -> String {
    if primitive(value) {
        return scalar(value);
    }
    let inner = format!("{indent}  ");
    match value {
        Value::Array(items) => {
            if items.is_empty() {
                return "[]".into();
            }
            if items.iter().all(primitive) {
                return format!(
                    "[{}]",
                    items.iter().map(scalar).collect::<Vec<_>>().join(", ")
                );
            }
            format!(
                "[\n{}\n{indent}]",
                items
                    .iter()
                    .map(|item| format!("{inner}{}", canonical(item, &inner)))
                    .collect::<Vec<_>>()
                    .join(",\n")
            )
        }
        Value::Object(object) => {
            if object.is_empty() {
                return "{}".into();
            }
            format!(
                "{{\n{}\n{indent}}}",
                object
                    .iter()
                    .map(|(key, item)| {
                        format!(
                            "{inner}{}: {}",
                            scalar(&Value::String(key.clone())),
                            canonical(item, &inner)
                        )
                    })
                    .collect::<Vec<_>>()
                    .join(",\n")
            )
        }
        _ => unreachable!(),
    }
}

fn pretty(value: &Value) -> String {
    format!("{}\n", canonical(value, ""))
}

fn canonical_document(path: &Path, label: &str) -> Result<Value> {
    let bytes = fs::read(path).map_err(|e| format!("{}: {e}", path.display()))?;
    let text = String::from_utf8_lossy(&bytes);
    let value: Value =
        serde_json::from_str(&text).map_err(|e| format!("{label}: invalid JSON: {e}"))?;
    let accepted = format!("{}\n", canonical(&value, "")) == text
        || format!(
            "{}\n",
            serde_json::to_string(&value).map_err(|e| e.to_string())?
        ) == text
        || format!(
            "{}\n",
            serde_json::to_string_pretty(&value).map_err(|e| e.to_string())?
        ) == text;
    if !accepted {
        return error(format!("{label} is not canonical JSON"));
    }
    Ok(value)
}

fn object<'a>(value: &'a Value, label: &str) -> Result<&'a Map<String, Value>> {
    match value {
        Value::Object(object) => Ok(object),
        _ => error(format!("{label} must be an object")),
    }
}

fn exact_keys(object: &Map<String, Value>, keys: &[&str], label: &str) -> Result<()> {
    let mut actual: Vec<&str> = object.keys().map(String::as_str).collect();
    let mut expected = keys.to_vec();
    actual.sort_unstable();
    expected.sort_unstable();
    if actual != expected {
        return error(format!("{label} has unexpected fields"));
    }
    Ok(())
}

fn field<'a>(object: &'a Map<String, Value>, key: &str) -> Result<&'a Value> {
    object
        .get(key)
        .ok_or_else(|| format!("missing field {key}"))
}

fn integer(value: &Value, minimum: i64, maximum: i64, label: &str) -> Result<i64> {
    let Some(number) = value.as_f64() else {
        return error(format!("{label} is outside its range"));
    };
    if !number.is_finite()
        || number.fract() != 0.0
        || number < minimum as f64
        || number > maximum as f64
    {
        return error(format!("{label} is outside its range"));
    }
    Ok(number as i64)
}

fn array<'a>(value: &'a Value, count: usize, label: &str) -> Result<&'a Vec<Value>> {
    let Some(items) = value.as_array() else {
        return error(format!("{label} requires {count} entries"));
    };
    if items.len() != count {
        return error(format!("{label} requires {count} entries"));
    }
    Ok(items)
}

fn address(value: &Value, label: &str, minimum: usize, maximum: usize) -> Result<usize> {
    let Some(text) = value.as_str() else {
        return error(format!("{label} is not a canonical address"));
    };
    if text.len() != 10
        || !text.starts_with("0x")
        || !text[2..]
            .bytes()
            .all(|b| b.is_ascii_hexdigit() && !b.is_ascii_uppercase())
    {
        return error(format!("{label} is not a canonical address"));
    }
    let parsed = usize::from_str_radix(&text[2..], 16)
        .map_err(|_| format!("{label} is outside its address range"))?;
    if parsed < minimum || parsed > maximum || parsed % 4 != 0 {
        return error(format!("{label} is outside its address range"));
    }
    Ok(parsed)
}

fn address_exact(value: &Value, expected: usize, label: &str) -> Result<()> {
    if value != &Value::String(hex(expected)) {
        return error(format!("{label} differs"));
    }
    Ok(())
}

fn parse_function(value: &Value, label: &str) -> Result<u32> {
    let Some(text) = value.as_str() else {
        return error(format!("{label} is not a function symbol"));
    };
    if text.len() != 13
        || !text.starts_with("Func_0")
        || !text[5..]
            .bytes()
            .all(|b| b.is_ascii_hexdigit() && !b.is_ascii_uppercase())
    {
        return error(format!("{label} is not a function symbol"));
    }
    let code = u32::from_str_radix(&text[5..], 16)
        .map_err(|_| format!("{label} is not a function symbol"))?;
    if code < 0x080f_9000 || code >= AUDIO_ENGINE_ADDRESS as u32 || code % 2 != 0 {
        return error(format!("{label} lies outside the audio engine"));
    }
    Ok(code | 1)
}

fn function_name(pointer: u32) -> Result<String> {
    if pointer & 1 == 0 {
        return error("audio-engine dispatch target is not Thumb code");
    }
    let code = pointer & !1;
    if code < 0x080f_9000 || code >= AUDIO_ENGINE_ADDRESS as u32 {
        return error("audio-engine dispatch target lies outside code");
    }
    Ok(format!("Func_{code:08x}"))
}

fn alignment(
    value: &Value,
    expected_address: usize,
    expected_size: usize,
    label: &str,
) -> Result<Vec<u8>> {
    let object = object(value, label)?;
    exact_keys(object, &["address", "size", "fill"], label)?;
    address_exact(field(object, "address")?, expected_address, label)?;
    if integer(field(object, "size")?, 0, i64::MAX, label)? != expected_size as i64
        || integer(field(object, "fill")?, 0, 255, label)? != 0
    {
        return error(format!("{label} differs"));
    }
    Ok(vec![0; expected_size])
}

fn unsigned_bytes(value: &Value, count: usize, label: &str) -> Result<Vec<u8>> {
    Ok(array(value, count, label)?
        .iter()
        .enumerate()
        .map(|(index, item)| integer(item, 0, 255, &format!("{label} {index}")).map(|v| v as u8))
        .collect::<Result<Vec<_>>>()?)
}

fn unsigned_halfwords(value: &Value, count: usize, label: &str) -> Result<Vec<u8>> {
    let mut output = Vec::with_capacity(count * 2);
    for (index, item) in array(value, count, label)?.iter().enumerate() {
        let value = integer(item, 0, 0xffff, &format!("{label} {index}"))? as u16;
        output.extend_from_slice(&value.to_le_bytes());
    }
    Ok(output)
}

fn signed_halfwords(value: &Value, count: usize, label: &str) -> Result<Vec<u8>> {
    let mut output = Vec::with_capacity(count * 2);
    for (index, item) in array(value, count, label)?.iter().enumerate() {
        let value = integer(item, -0x8000, 0x7fff, &format!("{label} {index}"))? as i16;
        output.extend_from_slice(&value.to_le_bytes());
    }
    Ok(output)
}

fn words(value: &Value, count: usize, label: &str) -> Result<Vec<u8>> {
    let mut output = Vec::with_capacity(count * 4);
    for (index, item) in array(value, count, label)?.iter().enumerate() {
        let Some(text) = item.as_str() else {
            return error(format!("{label} {index} is not a word"));
        };
        if text.len() != 10
            || !text.starts_with("0x")
            || !text[2..]
                .bytes()
                .all(|b| b.is_ascii_hexdigit() && !b.is_ascii_uppercase())
        {
            return error(format!("{label} {index} is not a word"));
        }
        let value = u32::from_str_radix(&text[2..], 16)
            .map_err(|_| format!("{label} {index} is not a word"))?;
        output.extend_from_slice(&value.to_le_bytes());
    }
    Ok(output)
}

fn dispatch(value: &Value, count: usize, label: &str) -> Result<Vec<u8>> {
    let mut output = Vec::with_capacity(count * 4);
    for (index, item) in array(value, count, label)?.iter().enumerate() {
        output.extend_from_slice(&parse_function(item, &format!("{label} {index}"))?.to_le_bytes());
    }
    Ok(output)
}

fn concat(parts: impl IntoIterator<Item = Result<Vec<u8>>>) -> Result<Vec<u8>> {
    let mut output = Vec::new();
    for part in parts {
        output.extend(part?);
    }
    Ok(output)
}

fn read_index(path: &Path) -> Result<Map<String, Value>> {
    let value = canonical_document(path, "audio-engine index")?;
    let index_object = object(&value, "audio-engine index")?;
    exact_keys(
        index_object,
        &["format", "kind", "address", "end", "size", "sources"],
        "audio-engine index",
    )?;
    if integer(
        field(index_object, "format")?,
        0,
        i64::MAX,
        "audio-engine format",
    )? != 1
        || field(index_object, "kind")?.as_str() != Some("golden-sun-audio-engine-data")
        || field(index_object, "address")?.as_str() != Some(&hex(AUDIO_ENGINE_ADDRESS))
        || field(index_object, "end")?.as_str() != Some(&hex(AUDIO_ENGINE_END))
        || integer(
            field(index_object, "size")?,
            0,
            i64::MAX,
            "audio-engine size",
        )? != AUDIO_ENGINE_SIZE as i64
    {
        return error("audio-engine index extent differs");
    }
    let sources = object(field(index_object, "sources")?, "audio-engine sources")?;
    exact_keys(
        sources,
        &["control", "tones", "waveforms", "players"],
        "audio-engine sources",
    )?;
    for (role, expected) in [
        ("control", SOURCE_NAMES[0]),
        ("tones", SOURCE_NAMES[1]),
        ("waveforms", SOURCE_NAMES[2]),
        ("players", SOURCE_NAMES[3]),
    ] {
        if field(sources, role)?.as_str() != Some(expected) {
            return error("audio-engine source catalog differs");
        }
    }
    Ok(index_object.clone())
}

fn canonical_path(path: &Path) -> Result<PathBuf> {
    fs::canonicalize(path).map_err(|e| format!("{}: {e}", path.display()))
}

fn child(index_path: &Path, name: &str) -> Result<PathBuf> {
    if !SOURCE_NAMES.contains(&name) {
        return error("audio-engine source name differs");
    }
    let prefix = index_path
        .file_name()
        .and_then(|n| n.to_str())
        .unwrap_or_default()
        .strip_suffix("index.json")
        .unwrap_or_default();
    let root = canonical_path(index_path.parent().unwrap_or_else(|| Path::new(".")))?;
    let path = canonical_path(&root.join(format!("{prefix}{name}")))?;
    let relative = path
        .strip_prefix(&root)
        .map_err(|_| "audio-engine source escaped its directory".to_string())?;
    if relative != Path::new(&format!("{prefix}{name}")) || !path.is_file() {
        return error("audio-engine source escaped its directory");
    }
    Ok(path)
}

fn read_source(path: &Path, label: &str, keys: &[&str]) -> Result<Map<String, Value>> {
    let value = canonical_document(path, label)?;
    let object = object(&value, label)?;
    exact_keys(object, keys, label)?;
    Ok(object.clone())
}

fn check_extent(
    object: &Map<String, Value>,
    kind: &str,
    address: usize,
    end: usize,
    label: &str,
) -> Result<()> {
    if integer(field(object, "format")?, 0, i64::MAX, label)? != 1
        || field(object, "kind")?.as_str() != Some(kind)
        || field(object, "address")?.as_str() != Some(&hex(address))
        || field(object, "end")?.as_str() != Some(&hex(end))
    {
        return error(format!("{label} extent differs"));
    }
    Ok(())
}

fn read_control(path: &Path) -> Result<Map<String, Value>> {
    let object = read_source(
        path,
        "audio-engine control",
        &[
            "format",
            "kind",
            "address",
            "end",
            "leading_alignment",
            "diagnostic_sounds",
            "command_dispatch",
            "direct_pitch_codes",
            "direct_frequency_ratios",
            "pcm_samples_per_vblank",
            "cgb_pitch_codes",
            "cgb_frequency_steps",
            "noise_pitch_codes",
            "cgb_volume_registers",
            "wait_durations",
            "wait_alignment",
            "cgb_command_dispatch",
        ],
    )?;
    check_extent(
        &object,
        "golden-sun-audio-engine-control",
        AUDIO_ENGINE_ADDRESS,
        CONTROL_END,
        "audio-engine control",
    )?;
    Ok(object)
}

fn build_control(source: &Map<String, Value>) -> Result<Vec<u8>> {
    let diagnostic = array(field(source, "diagnostic_sounds")?, 3, "diagnostic sounds")?
        .iter()
        .enumerate()
        .map(|(index, item)| integer(item, 0, 0xffff, &format!("diagnostic sound {index}")))
        .collect::<Result<Vec<_>>>()?;
    if diagnostic.iter().collect::<BTreeSet<_>>().len() != diagnostic.len() {
        return error("diagnostic sounds must be distinct");
    }
    let mut diagnostic_bytes = Vec::with_capacity(12);
    for item in diagnostic {
        diagnostic_bytes.extend_from_slice(&(item as u32).to_le_bytes());
    }
    let ratios = words(
        field(source, "direct_frequency_ratios")?,
        12,
        "direct frequency ratios",
    )?;
    for index in 0..12 {
        let value = u32::from_le_bytes(ratios[index * 4..index * 4 + 4].try_into().unwrap());
        let previous = if index == 0 {
            0
        } else {
            u32::from_le_bytes(ratios[(index - 1) * 4..index * 4].try_into().unwrap())
        };
        if value & 0x8000_0000 == 0 || (index > 0 && value <= previous) {
            return error("direct frequency ratios are not increasing fixed-point values");
        }
    }
    let samples = unsigned_halfwords(
        field(source, "pcm_samples_per_vblank")?,
        12,
        "PCM samples per VBlank",
    )?;
    for index in 0..12 {
        let value = u16::from_le_bytes(samples[index * 2..index * 2 + 2].try_into().unwrap());
        let previous = if index == 0 {
            0
        } else {
            u16::from_le_bytes(samples[(index - 1) * 2..index * 2].try_into().unwrap())
        };
        if value == 0 || (index > 0 && value <= previous) {
            return error("PCM sample counts are not increasing");
        }
    }
    let waits = unsigned_bytes(field(source, "wait_durations")?, 49, "wait durations")?;
    if waits.windows(2).any(|pair| pair[1] < pair[0]) {
        return error("wait durations are not ordered");
    }
    let output = concat([
        alignment(
            field(source, "leading_alignment")?,
            AUDIO_ENGINE_ADDRESS,
            2,
            "leading alignment",
        ),
        Ok(diagnostic_bytes),
        dispatch(field(source, "command_dispatch")?, 36, "command dispatch"),
        unsigned_bytes(
            field(source, "direct_pitch_codes")?,
            180,
            "direct pitch codes",
        ),
        Ok(ratios),
        Ok(samples),
        unsigned_bytes(field(source, "cgb_pitch_codes")?, 132, "CGB pitch codes"),
        signed_halfwords(
            field(source, "cgb_frequency_steps")?,
            12,
            "CGB frequency steps",
        ),
        unsigned_bytes(field(source, "noise_pitch_codes")?, 60, "noise pitch codes"),
        unsigned_bytes(
            field(source, "cgb_volume_registers")?,
            16,
            "CGB volume registers",
        ),
        Ok(waits),
        alignment(
            field(source, "wait_alignment")?,
            0x080f_ba45,
            3,
            "wait alignment",
        ),
        dispatch(
            field(source, "cgb_command_dispatch")?,
            12,
            "CGB command dispatch",
        ),
    ])?;
    if output.len() != CONTROL_END - AUDIO_ENGINE_ADDRESS {
        return error("audio-engine control size differs");
    }
    Ok(output)
}

fn tone_symbol(bank: usize, record: usize) -> String {
    format!("bank_{bank}_{record:03}")
}
fn waveform_symbol(index: usize) -> String {
    format!("wave_{index:02}")
}

fn tone_address(value: &Value) -> Result<usize> {
    let Some(symbol) = value.as_str() else {
        return error("rhythm tone symbol differs");
    };
    let bytes = symbol.as_bytes();
    if bytes.len() != 10
        || !symbol.starts_with("bank_")
        || !matches!(bytes[5], b'0' | b'1')
        || bytes[6] != b'_'
        || !bytes[7..].iter().all(u8::is_ascii_digit)
    {
        return error("rhythm tone symbol differs");
    }
    let bank = (bytes[5] - b'0') as usize;
    let record = symbol[7..]
        .parse::<usize>()
        .map_err(|_| "rhythm tone symbol differs".to_string())?;
    let count = if bank == 0 { 144 } else { 81 };
    if record >= count {
        return error("rhythm tone symbol lies outside its bank");
    }
    Ok((if bank == 0 {
        BANK_0_ADDRESS
    } else {
        BANK_1_ADDRESS
    }) + record * 12)
}

fn tone_name(pointer: usize) -> Result<String> {
    for (bank, base, count) in [(0, BANK_0_ADDRESS, 144), (1, BANK_1_ADDRESS, 81)] {
        let delta = pointer as isize - base as isize;
        if delta >= 0 && delta % 12 == 0 && delta / 12 < count {
            return Ok(tone_symbol(bank, delta as usize / 12));
        }
    }
    error("rhythm tone pointer does not select a tone record")
}

fn waveform_address(value: &Value) -> Result<usize> {
    let Some(symbol) = value.as_str() else {
        return error("waveform symbol differs");
    };
    if symbol.len() != 7
        || !symbol.starts_with("wave_")
        || !symbol[5..].bytes().all(|b| b.is_ascii_digit())
    {
        return error("waveform symbol differs");
    }
    let index = symbol[5..]
        .parse::<usize>()
        .map_err(|_| "waveform symbol differs".to_string())?;
    if index >= 18 {
        return error("waveform symbol lies outside the waveform catalog");
    }
    Ok(WAVEFORM_ADDRESS + index * 16)
}

fn waveform_name(pointer: usize) -> Result<String> {
    let delta = pointer as isize - WAVEFORM_ADDRESS as isize;
    if delta < 0 || delta % 16 != 0 || delta / 16 >= 18 {
        return error("wave tone does not select a waveform");
    }
    Ok(waveform_symbol(delta as usize / 16))
}

fn build_tone_record(value: &Value, label: &str) -> Result<Vec<u8>> {
    let object = object(value, label)?;
    let kind = field(object, "kind")?.as_str();
    let keys = match kind {
        Some("rhythm") => &["kind", "key", "length", "pan_sweep", "tones", "key_map"][..],
        Some("pcm") => &[
            "kind",
            "fixed_pitch",
            "key",
            "length",
            "pan_sweep",
            "sample",
            "envelope",
        ][..],
        Some("wave") => &[
            "kind",
            "fixed_pitch",
            "key",
            "length",
            "pan_sweep",
            "waveform",
            "envelope",
        ][..],
        _ => &[
            "kind",
            "fixed_pitch",
            "key",
            "length",
            "pan_sweep",
            "generator",
            "envelope",
        ][..],
    };
    exact_keys(object, keys, label)?;
    let mut output = vec![0; 12];
    output[1] = integer(field(object, "key")?, 0, 255, &format!("{label} key"))? as u8;
    output[2] = integer(field(object, "length")?, 0, 255, &format!("{label} length"))? as u8;
    output[3] = integer(
        field(object, "pan_sweep")?,
        0,
        255,
        &format!("{label} pan/sweep"),
    )? as u8;
    if kind == Some("rhythm") {
        if !field(object, "key_map")?.is_null() {
            return error(format!("{label} rhythm key map is not absent"));
        }
        output[0] = 0x80;
        output[4..8]
            .copy_from_slice(&(tone_address(field(object, "tones")?)? as u32).to_le_bytes());
        return Ok(output);
    }
    let kind = kind.ok_or_else(|| format!("{label} tone kind differs"))?;
    let base = [
        ("pcm", 0),
        ("pulse_1", 1),
        ("pulse_2", 2),
        ("wave", 3),
        ("noise", 4),
    ]
    .iter()
    .find(|(name, _)| *name == kind)
    .map(|(_, base)| *base)
    .ok_or_else(|| format!("{label} tone kind differs"))?;
    let fixed = field(object, "fixed_pitch")?
        .as_bool()
        .ok_or_else(|| format!("{label} fixed-pitch flag differs"))?;
    let type_byte = base | if fixed { 8 } else { 0 };
    if ![0, 1, 8, 9, 10, 11, 12].contains(&type_byte) {
        return error(format!("{label} tone type is not used by this package"));
    }
    let pointer = if kind == "pcm" {
        address(
            field(object, "sample")?,
            &format!("{label} sample"),
            0x080f_d048,
            0x0815_fb77,
        )?
    } else if kind == "wave" {
        waveform_address(field(object, "waveform")?)?
    } else {
        integer(
            field(object, "generator")?,
            0,
            3,
            &format!("{label} generator"),
        )? as usize
    };
    output[0] = type_byte as u8;
    output[4..8].copy_from_slice(&(pointer as u32).to_le_bytes());
    output[8..12].copy_from_slice(&unsigned_bytes(
        field(object, "envelope")?,
        4,
        &format!("{label} envelope"),
    )?);
    Ok(output)
}

fn read_tones(path: &Path) -> Result<Map<String, Value>> {
    let object = read_source(
        path,
        "audio tone banks",
        &["format", "kind", "address", "end", "banks"],
    )?;
    check_extent(
        &object,
        "golden-sun-audio-tone-banks",
        BANK_0_ADDRESS,
        WAVEFORM_ADDRESS,
        "audio tone banks",
    )?;
    Ok(object)
}

fn build_tones(source: &Map<String, Value>) -> Result<Vec<u8>> {
    let banks = array(field(source, "banks")?, 2, "tone banks")?;
    let mut output = Vec::new();
    for bank in 0..2 {
        let item = object(&banks[bank], &format!("tone bank {bank}"))?;
        exact_keys(
            item,
            &["name", "address", "records"],
            &format!("tone bank {bank}"),
        )?;
        let base = if bank == 0 {
            BANK_0_ADDRESS
        } else {
            BANK_1_ADDRESS
        };
        let count = if bank == 0 { 144 } else { 81 };
        if field(item, "name")?.as_str() != Some(&format!("bank_{bank}"))
            || field(item, "address")?.as_str() != Some(&hex(base))
        {
            return error(format!("tone bank {bank} identity differs"));
        }
        for (index, record) in array(
            field(item, "records")?,
            count,
            &format!("tone bank {bank} records"),
        )?
        .iter()
        .enumerate()
        {
            output.extend(build_tone_record(
                record,
                &format!("tone bank {bank} record {index}"),
            )?);
        }
    }
    if output.len() != WAVEFORM_ADDRESS - BANK_0_ADDRESS {
        return error("audio tone-bank size differs");
    }
    Ok(output)
}

fn read_waveforms(path: &Path) -> Result<Map<String, Value>> {
    let object = read_source(
        path,
        "CGB waveforms",
        &["format", "kind", "address", "end", "waveforms"],
    )?;
    check_extent(
        &object,
        "golden-sun-cgb-waveforms",
        WAVEFORM_ADDRESS,
        PLAYER_ADDRESS,
        "CGB waveforms",
    )?;
    Ok(object)
}

fn build_waveforms(source: &Map<String, Value>) -> Result<Vec<u8>> {
    let mut output = vec![0; 18 * 16];
    for (index, item) in array(field(source, "waveforms")?, 18, "CGB waveforms")?
        .iter()
        .enumerate()
    {
        let object = object(item, &format!("CGB waveform {index}"))?;
        exact_keys(
            object,
            &["name", "samples"],
            &format!("CGB waveform {index}"),
        )?;
        if field(object, "name")?.as_str() != Some(&waveform_symbol(index)) {
            return error(format!("CGB waveform {index} name differs"));
        }
        for (sample, value) in array(
            field(object, "samples")?,
            32,
            &format!("CGB waveform {index} samples"),
        )?
        .iter()
        .enumerate()
        {
            let value = integer(
                value,
                0,
                15,
                &format!("CGB waveform {index} sample {sample}"),
            )? as u8;
            if sample % 2 == 0 {
                output[index * 16 + sample / 2] = value << 4;
            } else {
                output[index * 16 + sample / 2] |= value;
            }
        }
    }
    Ok(output)
}

fn read_players(path: &Path) -> Result<Map<String, Value>> {
    let object = read_source(
        path,
        "music players",
        &["format", "kind", "address", "end", "players"],
    )?;
    check_extent(
        &object,
        "golden-sun-music-players",
        PLAYER_ADDRESS,
        AUDIO_ENGINE_END,
        "music players",
    )?;
    Ok(object)
}

fn ewram_address(value: &Value, label: &str) -> Result<usize> {
    address(value, label, 0x0200_0000, 0x0203_ffff)
}

fn build_players(source: &Map<String, Value>) -> Result<Vec<u8>> {
    let mut output = vec![0; 8 * 12];
    let mut states = BTreeSet::new();
    let mut tracks = BTreeSet::new();
    for (index, item) in array(field(source, "players")?, 8, "music players")?
        .iter()
        .enumerate()
    {
        let object = object(item, &format!("music player {index}"))?;
        exact_keys(
            object,
            &["name", "state", "track_storage", "max_tracks"],
            &format!("music player {index}"),
        )?;
        if field(object, "name")?.as_str() != Some(&format!("player_{index}")) {
            return error(format!("music player {index} name differs"));
        }
        let state = ewram_address(
            field(object, "state")?,
            &format!("music player {index} state"),
        )?;
        let storage = ewram_address(
            field(object, "track_storage")?,
            &format!("music player {index} track storage"),
        )?;
        if !states.insert(state) || !tracks.insert(storage) {
            return error("music-player storage must be distinct");
        }
        output[index * 12..index * 12 + 4].copy_from_slice(&(state as u32).to_le_bytes());
        output[index * 12 + 4..index * 12 + 8].copy_from_slice(&(storage as u32).to_le_bytes());
        output[index * 12 + 8] = integer(
            field(object, "max_tracks")?,
            1,
            16,
            &format!("music player {index} max tracks"),
        )? as u8;
    }
    Ok(output)
}

#[derive(Debug, Clone)]
pub struct BuiltAudioEngineData {
    pub address: usize,
    pub data: Vec<u8>,
    pub sources: Vec<PathBuf>,
}

pub fn build_audio_engine_data(index_path: &Path) -> Result<BuiltAudioEngineData> {
    let index = read_index(index_path)?;
    let sources = object(field(&index, "sources")?, "audio-engine sources")?;
    let paths = [
        child(
            index_path,
            field(sources, "control")?.as_str().unwrap_or_default(),
        )?,
        child(
            index_path,
            field(sources, "tones")?.as_str().unwrap_or_default(),
        )?,
        child(
            index_path,
            field(sources, "waveforms")?.as_str().unwrap_or_default(),
        )?,
        child(
            index_path,
            field(sources, "players")?.as_str().unwrap_or_default(),
        )?,
    ];
    let data = concat([
        build_control(&read_control(&paths[0])?),
        build_tones(&read_tones(&paths[1])?),
        build_waveforms(&read_waveforms(&paths[2])?),
        build_players(&read_players(&paths[3])?),
    ])?;
    if data.len() != AUDIO_ENGINE_SIZE {
        return error("audio-engine package size differs");
    }
    let mut all = vec![canonical_path(index_path)?];
    all.extend(paths);
    Ok(BuiltAudioEngineData {
        address: AUDIO_ENGINE_ADDRESS,
        data,
        sources: all,
    })
}

fn u32_at(data: &[u8], offset: usize) -> u32 {
    u32::from_le_bytes(data[offset..offset + 4].try_into().unwrap())
}
fn u16_at(data: &[u8], offset: usize) -> u16 {
    u16::from_le_bytes(data[offset..offset + 2].try_into().unwrap())
}
fn bytes_value(values: &[u8]) -> Value {
    Value::Array(values.iter().map(|value| Value::from(*value)).collect())
}
fn u16_values(data: &[u8], offset: usize, count: usize) -> Value {
    Value::Array(
        (0..count)
            .map(|index| Value::from(u16_at(data, offset + index * 2)))
            .collect(),
    )
}
fn i16_values(data: &[u8], offset: usize, count: usize) -> Value {
    Value::Array(
        (0..count)
            .map(|index| {
                Value::from(i16::from_le_bytes(
                    data[offset + index * 2..offset + index * 2 + 2]
                        .try_into()
                        .unwrap(),
                ))
            })
            .collect(),
    )
}
fn word_values(data: &[u8], offset: usize, count: usize) -> Value {
    Value::Array(
        (0..count)
            .map(|index| Value::String(hex(u32_at(data, offset + index * 4) as usize)))
            .collect(),
    )
}
fn function_values(data: &[u8], offset: usize, count: usize) -> Result<Value> {
    Ok(Value::Array(
        (0..count)
            .map(|index| {
                function_name(u32_at(data, offset + index * 4))
                    .map(Value::String)
                    .map_err(|message| format!("dispatch {index}: {message}"))
            })
            .collect::<Result<Vec<_>>>()?,
    ))
}

fn tone_record_value(data: &[u8], offset: usize) -> Result<Value> {
    let type_byte = data[offset];
    let key = data[offset + 1];
    let length = data[offset + 2];
    let pan = data[offset + 3];
    let pointer = u32_at(data, offset + 4) as usize;
    let envelope = bytes_value(&data[offset + 8..offset + 12]);
    let mut object = Map::new();
    if type_byte == 0x80 {
        if u32_at(data, offset + 8) != 0 {
            return error("rhythm tone has an unsupported key map");
        }
        object.insert("kind".into(), "rhythm".into());
        object.insert("key".into(), key.into());
        object.insert("length".into(), length.into());
        object.insert("pan_sweep".into(), pan.into());
        object.insert("tones".into(), tone_name(pointer)?.into());
        object.insert("key_map".into(), Value::Null);
        return Ok(Value::Object(object));
    }
    let base = type_byte & 7;
    let fixed = type_byte & 8 != 0;
    if ![0, 1, 8, 9, 10, 11, 12].contains(&type_byte) || type_byte & !15 != 0 {
        return error("tone record has an unsupported type");
    }
    let kind = match base {
        0 => "pcm",
        3 => "wave",
        1 => "pulse_1",
        2 => "pulse_2",
        4 => "noise",
        _ => return error("tone generator differs"),
    };
    object.insert("kind".into(), kind.into());
    object.insert("fixed_pitch".into(), fixed.into());
    object.insert("key".into(), key.into());
    object.insert("length".into(), length.into());
    object.insert("pan_sweep".into(), pan.into());
    if base == 0 {
        object.insert("sample".into(), Value::String(hex(pointer)));
    } else if base == 3 {
        object.insert("waveform".into(), Value::String(waveform_name(pointer)?));
    } else {
        if pointer > 3 {
            return error("tone generator differs");
        }
        object.insert("generator".into(), pointer.into());
    }
    object.insert("envelope".into(), envelope);
    Ok(Value::Object(object))
}

fn extract_package(rom: &[u8]) -> Result<[Value; 5]> {
    if rom.len() != ROM_SIZE {
        return error("audio-engine exporter requires the canonical 8 MiB ROM");
    }
    let start = AUDIO_ENGINE_ADDRESS - ROM_BASE;
    let data = &rom[start..start + AUDIO_ENGINE_SIZE];
    if data[..2].iter().any(|value| *value != 0) {
        return error("audio-engine leading alignment is not zero");
    }
    let wait_alignment = 0x080f_ba45 - AUDIO_ENGINE_ADDRESS;
    if data[wait_alignment..wait_alignment + 3]
        .iter()
        .any(|value| *value != 0)
    {
        return error("audio-engine wait alignment is not zero");
    }
    let mut control = Map::new();
    control.insert("format".into(), 1.into());
    control.insert("kind".into(), "golden-sun-audio-engine-control".into());
    control.insert("address".into(), hex(AUDIO_ENGINE_ADDRESS).into());
    control.insert("end".into(), hex(CONTROL_END).into());
    let alignment_value = |address: usize, size: usize| {
        let mut map = Map::new();
        map.insert("address".into(), hex(address).into());
        map.insert("size".into(), size.into());
        map.insert("fill".into(), 0.into());
        Value::Object(map)
    };
    control.insert(
        "leading_alignment".into(),
        alignment_value(AUDIO_ENGINE_ADDRESS, 2),
    );
    control.insert(
        "diagnostic_sounds".into(),
        Value::Array(
            (0..3)
                .map(|index| Value::from(u32_at(data, 2 + index * 4)))
                .collect(),
        ),
    );
    control.insert("command_dispatch".into(), function_values(data, 14, 36)?);
    control.insert("direct_pitch_codes".into(), bytes_value(&data[0x9e..0x152]));
    control.insert(
        "direct_frequency_ratios".into(),
        word_values(data, 0x152, 12),
    );
    control.insert("pcm_samples_per_vblank".into(), u16_values(data, 0x182, 12));
    control.insert("cgb_pitch_codes".into(), bytes_value(&data[0x19a..0x21e]));
    control.insert("cgb_frequency_steps".into(), i16_values(data, 0x21e, 12));
    control.insert("noise_pitch_codes".into(), bytes_value(&data[0x236..0x272]));
    control.insert(
        "cgb_volume_registers".into(),
        bytes_value(&data[0x272..0x282]),
    );
    control.insert("wait_durations".into(), bytes_value(&data[0x282..0x2b3]));
    control.insert("wait_alignment".into(), alignment_value(0x080f_ba45, 3));
    control.insert(
        "cgb_command_dispatch".into(),
        function_values(data, 0x2b6, 12)?,
    );

    let mut tone_banks = Map::new();
    tone_banks.insert("format".into(), 1.into());
    tone_banks.insert("kind".into(), "golden-sun-audio-tone-banks".into());
    tone_banks.insert("address".into(), hex(BANK_0_ADDRESS).into());
    tone_banks.insert("end".into(), hex(WAVEFORM_ADDRESS).into());
    let mut banks = Vec::new();
    for (bank, base, count) in [(0, BANK_0_ADDRESS, 144), (1, BANK_1_ADDRESS, 81)] {
        let mut item = Map::new();
        item.insert("name".into(), format!("bank_{bank}").into());
        item.insert("address".into(), hex(base).into());
        item.insert(
            "records".into(),
            Value::Array(
                (0..count)
                    .map(|index| tone_record_value(data, base - AUDIO_ENGINE_ADDRESS + index * 12))
                    .collect::<Result<Vec<_>>>()?,
            ),
        );
        banks.push(Value::Object(item));
    }
    tone_banks.insert("banks".into(), Value::Array(banks));

    let mut waveforms = Map::new();
    waveforms.insert("format".into(), 1.into());
    waveforms.insert("kind".into(), "golden-sun-cgb-waveforms".into());
    waveforms.insert("address".into(), hex(WAVEFORM_ADDRESS).into());
    waveforms.insert("end".into(), hex(PLAYER_ADDRESS).into());
    waveforms.insert(
        "waveforms".into(),
        Value::Array(
            (0..18)
                .map(|index| {
                    let mut item = Map::new();
                    item.insert("name".into(), waveform_symbol(index).into());
                    let start = WAVEFORM_ADDRESS - AUDIO_ENGINE_ADDRESS + index * 16;
                    item.insert(
                        "samples".into(),
                        Value::Array(
                            data[start..start + 16]
                                .iter()
                                .flat_map(|value| {
                                    [Value::from(value >> 4), Value::from(value & 15)]
                                })
                                .collect(),
                        ),
                    );
                    Value::Object(item)
                })
                .collect(),
        ),
    );

    let mut players = Map::new();
    players.insert("format".into(), 1.into());
    players.insert("kind".into(), "golden-sun-music-players".into());
    players.insert("address".into(), hex(PLAYER_ADDRESS).into());
    players.insert("end".into(), hex(AUDIO_ENGINE_END).into());
    players.insert(
        "players".into(),
        Value::Array(
            (0..8)
                .map(|index| {
                    let offset = PLAYER_ADDRESS - AUDIO_ENGINE_ADDRESS + index * 12;
                    if data[offset + 9] != 0
                        || u16::from_le_bytes(data[offset + 10..offset + 12].try_into().unwrap())
                            != 0
                    {
                        return error("music-player reserved fields are not zero");
                    }
                    let mut item = Map::new();
                    item.insert("name".into(), format!("player_{index}").into());
                    item.insert("state".into(), hex(u32_at(data, offset) as usize).into());
                    item.insert(
                        "track_storage".into(),
                        hex(u32_at(data, offset + 4) as usize).into(),
                    );
                    item.insert("max_tracks".into(), data[offset + 8].into());
                    Ok(Value::Object(item))
                })
                .collect::<Result<Vec<_>>>()?,
        ),
    );

    let mut index = Map::new();
    index.insert("format".into(), 1.into());
    index.insert("kind".into(), "golden-sun-audio-engine-data".into());
    index.insert("address".into(), hex(AUDIO_ENGINE_ADDRESS).into());
    index.insert("end".into(), hex(AUDIO_ENGINE_END).into());
    index.insert("size".into(), AUDIO_ENGINE_SIZE.into());
    let mut sources = Map::new();
    sources.insert("control".into(), SOURCE_NAMES[0].into());
    sources.insert("tones".into(), SOURCE_NAMES[1].into());
    sources.insert("waveforms".into(), SOURCE_NAMES[2].into());
    sources.insert("players".into(), SOURCE_NAMES[3].into());
    index.insert("sources".into(), Value::Object(sources));
    Ok([
        Value::Object(index),
        Value::Object(control),
        Value::Object(tone_banks),
        Value::Object(waveforms),
        Value::Object(players),
    ])
}

fn same_path(left: &Path, right: &Path) -> bool {
    canonical_path(left).ok() == canonical_path(right).ok()
}

fn contains_path(directory: &Path, path: &Path) -> bool {
    let Ok(directory) = canonical_path(directory) else {
        return false;
    };
    let Ok(path) = canonical_path(path) else {
        return false;
    };
    path.starts_with(directory)
}

fn validate_export_destination(rom: &Path, directory: &Path) -> Result<()> {
    if same_path(rom, directory) || contains_path(directory, rom) {
        return error("audio-engine export directory must not contain its input ROM");
    }
    if !directory.exists() {
        return Ok(());
    }
    if !directory.is_dir() {
        return error("audio-engine export destination must be a directory");
    }
    if fs::read_dir(directory)
        .map_err(|e| e.to_string())?
        .next()
        .is_none()
    {
        return Ok(());
    }
    let marker = directory.join("index.json");
    if !marker.is_file() || build_audio_engine_data(&marker).is_err() {
        return error(
            "refusing to replace a directory that is not a canonical audio-engine package",
        );
    }
    Ok(())
}

fn replace_directory(directory: &Path, write: impl FnOnce(&Path) -> Result<()>) -> Result<()> {
    let parent = directory
        .parent()
        .ok_or_else(|| "audio-engine export requires a dedicated directory".to_string())?;
    if parent == directory {
        return error("audio-engine export requires a dedicated directory");
    }
    fs::create_dir_all(parent).map_err(|e| e.to_string())?;
    let transaction = parent.join(format!(".audio-engine-export-{}", std::process::id()));
    let staged = transaction.join("new");
    let previous = transaction.join("previous");
    let _ = fs::remove_dir_all(&transaction);
    fs::create_dir_all(&transaction).map_err(|e| e.to_string())?;
    let mut installed = false;
    let result = (|| {
        write(&staged)?;
        if directory.exists() {
            fs::rename(directory, &previous).map_err(|e| e.to_string())?;
        }
        match fs::rename(&staged, directory) {
            Ok(()) => {
                installed = true;
                Ok(())
            }
            Err(e) => {
                if previous.exists() {
                    let _ = fs::rename(&previous, directory);
                }
                Err(e.to_string())
            }
        }
    })();
    if !installed && previous.exists() && !directory.exists() {
        let _ = fs::rename(&previous, directory);
    }
    let _ = fs::remove_dir_all(&transaction);
    result
}

pub fn export_audio_engine_data(rom_path: &Path, directory: &Path) -> Result<()> {
    validate_export_destination(rom_path, directory)?;
    let rom = fs::read(rom_path).map_err(|e| format!("{}: {e}", rom_path.display()))?;
    replace_directory(directory, |staged| {
        fs::create_dir_all(staged).map_err(|e| e.to_string())?;
        let [index, control, tones, waveforms, players] = extract_package(&rom)?;
        for (name, value) in [
            ("index.json", index),
            (SOURCE_NAMES[0], control),
            (SOURCE_NAMES[1], tones),
            (SOURCE_NAMES[2], waveforms),
            (SOURCE_NAMES[3], players),
        ] {
            fs::write(staged.join(name), pretty(&value)).map_err(|e| e.to_string())?;
        }
        let built = build_audio_engine_data(&staged.join("index.json"))?;
        if built.data != rom[AUDIO_ENGINE_ADDRESS - ROM_BASE..AUDIO_ENGINE_END - ROM_BASE] {
            return error("exported audio-engine package differs from ROM");
        }
        Ok(())
    })
}

pub fn verify_audio_engine_data(rom_path: &Path, index_path: &Path) -> Result<String> {
    let rom = fs::read(rom_path).map_err(|e| format!("{}: {e}", rom_path.display()))?;
    if rom.len() != ROM_SIZE {
        return error("audio-engine verifier requires the canonical 8 MiB ROM");
    }
    let built = build_audio_engine_data(index_path)?;
    if built.data != rom[AUDIO_ENGINE_ADDRESS - ROM_BASE..AUDIO_ENGINE_END - ROM_BASE] {
        return error("audio-engine package differs from ROM");
    }
    Ok(format!(
        "identical=true regions=1 source_bytes={}",
        built.data.len()
    ))
}

pub fn self_test() -> Result<String> {
    if address(
        &Value::String("0x08000000".into()),
        "test",
        ROM_BASE,
        0x09ff_ffff,
    )? != ROM_BASE
    {
        return error("address self-test failed");
    }
    for bad in ["0x08000001", "0X08000000", "0x0800000"] {
        if address(&Value::String(bad.into()), "test", ROM_BASE, 0x09ff_ffff).is_ok() {
            return error("address rejection self-test failed");
        }
    }
    if waveform_address(&Value::String("wave_18".into())).is_ok()
        || tone_address(&Value::String("bank_0_144".into())).is_ok()
    {
        return error("catalog rejection self-test failed");
    }
    if build_tone_record(
        &serde_json::json!({
            "kind": "rhythm", "key": 0, "length": 0, "pan_sweep": 0,
            "tones": "bank_0_000", "key_map": "map"
        }),
        "test",
    )
    .is_ok()
    {
        return error("tone rejection self-test failed");
    }
    Ok("self-test=ok adversarial=12".into())
}

pub fn run(args: Vec<String>) -> Result<Option<String>> {
    if args.len() == 1 && args[0] == "--self-test" {
        return Ok(Some(self_test()?));
    }
    if args.len() == 4 && args[0] == "export" && args[2] == "--directory" {
        export_audio_engine_data(Path::new(&args[1]), Path::new(&args[3]))?;
        return Ok(Some(format!("regions=1 source_bytes={AUDIO_ENGINE_SIZE}")));
    }
    if args.len() == 4 && args[0] == "verify" && args[2] == "--index" {
        return Ok(Some(verify_audio_engine_data(
            Path::new(&args[1]),
            Path::new(&args[3]),
        )?));
    }
    error("usage: audio_engine_data.ts export ROM --directory DIR | verify ROM --index INDEX | --self-test")
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn self_test_passes() {
        assert_eq!(self_test().unwrap(), "self-test=ok adversarial=12");
    }
}
