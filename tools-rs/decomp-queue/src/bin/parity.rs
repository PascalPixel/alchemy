//! Differential parity harness for `tools/decomp/decomp_queue.ts`.
//!
//! Both mirrors are pointed at one fixture root and run over the real corpus:
//! real `asm/` rows, real `exact/` sources, a real ROM, a real toolchain. Every
//! invocation is observed as (exit code, normalised stdout, normalised report
//! JSON, normalised targets file) and compared field by field.
//!
//! Order of operations, which is the whole point of the harness:
//!   1. build the fixture and PROVE the guards fail on a broken one,
//!   2. run the NEGATIVE CONTROL and require it to be caught at exactly the
//!      planted index,
//!   3. run the cold comparison,
//!   4. run the WARM comparison and require byte-identical results.

use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};
use std::process::Command;

// ---------------------------------------------------------------------------
// Floors. Every one of these counts the TYPESCRIPT side only. A floor computed
// over the union of both mirrors passes when the Rust side alone produced the
// rows, which is exactly the failure a floor exists to catch.
// ---------------------------------------------------------------------------

const FLOOR_INVOCATIONS: usize = 8;
const FLOOR_CASES: usize = 120;
const FLOOR_EMITTED: usize = 30;
const FLOOR_DISTINCT_PAYLOADS: usize = 25;
const FLOOR_DOMINANT_CATEGORIES: usize = 2;
const FLOOR_FAILURES: usize = 2;
const FLOOR_CORPUS: usize = 30;

fn main() {
    if let Err(message) = run() {
        eprintln!("parity: {message}");
        std::process::exit(1);
    }
}

// ---------------------------------------------------------------------------
// Normalisation
// ---------------------------------------------------------------------------

/// Replace the only three things that legitimately vary between two runs of the
/// same input: the fixture root (a pid-suffixed absolute path), the ISO
/// timestamp `generated_at` writes, and the harness's own scratch directory.
///
/// Everything else is required to be byte-identical. In particular the report
/// paths, the stem order, the scores and the counts are NOT normalised.
fn normalize(text: &str, root: &Path) -> String {
    let mut out = text.replace(&root.display().to_string(), "<ROOT>");
    out = rewrite_timestamps(&out);
    out
}

/// Rewrite `2026-08-07T20:14:42.905Z` to `<TS>`. Hand-written because the
/// `regex` crate is not a dependency and this shape is fixed by
/// `Date.prototype.toISOString`, which is exactly 24 characters.
fn rewrite_timestamps(text: &str) -> String {
    let bytes: Vec<char> = text.chars().collect();
    let mut out = String::with_capacity(text.len());
    let mut index = 0;
    while index < bytes.len() {
        if is_iso_timestamp(&bytes[index..]) {
            out.push_str("<TS>");
            index += 24;
        } else {
            out.push(bytes[index]);
            index += 1;
        }
    }
    out
}

fn is_iso_timestamp(window: &[char]) -> bool {
    // YYYY-MM-DDTHH:MM:SS.mmmZ
    const SHAPE: &str = "ddddSddSddTddCddCdd.dddZ";
    if window.len() < 24 {
        return false;
    }
    SHAPE.chars().zip(window).all(|(kind, &actual)| match kind {
        'd' => actual.is_ascii_digit(),
        'S' => actual == '-',
        'C' => actual == ':',
        other => actual == other,
    })
}

// ---------------------------------------------------------------------------
// Observation
// ---------------------------------------------------------------------------

#[derive(Clone, PartialEq, Eq)]
struct Observation {
    exit: i32,
    stdout: String,
    first_error: String,
    report: String,
    targets: String,
}

impl Observation {
    fn fields(&self) -> [(&'static str, &str); 5] {
        [
            ("exit", ""),
            ("stdout", &self.stdout),
            ("first_error", &self.first_error),
            ("report", &self.report),
            ("targets", &self.targets),
        ]
    }
}

/// Bun prints ENOENT and SyntaxError with no `error: ` prefix, and its wording
/// is not the Rust mirror's wording. So the failure comparison is exit code
/// plus the offending path, never the prose.
fn first_error_line(stderr: &str) -> String {
    for line in stderr.lines() {
        let trimmed = line.trim();
        if trimmed.is_empty() {
            continue;
        }
        return trimmed.to_string();
    }
    String::new()
}

/// The comparable part of an error line: the exit code carries the verdict and
/// the path carries the cause. Prose is deliberately dropped.
fn error_signature(line: &str) -> String {
    let mut paths: Vec<&str> = line
        .split(|character: char| character.is_whitespace() || character == '\'' || character == '"')
        .filter(|token| token.contains('/') || token.starts_with("<ROOT>"))
        .collect();
    paths.sort_unstable();
    paths.dedup();
    paths.join(" ")
}

// ---------------------------------------------------------------------------
// Fixture
// ---------------------------------------------------------------------------

/// PORT NOTE: the preamble the corpus generator prepends is the one the real
/// `permute1` search emits, which is the M2C preamble WITHOUT the `M2C_FIELD`
/// macro. `candidateFor` checks `startsWith("typedef")`, so this body is used
/// verbatim and the `M2C_PREAMBLE + body` branch is exercised by the drafts
/// that do not start with a typedef.
const STATE_PREAMBLE: &str = "typedef signed char s8;\ntypedef unsigned char u8;\ntypedef signed short s16;\ntypedef unsigned short u16;\ntypedef signed int s32;\ntypedef unsigned int u32;\ntypedef signed long long s64;\ntypedef unsigned long long u64;\ntypedef int bool;\n#define NULL ((void *)0)\n\n";

struct Fixture {
    root: PathBuf,
    corpus: usize,
}

fn repo() -> PathBuf {
    PathBuf::from(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("the crate sits two levels under the repository root")
        .to_path_buf()
}

fn shell(command: &str) -> Result<(), String> {
    let status = Command::new("/bin/sh")
        .arg("-c")
        .arg(command)
        .status()
        .map_err(|error| format!("cannot spawn sh: {error}"))?;
    if status.success() {
        Ok(())
    } else {
        Err(format!("command failed: {command}"))
    }
}

/// Build a pid-suffixed fixture root. Nothing is ever deleted: a fresh pid is a
/// fresh directory, so every run starts cold without a single `rm`.
fn build_fixture(limit: usize) -> Result<Fixture, String> {
    let repo = repo();
    let root = repo
        .join("tools-rs")
        .join("decomp-queue")
        .join("target")
        .join("fixtures")
        .join(format!("parity.{}", std::process::id()));
    std::fs::create_dir_all(&root).map_err(|error| error.to_string())?;

    // `tools/` must be a real directory: the TypeScript derives ROOT from
    // `import.meta.url`, and a symlink would resolve back to the repository.
    shell(&format!(
        "cp -R {repo}/tools {root}/tools",
        repo = repo.display(),
        root = root.display()
    ))?;
    for entry in [
        "roms",
        "alchemy-gcc",
        "node_modules",
        "include",
        "semantic",
        "exact",
        "toolchain",
        "assets",
        "games",
        "canonical",
        "metrics",
        "work",
        "package.json",
        "tsconfig.json",
        "bun.lock",
        "baserom.gba",
        "rom.sha1",
    ] {
        let source = repo.join(entry);
        if source.exists() {
            std::os::unix::fs::symlink(&source, root.join(entry))
                .map_err(|error| format!("link {entry}: {error}"))?;
        }
    }
    // `asm/` is a real copy, not a link: the corpus needs a stub row for each
    // synthesised stem, and the repository's own `asm/` is a tracked path that
    // must never be written to.
    shell(&format!(
        "cp -R {repo}/asm {root}/asm",
        repo = repo.display(),
        root = root.display()
    ))?;
    std::fs::create_dir_all(root.join("out")).map_err(|error| error.to_string())?;
    if repo.join("out").join("full").exists() {
        std::os::unix::fs::symlink(repo.join("out").join("full"), root.join("out").join("full"))
            .map_err(|error| error.to_string())?;
    }

    let corpus = write_corpus(&repo, &root, limit)?;
    Ok(Fixture { root, corpus })
}

/// Synthesise `out/permute1/state/*.json` from the real `exact/` sources, which
/// is the shape `tools/search/permute_v1.ts` leaves behind. Only sources that
/// pull `types.h` are eligible: the others declare their own typedefs and would
/// collide with the preamble, which is a fixture defect, not a mirror defect.
fn write_corpus(repo: &Path, root: &Path, limit: usize) -> Result<usize, String> {
    let states = root.join("out").join("permute1").join("state");
    std::fs::create_dir_all(&states).map_err(|error| error.to_string())?;

    let mut stems: Vec<String> = Vec::new();
    let entries = std::fs::read_dir(repo.join("exact"))
        .map_err(|error| format!("cannot read exact/: {error}"))?;
    for entry in entries {
        let name = entry.map_err(|error| error.to_string())?.file_name();
        let name = name.to_string_lossy().into_owned();
        let Some(stem) = name.strip_suffix(".c") else {
            continue;
        };
        stems.push(stem.to_string());
    }
    stems.sort();

    let mut written = 0;
    for stem in stems {
        if written == limit {
            break;
        }
        let source = std::fs::read_to_string(repo.join("exact").join(format!("{stem}.c")))
            .map_err(|error| error.to_string())?;
        if !source.contains("#include \"types.h\"") {
            continue;
        }
        // Every third draft gets a one-character mutation. It still compiles,
        // but it no longer matches the ROM, which is what puts entries into the
        // non-`exact` diagnosis categories and gives `byte_mismatches` a
        // nonzero value to rank on. Without it the whole corpus is exact and
        // the comparison never exercises the scoring formula's interesting arm.
        let stripped = source.replace("#include \"types.h\"\n\n", "");
        let mutated = if written % 3 == 2 {
            mutate_once(&stripped)
        } else {
            stripped
        };
        let body = format!("{STATE_PREAMBLE}{mutated}");
        // Rounds drive the `plateau` and `improvement` arms of expectedValue.
        // All four shapes appear: none, a plateau, an improvement, and a round
        // whose `before` is absent so `Number.isFinite(last.before)` is false.
        let rounds = match written % 4 {
            0 => "[]".to_string(),
            1 => "[{\"before\": 20, \"after\": 22}, {\"before\": 22, \"after\": 22}]".to_string(),
            2 => "[{\"before\": 40, \"after\": 30}, {\"before\": 30, \"after\": 12}]".to_string(),
            _ => "[{\"after\": 9}]".to_string(),
        };
        std::fs::write(states.join(format!("{stem}.json")), state)
            .map_err(|error| error.to_string())?;
        // `decomp_queue.ts` skips any stem without an `asm/<stem>.s`, so the
        // fixture gets a stub row. It is deliberately not a `.4byte` table:
        // `retainedAssemblyStems()` must not claim it.
        let row = root.join("asm").join(format!("{stem}.s"));
        if !row.exists() {
            std::fs::write(&row, "@ fixture stub\n").map_err(|error| error.to_string())?;
        }
        written += 1;
    }
    Ok(written)
}

/// Change the first standalone decimal literal in the draft. Deterministic, so
/// two runs of the harness synthesise byte-identical corpora.
fn mutate_once(source: &str) -> String {
    let bytes: Vec<char> = source.chars().collect();
    let mut out = String::with_capacity(source.len());
    let mut done = false;
    let mut index = 0;
    while index < bytes.len() {
        let previous = if index == 0 { ' ' } else { bytes[index - 1] };
        let alone = !previous.is_ascii_alphanumeric() && previous != '_' && previous != 'x';
        if !done && alone && bytes[index].is_ascii_digit() {
            let digit = bytes[index].to_digit(10).unwrap_or(0);
            out.push(char::from_digit((digit + 3) % 10, 10).unwrap_or('7'));
            done = true;
        } else {
            out.push(bytes[index]);
        }
        index += 1;
    }
    out
}

fn json_string(text: &str) -> String {
    let mut out = String::from("\"");
    for character in text.chars() {
        match character {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            other if (other as u32) < 0x20 => {
                out.push_str(&format!("\\u{:04x}", other as u32));
            }
            other => out.push(other),
        }
    }
    out.push('"');
    out
}

// ---------------------------------------------------------------------------
// Running the two mirrors
// ---------------------------------------------------------------------------

fn rust_binary() -> Result<PathBuf, String> {
    let here = std::env::current_exe().map_err(|error| error.to_string())?;
    let directory = here
        .parent()
        .ok_or_else(|| "the harness binary has no directory".to_string())?;
    let candidate = directory.join("decomp-queue");
    if candidate.exists() {
        return Ok(candidate);
    }
    Err(format!("built the harness but not {}", candidate.display()))
}

fn read_or_empty(path: &Path) -> String {
    std::fs::read_to_string(path).unwrap_or_default()
}

fn observe(
    root: &Path,
    binary: Option<&Path>,
    args: &[&str],
    targets: Option<&Path>,
) -> Result<Observation, String> {
    let report = root.join("out").join("decomp").join("queue.json");
    let _ = std::fs::remove_file(&report);
    if let Some(path) = targets {
        let _ = std::fs::remove_file(path);
    }

    let mut command = match binary {
        Some(path) => {
            let mut command = Command::new(path);
            command.env("DECOMP_QUEUE_ROOT", root);
            command.env("PERMUTE_M2C_ASM", root.join("asm"));
            command
        }
        None => {
            let mut command = Command::new("bun");
            command.arg("tools/decomp/decomp_queue.ts");
            command
        }
    };
    let output = command
        .args(args)
        .current_dir(root)
        .output()
        .map_err(|error| format!("cannot spawn the mirror: {error}"))?;

    let stdout = normalize(&String::from_utf8_lossy(&output.stdout), root);
    let stderr = normalize(&String::from_utf8_lossy(&output.stderr), root);
    Ok(Observation {
        exit: output.status.code().unwrap_or(-1),
        stdout,
        first_error: error_signature(&first_error_line(&stderr)),
        report: normalize(&read_or_empty(&report), root),
        targets: targets
            .map(|path| normalize(&read_or_empty(path), root))
            .unwrap_or_default(),
    })
}

// ---------------------------------------------------------------------------
// The comparison
// ---------------------------------------------------------------------------

struct Invocation {
    label: &'static str,
    args: Vec<String>,
    targets: bool,
}

fn invocations() -> Vec<Invocation> {
    let mut list = Vec::new();
    let mut add = |label: &'static str, args: &[&str], targets: bool| {
        list.push(Invocation {
            label,
            args: args.iter().map(|value| value.to_string()).collect(),
            targets,
        });
    };
    add("cold-refresh", &["--jobs", "8", "--refresh"], false);
    add("warm-reuse", &["--jobs", "8"], false);
    add("single-job", &["--jobs", "1", "--refresh"], false);
    add("limit-three", &["--limit", "3", "--refresh"], false);
    add("limit-one", &["--limit", "1", "--refresh"], false);
    add("targets-out", &["--jobs", "4", "--refresh"], true);
    add("help", &["--help"], false);
    add("bad-flag", &["--self-test"], false);
    add("bad-jobs", &["--jobs", "0"], false);
    add("bad-limit", &["--limit", "x"], false);
    list
}

struct Pass {
    observations: Vec<(String, Observation, Observation)>,
}

fn run_pass(fixture: &Fixture, binary: &Path) -> Result<Pass, String> {
    let mut observations = Vec::new();
    for invocation in invocations() {
        let ts_targets = fixture.root.join("out").join("t.ts.txt");
        let rs_targets = fixture.root.join("out").join("t.rs.txt");
        let (ts_path, rs_path) = if invocation.targets {
            (Some(ts_targets.as_path()), Some(rs_targets.as_path()))
        } else {
            (None, None)
        };
        let mut ts_args: Vec<&str> = invocation.args.iter().map(String::as_str).collect();
        let mut rs_args = ts_args.clone();
        let ts_flag = ts_targets.display().to_string();
        let rs_flag = rs_targets.display().to_string();
        if invocation.targets {
            ts_args.push("--targets-out");
            ts_args.push(&ts_flag);
            rs_args.push("--targets-out");
            rs_args.push(&rs_flag);
        }
        let typescript = observe(&fixture.root, None, &ts_args, ts_path)?;
        let rust = observe(&fixture.root, Some(binary), &rs_args, rs_path)?;
        observations.push((invocation.label.to_string(), typescript, rust));
    }
    Ok(Pass { observations })
}

/// Every (invocation, field) pair is one case, and each emitted queue item is
/// compared as its own case on top of that.
fn differences(pass: &Pass) -> (usize, Vec<String>) {
    let mut cases = 0;
    let mut failures = Vec::new();
    for (label, typescript, rust) in &pass.observations {
        cases += 1;
        if typescript.exit != rust.exit {
            failures.push(format!(
                "{label}/exit: typescript={} rust={}",
                typescript.exit, rust.exit
            ));
        }
        for ((name, left), (_, right)) in typescript.fields().iter().zip(rust.fields().iter()) {
            if *name == "exit" {
                continue;
            }
            cases += 1;
            if left != right {
                failures.push(format!("{label}/{name}: differs"));
            }
        }
        let left_items = items(&typescript.report);
        let right_items = items(&rust.report);
        for index in 0..left_items.len().max(right_items.len()) {
            cases += 1;
            if left_items.get(index) != right_items.get(index) {
                failures.push(format!("{label}/item[{index}]: differs"));
            }
        }
    }
    (cases, failures)
}

/// Split the canonical-JSON report into one string per queue item. The report
/// is written one key per line, so an item begins at each line whose content is
/// `{` inside the `items` array; splitting on the `"stem":` key is enough and
/// does not need a JSON parser.
fn items(report: &str) -> Vec<String> {
    // `diagnosis` carries its own nested `"stem"` key, so the split has to be
    // depth-aware: only a stem line at the SAME indentation as the first one
    // starts a new item. Splitting on the key alone doubles the item count and
    // quietly halves what the floors are measuring.
    let indent = report
        .lines()
        .find(|line| line.trim_start().starts_with("\"stem\":"))
        .map(|line| line.len() - line.trim_start().len());
    let Some(indent) = indent else {
        return Vec::new();
    };
    let mut result = Vec::new();
    let mut current: Option<String> = None;
    for line in report.lines() {
        let trimmed = line.trim_start();
        if trimmed.starts_with("\"stem\":") && line.len() - trimmed.len() == indent {
            if let Some(item) = current.take() {
                result.push(item);
            }
            current = Some(String::new());
        }
        if let Some(item) = current.as_mut() {
            item.push_str(line);
            item.push('\n');
        }
    }
    if let Some(item) = current {
        result.push(item);
    }
    result
}

fn field(item: &str, key: &str) -> String {
    let needle = format!("\"{key}\":");
    for line in item.lines() {
        let trimmed = line.trim_start();
        if let Some(rest) = trimmed.strip_prefix(&needle) {
            return rest.trim().trim_end_matches(',').trim_matches('"').to_string();
        }
    }
    String::new()
}

fn run() -> Result<(), String> {
    let binary = rust_binary()?;
    let fixture = build_fixture(36)?;
    println!("fixture root: {}", fixture.root.display());
    println!("corpus stems: {}", fixture.corpus);
    if fixture.corpus < FLOOR_CORPUS {
        return Err(format!(
            "corpus floor: {} stems, need {FLOOR_CORPUS}. Scanning nothing is not passing.",
            fixture.corpus
        ));
    }

    prove_hard_fails(&fixture, &binary)?;

    // ---- negative control, BEFORE the real comparison -------------------
    let control = run_pass(&fixture, &binary)?;
    let planted = 3usize;
    let mut poisoned = control;
    {
        let entry = poisoned
            .observations
            .get_mut(planted)
            .ok_or_else(|| "the planted index is past the end of the pass".to_string())?;
        entry.2.stdout.push_str("\nplanted divergence\n");
    }
    let (_, control_failures) = differences(&poisoned);
    if control_failures.len() != 1 {
        return Err(format!(
            "negative control: expected exactly one difference, saw {} ({control_failures:?})",
            control_failures.len()
        ));
    }
    let expected_label = invocations()[planted].label;
    if !control_failures[0].starts_with(&format!("{expected_label}/stdout")) {
        return Err(format!(
            "negative control: planted at {expected_label} but detected {}",
            control_failures[0]
        ));
    }
    println!("negative control: caught at {expected_label}, exactly one difference");

    // ---- cold pass ------------------------------------------------------
    let cold = run_pass(&fixture, &binary)?;
    let (cases, failures) = differences(&cold);
    report_floors(&cold, cases)?;
    if !failures.is_empty() {
        return Err(format!("cold pass: {} differences: {failures:?}", failures.len()));
    }
    println!("cold pass: {cases} cases, 0 differences");

    // ---- warm pass, required byte-identical ------------------------------
    let warm = run_pass(&fixture, &binary)?;
    let (warm_cases, warm_failures) = differences(&warm);
    if !warm_failures.is_empty() {
        return Err(format!("warm pass: {} differences", warm_failures.len()));
    }
    if warm_cases != cases {
        return Err(format!("warm pass changed case count: {cases} -> {warm_cases}"));
    }
    for ((label, cold_ts, cold_rs), (_, warm_ts, warm_rs)) in
        cold.observations.iter().zip(warm.observations.iter())
    {
        if cold_ts != warm_ts || cold_rs != warm_rs {
            return Err(format!("warm pass is not byte-identical to cold at {label}"));
        }
    }
    println!("warm pass: {warm_cases} cases, byte-identical to cold");
    Ok(())
}

fn report_floors(pass: &Pass, cases: usize) -> Result<(), String> {
    let mut emitted = 0usize;
    let mut payloads: BTreeSet<String> = BTreeSet::new();
    let mut dominants: BTreeMap<String, usize> = BTreeMap::new();
    let mut failures = 0usize;
    for (_, typescript, _) in &pass.observations {
        if typescript.exit != 0 {
            failures += 1;
        }
        for item in items(&typescript.report) {
            let value: f64 = field(&item, "expected_value").parse().unwrap_or(f64::NAN);
            if value.is_finite() && value > 0.0 {
                emitted += 1;
            }
            payloads.insert(item.clone());
            *dominants.entry(field(&item, "dominant")).or_default() += 1;
        }
    }
    println!(
        "typescript-side: invocations={} cases={cases} emitted={emitted} distinct_payloads={} dominant_categories={} failures={failures}",
        pass.observations.len(),
        payloads.len(),
        dominants.len()
    );
    for (kind, count) in &dominants {
        println!("  dominant {kind}: {count}");
    }
    let checks: [(&str, usize, usize); 6] = [
        ("invocations", pass.observations.len(), FLOOR_INVOCATIONS),
        ("cases", cases, FLOOR_CASES),
        ("emitted", emitted, FLOOR_EMITTED),
        ("distinct_payloads", payloads.len(), FLOOR_DISTINCT_PAYLOADS),
        ("dominant_categories", dominants.len(), FLOOR_DOMINANT_CATEGORIES),
        ("failures", failures, FLOOR_FAILURES),
    ];
    for (name, actual, floor) in checks {
        if actual < floor {
            return Err(format!("floor {name}: {actual} < {floor}"));
        }
    }
    Ok(())
}

// ---------------------------------------------------------------------------
// Hard-fail proofs
// ---------------------------------------------------------------------------

/// Scanning nothing is not passing. Each of these builds a deliberately broken
/// environment and requires BOTH mirrors to exit nonzero. If a mirror shrugs
/// and reports success on a missing corpus, a missing toolchain, a missing
/// `bun` or an unreadable ROM, the whole harness is measuring nothing.
fn prove_hard_fails(fixture: &Fixture, binary: &Path) -> Result<(), String> {
    let empty = fixture.root.parent().unwrap().join(format!(
        "empty.{}",
        std::process::id()
    ));
    std::fs::create_dir_all(empty.join("tools")).map_err(|error| error.to_string())?;
    shell(&format!(
        "cp -R {}/tools/. {}/tools/",
        fixture.root.display(),
        empty.display()
    ))?;
    std::os::unix::fs::symlink(fixture.root.join("node_modules"), empty.join("node_modules"))
        .map_err(|error| error.to_string())?;

    // 1. empty corpus: no out/permute1/state at all.
    let ts = observe(&empty, None, &["--refresh"], None)?;
    let rs = observe(&empty, Some(binary), &["--refresh"], None)?;
    if ts.exit == 0 || rs.exit == 0 {
        return Err(format!(
            "empty corpus did not hard-fail: typescript={} rust={}",
            ts.exit, rs.exit
        ));
    }
    println!("hard-fail proven: empty corpus (typescript={}, rust={})", ts.exit, rs.exit);

    // 2. missing ROM and missing toolchain: the corpus is present, but the
    //    fixture has neither roms/ nor toolchain/, so verification cannot run.
    let stripped = fixture.root.parent().unwrap().join(format!(
        "noroms.{}",
        std::process::id()
    ));
    std::fs::create_dir_all(&stripped).map_err(|error| error.to_string())?;
    shell(&format!(
        "cp -R {}/tools {}/tools",
        fixture.root.display(),
        stripped.display()
    ))?;
    for entry in ["node_modules", "asm", "exact", "include", "alchemy-gcc"] {
        let source = fixture.root.join(entry);
        if source.exists() {
            std::os::unix::fs::symlink(&source, stripped.join(entry))
                .map_err(|error| error.to_string())?;
        }
    }
    shell(&format!(
        "mkdir -p {stripped}/out && cp -R {root}/out/permute1 {stripped}/out/permute1",
        stripped = stripped.display(),
        root = fixture.root.display()
    ))?;
    let ts = observe(&stripped, None, &["--refresh"], None)?;
    let rs = observe(&stripped, Some(binary), &["--refresh"], None)?;
    if ts.exit == 0 || rs.exit == 0 {
        return Err(format!(
            "unreadable ROM did not hard-fail: typescript={} rust={}",
            ts.exit, rs.exit
        ));
    }
    println!("hard-fail proven: unreadable ROM and missing toolchain (typescript={}, rust={})", ts.exit, rs.exit);

    // 3. missing `bun`: the Rust mirror shells out to it to verify candidates.
    //    PORT NOTE: neither mirror exits nonzero here, and that is faithful --
    //    `main` wraps each target in `catch { console.error(`skip ...`) }`, so a
    //    toolchain that cannot run degrades into an EMPTY queue rather than an
    //    error. That is precisely the "scanning nothing" failure, so the guard
    //    lives in the harness: the run must produce zero ranked entries, and the
    //    emitted floor must REJECT such a pass. Both halves are triggered here.
    let output = Command::new(binary)
        .args(["--refresh"])
        .env("DECOMP_QUEUE_ROOT", &fixture.root)
        .env("PERMUTE_M2C_ASM", fixture.root.join("asm"))
        .env("PATH", "/nonexistent")
        .current_dir(&fixture.root)
        .output()
        .map_err(|error| error.to_string())?;
    let starved = Observation {
        exit: output.status.code().unwrap_or(-1),
        stdout: normalize(&String::from_utf8_lossy(&output.stdout), &fixture.root),
        first_error: String::new(),
        report: normalize(
            &read_or_empty(&fixture.root.join("out").join("decomp").join("queue.json")),
            &fixture.root,
        ),
        targets: String::new(),
    };
    let ranked = items(&starved.report).len();
    if ranked != 0 {
        return Err(format!(
            "missing bun still ranked {ranked} entries, so the fixture is not using it"
        ));
    }
    let starved_pass = Pass {
        observations: vec![("starved".to_string(), starved.clone(), starved)],
    };
    if report_floors(&starved_pass, 999_999).is_ok() {
        return Err("the emitted floor accepted a run that ranked nothing".to_string());
    }
    println!("hard-fail proven: missing bun ranks zero entries and the emitted floor rejects it");
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn the_normaliser_leaves_a_message_without_the_varying_part_alone() {
        let root = PathBuf::from("/fixtures/parity.999");
        let plain = "queued=34 plateaus=8 register_led=1\n080039bc\tvalue=1051.4\tbytes=0\texact";
        assert_eq!(normalize(plain, &root), plain, "nothing varies here");
    }

    #[test]
    fn the_normaliser_rewrites_only_the_root_and_the_timestamp() {
        let root = PathBuf::from("/fixtures/parity.999");
        let text = "report=/fixtures/parity.999/out/decomp/queue.json at 2026-08-07T20:14:42.905Z";
        assert_eq!(
            normalize(text, &root),
            "report=<ROOT>/out/decomp/queue.json at <TS>"
        );
    }

    #[test]
    fn a_near_timestamp_is_not_rewritten() {
        // One digit short of the ISO shape, and a date with no time part.
        assert_eq!(rewrite_timestamps("2026-08-07T20:14:42.90Z"), "2026-08-07T20:14:42.90Z");
        assert_eq!(rewrite_timestamps("2026-08-07"), "2026-08-07");
    }

    #[test]
    fn the_error_signature_keeps_the_path_and_drops_the_prose() {
        let bun = "ENOENT: no such file or directory, scandir '<ROOT>/out/permute1/state'";
        let rust = "error: ENOENT: <ROOT>/out/permute1/state: No such file (os error 2)";
        assert_eq!(error_signature(bun), error_signature(rust));
        assert_eq!(error_signature(bun), "<ROOT>/out/permute1/state");
    }

    #[test]
    fn items_split_on_the_stem_key() {
        let report = "{\n\"items\": [\n{\n\"stem\": \"a\",\n\"x\": 1\n},\n{\n\"stem\": \"b\",\n\"x\": 2\n}\n]\n}";
        let split = items(report);
        assert_eq!(split.len(), 2);
        assert_eq!(field(&split[0], "stem"), "a");
        assert_eq!(field(&split[1], "x"), "2");
    }
}
