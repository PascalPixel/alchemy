use candidate_compiler::verify::{CandidateCompilerConfiguration, CandidateCompilerFamily};
use candidate_show::{
    cli::Options as CandidateOptions,
    idioms::{lint, mine_dossiers},
    render::{render, RenderOutput},
    triage::{ResidualClass, ResidualReport},
};
use compiler_core::{
    routing::{root, CompilerTarget},
    sha256,
    source_paths::{SourceOwner, SourcePaths},
    translation_units::{TranslationUnit, TranslationUnits},
};
use regex::{Captures, Regex};
use serde::{Deserialize, Serialize};
use serde_json::{json, Value};
use std::{
    collections::{BTreeMap, BTreeSet},
    fs,
    path::{Path, PathBuf},
};

const USAGE: &str = "usage: compiler waves <inventory|bucket|pack|run> [--output PATH] [--family-index FILE] [--bucket FILE] [--limit N] [--stall-budget N] [--apply]";
const MANIFEST: &str = "out/gs1-en/full/asm/manifest.json";
const FAMILIES: &str = "out/gs1-en/reports/compiler-families.json";
const BUCKET: &str = "out/gs1-en/waves/bucket.json";
const CATALOG: &str = "games/gs1/recon/compiler-repair-patterns.json";
const DEBT: [&str; 5] = [
    "c_candidate",
    "merge_with_continuations",
    "merge_with_function_owner",
    "merge_with_owner",
    "split_first",
];
const WAVE_SIZE: usize = 50;
const STALL_BUDGET: usize = 18;

#[derive(Deserialize)]
struct Manifest {
    regions: Vec<Region>,
}
#[derive(Deserialize)]
struct Region {
    address: u64,
    size: usize,
    source: String,
    #[serde(default)]
    symbol: String,
    retention: Option<String>,
}
#[derive(Deserialize)]
struct FamilyIndex {
    schema_version: u32,
    targets: Vec<FamilyTarget>,
}
#[derive(Clone, Deserialize)]
struct FamilyTarget {
    owner: String,
    symbol: String,
    source: String,
    family: Option<String>,
    alternatives: Vec<Template>,
}
#[derive(Clone, Deserialize)]
struct Template {
    owner: String,
    symbol: String,
    source: String,
    score_basis_points: u16,
    #[serde(skip)]
    canonical_name: Option<String>,
}

#[derive(Clone, Copy, Debug, Deserialize, Eq, PartialEq, Serialize)]
#[serde(rename_all = "snake_case")]
enum Scope {
    TranslationUnit,
    StandaloneDraft,
    NoCandidate,
}
#[derive(Clone, Debug, Deserialize, Serialize)]
struct Owner {
    owner: String,
    address: u32,
    size: usize,
    retention: String,
    reference: String,
    symbol: String,
    name: Option<String>,
    scope: Scope,
    unit: Option<String>,
    candidate: Option<String>,
    candidate_sha256: Option<String>,
}
#[derive(Clone, Debug, Deserialize, Serialize)]
struct Inventory {
    schema_version: u32,
    manifest_sha256: String,
    owners: Vec<Owner>,
}
#[derive(Clone, Debug, Deserialize, Serialize)]
struct BucketEntry {
    owner: String,
    size: usize,
    scope: Scope,
    name: Option<String>,
    candidate_sha256: Option<String>,
    verdict: String,
    residual: Option<ResidualReport>,
    allocator_repair: Option<String>,
    allocator_dimensions: Vec<String>,
    lint: Option<Value>,
    error: Option<String>,
}
#[derive(Clone, Debug, Deserialize, Serialize)]
struct BucketReport {
    schema_version: u32,
    manifest_sha256: String,
    #[serde(default)]
    scoring_environment_sha256: String,
    owners: Vec<BucketEntry>,
}

#[derive(Clone)]
struct Args {
    output: Option<PathBuf>,
    family_index: PathBuf,
    bucket: PathBuf,
    limit: Option<usize>,
    stall_budget: usize,
    apply: bool,
}
struct Budget {
    limit: usize,
    used: usize,
}
impl Budget {
    fn take(&mut self) -> Result<(), String> {
        if self.used >= self.limit {
            return Err(format!("compiler stall budget {} exhausted", self.limit));
        }
        self.used += 1;
        Ok(())
    }
}

pub fn run(argv: &[String]) -> Result<(), String> {
    let Some(command) = argv.first().map(String::as_str) else {
        return Err(USAGE.into());
    };
    let args = parse_args(&argv[1..])?;
    match command {
        "inventory" => inventory_command(args),
        "bucket" => bucket_command(args),
        "pack" => pack_command(args),
        "run" => wave_command(args),
        "-h" | "--help" => {
            println!("{USAGE}");
            Ok(())
        }
        _ => Err(USAGE.into()),
    }
}

fn inventory_command(args: Args) -> Result<(), String> {
    let inventory = inventory(root())?;
    let output = args
        .output
        .unwrap_or_else(|| root().join("out/gs1-en/waves/inventory.json"));
    write_json(&output, &inventory)?;
    let (unit, draft, none, named) = inventory_counts(&inventory.owners);
    println!("debt_owners={} debt_bytes={} standalone_drafts={draft} translation_units={unit} no_candidate={none} named_wave_ready={named} inventory={}", inventory.owners.len(), inventory.owners.iter().map(|o| o.size).sum::<usize>(), output.display());
    Ok(())
}

fn bucket_command(args: Args) -> Result<(), String> {
    let repository = root();
    let inventory = inventory(repository)?;
    let output = args.output.unwrap_or_else(|| repository.join(BUCKET));
    let owner_dir = output.with_extension("").join("owners");
    let mut unit_scores = bucket_translation_units(repository, &inventory, &owner_dir)?;
    let mut entries = Vec::with_capacity(inventory.owners.len());
    for owner in &inventory.owners {
        let mut entry = BucketEntry {
            owner: owner.owner.clone(),
            size: owner.size,
            scope: owner.scope,
            name: owner.name.clone(),
            candidate_sha256: owner.candidate_sha256.clone(),
            verdict: String::new(),
            residual: None,
            allocator_repair: None,
            allocator_dimensions: Vec::new(),
            lint: None,
            error: None,
        };
        match owner.scope {
            Scope::StandaloneDraft => bucket_score(repository, owner, &owner_dir, &mut entry),
            Scope::TranslationUnit => {
                bucket_lint(repository, owner, &mut entry);
                if entry.verdict.is_empty() {
                    match unit_scores
                        .remove(&owner.address)
                        .unwrap_or_else(|| Err("translation-unit score missing".into()))
                    {
                        Ok(scored) => record_score(&mut entry, scored),
                        Err(error) => {
                            entry.verdict = "score_failed".into();
                            entry.error = Some(error);
                        }
                    }
                }
            }
            Scope::NoCandidate => {
                entry.verdict = "no_candidate".into();
                entry.error = Some(
                    "manifest debt has no semantic C candidate; it is not an allocation floor"
                        .into(),
                );
            }
        }
        write_json(
            &owner_dir.join(format!("{}.json", stem(owner.address))),
            &entry,
        )?;
        entries.push(entry);
    }
    let report = BucketReport {
        schema_version: 2,
        manifest_sha256: inventory.manifest_sha256,
        scoring_environment_sha256: scoring_environment(repository)?,
        owners: entries,
    };
    write_json(&output, &report)?;
    let scored = report
        .owners
        .iter()
        .filter(|o| o.verdict == "scored")
        .count();
    let covered = report
        .owners
        .iter()
        .filter(|o| {
            o.residual
                .as_ref()
                .is_some_and(|r| r.class == ResidualClass::AllocationCovered)
        })
        .count();
    println!(
        "bucketed={} scored={scored} allocation_covered={covered} report={}",
        report.owners.len(),
        output.display()
    );
    Ok(())
}

fn bucket_score(repository: &Path, owner: &Owner, output: &Path, entry: &mut BucketEntry) {
    bucket_lint(repository, owner, entry);
    if !entry.verdict.is_empty() {
        return;
    }
    let directory = output.join(stem(owner.address));
    match score(repository, owner, &directory, false, None) {
        Err(error) => {
            entry.verdict = "score_failed".into();
            entry.error = Some(error);
        }
        Ok(mut scored) => {
            if scored.residual.class == ResidualClass::AllocationUncovered {
                match score(repository, owner, &directory, true, None) {
                    Ok(decoded) => scored = decoded,
                    Err(error) => {
                        entry.verdict = "decoder_failed".into();
                        entry.error = Some(error);
                        return;
                    }
                }
            }
            record_score(entry, scored);
        }
    }
}

fn bucket_lint(repository: &Path, owner: &Owner, entry: &mut BucketEntry) {
    let source = match read(repository.join(owner.candidate.as_deref().unwrap_or(""))) {
        Ok(value) => value,
        Err(error) => {
            entry.verdict = "score_failed".into();
            entry.error = Some(error);
            return;
        }
    };
    let reference = match read(repository.join(&owner.reference)) {
        Ok(value) => value,
        Err(error) => {
            entry.verdict = "score_failed".into();
            entry.error = Some(error);
            return;
        }
    };
    let lint_report = lint(&source, &reference, None);
    entry.lint = serde_json::to_value(&lint_report).ok();
    if !lint_report.passes() {
        entry.verdict = "idiom_lint_parked".into();
    }
}

fn record_score(entry: &mut BucketEntry, scored: RenderOutput) {
    entry.allocator_repair = scored
        .allocator
        .as_ref()
        .and_then(|report| report.repair.as_ref())
        .map(|plan| plan.label());
    entry.allocator_dimensions = scored
        .allocator
        .as_ref()
        .map(|report| {
            report
                .dimensions
                .iter()
                .map(|dimension| dimension.to_string())
                .collect()
        })
        .unwrap_or_default();
    entry.residual = Some(scored.residual);
    entry.verdict = "scored".into();
}

fn bucket_translation_units(
    repository: &Path,
    inventory: &Inventory,
    output: &Path,
) -> Result<BTreeMap<u32, Result<RenderOutput, String>>, String> {
    let units = TranslationUnits::load(repository)?;
    let mut grouped = BTreeMap::<String, Vec<&Owner>>::new();
    for owner in inventory
        .owners
        .iter()
        .filter(|owner| owner.scope == Scope::TranslationUnit)
    {
        grouped
            .entry(
                owner
                    .unit
                    .clone()
                    .ok_or("translation-unit owner has no unit id")?,
            )
            .or_default()
            .push(owner);
    }
    let mut scores = BTreeMap::new();
    for (id, owners) in grouped {
        let unit = units
            .unit(&id)
            .ok_or_else(|| format!("unknown translation unit {id}"))?;
        match score_translation_unit(
            repository,
            unit,
            &owners,
            &output.join(format!("unit-{id}")),
        ) {
            Ok(outputs) => scores.extend(
                outputs
                    .into_iter()
                    .map(|(address, output)| (address, Ok(output))),
            ),
            Err(error) => scores.extend(
                owners
                    .into_iter()
                    .map(|owner| (owner.address, Err(error.clone()))),
            ),
        }
    }
    Ok(scores)
}

fn score_translation_unit(
    repository: &Path,
    unit: &TranslationUnit,
    selected: &[&Owner],
    work: &Path,
) -> Result<BTreeMap<u32, RenderOutput>, String> {
    if unit.overlay.is_some() {
        return Err("main-ROM wave inventory cannot score an overlay translation unit".into());
    }
    let first = unit
        .owners
        .first()
        .ok_or("translation unit has no owners")?;
    let configuration = CandidateCompilerConfiguration {
        family: Some(CandidateCompilerFamily::Routed),
        absolute_symbols: unit.canonical_symbols()?,
        ..Default::default()
    };
    let source = unit.source.to_string_lossy().into_owned();
    let rom = repository
        .join("roms/gs1-en.gba")
        .to_string_lossy()
        .into_owned();
    let shared_object = work.join("score").join(format!("{:08x}.o", first.address));
    let wanted = selected
        .iter()
        .map(|owner| (owner.address, *owner))
        .collect::<BTreeMap<_, _>>();
    let mut outputs = BTreeMap::new();
    for (index, member) in unit.owners.iter().enumerate() {
        let selected_owner = wanted.get(&member.address).copied();
        if index != 0 && selected_owner.is_none() {
            continue;
        }
        let options = CandidateOptions {
            source: source.clone(),
            rom: Some(rom.clone()),
            work: Some(work.join("score").to_string_lossy().into_owned()),
            flags: Vec::new(),
            configuration: configuration.clone(),
            target: CompilerTarget::Gs1,
            owner: Some(member.address),
            overlay: None,
            unit: Some(unit.id.clone()),
            precompiled_object: (index != 0).then(|| shared_object.to_string_lossy().into_owned()),
            size: Some(member.extent),
            align: true,
            first: false,
            allocator_order: false,
            asm: false,
            patch: None,
        };
        let mut scored = render(repository, &options)?;
        if selected_owner.is_some() && scored.residual.class == ResidualClass::AllocationUncovered {
            let mut decoded_options = options;
            decoded_options.allocator_order = true;
            scored = render(repository, &decoded_options)?;
        }
        if selected_owner.is_some() {
            outputs.insert(member.address, scored);
        }
    }
    Ok(outputs)
}

fn pack_command(args: Args) -> Result<(), String> {
    let repository = root();
    let inventory = inventory(repository)?;
    let families = families(repository, &args.family_index)?;
    let output = args
        .output
        .unwrap_or_else(|| repository.join("out/gs1-en/waves/packs"));
    let mut owners = inventory.owners.iter().collect::<Vec<_>>();
    owners.sort_by_key(|o| (std::cmp::Reverse(o.size), o.address));
    owners.truncate(args.limit.unwrap_or(owners.len()));
    let mut generated = 0;
    for owner in owners {
        let dir = output.join(stem(owner.address));
        let verdict = pack(repository, &dir, owner, families.get(&owner.owner));
        match verdict {
            Ok(value) => {
                write_json(&dir.join("pack.json"), &value)?;
                generated += 1;
            }
            Err(error) => write_json(
                &dir.join("verdict.json"),
                &json!({"owner": owner.owner, "verdict":"template_pack_failed", "error":error}),
            )?,
        }
    }
    println!("packs_generated={generated} output={}", output.display());
    Ok(())
}

fn wave_command(args: Args) -> Result<(), String> {
    let repository = root();
    let inventory = inventory(repository)?;
    let bucket = load_bucket(repository, &args.bucket, &inventory)?;
    let families = families(repository, &args.family_index)?;
    let limit = args.limit.unwrap_or(WAVE_SIZE);
    if limit < WAVE_SIZE {
        return Err(format!(
            "an autonomous wave must contain at least {WAVE_SIZE} owners"
        ));
    }
    let mut ranked = wave_safe(&inventory, &bucket);
    ranked.truncate(limit);
    if ranked.len() < limit {
        return Err(format!(
            "only {} named, scored standalone owners are wave-safe",
            ranked.len()
        ));
    }
    let signature = wave_signature(repository, &args, &inventory, &ranked)?;
    let output = args.output.unwrap_or_else(|| {
        repository
            .join("out/gs1-en/waves")
            .join(format!("wave-{}-{limit}", &signature[..12]))
    });
    write_json(&output.join("inventory.json"), &inventory)?;
    write_json(
        &output.join("dossier-mine.json"),
        &mine_dossiers(&repository.join("games/gs1/recon/en/main"))?,
    )?;
    let mut records = Vec::new();
    for (owner, _) in ranked {
        let directory = output.join("owners").join(stem(owner.address));
        let record = wave_owner(
            repository,
            &directory,
            owner,
            families.get(&owner.owner),
            args.stall_budget,
        );
        write_json(&directory.join("verdict.json"), &record)?;
        records.push(record);
    }
    let adopted = integrate_exact(repository, &output, &mut records, args.apply)?;
    let mut classes = BTreeMap::<String, usize>::new();
    for record in &records {
        if let Some(class) = record["residual"]["class"].as_str() {
            *classes.entry(class.into()).or_default() += 1;
        }
    }
    let needs_human = "smart_queue score_failed template_pack_failed idiom_lint_parked named_repair_parked stall_budget_exhausted integration_parked";
    let smart = records
        .iter()
        .filter(|record| {
            record["verdict"]
                .as_str()
                .is_some_and(|verdict| needs_human.split_ascii_whitespace().any(|v| v == verdict))
        })
        .map(|r| json!({"owner":r["owner"],"reason":r["error"],"class":r["residual"]["class"]}))
        .collect::<Vec<_>>();
    let human_routed = smart.len();
    let verdict = |name: &str| {
        records
            .iter()
            .filter(|record| record["verdict"] == name)
            .count()
    };
    let class = |name: &str| classes.get(name).copied().unwrap_or(0);
    let template_attempts = records
        .iter()
        .filter_map(|record| record["template_attempt"]["attempted"].as_u64())
        .sum::<u64>();
    let template_hits = records
        .iter()
        .filter(|record| !record["template_attempt"]["exact_rank"].is_null())
        .count();
    let template_improvements = records
        .iter()
        .filter(|record| !record["template_attempt"]["improved_rank"].is_null())
        .count();
    let report = json!({
        "schema_version":2,
        "wave_id":format!("wave-{}-{limit}",&signature[..12]),
        "derivation":"fresh bucket -> ranked retargeted siblings -> pre-score lint -> typed triage -> named finite operation -> exact integrator or parked verdict",
        "selected_order":"allocation-covered; exact extent; differing halfwords; descending bytes; address",
        "stall_budget_compiles":args.stall_budget,
        "class_counts":classes,
        "owners":records,
        "funnel":{
            "inventory_debt":inventory.owners.len(),"selected":limit,
            "template_attempts":template_attempts,"template_improved":template_improvements,
            "template_pack_failed":verdict("template_pack_failed"),
            "idiom_lint_parked":verdict("idiom_lint_parked"),"score_failed":verdict("score_failed"),
            "uncovered_floor":class("allocation_uncovered"),"smart_queue":verdict("smart_queue"),
            "playbook_parked":verdict("playbook_parked"),
            "named_repair_parked":verdict("named_repair_parked"),
            "stall_budget_exhausted":verdict("stall_budget_exhausted"),
            "exact_ready_for_integration":verdict("exact_ready_for_integration"),
            "integration_parked":verdict("integration_parked"),"adopted":adopted
        },
        "template_hit_rate":{
            "measured":true,"attempts":template_attempts,"attributed_exact_owners":template_hits,
            "improved_owners":template_improvements,
            "basis_points":if limit==0{0}else{template_hits*10000/limit},
            "attribution":"Only exact output from a recorded retargeted template rank counts as a hit."
        },
        "success_metric":{
            "owners_adopted":adopted,"apply_requested":args.apply,"human_routed":human_routed,
            "note":"Zero is a valid measured result; routing, attempts, and score movement are diagnostics."
        },
        "smart_queue":smart
    });
    write_json(&output.join("smart-queue.json"), &report["smart_queue"])?;
    write_json(&output.join("wave-report.json"), &report)?;
    println!(
        "wave={} selected={limit} template_attempts={template_attempts} exact_ready={} smart_queue={} adopted={adopted} output={}",
        report["wave_id"].as_str().unwrap_or("wave"),
        verdict("exact_ready_for_integration"),
        report["smart_queue"].as_array().map_or(0, Vec::len),
        output.display()
    );
    Ok(())
}

fn wave_owner(
    repository: &Path,
    directory: &Path,
    owner: &Owner,
    target: Option<&FamilyTarget>,
    stall: usize,
) -> Value {
    let mut stages = vec!["discovered"];
    let mut budget = Budget {
        limit: stall,
        used: 0,
    };
    let failed = |verdict: &str, error: String, stages: &[&str], used: usize| json!({"owner":owner.owner,"name":owner.name,"size":owner.size,"verdict":verdict,"error":error,"stages":stages,"compiles_used":used,"stall_budget":stall});
    let pack = match pack(repository, directory, owner, target) {
        Ok(v) => v,
        Err(e) => return failed("template_pack_failed", e, &stages, budget.used),
    };
    stages.push("packed");
    let source = match read(repository.join(owner.candidate.as_deref().unwrap_or(""))) {
        Ok(v) => v,
        Err(e) => return failed("score_failed", e, &stages, budget.used),
    };
    let reference = match read(repository.join(&owner.reference)) {
        Ok(v) => v,
        Err(e) => return failed("score_failed", e, &stages, budget.used),
    };
    let lint_report = lint(&source, &reference, None);
    stages.push("linted");
    if !lint_report.passes() {
        return json!({"owner":owner.owner,"name":owner.name,"size":owner.size,"verdict":"idiom_lint_parked","error":"pre-score idiom lint finding","stages":stages,"compiles_used":budget.used,"stall_budget":stall,"pack":pack,"lint":lint_report});
    }
    if let Err(e) = budget.take() {
        return failed("stall_budget_exhausted", e, &stages, budget.used);
    }
    let mut scored = match score(repository, owner, directory, false, None) {
        Ok(v) => v,
        Err(e) => return failed("score_failed", e, &stages, budget.used),
    };
    stages.push("draft_scored");
    let templates = match template_attempts(
        repository,
        directory,
        owner,
        target,
        &source,
        &reference,
        &scored,
        &mut budget,
    ) {
        Ok(run) => run,
        Err(error) => return failed("template_pack_failed", error, &stages, budget.used),
    };
    let TemplateRun {
        report: template_report,
        best: template_best,
    } = templates;
    let mut active_source = source.clone();
    if let Some((candidate, output, _)) = template_best {
        active_source = candidate;
        scored = output;
    }
    stages.push("templates_scored");
    if scored.residual.class == ResidualClass::AllocationUncovered {
        if let Err(e) = budget.take() {
            return failed("stall_budget_exhausted", e, &stages, budget.used);
        }
        let patch_path = (active_source != source).then(|| directory.join("templates/active.diff"));
        if let Some(path) = &patch_path {
            if let Err(error) = write(
                path,
                unified(
                    owner.candidate.as_deref().unwrap_or("candidate.c"),
                    &source,
                    &active_source,
                )
                .as_bytes(),
            ) {
                return failed("score_failed", error, &stages, budget.used);
            }
        }
        scored = match score(repository, owner, directory, true, patch_path.as_deref()) {
            Ok(v) => v,
            Err(e) => {
                return failed(
                    "score_failed",
                    format!("allocator decoder: {e}"),
                    &stages,
                    budget.used,
                )
            }
        };
        stages.push("decoded");
    }
    let allocator = scored
        .allocator
        .as_ref()
        .map(|a| json!({"repair":a.repair.as_ref().map(|p|p.label()),"dimensions":a.dimensions}));
    let mut residual = scored.residual.clone();
    if residual.class == ResidualClass::AllocationCovered {
        stages.push("repair_planned");
        let repair = match repair(
            repository,
            directory,
            owner,
            &source,
            &active_source,
            &scored,
            &mut budget,
        ) {
            Ok(v) => v,
            Err(e) => {
                return json!({"owner":owner.owner,"name":owner.name,"size":owner.size,"verdict":"named_repair_parked","error":e,"stages":stages,"compiles_used":budget.used,"stall_budget":stall,"pack":pack,"lint":lint_report,"template_attempt":template_report,"residual":residual,"allocator":allocator})
            }
        };
        stages.push("repair_scored");
        if let Some((candidate, best)) = repair.best.as_ref() {
            active_source = candidate.clone();
            residual = best.residual.clone();
        }
        let exact = residual.class == ResidualClass::Exact;
        let exact_source = if exact {
            match stage_exact(directory, owner, &active_source) {
                Ok(path) => Some(path),
                Err(error) => return failed("score_failed", error, &stages, budget.used),
            }
        } else {
            None
        };
        return json!({"owner":owner.owner,"name":owner.name,"size":owner.size,"verdict":if exact{"exact_ready_for_integration"}else if repair.exhausted{"stall_budget_exhausted"}else{"named_repair_parked"},"error":if exact{Value::Null}else{json!("finite decoder-named repair did not close exactly")},"stages":stages,"compiles_used":budget.used,"stall_budget":stall,"pack":pack,"lint":lint_report,"template_attempt":template_report,"residual":residual,"allocator":allocator,"repair_attempt":repair.report,"exact_source":exact_source});
    }
    if residual.class == ResidualClass::Exact {
        let exact_source = match stage_exact(directory, owner, &active_source) {
            Ok(path) => Some(path),
            Err(error) => return failed("score_failed", error, &stages, budget.used),
        };
        return json!({"owner":owner.owner,"name":owner.name,"size":owner.size,"verdict":"exact_ready_for_integration","stages":stages,"compiles_used":budget.used,"stall_budget":stall,"pack":pack,"lint":lint_report,"template_attempt":template_report,"residual":residual,"allocator":allocator,"exact_source":exact_source});
    }
    let Some(playbook) = residual.playbook.clone() else {
        return json!({"owner":owner.owner,"name":owner.name,"size":owner.size,"verdict":"smart_queue","error":"triage class has no executable playbook","stages":stages,"compiles_used":budget.used,"stall_budget":stall,"pack":pack,"lint":lint_report,"template_attempt":template_report,"residual":residual,"allocator":allocator});
    };
    if let Err(e) = work_order(
        directory,
        owner,
        &residual,
        &playbook,
        allocator.as_ref(),
        &pack,
        stall,
    ) {
        return failed("smart_queue", e, &stages, budget.used);
    }
    stages.push("playbook_recorded");
    json!({"owner":owner.owner,"name":owner.name,"size":owner.size,"verdict":"playbook_parked","error":"classified playbook has no executable catalog operation; bounded work order recorded after ranked template attempts","stages":stages,"compiles_used":budget.used,"stall_budget":stall,"pack":pack,"lint":lint_report,"template_attempt":template_report,"residual":residual,"allocator":allocator,"playbook":playbook})
}

fn integrate_exact(
    repository: &Path,
    output: &Path,
    records: &mut [Value],
    apply: bool,
) -> Result<usize, String> {
    let mut ready = Vec::new();
    for (index, record) in records.iter().enumerate() {
        if record["verdict"] != "exact_ready_for_integration" {
            continue;
        }
        let owner = record["owner"]
            .as_str()
            .ok_or("exact record has no owner")?;
        let source = record["exact_source"]
            .as_str()
            .ok_or_else(|| format!("{owner}: exact record has no staged source"))?;
        ready.push((index, SourceOwner::parse(owner)?, read(source)?));
    }
    if ready.is_empty() || !apply {
        return Ok(0);
    }
    let nonce = std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .map_err(|error| error.to_string())?
        .as_nanos();
    let candidates = output.join(format!(
        "integration-candidates-{}-{nonce}",
        std::process::id()
    ));
    fs::create_dir(&candidates).map_err(|error| format!("{}: {error}", candidates.display()))?;
    for (_, owner, source) in &ready {
        write(
            &candidates.join(format!("{}.c", owner.address_stem())),
            source.as_bytes(),
        )?;
    }
    let arguments = vec![
        "--apply".to_string(),
        candidates.to_string_lossy().into_owned(),
    ];
    if integrate_matches::entry(&arguments) != std::process::ExitCode::SUCCESS {
        return Err("exact integration gate failed".into());
    }
    let source_paths = SourcePaths::load(repository)?;
    let mut adopted = 0;
    for (index, owner, _) in ready {
        let installed = source_paths
            .registered_source_path(owner)
            .is_ok_and(|path| path.is_file());
        let retained = repository
            .join("games/gs1/asm")
            .join(format!("{}.s", owner.address_stem()))
            .is_file();
        if installed && !retained {
            records[index]["verdict"] = json!("adopted");
            records[index]["error"] = Value::Null;
            adopted += 1;
        } else {
            records[index]["verdict"] = json!("integration_parked");
            records[index]["error"] = json!(
                "exact gate did not install the registered source and retire retained assembly"
            );
        }
        write_json(
            &output
                .join("owners")
                .join(owner.address_stem())
                .join("verdict.json"),
            &records[index],
        )?;
    }
    Ok(adopted)
}

struct TemplateRun {
    report: Value,
    best: Option<(String, RenderOutput, usize)>,
}

fn template_attempts(
    repository: &Path,
    directory: &Path,
    owner: &Owner,
    target: Option<&FamilyTarget>,
    source: &str,
    reference: &str,
    baseline: &RenderOutput,
    budget: &mut Budget,
) -> Result<TemplateRun, String> {
    let mut attempts = Vec::new();
    let mut best = None;
    for (index, template) in target
        .into_iter()
        .flat_map(|target| &target.alternatives)
        .take(3)
        .enumerate()
    {
        let rank = index + 1;
        let template_source = read(repository.join(&template.source))?;
        let target_symbol = target.map_or(owner.symbol.as_str(), |target| target.symbol.as_str());
        let candidate = match prepare_template(template, target_symbol, &template_source, reference)
        {
            Ok(candidate) => candidate,
            Err(rejection) => {
                attempts.push(json!({"rank":rank,"owner":template.owner,"score_basis_points":template.score_basis_points,"compiled":false,"rejection":rejection}));
                continue;
            }
        };
        if budget.take().is_err() {
            break;
        }
        let work = directory.join("templates").join(format!("rank-{rank}"));
        let patch = work.join("candidate.diff");
        write(
            &patch,
            unified(
                owner.candidate.as_deref().unwrap_or("candidate.c"),
                source,
                &candidate,
            )
            .as_bytes(),
        )?;
        match score(repository, owner, &work, false, Some(&patch)) {
            Err(error) => attempts.push(json!({"rank":rank,"owner":template.owner,"score_basis_points":template.score_basis_points,"compiled":false,"error":error})),
            Ok(output) => {
                let exact = output.residual.class == ResidualClass::Exact;
                attempts.push(json!({"rank":rank,"owner":template.owner,"score_basis_points":template.score_basis_points,"compiled":true,"class":output.residual.class.label(),"differing_halfwords":output.differing_halfwords,"candidate_bytes":output.candidate_length,"reference_bytes":output.reference_length,"exact":exact}));
                if score_key(&output) < score_key(baseline)
                    && best.as_ref().is_none_or(|(_, current, _)| {
                        score_key(&output) < score_key(current)
                    })
                {
                    best = Some((candidate, output, rank));
                }
            }
        }
    }
    let best_source = if let Some((candidate, _, _)) = &best {
        let path = directory.join("templates/best.c");
        write(&path, candidate.as_bytes())?;
        write(&directory.join("skeleton.c"), candidate.as_bytes())?;
        Some(path.to_string_lossy().into_owned())
    } else {
        None
    };
    let report = json!({
        "attempted": attempts.len(),
        "compiled": attempts.iter().filter(|attempt| attempt["compiled"] == true).count(),
        "exact_rank": best.as_ref().and_then(|(_, output, rank)| (output.residual.class == ResidualClass::Exact).then_some(*rank)),
        "improved_rank": best.as_ref().map(|(_, _, rank)| *rank),
        "best_source": best_source,
        "attempts": attempts,
    });
    Ok(TemplateRun { report, best })
}

fn stage_exact(directory: &Path, owner: &Owner, source: &str) -> Result<String, String> {
    let path = directory
        .join("exact")
        .join(format!("{}.c", stem(owner.address)));
    write(&path, source.as_bytes())?;
    Ok(path.to_string_lossy().into_owned())
}

struct RepairRun {
    report: Value,
    best: Option<(String, RenderOutput)>,
    exhausted: bool,
}
fn repair(
    repository: &Path,
    directory: &Path,
    owner: &Owner,
    base_source: &str,
    source: &str,
    baseline: &RenderOutput,
    budget: &mut Budget,
) -> Result<RepairRun, String> {
    let decoder = baseline
        .allocator
        .as_ref()
        .ok_or("covered floor lacks decoder report")?;
    let plan = decoder
        .repair
        .as_ref()
        .ok_or("covered floor lacks structured repair")?;
    let permutation = permuter::parse_permutation(source, plan)?;
    let mut results = vec![
        json!({"choice":0,"mutations":[],"differing_halfwords":baseline.differing_halfwords,"exact":baseline.residual.class==ResidualClass::Exact,"class":baseline.residual.class.label(),"source_sha256":sha256::hex(source.as_bytes())}),
    ];
    let mut best: Option<(String, RenderOutput)> = None;
    let mut failures = 0;
    let mut exhausted = false;
    for choice in 1..permutation.count() {
        if budget.take().is_err() {
            exhausted = true;
            break;
        }
        let candidate = permutation.evaluate(choice)?;
        let work = directory.join("repair").join(format!("choice-{choice:02}"));
        let patch_path = work.join("repair.diff");
        write(
            &patch_path,
            unified(
                owner.candidate.as_deref().unwrap_or("candidate.c"),
                base_source,
                &candidate,
            )
            .as_bytes(),
        )?;
        match score(repository, owner, &work, false, Some(&patch_path)) {
            Err(_) => failures += 1,
            Ok(output) => {
                results.push(json!({"choice":choice,"mutations":permutation.mutations(choice).unwrap_or_default(),"differing_halfwords":output.differing_halfwords,"exact":output.residual.class==ResidualClass::Exact,"class":output.residual.class.label(),"source_sha256":sha256::hex(candidate.as_bytes())}));
                let better = score_key(&output) < score_key(baseline)
                    && best
                        .as_ref()
                        .is_none_or(|(_, current)| score_key(&output) < score_key(current));
                if better {
                    best = Some((candidate, output));
                }
            }
        }
    }
    let best_source = if let Some((source, _)) = &best {
        let path = directory.join("repair/best.c");
        write(&path, source.as_bytes())?;
        Some(path.to_string_lossy().into_owned())
    } else {
        None
    };
    let report = json!({"plan":plan.label(),"dimensions":permutation.dimensions(),"raw_choices":permutation.raw_count(),"unique_choices":permutation.count(),"attempted":results.len()+failures,"compile_failures":failures,"baseline_differing_halfwords":baseline.differing_halfwords,"improved":best.is_some(),"exact":best.as_ref().is_some_and(|(_,o)|o.residual.class==ResidualClass::Exact),"best_source":best_source,"choices":results});
    write_json(&directory.join("repair/report.json"), &report)?;
    Ok(RepairRun {
        report,
        best,
        exhausted,
    })
}

fn score_key(output: &RenderOutput) -> (bool, usize, usize) {
    (
        output.residual.class != ResidualClass::Exact,
        output.differing_halfwords,
        output.candidate_length.abs_diff(output.reference_length),
    )
}
fn unified(path: &str, old: &str, new: &str) -> String {
    let name = Path::new(path)
        .file_name()
        .and_then(|n| n.to_str())
        .unwrap_or("candidate.c");
    let mut patch = format!(
        "--- a/{name}\n+++ b/{name}\n@@ -1,{} +1,{} @@\n",
        old.lines().count(),
        new.lines().count()
    );
    for line in old.split_inclusive('\n') {
        patch.push('-');
        patch.push_str(line);
    }
    for line in new.split_inclusive('\n') {
        patch.push('+');
        patch.push_str(line);
    }
    patch
}

fn score(
    repository: &Path,
    owner: &Owner,
    work: &Path,
    allocator: bool,
    patch: Option<&Path>,
) -> Result<RenderOutput, String> {
    render(
        repository,
        &CandidateOptions {
            source: owner.candidate.clone().unwrap_or_default(),
            rom: Some(
                repository
                    .join("roms/gs1-en.gba")
                    .to_string_lossy()
                    .into_owned(),
            ),
            work: Some(work.join("score").to_string_lossy().into_owned()),
            flags: Vec::new(),
            configuration: CandidateCompilerConfiguration {
                family: Some(CandidateCompilerFamily::Routed),
                ..Default::default()
            },
            target: CompilerTarget::Gs1,
            owner: Some(owner.address),
            overlay: None,
            unit: None,
            precompiled_object: None,
            size: Some(owner.size),
            align: true,
            first: false,
            allocator_order: allocator,
            asm: false,
            patch: patch.map(|p| p.to_string_lossy().into_owned()),
        },
    )
}

fn pack(
    repository: &Path,
    directory: &Path,
    owner: &Owner,
    target: Option<&FamilyTarget>,
) -> Result<Value, String> {
    let status = match owner.scope {
        Scope::TranslationUnit => "translation_unit_parked",
        Scope::NoCandidate => "template_skeleton_only",
        Scope::StandaloneDraft => "draft_ready",
    };
    let candidate_path = owner.candidate.as_deref();
    let (candidate, seed_origin) = if let Some(path) = candidate_path {
        (read(repository.join(path))?, "semantic_candidate")
    } else if let Some((target, template)) = target.and_then(|target| {
        target
            .alternatives
            .first()
            .map(|template| (target, template))
    }) {
        let source = read(repository.join(&template.source))?;
        (
            retarget(template, &target.symbol, &source)?,
            "retargeted_exact_sibling",
        )
    } else {
        (
            format!(
                "/* {} has no semantic draft or ranked exact sibling yet. */\n",
                owner.owner
            ),
            "empty_parked_skeleton",
        )
    };
    write(&directory.join("candidate.c"), candidate.as_bytes())?;
    write(&directory.join("skeleton.c"), candidate.as_bytes())?;
    let mut templates = Vec::new();
    for (index, template) in target
        .into_iter()
        .flat_map(|target| &target.alternatives)
        .take(3)
        .enumerate()
    {
        let source = read(repository.join(&template.source))?;
        let packed = format!("template-{}.c", index + 1);
        write(&directory.join(&packed), source.as_bytes())?;
        let target_symbol = target.map_or(owner.symbol.as_str(), |target| target.symbol.as_str());
        let retargeted_source = retarget(template, target_symbol, &source)?;
        let retargeted = format!("retargeted-{}.c", index + 1);
        write(&directory.join(&retargeted), retargeted_source.as_bytes())?;
        templates.push(json!({"rank":index+1,"owner":template.owner,"symbol":template.symbol,"score_basis_points":template.score_basis_points,"source":template.source,"packed_source":packed,"source_sha256":sha256::hex(source.as_bytes()),"retargeted_source":retargeted,"retargeted_sha256":sha256::hex(retargeted_source.as_bytes()),"retargeted":retargeted_source!=source}));
    }
    let aggregate = repository.join("out/workbench/shared-aggregates/shared-aggregates.h");
    let aggregate = if aggregate.is_file() {
        let source = read(&aggregate)?;
        write(&directory.join("shared-aggregates.h"), source.as_bytes())?;
        json!({"source":aggregate,"packed_source":"shared-aggregates.h","source_sha256":sha256::hex(source.as_bytes())})
    } else {
        Value::Null
    };
    let template_count = templates.len();
    let value = json!({"schema_version":1,"owner":owner.owner,"name":owner.name,"scope":owner.scope,"status":status,"target_symbol":target.map(|target|target.symbol.as_str()).unwrap_or(&owner.symbol),"family_index_reference":target.map(|target|target.source.as_str()),"family":target.and_then(|target|target.family.as_deref()),"family_threshold_met":target.is_some_and(|target|target.family.is_some()),"seed_origin":seed_origin,"seed_source":candidate_path,"seed_sha256":sha256::hex(candidate.as_bytes()),"skeleton":"skeleton.c","templates":templates,"template_count":template_count,"shared_aggregates":aggregate,"raw_diff_included":false});
    write_json(&directory.join("pack.json"), &value)?;
    Ok(value)
}

fn work_order(
    directory: &Path,
    owner: &Owner,
    residual: &ResidualReport,
    playbook: &str,
    allocator: Option<&Value>,
    pack: &Value,
    stall: usize,
) -> Result<(), String> {
    if residual.playbook.is_none() {
        return Err("unclassified owner cannot receive work".into());
    }
    let template_count = pack["template_count"]
        .as_u64()
        .ok_or("template pack has no count")?;
    let body = json!({"owner":owner.owner,"name":owner.name,"candidate_source":owner.candidate,"candidate_sha256":owner.candidate_sha256,"residual":residual,"playbook":playbook,"decoder":allocator,"template_count":template_count,"stall_budget_compiles":stall,"pack_sha256":sha256::hex(&serde_json::to_vec(pack).map_err(|error|error.to_string())?),"allowed_files":["skeleton.c","WORK.json"],"constraints":["execute_named_playbook_only","no_raw_diff_improvisation","adopt_only_when_byte_exact"]});
    let input_sha256 = sha256::hex(&serde_json::to_vec(&body).map_err(|error| error.to_string())?);
    write_json(
        &directory.join("WORK.json"),
        &json!({"schema_version":1,"input_sha256":input_sha256,"work":body}),
    )
}

fn inventory(repository: &Path) -> Result<Inventory, String> {
    let manifest_text = read(repository.join(MANIFEST))?;
    let manifest: Manifest =
        serde_json::from_str(&manifest_text).map_err(|e| format!("{MANIFEST}: {e}"))?;
    let names = SourcePaths::load(repository)?;
    let units = TranslationUnits::load(repository)?;
    let mut owners = Vec::new();
    for region in manifest
        .regions
        .into_iter()
        .filter(|r| r.retention.as_deref().is_some_and(|v| DEBT.contains(&v)))
    {
        let address = u32::try_from(region.address)
            .map_err(|_| format!("bad address {:x}", region.address))?;
        let id = SourceOwner::Main(address);
        let relative = format!("games/gs1/recon/en/main/{}.c", stem(address));
        let translation_unit = units.unit_for_game_owner("gs1", id);
        let has_candidate = repository.join(&relative).is_file() || translation_unit.is_some();
        let unit = translation_unit.map(|u| u.id.clone());
        let scope = scope(unit.is_some(), has_candidate);
        let candidate = translation_unit
            .map(|u| u.source.to_string_lossy().into_owned())
            .or_else(|| repository.join(&relative).is_file().then_some(relative));
        let candidate_sha256 = candidate
            .as_deref()
            .map(|p| hash(repository.join(p)))
            .transpose()?;
        owners.push(Owner {
            owner: id.id(),
            address,
            size: region.size,
            retention: region.retention.unwrap_or_default(),
            reference: region.source,
            symbol: if region.symbol.is_empty() {
                id.legacy_name()
            } else {
                region.symbol
            },
            name: names.registered_name(id).map(str::to_string),
            scope,
            unit,
            candidate,
            candidate_sha256,
        });
    }
    owners.sort_by_key(|o| o.address);
    if owners.windows(2).any(|pair| pair[0].owner == pair[1].owner) {
        return Err("debt manifest contains a duplicate owner identity".into());
    }
    Ok(Inventory {
        schema_version: 1,
        manifest_sha256: sha256::hex(manifest_text.as_bytes()),
        owners,
    })
}

fn scope(unit: bool, candidate: bool) -> Scope {
    match (unit, candidate) {
        (true, _) => Scope::TranslationUnit,
        (false, true) => Scope::StandaloneDraft,
        _ => Scope::NoCandidate,
    }
}
fn inventory_counts(owners: &[Owner]) -> (usize, usize, usize, usize) {
    let scope = |wanted| owners.iter().filter(|owner| owner.scope == wanted).count();
    (
        scope(Scope::TranslationUnit),
        scope(Scope::StandaloneDraft),
        scope(Scope::NoCandidate),
        owners
            .iter()
            .filter(|o| o.scope == Scope::StandaloneDraft && o.name.is_some())
            .count(),
    )
}

fn families(repository: &Path, path: &Path) -> Result<BTreeMap<String, FamilyTarget>, String> {
    let mut index: FamilyIndex = json_file(&resolve(repository, path))?;
    if index.schema_version != 2 {
        return Err("family index must be schema 2".into());
    }
    let names = SourcePaths::load(repository)?;
    let units = TranslationUnits::load(repository)?;
    let aliases = crate::families::entry_aliases(repository)?;
    for target in &mut index.targets {
        let mut alternatives = Vec::new();
        for mut template in std::mem::take(&mut target.alternatives) {
            if standalone_template(repository, &names, &units, &template).is_err() {
                continue;
            }
            let owner = SourceOwner::parse(&template.owner).ok();
            let source = read(repository.join(&template.source))?;
            template.canonical_name = owner
                .and_then(|owner| names.registered_name(owner))
                .filter(|name| definition_count(&source, name) == 1)
                .map(str::to_string)
                .or_else(|| {
                    aliases
                        .get(&template.symbol)
                        .and_then(|names| unique_definition(names, &source))
                });
            if retarget(&template, &target.symbol, &source).is_ok() {
                alternatives.push(template);
            }
        }
        target.alternatives = alternatives;
    }
    Ok(index
        .targets
        .into_iter()
        .map(|t| (t.owner.clone(), t))
        .collect())
}

fn standalone_template(
    repository: &Path,
    names: &SourcePaths,
    units: &TranslationUnits,
    template: &Template,
) -> Result<(), String> {
    let owner = SourceOwner::parse(&template.owner)?;
    let relative = Path::new(&template.source);
    if units.unit_for_game_owner("gs1", owner).is_some()
        || units
            .units
            .iter()
            .any(|unit| unit.game == "gs1" && unit.source == relative)
    {
        return Err(format!("{} is a translation-unit template", template.owner));
    }
    match names.owners_for_path(&repository.join(relative)).as_slice() {
        [mapped] if *mapped == owner => Ok(()),
        [] => Err(format!("unregistered template source {}", template.source)),
        [_] => Err(format!("{} has the wrong source owner", template.owner)),
        _ => Err(format!("{} is a multi-owner source", template.source)),
    }
}

fn unique_definition(names: &[String], source: &str) -> Option<String> {
    let mut found = names
        .iter()
        .filter(|name| definition_count(source, name) == 1);
    let name = found.next()?.clone();
    found.next().is_none().then_some(name)
}

fn definition_count(source: &str, name: &str) -> usize {
    let literals =
        Regex::new(r#"(?ms)//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*'"#).unwrap();
    let code = literals.replace_all(source, |capture: &Captures| {
        capture[0]
            .bytes()
            .map(|byte| if byte == b'\n' { '\n' } else { ' ' })
            .collect::<String>()
    });
    let pattern = format!(
        r"(?m)^[ \t]*(?:[A-Za-z_][A-Za-z0-9_]*[\s*]+)+(\b{}\b)\s*\([^;{{}}]*\)\s*\{{",
        regex::escape(name)
    );
    Regex::new(&pattern).unwrap().captures_iter(&code).count()
}

fn retarget(template: &Template, target: &str, source: &str) -> Result<String, String> {
    let alias = template
        .canonical_name
        .as_deref()
        .filter(|name| *name != template.symbol);
    let source_name = match (
        definition_count(source, &template.symbol),
        alias.map_or(0, |name| definition_count(source, name)),
        alias,
    ) {
        (1, 0, _) => template.symbol.as_str(),
        (0, 1, Some(alias)) => alias,
        _ => return Err(format!("{} has no unique entry definition", template.owner)),
    };
    if definition_count(source, target) != 0 {
        return Err(format!("{} already defines {target}", template.owner));
    }
    let mut output = source.to_string();
    let (mut at, mut cursor) = (0, 0);
    for line in source.split_inclusive('\n') {
        cursor += line.len();
        if line.trim_start().starts_with("#include") {
            at = cursor;
        }
    }
    output.insert_str(
        at,
        &format!("#undef {source_name}\n#define {source_name} {target}\n\n"),
    );
    Ok(output)
}

fn prepare_template(
    template: &Template,
    target: &str,
    source: &str,
    reference: &str,
) -> Result<String, Value> {
    let candidate = retarget(template, target, source).map_err(|error| json!({"error":error}))?;
    let report = lint(&candidate, reference, None);
    report
        .passes()
        .then_some(candidate)
        .ok_or_else(|| json!({"error":"pre-score idiom lint finding","lint":report}))
}

fn load_bucket(
    repository: &Path,
    path: &Path,
    inventory: &Inventory,
) -> Result<BucketReport, String> {
    let report: BucketReport = json_file(&resolve(repository, path))?;
    if report.schema_version != 2
        || report.manifest_sha256 != inventory.manifest_sha256
        || report.scoring_environment_sha256 != scoring_environment(repository)?
        || report.owners.len() != inventory.owners.len()
    {
        return Err("bucket report is stale; run `compiler waves bucket`".into());
    }
    let identities = report
        .owners
        .iter()
        .map(|owner| owner.owner.as_str())
        .collect::<BTreeSet<_>>();
    if identities.len() != report.owners.len() {
        return Err("bucket report contains duplicate owner identities".into());
    }
    let current = inventory
        .owners
        .iter()
        .map(|o| (&o.owner, &o.candidate_sha256))
        .collect::<BTreeMap<_, _>>();
    if report
        .owners
        .iter()
        .any(|o| current.get(&o.owner) != Some(&&o.candidate_sha256))
    {
        return Err("bucket candidate hashes are stale; rerun bucket".into());
    }
    Ok(report)
}

fn wave_safe<'a>(
    inventory: &'a Inventory,
    bucket: &'a BucketReport,
) -> Vec<(&'a Owner, &'a BucketEntry)> {
    let entries = bucket
        .owners
        .iter()
        .map(|e| (e.owner.as_str(), e))
        .collect::<BTreeMap<_, _>>();
    let mut owners = inventory
        .owners
        .iter()
        .filter(|o| o.scope == Scope::StandaloneDraft && o.name.is_some())
        .filter_map(|o| {
            entries
                .get(o.owner.as_str())
                .copied()
                .filter(|e| e.verdict == "scored" && e.residual.as_ref().is_some())
                .map(|e| (o, e))
        })
        .collect::<Vec<_>>();
    owners.sort_by_key(|(o, e)| {
        let r = e.residual.as_ref().unwrap();
        (
            r.class != ResidualClass::AllocationCovered,
            r.facts.actual_bytes != r.facts.reference_bytes,
            r.facts.differing_halfwords,
            std::cmp::Reverse(o.size),
            o.address,
        )
    });
    owners
}

fn wave_signature(
    repository: &Path,
    args: &Args,
    inventory: &Inventory,
    selected: &[(&Owner, &BucketEntry)],
) -> Result<String, String> {
    let mut bytes = Vec::new();
    for path in [
        resolve(repository, &args.family_index),
        resolve(repository, &args.bucket),
        repository.join(CATALOG),
    ] {
        bytes.extend_from_slice(hash(path)?.as_bytes());
    }
    bytes.extend_from_slice(inventory.manifest_sha256.as_bytes());
    for (owner, _) in selected {
        bytes.extend_from_slice(owner.candidate_sha256.as_deref().unwrap_or("").as_bytes());
    }
    Ok(sha256::hex(&bytes))
}

fn parse_args(argv: &[String]) -> Result<Args, String> {
    let mut out = Args {
        output: None,
        family_index: FAMILIES.into(),
        bucket: BUCKET.into(),
        limit: None,
        stall_budget: STALL_BUDGET,
        apply: false,
    };
    let mut at = 0;
    while at < argv.len() {
        let value = |at: usize| {
            argv.get(at + 1)
                .ok_or_else(|| format!("{} requires a value", argv[at]))
        };
        match argv[at].as_str() {
            "--apply" => {
                out.apply = true;
                at += 1;
                continue;
            }
            "--output" => out.output = Some(value(at)?.into()),
            "--family-index" => out.family_index = value(at)?.into(),
            "--bucket" => out.bucket = value(at)?.into(),
            "--limit" => out.limit = Some(positive(value(at)?, "--limit")?),
            "--stall-budget" => out.stall_budget = positive(value(at)?, "--stall-budget")?,
            _ => return Err(USAGE.into()),
        }
        at += 2;
    }
    Ok(out)
}
fn positive(text: &str, flag: &str) -> Result<usize, String> {
    text.parse()
        .ok()
        .filter(|v| *v > 0)
        .ok_or_else(|| format!("{flag} must be positive"))
}
fn stem(address: u32) -> String {
    format!("{address:08x}")
}
fn resolve(repository: &Path, path: &Path) -> PathBuf {
    if path.is_absolute() {
        path.into()
    } else {
        repository.join(path)
    }
}
fn read(path: impl AsRef<Path>) -> Result<String, String> {
    let p = path.as_ref();
    fs::read_to_string(p).map_err(|e| format!("{}: {e}", p.display()))
}
fn hash(path: impl AsRef<Path>) -> Result<String, String> {
    let p = path.as_ref();
    fs::read(p)
        .map(|b| sha256::hex(&b))
        .map_err(|e| format!("{}: {e}", p.display()))
}
fn hash_tree(path: &Path) -> Result<String, String> {
    let mut paths = walkdir::WalkDir::new(path)
        .into_iter()
        .filter_map(Result::ok)
        .filter(|entry| entry.file_type().is_file())
        .map(|entry| entry.into_path())
        .collect::<Vec<_>>();
    paths.sort();
    let bytes = paths
        .iter()
        .map(fs::read)
        .collect::<Result<Vec<_>, _>>()
        .map_err(|error| error.to_string())?;
    Ok(sha256::hex(&bytes.concat()))
}
fn scoring_environment(repository: &Path) -> Result<String, String> {
    let bundle = compiler_core::routing::bundle();
    let executable = std::env::current_exe().map_err(|error| error.to_string())?;
    let paths = [
        executable,
        bundle.join("xgcc"),
        bundle.join("cc1"),
        bundle.join("cpp"),
        repository.join("roms/gs1-en.gba"),
        repository.join("games/gs1/source-paths.json"),
        repository.join("games/gs1/recon/translation-units.json"),
        repository.join(CATALOG),
    ];
    let mut fingerprints = paths.iter().map(hash).collect::<Result<String, String>>()?;
    fingerprints.push_str(&hash_tree(&repository.join("games/gs1/include"))?);
    Ok(sha256::hex(fingerprints.as_bytes()))
}
fn json_file<T: for<'de> Deserialize<'de>>(path: &Path) -> Result<T, String> {
    serde_json::from_str(&read(path)?).map_err(|e| format!("{}: {e}", path.display()))
}
fn write(path: &Path, bytes: &[u8]) -> Result<(), String> {
    fs::create_dir_all(path.parent().ok_or("output has no parent")?).map_err(|e| e.to_string())?;
    let temp = path.with_extension(format!("tmp-{}", std::process::id()));
    fs::write(&temp, bytes).map_err(|e| format!("{}: {e}", temp.display()))?;
    fs::rename(&temp, path).map_err(|e| format!("{}: {e}", path.display()))
}
fn write_json(path: &Path, value: &impl Serialize) -> Result<(), String> {
    let mut text = serde_json::to_string_pretty(value).map_err(|e| e.to_string())?;
    text.push('\n');
    write(path, text.as_bytes())
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn authoritative_registry_rejects_translation_unit_templates() {
        let repository = root();
        let names = SourcePaths::load(repository).unwrap();
        let units = TranslationUnits::load(repository).unwrap();
        let unit = units
            .units
            .iter()
            .find(|unit| unit.game == "gs1" && unit.owners.len() > 1)
            .unwrap();
        let owner = unit.source_owner(unit.owners[0].address).unwrap();
        let template = Template {
            owner: owner.id(),
            symbol: owner.legacy_name(),
            source: unit.source.to_string_lossy().into_owned(),
            score_basis_points: 10_000,
            canonical_name: None,
        };
        assert!(standalone_template(repository, &names, &units, &template).is_err());
    }
    #[test]
    fn retarget_requires_one_definition_and_lints_before_scoring() {
        let template = Template {
            owner: "main:08000000".into(),
            symbol: "Func_08000000".into(),
            source: "fixture.c".into(),
            score_basis_points: 10_000,
            canonical_name: None,
        };
        let source =
            "#include \"types.h\"\n/* Func_08000000() {} */\nvoid Func_08000000(void) {}\n";
        let output = retarget(&template, "Func_08000004", source).unwrap();
        assert!(output.contains("#define Func_08000000 Func_08000004"));
        assert!(retarget(&template, "Func_08000004", "/* Func_08000000() {} */").is_err());
        let wrong = "void Func_08000000(void) { Func_080072f4(); }";
        assert!(
            prepare_template(&template, "Func_08000004", wrong, "bl Func_080072f4").unwrap_err()
                ["lint"]
                .is_object()
        );
    }
}
