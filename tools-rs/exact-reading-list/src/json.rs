// A minimal JSON reader for this crate.
//
// WHY this exists instead of a dependency: the port rules for tools-rs prefer
// zero dependencies, and this tool needs exactly one direction of JSON — read
// two well-formed generator outputs (`out/decomp/overlays.json`, ~84 MB, and
// `metrics/gs1-en-executable.json`). Writing is done by hand in `lib.rs`
// because the only emitted shape is a fixed five-field record whose key order
// is load-bearing.
//
// PORT NOTE: object keys are kept in insertion order in a `Vec`, never a
// `HashMap`. JS object iteration order is insertion order for string keys and
// the rest of this port relies on that being preserved.
//
// PORT NOTE: on a duplicate key, `JSON.parse` keeps the LAST occurrence, so
// `Value::get` scans from the end.

#[derive(Debug, Clone, PartialEq)]
pub enum Value {
    Null,
    Bool(bool),
    Number(f64),
    String(String),
    Array(Vec<Value>),
    Object(Vec<(String, Value)>),
}

impl Value {
    pub fn get(&self, key: &str) -> Option<&Value> {
        match self {
            Value::Object(entries) => entries
                .iter()
                .rev()
                .find(|(name, _)| name == key)
                .map(|(_, value)| value),
            _ => None,
        }
    }

    pub fn as_array(&self) -> Option<&[Value]> {
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

    pub fn as_f64(&self) -> Option<f64> {
        match self {
            Value::Number(number) => Some(*number),
            _ => None,
        }
    }

    /// `true` exactly when JS `Boolean(value)` would be true for the values
    /// this tool reads (booleans and the absent/`null` case).
    pub fn truthy(&self) -> bool {
        match self {
            Value::Null => false,
            Value::Bool(flag) => *flag,
            Value::Number(number) => *number != 0.0 && !number.is_nan(),
            Value::String(text) => !text.is_empty(),
            _ => true,
        }
    }
}

pub fn parse(text: &str) -> Result<Value, String> {
    let bytes = text.as_bytes();
    let mut cursor = Cursor { bytes, at: 0 };
    cursor.skip_whitespace();
    let value = cursor.value()?;
    cursor.skip_whitespace();
    if cursor.at != bytes.len() {
        return Err(format!("trailing content at byte {}", cursor.at));
    }
    Ok(value)
}

struct Cursor<'a> {
    bytes: &'a [u8],
    at: usize,
}

impl Cursor<'_> {
    fn peek(&self) -> Option<u8> {
        self.bytes.get(self.at).copied()
    }

    fn skip_whitespace(&mut self) {
        while matches!(self.peek(), Some(b' ' | b'\t' | b'\n' | b'\r')) {
            self.at += 1;
        }
    }

    fn expect(&mut self, byte: u8) -> Result<(), String> {
        if self.peek() == Some(byte) {
            self.at += 1;
            Ok(())
        } else {
            Err(format!(
                "expected {:?} at byte {}",
                byte as char, self.at
            ))
        }
    }

    fn literal(&mut self, word: &str) -> Result<(), String> {
        if self.bytes[self.at..].starts_with(word.as_bytes()) {
            self.at += word.len();
            Ok(())
        } else {
            Err(format!("expected {word} at byte {}", self.at))
        }
    }

    fn value(&mut self) -> Result<Value, String> {
        match self.peek() {
            Some(b'{') => self.object(),
            Some(b'[') => self.array(),
            Some(b'"') => Ok(Value::String(self.string()?)),
            Some(b't') => {
                self.literal("true")?;
                Ok(Value::Bool(true))
            }
            Some(b'f') => {
                self.literal("false")?;
                Ok(Value::Bool(false))
            }
            Some(b'n') => {
                self.literal("null")?;
                Ok(Value::Null)
            }
            Some(_) => self.number(),
            None => Err("unexpected end of input".to_string()),
        }
    }

    fn object(&mut self) -> Result<Value, String> {
        self.expect(b'{')?;
        let mut entries: Vec<(String, Value)> = Vec::new();
        self.skip_whitespace();
        if self.peek() == Some(b'}') {
            self.at += 1;
            return Ok(Value::Object(entries));
        }
        loop {
            self.skip_whitespace();
            let key = self.string()?;
            self.skip_whitespace();
            self.expect(b':')?;
            self.skip_whitespace();
            let value = self.value()?;
            entries.push((key, value));
            self.skip_whitespace();
            match self.peek() {
                Some(b',') => self.at += 1,
                Some(b'}') => {
                    self.at += 1;
                    return Ok(Value::Object(entries));
                }
                _ => return Err(format!("expected ',' or '}}' at byte {}", self.at)),
            }
        }
    }

    fn array(&mut self) -> Result<Value, String> {
        self.expect(b'[')?;
        let mut items: Vec<Value> = Vec::new();
        self.skip_whitespace();
        if self.peek() == Some(b']') {
            self.at += 1;
            return Ok(Value::Array(items));
        }
        loop {
            self.skip_whitespace();
            items.push(self.value()?);
            self.skip_whitespace();
            match self.peek() {
                Some(b',') => self.at += 1,
                Some(b']') => {
                    self.at += 1;
                    return Ok(Value::Array(items));
                }
                _ => return Err(format!("expected ',' or ']' at byte {}", self.at)),
            }
        }
    }

    fn string(&mut self) -> Result<String, String> {
        self.expect(b'"')?;
        let start = self.at;
        // Fast path: no escapes means the slice is already the answer.
        while let Some(byte) = self.peek() {
            match byte {
                b'"' => {
                    let raw = &self.bytes[start..self.at];
                    self.at += 1;
                    return Ok(String::from_utf8_lossy(raw).into_owned());
                }
                b'\\' => break,
                _ => self.at += 1,
            }
        }
        // Slow path: rebuild from `start`, honouring escapes.
        let mut out = String::from_utf8_lossy(&self.bytes[start..self.at]).into_owned();
        loop {
            let byte = self.peek().ok_or("unterminated string")?;
            match byte {
                b'"' => {
                    self.at += 1;
                    return Ok(out);
                }
                b'\\' => {
                    self.at += 1;
                    let escape = self.peek().ok_or("unterminated escape")?;
                    self.at += 1;
                    match escape {
                        b'"' => out.push('"'),
                        b'\\' => out.push('\\'),
                        b'/' => out.push('/'),
                        b'b' => out.push('\u{8}'),
                        b'f' => out.push('\u{c}'),
                        b'n' => out.push('\n'),
                        b'r' => out.push('\r'),
                        b't' => out.push('\t'),
                        b'u' => {
                            let unit = self.hex4()?;
                            if (0xd800..0xdc00).contains(&unit)
                                && self.bytes[self.at..].starts_with(b"\\u")
                            {
                                let mark = self.at;
                                self.at += 2;
                                let low = self.hex4()?;
                                if (0xdc00..0xe000).contains(&low) {
                                    let code = 0x10000
                                        + ((unit - 0xd800) << 10)
                                        + (low - 0xdc00);
                                    out.push(char::from_u32(code).unwrap_or('\u{fffd}'));
                                    continue;
                                }
                                self.at = mark;
                            }
                            // PORT NOTE: a lone surrogate is not a Rust `char`.
                            // JS would keep it; the closest lossless-enough
                            // stand-in is U+FFFD, matching what writing such a
                            // string out as UTF-8 would produce anyway.
                            out.push(char::from_u32(unit).unwrap_or('\u{fffd}'));
                        }
                        other => {
                            return Err(format!("bad escape \\{}", other as char));
                        }
                    }
                }
                _ => {
                    let from = self.at;
                    while let Some(next) = self.peek() {
                        if next == b'"' || next == b'\\' {
                            break;
                        }
                        self.at += 1;
                    }
                    out.push_str(&String::from_utf8_lossy(&self.bytes[from..self.at]));
                }
            }
        }
    }

    fn hex4(&mut self) -> Result<u32, String> {
        if self.at + 4 > self.bytes.len() {
            return Err("truncated \\u escape".to_string());
        }
        let digits = std::str::from_utf8(&self.bytes[self.at..self.at + 4])
            .map_err(|_| "bad \\u escape".to_string())?;
        let value = u32::from_str_radix(digits, 16).map_err(|_| "bad \\u escape".to_string())?;
        self.at += 4;
        Ok(value)
    }

    fn number(&mut self) -> Result<Value, String> {
        let start = self.at;
        if self.peek() == Some(b'-') {
            self.at += 1;
        }
        while matches!(self.peek(), Some(b'0'..=b'9' | b'.' | b'e' | b'E' | b'+' | b'-')) {
            self.at += 1;
        }
        let text = std::str::from_utf8(&self.bytes[start..self.at])
            .map_err(|_| "bad number".to_string())?;
        text.parse::<f64>()
            .map(Value::Number)
            .map_err(|_| format!("bad number {text:?} at byte {start}"))
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn keeps_object_key_order() {
        let value = parse(r#"{"b":1,"a":2,"c":3}"#).unwrap();
        let Value::Object(entries) = &value else {
            panic!("expected object");
        };
        let keys: Vec<&str> = entries.iter().map(|(k, _)| k.as_str()).collect();
        assert_eq!(keys, vec!["b", "a", "c"]);
    }

    #[test]
    fn last_duplicate_key_wins_like_js() {
        let value = parse(r#"{"n":1,"n":2}"#).unwrap();
        assert_eq!(value.get("n").and_then(Value::as_f64), Some(2.0));
    }

    #[test]
    fn reads_nested_shapes_and_escapes() {
        let value = parse(
            r#"{"functions":[{"overlay":"resource_1é","offset":16,"ok":true,"gone":null}]}"#,
        )
        .unwrap();
        let row = &value.get("functions").unwrap().as_array().unwrap()[0];
        assert_eq!(row.get("overlay").unwrap().as_str(), Some("resource_1é"));
        assert_eq!(row.get("offset").unwrap().as_f64(), Some(16.0));
        assert!(row.get("ok").unwrap().truthy());
        assert!(!row.get("gone").unwrap().truthy());
    }

    #[test]
    fn reads_surrogate_pairs_and_whitespace() {
        let value = parse("  {\n\t\"k\" : \"a\\ud83d\\ude00b\"\r\n}  ").unwrap();
        assert_eq!(value.get("k").unwrap().as_str(), Some("a😀b"));
    }

    #[test]
    fn rejects_trailing_content() {
        assert!(parse("{} {}").is_err());
        assert!(parse("[1,]").is_err());
    }

    #[test]
    fn reads_empty_containers_and_exponents() {
        assert_eq!(parse("[]").unwrap(), Value::Array(vec![]));
        assert_eq!(parse("{}").unwrap(), Value::Object(vec![]));
        assert_eq!(parse("-1.5e3").unwrap(), Value::Number(-1500.0));
    }
}
