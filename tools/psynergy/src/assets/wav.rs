use super::AssetError;

/// Editable mono PCM and its standard RIFF sampler choices. Loop endpoints are
/// inclusive; pitch_fraction is an unsigned fraction of one semitone.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Pcm8 {
    pub rate: u32,
    pub samples: Vec<u8>,
    pub unity_note: u8,
    pub pitch_fraction: u32,
    pub loop_range: Option<(u32, u32)>,
}

impl Pcm8 {
    /// Sample playback rate at a MIDI key, including its root-note tuning.
    pub fn playback_rate(&self, key: u8) -> f64 {
        f64::from(self.rate)
            * 2.0_f64.powf(
                (f64::from(key)
                    - f64::from(self.unity_note)
                    - f64::from(self.pitch_fraction) / 4_294_967_296.0)
                    / 12.0,
            )
    }

    fn validate(&self) -> Result<(), AssetError> {
        if self.rate == 0 || self.unity_note > 127 {
            return Err(AssetError("WAV rate or MIDI unity note is invalid".into()));
        }
        if self
            .loop_range
            .is_some_and(|(start, end)| start > end || end as usize >= self.samples.len())
        {
            return Err(AssetError(
                "WAV sampler loop exceeds its PCM samples".into(),
            ));
        }
        Ok(())
    }
}

fn word(data: &[u8], offset: usize) -> Result<u32, AssetError> {
    let end = offset
        .checked_add(4)
        .ok_or_else(|| AssetError("WAV field overflows".into()))?;
    let bytes = data
        .get(offset..end)
        .ok_or_else(|| AssetError("WAV field is truncated".into()))?;
    Ok(u32::from_le_bytes(bytes.try_into().unwrap()))
}

/// Read bounded standard fmt/data/smpl chunks without accepting payload sidecars.
pub fn read_pcm8(data: &[u8]) -> Result<Pcm8, AssetError> {
    if data.get(..4) != Some(b"RIFF")
        || data.get(8..12) != Some(b"WAVE")
        || usize::try_from(word(data, 4)?)
            .ok()
            .and_then(|size| size.checked_add(8))
            != Some(data.len())
    {
        return Err(AssetError("WAV RIFF header is invalid".into()));
    }
    let mut rate = None;
    let mut samples = None;
    let mut sampler = None;
    let mut offset = 12usize;
    while offset < data.len() {
        let payload = offset
            .checked_add(8)
            .ok_or_else(|| AssetError("WAV chunk offset overflows".into()))?;
        let size = usize::try_from(word(data, offset + 4)?)
            .map_err(|_| AssetError("WAV chunk size exceeds memory bounds".into()))?;
        let end = payload
            .checked_add(size)
            .ok_or_else(|| AssetError("WAV chunk extent overflows".into()))?;
        let chunk = data
            .get(payload..end)
            .ok_or_else(|| AssetError("WAV chunk is truncated".into()))?;
        match data.get(offset..offset + 4) {
            Some(b"fmt ") => {
                if rate.is_some()
                    || chunk.len() != 16
                    || chunk.get(..4) != Some(&[1, 0, 1, 0])
                    || word(chunk, 4)? == 0
                    || word(chunk, 8)? != word(chunk, 4)?
                    || chunk.get(12..16) != Some(&[1, 0, 8, 0])
                {
                    return Err(AssetError("WAV is not mono 8-bit PCM".into()));
                }
                rate = Some(word(chunk, 4)?);
            }
            Some(b"data") => {
                if rate.is_none() || samples.is_some() {
                    return Err(AssetError("WAV data is duplicated or precedes fmt".into()));
                }
                samples = Some(chunk.iter().map(|v| v.wrapping_sub(128)).collect());
            }
            Some(b"smpl") => {
                let loops = word(chunk, 28)?;
                if sampler.is_some()
                    || loops > 1
                    || word(chunk, 32)? != 0
                    || chunk.len() != 36 + loops as usize * 24
                    || word(chunk, 12)? > 127
                {
                    return Err(AssetError("WAV sampler metadata is unsupported".into()));
                }
                let loop_range = if loops == 1 {
                    if word(chunk, 40)? != 0 || word(chunk, 52)? != 0 || word(chunk, 56)? != 0 {
                        return Err(AssetError("WAV requires one infinite forward loop".into()));
                    }
                    Some((word(chunk, 44)?, word(chunk, 48)?))
                } else {
                    None
                };
                sampler = Some((word(chunk, 12)? as u8, word(chunk, 16)?, loop_range));
            }
            _ => return Err(AssetError("WAV carries an unsupported RIFF chunk".into())),
        }
        offset = end;
        if size & 1 != 0 && offset < data.len() {
            if data[offset] != 0 {
                return Err(AssetError("WAV chunk padding is invalid".into()));
            }
            offset += 1;
        }
    }
    let (unity_note, pitch_fraction, loop_range) = sampler.unwrap_or((60, 0, None));
    let wave = Pcm8 {
        rate: rate.ok_or_else(|| AssetError("WAV fmt chunk is missing".into()))?,
        samples: samples.ok_or_else(|| AssetError("WAV data chunk is missing".into()))?,
        unity_note,
        pitch_fraction,
        loop_range,
    };
    wave.validate()?;
    Ok(wave)
}

/// Read mono 8-bit PCM WAV, returning its rate and signed PCM bytes.
pub fn wav_pcm8(data: &[u8]) -> Result<(u32, Vec<u8>), AssetError> {
    let wave = read_pcm8(data)?;
    Ok((wave.rate, wave.samples))
}

/// Write PCM with only its standard forward-loop and MIDI root-pitch choices.
pub fn write_pcm8(wave: &Pcm8) -> Result<Vec<u8>, AssetError> {
    wave.validate()?;
    let size = u32::try_from(wave.samples.len())
        .map_err(|_| AssetError("PCM input exceeds WAV size bounds".into()))?;
    let mut wav = b"RIFF\0\0\0\0WAVEfmt ".to_vec();
    wav.extend_from_slice(&16u32.to_le_bytes());
    wav.extend_from_slice(&1u16.to_le_bytes());
    wav.extend_from_slice(&1u16.to_le_bytes());
    wav.extend_from_slice(&wave.rate.to_le_bytes());
    wav.extend_from_slice(&wave.rate.to_le_bytes());
    wav.extend_from_slice(&1u16.to_le_bytes());
    wav.extend_from_slice(&8u16.to_le_bytes());
    wav.extend_from_slice(b"data");
    wav.extend_from_slice(&size.to_le_bytes());
    wav.extend(wave.samples.iter().map(|sample| sample.wrapping_add(128)));
    if size & 1 != 0 {
        wav.push(0);
    }
    if wave.unity_note != 60 || wave.pitch_fraction != 0 || wave.loop_range.is_some() {
        wav.extend_from_slice(b"smpl");
        let loops = u32::from(wave.loop_range.is_some());
        wav.extend_from_slice(&(36 + loops * 24).to_le_bytes());
        let period = (1_000_000_000u64 + u64::from(wave.rate) / 2) / u64::from(wave.rate);
        for field in [
            0,
            0,
            period as u32,
            u32::from(wave.unity_note),
            wave.pitch_fraction,
            0,
            0,
            loops,
            0,
        ] {
            wav.extend_from_slice(&field.to_le_bytes());
        }
        if let Some((start, end)) = wave.loop_range {
            for field in [0, 0, start, end, 0, 0] {
                wav.extend_from_slice(&field.to_le_bytes());
            }
        }
    }
    let riff_size =
        u32::try_from(wav.len() - 8).map_err(|_| AssetError("WAV RIFF size exceeds u32".into()))?;
    wav[4..8].copy_from_slice(&riff_size.to_le_bytes());
    Ok(wav)
}

/// Encode signed PCM bytes as mono 8-bit PCM WAV at `rate` hertz.
pub fn pcm8_wav(samples: &[u8], rate: u32) -> Result<Vec<u8>, AssetError> {
    write_pcm8(&Pcm8 {
        rate,
        samples: samples.to_vec(),
        unity_note: 60,
        pitch_fraction: 0,
        loop_range: None,
    })
}

#[test]
fn pcm_and_standard_sampler_choices_round_trip_without_changing_samples() {
    let wave = Pcm8 {
        rate: 8_000,
        samples: vec![128, 0, 127],
        unity_note: 59,
        pitch_fraction: 0x8000_0000,
        loop_range: Some((1, 2)),
    };
    let wav = write_pcm8(&wave).unwrap();
    assert_eq!(&wav[44..47], &[0, 128, 255]);
    assert_eq!(wav[47], 0);
    assert_eq!(read_pcm8(&wav).unwrap(), wave);
    assert!((wave.playback_rate(60) - 8_000.0 * 2.0_f64.powf(0.5 / 12.0)).abs() < 0.000_001);
    assert_eq!(wav_pcm8(&wav).unwrap(), (8_000, wave.samples));
}

#[test]
fn wav_rejects_truncation_duplicate_payloads_and_retired_custom_chunks() {
    let wave = Pcm8 {
        rate: 8_000,
        samples: vec![128, 255, 0, 127],
        unity_note: 60,
        pitch_fraction: 0,
        loop_range: Some((1, 3)),
    };
    let wav = write_pcm8(&wave).unwrap();
    for end in 0..wav.len() {
        assert!(read_pcm8(&wav[..end]).is_err(), "truncation {end}");
    }
    let mut duplicate = wav.clone();
    duplicate.extend_from_slice(&wav[36..48]);
    let length = (duplicate.len() - 8) as u32;
    duplicate[4..8].copy_from_slice(&length.to_le_bytes());
    assert!(read_pcm8(&duplicate).is_err());
    let mut custom = wav;
    custom[48..52].copy_from_slice(b"agbp");
    assert!(read_pcm8(&custom).is_err());
}

#[test]
fn wav_checks_pcm_format_sampler_bounds_and_forward_loop_shape() {
    let wave = Pcm8 {
        rate: 8_000,
        samples: vec![128, 0, 127, 0],
        unity_note: 60,
        pitch_fraction: 0,
        loop_range: Some((1, 3)),
    };
    let wav = write_pcm8(&wave).unwrap();
    for offset in [0, 4, 8, 12, 16, 20, 22, 28, 32, 34] {
        let mut bad = wav.clone();
        bad[offset] ^= 128;
        assert!(read_pcm8(&bad).is_err(), "format field {offset}");
    }
    for (offset, value) in [(68, 128u32), (84, 2), (96, 1), (104, 4), (108, 1)] {
        let mut bad = wav.clone();
        bad[offset..offset + 4].copy_from_slice(&value.to_le_bytes());
        assert!(read_pcm8(&bad).is_err(), "sampler field {offset}");
    }
    let mut bad = wave;
    bad.loop_range = Some((2, 1));
    assert!(write_pcm8(&bad).is_err());
}

#[test]
fn pcm8_wav_round_trips_and_rejects_invalid_rate() {
    let samples = [128, 255, 0, 127];
    let wav = pcm8_wav(&samples, 16_000).unwrap();
    assert_eq!(wav_pcm8(&wav).unwrap(), (16_000, samples.to_vec()));
    assert!(pcm8_wav(&samples, 0).is_err());
}
