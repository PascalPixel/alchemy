use std::fs;
use std::path::{Component, Path, PathBuf};

use compiler_core::routing::root;
use serde_json::json;

use crate::compile::{Score, Target};
use crate::options::{Options, MAX_SOURCE_BYTES};
use crate::Permutation;

#[derive(Debug)]
struct Evaluation {
    order: usize,
    choice: usize,
    score: Result<Score, String>,
}

fn load(path: &Path) -> Result<(PathBuf, String), String> {
    let path = if path.is_absolute() {
        path.to_path_buf()
    } else {
        root().join(path)
    };
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

fn allocator_report(path: &Path) -> Result<candidate_show::allocator::Report, String> {
    let work = tempfile::tempdir().map_err(|error| error.to_string())?;
    let arguments = [
        path.to_string_lossy().into_owned(),
        "--allocator-order".into(),
        "--work".into(),
        work.path().to_string_lossy().into_owned(),
    ];
    let candidate_show::cli::ParseOutcome::Options(options) =
        candidate_show::cli::options_of(root(), &arguments)?
    else {
        return Err("allocator decoder options unexpectedly requested help".into());
    };
    let report = candidate_show::render::render(root(), &options)?
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

fn mix(mut value: u64) -> u64 {
    value = (value ^ (value >> 30)).wrapping_mul(0xbf58_476d_1ce4_e5b9);
    value = (value ^ (value >> 27)).wrapping_mul(0x94d0_49bb_1331_11eb);
    value ^ (value >> 31)
}

fn choice_order(count: usize, limit: usize, seed: u64) -> Vec<usize> {
    let limit = limit.min(count);
    let mut order = Vec::with_capacity(limit);
    if limit == 0 {
        return order;
    }
    order.push(0);
    if limit == 1 || count == 1 {
        return order;
    }
    let remaining = count - 1;
    let start = mix(seed) as u128 % remaining as u128;
    order.extend(
        (0..limit - 1).map(|offset| 1 + ((start + offset as u128) % remaining as u128) as usize),
    );
    order
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
            results.extend(handle.join().map_err(|_| "permuter worker panicked")?);
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
    let path = if path.is_absolute() {
        path.to_path_buf()
    } else {
        root().join(path)
    };
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
    decoder: &candidate_show::allocator::Report,
) -> Result<(usize, bool), String> {
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
                Ok(score) => json!({
                    "exact": score.exact,
                    "differing_halfwords": score.differing_halfwords,
                    "actual_size": score.actual_size,
                    "expected_size": score.expected_size,
                    "first_difference": score.first_difference,
                }),
                Err(error) => json!({ "compile_error": error }),
            };
            Ok(json!({
                "choice": choice,
                "mutations": permutation.mutations(choice),
                "outcome": outcome,
            }))
        })
        .collect::<Result<Vec<_>, String>>()?;
    let baseline = evaluations
        .iter()
        .find(|evaluation| evaluation.choice == 0)
        .and_then(|evaluation| evaluation.score.as_ref().ok())
        .ok_or("baseline result is missing")?;
    let report = serde_json::to_string_pretty(&json!({
        "catalog_version": crate::perm::CATALOG_VERSION,
        "dimensions": decoder.dimensions,
        "decoder": {
            "repair": decoder.repair.as_ref().map(|repair| repair.label()),
            "evidence_sha256": compiler_core::sha256::hex(decoder.text.as_bytes()),
        },
        "raw_choices": permutation.raw_count(),
        "unique_choices": permutation.count(),
        "catalog_choices": (0..permutation.count()).filter_map(|choice| permutation.mutations(choice)).collect::<Vec<_>>(),
        "max_edits_per_candidate": decoder.repair.as_ref().map_or(0, |plan| plan.repairs().len()),
        "seed": options.seed,
        "compile_failures": evaluations.iter().filter(|evaluation| evaluation.score.is_err()).count(),
        "baseline_differing_halfwords": baseline.differing_halfwords,
        "results": results,
    }))
    .map_err(|error| error.to_string())?
        + "\n";
    fs::write(output.join("report.json"), report)
        .map_err(|error| format!("{}: {error}", output.join("report.json").display()))?;
    Ok((best_score.differing_halfwords, best_score.exact))
}

pub fn run(options: Options) -> Result<(), String> {
    let input = &options.candidate;
    let (path, source) = load(input)?;
    let decoder = allocator_report(&path)?;
    let permutation = crate::perm::parse(&source, decoder.repair.as_ref().unwrap())?;
    let choices = choice_order(permutation.count(), options.iterations, options.seed);
    let default_output = root().join("out/permuter").join(format!(
        "{}-seed-{}",
        compiler_core::sha256::hex(source.as_bytes()),
        options.seed
    ));
    let output = validate_output(options.output.as_deref().unwrap_or(&default_output))?;
    let target = Target::prepare(&path, &source)?;
    let evaluations = evaluate(&target, &permutation, &choices, options.jobs)?;
    let (best, exact) = save(&output, &evaluations, &options, &permutation, &decoder)?;
    let failures = evaluations
        .iter()
        .filter(|evaluation| evaluation.score.is_err())
        .count();
    println!(
        "done={} raw_choices={} unique_choices={} attempted={} failures={} best={} exact={} output={}",
        path.display(),
        permutation.raw_count(),
        permutation.count(),
        evaluations.len(),
        failures,
        best,
        exact,
        output.display()
    );
    Ok(())
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
