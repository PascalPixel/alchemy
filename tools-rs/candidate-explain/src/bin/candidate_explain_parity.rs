//! Differential parity harness: the live TypeScript against this port, over the
//! real candidate corpus and the real ROM.
//!
//! Two phases run per candidate, because the tool has two materially different
//! output shapes and the interesting one is NOT the one you get by default:
//!
//!   phase A  no scheduler dump in the work directory. This is what an ordinary
//!            invocation produces today (see the `dumps land in ROOT` defect in
//!            the report), and it exercises disassembly, halfword diffing and
//!            the table layout, but NOT alignment, `diagnose` or `formatPair`.
//!   phase B  the dump gcc actually wrote is relocated into the work directory,
//!            identically for both implementations, so `findDumpFile` succeeds.
//!            This is the phase that exercises the whole analysis core.
//!
//! Floors are counted on the TYPESCRIPT side only, never on the union of both
//! mirrors, so a Rust-only behaviour can never inflate a floor it is being
//! measured against.
//!
//! Nothing is written to `out/` or to anything tracked under `work/`. Every
//! byte this harness produces lands under
//! `tools-rs/candidate-explain/target/fixtures/parity/`, and the stray dumps
//! gcc drops into the repository root are RELOCATED there (`fs::rename`), never
//! deleted.

use std::collections::BTreeSet;
use std::path::{Path, PathBuf};
use std::process::Command;

use alchemy_routing::routing::root;

const DEFAULT_CASES: usize = 200;

/// Floors. Each is a count of TypeScript-side outcomes.
const FLOOR_CASES: usize = 300;
const FLOOR_DISTINCT: usize = 80;
const FLOOR_NO_DUMP: usize = 40;
const FLOOR_WITH_DUMP: usize = 40;
/// THE FLOOR ON THE THING ACTUALLY EXERCISED. `report_with_dump` alone would be
/// cleared by 40 outputs that merely say "(scheduler trace: 0 RTL insns ...)"
/// while `align`, `diagnose` and `formatPair` never produced a single row. This
/// floor counts only outputs carrying at least one real comparison row
/// ("beats " or "?? model expects"), which is the analysis core proper.
const FLOOR_TRACE_ROWS: usize = 30;

struct Case {
    source: PathBuf,
    stem: String,
}

struct Tally {
    cases: usize,
    compile_error: usize,
    report_no_dump: usize,
    report_with_dump: usize,
    with_trace_rows: usize,
    distinct: BTreeSet<String>,
    differences: Vec<String>,
}

fn main() {
    let repository = root();
    let fixtures = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/fixtures/parity");

    // --- scanning nothing is not passing -----------------------------------
    require_program("bun", &["--version"]);
    require_program("arm-none-eabi-objdump", &["--version"]);
    let rom = repository.join("roms/gs1-en.gba");
    let rom_bytes = std::fs::read(&rom)
        .unwrap_or_else(|error| die(&format!("unreadable ROM {}: {error}", rom.display())));
    if rom_bytes.len() < 0x100 {
        die(&format!("ROM {} is implausibly short", rom.display()));
    }

    let corpus = repository.join("work/matches/m2c");
    let mut sources: Vec<PathBuf> = std::fs::read_dir(&corpus)
        .unwrap_or_else(|error| die(&format!("unreadable corpus {}: {error}", corpus.display())))
        .flatten()
        .map(|entry| entry.path())
        .filter(|path| path.extension().is_some_and(|ext| ext == "c"))
        .collect();
    sources.sort();
    if sources.is_empty() {
        die(&format!("corpus {} is empty", corpus.display()));
    }
    eprintln!("corpus={} candidates at {}", sources.len(), corpus.display());

    let wanted: usize = std::env::args()
        .nth(1)
        .and_then(|text| text.parse().ok())
        .unwrap_or(DEFAULT_CASES);
    // Spread the sample across the whole corpus rather than taking a prefix:
    // the first dozen entries are all consecutive addresses from one region and
    // all fail to compile the same way.
    let stride = (sources.len() / wanted).max(1);
    let cases: Vec<Case> = sources
        .iter()
        .step_by(stride)
        .take(wanted)
        .map(|path| Case {
            stem: path
                .file_stem()
                .map(|s| s.to_string_lossy().into_owned())
                .unwrap_or_default(),
            source: path.clone(),
        })
        .collect();

    let rust_binary = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/release/candidate-explain");
    if !rust_binary.exists() {
        die(&format!(
            "build the release binary first: {}",
            rust_binary.display()
        ));
    }

    // --- negative control, BEFORE the real comparison -----------------------
    eprintln!("negative control: perturbing case 1 of 3");
    let control = run_sample(repository, &fixtures.join("control"), &rust_binary, &cases[..3], 1);
    if control.differences.len() != 1 {
        die(&format!(
            "negative control produced {} differences, expected exactly 1: {:?}",
            control.differences.len(),
            control.differences
        ));
    }
    if !control.differences[0].contains(&cases[1].stem) {
        die(&format!(
            "negative control flagged the wrong case: {}",
            control.differences[0]
        ));
    }
    eprintln!("negative control ok: exactly 1 difference, on the perturbed case");

    // --- the real comparison ------------------------------------------------
    let tally = run_sample(repository, &fixtures.join("real"), &rust_binary, &cases, usize::MAX);

    println!("cases={}", tally.cases);
    println!("distinct_payloads={}", tally.distinct.len());
    println!("ts_compile_error={}", tally.compile_error);
    println!("ts_report_no_dump={}", tally.report_no_dump);
    println!("ts_report_with_dump={}", tally.report_with_dump);
    println!("ts_with_trace_rows={}", tally.with_trace_rows);
    println!("differences={}", tally.differences.len());
    for difference in &tally.differences {
        println!("  {difference}");
    }

    let mut failures: Vec<String> = Vec::new();
    let mut floor = |name: &str, seen: usize, want: usize| {
        if seen < want {
            failures.push(format!("{name}={seen} below floor {want}"));
        }
    };
    floor("cases", tally.cases, FLOOR_CASES);
    floor("distinct_payloads", tally.distinct.len(), FLOOR_DISTINCT);
    floor("ts_report_no_dump", tally.report_no_dump, FLOOR_NO_DUMP);
    floor("ts_report_with_dump", tally.report_with_dump, FLOOR_WITH_DUMP);
    floor("ts_with_trace_rows", tally.with_trace_rows, FLOOR_TRACE_ROWS);
    if !tally.differences.is_empty() {
        failures.push(format!("{} parity difference(s)", tally.differences.len()));
    }
    if !failures.is_empty() {
        die(&format!("parity FAILED: {}", failures.join("; ")));
    }
    println!("parity=ok tool=candidate-explain");
}

/// Runs both implementations over `cases`, in both phases.
///
/// `perturb_index` names the case whose Rust output is deliberately corrupted;
/// pass `usize::MAX` for a real run.
fn run_sample(
    repository: &Path,
    workspace: &Path,
    rust_binary: &Path,
    cases: &[Case],
    perturb_index: usize,
) -> Tally {
    let mut tally = Tally {
        cases: 0,
        compile_error: 0,
        report_no_dump: 0,
        report_with_dump: 0,
        with_trace_rows: 0,
        distinct: BTreeSet::new(),
        differences: Vec::new(),
    };

    for (index, case) in cases.iter().enumerate() {
        let case_dir = workspace.join(format!("{:04}-{}", index, case.stem));
        let ts_work = case_dir.join("ts");
        let rs_work = case_dir.join("rs");
        let quarantine = case_dir.join("root-dumps");
        for directory in [&ts_work, &rs_work, &quarantine] {
            let _ = std::fs::create_dir_all(directory);
        }

        // ---- phase A: no dump in the work directory ----
        let ts_a = run_typescript(repository, &case.source, &ts_work);
        let dump = drain_root_dumps(repository, &case.stem, &quarantine);
        let rs_a = run_rust(repository, rust_binary, &case.source, &rs_work);
        drain_root_dumps(repository, &case.stem, &quarantine);
        record(
            &mut tally,
            &Observation {
                label: format!("{}#A", case.stem),
                ts: &ts_a,
                rs: &rs_a,
                ts_work: &ts_work,
                rs_work: &rs_work,
                perturb: index == perturb_index,
            },
        );

        // ---- phase B: the dump gcc really wrote, seeded into both sides ----
        let Some((name, bytes)) = dump else {
            continue;
        };
        let ts_work_b = case_dir.join("ts-b");
        let rs_work_b = case_dir.join("rs-b");
        for directory in [&ts_work_b, &rs_work_b] {
            let _ = std::fs::create_dir_all(directory);
            let _ = std::fs::write(directory.join(&name), &bytes);
        }
        let ts_b = run_typescript(repository, &case.source, &ts_work_b);
        drain_root_dumps(repository, &case.stem, &quarantine);
        let rs_b = run_rust(repository, rust_binary, &case.source, &rs_work_b);
        drain_root_dumps(repository, &case.stem, &quarantine);
        record(
            &mut tally,
            &Observation {
                label: format!("{}#B", case.stem),
                ts: &ts_b,
                rs: &rs_b,
                ts_work: &ts_work_b,
                rs_work: &rs_work_b,
                perturb: false,
            },
        );
    }
    tally
}

/// A single invocation's observable result.
struct Outcome {
    code: i32,
    /// stdout, which is the contracted payload.
    text: String,
    /// stderr, which is NOT contracted. Bun prints a source frame and a stack
    /// trace where the Rust binary prints one line, so the two are compared as
    /// SAME-FAILURE (exit code, non-empty diagnosis, the offending candidate
    /// named) rather than same-prose.
    stderr: String,
}

/// One comparison's inputs, bundled so `record` takes two parameters instead
/// of eight. Nothing is suppressed with `#[allow]`.
struct Observation<'a> {
    label: String,
    ts: &'a Outcome,
    rs: &'a Outcome,
    ts_work: &'a Path,
    rs_work: &'a Path,
    /// Whether this case is the negative control's deliberate perturbation.
    perturb: bool,
}

fn record(tally: &mut Tally, observation: &Observation) {
    let Observation {
        label,
        ts,
        rs,
        ts_work,
        rs_work,
        perturb,
    } = observation;
    let (ts_work, rs_work, perturb) = (*ts_work, *rs_work, *perturb);
    tally.cases += 1;

    // Categories, counted from the TYPESCRIPT side only.
    let ts_text = normalise(&ts.text, ts_work);
    if ts.code != 0 {
        tally.compile_error += 1;
    } else if ts_text.contains("(no scheduler dump found") {
        tally.report_no_dump += 1;
    } else if ts_text.contains("(scheduler trace:") {
        tally.report_with_dump += 1;
        if ts_text.contains("beats ") || ts_text.contains("?? model expects") {
            tally.with_trace_rows += 1;
        }
    }
    tally.distinct.insert(format!("{}|{}", ts.code, ts_text));

    let mut rs_text = normalise(&rs.text, rs_work);
    if perturb {
        // The deliberate perturbation: corrupt the halfword count, which lands
        // in the first line of every report. A candidate that failed to compile
        // has no such line, so that shape gets a trailing marker instead. Either
        // way exactly one case is perturbed and exactly one must be reported.
        let before = rs_text.clone();
        rs_text = rs_text.replacen("differing_halfwords=", "differing_halfwords=9", 1);
        if rs_text == before {
            rs_text.push_str("[perturbed]\n");
        }
    }
    if ts.code != rs.code || ts_text != rs_text {
        tally.differences.push(format!(
            "{label}: ts_exit={} rs_exit={} first_delta={}",
            ts.code,
            rs.code,
            first_delta(&ts_text, &rs_text)
        ));
        return;
    }
    // Same-failure, not same-prose.
    if ts.code != 0 && !perturb {
        let stem = label.split('#').next().unwrap_or(label);
        for (side, outcome) in [("ts", ts), ("rs", rs)] {
            if outcome.stderr.trim().is_empty() {
                tally
                    .differences
                    .push(format!("{label}: {side} failed with an empty diagnosis"));
            }
        }
        // Whether the diagnosis names the offending candidate is compared
        // BETWEEN the two sides rather than demanded of each. Some xgcc failures
        // report only the compiler's own message, and both mirrors are then
        // equally silent about the path; that is parity, not a defect.
        if ts.stderr.contains(stem) != rs.stderr.contains(stem) {
            tally.differences.push(format!(
                "{label}: only one side names the offending candidate (ts={}, rs={})",
                ts.stderr.contains(stem),
                rs.stderr.contains(stem)
            ));
        }
    }
}

/// Strips the one thing that legitimately differs: the work directory path,
/// which appears in the `full dump:` line and in error messages.
fn normalise(text: &str, work: &Path) -> String {
    text.replace(&work.to_string_lossy().into_owned(), "WORK")
}

fn first_delta(left: &str, right: &str) -> String {
    for (index, (a, b)) in left.lines().zip(right.lines()).enumerate() {
        if a != b {
            return format!("line {index}: {a:?} vs {b:?}");
        }
    }
    format!("line counts {} vs {}", left.lines().count(), right.lines().count())
}

fn run_typescript(repository: &Path, source: &Path, work: &Path) -> Outcome {
    capture(
        Command::new("bun")
            .current_dir(repository)
            .arg("tools/lib/candidate_explain.ts")
            .arg(source)
            .arg("--work")
            .arg(work),
    )
}

fn run_rust(repository: &Path, binary: &Path, source: &Path, work: &Path) -> Outcome {
    capture(
        Command::new(binary)
            .current_dir(repository)
            .arg(source)
            .arg("--work")
            .arg(work),
    )
}

fn capture(command: &mut Command) -> Outcome {
    let result = command
        .output()
        .unwrap_or_else(|error| die(&format!("spawn failed: {error}")));
    Outcome {
        code: result.status.code().unwrap_or(-1),
        text: String::from_utf8_lossy(&result.stdout).into_owned(),
        stderr: String::from_utf8_lossy(&result.stderr).into_owned(),
    }
}

/// gcc writes its `-dR` dumps into the process cwd (the repository root), not
/// into `--work`. This moves them out with `fs::rename` and returns the
/// scheduler dump's name and bytes so both implementations can be seeded with
/// exactly the same file. No path here is ever passed to a delete.
fn drain_root_dumps(repository: &Path, stem: &str, quarantine: &Path) -> Option<(String, Vec<u8>)> {
    let prefix = format!("{stem}.c.");
    let mut found = None;
    let Ok(entries) = std::fs::read_dir(repository) else {
        return None;
    };
    for entry in entries.flatten() {
        let name = entry.file_name().to_string_lossy().into_owned();
        if !name.starts_with(&prefix) {
            continue;
        }
        let from = repository.join(&name);
        if found.is_none() && name.ends_with(".sched2") {
            if let Ok(bytes) = std::fs::read(&from) {
                found = Some((name.clone(), bytes));
            }
        }
        let _ = std::fs::rename(&from, quarantine.join(&name));
    }
    found
}

fn require_program(program: &str, arguments: &[&str]) {
    let ok = Command::new(program)
        .args(arguments)
        .output()
        .map(|output| output.status.success())
        .unwrap_or(false);
    if !ok {
        die(&format!("required program not usable: {program}"));
    }
}

fn die(message: &str) -> ! {
    eprintln!("{message}");
    std::process::exit(1);
}
