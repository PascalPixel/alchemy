//! A Rust port of `tools/overlay/overlay_mode_cohort.ts`.
//!
//! The tool answers one question: across a small cohort of overlay functions
//! whose C is already written, is there a SINGLE compiler configuration that
//! does better than the routed default on more than one of them at once? A
//! configuration that only helps one owner is a coincidence; one that helps
//! several unrelated owners is evidence about which compiler actually built the
//! game.
//!
//! It is NOT a wrapper around `tools/compiler/mode_cohort.ts`. The two share
//! only the `mode_sweep` configuration planner. This one carries its own byte
//! scorer, its own candidate spec parser, its own on-disk cache, and its own
//! assembly path through the ARM binutils, and its report is a byte-level
//! `format: 1` document rather than the other tool's.
//!
//! Every module here documents the JavaScript semantics it is reproducing.
//! Where the TypeScript has a defect, the defect is PORTED and marked
//! `PORT NOTE`, because a mirror that quietly disagrees is worse than a mirror
//! that is faithfully wrong.

pub mod aggregate;
pub mod collate;
pub mod json;
pub mod jsops;
pub mod options;
pub mod paths;
pub mod plan;
pub mod score;
pub mod spec;
pub mod verify;

use aggregate::{best_by_region, by_config};
use json::{array_len, set};
use options::{options_of, Options, Parsed};
use overlay_disasm::compile::assemble_overlay;
use overlay_disasm::disasm::OVERLAY_BASE;
use overlay_disasm::paths::OverlaySource;
use paths::{hash, root, self_digest};
use plan::{address_from_source, configs_of, load_candidates, Candidate};
use score::{accepted_score, mark_cached, write_json, Score};
use search_compiler_modes::{canonical_json, js_number_text, parse_json, Json};
use spec::{byte_difference, candidate_spec};
use std::path::{Path, PathBuf};
use std::sync::atomic::{AtomicUsize, Ordering as AtomicOrdering};
use std::sync::Mutex;

/// `const FORMAT = 1`.
///
/// PORT NOTE: this is hashed into every cache key as `String(FORMAT)`, so it is
/// the sanctioned way to invalidate the cache wholesale. It is a NUMBER, not a
/// `-vN` string literal glued to a key prefix, which is why
/// `tools/cache-key-lint` has nothing to say about either mirror.
pub const FORMAT: u32 = 1;

/// `Buffer#subarray(start, end)`.
///
/// PORT NOTE -- BUFFER SLICING CLAMPS WHERE RUST PANICS. `offset` and
/// `span_bytes` come out of a JSON file this tool does not write, so an
/// out-of-range or NaN span is reachable. Node coerces NaN to 0 and clamps both
/// ends into `[0, len]`; `&buffer[a..b]` would abort the process instead.
fn clamped_subarray(buffer: &[u8], start: f64, end: f64) -> &[u8] {
    let clamp = |value: f64| -> usize {
        if value.is_nan() || value < 0.0 {
            return 0;
        }
        let truncated = value.trunc();
        if truncated >= buffer.len() as f64 {
            buffer.len()
        } else {
            truncated as usize
        }
    };
    let (from, to) = (clamp(start), clamp(end));
    if to <= from {
        return &[];
    }
    &buffer[from..to]
}

/// `path.slice(ROOT.length + 1)`.
///
/// PORT NOTE: this is a raw string slice in the TypeScript, not `relative()`.
/// A path that does NOT start with `ROOT` is therefore mangled rather than
/// rejected, and the port reproduces that: it cuts the same number of UTF-16
/// units regardless of what is there. Reachable through `--inventory` pointing
/// at an inventory whose sources live outside the tree.
fn strip_root(path: &str) -> String {
    jsops::js_slice_from(path, jsops::js_length(&root().to_string_lossy()) + 1)
}

fn overlay_image(overlay: &str) -> Result<Vec<u8>, String> {
    let source = OverlaySource::path(root().join("assets/code").join(format!("{overlay}_overlay.s")));
    assemble_overlay(&source, OVERLAY_BASE)
}

struct Task<'a> {
    candidate: &'a Candidate,
    config: &'a mode_sweep::Config,
}

/// One `(candidate, config)` cell: cache lookup, compile, score, cache write.
fn score_task(
    task: &Task<'_>,
    overlays: &[(String, Vec<u8>)],
    compiler_signature: &str,
    options: &Options,
) -> Result<Score, String> {
    let candidate = task.candidate;
    let address = address_from_source(candidate)?;
    let image = overlays
        .iter()
        .find(|(name, _)| name == &candidate.overlay)
        .map(|(_, image)| image.as_slice())
        // `overlays.get(candidate.overlay)!`: the map was built from this same
        // candidate list, so the assertion holds. Kept as an error rather than
        // an `unwrap` so a future caller cannot turn it into a panic.
        .ok_or_else(|| format!("overlay image missing: {}", candidate.overlay))?;
    let reference =
        clamped_subarray(image, candidate.offset, candidate.offset + candidate.span_bytes).to_vec();
    let source_bytes = std::fs::read(&candidate.source)
        .map_err(|error| format!("{}: {error}", candidate.source))?;
    let identity = Json::Object(vec![
        ("id".into(), Json::String(candidate.id.clone())),
        ("config".into(), task.config.to_json()),
    ]);
    let key = hash(&[
        FORMAT.to_string().as_bytes(),
        &source_bytes,
        &reference,
        compiler_signature.as_bytes(),
        canonical_json(&identity).as_bytes(),
    ]);
    let cache_path = Path::new(&options.output).join("cache").join(format!("{key}.json"));
    if cache_path.exists() {
        let text = std::fs::read(&cache_path).map_err(|error| format!("{}: {error}", cache_path.display()))?;
        // PORT NOTE: a corrupt cache file makes `JSON.parse` THROW, and that
        // throw is outside the tool's own try/catch, so the whole run dies. The
        // port keeps that: a poisoned cache must be loud, not silently treated
        // as a miss. This is the same failure mode a stale `overlay-c-v3` entry
        // once produced across checkouts, and hiding it is how it went unnoticed.
        let parsed = parse_json(&String::from_utf8_lossy(&text))?;
        if let Some(cached) = accepted_score(&parsed, &key) {
            return Ok(mark_cached(cached));
        }
    }
    let scratch = Path::new(&options.output).join("scratch").join(&key);
    std::fs::create_dir_all(&scratch).map_err(|error| format!("{}: {error}", scratch.display()))?;
    // The verifier requires an address-stemmed translation unit. Including the
    // original file keeps relative includes rooted at its real directory and
    // leaves the candidate itself untouched.
    let input = scratch.join(format!("{:08x}.c", address as i64));
    // `#include ${JSON.stringify(candidate.source)}`: JSON string quoting is
    // also valid C string quoting for every path this tool can see, and it is
    // what escapes a backslash or a quote in the path.
    let include = format!(
        "#include {}\n",
        canonical_json(&Json::String(candidate.source.clone()))
    );
    std::fs::write(&input, include).map_err(|error| format!("{}: {error}", input.display()))?;

    let attempt = verify::verify_overlay_candidate(
        &input.to_string_lossy(),
        candidate,
        image,
        &scratch,
        task.config,
    );
    let score = match attempt {
        Ok(actual) => {
            let difference = byte_difference(&actual, &reference);
            let mut row = Json::Object(vec![
                ("id".into(), Json::String(candidate.id.clone())),
                (
                    "source".into(),
                    Json::String(if Path::new(&candidate.source).is_absolute() {
                        strip_root(&candidate.source)
                    } else {
                        candidate.source.clone()
                    }),
                ),
                ("config".into(), task.config.to_json()),
                ("cache_key".into(), Json::String(key.clone())),
                ("cached".into(), Json::Bool(false)),
                ("compiled".into(), Json::Bool(true)),
                ("exact".into(), Json::Bool(difference.count == 0)),
                ("expected_size".into(), Json::Number(reference.len() as f64)),
                ("actual_size".into(), Json::Number(actual.len() as f64)),
                ("differing_bytes".into(), Json::Number(difference.count as f64)),
            ]);
            // `...(first === null ? {} : { first_difference: first })`: the key
            // is ABSENT, not null, when the bytes match. `canonicalJson` emits
            // absent and null differently, so this changes the report bytes.
            if let Some(first) = difference.first {
                set(&mut row, "first_difference", Json::Number(first as f64));
            }
            Score { json: row }
        }
        Err(message) => Score {
            json: Json::Object(vec![
                ("id".into(), Json::String(candidate.id.clone())),
                // PORT NOTE: the FAILURE branch slices unconditionally while the
                // success branch tests `isAbsolute` first. Both reach the same
                // value today because `loadCandidates` resolves every source to
                // an absolute path, but the asymmetry is real and is preserved.
                ("source".into(), Json::String(strip_root(&candidate.source))),
                ("config".into(), task.config.to_json()),
                ("cache_key".into(), Json::String(key.clone())),
                ("cached".into(), Json::Bool(false)),
                ("compiled".into(), Json::Bool(false)),
                ("exact".into(), Json::Bool(false)),
                ("expected_size".into(), Json::Number(reference.len() as f64)),
                (
                    "error".into(),
                    Json::String(jsops::js_slice_to(&message, 500).to_string()),
                ),
            ]),
        },
    };
    write_json(&cache_path, &score.json).map_err(|error| format!("{}: {error}", cache_path.display()))?;
    Ok(score)
}

/// `parallelMap(items, jobs, operation)`.
///
/// PORT NOTE: worker count is `Math.min(jobs, items.length)`, and results land
/// at their ORIGINAL INDEX rather than in completion order. Both matter: the
/// report's `results` array is compared byte-for-byte, and `bestByRegion` relies
/// on `scores` order to break its NaN ties.
fn parallel_map<'a>(
    tasks: &'a [Task<'a>],
    jobs: usize,
    operation: &(dyn Fn(&Task<'a>) -> Result<Score, String> + Sync),
) -> Result<Vec<Score>, String> {
    let slots: Mutex<Vec<Option<Result<Score, String>>>> =
        Mutex::new((0..tasks.len()).map(|_| None).collect());
    let cursor = AtomicUsize::new(0);
    let workers = jobs.min(tasks.len());
    std::thread::scope(|scope| {
        for _ in 0..workers {
            scope.spawn(|| loop {
                let index = cursor.fetch_add(1, AtomicOrdering::SeqCst);
                if index >= tasks.len() {
                    return;
                }
                let outcome = operation(&tasks[index]);
                slots.lock().expect("scoring slot")[index] = Some(outcome);
            });
        }
    });
    let mut scores = Vec::with_capacity(tasks.len());
    for slot in slots.into_inner().expect("scoring slots") {
        // A thrown operation rejects `Promise.all`, which aborts `main`. The
        // FIRST index that failed is reported, not the first to fail in time,
        // which is the deterministic half of that behaviour.
        scores.push(slot.expect("every task scored")?);
    }
    Ok(scores)
}

/// `selfTest()`.
pub fn self_test() -> Result<String, String> {
    let parsed = candidate_spec("resource_394:07e0=semantic/overlays/resource_394_c_020007e0.c")?;
    if parsed.id != "resource_394:07e0" || !parsed.source.starts_with("semantic/") {
        return Err("candidate parser failed".into());
    }
    let difference = byte_difference(&[1, 2, 3], &[1, 4]);
    if difference.count != 2 || difference.first != Some(1) {
        return Err("byte scorer failed".into());
    }
    let key = hash(&[
        b"source",
        b"target",
        canonical_json(&Json::Object(vec![(
            "family".into(),
            Json::String("gcc296".into()),
        )]))
        .as_bytes(),
    ]);
    let row = Json::Object(vec![
        ("id".into(), Json::String("x:0000".into())),
        ("source".into(), Json::String("x.c".into())),
        (
            "config".into(),
            mode_sweep::Config {
                ids: vec![],
                flags: vec![],
                remove_flags: vec![],
                compiler_family: "gcc296".into(),
            }
            .to_json(),
        ),
        ("cache_key".into(), Json::String(key.clone())),
        ("cached".into(), Json::Bool(false)),
        ("compiled".into(), Json::Bool(true)),
        ("exact".into(), Json::Bool(false)),
        ("expected_size".into(), Json::Number(2.0)),
        ("differing_bytes".into(), Json::Number(1.0)),
    ]);
    if accepted_score(&row, &key).is_none() || accepted_score(&row, &format!("{key}x")).is_some() {
        return Err("cache validation failed".into());
    }
    let families_only = Options {
        candidates: vec!["x:0000=x.c".into()],
        inventory: "x".into(),
        output: "x".into(),
        scope: options::Scope::Families,
        pairs: 0.0,
        triple_seeds: vec![],
        triples: 0.0,
        jobs: 1.0,
        top: 1.0,
    };
    if configs_of(&families_only).len() < 5 {
        return Err("family configuration planning failed".into());
    }
    let seeded = configs_of(&Options {
        triple_seeds: vec![
            "cse-two-insn-immediate-off".into(),
            "sched-low-dest-first".into(),
        ],
        ..families_only
    });
    if !seeded.iter().any(|config| config.ids.len() == 3) {
        return Err("seeded triple planning produced no triples".into());
    }
    if !seeded.iter().any(|config| {
        config.ids.iter().any(|id| id == "cse-two-insn-immediate-off") && config.ids.len() == 3
    }) {
        return Err("seeded triples do not include their own seeds".into());
    }
    let biased = overlay_disasm::compile::bias_in_image_label_words(".Ltable:\n\t.word\t.Ltable\n");
    if biased.biased != 1 || !biased.text.contains(".Ltable + 0x8000") {
        return Err("overlay label-word bias is not applied".into());
    }
    let default_call_via =
        alchemy_symbols::symbols::overlay_call_via_base("resource_373", Some("exact/not-an-override.c"));
    let overridden_call_via = alchemy_symbols::symbols::overlay_call_via_base(
        "resource_373",
        Some("exact/resource_373_c_02005610.c"),
    );
    if default_call_via == overridden_call_via
        || alchemy_symbols::symbols::external_symbol_assembly("_call_via_r3", overridden_call_via)?
            == alchemy_symbols::symbols::external_symbol_assembly("_call_via_r3", default_call_via)?
    {
        return Err("overlay call-via source routing is not applied".into());
    }
    Ok("overlay mode cohort self-test passed".into())
}

/// `main()`. Returns the lines the TypeScript writes to stdout, in order, so a
/// parity harness can compare them without capturing a process.
pub fn run(argv: &[String]) -> Result<Vec<String>, String> {
    if argv.iter().any(|argument| argument == "--self-test") {
        return Ok(vec![self_test()?]);
    }
    let options = match options_of(argv)? {
        Parsed::Help => return Ok(vec![options::HELP.to_string()]),
        Parsed::Options(options) => *options,
    };
    let candidates = load_candidates(&options)?;
    let configs = configs_of(&options);
    let compiler_signature = hash(&[
        alchemy_bundle::bundle::compiler_bundle_signature().as_bytes(),
        self_digest().as_bytes(),
    ]);
    // `new Set(candidates.map(c => c.overlay))`: insertion-ordered unique. A
    // `Vec` of pairs keeps that order; assembling an overlay twice would be
    // wasteful but not wrong, and assembling them in hash order would change
    // nothing observable, so the order here is for the error message alone.
    let mut overlays: Vec<(String, Vec<u8>)> = Vec::new();
    for candidate in &candidates {
        if overlays.iter().any(|(name, _)| name == &candidate.overlay) {
            continue;
        }
        overlays.push((candidate.overlay.clone(), overlay_image(&candidate.overlay)?));
    }
    let tasks: Vec<Task<'_>> = candidates
        .iter()
        .flat_map(|candidate| configs.iter().map(move |config| Task { candidate, config }))
        .collect();
    let scores = parallel_map(&tasks, options.jobs as usize, &|task| {
        score_task(task, &overlays, &compiler_signature, &options)
    })?;

    let shared = by_config(&configs, &scores);
    let best = best_by_region(&candidates, &scores);
    let compiled = scores.iter().filter(|row| row.compiled()).count();
    let exact = scores.iter().filter(|row| row.exact()).count();
    let report = Json::Object(vec![
        ("format".into(), Json::Number(FORMAT as f64)),
        (
            "policy".into(),
            Json::Object(vec![
                ("clean_room".into(), Json::Bool(true)),
                ("auto_promote".into(), Json::Bool(false)),
                (
                    "compiler_configurations".into(),
                    Json::String("already exposed by tools/lib/mode_sweep.ts".into()),
                ),
            ]),
        ),
        (
            "candidates".into(),
            Json::Array(
                candidates
                    .iter()
                    .map(|candidate| {
                        Json::Object(vec![
                            ("id".into(), Json::String(candidate.id.clone())),
                            ("source".into(), Json::String(strip_root(&candidate.source))),
                            ("span_bytes".into(), Json::Number(candidate.span_bytes)),
                        ])
                    })
                    .collect(),
            ),
        ),
        (
            "configurations".into(),
            Json::Array(configs.iter().map(|config| config.to_json()).collect()),
        ),
        ("attempted".into(), Json::Number(scores.len() as f64)),
        ("compiled".into(), Json::Number(compiled as f64)),
        ("exact".into(), Json::Number(exact as f64)),
        (
            "shared".into(),
            Json::Array(shared.iter().map(|row| row.to_json()).collect()),
        ),
        (
            "best_by_region".into(),
            Json::Array(
                best.iter()
                    .map(|row| match row {
                        Some(score) => score.json.clone(),
                        None => Json::Null,
                    })
                    .collect(),
            ),
        ),
        (
            "results".into(),
            Json::Array(scores.iter().map(|score| score.json.clone()).collect()),
        ),
    ]);
    let report_path = Path::new(&options.output).join("report.json");
    write_json(&report_path, &report).map_err(|error| format!("{}: {error}", report_path.display()))?;

    let mut lines = vec![format!(
        "candidates={} configurations={} attempted={} compiled={compiled} exact={exact}",
        candidates.len(),
        configs.len(),
        scores.len()
    )];
    lines.push("shared configurations (improvements relative to routed baseline):".into());
    for row in shared.iter().take(options.top as usize) {
        lines.push(format!(
            "exact={} improved={} gain={}B differing={} {} {}",
            row.exact,
            row.improves_baseline_regions.len(),
            // `${NaN}` is the string "NaN", which this line CAN print; see the
            // improvement-bytes PORT NOTE in `aggregate`.
            js_number_text(row.improvement_bytes),
            js_number_text(row.total_differing_bytes),
            row.config.compiler_family,
            config_label(&row.config.ids)
        ));
    }
    lines.push("best per region:".into());
    for row in best.iter().take(options.top as usize) {
        lines.push(match row {
            None => "uncompiled".into(),
            Some(score) => format!(
                "{} exact={} differing={} {} {}",
                score.id(),
                score.exact(),
                match json::as_number(json::get(&score.json, "differing_bytes")) {
                    Some(value) => js_number_text(value),
                    // `?? "?"`.
                    None => "?".into(),
                },
                score_family(score),
                config_label(&score_ids(score))
            ),
        });
    }
    lines.push(format!("report={}", report_path.display()));
    Ok(lines)
}

/// `config.ids.join("+") || "routed-default"`.
///
/// PORT NOTE: `||`, so the EMPTY string from an empty `ids` falls through. `??`
/// would have printed a blank column for the routed baseline, which is the row
/// every other row is measured against.
fn config_label(ids: &[String]) -> String {
    let joined = ids.join("+");
    if joined.is_empty() {
        "routed-default".into()
    } else {
        joined
    }
}

fn score_family(score: &Score) -> String {
    score
        .config()
        .and_then(|config| json::as_str(json::get(config, "compiler_family")))
        .unwrap_or_default()
        .to_string()
}

fn score_ids(score: &Score) -> Vec<String> {
    let ids = score.config().and_then(|config| json::get(config, "ids"));
    if array_len(ids) == 0 {
        return Vec::new();
    }
    match ids {
        Some(Json::Array(items)) => items
            .iter()
            .map(|item| match item {
                Json::String(text) => text.clone(),
                other => canonical_json(other),
            })
            .collect(),
        _ => Vec::new(),
    }
}

/// The default output directory, exposed so tests and the parity harness can
/// assert they are NOT writing into `out/`.
pub fn default_output() -> PathBuf {
    root().join("out/overlay-mode-cohort")
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn the_self_test_passes() {
        assert_eq!(self_test().unwrap(), "overlay mode cohort self-test passed");
        assert_eq!(run(&["--self-test".to_string()]).unwrap().len(), 1);
    }

    #[test]
    fn subarray_clamps_where_a_rust_range_would_panic() {
        let buffer = [1u8, 2, 3, 4];
        assert_eq!(clamped_subarray(&buffer, 2.0, 99.0), &[3, 4]);
        assert_eq!(clamped_subarray(&buffer, 99.0, 120.0), &[] as &[u8]);
        assert_eq!(clamped_subarray(&buffer, -5.0, 2.0), &[1, 2]);
        // NaN coerces to 0 at both ends, so the result is empty and not a panic.
        assert_eq!(clamped_subarray(&buffer, f64::NAN, f64::NAN), &[] as &[u8]);
        assert_eq!(clamped_subarray(&buffer, 3.0, 1.0), &[] as &[u8]);
    }

    #[test]
    fn the_routed_baseline_prints_a_label_rather_than_a_blank() {
        assert_eq!(config_label(&[]), "routed-default");
        assert_eq!(
            config_label(&["a".to_string(), "b".to_string()]),
            "a+b"
        );
    }

    #[test]
    fn the_include_line_is_quoted_the_way_json_quotes() {
        let awkward = Json::String("a\\b\"c.c".into());
        assert_eq!(canonical_json(&awkward), "\"a\\\\b\\\"c.c\"");
    }

    #[test]
    fn the_default_output_directory_is_the_one_the_harness_must_override() {
        // The TypeScript writes into `out/`. The port keeps that default for
        // parity, and every test and harness run passes `-o` into
        // `target/fixtures/` instead. This test exists so that a change to the
        // default is deliberate.
        assert!(default_output().ends_with("out/overlay-mode-cohort"));
    }
}
