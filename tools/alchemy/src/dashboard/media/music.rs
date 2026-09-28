//! Bounded audition of recovered MIDI and voice sources, not SMSH emulation.
use psynergy::assets::midi::{midi_events, EventBody};
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
    let report = midi_events(&midi).map_err(|e| e.to_string())?;
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
    let sound = root.join("games").join(game).join("SOUND");
    let engine_path = format!("games/{game}/SOUND/INSTRUMENT/ENGINE.JSON");
    let mut engine: Value =
        serde_json::from_slice(&read(&root.join(&engine_path))?).map_err(|e| e.to_string())?;
    engine["address"] = crate::build_assets::placed_address(root, &engine_path, "")?.into();
    let segments = engine["segments"]
        .as_array()
        .ok_or("No instrument segments")?;
    let starts = crate::build_assets::segment_starts(&engine)?
        .into_iter()
        .map(|start| start as u64)
        .collect::<Vec<_>>();
    let bank = segments
        .iter()
        .zip(&starts)
        .find(|(_, start)| **start == bank_address)
        .and_then(|(s, _)| s["records"].as_array())
        .ok_or("Sequence voice bank is not recovered")?;
    let mut samples = BTreeMap::new();
    let mut reference: Option<Vec<u8>> = None;
    let sample_table =
        String::from_utf8(read(&sound.join("SAMPLE/SAMPLES.TSV"))?).map_err(|e| e.to_string())?;
    for row in sample_table.lines().filter(|s| !s.starts_with('#')).skip(1) {
        let fields = row.split('\t').collect::<Vec<_>>();
        if fields.len() != 6 {
            return Err("Malformed sample table".into());
        }
        let path = Path::new(fields[5]);
        if path.components().count() != 1 {
            return Err("Sample source must be a filename".into());
        }
        let (rate, pcm) = psynergy::assets::wav::wav_pcm8(&read(&sound.join("SAMPLE").join(path))?)
            .map_err(|e| e.to_string())?;
        let address = u64::from_str_radix(fields[1].trim_start_matches("0x"), 16)
            .map_err(|e| e.to_string())?;
        let loop_at = if fields[3].is_empty() {
            None
        } else {
            Some(fields[3].parse::<usize>().map_err(|e| e.to_string())?)
        };
        if loop_at.is_some_and(|i| i >= pcm.len()) {
            return Err("Sample loop outside PCM extent".into());
        }
        samples.insert(
            address,
            Sample {
                values: pcm.iter().map(|b| *b as i8 as f32 / 128.).collect(),
                rate: f64::from(rate),
                loop_at,
            },
        );
    }
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
        let mut voice = bank
            .get(note.channel.program)
            .ok_or("Program outside recovered bank")?;
        let mut kind = number(&voice["type"])?;
        let rhythm = kind & 128 != 0;
        if rhythm {
            let offset = number(&voice["target"])? + u64::from(note.key) * 12;
            voice = segments
                .iter()
                .zip(&starts)
                .find_map(|(segment, &start)| {
                    let rows = segment["records"].as_array()?;
                    if !segment["id"].as_str()?.starts_with("voice_bank_")
                        || offset < start
                        || (offset - start) % 12 != 0
                    {
                        return None;
                    }
                    rows.get(((offset - start) / 12) as usize)
                })
                .ok_or("Rhythm voice is not recovered")?;
            kind = number(&voice["type"])?;
        }
        if kind & 0xc0 != 0 {
            return Err(format!(
                "Unsupported split voice type {kind}; preview refused"
            ));
        }
        let key = number(&voice["key"])? as f64;
        let pitch = if rhythm || kind & 8 != 0 {
            key
        } else {
            f64::from(note.key) + f64::from(note.channel.bend)
        };
        let target = number(&voice["target"])?;
        if kind & 7 == 0 && !samples.contains_key(&target) {
            if reference.is_none() {
                let id = if game == "THE BROKEN SEAL" {
                    "tbs-en"
                } else {
                    "tla-en"
                };
                let bytes = read(&root.join(format!("roms/{id}.gba")))?;
                crate::text_catalog::verify_reference(root, id, &bytes)?;
                reference = Some(bytes);
            }
            samples.insert(
                target,
                reference_sample(reference.as_ref().unwrap(), target)?,
            );
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
                let waves = segments
                    .iter()
                    .find(|s| s["id"] == "waveforms")
                    .ok_or("Waveform table is not recovered")?;
                let start = number(&waves["address"])?;
                let index = target
                    .checked_sub(start)
                    .ok_or("Waveform pointer before table")?
                    / 16;
                let packed = waves["records"][index as usize]["packed_samples"]
                    .as_array()
                    .ok_or("Waveform is not recovered")?;
                let mut values = vec![];
                for byte in packed {
                    let byte = number(byte)? as u8;
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
        let sustain =
            voice["envelope"][2].as_f64().unwrap_or(255.) / if kind & 7 == 0 { 255. } else { 15. };
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
