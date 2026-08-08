// Pure core of the batch front-end for the compiler configuration explorer
// (tools/search/search_compiler_modes.ts).
//
// WHY this file exists: the TypeScript batch driver is a thin shell around
// four things that are easy to get subtly wrong when translated —
//   * argument parsing that uses JavaScript `Number()` coercion, not
//     `f64::from_str`, so `--limit 0x10` is 16 and `--limit 12abc` is NaN;
//   * the `out/modesweep/<basename>-<sha256[..16]>` directory identity, which
//     is a repository-wide contract shared with mode_sweep.ts and must be
//     byte-identical or the driver reads the wrong report;
//   * order-preserving JSON, because the summary embeds each match's `config`
//     object verbatim and the canonical writer emits one key per line in
//     insertion order;
//   * the canonical JSON text itself, which is compared byte-for-byte by the
//     repository's checkers.
// Keeping all four here, away from process spawning and threads, means they
// can be tested against assembled fixtures instead of against a live compiler.

use std::path::{Component, Path};

// ---------------------------------------------------------------------------
// JSON values
// ---------------------------------------------------------------------------

/// An order-preserving JSON value.
///
/// PORT NOTE: objects are a `Vec` of pairs, never a `HashMap`. JavaScript
/// object iteration is insertion order and that order is load-bearing: the
/// `config` records copied out of each mode-sweep report are re-emitted key by
/// key, and canonical JSON puts one key per line.
#[derive(Debug, Clone, PartialEq)]
pub enum Json {
    Null,
    Bool(bool),
    Number(f64),
    String(String),
    Array(Vec<Json>),
    Object(Vec<(String, Json)>),
}

impl Json {
    pub fn get(&self, key: &str) -> Option<&Json> {
        match self {
            Json::Object(entries) => entries
                .iter()
                .find(|(name, _)| name == key)
                .map(|(_, value)| value),
            _ => None,
        }
    }

    pub fn as_str(&self) -> Option<&str> {
        match self {
            Json::String(text) => Some(text),
            _ => None,
        }
    }

    pub fn as_array(&self) -> Option<&[Json]> {
        match self {
            Json::Array(items) => Some(items),
            _ => None,
        }
    }

    /// JavaScript truthiness. `undefined` is modelled by `None` at the call
    /// site, so only the in-band falsy values are handled here: `null`,
    /// `false`, `0`/`-0`/`NaN`, and the empty string. Empty arrays and objects
    /// are truthy, as in JavaScript.
    pub fn truthy(&self) -> bool {
        match self {
            Json::Null => false,
            Json::Bool(value) => *value,
            Json::Number(value) => *value != 0.0 && !value.is_nan(),
            Json::String(text) => !text.is_empty(),
            Json::Array(_) | Json::Object(_) => true,
        }
    }
}

/// Optional-chaining helper: `value?.key` where `value` may be absent.
fn optional_get<'a>(value: Option<&'a Json>, key: &str) -> Option<&'a Json> {
    match value {
        None | Some(Json::Null) => None,
        Some(other) => other.get(key),
    }
}

// ---------------------------------------------------------------------------
// JSON parsing
// ---------------------------------------------------------------------------

pub fn parse_json(text: &str) -> Result<Json, String> {
    let bytes: Vec<char> = text.chars().collect();
    let mut cursor = 0usize;
    skip_whitespace(&bytes, &mut cursor);
    let value = parse_value(&bytes, &mut cursor)?;
    skip_whitespace(&bytes, &mut cursor);
    if cursor != bytes.len() {
        return Err(format!("Unexpected token at position {cursor} in JSON"));
    }
    Ok(value)
}

fn skip_whitespace(chars: &[char], cursor: &mut usize) {
    while *cursor < chars.len() && matches!(chars[*cursor], ' ' | '\t' | '\n' | '\r') {
        *cursor += 1;
    }
}

fn parse_value(chars: &[char], cursor: &mut usize) -> Result<Json, String> {
    match chars.get(*cursor) {
        None => Err("Unexpected end of JSON input".to_string()),
        Some('{') => parse_object(chars, cursor),
        Some('[') => parse_array(chars, cursor),
        Some('"') => Ok(Json::String(parse_string(chars, cursor)?)),
        Some('t') => parse_literal(chars, cursor, "true", Json::Bool(true)),
        Some('f') => parse_literal(chars, cursor, "false", Json::Bool(false)),
        Some('n') => parse_literal(chars, cursor, "null", Json::Null),
        Some(_) => parse_number(chars, cursor),
    }
}

fn parse_literal(
    chars: &[char],
    cursor: &mut usize,
    word: &str,
    value: Json,
) -> Result<Json, String> {
    for expected in word.chars() {
        if chars.get(*cursor) != Some(&expected) {
            return Err(format!("Unexpected token at position {cursor} in JSON"));
        }
        *cursor += 1;
    }
    Ok(value)
}

fn parse_number(chars: &[char], cursor: &mut usize) -> Result<Json, String> {
    let start = *cursor;
    if chars.get(*cursor) == Some(&'-') {
        *cursor += 1;
    }
    while matches!(chars.get(*cursor), Some(c) if c.is_ascii_digit()) {
        *cursor += 1;
    }
    if chars.get(*cursor) == Some(&'.') {
        *cursor += 1;
        while matches!(chars.get(*cursor), Some(c) if c.is_ascii_digit()) {
            *cursor += 1;
        }
    }
    if matches!(chars.get(*cursor), Some('e') | Some('E')) {
        *cursor += 1;
        if matches!(chars.get(*cursor), Some('+') | Some('-')) {
            *cursor += 1;
        }
        while matches!(chars.get(*cursor), Some(c) if c.is_ascii_digit()) {
            *cursor += 1;
        }
    }
    let text: String = chars[start..*cursor].iter().collect();
    text.parse::<f64>()
        .map(Json::Number)
        .map_err(|_| format!("Unexpected token at position {start} in JSON"))
}

fn parse_string(chars: &[char], cursor: &mut usize) -> Result<String, String> {
    *cursor += 1; // opening quote
    let mut out = String::new();
    let mut pending_high: Option<u32> = None;
    loop {
        let character = *chars
            .get(*cursor)
            .ok_or_else(|| "Unterminated string in JSON".to_string())?;
        *cursor += 1;
        if character == '"' {
            break;
        }
        if character != '\\' {
            if pending_high.take().is_some() {
                out.push('\u{fffd}');
            }
            out.push(character);
            continue;
        }
        let escape = *chars
            .get(*cursor)
            .ok_or_else(|| "Unterminated string in JSON".to_string())?;
        *cursor += 1;
        let decoded = match escape {
            '"' => '"',
            '\\' => '\\',
            '/' => '/',
            'b' => '\u{8}',
            'f' => '\u{c}',
            'n' => '\n',
            'r' => '\r',
            't' => '\t',
            'u' => {
                let mut code = 0u32;
                for _ in 0..4 {
                    let digit = chars
                        .get(*cursor)
                        .and_then(|c| c.to_digit(16))
                        .ok_or_else(|| "Bad Unicode escape in JSON".to_string())?;
                    code = code * 16 + digit;
                    *cursor += 1;
                }
                // PORT NOTE: JavaScript strings are UTF-16 and can hold lone
                // surrogates; Rust strings cannot. Pairs are combined, and a
                // lone surrogate becomes U+FFFD — the same substitution
                // `readFileSync(..., "utf8")` would already have applied to
                // malformed input.
                if let Some(high) = pending_high.take() {
                    if (0xdc00..0xe000).contains(&code) {
                        let combined =
                            0x10000 + ((high - 0xd800) << 10) + (code - 0xdc00);
                        out.push(char::from_u32(combined).unwrap_or('\u{fffd}'));
                        continue;
                    }
                    out.push('\u{fffd}');
                }
                if (0xd800..0xdc00).contains(&code) {
                    pending_high = Some(code);
                    continue;
                }
                char::from_u32(code).unwrap_or('\u{fffd}')
            }
            other => return Err(format!("Bad escape \\{other} in JSON")),
        };
        if pending_high.take().is_some() {
            out.push('\u{fffd}');
        }
        out.push(decoded);
    }
    if pending_high.take().is_some() {
        out.push('\u{fffd}');
    }
    Ok(out)
}

fn parse_array(chars: &[char], cursor: &mut usize) -> Result<Json, String> {
    *cursor += 1;
    let mut items = Vec::new();
    skip_whitespace(chars, cursor);
    if chars.get(*cursor) == Some(&']') {
        *cursor += 1;
        return Ok(Json::Array(items));
    }
    loop {
        skip_whitespace(chars, cursor);
        items.push(parse_value(chars, cursor)?);
        skip_whitespace(chars, cursor);
        match chars.get(*cursor) {
            Some(',') => *cursor += 1,
            Some(']') => {
                *cursor += 1;
                return Ok(Json::Array(items));
            }
            _ => return Err(format!("Unexpected token at position {cursor} in JSON")),
        }
    }
}

fn parse_object(chars: &[char], cursor: &mut usize) -> Result<Json, String> {
    *cursor += 1;
    let mut entries: Vec<(String, Json)> = Vec::new();
    skip_whitespace(chars, cursor);
    if chars.get(*cursor) == Some(&'}') {
        *cursor += 1;
        return Ok(Json::Object(entries));
    }
    loop {
        skip_whitespace(chars, cursor);
        if chars.get(*cursor) != Some(&'"') {
            return Err(format!("Unexpected token at position {cursor} in JSON"));
        }
        let key = parse_string(chars, cursor)?;
        skip_whitespace(chars, cursor);
        if chars.get(*cursor) != Some(&':') {
            return Err(format!("Unexpected token at position {cursor} in JSON"));
        }
        *cursor += 1;
        skip_whitespace(chars, cursor);
        let value = parse_value(chars, cursor)?;
        // PORT NOTE: a repeated key overwrites the value but keeps the
        // original insertion position, matching JavaScript object semantics.
        match entries.iter_mut().find(|(name, _)| *name == key) {
            Some(slot) => slot.1 = value,
            None => entries.push((key, value)),
        }
        skip_whitespace(chars, cursor);
        match chars.get(*cursor) {
            Some(',') => *cursor += 1,
            Some('}') => {
                *cursor += 1;
                return Ok(Json::Object(entries));
            }
            _ => return Err(format!("Unexpected token at position {cursor} in JSON")),
        }
    }
}

// ---------------------------------------------------------------------------
// JSON writing (JSON.stringify and tools/lib/canonical_json.ts)
// ---------------------------------------------------------------------------

/// `JSON.stringify` for a number: integers print without a decimal point,
/// `Infinity`/`NaN` become `null`.
pub fn js_number_text(value: f64) -> String {
    if value.is_nan() || value.is_infinite() {
        return "null".to_string();
    }
    if value == 0.0 {
        return "0".to_string();
    }
    if value.fract() == 0.0 {
        if value.abs() < 1e21 {
            return format!("{value:.0}");
        }
        // At 1e21 and above JavaScript switches to exponent form and always
        // writes the exponent's sign: `1e+21`, not Rust's `1e21`.
        let exponent = format!("{value:e}");
        return match exponent.split_once('e') {
            Some((mantissa, power)) if !power.starts_with('-') => {
                format!("{mantissa}e+{power}")
            }
            _ => exponent,
        };
    }
    // PORT NOTE: Rust's shortest round-trip float formatting agrees with
    // JavaScript's Number::toString for every value the summary can carry
    // (counts and CLI-supplied integers). Exponent-form disagreements
    // (`1e-7` vs Rust's `0.0000001`) are out of reach here because every
    // number written by this tool is a validated positive integer or a
    // count.
    format!("{value}")
}

pub fn js_string_literal(text: &str) -> String {
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
            c if (c as u32) < 0x20 => out.push_str(&format!("\\u{:04x}", c as u32)),
            c => out.push(c),
        }
    }
    out.push('"');
    out
}

fn is_primitive(value: &Json) -> bool {
    matches!(
        value,
        Json::Null | Json::Number(_) | Json::String(_) | Json::Bool(_)
    )
}

fn stringify_primitive(value: &Json) -> String {
    match value {
        Json::Null => "null".to_string(),
        Json::Bool(true) => "true".to_string(),
        Json::Bool(false) => "false".to_string(),
        Json::Number(number) => js_number_text(*number),
        Json::String(text) => js_string_literal(text),
        _ => unreachable!("stringify_primitive on a container"),
    }
}

fn reflow(value: &Json, indent: &str) -> String {
    if is_primitive(value) {
        return stringify_primitive(value);
    }
    match value {
        Json::Array(items) => {
            if items.is_empty() {
                return "[]".to_string();
            }
            if items.iter().all(is_primitive) {
                let inner: Vec<String> = items.iter().map(stringify_primitive).collect();
                return format!("[{}]", inner.join(", "));
            }
            let deeper = format!("{indent}  ");
            let inner: Vec<String> = items
                .iter()
                .map(|item| format!("{deeper}{}", reflow(item, &deeper)))
                .collect();
            format!("[\n{}\n{indent}]", inner.join(",\n"))
        }
        Json::Object(entries) => {
            if entries.is_empty() {
                return "{}".to_string();
            }
            let deeper = format!("{indent}  ");
            let inner: Vec<String> = entries
                .iter()
                .map(|(key, item)| {
                    format!(
                        "{deeper}{}: {}",
                        js_string_literal(key),
                        reflow(item, &deeper)
                    )
                })
                .collect();
            format!("{{\n{}\n{indent}}}", inner.join(",\n"))
        }
        _ => unreachable!("reflow on a primitive"),
    }
}

/// Port of `canonicalJson` from tools/lib/canonical_json.ts.
pub fn canonical_json(value: &Json) -> String {
    reflow(value, "")
}

// ---------------------------------------------------------------------------
// JavaScript Number() coercion
// ---------------------------------------------------------------------------

/// JavaScript `StrWhiteSpace`: the Unicode space separators plus tab, VT, FF,
/// NBSP, BOM, and the four line terminators.
///
/// PORT NOTE: this is deliberately not `char::is_whitespace`. JavaScript's
/// definition includes U+FEFF and excludes U+0085; Rust's `White_Space`
/// property is the other way around.
fn is_js_whitespace(character: char) -> bool {
    matches!(
        character,
        '\u{9}' | '\u{a}' | '\u{b}' | '\u{c}' | '\u{d}'
            | '\u{20}' | '\u{a0}' | '\u{feff}'
            | '\u{1680}' | '\u{2000}'..='\u{200a}'
            | '\u{2028}' | '\u{2029}' | '\u{202f}' | '\u{205f}' | '\u{3000}'
    )
}

/// `Number(text)` following the ECMAScript StringNumericLiteral grammar.
///
/// PORT NOTE: this is not `f64::from_str`. `Number("")` is 0, `Number("0x10")`
/// is 16, `Number("nan")` is NaN-because-invalid rather than the float `NaN`
/// literal Rust would accept, and `Number("12abc")` is NaN where a naive
/// prefix parse would say 12.
pub fn js_number(text: &str) -> f64 {
    let trimmed: &str = text.trim_matches(is_js_whitespace);
    if trimmed.is_empty() {
        return 0.0;
    }
    let (sign, body) = match trimmed.strip_prefix('-') {
        Some(rest) => (-1.0, rest),
        None => (1.0, trimmed.strip_prefix('+').unwrap_or(trimmed)),
    };
    if body == "Infinity" {
        return sign * f64::INFINITY;
    }
    if sign == 1.0 && trimmed.len() > 2 {
        let radix = match &trimmed[..2] {
            "0x" | "0X" => Some(16u32),
            "0o" | "0O" => Some(8),
            "0b" | "0B" => Some(2),
            _ => None,
        };
        if let Some(radix) = radix {
            let digits = &trimmed[2..];
            let mut accumulator = 0.0f64;
            for character in digits.chars() {
                match character.to_digit(radix) {
                    Some(digit) => accumulator = accumulator * f64::from(radix) + f64::from(digit),
                    None => return f64::NAN,
                }
            }
            return accumulator;
        }
    }
    // Reject anything the decimal grammar does not allow before handing the
    // text to Rust, which would otherwise accept "inf", "NaN", and "1e5f".
    if !body
        .chars()
        .all(|c| c.is_ascii_digit() || matches!(c, '.' | 'e' | 'E' | '+' | '-'))
    {
        return f64::NAN;
    }
    if body.is_empty() || body == "." {
        return f64::NAN;
    }
    match body.parse::<f64>() {
        Ok(value) => sign * value,
        Err(_) => f64::NAN,
    }
}

/// `Number.isInteger`.
///
/// PORT NOTE: JavaScript has one number type, so `1.0` is an integer here.
/// The check is on the value, not on a Rust type.
pub fn is_js_integer(value: f64) -> bool {
    value.is_finite() && value.fract() == 0.0
}

// ---------------------------------------------------------------------------
// Options
// ---------------------------------------------------------------------------

pub const USAGE: &str = "usage: search_compiler_modes.ts [options]\n  --limit N --jobs N --queue FILE --rom FILE\n  --pairs [--max-pairs N]\n  --triples [--max-triples N]";

#[derive(Debug, Clone, PartialEq)]
pub struct Options {
    pub limit: f64,
    pub jobs: f64,
    pub queue: String,
    pub rom: String,
    pub pairs: bool,
    pub triples: bool,
    pub max_pairs: f64,
    pub max_triples: f64,
}

/// What `optionsOf` can do besides returning options: `--help` prints usage
/// and calls `process.exit(0)` from inside the parse loop.
#[derive(Debug, PartialEq)]
pub enum ParseOutcome {
    Parsed(Box<Options>),
    Help,
}

pub fn default_options(root: &Path, jobs: f64) -> Options {
    Options {
        limit: 250.0,
        jobs,
        queue: join_path(root, "out/decomp/queue.json"),
        rom: join_path(root, "roms/gs1-en.gba"),
        pairs: false,
        triples: false,
        max_pairs: 256.0,
        max_triples: 64.0,
    }
}

/// Port of `optionsOf`.
///
/// PORT NOTE: when a value-taking flag is last, JavaScript reads `undefined`
/// past the end of the array. For the numeric flags `Number(undefined)` is
/// NaN, which trips the positive-integer check below with the same message;
/// that is reproduced exactly. For `--queue`/`--rom` the string becomes
/// `undefined` and the failure only surfaces later, inside `readFileSync`, so
/// the missing value is reported here instead — see `MISSING_PATH`.
pub const MISSING_PATH: &str = "\u{0}missing\u{0}";

pub fn options_of(arguments: &[String], defaults: &Options) -> Result<ParseOutcome, String> {
    let mut options = defaults.clone();
    let mut index = 0usize;
    while index < arguments.len() {
        let argument = arguments[index].as_str();
        let take = |index: &mut usize| -> Option<&str> {
            *index += 1;
            arguments.get(*index).map(String::as_str)
        };
        match argument {
            "--limit" => options.limit = take(&mut index).map_or(f64::NAN, js_number),
            "--jobs" => options.jobs = take(&mut index).map_or(f64::NAN, js_number),
            "--queue" => {
                options.queue = take(&mut index).unwrap_or(MISSING_PATH).to_string();
            }
            "--rom" => options.rom = take(&mut index).unwrap_or(MISSING_PATH).to_string(),
            "--pairs" => options.pairs = true,
            "--triples" => {
                options.pairs = true;
                options.triples = true;
            }
            "--max-pairs" => options.max_pairs = take(&mut index).map_or(f64::NAN, js_number),
            "--max-triples" => options.max_triples = take(&mut index).map_or(f64::NAN, js_number),
            "-h" | "--help" => return Ok(ParseOutcome::Help),
            other => return Err(format!("unrecognized argument: {other}")),
        }
        index += 1;
    }
    for (name, value) in [
        ("limit", options.limit),
        ("jobs", options.jobs),
        ("max-pairs", options.max_pairs),
        ("max-triples", options.max_triples),
    ] {
        if !is_js_integer(value) || value < 1.0 {
            return Err(format!("{name} must be a positive integer"));
        }
    }
    Ok(ParseOutcome::Parsed(Box::new(options)))
}

// ---------------------------------------------------------------------------
// Paths
// ---------------------------------------------------------------------------

fn join_path(base: &Path, tail: &str) -> String {
    normalize(&base.join(tail))
}

/// Node's `path.resolve(base, candidate)`: an absolute candidate wins, a
/// relative one is appended to base, and `.`/`..` are collapsed lexically
/// without touching the filesystem.
pub fn resolve_path(base: &Path, candidate: &str) -> String {
    let candidate_path = Path::new(candidate);
    if candidate_path.is_absolute() {
        normalize(candidate_path)
    } else {
        normalize(&base.join(candidate_path))
    }
}

fn normalize(path: &Path) -> String {
    let mut parts: Vec<String> = Vec::new();
    let mut absolute = false;
    for component in path.components() {
        match component {
            Component::RootDir => absolute = true,
            Component::CurDir => {}
            Component::ParentDir => {
                if parts.pop().is_none() && !absolute {
                    parts.push("..".to_string());
                }
            }
            Component::Normal(part) => parts.push(part.to_string_lossy().into_owned()),
            Component::Prefix(prefix) => parts.push(prefix.as_os_str().to_string_lossy().into_owned()),
        }
    }
    let joined = parts.join("/");
    if absolute {
        format!("/{joined}")
    } else if joined.is_empty() {
        ".".to_string()
    } else {
        joined
    }
}

/// Node's `path.relative(from, to)` restricted to the absolute-path case this
/// tool uses.
pub fn relative_path(from: &Path, to: &Path) -> String {
    let from_parts: Vec<String> = normalize(from)
        .split('/')
        .filter(|part| !part.is_empty())
        .map(str::to_string)
        .collect();
    let to_parts: Vec<String> = normalize(to)
        .split('/')
        .filter(|part| !part.is_empty())
        .map(str::to_string)
        .collect();
    let shared = from_parts
        .iter()
        .zip(to_parts.iter())
        .take_while(|(a, b)| a == b)
        .count();
    let mut parts: Vec<String> = vec!["..".to_string(); from_parts.len() - shared];
    parts.extend(to_parts[shared..].iter().cloned());
    parts.join("/")
}

// ---------------------------------------------------------------------------
// SHA-256 and the mode-sweep output directory
// ---------------------------------------------------------------------------

const SHA256_K: [u32; 64] = [
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
];

pub fn sha256_hex(input: &[u8]) -> String {
    let mut state: [u32; 8] = [
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab,
        0x5be0cd19,
    ];
    let mut message = input.to_vec();
    let bit_length = (input.len() as u64).wrapping_mul(8);
    message.push(0x80);
    while message.len() % 64 != 56 {
        message.push(0);
    }
    message.extend_from_slice(&bit_length.to_be_bytes());

    for chunk in message.chunks(64) {
        let mut schedule = [0u32; 64];
        for (index, slot) in schedule.iter_mut().enumerate().take(16) {
            let base = index * 4;
            *slot = u32::from_be_bytes([
                chunk[base],
                chunk[base + 1],
                chunk[base + 2],
                chunk[base + 3],
            ]);
        }
        for index in 16..64 {
            let s0 = schedule[index - 15].rotate_right(7)
                ^ schedule[index - 15].rotate_right(18)
                ^ (schedule[index - 15] >> 3);
            let s1 = schedule[index - 2].rotate_right(17)
                ^ schedule[index - 2].rotate_right(19)
                ^ (schedule[index - 2] >> 10);
            schedule[index] = schedule[index - 16]
                .wrapping_add(s0)
                .wrapping_add(schedule[index - 7])
                .wrapping_add(s1);
        }
        let [mut a, mut b, mut c, mut d, mut e, mut f, mut g, mut h] = state;
        for index in 0..64 {
            let s1 = e.rotate_right(6) ^ e.rotate_right(11) ^ e.rotate_right(25);
            let choose = (e & f) ^ ((!e) & g);
            let temp1 = h
                .wrapping_add(s1)
                .wrapping_add(choose)
                .wrapping_add(SHA256_K[index])
                .wrapping_add(schedule[index]);
            let s0 = a.rotate_right(2) ^ a.rotate_right(13) ^ a.rotate_right(22);
            let majority = (a & b) ^ (a & c) ^ (b & c);
            let temp2 = s0.wrapping_add(majority);
            h = g;
            g = f;
            f = e;
            e = d.wrapping_add(temp1);
            d = c;
            c = b;
            b = a;
            a = temp1.wrapping_add(temp2);
        }
        for (slot, value) in state.iter_mut().zip([a, b, c, d, e, f, g, h]) {
            *slot = slot.wrapping_add(value);
        }
    }
    state.iter().map(|word| format!("{word:08x}")).collect()
}

/// Port of `modeSweepOutputDirectory` from tools/lib/mode_sweep.ts: the
/// repository-relative POSIX path and the file bytes, each followed by a NUL,
/// hashed with SHA-256 and truncated to sixteen hex characters.
pub fn mode_sweep_output_directory(root: &Path, source: &str, contents: &[u8]) -> String {
    let absolute = resolve_path(root, source);
    let relative = relative_path(root, Path::new(&absolute));
    let mut buffer: Vec<u8> = Vec::new();
    buffer.extend_from_slice(relative.as_bytes());
    buffer.push(0);
    buffer.extend_from_slice(contents);
    buffer.push(0);
    let identity = &sha256_hex(&buffer)[..16];
    let stem = Path::new(&absolute)
        .file_name()
        .map(|name| name.to_string_lossy().into_owned())
        .unwrap_or_default();
    let stem = stem.strip_suffix(".c").unwrap_or(&stem).to_string();
    join_path(root, &format!("out/modesweep/{stem}-{identity}"))
}

// ---------------------------------------------------------------------------
// Aggregation
// ---------------------------------------------------------------------------

pub struct Match {
    pub stem: Json,
    pub source: Json,
    pub config: Json,
}

/// Port of the `matches` flatMap plus the `summary` object literal. Every
/// field is copied straight out of the parsed report so that a report with an
/// unexpected shape produces the same output as the TypeScript would.
pub fn summarize(reports: &[Json], options: &Options) -> Json {
    let mut matches: Vec<Json> = Vec::new();
    for report in reports {
        let results = report
            .get("results")
            .and_then(Json::as_array)
            .unwrap_or(&[])
            .to_vec();
        for result in results {
            let compiled = result.get("compiled").map(Json::truthy).unwrap_or(false);
            let exact = optional_get(result.get("evidence"), "exact")
                .map(Json::truthy)
                .unwrap_or(false);
            if compiled && exact {
                matches.push(Json::Object(vec![
                    ("stem".into(), pick(report, "stem")),
                    ("source".into(), pick(report, "source")),
                    ("config".into(), pick(&result, "config")),
                ]));
            }
        }
    }
    let members: Vec<Json> = reports
        .iter()
        .map(|report| {
            Json::Object(vec![
                ("stem".into(), pick(report, "stem")),
                ("source".into(), pick(report, "source")),
                ("source_sha256".into(), pick(report, "source_sha256")),
                ("reference_sha256".into(), pick(report, "reference_sha256")),
                (
                    "compiler_signature".into(),
                    pick(report, "compiler_signature"),
                ),
                (
                    "bounded_search_complete".into(),
                    report
                        .get("planning")
                        .map(|planning| pick(planning, "bounded_search_complete"))
                        .unwrap_or(Json::Null),
                ),
            ])
        })
        .collect();
    Json::Object(vec![
        ("format".into(), Json::Number(2.0)),
        ("engine".into(), Json::String("tools/lib/mode_sweep.ts".into())),
        (
            "search".into(),
            Json::Object(vec![
                ("pairs".into(), Json::Bool(options.pairs)),
                ("triples".into(), Json::Bool(options.triples)),
                ("max_pairs".into(), Json::Number(options.max_pairs)),
                ("max_triples".into(), Json::Number(options.max_triples)),
            ]),
        ),
        ("members".into(), Json::Array(members)),
        ("matches".into(), Json::Array(matches)),
        ("auto_promote".into(), Json::Bool(false)),
    ])
}

/// PORT NOTE: a missing key is `undefined` in JavaScript, and
/// `JSON.stringify` drops such a key entirely. Reports written by
/// mode_sweep.ts always carry these keys; when one is absent the port emits
/// `null` rather than silently dropping the field, which keeps the record
/// shape stable and is flagged here instead of hiding a malformed report.
fn pick(value: &Json, key: &str) -> Json {
    value.get(key).cloned().unwrap_or(Json::Null)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn defaults() -> Options {
        default_options(Path::new("/repo"), 8.0)
    }

    fn parsed(arguments: &[&str]) -> Options {
        let owned: Vec<String> = arguments.iter().map(|a| (*a).to_string()).collect();
        match options_of(&owned, &defaults()).expect("parses") {
            ParseOutcome::Parsed(options) => *options,
            ParseOutcome::Help => panic!("expected options, got help"),
        }
    }

    fn error(arguments: &[&str]) -> String {
        let owned: Vec<String> = arguments.iter().map(|a| (*a).to_string()).collect();
        options_of(&owned, &defaults()).expect_err("should fail")
    }

    // The TypeScript's own --self-test, reproduced verbatim.
    #[test]
    fn upstream_self_test_expectations_hold() {
        let options = parsed(&["--limit", "7", "--pairs", "--max-pairs", "11"]);
        assert_eq!(options.limit, 7.0);
        assert!(options.pairs);
        assert!(!options.triples);
        assert_eq!(options.max_pairs, 11.0);
        let triple = parsed(&["--triples"]);
        assert!(triple.pairs && triple.triples);
    }

    #[test]
    fn defaults_come_from_the_repository_root() {
        let options = parsed(&[]);
        assert_eq!(options.queue, "/repo/out/decomp/queue.json");
        assert_eq!(options.rom, "/repo/roms/gs1-en.gba");
        assert_eq!(options.limit, 250.0);
        assert_eq!(options.max_triples, 64.0);
    }

    #[test]
    fn unknown_arguments_stop_the_parse() {
        assert_eq!(error(&["--nope"]), "unrecognized argument: --nope");
        // A stray value is itself an unrecognized argument.
        assert_eq!(error(&["--pairs", "7"]), "unrecognized argument: 7");
    }

    #[test]
    fn help_short_circuits_before_validation() {
        let owned = vec!["--limit".to_string(), "0".to_string(), "-h".to_string()];
        assert_eq!(
            options_of(&owned, &defaults()).expect("help"),
            ParseOutcome::Help
        );
    }

    #[test]
    fn numeric_validation_reports_the_first_bad_field() {
        assert_eq!(error(&["--limit", "0"]), "limit must be a positive integer");
        assert_eq!(error(&["--jobs", "1.5"]), "jobs must be a positive integer");
        assert_eq!(
            error(&["--max-pairs", "-3"]),
            "max-pairs must be a positive integer"
        );
        assert_eq!(
            error(&["--max-triples", "abc"]),
            "max-triples must be a positive integer"
        );
        // limit is checked before jobs even though jobs was supplied later.
        assert_eq!(
            error(&["--jobs", "0", "--limit", "0"]),
            "limit must be a positive integer"
        );
        // A trailing flag with no value reads undefined -> NaN.
        assert_eq!(error(&["--limit"]), "limit must be a positive integer");
    }

    #[test]
    fn number_coercion_follows_the_ecmascript_grammar() {
        assert_eq!(js_number("0x10"), 16.0);
        assert_eq!(js_number("0b101"), 5.0);
        assert_eq!(js_number("0o17"), 15.0);
        assert_eq!(js_number(""), 0.0);
        assert_eq!(js_number("   \u{feff}12  "), 12.0);
        assert!(js_number("12abc").is_nan());
        assert!(js_number("inf").is_nan());
        assert!(js_number("NaN").is_nan());
        assert_eq!(js_number("Infinity"), f64::INFINITY);
        assert_eq!(js_number("1e3"), 1000.0);
        assert_eq!(js_number("-5"), -5.0);
        // U+0085 is whitespace to Rust but not to JavaScript.
        assert!(js_number("\u{85}12").is_nan());
        // Hex is unsigned-only in the grammar.
        assert!(js_number("-0x10").is_nan());
    }

    #[test]
    fn one_number_type_means_1_point_0_is_an_integer() {
        let options = parsed(&["--limit", "1e3"]);
        assert_eq!(options.limit, 1000.0);
        assert!(is_js_integer(1.0));
        assert!(!is_js_integer(f64::NAN));
        assert!(!is_js_integer(f64::INFINITY));
    }

    #[test]
    fn canonical_json_matches_the_typescript_layout() {
        let value = Json::Object(vec![
            ("format".into(), Json::Number(2.0)),
            ("flags".into(), Json::Array(vec![
                Json::String("-O2".into()),
                Json::String("-fno-inline".into()),
            ])),
            ("empty_list".into(), Json::Array(vec![])),
            ("empty_map".into(), Json::Object(vec![])),
            (
                "nested".into(),
                Json::Array(vec![Json::Object(vec![("a".into(), Json::Bool(true))])]),
            ),
        ]);
        assert_eq!(
            canonical_json(&value),
            concat!(
                "{\n",
                "  \"format\": 2,\n",
                "  \"flags\": [\"-O2\", \"-fno-inline\"],\n",
                "  \"empty_list\": [],\n",
                "  \"empty_map\": {},\n",
                "  \"nested\": [\n",
                "    {\n",
                "      \"a\": true\n",
                "    }\n",
                "  ]\n",
                "}"
            )
        );
    }

    #[test]
    fn object_key_order_survives_a_parse_and_rewrite() {
        let text = r#"{"zebra":1,"alpha":{"m":[1,2],"a":null},"zebra":9}"#;
        let value = parse_json(text).expect("parses");
        // Duplicate key keeps its original slot but takes the later value.
        assert_eq!(
            canonical_json(&value),
            "{\n  \"zebra\": 9,\n  \"alpha\": {\n    \"m\": [1, 2],\n    \"a\": null\n  }\n}"
        );
    }

    #[test]
    fn string_escaping_follows_json_stringify() {
        assert_eq!(js_string_literal("a\"b\\c"), r#""a\"b\\c""#);
        assert_eq!(js_string_literal("tab\there\nnl"), r#""tab\there\nnl""#);
        assert_eq!(js_string_literal("\u{1}"), "\"\\u0001\"");
        // Non-ASCII is passed through, not escaped.
        assert_eq!(js_string_literal("日本語"), "\"日本語\"");
    }

    #[test]
    fn sha256_matches_known_vectors() {
        assert_eq!(
            sha256_hex(b""),
            "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"
        );
        assert_eq!(
            sha256_hex(b"abc"),
            "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"
        );
        assert_eq!(
            sha256_hex(&[0u8; 64]),
            "f5a5fd42d16a20302798ef6ed309979b43003d2320d9f0e8ea9831a92759fb4b"
        );
    }

    #[test]
    fn mode_sweep_directory_hashes_relative_path_and_bytes() {
        let root = Path::new("/repo");
        let directory =
            mode_sweep_output_directory(root, "/repo/out/decomp/candidates/08021950.c", b"int f;\n");
        // Recompute the contract by hand: "<relative>\0<bytes>\0".
        let mut expected = Vec::new();
        expected.extend_from_slice(b"out/decomp/candidates/08021950.c\0int f;\n\0");
        let identity = &sha256_hex(&expected)[..16];
        assert_eq!(
            directory,
            format!("/repo/out/modesweep/08021950-{identity}")
        );
        // Different bytes must land in a different directory.
        let other = mode_sweep_output_directory(
            root,
            "/repo/out/decomp/candidates/08021950.c",
            b"int g;\n",
        );
        assert_ne!(directory, other);
    }

    #[test]
    fn relative_paths_walk_up_when_needed() {
        assert_eq!(
            relative_path(Path::new("/repo"), Path::new("/repo/a/b.c")),
            "a/b.c"
        );
        assert_eq!(
            relative_path(Path::new("/repo/tools"), Path::new("/repo/out/x")),
            "../out/x"
        );
    }

    #[test]
    fn resolve_prefers_an_absolute_candidate() {
        assert_eq!(resolve_path(Path::new("/repo"), "out/a.c"), "/repo/out/a.c");
        assert_eq!(resolve_path(Path::new("/repo"), "/tmp/a.c"), "/tmp/a.c");
        assert_eq!(resolve_path(Path::new("/repo"), "./x/../y.c"), "/repo/y.c");
    }

    fn report(stem: &str, results: Vec<Json>) -> Json {
        Json::Object(vec![
            ("format".into(), Json::Number(1.0)),
            ("stem".into(), Json::String(stem.into())),
            ("source".into(), Json::String(format!("out/{stem}.c"))),
            ("source_sha256".into(), Json::String("aa".into())),
            ("reference_sha256".into(), Json::String("bb".into())),
            ("compiler_signature".into(), Json::String("cc".into())),
            (
                "planning".into(),
                Json::Object(vec![(
                    "bounded_search_complete".into(),
                    Json::Bool(true),
                )]),
            ),
            ("results".into(), Json::Array(results)),
        ])
    }

    fn result(compiled: bool, exact: Option<bool>, ids: &[&str]) -> Json {
        let mut entries = vec![(
            "config".into(),
            Json::Object(vec![
                (
                    "ids".into(),
                    Json::Array(ids.iter().map(|id| Json::String((*id).into())).collect()),
                ),
                ("flags".into(), Json::Array(vec![])),
                ("remove_flags".into(), Json::Array(vec![])),
                ("compiler_family".into(), Json::String("agbcc".into())),
            ]),
        )];
        entries.push(("compiled".into(), Json::Bool(compiled)));
        if let Some(exact) = exact {
            entries.push((
                "evidence".into(),
                Json::Object(vec![("exact".into(), Json::Bool(exact))]),
            ));
        }
        Json::Object(entries)
    }

    #[test]
    fn only_compiled_and_exact_results_become_matches() {
        let reports = vec![
            report(
                "08021950",
                vec![
                    result(true, Some(true), &["a"]),
                    result(true, Some(false), &["b"]),
                    result(false, Some(true), &["c"]),
                    result(true, None, &["d"]),
                ],
            ),
            report("08002e00", vec![result(true, Some(true), &["e"])]),
        ];
        let options = parsed(&["--pairs", "--max-pairs", "9"]);
        let summary = summarize(&reports, &options);
        let matches = summary.get("matches").and_then(Json::as_array).unwrap();
        assert_eq!(matches.len(), 2);
        assert_eq!(matches[0].get("stem").and_then(Json::as_str), Some("08021950"));
        assert_eq!(
            matches[0]
                .get("config")
                .and_then(|c| c.get("ids"))
                .and_then(Json::as_array)
                .map(<[Json]>::len),
            Some(1)
        );
        assert_eq!(matches[1].get("stem").and_then(Json::as_str), Some("08002e00"));

        let search = summary.get("search").unwrap();
        assert_eq!(search.get("pairs"), Some(&Json::Bool(true)));
        assert_eq!(search.get("triples"), Some(&Json::Bool(false)));
        assert_eq!(search.get("max_pairs"), Some(&Json::Number(9.0)));

        let members = summary.get("members").and_then(Json::as_array).unwrap();
        assert_eq!(members.len(), 2);
        assert_eq!(
            members[0].get("bounded_search_complete"),
            Some(&Json::Bool(true))
        );
    }

    #[test]
    fn summary_field_order_is_the_typescript_literal_order() {
        let summary = summarize(&[], &parsed(&[]));
        let keys: Vec<&str> = match &summary {
            Json::Object(entries) => entries.iter().map(|(key, _)| key.as_str()).collect(),
            _ => panic!("summary is an object"),
        };
        assert_eq!(
            keys,
            ["format", "engine", "search", "members", "matches", "auto_promote"]
        );
        assert_eq!(
            canonical_json(&summary),
            concat!(
                "{\n",
                "  \"format\": 2,\n",
                "  \"engine\": \"tools/lib/mode_sweep.ts\",\n",
                "  \"search\": {\n",
                "    \"pairs\": false,\n",
                "    \"triples\": false,\n",
                "    \"max_pairs\": 256,\n",
                "    \"max_triples\": 64\n",
                "  },\n",
                "  \"members\": [],\n",
                "  \"matches\": [],\n",
                "  \"auto_promote\": false\n",
                "}"
            )
        );
    }

    #[test]
    fn config_key_order_is_preserved_through_the_summary() {
        let raw = r#"{"stem":"s","source":"x.c","source_sha256":"a","reference_sha256":"b",
          "compiler_signature":"c","planning":{"bounded_search_complete":false},
          "results":[{"config":{"zzz":1,"ids":["q"],"aaa":2},"compiled":true,
          "evidence":{"exact":true}}]}"#;
        let parsed_report = parse_json(raw).expect("parses");
        let summary = summarize(&[parsed_report], &parsed(&[]));
        let text = canonical_json(&summary);
        let zzz = text.find("\"zzz\"").expect("zzz present");
        let ids = text.find("\"ids\"").expect("ids present");
        let aaa = text.find("\"aaa\"").expect("aaa present");
        assert!(zzz < ids && ids < aaa, "config keys were reordered: {text}");
    }

    #[test]
    fn truthiness_gates_match_javascript() {
        assert!(!Json::Number(0.0).truthy());
        assert!(!Json::String(String::new()).truthy());
        assert!(Json::Array(vec![]).truthy());
        assert!(Json::Object(vec![]).truthy());
        assert!(!Json::Null.truthy());
        assert!(Json::Number(-1.0).truthy());
    }

    #[test]
    fn number_text_drops_the_decimal_point_for_integers() {
        assert_eq!(js_number_text(2.0), "2");
        assert_eq!(js_number_text(-0.0), "0");
        assert_eq!(js_number_text(1e21), "1e+21");
        assert_eq!(js_number_text(1e20), "100000000000000000000");
        assert_eq!(js_number_text(f64::NAN), "null");
        assert_eq!(js_number_text(0.5), "0.5");
    }
}
