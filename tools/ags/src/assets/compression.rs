//! Pixel compression formats; no resource addresses or image-size defaults.
use super::AssetError;

#[derive(Default)]
struct Bits {
    bytes: Vec<u8>,
    len: usize,
}
impl Bits {
    fn put(&mut self, value: u32, count: usize) {
        for bit in 0..count {
            if self.len % 8 == 0 {
                self.bytes.push(0);
            }
            self.bytes[self.len / 8] |= (((value >> bit) & 1) as u8) << (self.len % 8);
            self.len += 1;
        }
    }
    fn align(&mut self, bits: usize, fill: u32) {
        while self.len % bits != 0 {
            self.put(fill, 1);
        }
    }
}

/// Zero-skip byte stream: literals 1..=0xdf, runs of up to 32 zeros as
/// `0xdf + count`, and a terminating zero byte.
pub fn encode_zero_skip(pixels: &[u8]) -> Result<Vec<u8>, AssetError> {
    let mut output = Vec::new();
    let mut cursor = 0;
    while cursor < pixels.len() {
        if pixels[cursor] != 0 {
            if pixels[cursor] > 0xdf {
                return Err(AssetError("zero-skip literal exceeds 0xdf".into()));
            }
            output.push(pixels[cursor]);
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

pub fn encode_delta7(pixels: &[u8]) -> Result<Vec<u8>, AssetError> {
    let mut bits = Bits::default();
    let mut previous = 0u8;
    for &pixel in pixels {
        if pixel > 127 {
            return Err(AssetError("delta7 pixel exceeds seven bits".into()));
        }
        let delta = pixel.wrapping_sub(previous) & 127;
        let (prefix, prefix_bits, value, value_bits) = match delta {
            0 => (0, 2, 0, 0),
            1..=2 => (1, 3, delta - 1, 1),
            3..=10 => (3, 3, delta - 3, 3),
            11..=26 => (2, 4, delta - 11, 4),
            27..=101 => (6, 3, pixel, 7),
            102..=117 => (10, 4, 117 - delta, 4),
            118..=125 => (7, 3, 125 - delta, 3),
            _ => (5, 3, 127 - delta, 1),
        };
        bits.put(prefix, prefix_bits);
        bits.put(u32::from(value), value_bits);
        previous = pixel;
    }
    // The writer flushes its pending halfword even when the last code filled
    // the one before: a stream ending on a halfword boundary gains an empty
    // halfword.
    bits.put(0, 1);
    bits.align(16, 0);
    Ok(bits.bytes)
}

/// Delta-code little-endian 16-bit tile entries behind a mode byte: mode 0
/// keeps the entries, mode 2 XORs each entry with its predecessor, and mode 1
/// writes that XOR stream as a plane of high bytes followed by the low bytes.
pub fn encode_tilemap_delta(entries: &[u8], mode: u8) -> Result<Vec<u8>, AssetError> {
    if mode > 2 || entries.is_empty() || entries.len() % 2 != 0 {
        return Err(AssetError(
            "tilemap delta requires mode 0, 1 or 2 and whole 16-bit entries".into(),
        ));
    }
    let mut previous = 0u16;
    let transformed: Vec<u16> = entries
        .chunks_exact(2)
        .map(|entry| {
            let value = u16::from_le_bytes([entry[0], entry[1]]);
            let coded = if mode == 0 { value } else { value ^ previous };
            previous = value;
            coded
        })
        .collect();
    let mut output = vec![mode];
    if mode == 1 {
        output.extend(transformed.iter().map(|value| (value >> 8) as u8));
        output.extend(transformed.iter().map(|value| *value as u8));
    } else {
        output.extend(transformed.iter().flat_map(|value| value.to_le_bytes()));
    }
    Ok(output)
}

/// The 4-bit icon coder's prefix code for each move-to-front position, then
/// its terminator, in the order the bits are read.
const ICON4_CODES: [&str; 17] = [
    "0",
    "100",
    "1010",
    "1011",
    "1100",
    "1101",
    "1110",
    "111100",
    "111101",
    "111110",
    "11111100",
    "11111101",
    "11111110",
    "1111111100",
    "1111111101",
    "1111111110",
    "1111111111",
];

/// 4-bit pixels as move-to-front positions over the sixteen values, each a
/// prefix code read least significant bit first, then the terminator; the
/// last byte's unused bits are set.
pub fn encode_icon4(pixels: &[u8]) -> Result<Vec<u8>, AssetError> {
    let mut table: [u8; 16] = std::array::from_fn(|i| i as u8);
    let mut bits = Bits::default();
    let mut put = |code: &str| {
        for bit in code.bytes() {
            bits.put(u32::from(bit - b'0'), 1);
        }
    };
    for &pixel in pixels {
        let index = table
            .iter()
            .position(|&p| p == pixel)
            .ok_or_else(|| AssetError("icon pixel exceeds four bits".into()))?;
        put(ICON4_CODES[index]);
        table[..=index].rotate_right(1);
    }
    put(ICON4_CODES[16]);
    bits.align(8, 1);
    Ok(bits.bytes)
}
