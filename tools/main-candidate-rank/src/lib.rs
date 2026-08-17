//! `main-rank`: rank every main-image candidate, and say which to open first.
//!
//! The overlay half has `overlay candidate-rank`, which as `bce2bc1e9` puts it is
//! "the tool that says which owners are worth opening". The main image --
//! 548,364 executable bytes, the larger half of the target -- had no such tool, so
//! its drafts' distance from the reference was unmeasured in bulk and owners were
//! chosen by size from the target table instead.
//!
//! The verdict comes from `candidate_show::render::residual_class`, the same
//! function `overlay score` and `overlay candidate-rank` use, so the two halves
//! cannot disagree about whether an owner is worth opening.
//!
//! This file used to carry its OWN copy of that classification, because
//! `candidate-show` did not print the class and only the overlay half called the
//! shared function. Two implementations of one judgement is a defect that waits:
//! when `unemittable` was added -- a reference-side `stmia` or `ldmia` proving the
//! region was never C, which arm.md gates on TARGET_ARM -- the copy could not see
//! it, and went on offering hand-written assembly regions as readable source
//! defects. `candidate-show` now prints `class=` and this reads it.
//!
//! Ranking is by verdict first and distance second. A differing-halfword count
//! alone puts a blocked tie and a two-line defect in the same tier.

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
    /// `wrong_instructions` from the shared classifier: instructions the two
    /// sides disagree about, counted once per side. The ranking key, because a
    /// halfword count is not comparable once the sizes differ.
    pub wrong_instructions: Option<i64>,
    pub verdict: Verdict,
    pub error: Option<String>,
}

/// The shared verdict, ordered so the readable defects sort first.
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub enum Verdict {
    /// Byte-identical at the right size.
    Exact,
    /// The two sides emit different instructions, and none of the cheaper
    /// explanations apply: a type, a prototype, a constant, an evaluation order.
    Wrong,
    /// Sizes differ, so there is no meaningful positional comparison.
    SizeMismatch,
    /// The same instructions naming different registers. The allocator's choice.
    Allocation,
    /// The same instructions in a different order. The scheduler's choice.
    Ordering,
    /// A reference-side `stmia`/`ldmia`: stock gcc 2.96 cannot emit a Thumb
    /// multiple transfer from any source, so the region is hand-written assembly
    /// or a library object and no reading of the C reaches it.
    Unemittable,
    /// The candidate did not compile, or produced no score.
    Unscored,
}

impl Verdict {
    pub const fn label(self) -> &'static str {
        match self {
            Verdict::Exact => "exact",
            Verdict::Wrong => "wrong",
            Verdict::SizeMismatch => "size-mismatch",
            Verdict::Allocation => "allocation",
            Verdict::Ordering => "ordering",
            Verdict::Unemittable => "unemittable",
            Verdict::Unscored => "unscored",
        }
    }

    /// The class string `candidate-show` prints.
    pub fn from_class(text: &str) -> Option<Self> {
        Some(match text {
            "exact" => Verdict::Exact,
            "wrong" => Verdict::Wrong,
            "allocation" => Verdict::Allocation,
            "ordering" => Verdict::Ordering,
            "unemittable" => Verdict::Unemittable,
            _ => return None,
        })
    }

    /// Whether reading the source is the next step. Not a promise that a reading
    /// exists -- only that the cheaper explanations have been ruled out.
    pub const fn worth_opening(self) -> bool {
        matches!(self, Verdict::Wrong | Verdict::SizeMismatch)
    }
}

/// `class=<name> wrong_instructions=<n>`, as `candidate-show` prints it.
fn parse_class(output: &str) -> Option<(Verdict, i64)> {
    let at = output.find("class=")?;
    let rest = &output[at + "class=".len()..];
    let name: String = rest.chars().take_while(|c| !c.is_whitespace()).collect();
    let verdict = Verdict::from_class(&name)?;
    let key = "wrong_instructions=";
    let found = rest.find(key)?;
    let digits: String = rest[found + key.len()..]
        .chars()
        .take_while(char::is_ascii_digit)
        .collect();
    Some((verdict, digits.parse().unwrap_or(0)))
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

/// Owners withdrawn from routine work, from `semantic/unmatchable.json`.
///
/// A main-image owner is registered under its bare address, `08002f10`, which
/// is also its source stem, so no translation is needed here -- unlike the
/// overlay half, where the register and the reading list spell an owner
/// differently.
///
/// Scanned rather than parsed: this crate has no dependencies and pulling a
/// JSON one in for four hundred bytes of advisory data is not worth it. Same
/// call `rom_overlay` makes against `assets/manifest.json`. A missing or
/// malformed register yields an empty set, because ranking a withdrawn owner is
/// a smaller failure than refusing to rank at all.
fn unmatchable_owners(root: &Path) -> std::collections::BTreeSet<String> {
    match fs::read_to_string(root.join("semantic/unmatchable.json")) {
        Ok(text) => unmatchable_from_text(&text),
        Err(_) => std::collections::BTreeSet::new(),
    }
}

/// The scan itself, kept separate so the self-test can exercise it without a
/// fixture on disk.
fn unmatchable_from_text(text: &str) -> std::collections::BTreeSet<String> {
    let mut out = std::collections::BTreeSet::new();
    for piece in text.split("\"owner\"").skip(1) {
        let Some(rest) = piece.split_once(':').map(|(_, rest)| rest) else {
            continue;
        };
        let Some(start) = rest.find('"') else { continue };
        let tail = &rest[start + 1..];
        let Some(end) = tail.find('"') else { continue };
        out.insert(tail[..end].to_string());
    }
    out
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
                wrong_instructions: None,
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
            wrong_instructions: None,
            verdict: Verdict::Unscored,
            error: Some(first),
        };
    };
    // The shared classifier's own verdict, straight off `candidate-show`'s
    // `class=` line. A size difference outranks it: once the sizes differ the
    // positional comparison is meaningless, so the row is size-mismatch whatever
    // the class says, and the wrong-instruction count is the only usable distance.
    let (class, wrong) = parse_class(&text).unwrap_or((Verdict::Unscored, 0));
    let verdict = if candidate != reference {
        Verdict::SizeMismatch
    } else {
        class
    };
    Row {
        address,
        source: relative,
        candidate_bytes: Some(candidate),
        reference_bytes: Some(reference),
        differing_halfwords: Some(halfwords),
        wrong_instructions: Some(wrong),
        verdict,
        error: None,
    }
}

/// Rank order: reachable owners first, closest first, largest first.
pub fn compare(left: &Row, right: &Row) -> std::cmp::Ordering {
    let key = |row: &Row| {
        (
            row.verdict,
            row.wrong_instructions.unwrap_or(i64::MAX),
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
        if let Some(value) = row.wrong_instructions {
            out.push_str(&format!("      \"wrongInstructions\": {value},\n"));
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
    // The register's job is to keep withdrawn owners out of this queue. The
    // overlay ranker had the same gap and had all four of its entries in the
    // first twenty rows; 30 main-image owners are registered here.
    let withdrawn = unmatchable_owners(root);
    let before = sources.len();
    sources.retain(|path| {
        path.file_stem()
            .and_then(|stem| stem.to_str())
            .is_none_or(|stem| !withdrawn.contains(stem))
    });
    let dropped = before - sources.len();
    if dropped > 0 {
        // Said out loud: a queue that shrinks quietly reads as coverage.
        eprintln!("withdrawn={dropped} (semantic/unmatchable.json)");
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
        "address", "bytes", "delta", "wrong", "verdict"
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
            row.wrong_instructions.map(|r| r.to_string()).unwrap_or_else(|| "-".into()),
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

/// `--self-test`: this crate's own contract. The classification it no longer owns
/// is tested where it now lives, in `candidate_show::render`.
pub fn self_test() -> Result<(), String> {
    // The class line is the whole interface to the shared verdict.
    let cases = [
        ("class=exact wrong_instructions=0", Verdict::Exact, 0),
        ("class=wrong wrong_instructions=14", Verdict::Wrong, 14),
        ("class=ordering wrong_instructions=0", Verdict::Ordering, 0),
        ("class=allocation wrong_instructions=2", Verdict::Allocation, 2),
        ("class=unemittable wrong_instructions=8", Verdict::Unemittable, 8),
    ];
    for (text, want, count) in cases {
        match parse_class(text) {
            Some((verdict, wrong)) if verdict == want && wrong == count => {}
            other => return Err(format!("{text:?} parsed as {other:?}")),
        }
    }
    // A class this crate does not know must not be guessed at.
    if parse_class("class=something-new wrong_instructions=0").is_some() {
        return Err("an unknown class must not parse".to_string());
    }
    if parse_class("candidate=8 reference=8 differing_halfwords=0").is_some() {
        return Err("a score line without a class must not parse".to_string());
    }

    // Ranking order: the two a reader should open come first, and the three the
    // compiler decides come last.
    let mut order = vec![
        Verdict::Unemittable,
        Verdict::Ordering,
        Verdict::Wrong,
        Verdict::Exact,
        Verdict::Allocation,
        Verdict::SizeMismatch,
    ];
    order.sort();
    if order
        != vec![
            Verdict::Exact,
            Verdict::Wrong,
            Verdict::SizeMismatch,
            Verdict::Allocation,
            Verdict::Ordering,
            Verdict::Unemittable,
        ]
    {
        return Err(format!("verdicts sort in the wrong order: {order:?}"));
    }
    for verdict in [Verdict::Wrong, Verdict::SizeMismatch] {
        if !verdict.worth_opening() {
            return Err(format!("{} should be worth opening", verdict.label()));
        }
    }
    for verdict in [
        Verdict::Ordering,
        Verdict::Allocation,
        Verdict::Unemittable,
        Verdict::Exact,
    ] {
        if verdict.worth_opening() {
            return Err(format!("{} should not be worth opening", verdict.label()));
        }
    }

    if parse_score("candidate=304 reference=304 differing_halfwords=2") != Some((304, 304, 2)) {
        return Err("score line parse failed".to_string());
    }

    println!("self-test=ok checks=14");
    Ok(())
}
