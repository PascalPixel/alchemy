//! Candidate filtering, deduplication and saved source diffs.
use std::collections::HashSet;
use std::path::Path;
use super::SourceHash;
use super::score::Score;

#[derive(Clone, Copy, Default)]
pub struct Options {
    pub better_only: bool,
    pub best_only: bool,
    pub threshold: Option<i128>,
    pub no_context: bool,
}

pub struct State {
    base: (u64, usize),
    best: (u64, usize),
    hashes: HashSet<SourceHash>,
    options: Options,
}

impl State {
    pub fn new(base: &Score, hash: SourceHash, options: Options) -> Self {
        let base = base.distance();
        Self { base, best: base, hashes: HashSet::from([hash]), options }
    }

    pub fn accepts(&self, score: &Score, hash: Option<SourceHash>, has_source: bool) -> bool {
        let weighted = score.weighted;
        let score = score.distance();
        score <= self.base && has_source && hash.is_some_and(|hash| !self.hashes.contains(&hash))
            && !(self.options.best_only && score > self.best)
            && (score < self.base || !self.options.better_only)
            && self.options.threshold.is_none_or(|limit| i128::from(weighted) < limit)
    }

    pub fn record(&mut self, score: &Score, hash: SourceHash) {
        let score = score.distance();
        self.best = self.best.min(score);
        if score != (0, 0) { self.hashes.insert(hash); }
    }
}

pub fn trim<'a>(source: &'a str, function: Option<&str>) -> &'a str {
    function.and_then(|name| source.find(name)).and_then(|i| source[..i].rfind('\n')).map_or(source, |i| &source[i..])
}

pub fn write(root: &Path, score: u64, source: &str, base: &str, function: Option<&str>, options: Options) -> Result<(), String> {
    let mut counter = 1u64;
    let directory = loop {
        let directory = root.join(format!("output-{score}-{counter}"));
        match std::fs::create_dir(&directory) {
            Ok(()) => break directory,
            Err(error) if error.kind() == std::io::ErrorKind::AlreadyExists => counter += 1,
            Err(error) => return Err(format!("{}: {error}", directory.display())),
        }
    };
    let shown = if options.no_context { trim(source, function) } else { source };
    let diff = diff(base, source);
    for (name, text) in [("source.c", shown.to_string()), ("score.txt", format!("{score}\n")), ("diff.txt", diff)] {
        let path = directory.join(name);
        std::fs::write(&path, text).map_err(|error| format!("{}: {error}", path.display()))?;
    }
    println!("wrote to {}", directory.display());
    Ok(())
}

/// Compare stripped lines while preserving whitespace in the saved diff.
pub fn diff(base: &str, source: &str) -> String {
    let a: Vec<_> = base.split('\n').collect();
    let b: Vec<_> = source.split('\n').collect();
    let left: Vec<_> = a.iter().map(|s| s.trim()).collect();
    let right: Vec<_> = b.iter().map(|s| s.trim()).collect();
    let mut ops = Vec::new();
    let (mut i, mut j) = (0, 0);
    for (ai, bj, size) in super::score::matching_blocks_autojunk(&left, &right, true) {
        if i < ai || j < bj { ops.push((false, i, ai, j, bj)); }
        if size > 0 { ops.push((true, ai, ai + size, bj, bj + size)); }
        (i, j) = (ai + size, bj + size);
    }
    if let Some(op) = ops.first_mut().filter(|op| op.0) { op.1 = op.2.saturating_sub(3).max(op.1); op.3 = op.4.saturating_sub(3).max(op.3); }
    if let Some(op) = ops.last_mut().filter(|op| op.0) { op.2 = (op.1 + 3).min(op.2); op.4 = (op.3 + 3).min(op.4); }
    let mut groups = Vec::new();
    let mut group = Vec::new();
    for op in ops {
        if op.0 && op.2 - op.1 > 6 {
            group.push((true, op.1, op.1 + 3, op.3, op.3 + 3));
            groups.push(std::mem::take(&mut group));
            group.push((true, op.2 - 3, op.2, op.4 - 3, op.4));
        } else { group.push(op); }
    }
    if group.iter().any(|op| !op.0) { groups.push(group); }
    let range = |start: usize, end: usize| {
        let count = end - start;
        if count == 1 { (start + 1).to_string() }
        else { format!("{},{}", start + usize::from(count > 0), count) }
    };
    let mut lines = Vec::new();
    for group in groups {
        if lines.is_empty() { lines.extend(["--- before".into(), "+++ after".into()]); }
        let first = group[0];
        let last = group[group.len() - 1];
        lines.push(format!("@@ -{} +{} @@", range(first.1, last.2), range(first.3, last.4)));
        for (equal, i, end_i, j, end_j) in group {
            if equal { lines.extend(a[i..end_i].iter().map(|s| format!(" {s}"))); }
            else {
                lines.extend(a[i..end_i].iter().map(|s| format!("-{s}")));
                lines.extend(b[j..end_j].iter().map(|s| format!("+{s}")));
            }
        }
    }
    lines.join("\n") + "\n"
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn output_policy_sequences() {
        let hash = |name: &str| super::super::text_key(name);
        let base = Score { weighted: 100, ..Score::default() };
        let mut policy = State::new(&base, hash("base"), Options::default());
        for row in include_str!("c/fixtures/output-policy.tsv").lines() {
            let c: Vec<_> = row.split('\t').collect();
            if c[3] == "0" {
                policy = State::new(&base, hash("base"), Options { better_only: c[0] == "1", best_only: c[1] == "1", threshold: (c[2] != "none").then(|| c[2].parse().unwrap()), no_context: false });
            }
            let score = Score { weighted: c[4].parse().unwrap(), ..Score::default() };
            let hash = (c[5] != "none").then(|| hash(c[5]));
            let accepted = policy.accepts(&score, hash, c[6] == "1");
            assert_eq!(accepted, c[7] == "1", "{row}");
            if accepted { policy.record(&score, hash.unwrap()); }
            assert_eq!(policy.best.0, c[8].parse::<u64>().unwrap(), "{row}");
        }
    }

    #[test]
    fn equal_weighted_scores_retain_strict_improvements() {
        let hash = |name: &str| super::super::text_key(name);
        let base = Score { differ: 42, ..Score::default() };
        let options = Options { better_only: true, best_only: true, ..Options::default() };
        let mut policy = State::new(&base, hash("base"), options);
        assert!(!policy.accepts(&base, Some(hash("tie")), true));
        let improved = Score { differ: 2, ..Score::default() };
        assert!(policy.accepts(&improved, Some(hash("improved")), true));
        policy.record(&improved, hash("improved"));
        assert!(!policy.accepts(&improved, Some(hash("improved")), true));
        assert!(!policy.accepts(&base, Some(hash("worse")), true));
        let exact = Score::default();
        assert!(policy.accepts(&exact, Some(hash("exact")), true));
        policy.record(&exact, hash("exact"));
        assert!(!policy.accepts(&improved, Some(hash("another")), true));
    }

    #[test]
    fn whitespace_ignoring_source_diffs() {
        for row in include_str!("c/fixtures/output-diffs.tsv").lines() {
            let c: Vec<_> = row.split('\t').map(|s| s.replace("\\n", "\n")).collect();
            assert_eq!(diff(&c[0], &c[1]), c[2], "base {:?}, source {:?}", c[0], c[1]);
        }
    }

    #[test]
    fn candidate_files_keep_existing_outputs_and_trim_source_context() {
        let root = tempfile::tempdir().unwrap();
        let source = "extern int global;\n\nint target(void)\n{\n  return 2;\n}\n";
        let options = Options { no_context: true, ..Options::default() };
        write(root.path(), 0, source, source, Some("target"), options).unwrap();
        write(root.path(), 0, source, source, Some("target"), options).unwrap();
        for number in [1, 2] {
            let path = root.path().join(format!("output-0-{number}"));
            assert_eq!(std::fs::read_to_string(path.join("source.c")).unwrap(), "\nint target(void)\n{\n  return 2;\n}\n");
            assert_eq!(std::fs::read_to_string(path.join("score.txt")).unwrap(), "0\n");
            assert_eq!(std::fs::read_to_string(path.join("diff.txt")).unwrap(), "\n");
        }
        assert_eq!(trim(source, Some("absent")), source);
    }
}
