//! Benchmark. Two measurements are reported, and they are NOT comparable to
//! each other:
//!
//!   whole-process  `bun tools/lib/candidate_explain.ts CANDIDATE` against
//!                  `candidate-explain CANDIDATE`. This is the honest
//!                  like-for-like number: same work, same spawns, same I/O.
//!                  It is dominated by the five `arm-none-eabi-*` child
//!                  processes `verifyCandidate` runs per candidate, so the
//!                  language of the driver barely shows.
//!   in-process     `report()` alone, called directly on an already-compiled
//!                  candidate with a pre-seeded scheduler dump. There is no
//!                  TypeScript counterpart to this number, because Bun cannot
//!                  be entered without paying its startup, so it is reported
//!                  on its own and compared with NOTHING.

use std::path::{Path, PathBuf};
use std::process::Command;
use std::time::Instant;

use alchemy_routing::routing::root;
use candidate_explain::explain::report;

const REPEATS: usize = 5;

fn main() {
    let repository = root();
    let manifest = Path::new(env!("CARGO_MANIFEST_DIR"));
    let binary = manifest.join("target/release/candidate-explain");
    if !binary.exists() {
        eprintln!("build the release binary first: {}", binary.display());
        std::process::exit(1);
    }

    // Candidates that actually compile, so both sides do the full job rather
    // than racing each other to the same error message.
    let candidates: Vec<PathBuf> = ["08002dd8.c", "08028f98.c", "0800bb20.c"]
        .iter()
        .map(|name| repository.join("work/matches/m2c").join(name))
        .filter(|path| path.exists())
        .collect();
    if candidates.is_empty() {
        eprintln!("no benchmark candidate is present; measuring nothing is not a benchmark");
        std::process::exit(1);
    }

    let fixtures = manifest.join("target/fixtures/bench");
    let _ = std::fs::create_dir_all(&fixtures);

    // ---- whole-process ----
    let mut typescript = 0.0f64;
    let mut rust = 0.0f64;
    let mut runs = 0usize;
    for candidate in &candidates {
        for repeat in 0..REPEATS {
            let work = fixtures.join(format!("run{repeat}"));
            let _ = std::fs::create_dir_all(&work);
            typescript += time(
                Command::new("bun")
                    .current_dir(repository)
                    .arg("tools/lib/candidate_explain.ts")
                    .arg(candidate)
                    .arg("--work")
                    .arg(&work),
            );
            rust += time(
                Command::new(&binary)
                    .current_dir(repository)
                    .arg(candidate)
                    .arg("--work")
                    .arg(&work),
            );
            runs += 1;
        }
    }
    println!("whole-process runs={runs}");
    println!("whole-process typescript={:.3}s/run", typescript / runs as f64);
    println!("whole-process rust={:.3}s/run", rust / runs as f64);
    println!(
        "whole-process speedup={:.2}x",
        typescript / rust.max(f64::MIN_POSITIVE)
    );

    // ---- in-process, reported alone ----
    let seed = fixtures.join("seed");
    let _ = std::fs::create_dir_all(&seed);
    let Some((stem, actual)) = seed_one(repository, &binary, &candidates[0], &seed) else {
        eprintln!("could not stage an in-process fixture");
        std::process::exit(1);
    };
    let actual_path = seed.join(format!("{stem}.bin"));
    let start = Instant::now();
    let mut iterations = 0usize;
    while iterations < 50 {
        report(&actual, &actual, &seed, &actual_path, &stem).expect("report");
        iterations += 1;
    }
    println!(
        "in-process rust report()={:.4}s/call over {iterations} calls (NO TypeScript counterpart; \
do not compare this to the whole-process numbers)",
        start.elapsed().as_secs_f64() / iterations as f64
    );
}

fn time(command: &mut Command) -> f64 {
    let start = Instant::now();
    let _ = command.output();
    start.elapsed().as_secs_f64()
}

/// Runs the tool once so the work directory holds a compiled `.bin`, and moves
/// the scheduler dump gcc dropped in the repository root next to it. Returns
/// the routing stem and the compiled bytes.
fn seed_one(
    repository: &Path,
    binary: &Path,
    candidate: &Path,
    work: &Path,
) -> Option<(String, Vec<u8>)> {
    let stem = candidate.file_stem()?.to_string_lossy().into_owned();
    let _ = Command::new(binary)
        .current_dir(repository)
        .arg(candidate)
        .arg("--work")
        .arg(work)
        .output();
    let prefix = format!("{stem}.c.");
    for entry in std::fs::read_dir(repository).ok()?.flatten() {
        let name = entry.file_name().to_string_lossy().into_owned();
        if name.starts_with(&prefix) {
            // Relocated, never deleted.
            let _ = std::fs::rename(repository.join(&name), work.join(&name));
        }
    }
    let actual = std::fs::read(work.join(format!("{stem}.bin"))).ok()?;
    Some((stem, actual))
}
