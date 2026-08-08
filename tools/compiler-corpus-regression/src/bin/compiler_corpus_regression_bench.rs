//! Decomposed benchmark: the two costs the task named, measured separately
//! from the whole-process time so a win or a loss can be attributed.
//!
//! Cost 1: `compiler_bundle_signature()`, which digests roughly 20 MB of
//! compiler binaries. native process's `CryptoHasher` is native C++; `alchemy_bundle`'s
//! portable sha256 is roughly 7x slower and that single fact made the
//! `overlay_disasm` port 5.9x SLOWER end to end. It is called once per process
//! here, so it is a constant, and it is reported as one.
//!
//! Cost 2: manifest parse. native process's `JSON.parse` is native and has beaten a Rust
//! port before. The manifest is the only JSON of any size this tool reads.
//!
//! Neither is memoized to make the number look better; memoizing the bundle
//! signature is a fix that belongs in `alchemy-bundle`, centrally.

use compiler_corpus_regression::corpus::hash;
use std::time::Instant;

fn main() {
    let root = alchemy_routing::routing::root();

    let start = Instant::now();
    let signature = alchemy_bundle::bundle::compiler_bundle_signature();
    let bundle_ms = start.elapsed().as_secs_f64() * 1000.0;
    println!("compiler_bundle_signature {bundle_ms:.1}ms {}", &signature[..16]);

    let manifest = root.join("out/full/claimed/manifest.json");
    match std::fs::read_to_string(&manifest) {
        Ok(text) => {
            let start = Instant::now();
            let parsed = candidate_compiler::json::parse(&text);
            let parse_ms = start.elapsed().as_secs_f64() * 1000.0;
            let regions = match &parsed {
                Ok(value) => match value.get("regions") {
                    Some(candidate_compiler::json::Json::Array(items)) => items.len(),
                    _ => 0,
                },
                Err(_) => 0,
            };
            println!(
                "json_parse {parse_ms:.1}ms bytes={} regions={regions}",
                text.len()
            );
        }
        // SCANNING NOTHING IS NOT PASSING: a benchmark that silently skips its
        // only real input is not a measurement.
        Err(error) => {
            eprintln!("{}: {error}", manifest.display());
            std::process::exit(1);
        }
    }

    let start = Instant::now();
    let mut sink = String::new();
    for index in 0..1000u32 {
        sink = hash(&[&index.to_le_bytes()]);
    }
    let hash_ms = start.elapsed().as_secs_f64() * 1000.0;
    println!("hash_x1000 {hash_ms:.1}ms {}", &sink[..16]);
}
