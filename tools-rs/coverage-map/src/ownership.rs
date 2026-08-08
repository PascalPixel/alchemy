// Who owns which executable bytes: byte-exact C, then reviewed semantic C.
//
// WHY: the whole map is a claim about ownership, and a claim is only worth
// drawing if it is derived from tracked evidence rather than estimated. Every
// function here refuses to guess -- an owner whose extent cannot be established
// from a tracked file is reported as unresolved instead of being sized by
// assumption.

use std::collections::BTreeSet;

use crate::js;
use crate::jsnum::{hex8, parse_hex};
use crate::names::{assembly_source, main_source, overlay_assembly, semantic_overlay_source};
use crate::ordered::OrderedMap;
use crate::spans::{intersect, normalize, region_span, Span};
use crate::tree::{SourceTree, ROM_BASE};

/// Audited main-image region starts, ascending.
pub fn main_boundaries(tree: &SourceTree) -> Vec<i64> {
    // `new Set<number>()` then `.sort((l, r) => l - r)`: a numeric sort of a
    // de-duplicated integer set, which is exactly a `BTreeSet<i64>`.
    let mut addresses: BTreeSet<i64> = BTreeSet::new();
    for name in tree.list("exact") {
        if let Some(digits) = main_source(&name) {
            addresses.insert(parse_hex(digits));
        }
    }
    walk(tree, "asm", &mut addresses);
    if let Some(alignment) = tree.read("asm/alignment.json") {
        if let Ok(document) = crate::json::parse(&alignment) {
            if let Some(list) = document.get("addresses").and_then(|value| value.as_array()) {
                for entry in list {
                    if let Some(text) = entry.as_str() {
                        addresses.insert(parse_hex(text));
                    }
                }
            }
        }
    }
    addresses.into_iter().collect()
}

/// The recursive `asm/` walk. A name without a `.` is treated as a directory,
/// which is why `SourceTree::list` has to report subdirectories.
fn walk(tree: &SourceTree, directory: &str, addresses: &mut BTreeSet<i64>) {
    for name in tree.list(directory) {
        if let Some(digits) = assembly_source(&name) {
            addresses.insert(parse_hex(digits));
        } else if !name.contains('.') {
            walk(tree, &format!("{directory}/{name}"), addresses);
        }
    }
}

/// `executable.at(-1)?.end ?? ROM_BASE`.
pub fn extent_limit(executable: &[Span]) -> i64 {
    executable.last().map(|span| span.end).unwrap_or(ROM_BASE)
}

/// Byte-exact C ownership in the main image, keyed by source address.
pub fn exact_main_spans(tree: &SourceTree, executable: &[Span]) -> OrderedMap<i64, Vec<Span>> {
    let boundaries = main_boundaries(tree);
    let limit = extent_limit(executable);
    let mut owned: OrderedMap<i64, Vec<Span>> = OrderedMap::new();
    for name in tree.list("exact") {
        let Some(digits) = main_source(&name) else { continue };
        // Register-pinned, inline-assembly and fakematch C is not a C claim.
        let Some(source) = tree.read(&format!("exact/{name}")) else { continue };
        if !js::canonical_c_source(&source) {
            continue;
        }
        let address = parse_hex(digits);
        let spans = intersect(&[region_span(address, &boundaries, limit)], executable);
        if !spans.is_empty() {
            owned.insert(address, spans);
        }
    }
    owned
}

/// A verified C placeholder inside an overlay's reconstruction assembly.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct OverlayOwner {
    pub label: String,
    pub entry: i64,
    pub spans: Vec<Span>,
}

pub fn overlay_placeholder_owners(source: &str) -> Vec<OverlayOwner> {
    let mut owners: Vec<OverlayOwner> = Vec::new();
    let mut owner: Option<OverlayOwner> = None;
    let mut cursor: i64 = 0;
    for line in js::split_lines(source) {
        if let Some(digits) = js::alchemy_c_label(line) {
            flush(&mut owner, &mut owners);
            cursor = parse_hex(digits);
            owner = Some(OverlayOwner {
                label: format!("AlchemyC_{}", digits.to_ascii_lowercase()),
                entry: cursor,
                spans: Vec::new(),
            });
            continue;
        }
        if owner.is_some() && (js::blank_line(line) || js::local_label(line).is_some()) {
            continue;
        }
        // PORT NOTE: the TypeScript matches the `.space` reservation *before*
        // checking that an owner is open, then requires the owner. A reservation
        // outside an owner therefore falls through to the trailing flush, which
        // is a no-op; the ordering is preserved here so the cursor is only ever
        // advanced inside an owner.
        if let Some(size) = js::space_directive(line) {
            if let Some(open) = owner.as_mut() {
                open.spans.push(Span::new(cursor, cursor + size));
                cursor += size;
                continue;
            }
        }
        if !line.trim_matches(js::is_js_space).is_empty() {
            flush(&mut owner, &mut owners);
        }
    }
    flush(&mut owner, &mut owners);
    owners
}

/// Only an owner that actually reserved bytes is recorded.
fn flush(owner: &mut Option<OverlayOwner>, owners: &mut Vec<OverlayOwner>) {
    if let Some(open) = owner.take() {
        if !open.spans.is_empty() {
            owners.push(open);
        }
    }
}

pub fn overlay_placeholder_spans(source: &str) -> Vec<Span> {
    let owners = overlay_placeholder_owners(source);
    let flat: Vec<Span> = owners.into_iter().flat_map(|owner| owner.spans).collect();
    normalize(&flat)
}

pub fn exact_overlay_owners(tree: &SourceTree) -> OrderedMap<String, Vec<OverlayOwner>> {
    let mut owned: OrderedMap<String, Vec<OverlayOwner>> = OrderedMap::new();
    for name in tree.list("assets/code") {
        let Some(overlay) = overlay_assembly(&name) else { continue };
        let overlay = overlay.to_string();
        let Some(source) = tree.read(&format!("assets/code/{name}")) else { continue };
        let owners: Vec<OverlayOwner> = overlay_placeholder_owners(&source)
            .into_iter()
            .filter(|owner| {
                let path = format!("exact/{overlay}_c_{}.c", hex8(owner.entry));
                tree.read(&path).is_some_and(|text| js::canonical_c_source(&text))
            })
            .collect();
        if !owners.is_empty() {
            owned.insert(overlay, owners);
        }
    }
    owned
}

pub fn exact_overlay_spans(tree: &SourceTree) -> OrderedMap<String, Vec<Span>> {
    let mut result: OrderedMap<String, Vec<Span>> = OrderedMap::new();
    for (overlay, owners) in exact_overlay_owners(tree).iter() {
        let flat: Vec<Span> = owners.iter().flat_map(|owner| owner.spans.iter().copied()).collect();
        result.insert(overlay.clone(), normalize(&flat));
    }
    result
}

pub struct SemanticCoverage {
    pub main: OrderedMap<i64, Vec<Span>>,
    pub overlays: OrderedMap<String, Vec<Span>>,
    /// Natural source-owner spans before adjacent coverage is normalized.
    pub overlay_owners: OrderedMap<String, Vec<Span>>,
    pub sources: i64,
    pub unresolved: Vec<String>,
    pub main_census_closed: bool,
}

impl SemanticCoverage {
    /// The `options.semantic === undefined` literal from `buildCoverageMap`.
    pub fn empty() -> Self {
        Self {
            main: OrderedMap::new(),
            overlays: OrderedMap::new(),
            overlay_owners: OrderedMap::new(),
            sources: 0,
            unresolved: Vec::new(),
            main_census_closed: false,
        }
    }
}

/// A JSON number read where the TypeScript performs unchecked arithmetic.
///
/// PORT NOTE: `start + undefined` is `NaN` in JavaScript, and a `NaN` end makes
/// `end > start` false, so `normalize` silently drops the span. Reading a
/// missing or non-numeric size as `0` here produces an empty span, which
/// `normalize` drops for the same reason -- identical observable behaviour
/// without carrying `NaN` through integer arithmetic.
fn size_of(value: Option<&crate::json::Value>) -> i64 {
    value.and_then(|value| value.as_f64()).map(|number| number as i64).unwrap_or(0)
}

/// Reviewed semantic-C ownership.
pub fn semantic_spans(
    tree: &SourceTree,
    boundaries: &[i64],
    executable: &[Span],
    overlay_executable: &OrderedMap<String, Vec<Span>>,
) -> SemanticCoverage {
    let limit = extent_limit(executable);
    let mut main: OrderedMap<i64, Vec<Span>> = OrderedMap::new();
    let mut overlays: OrderedMap<String, Vec<Span>> = OrderedMap::new();
    let mut overlay_owners: OrderedMap<String, Vec<Span>> = OrderedMap::new();
    let mut unresolved: Vec<String> = Vec::new();
    let mut sources: i64 = 0;
    let mut main_census_closed = false;

    let mut owners: OrderedMap<i64, Vec<Span>> = OrderedMap::new();
    if let Some(text) = tree.read("semantic/main-regions.json") {
        if let Ok(document) = crate::json::parse(&text) {
            main_census_closed = document
                .get("ordinary_census")
                .and_then(|census| census.get("status"))
                .and_then(|status| status.as_str())
                == Some("closed");
            if let Some(list) = document.get("main_owners").and_then(|value| value.as_array()) {
                for owner in list {
                    let entry = owner.get("entry").and_then(|value| value.as_str()).unwrap_or("");
                    let ranges: Vec<Span> = owner
                        .get("executable_ranges")
                        .and_then(|value| value.as_array())
                        .unwrap_or(&[])
                        .iter()
                        .map(|range| {
                            let start =
                                parse_hex(range.get("address").and_then(|v| v.as_str()).unwrap_or(""));
                            Span::new(start, start + size_of(range.get("size")))
                        })
                        .collect();
                    owners.insert(parse_hex(entry), ranges);
                }
            }
        }
    }

    for name in tree.list("semantic") {
        let Some(digits) = main_source(&name) else { continue };
        sources += 1;
        let address = parse_hex(digits);
        let declared = match owners.get(&address) {
            Some(ranges) => ranges.clone(),
            None => vec![region_span(address, boundaries, limit)],
        };
        let spans = intersect(&declared, executable);
        if !spans.is_empty() {
            main.insert(address, spans);
        } else {
            unresolved.push(format!("semantic/{name}"));
        }
    }

    let reviewed_document = tree.read("semantic/regions.json");
    let mut reviewed: OrderedMap<String, i64> = OrderedMap::new();
    let mut full_overlays: OrderedMap<String, String> = OrderedMap::new();
    if let Some(text) = &reviewed_document {
        if let Ok(document) = crate::json::parse(text) {
            if let Some(list) = document.get("manual_regions").and_then(|value| value.as_array()) {
                for region in list {
                    let overlay = region.get("overlay").and_then(|v| v.as_str()).unwrap_or("");
                    let entry = parse_hex(region.get("entry").and_then(|v| v.as_str()).unwrap_or(""));
                    reviewed.insert(format!("{overlay}@{entry}"), size_of(region.get("span_bytes")));
                }
            }
            // A whole-overlay claim sizes every owner in one reviewed assertion.
            if let Some(list) = document.get("full_overlays").and_then(|value| value.as_array()) {
                for claim in list {
                    let overlay = claim.get("overlay").and_then(|v| v.as_str()).unwrap_or("");
                    let evidence =
                        claim.get("evidence").and_then(|v| v.as_str()).unwrap_or("").to_string();
                    full_overlays.insert(overlay.to_string(), evidence);
                }
            }
        }
    }
    let mut claimed_sources: Vec<String> = Vec::new();

    for name in tree.list("semantic") {
        let Some((overlay, entry)) = semantic_overlay_source(&name) else { continue };
        sources += 1;
        let overlay = overlay.to_string();
        let address = parse_hex(entry);
        if full_overlays.contains_key(&overlay) {
            if !claimed_sources.contains(&overlay) {
                claimed_sources.push(overlay);
            }
            continue;
        }
        let key = format!("{overlay}@{address}");
        let Some(span) = reviewed.get(&key).copied() else {
            // An unlisted owner is reported, never estimated.
            unresolved.push(format!("semantic/{name}"));
            continue;
        };
        let owner = Span::new(address, address + span);
        let mut existing = overlay_owners.get(&overlay).cloned().unwrap_or_default();
        existing.push(owner);
        overlay_owners.insert(overlay.clone(), existing);
        let mut existing = overlays.get(&overlay).cloned().unwrap_or_default();
        existing.push(owner);
        overlays.insert(overlay, existing);
    }

    // An empty claim credits nothing rather than crediting the whole overlay.
    let claims: Vec<String> = full_overlays.keys().cloned().collect();
    for overlay in claims {
        let extent = overlay_executable.get(&overlay).cloned().unwrap_or_default();
        if !claimed_sources.contains(&overlay) || extent.is_empty() {
            continue;
        }
        let mut merged = overlays.get(&overlay).cloned().unwrap_or_default();
        merged.extend(extent);
        overlays.insert(overlay, normalize(&merged));
    }
    let keys: Vec<String> = overlays.keys().cloned().collect();
    for overlay in keys {
        let spans = overlays.get(&overlay).cloned().unwrap_or_default();
        overlays.insert(overlay, normalize(&spans));
    }
    SemanticCoverage { main, overlays, overlay_owners, sources, unresolved, main_census_closed }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::tree::work_tree_at;
    use std::path::PathBuf;

    fn fixture(root: &str, files: &[(&str, &str)]) -> (PathBuf, SourceTree) {
        let base = crate::testsupport::scratch(root);
        for (path, body) in files {
            let full = base.join(path);
            std::fs::create_dir_all(full.parent().expect("fixture paths are nested")).expect("mkdir");
            std::fs::write(&full, body).expect("write fixture");
        }
        let tree = work_tree_at(base.clone());
        (base, tree)
    }

    const C_BODY: &str = "void sub_08000100(void) {\n  return;\n}\n";

    #[test]
    fn overlay_placeholder_owners_reads_labels_and_reservations() {
        let source = "\
\t.text
AlchemyC_08001000:
\t.space 0x10
.L_local:
\t.space 8
AlchemyC_08002000:

\t.space 0x8
";
        let owners = overlay_placeholder_owners(source);
        assert_eq!(owners.len(), 2);
        assert_eq!(owners[0].label, "AlchemyC_08001000");
        assert_eq!(owners[0].entry, 0x0800_1000);
        assert_eq!(owners[0].spans, vec![Span::new(0x0800_1000, 0x0800_1010), Span::new(0x0800_1010, 0x0800_1018)]);
        assert_eq!(owners[1].spans, vec![Span::new(0x0800_2000, 0x0800_2008)]);
        assert_eq!(overlay_placeholder_spans(source).len(), 2);
    }

    #[test]
    fn a_foreign_line_closes_the_open_owner() {
        let source = "AlchemyC_08001000:\n\tbx lr\n\t.space 0x10\n";
        assert!(overlay_placeholder_owners(source).is_empty(), "no reservation, no owner");
        let source = "AlchemyC_08001000:\n\t.space 0x10\n\tbx lr\n\t.space 0x10\n";
        let owners = overlay_placeholder_owners(source);
        assert_eq!(owners.len(), 1);
        assert_eq!(owners[0].spans, vec![Span::new(0x0800_1000, 0x0800_1010)]);
    }

    #[test]
    fn a_bare_reservation_outside_an_owner_is_ignored() {
        assert!(overlay_placeholder_owners("\t.space 0x40\n").is_empty());
    }

    #[test]
    fn crlf_input_still_splits_into_lines() {
        // JS `.split(/\r?\n/)`; a `.`-based line matcher would see one line.
        let owners = overlay_placeholder_owners("AlchemyC_08001000:\r\n\t.space 0x10\r\n");
        assert_eq!(owners.len(), 1);
    }

    #[test]
    fn main_boundaries_gathers_exact_asm_and_alignment() {
        let (_base, tree) = fixture(
            "boundaries",
            &[
                ("exact/08000100.c", C_BODY),
                ("exact/notes.md", "ignored"),
                ("asm/08000200.s", ""),
                ("asm/regions/08000300.s", ""),
                ("asm/alignment.json", "{\"addresses\":[\"08000400\"]}"),
            ],
        );
        assert_eq!(
            main_boundaries(&tree),
            vec![0x0800_0100, 0x0800_0200, 0x0800_0300, 0x0800_0400],
        );
    }

    #[test]
    fn exact_main_spans_requires_canonical_c() {
        let (_base, tree) = fixture(
            "exact-main",
            &[
                ("exact/08000100.c", C_BODY),
                ("exact/08000200.c", "void f(void) { asm volatile (\"nop\"); }\n"),
                ("asm/08000200.s", ""),
                ("asm/08000300.s", ""),
            ],
        );
        let executable = vec![Span::new(0x0800_0100, 0x0800_0300)];
        let owned = exact_main_spans(&tree, &executable);
        assert_eq!(owned.len(), 1, "inline assembly is not a C claim");
        assert_eq!(owned.get(&0x0800_0100), Some(&vec![Span::new(0x0800_0100, 0x0800_0200)]));
    }

    #[test]
    fn exact_overlay_owners_requires_a_matching_c_file() {
        let (_base, tree) = fixture(
            "exact-overlay",
            &[
                (
                    "assets/code/resource_1a2_overlay.s",
                    "AlchemyC_08001000:\n\t.space 0x10\nAlchemyC_08002000:\n\t.space 0x10\n",
                ),
                ("exact/resource_1a2_c_08001000.c", C_BODY),
            ],
        );
        let owned = exact_overlay_owners(&tree);
        assert_eq!(owned.len(), 1);
        let owners = owned.get(&"resource_1a2".to_string()).expect("overlay present");
        assert_eq!(owners.len(), 1, "the owner without a C file is dropped");
        assert_eq!(owners[0].entry, 0x0800_1000);
        let spans = exact_overlay_spans(&tree);
        assert_eq!(spans.get(&"resource_1a2".to_string()), Some(&vec![Span::new(0x0800_1000, 0x0800_1010)]));
    }

    #[test]
    fn semantic_spans_covers_every_branch() {
        let (_base, tree) = fixture(
            "semantic",
            &[
                ("semantic/08000100.c", C_BODY),
                ("semantic/08000500.c", C_BODY),
                ("semantic/08000900.c", C_BODY),
                ("semantic/resource_1_c_08001000.c", C_BODY),
                ("semantic/resource_2_c_08002000.c", C_BODY),
                ("semantic/resource_3_c_08003000.c", C_BODY),
                (
                    "semantic/main-regions.json",
                    "{\"ordinary_census\":{\"status\":\"closed\"},\"main_owners\":[\
                       {\"entry\":\"08000500\",\"executable_ranges\":[\
                         {\"address\":\"08000500\",\"size\":16},{\"address\":\"08000600\",\"size\":16}]}]}",
                ),
                (
                    "semantic/regions.json",
                    "{\"manual_regions\":[{\"overlay\":\"resource_1\",\"entry\":\"08001000\",\"span_bytes\":32}],\
                      \"full_overlays\":[{\"overlay\":\"resource_2\",\"evidence\":\"reviewed\"},\
                                         {\"overlay\":\"resource_9\",\"evidence\":\"\"}]}",
                ),
            ],
        );
        let boundaries = vec![0x0800_0100, 0x0800_0200, 0x0800_0900];
        let executable = vec![Span::new(0x0800_0100, 0x0800_0700)];
        let mut overlay_executable: OrderedMap<String, Vec<Span>> = OrderedMap::new();
        overlay_executable.insert("resource_2".to_string(), vec![Span::new(0x0800_2000, 0x0800_2100)]);
        overlay_executable.insert("resource_9".to_string(), vec![Span::new(0x0800_9000, 0x0800_9100)]);

        let coverage = semantic_spans(&tree, &boundaries, &executable, &overlay_executable);
        assert!(coverage.main_census_closed);
        assert_eq!(coverage.sources, 6);
        // Region-derived owner.
        assert_eq!(coverage.main.get(&0x0800_0100), Some(&vec![Span::new(0x0800_0100, 0x0800_0200)]));
        // Declared multi-range owner.
        assert_eq!(
            coverage.main.get(&0x0800_0500),
            Some(&vec![Span::new(0x0800_0500, 0x0800_0510), Span::new(0x0800_0600, 0x0800_0610)]),
        );
        // Outside the executable extent -> unresolved, not estimated.
        assert!(coverage.unresolved.contains(&"semantic/08000900.c".to_string()));
        // Reviewed overlay owner.
        assert_eq!(
            coverage.overlay_owners.get(&"resource_1".to_string()),
            Some(&vec![Span::new(0x0800_1000, 0x0800_1020)]),
        );
        // Honoured whole-overlay claim.
        assert_eq!(
            coverage.overlays.get(&"resource_2".to_string()),
            Some(&vec![Span::new(0x0800_2000, 0x0800_2100)]),
        );
        // A claim with no semantic source credits nothing.
        assert_eq!(coverage.overlays.get(&"resource_9".to_string()), None);
        // An overlay owner with no reviewed span is reported.
        assert!(coverage.unresolved.contains(&"semantic/resource_3_c_08003000.c".to_string()));
    }
}
