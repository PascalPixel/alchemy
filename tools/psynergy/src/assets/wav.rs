use super::AssetError;

/// Read canonical mono 8-bit PCM WAV, returning its rate and signed PCM bytes.
pub fn wav_pcm8(data: &[u8]) -> Result<(u32, Vec<u8>), AssetError> {
    if data.len() < 44 {
        return Err(AssetError("WAV is truncated".into()));
    }
    let word = |at| u32::from_le_bytes(data[at..at + 4].try_into().unwrap());
    let half = |at| u16::from_le_bytes(data[at..at + 2].try_into().unwrap());
    let rate = word(24);
    if &data[..4] != b"RIFF"
        || &data[8..16] != b"WAVEfmt "
        || &data[36..40] != b"data"
        || word(4) as usize != data.len() - 8
        || word(16) != 16
        || half(20) != 1
        || half(22) != 1
        || rate == 0
        || word(28) != rate
        || half(32) != 1
        || half(34) != 8
        || word(40) as usize != data.len() - 44
    {
        return Err(AssetError("WAV is not canonical mono 8-bit PCM".into()));
    }
    Ok((
        rate,
        data[44..].iter().map(|v| v.wrapping_sub(128)).collect(),
    ))
}

/// Encode signed PCM bytes as canonical mono 8-bit PCM WAV at `rate` hertz.
pub fn pcm8_wav(samples: &[u8], rate: u32) -> Result<Vec<u8>, AssetError> {
    if rate == 0 {
        return Err(AssetError("WAV rate must be greater than zero".into()));
    }
    let size = u32::try_from(samples.len())
        .map_err(|_| AssetError("PCM input exceeds WAV size bounds".into()))?;
    let riff_size = size
        .checked_add(36)
        .ok_or_else(|| AssetError("PCM input exceeds WAV size bounds".into()))?;
    let mut wav = Vec::with_capacity(samples.len() + 44);
    wav.extend_from_slice(b"RIFF");
    wav.extend_from_slice(&riff_size.to_le_bytes());
    wav.extend_from_slice(b"WAVEfmt ");
    wav.extend_from_slice(&16u32.to_le_bytes());
    wav.extend_from_slice(&1u16.to_le_bytes());
    wav.extend_from_slice(&1u16.to_le_bytes());
    wav.extend_from_slice(&rate.to_le_bytes());
    wav.extend_from_slice(&rate.to_le_bytes());
    wav.extend_from_slice(&1u16.to_le_bytes());
    wav.extend_from_slice(&8u16.to_le_bytes());
    wav.extend_from_slice(b"data");
    wav.extend_from_slice(&size.to_le_bytes());
    wav.extend(samples.iter().map(|sample| sample.wrapping_add(128)));
    Ok(wav)
}

#[test]
fn wav_pcm_checks_header_and_preserves_signed_samples() {
    let mut wav = Vec::from(&b"RIFF\x27\0\0\0WAVEfmt \x10\0\0\0\x01\0\x01\0\x40\x1f\0\0\x40\x1f\0\0\x01\0\x08\0data\x03\0\0\0"[..]);
    wav.extend([0, 128, 255]);
    assert_eq!(wav_pcm8(&wav).unwrap(), (8000, vec![128, 0, 127]));
    for offset in [0, 4, 8, 12, 16, 20, 22, 28, 32, 34, 36, 40] {
        let mut bad = wav.clone();
        bad[offset] ^= 128;
        assert!(wav_pcm8(&bad).is_err(), "offset {offset}");
    }
    for size in 0..44 {
        assert!(wav_pcm8(&wav[..size]).is_err());
    }
}

#[test]
fn pcm8_wav_round_trips_and_rejects_invalid_rate() {
    let samples = [128, 255, 0, 127];
    let wav = pcm8_wav(&samples, 16_000).unwrap();
    assert_eq!(wav_pcm8(&wav).unwrap(), (16_000, samples.to_vec()));
    assert!(pcm8_wav(&samples, 0).is_err());
}
