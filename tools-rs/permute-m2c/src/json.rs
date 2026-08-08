//! A small insertion-ordered JSON reader.
//!
//! PORT NOTE (trap: `HashMap` destroys a load-bearing order). Object members
//! are a `Vec` of pairs. `permute_m2c.ts` does not print JSON, but it does read
//! `asm/classification.json` and the match report, and duplicate keys resolve
//! last-wins in `JSON.parse` while `HashMap` would resolve them the same way by
//! accident and lose the order besides. Keeping the vector makes both explicit.
//!
//! PORT NOTE: this is deliberately not `serde_json`. The crate has zero
//! third-party dependencies, and `serde_json` without `preserve_order` would
//! reorder object members.

use crate::jsnum::js_number;

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
    /// Member lookup. Later duplicate keys win, as `JSON.parse` does.
    pub fn get(&self, key: &str) -> Option<&Value> {
        match self {
            Value::Object(members) => members
                .iter()
                .rev()
                .find(|(name, _)| name == key)
                .map(|(_, value)| value),
            _ => None,
        }
    }

    /// `value ?? fallback` semantics: `null` and "absent" both fall through.
    pub fn nullish(&self) -> bool {
        matches!(self, Value::Null)
    }

    /// ECMAScript `ToNumber`, which is what the `-` in the sort comparator
    /// applies.
    ///
    /// PORT NOTE (trap: NaN poisoning). `ToNumber([])` is `0`, `ToNumber([5])`
    /// is `5`, `ToNumber(["a"])` is NaN, `ToNumber({})` is NaN, `ToNumber(true)`
    /// is `1`. A NaN reaching the comparator makes it return NaN, which the
    /// specification coerces to `+0`, i.e. "equal" -- which is how a whole
    /// ordering can quietly stop ordering while still reporting a result.
    pub fn to_number(&self) -> f64 {
        match self {
            Value::Null => 0.0,
            Value::Bool(true) => 1.0,
            Value::Bool(false) => 0.0,
            Value::Number(value) => *value,
            Value::String(text) => js_number(text),
            Value::Array(items) => match items.len() {
                0 => 0.0,
                1 => match &items[0] {
                    Value::Array(_) | Value::Object(_) => f64::NAN,
                    other => other.to_number(),
                },
                _ => f64::NAN,
            },
            Value::Object(_) => f64::NAN,
        }
    }

    /// `!value`: JavaScript falsiness.
    pub fn truthy(&self) -> bool {
        match self {
            Value::Null => false,
            Value::Bool(value) => *value,
            // PORT NOTE (trap: `||` and `!` treat NaN as falsy). `0`, `-0` and
            // NaN are all falsy numbers.
            Value::Number(value) => *value != 0.0 && !value.is_nan(),
            Value::String(text) => !text.is_empty(),
            Value::Array(_) | Value::Object(_) => true,
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
}

/// `JSON.parse`. Rejects trailing content, as JavaScript does.
pub fn parse(text: &str) -> Result<Value, String> {
    let bytes: Vec<char> = text.chars().collect();
    let mut cursor = Cursor { chars: &bytes, at: 0 };
    cursor.skip_whitespace();
    let value = cursor.value()?;
    cursor.skip_whitespace();
    if cursor.at != cursor.chars.len() {
        return Err(format!("unexpected trailing content at {}", cursor.at));
    }
    Ok(value)
}

struct Cursor<'a> {
    chars: &'a [char],
    at: usize,
}

impl Cursor<'_> {
    fn peek(&self) -> Option<char> {
        self.chars.get(self.at).copied()
    }

    fn skip_whitespace(&mut self) {
        while matches!(self.peek(), Some(' ') | Some('\t') | Some('\n') | Some('\r')) {
            self.at += 1;
        }
    }

    fn expect(&mut self, c: char) -> Result<(), String> {
        if self.peek() == Some(c) {
            self.at += 1;
            Ok(())
        } else {
            Err(format!("expected {c:?} at {}", self.at))
        }
    }

    fn word(&mut self, word: &str) -> Result<(), String> {
        for c in word.chars() {
            self.expect(c)?;
        }
        Ok(())
    }

    fn value(&mut self) -> Result<Value, String> {
        match self.peek() {
            Some('{') => self.object(),
            Some('[') => self.array(),
            Some('"') => Ok(Value::String(self.string()?)),
            Some('t') => {
                self.word("true")?;
                Ok(Value::Bool(true))
            }
            Some('f') => {
                self.word("false")?;
                Ok(Value::Bool(false))
            }
            Some('n') => {
                self.word("null")?;
                Ok(Value::Null)
            }
            Some(_) => self.number(),
            None => Err("unexpected end of input".to_string()),
        }
    }

    fn object(&mut self) -> Result<Value, String> {
        self.expect('{')?;
        let mut members = Vec::new();
        self.skip_whitespace();
        if self.peek() == Some('}') {
            self.at += 1;
            return Ok(Value::Object(members));
        }
        loop {
            self.skip_whitespace();
            let key = self.string()?;
            self.skip_whitespace();
            self.expect(':')?;
            self.skip_whitespace();
            let value = self.value()?;
            members.push((key, value));
            self.skip_whitespace();
            match self.peek() {
                Some(',') => self.at += 1,
                Some('}') => {
                    self.at += 1;
                    return Ok(Value::Object(members));
                }
                _ => return Err(format!("expected ',' or '}}' at {}", self.at)),
            }
        }
    }

    fn array(&mut self) -> Result<Value, String> {
        self.expect('[')?;
        let mut items = Vec::new();
        self.skip_whitespace();
        if self.peek() == Some(']') {
            self.at += 1;
            return Ok(Value::Array(items));
        }
        loop {
            self.skip_whitespace();
            items.push(self.value()?);
            self.skip_whitespace();
            match self.peek() {
                Some(',') => self.at += 1,
                Some(']') => {
                    self.at += 1;
                    return Ok(Value::Array(items));
                }
                _ => return Err(format!("expected ',' or ']' at {}", self.at)),
            }
        }
    }

    fn string(&mut self) -> Result<String, String> {
        self.expect('"')?;
        let mut out = String::new();
        loop {
            let c = self.peek().ok_or("unterminated string")?;
            self.at += 1;
            match c {
                '"' => return Ok(out),
                '\\' => {
                    let escape = self.peek().ok_or("unterminated escape")?;
                    self.at += 1;
                    match escape {
                        '"' => out.push('"'),
                        '\\' => out.push('\\'),
                        '/' => out.push('/'),
                        'b' => out.push('\u{0008}'),
                        'f' => out.push('\u{000C}'),
                        'n' => out.push('\n'),
                        'r' => out.push('\r'),
                        't' => out.push('\t'),
                        'u' => {
                            let unit = self.hex4()?;
                            // Surrogate pairs, as JavaScript rebuilds them.
                            if (0xD800..0xDC00).contains(&unit)
                                && self.peek() == Some('\\')
                                && self.chars.get(self.at + 1) == Some(&'u')
                            {
                                let mark = self.at;
                                self.at += 2;
                                let low = self.hex4()?;
                                if (0xDC00..0xE000).contains(&low) {
                                    let combined = 0x10000
                                        + ((u32::from(unit) - 0xD800) << 10)
                                        + (u32::from(low) - 0xDC00);
                                    out.push(char::from_u32(combined).ok_or("bad surrogate")?);
                                } else {
                                    self.at = mark;
                                    out.push(char::REPLACEMENT_CHARACTER);
                                }
                            } else {
                                out.push(
                                    char::from_u32(u32::from(unit))
                                        .unwrap_or(char::REPLACEMENT_CHARACTER),
                                );
                            }
                        }
                        other => return Err(format!("bad escape \\{other}")),
                    }
                }
                other => out.push(other),
            }
        }
    }

    fn hex4(&mut self) -> Result<u16, String> {
        let mut value: u16 = 0;
        for _ in 0..4 {
            let c = self.peek().ok_or("truncated \\u escape")?;
            self.at += 1;
            let digit = c.to_digit(16).ok_or("bad \\u escape")?;
            value = value * 16 + digit as u16;
        }
        Ok(value)
    }

    fn number(&mut self) -> Result<Value, String> {
        let start = self.at;
        if self.peek() == Some('-') {
            self.at += 1;
        }
        while matches!(self.peek(), Some(c) if c.is_ascii_digit()) {
            self.at += 1;
        }
        if self.peek() == Some('.') {
            self.at += 1;
            while matches!(self.peek(), Some(c) if c.is_ascii_digit()) {
                self.at += 1;
            }
        }
        if matches!(self.peek(), Some('e') | Some('E')) {
            self.at += 1;
            if matches!(self.peek(), Some('+') | Some('-')) {
                self.at += 1;
            }
            while matches!(self.peek(), Some(c) if c.is_ascii_digit()) {
                self.at += 1;
            }
        }
        if self.at == start {
            return Err(format!("expected a value at {start}"));
        }
        let text: String = self.chars[start..self.at].iter().collect();
        text.parse::<f64>()
            .map(Value::Number)
            .map_err(|_| format!("bad number {text}"))
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn objects_keep_insertion_order() {
        let value = parse(r#"{"b":1,"a":2,"c":3}"#).expect("parses");
        let Value::Object(members) = &value else {
            panic!("expected an object");
        };
        let keys: Vec<&str> = members.iter().map(|(name, _)| name.as_str()).collect();
        assert_eq!(keys, vec!["b", "a", "c"]);
    }

    #[test]
    fn duplicate_keys_resolve_last_wins() {
        let value = parse(r#"{"a":1,"a":2}"#).expect("parses");
        assert_eq!(value.get("a"), Some(&Value::Number(2.0)));
    }

    #[test]
    fn to_number_reproduces_js_coercion() {
        assert_eq!(Value::Array(vec![]).to_number(), 0.0);
        assert_eq!(Value::Array(vec![Value::Number(5.0)]).to_number(), 5.0);
        assert!(Value::Array(vec![Value::String("a".into())]).to_number().is_nan());
        assert!(Value::Object(vec![]).to_number().is_nan());
        assert_eq!(Value::Bool(true).to_number(), 1.0);
        assert_eq!(Value::Null.to_number(), 0.0);
        assert_eq!(Value::String("  12  ".into()).to_number(), 12.0);
    }

    #[test]
    fn nan_is_falsy() {
        assert!(!Value::Number(f64::NAN).truthy());
        assert!(!Value::Number(0.0).truthy());
        assert!(Value::Number(1.0).truthy());
        assert!(!Value::String(String::new()).truthy());
    }

    #[test]
    fn trailing_content_is_rejected() {
        assert!(parse("[] []").is_err());
        assert!(parse("[1,2]").is_ok());
    }

    #[test]
    fn surrogate_pairs_round_trip() {
        let value = parse(r#""😀""#).expect("parses");
        assert_eq!(value.as_str(), Some("\u{1F600}"));
    }
}
