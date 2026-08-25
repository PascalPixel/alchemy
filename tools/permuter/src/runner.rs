use std::collections::{BTreeSet, HashSet};
use std::fs;
use std::path::{Component, Path, PathBuf};
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::{mpsc, Arc};

use compiler_core::routing::root;
use serde_json::json;

use crate::compile::{Score, Target};
use crate::options::{Options, MAX_SOURCE_BYTES};
use crate::Permutation;

const MAX_PLAN_BYTES: usize = 128 * 1024 * 1024;
const MAX_OUTPUT_BYTES: usize = 32 * 1024 * 1024;

#[derive(Clone, Debug)]
struct Candidate {
    index: usize,
    choice: usize,
    source: String,
    fingerprint: String,
}

#[derive(Debug)]
struct Evaluation {
    candidate: usize,
    score: Result<Score, String>,
}

fn expand_c_includes(
    path: &Path,
    source: &str,
    active: &mut BTreeSet<PathBuf>,
) -> Result<String, String> {
    let canonical =
        fs::canonicalize(path).map_err(|error| format!("{}: {error}", path.display()))?;
    if !active.insert(canonical.clone()) {
        return Err(format!(
            "recursive C include through {}",
            canonical.display()
        ));
    }
    let mut expanded = String::new();
    for line in source.lines() {
        let include = line
            .trim()
            .strip_prefix("#include")
            .map(str::trim)
            .and_then(|value| value.strip_prefix('"'))
            .and_then(|value| value.split_once('"').map(|(name, _)| name))
            .filter(|name| name.ends_with(".c"));
        if let Some(include) = include {
            let included = canonical.parent().unwrap_or(Path::new("")).join(include);
            let body = fs::read_to_string(&included)
                .map_err(|error| format!("{}: {error}", included.display()))?;
            expanded.push_str(&expand_c_includes(&included, &body, active)?);
        } else {
            expanded.push_str(line);
            expanded.push('\n');
        }
    }
    active.remove(&canonical);
    Ok(expanded)
}

fn load(path: &Path) -> Result<(PathBuf, String), String> {
    let path = if path.is_absolute() {
        path.to_path_buf()
    } else {
        root().join(path)
    };
    if path.is_dir() {
        return Err(format!(
            "{} is a directory; pass a candidate C file",
            path.display()
        ));
    }
    let source =
        fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?;
    if source.len() > MAX_SOURCE_BYTES {
        return Err(format!(
            "{} exceeds the {MAX_SOURCE_BYTES}-byte source limit",
            path.display()
        ));
    }
    let source = expand_c_includes(&path, &source, &mut BTreeSet::new())?;
    if source.len() > MAX_SOURCE_BYTES {
        return Err(format!(
            "expanded {} exceeds the {MAX_SOURCE_BYTES}-byte source limit",
            path.display()
        ));
    }
    Ok((path, source))
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

fn plan(permutation: &Permutation, iterations: usize, seed: u64) -> Result<Vec<Candidate>, String> {
    let mut candidates = Vec::new();
    let mut seen = HashSet::new();
    let mut bytes = 0usize;
    for choice in choice_order(permutation.count(), iterations, seed) {
        let source = permutation.evaluate(choice)?;
        if source.len() > MAX_SOURCE_BYTES {
            return Err(format!(
                "choice {choice} exceeds the {MAX_SOURCE_BYTES}-byte source limit"
            ));
        }
        let fingerprint = compiler_core::sha256::hex(source.as_bytes());
        if !seen.insert(fingerprint.clone()) {
            continue;
        }
        bytes = bytes.saturating_add(source.len());
        if bytes > MAX_PLAN_BYTES {
            return Err(format!(
                "candidate sources exceed the {MAX_PLAN_BYTES}-byte plan limit"
            ));
        }
        candidates.push(Candidate {
            index: candidates.len(),
            choice,
            source,
            fingerprint,
        });
    }
    if candidates.is_empty() {
        return Err("permutation produced no candidates".into());
    }
    Ok(candidates)
}

fn evaluate(
    target: Arc<Target>,
    candidates: Arc<Vec<Candidate>>,
    jobs: usize,
) -> Result<Vec<Evaluation>, String> {
    let next = Arc::new(AtomicUsize::new(0));
    let (sender, receiver) = mpsc::channel();
    let mut workers = Vec::new();
    for _ in 0..jobs.min(candidates.len()).max(1) {
        let target = Arc::clone(&target);
        let candidates = Arc::clone(&candidates);
        let next = Arc::clone(&next);
        let sender = sender.clone();
        workers.push(std::thread::spawn(move || loop {
            let index = next.fetch_add(1, Ordering::Relaxed);
            let Some(candidate) = candidates.get(index) else {
                break;
            };
            let score = if candidate.choice == 0 {
                Ok(target.baseline())
            } else {
                target.compile(&candidate.source)
            };
            if sender
                .send(Evaluation {
                    candidate: index,
                    score,
                })
                .is_err()
            {
                break;
            }
        }));
    }
    drop(sender);
    let mut evaluations = receiver.into_iter().collect::<Vec<_>>();
    for worker in workers {
        if worker.join().is_err() {
            return Err("permuter compiler worker panicked".into());
        }
    }
    evaluations.sort_by_key(|evaluation| evaluation.candidate);
    Ok(evaluations)
}

fn safe_label(path: &Path) -> String {
    let mut label = path
        .file_stem()
        .and_then(|value| value.to_str())
        .unwrap_or("candidate")
        .chars()
        .map(|character| {
            if character.is_ascii_alphanumeric() || matches!(character, '.' | '-' | '_') {
                character
            } else {
                '_'
            }
        })
        .collect::<String>();
    label.truncate(64);
    if label.is_empty() || matches!(label.as_str(), "." | "..") {
        "candidate".into()
    } else {
        label
    }
}

fn output_path(options: &Options, source: &Path, source_text: &str, multiple: bool) -> PathBuf {
    let child = format!(
        "{}-{}-seed-{}",
        safe_label(source),
        compiler_core::sha256::hex(source_text.as_bytes()),
        options.seed
    );
    match &options.output {
        Some(parent) if multiple => parent.join(child),
        Some(path) => path.clone(),
        None => root().join("out/permuter").join(child),
    }
}

fn validate_output(path: &Path) -> Result<PathBuf, String> {
    let path = if path.is_absolute() {
        path.to_path_buf()
    } else {
        root().join(path)
    };
    if path
        .components()
        .any(|component| component == Component::ParentDir)
    {
        return Err(format!(
            "output path may not contain '..': {}",
            path.display()
        ));
    }
    let repository_output = root().join("out");
    let temporary_output = std::env::temp_dir();
    if path == repository_output
        || path == temporary_output
        || !(path.starts_with(&repository_output) || path.starts_with(&temporary_output))
    {
        return Err(format!(
            "output must be a dedicated child of out/ or {}",
            temporary_output.display()
        ));
    }
    if path.exists() {
        return Err(format!(
            "{} already exists; choose a fresh run directory",
            path.display()
        ));
    }
    Ok(path)
}

fn ranking(left: &(usize, &Score), right: &(usize, &Score)) -> std::cmp::Ordering {
    right
        .1
        .exact
        .cmp(&left.1.exact)
        .then_with(|| left.1.differing_halfwords.cmp(&right.1.differing_halfwords))
        .then_with(|| {
            left.1
                .actual_size
                .abs_diff(left.1.expected_size)
                .cmp(&right.1.actual_size.abs_diff(right.1.expected_size))
        })
        .then_with(|| left.0.cmp(&right.0))
}

fn save(
    output: &Path,
    candidates: &[Candidate],
    evaluations: &[Evaluation],
    options: &Options,
) -> Result<(usize, bool), String> {
    let failures = evaluations
        .iter()
        .filter(|evaluation| evaluation.score.is_err())
        .count();
    let mut ranked = evaluations
        .iter()
        .filter_map(|evaluation| {
            evaluation
                .score
                .as_ref()
                .ok()
                .map(|score| (evaluation.candidate, score))
        })
        .collect::<Vec<_>>();
    ranked.sort_by(ranking);
    ranked.truncate(options.top);
    let output_bytes = ranked
        .iter()
        .map(|(index, _)| candidates[*index].source.len())
        .sum::<usize>();
    if output_bytes > MAX_OUTPUT_BYTES {
        return Err(format!(
            "retained sources exceed the {MAX_OUTPUT_BYTES}-byte output limit"
        ));
    }
    fs::create_dir_all(output.parent().ok_or("output has no parent")?)
        .map_err(|error| format!("{}: {error}", output.display()))?;
    fs::create_dir(output).map_err(|error| format!("{}: {error}", output.display()))?;
    let results = ranked
        .iter()
        .enumerate()
        .map(|(rank, (index, score))| {
            let candidate = &candidates[*index];
            let name = format!("candidate-{rank:03}-diff-{}.c", score.differing_halfwords);
            fs::write(output.join(&name), &candidate.source)
                .map_err(|error| format!("{}: {error}", output.join(&name).display()))?;
            Ok(json!({
                "rank": rank,
                "candidate": candidate.index,
                "choice": candidate.choice,
                "source_fingerprint": candidate.fingerprint,
                "exact": score.exact,
                "differing_halfwords": score.differing_halfwords,
                "actual_size": score.actual_size,
                "expected_size": score.expected_size,
                "first_difference": score.first_difference,
                "source": name,
            }))
        })
        .collect::<Result<Vec<_>, String>>()?;
    let Some((best_index, best_score)) = ranked.first() else {
        return Err("every candidate failed to compile".into());
    };
    fs::write(output.join("best.c"), &candidates[*best_index].source)
        .map_err(|error| format!("{}: {error}", output.join("best.c").display()))?;
    let baseline = evaluations
        .iter()
        .find(|evaluation| candidates[evaluation.candidate].choice == 0)
        .and_then(|evaluation| evaluation.score.as_ref().ok())
        .ok_or("baseline result is missing")?;
    let report = serde_json::to_string_pretty(&json!({
        "format": 1,
        "mode": "explicit",
        "status": "complete",
        "seed": options.seed,
        "attempted": evaluations.len(),
        "compile_failures": failures,
        "baseline_differing_halfwords": baseline.differing_halfwords,
        "results": results,
    }))
    .map_err(|error| error.to_string())?
        + "\n";
    fs::write(output.join("report.json"), report)
        .map_err(|error| format!("{}: {error}", output.join("report.json").display()))?;
    Ok((best_score.differing_halfwords, best_score.exact))
}

fn run_one(options: &Options, input: &Path, multiple: bool) -> Result<(), String> {
    let (path, source) = load(input)?;
    let permutation = crate::perm::parse(&source)?;
    let candidates = plan(&permutation, options.iterations, options.seed)?;
    let output = validate_output(&output_path(options, &path, &source, multiple))?;
    let target = Arc::new(Target::prepare(&path, &candidates[0].source)?);
    let candidates = Arc::new(candidates);
    let evaluations = evaluate(target, Arc::clone(&candidates), options.jobs)?;
    if options.show_errors {
        for evaluation in &evaluations {
            if let Err(error) = &evaluation.score {
                eprintln!("candidate {}: {error}", evaluation.candidate);
            }
        }
    }
    let (best, exact) = save(&output, &candidates, &evaluations, options)?;
    let failures = evaluations
        .iter()
        .filter(|evaluation| evaluation.score.is_err())
        .count();
    println!(
        "done={} choices={} attempted={} failures={} best={} exact={} output={}",
        path.display(),
        permutation.count(),
        evaluations.len(),
        failures,
        best,
        exact,
        output.display()
    );
    Ok(())
}

pub fn run(options: Options) -> Result<(), String> {
    let multiple = options.candidates.len() > 1;
    if !options.continue_on_error {
        return options
            .candidates
            .iter()
            .try_for_each(|input| run_one(&options, input, multiple));
    }
    let mut failed = 0;
    for input in &options.candidates {
        if let Err(error) = run_one(&options, input, multiple) {
            eprintln!("{}: {error}", input.display());
            failed += 1;
        }
    }
    if failed == 0 {
        Ok(())
    } else {
        Err(format!("{failed} candidate inputs failed"))
    }
}

pub fn self_test() -> Result<(), String> {
    let order = choice_order(7, 7, 42);
    if order != choice_order(7, 7, 42)
        || order.first() != Some(&0)
        || order.iter().copied().collect::<HashSet<_>>().len() != 7
    {
        return Err("seeded choice order is not deterministic and complete".into());
    }
    let permutation = crate::perm::parse("PERM_GENERAL(a,b,a)")?;
    if plan(&permutation, 3, 1)?.len() != 2 {
        return Err("candidate plan did not deduplicate identical sources".into());
    }
    Ok(())
}
