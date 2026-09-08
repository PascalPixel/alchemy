//! Byte differences and structural evidence are separate measurements.
//! Structural equality does not establish a byte-exact match.
pub mod insns;
pub mod topology;
use std::collections::BTreeSet;

/// Weighted sequence alignment with deterministic left-first gap ties.
/// A zero score forbids a pair; alignment is structural evidence, not byte equality.
pub fn alignment_indices(
    left: &[String],
    right: &[String],
    score: impl Copy + Fn(&str, &str) -> usize,
) -> Vec<(Option<usize>, Option<usize>)> {
    let mut table = vec![vec![0; right.len() + 1]; left.len() + 1];
    for i in (0..left.len()).rev() {
        for j in (0..right.len()).rev() {
            let pair = score(&left[i], &right[j]);
            let diagonal = (pair != 0).then(|| table[i + 1][j + 1] + pair);
            table[i][j] = diagonal
                .unwrap_or_default()
                .max(table[i + 1][j])
                .max(table[i][j + 1]);
        }
    }
    let (mut i, mut j) = (0, 0);
    let mut pairs = Vec::new();
    while i < left.len() && j < right.len() {
        let pair = score(&left[i], &right[j]);
        if pair != 0 && table[i][j] == table[i + 1][j + 1] + pair {
            pairs.push((Some(i), Some(j)));
            i += 1;
            j += 1;
        } else if table[i + 1][j] >= table[i][j + 1] {
            pairs.push((Some(i), None));
            i += 1;
        } else {
            pairs.push((None, Some(j)));
            j += 1;
        }
    }
    pairs.extend((i..left.len()).map(|index| (Some(index), None)));
    pairs.extend((j..right.len()).map(|index| (None, Some(index))));
    pairs
}

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
    fn alignment_preserves_every_index_and_maximizes_weight() {
        fn best(left: &[String], right: &[String]) -> usize {
            if left.is_empty() || right.is_empty() {
                return 0;
            }
            let pair = if left[0] == right[0] {
                2 + best(&left[1..], &right[1..])
            } else {
                0
            };
            pair.max(best(&left[1..], right))
                .max(best(left, &right[1..]))
        }
        let sequences: Vec<Vec<String>> = (0..5)
            .flat_map(|len| {
                (0..1 << len)
                    .map(move |bits| (0..len).map(|i| ((bits >> i) & 1).to_string()).collect())
            })
            .collect();
        for left in &sequences {
            for right in &sequences {
                let pairs = alignment_indices(left, right, |a, b| if a == b { 2 } else { 0 });
                assert_eq!(
                    pairs.iter().filter_map(|p| p.0).collect::<Vec<_>>(),
                    (0..left.len()).collect::<Vec<_>>()
                );
                assert_eq!(
                    pairs.iter().filter_map(|p| p.1).collect::<Vec<_>>(),
                    (0..right.len()).collect::<Vec<_>>()
                );
                let weight: usize = pairs
                    .iter()
                    .map(|&(a, b)| match (a, b) {
                        (Some(a), Some(b)) => {
                            assert_eq!(left[a], right[b]);
                            2
                        }
                        _ => 0,
                    })
                    .sum();
                assert_eq!(weight, best(left, right));
            }
        }
        assert_eq!(
            alignment_indices(&["a".into()], &["b".into()], |_, _| 0),
            vec![(Some(0), None), (None, Some(0))]
        );
    }
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
