use std::cmp::Ordering;
use std::collections::{BTreeMap, BTreeSet};
use std::fs::{self, File, OpenOptions};
use std::io::Write;
use std::path::{Path, PathBuf};
use std::sync::atomic::{AtomicBool, AtomicUsize, Ordering as AtomicOrdering};
use std::sync::{mpsc, Arc, Mutex};
use std::time::{Duration, Instant};

use alchemy_routing::routing::root;

use crate::backend::{self, Backend, Measurement};
use crate::options::Options;
use crate::perm::Permutation;
use crate::randomize::Weights;

#[derive(Clone, Debug, Eq, PartialEq)]
struct Candidate {
    index: usize,
    manual_seed: usize,
    mutation: String,
    source: String,
    fingerprint: String,
}

#[derive(Debug)]
struct Evaluated {
    candidate: Candidate,
    measurement: Result<Measurement, String>,
    elapsed: Duration,
}

struct Journal {
    cached: BTreeMap<String, Measurement>,
    path: PathBuf,
    writer: Mutex<File>,
}

impl Journal {
    fn open(output: &Path, identity: &str, resume: bool) -> Result<Self, String> {
        fs::create_dir_all(output).map_err(|error| format!("{}: {error}", output.display()))?;
        let path = output.join("journal.tsv");
        let header = format!("alchemy-permuter-journal-v2\t{identity}");
        let mut cached = BTreeMap::new();
        let mut append = false;
        if resume {
            match fs::read_to_string(&path) {
                Ok(text) => {
                    let mut lines = text.lines();
                    if lines.next() == Some(header.as_str()) {
                        append = true;
                        for (line_number, line) in lines.enumerate() {
                            let fields = line.split('\t').collect::<Vec<_>>();
                            let Some((fingerprint, measurement)) = parse_journal_row(&fields)
                            else {
                                return Err(format!(
                                    "{}: invalid or incomplete row {}",
                                    path.display(),
                                    line_number + 2
                                ));
                            };
                            cached.insert(fingerprint, measurement);
                        }
                    }
                }
                Err(error) if error.kind() == std::io::ErrorKind::NotFound => {}
                Err(error) => {
                    return Err(format!("{}: {error}", path.display()));
                }
            }
        }
        let mut writer = OpenOptions::new()
            .create(true)
            .write(true)
            .append(append)
            .truncate(!append)
            .open(&path)
            .map_err(|error| format!("{}: {error}", path.display()))?;
        if !append {
            writeln!(writer, "{header}").map_err(|error| format!("{}: {error}", path.display()))?;
        }
        Ok(Self {
            cached,
            path,
            writer: Mutex::new(writer),
        })
    }

    fn cached(&self, candidate: &str) -> Option<Measurement> {
        self.cached.get(candidate).cloned()
    }

    fn record(&self, candidate: &str, measurement: &Measurement) -> Result<(), String> {
        let first = measurement
            .first_difference
            .map_or_else(|| "-".to_string(), |value| value.to_string());
        let summary = measurement.summary.replace('\t', "\\t").replace('\n', " ");
        let mut writer = self
            .writer
            .lock()
            .map_err(|_| format!("{}: journal lock is poisoned", self.path.display()))?;
        writeln!(
            writer,
            "{candidate}\t{}\t{}\t{}\t{}\t{}\t{}\t{:016x}\t{}",
            u8::from(measurement.exact),
            measurement.score,
            measurement.differences,
            measurement.expected_size,
            measurement.actual_size,
            first,
            measurement.fingerprint,
            summary,
        )
        .map_err(|error| format!("{}: {error}", self.path.display()))?;
        writer
            .flush()
            .map_err(|error| format!("{}: {error}", self.path.display()))
    }
}

fn parse_journal_row(fields: &[&str]) -> Option<(String, Measurement)> {
    if fields.len() != 9 || fields[0].is_empty() || fields[8].is_empty() {
        return None;
    }
    let exact = match fields[1] {
        "0" => false,
        "1" => true,
        _ => return None,
    };
    let score: u64 = fields[2].parse().ok()?;
    let differences: usize = fields[3].parse().ok()?;
    let expected_size: usize = fields[4].parse().ok()?;
    let actual_size: usize = fields[5].parse().ok()?;
    let first_difference = if fields[6] == "-" {
        None
    } else {
        Some(fields[6].parse().ok()?)
    };
    let fingerprint = u64::from_str_radix(fields[7], 16).ok()?;
    if score == u64::MAX {
        return None;
    }
    if exact
        && (score != 0
            || differences != 0
            || expected_size != actual_size
            || first_difference.is_some())
    {
        return None;
    }
    if first_difference.is_some_and(|index| index > expected_size.max(actual_size)) {
        return None;
    }
    Some((
        fields[0].to_string(),
        Measurement {
            exact,
            score,
            differences,
            expected_size,
            actual_size,
            first_difference,
            fingerprint,
            summary: fields[8].replace("\\t", "\t"),
        },
    ))
}

fn source_fingerprint(source: &str) -> String {
    alchemy_bundle::sha256::hex(source.as_bytes())
}

fn candidate_plan(
    permutation: &Permutation,
    iterations: usize,
    seed: u64,
    manual_only: bool,
    weights: &Weights,
) -> Result<Vec<Candidate>, String> {
    let base_count = permutation.count().min(iterations).max(1);
    let randomize = permutation.is_random() && !manual_only;
    let per_base = if randomize {
        iterations.div_ceil(base_count)
    } else {
        1
    };
    let mut pools = Vec::with_capacity(base_count);
    for manual_seed in 0..base_count {
        let (source, _) = if randomize {
            permutation.evaluate_marked(manual_seed)?
        } else {
            permutation.evaluate(manual_seed)?
        };
        let variants = if randomize {
            crate::randomize::mutate_marked_with_weights(
                &source,
                seed ^ (manual_seed as u64).wrapping_mul(0x9e37_79b9_7f4a_7c15),
                per_base,
                weights,
            )
        } else {
            vec![crate::randomize::Mutation {
                id: "manual".into(),
                source,
            }]
        };
        pools.push((manual_seed, variants));
    }

    let mut candidates = Vec::new();
    let mut seen = BTreeSet::new();
    let mut layer = 0usize;
    while candidates.len() < iterations {
        let mut added = false;
        for (manual_seed, variants) in &pools {
            let Some(variant) = variants.get(layer) else {
                continue;
            };
            added = true;
            let fingerprint = source_fingerprint(&variant.source);
            if seen.insert(fingerprint.clone()) {
                candidates.push(Candidate {
                    index: candidates.len(),
                    manual_seed: *manual_seed,
                    mutation: variant.id.clone(),
                    source: variant.source.clone(),
                    fingerprint,
                });
                if candidates.len() == iterations {
                    break;
                }
            }
        }
        if !added {
            break;
        }
        layer += 1;
    }
    if candidates.is_empty() {
        return Err("permutation plan produced no candidates".into());
    }
    Ok(candidates)
}

fn measurement_order(
    left: &(&Candidate, &Measurement),
    right: &(&Candidate, &Measurement),
) -> Ordering {
    left.1
        .score
        .cmp(&right.1.score)
        .then_with(|| left.1.differences.cmp(&right.1.differences))
        .then_with(|| {
            left.1
                .actual_size
                .abs_diff(left.1.expected_size)
                .cmp(&right.1.actual_size.abs_diff(right.1.expected_size))
        })
        .then_with(|| left.0.index.cmp(&right.0.index))
}

fn json_escape(text: &str) -> String {
    let mut output = String::new();
    for character in text.chars() {
        match character {
            '"' => output.push_str("\\\""),
            '\\' => output.push_str("\\\\"),
            '\n' => output.push_str("\\n"),
            '\r' => output.push_str("\\r"),
            '\t' => output.push_str("\\t"),
            value if value.is_control() => {
                output.push_str(&format!("\\u{:04x}", value as u32));
            }
            value => output.push(value),
        }
    }
    output
}

fn input_label(input: &backend::Input) -> &str {
    let path = if input.directory_mode {
        &input.requested
    } else {
        &input.source_path
    };
    path.file_stem()
        .and_then(|value| value.to_str())
        .unwrap_or("candidate")
}

fn default_output(input: &backend::Input, seed: u64) -> PathBuf {
    let stem = input_label(input);
    root()
        .join("out")
        .join("alchemy-permuter")
        .join(format!("{stem}-seed-{seed}"))
}

fn save_results(
    output: &Path,
    backend_name: &str,
    baseline: &Measurement,
    retained: &[(&Candidate, &Measurement)],
    attempted: usize,
    failures: usize,
) -> Result<(), String> {
    fs::create_dir_all(output).map_err(|error| format!("{}: {error}", output.display()))?;
    for entry in fs::read_dir(output)
        .map_err(|error| format!("{}: {error}", output.display()))?
        .flatten()
    {
        let path = entry.path();
        let owned_candidate = path
            .file_name()
            .and_then(|name| name.to_str())
            .is_some_and(|name| name.starts_with("candidate-") && name.ends_with(".c"));
        if owned_candidate {
            fs::remove_file(&path).map_err(|error| format!("{}: {error}", path.display()))?;
        }
    }
    for (rank, (candidate, measurement)) in retained.iter().enumerate() {
        let path = output.join(format!("candidate-{rank:03}-score-{}.c", measurement.score));
        fs::write(&path, &candidate.source)
            .map_err(|error| format!("{}: {error}", path.display()))?;
    }
    let mut report = format!(
        "{{\n  \"backend\": \"{}\",\n  \"baseline_score\": {},\n  \"attempted\": {},\n  \"compile_failures\": {},\n  \"results\": [",
        json_escape(backend_name), baseline.score, attempted, failures
    );
    for (index, (candidate, measurement)) in retained.iter().enumerate() {
        if index != 0 {
            report.push(',');
        }
        report.push_str(&format!(
            "\n    {{\"rank\": {index}, \"candidate\": {}, \"manual_seed\": {}, \"mutation\": \"{}\", \"source_fingerprint\": \"{}\", \"score\": {}, \"exact\": {}, \"differences\": {}, \"actual_size\": {}, \"expected_size\": {}, \"summary\": \"{}\"}}",
            candidate.index,
            candidate.manual_seed,
            json_escape(&candidate.mutation),
            candidate.fingerprint,
            measurement.score,
            measurement.exact,
            measurement.differences,
            measurement.actual_size,
            measurement.expected_size,
            json_escape(&measurement.summary),
        ));
    }
    report.push_str("\n  ]\n}\n");
    let path = output.join("report.json");
    fs::write(&path, report).map_err(|error| format!("{}: {error}", path.display()))?;
    Ok(())
}

fn retain_result(
    options: &Options,
    baseline: &Measurement,
    former_best: u64,
    measurement: &Measurement,
) -> bool {
    if measurement.score == u64::MAX {
        return false;
    }
    if options.better_only && measurement.score >= baseline.score {
        return false;
    }
    if options.best_only && measurement.score > former_best {
        return false;
    }
    if options
        .score_threshold
        .is_some_and(|threshold| measurement.score >= threshold)
    {
        return false;
    }
    measurement.exact || measurement.score <= baseline.score
}

fn run_workers(
    backend: Arc<dyn Backend>,
    candidates: Arc<Vec<Candidate>>,
    jobs: usize,
    stop_exact: bool,
    baseline: Measurement,
    journal: Arc<Journal>,
) -> Result<Vec<Evaluated>, String> {
    let next = Arc::new(AtomicUsize::new(0));
    let stop = Arc::new(AtomicBool::new(false));
    let (sender, receiver) = mpsc::channel();
    let worker_count = jobs.min(candidates.len()).max(1);
    let mut workers = Vec::with_capacity(worker_count);
    for _ in 0..worker_count {
        let backend = Arc::clone(&backend);
        let candidates = Arc::clone(&candidates);
        let next = Arc::clone(&next);
        let stop = Arc::clone(&stop);
        let sender = sender.clone();
        let baseline = baseline.clone();
        let journal = Arc::clone(&journal);
        workers.push(std::thread::spawn(move || loop {
            if stop_exact && stop.load(AtomicOrdering::Acquire) {
                break;
            }
            let index = next.fetch_add(1, AtomicOrdering::Relaxed);
            let Some(candidate) = candidates.get(index).cloned() else {
                break;
            };
            let started = Instant::now();
            let cached = journal.cached(&candidate.fingerprint);
            let measurement = if let Some(cached) = cached.as_ref() {
                Ok(cached.clone())
            } else if index == 0 {
                Ok(baseline.clone())
            } else {
                backend.measure(&candidate.source)
            };
            if let Ok(measurement) = &measurement {
                if cached.is_none() {
                    if let Err(error) = journal.record(&candidate.fingerprint, measurement) {
                        stop.store(true, AtomicOrdering::Release);
                        let _ = sender.send(Err(error));
                        break;
                    }
                }
            }
            if stop_exact
                && measurement
                    .as_ref()
                    .is_ok_and(|measurement| measurement.exact)
            {
                stop.store(true, AtomicOrdering::Release);
            }
            let _ = sender.send(Ok(Evaluated {
                candidate,
                measurement,
                elapsed: started.elapsed(),
            }));
        }));
    }
    drop(sender);
    let mut evaluated = Vec::new();
    let mut worker_error = None;
    for result in receiver {
        match result {
            Ok(item) => evaluated.push(item),
            Err(error) => {
                worker_error.get_or_insert(error);
            }
        };
    }
    for worker in workers {
        if worker.join().is_err() && worker_error.is_none() {
            worker_error = Some("alchemy-permuter worker panicked".into());
        }
    }
    match worker_error {
        Some(error) => Err(error),
        None => Ok(evaluated),
    }
}

fn run_one(options: &Options, candidate: &Path, multiple: bool) -> Result<(), String> {
    let input = backend::load_input(candidate)?;
    let output = match &options.output {
        Some(base) if multiple => base.join(input_label(&input)),
        Some(output) => output.clone(),
        None => default_output(&input, options.seed),
    };
    let permutation = crate::perm::parse(&input.source)?;
    let weights = if input.directory_mode {
        Weights::from_settings(&input.requested.join("settings.toml"), "base")?
    } else {
        Weights::for_profile("gcc")
    };
    let candidates = candidate_plan(
        &permutation,
        if options.debug { 1 } else { options.iterations },
        options.seed,
        options.manual_only,
        &weights,
    )?;
    let target = backend::prepare(&input, &candidates[0].source, options.show_errors)?;
    let baseline = target.baseline();
    let journal = Arc::new(Journal::open(&output, &target.identity(), options.resume)?);
    println!(
        "backend={} bases={} candidates={} jobs={} baseline={} ({})",
        target.name(),
        permutation.count(),
        candidates.len(),
        options.jobs.min(candidates.len()),
        baseline.score,
        baseline.summary
    );
    if options.debug {
        return Ok(());
    }

    let backend_name = target.name().to_string();
    let target: Arc<dyn Backend> = Arc::from(target);
    let candidates = Arc::new(candidates);
    let started = Instant::now();
    let mut evaluated = run_workers(
        target,
        Arc::clone(&candidates),
        options.jobs,
        options.stop_exact,
        baseline.clone(),
        journal,
    )?;
    evaluated.sort_by_key(|item| item.candidate.index);

    let mut best = baseline.score;
    let mut retained = Vec::new();
    let mut failures = 0usize;
    let mut compile_time = Duration::ZERO;
    for (completed, item) in evaluated.iter().enumerate() {
        compile_time += item.elapsed;
        match &item.measurement {
            Ok(measurement) => {
                let former_best = best;
                if measurement.score < best {
                    best = measurement.score;
                    println!(
                        "new-best={} candidate={} mutation={} {}",
                        best, item.candidate.index, item.candidate.mutation, measurement.summary
                    );
                }
                if retain_result(options, &baseline, former_best, measurement) {
                    retained.push((&item.candidate, measurement));
                }
            }
            Err(error) => {
                failures += 1;
                if options.show_errors {
                    eprintln!("candidate {}: {error}", item.candidate.index);
                }
            }
        }
        if !options.quiet && (completed + 1) % 100 == 0 {
            println!(
                "progress={}/{} best={} failures={}",
                completed + 1,
                evaluated.len(),
                best,
                failures
            );
        }
    }
    retained.sort_by(measurement_order);
    retained.truncate(options.top);
    save_results(
        &output,
        &backend_name,
        &baseline,
        &retained,
        evaluated.len(),
        failures,
    )?;
    let exact = retained.iter().any(|(_, measurement)| measurement.exact);
    println!(
        "done={} attempted={} failures={} best={} exact={} wall_ms={} compiler_ms={} output={}",
        backend_name,
        evaluated.len(),
        failures,
        best,
        exact,
        started.elapsed().as_millis(),
        compile_time.as_millis(),
        output.display()
    );
    Ok(())
}

pub fn run(options: Options) -> Result<(), String> {
    let multiple = options.candidates.len() > 1;
    for candidate in &options.candidates {
        run_one(&options, candidate, multiple)?;
    }
    Ok(())
}

pub fn self_test() -> Result<(), String> {
    let permutation =
        crate::perm::parse("int f(void) { s32 a = PERM_GENERAL(1,2); return a + 0; }")?;
    let weights = Weights::for_profile("gcc");
    let first = candidate_plan(&permutation, 20, 9, false, &weights)?;
    let second = candidate_plan(&permutation, 20, 9, false, &weights)?;
    if first != second || first.len() != 2 {
        return Err(format!(
            "manual runner plan drifted: {} candidates",
            first.len()
        ));
    }
    let random = crate::perm::parse("int f(void) { s32 a = 1; return a + 0; }")?;
    let mutations = candidate_plan(&random, 20, 9, false, &weights)?;
    if mutations.len() < 2 || mutations[0].mutation != "identity" {
        return Err("random runner plan is degenerate".into());
    }
    if json_escape("a\n\"b") != "a\\n\\\"b" {
        return Err("JSON escaping drifted".into());
    }
    let journal_dir = std::env::temp_dir().join(format!(
        "alchemy-permuter-journal-self-test-{}",
        std::process::id()
    ));
    let measurement = Measurement {
        exact: false,
        score: 17,
        differences: 3,
        expected_size: 8,
        actual_size: 6,
        first_difference: Some(2),
        fingerprint: 0x1234,
        summary: "journal control".into(),
    };
    {
        let journal = Journal::open(&journal_dir, "55aa", false)?;
        journal.record("77cc", &measurement)?;
    }
    let restored = Journal::open(&journal_dir, "55aa", true)?.cached("77cc");
    if Journal::open(&journal_dir, "changed-identity", true)?
        .cached("77cc")
        .is_some()
    {
        return Err("stale journal data was reused after an identity change".into());
    }
    Journal::open(&journal_dir, "55aa", false)?;
    let journal_path = journal_dir.join("journal.tsv");
    let mut journal_text = fs::read_to_string(&journal_path)
        .map_err(|error| format!("{}: {error}", journal_path.display()))?;
    journal_text.push_str("incomplete row\n");
    fs::write(&journal_path, journal_text)
        .map_err(|error| format!("{}: {error}", journal_path.display()))?;
    if Journal::open(&journal_dir, "55aa", true).is_ok() {
        return Err("corrupt journal row was accepted during resume".into());
    }
    let _ = fs::remove_file(journal_dir.join("journal.tsv"));
    let _ = fs::remove_dir(&journal_dir);
    if restored != Some(measurement) {
        return Err("resume journal failed to restore a matching measurement".into());
    }
    Ok(())
}
