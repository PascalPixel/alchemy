//! Differential parity harness: this crate against the live
//! `tools/lib/match_m2c.ts`.
//!
//! The rule this harness is built around is that SCANNING NOTHING IS NOT
//! PASSING. An empty corpus, a missing `bun`, an unreadable candidate, or a
//! category that quietly stopped producing cases is a HARD FAILURE, not a clean
//! sweep. Every floor below counts the TYPESCRIPT side specifically, so a
//! Rust-side mirror cannot carry a category over its floor on its own.
//!
//! Three categories are compared:
//!
//! * `constants` -- `M2C_PREAMBLE` and `ROM_BASE`, read out of the live module.
//! * `cli` -- the real script's argument parsing, compared on exit code and
//!   message.
//! * `verify` -- `verifyCandidate` end to end on real candidates from
//!   `work/matches/m2c`, compared on `size` and on both byte arrays.
//!
//! `main()` itself is NOT compared end to end, and that is a real limitation:
//! for the default target it writes into `out/`, which this port is forbidden
//! to touch, and it also rewrites the tracked `work/matches/m2c.json`. What
//! `main()` adds over `verifyCandidate` is draft selection, the eight
//! substitutions, and the scoring, all of which are covered by unit tests
//! rather than by differential comparison.

use std::path::{Path, PathBuf};
use std::process::Command;

use alchemy_routing::routing::{root, CompilerTarget};
use match_m2c::verify::{verify_candidate, CandidateCompilerConfiguration, M2C_PREAMBLE, ROM_BASE};

/// Per-category floors, counted on the TypeScript side.
const CONSTANTS_FLOOR: usize = 2;
const CLI_FLOOR: usize = 10;
const VERIFY_FLOOR: usize = 80;
/// Of those, how many must have reached the linker and produced real BYTES on
/// the TypeScript side. Without this, a toolchain that failed on every single
/// candidate would still clear `VERIFY_FLOOR` on matching error strings, and the
/// byte comparison -- the entire point -- would never have run.
const VERIFY_BYTES_FLOOR: usize = 25;
/// Total cases, and the number of DISTINCT payloads required so that a corpus
/// of identical inputs cannot pass for coverage.
const CASE_FLOOR: usize = 92;
const DISTINCT_FLOOR: usize = 50;

struct Case {
    category: &'static str,
    id: String,
    typescript: String,
    rust: String,
}

fn main() {
    match run() {
        Ok(report) => println!("{report}"),
        Err(error) => {
            eprintln!("drift: {error}");
            std::process::exit(1);
        }
    }
}

fn run() -> Result<String, String> {
    let repository_root = root();
    require_bun()?;
    let corpus = corpus(repository_root)?;
    let sample_size: usize = std::env::var("MATCH_M2C_DRIFT_SAMPLE")
        .ok()
        .and_then(|value| value.parse().ok())
        .unwrap_or(100);
    if sample_size < VERIFY_FLOOR {
        return Err(format!(
            "sample size {sample_size} is below the verify floor {VERIFY_FLOOR}"
        ));
    }
    let sample = spread(&corpus, sample_size);

    // NEGATIVE CONTROL FIRST. If a deliberate perturbation does not show up as
    // exactly one difference, the comparison is not wired to anything and no
    // later green result means a thing.
    let control = collect(&sample, true)?;
    let control_differences = differences(&control);
    if control_differences.len() != 1 {
        return Err(format!(
            "negative control produced {} differences, expected exactly 1 -- the comparison is not \
             connected to the payloads\n{}",
            control_differences.len(),
            render(&control_differences)
        ));
    }

    let cases = collect(&sample, false)?;
    check_floors(&cases)?;
    let found = differences(&cases);
    if !found.is_empty() {
        return Err(format!("{} difference(s):\n{}", found.len(), render(&found)));
    }

    let distinct = distinct_payloads(&cases);
    Ok(format!(
        "corpus={} sampled={} cases={} distinct_payloads={} constants={} cli={} verify={} \
         verify_with_bytes={} negative_control=1 differences=0",
        corpus.len(),
        sample.len(),
        cases.len(),
        distinct,
        count(&cases, "constants"),
        count(&cases, "cli"),
        count(&cases, "verify"),
        verify_with_bytes(&cases),
    ))
}

fn require_bun() -> Result<(), String> {
    let output = Command::new("bun")
        .arg("--version")
        .output()
        .map_err(|error| format!("bun is not runnable, so nothing can be compared: {error}"))?;
    if !output.status.success() {
        return Err("bun --version failed; refusing to report a clean sweep".to_string());
    }
    Ok(())
}

/// The real repository corpus: every candidate `.c` in `work/matches/m2c`.
fn corpus(repository_root: &Path) -> Result<Vec<PathBuf>, String> {
    let directory = repository_root.join("work/matches/m2c");
    let mut files: Vec<PathBuf> = std::fs::read_dir(&directory)
        .map_err(|error| format!("{}: {error}", directory.display()))?
        .map(|entry| entry.map(|entry| entry.path()))
        .collect::<Result<Vec<_>, _>>()
        .map_err(|error| error.to_string())?
        .into_iter()
        .filter(|path| path.extension().is_some_and(|ext| ext == "c"))
        .collect();
    files.sort();
    if files.is_empty() {
        return Err(format!(
            "no candidates under {} -- an empty corpus is a failure, not a pass",
            directory.display()
        ));
    }
    Ok(files)
}

/// Spread the sample across the whole corpus rather than taking a prefix, so
/// the cases are not all neighbours from one translation unit.
fn spread(corpus: &[PathBuf], wanted: usize) -> Vec<PathBuf> {
    if wanted >= corpus.len() {
        return corpus.to_vec();
    }
    (0..wanted)
        .map(|index| corpus[index * corpus.len() / wanted].clone())
        .collect()
}

fn collect(sample: &[PathBuf], perturb: bool) -> Result<Vec<Case>, String> {
    let mut cases = Vec::new();
    cases.extend(constant_cases()?);
    cases.extend(cli_cases()?);
    cases.extend(verify_cases(sample)?);
    if perturb {
        // Exactly one payload, on exactly one case.
        let case = cases
            .iter_mut()
            .find(|case| case.category == "verify")
            .ok_or_else(|| "no verify case to perturb".to_string())?;
        case.rust.push_str("!perturbed");
    }
    Ok(cases)
}

/// Read the two exported constants out of the live module.
fn constant_cases() -> Result<Vec<Case>, String> {
    let script = "const m = await import(process.env.MODULE);\
                  console.log(JSON.stringify([m.M2C_PREAMBLE, String(m.ROM_BASE)]));";
    let text = bun_eval(script)?;
    let values: Vec<String> = parse_string_array(&text)?;
    if values.len() != 2 {
        return Err("constants probe did not return two values".to_string());
    }
    Ok(vec![
        Case {
            category: "constants",
            id: "M2C_PREAMBLE".to_string(),
            typescript: values[0].clone(),
            rust: M2C_PREAMBLE.to_string(),
        },
        Case {
            category: "constants",
            id: "ROM_BASE".to_string(),
            typescript: values[1].clone(),
            rust: format!("{}", ROM_BASE as u64),
        },
    ])
}

/// Argument parsing, through the real script.
fn cli_cases() -> Result<Vec<Case>, String> {
    let argvs: Vec<Vec<&str>> = vec![
        vec!["-h"],
        vec!["--help"],
        vec![],
        vec!["only-one"],
        vec!["a", "b", "c"],
        vec!["--nope", "a", "b"],
        vec!["--target", "zz", "a", "b"],
        vec!["--jobs", "0", "a", "b"],
        vec!["--jobs", "x", "a", "b"],
        vec!["--jobs="],
    ];
    let script = root().join("tools/lib/match_m2c.ts");
    let mut cases = Vec::new();
    for argv in argvs {
        let output = Command::new("bun")
            .arg(script.to_string_lossy().as_ref())
            .args(&argv)
            .current_dir(root())
            .output()
            .map_err(|error| format!("bun {}: {error}", script.display()))?;
        let typescript = summarise_process(
            output.status.code(),
            &String::from_utf8_lossy(&output.stdout),
            &String::from_utf8_lossy(&output.stderr),
        );
        let owned: Vec<String> = argv.iter().map(|a| (*a).to_string()).collect();
        let rust = match match_m2c::parse_arguments(&owned) {
            Ok(match_m2c::ParseOutcome::Help) => {
                summarise_process(Some(0), &format!("{}\n", match_m2c::USAGE), "")
            }
            // A successful parse would go on to read the ROM and fail there,
            // which is not what this category is measuring; every argv above
            // is chosen to end in help or in a parse error.
            Ok(match_m2c::ParseOutcome::Options(_)) => "rc=0 parsed".to_string(),
            Err(error) => summarise_process(Some(1), "", &error),
        };
        cases.push(Case {
            category: "cli",
            id: format!("argv={owned:?}"),
            typescript,
            rust,
        });
    }
    Ok(cases)
}

/// `rc=<code> out=<first stdout line> err=<message>`.
///
/// Bun prints an uncaught throw as `ValueError: <message>` plus a stack, and
/// has no `error: ` prefix. The prose around the message is runtime detail, so
/// this compares the exit code and the message, not the framing.
fn summarise_process(code: Option<i32>, stdout: &str, stderr: &str) -> String {
    let first = |text: &str| {
        text.lines()
            .map(str::trim_end)
            .find(|line| !line.trim().is_empty())
            .unwrap_or("")
            .to_string()
    };
    // Bun prints a source code frame, then the message, then a stack. The
    // message is the line that starts with `error: `; taking the first non-empty
    // line would pick up the code frame instead.
    let message = stderr
        .lines()
        .find_map(|line| line.trim().strip_prefix("error: "))
        .map(str::to_string)
        .unwrap_or_else(|| first(stderr));
    format!(
        "rc={} out={} err={message}",
        code.map(|c| c.to_string()).unwrap_or_else(|| "signal".into()),
        first(stdout)
    )
}

/// `verifyCandidate` on real candidates, both sides, same output directory
/// layout but separate roots so neither can read the other's artefacts.
fn verify_cases(sample: &[PathBuf]) -> Result<Vec<Case>, String> {
    let fixtures = fixtures_directory()?;
    let typescript_out = fixtures.join("ts");
    let rust_out = fixtures.join("rs");
    std::fs::create_dir_all(&typescript_out).map_err(|e| e.to_string())?;
    std::fs::create_dir_all(&rust_out).map_err(|e| e.to_string())?;

    let target = decomp_targets::target_for(decomp_targets::DEFAULT_TARGET);
    let rom_path = root().join(target.rom);
    let rom = std::fs::read(&rom_path).map_err(|error| {
        format!(
            "{}: {error} -- without the ROM there is nothing to compare against, which is a \
             failure, not a pass",
            rom_path.display()
        )
    })?;

    let mut cases = Vec::new();
    for candidate in sample {
        let source = candidate.to_string_lossy().into_owned();
        let typescript = typescript_verify(&source, &rom_path, &typescript_out)?;
        let rust = match verify_candidate(
            &source,
            &rom,
            rust_out.to_string_lossy().as_ref(),
            &[],
            ROM_BASE,
            CompilerTarget::Gs1,
            &CandidateCompilerConfiguration::default(),
        ) {
            Ok(verification) => format!(
                "size={} actual={} expected={}",
                verification.size,
                hex(&verification.actual),
                hex(&verification.expected)
            ),
            Err(error) => format!("error={error}"),
        };
        cases.push(Case {
            category: "verify",
            id: candidate
                .file_name()
                .map(|n| n.to_string_lossy().into_owned())
                .unwrap_or_default(),
            typescript,
            rust,
        });
    }
    Ok(cases)
}

fn typescript_verify(source: &str, rom: &Path, out: &Path) -> Result<String, String> {
    let script = "const m = await import(process.env.MODULE);\
        const rom = new Uint8Array(await Bun.file(process.env.ROM).arrayBuffer());\
        const hex = (b) => Buffer.from(b).toString('hex');\
        try {\
          const v = await m.verifyCandidate(process.env.SOURCE, rom, process.env.OUT);\
          console.log(`size=${v.size} actual=${hex(v.actual)} expected=${hex(v.expected)}`);\
        } catch (error) { console.log(`error=${error.message}`); }";
    bun_eval_with(
        script,
        &[
            ("ROM", rom.to_string_lossy().into_owned()),
            ("SOURCE", source.to_string()),
            ("OUT", out.to_string_lossy().into_owned()),
        ],
    )
    .map(|text| text.trim_end_matches('\n').to_string())
}

fn bun_eval(script: &str) -> Result<String, String> {
    bun_eval_with(script, &[])
}

fn bun_eval_with(script: &str, environment: &[(&str, String)]) -> Result<String, String> {
    let module = root().join("tools/lib/match_m2c.ts");
    let mut command = Command::new("bun");
    command
        .arg("-e")
        .arg(script)
        .current_dir(root())
        .env("MODULE", module.to_string_lossy().as_ref());
    for (key, value) in environment {
        command.env(key, value);
    }
    let output = command
        .output()
        .map_err(|error| format!("bun -e: {error}"))?;
    if !output.status.success() {
        return Err(format!(
            "bun -e failed ({}): {}",
            output.status,
            String::from_utf8_lossy(&output.stderr).trim()
        ));
    }
    Ok(String::from_utf8_lossy(&output.stdout).into_owned())
}

/// A tiny reader for the one shape the constants probe emits: an array of
/// strings. Uses this crate's own parser, so it is exercised here too.
fn parse_string_array(text: &str) -> Result<Vec<String>, String> {
    let value = match_m2c::json::parse(text.trim())?;
    let match_m2c::json::Json::Array(items) = value else {
        return Err("expected an array".to_string());
    };
    items
        .into_iter()
        .map(|item| match item {
            match_m2c::json::Json::String(text) => Ok(text),
            other => Err(format!("expected a string, got {other:?}")),
        })
        .collect()
}

fn fixtures_directory() -> Result<PathBuf, String> {
    // Under this crate's own `target/`, never the shared scratchpad root.
    let directory = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/fixtures");
    std::fs::create_dir_all(&directory).map_err(|error| error.to_string())?;
    Ok(directory)
}

fn hex(bytes: &[u8]) -> String {
    let mut text = String::with_capacity(bytes.len() * 2);
    for byte in bytes {
        text.push_str(&format!("{byte:02x}"));
    }
    text
}

/// Truncated so that a byte-array mismatch does not bury the report.
fn render(cases: &[&Case]) -> String {
    let clip = |text: &str| -> String {
        if text.chars().count() > 160 {
            format!("{}...", text.chars().take(160).collect::<String>())
        } else {
            text.to_string()
        }
    };
    cases
        .iter()
        .map(|case| {
            format!(
                "  [{}] {}\n    ts:   {}\n    rust: {}\n",
                case.category,
                case.id,
                clip(&case.typescript),
                clip(&case.rust)
            )
        })
        .collect()
}

fn differences(cases: &[Case]) -> Vec<&Case> {
    cases
        .iter()
        .filter(|case| case.typescript != case.rust)
        .collect()
}

/// Counted on the TYPESCRIPT payload specifically, never the union.
fn verify_with_bytes(cases: &[Case]) -> usize {
    cases
        .iter()
        .filter(|case| case.category == "verify" && case.typescript.starts_with("size="))
        .count()
}

fn count(cases: &[Case], category: &str) -> usize {
    cases.iter().filter(|case| case.category == category).count()
}

/// DISTINCT payloads on the TYPESCRIPT side. Counting the union of both mirrors
/// lets a category clear its floor on Rust-side values alone, which is exactly
/// how a sibling gate here passed against an emptied source.
fn distinct_payloads(cases: &[Case]) -> usize {
    let mut seen: Vec<&str> = cases.iter().map(|case| case.typescript.as_str()).collect();
    seen.sort_unstable();
    seen.dedup();
    seen.len()
}

fn check_floors(cases: &[Case]) -> Result<(), String> {
    let floors = [
        ("constants", CONSTANTS_FLOOR),
        ("cli", CLI_FLOOR),
        ("verify", VERIFY_FLOOR),
    ];
    for (category, floor) in floors {
        let found = count(cases, category);
        if found < floor {
            return Err(format!(
                "category {category} produced {found} cases, below its floor of {floor} -- a \
                 pattern stopped matching"
            ));
        }
    }
    let with_bytes = verify_with_bytes(cases);
    if with_bytes < VERIFY_BYTES_FLOOR {
        return Err(format!(
            "only {with_bytes} verify cases produced real bytes on the TypeScript side, below the \
             floor of {VERIFY_BYTES_FLOOR} -- the toolchain is failing, which is not a pass"
        ));
    }
    if cases.len() < CASE_FLOOR {
        return Err(format!(
            "{} cases, below the floor of {CASE_FLOOR}",
            cases.len()
        ));
    }
    let distinct = distinct_payloads(cases);
    if distinct < DISTINCT_FLOOR {
        return Err(format!(
            "{distinct} distinct TypeScript payloads, below the floor of {DISTINCT_FLOOR} -- the \
             corpus is degenerate"
        ));
    }
    Ok(())
}
