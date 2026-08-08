//! Parity check between this crate and `tools/lib/alchemy_gcc.ts`.
//!
//! Four things exist in both languages while the TypeScript is live: the
//! `CALL_VIA_BASE` constant, the `CALL_VIA_REGISTERS` alias map, the
//! `OVERLAY_CALL_VIA_BASE` table and the `SOURCE_CALL_VIA_BASE` override table.
//! Plus the three regex literals, whose *text* is compared -- a change to a
//! character class in the TypeScript would otherwise silently diverge from the
//! hand-written matcher in `symbols`.
//!
//! Nothing generates either side. So this measures the parity rather than
//! claiming it: the declarations are parsed back out of the TypeScript source
//! and compared element-wise and IN ORDER against the Rust mirrors, the check
//! refuses to pass if fewer than [`MINIMUM_SUBJECTS`] subjects were compared,
//! and it fails loudly if either mirror cannot be read -- a vanished TypeScript
//! must never read as agreement.
//!
//! RETIREMENT: delete this module, `src/bin/symbols_drift.rs` and the `[[bin]]`
//! entry when `tools/lib/alchemy_gcc.ts` is deleted.

use std::fs;
use std::path::{Path, PathBuf};

use crate::call_via_data::{
    CALL_VIA_BASE, CALL_VIA_REGISTERS, OVERLAY_CALL_VIA_BASE, SOURCE_CALL_VIA_BASE,
};

/// Path of the TypeScript mirror, relative to the repository root.
pub const TYPESCRIPT_SOURCE: &str = "tools/lib/alchemy_gcc.ts";

/// The number of subjects both sides carry: two `Record` tables, the register
/// alias map, the `CALL_VIA_BASE` constant, and three regex literals. A parser
/// that quietly stopped matching would otherwise report zero drift over zero
/// comparisons and read exactly like a clean run.
pub const MINIMUM_SUBJECTS: usize = 7;

/// One subject, flattened to the ordered sequence of its rendered elements. A
/// `Record` contributes key and value alternately.
pub type Subject = Vec<String>;

/// How one subject is recovered from the TypeScript source: `(source, name)`.
type Parser = fn(&str, &str) -> Option<Subject>;

#[derive(Debug, Default)]
pub struct Report {
    /// Subjects present on both sides and identical, element for element.
    pub agreed: Vec<String>,
    /// Declared in `alchemy_gcc.ts` and not found on the Rust side.
    pub only_ts: Vec<String>,
    /// Mirrored in Rust and not found in `alchemy_gcc.ts`.
    pub only_rs: Vec<String>,
    /// Present on both sides with differing contents.
    pub drifted: Vec<Drift>,
}

#[derive(Debug)]
pub struct Drift {
    pub name: String,
    pub typescript: Subject,
    pub rust: Subject,
}

impl Report {
    pub fn compared(&self) -> usize {
        self.agreed.len() + self.only_ts.len() + self.only_rs.len() + self.drifted.len()
    }

    pub fn in_sync(&self) -> bool {
        self.only_ts.is_empty() && self.only_rs.is_empty() && self.drifted.is_empty()
    }
}

/// Repository root: `<crate>/../..`.
pub fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .expect("crate dir has a parent")
        .parent()
        .expect("tools-rs has a parent")
        .to_path_buf()
}

fn typescript_path() -> PathBuf {
    root().join(TYPESCRIPT_SOURCE)
}

/// Strip a `//` line comment, respecting the one place it matters: a `//`
/// inside a string literal. The tables use quoted keys like
/// `"exact/resource_373_c_02005610.c"`, which contain no `//`, but the parser
/// must not depend on that staying true.
fn strip_line_comment(line: &str) -> &str {
    let bytes = line.as_bytes();
    let mut quote: Option<u8> = None;
    let mut index = 0;
    while index < bytes.len() {
        let byte = bytes[index];
        match quote {
            Some(open) => {
                if byte == b'\\' {
                    index += 1;
                } else if byte == open {
                    quote = None;
                }
            }
            None => {
                if byte == b'"' || byte == b'\'' {
                    quote = Some(byte);
                } else if byte == b'/' && bytes.get(index + 1) == Some(&b'/') {
                    return &line[..index];
                }
            }
        }
        index += 1;
    }
    line
}

/// Split on `separator`, ignoring separators inside string literals. Keys in
/// these tables are quoted paths; a comma or colon inside one must not split
/// the entry.
fn split_top_level(text: &str, separator: char) -> Vec<&str> {
    let bytes = text.as_bytes();
    let mut quote: Option<u8> = None;
    let mut parts = Vec::new();
    let mut start = 0;
    let mut index = 0;
    while index < bytes.len() {
        let byte = bytes[index];
        match quote {
            Some(open) => {
                if byte == b'\\' {
                    index += 1;
                } else if byte == open {
                    quote = None;
                }
            }
            None => {
                if byte == b'"' || byte == b'\'' {
                    quote = Some(byte);
                } else if byte == separator as u8 {
                    parts.push(&text[start..index]);
                    start = index + 1;
                }
            }
        }
        index += 1;
    }
    parts.push(&text[start..]);
    parts
}

/// JS numeric literal -> u64, for the two forms these tables use: `0x` hex and
/// plain decimal. PORT NOTE: deliberately NOT `f64::from_str`. `Number("0x10")`
/// is 16 under the ECMAScript StringNumericLiteral grammar while
/// `"0x10".parse::<f64>()` is an error, and these values are addresses where a
/// silent 0 would be catastrophic -- so anything not exactly one of the two
/// expected forms is rejected rather than coerced.
fn parse_number(text: &str) -> Option<u64> {
    let text = text.trim();
    let text = text.strip_suffix(',').unwrap_or(text).trim();
    if let Some(hex) = text.strip_prefix("0x").or_else(|| text.strip_prefix("0X")) {
        let hex = hex.replace('_', "");
        if hex.is_empty() || !hex.bytes().all(|b| b.is_ascii_hexdigit()) {
            return None;
        }
        return u64::from_str_radix(&hex, 16).ok();
    }
    if !text.is_empty() && text.bytes().all(|b| b.is_ascii_digit()) {
        return text.parse().ok();
    }
    None
}

fn unquote(text: &str) -> String {
    let text = text.trim();
    for quote in ['"', '\''] {
        if let Some(inner) = text.strip_prefix(quote).and_then(|t| t.strip_suffix(quote)) {
            return inner.to_string();
        }
    }
    text.to_string()
}

/// Render a numeric mirror value the way the parsed TypeScript renders, so the
/// element-wise comparison is over identical text on both sides.
fn render(value: u64) -> String {
    format!("0x{value:08x}")
}

fn pairs_to_subject(pairs: &[(&str, u64)]) -> Subject {
    pairs
        .iter()
        .flat_map(|(key, value)| [(*key).to_string(), render(*value)])
        .collect()
}

/// Parse a `const NAME<: type> = { ... };` object literal body into the
/// key/value sequence, in source order.
fn parse_record(source: &str, name: &str) -> Option<Subject> {
    let declaration = format!("const {name}");
    let start = source.find(&declaration)?;
    let open = source[start..].find('{')? + start;
    let close = source[open..].find("};")? + open;
    // Comments are stripped per line, then the body is split on top-level
    // commas: `CALL_VIA_REGISTERS` is written on one line and the two address
    // tables one entry per line, and both spellings have to parse.
    let body: String = source[open + 1..close]
        .lines()
        .map(strip_line_comment)
        .collect::<Vec<_>>()
        .join("\n");
    let mut out = Subject::new();
    for entry in split_top_level(&body, ',') {
        let entry = entry.trim();
        if entry.is_empty() {
            continue;
        }
        let (key, value) = split_top_level(entry, ':').into_iter().collect::<Vec<_>>()[..]
            .split_first()
            .map(|(key, rest)| ((*key).to_string(), rest.join(":")))?;
        out.push(unquote(&key));
        out.push(render(parse_number(&value)?));
    }
    Some(out)
}

/// Parse `const NAME = <number>;` / `export const NAME = <number>;`.
fn parse_scalar(source: &str, name: &str) -> Option<Subject> {
    let index = source.find(&format!("const {name} ="))?;
    let rest = &source[index..];
    let value = &rest[rest.find('=')? + 1..];
    let value = value.split(';').next()?;
    Some(vec![render(parse_number(strip_line_comment(value))?)])
}

/// Parse `const NAME = /regex/;`, keeping the literal text.
fn parse_regex(source: &str, name: &str) -> Option<Subject> {
    let index = source.find(&format!("const {name} ="))?;
    let rest = &source[index..];
    let line = rest.lines().next()?;
    let body = line.split_once('=')?.1.trim();
    let body = body.strip_suffix(';')?.trim();
    if !body.starts_with('/') {
        return None;
    }
    Some(vec![body.to_string()])
}

/// The regex literals as `symbols` hand-matches them. Compared as text: this
/// crate has no regex engine, so a change to a character class on the
/// TypeScript side can only be caught by noticing the source text moved.
pub static REGEX_LITERALS: &[(&str, &str)] = &[
    (
        "ADDRESS_SYMBOL",
        r"/^(Func|Data|Value)_([0-9a-f]{8})(?:_[a-z])?$/",
    ),
    ("CALL_VIA_SYMBOL", r"/^_call_via_r(1[0-3]|[0-9])$/"),
    ("CALL_VIA_ALIAS", r"/^_call_via_(sl|fp|ip|sp)$/"),
];

/// Compare the two mirrors. `Err` on an unreadable TypeScript source: a missing
/// file is a failure, never agreement.
pub fn compare() -> Result<Report, String> {
    let path = typescript_path();
    let source = fs::read_to_string(&path)
        .map_err(|error| format!("cannot read {}: {error}", path.display()))?;
    if source.trim().is_empty() {
        return Err(format!("{} is empty", path.display()));
    }
    compare_source(&source)
}

/// The comparison proper, over already-read TypeScript. Split out so tests can
/// feed it a perturbed copy and prove the gate bites.
pub fn compare_source(source: &str) -> Result<Report, String> {
    let mut report = Report::default();

    let rust: Vec<(&str, Subject, Parser)> = vec![
        (
            "CALL_VIA_BASE",
            vec![render(CALL_VIA_BASE)],
            parse_scalar as Parser,
        ),
        (
            "CALL_VIA_REGISTERS",
            pairs_to_subject(CALL_VIA_REGISTERS),
            parse_record,
        ),
        (
            "OVERLAY_CALL_VIA_BASE",
            pairs_to_subject(OVERLAY_CALL_VIA_BASE),
            parse_record,
        ),
        (
            "SOURCE_CALL_VIA_BASE",
            pairs_to_subject(SOURCE_CALL_VIA_BASE),
            parse_record,
        ),
    ];

    for (name, mirror, parse) in rust {
        match parse(source, name) {
            None => report.only_rs.push(name.to_string()),
            Some(typescript) if typescript == mirror => report.agreed.push(name.to_string()),
            Some(typescript) => report.drifted.push(Drift {
                name: name.to_string(),
                typescript,
                rust: mirror,
            }),
        }
    }

    for (name, literal) in REGEX_LITERALS {
        match parse_regex(source, name) {
            None => report.only_rs.push((*name).to_string()),
            Some(typescript) if typescript == vec![(*literal).to_string()] => {
                report.agreed.push((*name).to_string())
            }
            Some(typescript) => report.drifted.push(Drift {
                name: (*name).to_string(),
                typescript,
                rust: vec![(*literal).to_string()],
            }),
        }
    }

    Ok(report)
}

/// `compare`, plus the floor. This is what the binary and the test call.
pub fn check() -> Result<Report, String> {
    let report = compare()?;
    if report.compared() < MINIMUM_SUBJECTS {
        return Err(format!(
            "compared {} subjects, expected at least {MINIMUM_SUBJECTS}: the parser stopped \
             matching, which is a failure and not a clean run",
            report.compared()
        ));
    }
    if !report.in_sync() {
        let mut message = String::from("alchemy-symbols has drifted from alchemy_gcc.ts:\n");
        for name in &report.only_ts {
            message.push_str(&format!("  only in TypeScript: {name}\n"));
        }
        for name in &report.only_rs {
            message.push_str(&format!("  not found in TypeScript: {name}\n"));
        }
        for drift in &report.drifted {
            message.push_str(&format!(
                "  {}: ts={:?} rs={:?}\n",
                drift.name, drift.typescript, drift.rust
            ));
        }
        return Err(message);
    }
    Ok(report)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn mirrors_agree() {
        let report = check().unwrap();
        assert!(report.compared() >= MINIMUM_SUBJECTS);
        assert_eq!(report.agreed.len(), report.compared());
    }

    /// Prove the gate bites: perturb one element of one table and watch the
    /// comparison fail. This is the negative control for the drift check
    /// itself.
    #[test]
    fn a_single_perturbed_element_is_caught() {
        let source = fs::read_to_string(typescript_path()).unwrap();

        let perturbed = source.replacen("resource_3cb: 0x02001a96", "resource_3cb: 0x02001a97", 1);
        assert_ne!(perturbed, source, "the perturbation did not apply");
        let report = compare_source(&perturbed).unwrap();
        assert_eq!(report.drifted.len(), 1);
        assert_eq!(report.drifted[0].name, "OVERLAY_CALL_VIA_BASE");

        // Order, not just membership: swapping two rows must fail too, because
        // the comparison is element-wise and in order.
        let swapped = source.replacen(
            "  resource_382: 0x02003138,\n  resource_385: 0x020014fc,",
            "  resource_385: 0x020014fc,\n  resource_382: 0x02003138,",
            1,
        );
        assert_ne!(swapped, source, "the swap did not apply");
        assert_eq!(compare_source(&swapped).unwrap().drifted.len(), 1);

        // A changed regex character class is caught as text.
        let widened = source.replacen("(?:_[a-z])?$/", "(?:_[a-zA-Z])?$/", 1);
        assert_ne!(widened, source);
        let report = compare_source(&widened).unwrap();
        assert_eq!(report.drifted.len(), 1);
        assert_eq!(report.drifted[0].name, "ADDRESS_SYMBOL");
    }

    #[test]
    fn a_vanished_typescript_is_not_agreement() {
        let report = compare_source("// nothing here\n").unwrap();
        assert!(!report.in_sync());
        assert_eq!(report.only_rs.len(), MINIMUM_SUBJECTS);
        assert!(compare_source("").is_err() || !compare_source("").unwrap().in_sync());
    }

    #[test]
    fn numbers_are_parsed_by_the_javascript_grammar_or_rejected() {
        assert_eq!(parse_number("0x10"), Some(16));
        assert_eq!(parse_number(" 10, "), Some(10));
        assert_eq!(parse_number("1.5"), None);
        assert_eq!(parse_number(""), None);
        assert_eq!(parse_number("12abc"), None);
    }

    #[test]
    fn line_comments_are_stripped_outside_strings() {
        assert_eq!(strip_line_comment("a: 1, // note").trim(), "a: 1,");
        assert_eq!(strip_line_comment("\"a//b\": 1,").trim(), "\"a//b\": 1,");
    }
}
