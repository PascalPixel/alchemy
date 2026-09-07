use crate::Result;

use serde_json::{Map, Value};
use std::collections::BTreeSet;
use std::fs;
use std::path::{Component, Path, PathBuf};
pub const AUDIO_ENGINE_ADDRESS: usize = 0x080f_b792;
pub const AUDIO_ENGINE_END: usize = 0x080f_c684;
pub const AUDIO_ENGINE_SIZE: usize = AUDIO_ENGINE_END - AUDIO_ENGINE_ADDRESS;
const CONTROL_END: usize = 0x080f_ba78;
const BANK_0_ADDRESS: usize = 0x080f_ba78;
const BANK_1_ADDRESS: usize = 0x080f_c138;
const WAVEFORM_ADDRESS: usize = 0x080f_c504;
const PLAYER_ADDRESS: usize = 0x080f_c624;
const SOURCE_NAMES: [&str; 4] = [
    "control.tsv",
    "voicegroups/index.tsv",
    "waveforms.tsv",
    "players.tsv",
];
const CONTROL_SECTIONS: [(&str, usize); 13] = [
    ("leading_alignment", 2),
    ("diagnostic_sounds", 3),
    ("command_dispatch", 36),
    ("direct_pitch_codes", 180),
    ("direct_frequency_ratios", 12),
    ("pcm_samples_per_vblank", 12),
    ("cgb_pitch_codes", 132),
    ("cgb_frequency_steps", 12),
    ("noise_pitch_codes", 60),
    ("cgb_volume_registers", 16),
    ("wait_durations", 49),
    ("wait_alignment", 3),
    ("cgb_command_dispatch", 12),
];
fn error<T>(message: impl Into<String>) -> Result<T> {
    Err(message.into())
}
fn hex(value: usize) -> String {
    format!("0x{value:08x}")
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
    array(value, count, label)?
        .iter()
        .enumerate()
        .map(|(index, item)| integer(item, 0, 255, &format!("{label} {index}")).map(|v| v as u8))
        .collect::<Result<Vec<_>>>()
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
fn engine_paths(path: &Path) -> Result<[PathBuf; 4]> {
    let text = fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
    let mut lines = text.lines().filter(|line| !line.starts_with('#'));
    if lines.next() != Some("section\taddress\tend\tsource") {
        return error("audio-engine package header differs");
    }
    let sections = [
        ("control", AUDIO_ENGINE_ADDRESS, CONTROL_END),
        ("voicegroups", BANK_0_ADDRESS, WAVEFORM_ADDRESS),
        ("waveforms", WAVEFORM_ADDRESS, PLAYER_ADDRESS),
        ("players", PLAYER_ADDRESS, AUDIO_ENGINE_END),
    ];
    let mut paths = Vec::with_capacity(4);
    for (index, (section, address, end)) in sections.iter().enumerate() {
        let fields = lines
            .next()
            .ok_or_else(|| format!("audio-engine {section} section is absent"))?
            .split('\t')
            .collect::<Vec<_>>();
        if fields.len() != 4
            || fields[0] != *section
            || fields[1] != hex(*address)
            || fields[2] != hex(*end)
            || fields[3] != SOURCE_NAMES[index]
        {
            return error(format!("audio-engine {section} section differs"));
        }
        paths.push(child(path, fields[3])?);
    }
    if lines.next().is_some() {
        return error("audio-engine package has extra rows");
    }
    paths
        .try_into()
        .map_err(|_| "audio-engine package section count differs".to_string())
}
fn canonical_path(path: &Path) -> Result<PathBuf> {
    fs::canonicalize(path).map_err(|e| format!("{}: {e}", path.display()))
}
fn child(index_path: &Path, name: &str) -> Result<PathBuf> {
    let relative = Path::new(name);
    if relative.is_absolute()
        || relative
            .components()
            .any(|component| !matches!(component, Component::Normal(_)))
    {
        return error("audio-engine source escaped its directory");
    }
    let root = canonical_path(index_path.parent().unwrap_or_else(|| Path::new(".")))?;
    let path = canonical_path(&root.join(relative))?;
    let actual = path
        .strip_prefix(&root)
        .map_err(|_| "audio-engine source escaped its directory".to_string())?;
    if actual != relative || !path.is_file() {
        return error("audio-engine source escaped its directory");
    }
    Ok(path)
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

fn control_value(text: &str, section: &str, index: usize) -> Result<Value> {
    if text.starts_with("0x") || text.starts_with("Func_") {
        return Ok(Value::String(text.into()));
    }
    text.parse::<i64>()
        .map(Value::from)
        .map_err(|_| format!("audio control {section} row {index} value differs"))
}

fn build_control_file(path: &Path) -> Result<(Vec<u8>, PathBuf)> {
    let text = fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
    let mut lines = text.lines().filter(|line| !line.starts_with('#'));
    if lines.next() != Some("section\tindex\tvalue") {
        return error("audio-control table header differs");
    }
    let mut source = Map::new();
    source.insert("format".into(), Value::from(1));
    source.insert(
        "kind".into(),
        Value::String("golden-sun-audio-engine-control".into()),
    );
    source.insert("address".into(), Value::String(hex(AUDIO_ENGINE_ADDRESS)));
    source.insert("end".into(), Value::String(hex(CONTROL_END)));
    for (section, count) in CONTROL_SECTIONS {
        let mut values = Vec::with_capacity(count);
        for index in 0..count {
            let fields = lines
                .next()
                .ok_or_else(|| format!("audio control {section} row {index} is absent"))?
                .split('\t')
                .collect::<Vec<_>>();
            if fields.len() != 3 || fields[0] != section || fields[1] != index.to_string() {
                return error(format!(
                    "audio control {section} row {index} identity differs"
                ));
            }
            values.push(control_value(fields[2], section, index)?);
        }
        if matches!(section, "leading_alignment" | "wait_alignment") {
            let fill = values
                .first()
                .and_then(Value::as_i64)
                .ok_or_else(|| format!("audio control {section} fill differs"))?;
            if values.iter().any(|value| value.as_i64() != Some(fill)) {
                return error(format!("audio control {section} fill is inconsistent"));
            }
            let address = if section == "leading_alignment" {
                AUDIO_ENGINE_ADDRESS
            } else {
                0x080f_ba45
            };
            source.insert(
                section.into(),
                serde_json::json!({"address":hex(address),"size":count,"fill":fill}),
            );
        } else {
            source.insert(section.into(), Value::Array(values));
        }
    }
    if lines.next().is_some() {
        return error("audio-control table has extra rows");
    }
    Ok((build_control(&source)?, canonical_path(path)?))
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

fn sample_source_addresses(path: &Path) -> Result<std::collections::BTreeMap<String, String>> {
    let text = fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
    let mut rows = text.lines().filter(|line| !line.starts_with('#'));
    if rows.next() != Some("sample\taddress\tfrequency\tloop_start\tsample_count\tsource") {
        return error("direct-sound sample table header differs");
    }
    let mut addresses = std::collections::BTreeMap::new();
    for (sample, row) in rows.enumerate() {
        let fields = row.split('\t').collect::<Vec<_>>();
        if fields.len() != 6 || fields[0] != sample.to_string() {
            return error(format!("direct-sound sample {sample} identity differs"));
        }
        if addresses
            .insert(fields[5].into(), fields[1].into())
            .is_some()
        {
            return error("direct-sound sample source is duplicated");
        }
    }
    Ok(addresses)
}

fn build_tone_files(index: &Path) -> Result<(Vec<u8>, Vec<PathBuf>)> {
    let text =
        fs::read_to_string(index).map_err(|error| format!("{}: {error}", index.display()))?;
    let mut lines = text.lines().filter(|line| !line.starts_with('#'));
    if lines.next() != Some("bank\taddress\tsource") {
        return error("voice-group index header differs");
    }
    let sound = index
        .parent()
        .and_then(Path::parent)
        .ok_or("voice-group index location differs")?;
    let sample_index = sound.join("samples.tsv");
    let sample_addresses = sample_source_addresses(&sample_index)?;
    let mut output = Vec::with_capacity(WAVEFORM_ADDRESS - BANK_0_ADDRESS);
    let mut sources = vec![canonical_path(index)?, canonical_path(&sample_index)?];
    for bank in 0..2 {
        let base = if bank == 0 {
            BANK_0_ADDRESS
        } else {
            BANK_1_ADDRESS
        };
        let count = if bank == 0 { 144 } else { 81 };
        let relative = format!("voicegroups/voicegroup_{bank:03}.tsv");
        let fields = lines
            .next()
            .ok_or_else(|| format!("voice group {bank} is absent"))?
            .split('\t')
            .collect::<Vec<_>>();
        if fields.len() != 3
            || fields[0] != bank.to_string()
            || fields[1] != hex(base)
            || fields[2] != relative
        {
            return error(format!("voice group {bank} identity differs"));
        }
        let path = canonical_path(&sound.join(&relative))?;
        let text =
            fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?;
        let mut records = text.lines().filter(|line| !line.starts_with('#'));
        if records.next()
            != Some("program\tkind\tfixed_pitch\tkey\tlength\tpan_sweep\tsource\tattack\tdecay\tsustain\trelease")
        {
            return error(format!("voice group {bank} header differs"));
        }
        for program in 0..count {
            let fields = records
                .next()
                .ok_or_else(|| format!("voice group {bank} program {program} is absent"))?
                .split('\t')
                .collect::<Vec<_>>();
            if fields.len() != 11 || fields[0] != program.to_string() {
                return error(format!(
                    "voice group {bank} program {program} identity differs"
                ));
            }
            let number = |field: usize, label: &str| {
                fields[field]
                    .parse::<u8>()
                    .map_err(|_| format!("voice group {bank} program {program} {label} differs"))
            };
            let key = number(3, "key")?;
            let length = number(4, "length")?;
            let pan_sweep = number(5, "pan/sweep")?;
            let envelope = [
                number(7, "attack")?,
                number(8, "decay")?,
                number(9, "sustain")?,
                number(10, "release")?,
            ];
            let fixed_pitch = match fields[2] {
                "true" => true,
                "false" => false,
                _ => {
                    return error(format!(
                        "voice group {bank} program {program} fixed pitch differs"
                    ))
                }
            };
            let record = match fields[1] {
                "pcm" => {
                    let address = sample_addresses
                        .get(fields[6])
                        .cloned()
                        .or_else(|| fields[6].strip_prefix("embedded_pcm_").map(str::to_string))
                        .ok_or_else(|| {
                            format!("voice group {bank} program {program} sample differs")
                        })?;
                    serde_json::json!({"kind":"pcm","fixed_pitch":fixed_pitch,"key":key,"length":length,"pan_sweep":pan_sweep,"sample":address,"envelope":envelope})
                }
                "wave" => {
                    let waveform = Path::new(fields[6])
                        .file_stem()
                        .and_then(|name| name.to_str())
                        .ok_or_else(|| {
                            format!("voice group {bank} program {program} waveform differs")
                        })?;
                    serde_json::json!({"kind":"wave","fixed_pitch":fixed_pitch,"key":key,"length":length,"pan_sweep":pan_sweep,"waveform":waveform,"envelope":envelope})
                }
                "rhythm" => {
                    let (target_bank, target_program) = fields[6]
                        .strip_prefix("voicegroup_")
                        .and_then(|value| value.split_once(':'))
                        .ok_or_else(|| {
                            format!("voice group {bank} program {program} rhythm table differs")
                        })?;
                    let target_bank = target_bank.parse::<u8>().map_err(|_| {
                        format!("voice group {bank} program {program} rhythm bank differs")
                    })?;
                    let target_program = target_program.parse::<u8>().map_err(|_| {
                        format!("voice group {bank} program {program} rhythm program differs")
                    })?;
                    let tones = format!("bank_{target_bank}_{target_program:03}");
                    serde_json::json!({"kind":"rhythm","key":key,"length":length,"pan_sweep":pan_sweep,"tones":tones,"key_map":null})
                }
                "pulse_1" | "pulse_2" | "noise" => {
                    let generator = number(6, "generator")?;
                    serde_json::json!({"kind":fields[1],"fixed_pitch":fixed_pitch,"key":key,"length":length,"pan_sweep":pan_sweep,"generator":generator,"envelope":envelope})
                }
                _ => return error(format!("voice group {bank} program {program} kind differs")),
            };
            output.extend(build_tone_record(
                &record,
                &format!("voice group {bank} program {program}"),
            )?);
        }
        if records.next().is_some() {
            return error(format!("voice group {bank} has extra rows"));
        }
        sources.push(path);
    }
    if lines.next().is_some() || output.len() != WAVEFORM_ADDRESS - BANK_0_ADDRESS {
        return error("voice-group index extent differs");
    }
    Ok((output, sources))
}

fn build_waveform_files(index: &Path) -> Result<(Vec<u8>, Vec<PathBuf>)> {
    let text =
        fs::read_to_string(index).map_err(|error| format!("{}: {error}", index.display()))?;
    let mut lines = text.lines().filter(|line| !line.starts_with('#'));
    if lines.next() != Some("wave\taddress\tsource") {
        return error("CGB waveform table header differs");
    }
    let root = index.parent().unwrap_or_else(|| Path::new("."));
    let mut data = Vec::with_capacity(18 * 16);
    let mut sources = vec![canonical_path(index)?];
    for wave in 0..18 {
        let line = lines
            .next()
            .ok_or_else(|| format!("CGB waveform {wave} is absent"))?;
        let fields = line.split('\t').collect::<Vec<_>>();
        let expected_source = format!("programmable_wave_samples/wave_{wave:02}.pcm4");
        if fields.len() != 3
            || fields[0] != wave.to_string()
            || fields[1] != hex(WAVEFORM_ADDRESS + wave * 16)
            || fields[2] != expected_source
        {
            return error(format!("CGB waveform {wave} identity differs"));
        }
        let source = canonical_path(&root.join(&expected_source))?;
        let bytes = fs::read(&source).map_err(|error| format!("{}: {error}", source.display()))?;
        if bytes.len() != 16 {
            return error(format!("CGB waveform {wave} extent differs"));
        }
        data.extend(bytes);
        sources.push(source);
    }
    if lines.next().is_some() {
        return error("CGB waveform table has extra rows");
    }
    Ok((data, sources))
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

fn build_player_file(path: &Path) -> Result<(Vec<u8>, PathBuf)> {
    let text = fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
    let mut lines = text.lines().filter(|line| !line.starts_with('#'));
    if lines.next() != Some("player\tstate\ttrack_storage\tmax_tracks") {
        return error("music-player table header differs");
    }
    let mut players = Vec::with_capacity(8);
    for player in 0..8 {
        let fields = lines
            .next()
            .ok_or_else(|| format!("music player {player} is absent"))?
            .split('\t')
            .collect::<Vec<_>>();
        if fields.len() != 4 || fields[0] != player.to_string() {
            return error(format!("music player {player} identity differs"));
        }
        players.push(serde_json::json!({
            "name":format!("player_{player}"),
            "state":fields[1],
            "track_storage":fields[2],
            "max_tracks":fields[3].parse::<u8>().map_err(|_| format!("music player {player} track count differs"))?,
        }));
    }
    if lines.next().is_some() {
        return error("music-player table has extra rows");
    }
    let source = serde_json::json!({
        "format":1,
        "kind":"golden-sun-music-players",
        "address":hex(PLAYER_ADDRESS),
        "end":hex(AUDIO_ENGINE_END),
        "players":players,
    });
    Ok((
        build_players(source.as_object().unwrap())?,
        canonical_path(path)?,
    ))
}

#[derive(Debug, Clone)]
pub struct BuiltAudioEngineData {
    pub address: usize,
    pub data: Vec<u8>,
    pub sources: Vec<PathBuf>,
}
pub fn build_audio_engine_data(index_path: &Path) -> Result<BuiltAudioEngineData> {
    let paths = engine_paths(index_path)?;
    let (control, control_source) = build_control_file(&paths[0])?;
    let (tones, tone_sources) = build_tone_files(&paths[1])?;
    let (waveforms, waveform_sources) = build_waveform_files(&paths[2])?;
    let (players, player_source) = build_player_file(&paths[3])?;
    let data = concat([Ok(control), Ok(tones), Ok(waveforms), Ok(players)])?;
    if data.len() != AUDIO_ENGINE_SIZE {
        return error("audio-engine package size differs");
    }
    let mut all = vec![canonical_path(index_path)?];
    all.extend([control_source, player_source]);
    all.extend(tone_sources);
    all.extend(waveform_sources);
    Ok(BuiltAudioEngineData {
        address: AUDIO_ENGINE_ADDRESS,
        data,
        sources: all,
    })
}
