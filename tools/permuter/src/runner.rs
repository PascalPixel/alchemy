use std::cmp::Ordering;
use std::collections::{BTreeMap, BTreeSet};
use std::fs::{self, File};
use std::io::{self, Write};
use std::path::{Path, PathBuf};
use std::sync::atomic::{AtomicBool, AtomicUsize, Ordering as AtomicOrdering};
use std::sync::{mpsc, Arc, Mutex};
use std::time::{Duration, Instant};

use compiler_core::routing::root;
use serde_json::json;

use crate::backend::{self, Backend, Measurement};
use crate::options::{
    Options, MAX_ITERATIONS, MAX_JOURNAL_ROW_BYTES, MAX_OUTPUT_BYTES, MAX_PLAN_BYTES,
    MAX_SOURCE_BYTES, MAX_SUMMARY_BYTES,
};
use crate::perm::Permutation;
use crate::randomize::Weights;

#[derive(Clone, Debug, Eq, PartialEq)]
struct Candidate {
    index: usize,
    manual_seed: usize,
    mutation: String,
    source: String,
    fingerprint: String,
}

#[derive(Debug)]
struct Evaluated {
    candidate: Candidate,
    measurement: Result<Measurement, String>,
    elapsed: Duration,
}

const RUN_MARKER: &str = ".permuter-run";
const ACTIVE_MARKER: &str = ".permuter-active";
const OWNERSHIP_MANIFEST: &str = ".permuter-owned";

// Run directories are ordinary files under one of two trusted roots. The
// path is validated before creation, and every file written by the runner is
// registered in the ownership manifest.
struct RunDirectory {
    path: PathBuf,
    owned: BTreeSet<String>,
    _active: File,
}

impl RunDirectory {
    fn claim(output: &Path, identity: &str, resume: bool) -> Result<Self, String> {
        let path = output_location(output)?;
        let existed = path.exists();
        if existed && !resume {
            return Err(format!(
                "{} already exists; use a new dedicated run directory or --resume",
                path.display()
            ));
        }
        fs::create_dir_all(&path).map_err(|e| format!("{}: {e}", path.display()))?;
        let owned = if existed {
            let marker = read_text(&path.join(RUN_MARKER))?;
            if marker.trim_end() != identity {
                return Err(format!(
                    "{} belongs to a different input/compiler run; refusing to resume",
                    path.display()
                ));
            }
            parse_ownership_manifest(&read_text(&path.join(OWNERSHIP_MANIFEST))?, &path)?
        } else {
            fs::write(path.join(RUN_MARKER), identity.as_bytes())
                .map_err(|e| format!("{}: {e}", path.display()))?;
            fs::write(path.join(OWNERSHIP_MANIFEST), b"")
                .map_err(|e| format!("{}: {e}", path.display()))?;
            BTreeSet::new()
        };
        let active = File::options()
            .write(true)
            .create_new(true)
            .open(path.join(ACTIVE_MARKER))
            .map_err(|e| {
                if e.kind() == io::ErrorKind::AlreadyExists {
                    format!(
                        "{} is already claimed by another permuter process (or has a stale claim)",
                        path.display()
                    )
                } else {
                    format!("{}: {e}", path.display())
                }
            })?;
        Ok(Self {
            path,
            owned,
            _active: active,
        })
    }

    fn register(&mut self, name: &str) -> Result<(), String> {
        validate_owned_name(name)?;
        if self.owned.insert(name.to_string()) {
            self.sync_manifest()?;
        }
        Ok(())
    }

    fn replace_owned_file(&self, name: &str, contents: &[u8]) -> Result<(), String> {
        validate_owned_name(name)?;
        if !self.owned.contains(name) {
            return Err(format!("refusing to write unowned output file {name}"));
        }
        let path = self.path.join(name);
        if fs::symlink_metadata(&path)
            .map(|m| m.file_type().is_symlink())
            .unwrap_or(false)
        {
            return Err(format!("refusing to overwrite symlink {}", path.display()));
        }
        fs::write(&path, contents).map_err(|e| format!("{}: {e}", path.display()))
    }

    fn read_file_optional(&self, name: &str) -> Result<Option<Vec<u8>>, String> {
        let path = self.path.join(name);
        match fs::read(&path) {
            Ok(bytes) => Ok(Some(bytes)),
            Err(e) if e.kind() == io::ErrorKind::NotFound => Ok(None),
            Err(e) => Err(format!("{}: {e}", path.display())),
        }
    }

    fn open_append(&self, name: &str) -> Result<File, String> {
        let path = self.path.join(name);
        File::options()
            .append(true)
            .open(&path)
            .map_err(|e| format!("{}: {e}", path.display()))
    }

    fn write_control_file(&self, name: &str, contents: &[u8]) -> Result<(), String> {
        let path = self.path.join(name);
        if fs::symlink_metadata(&path)
            .map(|m| m.file_type().is_symlink())
            .unwrap_or(false)
        {
            return Err(format!("refusing to overwrite symlink {}", path.display()));
        }
        fs::write(&path, contents).map_err(|e| format!("{}: {e}", path.display()))
    }

    fn remove_previous_results(&mut self) -> Result<(), String> {
        let old = self.owned.iter().cloned().collect::<Vec<_>>();
        for name in old {
            if (name.starts_with("candidate-") && name.ends_with(".c"))
                || matches!(
                    name.as_str(),
                    "best.c" | "best-UNVERIFIED.c" | "report.json"
                )
            {
                let path = self.path.join(&name);
                match fs::remove_file(&path) {
                    Ok(()) => {}
                    Err(ref e) if e.kind() == io::ErrorKind::NotFound => {}
                    Err(e) => return Err(format!("{}: {e}", path.display())),
                }
                self.owned.remove(&name);
            }
        }
        self.sync_manifest()
    }

    fn sync_manifest(&self) -> Result<(), String> {
        let text = self.owned.iter().cloned().collect::<Vec<_>>().join("\n");
        let manifest = if text.is_empty() {
            String::new()
        } else {
            format!("{text}\n")
        };
        self.write_control_file(OWNERSHIP_MANIFEST, manifest.as_bytes())
    }
}

impl Drop for RunDirectory {
    fn drop(&mut self) {
        let _ = fs::remove_file(self.path.join(ACTIVE_MARKER));
    }
}

fn read_text(path: &Path) -> Result<String, String> {
    fs::read_to_string(path).map_err(|e| format!("{}: {e}", path.display()))
}

struct Journal {
    cached: BTreeMap<String, Measurement>,
    identity: String,
    path: PathBuf,
    writer: Mutex<File>,
    bytes_written: Mutex<usize>,
}

impl Journal {
    fn open(run: &mut RunDirectory, identity: &str, resume: bool) -> Result<Self, String> {
        run.register("journal.tsv")?;
        let path = run.path.join("journal.tsv");
        let header = format!("permuter-journal-v3\t{identity}");
        let existing = run.read_file_optional("journal.tsv")?;
        let mut cached = BTreeMap::new();
        if resume {
            if let Some(bytes) = &existing {
                let text = String::from_utf8(bytes.clone())
                    .map_err(|_| format!("{}: journal is not valid UTF-8", path.display()))?;
                let mut lines = text.lines();
                if lines.next() != Some(&header) {
                    return Err(format!(
                        "{}: journal identity or format does not match this run",
                        path.display()
                    ));
                }
                for (line, row) in lines.enumerate() {
                    let fields = row.split('\t').collect::<Vec<_>>();
                    let Some((fingerprint, measurement)) = parse_journal_row(&fields, identity)
                    else {
                        return Err(format!(
                            "{}: invalid, unauthenticated, or incomplete row {}",
                            path.display(),
                            line + 2
                        ));
                    };
                    cached.insert(fingerprint, measurement);
                }
            }
        }
        let bytes_written = existing.as_ref().map_or(0, Vec::len);
        if bytes_written > MAX_OUTPUT_BYTES {
            return Err(format!(
                "{} exceeds the {MAX_OUTPUT_BYTES}-byte journal budget",
                path.display()
            ));
        }
        if existing.is_none() {
            run.write_control_file("journal.tsv", format!("{header}\n").as_bytes())?;
        }
        let bytes_written = if existing.is_none() {
            header.len() + 1
        } else {
            bytes_written
        };
        Ok(Self {
            cached,
            identity: identity.to_string(),
            path,
            writer: Mutex::new(run.open_append("journal.tsv")?),
            bytes_written: Mutex::new(bytes_written),
        })
    }

    fn cached(&self, fingerprint: &str) -> Option<Measurement> {
        self.cached.get(fingerprint).cloned()
    }

    fn import(&mut self, path: &Path) -> Result<usize, String> {
        let text = read_text(path)?;
        let header = format!("permuter-journal-v3\t{}", self.identity);
        let mut lines = text.lines();
        if lines.next() != Some(&header) {
            return Err(format!(
                "{}: journal identity does not match this backend; not importing",
                path.display()
            ));
        }
        let mut imported = 0;
        for row in lines {
            if let Some((fingerprint, measurement)) =
                parse_journal_row(&row.split('\t').collect::<Vec<_>>(), &self.identity)
            {
                if self.cached.insert(fingerprint, measurement).is_none() {
                    imported += 1;
                }
            }
        }
        Ok(imported)
    }

    fn record(&self, candidate: &str, measurement: &Measurement) -> Result<(), String> {
        let measurement = persisted_measurement(measurement);
        let first = measurement
            .first_difference
            .map_or_else(|| "-".into(), |n| n.to_string());
        let line = format!(
            "{candidate}\t{}\t{}\t{}\t{}\t{}\t{}\t{:016x}\t{}\t{}\t{}\t{}\t{:016x}\t{:016x}\n",
            u8::from(measurement.exact),
            measurement.score,
            measurement.differences,
            measurement.expected_size,
            measurement.actual_size,
            first,
            measurement.fingerprint,
            measurement.bl_divergence,
            measurement.store_divergence,
            measurement.summary,
            journal_row_auth(&self.identity, candidate, &measurement, true),
            measurement.bl_signature,
            measurement.store_signature,
        );
        let mut size = self
            .bytes_written
            .lock()
            .map_err(|_| "journal size lock is poisoned".to_string())?;
        if size.saturating_add(line.len()) > MAX_OUTPUT_BYTES {
            return Err(format!(
                "{} would exceed the {MAX_OUTPUT_BYTES}-byte journal budget",
                self.path.display()
            ));
        }
        let mut writer = self
            .writer
            .lock()
            .map_err(|_| "journal lock is poisoned".to_string())?;
        writer
            .write_all(line.as_bytes())
            .and_then(|_| writer.flush())
            .map_err(|e| format!("{}: {e}", self.path.display()))?;
        *size += line.len();
        Ok(())
    }
}

fn persisted_summary(summary: &str) -> String {
    let mut output = String::new();
    for c in summary.chars() {
        if output.len() >= MAX_SUMMARY_BYTES {
            break;
        }
        output.push(if c.is_control() { ' ' } else { c });
    }
    output
}

fn persisted_measurement(measurement: &Measurement) -> Measurement {
    let mut value = measurement.clone();
    value.summary = persisted_summary(&measurement.summary);
    value
}

fn journal_row_auth(
    identity: &str,
    candidate: &str,
    measurement: &Measurement,
    v3: bool,
) -> String {
    let mut values = vec![
        if v3 {
            "permuter-journal-row-v3".to_string()
        } else {
            "permuter-journal-row-v2".to_string()
        },
        identity.to_string(),
        candidate.to_string(),
        u8::from(measurement.exact).to_string(),
        measurement.score.to_string(),
        measurement.differences.to_string(),
        measurement.expected_size.to_string(),
        measurement.actual_size.to_string(),
        measurement
            .first_difference
            .map_or_else(|| "-".into(), |n| n.to_string()),
        measurement.bl_divergence.to_string(),
        measurement.store_divergence.to_string(),
    ];
    if v3 {
        values.push(format!("{:016x}", measurement.bl_signature));
        values.push(format!("{:016x}", measurement.store_signature));
    }
    values.push(format!("{:016x}", measurement.fingerprint));
    values.push(measurement.summary.clone());
    let mut bytes = Vec::new();
    for value in values {
        bytes.extend_from_slice(&(value.len() as u64).to_be_bytes());
        bytes.extend_from_slice(value.as_bytes());
    }
    compiler_core::sha256::hex(&bytes)
}

fn parse_journal_row(fields: &[&str], identity: &str) -> Option<(String, Measurement)> {
    // 12 fields is the legacy v2 row; 14 appends the bl/store signatures.
    // Legacy rows import with unknown (zero) signatures, which still serve
    // compile dedup but never satisfy an equal-divergence guard comparison.
    let v3 = fields.len() == 14;
    if !(v3 || fields.len() == 12)
        || fields[0].is_empty()
        || fields[10].len() > MAX_SUMMARY_BYTES
        || fields[10].contains(['\r', '\n'])
        || fields[11].is_empty()
    {
        return None;
    }
    let exact = match fields[1] {
        "0" => false,
        "1" => true,
        _ => return None,
    };
    let score = fields[2].parse().ok()?;
    let differences = fields[3].parse().ok()?;
    let expected_size: usize = fields[4].parse().ok()?;
    let actual_size: usize = fields[5].parse().ok()?;
    let first_difference = if fields[6] == "-" {
        None
    } else {
        Some(fields[6].parse().ok()?)
    };
    let fingerprint = u64::from_str_radix(fields[7], 16).ok()?;
    let bl_divergence = fields[8].parse().ok()?;
    let store_divergence = fields[9].parse().ok()?;
    let bl_signature = if v3 {
        u64::from_str_radix(fields[12], 16).ok()?
    } else {
        0
    };
    let store_signature = if v3 {
        u64::from_str_radix(fields[13], 16).ok()?
    } else {
        0
    };
    if score == u64::MAX
        || first_difference.is_some_and(|n| n > expected_size.max(actual_size))
        || (exact
            && (score != 0
                || differences != 0
                || expected_size != actual_size
                || first_difference.is_some()))
    {
        return None;
    }
    let measurement = Measurement {
        exact,
        score,
        differences,
        expected_size,
        actual_size,
        first_difference,
        fingerprint,
        heat: Vec::new(),
        bl_divergence,
        store_divergence,
        bl_signature,
        store_signature,
        summary: fields[10].into(),
    };
    (journal_row_auth(identity, fields[0], &measurement, v3) == fields[11])
        .then(|| (fields[0].into(), measurement))
}

fn source_fingerprint(source: &str) -> String {
    compiler_core::sha256::hex(source.as_bytes())
}

fn candidate_plan(
    permutation: &Permutation,
    iterations: usize,
    seed: u64,
    manual_only: bool,
    weights: &Weights,
    source_bytes: usize,
) -> Result<Vec<Candidate>, String> {
    if iterations == 0 || iterations > MAX_ITERATIONS {
        return Err(format!(
            "candidate plan iterations must be between 1 and {MAX_ITERATIONS}"
        ));
    }
    if source_bytes > MAX_SOURCE_BYTES {
        return Err(format!(
            "source is {source_bytes} bytes; the maximum permutation source is {MAX_SOURCE_BYTES} bytes"
        ));
    }
    if iterations
        .saturating_mul(source_bytes.max(1))
        .saturating_mul(3)
        > MAX_PLAN_BYTES
    {
        return Err(format!(
            "{iterations} iterations would exceed the {MAX_PLAN_BYTES}-byte source plan budget"
        ));
    }
    let base_count = permutation.count().min(iterations).max(1);
    let randomize = permutation.is_random() && !manual_only;
    let per_base = if randomize {
        iterations.div_ceil(base_count)
    } else {
        1
    };
    let mut pools = Vec::with_capacity(base_count);
    let mut pool_bytes: usize = 0;
    for manual_seed in 0..base_count {
        let (source, _) = if randomize {
            permutation.evaluate_marked(manual_seed)?
        } else {
            permutation.evaluate(manual_seed)?
        };
        if source.len() > MAX_SOURCE_BYTES {
            return Err(format!(
                "a materialized base source exceeds the {MAX_SOURCE_BYTES}-byte source budget"
            ));
        }
        pool_bytes = pool_bytes.saturating_add(per_base.saturating_mul(source.len()));
        if pool_bytes.saturating_mul(3) > MAX_PLAN_BYTES {
            return Err(format!(
                "materialized mutation pools exceed the {MAX_PLAN_BYTES}-byte plan budget"
            ));
        }
        let variants = if randomize {
            crate::randomize::try_mutate_marked_with_weights(
                &source,
                seed ^ (manual_seed as u64).wrapping_mul(0x9e37_79b9_7f4a_7c15),
                per_base,
                weights,
            )?
        } else {
            vec![crate::randomize::Mutation {
                id: "manual".into(),
                source,
            }]
        };
        if variants
            .iter()
            .any(|variant| variant.source.len() > MAX_SOURCE_BYTES)
        {
            return Err(format!(
                "a generated source exceeds the {MAX_SOURCE_BYTES}-byte source budget"
            ));
        }
        pools.push((manual_seed, variants));
    }
    let mut candidates = Vec::new();
    let mut seen = BTreeSet::new();
    let mut bytes: usize = 0;
    for layer in 0.. {
        let mut added = false;
        for (manual_seed, variants) in &pools {
            let Some(variant) = variants.get(layer) else {
                continue;
            };
            added = true;
            let fingerprint = source_fingerprint(&variant.source);
            if seen.insert(fingerprint.clone()) {
                bytes = bytes.saturating_add(variant.source.len());
                if bytes.saturating_mul(3) > MAX_PLAN_BYTES {
                    return Err(format!(
                        "generated source plan exceeds the {MAX_PLAN_BYTES}-byte budget"
                    ));
                }
                candidates.push(Candidate {
                    index: candidates.len(),
                    manual_seed: *manual_seed,
                    mutation: variant.id.clone(),
                    source: variant.source.clone(),
                    fingerprint,
                });
                if candidates.len() == iterations {
                    return Ok(candidates);
                }
            }
        }
        if !added {
            break;
        }
    }
    if candidates.is_empty() {
        Err("permutation plan produced no candidates".into())
    } else {
        Ok(candidates)
    }
}

fn measurement_order(
    left: &(&Candidate, &Measurement),
    right: &(&Candidate, &Measurement),
) -> Ordering {
    right
        .1
        .exact
        .cmp(&left.1.exact)
        .then_with(|| left.1.score.cmp(&right.1.score))
        .then_with(|| left.1.differences.cmp(&right.1.differences))
        .then_with(|| {
            left.1
                .actual_size
                .abs_diff(left.1.expected_size)
                .cmp(&right.1.actual_size.abs_diff(right.1.expected_size))
        })
        .then_with(|| left.0.index.cmp(&right.0.index))
}

fn safe_label(input: &backend::Input) -> String {
    let path = if input.directory_mode {
        &input.requested
    } else {
        &input.source_path
    };
    let mut value = path
        .file_stem()
        .and_then(|v| v.to_str())
        .unwrap_or("candidate")
        .chars()
        .map(|c| {
            if c.is_ascii_alphanumeric() || matches!(c, '.' | '-' | '_') {
                c
            } else {
                '_'
            }
        })
        .collect::<String>();
    if value.is_empty() || value == "." || value == ".." {
        value = "candidate".into();
    }
    value.truncate(64);
    value
}

fn append_identity_field(bytes: &mut Vec<u8>, value: &str) {
    bytes.extend_from_slice(&(value.len() as u64).to_be_bytes());
    bytes.extend_from_slice(value.as_bytes());
}

fn input_identity(input: &backend::Input) -> Result<String, String> {
    let requested = fs::canonicalize(&input.requested)
        .map_err(|e| format!("{}: {e}", input.requested.display()))?;
    let source = fs::canonicalize(&input.source_path)
        .map_err(|e| format!("{}: {e}", input.source_path.display()))?;
    let mut bytes = Vec::new();
    append_identity_field(&mut bytes, "permuter-input-v2");
    append_identity_field(&mut bytes, &requested.to_string_lossy());
    append_identity_field(&mut bytes, &source.to_string_lossy());
    append_identity_field(&mut bytes, &source_fingerprint(&input.source));
    Ok(compiler_core::sha256::hex(&bytes))
}

fn run_identity(input: &backend::Input, target: &dyn Backend, seed: u64) -> Result<String, String> {
    let mut bytes = Vec::new();
    append_identity_field(&mut bytes, "permuter-run-v2");
    append_identity_field(&mut bytes, &input_identity(input)?);
    append_identity_field(&mut bytes, &target.identity());
    append_identity_field(&mut bytes, &seed.to_string());
    Ok(compiler_core::sha256::hex(&bytes))
}

fn output_location(path: &Path) -> Result<PathBuf, String> {
    let path = if path.is_absolute() {
        path.to_path_buf()
    } else {
        root().join(path)
    };
    let mut out = PathBuf::new();
    for component in path.components() {
        match component {
            std::path::Component::CurDir => {}
            std::path::Component::ParentDir => {
                out.pop();
            }
            component => out.push(component.as_os_str()),
        }
    }
    if !out.is_absolute() {
        return Err(format!("{} is not an absolute path", path.display()));
    }
    let mut existing = out.clone();
    while fs::symlink_metadata(&existing).is_err() && existing.pop() {}
    let mut resolved =
        fs::canonicalize(&existing).map_err(|e| format!("{}: {e}", existing.display()))?;
    if let Ok(relative) = out.strip_prefix(&existing) {
        resolved.extend(relative);
    }
    let out = fs::canonicalize(root().join("out"))
        .map_err(|e| format!("{}: {e}", root().join("out").display()))?;
    let temp = fs::canonicalize(std::env::temp_dir()).unwrap_or_else(|_| std::env::temp_dir());
    for trusted in [&out, &temp] {
        if resolved == *trusted {
            return Err(format!(
                "refusing shared output root {}; choose a dedicated child directory",
                path.display()
            ));
        }
        if resolved.starts_with(trusted) {
            return Ok(resolved);
        }
    }
    if resolved.starts_with(root()) {
        return Err(format!(
            "refusing output path {}: repository outputs must be under out/",
            path.display()
        ));
    }
    Err(format!(
        "refusing output path {}: it must be under repository out/ or the OS temporary directory",
        path.display()
    ))
}

fn validate_output_path(path: &Path) -> Result<(), String> {
    let resolved = output_location(path)?;
    if resolved.exists() && !resolved.is_dir() {
        return Err(format!("{} is not a directory", path.display()));
    }
    Ok(())
}

fn validate_owned_name(name: &str) -> Result<(), String> {
    let valid = (name.starts_with("candidate-") && name.ends_with(".c"))
        || matches!(
            name,
            "best.c" | "best-UNVERIFIED.c" | "journal.tsv" | "report.json"
        );
    if !valid || name.contains('/') || name.contains('\\') {
        Err(format!("invalid owned output name {name:?}"))
    } else {
        Ok(())
    }
}

fn parse_ownership_manifest(text: &str, path: &Path) -> Result<BTreeSet<String>, String> {
    text.lines()
        .map(|line| {
            validate_owned_name(line).map_err(|e| format!("{}: {e}", path.display()))?;
            Ok(line.to_string())
        })
        .collect()
}

fn save_results(
    run: &mut RunDirectory,
    backend_name: &str,
    search_mode: &str,
    baseline: &Measurement,
    retained: &[(&Candidate, &Measurement)],
    attempted: usize,
    failures: usize,
) -> Result<(), String> {
    let candidate_bytes = retained.iter().map(|(c, _)| c.source.len()).sum::<usize>();
    if candidate_bytes > MAX_OUTPUT_BYTES {
        return Err(format!(
            "retained candidate output exceeds the {MAX_OUTPUT_BYTES}-byte budget"
        ));
    }
    run.remove_previous_results()?;
    for (rank, (candidate, measurement)) in retained.iter().enumerate() {
        let unverified = search_mode == "classic-exact-only" && !measurement.exact;
        let suffix = if unverified { "-UNVERIFIED" } else { "" };
        let name = format!("candidate-{rank:03}-score-{}{suffix}.c", measurement.score);
        run.register(&name)?;
        run.replace_owned_file(&name, candidate.source.as_bytes())?;
    }
    if let Some((candidate, measurement)) = retained.first() {
        let name = if search_mode == "classic-exact-only" && !measurement.exact {
            "best-UNVERIFIED.c"
        } else {
            "best.c"
        };
        run.register(name)?;
        run.replace_owned_file(name, candidate.source.as_bytes())?;
    }
    let results = retained
        .iter()
        .enumerate()
        .map(|(rank, (candidate, measurement))| {
            json!({
                "rank": rank,
                "candidate": candidate.index,
                "manual_seed": candidate.manual_seed,
                "mutation": candidate.mutation,
                "source_fingerprint": candidate.fingerprint,
                "score": measurement.score,
                "exact": measurement.exact,
                "differences": measurement.differences,
                "actual_size": measurement.actual_size,
                "expected_size": measurement.expected_size,
                "summary": persisted_summary(&measurement.summary),
            })
        })
        .collect::<Vec<_>>();
    let report = serde_json::to_string_pretty(&json!({
        "backend": backend_name,
        "search_mode": search_mode,
        "baseline_score": baseline.score,
        "attempted": attempted,
        "compile_failures": failures,
        "results": results,
    }))
    .map_err(|e| format!("could not serialize report: {e}"))?
        + "\n";
    if candidate_bytes.saturating_add(report.len()) > MAX_OUTPUT_BYTES {
        return Err(format!(
            "candidate files and report exceed the {MAX_OUTPUT_BYTES}-byte output budget"
        ));
    }
    run.register("report.json")?;
    run.replace_owned_file("report.json", report.as_bytes())
}

fn output_budget_preflight(candidates: &[Candidate], top: usize) -> Result<(), String> {
    let mut sizes = candidates
        .iter()
        .map(|c| c.source.len())
        .collect::<Vec<_>>();
    sizes.sort_unstable_by(|a, b| b.cmp(a));
    let candidate_bytes = sizes.into_iter().take(top).sum::<usize>();
    let mut report_sizes = candidates
        .iter()
        .map(|c| 512 + c.mutation.len() * 6 + c.fingerprint.len() + MAX_SUMMARY_BYTES * 6)
        .collect::<Vec<_>>();
    report_sizes.sort_unstable_by(|a, b| b.cmp(a));
    let report_bytes = 2048usize.saturating_add(report_sizes.into_iter().take(top).sum::<usize>());
    if candidate_bytes.saturating_add(report_bytes) > MAX_OUTPUT_BYTES {
        return Err(format!(
            "planned retained candidates and report exceed the {MAX_OUTPUT_BYTES}-byte output budget before compilation"
        ));
    }
    if 128usize.saturating_add(candidates.len().saturating_mul(MAX_JOURNAL_ROW_BYTES))
        > MAX_OUTPUT_BYTES
    {
        return Err(format!(
            "the {MAX_JOURNAL_ROW_BYTES}-byte journal-row bound for {} candidates exceeds the journal budget before compilation",
            candidates.len()
        ));
    }
    Ok(())
}

/// Semantic-lie guard. Divergence must never exceed the baseline's, and at
/// EQUAL divergence the candidate's own row signature must match the
/// baseline's: the count alone is baseline-relative, so a lateral call or
/// store move can hide under existing divergence by trading one differing
/// row for another. Only a strict move toward the reference may change the
/// rows. Exact candidates are definitionally true.
fn guard_accepts(baseline: &Measurement, value: &Measurement) -> bool {
    value.exact
        || ((value.bl_divergence < baseline.bl_divergence
            || (value.bl_divergence == baseline.bl_divergence
                && value.bl_signature == baseline.bl_signature))
            && (value.store_divergence < baseline.store_divergence
                || (value.store_divergence == baseline.store_divergence
                    && value.store_signature == baseline.store_signature)))
}

fn retain_result(
    options: &Options,
    baseline: &Measurement,
    former_best: u64,
    measurement: &Measurement,
) -> bool {
    measurement.score != u64::MAX
        && guard_accepts(baseline, measurement)
        && (!options.better_only || measurement.score < baseline.score)
        && (!options.best_only || measurement.score <= former_best)
        && !options
            .score_threshold
            .is_some_and(|threshold| measurement.score >= threshold)
        && (measurement.exact || measurement.score <= baseline.score)
}

fn run_workers(
    backend: Arc<dyn Backend>,
    candidates: Arc<Vec<Candidate>>,
    jobs: usize,
    stop_exact: bool,
    baseline: Measurement,
    journal: Arc<Journal>,
    live_best: Arc<Mutex<LiveBest>>,
) -> Result<Vec<Evaluated>, String> {
    let next = Arc::new(AtomicUsize::new(0));
    let stop = Arc::new(AtomicBool::new(false));
    let (sender, receiver) = mpsc::channel::<Result<Evaluated, String>>();
    let mut workers = Vec::new();
    for _ in 0..jobs.min(candidates.len()).max(1) {
        let (backend, candidates, next, stop, sender, journal, baseline, live_best) = (
            Arc::clone(&backend),
            Arc::clone(&candidates),
            Arc::clone(&next),
            Arc::clone(&stop),
            sender.clone(),
            Arc::clone(&journal),
            baseline.clone(),
            Arc::clone(&live_best),
        );
        workers.push(std::thread::spawn(move || loop {
            if stop_exact && stop.load(AtomicOrdering::Acquire) {
                break;
            }
            let index = next.fetch_add(1, AtomicOrdering::Relaxed);
            let Some(candidate) = candidates.get(index).cloned() else {
                break;
            };
            let started = Instant::now();
            let cached = journal.cached(&candidate.fingerprint);
            let measurement = match cached.as_ref() {
                Some(value) if !value.exact => Ok(value.clone()),
                Some(_) => backend.measure(&candidate.source),
                None if index == 0 => Ok(baseline.clone()),
                None => backend.measure(&candidate.source),
            };
            if let Ok(value) = &measurement {
                if cached.is_none() || cached.as_ref().is_some_and(|old| old.exact && old != value)
                {
                    if let Err(error) = journal.record(&candidate.fingerprint, value) {
                        stop.store(true, AtomicOrdering::Release);
                        let _ = sender.send(Err(error));
                        break;
                    }
                }
                if stop_exact && value.exact {
                    stop.store(true, AtomicOrdering::Release);
                }
                if guard_accepts(&baseline, value)
                    && update_live_best(
                        &live_best,
                        baseline.score,
                        value,
                        &candidate.source,
                        &candidate.mutation,
                    )
                {
                    println!(
                        "new-best={} candidate={} mutation={} {}",
                        value.score, candidate.index, candidate.mutation, value.summary
                    );
                    let _ = io::stdout().flush();
                }
            }
            if sender
                .send(Ok(Evaluated {
                    candidate,
                    measurement,
                    elapsed: started.elapsed(),
                }))
                .is_err()
            {
                break;
            }
        }));
    }
    drop(sender);
    let mut evaluated = Vec::new();
    let mut error = None;
    for item in receiver {
        match item {
            Ok(item) => evaluated.push(item),
            Err(e) => {
                error.get_or_insert(e);
            }
        }
    }
    for worker in workers {
        if worker.join().is_err() && error.is_none() {
            error = Some("permuter worker panicked".into());
        }
    }
    error.map_or(Ok(evaluated), Err)
}

type LiveBest = Option<(u64, String, String)>;

fn update_live_best(
    best: &Mutex<LiveBest>,
    baseline_score: u64,
    measurement: &Measurement,
    source: &str,
    mutation: &str,
) -> bool {
    if measurement.score >= baseline_score {
        return false;
    }
    let Ok(mut best) = best.lock() else {
        return false;
    };
    if best
        .as_ref()
        .is_some_and(|(score, _, _)| measurement.score >= *score)
    {
        return false;
    }
    *best = Some((measurement.score, source.to_string(), mutation.to_string()));
    true
}

#[allow(clippy::too_many_arguments)]
fn walk_workers(
    backend: Arc<dyn Backend>,
    base_source: Arc<String>,
    total: usize,
    jobs: usize,
    seed: u64,
    keep_prob_permille: u32,
    stop_exact: bool,
    heat_enabled: bool,
    classic: bool,
    guard: Measurement,
    journal: Arc<Journal>,
    best: Arc<Mutex<LiveBest>>,
) -> Result<Vec<Evaluated>, String> {
    let counter = Arc::new(AtomicUsize::new(0));
    let stop = Arc::new(AtomicBool::new(false));
    let (sender, receiver) = mpsc::channel::<Result<Evaluated, String>>();
    let mut workers = Vec::new();
    for worker in 0..jobs.max(1) {
        let (backend, base_source, counter, stop, sender, best, journal, guard) = (
            Arc::clone(&backend),
            Arc::clone(&base_source),
            Arc::clone(&counter),
            Arc::clone(&stop),
            sender.clone(),
            Arc::clone(&best),
            Arc::clone(&journal),
            guard.clone(),
        );
        workers.push(std::thread::spawn(move || {
            let mut rng = crate::randomize::SplitMix64(
                seed ^ (worker as u64).wrapping_mul(0x9e37_79b9_7f4a_7c15) ^ 0x5851_f42d_4c95_7f2d,
            );
            let mut current: Option<(String, String)> = None;
            let mut heat = Vec::new();
            loop {
                if stop_exact && stop.load(AtomicOrdering::Acquire) {
                    break;
                }
                let index = counter.fetch_add(1, AtomicOrdering::Relaxed);
                if index >= total {
                    break;
                }
                if current.is_none() || rng.index(1000) >= keep_prob_permille as usize {
                    current = best
                        .lock()
                        .ok()
                        .and_then(|g| g.clone())
                        .map(|(_, source, lineage)| (source, format!("{lineage}~")))
                        .or_else(|| Some((base_source.as_ref().clone(), "base".into())));
                }
                let (source, lineage) = current.clone().expect("walk candidate");
                let mode = if classic {
                    crate::astpass::AstMode::Classic
                } else {
                    crate::astpass::AstMode::Safe
                };
                let mutated = crate::astpass::AstRandomizer::new_with_mode(
                    &source,
                    rng.0 ^ index as u64,
                    None,
                    mode,
                )
                .and_then(|mut randomizer| {
                    if heat_enabled {
                        randomizer.set_heat(heat.clone());
                    }
                    randomizer.randomize_named()
                });
                let Ok((source, pass)) = mutated else {
                    current = None;
                    continue;
                };
                let candidate = Candidate {
                    index,
                    manual_seed: worker,
                    mutation: format!("{lineage}>{pass}"),
                    fingerprint: source_fingerprint(&source),
                    source: source.clone(),
                };
                let started = Instant::now();
                let cached = journal.cached(&candidate.fingerprint);
                let measurement = match cached.as_ref() {
                    Some(value) if !value.exact => Ok(value.clone()),
                    _ => backend.measure(&source),
                };
                if let Ok(value) = &measurement {
                    if cached.is_none()
                        || cached.as_ref().is_some_and(|old| old.exact && old != value)
                    {
                        let _ = journal.record(&candidate.fingerprint, value);
                    }
                    if value.exact {
                        stop.store(true, AtomicOrdering::Release);
                    }
                    if guard_accepts(&guard, value) {
                        current = Some((source.clone(), candidate.mutation.clone()));
                        heat = value.heat.clone();
                        if update_live_best(&best, guard.score, value, &source, &candidate.mutation)
                        {
                            println!(
                                "new-best={} candidate={} mutation={} {}",
                                value.score, candidate.index, candidate.mutation, value.summary
                            );
                            let _ = io::stdout().flush();
                        }
                    } else {
                        current = None;
                    }
                } else {
                    current = None;
                }
                if sender
                    .send(Ok(Evaluated {
                        candidate,
                        measurement,
                        elapsed: started.elapsed(),
                    }))
                    .is_err()
                {
                    break;
                }
            }
        }));
    }
    drop(sender);
    let mut evaluated = Vec::new();
    for item in receiver {
        if let Ok(item) = item {
            evaluated.push(item);
        }
    }
    for worker in workers {
        if worker.join().is_err() {
            return Err("permuter walk worker panicked".into());
        }
    }
    Ok(evaluated)
}

fn collect_results(
    evaluated: &[Evaluated],
    options: &Options,
    baseline: &Measurement,
    best_score: &mut u64,
    owned: &mut Vec<(Candidate, Measurement)>,
    attempted: &mut usize,
    failures: &mut usize,
    compile_time: &mut Duration,
    exact_found: &mut bool,
    round: Option<usize>,
    chain_limit: Option<u64>,
) -> Option<(usize, u64)> {
    *attempted += evaluated.len();
    let mut round_best = None;
    for item in evaluated {
        *compile_time += item.elapsed;
        match &item.measurement {
            Ok(measurement) => {
                let former = *best_score;
                let guarded = guard_accepts(baseline, measurement);
                if guarded && measurement.score < *best_score {
                    *best_score = measurement.score;
                }
                *exact_found |= measurement.exact;
                if let Some(round) = round {
                    if guarded
                        && measurement.score <= chain_limit.unwrap_or(baseline.score)
                        && item.candidate.mutation != "identity"
                        && round_best.is_none_or(|(_, score)| {
                            measurement.score < score
                                || (measurement.score == score && round % 3 != 0)
                        })
                    {
                        round_best = Some((item.candidate.index, measurement.score));
                    }
                }
                if retain_result(options, baseline, former, measurement) {
                    owned.push((item.candidate.clone(), measurement.clone()));
                }
            }
            Err(error) => {
                *failures += 1;
                if options.show_errors {
                    eprintln!("candidate {}: {error}", item.candidate.index);
                }
            }
        }
    }
    round_best
}

fn run_one(options: &Options, candidate: &Path, multiple: bool) -> Result<(), String> {
    let input = backend::load_input(candidate)?;
    if input.source.len() > MAX_SOURCE_BYTES {
        return Err(format!(
            "{} is {} bytes; the maximum permutation source is {MAX_SOURCE_BYTES} bytes",
            input.source_path.display(),
            input.source.len()
        ));
    }
    let input_key = input_identity(&input)?;
    let output = match &options.output {
        Some(base) if multiple => base.join(format!(
            "{}-{input_key}-seed-{}",
            safe_label(&input),
            options.seed
        )),
        Some(path) => path.clone(),
        None => root().join("out").join("permuter").join(format!(
            "{}-{input_key}-seed-{}",
            safe_label(&input),
            options.seed
        )),
    };
    validate_output_path(&output)?;
    let permutation = crate::perm::parse(&input.source)?;
    let weights = if let Some(path) = &options.weights {
        if !path.is_file() {
            return Err(format!("--weights {}: no such file", path.display()));
        }
        Weights::from_settings(path, "gcc")?
    } else if input.directory_mode {
        Weights::from_settings(&input.requested.join("settings.toml"), "base")?
    } else {
        Weights::for_profile("gcc")
    };
    let candidates = if options.walk {
        Vec::new()
    } else {
        let planned = candidate_plan(
            &permutation,
            if options.debug { 1 } else { options.iterations },
            options.seed,
            options.manual_only,
            &weights,
            input.source.len(),
        )?;
        output_budget_preflight(&planned, options.top)?;
        planned
    };
    // The compiler baseline must be valid C. Planned sources may contain
    // PERM_* directives, so materialize seed zero before preparing the
    // routed target; compiling the raw template makes every explicit plan
    // fail before its candidates can run.
    let (base_source, _) = permutation.evaluate(0)?;
    let target = backend::prepare(&input, &base_source, options.show_errors)?;
    let baseline = target.baseline();
    let identity = run_identity(&input, target.as_ref(), options.seed)?;
    let mut run = RunDirectory::claim(&output, &identity, options.resume)?;
    let cache_identity = target.identity();
    let mut journal = Journal::open(&mut run, &cache_identity, options.resume)?;
    if let Some(prior) = &options.journal_from {
        println!(
            "journal: imported {} cached measurements from {}",
            journal.import(prior)?,
            prior.display()
        );
    }
    let journal = Arc::new(journal);
    let search_mode = if options.walk && options.classic {
        "classic-exact-only"
    } else if options.walk {
        "safe"
    } else {
        "planned"
    };
    println!(
        "backend={} mode={} bases={} candidates={} jobs={} baseline={} ({})",
        target.name(),
        search_mode,
        permutation.count(),
        if options.walk {
            options.iterations
        } else {
            candidates.len()
        },
        if options.walk {
            options.jobs
        } else {
            options.jobs.min(candidates.len())
        },
        baseline.score,
        baseline.summary
    );
    if options.debug {
        return Ok(());
    }
    let backend_name = target.name().to_string();
    let target: Arc<dyn Backend> = Arc::from(target);
    let started = Instant::now();
    let mut chain_source = input.source.clone();
    let mut chain_score = baseline.score;
    let mut best_score = baseline.score;
    let mut owned = Vec::new();
    let mut attempted = 0;
    let mut failures = 0;
    let mut compile_time = Duration::ZERO;
    let mut exact_found = false;
    let live_best = Arc::new(Mutex::new(None));
    if options.walk {
        let base = crate::astpass::preprocess_for_ast(&input.source)?;
        let evaluated = walk_workers(
            Arc::clone(&target),
            Arc::new(base),
            options.iterations,
            options.jobs,
            options.seed,
            options.keep_prob_permille,
            options.stop_exact,
            options.heat,
            options.classic,
            baseline.clone(),
            Arc::clone(&journal),
            Arc::clone(&live_best),
        )?;
        collect_results(
            &evaluated,
            options,
            &baseline,
            &mut best_score,
            &mut owned,
            &mut attempted,
            &mut failures,
            &mut compile_time,
            &mut exact_found,
            None,
            None,
        );
    } else {
        let mut round_candidates = Some(candidates);
        for round in 0..options.chain.max(1) {
            if round > 0 || round_candidates.is_none() {
                let permutation = crate::perm::parse(&chain_source)?;
                round_candidates = Some(candidate_plan(
                    &permutation,
                    options.iterations,
                    options.seed ^ (round as u64).wrapping_mul(0x9e37_79b9_7f4a_7c15),
                    options.manual_only,
                    &weights,
                    chain_source.len(),
                )?);
            }
            let shared = Arc::new(round_candidates.take().expect("round candidates planned"));
            let mut evaluated = run_workers(
                Arc::clone(&target),
                Arc::clone(&shared),
                options.jobs,
                options.stop_exact,
                baseline.clone(),
                Arc::clone(&journal),
                Arc::clone(&live_best),
            )?;
            evaluated.sort_by_key(|item| item.candidate.index);
            let round_best = collect_results(
                &evaluated,
                options,
                &baseline,
                &mut best_score,
                &mut owned,
                &mut attempted,
                &mut failures,
                &mut compile_time,
                &mut exact_found,
                Some(round),
                Some(chain_score),
            );
            if !options.quiet {
                println!(
                    "round={} attempted={} best={} chain={} failures={}",
                    round, attempted, best_score, chain_score, failures
                );
            }
            if let Some((index, score)) = round_best {
                if let Some(item) = evaluated.iter().find(|item| item.candidate.index == index) {
                    chain_source = item.candidate.source.clone();
                    chain_score = score;
                }
            }
            if exact_found && options.stop_exact {
                break;
            }
        }
    }
    let mut retained = owned
        .iter()
        .map(|(candidate, measurement)| (candidate, measurement))
        .collect::<Vec<_>>();
    retained.sort_by(measurement_order);
    retained.truncate(options.top);
    save_results(
        &mut run,
        &backend_name,
        search_mode,
        &baseline,
        &retained,
        attempted,
        failures,
    )?;
    println!(
        "done={} attempted={} failures={} best={} exact={} wall_ms={} compiler_ms={} output={}",
        backend_name,
        attempted,
        failures,
        best_score,
        retained.iter().any(|(_, m)| m.exact),
        started.elapsed().as_millis(),
        compile_time.as_millis(),
        output.display()
    );
    Ok(())
}

pub fn run(options: Options) -> Result<(), String> {
    let multiple = options.candidates.len() > 1;
    options
        .candidates
        .iter()
        .try_for_each(|candidate| run_one(&options, candidate, multiple))
}

pub fn self_test() -> Result<(), String> {
    let permutation =
        crate::perm::parse("int f(void) { s32 a = PERM_GENERAL(1,2); return a + 0; }")?;
    let weights = Weights::for_profile("gcc");
    if candidate_plan(&permutation, MAX_ITERATIONS + 1, 9, false, &weights, 64).is_ok() {
        return Err("candidate plan accepted an unbounded iteration request".into());
    }
    let first = candidate_plan(&permutation, 20, 9, false, &weights, 64)?;
    if first != candidate_plan(&permutation, 20, 9, false, &weights, 64)? || first.len() != 2 {
        return Err("runner plan is not deterministic".into());
    }
    if validate_output_path(&root().join("semantic")).is_ok()
        || validate_output_path(&root().join("out").join("permuter-self-test")).is_err()
        || validate_output_path(&std::env::temp_dir().join("permuter-self-test")).is_err()
    {
        return Err("output root validation drifted".into());
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn runner_smoke_checks_limits_and_authentication() {
        let permutation = crate::perm::parse("int f(void) { return 1; }").unwrap();
        let weights = Weights::for_profile("gcc");
        assert!(candidate_plan(&permutation, MAX_ITERATIONS + 1, 1, false, &weights, 64).is_err());
        assert!(validate_output_path(&root().join("semantic")).is_err());
        let mut measurement = Measurement {
            exact: false,
            score: 17,
            differences: 3,
            expected_size: 8,
            actual_size: 6,
            first_difference: Some(2),
            fingerprint: 0x1234,
            heat: Vec::new(),
            bl_divergence: 0,
            store_divergence: 0,
            bl_signature: 0,
            store_signature: 0,
            summary: "journal control".into(),
        };
        let legacy_auth = journal_row_auth("identity", "candidate", &measurement, false);
        let legacy = parse_journal_row(
            &[
                "candidate",
                "0",
                "17",
                "3",
                "8",
                "6",
                "2",
                "0000000000001234",
                "0",
                "0",
                "journal control",
                &legacy_auth,
            ],
            "identity",
        );
        assert!(legacy.is_some_and(|(_, m)| m.bl_signature == 0 && m.store_signature == 0));
        measurement.bl_signature = 0xabcd;
        measurement.store_signature = 0xef01;
        let auth = journal_row_auth("identity", "candidate", &measurement, true);
        let parsed = parse_journal_row(
            &[
                "candidate",
                "0",
                "17",
                "3",
                "8",
                "6",
                "2",
                "0000000000001234",
                "0",
                "0",
                "journal control",
                &auth,
                "000000000000abcd",
                "000000000000ef01",
            ],
            "identity",
        );
        assert!(
            parsed.is_some_and(|(_, m)| m.bl_signature == 0xabcd && m.store_signature == 0xef01)
        );
    }

    #[test]
    fn guard_rejects_lateral_divergence_trades() {
        let mut baseline = Measurement::failed("baseline");
        baseline.score = 100;
        baseline.bl_divergence = 3;
        baseline.store_divergence = 2;
        baseline.bl_signature = 0x1111;
        baseline.store_signature = 0x2222;
        let mut candidate = baseline.clone();
        // Equal divergence with identical rows continues the walk.
        assert!(guard_accepts(&baseline, &candidate));
        // Equal divergence with different rows is a lateral trade: a moved
        // call or store hiding under existing divergence.
        candidate.bl_signature = 0x3333;
        assert!(!guard_accepts(&baseline, &candidate));
        // A strict move toward the reference may change the rows.
        candidate.bl_divergence = 2;
        assert!(guard_accepts(&baseline, &candidate));
        // Unknown (legacy journal) signatures never satisfy equality.
        candidate.bl_divergence = 3;
        candidate.bl_signature = 0;
        assert!(!guard_accepts(&baseline, &candidate));
        // Exact bytes are definitionally true.
        candidate.exact = true;
        assert!(guard_accepts(&baseline, &candidate));
    }

    #[test]
    fn live_best_reports_strict_improvements_as_they_arrive() {
        let best = Mutex::new(None);
        let mut measurement = Measurement::failed("candidate");
        measurement.score = 110;
        assert!(!update_live_best(
            &best,
            100,
            &measurement,
            "worse",
            "worse"
        ));
        assert!(best.lock().unwrap().is_none());

        measurement.score = 90;
        assert!(update_live_best(&best, 100, &measurement, "first", "first"));
        measurement.score = 90;
        assert!(!update_live_best(&best, 100, &measurement, "tie", "tie"));
        measurement.score = 95;
        assert!(!update_live_best(
            &best,
            100,
            &measurement,
            "later",
            "later"
        ));
        measurement.score = 80;
        assert!(update_live_best(&best, 100, &measurement, "best", "best"));

        let state = best.into_inner().unwrap().unwrap();
        assert_eq!(state, (80, "best".into(), "best".into()));
    }
}
