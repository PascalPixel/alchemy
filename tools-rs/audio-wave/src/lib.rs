//! Byte-exact Rust port of `tools/make/audio_wave.ts`.
//!
//! PORT NOTE (provenance): the TypeScript original imports no local modules, so
//! nothing had to be inlined from elsewhere in the repo. Every function below
//! corresponds 1:1 to an export of `tools/make/audio_wave.ts`.
//!
//! PORT NOTE (numerics): JavaScript has a single `number` type (f64). Every
//! quantity that the TS keeps in a `number` is kept in `f64` here, including
//! `offset`/`size`/`rate` parameters and the value produced by
//! `Math.round(frequency / 1024)`. Conversion to integer types happens only
//! after the same `Number.isInteger` / range guards the TS applies. The two
//! sample transforms use `& 0xff` in JS on values that are already in
//! `-128..=255`; they are reproduced with `u8::wrapping_add` /
//! `u8::wrapping_sub`, which is bit-identical.
//!
//! PORT NOTE (errors): the TS throws `Error`; every fallible function here
//! returns `Result<_, String>` carrying the identical message text. Node's
//! `Buffer.alloc` throws a `RangeError` for absurd lengths; the Rust port
//! reports `wave record size is not allocatable` instead of aborting. That is
//! the only message that has no TS counterpart, and it is unreachable for any
//! input the TS would have accepted on a 64-bit host.

/// A manifest scalar: JSON gives these as either a string or a number, exactly
/// like the TS `string | number` unions.
#[derive(Debug, Clone, PartialEq)]
pub enum Scalar {
    Num(f64),
    Str(String),
}

impl Scalar {
    pub fn str(value: &str) -> Self {
        Scalar::Str(value.to_string())
    }
}

impl From<f64> for Scalar {
    fn from(value: f64) -> Self {
        Scalar::Num(value)
    }
}

impl From<u32> for Scalar {
    fn from(value: u32) -> Self {
        Scalar::Num(f64::from(value))
    }
}

impl From<&str> for Scalar {
    fn from(value: &str) -> Self {
        Scalar::str(value)
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct ExactWaveHeaderSource {
    pub control: Scalar,
    pub frequency: Scalar,
    pub loop_start: Scalar,
    pub sample_count: Scalar,
}

#[derive(Debug, Clone, PartialEq)]
pub struct ExactWavePaddingSource {
    pub size: Scalar,
    pub fill: Scalar,
}

#[derive(Debug, Clone, PartialEq)]
pub struct WaveRecordSource {
    pub frequency: Scalar,
    /// `null` in the TS is `None` here.
    pub loop_start: Option<f64>,
    pub size: Scalar,
    pub header: Option<ExactWaveHeaderSource>,
    pub padding: Option<ExactWavePaddingSource>,
}

#[derive(Debug, Clone, PartialEq)]
pub struct ProbedWaveRecord {
    pub header: ExactWaveHeaderSource,
    pub padding: ExactWavePaddingSource,
    pub samples: Vec<u8>,
}

/// Mirrors the `Record<string, number | boolean | null>` the TS returns
/// alongside the built record. Field order matches the TS object-literal
/// insertion order, which is the order a JSON serializer would emit.
#[derive(Debug, Clone, PartialEq)]
pub struct WaveReport {
    pub samples: f64,
    pub rate: f64,
    pub frequency: f64,
    pub control: f64,
    pub looped: bool,
    pub loop_start: Option<f64>,
    pub padding_bytes: f64,
    pub padding_fill: f64,
}

// ---------------------------------------------------------------------------
// JavaScript `Number(value)` on a string
// ---------------------------------------------------------------------------

fn is_js_trimmable(c: char) -> bool {
    matches!(
        c,
        '\u{9}'
            | '\u{A}'
            | '\u{B}'
            | '\u{C}'
            | '\u{D}'
            | '\u{20}'
            | '\u{A0}'
            | '\u{1680}'
            | '\u{2000}'..='\u{200A}'
            | '\u{2028}'
            | '\u{2029}'
            | '\u{202F}'
            | '\u{205F}'
            | '\u{3000}'
            | '\u{FEFF}'
    )
}

fn radix_value(text: &str, radix: u32) -> f64 {
    if text.is_empty() {
        return f64::NAN;
    }
    let mut exact: u128 = 0;
    let mut wide = 0.0f64;
    let mut overflowed = false;
    for c in text.chars() {
        let digit = match c.to_digit(radix) {
            Some(digit) => digit,
            None => return f64::NAN,
        };
        if !overflowed {
            match exact
                .checked_mul(u128::from(radix))
                .and_then(|v| v.checked_add(u128::from(digit)))
            {
                Some(next) => exact = next,
                None => {
                    overflowed = true;
                    wide = exact as f64;
                }
            }
        }
        if overflowed {
            wide = wide * f64::from(radix) + f64::from(digit);
        }
    }
    if overflowed {
        wide
    } else {
        exact as f64
    }
}

/// True when `text` is a JS `StrDecimalLiteral` (sign already stripped is *not*
/// assumed; this checks the whole production minus the Infinity case).
fn is_decimal_literal(text: &str) -> bool {
    let bytes = text.as_bytes();
    let mut index = 0;
    let mut significand_digits = 0;
    while index < bytes.len() && bytes[index].is_ascii_digit() {
        index += 1;
        significand_digits += 1;
    }
    if index < bytes.len() && bytes[index] == b'.' {
        index += 1;
        while index < bytes.len() && bytes[index].is_ascii_digit() {
            index += 1;
            significand_digits += 1;
        }
    }
    if significand_digits == 0 {
        return false;
    }
    if index < bytes.len() && (bytes[index] == b'e' || bytes[index] == b'E') {
        index += 1;
        if index < bytes.len() && (bytes[index] == b'+' || bytes[index] == b'-') {
            index += 1;
        }
        let mut exponent_digits = 0;
        while index < bytes.len() && bytes[index].is_ascii_digit() {
            index += 1;
            exponent_digits += 1;
        }
        if exponent_digits == 0 {
            return false;
        }
    }
    index == bytes.len()
}

/// `Number(string)` from the ECMAScript spec: trims JS whitespace, treats the
/// empty string as `0`, understands `0x`/`0o`/`0b` prefixes (unsigned only),
/// `Infinity`, and otherwise a decimal literal. Anything else is `NaN`.
pub fn js_number_from_string(value: &str) -> f64 {
    let text = value.trim_matches(is_js_trimmable);
    if text.is_empty() {
        return 0.0;
    }
    if text.len() > 2 {
        let (prefix, rest) = text.split_at(2);
        match prefix {
            "0x" | "0X" => return radix_value(rest, 16),
            "0o" | "0O" => return radix_value(rest, 8),
            "0b" | "0B" => return radix_value(rest, 2),
            _ => {}
        }
    }
    let (sign, body) = match text.as_bytes()[0] {
        b'+' => (1.0, &text[1..]),
        b'-' => (-1.0, &text[1..]),
        _ => (1.0, text),
    };
    if body == "Infinity" {
        return sign * f64::INFINITY;
    }
    if !is_decimal_literal(body) {
        return f64::NAN;
    }
    match body.parse::<f64>() {
        Ok(parsed) => sign * parsed,
        Err(_) => f64::NAN,
    }
}

fn to_number(value: &Scalar) -> f64 {
    match value {
        Scalar::Num(number) => *number,
        Scalar::Str(text) => js_number_from_string(text),
    }
}

fn is_integer(value: f64) -> bool {
    value.is_finite() && value.fract() == 0.0
}

// ---------------------------------------------------------------------------
// Guards
// ---------------------------------------------------------------------------

fn integer(value: &Scalar, label: &str) -> Result<f64, String> {
    let result = to_number(value);
    if !is_integer(result) || result < 0.0 {
        return Err(format!("{label} must be a nonnegative integer"));
    }
    Ok(result)
}

fn uint32(value: &Scalar, label: &str) -> Result<f64, String> {
    let result = integer(value, label)?;
    if result > 4294967295.0 {
        return Err(format!("{label} must fit an unsigned word"));
    }
    Ok(result)
}

fn byte(value: &Scalar, label: &str) -> Result<f64, String> {
    let result = integer(value, label)?;
    if result > 255.0 {
        return Err(format!("{label} must fit an unsigned byte"));
    }
    Ok(result)
}

fn four_cc(data: &[u8], offset: usize, expected: &str) -> Result<(), String> {
    // PORT NOTE: `Buffer.toString("ascii", …)` masks each byte with 0x7f, so a
    // byte of 0xC4 reads as 'D'. Reproduced here rather than using UTF-8
    // decoding, which would differ on non-ASCII input.
    let end = (offset + 4).min(data.len());
    let slice = if offset >= data.len() {
        &data[0..0]
    } else {
        &data[offset..end]
    };
    let decoded: String = slice.iter().map(|byte| (byte & 0x7f) as char).collect();
    if decoded != expected {
        return Err(format!("WAV is missing {expected}"));
    }
    Ok(())
}

fn read_u32_le(data: &[u8], offset: usize) -> f64 {
    f64::from(u32::from_le_bytes([
        data[offset],
        data[offset + 1],
        data[offset + 2],
        data[offset + 3],
    ]))
}

fn read_u16_le(data: &[u8], offset: usize) -> f64 {
    f64::from(u16::from_le_bytes([data[offset], data[offset + 1]]))
}

fn write_u32_le(data: &mut [u8], offset: usize, value: f64) {
    data[offset..offset + 4].copy_from_slice(&(value as u32).to_le_bytes());
}

fn write_u16_le(data: &mut [u8], offset: usize, value: u16) {
    data[offset..offset + 2].copy_from_slice(&value.to_le_bytes());
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

/// `wavFromSignedPcm`: wrap signed 8-bit PCM in a canonical mono RIFF/WAVE.
pub fn wav_from_signed_pcm(samples: &[u8], rate: f64) -> Result<Vec<u8>, String> {
    if !is_integer(rate) || !(1.0..=4294967295.0).contains(&rate) {
        return Err("invalid WAV rate".to_string());
    }
    let mut result = vec![0u8; 44 + samples.len()];
    result[0..4].copy_from_slice(b"RIFF");
    write_u32_le(&mut result, 4, (36 + samples.len()) as f64);
    result[8..12].copy_from_slice(b"WAVE");
    result[12..16].copy_from_slice(b"fmt ");
    write_u32_le(&mut result, 16, 16.0);
    write_u16_le(&mut result, 20, 1);
    write_u16_le(&mut result, 22, 1);
    write_u32_le(&mut result, 24, rate);
    write_u32_le(&mut result, 28, rate);
    write_u16_le(&mut result, 32, 1);
    write_u16_le(&mut result, 34, 8);
    result[36..40].copy_from_slice(b"data");
    write_u32_le(&mut result, 40, samples.len() as f64);
    for index in 0..samples.len() {
        result[44 + index] = samples[index].wrapping_add(128);
    }
    Ok(result)
}

/// `signedPcmFromWav`: unwrap a canonical mono 8-bit WAV back to signed PCM.
pub fn signed_pcm_from_wav(data: &[u8], expected_rate: f64) -> Result<Vec<u8>, String> {
    if data.len() < 44 {
        return Err("WAV is truncated".to_string());
    }
    four_cc(data, 0, "RIFF")?;
    four_cc(data, 8, "WAVE")?;
    four_cc(data, 12, "fmt ")?;
    four_cc(data, 36, "data")?;
    let length = data.len() as f64;
    if read_u32_le(data, 4) != length - 8.0
        || read_u32_le(data, 16) != 16.0
        || read_u16_le(data, 20) != 1.0
        || read_u16_le(data, 22) != 1.0
        || read_u32_le(data, 24) != expected_rate
        || read_u32_le(data, 28) != expected_rate
        || read_u16_le(data, 32) != 1.0
        || read_u16_le(data, 34) != 8.0
        || read_u32_le(data, 40) != length - 44.0
    {
        return Err("WAV is not canonical mono 8-bit PCM".to_string());
    }
    Ok(data[44..].iter().map(|byte| byte.wrapping_sub(128)).collect())
}

/// `buildWaveRecord`: assemble the 16-byte header plus samples plus padding.
pub fn build_wave_record(
    source: &WaveRecordSource,
    wav: &[u8],
) -> Result<(Vec<u8>, WaveReport), String> {
    let exact = source.header.as_ref();
    let frequency = match exact {
        Some(header) => uint32(&header.frequency, "wave frequency")?,
        None => uint32(&source.frequency, "wave frequency")?,
    };
    if exact.is_some() && uint32(&source.frequency, "wave catalog frequency")? != frequency {
        return Err("wave catalog frequency differs from exact header".to_string());
    }
    let size = integer(&source.size, "wave record size")?;
    let rate = js_round(frequency / 1024.0);
    let samples = signed_pcm_from_wav(wav, rate)?;
    if samples.is_empty() {
        return Err("wave record has no samples".to_string());
    }
    let control = match exact {
        None => {
            if source.loop_start.is_none() {
                0.0
            } else {
                1073741824.0
            }
        }
        Some(header) => uint32(&header.control, "wave control")?,
    };
    let loop_start = match exact {
        None => match source.loop_start {
            None => 0.0,
            Some(value) => uint32(&Scalar::Num(value), "wave loop start")?,
        },
        Some(header) => uint32(&header.loop_start, "wave loop start")?,
    };
    let looped = ((control as u32) & 0xc000_0000) != 0;
    let catalog_loop = match source.loop_start {
        None => None,
        Some(value) => Some(uint32(&Scalar::Num(value), "wave catalog loop start")?),
    };
    if exact.is_some() {
        let expected = if looped { Some(loop_start) } else { None };
        if catalog_loop != expected {
            return Err("wave catalog loop differs from exact header".to_string());
        }
    }
    if looped && loop_start >= samples.len() as f64 {
        return Err("wave loop starts beyond sample data".to_string());
    }
    if let Some(header) = exact {
        if integer(&header.sample_count, "wave sample count")? != samples.len() as f64 {
            return Err("wave sample count differs from WAV data".to_string());
        }
    }
    let padding_size = size - 16.0 - samples.len() as f64;
    if padding_size < 0.0 {
        return Err("wave record size is shorter than its sample data".to_string());
    }
    let mut padding_fill = 0.0;
    match exact {
        None => {
            if padding_size > 3.0 {
                return Err("wave record size has invalid alignment".to_string());
            }
        }
        Some(_) => {
            let matches = match source.padding.as_ref() {
                None => false,
                Some(padding) => integer(&padding.size, "wave padding size")? == padding_size,
            };
            if !matches {
                return Err("wave padding size differs from record extent".to_string());
            }
            let padding = source.padding.as_ref().expect("checked above");
            padding_fill = byte(&padding.fill, "wave padding fill")?;
        }
    }
    if size > usize::MAX as f64 {
        return Err("wave record size is not allocatable".to_string());
    }
    let mut result = vec![padding_fill as u8; size as usize];
    write_u32_le(&mut result, 0, control);
    write_u32_le(&mut result, 4, frequency);
    write_u32_le(&mut result, 8, loop_start);
    write_u32_le(&mut result, 12, samples.len() as f64 - 1.0);
    result[16..16 + samples.len()].copy_from_slice(&samples);
    let report = WaveReport {
        samples: samples.len() as f64,
        rate,
        frequency,
        control,
        looped,
        loop_start: if looped { Some(loop_start) } else { None },
        padding_bytes: padding_size,
        padding_fill,
    };
    Ok((result, report))
}

/// `probeWaveRecord`: read a record back out of a ROM image slice.
pub fn probe_wave_record(data: &[u8], offset: f64, size: f64) -> Result<ProbedWaveRecord, String> {
    if !is_integer(offset)
        || !is_integer(size)
        || offset < 0.0
        || size < 16.0
        || offset + size > data.len() as f64
    {
        return Err("wave probe extent is invalid".to_string());
    }
    let start = offset as usize;
    let control = read_u32_le(data, start);
    let frequency = read_u32_le(data, start + 4);
    let loop_start = read_u32_le(data, start + 8);
    let sample_count = read_u32_le(data, start + 12) + 1.0;
    if frequency == 0.0 {
        return Err("wave frequency is zero".to_string());
    }
    if sample_count > size - 16.0 {
        return Err("wave samples extend beyond the record".to_string());
    }
    if ((control as u32) & 0xc000_0000) != 0 && loop_start >= sample_count {
        return Err("wave loop starts beyond sample data".to_string());
    }
    let count = sample_count as usize;
    let samples = data[start + 16..start + 16 + count].to_vec();
    let tail = &data[start + 16 + count..start + size as usize];
    let fill = tail.first().copied().unwrap_or(0);
    if tail.iter().any(|value| *value != fill) {
        return Err("wave padding is not uniform".to_string());
    }
    Ok(ProbedWaveRecord {
        header: ExactWaveHeaderSource {
            control: Scalar::Str(format!("0x{:08x}", control as u32)),
            frequency: Scalar::Num(frequency),
            loop_start: Scalar::Num(loop_start),
            sample_count: Scalar::Num(sample_count),
        },
        padding: ExactWavePaddingSource {
            size: Scalar::Num(tail.len() as f64),
            fill: Scalar::Num(f64::from(fill)),
        },
        samples,
    })
}

/// `Math.round`: half rounds toward +Infinity, unlike Rust's `f64::round`,
/// which rounds half away from zero. They differ only for negative halves
/// (`Math.round(-0.5) === -0`, `(-0.5f64).round() == -1`).
fn js_round(value: f64) -> f64 {
    if value.is_nan() || value.is_infinite() {
        return value;
    }
    let floor = value.floor();
    if value - floor >= 0.5 {
        floor + 1.0
    } else {
        floor
    }
}

/// Port of the TS `selfTest()`. Returns `Err` with the same message the TS
/// throws.
pub fn self_test() -> Result<(), String> {
    let samples = vec![0x80u8, 0xff, 0, 1, 0x7f];
    let wav = wav_from_signed_pcm(&samples, 22050.0)?;
    let (built, report) = build_wave_record(
        &WaveRecordSource {
            frequency: Scalar::Num(22050.0 * 1024.0),
            loop_start: Some(2.0),
            size: Scalar::Num(24.0),
            header: None,
            padding: None,
        },
        &wav,
    )?;
    if built.len() != 24
        || read_u32_le(&built, 0) != 1073741824.0
        || read_u32_le(&built, 12) != 4.0
        || built[16..21] != samples[..]
        || report.samples != 5.0
    {
        return Err("wave-record self-test failed".to_string());
    }
    let exact_samples = vec![0x80u8, 0xc0, 0, 0x40, 0x7f, 0x40, 0, 0xc0];
    let exact_wav = wav_from_signed_pcm(&exact_samples, 4096.0)?;
    let exact_source = WaveRecordSource {
        frequency: Scalar::Num(4096.0 * 1024.0),
        loop_start: Some(0.0),
        size: Scalar::Num(28.0),
        header: Some(ExactWaveHeaderSource {
            control: Scalar::str("0xc0000000"),
            frequency: Scalar::Num(4096.0 * 1024.0),
            loop_start: Scalar::Num(0.0),
            sample_count: Scalar::Num(exact_samples.len() as f64),
        }),
        padding: Some(ExactWavePaddingSource {
            size: Scalar::Num(4.0),
            fill: Scalar::Num(0x5a as f64),
        }),
    };
    let (exact_built, _) = build_wave_record(&exact_source, &exact_wav)?;
    let probed = probe_wave_record(&exact_built, 0.0, exact_built.len() as f64)?;
    if read_u32_le(&exact_built, 0) != 3221225472.0
        || exact_built[16..24] != exact_samples[..]
        || exact_built[24..] != [0x5au8; 4][..]
        || probed.header.control != Scalar::str("0xc0000000")
        || probed.header.sample_count != Scalar::Num(8.0)
        || probed.padding.size != Scalar::Num(4.0)
        || probed.padding.fill != Scalar::Num(0x5a as f64)
        || probed.samples != exact_samples
    {
        return Err("exact wave-record self-test failed".to_string());
    }
    println!("self-test=ok");
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn ported_self_test_passes() {
        self_test().expect("self-test must pass");
    }

    // --- JS Number() semantics ------------------------------------------

    #[test]
    fn js_number_parses_hex_octal_and_binary_prefixes() {
        assert_eq!(js_number_from_string("0xc0000000"), 3221225472.0);
        assert_eq!(js_number_from_string("0X10"), 16.0);
        assert_eq!(js_number_from_string("0o17"), 15.0);
        assert_eq!(js_number_from_string("0b1011"), 11.0);
        // A sign is not permitted with a radix prefix.
        assert!(js_number_from_string("-0x10").is_nan());
    }

    #[test]
    fn js_number_handles_whitespace_empty_and_infinity() {
        assert_eq!(js_number_from_string(""), 0.0);
        assert_eq!(js_number_from_string("  \t\n\r\u{feff}"), 0.0);
        assert_eq!(js_number_from_string("  42  "), 42.0);
        assert_eq!(js_number_from_string("Infinity"), f64::INFINITY);
        assert_eq!(js_number_from_string("-Infinity"), f64::NEG_INFINITY);
        // Rust's own f64 parser accepts these spellings; JavaScript does not.
        assert!(js_number_from_string("inf").is_nan());
        assert!(js_number_from_string("NaN").is_nan());
        assert!(js_number_from_string("nan").is_nan());
    }

    #[test]
    fn js_number_accepts_decimal_edge_spellings() {
        assert_eq!(js_number_from_string("1."), 1.0);
        assert_eq!(js_number_from_string(".5"), 0.5);
        assert_eq!(js_number_from_string("+1e3"), 1000.0);
        assert_eq!(js_number_from_string("1e-2"), 0.01);
        assert!(js_number_from_string(".").is_nan());
        assert!(js_number_from_string("1e").is_nan());
        assert!(js_number_from_string("1 2").is_nan());
        assert!(js_number_from_string("0x").is_nan());
    }

    #[test]
    fn manifest_style_hex_size_round_trips() {
        // Real manifest entries carry sizes like "0x000002ac".
        assert_eq!(js_number_from_string("0x000002ac"), 684.0);
        assert_eq!(
            integer(&Scalar::str("0x000002ac"), "wave record size"),
            Ok(684.0)
        );
    }

    // --- Guards ----------------------------------------------------------

    #[test]
    fn guards_reject_out_of_range_values() {
        assert_eq!(
            integer(&Scalar::Num(-1.0), "wave record size"),
            Err("wave record size must be a nonnegative integer".to_string())
        );
        assert_eq!(
            integer(&Scalar::Num(1.5), "wave record size"),
            Err("wave record size must be a nonnegative integer".to_string())
        );
        assert_eq!(
            integer(&Scalar::str("banana"), "wave record size"),
            Err("wave record size must be a nonnegative integer".to_string())
        );
        assert_eq!(
            integer(&Scalar::Num(f64::INFINITY), "wave record size"),
            Err("wave record size must be a nonnegative integer".to_string())
        );
        assert_eq!(uint32(&Scalar::Num(4294967295.0), "w"), Ok(4294967295.0));
        assert_eq!(
            uint32(&Scalar::Num(4294967296.0), "w"),
            Err("w must fit an unsigned word".to_string())
        );
        assert_eq!(byte(&Scalar::Num(255.0), "p"), Ok(255.0));
        assert_eq!(
            byte(&Scalar::Num(256.0), "p"),
            Err("p must fit an unsigned byte".to_string())
        );
    }

    // --- Rounding / truncation pins --------------------------------------

    #[test]
    fn js_round_matches_math_round_including_halves() {
        assert_eq!(js_round(0.5), 1.0);
        assert_eq!(js_round(1.5), 2.0);
        assert_eq!(js_round(2.5), 3.0);
        assert_eq!(js_round(-0.5), 0.0); // Math.round(-0.5) === -0, not -1
        assert_eq!(js_round(-1.5), -1.0); // f64::round(-1.5) would be -2
        assert_eq!(js_round(22579200.0 / 1024.0), 22050.0);
        // A frequency that is not a clean multiple of 1024 still rounds.
        assert_eq!(js_round(22579199.0 / 1024.0), 22050.0);
        assert_eq!(js_round(22578688.0 / 1024.0), 22050.0); // exactly .5
    }

    #[test]
    fn top_bit_control_survives_as_unsigned() {
        // 0xffffffff as i32 is -1; the looped test must still see the top bits.
        let value: f64 = 4294967295.0;
        assert!(((value as u32) & 0xc000_0000) != 0);
        // And `as u32` on the f64 must not saturate the way `as i32` would.
        assert_eq!(4294967295.0f64 as u32, 0xffff_ffff);
    }

    #[test]
    fn sample_transforms_wrap_like_js_mask() {
        for value in 0u16..=255 {
            let byte = value as u8;
            assert_eq!(byte.wrapping_add(128), ((value + 128) & 0xff) as u8);
            assert_eq!(
                byte.wrapping_sub(128),
                ((i32::from(value) - 128) & 0xff) as u8
            );
        }
    }

    // --- WAV container ----------------------------------------------------

    #[test]
    fn wav_round_trip_is_exact() {
        let samples: Vec<u8> = (0u16..=255).map(|value| value as u8).collect();
        let wav = wav_from_signed_pcm(&samples, 22050.0).unwrap();
        assert_eq!(wav.len(), 44 + 256);
        assert_eq!(&wav[0..4], b"RIFF");
        assert_eq!(read_u32_le(&wav, 4), (36 + 256) as f64);
        assert_eq!(read_u32_le(&wav, 24), 22050.0);
        assert_eq!(read_u32_le(&wav, 40), 256.0);
        assert_eq!(signed_pcm_from_wav(&wav, 22050.0).unwrap(), samples);
    }

    #[test]
    fn wav_rate_bounds_are_enforced() {
        assert_eq!(
            wav_from_signed_pcm(&[0], 0.0),
            Err("invalid WAV rate".to_string())
        );
        assert_eq!(
            wav_from_signed_pcm(&[0], 1.5),
            Err("invalid WAV rate".to_string())
        );
        assert_eq!(
            wav_from_signed_pcm(&[0], 4294967296.0),
            Err("invalid WAV rate".to_string())
        );
        assert!(wav_from_signed_pcm(&[0], 4294967295.0).is_ok());
    }

    #[test]
    fn wav_reader_rejects_malformed_containers() {
        assert_eq!(
            signed_pcm_from_wav(&[0u8; 43], 8000.0),
            Err("WAV is truncated".to_string())
        );
        let good = wav_from_signed_pcm(&[1, 2, 3], 8000.0).unwrap();
        let mut broken = good.clone();
        broken[0] = b'X';
        assert_eq!(
            signed_pcm_from_wav(&broken, 8000.0),
            Err("WAV is missing RIFF".to_string())
        );
        let mut broken = good.clone();
        broken[9] = b'X';
        assert_eq!(
            signed_pcm_from_wav(&broken, 8000.0),
            Err("WAV is missing WAVE".to_string())
        );
        let mut broken = good.clone();
        broken[34] = 16;
        assert_eq!(
            signed_pcm_from_wav(&broken, 8000.0),
            Err("WAV is not canonical mono 8-bit PCM".to_string())
        );
        assert_eq!(
            signed_pcm_from_wav(&good, 8001.0),
            Err("WAV is not canonical mono 8-bit PCM".to_string())
        );
    }

    #[test]
    fn four_cc_masks_the_high_bit_like_node_ascii() {
        // 'R' | 0x80 == 0xd2 decodes back to 'R' under Buffer ascii semantics.
        let mut wav = wav_from_signed_pcm(&[1], 8000.0).unwrap();
        wav[0] |= 0x80;
        wav[1] |= 0x80;
        wav[2] |= 0x80;
        wav[3] |= 0x80;
        assert!(signed_pcm_from_wav(&wav, 8000.0).is_ok());
    }

    // --- Record building --------------------------------------------------

    fn catalog(frequency: f64, loop_start: Option<f64>, size: f64) -> WaveRecordSource {
        WaveRecordSource {
            frequency: Scalar::Num(frequency),
            loop_start,
            size: Scalar::Num(size),
            header: None,
            padding: None,
        }
    }

    #[test]
    fn unlooped_catalog_record_has_zero_control() {
        let samples = vec![1u8, 2, 3, 4];
        let wav = wav_from_signed_pcm(&samples, 8000.0).unwrap();
        let (built, report) =
            build_wave_record(&catalog(8000.0 * 1024.0, None, 20.0), &wav).unwrap();
        assert_eq!(read_u32_le(&built, 0), 0.0);
        assert_eq!(read_u32_le(&built, 8), 0.0);
        assert_eq!(read_u32_le(&built, 12), 3.0);
        assert!(!report.looped);
        assert_eq!(report.loop_start, None);
        assert_eq!(report.padding_bytes, 0.0);
    }

    #[test]
    fn catalog_alignment_padding_is_capped_at_three() {
        let samples = vec![1u8, 2, 3, 4];
        let wav = wav_from_signed_pcm(&samples, 8000.0).unwrap();
        assert!(build_wave_record(&catalog(8000.0 * 1024.0, None, 23.0), &wav).is_ok());
        assert_eq!(
            build_wave_record(&catalog(8000.0 * 1024.0, None, 24.0), &wav),
            Err("wave record size has invalid alignment".to_string())
        );
        assert_eq!(
            build_wave_record(&catalog(8000.0 * 1024.0, None, 19.0), &wav),
            Err("wave record size is shorter than its sample data".to_string())
        );
    }

    #[test]
    fn loop_start_must_be_inside_the_samples() {
        let samples = vec![1u8, 2, 3, 4];
        let wav = wav_from_signed_pcm(&samples, 8000.0).unwrap();
        assert_eq!(
            build_wave_record(&catalog(8000.0 * 1024.0, Some(4.0), 20.0), &wav),
            Err("wave loop starts beyond sample data".to_string())
        );
        assert!(build_wave_record(&catalog(8000.0 * 1024.0, Some(3.0), 20.0), &wav).is_ok());
    }

    #[test]
    fn empty_sample_data_is_rejected() {
        let wav = wav_from_signed_pcm(&[], 8000.0).unwrap();
        assert_eq!(
            build_wave_record(&catalog(8000.0 * 1024.0, None, 16.0), &wav),
            Err("wave record has no samples".to_string())
        );
    }

    #[test]
    fn exact_header_disagreements_are_caught() {
        let samples = vec![1u8, 2, 3, 4];
        let wav = wav_from_signed_pcm(&samples, 8000.0).unwrap();
        let base = WaveRecordSource {
            frequency: Scalar::Num(8000.0 * 1024.0),
            loop_start: Some(1.0),
            size: Scalar::Num(24.0),
            header: Some(ExactWaveHeaderSource {
                control: Scalar::str("0x40000000"),
                frequency: Scalar::Num(8000.0 * 1024.0),
                loop_start: Scalar::Num(1.0),
                sample_count: Scalar::Num(4.0),
            }),
            padding: Some(ExactWavePaddingSource {
                size: Scalar::Num(4.0),
                fill: Scalar::Num(0.0),
            }),
        };
        assert!(build_wave_record(&base, &wav).is_ok());

        let mut wrong = base.clone();
        wrong.frequency = Scalar::Num(8000.0 * 1024.0 + 1.0);
        assert_eq!(
            build_wave_record(&wrong, &wav),
            Err("wave catalog frequency differs from exact header".to_string())
        );

        let mut wrong = base.clone();
        wrong.loop_start = Some(2.0);
        assert_eq!(
            build_wave_record(&wrong, &wav),
            Err("wave catalog loop differs from exact header".to_string())
        );

        // An unlooped exact header must be paired with a null catalog loop.
        let mut wrong = base.clone();
        wrong.header.as_mut().unwrap().control = Scalar::Num(0.0);
        assert_eq!(
            build_wave_record(&wrong, &wav),
            Err("wave catalog loop differs from exact header".to_string())
        );
        wrong.loop_start = None;
        assert!(build_wave_record(&wrong, &wav).is_ok());

        let mut wrong = base.clone();
        wrong.header.as_mut().unwrap().sample_count = Scalar::Num(5.0);
        assert_eq!(
            build_wave_record(&wrong, &wav),
            Err("wave sample count differs from WAV data".to_string())
        );

        let mut wrong = base.clone();
        wrong.padding = None;
        assert_eq!(
            build_wave_record(&wrong, &wav),
            Err("wave padding size differs from record extent".to_string())
        );

        let mut wrong = base.clone();
        wrong.padding.as_mut().unwrap().size = Scalar::Num(3.0);
        assert_eq!(
            build_wave_record(&wrong, &wav),
            Err("wave padding size differs from record extent".to_string())
        );

        let mut wrong = base;
        wrong.padding.as_mut().unwrap().fill = Scalar::Num(256.0);
        assert_eq!(
            build_wave_record(&wrong, &wav),
            Err("wave padding fill must fit an unsigned byte".to_string())
        );
    }

    #[test]
    fn full_width_control_word_is_written_unsigned() {
        let samples = vec![9u8; 4];
        let wav = wav_from_signed_pcm(&samples, 8000.0).unwrap();
        let source = WaveRecordSource {
            frequency: Scalar::str("0x7d0000"),
            loop_start: Some(0.0),
            size: Scalar::Num(20.0),
            header: Some(ExactWaveHeaderSource {
                control: Scalar::str("0xffffffff"),
                frequency: Scalar::str("0x7d0000"),
                loop_start: Scalar::Num(0.0),
                sample_count: Scalar::Num(4.0),
            }),
            padding: Some(ExactWavePaddingSource {
                size: Scalar::Num(0.0),
                fill: Scalar::Num(0.0),
            }),
        };
        let (built, report) = build_wave_record(&source, &wav).unwrap();
        assert_eq!(&built[0..4], &[0xff, 0xff, 0xff, 0xff]);
        assert_eq!(report.control, 4294967295.0);
        assert!(report.looped);
    }

    // --- Probing ----------------------------------------------------------

    #[test]
    fn probe_rejects_invalid_extents() {
        let data = vec![0u8; 32];
        assert_eq!(
            probe_wave_record(&data, 0.0, 15.0),
            Err("wave probe extent is invalid".to_string())
        );
        assert_eq!(
            probe_wave_record(&data, -1.0, 16.0),
            Err("wave probe extent is invalid".to_string())
        );
        assert_eq!(
            probe_wave_record(&data, 0.5, 16.0),
            Err("wave probe extent is invalid".to_string())
        );
        assert_eq!(
            probe_wave_record(&data, 24.0, 16.0),
            Err("wave probe extent is invalid".to_string())
        );
        assert_eq!(
            probe_wave_record(&data, 0.0, 16.0),
            Err("wave frequency is zero".to_string())
        );
    }

    #[test]
    fn probe_detects_nonuniform_padding_and_overrun() {
        let samples = vec![7u8, 8, 9, 10];
        let wav = wav_from_signed_pcm(&samples, 8000.0).unwrap();
        let (mut built, _) = build_wave_record(&catalog(8000.0 * 1024.0, None, 23.0), &wav).unwrap();
        assert!(probe_wave_record(&built, 0.0, 23.0).is_ok());
        built[22] = 1;
        assert_eq!(
            probe_wave_record(&built, 0.0, 23.0),
            Err("wave padding is not uniform".to_string())
        );
        // Claim more samples than the record can hold.
        let mut overrun = built.clone();
        write_u32_le(&mut overrun, 12, 99.0);
        assert_eq!(
            probe_wave_record(&overrun, 0.0, 23.0),
            Err("wave samples extend beyond the record".to_string())
        );
        // sample_count is stored biased by one, so 0xffffffff means 2^32.
        write_u32_le(&mut overrun, 12, 4294967295.0);
        assert_eq!(
            probe_wave_record(&overrun, 0.0, 23.0),
            Err("wave samples extend beyond the record".to_string())
        );
    }

    #[test]
    fn probe_control_is_lowercase_zero_padded_hex() {
        let samples = vec![1u8, 2];
        let wav = wav_from_signed_pcm(&samples, 8000.0).unwrap();
        let (built, _) = build_wave_record(&catalog(8000.0 * 1024.0, Some(1.0), 18.0), &wav).unwrap();
        let probed = probe_wave_record(&built, 0.0, 18.0).unwrap();
        assert_eq!(probed.header.control, Scalar::str("0x40000000"));
        assert_eq!(probed.padding.size, Scalar::Num(0.0));
        assert_eq!(probed.padding.fill, Scalar::Num(0.0));
        assert_eq!(probed.samples, samples);
    }

    #[test]
    fn probe_at_a_nonzero_offset_reads_the_right_slice() {
        let samples = vec![0xaau8, 0xbb, 0xcc, 0xdd];
        let wav = wav_from_signed_pcm(&samples, 8000.0).unwrap();
        let (built, _) = build_wave_record(&catalog(8000.0 * 1024.0, None, 20.0), &wav).unwrap();
        let mut image = vec![0x11u8; 7];
        image.extend_from_slice(&built);
        image.extend_from_slice(&[0x22u8; 5]);
        let probed = probe_wave_record(&image, 7.0, 20.0).unwrap();
        assert_eq!(probed.samples, samples);
        assert_eq!(probed.header.frequency, Scalar::Num(8000.0 * 1024.0));
    }
}
