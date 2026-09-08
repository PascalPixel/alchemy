//! Byte differences and structural evidence are separate measurements.
//! Structural equality does not establish a byte-exact match.
pub mod insns;
pub mod topology;
use std::collections::BTreeSet;

/// Offsets of unequal fixed-width units, including a partial trailing unit.
/// Missing bytes differ; equal odd-length inputs do not. Width must be nonzero.
pub fn differing_offsets(actual: &[u8], expected: &[u8], width: usize) -> BTreeSet<usize> {
    assert!(width != 0, "comparison unit width must be nonzero");
    (0..actual.len().max(expected.len()))
        .step_by(width)
        .filter(|&at| {
            let left = actual.get(at..actual.len().min(at.saturating_add(width)));
            let right = expected.get(at..expected.len().min(at.saturating_add(width)));
            left != right
        })
        .collect()
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn all_lengths_and_unit_widths_count_each_difference_once() {
        for width in 1..=4 {
            for a in 0..=9 {
                for b in 0..=9 {
                    let left = vec![7; a];
                    let mut right = vec![7; b];
                    let oracle = |right: &[u8]| {
                        (0..a.max(b))
                            .filter(|i| left.get(*i) != right.get(*i))
                            .map(|i| i / width * width)
                            .collect::<BTreeSet<_>>()
                    };
                    assert_eq!(differing_offsets(&left, &right, width), oracle(&right));
                    for i in 0..b {
                        right[i] = 8;
                        assert_eq!(differing_offsets(&left, &right, width), oracle(&right));
                        assert_eq!(differing_offsets(&right, &left, width), oracle(&right));
                        right[i] = 7;
                    }
                }
            }
        }
    }
    #[test]
    #[should_panic(expected = "width must be nonzero")]
    fn zero_width_is_rejected() {
        differing_offsets(&[], &[], 0);
    }
}
