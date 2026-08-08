// A minimal JSON value, parser and canonical writer, sized exactly to what
// localization_tables.ts needs from `JSON.parse`, `JSON.stringify` and
// `tools/lib/canonical_json.ts`.
//
// Why this exists rather than serde_json: the port must reproduce JavaScript
// object semantics, not Rust map semantics. Key order is INSERTION order and it
// is load-bearing (the exported records are compared byte-for-byte against the
// tracked `.json` files), duplicate keys must update in place rather than move
// to the end, and `Object.keys` on an array yields its indices. A BTreeMap or a
// HashMap gets every one of those wrong.

use std::fmt::Write as _;

#[derive(Debug, Clone, PartialEq)]
pub enum Value {
    Null,
    Bool(bool),
    Num(f64),
    Str(String),
    Arr(Vec<Value>),
    // PORT NOTE: Vec, not a map. JS objects iterate in insertion order for
    // string keys that are not array indices, and every key here is such a key.
    Obj(Vec<(String, Value)>),
}

impl Value {
    pub fn obj(pairs: Vec<(&str, Value)>) -> Value {
        Value::Obj(pairs.into_iter().map(|(k, v)| (k.to_string(), v)).collect())
    }

    pub fn get(&self, key: &str) -> Option<&Value> {
        match self {
            Value::Obj(pairs) => pairs.iter().find(|(k, _)| k == key).map(|(_, v)| v),
            _ => None,
        }
    }

    pub fn as_str(&self) -> Option<&str> {
        match self {
            Value::Str(s) => Some(s),
            _ => None,
        }
    }

    pub fn as_num(&self) -> Option<f64> {
        match self {
            Value::Num(n) => Some(*n),
            _ => None,
        }
    }

    /// `Object.keys(value)`: an object's own keys in insertion order, an
    /// array's indices as decimal strings, and nothing for a primitive.
    pub fn own_keys(&self) -> Vec<String> {
        match self {
            Value::Obj(pairs) => pairs.iter().map(|(k, _)| k.clone()).collect(),
            Value::Arr(items) => (0..items.len()).map(|i| i.to_string()).collect(),
            _ => Vec::new(),
        }
    }
}

/// `JSON.stringify` for a number. Integral magnitudes below 1e21 print without
/// an exponent; Rust's own `{}` already agrees with JS shortest-round-trip for
/// everything this tool emits (integers only).
pub fn number_text(value: f64) -> String {
    if value == 0.0 {
        return "0".to_string(); // JS prints -0 as "0"
    }
    if !value.is_finite() {
        return "null".to_string();
    }
    format!("{value}")
}

pub fn quote(text: &str) -> String {
    let mut out = String::with_capacity(text.len() + 2);
    out.push('"');
    for ch in text.chars() {
        match ch {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            '\u{8}' => out.push_str("\\b"),
            '\u{c}' => out.push_str("\\f"),
            c if (c as u32) < 0x20 => {
                let _ = write!(out, "\\u{:04x}", c as u32);
            }
            c => out.push(c),
        }
    }
    out.push('"');
    out
}

/// `JSON.stringify(value)` with no spacing, for the consumer-list comparison.
pub fn minified(value: &Value) -> String {
    match value {
        Value::Null => "null".to_string(),
        Value::Bool(b) => b.to_string(),
        Value::Num(n) => number_text(*n),
        Value::Str(s) => quote(s),
        Value::Arr(items) => {
            let parts: Vec<String> = items.iter().map(minified).collect();
            format!("[{}]", parts.join(","))
        }
        Value::Obj(pairs) => {
            let parts: Vec<String> = pairs
                .iter()
                .map(|(k, v)| format!("{}:{}", quote(k), minified(v)))
                .collect();
            format!("{{{}}}", parts.join(","))
        }
    }
}

fn is_primitive(value: &Value) -> bool {
    matches!(
        value,
        Value::Null | Value::Num(_) | Value::Str(_) | Value::Bool(_)
    )
}

fn reflow(value: &Value, indent: &str) -> String {
    if is_primitive(value) {
        return minified(value);
    }
    match value {
        Value::Arr(items) => {
            if items.is_empty() {
                return "[]".to_string();
            }
            if items.iter().all(is_primitive) {
                let parts: Vec<String> = items.iter().map(minified).collect();
                return format!("[{}]", parts.join(", "));
            }
            let inner = format!("{indent}  ");
            let parts: Vec<String> = items
                .iter()
                .map(|item| format!("{inner}{}", reflow(item, &inner)))
                .collect();
            format!("[\n{}\n{indent}]", parts.join(",\n"))
        }
        Value::Obj(pairs) => {
            if pairs.is_empty() {
                return "{}".to_string();
            }
            let inner = format!("{indent}  ");
            let parts: Vec<String> = pairs
                .iter()
                .map(|(k, v)| format!("{inner}{}: {}", quote(k), reflow(v, &inner)))
                .collect();
            format!("{{\n{}\n{indent}}}", parts.join(",\n"))
        }
        _ => unreachable!(),
    }
}

/// `canonicalJson` from tools/lib/canonical_json.ts.
pub fn canonical_json(value: &Value) -> String {
    reflow(value, "")
}

// ---------------------------------------------------------------------------
// Parser
// ---------------------------------------------------------------------------

pub struct Parser<'a> {
    bytes: &'a [u8],
    at: usize,
}

pub fn parse(text: &str) -> Result<Value, String> {
    let mut parser = Parser {
        bytes: text.as_bytes(),
        at: 0,
    };
    parser.space();
    let value = parser.value()?;
    parser.space();
    if parser.at != parser.bytes.len() {
        return Err(format!(
            "Unexpected non-whitespace character after JSON at position {}",
            parser.at
        ));
    }
    Ok(value)
}

impl Parser<'_> {
    fn space(&mut self) {
        while let Some(byte) = self.bytes.get(self.at) {
            // JSON whitespace is exactly these four; not JS `\s`.
            if matches!(byte, b' ' | b'\t' | b'\n' | b'\r') {
                self.at += 1;
            } else {
                break;
            }
        }
    }

    fn peek(&self) -> Option<u8> {
        self.bytes.get(self.at).copied()
    }

    fn expect(&mut self, byte: u8) -> Result<(), String> {
        if self.peek() == Some(byte) {
            self.at += 1;
            Ok(())
        } else {
            Err(self.unexpected())
        }
    }

    fn unexpected(&self) -> String {
        match self.peek() {
            Some(byte) => format!(
                "Unexpected token '{}' at position {}",
                byte as char, self.at
            ),
            None => "Unexpected end of JSON input".to_string(),
        }
    }

    fn literal(&mut self, word: &str) -> bool {
        if self.bytes[self.at..].starts_with(word.as_bytes()) {
            self.at += word.len();
            true
        } else {
            false
        }
    }

    fn value(&mut self) -> Result<Value, String> {
        match self.peek() {
            Some(b'{') => self.object(),
            Some(b'[') => self.array(),
            Some(b'"') => Ok(Value::Str(self.string()?)),
            Some(b't') if self.literal("true") => Ok(Value::Bool(true)),
            Some(b'f') if self.literal("false") => Ok(Value::Bool(false)),
            Some(b'n') if self.literal("null") => Ok(Value::Null),
            Some(byte) if byte == b'-' || byte.is_ascii_digit() => self.number(),
            _ => Err(self.unexpected()),
        }
    }

    fn object(&mut self) -> Result<Value, String> {
        self.expect(b'{')?;
        let mut pairs: Vec<(String, Value)> = Vec::new();
        self.space();
        if self.peek() == Some(b'}') {
            self.at += 1;
            return Ok(Value::Obj(pairs));
        }
        loop {
            self.space();
            let key = self.string()?;
            self.space();
            self.expect(b':')?;
            self.space();
            let value = self.value()?;
            // PORT NOTE: a repeated key overwrites in place. JS keeps the
            // original insertion position, so the record's key order (which we
            // reproduce byte-for-byte) does not shift.
            match pairs.iter_mut().find(|(k, _)| *k == key) {
                Some(slot) => slot.1 = value,
                None => pairs.push((key, value)),
            }
            self.space();
            match self.peek() {
                Some(b',') => self.at += 1,
                Some(b'}') => {
                    self.at += 1;
                    return Ok(Value::Obj(pairs));
                }
                _ => return Err(self.unexpected()),
            }
        }
    }

    fn array(&mut self) -> Result<Value, String> {
        self.expect(b'[')?;
        let mut items = Vec::new();
        self.space();
        if self.peek() == Some(b']') {
            self.at += 1;
            return Ok(Value::Arr(items));
        }
        loop {
            self.space();
            items.push(self.value()?);
            self.space();
            match self.peek() {
                Some(b',') => self.at += 1,
                Some(b']') => {
                    self.at += 1;
                    return Ok(Value::Arr(items));
                }
                _ => return Err(self.unexpected()),
            }
        }
    }

    fn string(&mut self) -> Result<String, String> {
        self.expect(b'"')?;
        let mut units: Vec<u16> = Vec::new();
        loop {
            let byte = self.peek().ok_or("Unterminated string in JSON")?;
            match byte {
                b'"' => {
                    self.at += 1;
                    return String::from_utf16(&units)
                        .map_err(|_| "Bad Unicode escape".to_string());
                }
                b'\\' => {
                    self.at += 1;
                    let escape = self.peek().ok_or("Unterminated string in JSON")?;
                    self.at += 1;
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
                            let text = self
                                .bytes
                                .get(self.at..self.at + 4)
                                .ok_or("Bad Unicode escape in JSON")?;
                            let text = std::str::from_utf8(text)
                                .map_err(|_| "Bad Unicode escape in JSON".to_string())?;
                            let unit = u16::from_str_radix(text, 16)
                                .map_err(|_| "Bad Unicode escape in JSON".to_string())?;
                            self.at += 4;
                            units.push(unit);
                        }
                        _ => return Err("Bad escaped character in JSON".to_string()),
                    }
                }
                byte if byte < 0x20 => {
                    return Err("Bad control character in JSON string".to_string())
                }
                _ => {
                    // Copy one whole UTF-8 scalar and re-encode it as UTF-16.
                    let start = self.at;
                    self.at += 1;
                    while self.bytes.get(self.at).is_some_and(|b| (b & 0xc0) == 0x80) {
                        self.at += 1;
                    }
                    let text = std::str::from_utf8(&self.bytes[start..self.at])
                        .map_err(|_| "Bad UTF-8 in JSON string".to_string())?;
                    units.extend(text.encode_utf16());
                }
            }
        }
    }

    fn number(&mut self) -> Result<Value, String> {
        let start = self.at;
        if self.peek() == Some(b'-') {
            self.at += 1;
        }
        let digits_start = self.at;
        while self.peek().is_some_and(|b| b.is_ascii_digit()) {
            self.at += 1;
        }
        if self.at == digits_start {
            return Err(self.unexpected());
        }
        if self.bytes[digits_start] == b'0' && self.at - digits_start > 1 {
            return Err(format!("Unexpected number at position {digits_start}"));
        }
        if self.peek() == Some(b'.') {
            self.at += 1;
            let fraction = self.at;
            while self.peek().is_some_and(|b| b.is_ascii_digit()) {
                self.at += 1;
            }
            if self.at == fraction {
                return Err(self.unexpected());
            }
        }
        if matches!(self.peek(), Some(b'e') | Some(b'E')) {
            self.at += 1;
            if matches!(self.peek(), Some(b'+') | Some(b'-')) {
                self.at += 1;
            }
            let exponent = self.at;
            while self.peek().is_some_and(|b| b.is_ascii_digit()) {
                self.at += 1;
            }
            if self.at == exponent {
                return Err(self.unexpected());
            }
        }
        let text = std::str::from_utf8(&self.bytes[start..self.at]).unwrap();
        text.parse::<f64>()
            .map(Value::Num)
            .map_err(|_| format!("Unexpected number at position {start}"))
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn canonical_form_matches_the_typescript_reflow() {
        let value = Value::obj(vec![
            ("format", Value::Num(1.0)),
            ("kind", Value::Str("k".into())),
            (
                "segments",
                Value::Arr(vec![Value::obj(vec![
                    ("values", Value::Arr(vec![Value::Num(0.0), Value::Num(1.0)])),
                    ("empty", Value::Arr(vec![])),
                ])]),
            ),
        ]);
        assert_eq!(
            canonical_json(&value),
            "{\n  \"format\": 1,\n  \"kind\": \"k\",\n  \"segments\": [\n    {\n      \"values\": [0, 1],\n      \"empty\": []\n    }\n  ]\n}"
        );
    }

    #[test]
    fn duplicate_keys_overwrite_without_moving() {
        let value = parse("{\"a\":1,\"b\":2,\"a\":3}").unwrap();
        assert_eq!(
            value.own_keys(),
            vec!["a".to_string(), "b".to_string()],
            "the repeated key must keep its first position"
        );
        assert_eq!(value.get("a").unwrap().as_num(), Some(3.0));
    }

    #[test]
    fn arrays_expose_their_indices_as_own_keys() {
        let value = parse("[7, 8]").unwrap();
        assert_eq!(value.own_keys(), vec!["0".to_string(), "1".to_string()]);
    }

    #[test]
    fn parser_rejects_what_json_parse_rejects() {
        for text in ["{a:1}", "[1,]", "01", "{\"a\":1} x", "'x'", "[1"] {
            assert!(parse(text).is_err(), "{text} should not parse");
        }
        assert!(parse("  {\"a\" : [ 1.5e-3 , null ] }  ").is_ok());
    }

    #[test]
    fn integral_numbers_print_without_a_decimal_point() {
        assert_eq!(number_text(1.0), "1");
        assert_eq!(number_text(-0.0), "0");
        assert_eq!(number_text(1.5), "1.5");
    }
}
