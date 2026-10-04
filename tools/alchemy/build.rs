//! Digests of the tool sources each cached step depends on, as pret's
//! Makefile rebuilds only what a changed tool produces:
//! `ALCHEMY_ENCODER` is the ags crate, which encodes every asset and code
//! overlay; `ALCHEMY_LINK_RULE` is the build's own overlay rule in
//! build_rom.rs. Compiling, assembling and linking key on the toolchain's
//! executables and their commands instead. The byte-identical ROM comparison
//! stays the proof; the digests decide only what may be reused.
use sha2::{Digest, Sha256};
use std::path::{Path, PathBuf};

fn collect(path: &Path, files: &mut Vec<PathBuf>) {
    if path.is_dir() {
        for entry in std::fs::read_dir(path).unwrap() {
            collect(&entry.unwrap().path(), files);
        }
    } else if path.extension().is_some_and(|extension| extension == "rs") {
        files.push(path.to_path_buf());
    }
}

/// One digest over `paths` (directories read recursively), named from `tools`.
fn digest(tools: &Path, paths: &[&str]) -> String {
    let mut files = Vec::new();
    for path in paths {
        let path = tools.join(path);
        println!("cargo:rerun-if-changed={}", path.display());
        if path.is_dir() {
            collect(&path, &mut files);
        } else {
            files.push(path);
        }
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
        digest.update(Sha256::digest(std::fs::read(&path).unwrap_or_default()));
    }
    digest
        .finalize()
        .iter()
        .map(|byte| format!("{byte:02x}"))
        .collect()
}

pub(crate) fn main() {
    // Cargo can reuse this executable from a copied worktree cache.
    // Its runtime manifest directory names the checkout being built now.
    let manifest = PathBuf::from(std::env::var_os("CARGO_MANIFEST_DIR").unwrap());
    let tools = manifest.parent().unwrap();
    let encoder = digest(tools, &["ags/src", "ags/Cargo.toml", "Cargo.lock"]);
    println!("cargo:rustc-env=ALCHEMY_ENCODER={encoder}");
    let rule = digest(tools, &["alchemy/src/build_rom.rs"]);
    println!("cargo:rustc-env=ALCHEMY_LINK_RULE={rule}");
}
