// Reading the four tracked inputs and the exact/ + semantic/ source trees into
// the shapes `audit_core_retained` consumes.
//
// WHY it is a separate module: the audit function is injectable in the
// TypeScript specifically so the self-test can cover byte reconciliation
// without touching the filesystem. Keeping the loaders apart preserves that
// split, and it is where the one known defect lives (see BUG NOTE below).

use crate::json::Json;
use crate::{
    address_from_name, canonical_c, intersections, mark, normalized, overlap, read_json, read_text,
    source_names, span, AsmRegion, ClaimedRegion, InventoryInterval, NonCRange, Span, ROM_BASE,
    ROM_SIZE,
};
use std::path::Path;

pub fn main_inventory(path: &Path) -> Result<Vec<InventoryInterval>, String> {
    let document = read_json(path)?;
    let intervals = document.get("main").and_then(|main| main.get("intervals"));
    let ok = document.get("format") == Some(&Json::Num(1.0))
        && document.get("target").and_then(Json::as_str) == Some("gs1-en")
        && intervals.and_then(Json::as_array).is_some();
    if !ok {
        return Err("executable inventory is not the gs1-en main-image executable inventory".into());
    }
    intervals
        .and_then(Json::as_array)
        .unwrap()
        .iter()
        .enumerate()
        .map(|(index, item)| {
            let result = span(item, &format!("inventory.main.intervals[{index}]"))?;
            let kind = item.get("kind").and_then(Json::as_str);
            let evidence = item.get("evidence").and_then(Json::as_str);
            match (kind, evidence) {
                (Some(kind), Some(evidence)) if !evidence.trim_matches(crate::js::is_js_space).is_empty() => {
                    Ok(InventoryInterval {
                        span: result,
                        kind: kind.to_string(),
                        evidence: evidence.to_string(),
                    })
                }
                _ => Err(format!("inventory.main.intervals[{index}] lacks kind/evidence")),
            }
        })
        .collect()
}

fn nonempty_string(item: &Json, key: &str) -> Option<String> {
    let text = item.get(key).and_then(Json::as_str)?;
    // `!item[key].trim()` — JS `trim` uses the JS whitespace set.
    if text.trim_matches(crate::js::is_js_space).is_empty() {
        return None;
    }
    Some(text.to_string())
}

pub struct Manifests {
    pub asm: Vec<AsmRegion>,
    pub claimed: Vec<ClaimedRegion>,
}

pub fn manifests(asm_path: &Path, claimed_path: &Path) -> Result<Manifests, String> {
    let asm_document = read_json(asm_path)?;
    let claimed_document = read_json(claimed_path)?;
    let (Some(asm_regions), Some(claimed_regions)) = (
        asm_document.get("regions").and_then(Json::as_array),
        claimed_document.get("regions").and_then(Json::as_array),
    ) else {
        return Err("full manifests lack regions".into());
    };
    let mut asm = Vec::with_capacity(asm_regions.len());
    for (index, item) in asm_regions.iter().enumerate() {
        let result = span(item, &format!("asm.regions[{index}]"))?;
        let field = |key: &str| {
            nonempty_string(item, key).ok_or_else(|| format!("asm.regions[{index}] lacks {key}"))
        };
        asm.push(AsmRegion {
            span: result,
            source: field("source")?,
            kind: field("kind")?,
            retention: field("retention")?,
            confidence: field("confidence")?,
            evidence: field("evidence")?,
        });
    }
    let mut claimed = Vec::with_capacity(claimed_regions.len());
    for (index, item) in claimed_regions.iter().enumerate() {
        let result = span(item, &format!("claimed.regions[{index}]"))?;
        // Since the exact/semantic tree consolidation, main-image exact C lives
        // in exact/<8hex>.c; src/ remains accepted for pre-consolidation
        // manifests.
        let source = item.get("source").and_then(Json::as_str);
        let accepted = source.is_some_and(|text| {
            text.starts_with("src/") || is_exact_c_name(text)
        });
        if !accepted {
            return Err(format!("claimed.regions[{index}] is not a claimed C source"));
        }
        claimed.push(ClaimedRegion { span: result, source: source.unwrap().to_string() });
    }
    Ok(Manifests { asm, claimed })
}

/// `/^exact\/[0-9a-f]{8}\.c$/i`.
///
/// PORT NOTE: anchored at both ends, and JS `$` does NOT match before a
/// trailing newline (no `m` flag), so a source name ending in "\n" is rejected
/// here exactly as it is there.
fn is_exact_c_name(text: &str) -> bool {
    let Some(rest) = text.strip_prefix("exact/") else { return false };
    let Some(stem) = rest.strip_suffix(".c").or_else(|| rest.strip_suffix(".C")) else {
        return false;
    };
    stem.len() == 8 && stem.chars().all(|c| c.is_ascii_hexdigit())
}

/// `boundaries()`: every exact/ and asm/ source address plus the registered
/// alignment addresses, sorted ascending.
pub fn boundaries(root: &Path) -> Result<Vec<i64>, String> {
    let mut addresses: std::collections::BTreeSet<i64> = std::collections::BTreeSet::new();
    for path in source_names(&root.join("exact")) {
        if let Some(address) = address_from_name(&path, ".c") {
            addresses.insert(address);
        }
    }
    for path in source_names(&root.join("asm")) {
        if let Some(address) = address_from_name(&path, ".s") {
            addresses.insert(address);
        }
    }
    let alignment = read_json(&root.join("asm/alignment.json"))?;
    // `alignment.addresses ?? []`, each coerced with bare `Number()` — no
    // safe-integer guard, and the real file stores them as "0x08002efa".
    if let Some(list) = alignment.get("addresses").and_then(Json::as_array) {
        for entry in list {
            let value = match entry {
                Json::Str(text) => crate::js::js_number_from_str(text),
                Json::Num(number) => *number,
                Json::Null => 0.0,
                Json::Bool(flag) => {
                    if *flag {
                        1.0
                    } else {
                        0.0
                    }
                }
                _ => f64::NAN,
            };
            // NaN can enter the Set in JS. It would never satisfy `boundary >
            // address` in `regionSpan`, so dropping it is observably identical.
            if !value.is_nan() {
                addresses.insert(value as i64);
            }
        }
    }
    Ok(addresses.into_iter().collect())
}

/// `regionSpan(address, boundaries, limit)`.
pub fn region_span(address: i64, boundaries: &[i64], limit: i64) -> Span {
    let next = boundaries.iter().copied().find(|boundary| *boundary > address).unwrap_or(limit);
    Span { start: address, end: next }
}

/// `intersect(items, executable)`.
pub fn intersect(items: &[Span], executable: &[Span]) -> Result<Vec<Span>, String> {
    let parts: Vec<Span> = items
        .iter()
        .flat_map(|item| executable.iter().filter_map(move |region| overlap(*item, *region)))
        .collect();
    normalized(&parts, "intersected span")
}

pub struct CanonicalSpans {
    pub exact: Vec<Span>,
    pub semantic: Vec<Span>,
    pub non_c: Vec<NonCRange>,
    pub noncanonical_semantic_claims: Vec<ClaimedRegion>,
}

pub fn canonical_spans(root: &Path, executable: &[Span]) -> Result<CanonicalSpans, String> {
    let limit = executable.last().map(|item| item.end).unwrap_or(ROM_BASE);
    let starts = boundaries(root)?;
    let mut exact = Vec::new();
    for path in source_names(&root.join("exact")) {
        if let Some(address) = address_from_name(&path, ".c") {
            if canonical_c(&read_text(&path)?) {
                exact.push(region_span(address, &starts, limit));
            }
        }
    }
    let semantic = semantic_spans(root, &root.join("semantic/main-regions.json"), &starts, limit, executable)?;
    Ok(CanonicalSpans {
        exact: intersect(&exact, executable)?,
        semantic: semantic.semantic,
        non_c: semantic.non_c,
        noncanonical_semantic_claims: semantic.claims,
    })
}

#[derive(Debug)]
pub struct SemanticSpans {
    pub semantic: Vec<Span>,
    pub non_c: Vec<NonCRange>,
    pub claims: Vec<ClaimedRegion>,
}

/// `/^0x080[0-9a-f]{5}$/i`.
fn is_owner_entry(text: &str) -> bool {
    let lowered = text.to_ascii_lowercase();
    let Some(rest) = lowered.strip_prefix("0x080") else { return false };
    rest.len() == 5 && rest.chars().all(|c| c.is_ascii_hexdigit())
}

pub fn semantic_spans(
    root: &Path,
    path: &Path,
    boundaries: &[i64],
    limit: i64,
    executable: &[Span],
) -> Result<SemanticSpans, String> {
    let document = read_json(path)?;
    let owners = document.get("main_owners").and_then(Json::as_array);
    if document.get("format") != Some(&Json::Num(1.0)) || owners.is_none() {
        return Err("semantic/main-regions.json lacks main owners".into());
    }
    let owners = owners.unwrap();
    let mut semantic: Vec<Span> = Vec::new();
    let mut claims: Vec<ClaimedRegion> = Vec::new();

    // `addSemantic(entry, ranges)`: a source can be compiled into the full
    // claimed manifest while still being deliberately noncanonical (register
    // pins / inline asm). It remains reviewed semantic C, never byte-exact C;
    // naming that replacement here is what stops the claimed-manifest
    // cross-check from promoting it by accident.
    let mut add_semantic = |entry: &str, ranges: Vec<Span>| -> Result<(), String> {
        semantic.extend(ranges.iter().copied());
        let stem = entry[2..].to_ascii_lowercase();
        let claimed_source = root.join("exact").join(format!("{stem}.c"));
        if claimed_source.exists() && !canonical_c(&read_text(&claimed_source)?) {
            claims.extend(ranges.into_iter().map(|range| ClaimedRegion {
                span: range,
                source: format!("exact/{stem}.c"),
            }));
        }
        Ok(())
    };

    for (index, owner) in owners.iter().enumerate() {
        let entry = owner.get("entry").and_then(Json::as_str);
        let Some(entry) = entry.filter(|text| is_owner_entry(text)) else {
            return Err(format!("semantic owner {index} has an invalid entry"));
        };
        let source = root.join("semantic").join(format!("{}.c", entry[2..].to_ascii_lowercase()));
        if !source.exists() {
            return Err(format!(
                "semantic owner {entry} has no canonical source ({})",
                source.display()
            ));
        }
        let ranges = owner.get("executable_ranges").and_then(Json::as_array);
        let Some(ranges) = ranges.filter(|list| !list.is_empty()) else {
            return Err(format!("semantic owner {entry} has no executable ranges"));
        };
        let parsed = ranges
            .iter()
            .enumerate()
            .map(|(range_index, range)| {
                span(range, &format!("semantic owner {index} range {range_index}"))
            })
            .collect::<Result<Vec<Span>, String>>()?;
        add_semantic(entry, parsed)?;
    }

    let non_c = match document.get("non_c_ranges") {
        None | Some(Json::Null) => Vec::new(),
        Some(list) => {
            let items = list.as_array().ok_or("semantic non-C ranges must be an array")?;
            items
                .iter()
                .enumerate()
                .map(|(index, item)| {
                    let result = span(item, &format!("semantic non-C range {index}"))?;
                    let kind = item.get("kind").and_then(Json::as_str).unwrap_or("");
                    let evidence = nonempty_string(item, "evidence");
                    if !["literal_pool", "alignment_padding", "lookup_table"].contains(&kind)
                        || evidence.is_none()
                    {
                        return Err(format!(
                            "semantic non-C range {index} is not an evidenced literal-pool/alignment/lookup span"
                        ));
                    }
                    Ok(NonCRange {
                        span: result,
                        kind: kind.to_string(),
                        evidence: evidence.unwrap(),
                    })
                })
                .collect::<Result<Vec<NonCRange>, String>>()?
        }
    };

    // A source without a special multi-range registration owns its canonical
    // source boundary span. Registered owners deliberately override that
    // default (their range can exclude a literal pool or combine fragments).
    let registered: std::collections::BTreeSet<i64> = owners
        .iter()
        .map(|owner| match owner.get("entry") {
            Some(Json::Str(text)) => parse_int_hex_prefix(text),
            _ => i64::MIN,
        })
        .collect();
    for source in source_names(&root.join("semantic")) {
        let Some(address) = address_from_name(&source, ".c") else { continue };
        if registered.contains(&address) {
            continue;
        }
        // ------------------------------------------------------------------
        // BUG NOTE — REPRODUCED DELIBERATELY, NOT FIXED.
        //
        // The TypeScript builds this entry as
        //     `0x${address.toString(16)}`
        // with NO `padStart(8, "0")`, and `addSemantic` then does
        // `entry.slice(2)` to strip the "0x". Every main-image address is
        // 0x080xxxxx, whose `toString(16)` is only SEVEN digits ("80006fc"),
        // so slicing two characters off "0x80006fc" yields "80006fc" and the
        // lookup becomes `exact/80006fc.c` instead of `exact/080006fc.c`.
        // That path can never exist, so the noncanonical-semantic-claim
        // registration is silently skipped for every UNREGISTERED semantic
        // source. The registered branch above is unaffected because it uses
        // `owner.entry`, which is already a zero-padded "0x080xxxxx" string.
        //
        // Correct spelling would be `to_string_16_pad_start(address, 8)`.
        //
        // Status on the real tree as of this port: LATENT. The exact/ and
        // semantic/ address sets are disjoint (0 of 554 unregistered semantic
        // sources have an exact/ twin under either the buggy or the correct
        // path), and all 1456 exact/ sources are canonical, so the claim list
        // is empty either way. It becomes live the moment a semantic source
        // gains a noncanonical exact/ twin — at which point the audit would
        // wrongly report "unsupported claimed-C fragment" for it.
        // ------------------------------------------------------------------
        let entry = format!("0x{address:x}");
        add_semantic(&entry, vec![region_span(address, boundaries, limit)])?;
    }

    let intersected_claims = claims
        .iter()
        .map(|claim| {
            Ok(intersect(&[claim.span], executable)?
                .into_iter()
                .map(|range| ClaimedRegion { span: range, source: claim.source.clone() })
                .collect::<Vec<_>>())
        })
        .collect::<Result<Vec<_>, String>>()?
        .concat();

    Ok(SemanticSpans {
        semantic: intersect(&semantic, executable)?,
        non_c,
        claims: intersected_claims,
    })
}

/// `Number.parseInt(text, 16)` for the "0x080xxxxx" owner entries.
///
/// PORT NOTE: `parseInt` with radix 16 accepts and IGNORES a "0x" prefix, then
/// stops at the first non-hex character rather than erroring.
fn parse_int_hex_prefix(text: &str) -> i64 {
    let trimmed = text.trim_start_matches(crate::js::is_js_space);
    let body = trimmed
        .strip_prefix("0x")
        .or_else(|| trimmed.strip_prefix("0X"))
        .unwrap_or(trimmed);
    let digits: String = body.chars().take_while(|c| c.is_ascii_hexdigit()).collect();
    if digits.is_empty() {
        return i64::MIN; // NaN, which no address can equal.
    }
    i64::from_str_radix(&digits, 16).unwrap_or(i64::MIN)
}

/// The gate's own denominator check: an empty mask means the audit reconciled
/// nothing, which prints exactly like a clean run.
pub fn executable_is_nonempty(executable: &[Span]) -> bool {
    let mut mask = vec![0u8; ROM_SIZE];
    mark(&mut mask, executable, 1);
    !intersections(&mask, 1).is_empty()
}
