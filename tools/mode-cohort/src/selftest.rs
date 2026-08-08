//! Port of `selfTest()` in `tools/compiler/mode-cohort`, including its two
//! hand-written reports and all five assertions.

use crate::aggregate::{
    irreducible_configuration_improvements, multi_region_improvements, shared_exact_configurations,
    shared_non_regressing_improvements, single_mode_effects,
};
use crate::report::Report;
use search_compiler_modes::{parse_json, Json};

/// The two fixture reports, transcribed key for key and in key order. Key order
/// matters: `canonicalJson(config)` is the grouping key, so a reordering here
/// would silently change what groups with what.
pub const SELF_TEST_REPORTS: &str = r#"[
  {
    "stem": "08000000",
    "format": 4,
    "source_sha256": "",
    "reference_sha256": "",
    "compiler_signature": "",
    "policy": null,
    "planning": { "bounded_search_complete": true },
    "source": "a.c",
    "results": [
      { "config": { "ids": ["no-gcse"], "flags": ["-fno-gcse"], "remove_flags": [], "compiler_family": "routed" }, "compiled": true,
        "evidence": { "exact": true, "differing_halfwords": 0 } },
      { "config": { "ids": ["shared"], "flags": ["-fshared"], "remove_flags": [], "compiler_family": "routed" }, "compiled": true,
        "evidence": { "exact": false, "differing_halfwords": 1 } },
      { "config": { "ids": ["left"], "flags": ["-fleft"], "remove_flags": [], "compiler_family": "routed" }, "compiled": true,
        "evidence": { "exact": false, "differing_halfwords": 1 } },
      { "config": { "ids": ["left", "right"], "flags": ["-fleft", "-fright"], "remove_flags": [], "compiler_family": "routed" }, "compiled": true,
        "evidence": { "exact": false, "differing_halfwords": 1 } },
      { "config": { "ids": [], "flags": [], "remove_flags": [], "compiler_family": "routed" }, "compiled": true,
        "evidence": { "exact": false, "differing_halfwords": 2 } }
    ]
  },
  {
    "stem": "08000010",
    "format": 4,
    "source_sha256": "",
    "reference_sha256": "",
    "compiler_signature": "",
    "policy": null,
    "planning": { "bounded_search_complete": true },
    "source": "b.c",
    "results": [
      { "config": { "ids": ["no-gcse"], "flags": ["-fno-gcse"], "remove_flags": [], "compiler_family": "routed" }, "compiled": true,
        "evidence": { "exact": true, "differing_halfwords": 0 } },
      { "config": { "ids": ["shared"], "flags": ["-fshared"], "remove_flags": [], "compiler_family": "routed" }, "compiled": true,
        "evidence": { "exact": false, "differing_halfwords": 2 } },
      { "config": { "ids": ["right"], "flags": ["-fright"], "remove_flags": [], "compiler_family": "routed" }, "compiled": true,
        "evidence": { "exact": false, "differing_halfwords": 2 } },
      { "config": { "ids": ["left"], "flags": ["-fleft"], "remove_flags": [], "compiler_family": "routed" }, "compiled": true,
        "evidence": { "exact": false, "differing_halfwords": 3 } },
      { "config": { "ids": ["left", "right"], "flags": ["-fleft", "-fright"], "remove_flags": [], "compiler_family": "routed" }, "compiled": true,
        "evidence": { "exact": false, "differing_halfwords": 2 } },
      { "config": { "ids": [], "flags": [], "remove_flags": [], "compiler_family": "routed" }, "compiled": true,
        "evidence": { "exact": false, "differing_halfwords": 3 } }
    ]
  }
]"#;

pub fn self_test_reports() -> Result<Vec<Report>, String> {
    let parsed = parse_json(SELF_TEST_REPORTS).map_err(|error| error.to_string())?;
    parsed
        .as_array()
        .ok_or_else(|| "self-test fixture is not an array".to_string())?
        .iter()
        .map(|value| Report::from_json(value.clone()))
        .collect()
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

fn rows(value: &Json) -> Vec<Json> {
    value.as_array().map(<[Json]>::to_vec).unwrap_or_default()
}

fn number(row: &Json, key: &str) -> f64 {
    match row.get(key) {
        Some(Json::Number(value)) => *value,
        _ => f64::NAN,
    }
}

/// Returns the line `selfTest` prints on success.
pub fn self_test() -> Result<String, String> {
    let reports = self_test_reports()?;

    let shared = rows(&shared_exact_configurations(&reports)?);
    if shared.len() != 1
        || joined(&shared[0], "exact_stems", ",") != "08000000,08000010"
        || joined(&shared[0], "flags", " ") != "-fno-gcse"
    {
        return Err("mode cohort self-test: shared exact aggregation differs".into());
    }

    let improvements = rows(&shared_non_regressing_improvements(&reports)?);
    let shared_improvement = improvements
        .iter()
        .find(|row| joined(row, "ids", ",") == "shared");
    if improvements.len() != 2
        || shared_improvement.is_none()
        || shared_improvement.is_some_and(|row| {
            joined(row, "improved_stems", ",") != "08000000,08000010"
                || number(row, "total_halfwords_removed") != 2.0
        })
        || improvements
            .iter()
            .any(|row| joined(row, "ids", ",") == "left,right")
    {
        return Err("mode cohort self-test: shared improvement aggregation differs".into());
    }

    let multi = rows(&multi_region_improvements(&reports)?);
    if !multi.iter().any(|row| {
        joined(row, "ids", ",") == "shared"
            && joined(row, "improved_stems", ",") == "08000000,08000010"
    }) || multi
        .iter()
        .any(|row| joined(row, "ids", ",") == "left,right")
    {
        return Err("mode cohort self-test: multi-region improvement aggregation differs".into());
    }

    let irreducible = rows(&irreducible_configuration_improvements(&reports)?);
    if !irreducible
        .iter()
        .any(|row| joined(row, "ids", ",") == "left")
        || irreducible
            .iter()
            .any(|row| joined(row, "ids", ",") == "left,right")
    {
        return Err("mode cohort self-test: irreducible configuration aggregation differs".into());
    }

    let singles = rows(&single_mode_effects(&reports)?);
    if !singles.iter().any(|row| {
        joined(row, "ids", ",") == "shared"
            && joined(row, "improved_stems", ",") == "08000000,08000010"
    }) {
        return Err("mode cohort self-test: single-mode effect aggregation differs".into());
    }

    Ok("mode cohort self-test passed".into())
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::aggregate::category_counts;

    #[test]
    fn self_test_passes() {
        assert_eq!(self_test().unwrap(), "mode cohort self-test passed");
    }

    #[test]
    fn every_category_finds_something() {
        // Floor: if a whole aggregation silently stops matching, the self-test
        // alone would still pass some of its `!some(...)` assertions.
        let reports = self_test_reports().unwrap();
        let counts = category_counts(&crate::aggregate::all_aggregations(&reports).unwrap());
        assert_eq!(counts.len(), 5);
        for (name, found) in counts {
            assert!(found > 0, "category {name} found nothing");
        }
    }

    #[test]
    fn a_missing_routed_baseline_throws() {
        let mut reports = self_test_reports().unwrap();
        reports[0]
            .results
            .retain(|row| !row.is_routed_baseline().unwrap());
        let error = shared_non_regressing_improvements(&reports).unwrap_err();
        assert_eq!(
            error,
            "mode cohort: every report must contain a compiled routed baseline"
        );
    }

    #[test]
    fn shared_exact_ignores_uncompiled_and_inexact_rows() {
        let reports = self_test_reports().unwrap();
        let shared = rows(&shared_exact_configurations(&reports).unwrap());
        assert_eq!(shared.len(), 1);
        assert_eq!(joined(&shared[0], "ids", ","), "no-gcse");
    }
}
