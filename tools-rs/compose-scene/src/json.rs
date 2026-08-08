// A read-only, insertion-ordered JSON value with a `JSON.parse`-shaped reader.
//
// WHY this is here instead of a path dependency on tools-rs/canonical-json or
// serde_json: canonical-json declares `version.workspace` / `edition.workspace`
// and therefore only builds from inside tools-rs/Cargo.toml's workspace, while
// this crate carries a bare `[workspace]` line so it builds concurrently with
// its siblings. tools-rs/discover/src/json.rs solved the same problem the same
// way; this file is the reader half discover never needed, since compose_scene
// only ever *reads* JSON (map headers, the manifest, the loader table, object
// plans) and never writes any.
//
// PORT NOTE: object entries are a `Vec` of pairs, not a map. `Map`/object
// iteration order is load-bearing in this port -- `graphics_catalog` walks the
// manifest in file order -- and a duplicate key overwrites in place while
// keeping its original position, exactly as a JS object property does.

use std::fmt;

#[derive(Clone, Debug, PartialEq)]
pub enum Json {
    Null,
    Bool(bool),
    Num(f64),
    Str(String),
    Arr(Vec<Json>),
    Obj(Vec<(String, Json)>),
}

#[derive(Debug)]
pub struct JsonError(pub String);

impl fmt::Display for JsonError {
    fn fmt(&self, formatter: &mut fmt::Formatter<'_>) -> fmt::Result {
        formatter.write_str(&self.0)
    }
}

impl Json {
    /// Property access. Missing properties are `None`, which stands in for
    /// JavaScript's `undefined`.
    pub fn get(&self, key: &str) -> Option<&Json> {
        match self {
            Json::Obj(entries) => {
                entries.iter().find(|(name, _)| name == key).map(|(_, value)| value)
            }
            _ => None,
        }
    }

    /// Element access, clamping like JS rather than panicking like `arr[n]`.
    pub fn at(&self, index: usize) -> Option<&Json> {
        match self {
            Json::Arr(items) => items.get(index),
            _ => None,
        }
    }

    pub fn as_array(&self) -> Option<&[Json]> {
        match self {
            Json::Arr(items) => Some(items),
            _ => None,
        }
    }

    /// `?? []` over an array-valued property, which is how the TypeScript
    /// tolerates an absent `series` / `records` / `objects` list.
    pub fn array_or_empty(&self, key: &str) -> &[Json] {
        self.get(key).and_then(Json::as_array).unwrap_or(&[])
    }

    pub fn as_str(&self) -> Option<&str> {
        match self {
            Json::Str(text) => Some(text),
            _ => None,
        }
    }

    pub fn as_f64(&self) -> Option<f64> {
        match self {
            Json::Num(value) => Some(*value),
            _ => None,
        }
    }

    pub fn is_null(&self) -> bool {
        matches!(self, Json::Null)
    }

    /// ECMAScript `ToNumber`, restricted to the values `JSON.parse` produces.
    ///
    /// PORT NOTE: `null` is 0 and `true` is 1, which is why `Number(x)` in the
    /// TypeScript never throws on a malformed header -- it produces a value
    /// that then fails the explicit range check instead. Arrays and objects
    /// take the `ToPrimitive` path; an empty array is 0 and a one-element array
    /// is its element's number, everything else is NaN.
    pub fn to_number(&self) -> f64 {
        match self {
            Json::Null => 0.0,
            Json::Bool(true) => 1.0,
            Json::Bool(false) => 0.0,
            Json::Num(value) => *value,
            Json::Str(text) => export_asset::js_number(text),
            Json::Arr(items) => match items.len() {
                0 => 0.0,
                1 => items[0].to_number(),
                _ => f64::NAN,
            },
            Json::Obj(_) => f64::NAN,
        }
    }

    /// ECMAScript `String(x)` for the values `JSON.parse` produces. Only the
    /// scalar cases are reachable from the tracked tree (`String(row.container)`
    /// on a JSON string); the container cases follow `Array.prototype.join` and
    /// `Object.prototype.toString` so a malformed loader table degrades the
    /// same way in both implementations.
    pub fn to_js_string(&self) -> String {
        match self {
            Json::Null => "null".to_string(),
            Json::Bool(value) => value.to_string(),
            Json::Num(value) => crate::js::number_to_string(*value),
            Json::Str(text) => text.clone(),
            Json::Arr(items) => items
                .iter()
                .map(|item| if item.is_null() { String::new() } else { item.to_js_string() })
                .collect::<Vec<_>>()
                .join(","),
            Json::Obj(_) => "[object Object]".to_string(),
        }
    }
}

pub fn parse(text: &str) -> Result<Json, JsonError> {
    let units: Vec<char> = text.chars().collect();
    let mut parser = Parser { units, index: 0 };
    parser.skip_space();
    let value = parser.value()?;
    parser.skip_space();
    if parser.index != parser.units.len() {
        return Err(JsonError("Unexpected non-whitespace character after JSON".into()));
    }
    Ok(value)
}

struct Parser {
    units: Vec<char>,
    index: usize,
}

impl Parser {
    fn peek(&self) -> Option<char> {
        self.units.get(self.index).copied()
    }

    /// PORT NOTE: `JSON.parse` accepts only these four as whitespace -- it is a
    /// far smaller set than either `\s` or Rust's `char::is_whitespace`, so a
    /// JSON document padded with U+00A0 is a syntax error in both languages.
    fn skip_space(&mut self) {
        while matches!(self.peek(), Some('\t' | '\n' | '\r' | ' ')) {
            self.index += 1;
        }
    }

    fn expect(&mut self, character: char) -> Result<(), JsonError> {
        if self.peek() == Some(character) {
            self.index += 1;
            return Ok(());
        }
        Err(JsonError(format!("Expected '{character}' in JSON at position {}", self.index)))
    }

    fn literal(&mut self, word: &str) -> Result<(), JsonError> {
        for character in word.chars() {
            self.expect(character)?;
        }
        Ok(())
    }

    fn value(&mut self) -> Result<Json, JsonError> {
        match self.peek() {
            Some('{') => self.object(),
            Some('[') => self.array(),
            Some('"') => Ok(Json::Str(self.string()?)),
            Some('t') => {
                self.literal("true")?;
                Ok(Json::Bool(true))
            }
            Some('f') => {
                self.literal("false")?;
                Ok(Json::Bool(false))
            }
            Some('n') => {
                self.literal("null")?;
                Ok(Json::Null)
            }
            Some(_) => self.number(),
            None => Err(JsonError("Unexpected end of JSON input".into())),
        }
    }

    fn object(&mut self) -> Result<Json, JsonError> {
        self.expect('{')?;
        let mut entries: Vec<(String, Json)> = Vec::new();
        self.skip_space();
        if self.peek() == Some('}') {
            self.index += 1;
            return Ok(Json::Obj(entries));
        }
        loop {
            self.skip_space();
            let key = self.string()?;
            self.skip_space();
            self.expect(':')?;
            self.skip_space();
            let value = self.value()?;
            // A duplicate key overwrites in place, keeping the first
            // occurrence's position, exactly as assigning to a JS object does.
            match entries.iter_mut().find(|(name, _)| *name == key) {
                Some(slot) => slot.1 = value,
                None => entries.push((key, value)),
            }
            self.skip_space();
            match self.peek() {
                Some(',') => self.index += 1,
                Some('}') => {
                    self.index += 1;
                    return Ok(Json::Obj(entries));
                }
                _ => return Err(JsonError("Expected ',' or '}' in JSON object".into())),
            }
        }
    }

    fn array(&mut self) -> Result<Json, JsonError> {
        self.expect('[')?;
        let mut items = Vec::new();
        self.skip_space();
        if self.peek() == Some(']') {
            self.index += 1;
            return Ok(Json::Arr(items));
        }
        loop {
            self.skip_space();
            items.push(self.value()?);
            self.skip_space();
            match self.peek() {
                Some(',') => self.index += 1,
                Some(']') => {
                    self.index += 1;
                    return Ok(Json::Arr(items));
                }
                _ => return Err(JsonError("Expected ',' or ']' in JSON array".into())),
            }
        }
    }

    fn string(&mut self) -> Result<String, JsonError> {
        self.expect('"')?;
        // Escapes are decoded into UTF-16 units first so that a surrogate pair
        // written as two \u escapes recombines, which is what JSON.parse does.
        let mut units: Vec<u16> = Vec::new();
        loop {
            let character = self.peek().ok_or_else(|| JsonError("Unterminated string in JSON".into()))?;
            self.index += 1;
            match character {
                '"' => break,
                '\\' => {
                    let escape =
                        self.peek().ok_or_else(|| JsonError("Unterminated escape in JSON".into()))?;
                    self.index += 1;
                    let unit = match escape {
                        '"' => 0x22,
                        '\\' => 0x5c,
                        '/' => 0x2f,
                        'b' => 0x08,
                        'f' => 0x0c,
                        'n' => 0x0a,
                        'r' => 0x0d,
                        't' => 0x09,
                        'u' => {
                            let mut value = 0u16;
                            for _ in 0..4 {
                                let digit = self
                                    .peek()
                                    .and_then(|c| c.to_digit(16))
                                    .ok_or_else(|| JsonError("Bad Unicode escape in JSON".into()))?;
                                self.index += 1;
                                value = value * 16 + digit as u16;
                            }
                            value
                        }
                        other => {
                            return Err(JsonError(format!("Bad escaped character '{other}' in JSON")))
                        }
                    };
                    units.push(unit);
                }
                control if (control as u32) < 0x20 => {
                    return Err(JsonError("Bad control character in JSON string".into()))
                }
                other => {
                    let mut buffer = [0u16; 2];
                    units.extend_from_slice(other.encode_utf16(&mut buffer));
                }
            }
        }
        Ok(String::from_utf16_lossy(&units))
    }

    fn number(&mut self) -> Result<Json, JsonError> {
        let start = self.index;
        if self.peek() == Some('-') {
            self.index += 1;
        }
        let digits_start = self.index;
        while matches!(self.peek(), Some('0'..='9')) {
            self.index += 1;
        }
        if self.index == digits_start {
            return Err(JsonError(format!("Unexpected token in JSON at position {start}")));
        }
        // JSON forbids a leading zero followed by more digits.
        if self.units[digits_start] == '0' && self.index - digits_start > 1 {
            return Err(JsonError("Leading zero in JSON number".into()));
        }
        if self.peek() == Some('.') {
            self.index += 1;
            let fraction_start = self.index;
            while matches!(self.peek(), Some('0'..='9')) {
                self.index += 1;
            }
            if self.index == fraction_start {
                return Err(JsonError("Missing fraction digits in JSON number".into()));
            }
        }
        if matches!(self.peek(), Some('e' | 'E')) {
            self.index += 1;
            if matches!(self.peek(), Some('+' | '-')) {
                self.index += 1;
            }
            let exponent_start = self.index;
            while matches!(self.peek(), Some('0'..='9')) {
                self.index += 1;
            }
            if self.index == exponent_start {
                return Err(JsonError("Missing exponent digits in JSON number".into()));
            }
        }
        let text: String = self.units[start..self.index].iter().collect();
        text.parse::<f64>()
            .map(Json::Num)
            .map_err(|_| JsonError(format!("Unparseable JSON number {text}")))
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn reads_the_shapes_the_asset_tree_actually_contains() {
        let value = parse(
            r#"{ "format": 1, "parameters": [0, 0, 64, 64], "name_ja": "黄金門",
                 "flag": true, "gone": null, "ratio": -1.5e-3 }"#,
        )
        .unwrap();
        assert_eq!(value.get("format").unwrap().as_f64(), Some(1.0));
        assert_eq!(value.array_or_empty("parameters").len(), 4);
        assert_eq!(value.get("name_ja").unwrap().as_str(), Some("黄金門"));
        assert_eq!(value.get("flag"), Some(&Json::Bool(true)));
        assert!(value.get("gone").unwrap().is_null());
        assert_eq!(value.get("ratio").unwrap().as_f64(), Some(-0.0015));
        assert!(value.get("absent").is_none());
    }

    #[test]
    fn duplicate_keys_overwrite_in_place_like_a_js_object() {
        let value = parse(r#"{"a": 1, "b": 2, "a": 3}"#).unwrap();
        match value {
            Json::Obj(entries) => {
                assert_eq!(entries.len(), 2);
                assert_eq!(entries[0].0, "a");
                assert_eq!(entries[0].1, Json::Num(3.0));
                assert_eq!(entries[1].0, "b");
            }
            other => panic!("expected an object, got {other:?}"),
        }
    }

    #[test]
    fn surrogate_pairs_written_as_two_escapes_recombine() {
        let value = parse(r#""😀""#).unwrap();
        assert_eq!(value.as_str(), Some("😀"));
    }

    #[test]
    fn rejects_what_json_parse_rejects() {
        for bad in [
            "{",
            "[1,]",
            "01",
            "1.",
            "1e",
            "\"unterminated",
            "{\"a\" 1}",
            "1 2",
            "nul",
            // U+00A0 is \s but is not JSON whitespace.
            "\u{a0}1",
        ] {
            assert!(parse(bad).is_err(), "{bad:?} should not parse");
        }
    }

    #[test]
    fn to_number_follows_tonumber_not_from_str() {
        assert_eq!(Json::Null.to_number(), 0.0);
        assert_eq!(Json::Bool(true).to_number(), 1.0);
        assert_eq!(Json::Str("0x10".into()).to_number(), 16.0);
        assert_eq!(Json::Str(String::new()).to_number(), 0.0);
        assert!(Json::Str("12abc".into()).to_number().is_nan());
        assert!(Json::Str("inf".into()).to_number().is_nan());
        assert!(Json::Obj(Vec::new()).to_number().is_nan());
    }

    #[test]
    fn to_js_string_uses_ecmascript_number_formatting() {
        assert_eq!(Json::Num(1.0).to_js_string(), "1");
        assert_eq!(Json::Num(1.5).to_js_string(), "1.5");
        assert_eq!(Json::Str("152".into()).to_js_string(), "152");
        assert_eq!(Json::Null.to_js_string(), "null");
    }
}
