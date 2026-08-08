// A minimal, insertion-ordered JSON value with a `JSON.parse`-shaped reader
// and a `JSON.stringify(value, null, 2)`-shaped writer.
//
// WHY this is here and not a path dependency: `tools/canonical-json`
// declares `version.workspace`, so it only resolves from inside
// `tools/Cargo.toml`'s workspace; this crate carries a bare `[workspace]`
// line so it can build concurrently with its siblings and therefore cannot
// depend on it. `tools/discover/src/json.rs` inlines the same thing for the
// same reason. This variant additionally needs a READER, and one that keeps
// object key order, because `--json` output must byte-match the TypeScript.
//
// PORT NOTE: numbers are kept as f64 exactly as `JSON.parse` produces them.
// Nothing here re-serialises a parsed float, and every number this crate
// EMITS is an integer (asserted by a test that walks the emitted document), so
// the ECMAScript `ToString(Number)` algorithm is deliberately not implemented
// — an integer path is enough and a partial float formatter would be a trap.

use std::fmt::Write as _;

#[derive(Debug, Clone, PartialEq)]
pub enum Json {
    Null,
    Bool(bool),
    Num(f64),
    Str(String),
    Arr(Vec<Json>),
    Obj(Vec<(String, Json)>),
}

impl Json {
    pub fn get(&self, key: &str) -> Option<&Json> {
        match self {
            Json::Obj(entries) => entries.iter().find(|(name, _)| name == key).map(|(_, value)| value),
            _ => None,
        }
    }

    /// `value.key` where a missing key yields `undefined`. The original leans
    /// on `undefined` propagating into `??` and `typeof` checks, so absence and
    /// an explicit `null` must stay distinguishable: absence is `None`.
    pub fn as_array(&self) -> Option<&[Json]> {
        match self {
            Json::Arr(items) => Some(items),
            _ => None,
        }
    }

    pub fn as_str(&self) -> Option<&str> {
        match self {
            Json::Str(text) => Some(text),
            _ => None,
        }
    }
}

pub fn parse(text: &str) -> Result<Json, String> {
    let bytes: Vec<char> = text.chars().collect();
    let mut reader = Reader { chars: &bytes, index: 0 };
    reader.skip_space();
    let value = reader.value()?;
    reader.skip_space();
    if reader.index != reader.chars.len() {
        return Err("JSON Parse error: Unexpected content at end of input".into());
    }
    Ok(value)
}

struct Reader<'a> {
    chars: &'a [char],
    index: usize,
}

impl Reader<'_> {
    fn peek(&self) -> Option<char> {
        self.chars.get(self.index).copied()
    }

    fn skip_space(&mut self) {
        while matches!(self.peek(), Some(' ') | Some('\t') | Some('\n') | Some('\r')) {
            self.index += 1;
        }
    }

    fn expect(&mut self, expected: char) -> Result<(), String> {
        if self.peek() == Some(expected) {
            self.index += 1;
            Ok(())
        } else {
            Err(format!("JSON Parse error: Expected '{expected}'"))
        }
    }

    fn literal(&mut self, word: &str) -> Result<(), String> {
        for expected in word.chars() {
            self.expect(expected)?;
        }
        Ok(())
    }

    fn value(&mut self) -> Result<Json, String> {
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
            None => Err("JSON Parse error: Unexpected end of input".into()),
        }
    }

    fn object(&mut self) -> Result<Json, String> {
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
            // PORT NOTE: a duplicate key in JS overwrites IN PLACE, keeping the
            // original key's position in iteration order. Same here.
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
                _ => return Err("JSON Parse error: Expected ',' or '}'".into()),
            }
        }
    }

    fn array(&mut self) -> Result<Json, String> {
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
                _ => return Err("JSON Parse error: Expected ',' or ']'".into()),
            }
        }
    }

    fn string(&mut self) -> Result<String, String> {
        self.expect('"')?;
        let mut text = String::new();
        loop {
            let current = self.peek().ok_or("JSON Parse error: Unterminated string")?;
            self.index += 1;
            match current {
                '"' => return Ok(text),
                '\\' => {
                    let escape = self.peek().ok_or("JSON Parse error: Unterminated string")?;
                    self.index += 1;
                    match escape {
                        '"' => text.push('"'),
                        '\\' => text.push('\\'),
                        '/' => text.push('/'),
                        'b' => text.push('\u{8}'),
                        'f' => text.push('\u{c}'),
                        'n' => text.push('\n'),
                        'r' => text.push('\r'),
                        't' => text.push('\t'),
                        'u' => {
                            let unit = self.hex4()?;
                            // Surrogate pairs. A lone surrogate is replaced,
                            // which is where this diverges from JS's UTF-16
                            // strings; no input here contains one.
                            if (0xd800..0xdc00).contains(&unit) && self.peek() == Some('\\') {
                                let save = self.index;
                                self.index += 1;
                                if self.peek() == Some('u') {
                                    self.index += 1;
                                    let low = self.hex4()?;
                                    if (0xdc00..0xe000).contains(&low) {
                                        let combined =
                                            0x10000 + ((unit - 0xd800) << 10) + (low - 0xdc00);
                                        text.push(char::from_u32(combined).unwrap_or('\u{fffd}'));
                                        continue;
                                    }
                                }
                                self.index = save;
                            }
                            text.push(char::from_u32(unit).unwrap_or('\u{fffd}'));
                        }
                        other => return Err(format!("JSON Parse error: Invalid escape '{other}'")),
                    }
                }
                other => text.push(other),
            }
        }
    }

    fn hex4(&mut self) -> Result<u32, String> {
        let mut value = 0u32;
        for _ in 0..4 {
            let digit = self
                .peek()
                .and_then(|c| c.to_digit(16))
                .ok_or("JSON Parse error: Invalid unicode escape")?;
            self.index += 1;
            value = value * 16 + digit;
        }
        Ok(value)
    }

    fn number(&mut self) -> Result<Json, String> {
        let start = self.index;
        if self.peek() == Some('-') {
            self.index += 1;
        }
        while matches!(self.peek(), Some(c) if c.is_ascii_digit() || c == '.' || c == 'e' || c == 'E' || c == '+' || c == '-')
        {
            self.index += 1;
        }
        let text: String = self.chars[start..self.index].iter().collect();
        text.parse::<f64>()
            .map(Json::Num)
            .map_err(|_| format!("JSON Parse error: Unexpected token '{text}'"))
    }
}

/// `JSON.stringify(value, null, 2)`.
pub fn stringify_pretty(value: &Json) -> String {
    let mut out = String::new();
    write_value(value, 0, &mut out);
    out
}

fn write_value(value: &Json, depth: usize, out: &mut String) {
    match value {
        Json::Null => out.push_str("null"),
        Json::Bool(true) => out.push_str("true"),
        Json::Bool(false) => out.push_str("false"),
        Json::Num(number) => write_number(*number, out),
        Json::Str(text) => escape(text, out),
        Json::Arr(items) if items.is_empty() => out.push_str("[]"),
        Json::Arr(items) => {
            out.push_str("[\n");
            for (index, item) in items.iter().enumerate() {
                out.push_str(&"  ".repeat(depth + 1));
                write_value(item, depth + 1, out);
                if index + 1 < items.len() {
                    out.push(',');
                }
                out.push('\n');
            }
            out.push_str(&"  ".repeat(depth));
            out.push(']');
        }
        Json::Obj(entries) if entries.is_empty() => out.push_str("{}"),
        Json::Obj(entries) => {
            out.push_str("{\n");
            for (index, (key, item)) in entries.iter().enumerate() {
                out.push_str(&"  ".repeat(depth + 1));
                escape(key, out);
                out.push_str(": ");
                write_value(item, depth + 1, out);
                if index + 1 < entries.len() {
                    out.push(',');
                }
                out.push('\n');
            }
            out.push_str(&"  ".repeat(depth));
            out.push('}');
        }
    }
}

/// PORT NOTE: only the integer path is implemented. `serde_json`/ryu would
/// print `1.0` where JavaScript prints `1`, and would never reach the
/// exponential form JS uses below 1e-6, so a non-integer here is a bug in the
/// caller rather than something to format approximately — it panics loudly.
fn write_number(number: f64, out: &mut String) {
    if !number.is_finite() {
        // `JSON.stringify(NaN)` and `JSON.stringify(Infinity)` are both `null`.
        out.push_str("null");
        return;
    }
    assert!(
        number.fract() == 0.0 && number.abs() <= 9_007_199_254_740_991.0,
        "core-retained-audit emits only safe integers; got {number}"
    );
    let _ = write!(out, "{}", number as i64);
}

fn escape(text: &str, out: &mut String) {
    out.push('"');
    for character in text.chars() {
        match character {
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
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn parses_the_shapes_the_manifests_actually_use() {
        let document = parse(r#"{"format":1,"regions":[{"address":"0x08002344","size":512}],"e":[]}"#)
            .expect("parse");
        assert_eq!(document.get("format"), Some(&Json::Num(1.0)));
        let regions = document.get("regions").unwrap().as_array().unwrap();
        assert_eq!(regions[0].get("address").unwrap().as_str(), Some("0x08002344"));
        assert_eq!(document.get("e").unwrap().as_array().unwrap().len(), 0);
        assert!(document.get("absent").is_none());
    }

    #[test]
    fn duplicate_keys_overwrite_in_place_keeping_the_first_position() {
        let document = parse(r#"{"a":1,"b":2,"a":3}"#).expect("parse");
        match document {
            Json::Obj(entries) => {
                assert_eq!(entries.iter().map(|(k, _)| k.as_str()).collect::<Vec<_>>(), ["a", "b"]);
                assert_eq!(entries[0].1, Json::Num(3.0));
            }
            _ => panic!("expected object"),
        }
    }

    #[test]
    fn pretty_output_matches_the_two_space_stringify_shape() {
        let value = Json::Obj(vec![
            ("format".into(), Json::Num(1.0)),
            ("rows".into(), Json::Arr(vec![Json::Obj(vec![("k".into(), Json::Str("a\"b".into()))])])),
            ("empty".into(), Json::Arr(vec![])),
            ("nothing".into(), Json::Obj(vec![])),
        ]);
        assert_eq!(
            stringify_pretty(&value),
            "{\n  \"format\": 1,\n  \"rows\": [\n    {\n      \"k\": \"a\\\"b\"\n    }\n  ],\n  \"empty\": [],\n  \"nothing\": {}\n}"
        );
    }

    #[test]
    fn integers_print_without_a_trailing_point() {
        assert_eq!(stringify_pretty(&Json::Num(1.0)), "1");
        assert_eq!(stringify_pretty(&Json::Num(548364.0)), "548364");
    }

    #[test]
    fn malformed_input_is_an_error_rather_than_a_panic() {
        assert!(parse("{").is_err());
        assert!(parse("{\"a\":1,}").is_err());
        assert!(parse("1 2").is_err());
        assert!(parse("").is_err());
    }
}
