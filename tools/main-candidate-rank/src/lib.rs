//! `main-rank`: rank every main-image candidate, and say which to open first.
//!
//! The overlay half has `overlay candidate-rank`, which as `bce2bc1e9` puts it
//! is "the tool that says which owners are worth opening". The main image --
//! 548,364 executable bytes, the larger half of the target -- had no such tool,
//! so its drafts' distance from the reference was unmeasured in bulk and owners
//! were chosen by size from the target table instead.
//!
//! This measures all of them, and adds the one judgement the overlay ranker does
//! not make: whether a residual is a plain reorder, which nothing in the source
//! reaches, or something else.
//!
//! ## Why a distance alone is not enough
//!
//! Two owners can both sit two halfwords from the reference and be nothing
//! alike. If the two sides emit the same instructions in a different ORDER,
//! they already agree on what to emit and disagree only on when, which past
//! register allocation is the scheduler's business and no source spelling
//! reaches. If they emit DIFFERENT instructions, the disagreement is about a
//! type, a prototype, a constant or an evaluation order the source controls,
//! reading it MAY be ordinary work -- or may also turn out to be allocation,
//! which is why the bucket is called `divergent` rather than anything promising.
//!
//! Measured over the near-miss band of both halves, that split is 140 owners
//! and 18,568 bytes of reordering against 104 owners and 11,758 bytes of
//! divergence, so most of the band is the kind no source reading moves and a
//! ranker reporting distance alone would send a contributor straight at it.
//! Eight owners were opened by hand while building this: on every one flagged
//! reordering, and on the one `divergent` owner that turned out to be allocation
//! too, each behaviour-preserving reshape (statement order, prototype spelling,
//! mask and decrement forms, block versus function scope for a hoisted
//! invariant, inlining a single-use temporary), `shape-sweep`, and 600
//! iterations of `permute` left the residual exactly where it started.
//!
//! ## Reading the classification honestly
//!
//! `reordering` is a statement about the two instruction MULTISETS being equal,
//! nothing more. It is evidence that source shape is unlikely to reach the
//! residual, not proof that the owner is unmatchable: a different allocation
//! could still follow from a change this tool cannot see. Treat it as "open
//! these last", not "these are impossible". And `divergent` is only its
//! negation, so it carries no promise either -- it is the smaller pile to search,
//! not a list of fixable owners.
//!
//! Two artefacts have to be normalised away first, and both were caught by
//! disagreeing with owners already read by hand:
//!
//!   * moving a pc-relative load changes that load's own displacement, so
//!     `[pc, #524]` and `[pc, #516]` are one load at two positions;
//!   * the candidate column is a FIXED-WIDTH field -- `pad_end(text, 30)` clipped
//!     to 30 -- so the split is by offset. A run-of-spaces rule looks right and
//!     fails on exactly the rows that have no padding left, which are the wide
//!     ones; a column over 30 characters is also clipped and leaks its tail onto
//!     the reference side, so that side is anchored on its mnemonic.
//!
//! Without the first, 15 reorderings read as disagreements. The second bit twice:
//! an early draft split three characters off and inverted the verdict outright,
//! reporting 155 owners worth opening where there are 104, and the run-of-spaces
//! rule that replaced it read 13 overlay reorderings as divergent.

use std::collections::BTreeMap;
use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::mpsc;
use std::thread;

/// One measured candidate.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Row {
    pub address: String,
    pub source: String,
    pub candidate_bytes: Option<i64>,
    pub reference_bytes: Option<i64>,
    pub differing_halfwords: Option<i64>,
    /// Rows the aligned view marks `+`, `-` or `!`.
    pub differing_rows: Option<i64>,
    pub verdict: Verdict,
    pub error: Option<String>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub enum Verdict {
    /// Byte-identical at the right size.
    Exact,
    /// The two sides emit different instructions. That can be a type, a
    /// prototype, a constant or an evaluation order the source decides -- and
    /// it can also be a register the allocator simply chose differently, which
    /// source shape does not reach either. `0800fec8` is the second kind: it
    /// shifts through r3 where the reference shifts r1 in place, and inlining
    /// the load, splitting the temporary and using compound shifts all made it
    /// worse. So this bucket means "not provably a reorder", not "fixable".
    Divergent,
    /// The same instructions naming different registers. `080a524c` builds its
    /// flag in r3 where the reference uses r2 and is otherwise identical;
    /// `08004144` moves a pair through r1 where the reference uses r5. Which
    /// register holds a value is the allocator's decision, so like a reorder
    /// this is not something a source spelling names.
    Allocation,
    /// The same instructions in a different order: the scheduler's choice.
    Reordering,
    /// Sizes differ, so there is no meaningful positional comparison.
    SizeMismatch,
    /// The candidate did not compile, or produced no score.
    Unscored,
}

impl Verdict {
    pub const fn label(self) -> &'static str {
        match self {
            Verdict::Exact => "exact",
            Verdict::Divergent => "divergent",
            Verdict::Allocation => "allocation",
            Verdict::Reordering => "reordering",
            Verdict::SizeMismatch => "size-mismatch",
            Verdict::Unscored => "unscored",
        }
    }

    /// Whether this owner is worth opening before the reordering ones. It is
    /// not a promise that a source reading exists.
    pub const fn worth_opening(self) -> bool {
        matches!(self, Verdict::Divergent | Verdict::SizeMismatch)
    }
}

/// Strip everything that MOVING an instruction alone would change.
///
/// A relocated pc-relative load carries a different displacement while being
/// the same load, and branch and pool addresses shift with any size change, so
/// none of the three can distinguish a reorder from a real disagreement.
pub fn normalise(text: &str) -> String {
    let body = text.split('@').next().unwrap_or("").trim();
    let mut out = String::with_capacity(body.len());
    let bytes: Vec<char> = body.chars().collect();
    let mut index = 0usize;
    while index < bytes.len() {
        // `[pc, #NNN]` -> `[pc]`
        if bytes[index..].starts_with(&['[', 'p', 'c']) {
            if let Some(close) = bytes[index..].iter().position(|c| *c == ']') {
                out.push_str("[pc]");
                index += close + 1;
                continue;
            }
        }
        // `0x...` -> `ADDR`
        if bytes[index] == '0' && bytes.get(index + 1) == Some(&'x') {
            out.push_str("ADDR");
            index += 2;
            while index < bytes.len() && bytes[index].is_ascii_hexdigit() {
                index += 1;
            }
            continue;
        }
        if bytes[index].is_whitespace() {
            if !out.ends_with(' ') {
                out.push(' ');
            }
            index += 1;
            continue;
        }
        out.push(bytes[index]);
        index += 1;
    }
    out.trim().to_string()
}

/// The same instruction with every register name blanked to `R`.
///
/// Two sides that agree here and disagree on `normalise` differ only in which
/// registers they name, which the allocator picks. It keeps mnemonics,
/// immediates and addressing shape, so a genuine difference still shows: `strb`
/// against `strh` survives, and so does `#31` against `#30`.
pub fn register_blind(text: &str) -> String {
    let normalised = normalise(text);
    let mut out = String::with_capacity(normalised.len());
    let chars: Vec<char> = normalised.chars().collect();
    let mut index = 0usize;
    while index < chars.len() {
        let previous_is_word = index > 0 && (chars[index - 1].is_alphanumeric() || chars[index - 1] == '_');
        let (matched, width) = register_at(&chars[index..]);
        if matched && !previous_is_word {
            let after = chars.get(index + width);
            // A register name ends here only if what follows is not more of a word.
            if !after.is_some_and(|c| c.is_alphanumeric() || *c == '_') {
                out.push('R');
                index += width;
                continue;
            }
        }
        out.push(chars[index]);
        index += 1;
    }
    out
}

/// Whether a register name starts here, and how many characters it spans.
fn register_at(rest: &[char]) -> (bool, usize) {
    let word: String = rest
        .iter()
        .take_while(|c| c.is_alphanumeric())
        .collect::<String>()
        .to_ascii_lowercase();
    if word.len() >= 2 && word.starts_with('r') && word[1..].chars().all(|c| c.is_ascii_digit()) {
        if let Ok(number) = word[1..].parse::<u32>() {
            if number <= 15 {
                return (true, word.len());
            }
        }
    }
    // `pc` is deliberately absent: `[pc]` is an addressing mode this tool
    // already collapses, and blanking it would hide a real pool-versus-register
    // difference.
    if matches!(word.as_str(), "sp" | "lr" | "fp" | "ip" | "sl") {
        return (true, word.len());
    }
    (false, 0)
}

/// Split an aligned `!` row into its candidate and reference halves.
///
/// The candidate column is `pad_end(text, 30)` clipped to exactly 30 units (see
/// `candidate_show::render`), so the row is `"  ! "` then 30 characters then the
/// reference. It is a FIXED-WIDTH field, and the run-of-spaces rule an earlier
/// draft used only works while the content is short enough to leave padding.
/// This row has none:
///
/// ```text
///   ! ldr	r2, [pc, #20]	@ (0x2000e80) ldr	r2, [pc, #16]	@ (0x2000e80)
/// ```
///
/// One space, because the left text is 31 characters and was clipped -- which
/// also leaks its tail, here a `)`, onto the front of the reference column. So
/// the split is by offset and the reference side is then anchored on its
/// mnemonic, discarding whatever the clip left behind. Without that anchoring a
/// clipped row compares a mnemonic against `) ldr` and reads as a disagreement.
pub const CANDIDATE_COLUMN: usize = 30;

pub fn split_columns(body: &str) -> (String, Option<String>) {
    // `body` is the row past its three-character mark, so the column starts one
    // space in.
    let chars: Vec<char> = body.trim_end().chars().collect();
    if chars.len() <= 1 {
        return (String::new(), None);
    }
    let start = 1usize;
    let end = (start + CANDIDATE_COLUMN).min(chars.len());
    let left: String = chars[start..end].iter().collect();
    if end >= chars.len() {
        return (left.trim().to_string(), None);
    }
    let right: String = chars[end..].iter().collect();
    let right = anchor_mnemonic(&right);
    if right.is_empty() {
        return (left.trim().to_string(), None);
    }
    (left.trim().to_string(), Some(right))
}

/// Drop anything before the first mnemonic, which is what a clipped candidate
/// column leaves on the front of the reference column.
fn anchor_mnemonic(text: &str) -> String {
    let trimmed = text.trim_start();
    let chars: Vec<char> = trimmed.chars().collect();
    for (index, ch) in chars.iter().enumerate() {
        if !ch.is_ascii_alphabetic() {
            continue;
        }
        // A mnemonic starts a word and is followed by a separator or its operands.
        let starts_word = index == 0 || !chars[index - 1].is_ascii_alphanumeric();
        if starts_word {
            return chars[index..].iter().collect::<String>().trim().to_string();
        }
    }
    trimmed.trim().to_string()
}

/// Classify one aligned `candidate-show` output.
pub fn classify(output: &str) -> (Verdict, i64) {
    let mut left: BTreeMap<String, i64> = BTreeMap::new();
    let mut right: BTreeMap<String, i64> = BTreeMap::new();
    let mut blind_left: BTreeMap<String, i64> = BTreeMap::new();
    let mut blind_right: BTreeMap<String, i64> = BTreeMap::new();
    let mut differing = 0i64;
    let mut record = |side: &mut BTreeMap<String, i64>,
                      blind: &mut BTreeMap<String, i64>,
                      text: &str| {
        *side.entry(normalise(text)).or_default() += 1;
        *blind.entry(register_blind(text)).or_default() += 1;
    };
    for line in output.lines().skip(2) {
        if line.len() < 3 {
            continue;
        }
        let mark = line[..3].trim();
        let body = &line[3..];
        match mark {
            "+" => {
                record(&mut left, &mut blind_left, body);
                differing += 1;
            }
            "-" => {
                record(&mut right, &mut blind_right, body);
                differing += 1;
            }
            "!" => {
                let (a, b) = split_columns(body);
                record(&mut left, &mut blind_left, &a);
                if let Some(b) = b {
                    record(&mut right, &mut blind_right, &b);
                }
                differing += 1;
            }
            _ => {}
        }
    }
    if differing == 0 {
        return (Verdict::Exact, 0);
    }
    // Order matters: a reorder is the narrowest claim, so test it first, then
    // the register-blind comparison, and only call it divergent when the two
    // sides disagree about something neither the scheduler nor the allocator
    // decides.
    if left == right {
        return (Verdict::Reordering, differing);
    }
    if blind_left == blind_right {
        return (Verdict::Allocation, differing);
    }
    (Verdict::Divergent, differing)
}

fn parse_score(output: &str) -> Option<(i64, i64, i64)> {
    let at = output.find("candidate=")?;
    let rest = &output[at + "candidate=".len()..];
    let mut numbers = Vec::new();
    for key in ["", "reference=", "differing_halfwords="] {
        let slice = if key.is_empty() {
            rest
        } else {
            let found = rest.find(key)?;
            &rest[found + key.len()..]
        };
        let digits: String = slice.chars().take_while(char::is_ascii_digit).collect();
        numbers.push(digits.parse::<i64>().ok()?);
    }
    Some((numbers[0], numbers[1], numbers[2]))
}

/// Main-image candidate sources: `semantic/08xxxxxx.c` and nothing else.
pub fn candidates(root: &Path) -> Vec<PathBuf> {
    let mut found = Vec::new();
    let Ok(entries) = fs::read_dir(root.join("semantic")) else {
        return found;
    };
    for entry in entries.flatten() {
        let path = entry.path();
        if path.extension().and_then(|e| e.to_str()) != Some("c") {
            continue;
        }
        let Some(stem) = path.file_stem().and_then(|s| s.to_str()) else {
            continue;
        };
        if stem.len() == 8 && stem.starts_with('0') && stem.chars().all(|c| c.is_ascii_hexdigit()) {
            found.push(path);
        }
    }
    found.sort();
    found
}

/// Measure one candidate.
///
/// `work` MUST be private to this call. `candidate-show` compiles, links and
/// disassembles through a work directory that defaults to a single shared path,
/// so concurrent invocations overwrite each other's intermediates: the first
/// draft of this ranker shared it and got `objdump failed` on owners that score
/// perfectly well alone, plus a row count of 183 out of a 108-line comparison.
/// Both symptoms were one collision, and both would have been read as facts
/// about the owner rather than about the tool.
fn measure(
    root: &Path,
    self_exe: &Path,
    subcommand: &[&str],
    source: &Path,
    work: &Path,
) -> Row {
    let relative = source
        .strip_prefix(root)
        .unwrap_or(source)
        .to_string_lossy()
        .to_string();
    let address = source
        .file_stem()
        .and_then(|s| s.to_str())
        .unwrap_or("")
        .to_string();
    let _ = fs::create_dir_all(work);
    let run = Command::new(self_exe)
        .args(subcommand)
        .arg(source)
        .arg("--work")
        .arg(work)
        .arg("--align")
        .current_dir(root)
        .output();
    let output = match run {
        Ok(done) => done,
        Err(error) => {
            return Row {
                address,
                source: relative,
                candidate_bytes: None,
                reference_bytes: None,
                differing_halfwords: None,
                differing_rows: None,
                verdict: Verdict::Unscored,
                error: Some(error.to_string()),
            }
        }
    };
    let text = String::from_utf8_lossy(&output.stdout).to_string();
    let Some((candidate, reference, halfwords)) = parse_score(&text) else {
        let stderr = String::from_utf8_lossy(&output.stderr);
        let first = stderr
            .lines()
            .chain(text.lines())
            .find(|line| !line.trim().is_empty())
            .unwrap_or("no score line")
            .chars()
            .take(160)
            .collect::<String>();
        return Row {
            address,
            source: relative,
            candidate_bytes: None,
            reference_bytes: None,
            differing_halfwords: None,
            differing_rows: None,
            verdict: Verdict::Unscored,
            error: Some(first),
        };
    };
    let (verdict, rows) = if candidate != reference {
        // A size difference shifts every later halfword, so neither the count
        // nor a positional comparison means anything here; the aligned row
        // count is the only usable distance.
        let (_, rows) = classify(&text);
        (Verdict::SizeMismatch, rows)
    } else {
        classify(&text)
    };
    Row {
        address,
        source: relative,
        candidate_bytes: Some(candidate),
        reference_bytes: Some(reference),
        differing_halfwords: Some(halfwords),
        differing_rows: Some(rows),
        verdict,
        error: None,
    }
}

/// Rank order: reachable owners first, closest first, largest first.
pub fn compare(left: &Row, right: &Row) -> std::cmp::Ordering {
    let key = |row: &Row| {
        (
            row.verdict,
            row.differing_rows.unwrap_or(i64::MAX),
            -row.reference_bytes.unwrap_or(0),
        )
    };
    key(left).cmp(&key(right))
}

fn escape(text: &str) -> String {
    let mut out = String::with_capacity(text.len());
    for ch in text.chars() {
        match ch {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            c if (c as u32) < 0x20 => out.push_str(&format!("\\u{:04x}", c as u32)),
            c => out.push(c),
        }
    }
    out
}

fn render_report(rows: &[Row]) -> String {
    let mut out = String::from("{\n  \"measured\": [\n");
    for (index, row) in rows.iter().enumerate() {
        out.push_str("    {\n");
        out.push_str(&format!("      \"address\": \"{}\",\n", escape(&row.address)));
        out.push_str(&format!("      \"source\": \"{}\",\n", escape(&row.source)));
        if let Some(value) = row.candidate_bytes {
            out.push_str(&format!("      \"candidateBytes\": {value},\n"));
        }
        if let Some(value) = row.reference_bytes {
            out.push_str(&format!("      \"referenceBytes\": {value},\n"));
        }
        if let Some(value) = row.differing_halfwords {
            out.push_str(&format!("      \"differingHalfwords\": {value},\n"));
        }
        if let Some(value) = row.differing_rows {
            out.push_str(&format!("      \"differingRows\": {value},\n"));
        }
        if let Some(error) = &row.error {
            out.push_str(&format!("      \"error\": \"{}\",\n", escape(error)));
        }
        out.push_str(&format!("      \"verdict\": \"{}\"\n", row.verdict.label()));
        out.push_str(if index + 1 == rows.len() {
            "    }\n"
        } else {
            "    },\n"
        });
    }
    out.push_str("  ]\n}\n");
    out
}

const USAGE: &str = "usage: main-rank [--jobs N] [--top N] [--max N] [--worth-opening] [--json]\n       main-rank --self-test";

fn value_after<'a>(args: &'a [String], flag: &str) -> Option<&'a str> {
    args.iter()
        .position(|arg| arg == flag)
        .and_then(|index| args.get(index + 1))
        .map(String::as_str)
}

/// `main()`. `subcommand` is the argv this command's SIBLING `candidate-show`
/// is reached by inside the host binary; children spawned without it land on
/// the host's dispatcher, which is how `candidate-rank`'s fan-out was broken
/// for as long as it was (`bce2bc1e9`).
pub fn run(root: &Path, self_exe: &Path, subcommand: &[&str], args: &[String]) -> Result<(), String> {
    if args.iter().any(|arg| arg == "-h" || arg == "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    if args.iter().any(|arg| arg == "--self-test") {
        return self_test();
    }
    let jobs = value_after(args, "--jobs")
        .and_then(|text| text.parse::<usize>().ok())
        .unwrap_or_else(|| {
            thread::available_parallelism()
                .map(|n| n.get().saturating_sub(2).max(1))
                .unwrap_or(1)
        })
        .clamp(1, 64);
    let top = value_after(args, "--top")
        .and_then(|text| text.parse::<usize>().ok())
        .unwrap_or(30);
    let limit = value_after(args, "--max")
        .and_then(|text| text.parse::<usize>().ok())
        .unwrap_or(usize::MAX);
    let only_worth = args.iter().any(|arg| arg == "--worth-opening");

    let mut sources = candidates(root);
    if sources.is_empty() {
        return Err("no main-image candidates under semantic/".to_string());
    }
    sources.truncate(limit);

    // One directory per candidate, so no two concurrent `candidate-show` runs
    // share an intermediate. Keyed by address, so a re-run reuses the same paths
    // rather than accumulating them.
    let work_root = root.join("out/main-candidate-rank/work");
    let work_root = work_root.as_path();

    let (sender, receiver) = mpsc::channel();
    let chunks: Vec<Vec<PathBuf>> = {
        let mut buckets: Vec<Vec<PathBuf>> = (0..jobs.min(sources.len()))
            .map(|_| Vec::new())
            .collect();
        for (index, source) in sources.into_iter().enumerate() {
            let at = index % buckets.len();
            buckets[at].push(source);
        }
        buckets
    };
    thread::scope(|scope| {
        for chunk in &chunks {
            let sender = sender.clone();
            scope.spawn(move || {
                for source in chunk {
                    let stem = source
                        .file_stem()
                        .and_then(|s| s.to_str())
                        .unwrap_or("candidate");
                    let work = work_root.join(stem);
                    let _ = sender.send(
                        measure(root, self_exe, subcommand, source, &work));
                }
            });
        }
        drop(sender);
        let mut rows: Vec<Row> = receiver.iter().collect();
        rows.sort_by(compare);

        if args.iter().any(|arg| arg == "--json") {
            print!("{}", render_report(&rows));
        } else {
            report(root, &rows, top, only_worth);
        }
    });
    Ok(())
}

fn report(root: &Path, rows: &[Row], top: usize, only_worth: bool) {
    let mut counts: BTreeMap<&'static str, (i64, i64)> = BTreeMap::new();
    for row in rows {
        let entry = counts.entry(row.verdict.label()).or_insert((0, 0));
        entry.0 += 1;
        entry.1 += row.reference_bytes.unwrap_or(0);
    }
    println!("{:<22} {:>7} {:>9}", "verdict", "owners", "bytes");
    for (label, (owners, bytes)) in &counts {
        println!("{label:<22} {owners:>7} {bytes:>9}");
    }
    let reachable: Vec<&Row> = rows.iter().filter(|row| row.verdict.worth_opening()).collect();
    println!(
        "\nworth opening first: {} owners, {} bytes (residual is not a plain reorder)",
        reachable.len(),
        reachable
            .iter()
            .map(|row| row.reference_bytes.unwrap_or(0))
            .sum::<i64>()
    );
    let shown: Vec<&Row> = if only_worth {
        reachable
    } else {
        rows.iter().collect()
    };
    println!(
        "\n{:<9} {:>7} {:>7} {:>6} {:<20} source",
        "address", "bytes", "delta", "rows", "verdict"
    );
    for row in shown.iter().take(top) {
        let delta = match (row.candidate_bytes, row.reference_bytes) {
            (Some(candidate), Some(reference)) => format!("{:+}", candidate - reference),
            _ => "-".to_string(),
        };
        println!(
            "{:<9} {:>7} {:>7} {:>6} {:<20} {}",
            row.address,
            row.reference_bytes.map(|b| b.to_string()).unwrap_or_else(|| "-".into()),
            delta,
            row.differing_rows.map(|r| r.to_string()).unwrap_or_else(|| "-".into()),
            row.verdict.label(),
            row.source,
        );
    }
    let out = root.join("out/main-candidate-rank");
    if fs::create_dir_all(&out).is_ok() {
        let path = out.join("report.json");
        if fs::write(&path, render_report(rows)).is_ok() {
            // `root` is reached through `../..` from the crate directory, so the
            // joined path prints with the traversal still in it.
            let shown = fs::canonicalize(&path).unwrap_or(path);
            println!("\nreport={}", shown.display());
        }
    }
}

/// `--self-test`: the classifier's contract, without compiling anything.
pub fn self_test() -> Result<(), String> {
    // Built from explicit lines rather than with `\` continuations, which strip
    // the leading whitespace and so would delete the mark column these cases
    // exist to exercise -- the first draft of this test did exactly that and
    // reported the classifier broken.
    let lines = |rows: &[&str]| {
        let mut text = String::from("candidate=8 reference=8 differing_halfwords=2\n");
        text.push_str("      candidate                      reference\n");
        for row in rows {
            text.push_str(row);
            text.push('\n');
        }
        text
    };

    // A reorder: same instructions, different positions, and the moved load
    // carries a different pc displacement because it moved.
    let reorder = lines(&[
        "  + ldr\tr5, [pc, #524]\t@ (0x29c)",
        "    adds\tr7, r5, #0            adds\tr7, r5, #0",
        "  -                                ldr\tr5, [pc, #516]\t@ (0x29c)",
    ]);
    let (verdict, rows) = classify(&reorder);
    if verdict != Verdict::Reordering {
        return Err(format!("a relocated pool load must read as a reorder, got {verdict:?}"));
    }
    if rows != 2 {
        return Err(format!("expected 2 differing rows, got {rows}"));
    }

    // A `!` row is a PAIR: its left column belongs to the candidate and its
    // right to the reference. Splitting it wrongly is what inverts the verdict.
    // These four rows are `080a90bc`'s real residual.
    let paired = lines(&[
        "  + adds\tr1, #72\t@ 0x48",
        "  ! movs\tr2, #31                   movs\tr0, #245\t@ 0xf5",
        "  -                                adds\tr1, #72\t@ 0x48",
        "  ! movs\tr0, #245\t@ 0xf5           movs\tr2, #31",
    ]);
    if classify(&paired).0 != Verdict::Reordering {
        return Err("a reorder spread across ! rows must not read as disagreement".to_string());
    }

    // A real disagreement: one side stores a halfword where the other stores a
    // byte, which is a type the source decides. It must survive both the
    // register blanking and the reorder test.
    let differs = lines(&["  ! strb\tr3, [r2, #0]             strh\tr3, [r2, #0]"]);
    if classify(&differs).0 != Verdict::Divergent {
        return Err("differing store widths must stay divergent".to_string());
    }

    // Registers renamed and nothing else: `080a524c`'s real residual.
    let renamed = lines(&[
        "  ! movs\tr3, #1                    movs\tr2, #1",
        "  ! mov\tr8, r3                     mov\tr8, r2",
        "  ! cmp\tr3, #0                     cmp\tr2, #0",
    ]);
    if classify(&renamed).0 != Verdict::Allocation {
        return Err("a pure register renaming must read as allocation".to_string());
    }

    // A commutative add with its operands the other way round is still the same
    // instruction naming the same registers: `080b0958`.
    let swapped = lines(&["  ! adds\tr3, r3, r2                adds\tr3, r2, r3"]);
    if classify(&swapped).0 != Verdict::Allocation {
        return Err("a commutative operand swap must read as allocation".to_string());
    }

    // Blanking registers must not blank an immediate or a mnemonic.
    if register_blind("movs\tr2, #31") == register_blind("movs\tr2, #30") {
        return Err("register blanking must keep immediates distinct".to_string());
    }
    if register_blind("strb\tr3, [r2, #0]") == register_blind("strh\tr3, [r2, #0]") {
        return Err("register blanking must keep store widths distinct".to_string());
    }
    // r16 is not a register, and a symbol beginning with `r` is not either.
    if register_blind("bl\tr16_helper") != "bl r16_helper" {
        return Err(format!(
            "blanking reached into a symbol: {}",
            register_blind("bl\tr16_helper")
        ));
    }

    if classify(&lines(&[])).0 != Verdict::Exact {
        return Err("no differing rows must read as exact".to_string());
    }

    // The column split must survive an operand list containing spaces. Built
    // with real padding rather than counted spaces, so the case cannot drift
    // away from the renderer's `pad_end(text, 30)`.
    let padded = |left: &str, right: &str| {
        let mut row = String::from(" ");
        row.push_str(left);
        while row.chars().count() < 1 + CANDIDATE_COLUMN {
            row.push(' ');
        }
        row.push_str(right);
        row
    };
    let row = padded("push\t{r5, r6, r7, lr}", "pop\t{r0}");
    let (left, right) = split_columns(&row);
    if left != "push\t{r5, r6, r7, lr}" || right.as_deref() != Some("pop\t{r0}") {
        return Err(format!("column split cut the wrong place: {left:?} / {right:?}"));
    }

    // A candidate column of exactly 30 characters leaves NO padding, and one
    // longer than 30 is clipped and leaks its tail onto the reference column.
    // This is resource_373:0e54's real row: one space between the columns, and a
    // stray `)` from the clip. A run-of-spaces split reads the whole row as the
    // candidate side and reports a disagreement that is not there.
    let clipped = " ldr\tr2, [pc, #20]\t@ (0x2000e80) ldr\tr2, [pc, #16]\t@ (0x2000e80)";
    let (left, right) = split_columns(clipped);
    if normalise(&left) != "ldr r2, [pc]" {
        return Err(format!("clipped candidate column misread: {left:?}"));
    }
    match right.as_deref() {
        Some(text) if normalise(text) == "ldr r2, [pc]" => {}
        other => return Err(format!("clipped reference column misread: {other:?}")),
    }
    // And so the row is what it is: one load at two positions, not a difference.
    let leaked_row = format!("  !{clipped}");
    let leaked = lines(&[leaked_row.as_str()]);
    if classify(&leaked).0 != Verdict::Reordering {
        return Err("a clipped row must not read as a disagreement".to_string());
    }

    if normalise("ldr\tr3, [pc, #916]\t@ (0x2003744)") != normalise("ldr\tr3, [pc, #108]\t@ (0x200341c)")
    {
        return Err("two positions of one pool load must normalise alike".to_string());
    }
    if normalise("movs\tr2, #31") == normalise("movs\tr2, #30") {
        return Err("different immediates must not normalise alike".to_string());
    }

    if parse_score("candidate=304 reference=304 differing_halfwords=2") != Some((304, 304, 2)) {
        return Err("score line parse failed".to_string());
    }

    println!("self-test=ok checks=13");
    Ok(())
}
