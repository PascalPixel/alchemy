//! CLI for `semantic-superseded`, a port of `tools/semantic/semantic_superseded.ts`.
//!
//!   semantic-superseded             # print the paths, one per line
//!   semantic-superseded --check     # exit 1 if any exist
//!   semantic-superseded --self-test

use std::fs;
use std::path::{Path, PathBuf};
use std::process::ExitCode;

use semantic_superseded::{canonical_c_source, superseded_sources};

fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap()
        .to_path_buf()
}

/// The TypeScript reaches disk through `existsSync` guards followed by an
/// unguarded `readdirSync` / `readFileSync`. A path that exists but cannot be
/// read (EACCES, EISDIR) therefore throws an uncaught Node exception and Bun
/// exits nonzero. Swallowing that into `false` would be a silent divergence,
/// so this aborts too.
///
/// PORT NOTE: the failure *prose* cannot match. An uncaught Node `readFileSync`
/// error prints a Node stack trace with no `error:` line; this prints one
/// diagnostic. Only the fact of failure and the nonzero exit are parity.
fn abort(what: &str, path: &Path, error: &std::io::Error) -> ! {
    eprintln!("semantic-superseded: cannot read {what} {}: {error}", path.display());
    std::process::exit(1);
}

fn from_disk() -> Vec<String> {
    let root = root();
    let list_root = root.clone();
    superseded_sources(
        &mut |directory| {
            let path = list_root.join(directory);
            // `existsSync(path) ? readdirSync(path) : []`
            if !path.exists() {
                return Vec::new();
            }
            match fs::read_dir(&path) {
                Ok(entries) => entries
                    .map(|entry| match entry {
                        Ok(entry) => entry.file_name().to_string_lossy().into_owned(),
                        Err(error) => abort("directory entry in", &path, &error),
                    })
                    .collect(),
                Err(error) => abort("directory", &path, &error),
            }
        },
        &mut |path| {
            let exact = root.join(path);
            // `existsSync(exact) && canonicalCSource(readFileSync(exact, "utf8"))`
            if !exact.exists() {
                return false;
            }
            // PORT NOTE: `readFileSync(..., "utf8")` replaces invalid bytes
            // with U+FFFD rather than throwing, which is what `from_utf8_lossy`
            // does, so the four patterns see the same text either way.
            match fs::read(&exact) {
                Ok(bytes) => canonical_c_source(&String::from_utf8_lossy(&bytes)),
                Err(error) => abort("source", &exact, &error),
            }
        },
    )
}

fn self_test() {
    let listing = |directory: &str| -> Vec<String> {
        if directory == "semantic" {
            [
                "resource_3aa_c_02000230.c",
                "resource_3aa_c_02000400.c",
                "notes.md",
                "08006d50.c",
            ]
            .iter()
            .map(|s| s.to_string())
            .collect()
        } else {
            Vec::new()
        }
    };
    let found = superseded_sources(&mut { listing }, &mut |path: &str| {
        path == "exact/resource_3aa_c_02000230.c" || path == "exact/08006d50.c"
    });
    let expected = ["semantic/08006d50.c", "semantic/resource_3aa_c_02000230.c"];
    if found != expected {
        panic!("superseded self-test failed: {found:?}");
    }
    // A semantic source with no exact counterpart is not superseded, and a
    // non-source file in the directory is never reported.
    if !superseded_sources(&mut { listing }, &mut |_| false).is_empty() {
        panic!("superseded self-test reported a source with no exact counterpart");
    }
    println!("self-test=ok");
}

fn main() -> ExitCode {
    let argv: Vec<String> = std::env::args().skip(1).collect();
    if argv.iter().any(|arg| arg == "--self-test") {
        self_test();
        return ExitCode::SUCCESS;
    }
    let found = from_disk();
    for path in &found {
        println!("{path}");
    }
    if argv.iter().any(|arg| arg == "--check") {
        println!("superseded={}", found.len());
        if !found.is_empty() {
            return ExitCode::from(1);
        }
    }
    ExitCode::SUCCESS
}
