pub mod cli;

use std::collections::{HashMap, HashSet};

pub type Error = String;
pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct SoundTableEntry {
    pub header: String,
    pub player: i64,
}

#[derive(Debug, Clone, PartialEq)]
pub struct SoundTableSource {
    pub format: i64,
    pub fields: Vec<String>,
    pub auxiliary: String,
    pub symbols: Vec<(String, SymbolValue)>,
    pub entries: Vec<SoundTableEntry>,
}

#[derive(Debug, Clone, PartialEq)]
pub enum SymbolValue {
    Text(String),
    Number(f64),
}

#[derive(Debug, Clone, PartialEq)]
pub struct SoundTableReport {
    pub entries: usize,
    pub unique_headers: usize,
    pub players: Vec<(String, u64)>,
    pub mirrored_auxiliary: bool,
}

pub fn js_number(text: &str) -> f64 {
    let t = text.trim();
    if t.is_empty() {
        return 0.0;
    }
    let (sign, body) = match t.strip_prefix('-') {
        Some(rest) => (-1.0, rest),
        None => (1.0, t.strip_prefix('+').unwrap_or(t)),
    };
    let radix_parse = |digits: &str, radix: u32| -> f64 {
        if digits.is_empty() || !digits.chars().all(|c| c.is_digit(radix)) {
            return f64::NAN;
        }
        let mut acc = 0.0f64;
        for c in digits.chars() {
            acc = acc * f64::from(radix) + f64::from(c.to_digit(radix).unwrap());
        }
        acc
    };
    if let Some(d) = body.strip_prefix("0x").or_else(|| body.strip_prefix("0X")) {
        return if sign < 0.0 {
            f64::NAN
        } else {
            radix_parse(d, 16)
        };
    }
    if let Some(d) = body.strip_prefix("0o").or_else(|| body.strip_prefix("0O")) {
        return if sign < 0.0 {
            f64::NAN
        } else {
            radix_parse(d, 8)
        };
    }
    if let Some(d) = body.strip_prefix("0b").or_else(|| body.strip_prefix("0B")) {
        return if sign < 0.0 {
            f64::NAN
        } else {
            radix_parse(d, 2)
        };
    }
    if body == "Infinity" {
        return sign * f64::INFINITY;
    }
    match body.parse::<f64>() {
        Ok(v)
            if !body.eq_ignore_ascii_case("inf")
                && !body.eq_ignore_ascii_case("infinity")
                && !body.eq_ignore_ascii_case("nan") =>
        {
            sign * v
        }
        _ => f64::NAN,
    }
}

fn integer_from_str(value: &str, label: &str) -> Result<i64> {
    integer_from_f64(js_number(value), label)
}

fn integer_from_f64(result: f64, label: &str) -> Result<i64> {
    if !result.is_finite() || result != result.trunc() {
        return Err(format!("{label} must be an integer"));
    }
    Ok(result as i64)
}

fn u16_check(value: i64, label: &str) -> Result<u16> {
    if !(0..=0xffff).contains(&value) {
        return Err(format!("{label} does not fit u16"));
    }
    Ok(value as u16)
}

fn address_from_i64(result: i64, label: &str) -> Result<u32> {
    if !(0x0800_0000..0x0a00_0000).contains(&result) || result % 4 != 0 {
        return Err(format!("{label} is not an aligned ROM address"));
    }
    Ok(result as u32)
}

fn address(value: &SymbolValue, label: &str) -> Result<u32> {
    let result = match value {
        SymbolValue::Text(s) => integer_from_str(s, label)?,
        SymbolValue::Number(n) => integer_from_f64(*n, label)?,
    };
    address_from_i64(result, label)
}

fn valid_symbol(name: &str) -> bool {
    let mut chars = name.chars();
    match chars.next() {
        Some(c) if c.is_ascii_lowercase() => {}
        _ => return false,
    }
    chars.all(|c| c.is_ascii_lowercase() || c.is_ascii_digit() || c == '_')
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct SequenceBuildReport {
    pub base: u32,
    pub end: u32,
    pub bytes: usize,
    pub streams: usize,
    pub tracks: usize,
    pub events: usize,
}

pub fn build_reserve_sequence(base: u32) -> (Vec<u8>, SequenceBuildReport) {
    let data = vec![
        0xbe, 120, 0xbc, 0, 0xbb, 30, 0xbd, 21, 0xd0, 61, 127, 0x81, 66, 0x81, 0xee, 69, 0xa0, 0xb1,
    ];
    let report = SequenceBuildReport {
        base,
        end: base + data.len() as u32,
        bytes: data.len(),
        streams: 1,
        tracks: 0,
        events: 11,
    };
    (data, report)
}

pub fn build_sound_table(source: &SoundTableSource) -> Result<(Vec<u8>, SoundTableReport)> {
    if source.format != 1
        || source.fields != ["header", "player"]
        || source.auxiliary != "copy-player"
    {
        return Err("unsupported sound-table source".to_string());
    }
    let mut symbols: HashMap<&str, u32> = HashMap::new();
    let mut order: Vec<&str> = Vec::new();
    let mut addresses: HashSet<u32> = HashSet::new();
    for (name, value) in &source.symbols {
        if !valid_symbol(name) {
            return Err(format!("invalid sound symbol: {name}"));
        }
        let resolved = address(value, &format!("symbol {name}"))?;
        if addresses.contains(&resolved) {
            return Err(format!("duplicate sound-header address: {name}"));
        }
        if symbols.insert(name.as_str(), resolved).is_none() {
            order.push(name.as_str());
        }
        addresses.insert(resolved);
    }
    let mut used: HashSet<&str> = HashSet::new();
    let mut players: Vec<(String, u64)> = Vec::new();
    let mut built = vec![0u8; source.entries.len() * 8];
    for (index, entry) in source.entries.iter().enumerate() {
        let Some(&header) = symbols.get(entry.header.as_str()) else {
            return Err(format!("entry {index} references an unknown sound header"));
        };
        let label = format!("entry {index} player");
        let player = u16_check(integer_from_f64(entry.player as f64, &label)?, &label)?;
        let base = index * 8;
        built[base..base + 4].copy_from_slice(&header.to_le_bytes());
        built[base + 4..base + 6].copy_from_slice(&player.to_le_bytes());
        built[base + 6..base + 8].copy_from_slice(&player.to_le_bytes());
        used.insert(entry.header.as_str());
        let key = player.to_string();
        match players.iter_mut().find(|(k, _)| *k == key) {
            Some((_, count)) => *count += 1,
            None => players.push((key, 1)),
        }
    }
    let unused: Vec<&str> = order
        .iter()
        .copied()
        .filter(|name| !used.contains(name))
        .collect();
    if !unused.is_empty() {
        return Err(format!("unused sound symbols: {}", unused.join(", ")));
    }
    let report = SoundTableReport {
        entries: source.entries.len(),
        unique_headers: symbols.len(),
        players,
        mirrored_auxiliary: true,
    };
    Ok((built, report))
}
