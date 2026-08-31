use crate::Result;
use serde_json::{json, Value};
use std::collections::{BTreeMap, BTreeSet};

const ROM_BASE: u32 = 0x0800_0000;
const DURATIONS: [u8; 49] = [
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 28,
    30, 32, 36, 40, 42, 44, 48, 52, 54, 56, 60, 64, 66, 68, 72, 76, 78, 80, 84, 88, 90, 92, 96,
];

#[derive(Debug, Clone)]
struct TrackRange {
    label: String,
    start: u32,
    end: u32,
}

#[derive(Debug, Clone)]
pub struct ExtractedSequence {
    pub source: Value,
    pub base: u32,
    pub end: u32,
    pub tracks: usize,
    pub events: usize,
}

fn offset(rom: &[u8], address: u32, width: usize) -> Result<usize> {
    let start = address
        .checked_sub(ROM_BASE)
        .ok_or_else(|| format!("address 0x{address:08x} is below ROM"))? as usize;
    start
        .checked_add(width)
        .filter(|end| *end <= rom.len())
        .map(|_| start)
        .ok_or_else(|| format!("address 0x{address:08x} is outside ROM"))
}

fn byte(rom: &[u8], address: u32) -> Result<u8> {
    Ok(rom[offset(rom, address, 1)?])
}

fn word(rom: &[u8], address: u32) -> Result<u32> {
    let at = offset(rom, address, 4)?;
    Ok(u32::from_le_bytes(rom[at..at + 4].try_into().unwrap()))
}

fn control_name(opcode: u8) -> Option<&'static str> {
    Some(match opcode {
        0xba => "priority",
        0xbb => "tempo",
        0xbc => "key_shift",
        0xbd => "voice",
        0xbe => "volume",
        0xbf => "pan",
        0xc0 => "pitch_bend",
        0xc1 => "pitch_bend_range",
        0xc2 => "lfo_speed",
        0xc3 => "lfo_delay",
        0xc4 => "modulation_depth",
        0xc5 => "modulation_type",
        0xc8 => "tuning",
        _ => return None,
    })
}

fn parameter(name: &str, value: u8) -> Value {
    if matches!(name, "key_shift" | "pan" | "pitch_bend" | "tuning") {
        Value::from(i64::from(value as i8))
    } else {
        Value::from(value)
    }
}

fn target_label(labels: Option<&BTreeMap<u32, String>>, target: u32) -> Result<String> {
    labels
        .and_then(|labels| labels.get(&target))
        .cloned()
        .or_else(|| labels.is_none().then(String::new))
        .ok_or_else(|| format!("sequence pointer target 0x{target:08x} has no label"))
}

fn decode_track(
    rom: &[u8],
    range: &TrackRange,
    labels: Option<&BTreeMap<u32, String>>,
) -> Result<(Vec<Value>, Vec<(u32, u8)>)> {
    let mut events = Vec::new();
    let mut targets = Vec::new();
    let mut cursor = range.start;
    let mut running = None::<u8>;
    while cursor < range.end {
        if let Some(label) = labels.and_then(|labels| labels.get(&cursor)) {
            if cursor != range.start {
                events.push(json!(["label", label]));
            }
        }
        let raw = byte(rom, cursor)?;
        cursor += 1;
        let (opcode, first) = if raw < 0x80 {
            (
                running.ok_or_else(|| {
                    format!(
                        "{} starts running data at 0x{:08x} without a command",
                        range.label,
                        cursor - 1
                    )
                })?,
                Some(raw),
            )
        } else {
            (raw, None)
        };
        if (0x80..=0xb0).contains(&opcode) {
            events.push(json!(["wait", DURATIONS[usize::from(opcode - 0x80)]]));
            continue;
        }
        match opcode {
            0xb1 => events.push(json!(["fine"])),
            0xb2 | 0xb3 => {
                if first.is_some() {
                    return Err("branch command cannot use running status".into());
                }
                let target = word(rom, cursor)?;
                cursor += 4;
                targets.push((target, opcode));
                events.push(json!([
                    if opcode == 0xb2 { "goto" } else { "pattern" },
                    target_label(labels, target)?
                ]));
            }
            0xb4 => events.push(json!(["pattern_end"])),
            0xb5 => {
                if first.is_some() {
                    return Err("repeat command cannot use running status".into());
                }
                let count = byte(rom, cursor)?;
                let target = word(rom, cursor + 1)?;
                cursor += 5;
                targets.push((target, opcode));
                events.push(json!(["repeat", count, target_label(labels, target)?]));
            }
            0xce => {
                running = Some(opcode);
                if let Some(value) = first {
                    events.push(json!(["note_end_running", value]));
                } else if cursor < range.end && byte(rom, cursor)? < 0x80 {
                    let value = byte(rom, cursor)?;
                    cursor += 1;
                    events.push(json!(["note_end", value]));
                } else {
                    events.push(json!(["note_end"]));
                }
            }
            0xcf..=0xff => {
                let duration = DURATIONS[usize::from(opcode - 0xcf)];
                let mut values = Vec::new();
                if let Some(value) = first {
                    values.push(value);
                }
                while values.len() < 3 && cursor < range.end && byte(rom, cursor)? < 0x80 {
                    values.push(byte(rom, cursor)?);
                    cursor += 1;
                }
                let mut event = vec![
                    Value::from(if first.is_some() {
                        "note_running"
                    } else {
                        "note"
                    }),
                    Value::from(duration),
                ];
                event.extend(values.into_iter().map(Value::from));
                events.push(Value::Array(event));
                running = Some(opcode);
            }
            _ => {
                let name = control_name(opcode).ok_or_else(|| {
                    format!(
                        "unsupported sequence command 0x{opcode:02x} at 0x{:08x}",
                        cursor - 1
                    )
                })?;
                let value = if let Some(value) = first {
                    if opcode == 0xbb {
                        return Err("tempo cannot use running status".into());
                    }
                    value
                } else {
                    let value = byte(rom, cursor)?;
                    cursor += 1;
                    value
                };
                events.push(if first.is_some() {
                    json!(["control_running", name, parameter(name, value)])
                } else {
                    json!([name, parameter(name, value)])
                });
                if opcode != 0xbb {
                    running = Some(opcode);
                }
            }
        }
    }
    if cursor != range.end {
        return Err(format!("{} crosses its native extent", range.label));
    }
    Ok((events, targets))
}

pub fn extract_sequence(rom: &[u8], header: u32, name: &str) -> Result<ExtractedSequence> {
    let track_count = usize::from(byte(rom, header)?);
    if !(1..=16).contains(&track_count) {
        return Err(format!("sequence header has {track_count} tracks"));
    }
    let block_count = byte(rom, header + 1)?;
    let priority = byte(rom, header + 2)?;
    let reverb = byte(rom, header + 3)?;
    let tone_bank = word(rom, header + 4)?;
    let mut tracks = (0..track_count)
        .map(|index| {
            Ok((
                index,
                word(rom, header + 8 + u32::try_from(index * 4).unwrap())?,
            ))
        })
        .collect::<Result<Vec<_>>>()?;
    if tracks.iter().any(|(_, address)| *address >= header) {
        return Err("sequence track does not precede its header".into());
    }
    tracks.sort_by_key(|(_, address)| *address);
    if tracks.windows(2).any(|pair| pair[0].1 == pair[1].1) {
        return Err("sequence header aliases track pointers".into());
    }
    let base = tracks[0].1;
    let ranges = tracks
        .iter()
        .enumerate()
        .map(|(position, (original, start))| TrackRange {
            label: format!("track_{}", original + 1),
            start: *start,
            end: tracks
                .get(position + 1)
                .map_or(header, |(_, address)| *address),
        })
        .collect::<Vec<_>>();
    let mut references = Vec::new();
    for range in &ranges {
        references.extend(decode_track(rom, range, None)?.1);
    }
    let mut labels = ranges
        .iter()
        .map(|range| (range.start, range.label.clone()))
        .collect::<BTreeMap<_, _>>();
    let mut targets = references
        .iter()
        .map(|(target, _)| *target)
        .collect::<BTreeSet<_>>();
    for range in &ranges {
        let contained = targets
            .iter()
            .copied()
            .filter(|target| range.start < *target && *target < range.end)
            .collect::<Vec<_>>();
        for (index, target) in contained.into_iter().enumerate() {
            let pattern_only = references
                .iter()
                .filter(|(address, _)| *address == target)
                .all(|(_, opcode)| *opcode == 0xb3);
            labels.insert(
                target,
                format!(
                    "{}_{}_{}",
                    range.label,
                    if pattern_only { "pattern" } else { "loop" },
                    index + 1
                ),
            );
            targets.remove(&target);
        }
    }
    for target in targets {
        if !labels.contains_key(&target) {
            return Err(format!(
                "sequence branch target 0x{target:08x} is outside every track"
            ));
        }
    }
    let mut layout = Vec::new();
    let mut event_count = 0usize;
    for range in &ranges {
        let (events, _) = decode_track(rom, range, Some(&labels))?;
        event_count += events.len();
        layout.push(json!({"kind":"stream", "label":range.label, "events":events}));
    }
    let header_tracks = (0..track_count)
        .map(|index| Value::from(format!("track_{}", index + 1)))
        .collect::<Vec<_>>();
    layout.push(json!({
        "kind":"header",
        "label":name,
        "block_count":block_count,
        "priority":priority,
        "reverb":reverb,
        "tone_bank":"tone_bank",
        "tracks":header_tracks
    }));
    let end = header + 8 + u32::try_from(track_count * 4).unwrap();
    Ok(ExtractedSequence {
        source: json!({
            "format":1,
            "engine":"smsh-sequence",
            "base":format!("0x{base:08x}"),
            "externals":{"tone_bank":format!("0x{tone_bank:08x}")},
            "layout":layout
        }),
        base,
        end,
        tracks: track_count,
        events: event_count,
    })
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn extracts_priority_and_key_shift_running_status() {
        let mut rom = vec![0u8; 0x200];
        let track = 0x0800_0100u32;
        let header = 0x0800_010cu32;
        rom[0x100..0x10c].copy_from_slice(&[0xba, 5, 6, 0xbc, 0, 1, 0xbb, 30, 0xd0, 60, 100, 0xb1]);
        rom[0x10c..0x114].copy_from_slice(&[1, 0, 0, 0, 0x00, 0x20, 0x00, 0x08]);
        rom[0x114..0x118].copy_from_slice(&track.to_le_bytes());
        let extracted = extract_sequence(&rom, header, "sound_test").unwrap();
        assert_eq!(extracted.base, track);
        assert_eq!(extracted.end, 0x0800_0118);
        assert_eq!(
            extracted.source["layout"][0]["events"],
            json!([
                ["priority", 5],
                ["control_running", "priority", 6],
                ["key_shift", 0],
                ["control_running", "key_shift", 1],
                ["tempo", 30],
                ["note", 1, 60, 100],
                ["fine"]
            ])
        );
    }
}
