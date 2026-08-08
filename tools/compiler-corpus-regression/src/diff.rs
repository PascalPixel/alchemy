//! `byteDifference(actual, expected)`.

/// The result of `byteDifference`. `first` is `null` in the TypeScript.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct ByteDifference {
    pub count: usize,
    pub first: Option<usize>,
}

/// `byteDifference(actual, expected)`.
///
/// PORT NOTE -- THIS ONE IS CORRECT, and the note exists so that a later
/// "simplification" cannot quietly make it wrong. A sibling tool
/// (`candidate_show.ts`) reports a PHANTOM difference on two identical
/// odd-length buffers because it walks `shared & ~1`, which falls BELOW
/// `Math.max(actual.length, expected.length)` and so counts the final odd byte
/// as differing. The shape here is different: `extent` is the plain
/// `Math.max`, and the per-offset test is
///
/// ```text
/// if (actual[o] === expected[o] && o < actual.length && o < expected.length) continue;
/// ```
///
/// Reading past the end of a JavaScript typed array yields `undefined`, and
/// `undefined === undefined` is TRUE, so without those two length guards every
/// offset beyond BOTH lengths would be counted equal. They are present, so the
/// tail past the shorter buffer is counted as differing, once per byte, which
/// is what the report means by `differing_bytes`. `tests/diff.rs` pins the
/// identical-odd-length case at zero and the phantom shape as absent.
///
/// PORT NOTE -- `actual[offset]` PANICS in Rust where JavaScript returns
/// `undefined`. The loop below never indexes without a bounds test for that
/// reason; the guards are not redundant with the `get` calls, they ARE the
/// port of `undefined`.
pub fn byte_difference(actual: &[u8], expected: &[u8]) -> ByteDifference {
    // `Math.max` over two `usize` lengths: no NaN can reach this, unlike the
    // `f64` `Math.max` elsewhere in the port.
    let extent = actual.len().max(expected.len());
    let mut count = 0usize;
    let mut first: Option<usize> = None;
    for offset in 0..extent {
        let same = actual.get(offset) == expected.get(offset);
        let in_both = offset < actual.len() && offset < expected.len();
        if same && in_both {
            continue;
        }
        count += 1;
        if first.is_none() {
            first = Some(offset);
        }
    }
    ByteDifference { count, first }
}
