pub mod cli;

#[derive(Debug, Clone, PartialEq)]
pub enum Scalar {
    Num(f64),
    Str(String),
}

impl Scalar {
    pub fn string(value: &str) -> Self {
        Self::Str(value.into())
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
    pub loop_start: Option<f64>,
    pub size: Scalar,
    pub header: Option<ExactWaveHeaderSource>,
    pub padding: Option<ExactWavePaddingSource>,
}

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

fn js_number(value: &str) -> f64 {
    let value = value.trim_matches(|c: char| c.is_ascii_whitespace() || c == '\u{feff}');
    if value.is_empty() {
        return 0.0;
    }
    if let Some(digits) = value.strip_prefix("0x").or_else(|| value.strip_prefix("0X")) {
        return u128::from_str_radix(digits, 16).map_or(f64::NAN, |n| n as f64);
    }
    if let Some(digits) = value.strip_prefix("0o").or_else(|| value.strip_prefix("0O")) {
        return u128::from_str_radix(digits, 8).map_or(f64::NAN, |n| n as f64);
    }
    if let Some(digits) = value.strip_prefix("0b").or_else(|| value.strip_prefix("0B")) {
        return u128::from_str_radix(digits, 2).map_or(f64::NAN, |n| n as f64);
    }
    match value {
        "Infinity" | "+Infinity" => f64::INFINITY,
        "-Infinity" => f64::NEG_INFINITY,
        _ => value.parse().unwrap_or(f64::NAN),
    }
}

fn number(value: &Scalar) -> f64 {
    match value {
        Scalar::Num(value) => *value,
        Scalar::Str(value) => js_number(value),
    }
}
fn integer(value: &Scalar, label: &str) -> Result<f64, String> {
    let value = number(value);
    if value.is_finite() && value.fract() == 0.0 && value >= 0.0 {
        Ok(value)
    } else {
        Err(format!("{label} must be a nonnegative integer"))
    }
}
fn word(value: &Scalar, label: &str) -> Result<f64, String> {
    let value = integer(value, label)?;
    if value <= 4_294_967_295.0 {
        Ok(value)
    } else {
        Err(format!("{label} must fit an unsigned word"))
    }
}
fn byte(value: &Scalar, label: &str) -> Result<f64, String> {
    let value = integer(value, label)?;
    if value <= 255.0 {
        Ok(value)
    } else {
        Err(format!("{label} must fit an unsigned byte"))
    }
}

fn chunk(data: &[u8], at: usize, name: &str) -> Result<(), String> {
    let end = at.saturating_add(4).min(data.len());
    let text: String = data.get(at..end).unwrap_or_default().iter().map(|byte| char::from(byte & 0x7f)).collect();
    if text == name {
        Ok(())
    } else {
        Err(format!("WAV is missing {name}"))
    }
}
fn u16_at(data: &[u8], at: usize) -> f64 {
    f64::from(u16::from_le_bytes([data[at], data[at + 1]]))
}
fn u32_at(data: &[u8], at: usize) -> f64 {
    f64::from(u32::from_le_bytes([data[at], data[at + 1], data[at + 2], data[at + 3]]))
}
fn put_u32(data: &mut [u8], at: usize, value: f64) {
    data[at..at + 4].copy_from_slice(&(value as u32).to_le_bytes());
}

fn pcm_from_wav(data: &[u8], rate: f64) -> Result<Vec<u8>, String> {
    if data.len() < 44 {
        return Err("WAV is truncated".into());
    }
    chunk(data, 0, "RIFF")?;
    chunk(data, 8, "WAVE")?;
    chunk(data, 12, "fmt ")?;
    chunk(data, 36, "data")?;
    let length = data.len() as f64;
    if u32_at(data, 4) != length - 8.0 || u32_at(data, 16) != 16.0 || u16_at(data, 20) != 1.0 || u16_at(data, 22) != 1.0 || u32_at(data, 24) != rate || u32_at(data, 28) != rate || u16_at(data, 32) != 1.0 || u16_at(data, 34) != 8.0 || u32_at(data, 40) != length - 44.0 {
        return Err("WAV is not canonical mono 8-bit PCM".into());
    }
    Ok(data[44..].iter().map(|byte| byte.wrapping_sub(128)).collect())
}

pub fn build_wave_record(source: &WaveRecordSource, wav: &[u8]) -> Result<(Vec<u8>, WaveReport), String> {
    let exact = source.header.as_ref();
    let frequency = match exact {
        Some(header) => word(&header.frequency, "wave frequency")?,
        None => word(&source.frequency, "wave frequency")?,
    };
    if exact.is_some() && word(&source.frequency, "wave catalog frequency")? != frequency {
        return Err("wave catalog frequency differs from exact header".into());
    }
    let size = integer(&source.size, "wave record size")?;
    let rate = (frequency / 1024.0).floor() + if frequency / 1024.0 - (frequency / 1024.0).floor() >= 0.5 { 1.0 } else { 0.0 };
    let samples = pcm_from_wav(wav, rate)?;
    if samples.is_empty() {
        return Err("wave record has no samples".into());
    }
    let control = match exact {
        Some(header) => word(&header.control, "wave control")?,
        None => {
            if source.loop_start.is_some() {
                1_073_741_824.0
            } else {
                0.0
            }
        }
    };
    let loop_start = match exact {
        Some(header) => word(&header.loop_start, "wave loop start")?,
        None => source.loop_start.map_or(Ok(0.0), |value| word(&Scalar::Num(value), "wave loop start"))?,
    };
    let looped = (control as u32 & 0xc000_0000) != 0;
    let catalog_loop = source.loop_start.map(|value| word(&Scalar::Num(value), "wave catalog loop start")).transpose()?;
    if exact.is_some() && catalog_loop != if looped { Some(loop_start) } else { None } {
        return Err("wave catalog loop differs from exact header".into());
    }
    if looped && loop_start >= samples.len() as f64 {
        return Err("wave loop starts beyond sample data".into());
    }
    if let Some(header) = exact {
        if integer(&header.sample_count, "wave sample count")? != samples.len() as f64 {
            return Err("wave sample count differs from WAV data".into());
        }
    }
    let padding_size = size - 16.0 - samples.len() as f64;
    if padding_size < 0.0 {
        return Err("wave record size is shorter than its sample data".into());
    }
    let padding_fill = match exact {
        None if padding_size <= 3.0 => 0.0,
        None => return Err("wave record size has invalid alignment".into()),
        Some(_) => {
            let padding = source.padding.as_ref().ok_or("wave padding size differs from record extent")?;
            if integer(&padding.size, "wave padding size")? != padding_size {
                return Err("wave padding size differs from record extent".into());
            }
            byte(&padding.fill, "wave padding fill")?
        }
    };
    if size > usize::MAX as f64 {
        return Err("wave record size is not allocatable".into());
    }
    let mut result = vec![padding_fill as u8; size as usize];
    put_u32(&mut result, 0, control);
    put_u32(&mut result, 4, frequency);
    put_u32(&mut result, 8, loop_start);
    put_u32(&mut result, 12, samples.len() as f64 - 1.0);
    result[16..16 + samples.len()].copy_from_slice(&samples);
    Ok((result, WaveReport { samples: samples.len() as f64, rate, frequency, control, looped, loop_start: looped.then_some(loop_start), padding_bytes: padding_size, padding_fill }))
}
