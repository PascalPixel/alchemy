//! The slice of a `mode_sweep` `report.json` that `mode_cohort.ts` reads.

use search_compiler_modes::{canonical_json, Json};

/// `result.evidence` as JavaScript actually sees it.
///
/// PORT NOTE: `Missing` and `Null` are genuinely different states here.
/// `mode_cohort.ts` tests `result.evidence?.exact` in one function and
/// `result.evidence === undefined` in three others. Optional chaining short
/// circuits on `null`, but `=== undefined` does not, so an explicit
/// `"evidence": null` passes the guard and then throws a `TypeError` on the
/// property read. Collapsing both into one `Option` would silently invent a
/// clean skip where the TypeScript aborts the whole run.
#[derive(Debug, Clone, PartialEq)]
pub enum Evidence {
    Missing,
    Null,
    Present {
        exact: bool,
        differing_halfwords: f64,
    },
}

#[derive(Debug, Clone)]
pub struct ResultRow {
    /// The `config` object exactly as parsed, key order intact. It is both the
    /// grouping key (via `canonicalJson`) and, spread with `{...config}`, the
    /// leading keys of every emitted row.
    pub config: Json,
    pub compiled: bool,
    pub evidence: Evidence,
}

#[derive(Debug, Clone)]
pub struct Report {
    pub stem: String,
    pub results: Vec<ResultRow>,
    /// The whole document, for the members/digest projection in `main`.
    pub raw: Json,
}

fn field<'a>(object: &'a Json, key: &str, context: &str) -> Result<&'a Json, String> {
    object
        .get(key)
        .ok_or_else(|| format!("{context}: missing {key}"))
}

impl ResultRow {
    /// `canonicalJson(result.config)`.
    pub fn key(&self) -> String {
        canonical_json(&self.config)
    }

    /// `result.config.ids`. JavaScript would throw a `TypeError` reading
    /// `.length` off `undefined`; so does this.
    pub fn ids(&self) -> Result<Vec<&str>, String> {
        let ids = field(&self.config, "ids", "config")?;
        let items = ids
            .as_array()
            .ok_or_else(|| "config: ids is not an array".to_string())?;
        items
            .iter()
            .map(|item| {
                item.as_str()
                    .ok_or_else(|| "config: ids contains a non-string".to_string())
            })
            .collect()
    }

    pub fn compiler_family(&self) -> Option<&str> {
        self.config.get("compiler_family").and_then(Json::as_str)
    }

    /// The routed baseline predicate, spelled out in four places in the
    /// TypeScript and identical in all four.
    pub fn is_routed_baseline(&self) -> Result<bool, String> {
        let empty = |key: &str| -> bool {
            matches!(self.config.get(key), Some(Json::Array(items)) if items.is_empty())
        };
        Ok(self.ids()?.is_empty()
            && self.compiler_family() == Some("routed")
            && empty("flags")
            && empty("remove_flags"))
    }
}

impl Report {
    pub fn from_json(raw: Json) -> Result<Self, String> {
        let stem = field(&raw, "stem", "report")?
            .as_str()
            .ok_or_else(|| "report: stem is not a string".to_string())?
            .to_string();
        let results = field(&raw, "results", "report")?
            .as_array()
            .ok_or_else(|| "report: results is not an array".to_string())?
            .iter()
            .map(|row| {
                let config = field(row, "config", "result")?.clone();
                if !matches!(config, Json::Object(_)) {
                    return Err("result: config is not an object".to_string());
                }
                // `!result.compiled` is a truthiness test, not a type test.
                let compiled = row.get("compiled").map(Json::truthy).unwrap_or(false);
                let evidence = match row.get("evidence") {
                    None => Evidence::Missing,
                    Some(Json::Null) => Evidence::Null,
                    Some(value) => Evidence::Present {
                        exact: value.get("exact").map(Json::truthy).unwrap_or(false),
                        differing_halfwords: match value.get("differing_halfwords") {
                            Some(Json::Number(number)) => *number,
                            // JavaScript would carry `undefined` into the
                            // comparisons, where every relational operator is
                            // false. `NaN` is the exact f64 equivalent, and
                            // `js_diff`/`js_min` are built to preserve it.
                            _ => f64::NAN,
                        },
                    },
                };
                Ok(ResultRow {
                    config,
                    compiled,
                    evidence,
                })
            })
            .collect::<Result<Vec<_>, String>>()?;
        Ok(Report { stem, results, raw })
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use search_compiler_modes::parse_json;

    fn row(text: &str) -> ResultRow {
        let json = parse_json(text).expect("fixture parses");
        let report = parse_json(&format!(
            "{{\"stem\":\"s\",\"results\":[{}]}}",
            canonical_json(&json)
        ))
        .expect("wrapper parses");
        Report::from_json(report)
            .expect("report parses")
            .results
            .remove(0)
    }

    #[test]
    fn null_evidence_is_not_missing_evidence() {
        assert_eq!(
            row(r#"{"config":{"ids":[]},"compiled":true,"evidence":null}"#).evidence,
            Evidence::Null
        );
        assert_eq!(
            row(r#"{"config":{"ids":[]},"compiled":true}"#).evidence,
            Evidence::Missing
        );
    }

    #[test]
    fn compiled_is_a_truthiness_test() {
        assert!(row(r#"{"config":{"ids":[]},"compiled":1}"#).compiled);
        assert!(!row(r#"{"config":{"ids":[]},"compiled":0}"#).compiled);
        assert!(!row(r#"{"config":{"ids":[]}}"#).compiled);
    }

    #[test]
    fn config_key_preserves_source_key_order() {
        let a = row(r#"{"config":{"ids":[],"flags":[]},"compiled":true}"#);
        let b = row(r#"{"config":{"flags":[],"ids":[]},"compiled":true}"#);
        // canonicalJson does not sort keys, so these are genuinely different
        // grouping keys in the TypeScript. A struct-shaped port would merge
        // them.
        assert_ne!(a.key(), b.key());
    }

    #[test]
    fn routed_baseline_needs_all_four_conditions() {
        let base = r#"{"config":{"ids":[],"flags":[],"remove_flags":[],"compiler_family":"routed"},"compiled":true}"#;
        assert!(row(base).is_routed_baseline().unwrap());
        let other = r#"{"config":{"ids":[],"flags":[],"remove_flags":[],"compiler_family":"gcc296"},"compiled":true}"#;
        assert!(!row(other).is_routed_baseline().unwrap());
        let flagged = r#"{"config":{"ids":[],"flags":["-O1"],"remove_flags":[],"compiler_family":"routed"},"compiled":true}"#;
        assert!(!row(flagged).is_routed_baseline().unwrap());
    }
}
