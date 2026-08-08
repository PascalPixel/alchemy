// Text form for a 16-bit tilemap: one row per line, one four-digit
// little-endian hex word per entry.
//
// Ported from tools/lib/tilemap.ts.

#[derive(Debug, PartialEq, Eq)]
pub enum TilemapError {
    NotWholeEntries,
    WidthDoesNotDivide { count: usize, width: usize },
    EmptyText,
    NotAHexWord(String),
}

impl std::fmt::Display for TilemapError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::NotWholeEntries => write!(f, "tilemap must contain whole 16-bit entries"),
            Self::WidthDoesNotDivide { count, width } => {
                write!(
                    f,
                    "width {width} must divide the tilemap entry count {count}"
                )
            }
            Self::EmptyText => write!(f, "tilemap text must contain four-digit hex words"),
            Self::NotAHexWord(token) => {
                write!(f, "tilemap token {token:?} is not a four-digit hex word")
            }
        }
    }
}

impl std::error::Error for TilemapError {}

pub fn export_tilemap(data: &[u8], width: usize) -> Result<String, TilemapError> {
    if width == 0 {
        return Err(TilemapError::WidthDoesNotDivide {
            count: data.len() / 2,
            width,
        });
    }
    if !data.len().is_multiple_of(2) {
        return Err(TilemapError::NotWholeEntries);
    }
    let count = data.len() / 2;
    if count == 0 || !count.is_multiple_of(width) {
        return Err(TilemapError::WidthDoesNotDivide { count, width });
    }
    let mut result = String::new();
    for start in (0..count).step_by(width) {
        for index in start..start + width {
            if index > start {
                result.push(' ');
            }
            let word = u16::from_le_bytes([data[index * 2], data[index * 2 + 1]]);
            result.push_str(&format!("{word:04x}"));
        }
        result.push('\n');
    }
    Ok(result)
}

fn is_hex_word(token: &str) -> bool {
    token.len() == 4 && token.chars().all(|c| c.is_ascii_hexdigit())
}

pub fn import_tilemap(text: &str) -> Result<Vec<u8>, TilemapError> {
    let tokens: Vec<&str> = text.split_whitespace().collect();
    if tokens.is_empty() {
        return Err(TilemapError::EmptyText);
    }
    let mut result = Vec::with_capacity(tokens.len() * 2);
    for token in tokens {
        if !is_hex_word(token) {
            return Err(TilemapError::NotAHexWord(token.to_string()));
        }
        let word = u16::from_str_radix(token, 16).expect("four hex digits always parse");
        result.extend_from_slice(&word.to_le_bytes());
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn sample() -> Vec<u8> {
        (0..64u16)
            .flat_map(|index| (index.wrapping_mul(257)).to_le_bytes())
            .collect()
    }

    #[test]
    fn round_trips() {
        let raw = sample();
        let text = export_tilemap(&raw, 8).unwrap();
        assert_eq!(import_tilemap(&text).unwrap(), raw);
    }

    #[test]
    fn rows_are_width_wide() {
        let text = export_tilemap(&sample(), 8).unwrap();
        assert_eq!(text.lines().count(), 8);
        assert!(text.lines().all(|line| line.split(' ').count() == 8));
    }

    #[test]
    fn bad_inputs_are_rejected() {
        assert_eq!(
            export_tilemap(&[0u8; 3], 1),
            Err(TilemapError::NotWholeEntries)
        );
        assert!(export_tilemap(&[0u8; 8], 3).is_err());
        assert!(export_tilemap(&[], 1).is_err());
        assert!(import_tilemap("   ").is_err());
        assert!(import_tilemap("abc").is_err());
        assert!(import_tilemap("gggg").is_err());
    }
}
