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
    let (radix, digits) =
        if let Some(d) = rest.strip_prefix("0x").or_else(|| rest.strip_prefix("0X")) {
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
        result.push_str(&format!(
            "0x{:04x}\n",
            u16::from_le_bytes([chunk[0], chunk[1]])
        ));
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

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn round_trips() {
        let raw: Vec<u8> = [0u16, 1, 0x1234, 0xabcd, 0xffff]
            .iter()
            .flat_map(|w| w.to_le_bytes())
            .collect();
        assert_eq!(import_words(&export_words(&raw).unwrap()).unwrap(), raw);
    }

    #[test]
    fn comments_and_blank_lines_are_skipped() {
        assert_eq!(
            import_words("# header\n\n0x0001 # one\n").unwrap(),
            vec![1, 0]
        );
    }

    #[test]
    fn every_radix_parses() {
        assert_eq!(parse_literal("0x10"), Some(16));
        assert_eq!(parse_literal("0b101"), Some(5));
        assert_eq!(parse_literal("0o17"), Some(15));
        assert_eq!(parse_literal("+42"), Some(42));
        assert_eq!(parse_literal("-1"), Some(-1));
        assert_eq!(parse_literal("0x"), None);
        assert_eq!(parse_literal("zz"), None);
        assert_eq!(parse_literal(""), None);
    }

    #[test]
    fn bad_input_is_rejected_with_its_line() {
        assert_eq!(export_words(&[0, 1, 2]), Err(WordError::OddByteCount));
        assert_eq!(
            import_words("0x1\nnope\n"),
            Err(WordError::InvalidWord { line: 2 })
        );
        assert_eq!(
            import_words("0x10000\n"),
            Err(WordError::OutOfRange { line: 1 })
        );
        assert_eq!(import_words("-1\n"), Err(WordError::OutOfRange { line: 1 }));
    }
}
