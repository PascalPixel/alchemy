use candidate_compiler::verify::{CandidateCompilerConfiguration, CandidateCompilerFamily};
use candidate_show::{
    cli::Options as CandidateOptions,
    idioms::lint,
    render::{render, RenderOutput},
    triage::{ResidualClass, ResidualReport},
};
use compiler_core::{
    routing::{root, CompilerTarget},
    sha256,
    source_paths::{SourceOwner, SourcePaths},
    translation_units::{TranslationUnit, TranslationUnits},
};
use serde::{Deserialize, Serialize};
use serde_json::{json, Value};
use std::{
    collections::{BTreeMap, BTreeSet},
    fs,
    path::{Path, PathBuf},
};

const USAGE: &str = "usage: compiler waves <inventory|bucket|pack|dispatch> [--output PATH] [--family-index FILE] [--bucket FILE] [--limit N] [--stall-budget N]";
const MANIFEST: &str = "out/gs1-en/full/asm/manifest.json";
const FAMILIES: &str = "out/gs1-en/reports/compiler-families.json";
const BUCKET: &str = "out/gs1-en/waves/bucket.json";
const PACKS: &str = "out/gs1-en/waves/packs";
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
        "dispatch" => dispatch_command(args),
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
    match score(repository, owner, &directory, false) {
        Err(error) => {
            entry.verdict = "score_failed".into();
            entry.error = Some(error);
        }
        Ok(mut scored) => {
            if scored.residual.class == ResidualClass::AllocationUncovered {
                match score(repository, owner, &directory, true) {
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
                    .ok_or("translation-unit owner has no id")?,
            )
            .or_default()
            .push(owner);
    }
    let mut scores = BTreeMap::new();
    for (id, owners) in grouped {
        let unit = units
            .unit(&id)
            .ok_or_else(|| format!("unknown unit {id}"))?;
        match score_translation_unit(
            repository,
            unit,
            &owners,
            &output.join(format!("unit-{id}")),
        ) {
            Ok(found) => scores.extend(found.into_iter().map(|row| (row.0, Ok(row.1)))),
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
        return Err("main-ROM inventory cannot score an overlay unit".into());
    }
    let first = unit
        .owners
        .first()
        .ok_or("translation unit has no owners")?;
    let source = unit.source.to_string_lossy().into_owned();
    let rom = repository
        .join("roms/gs1-en.gba")
        .to_string_lossy()
        .into_owned();
    let object = work.join("score").join(format!("{:08x}.o", first.address));
    let wanted = selected
        .iter()
        .map(|owner| (owner.address, *owner))
        .collect::<BTreeMap<_, _>>();
    let mut outputs = BTreeMap::new();
    for (index, member) in unit.owners.iter().enumerate() {
        if index != 0 && !wanted.contains_key(&member.address) {
            continue;
        }
        let mut options = CandidateOptions {
            source: source.clone(),
            rom: Some(rom.clone()),
            work: Some(work.join("score").to_string_lossy().into_owned()),
            flags: Vec::new(),
            configuration: CandidateCompilerConfiguration {
                family: Some(CandidateCompilerFamily::Routed),
                absolute_symbols: unit.canonical_symbols()?,
                ..Default::default()
            },
            target: CompilerTarget::Gs1,
            owner: Some(member.address),
            overlay: None,
            unit: Some(unit.id.clone()),
            precompiled_object: (index != 0).then(|| object.to_string_lossy().into_owned()),
            size: Some(member.extent),
            align: true,
            first: false,
            allocator_order: false,
            asm: false,
            patch: None,
        };
        let mut scored = render(repository, &options)?;
        if wanted.contains_key(&member.address)
            && scored.residual.class == ResidualClass::AllocationUncovered
        {
            options.allocator_order = true;
            scored = render(repository, &options)?;
        }
        if wanted.contains_key(&member.address) {
            outputs.insert(member.address, scored);
        }
    }
    Ok(outputs)
}

fn pack_command(args: Args) -> Result<(), String> {
    let repository = root();
    if args.output.is_some() {
        return Err("waves pack writes the canonical hashed pack directory; omit --output".into());
    }
    let inventory = inventory(repository)?;
    let families = families(repository, &args.family_index)?;
    let family_sha256 = hash(resolve(repository, &args.family_index))?;
    let output = repository.join(PACKS);
    let mut owners = inventory.owners.iter().collect::<Vec<_>>();
    owners.sort_by_key(|o| (std::cmp::Reverse(o.size), o.address));
    owners.truncate(args.limit.unwrap_or(owners.len()));
    let mut generated = 0;
    for owner in owners {
        let dir = output.join(stem(owner.address));
        let verdict = pack(
            repository,
            &dir,
            owner,
            families.get(&owner.owner),
            &inventory.manifest_sha256,
            &family_sha256,
        );
        match verdict {
            Ok(_) => {
                fs::remove_file(dir.join("verdict.json")).ok();
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

fn dispatch_command(args: Args) -> Result<(), String> {
    let repository = root();
    let inventory = inventory(repository)?;
    let bucket = load_bucket(repository, &args.bucket, &inventory)?;
    let limit = args.limit.unwrap_or(WAVE_SIZE);
    if limit < WAVE_SIZE {
        return Err(format!(
            "an autonomous dispatch must contain at least {WAVE_SIZE} owners"
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
            .join(format!("dispatch-{}-{limit}", &signature[..12]))
    });
    write_json(&output.join("inventory.json"), &inventory)?;
    let mut records = Vec::new();
    for (owner, entry) in ranked {
        let directory = output.join("owners").join(stem(owner.address));
        let record = wave_owner(
            repository,
            &directory,
            owner,
            entry,
            &args.family_index,
            &inventory,
            args.stall_budget,
        );
        write_json(&directory.join("verdict.json"), &record)?;
        records.push(record);
    }
    let classes = record_counts(&records, |record| record["residual"]["class"].as_str());
    let verdicts = record_counts(&records, |record| record["verdict"].as_str());
    let smart = records
        .iter()
        .filter(|record| record["verdict"] != "work_order_ready")
        .map(|r| json!({"owner":r["owner"],"reason":r["error"],"class":r["residual"]["class"]}))
        .collect::<Vec<_>>();
    let templates = records
        .iter()
        .filter_map(|record| record["template_count"].as_u64())
        .sum::<u64>();
    let ready = verdicts.get("work_order_ready").copied().unwrap_or(0);
    let smart_count = smart.len();
    let report = json!({
        "schema_version":2,
        "dispatch_id":format!("dispatch-{}-{limit}",&signature[..12]),
        "stall_budget_compiles":args.stall_budget,
        "class_counts":classes,
        "verdict_counts":verdicts,
        "owners":records,
        "funnel":{
            "inventory_debt":inventory.owners.len(),"selected":limit,
            "packed_templates":templates,"work_order_ready":ready,"smart_queue":smart_count
        },
        "smart_queue":smart
    });
    write_json(&output.join("smart-queue.json"), &report["smart_queue"])?;
    write_json(&output.join("dispatch-report.json"), &report)?;
    println!(
        "dispatch={} selected={limit} packed_templates={templates} work_order_ready={ready} smart_queue={smart_count} output={}",
        report["dispatch_id"].as_str().unwrap_or("dispatch"),
        output.display()
    );
    Ok(())
}

fn record_counts(
    records: &[Value],
    value: impl for<'a> Fn(&'a Value) -> Option<&'a str>,
) -> BTreeMap<String, usize> {
    let mut counts = BTreeMap::new();
    for label in records.iter().filter_map(value) {
        *counts.entry(label.into()).or_default() += 1;
    }
    counts
}

fn wave_owner(
    repository: &Path,
    directory: &Path,
    owner: &Owner,
    entry: &BucketEntry,
    family_index: &Path,
    inventory: &Inventory,
    stall: usize,
) -> Value {
    let (pack_path, pack) = match load_pack(repository, owner, family_index, inventory) {
        Ok(v) => v,
        Err(error) => return wave_record(owner, entry, "template_pack_failed", Some(error), 0),
    };
    let residual = entry.residual.as_ref().expect("wave-safe entry is scored");
    let Some(playbook) = residual.class.playbook() else {
        return wave_record(
            owner,
            entry,
            "smart_queue",
            Some("triage class has no executable playbook".into()),
            pack["template_count"].as_u64().unwrap_or(0),
        );
    };
    let allocator = json!({
        "repair":entry.allocator_repair,
        "dimensions":entry.allocator_dimensions,
    });
    if let Err(error) = work_order(
        directory, owner, residual, playbook, &allocator, &pack_path, &pack, stall,
    ) {
        return wave_record(owner, entry, "smart_queue", Some(error), 0);
    }
    wave_record(
        owner,
        entry,
        "work_order_ready",
        None,
        pack["template_count"].as_u64().unwrap_or(0),
    )
}

fn wave_record(
    owner: &Owner,
    entry: &BucketEntry,
    verdict: &str,
    error: Option<String>,
    template_count: u64,
) -> Value {
    json!({"owner":owner.owner,"name":owner.name,"size":owner.size,"verdict":verdict,"error":error,"template_count":template_count,"residual":entry.residual})
}

fn load_pack(
    repository: &Path,
    owner: &Owner,
    family_index: &Path,
    inventory: &Inventory,
) -> Result<(PathBuf, Value), String> {
    let path = repository
        .join(PACKS)
        .join(stem(owner.address))
        .join("pack.json");
    let pack: Value = json_file(&path)?;
    let family_sha256 = hash(resolve(repository, family_index))?;
    let fresh = pack["schema_version"] == 3
        && pack["owner"] == owner.owner
        && pack["candidate_sha256"] == json!(owner.candidate_sha256)
        && pack["manifest_sha256"] == inventory.manifest_sha256
        && pack["family_index_sha256"] == family_sha256
        && pack["reference_sha256"] == hash(repository.join(&owner.reference))?;
    if !fresh {
        return Err(format!(
            "{} is stale; rerun `compiler waves pack`",
            path.display()
        ));
    }
    let directory = path.parent().ok_or("pack has no directory")?;
    let skeleton = pack["skeleton"].as_str().ok_or("pack has no skeleton")?;
    if hash(directory.join(skeleton))? != pack["seed_sha256"].as_str().unwrap_or("") {
        return Err(format!("{} skeleton hash is stale", owner.owner));
    }
    for template in pack["templates"]
        .as_array()
        .ok_or("pack has no template array")?
    {
        let source = template["source"]
            .as_str()
            .ok_or("template has no source")?;
        let packed = template["packed_source"]
            .as_str()
            .ok_or("template has no packed source")?;
        let retargeted = template["retargeted_source"]
            .as_str()
            .ok_or("template has no retargeted source")?;
        if hash(repository.join(source))? != template["source_sha256"].as_str().unwrap_or("")
            || hash(directory.join(packed))? != template["source_sha256"].as_str().unwrap_or("")
            || hash(directory.join(retargeted))?
                != template["retargeted_sha256"].as_str().unwrap_or("")
        {
            return Err(format!("{} template pack is stale", owner.owner));
        }
    }
    if let Some(m2c) = pack["m2c"].as_object() {
        for (path, digest) in [("seed", "seed_sha256"), ("context", "context_sha256")] {
            if let Some(relative) = m2c[path].as_str() {
                if hash(directory.join(relative))? != m2c[digest].as_str().unwrap_or("") {
                    return Err(format!("{} m2c pack is stale", owner.owner));
                }
            }
        }
    }
    Ok((path, pack))
}

fn score(
    repository: &Path,
    owner: &Owner,
    work: &Path,
    allocator: bool,
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
            patch: None,
        },
    )
}

fn pack(
    repository: &Path,
    directory: &Path,
    owner: &Owner,
    target: Option<&FamilyTarget>,
    manifest_sha256: &str,
    family_index_sha256: &str,
) -> Result<Value, String> {
    let status = match owner.scope {
        Scope::TranslationUnit => "translation_unit_parked",
        Scope::NoCandidate => "m2c_seed_ready",
        Scope::StandaloneDraft => "draft_ready",
    };
    let candidate_path = owner.candidate.as_deref();
    let mut m2c = Value::Null;
    let (candidate, seed_origin, seed_source) = if let Some(path) = candidate_path {
        (
            read(repository.join(path))?,
            "semantic_candidate",
            Some(path),
        )
    } else {
        let seed = m2c_seed(repository, directory, owner, target)?;
        let candidate = read(&seed.source)?;
        let context_sha256 = seed.context.as_ref().map(hash).transpose()?;
        m2c = json!({"seed":format!("m2c/{}.c",stem(owner.address)),"seed_sha256":sha256::hex(candidate.as_bytes()),"context":seed.context.as_ref().map(|_|"m2c/family-template.i"),"context_sha256":context_sha256,"context_kind":seed.context_kind});
        (
            candidate,
            if seed.context.is_some() {
                "m2c_exact_sibling_context"
            } else {
                "m2c_no_context"
            },
            Some(owner.reference.as_str()),
        )
    };
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
        let target_symbol = owner.symbol.as_str();
        let retargeted_source = retarget(template, target_symbol, &source)?;
        let retargeted = format!("retargeted-{}.c", index + 1);
        write(&directory.join(&retargeted), retargeted_source.as_bytes())?;
        templates.push(json!({"rank":index+1,"owner":template.owner,"symbol":template.symbol,"score_basis_points":template.score_basis_points,"source":template.source,"packed_source":packed,"source_sha256":sha256::hex(source.as_bytes()),"retargeted_source":retargeted,"retargeted_sha256":sha256::hex(retargeted_source.as_bytes()),"retargeted":retargeted_source!=source}));
    }
    let template_count = templates.len();
    let value = json!({"schema_version":3,"owner":owner.owner,"name":owner.name,"scope":owner.scope,"status":status,"manifest_sha256":manifest_sha256,"family_index_sha256":family_index_sha256,"candidate_sha256":owner.candidate_sha256,"reference_sha256":hash(repository.join(&owner.reference))?,"target_symbol":owner.symbol,"family_index_reference":target.map(|target|target.source.as_str()),"family":target.and_then(|target|target.family.as_deref()),"family_threshold_met":target.is_some_and(|target|target.family.is_some()),"seed_origin":seed_origin,"seed_source":seed_source,"seed_sha256":sha256::hex(candidate.as_bytes()),"skeleton":"skeleton.c","m2c":m2c,"templates":templates,"template_count":template_count,"raw_diff_included":false});
    write_json(&directory.join("pack.json"), &value)?;
    Ok(value)
}

fn m2c_seed(
    repository: &Path,
    directory: &Path,
    owner: &Owner,
    target: Option<&FamilyTarget>,
) -> Result<crate::family_m2c::Seed, String> {
    let target_owner = SourceOwner::parse(&owner.owner)?;
    let assembly = repository.join(&owner.reference);
    let output = directory.join("m2c");
    let plain =
        || crate::family_m2c::generate(target_owner, &assembly, &owner.symbol, None, &output);
    let Some(template) = target.and_then(|family| family.alternatives.first()) else {
        return plain();
    };
    let template_owner = SourceOwner::parse(&template.owner)?;
    crate::family_m2c::generate(
        target_owner,
        &assembly,
        &owner.symbol,
        Some((
            template_owner,
            &repository.join(&template.source),
            &template.symbol,
        )),
        &output,
    )
    .or_else(|context_error| {
        plain().map_err(|plain_error| {
            format!("exact-sibling context: {context_error}; no context: {plain_error}")
        })
    })
}

fn work_order(
    directory: &Path,
    owner: &Owner,
    residual: &ResidualReport,
    playbook: &str,
    decoder: &Value,
    pack_path: &Path,
    pack: &Value,
    stall: usize,
) -> Result<(), String> {
    let template_count = pack["template_count"]
        .as_u64()
        .ok_or("template pack has no count")?;
    let skeleton = pack_path
        .parent()
        .ok_or("pack has no directory")?
        .join(pack["skeleton"].as_str().ok_or("pack has no skeleton")?);
    let body = json!({"owner":owner.owner,"name":owner.name,"candidate_source":owner.candidate,"candidate_sha256":owner.candidate_sha256,"residual":residual,"playbook":playbook,"decoder":decoder,"template_count":template_count,"stall_budget_compiles":stall,"pack":pack_path,"pack_sha256":sha256::hex(&serde_json::to_vec(pack).map_err(|error|error.to_string())?),"editable_source":skeleton,"allowed_files":[skeleton,"WORK.json"],"constraints":["execute_named_playbook_only","no_raw_diff_improvisation","adopt_only_when_byte_exact"]});
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
        let name = names.registered_name(id).map(str::to_string);
        owners.push(Owner {
            owner: id.id(),
            address,
            size: region.size,
            retention: region.retention.unwrap_or_default(),
            reference: region.source,
            symbol: if region.symbol.is_empty() {
                name.clone().unwrap_or_else(|| id.legacy_name())
            } else {
                region.symbol
            },
            name,
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
                .map(str::to_string);
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
    if units.unit_for_game_owner("gs1", owner).is_some() {
        return Err(format!("{} is a translation-unit template", template.owner));
    }
    match names.owners_for_path(&repository.join(relative)).as_slice() {
        [mapped] if *mapped == owner => Ok(()),
        [] => Err(format!("unregistered template source {}", template.source)),
        [_] => Err(format!("{} has the wrong source owner", template.owner)),
        _ => Err(format!("{} is a multi-owner source", template.source)),
    }
}

fn retarget(template: &Template, target: &str, source: &str) -> Result<String, String> {
    let source_name = template
        .canonical_name
        .as_deref()
        .unwrap_or(&template.symbol);
    if source_name == target || source.contains(&format!(" {target}(")) {
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
        bytes.extend_from_slice(
            hash(
                repository
                    .join(PACKS)
                    .join(stem(owner.address))
                    .join("pack.json"),
            )?
            .as_bytes(),
        );
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
    };
    let mut at = 0;
    while at < argv.len() {
        let value = |at: usize| {
            argv.get(at + 1)
                .ok_or_else(|| format!("{} requires a value", argv[at]))
        };
        match argv[at].as_str() {
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
    fn retarget_uses_registered_entry() {
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
    }
    #[test]
    fn dispatch_has_no_implicit_adoption_switch() {
        assert!(parse_args(&["--apply".into()]).is_err());
        assert!(!USAGE.contains("--apply"));
    }
}
