//! Unit-cost sequence alignment using scalar dynamic programming.
//! Equal spans are merged before operand-level scoring.

type Block = (usize, usize, usize);

fn append(out: &mut Vec<Block>, a: usize, b: usize, length: usize) {
    if length == 0 {
        return;
    }
    if let Some((i, j, n)) = out
        .last_mut()
        .filter(|(i, j, n)| *i + *n == a && *j + *n == b)
    {
        let _ = (i, j);
        *n += length;
    } else {
        out.push((a, b, length));
    }
}

fn row(a: &[&str], b: &[&str], reverse: bool) -> Vec<usize> {
    let mut previous: Vec<_> = (0..=a.len()).collect();
    let mut current = vec![0; a.len() + 1];
    for j in 0..b.len() {
        current[0] = j + 1;
        let rhs = b[if reverse { b.len() - j - 1 } else { j }];
        for i in 0..a.len() {
            let lhs = a[if reverse { a.len() - i - 1 } else { i }];
            current[i + 1] = (current[i] + 1)
                .min(previous[i + 1] + 1)
                .min(previous[i] + usize::from(lhs != rhs));
        }
        std::mem::swap(&mut previous, &mut current);
    }
    previous
}

fn align(a: &[&str], b: &[&str], src: usize, dst: usize, max: usize, out: &mut Vec<Block>) {
    let prefix = a.iter().zip(b).take_while(|(a, b)| a == b).count();
    append(out, src, dst, prefix);
    let (a, b) = (&a[prefix..], &b[prefix..]);
    let (src, dst) = (src + prefix, dst + prefix);
    let suffix = a
        .iter()
        .rev()
        .zip(b.iter().rev())
        .take_while(|(a, b)| a == b)
        .count();
    let (a, b) = (&a[..a.len() - suffix], &b[..b.len() - suffix]);
    let (n, m) = (a.len(), b.len());
    let max = max.min(n.max(m));
    let band = n.min(max.saturating_mul(2).saturating_add(1));
    // Split above the distance-band threshold. This boundary is part of
    // deterministic tie-breaking, independent of the matrix's allocation size.
    if 2usize.saturating_mul(band).saturating_mul(m) / 8 >= 1024 * 1024 && n >= 65 && m >= 10 {
        let mid = m / 2;
        let left = row(a, &b[..mid], false);
        let right = row(a, &b[mid..], true);
        let split = (0..=n).min_by_key(|&i| left[i] + right[n - i]).unwrap();
        align(&a[..split], &b[..mid], src, dst, left[split], out);
        align(
            &a[split..],
            &b[mid..],
            src + split,
            dst + mid,
            right[n - split],
            out,
        );
    } else if n != 0 && m != 0 {
        let width = n + 1;
        let mut matrix = vec![0usize; width * (m + 1)];
        for (i, value) in matrix[..width].iter_mut().enumerate() {
            *value = i;
        }
        for j in 1..=m {
            matrix[j * width] = j;
            for i in 1..=n {
                let at = j * width + i;
                matrix[at] = (matrix[at - 1] + 1)
                    .min(matrix[at - width] + 1)
                    .min(matrix[at - width - 1] + usize::from(a[i - 1] != b[j - 1]));
            }
        }
        let (mut i, mut j) = (n, m);
        let mut equal = Vec::new();
        while i != 0 && j != 0 {
            let at = j * width + i;
            if matrix[at] == matrix[at - 1] + 1 {
                i -= 1;
            } else if j > 1 && matrix[at - width] + 1 == matrix[at - width - 1] {
                j -= 1;
            } else {
                i -= 1;
                j -= 1;
                if a[i] == b[j] {
                    equal.push((src + i, dst + j));
                }
            }
        }
        for (i, j) in equal.into_iter().rev() {
            append(out, i, j, 1);
        }
    }
    append(out, src + n, dst + m, suffix);
}

pub(super) fn matching_blocks(a: &[&str], b: &[&str]) -> Vec<Block> {
    let mut blocks = Vec::new();
    align(a, b, 0, 0, a.len().max(b.len()), &mut blocks);
    blocks.push((a.len(), b.len(), 0));
    blocks
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn alignments_match_native_opcodes() {
        for row in include_str!("../c/fixtures/levenshtein-alignments.tsv").lines() {
            let c: Vec<_> = row.split('\t').collect();
            let chars = |s: &str| {
                s.as_bytes()
                    .chunks(1)
                    .map(|c| std::str::from_utf8(c).unwrap().to_string())
                    .collect::<Vec<_>>()
            };
            let (a, b) = (chars(c[0]), chars(c[1]));
            let actual = matching_blocks(
                &a.iter().map(String::as_str).collect::<Vec<_>>(),
                &b.iter().map(String::as_str).collect::<Vec<_>>(),
            );
            let expected: Vec<_> = c[2]
                .split(';')
                .map(|b| {
                    let n: Vec<usize> = b
                        .split_whitespace()
                        .skip(1)
                        .step_by(2)
                        .map(|n| n.parse().unwrap())
                        .collect();
                    (n[0], n[1], n[2])
                })
                .collect();
            assert_eq!(actual, expected, "{} -> {}", c[0], c[1]);
        }
    }
}
