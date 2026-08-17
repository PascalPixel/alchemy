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
//!   * the aligned view's two columns are tab-formatted, so splitting them at a
//!     fixed character offset cuts through the middle of an operand list.
//!
//! Without the first, 15 reorderings read as disagreements; without the second,
//! the split is wrong on every row wide enough to matter and the verdict
//! inverts outright.

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

/// Split an aligned `!` row into its candidate and reference halves.
///
/// The columns are padded with spaces but the instructions themselves contain
/// tabs, so the run of padding is the only reliable separator; a fixed offset
/// lands inside an operand list on any row wide enough to need one.
pub fn split_columns(body: &str) -> (String, Option<String>) {
    let trimmed = body.trim_end();
    let mut gap: Option<usize> = None;
    let chars: Vec<char> = trimmed.chars().collect();
    let mut run = 0usize;
    for (index, ch) in chars.iter().enumerate() {
        if *ch == ' ' {
            run += 1;
        } else {
            if run >= 3 {
                gap = Some(index - run);
                break;
            }
            run = 0;
        }
    }
    match gap {
        Some(at) => {
            let left: String = chars[..at].iter().collect();
            let right: String = chars[at..].iter().collect();
            (left.trim().to_string(), Some(right.trim().to_string()))
        }
        None => (trimmed.trim().to_string(), None),
    }
}

/// Classify one aligned `candidate-show` output.
pub fn classify(output: &str) -> (Verdict, i64) {
    let mut left: BTreeMap<String, i64> = BTreeMap::new();
    let mut right: BTreeMap<String, i64> = BTreeMap::new();
    let mut differing = 0i64;
    for line in output.lines().skip(2) {
        if line.len() < 3 {
            continue;
        }
        let mark = line[..3].trim();
        let body = &line[3..];
        match mark {
            "+" => {
                *left.entry(normalise(body)).or_default() += 1;
                differing += 1;
            }
            "-" => {
                *right.entry(normalise(body)).or_default() += 1;
                differing += 1;
            }
            "!" => {
                let (a, b) = split_columns(body);
                *left.entry(normalise(&a)).or_default() += 1;
                if let Some(b) = b {
                    *right.entry(normalise(&b)).or_default() += 1;
                }
                differing += 1;
            }
            _ => {}
        }
    }
    if differing == 0 {
        return (Verdict::Exact, 0);
    }
    if left == right {
        (Verdict::Reordering, differing)
    } else {
        (Verdict::Divergent, differing)
    }
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
    // byte, which is a type the source decides.
    let differs = lines(&["  ! strb\tr3, [r2, #0]             strh\tr3, [r2, #0]"]);
    if classify(&differs).0 != Verdict::Divergent {
        return Err("differing store widths must read as a source disagreement".to_string());
    }

    if classify(&lines(&[])).0 != Verdict::Exact {
        return Err("no differing rows must read as exact".to_string());
    }

    // The column split must survive an operand list containing spaces.
    let (left, right) = split_columns("push\t{r5, r6, r7, lr}          pop\t{r0}");
    if left != "push\t{r5, r6, r7, lr}" || right.as_deref() != Some("pop\t{r0}") {
        return Err(format!("column split cut the wrong place: {left:?} / {right:?}"));
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

    println!("self-test=ok checks=8");
    Ok(())
}
