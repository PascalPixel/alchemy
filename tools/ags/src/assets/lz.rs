//! Explicit LZ stream codecs, independent of ROM layouts and resource tables.
use super::{err, AssetError};
/// `["l", n]` / `["c", length, distance]` from the TypeScript.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum GeneralToken {
    Literal(u32),
    Copy { length: u32, distance: u32 },
}
/// `["l"]` / `["e"]` / `["c", length, distance]` from the TypeScript.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum PaletteOperation {
    Literal,
    End,
    Copy { length: u32, distance: u32 },
}
/// `["z"]` / `["g", ops]` from the TypeScript.
#[derive(Debug, Clone, PartialEq, Eq)]
pub enum PaletteGroup {
    Zeros,
    Group(Vec<PaletteOperation>),
}
// ---------------------------------------------------------------------------
// bit reader
// ---------------------------------------------------------------------------
/// LSB-first bit reader over 16-bit little-endian words.
///
/// PORT NOTE: the TypeScript accumulator is a JS number normalised with
/// `>>> 0`, i.e. exactly a `u32`. `count` never exceeds 27 here (the widest
/// `get` is 12 bits and `fill` only runs when `count < requested`), so the
/// `value | word << count` shift can never reach 32 and the two
/// implementations drop the same high bits.
struct LsbBits<'a> {
    data: &'a [u8],
    origin: usize,
    cursor: usize,
    end: usize,
    value: u32,
    count: u32,
}
impl<'a> LsbBits<'a> {
    fn new(data: &'a [u8], cursor: usize, end: usize) -> Result<Self, AssetError> {
        let mut bits = LsbBits {
            data,
            origin: cursor,
            cursor,
            end,
            value: 0,
            count: 0,
        };
        if cursor & 1 != 0 {
            bits.value = u32::from(bits.byte()?);
            bits.count = 8;
        }
        bits.fill()?;
        Ok(bits)
    }
    fn need(&self, size: usize) -> Result<(), AssetError> {
        if self.cursor + size > self.end {
            return err("compressed input ended before terminator");
        }
        Ok(())
    }
    fn byte(&mut self) -> Result<u8, AssetError> {
        self.need(1)?;
        let value = self.data[self.cursor];
        self.cursor += 1;
        Ok(value)
    }
    fn fill(&mut self) -> Result<(), AssetError> {
        self.need(2)?;
        let word = u32::from(self.data[self.cursor]) | u32::from(self.data[self.cursor + 1]) << 8;
        self.value |= word.wrapping_shl(self.count);
        self.cursor += 2;
        self.count += 16;
        Ok(())
    }
    /// One past the last byte holding a consumed bit. The reader fetches whole
    /// words, so `cursor` can already stand one byte beyond the stream.
    fn stored_end(&self) -> usize {
        self.origin + ((self.cursor - self.origin) * 8 - self.count as usize).div_ceil(8)
    }
    fn get(&mut self, count: u32) -> Result<u32, AssetError> {
        while self.count < count {
            self.fill()?;
        }
        // `2 ** count - 1` for count in 0..=12.
        let mask = (1u32 << count) - 1;
        let value = self.value & mask;
        self.value >>= count;
        self.count -= count;
        Ok(value)
    }
}
/// `Math.floor(Math.log2(value)) + 1`, with `0 -> 0`.
fn bit_length(value: u32) -> u32 {
    if value == 0 {
        return 0;
    }
    32 - value.leading_zeros()
}
// ---------------------------------------------------------------------------
// general stream
// ---------------------------------------------------------------------------
fn append_copy(
    output: &mut Vec<u8>,
    distance: u32,
    length: u32,
    maximum: u64,
) -> Result<(), AssetError> {
    if distance < 1 || u64::from(distance) > output.len() as u64 {
        return err(format!(
            "invalid back-reference distance at output offset 0x{:x}",
            output.len()
        ));
    }
    if length < 1 || output.len() as u64 + u64::from(length) > maximum {
        return err("decoded output crossed configured bound");
    }
    for _ in 0..length {
        output.push(output[output.len() - distance as usize]);
    }
    Ok(())
}
fn decode_length(bits: &mut LsbBits) -> Result<Option<u32>, AssetError> {
    if bits.get(1)? == 0 {
        return Ok(Some(2));
    }
    if bits.get(1)? == 0 {
        return Ok(Some(3));
    }
    if bits.get(1)? == 0 {
        return Ok(Some(4));
    }
    if bits.get(1)? == 0 {
        return Ok(Some(5));
    }
    if bits.get(1)? == 0 {
        return Ok(Some(if bits.get(1)? != 0 { 7 } else { 6 }));
    }
    let short = bits.get(2)?;
    if short != 0 {
        return Ok(Some(short + 7));
    }
    let long = bits.get(7)?;
    Ok(if long == 0 { None } else { Some(long + 10) })
}
/// Shared body of `decode_general_trace` and `decode_general_prefill_trace`.
fn decode_general_body(
    data: &[u8],
    start: usize,
    end: usize,
    maximum: u64,
    prefill: usize,
    header: usize,
) -> Result<(Vec<u8>, usize, Vec<GeneralToken>, usize), AssetError> {
    let mut bits = LsbBits::new(data, start + header, end)?;
    let mut output: Vec<u8> = vec![0; prefill];
    let mut tokens: Vec<GeneralToken> = Vec::new();
    loop {
        if bits.get(1)? != 0 {
            if (output.len() - prefill) as u64 >= maximum {
                return err("decoded output crossed configured bound");
            }
            output.push(bits.get(8)? as u8);
            match tokens.last_mut() {
                Some(GeneralToken::Literal(count)) => *count += 1,
                _ => tokens.push(GeneralToken::Literal(1)),
            }
            continue;
        }
        let length = match decode_length(&mut bits)? {
            Some(length) => length,
            None => {
                return Ok((
                    output[prefill..].to_vec(),
                    bits.cursor,
                    tokens,
                    bits.stored_end(),
                ))
            }
        };
        let distance = if bits.get(1)? != 0 {
            bits.get(5)? + 1
        } else {
            let window = output.len() as i64 - prefill as i64 - 33;
            let width = if (0..2048).contains(&window) {
                bit_length(window as u32)
            } else {
                12
            };
            bits.get(width)? + 33
        };
        tokens.push(GeneralToken::Copy { length, distance });
        append_copy(&mut output, distance, length, prefill as u64 + maximum)?;
    }
}
pub(crate) fn decode_general_trace(
    data: &[u8],
    start: usize,
    end: usize,
    maximum: u64,
) -> Result<(Vec<u8>, usize, Vec<GeneralToken>), AssetError> {
    if start >= end || data[start] != 0 {
        return err("general stream is missing its kind-zero header");
    }
    decode_general_body(data, start, end, maximum, 0, 1)
        .map(|(output, cursor, tokens, _)| (output, cursor, tokens))
}
pub fn decode_general(
    data: &[u8],
    start: usize,
    end: usize,
    maximum: u64,
) -> Result<(Vec<u8>, usize), AssetError> {
    let (output, cursor, _) = decode_general_trace(data, start, end, maximum)?;
    Ok((output, cursor))
}
fn put(bits: &mut Vec<u8>, value: u32, count: u32) {
    for index in 0..count {
        bits.push(((value >> index) & 1) as u8);
    }
}
fn encode_length(bits: &mut Vec<u8>, length: u32) -> Result<(), AssetError> {
    put(bits, 0, 1);
    match length {
        2 => put(bits, 0, 1),
        3 => {
            put(bits, 1, 1);
            put(bits, 0, 1);
        }
        4 => {
            put(bits, 3, 2);
            put(bits, 0, 1);
        }
        5 => {
            put(bits, 7, 3);
            put(bits, 0, 1);
        }
        6 | 7 => {
            put(bits, 15, 4);
            put(bits, 0, 1);
            put(bits, length - 6, 1);
        }
        8..=10 => {
            put(bits, 31, 5);
            put(bits, length - 7, 2);
        }
        11..=137 => {
            put(bits, 31, 5);
            put(bits, 0, 2);
            put(bits, length - 10, 7);
        }
        _ => return err(format!("unencodable copy length: {length}")),
    }
    Ok(())
}
fn finish_bits(bits: &[u8], header: usize) -> Vec<u8> {
    let mut bits = bits.to_vec();
    put(&mut bits, 0, 1);
    put(&mut bits, 31, 5);
    put(&mut bits, 0, 2);
    put(&mut bits, 0, 7);
    let mut packed: Vec<u8> = vec![0; header];
    for chunk in bits.chunks(8) {
        let mut byte = 0u8;
        for (index, bit) in chunk.iter().enumerate() {
            byte |= bit << index;
        }
        packed.push(byte);
    }
    packed
}
fn encode_general_inner(
    decoded: &[u8],
    tokens: &[GeneralToken],
    prefill: usize,
    header: usize,
) -> Result<Vec<u8>, AssetError> {
    let mut bits: Vec<u8> = Vec::new();
    let mut replay: Vec<u8> = vec![0; prefill];
    let mut cursor: usize = 0;
    for token in tokens {
        match *token {
            GeneralToken::Literal(count) => {
                if count < 1 || cursor + count as usize > decoded.len() {
                    return err("literal run crossed decoded input");
                }
                for _ in 0..count {
                    put(&mut bits, 1, 1);
                    let value = decoded[cursor];
                    cursor += 1;
                    put(&mut bits, u32::from(value), 8);
                    replay.push(value);
                }
            }
            GeneralToken::Copy { length, distance } => {
                encode_length(&mut bits, length)?;
                if distance < 1 || distance as usize > replay.len() {
                    return err("copy distance crossed replay prefix");
                }
                if distance <= 32 {
                    put(&mut bits, 1, 1);
                    put(&mut bits, distance - 1, 5);
                } else {
                    put(&mut bits, 0, 1);
                    let window = replay.len() as i64 - prefill as i64 - 33;
                    let width = if (0..2048).contains(&window) {
                        bit_length(window as u32)
                    } else {
                        12
                    };
                    if u64::from(distance - 33) >= 1u64 << width {
                        return err("copy distance exceeds position-dependent width");
                    }
                    put(&mut bits, distance - 33, width);
                }
                for _ in 0..length {
                    replay.push(replay[replay.len() - distance as usize]);
                }
                cursor += length as usize;
            }
        }
    }
    // A failure here must name a COORDINATE, not just a verdict: the divergence
    // offset is the whole diagnosis, and it costs one loop to compute on a path
    // that is already throwing.
    let rebuilt = &replay[prefill..];
    if cursor != decoded.len() || rebuilt != decoded {
        let mut at: i64 = -1;
        let mut differing = 0usize;
        for (index, (left, right)) in rebuilt.iter().zip(decoded.iter()).enumerate() {
            if left != right {
                if at < 0 {
                    at = index as i64;
                }
                differing += 1;
            }
        }
        let first_diff = if at < 0 {
            "none".to_string()
        } else {
            format!("0x{at:x}")
        };
        return err(format!("token plan does not reconstruct decoded input (replay={} decoded={} cursor={} first_diff={} differing={})", rebuilt.len(), decoded.len(), cursor, first_diff, differing));
    }
    Ok(finish_bits(&bits, header))
}
pub fn encode_general(decoded: &[u8], tokens: &[GeneralToken]) -> Result<Vec<u8>, AssetError> {
    encode_general_inner(decoded, tokens, 0, 1)
}
// ---------------------------------------------------------------------------
// palette stream
// ---------------------------------------------------------------------------
pub(crate) fn decode_palette_trace(
    data: &[u8],
    start: usize,
    end: usize,
    maximum: u64,
) -> Result<(Vec<u8>, usize, Vec<PaletteGroup>), AssetError> {
    let mut cursor = start;
    let mut output: Vec<u8> = Vec::new();
    let mut groups: Vec<PaletteGroup> = Vec::new();
    macro_rules! take {
        ($size:expr) => {{
            let size: usize = $size;
            if cursor + size > end {
                return err("compressed input ended before terminator");
            }
            let result = &data[cursor..cursor + size];
            cursor += size;
            result
        }};
    }
    loop {
        let flags = take!(1)[0];
        if flags == 0 {
            let block = take!(8).to_vec();
            if output.len() as u64 + 8 > maximum {
                return err("decoded output crossed configured bound");
            }
            output.extend_from_slice(&block);
            groups.push(PaletteGroup::Zeros);
            continue;
        }
        let mut operations: Vec<PaletteOperation> = Vec::new();
        for bit in (0..8).rev() {
            if flags & (1 << bit) == 0 {
                if output.len() as u64 >= maximum {
                    return err("decoded output crossed configured bound");
                }
                let value = take!(1)[0];
                output.push(value);
                operations.push(PaletteOperation::Literal);
                continue;
            }
            let pair = take!(2);
            let (first, second) = (pair[0], pair[1]);
            let distance = (u32::from(first & 0xf0) << 4) | u32::from(second);
            let mut length = u32::from(first & 0x0f);
            if length == 0 {
                if distance == 0 {
                    operations.push(PaletteOperation::End);
                    groups.push(PaletteGroup::Group(operations));
                    return Ok((output, cursor, groups));
                }
                length = u32::from(take!(1)[0]) + 17;
            } else {
                length += 1;
            }
            operations.push(PaletteOperation::Copy { length, distance });
            append_copy(&mut output, distance, length, maximum)?;
        }
        groups.push(PaletteGroup::Group(operations));
    }
}
pub fn decode_palette(
    data: &[u8],
    start: usize,
    end: usize,
    maximum: u64,
) -> Result<(Vec<u8>, usize), AssetError> {
    let (output, cursor, _) = decode_palette_trace(data, start, end, maximum)?;
    Ok((output, cursor))
}
pub fn encode_palette(decoded: &[u8], groups: &[PaletteGroup]) -> Result<Vec<u8>, AssetError> {
    let mut output: Vec<u8> = Vec::new();
    let mut encoded: Vec<u8> = Vec::new();
    let mut cursor: usize = 0;
    let mut ended = false;
    for group in groups {
        if ended {
            return err("palette plan contains data after terminator");
        }
        let operations = match group {
            PaletteGroup::Zeros => {
                if cursor + 8 > decoded.len() {
                    return err("palette literal block crossed decoded input");
                }
                encoded.push(0);
                encoded.extend_from_slice(&decoded[cursor..cursor + 8]);
                output.extend_from_slice(&decoded[cursor..cursor + 8]);
                cursor += 8;
                continue;
            }
            PaletteGroup::Group(operations) => operations,
        };
        if operations.is_empty() {
            return err("invalid palette token group");
        }
        if operations.len() > 8 {
            return err("palette group exceeds eight operations");
        }
        let mut flags: u8 = 0;
        let mut payload: Vec<u8> = Vec::new();
        for (index, token) in operations.iter().enumerate() {
            match *token {
                PaletteOperation::Literal => {
                    if cursor >= decoded.len() {
                        return err("palette literal crossed decoded input");
                    }
                    payload.push(decoded[cursor]);
                    output.push(decoded[cursor]);
                    cursor += 1;
                    continue;
                }
                PaletteOperation::End => {
                    flags |= 1 << (7 - index);
                    payload.push(0);
                    payload.push(0);
                    ended = true;
                    if index + 1 != operations.len() {
                        return err("palette terminator is not group-final");
                    }
                    continue;
                }
                PaletteOperation::Copy { length, distance } => {
                    flags |= 1 << (7 - index);
                    if distance < 1 || distance as usize > output.len() || distance > 0xfff {
                        return err("palette copy distance is invalid");
                    }
                    match length {
                        2..=16 => {
                            payload.push((((distance >> 8) << 4) | (length - 1)) as u8);
                            payload.push((distance & 0xff) as u8);
                        }
                        17..=272 => {
                            payload.push(((distance >> 8) << 4) as u8);
                            payload.push((distance & 0xff) as u8);
                            payload.push((length - 17) as u8);
                        }
                        _ => return err("palette copy length is invalid"),
                    }
                    for _ in 0..length {
                        output.push(output[output.len() - distance as usize]);
                    }
                    cursor += length as usize;
                }
            }
        }
        encoded.push(flags);
        encoded.extend_from_slice(&payload);
    }
    if !ended {
        return err("palette plan lacks a terminator");
    }
    if cursor != decoded.len() || output != decoded {
        return err("palette token plan does not reconstruct input");
    }
    Ok(encoded)
}
// ---------------------------------------------------------------------------
// MTF4 LZ stream (tag 2)
// ---------------------------------------------------------------------------
/// A token of the tag-2 stream: the general stream's copy coding with
/// literals written as two move-to-front nibble indices of `width` bits.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Mtf4LzToken {
    Literal { width: u32 },
    Copy { length: u32, distance: u32 },
}
fn mtf4_index(table: &mut [u8; 16], value: u8) -> u32 {
    let index = table
        .iter()
        .position(|candidate| *candidate == value)
        .unwrap_or(0);
    table[..=index].rotate_right(1);
    index as u32
}
pub fn decode_mtf4_lz(
    data: &[u8],
    start: usize,
    end: usize,
    maximum: u64,
) -> Result<(Vec<u8>, usize), AssetError> {
    decode_mtf4_body(data, start, end, maximum).map(|(output, cursor, _)| (output, cursor))
}
fn decode_mtf4_body(
    data: &[u8],
    start: usize,
    end: usize,
    maximum: u64,
) -> Result<(Vec<u8>, usize, usize), AssetError> {
    if start >= end || data.get(start) != Some(&2) {
        return err("tag-2 stream header missing");
    }
    let mut bits = LsbBits::new(data, start + 1, end)?;
    let mut table: [u8; 16] = std::array::from_fn(|i| i as u8);
    let mut output = Vec::new();
    loop {
        if bits.get(1)? != 0 {
            if output.len() as u64 >= maximum {
                return err("decoded output crossed configured bound");
            }
            let width = if bits.get(1)? != 0 {
                2
            } else if bits.get(1)? != 0 {
                3
            } else {
                4
            };
            let low = bits.get(width)? as usize;
            let a = table[low];
            table[..=low].rotate_right(1);
            let high = bits.get(width)? as usize;
            let b = table[high];
            table[..=high].rotate_right(1);
            output.push(a | b << 4);
            continue;
        }
        let Some(length) = decode_length(&mut bits)? else {
            return Ok((output, bits.cursor, bits.stored_end()));
        };
        let distance = if bits.get(1)? != 0 {
            bits.get(5)? + 1
        } else {
            let window = output.len() as i64 - 33;
            let width = if (0..2048).contains(&window) {
                bit_length(window as u32)
            } else {
                12
            };
            bits.get(width)? + 33
        };
        append_copy(&mut output, distance, length, maximum)?;
    }
}

/// Encode `decoded` as a tag-2 stream: tag byte 2, then LSB-first bits where
/// a literal is `1`, a two-bit width selector (`1` = 2 bits, `01` = 3 bits,
/// `00` = 4 bits) and the low then high nibble as MTF indices, and a copy uses
/// the general stream's length and position-dependent distance coding.
pub fn encode_mtf4_lz(decoded: &[u8], tokens: &[Mtf4LzToken]) -> Result<Vec<u8>, AssetError> {
    let mut bits: Vec<u8> = Vec::new();
    let mut table: [u8; 16] = std::array::from_fn(|index| index as u8);
    let mut cursor = 0usize;
    for token in tokens {
        match *token {
            Mtf4LzToken::Literal { width } => {
                if !(2..=4).contains(&width) || cursor >= decoded.len() {
                    return err("invalid tag-2 literal token");
                }
                put(&mut bits, 1, 1);
                if width == 2 {
                    put(&mut bits, 1, 1);
                } else {
                    put(&mut bits, 0, 1);
                    put(&mut bits, u32::from(width == 3), 1);
                }
                let value = decoded[cursor];
                let low = mtf4_index(&mut table, value & 15);
                let high = mtf4_index(&mut table, value >> 4);
                if low >= 1 << width || high >= 1 << width {
                    return err("literal does not fit its recorded MTF width");
                }
                put(&mut bits, low, width);
                put(&mut bits, high, width);
                cursor += 1;
            }
            Mtf4LzToken::Copy { length, distance } => {
                let (length, distance) = (length as usize, distance as usize);
                if distance < 1 || distance > cursor || cursor + length > decoded.len() {
                    return err("tag-2 copy is outside decoded data");
                }
                if (0..length)
                    .any(|index| decoded[cursor + index] != decoded[cursor + index - distance])
                {
                    return err("tag-2 copy token differs from source pixels");
                }
                encode_length(&mut bits, length as u32)?;
                if distance <= 32 {
                    put(&mut bits, 1, 1);
                    put(&mut bits, distance as u32 - 1, 5);
                } else {
                    put(&mut bits, 0, 1);
                    let window = cursor as i64 - 33;
                    let width = if (0..2048).contains(&window) {
                        bit_length(window as u32)
                    } else {
                        12
                    };
                    if (distance - 33) as u64 >= 1u64 << width {
                        return err("tag-2 long distance does not fit");
                    }
                    put(&mut bits, distance as u32 - 33, width);
                }
                cursor += length;
            }
        }
    }
    if cursor != decoded.len() {
        return err("tag-2 tokens do not cover decoded data");
    }
    let mut packed = finish_bits(&bits, 1);
    packed[0] = 2;
    Ok(packed)
}

// ---------------------------------------------------------------------------
// arena stream
// ---------------------------------------------------------------------------
/// Encode `decoded` as an arena stream. `tokens` of `None` stores it raw behind
/// a zero split; otherwise the literal runs and copies are laid out from the
/// plan and every copy is checked against `arena` (the bytes that precede this
/// stream) followed by the stream itself.
pub(crate) fn encode_arena(
    decoded: &[u8],
    tokens: Option<&[GeneralToken]>,
    final_flags: u8,
    arena: &[u8],
) -> Result<Vec<u8>, AssetError> {
    let Some(tokens) = tokens else {
        // An empty input is a bare split: the writer adds no bytes after it.
        if !decoded.is_empty()
            && (decoded.last() != Some(&0) || decoded[..decoded.len() - 1].contains(&0))
        {
            return err("raw arena stream must end with its only zero byte");
        }
        let mut output = vec![0, 0];
        output.extend_from_slice(decoded);
        return Ok(output);
    };
    let mut literals = Vec::new();
    let mut flags = Vec::new();
    let mut payloads: Vec<Vec<u8>> = Vec::new();
    let mut consumed = 0usize;
    for token in tokens {
        match *token {
            GeneralToken::Literal(count) => {
                let count = count as usize;
                if consumed + count > decoded.len() {
                    return err("arena literal run exceeds decoded input");
                }
                literals.extend_from_slice(&decoded[consumed..consumed + count]);
                consumed += count;
                flags.extend(std::iter::repeat_n(true, count));
                payloads.extend(std::iter::repeat_n(Vec::new(), count));
            }
            GeneralToken::Copy { length, distance } => {
                if !(1..=0xfff).contains(&distance) || !(3..=273).contains(&length) {
                    return err("arena copy token is invalid");
                }
                if consumed + length as usize > decoded.len() {
                    return err("arena copy exceeds decoded input");
                }
                consumed += length as usize;
                flags.push(false);
                payloads.push(if length <= 17 {
                    (((length - 2) << 12 | distance) as u16)
                        .to_be_bytes()
                        .to_vec()
                } else {
                    let mut payload = (distance as u16).to_be_bytes().to_vec();
                    payload.push((length - 18) as u8);
                    payload
                });
            }
        }
    }
    if consumed != decoded.len() {
        return err("arena plan does not cover decoded input");
    }
    let split = u16::try_from(literals.len() + 2)
        .map_err(|_| AssetError("arena literal block exceeds the split halfword".into()))?;
    flags.push(false);
    payloads.push(vec![0, 0]);
    let used = (flags.len() - 1) % 8 + 1;
    if final_flags & ((1u16 << used) - 1) as u8 != 0 {
        return err("arena final flags overlap planned operations");
    }
    let mut output = split.to_le_bytes().to_vec();
    output.extend_from_slice(&literals);
    for group in 0..flags.len().div_ceil(8) {
        let members = &flags[group * 8..flags.len().min(group * 8 + 8)];
        let mut byte = if group * 8 + members.len() == flags.len() {
            final_flags
        } else {
            0
        };
        for (bit, flag) in members.iter().enumerate() {
            byte |= u8::from(*flag) << bit;
        }
        output.push(byte);
        for payload in &payloads[group * 8..group * 8 + members.len()] {
            output.extend_from_slice(payload);
        }
    }
    let mut combined = arena.to_vec();
    combined.extend_from_slice(&output);
    let base = arena.len() + 2;
    let mut position = 0usize;
    for token in tokens {
        match *token {
            GeneralToken::Literal(count) => position += count as usize,
            GeneralToken::Copy { length, distance } => {
                let source = base - distance as usize;
                if source + length as usize > combined.len()
                    || decoded[position..position + length as usize]
                        != combined[source..source + length as usize]
                {
                    return err("arena copy differs from the preceding bytes");
                }
                position += length as usize;
            }
        }
    }
    Ok(output)
}
/// Copies count back at most 0xfff bytes from the literal block.
const ARENA_WINDOW: usize = 0xfff;
const ARENA_MAX_COPY: usize = 273;
/// A compressed stream longer than this work buffer is stored raw.
const ARENA_WORK_BUFFER: usize = 0x400;
/// Compress `decoded` against `arena`, the bytes that precede the stream.
///
/// Each position takes its longest match, the oldest source on ties. A match
/// ends before the stream's split halfword, lies within the copy window and
/// never reaches past the decoded offset equal to the arena's length. Token
/// lengths are decided from the end: when a strictly longer match starts
/// inside a copy, the parse from that match is walked while it stays inside
/// the copy, and the first token longer than the copy cuts the copy there (a
/// cut shorter than three bytes is a literal). The stream is stored raw when
/// its compressed form exceeds the work buffer or saves less than a tenth of
/// the payload before its terminator.
pub fn compress_arena(decoded: &[u8], arena: &[u8]) -> Result<Vec<u8>, AssetError> {
    let compressed = encode_arena(decoded, Some(&arena_tokens(decoded, arena)), 0, arena)?;
    let payload = decoded.len().saturating_sub(1);
    if compressed.len() > ARENA_WORK_BUFFER || compressed.len() * 10 >= payload * 9 {
        return encode_arena(decoded, None, 0, arena);
    }
    Ok(compressed)
}
fn arena_tokens(decoded: &[u8], arena: &[u8]) -> Vec<GeneralToken> {
    let n = decoded.len();
    let base = arena.len() + 2;
    let low = base.saturating_sub(ARENA_WINDOW).min(arena.len());
    let mut sources = std::collections::HashMap::<u16, Vec<usize>>::new();
    for (index, pair) in arena[low..].windows(2).enumerate() {
        sources
            .entry(u16::from_le_bytes([pair[0], pair[1]]))
            .or_default()
            .push(low + index);
    }
    let mut length = vec![0usize; n];
    let mut source = vec![0usize; n];
    for position in 0..n {
        let limit = ARENA_MAX_COPY
            .min(n - position)
            .min(arena.len().saturating_sub(position));
        if limit < 3 {
            continue;
        }
        let pair = u16::from_le_bytes([decoded[position], decoded[position + 1]]);
        for &start in sources.get(&pair).into_iter().flatten() {
            let mut count = 2;
            while count < limit
                && start + count < arena.len()
                && arena[start + count] == decoded[position + count]
            {
                count += 1;
            }
            if count > length[position] {
                length[position] = count;
                source[position] = start;
            }
        }
        if length[position] < 3 {
            length[position] = 0;
        }
    }
    let mut token = vec![1usize; n];
    for position in (0..n).rev() {
        let own = length[position];
        if own < 3 {
            continue;
        }
        token[position] = own;
        let Some(longer) = (position + 1..position + own).find(|next| length[*next] > own) else {
            continue;
        };
        let mut next = longer;
        while next < n && next - position < own {
            if token[next] > own {
                token[position] = if next - position < 3 {
                    1
                } else {
                    next - position
                };
                break;
            }
            next += token[next];
        }
    }
    let mut tokens = Vec::new();
    let mut position = 0;
    while position < n {
        let step = token[position];
        if step == 1 {
            match tokens.last_mut() {
                Some(GeneralToken::Literal(count)) => *count += 1,
                _ => tokens.push(GeneralToken::Literal(1)),
            }
        } else {
            tokens.push(GeneralToken::Copy {
                length: step as u32,
                distance: (base - source[position]) as u32,
            });
        }
        position += step;
    }
    tokens
}

// ---------------------------------------------------------------------------
// halfword LZ: 16-flag groups over little-endian 16-bit units
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// self-test
// ---------------------------------------------------------------------------
