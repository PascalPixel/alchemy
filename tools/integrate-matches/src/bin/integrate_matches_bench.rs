//! Decomposed benchmark: whole-process, interpreter startup, and the work with
//! startup subtracted.
//!
//! Usage: `integrate-matches-bench <directory> <repeats>`. It times the Rust
//! pipeline in-process; the legacy implementation side and the startup floor are measured
//! by the harness script, which owns the comparison.

use std::time::Instant;

use integrate_matches::pipeline::run_pipeline;

fn main() {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    let Some(directory) = arguments.first() else {
        eprintln!("usage: integrate-matches-bench <directory> [repeats]");
        std::process::exit(2);
    };
    let repeats: u32 = arguments.get(1).and_then(|v| v.parse().ok()).unwrap_or(1);
    for _ in 0..repeats {
        let started = Instant::now();
        match run_pipeline(directory, false) {
            Ok(report) => println!(
                "elapsed_ms={:.1} accepted={} rejected={}",
                started.elapsed().as_secs_f64() * 1000.0,
                report.accepted,
                report.rejected
            ),
            Err(message) => {
                eprintln!("{message}");
                std::process::exit(1);
            }
        }
    }
}
