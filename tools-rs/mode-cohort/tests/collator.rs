//! Replays 27,225 `localeCompare` results MEASURED in Bun against the ported
//! collator, so a "simplification" back to `str::cmp` or to a single-pass
//! interleaved table fails immediately rather than quietly reshuffling reports.
//!
//! The fixture covers every printable-ASCII punctuation character in three
//! positions (leading, medial, trailing), the digits, the case pairs, and the
//! real shapes this tool sorts: eight-hex-digit stems, absolute `.c` paths and
//! `+`-joined mode id lists.

use mode_cohort::collate;
use search_compiler_modes::{parse_json, Json};
use std::cmp::Ordering;

#[test]
fn matches_every_measured_locale_compare() {
    let text = std::fs::read_to_string(concat!(
        env!("CARGO_MANIFEST_DIR"),
        "/tests/collator_pairs.json"
    ))
    .expect("measured collator fixture is present");
    let pairs = parse_json(&text).expect("fixture parses");
    let pairs = pairs.as_array().expect("fixture is an array");

    // HARD FLOOR: an empty or truncated fixture must not read as a clean sweep.
    assert!(
        pairs.len() >= 20_000,
        "fixture shrank to {} pairs; refusing to call that a pass",
        pairs.len()
    );

    let mut checked = 0usize;
    let mut less = 0usize;
    let mut greater = 0usize;
    let mut equal = 0usize;
    let mut disagrees_with_ascii = 0usize;
    for pair in pairs {
        let items = pair.as_array().expect("pair is an array");
        let left = items[0].as_str().expect("left is a string");
        let right = items[1].as_str().expect("right is a string");
        let expected = match items[2] {
            Json::Number(value) if value < 0.0 => Ordering::Less,
            Json::Number(value) if value > 0.0 => Ordering::Greater,
            _ => Ordering::Equal,
        };
        assert_eq!(
            collate(left, right),
            expected,
            "collate({left:?}, {right:?}) disagrees with the measured Bun result"
        );
        if left.cmp(right) != expected {
            disagrees_with_ascii += 1;
        }
        match expected {
            Ordering::Less => less += 1,
            Ordering::Greater => greater += 1,
            Ordering::Equal => equal += 1,
        }
        checked += 1;
    }

    // Floors per outcome, so the corpus cannot go degenerate in one direction.
    assert!(checked >= 20_000, "only {checked} pairs checked");
    assert!(less >= 5_000 && greater >= 5_000, "{less} less, {greater} greater");
    assert!(equal >= 100, "only {equal} equal pairs");
    // The whole point of the exercise: raw ASCII order gets a large number of
    // these wrong. If this ever drops to zero the fixture has lost its teeth.
    assert!(
        disagrees_with_ascii >= 1_000,
        "only {disagrees_with_ascii} pairs distinguish the collator from str::cmp"
    );
}
