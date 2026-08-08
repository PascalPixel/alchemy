//! `Score`, `acceptedScore` and `writeJson`.
//!
//! A score is carried as an ORDERED `Json::Object` rather than a struct. Two
//! reasons, both load-bearing:
//!
//!  * `{ ...cached, cached: true }` must preserve the on-disk key order, and a
//!    struct with a fixed `to_json` would silently rewrite a cached row into
//!    the current field order. Every cached row would then differ from the
//!    TypeScript's byte-for-byte in the report.
//!  * `acceptedScore` validates FIVE fields and passes everything else through
//!    untouched, including keys this version has never heard of. A struct would
//!    drop them.

use crate::json::{as_bool, as_number, as_str, get, set};
use search_compiler_modes::{canonical_json, Json};
use std::io;
use std::path::Path;

#[derive(Debug, Clone)]
pub struct Score {
    pub json: Json,
}

impl Score {
    pub fn id(&self) -> &str {
        as_str(get(&self.json, "id")).unwrap_or_default()
    }

    pub fn compiled(&self) -> bool {
        as_bool(get(&self.json, "compiled")).unwrap_or(false)
    }

    pub fn exact(&self) -> bool {
        as_bool(get(&self.json, "exact")).unwrap_or(false)
    }

    pub fn config(&self) -> Option<&Json> {
        get(&self.json, "config")
    }

    pub fn config_key(&self) -> String {
        match self.config() {
            Some(config) => canonical_json(config),
            // `canonicalJson(undefined)` is `JSON.stringify(undefined)`, which
            // is the JS value `undefined`, not a string. Unreachable for rows
            // this tool writes; kept total so a hand-edited cache file cannot
            // panic the aggregation.
            None => String::new(),
        }
    }

    /// `row.differing_bytes ?? Infinity`.
    pub fn differing_or_infinity(&self) -> f64 {
        crate::json::number_or(get(&self.json, "differing_bytes"), f64::INFINITY)
    }

    /// `row.differing_bytes ?? 0`.
    pub fn differing_or_zero(&self) -> f64 {
        crate::json::number_or(get(&self.json, "differing_bytes"), 0.0)
    }

    /// `row.differing_bytes!`, the NON-NULL ASSERTION.
    ///
    /// PORT NOTE -- THIS RETURNS NaN, DELIBERATELY. `improvement_bytes` is
    /// summed as `baseline.get(row.id)!.differing_bytes! - row.differing_bytes!`
    /// with two non-null assertions the type checker cannot honour at runtime.
    /// If either side is absent, TypeScript computes `undefined - number`, which
    /// is NaN, and the NaN then poisons the whole `reduce` and falls through the
    /// `||` comparator chain (see `aggregate::js_or`). An `Option` port that
    /// treated a missing value as 0 would sort differently.
    pub fn differing_asserted(&self) -> f64 {
        as_number(get(&self.json, "differing_bytes")).unwrap_or(f64::NAN)
    }
}

/// `acceptedScore(value, key)`.
///
/// PORT NOTE: `typeof value !== "object"` also admits arrays and rejects
/// `null` separately, which is why the `null` test comes first in the
/// TypeScript. Here a non-object `Json` simply fails the field lookups, and an
/// array fails them too because `get` only searches objects.
pub fn accepted_score(value: &Json, key: &str) -> Option<Score> {
    if !matches!(value, Json::Object(_)) {
        return None;
    }
    if as_str(get(value, "cache_key")) != Some(key) {
        return None;
    }
    as_str(get(value, "id"))?;
    as_bool(get(value, "compiled"))?;
    as_bool(get(value, "exact"))?;
    as_number(get(value, "expected_size"))?;
    Some(Score { json: value.clone() })
}

/// `{ ...cached, cached: true }`.
pub fn mark_cached(mut score: Score) -> Score {
    set(&mut score.json, "cached", Json::Bool(true));
    score
}

/// `writeJson(path, value)`: canonical text plus a newline, written to a
/// pid-suffixed temporary and renamed into place.
///
/// PORT NOTE: the rename is the atomicity guarantee, and the pid is what keeps
/// two concurrent processes from writing the same temporary. The port keeps
/// both. `rename` is atomic within a filesystem on macOS and Linux alike.
pub fn write_json(path: &Path, value: &Json) -> io::Result<()> {
    if let Some(parent) = path.parent() {
        std::fs::create_dir_all(parent)?;
    }
    let temporary = path.with_file_name(format!(
        "{}.{}.tmp",
        path.file_name().unwrap_or_default().to_string_lossy(),
        std::process::id()
    ));
    std::fs::write(&temporary, canonical_json(value) + "\n")?;
    std::fs::rename(&temporary, path)?;
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    fn row(key: &str) -> Json {
        Json::Object(vec![
            ("id".into(), Json::String("x:0000".into())),
            ("cache_key".into(), Json::String(key.into())),
            ("cached".into(), Json::Bool(false)),
            ("compiled".into(), Json::Bool(true)),
            ("exact".into(), Json::Bool(false)),
            ("expected_size".into(), Json::Number(2.0)),
        ])
    }

    #[test]
    fn a_row_is_accepted_only_under_its_own_key() {
        assert!(accepted_score(&row("k"), "k").is_some());
        assert!(accepted_score(&row("k"), "kx").is_none());
        assert!(accepted_score(&Json::Null, "k").is_none());
        assert!(accepted_score(&Json::Array(vec![]), "k").is_none());
    }

    #[test]
    fn a_missing_or_mistyped_field_rejects_the_row() {
        let mut broken = row("k");
        set(&mut broken, "compiled", Json::String("yes".into()));
        assert!(accepted_score(&broken, "k").is_none());
        let mut nan_written_back = row("k");
        // `JSON.stringify(NaN)` is `null`, and `null` is not a number.
        set(&mut nan_written_back, "expected_size", Json::Null);
        assert!(accepted_score(&nan_written_back, "k").is_none());
    }

    #[test]
    fn unknown_keys_survive_the_round_trip_in_order() {
        let mut extended = row("k");
        set(&mut extended, "future_field", Json::Number(7.0));
        let score = accepted_score(&extended, "k").expect("accepted");
        let marked = mark_cached(score);
        let keys: Vec<String> = match &marked.json {
            Json::Object(entries) => entries.iter().map(|(k, _)| k.clone()).collect(),
            _ => unreachable!(),
        };
        assert_eq!(keys.last().map(String::as_str), Some("future_field"));
        assert_eq!(as_bool(get(&marked.json, "cached")), Some(true));
    }

    #[test]
    fn the_non_null_assertion_yields_nan_not_zero() {
        let score = Score { json: row("k") };
        assert!(score.differing_asserted().is_nan());
        assert_eq!(score.differing_or_zero(), 0.0);
        assert_eq!(score.differing_or_infinity(), f64::INFINITY);
    }
}
