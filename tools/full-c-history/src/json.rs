// Minimal JSON reader/writer for the Full-C history ledger.
//
// WHY: this crate is dependency-free by policy, and the ledger only has to
// read two manifest shapes and one progress report, then emit one document in
// the repository's `canonical_json` layout. A general JSON library would add a
// dependency for that. Object members keep source order (`Vec<(String, Value)>`,
// never a HashMap) because JS object iteration order is insertion order and the
// canonical writer depends on it.

use std::fmt::Write as _;

#[derive(Debug, Clone, PartialEq)]
pub enum Value {
    Null,
    Bool(bool),
    Num(f64),
    Str(String),
    Arr(Vec<Value>),
    Obj(Vec<(String, Value)>),
}

impl Value {
    pub fn get(&self, key: &str) -> Option<&Value> {
        match self {
            Value::Obj(members) => members.iter().find(|(k, _)| k == key).map(|(_, v)| v),
            _ => None,
        }
    }

    pub fn as_str(&self) -> Option<&str> {
        match self {
            Value::Str(text) => Some(text),
            _ => None,
        }
    }

    pub fn as_f64(&self) -> Option<f64> {
        match self {
            Value::Num(value) => Some(*value),
            _ => None,
        }
    }

    pub fn as_array(&self) -> Option<&[Value]> {
        match self {
            Value::Arr(items) => Some(items),
            _ => None,
        }
    }
}

pub fn parse(text: &str) -> Result<Value, String> {
    let bytes = text.as_bytes();
    let mut cursor = 0usize;
    let value = parse_value(bytes, &mut cursor)?;
    skip_ws(bytes, &mut cursor);
    if cursor != bytes.len() {
        return Err(format!("trailing JSON content at byte {cursor}"));
    }
    Ok(value)
}

fn skip_ws(bytes: &[u8], cursor: &mut usize) {
    while *cursor < bytes.len() && matches!(bytes[*cursor], b' ' | b'\t' | b'\n' | b'\r') {
        *cursor += 1;
    }
}

fn parse_value(bytes: &[u8], cursor: &mut usize) -> Result<Value, String> {
    skip_ws(bytes, cursor);
    let first = *bytes.get(*cursor).ok_or("unexpected end of JSON")?;
    match first {
        b'{' => parse_object(bytes, cursor),
        b'[' => parse_array(bytes, cursor),
        b'"' => Ok(Value::Str(parse_string(bytes, cursor)?)),
        b't' => literal(bytes, cursor, "true", Value::Bool(true)),
        b'f' => literal(bytes, cursor, "false", Value::Bool(false)),
        b'n' => literal(bytes, cursor, "null", Value::Null),
        _ => parse_number(bytes, cursor),
    }
}

fn literal(bytes: &[u8], cursor: &mut usize, word: &str, value: Value) -> Result<Value, String> {
    if bytes[*cursor..].starts_with(word.as_bytes()) {
        *cursor += word.len();
        Ok(value)
    } else {
        Err(format!("invalid JSON literal at byte {cursor}"))
    }
}

fn parse_number(bytes: &[u8], cursor: &mut usize) -> Result<Value, String> {
    let start = *cursor;
    if *cursor < bytes.len() && (bytes[*cursor] == b'-' || bytes[*cursor] == b'+') {
        *cursor += 1;
    }
    while *cursor < bytes.len()
        && matches!(bytes[*cursor], b'0'..=b'9' | b'.' | b'e' | b'E' | b'-' | b'+')
    {
        *cursor += 1;
    }
    let text = std::str::from_utf8(&bytes[start..*cursor]).map_err(|error| error.to_string())?;
    text.parse::<f64>()
        .map(Value::Num)
        .map_err(|_| format!("invalid JSON number {text:?}"))
}

fn parse_string(bytes: &[u8], cursor: &mut usize) -> Result<String, String> {
    *cursor += 1; // opening quote
    let mut units: Vec<u16> = Vec::new();
    loop {
        let byte = *bytes.get(*cursor).ok_or("unterminated JSON string")?;
        match byte {
            b'"' => {
                *cursor += 1;
                return Ok(String::from_utf16_lossy(&units));
            }
            b'\\' => {
                *cursor += 1;
                let escape = *bytes.get(*cursor).ok_or("unterminated JSON escape")?;
                *cursor += 1;
                match escape {
                    b'"' => units.push(u16::from(b'"')),
                    b'\\' => units.push(u16::from(b'\\')),
                    b'/' => units.push(u16::from(b'/')),
                    b'b' => units.push(0x08),
                    b'f' => units.push(0x0c),
                    b'n' => units.push(0x0a),
                    b'r' => units.push(0x0d),
                    b't' => units.push(0x09),
                    b'u' => {
                        let hex = std::str::from_utf8(
                            bytes.get(*cursor..*cursor + 4).ok_or("short \\u escape")?,
                        )
                        .map_err(|error| error.to_string())?;
                        units.push(
                            u16::from_str_radix(hex, 16).map_err(|_| "bad \\u escape".to_string())?,
                        );
                        *cursor += 4;
                    }
                    other => return Err(format!("unknown JSON escape \\{}", other as char)),
                }
            }
            _ => {
                // Copy the whole UTF-8 sequence, then re-encode to UTF-16.
                let start = *cursor;
                *cursor += 1;
                while *cursor < bytes.len() && (bytes[*cursor] & 0xc0) == 0x80 {
                    *cursor += 1;
                }
                let chunk = String::from_utf8_lossy(&bytes[start..*cursor]);
                units.extend(chunk.encode_utf16());
            }
        }
    }
}

fn parse_array(bytes: &[u8], cursor: &mut usize) -> Result<Value, String> {
    *cursor += 1;
    let mut items = Vec::new();
    skip_ws(bytes, cursor);
    if bytes.get(*cursor) == Some(&b']') {
        *cursor += 1;
        return Ok(Value::Arr(items));
    }
    loop {
        items.push(parse_value(bytes, cursor)?);
        skip_ws(bytes, cursor);
        match bytes.get(*cursor) {
            Some(b',') => *cursor += 1,
            Some(b']') => {
                *cursor += 1;
                return Ok(Value::Arr(items));
            }
            _ => return Err(format!("expected , or ] at byte {cursor}")),
        }
    }
}

fn parse_object(bytes: &[u8], cursor: &mut usize) -> Result<Value, String> {
    *cursor += 1;
    let mut members: Vec<(String, Value)> = Vec::new();
    skip_ws(bytes, cursor);
    if bytes.get(*cursor) == Some(&b'}') {
        *cursor += 1;
        return Ok(Value::Obj(members));
    }
    loop {
        skip_ws(bytes, cursor);
        if bytes.get(*cursor) != Some(&b'"') {
            return Err(format!("expected object key at byte {cursor}"));
        }
        let key = parse_string(bytes, cursor)?;
        skip_ws(bytes, cursor);
        if bytes.get(*cursor) != Some(&b':') {
            return Err(format!("expected : at byte {cursor}"));
        }
        *cursor += 1;
        let value = parse_value(bytes, cursor)?;
        // PORT NOTE: JS object literals keep the FIRST insertion position for a
        // repeated key but take the LAST value; mirror that here.
        if let Some(slot) = members.iter_mut().find(|(existing, _)| *existing == key) {
            slot.1 = value;
        } else {
            members.push((key, value));
        }
        skip_ws(bytes, cursor);
        match bytes.get(*cursor) {
            Some(b',') => *cursor += 1,
            Some(b'}') => {
                *cursor += 1;
                return Ok(Value::Obj(members));
            }
            _ => return Err(format!("expected , or }} at byte {cursor}")),
        }
    }
}

/// `JSON.stringify` of a string: escape `"`, `\`, and C0 controls only. Every
/// other code point, including non-ASCII such as the ☀️ in a commit subject, is
/// emitted literally.
pub fn quote(text: &str) -> String {
    let mut out = String::with_capacity(text.len() + 2);
    out.push('"');
    for character in text.chars() {
        match character {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\u{8}' => out.push_str("\\b"),
            '\u{c}' => out.push_str("\\f"),
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            other if (other as u32) < 0x20 => {
                let _ = write!(out, "\\u{:04x}", other as u32);
            }
            other => out.push(other),
        }
    }
    out.push('"');
    out
}

/// `String(number)` / `JSON.stringify(number)` for the finite values this
/// ledger emits. Rust's `f64` Display is shortest-round-trip and, like JS,
/// prints `23` rather than `23.0`, so integral values need no special case.
pub fn number(value: f64) -> String {
    if value == 0.0 {
        // -0 stringifies as "0" in JS.
        return "0".to_string();
    }
    format!("{value}")
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn parses_nested_documents_in_source_order() {
        let document = parse(r#"{"b": 1, "a": [1, {"z": "y"}], "c": null}"#).unwrap();
        let Value::Obj(members) = &document else { panic!("expected object") };
        assert_eq!(
            members.iter().map(|(k, _)| k.as_str()).collect::<Vec<_>>(),
            vec!["b", "a", "c"]
        );
        assert_eq!(
            document.get("a").unwrap().as_array().unwrap()[1].get("z").unwrap().as_str(),
            Some("y")
        );
    }

    #[test]
    fn repeated_keys_keep_first_position_and_last_value() {
        let document = parse(r#"{"a": 1, "b": 2, "a": 3}"#).unwrap();
        let Value::Obj(members) = &document else { panic!("expected object") };
        assert_eq!(members[0].0, "a");
        assert_eq!(members[0].1, Value::Num(3.0));
        assert_eq!(members.len(), 2);
    }

    #[test]
    fn decodes_escapes_and_surrogate_pairs() {
        let document = parse(r#"["a\tb", "☀️", "😀"]"#).unwrap();
        let items = document.as_array().unwrap();
        assert_eq!(items[0].as_str(), Some("a\tb"));
        assert_eq!(items[1].as_str(), Some("☀️"));
        assert_eq!(items[2].as_str(), Some("😀"));
    }

    #[test]
    fn quote_matches_json_stringify() {
        assert_eq!(quote("a\"b\\c"), "\"a\\\"b\\\\c\"");
        assert_eq!(quote("\u{1}\n"), "\"\\u0001\\n\"");
        assert_eq!(quote("☀️ ok"), "\"☀️ ok\"");
    }

    #[test]
    fn number_matches_js_stringification() {
        assert_eq!(number(0.0), "0");
        assert_eq!(number(-0.0), "0");
        assert_eq!(number(23.56), "23.56");
        assert_eq!(number(12.5), "12.5");
        assert_eq!(number(1345722.0), "1345722");
    }
}
