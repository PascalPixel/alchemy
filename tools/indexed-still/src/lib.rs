pub mod cli;
use import_asset::{indexed_png, Report, Rgb};
pub const STILL_WIDTH: usize = 256;
pub const STILL_HEIGHT: usize = 120;
pub const STILL_PALETTE_ENTRIES: usize = 128;
pub const STILL_PALETTE_BYTES: usize = STILL_PALETTE_ENTRIES * 2;

type Result<T> = std::result::Result<T, Error>;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);

impl std::fmt::Display for Error {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        formatter.write_str(&self.0)
    }
}

fn err<T>(message: impl Into<String>) -> Result<T> {
    Err(Error(message.into()))
}

fn put(bits: &mut Vec<u8>, value: u32, count: usize) {
    for index in 0..count {
        bits.push(((value >> index) & 1) as u8);
    }
}

pub fn encode_delta7(pixels: &[u8]) -> Result<Vec<u8>> {
    let mut bits = Vec::new();
    let mut previous = 0u8;
    for &pixel in pixels {
        if pixel > 0x7f {
            return err("delta7 pixel exceeds seven bits");
        }
        let delta = pixel.wrapping_sub(previous) & 0x7f;
        if delta == 0 {
            put(&mut bits, 0, 2);
        } else if delta <= 2 {
            put(&mut bits, 1, 1);
            put(&mut bits, 0, 1);
            put(&mut bits, 0, 1);
            put(&mut bits, u32::from(delta - 1), 1);
        } else if delta <= 10 {
            put(&mut bits, 1, 1);
            put(&mut bits, 1, 1);
            put(&mut bits, 0, 1);
            put(&mut bits, u32::from(delta - 3), 3);
        } else if delta <= 26 {
            put(&mut bits, 0, 1);
            put(&mut bits, 1, 1);
            put(&mut bits, 0, 1);
            put(&mut bits, 0, 1);
            put(&mut bits, u32::from(delta - 11), 4);
        } else if delta <= 101 {
            put(&mut bits, 0, 1);
            put(&mut bits, 1, 1);
            put(&mut bits, 1, 1);
            put(&mut bits, u32::from(pixel), 7);
        } else if delta <= 117 {
            put(&mut bits, 0, 1);
            put(&mut bits, 1, 1);
            put(&mut bits, 0, 1);
            put(&mut bits, 1, 1);
            put(&mut bits, u32::from(117 - delta), 4);
        } else if delta <= 125 {
            put(&mut bits, 1, 1);
            put(&mut bits, 1, 1);
            put(&mut bits, 1, 1);
            put(&mut bits, u32::from(125 - delta), 3);
        } else {
            put(&mut bits, 1, 1);
            put(&mut bits, 0, 1);
            put(&mut bits, 1, 1);
            put(&mut bits, u32::from(127 - delta), 1);
        }
        previous = pixel;
    }
    let mut result = vec![0u8; bits.len().div_ceil(16) * 2];
    for (index, bit) in bits.into_iter().enumerate() {
        result[index / 8] |= bit << (index & 7);
    }
    Ok(result)
}

fn palette_bytes(palette: &[Rgb]) -> Result<Vec<u8>> {
    if palette.len() != STILL_PALETTE_ENTRIES {
        return err("still PNG must contain exactly 128 palette entries");
    }
    let mut result = Vec::with_capacity(STILL_PALETTE_BYTES);
    for &[red, green, blue] in palette {
        if red & 7 != 0 || green & 7 != 0 || blue & 7 != 0 {
            return err("still palette channels must be exact five-bit values");
        }
        let value = u16::from(red >> 3) | (u16::from(green >> 3) << 5) | (u16::from(blue >> 3) << 10);
        result.extend_from_slice(&value.to_le_bytes());
    }
    Ok(result)
}

pub fn build_still(image: &[u8]) -> Result<(Vec<u8>, Report)> {
    let decoded = indexed_png(image).map_err(|error| Error(error.0))?;
    if decoded.width as usize != STILL_WIDTH || decoded.height as usize != STILL_HEIGHT {
        return err("still PNG must be 256x120");
    }
    let colors = palette_bytes(&decoded.palette)?;
    let pixels: Vec<u8> = decoded.pixels.into_iter().map(|pixel| pixel as u8).collect();
    let encoded = encode_delta7(&pixels)?;
    let mut result = colors;
    result.extend_from_slice(&encoded);
    let mut report = Report::default();
    report.set("width", STILL_WIDTH as f64);
    report.set("height", STILL_HEIGHT as f64);
    report.set("palette_entries", decoded.palette.len() as f64);
    report.set("bits", (encoded.len() * 8) as f64);
    report.set("encoded_bytes", encoded.len() as f64);
    Ok((result, report))
}
