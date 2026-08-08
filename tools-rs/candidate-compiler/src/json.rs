//! An insertion-ordered JSON value, a reader for `report.json`, and the port of
//! `tools/lib/canonical_json.ts`.
//!
//! NO `HashMap` ANYWHERE. Object key order is the output format here: the
//! canonical writer emits keys in `Object.keys` order, which for a JavaScript
//! object literal is insertion order. A hash map would produce a different
//! report on every run and no test would notice until a diff appeared in the
//! tracked tree.

use crate::jsnum::to_js_number_string;

#[derive(Debug, Clone, PartialEq)]
pub enum Json {
    Null,
    Bool(bool),
    /// Only integral values ever reach the writer; see
    /// [`crate::jsnum::to_js_number_string`].
    Number(f64),
    String(String),
    Array(Vec<Json>),
    /// A `Vec` of pairs, not a map. Duplicate keys are possible in arbitrary
    /// input JSON and JavaScript's `JSON.parse` keeps the last one, which
    /// [`parse`] reproduces; the ordering guarantee is what the `Vec` is for.
    Object(Vec<(String, Json)>),
}

impl Json {
    pub fn get(&self, key: &str) -> Option<&Json> {
        match self {
            Json::Object(entries) => entries.iter().find(|(k, _)| k == key).map(|(_, v)| v),
            _ => None,
        }
    }

    fn is_primitive(&self) -> bool {
        matches!(
            self,
            Json::Null | Json::Bool(_) | Json::Number(_) | Json::String(_)
        )
    }

    /// ECMAScript `ToNumber`, as applied by `Number(row.entry)`.
    ///
    /// PORT NOTE -- this is the single most consequential JavaScript coercion
    /// in the file. `Number(undefined)` is `NaN`, so a `report.json` row that
    /// omits `size` poisons the span and, through `Math.max`, the whole score.
    /// See [`crate::jsnum::compare_tuple`]. `Number(null)` is `0`,
    /// `Number(true)` is `1`, `Number("")` is `0`, `Number("0x10")` is `16` and
    /// `Number("12abc")` is `NaN` -- the StringNumericLiteral grammar, which
    /// `f64::from_str` does not implement (it accepts `inf`, `nan` and `1_0`,
    /// and rejects `0x10` and the empty string).
    pub fn to_js_number(value: Option<&Json>) -> f64 {
        match value {
            None => f64::NAN, // `undefined`
            Some(Json::Null) => 0.0,
            Some(Json::Bool(b)) => {
                if *b {
                    1.0
                } else {
                    0.0
                }
            }
            Some(Json::Number(n)) => *n,
            Some(Json::String(s)) => string_to_number(s),
            // `Number([])` is 0, `Number([7])` is 7, `Number([1,2])` is NaN,
            // `Number({})` is NaN. Reproduced because `report.json` is
            // untyped input and nothing validates it.
            Some(Json::Array(items)) => match items.len() {
                0 => 0.0,
                1 => Json::to_js_number(Some(&items[0])),
                _ => f64::NAN,
            },
            Some(Json::Object(_)) => f64::NAN,
        }
    }
}

/// ECMAScript `StringToNumber`, the grammar behind `Number(someString)`.
fn string_to_number(text: &str) -> f64 {
    let trimmed = crate::jsstring::js_trim(text);
    if trimmed.is_empty() {
        return 0.0;
    }
    if let Some(rest) = trimmed
        .strip_prefix("0x")
        .or_else(|| trimmed.strip_prefix("0X"))
    {
        return radix_literal(rest, 16);
    }
    if let Some(rest) = trimmed
        .strip_prefix("0o")
        .or_else(|| trimmed.strip_prefix("0O"))
    {
        return radix_literal(rest, 8);
    }
    if let Some(rest) = trimmed
        .strip_prefix("0b")
        .or_else(|| trimmed.strip_prefix("0B"))
    {
        return radix_literal(rest, 2);
    }
    if trimmed == "Infinity" || trimmed == "+Infinity" {
        return f64::INFINITY;
    }
    if trimmed == "-Infinity" {
        return f64::NEG_INFINITY;
    }
    // `f64::from_str` accepts `inf`, `infinity`, `nan` (any case) and, on
    // recent toolchains, nothing else that the decimal grammar rejects. The
    // three words are the only over-acceptance, and they are screened here.
    let lower = trimmed.to_ascii_lowercase();
    if matches!(
        lower.as_str(),
        "inf" | "+inf" | "-inf" | "infinity" | "+infinity" | "-infinity" | "nan" | "+nan" | "-nan"
    ) {
        return f64::NAN;
    }
    if trimmed.contains('_') {
        return f64::NAN;
    }
    trimmed.parse::<f64>().unwrap_or(f64::NAN)
}

fn radix_literal(digits: &str, radix: u32) -> f64 {
    if digits.is_empty() {
        return f64::NAN;
    }
    let mut acc = 0.0f64;
    for c in digits.chars() {
        match c.to_digit(radix) {
            // `to_digit` is ASCII-only for radix <= 36, matching the grammar.
            Some(d) => acc = acc * f64::from(radix) + f64::from(d),
            None => return f64::NAN,
        }
    }
    acc
}

// ---------------------------------------------------------------------------
// canonical_json.ts
// ---------------------------------------------------------------------------

/// `canonicalJson(value)`: objects one key per line, arrays of primitives
/// inline on one line.
pub fn canonical_json(value: &Json) -> Result<String, String> {
    reflow(value, "")
}

fn reflow(value: &Json, indent: &str) -> Result<String, String> {
    if value.is_primitive() {
        return stringify_primitive(value);
    }
    match value {
        Json::Array(items) => {
            if items.is_empty() {
                return Ok("[]".to_string());
            }
            if items.iter().all(Json::is_primitive) {
                let mut parts = Vec::with_capacity(items.len());
                for item in items {
                    parts.push(stringify_primitive(item)?);
                }
                return Ok(format!("[{}]", parts.join(", ")));
            }
            let inner = format!("{indent}  ");
            let mut parts = Vec::with_capacity(items.len());
            for item in items {
                parts.push(format!("{inner}{}", reflow(item, &inner)?));
            }
            Ok(format!("[\n{}\n{indent}]", parts.join(",\n")))
        }
        Json::Object(entries) => {
            if entries.is_empty() {
                return Ok("{}".to_string());
            }
            let inner = format!("{indent}  ");
            let mut parts = Vec::with_capacity(entries.len());
            for (key, item) in entries {
                parts.push(format!("{inner}{}: {}", quote(key), reflow(item, &inner)?));
            }
            Ok(format!("{{\n{}\n{indent}}}", parts.join(",\n")))
        }
        _ => unreachable!("primitives handled above"),
    }
}

fn stringify_primitive(value: &Json) -> Result<String, String> {
    Ok(match value {
        Json::Null => "null".to_string(),
        Json::Bool(true) => "true".to_string(),
        Json::Bool(false) => "false".to_string(),
        Json::Number(n) => to_js_number_string(*n)?,
        Json::String(s) => quote(s),
        _ => return Err("not a primitive".to_string()),
    })
}

/// `JSON.stringify` of a string.
///
/// PORT NOTE -- JavaScript escapes only `"`, `\` and the C0 controls, using the
/// short forms `\b \t \n \f \r` where they exist and `\uXXXX` otherwise. It
/// does NOT escape DEL, non-ASCII, or the line separators U+2028/U+2029 (that
/// is `JSON.stringify` since ES2019: they are emitted literally, being valid
/// JSON). Lone surrogates are emitted as `\udXXX`, which cannot arise from a
/// Rust `String`.
fn quote(text: &str) -> String {
    let mut out = String::with_capacity(text.len() + 2);
    out.push('"');
    for c in text.chars() {
        match c {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\u{8}' => out.push_str("\\b"),
            '\u{9}' => out.push_str("\\t"),
            '\u{A}' => out.push_str("\\n"),
            '\u{C}' => out.push_str("\\f"),
            '\u{D}' => out.push_str("\\r"),
            c if (c as u32) < 0x20 => out.push_str(&format!("\\u{:04x}", c as u32)),
            c => out.push(c),
        }
    }
    out.push('"');
    out
}

// ---------------------------------------------------------------------------
// JSON.parse
// ---------------------------------------------------------------------------

/// `JSON.parse`. Strict RFC 8259 with JavaScript's duplicate-key rule
/// (last wins, but the key keeps its FIRST position, matching V8 and
/// JavaScriptCore property-order semantics).
pub fn parse(text: &str) -> Result<Json, String> {
    let bytes: Vec<char> = text.chars().collect();
    let mut parser = Parser {
        chars: &bytes,
        index: 0,
    };
    parser.skip_ws();
    let value = parser.value()?;
    parser.skip_ws();
    if parser.index != parser.chars.len() {
        return Err(format!("Unexpected token at position {}", parser.index));
    }
    Ok(value)
}

struct Parser<'a> {
    chars: &'a [char],
    index: usize,
}

impl Parser<'_> {
    fn peek(&self) -> Option<char> {
        self.chars.get(self.index).copied()
    }

    /// JSON's own whitespace set is only these four, NOT the `\s` set above.
    fn skip_ws(&mut self) {
        while matches!(self.peek(), Some(' ' | '\t' | '\n' | '\r')) {
            self.index += 1;
        }
    }

    fn expect(&mut self, c: char) -> Result<(), String> {
        if self.peek() == Some(c) {
            self.index += 1;
            Ok(())
        } else {
            Err(format!("Expected '{c}' at position {}", self.index))
        }
    }

    fn value(&mut self) -> Result<Json, String> {
        match self.peek() {
            None => Err("Unexpected end of JSON input".to_string()),
            Some('{') => self.object(),
            Some('[') => self.array(),
            Some('"') => Ok(Json::String(self.string()?)),
            Some('t') => self.literal("true", Json::Bool(true)),
            Some('f') => self.literal("false", Json::Bool(false)),
            Some('n') => self.literal("null", Json::Null),
            Some(_) => self.number(),
        }
    }

    fn literal(&mut self, word: &str, value: Json) -> Result<Json, String> {
        for c in word.chars() {
            self.expect(c)?;
        }
        Ok(value)
    }

    fn object(&mut self) -> Result<Json, String> {
        self.expect('{')?;
        let mut entries: Vec<(String, Json)> = Vec::new();
        self.skip_ws();
        if self.peek() == Some('}') {
            self.index += 1;
            return Ok(Json::Object(entries));
        }
        loop {
            self.skip_ws();
            let key = self.string()?;
            self.skip_ws();
            self.expect(':')?;
            self.skip_ws();
            let value = self.value()?;
            match entries.iter_mut().find(|(k, _)| *k == key) {
                Some(slot) => slot.1 = value,
                None => entries.push((key, value)),
            }
            self.skip_ws();
            match self.peek() {
                Some(',') => self.index += 1,
                Some('}') => {
                    self.index += 1;
                    return Ok(Json::Object(entries));
                }
                _ => return Err(format!("Expected ',' or '}}' at position {}", self.index)),
            }
        }
    }

    fn array(&mut self) -> Result<Json, String> {
        self.expect('[')?;
        let mut items = Vec::new();
        self.skip_ws();
        if self.peek() == Some(']') {
            self.index += 1;
            return Ok(Json::Array(items));
        }
        loop {
            self.skip_ws();
            items.push(self.value()?);
            self.skip_ws();
            match self.peek() {
                Some(',') => self.index += 1,
                Some(']') => {
                    self.index += 1;
                    return Ok(Json::Array(items));
                }
                _ => return Err(format!("Expected ',' or ']' at position {}", self.index)),
            }
        }
    }

    fn string(&mut self) -> Result<String, String> {
        self.expect('"')?;
        let mut out = String::new();
        loop {
            let c = self.peek().ok_or("Unterminated string in JSON")?;
            self.index += 1;
            match c {
                '"' => return Ok(out),
                '\\' => {
                    let e = self.peek().ok_or("Unterminated escape in JSON")?;
                    self.index += 1;
                    match e {
                        '"' => out.push('"'),
                        '\\' => out.push('\\'),
                        '/' => out.push('/'),
                        'b' => out.push('\u{8}'),
                        'f' => out.push('\u{C}'),
                        'n' => out.push('\n'),
                        'r' => out.push('\r'),
                        't' => out.push('\t'),
                        'u' => {
                            let unit = self.hex4()?;
                            // Surrogate pairing, so `😀` becomes one
                            // char rather than two replacement characters.
                            if (0xd800..0xdc00).contains(&unit)
                                && self.chars.get(self.index) == Some(&'\\')
                                && self.chars.get(self.index + 1) == Some(&'u')
                            {
                                let save = self.index;
                                self.index += 2;
                                let low = self.hex4()?;
                                if (0xdc00..0xe000).contains(&low) {
                                    let combined =
                                        0x10000 + ((unit - 0xd800) << 10) + (low - 0xdc00);
                                    out.push(char::from_u32(combined).unwrap_or('\u{fffd}'));
                                    continue;
                                }
                                self.index = save;
                            }
                            out.push(char::from_u32(unit).unwrap_or('\u{fffd}'));
                        }
                        other => return Err(format!("Bad Unicode escape '\\{other}'")),
                    }
                }
                c if (c as u32) < 0x20 => {
                    return Err(format!(
                        "Bad control character in string at position {}",
                        self.index
                    ))
                }
                c => out.push(c),
            }
        }
    }

    fn hex4(&mut self) -> Result<u32, String> {
        let mut value = 0u32;
        for _ in 0..4 {
            let c = self.peek().ok_or("Truncated Unicode escape")?;
            let digit = c.to_digit(16).ok_or("Bad Unicode escape")?;
            value = value * 16 + digit;
            self.index += 1;
        }
        Ok(value)
    }

    fn number(&mut self) -> Result<Json, String> {
        let start = self.index;
        if self.peek() == Some('-') {
            self.index += 1;
        }
        while matches!(self.peek(), Some('0'..='9')) {
            self.index += 1;
        }
        if self.peek() == Some('.') {
            self.index += 1;
            while matches!(self.peek(), Some('0'..='9')) {
                self.index += 1;
            }
        }
        if matches!(self.peek(), Some('e' | 'E')) {
            self.index += 1;
            if matches!(self.peek(), Some('+' | '-')) {
                self.index += 1;
            }
            while matches!(self.peek(), Some('0'..='9')) {
                self.index += 1;
            }
        }
        if self.index == start {
            return Err(format!("Unexpected token at position {start}"));
        }
        let text: String = self.chars[start..self.index].iter().collect();
        text.parse::<f64>()
            .map(Json::Number)
            .map_err(|_| format!("Unexpected number: {text}"))
    }
}
