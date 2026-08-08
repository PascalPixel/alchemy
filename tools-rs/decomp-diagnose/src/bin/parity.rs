// Differential parity harness for the decomp_diagnose port.
//
// WHY THIS FILE EXISTS
// Unit tests pin the semantics this port's author thought about. They cannot
// catch the semantics nobody thought about. This binary runs the live
// TypeScript (`bun tools/decomp/decomp_diagnose.ts`) and the Rust binary over
// the real candidate corpus, one process each, and asserts that every
// observation agrees: exit code, stdout, and the substantive `error:` line.
//
// Three properties make it a gate rather than a ritual:
//
//   * A NEGATIVE CONTROL runs BEFORE the real comparison. It hands the Rust
//     side a different ROM for exactly one case and requires the comparator to
//     report exactly one difference, at exactly that index. A harness that
//     cannot see a planted difference cannot see a real one.
//   * FLOORS count the TypeScript side specifically, never the union. In
//     particular `emitted` (TypeScript exit 0 with a JSON document) is floored
//     on its own, because four corpus candidates FAIL TO COMPILE on both sides
//     and a bare case floor would be cleared entirely by matching failures
//     while the interesting code never ran.
//   * SCANNING NOTHING IS NOT PASSING. An empty corpus, a missing `bun`, a
//     missing `arm-none-eabi-objdump`, or an unreadable ROM are hard failures,
//     each reachable through a `PARITY_FORCE_*` knob so the hard-fail paths can
//     be triggered and proven rather than merely asserted.
//
// A WARM SECOND PASS re-runs the whole comparison. `verifyCandidate` caches
// compiles under `out/cache`, so a cold-only harness never exercises the
// cache-hit path; the warm pass re-compares and requires identical
// observations, catching a cache that serves a different answer than the
// compile did.

use std::collections::BTreeSet;
use std::path::{Path, PathBuf};
use std::process::Command;
use std::time::{Duration, Instant};

use decomp_diagnose::repo_root;

// Floors, all counting the TypeScript side. Set below the values observed on
// the 2026-08-07 corpus (24 cases / 20 emitted / 20 distinct / 3 dominants /
// 4 failures) with headroom, so corpus churn does not flap the gate but an
// emptied or short-circuited corpus does.
const FLOOR_CASES: usize = 20;
const FLOOR_EMITTED: usize = 18;
const FLOOR_DISTINCT_PAYLOADS: usize = 15;
const FLOOR_FAILURES: usize = 2;
const FLOOR_DOMINANT_CATEGORIES: usize = 3;
const FLOOR_NONZERO_COUNT_CATEGORIES: usize = 4;

/// One side's externally visible behaviour for one case, after normalisation.
#[derive(Clone, PartialEq, Eq, Debug)]
struct Observation {
    code: i32,
    stdout: String,
    /// The first `error: ...` line from stderr, or the empty string.
    ///
    /// PORT NOTE: Bun prints the offending source line, a JS stack trace and a
    /// version banner around that line; this port prints the line alone. The
    /// contract asserted here is same-failure (exit code, offending path,
    /// stdout), not same-prose, exactly as the porting rules require.
    error: String,
}

/// Removes the two things that legitimately vary between runs and machines:
/// the absolute repository root, and any `0x`-suffixed temporary directory
/// name. Everything else, including every number the tool emits, is compared
/// verbatim.
fn normalize(text: &str, root: &str) -> String {
    let mut out = text.replace(root, "<ROOT>");
    // mkdtemp-style suffixes: a dot, then eight or more hex digits, at a path
    // component boundary. Rewritten to a stable token.
    out = rewrite_temp_suffixes(&out);
    out.trim_end().to_string()
}

fn rewrite_temp_suffixes(text: &str) -> String {
    let chars: Vec<char> = text.chars().collect();
    let mut out = String::new();
    let mut index = 0usize;
    while index < chars.len() {
        if chars[index] == '.' {
            let mut end = index + 1;
            while end < chars.len() && chars[end].is_ascii_hexdigit() {
                end += 1;
            }
            let digits = end - index - 1;
            let boundary = end == chars.len() || !chars[end].is_ascii_alphanumeric();
            if digits >= 8 && boundary {
                out.push_str(".<tmp>");
                index = end;
                continue;
            }
        }
        out.push(chars[index]);
        index += 1;
    }
    out
}

/// Keeps the first `error: ` line and drops Bun's stack frames, source echo and
/// version banner, none of which the Rust binary has or should have.
fn first_error_line(stderr: &str) -> String {
    stderr
        .lines()
        .find(|line| line.trim_start().starts_with("error: "))
        .map(|line| line.trim().to_string())
        .unwrap_or_default()
}

fn observe(command: &mut Command, root: &str) -> Result<Observation, String> {
    let output = command.output().map_err(|error| error.to_string())?;
    Ok(Observation {
        code: output.status.code().unwrap_or(-1),
        stdout: normalize(&String::from_utf8_lossy(&output.stdout), root),
        error: normalize(&first_error_line(&String::from_utf8_lossy(&output.stderr)), root),
    })
}

fn on_path(program: &str) -> bool {
    Command::new(program)
        .arg("--version")
        .output()
        .map(|output| output.status.code().is_some())
        .unwrap_or(false)
}

/// One canonical path per distinct candidate basename, in sorted order. The
/// corpus stores the same candidate under many scratch directories; comparing
/// each copy would inflate the case count without adding a single distinct
/// payload, and floors that can be cleared by duplication are not floors.
fn collect_corpus(root: &Path) -> Vec<PathBuf> {
    let mut found: Vec<(String, PathBuf)> = Vec::new();
    let mut stack = vec![root.join("out")];
    while let Some(directory) = stack.pop() {
        let Ok(entries) = std::fs::read_dir(&directory) else {
            continue;
        };
        for entry in entries.flatten() {
            let path = entry.path();
            if path.is_dir() {
                stack.push(path);
                continue;
            }
            let Some(name) = path.file_name().and_then(|name| name.to_str()) else {
                continue;
            };
            let Some(stem) = name.strip_suffix(".c") else {
                continue;
            };
            if stem.len() != 8 || !stem.chars().all(|item| item.is_ascii_hexdigit()) {
                continue;
            }
            found.push((name.to_string(), path));
        }
    }
    found.sort();
    found.dedup_by(|left, right| left.0 == right.0);
    found.into_iter().map(|(_, path)| path).collect()
}

struct Pass {
    observations: Vec<(Observation, Observation)>,
    elapsed: Duration,
}

fn run_pass(
    corpus: &[PathBuf],
    root: &Path,
    root_text: &str,
    binary: &Path,
    rom: &Path,
    perturb: Option<usize>,
    perturb_rom: &Path,
) -> Result<Pass, String> {
    let started = Instant::now();
    let mut observations = Vec::new();
    for (index, candidate) in corpus.iter().enumerate() {
        let typescript = observe(
            Command::new("bun")
                .current_dir(root)
                .arg("tools/decomp/decomp_diagnose.ts")
                .arg(candidate)
                .arg(rom),
            root_text,
        )?;
        // The negative control genuinely changes the Rust side's input for one
        // case; it does not doctor the comparison after the fact.
        let rust_rom = if perturb == Some(index) { perturb_rom } else { rom };
        let rust = observe(
            Command::new(binary)
                .current_dir(root)
                .arg(candidate)
                .arg(rust_rom),
            root_text,
        )?;
        observations.push((typescript, rust));
    }
    Ok(Pass {
        observations,
        elapsed: started.elapsed(),
    })
}

fn differing(pass: &Pass) -> Vec<usize> {
    pass.observations
        .iter()
        .enumerate()
        .filter(|(_, (typescript, rust))| typescript != rust)
        .map(|(index, _)| index)
        .collect()
}

/// Pulls `"dominant": "x"` out of an emitted document without a JSON parse, so
/// a malformed document reports as absent rather than aborting the harness.
fn field(document: &str, key: &str) -> Option<String> {
    let needle = format!("\"{key}\": \"");
    let start = document.find(&needle)? + needle.len();
    let rest = &document[start..];
    let end = rest.find('"')?;
    Some(rest[..end].to_string())
}

fn nonzero_count_categories(document: &str) -> Vec<String> {
    let mut names = Vec::new();
    for kind in decomp_diagnose::KINDS {
        let needle = format!("\"{}\": ", kind.name());
        if let Some(start) = document.find(&needle) {
            let rest = &document[start + needle.len()..];
            let digits: String = rest.chars().take_while(|item| item.is_ascii_digit()).collect();
            if digits.parse::<i64>().unwrap_or(0) > 0 {
                names.push(kind.name().to_string());
            }
        }
    }
    names
}

fn main() {
    if let Err(message) = run() {
        eprintln!("parity harness FAILED: {message}");
        std::process::exit(1);
    }
}

fn run() -> Result<(), String> {
    let root = repo_root();
    let root_text = root.display().to_string();

    // Preconditions. Each is a hard failure, and each has a force knob so the
    // failure path can be triggered on demand instead of taken on faith.
    if std::env::var("PARITY_FORCE_NO_BUN").is_ok() || !on_path("bun") {
        return Err("bun is not available; scanning nothing is not passing".to_string());
    }
    if std::env::var("PARITY_FORCE_NO_OBJDUMP").is_ok() || !on_path("arm-none-eabi-objdump") {
        return Err(
            "arm-none-eabi-objdump is not available; scanning nothing is not passing".to_string(),
        );
    }

    let rom = std::env::var("PARITY_FORCE_BAD_ROM")
        .map(PathBuf::from)
        .unwrap_or_else(|_| root.join("roms").join("gs1-en.gba"));
    let rom_bytes = std::fs::read(&rom)
        .map_err(|error| format!("ROM {} is unreadable: {error}", rom.display()))?;
    if rom_bytes.is_empty() {
        return Err(format!("ROM {} is empty", rom.display()));
    }
    let perturb_rom = root.join("roms").join("gs1-de.gba");
    if !perturb_rom.exists() {
        return Err(format!(
            "negative-control ROM {} is missing",
            perturb_rom.display()
        ));
    }

    let binary = root
        .join("tools-rs/decomp-diagnose/target/release/decomp-diagnose")
        .canonicalize()
        .map_err(|error| format!("release binary missing (cargo build --release): {error}"))?;

    let corpus = if std::env::var("PARITY_FORCE_EMPTY_CORPUS").is_ok() {
        Vec::new()
    } else {
        collect_corpus(&root)
    };
    if corpus.is_empty() {
        return Err("candidate corpus is empty; scanning nothing is not passing".to_string());
    }
    if corpus.len() < FLOOR_CASES {
        return Err(format!(
            "case floor: {} candidates < {FLOOR_CASES}",
            corpus.len()
        ));
    }

    // NEGATIVE CONTROL, before the real comparison. Perturb exactly one case's
    // Rust-side ROM and require exactly one reported difference, at that index.
    let control_index = 4.min(corpus.len() - 1);
    let control = run_pass(
        &corpus,
        &root,
        &root_text,
        &binary,
        &rom,
        Some(control_index),
        &perturb_rom,
    )?;
    let control_differences = differing(&control);
    if control_differences != vec![control_index] {
        return Err(format!(
            "negative control did not yield exactly one difference at index {control_index}: got {control_differences:?}. \
             The comparator cannot see a planted difference, so it cannot be trusted to see a real one."
        ));
    }
    println!(
        "negative control: 1 planted difference at case {} ({}), detected exactly",
        control_index,
        corpus[control_index].display()
    );

    // Cold pass, then warm pass over the same corpus.
    let cold = run_pass(&corpus, &root, &root_text, &binary, &rom, None, &perturb_rom)?;
    let cold_differences = differing(&cold);
    if !cold_differences.is_empty() {
        for index in &cold_differences {
            let (typescript, rust) = &cold.observations[*index];
            eprintln!(
                "case {} {}\n  typescript: {typescript:?}\n  rust:       {rust:?}",
                index,
                corpus[*index].display()
            );
        }
        return Err(format!("{} case(s) differ", cold_differences.len()));
    }

    let warm = run_pass(&corpus, &root, &root_text, &binary, &rom, None, &perturb_rom)?;
    if !differing(&warm).is_empty() {
        return Err("warm pass differs; a cache is serving a different answer".to_string());
    }
    if warm.observations != cold.observations {
        return Err(
            "warm pass observations differ from the cold pass; the compile cache is not \
             transparent"
                .to_string(),
        );
    }

    // Floors, counted on the TYPESCRIPT side only.
    let typescript: Vec<&Observation> = cold
        .observations
        .iter()
        .map(|(typescript, _)| typescript)
        .collect();
    let emitted: Vec<&Observation> = typescript
        .iter()
        .copied()
        .filter(|item| item.code == 0 && item.stdout.starts_with('{'))
        .collect();
    let failures = typescript.iter().filter(|item| item.code != 0).count();
    let distinct: BTreeSet<&str> = emitted.iter().map(|item| item.stdout.as_str()).collect();
    let dominants: BTreeSet<String> = emitted
        .iter()
        .filter_map(|item| field(&item.stdout, "dominant"))
        .collect();
    let categories: BTreeSet<String> = emitted
        .iter()
        .flat_map(|item| nonzero_count_categories(&item.stdout))
        .collect();

    println!(
        "cases={} emitted={} failures={} distinct_payloads={} dominants={:?} nonzero_categories={:?}",
        typescript.len(),
        emitted.len(),
        failures,
        distinct.len(),
        dominants,
        categories
    );

    // THE SUCCESSFUL-PATH FLOOR. Four corpus candidates reference overlay
    // sources that no longer exist and fail to compile on BOTH sides; their
    // agreement is real but it exercises only the error path. `emitted` floors
    // the path where the tool actually disassembles, aligns and classifies.
    let mut violations = Vec::new();
    if emitted.len() < FLOOR_EMITTED {
        violations.push(format!(
            "successful-path floor: {} emitted documents < {FLOOR_EMITTED}",
            emitted.len()
        ));
    }
    if distinct.len() < FLOOR_DISTINCT_PAYLOADS {
        violations.push(format!(
            "distinct-payload floor: {} < {FLOOR_DISTINCT_PAYLOADS}",
            distinct.len()
        ));
    }
    if failures < FLOOR_FAILURES {
        violations.push(format!(
            "failure-path floor: {failures} < {FLOOR_FAILURES}"
        ));
    }
    if dominants.len() < FLOOR_DOMINANT_CATEGORIES {
        violations.push(format!(
            "dominant-category floor: {} < {FLOOR_DOMINANT_CATEGORIES}",
            dominants.len()
        ));
    }
    if categories.len() < FLOOR_NONZERO_COUNT_CATEGORIES {
        violations.push(format!(
            "mismatch-category floor: {} < {FLOOR_NONZERO_COUNT_CATEGORIES}",
            categories.len()
        ));
    }
    if !violations.is_empty() {
        return Err(violations.join("; "));
    }

    println!(
        "cold pass {:.2}s, warm pass {:.2}s, all {} cases identical",
        cold.elapsed.as_secs_f64(),
        warm.elapsed.as_secs_f64(),
        typescript.len()
    );
    println!("parity harness PASSED");
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    const ROOT: &str = "/Users/someone/repo";

    #[test]
    fn the_normalizer_leaves_a_message_without_a_varying_part_untouched() {
        // The whole risk of a normaliser is that it papers over real
        // differences. A message with no absolute path and no temporary suffix
        // must survive byte-for-byte, punctuation and numbers included.
        let message = "error: xgcc failed: candidate.c:1:98: bad thing 0x08003e58, size 164";
        assert_eq!(normalize(message, ROOT), message);
        let document = "{\n  \"stem\": \"080037d4\",\n  \"register_fraction\": 1\n}";
        assert_eq!(normalize(document, ROOT), document);
    }

    #[test]
    fn the_normalizer_removes_only_the_root_and_temp_suffixes() {
        assert_eq!(
            normalize("at /Users/someone/repo/tools/lib/match_m2c.ts:86:11", ROOT),
            "at <ROOT>/tools/lib/match_m2c.ts:86:11"
        );
        assert_eq!(normalize("scratch.0a1b2c3d4e/x", ROOT), "scratch.<tmp>/x");
        // Seven hex digits is below the threshold, and an extension is not a
        // temporary suffix.
        assert_eq!(normalize("scratch.0a1b2c3/x", ROOT), "scratch.0a1b2c3/x");
        assert_eq!(normalize("080037d4.actual.bin", ROOT), "080037d4.actual.bin");
    }

    #[test]
    fn only_the_first_error_line_survives_and_stack_frames_do_not() {
        let stderr = concat!(
            "85 |     const detail = (error || output).trim();\n",
            "error: xgcc failed: nope\n",
            "      at run (/x/tools/lib/match_m2c.ts:86:11)\n",
            "\nBun v1.3.14 (macOS arm64)\n",
        );
        assert_eq!(first_error_line(stderr), "error: xgcc failed: nope");
        assert_eq!(first_error_line("all quiet\n"), "");
    }

    #[test]
    fn observations_compare_on_code_stdout_and_the_error_line() {
        let base = Observation {
            code: 0,
            stdout: "{}".to_string(),
            error: String::new(),
        };
        let mut other = base.clone();
        assert_eq!(base, other);
        other.code = 1;
        assert_ne!(base, other);
    }

    #[test]
    fn field_and_category_extraction_survive_a_malformed_document() {
        let document = "{ \"dominant\": \"register_only\", \"register_only\": 3, \"semantic\": 0 }";
        assert_eq!(field(document, "dominant").as_deref(), Some("register_only"));
        assert_eq!(field(document, "missing"), None);
        assert_eq!(nonzero_count_categories(document), vec!["register_only"]);
        assert_eq!(field("not json at all", "dominant"), None);
        assert!(nonzero_count_categories("").is_empty());
    }
}
