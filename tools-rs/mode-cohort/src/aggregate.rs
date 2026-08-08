//! Port of the five exported aggregation functions in
//! `tools/compiler/mode_cohort.ts`.
//!
//! This layer is pure: it takes already-parsed `mode_sweep` reports and emits
//! report rows. It never compiles, never routes and never promotes, exactly as
//! the TypeScript header promises.

use crate::collate::{collate, sort_default};
use crate::jsops::{js_diff, js_min, OrderedMap};
use crate::report::{Evidence, Report, ResultRow};
use search_compiler_modes::Json;

/// `{...config, extra: value, ...}`.
///
/// PORT NOTE: object spread keeps the *original* position of a key that is
/// overwritten and appends only genuinely new keys. Since `canonicalJson` emits
/// one key per line in iteration order, rebuilding these rows from a fixed
/// struct layout would reorder the emitted JSON whenever a `mode_sweep` report
/// happens to serialise `config` differently.
fn spread(config: &Json, extras: Vec<(String, Json)>) -> Json {
    let mut entries = match config {
        Json::Object(entries) => entries.clone(),
        other => vec![("".to_string(), other.clone())],
    };
    for (key, value) in extras {
        match entries.iter_mut().find(|(existing, _)| *existing == key) {
            Some(slot) => slot.1 = value,
            None => entries.push((key, value)),
        }
    }
    Json::Object(entries)
}

fn strings(items: &[String]) -> Json {
    Json::Array(items.iter().map(|s| Json::String(s.clone())).collect())
}

fn ids_key(ids: &[String]) -> String {
    ids.join("+")
}

// ---------------------------------------------------------------------------
// sharedExactConfigurations
// ---------------------------------------------------------------------------

#[derive(Debug, Clone)]
struct SharedExact {
    ids: Vec<String>,
    ids_value: Json,
    flags: Json,
    remove_flags: Json,
    compiler_family: Json,
    exact_stems: Vec<String>,
}

fn config_member(config: &Json, key: &str) -> Result<Json, String> {
    config
        .get(key)
        .cloned()
        .ok_or_else(|| format!("config: missing {key}"))
}

/// Port of `sharedExactConfigurations`.
pub fn shared_exact_configurations(reports: &[Report]) -> Result<Json, String> {
    let mut grouped: OrderedMap<SharedExact> = OrderedMap::new();
    for report in reports {
        for result in &report.results {
            if !result.compiled {
                continue;
            }
            // `result.evidence?.exact`: optional chaining short circuits on
            // both `undefined` and `null`, so neither reaches the property.
            let exact = matches!(result.evidence, Evidence::Present { exact: true, .. });
            if !exact {
                continue;
            }
            let ids: Vec<String> = result.ids()?.into_iter().map(str::to_string).collect();
            let key = result.key();
            let ids_value = config_member(&result.config, "ids")?;
            let flags = config_member(&result.config, "flags")?;
            let remove_flags = config_member(&result.config, "remove_flags")?;
            let compiler_family = config_member(&result.config, "compiler_family")?;
            let row = grouped.entry_or_insert_with(&key, || SharedExact {
                ids,
                ids_value,
                flags,
                remove_flags,
                compiler_family,
                exact_stems: Vec::new(),
            });
            if !row.exact_stems.contains(&report.stem) {
                row.exact_stems.push(report.stem.clone());
            }
        }
    }

    let mut rows: Vec<SharedExact> = grouped
        .values()
        .filter(|row| row.exact_stems.len() > 1)
        .cloned()
        .collect();
    for row in &mut rows {
        sort_default(&mut row.exact_stems);
    }
    rows.sort_by(|left, right| {
        right
            .exact_stems
            .len()
            .cmp(&left.exact_stems.len())
            .then_with(|| collate(&ids_key(&left.ids), &ids_key(&right.ids)))
    });

    Ok(Json::Array(
        rows.into_iter()
            .map(|row| {
                Json::Object(vec![
                    ("ids".into(), row.ids_value),
                    ("flags".into(), row.flags),
                    ("remove_flags".into(), row.remove_flags),
                    ("compiler_family".into(), row.compiler_family),
                    ("exact_stems".into(), strings(&row.exact_stems)),
                ])
            })
            .collect(),
    ))
}

// ---------------------------------------------------------------------------
// Shared scan used by the improvement aggregations
// ---------------------------------------------------------------------------

#[derive(Debug, Clone)]
struct ConfigGroup {
    config: Json,
    ids: Vec<String>,
    family: Option<Json>,
    scores: OrderedMap<f64>,
}

struct Scan {
    baselines: OrderedMap<f64>,
    configs: OrderedMap<ConfigGroup>,
}

fn scan(reports: &[Report], singles_only: bool) -> Result<Scan, String> {
    let mut baselines: OrderedMap<f64> = OrderedMap::new();
    let mut configs: OrderedMap<ConfigGroup> = OrderedMap::new();
    for report in reports {
        for result in &report.results {
            if !result.compiled {
                continue;
            }
            let differing = match result.evidence {
                Evidence::Missing => continue,
                // `result.evidence === undefined` is false for `null`, so the
                // TypeScript proceeds and throws on `null.differing_halfwords`.
                Evidence::Null => {
                    return Err("evidence is null: TypeError reading differing_halfwords".into())
                }
                Evidence::Present {
                    differing_halfwords,
                    ..
                } => differing_halfwords,
            };
            let baseline = result.is_routed_baseline()?;
            if baseline {
                baselines.set(&report.stem, differing);
            }
            // `singleModeEffects` uses `else if`; the other two do not, so the
            // baseline row is also a configuration group there.
            if singles_only && (baseline || result.ids()?.len() != 1) {
                continue;
            }
            insert_score(&mut configs, result, &report.stem, differing)?;
        }
    }
    if baselines.len() != reports.len() {
        return Err("mode cohort: every report must contain a compiled routed baseline".into());
    }
    Ok(Scan { baselines, configs })
}

fn insert_score(
    configs: &mut OrderedMap<ConfigGroup>,
    result: &ResultRow,
    stem: &str,
    differing: f64,
) -> Result<(), String> {
    let key = result.key();
    let ids: Vec<String> = result.ids()?.into_iter().map(str::to_string).collect();
    let family = result.config.get("compiler_family").cloned();
    let config = result.config.clone();
    let row = configs.entry_or_insert_with(&key, || ConfigGroup {
        config,
        ids,
        family,
        scores: OrderedMap::new(),
    });
    row.scores.set(stem, differing);
    Ok(())
}

/// The "beats every compiled proper subset from the same compiler family"
/// bound, shared verbatim by both improvement aggregations.
fn best_proper_subset(
    configs: &OrderedMap<ConfigGroup>,
    config: &ConfigGroup,
    stem: &str,
    baseline: f64,
) -> f64 {
    let subset_scores: Vec<f64> = configs
        .values()
        .filter(|candidate| {
            // `===` between two `compiler_family` values, including the
            // `undefined === undefined` case when neither config has the key.
            candidate.family == config.family
                && candidate.ids.len() < config.ids.len()
                && candidate.ids.iter().all(|id| config.ids.contains(id))
        })
        .filter_map(|candidate| candidate.scores.get(stem).copied())
        .collect();
    js_min(baseline, &subset_scores)
}

struct ScoreChange {
    stem: String,
    baseline_halfwords: f64,
    result_halfwords: f64,
    removed_halfwords: f64,
}

fn score_changes_json(changes: &mut [ScoreChange]) -> Json {
    // `sort((left, right) => left.stem.localeCompare(right.stem))` -- collated,
    // unlike the `.sort()` calls on the stem lists ten lines above it.
    changes.sort_by(|left, right| collate(&left.stem, &right.stem));
    Json::Array(
        changes
            .iter()
            .map(|change| {
                Json::Object(vec![
                    ("stem".into(), Json::String(change.stem.clone())),
                    (
                        "baseline_halfwords".into(),
                        Json::Number(change.baseline_halfwords),
                    ),
                    (
                        "result_halfwords".into(),
                        Json::Number(change.result_halfwords),
                    ),
                    (
                        "removed_halfwords".into(),
                        Json::Number(change.removed_halfwords),
                    ),
                ])
            })
            .collect(),
    )
}

// ---------------------------------------------------------------------------
// sharedNonRegressingImprovements
// ---------------------------------------------------------------------------

struct SharedImprovement {
    config: Json,
    ids: Vec<String>,
    improved: Vec<String>,
    unchanged: Vec<String>,
    changes: Vec<ScoreChange>,
    removed: f64,
}

/// Port of `sharedNonRegressingImprovements`.
pub fn shared_non_regressing_improvements(reports: &[Report]) -> Result<Json, String> {
    let Scan { baselines, configs } = scan(reports, false)?;
    let mut found: Vec<SharedImprovement> = Vec::new();
    for config in configs.values() {
        let mut improved: Vec<String> = Vec::new();
        let mut unchanged: Vec<String> = Vec::new();
        let mut changes: Vec<ScoreChange> = Vec::new();
        let mut removed = 0.0f64;
        let mut worsened = false;
        for report in reports {
            let baseline = *baselines.get(&report.stem).expect("baseline count checked");
            let score = config.scores.get(&report.stem).copied();
            // A configuration that fails to compile one cohort member is not
            // shared. NaN never satisfies `>`, matching JavaScript.
            let Some(score) = score else {
                worsened = true;
                break;
            };
            if score > baseline {
                worsened = true;
                break;
            }
            if score < baseline {
                let bound = best_proper_subset(&configs, config, &report.stem, baseline);
                if score < bound {
                    improved.push(report.stem.clone());
                    let removed_here = baseline - score;
                    removed += removed_here;
                    changes.push(ScoreChange {
                        stem: report.stem.clone(),
                        baseline_halfwords: baseline,
                        result_halfwords: score,
                        removed_halfwords: removed_here,
                    });
                } else {
                    unchanged.push(report.stem.clone());
                }
            } else {
                unchanged.push(report.stem.clone());
            }
        }
        if !worsened && improved.len() > 1 {
            found.push(SharedImprovement {
                config: config.config.clone(),
                ids: config.ids.clone(),
                improved,
                unchanged,
                changes,
                removed,
            });
        }
    }

    found.sort_by(|left, right| {
        right
            .improved
            .len()
            .cmp(&left.improved.len())
            .then_with(|| js_diff(right.removed, left.removed))
            .then_with(|| collate(&ids_key(&left.ids), &ids_key(&right.ids)))
    });

    Ok(Json::Array(
        found
            .into_iter()
            .map(|mut row| {
                sort_default(&mut row.improved);
                sort_default(&mut row.unchanged);
                spread(
                    &row.config,
                    vec![
                        ("improved_stems".into(), strings(&row.improved)),
                        ("unchanged_stems".into(), strings(&row.unchanged)),
                        ("score_changes".into(), score_changes_json(&mut row.changes)),
                        ("total_halfwords_removed".into(), Json::Number(row.removed)),
                    ],
                )
            })
            .collect(),
    ))
}

// ---------------------------------------------------------------------------
// irreducibleConfigurationImprovements / multiRegionImprovements
// ---------------------------------------------------------------------------

struct MultiRegion {
    config: Json,
    ids: Vec<String>,
    improved: Vec<String>,
    unchanged: Vec<String>,
    regressed: Vec<String>,
    changes: Vec<ScoreChange>,
    removed: f64,
    added: f64,
}

fn irreducible_rows(reports: &[Report]) -> Result<Vec<MultiRegion>, String> {
    let Scan { baselines, configs } = scan(reports, false)?;
    let mut found: Vec<MultiRegion> = Vec::new();
    for config in configs.values() {
        let mut improved: Vec<String> = Vec::new();
        let mut unchanged: Vec<String> = Vec::new();
        let mut regressed: Vec<String> = Vec::new();
        let mut changes: Vec<ScoreChange> = Vec::new();
        let mut removed = 0.0f64;
        let mut added = 0.0f64;
        let mut incomplete = false;
        for report in reports {
            let baseline = *baselines.get(&report.stem).expect("baseline count checked");
            let Some(score) = config.scores.get(&report.stem).copied() else {
                incomplete = true;
                break;
            };
            if score < baseline {
                let bound = best_proper_subset(&configs, config, &report.stem, baseline);
                if score < bound {
                    improved.push(report.stem.clone());
                    let removed_here = baseline - score;
                    removed += removed_here;
                    changes.push(ScoreChange {
                        stem: report.stem.clone(),
                        baseline_halfwords: baseline,
                        result_halfwords: score,
                        removed_halfwords: removed_here,
                    });
                } else {
                    unchanged.push(report.stem.clone());
                }
            } else if score > baseline {
                regressed.push(report.stem.clone());
                added += score - baseline;
            } else {
                // Also the landing place for a NaN score: neither `<` nor `>`
                // holds, so JavaScript calls it unchanged.
                unchanged.push(report.stem.clone());
            }
        }
        if !incomplete && !improved.is_empty() {
            found.push(MultiRegion {
                config: config.config.clone(),
                ids: config.ids.clone(),
                improved,
                unchanged,
                regressed,
                changes,
                removed,
                added,
            });
        }
    }
    found.sort_by(|left, right| {
        right
            .improved
            .len()
            .cmp(&left.improved.len())
            .then_with(|| js_diff(right.removed, left.removed))
            .then_with(|| left.regressed.len().cmp(&right.regressed.len()))
            .then_with(|| collate(&ids_key(&left.ids), &ids_key(&right.ids)))
    });
    Ok(found)
}

fn multi_region_json(rows: Vec<MultiRegion>) -> Json {
    Json::Array(
        rows.into_iter()
            .map(|mut row| {
                sort_default(&mut row.improved);
                sort_default(&mut row.unchanged);
                sort_default(&mut row.regressed);
                spread(
                    &row.config,
                    vec![
                        ("improved_stems".into(), strings(&row.improved)),
                        ("unchanged_stems".into(), strings(&row.unchanged)),
                        ("regressed_stems".into(), strings(&row.regressed)),
                        ("score_changes".into(), score_changes_json(&mut row.changes)),
                        ("total_halfwords_removed".into(), Json::Number(row.removed)),
                        ("total_halfwords_added".into(), Json::Number(row.added)),
                    ],
                )
            })
            .collect(),
    )
}

/// Port of `irreducibleConfigurationImprovements`.
pub fn irreducible_configuration_improvements(reports: &[Report]) -> Result<Json, String> {
    Ok(multi_region_json(irreducible_rows(reports)?))
}

/// Port of `multiRegionImprovements`.
pub fn multi_region_improvements(reports: &[Report]) -> Result<Json, String> {
    let rows = irreducible_rows(reports)?
        .into_iter()
        .filter(|row| row.improved.len() > 1)
        .collect();
    Ok(multi_region_json(rows))
}

/// `irreducible.filter((row) => row.ids.length > 1)`, the projection `main`
/// writes into the summary.
pub fn irreducible_multi_id(reports: &[Report]) -> Result<Json, String> {
    let rows = irreducible_rows(reports)?
        .into_iter()
        .filter(|row| row.ids.len() > 1)
        .collect();
    Ok(multi_region_json(rows))
}

// ---------------------------------------------------------------------------
// singleModeEffects
// ---------------------------------------------------------------------------

struct SingleEffect {
    config: Json,
    ids: Vec<String>,
    improved: Vec<String>,
    unchanged: Vec<String>,
    regressed: Vec<String>,
    removed: f64,
    added: f64,
}

/// Port of `singleModeEffects`.
pub fn single_mode_effects(reports: &[Report]) -> Result<Json, String> {
    let Scan { baselines, configs } = scan(reports, true)?;
    let mut effects: Vec<SingleEffect> = Vec::new();
    for config in configs.values() {
        if config.scores.len() != reports.len() {
            continue;
        }
        let mut improved: Vec<String> = Vec::new();
        let mut unchanged: Vec<String> = Vec::new();
        let mut regressed: Vec<String> = Vec::new();
        let mut removed = 0.0f64;
        let mut added = 0.0f64;
        for report in reports {
            let baseline = *baselines.get(&report.stem).expect("baseline count checked");
            let score = *config
                .scores
                .get(&report.stem)
                .expect("score count equals report count");
            if score < baseline {
                improved.push(report.stem.clone());
                removed += baseline - score;
            } else if score > baseline {
                regressed.push(report.stem.clone());
                added += score - baseline;
            } else {
                unchanged.push(report.stem.clone());
            }
        }
        effects.push(SingleEffect {
            config: config.config.clone(),
            ids: config.ids.clone(),
            improved,
            unchanged,
            regressed,
            removed,
            added,
        });
    }
    effects.sort_by(|left, right| {
        right
            .improved
            .len()
            .cmp(&left.improved.len())
            .then_with(|| js_diff(right.removed, left.removed))
            .then_with(|| left.regressed.len().cmp(&right.regressed.len()))
            .then_with(|| collate(&ids_key(&left.ids), &ids_key(&right.ids)))
    });
    Ok(Json::Array(
        effects
            .into_iter()
            .map(|mut row| {
                sort_default(&mut row.improved);
                sort_default(&mut row.unchanged);
                sort_default(&mut row.regressed);
                spread(
                    &row.config,
                    vec![
                        ("improved_stems".into(), strings(&row.improved)),
                        ("unchanged_stems".into(), strings(&row.unchanged)),
                        ("regressed_stems".into(), strings(&row.regressed)),
                        ("total_halfwords_removed".into(), Json::Number(row.removed)),
                        ("total_halfwords_added".into(), Json::Number(row.added)),
                    ],
                )
            })
            .collect(),
    ))
}

/// Every aggregation at once, in the order `main` computes them. This is the
/// unit the differential parity harness compares.
pub fn all_aggregations(reports: &[Report]) -> Result<Json, String> {
    Ok(Json::Object(vec![
        ("shared_exact".into(), shared_exact_configurations(reports)?),
        (
            "shared_improvements".into(),
            shared_non_regressing_improvements(reports)?,
        ),
        (
            "irreducible".into(),
            irreducible_configuration_improvements(reports)?,
        ),
        ("multi_region".into(), multi_region_improvements(reports)?),
        ("single_effects".into(), single_mode_effects(reports)?),
    ]))
}

/// Counts per category, so a parity gate cannot go green because a whole
/// aggregation silently stopped producing rows.
pub fn category_counts(value: &Json) -> Vec<(String, usize)> {
    match value {
        Json::Object(entries) => entries
            .iter()
            .map(|(key, rows)| (key.clone(), rows.as_array().map_or(0, <[Json]>::len)))
            .collect(),
        _ => Vec::new(),
    }
}

/// A DELIBERATELY WRONG variant used only by the parity harness's negative
/// control: it breaks the tie with `str::cmp` instead of the measured
/// collator. Nothing else may call this.
#[doc(hidden)]
pub fn shared_exact_configurations_ascii_tiebreak(reports: &[Report]) -> Result<Json, String> {
    let value = shared_exact_configurations(reports)?;
    let Json::Array(mut rows) = value else {
        return Ok(value);
    };
    rows.sort_by(|left, right| {
        let key = |row: &Json| -> (usize, String) {
            let stems = row
                .get("exact_stems")
                .and_then(Json::as_array)
                .map_or(0, <[Json]>::len);
            let ids = row
                .get("ids")
                .and_then(Json::as_array)
                .map(|items| {
                    items
                        .iter()
                        .filter_map(Json::as_str)
                        .collect::<Vec<_>>()
                        .join("+")
                })
                .unwrap_or_default();
            (stems, ids)
        };
        let (left_stems, left_ids) = key(left);
        let (right_stems, right_ids) = key(right);
        right_stems
            .cmp(&left_stems)
            .then_with(|| left_ids.cmp(&right_ids))
    });
    Ok(Json::Array(rows))
}
