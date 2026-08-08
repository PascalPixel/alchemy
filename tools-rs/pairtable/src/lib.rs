// A table of u16 pairs as text, two `0xNNNN` values per line.
//
// Ported from tools/make/pairtable.ts. The literal grammar and comment
// stripping come from the wordstream crate; only the two-per-line shape and
// the nonempty requirement differ.

use wordstream::{parse_literal, strip_comment};

#[derive(Debug, PartialEq, Eq)]
pub enum PairError {
    NotWholePairs,
    ExpectedTwoValues { line: usize },
    InvalidValue { line: usize },
    OutOfRange { line: usize },
    Empty,
}

impl std::fmt::Display for PairError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::NotWholePairs => write!(f, "pair table must contain whole nonempty u16 pairs"),
            Self::ExpectedTwoValues { line } => {
                write!(f, "line {line}: expected exactly two values")
            }
            Self::InvalidValue { line } => write!(f, "line {line}: invalid pair value"),
            Self::OutOfRange { line } => write!(f, "line {line}: pair value is out of range"),
            Self::Empty => write!(f, "pair table is empty"),
        }
    }
}

impl std::error::Error for PairError {}

pub fn export_pairs(data: &[u8]) -> Result<String, PairError> {
    if data.is_empty() || !data.len().is_multiple_of(4) {
        return Err(PairError::NotWholePairs);
    }
    let mut result = String::new();
    for chunk in data.chunks_exact(4) {
        let first = u16::from_le_bytes([chunk[0], chunk[1]]);
        let second = u16::from_le_bytes([chunk[2], chunk[3]]);
        result.push_str(&format!("0x{first:04x} 0x{second:04x}\n"));
    }
    Ok(result)
}

pub fn import_pairs(text: &str) -> Result<Vec<u8>, PairError> {
    let mut result = Vec::new();
    for (index, line) in text.lines().enumerate() {
        let tokens: Vec<&str> = strip_comment(line).split_whitespace().collect();
        if tokens.is_empty() {
            continue;
        }
        if tokens.len() != 2 {
            return Err(PairError::ExpectedTwoValues { line: index + 1 });
        }
        for token in tokens {
            let value = parse_literal(token).ok_or(PairError::InvalidValue { line: index + 1 })?;
            if !(0..=0xffff).contains(&value) {
                return Err(PairError::OutOfRange { line: index + 1 });
            }
            result.extend_from_slice(&(value as u16).to_le_bytes());
        }
    }
    if result.is_empty() {
        return Err(PairError::Empty);
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn sample() -> Vec<u8> {
        [0u16, 1, 0x1234, 0xabcd, 0xffff, 2].iter().flat_map(|w| w.to_le_bytes()).collect()
    }

    #[test]
    fn round_trips() {
        let raw = sample();
        assert_eq!(import_pairs(&export_pairs(&raw).unwrap()).unwrap(), raw);
    }

    #[test]
    fn one_line_per_pair() {
        assert_eq!(export_pairs(&sample()).unwrap().lines().count(), 3);
    }

    #[test]
    fn bad_input_is_rejected_with_its_line() {
        assert_eq!(export_pairs(&[]), Err(PairError::NotWholePairs));
        assert_eq!(export_pairs(&[0; 6]), Err(PairError::NotWholePairs));
        assert_eq!(import_pairs("0x1\n"), Err(PairError::ExpectedTwoValues { line: 1 }));
        assert_eq!(import_pairs("0x1 0x2 0x3\n"), Err(PairError::ExpectedTwoValues { line: 1 }));
        assert_eq!(import_pairs("0x1 nope\n"), Err(PairError::InvalidValue { line: 1 }));
        assert_eq!(import_pairs("0x1 0x10000\n"), Err(PairError::OutOfRange { line: 1 }));
        assert_eq!(import_pairs("# only a comment\n"), Err(PairError::Empty));
    }
}
