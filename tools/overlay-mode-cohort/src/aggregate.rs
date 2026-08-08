//! The two aggregations and the report shape.

use crate::collate::collate;
use crate::json::array_of_strings;
use crate::plan::Candidate;
use crate::score::Score;
use mode_sweep::Config;
use search_compiler_modes::{canonical_json, Json};
use std::cmp::Ordering;

/// One rung of a JavaScript `a || b || c` comparator chain.
///
/// PORT NOTE -- NaN FALLS THROUGH, AND THAT IS NOT A ROUNDING DETAIL.
/// `Infinity - Infinity` is NaN, JavaScript's `||` treats NaN as FALSY, so the
/// chain moves to the next rung exactly as it would for a 0. Both
/// `f64::partial_cmp` and `f64::total_cmp` get this wrong: `partial_cmp`
/// returns `None` (which a `.unwrap_or(Equal)` would coincidentally rescue,
/// but only by accident) and `total_cmp` orders NaN as the LARGEST value,
/// which would put the poisoned row first.
///
/// The ECMAScript sort spec also says that if the comparator's result `v` is
/// NaN, `+0` is returned, so a chain that ends on a NaN is a tie and the sort's
/// stability decides. That is reproduced by returning `Equal` at the end.
pub fn js_or(difference: f64) -> Option<Ordering> {
    if difference.is_nan() || difference == 0.0 {
        return None;
    }
    if difference < 0.0 {
        Some(Ordering::Less)
    } else {
        Some(Ordering::Greater)
    }
}

/// One row of `shared`.
pub struct ConfigSummary {
    pub config: Config,
    pub attempted: usize,
    pub compiled: usize,
    pub exact: usize,
    pub exact_regions: Vec<String>,
    pub improves_baseline_regions: Vec<String>,
    pub improvement_bytes: f64,
    pub total_differing_bytes: f64,
}

impl ConfigSummary {
    /// The JSON form in TypeScript object-literal order, which `canonicalJson`
    /// preserves and the report is compared on.
    pub fn to_json(&self) -> Json {
        Json::Object(vec![
            ("config".into(), self.config.to_json()),
            ("attempted".into(), Json::Number(self.attempted as f64)),
            ("compiled".into(), Json::Number(self.compiled as f64)),
            ("exact".into(), Json::Number(self.exact as f64)),
            ("exact_regions".into(), array_of_strings(&self.exact_regions)),
            (
                "improves_baseline_regions".into(),
                array_of_strings(&self.improves_baseline_regions),
            ),
            ("improvement_bytes".into(), Json::Number(self.improvement_bytes)),
            (
                "total_differing_bytes".into(),
                Json::Number(self.total_differing_bytes),
            ),
        ])
    }
}

/// `byConfig`, including its five-rung sort.
pub fn by_config(configs: &[Config], scores: &[Score]) -> Vec<ConfigSummary> {
    // `new Map(scores.filter(ids.length === 0).map(row => [row.id, row]))`:
    // LAST row per id wins, and there is one per id because the routed default
    // appears once per candidate.
    let mut baseline: Vec<(String, &Score)> = Vec::new();
    for score in scores.iter() {
        let empty = match score.config().and_then(|config| config.get("ids")) {
            Some(Json::Array(ids)) => ids.is_empty(),
            _ => false,
        };
        if !empty {
            continue;
        }
        match baseline.iter_mut().find(|(id, _)| id == score.id()) {
            Some(slot) => slot.1 = score,
            None => baseline.push((score.id().to_string(), score)),
        }
    }
    let floor_of = |id: &str| -> Option<&Score> {
        baseline
            .iter()
            .find(|(key, _)| key == id)
            .map(|(_, score)| *score)
    };

    let mut summaries: Vec<ConfigSummary> = configs
        .iter()
        .map(|config| {
            let key = canonical_json(&config.to_json());
            let rows: Vec<&Score> = scores
                .iter()
                .filter(|score| score.config_key() == key)
                .collect();
            let compiled: Vec<&Score> = rows.iter().copied().filter(|row| row.compiled()).collect();
            let exact: Vec<&Score> = compiled.iter().copied().filter(|row| row.exact()).collect();
            let improved: Vec<&Score> = compiled
                .iter()
                .copied()
                .filter(|row| match floor_of(row.id()) {
                    // `floor?.compiled === true`: an ABSENT baseline is not an
                    // improvement, and neither is an uncompiled one.
                    Some(floor) => {
                        floor.compiled() && row.differing_or_infinity() < floor.differing_or_infinity()
                    }
                    None => false,
                })
                .collect();
            let improvement_bytes = improved.iter().fold(0.0f64, |sum, row| {
                // Two non-null assertions; see `Score::differing_asserted`.
                let floor = floor_of(row.id())
                    .map(|floor| floor.differing_asserted())
                    .unwrap_or(f64::NAN);
                sum + (floor - row.differing_asserted())
            });
            let total_differing_bytes = compiled
                .iter()
                .fold(0.0f64, |sum, row| sum + row.differing_or_zero());
            ConfigSummary {
                config: config.clone(),
                attempted: rows.len(),
                compiled: compiled.len(),
                exact: exact.len(),
                exact_regions: exact.iter().map(|row| row.id().to_string()).collect(),
                improves_baseline_regions: improved
                    .iter()
                    .map(|row| row.id().to_string())
                    .collect(),
                improvement_bytes,
                total_differing_bytes,
            }
        })
        .collect();
    summaries.sort_by(|left, right| {
        js_or(right.exact as f64 - left.exact as f64)
            .or_else(|| {
                js_or(
                    right.improves_baseline_regions.len() as f64
                        - left.improves_baseline_regions.len() as f64,
                )
            })
            .or_else(|| js_or(right.improvement_bytes - left.improvement_bytes))
            .or_else(|| js_or(left.total_differing_bytes - right.total_differing_bytes))
            // The last rung is `localeCompare`, which returns -1/0/1 directly
            // rather than a difference, so it does not go through `js_or`.
            .unwrap_or_else(|| collate(&left.config.key(), &right.config.key()))
    });
    summaries
}

/// `bestByRegion`.
///
/// PORT NOTE -- THE SECOND RUNG IS THE `Infinity - Infinity` SITE. Two rows
/// that both lack `differing_bytes` compare NaN, the `||` treats that as a tie,
/// and the STABLE sort leaves them in `scores` order. Sorting with `total_cmp`
/// would instead push them to the end and pick a different winner.
pub fn best_by_region<'a>(candidates: &[Candidate], scores: &'a [Score]) -> Vec<Option<&'a Score>> {
    candidates
        .iter()
        .map(|candidate| {
            let mut rows: Vec<&Score> = scores
                .iter()
                .filter(|row| row.id() == candidate.id && row.compiled())
                .collect();
            rows.sort_by(|left, right| {
                // `Number(right.exact) - Number(left.exact)`: booleans coerce
                // to 1 and 0, so `true` sorts first.
                js_or(f64::from(right.exact()) - f64::from(left.exact()))
                    .or_else(|| {
                        js_or(left.differing_or_infinity() - right.differing_or_infinity())
                    })
                    .unwrap_or(Ordering::Equal)
            });
            rows.first().copied()
        })
        .collect()
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::json::set;

    fn score(id: &str, ids: &[&str], compiled: bool, exact: bool, differing: Option<f64>) -> Score {
        let mut json = Json::Object(vec![
            ("id".into(), Json::String(id.into())),
            (
                "config".into(),
                Config {
                    ids: ids.iter().map(|i| i.to_string()).collect(),
                    flags: vec![],
                    remove_flags: vec![],
                    compiler_family: "gcc296".into(),
                }
                .to_json(),
            ),
            ("compiled".into(), Json::Bool(compiled)),
            ("exact".into(), Json::Bool(exact)),
        ]);
        if let Some(value) = differing {
            set(&mut json, "differing_bytes", Json::Number(value));
        }
        Score { json }
    }

    #[test]
    fn nan_falls_through_the_comparator_chain() {
        assert_eq!(js_or(f64::INFINITY - f64::INFINITY), None);
        assert_eq!(js_or(0.0), None);
        assert_eq!(js_or(-0.0), None);
        assert_eq!(js_or(-3.0), Some(Ordering::Less));
        assert_eq!(js_or(3.0), Some(Ordering::Greater));
        // What the two tempting Rust idioms would have said instead.
        let nan = f64::NAN;
        assert_eq!(nan.partial_cmp(&0.0), None);
        assert_eq!(nan.total_cmp(&0.0), Ordering::Greater);
    }

    #[test]
    fn best_by_region_keeps_input_order_when_both_rows_lack_a_byte_count() {
        let candidates = vec![Candidate {
            id: "a:0000".into(),
            overlay: "o".into(),
            entry: 0.0,
            offset: 0.0,
            span_bytes: 0.0,
            source: "x.c".into(),
        }];
        let scores = vec![
            score("a:0000", &["first"], true, false, None),
            score("a:0000", &["second"], true, false, None),
        ];
        let best = best_by_region(&candidates, &scores);
        let winner = best[0].expect("a compiled row");
        // Both compare Infinity - Infinity = NaN, which is a tie, so the stable
        // sort keeps `first`.
        assert!(canonical_json(winner.config().unwrap()).contains("first"));
    }

    #[test]
    fn best_by_region_prefers_exact_then_fewest_differing_bytes() {
        let candidates = vec![Candidate {
            id: "a:0000".into(),
            overlay: "o".into(),
            entry: 0.0,
            offset: 0.0,
            span_bytes: 0.0,
            source: "x.c".into(),
        }];
        let scores = vec![
            score("a:0000", &["wide"], true, false, Some(9.0)),
            score("a:0000", &["narrow"], true, false, Some(2.0)),
            score("a:0000", &["broken"], false, false, Some(0.0)),
        ];
        let best = best_by_region(&candidates, &scores);
        assert!(canonical_json(best[0].unwrap().config().unwrap()).contains("narrow"));
        let with_exact = {
            let mut rows = scores.clone();
            rows.push(score("a:0000", &["hit"], true, true, Some(0.0)));
            rows
        };
        let best = best_by_region(&candidates, &with_exact);
        assert!(canonical_json(best[0].unwrap().config().unwrap()).contains("hit"));
    }

    #[test]
    fn a_missing_baseline_is_never_an_improvement() {
        let configs = vec![Config {
            ids: vec!["one".into()],
            flags: vec![],
            remove_flags: vec![],
            compiler_family: "gcc296".into(),
        }];
        // No routed-default row at all, so `baseline.get(id)` is undefined and
        // `floor?.compiled === true` is false.
        let scores = vec![score("a:0000", &["one"], true, false, Some(1.0))];
        let summaries = by_config(&configs, &scores);
        assert_eq!(summaries[0].improves_baseline_regions.len(), 0);
        assert_eq!(summaries[0].improvement_bytes, 0.0);
        assert_eq!(summaries[0].total_differing_bytes, 1.0);
    }

    #[test]
    fn improvement_bytes_goes_nan_when_the_baseline_has_no_byte_count() {
        let configs = vec![Config {
            ids: vec!["one".into()],
            flags: vec![],
            remove_flags: vec![],
            compiler_family: "gcc296".into(),
        }];
        let scores = vec![
            // A compiled baseline with NO differing_bytes: `?? Infinity` makes
            // it beatable, and `!` then produces `undefined - 1` = NaN.
            score("a:0000", &[], true, false, None),
            score("a:0000", &["one"], true, false, Some(1.0)),
        ];
        let summaries = by_config(&configs, &scores);
        assert_eq!(summaries[0].improves_baseline_regions, vec!["a:0000".to_string()]);
        assert!(
            summaries[0].improvement_bytes.is_nan(),
            "the two non-null assertions must produce NaN, not 0"
        );
    }
}
