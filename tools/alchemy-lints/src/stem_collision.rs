//! Detect overlay collisions in the native routing tables.
//!
//! A bare address route applies to every C owner whose filename ends in that
//! address. A path-keyed route does not. The lint asks the native routing
//! function which distinction it actually makes, then compares that answer
//! with the current exact-owner inventory. It therefore checks the behavior
//! that matters without maintaining a second list of routing containers.

use std::collections::BTreeMap;
use std::fs;
use std::path::Path;

use alchemy_routing::routing::{cflags_for_target, cflags_for_target_source, CompilerTarget};

use crate::{is_stem, Finding, Report};

const OWNER_FLOOR: usize = 200;

/// Stem -> the exact C sources answering to it.
pub fn owners_by_stem(names: &[String]) -> BTreeMap<String, Vec<String>> {
    let mut owners = BTreeMap::new();
    for name in names {
        let Some(head) = name.strip_suffix(".c") else {
            continue;
        };
        let Some(index) = head.rfind("_c_") else {
            if is_stem(head) {
                owners
                    .entry(head.to_string())
                    .or_insert_with(Vec::new)
                    .push(name.clone());
            }
            continue;
        };
        let stem = &head[index + 3..];
        if !is_stem(stem) {
            continue;
        }
        owners
            .entry(stem.to_string())
            .or_insert_with(Vec::new)
            .push(name.clone());
    }
    for list in owners.values_mut() {
        list.sort();
    }
    owners
}

fn native_bare_route(stem: &str) -> bool {
    let baseline = cflags_for_target(CompilerTarget::Gs1);
    let bare = format!("/tmp/{stem}.c");
    cflags_for_target_source(CompilerTarget::Gs1, &bare) != baseline
}

/// Analyze exact C owners against the native routing behavior.
pub fn analyse(exact_names: &[String]) -> Report {
    let mut report = Report::new("overlay-stem-collision");
    let owners = owners_by_stem(exact_names);
    let mut routed_stems = 0usize;
    let mut collisions = Vec::new();

    for (stem, matched) in &owners {
        if !native_bare_route(stem) {
            continue;
        }
        routed_stems += 1;
        if matched.len() > 1 {
            collisions.push(Finding {
                file: "tools/alchemy-routing/src/routing_data.rs".to_string(),
                line: 0,
                message: format!(
                    "bare native route for {stem} matches {} C owners ({}) -- move the route to a path-keyed entry",
                    matched.len(),
                    matched.join(", ")
                ),
            });
        }
    }

    report.count_with_floor("exact_sources_indexed", owners.len(), OWNER_FLOOR);
    report.count_with_floor("routed_stems_inspected", routed_stems, 1);
    report.count("collisions", collisions.len());
    report.findings = collisions;
    report
}

pub fn read_tree(root: &Path) -> Result<Vec<String>, String> {
    let directory = root.join("exact");
    let mut names = fs::read_dir(&directory)
        .map_err(|error| format!("{}: {error}", directory.display()))?
        .filter_map(Result::ok)
        .filter_map(|entry| entry.file_name().into_string().ok())
        .filter(|name| name.ends_with(".c"))
        .collect::<Vec<_>>();
    names.sort();
    Ok(names)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn owners_group_overlay_rows_by_address() {
        let names = vec![
            "resource_a_c_020039fc.c".to_string(),
            "resource_b_c_020039fc.c".to_string(),
            "080049e8.c".to_string(),
        ];
        let owners = owners_by_stem(&names);
        assert_eq!(owners["020039fc"].len(), 2);
        assert_eq!(owners["080049e8"].len(), 1);
    }

    #[test]
    fn native_bare_route_catches_a_known_routed_stem() {
        let report = analyse(&[
            "resource_a_c_080049e8.c".to_string(),
            "resource_b_c_080049e8.c".to_string(),
        ]);
        assert_eq!(report.findings.len(), 1);
        assert!(report.findings[0].message.contains("080049e8"));
    }

    #[test]
    fn path_only_route_does_not_look_like_a_stem_collision() {
        let report = analyse(&[
            "resource_a_c_02000944.c".to_string(),
            "resource_b_c_02000944.c".to_string(),
        ]);
        assert!(report.findings.is_empty());
    }

    #[test]
    fn empty_corpus_is_not_a_pass() {
        let report = analyse(&[]);
        assert!(!report.ok());
        assert!(report
            .blind
            .iter()
            .any(|reason| reason.contains("exact_sources_indexed")));
    }
}
