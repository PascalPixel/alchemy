use canonical_json::is_canonical_json_text;
use serde_json::{Map, Value};
use std::collections::BTreeSet;
use std::fs;
use std::path::{Path, PathBuf};
pub type Result<T> = std::result::Result<T, String>;
pub const AUDIO_ENGINE_ADDRESS: usize = 0x080f_b792;
pub const AUDIO_ENGINE_END: usize = 0x080f_c684;
pub const AUDIO_ENGINE_SIZE: usize = AUDIO_ENGINE_END - AUDIO_ENGINE_ADDRESS;
const CONTROL_END: usize = 0x080f_ba78;
const BANK_0_ADDRESS: usize = 0x080f_ba78;
const BANK_1_ADDRESS: usize = 0x080f_c138;
const WAVEFORM_ADDRESS: usize = 0x080f_c504;
const PLAYER_ADDRESS: usize = 0x080f_c624;
const SOURCE_NAMES: [&str; 4] = ["seigyo.json", "onshoku.json", "hakei.json", "saisei.json"];
const USAGE: &str = "usage: audio-engine-data build-stdout INDEX";
fn error<T>(message: impl Into<String>) -> Result<T> {
    Err(message.into())
}
fn hex(value: usize) -> String {
    format!("0x{value:08x}")
}
fn canonical_document(path: &Path, label: &str) -> Result<Value> {
    let bytes = fs::read(path).map_err(|e| format!("{}: {e}", path.display()))?;
    let text = String::from_utf8_lossy(&bytes);
    let value: Value = serde_json::from_str(&text).map_err(|e| format!("{label}: invalid JSON: {e}"))?;
    if !is_canonical_json_text(&text, &value) {
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
    object.get(key).ok_or_else(|| format!("missing field {key}"))
}
fn integer(value: &Value, minimum: i64, maximum: i64, label: &str) -> Result<i64> {
    let Some(number) = value.as_f64() else {
        return error(format!("{label} is outside its range"));
    };
    if !number.is_finite() || number.fract() != 0.0 || number < minimum as f64 || number > maximum as f64 {
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
        || !text[2..].bytes().all(|b| b.is_ascii_hexdigit() && !b.is_ascii_uppercase())
    {
        return error(format!("{label} is not a canonical address"));
    }
    let parsed = usize::from_str_radix(&text[2..], 16).map_err(|_| format!("{label} is outside its address range"))?;
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
        || !text[5..].bytes().all(|b| b.is_ascii_hexdigit() && !b.is_ascii_uppercase())
    {
        return error(format!("{label} is not a function symbol"));
    }
    let code = u32::from_str_radix(&text[5..], 16).map_err(|_| format!("{label} is not a function symbol"))?;
    if code < 0x080f_9000 || code >= AUDIO_ENGINE_ADDRESS as u32 || code % 2 != 0 {
        return error(format!("{label} lies outside the audio engine"));
    }
    Ok(code | 1)
}
fn alignment(value: &Value, expected_address: usize, expected_size: usize, label: &str) -> Result<Vec<u8>> {
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
            || !text[2..].bytes().all(|b| b.is_ascii_hexdigit() && !b.is_ascii_uppercase())
        {
            return error(format!("{label} {index} is not a word"));
        }
        let value = u32::from_str_radix(&text[2..], 16).map_err(|_| format!("{label} {index} is not a word"))?;
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
    exact_keys(index_object, &["format", "kind", "address", "end", "size", "sources"], "audio-engine index")?;
    if integer(field(index_object, "format")?, 0, i64::MAX, "audio-engine format")? != 1
        || field(index_object, "kind")?.as_str() != Some("golden-sun-audio-engine-data")
        || field(index_object, "address")?.as_str() != Some(&hex(AUDIO_ENGINE_ADDRESS))
        || field(index_object, "end")?.as_str() != Some(&hex(AUDIO_ENGINE_END))
        || integer(field(index_object, "size")?, 0, i64::MAX, "audio-engine size")? != AUDIO_ENGINE_SIZE as i64
    {
        return error("audio-engine index extent differs");
    }
    let sources = object(field(index_object, "sources")?, "audio-engine sources")?;
    exact_keys(sources, &["control", "tones", "waveforms", "players"], "audio-engine sources")?;
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
    let relative = path.strip_prefix(&root).map_err(|_| "audio-engine source escaped its directory".to_string())?;
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
fn check_extent(object: &Map<String, Value>, kind: &str, address: usize, end: usize, label: &str) -> Result<()> {
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
    let ratios = words(field(source, "direct_frequency_ratios")?, 12, "direct frequency ratios")?;
    for index in 0..12 {
        let value = u32::from_le_bytes(ratios[index * 4..index * 4 + 4].try_into().unwrap());
        let previous =
            if index == 0 { 0 } else { u32::from_le_bytes(ratios[(index - 1) * 4..index * 4].try_into().unwrap()) };
        if value & 0x8000_0000 == 0 || (index > 0 && value <= previous) {
            return error("direct frequency ratios are not increasing fixed-point values");
        }
    }
    let samples = unsigned_halfwords(field(source, "pcm_samples_per_vblank")?, 12, "PCM samples per VBlank")?;
    for index in 0..12 {
        let value = u16::from_le_bytes(samples[index * 2..index * 2 + 2].try_into().unwrap());
        let previous =
            if index == 0 { 0 } else { u16::from_le_bytes(samples[(index - 1) * 2..index * 2].try_into().unwrap()) };
        if value == 0 || (index > 0 && value <= previous) {
            return error("PCM sample counts are not increasing");
        }
    }
    let waits = unsigned_bytes(field(source, "wait_durations")?, 49, "wait durations")?;
    if waits.windows(2).any(|pair| pair[1] < pair[0]) {
        return error("wait durations are not ordered");
    }
    let output = concat([
        alignment(field(source, "leading_alignment")?, AUDIO_ENGINE_ADDRESS, 2, "leading alignment"),
        Ok(diagnostic_bytes),
        dispatch(field(source, "command_dispatch")?, 36, "command dispatch"),
        unsigned_bytes(field(source, "direct_pitch_codes")?, 180, "direct pitch codes"),
        Ok(ratios),
        Ok(samples),
        unsigned_bytes(field(source, "cgb_pitch_codes")?, 132, "CGB pitch codes"),
        signed_halfwords(field(source, "cgb_frequency_steps")?, 12, "CGB frequency steps"),
        unsigned_bytes(field(source, "noise_pitch_codes")?, 60, "noise pitch codes"),
        unsigned_bytes(field(source, "cgb_volume_registers")?, 16, "CGB volume registers"),
        Ok(waits),
        alignment(field(source, "wait_alignment")?, 0x080f_ba45, 3, "wait alignment"),
        dispatch(field(source, "cgb_command_dispatch")?, 12, "CGB command dispatch"),
    ])?;
    if output.len() != CONTROL_END - AUDIO_ENGINE_ADDRESS {
        return error("audio-engine control size differs");
    }
    Ok(output)
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
    let record = symbol[7..].parse::<usize>().map_err(|_| "rhythm tone symbol differs".to_string())?;
    let count = if bank == 0 { 144 } else { 81 };
    if record >= count {
        return error("rhythm tone symbol lies outside its bank");
    }
    Ok((if bank == 0 { BANK_0_ADDRESS } else { BANK_1_ADDRESS }) + record * 12)
}
fn waveform_symbol(index: usize) -> String {
    format!("wave_{index:02}")
}
fn waveform_address(value: &Value) -> Result<usize> {
    let Some(symbol) = value.as_str() else {
        return error("waveform symbol differs");
    };
    if symbol.len() != 7 || !symbol.starts_with("wave_") || !symbol[5..].bytes().all(|b| b.is_ascii_digit()) {
        return error("waveform symbol differs");
    }
    let index = symbol[5..].parse::<usize>().map_err(|_| "waveform symbol differs".to_string())?;
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
        Some("pcm") => &["kind", "fixed_pitch", "key", "length", "pan_sweep", "sample", "envelope"][..],
        Some("wave") => &["kind", "fixed_pitch", "key", "length", "pan_sweep", "waveform", "envelope"][..],
        _ => &["kind", "fixed_pitch", "key", "length", "pan_sweep", "generator", "envelope"][..],
    };
    exact_keys(object, keys, label)?;
    let mut output = vec![0; 12];
    output[1] = integer(field(object, "key")?, 0, 255, &format!("{label} key"))? as u8;
    output[2] = integer(field(object, "length")?, 0, 255, &format!("{label} length"))? as u8;
    output[3] = integer(field(object, "pan_sweep")?, 0, 255, &format!("{label} pan/sweep"))? as u8;
    if kind == Some("rhythm") {
        if !field(object, "key_map")?.is_null() {
            return error(format!("{label} rhythm key map is not absent"));
        }
        output[0] = 0x80;
        output[4..8].copy_from_slice(&(tone_address(field(object, "tones")?)? as u32).to_le_bytes());
        return Ok(output);
    }
    let kind = kind.ok_or_else(|| format!("{label} tone kind differs"))?;
    let base = [("pcm", 0), ("pulse_1", 1), ("pulse_2", 2), ("wave", 3), ("noise", 4)]
        .iter()
        .find(|(name, _)| *name == kind)
        .map(|(_, base)| *base)
        .ok_or_else(|| format!("{label} tone kind differs"))?;
    let fixed = field(object, "fixed_pitch")?.as_bool().ok_or_else(|| format!("{label} fixed-pitch flag differs"))?;
    let type_byte = base | if fixed { 8 } else { 0 };
    if ![0, 1, 8, 9, 10, 11, 12].contains(&type_byte) {
        return error(format!("{label} tone type is not used by this package"));
    }
    let pointer = if kind == "pcm" {
        address(field(object, "sample")?, &format!("{label} sample"), 0x080f_d048, 0x0815_fb77)?
    } else if kind == "wave" {
        waveform_address(field(object, "waveform")?)?
    } else {
        integer(field(object, "generator")?, 0, 3, &format!("{label} generator"))? as usize
    };
    output[0] = type_byte as u8;
    output[4..8].copy_from_slice(&(pointer as u32).to_le_bytes());
    output[8..12].copy_from_slice(&unsigned_bytes(field(object, "envelope")?, 4, &format!("{label} envelope"))?);
    Ok(output)
}
fn read_tones(path: &Path) -> Result<Map<String, Value>> {
    let object = read_source(path, "audio tone banks", &["format", "kind", "address", "end", "banks"])?;
    check_extent(&object, "golden-sun-audio-tone-banks", BANK_0_ADDRESS, WAVEFORM_ADDRESS, "audio tone banks")?;
    Ok(object)
}
fn build_tones(source: &Map<String, Value>) -> Result<Vec<u8>> {
    let banks = array(field(source, "banks")?, 2, "tone banks")?;
    let mut output = Vec::new();
    for (bank, bank_value) in banks.iter().enumerate() {
        let item = object(bank_value, &format!("tone bank {bank}"))?;
        exact_keys(item, &["name", "address", "records"], &format!("tone bank {bank}"))?;
        let base = if bank == 0 { BANK_0_ADDRESS } else { BANK_1_ADDRESS };
        let count = if bank == 0 { 144 } else { 81 };
        if field(item, "name")?.as_str() != Some(&format!("bank_{bank}"))
            || field(item, "address")?.as_str() != Some(&hex(base))
        {
            return error(format!("tone bank {bank} identity differs"));
        }
        for (index, record) in
            array(field(item, "records")?, count, &format!("tone bank {bank} records"))?.iter().enumerate()
        {
            output.extend(build_tone_record(record, &format!("tone bank {bank} record {index}"))?);
        }
    }
    if output.len() != WAVEFORM_ADDRESS - BANK_0_ADDRESS {
        return error("audio tone-bank size differs");
    }
    Ok(output)
}
fn read_waveforms(path: &Path) -> Result<Map<String, Value>> {
    let object = read_source(path, "CGB waveforms", &["format", "kind", "address", "end", "waveforms"])?;
    check_extent(&object, "golden-sun-cgb-waveforms", WAVEFORM_ADDRESS, PLAYER_ADDRESS, "CGB waveforms")?;
    Ok(object)
}
fn build_waveforms(source: &Map<String, Value>) -> Result<Vec<u8>> {
    let mut output = vec![0; 18 * 16];
    for (index, item) in array(field(source, "waveforms")?, 18, "CGB waveforms")?.iter().enumerate() {
        let object = object(item, &format!("CGB waveform {index}"))?;
        exact_keys(object, &["name", "samples"], &format!("CGB waveform {index}"))?;
        if field(object, "name")?.as_str() != Some(&waveform_symbol(index)) {
            return error(format!("CGB waveform {index} name differs"));
        }
        for (sample, value) in
            array(field(object, "samples")?, 32, &format!("CGB waveform {index} samples"))?.iter().enumerate()
        {
            let value = integer(value, 0, 15, &format!("CGB waveform {index} sample {sample}"))? as u8;
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
    let object = read_source(path, "music players", &["format", "kind", "address", "end", "players"])?;
    check_extent(&object, "golden-sun-music-players", PLAYER_ADDRESS, AUDIO_ENGINE_END, "music players")?;
    Ok(object)
}
fn ewram_address(value: &Value, label: &str) -> Result<usize> {
    address(value, label, 0x0200_0000, 0x0203_ffff)
}
fn build_players(source: &Map<String, Value>) -> Result<Vec<u8>> {
    let mut output = vec![0; 8 * 12];
    let mut states = BTreeSet::new();
    let mut tracks = BTreeSet::new();
    for (index, item) in array(field(source, "players")?, 8, "music players")?.iter().enumerate() {
        let object = object(item, &format!("music player {index}"))?;
        exact_keys(object, &["name", "state", "track_storage", "max_tracks"], &format!("music player {index}"))?;
        if field(object, "name")?.as_str() != Some(&format!("player_{index}")) {
            return error(format!("music player {index} name differs"));
        }
        let state = ewram_address(field(object, "state")?, &format!("music player {index} state"))?;
        let storage = ewram_address(field(object, "track_storage")?, &format!("music player {index} track storage"))?;
        if !states.insert(state) || !tracks.insert(storage) {
            return error("music-player storage must be distinct");
        }
        output[index * 12..index * 12 + 4].copy_from_slice(&(state as u32).to_le_bytes());
        output[index * 12 + 4..index * 12 + 8].copy_from_slice(&(storage as u32).to_le_bytes());
        output[index * 12 + 8] =
            integer(field(object, "max_tracks")?, 1, 16, &format!("music player {index} max tracks"))? as u8;
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
        child(index_path, field(sources, "control")?.as_str().unwrap_or_default())?,
        child(index_path, field(sources, "tones")?.as_str().unwrap_or_default())?,
        child(index_path, field(sources, "waveforms")?.as_str().unwrap_or_default())?,
        child(index_path, field(sources, "players")?.as_str().unwrap_or_default())?,
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
    Ok(BuiltAudioEngineData { address: AUDIO_ENGINE_ADDRESS, data, sources: all })
}
pub fn run(args: Vec<String>) -> Result<Option<String>> {
    if args == ["-h"] || args == ["--help"] {
        return Ok(Some(USAGE.into()));
    }
    if args.len() == 2 && args[0] == "build-stdout" {
        let built = build_audio_engine_data(Path::new(&args[1]))?;
        std::io::Write::write_all(&mut std::io::stdout(), &built.data).map_err(|error| error.to_string())?;
        return Ok(None);
    }
    error(USAGE)
}
