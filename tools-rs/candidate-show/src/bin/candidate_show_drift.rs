//! Differential parity harness: the Rust port against the live TypeScript.
//!
//! WHOLE DOCUMENTS are compared byte for byte. Nothing is summarised before the
//! comparison, because a renderer that agrees on three header counts and
//! disagrees on a hundred table rows would pass a summary check.
//!
//! Every write this harness makes goes under
//! `tools-rs/candidate-show/target/fixtures/`. Neither `out/` nor anything
//! tracked under `work/` is touched, even though `candidate_show.ts` defaults
//! its own `--work` into `work/candidate-show`: both sides are given an
//! explicit `--work` inside the fixtures tree.
//!
//! SCANNING NOTHING IS NOT PASSING. A missing `bun`, a missing
//! `arm-none-eabi-objdump`, an unreadable ROM, an empty corpus, or a case count
//! under the floors all exit 1.

use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::Mutex;

use alchemy_routing::routing::root;
use candidate_show::cli::Options;
use candidate_show::render::render;
use match_m2c::verify::{CandidateCompilerConfiguration, CandidateCompilerFamily};

/// Cases attempted. Below this the run is not evidence of anything.
const CASE_FLOOR: usize = 60;
/// Distinct documents, so a corpus of clones cannot clear `CASE_FLOOR`.
const DISTINCT_FLOOR: usize = 20;
/// FLOOR THE THING ACTUALLY EXERCISED. Roughly two thirds of the corpus dies in
/// gcc, and those cases agree on an error string without ever reaching objdump,
/// `region_size`, `linked_function_extent`, the offset union, the numeric
/// comparator, or the padding. This floor counts documents that rendered a
/// non-empty side-by-side table, which is the only shape that exercises them.
const RENDERED_FLOOR: usize = 20;
/// Cases where candidate and reference lengths differ, which is the branch
/// where `js_subarray` clamping and the phantom-odd-length diff live.
const UNEQUAL_LENGTH_FLOOR: usize = 1;
/// Hard cases: rendered documents with at least 40 table rows.
const HARD_FLOOR: usize = 10;

#[derive(Debug, Clone)]
struct Case {
    stem: String,
    typescript: Outcome,
    rust: Outcome,
}

#[derive(Debug, Clone, PartialEq, Eq)]
struct Outcome {
    ok: bool,
    stdout: String,
}

fn main() {
    let root = root().to_path_buf();
    let fixtures = root.join("tools-rs/candidate-show/target/fixtures");
    require_tool("bun");
    require_tool("arm-none-eabi-objdump");
    require_tool("arm-none-eabi-nm");
    let rom = root.join("roms/gs1-en.gba");
    let rom_bytes = std::fs::metadata(&rom).map(|m| m.len()).unwrap_or(0);
    if rom_bytes == 0 {
        fail(&format!("unreadable or empty ROM: {}", rom.display()));
    }

    let corpus = collect_corpus(&root.join("work/matches/m2c"));
    if corpus.is_empty() {
        fail("empty corpus: work/matches/m2c holds no candidates");
    }
    eprintln!("corpus: {} candidates", corpus.len());

    // Sample deterministically: an even stride over the sorted corpus, so the
    // set is reproducible and is not the first N addresses of the ROM.
    let wanted = 100usize;
    let stride = std::cmp::max(1, corpus.len() / wanted);
    let sample: Vec<PathBuf> = corpus.iter().step_by(stride).take(wanted).cloned().collect();
    if sample.len() < CASE_FLOOR {
        fail(&format!(
            "sample of {} is below the case floor of {CASE_FLOOR}",
            sample.len()
        ));
    }

    let cases = run_all(&root, &fixtures, &rom, &sample);

    // NEGATIVE CONTROL, BEFORE the real comparison. Perturb exactly one Rust
    // document by one byte and prove the comparator reports exactly one
    // difference. A comparator that always passes would report zero here.
    negative_control(&cases);

    let mut differences: Vec<&Case> = Vec::new();
    for case in &cases {
        if case.typescript != case.rust {
            differences.push(case);
        }
    }

    let rendered = cases
        .iter()
        .filter(|case| typescript_rendered_rows(case) > 0)
        .count();
    let hard = cases
        .iter()
        .filter(|case| typescript_rendered_rows(case) >= 40)
        .count();
    let unequal = cases.iter().filter(|case| typescript_unequal(case)).count();
    let distinct = {
        let mut payloads: Vec<&str> = cases.iter().map(|case| case.typescript.stdout.as_str()).collect();
        payloads.sort_unstable();
        payloads.dedup();
        payloads.len()
    };

    println!("cases            {}", cases.len());
    println!("distinct         {distinct}");
    println!("rendered (ts)    {rendered}");
    println!("hard >=40 rows   {hard}");
    println!("unequal lengths  {unequal}");
    println!("differences      {}", differences.len());

    for case in differences.iter().take(5) {
        println!("--- {} ---", case.stem);
        print_first_difference(&case.typescript, &case.rust);
    }

    let mut failures: Vec<String> = Vec::new();
    floor(&mut failures, "cases", cases.len(), CASE_FLOOR);
    floor(&mut failures, "distinct payloads", distinct, DISTINCT_FLOOR);
    floor(&mut failures, "rendered documents (typescript side)", rendered, RENDERED_FLOOR);
    floor(&mut failures, "hard documents (typescript side)", hard, HARD_FLOOR);
    floor(&mut failures, "unequal-length documents (typescript side)", unequal, UNEQUAL_LENGTH_FLOOR);
    if !differences.is_empty() {
        failures.push(format!("{} documents differ", differences.len()));
    }
    if failures.is_empty() {
        println!("parity holds");
        return;
    }
    for failure in &failures {
        eprintln!("{failure}");
    }
    std::process::exit(1);
}

/// The counted quantity is the TYPESCRIPT side specifically, never the union of
/// both mirrors: a Rust-only rendering would otherwise inflate the floor it is
/// supposed to be measured against.
fn typescript_rendered_rows(case: &Case) -> usize {
    case.typescript
        .stdout
        .lines()
        .filter(|line| line.starts_with("  ! ") || line.starts_with("    "))
        .count()
}

fn typescript_unequal(case: &Case) -> bool {
    let Some(header) = case.typescript.stdout.lines().next() else {
        return false;
    };
    let mut fields = header.split(' ');
    let candidate = fields.next().and_then(|f| f.strip_prefix("candidate="));
    let reference = fields.next().and_then(|f| f.strip_prefix("reference="));
    match (candidate, reference) {
        (Some(a), Some(b)) => a != b,
        _ => false,
    }
}

fn floor(failures: &mut Vec<String>, label: &str, actual: usize, minimum: usize) {
    if actual < minimum {
        failures.push(format!("{label}: {actual} is below the floor of {minimum}"));
    }
}

fn negative_control(cases: &[Case]) {
    let Some(target) = cases.iter().find(|case| typescript_rendered_rows(case) > 0) else {
        fail("negative control: no rendered document to perturb");
    };
    let mut perturbed: Vec<Case> = cases.to_vec();
    let index = cases
        .iter()
        .position(|case| case.stem == target.stem)
        .expect("target is in the list");
    // One byte: the first differing-row marker becomes a space.
    let original = perturbed[index].rust.stdout.clone();
    perturbed[index].rust.stdout = original.replacen("  ! ", "    ", 1);
    if perturbed[index].rust.stdout == original {
        fail("negative control: the perturbation changed nothing");
    }
    let observed = perturbed
        .iter()
        .filter(|case| case.typescript != case.rust)
        .count();
    if observed != 1 {
        fail(&format!(
            "negative control: expected exactly 1 difference, saw {observed}"
        ));
    }
    println!("negative control  1 perturbation -> 1 difference");
}

fn print_first_difference(left: &Outcome, right: &Outcome) {
    if left.ok != right.ok {
        println!("exit ok: typescript={} rust={}", left.ok, right.ok);
    }
    let mut a = left.stdout.lines();
    let mut b = right.stdout.lines();
    let mut line = 0usize;
    loop {
        line += 1;
        match (a.next(), b.next()) {
            (None, None) => return,
            (x, y) if x == y => continue,
            (x, y) => {
                println!("line {line}\n  ts: {x:?}\n  rs: {y:?}");
                return;
            }
        }
    }
}

fn run_all(root: &Path, fixtures: &Path, rom: &Path, sample: &[PathBuf]) -> Vec<Case> {
    let workers = std::cmp::max(1, (available_cores() * 8) / 10);
    let next = AtomicUsize::new(0);
    let collected: Mutex<Vec<Case>> = Mutex::new(Vec::new());
    std::thread::scope(|scope| {
        for _ in 0..workers {
            scope.spawn(|| loop {
                let index = next.fetch_add(1, Ordering::SeqCst);
                if index >= sample.len() {
                    return;
                }
                let case = run_case(root, fixtures, rom, &sample[index]);
                collected.lock().expect("lock").push(case);
            });
        }
    });
    let mut cases = collected.into_inner().expect("lock");
    cases.sort_by(|left, right| left.stem.cmp(&right.stem));
    cases
}

fn run_case(root: &Path, fixtures: &Path, rom: &Path, source: &Path) -> Case {
    let stem = source
        .file_stem()
        .map(|s| s.to_string_lossy().into_owned())
        .unwrap_or_default();
    let typescript_work = fixtures.join("ts").join(&stem);
    let rust_work = fixtures.join("rs").join(&stem);
    let _ = std::fs::create_dir_all(&typescript_work);
    let _ = std::fs::create_dir_all(&rust_work);

    let output = Command::new("bun")
        .current_dir(root)
        .args([
            "tools/lib/candidate_show.ts",
            &source.to_string_lossy(),
            "--rom",
            &rom.to_string_lossy(),
            "--work",
            &typescript_work.to_string_lossy(),
        ])
        .output()
        .unwrap_or_else(|error| fail(&format!("bun failed to spawn: {error}")));
    let typescript = Outcome {
        ok: output.status.success(),
        stdout: String::from_utf8_lossy(&output.stdout).into_owned(),
    };

    let options = Options {
        source: source.to_string_lossy().into_owned(),
        rom: Some(rom.to_string_lossy().into_owned()),
        work: Some(rust_work.to_string_lossy().into_owned()),
        flags: Vec::new(),
        configuration: CandidateCompilerConfiguration {
            family: Some(CandidateCompilerFamily::Routed),
            add_flags: Vec::new(),
            remove_flags: Vec::new(),
        },
    };
    let rust = match render(root, &options) {
        Ok(output) => Outcome {
            ok: true,
            stdout: output.stdout,
        },
        Err(_) => Outcome {
            ok: false,
            stdout: String::new(),
        },
    };
    Case {
        stem,
        typescript,
        rust,
    }
}

fn collect_corpus(directory: &Path) -> Vec<PathBuf> {
    let Ok(entries) = std::fs::read_dir(directory) else {
        return Vec::new();
    };
    let mut paths: Vec<PathBuf> = entries
        .filter_map(Result::ok)
        .map(|entry| entry.path())
        .filter(|path| path.extension().is_some_and(|ext| ext == "c"))
        .collect();
    paths.sort();
    paths
}

fn require_tool(name: &str) {
    let found = Command::new("which")
        .arg(name)
        .output()
        .map(|output| output.status.success())
        .unwrap_or(false);
    if !found {
        fail(&format!("missing required tool: {name}"));
    }
}

fn available_cores() -> usize {
    std::thread::available_parallelism()
        .map(std::num::NonZeroUsize::get)
        .unwrap_or(1)
}

fn fail(message: &str) -> ! {
    eprintln!("{message}");
    std::process::exit(1);
}
