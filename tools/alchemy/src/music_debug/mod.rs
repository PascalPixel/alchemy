//! Optional GS1/GS2 playback debugger; synthesis is not a fidelity claim.
use super::http::{self, root, Response};
use serde_json::{json, Value};
use std::{
    collections::{BTreeMap, BTreeSet},
    net::TcpListener,
    path::{Path, PathBuf},
};
const CLIENT: &str = include_str!("client.js");
const STYLES: &str = include_str!("style.css");
fn music_dir() -> PathBuf {
    root().join("games/gs1/sound/songs/midi")
}
fn audio_dir() -> PathBuf {
    root().join("games/gs1/sound")
}
fn gs2_sound_dir() -> PathBuf {
    root().join("games/gs2/sound")
}
fn music_register() -> PathBuf {
    root().join("games/music.tsv")
}

fn titles(text: &str) -> Result<BTreeMap<(String, u16), String>, String> {
    let mut rows = text.lines().filter(|line| !line.starts_with('#'));
    if rows.next()
        != Some("kind\tgame\tsound_id\tname\ten\tde\tes\tfr\tit\tja\tsource\tgs1\tgs2\tevidence")
    {
        return Err("music catalog header differs".into());
    }
    let mut named = BTreeMap::new();
    let mut shared = Vec::new();
    let mut seen = BTreeSet::new();
    for line in rows {
        let fields: Vec<_> = line.split('\t').collect();
        let id = |i: usize| {
            fields
                .get(i)
                .ok_or("missing music identity")?
                .parse::<u16>()
                .map_err(|_| "invalid music identity")
        };
        match fields.first().copied() {
            Some("track") if fields.len() == 11 || fields.len() == 14 => {
                if !matches!(fields[1], "gs1" | "gs2") || fields[10].is_empty() {
                    return Err("music title lacks game or source".into());
                }
                let key = (fields[1].to_string(), id(2)?);
                if !seen.insert(key.clone()) {
                    return Err("duplicate music title identity".into());
                }
                if !fields[4].is_empty() {
                    named.insert(key, fields[4].to_string());
                }
            }
            Some("shared") if fields.len() == 14 && !fields[13].is_empty() => {
                shared.push((id(11)?, id(12)?));
            }
            _ => return Err("music catalog row differs".into()),
        }
    }
    let direct = named.clone();
    for (gs1, gs2) in shared {
        for (key, other) in [
            (("gs1".into(), gs1), ("gs2".into(), gs2)),
            (("gs2".into(), gs2), ("gs1".into(), gs1)),
        ] {
            if let Some(title) = direct.get(&other) {
                named.entry(key).or_insert_with(|| title.clone());
            }
        }
    }
    Ok(named)
}
fn music_catalog() -> Result<Vec<u8>, String> {
    let register = std::fs::read_to_string(music_register())
        .map_err(|error| format!("music register: {error}"))?;
    let catalog = titles(&register)?;
    let sequences = std::fs::read_to_string(audio_dir().join("sequences.tsv"))
        .map_err(|error| format!("GS1 music sequences: {error}"))?;
    let mut tracks = sequences
        .lines()
        .filter(|line| !line.starts_with('#'))
        .skip(1)
        .filter_map(|line| {
            let fields = line.split('\t').collect::<Vec<_>>();
            (fields.get(1) == Some(&"music")).then_some(fields)
        })
        .map(|fields| {
            if fields.len() != 5 {
                return Err("GS1 music sequence row width differs".to_string());
            }
            let sound_id = fields[0]
                .parse::<u16>()
                .map_err(|_| "GS1 music sequence ID differs".to_string())?;
            let file = Path::new(fields[4])
                .file_name()
                .and_then(|name| name.to_str())
                .ok_or("GS1 music sequence filename differs")?
                .to_string();
            let title = catalog
                .get(&("gs1".to_string(), sound_id))
                .map(String::as_str);
            Ok(json!({
                "key": format!("gs1:{sound_id}"),
                "game": "gs1",
                "gameTitle": "Golden Sun",
                "soundId": sound_id,
                "file": file,
                "path": format!("/music/{file}"),
                "available": true,
                "status": "byte-exact",
                "title": title.map(str::to_owned).unwrap_or_else(|| "Untitled".to_string()),
                "source": format!("Golden Sun · ROM sequence {sound_id:03}")
            }))
        })
        .collect::<Result<Vec<_>, _>>()?;
    if tracks.len() != 58 {
        return Err("GS1 music catalog does not expose all 58 music sequences".into());
    }
    let requests = std::fs::read_to_string(gs2_sound_dir().join("music_requests.tsv"))
        .map_err(|error| format!("GS2 music requests: {error}"))?;
    let requests = requests
        .lines()
        .filter(|line| !line.starts_with('#'))
        .skip(1)
        .map(|line| {
            let fields = line.split('\t').collect::<Vec<_>>();
            if fields.len() != 5 {
                return Err("GS2 music request row width differs".to_string());
            }
            let request = fields[0]
                .parse::<u16>()
                .map_err(|_| "GS2 music request number differs".to_string())?;
            let sound_id = fields[1]
                .parse::<u16>()
                .map_err(|_| "GS2 music request sound ID differs".to_string())?;
            let status = fields[4].to_string();
            let file = (status == "byte-exact")
                .then(|| {
                    Path::new(fields[3])
                        .file_name()?
                        .to_str()
                        .map(str::to_string)
                })
                .flatten();
            Ok((request, sound_id, fields[2].to_string(), file, status))
        })
        .collect::<Result<Vec<_>, _>>()?;
    if requests.len() != 97
        || requests
            .iter()
            .enumerate()
            .any(|(index, row)| usize::from(row.0) != index)
    {
        return Err("GS2 music request register is not the complete ordered 0-96 set".into());
    }
    tracks.extend(
        requests
            .into_iter()
            .map(|(request, sound_id, header, file, status)| {
                let title = catalog
                    .get(&("gs2".to_string(), sound_id))
                    .map(String::as_str);
                json!({
                    "key": format!("gs2:{request}"),
                    "game": "gs2",
                    "gameTitle": "Golden Sun: The Lost Age",
                    "request": request,
                    "soundId": sound_id,
                    "header": header,
                    "file": file,
                    "path": file.as_ref().map(|file| format!("/music/gs2/{file}")),
                    "available": file.is_some(),
                    "status": status,
                    "title": title.map(str::to_owned).unwrap_or_else(|| "Untitled".to_string()),
                    "source": format!("Golden Sun: The Lost Age · ROM sequence {sound_id}")
                })
            }),
    );
    serde_json::to_vec(&tracks).map_err(|error| error.to_string())
}
fn read_programmable_waveforms(
    sound: &Path,
    count: usize,
    digits: usize,
    base: u32,
) -> Result<Vec<Value>, String> {
    let path = sound.join("waveforms.tsv");
    let text =
        std::fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?;
    let mut lines = text.lines().filter(|line| !line.starts_with('#'));
    if lines.next() != Some("wave\taddress\tsource") {
        return Err(format!("{}: waveform table header differs", path.display()));
    }
    let mut waveforms = Vec::with_capacity(count);
    for index in 0..count {
        let fields = lines
            .next()
            .ok_or_else(|| format!("waveform {index} is absent"))?
            .split('\t')
            .collect::<Vec<_>>();
        let name = format!("wave_{index:0digits$}");
        let source = format!("programmable_wave_samples/{name}.pcm4");
        let address = fields
            .get(1)
            .and_then(|value| u32::from_str_radix(value.trim_start_matches("0x"), 16).ok());
        if fields.len() != 3
            || fields[0] != index.to_string()
            || address != Some(base + index as u32 * 16)
            || fields[2] != source
        {
            return Err(format!("waveform {index} identity differs"));
        }
        let data = std::fs::read(sound.join(&source))
            .map_err(|error| format!("waveform {index}: {error}"))?;
        if data.len() != 16 {
            return Err(format!("waveform {index} extent differs"));
        }
        waveforms.push(json!({
            "name": name,
            "samples": data.into_iter().flat_map(|byte| [byte >> 4, byte & 15]).collect::<Vec<_>>()
        }));
    }
    if lines.next().is_some() {
        return Err("waveform table has extra rows".into());
    }
    Ok(waveforms)
}

fn read_gs1_samples(sound: &Path) -> Result<Vec<Value>, String> {
    let path = sound.join("samples.tsv");
    let text =
        std::fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?;
    let mut lines = text.lines().filter(|line| !line.starts_with('#'));
    if lines.next() != Some("sample\taddress\tfrequency\tloop_start\tsample_count\tsource") {
        return Err("GS1 sample table header differs".into());
    }
    let mut samples = Vec::with_capacity(32);
    for sample in 0..32 {
        let fields = lines
            .next()
            .ok_or_else(|| format!("GS1 sample {sample} is absent"))?
            .split('\t')
            .collect::<Vec<_>>();
        if fields.len() != 6 || fields[0] != sample.to_string() {
            return Err(format!("GS1 sample {sample} identity differs"));
        }
        let count = fields[4]
            .parse::<usize>()
            .map_err(|_| format!("GS1 sample {sample} extent differs"))?;
        samples.push(json!({
            "name": format!("wave_{sample:02}"),
            "address": fields[1],
            "size": (16 + count + 3) & !3,
            "frequency": fields[2].parse::<u32>().map_err(|_| format!("GS1 sample {sample} frequency differs"))?,
            "loop_start": if fields[3].is_empty() { Value::Null } else { json!(fields[3].parse::<u32>().map_err(|_| format!("GS1 sample {sample} loop differs"))?) },
            "source": Path::new(fields[5]).file_name().and_then(|name| name.to_str()).ok_or_else(|| format!("GS1 sample {sample} source differs"))?
        }));
    }
    if lines.next().is_some() {
        return Err("GS1 sample table has extra rows".into());
    }
    Ok(samples)
}

fn read_gs1_voicegroup(sound: &Path, samples: &[Value]) -> Result<Vec<Value>, String> {
    let sample_by_source = samples
        .iter()
        .filter_map(|sample| {
            Some((
                format!("direct_sound_samples/{}", sample.get("source")?.as_str()?),
                sample.get("address")?.as_str()?,
            ))
        })
        .collect::<BTreeMap<_, _>>();
    let path = sound.join("voicegroups/voicegroup_000.tsv");
    let text =
        std::fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?;
    let mut lines = text.lines().filter(|line| !line.starts_with('#'));
    if lines.next()
        != Some("program\tkind\tfixed_pitch\tkey\tlength\tpan_sweep\tsource\tattack\tdecay\tsustain\trelease")
    {
        return Err("GS1 voice-group header differs".into());
    }
    let mut bank = Vec::with_capacity(144);
    for program in 0..144 {
        let fields = lines
            .next()
            .ok_or_else(|| format!("GS1 voice {program} is absent"))?
            .split('\t')
            .collect::<Vec<_>>();
        if fields.len() != 11 || fields[0] != program.to_string() {
            return Err(format!("GS1 voice {program} identity differs"));
        }
        let number = |field: usize, label: &str| {
            fields[field]
                .parse::<u8>()
                .map_err(|_| format!("GS1 voice {program} {label} differs"))
        };
        let mut tone = json!({
            "kind": fields[1],
            "fixed_pitch": fields[2] == "true",
            "key": number(3, "key")?,
            "length": number(4, "length")?,
            "pan_sweep": number(5, "pan")?,
            "envelope": [number(7, "attack")?, number(8, "decay")?, number(9, "sustain")?, number(10, "release")?]
        });
        match fields[1] {
            "pcm" => {
                tone["sample"] = json!(sample_by_source
                    .get(fields[6])
                    .copied()
                    .or_else(|| fields[6].strip_prefix("embedded_pcm_"))
                    .ok_or_else(|| format!("GS1 voice {program} sample differs"))?)
            }
            "wave" => {
                tone["waveform"] = json!(Path::new(fields[6])
                    .file_stem()
                    .and_then(|name| name.to_str())
                    .ok_or_else(|| format!("GS1 voice {program} waveform differs"))?)
            }
            "rhythm" => {
                let (bank_number, program_number) = fields[6]
                    .strip_prefix("voicegroup_")
                    .and_then(|value| value.split_once(':'))
                    .ok_or_else(|| format!("GS1 voice {program} rhythm table differs"))?;
                tone["tones"] = json!(format!(
                    "bank_{}_{:03}",
                    bank_number
                        .parse::<u8>()
                        .map_err(|_| format!("GS1 voice {program} rhythm bank differs"))?,
                    program_number
                        .parse::<u8>()
                        .map_err(|_| format!("GS1 voice {program} rhythm program differs"))?
                ));
            }
            "pulse_1" | "pulse_2" | "noise" => tone["generator"] = json!(number(6, "generator")?),
            _ => return Err(format!("GS1 voice {program} kind differs")),
        }
        bank.push(tone);
    }
    if lines.next().is_some() {
        return Err("GS1 voice group has extra rows".into());
    }
    Ok(bank)
}

fn soundfont() -> Result<Vec<u8>, String> {
    let samples = read_gs1_samples(&audio_dir())?;
    let waveforms = read_programmable_waveforms(&audio_dir(), 18, 2, 0x080f_c504)?;
    let residuals_path = root().join("games/gs1/assets/data/final_byte_regions_index.json");
    let residuals: Value = serde_json::from_slice(
        &std::fs::read(&residuals_path)
            .map_err(|error| format!("{}: {error}", residuals_path.display()))?,
    )
    .map_err(|error| format!("{}: {error}", residuals_path.display()))?;
    let bank = read_gs1_voicegroup(&audio_dir(), &samples)?;
    let mut sample_addresses = samples
        .iter()
        .filter_map(|sample| sample.get("address").and_then(Value::as_str))
        .map(str::to_string)
        .collect::<BTreeSet<_>>();
    let embedded_region = residuals
        .get("regions")
        .or_else(|| residuals.get("entries"))
        .and_then(Value::as_array)
        .or_else(|| residuals.as_array())
        .and_then(|regions| {
            regions
                .iter()
                .find(|region| region.get("address").and_then(Value::as_str) == Some("0x0811dac8"))
        })
        .ok_or("embedded PCM region is absent")?;
    let values = embedded_region
        .get("values")
        .and_then(Value::as_array)
        .ok_or("embedded PCM bytes are absent")?;
    let byte = |offset: usize| {
        values
            .get(offset)
            .and_then(Value::as_u64)
            .map(|value| value as u8)
            .ok_or("embedded PCM byte is absent")
    };
    let mut embedded_addresses = bank
        .iter()
        .filter(|tone| tone.get("kind").and_then(Value::as_str) == Some("pcm"))
        .filter_map(|tone| tone.get("sample").and_then(Value::as_str))
        .filter_map(|address| u32::from_str_radix(address.trim_start_matches("0x"), 16).ok())
        .filter(|address| (0x0811_dac8..0x0811_db38).contains(address))
        .collect::<BTreeSet<_>>()
        .into_iter()
        .collect::<Vec<_>>();
    embedded_addresses.push(0x0811_db38);
    let mut embedded_samples = Vec::new();
    for pair in embedded_addresses.windows(2) {
        let address = pair[0];
        let offset = (address - 0x0811_dac8) as usize;
        let extent = (pair[1] - address) as usize;
        if extent <= 16 {
            return Err("embedded PCM record is truncated".into());
        }
        let word = |at: usize| -> Result<u32, String> {
            Ok(u32::from_le_bytes([
                byte(offset + at)?,
                byte(offset + at + 1)?,
                byte(offset + at + 2)?,
                byte(offset + at + 3)?,
            ]))
        };
        let address_text = format!("0x{address:08x}");
        sample_addresses.insert(address_text.clone());
        embedded_samples.push(json!({
            "address": address_text,
            "control": word(0)?,
            "frequency": word(4)?,
            "loop_start": word(8)?,
            "samples": (16..extent).map(|at| byte(offset + at).map(Value::from)).collect::<Result<Vec<_>, _>>()?
        }));
    }
    let waveform_names = waveforms
        .iter()
        .filter_map(|waveform| waveform.get("name").and_then(Value::as_str))
        .collect::<BTreeSet<_>>();
    for (program, tone) in bank.iter().enumerate() {
        match tone.get("kind").and_then(Value::as_str) {
            Some("pcm") if !sample_addresses.contains(tone["sample"].as_str().unwrap_or("")) => {
                return Err(format!(
                    "music program {program} names an unknown PCM sample"
                ));
            }
            Some("wave") if !waveform_names.contains(tone["waveform"].as_str().unwrap_or("")) => {
                return Err(format!(
                    "music program {program} names an unknown CGB waveform"
                ));
            }
            Some("pcm" | "pulse_1" | "pulse_2" | "wave" | "noise" | "rhythm") => {}
            _ => return Err(format!("music program {program} has an unknown tone kind")),
        }
    }
    for program in [
        8, 24, 33, 45, 46, 47, 48, 52, 56, 61, 68, 72, 73, 75, 80, 81, 82, 83, 84, 89, 90, 91, 93,
    ] {
        if bank.get(program).is_none() {
            return Err(format!("recovered BGM program {program} is absent"));
        }
    }
    if bank[127].get("tones").and_then(Value::as_str) != Some("bank_0_092")
        || (128..=143).any(|record| bank.get(record).is_none())
    {
        return Err("recovered BGM percussion subtable is incomplete".into());
    }
    serde_json::to_vec(&json!({
        "format": 1,
        "engine": "golden-sun-rom-audio-bank",
        "bank": bank,
        "samples": samples,
        "embedded_samples": embedded_samples,
        "waveforms": waveforms
    }))
    .map_err(|error| error.to_string())
}

fn gs2_soundfont() -> Result<Vec<u8>, String> {
    let sound = gs2_sound_dir();
    let samples_text = std::fs::read_to_string(sound.join("samples.tsv"))
        .map_err(|error| format!("GS2 samples: {error}"))?;
    let mut samples = Vec::new();
    let mut sample_by_source = BTreeMap::new();
    for line in samples_text
        .lines()
        .filter(|line| !line.starts_with('#'))
        .skip(1)
    {
        let fields = line.split('\t').collect::<Vec<_>>();
        if fields.len() != 6 {
            return Err("GS2 sample row width differs".into());
        }
        let source = Path::new(fields[5])
            .file_name()
            .and_then(|name| name.to_str())
            .ok_or("GS2 sample filename differs")?;
        sample_by_source.insert(fields[5], fields[1]);
        samples.push(json!({
            "address": fields[1],
            "frequency": fields[2].parse::<u32>().map_err(|_| "GS2 sample frequency differs")?,
            "loop_start": if fields[3].is_empty() { Value::Null } else { json!(fields[3].parse::<u32>().map_err(|_| "GS2 sample loop differs")?) },
            "source": source
        }));
    }
    let waveforms = read_programmable_waveforms(&sound, 9, 3, 0x081c_4440)?;
    let tones_text = std::fs::read_to_string(sound.join("voicegroups/voicegroup_000.tsv"))
        .map_err(|error| format!("GS2 voice group: {error}"))?;
    let mut bank = Vec::new();
    for line in tones_text
        .lines()
        .filter(|line| !line.starts_with('#'))
        .skip(1)
    {
        let fields = line.split('\t').collect::<Vec<_>>();
        if fields.len() != 11 {
            return Err("GS2 voice row width differs".into());
        }
        let mut tone = json!({
            "kind": fields[1], "fixed_pitch": fields[2] == "true",
            "key": fields[3].parse::<u8>().map_err(|_| "GS2 voice key differs")?,
            "length": fields[4].parse::<u8>().map_err(|_| "GS2 voice length differs")?,
            "pan_sweep": fields[5].parse::<u8>().map_err(|_| "GS2 voice pan differs")?,
            "envelope": fields[7..11].iter().map(|value| value.parse::<u8>()).collect::<Result<Vec<_>, _>>().map_err(|_| "GS2 voice envelope differs")?
        });
        match fields[1] {
            "pcm" => {
                tone["sample"] = json!(sample_by_source
                    .get(fields[6])
                    .ok_or("GS2 voice names an unknown sample")?)
            }
            "wave" => {
                tone["waveform"] = json!(Path::new(fields[6])
                    .file_stem()
                    .and_then(|name| name.to_str())
                    .ok_or("GS2 waveform name differs")?)
            }
            "rhythm" => tone["tones"] = json!("bank_0_092"),
            "pulse_1" | "pulse_2" | "noise" => {
                tone["generator"] = json!(fields[6]
                    .parse::<u8>()
                    .map_err(|_| "GS2 generator differs")?)
            }
            _ => return Err("GS2 voice kind differs".into()),
        }
        bank.push(tone);
    }
    if bank.len() != 145 || samples.len() != 76 || waveforms.len() != 9 {
        return Err("GS2 soundfont inventory differs".into());
    }
    serde_json::to_vec(&json!({
        "format": 1, "engine": "golden-sun-rom-audio-bank", "game": "gs2",
        "bank": bank, "samples": samples, "embedded_samples": [], "waveforms": waveforms
    }))
    .map_err(|error| error.to_string())
}

fn music_file(path: &str) -> Option<PathBuf> {
    let name = path.strip_prefix("/music/")?;
    let number = name.strip_prefix("bgm_")?.strip_suffix(".mid")?;
    (number.len() == 3 && number.bytes().all(|byte| byte.is_ascii_digit()))
        .then(|| music_dir().join(name))
}
fn sample_file(path: &str) -> Option<PathBuf> {
    let name = path.strip_prefix("/music/samples/")?;
    let number = name.strip_prefix("wave_")?.strip_suffix(".pcm8.wav")?;
    (number.len() == 2 && number.bytes().all(|byte| byte.is_ascii_digit()))
        .then(|| audio_dir().join("direct_sound_samples").join(name))
}
fn gs2_music_file(path: &str) -> Option<PathBuf> {
    let name = path.strip_prefix("/music/gs2/")?;
    let number = name.strip_prefix("sound_")?.strip_suffix(".mid")?;
    (number.len() == 3 && number.bytes().all(|byte| byte.is_ascii_digit()))
        .then(|| gs2_sound_dir().join("songs/midi").join(name))
}
fn gs2_sample_file(path: &str) -> Option<PathBuf> {
    let name = path.strip_prefix("/music/gs2/samples/")?;
    let number = name.strip_prefix("sample_")?.strip_suffix(".pcm8.wav")?;
    (number.len() == 3 && number.bytes().all(|byte| byte.is_ascii_digit()))
        .then(|| gs2_sound_dir().join("direct_sound_samples").join(name))
}

fn response(path: &str) -> Response {
    match path {
        "/" => http::shell("Alchemy music debugger — approximate playback", STYLES),
        "/client.js" => Response::new(
            200,
            "OK",
            Some("text/javascript; charset=utf-8"),
            "no-store",
            CLIENT,
        ),
        "/music/catalog" => music_catalog()
            .map(|body| {
                Response::new(
                    200,
                    "OK",
                    Some("application/json; charset=utf-8"),
                    "no-store",
                    body,
                )
            })
            .unwrap_or_else(|error| {
                Response::new(
                    503,
                    "Service Unavailable",
                    Some("text/plain; charset=utf-8"),
                    "no-store",
                    error.into_bytes(),
                )
            }),
        "/music/soundfont" => soundfont()
            .map(|body| {
                Response::new(
                    200,
                    "OK",
                    Some("application/json; charset=utf-8"),
                    "public, max-age=300",
                    body,
                )
            })
            .unwrap_or_else(|error| {
                Response::new(
                    503,
                    "Service Unavailable",
                    Some("text/plain; charset=utf-8"),
                    "no-store",
                    error.into_bytes(),
                )
            }),
        "/music/gs2/soundfont" => gs2_soundfont()
            .map(|body| {
                Response::new(
                    200,
                    "OK",
                    Some("application/json; charset=utf-8"),
                    "public, max-age=300",
                    body,
                )
            })
            .unwrap_or_else(|error| {
                Response::new(
                    503,
                    "Service Unavailable",
                    Some("text/plain; charset=utf-8"),
                    "no-store",
                    error.into_bytes(),
                )
            }),
        "/weyard.otf" => http::font(),
        path if sample_file(path).is_some() => std::fs::read(sample_file(path).unwrap())
            .map(|body| Response::new(200, "OK", Some("audio/wav"), "public, max-age=300", body))
            .unwrap_or_else(|_| {
                Response::new(
                    404,
                    "Not Found",
                    Some("text/plain; charset=utf-8"),
                    "no-store",
                    b"Sample not found".to_vec(),
                )
            }),
        path if gs2_sample_file(path).is_some() => std::fs::read(gs2_sample_file(path).unwrap())
            .map(|body| Response::new(200, "OK", Some("audio/wav"), "public, max-age=300", body))
            .unwrap_or_else(|_| {
                Response::new(
                    404,
                    "Not Found",
                    Some("text/plain; charset=utf-8"),
                    "no-store",
                    b"Sample not found".to_vec(),
                )
            }),
        path if gs2_music_file(path).is_some() => std::fs::read(gs2_music_file(path).unwrap())
            .map(|body| Response::new(200, "OK", Some("audio/midi"), "public, max-age=300", body))
            .unwrap_or_else(|_| {
                Response::new(
                    404,
                    "Not Found",
                    Some("text/plain; charset=utf-8"),
                    "no-store",
                    b"Track not found".to_vec(),
                )
            }),
        path if music_file(path).is_some() => std::fs::read(music_file(path).unwrap())
            .map(|body| Response::new(200, "OK", Some("audio/midi"), "public, max-age=300", body))
            .unwrap_or_else(|_| {
                Response::new(
                    404,
                    "Not Found",
                    Some("text/plain; charset=utf-8"),
                    "no-store",
                    b"Track not found".to_vec(),
                )
            }),

        _ => http::not_found(),
    }
}
pub fn entry(args: &[String]) -> Result<(), String> {
    let Some(bind) = http::bind(args, "music-debug", 4651)? else {
        return Ok(());
    };
    let listener = TcpListener::bind(bind).map_err(|e| e.to_string())?;
    println!(
        "Alchemy music debugger (approximate playback) on http://{}/",
        listener.local_addr().map_err(|e| e.to_string())?
    );
    http::run(listener, response).map_err(|e| e.to_string())
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn isolated_routes_and_assets() {
        assert_eq!(response("/").status, 200);
        for path in [
            "/snapshot",
            "/events",
            "/svg/core",
            "/music/../Cargo.toml",
            "/music/gs2/samples/../../Cargo.toml",
        ] {
            assert_eq!(response(path).status, 404);
        }
        assert!(!CLIENT.contains("EventSource"));
        assert!(!CLIENT.contains("/snapshot"));
        assert!(CLIENT.contains("AudioContext"));
    }
    #[test]
    fn recovered_music_routes() -> Result<(), String> {
        let catalog = response("/music/catalog");
        if catalog.status != 200 || !catalog.body.starts_with(b"[") {
            return Err("music catalog route failed".into());
        }
        let catalog_json: Value = serde_json::from_slice(&catalog.body)
            .map_err(|error| format!("music catalog JSON: {error}"))?;
        let tracks = catalog_json
            .as_array()
            .filter(|tracks| tracks.len() == 155)
            .ok_or("music catalog does not expose both games")?;
        if tracks[0].get("game").and_then(Value::as_str) != Some("gs1")
            || tracks[0].get("soundId").and_then(Value::as_u64) != Some(0)
            || tracks[0].get("file").and_then(Value::as_str) != Some("bgm_000.mid")
            || tracks[57].get("game").and_then(Value::as_str) != Some("gs1")
            || tracks[57].get("soundId").and_then(Value::as_u64) != Some(93)
            || tracks[58].get("game").and_then(Value::as_str) != Some("gs2")
            || tracks[58].get("soundId").and_then(Value::as_u64) != Some(709)
            || tracks[59].get("soundId").and_then(Value::as_u64) != Some(68)
            || tracks[154].get("soundId").and_then(Value::as_u64) != Some(754)
            || tracks[62].get("file").and_then(Value::as_str) != Some("sound_001.mid")
        {
            return Err("music catalog does not preserve both ROM sequence inventories".into());
        }
        if tracks[58].get("title").and_then(Value::as_str) != Some("Try Your Luck!")
            || tracks[65].get("title").and_then(Value::as_str) != Some("Daila")
            || tracks[79].get("title").and_then(Value::as_str) != Some("Untitled")
            || tracks[154].get("title").and_then(Value::as_str) != Some("Battle with Dullahan")
            || tracks[..58]
                .iter()
                .filter(|track| track.get("title").and_then(Value::as_str) != Some("Untitled"))
                .count()
                != 15
            || tracks[58..]
                .iter()
                .filter(|track| track.get("title").and_then(Value::as_str) != Some("Untitled"))
                .count()
                != 96
            || tracks
                .iter()
                .any(|track| track.get("path").and_then(Value::as_str).is_none())
        {
            return Err("music catalog title evidence or recovered audio inventory differs".into());
        }
        for path in ["/music/soundfont", "/music/gs2/soundfont"] {
            let soundfont = response(path);
            if soundfont.status != 200
                || !String::from_utf8_lossy(&soundfont.body).contains("golden-sun-rom-audio-bank")
            {
                return Err(format!("music soundfont route failed: {path}"));
            }
        }
        if response("/music/gs2/samples/sample_000.pcm8.wav").status != 200
            || response("/music/gs2/samples/sample_076.pcm8.wav").status != 404
        {
            return Err("music sample route failed".into());
        }
        Ok(())
    }
    #[test]
    fn browser_regressions() {
        assert!(std::process::Command::new("bun")
            .args([
                "test",
                concat!(
                    env!("CARGO_MANIFEST_DIR"),
                    "/src/music_debug/client.test.js"
                )
            ])
            .status()
            .expect("Bun is required for music tests")
            .success());
    }
}
