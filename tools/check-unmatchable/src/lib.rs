//! Gate for the registers, the unmatchable-owner ledger.
//!
//! A ledger nobody checks decays into folklore. Two failures matter and both are
//! silent without this: an entry naming an owner that no longer exists, and an
//! entry for an owner that has since gone exact. The second is the dangerous
//! one -- it tells the next agent to skip work that is already done, or worse,
//! to leave a solved owner unadopted.
//!
//! Port of `tools/check/check_unmatchable.ts`.

pub mod cli;

use std::collections::{BTreeMap, HashSet};
use std::path::{Path, PathBuf};

/// Axes that must both be spent before an owner may be sealed.
pub const REQUIRED_AXES: [&str; 2] = ["compiler", "shape"];

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct UnmatchableEntry {
    pub owner: String,
    pub floor: u64,
    pub axes: Vec<String>,
    pub reason: String,
}

/// Hand-rolled equivalent of

/// The unmatchable set, read from `semantic/unmatchable.json`.
///
/// It used to be a prose section that every contributor read on the way to
/// anything else. A list of owners that resisted is data for this gate, not
/// guidance: it belongs in a file the tool consults.
pub fn parse_unmatchable_file(root: &Path) -> Result<Vec<UnmatchableEntry>, String> {
    let path = root.join("semantic/unmatchable.json");
    let text = std::fs::read_to_string(&path)
        .map_err(|error| format!("cannot read {}: {error}", path.display()))?;
    let value: serde_json::Value =
        serde_json::from_str(&text).map_err(|error| format!("{}: {error}", path.display()))?;
    let rows = value
        .get("unmatchable")
        .and_then(|node| node.as_array())
        .ok_or_else(|| format!("{}: no `unmatchable` array", path.display()))?;
    let mut entries = Vec::new();
    for row in rows {
        let field = |name: &str| row.get(name).and_then(|node| node.as_str());
        let owner = field("owner")
            .ok_or_else(|| format!("{}: an entry has no owner", path.display()))?;
        let floor = row
            .get("floor_halfwords")
            .and_then(|node| node.as_u64())
            .ok_or_else(|| format!("{owner}: no floor_halfwords"))?;
        let axes: Vec<String> = row
            .get("axes")
            .and_then(|node| node.as_array())
            .map(|list| {
                list.iter()
                    .filter_map(|item| item.as_str().map(str::to_string))
                    .collect()
            })
            .unwrap_or_default();
        let reason = field("reason")
            .ok_or_else(|| format!("{owner}: no reason"))?;
        if axes.is_empty() || reason.trim().is_empty() {
            return Err(format!("{owner}: an entry needs axes and a reason"));
        }
        entries.push(UnmatchableEntry {
            owner: owner.to_string(),
            floor,
            axes,
            reason: reason.to_string(),
        });
    }
    Ok(entries)
}


/// Owners whose bytes match but whose source is not believed to be the
/// original's, read from `exact/provisional.json`.
///
/// The opposite case to the unmatchable set, and deliberately a separate register:
/// one says "this does not match", the other says "this matches and you should
/// not learn from it". Both have to be data, because `exact/` is the corpus
/// everything else is pattern-matched against, and a hack sitting in it
/// unlabelled teaches the wrong shape.
pub fn parse_provisional_file(root: &Path) -> Result<Vec<(String, String)>, String> {
    let path = root.join("exact/provisional.json");
    let text = std::fs::read_to_string(&path)
        .map_err(|error| format!("cannot read {}: {error}", path.display()))?;
    let value: serde_json::Value =
        serde_json::from_str(&text).map_err(|error| format!("{}: {error}", path.display()))?;
    let rows = value
        .get("provisional")
        .and_then(|node| node.as_array())
        .ok_or_else(|| format!("{}: no `provisional` array", path.display()))?;
    let mut entries = Vec::new();
    for row in rows {
        let field = |name: &str| row.get(name).and_then(|node| node.as_str());
        let owner = field("owner")
            .ok_or_else(|| format!("{}: an entry has no owner", path.display()))?;
        let reason = field("reason").unwrap_or("").trim();
        if reason.is_empty() {
            return Err(format!("{owner}: a provisional entry needs a reason"));
        }
        entries.push((owner.to_string(), reason.to_string()));
    }
    Ok(entries)
}

/// A fakematch entry must name an owner that is actually in `exact/`; once it
/// is reconstructed properly the entry has to go.
pub fn provisional_violations(
    entries: &[(String, String)],
    exact: &HashSet<String>,
) -> Vec<String> {
    entries
        .iter()
        .filter(|(owner, _)| !exact.contains(owner))
        .map(|(owner, _)| {
            format!("{owner} is registered provisional but is not in exact/; remove the entry")
        })
        .collect()
}

/// `.c` stems directly under `<root>/<directory>`. A missing directory yields an
/// empty set, exactly as the TypeScript does; callers that need a non-empty
/// corpus must say so themselves (see `main.rs`).
pub fn stems(root: &Path, directory: &str) -> HashSet<String> {
    let path = root.join(directory);
    let Ok(read) = std::fs::read_dir(&path) else {
        return HashSet::new();
    };
    let mut out = HashSet::new();
    for entry in read.flatten() {
        let name = entry.file_name().to_string_lossy().into_owned();
        if let Some(stem) = name.strip_suffix(".c") {
            out.insert(stem.to_string());
        }
    }
    out
}

pub fn violations(
    entries: &[UnmatchableEntry],
    exact: &HashSet<String>,
    semantic: &HashSet<String>,
) -> Vec<String> {
    let mut problems = Vec::new();
    let mut seen: HashSet<&str> = HashSet::new();
    for entry in entries {
        if seen.contains(entry.owner.as_str()) {
            problems.push(format!("{}: listed twice", entry.owner));
        }
        seen.insert(entry.owner.as_str());
        if exact.contains(&entry.owner) {
            problems.push(format!(
                "{}: is byte-exact now; remove it from the ledger",
                entry.owner
            ));
        } else if !semantic.contains(&entry.owner) {
            problems.push(format!(
                "{}: names no owner in semantic/regions.json",
                entry.owner
            ));
        }
        for axis in REQUIRED_AXES {
            if !entry.axes.iter().any(|a| a == axis) {
                problems.push(format!(
                    "{}: {axis} axis not exhausted; not sealable",
                    entry.owner
                ));
            }
        }
        if entry.floor < 1 {
            problems.push(format!(
                "{}: floor={}hw would mean exact",
                entry.owner, entry.floor
            ));
        }
    }
    problems
}

/// PORT NOTE: added, no TypeScript counterpart. `violations` compares every
/// entry against these two corpora; if either is empty the gate reports
/// "owner registers ok" while having verified nothing, and an empty `exact/` silently
/// disables the one check this tool exists for -- that a sealed owner has since
/// gone byte-exact. Scanning nothing is not passing.
///
/// Returns `None` when both corpora are populated.
pub fn corpus_guard(root: &Path, exact: &HashSet<String>, semantic: &HashSet<String>) -> Option<String> {
    // The set is the AUDITED owner boundaries now, not a directory of C. It was
    // briefly the deleted source tree, and the guard was switched off rather
    // than repointed -- which meant a scan that read nothing reported success.
    // 2,340 owners are audited, so an empty set here is a broken read.
    if semantic.is_empty() {
        return Some(format!(
            "{} lists no audited owners; the ledger would be checked against nothing",
            root.join("semantic/regions.json").display()
        ));
    }
    if exact.is_empty() {
        return Some(format!(
            "{} holds no .c owners; the went-exact check would never fire",
            root.join("exact").display()
        ));
    }
    None
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct QueueRow {
    pub owner: String,
    pub floor: i64,
}

/// Why a queue scan looked at nothing. A queue of zero owners produced by
/// reading zero records is not an empty queue, it is an unrun gate.
#[derive(Debug, Clone, PartialEq, Eq)]
pub enum QueueScan {
    /// `out/modesweep` does not exist.
    RootMissing(PathBuf),
    /// `out/modesweep` exists but holds no sweep directories at all.
    RootEmpty(PathBuf),
    /// Directories existed but none carried a readable `floor.json`.
    NoRecords(PathBuf),
    /// Records were read; `rows` may legitimately be empty.
    Scanned { records: usize, rows: Vec<QueueRow> },
}

/// Owners whose compiler axis is spent but whose shape axis has never run. These
/// are not sealed and must not be: they are the queue that feeds shape_sweep.
pub fn queue(root: &Path) -> QueueScan {
    let exact = stems(root, "exact");
    let sweep_root = root.join("out").join("modesweep");
    let Ok(read) = std::fs::read_dir(&sweep_root) else {
        return QueueScan::RootMissing(sweep_root);
    };
    // PORT NOTE: `readdirSync` order is platform-dependent; sorting makes the
    // scan order (and therefore tie ordering below) reproducible.
    let mut directories: Vec<String> = read
        .flatten()
        .map(|e| e.file_name().to_string_lossy().into_owned())
        .collect();
    if directories.is_empty() {
        return QueueScan::RootEmpty(sweep_root);
    }
    directories.sort();

    let mut records = 0usize;
    // BTreeMap so the final tie order is by owner rather than by scan order.
    let mut best: BTreeMap<String, i64> = BTreeMap::new();
    for directory in directories {
        let path = sweep_root.join(&directory).join("floor.json");
        let Ok(text) = std::fs::read_to_string(&path) else {
            continue;
        };
        let Ok(record) = serde_json::from_str::<serde_json::Value>(&text) else {
            continue;
        };
        records += 1;
        if record.get("bounded_search_complete") != Some(&serde_json::Value::Bool(true)) {
            continue;
        }
        if record.get("exact") == Some(&serde_json::Value::Bool(true)) {
            continue;
        }
        let Some(owner) = record.get("stem").and_then(|v| v.as_str()) else {
            continue;
        };
        // PORT NOTE: the TS accepts any JS number; a non-integral floor is
        // meaningless in halfwords, so it is rejected here rather than printed
        // with a fractional tail.
        let Some(floor) = record
            .get("irreducible_floor_halfwords")
            .and_then(|v| v.as_i64())
        else {
            continue;
        };
        if floor < 1 || exact.contains(owner) {
            continue;
        }
        let slot = best.entry(owner.to_string()).or_insert(floor);
        if floor < *slot {
            *slot = floor;
        }
    }

    if records == 0 {
        return QueueScan::NoRecords(sweep_root);
    }
    let mut rows: Vec<QueueRow> = best
        .into_iter()
        .map(|(owner, floor)| QueueRow { owner, floor })
        .collect();
    rows.sort_by_key(|r| r.floor);
    QueueScan::Scanned { records, rows }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn set(items: &[&str]) -> HashSet<String> {
        items.iter().map(|s| (*s).to_string()).collect()
    }


    // --- ported from the TypeScript self-test ---








    // --- edge cases the TypeScript self-test missed ---















    // --- filesystem-facing behaviour ---

    #[test]
    fn stems_of_a_missing_directory_is_empty() {
        let root = Path::new("/nonexistent-alchemy-root-for-tests");
        assert!(stems(root, "semantic").is_empty());
    }

    #[test]
    fn queue_reports_a_missing_root_rather_than_an_empty_queue() {
        let root = Path::new("/nonexistent-alchemy-root-for-tests");
        assert!(matches!(queue(root), QueueScan::RootMissing(_)));
    }

    #[test]
    fn an_empty_audited_corpus_is_not_a_pass() {
        let root = Path::new("/repo");
        let message = corpus_guard(root, &set(&["aa"]), &set(&[])).expect("must refuse");
        assert!(message.contains("/repo/semantic/regions.json"), "{message}");
        assert!(message.contains("checked against nothing"), "{message}");
    }

    #[test]
    fn an_empty_exact_corpus_is_not_a_pass() {
        let root = Path::new("/repo");
        let message = corpus_guard(root, &set(&[]), &set(&["aa"])).expect("must refuse");
        assert!(message.contains("/repo/exact"), "{message}");
        assert!(message.contains("never fire"), "{message}");
    }

    #[test]
    fn a_populated_pair_of_corpora_passes_the_guard() {
        assert_eq!(
            corpus_guard(Path::new("/repo"), &set(&["aa"]), &set(&["bb"])),
            None
        );
    }

    #[test]
    fn the_guard_fires_on_a_missing_repository() {
        // The exact shape of the silent failure: an empty ledger plus no corpus
        // yields zero violations, which without the guard reads as success.
        let root = Path::new("/nonexistent-alchemy-root-for-tests");
        let exact = stems(root, "exact");
        let semantic = stems(root, "semantic");
        assert!(violations(&[], &exact, &semantic).is_empty());
        assert!(corpus_guard(root, &exact, &semantic).is_some());
    }

}
