//! Zero-skip pixel streams shared by Golden Sun sprite archives.
//!
//! Bytes `1..=0xdf` are literal palette indices, `0xe0..=0xff` encode
//! runs of `1..=32` zero pixels, and a final zero terminates the stream.

use crate::AssetError;

/// Encode palette indices as a terminated zero-skip stream.
///
/// The format reserves `0xe0..=0xff` for zero runs, so literal pixels must
/// remain in `1..=0xdf`. Empty input is the one-byte terminated stream.
pub fn encode_zero_skip(pixels: &[u8]) -> Result<Vec<u8>, AssetError> {
    let mut output = Vec::new();
    let mut cursor = 0usize;
    while cursor < pixels.len() {
        let value = pixels[cursor];
        if value != 0 {
            if value > 0xdf {
                return Err(AssetError("zero-skip literal is outside 1..=223".into()));
            }
            output.push(value);
            cursor += 1;
            continue;
        }
        let mut end = cursor + 1;
        while end < pixels.len() && pixels[end] == 0 {
            end += 1;
        }
        let mut remaining = end - cursor;
        while remaining != 0 {
            let count = remaining.min(32);
            output.push(0xdf + count as u8);
            remaining -= count;
        }
        cursor = end;
    }
    output.push(0);
    Ok(output)
}

#[cfg(test)]
mod tests {
    use super::encode_zero_skip;

    #[test]
    fn zero_skip_stream_handles_boundaries_and_reserved_literals() {
        assert_eq!(encode_zero_skip(&[]).unwrap(), [0]);
        assert_eq!(encode_zero_skip(&[0]).unwrap(), [0xe0, 0]);
        assert_eq!(encode_zero_skip(&[0; 32]).unwrap(), [0xff, 0]);
        assert_eq!(encode_zero_skip(&[0; 33]).unwrap(), [0xff, 0xe0, 0]);
        assert_eq!(encode_zero_skip(&[0xdf]).unwrap(), [0xdf, 0]);
        assert!(encode_zero_skip(&[0xe0]).is_err());
    }
}
