// Minimal JSON reader/writer.
//
// WHY THIS FILE EXISTS
// The only JSON this tool consumes is the assembly manifest, and the only JSON
// it emits is one flat report object. Pulling in serde for that would add a
// dependency whose default map ordering we would then have to fight, so a
// small insertion-ordered parser and a `JSON.stringify`-compatible number
// formatter live here instead.

#[derive(Clone, Debug, PartialEq)]
pub enum Value {
    Null,
    Bool(bool),
    Number(f64),
    String(String),
    Array(Vec<Value>),
    /// Insertion-ordered, exactly like a JS object literal.
    Object(Vec<(String, Value)>),
}

impl Value {
    pub fn get(&self, key: &str) -> Option<&Value> {
        match self {
            Value::Object(entries) => entries
                .iter()
                .find(|(name, _)| name == key)
                .map(|(_, value)| value),
            _ => None,
        }
    }

    pub fn as_array(&self) -> Option<&Vec<Value>> {
        match self {
            Value::Array(items) => Some(items),
            _ => None,
        }
    }

    pub fn as_str(&self) -> Option<&str> {
        match self {
            Value::String(text) => Some(text),
            _ => None,
        }
    }

    pub fn as_number(&self) -> Option<f64> {
        match self {
            Value::Number(value) => Some(*value),
            _ => None,
        }
    }
}

pub fn parse(text: &str) -> Result<Value, String> {
    let chars: Vec<char> = text.chars().collect();
    let mut cursor = 0usize;
    let value = parse_value(&chars, &mut cursor)?;
    skip_space(&chars, &mut cursor);
    if cursor != chars.len() {
        return Err(format!("trailing content at {cursor}"));
    }
    Ok(value)
}

fn skip_space(chars: &[char], cursor: &mut usize) {
    while *cursor < chars.len() && matches!(chars[*cursor], ' ' | '\t' | '\n' | '\r') {
        *cursor += 1;
    }
}

fn expect(chars: &[char], cursor: &mut usize, character: char) -> Result<(), String> {
    if chars.get(*cursor) == Some(&character) {
        *cursor += 1;
        Ok(())
    } else {
        Err(format!("expected {character} at {cursor}"))
    }
}

fn parse_value(chars: &[char], cursor: &mut usize) -> Result<Value, String> {
    skip_space(chars, cursor);
    match chars.get(*cursor) {
        None => Err("unexpected end of input".to_string()),
        Some('{') => parse_object(chars, cursor),
        Some('[') => parse_array(chars, cursor),
        Some('"') => Ok(Value::String(parse_string(chars, cursor)?)),
        Some('t') => parse_literal(chars, cursor, "true", Value::Bool(true)),
        Some('f') => parse_literal(chars, cursor, "false", Value::Bool(false)),
        Some('n') => parse_literal(chars, cursor, "null", Value::Null),
        Some(_) => parse_number(chars, cursor),
    }
}

fn parse_literal(
    chars: &[char],
    cursor: &mut usize,
    word: &str,
    value: Value,
) -> Result<Value, String> {
    for expected in word.chars() {
        expect(chars, cursor, expected)?;
    }
    Ok(value)
}

fn parse_object(chars: &[char], cursor: &mut usize) -> Result<Value, String> {
    expect(chars, cursor, '{')?;
    let mut entries: Vec<(String, Value)> = Vec::new();
    skip_space(chars, cursor);
    if chars.get(*cursor) == Some(&'}') {
        *cursor += 1;
        return Ok(Value::Object(entries));
    }
    loop {
        skip_space(chars, cursor);
        let key = parse_string(chars, cursor)?;
        skip_space(chars, cursor);
        expect(chars, cursor, ':')?;
        let value = parse_value(chars, cursor)?;
        entries.push((key, value));
        skip_space(chars, cursor);
        match chars.get(*cursor) {
            Some(',') => *cursor += 1,
            Some('}') => {
                *cursor += 1;
                return Ok(Value::Object(entries));
            }
            _ => return Err(format!("expected , or }} at {cursor}")),
        }
    }
}

fn parse_array(chars: &[char], cursor: &mut usize) -> Result<Value, String> {
    expect(chars, cursor, '[')?;
    let mut items = Vec::new();
    skip_space(chars, cursor);
    if chars.get(*cursor) == Some(&']') {
        *cursor += 1;
        return Ok(Value::Array(items));
    }
    loop {
        items.push(parse_value(chars, cursor)?);
        skip_space(chars, cursor);
        match chars.get(*cursor) {
            Some(',') => *cursor += 1,
            Some(']') => {
                *cursor += 1;
                return Ok(Value::Array(items));
            }
            _ => return Err(format!("expected , or ] at {cursor}")),
        }
    }
}

fn parse_string(chars: &[char], cursor: &mut usize) -> Result<String, String> {
    expect(chars, cursor, '"')?;
    let mut out = String::new();
    loop {
        let character = *chars.get(*cursor).ok_or("unterminated string")?;
        *cursor += 1;
        match character {
            '"' => return Ok(out),
            '\\' => {
                let escape = *chars.get(*cursor).ok_or("unterminated escape")?;
                *cursor += 1;
                match escape {
                    '"' => out.push('"'),
                    '\\' => out.push('\\'),
                    '/' => out.push('/'),
                    'b' => out.push('\u{8}'),
                    'f' => out.push('\u{c}'),
                    'n' => out.push('\n'),
                    'r' => out.push('\r'),
                    't' => out.push('\t'),
                    'u' => {
                        let hex: String = chars
                            .get(*cursor..*cursor + 4)
                            .ok_or("short \\u escape")?
                            .iter()
                            .collect();
                        *cursor += 4;
                        let code = u32::from_str_radix(&hex, 16)
                            .map_err(|_| "invalid \\u escape".to_string())?;
                        out.push(char::from_u32(code).unwrap_or('\u{fffd}'));
                    }
                    other => return Err(format!("invalid escape {other}")),
                }
            }
            other => out.push(other),
        }
    }
}

fn parse_number(chars: &[char], cursor: &mut usize) -> Result<Value, String> {
    let start = *cursor;
    if chars.get(*cursor) == Some(&'-') {
        *cursor += 1;
    }
    while chars
        .get(*cursor)
        .is_some_and(|character| character.is_ascii_digit() || matches!(character, '.' | 'e' | 'E' | '+' | '-'))
    {
        *cursor += 1;
    }
    let text: String = chars[start..*cursor].iter().collect();
    text.parse::<f64>()
        .map(Value::Number)
        .map_err(|_| format!("invalid number {text}"))
}

/// `JSON.stringify` string escaping for the ASCII payloads this tool emits.
pub fn quote(value: &str) -> String {
    let mut out = String::from("\"");
    for character in value.chars() {
        match character {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            '\u{8}' => out.push_str("\\b"),
            '\u{c}' => out.push_str("\\f"),
            other if (other as u32) < 0x20 => out.push_str(&format!("\\u{:04x}", other as u32)),
            other => out.push(other),
        }
    }
    out.push('"');
    out
}

/// `Number#toString` for the finite fractions this tool emits. Rust's shortest
/// round-tripping `{}` agrees with JS across [0, 1]; integral values print
/// without a decimal point in both.
pub fn number(value: f64) -> String {
    if value == value.trunc() && value.abs() < 1e21 {
        return format!("{}", value as i64);
    }
    format!("{value}")
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn objects_keep_insertion_order() {
        let value = parse(r#"{"b": 1, "a": 2, "b": 3}"#).unwrap();
        let Value::Object(entries) = &value else {
            panic!("expected object");
        };
        let keys: Vec<&str> = entries.iter().map(|(key, _)| key.as_str()).collect();
        assert_eq!(keys, vec!["b", "a", "b"]);
    }

    #[test]
    fn manifest_shaped_input_round_trips_the_fields_we_read() {
        let text = r#"{
          "format": 1,
          "regions": [
            { "address": 134217920, "size": 768, "source": "asm/080000c0.s" },
            { "size": 4, "source": "asm/08000100.s", "kind": "code" }
          ]
        }"#;
        let document = parse(text).unwrap();
        let regions = document.get("regions").unwrap().as_array().unwrap();
        assert_eq!(regions.len(), 2);
        assert_eq!(regions[0].get("source").unwrap().as_str(), Some("asm/080000c0.s"));
        assert_eq!(regions[1].get("size").unwrap().as_number(), Some(4.0));
    }

    #[test]
    fn escapes_and_literals_decode() {
        assert_eq!(
            parse(r#""aé\n\t\\""#).unwrap(),
            Value::String("aé\n\t\\".to_string())
        );
        assert_eq!(parse("[true,false,null]").unwrap().as_array().unwrap().len(), 3);
        assert_eq!(parse(" -1.5e2 ").unwrap().as_number(), Some(-150.0));
    }

    #[test]
    fn trailing_content_is_rejected() {
        assert!(parse("{} {}").is_err());
        assert!(parse("{\"a\" 1}").is_err());
    }

    #[test]
    fn numbers_print_the_way_json_stringify_does() {
        assert_eq!(number(0.0), "0");
        assert_eq!(number(1.0), "1");
        assert_eq!(number(0.23076923076923078), "0.23076923076923078");
        assert_eq!(number(1.0 / 3.0), "0.3333333333333333");
        assert_eq!(quote("a\"b"), "\"a\\\"b\"");
    }
}
