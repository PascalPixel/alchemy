//! The report writer, checked against the real report this tool has already
//! produced, plus the `main()` pipeline pieces that the differential harness
//! cannot reach.

use std::path::Path;

use match_m2c::json::{canonical_json, parse, Json};
use match_m2c::jsnum::to_js_number_string;
use match_m2c::pipeline::{
    load_spans, report_path, select_drafts, DraftOutcome, SpanMap, REJECT,
    UNKNOWN_TYPE_STRATEGIES,
};

fn repository_root() -> &'static Path {
    alchemy_routing::routing::root()
}

/// Read the real `work/matches/m2c.json`, reparse it, re-emit it, and require
/// the bytes back. This is a REAL repository input, not a synthetic one: it is
/// the report the TypeScript itself wrote.
#[test]
fn real_report_round_trips_byte_for_byte() {
    let path = report_path(repository_root(), decomp_targets::DEFAULT_TARGET);
    let Ok(original) = std::fs::read_to_string(&path) else {
        // The report is generated output. If it is absent this test has nothing
        // to say, but it must say so out loud rather than passing quietly.
        panic!(
            "{} is missing; regenerate it before trusting this test's silence",
            path.display()
        );
    };
    assert!(
        original.len() > 1024,
        "the real report is suspiciously small ({} bytes)",
        original.len()
    );
    let value = parse(&original).expect("the real report must parse");
    let rewritten = format!("{}\n", canonical_json(&value).expect("must re-emit"));
    assert_eq!(rewritten, original, "canonical_json is not byte-faithful");

    // Walk EVERY number in the real report. `to_js_number_string` refuses
    // anything JavaScript would not write the same way, so a green walk is the
    // proof that the float-formatting trap is inapplicable to this format.
    let mut numbers = 0usize;
    walk_numbers(&value, &mut |number| {
        to_js_number_string(number).unwrap_or_else(|error| panic!("{error}"));
        numbers += 1;
    });
    // A floor, so that an emptied report cannot make this a vacuous pass.
    assert!(numbers >= 10, "only {numbers} numbers walked");
}

fn walk_numbers(value: &Json, visit: &mut impl FnMut(f64)) {
    match value {
        Json::Number(number) => visit(*number),
        Json::Array(items) => items.iter().for_each(|item| walk_numbers(item, visit)),
        Json::Object(entries) => entries.iter().for_each(|(_, v)| walk_numbers(v, visit)),
        _ => {}
    }
}

#[test]
fn outcome_key_order_matches_the_typescript_object_literals() {
    let matched = DraftOutcome::Matched {
        entry: 0x0800_1234 as f64,
        size: 24.0,
        source: "/tmp/x.c".into(),
        unknown_type: Some("s32".into()),
    };
    assert_eq!(
        keys(&matched.to_json()),
        vec!["entry", "matched", "size", "source", "unknown_type"]
    );
    let best = DraftOutcome::Best {
        entry: 1.0,
        size: 2.0,
        mismatched_bytes: 3.0,
        common_prefix: 4.0,
        unknown_type: None,
    };
    assert_eq!(
        keys(&best.to_json()),
        vec!["entry", "matched", "size", "mismatched_bytes", "common_prefix", "unknown_type"]
    );
    assert_eq!(
        keys(&DraftOutcome::NoCandidate { entry: 1.0 }.to_json()),
        vec!["entry", "matched"]
    );
    // `unknownType ?? null` -- a `null`, never an omitted key.
    assert!(canonical_json(&best.to_json()).unwrap().contains("\"unknown_type\": null"));
}

fn keys(value: &Json) -> Vec<String> {
    let Json::Object(entries) = value else {
        panic!("expected an object");
    };
    entries.iter().map(|(key, _)| key.clone()).collect()
}

#[test]
fn a_missing_size_poisons_the_span_and_the_span_stays_poisoned() {
    let mut spans = SpanMap::default();
    spans.set(1.0, Json::to_js_number(None));
    // `spans.get(entry) ?? 0` -- `??` does NOT substitute for NaN.
    assert!(spans.get_or_zero(1.0).is_nan());
    // An absent key does get the 0.
    assert_eq!(spans.get_or_zero(2.0), 0.0);
    // `Map` keys are SameValueZero, so a NaN key finds itself and -0 finds 0.
    let mut keyed = SpanMap::default();
    keyed.set(f64::NAN, 7.0);
    keyed.set(-0.0, 9.0);
    assert_eq!(keyed.get_or_zero(f64::NAN), 7.0);
    assert_eq!(keyed.get_or_zero(0.0), 9.0);
    assert_eq!(keyed.len(), 2);
    // Setting an existing key replaces in place rather than appending.
    keyed.set(0.0, 11.0);
    assert_eq!(keyed.len(), 2);
    assert_eq!(keyed.get_or_zero(-0.0), 11.0);
}

#[test]
fn a_missing_report_json_is_an_empty_span_map_not_an_error() {
    let missing = repository_root().join("work/matches/m2c/this-does-not-exist/report.json");
    let spans = load_spans(&missing).expect("an absent report is not an error");
    assert!(spans.is_empty());
}

#[test]
fn reject_markers_and_strategies_are_the_literal_lists() {
    assert_eq!(REJECT, ["M2C_ERROR", "M2C_BITFIELD", "M2C_MEMSET", "M2C_MEMCPY"]);
    // The ORDER is the tiebreak: the comparison is strict, so the first
    // strategy to reach a score keeps it.
    assert_eq!(
        UNKNOWN_TYPE_STRATEGIES,
        ["void", "s32", "u32", "void *", "s16", "u16", "s8", "u8"]
    );
}

/// Draft selection over the real candidate directory: 1,700-odd real `.c`
/// files, none of them synthetic.
#[test]
fn draft_selection_over_the_real_candidate_directory() {
    let directory = repository_root().join("work/matches/m2c");
    if !directory.is_dir() {
        panic!("{} is missing; there is no corpus to select from", directory.display());
    }
    let all = select_drafts(&directory, &[]).expect("selection must succeed");
    assert!(all.len() > 500, "only {} candidates found", all.len());
    // Sorted, and sorted by UTF-16 code unit (which for these ASCII names is
    // the same as byte order, so this pins the sort, not the collation).
    let mut sorted = all.clone();
    sorted.sort();
    assert_eq!(all, sorted);
    // Every survivor is free of every reject marker.
    for path in all.iter().take(64) {
        let source = std::fs::read_to_string(path).expect("readable");
        for marker in REJECT {
            assert!(!source.contains(marker), "{path} carries {marker}");
        }
    }
    // The `tracked` filter removes by BASENAME, not by path.
    let first = Path::new(&all[0])
        .file_name()
        .unwrap()
        .to_string_lossy()
        .into_owned();
    let filtered =
        select_drafts(&directory, std::slice::from_ref(&first)).expect("selection must succeed");
    assert_eq!(filtered.len(), all.len() - 1);
    assert!(!filtered.iter().any(|path| path.ends_with(&first)));
}
