//! Port of `main()` in `tools/compiler/mode-cohort`.

use mode_cohort::{cohort_digest, cohort_summary, member_report_path, root, self_test, Report};
use search_compiler_modes::{canonical_json, is_js_integer, js_number, parse_json, Json};
use std::path::Path;
use std::process::Command;

const USAGE: &str = "usage: mode-cohort [--jobs N] [--max-pairs N] [--max-triples N] CANDIDATE.c [CANDIDATE.c ...]";

fn main() {
    match run() {
        Ok(lines) => {
            for line in lines {
                println!("{line}");
            }
        }
        Err(message) => {
            // Child-process failures are reported with the native command's
            // exit code and message text, not a wrapper-specific stack trace.
            eprintln!("error: {message}");
            std::process::exit(1);
        }
    }
}

fn number_argument(arguments: &[String], index: usize) -> f64 {
    // `Number(argv[++index])`: reading past the end yields `undefined`, and
    // `Number(undefined)` is NaN, which then fails `Number.isInteger`.
    match arguments.get(index) {
        Some(text) => js_number(text),
        None => f64::NAN,
    }
}

fn available_parallelism() -> f64 {
    std::thread::available_parallelism().map_or(8.0, |n| n.get() as f64)
}

fn run() -> Result<Vec<String>, String> {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    if arguments.iter().any(|argument| argument == "--self-test") {
        return Ok(vec![self_test()?]);
    }

    let root = root();
    // `Math.max(1, Math.min(8, (navigator.hardwareConcurrency || 8) - 2))`.
    let mut jobs = 1.0f64.max(8.0f64.min(available_parallelism() - 2.0));
    let exhaustive_pair_count = mode_sweep::pair_configs(None).len() as f64;
    let mut max_pairs = exhaustive_pair_count;
    let mut max_triples = 64.0f64;
    let mut sources: Vec<String> = Vec::new();

    let mut index = 0usize;
    while index < arguments.len() {
        match arguments[index].as_str() {
            "--jobs" => {
                index += 1;
                jobs = number_argument(&arguments, index);
            }
            "--max-pairs" => {
                index += 1;
                max_pairs = number_argument(&arguments, index);
            }
            "--max-triples" => {
                index += 1;
                max_triples = number_argument(&arguments, index);
            }
            "-h" | "--help" => return Ok(vec![USAGE.to_string()]),
            other => sources.push(search_compiler_modes::resolve_path(
                &std::env::current_dir().map_err(|error| error.to_string())?,
                other,
            )),
        }
        index += 1;
    }

    let positive_integer = |value: f64| is_js_integer(value) && value >= 1.0;
    if !positive_integer(jobs) || !positive_integer(max_pairs) || !positive_integer(max_triples) {
        return Err("jobs and search bounds must be positive integers".into());
    }
    if max_pairs < exhaustive_pair_count {
        return Err(format!(
            "mode cohort requires one exhaustive common pair matrix ({exhaustive_pair_count} pairs)"
        ));
    }
    if sources.len() < 2 {
        return Err("a cohort requires at least two candidates".into());
    }

    let mut reports: Vec<Report> = Vec::with_capacity(sources.len());
    for source in &sources {
        reports.push(sweep_member(&root, source, max_pairs, max_triples)?);
    }

    let summary = cohort_summary(&reports, exhaustive_pair_count)?;
    let digest = cohort_digest(&reports);
    let output = root.join("out/modesweep/cohort").join(&digest);
    std::fs::create_dir_all(&output).map_err(|error| error.to_string())?;
    let report_path = output.join("report.json");
    std::fs::write(&report_path, canonical_json(&summary) + "\n")
        .map_err(|error| error.to_string())?;

    Ok(console_lines(
        &summary,
        reports.len(),
        &report_path.to_string_lossy(),
    ))
}

/// One worker iteration: run the native mode-sweep binary over one candidate
/// and read back its report.
///
/// PORT NOTE ON CONCURRENCY: the legacy implementation starts `min(jobs, sources.length)`
/// workers off a shared cursor and stores each result at its *source* index, so
/// `reports` is in source order regardless of completion order. Running the
/// members sequentially produces the identical `reports` array; only wall clock
/// differs, and every member is a multi-minute compile sweep dominated by its
/// own child process.
fn sweep_member(
    root: &Path,
    source: &str,
    max_pairs: f64,
    max_triples: f64,
) -> Result<Report, String> {
    let output = Command::new(root.join("tools/mode-sweep/target/release/mode-sweep"))
        .arg(source)
        .args([
            "--pairs",
            "--triples",
            "--family-factorial",
            "--jobs",
            "1",
            "--top",
            "1",
        ])
        .arg("--max-pairs")
        .arg(search_compiler_modes::js_number_text(max_pairs))
        .arg("--max-triples")
        .arg(search_compiler_modes::js_number_text(max_triples))
        .current_dir(root)
        .output()
        .map_err(|error| format!("{}: {error}", basename(source)))?;
    if !output.status.success() {
        let stderr = String::from_utf8_lossy(&output.stderr).to_string();
        let stdout = String::from_utf8_lossy(&output.stdout).to_string();
        // `stderr || stdout`: the empty string is falsy.
        let text = if stderr.is_empty() { stdout } else { stderr };
        let trimmed = text.trim();
        // `.slice(0, 500)` counts UTF-16 code units.
        let units: Vec<u16> = trimmed.encode_utf16().take(500).collect();
        return Err(format!(
            "{}: {}",
            basename(source),
            String::from_utf16_lossy(&units)
        ));
    }
    let path = member_report_path(root, source).map_err(|error| error.to_string())?;
    let text = std::fs::read_to_string(&path).map_err(|error| error.to_string())?;
    Report::from_json(parse_json(&text)?)
}

fn basename(path: &str) -> &str {
    path.rsplit('/').next().unwrap_or(path)
}

fn rows<'a>(summary: &'a Json, key: &str) -> &'a [Json] {
    summary.get(key).and_then(Json::as_array).unwrap_or(&[])
}

fn joined(row: &Json, key: &str, separator: &str) -> String {
    row.get(key)
        .and_then(Json::as_array)
        .map(|items| {
            items
                .iter()
                .filter_map(Json::as_str)
                .collect::<Vec<_>>()
                .join(separator)
        })
        .unwrap_or_default()
}

fn count(row: &Json, key: &str) -> usize {
    row.get(key)
        .and_then(Json::as_array)
        .map_or(0, <[Json]>::len)
}

fn number_text(row: &Json, key: &str) -> String {
    match row.get(key) {
        Some(Json::Number(value)) => search_compiler_modes::js_number_text(*value),
        _ => "undefined".to_string(),
    }
}

/// The trailing `console.log` block, kept out of `run` so it can be tested.
fn console_lines(summary: &Json, cohort: usize, report_path: &str) -> Vec<String> {
    let shared = rows(summary, "shared_exact_configurations");
    let improvements = rows(summary, "shared_nonregressing_improvements");
    let multi_region = rows(summary, "multi_region_improvements");
    let singles = rows(summary, "single_mode_effects");
    let mut lines = vec![format!(
        "cohort={cohort} shared_exact={} shared_improvements={} multi_region={}",
        shared.len(),
        improvements.len(),
        multi_region.len()
    )];
    for row in shared {
        // `row.flags.join(" ") || "(routed default)"`: the empty join is falsy.
        let flags = joined(row, "flags", " ");
        let label = if flags.is_empty() {
            "(routed default)".to_string()
        } else {
            flags
        };
        lines.push(format!(
            "{}  {label}  {}",
            count(row, "exact_stems"),
            joined(row, "exact_stems", ",")
        ));
    }
    for row in improvements {
        let flags = joined(row, "flags", " ");
        let ids = joined(row, "ids", "+");
        let label = if !flags.is_empty() {
            flags
        } else if !ids.is_empty() {
            ids
        } else {
            "(routed default)".to_string()
        };
        lines.push(format!(
            "improves={} removed={}hw  {label}  {}",
            count(row, "improved_stems"),
            number_text(row, "total_halfwords_removed"),
            joined(row, "improved_stems", ",")
        ));
    }
    for row in multi_region {
        let flags = joined(row, "flags", " ");
        let label = if flags.is_empty() {
            joined(row, "ids", "+")
        } else {
            flags
        };
        lines.push(format!(
            "multi={} regressed={} removed={}hw added={}hw  {label}  {}",
            count(row, "improved_stems"),
            count(row, "regressed_stems"),
            number_text(row, "total_halfwords_removed"),
            number_text(row, "total_halfwords_added"),
            joined(row, "improved_stems", ",")
        ));
    }
    for row in singles
        .iter()
        .filter(|row| count(row, "improved_stems") > 0)
    {
        lines.push(format!(
            "single={} improved={} regressed={} removed={}hw added={}hw",
            joined(row, "ids", "+"),
            count(row, "improved_stems"),
            count(row, "regressed_stems"),
            number_text(row, "total_halfwords_removed"),
            number_text(row, "total_halfwords_added")
        ));
    }
    lines.push(format!("report={report_path}"));
    lines
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn self_test_is_the_only_runnable_mode() {
        assert_eq!(self_test().unwrap(), "mode cohort self-test passed");
    }

    #[test]
    fn console_block_matches_the_native_shape() {
        let reports = mode_cohort::self_test_reports().unwrap();
        let summary = cohort_summary(&reports, 10.0).unwrap();
        let lines = console_lines(&summary, reports.len(), "/tmp/report.json");
        assert!(lines[0].starts_with("cohort=2 shared_exact=1 shared_improvements=2 "));
        assert!(lines.iter().any(|line| line.starts_with("2  -fno-gcse  ")));
        assert!(lines.iter().any(|line| line.contains("removed=2hw")));
        assert_eq!(lines.last().unwrap(), "report=/tmp/report.json");
        // Floor: the block is not silently empty.
        assert!(lines.len() >= 6, "only {} lines emitted", lines.len());
    }

    #[test]
    fn integral_halfwords_print_without_a_decimal_point() {
        // `${2}` is "2" in JavaScript. Rust's `{}` on an f64 gives "2".
        // `js_number_text` is what actually guarantees it across the range.
        assert_eq!(search_compiler_modes::js_number_text(2.0), "2");
        assert_eq!(search_compiler_modes::js_number_text(1e21), "1e+21");
    }

    #[test]
    fn a_trailing_option_without_a_value_is_rejected() {
        assert!(number_argument(&["--jobs".to_string()], 1).is_nan());
        assert!(!is_js_integer(f64::NAN));
    }

    #[test]
    fn basename_strips_directories() {
        assert_eq!(basename("/a/b/080006fc.c"), "080006fc.c");
        assert_eq!(basename("080006fc.c"), "080006fc.c");
    }

    #[test]
    fn collate_is_reachable_from_the_binary() {
        assert_eq!(mode_cohort::collate("a-b", "a+b"), std::cmp::Ordering::Less);
    }
}
