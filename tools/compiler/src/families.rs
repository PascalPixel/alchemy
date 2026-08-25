use candidate_compiler::verify::{
    verify_candidate_owned_routed, CandidateCompilerConfiguration, CandidateCompilerFamily, ROM_BASE,
};
use candidate_show::{
    disasm::disassemble,
    insns::gas_function_insns,
    render::{align_streams, alignment_key, residual_class, without_pc_offset, without_register},
};
use compiler_core::{routing::CompilerTarget, sha256, source_paths::SourceOwner};
use serde::{Deserialize, Serialize};
use serde_json::Value;
use std::{
    collections::{BTreeMap, BTreeSet},
    fs,
    path::{Component, Path, PathBuf},
};

const USAGE: &str = "usage: compiler families <cluster (--write|--check) FILE | transplant main:ADDRESS [--index FILE] [--output DIR] | prove [FILE]>";
const DEFAULT_INDEX: &str = "games/gs1/recon/compiler-families.json";
const DEFAULT_PROOFS: &str = "games/gs1/recon/family-retention.json";
const INDEX_SCHEMA_VERSION: u32 = 2;
const MIN_SCORE: u16 = 7500;

#[derive(Debug, Deserialize)]
struct BuildManifest {
    regions: Vec<BuildRegion>,
}

#[derive(Debug, Deserialize)]
struct BuildRegion {
    source: String,
    #[serde(default)]
    symbol: String,
    address: u64,
    size: usize,
    retention: Option<String>,
    origin: Option<String>,
}

#[derive(Clone)]
struct Owner {
    id: String,
    symbol: String,
    source: String,
    assembly: String,
    size: usize,
    instructions: Vec<String>,
    features: BTreeMap<String, u32>,
    call_features: BTreeMap<String, u32>,
    calls: usize,
    branches: usize,
}

#[derive(Debug, Deserialize, Serialize)]
struct FamilyIndex {
    schema_version: u32,
    target: String,
    derivation: String,
    minimum_score_basis_points: u16,
    exact_templates: usize,
    unresolved_targets: usize,
    clustered_targets: usize,
    clustered_bytes: usize,
    families: Vec<Family>,
    targets: Vec<TargetMatch>,
}

#[derive(Debug, Deserialize, Serialize)]
struct Family {
    id: String,
    template_owner: String,
    template_source: String,
    members: Vec<String>,
    target_bytes: usize,
}

#[derive(Debug, Deserialize, Serialize)]
struct TargetMatch {
    owner: String,
    symbol: String,
    source: String,
    size: usize,
    instructions: usize,
    family: Option<String>,
    alternatives: Vec<TemplateMatch>,
}

#[derive(Debug, Deserialize, Serialize)]
struct TemplateMatch {
    owner: String,
    symbol: String,
    source: String,
    assembly: String,
    size: usize,
    instructions: usize,
    score_basis_points: u16,
    ngram_similarity_basis_points: u16,
    length_similarity_basis_points: u16,
    call_target_similarity_basis_points: u16,
    call_count_similarity_basis_points: u16,
    branch_similarity_basis_points: u16,
}

#[derive(Debug, Deserialize)]
struct ProofManifest {
    format: u32,
    families: Vec<ProofFamily>,
}

#[derive(Debug, Deserialize)]
struct ProofFamily {
    id: String,
    classification: String,
    exact_templates: Vec<String>,
    members: Vec<String>,
    minimum_attempted_candidates_per_member: usize,
    maximum_mismatch_run_rows: usize,
}

#[derive(Debug, Serialize)]
struct Block {
    kind: &'static str,
    template_start: usize,
    template_end: usize,
    target_start: usize,
    target_end: usize,
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    match arguments.first().map(String::as_str) {
        Some("cluster") => cluster_command(&arguments[1..]),
        Some("transplant") => transplant_command(&arguments[1..]),
        Some("prove") => prove_command(&arguments[1..]),
        Some("-h" | "--help") => {
            println!("{USAGE}");
            Ok(())
        }
        _ => Err(USAGE.into()),
    }
}

fn cluster_command(arguments: &[String]) -> Result<(), String> {
    let (mode, path) = match arguments {
        [mode, path] if matches!(mode.as_str(), "--write" | "--check") => (mode.as_str(), Path::new(path)),
        _ => return Err(USAGE.into()),
    };
    let index = build_index()?;
    let text = serde_json::to_string_pretty(&index).map_err(|error| format!("serialize family index: {error}"))? + "\n";
    if mode == "--write" {
        write(path, text.as_bytes())?;
        println!(
            "families={} targets={} clustered={} bytes={} index={}",
            index.families.len(),
            index.unresolved_targets,
            index.clustered_targets,
            index.clustered_bytes,
            path.display()
        );
        return Ok(());
    }
    let current = read(path)?;
    if current.as_bytes() != text.as_bytes() {
        return Err(format!("{} is stale; run `make families`", path.display()));
    }
    println!(
        "family-index=current families={} targets={} clustered={}",
        index.families.len(),
        index.unresolved_targets,
        index.clustered_targets
    );
    Ok(())
}

fn build_index() -> Result<FamilyIndex, String> {
    let exact_manifest: BuildManifest = json("out/gs1-en/claimed/manifest.json")?;
    let asm_manifest: BuildManifest = json("out/gs1-en/asm/manifest.json")?;
    let mut exact = Vec::new();
    for region in exact_manifest.regions {
        let stem = stem(region.address)?;
        let assembly = format!("out/gs1-en/claimed/obj/{stem}.s");
        exact.push(owner(format!("main:{stem}"), region.source, assembly, region.size, &region.symbol)?);
    }
    let mut targets = Vec::new();
    for region in asm_manifest.regions.into_iter().filter(|region| {
        region.retention.as_deref() == Some("c_candidate")
            || (region.retention.as_deref() == Some("keep_structured_asm")
                && region.origin.as_deref() == Some("compiler"))
    }) {
        let stem = stem(region.address)?;
        let symbol = if region.symbol.is_empty() { format!("Func_{stem}") } else { region.symbol.clone() };
        targets.push(owner(format!("main:{stem}"), region.source.clone(), region.source, region.size, &symbol)?);
    }
    exact.sort_by(|left, right| left.id.cmp(&right.id));
    targets.sort_by(|left, right| left.id.cmp(&right.id));
    let mut matches = Vec::new();
    for target in &targets {
        let mut alternatives = exact
            .iter()
            .filter(|template| compatible(target, template))
            .map(|template| similarity(target, template))
            .collect::<Vec<_>>();
        alternatives.sort_by(|left, right| {
            right.score_basis_points.cmp(&left.score_basis_points).then_with(|| left.owner.cmp(&right.owner))
        });
        alternatives.truncate(3);
        let family = alternatives
            .first()
            .filter(|value| value.score_basis_points >= MIN_SCORE)
            .map(|value| format!("template-{}", value.owner.replace(':', "-")));
        matches.push(TargetMatch {
            owner: target.id.clone(),
            symbol: target.symbol.clone(),
            source: target.source.clone(),
            size: target.size,
            instructions: target.instructions.len(),
            family,
            alternatives,
        });
    }
    let mut grouped: BTreeMap<String, Vec<&TargetMatch>> = BTreeMap::new();
    for target in &matches {
        if let Some(family) = &target.family {
            grouped.entry(family.clone()).or_default().push(target);
        }
    }
    let families = grouped
        .into_iter()
        .map(|(id, targets)| {
            let owner = targets[0].alternatives[0].owner.clone();
            Family {
                id,
                template_source: targets[0].alternatives[0].source.clone(),
                template_owner: owner,
                members: targets.iter().map(|target| target.owner.clone()).collect(),
                target_bytes: targets.iter().map(|target| target.size).sum(),
            }
        })
        .collect::<Vec<_>>();
    let clustered_targets = matches.iter().filter(|target| target.family.is_some()).count();
    let clustered_bytes = matches.iter().filter(|target| target.family.is_some()).map(|target| target.size).sum();
    Ok(FamilyIndex {
        schema_version: INDEX_SCHEMA_VERSION,
        target: "gs1-en".into(),
        derivation: "canonical-instruction-ngram-and-call-target-cosine-v2".into(),
        minimum_score_basis_points: MIN_SCORE,
        exact_templates: exact.len(),
        unresolved_targets: targets.len(),
        clustered_targets,
        clustered_bytes,
        families,
        targets: matches,
    })
}

fn owner(id: String, source: String, assembly: String, size: usize, symbol: &str) -> Result<Owner, String> {
    let text = read(Path::new(&assembly))?;
    let raw = gas_function_insns(&text, symbol);
    let mut call_features = BTreeMap::new();
    for target in raw.iter().filter_map(|line| call_target(line)) {
        *call_features.entry(target).or_default() += 1;
    }
    let instructions = raw.iter().map(|line| token(line)).collect::<Vec<_>>();
    if instructions.is_empty() {
        return Err(format!("{id}: no instructions in {assembly}"));
    }
    let mut features = BTreeMap::new();
    for item in &instructions {
        *features.entry(format!("1:{item}")).or_default() += 1;
    }
    for pair in instructions.windows(2) {
        *features.entry(format!("2:{}|{}", pair[0], pair[1])).or_default() += 1;
    }
    let calls = instructions.iter().filter(|line| line.starts_with("bl ") || line.starts_with("blx ")).count();
    let branches = instructions.iter().filter(|line| line.starts_with('b') && !line.starts_with("bic")).count();
    Ok(Owner {
        id,
        symbol: symbol.to_string(),
        source,
        assembly,
        size,
        instructions,
        features,
        call_features,
        calls,
        branches,
    })
}

fn call_target(line: &str) -> Option<String> {
    let mut words = line.split('@').next()?.split_whitespace();
    let mnemonic = words.next()?.trim_end_matches(".n");
    if !matches!(mnemonic, "bl" | "blx") {
        return None;
    }
    Some(words.next()?.trim_end_matches(',').to_string())
}

fn token(line: &str) -> String {
    let text = alignment_key(&without_register(&without_pc_offset(line)));
    let Some((mnemonic, operands)) = text.split_once(' ') else {
        return text;
    };
    if matches!(mnemonic, "bl" | "blx") {
        return format!("{mnemonic} CALL");
    }
    if mnemonic.starts_with('b') {
        return format!("{mnemonic} TARGET");
    }
    let operands =
        operands
            .split_whitespace()
            .map(|word| {
                if word.contains("Func_") || word.contains("Data_") || word.contains("Region_") {
                    "SYMBOL"
                } else {
                    word
                }
            })
            .collect::<Vec<_>>()
            .join(" ");
    format!("{mnemonic} {operands}")
}

fn compatible(left: &Owner, right: &Owner) -> bool {
    let length = ratio(left.instructions.len(), right.instructions.len());
    length >= 4500 && left.calls.abs_diff(right.calls) <= 4.max(left.calls.max(right.calls) / 3)
}

fn similarity(target: &Owner, template: &Owner) -> TemplateMatch {
    let ngram = cosine(&target.features, &template.features);
    let length = ratio(target.instructions.len(), template.instructions.len());
    let call_targets = if target.calls == 0 && template.calls == 0 {
        10_000
    } else {
        cosine(&target.call_features, &template.call_features)
    };
    let call_count = ratio(target.calls + 1, template.calls + 1);
    let branches = ratio(target.branches + 1, template.branches + 1);
    let score = ((u32::from(ngram) * 65
        + u32::from(length) * 10
        + u32::from(call_targets) * 15
        + u32::from(call_count) * 5
        + u32::from(branches) * 5)
        / 100) as u16;
    TemplateMatch {
        owner: template.id.clone(),
        symbol: template.symbol.clone(),
        source: template.source.clone(),
        assembly: template.assembly.clone(),
        size: template.size,
        instructions: template.instructions.len(),
        score_basis_points: score,
        ngram_similarity_basis_points: ngram,
        length_similarity_basis_points: length,
        call_target_similarity_basis_points: call_targets,
        call_count_similarity_basis_points: call_count,
        branch_similarity_basis_points: branches,
    }
}

fn cosine(left: &BTreeMap<String, u32>, right: &BTreeMap<String, u32>) -> u16 {
    let dot = left
        .iter()
        .filter_map(|(key, value)| right.get(key).map(|other| u64::from(*value) * u64::from(*other)))
        .sum::<u64>() as f64;
    let norm =
        |value: &BTreeMap<String, u32>| value.values().map(|count| f64::from(*count).powi(2)).sum::<f64>().sqrt();
    if dot == 0.0 {
        0
    } else {
        (dot / (norm(left) * norm(right)) * 10_000.0).round() as u16
    }
}

fn ratio(left: usize, right: usize) -> u16 {
    (left.min(right) * 10_000 / left.max(right).max(1)) as u16
}

fn transplant_command(arguments: &[String]) -> Result<(), String> {
    let owner = arguments.first().ok_or(USAGE)?;
    let mut index = PathBuf::from(DEFAULT_INDEX);
    let mut output = PathBuf::from("out/family-transplants").join(owner.replace(':', "-"));
    let mut at = 1;
    while at < arguments.len() {
        let target = arguments.get(at + 1).ok_or(USAGE)?;
        match arguments[at].as_str() {
            "--index" => index = target.into(),
            "--output" => output = target.into(),
            _ => return Err(USAGE.into()),
        }
        at += 2;
    }
    let output = output_path(&output)?;
    let index: FamilyIndex = json(&index)?;
    if index.schema_version != INDEX_SCHEMA_VERSION {
        return Err(format!("family index schema {} is not supported; run `make families`", index.schema_version));
    }
    let target = index
        .targets
        .iter()
        .find(|target| target.owner == *owner)
        .ok_or_else(|| format!("{owner}: not in family index"))?;
    require_family(owner, target.family.as_deref())?;
    let template = target.alternatives.first().ok_or_else(|| format!("{owner}: no compatible exact template"))?;
    let template_source = read(Path::new(&template.source))?;
    let target_owner = SourceOwner::parse(owner)?;
    let template_owner = SourceOwner::parse(&template.owner)?;
    if !target_owner.is_main() || !template_owner.is_main() {
        return Err("family transplant currently supports main-image owners".into());
    }
    let stem = target_owner.address_stem();
    let candidate_path = PathBuf::from(format!("games/gs1/recon/en/main/{stem}.c"));
    let (seed, origin, seed_source) = if candidate_path.is_file() {
        (read(&candidate_path)?, "semantic_candidate", candidate_path.clone())
    } else {
        (
            template_source.replace(&template.symbol, &target.symbol),
            "exact_template_symbol_seed",
            PathBuf::from(&template.source),
        )
    };
    let template_path = output.join("template.c");
    let seed_path = output.join(format!("{stem}.c"));
    write(&template_path, template_source.as_bytes())?;
    write(&seed_path, seed.as_bytes())?;
    let target_asm = read(Path::new(&target.source))?;
    let template_asm = read(Path::new(&template.assembly))?;
    let target_lines =
        gas_function_insns(&target_asm, &target.symbol).into_iter().map(|line| token(&line)).collect::<Vec<_>>();
    let template_lines =
        gas_function_insns(&template_asm, &template.symbol).into_iter().map(|line| token(&line)).collect::<Vec<_>>();
    if target_lines.is_empty() || template_lines.is_empty() {
        return Err(format!(
            "cannot align {} ({}) with {} ({})",
            target.owner, target.symbol, template.owner, template.symbol
        ));
    }
    let blocks = blocks(&template_lines, &target_lines);
    write_json(&output.join("alignment.json"), &blocks)?;
    let m2c = crate::workbench::generate_family_m2c_seed(
        target_owner,
        Path::new(&target.source),
        &target.symbol,
        template_owner,
        Path::new(&template.source),
        &template.symbol,
        &output.join("m2c"),
    )?;
    let recipe = serde_json::json!({
        "schema_version": 1,
        "family": target.family,
        "target": target.owner,
        "target_assembly": target.source,
        "template": template,
        "seed_origin": origin,
        "seed_source": seed_source,
        "seed": seed_path,
        "seed_sha256": sha256::hex(seed.as_bytes()),
        "template_sha256": sha256::hex(template_source.as_bytes()),
        "m2c": {
            "seed": m2c.source,
            "context": m2c.context,
            "context_kind": m2c.context_kind,
            "template_symbol": template.symbol,
            "target_symbol": target.symbol,
        },
        "safe_command": format!("cargo run --offline --quiet --release --manifest-path tools/compiler/Cargo.toml -- permute {} --walk --heat --family-template {} --iterations 20000 --output out/family-search/{stem}-safe", seed_path.display(), template_path.display()),
        "classic_command": format!("cargo run --offline --quiet --release --manifest-path tools/compiler/Cargo.toml -- permute {} --walk --classic --heat --family-template {} --iterations 10000 --output out/family-search/{stem}-classic", seed_path.display(), template_path.display()),
    });
    write_json(&output.join("recipe.json"), &recipe)?;
    println!(
        "transplant={} template={} score={} output={}",
        owner,
        template.owner,
        template.score_basis_points,
        output.display()
    );
    Ok(())
}

fn require_family(owner: &str, family: Option<&str>) -> Result<(), String> {
    family
        .map(|_| ())
        .ok_or_else(|| format!("{owner}: no exact template reaches the {MIN_SCORE}-point family threshold"))
}

fn blocks(template: &[String], target: &[String]) -> Vec<Block> {
    let pairs = align_streams(template, target);
    let mut output = Vec::new();
    let (mut row, mut template_at, mut target_at) = (0, 0, 0);
    while row < pairs.len() {
        let matched = matches!(&pairs[row], (Some(left), Some(right)) if left == right);
        let (template_start, target_start, start) = (template_at, target_at, row);
        while row < pairs.len() && matches!(&pairs[row], (Some(left), Some(right)) if left == right) == matched {
            template_at += usize::from(pairs[row].0.is_some());
            target_at += usize::from(pairs[row].1.is_some());
            row += 1;
        }
        output.push(Block {
            kind: if matched { "shared" } else { "lowering_delta" },
            template_start,
            template_end: template_at,
            target_start,
            target_end: target_at,
        });
        debug_assert!(row > start);
    }
    output
}

fn prove_command(arguments: &[String]) -> Result<(), String> {
    let path = match arguments {
        [] => Path::new(DEFAULT_PROOFS),
        [path] => Path::new(path),
        _ => return Err(USAGE.into()),
    };
    let manifest: ProofManifest = json(path)?;
    if manifest.format != 1 || manifest.families.is_empty() {
        return Err(format!("{}: invalid or empty family proof manifest", path.display()));
    }
    let classification: Value = json("games/gs1/asm/classification.json")?;
    let exact: BuildManifest = json("out/gs1-en/claimed/manifest.json")?;
    let exact = exact
        .regions
        .into_iter()
        .map(|region| stem(region.address).map(|stem| format!("main:{stem}")))
        .collect::<Result<BTreeSet<_>, _>>()?;
    let mut bytes = 0;
    let mut seen = BTreeSet::new();
    let mut classifications = BTreeSet::new();
    let mut owners = BTreeSet::new();
    for family in &manifest.families {
        if !seen.insert(&family.id)
            || !classifications.insert(&family.classification)
            || family.members.len() < 2
            || family.exact_templates.is_empty()
            || family.minimum_attempted_candidates_per_member == 0
            || family.maximum_mismatch_run_rows == 0
        {
            return Err(format!("{}: invalid or duplicate family contract", family.id));
        }
        for template in &family.exact_templates {
            if !exact.contains(template) {
                return Err(format!("{}: template {template} is not exact C", family.id));
            }
        }
        let group = classification["groups"]
            .as_array()
            .and_then(|groups| groups.iter().find(|group| group["kind"].as_str() == Some(&family.classification)))
            .ok_or_else(|| format!("{}: classification group missing", family.id))?;
        let classified = group["files"]
            .as_array()
            .ok_or_else(|| format!("{}: classification has no files", family.id))?
            .iter()
            .filter_map(Value::as_str)
            .map(|stem| format!("main:{stem}"))
            .collect::<BTreeSet<_>>();
        let members = family.members.iter().cloned().collect::<BTreeSet<_>>();
        if members.len() != family.members.len() || members.iter().any(|owner| !owners.insert(owner.clone())) {
            return Err(format!("{}: duplicate proof member", family.id));
        }
        if classified != members {
            return Err(format!("{}: proof does not cover the whole classification", family.id));
        }
        let mut family_bytes = 0;
        for owner in &family.members {
            family_bytes += prove_member(owner, family)?;
        }
        if group["expected_files"].as_u64() != Some(family.members.len() as u64)
            || group["expected_bytes"].as_u64() != Some(family_bytes as u64)
        {
            return Err(format!("{}: classification counts disagree with proof", family.id));
        }
        bytes += family_bytes;
    }
    println!(
        "family-retention=ok families={} members={} bytes={bytes}",
        manifest.families.len(),
        manifest.families.iter().map(|family| family.members.len()).sum::<usize>()
    );
    Ok(())
}

fn prove_member(owner: &str, family: &ProofFamily) -> Result<usize, String> {
    let stem = owner
        .strip_prefix("main:")
        .filter(|stem| stem.len() == 8 && stem.bytes().all(|byte| byte.is_ascii_hexdigit()))
        .ok_or_else(|| format!("{}: invalid owner {owner}", family.id))?;
    let evidence_path = PathBuf::from(format!("games/gs1/recon/en/main/{stem}.json"));
    let evidence: Value = json(&evidence_path)?;
    let owner_bytes = evidence["owner_bytes"].as_u64().ok_or_else(|| format!("{owner}: missing owner_bytes"))? as usize;
    if evidence["candidate_bytes"].as_u64() != Some(owner_bytes as u64)
        || evidence["wrong_instructions"].as_u64() != Some(0)
        || evidence["classification"].as_str() != Some(&family.classification)
        || evidence.pointer("/resolved_asm_classification/kind").and_then(Value::as_str) != Some(&family.classification)
        || evidence.pointer("/cross_edition/core_identical").and_then(Value::as_bool) != Some(true)
    {
        return Err(format!("{owner}: candidate extent, wrong count, or six-ROM core failed"));
    }
    let editions = evidence
        .pointer("/cross_edition/editions")
        .and_then(Value::as_array)
        .ok_or_else(|| format!("{owner}: missing edition set"))?
        .iter()
        .filter_map(Value::as_str)
        .collect::<BTreeSet<_>>();
    if editions != BTreeSet::from(["ja", "en", "de", "es", "fr", "it"]) {
        return Err(format!("{owner}: incomplete edition proof"));
    }
    if attempted(&evidence["bounded_search"]) < family.minimum_attempted_candidates_per_member {
        return Err(format!("{owner}: bounded search is below the family minimum"));
    }
    let residual = evidence["residual"]
        .as_array()
        .filter(|rows| !rows.is_empty())
        .ok_or_else(|| format!("{owner}: no structured residual evidence"))?;
    if residual.iter().any(|row| {
        ["candidate", "reference", "equivalence", "resynchronization"]
            .iter()
            .any(|field| row[*field].as_str().is_none_or(str::is_empty))
    }) {
        return Err(format!("{owner}: incomplete residual evidence"));
    }
    let source = PathBuf::from(format!("games/gs1/recon/en/main/{stem}.c"));
    let work = PathBuf::from(format!("out/family-proof/{stem}"));
    fs::create_dir_all(&work).map_err(|error| format!("{}: {error}", work.display()))?;
    let config = CandidateCompilerConfiguration { family: Some(CandidateCompilerFamily::Routed), ..Default::default() };
    let address = u32::from_str_radix(stem, 16).map_err(|error| error.to_string())?;
    let route = SourceOwner::Main(address).routing_path();
    let rom_path = compiler_core::routing::root().join("roms/gs1-en.gba");
    let rom = fs::read(&rom_path).map_err(|error| format!("{}: {error}", rom_path.display()))?;
    let verification = verify_candidate_owned_routed(
        &source.to_string_lossy(),
        &route.to_string_lossy(),
        stem,
        &rom,
        &work.to_string_lossy(),
        &[],
        ROM_BASE,
        CompilerTarget::Gs1,
        &config,
    )?;
    if verification.actual.len() != owner_bytes || verification.expected.len() != owner_bytes {
        return Err(format!(
            "{owner}: live extent is {}/{}, expected {owner_bytes}",
            verification.actual.len(),
            verification.expected.len()
        ));
    }
    let differing = verification
        .actual
        .chunks(2)
        .zip(verification.expected.chunks(2))
        .filter(|(left, right)| left != right)
        .count();
    if evidence["differing_halfwords"].as_u64() != Some(differing as u64) {
        return Err(format!("{owner}: recorded and live differing-halfword counts disagree"));
    }
    let candidate_binary = work.join("candidate.bin");
    let reference_binary = work.join("reference.bin");
    write(&candidate_binary, &verification.actual)?;
    write(&reference_binary, &verification.expected)?;
    let candidate = disassembled(&candidate_binary)?;
    let target = disassembled(&reference_binary)?;
    let (class, wrong) = residual_class(&candidate, &target);
    if class != "ordering" || wrong != 0 || candidate.len() != target.len() {
        return Err(format!("{owner}: live residual is {class}/{wrong}"));
    }
    let pairs = align_streams(&candidate, &target);
    let groups =
        reorder_groups(&pairs, family.maximum_mismatch_run_rows).map_err(|error| format!("{owner}: {error}"))?;
    if groups == 0 {
        return Err(format!("{owner}: retained proof unexpectedly compiles exact"));
    }
    if groups < residual.len() {
        return Err(format!(
            "{owner}: {} live reorder groups cannot support {} recorded hunks",
            groups,
            residual.len()
        ));
    }
    Ok(owner_bytes)
}

fn reorder_groups(pairs: &[(Option<String>, Option<String>)], maximum_rows: usize) -> Result<usize, String> {
    let mut balance = BTreeMap::<String, i64>::new();
    let mut start = None;
    let mut groups = 0;
    for (row, pair) in pairs.iter().enumerate() {
        if start.is_none() && row_matches(pair) {
            continue;
        }
        let begin = *start.get_or_insert(row);
        if let Some(left) = &pair.0 {
            *balance.entry(without_pc_offset(left)).or_default() += 1;
        }
        if let Some(right) = &pair.1 {
            *balance.entry(without_pc_offset(right)).or_default() -= 1;
        }
        balance.retain(|_, count| *count != 0);
        if balance.is_empty() {
            if row + 1 - begin > maximum_rows {
                return Err("reorder group exceeds its local row bound".into());
            }
            groups += 1;
            start = None;
        }
    }
    if start.is_some() {
        return Err("residual does not locally resynchronize".into());
    }
    Ok(groups)
}

fn disassembled(path: &Path) -> Result<Vec<String>, String> {
    let rows = disassemble(&path.to_string_lossy(), 0.0)?;
    let mut offsets = rows.keys().collect::<Vec<_>>();
    offsets.sort_by(f64::total_cmp);
    Ok(offsets.into_iter().filter_map(|offset| rows.get(offset).map(str::to_string)).collect())
}

fn attempted(value: &Value) -> usize {
    match value {
        Value::Object(map) => {
            let own = map.get("attempted_candidates").and_then(Value::as_u64).unwrap_or(0) as usize;
            own + map
                .iter()
                .filter(|(key, _)| key.as_str() != "attempted_candidates")
                .map(|(_, value)| attempted(value))
                .sum::<usize>()
        }
        Value::Array(values) => values.iter().map(attempted).sum(),
        _ => 0,
    }
}

fn row_matches(pair: &(Option<String>, Option<String>)) -> bool {
    matches!(pair, (Some(left), Some(right)) if without_pc_offset(left) == without_pc_offset(right))
}

fn stem(address: u64) -> Result<String, String> {
    if !(0x0800_0000..0x0900_0000).contains(&address) {
        return Err(format!("invalid main owner address 0x{address:08x}"));
    }
    Ok(format!("{address:08x}"))
}

fn json<T: serde::de::DeserializeOwned>(path: impl AsRef<Path>) -> Result<T, String> {
    let path = path.as_ref();
    serde_json::from_str(&read(path)?).map_err(|error| format!("{}: {error}", path.display()))
}

fn read(path: &Path) -> Result<String, String> {
    fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))
}

fn write(path: &Path, bytes: &[u8]) -> Result<(), String> {
    if let Some(parent) = path.parent().filter(|parent| !parent.as_os_str().is_empty()) {
        fs::create_dir_all(parent).map_err(|error| format!("{}: {error}", parent.display()))?;
    }
    fs::write(path, bytes).map_err(|error| format!("{}: {error}", path.display()))
}

fn write_json(path: &Path, value: &impl Serialize) -> Result<(), String> {
    let text = serde_json::to_string_pretty(value).map_err(|error| format!("{}: {error}", path.display()))? + "\n";
    write(path, text.as_bytes())
}

fn output_path(path: &Path) -> Result<PathBuf, String> {
    let root = compiler_core::routing::root();
    let path = if path.is_absolute() { path.to_path_buf() } else { root.join(path) };
    let mut normalized = PathBuf::new();
    for component in path.components() {
        match component {
            Component::CurDir => {}
            Component::ParentDir => {
                normalized.pop();
            }
            value => normalized.push(value.as_os_str()),
        }
    }
    let trusted = root.join("out");
    if normalized == trusted || !normalized.starts_with(&trusted) {
        return Err(format!("transplant output must be under out/: {}", normalized.display()));
    }
    fs::create_dir_all(&normalized).map_err(|error| format!("{}: {error}", normalized.display()))?;
    let trusted = trusted.canonicalize().map_err(|error| format!("{}: {error}", trusted.display()))?;
    let resolved = normalized.canonicalize().map_err(|error| format!("{}: {error}", normalized.display()))?;
    if resolved == trusted || !resolved.starts_with(&trusted) {
        return Err(format!("transplant output resolves outside out/: {}", resolved.display()));
    }
    Ok(resolved)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn instruction_tokens_erase_addresses_and_registers() {
        assert_eq!(token("bl Func_08001234"), "bl CALL");
        assert_eq!(token("bne .L42"), "bne TARGET");
        assert_eq!(token("ldr r3, [r2, #12]"), "ldr R, [R, #N]");
    }

    #[test]
    fn call_targets_preserve_family_identity() {
        assert_eq!(call_target("bl Func_08001234"), Some("Func_08001234".into()));
        assert_eq!(call_target("blx r3"), Some("r3".into()));
        assert_eq!(call_target("bne .L42"), None);
    }

    #[test]
    fn search_count_is_recursive_without_double_counting_leaf_fields() {
        let value = serde_json::json!({
            "safe": {"attempted_candidates": 20000, "compile_failures": 1},
            "classic": {"attempted_candidates": 10000}
        });
        assert_eq!(attempted(&value), 30000);
    }

    #[test]
    fn cosine_and_ratios_are_stable_basis_points() {
        let left = BTreeMap::from([("a".into(), 2), ("b".into(), 1)]);
        assert_eq!(cosine(&left, &left), 10_000);
        assert_eq!(ratio(3, 4), 7500);
    }

    #[test]
    fn transplant_rejects_unqualified_similarity() {
        assert!(require_family("main:08000000", None).is_err());
        assert!(require_family("main:08000000", Some("family")).is_ok());
    }

    #[test]
    fn reordered_instruction_around_a_match_is_one_group() {
        let pairs = vec![
            (Some("adds r2, #100".into()), None),
            (Some("str r3, [r5]".into()), Some("str r3, [r5]".into())),
            (None, Some("adds r2, #100".into())),
        ];
        assert_eq!(reorder_groups(&pairs, 3).unwrap(), 1);
        assert!(reorder_groups(&pairs, 2).is_err());
    }
}
