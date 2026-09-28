//! Bounded MIDI audition using the approved local reference's sound voices.
use psynergy::assets::midi::{midi_events, sound_voice, EventBody};
use serde_json::Value;
use std::{collections::BTreeMap, path::Path};
const RATE: usize = 22050;
const LIMIT: f64 = 60.0;
fn number(v: &Value) -> Result<u64, String> {
    v.as_u64()
        .or_else(|| {
            v.as_str()
                .and_then(|s| u64::from_str_radix(s.trim_start_matches("0x"), 16).ok())
        })
        .ok_or_else(|| format!("invalid voice value {v}"))
}
fn read(path: &Path) -> Result<Vec<u8>, String> {
    std::fs::read(path).map_err(|e| format!("{}: {e}", path.display()))
}
#[derive(Clone, Copy)]
struct Channel {
    program: usize,
    volume: f32,
    pan: f32,
    bend: f32,
}
impl Default for Channel {
    fn default() -> Self {
        Self {
            program: 0,
            volume: 1.,
            pan: 0.,
            bend: 0.,
        }
    }
}
struct Note {
    start: f64,
    end: f64,
    key: u8,
    velocity: f32,
    channel: Channel,
}
struct Sample {
    values: Vec<f32>,
    rate: f64,
    loop_at: Option<usize>,
}

pub(super) fn render(root: &Path, game: &str, source: &Path) -> Result<Vec<u8>, String> {
    let midi = read(source)?;
    let id = match game {
        "THE BROKEN SEAL" => "tbs-en",
        "THE LOST AGE" => "tla-en",
        _ => return Err(format!("unknown sound source game {game}")),
    };
    let reference = read(&root.join(format!("roms/{id}.gba")))?;
    crate::text_catalog::verify_reference(root, id, &reference)?;
    render_midi(&midi, &reference)
}

fn render_midi(midi: &[u8], reference: &[u8]) -> Result<Vec<u8>, String> {
    let report = midi_events(midi).map_err(|e| e.to_string())?;
    if report.ticks_per_quarter == 0 {
        return Err("MIDI has zero time division".into());
    }
    let skeleton = report
        .events
        .iter()
        .find_map(|e| match &e.body {
            EventBody::Meta { meta: 1, data } => serde_json::from_slice::<Value>(data).ok(),
            _ => None,
        })
        .ok_or("Sequence has no native voice-bank metadata")?;
    let bank_address = number(&skeleton["externals"]["tone_bank"])?;
    let mut samples = BTreeMap::new();
    let mut channels = BTreeMap::<(usize, u8), Channel>::new();
    let mut active = BTreeMap::<(usize, u8, u8), Note>::new();
    let mut notes = vec![];
    let (mut tick, mut time, mut tempo) = (0, 0., 500000.);
    let mut events = report.events;
    events.sort_by_key(|e| (e.tick, e.track, e.order));
    for event in events {
        time +=
            (event.tick - tick) as f64 * tempo / 1_000_000. / f64::from(report.ticks_per_quarter);
        tick = event.tick;
        if time > LIMIT {
            break;
        }
        match event.body {
            EventBody::Meta { meta: 0x51, data } if data.len() == 3 => {
                tempo = f64::from(u32::from_be_bytes([0, data[0], data[1], data[2]]))
            }
            EventBody::Channel { status, data } => {
                let channel = channels.entry((event.track, status & 15)).or_default();
                let command = status & 0xf0;
                let key = (event.track, status & 15, data[0]);
                match command {
                    0xc0 => channel.program = data[0] as usize,
                    0xb0 if data[0] == 7 => channel.volume = f32::from(data[1]) / 127.,
                    0xb0 if data[0] == 10 => channel.pan = (f32::from(data[1]) - 64.) / 64.,
                    0xe0 => {
                        channel.bend =
                            (f32::from(data[0]) + 128. * f32::from(data[1]) - 8192.) / 8192. * 2.
                    }
                    0x80 | 0x90 => {
                        if let Some(mut old) = active.remove(&key) {
                            old.end = time;
                            notes.push(old)
                        }
                        if command == 0x90 && data[1] > 0 {
                            active.insert(
                                key,
                                Note {
                                    start: time,
                                    end: LIMIT,
                                    key: data[0],
                                    velocity: f32::from(data[1]) / 127.,
                                    channel: *channel,
                                },
                            );
                        }
                    }
                    _ => {}
                }
            }
            _ => {}
        }
    }
    notes.extend(active.into_values());
    let duration = notes
        .iter()
        .map(|n| n.end + 0.4)
        .fold(0., f64::max)
        .min(LIMIT);
    if duration <= 0. {
        return Err("Sequence has no playable MIDI notes".into());
    }
    let mut mix = vec![[0f32; 2]; (duration * RATE as f64) as usize];
    for note in notes {
        let (voice, rhythm) = sound_voice(
            reference,
            0x08000000,
            bank_address,
            note.channel.program as u8,
            note.key,
        )
        .map_err(|error| error.to_string())?;
        let kind = voice.kind;
        let key = f64::from(voice.base_key);
        let pitch = if rhythm || kind & 8 != 0 {
            key
        } else {
            f64::from(note.key) + f64::from(note.channel.bend)
        };
        let target = u64::from(voice.target);
        if kind & 7 == 0 && !samples.contains_key(&target) {
            samples.insert(target, reference_sample(reference, target)?);
        }
        let generator;
        let sample = match kind & 7 {
            0 => samples
                .get(&target)
                .ok_or_else(|| format!("PCM sample 0x{target:08x} is not recovered"))?,
            1 | 2 => {
                let duty = [0.125, 0.25, 0.5, 0.75]
                    .get(target as usize)
                    .ok_or("Invalid pulse duty")?;
                generator = Sample {
                    values: (0..256)
                        .map(|i| if i as f64 / 256. < *duty { 0.6 } else { -0.6 })
                        .collect(),
                    rate: 440. * 2f64.powf((key - 69.) / 12.) * 256.,
                    loop_at: Some(0),
                };
                &generator
            }
            3 => {
                let start = usize::try_from(
                    target
                        .checked_sub(0x08000000)
                        .ok_or("Waveform pointer before ROM")?,
                )
                .map_err(|error| error.to_string())?;
                let packed = reference
                    .get(start..start.checked_add(16).ok_or("Waveform extent overflows")?)
                    .ok_or("Waveform outside ROM")?;
                let mut values = vec![];
                for byte in packed {
                    values.extend([
                        (f32::from(byte >> 4) - 7.5) / 7.5,
                        (f32::from(byte & 15) - 7.5) / 7.5,
                    ]);
                }
                generator = Sample {
                    rate: 440. * 2f64.powf((key - 69.) / 12.) * values.len() as f64,
                    values,
                    loop_at: Some(0),
                };
                &generator
            }
            4 => {
                let mut state = 0x7fffu16;
                let values = (0..32767)
                    .map(|_| {
                        let feedback = (state ^ (state >> 1)) & 1;
                        state = (state >> 1) | (feedback << 14);
                        if state & 1 == 0 {
                            0.4
                        } else {
                            -0.4
                        }
                    })
                    .collect();
                generator = Sample {
                    values,
                    rate: RATE as f64,
                    loop_at: Some(0),
                };
                &generator
            }
            _ => return Err(format!("Unsupported instrument type {kind}")),
        };
        let sustain = f64::from(voice.envelope[2]) / if kind & 7 == 0 { 255. } else { 15. };
        let step = sample.rate / RATE as f64 * 2f64.powf((pitch - key) / 12.);
        let begin = (note.start * RATE as f64) as usize;
        let end = ((note.end + 0.3) * RATE as f64) as usize;
        for i in begin..end.min(mix.len()) {
            let age = (i - begin) as f64 / RATE as f64;
            let mut position = (i - begin) as f64 * step;
            if position >= sample.values.len() as f64 {
                let Some(start) = sample.loop_at else { break };
                position =
                    start as f64 + (position - start as f64) % (sample.values.len() - start) as f64;
            }
            let index = position as usize;
            let next = if index + 1 < sample.values.len() {
                index + 1
            } else {
                sample.loop_at.unwrap_or(index)
            };
            let value = sample.values[index]
                + (sample.values[next] - sample.values[index]) * (position - index as f64) as f32;
            let envelope = (age / 0.008).min(1.)
                * sustain
                * (1. - ((i as f64 / RATE as f64 - note.end) / 0.3).max(0.)).max(0.);
            let value = value * envelope as f32 * note.velocity * note.channel.volume * 0.12;
            mix[i][0] += value * (1. - note.channel.pan).sqrt() * 0.707;
            mix[i][1] += value * (1. + note.channel.pan).sqrt() * 0.707;
        }
    }
    let mut data = vec![];
    for frame in mix {
        for sample in frame {
            data.extend_from_slice(&((sample.clamp(-1., 1.) * 32767.) as i16).to_le_bytes());
        }
    }
    let mut wav = b"RIFF".to_vec();
    wav.extend_from_slice(&(data.len() as u32 + 36).to_le_bytes());
    wav.extend_from_slice(b"WAVEfmt ");
    wav.extend_from_slice(&16u32.to_le_bytes());
    wav.extend_from_slice(&1u16.to_le_bytes());
    wav.extend_from_slice(&2u16.to_le_bytes());
    wav.extend_from_slice(&(RATE as u32).to_le_bytes());
    wav.extend_from_slice(&(RATE as u32 * 4).to_le_bytes());
    wav.extend_from_slice(&4u16.to_le_bytes());
    wav.extend_from_slice(&16u16.to_le_bytes());
    wav.extend_from_slice(b"data");
    wav.extend_from_slice(&(data.len() as u32).to_le_bytes());
    wav.extend(data);
    Ok(wav)
}

fn reference_sample(rom: &[u8], address: u64) -> Result<Sample, String> {
    let at = address
        .checked_sub(0x08000000)
        .ok_or("PCM pointer before ROM")? as usize;
    let header = rom
        .get(at..at.saturating_add(16))
        .ok_or("PCM header outside ROM")?;
    let word = |i| u32::from_le_bytes(header[i..i + 4].try_into().unwrap()) as usize;
    let (control, frequency, start, last) = (word(0), word(4), word(8), word(12));
    if control & 0x3fffffff != 0
        || frequency == 0
        || frequency > 192000 * 1024
        || (control & 0xc0000000 != 0 && start > last)
    {
        return Err("Invalid PCM header".into());
    }
    let pcm = rom
        .get(at + 16..at.saturating_add(17).saturating_add(last))
        .ok_or("PCM extent outside ROM")?;
    Ok(Sample {
        values: pcm.iter().map(|b| *b as i8 as f32 / 128.).collect(),
        rate: frequency as f64 / 1024.,
        loop_at: if control & 0xc0000000 != 0 {
            Some(start)
        } else {
            None
        },
    })
}

#[test]
fn midi_preview_reads_bound_voices_without_a_catalog() {
    let midi = psynergy::assets::midi::append_conductor_meta(
        b"MThd\0\0\0\x06\0\0\0\x01\0\x60MTrk\0\0\0\x0c\0\x90\x3c\x7f\x60\x80\x3c\0\0\xff\x2f\0",
        1,
        br#"{"externals":{"tone_bank":"0x08000000"}}"#,
    )
    .unwrap();
    let mut rom = vec![0; 32];
    rom[1] = 60;
    rom[4..8].copy_from_slice(&0x0800000cu32.to_le_bytes());
    rom[10] = 255;
    rom[12..16].copy_from_slice(&0x40000000u32.to_le_bytes());
    rom[16..20].copy_from_slice(&(8000u32 * 1024).to_le_bytes());
    rom[24..28].copy_from_slice(&3u32.to_le_bytes());
    rom[28..32].copy_from_slice(&[128, 64, 128, 64]);
    let wav = render_midi(&midi, &rom).unwrap();
    assert_eq!(&wav[..4], b"RIFF");
    assert_eq!(
        u32::from_le_bytes(wav[4..8].try_into().unwrap()) as usize + 8,
        wav.len()
    );
    assert!(wav[44..].iter().any(|byte| *byte != 0));
    assert!(render_midi(&midi, &rom[..31]).is_err());
    rom[0] = 3;
    rom[10] = 15;
    assert!(render_midi(&midi, &rom[..28]).is_ok());
    assert!(render_midi(&midi, &rom[..27]).is_err());
}

#[test]
#[ignore = "requires approved local reference ROMs"]
fn current_midi_previews_use_approved_reference_voices() {
    let root = Path::new(env!("CARGO_MANIFEST_DIR")).join("../..");
    for (game, sequence) in [("THE BROKEN SEAL", "BGM_000"), ("THE LOST AGE", "BGM_008")] {
        let source = root.join(format!("games/{game}/SOUND/SEQUENCE/{sequence}.MID"));
        let wav = render(&root, game, &source).unwrap();
        assert_eq!(&wav[..4], b"RIFF");
        assert!(wav[44..].iter().any(|byte| *byte != 0));
        eprintln!("{game} MIDI preview: {} generated WAV bytes", wav.len());
    }
}

#[test]
fn reference_pcm_validates_extent_and_loop() {
    let mut rom = vec![0; 18];
    rom[..4].copy_from_slice(&0x40000000u32.to_le_bytes());
    rom[4..8].copy_from_slice(&(8000u32 * 1024).to_le_bytes());
    rom[12..16].copy_from_slice(&1u32.to_le_bytes());
    rom[16..].copy_from_slice(&[128, 127]);
    let s = reference_sample(&rom, 0x08000000).unwrap();
    assert_eq!(s.rate, 8000.);
    assert_eq!(s.loop_at, Some(0));
    assert_eq!(s.values.len(), 2);
    assert!(reference_sample(&rom[..17], 0x08000000).is_err());
    rom[8..12].copy_from_slice(&2u32.to_le_bytes());
    assert!(reference_sample(&rom, 0x08000000).is_err());
}
