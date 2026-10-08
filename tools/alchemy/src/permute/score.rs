//! The weighted score beside `alchemy diff`'s differ, as pret's
//! decomp-permuter weighs objdump rows: an instruction that differs only in
//! its registers (or a pc-relative or branch distance) costs REGALLOC, one
//! found elsewhere in the function costs REORDER, and one with no
//! counterpart costs INSERT or DELETE. Both count the same normalized rows
//! `alchemy diff` compares, so the weighted score is 0 exactly when differ
//! is 0.

use regex::Regex;
use std::collections::BTreeMap;
use std::sync::OnceLock;

pub const REGALLOC: u64 = 10;
pub const REORDER: u64 = 60;
pub const INSERT: u64 = 100;
pub const DELETE: u64 = 100;

#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub struct Score {
    pub weighted: u64,
    /// `alchemy diff`'s count: listing + draft - 2 * common.
    pub differ: usize,
    pub regalloc: usize,
    pub reordered: usize,
    pub inserted: usize,
    pub deleted: usize,
    /// Rows that match from the start before diff's alignment first
    /// diverges.
    pub prefix: usize,
}

impl Score {
    /// What the search lowers: with `prefix`, a longer matched prefix
    /// first, then the weighted score.
    pub fn key(&self, prefix: bool) -> (std::cmp::Reverse<usize>, u64) {
        (std::cmp::Reverse(if prefix { self.prefix } else { 0 }), self.weighted)
    }

    /// The summary, with the matched prefix when the search ranks by it.
    pub fn summary_with(&self, prefix: bool) -> String {
        if prefix { format!("prefix {}, {}", self.prefix, self.summary()) } else { self.summary() }
    }

    pub fn summary(&self) -> String {
        let mut parts = Vec::new();
        for (count, name) in [
            (self.regalloc, "register"),
            (self.reordered, "reordered"),
            (self.inserted, "inserted"),
            (self.deleted, "deleted"),
        ] {
            if count > 0 {
                parts.push(format!("{count} {name}"));
            }
        }
        if parts.is_empty() {
            return "weighted 0, differ 0".into();
        }
        format!("weighted {}, differ {} ({})", self.weighted, self.differ, parts.join(", "))
    }
}

fn masked(text: &str) -> String {
    static REGISTERS: OnceLock<Regex> = OnceLock::new();
    static DISTANCES: OnceLock<Regex> = OnceLock::new();
    let registers = REGISTERS.get_or_init(|| Regex::new(r"\b(?:r1[0-2]|r[0-9]|sl|fp|ip|sb)\b").expect("static pattern"));
    let distances = DISTANCES.get_or_init(|| Regex::new(r"(\[pc, #)\d+|^(b\S*) [+-]\d+$").expect("static pattern"));
    let text = registers.replace_all(text, "r?");
    distances.replace_all(&text, "$1$2 ?").into_owned()
}

pub fn score(listing: &[String], draft: &[String]) -> Score {
    let (common, script) = crate::diff::compare(listing, draft);
    let prefix = script.iter().take_while(|line| line.starts_with("  ")).count();
    let mut result = Score { differ: listing.len() + draft.len() - 2 * common, prefix, ..Score::default() };
    // Runs of changed rows between common rows: pair a deleted and an
    // inserted row in the same run that differ only in registers.
    let mut deleted: Vec<String> = Vec::new();
    let mut inserted: Vec<String> = Vec::new();
    let mut run: (Vec<String>, Vec<String>) = (Vec::new(), Vec::new());
    let flush = |run: &mut (Vec<String>, Vec<String>), deleted: &mut Vec<String>, inserted: &mut Vec<String>, result: &mut Score| {
        let mut used = vec![false; run.1.len()];
        for old in run.0.drain(..) {
            let key = masked(&old);
            match (0..run.1.len()).find(|&j| !used[j] && masked(&run.1[j]) == key) {
                Some(j) => {
                    used[j] = true;
                    result.regalloc += 1;
                    result.weighted += REGALLOC;
                }
                None => deleted.push(old),
            }
        }
        inserted.extend(run.1.drain(..).zip(used).filter(|(_, used)| !used).map(|(new, _)| new));
    };
    for line in &script {
        match line.split_at(2) {
            ("- ", text) => run.0.push(text.to_string()),
            ("+ ", text) => run.1.push(text.to_string()),
            _ => flush(&mut run, &mut deleted, &mut inserted, &mut result),
        }
    }
    flush(&mut run, &mut deleted, &mut inserted, &mut result);
    // Rows found elsewhere: exactly (a reordering) or with other registers.
    for (exact, cost) in [(true, REORDER), (false, REORDER + REGALLOC)] {
        let key = |text: &str| if exact { text.to_string() } else { masked(text) };
        let mut missing: BTreeMap<String, usize> = BTreeMap::new();
        for old in &deleted {
            *missing.entry(key(old)).or_default() += 1;
        }
        let mut moved: BTreeMap<String, usize> = BTreeMap::new();
        inserted.retain(|new| match missing.get_mut(&key(new)) {
            Some(count) if *count > 0 => {
                *count -= 1;
                *moved.entry(key(new)).or_default() += 1;
                result.reordered += 1;
                result.weighted += cost;
                false
            }
            _ => true,
        });
        deleted.retain(|old| match moved.get_mut(&key(old)) {
            Some(count) if *count > 0 => {
                *count -= 1;
                false
            }
            _ => true,
        });
    }
    result.inserted = inserted.len();
    result.deleted = deleted.len();
    result.weighted += INSERT * inserted.len() as u64 + DELETE * deleted.len() as u64;
    result
}

#[cfg(test)]
mod tests {
    use super::*;

    fn rows(texts: &[&str]) -> Vec<String> {
        texts.iter().map(|text| text.to_string()).collect()
    }

    #[test]
    fn identical_code_scores_zero_and_registers_cost_least() {
        let listing = rows(&["push {r5, lr}", "ldrh r3, [r0, r1]", "lsrs r0, r3, #9", "bx r1"]);
        assert_eq!(score(&listing, &listing), Score { prefix: 4, ..Score::default() });
        let renamed = score(&listing, &rows(&["push {r5, lr}", "ldrh r2, [r0, r1]", "lsrs r0, r2, #9", "bx r1"]));
        assert_eq!((renamed.weighted, renamed.regalloc, renamed.differ), (2 * REGALLOC, 2, 4));
        let pool = score(&rows(&["ldr r0, [pc, #4]", "bx lr"]), &rows(&["ldr r0, [pc, #8]", "bx lr"]));
        assert_eq!((pool.weighted, pool.regalloc), (REGALLOC, 1));
    }

    #[test]
    fn reorderings_cost_less_than_insertions_and_deletions() {
        let listing = rows(&["movs r0, #1", "adds r1, #2", "str r0, [r2]", "bx lr"]);
        let moved = score(&listing, &rows(&["adds r1, #2", "movs r0, #1", "str r0, [r2]", "bx lr"]));
        assert_eq!((moved.weighted, moved.reordered, moved.differ), (REORDER, 1, 2));
        let renamed = score(&listing, &rows(&["adds r3, #2", "movs r0, #1", "str r0, [r2]", "bx lr"]));
        assert_eq!((renamed.weighted, renamed.reordered), (REORDER + REGALLOC, 1));
        let extra = score(&listing, &rows(&["movs r0, #1", "adds r1, #2", "mov r8, r0", "str r0, [r2]", "bx lr"]));
        assert_eq!((extra.weighted, extra.inserted, extra.differ), (INSERT, 1, 1));
        let missing = score(&listing, &rows(&["movs r0, #1", "str r0, [r2]", "bx lr"]));
        assert_eq!((missing.weighted, missing.deleted), (DELETE, 1));
        const { assert!(REGALLOC < REORDER && REORDER < INSERT) };
    }

    #[test]
    fn prefix_ranking_puts_a_longer_matched_start_first() {
        let listing = rows(&["push {r5, lr}", "movs r0, #1", "adds r1, #2", "str r0, [r2]", "bx lr"]);
        // Early difference, otherwise close: weighted 10, prefix 1.
        let early = score(&listing, &rows(&["push {r5, lr}", "movs r3, #1", "adds r1, #2", "str r0, [r2]", "bx lr"]));
        // Late difference, costlier: weighted 100, prefix 3.
        let late = score(&listing, &rows(&["push {r5, lr}", "movs r0, #1", "adds r1, #2", "bx lr"]));
        assert_eq!((early.prefix, early.weighted), (1, REGALLOC));
        assert_eq!((late.prefix, late.weighted), (3, DELETE));
        assert!(early.key(false) < late.key(false));
        assert!(late.key(true) < early.key(true));
        // Equal prefixes fall back to the weighted score.
        let other = score(&listing, &rows(&["push {r5, lr}", "movs r3, #1", "adds r1, #2", "bx lr"]));
        assert!(early.key(true) < other.key(true));
        assert_eq!(late.summary_with(true), format!("prefix 3, {}", late.summary()));
        assert_eq!(late.summary_with(false), late.summary());
    }

    #[test]
    fn weighted_zero_is_exactly_differ_zero() {
        let listing = rows(&["movs r0, #1", "bx lr"]);
        for draft in [rows(&["movs r0, #1", "bx lr"]), rows(&["movs r1, #1", "bx lr"]), rows(&["bx lr"])] {
            let result = score(&listing, &draft);
            assert_eq!(result.weighted == 0, result.differ == 0);
        }
    }
}
