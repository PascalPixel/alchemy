//! Forbid a bare-stem routing key that more than one overlay source answers to.
//!
//! WHY THIS EXISTS. The routing key in the flag tables is the bare address, so
//! every stem-keyed container is OVERLAY-BLIND: an entry added for one overlay
//! silently applies to every other overlay that later gains a row at the same
//! offset. It does not surface as a byte diff. It surfaces two layers away as
//! `palette token plan does not reconstruct input` from `build_assets`, on an
//! overlay the change never mentioned.
//!
//! WHY IT IS REBUILT. The TypeScript looked for
//! `const [A-Z0-9_]+ = new Set\(\[ ... \]\)` against the RAW file, and that one
//! regex carried four separate blind spots:
//!
//!   1. No type argument allowed. `SCHED_CALL_DEST_DESCENDING_OVERLAY_SOURCES`
//!      (alchemy_gcc.ts:1688) and `SCHED_IMMEDIATE_BEFORE_POOL_OVERLAY_SOURCES`
//!      (:1880) are declared `new Set<string>([`, and were never inspected.
//!   2. `Set` only. `THUMB_LOW_REG_ORDER_SOURCES` (:595) and
//!      `THUMB_CALLEE_REG_ORDER_SOURCES` (:606) are `new Map([`, are keyed by
//!      exactly the bare stems this lint polices, and were never inspected.
//!   3. Raw text. A stem written inside a comment counted as a routing entry,
//!      so the prose that explains a decision could manufacture a collision.
//!   4. `[\s\S]*?\]\)` ends the body at the first `])` in it, so a container
//!      holding a nested array silently loses every entry after that point.
//!
//! A lint whose stated scope is "every stem-keyed set" and which cannot see a
//! `Map` or a type annotation is not policing the rule; it is policing one
//! spelling of it. This version parses declarations structurally and takes a
//! `Map`'s keys rather than its literals, because a `Map`'s VALUES are data --
//! `THUMB_LOW_REG_ORDER_SOURCES` maps `"080f9a30"` to `"01231230"`, eight
//! hex-legal characters that are a register order and not an address.

use std::collections::BTreeMap;
use std::fs;
use std::path::Path;

use crate::{blank_comments, declarations, is_stem, routing_keys, Finding, Report};

/// Floors. The 2026 tree declares ~180 containers holding ~1500 stem keys over
/// ~4000 `exact/` sources; these sit far under that.
const DECLARATION_FLOOR: usize = 60;
const KEY_FLOOR: usize = 200;
const OWNER_FLOOR: usize = 200;
/// Declarations the original regex could not express. Their presence is checked
/// by NAME, so a future edit that reverts the parser to a `new Set([`-only
/// match fails loudly instead of quietly shrinking the corpus again.
const MUST_INSPECT: [&str; 4] = [
    "SCHED_CALL_DEST_DESCENDING_OVERLAY_SOURCES",
    "SCHED_IMMEDIATE_BEFORE_POOL_OVERLAY_SOURCES",
    "THUMB_LOW_REG_ORDER_SOURCES",
    "THUMB_CALLEE_REG_ORDER_SOURCES",
];

/// Stem -> the `exact/` sources that answer to it.
///
/// The filename shape is `<owner>_c_<stem>.c`. Hand-matched rather than
/// regexed; JS `\d` and `$` semantics have no say here.
pub fn owners_by_stem(names: &[String]) -> BTreeMap<String, Vec<String>> {
    let mut owners: BTreeMap<String, Vec<String>> = BTreeMap::new();
    for name in names {
        let Some(head) = name.strip_suffix(".c") else { continue };
        let Some(index) = head.rfind("_c_") else { continue };
        let stem = &head[index + 3..];
        if !is_stem(stem) {
            continue;
        }
        owners.entry(stem.to_string()).or_default().push(name.clone());
    }
    for list in owners.values_mut() {
        list.sort();
    }
    owners
}

/// `source` is the text of `tools/lib/alchemy_gcc.ts`; `exact_names` is the
/// file listing of `exact/`.
pub fn analyse(source: &str, exact_names: &[String]) -> Report {
    let mut report = Report::new("overlay-stem-collision");
    let blanked = blank_comments(source);
    let containers = declarations(&blanked);
    let owners = owners_by_stem(exact_names);

    let mut keys = 0usize;
    let mut findings = Vec::new();
    let mut inspected: Vec<&str> = Vec::new();
    for container in &containers {
        inspected.push(&container.name);
        for (offset, value) in routing_keys(container) {
            if !is_stem(&value) {
                continue;
            }
            keys += 1;
            let matched = owners.get(&value).cloned().unwrap_or_default();
            if matched.len() > 1 {
                findings.push(Finding {
                    file: "tools/lib/alchemy_gcc.ts".to_string(),
                    line: crate::line_of(&blanked, offset),
                    message: format!(
                        "{} \"{value}\" matches {} overlay sources ({}) -- the entry applies to \
                         overlays it was never meant for; move it to the path-keyed container \
                         naming the overlay it was added for",
                        container.name,
                        matched.len(),
                        matched.join(", ")
                    ),
                });
            }
        }
    }

    report.count_with_floor("containers_inspected", containers.len(), DECLARATION_FLOOR);
    report.count_with_floor("stem_keys_inspected", keys, KEY_FLOOR);
    report.count_with_floor("exact_sources_indexed", owners.len(), OWNER_FLOOR);
    for name in MUST_INSPECT {
        if !inspected.contains(&name) {
            report.blind.push(format!(
                "overlay-stem-collision: {name} was not inspected -- it is declared with a type \
                 argument or as a Map, the exact spellings the previous regex could not see"
            ));
        }
    }
    report.count("collisions", findings.len());
    report.findings = findings;
    report
}

pub fn read_tree(root: &Path) -> Result<(String, Vec<String>), String> {
    let path = root.join("tools/lib/alchemy_gcc.ts");
    let source =
        fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?;
    let directory = root.join("exact");
    let mut names: Vec<String> = fs::read_dir(&directory)
        .map_err(|error| format!("{}: {error}", directory.display()))?
        .filter_map(Result::ok)
        .filter_map(|entry| entry.file_name().into_string().ok())
        .collect();
    names.sort();
    Ok((source, names))
}

#[cfg(test)]
mod tests {
    use super::*;

    fn exact() -> Vec<String> {
        (0..300)
            .map(|index| format!("resource_{index:03x}_c_{:08x}.c", 0x0200_0000 + index * 4))
            .chain([
                "resource_3bb_c_020039fc.c".to_string(),
                "resource_3c8_c_020039fc.c".to_string(),
                "resource_100_c_08001234.c".to_string(),
            ])
            .collect()
    }

    #[test]
    fn a_stem_owned_by_two_overlays_is_a_collision() {
        let source = "const A_SOURCES = new Set([\"020039fc\"]);\n";
        let report = analyse(source, &exact());
        assert_eq!(report.findings.len(), 1);
        assert!(report.findings[0].message.contains("resource_3bb"));
        assert!(report.findings[0].message.contains("resource_3c8"));
    }

    #[test]
    fn a_uniquely_owned_stem_is_fine() {
        let source = "const A_SOURCES = new Set([\"08001234\"]);\n";
        assert!(analyse(source, &exact()).findings.is_empty());
    }

    #[test]
    fn a_typed_set_is_inspected() {
        // alchemy_gcc.ts:1688 and :1880 are spelled exactly like this and the
        // previous regex could not see either.
        let source = "const A_SOURCES = new Set<string>([\"020039fc\"]);\n";
        assert_eq!(analyse(source, &exact()).findings.len(), 1);
    }

    #[test]
    fn a_map_is_inspected_by_its_keys_only() {
        // alchemy_gcc.ts:595 and :606. The value is a register order that
        // happens to be eight hex-legal characters; only the key is a stem.
        let source = "const A_SOURCES = new Map([[\"020039fc\", \"01231230\"]]);\n";
        let report = analyse(source, &exact());
        assert_eq!(report.findings.len(), 1);
        assert!(report.findings[0].message.contains("020039fc"));
    }

    #[test]
    fn a_stem_named_in_a_comment_is_not_a_routing_entry() {
        let source = "// witness 020039fc was tried and rejected\n\
                      const A_SOURCES = new Set([\"08001234\"]);\n";
        assert!(analyse(source, &exact()).findings.is_empty());
    }

    #[test]
    fn a_nested_array_does_not_truncate_the_container() {
        // `[\s\S]*?\]\)` stops at the first `])`, losing everything after it.
        let source = "const A_SOURCES = new Set([\"08001234\", ...list[0], \"020039fc\"]);\n";
        assert_eq!(
            analyse(source, &exact()).findings.len(),
            1,
            "the entry after the nested bracket must still be inspected"
        );
    }

    #[test]
    fn an_empty_corpus_is_a_failure_and_not_a_pass() {
        let report = analyse("", &[]);
        assert!(report.findings.is_empty());
        assert!(!report.ok(), "scanning nothing is not passing");
    }

    #[test]
    fn losing_the_typed_and_map_spellings_is_reported_as_blindness() {
        // The regression guard: if the parser is ever narrowed back to
        // `new Set([`, these four names disappear and the lint says so.
        let report = analyse("", &[]);
        for name in MUST_INSPECT {
            assert!(report.blind.iter().any(|reason| reason.contains(name)), "{name}");
        }
    }
}
