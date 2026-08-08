// Non-destructive Full-C history ledger, ported from tools/metrics/full_c_history.ts.
//
// WHY THIS FILE EXISTS: the ledger walks every first-parent commit, so it runs
// `git ls-tree` and `git cat-file` thousands of times and re-scans every tracked
// C file in every historical tree. That is the whole cost of the tool, and it is
// exactly the kind of work the TypeScript version pays an interpreter tax on.
// The measurement rules themselves are unchanged: commit objects are never
// rewritten, each first-parent tree is measured from its tracked C ownership
// against the current audited fixed executable denominator, and commit subjects
// are omitted because this is a numeric progress ledger, not a copy of the
// repository's messages.
//
// PORT NOTE: the TypeScript entry point throws on failure, so `bun` prints a
// JavaScript stack trace and exits 1. This binary prints `error: <message>` on
// stderr and exits 1. The message text matches; the trace does not.

mod js;
mod json;

use std::collections::HashMap;
use std::fmt::Write as _;
use std::path::{Path, PathBuf};
use std::process::Command;

/// Insertion-ordered string→i64 map.
///
/// PORT NOTE: JS `Map` iteration is insertion order and the ledger's diagnostic
/// strings (`removed`, `spans shrunk in place`) are built from that order, so a
/// `HashMap` would silently reorder the ledger's prose. Only the blob cache,
/// which is never iterated, uses a `HashMap`.
#[derive(Default, Clone)]
struct OrderedMap {
    entries: Vec<(String, i64)>,
    index: HashMap<String, usize>,
}

impl OrderedMap {
    fn set(&mut self, key: &str, value: i64) {
        match self.index.get(key) {
            Some(&slot) => self.entries[slot].1 = value,
            None => {
                self.index.insert(key.to_string(), self.entries.len());
                self.entries.push((key.to_string(), value));
            }
        }
    }

    fn get(&self, key: &str) -> Option<i64> {
        self.index.get(key).map(|&slot| self.entries[slot].1)
    }

    fn has(&self, key: &str) -> bool {
        self.index.contains_key(key)
    }

    fn iter(&self) -> impl Iterator<Item = &(String, i64)> {
        self.entries.iter()
    }
}

#[derive(Clone, Copy, PartialEq, Eq)]
struct RegionSize {
    address: i64,
    size: i64,
}

struct TreeEntry {
    oid: String,
    path: String,
}

struct HistoryEntry {
    commit: String,
    first_parent_position: i64,
    author_time: String,
    committer_time: String,
    full_c_bytes: i64,
    executable_bytes: i64,
    remaining_bytes: i64,
    percent: f64,
    main_full_c_bytes: i64,
    overlay_full_c_bytes: i64,
    canonical_suffix: String,
    evidence: Vec<String>,
    correction: Option<String>,
}

fn repo_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("crate lives at <root>/tools-rs/<crate>")
        .to_path_buf()
}

fn git(root: &Path, args: &[&str]) -> Result<String, String> {
    let output = Command::new("git")
        .args(args)
        .current_dir(root)
        .output()
        .map_err(|error| format!("git {}: {error}", args.join(" ")))?;
    if !output.status.success() {
        let stderr = String::from_utf8_lossy(&output.stderr).trim().to_string();
        return Err(if stderr.is_empty() {
            format!("git {} failed", args.join(" "))
        } else {
            stderr
        });
    }
    // PORT NOTE: readFileSync/Buffer#toString("utf8") is lossy, so is this.
    Ok(String::from_utf8_lossy(&output.stdout).into_owned())
}

fn read_json(path: &Path) -> Result<json::Value, String> {
    let bytes = std::fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?;
    json::parse(&String::from_utf8_lossy(&bytes))
        .map_err(|error| format!("{}: {error}", path.display()))
}

// ---------------------------------------------------------------------------
// metric helpers, mirrored from tools/lib/full_c_progress.ts

fn integer(value: f64, label: &str) -> Result<i64, String> {
    // Number.isSafeInteger: one number type, so 1.0 is an integer here too.
    if !value.is_finite() || value.fract() != 0.0 || value.abs() > 9_007_199_254_740_991.0 || value < 0.0
    {
        return Err(format!("{label} must be a non-negative safe integer"));
    }
    Ok(value as i64)
}

/// The commit marker is rendered in whole kilobytes (floor of bytes/1000).
fn kilobytes(value: i64) -> Result<i64, String> {
    Ok(integer(value as f64, "byte count")?.div_euclid(1000))
}

fn format_subject(full_c_bytes: i64, executable_bytes: i64) -> Result<String, String> {
    if full_c_bytes > executable_bytes {
        return Err("Full-C numerator exceeds executable denominator".to_string());
    }
    Ok(format!(
        "[ ☀️ {} / {} ]",
        js::commas(kilobytes(full_c_bytes)?),
        js::commas(kilobytes(executable_bytes)?)
    ))
}

fn round_half_up_percent(numerator: i64, denominator: i64) -> Result<f64, String> {
    if denominator <= 0 {
        return Err("executable denominator must be positive".to_string());
    }
    let numerator = numerator as f64;
    let denominator = denominator as f64;
    Ok(((numerator * 10000.0 + denominator / 2.0) / denominator).floor() / 100.0)
}

// ---------------------------------------------------------------------------

fn region_map(root: &Path) -> Result<HashMap<String, RegionSize>, String> {
    let mut result: HashMap<String, RegionSize> = HashMap::new();
    for relative in ["out/full/asm/manifest.json", "out/full/claimed/manifest.json"] {
        let document = read_json(&root.join(relative))?;
        let rows = document
            .get("regions")
            .and_then(json::Value::as_array)
            .unwrap_or(&[])
            .to_vec();
        for row in rows {
            let source = row.get("source").and_then(json::Value::as_str).unwrap_or("undefined");
            let Some(stem) = js::region_stem(source) else { continue };
            let value = RegionSize {
                address: row.get("address").and_then(json::Value::as_f64).unwrap_or(f64::NAN) as i64,
                size: row.get("size").and_then(json::Value::as_f64).unwrap_or(f64::NAN) as i64,
            };
            if let Some(previous) = result.get(&stem) {
                if *previous != value {
                    return Err(format!("conflicting region boundary for {stem}"));
                }
            }
            result.insert(stem, value);
        }
    }
    Ok(result)
}

struct Commit {
    commit: String,
    author: String,
    committer: String,
}

fn commits(root: &Path) -> Result<Vec<Commit>, String> {
    let format = "--format=%H%x1f%aI%x1f%cI%x1e";
    let log = git(root, &["log", "--first-parent", "--reverse", format])?;
    Ok(log
        .split('\u{1e}')
        .map(str::trim)
        .filter(|record| !record.is_empty())
        .map(|record| {
            let mut fields = record.split('\u{1f}');
            Commit {
                commit: fields.next().unwrap_or_default().to_string(),
                author: fields.next().unwrap_or_default().to_string(),
                committer: fields.next().unwrap_or_default().to_string(),
            }
        })
        .collect())
}

fn tree(root: &Path, commit: &str) -> Result<Vec<TreeEntry>, String> {
    let listing = git(root, &["ls-tree", "-r", commit])?;
    let mut entries = Vec::new();
    for line in listing.split('\n') {
        let line = line.strip_suffix('\r').unwrap_or(line);
        if line.is_empty() {
            continue;
        }
        let (oid, path) =
            js::ls_tree_row(line).ok_or_else(|| format!("cannot parse ls-tree row: {line}"))?;
        entries.push(TreeEntry { oid, path });
    }
    Ok(entries)
}

#[derive(Default)]
struct BlobCache {
    cache: HashMap<String, String>,
}

impl BlobCache {
    fn get(&mut self, root: &Path, oid: &str) -> Result<&str, String> {
        if !self.cache.contains_key(oid) {
            let value = git(root, &["cat-file", "blob", oid])?;
            self.cache.insert(oid.to_string(), value);
        }
        Ok(&self.cache[oid])
    }
}

/// `overlayPlaceholders`: sum the `.space` spans under each `AlchemyC_` label.
fn overlay_placeholders(source: &str) -> Vec<(String, i64)> {
    let mut result: Vec<(String, i64)> = Vec::new();
    let mut owner = String::new();
    let mut in_placeholder = false;
    for line in split_lines(source) {
        if let Some(label) = js::alchemy_c_label(line) {
            owner = label;
            in_placeholder = true;
            continue;
        }
        if in_placeholder && (js::blank_line(line) || js::local_label(line)) {
            continue;
        }
        if in_placeholder {
            if let Some(size) = js::space_directive(line) {
                match result.iter_mut().find(|(key, _)| *key == owner) {
                    Some(slot) => slot.1 += size,
                    None => result.push((owner.clone(), size)),
                }
                continue;
            }
        }
        if !line.trim_matches(js::is_js_space).is_empty() {
            in_placeholder = false;
        }
    }
    result
}

/// `source.split(/\r?\n/)`
fn split_lines(source: &str) -> impl Iterator<Item = &str> {
    source.split('\n').map(|line| line.strip_suffix('\r').unwrap_or(line))
}

struct Measured {
    main: i64,
    overlays: i64,
    accepted: OrderedMap,
    excluded: Vec<String>,
}

fn measured_tree(
    root: &Path,
    blobs: &mut BlobCache,
    entries: &[TreeEntry],
    sizes: &HashMap<String, RegionSize>,
) -> Result<Measured, String> {
    let mut main = 0i64;
    let mut accepted = OrderedMap::default();
    let mut excluded: Vec<String> = Vec::new();
    let mut paths: HashMap<&str, &TreeEntry> = HashMap::new();
    for entry in entries {
        paths.insert(entry.path.as_str(), entry);
    }

    for entry in entries {
        // src/ is the pre-consolidation layout; exact/ is where the tree
        // consolidation moved the main image's canonical exact C.
        let Some(stem) = js::main_c_path(&entry.path) else { continue };
        let region = sizes.get(&stem).copied();
        let acceptable = js::acceptable_historical_c(blobs.get(root, &entry.oid)?);
        if !acceptable {
            excluded.push(format!("{}:noncanonical-C", entry.path));
            continue;
        }
        let Some(region) = region else {
            excluded.push(format!("{}:no-audited-region", entry.path));
            continue;
        };
        main += region.size;
        accepted.set(&entry.path, region.size);
    }

    let mut overlays = 0i64;
    for entry in entries {
        let Some(stem) = js::overlay_container(&entry.path) else { continue };
        // Overlay exact C moved from assets/code/ to exact/ in the tree
        // consolidation; measure whichever layout the commit's tree carries.
        let prefixes = [format!("assets/code/{stem}_c_"), format!("exact/{stem}_c_")];
        let mut c_paths_by_address: Vec<(String, String)> = Vec::new();
        for candidate in entries {
            if !prefixes.iter().any(|prefix| candidate.path.starts_with(prefix.as_str())) {
                continue;
            }
            if let Some(address) = js::overlay_c_address(&candidate.path) {
                match c_paths_by_address.iter_mut().find(|(key, _)| *key == address) {
                    Some(slot) => slot.1 = candidate.path.clone(),
                    None => c_paths_by_address.push((address, candidate.path.clone())),
                }
            }
        }
        let placeholders = overlay_placeholders(blobs.get(root, &entry.oid)?);
        for (address, c_path) in &c_paths_by_address {
            let Some(&(_, size)) = placeholders.iter().find(|(key, _)| key == address) else {
                excluded.push(format!("{c_path}:no-placeholder"));
                continue;
            };
            let acceptable = match paths.get(c_path.as_str()) {
                Some(c_entry) => js::acceptable_historical_c(blobs.get(root, &c_entry.oid)?),
                None => false,
            };
            if !acceptable {
                excluded.push(format!("{c_path}:noncanonical-C"));
                continue;
            }
            overlays += size;
            accepted.set(c_path, size);
        }
    }

    Ok(Measured { main, overlays, accepted, excluded })
}

struct Ledger {
    denominator_commit: String,
    executable_bytes: i64,
    generated_from: String,
    entries: Vec<HistoryEntry>,
}

fn ledger(root: &Path) -> Result<Ledger, String> {
    let report = read_json(&root.join("metrics/gs1-en-progress.json"))?;
    if report.get("audit").and_then(json::Value::as_str) != Some("complete")
        || report.get("metric").and_then(json::Value::as_str) != Some("full-c-byte-share")
    {
        return Err("current audited Full-C report is required".to_string());
    }
    let denominator = report
        .get("executable_bytes")
        .and_then(json::Value::as_f64)
        .ok_or("executable_bytes is missing")? as i64;
    let sizes = region_map(root)?;
    let history = commits(root)?;
    let mut blobs = BlobCache::default();
    let mut rows: Vec<HistoryEntry> = Vec::new();
    let mut previous = 0i64;
    let mut previous_accepted = OrderedMap::default();

    for (index, commit) in history.iter().enumerate() {
        let measured = measured_tree(root, &mut blobs, &tree(root, &commit.commit)?, &sizes)?;
        let full_c = measured.main + measured.overlays;
        if full_c > denominator {
            return Err(format!("{}: numerator exceeds denominator", commit.commit));
        }
        let removed: Vec<&str> = previous_accepted
            .iter()
            .map(|(path, _)| path.as_str())
            .filter(|path| !measured.accepted.has(path))
            .collect();
        // A decrease can also happen with every path still present: an owner's
        // AlchemyC placeholder span shrinking in place (an adoption-span
        // correction, e.g. alignment padding no longer claimed). Name those
        // paths so a decrease is only fatal when nothing accounts for it.
        let shrunk: Vec<String> = measured
            .accepted
            .iter()
            .filter(|(path, size)| previous_accepted.get(path).unwrap_or(*size) > *size)
            .map(|(path, size)| {
                format!("{path}:-{}B", previous_accepted.get(path).unwrap_or(*size) - size)
            })
            .collect();
        let correction = if full_c < previous {
            let mut text = format!(
                "measured C ownership decreased by {} bytes; removed/reclassified paths: {}",
                previous - full_c,
                if removed.is_empty() { "(none)".to_string() } else { removed.join(", ") }
            );
            if !shrunk.is_empty() {
                let _ = write!(text, "; spans shrunk in place: {}", shrunk.join(", "));
            }
            Some(text)
        } else {
            None
        };
        if full_c < previous && removed.is_empty() && shrunk.is_empty() {
            return Err(format!("{}: unexplained Full-C regression", commit.commit));
        }
        let mut evidence = vec![
            "commit tree src/<address>.c ownership mapped to audited main executable regions"
                .to_string(),
            "commit tree overlay C files mapped to same-tree AlchemyC placeholder spans".to_string(),
            "noncanonical register-pinned/inline-assembly/fakematch C excluded".to_string(),
        ];
        if !measured.excluded.is_empty() {
            evidence.push(format!("excluded={}", measured.excluded.join(";")));
        }
        rows.push(HistoryEntry {
            commit: commit.commit.clone(),
            first_parent_position: index as i64 + 1,
            author_time: commit.author.clone(),
            committer_time: commit.committer.clone(),
            full_c_bytes: full_c,
            executable_bytes: denominator,
            remaining_bytes: denominator - full_c,
            percent: round_half_up_percent(full_c, denominator)?,
            main_full_c_bytes: measured.main,
            overlay_full_c_bytes: measured.overlays,
            canonical_suffix: format_subject(full_c, denominator)?,
            evidence,
            correction,
        });
        previous = full_c;
        previous_accepted = measured.accepted;
    }

    Ok(Ledger {
        denominator_commit: git(root, &["rev-parse", "HEAD"])?.trim().to_string(),
        executable_bytes: denominator,
        generated_from: git(root, &["rev-parse", "HEAD^{tree}"])?.trim().to_string(),
        entries: rows,
    })
}

/// The `canonical_json` layout for this document's fixed shape: one key per
/// line for objects, arrays of primitives inline on one line.
fn canonical_ledger_json(output: &Ledger) -> String {
    let mut text = String::new();
    text.push_str("{\n");
    text.push_str("  \"format\": 1,\n");
    text.push_str("  \"metric\": \"full-c-byte-share\",\n");
    text.push_str("  \"target\": \"gs1-en\",\n");
    let _ = writeln!(
        text,
        "  \"denominator_commit\": {},",
        json::quote(&output.denominator_commit)
    );
    let _ = writeln!(text, "  \"executable_bytes\": {},", output.executable_bytes);
    text.push_str("  \"history_scope\": \"first-parent\",\n");
    let _ = writeln!(text, "  \"generated_from\": {},", json::quote(&output.generated_from));
    if output.entries.is_empty() {
        text.push_str("  \"entries\": []\n}");
        return text;
    }
    text.push_str("  \"entries\": [\n");
    let mut records: Vec<String> = Vec::with_capacity(output.entries.len());
    for entry in &output.entries {
        let mut record = String::from("    {\n");
        let _ = writeln!(record, "      \"commit\": {},", json::quote(&entry.commit));
        let _ = writeln!(
            record,
            "      \"first_parent_position\": {},",
            entry.first_parent_position
        );
        let _ = writeln!(record, "      \"author_time\": {},", json::quote(&entry.author_time));
        let _ = writeln!(
            record,
            "      \"committer_time\": {},",
            json::quote(&entry.committer_time)
        );
        let _ = writeln!(record, "      \"full_c_bytes\": {},", entry.full_c_bytes);
        let _ = writeln!(record, "      \"executable_bytes\": {},", entry.executable_bytes);
        let _ = writeln!(record, "      \"remaining_bytes\": {},", entry.remaining_bytes);
        let _ = writeln!(record, "      \"percent\": {},", json::number(entry.percent));
        let _ = writeln!(record, "      \"main_full_c_bytes\": {},", entry.main_full_c_bytes);
        let _ = writeln!(
            record,
            "      \"overlay_full_c_bytes\": {},",
            entry.overlay_full_c_bytes
        );
        let _ = writeln!(
            record,
            "      \"canonical_suffix\": {},",
            json::quote(&entry.canonical_suffix)
        );
        record.push_str("      \"derivation_status\": \"measured\",\n");
        let inline = entry
            .evidence
            .iter()
            .map(|item| json::quote(item))
            .collect::<Vec<_>>()
            .join(", ");
        let _ = write!(record, "      \"evidence\": [{inline}]");
        if let Some(correction) = &entry.correction {
            let _ = write!(record, ",\n      \"correction\": {}", json::quote(correction));
        }
        record.push_str("\n    }");
        records.push(record);
    }
    text.push_str(&records.join(",\n"));
    text.push_str("\n  ]\n}");
    text
}

const COLUMNS: [&str; 13] = [
    "commit",
    "first_parent_position",
    "author_time",
    "committer_time",
    "full_c_bytes",
    "executable_bytes",
    "remaining_bytes",
    "percent",
    "main_full_c_bytes",
    "overlay_full_c_bytes",
    "canonical_suffix",
    "derivation_status",
    "correction",
];

fn csv_column(entry: &HistoryEntry, column: &str) -> String {
    match column {
        "commit" => entry.commit.clone(),
        "first_parent_position" => entry.first_parent_position.to_string(),
        "author_time" => entry.author_time.clone(),
        "committer_time" => entry.committer_time.clone(),
        "full_c_bytes" => entry.full_c_bytes.to_string(),
        "executable_bytes" => entry.executable_bytes.to_string(),
        "remaining_bytes" => entry.remaining_bytes.to_string(),
        "percent" => json::number(entry.percent),
        "main_full_c_bytes" => entry.main_full_c_bytes.to_string(),
        "overlay_full_c_bytes" => entry.overlay_full_c_bytes.to_string(),
        "canonical_suffix" => entry.canonical_suffix.clone(),
        "derivation_status" => "measured".to_string(),
        // `entry[column] ?? ""`: an absent correction becomes an empty cell.
        "correction" => entry.correction.clone().unwrap_or_default(),
        other => unreachable!("unknown ledger column {other}"),
    }
}

fn ledger_csv(output: &Ledger) -> String {
    let mut lines = vec![COLUMNS.join(",")];
    for entry in &output.entries {
        lines.push(
            COLUMNS
                .iter()
                .map(|column| js::csv_cell(&csv_column(entry, column)))
                .collect::<Vec<_>>()
                .join(","),
        );
    }
    lines.join("\n") + "\n"
}

fn write_ledger(root: &Path) -> Result<(), String> {
    let output = ledger(root)?;
    // PORT NOTE: the TypeScript would happily write an empty ledger and report
    // `history=0`, which is a generator that passes without measuring anything.
    // Refuse instead. `git log` over a repository with commits always yields at
    // least one first-parent record, so this cannot fire on a real checkout and
    // the measured parity run is unaffected.
    if output.entries.is_empty() {
        return Err("no first-parent history was measured".to_string());
    }
    std::fs::write(root.join("docs/full-c-history.json"), canonical_ledger_json(&output))
        .map_err(|error| error.to_string())?;
    std::fs::write(root.join("docs/full-c-history.csv"), ledger_csv(&output))
        .map_err(|error| error.to_string())?;
    let regressions = output.entries.iter().filter(|entry| entry.correction.is_some()).count();
    println!(
        "history={} measured={} unmeasured=0 corrections={regressions} denominator={}",
        output.entries.len(),
        output.entries.len(),
        output.executable_bytes
    );
    Ok(())
}

fn self_test() -> Result<(), String> {
    let placeholders =
        overlay_placeholders("AlchemyC_02000010:\n\t.space 2\n.L_02000012:\n\t.space 4\n\tbx lr\n");
    if placeholders.iter().find(|(key, _)| key == "02000010").map(|(_, size)| *size) != Some(6) {
        return Err("overlay placeholder adapter failed".to_string());
    }
    if js::acceptable_historical_c("register int x asm(\"r4\");") {
        return Err("register pin accepted".to_string());
    }
    if !js::acceptable_historical_c("int f(void) { return 1; }") {
        return Err("ordinary C rejected".to_string());
    }
    println!("self-test=ok history=full-c-byte-share");
    Ok(())
}

fn run() -> Result<(), String> {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    let root = repo_root();
    match arguments.first().map(String::as_str) {
        Some("--self-test") => self_test(),
        None | Some("--write") => write_ledger(&root),
        _ => Err("usage: full_c_history.ts [--write|--self-test]".to_string()),
    }
}

fn main() {
    if let Err(error) = run() {
        eprintln!("error: {error}");
        std::process::exit(1);
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn entry(oid: &str, path: &str) -> TreeEntry {
        TreeEntry { oid: oid.to_string(), path: path.to_string() }
    }

    #[test]
    fn placeholder_spans_accumulate_until_a_real_instruction() {
        let spans = overlay_placeholders(
            "AlchemyC_02000010:\n\t.space 2\n.L_02000012:\n\t.space 4\n\tbx lr\n\t.space 8\n\
             AlchemyC_02000020:\n\t.space 0x10\n",
        );
        assert_eq!(
            spans,
            vec![("02000010".to_string(), 6), ("02000020".to_string(), 16)]
        );
    }

    #[test]
    fn percent_rounds_half_up_and_subject_uses_floored_kilobytes() {
        assert_eq!(round_half_up_percent(1, 8).unwrap(), 12.5);
        assert_eq!(round_half_up_percent(317066, 1345890).unwrap(), 23.56);
        assert_eq!(format_subject(123456, 1234567).unwrap(), "[ ☀️ 123 / 1,234 ]");
        assert!(format_subject(2, 1).is_err());
        assert!(round_half_up_percent(1, 0).is_err());
    }

    #[test]
    fn integer_accepts_whole_floats_like_number_is_safe_integer() {
        assert_eq!(integer(1.0, "x").unwrap(), 1);
        assert!(integer(1.5, "x").is_err());
        assert!(integer(-1.0, "x").is_err());
    }

    #[test]
    fn ordered_map_preserves_insertion_order_through_updates() {
        let mut map = OrderedMap::default();
        map.set("z", 1);
        map.set("a", 2);
        map.set("z", 3);
        assert_eq!(
            map.iter().map(|(key, value)| (key.as_str(), *value)).collect::<Vec<_>>(),
            vec![("z", 3), ("a", 2)]
        );
    }

    /// Builds a fake tree out of an in-memory blob cache so the measurement
    /// rules are exercised without touching the repository's real history.
    fn fixture() -> (BlobCache, Vec<TreeEntry>, HashMap<String, RegionSize>) {
        let mut blobs = BlobCache::default();
        blobs.cache.insert("aa".into(), "int f(void) { return 1; }\n".into());
        blobs.cache.insert("bb".into(), "register int x asm(\"r4\");\n".into());
        blobs.cache.insert("cc".into(), "int g(void) { return 2; }\n".into());
        blobs.cache.insert("dd".into(), "int h(void) { return 3; }\n".into());
        blobs.cache.insert(
            "ee".into(),
            "AlchemyC_02000010:\n\t.space 0x20\n\tbx lr\nAlchemyC_02000040:\n\t.space 4\n".into(),
        );
        let entries = vec![
            entry("aa", "src/08000100.c"),
            entry("bb", "exact/08000200.c"),
            entry("cc", "src/08000300.c"),
            entry("ee", "assets/code/kind1_overlay.s"),
            entry("dd", "exact/kind1_c_02000010.c"),
            entry("dd", "exact/kind1_c_02000099.c"),
        ];
        let mut sizes = HashMap::new();
        sizes.insert("08000100".to_string(), RegionSize { address: 0x0800_0100, size: 64 });
        sizes.insert("08000200".to_string(), RegionSize { address: 0x0800_0200, size: 16 });
        (blobs, entries, sizes)
    }

    #[test]
    fn measures_main_and_overlay_ownership_and_names_every_exclusion() {
        let (mut blobs, entries, sizes) = fixture();
        let root = repo_root();
        let measured = measured_tree(&root, &mut blobs, &entries, &sizes).unwrap();
        assert_eq!(measured.main, 64);
        assert_eq!(measured.overlays, 32);
        assert_eq!(
            measured.excluded,
            vec![
                "exact/08000200.c:noncanonical-C".to_string(),
                "src/08000300.c:no-audited-region".to_string(),
                "exact/kind1_c_02000099.c:no-placeholder".to_string(),
            ]
        );
        assert_eq!(
            measured
                .accepted
                .iter()
                .map(|(path, size)| (path.as_str(), *size))
                .collect::<Vec<_>>(),
            vec![("src/08000100.c", 64), ("exact/kind1_c_02000010.c", 32)]
        );
    }

    #[test]
    fn noncanonical_c_is_checked_before_the_region_lookup() {
        // exact/08000200.c has no audited region either, but the C check runs
        // first in the TypeScript, so the reason must be noncanonical-C.
        let (mut blobs, entries, mut sizes) = fixture();
        sizes.remove("08000200");
        let measured = measured_tree(&repo_root(), &mut blobs, &entries, &sizes).unwrap();
        assert!(measured.excluded.contains(&"exact/08000200.c:noncanonical-C".to_string()));
    }

    fn sample_ledger() -> Ledger {
        Ledger {
            denominator_commit: "c0ffee".to_string(),
            executable_bytes: 1_345_890,
            generated_from: "deadbeef".to_string(),
            entries: vec![
                HistoryEntry {
                    commit: "aaa".to_string(),
                    first_parent_position: 1,
                    author_time: "2026-07-16T14:49:11+01:00".to_string(),
                    committer_time: "2026-07-16T14:49:11+01:00".to_string(),
                    full_c_bytes: 0,
                    executable_bytes: 1_345_890,
                    remaining_bytes: 1_345_890,
                    percent: 0.0,
                    main_full_c_bytes: 0,
                    overlay_full_c_bytes: 0,
                    canonical_suffix: format_subject(0, 1_345_890).unwrap(),
                    evidence: vec!["one".to_string(), "two".to_string()],
                    correction: None,
                },
                HistoryEntry {
                    commit: "bbb".to_string(),
                    first_parent_position: 2,
                    author_time: "2026-07-17T14:49:11+01:00".to_string(),
                    committer_time: "2026-07-17T14:49:11+01:00".to_string(),
                    full_c_bytes: 317_066,
                    executable_bytes: 1_345_890,
                    remaining_bytes: 1_028_824,
                    percent: round_half_up_percent(317_066, 1_345_890).unwrap(),
                    main_full_c_bytes: 109_020,
                    overlay_full_c_bytes: 208_046,
                    canonical_suffix: format_subject(317_066, 1_345_890).unwrap(),
                    evidence: vec!["one".to_string()],
                    correction: Some("dropped src/a.c, src/b.c".to_string()),
                },
            ],
        }
    }

    #[test]
    fn canonical_json_puts_one_key_per_line_and_inlines_primitive_arrays() {
        let text = canonical_ledger_json(&sample_ledger());
        assert!(text.starts_with("{\n  \"format\": 1,\n  \"metric\": \"full-c-byte-share\",\n"));
        assert!(text.contains("\n  \"entries\": [\n    {\n      \"commit\": \"aaa\","));
        assert!(text.contains("\n      \"evidence\": [\"one\", \"two\"]\n    },\n"));
        assert!(text.contains("\n      \"percent\": 23.56,\n"));
        assert!(text.contains("\n      \"correction\": \"dropped src/a.c, src/b.c\"\n    }\n  ]\n}"));
        assert!(!text.ends_with('\n'));
        // The subject's emoji survives unescaped, as JSON.stringify leaves it.
        assert!(text.contains("☀️"));
    }

    #[test]
    fn csv_quotes_only_cells_that_need_it_and_ends_with_a_newline() {
        let text = ledger_csv(&sample_ledger());
        let lines: Vec<&str> = text.trim_end_matches('\n').split('\n').collect();
        assert_eq!(lines[0], COLUMNS.join(","));
        assert!(lines[1].ends_with(",\"[ ☀️ 0 / 1,345 ]\",measured,"));
        assert!(lines[2].contains(",23.56,"));
        assert!(lines[2].ends_with(",measured,\"dropped src/a.c, src/b.c\""));
        assert!(text.ends_with("\n"));
    }
}
