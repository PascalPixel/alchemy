//! Differential parity harness: the ported aggregations against the live
//! TypeScript in `tools/compiler/mode_cohort.ts`.
//!
//! The pipeline `main` drives cannot be compared, because `mode_sweep` dies in
//! `compilerSignature()` before it compiles anything (see the crate docs). What
//! CAN be compared is the entire pure layer, which is all five exported
//! functions and every sort, tiebreak and emission rule in them. This harness
//! generates corpora, runs both implementations over them, and compares the
//! canonical JSON byte for byte.
//!
//! Rules this harness enforces on itself:
//!  * A missing `bun`, a failed driver, or an empty corpus HARD-FAILS. Scanning
//!    nothing is not passing.
//!  * Floors on cases run, on DISTINCT payloads (so a degenerate corpus that
//!    produces one answer over and over cannot pass), and per category on the
//!    number of rows FOUND, so the gate cannot go green because an aggregation
//!    silently stopped matching.
//!  * A NEGATIVE CONTROL runs BEFORE the real comparison: one deliberate
//!    perturbation must produce exactly one difference. If no corpus is
//!    sensitive to it, the harness fails rather than reporting a clean sweep.

use mode_cohort::{all_aggregations, category_counts, Report};
use search_compiler_modes::{canonical_json, parse_json, Json};
use std::path::PathBuf;
use std::process::Command;

const CORPUS_COUNT: usize = 240;

// ---------------------------------------------------------------------------
// Deterministic corpus generation
// ---------------------------------------------------------------------------

struct Rng(u64);

impl Rng {
    fn next(&mut self) -> u64 {
        // SplitMix64.
        self.0 = self.0.wrapping_add(0x9e37_79b9_7f4a_7c15);
        let mut z = self.0;
        z = (z ^ (z >> 30)).wrapping_mul(0xbf58_476d_1ce4_e5b9);
        z = (z ^ (z >> 27)).wrapping_mul(0x94d0_49bb_1331_11eb);
        z ^ (z >> 31)
    }

    fn below(&mut self, bound: usize) -> usize {
        (self.next() % bound as u64) as usize
    }
}

/// Mode ids chosen so that ASCII order and ICU collation DISAGREE on them.
/// `no-gcse` vs `nogcse`, `x=y` vs `x0y` and `Alpha` vs `alpha` are the three
/// families of divergence, so a `str::cmp` tiebreak is detectable.
const IDS: [&str; 6] = ["no-gcse", "nogcse", "x=y", "x0y", "Alpha", "alpha"];
const FAMILIES: [&str; 2] = ["routed", "gcc296"];
const STEMS: [&str; 4] = ["08000000", "08000010", "0800230c", "080006fc"];

fn string_array(items: &[String]) -> Json {
    Json::Array(items.iter().map(|s| Json::String(s.clone())).collect())
}

fn config_json(ids: &[String], family: &str) -> Json {
    // Key order is fixed and matches `mode_sweep`, because `canonicalJson` does
    // not sort keys and the grouping key is the serialised object.
    Json::Object(vec![
        ("ids".into(), string_array(ids)),
        (
            "flags".into(),
            string_array(&ids.iter().map(|id| format!("-f{id}")).collect::<Vec<_>>()),
        ),
        ("remove_flags".into(), Json::Array(Vec::new())),
        ("compiler_family".into(), Json::String(family.into())),
    ])
}

/// Every configuration a corpus may contain: the routed baseline, each single
/// id in each family, and the pairs, so proper-subset bounds are exercised.
fn config_pool(rng: &mut Rng) -> Vec<Json> {
    let mut pool = vec![config_json(&[], "routed")];
    for family in FAMILIES {
        for id in IDS {
            pool.push(config_json(&[id.to_string()], family));
        }
        for (index, first) in IDS.iter().enumerate() {
            for second in IDS.iter().skip(index + 1) {
                pool.push(config_json(&[(*first).to_string(), (*second).to_string()], family));
            }
        }
    }
    // Shuffle so insertion order into the config map varies between corpora;
    // insertion order is load-bearing for the emitted row order before sorting.
    for index in (1..pool.len()).rev() {
        let other = 1 + rng.below(index);
        pool.swap(index, other);
    }
    pool
}

fn corpus(rng: &mut Rng) -> Json {
    let report_count = 2 + rng.below(3);
    let pool = config_pool(rng);
    let take = 8 + rng.below(pool.len() - 8);
    let mut reports = Vec::new();
    for index in 0..report_count {
        let stem = STEMS[index % STEMS.len()];
        let mut results = Vec::new();
        for config in pool.iter().take(take) {
            // A small score range makes ties, and therefore tiebreaks, common.
            let score = rng.below(4) as f64;
            // Occasionally drop a row entirely, so `score === undefined` and the
            // `scores.size !== reports.length` skip both get exercised.
            if rng.below(16) == 0 {
                continue;
            }
            let compiled = rng.below(32) != 0;
            let mut row = vec![
                ("config".into(), config.clone()),
                ("compiled".into(), Json::Bool(compiled)),
            ];
            if rng.below(24) != 0 {
                row.push((
                    "evidence".into(),
                    Json::Object(vec![
                        ("exact".into(), Json::Bool(score == 0.0)),
                        ("differing_halfwords".into(), Json::Number(score)),
                    ]),
                ));
            }
            results.push(Json::Object(row));
        }
        reports.push(Json::Object(vec![
            ("stem".into(), Json::String(stem.into())),
            ("format".into(), Json::Number(4.0)),
            ("source".into(), Json::String(format!("semantic/{stem}.c"))),
            ("source_sha256".into(), Json::String(String::new())),
            ("reference_sha256".into(), Json::String(String::new())),
            ("compiler_signature".into(), Json::String(String::new())),
            ("policy".into(), Json::Null),
            (
                "planning".into(),
                Json::Object(vec![("bounded_search_complete".into(), Json::Bool(true))]),
            ),
            ("results".into(), Json::Array(results)),
        ]));
    }
    Json::Array(reports)
}

fn corpora() -> Vec<Json> {
    let mut rng = Rng(0x5eed_1234_abcd_0001);
    (0..CORPUS_COUNT).map(|_| corpus(&mut rng)).collect()
}

// ---------------------------------------------------------------------------
// Driving the two implementations
// ---------------------------------------------------------------------------

fn fixtures_dir() -> PathBuf {
    // Confined to this crate's own target directory, per the destructive-command
    // rule. Nothing here is ever removed by path expansion.
    let dir = PathBuf::from(concat!(env!("CARGO_MANIFEST_DIR"), "/target/fixtures"));
    std::fs::create_dir_all(&dir).expect("fixture directory is creatable");
    dir
}

const DRIVER: &str = r#"
import { canonicalJson } from "REPO/tools/lib/canonical_json.ts";
import {
  sharedExactConfigurations,
  sharedNonRegressingImprovements,
  irreducibleConfigurationImprovements,
  multiRegionImprovements,
  singleModeEffects,
} from "REPO/tools/compiler/mode_cohort.ts";
import { readFileSync } from "node:fs";

const corpora = JSON.parse(readFileSync(Bun.argv[2], "utf8"));
const out = [];
for (const reports of corpora) {
  try {
    out.push({ ok: canonicalJson({
      shared_exact: sharedExactConfigurations(reports),
      shared_improvements: sharedNonRegressingImprovements(reports),
      irreducible: irreducibleConfigurationImprovements(reports),
      multi_region: multiRegionImprovements(reports),
      single_effects: singleModeEffects(reports),
    }) });
  } catch (error) {
    out.push({ error: String((error as Error).message) });
  }
}
console.log(JSON.stringify(out));
"#;

/// `{ok}` or `{error}` per corpus, from either implementation.
type Outcome = Result<String, String>;

fn typescript_outcomes(corpora: &[Json]) -> Vec<Outcome> {
    let dir = fixtures_dir();
    let repo = mode_cohort::root();
    let repo = repo.to_str().expect("repository path is UTF-8");
    let driver = dir.join("parity_driver.ts");
    std::fs::write(&driver, DRIVER.replace("REPO", repo)).expect("driver is writable");
    let corpus_path = dir.join("parity_corpora.json");
    std::fs::write(
        &corpus_path,
        canonical_json(&Json::Array(corpora.to_vec())),
    )
    .expect("corpus is writable");

    let output = Command::new("bun")
        .arg(&driver)
        .arg(&corpus_path)
        .current_dir(repo)
        .output()
        .expect("bun must be installed: a missing runtime is a FAILURE, not a clean sweep");
    assert!(
        output.status.success(),
        "the TypeScript driver failed, which is a harness failure and not a pass:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let text = String::from_utf8(output.stdout).expect("driver output is UTF-8");
    let parsed = parse_json(text.trim()).expect("driver output parses");
    parsed
        .as_array()
        .expect("driver output is an array")
        .iter()
        .map(|entry| match (entry.get("ok"), entry.get("error")) {
            (Some(Json::String(ok)), _) => Ok(ok.clone()),
            (_, Some(Json::String(error))) => Err(error.clone()),
            _ => panic!("driver entry has neither ok nor error"),
        })
        .collect()
}

fn rust_outcome(reports: &Json) -> Outcome {
    let parsed: Result<Vec<Report>, String> = reports
        .as_array()
        .expect("corpus is an array")
        .iter()
        .map(|value| Report::from_json(value.clone()))
        .collect();
    let reports = parsed?;
    Ok(canonical_json(&all_aggregations(&reports)?))
}

/// The negative control's perturbation: identical to `rust_outcome` except the
/// shared-exact tiebreak uses `str::cmp` instead of the measured collator.
fn perturbed_outcome(reports: &Json) -> Outcome {
    let parsed: Result<Vec<Report>, String> = reports
        .as_array()
        .expect("corpus is an array")
        .iter()
        .map(|value| Report::from_json(value.clone()))
        .collect();
    let reports = parsed?;
    let mut value = all_aggregations(&reports)?;
    let broken = mode_cohort::aggregate::shared_exact_configurations_ascii_tiebreak(&reports)?;
    if let Json::Object(entries) = &mut value {
        for entry in entries.iter_mut() {
            if entry.0 == "shared_exact" {
                entry.1 = broken.clone();
            }
        }
    }
    Ok(canonical_json(&value))
}

#[test]
fn differential_parity_against_the_typescript() {
    let corpora = corpora();
    assert!(
        !corpora.is_empty(),
        "empty corpus: refusing to report a clean sweep"
    );

    let bun_started = std::time::Instant::now();
    let expected = typescript_outcomes(&corpora);
    let bun_elapsed = bun_started.elapsed();
    let rust_started = std::time::Instant::now();
    let rust_all: Vec<Outcome> = corpora.iter().map(rust_outcome).collect();
    let rust_elapsed = rust_started.elapsed();
    eprintln!(
        "benchmark: {} corpora -- bun {:?} (whole process, includes startup and \
         module load), rust {:?} (in-process)",
        corpora.len(),
        bun_elapsed,
        rust_elapsed
    );
    assert_eq!(rust_all.len(), corpora.len());

    assert_eq!(expected.len(), corpora.len(), "driver dropped cases");

    // ---- NEGATIVE CONTROL, run BEFORE the real comparison ----------------
    let sensitive: Vec<usize> = (0..corpora.len())
        .filter(|&index| perturbed_outcome(&corpora[index]) != rust_outcome(&corpora[index]))
        .collect();
    assert!(
        !sensitive.is_empty(),
        "no corpus is sensitive to the collation perturbation: the harness would \
         pass a broken port, so it fails instead"
    );
    let target = sensitive[0];
    let mut control_differences = 0usize;
    for (index, corpus) in corpora.iter().enumerate() {
        let observed = if index == target {
            perturbed_outcome(corpus)
        } else {
            rust_outcome(corpus)
        };
        if observed != expected[index] {
            control_differences += 1;
        }
    }
    assert_eq!(
        control_differences, 1,
        "negative control must yield exactly one difference, got {control_differences}"
    );

    // ---- The real comparison --------------------------------------------
    let mut payloads: Vec<String> = Vec::new();
    let mut throws = 0usize;
    let mut totals: Vec<(String, usize)> = Vec::new();
    for (index, corpus) in corpora.iter().enumerate() {
        let observed = rust_outcome(corpus);
        assert_eq!(
            observed, expected[index],
            "corpus {index} differs from the TypeScript"
        );
        match &observed {
            Ok(text) => {
                payloads.push(text.clone());
                let value = parse_json(text).expect("payload parses");
                for (name, found) in category_counts(&value) {
                    match totals.iter_mut().find(|(key, _)| *key == name) {
                        Some(slot) => slot.1 += found,
                        None => totals.push((name, found)),
                    }
                }
            }
            Err(_) => throws += 1,
        }
    }

    // ---- Floors ----------------------------------------------------------
    assert!(
        corpora.len() >= 200,
        "case floor: only {} corpora run",
        corpora.len()
    );
    let mut distinct = payloads.clone();
    distinct.sort_unstable();
    distinct.dedup();
    assert!(
        distinct.len() >= 100,
        "distinct-payload floor: {} distinct of {} successful runs; the corpus \
         has gone degenerate",
        distinct.len(),
        payloads.len()
    );
    assert!(
        throws >= 1,
        "no corpus exercised the missing-routed-baseline throw"
    );
    assert!(
        throws < corpora.len() / 2,
        "{throws} of {} corpora threw; the corpus is mostly error paths",
        corpora.len()
    );
    assert_eq!(totals.len(), 5, "expected five aggregation categories");
    for (name, found) in &totals {
        assert!(
            *found >= 50,
            "found-floor: category {name} produced only {found} rows across the \
             whole corpus, so it may have silently stopped matching"
        );
    }

    eprintln!(
        "parity: {} corpora, {} distinct payloads, {} throws, category totals {:?}",
        corpora.len(),
        distinct.len(),
        throws,
        totals
    );
}
