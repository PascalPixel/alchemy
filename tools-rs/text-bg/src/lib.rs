// Text-background screen entries: a u16 per tile holding the tile index, two
// flip bits, and a palette bank.
//
// Ported from tools/lib/text_bg.ts. The TypeScript validated its inputs at
// runtime and threw; here the bounds that Rust's types cannot express return
// Result, and the ones they can (a u16 entry is always in range) are gone
// entirely. That is the shape most of this port takes: runtime checks become
// either signatures or errors, never silent truncation.

pub const TILE_INDEX_MASK: u16 = 0x03ff;
pub const HFLIP: u16 = 0x0400;
pub const VFLIP: u16 = 0x0800;
pub const PALETTE_SHIFT: u32 = 12;
pub const PALETTE_MASK: u16 = 0x0f;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct TextBgEntry {
    pub tile: u16,
    pub palette: u16,
    pub hflip: bool,
    pub vflip: bool,
}

#[derive(Debug, PartialEq, Eq)]
pub enum TextBgError {
    TileOutOfRange(u16),
    PaletteOutOfRange(u16),
    TileSizeMismatch { got: usize, want: usize },
}

impl std::fmt::Display for TextBgError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::TileOutOfRange(v) => {
                write!(f, "text-background tile index {v} is outside 0..1023")
            }
            Self::PaletteOutOfRange(v) => {
                write!(f, "text-background palette bank {v} is outside 0..15")
            }
            Self::TileSizeMismatch { got, want } => write!(
                f,
                "tile pixel count {got} differs from its square dimensions ({want})"
            ),
        }
    }
}

impl std::error::Error for TextBgError {}

/// A u16 cannot fall outside the entry range, so unlike the TypeScript this
/// needs no bounds check and cannot fail.
pub fn decode_entry(entry: u16) -> TextBgEntry {
    TextBgEntry {
        tile: entry & TILE_INDEX_MASK,
        palette: (entry >> PALETTE_SHIFT) & PALETTE_MASK,
        hflip: entry & HFLIP != 0,
        vflip: entry & VFLIP != 0,
    }
}

pub fn encode_entry(entry: TextBgEntry) -> Result<u16, TextBgError> {
    if entry.tile > TILE_INDEX_MASK {
        return Err(TextBgError::TileOutOfRange(entry.tile));
    }
    if entry.palette > PALETTE_MASK {
        return Err(TextBgError::PaletteOutOfRange(entry.palette));
    }
    Ok(entry.tile
        | if entry.hflip { HFLIP } else { 0 }
        | if entry.vflip { VFLIP } else { 0 }
        | entry.palette << PALETTE_SHIFT)
}

pub fn flip_tile<T: Copy>(
    pixels: &[T],
    hflip: bool,
    vflip: bool,
    size: usize,
) -> Result<Vec<T>, TextBgError> {
    if pixels.len() != size * size {
        return Err(TextBgError::TileSizeMismatch {
            got: pixels.len(),
            want: size * size,
        });
    }
    let mut result = Vec::with_capacity(pixels.len());
    for y in 0..size {
        for x in 0..size {
            let row = if vflip { size - 1 - y } else { y };
            let column = if hflip { size - 1 - x } else { x };
            result.push(pixels[row * size + column]);
        }
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn entry_round_trips() {
        let entry = TextBgEntry { tile: 0x2a5, palette: 13, hflip: true, vflip: true };
        let encoded = encode_entry(entry).unwrap();
        assert_eq!(decode_entry(encoded), entry);
    }

    #[test]
    fn flips_are_reversible() {
        let tile: Vec<u16> = (0..64).collect();
        let once = flip_tile(&tile, true, false, 8).unwrap();
        assert_eq!(flip_tile(&once, true, false, 8).unwrap(), tile);
        let once = flip_tile(&tile, false, true, 8).unwrap();
        assert_eq!(flip_tile(&once, false, true, 8).unwrap(), tile);
    }

    #[test]
    fn out_of_range_inputs_are_rejected() {
        let bad_tile = TextBgEntry { tile: 0x400, palette: 0, hflip: false, vflip: false };
        assert_eq!(encode_entry(bad_tile), Err(TextBgError::TileOutOfRange(0x400)));
        let bad_palette = TextBgEntry { tile: 0, palette: 16, hflip: false, vflip: false };
        assert_eq!(encode_entry(bad_palette), Err(TextBgError::PaletteOutOfRange(16)));
        assert!(flip_tile(&[0u8; 63], false, false, 8).is_err());
    }
}
