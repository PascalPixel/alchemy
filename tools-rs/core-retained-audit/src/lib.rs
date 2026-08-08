// Main-image retained-assembly guard — Rust port of
// `tools/check/core_retained_audit.ts`.
//
// WHY this file exists: the orange part of the main-image map is not a
// hand-maintained total. It is exactly the audited executable inventory less
// byte-identical C and reviewed semantic C. This guard recomputes that
// complement and makes the two current full-build manifests account for every
// one of its bytes. The port exists because the audit is a gate that runs on
// every full build and the TypeScript spends most of its time re-zeroing an
// 8 MiB mask inside a 1456-iteration loop.
//
// Every deliberate divergence from the TypeScript is marked `PORT NOTE`.

pub mod js;
pub mod json;
pub mod load;

use js::{is_js_word, is_safe_integer, js_number_from_str, locale_compare, relative_index};
use json::Json;
use std::collections::BTreeSet;
use std::path::{Path, PathBuf};

pub const ROM_BASE: i64 = 0x0800_0000;
pub const ROM_SIZE: usize = 0x80_0000;

pub type Failure = String;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct Span {
    pub start: i64,
    pub end: i64,
}

#[derive(Clone, Debug)]
pub struct InventoryInterval {
    pub span: Span,
    pub kind: String,
    pub evidence: String,
}

#[derive(Clone, Debug)]
pub struct AsmRegion {
    pub span: Span,
    pub source: String,
    pub kind: String,
    pub retention: String,
    pub confidence: String,
    pub evidence: String,
}

#[derive(Clone, Debug)]
pub struct ClaimedRegion {
    pub span: Span,
    pub source: String,
}

#[derive(Clone, Debug)]
pub struct NonCRange {
    pub span: Span,
    pub kind: String,
    pub evidence: String,
}

#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub struct InputPaths {
    pub inventory: String,
    pub semantic: String,
    pub asm_manifest: String,
    pub claimed_manifest: String,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct KindConfidenceRow {
    pub kind: String,
    pub confidence: String,
    pub regions: i64,
    pub bytes: i64,
}

#[derive(Clone, Debug)]
pub struct CoreRetainedAudit {
    pub status_ok: bool,
    pub inputs: InputPaths,
    pub executable_bytes: i64,
    pub exact_c_bytes: i64,
    pub semantic_c_bytes: i64,
    pub retained_bytes: i64,
    pub retained_by_kind_confidence: Vec<KindConfidenceRow>,
    pub failures: Vec<Failure>,
}

impl CoreRetainedAudit {
    pub fn status(&self) -> &'static str {
        if self.status_ok {
            "ok"
        } else {
            "failed"
        }
    }

    /// The `--json` document, in the TypeScript object literal's key order.
    pub fn to_json(&self) -> Json {
        Json::Obj(vec![
            ("format".into(), Json::Num(1.0)),
            ("kind".into(), Json::Str("core-retained-complement-audit".into())),
            ("status".into(), Json::Str(self.status().into())),
            (
                "inputs".into(),
                Json::Obj(vec![
                    ("inventory".into(), Json::Str(self.inputs.inventory.clone())),
                    ("semantic".into(), Json::Str(self.inputs.semantic.clone())),
                    ("asm_manifest".into(), Json::Str(self.inputs.asm_manifest.clone())),
                    ("claimed_manifest".into(), Json::Str(self.inputs.claimed_manifest.clone())),
                ]),
            ),
            (
                "bytes".into(),
                Json::Obj(vec![
                    ("executable".into(), Json::Num(self.executable_bytes as f64)),
                    ("exact_c".into(), Json::Num(self.exact_c_bytes as f64)),
                    ("semantic_c".into(), Json::Num(self.semantic_c_bytes as f64)),
                    ("retained".into(), Json::Num(self.retained_bytes as f64)),
                ]),
            ),
            (
                "retained_by_kind_confidence".into(),
                Json::Arr(
                    self.retained_by_kind_confidence
                        .iter()
                        .map(|row| {
                            Json::Obj(vec![
                                ("kind".into(), Json::Str(row.kind.clone())),
                                ("confidence".into(), Json::Str(row.confidence.clone())),
                                ("regions".into(), Json::Num(row.regions as f64)),
                                ("bytes".into(), Json::Num(row.bytes as f64)),
                            ])
                        })
                        .collect(),
                ),
            ),
            (
                "failures".into(),
                Json::Arr(self.failures.iter().map(|text| Json::Str(text.clone())).collect()),
            ),
        ])
    }
}

// ---------------------------------------------------------------------------
// Primitive helpers, one per TypeScript function
// ---------------------------------------------------------------------------

/// `number(value, label)`.
///
/// PORT NOTE: a JSON string is coerced with `Number()`, not `f64::from_str`,
/// because 229 addresses in the real tree are the hex strings `"0x08002344"`.
fn number(value: Option<&Json>, label: &str) -> Result<i64, String> {
    let parsed = match value {
        Some(Json::Str(text)) => js_number_from_str(text),
        Some(Json::Num(number)) => *number,
        // `undefined`, `null`, booleans, arrays and objects all fall through
        // to the `Number.isSafeInteger` rejection in the original.
        _ => f64::NAN,
    };
    if !is_safe_integer(parsed) || parsed < 0.0 {
        return Err(format!("{label} must be a non-negative integer"));
    }
    Ok(parsed as i64)
}

/// `span(value, label)`: `start ?? address`, then `end` or `start + size`.
pub fn span(value: &Json, label: &str) -> Result<Span, String> {
    // `??` is nullish coalescing: an explicit `null` start also falls through
    // to `address`, which a plain "is the key present" check would not.
    let start_source = match value.get("start") {
        None | Some(Json::Null) => value.get("address"),
        present => present,
    };
    let start = number(start_source, &format!("{label}.start"))?;
    let end = match value.get("end") {
        None => start + number(value.get("size"), &format!("{label}.size"))?,
        present => number(present, &format!("{label}.end"))?,
    };
    if end <= start {
        return Err(format!("{label} is empty or reversed"));
    }
    Ok(Span { start, end })
}

/// `normalized(items, label)`: sort, bounds-check, merge touching/overlapping.
pub fn normalized(items: &[Span], label: &str) -> Result<Vec<Span>, String> {
    let mut sorted = items.to_vec();
    // JS `sort` is stable and the comparator is a total order on (start, end),
    // so `sort_by` (also stable) is exact.
    sorted.sort_by(|a, b| a.start.cmp(&b.start).then(a.end.cmp(&b.end)));
    let mut result: Vec<Span> = Vec::new();
    for item in sorted {
        if item.start < ROM_BASE || item.end > ROM_BASE + ROM_SIZE as i64 {
            return Err(format!("{label} lies outside the gs1-en ROM"));
        }
        match result.last_mut() {
            // Touching spans MERGE: the guard is `item.start > previous.end`.
            Some(previous) if item.start <= previous.end => {
                previous.end = previous.end.max(item.end);
            }
            _ => result.push(item),
        }
    }
    Ok(result)
}

pub fn bytes(items: &[Span]) -> Result<i64, String> {
    Ok(normalized(items, "span")?.iter().map(|item| item.end - item.start).sum())
}

/// `mark(mask, spans, bit)` via `TypedArray#fill`.
///
/// PORT NOTE: `fill` uses relative indices that clamp (and wrap on negatives)
/// rather than panicking. `mark` is called on manifest spans that were never
/// bounds-checked by `normalized`, so this behaviour is reachable and is
/// reproduced exactly rather than tightened.
pub fn mark(mask: &mut [u8], spans: &[Span], bit: u8) {
    let length = mask.len();
    for item in spans {
        let start = relative_index(item.start - ROM_BASE, length);
        let end = relative_index(item.end - ROM_BASE, length);
        if start < end {
            mask[start..end].fill(bit);
        }
    }
}

/// `allCovered(mask, spans)`: no zero byte anywhere under any span.
/// An out-of-range or empty subarray is vacuously covered, as in JS.
pub fn all_covered(mask: &[u8], spans: &[Span]) -> bool {
    let length = mask.len();
    spans.iter().all(|item| {
        let start = relative_index(item.start - ROM_BASE, length);
        let end = relative_index(item.end - ROM_BASE, length).max(start);
        !mask[start..end].contains(&0)
    })
}

/// `intersections(mask, selected)`: maximal runs equal to `selected`.
pub fn intersections(mask: &[u8], selected: u8) -> Vec<Span> {
    let mut result = Vec::new();
    let mut start: Option<usize> = None;
    for offset in 0..=mask.len() {
        let matches = offset < mask.len() && mask[offset] == selected;
        match (matches, start) {
            (true, None) => start = Some(offset),
            (false, Some(begin)) => {
                result.push(Span {
                    start: ROM_BASE + begin as i64,
                    end: ROM_BASE + offset as i64,
                });
                start = None;
            }
            _ => {}
        }
    }
    result
}

pub fn without(left: &[Span], right: &[Span]) -> Vec<Span> {
    let mut mask = vec![0u8; ROM_SIZE];
    mark(&mut mask, left, 1);
    mark(&mut mask, right, 2);
    for value in mask.iter_mut() {
        if *value == 2 {
            *value = 0;
        }
    }
    intersections(&mask, 1)
}

pub fn missing_coverage(mask: &[u8], spans: &[Span]) -> Vec<Span> {
    let mut requested = vec![0u8; ROM_SIZE];
    mark(&mut requested, spans, 1);
    for (offset, value) in requested.iter_mut().enumerate() {
        if *value != 0 && mask[offset] != 0 {
            *value = 0;
        }
    }
    intersections(&requested, 1)
}

pub fn overlap(a: Span, b: Span) -> Option<Span> {
    let start = a.start.max(b.start);
    let end = a.end.min(b.end);
    if start < end {
        Some(Span { start, end })
    } else {
        None
    }
}

/// `hex(value)` = `0x` + `toString(16).padStart(8, "0")`.
pub fn hex(value: i64) -> String {
    format!("0x{}", js::to_string_16_pad_start(value, 8))
}

// ---------------------------------------------------------------------------
// The audit itself
// ---------------------------------------------------------------------------

pub struct AuditInput {
    pub inventory: Vec<InventoryInterval>,
    pub exact: Vec<Span>,
    pub semantic: Vec<Span>,
    pub non_c: Vec<NonCRange>,
    pub asm: Vec<AsmRegion>,
    pub claimed: Vec<ClaimedRegion>,
    pub noncanonical_semantic_claims: Vec<ClaimedRegion>,
    pub paths: Option<InputPaths>,
}

pub fn audit_core_retained(input: &AuditInput) -> Result<CoreRetainedAudit, String> {
    let mut failures: Vec<Failure> = Vec::new();
    let inventory_spans: Vec<Span> = input.inventory.iter().map(|item| item.span).collect();
    let executable = normalized(&inventory_spans, "executable inventory")?;
    let exact = normalized(&input.exact, "canonical exact-C spans")?;
    let semantic = normalized(&input.semantic, "semantic spans")?;
    let semantic_only = without(&semantic, &exact);

    let claim_spans: Vec<Span> =
        input.noncanonical_semantic_claims.iter().map(|item| item.span).collect();
    let mut semantic_claim_mask = vec![0u8; ROM_SIZE];
    mark(&mut semantic_claim_mask, &claim_spans, 1);

    let mut mask = vec![0u8; ROM_SIZE];
    mark(&mut mask, &executable, 1);

    // Semantic and exact C must be subsets of the audited denominator. This is
    // deliberately checked before they are subtracted: a registry typo cannot
    // make a byte disappear from the audit.
    for (label, spans) in [("exact C", &exact), ("semantic C", &semantic)] {
        if !all_covered(&mask, spans) {
            failures.push(format!(
                "{label} contains bytes outside the tracked executable inventory"
            ));
        }
    }

    let mut owned = mask.clone();
    mark(&mut owned, &exact, 2);
    mark(&mut owned, &semantic, 3);
    for offset in 0..mask.len() {
        if mask[offset] == 1 && owned[offset] != 1 {
            owned[offset] = 0;
        }
    }
    let retained = intersections(&owned, 1);

    let mut claimed_mask = vec![0u8; ROM_SIZE];
    let mut asm_mask = vec![0u8; ROM_SIZE];
    let mut non_c_mask = vec![0u8; ROM_SIZE];
    mark(&mut claimed_mask, &input.claimed.iter().map(|r| r.span).collect::<Vec<_>>(), 1);
    mark(&mut asm_mask, &input.asm.iter().map(|r| r.span).collect::<Vec<_>>(), 1);
    mark(&mut non_c_mask, &input.non_c.iter().map(|r| r.span).collect::<Vec<_>>(), 1);

    let semantic_or_claim_mask: Vec<u8> = asm_mask
        .iter()
        .zip(semantic_claim_mask.iter())
        .map(|(asm, claim)| if *claim != 0 { 1 } else { *asm })
        .collect();

    for (label, coverage, spans) in [
        (
            "canonical exact-C span absent from the current full claimed manifest",
            &claimed_mask,
            &exact,
        ),
        (
            "canonical semantic-C span absent from the current full asm/approved noncanonical-claim manifest",
            &semantic_or_claim_mask,
            &semantic_only,
        ),
        ("retained complement absent from the current full asm manifest", &asm_mask, &retained),
    ] {
        let missing = missing_coverage(coverage, spans);
        if !missing.is_empty() {
            failures.extend(missing.iter().take(20).map(|part| {
                format!("{label} at {}..{}", hex(part.start), hex(part.end))
            }));
        }
        if missing.len() > 20 {
            failures.push(format!("{label}: {} further spans omitted", missing.len() - 20));
        }
    }

    // A C fragment in executable space needs a matching canonical exact-C
    // inventory span; compilation alone is not admission to the numerator.
    //
    // PORT NOTE: the TypeScript rebuilds `exactMask(exact)` AND copies it
    // (`new Uint8Array(...)`) on every iteration of this doubly-nested loop.
    // Both are pure and neither is mutated, so the mask is hoisted. This is a
    // performance change with identical observable behaviour, and it is where
    // essentially the whole speedup comes from.
    let mut exact_mask = vec![0u8; ROM_SIZE];
    mark(&mut exact_mask, &exact, 1);
    for region in &input.claimed {
        for part in executable.iter().filter_map(|item| overlap(region.span, *item)) {
            let exact_coverage = all_covered(&exact_mask, &[part]);
            let approved_semantic_replacement = all_covered(&semantic_claim_mask, &[part])
                && input
                    .noncanonical_semantic_claims
                    .iter()
                    .any(|claim| claim.source == region.source && overlap(claim.span, part).is_some());
            if !exact_coverage && !approved_semantic_replacement {
                failures.push(format!(
                    "unsupported claimed-C fragment {} at {}..{}",
                    region.source,
                    hex(part.start),
                    hex(part.end)
                ));
            }
        }
    }

    // Insertion-ordered accumulation: the TypeScript uses a `Map`, and while
    // the rows are sorted afterwards, `Map#set` semantics still decide which
    // row a repeat key lands in. Never a HashMap.
    let mut totals: Vec<KindConfidenceRow> = Vec::new();
    for region in &input.asm {
        for part in retained.iter().filter_map(|item| overlap(region.span, *item)) {
            let permitted_directive = all_covered(&non_c_mask, &[part]);
            if (region.retention == "c_candidate" || region.kind == "compiler_output")
                && !permitted_directive
            {
                failures.push(format!(
                    "ordinary {}/{} retained code at {}..{} ({})",
                    region.retention,
                    region.kind,
                    hex(part.start),
                    hex(part.end),
                    region.source
                ));
            }
            match totals
                .iter_mut()
                .find(|row| row.kind == region.kind && row.confidence == region.confidence)
            {
                Some(row) => {
                    row.regions += 1;
                    row.bytes += part.end - part.start;
                }
                None => totals.push(KindConfidenceRow {
                    kind: region.kind.clone(),
                    confidence: region.confidence.clone(),
                    regions: 1,
                    bytes: part.end - part.start,
                }),
            }
        }
    }
    totals.sort_by(|a, b| {
        locale_compare(&a.kind, &b.kind).then_with(|| locale_compare(&a.confidence, &b.confidence))
    });

    // `[...new Set(failures)]`: order-preserving dedup.
    let mut seen = BTreeSet::new();
    let deduped: Vec<Failure> =
        failures.into_iter().filter(|failure| seen.insert(failure.clone())).collect();

    Ok(CoreRetainedAudit {
        status_ok: deduped.is_empty(),
        inputs: input.paths.clone().unwrap_or_default(),
        executable_bytes: bytes(&executable)?,
        exact_c_bytes: bytes(&exact)?,
        semantic_c_bytes: bytes(&semantic_only)?,
        retained_bytes: bytes(&retained)?,
        retained_by_kind_confidence: totals,
        failures: deduped,
    })
}

// ---------------------------------------------------------------------------
// Source scanning
// ---------------------------------------------------------------------------

fn word_boundary(chars: &[char], index: usize) -> bool {
    let before = index > 0 && is_js_word(chars[index - 1]);
    let after = index < chars.len() && is_js_word(chars[index]);
    before != after
}

fn literal_at(chars: &[char], index: usize, word: &str) -> bool {
    let word: Vec<char> = word.chars().collect();
    index + word.len() <= chars.len() && chars[index..index + word.len()] == word[..]
}

fn word_at(chars: &[char], index: usize, word: &str) -> bool {
    literal_at(chars, index, word)
        && word_boundary(chars, index)
        && word_boundary(chars, index + word.chars().count())
}

/// `canonicalC(source)` — the four rejection patterns, hand-scanned.
///
/// PORT NOTE: these replace JS regexes rather than using a regex crate.
/// `\b` and `\w` in JavaScript are ASCII-only (`js::is_js_word`), which a
/// Unicode-aware engine would not reproduce, and `\s` is JavaScript's set
/// (`js::is_js_space`), which includes U+FEFF and excludes U+0085. Both
/// `[^;\n]*` and `\s*`/`\s+` are greedy over character sets disjoint from what
/// follows them, so greedy-then-check is exact and no backtracking is needed.
pub fn canonical_c(source: &str) -> bool {
    let chars: Vec<char> = source.chars().collect();
    for index in 0..chars.len() {
        // /\bM2C_ERROR\b/
        if word_at(&chars, index, "M2C_ERROR") {
            return false;
        }
        // /\b__asm__\b/
        if word_at(&chars, index, "__asm__") {
            return false;
        }
        // /\.incbin\b/ — no boundary requirement before the dot.
        if literal_at(&chars, index, ".incbin") && word_boundary(&chars, index + 7) {
            return false;
        }
        // /\basm\s+volatile\b/
        if literal_at(&chars, index, "asm") && word_boundary(&chars, index) {
            let mut cursor = index + 3;
            let space_start = cursor;
            while cursor < chars.len() && js::is_js_space(chars[cursor]) {
                cursor += 1;
            }
            if cursor > space_start && word_at(&chars, cursor, "volatile") {
                return false;
            }
        }
        // /\bregister\b[^;\n]*\basm\s*\(/
        if word_at(&chars, index, "register") {
            let mut limit = index + 8;
            while limit < chars.len() && chars[limit] != ';' && chars[limit] != '\n' {
                limit += 1;
            }
            for candidate in index + 8..limit {
                if !(literal_at(&chars, candidate, "asm") && word_boundary(&chars, candidate)) {
                    continue;
                }
                let mut cursor = candidate + 3;
                while cursor < chars.len() && js::is_js_space(chars[cursor]) {
                    cursor += 1;
                }
                if chars.get(cursor) == Some(&'(') {
                    return false;
                }
            }
        }
    }
    true
}

/// `sourceNames(directory)`: recursive, files only.
///
/// PORT NOTE: `readdirSync` returns OS directory order; this sorts. Every
/// consumer either feeds a `Set` of addresses that is sorted afterwards or a
/// span list that goes through `normalized`, so order is not observable — but
/// sorting makes the port deterministic across filesystems. `Dirent#isDirectory`
/// does not follow symlinks, and neither does `DirEntry::file_type`, so a
/// symlinked directory is a plain entry in both.
pub fn source_names(directory: &Path) -> Vec<PathBuf> {
    let mut result = Vec::new();
    let Ok(entries) = std::fs::read_dir(directory) else {
        // `existsSync(directory) ? ... : []`
        return result;
    };
    let mut names: Vec<PathBuf> = entries.filter_map(|entry| entry.ok().map(|e| e.path())).collect();
    names.sort();
    for path in names {
        match std::fs::symlink_metadata(&path) {
            Ok(metadata) if metadata.is_dir() => result.extend(source_names(&path)),
            _ => result.push(path),
        }
    }
    result
}

/// `/\/([0-9a-f]{8})\.(c|s)$/i` applied to a path.
pub fn address_from_name(path: &Path, extension: &str) -> Option<i64> {
    let name = path.file_name()?.to_str()?;
    let stem = name.strip_suffix(extension)?;
    if stem.len() != 8 || !stem.chars().all(|c| c.is_ascii_hexdigit()) {
        return None;
    }
    // The regex requires a `/` before the digits, so a bare 8-hex filename at
    // the filesystem root would not match; every caller passes a path under a
    // directory, so the distinction is unreachable.
    i64::from_str_radix(stem, 16).ok()
}

/// PORT NOTE: `readFileSync(path, "utf8")` replaces invalid UTF-8 with U+FFFD
/// rather than failing, so this is lossy on purpose.
pub fn read_text(path: &Path) -> Result<String, String> {
    std::fs::read(path)
        .map(|bytes| String::from_utf8_lossy(&bytes).into_owned())
        .map_err(|error| format!("{}: {error}", path.display()))
}

pub fn read_json(path: &Path) -> Result<Json, String> {
    if !path.exists() {
        return Err(format!("missing required input: {}", path.display()));
    }
    json::parse(&read_text(path)?)
}

#[cfg(test)]
mod tests;
