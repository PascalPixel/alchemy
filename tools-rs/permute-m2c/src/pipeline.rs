//! `main()`: read the near-miss report, permute each draft, and keep the first
//! variant whose linked `.text` equals the ROM.

use std::collections::BTreeSet;

use match_m2c::M2C_PREAMBLE;
use verify::routing::CompilerTarget;

use crate::assembly::{replaceable_assembly, retained_assembly_stems};
use crate::cli::Options;
use crate::jsnum::hex_stem;
use crate::json;
use crate::paths;
use crate::transforms::candidates;

/// The six substitutions tried for a draft that still contains `M2C_UNK`.
///
/// PORT NOTE: order is load-bearing. `main` stops at the first variant that
/// verifies, so this list decides which type a rescued function ends up with.
pub const UNKNOWN_TYPES: [&str; 6] = ["s32", "u32", "void *", "s16", "u16", "s8"];

/// The `mismatched_bytes` fallback used both for the filter and for the sort.
///
/// PORT NOTE: `row.mismatched_bytes ?? 999` is nullish coalescing, so `0` and
/// `false` pass through while `null` and a missing field become `999`.
const MISSING_MISMATCH: f64 = 999.0;

#[derive(Debug, Clone)]
pub struct NearMiss {
    pub entry: f64,
    pub mismatched: f64,
}

#[derive(Debug, Default)]
pub struct Outcome {
    pub matched: usize,
    pub near: usize,
    pub lines: Vec<String>,
    /// Instrumentation for the parity harness and the benchmark. Not printed.
    pub drafts_read: usize,
    pub candidates_built: usize,
    pub verifies_run: usize,
}

/// The `filter` plus `sort` that produce the working list.
///
/// PORT NOTE (trap: a NaN comparator is spec-treated as `+0`). The comparator
/// is `left - right`; if either side is NaN the subtraction is NaN, which
/// `Array#sort` coerces to zero, i.e. "equal", leaving the pair in its existing
/// relative position. `f64::partial_cmp` returns `None` there and `total_cmp`
/// orders NaN last, so both would reorder rows the TypeScript leaves alone.
/// `subtract_ordering` models the specification. `Vec::sort_by` is stable, as
/// `Array#sort` is.
pub fn near_misses(rows: &[json::Value]) -> Vec<NearMiss> {
    let mut near: Vec<NearMiss> = rows
        .iter()
        .filter(|row| {
            let matched = row.get("matched").map(|value| value.truthy()).unwrap_or(false);
            if matched {
                return false;
            }
            let mismatched = mismatched_bytes(row);
            // PORT NOTE: `NaN <= 24` is false, so a non-numeric field drops the
            // row. `!(x > 24.0)` would keep it. Written as a plain `<=` on
            // purpose.
            mismatched <= 24.0
        })
        .map(|row| NearMiss {
            entry: row
                .get("entry")
                .map(|value| value.to_number())
                .unwrap_or(f64::NAN),
            mismatched: mismatched_bytes(row),
        })
        .collect();
    near.sort_by(|left, right| crate::jsnum::subtract_ordering(left.mismatched, right.mismatched));
    near
}

fn mismatched_bytes(row: &json::Value) -> f64 {
    match row.get("mismatched_bytes") {
        None => MISSING_MISMATCH,
        Some(value) if value.nullish() => MISSING_MISMATCH,
        Some(value) => value.to_number(),
    }
}

/// `Array.isArray(document) ? document : (document.results ?? [])`.
pub fn report_rows(document: &json::Value) -> Vec<json::Value> {
    if let Some(items) = document.as_array() {
        return items.to_vec();
    }
    match document.get("results") {
        Some(value) if !value.nullish() => value.as_array().map(<[json::Value]>::to_vec).unwrap_or_default(),
        _ => Vec::new(),
    }
}

pub fn run(options: &Options) -> Result<Outcome, String> {
    let rom = std::fs::read(&options.rom)
        .map_err(|error| format!("cannot read {}: {error}", options.rom))?;
    let output = paths::out_dir();
    std::fs::create_dir_all(&output)
        .map_err(|error| format!("cannot create {}: {error}", output.display()))?;

    let exact = paths::exact_dir();
    let mut tracked: BTreeSet<String> = BTreeSet::new();
    let entries = std::fs::read_dir(&exact)
        .map_err(|error| format!("cannot read {}: {error}", exact.display()))?;
    for entry in entries {
        let entry = entry.map_err(|error| format!("cannot read {}: {error}", exact.display()))?;
        let name = entry.file_name().to_string_lossy().into_owned();
        if let Some(stem) = name.strip_suffix(".c") {
            tracked.insert(stem.to_string());
        }
    }
    let retained = retained_assembly_stems()?;

    let text = std::fs::read_to_string(&options.report)
        .map_err(|error| format!("cannot read {}: {error}", options.report))?;
    let document = json::parse(&text)
        .map_err(|error| format!("cannot parse {}: {error}", options.report))?;
    let rows = report_rows(&document);
    let near = near_misses(&rows);

    let mut outcome = Outcome {
        near: near.len(),
        ..Outcome::default()
    };

    for row in &near {
        let stem = hex_stem(row.entry);
        if tracked.contains(&stem) || retained.contains(&stem) {
            continue;
        }
        let draft = std::path::Path::new(&options.drafts).join(format!("{stem}.c"));
        if !draft.exists() {
            continue;
        }
        // PORT NOTE: `readFileSync(draft, "utf8")` replaces invalid sequences
        // rather than throwing, which `std::fs::read_to_string` would.
        let raw = match std::fs::read(&draft) {
            Ok(bytes) => String::from_utf8_lossy(&bytes).into_owned(),
            Err(error) => return Err(format!("cannot read {}: {error}", draft.display())),
        };
        outcome.drafts_read += 1;

        let bases: Vec<String> = if raw.contains("M2C_UNK") {
            UNKNOWN_TYPES
                .iter()
                .map(|replacement| crate::jsstring::replace_all(&raw, "M2C_UNK", replacement))
                .collect()
        } else {
            vec![raw.clone()]
        };

        let mut hit = false;
        for base in &bases {
            for body in candidates(base) {
                if body == *base {
                    continue;
                }
                outcome.candidates_built += 1;
                let candidate = output.join(format!("{stem}.c"));
                let payload = format!("{M2C_PREAMBLE}{body}");
                std::fs::write(&candidate, payload.as_bytes())
                    .map_err(|error| format!("cannot write {}: {error}", candidate.display()))?;
                outcome.verifies_run += 1;
                // PORT NOTE: the TypeScript wraps `verify` in a bare
                // `try { } catch { continue }`, so every failure -- a compiler
                // error, a missing symbol, an unreadable ROM slice -- is the
                // same "try the next variant". Reproduced by discarding the
                // error rather than propagating it.
                let Ok((okay, linked_size)) = verify::verify(
                    &candidate.to_string_lossy(),
                    &rom,
                    &output.to_string_lossy(),
                    CompilerTarget::Gs1,
                ) else {
                    continue;
                };
                if !okay {
                    continue;
                }
                if !replaceable_assembly(&stem, linked_size, &output) {
                    outcome
                        .lines
                        .push(format!("skip {stem}: asm region longer than matched C"));
                    continue;
                }
                let landed = exact.join(format!("{stem}.c"));
                std::fs::write(&landed, payload.as_bytes())
                    .map_err(|error| format!("cannot write {}: {error}", landed.display()))?;
                // `rmSync(..., { force: true })`: a missing file is not an error.
                let _ = std::fs::remove_file(paths::asm_dir().join(format!("{stem}.s")));
                outcome.matched += 1;
                hit = true;
                outcome.lines.push(format!("matched {stem}"));
                break;
            }
            if hit {
                break;
            }
        }
    }

    outcome.lines.push(format!(
        "matched={} of {} near-misses",
        outcome.matched, outcome.near
    ));
    Ok(outcome)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn rows(text: &str) -> Vec<json::Value> {
        report_rows(&json::parse(text).expect("parses"))
    }

    #[test]
    fn a_bare_array_and_a_results_object_are_both_accepted() {
        assert_eq!(rows("[{\"entry\":1}]").len(), 1);
        assert_eq!(rows("{\"results\":[{\"entry\":1},{\"entry\":2}]}").len(), 2);
        assert_eq!(rows("{\"results\":null}").len(), 0);
        assert_eq!(rows("{}").len(), 0);
    }

    #[test]
    fn a_missing_mismatch_field_drops_the_row() {
        // 999 is above the threshold, so a row without the field never runs.
        let near = near_misses(&rows("[{\"entry\":1,\"matched\":false}]"));
        assert!(near.is_empty());
    }

    #[test]
    fn zero_mismatched_bytes_survives_nullish_coalescing() {
        // `0 ?? 999` is 0, unlike `0 || 999`.
        let near = near_misses(&rows(
            "[{\"entry\":1,\"matched\":false,\"mismatched_bytes\":0}]",
        ));
        assert_eq!(near.len(), 1);
        assert_eq!(near[0].mismatched, 0.0);
    }

    #[test]
    fn a_nan_mismatch_drops_the_row_rather_than_sorting_last() {
        let near = near_misses(&rows(
            "[{\"entry\":1,\"matched\":false,\"mismatched_bytes\":\"x\"}]",
        ));
        assert!(near.is_empty());
    }

    #[test]
    fn the_sort_is_stable_and_treats_an_unorderable_pair_as_equal() {
        let near = near_misses(&rows(
            "[{\"entry\":1,\"matched\":false,\"mismatched_bytes\":4},\
              {\"entry\":2,\"matched\":false,\"mismatched_bytes\":1},\
              {\"entry\":3,\"matched\":false,\"mismatched_bytes\":4}]",
        ));
        let order: Vec<f64> = near.iter().map(|row| row.entry).collect();
        assert_eq!(order, vec![2.0, 1.0, 3.0]);
    }

    #[test]
    fn truthiness_not_equality_decides_matched() {
        // `!row.matched` is falsiness, so 0 and "" also count as unmatched.
        let near = near_misses(&rows(
            "[{\"entry\":1,\"matched\":0,\"mismatched_bytes\":1},\
              {\"entry\":2,\"matched\":\"no\",\"mismatched_bytes\":1}]",
        ));
        assert_eq!(near.len(), 1);
        assert_eq!(near[0].entry, 1.0);
    }

    #[test]
    fn the_stem_is_eight_hex_digits_and_is_never_truncated() {
        assert_eq!(hex_stem(0x0800_1234_u32 as f64), "08001234");
        assert_eq!(hex_stem(1.0), "00000001");
        // padStart never truncates a longer value.
        assert_eq!(hex_stem(0x1_0800_1234_u64 as f64), "108001234");
    }
}
