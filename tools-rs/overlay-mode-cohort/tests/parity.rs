//! Differential parity harness: this crate against the live TypeScript at
//! `tools/overlay/overlay_mode_cohort.ts`.
//!
//! The corpus is REAL repository overlays: every `semantic/**/*_c_020*.c` whose
//! derived id is present in the regenerated overlay inventory. Both mirrors are
//! driven over the whole corpus in one invocation each and their `report.json`
//! documents plus their stdout are compared row by row.
//!
//! SCANNING NOTHING IS NOT PASSING. Every precondition below hard-fails: an
//! empty or short corpus, a missing `bun`, a missing ARM binutil, an unreadable
//! inventory, or a mirror that exits non-zero. A green run therefore cannot mean
//! "nothing was examined".
//!
//! ALL artifacts are written under `tools-rs/overlay-mode-cohort/target/fixtures/`.
//! Nothing here writes to the repository's `out/`, even though the TypeScript's
//! default output directory lives there; both mirrors are given an explicit
//! `-o` inside the fixtures tree.

use search_compiler_modes::{canonical_json, parse_json, Json};
use std::collections::BTreeSet;
use std::path::{Path, PathBuf};
use std::process::Command;

// Floors are counted on the TYPESCRIPT side specifically, never on the union of
// the two mirrors. A union floor passes on an emptied source as long as the
// other mirror still produces rows, which is exactly the hole that let a
// sibling agent's gate go green over nothing.
const CANDIDATE_FLOOR: usize = 400;
const ROW_FLOOR: usize = 2000;
const DISTINCT_PAYLOAD_FLOOR: usize = 2000;
const COMPILED_FLOOR: usize = 2000;
const ERROR_ROW_FLOOR: usize = 10;
const DISTINCT_ERROR_FLOOR: usize = 5;
const EXACT_FLOOR: usize = 1;
const FIRST_DIFFERENCE_FLOOR: usize = 2000;
const DISTINCT_FIRST_DIFFERENCE_FLOOR: usize = 20;
const DISTINCT_EXPECTED_SIZE_FLOOR: usize = 100;
const FAMILY_COUNT: usize = 5;
const BEST_BY_REGION_FLOOR: usize = 400;

fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("crate sits two levels below the repository root")
        .to_path_buf()
}

fn fixtures() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("target/fixtures")
}

fn on_path(program: &str) -> bool {
    Command::new("which")
        .arg(program)
        .output()
        .map(|output| output.status.success())
        .unwrap_or(false)
}

/// The two normalisations, and ONLY these two.
///
/// `cache_key` differs BY DESIGN: the TypeScript folds its own `.ts` bytes into
/// the key and this crate folds its own Rust sources, deliberately putting the
/// two mirrors in different key spaces so that neither can serve the other a
/// stale row. Sharing one key space between two implementations that can
/// disagree is how a single stale `overlay-c-v3` entry once made `main` look red
/// across checkouts.
///
/// The scratch directory name is a function of that same key, so a compiler
/// error message quoting the path it was handed inherits the difference. That is
/// the only place a key leaks into a payload, and it is replaced with a literal
/// rather than deleted, so an error that lost its path would still be a
/// difference.
fn normalise(row: &mut Json) {
    if let Json::Object(entries) = row {
        for (key, value) in entries.iter_mut() {
            if key == "cache_key" {
                *value = Json::String("<normalised-by-design>".into());
            }
            if key == "error" {
                if let Json::String(text) = value {
                    *value = Json::String(strip_scratch(text));
                }
            }
        }
    }
}

/// Replace `<...>/fixtures/<mirror>/scratch/<64 hex>/` with a fixed literal.
fn strip_scratch(text: &str) -> String {
    let mut out = String::new();
    let mut rest = text;
    while let Some(index) = rest.find("/scratch/") {
        let (head, tail) = rest.split_at(index);
        // Walk back over the mirror directory name to the fixtures segment so
        // `ts-full` and `rs-full` both vanish.
        let head = match head.rfind("/fixtures/") {
            Some(cut) => {
                out.push_str(&head[..cut]);
                ""
            }
            None => head,
        };
        out.push_str(head);
        let tail = &tail["/scratch/".len()..];
        let digest_end = tail
            .find('/')
            .expect("a scratch path always carries a trailing component");
        out.push_str("<scratch>/");
        rest = &tail[digest_end + 1..];
    }
    out.push_str(rest);
    out
}

fn results(report: &Json) -> Vec<Json> {
    match report.get("results") {
        Some(Json::Array(rows)) => rows.clone(),
        _ => panic!("report has no results array"),
    }
}

fn field<'a>(row: &'a Json, key: &str) -> Option<&'a Json> {
    row.get(key)
}

fn read_report(directory: &Path) -> Json {
    let path = directory.join("report.json");
    let text = std::fs::read_to_string(&path)
        .unwrap_or_else(|error| panic!("unreadable report at {}: {error}", path.display()));
    parse_json(&text).unwrap_or_else(|error| panic!("unparsable report: {error}"))
}

fn candidate_arguments(corpus: &[String], inventory: &Path, output: &Path) -> Vec<String> {
    let mut argv: Vec<String> = Vec::new();
    for spec in corpus {
        argv.push("-c".into());
        argv.push(spec.clone());
    }
    argv.push("--inventory".into());
    argv.push(inventory.to_string_lossy().into_owned());
    argv.push("-o".into());
    argv.push(output.to_string_lossy().into_owned());
    argv
}

#[test]
fn the_two_mirrors_agree_over_the_real_overlay_corpus() {
    // ---- preconditions, every one of them fatal -------------------------
    assert!(
        on_path("bun"),
        "bun is not on PATH, so the TypeScript side cannot run; a parity harness that cannot \
         reach its reference has not compared anything"
    );
    for binutil in [
        "arm-none-eabi-as",
        "arm-none-eabi-ld",
        "arm-none-eabi-nm",
        "arm-none-eabi-objcopy",
    ] {
        assert!(on_path(binutil), "{binutil} is not on PATH");
    }

    let fixtures = fixtures();
    let corpus_path = fixtures.join("corpus.txt");
    let corpus_text = std::fs::read_to_string(&corpus_path).unwrap_or_else(|error| {
        panic!(
            "unreadable corpus at {}: {error}. Regenerate it before running parity.",
            corpus_path.display()
        )
    });
    let corpus: Vec<String> = corpus_text
        .lines()
        .map(str::trim)
        .filter(|line| !line.is_empty())
        .map(str::to_string)
        .collect();
    assert!(
        corpus.len() >= CANDIDATE_FLOOR,
        "corpus holds {} candidates, floor is {CANDIDATE_FLOOR}",
        corpus.len()
    );
    for spec in corpus.iter() {
        let source = spec
            .split_once('=')
            .expect("every corpus line is id=path")
            .1;
        assert!(
            root().join(source).exists(),
            "corpus names a source that does not exist: {source}"
        );
    }

    let inventory = fixtures.join("overlays.json");
    let inventory_text = std::fs::read_to_string(&inventory)
        .unwrap_or_else(|error| panic!("unreadable inventory: {error}"));
    let parsed_inventory = parse_json(&inventory_text).expect("inventory parses");
    let inventory_rows = match parsed_inventory.get("functions") {
        Some(Json::Array(rows)) => rows.len(),
        _ => 0,
    };
    assert!(
        inventory_rows > 1000,
        "inventory holds only {inventory_rows} functions"
    );

    // ---- run both mirrors ------------------------------------------------
    //
    // The output directories are suffixed with this process's pid so every run
    // starts COLD without deleting anything. Reusing a fixed directory made an
    // earlier attempt of this harness compare a warm TypeScript cache against a
    // cold Rust one, and every row differed on `cached`. Nothing here removes a
    // path: `target/` is disposable, and building a deletion path from a
    // variable is how a previous agent destroyed live build caches.
    let stamp = std::process::id();
    let typescript_output = fixtures.join(format!("parity-ts-{stamp}"));
    let rust_output = fixtures.join(format!("parity-rs-{stamp}"));

    // The Rust side runs as a SUBPROCESS rather than through `run()` in-process
    // so both mirrors resolve relative candidate paths against the same working
    // directory without this test mutating a process-global cwd, and so the
    // comparison is stdout against stdout.
    let run_typescript = || -> String {
        let output = Command::new("bun")
            .arg(root().join("tools/overlay/overlay_mode_cohort.ts"))
            .args(candidate_arguments(&corpus, &inventory, &typescript_output))
            .current_dir(root())
            .output()
            .expect("bun runs");
        assert!(
            output.status.success(),
            "the TypeScript mirror exited {:?}: {}",
            output.status.code(),
            String::from_utf8_lossy(&output.stderr)
        );
        String::from_utf8_lossy(&output.stdout).into_owned()
    };
    let run_rust = || -> String {
        let output = Command::new(env!("CARGO_BIN_EXE_overlay-mode-cohort"))
            .args(candidate_arguments(&corpus, &inventory, &rust_output))
            .current_dir(root())
            .output()
            .expect("the Rust mirror runs");
        assert!(
            output.status.success(),
            "the Rust mirror exited {:?}: {}",
            output.status.code(),
            String::from_utf8_lossy(&output.stderr)
        );
        String::from_utf8_lossy(&output.stdout).into_owned()
    };

    let typescript_stdout = run_typescript();
    let rust_stdout = run_rust();
    let rust_lines: Vec<&str> = rust_stdout.lines().collect();

    let typescript_report = read_report(&typescript_output);
    let rust_report = read_report(&rust_output);

    let mut typescript_rows = results(&typescript_report);
    let mut rust_rows = results(&rust_report);
    for row in typescript_rows.iter_mut() {
        normalise(row);
    }
    for row in rust_rows.iter_mut() {
        normalise(row);
    }

    // ---- floors, counted on the TYPESCRIPT side --------------------------
    assert!(
        typescript_rows.len() >= ROW_FLOOR,
        "the TypeScript produced {} rows, floor is {ROW_FLOOR}",
        typescript_rows.len()
    );
    let distinct: BTreeSet<String> = typescript_rows.iter().map(canonical_json).collect();
    assert!(
        distinct.len() >= DISTINCT_PAYLOAD_FLOOR,
        "only {} distinct TypeScript payloads, floor is {DISTINCT_PAYLOAD_FLOOR}",
        distinct.len()
    );
    let count = |predicate: &dyn Fn(&Json) -> bool| -> usize {
        typescript_rows.iter().filter(|row| predicate(row)).count()
    };
    let compiled = count(&|row| matches!(field(row, "compiled"), Some(Json::Bool(true))));
    let exact = count(&|row| matches!(field(row, "exact"), Some(Json::Bool(true))));
    let errored = count(&|row| field(row, "error").is_some());
    let with_first_difference = count(&|row| field(row, "first_difference").is_some());
    assert!(compiled >= COMPILED_FLOOR, "compiled rows: {compiled}");
    assert!(exact >= EXACT_FLOOR, "exact rows: {exact}");
    assert!(errored >= ERROR_ROW_FLOOR, "error rows: {errored}");
    assert!(
        with_first_difference >= FIRST_DIFFERENCE_FLOOR,
        "rows carrying first_difference: {with_first_difference}"
    );

    let distinct_of = |key: &str| -> usize {
        typescript_rows
            .iter()
            .filter_map(|row| field(row, key).map(canonical_json))
            .collect::<BTreeSet<String>>()
            .len()
    };
    assert!(
        distinct_of("error") >= DISTINCT_ERROR_FLOOR,
        "distinct error texts: {}",
        distinct_of("error")
    );
    assert!(
        distinct_of("first_difference") >= DISTINCT_FIRST_DIFFERENCE_FLOOR,
        "distinct first_difference offsets: {}",
        distinct_of("first_difference")
    );
    assert!(
        distinct_of("expected_size") >= DISTINCT_EXPECTED_SIZE_FLOOR,
        "distinct expected sizes: {}",
        distinct_of("expected_size")
    );
    let families: BTreeSet<String> = typescript_rows
        .iter()
        .filter_map(|row| row.get("config").and_then(|c| c.get("compiler_family")))
        .map(canonical_json)
        .collect();
    assert_eq!(
        families.len(),
        FAMILY_COUNT,
        "compiler families exercised: {families:?}"
    );
    let best_by_region = match typescript_report.get("best_by_region") {
        Some(Json::Array(rows)) => rows.iter().filter(|row| !matches!(row, Json::Null)).count(),
        _ => 0,
    };
    assert!(
        best_by_region >= BEST_BY_REGION_FLOOR,
        "regions with a winner: {best_by_region}"
    );

    // ---- NEGATIVE CONTROL, run BEFORE the real comparison -----------------
    //
    // Perturb exactly one field of one row of a COPY of the Rust side and prove
    // the comparison reports exactly one difference. A comparison that cannot
    // see a planted difference cannot testify to the absence of a real one.
    {
        let mut perturbed = rust_rows.clone();
        let victim = perturbed.len() / 2;
        if let Json::Object(entries) = &mut perturbed[victim] {
            let slot = entries
                .iter_mut()
                .find(|(key, _)| key == "expected_size")
                .expect("every row carries expected_size");
            slot.1 = match &slot.1 {
                Json::Number(size) => Json::Number(size + 1.0),
                other => panic!("expected_size was {other:?}"),
            };
        }
        let differences = typescript_rows
            .iter()
            .zip(perturbed.iter())
            .filter(|(left, right)| canonical_json(left) != canonical_json(right))
            .count();
        assert_eq!(
            differences, 1,
            "the negative control must yield exactly one difference, not {differences}"
        );
    }

    // ---- the real comparison ---------------------------------------------
    assert_eq!(
        typescript_rows.len(),
        rust_rows.len(),
        "row counts diverge before any field is compared"
    );
    let mut mismatches: Vec<String> = Vec::new();
    for (index, (left, right)) in typescript_rows.iter().zip(rust_rows.iter()).enumerate() {
        let left_text = canonical_json(left);
        let right_text = canonical_json(right);
        if left_text != right_text {
            mismatches.push(format!("row {index}:\n  ts: {left_text}\n  rs: {right_text}"));
        }
    }
    assert!(
        mismatches.is_empty(),
        "{} of {} rows differ:\n{}",
        mismatches.len(),
        typescript_rows.len(),
        mismatches
            .iter()
            .take(5)
            .cloned()
            .collect::<Vec<_>>()
            .join("\n")
    );

    // Everything outside `results` must match verbatim; none of it carries a
    // cache key or a path, so no normalisation applies.
    for key in [
        "format",
        "policy",
        "candidates",
        "configurations",
        "attempted",
        "compiled",
        "exact",
        "shared",
    ] {
        assert_eq!(
            typescript_report.get(key).map(canonical_json),
            rust_report.get(key).map(canonical_json),
            "report key {key} differs"
        );
    }

    // `best_by_region` republishes whole score rows, so it carries the same
    // by-design cache-key difference and takes the same normalisation.
    let winners = |report: &Json| -> Vec<String> {
        match report.get("best_by_region") {
            Some(Json::Array(rows)) => rows
                .iter()
                .map(|row| {
                    let mut row = row.clone();
                    normalise(&mut row);
                    canonical_json(&row)
                })
                .collect(),
            _ => panic!("report has no best_by_region array"),
        }
    };
    assert_eq!(
        winners(&typescript_report),
        winners(&rust_report),
        "best_by_region differs"
    );

    // Console output, line for line. The `report=` line names the output
    // directory, which the two mirrors are deliberately given differently, so
    // it is compared on its prefix only.
    let typescript_console: Vec<&str> = typescript_stdout.lines().collect();
    assert_eq!(
        typescript_console.len(),
        rust_lines.len(),
        "console line counts differ"
    );
    for (left, right) in typescript_console.iter().zip(rust_lines.iter()) {
        if left.starts_with("report=") {
            assert!(right.starts_with("report="), "report line: {right}");
            continue;
        }
        assert_eq!(left, right, "console line differs");
    }

    // ---- the WARM pass ----------------------------------------------------
    //
    // The first pass was cold, so it proves nothing about the disk cache, which
    // is the riskiest thing in this file. Running both mirrors a second time
    // into the same directories exercises `acceptedScore` and the
    // `{ ...cached, cached: true }` spread, and the rows must still agree.
    let cold_cached = typescript_rows
        .iter()
        .filter(|row| matches!(field(row, "cached"), Some(Json::Bool(true))))
        .count();
    assert_eq!(cold_cached, 0, "the first pass was not cold");

    let warm_typescript_stdout = run_typescript();
    let warm_rust_stdout = run_rust();
    let mut warm_typescript = results(&read_report(&typescript_output));
    let mut warm_rust = results(&read_report(&rust_output));
    for row in warm_typescript.iter_mut() {
        normalise(row);
    }
    for row in warm_rust.iter_mut() {
        normalise(row);
    }
    let warm_cached = warm_typescript
        .iter()
        .filter(|row| matches!(field(row, "cached"), Some(Json::Bool(true))))
        .count();
    assert_eq!(
        warm_cached,
        warm_typescript.len(),
        "the second pass did not hit the cache, so the cache path was never compared"
    );
    let warm_mismatches = warm_typescript
        .iter()
        .zip(warm_rust.iter())
        .filter(|(left, right)| canonical_json(left) != canonical_json(right))
        .count();
    assert_eq!(warm_mismatches, 0, "the warm pass diverged");
    assert_eq!(
        warm_typescript_stdout.lines().count(),
        warm_rust_stdout.lines().count(),
        "warm console line counts differ"
    );

    eprintln!(
        "parity: {} candidates, {} rows, {} distinct payloads, {compiled} compiled, {exact} exact, \
         {errored} errored ({} distinct texts), {with_first_difference} with a first difference, \
         {best_by_region} regions with a winner, {} families, 0 mismatches",
        corpus.len(),
        typescript_rows.len(),
        distinct.len(),
        distinct_of("error"),
        families.len()
    );
}

#[test]
fn the_scratch_normaliser_removes_the_path_and_nothing_else() {
    let text = "xgcc failed: In file included from \
                /repo/tools-rs/overlay-mode-cohort/target/fixtures/ts-full/scratch/\
                aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa/02001ca8.c:2:\n\
                /repo/semantic/x.c:101: parse error";
    let stripped = strip_scratch(text);
    assert!(stripped.contains("<scratch>/02001ca8.c:2:"));
    assert!(stripped.contains("/repo/semantic/x.c:101: parse error"));
    assert!(!stripped.contains("ts-full"));
    // A message with no scratch path is returned untouched, so a mirror that
    // stopped emitting the path would still register as a difference.
    assert_eq!(strip_scratch("plain error"), "plain error");
}
