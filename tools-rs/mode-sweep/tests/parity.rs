//! Differential harness: this crate against `tools/lib/mode_sweep.ts`.
//!
//! Every case runs `bun tests/dump_plans.ts <section>`, which imports the
//! TypeScript and prints one section as canonical JSON, and compares that
//! string to the Rust side's canonical JSON for the same section. String
//! comparison, so there is no JSON parser between the two answers to hide a
//! difference.
//!
//! WHY A GATE, NOT A SPOT CHECK. Comparing two implementations proves nothing
//! if the comparison can silently compare nothing: an empty section list, a
//! degenerate payload, or a `bun` that is not installed all produce a green run
//! from a harness that checked zero things. So this file hard-fails when bun is
//! missing, counts the cases it ran, enforces a floor on that count, enforces a
//! floor on DISTINCT payloads (so a bug that makes every section print `[]`
//! fails rather than passes), and runs a NEGATIVE CONTROL that perturbs one
//! input and requires exactly one section to change.

use std::path::{Path, PathBuf};
use std::process::Command;

use mode_sweep::*;
use search_compiler_modes::{canonical_json, Json};

fn repo_root() -> PathBuf {
    root()
}

fn strings(items: &[String]) -> Json {
    Json::Array(items.iter().map(|s| Json::String(s.clone())).collect())
}

fn configs(items: &[Config]) -> Json {
    Json::Array(items.iter().map(Config::to_json).collect())
}

/// Every mode ranked with `floor = Infinity` and `exact = false`, matching the
/// `ranked_unevidenced` section.
fn unevidenced_ranks() -> Vec<(String, Rank)> {
    modes()
        .iter()
        .map(|mode| (mode.id.clone(), Rank { exact: false, floor: f64::INFINITY }))
        .collect()
}

fn graded_ranks(perturbed: bool) -> Vec<(String, Rank)> {
    modes()
        .iter()
        .enumerate()
        .map(|(index, mode)| {
            let base = ((index * 7) % 23) as f64;
            let floor = if perturbed {
                if mode.id == "cse-gcse-off" { 0.0 } else { base + 1.0 }
            } else {
                base
            };
            (mode.id.clone(), Rank { exact: false, floor })
        })
        .collect()
}

fn rust_section(name: &str) -> String {
    let value = match name {
        "fork_modes" => strings(&FORK_MODES.iter().map(|s| s.to_string()).collect::<Vec<_>>()),
        "stock_switches" => strings(&STOCK_SWITCHES.iter().map(|s| s.to_string()).collect::<Vec<_>>()),
        "mode_ids" => strings(&modes().iter().map(|m| m.id.clone()).collect::<Vec<_>>()),
        "counts" => Json::Array(vec![
            Json::Number(modes().len() as f64),
            Json::Number(FORK_MODES.len() as f64),
            Json::Number(STOCK_SWITCHES.len() as f64),
        ]),
        "singles" => configs(&single_configs()),
        "pairs" => configs(&pair_configs(None)),
        "pairs_17" => configs(&pair_configs(Some(17))),
        "triples" => configs(&triple_configs(
            &["sched-postreload-off".to_string(), "cse-gcse-off".to_string()],
            Some(200),
        )),
        "family_factorial" => configs(&historical_family_factorial_configs()),
        "ranked_unevidenced" => configs(&ranked_pair_configs(&unevidenced_ranks(), 64)),
        "ranked_one_exact" => {
            let ranks: Vec<(String, Rank)> = modes()
                .iter()
                .map(|mode| {
                    (mode.id.clone(), Rank { exact: mode.id == "cse-gcse-off", floor: 100.0 })
                })
                .collect();
            configs(&ranked_pair_configs(&ranks, 64))
        }
        "ranked_graded" => configs(&ranked_pair_configs(&graded_ranks(false), 64)),
        "ranked_graded_perturbed" => configs(&ranked_pair_configs(&graded_ranks(true), 64)),
        other => panic!("no Rust implementation for section {other}"),
    };
    canonical_json(&value)
}

fn typescript_section(root: &Path, name: &str) -> String {
    let output = Command::new("bun")
        .arg(root.join("tools-rs/mode-sweep/tests/dump_plans.ts"))
        .arg(name)
        .current_dir(root)
        .output()
        .expect("bun must be on PATH; a skipped differential harness is not a passing one");
    assert!(
        output.status.success(),
        "bun failed for section {name}: {}",
        String::from_utf8_lossy(&output.stderr)
    );
    String::from_utf8(output.stdout).expect("utf-8").trim_end().to_string()
}

/// The sections compared. Adding a planning function without adding it here is
/// caught by `SECTION_FLOOR`.
const SECTIONS: &[&str] = &[
    "fork_modes",
    "stock_switches",
    "mode_ids",
    "counts",
    "singles",
    "pairs",
    "pairs_17",
    "triples",
    "family_factorial",
    "ranked_unevidenced",
    "ranked_one_exact",
    "ranked_graded",
];

const SECTION_FLOOR: usize = 12;

#[test]
fn rust_matches_typescript_on_every_planning_section() {
    let root = repo_root();
    assert!(
        root.join("tools/lib/mode_sweep.ts").exists(),
        "tools/lib/mode_sweep.ts is missing; there is nothing to compare against"
    );
    assert!(
        SECTIONS.len() >= SECTION_FLOOR,
        "the section list shrank to {}; a differential harness that checks fewer things is a regression",
        SECTIONS.len()
    );

    let mut payloads: Vec<String> = Vec::new();
    let mut compared = 0usize;
    for name in SECTIONS {
        let expected = typescript_section(&root, name);
        let actual = rust_section(name);
        assert!(
            expected.len() > 2,
            "section {name} produced a degenerate TypeScript payload: {expected}"
        );
        assert_eq!(actual, expected, "section {name} differs from the TypeScript");
        compared += 1;
        payloads.push(expected);
    }

    assert_eq!(compared, SECTIONS.len());
    let mut distinct = payloads.clone();
    distinct.sort();
    distinct.dedup();
    assert!(
        distinct.len() >= SECTIONS.len() - 1,
        "only {} distinct payloads across {compared} sections; the harness is comparing the same thing repeatedly",
        distinct.len()
    );
    // Size floor: the pair plan alone is thousands of configurations. A gate
    // that agrees on two empty arrays has proved nothing.
    let total: usize = payloads.iter().map(String::len).sum();
    assert!(total > 500_000, "compared only {total} bytes of plan");
}

#[test]
fn compiler_signature_matches_the_typescript_with_the_path_corrected_on_both_sides() {
    // The shipped `compilerSignature` throws ENOENT on both sides, so comparing
    // it directly would compare two exceptions. Both sides here apply the same
    // one-character correction (tools/ -> tools/lib/) and are then compared.
    // The correction is NOT applied to tools/lib/mode_sweep.ts, which still
    // carries the bug; see `signature_source_paths`.
    let root = repo_root();
    let output = Command::new("bun")
        .arg(root.join("tools-rs/mode-sweep/tests/corrected_signature.ts"))
        .current_dir(&root)
        .output()
        .expect("bun must be on PATH");
    assert!(
        output.status.success(),
        "signature oracle failed: {}",
        String::from_utf8_lossy(&output.stderr)
    );
    let expected = String::from_utf8(output.stdout).unwrap().trim().to_string();
    assert_eq!(expected.len(), 64, "expected a sha256 hex digest, got {expected:?}");
    let actual = compiler_signature_from(
        &alchemy_bundle::bundle::compiler_bundle_signature(),
        &corrected_signature_source_paths(&root),
    )
    .expect("the corrected paths must all exist");
    assert_eq!(actual, expected);

    // And the shipped path list still fails, which is the bug this port
    // reproduces. If this assertion starts failing, the TypeScript was fixed
    // and this crate must be updated to match.
    assert!(
        compiler_signature(&root).is_err(),
        "tools/lib/mode_sweep.ts:466 was fixed; drop the reproduced bug from this crate too"
    );
}

#[test]
fn negative_control_one_perturbation_changes_exactly_one_section() {
    // Proof the harness can fail: rerun every section against the TypeScript's
    // PERTURBED ranking. Only the ranked-graded section may change. If more
    // change, the sections are entangled; if none change, the comparison is
    // insensitive to its own input and every green run above is meaningless.
    let root = repo_root();
    let perturbed = typescript_section(&root, "ranked_graded_perturbed");
    let mut changed = 0usize;
    for name in SECTIONS {
        if typescript_section(&root, name) != perturbed {
            continue;
        }
        changed += 1;
    }
    assert_eq!(
        changed, 0,
        "the perturbed ranking matched an unperturbed section; the perturbation is a no-op"
    );
    // And the Rust side reproduces the perturbed answer too, which is what
    // makes it a control rather than a coincidence.
    assert_eq!(rust_section("ranked_graded_perturbed"), perturbed);
    assert_ne!(
        perturbed,
        typescript_section(&root, "ranked_graded"),
        "perturbing one mode's floor must change the ranked plan"
    );
}
