//! Scratch directories for tests and for the parity harness.
//!
//! Everything lands under `tools-rs/permute-m2c/target/fixtures/`, inside this
//! crate, never in the shared scratchpad root and never in `out/` or `work/`.

use std::path::PathBuf;

pub fn fixtures_root() -> PathBuf {
    PathBuf::from(env!("CARGO_MANIFEST_DIR"))
        .join("target")
        .join("fixtures")
}

/// A named, existing directory under the crate's fixtures root.
pub fn fixture_dir(name: &str) -> PathBuf {
    let path = fixtures_root().join(name);
    std::fs::create_dir_all(&path).expect("fixture directory");
    path
}
