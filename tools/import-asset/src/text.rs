//! Text representations of little-endian 16-bit data share one checked parser.
#[derive(Clone, Copy)]
enum Form {
    Words,
    Pairs,
    Tilemap,
}

fn literal(token: &str) -> Option<i64> {
    let (negative, token) = match token.strip_prefix('-') {
        Some(rest) => (true, rest),
        None => (false, token.strip_prefix('+').unwrap_or(token)),
    };
    let (base, digits) = if let Some(rest) = token
        .strip_prefix("0x")
        .or_else(|| token.strip_prefix("0X"))
    {
        (16, rest)
    } else if let Some(rest) = token
        .strip_prefix("0b")
        .or_else(|| token.strip_prefix("0B"))
    {
        (2, rest)
    } else if let Some(rest) = token
        .strip_prefix("0o")
        .or_else(|| token.strip_prefix("0O"))
    {
        (8, rest)
    } else {
        (10, token)
    };
    let value = i64::from_str_radix(digits, base).ok()?;
    Some(if negative { -value } else { value })
}

fn encode(text: &str, form: Form) -> Result<Vec<u8>, String> {
    let mut output = Vec::new();
    for (line, source) in text.lines().enumerate() {
        let source = if matches!(form, Form::Tilemap) {
            source
        } else {
            source.split('#').next().unwrap_or("")
        };
        let tokens: Vec<_> = source.split_whitespace().collect();
        if tokens.is_empty() {
            continue;
        }
        let columns = match form {
            Form::Words => 1,
            Form::Pairs => 2,
            Form::Tilemap => tokens.len(),
        };
        if tokens.len() != columns {
            return Err(format!("line {}: expected {columns} values", line + 1));
        }
        for token in tokens {
            let value = if matches!(form, Form::Tilemap) {
                if token.len() != 4 || !token.bytes().all(|b| b.is_ascii_hexdigit()) {
                    return Err(format!("line {}: expected four-digit hex word", line + 1));
                }
                i64::from_str_radix(token, 16).ok()
            } else {
                literal(token)
            };
            let value = value
                .and_then(|n| u16::try_from(n).ok())
                .ok_or_else(|| format!("line {}: invalid u16 value", line + 1))?;
            output.extend(value.to_le_bytes());
        }
    }
    if output.is_empty() && !matches!(form, Form::Words) {
        return Err("empty table".into());
    }
    Ok(output)
}

pub fn import_words(text: &str) -> Result<Vec<u8>, String> {
    encode(text, Form::Words)
}
pub fn import_pairs(text: &str) -> Result<Vec<u8>, String> {
    encode(text, Form::Pairs)
}
pub fn import_tilemap(text: &str) -> Result<Vec<u8>, String> {
    encode(text, Form::Tilemap)
}

#[test]
fn text_formats_keep_their_grammars_and_limits() {
    assert_eq!(
        import_words("# words\n+0x1234\n0b11\n0o10\n65535\n").unwrap(),
        [52, 18, 3, 0, 8, 0, 255, 255]
    );
    assert_eq!(
        import_pairs("0x1234 65535 # pair").unwrap(),
        [52, 18, 255, 255]
    );
    assert_eq!(
        import_tilemap("1234 FFFF\n0000").unwrap(),
        [52, 18, 255, 255, 0, 0]
    );
    assert!(import_words("").unwrap().is_empty());
    for bad in ["-1", "65536", "1 2", "0x", "1.0"] {
        assert!(import_words(bad).is_err());
    }
    for bad in ["", "1", "1 2 3", "0 65536"] {
        assert!(import_pairs(bad).is_err());
    }
    for bad in ["", "0x12", "123", "12345", "0000 # comment"] {
        assert!(import_tilemap(bad).is_err());
    }
}
