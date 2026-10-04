//! The compressors Camelot's resource packer ran: general and palette LZSS
//! and their tagged choice, the tag-2 tile compressor, and arena streams.
//! Every stream is compressed from its decoded bytes and the encoder's own
//! settings alone: no plan, token or recorded choice reaches an encoder.
use crate::assets::lz::{
    encode_general, encode_mtf4_lz, encode_palette, GeneralToken, Mtf4LzToken, PaletteGroup,
    PaletteOperation,
};
use std::collections::HashMap;

/// Palette LZ searches this many bytes back from the byte it encodes.
const PALETTE_WINDOW: usize = 4092;
/// The palette format's largest copy distance, which its look-ahead reaches.
const PALETTE_REACH: usize = 4095;
/// The longest copy each format encodes.
const GENERAL_LONGEST: usize = 137;
const PALETTE_LONGEST: usize = 272;
/// Tag-2 tile streams copy from at most this many bytes back.
const MTF4_WINDOW: usize = 4123;

/// How an LZSS compressor streams its input. It keeps `read_ahead` bytes of
/// input ahead of the byte it encodes in a ring of `window + read_ahead`
/// bytes and reads one more byte for each byte it consumes, so a copy may
/// start at any byte still in the ring, at most `max_distance` back. At the
/// end of the input reading stops and the ring's oldest byte stops
/// advancing: over the final `read_ahead` bytes the history grows by one
/// byte for each byte encoded, until `max_distance` limits it.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
struct Ring {
    window: usize,
    read_ahead: usize,
    max_distance: usize,
}
impl Ring {
    /// The oldest byte the ring holds while it encodes `position` of `size`
    /// input bytes: `read_ahead` bytes are read ahead unless the input ends.
    fn oldest(&self, position: usize, size: usize) -> usize {
        (position + self.read_ahead)
            .min(size)
            .saturating_sub(self.window + self.read_ahead)
    }
}

/// The packer's two LZSS compressors. General LZ has its window, read-ahead
/// and maximum distance; palette LZ its read-ahead, beside its fixed
/// 4,092-byte window and the format's largest distance, 4,095. These are
/// encoder options, kept beside the inputs that use them.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct LzMachine {
    general: Ring,
    palette: Ring,
}
impl LzMachine {
    /// A machine from its settings: the general ring, and how far the palette
    /// ring reads ahead.
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

/// One control: a literal byte, or a copy of `length` bytes from `distance`
/// bytes back.
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

/// Every position of each byte pair, for the nearest-longest search.
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
    /// The nearest longest copy at `position`, at most `longest` bytes, whose
    /// source is at least `lowest`, else a literal.
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

// DRAFT (end of stream, open): ☀️ Title_IntroGraphicsC (⚓️ Resource_Data017),
// 85,440 decoded bytes, differs from Camelot only in lazy deferrals at
// 85224, 85239, 85240 and 85264, inside the final 272-byte read-ahead.
// Camelot defers where `alt + 1 >= count + fol` says not to (fol would need
// to be at most 4, 2, 4 and 1 instead of 5, 6, 5 and 2), and defers again
// at 85240 straight after a deferral. It does not defer at 85247. Rules keyed
// on distance from the end (fol forced to 0 or 1, lazy always on, fol
// history cut to the last K bytes) and hiding copy interiors all fail or
// break the lighthouse picture, which matches today. The likeliest cause is
// stale ring bytes past the end that the matcher still compares.
/// Compress `decoded` on one ring with nearest-longest matches and one-byte
/// lazy evaluation. The encoder defers a copy to a literal when the copy at
/// the next byte (longer than two bytes) covers at least as far as the copy
/// and the token after it. Both look-ahead searches copy only from the
/// history of the current position, so their distance limit grows by the
/// look-ahead offset, up to the largest distance.
///
/// A deferral suppresses lazy evaluation of the match that follows it. The
/// palette encoder then resumes lazy evaluation; the general encoder never
/// does, so a general stream defers at most once.
fn lzss(decoded: &[u8], ring: Ring, palette: bool) -> Vec<Token> {
    let longest = if palette {
        PALETTE_LONGEST
    } else {
        GENERAL_LONGEST
    };
    // The lowest source a search at `at` may copy from, in the history whose
    // oldest byte is `oldest`.
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

/// General LZ, tagged 0.
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

/// Palette LZ without its tag: groups of eight operations and a terminator, a
/// group of eight literals written as a zero-flag block.
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

/// Palette LZ, tagged 1.
pub fn compress_tagged_palette(decoded: &[u8], machine: &LzMachine) -> Result<Vec<u8>, String> {
    let mut stream = vec![1];
    stream.extend(compress_palette(decoded, machine)?);
    Ok(stream)
}

/// A tagged resource stream, as the packer wrote every code overlay: the
/// smaller of the general and tagged palette encodings, the palette one on
/// ties.
pub fn compress_tagged(decoded: &[u8], machine: &LzMachine) -> Result<Vec<u8>, String> {
    let general = compress_general(decoded, machine)?;
    let palette = compress_tagged_palette(decoded, machine)?;
    Ok(if palette.len() <= general.len() {
        palette
    } else {
        general
    })
}

/// The tag-2 tile compressor's controls: greedy nearest-longest copies from
/// the last 4,123 bytes, and literals as two move-to-front nibble indices in
/// the narrowest of two, three or four bits that holds both. A copied byte
/// never enters the move-to-front table.
fn mtf4_tokens(decoded: &[u8]) -> Vec<Mtf4LzToken> {
    let matcher = Matcher::new(decoded);
    let mut table: [u8; 16] = std::array::from_fn(|index| index as u8);
    let mut position = 0;
    let mut tokens = Vec::new();
    while position < decoded.len() {
        let lowest = position.saturating_sub(MTF4_WINDOW);
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

/// A tag-2 tile stream.
pub fn compress_mtf4(decoded: &[u8]) -> Result<Vec<u8>, String> {
    encode_mtf4_lz(decoded, &mtf4_tokens(decoded)).map_err(|error| error.to_string())
}

/// One arena stream, whose copies read from `arena`, the bytes before it in
/// its container.
pub fn compress_arena(decoded: &[u8], arena: &[u8]) -> Result<Vec<u8>, String> {
    crate::assets::lz::compress_arena(decoded, arena).map_err(|error| error.to_string())
}
