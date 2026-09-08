//! Explicit LZ stream codecs, independent of ROM layouts and resource tables.
use std::fmt;
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct DecodeError(pub String);
impl fmt::Display for DecodeError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(&self.0)
    }
}
impl std::error::Error for DecodeError {}
fn err<T>(message: impl Into<String>) -> Result<T, DecodeError> {
    Err(DecodeError(message.into()))
}
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
pub struct LsbBits<'a> {
    pub data: &'a [u8],
    pub cursor: usize,
    pub end: usize,
    pub value: u32,
    pub count: u32,
}
impl<'a> LsbBits<'a> {
    pub fn new(data: &'a [u8], cursor: usize, end: usize) -> Result<Self, DecodeError> {
        let mut bits = LsbBits {
            data,
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
    pub fn need(&self, size: usize) -> Result<(), DecodeError> {
        if self.cursor + size > self.end {
            return err("compressed input ended before terminator");
        }
        Ok(())
    }
    pub fn byte(&mut self) -> Result<u8, DecodeError> {
        self.need(1)?;
        let value = self.data[self.cursor];
        self.cursor += 1;
        Ok(value)
    }
    pub fn fill(&mut self) -> Result<(), DecodeError> {
        self.need(2)?;
        let word = u32::from(self.data[self.cursor]) | u32::from(self.data[self.cursor + 1]) << 8;
        self.value |= word.wrapping_shl(self.count);
        self.cursor += 2;
        self.count += 16;
        Ok(())
    }
    pub fn get(&mut self, count: u32) -> Result<u32, DecodeError> {
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
pub fn append_copy(
    output: &mut Vec<u8>,
    distance: u32,
    length: u32,
    maximum: u64,
) -> Result<(), DecodeError> {
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
fn decode_length(bits: &mut LsbBits) -> Result<Option<u32>, DecodeError> {
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
) -> Result<(Vec<u8>, usize, Vec<GeneralToken>), DecodeError> {
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
            None => return Ok((output[prefill..].to_vec(), bits.cursor, tokens)),
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
pub fn decode_general_trace(
    data: &[u8],
    start: usize,
    end: usize,
    maximum: u64,
) -> Result<(Vec<u8>, usize, Vec<GeneralToken>), DecodeError> {
    if start >= end || data[start] != 0 {
        return err("general stream is missing its kind-zero header");
    }
    decode_general_body(data, start, end, maximum, 0, 1)
}
pub fn decode_general(
    data: &[u8],
    start: usize,
    end: usize,
    maximum: u64,
) -> Result<(Vec<u8>, usize), DecodeError> {
    let (output, cursor, _) = decode_general_trace(data, start, end, maximum)?;
    Ok((output, cursor))
}
pub fn decode_general_prefill_trace(
    data: &[u8],
    start: usize,
    end: usize,
    maximum: u64,
    prefill: usize,
    header: usize,
) -> Result<(Vec<u8>, usize, Vec<GeneralToken>), DecodeError> {
    if start >= end {
        return err("general stream is empty");
    }
    if header != 0 && data[start] != 0 {
        return err("general stream is missing its kind-zero header");
    }
    decode_general_body(data, start, end, maximum, prefill, header)
}
pub fn put(bits: &mut Vec<u8>, value: u32, count: u32) {
    for index in 0..count {
        bits.push(((value >> index) & 1) as u8);
    }
}
fn encode_length(bits: &mut Vec<u8>, length: u32) -> Result<(), DecodeError> {
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
) -> Result<Vec<u8>, DecodeError> {
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
pub fn encode_general(decoded: &[u8], tokens: &[GeneralToken]) -> Result<Vec<u8>, DecodeError> {
    encode_general_inner(decoded, tokens, 0, 1)
}
pub fn encode_general_prefill(
    decoded: &[u8],
    tokens: &[GeneralToken],
    prefill: usize,
    header: usize,
) -> Result<Vec<u8>, DecodeError> {
    encode_general_inner(decoded, tokens, prefill, header)
}
// ---------------------------------------------------------------------------
// palette stream
// ---------------------------------------------------------------------------
pub fn decode_palette_trace(
    data: &[u8],
    start: usize,
    end: usize,
    maximum: u64,
) -> Result<(Vec<u8>, usize, Vec<PaletteGroup>), DecodeError> {
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
) -> Result<(Vec<u8>, usize), DecodeError> {
    let (output, cursor, _) = decode_palette_trace(data, start, end, maximum)?;
    Ok((output, cursor))
}
pub fn encode_palette(decoded: &[u8], groups: &[PaletteGroup]) -> Result<Vec<u8>, DecodeError> {
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
/// Encode `decoded` as a tag-2 stream: tag byte 2, then LSB-first bits where
/// a literal is `1`, a two-bit width selector (`1` = 2 bits, `01` = 3 bits,
/// `00` = 4 bits) and the low then high nibble as MTF indices, and a copy uses
/// the general stream's length and position-dependent distance coding.
pub fn encode_mtf4_lz(decoded: &[u8], tokens: &[Mtf4LzToken]) -> Result<Vec<u8>, DecodeError> {
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
#[test]
fn mtf4_lz_streams_carry_the_tag_and_move_to_front_literals() {
    let literal = |width| Mtf4LzToken::Literal { width };
    assert_eq!(
        encode_mtf4_lz(&[0], &[literal(2)]).unwrap(),
        [2, 0x83, 0x0f, 0]
    );
    // 0x21 moves 1 then 2 to the front; the second byte then reads them back
    // at MTF indices 1 and 1, and the copy repeats the pair at distance 2.
    let encoded = encode_mtf4_lz(
        &[0x21, 0x12, 0x21, 0x12],
        &[
            literal(2),
            literal(2),
            Mtf4LzToken::Copy {
                length: 2,
                distance: 2,
            },
        ],
    )
    .unwrap();
    assert_eq!(encoded, [2, 0xe7, 0xc4, 0xe0, 0x03, 0]);
    assert!(encode_mtf4_lz(&[0x1f], &[literal(2)]).is_err());
    assert!(encode_mtf4_lz(&[0, 0], &[literal(2)]).is_err());
    assert!(encode_mtf4_lz(
        &[0, 1],
        &[
            literal(2),
            Mtf4LzToken::Copy {
                length: 1,
                distance: 1
            }
        ]
    )
    .is_err());
}
// ---------------------------------------------------------------------------
// arena stream
// ---------------------------------------------------------------------------
/// Decode an arena stream at `offset`: a little-endian split halfword, the
/// literal block, then flag groups whose copies read from the bytes that
/// precede the literal block in `data`. A zero split stores the stream raw
/// up to and including its zero terminator. Returns the decoded stream, the
/// encoded length and the unused final flag bits.
pub fn decode_arena(data: &[u8], offset: usize) -> Result<(Vec<u8>, usize, u8), DecodeError> {
    let split = usize::from(u16::from_le_bytes([
        *data
            .get(offset)
            .ok_or_else(|| DecodeError("arena split is truncated".into()))?,
        *data
            .get(offset + 1)
            .ok_or_else(|| DecodeError("arena split is truncated".into()))?,
    ]));
    let base = offset + 2;
    if split == 0 {
        let end = data[base..]
            .iter()
            .position(|byte| *byte == 0)
            .ok_or_else(|| DecodeError("raw arena stream has no terminator".into()))?;
        return Ok((data[base..=base + end].to_vec(), 3 + end, 0));
    }
    if split < 2 || offset + split >= data.len() {
        return err("arena split is outside the encoded data");
    }
    let control = offset + split;
    let mut literal = base;
    let mut cursor = control;
    let mut output = Vec::new();
    loop {
        let flags = *data
            .get(cursor)
            .ok_or_else(|| DecodeError("arena flag group is truncated".into()))?;
        cursor += 1;
        for bit in 0..8 {
            if flags & (1 << bit) != 0 {
                if literal >= control {
                    return err("arena literal block is exhausted");
                }
                output.push(data[literal]);
                literal += 1;
                continue;
            }
            let word = u16::from_be_bytes([
                *data
                    .get(cursor)
                    .ok_or_else(|| DecodeError("arena copy is truncated".into()))?,
                *data
                    .get(cursor + 1)
                    .ok_or_else(|| DecodeError("arena copy is truncated".into()))?,
            ]);
            cursor += 2;
            if word == 0 {
                if literal != control {
                    return err("arena literal block was not consumed exactly");
                }
                return Ok((output, cursor - offset, flags & !((2u16 << bit) - 1) as u8));
            }
            let distance = usize::from(word & 0x0fff);
            let length = if word >> 12 == 0 {
                let extra = *data
                    .get(cursor)
                    .ok_or_else(|| DecodeError("arena copy length is truncated".into()))?;
                cursor += 1;
                usize::from(extra) + 18
            } else {
                usize::from(word >> 12) + 2
            };
            let source = base
                .checked_sub(distance)
                .filter(|source| distance != 0 && source + length <= data.len())
                .ok_or_else(|| DecodeError("arena copy is outside the encoded data".into()))?;
            for index in 0..length {
                output.push(data[source + index]);
            }
        }
    }
}
/// Encode `decoded` as an arena stream. `tokens` of `None` stores it raw behind
/// a zero split; otherwise the literal runs and copies are laid out from the
/// plan and every copy is checked against `arena` (the bytes that precede this
/// stream) followed by the stream itself.
pub fn encode_arena(
    decoded: &[u8],
    tokens: Option<&[GeneralToken]>,
    final_flags: u8,
    arena: &[u8],
) -> Result<Vec<u8>, DecodeError> {
    let Some(tokens) = tokens else {
        if decoded.last() != Some(&0) || decoded[..decoded.len() - 1].contains(&0) {
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
        .map_err(|_| DecodeError("arena literal block exceeds the split halfword".into()))?;
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
#[test]
fn arena_streams_round_trip_and_check_their_dictionary() {
    let arena = vec![0x55; 96];
    let raw = encode_arena(&[1, 2, 3, 0], None, 0, &arena).unwrap();
    assert_eq!(raw, [0, 0, 1, 2, 3, 0]);
    assert_eq!(decode_arena(&raw, 0).unwrap(), (vec![1, 2, 3, 0], 6, 0));
    assert!(encode_arena(&[1, 0, 2], None, 0, &arena).is_err());
    let tokens = [
        GeneralToken::Literal(2),
        GeneralToken::Copy {
            length: 4,
            distance: 8,
        },
        GeneralToken::Literal(1),
    ];
    let decoded = [7, 8, 0x55, 0x55, 0x55, 0x55, 0];
    let encoded = encode_arena(&decoded, Some(&tokens), 0xe0, &arena).unwrap();
    assert_eq!(encoded, [5, 0, 7, 8, 0, 0xeb, 0x20, 0x08, 0, 0]);
    let mut data = arena.clone();
    data.extend_from_slice(&encoded);
    assert_eq!(
        decode_arena(&data, arena.len()).unwrap(),
        (decoded.to_vec(), encoded.len(), 0xe0)
    );
    assert!(encode_arena(&[7, 8, 1, 2, 3, 4, 0], Some(&tokens), 0, &arena).is_err());
    assert!(encode_arena(&decoded, Some(&tokens), 0x08, &arena).is_err());
    assert!(encode_arena(&decoded[..6], Some(&tokens), 0, &arena).is_err());
}
// ---------------------------------------------------------------------------
// halfword LZ: 16-flag groups over little-endian 16-bit units
// ---------------------------------------------------------------------------
/// `["l", n]` / `["c", length, distance]` / `["e"]` over halfword units.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum HalfwordToken {
    Literal(u32),
    Copy { length: u32, distance: u32 },
    End,
}
/// Each group is a flag halfword (bit 15 first) followed by up to sixteen
/// halfwords: a literal unit, a copy `(distance << 5) | (length - 2)` with
/// `distance` 1..=2047 and `length` 2..=33, or a flagged zero terminator.
pub fn decode_halfword(data: &[u8]) -> Result<(Vec<u8>, Vec<HalfwordToken>), DecodeError> {
    let read = |at: usize| -> Result<u16, DecodeError> {
        match data.get(at..at + 2) {
            Some(bytes) => Ok(u16::from_le_bytes([bytes[0], bytes[1]])),
            None => err("halfword stream ended inside a group"),
        }
    };
    let mut output: Vec<u16> = Vec::new();
    let mut tokens = Vec::new();
    let mut cursor = 0;
    loop {
        let flags = read(cursor)?;
        cursor += 2;
        for index in 0..16 {
            let word = read(cursor)?;
            cursor += 2;
            if flags & (1 << (15 - index)) == 0 {
                output.push(word);
                match tokens.last_mut() {
                    Some(HalfwordToken::Literal(count)) => *count += 1,
                    _ => tokens.push(HalfwordToken::Literal(1)),
                }
                continue;
            }
            if word == 0 {
                tokens.push(HalfwordToken::End);
                return Ok((
                    output.iter().flat_map(|unit| unit.to_le_bytes()).collect(),
                    tokens,
                ));
            }
            let distance = u32::from(word >> 5);
            let length = u32::from(word & 31) + 2;
            if distance as usize > output.len() {
                return err("halfword copy crossed replay prefix");
            }
            for _ in 0..length {
                output.push(output[output.len() - distance as usize]);
            }
            tokens.push(HalfwordToken::Copy { length, distance });
        }
    }
}
pub fn encode_halfword(decoded: &[u8], tokens: &[HalfwordToken]) -> Result<Vec<u8>, DecodeError> {
    if decoded.len() % 2 != 0 {
        return err("halfword pixels have an odd size");
    }
    let units: Vec<u16> = decoded
        .chunks(2)
        .map(|pair| u16::from_le_bytes([pair[0], pair[1]]))
        .collect();
    let mut operations = Vec::new();
    for token in tokens {
        match *token {
            HalfwordToken::Literal(count) => {
                if count == 0 || count as usize > units.len() {
                    return err("halfword literal count is invalid");
                }
                operations.extend((0..count).map(|_| HalfwordToken::Literal(1)));
            }
            HalfwordToken::Copy { length, distance } => {
                if !(2..=33).contains(&length) || !(1..=2047).contains(&distance) {
                    return err("halfword copy differs");
                }
                operations.push(*token);
            }
            HalfwordToken::End => operations.push(HalfwordToken::End),
        }
    }
    let mut encoded = Vec::new();
    let mut replay: Vec<u16> = Vec::new();
    let mut ended = false;
    for group in operations.chunks(16) {
        let mut flags = 0u16;
        let mut words = Vec::new();
        for (index, operation) in group.iter().enumerate() {
            if ended {
                return err("halfword plan has data after terminator");
            }
            match *operation {
                HalfwordToken::Literal(_) => {
                    let unit = *units.get(replay.len()).ok_or(DecodeError(
                        "halfword literal crossed decoded pixels".into(),
                    ))?;
                    replay.push(unit);
                    words.push(unit);
                }
                HalfwordToken::Copy { length, distance } => {
                    flags |= 1 << (15 - index);
                    if distance as usize > replay.len() {
                        return err("halfword copy crossed replay prefix");
                    }
                    words.push(((distance << 5) | (length - 2)) as u16);
                    for _ in 0..length {
                        replay.push(replay[replay.len() - distance as usize]);
                    }
                }
                HalfwordToken::End => {
                    flags |= 1 << (15 - index);
                    words.push(0);
                    ended = true;
                }
            }
        }
        encoded.extend(flags.to_le_bytes());
        encoded.extend(words.iter().flat_map(|word| word.to_le_bytes()));
    }
    if !ended || replay != units {
        return err("halfword plan does not reconstruct decoded pixels");
    }
    Ok(encoded)
}
// ---------------------------------------------------------------------------
// self-test
// ---------------------------------------------------------------------------
#[cfg(test)]
fn synthetic_general() -> Vec<u8> {
    let mut bits: Vec<u8> = Vec::new();
    for value in b"AB" {
        put(&mut bits, 1, 1);
        put(&mut bits, u32::from(*value), 8);
    }
    put(&mut bits, 0, 1);
    put(&mut bits, 0, 1);
    put(&mut bits, 1, 1);
    put(&mut bits, 1, 5);
    put(&mut bits, 0, 1);
    put(&mut bits, 0x1f, 5);
    put(&mut bits, 0, 2);
    put(&mut bits, 0, 7);
    let mut packed: Vec<u8> = vec![0];
    for chunk in bits.chunks(8) {
        let mut value = 0u8;
        for (index, bit) in chunk.iter().enumerate() {
            value |= bit << index;
        }
        packed.push(value);
    }
    packed.push(0);
    packed.push(0);
    packed
}
#[test]
fn codec_round_trips_and_truncation() -> Result<(), String> {
    let general = synthetic_general();
    let (output, cursor, tokens) =
        decode_general_trace(&general, 0, general.len(), 4).map_err(|error| error.0)?;
    if output != b"ABAB" || cursor > general.len() {
        return Err("general decoder self-test failed".into());
    }
    let reencoded = encode_general(&output, &tokens).map_err(|error| error.0)?;
    if general.len() < reencoded.len() || general[..reencoded.len()] != reencoded[..] {
        return Err("general encoder self-test failed".into());
    }
    let palette: Vec<u8> = vec![0x30, 65, 66, 0x01, 0x02, 0, 0];
    let (output, cursor, groups) =
        decode_palette_trace(&palette, 0, palette.len(), 4).map_err(|error| error.0)?;
    if output != b"ABAB" || cursor != palette.len() {
        return Err("palette decoder self-test failed".into());
    }
    if encode_palette(&output, &groups).map_err(|error| error.0)? != palette {
        return Err("palette encoder self-test failed".into());
    }
    let halfword: Vec<u8> = vec![0x00, 0x60, 0x41, 0x00, 0x20, 0x00, 0x00, 0x00];
    let (output, tokens) = decode_halfword(&halfword).map_err(|error| error.0)?;
    if output != b"A\0A\0A\0"
        || encode_halfword(&output, &tokens).map_err(|error| error.0)? != halfword
    {
        return Err("halfword codec self-test failed".into());
    }
    let truncated_general = &general[..general.len() - 2];
    if decode_general(truncated_general, 0, truncated_general.len(), 4).is_ok() {
        return Err("truncated stream was accepted".into());
    }
    let truncated_palette = &palette[..palette.len() - 1];
    if decode_palette(truncated_palette, 0, truncated_palette.len(), 4).is_ok() {
        return Err("truncated stream was accepted".into());
    }
    Ok(())
}
