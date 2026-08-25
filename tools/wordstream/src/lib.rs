pub mod cli;

// A stream of little-endian u16 words as text, one `0xNNNN` per line.
//
// Ported from tools/make/wordstream.ts. The numeric-literal grammar is shared
// with the pair table, so it lives here and pairtable depends on it rather
// than growing a second copy that could drift.

#[derive(Debug, PartialEq, Eq)]
pub enum WordError {
    OddByteCount,
    InvalidWord { line: usize },
    OutOfRange { line: usize },
}

impl std::fmt::Display for WordError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::OddByteCount => write!(f, "word stream has an odd byte count"),
            Self::InvalidWord { line } => write!(f, "line {line}: invalid word"),
            Self::OutOfRange { line } => write!(f, "line {line}: word is out of range"),
        }
    }
}

impl std::error::Error for WordError {}

/// Accepts the same literals as the TypeScript: an optional sign, then
/// decimal, `0x`, `0b`, or `0o`. A negative value parses here and is rejected
/// by the range check, exactly as it was before.
pub fn parse_literal(token: &str) -> Option<i64> {
    let (negative, rest) = match token.strip_prefix('-') {
        Some(rest) => (true, rest),
        None => (false, token.strip_prefix('+').unwrap_or(token)),
    };
    if rest.is_empty() {
        return None;
    }
    let (radix, digits) = if let Some(d) = rest.strip_prefix("0x").or_else(|| rest.strip_prefix("0X")) {
        (16, d)
    } else if let Some(d) = rest.strip_prefix("0b").or_else(|| rest.strip_prefix("0B")) {
        (2, d)
    } else if let Some(d) = rest.strip_prefix("0o").or_else(|| rest.strip_prefix("0O")) {
        (8, d)
    } else {
        (10, rest)
    };
    if digits.is_empty() {
        return None;
    }
    let value = i64::from_str_radix(digits, radix).ok()?;
    Some(if negative { -value } else { value })
}

/// Strips a `#` comment and surrounding whitespace, mirroring the TypeScript's
/// `split("#", 1)[0].trim()`.
pub fn strip_comment(line: &str) -> &str {
    line.split('#').next().unwrap_or("").trim()
}

pub fn export_words(data: &[u8]) -> Result<String, WordError> {
    if !data.len().is_multiple_of(2) {
        return Err(WordError::OddByteCount);
    }
    let mut result = String::new();
    for chunk in data.chunks_exact(2) {
        result.push_str(&format!("0x{:04x}\n", u16::from_le_bytes([chunk[0], chunk[1]])));
    }
    Ok(result)
}

pub fn import_words(text: &str) -> Result<Vec<u8>, WordError> {
    let mut result = Vec::new();
    for (index, line) in text.lines().enumerate() {
        let value = strip_comment(line);
        if value.is_empty() {
            continue;
        }
        let word = parse_literal(value).ok_or(WordError::InvalidWord { line: index + 1 })?;
        if !(0..=0xffff).contains(&word) {
            return Err(WordError::OutOfRange { line: index + 1 });
        }
        result.extend_from_slice(&(word as u16).to_le_bytes());
    }
    Ok(result)
}

/// Exercise the binary's actual word-stream format in both directions and
/// retain a negative case so a parser that accepts everything cannot pass.
pub fn self_test() -> Result<String, String> {
    let raw: Vec<u8> = [0u16, 1, 0x1234, 0xabcd, 0xffff].iter().flat_map(|word| word.to_le_bytes()).collect();
    let text = export_words(&raw).map_err(|error| error.to_string())?;
    let rebuilt = import_words(&text).map_err(|error| error.to_string())?;
    if rebuilt != raw {
        return Err("wordstream self-test round trip changed".into());
    }
    if import_words("0x10000\n").is_ok() || import_words("0x1\ninvalid\n").is_ok() {
        return Err("wordstream self-test accepted invalid input".into());
    }
    Ok(format!("self-test=ok words={}", raw.len() / 2))
}
