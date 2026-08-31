pub mod cli;
pub mod smsh;

use std::collections::{HashMap, HashSet};

pub type Error = String;
pub type Result<T> = std::result::Result<T, Error>;
pub const MIDI_BUILD_DIRECTIVE: &[u8] = b"alchemy-mid2agb\0";
const SMSH_TEMPO_CLOCK: u64 = 120_547_500;

fn midi_variable(mut value: usize) -> Vec<u8> {
    let mut bytes = vec![(value & 0x7f) as u8];
    while {
        value >>= 7;
        value != 0
    } {
        bytes.push(((value & 0x7f) as u8) | 0x80);
    }
    bytes.reverse();
    bytes
}

fn read_midi_variable(bytes: &[u8], cursor: &mut usize) -> Result<usize> {
    let mut value = 0usize;
    for _ in 0..4 {
        let byte = *bytes
            .get(*cursor)
            .ok_or_else(|| "MIDI variable-length number is truncated".to_string())?;
        *cursor += 1;
        value = (value << 7) | usize::from(byte & 0x7f);
        if byte & 0x80 == 0 {
            return Ok(value);
        }
    }
    Err("MIDI variable-length number is too long".into())
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct MidiNoteEvent {
    pub track: usize,
    pub tick: usize,
    pub note: u8,
    pub velocity: u8,
    pub on: bool,
}

pub fn normalized_midi_notes(midi: &[u8]) -> Result<Vec<MidiNoteEvent>> {
    if midi.len() < 14 || &midi[..4] != b"MThd" {
        return Err("MIDI header is missing".into());
    }
    let header = u32::from_be_bytes(midi[4..8].try_into().unwrap()) as usize;
    let mut cursor = 8usize
        .checked_add(header)
        .filter(|end| *end <= midi.len())
        .ok_or_else(|| "MIDI header is truncated".to_string())?;
    let mut notes = Vec::new();
    let mut track = 0usize;
    while cursor < midi.len() {
        if midi.get(cursor..cursor + 4) != Some(b"MTrk") {
            return Err("MIDI track header is missing".into());
        }
        let length = u32::from_be_bytes(
            midi.get(cursor + 4..cursor + 8)
                .ok_or_else(|| "MIDI track length is truncated".to_string())?
                .try_into()
                .unwrap(),
        ) as usize;
        let end = cursor
            .checked_add(8 + length)
            .filter(|end| *end <= midi.len())
            .ok_or_else(|| "MIDI track is truncated".to_string())?;
        let bytes = &midi[cursor + 8..end];
        let mut at = 0usize;
        let mut tick = 0usize;
        let mut running = 0u8;
        while at < bytes.len() {
            tick = tick
                .checked_add(read_midi_variable(bytes, &mut at)?)
                .ok_or_else(|| "MIDI tick overflows".to_string())?;
            let raw = *bytes
                .get(at)
                .ok_or_else(|| "MIDI event is truncated".to_string())?;
            let status = if raw & 0x80 != 0 {
                at += 1;
                if raw < 0xf0 {
                    running = raw;
                }
                raw
            } else if (0x80..0xf0).contains(&running) {
                running
            } else {
                return Err("MIDI running status has no channel event".into());
            };
            if status == 0xff {
                at += 1;
                let size = read_midi_variable(bytes, &mut at)?;
                at = at
                    .checked_add(size)
                    .filter(|end| *end <= bytes.len())
                    .ok_or_else(|| "MIDI meta event is truncated".to_string())?;
                continue;
            }
            if matches!(status, 0xf0 | 0xf7) {
                let size = read_midi_variable(bytes, &mut at)?;
                at = at
                    .checked_add(size)
                    .filter(|end| *end <= bytes.len())
                    .ok_or_else(|| "MIDI system event is truncated".to_string())?;
                continue;
            }
            let size = if matches!(status >> 4, 0xc | 0xd) {
                1
            } else {
                2
            };
            let data = bytes
                .get(at..at + size)
                .ok_or_else(|| "MIDI channel event is truncated".to_string())?;
            at += size;
            if matches!(status >> 4, 0x8 | 0x9) {
                let on = status >> 4 == 0x9 && data[1] != 0;
                notes.push(MidiNoteEvent {
                    track,
                    tick,
                    note: data[0],
                    velocity: if on { data[1] } else { 0 },
                    on,
                });
            }
        }
        cursor = end;
        track += 1;
    }
    Ok(notes)
}

fn rewrite_track_tempos(track: &[u8]) -> Result<(Vec<u8>, usize)> {
    let mut output = Vec::with_capacity(track.len());
    let mut cursor = 0usize;
    let mut running = 0u8;
    let mut inserted = 0usize;
    while cursor < track.len() {
        let event_start = cursor;
        read_midi_variable(track, &mut cursor)?;
        let body_start = cursor;
        let raw_status = *track
            .get(cursor)
            .ok_or_else(|| "MIDI event is truncated".to_string())?;
        let explicit = raw_status & 0x80 != 0;
        let status = if explicit {
            cursor += 1;
            if raw_status < 0xf0 {
                running = raw_status;
            }
            raw_status
        } else if (0x80..0xf0).contains(&running) {
            running
        } else {
            return Err("MIDI running status has no channel event".into());
        };
        let mut native_tempo = None;
        if status == 0xff {
            let kind = *track
                .get(cursor)
                .ok_or_else(|| "MIDI meta event is truncated".to_string())?;
            cursor += 1;
            let length = read_midi_variable(track, &mut cursor)?;
            let end = cursor
                .checked_add(length)
                .filter(|end| *end <= track.len())
                .ok_or_else(|| "MIDI meta payload is truncated".to_string())?;
            if kind == 0x06 {
                if let Ok(serde_json::Value::Array(marker)) =
                    serde_json::from_slice::<serde_json::Value>(&track[cursor..end])
                {
                    if marker.first().and_then(serde_json::Value::as_str) == Some("tempo") {
                        native_tempo = marker.get(1).and_then(serde_json::Value::as_u64);
                    }
                }
            }
            cursor = end;
        } else if status == 0xf0 || status == 0xf7 {
            let length = read_midi_variable(track, &mut cursor)?;
            cursor = cursor
                .checked_add(length)
                .filter(|end| *end <= track.len())
                .ok_or_else(|| "MIDI system-exclusive payload is truncated".to_string())?;
        } else {
            let data = match status >> 4 {
                0x8 | 0x9 | 0xa | 0xb | 0xe => 2,
                0xc | 0xd => 1,
                _ => return Err("unsupported MIDI event".into()),
            };
            let consumed = usize::from(explicit);
            cursor = body_start
                .checked_add(consumed + data)
                .filter(|end| *end <= track.len())
                .ok_or_else(|| "MIDI channel event is truncated".to_string())?;
        }
        if let Some(native) = native_tempo.filter(|value| *value != 0) {
            output.extend_from_slice(&track[event_start..body_start]);
            output.extend_from_slice(&[0xff, 0x51, 0x03]);
            let tempo = u32::try_from((SMSH_TEMPO_CLOCK + native / 2) / native)
                .map_err(|_| "SMSH tempo is outside MIDI range".to_string())?;
            if tempo > 0x00ff_ffff {
                return Err("SMSH tempo is outside MIDI range".into());
            }
            output.extend_from_slice(&tempo.to_be_bytes()[1..]);
            output.push(0);
            output.extend_from_slice(&track[body_start..cursor]);
            inserted += 1;
        } else {
            output.extend_from_slice(&track[event_start..cursor]);
        }
    }
    Ok((output, inserted))
}

pub fn add_smsh_midi_tempos(midi: &[u8]) -> Result<Vec<u8>> {
    if midi.len() < 14 || &midi[..4] != b"MThd" {
        return Err("MIDI header is missing".into());
    }
    if midi.windows(3).any(|bytes| bytes == [0xff, 0x51, 0x03]) {
        return Ok(midi.to_vec());
    }
    let header = u32::from_be_bytes(midi[4..8].try_into().unwrap()) as usize;
    let mut cursor = 8usize
        .checked_add(header)
        .filter(|end| *end <= midi.len())
        .ok_or_else(|| "MIDI header is truncated".to_string())?;
    let mut output = midi[..cursor].to_vec();
    let mut inserted = 0usize;
    while cursor < midi.len() {
        if midi.get(cursor..cursor + 4) != Some(b"MTrk") {
            return Err("MIDI track header is missing".into());
        }
        let length = u32::from_be_bytes(
            midi.get(cursor + 4..cursor + 8)
                .ok_or_else(|| "MIDI track length is truncated".to_string())?
                .try_into()
                .unwrap(),
        ) as usize;
        let end = cursor
            .checked_add(8 + length)
            .filter(|end| *end <= midi.len())
            .ok_or_else(|| "MIDI track is truncated".to_string())?;
        let (track, count) = rewrite_track_tempos(&midi[cursor + 8..end])?;
        output.extend_from_slice(b"MTrk");
        output.extend_from_slice(
            &u32::try_from(track.len())
                .map_err(|_| "MIDI track is too large".to_string())?
                .to_be_bytes(),
        );
        output.extend_from_slice(&track);
        inserted += count;
        cursor = end;
    }
    if inserted == 0 {
        return Err("MIDI has no native SMSH tempo markers".into());
    }
    Ok(output)
}

pub fn add_midi_build_directive(midi: &[u8], directive: &[u8]) -> Result<Vec<u8>> {
    if midi.len() < 26 || &midi[..4] != b"MThd" {
        return Err("MIDI header is missing".into());
    }
    let header = u32::from_be_bytes(midi[4..8].try_into().unwrap()) as usize;
    let track = 8 + header;
    if midi.get(track..track + 4) != Some(b"MTrk") {
        return Err("MIDI conductor track is missing".into());
    }
    let length = u32::from_be_bytes(midi[track + 4..track + 8].try_into().unwrap()) as usize;
    let end = track + 8 + length;
    if end > midi.len() || midi.get(end - 4..end) != Some(&[0, 0xff, 0x2f, 0]) {
        return Err("MIDI conductor end is not canonical".into());
    }
    if midi
        .windows(MIDI_BUILD_DIRECTIVE.len())
        .any(|part| part == MIDI_BUILD_DIRECTIVE)
    {
        return Err("MIDI already has build directives".into());
    }
    let mut event = vec![0, 0xff, 0x7f];
    event.extend(midi_variable(MIDI_BUILD_DIRECTIVE.len() + directive.len()));
    event.extend_from_slice(MIDI_BUILD_DIRECTIVE);
    event.extend_from_slice(directive);
    let new_length =
        u32::try_from(length + event.len()).map_err(|_| "MIDI conductor is too large")?;
    let mut output = Vec::with_capacity(midi.len() + event.len());
    output.extend_from_slice(&midi[..track + 4]);
    output.extend_from_slice(&new_length.to_be_bytes());
    output.extend_from_slice(&midi[track + 8..end - 4]);
    output.extend_from_slice(&event);
    output.extend_from_slice(&midi[end - 4..]);
    Ok(output)
}

pub fn add_midi_conductor_text(midi: &[u8], text: &[u8]) -> Result<Vec<u8>> {
    if midi.len() < 26 || &midi[..4] != b"MThd" {
        return Err("MIDI header is missing".into());
    }
    let header = u32::from_be_bytes(midi[4..8].try_into().unwrap()) as usize;
    let track = 8 + header;
    if midi.get(track..track + 4) != Some(b"MTrk") {
        return Err("MIDI conductor track is missing".into());
    }
    let length = u32::from_be_bytes(midi[track + 4..track + 8].try_into().unwrap()) as usize;
    let end = track + 8 + length;
    if end > midi.len() || midi.get(end - 4..end) != Some(&[0, 0xff, 0x2f, 0]) {
        return Err("MIDI conductor end is not canonical".into());
    }
    let mut event = vec![0, 0xff, 0x01];
    event.extend(midi_variable(text.len()));
    event.extend_from_slice(text);
    let new_length =
        u32::try_from(length + event.len()).map_err(|_| "MIDI conductor is too large")?;
    let mut output = Vec::with_capacity(midi.len() + event.len());
    output.extend_from_slice(&midi[..track + 4]);
    output.extend_from_slice(&new_length.to_be_bytes());
    output.extend_from_slice(&midi[track + 8..end - 4]);
    output.extend_from_slice(&event);
    output.extend_from_slice(&midi[end - 4..]);
    Ok(output)
}
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct NamedTrack {
    pub game: String,
    pub sound_id: u16,
    pub name: String,
    pub titles: [String; 6],
    pub source: String,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct SharedSequence {
    pub sound_id: u16,
    pub gs1: u16,
    pub gs2: u16,
    pub evidence: String,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct MusicCatalog {
    pub tracks: Vec<NamedTrack>,
    pub shared: Vec<SharedSequence>,
}

impl MusicCatalog {
    pub fn english_title(&self, game: &str, sound_id: u16) -> Option<&str> {
        let direct = |game: &str, sound_id: u16| {
            self.tracks
                .iter()
                .find(|track| track.game == game && track.sound_id == sound_id)
                .map(|track| track.titles[0].as_str())
                .filter(|title| !title.is_empty())
        };
        direct(game, sound_id).or_else(|| {
            let shared = self.shared.iter().find(|shared| match game {
                "gs1" => shared.gs1 == sound_id,
                "gs2" => shared.gs2 == sound_id,
                _ => false,
            })?;
            match game {
                "gs1" => direct("gs2", shared.gs2),
                "gs2" => direct("gs1", shared.gs1),
                _ => None,
            }
        })
    }
}

pub fn parse_music_catalog(text: &str) -> Result<MusicCatalog> {
    let mut catalog = MusicCatalog {
        tracks: Vec::new(),
        shared: Vec::new(),
    };
    let mut rows = text.lines().filter(|line| !line.starts_with('#'));
    if rows.next()
        != Some("kind\tgame\tsound_id\tname\ten\tde\tes\tfr\tit\tja\tsource\tgs1\tgs2\tevidence")
    {
        return Err("music catalog header differs".into());
    }
    for (index, line) in rows.enumerate() {
        let mut fields = line.split('\t').collect::<Vec<_>>();
        if fields.first() == Some(&"track") && fields.len() == 11 {
            fields.resize(14, "");
        }
        if fields.len() != 14 {
            return Err(format!(
                "music catalog row {} has the wrong width",
                index + 2
            ));
        }
        let number = |field: usize| {
            fields[field]
                .parse::<u16>()
                .map_err(|_| format!("music catalog row {} has an invalid number", index + 2))
        };
        match fields[0] {
            "track" => {
                let track = NamedTrack {
                    game: fields[1].into(),
                    sound_id: number(2)?,
                    name: fields[3].into(),
                    titles: [
                        fields[4].into(),
                        fields[5].into(),
                        fields[6].into(),
                        fields[7].into(),
                        fields[8].into(),
                        fields[9].into(),
                    ],
                    source: fields[10].into(),
                };
                if !matches!(track.game.as_str(), "gs1" | "gs2")
                    || track.name.is_empty()
                    || track.source.is_empty()
                    || track.titles.iter().all(String::is_empty)
                    || !track.name.bytes().all(|byte| {
                        byte.is_ascii_lowercase() || byte.is_ascii_digit() || byte == b'-'
                    })
                    || fields[11..].iter().any(|field| !field.is_empty())
                {
                    return Err(format!(
                        "music catalog row {} has an invalid track identity",
                        index + 2
                    ));
                }
                if catalog
                    .tracks
                    .iter()
                    .any(|item| item.game == track.game && item.sound_id == track.sound_id)
                {
                    return Err(format!(
                        "music catalog row {} duplicates a track identity",
                        index + 2
                    ));
                }
                catalog.tracks.push(track);
            }
            "shared" => {
                if !fields[1].is_empty()
                    || fields[3..11].iter().any(|field| !field.is_empty())
                    || fields[13].is_empty()
                {
                    return Err(format!(
                        "music catalog row {} has invalid shared-sequence evidence",
                        index + 2
                    ));
                }
                let shared = SharedSequence {
                    sound_id: number(2)?,
                    gs1: number(11)?,
                    gs2: number(12)?,
                    evidence: fields[13].into(),
                };
                if catalog.shared.iter().any(|item| {
                    item.sound_id == shared.sound_id
                        || item.gs1 == shared.gs1
                        || item.gs2 == shared.gs2
                }) {
                    return Err(format!(
                        "music catalog row {} duplicates a shared sequence",
                        index + 2
                    ));
                }
                catalog.shared.push(shared);
            }
            _ => {
                return Err(format!(
                    "music catalog row {} has an unknown kind",
                    index + 2
                ))
            }
        }
    }
    Ok(catalog)
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct SoundTableEntry {
    pub header: String,
    pub player: i64,
}

#[derive(Debug, Clone, PartialEq)]
pub struct SoundTableSource {
    pub format: i64,
    pub fields: Vec<String>,
    pub auxiliary: String,
    pub symbols: Vec<(String, SymbolValue)>,
    pub entries: Vec<SoundTableEntry>,
}

#[derive(Debug, Clone, PartialEq)]
pub enum SymbolValue {
    Text(String),
    Number(f64),
}

#[derive(Debug, Clone, PartialEq)]
pub struct SoundTableReport {
    pub entries: usize,
    pub unique_headers: usize,
    pub players: Vec<(String, u64)>,
    pub mirrored_auxiliary: bool,
}

pub fn js_number(text: &str) -> f64 {
    let t = text.trim();
    if t.is_empty() {
        return 0.0;
    }
    let (sign, body) = match t.strip_prefix('-') {
        Some(rest) => (-1.0, rest),
        None => (1.0, t.strip_prefix('+').unwrap_or(t)),
    };
    let radix_parse = |digits: &str, radix: u32| -> f64 {
        if digits.is_empty() || !digits.chars().all(|c| c.is_digit(radix)) {
            return f64::NAN;
        }
        let mut acc = 0.0f64;
        for c in digits.chars() {
            acc = acc * f64::from(radix) + f64::from(c.to_digit(radix).unwrap());
        }
        acc
    };
    if let Some(d) = body.strip_prefix("0x").or_else(|| body.strip_prefix("0X")) {
        return if sign < 0.0 {
            f64::NAN
        } else {
            radix_parse(d, 16)
        };
    }
    if let Some(d) = body.strip_prefix("0o").or_else(|| body.strip_prefix("0O")) {
        return if sign < 0.0 {
            f64::NAN
        } else {
            radix_parse(d, 8)
        };
    }
    if let Some(d) = body.strip_prefix("0b").or_else(|| body.strip_prefix("0B")) {
        return if sign < 0.0 {
            f64::NAN
        } else {
            radix_parse(d, 2)
        };
    }
    if body == "Infinity" {
        return sign * f64::INFINITY;
    }
    match body.parse::<f64>() {
        Ok(v)
            if !body.eq_ignore_ascii_case("inf")
                && !body.eq_ignore_ascii_case("infinity")
                && !body.eq_ignore_ascii_case("nan") =>
        {
            sign * v
        }
        _ => f64::NAN,
    }
}

fn integer_from_str(value: &str, label: &str) -> Result<i64> {
    integer_from_f64(js_number(value), label)
}

fn integer_from_f64(result: f64, label: &str) -> Result<i64> {
    if !result.is_finite() || result != result.trunc() {
        return Err(format!("{label} must be an integer"));
    }
    Ok(result as i64)
}

fn u16_check(value: i64, label: &str) -> Result<u16> {
    if !(0..=0xffff).contains(&value) {
        return Err(format!("{label} does not fit u16"));
    }
    Ok(value as u16)
}

fn address_from_i64(result: i64, label: &str) -> Result<u32> {
    if !(0x0800_0000..0x0a00_0000).contains(&result) || result % 4 != 0 {
        return Err(format!("{label} is not an aligned ROM address"));
    }
    Ok(result as u32)
}

fn address(value: &SymbolValue, label: &str) -> Result<u32> {
    let result = match value {
        SymbolValue::Text(s) => integer_from_str(s, label)?,
        SymbolValue::Number(n) => integer_from_f64(*n, label)?,
    };
    address_from_i64(result, label)
}

fn valid_symbol(name: &str) -> bool {
    let mut chars = name.chars();
    match chars.next() {
        Some(c) if c.is_ascii_lowercase() => {}
        _ => return false,
    }
    chars.all(|c| c.is_ascii_lowercase() || c.is_ascii_digit() || c == '_')
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct SequenceBuildReport {
    pub base: u32,
    pub end: u32,
    pub bytes: usize,
    pub streams: usize,
    pub tracks: usize,
    pub events: usize,
}

pub fn build_reserve_sequence(base: u32) -> (Vec<u8>, SequenceBuildReport) {
    let data = vec![
        0xbe, 120, 0xbc, 0, 0xbb, 30, 0xbd, 21, 0xd0, 61, 127, 0x81, 66, 0x81, 0xee, 69, 0xa0, 0xb1,
    ];
    let report = SequenceBuildReport {
        base,
        end: base + data.len() as u32,
        bytes: data.len(),
        streams: 1,
        tracks: 0,
        events: 11,
    };
    (data, report)
}

pub fn build_sound_table(source: &SoundTableSource) -> Result<(Vec<u8>, SoundTableReport)> {
    if source.format != 1
        || source.fields != ["header", "player"]
        || source.auxiliary != "copy-player"
    {
        return Err("unsupported sound-table source".to_string());
    }
    let mut symbols: HashMap<&str, u32> = HashMap::new();
    let mut order: Vec<&str> = Vec::new();
    let mut addresses: HashSet<u32> = HashSet::new();
    for (name, value) in &source.symbols {
        if !valid_symbol(name) {
            return Err(format!("invalid sound symbol: {name}"));
        }
        let resolved = address(value, &format!("symbol {name}"))?;
        if addresses.contains(&resolved) {
            return Err(format!("duplicate sound-header address: {name}"));
        }
        if symbols.insert(name.as_str(), resolved).is_none() {
            order.push(name.as_str());
        }
        addresses.insert(resolved);
    }
    let mut used: HashSet<&str> = HashSet::new();
    let mut players: Vec<(String, u64)> = Vec::new();
    let mut built = vec![0u8; source.entries.len() * 8];
    for (index, entry) in source.entries.iter().enumerate() {
        let Some(&header) = symbols.get(entry.header.as_str()) else {
            return Err(format!("entry {index} references an unknown sound header"));
        };
        let label = format!("entry {index} player");
        let player = u16_check(integer_from_f64(entry.player as f64, &label)?, &label)?;
        let base = index * 8;
        built[base..base + 4].copy_from_slice(&header.to_le_bytes());
        built[base + 4..base + 6].copy_from_slice(&player.to_le_bytes());
        built[base + 6..base + 8].copy_from_slice(&player.to_le_bytes());
        used.insert(entry.header.as_str());
        let key = player.to_string();
        match players.iter_mut().find(|(k, _)| *k == key) {
            Some((_, count)) => *count += 1,
            None => players.push((key, 1)),
        }
    }
    let unused: Vec<&str> = order
        .iter()
        .copied()
        .filter(|name| !used.contains(name))
        .collect();
    if !unused.is_empty() {
        return Err(format!("unused sound symbols: {}", unused.join(", ")));
    }
    let report = SoundTableReport {
        entries: source.entries.len(),
        unique_headers: symbols.len(),
        players,
        mirrored_auxiliary: true,
    };
    Ok((built, report))
}

#[cfg(test)]
mod catalog_tests {
    use super::{add_smsh_midi_tempos, normalized_midi_notes, parse_music_catalog};

    fn midi_with_marker(marker: &[u8]) -> Vec<u8> {
        let mut track = vec![0, 0xff, 0x06, marker.len() as u8];
        track.extend_from_slice(marker);
        track.extend_from_slice(&[0, 0xff, 0x2f, 0]);
        let mut midi = b"MThd\0\0\0\x06\0\0\0\x01\0\x18MTrk".to_vec();
        midi.extend_from_slice(&(track.len() as u32).to_be_bytes());
        midi.extend_from_slice(&track);
        midi
    }

    #[test]
    fn adds_standard_tempo_without_removing_native_marker() {
        let midi = midi_with_marker(br#"["tempo",51]"#);
        let normalized = add_smsh_midi_tempos(&midi).unwrap();
        assert!(normalized
            .windows(6)
            .any(|part| part == [0xff, 0x51, 3, 0x24, 0x11, 0x1c]));
        assert!(normalized
            .windows(12)
            .any(|part| part == br#"["tempo",51]"#));
        assert_eq!(add_smsh_midi_tempos(&normalized).unwrap(), normalized);
    }

    #[test]
    fn normalizes_timed_notes_with_running_status() {
        let track = [
            0, 0x90, 60, 100, 1, 62, 80, 1, 0x80, 60, 64, 0, 62, 32, 0, 0xff, 0x2f, 0,
        ];
        let mut midi = b"MThd\0\0\0\x06\0\0\0\x01\0\x18MTrk".to_vec();
        midi.extend_from_slice(&(track.len() as u32).to_be_bytes());
        midi.extend_from_slice(&track);
        let notes = normalized_midi_notes(&midi).unwrap();
        assert_eq!(notes.len(), 4);
        assert_eq!(
            notes.iter().map(|event| event.tick).collect::<Vec<_>>(),
            [0, 1, 2, 2]
        );
        assert!(notes[0].on && notes[1].on);
        assert!(!notes[2].on && !notes[3].on);
        assert_eq!(notes[2].velocity, 0);
    }

    #[test]
    fn reads_named_and_shared_tracks_without_sidecars() {
        let source = "kind\tgame\tsound_id\tname\ten\tde\tes\tfr\tit\tja\tsource\tgs1\tgs2\tevidence\ntrack\tgs2\t0\tcoolin-casino\tCoolin' Casino\tCool Cat Casino\tCoolin' Casino\tCoolin' Casino\tShow Casinò\t\tbattle-mode-music-requester\t\t\t\nshared\t\t80\t\t\t\t\t\t\t\t\t80\t80\tidentical-normalized-note-events";
        let catalog = parse_music_catalog(source).unwrap();
        assert_eq!(catalog.tracks[0].name, "coolin-casino");
        assert_eq!(catalog.tracks[0].titles[4], "Show Casinò");
        assert_eq!(catalog.shared[0].gs1, 80);
    }

    #[test]
    fn permits_an_evidence_only_catalog_before_titles_are_supplied() {
        let source = "kind\tgame\tsound_id\tname\ten\tde\tes\tfr\tit\tja\tsource\tgs1\tgs2\tevidence\nshared\t\t80\t\t\t\t\t\t\t\t\t80\t80\tidentical-normalized-note-events\n";
        let catalog = parse_music_catalog(source).unwrap();
        assert!(catalog.tracks.is_empty());
        assert_eq!(catalog.shared.len(), 1);
    }

    #[test]
    fn shares_a_title_only_through_recorded_identity_evidence() {
        let source = "kind\tgame\tsound_id\tname\ten\tde\tes\tfr\tit\tja\tsource\tgs1\tgs2\tevidence\ntrack\tgs1\t80\ttrial-road\tTrial Road\t\t\t\t\t\tnintendo-music\t\t\t\nshared\t\t80\t\t\t\t\t\t\t\t\t80\t80\tcompare-midi-notes:64\n";
        let catalog = parse_music_catalog(source).unwrap();
        assert_eq!(catalog.english_title("gs1", 80), Some("Trial Road"));
        assert_eq!(catalog.english_title("gs2", 80), Some("Trial Road"));
        assert_eq!(catalog.english_title("gs2", 81), None);
    }

    #[test]
    fn rejects_duplicate_sound_identity() {
        let header =
            "kind\tgame\tsound_id\tname\ten\tde\tes\tfr\tit\tja\tsource\tgs1\tgs2\tevidence\n";
        let record = "track\tgs2\t0\tcoolin-casino\tCoolin' Casino\tCool Cat Casino\tCoolin' Casino\tCoolin' Casino\tShow Casinò\t\tbattle-mode-music-requester\t\t\t\n";
        assert!(parse_music_catalog(&format!("{header}{record}{record}"))
            .unwrap_err()
            .contains("duplicates a track identity"));
    }

    #[test]
    fn requires_sources_for_titles_and_evidence_for_shared_sequences() {
        let header =
            "kind\tgame\tsound_id\tname\ten\tde\tes\tfr\tit\tja\tsource\tgs1\tgs2\tevidence\n";
        let title_without_source =
            "track\tgs2\t0\tcoolin-casino\tCoolin' Casino\t\t\t\t\t\t\t\t\t\n";
        let shared_without_evidence = "shared\t\t80\t\t\t\t\t\t\t\t\t80\t80\t\n";
        assert!(
            parse_music_catalog(&format!("{header}{title_without_source}"))
                .unwrap_err()
                .contains("invalid track identity")
        );
        assert!(
            parse_music_catalog(&format!("{header}{shared_without_evidence}"))
                .unwrap_err()
                .contains("shared-sequence evidence")
        );
    }

    #[test]
    fn rejects_ambiguous_shared_sequence_mappings() {
        let header =
            "kind\tgame\tsound_id\tname\ten\tde\tes\tfr\tit\tja\tsource\tgs1\tgs2\tevidence\n";
        let first = "shared\t\t80\t\t\t\t\t\t\t\t\t80\t80\tidentical-normalized-note-events\n";
        let second = "shared\t\t82\t\t\t\t\t\t\t\t\t80\t82\tidentical-normalized-note-events\n";
        assert!(parse_music_catalog(&format!("{header}{first}{second}"))
            .unwrap_err()
            .contains("duplicates a shared sequence"));
    }
}
