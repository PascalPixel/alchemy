use std::collections::BTreeSet;
pub fn differing_offsets(actual: &[u8], expected: &[u8]) -> BTreeSet<usize> {
    let shared = actual.len().min(expected.len());
    let mut out = BTreeSet::new();
    for at in (0..shared.saturating_sub(1)).step_by(2) {
        if actual[at..at + 2] != expected[at..at + 2] {
            out.insert(at);
        }
    }
    for at in (shared & !1..actual.len().max(expected.len())).step_by(2) {
        out.insert(at);
    }
    out
}
pub fn self_test() -> Result<String, String> {
    let a = [0x00, 0xb5, 0x01, 0x20];
    let b = [0x00, 0xb5, 0x02, 0x20];
    if !differing_offsets(&a, &a).is_empty() || differing_offsets(&a, &b) != [2].into() || differing_offsets(&a, &a[..2]).len() != 1 {
        return Err("candidate show self-test failed".into());
    }
    Ok("candidate show self-test passed".into())
}
