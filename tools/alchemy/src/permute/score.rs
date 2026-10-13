//! Mnemonic alignment and operand penalties for compiled candidates.
//! Weighted distance is separate from the stricter instruction difference.

use regex::Regex;
use std::collections::BTreeMap;
use std::path::Path;
use std::sync::OnceLock;
mod levenshtein;

#[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]
pub enum Algorithm { #[default] Difflib, Levenshtein }

impl Algorithm {
    pub fn parse(value: &str) -> Result<Self, String> {
        match value {
            "difflib" => Ok(Self::Difflib),
            "levenshtein" => Ok(Self::Levenshtein),
            _ => Err("--algorithm: expected difflib or levenshtein".into()),
        }
    }
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Line {
    pub row: String,
    pub has_symbol: bool,
}

pub fn disassemble(object: &Path, symbol: Option<&str>) -> Result<Vec<Line>, String> {
    let mut args = vec!["arm-none-eabi-objdump".to_string(), "-drz".into(), "--no-show-raw-insn".into()];
    if let Some(symbol) = symbol {
        args.push(format!("--disassemble={symbol}"));
    }
    args.push(object.to_string_lossy().into_owned());
    parse_objdump(&crate::process::run(&args, crate::compiler::routing::root())?)
}

/// Candidate scoring needs both diff's encoded branch view and all of the
/// weighted scorer's data and relocation rows. Reuse the strict disassembly
/// only when objdump did not omit any bytes and its raw column is recognized.
pub fn disassemble_both(object: &Path, symbol: Option<&str>) -> Result<(Vec<String>, Vec<Line>), String> {
    let text = crate::diff::disassemble_text(object, symbol)?;
    let strict = crate::diff::normalize(&text);
    let rows = match without_raw_instructions(&text) {
        Some(text) => parse_objdump(&text)?,
        None => disassemble(object, symbol)?,
    };
    Ok((strict, rows))
}

/// Strip only objdump's raw encoding column. Its `...` marker means -z
/// would supply additional rows, so that case keeps the existing command.
fn without_raw_instructions(text: &str) -> Option<String> {
    let mut stripped = String::with_capacity(text.len());
    for line in text.lines() {
        if line.trim() == "..." {
            return None;
        }
        if let Some((address, row)) = line.split_once(':') {
            if u64::from_str_radix(address.trim(), 16).is_ok() {
                let row = row.trim_start();
                if !row.starts_with("R_ARM_") {
                    let (raw, rest) = row.split_once('\t')?;
                    if raw.trim().is_empty() || !raw.split_whitespace().all(|word| {
                        matches!(word.len(), 2 | 4 | 8) && word.chars().all(|c| c.is_ascii_hexdigit())
                    }) {
                        return None;
                    }
                    stripped.push_str(address);
                    stripped.push_str(":\t");
                    stripped.push_str(rest);
                    stripped.push('\n');
                    continue;
                }
            }
        }
        stripped.push_str(line);
        stripped.push('\n');
    }
    Some(stripped)
}

/// Normalize ARM32 disassembly, retaining relocations and data rows.
fn parse_objdump(text: &str) -> Result<Vec<Line>, String> {
    static COMMENT: OnceLock<Regex> = OnceLock::new();
    let comment = COMMENT.get_or_init(|| Regex::new(r"<.*>|//.*$").unwrap());
    let mut lines: Vec<Line> = Vec::new();
    for line in text.lines() {
        let Some((address, row)) = line.trim_start().split_once(':') else { continue };
        if u64::from_str_radix(address, 16).is_err() {
            continue;
        }
        let row = comment.replace_all(row, "");
        let row = row.trim();
        if row.is_empty() {
            continue;
        }
        if row.starts_with("R_ARM_") {
            // AAELF32's V4BX relocation marks a BX instruction for optional
            // ARMv4 linker rewriting. It does not relocate its register operand.
            if row.split_whitespace().next() == Some("R_ARM_V4BX") {
                continue;
            }
            if !["R_ARM_THM_CALL", "R_ARM_CALL", "R_ARM_ABS32"].iter().any(|kind| row.starts_with(kind)) {
                return Err(format!("unsupported ARM relocation: {row}"));
            }
            let previous = lines.last_mut().ok_or("relocation without an instruction")?;
            let start = [',', '\t', ' ']
                .iter()
                .find_map(|c| previous.row.rfind(*c))
                .ok_or("relocation without an operand")?
                + 1;
            let suffix = previous.row[start..]
                .find('(')
                .map(|i| previous.row[start + i..].to_string())
                .unwrap_or_default();
            previous.row.truncate(start);
            previous.row.push_str(row.split_whitespace().last().ok_or("relocation without symbol")?);
            previous.row.push_str(&suffix);
            previous.has_symbol = true;
        } else {
            let (op, args) = row.split_once(char::is_whitespace).unwrap_or((row, ""));
            lines.push(Line {
                row: format!("{op}\t{}", args.trim().replace('\t', "  ")),
                has_symbol: false,
            });
        }
    }
    Ok(lines)
}

pub const REGALLOC: u64 = 5;
pub const REORDER: u64 = 60;
pub const INSERT: u64 = 100;
pub const DELETE: u64 = 100;

#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub struct Score {
    pub weighted: u64,
    /// `alchemy diff`'s count: listing + draft - 2 * common.
    pub differ: usize,
    pub regalloc: usize,
    pub stack: u64,
    pub branch: usize,
    pub reordered: usize,
    pub inserted: usize,
    pub deleted: usize,
    /// Rows that match from the start before diff's alignment first
    /// diverges.
    pub prefix: usize,
}

impl Score {
    /// Weighted search distance, with strict instruction differences breaking ties.
    pub fn distance(&self) -> (u64, usize) {
        (self.weighted, self.differ)
    }

    /// Ignored operands can leave a nonmatching candidate at weighted zero.
    pub fn is_zero(&self) -> bool {
        self.distance() == (0, 0)
    }

    /// What the search lowers: with `prefix`, a longer matched prefix
    /// first, then the weighted score.
    pub fn key(&self, prefix: bool) -> (std::cmp::Reverse<usize>, u64) {
        (std::cmp::Reverse(if prefix { self.prefix } else { 0 }), self.weighted)
    }

    pub fn rank(&self, prefix: bool) -> ((std::cmp::Reverse<usize>, u64), usize) {
        (self.key(prefix), self.differ)
    }

    /// The summary, with the matched prefix when the search ranks by it.
    pub fn summary_with(&self, prefix: bool) -> String {
        if prefix {
            format!("prefix {}, {}", self.prefix, self.summary())
        } else {
            self.summary()
        }
    }

    pub fn summary(&self) -> String {
        let mut parts = Vec::new();
        for (count, name) in [
            (self.regalloc, "register"),
            (self.stack as usize, "stack"),
            (self.branch, "branch"),
            (self.reordered, "reordered"),
            (self.inserted, "inserted"),
            (self.deleted, "deleted"),
        ] {
            if count > 0 {
                parts.push(format!("{count} {name}"));
            }
        }
        if parts.is_empty() {
            return format!("weighted 0, differ {}", self.differ);
        }
        format!("weighted {}, differ {} ({})", self.weighted, self.differ, parts.join(", "))
    }
}

#[derive(Clone, Copy, Debug)]
pub struct Options {
    pub algorithm: Algorithm,
    pub stack_differences: bool,
    pub ignore_branch_targets: bool,
}

impl Default for Options {
    fn default() -> Self {
        // Branch destinations do not contribute to the default weighted score.
        Self { algorithm: Algorithm::Difflib, stack_differences: false, ignore_branch_targets: true }
    }
}

fn mnemonic(text: &str) -> &str {
    text.split_whitespace().next().unwrap_or("")
}

fn branch(mnemonic: &str) -> bool {
    let mnemonic = mnemonic.strip_suffix(".n").or_else(|| mnemonic.strip_suffix(".w")).unwrap_or(mnemonic);
    ["b", "bl"].iter().any(|prefix| {
        mnemonic.strip_prefix(prefix).is_some_and(|cond| {
            matches!(
                cond,
                "" | "eq" | "ne" | "cs" | "hs" | "cc" | "lo" | "mi" | "pl" | "vs" | "vc" | "hi" | "ls" | "ge" | "lt" | "gt" | "le" | "al"
            )
        })
    })
}

fn stack_pattern() -> &'static Regex {
    static STACK: OnceLock<Regex> = OnceLock::new();
    STACK.get_or_init(|| Regex::new(r"\bsp, #-?(?P<offset>0x[[:xdigit:]]+|\d+)\b").unwrap())
}

fn normalize(text: &str, options: Options) -> String {
    let mut row = text.to_string();
    if !options.stack_differences {
        if matches!(mnemonic(text), "add" | "sub") && text.split(|c: char| !c.is_alphanumeric()).any(|p| p == "sp") {
            static NUMBER: OnceLock<Regex> = OnceLock::new();
            row = NUMBER
                .get_or_init(|| Regex::new(r"\b(?:0x[0-9a-fA-F]+|[0-9]+)\b").unwrap())
                .replace_all(&row, "imm")
                .into_owned();
        }
        row = stack_pattern().replace_all(&row, "addr(sp)").into_owned();
    }
    if options.ignore_branch_targets && branch(mnemonic(text)) {
        if let Some((prefix, _)) = row.rsplit_once(',') {
            row = format!("{prefix},<target>");
        } else {
            row = format!("{}\t<target>", mnemonic(text));
        }
    }
    if !branch(mnemonic(text)) {
        static INTEGER: OnceLock<Regex> = OnceLock::new();
        let integer = INTEGER.get_or_init(|| Regex::new(r"-?[0-9]+").unwrap());
        let mut converted = String::new();
        let mut end = 0;
        for m in integer.find_iter(&row) {
            converted.push_str(&row[end..m.start()]);
            let forbidden = |c: u8| c.is_ascii_alphabetic() || c == b'_';
            if m.len() <= 1
                || m.start().checked_sub(1).is_some_and(|i| forbidden(row.as_bytes()[i]))
                || row.as_bytes().get(m.end()).is_some_and(|c| forbidden(*c))
            {
                converted.push_str(m.as_str());
            } else if let Ok(n) = m.as_str().parse::<i128>() {
                if n < 0 {
                    converted.push_str(&format!("-0x{:x}", n.unsigned_abs()));
                } else {
                    converted.push_str(&format!("0x{n:x}"));
                }
            } else {
                converted.push_str(m.as_str());
            }
            end = m.end();
        }
        converted.push_str(&row[end..]);
        row = converted;
    }
    row
}

/// Match candidate rows against target rows without suppressing frequent rows.
/// Prefer longest contiguous matches, earliest candidate index,
/// then earliest target index: an LCS gives different answers on repeats.
fn matching_blocks(a: &[&str], b: &[&str]) -> Vec<(usize, usize, usize)> {
    matching_blocks_autojunk(a, b, false)
}

pub fn matching_blocks_autojunk(a: &[&str], b: &[&str], autojunk: bool) -> Vec<(usize, usize, usize)> {
    let mut positions: BTreeMap<&str, Vec<usize>> = BTreeMap::new();
    for (j, word) in b.iter().enumerate() {
        positions.entry(word).or_default().push(j);
    }
    if autojunk && b.len() >= 200 { positions.retain(|_, locations| locations.len() <= b.len() / 100 + 1); }
    let mut queue = vec![(0, a.len(), 0, b.len())];
    let mut blocks = Vec::new();
    while let Some((alo, ahi, blo, bhi)) = queue.pop() {
        let (mut best_i, mut best_j, mut size) = (alo, blo, 0);
        let mut previous = BTreeMap::new();
        for (i, word) in a.iter().enumerate().take(ahi).skip(alo) {
            let mut current = BTreeMap::new();
            for &j in positions.get(word).into_iter().flatten() {
                if j < blo {
                    continue;
                }
                if j >= bhi {
                    break;
                }
                let k = 1 + j.checked_sub(1).and_then(|j| previous.get(&j)).copied().unwrap_or(0);
                current.insert(j, k);
                if k > size {
                    (best_i, best_j, size) = (i + 1 - k, j + 1 - k, k);
                }
            }
            previous = current;
        }
        while best_i > alo && best_j > blo && a[best_i - 1] == b[best_j - 1] { best_i -= 1; best_j -= 1; size += 1; }
        while best_i + size < ahi && best_j + size < bhi && a[best_i + size] == b[best_j + size] { size += 1; }
        if size > 0 {
            blocks.push((best_i, best_j, size));
            if alo < best_i && blo < best_j {
                queue.push((alo, best_i, blo, best_j));
            }
            if best_i + size < ahi && best_j + size < bhi {
                queue.push((best_i + size, ahi, best_j + size, bhi));
            }
        }
    }
    blocks.sort_unstable();
    let mut merged: Vec<(usize, usize, usize)> = Vec::new();
    for (i, j, n) in blocks {
        if let Some((pi, pj, pn)) = merged.last_mut().filter(|(pi, pj, pn)| *pi + *pn == i && *pj + *pn == j) {
            let _ = (pi, pj);
            *pn += n;
        } else {
            merged.push((i, j, n));
        }
    }
    merged.push((a.len(), b.len(), 0));
    merged
}

fn operands(text: &str) -> Vec<&str> {
    text.split_once(char::is_whitespace)
        .map_or_else(Vec::new, |(_, args)| args.trim_start().split(',').collect())
}

fn same_line(old: &str, new: &str, old_symbol: bool, new_symbol: bool, options: Options, score: &mut Score) {
    if old == new {
        return;
    }
    let mut ignore_last = false;
    if options.stack_differences {
        if let (Some(old), Some(new)) = (stack_pattern().captures(old), stack_pattern().captures(new)) {
            let int = |s: &str| {
                s.strip_prefix("0x")
                    .map_or_else(|| s.parse::<u64>().unwrap(), |s| u64::from_str_radix(s, 16).unwrap())
            };
            score.stack += int(&old[1]).abs_diff(int(&new[1]));
            ignore_last = true;
        }
    }
    if !options.ignore_branch_targets && !old_symbol && !new_symbol && branch(mnemonic(old)) && old.rsplit(',').next() != new.rsplit(',').next() {
        score.branch += 1;
        ignore_last = true;
    }
    let (mut old, mut new) = (operands(old), operands(new));
    if ignore_last {
        old.pop();
        new.pop();
    } else {
        for fields in [&mut old, &mut new] {
            if let Some(last) = fields.pop() {
                fields.extend(last.split('('));
            }
        }
    }
    score.regalloc += old.iter().zip(&new).filter(|(a, b)| a != b && !(old_symbol && b.contains('.'))).count() + old.len().abs_diff(new.len());
}

#[cfg(test)]
pub fn score(listing: &[String], draft: &[String]) -> Score {
    score_with(listing, draft, Options::default())
}

pub fn score_with(listing: &[String], draft: &[String], options: Options) -> Score {
    let (common, prefix) = crate::diff::compare_counts(listing, draft);
    let mut result = Score {
        differ: listing.len() + draft.len() - 2 * common,
        prefix,
        ..Score::default()
    };
    let lines = |rows: &[String]| {
        rows.iter()
            .map(|row| Line {
                row: row.clone(),
                has_symbol: false,
            })
            .collect::<Vec<_>>()
    };
    weighted(&lines(listing), &lines(draft), options, &mut result);
    result
}

pub fn prepared(rows: &[Line], options: Options) -> Vec<Line> {
    let end = rows.iter().rposition(|s| mnemonic(&s.row) != "nop").map_or(0, |i| i + 1);
    rows[..end].iter().map(|s| Line { row: normalize(&s.row, options), has_symbol: s.has_symbol }).collect()
}

#[cfg(test)]
pub fn assembly_key(rows: &[Line], options: Options) -> [u8; 32] {
    use sha2::{Digest, Sha256};
    Sha256::digest(prepared(rows, options).iter().map(|s| s.row.as_str()).collect::<Vec<_>>().join("\n").as_bytes()).into()
}

/// Deduplication must retain operands the weighted search deliberately ignores.
pub fn strict_assembly_key(rows: &[Line]) -> [u8; 32] {
    use sha2::{Digest, Sha256};
    Sha256::digest(rows.iter().map(|s| s.row.as_str()).collect::<Vec<_>>().join("\n").as_bytes()).into()
}

pub fn weighted(listing: &[Line], draft: &[Line], options: Options, result: &mut Score) {
    let (differ, prefix) = (result.differ, result.prefix);
    *result = Score {
        differ,
        prefix,
        ..Score::default()
    };
    let (old, new) = (prepared(listing, options), prepared(draft, options));
    let (a, b) = (new.iter().map(|s| mnemonic(&s.row)).collect::<Vec<_>>(), old.iter().map(|s| mnemonic(&s.row)).collect::<Vec<_>>());
    let blocks = match options.algorithm {
        Algorithm::Difflib => matching_blocks(&a, &b),
        Algorithm::Levenshtein => levenshtein::matching_blocks(&a, &b),
    };
    let mut unmatched: BTreeMap<&str, (usize, usize)> = BTreeMap::new();
    let (mut a, mut b) = (0, 0);
    for (i, j, n) in blocks {
        for line in &new[a..i] {
            unmatched.entry(&line.row).or_default().0 += 1;
        }
        for line in &old[b..j] {
            unmatched.entry(&line.row).or_default().1 += 1;
        }
        for k in 0..n {
            same_line(&old[j + k].row, &new[i + k].row, old[j + k].has_symbol, new[i + k].has_symbol, options, result);
        }
        (a, b) = (i + n, j + n);
    }
    for (insertions, deletions) in unmatched.values() {
        let moved = (*insertions).min(*deletions);
        result.reordered += moved;
        result.inserted += insertions - moved;
        result.deleted += deletions - moved;
    }
    result.weighted = result.stack
        + result.branch as u64
        + result.regalloc as u64 * REGALLOC
        + result.reordered as u64 * REORDER
        + result.inserted as u64 * INSERT
        + result.deleted as u64 * DELETE;
}

#[cfg(test)]
mod tests {
    #[test]
    fn raw_column_reuse_preserves_relocations_and_refuses_omissions() {
        let raw = "header\n  0:\tf7ff fffe \tbl\t0 <Call>\n  0: R_ARM_THM_CALL Call\n  4:\t00000000 \t.word\t0x00000000\n  4: R_ARM_ABS32 Data\n  8:\t4770     \tbx\tlr\n  8: R_ARM_V4BX *ABS*\n";
        let plain = "header\n  0:\tbl\t0 <Call>\n  0: R_ARM_THM_CALL Call\n  4:\t.word\t0x00000000\n  4: R_ARM_ABS32 Data\n  8:\tbx\tlr\n  8: R_ARM_V4BX *ABS*\n";
        let reused = super::parse_objdump(&super::without_raw_instructions(raw).unwrap()).unwrap();
        let expected = super::parse_objdump(plain).unwrap();
        assert_eq!(reused, expected);
        assert_eq!(super::strict_assembly_key(&reused), super::strict_assembly_key(&expected));
        for ambiguous in ["\t...\n", "  0:\tunknown raw\tmovs\tr0, #1\n", "  0:\t123\tmovs\tr0, #1\n"] {
            assert!(super::without_raw_instructions(ambiguous).is_none(), "{ambiguous}");
        }
    }

    #[test]
    fn shared_disassembly_matches_legacy_thumb_arm_pools_and_zeros() {
        crate::compiler::routing::prefer_installed_binutils();
        let work = tempfile::tempdir().unwrap();
        for (arm, zeros) in [(false, false), (false, true), (true, false), (true, true)] {
            let source = work.path().join(format!("probe-{arm}-{zeros}.s"));
            let object = source.with_extension("o");
            let mode = if arm { ".arm\n.type Target, %function\n" } else { ".thumb\n.thumb_func\n" };
            let body = if arm {
                "push {lr}\nldr r0, .Lpool\ncmp r0, #0\nbeq .Lend\nbl Called\n.Lend:\npop {lr}\nbx lr\n"
            } else {
                "push {lr}\nldr r0, .Lpool\ncmp r0, #0\nbeq .Lend\nbl Called\n.Lend:\npop {r0}\nbx r0\n"
            };
            let padding = if zeros { ".space 16\n" } else { "" };
            std::fs::write(&source, format!(".syntax unified\n.text\n.global Target\n{mode}Target:\n{body}{padding}.align 2\n.Lpool:\n.word Data\n.word 0x12345678\n.size Target, .-Target\n")).unwrap();
            crate::process::run(&["arm-none-eabi-as", "-mcpu=arm7tdmi", "-o", &object.to_string_lossy(), &source.to_string_lossy()], crate::compiler::routing::root()).unwrap();
            for symbol in [None, Some("Target")] {
                let text = crate::diff::disassemble_text(&object, symbol).unwrap();
                assert_eq!(super::without_raw_instructions(&text).is_none(), zeros, "arm={arm}, symbol={symbol:?}: {text}");
                let legacy = (crate::diff::disassemble(&object, symbol).unwrap(), super::disassemble(&object, symbol).unwrap());
                let reused = super::disassemble_both(&object, symbol).unwrap();
                assert_eq!(reused, legacy, "arm={arm}, zeros={zeros}, symbol={symbol:?}");
                assert_eq!(super::strict_assembly_key(&reused.1), super::strict_assembly_key(&legacy.1));
                assert!(reused.1.iter().any(|line| line.has_symbol && line.row.ends_with("Data")));
            }
        }
    }

    #[test]
    fn archive_identity_retains_operands_ignored_by_weighting() {
        let rows = |row: &str| vec![super::Line { row: row.into(), has_symbol: false }];
        for (old, new) in [("ldr\tr0, [sp, #0]", "ldr\tr0, [sp, #4]"), ("b\t10", "b\t12")] {
            let (old, new) = (rows(old), rows(new));
            assert_eq!(super::assembly_key(&old, super::Options::default()), super::assembly_key(&new, super::Options::default()));
            assert_ne!(super::strict_assembly_key(&old), super::strict_assembly_key(&new));
        }
        let old = rows("bx\tlr");
        let mut new = old.clone();
        new.extend(rows("nop"));
        assert_eq!(super::assembly_key(&old, super::Options::default()), super::assembly_key(&new, super::Options::default()));
        assert_ne!(super::strict_assembly_key(&old), super::strict_assembly_key(&new));
        assert!(!super::Score { differ: 42, ..super::Score::default() }.is_zero());
        assert!(super::Score::default().is_zero());
    }

    #[test]
    fn assembly_identity_rows_and_hashes() {
        use sha2::{Digest, Sha256};
        for row in include_str!("c/fixtures/output-assembly.tsv").lines() {
            let c: Vec<_> = row.split('\t').collect();
            let options = super::Options { stack_differences: c[0] == "1", ignore_branch_targets: c[1] == "1", ..super::Options::default() };
            let raw = c[2].replace("\\n", "\n").replace("\\t", "\t");
            let expected = c[3].replace("\\n", "\n").replace("\\t", "\t");
            let lines = super::parse_objdump(&raw).unwrap();
            assert_eq!(super::prepared(&lines, options).iter().map(|line| line.row.as_str()).collect::<Vec<_>>().join("\n"), expected);
            let expected_hash: [u8; 32] = Sha256::digest(expected.as_bytes()).into();
            assert_eq!(super::assembly_key(&lines, options), expected_hash);
        }
    }
    use super::*;

    #[test]
    fn both_algorithms_match_scores_and_normalization_options() {
        for row in include_str!("c/fixtures/scoring-algorithms.tsv").lines() {
            let c: Vec<_> = row.split('\t').collect();
            let options = Options { algorithm: Algorithm::parse(c[0]).unwrap(), stack_differences: c[1] == "1", ignore_branch_targets: c[2] == "1" };
            let decode = |s: &str| s.replace("\\t", "\t").replace("\\n", "\n");
            let (old, new) = (parse_objdump(&decode(c[5])).unwrap(), parse_objdump(&decode(c[6])).unwrap());
            let mut actual = Score::default();
            weighted(&old, &new, options, &mut actual);
            assert_eq!(actual.weighted, c[4].parse::<u64>().unwrap(), "{} seed {} stack={} ignore={} {:?}", c[0], c[3], c[1], c[2], actual);
        }
    }

    fn rows(texts: &[&str]) -> Vec<String> {
        texts.iter().map(|text| text.to_string()).collect()
    }

    #[test]
    fn count_only_strict_scoring_preserves_all_score_fields_and_options() {
        let listing = rows(&["push {lr}", "ldr r0, [sp, #12]", "mov r0, r1", "mov r0, r1", "b.n +4", "bx lr", "nop"]);
        let drafts = [
            Vec::new(),
            listing.clone(),
            rows(&["push {lr}", "ldr r0, [sp, #20]", "mov r0, r1", "mov r0, r1", "b.n +8", "bx lr"]),
            rows(&["mov r0, r1", "push {lr}", "ldr r0, [sp, #12]", "mov r0, r1", "b.n +4", "bx lr", "nop"]),
            rows(&["push {lr}", "ldr r0, [sp, #12]", "mov r0, r1", "bl", "b.n +4", "bx lr", "nop"]),
            rows(&["mov r0, r1", "mov r0, r1"]),
        ];
        let lines = |rows: &[String]| rows.iter().map(|row| Line { row: row.clone(), has_symbol: false }).collect::<Vec<_>>();
        for draft in &drafts {
            for (listing, draft) in [(&listing, draft), (draft, &listing)] {
                let (common, script) = crate::diff::compare(listing, draft);
                let prefix = script.iter().take_while(|line| line.starts_with("  ")).count();
                for algorithm in [Algorithm::Difflib, Algorithm::Levenshtein] {
                    for stack_differences in [false, true] {
                        for ignore_branch_targets in [false, true] {
                            let options = Options { algorithm, stack_differences, ignore_branch_targets };
                            let mut expected = Score { differ: listing.len() + draft.len() - 2 * common, prefix, ..Score::default() };
                            weighted(&lines(listing), &lines(draft), options, &mut expected);
                            assert_eq!(score_with(listing, draft, options), expected, "{options:?} {listing:?} / {draft:?}");
                        }
                    }
                }
            }
        }
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
    fn unchanged_rows_keep_both_measurements_zero() {
        let listing = rows(&["movs r0, #1", "bx lr"]);
        for draft in [rows(&["movs r0, #1", "bx lr"]), rows(&["movs r1, #1", "bx lr"]), rows(&["bx lr"])] {
            let result = score(&listing, &draft);
            assert_eq!(result.weighted == 0, result.differ == 0);
        }
    }

    #[test]
    fn penalties_count_operands_and_immediates_separately() {
        let result = score(&rows(&["adds r0, r1, #3"]), &rows(&["adds r2, r3, #4"]));
        assert_eq!((result.regalloc, result.weighted), (3, 15));
        let result = score(&rows(&["mov r0, r1", "add r2, r3"]), &rows(&["add r2, r3", "mov r4, r1"]));
        assert_eq!((result.weighted, result.inserted, result.deleted, result.reordered), (200, 1, 1, 0));
    }

    #[test]
    fn ignores_stack_by_default_and_charges_branch_targets_one() {
        let old = rows(&["ldr r0, [sp, #12]", "b.n +4"]);
        let new = rows(&["ldr r0, [sp, #20]", "b.n +8"]);
        let result = score(&old, &new);
        assert_eq!((result.weighted, result.branch, result.stack), (0, 0, 0));
        let result = score_with(&old, &new, Options { stack_differences: false, ignore_branch_targets: false, ..Options::default() });
        assert_eq!((result.weighted, result.branch, result.stack), (1, 1, 0));
        let result = score_with(
            &old,
            &new,
            Options {
                stack_differences: true,
                ignore_branch_targets: false,
                ..Options::default()
            },
        );
        assert_eq!((result.weighted, result.branch, result.stack), (9, 1, 8));
        let result = score_with(
            &old,
            &new,
            Options {
                stack_differences: false,
                ignore_branch_targets: true,
                ..Options::default()
            },
        );
        assert_eq!(result.weighted, 0);
        assert_ne!(result.differ, 0, "weighted zero must not be presented as byte identity");
    }

    #[test]
    fn only_discards_trailing_nops() {
        assert_eq!(score(&rows(&["mov r0, r1", "nop"]), &rows(&["mov r0, r1"])).weighted, 0);
        assert_eq!(score(&rows(&["nop", "mov r0, r1"]), &rows(&["mov r0, r1"])).weighted, DELETE);
    }

    #[test]
    fn objdump_keeps_calls_relocations_literals_and_symbol_wildcards() {
        let target = parse_objdump("header\n  0:\tbl\t4 <f>\n  0: R_ARM_THM_CALL Call\n  4:\t.word\t0x00000000\n  4: R_ARM_ABS32 Data\n  8:\tnop\t\n").unwrap();
        assert_eq!(target[0].row, "bl\tCall");
        assert_eq!(target[1].row, ".word\tData");
        assert!(target[0].has_symbol && target[1].has_symbol);
        let candidate = parse_objdump("header\n  0:\tbl\t4 <f>\n  0: R_ARM_THM_CALL .text+0x34\n  4:\t.word\t0x00000000\n  4: R_ARM_ABS32 Data\n").unwrap();
        let mut score = Score::default();
        let options = Options { stack_differences: false, ignore_branch_targets: false, ..Options::default() };
        weighted(&target, &candidate, options, &mut score);
        assert_eq!(score.weighted, 0);
        weighted(&candidate, &target, options, &mut score);
        assert_eq!(score.weighted, REGALLOC, "symbol wildcard is directional, in this scoring direction");
        let wrong_call = parse_objdump("  0:\tbl\t4 <f>\n  0: R_ARM_THM_CALL Other\n  4:\t.word\t0x00000000\n  4: R_ARM_ABS32 Data\n").unwrap();
        weighted(&target, &wrong_call, options, &mut score);
        assert_eq!(score.weighted, REGALLOC, "test options retain relocated callees");
        weighted(&target, &wrong_call, Options::default(), &mut score);
        assert_eq!(score.weighted, 0, "the CLI default ignores call targets as well as branch destinations");
    }

    #[test]
    fn objdump_v4bx_keeps_the_branch_register() {
        let target = parse_objdump("  0:\tbx\tlr\n  0: R_ARM_V4BX\t*ABS*\n").unwrap();
        assert_eq!(target.len(), 1);
        assert_eq!(target[0].row, "bx\tlr");
        assert!(!target[0].has_symbol);
        let candidate = parse_objdump("  0:\tbx\tr3\n  0: R_ARM_V4BX\t*ABS*\n").unwrap();
        let mut score = Score::default();
        weighted(&target, &candidate, Options::default(), &mut score);
        assert_eq!(score.weighted, REGALLOC);
        assert!(parse_objdump("  0:\tbx\tlr\n  0: R_ARM_UNKNOWN\t*ABS*\n").is_err());
    }

    #[test]
    fn mnemonic_alignment_prefers_the_earliest_candidate_tie() {
        assert_eq!(matching_blocks(&["a", "b", "a"], &["b", "a", "b"]), [(0, 1, 2), (3, 3, 0)]);
        assert_eq!(matching_blocks(&[], &["a"]), [(0, 1, 0)]);
        assert_eq!(matching_blocks(&["a", "b", "c"], &["a", "x", "c"]), [(0, 0, 1), (2, 2, 1), (3, 3, 0)]);
    }
}
