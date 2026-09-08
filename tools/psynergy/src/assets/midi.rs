//! Standard MIDI file events and conductor metadata.
use super::{err, AssetError};
fn be_u16(data: &[u8], at: usize) -> Option<u16> {
    data.get(at..at + 2)
        .map(|x| u16::from_be_bytes([x[0], x[1]]))
}
fn be_u32(data: &[u8], at: usize) -> Option<u32> {
    data.get(at..at + 4)
        .map(|x| u32::from_be_bytes([x[0], x[1], x[2], x[3]]))
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum EventBody {
    Meta { meta: u8, data: Vec<u8> },
    Sysex { status: u8, data: Vec<u8> },
    Channel { status: u8, data: Vec<u8> },
}
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct MidiEvent {
    pub tick: i64,
    pub track: usize,
    pub order: usize,
    pub body: EventBody,
}
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct MidiReport {
    pub format: u16,
    pub tracks: u16,
    pub ticks_per_quarter: u16,
    pub events: Vec<MidiEvent>,
}

fn vlq(data: &[u8], mut at: usize) -> Result<(i32, usize), AssetError> {
    let mut value = 0;
    for _ in 0..4 {
        let byte = *data
            .get(at)
            .ok_or_else(|| AssetError("truncated variable-length quantity".into()))?;
        at += 1;
        value = value << 7 | i32::from(byte & 0x7f);
        if byte < 0x80 {
            return Ok((value, at));
        }
    }
    err("overlong variable-length quantity")
}

pub fn midi_events(data: &[u8]) -> Result<MidiReport, AssetError> {
    let mut chunks = Vec::new();
    let mut at = 0;
    while at < data.len() {
        if at + 8 > data.len() {
            return err("truncated MIDI chunk");
        }
        let kind = &data[at..at + 4];
        let size = be_u32(data, at + 4).unwrap() as usize;
        at += 8;
        let body = data
            .get(at..at.saturating_add(size))
            .ok_or_else(|| AssetError("truncated MIDI chunk payload".into()))?;
        at = at.saturating_add(size);
        chunks.push((kind, body));
    }
    if chunks.first().map(|(kind, body)| (*kind, body.len())) != Some((b"MThd".as_slice(), 6)) {
        return err("invalid MIDI header");
    }
    let header = chunks[0].1;
    let format = be_u16(header, 0).unwrap();
    let tracks = be_u16(header, 2).unwrap();
    let division = be_u16(header, 4).unwrap();
    if !matches!(format, 0 | 1) || tracks == 0 || division & 0x8000 != 0 {
        return err("only format 0/1 PPQN MIDI is supported");
    }
    let tracks_data: Vec<&[u8]> = chunks
        .iter()
        .skip(1)
        .filter(|(kind, _)| *kind == b"MTrk")
        .map(|(_, body)| *body)
        .collect();
    if tracks_data.len() != tracks as usize {
        return err("MIDI track count mismatch");
    }
    let mut events = Vec::new();
    for (track, bytes) in tracks_data.into_iter().enumerate() {
        let (mut at, mut tick, mut order, mut running) = (0, 0i64, 0, None);
        while at < bytes.len() {
            let (delta, next) = vlq(bytes, at)?;
            at = next;
            tick += i64::from(delta);
            let mut status = *bytes
                .get(at)
                .ok_or_else(|| AssetError("truncated MIDI event".into()))?;
            if status < 0x80 {
                status = running
                    .ok_or_else(|| AssetError("running status without channel status".into()))?;
            } else {
                at += 1;
            }
            let body = match status {
                0xff => {
                    let meta = *bytes
                        .get(at)
                        .ok_or_else(|| AssetError("truncated meta event".into()))?;
                    let (n, next) = vlq(bytes, at + 1)?;
                    at = next;
                    let value = bytes
                        .get(at..at.saturating_add(n as usize))
                        .ok_or_else(|| AssetError("truncated meta payload".into()))?;
                    at += n as usize;
                    running = None;
                    EventBody::Meta {
                        meta,
                        data: value.to_vec(),
                    }
                }
                0xf0 | 0xf7 => {
                    let (n, next) = vlq(bytes, at)?;
                    at = next;
                    let value = bytes
                        .get(at..at.saturating_add(n as usize))
                        .ok_or_else(|| AssetError("truncated system-exclusive payload".into()))?;
                    at += n as usize;
                    running = None;
                    EventBody::Sysex {
                        status,
                        data: value.to_vec(),
                    }
                }
                0x80..=0xef => {
                    running = Some(status);
                    let n = if matches!(status & 0xf0, 0xc0 | 0xd0) {
                        1
                    } else {
                        2
                    };
                    let value = bytes
                        .get(at..at + n)
                        .ok_or_else(|| AssetError("invalid channel event".into()))?;
                    at += n;
                    if value.iter().any(|byte| byte & 0x80 != 0) {
                        return err("invalid channel event");
                    }
                    EventBody::Channel {
                        status,
                        data: value.to_vec(),
                    }
                }
                _ => return err("unsupported MIDI system event"),
            };
            events.push(MidiEvent {
                tick,
                track,
                order,
                body,
            });
            order += 1;
        }
    }
    events.sort_by_key(|event| (event.tick, event.track, event.order));
    Ok(MidiReport {
        format,
        tracks,
        ticks_per_quarter: division,
        events,
    })
}

fn midi_vlq(mut value: usize) -> Vec<u8> {
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

/// Append a meta event with `payload` to the first (conductor) track of a MIDI
/// file, immediately before its canonical end-of-track event, and rewrite the
/// track length.
pub fn append_conductor_meta(midi: &[u8], meta: u8, payload: &[u8]) -> Result<Vec<u8>, AssetError> {
    if midi.len() < 26 || &midi[..4] != b"MThd" {
        return err("MIDI header is missing");
    }
    let track = 8 + be_u32(midi, 4).unwrap() as usize;
    if midi.get(track..track + 4) != Some(b"MTrk") {
        return err("MIDI conductor track is missing");
    }
    let length = be_u32(midi, track + 4)
        .ok_or_else(|| AssetError("MIDI conductor length is truncated".into()))?
        as usize;
    let end = track + 8 + length;
    if end > midi.len() || midi.get(end - 4..end) != Some(&[0, 0xff, 0x2f, 0]) {
        return err("MIDI conductor end is not canonical");
    }
    let mut event = vec![0, 0xff, meta];
    event.extend(midi_vlq(payload.len()));
    event.extend_from_slice(payload);
    let new_length = u32::try_from(length + event.len())
        .map_err(|_| AssetError("MIDI conductor is too large".into()))?;
    let mut output = Vec::with_capacity(midi.len() + event.len());
    output.extend_from_slice(&midi[..track + 4]);
    output.extend_from_slice(&new_length.to_be_bytes());
    output.extend_from_slice(&midi[track + 8..end - 4]);
    output.extend_from_slice(&event);
    output.extend_from_slice(&midi[end - 4..]);
    Ok(output)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn midi_chunk_names_are_exact_and_payloads_are_bounded() {
        let midi = b"MThd\0\0\0\x06\0\0\0\x01\0\x60MTrk\0\0\0\x04\0\xff\x2f\0";
        for index in [0, 1, 2, 3, 14, 15, 16, 17] {
            let mut damaged = midi.to_vec();
            damaged[index] |= 0x80;
            assert!(midi_events(&damaged).is_err());
        }
        for end in 0..midi.len() {
            assert!(midi_events(&midi[..end]).is_err());
        }
        assert!(midi_events(midi).is_ok());
    }

    #[test]
    fn midi_metadata_and_sysex_preserve_binary_payloads() {
        let midi = b"MThd\0\0\0\x06\0\0\0\x01\0\x60MTrk\0\0\0\x12\0\xff\x7f\x03\0\xff\x80\0\xf0\x04\0\x80\xff\xf7\0\xff\x2f\0";
        let events = midi_events(midi).unwrap().events;
        assert_eq!(
            events[0].body,
            EventBody::Meta {
                meta: 0x7f,
                data: vec![0, 255, 128]
            }
        );
        assert_eq!(
            events[1].body,
            EventBody::Sysex {
                status: 0xf0,
                data: vec![0, 128, 255, 247]
            }
        );
        assert_eq!(events.len(), 3);
    }

    #[test]
    fn conductor_meta_events_are_appended_before_the_canonical_end() {
        let midi = b"MThd\0\0\0\x06\0\0\0\x01\0\x60MTrk\0\0\0\x09\0\xff\x01\x01a\0\xff\x2f\0";
        let appended = append_conductor_meta(midi, 0x7f, b"directive").unwrap();
        let expected = b"MThd\0\0\0\x06\0\0\0\x01\0\x60MTrk\0\0\0\x16\0\xff\x01\x01a\0\xff\x7f\x09directive\0\xff\x2f\0";
        assert_eq!(appended, expected);
        assert_eq!(midi_events(&appended).unwrap().events.len(), 3);
        assert_eq!(midi_vlq(0x4000), [0x81, 0x80, 0x00]);
        let truncated = &midi[..midi.len() - 1];
        assert!(append_conductor_meta(truncated, 0x01, b"x").is_err());
        assert!(append_conductor_meta(b"MTrk", 0x01, b"x").is_err());
    }
}
