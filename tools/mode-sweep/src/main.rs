//! Native CLI for compiler-mode planning, scoring, and reporting.

use std::fs;
use std::path::{Path, PathBuf};
use std::process::{Command, ExitCode};
use std::sync::{
    atomic::{AtomicUsize, Ordering},
    Mutex,
};

use candidate_compiler::jsnum::parse_hex;
use candidate_compiler::verify::{
    js_subarray, verify_candidate, CandidateCompilerConfiguration, CandidateCompilerFamily,
    ROM_BASE,
};
use candidate_compiler::CompilerTarget;
use candidate_show::region_size;
use integrate_matches::linked_function_extent;
use mode_sweep::{
    classify, compiler_signature, disassembly_line, hash, historical_family_factorial_configs,
    options_of, pair_configs, ranked_pair_configs, root, self_test, single_configs, triple_configs,
    Config, Evidence, Options, OptionsOutcome, SELF_TEST_CHECK_FLOOR, USAGE,
};
use search_compiler_modes::{canonical_json, parse_json, Json};

#[derive(Debug, Clone)]
struct Score {
    config: Config,
    cache_key: String,
    cached: bool,
    compiled: bool,
    size: Option<usize>,
    error: Option<String>,
    evidence: Option<Evidence>,
}

impl Score {
    fn to_json(&self) -> Json {
        let mut fields = vec![
            ("config".into(), self.config.to_json()),
            ("cache_key".into(), Json::String(self.cache_key.clone())),
            ("cached".into(), Json::Bool(self.cached)),
            ("compiled".into(), Json::Bool(self.compiled)),
        ];
        if let Some(size) = self.size {
            fields.push(("size".into(), Json::Number(size as f64)));
        }
        if let Some(error) = &self.error {
            fields.push(("error".into(), Json::String(error.clone())));
        }
        if let Some(evidence) = &self.evidence {
            fields.push(("evidence".into(), evidence.to_json()));
        }
        Json::Object(fields)
    }
}

fn json_bool(value: Option<&Json>) -> Option<bool> {
    match value {
        Some(Json::Bool(value)) => Some(*value),
        _ => None,
    }
}

fn json_number(value: Option<&Json>) -> Option<f64> {
    match value {
        Some(Json::Number(value)) => Some(*value),
        _ => None,
    }
}

fn strings(value: Option<&Json>) -> Option<Vec<String>> {
    let Json::Array(items) = value? else {
        return None;
    };
    items
        .iter()
        .map(|item| item.as_str().map(str::to_string))
        .collect()
}

fn config_from_json(value: Option<&Json>) -> Option<Config> {
    Some(Config {
        ids: strings(value?.get("ids"))?,
        flags: strings(value?.get("flags"))?,
        remove_flags: strings(value?.get("remove_flags"))?,
        compiler_family: value?.get("compiler_family")?.as_str()?.to_string(),
    })
}

fn evidence_from_json(value: Option<&Json>) -> Option<Evidence> {
    Some(Evidence {
        differing_halfwords: json_number(value?.get("differing_halfwords"))? as u64,
        size_delta: json_number(value?.get("size_delta"))? as i64,
        exact: json_bool(value?.get("exact"))?,
        exact_size: json_bool(value?.get("exact_size"))?,
        instruction_order_proxy: json_bool(value?.get("instruction_order_proxy"))?,
        register_allocation_proxy: json_number(value?.get("register_allocation_proxy"))? as u64,
        literal_placement_proxy: json_bool(value?.get("literal_placement_proxy"))?,
        control_flow_proxy: json_bool(value?.get("control_flow_proxy"))?,
    })
}

fn score_from_json(value: &Json, cache_key: &str) -> Option<Score> {
    if !mode_sweep::accepted_cache(value, cache_key) {
        return None;
    }
    Some(Score {
        config: config_from_json(value.get("config"))?,
        cache_key: value.get("cache_key")?.as_str()?.to_string(),
        cached: false,
        compiled: json_bool(value.get("compiled"))?,
        size: json_number(value.get("size")).map(|size| size as usize),
        error: value
            .get("error")
            .and_then(Json::as_str)
            .map(str::to_string),
        evidence: evidence_from_json(value.get("evidence")),
    })
}

fn disassemble(path: &Path) -> Vec<String> {
    let output = match Command::new("arm-none-eabi-objdump")
        .args(["-D", "-b", "binary", "-m", "arm", "-M", "force-thumb"])
        .arg(path)
        .output()
    {
        Ok(output) if output.status.success() => output,
        _ => return Vec::new(),
    };
    String::from_utf8_lossy(&output.stdout)
        .split('\n')
        .filter_map(disassembly_line)
        .collect()
}

fn compiler_family(name: &str) -> Result<CandidateCompilerFamily, String> {
    CandidateCompilerFamily::parse(name).ok_or_else(|| format!("unknown compiler family: {name}"))
}

fn bounded_integer(value: f64, name: &str) -> Result<usize, String> {
    if !value.is_finite() || value.fract() != 0.0 || value < 1.0 {
        return Err(format!("{name} must be a positive integer"));
    }
    Ok(value as usize)
}

fn score_one(
    _root: &Path,
    options: &Options,
    rom: &[u8],
    source_bytes: &[u8],
    reference: &[u8],
    reference_asm: &[String],
    compiler_digest: &str,
    output: &Path,
    stem: &str,
    address: f64,
    config: Config,
) -> Score {
    let config_json = canonical_json(&config.to_json());
    let cache_key = hash(&[
        &mode_sweep::FORMAT.to_string().into_bytes(),
        source_bytes,
        reference,
        compiler_digest.as_bytes(),
        config_json.as_bytes(),
    ]);
    let cache_path = output.join("cache").join(format!("{cache_key}.json"));
    if let Ok(text) = fs::read_to_string(&cache_path) {
        if let Ok(document) = parse_json(&text) {
            if let Some(mut score) = score_from_json(&document, &cache_key) {
                score.cached = true;
                return score;
            }
        }
    }

    let work = output.join("scratch").join(&cache_key);
    let result = (|| -> Result<Score, String> {
        fs::create_dir_all(&work).map_err(|error| format!("{}: {error}", work.display()))?;
        let family = compiler_family(&config.compiler_family)?;
        let configuration = CandidateCompilerConfiguration {
            family: Some(family),
            add_flags: config.flags.clone(),
            remove_flags: config.remove_flags.clone(),
        };
        verify_candidate(
            &options.source,
            rom,
            &work.to_string_lossy(),
            &[],
            ROM_BASE,
            CompilerTarget::Gs1,
            &configuration,
        )?;
        let linked_path = work.join(format!("{stem}.bin"));
        let linked = fs::read(&linked_path)
            .map_err(|error| format!("{}: {error}", linked_path.display()))?;
        let symbols_path = work.join(format!("{stem}.elf"));
        let symbols = Command::new("arm-none-eabi-nm")
            .args(["-S", "--defined-only"])
            .arg(&symbols_path)
            .output()
            .map_err(|error| format!("arm-none-eabi-nm: {error}"))?;
        let extent = if symbols.status.success() {
            linked_function_extent(
                &String::from_utf8_lossy(&symbols.stdout),
                &format!("Func_{stem}"),
                address,
                linked.len(),
            )?
        } else {
            linked.len()
        };
        let actual = &linked[..extent.min(linked.len())];
        let actual_path = work.join("actual.bin");
        fs::write(&actual_path, actual)
            .map_err(|error| format!("{}: {error}", actual_path.display()))?;
        Ok(Score {
            config: config.clone(),
            cache_key: cache_key.clone(),
            cached: false,
            compiled: true,
            size: Some(actual.len()),
            error: None,
            evidence: Some(classify(
                actual,
                reference,
                &disassemble(&actual_path),
                reference_asm,
            )),
        })
    })();

    let score = match result {
        Ok(score) => score,
        Err(error) => Score {
            config,
            cache_key: cache_key.clone(),
            cached: false,
            compiled: false,
            size: None,
            error: Some(error.chars().take(500).collect()),
            evidence: None,
        },
    };
    let _ = fs::create_dir_all(output.join("cache"));
    let _ = fs::write(&cache_path, canonical_json(&score.to_json()) + "\n");
    score
}

fn phase<F>(configs: &[Config], jobs: usize, operation: F) -> Vec<Score>
where
    F: Fn(&Config) -> Score + Sync,
{
    let results = Mutex::new(
        (0..configs.len())
            .map(|_| None)
            .collect::<Vec<Option<Score>>>(),
    );
    let cursor = AtomicUsize::new(0);
    std::thread::scope(|scope| {
        for _ in 0..jobs.min(configs.len()).max(1) {
            scope.spawn(|| loop {
                let index = cursor.fetch_add(1, Ordering::Relaxed);
                if index >= configs.len() {
                    return;
                }
                let score = operation(&configs[index]);
                results.lock().expect("score result lock")[index] = Some(score);
            });
        }
    });
    results
        .into_inner()
        .expect("score result lock")
        .into_iter()
        .map(|score| score.expect("every mode-sweep worker produced a result"))
        .collect()
}

fn rank(mut scores: Vec<Score>) -> Vec<Score> {
    scores.retain(|score| score.evidence.is_some());
    scores.sort_by(|left, right| {
        let a = left.evidence.as_ref().expect("retained evidence");
        let b = right.evidence.as_ref().expect("retained evidence");
        b.exact
            .cmp(&a.exact)
            .then(a.differing_halfwords.cmp(&b.differing_halfwords))
            .then(
                a.size_delta
                    .unsigned_abs()
                    .cmp(&b.size_delta.unsigned_abs()),
            )
            .then(left.config.flags.len().cmp(&right.config.flags.len()))
            .then_with(|| mode_sweep::collate(&left.config.key(), &right.config.key()))
    });
    scores
}

fn report_json(
    options: &Options,
    stem: &str,
    source_bytes: &[u8],
    reference: &[u8],
    compiler_digest: &str,
    single: &[Score],
    pairs: &[Score],
    triples: &[Score],
    factorial: &[Score],
    pair_plan: &[Config],
    all_pairs: &[Config],
    triple_plan: &[Config],
    all_triples: &[Config],
    factorial_plan: &[Config],
    all_factorial: &[Config],
    seeds: &[String],
    bounded: bool,
) -> Json {
    let phases = {
        let mut phases = vec![
            Json::String("routed-default".into()),
            Json::String("single".into()),
        ];
        if options.pairs {
            phases.push(Json::String("compatible-pair".into()));
        }
        if options.triples {
            phases.push(Json::String("evidence-supported-triple".into()));
        }
        if options.family_factorial {
            phases.push(Json::String("historical-family-stock-factorial".into()));
        }
        Json::Array(phases)
    };
    let results = single
        .iter()
        .chain(pairs)
        .chain(triples)
        .chain(factorial)
        .map(Score::to_json)
        .collect::<Vec<_>>();
    let all_results = single
        .iter()
        .chain(pairs)
        .chain(triples)
        .chain(factorial)
        .collect::<Vec<_>>();
    Json::Object(vec![
        ("format".into(), Json::Number(mode_sweep::FORMAT as f64)),
        ("stem".into(), Json::String(stem.into())),
        ("source".into(), Json::String(options.source.clone())),
        ("source_sha256".into(), Json::String(hash(&[source_bytes]))),
        ("reference_sha256".into(), Json::String(hash(&[reference]))),
        (
            "compiler_signature".into(),
            Json::String(compiler_digest.into()),
        ),
        (
            "policy".into(),
            Json::Object(vec![
                (
                    "families".into(),
                    Json::Array(
                        [
                            "routed",
                            "gcc296",
                            "old-agbcc",
                            "pret-early-thumb",
                            "gcc2951",
                        ]
                        .into_iter()
                        .map(|s| Json::String(s.into()))
                        .collect(),
                    ),
                ),
                ("phases".into(), phases),
                (
                    "triple_threshold_halfwords".into(),
                    Json::Array(vec![Json::Number(2.0), Json::Number(5.0)]),
                ),
                ("auto_promote".into(), Json::Bool(false)),
            ]),
        ),
        ("attempted".into(), Json::Number(results.len() as f64)),
        (
            "compiled".into(),
            Json::Number(all_results.iter().filter(|score| score.compiled).count() as f64),
        ),
        (
            "cache_hits".into(),
            Json::Number(all_results.iter().filter(|score| score.cached).count() as f64),
        ),
        (
            "planning".into(),
            Json::Object(vec![
                ("pairs_planned".into(), Json::Number(pair_plan.len() as f64)),
                (
                    "pairs_available".into(),
                    Json::Number(all_pairs.len() as f64),
                ),
                (
                    "triples_planned".into(),
                    Json::Number(triple_plan.len() as f64),
                ),
                (
                    "triples_available".into(),
                    Json::Number(all_triples.len() as f64),
                ),
                (
                    "family_factorial_planned".into(),
                    Json::Number(factorial_plan.len() as f64),
                ),
                (
                    "family_factorial_available".into(),
                    Json::Number(all_factorial.len() as f64),
                ),
                ("bounded_search_complete".into(), Json::Bool(bounded)),
            ]),
        ),
        (
            "strong_seed_ids".into(),
            Json::Array(seeds.iter().map(|s| Json::String(s.clone())).collect()),
        ),
        ("results".into(), Json::Array(results)),
    ])
}

fn run(options: Options, root: &Path) -> Result<(), String> {
    let rom_path = options
        .rom
        .as_deref()
        .ok_or_else(|| "ROM path is required".to_string())?;
    let rom = fs::read(rom_path).map_err(|error| format!("{rom_path}: {error}"))?;
    let stem = candidate_compiler::verify::source_stem(&options.source);
    let address = parse_hex(&stem)?;
    let wanted = region_size(root, &stem)
        .ok_or_else(|| format!("no assembly region is recorded for {stem}"))?;
    let wanted = bounded_integer(wanted, "region size")?;
    let reference = js_subarray(&rom, address - ROM_BASE, address - ROM_BASE + wanted as f64);
    let source_bytes =
        fs::read(&options.source).map_err(|error| format!("{}: {error}", options.source))?;
    let output = PathBuf::from(mode_sweep::mode_sweep_output_directory(
        root,
        &options.source,
        &source_bytes,
    ));
    fs::create_dir_all(output.join("cache"))
        .map_err(|error| format!("{}: {error}", output.display()))?;
    fs::write(output.join("reference.bin"), &reference)
        .map_err(|error| format!("reference.bin: {error}"))?;
    let reference_asm = disassemble(&output.join("reference.bin"));
    let compiler_digest =
        compiler_signature(root).map_err(|error| format!("compiler signature: {error}"))?;
    let jobs = bounded_integer(options.jobs, "jobs")?;
    let score = |config: &Config| {
        score_one(
            root,
            &options,
            &rom,
            &source_bytes,
            &reference,
            &reference_asm,
            &compiler_digest,
            &output,
            &stem,
            address,
            config.clone(),
        )
    };
    let single = phase(&single_configs(), jobs, score);
    let all_pairs = pair_configs(None);
    let pair_plan = if options.pairs {
        ranked_pair_configs(
            &single
                .iter()
                .map(|row| {
                    (
                        row.config.ids.first().cloned().unwrap_or_default(),
                        mode_sweep::Rank {
                            exact: row.evidence.as_ref().is_some_and(|e| e.exact),
                            floor: row
                                .evidence
                                .as_ref()
                                .map_or(f64::INFINITY, |e| e.differing_halfwords as f64),
                        },
                    )
                })
                .collect::<Vec<_>>(),
            bounded_integer(options.max_pairs, "max-pairs")?,
        )
    } else {
        Vec::new()
    };
    let pairs = phase(&pair_plan, jobs, score);
    let best_single_floor = single
        .iter()
        .filter_map(|row| row.evidence.as_ref().map(|e| e.differing_halfwords))
        .min()
        .unwrap_or(u64::MAX);
    let strong = pairs.iter().filter(|row| {
        row.evidence.as_ref().is_some_and(|e| {
            e.exact_size
                && (2..=5).contains(&e.differing_halfwords)
                && e.differing_halfwords < best_single_floor
        })
    });
    let mut seeds = Vec::new();
    for id in strong.flat_map(|row| row.config.ids.iter()) {
        if !seeds.contains(id) {
            seeds.push(id.clone());
        }
    }
    let all_triples = triple_configs(&seeds, None);
    let triple_plan = if options.triples {
        all_triples[..all_triples
            .len()
            .min(bounded_integer(options.max_triples, "max-triples")?)]
            .to_vec()
    } else {
        Vec::new()
    };
    let triples = phase(&triple_plan, jobs, score);
    let all_factorial = historical_family_factorial_configs();
    let factorial_plan = if options.family_factorial {
        all_factorial.clone()
    } else {
        Vec::new()
    };
    let factorial = phase(&factorial_plan, jobs, score);
    let bounded = options.pairs
        && pair_plan.len() == all_pairs.len()
        && (!options.family_factorial || factorial_plan.len() == all_factorial.len())
        && (all_triples.is_empty() || options.triples && triple_plan.len() == all_triples.len());
    let report = report_json(
        &options,
        &stem,
        &source_bytes,
        &reference,
        &compiler_digest,
        &single,
        &pairs,
        &triples,
        &factorial,
        &pair_plan,
        &all_pairs,
        &triple_plan,
        &all_triples,
        &factorial_plan,
        &all_factorial,
        &seeds,
        bounded,
    );
    let ranked = rank(
        single
            .iter()
            .chain(&pairs)
            .chain(&triples)
            .chain(&factorial)
            .cloned()
            .collect(),
    );
    let best = ranked.first();
    let best_evidence = best.and_then(|score| score.evidence.clone());
    let floor = Json::Object(vec![
        ("format".into(), Json::Number(mode_sweep::FORMAT as f64)),
        ("stem".into(), Json::String(stem.clone())),
        ("source_sha256".into(), Json::String(hash(&[&source_bytes]))),
        ("reference_sha256".into(), Json::String(hash(&[&reference]))),
        (
            "compiler_signature".into(),
            Json::String(compiler_digest.clone()),
        ),
        (
            "searched".into(),
            report
                .get("policy")
                .and_then(|p| p.get("phases"))
                .cloned()
                .unwrap_or(Json::Array(Vec::new())),
        ),
        ("bounded_search_complete".into(), Json::Bool(bounded)),
        (
            "exact".into(),
            Json::Bool(best_evidence.as_ref().is_some_and(|e| e.exact)),
        ),
        (
            "irreducible_floor_halfwords".into(),
            if bounded {
                best_evidence
                    .as_ref()
                    .map_or(Json::Null, |e| Json::Number(e.differing_halfwords as f64))
            } else {
                Json::Null
            },
        ),
        (
            "best_observed_halfwords".into(),
            best_evidence
                .as_ref()
                .map_or(Json::Null, |e| Json::Number(e.differing_halfwords as f64)),
        ),
        (
            "best_config".into(),
            best.map_or(Json::Null, |score| score.config.to_json()),
        ),
        (
            "classification".into(),
            best_evidence.as_ref().map_or(Json::Null, Evidence::to_json),
        ),
        (
            "escalation".into(),
            if bounded
                && options.triples
                && best_evidence
                    .as_ref()
                    .is_some_and(|e| e.exact_size && (2..=5).contains(&e.differing_halfwords))
            {
                Json::String("compiler-rtl-scheduler-trace".into())
            } else {
                Json::Null
            },
        ),
    ]);
    fs::write(output.join("report.json"), canonical_json(&report) + "\n")
        .map_err(|error| format!("report.json: {error}"))?;
    fs::write(output.join("floor.json"), canonical_json(&floor) + "\n")
        .map_err(|error| format!("floor.json: {error}"))?;
    println!(
        "region={wanted}B attempted={} compiled={} cache_hits={}",
        report
            .get("attempted")
            .and_then(|v| json_number(Some(v)))
            .unwrap_or(0.0),
        report
            .get("compiled")
            .and_then(|v| json_number(Some(v)))
            .unwrap_or(0.0),
        report
            .get("cache_hits")
            .and_then(|v| json_number(Some(v)))
            .unwrap_or(0.0)
    );
    for row in ranked.iter().take(bounded_integer(options.top, "top")?) {
        let evidence = row.evidence.as_ref().expect("ranked evidence");
        let mut tags = Vec::new();
        if evidence.exact {
            tags.push("EXACT".into());
        }
        if evidence.instruction_order_proxy {
            tags.push("order".into());
        }
        if evidence.register_allocation_proxy != 0 {
            tags.push(format!("register={}", evidence.register_allocation_proxy));
        }
        if evidence.literal_placement_proxy {
            tags.push("literal".into());
        }
        if evidence.control_flow_proxy {
            tags.push("cfg".into());
        }
        let mut mutation = vec![row.config.compiler_family.clone()];
        mutation.extend(row.config.flags.clone());
        mutation.extend(
            row.config
                .remove_flags
                .iter()
                .map(|flag| format!("remove:{flag}")),
        );
        println!(
            "{:4}hw size={:4} {}{}",
            evidence.differing_halfwords,
            row.size.unwrap_or(0),
            mutation.join(" "),
            if tags.is_empty() {
                String::new()
            } else {
                format!(" [{}]", tags.join(","))
            }
        );
    }
    println!(
        "report={} floor={}",
        output.join("report.json").display(),
        output.join("floor.json").display()
    );
    Ok(())
}

fn main() -> ExitCode {
    let argv: Vec<String> = std::env::args().skip(1).collect();
    if argv.iter().any(|arg| arg == "--self-test") {
        return match self_test() {
            Ok(checks) if checks >= SELF_TEST_CHECK_FLOOR => {
                println!("mode sweep self-test passed ({checks} checks)");
                ExitCode::SUCCESS
            }
            Ok(checks) => {
                eprintln!("mode sweep self-test executed only {checks} checks, below the floor of {SELF_TEST_CHECK_FLOOR}");
                ExitCode::FAILURE
            }
            Err(error) => {
                eprintln!("mode sweep self-test failed: {error}");
                ExitCode::FAILURE
            }
        };
    }
    let repo = root();
    if argv.iter().any(|arg| arg == "--compiler-signature") {
        return match compiler_signature(&repo) {
            Ok(signature) => {
                println!("{signature}");
                ExitCode::SUCCESS
            }
            Err(error) => {
                eprintln!("compiler signature failed: {error}");
                ExitCode::FAILURE
            }
        };
    }
    if let Some(plan) = argv.iter().find_map(|arg| arg.strip_prefix("--plan=")) {
        let configs = match plan {
            "singles" => single_configs(),
            "pairs" => pair_configs(None),
            "family-factorial" => historical_family_factorial_configs(),
            value if value.starts_with("triples:") => triple_configs(
                &value[8..]
                    .split(',')
                    .map(str::to_string)
                    .collect::<Vec<_>>(),
                None,
            ),
            _ => {
                eprintln!("unknown plan: {plan}");
                return ExitCode::FAILURE;
            }
        };
        println!(
            "{}",
            canonical_json(&Json::Array(configs.iter().map(Config::to_json).collect()))
        );
        return ExitCode::SUCCESS;
    }
    match options_of(
        &argv,
        &repo,
        std::thread::available_parallelism()
            .ok()
            .map(|n| n.get() as u32),
    ) {
        Ok(OptionsOutcome::Help) => {
            println!("{USAGE}");
            println!("  --self-test              run the invariant suite");
            println!("  --plan=singles|pairs|family-factorial|triples:a,b");
            println!("  --compiler-signature     hash the native compiler inputs");
            ExitCode::SUCCESS
        }
        Ok(OptionsOutcome::Parsed(options)) => match run(*options, &repo) {
            Ok(()) => ExitCode::SUCCESS,
            Err(error) => {
                eprintln!("mode sweep failed: {error}");
                ExitCode::FAILURE
            }
        },
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn sample_score(cached: bool) -> Score {
        Score {
            config: Config {
                ids: vec!["opt-o1".into()],
                flags: vec!["-O1".into()],
                remove_flags: Vec::new(),
                compiler_family: "routed".into(),
            },
            cache_key: "abc".into(),
            cached,
            compiled: true,
            size: Some(4),
            error: None,
            evidence: Some(Evidence {
                differing_halfwords: 0,
                size_delta: 0,
                exact: true,
                exact_size: true,
                instruction_order_proxy: false,
                register_allocation_proxy: 0,
                literal_placement_proxy: false,
                control_flow_proxy: false,
            }),
        }
    }

    #[test]
    fn cached_score_round_trips_through_canonical_json() {
        let original = sample_score(false);
        let document = parse_json(&canonical_json(&original.to_json())).expect("score JSON");
        let restored = score_from_json(&document, "abc").expect("accepted score");
        assert_eq!(restored.config, original.config);
        assert_eq!(restored.compiled, original.compiled);
        assert_eq!(restored.size, original.size);
        assert_eq!(restored.evidence.as_ref().unwrap().differing_halfwords, 0);
    }

    #[test]
    fn stale_or_incomplete_cache_entries_are_rejected_before_execution() {
        let original = sample_score(false).to_json();
        assert!(score_from_json(&original, "wrong").is_none());
        let incomplete = Json::Object(vec![
            ("cache_key".into(), Json::String("abc".into())),
            ("compiled".into(), Json::Bool(true)),
        ]);
        assert!(score_from_json(&incomplete, "abc").is_none());
    }

    #[test]
    fn ranking_prefers_exact_then_smallest_halfword_floor() {
        let mut worse = sample_score(false);
        worse.evidence.as_mut().unwrap().exact = false;
        worse.evidence.as_mut().unwrap().differing_halfwords = 1;
        let ranked = rank(vec![worse, sample_score(false)]);
        assert!(ranked[0].evidence.as_ref().unwrap().exact);
        assert_eq!(ranked[1].evidence.as_ref().unwrap().differing_halfwords, 1);
    }
}
