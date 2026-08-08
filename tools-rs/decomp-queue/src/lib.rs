// Support library for the exact-C search queue, ported from
// tools/decomp/decomp_queue.ts.
//
// WHY THIS FILE EXISTS
// The queue builder writes a tracked artifact (out/decomp/queue.json) whose
// every byte is compared against the TypeScript's output: the hash spelling,
// the key order, the number formatting, and the ranking are all load-bearing.
// The pieces that decide those bytes live here, apart from the orchestration in
// main.rs, so each one can be pinned by a test against a captured Bun result.
//
// The TypeScript reaches into three shared modules for this — canonical_json.ts
// for the writer format, match_m2c.ts for M2C_PREAMBLE, permute_m2c.ts for
// retainedAssemblyStems. Those files are reserved by sibling ports, so the
// small pieces used here are reproduced with PORT NOTEs rather than shared.

pub mod js;
pub mod wyhash;

use decomp_diagnose::json::{self, Value};
use decomp_diagnose::{CandidateDiagnosis, KINDS};

/// PORT NOTE: copied verbatim from `tools/lib/match_m2c.ts`, which a sibling
/// port owns. Verified byte-for-byte against `JSON.stringify(M2C_PREAMBLE)`
/// under Bun; if that file's preamble ever changes, this must change with it.
pub const M2C_PREAMBLE: &str = concat!(
    "typedef signed char s8;\n",
    "typedef unsigned char u8;\n",
    "typedef signed short s16;\n",
    "typedef unsigned short u16;\n",
    "typedef signed int s32;\n",
    "typedef unsigned int u32;\n",
    "typedef signed long long s64;\n",
    "typedef unsigned long long u64;\n",
    "typedef int bool;\n",
    "#define NULL ((void *)0)\n",
    "#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))\n",
    "\n",
);

/// `Number.MAX_SAFE_INTEGER`, the score stand-in for a state with no best body.
pub const MAX_SAFE_INTEGER: f64 = 9_007_199_254_740_991.0;

// ---------------------------------------------------------------------------
// canonical_json.ts
// ---------------------------------------------------------------------------

fn is_primitive(value: &Value) -> bool {
    !matches!(value, Value::Array(_) | Value::Object(_))
}

fn stringify_primitive(value: &Value) -> String {
    match value {
        Value::Null => "null".to_string(),
        Value::Bool(flag) => flag.to_string(),
        Value::Number(number) => json::number(*number),
        Value::String(text) => json::quote(text),
        _ => unreachable!("stringify_primitive is only called on primitives"),
    }
}

fn reflow(value: &Value, indent: &str) -> String {
    if is_primitive(value) {
        return stringify_primitive(value);
    }
    let deeper = format!("{indent}  ");
    match value {
        Value::Array(items) => {
            if items.is_empty() {
                return "[]".to_string();
            }
            if items.iter().all(is_primitive) {
                let inner: Vec<String> = items.iter().map(stringify_primitive).collect();
                return format!("[{}]", inner.join(", "));
            }
            let inner: Vec<String> = items
                .iter()
                .map(|item| format!("{deeper}{}", reflow(item, &deeper)))
                .collect();
            format!("[\n{}\n{indent}]", inner.join(",\n"))
        }
        Value::Object(entries) => {
            if entries.is_empty() {
                return "{}".to_string();
            }
            let inner: Vec<String> = entries
                .iter()
                .map(|(key, item)| {
                    format!("{deeper}{}: {}", json::quote(key), reflow(item, &deeper))
                })
                .collect();
            format!("{{\n{}\n{indent}}}", inner.join(",\n"))
        }
        _ => unreachable!("primitives were handled above"),
    }
}

/// `canonicalJson(value)` — objects one key per line, all-primitive arrays
/// inline.
///
/// PORT NOTE: `Value::Object` is an insertion-ordered `Vec`, matching
/// `Object.keys` on a JS object literal. A `HashMap`/`BTreeMap` here would
/// silently re-sort every key in the tracked report.
pub fn canonical_json(value: &Value) -> String {
    reflow(value, "")
}

// ---------------------------------------------------------------------------
// permute_m2c.ts: retainedAssemblyStems()
// ---------------------------------------------------------------------------

/// `retainedAssemblyStems()` from `tools/lib/permute_m2c.ts`.
///
/// PORT NOTE: this used to be a second copy of that function, carried here
/// because `tools-rs/permute-m2c/` did not exist yet. It does now, it owns the
/// port, and this is a re-export of it. The six ECMAScript traps that copy
/// existed to pin -- the `/i` flag on `/^\.4byte\s+(0x[0-9a-f]+)$/i`, JS `\s`
/// including U+FEFF and excluding U+0085, `.` excluding the four line
/// terminators, the `entry + 3 >= rows.length` off-by-one, `parseInt` via
/// `Number` + `ToInt32`, and `toLowerCase` full case mapping -- are pinned by
/// that crate's own tests, where the code they police lives.
///
/// PORT NOTE: `permute-m2c` resolves `asm/` through `PERMUTE_M2C_ASM`, which
/// defaults to `<repo>/asm`. The parity harness sets it alongside
/// `DECOMP_QUEUE_ROOT` so both knobs name the same fixture root; production
/// sets neither and both resolve to the repository.
///
/// PORT NOTE: the failure *text* differs from Bun's raw `ENOENT: ...` for a
/// missing `asm/classification.json`; the exit code, the offending path and the
/// fact of failing do not. The harness asserts same-failure, never same-prose.
pub use permute_m2c::retained_assembly_stems;

// ---------------------------------------------------------------------------
// Arguments
// ---------------------------------------------------------------------------

#[derive(Clone, Debug, PartialEq)]
pub struct Options {
    pub jobs: f64,
    pub limit: f64,
    pub refresh: bool,
    pub targets_out: Option<String>,
}

/// The outcome of argument parsing: the TypeScript can also exit 0 after
/// printing usage, which is not an error and not a set of options.
#[derive(Debug, PartialEq)]
pub enum Parsed {
    Options(Options),
    Help(String),
}

pub const USAGE: &str =
    "usage: decomp_queue.ts [--jobs N] [--limit N] [--refresh] [--targets-out FILE]";

/// `argumentsOf(argv)`.
///
/// PORT NOTE: `jobs` and `limit` are carried as `f64`, not `usize`. The JS
/// assigns `Number(argv[++index])` unconditionally and only validates
/// afterwards, so `--jobs 2.5`, `--jobs 0x10` and a trailing bare `--jobs`
/// (which yields `Number(undefined)` = NaN) all have to survive to the
/// validation step to produce the same message.
pub fn arguments_of(argv: &[String], hardware_concurrency: f64) -> Result<Parsed, String> {
    // `Math.max(1, Math.min(16, (navigator.hardwareConcurrency || 8) - 2))`
    let cores = if hardware_concurrency == 0.0 || hardware_concurrency.is_nan() {
        8.0
    } else {
        hardware_concurrency
    };
    let mut jobs = 1.0f64.max(16.0f64.min(cores - 2.0));
    let mut limit = 200.0f64;
    let mut refresh = false;
    let mut targets_out: Option<String> = None;
    let mut index = 0usize;
    while index < argv.len() {
        let argument = argv[index].as_str();
        match argument {
            "--jobs" => {
                index += 1;
                jobs = argv.get(index).map_or(f64::NAN, |value| js::js_number(value));
            }
            "--limit" => {
                index += 1;
                limit = argv.get(index).map_or(f64::NAN, |value| js::js_number(value));
            }
            "--refresh" => refresh = true,
            "--targets-out" => {
                index += 1;
                // PORT NOTE: a trailing `--targets-out` assigns `undefined`,
                // which is falsy, so the JS then skips the write entirely.
                targets_out = argv.get(index).cloned();
            }
            "-h" | "--help" => return Ok(Parsed::Help(USAGE.to_string())),
            other => return Err(format!("unrecognized argument: {other}")),
        }
        index += 1;
    }
    if !js::is_integer(jobs) || jobs < 1.0 || !js::is_integer(limit) || limit < 1.0 {
        return Err("jobs and limit must be positive integers".to_string());
    }
    Ok(Parsed::Options(Options {
        jobs,
        limit,
        refresh,
        targets_out,
    }))
}

// ---------------------------------------------------------------------------
// Scoring
// ---------------------------------------------------------------------------

#[derive(Clone, Copy, Debug, PartialEq)]
pub struct Priority {
    pub value: f64,
    pub plateau: bool,
    pub improvement: f64,
}

fn number_field(value: Option<&Value>) -> Option<f64> {
    match value {
        Some(Value::Number(number)) => Some(*number),
        _ => None,
    }
}

/// `expectedValue(diagnosis, state)`.
///
/// PORT NOTE: a round's `before`/`after` are read with `??`-free property
/// access in the JS, so a missing `after` would make the arithmetic NaN. That
/// is reproduced: an absent number becomes NaN here rather than 0, and NaN
/// propagates through `Math.max` exactly as it does in JS (`Math.max(0, NaN)`
/// is NaN).
pub fn expected_value(diagnosis: &CandidateDiagnosis, state: &Value) -> Priority {
    let empty = Vec::new();
    let rounds = match state.get("rounds") {
        Some(Value::Array(items)) => items,
        _ => &empty,
    };
    let improvement = match rounds.last() {
        Some(last) if number_field(last.get("before")).is_some_and(f64::is_finite) => {
            let before = number_field(last.get("before")).unwrap_or(f64::NAN);
            let after = number_field(last.get("after")).unwrap_or(f64::NAN);
            js_max(0.0, before - after)
        }
        _ => 0.0,
    };
    let tail = &rounds[rounds.len().saturating_sub(2)..];
    let plateau = rounds.len() >= 2
        && tail.iter().all(|round| {
            let before = number_field(round.get("before")).unwrap_or(f64::NAN);
            let after = number_field(round.get("after")).unwrap_or(f64::NAN);
            after >= before
        });
    let closeness = 1000.0 / (1.0 + diagnosis.byte_mismatches as f64);
    let tractability = 120.0 / (1.0 + diagnosis.actual_size as f64 / 48.0);
    let diagnosis_bonus = diagnosis.semantic_fraction * 70.0 + diagnosis.register_fraction * 20.0;
    let value = closeness
        + tractability
        + diagnosis_bonus
        + js_min(80.0, improvement * 4.0)
        - if plateau { 35.0 } else { 0.0 };
    Priority {
        value,
        plateau,
        improvement,
    }
}

/// `Math.max(a, b)` — NaN-propagating, unlike Rust's `f64::max`.
fn js_max(left: f64, right: f64) -> f64 {
    if left.is_nan() || right.is_nan() {
        return f64::NAN;
    }
    if left > right {
        left
    } else {
        right
    }
}

/// `Math.min(a, b)` — NaN-propagating, unlike Rust's `f64::min`.
fn js_min(left: f64, right: f64) -> f64 {
    if left.is_nan() || right.is_nan() {
        return f64::NAN;
    }
    if left < right {
        left
    } else {
        right
    }
}

/// `state.best?.score ?? Infinity` and its `Number.MAX_SAFE_INTEGER` variant.
///
/// PORT NOTE: `?.` short-circuits on `null` and `undefined` only, and `??`
/// falls through on those same two. A `best` of `false` or `0` would throw in
/// the JS; a non-numeric `score` would flow through as-is. Only the numeric
/// case is meaningful for the ranking, so anything else takes the fallback.
pub fn best_score(state: &Value, fallback: f64) -> f64 {
    match state.get("best") {
        Some(best) => number_field(best.get("score")).unwrap_or(fallback),
        None => fallback,
    }
}

/// `state.best?.body` when it is a truthy string.
pub fn best_body(state: &Value) -> Option<&str> {
    match state.get("best")?.get("body") {
        Some(Value::String(body)) if !body.is_empty() => Some(body),
        _ => None,
    }
}

/// The queue item, built in the TypeScript's object-literal key order. That
/// order reaches the tracked report verbatim, so it is part of the contract.
pub fn queue_item(
    stem: &str,
    candidate: &str,
    source_hash: &str,
    weighted_score: f64,
    priority: Priority,
    diagnosis: &CandidateDiagnosis,
) -> Value {
    Value::Object(vec![
        ("stem".to_string(), Value::String(stem.to_string())),
        ("candidate".to_string(), Value::String(candidate.to_string())),
        (
            "source_hash".to_string(),
            Value::String(source_hash.to_string()),
        ),
        ("weighted_score".to_string(), Value::Number(weighted_score)),
        ("expected_value".to_string(), Value::Number(priority.value)),
        ("plateau".to_string(), Value::Bool(priority.plateau)),
        ("improvement".to_string(), Value::Number(priority.improvement)),
        ("diagnosis".to_string(), diagnosis_value(diagnosis)),
    ])
}

/// `CandidateDiagnosis` as a JSON value, in the same key order its own
/// `to_json` uses.
pub fn diagnosis_value(diagnosis: &CandidateDiagnosis) -> Value {
    let counts = KINDS
        .iter()
        .enumerate()
        .map(|(index, kind)| {
            (
                kind.name().to_string(),
                Value::Number(diagnosis.counts[index] as f64),
            )
        })
        .collect();
    Value::Object(vec![
        ("stem".to_string(), Value::String(diagnosis.stem.clone())),
        (
            "byte_mismatches".to_string(),
            Value::Number(diagnosis.byte_mismatches as f64),
        ),
        (
            "actual_size".to_string(),
            Value::Number(diagnosis.actual_size as f64),
        ),
        (
            "expected_size".to_string(),
            Value::Number(diagnosis.expected_size as f64),
        ),
        (
            "instruction_mismatches".to_string(),
            Value::Number(diagnosis.instruction_mismatches as f64),
        ),
        ("counts".to_string(), Value::Object(counts)),
        (
            "dominant".to_string(),
            Value::String(diagnosis.dominant.clone()),
        ),
        (
            "register_fraction".to_string(),
            Value::Number(diagnosis.register_fraction),
        ),
        (
            "semantic_fraction".to_string(),
            Value::Number(diagnosis.semantic_fraction),
        ),
    ])
}

#[cfg(test)]
mod tests {
    use super::*;

    fn object(pairs: &[(&str, Value)]) -> Value {
        Value::Object(
            pairs
                .iter()
                .map(|(key, value)| ((*key).to_string(), value.clone()))
                .collect(),
        )
    }

    fn round(before: f64, after: f64) -> Value {
        object(&[
            ("before", Value::Number(before)),
            ("after", Value::Number(after)),
        ])
    }

    fn diagnosis(byte_mismatches: i64, actual_size: i64, semantic: f64, register: f64) -> CandidateDiagnosis {
        CandidateDiagnosis {
            stem: "08021950".to_string(),
            byte_mismatches,
            actual_size,
            expected_size: actual_size,
            instruction_mismatches: 3,
            counts: [1, 0, 2, 0, 0, 0, 0],
            dominant: "register_only".to_string(),
            register_fraction: register,
            semantic_fraction: semantic,
        }
    }

    #[test]
    fn canonical_json_keeps_object_order_and_inlines_primitive_arrays() {
        let value = object(&[
            ("format", Value::Number(1.0)),
            ("zebra", Value::String("first".to_string())),
            ("alpha", Value::Array(vec![Value::Number(1.0), Value::Number(2.0)])),
            ("nested", Value::Array(vec![object(&[("a", Value::Bool(true))])])),
            ("empty_array", Value::Array(vec![])),
            ("empty_object", Value::Object(vec![])),
        ]);
        // Captured from `canonicalJson(...)` under Bun for the same document.
        let expected = "{\n  \"format\": 1,\n  \"zebra\": \"first\",\n  \"alpha\": [1, 2],\n  \"nested\": [\n    {\n      \"a\": true\n    }\n  ],\n  \"empty_array\": [],\n  \"empty_object\": {}\n}";
        assert_eq!(canonical_json(&value), expected);
        // The zebra/alpha inversion would be silently repaired by a sorted map.
        assert!(
            canonical_json(&value).find("zebra").unwrap() < canonical_json(&value).find("alpha").unwrap(),
            "key order must be insertion order, not sorted"
        );
    }

    #[test]
    fn arguments_default_and_validate_like_the_typescript() {
        let argv = |items: &[&str]| -> Vec<String> {
            items.iter().map(|item| (*item).to_string()).collect()
        };
        // 18 cores - 2, clamped to 16.
        let Parsed::Options(defaults) = arguments_of(&argv(&[]), 18.0).unwrap() else {
            panic!("expected options");
        };
        assert_eq!(defaults.jobs, 16.0);
        assert_eq!(defaults.limit, 200.0);
        assert!(!defaults.refresh);
        assert_eq!(defaults.targets_out, None);
        // 4 cores - 2 = 2; 1 core - 2 = -1 clamps up to 1; 0 falls back to 8.
        for (cores, jobs) in [(4.0, 2.0), (1.0, 1.0), (0.0, 6.0)] {
            let Parsed::Options(options) = arguments_of(&argv(&[]), cores).unwrap() else {
                panic!("expected options");
            };
            assert_eq!(options.jobs, jobs, "hardwareConcurrency {cores}");
        }
        // `Number("0x10")` is 16, not a parse error.
        let Parsed::Options(hex) = arguments_of(&argv(&["--jobs", "0x10"]), 8.0).unwrap() else {
            panic!("expected options");
        };
        assert_eq!(hex.jobs, 16.0);
        let Parsed::Options(full) =
            arguments_of(&argv(&["--limit", "3", "--refresh", "--targets-out", "t.txt"]), 8.0)
                .unwrap()
        else {
            panic!("expected options");
        };
        assert_eq!(full.limit, 3.0);
        assert!(full.refresh);
        assert_eq!(full.targets_out.as_deref(), Some("t.txt"));
        assert_eq!(
            arguments_of(&argv(&["-h"]), 8.0).unwrap(),
            Parsed::Help(USAGE.to_string())
        );
        for bad in [
            vec!["--jobs"],          // Number(undefined) is NaN
            vec!["--jobs", "0"],
            vec!["--jobs", "2.5"],
            vec!["--limit", "-1"],
            vec!["--limit", "abc"],
        ] {
            assert_eq!(
                arguments_of(&argv(&bad), 8.0).unwrap_err(),
                "jobs and limit must be positive integers",
                "{bad:?}"
            );
        }
        assert_eq!(
            arguments_of(&argv(&["--nope"]), 8.0).unwrap_err(),
            "unrecognized argument: --nope"
        );
        // A trailing `--targets-out` leaves it unset, so nothing is written.
        let Parsed::Options(trailing) = arguments_of(&argv(&["--targets-out"]), 8.0).unwrap()
        else {
            panic!("expected options");
        };
        assert_eq!(trailing.targets_out, None);
    }

    #[test]
    fn expected_value_reproduces_the_scoring_formula() {
        // No rounds: no improvement, no plateau.
        let bare = expected_value(&diagnosis(6, 48, 0.5, 0.25), &object(&[]));
        assert!(!bare.plateau);
        assert_eq!(bare.improvement, 0.0);
        // 1000/7 + 120/2 + (0.5*70 + 0.25*20) + 0 - 0
        assert_eq!(bare.value, 1000.0 / 7.0 + 60.0 + 40.0);

        // One improving round: improvement counts, two rounds are needed for a
        // plateau so this one cannot plateau.
        let improving = object(&[("rounds", Value::Array(vec![round(30.0, 21.0)]))]);
        let scored = expected_value(&diagnosis(6, 48, 0.0, 0.0), &improving);
        assert_eq!(scored.improvement, 9.0);
        assert!(!scored.plateau);
        assert_eq!(scored.value, 1000.0 / 7.0 + 60.0 + 36.0);

        // The improvement bonus saturates at 80 (improvement * 4).
        let huge = object(&[("rounds", Value::Array(vec![round(1000.0, 0.0)]))]);
        assert_eq!(
            expected_value(&diagnosis(6, 48, 0.0, 0.0), &huge).value,
            1000.0 / 7.0 + 60.0 + 80.0
        );

        // Two non-improving rounds plateau and take the 35-point penalty; only
        // the last two rounds are inspected.
        let plateaued = object(&[(
            "rounds",
            Value::Array(vec![round(30.0, 10.0), round(10.0, 10.0), round(10.0, 12.0)]),
        )]);
        let flat = expected_value(&diagnosis(6, 48, 0.0, 0.0), &plateaued);
        assert!(flat.plateau);
        assert_eq!(flat.improvement, 0.0, "after > before clamps to zero");
        assert_eq!(flat.value, 1000.0 / 7.0 + 60.0 - 35.0);

        // A non-finite `before` disables the improvement term entirely.
        let infinite = object(&[(
            "rounds",
            Value::Array(vec![round(f64::INFINITY, 4.0)]),
        )]);
        assert_eq!(
            expected_value(&diagnosis(6, 48, 0.0, 0.0), &infinite).improvement,
            0.0
        );
    }

    #[test]
    fn best_score_and_body_follow_optional_chaining() {
        let missing = object(&[]);
        assert_eq!(best_score(&missing, f64::INFINITY), f64::INFINITY);
        assert_eq!(best_body(&missing), None);
        let null_best = object(&[("best", Value::Null)]);
        assert_eq!(best_score(&null_best, 7.0), 7.0);
        assert_eq!(best_body(&null_best), None);
        let empty_body = object(&[(
            "best",
            object(&[("body", Value::String(String::new())), ("score", Value::Number(9.0))]),
        )]);
        assert_eq!(best_score(&empty_body, 7.0), 9.0);
        assert_eq!(
            best_body(&empty_body),
            None,
            "an empty body is falsy, so the brute fallback runs"
        );
        let real = object(&[(
            "best",
            object(&[("body", Value::String("void f(){}".to_string())), ("score", Value::Number(0.0))]),
        )]);
        assert_eq!(best_score(&real, 7.0), 0.0);
        assert_eq!(best_body(&real), Some("void f(){}"));
    }

    #[test]
    fn queue_item_key_order_matches_the_object_literal() {
        let item = queue_item(
            "08021950",
            "out/decomp/candidates/08021950.c",
            "1vlaxc15ccq7a",
            9.0,
            Priority { value: 277.5, plateau: false, improvement: 21.0 },
            &diagnosis(3, 96, 0.25, 0.75),
        );
        let Value::Object(entries) = &item else {
            panic!("expected an object");
        };
        let keys: Vec<&str> = entries.iter().map(|(key, _)| key.as_str()).collect();
        assert_eq!(
            keys,
            [
                "stem",
                "candidate",
                "source_hash",
                "weighted_score",
                "expected_value",
                "plateau",
                "improvement",
                "diagnosis"
            ]
        );
        let Some(Value::Object(counts)) = item.get("diagnosis").and_then(|d| d.get("counts")) else {
            panic!("expected counts");
        };
        assert_eq!(counts.len(), KINDS.len());
        assert_eq!(counts[0].0, KINDS[0].name());
    }

    #[test]
    fn the_preamble_matches_the_typescript_byte_for_byte() {
        // Captured with `JSON.stringify(M2C_PREAMBLE)` under Bun.
        let captured = "typedef signed char s8;\ntypedef unsigned char u8;\ntypedef signed short s16;\ntypedef unsigned short u16;\ntypedef signed int s32;\ntypedef unsigned int u32;\ntypedef signed long long s64;\ntypedef unsigned long long u64;\ntypedef int bool;\n#define NULL ((void *)0)\n#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))\n\n";
        assert_eq!(M2C_PREAMBLE, captured);
        assert!(M2C_PREAMBLE.starts_with("typedef"), "the startsWith guard depends on this");
    }

    #[test]
    fn retained_stems_read_the_real_tree_and_are_not_vacuous() {
        let retained = retained_assembly_stems().expect("the real tree must classify");
        assert!(
            retained.len() > 100,
            "a retained set this small means the scan found nothing: {}",
            retained.len()
        );
    }
}
