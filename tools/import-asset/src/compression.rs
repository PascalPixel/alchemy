//! Pixel compression formats; no resource addresses or image-size defaults.
use crate::{indexed_png, AssetError};

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

pub fn encode_mtf4(pixels: &[u8]) -> Result<Vec<u8>, AssetError> {
    const PREFIX: [&str; 15] = [
        "00",
        "010",
        "011",
        "100",
        "101",
        "110",
        "11100",
        "11101",
        "11110",
        "1111100",
        "1111101",
        "1111110",
        "111111100",
        "111111101",
        "111111110",
    ];
    let mut table: [u8; 16] = std::array::from_fn(|i| i as u8);
    let mut bits = Bits::default();
    for &pixel in pixels {
        let index = table
            .iter()
            .position(|&p| p == pixel)
            .ok_or_else(|| AssetError("MTF4 pixel exceeds four bits".into()))?;
        bits.put(u32::from(index != 0), 1);
        if index != 0 {
            for bit in PREFIX[index - 1].bytes() {
                bits.put(u32::from(bit - b'0'), 1);
            }
            table[..=index].rotate_right(1);
        }
    }
    bits.put(1023, 10);
    bits.align(8, 1);
    Ok(bits.bytes)
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

pub fn delta7_image(
    image: &[u8],
    width: usize,
    height: usize,
    colors: usize,
) -> Result<Vec<u8>, AssetError> {
    let image = indexed_png(image)?;
    if image.width as usize != width
        || image.height as usize != height
        || image.palette.len() != colors
        || !(1..=128).contains(&colors)
    {
        return Err(AssetError("delta7 image layout differs".into()));
    }
    let mut output = Vec::new();
    for [red, green, blue] in image.palette {
        if red & 7 != 0 || green & 7 != 0 || blue & 7 != 0 {
            return Err(AssetError(
                "palette channels must be exact five-bit values".into(),
            ));
        }
        output.extend(
            (u16::from(red >> 3) | u16::from(green >> 3) << 5 | u16::from(blue >> 3) << 10)
                .to_le_bytes(),
        );
    }
    let pixels: Vec<u8> = image.pixels.into_iter().map(|p| p as u8).collect();
    output.extend(encode_delta7(&pixels)?);
    Ok(output)
}

#[test]
fn compression_checks_pixel_domains_and_padding() {
    assert_eq!(encode_mtf4(&[]).unwrap(), [255, 255]);
    assert_eq!(encode_mtf4(&[0]).unwrap(), [254, 255]);
    assert_eq!(encode_mtf4(&[1]).unwrap(), [249, 255]);
    assert_eq!(encode_delta7(&[0]).unwrap(), [0, 0]);
    assert_eq!(encode_delta7(&[1]).unwrap(), [1, 0]);
    assert_eq!(encode_delta7(&[127]).unwrap(), [5, 0]);
    assert!(encode_mtf4(&[16]).is_err());
    assert!(encode_delta7(&[128]).is_err());
    for (pixel, expected) in [
        (2, [9, 0]),
        (3, [3, 0]),
        (10, [59, 0]),
        (11, [2, 0]),
        (26, [242, 0]),
        (27, [222, 0]),
        (101, [46, 3]),
        (102, [250, 0]),
        (117, [10, 0]),
        (118, [63, 0]),
        (125, [7, 0]),
        (126, [13, 0]),
    ] {
        assert_eq!(encode_delta7(&[pixel]).unwrap(), expected);
    }
    assert_eq!(encode_delta7(&[127, 0]).unwrap(), [21, 0]);
    assert_eq!(encode_mtf4(&[1, 1]).unwrap(), [241, 255]);
    let entries = [0x34, 0x12, 0x78, 0x56];
    assert_eq!(
        encode_tilemap_delta(&entries, 0).unwrap(),
        [0, 0x34, 0x12, 0x78, 0x56]
    );
    assert_eq!(
        encode_tilemap_delta(&entries, 2).unwrap(),
        [2, 0x34, 0x12, 0x4c, 0x44]
    );
    assert_eq!(
        encode_tilemap_delta(&entries, 1).unwrap(),
        [1, 0x12, 0x44, 0x34, 0x4c]
    );
    assert!(encode_tilemap_delta(&entries, 3).is_err());
    assert!(encode_tilemap_delta(&entries[..3], 0).is_err());
    assert!(encode_tilemap_delta(&[], 0).is_err());
    assert_eq!(encode_zero_skip(&[]).unwrap(), [0]);
    assert_eq!(
        encode_zero_skip(&[1, 0, 0, 0, 2, 0, 0]).unwrap(),
        [1, 0xe2, 2, 0xe1, 0]
    );
    assert_eq!(encode_zero_skip(&[0; 33]).unwrap(), [0xff, 0xe0, 0]);
    assert!(encode_zero_skip(&[0xe0]).is_err());
}

#[test]
fn delta7_image_uses_declared_layout() {
    let mut png = Vec::new();
    {
        let mut encoder = png::Encoder::new(&mut png, 8, 8);
        encoder.set_color(png::ColorType::Indexed);
        encoder.set_depth(png::BitDepth::Eight);
        encoder.set_palette(vec![0, 0, 0, 8, 0, 0]);
        let mut writer = encoder.write_header().unwrap();
        writer.write_image_data(&[1; 64]).unwrap();
    }
    let mut expected = vec![0, 0, 1, 0, 1];
    expected.resize(22, 0);
    assert_eq!(delta7_image(&png, 8, 8, 2).unwrap(), expected);
    assert!(delta7_image(&png, 16, 8, 2).is_err());
    assert!(delta7_image(&png, 8, 8, 128).is_err());
}
