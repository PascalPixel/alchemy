//! An honest benchmark: this crate's `verify_candidate` against the
//! TypeScript's, over the same real candidates, single-threaded on both sides.
//!
//! Both sides spend nearly all of their wall clock inside the same five
//! `arm-none-eabi-*` processes, so this measures the thin layer around them and
//! nothing more. Expect a wash.

use std::path::PathBuf;
use std::time::Instant;

use alchemy_routing::routing::{root, CompilerTarget};
use match_m2c::verify::{verify_candidate, CandidateCompilerConfiguration, ROM_BASE};

fn main() {
    let count: usize = std::env::var("MATCH_M2C_BENCH")
        .ok()
        .and_then(|value| value.parse().ok())
        .unwrap_or(40);
    let directory = root().join("work/matches/m2c");
    let mut files: Vec<PathBuf> = std::fs::read_dir(&directory)
        .expect("corpus must be readable")
        .filter_map(Result::ok)
        .map(|entry| entry.path())
        .filter(|path| path.extension().is_some_and(|ext| ext == "c"))
        .collect();
    files.sort();
    assert!(!files.is_empty(), "empty corpus");
    let sample: Vec<PathBuf> = files.iter().take(400).step_by(10).take(count).cloned().collect();

    let target = decomp_targets::target_for(decomp_targets::DEFAULT_TARGET);
    let rom = std::fs::read(root().join(target.rom)).expect("ROM must be readable");
    let out = PathBuf::from(env!("CARGO_MANIFEST_DIR")).join("target/fixtures/bench-rs");
    std::fs::create_dir_all(&out).expect("fixture directory");

    let started = Instant::now();
    let mut ok = 0usize;
    for candidate in &sample {
        if verify_candidate(
            candidate.to_string_lossy().as_ref(),
            &rom,
            out.to_string_lossy().as_ref(),
            &[],
            ROM_BASE,
            CompilerTarget::Gs1,
            &CandidateCompilerConfiguration::default(),
        )
        .is_ok()
        {
            ok += 1;
        }
    }
    println!(
        "rust candidates={} ok={ok} elapsed={:.3}s",
        sample.len(),
        started.elapsed().as_secs_f64()
    );
}
