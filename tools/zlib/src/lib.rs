//! Deterministic DEFLATE/zlib codec, ported byte-for-byte from
//! `tools/lib/zlib.ts`.
//!
//! Cross-platform note (carried over from the TypeScript): every
//! DEFLATE-compressed byte this repo tracks (PNG IDAT chunks) must be
//! reproducible from any supported host: darwin/linux on arm64/x86_64. That
//! rule out-ranks compression ratio.
//!
//! The compressor is implemented here with no host library in the loop: one
//! fixed-Huffman DEFLATE block, greedy hash-chain LZ77 with fully specified
//! tie-breaking, and an adler32 wrapper. Same input, same bytes, on every
//! host, by construction. It compresses a little worse than zlib level 9;
//! that is the price of determinism, paid knowingly.
//!
//! PORT NOTE: the TypeScript delegates `inflateSync` and `crc32` to
//! `node:zlib`. Rust has no such standard library and this crate is required
//! to carry zero third-party dependencies, so both are implemented here.
//! That is safe for parity precisely for the reason the TypeScript header
//! gives: inflate output and CRC values are fully specified by RFC 1950/1951,
//! so any correct implementation agrees byte-for-byte. The inflater below
//! reproduces node's observable behaviour: it validates the zlib header, the
//! Huffman tables, back-reference distances, the stored-block length
//! complement, and the trailing adler32 checksum, and it fails on truncated
//! input rather than returning a partial result. Trailing bytes after the
//! adler32 are ignored, as node does. Error *messages* are this crate's own;
//! only the success/failure decision and the produced bytes are contractual.

use std::fmt;

/// Compression level, accepted for call-site compatibility with the
/// TypeScript. The deterministic encoder has a single mode, so the level
/// does not change the output.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Default)]
pub struct DeflateOptions {
    pub level: Option<u8>,
}

// ---------------------------------------------------------------------------
// Checksums
// ---------------------------------------------------------------------------

const fn crc32_table() -> [u32; 256] {
    let mut table = [0u32; 256];
    let mut n = 0;
    while n < 256 {
        let mut c = n as u32;
        let mut k = 0;
        while k < 8 {
            c = if c & 1 != 0 {
                0xedb8_8320 ^ (c >> 1)
            } else {
                c >> 1
            };
            k += 1;
        }
        table[n] = c;
        n += 1;
    }
    table
}

static CRC32_TABLE: [u32; 256] = crc32_table();

/// CRC-32 (IEEE, as specified by RFC 1952 and implemented by `zlib.crc32`).
pub fn crc32(data: &[u8]) -> u32 {
    let mut crc = 0xffff_ffffu32;
    for &byte in data {
        crc = CRC32_TABLE[((crc ^ byte as u32) & 0xff) as usize] ^ (crc >> 8);
    }
    crc ^ 0xffff_ffff
}

fn adler32(data: &[u8]) -> u32 {
    let mut s1: u32 = 1;
    let mut s2: u32 = 0;
    // 5552 is zlib's NMAX: the largest run whose sums stay inside a u32
    // between modulo reductions.
    for chunk in data.chunks(5552) {
        for &byte in chunk {
            s1 += byte as u32;
            s2 += s1;
        }
        s1 %= 65521;
        s2 %= 65521;
    }
    (s2 << 16) | s1
}

// ---------------------------------------------------------------------------
// Bit writer
// ---------------------------------------------------------------------------

/// LSB-first bit writer (RFC 1951 packing: data element bits fill each byte
/// starting at its least significant bit).
struct BitWriter {
    bytes: Vec<u8>,
    bit_buffer: u32,
    bit_count: u32,
}

impl BitWriter {
    fn new() -> Self {
        BitWriter {
            bytes: Vec::new(),
            bit_buffer: 0,
            bit_count: 0,
        }
    }

    /// Append `count` bits whose LSB is the first bit of the element.
    fn write_bits(&mut self, value: u32, count: u32) {
        self.bit_buffer |= value << self.bit_count;
        self.bit_count += count;
        while self.bit_count >= 8 {
            self.bytes.push((self.bit_buffer & 0xff) as u8);
            self.bit_buffer >>= 8;
            self.bit_count -= 8;
        }
    }

    /// Huffman codes are emitted most significant code bit first.
    fn write_code(&mut self, code: u32, length: u32) {
        let mut reversed = 0u32;
        for bit in 0..length {
            reversed = (reversed << 1) | ((code >> bit) & 1);
        }
        self.write_bits(reversed, length);
    }

    fn finish(mut self) -> Vec<u8> {
        if self.bit_count > 0 {
            self.bytes.push((self.bit_buffer & 0xff) as u8);
        }
        self.bytes
    }
}

fn fixed_literal_code(symbol: u32) -> (u32, u32) {
    if symbol <= 143 {
        (0x30 + symbol, 8)
    } else if symbol <= 255 {
        (0x190 + (symbol - 144), 9)
    } else if symbol <= 279 {
        (symbol - 256, 7)
    } else {
        (0xc0 + (symbol - 280), 8)
    }
}

// RFC 1951 section 3.2.5 length and distance code tables.
const LENGTH_BASE: [u32; 29] = [
    3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131,
    163, 195, 227, 258,
];
const LENGTH_EXTRA: [u32; 29] = [
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0,
];
const DISTANCE_BASE: [u32; 30] = [
    1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537,
    2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577,
];
const DISTANCE_EXTRA: [u32; 30] = [
    0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13,
    13,
];

const WINDOW_SIZE: usize = 32768;
const MIN_MATCH: usize = 3;
const MAX_MATCH: usize = 258;
// Fixed search budget per position. Any fixed budget is deterministic; this
// one keeps worst-case time near-linear while finding almost every long match.
const MAX_CHAIN_STEPS: usize = 4096;
const HASH_BITS: u32 = 16;

fn emit_match(writer: &mut BitWriter, length: u32, distance: u32) {
    let mut length_symbol = LENGTH_BASE.len() - 1;
    while LENGTH_BASE[length_symbol] > length {
        length_symbol -= 1;
    }
    let (code, bits) = fixed_literal_code(257 + length_symbol as u32);
    writer.write_code(code, bits);
    writer.write_bits(
        length - LENGTH_BASE[length_symbol],
        LENGTH_EXTRA[length_symbol],
    );

    let mut distance_symbol = DISTANCE_BASE.len() - 1;
    while DISTANCE_BASE[distance_symbol] > distance {
        distance_symbol -= 1;
    }
    writer.write_code(distance_symbol as u32, 5);
    writer.write_bits(
        distance - DISTANCE_BASE[distance_symbol],
        DISTANCE_EXTRA[distance_symbol],
    );
}

/// Compress `data` into a zlib (RFC 1950) stream. Deterministic: the same
/// input always yields the same bytes, on every host.
pub fn deflate_sync(data: &[u8], _options: DeflateOptions) -> Vec<u8> {
    let mut writer = BitWriter::new();
    // zlib wrapper: CMF 0x78 (32K window, method 8), FLG 0x01 (FLEVEL 0,
    // no dictionary, check bits making CMF<<8|FLG divisible by 31).
    writer.write_bits(0x78, 8);
    writer.write_bits(0x01, 8);
    // One final fixed-Huffman block (BFINAL=1, BTYPE=01). A single block is
    // valid at any length and removes block-splitting policy from the output.
    writer.write_bits(1, 1);
    writer.write_bits(1, 2);

    let mut head = vec![-1i32; 1 << HASH_BITS];
    // PORT NOTE: the TypeScript allocates `previous` as a zero-filled
    // Int32Array, not -1-filled. Every position reachable through a chain has
    // its `previous` entry written before it is ever linked into `head`, so
    // the initial value is never observed; the zero fill is kept anyway so the
    // two implementations are structurally identical.
    let mut previous = vec![0i32; data.len()];
    let hash_at = |index: usize| -> usize {
        (((data[index] as usize) << 10)
            ^ ((data[index + 1] as usize) << 5)
            ^ (data[index + 2] as usize))
            & ((1usize << HASH_BITS) - 1)
    };

    let mut index = 0usize;
    while index < data.len() {
        let mut best_length = 0usize;
        let mut best_distance = 0usize;
        if index + MIN_MATCH <= data.len() {
            let hash = hash_at(index);
            let mut candidate = head[hash];
            let mut steps = 0usize;
            let limit = MAX_MATCH.min(data.len() - index);
            // Chains are most-recent-first, so the first candidate reaching a
            // given length has the smallest distance; requiring strictly
            // greater lengths afterwards makes the (length, distance) choice
            // fully specified.
            while candidate >= 0
                && index - candidate as usize <= WINDOW_SIZE
                && steps < MAX_CHAIN_STEPS
            {
                let candidate_index = candidate as usize;
                let mut matched = 0usize;
                while matched < limit && data[candidate_index + matched] == data[index + matched] {
                    matched += 1;
                }
                if matched > best_length {
                    best_length = matched;
                    best_distance = index - candidate_index;
                    if matched == limit {
                        break;
                    }
                }
                candidate = previous[candidate_index];
                steps += 1;
            }
        }
        if best_length >= MIN_MATCH {
            emit_match(&mut writer, best_length as u32, best_distance as u32);
            let end = (index + best_length).min(data.len() + 1 - MIN_MATCH);
            let mut position = index;
            while position < end {
                let hash = hash_at(position);
                previous[position] = head[hash];
                head[hash] = position as i32;
                position += 1;
            }
            index += best_length;
        } else {
            let (code, bits) = fixed_literal_code(data[index] as u32);
            writer.write_code(code, bits);
            if index + MIN_MATCH <= data.len() {
                let hash = hash_at(index);
                previous[index] = head[hash];
                head[hash] = index as i32;
            }
            index += 1;
        }
    }
    let (code, bits) = fixed_literal_code(256);
    writer.write_code(code, bits);

    let mut output = writer.finish();
    output.extend_from_slice(&adler32(data).to_be_bytes());
    output
}

// ---------------------------------------------------------------------------
// Inflate
// ---------------------------------------------------------------------------

/// Failure decoding a zlib stream.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct InflateError(pub &'static str);

impl fmt::Display for InflateError {
    fn fmt(&self, formatter: &mut fmt::Formatter<'_>) -> fmt::Result {
        formatter.write_str(self.0)
    }
}

impl std::error::Error for InflateError {}

struct BitReader<'a> {
    data: &'a [u8],
    byte_position: usize,
    bit_buffer: u32,
    bit_count: u32,
}

impl<'a> BitReader<'a> {
    fn new(data: &'a [u8]) -> Self {
        BitReader {
            data,
            byte_position: 0,
            bit_buffer: 0,
            bit_count: 0,
        }
    }

    fn need(&mut self, count: u32) -> Result<(), InflateError> {
        while self.bit_count < count {
            let byte = *self
                .data
                .get(self.byte_position)
                .ok_or(InflateError("unexpected end of file"))?;
            self.byte_position += 1;
            self.bit_buffer |= (byte as u32) << self.bit_count;
            self.bit_count += 8;
        }
        Ok(())
    }

    fn bits(&mut self, count: u32) -> Result<u32, InflateError> {
        if count == 0 {
            return Ok(0);
        }
        self.need(count)?;
        let value = self.bit_buffer & ((1u32 << count) - 1);
        self.bit_buffer >>= count;
        self.bit_count -= count;
        Ok(value)
    }

    fn align_to_byte(&mut self) {
        let drop = self.bit_count % 8;
        self.bit_buffer >>= drop;
        self.bit_count -= drop;
    }

    /// Byte offset of the next unconsumed byte, counting whole buffered bytes
    /// as not yet consumed. Only valid right after `align_to_byte`.
    fn byte_cursor(&self) -> usize {
        self.byte_position - (self.bit_count / 8) as usize
    }
}

/// Canonical Huffman decoding table in the "puff" shape: a count of codes per
/// bit length plus the symbols ordered by (length, symbol).
struct Huffman {
    counts: Vec<u16>,
    symbols: Vec<u16>,
}

impl Huffman {
    fn new(lengths: &[u8]) -> Result<Huffman, InflateError> {
        let max_bits = 15usize;
        let mut counts = vec![0u16; max_bits + 1];
        for &length in lengths {
            counts[length as usize] += 1;
        }
        if counts[0] as usize == lengths.len() {
            // No codes at all: legal only for an unused distance table, which
            // the caller handles by never asking this table to decode.
            return Ok(Huffman {
                counts,
                symbols: Vec::new(),
            });
        }
        // Check for an over- or under-subscribed set (incomplete sets with a
        // single code are legal, as zlib allows).
        let mut left = 1i32;
        for &count in counts.iter().take(max_bits + 1).skip(1) {
            left <<= 1;
            left -= count as i32;
            if left < 0 {
                return Err(InflateError("invalid code lengths set"));
            }
        }
        let mut offsets = vec![0u16; max_bits + 2];
        for length in 1..=max_bits {
            offsets[length + 1] = offsets[length] + counts[length];
        }
        let mut symbols = vec![0u16; lengths.len()];
        for (symbol, &length) in lengths.iter().enumerate() {
            if length != 0 {
                symbols[offsets[length as usize] as usize] = symbol as u16;
                offsets[length as usize] += 1;
            }
        }
        Ok(Huffman { counts, symbols })
    }

    fn decode(&self, reader: &mut BitReader<'_>) -> Result<u16, InflateError> {
        let mut code = 0i32;
        let mut first = 0i32;
        let mut index = 0i32;
        for length in 1..=15usize {
            code |= reader.bits(1)? as i32;
            let count = self.counts[length] as i32;
            if code - first < count {
                return Ok(self.symbols[(index + (code - first)) as usize]);
            }
            index += count;
            first = (first + count) << 1;
            code <<= 1;
        }
        Err(InflateError("invalid code"))
    }
}

fn fixed_tables() -> (Huffman, Huffman) {
    let mut literal_lengths = [0u8; 288];
    for (symbol, slot) in literal_lengths.iter_mut().enumerate() {
        *slot = match symbol {
            0..=143 => 8,
            144..=255 => 9,
            256..=279 => 7,
            _ => 8,
        };
    }
    let distance_lengths = [5u8; 30];
    (
        Huffman::new(&literal_lengths).expect("fixed literal table is valid"),
        Huffman::new(&distance_lengths).expect("fixed distance table is valid"),
    )
}

const CODE_LENGTH_ORDER: [usize; 19] = [
    16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15,
];

fn inflate_block_data(
    reader: &mut BitReader<'_>,
    output: &mut Vec<u8>,
    literals: &Huffman,
    distances: &Huffman,
) -> Result<(), InflateError> {
    loop {
        let symbol = literals.decode(reader)?;
        match symbol {
            0..=255 => output.push(symbol as u8),
            256 => return Ok(()),
            257..=285 => {
                let index = (symbol - 257) as usize;
                let length = LENGTH_BASE[index] + reader.bits(LENGTH_EXTRA[index])?;
                let distance_symbol = distances.decode(reader)? as usize;
                if distance_symbol >= DISTANCE_BASE.len() {
                    return Err(InflateError("invalid distance code"));
                }
                let distance = (DISTANCE_BASE[distance_symbol]
                    + reader.bits(DISTANCE_EXTRA[distance_symbol])?)
                    as usize;
                if distance > output.len() {
                    return Err(InflateError("invalid distance too far back"));
                }
                let start = output.len() - distance;
                for offset in 0..length as usize {
                    let byte = output[start + offset];
                    output.push(byte);
                }
            }
            _ => return Err(InflateError("invalid literal/length code")),
        }
    }
}

fn inflate_dynamic_tables(reader: &mut BitReader<'_>) -> Result<(Huffman, Huffman), InflateError> {
    let literal_count = reader.bits(5)? as usize + 257;
    let distance_count = reader.bits(5)? as usize + 1;
    let code_length_count = reader.bits(4)? as usize + 4;
    if literal_count > 286 || distance_count > 30 {
        return Err(InflateError("too many length or distance symbols"));
    }
    let mut code_lengths = [0u8; 19];
    for slot in CODE_LENGTH_ORDER.iter().take(code_length_count) {
        code_lengths[*slot] = reader.bits(3)? as u8;
    }
    let code_length_table = Huffman::new(&code_lengths)?;

    let mut lengths = vec![0u8; literal_count + distance_count];
    let mut index = 0usize;
    while index < lengths.len() {
        let symbol = code_length_table.decode(reader)?;
        match symbol {
            0..=15 => {
                lengths[index] = symbol as u8;
                index += 1;
            }
            16 => {
                if index == 0 {
                    return Err(InflateError("invalid bit length repeat"));
                }
                let previous = lengths[index - 1];
                let repeat = 3 + reader.bits(2)? as usize;
                if index + repeat > lengths.len() {
                    return Err(InflateError("invalid bit length repeat"));
                }
                for _ in 0..repeat {
                    lengths[index] = previous;
                    index += 1;
                }
            }
            17 | 18 => {
                let repeat = if symbol == 17 {
                    3 + reader.bits(3)? as usize
                } else {
                    11 + reader.bits(7)? as usize
                };
                if index + repeat > lengths.len() {
                    return Err(InflateError("invalid bit length repeat"));
                }
                index += repeat;
            }
            _ => return Err(InflateError("invalid code length code")),
        }
    }
    if lengths[256] == 0 {
        return Err(InflateError("invalid code -- missing end-of-block"));
    }
    let literals = Huffman::new(&lengths[..literal_count])?;
    let distances = Huffman::new(&lengths[literal_count..])?;
    Ok((literals, distances))
}

/// Decode a raw DEFLATE (RFC 1951) stream. Returns the decoded bytes and the
/// number of input bytes consumed.
pub fn inflate_raw(data: &[u8]) -> Result<(Vec<u8>, usize), InflateError> {
    let mut reader = BitReader::new(data);
    let mut output = Vec::new();
    loop {
        let final_block = reader.bits(1)?;
        let block_type = reader.bits(2)?;
        match block_type {
            0 => {
                reader.align_to_byte();
                let cursor = reader.byte_cursor();
                if cursor + 4 > data.len() {
                    return Err(InflateError("unexpected end of file"));
                }
                let length = u16::from_le_bytes([data[cursor], data[cursor + 1]]) as usize;
                let complement = u16::from_le_bytes([data[cursor + 2], data[cursor + 3]]) as usize;
                if length != (!complement & 0xffff) {
                    return Err(InflateError("invalid stored block lengths"));
                }
                let start = cursor + 4;
                if start + length > data.len() {
                    return Err(InflateError("unexpected end of file"));
                }
                output.extend_from_slice(&data[start..start + length]);
                reader = BitReader::new(data);
                reader.byte_position = start + length;
            }
            1 => {
                let (literals, distances) = fixed_tables();
                inflate_block_data(&mut reader, &mut output, &literals, &distances)?;
            }
            2 => {
                let (literals, distances) = inflate_dynamic_tables(&mut reader)?;
                inflate_block_data(&mut reader, &mut output, &literals, &distances)?;
            }
            _ => return Err(InflateError("invalid block type")),
        }
        if final_block == 1 {
            break;
        }
    }
    reader.align_to_byte();
    Ok((output, reader.byte_cursor()))
}

/// Decode a zlib (RFC 1950) stream, validating the header and the trailing
/// adler32 checksum.
pub fn inflate_sync(data: &[u8]) -> Result<Vec<u8>, InflateError> {
    if data.len() < 2 {
        return Err(InflateError("unexpected end of file"));
    }
    let cmf = data[0];
    let flg = data[1];
    if cmf & 0x0f != 8 || (cmf >> 4) > 7 || !(((cmf as u16) << 8) | flg as u16).is_multiple_of(31) {
        return Err(InflateError("incorrect header check"));
    }
    if flg & 0x20 != 0 {
        return Err(InflateError("need dictionary"));
    }
    let (output, consumed) = inflate_raw(&data[2..])?;
    let checksum_start = 2 + consumed;
    if checksum_start + 4 > data.len() {
        return Err(InflateError("unexpected end of file"));
    }
    let stored = u32::from_be_bytes([
        data[checksum_start],
        data[checksum_start + 1],
        data[checksum_start + 2],
        data[checksum_start + 3],
    ]);
    if stored != adler32(&output) {
        return Err(InflateError("incorrect data check"));
    }
    Ok(output)
}

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------

#[cfg(test)]
mod tests {
    use super::*;

    /// Deterministic PRNG so generated corpora are reproducible.
    struct Rng(u64);

    impl Rng {
        fn next_u32(&mut self) -> u32 {
            // xorshift64*
            let mut x = self.0;
            x ^= x >> 12;
            x ^= x << 25;
            x ^= x >> 27;
            self.0 = x;
            ((x.wrapping_mul(0x2545_f491_4f6c_dd1d)) >> 32) as u32
        }
    }

    fn round_trip(data: &[u8]) {
        let compressed = deflate_sync(data, DeflateOptions::default());
        let restored = inflate_sync(&compressed)
            .unwrap_or_else(|error| panic!("inflate failed on {} bytes: {error}", data.len()));
        assert_eq!(
            restored.as_slice(),
            data,
            "round-trip mismatch at {} bytes",
            data.len()
        );
        let again = deflate_sync(data, DeflateOptions::default());
        assert_eq!(
            again,
            compressed,
            "deflate is not deterministic at {} bytes",
            data.len()
        );
    }

    /// The cases from the TypeScript `selfTest`.
    #[test]
    fn typescript_self_test_cases() {
        let cases: Vec<Vec<u8>> = vec![
            Vec::new(),
            vec![0],
            vec![255],
            (0..256).map(|value| value as u8).collect(),
            vec![0x41; 70000],
            (0..200_000u64)
                .map(|value| ((value.wrapping_mul(2_654_435_761) & 0xffff_ffff) >> 24) as u8)
                .collect(),
            (0..5000u32).map(|value| (value % 7) as u8).collect(),
        ];
        assert_eq!(cases.len(), 7);
        for case in &cases {
            round_trip(case);
        }
    }

    #[test]
    fn empty_input_is_a_bare_wrapper() {
        let compressed = deflate_sync(&[], DeflateOptions::default());
        // 78 01, then BFINAL/BTYPE + the 7-bit end-of-block code, then adler.
        assert_eq!(
            compressed,
            vec![0x78, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01]
        );
        assert_eq!(inflate_sync(&compressed).unwrap(), Vec::<u8>::new());
    }

    #[test]
    fn single_byte() {
        for byte in 0..=255u8 {
            round_trip(&[byte]);
        }
    }

    #[test]
    fn all_identical_bytes() {
        for length in [1usize, 2, 3, 4, 258, 259, 261, 1000, 65536, 100_000] {
            round_trip(&vec![0xab; length]);
        }
    }

    #[test]
    fn incompressible_random_bytes() {
        let mut rng = Rng(0x1234_5678_9abc_def0);
        let data: Vec<u8> = (0..200_000).map(|_| rng.next_u32() as u8).collect();
        round_trip(&data);
        // Random data must not shrink below its own size by any real margin.
        let compressed = deflate_sync(&data, DeflateOptions::default());
        assert!(
            compressed.len() > data.len(),
            "random data should not compress"
        );
    }

    #[test]
    fn compressible_data_actually_shrinks() {
        let data: Vec<u8> = (0..100_000u32).map(|value| (value % 11) as u8).collect();
        let compressed = deflate_sync(&data, DeflateOptions::default());
        assert!(
            compressed.len() * 20 < data.len(),
            "expected a big win, got {}",
            compressed.len()
        );
        assert_eq!(inflate_sync(&compressed).unwrap(), data);
    }

    #[test]
    fn maximum_match_and_distance_edges() {
        // A 258-byte run repeated at exactly the far edge of the window.
        let mut data = vec![0u8; WINDOW_SIZE + 600];
        let mut rng = Rng(99);
        for slot in data.iter_mut().take(600) {
            *slot = rng.next_u32() as u8;
        }
        let head: Vec<u8> = data[..600].to_vec();
        data[WINDOW_SIZE..WINDOW_SIZE + 600].copy_from_slice(&head);
        round_trip(&data);
    }

    #[test]
    fn round_trip_property_over_generated_inputs() {
        let mut rng = Rng(0xdead_beef_cafe_babe);
        let mut cases = 0;
        for trial in 0..400 {
            let length = (rng.next_u32() % 6000) as usize;
            let alphabet = 1 + (trial % 16) as u32;
            let data: Vec<u8> = (0..length)
                .map(|_| (rng.next_u32() % alphabet) as u8)
                .collect();
            round_trip(&data);
            cases += 1;
        }
        assert_eq!(cases, 400);
    }

    #[test]
    fn crc32_known_vectors() {
        assert_eq!(crc32(b""), 0);
        assert_eq!(crc32(b"a"), 0xe8b7_be43);
        assert_eq!(crc32(b"123456789"), 0xcbf4_3926);
        assert_eq!(
            crc32(b"The quick brown fox jumps over the lazy dog"),
            0x414f_a339
        );
    }

    #[test]
    fn adler32_known_vectors() {
        assert_eq!(adler32(b""), 1);
        assert_eq!(adler32(b"a"), 0x0062_0062);
        assert_eq!(adler32(b"Wikipedia"), 0x11e6_0398);
        // Crosses the NMAX reduction boundary.
        assert_eq!(adler32(&vec![0xffu8; 20000]), 0x9f51_d664);
    }

    #[test]
    fn rejects_bad_header() {
        assert!(inflate_sync(&[]).is_err());
        assert!(inflate_sync(&[0x78]).is_err());
        assert!(inflate_sync(&[0x00, 0x00, 0x00]).is_err());
        // Valid method but a bad FCHECK.
        assert!(inflate_sync(&[0x78, 0x02, 0x03, 0x00]).is_err());
        // Preset dictionary flag set (0x78 0x20 has FCHECK 0 but is % 31 != 0,
        // so use 0x78 0xbb which is divisible by 31 and has FDICT set).
        assert_eq!(((0x78u16 << 8) | 0xbb) % 31, 0);
        assert_eq!(
            inflate_sync(&[0x78, 0xbb, 0x03, 0x00]),
            Err(InflateError("need dictionary"))
        );
    }

    #[test]
    fn rejects_truncated_streams() {
        let data: Vec<u8> = (0..20_000u32).map(|value| (value % 251) as u8).collect();
        let compressed = deflate_sync(&data, DeflateOptions::default());
        for cut in 2..compressed.len() {
            assert!(
                inflate_sync(&compressed[..cut]).is_err(),
                "truncation to {cut} bytes should not decode"
            );
        }
    }

    #[test]
    fn rejects_corrupt_checksum() {
        let data = b"hello hello hello hello".to_vec();
        let mut compressed = deflate_sync(&data, DeflateOptions::default());
        let last = compressed.len() - 1;
        compressed[last] ^= 0xff;
        assert_eq!(
            inflate_sync(&compressed),
            Err(InflateError("incorrect data check"))
        );
    }

    #[test]
    fn rejects_reserved_block_type() {
        // 78 01 then BFINAL=1, BTYPE=11 -> 0b111 = 0x07.
        assert_eq!(
            inflate_sync(&[0x78, 0x01, 0x07, 0x00, 0x00, 0x00, 0x00, 0x01]),
            Err(InflateError("invalid block type"))
        );
    }

    #[test]
    fn corrupt_bodies_never_panic() {
        let data: Vec<u8> = (0..3000u32).map(|value| (value % 13) as u8).collect();
        let compressed = deflate_sync(&data, DeflateOptions::default());
        let mut rng = Rng(0x5151_5151_5151_5151);
        for _ in 0..3000 {
            let mut corrupt = compressed.clone();
            let position = 2 + (rng.next_u32() as usize % (corrupt.len() - 2));
            corrupt[position] ^= 1 << (rng.next_u32() % 8);
            // Either it decodes (a bit flip can land on a still-valid stream
            // only if the checksum still matches) or it errors. Never panics.
            if let Ok(restored) = inflate_sync(&corrupt) {
                assert_eq!(restored, data);
            }
        }
    }

    /// Decodes streams this crate did not produce: stored blocks and dynamic
    /// Huffman blocks, exercising the inflate paths deflate_sync never emits.
    #[test]
    fn inflates_stored_blocks() {
        let payload = b"stored block payload";
        let mut stream = vec![0x78u8, 0x01, 0x01];
        stream.extend_from_slice(&(payload.len() as u16).to_le_bytes());
        stream.extend_from_slice(&(!(payload.len() as u16)).to_le_bytes());
        stream.extend_from_slice(payload);
        stream.extend_from_slice(&adler32(payload).to_be_bytes());
        assert_eq!(inflate_sync(&stream).unwrap(), payload);

        // A bad length complement must be rejected.
        let mut bad = stream.clone();
        bad[5] ^= 0xff;
        assert_eq!(
            inflate_sync(&bad),
            Err(InflateError("invalid stored block lengths"))
        );
    }

    #[test]
    fn inflates_multi_block_streams() {
        // Two stored blocks, only the second final.
        let first = b"aaaa";
        let second = b"bbbb";
        let mut stream = vec![0x78u8, 0x01, 0x00];
        stream.extend_from_slice(&(first.len() as u16).to_le_bytes());
        stream.extend_from_slice(&(!(first.len() as u16)).to_le_bytes());
        stream.extend_from_slice(first);
        stream.push(0x01);
        stream.extend_from_slice(&(second.len() as u16).to_le_bytes());
        stream.extend_from_slice(&(!(second.len() as u16)).to_le_bytes());
        stream.extend_from_slice(second);
        let mut both = first.to_vec();
        both.extend_from_slice(second);
        stream.extend_from_slice(&adler32(&both).to_be_bytes());
        assert_eq!(inflate_sync(&stream).unwrap(), both);
    }

    #[test]
    fn inflates_streams_from_zlib_itself() {
        // Captured from node:zlib deflateSync(level 9). Kept as literals so
        // the test needs no compression library. Proves the inflater decodes
        // encoders other than this crate's.
        let fixed: &[u8] = &[
            120, 218, 75, 76, 42, 74, 76, 78, 76, 73, 4, 82, 10, 137, 216, 217, 0, 238, 40, 13, 61,
        ];
        assert_eq!(
            inflate_sync(fixed).unwrap(),
            b"abracadabra abracadabra abracadabra".to_vec()
        );

        // A dynamic-Huffman block (BTYPE=10), a path deflate_sync never emits.
        assert_eq!(fixed[2] >> 1 & 3, 1, "expected a fixed-Huffman block");
        let dynamic: &[u8] = &[
            120, 218, 237, 210, 169, 17, 0, 32, 20, 197, 192, 90, 129, 127, 8, 238, 254, 13, 85,
            60, 195, 68, 69, 198, 108, 175, 205, 60, 178, 200, 114, 139, 248, 112, 82, 124, 216,
            33, 62, 44, 23, 31, 166, 137, 15, 67, 140, 40, 59, 76, 97, 10, 83, 152, 194, 20, 166,
            48, 133, 41, 76, 97, 10, 83, 152, 254, 206, 244, 1, 150, 203, 28, 121,
        ];
        assert_eq!(dynamic[2] >> 1 & 3, 2, "expected a dynamic-Huffman block");
        let expected: Vec<u8> = (0..4000usize)
            .map(|i| [97u8, 98, 99, 100, 101, 102, 103][i % 7] + if i % 97 == 0 { 10 } else { 0 })
            .collect();
        assert_eq!(inflate_sync(dynamic).unwrap(), expected);
        assert_eq!(crc32(&expected), 754_471_328);
    }

    #[test]
    fn trailing_bytes_after_the_checksum_are_ignored() {
        let mut compressed = deflate_sync(b"trailing", DeflateOptions::default());
        compressed.extend_from_slice(&[0xde, 0xad, 0xbe, 0xef]);
        assert_eq!(inflate_sync(&compressed).unwrap(), b"trailing".to_vec());
    }
}
