//! Measured differential harness: this crate against the live TypeScript, over
//! every source the repository actually compiles.
//!
//! WHY. `compilerBundleSignature` is cache-key material in six TypeScript call
//! sites. One differing byte either silently invalidates every cache entry or,
//! worse, wrongly validates one -- a stale object accepted as fresh. And
//! `compilerCommandForTargetSource` chooses the compiler binary and the flag
//! order, where gcc is later-flag-wins. Neither can be checked by reading the
//! code. This runs both implementations over the real corpus and byte-compares
//! every result.
//!
//! Guards, because a harness that measures nothing looks exactly like a harness
//! that passes:
//!   * a case floor -- fewer than [`MINIMUM_CASES`] comparisons is a failure;
//!   * a negative control -- the comparison must reject a deliberately
//!     perturbed record, proving it can tell two outputs apart;
//!   * a distinct-value count -- so a reviewer can see the corpus is not
//!     degenerately all-same.
//!
//! Usage: `cargo run --bin bundle-corpus` from this crate. Needs `bun` on PATH.
//!
//! RETIREMENT: delete this file, `harness/typescript_probe.ts` and the
//! `bundle-corpus` `[[bin]]` entry when `tools/lib/alchemy_gcc.ts` is deleted.

use std::io::Write;
use std::path::{Path, PathBuf};
use std::process::{Command, Stdio};

use alchemy_bundle::bundle;
use alchemy_routing::routing::{cflags_for_target_source, CompilerTarget};

/// Below this the run is not evidence. The tree carries several thousand
/// sources; if the corpus collapses, that is a broken harness, not a pass.
const MINIMUM_CASES: usize = 2_000;

/// Directories holding the sources the repository compiles.
const CORPUS_ROOTS: [&str; 2] = ["exact", "semantic"];

const UNIT: char = '\u{1f}';

fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .expect("crate dir has a parent")
        .parent()
        .expect("tools-rs has a parent")
        .to_path_buf()
}

/// Every `.c` file under the corpus roots, as repository-relative paths, sorted
/// so both sides walk them in the same order.
///
/// PORT NOTE: sorted by bytes. JavaScript sorts strings by UTF-16 code unit,
/// which differs above U+FFFF -- but the order here only has to be identical
/// between the two sides, and the Rust side dictates it by writing the corpus
/// to the probe's stdin.
fn corpus() -> Vec<String> {
    fn walk(dir: &Path, root: &Path, out: &mut Vec<String>) {
        let Ok(entries) = std::fs::read_dir(dir) else {
            return;
        };
        let mut paths: Vec<PathBuf> = entries.flatten().map(|entry| entry.path()).collect();
        paths.sort();
        for path in paths {
            if path.is_dir() {
                walk(&path, root, out);
            } else if path.extension().is_some_and(|ext| ext == "c") {
                if let Ok(relative) = path.strip_prefix(root) {
                    out.push(relative.to_string_lossy().into_owned());
                }
            }
        }
    }
    let root = root();
    let mut out = Vec::new();
    for name in CORPUS_ROOTS {
        walk(&root.join(name), &root, &mut out);
    }
    out.sort();
    out
}

/// The Rust side, in the probe's record format.
fn rust_records(corpus: &[String]) -> Vec<String> {
    let mut out = Vec::with_capacity(corpus.len() * 2 + 1);
    out.push(format!("SIG\t{}", bundle::compiler_bundle_signature()));
    for source in corpus {
        for target in [CompilerTarget::Gs1, CompilerTarget::Gs2] {
            let mut flags = cflags_for_target_source(target, source);
            flags.extend(
                ["-S", "-o", "-", source.as_str()]
                    .iter()
                    .map(|flag| (*flag).to_string()),
            );
            let argv = match bundle::compiler_command_for_target_source(target, source, &flags) {
                Ok(argv) => argv,
                Err(error) => {
                    eprintln!("compiler_command_for_target_source failed for {source}: {error}");
                    std::process::exit(1);
                }
            };
            out.push(format!(
                "CMD\t{source}\t{}\t{}",
                target.as_str(),
                argv.join(&UNIT.to_string())
            ));
        }
    }
    out
}

/// Run the TypeScript probe, feeding it the same corpus.
fn typescript_records(corpus: &[String]) -> Result<Vec<String>, String> {
    let probe = Path::new(env!("CARGO_MANIFEST_DIR")).join("harness/typescript_probe.ts");
    let mut child = Command::new("bun")
        .arg("run")
        .arg(&probe)
        .current_dir(root())
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .stderr(Stdio::inherit())
        .spawn()
        .map_err(|error| format!("could not start bun for {}: {error}", probe.display()))?;
    {
        let stdin = child.stdin.as_mut().expect("stdin piped");
        stdin
            .write_all(corpus.join("\n").as_bytes())
            .map_err(|error| format!("writing corpus to the probe: {error}"))?;
    }
    let output = child
        .wait_with_output()
        .map_err(|error| format!("waiting for the probe: {error}"))?;
    if !output.status.success() {
        return Err(format!("the TypeScript probe exited {}", output.status));
    }
    // PORT NOTE: lossy, matching `readFileSync(..., "utf8")`, and the trailing
    // newline is dropped rather than becoming an empty final record.
    Ok(String::from_utf8_lossy(&output.stdout)
        .lines()
        .map(|line| line.to_string())
        .collect())
}

fn distinct(records: &[String]) -> usize {
    let mut sorted: Vec<&str> = records.iter().map(|record| record.as_str()).collect();
    sorted.sort_unstable();
    sorted.dedup();
    sorted.len()
}

/// Byte-compare two record streams. Returns the differing indices.
fn diff(a: &[String], b: &[String]) -> Vec<usize> {
    let mut out = Vec::new();
    for index in 0..a.len().max(b.len()) {
        if a.get(index) != b.get(index) {
            out.push(index);
        }
    }
    out
}

fn main() {
    let corpus = corpus();
    let rust = rust_records(&corpus);

    // NEGATIVE CONTROL, run before anything else so a comparison that cannot
    // distinguish two different streams fails immediately rather than reporting
    // a clean run.
    let mut perturbed = rust.clone();
    let victim = perturbed.len() / 2;
    perturbed[victim].push('X');
    let control = diff(&rust, &perturbed);
    if control != vec![victim] {
        eprintln!(
            "NEGATIVE CONTROL FAILED: perturbing record {victim} produced {} differences, \
             expected exactly 1. The comparison cannot tell two outputs apart, so a clean \
             result from it would mean nothing.",
            control.len()
        );
        std::process::exit(1);
    }

    let typescript = match typescript_records(&corpus) {
        Ok(records) => records,
        Err(error) => {
            eprintln!("{error}");
            std::process::exit(1);
        }
    };

    let differences = diff(&rust, &typescript);
    let cases = rust.len().max(typescript.len());

    println!("corpus sources          {}", corpus.len());
    println!("cases compared          {cases}");
    println!("differences             {}", differences.len());
    println!("distinct rust records   {}", distinct(&rust));
    println!("distinct ts records     {}", distinct(&typescript));
    println!(
        "negative control        ok (1 perturbation, 1 difference detected at record {victim})"
    );

    for index in differences.iter().take(10) {
        println!("  [{index}]");
        println!("    ts   {:?}", typescript.get(*index));
        println!("    rust {:?}", rust.get(*index));
    }

    if cases < MINIMUM_CASES {
        eprintln!(
            "ONLY {cases} CASES COMPARED, EXPECTED AT LEAST {MINIMUM_CASES} -- this is a \
             FAILURE, not a pass. Scanning nothing is not passing."
        );
        std::process::exit(1);
    }
    if !differences.is_empty() {
        eprintln!(
            "{} DIFFERENCES between tools/lib/alchemy_gcc.ts and tools-rs/alchemy-bundle.",
            differences.len()
        );
        std::process::exit(1);
    }
    println!("PARITY: {cases} cases, 0 differences.");
}
