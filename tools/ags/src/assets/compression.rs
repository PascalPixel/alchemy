use super::AssetError;

#[derive(Default)]
struct Bits {
    bytes: Vec<u8>,
    len: usize,
}
impl Bits {
    fn put(&mut self, value: u32, count: usize) {
        for bit in 0..count {
            if self.len.is_multiple_of(8) {
                self.bytes.push(0);
            }
            self.bytes[self.len / 8] |= (((value >> bit) & 1) as u8) << (self.len % 8);
            self.len += 1;
        }
    }
    fn align(&mut self, bits: usize, fill: u32) {
        while !self.len.is_multiple_of(bits) {
            self.put(fill, 1);
        }
    }
}

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
    bits.put(0, 1);
    bits.align(16, 0);
    Ok(bits.bytes)
}

pub fn encode_tilemap_delta(entries: &[u8], mode: u8) -> Result<Vec<u8>, AssetError> {
    if mode > 2 || entries.is_empty() || !entries.len().is_multiple_of(2) {
        return Err(AssetError(
            "tilemap delta requires mode 0, 1 or 2 and whole 16-bit entries".into(),
        ));
    }
    let mut previous = 0u16;
    let transformed: Vec<u16> = entries
        .as_chunks::<2>().0.iter()
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

/// Halfword LZ, as Resource_DecompressHalfwords reads it: a little-endian
/// flag word ahead of every 16 tokens, its bits taken from the top; a clear
/// bit is a literal halfword, a set bit a copy word `distance << 5 |
/// (length - 2)` (length 2-33, distance 1-2047 halfwords back), and a
/// zero copy word ends the stream. One rule: the longest match, the
/// nearest on a tie, a literal when no match reaches two halfwords.
pub fn encode_halfword_lz(data: &[u8]) -> Result<Vec<u8>, AssetError> {
    if !data.len().is_multiple_of(2) {
        return Err(AssetError("halfword LZ input is not whole halfwords".into()));
    }
    let words: Vec<u16> = data
        .as_chunks::<2>().0.iter()
        .map(|pair| u16::from_le_bytes([pair[0], pair[1]]))
        .collect();
    let mut tokens: Vec<(bool, u16)> = Vec::new();
    let mut at = 0;
    while at < words.len() {
        let (mut length, mut distance) = (0, 0);
        for back in 1..=at.min(2047) {
            let mut run = 0;
            while run < 33 && at + run < words.len() && words[at + run] == words[at + run - back] {
                run += 1;
            }
            if run > length {
                (length, distance) = (run, back);
            }
        }
        if length >= 2 {
            tokens.push((true, (distance << 5 | (length - 2)) as u16));
            at += length;
        } else {
            tokens.push((false, words[at]));
            at += 1;
        }
    }
    tokens.push((true, 0));
    let mut output = Vec::new();
    for group in tokens.chunks(16) {
        let flags = group
            .iter()
            .enumerate()
            .fold(0u16, |flags, (index, &(copy, _))| flags | (u16::from(copy) << (15 - index)));
        output.extend(flags.to_le_bytes());
        for &(_, word) in group {
            output.extend(word.to_le_bytes());
        }
    }
    Ok(output)
}

#[cfg(test)]
mod halfword_lz_tests {
    use super::encode_halfword_lz;

    fn decode(stream: &[u8]) -> Vec<u8> {
        let word = |at: usize| u16::from_le_bytes([stream[at], stream[at + 1]]);
        let (mut at, mut out): (usize, Vec<u16>) = (0, Vec::new());
        loop {
            let flags = word(at);
            at += 2;
            for index in 0..16 {
                let token = word(at);
                at += 2;
                if flags & (0x8000 >> index) == 0 {
                    out.push(token);
                    continue;
                }
                if token == 0 {
                    return out.iter().flat_map(|w| w.to_le_bytes()).collect();
                }
                let (distance, length) = (usize::from(token >> 5), usize::from(token & 31) + 2);
                for _ in 0..length {
                    out.push(out[out.len() - distance]);
                }
            }
        }
    }

    #[test]
    fn literals_copies_and_the_end_word() {
        let data: Vec<u8> = [1u16, 2, 1, 2, 1, 2, 7]
            .iter()
            .flat_map(|w| w.to_le_bytes())
            .collect();
        let stream = encode_halfword_lz(&data).unwrap();
        // two literals, a copy of four from two back, a literal, the end
        assert_eq!(stream, [0x00, 0x28, 1, 0, 2, 0, 0x42, 0, 7, 0, 0, 0]);
        assert_eq!(decode(&stream), data);
    }

    #[test]
    fn long_runs_round_trip_across_flag_groups() {
        let data: Vec<u8> = (0..400u16)
            .map(|i| (i % 7) * 3 + (i / 50))
            .flat_map(|w| w.to_le_bytes())
            .collect();
        assert_eq!(decode(&encode_halfword_lz(&data).unwrap()), data);
        assert!(encode_halfword_lz(&[1, 2, 3]).is_err());
    }
}
