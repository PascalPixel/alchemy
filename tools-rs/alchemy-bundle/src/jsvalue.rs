//! A small object-literal parser for TypeScript source.
//!
//! WHY THIS EXISTS. The routing crate's drift check compares flat
//! `Set`/`Map` literals by splitting on commas, which is enough for a list of
//! quoted strings. It is NOT enough for `EXPECTED`, the three-level nested
//! literal in `tools/lib/alchemy_gcc.ts` whose keys are bare identifiers rather
//! than strings -- which is exactly why the digest tables shipped for months
//! with an honest note saying they were hand-synced with no gate. Closing that
//! gap needs a parser that understands nesting, both key spellings, comments,
//! and the non-string expressions (`join(BUNDLE, "xgcc")`) that appear in the
//! signature path list.
//!
//! It is deliberately not a JavaScript parser. It reads the value grammar these
//! tables actually use and preserves anything else as raw source text, so a
//! change to an expression still shows up as drift rather than being silently
//! normalised away.
//!
//! RETIREMENT: delete with `crate::drift` when `tools/lib/alchemy_gcc.ts` is
//! deleted.

/// A parsed value. `Raw` holds source text for anything that is not an object,
/// array or string literal -- identifiers, call expressions, numbers.
#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Value {
    /// A quoted string, with its delimiters stripped and no escape processing.
    /// These tables contain hex digests, flags and paths; none carries an
    /// escape, and a literal that grew one would show up as drift, which is the
    /// correct outcome for a check whose whole job is to notice change.
    Str(String),
    /// A template literal, kept verbatim INCLUDING its backticks. `${...}`
    /// interpolation cannot be evaluated here, so the text is compared instead.
    Template(String),
    /// Source text, whitespace-collapsed.
    Raw(String),
    Array(Vec<Value>),
    /// Insertion-ordered. Object key order is load-bearing: `validateBundle`
    /// walks `Object.entries` and stats the executables in that order.
    Object(Vec<(String, Value)>),
}

impl Value {
    /// Flatten to the ordered sequence of rendered leaves. An object
    /// contributes each key immediately before its value's leaves, so a
    /// reordering, a rename or a moved digest all show as a sequence
    /// difference.
    pub fn flatten(&self) -> Vec<String> {
        let mut out = Vec::new();
        self.flatten_into(&mut out);
        out
    }

    fn flatten_into(&self, out: &mut Vec<String>) {
        match self {
            Value::Str(text) => out.push(text.clone()),
            Value::Template(text) | Value::Raw(text) => out.push(text.clone()),
            Value::Array(items) => {
                for item in items {
                    item.flatten_into(out);
                }
            }
            Value::Object(entries) => {
                for (key, value) in entries {
                    out.push(key.clone());
                    value.flatten_into(out);
                }
            }
        }
    }
}

pub struct Parser<'a> {
    bytes: &'a [u8],
    text: &'a str,
    at: usize,
}

impl<'a> Parser<'a> {
    pub fn new(text: &'a str, at: usize) -> Self {
        Parser {
            bytes: text.as_bytes(),
            text,
            at,
        }
    }

    pub fn position(&self) -> usize {
        self.at
    }

    /// Skip whitespace, `//` line comments and `/* */` block comments.
    ///
    /// PORT NOTE: JS `\s` and Rust's `char::is_whitespace` disagree at the
    /// edges (U+FEFF is whitespace to JS and not to Rust; U+0085 the reverse).
    /// Neither appears in this file and neither can appear between tokens
    /// without a syntax error in the TypeScript, so ASCII whitespace is the
    /// whole set that matters here.
    fn skip_trivia(&mut self) {
        loop {
            while self.at < self.bytes.len() && self.bytes[self.at].is_ascii_whitespace() {
                self.at += 1;
            }
            if self.bytes[self.at..].starts_with(b"//") {
                while self.at < self.bytes.len() && self.bytes[self.at] != b'\n' {
                    self.at += 1;
                }
                continue;
            }
            if self.bytes[self.at..].starts_with(b"/*") {
                self.at += 2;
                while self.at < self.bytes.len() && !self.bytes[self.at..].starts_with(b"*/") {
                    self.at += 1;
                }
                self.at = (self.at + 2).min(self.bytes.len());
                continue;
            }
            return;
        }
    }

    fn peek(&self) -> Option<u8> {
        self.bytes.get(self.at).copied()
    }

    /// Advance past a quoted or template literal that starts at `self.at`,
    /// returning its full source text including delimiters.
    fn read_quoted(&mut self) -> Option<&'a str> {
        let open = self.peek()?;
        if open != b'"' && open != b'\'' && open != b'`' {
            return None;
        }
        let start = self.at;
        self.at += 1;
        while self.at < self.bytes.len() {
            let byte = self.bytes[self.at];
            if byte == b'\\' {
                self.at += 2;
                continue;
            }
            if byte == open {
                self.at += 1;
                return Some(&self.text[start..self.at]);
            }
            self.at += 1;
        }
        None
    }

    pub fn parse_value(&mut self) -> Option<Value> {
        self.skip_trivia();
        match self.peek()? {
            b'{' => self.parse_object(),
            b'[' => self.parse_array(),
            b'"' | b'\'' => {
                let raw = self.read_quoted()?;
                Some(Value::Str(raw[1..raw.len() - 1].to_string()))
            }
            b'`' => {
                let raw = self.read_quoted()?;
                Some(Value::Template(raw.to_string()))
            }
            _ => self.parse_raw(),
        }
    }

    /// Anything else: read source text up to the next `,`, `}` or `]` that is
    /// not nested inside brackets, parens or a string.
    fn parse_raw(&mut self) -> Option<Value> {
        let start = self.at;
        let mut depth = 0i32;
        while self.at < self.bytes.len() {
            match self.bytes[self.at] {
                b'"' | b'\'' | b'`' => {
                    self.read_quoted()?;
                    continue;
                }
                b'(' | b'[' | b'{' => depth += 1,
                b')' | b']' | b'}' if depth > 0 => depth -= 1,
                b',' | b')' | b']' | b'}' if depth == 0 => break,
                _ => {}
            }
            self.at += 1;
        }
        if self.at == start {
            return None;
        }
        Some(Value::Raw(collapse(&self.text[start..self.at])))
    }

    fn parse_array(&mut self) -> Option<Value> {
        self.at += 1; // '['
        let mut items = Vec::new();
        loop {
            self.skip_trivia();
            match self.peek()? {
                b']' => {
                    self.at += 1;
                    return Some(Value::Array(items));
                }
                b',' => {
                    self.at += 1;
                }
                _ => items.push(self.parse_value()?),
            }
        }
    }

    fn parse_object(&mut self) -> Option<Value> {
        self.at += 1; // '{'
        let mut entries = Vec::new();
        loop {
            self.skip_trivia();
            match self.peek()? {
                b'}' => {
                    self.at += 1;
                    return Some(Value::Object(entries));
                }
                b',' => {
                    self.at += 1;
                }
                _ => {
                    let key = self.parse_key()?;
                    self.skip_trivia();
                    if self.peek()? != b':' {
                        return None;
                    }
                    self.at += 1;
                    entries.push((key, self.parse_value()?));
                }
            }
        }
    }

    /// A key is a quoted string or a bare identifier. The bare-identifier case
    /// is precisely why the routing crate's string-sequence check could not
    /// cover these tables.
    fn parse_key(&mut self) -> Option<String> {
        self.skip_trivia();
        match self.peek()? {
            b'"' | b'\'' => {
                let raw = self.read_quoted()?;
                Some(raw[1..raw.len() - 1].to_string())
            }
            _ => {
                let start = self.at;
                while self.at < self.bytes.len() {
                    let byte = self.bytes[self.at];
                    // JS `\w` is ASCII-only; identifiers here are too.
                    if byte.is_ascii_alphanumeric() || byte == b'_' || byte == b'$' {
                        self.at += 1;
                    } else {
                        break;
                    }
                }
                if self.at == start {
                    None
                } else {
                    Some(self.text[start..self.at].to_string())
                }
            }
        }
    }
}

/// Collapse every run of ASCII whitespace to a single space and trim, so that a
/// reflow of the TypeScript is not reported as drift while a token change is.
pub fn collapse(text: &str) -> String {
    let mut out = String::with_capacity(text.len());
    let mut spaced = false;
    for character in text.chars() {
        if character.is_ascii_whitespace() {
            spaced = true;
            continue;
        }
        if spaced && !out.is_empty() {
            out.push(' ');
        }
        spaced = false;
        out.push(character);
    }
    out
}

/// Every string, template and raw leaf of the declaration named `name`, in
/// source order. `name` is matched as `const <name>` or `let <name>` at the
/// start of a line; the value begins at the first `=` after it.
pub fn declaration(source: &str, name: &str) -> Option<Value> {
    let at = find_declaration(source, name)?;
    Parser::new(source, at).parse_value()
}

fn find_declaration(source: &str, name: &str) -> Option<usize> {
    for keyword in ["const ", "let ", "export const "] {
        let needle = format!("{keyword}{name}");
        let mut from = 0;
        while let Some(found) = source[from..].find(&needle) {
            let at = from + found;
            let line_start = source[..at].rfind('\n').map_or(0, |index| index + 1);
            if source[line_start..at].trim().is_empty() {
                // The value starts after the first `=` that is not part of the
                // type annotation. Type annotations here contain no `=`.
                let equals = source[at..].find('=')? + at;
                return Some(equals + 1);
            }
            from = at + needle.len();
        }
    }
    None
}

/// Source text of `function <name>(` through its matching closing brace.
pub fn function_body<'a>(source: &'a str, name: &str) -> Option<&'a str> {
    let at = source.find(&format!("function {name}("))?;
    let open = source[at..].find('{')? + at;
    let bytes = source.as_bytes();
    let mut index = open;
    let mut depth = 0i32;
    let mut parser = Parser::new(source, 0);
    while index < bytes.len() {
        match bytes[index] {
            b'"' | b'\'' | b'`' => {
                parser.at = index;
                parser.read_quoted()?;
                index = parser.at;
                continue;
            }
            b'/' if source[index..].starts_with("//") => {
                while index < bytes.len() && bytes[index] != b'\n' {
                    index += 1;
                }
                continue;
            }
            b'{' => depth += 1,
            b'}' => {
                depth -= 1;
                if depth == 0 {
                    return Some(&source[open..=index]);
                }
            }
            _ => {}
        }
        index += 1;
    }
    None
}

/// Every quoted or template literal in `text`, in order, WITH delimiters.
/// Comments are skipped so a commented-out example never counts.
pub fn string_literals(text: &str) -> Vec<String> {
    let mut parser = Parser::new(text, 0);
    let mut out = Vec::new();
    while parser.at < parser.bytes.len() {
        let before = parser.at;
        parser.skip_trivia();
        if parser.at >= parser.bytes.len() {
            break;
        }
        match parser.peek() {
            Some(b'"') | Some(b'\'') | Some(b'`') => {
                if let Some(raw) = parser.read_quoted() {
                    out.push(raw.to_string());
                } else {
                    break;
                }
            }
            _ => parser.at += 1,
        }
        if parser.at == before {
            parser.at += 1;
        }
    }
    out
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn parses_bare_identifier_keys_and_nesting() {
        let source = "const T: X = {\n  \"a\": { gs1: { xgcc: [\"d1\", \"d2\"] } },\n};\n";
        let value = declaration(source, "T").expect("parses");
        assert_eq!(value.flatten(), vec!["a", "gs1", "xgcc", "d1", "d2"]);
    }

    #[test]
    fn skips_comments_including_ones_holding_quotes() {
        let source = "const T = [\n  // \"ghost\",\n  \"real\", /* \"also ghost\" */ \"second\",\n];";
        let value = declaration(source, "T").expect("parses");
        assert_eq!(value.flatten(), vec!["real", "second"]);
    }

    #[test]
    fn keeps_call_expressions_as_raw_text() {
        let source = "const T = [ join(BUNDLE, \"xgcc\"), AGBCC_DRIVER ];";
        let value = declaration(source, "T").expect("parses");
        assert_eq!(
            value.flatten(),
            vec!["join(BUNDLE, \"xgcc\")", "AGBCC_DRIVER"]
        );
    }

    #[test]
    fn empty_array_flattens_to_nothing_but_the_key_remains() {
        let source = "const T = { \"darwin-x64\": [] };";
        let value = declaration(source, "T").expect("parses");
        assert_eq!(value.flatten(), vec!["darwin-x64"]);
    }

    #[test]
    fn string_literals_survive_a_backtick_inside_a_quoted_string() {
        let literals = string_literals("[\"run the full `bun run verify`, and pin\", \"next\"]");
        assert_eq!(
            literals,
            vec![
                "\"run the full `bun run verify`, and pin\"".to_string(),
                "\"next\"".to_string()
            ]
        );
    }

    #[test]
    fn a_missing_declaration_is_none_not_an_empty_value() {
        // A vanished table must never read as agreement with an empty mirror.
        assert!(declaration("const OTHER = [];", "T").is_none());
    }

    #[test]
    fn collapse_normalises_reflow_only() {
        assert_eq!(collapse("  join(BUNDLE,\n    \"cc1\")  "), "join(BUNDLE, \"cc1\")");
    }
}
