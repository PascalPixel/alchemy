use crate::AssetError;

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
