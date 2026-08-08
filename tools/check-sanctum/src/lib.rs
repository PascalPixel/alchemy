//! Gate for SANCTUM.md, the sealed-owner ledger.
//!
//! A ledger nobody checks decays into folklore. Two failures matter and both are
//! silent without this: an entry naming an owner that no longer exists, and an
//! entry for an owner that has since gone exact. The second is the dangerous
//! one -- it tells the next agent to skip work that is already done, or worse,
//! to leave a solved owner unadopted.
//!
//! Port of `tools/check/check_sanctum.ts`.

use std::collections::{BTreeMap, HashSet};
use std::path::{Path, PathBuf};

/// Axes that must both be spent before an owner may be sealed.
pub const REQUIRED_AXES: [&str; 2] = ["compiler", "shape"];

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct SealedEntry {
    pub owner: String,
    pub floor: u64,
    pub axes: Vec<String>,
    pub reason: String,
}

/// Hand-rolled equivalent of
/// `^- \`([A-Za-z0-9_]+)\` floor=(\d+)hw axes=([a-z,]+) — (.+)$`.
///
/// The TypeScript applies this to the trimmed line, so the caller trims first.
fn match_entry(line: &str) -> Option<SealedEntry> {
    let rest = line.strip_prefix("- `")?;
    let (owner, rest) = rest.split_once('`')?;
    if owner.is_empty() || !owner.chars().all(|c| c.is_ascii_alphanumeric() || c == '_') {
        return None;
    }
    let rest = rest.strip_prefix(" floor=")?;
    let digits_len = rest.chars().take_while(char::is_ascii_digit).count();
    if digits_len == 0 {
        return None;
    }
    let (digits, rest) = rest.split_at(digits_len);
    let rest = rest.strip_prefix("hw axes=")?;
    let axes_len = rest
        .chars()
        .take_while(|c| c.is_ascii_lowercase() || *c == ',')
        .count();
    if axes_len == 0 {
        return None;
    }
    let (axes, rest) = rest.split_at(axes_len);
    // ` — ` is an em dash flanked by spaces, exactly as the TS regex spells it.
    let reason = rest.strip_prefix(" \u{2014} ")?;
    // `.+` requires at least one character and never spans a newline.
    if reason.is_empty() || reason.contains('\n') {
        return None;
    }
    // PORT NOTE: JS `Number("…")` on an arbitrarily long digit run yields an
    // imprecise float; a floor that does not fit in u64 is rejected as
    // malformed instead, which is strictly safer for a gate.
    let floor: u64 = digits.parse().ok()?;
    Some(SealedEntry {
        owner: owner.to_string(),
        floor,
        axes: axes.split(',').map(str::to_string).collect(),
        reason: reason.to_string(),
    })
}

/// Only the `## Sealed` section is enforced. Prose elsewhere in the ledger names
/// owners freely -- the queue section lists dozens -- and parsing those as
/// entries would fail the gate on documentation.
pub fn parse_sealed(markdown: &str) -> Result<Vec<SealedEntry>, String> {
    let start = markdown
        .find("\n## Sealed")
        .ok_or_else(|| "SANCTUM.md has no '## Sealed' section".to_string())?;
    let rest = &markdown[start + 1..];
    // `rest.indexOf("\n## ", 1)`: search starts at offset 1 so the heading that
    // opens the section is not treated as the one that closes it.
    let section = match rest[1..].find("\n## ") {
        Some(offset) => &rest[..offset + 1],
        None => rest,
    };
    let mut entries = Vec::new();
    for line in section.split('\n') {
        if !line.starts_with("- ") {
            continue;
        }
        let trimmed = line.trim();
        match match_entry(trimmed) {
            Some(entry) => entries.push(entry),
            None => return Err(format!("malformed sanctum entry: {trimmed}")),
        }
    }
    Ok(entries)
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
    entries: &[SealedEntry],
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
            problems.push(format!("{}: names no owner under semantic/", entry.owner));
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
/// "sanctum ok" while having verified nothing, and an empty `exact/` silently
/// disables the one check this tool exists for -- that a sealed owner has since
/// gone byte-exact. Scanning nothing is not passing.
///
/// Returns `None` when both corpora are populated.
pub fn corpus_guard(root: &Path, exact: &HashSet<String>, semantic: &HashSet<String>) -> Option<String> {
    if semantic.is_empty() {
        return Some(format!(
            "{} holds no .c owners; the ledger would be checked against nothing",
            root.join("semantic").display()
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

    const GOOD: &str = "\n## Sealed\n\n- `08011568` floor=2hw axes=compiler,shape — scheduler tie the reference wins\n\n## Next\n";

    // --- ported from the TypeScript self-test ---

    #[test]
    fn reads_a_well_formed_entry() {
        let parsed = parse_sealed(GOOD).expect("well-formed ledger must parse");
        assert_eq!(parsed.len(), 1);
        assert_eq!(parsed[0].owner, "08011568");
        assert_eq!(parsed[0].floor, 2);
        assert_eq!(parsed[0].axes, vec!["compiler", "shape"]);
        assert_eq!(parsed[0].reason, "scheduler tie the reference wins");
    }

    #[test]
    fn does_not_read_beyond_its_own_section() {
        let text = format!("{GOOD}\n- `08099999` some prose bullet\n");
        assert_eq!(parse_sealed(&text).expect("must parse").len(), 1);
    }

    #[test]
    fn exact_owner_is_a_violation() {
        let entries = parse_sealed(GOOD).unwrap();
        let problems = violations(&entries, &set(&["08011568"]), &set(&[]));
        assert_eq!(
            problems,
            vec!["08011568: is byte-exact now; remove it from the ledger"]
        );
    }

    #[test]
    fn owner_with_no_semantic_source_is_a_violation() {
        let entries = parse_sealed(GOOD).unwrap();
        let problems = violations(&entries, &set(&[]), &set(&[]));
        assert_eq!(problems, vec!["08011568: names no owner under semantic/"]);
    }

    #[test]
    fn well_formed_sealed_owner_passes() {
        let entries = parse_sealed(GOOD).unwrap();
        assert!(violations(&entries, &set(&[]), &set(&["08011568"])).is_empty());
    }

    #[test]
    fn sealing_on_one_axis_is_refused() {
        let entries =
            parse_sealed("\n## Sealed\n\n- `08011568` floor=2hw axes=compiler — only flags tried\n")
                .unwrap();
        let problems = violations(&entries, &set(&[]), &set(&["08011568"]));
        assert_eq!(
            problems,
            vec!["08011568: shape axis not exhausted; not sealable"]
        );
    }

    #[test]
    fn malformed_entry_is_an_error() {
        let err = parse_sealed("\n## Sealed\n\n- `08011568` missing the rest\n").unwrap_err();
        assert_eq!(err, "malformed sanctum entry: - `08011568` missing the rest");
    }

    // --- edge cases the TypeScript self-test missed ---

    #[test]
    fn a_ledger_without_the_section_is_an_error() {
        let err = parse_sealed("# Sanctum\n\n## Other\n").unwrap_err();
        assert_eq!(err, "SANCTUM.md has no '## Sealed' section");
    }

    #[test]
    fn section_heading_at_offset_zero_is_not_found() {
        // `indexOf("\n## Sealed")` needs the leading newline, so a file that
        // opens with the heading has no section as far as the gate is concerned.
        assert!(parse_sealed("## Sealed\n\n- `08011568` floor=2hw axes=compiler,shape — x\n").is_err());
    }

    #[test]
    fn the_empty_section_parses_to_no_entries() {
        let entries = parse_sealed("\n## Sealed\n\n<!-- nothing qualifies -->\n\n## Next\n").unwrap();
        assert!(entries.is_empty());
        assert!(violations(&entries, &set(&[]), &set(&[])).is_empty());
    }

    #[test]
    fn duplicate_owners_are_reported_once_each_pass() {
        let text = "\n## Sealed\n\n- `aa` floor=1hw axes=compiler,shape — a\n- `aa` floor=3hw axes=compiler,shape — b\n";
        let entries = parse_sealed(text).unwrap();
        assert_eq!(entries.len(), 2);
        let problems = violations(&entries, &set(&[]), &set(&["aa"]));
        assert_eq!(problems, vec!["aa: listed twice"]);
    }

    #[test]
    fn floor_zero_would_mean_exact() {
        let entries =
            parse_sealed("\n## Sealed\n\n- `aa` floor=0hw axes=compiler,shape — nothing left\n")
                .unwrap();
        let problems = violations(&entries, &set(&[]), &set(&["aa"]));
        assert_eq!(problems, vec!["aa: floor=0hw would mean exact"]);
    }

    #[test]
    fn missing_both_axes_reports_both() {
        let entries =
            parse_sealed("\n## Sealed\n\n- `aa` floor=2hw axes=annealer — stochastic only\n")
                .unwrap();
        let problems = violations(&entries, &set(&[]), &set(&["aa"]));
        assert_eq!(
            problems,
            vec![
                "aa: compiler axis not exhausted; not sealable",
                "aa: shape axis not exhausted; not sealable",
            ]
        );
    }

    #[test]
    fn axis_match_is_exact_not_substring() {
        // "compilerish" must not satisfy the "compiler" requirement. The TS uses
        // Array.includes on split values, so this holds there too; assert it.
        let entries = parse_sealed(
            "\n## Sealed\n\n- `aa` floor=2hw axes=compilerish,shapely — near misses\n",
        )
        .unwrap();
        let problems = violations(&entries, &set(&[]), &set(&["aa"]));
        assert_eq!(problems.len(), 2);
    }

    #[test]
    fn exact_takes_precedence_over_missing_semantic() {
        let entries = parse_sealed(GOOD).unwrap();
        let problems = violations(&entries, &set(&["08011568"]), &set(&["08011568"]));
        assert_eq!(
            problems,
            vec!["08011568: is byte-exact now; remove it from the ledger"]
        );
    }

    #[test]
    fn indented_bullets_are_not_entries() {
        // `startsWith("- ")` is applied to the untrimmed line, so an indented
        // continuation bullet is skipped rather than parsed.
        let entries =
            parse_sealed("\n## Sealed\n\n  - `aa` some nested prose bullet\n\n## Next\n").unwrap();
        assert!(entries.is_empty());
    }

    #[test]
    fn an_ascii_hyphen_separator_is_malformed() {
        // The format demands an em dash; a hyphen must not sneak through.
        assert!(parse_sealed("\n## Sealed\n\n- `aa` floor=2hw axes=compiler,shape - reason\n").is_err());
    }

    #[test]
    fn an_empty_reason_is_malformed() {
        assert!(parse_sealed("\n## Sealed\n\n- `aa` floor=2hw axes=compiler,shape — \n").is_err());
    }

    #[test]
    fn uppercase_axes_are_malformed() {
        assert!(
            parse_sealed("\n## Sealed\n\n- `aa` floor=2hw axes=Compiler,shape — reason\n").is_err()
        );
    }

    #[test]
    fn a_hyphenated_owner_is_malformed() {
        assert!(parse_sealed(
            "\n## Sealed\n\n- `08011568-x` floor=2hw axes=compiler,shape — reason\n"
        )
        .is_err());
    }

    #[test]
    fn trailing_section_without_a_closing_heading_still_parses() {
        let entries =
            parse_sealed("\n## Sealed\n\n- `aa` floor=9hw axes=compiler,shape — last line\n")
                .unwrap();
        assert_eq!(entries.len(), 1);
        assert_eq!(entries[0].floor, 9);
    }

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
    fn an_empty_semantic_corpus_is_not_a_pass() {
        let root = Path::new("/repo");
        let message = corpus_guard(root, &set(&["aa"]), &set(&[])).expect("must refuse");
        assert!(message.contains("/repo/semantic"), "{message}");
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

    #[test]
    fn the_real_ledger_parses_and_passes() {
        let root = Path::new(env!("CARGO_MANIFEST_DIR"))
            .parent()
            .unwrap()
            .parent()
            .unwrap();
        let text = std::fs::read_to_string(root.join("SANCTUM.md")).expect("SANCTUM.md must exist");
        let entries = parse_sealed(&text).expect("the real ledger must parse");
        let semantic = stems(root, "semantic");
        let exact = stems(root, "exact");
        assert!(!semantic.is_empty(), "semantic/ must hold owners");
        assert!(
            violations(&entries, &exact, &semantic).is_empty(),
            "the committed ledger must be clean"
        );
    }
}
