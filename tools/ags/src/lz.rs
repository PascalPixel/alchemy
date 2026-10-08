use crate::assets::lz::{
    encode_general, encode_mtf4_lz, encode_palette, GeneralToken, Mtf4LzToken, PaletteGroup,
    PaletteOperation,
};
use std::collections::HashMap;

const PALETTE_WINDOW: usize = 4092;
const PALETTE_REACH: usize = 4095;
const GENERAL_LONGEST: usize = 137;
const PALETTE_LONGEST: usize = 272;
const MTF4_WINDOW: usize = 4123;
/// The tile packer's ring holds 0x1100 bytes: the window and a read-ahead,
/// so near the end of input it still reaches PACKER's 4126 bytes back.
const MTF4: Ring = Ring {
    window: MTF4_WINDOW,
    read_ahead: 0x1100 - MTF4_WINDOW,
    max_distance: 4126,
};

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
struct Ring {
    window: usize,
    read_ahead: usize,
    max_distance: usize,
}
impl Ring {
    fn oldest(&self, position: usize, size: usize) -> usize {
        (position + self.read_ahead)
            .min(size)
            .saturating_sub(self.window + self.read_ahead)
    }
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct LzMachine {
    general: Ring,
    palette: Ring,
}
impl LzMachine {
    pub const fn new(
        window: usize,
        read_ahead: usize,
        max_distance: usize,
        palette_read_ahead: usize,
    ) -> Self {
        Self {
            general: Ring {
                window,
                read_ahead,
                max_distance,
            },
            palette: Ring {
                window: PALETTE_WINDOW,
                read_ahead: palette_read_ahead,
                max_distance: PALETTE_REACH,
            },
        }
    }
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
enum Token {
    Literal,
    Copy { length: usize, distance: usize },
}
impl Token {
    fn length(self) -> usize {
        match self {
            Token::Literal => 1,
            Token::Copy { length, .. } => length,
        }
    }
}

struct Matcher<'a> {
    data: &'a [u8],
    pairs: HashMap<u16, Vec<usize>>,
}
impl<'a> Matcher<'a> {
    fn new(data: &'a [u8]) -> Self {
        let mut pairs = HashMap::<u16, Vec<usize>>::new();
        for (position, pair) in data.windows(2).enumerate() {
            pairs
                .entry(u16::from_le_bytes([pair[0], pair[1]]))
                .or_default()
                .push(position);
        }
        Self { data, pairs }
    }
    fn search(&self, position: usize, longest: usize, lowest: usize) -> Token {
        let maximum = longest.min(self.data.len() - position);
        let mut length = 1;
        let mut distance = 0;
        if let Some(pair) = self.data.get(position..position + 2) {
            if let Some(positions) = self.pairs.get(&u16::from_le_bytes([pair[0], pair[1]])) {
                let start = positions.partition_point(|p| *p < lowest);
                let end = positions.partition_point(|p| *p < position);
                for source in positions[start..end].iter().rev() {
                    if self.data[*source + length] != self.data[position + length] {
                        continue;
                    }
                    let mut count = 2;
                    while count < maximum
                        && position + count < self.data.len()
                        && self.data[*source + count] == self.data[position + count]
                    {
                        count += 1;
                    }
                    if count > length {
                        length = count;
                        distance = position - source;
                    }
                    if length == maximum {
                        break;
                    }
                }
            }
        }
        if length >= 2 {
            Token::Copy { length, distance }
        } else {
            Token::Literal
        }
    }
}

fn lzss(decoded: &[u8], ring: Ring, palette: bool) -> Vec<Token> {
    let longest = if palette {
        PALETTE_LONGEST
    } else {
        GENERAL_LONGEST
    };
    let lowest = |oldest: usize, at: usize| oldest.max(at.saturating_sub(ring.max_distance));
    let matcher = Matcher::new(decoded);
    let mut deferred = false;
    let mut position = 0;
    let mut result = Vec::new();
    while position < decoded.len() {
        let oldest = ring.oldest(position, decoded.len());
        let mut token = matcher.search(position, longest, lowest(oldest, position));
        let count = token.length();
        let lazy = !deferred;
        if palette {
            deferred = false;
        }
        if lazy && count > 1 && position + count < decoded.len() {
            let alternative = matcher
                .search(position + 1, longest, lowest(oldest, position + 1))
                .length();
            let following = matcher
                .search(position + count, longest, lowest(oldest, position + count))
                .length();
            if alternative > 2 && alternative + 1 >= count + following {
                token = Token::Literal;
                deferred = true;
            }
        }
        position += token.length();
        result.push(token);
    }
    result
}

pub fn compress_general(decoded: &[u8], machine: &LzMachine) -> Result<Vec<u8>, String> {
    let tokens = lzss(decoded, machine.general, false)
        .into_iter()
        .map(|token| match token {
            Token::Literal => GeneralToken::Literal(1),
            Token::Copy { length, distance } => GeneralToken::Copy {
                length: length as u32,
                distance: distance as u32,
            },
        })
        .collect::<Vec<_>>();
    encode_general(decoded, &tokens).map_err(|error| error.to_string())
}

pub(crate) fn compress_palette(decoded: &[u8], machine: &LzMachine) -> Result<Vec<u8>, String> {
    let mut operations = lzss(decoded, machine.palette, true)
        .into_iter()
        .map(|token| match token {
            Token::Literal => PaletteOperation::Literal,
            Token::Copy { length, distance } => PaletteOperation::Copy {
                length: length as u32,
                distance: distance as u32,
            },
        })
        .collect::<Vec<_>>();
    operations.push(PaletteOperation::End);
    let groups = operations
        .chunks(8)
        .map(|group| {
            if group.len() == 8 && group.iter().all(|op| *op == PaletteOperation::Literal) {
                PaletteGroup::Zeros
            } else {
                PaletteGroup::Group(group.to_vec())
            }
        })
        .collect::<Vec<_>>();
    encode_palette(decoded, &groups).map_err(|error| error.to_string())
}

pub fn compress_tagged_palette(decoded: &[u8], machine: &LzMachine) -> Result<Vec<u8>, String> {
    let mut stream = vec![1];
    stream.extend(compress_palette(decoded, machine)?);
    Ok(stream)
}

pub fn compress_tagged(decoded: &[u8], machine: &LzMachine) -> Result<Vec<u8>, String> {
    let general = compress_general(decoded, machine)?;
    let palette = compress_tagged_palette(decoded, machine)?;
    Ok(if palette.len() <= general.len() {
        palette
    } else {
        general
    })
}

fn mtf4_tokens(decoded: &[u8]) -> Vec<Mtf4LzToken> {
    let matcher = Matcher::new(decoded);
    let mut table: [u8; 16] = std::array::from_fn(|index| index as u8);
    let mut position = 0;
    let mut tokens = Vec::new();
    while position < decoded.len() {
        let lowest = MTF4
            .oldest(position, decoded.len())
            .max(position.saturating_sub(MTF4.max_distance));
        match matcher.search(position, GENERAL_LONGEST, lowest) {
            Token::Copy { length, distance } => {
                tokens.push(Mtf4LzToken::Copy {
                    length: length as u32,
                    distance: distance as u32,
                });
                position += length;
            }
            Token::Literal => {
                let byte = decoded[position];
                let mut largest = 0usize;
                for nibble in [byte & 15, byte >> 4] {
                    let index = table.iter().position(|n| *n == nibble).unwrap();
                    largest = largest.max(index);
                    table[..=index].rotate_right(1);
                }
                let width = (usize::BITS - largest.leading_zeros()).max(2);
                tokens.push(Mtf4LzToken::Literal { width });
                position += 1;
            }
        }
    }
    tokens
}

pub fn compress_mtf4(decoded: &[u8]) -> Result<Vec<u8>, String> {
    encode_mtf4_lz(decoded, &mtf4_tokens(decoded)).map_err(|error| error.to_string())
}

pub fn compress_arena(decoded: &[u8], arena: &[u8]) -> Result<Vec<u8>, String> {
    crate::assets::lz::compress_arena(decoded, arena).map_err(|error| error.to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    /// Noise with one 8-byte run repeated 4126 bytes after itself, ending at `end`.
    fn noise_with_echo(end: usize) -> Vec<u8> {
        let mut seed = 0x1234_5678u32;
        let mut data: Vec<u8> = (0..0x4000)
            .map(|_| {
                seed = seed.wrapping_mul(1_103_515_245).wrapping_add(12_345);
                (seed >> 16) as u8
            })
            .collect();
        let echo = [0x5a, 0xa5, 0x3c, 0xc3, 0x0f, 0xf0, 0x69, 0x96];
        data[end - 8 - 4126..end - 4126].copy_from_slice(&echo);
        data[end - 8..end].copy_from_slice(&echo);
        data
    }

    fn copy_at(data: &[u8], at: usize) -> Option<(u32, u32)> {
        let mut position = 0;
        for token in mtf4_tokens(data) {
            let length = match token {
                Mtf4LzToken::Literal { .. } => 1,
                Mtf4LzToken::Copy { length, distance } if position == at => {
                    return Some((length, distance));
                }
                Mtf4LzToken::Copy { length, .. } => length as usize,
            };
            position += length;
            if position > at {
                return None;
            }
        }
        None
    }

    #[test]
    fn the_tile_packer_reaches_further_back_only_near_the_end() {
        let late = noise_with_echo(0x4000 - 100);
        assert_eq!(copy_at(&late, 0x4000 - 108), Some((8, 4126)));
        let middle = noise_with_echo(0x2000);
        assert_ne!(copy_at(&middle, 0x2000 - 8).map(|copy| copy.1), Some(4126));
        for data in [late, middle] {
            let mut stream = compress_mtf4(&data).unwrap();
            stream.resize(stream.len() + 4, 0);
            let (decoded, _) =
                crate::assets::lz::decode_mtf4_lz(&stream, 0, stream.len(), 0x4000).unwrap();
            assert_eq!(decoded, data);
        }
    }
}
