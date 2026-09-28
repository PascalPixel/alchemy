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

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct SoundVoice {
    pub kind: u8,
    pub base_key: u8,
    pub target: u32,
    pub envelope: [u8; 4],
}

pub const SOUND_VOICE_BYTES: usize = 12;

/// The twelve-byte SoundVoice consumed by the maintained HANDLE_NOTE.S readers.
pub fn sound_voice_record(data: &[u8], offset: usize) -> Result<SoundVoice, AssetError> {
    let end = offset
        .checked_add(SOUND_VOICE_BYTES)
        .ok_or_else(|| AssetError("sound voice offset overflows".into()))?;
    let record = data
        .get(offset..end)
        .ok_or_else(|| AssetError("sound voice record is truncated".into()))?;
    Ok(SoundVoice {
        kind: record[0],
        base_key: record[1],
        target: u32::from_le_bytes(record[4..8].try_into().unwrap()),
        envelope: record[8..12].try_into().unwrap(),
    })
}

/// Resolve a MIDI program/key through the driver's single split/rhythm level.
/// The caller owns the address base and bank binding; no bank catalog is used.
pub fn sound_voice(
    data: &[u8],
    address_base: u64,
    bank: u64,
    program: u8,
    key: u8,
) -> Result<(SoundVoice, bool), AssetError> {
    if program >= 128 || key >= 128 {
        return err("sound program and key must be seven-bit indices");
    }
    let offset = |address: u64| -> Result<usize, AssetError> {
        let offset = address
            .checked_sub(address_base)
            .ok_or_else(|| AssetError("sound pointer precedes the image".into()))?;
        usize::try_from(offset).map_err(|_| AssetError("sound pointer overflows".into()))
    };
    let record = |bank: u64, index: u8| -> Result<SoundVoice, AssetError> {
        let address = bank
            .checked_add(u64::from(index) * SOUND_VOICE_BYTES as u64)
            .ok_or_else(|| AssetError("sound voice address overflows".into()))?;
        sound_voice_record(data, offset(address)?)
    };
    let mut voice = record(bank, program)?;
    let rhythm = voice.kind & 128 != 0;
    if voice.kind & 0xc0 != 0 {
        if !matches!(voice.kind, 64 | 128 | 192) {
            return err("invalid sound selector type");
        }
        let index = if voice.kind & 64 != 0 {
            let at = offset(u64::from(u32::from_le_bytes(voice.envelope)))?;
            let end = at
                .checked_add(128)
                .ok_or_else(|| AssetError("sound key-map offset overflows".into()))?;
            let map = data
                .get(at..end)
                .ok_or_else(|| AssetError("sound key map is truncated".into()))?;
            map[usize::from(key)]
        } else {
            key
        };
        voice = record(u64::from(voice.target), index)?;
    }
    // Both retained note handlers refuse a second selector level.
    if !matches!(voice.kind, 0..=4 | 8..=12) || voice.base_key >= 128 {
        return err("invalid or nested sound voice");
    }
    Ok((voice, rhythm))
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
    fn sound_records_and_key_selection_are_bounded() {
        const BASE: u64 = 0x08000000;
        let mut image = vec![0; 512];
        image[0] = 64;
        image[4..8].copy_from_slice(&(BASE as u32 + 32).to_le_bytes());
        image[8..12].copy_from_slice(&(BASE as u32 + 320).to_le_bytes());
        image[320 + 72] = 2;
        image[56] = 3;
        image[57] = 60;
        image[60..64].copy_from_slice(&(BASE as u32 + 480).to_le_bytes());
        image[64..68].copy_from_slice(&[1, 2, 3, 4]);
        let expected = SoundVoice {
            kind: 3,
            base_key: 60,
            target: BASE as u32 + 480,
            envelope: [1, 2, 3, 4],
        };
        assert_eq!(
            sound_voice(&image, BASE, BASE, 0, 72).unwrap(),
            (expected, false)
        );
        image[0] = 128;
        assert_eq!(
            sound_voice(&image, BASE, BASE, 0, 2).unwrap(),
            (expected, true)
        );
        image[0] = 192;
        assert_eq!(
            sound_voice(&image, BASE, BASE, 0, 72).unwrap(),
            (expected, true)
        );
        assert_eq!(
            sound_voice(&image, BASE, BASE + 56, 0, 72).unwrap(),
            (expected, false)
        );
        for end in 0..SOUND_VOICE_BYTES {
            assert!(sound_voice_record(&image[..end], 0).is_err());
        }
        assert!(sound_voice_record(&image, usize::MAX).is_err());
        assert!(sound_voice(&image, BASE, BASE - 1, 0, 72).is_err());
        assert!(sound_voice(&image, BASE, u64::MAX, 1, 72).is_err());
        assert!(sound_voice(&image, BASE, BASE, 128, 72).is_err());
        assert!(sound_voice(&image, BASE, BASE, 0, 128).is_err());
        image[0] = 64;
        image[8..12].copy_from_slice(&(BASE as u32 + 385).to_le_bytes());
        assert!(sound_voice(&image, BASE, BASE, 0, 2).is_err());
        image[8..12].copy_from_slice(&(BASE as u32 + 320).to_le_bytes());
        image[56] = 128;
        assert!(sound_voice(&image, BASE, BASE, 0, 72).is_err());
    }

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
