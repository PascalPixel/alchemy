//! Score a candidate's rows against the reference rows, as decomp-permuter
//! scores objdump output: align the mnemonic sequences, then charge each
//! aligned pair by how its operands differ and each unaligned row as an
//! insertion, a deletion or, when it appears on both sides, a reordering.

use super::routine::{Routine, Row};
use regex::Regex;
use std::collections::BTreeMap;
use std::sync::OnceLock;

pub const STACK: u64 = 1;
pub const REGISTER: u64 = 5;
pub const OPERAND: u64 = 20;
pub const REORDER: u64 = 60;
pub const INSERT: u64 = 100;
pub const DELETE: u64 = 100;

#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub struct Score {
    pub total: u64,
    pub exact: bool,
    pub stack: usize,
    pub register: usize,
    pub operand: usize,
    pub reordered: usize,
    pub inserted: usize,
    pub deleted: usize,
    /// The differing rows: class, reference text, candidate text.
    pub lines: Vec<(&'static str, String, String)>,
}

impl Score {
    pub fn summary(&self) -> String {
        if self.exact {
            return "0 (exact)".into();
        }
        let mut parts = Vec::new();
        for (count, name) in [
            (self.register, "register-only"),
            (self.stack, "stack-only"),
            (self.operand, "operand"),
            (self.reordered, "reordered"),
            (self.inserted, "inserted"),
            (self.deleted, "deleted"),
        ] {
            if count > 0 {
                parts.push(format!("{count} {name}"));
            }
        }
        if parts.is_empty() {
            parts.push("layout".into());
        }
        format!("{} ({})", self.total, parts.join(", "))
    }
}

fn registers() -> &'static Regex {
    static PATTERN: OnceLock<Regex> = OnceLock::new();
    PATTERN.get_or_init(|| Regex::new(r"\br(?:1[0-2]|[0-9])\b").expect("static pattern"))
}

fn stack_offsets() -> &'static Regex {
    static PATTERN: OnceLock<Regex> = OnceLock::new();
    PATTERN.get_or_init(|| Regex::new(r"sp, #-?\d+").expect("static pattern"))
}

/// Longest common subsequence of the two key sequences, as index pairs.
pub fn align(reference: &[&str], candidate: &[&str]) -> Vec<(usize, usize)> {
    let (n, m) = (reference.len(), candidate.len());
    let mut table = vec![0u32; (n + 1) * (m + 1)];
    let at = |i: usize, j: usize| i * (m + 1) + j;
    for i in (0..n).rev() {
        for j in (0..m).rev() {
            table[at(i, j)] = if reference[i] == candidate[j] {
                table[at(i + 1, j + 1)] + 1
            } else {
                table[at(i + 1, j)].max(table[at(i, j + 1)])
            };
        }
    }
    let mut pairs = Vec::new();
    let (mut i, mut j) = (0, 0);
    while i < n && j < m {
        if reference[i] == candidate[j] {
            pairs.push((i, j));
            i += 1;
            j += 1;
        } else if table[at(i + 1, j)] >= table[at(i, j + 1)] {
            i += 1;
        } else {
            j += 1;
        }
    }
    pairs
}

fn same_target(reference: &Row, candidate: &Row, mapped: &BTreeMap<u32, u32>) -> bool {
    match (reference.target, candidate.target) {
        (None, None) => true,
        (Some(expected), Some(found)) => mapped.get(&found) == Some(&expected),
        _ => false,
    }
}

pub fn score(reference: &Routine, candidate: &Routine) -> Score {
    let reference_keys: Vec<&str> = reference.rows.iter().map(|row| row.key.as_str()).collect();
    let candidate_keys: Vec<&str> = candidate.rows.iter().map(|row| row.key.as_str()).collect();
    let pairs = align(&reference_keys, &candidate_keys);
    // Candidate offsets of aligned rows map to reference offsets.
    let mapped: BTreeMap<u32, u32> = pairs
        .iter()
        .map(|&(i, j)| (candidate.rows[j].offset, reference.rows[i].offset))
        .collect();
    let mut result = Score::default();
    let mut exact = reference.size == candidate.size
        && reference.rows.len() == candidate.rows.len()
        && pairs.len() == reference.rows.len()
        && reference.padding == candidate.padding
        && reference.pool == candidate.pool;
    for &(i, j) in &pairs {
        let (expected, found) = (&reference.rows[i], &candidate.rows[j]);
        let same_text = expected.text == found.text;
        let targets = same_target(expected, found, &mapped);
        if exact
            && (expected.offset != found.offset
                || !same_text
                || expected.target != found.target
                || (expected.literal && found.literal && expected.raw != found.raw))
        {
            exact = false;
        }
        if same_text && targets {
            continue;
        }
        let masked = |text: &str| registers().replace_all(text, "r?").into_owned();
        let unstacked = |text: &str| stack_offsets().replace_all(text, "sp, #?").into_owned();
        let class = if !targets {
            result.operand += 1;
            result.total += OPERAND;
            "operand"
        } else if unstacked(&expected.text) == unstacked(&found.text) {
            result.stack += 1;
            result.total += STACK;
            "stack"
        } else if masked(&unstacked(&expected.text)) == masked(&unstacked(&found.text)) {
            result.register += 1;
            result.total += REGISTER;
            "register"
        } else {
            result.operand += 1;
            result.total += OPERAND;
            "operand"
        };
        result
            .lines
            .push((class, expected.text.clone(), found.text.clone()));
    }
    // Unaligned rows: a row missing on one side and extra on the other is a
    // reordering; the rest are insertions and deletions.
    let mut paired_reference = vec![false; reference.rows.len()];
    let mut paired_candidate = vec![false; candidate.rows.len()];
    for &(i, j) in &pairs {
        paired_reference[i] = true;
        paired_candidate[j] = true;
    }
    // Moved rows match by text first, then by text with registers masked (a
    // moved row that also changed registers).
    let masked = |text: &str| {
        registers()
            .replace_all(&stack_offsets().replace_all(text, "sp, #?"), "r?")
            .into_owned()
    };
    let mut unmatched: Vec<usize> = (0..candidate.rows.len())
        .filter(|&j| !paired_candidate[j])
        .collect();
    for (pass, penalty, class) in [(0, REORDER, "reordered"), (1, REORDER + REGISTER, "moved")] {
        let key = |text: &str| {
            if pass == 0 {
                text.to_string()
            } else {
                masked(text)
            }
        };
        let mut deleted: BTreeMap<String, Vec<usize>> = BTreeMap::new();
        for (i, row) in reference.rows.iter().enumerate() {
            if !paired_reference[i] {
                deleted.entry(key(&row.text)).or_default().push(i);
            }
        }
        unmatched.retain(|&j| {
            let row = &candidate.rows[j];
            let Some(i) = deleted
                .get_mut(&key(&row.text))
                .and_then(|found| found.pop())
            else {
                return true;
            };
            paired_reference[i] = true;
            result.reordered += 1;
            result.total += penalty;
            result
                .lines
                .push((class, reference.rows[i].text.clone(), row.text.clone()));
            false
        });
    }
    for j in unmatched {
        result.inserted += 1;
        result.total += INSERT;
        result
            .lines
            .push(("inserted", String::new(), candidate.rows[j].text.clone()));
    }
    for (i, row) in reference.rows.iter().enumerate() {
        if !paired_reference[i] {
            result.deleted += 1;
            result.total += DELETE;
            result
                .lines
                .push(("deleted", row.text.clone(), String::new()));
        }
    }
    result.exact = exact && result.total == 0;
    if result.total == 0 && !result.exact {
        // Same instructions, different layout: never report a match.
        result.total = 1;
    }
    result
}

#[cfg(test)]
mod tests {
    use super::super::routine::Routine;
    use super::*;

    fn rows(texts: &[&str]) -> Routine {
        Routine {
            rows: texts
                .iter()
                .enumerate()
                .map(|(index, text)| Row {
                    offset: index as u32 * 2,
                    key: text.split_whitespace().next().unwrap().into(),
                    text: text.to_string(),
                    target: None,
                    raw: text.as_bytes().to_vec(),
                    literal: true,
                })
                .collect(),
            size: texts.len() as u32 * 2,
            padding: Vec::new(),
            pool: Vec::new(),
            unresolved: Default::default(),
        }
    }

    #[test]
    fn register_differences_cost_less_than_operand_differences() {
        let reference = rows(&[
            "push {r5, lr}",
            "ldrh r3, [r0, r1]",
            "lsrs r0, r3, #9",
            "bx r1",
        ]);
        let same = score(
            &reference,
            &rows(&[
                "push {r5, lr}",
                "ldrh r3, [r0, r1]",
                "lsrs r0, r3, #9",
                "bx r1",
            ]),
        );
        assert!(same.exact && same.total == 0);
        let swapped = score(
            &reference,
            &rows(&[
                "push {r5, lr}",
                "ldrh r3, [r1, r0]",
                "lsrs r0, r3, #9",
                "bx r1",
            ]),
        );
        assert_eq!(
            (swapped.total, swapped.register, swapped.exact),
            (REGISTER, 1, false)
        );
        let operand = score(
            &reference,
            &rows(&[
                "push {r5, lr}",
                "ldrh r3, [r0, #2]",
                "lsrs r0, r3, #9",
                "bx r1",
            ]),
        );
        assert_eq!((operand.total, operand.operand), (OPERAND, 1));
        let stack = score(&rows(&["ldr r0, [sp, #4]"]), &rows(&["ldr r0, [sp, #8]"]));
        assert_eq!((stack.total, stack.stack), (STACK, 1));
    }

    #[test]
    fn moved_rows_are_reorderings_and_missing_rows_insertions() {
        let reference = rows(&["movs r0, #1", "adds r1, #2", "str r0, [r2]", "bx lr"]);
        let moved = score(
            &reference,
            &rows(&["adds r1, #2", "movs r0, #1", "str r0, [r2]", "bx lr"]),
        );
        assert_eq!((moved.reordered, moved.total), (1, REORDER));
        let extra = score(
            &reference,
            &rows(&[
                "movs r0, #1",
                "adds r1, #2",
                "mov r8, r0",
                "str r0, [r2]",
                "bx lr",
            ]),
        );
        assert_eq!((extra.inserted, extra.total), (1, INSERT));
        let missing = score(&reference, &rows(&["movs r0, #1", "str r0, [r2]", "bx lr"]));
        assert_eq!((missing.deleted, missing.total), (1, DELETE));
        // Moved and renamed: a reordering that also changed registers.
        let renamed = score(
            &reference,
            &rows(&["adds r3, #2", "movs r0, #1", "str r0, [r2]", "bx lr"]),
        );
        assert_eq!((renamed.reordered, renamed.total), (1, REORDER + REGISTER));
    }

    #[test]
    fn identical_text_with_different_bytes_is_never_a_match() {
        let reference = rows(&["movs r0, #1", "bx lr"]);
        let mut candidate = rows(&["movs r0, #1", "bx lr"]);
        candidate.padding.push((4, vec![0xc0, 0x46]));
        let result = score(&reference, &candidate);
        assert!(!result.exact);
        assert_eq!(result.total, 1);
    }
}
