//! Measured differential harness: this crate against the live TypeScript, over
//! every source the repository actually compiles.
//!
//! WHY. `sourceToAssemblyPlan` chooses the compiler binary, decides whether a
//! separate preprocessing pass runs, and fixes the argv of every step -- and gcc
//! is later-flag-wins, so argv order is machine code. The routing tables feeding
//! it carry thousands of entries with per-source overlays. Reading the two
//! implementations side by side cannot prove they agree; running both over the
//! whole corpus and byte-comparing the results can.
//!
//! Errors are compared, not skipped. A family whose bundle is not installed
//! throws on both sides, and the message is part of the record, so
//! throw-for-throw parity is measured alongside success parity. A difference in
//! *which* input throws is exactly the kind of drift that would otherwise hide.
//!
//! Guards, because a harness that measures nothing looks exactly like a harness
//! that passes:
//!   * a case floor -- fewer than [`MINIMUM_CASES`] comparisons is a failure;
//!   * an `ok` floor -- if [`MINIMUM_OK_RECORDS`] plans do not actually succeed,
//!     the run has only proved the two sides throw in the same places;
//!   * a negative control -- the comparison must reject a deliberately perturbed
//!     record, proving it can tell two outputs apart;
//!   * a distinct-value count -- so a reviewer can see the corpus is not
//!     degenerately all-same.
//!
//! Usage: `cargo run --release --bin plan-corpus` from this crate. Needs `bun`
//! on PATH.
//!
//! NOTHING IS WRITTEN TO DISK. The output paths in the records are strings that
//! never reach the filesystem, and they are rooted at this crate's own
//! `target/fixtures` so that stays true if someone later wires a real build up
//! to them.
//!
//! RETIREMENT: delete this file, `harness/typescript_probe.ts` and the
//! `plan-corpus` `[[bin]]` entry when `tools/lib/alchemy_gcc.ts` is deleted.

use std::io::Write;
use std::path::{Path, PathBuf};
use std::process::{Command, Stdio};

use alchemy_plan::plan::{
    direct_compiler_command, direct_compiler_command_for_source, direct_preprocessor_command,
    source_to_assembly_plan, CompilerFamily, CompilerFlagMutations, SourceToAssemblyPlan,
    SourceToAssemblyPlanOptions,
};
use alchemy_routing::routing::CompilerTarget;

/// Below this the run is not evidence. The tree carries several thousand
/// sources and each contributes sixteen records; if the corpus collapses, that
/// is a broken harness, not a pass.
const MINIMUM_CASES: usize = 20_000;

/// Below this the run has only proved the two sides fail identically. At least
/// the gcc296 and routed plans must succeed on a machine with the bundle
/// installed, which is two records per source.
const MINIMUM_OK_RECORDS: usize = 5_000;

/// Directories holding the sources the repository compiles.
const CORPUS_ROOTS: [&str; 2] = ["exact", "semantic"];

/// Separates argv items inside a record.
const UNIT: char = '\u{1f}';
/// Separates the sections of a serialised plan.
const SECTION: char = '\u{1e}';

/// This crate's own scratch root. Never written to; see the module note.
const FIXTURES: &str = "tools-rs/alchemy-plan/target/fixtures";

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

/// Serialise a plan exactly as the probe's `payload` does.
fn payload(plan: &SourceToAssemblyPlan) -> String {
    let steps = plan
        .steps
        .iter()
        .map(|step| {
            let mut parts = vec![step.kind.as_str().to_string()];
            parts.extend(step.command.iter().cloned());
            parts.join(&UNIT.to_string())
        })
        .collect::<Vec<_>>()
        .join(&SECTION.to_string());
    [
        plan.target.as_str().to_string(),
        plan.requested_family.as_str().to_string(),
        plan.family.as_str().to_string(),
        plan.routing_source.clone(),
        plan.input.clone(),
        plan.output.clone(),
        plan.compiler_input.clone(),
        plan.flags.join(&UNIT.to_string()),
        steps,
    ]
    .join(&SECTION.to_string())
}

/// `ok\t<value>` or `err\t<message>`, matching the probe's `attempt`.
fn attempt(result: Result<String, String>) -> String {
    match result {
        Ok(value) => format!("ok\t{value}"),
        Err(message) => format!("err\t{message}"),
    }
}

/// The Rust side, in the probe's record format and record order.
fn rust_records(corpus: &[String]) -> Vec<String> {
    let separator = UNIT.to_string();
    let mut out = Vec::with_capacity(corpus.len() * 16);
    for source in corpus {
        let input = source.clone();
        let output = format!("{FIXTURES}/{source}.s");

        for target in [CompilerTarget::Gs1, CompilerTarget::Gs2] {
            for family in CompilerFamily::ALL {
                let mut options =
                    SourceToAssemblyPlanOptions::new(target, source, &input, &output);
                options.family = Some(family);
                let record = attempt(
                    source_to_assembly_plan(&options).map(|plan| payload(&plan)),
                );
                out.push(format!(
                    "PLAN\t{source}\t{}\t{}\t{record}",
                    target.as_str(),
                    family.as_str()
                ));
            }
        }

        let mut options =
            SourceToAssemblyPlanOptions::new(CompilerTarget::Gs1, source, &input, &output);
        options.flags = Some(CompilerFlagMutations {
            add_flags: vec!["-fno-inline".to_string()],
            remove_flags: vec!["-O2".to_string()],
        });
        options.preprocessed_output = Some(format!("{FIXTURES}/{source}.i"));
        options.dumpbase = Some("explicit.c".to_string());
        out.push(format!(
            "MUT\t{source}\t{}",
            attempt(source_to_assembly_plan(&options).map(|plan| payload(&plan)))
        ));

        out.push(format!(
            "DPP\t{source}\t{}",
            attempt(
                direct_preprocessor_command(&input, &format!("{FIXTURES}/{source}.i"))
                    .map(|argv| argv.join(&separator))
            )
        ));
        out.push(format!(
            "DCC\t{source}\t{}",
            attempt(
                direct_compiler_command(&input, &output, "explicit.c", Some(source))
                    .map(|argv| argv.join(&separator))
            )
        ));
        out.push(format!(
            "DCCS\t{source}\t{}",
            attempt(
                direct_compiler_command_for_source(source, &input, &output, "explicit.c")
                    .map(|argv| argv.join(&separator))
            )
        ));
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

/// How many records reported success rather than a reproduced throw.
fn successes(records: &[String]) -> usize {
    records
        .iter()
        .filter(|record| record.contains("\tok\t"))
        .count()
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
    let ok_records = successes(&rust);

    println!("corpus sources          {}", corpus.len());
    println!("cases compared          {cases}");
    println!("differences             {}", differences.len());
    println!("successful plans        {ok_records}");
    println!("reproduced throws       {}", rust.len() - ok_records);
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
    if ok_records < MINIMUM_OK_RECORDS {
        eprintln!(
            "ONLY {ok_records} PLANS SUCCEEDED, EXPECTED AT LEAST {MINIMUM_OK_RECORDS} -- the \
             run has proved only that both sides throw in the same places, which is not the \
             property under test."
        );
        std::process::exit(1);
    }
    if !differences.is_empty() {
        eprintln!(
            "{} DIFFERENCES between tools/lib/alchemy_gcc.ts and tools-rs/alchemy-plan.",
            differences.len()
        );
        std::process::exit(1);
    }
    println!("PARITY: {cases} cases, 0 differences.");
}
