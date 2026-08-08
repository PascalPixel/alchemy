//! Port of `tools/lib/extract_resource.ts`.
//!
//! PORT NOTE: this crate has no dependency on `tools/lib/zlib.ts`; the two
//! codecs here (the "general" LZ bitstream and the "palette" flag-byte codec)
//! are self-contained, so no decompression seam had to be left open.
//!
//! PORT NOTE: the TypeScript uses `throw new DecodeError(...)` for stream
//! problems and plain `Error` for CLI/usage problems. Rust mirrors that with
//! [`DecodeError`] and, in `main.rs`, a separate usage-error path. Every
//! message string is character-for-character identical to the TypeScript.

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
        let mut bits = LsbBits { data, cursor, end, value: 0, count: 0 };
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
        let word =
            u32::from(self.data[self.cursor]) | u32::from(self.data[self.cursor + 1]) << 8;
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
            let width = if (0..2048).contains(&window) { bit_length(window as u32) } else { 12 };
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
                    let width =
                        if (0..2048).contains(&window) { bit_length(window as u32) } else { 12 };
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
        let first_diff = if at < 0 { "none".to_string() } else { format!("0x{at:x}") };
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
                    if distance < 1
                        || distance as usize > output.len()
                        || distance > 0xfff
                    {
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
        return err(format!("no decoder accepted stream ({})", errors.join("; ")));
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
    let (output, cursor, tokens) = decode_general_trace(&general, 0, general.len(), 4)
        .map_err(|error| error.0)?;
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

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn ported_self_test_passes() {
        self_test().expect("self-test");
    }

    #[test]
    fn general_round_trip_of_synthetic_stream() {
        let general = synthetic_general();
        let (output, cursor, tokens) =
            decode_general_trace(&general, 0, general.len(), 4).unwrap();
        assert_eq!(output, b"ABAB");
        assert!(cursor <= general.len());
        assert_eq!(
            tokens,
            vec![
                GeneralToken::Literal(2),
                GeneralToken::Copy { length: 2, distance: 2 },
            ]
        );
        let reencoded = encode_general(&output, &tokens).unwrap();
        assert_eq!(&general[..reencoded.len()], &reencoded[..]);
        // Re-decoding the re-encoded bytes must give the same output. The
        // reader refills two bytes at a time and may read past the terminator,
        // so a standalone stream needs the same two-byte tail that
        // `synthetic_general` appends.
        let mut reencoded = reencoded.clone();
        reencoded.extend_from_slice(&[0, 0]);
        let (again, _, again_tokens) =
            decode_general_trace(&reencoded, 0, reencoded.len(), 4).unwrap();
        assert_eq!(again, output);
        assert_eq!(again_tokens, tokens);
    }

    #[test]
    fn palette_round_trip_of_synthetic_stream() {
        let palette: Vec<u8> = vec![0x30, 65, 66, 0x01, 0x02, 0, 0];
        let (output, cursor, groups) =
            decode_palette_trace(&palette, 0, palette.len(), 4).unwrap();
        assert_eq!(output, b"ABAB");
        assert_eq!(cursor, palette.len());
        assert_eq!(
            groups,
            vec![PaletteGroup::Group(vec![
                PaletteOperation::Literal,
                PaletteOperation::Literal,
                PaletteOperation::Copy { length: 2, distance: 2 },
                PaletteOperation::End,
            ])]
        );
        assert_eq!(encode_palette(&output, &groups).unwrap(), palette);
    }

    #[test]
    fn truncated_streams_are_rejected() {
        let general = synthetic_general();
        let cut = &general[..general.len() - 2];
        assert_eq!(
            decode_general(cut, 0, cut.len(), 4).unwrap_err(),
            DecodeError("compressed input ended before terminator".into())
        );
        let palette: Vec<u8> = vec![0x30, 65, 66, 0x01, 0x02, 0, 0];
        let cut = &palette[..palette.len() - 1];
        assert_eq!(
            decode_palette(cut, 0, cut.len(), 4).unwrap_err(),
            DecodeError("compressed input ended before terminator".into())
        );
    }

    #[test]
    fn general_requires_kind_zero_header() {
        let mut general = synthetic_general();
        general[0] = 1;
        assert_eq!(
            decode_general(&general, 0, general.len(), 4).unwrap_err(),
            DecodeError("general stream is missing its kind-zero header".into())
        );
        // Empty span reports the same message via the non-prefill entry point.
        assert_eq!(
            decode_general(&general, 0, 0, 4).unwrap_err(),
            DecodeError("general stream is missing its kind-zero header".into())
        );
        // ...but the prefill entry point distinguishes the empty case.
        assert_eq!(
            decode_general_prefill_trace(&general, 0, 0, 4, 0, 1).unwrap_err(),
            DecodeError("general stream is empty".into())
        );
    }

    #[test]
    fn general_bound_is_enforced() {
        let general = synthetic_general();
        // "ABAB" needs 4 bytes; a bound of 3 must be refused.
        assert_eq!(
            decode_general(&general, 0, general.len(), 3).unwrap_err(),
            DecodeError("decoded output crossed configured bound".into())
        );
        assert!(decode_general(&general, 0, general.len(), 4).is_ok());
    }

    #[test]
    fn headerless_prefill_round_trip() {
        // Same bitstream, but written without the kind-zero header byte and
        // decoded against a 64-byte zero prefill.
        let general = synthetic_general();
        let body = &general[1..];
        let (output, _, tokens) =
            decode_general_prefill_trace(body, 0, body.len(), 4, 64, 0).unwrap();
        assert_eq!(output, b"ABAB");
        let mut reencoded = encode_general_prefill(&output, &tokens, 64, 0).unwrap();
        reencoded.extend_from_slice(&[0, 0]);
        let (again, _, _) =
            decode_general_prefill_trace(&reencoded, 0, reencoded.len(), 4, 64, 0).unwrap();
        assert_eq!(again, output);
    }

    #[test]
    fn far_distance_round_trips_through_the_width_ladder() {
        // 300 pseudo-random literals then a distance-200 copy, which forces the
        // position-dependent variable-width distance path.
        let mut decoded: Vec<u8> = Vec::new();
        let mut state: u32 = 12345;
        for _ in 0..300 {
            state = state.wrapping_mul(1103515245).wrapping_add(12345);
            decoded.push((state >> 16) as u8);
        }
        let tail = decoded[300 - 200..300 - 200 + 64].to_vec();
        decoded.extend_from_slice(&tail);
        let tokens = vec![
            GeneralToken::Literal(300),
            GeneralToken::Copy { length: 64, distance: 200 },
        ];
        let mut encoded = encode_general(&decoded, &tokens).unwrap();
        encoded.extend_from_slice(&[0, 0]);
        let (output, _, again) =
            decode_general_trace(&encoded, 0, encoded.len(), 4096).unwrap();
        assert_eq!(output, decoded);
        assert_eq!(again, tokens);
    }

    #[test]
    fn encode_general_reports_the_divergence_coordinate() {
        // Token plan claims two literals but the decoded input has three bytes.
        let error =
            encode_general(b"ABC", &[GeneralToken::Literal(2)]).unwrap_err();
        assert_eq!(
            error,
            DecodeError(
                "token plan does not reconstruct decoded input (replay=2 decoded=3 cursor=2 first_diff=none differing=0)".into()
            )
        );
        // And a plan that mismatches in the middle names the offset.
        let error = encode_general(
            b"AAAAB",
            &[GeneralToken::Literal(1), GeneralToken::Copy { length: 4, distance: 1 }],
        )
        .unwrap_err();
        assert_eq!(
            error,
            DecodeError(
                "token plan does not reconstruct decoded input (replay=5 decoded=5 cursor=5 first_diff=0x4 differing=1)".into()
            )
        );
    }

    #[test]
    fn unencodable_copy_length_is_named() {
        assert_eq!(
            encode_general(b"", &[GeneralToken::Copy { length: 138, distance: 1 }])
                .unwrap_err(),
            DecodeError("unencodable copy length: 138".into())
        );
    }

    #[test]
    fn every_encodable_length_round_trips() {
        for length in 2u32..=137 {
            let decoded = vec![b'X'; 1 + length as usize];
            let tokens = vec![
                GeneralToken::Literal(1),
                GeneralToken::Copy { length, distance: 1 },
            ];
            let mut encoded = encode_general(&decoded, &tokens).unwrap();
            encoded.extend_from_slice(&[0, 0]);
            let (output, _, again) =
                decode_general_trace(&encoded, 0, encoded.len(), 4096).unwrap();
            assert_eq!(output, decoded, "length {length}");
            assert_eq!(again, tokens, "length {length}");
        }
    }

    #[test]
    fn palette_zero_flag_block_round_trips() {
        // Flags byte 0 emits eight raw bytes; then a group with a terminator.
        let mut stream: Vec<u8> = vec![0];
        stream.extend_from_slice(b"01234567");
        stream.push(0x80);
        stream.push(0);
        stream.push(0);
        let (output, cursor, groups) =
            decode_palette_trace(&stream, 0, stream.len(), 64).unwrap();
        assert_eq!(output, b"01234567");
        assert_eq!(cursor, stream.len());
        assert_eq!(groups[0], PaletteGroup::Zeros);
        assert_eq!(encode_palette(&output, &groups).unwrap(), stream);
    }

    #[test]
    fn palette_long_copy_round_trips() {
        // length >= 17 uses the three-byte copy encoding.
        let mut decoded: Vec<u8> = b"ABCD".to_vec();
        for index in 0..20usize {
            let value = decoded[index];
            decoded.push(value);
        }
        let groups = vec![PaletteGroup::Group(vec![
            PaletteOperation::Literal,
            PaletteOperation::Literal,
            PaletteOperation::Literal,
            PaletteOperation::Literal,
            PaletteOperation::Copy { length: 20, distance: 4 },
            PaletteOperation::End,
        ])];
        let encoded = encode_palette(&decoded, &groups).unwrap();
        let (output, cursor, again) =
            decode_palette_trace(&encoded, 0, encoded.len(), 64).unwrap();
        assert_eq!(output, decoded);
        assert_eq!(cursor, encoded.len());
        assert_eq!(again, groups);
    }

    #[test]
    fn palette_plan_guards() {
        assert_eq!(
            encode_palette(b"AB", &[PaletteGroup::Group(vec![PaletteOperation::Literal, PaletteOperation::Literal])])
                .unwrap_err(),
            DecodeError("palette plan lacks a terminator".into())
        );
        assert_eq!(
            encode_palette(
                b"A",
                &[
                    PaletteGroup::Group(vec![PaletteOperation::End]),
                    PaletteGroup::Group(vec![PaletteOperation::Literal]),
                ]
            )
            .unwrap_err(),
            DecodeError("palette plan contains data after terminator".into())
        );
        assert_eq!(
            encode_palette(
                b"A",
                &[PaletteGroup::Group(vec![PaletteOperation::End, PaletteOperation::Literal])]
            )
            .unwrap_err(),
            DecodeError("palette terminator is not group-final".into())
        );
        assert_eq!(
            encode_palette(b"", &[PaletteGroup::Group(vec![])]).unwrap_err(),
            DecodeError("invalid palette token group".into())
        );
        assert_eq!(
            encode_palette(b"AAAAAAAAA", &[PaletteGroup::Group(vec![PaletteOperation::Literal; 9])])
                .unwrap_err(),
            DecodeError("palette group exceeds eight operations".into())
        );
        assert_eq!(
            encode_palette(
                b"A",
                &[PaletteGroup::Group(vec![
                    PaletteOperation::Literal,
                    PaletteOperation::Copy { length: 2, distance: 5 },
                    PaletteOperation::End,
                ])]
            )
            .unwrap_err(),
            DecodeError("palette copy distance is invalid".into())
        );
        assert_eq!(
            encode_palette(
                b"A",
                &[PaletteGroup::Group(vec![
                    PaletteOperation::Literal,
                    PaletteOperation::Copy { length: 273, distance: 1 },
                    PaletteOperation::End,
                ])]
            )
            .unwrap_err(),
            DecodeError("palette copy length is invalid".into())
        );
        assert_eq!(
            encode_palette(b"", &[PaletteGroup::Zeros]).unwrap_err(),
            DecodeError("palette literal block crossed decoded input".into())
        );
    }

    #[test]
    fn auto_dispatch_reports_both_failures() {
        let junk = [1u8, 2, 3, 4];
        let error = decode(&junk, 0, junk.len(), 16, None).unwrap_err();
        assert!(error.0.starts_with("no decoder accepted stream (general: "));
        assert!(error.0.contains("; palette: "));
        assert!(error.0.ends_with(')'));
    }

    #[test]
    fn auto_dispatch_picks_the_single_acceptor() {
        let general = synthetic_general();
        let (kind, output, _) = decode(&general, 0, general.len(), 16, None).unwrap();
        assert_eq!(kind, ResourceKind::General);
        assert_eq!(output, b"ABAB");
    }

    #[test]
    fn explicit_format_bypasses_dispatch() {
        let palette: Vec<u8> = vec![0x30, 65, 66, 0x01, 0x02, 0, 0];
        let (kind, output, cursor) =
            decode(&palette, 0, palette.len(), 4, Some(ResourceKind::Palette)).unwrap();
        assert_eq!(kind, ResourceKind::Palette);
        assert_eq!(output, b"ABAB");
        assert_eq!(cursor, palette.len());
    }

    #[test]
    fn append_copy_guards() {
        let mut output = vec![1u8, 2, 3];
        assert_eq!(
            append_copy(&mut output, 4, 1, 64).unwrap_err(),
            DecodeError("invalid back-reference distance at output offset 0x3".into())
        );
        assert_eq!(
            append_copy(&mut output, 0, 1, 64).unwrap_err(),
            DecodeError("invalid back-reference distance at output offset 0x3".into())
        );
        assert_eq!(
            append_copy(&mut output, 1, 100, 64).unwrap_err(),
            DecodeError("decoded output crossed configured bound".into())
        );
        append_copy(&mut output, 3, 3, 64).unwrap();
        assert_eq!(output, vec![1, 2, 3, 1, 2, 3]);
    }

    #[test]
    fn bit_length_matches_the_javascript_definition() {
        assert_eq!(bit_length(0), 0);
        assert_eq!(bit_length(1), 1);
        assert_eq!(bit_length(2), 2);
        assert_eq!(bit_length(2047), 11);
        assert_eq!(bit_length(2048), 12);
    }

    #[test]
    fn odd_cursor_primes_the_bit_reader() {
        // Constructing at an odd offset consumes one byte before the first
        // 16-bit fill; the payload must still decode identically.
        let general = synthetic_general();
        let mut shifted = vec![0xffu8];
        shifted.extend_from_slice(&general);
        let bits = LsbBits::new(&shifted, 1, shifted.len()).unwrap();
        assert_eq!(bits.count, 24);
        assert_eq!(bits.value & 0xff, 0x00);
    }
}
