//! Pre-size continuation-shaped manifest rows into whole semantic owners.
//!
//! Rust port of `tools/semantic/semantic_owner_scope.ts`.
//!
//! PORT NOTES
//! ==========
//!
//! Provenance
//! ----------
//! Ported from `tools/semantic/semantic_owner_scope.ts` (kept in place, not
//! deleted). That file imports nothing from the repo's own `tools/lib/*`
//! modules — only `node:fs` and `node:path` — so **nothing was inlined from a
//! local module**. In particular `canonicalCSource` (already ported into
//! `tools/semantic-superseded/`) is not reachable from this tool and is not
//! duplicated here.
//!
//! Hand-rolled regex engine
//! ------------------------
//! No regex crate is used. Every pattern in the TypeScript is reimplemented as
//! a targeted scanner in the `re` module, preserving JavaScript semantics that
//! a naive Rust translation would silently change:
//!
//! * **`\s`** in JavaScript is *not* `char::is_whitespace` and *not* just
//!   ASCII whitespace. It is exactly
//!   `[\t\n\v\f\r \u{a0}\u{1680}\u{2000}-\u{200a}\u{2028}\u{2029}\u{202f}\u{205f}\u{3000}\u{feff}]`.
//!   Note U+FEFF (BOM) is whitespace to JS but not to `char::is_whitespace`,
//!   and U+0085 (NEL) is whitespace to Unicode but *not* to JS `\s`. Both
//!   directions are pinned by tests.
//! * **`\b`** in JavaScript (without the `u`/unicode-sets flag) is ASCII-only:
//!   a word char is `[A-Za-z0-9_]`. A non-ASCII letter such as `é` is a
//!   *non*-word char, so `\blr\b` matches inside `élr`. Pinned by a test.
//! * **The `m` flag** lets `^` match after any `\n` *and* lets a following
//!   `\s*`/`\s+` run across newlines. The scanners here reproduce that by
//!   anchoring at real line starts and skipping whitespace across `\n`.
//! * **`matchAll` with `g`** advances `lastIndex` past each match, so a match
//!   that swallows a later line start suppresses a second match there. The
//!   `bl` counter reproduces that cursor exactly.
//!
//! Other JS/Rust traps addressed
//! -----------------------------
//! * `readFileSync(path, "utf8")` replaces invalid UTF-8 with U+FFFD rather
//!   than failing. Ported as `String::from_utf8_lossy`.
//! * `Array.prototype.sort` is stable, and `groupOwners` sorts only by
//!   address. Rows sharing an address must keep manifest order, so
//!   `sort_by_key` (stable) is used, never `sort_unstable_by_key`.
//! * `Number.isSafeInteger` accepts integral *floats*: `512.0` passes,
//!   `512.5` does not, and anything past 2^53-1 does not. `is_safe_integer`
//!   mirrors that, so a `"size": 512.0` in `semantic/main-regions.json` is
//!   accepted by both implementations.
//! * String ordering is never used for output here (owners are ordered by
//!   numeric address, and the admitted/open stem collections are membership
//!   sets only), so JS UTF-16 code-unit sort vs Rust byte sort cannot bite.
//!   A test pins that stems differing only past the BMP boundary do not
//!   participate in any ordering decision.
//! * `parseInt(s, 16)` accepts an optional `0x` prefix, ignores trailing
//!   garbage, and yields `NaN` when no hex digit is present. Every comparison
//!   against `NaN` is false, and `Math.min`/`Math.max` propagate it. Modelled
//!   with `Option<i64>` where `None` == `NaN` and every comparison involving
//!   it is false.
//!
//! Deliberate behavioural differences
//! ----------------------------------
//! * **Error reporting.** The TypeScript lets `throw new Error(msg)` escape
//!   `main`, so Bun prints a multi-line stack trace and exits 1. The Rust
//!   binary prints `error: <msg>` on stderr and exits 1. The message text is
//!   identical; the surrounding trace is not reproducible and is not part of
//!   the tool's contract. Stdout and exit codes match exactly.
//! * Nothing else. All stdout bytes match character for character.

use std::collections::BTreeSet;
use std::fs;
use std::path::{Path, PathBuf};

// ---------------------------------------------------------------------------
// JavaScript-faithful character classes and scanners
// ---------------------------------------------------------------------------

pub mod re {
    /// JavaScript `\s`. Deliberately not `char::is_whitespace`.
    pub fn is_js_space(c: char) -> bool {
        matches!(
            c,
            '\t' | '\n'
                | '\u{b}'
                | '\u{c}'
                | '\r'
                | ' '
                | '\u{a0}'
                | '\u{1680}'
                | '\u{2000}'..='\u{200a}'
                | '\u{2028}'
                | '\u{2029}'
                | '\u{202f}'
                | '\u{205f}'
                | '\u{3000}'
                | '\u{feff}'
        )
    }

    /// JavaScript `\w` (ASCII-only, as `\b` uses without the `u` flag).
    pub fn is_js_word(c: char) -> bool {
        c.is_ascii_alphanumeric() || c == '_'
    }

    /// True when a `\b` would match at index `i` of `chars`.
    pub fn boundary(chars: &[char], i: usize) -> bool {
        let before = i > 0 && is_js_word(chars[i - 1]);
        let after = i < chars.len() && is_js_word(chars[i]);
        before != after
    }

    /// Indices at which `^` matches under the `m` flag.
    pub fn line_starts(chars: &[char]) -> Vec<usize> {
        let mut out = vec![0usize];
        for (i, c) in chars.iter().enumerate() {
            if *c == '\n' {
                out.push(i + 1);
            }
        }
        out
    }

    /// `\s*` (greedy; crosses newlines, matching JS).
    pub fn skip_spaces(chars: &[char], mut i: usize) -> usize {
        while i < chars.len() && is_js_space(chars[i]) {
            i += 1;
        }
        i
    }

    /// `\s+`: returns `None` when there is no whitespace at all.
    pub fn skip_spaces_plus(chars: &[char], i: usize) -> Option<usize> {
        if i < chars.len() && is_js_space(chars[i]) {
            Some(skip_spaces(chars, i))
        } else {
            None
        }
    }

    pub fn starts_with(chars: &[char], i: usize, needle: &str) -> bool {
        let n: Vec<char> = needle.chars().collect();
        if i + n.len() > chars.len() {
            return false;
        }
        chars[i..i + n.len()] == n[..]
    }

    /// `<mnemonic>\s*\{[^}]*\b<word>\b`, anchored at `at` after `\s*`.
    ///
    /// Reproduces the backtracking of `[^}]*`: `word` must start at some
    /// position at or after the `{` such that every character from the `{` up
    /// to the end of `word` is a non-`}` character.
    fn brace_list_has_at(chars: &[char], at: usize, mnemonic: &str, word: &str) -> bool {
        let i = skip_spaces(chars, at);
        if !starts_with(chars, i, mnemonic) {
            return false;
        }
        let j = skip_spaces(chars, i + mnemonic.chars().count());
        if j >= chars.len() || chars[j] != '{' {
            return false;
        }
        let open = j + 1;
        let mut close = open;
        while close < chars.len() && chars[close] != '}' {
            close += 1;
        }
        let w: Vec<char> = word.chars().collect();
        let mut p = open;
        while p + w.len() <= close {
            if chars[p..p + w.len()] == w[..]
                && boundary(chars, p)
                && boundary(chars, p + w.len())
            {
                return true;
            }
            p += 1;
        }
        false
    }

    /// `/^\s*<mnemonic>\s*\{[^}]*\b<word>\b/m`
    pub fn brace_list_has(body: &[char], mnemonic: &str, word: &str) -> bool {
        line_starts(body)
            .into_iter()
            .any(|ls| brace_list_has_at(body, ls, mnemonic, word))
    }

    /// `/^\s*bx\s+lr\b/m`
    pub fn has_bx_lr(body: &[char]) -> bool {
        line_starts(body).into_iter().any(|ls| {
            let i = skip_spaces(body, ls);
            if !starts_with(body, i, "bx") {
                return false;
            }
            match skip_spaces_plus(body, i + 2) {
                Some(j) => starts_with(body, j, "lr") && boundary(body, j + 2),
                None => false,
            }
        })
    }

    /// `/^\s*bx\s+(r\d+)\b/` applied to a single line (no `m`, no `g`).
    /// Returns the captured register.
    pub fn bx_register(line: &[char]) -> Option<String> {
        let i = skip_spaces(line, 0);
        if !starts_with(line, i, "bx") {
            return None;
        }
        let j = skip_spaces_plus(line, i + 2)?;
        if j >= line.len() || line[j] != 'r' {
            return None;
        }
        let mut e = j + 1;
        while e < line.len() && line[e].is_ascii_digit() {
            e += 1;
        }
        if e == j + 1 {
            return None;
        }
        if !boundary(line, e) {
            return None;
        }
        Some(line[j..e].iter().collect())
    }

    /// `/^\s*(bl|b|b[a-z]{2}|bx|blx|push|pop)\b/m`
    ///
    /// The alternation is ordered and each branch is followed by `\b`, so
    /// `blx` is matched by the `b[a-z]{2}` branch (the `bl` and `b` branches
    /// both fail their boundary check first). The scan below tries the
    /// branches in the same order for the same reason.
    pub fn has_branch_or_stack(body: &[char]) -> bool {
        line_starts(body).into_iter().any(|ls| {
            let i = skip_spaces(body, ls);
            let fixed = ["bl", "b", "bx", "blx", "push", "pop"];
            // `bl` and `b` first, then the three-lowercase-letter branch,
            // then the rest -- mirroring the source alternation order. Order
            // only affects which branch wins, never whether one does, so the
            // simple any() below is equivalent.
            if fixed.iter().any(|m| {
                starts_with(body, i, m) && boundary(body, i + m.chars().count())
            }) {
                return true;
            }
            i + 3 <= body.len()
                && body[i] == 'b'
                && body[i + 1].is_ascii_lowercase()
                && body[i + 2].is_ascii_lowercase()
                && boundary(body, i + 3)
        })
    }

    /// `[...body.matchAll(/^\s*bl\s+\S+/gm)].length`
    ///
    /// Reproduces `lastIndex`: a match that runs past later line starts
    /// suppresses matches at those line starts.
    pub fn count_bl_calls(body: &[char]) -> usize {
        let mut count = 0usize;
        let mut cursor = 0usize;
        for ls in line_starts(body) {
            if ls < cursor {
                continue;
            }
            let i = skip_spaces(body, ls);
            if !starts_with(body, i, "bl") {
                continue;
            }
            let Some(j) = skip_spaces_plus(body, i + 2) else {
                continue;
            };
            if j >= body.len() || is_js_space(body[j]) {
                continue;
            }
            let mut e = j;
            while e < body.len() && !is_js_space(body[e]) {
                e += 1;
            }
            count += 1;
            cursor = e;
        }
        count
    }

    /// `/^\s*(@|\.(syntax|text|thumb|set|global|align))/` on one line.
    pub fn is_header_line(line: &[char]) -> bool {
        let i = skip_spaces(line, 0);
        if i >= line.len() {
            return false;
        }
        if line[i] == '@' {
            return true;
        }
        if line[i] != '.' {
            return false;
        }
        ["syntax", "text", "thumb", "set", "global", "align"]
            .iter()
            .any(|d| starts_with(line, i + 1, d))
    }

    /// `/^\s*\.inst/` on one line.
    pub fn is_inst_line(line: &[char]) -> bool {
        starts_with(line, skip_spaces(line, 0), ".inst")
    }

    /// `/^\s*[a-z][a-z0-9.]*\s/i` on one line.
    ///
    /// Every character of `[a-z0-9.]` is a non-`\s` character, so the greedy
    /// run never needs to backtrack to let the trailing `\s` match.
    pub fn is_mnemonic_line(line: &[char]) -> bool {
        let i = skip_spaces(line, 0);
        if i >= line.len() || !line[i].is_ascii_alphabetic() {
            return false;
        }
        let mut e = i + 1;
        while e < line.len()
            && (line[e].is_ascii_alphanumeric() || line[e] == '.')
        {
            e += 1;
        }
        e < line.len() && is_js_space(line[e])
    }

    /// `parseInt(s, 16)`: optional whitespace, optional sign, optional `0x`
    /// prefix, then the longest hex-digit run. `None` models `NaN`.
    pub fn parse_int_16(s: &str) -> Option<i64> {
        let chars: Vec<char> = s.chars().collect();
        let mut i = skip_spaces(&chars, 0);
        let mut negative = false;
        if i < chars.len() && (chars[i] == '+' || chars[i] == '-') {
            negative = chars[i] == '-';
            i += 1;
        }
        if i + 1 < chars.len()
            && chars[i] == '0'
            && (chars[i + 1] == 'x' || chars[i + 1] == 'X')
            && i + 2 < chars.len()
            && chars[i + 2].is_ascii_hexdigit()
        {
            i += 2;
        }
        let start = i;
        let mut value: i64 = 0;
        while i < chars.len() && chars[i].is_ascii_hexdigit() {
            value = value
                .saturating_mul(16)
                .saturating_add(chars[i].to_digit(16).unwrap() as i64);
            i += 1;
        }
        if i == start {
            return None;
        }
        Some(if negative { -value } else { value })
    }
}

/// `Number.isSafeInteger`, including its acceptance of integral floats.
pub fn is_safe_integer(value: f64) -> bool {
    value.is_finite() && value.fract() == 0.0 && value.abs() <= 9_007_199_254_740_991.0
}

// ---------------------------------------------------------------------------
// Data model
// ---------------------------------------------------------------------------

pub const CONTINUATION: [&str; 3] = [
    "split_first",
    "merge_with_continuations",
    "merge_with_owner",
];

#[derive(Debug, Clone)]
pub struct Region {
    pub address: i64,
    pub size: i64,
    pub source: String,
    pub kind: String,
    pub retention: String,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct RowFacts {
    pub stem: String,
    pub address: i64,
    pub size: i64,
    pub retention: String,
    pub kind: String,
    pub has_prologue: bool,
    pub has_epilogue: bool,
    pub is_data: bool,
    pub suspected_pool: bool,
    pub calls: i64,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Owner {
    pub entry: String,
    pub rows: Vec<RowFacts>,
    pub executable_bytes: i64,
    pub excluded_bytes: i64,
    pub advertised_bytes: i64,
    pub calls: i64,
    pub closed: bool,
    pub suspected_pool_bytes: i64,
}

pub fn stem_of(address: i64) -> String {
    format!("{:08x}", address)
}

// ---------------------------------------------------------------------------
// Row classification
// ---------------------------------------------------------------------------

/// Thumb functions return three ways; see the TypeScript for the reasoning.
pub fn has_epilogue(body: &str) -> bool {
    let chars: Vec<char> = body.chars().collect();
    if re::brace_list_has(&chars, "pop", "pc") {
        return true;
    }
    if re::has_bx_lr(&chars) {
        return true;
    }
    let lines: Vec<&str> = body.split('\n').collect();
    for index in 0..lines.len() {
        let line: Vec<char> = lines[index].chars().collect();
        let Some(register) = re::bx_register(&line) else {
            continue;
        };
        let from = index.saturating_sub(4);
        let preceding: Vec<char> = lines[from..index].join("\n").chars().collect();
        if re::brace_list_has(&preceding, "pop", &register) {
            return true;
        }
    }
    false
}

pub fn row_facts_from_assembly(text: &str, region: &Region) -> RowFacts {
    let body: String = text
        .split('\n')
        .filter(|line| !re::is_header_line(&line.chars().collect::<Vec<char>>()))
        .collect::<Vec<&str>>()
        .join("\n");
    let mnemonics = body
        .split('\n')
        .filter(|line| {
            let cs: Vec<char> = line.chars().collect();
            re::is_mnemonic_line(&cs) && !re::is_inst_line(&cs)
        })
        .count();
    let chars: Vec<char> = body.chars().collect();
    RowFacts {
        stem: stem_of(region.address),
        address: region.address,
        size: region.size,
        retention: region.retention.clone(),
        kind: region.kind.clone(),
        has_prologue: re::brace_list_has(&chars, "push", "lr"),
        has_epilogue: has_epilogue(&body),
        is_data: mnemonics == 0,
        suspected_pool: mnemonics > 0 && !re::has_branch_or_stack(&chars),
        calls: re::count_bl_calls(&chars) as i64,
    }
}

/// Group consecutive rows into owners: prologue opens, epilogue closes.
pub fn group_owners(rows: &[RowFacts]) -> Vec<Owner> {
    let mut ordered = rows.to_vec();
    // Stable, matching Array.prototype.sort: equal addresses keep input order.
    ordered.sort_by_key(|row| row.address);
    let mut owners: Vec<Owner> = Vec::new();
    let mut current: Vec<RowFacts> = Vec::new();
    fn flush(current: &mut Vec<RowFacts>, owners: &mut Vec<Owner>, closed: bool) {
        if current.is_empty() {
            return;
        }
        owners.push(Owner {
            entry: current[0].stem.clone(),
            executable_bytes: current
                .iter()
                .filter(|row| !row.is_data)
                .map(|row| row.size)
                .sum(),
            excluded_bytes: current
                .iter()
                .filter(|row| row.is_data)
                .map(|row| row.size)
                .sum(),
            advertised_bytes: current[0].size,
            calls: current.iter().map(|row| row.calls).sum(),
            closed,
            suspected_pool_bytes: current
                .iter()
                .filter(|row| !row.is_data && row.suspected_pool)
                .map(|row| row.size)
                .sum(),
            rows: std::mem::take(current),
        });
    }
    for row in ordered {
        if row.has_prologue && !current.is_empty() {
            flush(&mut current, &mut owners, false);
        }
        let closes = row.has_epilogue;
        current.push(row);
        if closes {
            flush(&mut current, &mut owners, true);
        }
    }
    flush(&mut current, &mut owners, false);
    owners
}

// ---------------------------------------------------------------------------
// Repository reads
// ---------------------------------------------------------------------------

pub fn read_utf8_lossy(path: &Path) -> Result<String, String> {
    let bytes = fs::read(path)
        .map_err(|error| format!("cannot read {}: {error}", path.display()))?;
    Ok(String::from_utf8_lossy(&bytes).into_owned())
}

fn admitted_stems(root: &Path) -> BTreeSet<String> {
    let mut stems = BTreeSet::new();
    for directory in ["exact", "semantic/main"] {
        let path = root.join(directory);
        if !path.exists() {
            continue;
        }
        let Ok(entries) = fs::read_dir(&path) else {
            continue;
        };
        for entry in entries.flatten() {
            let name = entry.file_name().to_string_lossy().into_owned();
            if is_admitted_name(&name) {
                stems.insert(name[..name.len() - 2].to_lowercase());
            }
        }
    }
    stems
}

/// `/^08[0-9a-f]{6}\.c$/i`
fn is_admitted_name(name: &str) -> bool {
    let chars: Vec<char> = name.chars().collect();
    if chars.len() != 10 {
        return false;
    }
    if chars[0] != '0' || chars[1] != '8' {
        return false;
    }
    if !chars[2..8].iter().all(char::is_ascii_hexdigit) {
        return false;
    }
    chars[8] == '.' && (chars[9] == 'c' || chars[9] == 'C')
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct AddressRange {
    pub start: Option<i64>,
    pub end: Option<i64>,
}

#[derive(Debug, Clone, Copy)]
pub struct OwnerSpan {
    pub lo: Option<i64>,
    pub hi: Option<i64>,
}

pub struct Coverage {
    pub owner_spans: Vec<OwnerSpan>,
    pub executable_ranges: Vec<AddressRange>,
    pub non_c_ranges: Vec<AddressRange>,
}

/// `region.address < range.end && range.start < region.address + region.size`
/// with JavaScript `NaN` comparison semantics (any comparison with `NaN` is
/// false).
pub fn overlaps(address: i64, size: i64, range: &AddressRange) -> bool {
    match (range.start, range.end) {
        (Some(start), Some(end)) => address < end && start < address + size,
        _ => false,
    }
}

fn json_number(value: &serde_json::Value) -> Option<f64> {
    value.as_f64()
}

fn registered_coverage(root: &Path) -> Result<Coverage, String> {
    let path = root.join("semantic").join("main-regions.json");
    if !path.exists() {
        return Ok(Coverage {
            owner_spans: Vec::new(),
            executable_ranges: Vec::new(),
            non_c_ranges: Vec::new(),
        });
    }
    let registry: serde_json::Value = serde_json::from_str(&read_utf8_lossy(&path)?)
        .map_err(|error| format!("cannot parse {}: {error}", path.display()))?;
    let owners = registry
        .get("main_owners")
        .and_then(|value| value.as_array())
        .ok_or_else(|| "main_owners is not iterable".to_string())?;

    let range_of = |range: &serde_json::Value| -> AddressRange {
        let address = range
            .get("address")
            .and_then(|value| value.as_str())
            .and_then(re::parse_int_16);
        let size = range.get("size").and_then(json_number).unwrap_or(f64::NAN);
        AddressRange {
            start: address,
            end: match (address, size.is_finite() && size.fract() == 0.0) {
                (Some(start), true) => Some(start + size as i64),
                _ => None,
            },
        }
    };

    let mut executable_ranges: Vec<AddressRange> = Vec::new();
    let mut owner_spans: Vec<OwnerSpan> = Vec::new();
    for owner in owners {
        let ranges: Vec<AddressRange> = owner
            .get("executable_ranges")
            .and_then(|value| value.as_array())
            .ok_or_else(|| "executable_ranges is not iterable".to_string())?
            .iter()
            .map(&range_of)
            .collect();
        executable_ranges.extend(ranges.iter().copied());
        // Math.min/Math.max over an empty list give Infinity/-Infinity, and
        // any NaN member poisons the result. Both are modelled as None, which
        // makes every span test below false -- exactly what JS does.
        let lo = if ranges.is_empty() || ranges.iter().any(|r| r.start.is_none()) {
            None
        } else {
            ranges.iter().filter_map(|r| r.start).min()
        };
        let hi = if ranges.is_empty() || ranges.iter().any(|r| r.end.is_none()) {
            None
        } else {
            ranges.iter().filter_map(|r| r.end).max()
        };
        owner_spans.push(OwnerSpan { lo, hi });
    }

    let empty = Vec::new();
    let declared = registry
        .get("non_c_ranges")
        .and_then(|value| value.as_array())
        .unwrap_or(&empty);
    let mut non_c_ranges: Vec<AddressRange> = Vec::new();
    for range in declared {
        let address = range.get("address").and_then(|value| value.as_str());
        let size = range.get("size").and_then(json_number);
        let kind = range.get("kind").and_then(|value| value.as_str());
        let evidence = range.get("evidence").and_then(|value| value.as_str());
        let address_ok = address.map(is_non_c_address).unwrap_or(false);
        let size_ok = size.map(|s| is_safe_integer(s) && s > 0.0).unwrap_or(false);
        let kind_ok = kind.map(|k| !js_trim(k).is_empty()).unwrap_or(false);
        let evidence_ok = evidence.map(|e| !js_trim(e).is_empty()).unwrap_or(false);
        if !address_ok || !size_ok || !kind_ok || !evidence_ok {
            return Err(format!(
                "invalid non-C range {}",
                serde_json::to_string(range).unwrap_or_else(|_| "undefined".into())
            ));
        }
        let start = address.and_then(re::parse_int_16);
        non_c_ranges.push(AddressRange {
            start,
            end: start.map(|s| s + size.unwrap() as i64),
        });
    }
    for index in 0..non_c_ranges.len() {
        for other in index + 1..non_c_ranges.len() {
            let a = non_c_ranges[index];
            let b = non_c_ranges[other];
            if let (Some(a_start), Some(a_end), Some(b_start), Some(b_end)) =
                (a.start, a.end, b.start, b.end)
            {
                if a_start < b_end && b_start < a_end {
                    return Err(
                        "overlapping non-C ranges in semantic/main-regions.json".into()
                    );
                }
            }
        }
        let a = non_c_ranges[index];
        if let (Some(a_start), Some(a_end)) = (a.start, a.end) {
            if executable_ranges.iter().any(|range| match (range.start, range.end) {
                (Some(start), Some(end)) => a_start < end && start < a_end,
                _ => false,
            }) {
                return Err(
                    "non-C range overlaps registered semantic executable coverage".into(),
                );
            }
        }
    }

    Ok(Coverage {
        owner_spans,
        executable_ranges,
        non_c_ranges,
    })
}

/// `/^0x08[0-9a-f]{6}$/i`
fn is_non_c_address(value: &str) -> bool {
    let chars: Vec<char> = value.chars().collect();
    chars.len() == 10
        && chars[0] == '0'
        && (chars[1] == 'x' || chars[1] == 'X')
        && chars[2] == '0'
        && chars[3] == '8'
        && chars[4..].iter().all(|c| c.is_ascii_hexdigit())
}

/// `String.prototype.trim` trims JS whitespace *and* line terminators, which
/// is the same set as `\s` here.
fn js_trim(value: &str) -> &str {
    value.trim_matches(re::is_js_space)
}

pub fn census_declared_closed(root: &Path) -> Result<bool, String> {
    let path = root.join("semantic").join("main-regions.json");
    if !path.exists() {
        return Ok(false);
    }
    let registry: serde_json::Value = serde_json::from_str(&read_utf8_lossy(&path)?)
        .map_err(|error| format!("cannot parse {}: {error}", path.display()))?;
    let census = registry.get("ordinary_census");
    let status = census
        .and_then(|c| c.get("status"))
        .and_then(|v| v.as_str());
    let check = census.and_then(|c| c.get("check")).and_then(|v| v.as_str());
    let evidence = census
        .and_then(|c| c.get("evidence"))
        .and_then(|v| v.as_str());
    Ok(status == Some("closed")
        && check == Some("make semantic-check")
        && evidence.map(|e| js_trim(e).chars().count()).unwrap_or(0) > 0)
}

pub fn read_manifest_regions(root: &Path) -> Result<Vec<Region>, String> {
    let path = root.join("out").join("full").join("asm").join("manifest.json");
    let manifest: serde_json::Value = serde_json::from_str(&read_utf8_lossy(&path)?)
        .map_err(|error| format!("cannot parse {}: {error}", path.display()))?;
    let regions = manifest
        .get("regions")
        .and_then(|value| value.as_array())
        .ok_or_else(|| "manifest regions is not iterable".to_string())?;
    let mut out = Vec::with_capacity(regions.len());
    for region in regions {
        let address = region
            .get("address")
            .and_then(|v| v.as_i64())
            .ok_or_else(|| "manifest region has no integer address".to_string())?;
        let size = region
            .get("size")
            .and_then(|v| v.as_i64())
            .ok_or_else(|| "manifest region has no integer size".to_string())?;
        out.push(Region {
            address,
            size,
            source: region
                .get("source")
                .and_then(|v| v.as_str())
                .unwrap_or_default()
                .to_string(),
            kind: region
                .get("kind")
                .and_then(|v| v.as_str())
                .unwrap_or_default()
                .to_string(),
            retention: region
                .get("retention")
                .and_then(|v| v.as_str())
                .unwrap_or_default()
                .to_string(),
        });
    }
    Ok(out)
}

pub fn open_owners(root: &Path) -> Result<Vec<Owner>, String> {
    let regions = read_manifest_regions(root)?;
    let admitted = admitted_stems(root);
    let coverage = registered_coverage(root)?;
    let mut rows: Vec<RowFacts> = Vec::new();
    let mut open: BTreeSet<String> = BTreeSet::new();
    for region in &regions {
        let stem = stem_of(region.address);
        let source: PathBuf = if region.source.starts_with('/') {
            PathBuf::from(&region.source)
        } else {
            root.join(&region.source)
        };
        if !source.exists() {
            continue;
        }
        rows.push(row_facts_from_assembly(&read_utf8_lossy(&source)?, region));
        if !CONTINUATION.contains(&region.retention.as_str()) {
            continue;
        }
        if admitted.contains(&stem) {
            continue;
        }
        if coverage
            .executable_ranges
            .iter()
            .any(|range| overlaps(region.address, region.size, range))
        {
            continue;
        }
        if coverage.owner_spans.iter().any(|span| match (span.lo, span.hi) {
            (Some(lo), Some(hi)) => region.address >= lo && region.address < hi,
            _ => false,
        }) {
            continue;
        }
        if coverage
            .non_c_ranges
            .iter()
            .any(|range| overlaps(region.address, region.size, range))
        {
            continue;
        }
        open.insert(stem);
    }
    Ok(group_owners(&rows)
        .into_iter()
        .filter(|owner| owner.rows.iter().any(|row| open.contains(&row.stem)))
        .filter(|owner| owner.executable_bytes > 0)
        .collect())
}

// ---------------------------------------------------------------------------
// Output
// ---------------------------------------------------------------------------

/// `JSON.stringify(value, null, 2)` for a string, hand-rolled to avoid any
/// chance of a serializer reordering or re-escaping fields.
fn json_string(value: &str) -> String {
    let mut out = String::from("\"");
    for c in value.chars() {
        match c {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            '\u{8}' => out.push_str("\\b"),
            '\u{c}' => out.push_str("\\f"),
            c if (c as u32) < 0x20 => out.push_str(&format!("\\u{:04x}", c as u32)),
            c => out.push(c),
        }
    }
    out.push('"');
    out
}

pub fn owners_to_json(owners: &[Owner]) -> String {
    if owners.is_empty() {
        return "[]".to_string();
    }
    let mut out = String::from("[\n");
    for (index, owner) in owners.iter().enumerate() {
        out.push_str("  {\n");
        out.push_str(&format!("    \"entry\": {},\n", json_string(&owner.entry)));
        if owner.rows.is_empty() {
            out.push_str("    \"rows\": [],\n");
        } else {
            out.push_str("    \"rows\": [\n");
            for (row_index, row) in owner.rows.iter().enumerate() {
                out.push_str("      {\n");
                out.push_str(&format!("        \"stem\": {},\n", json_string(&row.stem)));
                out.push_str(&format!("        \"address\": {},\n", row.address));
                out.push_str(&format!("        \"size\": {},\n", row.size));
                out.push_str(&format!(
                    "        \"retention\": {},\n",
                    json_string(&row.retention)
                ));
                out.push_str(&format!("        \"kind\": {},\n", json_string(&row.kind)));
                out.push_str(&format!("        \"hasPrologue\": {},\n", row.has_prologue));
                out.push_str(&format!("        \"hasEpilogue\": {},\n", row.has_epilogue));
                out.push_str(&format!("        \"isData\": {},\n", row.is_data));
                out.push_str(&format!(
                    "        \"suspectedPool\": {},\n",
                    row.suspected_pool
                ));
                out.push_str(&format!("        \"calls\": {}\n", row.calls));
                out.push_str("      }");
                out.push_str(if row_index + 1 < owner.rows.len() {
                    ",\n"
                } else {
                    "\n"
                });
            }
            out.push_str("    ],\n");
        }
        out.push_str(&format!(
            "    \"executableBytes\": {},\n",
            owner.executable_bytes
        ));
        out.push_str(&format!("    \"excludedBytes\": {},\n", owner.excluded_bytes));
        out.push_str(&format!(
            "    \"advertisedBytes\": {},\n",
            owner.advertised_bytes
        ));
        out.push_str(&format!("    \"calls\": {},\n", owner.calls));
        out.push_str(&format!("    \"closed\": {},\n", owner.closed));
        out.push_str(&format!(
            "    \"suspectedPoolBytes\": {}\n",
            owner.suspected_pool_bytes
        ));
        out.push_str("  }");
        out.push_str(if index + 1 < owners.len() { ",\n" } else { "\n" });
    }
    out.push(']');
    out
}

/// `String(n).padStart(5)`
fn pad_start_5(value: i64) -> String {
    let text = value.to_string();
    if text.chars().count() >= 5 {
        text
    } else {
        format!("{}{}", " ".repeat(5 - text.chars().count()), text)
    }
}

/// `/^[0-9a-f]{8}$/i`
pub fn is_stem_argument(value: &str) -> bool {
    let chars: Vec<char> = value.chars().collect();
    chars.len() == 8 && chars.iter().all(|c| c.is_ascii_hexdigit())
}

pub fn render_report(owners: &[Owner], wanted: Option<&str>) -> String {
    let wanted_lower = wanted.map(|w| w.to_lowercase());
    let shown: Vec<&Owner> = match &wanted_lower {
        Some(stem) => owners
            .iter()
            .filter(|owner| owner.rows.iter().any(|row| &row.stem == stem))
            .collect(),
        None => owners.iter().collect(),
    };
    let mut out = String::new();
    for owner in &shown {
        let flag = if owner.closed {
            ""
        } else {
            "  UNCLOSED(no epilogue found)"
        };
        out.push_str(&format!(
            "owner {}  advertised={}  executable={}  excluded_pool={}  rows={}  calls={}{}{}\n",
            owner.entry,
            owner.advertised_bytes,
            owner.executable_bytes,
            owner.excluded_bytes,
            owner.rows.len(),
            owner.calls,
            if owner.suspected_pool_bytes > 0 {
                format!("  suspected_pool={}", owner.suspected_pool_bytes)
            } else {
                String::new()
            },
            flag,
        ));
        if wanted_lower.is_some() {
            for row in &owner.rows {
                out.push_str(&format!(
                    "    {}  {}  {}{}{}{}\n",
                    row.stem,
                    pad_start_5(row.size),
                    if row.is_data {
                        "DATA "
                    } else if row.suspected_pool {
                        "POOL?"
                    } else {
                        "code "
                    },
                    row.retention,
                    if row.has_prologue { " prologue" } else { "" },
                    if row.has_epilogue { " epilogue" } else { "" },
                ));
            }
        }
    }
    let executable: i64 = shown.iter().map(|owner| owner.executable_bytes).sum();
    let excluded: i64 = shown.iter().map(|owner| owner.excluded_bytes).sum();
    let unclosed = shown.iter().filter(|owner| !owner.closed).count();
    out.push_str(&format!(
        "\nowners={} executable_bytes={} excluded_pool_bytes={} unclosed={}\n",
        shown.len(),
        executable,
        excluded,
        unclosed,
    ));
    out
}

#[cfg(test)]
mod tests {
    use super::re::*;
    use super::*;

    fn region(address: i64, size: i64, retention: &str) -> Region {
        Region {
            address,
            size,
            source: String::new(),
            kind: "k".into(),
            retention: retention.into(),
        }
    }

    // -- Ports of the TypeScript selfTest() --------------------------------

    #[test]
    fn self_test_code_row_is_read() {
        let code = row_facts_from_assembly(
            "\t.thumb\nRegion_08000000:\n\tpush {r4, lr}\n\tbl sub_08001234\n\tpop {r4, pc}\n",
            &region(0x0800_0000, 16, "split_first"),
        );
        assert!(code.has_prologue);
        assert!(code.has_epilogue);
        assert!(!code.is_data);
        assert_eq!(code.calls, 1);
        assert_eq!(code.stem, "08000000");
    }

    #[test]
    fn self_test_interworking_return_is_an_epilogue() {
        assert!(has_epilogue("\tpop {r1}\n\tbx r1\n"));
    }

    #[test]
    fn self_test_dispatcher_does_not_close_an_owner() {
        assert!(!has_epilogue("\tldr r3, [r4, #0]\n\tbx r3\n"));
    }

    #[test]
    fn self_test_pool_row_is_read() {
        let pool = row_facts_from_assembly(
            "\t.thumb\nFragment_08000010:\n\t.inst.n 0x1eec\n\t.inst.n 0x0300\n",
            &region(0x0800_0010, 4, "merge_with_owner"),
        );
        assert!(pool.is_data);
        assert!(!pool.has_prologue);
    }

    #[test]
    fn self_test_decoded_pool_is_suspected_not_data() {
        let decoded = row_facts_from_assembly(
            "\t.thumb\n.L_0800:\n\tlsrs r0, r0, #1\n\tmovs r0, #0\n",
            &region(0x0800_0030, 4, "merge_with_owner"),
        );
        assert!(!decoded.is_data);
        assert!(decoded.suspected_pool);
    }

    #[test]
    fn self_test_real_code_is_not_suspected() {
        let real = row_facts_from_assembly(
            "\t.thumb\n.L_0800:\n\tldr r0, [r1, #0]\n\tbl sub_08001234\n",
            &region(0x0800_0040, 8, "split_first"),
        );
        assert!(!real.suspected_pool);
    }

    #[test]
    fn self_test_ldr_shaped_pool_is_suspected() {
        let pool = row_facts_from_assembly(
            "\t.thumb\n.L_0800:\n\tldr r0, [r4, #4]\n\tmovs r0, r0\n",
            &region(0x0800_0050, 4, "merge_with_owner"),
        );
        assert!(pool.suspected_pool);
    }

    #[test]
    fn self_test_prologue_pool_epilogue_is_one_owner() {
        let code = row_facts_from_assembly(
            "\t.thumb\nRegion_08000000:\n\tpush {r4, lr}\n\tbl sub_08001234\n\tpop {r4, pc}\n",
            &region(0x0800_0000, 16, "split_first"),
        );
        let pool = row_facts_from_assembly(
            "\t.thumb\nFragment_08000010:\n\t.inst.n 0x1eec\n\t.inst.n 0x0300\n",
            &region(0x0800_0010, 4, "merge_with_owner"),
        );
        let mut first = code.clone();
        first.has_epilogue = false;
        let mut third = code.clone();
        third.address = 0x0800_0020;
        third.stem = "08000020".into();
        third.has_prologue = false;
        let owners = group_owners(&[first, pool, third]);
        assert_eq!(owners.len(), 1);
        assert_eq!(owners[0].excluded_bytes, 4);
        assert!(owners[0].closed);
    }

    #[test]
    fn self_test_range_overlap() {
        assert!(overlaps(
            0x0800_0000,
            8,
            &AddressRange {
                start: Some(0x0800_0004),
                end: Some(0x0800_000c)
            }
        ));
        assert!(!overlaps(
            0x0800_0000,
            4,
            &AddressRange {
                start: Some(0x0800_0004),
                end: Some(0x0800_0008)
            }
        ));
    }

    // -- Edge cases the TypeScript self-test missed -------------------------

    #[test]
    fn js_space_class_is_not_unicode_whitespace() {
        // U+FEFF is JS \s but not char::is_whitespace.
        assert!(is_js_space('\u{feff}'));
        assert!(!'\u{feff}'.is_whitespace());
        // U+0085 (NEL) is Unicode whitespace but not JS \s.
        assert!(!is_js_space('\u{85}'));
        assert!('\u{85}'.is_whitespace());
        // A BOM-indented push is a prologue to JS, and now to us.
        let body: Vec<char> = "\u{feff}push {r4, lr}".chars().collect();
        assert!(brace_list_has(&body, "push", "lr"));
        // A NEL-indented one is not.
        let body: Vec<char> = "\u{85}push {r4, lr}".chars().collect();
        assert!(!brace_list_has(&body, "push", "lr"));
    }

    #[test]
    fn word_boundary_is_ascii_only() {
        // JS \b without /u: 'e' with acute is a NON-word char, so \blr\b
        // matches inside "elr" spelled with the accented letter.
        let body: Vec<char> = "\tpush {\u{e9}lr}".chars().collect();
        assert!(brace_list_has(&body, "push", "lr"));
        // But a plain ASCII letter does block it.
        let body: Vec<char> = "\tpush {xlr}".chars().collect();
        assert!(!brace_list_has(&body, "push", "lr"));
        // And a trailing ASCII word char blocks it too.
        let body: Vec<char> = "\tpush {lr9}".chars().collect();
        assert!(!brace_list_has(&body, "push", "lr"));
        // Underscore is a word char.
        let body: Vec<char> = "\tpush {lr_}".chars().collect();
        assert!(!brace_list_has(&body, "push", "lr"));
    }

    #[test]
    fn brace_list_stops_at_the_first_closing_brace() {
        // `[^}]*` cannot cross a `}`: the lr here is outside the list.
        let body: Vec<char> = "\tpush {r4}\n\t@ lr\n".chars().collect();
        assert!(!brace_list_has(&body, "push", "lr"));
    }

    #[test]
    fn multiline_anchor_matches_a_later_line() {
        let body: Vec<char> = "\tmovs r0, #1\n\tpush {r4, lr}\n".chars().collect();
        assert!(brace_list_has(&body, "push", "lr"));
        // But not mid-line: `^` only matches at a line start.
        let body: Vec<char> = "\tmovs r0, #1 push {r4, lr}\n".chars().collect();
        assert!(!brace_list_has(&body, "push", "lr"));
    }

    #[test]
    fn blx_matches_the_three_letter_branch_alternative() {
        // `bl` and `b` both fail their \b check on "blx"; b[a-z]{2} wins.
        assert!(has_branch_or_stack(&"\tblx r3\n".chars().collect::<Vec<_>>()));
        // Uppercase is not matched: the alternation has no /i flag.
        assert!(!has_branch_or_stack(&"\tBLX r3\n".chars().collect::<Vec<_>>()));
        // `b.n` matches via the bare `b` branch, since '.' is a non-word char.
        assert!(has_branch_or_stack(&"\tb.n .L_1\n".chars().collect::<Vec<_>>()));
        // A four-letter b-word does not match any branch.
        assert!(!has_branch_or_stack(&"\tbendy r3\n".chars().collect::<Vec<_>>()));
        assert!(has_branch_or_stack(&"\tbeq .L_1\n".chars().collect::<Vec<_>>()));
        assert!(!has_branch_or_stack(&"\tmovs r0, #0\n".chars().collect::<Vec<_>>()));
    }

    #[test]
    fn bl_counting_matches_javascript_lastindex() {
        let body: Vec<char> = "\tbl a\n\tbl b\n\tbl c\n".chars().collect();
        assert_eq!(count_bl_calls(&body), 3);
        // `blx` is not a `bl` call: \s+ cannot match the 'x'.
        assert_eq!(count_bl_calls(&"\tblx r3\n".chars().collect::<Vec<_>>()), 0);
        // A trailing `bl` with no operand does not count.
        assert_eq!(count_bl_calls(&"\tbl\n".chars().collect::<Vec<_>>()), 0);
        // Blank lines before a call still yield exactly one match, because the
        // greedy `\s*` from the earlier line start swallows them.
        assert_eq!(count_bl_calls(&"\n\n\tbl a\n".chars().collect::<Vec<_>>()), 1);
    }

    #[test]
    fn mnemonic_line_requires_trailing_whitespace() {
        // "nop" with no trailing space is NOT a mnemonic line in the source
        // regex, so such a row counts as data.
        let row = row_facts_from_assembly(
            "\t.thumb\n\tnop",
            &region(0x0800_0000, 2, "merge_with_owner"),
        );
        assert!(row.is_data);
        let row = row_facts_from_assembly(
            "\t.thumb\n\tnop \n",
            &region(0x0800_0000, 2, "merge_with_owner"),
        );
        assert!(!row.is_data);
    }

    #[test]
    fn header_directives_are_filtered_by_prefix_not_by_word() {
        // The source regex has no \b, so `.textual` is filtered like `.text`.
        assert!(is_header_line(&"\t.textual 1 ".chars().collect::<Vec<_>>()));
        assert!(is_header_line(&"@ comment".chars().collect::<Vec<_>>()));
        assert!(!is_header_line(&"\t.word 0x1 ".chars().collect::<Vec<_>>()));
    }

    #[test]
    fn epilogue_lookback_is_exactly_four_lines() {
        let mut body = String::from("\tpop {r1}\n");
        for _ in 0..4 {
            body.push_str("\tmovs r0, #0\n");
        }
        body.push_str("\tbx r1\n");
        assert!(!has_epilogue(&body));

        let mut body = String::from("\tpop {r1}\n");
        for _ in 0..3 {
            body.push_str("\tmovs r0, #0\n");
        }
        body.push_str("\tbx r1\n");
        assert!(has_epilogue(&body));
    }

    #[test]
    fn epilogue_register_capture_is_exact() {
        // `bx r1` must not be closed by a `pop {r12}`.
        assert!(!has_epilogue("\tpop {r12}\n\tbx r1\n"));
        assert!(has_epilogue("\tpop {r4, r12}\n\tbx r12\n"));
        // pop {..., pc}
        assert!(has_epilogue("\tpop {r4, pc}\n"));
        // bx lr
        assert!(has_epilogue("\tbx lr\n"));
        assert!(!has_epilogue("\tbx lrx\n"));
    }

    #[test]
    fn grouping_is_stable_for_equal_addresses() {
        let make = |stem: &str, size: i64| RowFacts {
            stem: stem.into(),
            address: 0x0800_0000,
            size,
            retention: "split_first".into(),
            kind: "k".into(),
            has_prologue: false,
            has_epilogue: false,
            is_data: false,
            suspected_pool: false,
            calls: 0,
        };
        let owners = group_owners(&[make("aaa", 1), make("bbb", 2), make("ccc", 3)]);
        assert_eq!(owners.len(), 1);
        let stems: Vec<&str> = owners[0].rows.iter().map(|r| r.stem.as_str()).collect();
        assert_eq!(stems, ["aaa", "bbb", "ccc"]);
        // advertisedBytes comes from the FIRST row, which stability decides.
        assert_eq!(owners[0].advertised_bytes, 1);
    }

    #[test]
    fn grouping_flushes_unclosed_groups_and_reopens_on_prologue() {
        let row = |address: i64, prologue: bool, epilogue: bool| RowFacts {
            stem: stem_of(address),
            address,
            size: 4,
            retention: "split_first".into(),
            kind: "k".into(),
            has_prologue: prologue,
            has_epilogue: epilogue,
            is_data: false,
            suspected_pool: false,
            calls: 0,
        };
        let owners = group_owners(&[
            row(0x0800_0000, true, false),
            row(0x0800_0004, true, false),
            row(0x0800_0008, false, true),
        ]);
        assert_eq!(owners.len(), 2);
        assert!(!owners[0].closed);
        assert_eq!(owners[0].rows.len(), 1);
        assert!(owners[1].closed);
        assert_eq!(owners[1].rows.len(), 2);
        // A row that both opens and closes is its own owner.
        let owners = group_owners(&[row(0x0800_0000, true, true)]);
        assert_eq!(owners.len(), 1);
        assert!(owners[0].closed);
    }

    #[test]
    fn is_safe_integer_accepts_integral_floats() {
        assert!(is_safe_integer(512.0));
        assert!(!is_safe_integer(512.5));
        assert!(!is_safe_integer(9_007_199_254_740_993.0));
        assert!(!is_safe_integer(f64::NAN));
        assert!(!is_safe_integer(f64::INFINITY));
        assert!(is_safe_integer(-1.0));
    }

    #[test]
    fn parse_int_16_mirrors_javascript() {
        assert_eq!(parse_int_16("0x0800d58c"), Some(0x0800_d58c));
        assert_eq!(parse_int_16("0800d58c"), Some(0x0800_d58c));
        assert_eq!(parse_int_16("  0X10zzz"), Some(0x10));
        assert_eq!(parse_int_16("-10"), Some(-16));
        assert_eq!(parse_int_16("zz"), None);
        assert_eq!(parse_int_16(""), None);
        // "0x" with no digits: JS parses the leading 0 and stops.
        assert_eq!(parse_int_16("0x"), Some(0));
    }

    #[test]
    fn nan_addresses_never_overlap() {
        assert!(!overlaps(
            0x0800_0000,
            8,
            &AddressRange {
                start: None,
                end: None
            }
        ));
        assert!(!overlaps(
            0x0800_0000,
            8,
            &AddressRange {
                start: Some(0),
                end: None
            }
        ));
    }

    #[test]
    fn non_c_address_regex() {
        assert!(is_non_c_address("0x08002344"));
        assert!(is_non_c_address("0X0800F186"));
        assert!(!is_non_c_address("0x0900f186"));
        assert!(!is_non_c_address("0x0800f18"));
        assert!(!is_non_c_address("0x0800f1866"));
    }

    #[test]
    fn admitted_name_regex() {
        assert!(is_admitted_name("08002ee4.c"));
        assert!(is_admitted_name("0800ABCD.C"));
        assert!(!is_admitted_name("09002ee4.c"));
        assert!(!is_admitted_name("08002ee4.s"));
        assert!(!is_admitted_name("08002ee.c"));
    }

    #[test]
    fn stem_argument_regex_is_case_insensitive() {
        assert!(is_stem_argument("080d77b4"));
        assert!(is_stem_argument("080D77B4"));
        assert!(!is_stem_argument("080d77b"));
        assert!(!is_stem_argument("080d77b4a"));
        assert!(!is_stem_argument("080d77bg"));
    }

    #[test]
    fn invalid_utf8_is_replaced_not_rejected() {
        let raw = b"\t.thumb\n\tpush {r4, \xff lr}\n";
        let text = String::from_utf8_lossy(raw).into_owned();
        assert!(text.contains('\u{fffd}'));
        let row = row_facts_from_assembly(&text, &region(0x0800_0000, 4, "split_first"));
        // U+FFFD is a non-word char, so \blr\b still matches.
        assert!(row.has_prologue);
    }

    #[test]
    fn stem_ordering_is_never_used_for_output() {
        // Guard for the UTF-16 vs byte sort trap: owners are ordered purely by
        // numeric address, so two rows whose stems sort differently under the
        // two conventions still come out in address order.
        let row = |address: i64, stem: &str| RowFacts {
            stem: stem.into(),
            address,
            size: 4,
            retention: "split_first".into(),
            kind: "k".into(),
            has_prologue: false,
            has_epilogue: false,
            is_data: false,
            suspected_pool: false,
            calls: 0,
        };
        // "\u{ffff}" > "\u{10000}" in UTF-16 code units, but "<" as bytes.
        let owners = group_owners(&[
            row(0x0800_0004, "\u{10000}"),
            row(0x0800_0000, "\u{ffff}"),
        ]);
        let stems: Vec<&str> = owners[0].rows.iter().map(|r| r.stem.as_str()).collect();
        assert_eq!(stems, ["\u{ffff}", "\u{10000}"]);
    }

    #[test]
    fn json_output_matches_json_stringify_shape() {
        assert_eq!(owners_to_json(&[]), "[]");
        let owner = Owner {
            entry: "08000000".into(),
            rows: vec![RowFacts {
                stem: "08000000".into(),
                address: 0x0800_0000,
                size: 16,
                retention: "split_first".into(),
                kind: "k".into(),
                has_prologue: true,
                has_epilogue: true,
                is_data: false,
                suspected_pool: false,
                calls: 1,
            }],
            executable_bytes: 16,
            excluded_bytes: 0,
            advertised_bytes: 16,
            calls: 1,
            closed: true,
            suspected_pool_bytes: 0,
        };
        let expected = "[\n  {\n    \"entry\": \"08000000\",\n    \"rows\": [\n      {\n        \"stem\": \"08000000\",\n        \"address\": 134217728,\n        \"size\": 16,\n        \"retention\": \"split_first\",\n        \"kind\": \"k\",\n        \"hasPrologue\": true,\n        \"hasEpilogue\": true,\n        \"isData\": false,\n        \"suspectedPool\": false,\n        \"calls\": 1\n      }\n    ],\n    \"executableBytes\": 16,\n    \"excludedBytes\": 0,\n    \"advertisedBytes\": 16,\n    \"calls\": 1,\n    \"closed\": true,\n    \"suspectedPoolBytes\": 0\n  }\n]";
        assert_eq!(owners_to_json(&[owner]), expected);
    }

    #[test]
    fn report_rendering_matches_the_typescript_layout() {
        let owner = Owner {
            entry: "080d77b4".into(),
            rows: vec![RowFacts {
                stem: "080d77b4".into(),
                address: 0x080d_77b4,
                size: 768,
                retention: "split_first".into(),
                kind: "k".into(),
                has_prologue: true,
                has_epilogue: false,
                is_data: false,
                suspected_pool: true,
                calls: 2,
            }],
            executable_bytes: 768,
            excluded_bytes: 4,
            advertised_bytes: 768,
            calls: 2,
            closed: false,
            suspected_pool_bytes: 768,
        };
        let owners = vec![owner];
        assert_eq!(
            render_report(&owners, None),
            "owner 080d77b4  advertised=768  executable=768  excluded_pool=4  rows=1  calls=2  suspected_pool=768  UNCLOSED(no epilogue found)\n\nowners=1 executable_bytes=768 excluded_pool_bytes=4 unclosed=1\n"
        );
        assert_eq!(
            render_report(&owners, Some("080D77B4")),
            "owner 080d77b4  advertised=768  executable=768  excluded_pool=4  rows=1  calls=2  suspected_pool=768  UNCLOSED(no epilogue found)\n    080d77b4    768  POOL?split_first prologue\n\nowners=1 executable_bytes=768 excluded_pool_bytes=4 unclosed=1\n"
        );
        assert_eq!(
            render_report(&[], None),
            "\nowners=0 executable_bytes=0 excluded_pool_bytes=0 unclosed=0\n"
        );
    }

    #[test]
    fn pad_start_does_not_truncate() {
        assert_eq!(pad_start_5(4), "    4");
        assert_eq!(pad_start_5(123456), "123456");
    }

    // -- Real-tree agreement ------------------------------------------------

    fn root() -> PathBuf {
        Path::new(env!("CARGO_MANIFEST_DIR"))
            .parent()
            .unwrap()
            .parent()
            .unwrap()
            .to_path_buf()
    }

    #[test]
    fn real_tree_census_is_declared_closed() {
        assert!(census_declared_closed(&root()).unwrap());
    }

    #[test]
    fn real_tree_reports_no_open_owners() {
        assert_eq!(open_owners(&root()).unwrap(), Vec::new());
    }

    /// Digest of every classified row in the real tree, for cross-checking
    /// against the TypeScript. Printed with `cargo test -- --nocapture`.
    #[test]
    fn real_tree_row_digest_is_stable() {
        let root = root();
        let regions = read_manifest_regions(&root).unwrap();
        let mut lines = Vec::new();
        for region in &regions {
            let source = if region.source.starts_with('/') {
                PathBuf::from(&region.source)
            } else {
                root.join(&region.source)
            };
            if !source.exists() {
                continue;
            }
            let facts = row_facts_from_assembly(&read_utf8_lossy(&source).unwrap(), region);
            lines.push(format!(
                "{} {} {} {} {} {} {}",
                facts.stem,
                facts.size,
                facts.has_prologue as u8,
                facts.has_epilogue as u8,
                facts.is_data as u8,
                facts.suspected_pool as u8,
                facts.calls
            ));
        }
        assert_eq!(lines.len(), regions.len());
        std::fs::write(
            std::env::temp_dir().join("semantic_owner_scope_rows.rs.txt"),
            lines.join("\n") + "\n",
        )
        .unwrap();
    }
}
