// Namespaced scratch directories for fixture-assembling tests.
//
// WHY: the real tree cannot exercise every branch -- there is no overlay whose
// whole-overlay claim has no semantic source, for instance -- so several tests
// assemble a small synthetic source tree instead of asserting a tautology.
// Those trees must never land in the repository and never collide with another
// agent's harness, so each one gets its own subdirectory under this crate's
// own build directory, wiped by an explicit literal-rooted removal.
//
// `target/fixtures/` rather than the shared /tmp scratchpad: it is untracked,
// it is inside the one directory this crate owns, and a sibling agent cannot
// reach it by accident.

#![cfg(test)]

use std::path::PathBuf;

const SCRATCH_ROOT: &str =
    "/Users/pixel/Developer/PascalPixel/alchemy/tools/coverage-map/target/fixtures/scratch";

/// A clean, empty directory named `name` inside this crate's fixture namespace.
pub fn scratch(name: &str) -> PathBuf {
    assert!(
        !name.is_empty() && name.chars().all(|c| c.is_ascii_alphanumeric() || c == '-'),
        "fixture names stay simple so the removal path below can never escape",
    );
    let base = PathBuf::from(SCRATCH_ROOT).join(name);
    // The removed path is rooted in a literal constant and constrained to a
    // single validated segment; nothing outside this crate's namespace is
    // reachable from here.
    if base.exists() {
        std::fs::remove_dir_all(&base).expect("clear the fixture directory");
    }
    std::fs::create_dir_all(&base).expect("create the fixture directory");
    base
}
