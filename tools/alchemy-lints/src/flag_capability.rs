//! Check compiler-flag coverage through the native routing API.
//!
//! The old lint inspected a TypeScript helper that hand-enumerated routing
//! containers. That helper is retired. The native check now derives its corpus
//! from the C owners that are actually built and asks the Rust routing crate
//! for every command line, so a newly routed live owner cannot disappear from
//! the capability inventory by being omitted from a second list.

use std::collections::BTreeSet;
use std::fs;
use std::path::{Path, PathBuf};

use alchemy_routing::routing::{cflags_for_target, cflags_for_target_source, CompilerTarget};

use crate::{Finding, Report};

const SOURCE_FLOOR: usize = 100;

fn collect_c_files(directory: &Path, out: &mut Vec<PathBuf>) -> Result<(), String> {
    let entries =
        fs::read_dir(directory).map_err(|error| format!("{}: {error}", directory.display()))?;
    for entry in entries {
        let entry = entry.map_err(|error| format!("{}: {error}", directory.display()))?;
        let path = entry.path();
        if path.is_dir() {
            collect_c_files(&path, out)?;
        } else if path.extension().and_then(|ext| ext.to_str()) == Some("c") {
            out.push(path);
        }
    }
    Ok(())
}

/// Analyze all live exact and semantic C owners through native routing.
pub fn analyse(root: &Path) -> Result<Report, String> {
    let mut files = Vec::new();
    collect_c_files(&root.join("exact"), &mut files)?;
    collect_c_files(&root.join("semantic"), &mut files)?;
    files.sort();

    let baseline = cflags_for_target(CompilerTarget::Gs1);
    let baseline_set: BTreeSet<_> = baseline.iter().collect();
    let mut observed = BTreeSet::new();
    let mut routed_sources = 0usize;
    let mut findings = Vec::new();

    for path in &files {
        let source = path.to_string_lossy();
        let flags = cflags_for_target_source(CompilerTarget::Gs1, &source);
        if flags == baseline {
            continue;
        }
        routed_sources += 1;
        for flag in &flags {
            if !baseline_set.contains(&flag) {
                if !flag.starts_with('-') || flag.len() == 1 {
                    findings.push(Finding {
                        file: "tools/alchemy-routing/src/routing.rs".to_string(),
                        line: 0,
                        message: format!("native routing emitted malformed compiler flag {flag:?}"),
                    });
                }
                observed.insert(flag.clone());
            }
        }
    }

    let mut report = Report::new("flag-capability");
    report.count_with_floor("live_c_sources_inspected", files.len(), SOURCE_FLOOR);
    report.count_with_floor("routed_live_sources", routed_sources, 1);
    report.count_with_floor("native_flags_observed", observed.len(), 1);
    report.count("malformed_flags", findings.len());
    report.findings = findings;
    Ok(report)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn native_routing_exposes_known_flag() {
        let flags = cflags_for_target_source(CompilerTarget::Gs1, "/tmp/080049e8.c");
        assert!(flags.iter().any(|flag| flag == "-O1"));
    }

    #[test]
    fn baseline_is_not_reported_as_a_routed_flag() {
        let baseline = cflags_for_target(CompilerTarget::Gs1);
        let flags = cflags_for_target_source(CompilerTarget::Gs1, "/tmp/0deadbee.c");
        assert_eq!(flags, baseline);
    }
}
