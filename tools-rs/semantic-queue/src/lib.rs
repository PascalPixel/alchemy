//! Rank the remaining main-image semantic work by reconstruction cost.
//!
//! Rust port of `tools/semantic/semantic_queue.ts`. Historical m2c output is
//! useful evidence, but it is not admission-ready C. This queue puts compact,
//! ordinary-ABI owners first and makes the expensive failure modes visible
//! before a human spends time cleaning a draft.
//!
//! PORT NOTE (inlined dependencies): the TypeScript original imports only
//! `node:fs` and `node:path`; it has no local-module imports, so nothing from
//! the repository's own TS libraries was inlined here. The only host-library
//! behaviours reimplemented are `basename`/`relative`/`join` (trivially, over
//! ASCII paths) and the JavaScript regular expressions listed below, which are
//! hand-rolled in `matchers` because no regex crate is permitted.
//!
//! PORT NOTE (regex semantics): every pattern is reimplemented against
//! JavaScript semantics, not Rust's `regex` semantics:
//!   * `\s` is the JavaScript whitespace set (ASCII whitespace plus NBSP, BOM,
//!     the Unicode `Zs` category, U+2028 and U+2029), which notably *includes*
//!     `\n`. `^\s*bl` under the `m` flag can therefore start on a line before
//!     the `bl`, and `bl\s+\S+` can span a line break. See `bl_spanning_newline`.
//!   * `\b` / `\B` use the ASCII word set `[A-Za-z0-9_]`.
//!   * `$`/`^` without the `m` flag anchor at the very ends of the input; JS has
//!     no "before a trailing newline" rule (unlike Python).
//!
//! PORT NOTE (file decoding): Node's `readFileSync(path, "utf8")` replaces
//! invalid byte sequences with U+FFFD rather than throwing, so all reads here
//! go through `String::from_utf8_lossy`.
//!
//! PORT NOTE (ordering): the final tiebreak in the TS is
//! `left.stem.localeCompare(right.stem)`. Stems are lowercase hexadecimal, and
//! for `[0-9a-f]` the ICU root collation and byte order agree, so plain `Ord`
//! on `str` is used. Stems are unique, so the comparator is total and the
//! (stable) JS sort and Rust's stable sort agree unconditionally.

use std::collections::{BTreeSet, HashMap};
use std::fs;
use std::path::{Path, PathBuf};

use serde_json::{Map, Value};

pub const ORDINARY_RETENTION: [&str; 4] = [
    "c_candidate",
    "split_first",
    "merge_with_continuations",
    "merge_with_owner",
];

#[derive(Debug, Clone)]
pub struct Region {
    pub address: u64,
    pub size: i64,
    pub source: String,
    pub retention: String,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Candidate {
    pub stem: String,
    pub bytes: i64,
    pub calls: i64,
    pub source_lines: i64,
    pub unset_registers: i64,
    pub internal_exits: i64,
    pub unknown_types: i64,
    pub high_register_call_setups: i64,
    pub runtime_thunk_calls: i64,
    pub established_thunk_family: Option<String>,
    pub boundary_shape: String,
    pub scope_audit_required: bool,
    pub blocker_class: Option<String>,
    pub blocked_reason: Option<String>,
    pub draft: String,
    pub score: i64,
}

// ---------------------------------------------------------------------------
// Hand-rolled matchers (see the PORT NOTE on regex semantics above).
// ---------------------------------------------------------------------------
pub mod matchers {
    /// JavaScript `\s`.
    pub fn is_space(c: char) -> bool {
        matches!(
            c,
            '\u{9}'
                | '\u{a}'
                | '\u{b}'
                | '\u{c}'
                | '\u{d}'
                | '\u{20}'
                | '\u{a0}'
                | '\u{1680}'
                | '\u{2000}'
                ..='\u{200a}'
                    | '\u{2028}'
                    | '\u{2029}'
                    | '\u{202f}'
                    | '\u{205f}'
                    | '\u{3000}'
                    | '\u{feff}'
        )
    }

    /// JavaScript `\w` (ASCII only, as in JS without the `u` flag).
    pub fn is_word(c: char) -> bool {
        c.is_ascii_alphanumeric() || c == '_'
    }

    fn skip_space(text: &[char], mut index: usize) -> usize {
        while index < text.len() && is_space(text[index]) {
            index += 1;
        }
        index
    }

    /// Greedy `\s+`; returns `None` when no whitespace is present.
    fn take_space(text: &[char], index: usize) -> Option<usize> {
        let end = skip_space(text, index);
        if end == index {
            None
        } else {
            Some(end)
        }
    }

    fn literal(text: &[char], index: usize, needle: &str, fold_case: bool) -> Option<usize> {
        let mut cursor = index;
        for want in needle.chars() {
            let got = *text.get(cursor)?;
            let same = if fold_case {
                got.eq_ignore_ascii_case(&want)
            } else {
                got == want
            };
            if !same {
                return None;
            }
            cursor += 1;
        }
        Some(cursor)
    }

    fn word_boundary_after(text: &[char], index: usize) -> bool {
        !text.get(index).copied().is_some_and(is_word)
    }

    fn word_boundary_before(text: &[char], index: usize) -> bool {
        index == 0 || !is_word(text[index - 1])
    }

    fn at_line_start(text: &[char], index: usize) -> bool {
        index == 0 || text[index - 1] == '\n'
    }

    /// Count non-overlapping matches anchored at line starts (`m` flag),
    /// resuming the scan at each match end exactly as `String.matchAll` does.
    fn count_multiline<F>(text: &[char], matcher: F) -> i64
    where
        F: Fn(&[char], usize) -> Option<usize>,
    {
        let mut index = 0usize;
        let mut total = 0i64;
        while index <= text.len() {
            if at_line_start(text, index) {
                if let Some(end) = matcher(text, index) {
                    total += 1;
                    index = end;
                    continue;
                }
            }
            index += 1;
        }
        total
    }

    fn count_anywhere<F>(text: &[char], matcher: F) -> i64
    where
        F: Fn(&[char], usize) -> Option<usize>,
    {
        let mut index = 0usize;
        let mut total = 0i64;
        while index <= text.len() {
            if let Some(end) = matcher(text, index) {
                total += 1;
                index = end;
                continue;
            }
            index += 1;
        }
        total
    }

    /// `/^\s*bl\s+\S+/` — the leading `\s*` is greedy but never needs to
    /// backtrack: a shorter run would leave the cursor on whitespace, which can
    /// never begin `bl`. The same argument covers `\s+` before `\S+`.
    pub fn match_call(text: &[char], index: usize) -> Option<usize> {
        let mut cursor = skip_space(text, index);
        cursor = literal(text, cursor, "bl", false)?;
        cursor = take_space(text, cursor)?;
        let start = cursor;
        while cursor < text.len() && !is_space(text[cursor]) {
            cursor += 1;
        }
        if cursor == start {
            None
        } else {
            Some(cursor)
        }
    }

    /// `/^\s*bl\s+\S+/gm`
    pub fn count_calls(text: &[char]) -> i64 {
        count_multiline(text, match_call)
    }

    fn hex6(text: &[char], index: usize) -> Option<usize> {
        for offset in 0..6 {
            if !text.get(index + offset)?.is_ascii_hexdigit() {
                return None;
            }
        }
        Some(index + 6)
    }

    /// `/^\s*b(?:\.[a-z]+)?\s+Func_08[0-9a-f]{6}\b/gim`
    pub fn match_internal_exit(text: &[char], index: usize) -> Option<usize> {
        let mut cursor = skip_space(text, index);
        cursor = literal(text, cursor, "b", true)?;
        // Optional `(?:\.[a-z]+)?`, greedy, but only taken when it matches in
        // full; `\s+` can never succeed at the `.` so failing back to the empty
        // alternative always dead-ends there too.
        if text.get(cursor) == Some(&'.') {
            let mut probe = cursor + 1;
            let mut letters = 0;
            while probe < text.len() && text[probe].is_ascii_alphabetic() {
                probe += 1;
                letters += 1;
            }
            if letters > 0 {
                cursor = probe;
            }
        }
        cursor = take_space(text, cursor)?;
        cursor = literal(text, cursor, "Func_08", true)?;
        cursor = hex6(text, cursor)?;
        if word_boundary_after(text, cursor) {
            Some(cursor)
        } else {
            None
        }
    }

    /// `/^\s*b(?:\.[a-z]+)?\s+Func_08[0-9a-f]{6}\b/gim`
    pub fn count_internal_exits(text: &[char]) -> i64 {
        count_multiline(text, match_internal_exit)
    }

    pub const THUNK_SUFFIXES: [&str; 14] = [
        "2e4", "2e8", "2ec", "2f0", "2f4", "2f8", "2fc", "300", "304", "308", "30c", "310", "314",
        "318",
    ];

    /// `/^\s*bl\s+Func_08007(?:2e4|…|318)\b/gim`
    pub fn match_runtime_thunk(text: &[char], index: usize) -> Option<usize> {
        let mut cursor = skip_space(text, index);
        cursor = literal(text, cursor, "bl", true)?;
        cursor = take_space(text, cursor)?;
        cursor = literal(text, cursor, "Func_08007", true)?;
        for suffix in THUNK_SUFFIXES {
            if let Some(end) = literal(text, cursor, suffix, true) {
                if word_boundary_after(text, end) {
                    return Some(end);
                }
            }
        }
        None
    }

    /// `/^\s*bl\s+Func_08007(?:…)\b/gim`
    pub fn count_runtime_thunks(text: &[char]) -> i64 {
        count_multiline(text, match_runtime_thunk)
    }

    /// `/\bM2C_ERROR\s*\(/g`
    pub fn count_unset_registers(text: &[char]) -> i64 {
        count_anywhere(text, |text, index| {
            if !word_boundary_before(text, index) {
                return None;
            }
            let mut cursor = literal(text, index, "M2C_ERROR", false)?;
            cursor = skip_space(text, cursor);
            literal(text, cursor, "(", false)
        })
    }

    /// `/\bM2C_UNK\b/g`
    pub fn count_unknown_types(text: &[char]) -> i64 {
        count_anywhere(text, |text, index| {
            if !word_boundary_before(text, index) {
                return None;
            }
            let end = literal(text, index, "M2C_UNK", false)?;
            if word_boundary_after(text, end) {
                Some(end)
            } else {
                None
            }
        })
    }

    /// `/\bmov\s+(?:r8|r9|sl|fp)\s*,/.test(setup)`
    pub fn has_high_register_setup(text: &[char]) -> bool {
        for index in 0..=text.len() {
            if !word_boundary_before(text, index) {
                continue;
            }
            let Some(after_mov) = literal(text, index, "mov", false) else {
                continue;
            };
            let Some(after_space) = take_space(text, after_mov) else {
                continue;
            };
            for register in ["r8", "r9", "sl", "fp"] {
                if let Some(end) = literal(text, after_space, register, false) {
                    let end = skip_space(text, end);
                    if literal(text, end, ",", false).is_some() {
                        return true;
                    }
                }
            }
        }
        false
    }

    /// `/\bbl\s+(?:Func_080cef64|Func_080ed408)\b/i.test(assembly)`
    pub fn has_established_thunk_publisher(text: &[char]) -> bool {
        for index in 0..=text.len() {
            if !word_boundary_before(text, index) {
                continue;
            }
            let Some(after_bl) = literal(text, index, "bl", true) else {
                continue;
            };
            let Some(after_space) = take_space(text, after_bl) else {
                continue;
            };
            for name in ["Func_080cef64", "Func_080ed408"] {
                if let Some(end) = literal(text, after_space, name, true) {
                    if word_boundary_after(text, end) {
                        return true;
                    }
                }
            }
        }
        false
    }

    /// `/^\s*bl\s+\S+/.test(line)` — no `m` flag, so `^` is the string start.
    pub fn line_is_call(text: &[char]) -> bool {
        // Without `m`, only offset 0 can satisfy `^`.
        match_call(text, 0).is_some()
    }

    /// `/^08[0-9a-f]{6}\.c$/i.test(name)`
    pub fn is_source_stem_name(name: &str) -> bool {
        let chars: Vec<char> = name.chars().collect();
        if chars.len() != 10 {
            return false;
        }
        let Some(cursor) = literal(&chars, 0, "08", false) else {
            return false;
        };
        let Some(cursor) = hex6(&chars, cursor) else {
            return false;
        };
        literal(&chars, cursor, ".c", true) == Some(10)
    }
}

use matchers::*;

fn read_text(path: &Path) -> std::io::Result<String> {
    // PORT NOTE: Node's readFileSync(..., "utf8") is lossy on invalid bytes.
    Ok(String::from_utf8_lossy(&fs::read(path)?).into_owned())
}

/// `Math.ceil(value / 10)` for the non-negative line counts used here.
fn ceil_div_10(value: i64) -> i64 {
    (value + 9) / 10
}

pub fn stem_of(address: u64) -> String {
    format!("{address:08x}")
}

fn source_stems(directory: &Path) -> BTreeSet<String> {
    let Ok(entries) = fs::read_dir(directory) else {
        return BTreeSet::new();
    };
    let mut stems = BTreeSet::new();
    for entry in entries.flatten() {
        let name = entry.file_name().to_string_lossy().into_owned();
        if is_source_stem_name(&name) {
            stems.insert(name[..name.len() - 2].to_ascii_lowercase());
        }
    }
    stems
}

pub fn analyze_candidate(root: &Path, region: &Region, draft_path: &Path) -> Candidate {
    let draft = read_text(draft_path).unwrap_or_else(|error| {
        panic!("cannot read draft {}: {error}", draft_path.display());
    });
    let assembly_path = if region.source.starts_with('/') {
        PathBuf::from(&region.source)
    } else {
        root.join(&region.source)
    };
    let assembly = read_text(&assembly_path).unwrap_or_else(|error| {
        panic!("cannot read assembly {}: {error}", assembly_path.display());
    });

    let draft_chars: Vec<char> = draft.chars().collect();
    let assembly_chars: Vec<char> = assembly.chars().collect();

    let calls = count_calls(&assembly_chars);
    let unset_registers = count_unset_registers(&draft_chars);
    let internal_exits = count_internal_exits(&assembly_chars);
    let unknown_types = count_unknown_types(&draft_chars);
    let runtime_thunk_calls = count_runtime_thunks(&assembly_chars);
    let established_thunk_family = if has_established_thunk_publisher(&assembly_chars) {
        Some("renderer".to_string())
    } else {
        None
    };
    let thunk_penalty = if established_thunk_family.is_none() {
        250
    } else {
        40
    };
    let source_lines = draft.split('\n').count() as i64;
    let assembly_lines: Vec<&str> = assembly.split('\n').collect();
    let mut high_register_call_setups = 0i64;
    for index in 0..assembly_lines.len() {
        let line: Vec<char> = assembly_lines[index].chars().collect();
        if !line_is_call(&line) {
            continue;
        }
        let start = index.saturating_sub(5);
        let setup: Vec<char> = assembly_lines[start..index].join("\n").chars().collect();
        if has_high_register_setup(&setup) {
            high_register_call_setups += 1;
        }
    }

    let scope_audit_required = region.retention != "c_candidate";
    let boundary_penalty = if scope_audit_required { 5_000 } else { 0 };

    let score = calls * 20
        + unset_registers * 200
        + internal_exits * 250
        + unknown_types * 8
        + high_register_call_setups * 40
        + runtime_thunk_calls * thunk_penalty
        + boundary_penalty
        + ceil_div_10(source_lines);

    Candidate {
        stem: stem_of(region.address),
        bytes: region.size,
        calls,
        source_lines,
        unset_registers,
        internal_exits,
        unknown_types,
        high_register_call_setups,
        runtime_thunk_calls,
        established_thunk_family,
        boundary_shape: region.retention.clone(),
        scope_audit_required,
        blocker_class: None,
        blocked_reason: None,
        draft: draft_path
            .strip_prefix(root)
            .unwrap_or(draft_path)
            .to_string_lossy()
            .into_owned(),
        score,
    }
}

fn parse_regions(manifest: &Value) -> Vec<Region> {
    manifest
        .get("regions")
        .and_then(Value::as_array)
        .map(Vec::as_slice)
        .unwrap_or(&[])
        .iter()
        .map(|region| Region {
            address: region.get("address").and_then(Value::as_u64).unwrap_or(0),
            size: region.get("size").and_then(Value::as_i64).unwrap_or(0),
            source: region
                .get("source")
                .and_then(Value::as_str)
                .unwrap_or_default()
                .to_string(),
            retention: region
                .get("retention")
                .and_then(Value::as_str)
                .unwrap_or_default()
                .to_string(),
        })
        .collect()
}

pub fn semantic_queue(root: &Path) -> Vec<Candidate> {
    let manifest_text = read_text(
        &root
            .join("out")
            .join("full")
            .join("asm")
            .join("manifest.json"),
    )
    .expect("cannot read out/full/asm/manifest.json");
    let manifest: Value = serde_json::from_str(&manifest_text).expect("manifest.json is not JSON");
    let regions = parse_regions(&manifest);

    // Exact owners are finished. Semantic owners are the primary queue input:
    // they are readable C awaiting byte-exact closure, not owners to exclude.
    let admitted = source_stems(&root.join("exact"));

    let blockers_path = root.join("semantic").join("ordinary-blockers.json");
    let mut blockers: HashMap<String, (String, String)> = HashMap::new();
    if blockers_path.exists() {
        let text = read_text(&blockers_path).expect("cannot read ordinary-blockers.json");
        let parsed: Value =
            serde_json::from_str(&text).expect("ordinary-blockers.json is not JSON");
        if let Some(owners) = parsed.get("owners").and_then(Value::as_object) {
            for (stem, entry) in owners {
                blockers.insert(
                    stem.clone(),
                    (
                        entry
                            .get("class")
                            .and_then(Value::as_str)
                            .unwrap_or_default()
                            .to_string(),
                        entry
                            .get("reason")
                            .and_then(Value::as_str)
                            .unwrap_or_default()
                            .to_string(),
                    ),
                );
            }
        }
    }

    let mut candidates: Vec<Candidate> = Vec::new();
    for region in &regions {
        if !ORDINARY_RETENTION.contains(&region.retention.as_str()) {
            continue;
        }
        let stem = stem_of(region.address);
        if admitted.contains(&stem) {
            continue;
        }
        let paths = [
            root.join("semantic").join(format!("{stem}.c")),
            root.join("work")
                .join("candidates")
                .join(format!("{stem}.c")),
            root.join("work").join(format!("{stem}.c")),
        ];
        let Some(draft) = paths.into_iter().find(|path| path.exists()) else {
            continue;
        };
        let mut candidate = analyze_candidate(root, region, &draft);
        if let Some((class, reason)) = blockers.get(&stem) {
            candidate.score += 10_000;
            candidate.blocker_class = Some(class.clone());
            candidate.blocked_reason = Some(reason.clone());
        }
        candidates.push(candidate);
    }

    candidates.sort_by(|left, right| {
        left.score
            .cmp(&right.score)
            .then(right.bytes.cmp(&left.bytes))
            .then(left.stem.cmp(&right.stem))
    });
    candidates
}

/// Mirrors `JSON.stringify(queue, null, 2)`: keys in literal insertion order,
/// `undefined`-valued keys omitted, and the two blocker keys appended last
/// because the TS assigns them after the object literal is built.
pub fn to_json(queue: &[Candidate]) -> String {
    let array: Vec<Value> = queue
        .iter()
        .map(|item| {
            let mut object = Map::new();
            object.insert("stem".into(), Value::from(item.stem.clone()));
            object.insert("bytes".into(), Value::from(item.bytes));
            object.insert("calls".into(), Value::from(item.calls));
            object.insert("sourceLines".into(), Value::from(item.source_lines));
            object.insert("unsetRegisters".into(), Value::from(item.unset_registers));
            object.insert("internalExits".into(), Value::from(item.internal_exits));
            object.insert("unknownTypes".into(), Value::from(item.unknown_types));
            object.insert(
                "highRegisterCallSetups".into(),
                Value::from(item.high_register_call_setups),
            );
            object.insert(
                "runtimeThunkCalls".into(),
                Value::from(item.runtime_thunk_calls),
            );
            if let Some(family) = &item.established_thunk_family {
                object.insert("establishedThunkFamily".into(), Value::from(family.clone()));
            }
            object.insert(
                "boundaryShape".into(),
                Value::from(item.boundary_shape.clone()),
            );
            object.insert(
                "scopeAuditRequired".into(),
                Value::from(item.scope_audit_required),
            );
            object.insert("draft".into(), Value::from(item.draft.clone()));
            object.insert("score".into(), Value::from(item.score));
            if let Some(class) = &item.blocker_class {
                object.insert("blockerClass".into(), Value::from(class.clone()));
            }
            if let Some(reason) = &item.blocked_reason {
                object.insert("blockedReason".into(), Value::from(reason.clone()));
            }
            Value::Object(object)
        })
        .collect();
    serde_json::to_string_pretty(&Value::Array(array)).expect("queue is serialisable")
}

fn pad_start(text: &str, width: usize) -> String {
    // JS String.padStart counts UTF-16 code units; every value here is ASCII.
    if text.chars().count() >= width {
        text.to_string()
    } else {
        format!("{}{text}", " ".repeat(width - text.chars().count()))
    }
}

pub fn format_row(item: &Candidate) -> String {
    format!(
        "{} {}B score={} calls={} unset={} exits={} higharg={} thunks={} {}unk={} shape={} {}{}{}",
        item.stem,
        pad_start(&item.bytes.to_string(), 5),
        pad_start(&item.score.to_string(), 4),
        pad_start(&item.calls.to_string(), 3),
        item.unset_registers,
        item.internal_exits,
        item.high_register_call_setups,
        item.runtime_thunk_calls,
        match &item.established_thunk_family {
            None => String::new(),
            Some(family) => format!("family={family} "),
        },
        pad_start(&item.unknown_types.to_string(), 2),
        item.boundary_shape,
        if item.scope_audit_required {
            "scope=transitive-unsized "
        } else {
            ""
        },
        match &item.blocker_class {
            None => String::new(),
            Some(class) => format!("blocked={class} "),
        },
        item.draft,
    )
}

/// `Number.parseInt(text, 10)`. `None` stands for `NaN`.
pub fn js_parse_int(text: &str) -> Option<f64> {
    let trimmed = text.trim_start_matches(is_space);
    let bytes = trimmed.as_bytes();
    let mut index = 0usize;
    let mut negative = false;
    if index < bytes.len() && (bytes[index] == b'+' || bytes[index] == b'-') {
        negative = bytes[index] == b'-';
        index += 1;
    }
    let start = index;
    while index < bytes.len() && bytes[index].is_ascii_digit() {
        index += 1;
    }
    if index == start {
        return None;
    }
    let magnitude: f64 = trimmed[start..index].parse().ok()?;
    Some(if negative { -magnitude } else { magnitude })
}

/// `Array.prototype.slice(0, limit)` where `limit` may be `NaN` or negative.
pub fn js_slice_limit(length: usize, limit: Option<f64>) -> usize {
    let Some(limit) = limit else { return 0 };
    let length = length as f64;
    let end = if limit < 0.0 { length + limit } else { limit };
    end.clamp(0.0, length) as usize
}

#[cfg(test)]
mod tests {
    use super::matchers::*;
    use super::*;

    fn chars(text: &str) -> Vec<char> {
        text.chars().collect()
    }

    // --- the TypeScript selfTest(), ported verbatim ------------------------
    #[test]
    fn ts_self_test() {
        let draft = chars("M2C_UNK Func_08001234(void);\nM2C_ERROR(/* r0 */);\n");
        assert_eq!(count_unknown_types(&draft), 1);
        assert_eq!(count_unset_registers(&draft), 1);
        let assembly = chars("  bl Func_08001234\n  b.n Func_08005678\n");
        assert_eq!(count_calls(&assembly), 1);
        assert_eq!(count_internal_exits(&assembly), 1);
    }

    // --- edge cases the TypeScript self-test missed ------------------------
    #[test]
    fn call_counting_is_per_line_and_rejects_lookalikes() {
        assert_eq!(count_calls(&chars("  bl a\n  bl b\n  bl c")), 3);
        // `blx` is not `bl` + whitespace.
        assert_eq!(count_calls(&chars("  blx r3\n")), 0);
        // `bl` needs an operand.
        assert_eq!(count_calls(&chars("  bl\n")), 0);
        // Not at a line start.
        assert_eq!(count_calls(&chars("foo bl bar\n")), 0);
        // Case matters for the non-`i` pattern.
        assert_eq!(count_calls(&chars("  BL foo\n")), 0);
    }

    /// JS `\s` includes `\n`, so `^\s*bl\s+\S+` can straddle line breaks; a
    /// naive line-by-line port would report 0 here and 1 for the second case.
    #[test]
    fn bl_spanning_newline() {
        assert_eq!(count_calls(&chars("\n\n  bl foo\n")), 1);
        assert_eq!(count_calls(&chars("  bl\n  foo\n")), 1);
        // Blank lines between calls must not swallow the second call.
        assert_eq!(count_calls(&chars("  bl a\n\n\n  bl b\n")), 2);
    }

    #[test]
    fn internal_exit_variants() {
        assert_eq!(count_internal_exits(&chars("  b Func_08001234\n")), 1);
        assert_eq!(count_internal_exits(&chars("  b.w Func_08001234\n")), 1);
        assert_eq!(count_internal_exits(&chars("  B.N FUNC_08ABCDEF\n")), 1);
        // `bl` is not `b` followed by whitespace.
        assert_eq!(count_internal_exits(&chars("  bl Func_08001234\n")), 0);
        // Seven hex digits break the trailing `\b`.
        assert_eq!(count_internal_exits(&chars("  b Func_080012345\n")), 0);
        // A dot with no letters cannot satisfy `\s+`.
        assert_eq!(count_internal_exits(&chars("  b. Func_08001234\n")), 0);
        // Only 08-prefixed targets count.
        assert_eq!(count_internal_exits(&chars("  b Func_02001234\n")), 0);
    }

    #[test]
    fn runtime_thunk_bank() {
        assert_eq!(count_runtime_thunks(&chars("  bl Func_080072e4\n")), 1);
        assert_eq!(count_runtime_thunks(&chars("  bl Func_08007318\n")), 1);
        assert_eq!(count_runtime_thunks(&chars("  BL FUNC_08007300\n")), 1);
        // Not in the bank.
        assert_eq!(count_runtime_thunks(&chars("  bl Func_080072e0\n")), 0);
        // Trailing word character defeats `\b`.
        assert_eq!(count_runtime_thunks(&chars("  bl Func_080073180\n")), 0);
    }

    #[test]
    fn m2c_markers_respect_word_boundaries() {
        assert_eq!(count_unknown_types(&chars("M2C_UNKNOWN x;")), 0);
        assert_eq!(count_unknown_types(&chars("XM2C_UNK x;")), 0);
        assert_eq!(count_unknown_types(&chars("(M2C_UNK)")), 1);
        assert_eq!(count_unset_registers(&chars("M2C_ERROR  (x)")), 1);
        assert_eq!(count_unset_registers(&chars("M2C_ERROR\n(x)")), 1);
        assert_eq!(count_unset_registers(&chars("M2C_ERRORS(x)")), 0);
        assert_eq!(count_unset_registers(&chars("M2C_ERROR x")), 0);
    }

    #[test]
    fn high_register_setup_detection() {
        assert!(has_high_register_setup(&chars("  mov r8, r0")));
        assert!(has_high_register_setup(&chars("  mov  fp , r0")));
        assert!(has_high_register_setup(&chars("  mov\n  sl,r0")));
        assert!(!has_high_register_setup(&chars("  mov r1, r0")));
        assert!(!has_high_register_setup(&chars("  movs r8, r0")));
        assert!(!has_high_register_setup(&chars("  vmov r8, r0")));
        // `r8` must be followed by a comma.
        assert!(!has_high_register_setup(&chars("  mov r8x, r0")));
    }

    #[test]
    fn established_publisher_detection() {
        assert!(has_established_thunk_publisher(&chars(
            "  bl Func_080cef64\n"
        )));
        assert!(has_established_thunk_publisher(&chars(
            "  BL FUNC_080ED408\n"
        )));
        // Indentation is irrelevant: the pattern is unanchored.
        assert!(has_established_thunk_publisher(&chars(
            "x bl Func_080cef64"
        )));
        assert!(!has_established_thunk_publisher(&chars(
            "  bl Func_080cef640\n"
        )));
        assert!(!has_established_thunk_publisher(&chars(
            "  bl Func_080cef6\n"
        )));
        assert!(!has_established_thunk_publisher(&chars(
            "  xbl Func_080cef64"
        )));
    }

    #[test]
    fn source_stem_names() {
        assert!(is_source_stem_name("08001234.c"));
        assert!(is_source_stem_name("08ABCDEF.C"));
        assert!(!is_source_stem_name("02001234.c"));
        assert!(!is_source_stem_name("08001234.h"));
        assert!(!is_source_stem_name("08001234.c.bak"));
        // JS `$` has no "before a trailing newline" allowance.
        assert!(!is_source_stem_name("08001234.c\n"));
        assert!(!is_source_stem_name("0800123.c"));
    }

    /// JS sorts strings by UTF-16 code unit; the stems here are ASCII hex, so
    /// byte order, code-unit order and ICU root collation all agree. Pin it.
    #[test]
    fn stem_order_matches_js() {
        let mut stems = vec!["08ffffff", "08000001", "080a0000", "08000010"];
        stems.sort_unstable();
        assert_eq!(stems, ["08000001", "08000010", "080a0000", "08ffffff"]);
        assert_eq!(stem_of(0x0800_12f4), "080012f4");
        assert_eq!(stem_of(0x1234_5678), "12345678");
    }

    #[test]
    fn sort_uses_score_then_bytes_desc_then_stem() {
        let base = Candidate {
            stem: "08000000".into(),
            bytes: 0,
            calls: 0,
            source_lines: 0,
            unset_registers: 0,
            internal_exits: 0,
            unknown_types: 0,
            high_register_call_setups: 0,
            runtime_thunk_calls: 0,
            established_thunk_family: None,
            boundary_shape: "c_candidate".into(),
            scope_audit_required: false,
            blocker_class: None,
            blocked_reason: None,
            draft: "work/08000000.c".into(),
            score: 0,
        };
        let mut queue = [
            Candidate {
                stem: "08000003".into(),
                score: 5,
                bytes: 10,
                ..base.clone()
            },
            Candidate {
                stem: "08000001".into(),
                score: 5,
                bytes: 20,
                ..base.clone()
            },
            Candidate {
                stem: "08000002".into(),
                score: 5,
                bytes: 20,
                ..base.clone()
            },
            Candidate {
                stem: "08000000".into(),
                score: 1,
                bytes: 1,
                ..base.clone()
            },
        ];
        queue.sort_by(|left, right| {
            left.score
                .cmp(&right.score)
                .then(right.bytes.cmp(&left.bytes))
                .then(left.stem.cmp(&right.stem))
        });
        let order: Vec<&str> = queue.iter().map(|item| item.stem.as_str()).collect();
        assert_eq!(order, ["08000000", "08000001", "08000002", "08000003"]);
    }

    #[test]
    fn score_rounds_source_lines_up() {
        // Math.ceil(sourceLines / 10)
        assert_eq!(ceil_div_10(1), 1);
        assert_eq!(ceil_div_10(10), 1);
        assert_eq!(ceil_div_10(11), 2);
        assert_eq!(ceil_div_10(0), 0);
        assert_eq!(ceil_div_10(9), 1);
    }

    #[test]
    fn source_lines_counts_split_segments() {
        // "a\nb".split("\n").length === 2; "a\n".split("\n").length === 2.
        assert_eq!("a\nb".split('\n').count(), 2);
        assert_eq!("a\n".split('\n').count(), 2);
        assert_eq!("".split('\n').count(), 1);
    }

    #[test]
    fn lossy_decoding_matches_node() {
        // Node replaces invalid bytes with U+FFFD instead of throwing.
        let decoded = String::from_utf8_lossy(b"M2C_UNK \xff\xfe x");
        assert!(decoded.contains('\u{fffd}'));
        assert_eq!(count_unknown_types(&chars(&decoded)), 1);
    }

    #[test]
    fn parse_int_matches_javascript() {
        assert_eq!(js_parse_int("30"), Some(30.0));
        assert_eq!(js_parse_int("5x"), Some(5.0));
        assert_eq!(js_parse_int(" 7 "), Some(7.0));
        assert_eq!(js_parse_int("-3"), Some(-3.0));
        assert_eq!(js_parse_int("+8"), Some(8.0));
        assert_eq!(js_parse_int("abc"), None);
        assert_eq!(js_parse_int(""), None);
        // Integral floats: parseInt stops at the dot, unlike Number("7.0").
        assert_eq!(js_parse_int("7.9"), Some(7.0));
    }

    #[test]
    fn slice_limit_matches_javascript() {
        assert_eq!(js_slice_limit(10, Some(3.0)), 3);
        assert_eq!(js_slice_limit(10, Some(99.0)), 10);
        // slice(0, NaN) is empty.
        assert_eq!(js_slice_limit(10, None), 0);
        // A negative end counts back from the end of the array.
        assert_eq!(js_slice_limit(10, Some(-3.0)), 7);
        assert_eq!(js_slice_limit(10, Some(-99.0)), 0);
        assert_eq!(js_slice_limit(0, Some(5.0)), 0);
    }

    #[test]
    fn row_formatting_pads_like_padstart() {
        let item = Candidate {
            stem: "08001234".into(),
            bytes: 42,
            calls: 3,
            source_lines: 12,
            unset_registers: 1,
            internal_exits: 0,
            unknown_types: 2,
            high_register_call_setups: 0,
            runtime_thunk_calls: 1,
            established_thunk_family: Some("renderer".into()),
            boundary_shape: "split_first".into(),
            scope_audit_required: true,
            blocker_class: Some("multi_region_function".into()),
            blocked_reason: Some("because".into()),
            draft: "work/candidates/08001234.c".into(),
            score: 7,
        };
        assert_eq!(
            format_row(&item),
            "08001234    42B score=   7 calls=  3 unset=1 exits=0 higharg=0 \
thunks=1 family=renderer unk= 2 shape=split_first scope=transitive-unsized \
blocked=multi_region_function work/candidates/08001234.c"
        );
        let plain = Candidate {
            established_thunk_family: None,
            scope_audit_required: false,
            blocker_class: None,
            blocked_reason: None,
            boundary_shape: "c_candidate".into(),
            ..item
        };
        assert_eq!(
            format_row(&plain),
            "08001234    42B score=   7 calls=  3 unset=1 exits=0 higharg=0 \
thunks=1 unk= 2 shape=c_candidate work/candidates/08001234.c"
        );
    }

    #[test]
    fn json_omits_undefined_and_keeps_insertion_order() {
        let item = Candidate {
            stem: "08001234".into(),
            bytes: 42,
            calls: 3,
            source_lines: 12,
            unset_registers: 1,
            internal_exits: 0,
            unknown_types: 2,
            high_register_call_setups: 0,
            runtime_thunk_calls: 1,
            established_thunk_family: None,
            boundary_shape: "c_candidate".into(),
            scope_audit_required: false,
            blocker_class: None,
            blocked_reason: None,
            draft: "work/08001234.c".into(),
            score: 7,
        };
        let json = to_json(&[item]);
        assert!(!json.contains("establishedThunkFamily"));
        assert!(!json.contains("blockerClass"));
        let stem_at = json.find("\"stem\"").unwrap();
        let bytes_at = json.find("\"bytes\"").unwrap();
        let score_at = json.find("\"score\"").unwrap();
        let draft_at = json.find("\"draft\"").unwrap();
        assert!(stem_at < bytes_at && draft_at < score_at);
    }

    #[test]
    fn analyze_candidate_scores_a_synthetic_owner() {
        let root = std::env::temp_dir().join(format!(
            "semantic-queue-test-{}-{}",
            std::process::id(),
            line!()
        ));
        let asm_dir = root.join("asm");
        fs::create_dir_all(&asm_dir).unwrap();
        fs::write(
            asm_dir.join("08001234.s"),
            "  mov r8, r0\n  bl Func_080cef64\n  bl Func_080072e4\n  b.n Func_08005678\n",
        )
        .unwrap();
        let draft_path = root.join("draft.c");
        fs::write(
            &draft_path,
            "M2C_UNK f(void) {\n  M2C_ERROR(/* r0 */);\n}\n",
        )
        .unwrap();
        let region = Region {
            address: 0x0800_1234,
            size: 64,
            source: "asm/08001234.s".into(),
            retention: "c_candidate".into(),
        };
        let candidate = analyze_candidate(&root, &region, &draft_path);
        fs::remove_dir_all(&root).unwrap();

        assert_eq!(candidate.stem, "08001234");
        assert_eq!(candidate.calls, 2);
        assert_eq!(candidate.internal_exits, 1);
        assert_eq!(candidate.runtime_thunk_calls, 1);
        assert_eq!(candidate.unset_registers, 1);
        assert_eq!(candidate.unknown_types, 1);
        assert_eq!(candidate.high_register_call_setups, 2);
        assert_eq!(
            candidate.established_thunk_family.as_deref(),
            Some("renderer")
        );
        assert_eq!(candidate.source_lines, 4);
        assert!(!candidate.scope_audit_required);
        assert_eq!(candidate.draft, "draft.c");
        // 2*20 + 1*200 + 1*250 + 1*8 + 2*40 + 1*40 + 0 + ceil(4/10)
        assert_eq!(candidate.score, 40 + 200 + 250 + 8 + 80 + 40 + 1);
    }

    #[test]
    fn unestablished_family_carries_the_full_thunk_penalty() {
        let root = std::env::temp_dir().join(format!(
            "semantic-queue-test-{}-{}",
            std::process::id(),
            line!()
        ));
        let asm_dir = root.join("asm");
        fs::create_dir_all(&asm_dir).unwrap();
        fs::write(asm_dir.join("08001234.s"), "  bl Func_08007318\n").unwrap();
        let draft_path = root.join("draft.c");
        fs::write(&draft_path, "void f(void) {}\n").unwrap();
        let region = Region {
            address: 0x0800_1234,
            size: 64,
            source: "asm/08001234.s".into(),
            retention: "split_first".into(),
        };
        let candidate = analyze_candidate(&root, &region, &draft_path);
        fs::remove_dir_all(&root).unwrap();

        assert_eq!(candidate.established_thunk_family, None);
        assert!(candidate.scope_audit_required);
        // 1*20 + 1*250 (thunk) + 5000 (boundary) + ceil(2/10)
        assert_eq!(candidate.score, 20 + 250 + 5_000 + 1);
    }
}
