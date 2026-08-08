//! Differential parity harness: the ported `selfTest` body against the live
//! TypeScript.
//!
//! WHAT IS COMPARED. Not "both printed the same success line" -- that is one
//! case and it would still pass if both sides had drifted together or if the
//! assertions had been quietly weakened. Instead the Rust body records every
//! primitive call it makes (`cflagsForTargetSource`, `usesAgbccCompiler`,
//! `directCompilerCommand`, `sourceToAssemblyPlan`) with its arguments and its
//! result, and this harness replays those argument sets through the live
//! TypeScript and byte-compares the results. The corpus is therefore derived
//! from the port rather than hand-listed beside it, and cannot fall out of step
//! with it.
//!
//! GUARDS, because a green harness that scanned nothing is worse than no
//! harness:
//!   * a case floor -- fewer than `MINIMUM_CASES` keys is a hard failure;
//!   * an empty corpus is a hard failure, never "0 differences, ok";
//!   * a distinct-payload floor -- a corpus of 700 identical answers proves
//!     nothing, so the number of DISTINCT values is counted and floored too;
//!   * per-kind floors, so a change that silently stops exercising plans
//!     cannot hide behind several hundred cflags cases;
//!   * a NEGATIVE CONTROL -- one deliberately perturbed record must produce
//!     exactly one difference. Without it the comparison could be vacuous and
//!     nobody would know.

use std::io::Write as _;
use std::process::{Command, Stdio};


/// Below this the corpus is not a corpus. The body currently yields well over
/// three hundred and forty distinct keys; the floor sits far enough below that number to
/// survive honest edits and far enough above zero to catch a body that stopped
/// exercising anything.
const MINIMUM_CASES: usize = 300;
/// Distinct values, not distinct keys. Guards against a degenerate corpus where
/// every answer is the same string.
const MINIMUM_DISTINCT_VALUES: usize = 40;
/// Per-kind floors. `PLAN` is small by nature -- there are only a handful of
/// plan assertions -- but it must not be zero.
const KIND_FLOORS: [(&str, usize); 4] = [("CF", 200), ("UA", 80), ("DCC", 15), ("PLAN", 8)];

fn main() -> std::process::ExitCode {
    match run() {
        Ok(report) => {
            println!("{report}");
            std::process::ExitCode::SUCCESS
        }
        Err(message) => {
            eprintln!("selftest-corpus: {message}");
            std::process::ExitCode::FAILURE
        }
    }
}

fn run() -> Result<String, String> {
    let (_summary, trace) = alchemy_selftest::self_test()?;
    let keys = trace.distinct_keys();

    if keys.is_empty() {
        return Err("the corpus is empty: scanning nothing is not passing".to_string());
    }
    if keys.len() < MINIMUM_CASES {
        return Err(format!(
            "corpus floor: {} cases, need at least {MINIMUM_CASES}",
            keys.len()
        ));
    }
    for (kind, floor) in KIND_FLOORS {
        let count = keys
            .iter()
            .filter(|key| key.split('\t').next() == Some(kind))
            .count();
        if count < floor {
            return Err(format!(
                "corpus floor for {kind}: {count} cases, need at least {floor}"
            ));
        }
    }

    let ours: Vec<String> = keys
        .iter()
        .map(|key| {
            trace
                .value_of(key)
                .expect("every distinct key came from a record")
                .to_string()
        })
        .collect();

    let distinct_values = distinct(&ours);
    if distinct_values < MINIMUM_DISTINCT_VALUES {
        return Err(format!(
            "degenerate corpus: {distinct_values} distinct payloads across {} cases, \
             need at least {MINIMUM_DISTINCT_VALUES}",
            ours.len()
        ));
    }

    let theirs = probe(&keys)?;
    if theirs.len() != keys.len() {
        return Err(format!(
            "probe returned {} rows for {} keys",
            theirs.len(),
            keys.len()
        ));
    }

    let differences = compare(&keys, &ours, &theirs);
    if !differences.is_empty() {
        let shown: Vec<String> = differences.iter().take(5).cloned().collect();
        return Err(format!(
            "{} of {} cases differ from the TypeScript:\n{}",
            differences.len(),
            keys.len(),
            shown.join("\n")
        ));
    }

    // NEGATIVE CONTROL. Perturb exactly one of our answers and confirm the
    // comparison notices, and notices exactly once. A harness that cannot fail
    // has not passed.
    let control = negative_control(&keys, &ours, &theirs)?;

    Ok(format!(
        "selftest-corpus=ok cases={} distinct_payloads={} kinds={} negative_control={control}",
        keys.len(),
        distinct_values,
        kind_summary(&keys),
    ))
}

fn negative_control(
    keys: &[String],
    ours: &[String],
    theirs: &[String],
) -> Result<String, String> {
    let index = keys.len() / 2;
    let mut perturbed = ours.to_vec();
    perturbed[index] = format!("{}\u{1f}--negative-control", ours[index]);
    if perturbed[index] == ours[index] {
        return Err("the negative control did not actually perturb anything".to_string());
    }
    let found = compare(keys, &perturbed, theirs);
    if found.len() != 1 {
        return Err(format!(
            "negative control produced {} differences, expected exactly 1",
            found.len()
        ));
    }
    Ok(format!("1-difference-at-case-{index}"))
}

fn compare(keys: &[String], ours: &[String], theirs: &[String]) -> Vec<String> {
    let mut differences = Vec::new();
    for ((key, mine), yours) in keys.iter().zip(ours).zip(theirs) {
        if mine != yours {
            differences.push(format!(
                "  {}\n    rust: {}\n    ts:   {}",
                key.replace('\t', " | "),
                mine.replace('\u{1f}', " "),
                yours.replace('\u{1f}', " ")
            ));
        }
    }
    differences
}

/// Distinct entries, first-seen order preserved. No `HashSet`: the counts are
/// reported to a human and stable output beats a fast one at this size.
fn distinct(values: &[String]) -> usize {
    let mut seen: Vec<&str> = Vec::new();
    for value in values {
        if !seen.contains(&value.as_str()) {
            seen.push(value);
        }
    }
    seen.len()
}

fn kind_summary(keys: &[String]) -> String {
    let mut counts: Vec<(String, usize)> = Vec::new();
    for key in keys {
        let kind = key.split('\t').next().unwrap_or("?").to_string();
        match counts.iter_mut().find(|(name, _)| name == &kind) {
            Some((_, count)) => *count += 1,
            None => counts.push((kind, 1)),
        }
    }
    counts
        .iter()
        .map(|(kind, count)| format!("{kind}:{count}"))
        .collect::<Vec<String>>()
        .join(",")
}

/// Run `harness/typescript_probe.ts` over the keys and read back the values.
fn probe(keys: &[String]) -> Result<Vec<String>, String> {
    let root = alchemy_lints::repository_root();
    let script = root.join("tools-rs/alchemy-selftest/harness/typescript_probe.ts");
    if !script.exists() {
        return Err(format!("probe script missing: {}", script.display()));
    }
    let mut child = Command::new("bun")
        .arg(&script)
        .current_dir(&root)
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
        .spawn()
        .map_err(|error| format!("cannot spawn bun: {error}"))?;
    let payload = format!("{}\n", keys.join("\n"));
    child
        .stdin
        .take()
        .expect("stdin was piped")
        .write_all(payload.as_bytes())
        .map_err(|error| format!("cannot write to probe: {error}"))?;
    let output = child
        .wait_with_output()
        .map_err(|error| format!("probe failed: {error}"))?;
    if !output.status.success() {
        return Err(format!(
            "probe exited {}: {}",
            output.status,
            String::from_utf8_lossy(&output.stderr).trim()
        ));
    }
    let text = String::from_utf8(output.stdout)
        .map_err(|error| format!("probe emitted non-UTF-8: {error}"))?;

    let mut values = Vec::new();
    for (index, line) in text.lines().enumerate() {
        let (key, value) = line
            .split_once('\u{1d}')
            .ok_or_else(|| format!("probe row {index} has no key/value separator"))?;
        if keys.get(index).map(String::as_str) != Some(key) {
            return Err(format!(
                "probe row {index} answered the wrong key: {key}"
            ));
        }
        values.push(value.to_string());
    }
    Ok(values)
}
