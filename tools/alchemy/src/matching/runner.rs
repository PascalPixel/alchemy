use std::fs;
use std::path::{Component, Path, PathBuf};

use crate::compiler::routing::root;
use serde_json::json;

use super::compile::{Score, Target};
use super::options::{Options, MAX_SOURCE_BYTES};
use psynergy::repair::Permutation;

#[derive(Debug)]
struct Evaluation {
    order: usize,
    choice: usize,
    score: Result<Score, String>,
}

pub(crate) struct RunSummary {
    pub baseline_differing_halfwords: usize,
    pub repair: Option<String>,
    pub exact: bool,
    pub report: serde_json::Value,
    best: usize,
    failures: usize,
}

fn load(path: &Path) -> Result<(PathBuf, String), String> {
    let path = crate::compiler::build_io::rooted(root(), path);
    let source =
        fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?;
    if source.len() > MAX_SOURCE_BYTES {
        return Err(format!(
            "{} exceeds the {MAX_SOURCE_BYTES}-byte source limit",
            path.display()
        ));
    }
    Ok((path, source))
}

fn allocator_options(path: &Path, work: &Path) -> Result<Box<crate::score::cli::Options>, String> {
    let arguments = [
        path.to_string_lossy().into_owned(),
        "--owner".into(),
        super::compile::source_owner(path)?.id(),
        "--allocator-order".into(),
        "--work".into(),
        work.to_string_lossy().into_owned(),
    ];
    let crate::score::cli::ParseOutcome::Options(options) =
        crate::score::cli::options_of(root(), &arguments)?
    else {
        return Err("allocator decoder options unexpectedly requested help".into());
    };
    Ok(options)
}

#[test]
fn allocator_preflight_preserves_overlay_identity() {
    let source = root().join("games/gs1/src/overlays/scene/actor_transition_presentation_scene/run_scene_four_pass_callback_sequence.c");
    let work = tempfile::tempdir().unwrap();
    let options = allocator_options(&source, work.path()).unwrap();
    assert_eq!(options.owner, Some(0x02001e08));
    assert_eq!(options.overlay.as_deref(), Some("resource_39a"));
    assert!(options.allocator_order);
}

fn allocator_report(path: &Path) -> Result<crate::score::allocator::Report, String> {
    let work = tempfile::tempdir().map_err(|error| error.to_string())?;
    let options = allocator_options(path, work.path())?;
    let rendered = if options.overlay.is_some() {
        crate::overlay::score::render_options(root(), options)?
    } else {
        crate::score::render::render(root(), &options)?
    };
    let report = rendered
        .allocator
        .ok_or("allocator decoder produced no report")?;
    if report.dimensions.is_empty() || report.repair.is_none() {
        return Err(format!(
            "allocator decoder found no repair\n{}",
            report.text
        ));
    }
    Ok(report)
}

fn guard_call_via(source: &str) -> Result<(), String> {
    for register in 0..14 {
        let symbol = format!(
            "Func_{:08x}(",
            crate::compiler::symbols::CALL_VIA_BASE + register * 4
        );
        if source.contains(&symbol) {
            return Err(format!("semantic guard: {symbol} is a main-image call-via trampoline; model the typed indirect call first"));
        }
    }
    Ok(())
}

#[test]
fn golden_sun_trampolines_require_typed_indirect_calls() {
    assert!(guard_call_via("void f(void) { Func_080072e4(); }").is_err());
    assert!(guard_call_via("void f(void) { callback(); }").is_ok());
}

fn mix(mut value: u64) -> u64 {
    value = (value ^ (value >> 30)).wrapping_mul(0xbf58_476d_1ce4_e5b9);
    value = (value ^ (value >> 27)).wrapping_mul(0x94d0_49bb_1331_11eb);
    value ^ (value >> 31)
}

fn choice_order(count: usize, limit: usize, seed: u64) -> Vec<usize> {
    let limit = limit.min(count);
    if limit <= 1 {
        return (0..limit).collect();
    }
    let remaining = count - 1;
    let start = mix(seed) as u128 % remaining as u128;
    std::iter::once(0)
        .chain(
            (0..limit - 1)
                .map(|offset| 1 + ((start + offset as u128) % remaining as u128) as usize),
        )
        .collect()
}

fn evaluate(
    target: &Target,
    permutation: &Permutation,
    choices: &[usize],
    jobs: usize,
) -> Result<Vec<Evaluation>, String> {
    let workers = jobs.min(choices.len()).max(1);
    let mut evaluations = std::thread::scope(|scope| {
        let handles = (0..workers)
            .map(|worker| {
                scope.spawn(move || {
                    choices[worker..]
                        .iter()
                        .step_by(workers)
                        .enumerate()
                        .map(|(offset, &choice)| Evaluation {
                            order: worker + offset * workers,
                            choice,
                            score: if choice == 0 {
                                Ok(target.baseline())
                            } else {
                                permutation
                                    .evaluate(choice)
                                    .and_then(|source| target.compile(&source))
                            },
                        })
                        .collect::<Vec<_>>()
                })
            })
            .collect::<Vec<_>>();
        let mut results = Vec::new();
        for handle in handles {
            results.extend(handle.join().map_err(|_| "matching worker panicked")?);
        }
        Ok::<_, String>(results)
    })?;
    evaluations.sort_by_key(|evaluation| evaluation.order);
    Ok(evaluations)
}

fn validate_output(path: &Path) -> Result<PathBuf, String> {
    if path.components().any(|part| part == Component::ParentDir) {
        return Err("output path must not contain ..".into());
    }
    let path = crate::compiler::build_io::rooted(root(), path);
    let roots = [root().join("out"), std::env::temp_dir()];
    if path.exists()
        || !roots
            .iter()
            .any(|allowed| path.starts_with(allowed) && path != *allowed)
    {
        return Err(format!(
            "output must be a fresh child of out/ or the temporary directory: {}",
            path.display()
        ));
    }
    Ok(path)
}

fn save(
    output: &Path,
    evaluations: &[Evaluation],
    options: &Options,
    permutation: &Permutation,
    decoder: &crate::score::allocator::Report,
) -> Result<RunSummary, String> {
    let best = evaluations
        .iter()
        .filter_map(|evaluation| {
            evaluation
                .score
                .as_ref()
                .ok()
                .map(|score| (evaluation, score))
        })
        .min_by_key(|(evaluation, score)| {
            (
                !score.exact,
                score.differing_halfwords,
                score.actual_size.abs_diff(score.expected_size),
                evaluation.order,
            )
        })
        .ok_or("every candidate failed to compile")?;
    let (best_evaluation, best_score) = best;
    let best_choice = best_evaluation.choice;
    fs::create_dir_all(output.parent().ok_or("output has no parent")?)
        .map_err(|error| format!("{}: {error}", output.display()))?;
    fs::create_dir(output).map_err(|error| format!("{}: {error}", output.display()))?;
    fs::write(output.join("best.c"), permutation.evaluate(best_choice)?)
        .map_err(|error| format!("{}: {error}", output.join("best.c").display()))?;
    let results = evaluations
        .iter()
        .map(|evaluation| {
            let choice = evaluation.choice;
            let outcome = match &evaluation.score {
                Ok(score) => json!(score),
                Err(error) => json!({ "compile_error": error }),
            };
            json!({
                "choice": choice,
                "mutations": permutation.mutations(choice),
                "outcome": outcome,
            })
        })
        .collect::<Vec<_>>();
    let baseline = evaluations
        .iter()
        .find(|evaluation| evaluation.choice == 0)
        .and_then(|evaluation| evaluation.score.as_ref().ok())
        .ok_or("baseline result is missing")?;
    let repair = decoder.repair.as_ref().map(|repair| repair.label());
    let failures = evaluations
        .iter()
        .filter(|evaluation| evaluation.score.is_err())
        .count();
    let report = json!({
        "catalog_version": super::CATALOG_VERSION,
        "dimensions": decoder.dimensions,
        "decoder": {
            "repair": repair,
            "evidence_sha256": crate::compiler::sha256::hex(decoder.text.as_bytes()),
        },
        "raw_choices": permutation.raw_count(),
        "unique_choices": permutation.count(),
        "catalog_choices": (0..permutation.count()).filter_map(|choice| permutation.mutations(choice)).collect::<Vec<_>>(),
        "max_edits_per_candidate": decoder.repair.as_ref().map_or(0, |plan| plan.repairs().len()),
        "seed": options.seed,
        "compile_failures": failures,
        "baseline_differing_halfwords": baseline.differing_halfwords,
        "results": results,
    });
    let text = serde_json::to_string_pretty(&report).map_err(|error| error.to_string())? + "\n";
    fs::write(output.join("report.json"), text)
        .map_err(|error| format!("{}: {error}", output.join("report.json").display()))?;
    Ok(RunSummary {
        baseline_differing_halfwords: baseline.differing_halfwords,
        repair,
        exact: best_score.exact,
        report,
        best: best_score.differing_halfwords,
        failures,
    })
}

pub(crate) fn run(options: Options) -> Result<RunSummary, String> {
    let input = &options.candidate;
    let (path, source) = load(input)?;
    let decoder = allocator_report(&path)?;
    guard_call_via(&source)?;
    let permutation = psynergy::repair::enumerate(&source, decoder.repair.as_ref().unwrap())?;
    let choices = choice_order(permutation.count(), options.iterations, options.seed);
    let default_output = root().join("out/matching").join(format!(
        "{}-seed-{}",
        crate::compiler::sha256::hex(source.as_bytes()),
        options.seed
    ));
    let output = validate_output(options.output.as_deref().unwrap_or(&default_output))?;
    let target = Target::prepare(&path, &source)?;
    let evaluations = evaluate(&target, &permutation, &choices, options.jobs)?;
    let summary = save(&output, &evaluations, &options, &permutation, &decoder)?;
    println!(
        "done={} raw_choices={} unique_choices={} attempted={} failures={} best={} exact={} output={}",
        path.display(),
        permutation.raw_count(),
        permutation.count(),
        evaluations.len(),
        summary.failures,
        summary.best,
        summary.exact,
        output.display()
    );
    Ok(summary)
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn seed_rotates_evaluation_order() {
        assert_ne!(choice_order(8, 8, 1)[1], choice_order(8, 8, 2)[1]);
        assert_eq!(choice_order(8, 8, 1), choice_order(8, 8, 1));
    }
}
