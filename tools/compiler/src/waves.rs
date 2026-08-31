use crate::families::{FamilyCatalog, RetargetMode, TemplateMatch};
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
    collections::BTreeMap,
    fs,
    path::{Path, PathBuf},
    process::Command,
};

const USAGE: &str = "usage: compiler waves <inventory|bucket|scoreboard|draft <prepare|score --shard I/N|collect>> [--output PATH] [--family-index FILE]";
const MANIFEST: &str = "out/gs1-en/full/asm/manifest.json";
const FAMILIES: &str = "out/gs1-en/reports/compiler-families.json";
const BUCKET: &str = "out/gs1-en/waves/bucket.json";
const RESIDUAL_SCOREBOARD: &str = "out/gs1-en/reports/residual-scoreboard.json";
const DRAFT: &str = "out/gs1-en/waves/draft";
const CATALOG: &str = "games/gs1/recon/compiler-repair-patterns.json";
const MAIN_REGIONS: &str = "games/gs1/semantic/main-regions.json";
const DEBT: [&str; 5] = [
    "c_candidate",
    "merge_with_continuations",
    "merge_with_function_owner",
    "merge_with_owner",
    "split_first",
];
// The drafting cohort is frozen so unintended drift fails loudly; intended
// drift — an adoption or a draft tracked into the corpus — updates these
// numbers as its acknowledgment. Last acknowledged: the current worktree's
// exact-C adoptions and tracked reconstruction drafts.
const DRAFT_OWNERS: usize = 204;
const INDEPENDENT_OWNERS: usize = 133;
const OWNER_GROUP_OWNERS: usize = 52;
const SPLIT_REGION_OWNERS: usize = 19;

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
    kind: String,
    #[serde(default)]
    symbol: String,
    retention: Option<String>,
}
#[derive(Deserialize)]
struct MainRegions {
    non_c_ranges: Vec<NonCRange>,
}
#[derive(Deserialize)]
struct NonCRange {
    address: String,
    size: usize,
    kind: String,
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
    kind: String,
    reference: String,
    #[serde(default)]
    reference_sha256: String,
    symbol: String,
    name: Option<String>,
    scope: Scope,
    unit: Option<String>,
    candidate: Option<String>,
    candidate_inputs_sha256: Option<String>,
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
    #[serde(default)]
    reference_sha256: String,
    candidate_inputs_sha256: Option<String>,
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

#[derive(Clone, Copy, Debug, Deserialize, Eq, PartialEq, Serialize)]
#[serde(rename_all = "snake_case")]
enum DraftRoute {
    IndependentM2c,
    OwnerGroup,
    SplitRegion,
}

#[derive(Clone, Copy, Debug, Deserialize, Serialize)]
struct Shard {
    index: usize,
    count: usize,
}

#[derive(Deserialize)]
struct DraftShardReport {
    schema_version: u32,
    cohort_sha256: String,
    prepare_receipt_sha256: String,
    scoring_environment_sha256: String,
    shard: Shard,
    scoreboard: Value,
    owners: Vec<Value>,
}

#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
struct DraftOwner {
    owner: String,
    address: u32,
    size: usize,
    retention: String,
    kind: String,
    reference: String,
    reference_sha256: String,
    symbol: String,
    name: Option<String>,
    route: DraftRoute,
}

#[derive(Debug, Deserialize, Eq, PartialEq, Serialize)]
struct DraftCohort {
    schema_version: u32,
    manifest_sha256: String,
    ordering: String,
    owners: Vec<DraftOwner>,
}

struct DraftArgs {
    output: PathBuf,
    family_index: PathBuf,
    shard: Option<Shard>,
}

#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
struct M2cIdentity {
    path: String,
    sha256: String,
    macros_sha256: String,
    source_tree_sha256: String,
    revision: Option<String>,
}

pub fn run(argv: &[String]) -> Result<(), String> {
    let Some(command) = argv.first().map(String::as_str) else {
        return Err(USAGE.into());
    };
    match command {
        "inventory" => inventory_command(simple_output(&argv[1..])?),
        "bucket" => bucket_command(simple_output(&argv[1..])?),
        "scoreboard" => scoreboard_command(simple_output(&argv[1..])?),
        "draft" => draft_command(&argv[1..]),
        "-h" | "--help" => {
            println!("{USAGE}");
            Ok(())
        }
        _ => Err(USAGE.into()),
    }
}

fn inventory_command(output: Option<PathBuf>) -> Result<(), String> {
    let inventory = inventory(root())?;
    let output = output.unwrap_or_else(|| root().join("out/gs1-en/waves/inventory.json"));
    write_json(&output, &inventory)?;
    let (unit, draft, none, named) = inventory_counts(&inventory.owners);
    println!("debt_owners={} debt_bytes={} standalone_drafts={draft} translation_units={unit} no_candidate={none} named_wave_ready={named} inventory={}", inventory.owners.len(), inventory.owners.iter().map(|o| o.size).sum::<usize>(), output.display());
    Ok(())
}

fn bucket_command(output: Option<PathBuf>) -> Result<(), String> {
    let repository = root();
    let inventory = inventory(repository)?;
    let output = output.unwrap_or_else(|| repository.join(BUCKET));
    let report = build_bucket_report(repository, &inventory, &output)?;
    print_bucket_summary(&report, &output);
    Ok(())
}

fn build_bucket_report(
    repository: &Path,
    inventory: &Inventory,
    output: &Path,
) -> Result<BucketReport, String> {
    let owner_dir = output.with_extension("").join("owners");
    let mut unit_scores = bucket_translation_units(repository, inventory, &owner_dir)?;
    let mut entries = Vec::with_capacity(inventory.owners.len());
    for owner in &inventory.owners {
        let mut entry = BucketEntry {
            owner: owner.owner.clone(),
            size: owner.size,
            scope: owner.scope,
            name: owner.name.clone(),
            reference_sha256: owner.reference_sha256.clone(),
            candidate_inputs_sha256: owner.candidate_inputs_sha256.clone(),
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
        schema_version: 3,
        manifest_sha256: inventory.manifest_sha256.clone(),
        scoring_environment_sha256: scoring_environment(repository)?,
        owners: entries,
    };
    write_json(&output, &report)?;
    Ok(report)
}

fn print_bucket_summary(report: &BucketReport, output: &Path) {
    let (scored, covered) = report
        .owners
        .iter()
        .fold((0, 0), |(scored, covered), owner| {
            (
                scored + usize::from(owner.verdict == "scored"),
                covered
                    + usize::from(owner.residual.as_ref().is_some_and(|residual| {
                        residual.class == ResidualClass::AllocationCovered
                    })),
            )
        });
    println!(
        "bucketed={} scored={scored} allocation_covered={covered} report={}",
        report.owners.len(),
        output.display()
    );
}

fn scoreboard_command(output: Option<PathBuf>) -> Result<(), String> {
    let repository = root();
    let inventory = inventory(repository)?;
    let environment = scoring_environment(repository)?;
    let bucket_path = repository.join(BUCKET);
    let bucket = cached_bucket_report(repository, &inventory, &bucket_path, &environment)?;
    let report = residual_scoreboard(
        &inventory,
        &bucket,
        json!({
            "manifest_sha256": inventory.manifest_sha256,
            "scoring_environment_sha256": environment,
            "bucket_sha256": hash(&bucket_path)?,
        }),
    )?;
    let output = output.unwrap_or_else(|| repository.join(RESIDUAL_SCOREBOARD));
    write_json(&output, &report)?;
    println!("residual_class\towners\tbytes");
    for (class, counts) in report["scoreboard"]["classes"]
        .as_object()
        .ok_or("scoreboard has no class table")?
    {
        println!(
            "{class}\t{}\t{}",
            counts["owners"].as_u64().unwrap_or(0),
            counts["bytes"].as_u64().unwrap_or(0)
        );
    }
    println!(
        "retriaged={} structural={} unclassified=0 report={} sha256={}",
        report["retriage"]["candidate_owners_retriaged"]
            .as_u64()
            .unwrap_or(0),
        report["retriage"]["classes"]["classes"]["structural_topology"]["owners"]
            .as_u64()
            .unwrap_or(0),
        output.display(),
        hash(&output)?
    );
    Ok(())
}

fn cached_bucket_report(
    repository: &Path,
    inventory: &Inventory,
    output: &Path,
    environment: &str,
) -> Result<BucketReport, String> {
    if let Ok(report) = json_file::<BucketReport>(output) {
        if bucket_report_is_fresh(&report, inventory, environment) {
            return Ok(report);
        }
    }
    build_bucket_report(repository, inventory, output)
}

fn bucket_report_is_fresh(report: &BucketReport, inventory: &Inventory, environment: &str) -> bool {
    report.schema_version >= 2
        && report.manifest_sha256 == inventory.manifest_sha256
        && report.scoring_environment_sha256 == environment
        && report.owners.len() == inventory.owners.len()
        && report
            .owners
            .iter()
            .zip(&inventory.owners)
            .all(|(entry, owner)| {
                entry.owner == owner.owner
                    && entry.size == owner.size
                    && entry.scope == owner.scope
                    && entry.reference_sha256 == owner.reference_sha256
                    && entry.candidate_inputs_sha256 == owner.candidate_inputs_sha256
            })
}

fn bucket_class(entry: &BucketEntry) -> Result<&str, String> {
    if entry.residual.is_none()
        && !matches!(
            entry.verdict.as_str(),
            "no_candidate" | "idiom_lint_parked" | "score_failed" | "decoder_failed"
        )
    {
        return Err(format!(
            "{} has unknown verdict {}",
            entry.owner, entry.verdict
        ));
    }
    let class = entry
        .residual
        .as_ref()
        .map(|residual| match &residual.class {
            ResidualClass::Exact => "exact",
            ResidualClass::LayoutOnly => "layout_only",
            ResidualClass::AllocationCovered => "allocation_covered",
            ResidualClass::AllocationUncovered => "allocation_uncovered",
            ResidualClass::SchedulingFloor => "scheduling_floor",
            ResidualClass::TypeWidthMismatch => "type_width_mismatch",
            ResidualClass::StructuralTopology => "structural_topology",
            ResidualClass::MissingExtraCode => "missing_extra_code",
            ResidualClass::FrameContext => "frame_context",
            ResidualClass::CompilerUnemittable => "compiler_unemittable",
            ResidualClass::Unclassified => "unclassified",
        })
        .unwrap_or(entry.verdict.as_str());
    if class.is_empty() || class == "unclassified" {
        Err(format!("{} has no triage class", entry.owner))
    } else {
        Ok(class)
    }
}

fn bucket_scoreboard<'a>(entries: impl Iterator<Item = &'a BucketEntry>) -> Result<Value, String> {
    let mut classes = BTreeMap::<String, (usize, usize)>::new();
    let mut owners = 0;
    let mut bytes = 0;
    for entry in entries {
        let row = classes.entry(bucket_class(entry)?.into()).or_default();
        row.0 += 1;
        row.1 += entry.size;
        owners += 1;
        bytes += entry.size;
    }
    Ok(json!({
        "owners": owners,
        "bytes": bytes,
        "classes": classes.into_iter().map(|(class, (owners, bytes))| {
            (class, json!({"owners": owners, "bytes": bytes}))
        }).collect::<BTreeMap<_, _>>()
    }))
}

fn residual_scoreboard(
    inventory: &Inventory,
    bucket: &BucketReport,
    inputs: Value,
) -> Result<Value, String> {
    if bucket.owners.len() != inventory.owners.len() {
        return Err("residual scoreboard inputs do not cover the current inventory".into());
    }
    for (owner, entry) in inventory.owners.iter().zip(&bucket.owners) {
        if entry.owner != owner.owner {
            return Err(format!("bucket ordering diverges at {}", owner.owner));
        }
        bucket_class(entry)?;
    }
    let candidates = bucket
        .owners
        .iter()
        .filter(|entry| entry.scope != Scope::NoCandidate)
        .collect::<Vec<_>>();
    let topology_coverage = bucket
        .owners
        .iter()
        .filter_map(|entry| entry.residual.as_ref())
        .fold(
            (0usize, 0usize, 0usize, 0usize),
            |(covered_owners, covered_bytes, uncovered_owners, uncovered_bytes), residual| {
                if matches!(
                    residual.facts.topology.status,
                    candidate_show::triage::TopologyStatus::Uncovered
                ) {
                    (
                        covered_owners,
                        covered_bytes,
                        uncovered_owners + 1,
                        uncovered_bytes + residual.facts.reference_bytes,
                    )
                } else {
                    (
                        covered_owners + 1,
                        covered_bytes + residual.facts.reference_bytes,
                        uncovered_owners,
                        uncovered_bytes,
                    )
                }
            },
        );
    let topology_unassessed = bucket
        .owners
        .iter()
        .filter(|entry| entry.scope != Scope::NoCandidate && entry.residual.is_none())
        .fold((0usize, 0usize), |(owners, bytes), entry| {
            (owners + 1, bytes + entry.size)
        });
    Ok(json!({
        "schema_version": 2,
        "kind": "main_rom_residual_scoreboard",
        "target": "gs1-en",
        "inventory": {
            "manifest_sha256": inventory.manifest_sha256,
            "owners": inventory.owners.len(),
            "bytes": inventory.owners.iter().map(|owner| owner.size).sum::<usize>(),
        },
        "inputs": inputs,
        "scoreboard": bucket_scoreboard(bucket.owners.iter())?,
        "retriage": {
            "method": "ordinary tracked candidate source scored with the current router; pre-score idiom failures remain parked; shared translation units compile once",
            "candidate_owners_retriaged": candidates.len(),
            "classes": bucket_scoreboard(candidates.into_iter())?,
        },
        "topology_coverage": {
            "covered": {"owners": topology_coverage.0, "bytes": topology_coverage.1},
            "uncovered": {"owners": topology_coverage.2, "bytes": topology_coverage.3},
            "unassessed_not_scored": {"owners": topology_unassessed.0, "bytes": topology_unassessed.1},
        },
        "unclassified": 0,
    }))
}

fn bucket_score(repository: &Path, owner: &Owner, output: &Path, entry: &mut BucketEntry) {
    bucket_lint(repository, owner, entry);
    if !entry.verdict.is_empty() {
        return;
    }
    let directory = output.join(stem(owner.address));
    match score_with_decoder(repository, owner, &directory) {
        Err(error) => {
            entry.verdict = "score_failed".into();
            entry.error = Some(error);
        }
        Ok((_, Some(error))) => {
            entry.verdict = "decoder_failed".into();
            entry.error = Some(error);
        }
        Ok((scored, None)) => record_score(entry, scored),
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
    (entry.allocator_repair, entry.allocator_dimensions) = allocator_parts(&scored);
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

fn draft_command(argv: &[String]) -> Result<(), String> {
    let stage = argv.first().map(String::as_str).ok_or(USAGE)?;
    let args = draft_args(&argv[1..])?;
    match stage {
        "prepare" => draft_prepare(args),
        "score" => draft_score(args),
        "collect" => draft_collect(args),
        _ => Err(USAGE.into()),
    }
}

fn draft_prepare(args: DraftArgs) -> Result<(), String> {
    if args.shard.is_some() {
        return Err("draft prepare does not accept --shard".into());
    }
    let repository = root();
    let family_sha256 = hash(resolve(repository, &args.family_index))?;
    let m2c = crate::family_m2c::locate_m2c()?;
    let m2c_identity = m2c_identity(&m2c)?;
    if args.output.join("prepare-receipt.json").is_file() {
        let (cohort, receipt) = prepared(repository, &args.output).map_err(|error| {
            format!(
                "draft output is nonempty but cannot be reused ({error}); choose a new --output"
            )
        })?;
        if receipt["family_index_sha256"] != family_sha256
            || receipt["m2c_identity"] != json!(m2c_identity)
        {
            return Err(
                "draft output was prepared with other inputs; choose a new --output".into(),
            );
        }
        println!(
            "draft_reused={} independent={INDEPENDENT_OWNERS} output={}",
            cohort.owners.len(),
            args.output.display()
        );
        return Ok(());
    }
    let inventory = inventory(repository)?;
    let cohort = draft_cohort(repository, &inventory)?;
    fs::create_dir_all(&args.output).map_err(|error| error.to_string())?;
    resume_cohort(&args.output, &cohort)?;
    let cohort_sha256 = hash(args.output.join("cohort.json"))?;
    let environment_sha256 = scoring_environment(repository)?;
    let state = json!({"schema_version":1,"cohort_sha256":cohort_sha256,"manifest_sha256":cohort.manifest_sha256,"family_index_sha256":family_sha256,"m2c_identity":m2c_identity,"scoring_environment_sha256":environment_sha256,"ordering":cohort.ordering});
    resume_state(&args.output, &state)?;
    let families = FamilyCatalog::load(repository, &args.family_index)?;
    let mut packs = Vec::new();
    for owner in &cohort.owners {
        let directory = args.output.join("packs").join(stem(owner.address));
        fs::create_dir_all(&directory).map_err(|error| error.to_string())?;
        let pack_path = directory.join("pack.json");
        let value = match current_pack(&pack_path, owner, &state) {
            Some(value) => value,
            None => {
                let value = pack(
                    repository,
                    &directory,
                    owner,
                    &families,
                    &cohort.manifest_sha256,
                    &family_sha256,
                    &m2c,
                    &m2c_identity,
                )
                .unwrap_or_else(|error| json!({"schema_version":4,"owner":owner.owner,"status":"pack_failed","route":owner.route,"category":owner.kind,"manifest_sha256":cohort.manifest_sha256,"family_index_sha256":family_sha256,"reference_sha256":owner.reference_sha256,"m2c_identity":m2c_identity,"contexts":Value::Null,"m2c":Value::Null,"templates":[],"template_count":0,"error":error}));
                write_json(&pack_path, &value)?;
                value
            }
        };
        packs.push(json!({"owner":owner.owner,"pack_sha256":hash(directory.join("pack.json"))?,"status":value["status"]}));
    }
    let receipt = json!({"schema_version":3,"state_sha256":hash(args.output.join("prepare-state.json"))?,"cohort_sha256":cohort_sha256,"manifest_sha256":cohort.manifest_sha256,"family_index_sha256":family_sha256,"m2c_identity":m2c_identity,"scoring_environment_sha256":environment_sha256,"ordering":cohort.ordering,"source_policy":"prepared_sources_only","denominators":{"all_records":DRAFT_OWNERS,"independent_m2c":INDEPENDENT_OWNERS},"predictions":prediction_contract(),"packs":packs});
    write_json(&args.output.join("prepare-receipt.json"), &receipt)?;
    println!(
        "draft_prepared={} independent={INDEPENDENT_OWNERS} output={}",
        cohort.owners.len(),
        args.output.display()
    );
    Ok(())
}

fn draft_score(args: DraftArgs) -> Result<(), String> {
    let shard = args.shard.ok_or("draft score requires --shard I/N")?;
    let repository = root();
    let (cohort, receipt) = prepared(repository, &args.output)?;
    let mut records = Vec::new();
    for (ordinal, owner) in cohort.owners.iter().enumerate() {
        if ordinal % shard.count != shard.index {
            continue;
        }
        let directory = args.output.join("results").join(stem(owner.address));
        let verdict = directory.join("verdict.json");
        if let Ok(record) = json_file::<Value>(&verdict) {
            if record_fresh(&record, owner, &receipt) {
                records.push(record);
                continue;
            }
        }
        let record = draft_owner(repository, &args.output, &directory, owner, &receipt);
        write_json(&verdict, &record)?;
        records.push(record);
    }
    let report = json!({"schema_version":2,"cohort_sha256":receipt["cohort_sha256"],"prepare_receipt_sha256":receipt["receipt_sha256"],"scoring_environment_sha256":receipt["scoring_environment_sha256"],"shard":{"index":shard.index,"count":shard.count,"rule":"global_size_address_ordinal_modulo_count"},"scoreboard":scoreboard(&records),"owners":records});
    write_json(
        &args
            .output
            .join("shards")
            .join(format!("{}-of-{}.json", shard.index, shard.count)),
        &report,
    )?;
    println!(
        "draft_scored={} shard={}/{} output={}",
        report["owners"].as_array().map_or(0, Vec::len),
        shard.index,
        shard.count,
        args.output.display()
    );
    Ok(())
}

fn draft_collect(args: DraftArgs) -> Result<(), String> {
    if args.shard.is_some() {
        return Err("draft collect does not accept --shard".into());
    }
    let repository = root();
    let (cohort, receipt) = prepared(repository, &args.output)?;
    let collected = collect_shard_reports(&args.output, &cohort.owners, &receipt);
    let records = collected.records;
    let mut errors = collected.errors;
    errors.extend(result_errors(&records));
    let independent = records
        .iter()
        .filter(|record| record["route"] == "independent_m2c")
        .collect::<Vec<_>>();
    let all = scoreboard(&records);
    let independent_scoreboard = scoreboard(&independent.into_iter().cloned().collect::<Vec<_>>());
    let comparison = prediction_comparison(&records);
    let report = json!({"schema_version":2,"complete":errors.is_empty(),"errors":errors,"receipt":receipt,"shard_count":collected.shard_count,"predictions":prediction_contract(),"denominators":{"all_records":DRAFT_OWNERS,"independent_m2c":INDEPENDENT_OWNERS},"scoreboards":{"all_cohort":all,"independent_m2c":independent_scoreboard},"comparison":comparison,"owners":records});
    write_json(&args.output.join("draft-report.json"), &report)?;
    if !report["complete"].as_bool().unwrap_or(false) {
        return Err(format!(
            "draft collection failed; see {}",
            args.output.join("draft-report.json").display()
        ));
    }
    println!(
        "draft_collected={DRAFT_OWNERS} output={}",
        args.output.display()
    );
    Ok(())
}

struct ShardCollection {
    records: Vec<Value>,
    errors: Vec<String>,
    shard_count: Option<usize>,
}

fn collect_shard_reports(output: &Path, cohort: &[DraftOwner], receipt: &Value) -> ShardCollection {
    let shard_directory = output.join("shards");
    let mut paths = fs::read_dir(&shard_directory)
        .map(|entries| {
            entries
                .filter_map(Result::ok)
                .map(|entry| entry.path())
                .filter(|path| path.extension().and_then(|value| value.to_str()) == Some("json"))
                .collect::<Vec<_>>()
        })
        .unwrap_or_default();
    paths.sort();
    let mut errors = Vec::new();
    if paths.is_empty() {
        errors.push(format!("no shard reports in {}", shard_directory.display()));
    }
    let ordinals = cohort
        .iter()
        .enumerate()
        .map(|(ordinal, owner)| (owner.owner.as_str(), ordinal))
        .collect::<BTreeMap<_, _>>();
    let mut assigned = vec![None; cohort.len()];
    let mut shard_count = None;
    let mut seen = BTreeMap::new();
    for path in paths {
        let report = match json_file::<DraftShardReport>(&path) {
            Ok(report) => report,
            Err(error) => {
                errors.push(error);
                continue;
            }
        };
        let checked = (|| -> Result<Vec<(usize, Value)>, String> {
            let Shard { index, count } = report.shard;
            if report.schema_version != 2
                || report.cohort_sha256 != receipt["cohort_sha256"]
                || report.prepare_receipt_sha256 != receipt["receipt_sha256"]
                || report.scoring_environment_sha256 != receipt["scoring_environment_sha256"]
            {
                return Err("does not identify this preparation".into());
            }
            let filename = format!("{index}-of-{count}.json");
            if count == 0
                || index >= count
                || path.file_name().and_then(|name| name.to_str()) != Some(&filename)
                || shard_count.is_some_and(|expected| expected != count)
                || seen.contains_key(&index)
            {
                return Err(format!("has incoherent shard identity {index}/{count}"));
            }
            if report.scoreboard != scoreboard(&report.owners) {
                return Err("has a stale scoreboard".into());
            }
            let mut checked = Vec::new();
            for record in report.owners {
                let owner_id = record["owner"].as_str().ok_or("owner has no identity")?;
                let ordinal = *ordinals
                    .get(owner_id)
                    .ok_or_else(|| format!("contains unknown owner {owner_id}"))?;
                if checked
                    .last()
                    .is_some_and(|(previous, _)| *previous >= ordinal)
                    || ordinal % count != index
                {
                    return Err(format!("{owner_id} is not in shard {index}/{count} order"));
                }
                let owner = &cohort[ordinal];
                if !record_fresh(&record, owner, receipt) {
                    return Err(format!("{owner_id} record is stale"));
                }
                let verdict = output
                    .join("results")
                    .join(stem(owner.address))
                    .join("verdict.json");
                if json_file::<Value>(&verdict)? != record {
                    return Err(format!("{owner_id} shard and saved verdict disagree"));
                }
                checked.push((ordinal, record));
            }
            Ok(checked)
        })();
        match checked {
            Ok(rows) => {
                let Shard { index, count } = report.shard;
                shard_count = Some(count);
                seen.insert(index, ());
                for (ordinal, record) in rows {
                    if assigned[ordinal].replace(record).is_some() {
                        errors.push(format!("{} is assigned twice", cohort[ordinal].owner));
                    }
                }
            }
            Err(error) => errors.push(format!("{} {error}", path.display())),
        }
    }
    if let Some(count) = shard_count {
        for index in 0..count {
            if !seen.contains_key(&index) {
                errors.push(format!("missing shard report {index}/{count}"));
            }
        }
    }
    let missing = assigned.iter().filter(|record| record.is_none()).count();
    if missing != 0 {
        errors.push(format!(
            "shard reports assign {} of {} cohort ordinals exactly once",
            cohort.len() - missing,
            cohort.len()
        ));
    }
    ShardCollection {
        records: assigned.into_iter().flatten().collect(),
        errors,
        shard_count,
    }
}

fn result_errors(records: &[Value]) -> Vec<String> {
    let independent = |record: &&Value| record["route"] == "independent_m2c";
    let independent_count = records.iter().filter(independent).count();
    let missing = records
        .iter()
        .filter(independent)
        .filter(|record| record["compiled"] == true && !record["residual"].is_object())
        .count();
    let unclassified = records
        .iter()
        .filter(|record| record["residual"]["class"] == "unclassified")
        .count();
    let nonterminal = records
        .iter()
        .filter(|record| !terminal_record(record))
        .count();
    let stats = (
        records.len(),
        independent_count,
        missing,
        unclassified,
        nonterminal,
    );
    if stats == (DRAFT_OWNERS, INDEPENDENT_OWNERS, 0, 0, 0) {
        Vec::new()
    } else {
        vec![format!("completion gate (total, independent, missing residual, unclassified, nonterminal): {stats:?}")]
    }
}

fn draft_args(argv: &[String]) -> Result<DraftArgs, String> {
    let mut args = DraftArgs {
        output: root().join(DRAFT),
        family_index: FAMILIES.into(),
        shard: None,
    };
    let mut pairs = argv.chunks_exact(2);
    for pair in &mut pairs {
        match pair[0].as_str() {
            "--output" => args.output = resolve(root(), Path::new(&pair[1])),
            "--family-index" => args.family_index = pair[1].clone().into(),
            "--shard" => args.shard = Some(parse_shard(&pair[1])?),
            _ => return Err(USAGE.into()),
        }
    }
    if !pairs.remainder().is_empty() {
        return Err(format!("{} requires a value", pairs.remainder()[0]));
    }
    validate_draft_output(root(), &args.output)?;
    Ok(args)
}

fn validate_draft_output(repository: &Path, output: &Path) -> Result<(), String> {
    let out = repository.join("out");
    let relative = output
        .strip_prefix(&out)
        .map_err(|_| "draft output must be a resolved path under out/".to_string())?;
    if relative.as_os_str().is_empty()
        || relative
            .components()
            .any(|part| !matches!(part, std::path::Component::Normal(_)))
    {
        return Err("draft output must be a resolved path under out/".into());
    }
    let mut current = out;
    for component in std::iter::once(None).chain(relative.components().map(Some)) {
        if let Some(component) = component {
            current.push(component.as_os_str());
        }
        match fs::symlink_metadata(&current) {
            Ok(metadata) if metadata.file_type().is_symlink() => {
                return Err(format!(
                    "draft output may not traverse symlink {}",
                    current.display()
                ));
            }
            Ok(metadata) if current != output && !metadata.is_dir() => {
                return Err(format!(
                    "draft output parent is not a directory: {}",
                    current.display()
                ));
            }
            Ok(_) => {}
            Err(error) if error.kind() == std::io::ErrorKind::NotFound => break,
            Err(error) => return Err(format!("{}: {error}", current.display())),
        }
    }
    if output.is_dir() {
        for entry in walkdir::WalkDir::new(output) {
            let entry = entry.map_err(|error| error.to_string())?;
            if entry.file_type().is_symlink() {
                return Err(format!(
                    "draft output contains symlink {}",
                    entry.path().display()
                ));
            }
        }
    }
    Ok(())
}

fn parse_shard(value: &str) -> Result<Shard, String> {
    let (index, count) = value.split_once('/').ok_or("--shard must be I/N")?;
    let (index, count) = (index.parse::<usize>().ok(), count.parse::<usize>().ok());
    match (index, count) {
        (Some(index), Some(count)) if count > 0 && index < count => Ok(Shard { index, count }),
        _ => Err("--shard uses zero-based I with 0 <= I < N".into()),
    }
}

fn draft_route(retention: &str) -> Result<DraftRoute, String> {
    match retention {
        "c_candidate" => Ok(DraftRoute::IndependentM2c),
        "merge_with_owner" | "merge_with_function_owner" | "merge_with_continuations" => {
            Ok(DraftRoute::OwnerGroup)
        }
        "split_first" => Ok(DraftRoute::SplitRegion),
        _ => Err(format!("unsupported draft retention {retention}")),
    }
}

fn draft_cohort(_repository: &Path, inventory: &Inventory) -> Result<DraftCohort, String> {
    let mut owners = inventory
        .owners
        .iter()
        .filter(|owner| owner.scope == Scope::NoCandidate)
        .map(|owner| {
            Ok(DraftOwner {
                owner: owner.owner.clone(),
                address: owner.address,
                size: owner.size,
                retention: owner.retention.clone(),
                kind: owner.kind.clone(),
                reference: owner.reference.clone(),
                reference_sha256: owner.reference_sha256.clone(),
                symbol: owner.symbol.clone(),
                name: owner.name.clone(),
                route: draft_route(&owner.retention)?,
            })
        })
        .collect::<Result<Vec<_>, String>>()?;
    owners.sort_by_key(|owner| (owner.size, owner.address));
    validate_cohort(&owners)?;
    Ok(DraftCohort {
        schema_version: 1,
        manifest_sha256: inventory.manifest_sha256.clone(),
        ordering: "reference_size_ascending_then_address".into(),
        owners,
    })
}

fn cohort_counts(owners: &[DraftOwner]) -> (usize, usize, usize) {
    let count = |route| owners.iter().filter(|owner| owner.route == route).count();
    (
        count(DraftRoute::IndependentM2c),
        count(DraftRoute::OwnerGroup),
        count(DraftRoute::SplitRegion),
    )
}

fn validate_cohort(owners: &[DraftOwner]) -> Result<(), String> {
    let (independent, groups, splits) = cohort_counts(owners);
    let expected = (
        DRAFT_OWNERS,
        INDEPENDENT_OWNERS,
        OWNER_GROUP_OWNERS,
        SPLIT_REGION_OWNERS,
    );
    if (owners.len(), independent, groups, splits) != expected {
        return Err(format!(
            "draft cohort changed: {} total, {independent} independent, {groups} owner-group, {splits} split-region",
            owners.len()
        ));
    }
    let disordered = owners
        .windows(2)
        .any(|pair| (pair[0].size, pair[0].address) >= (pair[1].size, pair[1].address));
    if disordered {
        return Err("draft cohort is duplicate or not in size/address order".into());
    }
    Ok(())
}

fn output_nonempty(output: &Path) -> Result<bool, String> {
    if !output.exists() {
        return Ok(false);
    }
    Ok(fs::read_dir(output)
        .map_err(|error| format!("{}: {error}", output.display()))?
        .next()
        .is_some())
}

fn resume_cohort(output: &Path, cohort: &DraftCohort) -> Result<(), String> {
    let path = output.join("cohort.json");
    if path.is_file() {
        if json_file::<DraftCohort>(&path)? == *cohort {
            return Ok(());
        }
        return Err("partial draft output has another cohort".into());
    }
    if [
        "prepare-state.json",
        "prepare-receipt.json",
        "packs",
        "results",
        "shards",
    ]
    .iter()
    .any(|name| output.join(name).exists())
    {
        return Err("partial draft output has products but no cohort".into());
    }
    write_json(&path, cohort)
}

fn resume_state(output: &Path, state: &Value) -> Result<(), String> {
    let path = output.join("prepare-state.json");
    if path.is_file() {
        return (json_file::<Value>(&path)? == *state)
            .then_some(())
            .ok_or_else(|| "partial draft output was prepared with other inputs".into());
    }
    let has_products = ["prepare-receipt.json", "packs", "results", "shards"]
        .iter()
        .map(|name| output_nonempty(&output.join(name)))
        .collect::<Result<Vec<_>, _>>()?
        .into_iter()
        .any(|present| present);
    if has_products {
        return Err("partial draft output has products but no preparation state".into());
    }
    write_json(&path, state)
}

fn current_pack(path: &Path, owner: &DraftOwner, state: &Value) -> Option<Value> {
    let pack = json_file::<Value>(path).ok()?;
    let status = pack["status"].as_str()?;
    let known_status = matches!(
        status,
        "ready" | "template_fallback" | "pack_failed" | "owner_group" | "split_region"
    );
    (known_status
        && pack["schema_version"] == 4
        && pack["owner"] == owner.owner
        && pack["route"] == json!(owner.route)
        && pack["reference_sha256"] == owner.reference_sha256
        && pack["manifest_sha256"] == state["manifest_sha256"]
        && pack["family_index_sha256"] == state["family_index_sha256"]
        && pack["m2c_identity"] == state["m2c_identity"]
        && validate_pack(path, &pack).is_ok())
    .then_some(pack)
}

fn m2c_identity(path: &Path) -> Result<M2cIdentity, String> {
    let path = path
        .canonicalize()
        .map_err(|error| format!("{}: {error}", path.display()))?;
    let macros = path
        .parent()
        .ok_or("m2c executable has no parent")?
        .join("m2c_macros.h");
    let output = Command::new("git")
        .args(["-C"])
        .arg(path.parent().ok_or("m2c executable has no parent")?)
        .args(["rev-parse", "HEAD"])
        .output();
    let revision = output
        .ok()
        .filter(|output| output.status.success())
        .and_then(|output| String::from_utf8(output.stdout).ok())
        .map(|value| value.trim().to_string());
    Ok(M2cIdentity {
        path: path.to_string_lossy().into_owned(),
        sha256: hash(&path)?,
        macros_sha256: hash(macros)?,
        source_tree_sha256: m2c_source_tree(&path)?,
        revision,
    })
}

fn m2c_source_tree(executable: &Path) -> Result<String, String> {
    let root = executable.parent().ok_or("m2c executable has no parent")?;
    let mut paths = vec![executable.to_path_buf(), root.join("m2c_macros.h")];
    for directory in ["m2c", "m2c_pycparser"] {
        paths.extend(
            walkdir::WalkDir::new(root.join(directory))
                .into_iter()
                .filter_map(Result::ok)
                .filter(|entry| entry.file_type().is_file())
                .map(|entry| entry.into_path())
                .filter(|path| path.extension().and_then(|value| value.to_str()) == Some("py")),
        );
    }
    hash_paths(root, paths)
}

fn prepared(repository: &Path, output: &Path) -> Result<(DraftCohort, Value), String> {
    let cohort_path = output.join("cohort.json");
    let cohort: DraftCohort = json_file(&cohort_path)?;
    let state_path = output.join("prepare-state.json");
    let state: Value = json_file(&state_path)?;
    let receipt_path = output.join("prepare-receipt.json");
    let mut receipt: Value = json_file(&receipt_path)?;
    validate_cohort(&cohort.owners)?;
    if cohort.schema_version != 1
        || state["schema_version"] != 1
        || receipt["schema_version"] != 3
        || receipt["state_sha256"] != hash(&state_path)?
        || receipt["cohort_sha256"] != hash(&cohort_path)?
        || receipt["cohort_sha256"] != state["cohort_sha256"]
        || receipt["manifest_sha256"] != cohort.manifest_sha256
        || receipt["manifest_sha256"] != state["manifest_sha256"]
        || receipt["manifest_sha256"] != hash(repository.join(MANIFEST))?
        || receipt["family_index_sha256"] != state["family_index_sha256"]
        || receipt["m2c_identity"] != state["m2c_identity"]
        || receipt["scoring_environment_sha256"] != state["scoring_environment_sha256"]
        || receipt["scoring_environment_sha256"] != scoring_environment(repository)?
    {
        return Err("draft preparation is stale".into());
    }
    let packs = receipt["packs"]
        .as_array()
        .ok_or("prepare receipt has no packs")?;
    if packs.len() != DRAFT_OWNERS {
        return Err("prepare receipt does not cover the cohort".into());
    }
    for (row, expected) in packs.iter().zip(&cohort.owners) {
        let owner = row["owner"].as_str().ok_or("pack receipt has no owner")?;
        if owner != expected.owner {
            return Err("prepare receipt pack order does not match the cohort".into());
        }
        validate_reference(repository, expected)?;
        let pack_path = output
            .join("packs")
            .join(stem(expected.address))
            .join("pack.json");
        if row["pack_sha256"] != hash(&pack_path)? {
            return Err(format!("{owner} pack is stale"));
        }
        current_pack(&pack_path, expected, &state)
            .ok_or_else(|| format!("{owner} pack is stale"))?;
    }
    receipt["receipt_sha256"] = json!(hash(receipt_path)?);
    Ok((cohort, receipt))
}

fn validate_reference(repository: &Path, owner: &DraftOwner) -> Result<(), String> {
    if hash(repository.join(&owner.reference))? != owner.reference_sha256 {
        return Err(format!("{} reference is stale", owner.owner));
    }
    Ok(())
}

fn validate_pack(path: &Path, pack: &Value) -> Result<(), String> {
    for (_, source, digest) in pack_sources(path, pack) {
        if hash(source)? != digest {
            return Err(format!("{} contains a stale source", pack["owner"]));
        }
    }
    let directory = path.parent().ok_or("pack has no directory")?;
    let inputs = pack["contexts"]["inputs"]
        .as_array()
        .map(Vec::as_slice)
        .unwrap_or_default();
    let named = ["aggregate_report", "compile_header"]
        .into_iter()
        .map(|name| &pack["contexts"][name]);
    for item in inputs.iter().chain(named).filter(|item| item.is_object()) {
        let relative = item["path"].as_str().ok_or("context has no path")?;
        if hash(directory.join(relative))? != item["sha256"].as_str().unwrap_or("") {
            return Err(format!("{} contains a stale context", pack["owner"]));
        }
    }
    Ok(())
}

fn record_fresh(record: &Value, owner: &DraftOwner, receipt: &Value) -> bool {
    record["owner"] == owner.owner
        && record["size"] == owner.size
        && record["route"] == json!(owner.route)
        && record["reference_sha256"] == owner.reference_sha256
        && record["cohort_sha256"] == receipt["cohort_sha256"]
        && record["prepare_receipt_sha256"] == receipt["receipt_sha256"]
        && record["scoring_environment_sha256"] == receipt["scoring_environment_sha256"]
}

fn draft_owner(
    repository: &Path,
    prepared: &Path,
    directory: &Path,
    owner: &DraftOwner,
    receipt: &Value,
) -> Value {
    let base = |verdict: &str, error: Option<String>| json!({"owner":owner.owner,"name":owner.name,"size":owner.size,"retention":owner.retention,"category":owner.kind,"route":owner.route,"reference_sha256":owner.reference_sha256,"cohort_sha256":receipt["cohort_sha256"],"prepare_receipt_sha256":receipt["receipt_sha256"],"scoring_environment_sha256":receipt["scoring_environment_sha256"],"source_policy":"prepared_sources_only","verdict":verdict,"compiled":false,"residual":Value::Null,"error":error});
    if owner.route != DraftRoute::IndependentM2c {
        return base(
            if owner.route == DraftRoute::OwnerGroup {
                "owner_group_parked"
            } else {
                "split_region_parked"
            },
            Some("audited non-independent region requires its complete owner group".into()),
        );
    }
    let reference = match read(repository.join(&owner.reference)) {
        Ok(reference) if sha256::hex(reference.as_bytes()) == owner.reference_sha256 => reference,
        Ok(_) => return base("reference_stale", Some("reference hash changed".into())),
        Err(error) => return base("reference_stale", Some(error)),
    };
    let pack_path = prepared
        .join("packs")
        .join(stem(owner.address))
        .join("pack.json");
    let pack = match json_file::<Value>(&pack_path) {
        Ok(value) => value,
        Err(error) => return base("pack_failed", Some(error)),
    };
    let pack_sha256 = match hash(&pack_path) {
        Ok(value) => value,
        Err(error) => return base("pack_failed", Some(error)),
    };
    if pack["status"] == "pack_failed" {
        let error = pack["error"]
            .as_str()
            .or_else(|| pack["m2c_error"].as_str())
            .unwrap_or("prepared pack failed without an error")
            .to_string();
        return base("pack_failed", Some(error));
    }
    if let Some(header) = pack["contexts"]["compile_header"]["path"].as_str() {
        let source = pack_path.parent().unwrap_or(Path::new(".")).join(header);
        let copied = fs::read(&source)
            .map_err(|error| format!("{}: {error}", source.display()))
            .and_then(|bytes| write(&directory.join("shared-aggregates.h"), &bytes));
        if let Err(error) = copied {
            return base("pack_failed", Some(error));
        }
    }
    let mut attempts = Vec::new();
    for (origin, path, expected) in pack_sources(&pack_path, &pack) {
        let source = match read(&path) {
            Ok(value) => value,
            Err(error) => {
                attempts.push(json!({"origin":origin,"compiled":false,"error":error}));
                continue;
            }
        };
        if sha256::hex(source.as_bytes()) != expected {
            attempts.push(
                json!({"origin":origin,"compiled":false,"error":"prepared source hash mismatch"}),
            );
            continue;
        }
        let candidate = directory.join("candidate.c");
        if let Err(error) = write(&candidate, source.as_bytes()) {
            return base("score_failed", Some(error));
        }
        let lint_report = lint(&source, &reference, None);
        match score_draft(repository, owner, &candidate, directory) {
            Err(error)=>attempts.push(json!({"origin":origin,"source_sha256":sha256::hex(source.as_bytes()),"compiled":false,"error":error})),
            Ok((scored,decoder_error))=> {
                let source_sha256 = sha256::hex(source.as_bytes());
                attempts.push(json!({"origin":origin,"source_sha256":source_sha256,"compiled":true}));
                let allocator = allocator_receipt(&scored);
                return json!({"owner":owner.owner,"name":owner.name,"size":owner.size,"retention":owner.retention,"category":owner.kind,"route":owner.route,"reference_sha256":owner.reference_sha256,"cohort_sha256":receipt["cohort_sha256"],"prepare_receipt_sha256":receipt["receipt_sha256"],"scoring_environment_sha256":receipt["scoring_environment_sha256"],"source_policy":"prepared_sources_only","pack_sha256":pack_sha256,"contexts":pack["contexts"],"verdict":if lint_report.passes(){"scored"}else{"scored_lint_flagged"},"compiled":true,"source_winner":{"origin":origin,"sha256":source_sha256},"candidate_sha256":hash(candidate).ok(),"attempts":attempts,"lint":lint_report,"residual":scored.residual,"allocator":allocator,"decoder_error":decoder_error,"error":Value::Null});
            }
        }
    }
    let mut record = base(
        "draft_compile_failed",
        Some("bound-context m2c and ranked templates did not compile".into()),
    );
    record["pack_sha256"] = json!(pack_sha256);
    record["contexts"] = pack["contexts"].clone();
    record["attempts"] = json!(attempts);
    record
}

fn allocator_receipt(scored: &RenderOutput) -> Value {
    if scored.allocator.is_none() {
        return Value::Null;
    }
    let (repair, dimensions) = allocator_parts(scored);
    json!({"repair":repair,"dimensions":dimensions})
}

fn allocator_parts(scored: &RenderOutput) -> (Option<String>, Vec<String>) {
    scored.allocator.as_ref().map_or_else(
        || (None, Vec::new()),
        |report| {
            (
                report.repair.as_ref().map(|repair| repair.label()),
                report.dimensions.iter().map(ToString::to_string).collect(),
            )
        },
    )
}

fn pack_sources(pack_path: &Path, pack: &Value) -> Vec<(String, PathBuf, String)> {
    let directory = pack_path.parent().unwrap_or(Path::new("."));
    let source = |origin: String, receipt: &Value, path: &str, digest: &str| {
        Some((
            origin,
            directory.join(receipt[path].as_str()?),
            receipt[digest].as_str()?.into(),
        ))
    };
    let seed = source(
        "bound_aggregate_m2c".into(),
        &pack["m2c"],
        "seed",
        "seed_sha256",
    );
    let templates = pack["templates"]
        .as_array()
        .into_iter()
        .flatten()
        .filter_map(|template| {
            source(
                format!("retargeted_template_{}", template["rank"].as_u64()?),
                template,
                "retargeted_source",
                "retargeted_sha256",
            )
        });
    seed.into_iter().chain(templates).collect()
}

fn score_draft(
    repository: &Path,
    owner: &DraftOwner,
    candidate: &Path,
    work: &Path,
) -> Result<(RenderOutput, Option<String>), String> {
    let scored_owner = Owner {
        owner: owner.owner.clone(),
        address: owner.address,
        size: owner.size,
        retention: owner.retention.clone(),
        kind: owner.kind.clone(),
        reference: owner.reference.clone(),
        reference_sha256: owner.reference_sha256.clone(),
        symbol: owner.symbol.clone(),
        name: owner.name.clone(),
        scope: Scope::StandaloneDraft,
        unit: None,
        candidate: Some(candidate.to_string_lossy().into_owned()),
        candidate_inputs_sha256: Some(candidate_inputs_hash(repository, candidate)?),
    };
    score_with_decoder(repository, &scored_owner, work)
}

fn score_with_decoder(
    repository: &Path,
    owner: &Owner,
    work: &Path,
) -> Result<(RenderOutput, Option<String>), String> {
    let source = owner.candidate.as_deref().unwrap_or("");
    let mut scored = score(repository, owner, &source, work, false)?;
    let mut decoder_error = None;
    if scored.residual.class == ResidualClass::AllocationUncovered {
        match score(repository, owner, &source, work, true) {
            Ok(decoded) => scored = decoded,
            Err(error) => decoder_error = Some(error),
        }
    }
    Ok((scored, decoder_error))
}

fn scoreboard(records: &[Value]) -> Value {
    let mut classes = BTreeMap::<String, (usize, usize)>::new();
    for record in records {
        let label = record["residual"]["class"]
            .as_str()
            .or_else(|| record["verdict"].as_str())
            .unwrap_or("missing");
        let row = classes.entry(label.into()).or_default();
        row.0 += 1;
        row.1 += record["size"].as_u64().unwrap_or(0) as usize;
    }
    json!({"owners":records.len(),"bytes":records.iter().filter_map(|record|record["size"].as_u64()).sum::<u64>(),"classes":classes.into_iter().map(|(name,(owners,bytes))|(name,json!({"owners":owners,"bytes":bytes}))).collect::<BTreeMap<_,_>>()})
}

fn terminal_record(record: &Value) -> bool {
    matches!(
        record["verdict"].as_str(),
        Some(
            "scored"
                | "scored_lint_flagged"
                | "draft_compile_failed"
                | "pack_failed"
                | "reference_stale"
                | "score_failed"
                | "owner_group_parked"
                | "split_region_parked"
        )
    )
}

fn prediction_comparison(records: &[Value]) -> Value {
    fn measured<'a>(records: impl Iterator<Item = &'a Value>, denominator: usize) -> Value {
        let floor = [
            "exact",
            "allocation_covered",
            "allocation_uncovered",
            "scheduling_floor",
        ];
        let (mut exact_or_floor, mut scored_nonexact, mut structural) = (0, 0, 0);
        let mut classes = BTreeMap::<String, (usize, u64)>::new();
        for record in records {
            let class = record["residual"]["class"].as_str().unwrap_or("");
            exact_or_floor += usize::from(floor.contains(&class));
            scored_nonexact += usize::from(record["compiled"] == true && class != "exact");
            structural += usize::from(class == "structural_topology");
            if record["compiled"] == true && !class.is_empty() && class != "exact" {
                let row = classes.entry(class.into()).or_default();
                row.0 += 1;
                row.1 += record["size"].as_u64().unwrap_or(0);
            }
        }
        let dominant = classes.into_iter().fold(None, |best, row| match &best {
            Some((_, score)) if *score >= row.1 => best,
            _ => Some(row),
        });
        let basis = |n, d| (d > 0).then(|| json!(n * 10000 / d)).unwrap_or(Value::Null);
        json!({"denominator":denominator,"exact_or_floor":{"owners":exact_or_floor,"basis_points":basis(exact_or_floor,denominator),"prediction_met":exact_or_floor*100<15*denominator},"structural_topology":{"owners":structural,"scored_nonexact_denominator":scored_nonexact,"basis_points":basis(structural,scored_nonexact),"prediction_met":scored_nonexact>0 && structural*100>=60*scored_nonexact},"dominant_scored_nonexact":dominant.map(|(class,(owners,bytes))|json!({"class":class,"owners":owners,"bytes":bytes})).unwrap_or(Value::Null)})
    }
    let all = measured(records.iter(), DRAFT_OWNERS);
    let independent = measured(
        records
            .iter()
            .filter(|record| record["route"] == "independent_m2c"),
        INDEPENDENT_OWNERS,
    );
    let winner = independent["dominant_scored_nonexact"]["class"].clone();
    let structural_prediction_met = independent["structural_topology"]["prediction_met"].clone();
    json!({"all_cohort":all,"independent_m2c":independent,"winner":winner,"winner_tie_rule":"owners_desc_bytes_desc_class_asc","structural_prediction_met":structural_prediction_met})
}

fn prediction_contract() -> Value {
    json!({"exact_or_floor":"<15% of denominator; classes exact, allocation_covered, allocation_uncovered, scheduling_floor","structural_topology":">=60% of scored non-exact","rates_use_basis_points":"numerator * 10000 / denominator"})
}

fn score(
    repository: &Path,
    owner: &Owner,
    source: &str,
    work: &Path,
    allocator: bool,
) -> Result<RenderOutput, String> {
    render(
        repository,
        &CandidateOptions {
            source: source.into(),
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
    owner: &DraftOwner,
    catalog: &FamilyCatalog,
    manifest_sha256: &str,
    family_index_sha256: &str,
    m2c: &Path,
    m2c_identity: &M2cIdentity,
) -> Result<Value, String> {
    if owner.route != DraftRoute::IndependentM2c {
        return Ok(
            json!({"schema_version":4,"owner":owner.owner,"status":if owner.route==DraftRoute::OwnerGroup{"owner_group"}else{"split_region"},"route":owner.route,"category":owner.kind,"manifest_sha256":manifest_sha256,"family_index_sha256":family_index_sha256,"reference_sha256":owner.reference_sha256,"m2c_identity":m2c_identity,"contexts":Value::Null,"m2c":Value::Null,"templates":[],"template_count":0}),
        );
    }
    let target_symbol = SourceOwner::parse(&owner.owner)?.legacy_name();
    let target = catalog.target(&owner.owner);
    let prepared = target
        .into_iter()
        .flat_map(|target| &target.alternatives)
        .filter_map(|details| {
            let template = catalog.template(details)?;
            let retargeted = crate::families::retarget_source(
                &details.owner,
                &template.source,
                &details.symbol,
                &target_symbol,
                RetargetMode::EntryMacro {
                    entry: template.entry_name.as_deref()?,
                },
            )
            .ok()?;
            Some((details, template, retargeted))
        })
        .take(3)
        .collect::<Vec<_>>();
    let mut templates = Vec::new();
    for (index, (details, template, retargeted_source)) in prepared.iter().enumerate() {
        let source = &template.source;
        let packed = format!("template-{}.c", index + 1);
        write(&directory.join(&packed), source.as_bytes())?;
        let retargeted = format!("retargeted-{}.c", index + 1);
        write(&directory.join(&retargeted), retargeted_source.as_bytes())?;
        templates.push(json!({"rank":index+1,"owner":details.owner,"symbol":details.symbol,"source_entry":template.entry_name.as_deref(),"score_basis_points":details.score_basis_points,"source":details.source,"packed_source":packed,"source_sha256":sha256::hex(source.as_bytes()),"retargeted_source":retargeted,"retargeted_sha256":sha256::hex(retargeted_source.as_bytes()),"retargeted":retargeted_source!=source}));
    }
    let template_count = templates.len();
    let template = prepared.first().map(|(details, _, _)| *details);
    let (m2c_seed, contexts, m2c_error) = match m2c_seed(
        repository, directory, owner, template, m2c,
    ) {
        Ok(seed) => (
            json!({"seed":seed.source.strip_prefix(directory).unwrap_or(&seed.source),"seed_sha256":seed.source_sha256}),
            seed_context_receipt(directory, &seed)?,
            Value::Null,
        ),
        Err(error) => (Value::Null, Value::Null, json!(error)),
    };
    let status = if m2c_seed.is_object() {
        "ready"
    } else if template_count > 0 {
        "template_fallback"
    } else {
        "pack_failed"
    };
    Ok(
        json!({"schema_version":4,"owner":owner.owner,"name":owner.name,"status":status,"route":owner.route,"category":owner.kind,"manifest_sha256":manifest_sha256,"family_index_sha256":family_index_sha256,"reference_sha256":owner.reference_sha256,"target_symbol":target_symbol,"family_index_reference":target.map(|target|target.source.as_str()),"family":target.and_then(|target|target.family.as_deref()),"m2c_identity":m2c_identity,"contexts":contexts,"m2c":m2c_seed,"m2c_error":m2c_error,"templates":templates,"template_count":template_count,"raw_diff_included":false}),
    )
}

fn seed_context_receipt(directory: &Path, seed: &crate::family_m2c::Seed) -> Result<Value, String> {
    let contexts=seed.contexts.iter().map(|context|json!({"kind":context.kind,"path":context.path.strip_prefix(directory).unwrap_or(&context.path),"sha256":context.sha256})).collect::<Vec<_>>();
    let report = directory.join("m2c/aggregate-report.json");
    write(
        &report,
        &fs::read(&seed.aggregate_report)
            .map_err(|error| format!("{}: {error}", seed.aggregate_report.display()))?,
    )?;
    Ok(
        json!({"kind":seed.context_kind,"inputs":contexts,"aggregate_report":{"path":report.strip_prefix(directory).unwrap_or(&report),"sha256":seed.aggregate_report_sha256,"proposals":seed.proposal_count,"structs":seed.struct_count,"rejected_misaligned_fields":seed.rejected_misaligned_fields},"compile_header":{"path":seed.compile_header.strip_prefix(directory).unwrap_or(&seed.compile_header),"sha256":seed.compile_header_sha256}}),
    )
}

fn m2c_seed(
    repository: &Path,
    directory: &Path,
    owner: &DraftOwner,
    template: Option<&TemplateMatch>,
    m2c: &Path,
) -> Result<crate::family_m2c::Seed, String> {
    let target_owner = SourceOwner::parse(&owner.owner)?;
    let assembly = repository.join(&owner.reference);
    let output = directory.join("m2c");
    let plain = || {
        crate::family_m2c::generate_with_m2c(
            target_owner,
            &assembly,
            &owner.symbol,
            None,
            &output,
            m2c,
        )
    };
    let Some(template) = template else {
        return plain();
    };
    let template_owner = SourceOwner::parse(&template.owner)?;
    crate::family_m2c::generate_with_m2c(
        target_owner,
        &assembly,
        &owner.symbol,
        Some((
            template_owner,
            &repository.join(&template.source),
            &template.symbol,
        )),
        &output,
        m2c,
    )
    .or_else(|context_error| {
        plain().map_err(|plain_error| {
            format!("exact-sibling context: {context_error}; no context: {plain_error}")
        })
    })
}

fn inventory(repository: &Path) -> Result<Inventory, String> {
    let manifest_text = read(repository.join(MANIFEST))?;
    let manifest: Manifest =
        serde_json::from_str(&manifest_text).map_err(|e| format!("{MANIFEST}: {e}"))?;
    let excluded = non_c_candidate_ranges(repository)?;
    reject_non_c_candidate_intersections(&manifest.regions, &excluded)?;
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
        let candidate_inputs_sha256 = candidate
            .as_deref()
            .map(|path| candidate_inputs_hash(repository, Path::new(path)))
            .transpose()?;
        let name = names.registered_name(id).map(str::to_string);
        let symbol = if region.symbol.is_empty() {
            assembly_symbol(&repository.join(&region.source)).unwrap_or_else(|| {
                if scope == Scope::NoCandidate {
                    String::new()
                } else {
                    name.clone().unwrap_or_else(|| id.legacy_name())
                }
            })
        } else {
            region.symbol
        };
        let reference_sha256 = hash(repository.join(&region.source))?;
        owners.push(Owner {
            owner: id.id(),
            address,
            size: region.size,
            retention: region.retention.unwrap_or_default(),
            kind: region.kind,
            reference: region.source,
            reference_sha256,
            symbol,
            name,
            scope,
            unit,
            candidate,
            candidate_inputs_sha256,
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

fn reject_non_c_candidate_intersections(
    regions: &[Region],
    excluded: &[(u64, u64)],
) -> Result<(), String> {
    for region in regions.iter().filter(|region| {
        region.source.starts_with("games/gs1/asm/executable_gaps/")
            && region
                .retention
                .as_deref()
                .is_some_and(|value| DEBT.contains(&value))
    }) {
        let end = region
            .address
            .checked_add(region.size as u64)
            .ok_or_else(|| format!("debt manifest range overflow at {:08x}", region.address))?;
        if let Some(&(excluded_start, excluded_end)) =
            excluded.iter().find(|&&(excluded_start, excluded_end)| {
                region.address < excluded_end && excluded_start < end
            })
        {
            return Err(format!(
                "debt manifest region {:08x}..{:08x} intersects registered non-C range {:08x}..{:08x}",
                region.address, end, excluded_start, excluded_end
            ));
        }
    }
    Ok(())
}

fn non_c_candidate_ranges(repository: &Path) -> Result<Vec<(u64, u64)>, String> {
    let document: MainRegions = json_file(&repository.join(MAIN_REGIONS))?;
    document
        .non_c_ranges
        .into_iter()
        .filter(|range| {
            matches!(
                range.kind.as_str(),
                "literal_pool" | "alignment_padding" | "lookup_table"
            )
        })
        .map(|range| {
            let address = range
                .address
                .strip_prefix("0x")
                .and_then(|value| u64::from_str_radix(value, 16).ok())
                .ok_or_else(|| format!("{MAIN_REGIONS}: bad address {}", range.address))?;
            let end = address
                .checked_add(range.size as u64)
                .ok_or_else(|| format!("{MAIN_REGIONS}: range overflow at {}", range.address))?;
            Ok((address, end))
        })
        .collect()
}

fn assembly_symbol(path: &Path) -> Option<String> {
    fs::read_to_string(path).ok()?.lines().find_map(|line| {
        line.trim()
            .strip_prefix(".global ")
            .map(str::trim)
            .filter(|name| !name.is_empty())
            .map(str::to_string)
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

fn simple_output(argv: &[String]) -> Result<Option<PathBuf>, String> {
    match argv {
        [] => Ok(None),
        [flag, value] if flag == "--output" => Ok(Some(value.into())),
        _ => Err(USAGE.into()),
    }
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
fn candidate_inputs_hash(repository: &Path, source: &Path) -> Result<String, String> {
    let source = resolve(repository, source);
    let include = repository.join("games/gs1/include");
    compiler_core::source_inputs::source_tree_signature(&source, &[include]).map(|digest| {
        digest
            .into_iter()
            .map(|byte| format!("{byte:02x}"))
            .collect()
    })
}
fn hash_tree(path: &Path) -> Result<String, String> {
    let paths = walkdir::WalkDir::new(path)
        .into_iter()
        .filter_map(Result::ok)
        .filter(|entry| entry.file_type().is_file())
        .map(|entry| entry.into_path())
        .collect::<Vec<_>>();
    hash_paths(path, paths)
}
fn hash_paths(root: &Path, mut paths: Vec<PathBuf>) -> Result<String, String> {
    paths.sort();
    let mut fingerprint = Vec::new();
    for path in paths {
        let name = path.strip_prefix(root).unwrap_or(&path).to_string_lossy();
        let bytes = fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))?;
        fingerprint.extend_from_slice(&(name.len() as u64).to_le_bytes());
        fingerprint.extend_from_slice(name.as_bytes());
        fingerprint.extend_from_slice(&(bytes.len() as u64).to_le_bytes());
        fingerprint.extend_from_slice(&bytes);
    }
    Ok(sha256::hex(&fingerprint))
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

    fn scratch(label: &str) -> PathBuf {
        std::env::temp_dir().join(format!("alchemy-waves-{label}-{}", std::process::id()))
    }

    fn fixture_owner(index: u32) -> DraftOwner {
        DraftOwner {
            owner: format!("main:{:08x}", 0x0800_0000 + index * 4),
            address: 0x0800_0000 + index * 4,
            size: 4,
            retention: "c_candidate".into(),
            kind: "code".into(),
            reference: format!("reference-{index}.s"),
            reference_sha256: format!("reference-{index}"),
            symbol: format!("Fixture_{index}"),
            name: None,
            route: DraftRoute::IndependentM2c,
        }
    }

    fn fixture_inventory_owner(index: u32, size: usize, scope: Scope) -> Owner {
        Owner {
            owner: format!("main:{:08x}", 0x0800_0000 + index * 4),
            address: 0x0800_0000 + index * 4,
            size,
            retention: "c_candidate".into(),
            kind: "code".into(),
            reference: format!("reference-{index}.s"),
            reference_sha256: format!("reference-{index}"),
            symbol: format!("Fixture_{index}"),
            name: None,
            scope,
            unit: None,
            candidate: (scope != Scope::NoCandidate).then(|| format!("candidate-{index}.c")),
            candidate_inputs_sha256: (scope != Scope::NoCandidate)
                .then(|| format!("candidate-{index}")),
        }
    }

    fn assert_retargeted_template_defines_target(template_owner: &str, target_owner: &str) {
        let repository = root();
        let catalog = FamilyCatalog::load(repository, Path::new(FAMILIES)).unwrap();
        let target = catalog.target(target_owner).unwrap();
        let details = target
            .alternatives
            .iter()
            .find(|details| details.owner == template_owner)
            .unwrap();
        let template = catalog.template(details).unwrap();
        let entry = template.entry_name.as_deref().unwrap();
        let source = crate::families::retarget_source(
            &details.owner,
            &template.source,
            &details.symbol,
            &target.symbol,
            RetargetMode::EntryMacro { entry },
        )
        .unwrap();
        let owner = SourceOwner::parse(target_owner).unwrap();
        let work = scratch(&format!(
            "retarget-{}",
            target_owner.trim_start_matches("main:")
        ));
        let candidate = work.join("candidate.c");
        write(&candidate, source.as_bytes()).unwrap();
        fs::create_dir_all(work.join("score")).unwrap();
        let rom = fs::read(repository.join("roms/gs1-en.gba")).unwrap();
        let verification = candidate_compiler::verify::verify_candidate_owned_routed(
            &candidate.to_string_lossy(),
            &owner.routing_path().to_string_lossy(),
            &owner.address_stem(),
            &rom,
            &work.join("score").to_string_lossy(),
            &[],
            candidate_compiler::verify::ROM_BASE,
            CompilerTarget::Gs1,
            &CandidateCompilerConfiguration {
                family: Some(CandidateCompilerFamily::Routed),
                ..Default::default()
            },
        )
        .unwrap();
        assert!(!verification.actual.is_empty());
        fs::remove_dir_all(work).unwrap();
    }

    fn fixture_bucket_entry(owner: &Owner, class: Option<ResidualClass>) -> BucketEntry {
        BucketEntry {
            owner: owner.owner.clone(),
            size: owner.size,
            scope: owner.scope,
            name: None,
            reference_sha256: owner.reference_sha256.clone(),
            candidate_inputs_sha256: owner.candidate_inputs_sha256.clone(),
            verdict: class.as_ref().map_or("no_candidate", |_| "scored").into(),
            residual: class.map(|class| ResidualReport {
                class,
                wrong_instructions: 1,
                facts: candidate_show::triage::ResidualFacts {
                    actual_bytes: owner.size,
                    reference_bytes: owner.size,
                    differing_halfwords: 1,
                    branch_topology_equal: false,
                    topology: candidate_show::triage::TopologyEvidence::default(),
                    type_width_fingerprints: Vec::new(),
                },
            }),
            allocator_repair: None,
            allocator_dimensions: Vec::new(),
            lint: None,
            error: None,
        }
    }

    fn fixture_receipt() -> Value {
        json!({"cohort_sha256":"cohort","receipt_sha256":"receipt","scoring_environment_sha256":"environment"})
    }

    fn fixture_record(owner: &DraftOwner, receipt: &Value) -> Value {
        json!({"owner":owner.owner,"size":owner.size,"route":owner.route,"reference_sha256":owner.reference_sha256,"cohort_sha256":receipt["cohort_sha256"],"prepare_receipt_sha256":receipt["receipt_sha256"],"scoring_environment_sha256":receipt["scoring_environment_sha256"],"verdict":"scored","compiled":true,"residual":{"class":"structural_topology"}})
    }

    fn write_fixture_shard(
        output: &Path,
        cohort: &[DraftOwner],
        receipt: &Value,
        index: usize,
        count: usize,
    ) {
        let rows = cohort
            .iter()
            .enumerate()
            .filter(|(ordinal, _)| ordinal % count == index)
            .map(|(_, owner)| {
                let record = fixture_record(owner, receipt);
                write_json(
                    &output
                        .join("results")
                        .join(stem(owner.address))
                        .join("verdict.json"),
                    &record,
                )
                .unwrap();
                record
            })
            .collect::<Vec<_>>();
        let report = json!({"schema_version":2,"cohort_sha256":receipt["cohort_sha256"],"prepare_receipt_sha256":receipt["receipt_sha256"],"scoring_environment_sha256":receipt["scoring_environment_sha256"],"shard":{"index":index,"count":count},"scoreboard":scoreboard(&rows),"owners":rows});
        write_json(
            &output
                .join("shards")
                .join(format!("{index}-of-{count}.json")),
            &report,
        )
        .unwrap();
    }

    #[test]
    fn drafting_cohort_is_the_frozen_smallest_first_experiment() {
        let repository = root();
        let inventory = inventory(repository).unwrap();
        let cohort = draft_cohort(repository, &inventory).unwrap();
        assert_eq!(cohort.owners.len(), DRAFT_OWNERS);
        assert_eq!(
            cohort_counts(&cohort.owners),
            (INDEPENDENT_OWNERS, OWNER_GROUP_OWNERS, SPLIT_REGION_OWNERS)
        );
        assert_eq!(cohort.ordering, "reference_size_ascending_then_address");
        assert!(cohort.owners.iter().all(|owner| owner.symbol
            == assembly_symbol(&repository.join(&owner.reference)).unwrap_or_default()));
    }

    #[test]
    fn inventory_rejects_registered_non_c_intersections() {
        let region = |address, size| Region {
            address,
            size,
            source: "games/gs1/asm/executable_gaps/fixture.s".into(),
            kind: "executable_gap_continuation".into(),
            symbol: "Fixture".into(),
            retention: Some("merge_with_owner".into()),
        };
        let excluded = [(0x080b_f1e8, 0x080b_f208)];

        assert!(
            reject_non_c_candidate_intersections(&[region(0x080b_f1e8, 32)], &excluded)
                .unwrap_err()
                .contains("intersects registered non-C range")
        );
        assert!(
            reject_non_c_candidate_intersections(&[region(0x080b_f1e0, 16)], &excluded).is_err()
        );
        assert!(reject_non_c_candidate_intersections(&[region(0x080b_f208, 4)], &excluded).is_ok());

        let ordinary_owner = Region {
            address: 0x0802_6b44,
            size: 0x33c,
            source: "games/gs1/asm/08026b44.s".into(),
            kind: "compiler_output".into(),
            symbol: "Func_08026b44".into(),
            retention: Some("c_candidate".into()),
        };
        assert!(reject_non_c_candidate_intersections(
            &[ordinary_owner],
            &[(0x0802_6e72, 0x0802_6e80)]
        )
        .is_ok());
    }

    #[test]
    fn shards_are_zero_based_and_deterministic() {
        let shard = parse_shard("2/3").unwrap();
        assert_eq!((shard.index, shard.count), (2, 3));
        let selected = (0..10).filter(|ordinal| ordinal % shard.count == shard.index);
        assert_eq!(selected.collect::<Vec<_>>(), vec![2, 5, 8]);
        assert!(parse_shard("3/3").is_err());
        assert!(parse_shard("0/0").is_err());
        assert!(parse_shard("1").is_err());
    }

    #[test]
    fn prepared_sources_try_bound_m2c_then_ranked_templates() {
        let pack = json!({"m2c":{"seed":"m2c/seed.c","seed_sha256":"seed"},"templates":[{"rank":1,"retargeted_source":"retargeted-1.c","retargeted_sha256":"one"},{"rank":2,"retargeted_source":"retargeted-2.c","retargeted_sha256":"two"}]});
        let sources = pack_sources(Path::new("run/packs/08000000/pack.json"), &pack)
            .into_iter()
            .map(|(origin, _, digest)| (origin, digest))
            .collect::<Vec<_>>();
        assert_eq!(
            sources,
            vec![
                ("bound_aggregate_m2c".into(), "seed".into()),
                ("retargeted_template_1".into(), "one".into()),
                ("retargeted_template_2".into(), "two".into())
            ]
        );
    }

    #[test]
    fn layout_only_is_not_an_exact_or_floor_prediction_hit() {
        let comparison = prediction_comparison(&[
            json!({"route":"independent_m2c","compiled":true,"residual":{"class":"layout_only"}}),
        ]);
        assert_eq!(comparison["independent_m2c"]["exact_or_floor"]["owners"], 0);
    }

    #[test]
    fn only_recorded_end_states_are_terminal() {
        assert!(terminal_record(&json!({"verdict": "scored"})));
        assert!(terminal_record(&json!({"verdict": "owner_group_parked"})));
        assert!(!terminal_record(&json!({"verdict": "unclassified"})));
        assert!(!terminal_record(&json!({})));
    }

    #[test]
    fn residual_scoreboard_covers_inventory_without_claiming_aggregate_effect() {
        let inventory = Inventory {
            schema_version: 1,
            manifest_sha256: "manifest".into(),
            owners: vec![
                fixture_inventory_owner(0, 10, Scope::StandaloneDraft),
                fixture_inventory_owner(1, 7, Scope::TranslationUnit),
                fixture_inventory_owner(2, 4, Scope::NoCandidate),
                fixture_inventory_owner(3, 3, Scope::StandaloneDraft),
            ],
        };
        let bucket = BucketReport {
            schema_version: 3,
            manifest_sha256: "manifest".into(),
            scoring_environment_sha256: "environment".into(),
            owners: vec![
                fixture_bucket_entry(
                    &inventory.owners[0],
                    Some(ResidualClass::StructuralTopology),
                ),
                fixture_bucket_entry(&inventory.owners[1], Some(ResidualClass::SchedulingFloor)),
                fixture_bucket_entry(&inventory.owners[2], None),
                {
                    let mut failed = fixture_bucket_entry(&inventory.owners[3], None);
                    failed.verdict = "score_failed".into();
                    failed
                },
            ],
        };
        let report = residual_scoreboard(&inventory, &bucket, json!({})).unwrap();
        assert_eq!(report["scoreboard"]["owners"], 4);
        assert_eq!(report["scoreboard"]["bytes"], 24);
        assert_eq!(report["scoreboard"]["classes"]["no_candidate"]["owners"], 1);
        assert_eq!(report["retriage"]["candidate_owners_retriaged"], 3);
        assert_eq!(report["topology_coverage"]["covered"]["owners"], 0);
        assert_eq!(report["topology_coverage"]["uncovered"]["owners"], 2);
        assert_eq!(
            report["topology_coverage"]["unassessed_not_scored"]["owners"],
            1
        );
        assert_eq!(
            report["topology_coverage"]["unassessed_not_scored"]["bytes"],
            3
        );
    }

    #[test]
    fn included_recon_source_invalidates_bucket_cache() {
        let repository = scratch("bucket-inputs");
        let source = Path::new("games/gs1/recon/en/main/owner.c");
        let included = repository.join("games/gs1/src/shared.h");
        write(
            &repository.join(source),
            b"#include \"../../../src/shared.h\"\n",
        )
        .unwrap();
        write(&included, b"before\n").unwrap();
        let mut owner = fixture_inventory_owner(0, 4, Scope::StandaloneDraft);
        owner.candidate = Some(source.to_string_lossy().into_owned());
        owner.candidate_inputs_sha256 = Some(candidate_inputs_hash(&repository, source).unwrap());
        let entry = fixture_bucket_entry(&owner, Some(ResidualClass::StructuralTopology));
        let report = BucketReport {
            schema_version: 3,
            manifest_sha256: "manifest".into(),
            scoring_environment_sha256: "environment".into(),
            owners: vec![entry],
        };
        let mut inventory = Inventory {
            schema_version: 1,
            manifest_sha256: "manifest".into(),
            owners: vec![owner],
        };
        assert!(bucket_report_is_fresh(&report, &inventory, "environment"));
        write(&included, b"after\n").unwrap();
        inventory.owners[0].candidate_inputs_sha256 =
            Some(candidate_inputs_hash(&repository, source).unwrap());
        assert!(!bucket_report_is_fresh(&report, &inventory, "environment"));
        fs::remove_dir_all(repository).unwrap();
    }

    #[test]
    fn authoritative_registry_rejects_translation_unit_templates() {
        let repository = root();
        let units = TranslationUnits::load(repository).unwrap();
        let catalog = FamilyCatalog::load(repository, Path::new(FAMILIES)).unwrap();
        let templates = catalog
            .targets()
            .iter()
            .flat_map(|target| &target.alternatives)
            .filter_map(|details| catalog.template(details))
            .collect::<Vec<_>>();
        assert!(!templates.is_empty());
        assert!(templates
            .iter()
            .all(|template| units.unit_for_game_owner("gs1", template.owner).is_none()));
    }
    #[test]
    fn prepared_templates_define_their_retargeted_object_symbols() {
        assert_retargeted_template_defines_target("main:08004080", "main:080b6e7c");
        assert_retargeted_template_defines_target("main:08003538", "main:08017e88");
        // 080b7424 joined the battle-summon unit, which rightly disables
        // macro-entry retargeting for its template; a standalone pair from
        // the same catalog stands in.
        assert_retargeted_template_defines_target("main:0809b0b0", "main:0800383c");
        assert_retargeted_template_defines_target("main:080ae7fc", "main:080a2324");
    }
    #[test]
    fn draft_has_no_adoption_or_freeform_switch() {
        assert!(simple_output(&["--apply".into()]).is_err());
        assert!(draft_args(&["--edit".into(), "yes".into()]).is_err());
        assert!(!USAGE.contains("--apply"));
    }

    #[test]
    fn reference_hash_is_a_preparation_invariant() {
        let repository = scratch("reference");
        fs::create_dir_all(&repository).unwrap();
        let mut owner = fixture_owner(0);
        write(&repository.join(&owner.reference), b"reference").unwrap();
        owner.reference_sha256 = hash(repository.join(&owner.reference)).unwrap();
        assert!(validate_reference(&repository, &owner).is_ok());
        write(&repository.join(&owner.reference), b"changed").unwrap();
        assert!(validate_reference(&repository, &owner).is_err());
        fs::remove_file(repository.join(&owner.reference)).unwrap();
        assert!(validate_reference(&repository, &owner).is_err());
        fs::remove_dir_all(repository).unwrap();
    }

    #[test]
    fn m2c_identity_fingerprints_the_resolved_script_and_macros() {
        let directory = scratch("m2c");
        fs::create_dir_all(&directory).unwrap();
        let executable = directory.join("selected-m2c.py");
        write(&executable, b"selected executable").unwrap();
        write(&directory.join("m2c_macros.h"), b"selected macros").unwrap();
        write(&directory.join("m2c/main.py"), b"imported module").unwrap();
        let first = m2c_identity(&executable).unwrap();
        assert_eq!(Path::new(&first.path), executable.canonicalize().unwrap());
        assert_eq!(first.sha256, hash(&executable).unwrap());
        write(&directory.join("m2c/main.py"), b"changed module").unwrap();
        let changed = m2c_identity(&executable).unwrap();
        assert_ne!(first, changed);
        fs::rename(
            directory.join("m2c/main.py"),
            directory.join("m2c/renamed.py"),
        )
        .unwrap();
        assert_ne!(changed, m2c_identity(&executable).unwrap());
        fs::remove_dir_all(directory).unwrap();
    }

    #[cfg(unix)]
    #[test]
    fn draft_output_rejects_symlink_escape() {
        use std::os::unix::fs::symlink;

        let repository = scratch("output-repository");
        let outside = scratch("output-outside");
        fs::create_dir_all(repository.join("out")).unwrap();
        fs::create_dir_all(&outside).unwrap();
        assert!(validate_draft_output(&repository, &repository.join("out/run")).is_ok());
        symlink(&outside, repository.join("out/escape")).unwrap();
        let error =
            validate_draft_output(&repository, &repository.join("out/escape/run")).unwrap_err();
        assert!(error.contains("may not traverse symlink"));
        fs::remove_file(repository.join("out/escape")).unwrap();
        fs::create_dir_all(repository.join("out/run/packs")).unwrap();
        symlink(&outside, repository.join("out/run/packs/escape")).unwrap();
        let error = validate_draft_output(&repository, &repository.join("out/run")).unwrap_err();
        assert!(error.contains("contains symlink"));
        fs::remove_dir_all(repository).unwrap();
        fs::remove_dir_all(outside).unwrap();
    }

    #[test]
    fn collector_requires_one_coherent_shard_assignment_per_ordinal() {
        let output = scratch("shards");
        let cohort = (0..4).map(fixture_owner).collect::<Vec<_>>();
        let receipt = fixture_receipt();
        write_fixture_shard(&output, &cohort, &receipt, 0, 2);
        write_fixture_shard(&output, &cohort, &receipt, 1, 2);
        let collected = collect_shard_reports(&output, &cohort, &receipt);
        assert!(collected.errors.is_empty(), "{:?}", collected.errors);
        assert_eq!(collected.shard_count, Some(2));
        assert_eq!(
            collected
                .records
                .iter()
                .map(|record| record["owner"].as_str().unwrap())
                .collect::<Vec<_>>(),
            cohort
                .iter()
                .map(|owner| owner.owner.as_str())
                .collect::<Vec<_>>()
        );

        let path = output.join("shards/0-of-2.json");
        let mut duplicate: Value = json_file(&path).unwrap();
        let row = duplicate["owners"][0].clone();
        duplicate["owners"].as_array_mut().unwrap().push(row);
        duplicate["scoreboard"] = scoreboard(duplicate["owners"].as_array().unwrap());
        write_json(&path, &duplicate).unwrap();
        let collected = collect_shard_reports(&output, &cohort, &receipt);
        assert!(collected
            .errors
            .iter()
            .any(|error| error.contains("not in shard")));
        fs::remove_dir_all(output).unwrap();
    }

    #[test]
    fn failed_pack_keeps_its_original_terminal_error() {
        let repository = scratch("pack-failed");
        let mut owner = fixture_owner(0);
        write(&repository.join(&owner.reference), b"reference").unwrap();
        owner.reference_sha256 = hash(repository.join(&owner.reference)).unwrap();
        let prepared = repository.join("prepared");
        let pack_path = prepared.join("packs/08000000/pack.json");
        write_json(
            &pack_path,
            &json!({"status":"pack_failed","error":"m2c rejected fixture"}),
        )
        .unwrap();
        let record = draft_owner(
            &repository,
            &prepared,
            &repository.join("result"),
            &owner,
            &fixture_receipt(),
        );
        assert_eq!(record["verdict"], "pack_failed");
        assert_eq!(record["error"], "m2c rejected fixture");
        fs::remove_dir_all(repository).unwrap();
    }

    #[test]
    fn partial_preparation_reuses_current_packs_and_rejects_changed_inputs() {
        let output = scratch("resume");
        fs::create_dir_all(&output).unwrap();
        let cohort = DraftCohort {
            schema_version: 1,
            manifest_sha256: "manifest".into(),
            ordering: "fixture".into(),
            owners: (0..2).map(fixture_owner).collect(),
        };
        write(&output.join("cohort.tmp-interrupted"), b"partial").unwrap();
        resume_cohort(&output, &cohort).unwrap();
        let state = json!({"schema_version":1,"manifest_sha256":"manifest","family_index_sha256":"family","m2c_identity":{"source_tree_sha256":"m2c"}});
        write(&output.join("prepare-state.tmp-interrupted"), b"partial").unwrap();
        resume_state(&output, &state).unwrap();
        let path = output.join("packs/08000000/pack.json");
        write_json(&path,&json!({"schema_version":4,"owner":cohort.owners[0].owner,"route":cohort.owners[0].route,"reference_sha256":cohort.owners[0].reference_sha256,"manifest_sha256":"manifest","family_index_sha256":"family","m2c_identity":state["m2c_identity"],"status":"pack_failed","error":"recorded","contexts":Value::Null,"m2c":Value::Null,"templates":[]})).unwrap();
        assert!(current_pack(&path, &cohort.owners[0], &state).is_some());
        let mut stale: Value = json_file(&path).unwrap();
        stale["reference_sha256"] = json!("stale");
        write_json(&path, &stale).unwrap();
        assert!(current_pack(&path, &cohort.owners[0], &state).is_none());
        assert!(current_pack(
            &output.join("packs/08000004/pack.json"),
            &cohort.owners[1],
            &state
        )
        .is_none());
        let mut changed = state.clone();
        changed["m2c_identity"]["source_tree_sha256"] = json!("changed");
        assert!(resume_state(&output, &changed).is_err());
        fs::remove_dir_all(output).unwrap();
    }

    #[test]
    fn winner_is_the_dominant_scored_nonexact_class_with_stable_ties() {
        let record = |class: &str, size| json!({"route":"independent_m2c","compiled":true,"size":size,"residual":{"class":class}});
        let comparison = prediction_comparison(&[
            record("missing_extra_code", 4),
            record("missing_extra_code", 4),
            record("structural_topology", 16),
        ]);
        assert_eq!(comparison["winner"], "missing_extra_code");
        assert_eq!(comparison["structural_prediction_met"], false);
        let tied = prediction_comparison(&[record("beta", 4), record("alpha", 4)]);
        assert_eq!(tied["winner"], "alpha");
    }
}
