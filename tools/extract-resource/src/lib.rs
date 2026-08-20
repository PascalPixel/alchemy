//! Port of `tools/lib/extract_resource.ts`.
//!
//! PORT NOTE: this crate has no dependency on the shared zlib crate; the two
//! codecs here (the "general" LZ bitstream and the "palette" flag-byte codec)
//! are self-contained, so no decompression seam had to be left open.
//!
//! PORT NOTE: the former implementation used `throw new DecodeError(...)` for
//! stream problems and plain `Error` for CLI/usage problems. Rust mirrors that with
//! [`DecodeError`] and, in `main.rs`, a separate usage-error path. Every
//! message string is character-for-character identical to the former behavior.

pub mod cli;

use std::fmt;

pub const ROM_BASE: u32 = 0x0800_0000;
pub const TABLE: u32 = 0x0832_0000;

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
        return err(format!(
            "token plan does not reconstruct decoded input (replay={} decoded={} cursor={} first_diff={} differing={})",
            rebuilt.len(),
            decoded.len(),
            cursor,
            first_diff,
            differing
        ));
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
// dispatch
// ---------------------------------------------------------------------------

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ResourceKind {
    General,
    Palette,
}

impl ResourceKind {
    pub fn name(self) -> &'static str {
        match self {
            ResourceKind::General => "general",
            ResourceKind::Palette => "palette",
        }
    }
}

impl fmt::Display for ResourceKind {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(self.name())
    }
}

pub fn decode(
    data: &[u8],
    start: usize,
    end: usize,
    maximum: u64,
    kind: Option<ResourceKind>,
) -> Result<(ResourceKind, Vec<u8>, usize), DecodeError> {
    if let Some(kind) = kind {
        let (output, cursor) = match kind {
            ResourceKind::General => decode_general(data, start, end, maximum)?,
            ResourceKind::Palette => decode_palette(data, start, end, maximum)?,
        };
        return Ok((kind, output, cursor));
    }
    let mut valid: Vec<(ResourceKind, Vec<u8>, usize)> = Vec::new();
    let mut errors: Vec<String> = Vec::new();
    for name in [ResourceKind::General, ResourceKind::Palette] {
        let attempt = match name {
            ResourceKind::General => decode_general(data, start, end, maximum),
            ResourceKind::Palette => decode_palette(data, start, end, maximum),
        };
        match attempt {
            Ok((output, cursor)) => valid.push((name, output, cursor)),
            Err(error) => errors.push(format!("{}: {}", name.name(), error.0)),
        }
    }
    if valid.len() == 1 {
        return Ok(valid.remove(0));
    }
    if valid.is_empty() {
        return err(format!(
            "no decoder accepted stream ({})",
            errors.join("; ")
        ));
    }
    err("stream is ambiguous; specify --format general or palette")
}

// ---------------------------------------------------------------------------
// self-test
// ---------------------------------------------------------------------------

pub fn synthetic_general() -> Vec<u8> {
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

/// Mirrors the TypeScript `self_test`. Returns `Err` with the same message the
/// TypeScript would have thrown.
pub fn self_test() -> Result<(), String> {
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
