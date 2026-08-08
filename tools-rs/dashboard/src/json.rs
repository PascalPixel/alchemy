// A 200-line JSON reader instead of a dependency.
//
// The dashboard reads exactly one number out of metrics/<target>-coverage-map.json
// (categories.exact_c.percent_of_executable) to title the page. Pulling serde +
// serde_json in for that would add two crates and a proc-macro build to a binary
// that otherwise has zero dependencies. This parser is read-only: it never
// re-serializes, so the key-order hazard that forces `preserve_order` on
// serde_json elsewhere in this repo cannot apply here.
//
// It is deliberately not a complete JSON implementation: numbers become f64,
// \u escapes are decoded but lone surrogates are passed through as U+FFFD, and
// duplicate object keys keep the last one. All of that is fine for reading our
// own generated metrics files, and the tests below pin the behaviour.

use std::collections::HashMap;

#[derive(Debug, Clone, PartialEq)]
pub enum Value {
    Null,
    Bool(bool),
    Number(f64),
    String(String),
    Array(Vec<Value>),
    Object(HashMap<String, Value>),
}

impl Value {
    /// Walk a dotted path (`"categories.exact_c.bytes"`) through nested objects.
    pub fn path(&self, path: &str) -> Option<&Value> {
        let mut here = self;
        for key in path.split('.') {
            match here {
                Value::Object(map) => here = map.get(key)?,
                _ => return None,
            }
        }
        Some(here)
    }

    pub fn as_f64(&self) -> Option<f64> {
        match self {
            Value::Number(n) => Some(*n),
            _ => None,
        }
    }
}

struct Parser<'a> {
    bytes: &'a [u8],
    at: usize,
}

pub fn parse(text: &str) -> Result<Value, String> {
    let mut parser = Parser { bytes: text.as_bytes(), at: 0 };
    parser.space();
    let value = parser.value()?;
    parser.space();
    if parser.at != parser.bytes.len() {
        return Err(format!("trailing input at byte {}", parser.at));
    }
    Ok(value)
}

impl Parser<'_> {
    fn peek(&self) -> Option<u8> {
        self.bytes.get(self.at).copied()
    }

    fn space(&mut self) {
        while matches!(self.peek(), Some(b' ' | b'\t' | b'\n' | b'\r')) {
            self.at += 1;
        }
    }

    fn expect(&mut self, byte: u8) -> Result<(), String> {
        if self.peek() == Some(byte) {
            self.at += 1;
            Ok(())
        } else {
            Err(format!("expected {:?} at byte {}", byte as char, self.at))
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
            Some(b't') => self.literal("true").map(|()| Value::Bool(true)),
            Some(b'f') => self.literal("false").map(|()| Value::Bool(false)),
            Some(b'n') => self.literal("null").map(|()| Value::Null),
            Some(_) => self.number(),
            None => Err("unexpected end of input".to_string()),
        }
    }

    fn object(&mut self) -> Result<Value, String> {
        self.expect(b'{')?;
        let mut map = HashMap::new();
        self.space();
        if self.peek() == Some(b'}') {
            self.at += 1;
            return Ok(Value::Object(map));
        }
        loop {
            self.space();
            let key = self.string()?;
            self.space();
            self.expect(b':')?;
            self.space();
            let value = self.value()?;
            map.insert(key, value);
            self.space();
            match self.peek() {
                Some(b',') => self.at += 1,
                Some(b'}') => {
                    self.at += 1;
                    return Ok(Value::Object(map));
                }
                _ => return Err(format!("expected ',' or '}}' at byte {}", self.at)),
            }
        }
    }

    fn array(&mut self) -> Result<Value, String> {
        self.expect(b'[')?;
        let mut items = Vec::new();
        self.space();
        if self.peek() == Some(b']') {
            self.at += 1;
            return Ok(Value::Array(items));
        }
        loop {
            self.space();
            items.push(self.value()?);
            self.space();
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
        let mut out = String::new();
        loop {
            let byte = self.peek().ok_or("unterminated string")?;
            self.at += 1;
            match byte {
                b'"' => return Ok(out),
                b'\\' => {
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
                        b'u' => out.push(self.unicode_escape()?),
                        other => return Err(format!("bad escape \\{}", other as char)),
                    }
                }
                _ => {
                    // Copy the whole UTF-8 sequence: the input is a &str, so the
                    // byte offsets always land on character boundaries.
                    let start = self.at - 1;
                    while self.peek().is_some_and(|b| b & 0xc0 == 0x80) {
                        self.at += 1;
                    }
                    out.push_str(std::str::from_utf8(&self.bytes[start..self.at]).map_err(
                        |error| format!("invalid utf-8 in string: {error}"),
                    )?);
                }
            }
        }
    }

    fn unicode_escape(&mut self) -> Result<char, String> {
        let hex = self
            .bytes
            .get(self.at..self.at + 4)
            .ok_or("truncated \\u escape")?;
        let text = std::str::from_utf8(hex).map_err(|_| "bad \\u escape".to_string())?;
        let code = u32::from_str_radix(text, 16).map_err(|_| "bad \\u escape".to_string())?;
        self.at += 4;
        // Surrogate pairs: consume the low half when it follows.
        if (0xd800..0xdc00).contains(&code) && self.bytes[self.at..].starts_with(b"\\u") {
            let save = self.at;
            self.at += 2;
            let low = self.unicode_escape_raw()?;
            if (0xdc00..0xe000).contains(&low) {
                let combined = 0x10000 + ((code - 0xd800) << 10) + (low - 0xdc00);
                return Ok(char::from_u32(combined).unwrap_or('\u{fffd}'));
            }
            self.at = save;
        }
        Ok(char::from_u32(code).unwrap_or('\u{fffd}'))
    }

    fn unicode_escape_raw(&mut self) -> Result<u32, String> {
        let hex = self
            .bytes
            .get(self.at..self.at + 4)
            .ok_or("truncated \\u escape")?;
        let text = std::str::from_utf8(hex).map_err(|_| "bad \\u escape".to_string())?;
        let code = u32::from_str_radix(text, 16).map_err(|_| "bad \\u escape".to_string())?;
        self.at += 4;
        Ok(code)
    }

    fn number(&mut self) -> Result<Value, String> {
        let start = self.at;
        if self.peek() == Some(b'-') {
            self.at += 1;
        }
        while self
            .peek()
            .is_some_and(|b| b.is_ascii_digit() || matches!(b, b'.' | b'e' | b'E' | b'+' | b'-'))
        {
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

    /// Assemble the shape the real metrics file has, then read the one field
    /// the dashboard actually wants out of it.
    fn coverage_fixture() -> String {
        r#"{
          "format": "alchemy-coverage-map/1",
          "target": "gs1-en",
          "executable_bytes": 1345860,
          "categories": {
            "exact_c": { "bytes": 316342, "percent_of_executable": 23.5 },
            "semantic_c": { "bytes": 951792, "percent_of_executable": 70.72 },
            "assembly": { "bytes": 0, "percent_of_executable": 0 }
          },
          "executable_areas": [ { "id": "overlays", "tiles": [] } ]
        }"#
        .to_string()
    }

    #[test]
    fn reads_the_exact_percentage_out_of_a_coverage_map() {
        let value = parse(&coverage_fixture()).expect("fixture parses");
        assert_eq!(
            value.path("categories.exact_c.percent_of_executable").and_then(Value::as_f64),
            Some(23.5)
        );
        assert_eq!(
            value.path("categories.exact_c.bytes").and_then(Value::as_f64),
            Some(316342.0)
        );
        assert_eq!(value.path("categories.missing.bytes"), None);
        assert_eq!(value.path("executable_areas.0"), None);
    }

    #[test]
    fn keeps_numbers_that_would_round_differently_as_f64() {
        let value = parse(r#"{"a": 70.72, "b": -1e3, "c": 0}"#).unwrap();
        assert_eq!(value.path("a").and_then(Value::as_f64), Some(70.72));
        assert_eq!(value.path("b").and_then(Value::as_f64), Some(-1000.0));
        assert_eq!(value.path("c").and_then(Value::as_f64), Some(0.0));
    }

    #[test]
    fn decodes_the_string_escapes_our_labels_can_contain() {
        let value = parse(r#"{"label": "a·b \"q\" \\ \n é 😀"}"#).unwrap();
        assert_eq!(
            value.path("label"),
            Some(&Value::String("a·b \"q\" \\ \n é 😀".to_string()))
        );
    }

    #[test]
    fn passes_through_multibyte_text_that_is_not_escaped() {
        let value = parse("{\"label\": \"golden·sun — 日本\"}").unwrap();
        assert_eq!(
            value.path("label"),
            Some(&Value::String("golden·sun — 日本".to_string()))
        );
    }

    #[test]
    fn rejects_input_that_is_not_json() {
        assert!(parse("{").is_err());
        assert!(parse("{\"a\": 1,}").is_err());
        assert!(parse("{} {}").is_err());
        assert!(parse("").is_err());
    }

    #[test]
    fn handles_empty_containers_and_literals() {
        assert_eq!(parse("{}").unwrap(), Value::Object(HashMap::new()));
        assert_eq!(parse("[]").unwrap(), Value::Array(vec![]));
        assert_eq!(parse("[true,false,null]").unwrap(), Value::Array(vec![
            Value::Bool(true),
            Value::Bool(false),
            Value::Null,
        ]));
    }
}
