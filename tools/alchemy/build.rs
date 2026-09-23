//! The build implementation signature: one digest of the tool sources that
//! can change what a build stage writes into its caches. Object, assembly,
//! overlay, score and asset caches key on it instead of on the whole
//! executable, so a change to reports, checks, coverage or the dashboard no
//! longer recompiles every C file, as pret's Makefile rebuilds only what a
//! changed tool produces. The byte-identical ROM comparison stays the proof;
//! the signature decides only what may be reused.
use sha2::{Digest, Sha256};
use std::path::{Path, PathBuf};

/// Modules that read build outputs but never produce a cached one: checks,
/// coverage and progress, the dashboard, matching searches, recovery aids,
/// formatting and the command dispatch. Everything else, including every
/// Psynergy source and the lockfile, is part of the implementation.
const OUTSIDE: &[&str] = &[
    "alchemy/src/check",
    "alchemy/src/check.rs",
    "alchemy/src/coverage",
    "alchemy/src/dashboard",
    "alchemy/src/matching",
    "alchemy/src/recovery",
    "alchemy/src/allocator.rs",
    "alchemy/src/build_full.rs",
    "alchemy/src/cross_edition.rs",
    "alchemy/src/flatten.rs",
    "alchemy/src/format.rs",
    "alchemy/src/http.rs",
    "alchemy/src/main.rs",
    "alchemy/src/scaffold.rs",
    "alchemy/src/siblings.rs",
];

fn collect(tools: &Path, path: &Path, files: &mut Vec<PathBuf>) {
    let relative = path.strip_prefix(tools).unwrap().to_string_lossy();
    if OUTSIDE.iter().any(|outside| relative == *outside) {
        return;
    }
    if path.is_dir() {
        for entry in std::fs::read_dir(path).unwrap() {
            collect(tools, &entry.unwrap().path(), files);
        }
    } else if path.extension().is_some_and(|extension| extension == "rs") {
        files.push(path.to_path_buf());
    }
}

fn main() {
    let tools = Path::new(env!("CARGO_MANIFEST_DIR")).parent().unwrap();
    let mut files = Vec::new();
    for directory in ["alchemy/src", "psynergy/src"] {
        let path = tools.join(directory);
        println!("cargo:rerun-if-changed={}", path.display());
        collect(tools, &path, &mut files);
    }
    for manifest in ["Cargo.lock", "alchemy/Cargo.toml", "psynergy/Cargo.toml"] {
        let path = tools.join(manifest);
        println!("cargo:rerun-if-changed={}", path.display());
        files.push(path);
    }
    files.sort();
    let mut digest = Sha256::new();
    for path in files {
        let relative = path
            .strip_prefix(tools)
            .unwrap()
            .to_string_lossy()
            .into_owned();
        digest.update(relative.as_bytes());
        digest.update([0]);
        digest.update(Sha256::digest(std::fs::read(&path).unwrap()));
    }
    let hex = digest
        .finalize()
        .iter()
        .map(|byte| format!("{byte:02x}"))
        .collect::<String>();
    println!("cargo:rustc-env=ALCHEMY_BUILD_IMPLEMENTATION={hex}");
}
