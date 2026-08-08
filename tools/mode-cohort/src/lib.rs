//! Rust port of `tools/compiler/mode_cohort.ts`.
//!
//! The TypeScript header states the tool's remit: run `mode_sweep` over several
//! candidates and report the compiler configurations that behave the same way
//! across all of them. It never compiles, never routes and never promotes; the
//! aggregation layer here is pure and total.
//!
//! The worker invokes the native `tools/mode-sweep` binary. This crate owns
//! only cohort aggregation and report shaping.

pub mod aggregate;
pub mod collate;
pub mod jsops;
pub mod report;
pub mod selftest;

pub use aggregate::{
    all_aggregations, category_counts, irreducible_configuration_improvements,
    irreducible_multi_id, multi_region_improvements, shared_exact_configurations,
    shared_non_regressing_improvements, single_mode_effects,
};
pub use collate::{cmp_utf16, collate, sort_default, COLLATED_ASCII};
pub use report::{Evidence, Report, ResultRow};
pub use selftest::{self_test, self_test_reports};

use search_compiler_modes::{canonical_json, sha256_hex, Json};
use std::path::Path;

/// `FORMAT`-equivalent: the summary's `format` field.
pub const FORMAT: f64 = 4.0;

fn projected_member(report: &Report) -> Json {
    let pick = |key: &str| report.raw.get(key).cloned().unwrap_or(Json::Null);
    Json::Object(vec![
        ("source".into(), pick("source")),
        ("source_sha256".into(), pick("source_sha256")),
        ("reference_sha256".into(), pick("reference_sha256")),
        ("compiler_signature".into(), pick("compiler_signature")),
        ("policy".into(), pick("policy")),
        ("planning".into(), pick("planning")),
    ])
}

fn source_of(value: &Json) -> &str {
    value.get("source").and_then(Json::as_str).unwrap_or("")
}

/// The cohort output directory name: the first sixteen hex digits of the
/// SHA-256 of the canonical JSON of the projected, source-sorted members.
pub fn cohort_digest(reports: &[Report]) -> String {
    let mut members: Vec<Json> = reports.iter().map(projected_member).collect();
    // `localeCompare`, not `str::cmp`. Sources are absolute paths, so they carry
    // `/`, `.`, `-` and `_`, every one of which the collator orders differently
    // from ASCII.
    members.sort_by(|left, right| collate(source_of(left), source_of(right)));
    let text = canonical_json(&Json::Array(members));
    sha256_hex(text.as_bytes())[..16].to_string()
}

/// The `summary` object `main` writes to `out/modesweep/cohort/<digest>/report.json`.
pub fn cohort_summary(reports: &[Report], exhaustive_pair_count: f64) -> Result<Json, String> {
    let shared = shared_exact_configurations(reports)?;
    let improvements = shared_non_regressing_improvements(reports)?;
    let irreducible = irreducible_multi_id(reports)?;
    let multi_region = multi_region_improvements(reports)?;
    let single_effects = single_mode_effects(reports)?;
    let members = Json::Array(
        reports
            .iter()
            .map(|report| {
                let pick = |key: &str| report.raw.get(key).cloned().unwrap_or(Json::Null);
                Json::Object(vec![
                    ("source".into(), pick("source")),
                    ("source_sha256".into(), pick("source_sha256")),
                    ("reference_sha256".into(), pick("reference_sha256")),
                    ("compiler_signature".into(), pick("compiler_signature")),
                    (
                        "bounded_search_complete".into(),
                        report
                            .raw
                            .get("planning")
                            .and_then(|planning| planning.get("bounded_search_complete"))
                            .cloned()
                            .unwrap_or(Json::Null),
                    ),
                ])
            })
            .collect(),
    );
    let stems = Json::Array(
        reports
            .iter()
            .map(|report| Json::String(report.stem.clone()))
            .collect(),
    );
    Ok(Json::Object(vec![
        ("format".into(), Json::Number(FORMAT)),
        ("members".into(), members),
        ("stems".into(), stems),
        ("shared_exact_configurations".into(), shared),
        ("shared_nonregressing_improvements".into(), improvements),
        ("irreducible_configuration_improvements".into(), irreducible),
        ("multi_region_improvements".into(), multi_region),
        ("single_mode_effects".into(), single_effects),
        (
            "common_pair_configurations".into(),
            Json::Number(exhaustive_pair_count),
        ),
        ("auto_promote".into(), Json::Bool(false)),
    ]))
}

/// The repository root, matching `ROOT` in the TypeScript.
pub fn root() -> std::path::PathBuf {
    mode_sweep::root()
}

/// `join(modeSweepOutputDirectory(source), "report.json")`.
pub fn member_report_path(root: &Path, source: &str) -> std::io::Result<String> {
    let contents = std::fs::read(source)?;
    Ok(format!(
        "{}/report.json",
        mode_sweep::mode_sweep_output_directory(root, source, &contents)
    ))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn digest_is_sixteen_hex_digits_and_order_independent() {
        let mut reports = self_test_reports().unwrap();
        let forward = cohort_digest(&reports);
        assert_eq!(forward.len(), 16);
        assert!(forward.chars().all(|c| c.is_ascii_hexdigit()));
        reports.reverse();
        assert_eq!(cohort_digest(&reports), forward, "digest sorts by source");
    }

    #[test]
    fn summary_key_order_is_the_typescript_key_order() {
        let reports = self_test_reports().unwrap();
        let summary = cohort_summary(&reports, 10.0).unwrap();
        let Json::Object(entries) = &summary else {
            panic!("summary is an object");
        };
        let keys: Vec<&str> = entries.iter().map(|(key, _)| key.as_str()).collect();
        assert_eq!(
            keys,
            vec![
                "format",
                "members",
                "stems",
                "shared_exact_configurations",
                "shared_nonregressing_improvements",
                "irreducible_configuration_improvements",
                "multi_region_improvements",
                "single_mode_effects",
                "common_pair_configurations",
                "auto_promote",
            ]
        );
    }

    #[test]
    fn summary_projects_irreducible_to_multi_id_rows_only() {
        let reports = self_test_reports().unwrap();
        let summary = cohort_summary(&reports, 10.0).unwrap();
        let rows = summary
            .get("irreducible_configuration_improvements")
            .and_then(Json::as_array)
            .unwrap();
        for row in rows {
            let ids = row.get("ids").and_then(Json::as_array).unwrap();
            assert!(ids.len() > 1);
        }
    }

    #[test]
    fn every_emitted_number_is_integral() {
        // PORT NOTE: `canonical_json` routes numbers through `js_number_text`,
        // which implements ECMAScript ToString(Number), so this is belt and
        // braces. It also fails loudly if a future change starts emitting a
        // fractional or sub-1e-6 halfword count, where a naive `{}` formatter
        // would have diverged from `JSON.stringify`.
        fn walk(value: &Json, found: &mut usize) {
            match value {
                Json::Number(number) => {
                    assert!(
                        number.fract() == 0.0 && number.is_finite(),
                        "non-integral number {number} reaches JSON"
                    );
                    *found += 1;
                }
                Json::Array(items) => items.iter().for_each(|item| walk(item, found)),
                Json::Object(entries) => entries.iter().for_each(|(_, item)| walk(item, found)),
                _ => {}
            }
        }
        let reports = self_test_reports().unwrap();
        let summary = cohort_summary(&reports, 10.0).unwrap();
        let mut found = 0;
        walk(&summary, &mut found);
        assert!(found >= 10, "floor: only {found} numbers walked");
    }
}
