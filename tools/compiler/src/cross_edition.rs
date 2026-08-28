use candidate_compiler::verify::{
    compile_to_assembly, CandidateCompilerConfiguration, CandidateCompilerFamily,
};
use compiler_core::routing::CompilerTarget;
use compiler_core::source_paths::{SourceOwner, SourcePaths};
use compiler_core::symbol_is_thumb;
use compiler_core::translation_units::{TranslationUnit, TranslationUnits};
use objdiff_core::{
    diff::{ArmArchVersion, DiffObjConfig, DiffSide},
    obj,
};
use overlay_disasm::compile_declared_overlay_unit;
use serde::Serialize;
use std::{
    collections::{BTreeMap, BTreeSet},
    fs,
    path::{Path, PathBuf},
    process::Command,
    sync::OnceLock,
};
const ROM_BASE: u64 = 0x0800_0000;
const EDITIONS: [&str; 6] = ["ja", "en", "de", "es", "fr", "it"];
const OVERLAY_BASE: u64 = 0x0200_0000;
const OVERLAY_FIRST: usize = 0x36f;
const OVERLAY_LAST: usize = 0x3ce;
const CORRESPONDENCE_SCHEMA_VERSION: u32 = 4;
const CORPUS_EDITION_BUILD_SCHEMA_VERSION: u32 = 3;
const USAGE: &str = "usage: compiler cross-edition ([--calls] [--json] [--rom-dir DIR] [--object FILE] [--edition-build FILE] <8-digit-owner> | [--json] [--rom-dir DIR] --span BYTES <resource_xxx:02xxxxxx> | --all [--rom-dir DIR] [--object-dir DIR] [--write FILE] [--edition-build FILE] | --all-overlays [--rom-dir DIR] [--write FILE] [--edition-build FILE])";
#[derive(Debug, Serialize)]
struct Report {
    schema_version: u32,
    game: &'static str,
    owner: String,
    owner_edition: &'static str,
    base_edition: &'static str,
    relocation_source_edition: &'static str,
    size: usize,
    relocations: usize,
    relocation_bytes: usize,
    literal_fields: usize,
    literal_bytes: usize,
    core_bytes: usize,
    relocation_kinds: BTreeMap<String, usize>,
    call_targets: Vec<CallTarget>,
    core_identical: bool,
    editions: Vec<EditionReport>,
}
#[derive(Debug, Serialize)]
struct CorpusReport {
    schema_version: u32,
    game: &'static str,
    source_edition: &'static str,
    base_edition: &'static str,
    source_state: &'static str,
    #[serde(skip_serializing_if = "Option::is_none")]
    resources_total: Option<usize>,
    owners_total: usize,
    owner_symbol_bytes: usize,
    matched_owners: usize,
    matched_bytes: usize,
    shared_core_owners: usize,
    shared_core_bytes: usize,
    regional_core_owners: usize,
    regional_core_bytes: usize,
    unresolved_owners: usize,
    compliance_excluded_owners: usize,
    #[serde(skip_serializing_if = "Option::is_none")]
    resource_tables: Option<BTreeMap<String, String>>,
    location_method_codes: BTreeMap<&'static str, &'static str>,
    editions: Vec<EditionSummary>,
    owners: Vec<CorpusOwner>,
    unresolved: Vec<UnresolvedOwner>,
}
#[derive(Debug, Serialize)]
struct EditionSummary {
    edition: String,
    matched_owners: usize,
    matched_bytes: usize,
    core_identical_owners: usize,
    core_identical_bytes: usize,
    core_different_owners: usize,
    core_different_bytes: usize,
}
type EditionTotals = BTreeMap<&'static str, [usize; 6]>;
type EditionLocations = BTreeMap<String, BTreeMap<String, u64>>;
fn edition_totals() -> EditionTotals {
    EDITIONS
        .into_iter()
        .map(|edition| (edition, [0; 6]))
        .collect()
}
fn record_edition(totals: &mut EditionTotals, edition: &str, size: usize, identical: bool) {
    let totals = totals.get_mut(edition).expect("known edition");
    totals[0] += 1;
    totals[1] += size;
    let index = if identical { 2 } else { 4 };
    totals[index] += 1;
    totals[index + 1] += size;
}
fn edition_summaries(totals: &EditionTotals) -> Vec<EditionSummary> {
    EDITIONS
        .into_iter()
        .map(|edition| {
            let totals = totals[edition];
            EditionSummary {
                edition: edition.into(),
                matched_owners: totals[0],
                matched_bytes: totals[1],
                core_identical_owners: totals[2],
                core_identical_bytes: totals[3],
                core_different_owners: totals[4],
                core_different_bytes: totals[5],
            }
        })
        .collect()
}
#[derive(Debug, Serialize)]
struct CorpusOwner {
    en_owner: String,
    size: usize,
    #[serde(skip_serializing_if = "Option::is_none")]
    core_bytes: Option<usize>,
    #[serde(skip_serializing_if = "BTreeMap::is_empty")]
    start_overrides: BTreeMap<String, String>,
    location_methods: String,
    #[serde(skip_serializing_if = "BTreeMap::is_empty")]
    core_diff_bytes_from_ja: BTreeMap<String, usize>,
}
impl CorpusOwner {
    fn new(en_owner: String, size: usize, core_bytes: Option<usize>) -> Self {
        Self {
            en_owner,
            size,
            core_bytes,
            start_overrides: BTreeMap::new(),
            location_methods: String::with_capacity(EDITIONS.len()),
            core_diff_bytes_from_ja: BTreeMap::new(),
        }
    }

    fn record_edition(
        &mut self,
        totals: &mut EditionTotals,
        edition: &str,
        start: String,
        canonical_start: &str,
        location_method: &str,
        difference: usize,
    ) -> Result<(), String> {
        if start != canonical_start {
            self.start_overrides.insert(edition.into(), start);
        }
        self.location_methods
            .push(location_method_code(location_method)?);
        if difference != 0 {
            self.core_diff_bytes_from_ja
                .insert(edition.into(), difference);
        }
        record_edition(totals, edition, self.size, difference == 0);
        Ok(())
    }
}
#[derive(Debug, Serialize)]
struct UnresolvedOwner {
    en_owner: String,
    #[serde(skip_serializing_if = "Option::is_none")]
    size: Option<usize>,
    error: String,
}
#[derive(Debug, Serialize)]
struct EditionReport {
    edition: String,
    start: String,
    shift: i64,
    raw_diff_bytes: usize,
    raw_diff_halfwords: usize,
    relocation_diff_bytes: usize,
    literal_diff_bytes: usize,
    relocation_fields_changed: usize,
    literal_fields_changed: usize,
    call_fields_changed: usize,
    absolute_fields_changed: usize,
    core_diff_bytes: usize,
    anchor_matches: usize,
    core_identical: bool,
}
#[derive(Debug, Serialize)]
struct CallTarget {
    en_symbol: String,
    sites: usize,
    editions: Vec<TargetAddress>,
}
#[derive(Debug, Serialize)]
struct TargetAddress {
    edition: String,
    address: String,
}
#[derive(Clone)]
struct RelocationSite {
    offset: usize,
    size: usize,
    kind: String,
    symbol: String,
    addend: i64,
    external: bool,
}
struct LiteralSite {
    offset: usize,
    size: usize,
}
struct Options {
    owner: Option<String>,
    object: Option<PathBuf>,
    object_dir: PathBuf,
    rom_dir: PathBuf,
    json: bool,
    calls: bool,
    all: bool,
    all_overlays: bool,
    write: Option<PathBuf>,
    edition_build: Option<PathBuf>,
    span: Option<usize>,
}
struct Registers {
    sources: SourcePaths,
    units: TranslationUnits,
}
#[derive(Debug, Serialize)]
struct EditionBuildReport {
    schema_version: u32,
    game: &'static str,
    source_edition: &'static str,
    source: String,
    object: String,
    owner_symbol: String,
    size: usize,
    #[serde(skip_serializing_if = "is_false")]
    edition_variant: bool,
    all_exact: bool,
    editions: Vec<EditionBuildEntry>,
}
#[derive(Debug, Serialize)]
struct EditionBuildEntry {
    edition: String,
    start: String,
    size: usize,
    external_symbols: BTreeMap<String, String>,
    differing_bytes: Option<usize>,
    byte_exact: bool,
    error: Option<String>,
}
#[derive(Debug, Serialize)]
struct CorpusEditionBuildReport {
    schema_version: u32,
    game: &'static str,
    source_edition: &'static str,
    source_state: &'static str,
    owners_total: usize,
    owner_symbol_bytes: usize,
    located_owners: usize,
    unresolved_owners: usize,
    built_owners: usize,
    all_exact_owners: usize,
    all_exact_bytes: usize,
    all_exact: bool,
    editions: Vec<EditionBuildSummary>,
    owners: Vec<CorpusEditionBuildOwner>,
    failures: Vec<EditionBuildFailure>,
}
#[derive(Debug, Serialize)]
struct CorpusEditionBuildOwner {
    en_owner: String,
    size: usize,
    #[serde(skip_serializing_if = "is_false")]
    edition_variant: bool,
    #[serde(skip_serializing_if = "BTreeMap::is_empty")]
    start_overrides: BTreeMap<String, String>,
    #[serde(skip_serializing_if = "BTreeMap::is_empty")]
    size_overrides: BTreeMap<String, usize>,
    #[serde(skip_serializing_if = "BTreeMap::is_empty")]
    differing_bytes: BTreeMap<String, usize>,
    #[serde(skip_serializing_if = "BTreeMap::is_empty")]
    errors: BTreeMap<String, String>,
}
#[derive(Debug, Serialize)]
struct EditionBuildSummary {
    edition: String,
    checked_owners: usize,
    checked_bytes: usize,
    byte_exact_owners: usize,
    byte_exact_bytes: usize,
    nonexact_owners: usize,
    nonexact_bytes: usize,
    differing_bytes: usize,
    unbuildable_owners: usize,
}
#[derive(Debug, Serialize)]
struct EditionBuildFailure {
    en_owner: String,
    error: String,
}
fn is_false(value: &bool) -> bool {
    !*value
}
struct EditionRoms {
    images: BTreeMap<&'static str, Vec<u8>>,
}
struct FoundEdition {
    start: usize,
    bytes: Vec<u8>,
    anchor_matches: usize,
}
struct ComparisonContext<'a> {
    base_start: usize,
    base: &'a [u8],
    mask: &'a [bool],
    relocation_mask: &'a [bool],
    relocations: &'a [RelocationSite],
    literals: &'a [LiteralSite],
}
#[derive(Clone, Copy, Eq, Ord, PartialEq, PartialOrd)]
struct LocationKey {
    group: Option<usize>,
    en_offset: usize,
}
struct LocationAnchor {
    key: LocationKey,
    starts: BTreeMap<&'static str, usize>,
}
struct OverlayOwner {
    name: String,
    resource: usize,
    en_offset: usize,
    size: usize,
}
struct OverlayMatch {
    owner: OverlayOwner,
    mask: Vec<bool>,
    starts: BTreeMap<&'static str, usize>,
    methods: BTreeMap<&'static str, &'static str>,
}
trait Located {
    fn location(&self) -> Result<LocationAnchor, String>;
}
#[derive(Debug, Serialize)]
struct OverlayOwnerReport {
    schema_version: u32,
    game: &'static str,
    owner: String,
    size: usize,
    core_bytes: usize,
    core_identical: bool,
    editions: Vec<OverlayOwnerEdition>,
}
#[derive(Debug, Serialize)]
struct OverlayOwnerEdition {
    edition: String,
    start: String,
    location_method: String,
    core_diff_bytes: usize,
    core_identical: bool,
}
pub fn run(args: &[String]) -> Result<(), String> {
    let options = parse(args)?;
    let roms = read_roms(&options.rom_dir)?;
    if options.all_overlays {
        return run_all_overlays(&options, &roms);
    }
    if options.all {
        return run_all(&options, &roms);
    }
    let owner = options.owner.as_deref().ok_or(USAGE)?;
    if owner.starts_with("resource_") {
        return run_overlay_owner(&options, &roms, owner);
    }
    let owner_address = owner_address(owner)?;
    let object_path = resolve_object(owner, options.object.as_deref(), &options.object_dir)?;
    let report = analyze_owner(
        owner,
        owner_address,
        &object_path,
        &roms,
        options.calls,
        None,
    )?;
    let edition_build = options
        .edition_build
        .as_deref()
        .map(|path| write_edition_build(path, owner, &object_path, &report, &roms))
        .transpose()?;
    if options.json {
        println!(
            "{}",
            serde_json::to_string_pretty(&report)
                .map_err(|error| format!("serialize report: {error}"))?
        );
    } else {
        print_report(&report, options.calls);
    }
    let all_exact = edition_build
        .as_ref()
        .map_or(report.core_identical, |build| build.all_exact);
    if all_exact {
        Ok(())
    } else {
        Err("one or more editions are not byte-exact".into())
    }
}
fn analyze_owner(
    owner: &str,
    owner_address: u64,
    object_path: &Path,
    roms: &EditionRoms,
    calls: bool,
    hints: Option<&BTreeMap<&str, usize>>,
) -> Result<Report, String> {
    let (size, symbol_address, _, relocation_mask, relocations, _) =
        relocation_mask(object_path, owner)?;
    let literals = literal_sites(object_path, symbol_address, size)?;
    let mut mask = relocation_mask.clone();
    for literal in &literals {
        mask[literal.offset..literal.offset + literal.size].fill(true);
    }
    let en_rom = &roms.images["en"];
    let en_offset = rom_offset(owner_address, en_rom.len())?;
    let en_owner = en_rom
        .get(en_offset..en_offset + size)
        .ok_or("EN owner extends past ROM")?;
    let anchors = anchors(en_owner, &mask);
    if anchors.is_empty() && hints.is_none() {
        return Err(
            "owner has no relocation-free anchor long enough to locate counterparts".into(),
        );
    }
    let mut found = BTreeMap::new();
    found.insert(
        "en",
        FoundEdition {
            start: en_offset,
            bytes: en_owner.to_vec(),
            anchor_matches: anchors.len(),
        },
    );
    for edition in EDITIONS.into_iter().filter(|edition| *edition != "en") {
        let rom = &roms.images[edition];
        let located = if anchors.is_empty() {
            Err("no relocation-free anchor found in ROM".into())
        } else {
            locate(en_owner, &mask, &anchors, rom)
        };
        let (start, support) = located
            .or_else(|global_error| {
                hints
                    .and_then(|values| values.get(edition).copied())
                    .ok_or(global_error)
                    .and_then(|predicted| {
                        locate_near_exact(en_owner, &mask, rom, predicted, 0x1000, ROM_BASE)
                    })
            })
            .map_err(|error| format!("{edition}: {error}"))?;
        found.insert(
            edition,
            FoundEdition {
                start,
                bytes: rom[start..start + size].to_vec(),
                anchor_matches: support,
            },
        );
    }
    let ja = found.get("ja").ok_or("JA counterpart was not located")?;
    let context = ComparisonContext {
        base_start: ja.start,
        base: &ja.bytes,
        mask: &mask,
        relocation_mask: &relocation_mask,
        relocations: &relocations,
        literals: &literals,
    };
    let editions = EDITIONS
        .into_iter()
        .map(|edition| {
            let entry = &found[edition];
            edition_report(
                edition,
                entry.start,
                &entry.bytes,
                &context,
                entry.anchor_matches,
            )
        })
        .collect::<Vec<_>>();
    let call_targets = if calls {
        call_targets(&relocations, &found)?
    } else {
        Vec::new()
    };
    let mut relocation_kinds = BTreeMap::new();
    for relocation in &relocations {
        *relocation_kinds.entry(relocation.kind.clone()).or_default() += 1;
    }
    let report = Report {
        schema_version: 1,
        game: "gs1",
        owner: owner.into(),
        owner_edition: "en",
        base_edition: "ja",
        relocation_source_edition: "en",
        size,
        relocations: relocations.len(),
        relocation_bytes: relocation_mask.iter().filter(|value| **value).count(),
        literal_fields: literals.len(),
        literal_bytes: literals.iter().map(|literal| literal.size).sum(),
        core_bytes: mask.iter().filter(|value| !**value).count(),
        relocation_kinds,
        call_targets,
        core_identical: editions.iter().all(|edition| edition.core_identical),
        editions,
    };
    Ok(report)
}
fn write_edition_build(
    path: &Path,
    owner: &str,
    object_path: &Path,
    report: &Report,
    roms: &EditionRoms,
) -> Result<EditionBuildReport, String> {
    let build = edition_build_report(owner, object_path, report, roms, &mut BTreeMap::new(), None)?;
    write_json(path, &build, "edition build")?;
    println!(
        "edition_build={} owner={} editions={} all_exact={}",
        path.display(),
        owner,
        build.editions.len(),
        build.all_exact
    );
    Ok(build)
}
fn compile_edition_object(owner: &str, edition: &str, source: &Path) -> Result<PathBuf, String> {
    // Pure per-process compile scratch: nothing outside this run ever reads
    // it, so it lives under system temp rather than the tracked repo tree
    // (this used to leave tens of thousands of files under out/cross-edition
    // that regrew on every `make verify`).
    let output = std::env::temp_dir()
        .join("alchemy-cross-edition")
        .join(owner)
        .join("compiled")
        .join(edition);
    fs::create_dir_all(&output).map_err(|error| format!("{}: {error}", output.display()))?;
    let wrapper = output.join("source.c");
    let source =
        fs::canonicalize(source).map_err(|error| format!("{}: {error}", source.display()))?;
    let include = source
        .to_string_lossy()
        .replace('\\', "\\\\")
        .replace('"', "\\\"");
    fs::write(
        &wrapper,
        format!(
            "#define GS1_EDITION_{} 1\n#include \"{}\"\n",
            edition.to_ascii_uppercase(),
            include
        ),
    )
    .map_err(|error| format!("{}: {error}", wrapper.display()))?;
    let wrapper_text = wrapper.to_string_lossy().into_owned();
    let owner_address = u32::from_str_radix(owner, 16)
        .map_err(|error| format!("invalid main owner {owner}: {error}"))?;
    let routing_text = SourceOwner::Main(owner_address)
        .routing_path()
        .to_string_lossy()
        .into_owned();
    let output_text = output.to_string_lossy().into_owned();
    let assembly = compile_to_assembly(
        &wrapper_text,
        &routing_text,
        &output_text,
        &[],
        CompilerTarget::Gs1,
        &CandidateCompilerConfiguration {
            family: Some(CandidateCompilerFamily::Routed),
            ..Default::default()
        },
    )?;
    let object = output.join("owner.o");
    run_tool(
        Command::new("arm-none-eabi-as")
            .args(["-mcpu=arm7tdmi", "-mthumb-interwork", "-o"])
            .arg(&object)
            .arg(&assembly),
        "assemble edition source",
    )?;
    Ok(object)
}
fn compiled_edition_object(
    compiled: &mut BTreeMap<(String, String), Result<PathBuf, String>>,
    route: &str,
    edition: &str,
    source: &Path,
) -> Result<PathBuf, String> {
    compiled
        .entry((route.into(), edition.into()))
        .or_insert_with(|| compile_edition_object(route, edition, source))
        .clone()
}
fn edition_build_report(
    owner: &str,
    object_path: &Path,
    report: &Report,
    roms: &EditionRoms,
    compiled: &mut BTreeMap<(String, String), Result<PathBuf, String>>,
    locations: Option<&EditionLocations>,
) -> Result<EditionBuildReport, String> {
    let (size, symbol_offset, owner_bytes, owner_mask, relocations, complete_object) =
        relocation_mask(object_path, owner)?;
    if size != report.size {
        return Err(format!(
            "edition build object size {size} differs from located owner size {}",
            report.size
        ));
    }
    let owner_symbol = format!("Func_{owner}");
    let output_root = std::env::temp_dir()
        .join("alchemy-cross-edition")
        .join(owner)
        .join("linked");
    fs::create_dir_all(&output_root)
        .map_err(|error| format!("{}: {error}", output_root.display()))?;
    let source_paths = &registers()?.sources;
    let source_owner = SourceOwner::parse(&format!("main:{owner}"))?;
    let unit = translation_unit(owner)?;
    if unit.is_some() && !complete_object {
        return Err(format!(
            "{}: supplied object is not a complete TU",
            object_path.display()
        ));
    }
    let (route, source_path) = unit
        .map(|unit| {
            (
                format!("{:08x}", unit.owners[0].address),
                compiler_core::routing::root().join(&unit.source),
            )
        })
        .unwrap_or_else(|| {
            (
                owner.into(),
                source_paths
                    .mapped_source_path(source_owner)
                    .unwrap_or_else(|| source_paths.source_path(source_owner)),
            )
        });
    let source_report_path = source_path
        .strip_prefix(compiler_core::routing::root())
        .unwrap_or(&source_path)
        .to_string_lossy()
        .replace('\\', "/");
    let edition_variant = fs::read_to_string(&source_path)
        .map_err(|error| format!("{}: {error}", source_path.display()))?
        .contains("#include \"gs1_edition.h\"");
    let compile_each_edition = unit.is_some() || edition_variant;
    let mut editions = Vec::with_capacity(EDITIONS.len());
    for edition in EDITIONS {
        let reported_start = report_start(report, edition)?;
        let variant_object = if compile_each_edition {
            compiled_edition_object(compiled, &route, edition, &source_path)?
        } else {
            object_path.to_path_buf()
        };
        let (
            variant_size,
            variant_symbol_offset,
            variant_bytes,
            variant_mask,
            variant_relocations,
            _,
        ) = if compile_each_edition {
            relocation_mask(&variant_object, owner)?
        } else {
            (
                size,
                symbol_offset,
                owner_bytes.clone(),
                owner_mask.clone(),
                relocations.clone(),
                complete_object,
            )
        };
        let start = if compile_each_edition && edition != "en" {
            locate_near_exact(
                &variant_bytes,
                &variant_mask,
                &roms.images[edition],
                reported_start,
                0x1000,
                ROM_BASE,
            )
            .map(|(start, _)| start)
            .unwrap_or(reported_start)
        } else {
            reported_start
        };
        let reference = roms.images[edition]
            .get(start..start + variant_size)
            .ok_or_else(|| format!("{edition}: owner extends past ROM"))?;
        let built = derive_link_symbols(
            &variant_relocations,
            reference,
            start,
            &owner_symbol,
            unit.is_some(),
            edition,
            locations,
        )
        .and_then(|values| {
            link_owner_for_edition(
                &output_root,
                edition,
                &variant_object,
                &owner_symbol,
                ROM_BASE + start as u64,
                variant_symbol_offset,
                variant_size,
                &values,
            )
            .map(|linked| (values, linked))
        });
        match built {
            Ok((values, linked)) => {
                let differing_bytes = reference
                    .iter()
                    .zip(&linked)
                    .filter(|(left, right)| left != right)
                    .count()
                    + reference.len().abs_diff(linked.len());
                editions.push(EditionBuildEntry {
                    edition: edition.into(),
                    start: format!("0x{:08x}", ROM_BASE + start as u64),
                    size: variant_size,
                    external_symbols: values
                        .into_iter()
                        .map(|(name, value)| (name, format!("0x{value:08x}")))
                        .collect(),
                    differing_bytes: Some(differing_bytes),
                    byte_exact: differing_bytes == 0,
                    error: None,
                });
            }
            Err(error) => editions.push(EditionBuildEntry {
                edition: edition.into(),
                start: format!("0x{:08x}", ROM_BASE + start as u64),
                size: variant_size,
                external_symbols: BTreeMap::new(),
                differing_bytes: None,
                byte_exact: false,
                error: Some(error),
            }),
        }
    }
    let build = EditionBuildReport {
        schema_version: 1,
        game: "gs1",
        source_edition: "en",
        source: source_report_path,
        object: object_path.display().to_string(),
        owner_symbol,
        size,
        edition_variant,
        all_exact: editions.iter().all(|edition| edition.byte_exact),
        editions,
    };
    Ok(build)
}
fn write_json<T: Serialize>(path: &Path, value: &T, label: &str) -> Result<(), String> {
    if let Some(parent) = path
        .parent()
        .filter(|parent| !parent.as_os_str().is_empty())
    {
        fs::create_dir_all(parent).map_err(|error| format!("{}: {error}", parent.display()))?;
    }
    let json = serde_json::to_string_pretty(value)
        .map_err(|error| format!("serialize {label}: {error}"))?
        + "\n";
    fs::write(path, json).map_err(|error| format!("{}: {error}", path.display()))?;
    Ok(())
}
fn corpus_report(
    source_state: &'static str,
    resources_total: Option<usize>,
    owners_total: usize,
    owner_symbol_bytes: usize,
    resource_tables: Option<BTreeMap<String, String>>,
    mut owners: Vec<CorpusOwner>,
    unresolved: Vec<UnresolvedOwner>,
    totals: &EditionTotals,
) -> CorpusReport {
    owners.sort_by(|left, right| left.en_owner.cmp(&right.en_owner));
    let shared = |owner: &&CorpusOwner| owner.core_diff_bytes_from_ja.is_empty();
    let regional = |owner: &&CorpusOwner| !owner.core_diff_bytes_from_ja.is_empty();
    CorpusReport {
        schema_version: CORRESPONDENCE_SCHEMA_VERSION,
        game: "gs1",
        source_edition: "en",
        base_edition: "ja",
        source_state,
        resources_total,
        owners_total,
        owner_symbol_bytes,
        matched_owners: owners.len(),
        matched_bytes: owners.iter().map(|owner| owner.size).sum(),
        shared_core_owners: owners.iter().filter(shared).count(),
        shared_core_bytes: owners.iter().filter(shared).map(|owner| owner.size).sum(),
        regional_core_owners: owners.iter().filter(regional).count(),
        regional_core_bytes: owners.iter().filter(regional).map(|owner| owner.size).sum(),
        unresolved_owners: unresolved.len(),
        compliance_excluded_owners: unresolved
            .iter()
            .filter(|owner| owner.error.starts_with("compliance:"))
            .count(),
        resource_tables,
        location_method_codes: location_method_codes(),
        editions: edition_summaries(totals),
        owners,
        unresolved,
    }
}
fn emit_corpus_report(
    options: &Options,
    report: &CorpusReport,
    label: &str,
    output_key: &str,
) -> Result<(), String> {
    if let Some(path) = &options.write {
        write_json(path, report, label)?;
        println!(
            "{output_key}={} owners={} matched={} shared_core={} regional_core={} unresolved={}",
            path.display(),
            report.owners_total,
            report.matched_owners,
            report.shared_core_owners,
            report.regional_core_owners,
            report.unresolved_owners
        );
    } else {
        println!(
            "{}",
            serde_json::to_string_pretty(report)
                .map_err(|error| format!("serialize {label}: {error}"))?
        );
    }
    Ok(())
}
fn compact_corpus_edition_build_owner(
    owner: EditionBuildReport,
) -> Result<CorpusEditionBuildOwner, String> {
    let en_owner = canonical_main_owner_id(owner.owner_symbol.trim_start_matches("Func_"))?;
    let canonical_start = format!("0x{}", en_owner.trim_start_matches("main:"));
    let mut editions = BTreeSet::new();
    let mut start_overrides = BTreeMap::new();
    let mut size_overrides = BTreeMap::new();
    let mut differing_bytes = BTreeMap::new();
    let mut errors = BTreeMap::new();
    for entry in owner.editions {
        if !editions.insert(entry.edition.clone()) {
            return Err(format!("{en_owner}: duplicate {} edition", entry.edition));
        }
        if entry.start != canonical_start {
            start_overrides.insert(entry.edition.clone(), entry.start);
        }
        if entry.size != owner.size {
            size_overrides.insert(entry.edition.clone(), entry.size);
        }
        match (entry.differing_bytes, entry.error) {
            (Some(difference), None) => {
                if entry.byte_exact != (difference == 0) {
                    return Err(format!(
                        "{en_owner}: {} byte_exact disagrees with differing_bytes",
                        entry.edition
                    ));
                }
                if difference != 0 {
                    differing_bytes.insert(entry.edition, difference);
                }
            }
            (None, Some(error)) if !entry.byte_exact => {
                errors.insert(entry.edition, error);
            }
            _ => {
                return Err(format!(
                    "{en_owner}: {} must have either differing_bytes or an error",
                    entry.edition
                ));
            }
        }
    }
    if editions.len() != EDITIONS.len()
        || EDITIONS
            .into_iter()
            .any(|edition| !editions.contains(edition))
    {
        return Err(format!("{en_owner}: edition build set is incomplete"));
    }
    let derived_all_exact = differing_bytes.is_empty() && errors.is_empty();
    if owner.all_exact != derived_all_exact {
        return Err(format!(
            "{en_owner}: all_exact disagrees with sparse edition evidence"
        ));
    }
    Ok(CorpusEditionBuildOwner {
        en_owner,
        size: owner.size,
        edition_variant: owner.edition_variant,
        start_overrides,
        size_overrides,
        differing_bytes,
        errors,
    })
}
fn write_corpus_edition_build(
    path: &Path,
    objects: &BTreeMap<String, PathBuf>,
    reports: &BTreeMap<String, Report>,
    owners_total: usize,
    owner_symbol_bytes: usize,
    unresolved_owners: usize,
    roms: &EditionRoms,
) -> Result<CorpusEditionBuildReport, String> {
    let (mut owners, mut failures, mut compiled) = (Vec::new(), Vec::new(), BTreeMap::new());
    let mut locations = EditionLocations::new();
    for (owner, report) in reports {
        for entry in &report.editions {
            locations
                .entry(format!("Func_{owner}"))
                .or_default()
                .insert(
                    entry.edition.clone(),
                    ROM_BASE + parse_rom_address(&entry.start)? as u64,
                );
        }
    }
    for (index, (owner, report)) in reports.iter().enumerate() {
        let object = &objects[owner];
        match edition_build_report(
            owner,
            &object,
            report,
            roms,
            &mut compiled,
            Some(&locations),
        ) {
            Ok(build) => owners.push(build),
            Err(error) => failures.push(EditionBuildFailure {
                en_owner: canonical_main_owner_id(owner)?,
                error,
            }),
        }
        if (index + 1) % 100 == 0 || index + 1 == reports.len() {
            eprintln!(
                "edition builds={}/{} failures={}",
                owners.len(),
                index + 1,
                failures.len()
            );
        }
    }
    let editions = EDITIONS
        .into_iter()
        .map(|edition| {
            let mut summary = EditionBuildSummary {
                edition: edition.into(),
                checked_owners: 0,
                checked_bytes: 0,
                byte_exact_owners: 0,
                byte_exact_bytes: 0,
                nonexact_owners: 0,
                nonexact_bytes: 0,
                differing_bytes: 0,
                unbuildable_owners: 0,
            };
            for owner in &owners {
                let entry = owner
                    .editions
                    .iter()
                    .find(|entry| entry.edition == edition)
                    .expect("every edition build contains every edition");
                summary.checked_owners += 1;
                summary.checked_bytes += entry.size;
                if let Some(differing_bytes) = entry.differing_bytes {
                    summary.differing_bytes += differing_bytes;
                } else {
                    summary.unbuildable_owners += 1;
                }
                if entry.byte_exact {
                    summary.byte_exact_owners += 1;
                    summary.byte_exact_bytes += entry.size;
                } else {
                    summary.nonexact_owners += 1;
                    summary.nonexact_bytes += entry.size;
                }
            }
            summary
        })
        .collect::<Vec<_>>();
    let all_exact_owners = owners.iter().filter(|owner| owner.all_exact).count();
    let all_exact_bytes = owners
        .iter()
        .filter(|owner| owner.all_exact)
        .map(|owner| owner.size)
        .sum();
    let owners = owners
        .into_iter()
        .map(compact_corpus_edition_build_owner)
        .collect::<Result<Vec<_>, String>>()?;
    let build = CorpusEditionBuildReport {
        schema_version: CORPUS_EDITION_BUILD_SCHEMA_VERSION,
        game: "gs1",
        source_edition: "en",
        source_state: "byte-exact C compiled or relinked per edition",
        owners_total,
        owner_symbol_bytes,
        located_owners: reports.len(),
        unresolved_owners,
        built_owners: owners.len(),
        all_exact_owners,
        all_exact_bytes,
        all_exact: unresolved_owners == 0
            && failures.is_empty()
            && owners.len() == owners_total
            && all_exact_owners == owners_total,
        editions,
        owners,
        failures,
    };
    write_json(path, &build, "corpus edition build")?;
    println!(
        "edition_builds={} owners={} built={} all_exact={} unresolved={} failures={}",
        path.display(),
        build.owners_total,
        build.built_owners,
        build.all_exact_owners,
        build.unresolved_owners,
        build.failures.len()
    );
    Ok(build)
}
fn derive_link_symbols(
    relocations: &[RelocationSite],
    reference: &[u8],
    start: usize,
    owner_symbol: &str,
    complete_unit: bool,
    edition: &str,
    locations: Option<&EditionLocations>,
) -> Result<BTreeMap<String, u64>, String> {
    let mut values = BTreeMap::new();
    for site in relocations.iter().filter(|site| {
        site.external
            || complete_unit && site.symbol.starts_with("Func_") && site.symbol != owner_symbol
    }) {
        let value = if is_thumb_call(site) {
            thumb_bl_target(reference, start, site.offset)?
        } else if site.kind == "R_ARM_ABS32" {
            let bytes = reference
                .get(site.offset..site.offset + 4)
                .ok_or_else(|| format!("{} relocation extends past owner", site.symbol))?;
            let word = u32::from_le_bytes(bytes.try_into().expect("four-byte relocation"));
            let value = word.wrapping_sub(site.addend as u32) as u64;
            if symbol_is_thumb(&site.symbol) {
                value & !1
            } else {
                value
            }
        } else {
            return Err(format!(
                "unsupported external relocation {} for {}",
                site.kind, site.symbol
            ));
        };
        let value = proved_symbol_location(locations, &site.symbol, edition, value)?;
        if let Some(previous) = values.insert(site.symbol.clone(), value) {
            if previous != value {
                return Err(format!(
                    "{} resolves inconsistently: 0x{previous:08x} and 0x{value:08x}",
                    site.symbol
                ));
            }
        }
    }
    Ok(values)
}
fn proved_symbol_location(
    locations: Option<&EditionLocations>,
    symbol: &str,
    edition: &str,
    decoded: u64,
) -> Result<u64, String> {
    let proved = locations
        .and_then(|locations| locations.get(symbol)?.get(edition))
        .copied();
    match proved {
        Some(proved) if proved != decoded => Err(format!(
            "{edition}: {symbol} decodes to 0x{decoded:08x}, proved at 0x{proved:08x}"
        )),
        Some(proved) => Ok(proved),
        None if locations.is_none() && address_function(symbol) => Err(format!(
            "{edition}: {symbol} has no proved counterpart location"
        )),
        None => Ok(decoded),
    }
}
fn address_function(symbol: &str) -> bool {
    symbol
        .strip_prefix("Func_")
        .is_some_and(|value| value.len() == 8 && value.bytes().all(|byte| byte.is_ascii_hexdigit()))
}
fn link_owner_for_edition(
    output_root: &Path,
    edition: &str,
    object_path: &Path,
    owner_symbol: &str,
    start: u64,
    symbol_offset: u64,
    size: usize,
    values: &BTreeMap<String, u64>,
) -> Result<Vec<u8>, String> {
    let output = output_root.join(edition);
    fs::create_dir_all(&output).map_err(|error| format!("{}: {error}", output.display()))?;
    let symbols_source = output.join("symbols.s");
    let symbols_object = output.join("symbols.o");
    let canonical_object = output.join("canonical.o");
    let elf = output.join("owner.elf");
    let binary = output.join("owner.bin");
    let mut source = String::from(".syntax unified\n.thumb\n");
    for (name, value) in values {
        let directive = if symbol_is_thumb(name) {
            ".thumb_set"
        } else {
            ".set"
        };
        source.push_str(&format!(
            ".global {name}\n{directive} {name}, 0x{value:08x}\n"
        ));
    }
    fs::write(&symbols_source, source)
        .map_err(|error| format!("{}: {error}", symbols_source.display()))?;
    run_tool(
        Command::new("arm-none-eabi-as")
            .args(["-mcpu=arm7tdmi", "-mthumb-interwork", "-o"])
            .arg(&symbols_object)
            .arg(&symbols_source),
        "assemble edition symbols",
    )?;
    let mut canonical = Command::new("arm-none-eabi-objcopy");
    for name in values.keys() {
        canonical.arg(format!("--weaken-symbol={name}"));
    }
    run_tool(
        canonical.arg(object_path).arg(&canonical_object),
        "weaken TU peers",
    )?;
    run_tool(
        Command::new("arm-none-eabi-ld")
            .arg(format!(
                "-Ttext=0x{:08x}",
                start
                    .checked_sub(symbol_offset)
                    .ok_or("owner symbol offset exceeds destination address")?
            ))
            .arg("--unresolved-symbols=ignore-all")
            .args(["-e", owner_symbol, "-o"])
            .arg(&elf)
            .arg(&canonical_object)
            .arg(&symbols_object),
        "link edition owner",
    )?;
    run_tool(
        Command::new("arm-none-eabi-objcopy")
            .args(["-O", "binary", "-j", ".text"])
            .arg(&elf)
            .arg(&binary),
        "extract edition owner",
    )?;
    let bytes = fs::read(&binary).map_err(|error| format!("{}: {error}", binary.display()))?;
    let offset = usize::try_from(symbol_offset).map_err(|_| "owner symbol offset is too large")?;
    let owner = bytes
        .get(offset..offset + size)
        .map(Vec::from)
        .ok_or_else(|| format!("{}: linked owner extends past .text", binary.display()))?;
    fs::write(output.join("owner.slice.bin"), &owner)
        .map_err(|error| format!("{}: {error}", output.display()))?;
    Ok(owner)
}
fn run_tool(command: &mut Command, label: &str) -> Result<(), String> {
    let output = command
        .output()
        .map_err(|error| format!("{label}: {error}"))?;
    if output.status.success() {
        Ok(())
    } else {
        Err(format!(
            "{label} failed: {}",
            String::from_utf8_lossy(&output.stderr).trim()
        ))
    }
}
fn parse(args: &[String]) -> Result<Options, String> {
    let mut owner = None;
    let mut object = None;
    let mut object_dir = PathBuf::from("out/gs1-en/full/claimed/obj");
    let mut rom_dir = PathBuf::from("roms");
    let mut json = false;
    let mut calls = false;
    let mut all = false;
    let mut all_overlays = false;
    let mut write = None;
    let mut edition_build = None;
    let mut span = None;
    let mut index = 0;
    while index < args.len() {
        match args[index].as_str() {
            "--json" => json = true,
            "--calls" => calls = true,
            "--all" => all = true,
            "--all-overlays" => all_overlays = true,
            "--rom-dir" => {
                index += 1;
                rom_dir = PathBuf::from(args.get(index).ok_or(USAGE)?);
            }
            "--object" => {
                index += 1;
                object = Some(PathBuf::from(args.get(index).ok_or(USAGE)?));
            }
            "--object-dir" => {
                index += 1;
                object_dir = PathBuf::from(args.get(index).ok_or(USAGE)?);
            }
            "--write" => {
                index += 1;
                write = Some(PathBuf::from(args.get(index).ok_or(USAGE)?));
            }
            "--edition-build" => {
                index += 1;
                edition_build = Some(PathBuf::from(args.get(index).ok_or(USAGE)?));
            }
            "--span" => {
                index += 1;
                let value = args.get(index).ok_or(USAGE)?;
                let parsed = parse_usize(value)?;
                if parsed == 0 {
                    return Err("--span wants a positive byte count".into());
                }
                span = Some(parsed);
            }
            "-h" | "--help" => return Err(USAGE.into()),
            value if value.starts_with('-') => {
                return Err(format!("unknown option: {value}\n{USAGE}"))
            }
            value if owner.is_none() => owner = Some(value.to_ascii_lowercase()),
            value => return Err(format!("unexpected argument: {value}\n{USAGE}")),
        }
        index += 1;
    }
    if all && all_overlays {
        return Err(format!(
            "--all and --all-overlays are mutually exclusive\n{USAGE}"
        ));
    }
    if (all || all_overlays) && span.is_some() {
        return Err(format!("corpus scans do not accept --span\n{USAGE}"));
    }
    if (all || all_overlays) && owner.is_some() {
        return Err(format!("corpus scans do not accept an owner\n{USAGE}"));
    }
    if !all && !all_overlays && write.is_some() {
        return Err(format!("--write requires a corpus scan\n{USAGE}"));
    }
    if (all || all_overlays) && (object.is_some() || calls || json) {
        return Err(format!(
            "corpus scans omit single-object options and write JSON directly\n{USAGE}"
        ));
    }
    if !all && !all_overlays {
        let value = owner.as_deref().ok_or(USAGE)?;
        if value.starts_with("resource_") {
            parse_explicit_overlay_owner(value, span)?;
            if object.is_some() || calls || edition_build.is_some() {
                return Err(format!("explicit overlay owners do not accept --object, --calls, or --edition-build\n{USAGE}"));
            }
        } else {
            owner_address(value)?;
            if span.is_some() {
                return Err(format!(
                    "--span is only for explicit overlay owners\n{USAGE}"
                ));
            }
        }
    }
    Ok(Options {
        owner,
        object,
        object_dir,
        rom_dir,
        json,
        calls,
        all,
        all_overlays,
        write,
        edition_build,
        span,
    })
}
fn parse_explicit_overlay_owner(value: &str, span: Option<usize>) -> Result<OverlayOwner, String> {
    let source = SourceOwner::parse(value)?;
    let SourceOwner::Overlay { resource, address } = source else {
        return Err(format!("expected an overlay owner: {value}"));
    };
    let resource = resource as usize;
    if !(OVERLAY_FIRST..=OVERLAY_LAST).contains(&resource) {
        return Err(format!(
            "overlay resource {resource:03x} is outside the GS1 code-overlay range"
        ));
    }
    let en_offset = u64::from(address)
        .checked_sub(OVERLAY_BASE)
        .ok_or_else(|| format!("{value}: address is below the overlay base"))?
        as usize;
    Ok(OverlayOwner {
        name: source.id(),
        resource,
        en_offset,
        size: span.ok_or_else(|| format!("explicit overlay owners require --span\n{USAGE}"))?,
    })
}
fn owner_address(owner: &str) -> Result<u64, String> {
    if owner.len() != 8 || !owner.bytes().all(|byte| byte.is_ascii_hexdigit()) {
        return Err(format!(
            "owner must be an 8-digit hexadecimal address: {owner}"
        ));
    }
    let address = u64::from_str_radix(owner, 16)
        .map_err(|error| format!("invalid owner {owner}: {error}"))?;
    if address < ROM_BASE {
        return Err(format!("owner is below GBA ROM: {owner}"));
    }
    Ok(address)
}
fn canonical_main_owner_id(owner: &str) -> Result<String, String> {
    SourceOwner::parse(&format!("main:{owner}")).map(SourceOwner::id)
}
fn location_method_codes() -> BTreeMap<&'static str, &'static str> {
    [
        ("a", "global_anchor"),
        ("n", "neighbor_exact"),
        ("o", "resource_offset_exact"),
        ("s", "source"),
        ("t", "global_thumb_anchor"),
    ]
    .into_iter()
    .collect()
}
fn location_method_code(method: &str) -> Result<char, String> {
    match method {
        "global_anchor" => Ok('a'),
        "neighbor_exact" => Ok('n'),
        "resource_offset_exact" => Ok('o'),
        "source" => Ok('s'),
        "global_thumb_anchor" => Ok('t'),
        _ => Err(format!("unknown location method {method:?}")),
    }
}
fn resolve_object(
    owner: &str,
    explicit: Option<&Path>,
    object_dir: &Path,
) -> Result<PathBuf, String> {
    if let Some(path) = explicit {
        return path
            .is_file()
            .then(|| path.to_path_buf())
            .ok_or_else(|| format!("missing object: {}", path.display()));
    }
    let paths = [
        object_dir.join(format!("{owner}.o")),
        PathBuf::from(format!("out/gs1-en/claimed/obj/{owner}.o")),
    ];
    exact_objects(object_dir)
        .ok()
        .and_then(|mut objects| objects.remove(owner))
        .filter(|path| path.is_file())
        .or_else(|| paths.into_iter().find(|path| path.is_file()))
        .ok_or_else(|| {
            format!(
                "missing exact object for {}; run `make build-claimed`",
                owner
            )
        })
}
fn read_roms(directory: &Path) -> Result<EditionRoms, String> {
    let mut images = BTreeMap::new();
    for edition in EDITIONS {
        let path = directory.join(format!("gs1-{edition}.gba"));
        images.insert(edition, read_rom(&path)?);
    }
    Ok(EditionRoms { images })
}
fn registers() -> Result<&'static Registers, String> {
    static REGISTERS: OnceLock<Result<Registers, String>> = OnceLock::new();
    REGISTERS
        .get_or_init(|| {
            let root = compiler_core::routing::root();
            Ok(Registers {
                sources: SourcePaths::load(root)?,
                units: TranslationUnits::load(root)?,
            })
        })
        .as_ref()
        .map_err(Clone::clone)
}
fn translation_unit(owner: &str) -> Result<Option<&'static TranslationUnit>, String> {
    let source_owner = SourceOwner::parse(&format!("main:{owner}"))?;
    Ok(registers()?.units.unit_for_game_owner("gs1", source_owner))
}
/// Refuse to count a corpus probe whose source is unregistered or contains a
/// forbidden construct; the correspondence record must never rest on
/// nonordinary or unmapped C. Grouped unit members resolve through their
/// declared unit source, exactly as the production compile does.
pub(crate) fn compliance_error_for(owner: SourceOwner) -> Result<Option<String>, String> {
    let registers = registers()?;
    let source = match registers
        .units
        .unit_for_game_owner("gs1", owner)
        .map(|unit| compiler_core::routing::root().join(&unit.source))
    {
        Some(path) => path,
        None => match registers.sources.registered_source_path(owner) {
            Ok(path) => path,
            Err(_) => return Ok(Some("compliance: unregistered owner".into())),
        },
    };
    let text =
        fs::read_to_string(&source).map_err(|error| format!("{}: {error}", source.display()))?;
    let forbidden = no_asm_c::find_forbidden(&source.to_string_lossy(), &text);
    Ok(forbidden.first().map(|finding| {
        format!(
            "compliance: nonordinary C ({} at line {})",
            finding.token, finding.line
        )
    }))
}
fn complete_unit_symbols(unit: &TranslationUnit, mut defined: impl FnMut(&str) -> bool) -> bool {
    unit.symbols()
        .all(|(address, _, _)| defined(&format!("Func_{address:08x}")))
}
fn exact_objects(object_dir: &Path) -> Result<BTreeMap<String, PathBuf>, String> {
    let units = &registers()?.units;
    let grouped = units
        .units
        .iter()
        .filter(|unit| unit.overlay.is_none())
        .flat_map(|unit| &unit.owners)
        .map(|owner| format!("{:08x}", owner.address))
        .collect::<BTreeSet<_>>();
    let path = object_dir
        .parent()
        .ok_or("object directory has no manifest parent")?
        .join("manifest.json");
    let manifest: serde_json::Value = serde_json::from_str(
        &fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?,
    )
    .map_err(|error| error.to_string())?;
    let mut owners = BTreeMap::new();
    for region in manifest["regions"].as_array().into_iter().flatten() {
        let primary = region["symbol"]
            .as_str()
            .and_then(|symbol| symbol.strip_prefix("Func_"))
            .ok_or("exact region has no owner symbol")?;
        let object = region["translation_unit"]
            .as_str()
            .and_then(|id| units.unit(id))
            .filter(|unit| !unit.exact())
            .map(|unit| {
                object_dir
                    .join("tu")
                    .join(&unit.id)
                    .join(format!("{:08x}.o", unit.owners[0].address))
            })
            .unwrap_or_else(|| object_dir.join(format!("{primary}.o")));
        for owner in region["symbols"]
            .as_array()
            .into_iter()
            .flatten()
            .filter_map(|symbol| symbol.as_str()?.strip_prefix("Func_"))
        {
            if owner == primary || grouped.contains(owner) {
                owners.insert(owner.to_string(), object.clone());
            }
        }
    }
    if owners.is_empty() {
        return Err(format!(
            "{} contains no exact main-image owner objects; run `make build-claimed`",
            object_dir.display()
        ));
    }
    Ok(owners)
}
fn run_all(options: &Options, roms: &EditionRoms) -> Result<(), String> {
    let objects = exact_objects(&options.object_dir)?;
    let owner_names = objects.keys().cloned().collect::<Vec<_>>();
    let mut reports = BTreeMap::new();
    let mut failures = BTreeMap::new();
    let mut owner_symbol_bytes = 0;
    for (index, owner) in owner_names.iter().enumerate() {
        let object = &objects[owner];
        if let Some(error) = compliance_error_for(SourceOwner::parse(&format!("main:{owner}"))?)? {
            failures.insert(owner.clone(), error);
            continue;
        }
        if let Ok((size, _, _, _, _, _)) = relocation_mask(&object, owner) {
            owner_symbol_bytes += size;
        }
        match analyze_owner(owner, owner_address(owner)?, &object, roms, false, None) {
            Ok(report) => {
                reports.insert(owner.clone(), report);
            }
            Err(error) => {
                failures.insert(owner.clone(), error);
            }
        }
        if (index + 1) % 100 == 0 || index + 1 == owner_names.len() {
            eprintln!(
                "global matched={}/{} unresolved={}",
                reports.len(),
                index + 1,
                failures.len()
            );
        }
    }
    remove_order_conflicts(&mut reports, &mut failures, "global")?;
    let location_anchors = location_anchors(&reports)?;
    let retry_names = failures
        .iter()
        .filter(|(_, error)| !error.starts_with("compliance:"))
        .map(|(owner, _)| owner.clone())
        .collect::<Vec<_>>();
    for (index, owner) in retry_names.iter().enumerate() {
        let hints = nearest_location_hints(
            LocationKey {
                group: None,
                en_offset: rom_offset(owner_address(owner)?, usize::MAX)?,
            },
            &location_anchors,
        )?;
        let object = &objects[owner];
        match analyze_owner(
            owner,
            owner_address(owner)?,
            &object,
            roms,
            false,
            Some(&hints),
        ) {
            Ok(report) => {
                reports.insert(owner.clone(), report);
                failures.remove(owner);
            }
            Err(error) => {
                failures.insert(owner.clone(), error);
            }
        }
        if (index + 1) % 100 == 0 || index + 1 == retry_names.len() {
            eprintln!(
                "locality matched={}/{} unresolved={}",
                reports.len(),
                owner_names.len(),
                failures.len()
            );
        }
    }
    remove_order_conflicts(&mut reports, &mut failures, "locality")?;
    if let Some(path) = &options.edition_build {
        write_corpus_edition_build(
            path,
            &objects,
            &reports,
            owner_names.len(),
            owner_symbol_bytes,
            failures.len(),
            roms,
        )?;
    }
    let mut owners = Vec::new();
    let mut edition_totals = edition_totals();
    for (owner, report) in reports {
        let canonical_start = format!("0x{owner}");
        let mut row = CorpusOwner::new(canonical_main_owner_id(&owner)?, report.size, None);
        for edition in &report.editions {
            let location_method = if edition.edition == "en" {
                "source"
            } else if edition.anchor_matches == 0 {
                "neighbor_exact"
            } else {
                "global_anchor"
            };
            row.record_edition(
                &mut edition_totals,
                &edition.edition,
                edition.start.clone(),
                &canonical_start,
                location_method,
                edition.core_diff_bytes,
            )?;
        }
        owners.push(row);
    }
    let unresolved = failures
        .into_iter()
        .map(|(owner, error)| {
            Ok(UnresolvedOwner {
                en_owner: canonical_main_owner_id(&owner)?,
                size: None,
                error,
            })
        })
        .collect::<Result<Vec<_>, String>>()?;
    let report = corpus_report(
        "byte-exact C",
        None,
        owner_names.len(),
        owner_symbol_bytes,
        None,
        owners,
        unresolved,
        &edition_totals,
    );
    emit_corpus_report(options, &report, "corpus report", "correspondence")
}
fn run_all_overlays(options: &Options, roms: &EditionRoms) -> Result<(), String> {
    let owner_list = exact_overlay_owners()?;
    let (resource_tables, decoded) = decode_overlay_resources(roms)?;
    let mut matches = BTreeMap::new();
    let mut failures = BTreeMap::new();
    for (index, owner) in owner_list.iter().enumerate() {
        if let Some(error) = compliance_error_for(SourceOwner::parse(&owner.name)?)? {
            failures.insert(owner.name.clone(), error);
            continue;
        }
        match analyze_overlay_owner(owner, &decoded, None) {
            Ok(found) => {
                matches.insert(owner.name.clone(), found);
            }
            Err(error) => {
                failures.insert(owner.name.clone(), error);
            }
        }
        if (index + 1) % 200 == 0 || index + 1 == owner_list.len() {
            eprintln!(
                "overlay global matched={}/{} unresolved={}",
                matches.len(),
                index + 1,
                failures.len()
            );
        }
    }
    remove_order_conflicts(&mut matches, &mut failures, "global")?;
    let global_hints = location_anchors(&matches)?;
    let retry = failures
        .iter()
        .filter(|(_, error)| !error.starts_with("compliance:"))
        .map(|(owner, _)| owner.clone())
        .collect::<Vec<_>>();
    let by_name = owner_list
        .iter()
        .map(|owner| (owner.name.as_str(), owner))
        .collect::<BTreeMap<_, _>>();
    for (index, name) in retry.iter().enumerate() {
        let owner = by_name[name.as_str()];
        let hints = match nearest_location_hints(
            LocationKey {
                group: Some(owner.resource),
                en_offset: owner.en_offset,
            },
            &global_hints,
        ) {
            Ok(hints) => hints,
            Err(error) => {
                failures.insert(name.clone(), error);
                continue;
            }
        };
        match analyze_overlay_owner(owner, &decoded, Some(&hints)) {
            Ok(found) => {
                matches.insert(name.clone(), found);
                failures.remove(name);
            }
            Err(error) => {
                failures.insert(name.clone(), error);
            }
        }
        if (index + 1) % 200 == 0 || index + 1 == retry.len() {
            eprintln!(
                "overlay locality matched={}/{} unresolved={}",
                matches.len(),
                owner_list.len(),
                failures.len()
            );
        }
    }
    remove_order_conflicts(&mut matches, &mut failures, "locality")?;
    if let Some(path) = &options.edition_build {
        write_overlay_edition_build(path, &matches, &decoded)?;
    }
    let mut owners = Vec::new();
    let mut edition_totals = edition_totals();
    for found in matches.into_values() {
        let ja_start = found.starts["ja"];
        let ja = overlay_window(
            &decoded,
            "ja",
            found.owner.resource,
            ja_start,
            found.owner.size,
        )?;
        let canonical_start = format!("0x{:08x}", OVERLAY_BASE + found.owner.en_offset as u64);
        let mut row = CorpusOwner::new(
            found.owner.name.clone(),
            found.owner.size,
            Some(found.mask.iter().filter(|masked| !**masked).count()),
        );
        for edition in EDITIONS {
            let start = found.starts[edition];
            let other = overlay_window(
                &decoded,
                edition,
                found.owner.resource,
                start,
                found.owner.size,
            )?;
            let difference = core_diff_bytes(ja, other, &found.mask);
            let start = format!("0x{:08x}", OVERLAY_BASE + start as u64);
            row.record_edition(
                &mut edition_totals,
                edition,
                start,
                &canonical_start,
                found.methods[edition],
                difference,
            )?;
        }
        owners.push(row);
    }
    let unresolved = owner_list
        .iter()
        .filter_map(|owner| {
            failures.get(&owner.name).map(|error| UnresolvedOwner {
                en_owner: owner.name.clone(),
                size: Some(owner.size),
                error: error.clone(),
            })
        })
        .collect::<Vec<_>>();
    let report = corpus_report(
        "byte-exact overlay C",
        Some(OVERLAY_LAST - OVERLAY_FIRST + 1),
        owner_list.len(),
        owner_list.iter().map(|owner| owner.size).sum(),
        Some(resource_tables),
        owners,
        unresolved,
        &edition_totals,
    );
    emit_corpus_report(
        options,
        &report,
        "overlay corpus report",
        "overlay_correspondence",
    )
}
fn write_overlay_edition_build(
    path: &Path,
    matches: &BTreeMap<String, OverlayMatch>,
    decoded: &DecodedOverlays,
) -> Result<(), String> {
    let units = registers()?
        .units
        .units
        .iter()
        .filter(|unit| unit.game == "gs1" && unit.overlay.is_some() && unit.exact())
        .collect::<Vec<_>>();
    if units.is_empty() {
        return Err("no declared exact overlay reconstruction composition".into());
    }
    let mut reports = Vec::new();
    for unit in units {
        for edition in EDITIONS {
            let compiled = compile_declared_overlay_unit(unit, edition)?;
            for owner in &unit.owners {
                let source_owner = unit.source_owner(owner.address)?;
                let found = matches.get(&source_owner.id()).ok_or_else(|| {
                    format!("{}: owner lacks overlay correspondence", source_owner.id())
                })?;
                if found.owner.size != owner.extent {
                    return Err(format!(
                        "{}: owner extent differs from placeholder",
                        source_owner.id()
                    ));
                }
                let expected = overlay_window(
                    decoded,
                    edition,
                    found.owner.resource,
                    found.starts[edition],
                    owner.extent,
                )?;
                let offset =
                    usize::try_from(i64::from(owner.address) - compiled.address).map_err(|_| {
                        format!("{}: owner precedes compiled overlay", source_owner.id())
                    })?;
                let actual = offset
                    .checked_add(owner.extent)
                    .and_then(|end| compiled.data.get(offset..end))
                    .ok_or_else(|| {
                        format!("{}: compiled owner extent is absent", source_owner.id())
                    })?;
                if actual != expected {
                    return Err(format!(
                        "{}: {edition} overlay owner differs",
                        source_owner.id()
                    ));
                }
            }
        }
        reports.push(serde_json::json!({"id":unit.id,"overlay":unit.overlay,"source":unit.source,"c_compiles":EDITIONS.len(),"owners":unit.owners.len(),"editions":EDITIONS}));
    }
    write_json(
        path,
        &serde_json::json!({"format":1,"kind":"declared-overlay-reconstruction-composition-edition-builds","original_translation_units":"unknown","units":&reports}),
        "overlay edition build",
    )?;
    println!("overlay_edition_build={}", path.display());
    Ok(())
}
fn run_overlay_owner(options: &Options, roms: &EditionRoms, value: &str) -> Result<(), String> {
    let owner = parse_explicit_overlay_owner(value, options.span)?;
    let (_, decoded) = decode_overlay_resources(roms)?;
    let found = analyze_overlay_owner(&owner, &decoded, None)?;
    let en = overlay_window(&decoded, "en", owner.resource, owner.en_offset, owner.size)?;
    let editions = EDITIONS
        .into_iter()
        .map(|edition| {
            let start = found.starts[edition];
            let other = overlay_window(&decoded, edition, owner.resource, start, owner.size)?;
            let difference = core_diff_bytes(en, other, &found.mask);
            Ok(OverlayOwnerEdition {
                edition: edition.into(),
                start: format!("0x{:08x}", OVERLAY_BASE + start as u64),
                location_method: found.methods[edition].into(),
                core_diff_bytes: difference,
                core_identical: difference == 0,
            })
        })
        .collect::<Result<Vec<_>, String>>()?;
    let report = OverlayOwnerReport {
        schema_version: CORRESPONDENCE_SCHEMA_VERSION,
        game: "gs1",
        owner: owner.name,
        size: owner.size,
        core_bytes: found.mask.iter().filter(|masked| !**masked).count(),
        core_identical: editions.iter().all(|edition| edition.core_identical),
        editions,
    };
    if options.json {
        println!(
            "{}",
            serde_json::to_string_pretty(&report)
                .map_err(|error| format!("serialize overlay owner report: {error}"))?
        );
    } else {
        println!(
            "owner={} size={} core_bytes={} core_identical={}",
            report.owner,
            report.size,
            report.core_bytes,
            if report.core_identical { "yes" } else { "no" }
        );
        for edition in &report.editions {
            println!(
                "edition={} start={} location_method={} core_diff_bytes={} core_identical={}",
                edition.edition,
                edition.start,
                edition.location_method,
                edition.core_diff_bytes,
                if edition.core_identical { "yes" } else { "no" }
            );
        }
    }
    if report.core_identical {
        Ok(())
    } else {
        Err("overlay owner has regional core differences".into())
    }
}
fn exact_overlay_owners() -> Result<Vec<OverlayOwner>, String> {
    let mut assembly = BTreeMap::<usize, Vec<String>>::new();
    let mut owners = Vec::new();
    let source_paths = &registers()?.sources;
    for source in source_paths.all_sources()? {
        let SourceOwner::Overlay { resource, address } = source.owner else {
            continue;
        };
        let resource = resource as usize;
        if !(OVERLAY_FIRST..=OVERLAY_LAST).contains(&resource) {
            continue;
        }
        let absolute = address as u64;
        let en_offset = absolute
            .checked_sub(OVERLAY_BASE)
            .ok_or_else(|| format!("{}: address is below overlay base", source.owner.id()))?
            as usize;
        if !assembly.contains_key(&resource) {
            let source = format!("games/gs1/assets/code/resource_{resource:03x}_overlay.s");
            let text = fs::read_to_string(&source).map_err(|error| format!("{source}: {error}"))?;
            assembly.insert(resource, text.lines().map(str::to_string).collect());
        }
        let tag = format!("AlchemyC_{absolute:08x}:");
        let lines = &assembly[&resource];
        let start = lines
            .iter()
            .position(|line| line.trim() == tag)
            .ok_or_else(|| format!("{}: missing {tag} placeholder", source.owner.id()))?;
        let mut size = 0usize;
        for line in &lines[start + 1..] {
            let trimmed = line.trim();
            if let Some(value) = trimmed.strip_prefix(".space ") {
                size += parse_usize(value.trim())?;
            } else if trimmed.starts_with(".L_") && trimmed.ends_with(':') {
                continue;
            } else {
                break;
            }
        }
        if size == 0 {
            return Err(format!(
                "{}: exact placeholder has no span",
                source.owner.id()
            ));
        }
        owners.push(OverlayOwner {
            name: source.owner.id(),
            resource,
            en_offset,
            size,
        });
    }
    owners.sort_by_key(|owner| (owner.resource, owner.en_offset));
    if owners.is_empty() {
        return Err("games/gs1/src/ contains no exact overlay C owners".into());
    }
    Ok(owners)
}
fn parse_usize(value: &str) -> Result<usize, String> {
    value
        .strip_prefix("0x")
        .map_or_else(|| value.parse(), |hex| usize::from_str_radix(hex, 16))
        .map_err(|error| format!("invalid integer {value}: {error}"))
}
type DecodedOverlays = BTreeMap<&'static str, BTreeMap<usize, Vec<u8>>>;
fn decode_overlay_resources(
    roms: &EditionRoms,
) -> Result<(BTreeMap<String, String>, DecodedOverlays), String> {
    let mut tables = BTreeMap::new();
    let mut decoded = BTreeMap::new();
    for edition in EDITIONS {
        let rom = &roms.images[edition];
        let table =
            overlay_disasm::resource_table(rom).map_err(|error| format!("{edition}: {error}"))?;
        tables.insert(
            edition.to_string(),
            format!("0x{:08x}", ROM_BASE + table as u64),
        );
        let mut resources = BTreeMap::new();
        for resource in OVERLAY_FIRST..=OVERLAY_LAST {
            let bytes =
                overlay_disasm::decode_overlay(rom, table, &format!("resource_{resource:03x}"))
                    .map_err(|error| format!("{edition}: resource {resource:03x}: {error}"))?;
            resources.insert(resource, bytes);
        }
        decoded.insert(edition, resources);
    }
    Ok((tables, decoded))
}
fn overlay_window<'a>(
    decoded: &'a DecodedOverlays,
    edition: &str,
    resource: usize,
    start: usize,
    size: usize,
) -> Result<&'a [u8], String> {
    decoded[edition][&resource]
        .get(start..start + size)
        .ok_or_else(|| format!("{edition}: resource {resource:03x} owner extends past container"))
}
fn overlay_mask(owner: &[u8], en_offset: usize) -> Vec<bool> {
    compiler_core::thumb::relocation_info(owner, OVERLAY_BASE + en_offset as u64).0
}
fn analyze_overlay_owner(
    owner: &OverlayOwner,
    decoded: &DecodedOverlays,
    hints: Option<&BTreeMap<&str, usize>>,
) -> Result<OverlayMatch, String> {
    let en = overlay_window(decoded, "en", owner.resource, owner.en_offset, owner.size)?;
    let mask = overlay_mask(en, owner.en_offset);
    let core_bytes = mask.iter().filter(|masked| !**masked).count();
    if core_bytes < 4 {
        let raw_identical = EDITIONS.into_iter().all(|edition| {
            overlay_window(
                decoded,
                edition,
                owner.resource,
                owner.en_offset,
                owner.size,
            ) == Ok(en)
        });
        if raw_identical {
            return Ok(OverlayMatch {
                owner: OverlayOwner {
                    name: owner.name.clone(),
                    resource: owner.resource,
                    en_offset: owner.en_offset,
                    size: owner.size,
                },
                mask,
                starts: EDITIONS
                    .into_iter()
                    .map(|edition| (edition, owner.en_offset))
                    .collect(),
                methods: EDITIONS
                    .into_iter()
                    .map(|edition| {
                        (
                            edition,
                            if edition == "en" {
                                "source"
                            } else {
                                "resource_offset_exact"
                            },
                        )
                    })
                    .collect(),
            });
        }
        return Err(format!(
            "only {core_bytes} bytes remain after masking Thumb calls and literal fields"
        ));
    }
    let owner_anchors = anchors(en, &mask);
    let mut starts = BTreeMap::new();
    let mut methods = BTreeMap::new();
    starts.insert("en", owner.en_offset);
    methods.insert("en", "source");
    for edition in EDITIONS.into_iter().filter(|edition| *edition != "en") {
        let container = &decoded[edition][&owner.resource];
        let located = if owner_anchors.is_empty() {
            Err("no relocation-free anchor found in decoded resource".into())
        } else {
            locate(en, &mask, &owner_anchors, container)
        };
        let (start, method) = match located {
            Ok((start, _)) => (start, "global_thumb_anchor"),
            Err(_) if container.get(owner.en_offset..owner.en_offset + owner.size) == Some(en) => {
                (owner.en_offset, "resource_offset_exact")
            }
            Err(global_error) => {
                let predicted = hints
                    .and_then(|values| values.get(edition).copied())
                    .ok_or(global_error)
                    .map_err(|error| format!("{edition}: {error}"))?;
                let (start, _) =
                    locate_near_exact(en, &mask, container, predicted, 0x1000, OVERLAY_BASE)
                        .map_err(|error| format!("{edition}: {error}"))?;
                (start, "neighbor_exact")
            }
        };
        starts.insert(edition, start);
        methods.insert(edition, method);
    }
    Ok(OverlayMatch {
        owner: OverlayOwner {
            name: owner.name.clone(),
            resource: owner.resource,
            en_offset: owner.en_offset,
            size: owner.size,
        },
        mask,
        starts,
        methods,
    })
}
impl Located for Report {
    fn location(&self) -> Result<LocationAnchor, String> {
        let starts = EDITIONS
            .into_iter()
            .map(|edition| Ok((edition, report_start(self, edition)?)))
            .collect::<Result<_, String>>()?;
        Ok(LocationAnchor {
            key: LocationKey {
                group: None,
                en_offset: rom_offset(owner_address(&self.owner)?, usize::MAX)?,
            },
            starts,
        })
    }
}
impl Located for OverlayMatch {
    fn location(&self) -> Result<LocationAnchor, String> {
        Ok(LocationAnchor {
            key: LocationKey {
                group: Some(self.owner.resource),
                en_offset: self.owner.en_offset,
            },
            starts: self.starts.clone(),
        })
    }
}
fn location_anchors<T: Located>(
    entries: &BTreeMap<String, T>,
) -> Result<Vec<LocationAnchor>, String> {
    entries.values().map(Located::location).collect()
}
fn remove_order_conflicts<T: Located>(
    entries: &mut BTreeMap<String, T>,
    failures: &mut BTreeMap<String, String>,
    phase: &str,
) -> Result<(), String> {
    loop {
        let locations = entries
            .iter()
            .map(|(name, entry)| Ok((name.clone(), entry.location()?)))
            .collect::<Result<BTreeMap<_, _>, String>>()?;
        let mut conflict = None;
        for edition in EDITIONS.into_iter().filter(|edition| *edition != "en") {
            let mut previous = BTreeMap::<Option<usize>, (&str, usize)>::new();
            for (name, location) in &locations {
                if let Some((left, left_start)) =
                    previous.insert(location.key.group, (name, location.starts[edition]))
                {
                    if location.starts[edition] <= left_start {
                        conflict = Some((edition, left.to_string(), name.clone()));
                        break;
                    }
                }
            }
            if conflict.is_some() {
                break;
            }
        }
        let Some((edition, left, right)) = conflict else {
            return Ok(());
        };
        let left_error = location_prediction_error(&left, edition, &locations, [&left, &right])?;
        let right_error = location_prediction_error(&right, edition, &locations, [&left, &right])?;
        let rejected = if right_error >= left_error {
            right
        } else {
            left
        };
        entries.remove(&rejected);
        failures.insert(
            rejected,
            format!("{edition}: {phase} counterpart conflicts with neighboring proved owner order"),
        );
    }
}
fn location_prediction_error(
    owner: &str,
    edition: &str,
    locations: &BTreeMap<String, LocationAnchor>,
    excluded: [&str; 2],
) -> Result<usize, String> {
    let location = &locations[owner];
    let neighbor = locations
        .iter()
        .filter(|(name, candidate)| {
            candidate.key.group == location.key.group && !excluded.contains(&name.as_str())
        })
        .min_by_key(|(_, candidate)| candidate.key.en_offset.abs_diff(location.key.en_offset));
    let Some((_, neighbor)) = neighbor else {
        return location.key.group.map_or(
            Err("not enough correspondence anchors to resolve order conflict".into()),
            |_| Ok(usize::MAX),
        );
    };
    let predicted = neighbor.starts[edition] as i128 + location.key.en_offset as i128
        - neighbor.key.en_offset as i128;
    Ok(location.starts[edition].abs_diff(predicted.max(0) as usize))
}
fn report_start(report: &Report, edition: &str) -> Result<usize, String> {
    let entry = report
        .editions
        .iter()
        .find(|entry| entry.edition == edition)
        .ok_or_else(|| format!("{} lacks {edition} location", report.owner))?;
    parse_rom_address(&entry.start)
}
fn nearest_location_hints(
    owner: LocationKey,
    anchors: &[LocationAnchor],
) -> Result<BTreeMap<&'static str, usize>, String> {
    let anchor = anchors
        .iter()
        .filter(|anchor| anchor.key.group == owner.group)
        .min_by_key(|anchor| anchor.key.en_offset.abs_diff(owner.en_offset))
        .ok_or_else(|| {
            owner.group.map_or_else(
                || "no global correspondence anchors are available".to_string(),
                |resource| format!("resource_{resource:03x}: no global anchors"),
            )
        })?;
    let mut hints = BTreeMap::new();
    for edition in EDITIONS.into_iter().filter(|edition| *edition != "en") {
        let start =
            anchor.starts[edition] as i128 + owner.en_offset as i128 - anchor.key.en_offset as i128;
        if (0..=usize::MAX as i128).contains(&start) {
            hints.insert(edition, start as usize);
        }
    }
    Ok(hints)
}
fn parse_rom_address(value: &str) -> Result<usize, String> {
    let address = u64::from_str_radix(value.trim_start_matches("0x"), 16)
        .map_err(|error| format!("invalid ROM address {value}: {error}"))?;
    rom_offset(address, usize::MAX)
}
fn relocation_mask(
    path: &Path,
    owner: &str,
) -> Result<(usize, u64, Vec<u8>, Vec<bool>, Vec<RelocationSite>, bool), String> {
    let config = DiffObjConfig {
        arm_arch_version: ArmArchVersion::V4t,
        ..Default::default()
    };
    let object = obj::read::read(path, &config, DiffSide::Base)
        .map_err(|error| format!("{}: {error}", path.display()))?;
    let complete = translation_unit(owner)?.is_none_or(|unit| {
        complete_unit_symbols(unit, |name| {
            object
                .symbol_by_name(name)
                .is_some_and(|index| object.symbols[index].section.is_some())
        })
    });
    let legacy_symbol = format!("Func_{owner}");
    let source_owner = SourceOwner::parse(&format!("main:{owner}"))?;
    let source_paths = &registers()?.sources;
    let registered_symbol = source_paths.registered_name(source_owner);
    let (symbol_name, symbol_index) = registered_symbol
        .and_then(|name| object.symbol_by_name(name).map(|index| (name, index)))
        .or_else(|| {
            object
                .symbol_by_name(&legacy_symbol)
                .map(|index| (legacy_symbol.as_str(), index))
        })
        .ok_or_else(|| {
            format!(
                "{}: missing owner symbol {}",
                path.display(),
                registered_symbol.unwrap_or(&legacy_symbol)
            )
        })?;
    let symbol = &object.symbols[symbol_index];
    let section = &object.sections[symbol.section.ok_or("owner symbol has no section")?];
    let section_offset = symbol
        .address
        .checked_sub(section.address)
        .ok_or("owner symbol precedes its section")?;
    let section_offset =
        usize::try_from(section_offset).map_err(|_| "owner section offset is too large")?;
    let size = if symbol.size == 0 {
        section
            .data
            .len()
            .checked_sub(section_offset)
            .ok_or("owner symbol extends past its section")?
    } else {
        usize::try_from(symbol.size).map_err(|_| "owner is too large")?
    };
    if size == 0 {
        return Err(format!("{symbol_name} has zero size"));
    }
    let bytes = section
        .data
        .get(section_offset..section_offset + size)
        .ok_or("owner symbol extends past its section")?
        .to_vec();
    let mut mask = vec![false; size];
    let mut sites = Vec::new();
    for relocation in &section.relocations {
        let Some(offset) = relocation.address.checked_sub(symbol.address) else {
            continue;
        };
        if offset >= symbol.size {
            continue;
        }
        let relocation_size = object.arch.data_reloc_size(relocation.flags);
        let start = usize::try_from(offset).map_err(|_| "relocation offset is too large")?;
        let end = start
            .checked_add(relocation_size)
            .ok_or("relocation range overflow")?
            .min(mask.len());
        mask[start..end].fill(true);
        let kind = object
            .arch
            .reloc_name(relocation.flags)
            .map(str::to_string)
            .unwrap_or_else(|| format!("{:?}", relocation.flags));
        let target = object.symbols.get(relocation.target_symbol);
        let symbol = target
            .map(|symbol| symbol.name.clone())
            .unwrap_or_else(|| "<missing>".into());
        sites.push(RelocationSite {
            offset: start,
            size: end - start,
            kind,
            symbol,
            addend: relocation.addend,
            external: target.is_some_and(|symbol| symbol.section.is_none()),
        });
    }
    Ok((size, symbol.address, bytes, mask, sites, complete))
}
fn literal_sites(
    path: &Path,
    symbol_address: u64,
    size: usize,
) -> Result<Vec<LiteralSite>, String> {
    let output = Command::new("arm-none-eabi-objdump")
        .args(["-dr"])
        .arg(path)
        .output()
        .map_err(|error| format!("arm-none-eabi-objdump: {error}"))?;
    if !output.status.success() {
        return Err(format!(
            "arm-none-eabi-objdump failed: {}",
            String::from_utf8_lossy(&output.stderr).trim()
        ));
    }
    Ok(parse_literal_sites(
        &String::from_utf8_lossy(&output.stdout),
        symbol_address,
        size,
    ))
}
fn parse_literal_sites(output: &str, symbol_address: u64, size: usize) -> Vec<LiteralSite> {
    let mut sites = Vec::new();
    for line in output.lines() {
        let Some((address, text)) = line.trim_start().split_once(':') else {
            continue;
        };
        let field_size = if text.contains(".word") {
            4
        } else if text.contains(".short") {
            2
        } else {
            continue;
        };
        let Ok(address) = u64::from_str_radix(address.trim(), 16) else {
            continue;
        };
        let Some(offset) = address.checked_sub(symbol_address) else {
            continue;
        };
        let Ok(offset) = usize::try_from(offset) else {
            continue;
        };
        if offset + field_size <= size {
            sites.push(LiteralSite {
                offset,
                size: field_size,
            });
        }
    }
    sites.sort_by_key(|site| site.offset);
    sites.dedup_by_key(|site| site.offset);
    sites
}
fn read_rom(path: &Path) -> Result<Vec<u8>, String> {
    fs::read(path).map_err(|error| format!("{}: {error}", path.display()))
}
fn rom_offset(address: u64, rom_len: usize) -> Result<usize, String> {
    let offset = address
        .checked_sub(ROM_BASE)
        .ok_or("address is below ROM")?;
    let offset = usize::try_from(offset).map_err(|_| "address is too large")?;
    (offset < rom_len)
        .then_some(offset)
        .ok_or_else(|| format!("address 0x{address:08x} is outside ROM"))
}
#[derive(Clone, Copy)]
struct Anchor {
    offset: usize,
    size: usize,
}
fn anchors(owner: &[u8], mask: &[bool]) -> Vec<Anchor> {
    let mut runs = Vec::new();
    let mut start = None;
    for (index, relocated) in mask.iter().copied().chain([true]).enumerate() {
        match (start, relocated) {
            (None, false) => start = Some(index),
            (Some(run_start), true) => {
                let size = index - run_start;
                if size >= 16 {
                    runs.push((size, run_start));
                }
                start = None;
            }
            _ => {}
        }
    }
    runs.sort_by(|left, right| right.cmp(left));
    runs.into_iter()
        .take(16)
        .map(|(size, offset)| Anchor {
            offset,
            size: size.min(32).min(owner.len() - offset),
        })
        .collect()
}
fn locate(
    owner: &[u8],
    mask: &[bool],
    anchors: &[Anchor],
    rom: &[u8],
) -> Result<(usize, usize), String> {
    let mut support = BTreeMap::<usize, usize>::new();
    for anchor in anchors {
        let needle = &owner[anchor.offset..anchor.offset + anchor.size];
        for hit in find_all(rom, needle) {
            let Some(start) = hit.checked_sub(anchor.offset) else {
                continue;
            };
            if start % 2 != 0 || start + owner.len() > rom.len() {
                continue;
            }
            *support.entry(start).or_default() += 1;
        }
        let exact = support
            .keys()
            .copied()
            .filter(|start| core_diff_bytes(owner, &rom[*start..*start + owner.len()], mask) == 0)
            .collect::<Vec<_>>();
        if let [start] = exact.as_slice() {
            return Ok((*start, support[start]));
        }
    }
    let mut scored = support
        .into_iter()
        .map(|(start, anchor_matches)| {
            (
                core_diff_bytes(owner, &rom[start..start + owner.len()], mask),
                usize::MAX - anchor_matches,
                start,
                anchor_matches,
            )
        })
        .collect::<Vec<_>>();
    scored.sort_unstable();
    let Some(&(best_diff, _, best_start, best_support)) = scored.first() else {
        return Err("no relocation-free anchor found in ROM".into());
    };
    if scored
        .get(1)
        .is_some_and(|next| next.0 == best_diff && next.1 == scored[0].1)
    {
        return Err(format!(
            "ambiguous counterpart with {best_diff} core byte differences"
        ));
    }
    Ok((best_start, best_support))
}
fn locate_near_exact(
    owner: &[u8],
    mask: &[bool],
    rom: &[u8],
    predicted: usize,
    radius: usize,
    address_base: u64,
) -> Result<(usize, usize), String> {
    if owner.len() > rom.len() {
        return Err("owner is larger than ROM".into());
    }
    let first = predicted.saturating_sub(radius) & !1;
    let last = predicted
        .saturating_add(radius)
        .min(rom.len().saturating_sub(owner.len()))
        & !1;
    let mut exact = (first..=last)
        .step_by(2)
        .filter(|start| core_diff_bytes(owner, &rom[*start..*start + owner.len()], mask) == 0)
        .map(|start| (start.abs_diff(predicted), start))
        .collect::<Vec<_>>();
    exact.sort_unstable();
    let Some(&(distance, start)) = exact.first() else {
        return Err(format!(
            "no exact core within {radius:#x} bytes of predicted 0x{:08x}",
            address_base + predicted as u64
        ));
    };
    if exact.get(1).is_some_and(|next| next.0 == distance) {
        return Err(format!(
            "ambiguous exact core near predicted 0x{:08x}",
            address_base + predicted as u64
        ));
    }
    Ok((start, 0))
}
fn find_all(haystack: &[u8], needle: &[u8]) -> Vec<usize> {
    if needle.is_empty() || needle.len() > haystack.len() {
        return Vec::new();
    }
    let first = needle[0];
    haystack
        .windows(needle.len())
        .enumerate()
        .filter_map(|(index, window)| (window[0] == first && window == needle).then_some(index))
        .collect()
}
fn edition_report(
    edition: &str,
    start: usize,
    other: &[u8],
    context: &ComparisonContext<'_>,
    anchor_matches: usize,
) -> EditionReport {
    let base = context.base;
    let raw_diff_bytes = base
        .iter()
        .zip(other)
        .filter(|(left, right)| left != right)
        .count();
    let raw_diff_halfwords = base
        .chunks_exact(2)
        .zip(other.chunks_exact(2))
        .filter(|(left, right)| left != right)
        .count();
    let relocation_diff_bytes = base
        .iter()
        .zip(other)
        .zip(context.relocation_mask)
        .filter(|((left, right), relocated)| **relocated && left != right)
        .count();
    let literal_diff_bytes = context
        .literals
        .iter()
        .map(|site| {
            base[site.offset..site.offset + site.size]
                .iter()
                .zip(&other[site.offset..site.offset + site.size])
                .filter(|(left, right)| left != right)
                .count()
        })
        .sum();
    let core_diff_bytes = core_diff_bytes(base, other, context.mask);
    let changed = context
        .relocations
        .iter()
        .filter(|site| {
            base[site.offset..site.offset + site.size]
                != other[site.offset..site.offset + site.size]
        })
        .collect::<Vec<_>>();
    EditionReport {
        edition: edition.into(),
        start: format!("0x{:08x}", ROM_BASE + start as u64),
        shift: start as i64 - context.base_start as i64,
        raw_diff_bytes,
        raw_diff_halfwords,
        relocation_diff_bytes,
        literal_diff_bytes,
        relocation_fields_changed: changed.len(),
        literal_fields_changed: context
            .literals
            .iter()
            .filter(|site| {
                base[site.offset..site.offset + site.size]
                    != other[site.offset..site.offset + site.size]
            })
            .count(),
        call_fields_changed: changed.iter().filter(|site| is_thumb_call(site)).count(),
        absolute_fields_changed: changed
            .iter()
            .filter(|site| site.kind == "R_ARM_ABS32")
            .count(),
        core_diff_bytes,
        anchor_matches,
        core_identical: core_diff_bytes == 0,
    }
}
fn is_thumb_call(site: &RelocationSite) -> bool {
    site.kind == "R_ARM_THM_PC22" || site.kind == "R_ARM_THM_XPC22"
}
fn call_targets(
    relocations: &[RelocationSite],
    found: &BTreeMap<&str, FoundEdition>,
) -> Result<Vec<CallTarget>, String> {
    let mut groups = BTreeMap::<&str, Vec<usize>>::new();
    for site in relocations.iter().filter(|site| is_thumb_call(site)) {
        groups.entry(&site.symbol).or_default().push(site.offset);
    }
    groups
        .into_iter()
        .map(|(symbol, offsets)| {
            let mut editions = Vec::with_capacity(EDITIONS.len());
            for edition in EDITIONS {
                let entry = &found[edition];
                let mut targets = offsets
                    .iter()
                    .map(|offset| {
                        thumb_bl_target(&entry.bytes, entry.start, *offset)
                            .map_err(|error| format!("{edition}: {error}"))
                    })
                    .collect::<Result<Vec<_>, _>>()?;
                targets.sort_unstable();
                targets.dedup();
                if targets.len() != 1 {
                    return Err(format!(
                        "{edition}: {symbol} resolves to {} call targets",
                        targets.len()
                    ));
                }
                editions.push(TargetAddress {
                    edition: edition.into(),
                    address: format!("0x{:08x}", targets[0]),
                });
            }
            Ok(CallTarget {
                en_symbol: symbol.into(),
                sites: offsets.len(),
                editions,
            })
        })
        .collect()
}
fn thumb_bl_target(owner: &[u8], start: usize, offset: usize) -> Result<u64, String> {
    let bytes = owner
        .get(offset..offset + 4)
        .ok_or_else(|| format!("call at 0x{offset:x} extends past owner"))?;
    let high = u16::from_le_bytes([bytes[0], bytes[1]]);
    let low = u16::from_le_bytes([bytes[2], bytes[3]]);
    if high & 0xf800 != 0xf000 || low & 0xf800 != 0xf800 {
        return Err(format!("relocation at 0x{offset:x} is not a Thumb BL"));
    }
    let mut displacement = (((high & 0x07ff) as i64) << 12) | (((low & 0x07ff) as i64) << 1);
    if displacement & (1 << 22) != 0 {
        displacement -= 1 << 23;
    }
    let pc = ROM_BASE as i64 + start as i64 + offset as i64 + 4;
    u64::try_from(pc + displacement).map_err(|_| format!("call at 0x{offset:x} is below ROM"))
}
fn core_diff_bytes(left: &[u8], right: &[u8], mask: &[bool]) -> usize {
    left.iter()
        .zip(right)
        .zip(mask)
        .filter(|((left, right), relocated)| !**relocated && left != right)
        .count()
}
fn print_report(report: &Report, calls: bool) {
    println!(
        "owner={} owner_edition={} base_edition={} relocation_source_edition={} size={} relocations={} relocation_bytes={} literal_fields={} literal_bytes={} core_bytes={}",
        report.owner, report.owner_edition, report.base_edition, report.relocation_source_edition, report.size, report.relocations, report.relocation_bytes, report.literal_fields, report.literal_bytes, report.core_bytes
    );
    for edition in &report.editions {
        println!(
            "edition={} start={} shift={:+#x} raw_diff_bytes={} raw_diff_halfwords={} relocation_diff_bytes={} literal_diff_bytes={} relocation_fields_changed={} literal_fields_changed={} call_fields_changed={} absolute_fields_changed={} core_diff_bytes={} core_identical={}",
            edition.edition,
            edition.start,
            edition.shift,
            edition.raw_diff_bytes,
            edition.raw_diff_halfwords,
            edition.relocation_diff_bytes,
            edition.literal_diff_bytes,
            edition.relocation_fields_changed,
            edition.literal_fields_changed,
            edition.call_fields_changed,
            edition.absolute_fields_changed,
            edition.core_diff_bytes,
            if edition.core_identical { "yes" } else { "no" },
        );
    }
    println!(
        "callee_correspondences={} core_identical={}",
        report.call_targets.len(),
        if report.core_identical { "yes" } else { "no" }
    );
    if calls {
        for call in &report.call_targets {
            let addresses = call
                .editions
                .iter()
                .map(|target| format!("{}={}", target.edition, target.address))
                .collect::<Vec<_>>()
                .join(" ");
            println!(
                "callee={} sites={} {}",
                call.en_symbol, call.sites, addresses
            );
        }
    }
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn artifact_substitution_and_callee_inference_fail_closed() {
        let unit = translation_unit("08003b70").unwrap().unwrap();
        assert!(!complete_unit_symbols(unit, |name| name == "Func_08003b70"));
        assert!(complete_unit_symbols(unit, |_| true));
        let locations = EditionLocations::from([(
            "Func_0801e940".into(),
            BTreeMap::from([("fr".into(), 0x0801_d8ac)]),
        )]);
        assert!(
            proved_symbol_location(Some(&locations), "Func_0801e940", "fr", 0x0801_d81c).is_err()
        );
        assert_eq!(
            proved_symbol_location(Some(&locations), "Func_0801e940", "fr", 0x0801_d8ac),
            Ok(0x0801_d8ac)
        );
        assert_eq!(
            proved_symbol_location(Some(&locations), "Func_08000000", "fr", 7),
            Ok(7)
        );
        assert!(proved_symbol_location(None, "Func_08000000", "fr", 7).is_err());
    }
    #[test]
    fn builds_anchors_only_from_core_bytes() {
        let owner = vec![0x55; 80];
        let mut mask = vec![false; owner.len()];
        mask[20..24].fill(true);
        let found = anchors(&owner, &mask);
        assert_eq!(found[0].offset, 24);
        assert_eq!(found[0].size, 32);
        assert_eq!(found[1].offset, 0);
        assert_eq!(found[1].size, 20);
    }
    #[test]
    fn locates_owner_while_ignoring_relocations() {
        let owner = (0..96).map(|value| value as u8).collect::<Vec<_>>();
        let mut mask = vec![false; owner.len()];
        mask[32..36].fill(true);
        let mut counterpart = owner.clone();
        counterpart[32..36].copy_from_slice(&[9, 8, 7, 6]);
        let mut rom = vec![0xff; 256];
        rom[100..196].copy_from_slice(&counterpart);
        let found = anchors(&owner, &mask);
        assert_eq!(locate(&owner, &mask, &found, &rom), Ok((100, 1)));
        assert_eq!(core_diff_bytes(&owner, &counterpart, &mask), 0);
        counterpart[40] ^= 1;
        assert_eq!(core_diff_bytes(&owner, &counterpart, &mask), 1);
    }
    #[test]
    fn locates_short_exact_core_near_proved_neighbor() {
        let owner = (0..12).collect::<Vec<_>>();
        let mask = vec![false; owner.len()];
        let mut rom = vec![0xff; 128];
        rom[24..36].copy_from_slice(&owner);
        rom[88..100].copy_from_slice(&owner);
        assert_eq!(
            locate_near_exact(&owner, &mask, &rom, 80, 32, ROM_BASE),
            Ok((88, 0))
        );
    }
    #[test]
    fn rejects_equidistant_short_core_matches() {
        let owner = (0..12).collect::<Vec<_>>();
        let mask = vec![false; owner.len()];
        let mut rom = vec![0xff; 128];
        rom[24..36].copy_from_slice(&owner);
        rom[56..68].copy_from_slice(&owner);
        assert!(locate_near_exact(&owner, &mask, &rom, 40, 32, ROM_BASE)
            .unwrap_err()
            .contains("ambiguous"));
    }
    #[test]
    fn masks_overlay_calls_and_reached_literal_words() {
        let mut owner = vec![0; 16];
        owner[0..2].copy_from_slice(&0x4801u16.to_le_bytes());
        owner[4..6].copy_from_slice(&0xf000u16.to_le_bytes());
        owner[6..8].copy_from_slice(&0xf800u16.to_le_bytes());
        let mask = overlay_mask(&owner, 0);
        assert_eq!(&mask[0..4], &[false; 4]);
        assert_eq!(&mask[4..12], &[true; 8]);
        assert_eq!(&mask[12..16], &[false; 4]);
    }
    #[test]
    fn parses_explicit_unresolved_overlay_owner_with_span() {
        let owner = parse_explicit_overlay_owner("resource_392:02000bcc", Some(64))
            .expect("explicit overlay owner");
        assert_eq!(owner.resource, 0x392);
        assert_eq!(owner.en_offset, 0xbcc);
        assert_eq!(owner.size, 64);
        assert_eq!(owner.name, "resource_392:02000bcc");
        assert!(matches!(
            parse_explicit_overlay_owner("resource_392:02000bcc", None),
            Err(error) if error.contains("require --span")
        ));
    }
    #[test]
    fn locates_explicit_overlay_owner_with_masked_regional_literal() {
        let mut bytes = Vec::new();
        bytes.extend_from_slice(&0x4806u16.to_le_bytes());
        for immediate in 1..14u16 {
            bytes.extend_from_slice(&(0x2000 | immediate).to_le_bytes());
        }
        bytes.extend_from_slice(&0x0300_1234u32.to_le_bytes());
        assert_eq!(bytes.len(), 32);
        let resource = 0x392;
        let mut decoded = DecodedOverlays::new();
        for edition in EDITIONS {
            let start = if edition == "en" { 16 } else { 24 };
            let mut image = vec![0xff; 96];
            image[start..start + bytes.len()].copy_from_slice(&bytes);
            if edition != "en" {
                image[start + 28..start + 32].copy_from_slice(&(0x0300_5678u32).to_le_bytes());
            }
            decoded.insert(edition, [(resource, image)].into_iter().collect());
        }
        let owner = OverlayOwner {
            name: "resource_392:02000010".into(),
            resource,
            en_offset: 16,
            size: bytes.len(),
        };
        let found =
            analyze_overlay_owner(&owner, &decoded, None).expect("masked owner correspondence");
        assert_eq!(found.starts["en"], 16);
        for edition in EDITIONS.into_iter().filter(|edition| *edition != "en") {
            assert_eq!(found.starts[edition], 24);
        }
        assert_eq!(found.mask.iter().filter(|masked| !**masked).count(), 28);
    }
    #[test]
    fn correspondence_rows_serialize_only_canonical_owner_identity() {
        let owner = CorpusOwner {
            en_owner: SourceOwner::Main(0x0800_2ee4).id(),
            size: 4,
            core_bytes: None,
            start_overrides: BTreeMap::new(),
            location_methods: "asaaaa".into(),
            core_diff_bytes_from_ja: BTreeMap::new(),
        };
        let report = corpus_report(
            "test",
            None,
            1,
            4,
            None,
            vec![owner],
            Vec::new(),
            &edition_totals(),
        );
        assert_eq!((report.matched_owners, report.shared_core_bytes), (1, 4));
        let compact = serde_json::to_value(&report.owners[0]).expect("serialize compact owner");
        assert_eq!(compact["en_owner"], "main:08002ee4");
        for omitted in [
            "source",
            "resource",
            "status",
            "starts",
            "start_overrides",
            "core_diff_bytes_from_ja",
        ] {
            assert!(compact.get(omitted).is_none(), "unexpected {omitted}");
        }
        assert_eq!(compact["location_methods"], "asaaaa");
        let overlay_owner = SourceOwner::Overlay {
            resource: 0x36f,
            address: 0x0200_0030,
        }
        .id();
        let unresolved = serde_json::to_value(UnresolvedOwner {
            en_owner: overlay_owner,
            size: Some(4),
            error: "unresolved".into(),
        })
        .unwrap();
        assert_eq!(unresolved["en_owner"], "resource_36f:02000030");
        assert!(unresolved.get("resource").is_none());
        assert_eq!(CORRESPONDENCE_SCHEMA_VERSION, 4);
        assert_eq!(CORPUS_EDITION_BUILD_SCHEMA_VERSION, 3);
        assert_eq!(location_method_codes()["a"], "global_anchor");
    }
    #[test]
    fn corpus_edition_build_rows_encode_only_nondefault_evidence() {
        let entries = EDITIONS
            .into_iter()
            .map(|edition| EditionBuildEntry {
                edition: edition.into(),
                start: if edition == "en" {
                    "0x08002ee4".into()
                } else {
                    "0x08002ef0".into()
                },
                size: if edition == "ja" { 8 } else { 4 },
                external_symbols: BTreeMap::new(),
                differing_bytes: (edition != "de").then_some(if edition == "es" { 2 } else { 0 }),
                byte_exact: !matches!(edition, "de" | "es"),
                error: (edition == "de").then(|| "link failed".into()),
            })
            .collect();
        let row = compact_corpus_edition_build_owner(EditionBuildReport {
            schema_version: 1,
            game: "gs1",
            source_edition: "en",
            source: "games/gs1/src/example.c".into(),
            object: "out/example.o".into(),
            owner_symbol: "Func_08002ee4".into(),
            size: 4,
            edition_variant: false,
            all_exact: false,
            editions: entries,
        })
        .expect("compact corpus row");
        let value = serde_json::to_value(row).expect("serialize compact corpus row");
        assert_eq!(value["en_owner"], "main:08002ee4");
        assert_eq!(
            value["start_overrides"].as_object().map(|map| map.len()),
            Some(5)
        );
        assert!(value["start_overrides"].get("en").is_none());
        assert_eq!(value["size_overrides"]["ja"], 8);
        assert_eq!(value["differing_bytes"]["es"], 2);
        assert_eq!(value["errors"]["de"], "link failed");
        for removed in ["source", "editions", "starts", "all_exact", "byte_exact"] {
            assert!(value.get(removed).is_none(), "unexpected {removed}");
        }
    }
    #[test]
    fn decodes_thumb_bl_target() {
        let bytes = [0x48, 0xf7, 0xfd, 0xfe];
        assert_eq!(thumb_bl_target(&bytes, 0x0bbb3a, 0), Ok(0x08004938));
    }
    #[test]
    fn reads_literal_fields_from_objdump() {
        let output = "  1008:\t12345678 \t.word\t0x12345678\n  100c:\t0000 \t.short\t0x0000\n";
        let sites = parse_literal_sites(output, 0x1000, 16);
        assert_eq!(sites.len(), 2);
        assert_eq!((sites[0].offset, sites[0].size), (8, 4));
        assert_eq!((sites[1].offset, sites[1].size), (12, 2));
    }
}
