// Namespaced scratch directories for fixture-assembling tests.
//
// WHY: the real tree cannot exercise every branch -- there is no overlay whose
// whole-overlay claim has no semantic source, for instance -- so several tests
// assemble a small synthetic source tree instead of asserting a tautology.
// Those trees must never land in the repository or collide with another test
// process, so each one gets its own subdirectory under this crate's build
// directory.
//
#![cfg(test)]

use std::path::PathBuf;

fn scratch_root() -> PathBuf {
    PathBuf::from(env!("CARGO_MANIFEST_DIR")).join("target/fixtures/scratch")
}

/// A clean, empty directory named `name` inside this crate's fixture namespace.
pub fn scratch(name: &str) -> PathBuf {
    assert!(
        !name.is_empty() && name.chars().all(|c| c.is_ascii_alphanumeric() || c == '-'),
        "fixture names stay simple so the removal path below can never escape",
    );
    let base = scratch_root().join(name);
    // The path is rooted in this crate's manifest directory and constrained to
    // a single validated segment, so cleanup cannot escape its fixture area.
    if base.exists() {
        std::fs::remove_dir_all(&base).expect("clear the fixture directory");
    }
    std::fs::create_dir_all(&base).expect("create the fixture directory");
    base
}
