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
    let manifest = PathBuf::from(std::env::var_os("CARGO_MANIFEST_DIR").unwrap());
    let tools = manifest.parent().unwrap();
    let encoder = digest(tools, &["ags/src", "ags/Cargo.toml", "Cargo.lock"]);
    println!("cargo:rustc-env=ALCHEMY_ENCODER={encoder}");
    let rule = digest(tools, &["alchemy/src/build_rom.rs"]);
    println!("cargo:rustc-env=ALCHEMY_LINK_RULE={rule}");
    let header = digest(tools, &["alchemy/src/gbafix.rs"]);
    println!("cargo:rustc-env=ALCHEMY_HEADER_FIX={header}");
}
