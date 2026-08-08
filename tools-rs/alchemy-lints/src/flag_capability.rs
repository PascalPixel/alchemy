//! Prove the cross-host flag-capability probe can actually SEE every flag the
//! routing tables can emit.
//!
//! WHY THIS EXISTS. Every flag the live routing tables can emit must be
//! accepted by the staged binary that would receive it. Digest pinning cannot
//! catch the skew: a route added on one host family, with that host's rebuilt
//! and pinned binary, leaves every other host's staged binary
//! approved-but-behind, and the first symptom is an unrelated-looking mid-build
//! failure deep inside an overlay rebuild.
//!
//! THE AUDIT. `flagCapabilityLint` spawns the two staged compilers with
//! `evidencedRoutingFlags(...)` and greps stderr for rejected options. Four
//! findings, in descending order of how badly they break the claim:
//!
//!   1. THE FLAG LIST IS HAND-ENUMERATED. `evidencedRoutingFlags` sweeps
//!      `/tmp/<stem>.c` across `0x08000000..0x08100000`, which reaches every
//!      MAIN-IMAGE stem-keyed container for free, and then spreads a literal
//!      list of ~53 path-keyed and overlay-stem containers. That list is a
//!      hand-maintained copy of a fact about the file, which is the same
//!      mechanism-vs-memory defect `cache-key-lint` exists to forbid. SIX live
//!      containers are missing from it (reported below), so a flag reachable
//!      only through one of them is never offered to the binary the lint claims
//!      to have proven. Adding a container is the routine act; remembering to
//!      add it to a spread two thousand lines away is not.
//!
//!   2. A SPAWN THAT NEVER RAN READS AS A PASS. The result of `Bun.spawnSync`
//!      is used only for its stderr text. Exit status is never consulted. If
//!      the binary is missing, non-executable, or dies before parsing its
//!      arguments, stderr contains no `Unrecognized option` line and the lint
//!      returns green having proven nothing.
//!
//!   3. AN EMPTY FLAG LIST READS AS A PASS. Nothing requires
//!      `evidencedRoutingFlags` to have returned anything. Zero flags probed is
//!      indistinguishable from every flag accepted.
//!
//!   4. TWO STDERR SPELLINGS ARE RECOGNISED. `Unrecognized option \`-x'` and
//!      `Invalid option \`x'` are the fork's wording. Any other rejection
//!      phrasing -- and any rejection written to stdout -- is invisible.
//!
//! WHAT THIS CRATE FIXES. (1) is the one that needs a lint, because it is the
//! one no amount of care at the probe site can close: it is a synchronisation
//! bug between two distant lists. This module derives the routing containers
//! from the file and reports every one the probe's enumeration cannot reach.
//! (2) and (3) are fixed by construction in whatever runs the probe -- they are
//! "count what you did and floor it", and every `Report` here does that.

use std::fs;
use std::path::Path;

use crate::{blank_comments, declarations, is_stem, is_word_byte, routing_keys, Finding, Report};

/// Floors for the 2026 tree, which declares ~137 routing containers and spreads
/// ~53 of them into the probe.
const CONTAINER_FLOOR: usize = 60;
const ENUMERATED_FLOOR: usize = 20;
const CLASSIFIED_KEY_FLOOR: usize = 200;

/// The main-image sweep in `evidencedRoutingFlags` walks
/// `0x08000000..0x08100000` in steps of four and inspects `/tmp/<stem>.c`, so
/// any stem in that window is probed without being named anywhere.
fn covered_by_address_sweep(stem: &str) -> bool {
    let Ok(address) = u32::from_str_radix(stem, 16) else { return false };
    (0x0800_0000..0x0810_0000).contains(&address) && address % 4 == 0
}

/// How the probe could reach one routing key, if at all.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Reach {
    AddressSweep,
    /// A repository-relative path; only reachable if the container is spread
    /// into the probe's explicit list.
    NamedContainer,
    /// Not an address and not a path -- data, not a routing key.
    NotAKey,
}

fn classify(key: &str) -> Reach {
    if key.contains('/') {
        return Reach::NamedContainer;
    }
    if is_stem(key) {
        // An overlay stem (`0200xxxx`) falls outside the sweep window.
        if covered_by_address_sweep(key) {
            return Reach::AddressSweep;
        }
        return Reach::NamedContainer;
    }
    Reach::NotAKey
}

/// Every identifier mentioned inside the body of `function NAME(`.
///
/// Deliberately crude: any identifier occurrence counts, whether spread,
/// iterated, or passed. The question this answers is "could the probe possibly
/// reach this container", and a mention is the weakest sufficient evidence. A
/// stricter parse would produce false alarms, and a lint that cries wolf about
/// the flag tables is a lint people learn to skip.
pub fn identifiers_in_function(blanked: &str, name: &str) -> Option<Vec<String>> {
    let needle = format!("function {name}");
    let start = blanked.find(&needle)?;
    let open = blanked[start..].find('{').map(|offset| start + offset)?;
    let end = crate::balanced_end(blanked, open)?;
    let body = &blanked[open..end];
    let bytes = body.as_bytes();
    let mut names = Vec::new();
    let mut index = 0;
    while index < bytes.len() {
        if !is_word_byte(bytes[index]) || bytes[index].is_ascii_digit() {
            index += 1;
            continue;
        }
        let word_start = index;
        while index < bytes.len() && is_word_byte(bytes[index]) {
            index += 1;
        }
        names.push(body[word_start..index].to_string());
    }
    names.sort();
    names.dedup();
    Some(names)
}

/// Does a container's name mark it as a routing table? `*_SOURCES` and
/// `*_PATHS` is the file's own convention and every flag-bearing container
/// follows it.
fn is_routing_container(name: &str) -> bool {
    name.ends_with("_SOURCES") || name.ends_with("_PATHS")
}

pub fn analyse(source: &str) -> Report {
    let mut report = Report::new("flag-capability");
    let blanked = blank_comments(source);
    let containers: Vec<_> =
        declarations(&blanked).into_iter().filter(|one| is_routing_container(&one.name)).collect();

    let Some(mentioned) = identifiers_in_function(&blanked, "evidencedRoutingFlags") else {
        report.blind.push(
            "flag-capability: evidencedRoutingFlags was not found in the source -- the probe's \
             flag list cannot be checked, and an unchecked probe is the whole defect"
                .to_string(),
        );
        return report;
    };
    let enumerated: Vec<&String> =
        mentioned.iter().filter(|name| is_routing_container(name)).collect();

    let mut classified = 0usize;
    let mut findings = Vec::new();
    for container in &containers {
        let needs_naming = routing_keys(container).into_iter().any(|(_, key)| {
            let reach = classify(&key);
            if reach != Reach::NotAKey {
                classified += 1;
            }
            reach == Reach::NamedContainer
        });
        if !needs_naming {
            continue;
        }
        if enumerated.iter().any(|name| name.as_str() == container.name) {
            continue;
        }
        findings.push(Finding {
            file: "tools/lib/alchemy_gcc.ts".to_string(),
            line: container.line,
            message: format!(
                "{} holds path- or overlay-keyed routes and is not enumerated by \
                 evidencedRoutingFlags, so the flags it emits are never offered to the staged \
                 compilers -- the capability probe cannot see them and passes without having \
                 tested them",
                container.name
            ),
        });
    }

    report.count_with_floor("routing_containers", containers.len(), CONTAINER_FLOOR);
    report.count_with_floor("containers_enumerated_by_probe", enumerated.len(), ENUMERATED_FLOOR);
    report.count_with_floor("routing_keys_classified", classified, CLASSIFIED_KEY_FLOOR);
    report.count("unreachable_containers", findings.len());
    report.findings = findings;
    report
}

pub fn read_tree(root: &Path) -> Result<String, String> {
    let path = root.join("tools/lib/alchemy_gcc.ts");
    fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))
}

#[cfg(test)]
mod tests {
    use super::*;

    /// A miniature of the real file: a sweep-covered container, a path-keyed
    /// container that IS enumerated, and one that is not.
    const SOURCE: &str = "\
const MAIN_SOURCES = new Set([\"08004000\"]);
const SEEN_OVERLAY_SOURCES = new Set([\"exact/resource_1_c_02000000.c\"]);
const LOST_OVERLAY_SOURCES = new Set<string>([\"exact/resource_2_c_02000004.c\"]);
export function evidencedRoutingFlags(compiler) {
  for (const source of [
    ...SEEN_OVERLAY_SOURCES,
  ]) inspect(join(ROOT, source));
}
";

    #[test]
    fn a_container_the_probe_cannot_reach_is_reported() {
        let report = analyse(SOURCE);
        let names: Vec<&str> = report.findings.iter().map(|one| one.message.as_str()).collect();
        assert_eq!(report.findings.len(), 1, "{names:?}");
        assert!(report.findings[0].message.starts_with("LOST_OVERLAY_SOURCES"));
    }

    #[test]
    fn an_enumerated_container_is_not_reported() {
        assert!(!analyse(SOURCE)
            .findings
            .iter()
            .any(|one| one.message.starts_with("SEEN_OVERLAY_SOURCES")));
    }

    #[test]
    fn a_main_image_stem_needs_no_enumeration() {
        // The address sweep reaches it without anyone naming it, and demanding
        // a name would bury the real findings under a hundred false ones.
        assert!(!analyse(SOURCE)
            .findings
            .iter()
            .any(|one| one.message.starts_with("MAIN_SOURCES")));
        assert!(covered_by_address_sweep("08004000"));
        assert!(!covered_by_address_sweep("02000e3c"), "an overlay stem is outside the window");
        assert!(!covered_by_address_sweep("08004002"), "the sweep steps by four");
    }

    #[test]
    fn a_missing_probe_function_is_blindness_and_not_a_pass() {
        let report = analyse("const A_SOURCES = new Set([\"exact/x_c_02000000.c\"]);\n");
        assert!(report.findings.is_empty());
        assert!(!report.ok());
        assert!(report.blind[0].contains("evidencedRoutingFlags"));
    }

    #[test]
    fn an_empty_corpus_is_a_failure_and_not_a_pass() {
        assert!(!analyse("").ok(), "scanning nothing is not passing");
    }

    #[test]
    fn a_map_value_is_not_treated_as_a_route() {
        let source = "const A_SOURCES = new Map([[\"08004000\", \"01231230\"]]);\n\
                      function evidencedRoutingFlags() { return [...A_SOURCES]; }\n";
        assert!(analyse(source).findings.is_empty());
    }
}
