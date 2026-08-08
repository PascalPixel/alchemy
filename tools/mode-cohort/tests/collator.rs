use mode_cohort::collate;
use std::cmp::Ordering;

#[test]
fn native_collator_cases_cover_report_ordering() {
    // These are the actual strings mode-cohort sorts: mode IDs, source stems,
    // and repository paths. Expected ordering is part of the native report
    // contract; no external runtime or generated comparison corpus is needed.
    let cases = [
        ("a-b", "a+b", Ordering::Less),
        ("a=b", "a0b", Ordering::Less),
        ("a", "A", Ordering::Less),
        ("a-c", "ab", Ordering::Less),
        ("0800000a", "0800000b", Ordering::Less),
        ("tools/mode-sweep/src/main.rs", "tools/mode-sweep/src/lib.rs", Ordering::Greater),
        ("no-gcse", "nogcse", Ordering::Less),
    ];
    for (left, right, expected) in cases {
        assert_eq!(collate(left, right), expected, "collate({left:?}, {right:?})");
        assert_eq!(collate(right, left), expected.reverse(), "reverse ordering");
    }

    let mut values = vec![
        "a-b".to_string(),
        "a+b".to_string(),
        "A".to_string(),
        "a".to_string(),
    ];
    values.sort_by(|left, right| collate(left, right));
    assert_eq!(values, vec!["a", "A", "a-b", "a+b"]);
}
