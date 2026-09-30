//! The compressors Camelot's resource packer ran: general and palette LZSS
//! and their tagged choice, the tag-2 tile compressor, and arena streams.
//! Every stream is compressed from its decoded bytes and the encoder's own
//! settings alone: no plan, token or recorded choice reaches an encoder.
use psynergy::assets::lz::{
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
// stale ring bytes past the end that the matcher still compares. The
// TypeScript port used to test this is tools/ags/drafts/palette_lz_model.ts.
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
pub fn compress_palette(decoded: &[u8], machine: &LzMachine) -> Result<Vec<u8>, String> {
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
    psynergy::assets::lz::compress_arena(decoded, arena).map_err(|error| error.to_string())
}

/// A container of arena streams, one per frame: each stream reads the
/// streams before it as its arena and is padded with zeros to `alignment`.
/// An empty frame is a bare split halfword.
pub fn compress_arena_sequence(frames: &[Vec<u8>], alignment: usize) -> Result<Vec<u8>, String> {
    if alignment == 0 {
        return Err("stream alignment must be positive".into());
    }
    let mut built = Vec::new();
    for frame in frames {
        let mut stream = compress_arena(frame, &built)?;
        stream.resize(stream.len().next_multiple_of(alignment), 0);
        built.extend(stream);
    }
    Ok(built)
}

#[cfg(test)]
mod tests {
    use super::*;
    use psynergy::assets::lz::{decode_arena, decode_general, decode_mtf4_lz, decode_palette};

    /// A machine for tests of the compressor's rules: its general ring, and a
    /// palette ring reading `palette_read_ahead` bytes ahead.
    const fn machine(
        window: usize,
        read_ahead: usize,
        max_distance: usize,
        palette_read_ahead: usize,
    ) -> LzMachine {
        LzMachine::new(window, read_ahead, max_distance, palette_read_ahead)
    }
    /// A machine whose general history spans every short test input.
    const WIDE: LzMachine = machine(64, 8, 72, 0);

    /// `size` distinct bytes, except that the pair at `copy` repeats the pair
    /// at `source`.
    fn distinct_with_copy(size: usize, source: usize, copy: usize) -> Vec<u8> {
        let mut decoded = (0..size as u8).collect::<Vec<_>>();
        decoded[copy] = decoded[source];
        decoded[copy + 1] = decoded[source + 1];
        decoded
    }
    /// The token that `ring` emits at `at`.
    fn token_at(decoded: &[u8], ring: Ring, palette: bool, at: usize) -> Token {
        let mut position = 0;
        for token in lzss(decoded, ring, palette) {
            if position == at {
                return token;
            }
            position += token.length();
        }
        unreachable!()
    }
    fn copy(length: usize, distance: usize) -> Token {
        Token::Copy { length, distance }
    }

    #[test]
    fn general_history_grows_over_the_final_read_ahead_bytes() {
        // A 16-byte window and 4 bytes read ahead: a 20-byte ring.
        let general = machine(16, 4, 32, 0).general;
        // The ring's oldest byte trails the window until reading stops.
        assert_eq!(general.oldest(10, 40), 0);
        assert_eq!(general.oldest(30, 40), 14);
        assert_eq!(general.oldest(36, 40), 20);
        assert_eq!(general.oldest(39, 40), 20);
        // A pair 17 bytes back is out of reach with four bytes left and in
        // reach with three; with two left the history reaches 18 bytes.
        for (left, distance, expected) in [
            (4, 17, Token::Literal),
            (3, 17, copy(2, 17)),
            (2, 18, copy(2, 18)),
            (2, 19, Token::Literal),
        ] {
            let at = 40 - left;
            let decoded = distinct_with_copy(40, at - distance, at);
            assert_eq!(token_at(&decoded, general, false, at), expected);
        }
        // Mid-stream the same pair 17 bytes back stays out of reach.
        let decoded = distinct_with_copy(60, 13, 30);
        assert_eq!(token_at(&decoded, general, false, 30), Token::Literal);
    }

    #[test]
    fn general_history_growth_stops_at_the_maximum_distance() {
        // Eight bytes read ahead open 22 bytes of history two bytes before
        // the end, but the machine copies at most 18 bytes back.
        let general = machine(16, 8, 18, 0).general;
        assert_eq!(general.oldest(38, 40), 16);
        for (distance, expected) in [(18, copy(2, 18)), (19, Token::Literal)] {
            let decoded = distinct_with_copy(40, 38 - distance, 38);
            assert_eq!(token_at(&decoded, general, false, 38), expected);
        }
    }

    #[test]
    fn general_lazy_look_ahead_searches_the_current_history() {
        // At 30 a two-byte copy (distance 5); at 31 a three-byte copy from
        // 14, the ring's oldest byte while 30 is encoded (distance 17); at
        // 32 a two-byte copy. Distinct bytes elsewhere.
        let mut decoded = (0..50u8).collect::<Vec<_>>();
        decoded[26] = 14;
        decoded[30] = 25;
        decoded[31..34].copy_from_slice(&[14, 15, 16]);
        let general = machine(16, 4, 18, 0).general;
        assert_eq!(general.oldest(30, 50), 14);
        // Both look-aheads search from 30's oldest byte, so the copy at 31
        // covers as far as the two greedy copies and 30 defers. Once 31 is
        // encoded, byte 14 has left the ring: the copy is gone.
        for at in [30, 31] {
            assert_eq!(token_at(&decoded, general, false, at), Token::Literal);
        }
        // The look-ahead also copies at most the maximum distance back.
        let general = machine(16, 4, 16, 0).general;
        assert_eq!(token_at(&decoded, general, false, 30), copy(2, 5));
    }

    /// 4,400 bytes whose byte pairs are distinct.
    fn distinct_pairs() -> Vec<u8> {
        let mut decoded = Vec::new();
        let mut value = 0u8;
        for index in 0..4400usize {
            decoded.push(value);
            value = value.wrapping_add((2 * (index / 256) + 1) as u8);
        }
        decoded
    }

    /// Palette LZ at `at` of distinct byte pairs, except for a two-byte copy
    /// at `at` (distance 100), a three-byte copy at `at + 1` (distance 201)
    /// and a three-byte copy at `at + 2` from `far` bytes back.
    fn planted_palette_token(far: usize) -> Token {
        let mut decoded = distinct_pairs();
        let at = 4300;
        let (near, alternative, following) = (at - 100, at - 200, at + 2 - far);
        decoded[near + 1] = decoded[alternative];
        decoded[following] = decoded[alternative + 1];
        decoded[following + 1] = decoded[alternative + 2];
        decoded[at] = decoded[near];
        decoded[at + 1] = decoded[alternative];
        decoded[at + 2] = decoded[alternative + 1];
        decoded[at + 3] = decoded[alternative + 2];
        decoded[at + 4] = decoded[following + 2];
        token_at(&decoded, WIDE.palette, true, at)
    }

    #[test]
    fn lookahead_matches_reach_past_the_window_by_their_offset() {
        // The following copy at `at + 2` is visible from `at` up to distance
        // 4092 + 2, so the two greedy copies cover more and the copy stays.
        assert_eq!(planted_palette_token(4094), copy(2, 100));
        // One byte further it is out of reach and the copy is deferred.
        assert_eq!(planted_palette_token(4095), Token::Literal);
    }

    /// The palette token that a ring reading `read_ahead` bytes ahead emits
    /// `left` bytes before the end of distinct byte pairs, except that the
    /// pair there repeats the pair `distance` bytes back.
    fn palette_end_token(read_ahead: usize, left: usize, distance: usize) -> Token {
        let mut decoded = distinct_pairs();
        let at = decoded.len() - left;
        decoded[at] = decoded[at - distance];
        decoded[at + 1] = decoded[at - distance + 1];
        token_at(&decoded, machine(64, 8, 72, read_ahead).palette, true, at)
    }

    #[test]
    fn palette_history_grows_over_the_final_read_ahead_bytes() {
        // Mid-stream and without read-ahead the window is 4,092 bytes.
        assert_eq!(palette_end_token(0, 8, 4092), copy(2, 4092));
        assert_eq!(palette_end_token(0, 8, 4093), Token::Literal);
        assert_eq!(palette_end_token(8, 9, 4093), Token::Literal);
        // Reading 8 bytes ahead, the history grows by one byte for each of
        // the last 8 bytes encoded, up to the format's distance of 4,095.
        assert_eq!(palette_end_token(8, 7, 4093), copy(2, 4093));
        assert_eq!(palette_end_token(8, 6, 4094), copy(2, 4094));
        assert_eq!(palette_end_token(8, 6, 4095), Token::Literal);
        assert_eq!(palette_end_token(8, 2, 4095), copy(2, 4095));
        assert_eq!(palette_end_token(8, 2, 4096), Token::Literal);
    }

    #[test]
    fn lazy_evaluation_defers_once_for_general_and_repeatedly_for_palette() {
        let decoded = b"abcXbcdefYabcdefghiUhijklVghijkl";
        for (ring, palette) in [(WIDE.general, false), (WIDE.palette, true)] {
            assert_eq!(token_at(decoded, ring, palette, 10), Token::Literal);
            assert_eq!(
                token_at(decoded, ring, palette, 26) == Token::Literal,
                palette
            );
            let covered: usize = lzss(decoded, ring, palette)
                .iter()
                .map(|token| token.length())
                .sum();
            assert_eq!(covered, decoded.len());
        }
    }

    #[test]
    fn tagged_streams_decode_to_their_input_and_take_the_smaller_encoding() {
        let mut decoded = b"abcXbcdefYabcdefghiUhijklVghijkl".repeat(9);
        decoded.extend(0..=255u8);
        let general = compress_general(&decoded, &WIDE).unwrap();
        let palette = compress_tagged_palette(&decoded, &WIDE).unwrap();
        assert_eq!(palette[0], 1);
        let maximum = decoded.len() as u64;
        // A general stream ends in zero bits, which the packer's alignment
        // bytes after it supply.
        let mut padded = general.clone();
        padded.extend([0; 4]);
        let (unpacked, _) = decode_general(&padded, 0, padded.len(), maximum).unwrap();
        assert_eq!(unpacked, decoded);
        let (unpacked, _) = decode_palette(&palette, 1, palette.len(), maximum).unwrap();
        assert_eq!(unpacked, decoded);
        let chosen = compress_tagged(&decoded, &WIDE).unwrap();
        assert_eq!(chosen.len(), general.len().min(palette.len()));
        assert!(chosen == palette || (chosen == general && general.len() < palette.len()));
    }

    #[test]
    fn tile_compressor_derives_copies_and_literal_widths_from_pixels() {
        let literal = |width| Mtf4LzToken::Literal { width };
        let decoded = [0x21, 0x12, 0x21, 0x12, 0x21, 0x12, 0xfe];
        assert_eq!(
            mtf4_tokens(&decoded),
            [
                literal(2),
                literal(2),
                Mtf4LzToken::Copy {
                    length: 4,
                    distance: 2
                },
                literal(4)
            ]
        );
        let decoded = b"abcXbcdefYabcdefghiUhijklVghijkl";
        let mut position = 0;
        let mut checked = false;
        for token in mtf4_tokens(decoded) {
            if position == 10 {
                assert_eq!(
                    token,
                    Mtf4LzToken::Copy {
                        length: 3,
                        distance: 10
                    }
                );
                checked = true;
            }
            position += match token {
                Mtf4LzToken::Literal { .. } => 1,
                Mtf4LzToken::Copy { length, .. } => length as usize,
            };
        }
        assert_eq!(position, decoded.len());
        assert!(checked);
        let mut stream = compress_mtf4(decoded).unwrap();
        stream.extend([0, 0]);
        let (unpacked, _) = decode_mtf4_lz(&stream, 0, stream.len(), 64).unwrap();
        assert_eq!(unpacked, decoded);
    }

    #[test]
    fn arena_sequences_read_earlier_streams_and_align_each_one() {
        // Two zero-skip frames: a raw split and four literal bytes each.
        let frame = |value: u8| vec![value, 0xff, 0xfe, 0];
        let built = compress_arena_sequence(&[frame(3), frame(5)], 8).unwrap();
        assert_eq!(
            built,
            [0, 0, 3, 0xff, 0xfe, 0, 0, 0, 0, 0, 5, 0xff, 0xfe, 0, 0, 0]
        );
        // An empty frame is a bare split after the stream before it.
        let built = compress_arena_sequence(&[frame(3), Vec::new()], 1).unwrap();
        assert_eq!(built, [0, 0, 3, 0xff, 0xfe, 0, 0, 0]);
        let (decoded, _, _) = decode_arena(&built, 0).unwrap();
        assert_eq!(decoded, frame(3));
        assert!(compress_arena_sequence(&[frame(3)], 0).is_err());
    }
}
