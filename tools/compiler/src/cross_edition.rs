use objdiff_core::{
    diff::{ArmArchVersion, DiffObjConfig, DiffSide},
    obj,
};
use serde::Serialize;
use std::{
    collections::BTreeMap,
    fs,
    path::{Path, PathBuf},
    process::Command,
};

const ROM_BASE: u64 = 0x0800_0000;
const EDITIONS: [&str; 6] = ["ja", "en", "de", "es", "fr", "it"];
const OVERLAY_BASE: u64 = 0x0200_0000;
const OVERLAY_FIRST: usize = 0x36f;
const OVERLAY_LAST: usize = 0x3ce;
const USAGE: &str = "usage: compiler cross-edition ([--calls] [--json] [--rom-dir DIR] [--object FILE] <8-digit-owner> | --all [--rom-dir DIR] [--object-dir DIR] [--write FILE] | --all-overlays [--rom-dir DIR] [--write FILE])";

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
    owners_total: usize,
    owner_symbol_bytes: usize,
    matched_owners: usize,
    matched_bytes: usize,
    shared_core_owners: usize,
    shared_core_bytes: usize,
    regional_core_owners: usize,
    regional_core_bytes: usize,
    unresolved_owners: usize,
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

#[derive(Debug, Serialize)]
struct CorpusOwner {
    en_owner: String,
    source: String,
    size: usize,
    status: &'static str,
    starts: BTreeMap<String, String>,
    location_methods: BTreeMap<String, &'static str>,
    core_diff_bytes_from_ja: BTreeMap<String, usize>,
}

#[derive(Debug, Serialize)]
struct UnresolvedOwner {
    en_owner: String,
    source: String,
    error: String,
}

#[derive(Debug, Serialize)]
struct OverlayCorpusReport {
    schema_version: u32,
    game: &'static str,
    source_edition: &'static str,
    base_edition: &'static str,
    source_state: &'static str,
    resources_total: usize,
    owners_total: usize,
    owner_symbol_bytes: usize,
    matched_owners: usize,
    matched_bytes: usize,
    shared_core_owners: usize,
    shared_core_bytes: usize,
    regional_core_owners: usize,
    regional_core_bytes: usize,
    unresolved_owners: usize,
    resource_tables: BTreeMap<String, String>,
    editions: Vec<EditionSummary>,
    owners: Vec<OverlayCorpusOwner>,
    unresolved: Vec<OverlayUnresolvedOwner>,
}

#[derive(Debug, Serialize)]
struct OverlayCorpusOwner {
    en_owner: String,
    source: String,
    resource: String,
    size: usize,
    core_bytes: usize,
    status: &'static str,
    starts: BTreeMap<String, String>,
    location_methods: BTreeMap<String, &'static str>,
    core_diff_bytes_from_ja: BTreeMap<String, usize>,
}

#[derive(Debug, Serialize)]
struct OverlayUnresolvedOwner {
    en_owner: String,
    source: String,
    resource: String,
    size: usize,
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

struct RelocationSite {
    offset: usize,
    size: usize,
    kind: String,
    symbol: String,
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

struct LocationAnchor {
    en_offset: usize,
    starts: BTreeMap<&'static str, usize>,
}

struct OverlayOwner {
    name: String,
    source: String,
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
    if options.json {
        println!(
            "{}",
            serde_json::to_string_pretty(&report)
                .map_err(|error| format!("serialize report: {error}"))?
        );
    } else {
        print_report(&report, options.calls);
    }
    if report.core_identical {
        Ok(())
    } else {
        Err("one or more editions differ outside EN relocation and literal fields".into())
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
    let (size, symbol_address, relocation_mask, relocations) = relocation_mask(object_path, owner)?;
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

fn parse(args: &[String]) -> Result<Options, String> {
    let mut owner = None;
    let mut object = None;
    let mut object_dir = PathBuf::from("out/full/claimed/obj");
    let mut rom_dir = PathBuf::from("roms");
    let mut json = false;
    let mut calls = false;
    let mut all = false;
    let mut all_overlays = false;
    let mut write = None;
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
        owner_address(value)?;
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
        PathBuf::from(format!("out/claimed/obj/{owner}.o")),
    ];
    paths
        .into_iter()
        .find(|path| path.is_file())
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

fn exact_owners(object_dir: &Path) -> Result<Vec<String>, String> {
    let entries =
        fs::read_dir(object_dir).map_err(|error| format!("{}: {error}", object_dir.display()))?;
    let mut owners = entries
        .filter_map(Result::ok)
        .filter_map(|entry| {
            let path = entry.path();
            let owner = path.file_stem()?.to_str()?;
            (path.extension().is_some_and(|extension| extension == "o")
                && owner.len() == 8
                && owner.bytes().all(|byte| byte.is_ascii_hexdigit())
                && Path::new("exact").join(format!("{owner}.c")).is_file())
            .then(|| owner.to_ascii_lowercase())
        })
        .collect::<Vec<_>>();
    owners.sort();
    owners.dedup();
    if owners.is_empty() {
        return Err(format!(
            "{} contains no exact main-image owner objects; run `make build-claimed`",
            object_dir.display()
        ));
    }
    Ok(owners)
}

fn run_all(options: &Options, roms: &EditionRoms) -> Result<(), String> {
    let owner_names = exact_owners(&options.object_dir)?;
    let mut reports = BTreeMap::new();
    let mut failures = BTreeMap::new();
    let mut owner_symbol_bytes = 0;

    for (index, owner) in owner_names.iter().enumerate() {
        let object = options.object_dir.join(format!("{owner}.o"));
        if let Ok((size, _, _, _)) = relocation_mask(&object, owner) {
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
    let retry_names = failures.keys().cloned().collect::<Vec<_>>();
    for (index, owner) in retry_names.iter().enumerate() {
        let hints = nearest_location_hints(owner_address(owner)?, &location_anchors)?;
        let object = options.object_dir.join(format!("{owner}.o"));
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

    let mut owners = Vec::new();
    let mut matched_bytes = 0;
    let mut shared_core_bytes = 0;
    let mut regional_core_bytes = 0;
    let mut edition_totals = EDITIONS
        .into_iter()
        .map(|edition| (edition, [0usize; 6]))
        .collect::<BTreeMap<_, _>>();

    for (owner, report) in reports {
        matched_bytes += report.size;
        if report.core_identical {
            shared_core_bytes += report.size;
        } else {
            regional_core_bytes += report.size;
        }
        let mut starts = BTreeMap::new();
        let mut location_methods = BTreeMap::new();
        let mut core_diff_bytes_from_ja = BTreeMap::new();
        for edition in &report.editions {
            starts.insert(edition.edition.clone(), edition.start.clone());
            location_methods.insert(
                edition.edition.clone(),
                if edition.edition == "en" {
                    "source"
                } else if edition.anchor_matches == 0 {
                    "neighbor_exact"
                } else {
                    "global_anchor"
                },
            );
            core_diff_bytes_from_ja.insert(edition.edition.clone(), edition.core_diff_bytes);
            let totals = edition_totals
                .get_mut(edition.edition.as_str())
                .expect("known edition");
            totals[0] += 1;
            totals[1] += report.size;
            if edition.core_identical {
                totals[2] += 1;
                totals[3] += report.size;
            } else {
                totals[4] += 1;
                totals[5] += report.size;
            }
        }
        owners.push(CorpusOwner {
            en_owner: owner.clone(),
            source: format!("exact/{owner}.c"),
            size: report.size,
            status: if report.core_identical {
                "shared_core"
            } else {
                "regional_core"
            },
            starts,
            location_methods,
            core_diff_bytes_from_ja,
        });
    }

    let unresolved = failures
        .into_iter()
        .map(|(owner, error)| UnresolvedOwner {
            en_owner: owner.clone(),
            source: format!("exact/{owner}.c"),
            error,
        })
        .collect::<Vec<_>>();

    let editions = EDITIONS
        .into_iter()
        .map(|edition| {
            let totals = edition_totals[edition];
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
        .collect();
    let report = CorpusReport {
        schema_version: 1,
        game: "gs1",
        source_edition: "en",
        base_edition: "ja",
        source_state: "byte-exact C",
        owners_total: owner_names.len(),
        owner_symbol_bytes,
        matched_owners: owners.len(),
        matched_bytes,
        shared_core_owners: owners
            .iter()
            .filter(|owner| owner.status == "shared_core")
            .count(),
        shared_core_bytes,
        regional_core_owners: owners
            .iter()
            .filter(|owner| owner.status == "regional_core")
            .count(),
        regional_core_bytes,
        unresolved_owners: unresolved.len(),
        editions,
        owners,
        unresolved,
    };
    let json = serde_json::to_string_pretty(&report)
        .map_err(|error| format!("serialize corpus report: {error}"))?
        + "\n";
    if let Some(path) = &options.write {
        if let Some(parent) = path
            .parent()
            .filter(|parent| !parent.as_os_str().is_empty())
        {
            fs::create_dir_all(parent).map_err(|error| format!("{}: {error}", parent.display()))?;
        }
        fs::write(path, json).map_err(|error| format!("{}: {error}", path.display()))?;
        println!(
            "correspondence={} owners={} matched={} shared_core={} regional_core={} unresolved={}",
            path.display(),
            report.owners_total,
            report.matched_owners,
            report.shared_core_owners,
            report.regional_core_owners,
            report.unresolved_owners
        );
    } else {
        print!("{json}");
    }
    Ok(())
}

fn run_all_overlays(options: &Options, roms: &EditionRoms) -> Result<(), String> {
    let owner_list = exact_overlay_owners()?;
    let (resource_tables, decoded) = decode_overlay_resources(roms)?;
    let mut matches = BTreeMap::new();
    let mut failures = BTreeMap::new();

    for (index, owner) in owner_list.iter().enumerate() {
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

    remove_overlay_order_conflicts(&mut matches, &mut failures, "global")?;
    let global_hints = overlay_location_anchors(&matches);
    let retry = failures.keys().cloned().collect::<Vec<_>>();
    let by_name = owner_list
        .iter()
        .map(|owner| (owner.name.as_str(), owner))
        .collect::<BTreeMap<_, _>>();
    for (index, name) in retry.iter().enumerate() {
        let owner = by_name[name.as_str()];
        let hints = match nearest_overlay_hints(owner, &global_hints) {
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
    remove_overlay_order_conflicts(&mut matches, &mut failures, "locality")?;

    let mut owners = Vec::new();
    let mut edition_totals = EDITIONS
        .into_iter()
        .map(|edition| (edition, [0usize; 6]))
        .collect::<BTreeMap<_, _>>();
    let mut shared_core_bytes = 0;
    let mut regional_core_bytes = 0;
    for found in matches.into_values() {
        let ja_start = found.starts["ja"];
        let ja = overlay_window(
            &decoded,
            "ja",
            found.owner.resource,
            ja_start,
            found.owner.size,
        )?;
        let mut starts = BTreeMap::new();
        let mut methods = BTreeMap::new();
        let mut differences = BTreeMap::new();
        let mut shared = true;
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
            shared &= difference == 0;
            starts.insert(
                edition.to_string(),
                format!("0x{:08x}", OVERLAY_BASE + start as u64),
            );
            methods.insert(edition.to_string(), found.methods[edition]);
            differences.insert(edition.to_string(), difference);
            let totals = edition_totals.get_mut(edition).expect("known edition");
            totals[0] += 1;
            totals[1] += found.owner.size;
            if difference == 0 {
                totals[2] += 1;
                totals[3] += found.owner.size;
            } else {
                totals[4] += 1;
                totals[5] += found.owner.size;
            }
        }
        if shared {
            shared_core_bytes += found.owner.size;
        } else {
            regional_core_bytes += found.owner.size;
        }
        owners.push(OverlayCorpusOwner {
            en_owner: found.owner.name.clone(),
            source: found.owner.source.clone(),
            resource: format!("resource_{:03x}", found.owner.resource),
            size: found.owner.size,
            core_bytes: found.mask.iter().filter(|masked| !**masked).count(),
            status: if shared {
                "shared_core"
            } else {
                "regional_core"
            },
            starts,
            location_methods: methods,
            core_diff_bytes_from_ja: differences,
        });
    }
    owners.sort_by(|left, right| left.en_owner.cmp(&right.en_owner));

    let unresolved = owner_list
        .iter()
        .filter_map(|owner| {
            failures
                .get(&owner.name)
                .map(|error| OverlayUnresolvedOwner {
                    en_owner: owner.name.clone(),
                    source: owner.source.clone(),
                    resource: format!("resource_{:03x}", owner.resource),
                    size: owner.size,
                    error: error.clone(),
                })
        })
        .collect::<Vec<_>>();
    let editions = EDITIONS
        .into_iter()
        .map(|edition| {
            let totals = edition_totals[edition];
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
        .collect();
    let report = OverlayCorpusReport {
        schema_version: 1,
        game: "gs1",
        source_edition: "en",
        base_edition: "ja",
        source_state: "byte-exact overlay C",
        resources_total: OVERLAY_LAST - OVERLAY_FIRST + 1,
        owners_total: owner_list.len(),
        owner_symbol_bytes: owner_list.iter().map(|owner| owner.size).sum(),
        matched_owners: owners.len(),
        matched_bytes: owners.iter().map(|owner| owner.size).sum(),
        shared_core_owners: owners
            .iter()
            .filter(|owner| owner.status == "shared_core")
            .count(),
        shared_core_bytes,
        regional_core_owners: owners
            .iter()
            .filter(|owner| owner.status == "regional_core")
            .count(),
        regional_core_bytes,
        unresolved_owners: unresolved.len(),
        resource_tables,
        editions,
        owners,
        unresolved,
    };
    let json = serde_json::to_string_pretty(&report)
        .map_err(|error| format!("serialize overlay corpus report: {error}"))?
        + "\n";
    if let Some(path) = &options.write {
        if let Some(parent) = path
            .parent()
            .filter(|parent| !parent.as_os_str().is_empty())
        {
            fs::create_dir_all(parent).map_err(|error| format!("{}: {error}", parent.display()))?;
        }
        fs::write(path, json).map_err(|error| format!("{}: {error}", path.display()))?;
        println!(
            "overlay_correspondence={} owners={} matched={} shared_core={} regional_core={} unresolved={}",
            path.display(),
            report.owners_total,
            report.matched_owners,
            report.shared_core_owners,
            report.regional_core_owners,
            report.unresolved_owners
        );
    } else {
        print!("{json}");
    }
    Ok(())
}

fn exact_overlay_owners() -> Result<Vec<OverlayOwner>, String> {
    let mut assembly = BTreeMap::<usize, Vec<String>>::new();
    let mut owners = Vec::new();
    for entry in fs::read_dir("exact").map_err(|error| format!("exact: {error}"))? {
        let path = entry.map_err(|error| error.to_string())?.path();
        let Some(name) = path.file_name().and_then(|name| name.to_str()) else {
            continue;
        };
        let Some(stem) = name.strip_suffix(".c") else {
            continue;
        };
        let Some((resource, address)) = stem.split_once("_c_") else {
            continue;
        };
        let Some(resource) = resource.strip_prefix("resource_") else {
            continue;
        };
        let Ok(resource) = usize::from_str_radix(resource, 16) else {
            continue;
        };
        if !(OVERLAY_FIRST..=OVERLAY_LAST).contains(&resource)
            || address.len() != 8
            || !address.bytes().all(|byte| byte.is_ascii_hexdigit())
        {
            continue;
        }
        let absolute = u64::from_str_radix(address, 16).map_err(|error| error.to_string())?;
        let en_offset = absolute
            .checked_sub(OVERLAY_BASE)
            .ok_or_else(|| format!("{name}: address is below overlay base"))?
            as usize;
        if !assembly.contains_key(&resource) {
            let source = format!("assets/code/resource_{resource:03x}_overlay.s");
            let text = fs::read_to_string(&source).map_err(|error| format!("{source}: {error}"))?;
            assembly.insert(resource, text.lines().map(str::to_string).collect());
        }
        let tag = format!("AlchemyC_{absolute:08x}:");
        let lines = &assembly[&resource];
        let start = lines
            .iter()
            .position(|line| line.trim() == tag)
            .ok_or_else(|| format!("{name}: missing {tag} placeholder"))?;
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
            return Err(format!("{name}: exact placeholder has no span"));
        }
        owners.push(OverlayOwner {
            name: format!("resource_{resource:03x}:0x{absolute:08x}"),
            source: format!("exact/{name}"),
            resource,
            en_offset,
            size,
        });
    }
    owners.sort_by_key(|owner| (owner.resource, owner.en_offset));
    if owners.is_empty() {
        return Err("exact/ contains no exact overlay C owners".into());
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
        let table = resource_table(rom)?;
        tables.insert(
            edition.to_string(),
            format!("0x{:08x}", ROM_BASE + table as u64),
        );
        let mut resources = BTreeMap::new();
        for resource in OVERLAY_FIRST..=OVERLAY_LAST {
            let start = resource_pointer(rom, table, resource)?;
            let next = resource_pointer(rom, table, resource + 1).ok();
            let end = next
                .filter(|next| *next > start && *next <= rom.len())
                .unwrap_or(rom.len());
            let (bytes, _) = match rom[start] {
                0 => extract_resource::decode_general(rom, start, end, 0x10_0000),
                1 => extract_resource::decode_palette(rom, start + 1, end, 0x10_0000),
                tag => return Err(format!("{edition}: resource {resource:03x} has tag {tag}")),
            }
            .map_err(|error| format!("{edition}: resource {resource:03x}: {error}"))?;
            resources.insert(resource, bytes);
        }
        decoded.insert(edition, resources);
    }
    Ok((tables, decoded))
}

fn resource_table(rom: &[u8]) -> Result<usize, String> {
    (0..rom.len().saturating_sub(8))
        .step_by(4)
        .find(|offset| {
            u32::from_le_bytes(rom[*offset..*offset + 4].try_into().unwrap()) as u64 == ROM_BASE
                && u32::from_le_bytes(rom[*offset + 4..*offset + 8].try_into().unwrap()) as u64
                    == ROM_BASE + *offset as u64
        })
        .ok_or("resource directory self-pointer was not found".into())
}

fn resource_pointer(rom: &[u8], table: usize, resource: usize) -> Result<usize, String> {
    let at = table
        .checked_add(resource * 4)
        .ok_or("resource directory offset overflow")?;
    let address = u32::from_le_bytes(
        rom.get(at..at + 4)
            .ok_or("resource directory extends past ROM")?
            .try_into()
            .unwrap(),
    ) as u64;
    rom_offset(address, rom.len())
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
    let mut mask = vec![false; owner.len()];
    for at in (0..owner.len().saturating_sub(3)).step_by(2) {
        let high = u16::from_le_bytes([owner[at], owner[at + 1]]);
        let low = u16::from_le_bytes([owner[at + 2], owner[at + 3]]);
        if high & 0xf800 == 0xf000 && low & 0xf800 == 0xf800 {
            mask[at..at + 4].fill(true);
        }
    }
    for at in (0..owner.len().saturating_sub(1)).step_by(2) {
        let instruction = u16::from_le_bytes([owner[at], owner[at + 1]]);
        if instruction & 0xf800 != 0x4800 {
            continue;
        }
        let pc = (OVERLAY_BASE + en_offset as u64 + at as u64 + 4) & !3;
        let target = pc + u64::from(instruction & 0xff) * 4;
        let Some(literal) = target.checked_sub(OVERLAY_BASE + en_offset as u64) else {
            continue;
        };
        let literal = literal as usize;
        if literal + 4 <= mask.len() {
            mask[literal..literal + 4].fill(true);
        }
    }
    mask
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
                    source: owner.source.clone(),
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
            source: owner.source.clone(),
            resource: owner.resource,
            en_offset: owner.en_offset,
            size: owner.size,
        },
        mask,
        starts,
        methods,
    })
}

struct OverlayLocationAnchor {
    resource: usize,
    en_offset: usize,
    starts: BTreeMap<&'static str, usize>,
}

fn overlay_location_anchors(
    matches: &BTreeMap<String, OverlayMatch>,
) -> Vec<OverlayLocationAnchor> {
    matches
        .values()
        .map(|found| OverlayLocationAnchor {
            resource: found.owner.resource,
            en_offset: found.owner.en_offset,
            starts: found.starts.clone(),
        })
        .collect()
}

fn nearest_overlay_hints(
    owner: &OverlayOwner,
    anchors: &[OverlayLocationAnchor],
) -> Result<BTreeMap<&'static str, usize>, String> {
    let anchor = anchors
        .iter()
        .filter(|anchor| anchor.resource == owner.resource)
        .min_by_key(|anchor| anchor.en_offset.abs_diff(owner.en_offset))
        .ok_or_else(|| format!("resource_{:03x}: no global anchors", owner.resource))?;
    let mut hints = BTreeMap::new();
    for edition in EDITIONS.into_iter().filter(|edition| *edition != "en") {
        let predicted =
            anchor.starts[edition] as i128 + owner.en_offset as i128 - anchor.en_offset as i128;
        if predicted >= 0 {
            hints.insert(edition, predicted as usize);
        }
    }
    Ok(hints)
}

fn remove_overlay_order_conflicts(
    matches: &mut BTreeMap<String, OverlayMatch>,
    failures: &mut BTreeMap<String, String>,
    phase: &str,
) -> Result<(), String> {
    loop {
        let mut conflict = None;
        for resource in OVERLAY_FIRST..=OVERLAY_LAST {
            let names = matches
                .iter()
                .filter(|(_, found)| found.owner.resource == resource)
                .map(|(name, _)| name.clone())
                .collect::<Vec<_>>();
            for edition in EDITIONS.into_iter().filter(|edition| *edition != "en") {
                for pair in names.windows(2) {
                    if matches[&pair[1]].starts[edition] <= matches[&pair[0]].starts[edition] {
                        conflict = Some((edition, pair[0].clone(), pair[1].clone()));
                        break;
                    }
                }
                if conflict.is_some() {
                    break;
                }
            }
            if conflict.is_some() {
                break;
            }
        }
        let Some((edition, left, right)) = conflict else {
            return Ok(());
        };
        let left_error = overlay_prediction_error(&left, edition, matches, [&left, &right]);
        let right_error = overlay_prediction_error(&right, edition, matches, [&left, &right]);
        let rejected = if right_error >= left_error {
            right
        } else {
            left
        };
        matches.remove(&rejected);
        failures.insert(
            rejected,
            format!("{edition}: {phase} counterpart conflicts with neighboring proved owner order"),
        );
    }
}

fn overlay_prediction_error(
    owner: &str,
    edition: &str,
    matches: &BTreeMap<String, OverlayMatch>,
    excluded: [&str; 2],
) -> usize {
    let found = &matches[owner];
    let neighbor = matches
        .iter()
        .filter(|(name, candidate)| {
            candidate.owner.resource == found.owner.resource && !excluded.contains(&name.as_str())
        })
        .min_by_key(|(_, candidate)| candidate.owner.en_offset.abs_diff(found.owner.en_offset));
    let Some((_, neighbor)) = neighbor else {
        return usize::MAX;
    };
    let predicted = neighbor.starts[edition] as i128 + found.owner.en_offset as i128
        - neighbor.owner.en_offset as i128;
    found.starts[edition].abs_diff(predicted.max(0) as usize)
}

fn location_anchors(reports: &BTreeMap<String, Report>) -> Result<Vec<LocationAnchor>, String> {
    reports
        .values()
        .map(|report| {
            let mut starts = BTreeMap::new();
            for edition in EDITIONS {
                let value = report
                    .editions
                    .iter()
                    .find(|entry| entry.edition == edition)
                    .ok_or_else(|| format!("{} lacks {edition} location", report.owner))?;
                starts.insert(edition, parse_rom_address(&value.start)?);
            }
            Ok(LocationAnchor {
                en_offset: rom_offset(owner_address(&report.owner)?, usize::MAX)?,
                starts,
            })
        })
        .collect()
}

fn remove_order_conflicts(
    reports: &mut BTreeMap<String, Report>,
    failures: &mut BTreeMap<String, String>,
    phase: &str,
) -> Result<(), String> {
    loop {
        let names = reports.keys().cloned().collect::<Vec<_>>();
        let mut conflict = None;
        for edition in EDITIONS.into_iter().filter(|edition| *edition != "en") {
            for pair in names.windows(2) {
                let left = &reports[&pair[0]];
                let right = &reports[&pair[1]];
                let left_start = report_start(left, edition)?;
                let right_start = report_start(right, edition)?;
                if right_start <= left_start {
                    conflict = Some((edition, pair[0].clone(), pair[1].clone()));
                    break;
                }
            }
            if conflict.is_some() {
                break;
            }
        }
        let Some((edition, left, right)) = conflict else {
            return Ok(());
        };
        let left_error = location_prediction_error(&left, edition, reports, [&left, &right])?;
        let right_error = location_prediction_error(&right, edition, reports, [&left, &right])?;
        let rejected = if right_error >= left_error {
            right
        } else {
            left
        };
        reports.remove(&rejected);
        failures.insert(
            rejected,
            format!("{edition}: {phase} counterpart conflicts with neighboring proved owner order"),
        );
    }
}

fn location_prediction_error(
    owner: &str,
    edition: &str,
    reports: &BTreeMap<String, Report>,
    excluded: [&str; 2],
) -> Result<usize, String> {
    let en_offset = rom_offset(owner_address(owner)?, usize::MAX)?;
    let report = &reports[owner];
    let actual = report_start(report, edition)?;
    let neighbor = reports
        .iter()
        .filter(|(name, _)| !excluded.contains(&name.as_str()))
        .min_by_key(|(name, _)| {
            owner_address(name)
                .ok()
                .and_then(|address| rom_offset(address, usize::MAX).ok())
                .map_or(usize::MAX, |offset| offset.abs_diff(en_offset))
        })
        .ok_or("not enough correspondence anchors to resolve order conflict")?;
    let neighbor_en = rom_offset(owner_address(neighbor.0)?, usize::MAX)?;
    let neighbor_start = report_start(neighbor.1, edition)?;
    let predicted = neighbor_start as i128 + en_offset as i128 - neighbor_en as i128;
    Ok(actual.abs_diff(predicted.max(0) as usize))
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
    owner_address: u64,
    anchors: &[LocationAnchor],
) -> Result<BTreeMap<&'static str, usize>, String> {
    let en_offset = rom_offset(owner_address, usize::MAX)?;
    let mut hints = BTreeMap::new();
    for edition in EDITIONS.into_iter().filter(|edition| *edition != "en") {
        let anchor = anchors
            .iter()
            .min_by_key(|anchor| anchor.en_offset.abs_diff(en_offset))
            .ok_or("no global correspondence anchors are available")?;
        let start = anchor.starts[edition] as i128 + en_offset as i128 - anchor.en_offset as i128;
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
) -> Result<(usize, u64, Vec<bool>, Vec<RelocationSite>), String> {
    let config = DiffObjConfig {
        arm_arch_version: ArmArchVersion::V4t,
        ..Default::default()
    };
    let object = obj::read::read(path, &config, DiffSide::Base)
        .map_err(|error| format!("{}: {error}", path.display()))?;
    let symbol_name = format!("Func_{owner}");
    let symbol_index = object
        .symbol_by_name(&symbol_name)
        .ok_or_else(|| format!("{}: missing symbol {symbol_name}", path.display()))?;
    let symbol = &object.symbols[symbol_index];
    let section = &object.sections[symbol.section.ok_or("owner symbol has no section")?];
    let size = usize::try_from(symbol.size).map_err(|_| "owner is too large")?;
    if size == 0 {
        return Err(format!("{symbol_name} has zero size"));
    }
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
        let symbol = object
            .symbols
            .get(relocation.target_symbol)
            .map(|symbol| symbol.name.clone())
            .unwrap_or_else(|| "<missing>".into());
        sites.push(RelocationSite {
            offset: start,
            size: end - start,
            kind,
            symbol,
        });
    }
    Ok((size, symbol.address, mask, sites))
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
        report.owner,
        report.owner_edition,
        report.base_edition,
        report.relocation_source_edition,
        report.size,
        report.relocations,
        report.relocation_bytes,
        report.literal_fields,
        report.literal_bytes,
        report.core_bytes
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
    }

    #[test]
    fn counts_only_unmasked_differences_as_core() {
        let left = [1, 2, 3, 4, 5, 6];
        let right = [1, 9, 3, 8, 5, 6];
        let mask = [false, true, false, false, false, false];
        assert_eq!(core_diff_bytes(&left, &right, &mask), 1);
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
    fn finds_relocated_resource_directory_from_self_pointer() {
        let mut rom = vec![0xff; 64];
        rom[16..20].copy_from_slice(&(ROM_BASE as u32).to_le_bytes());
        rom[20..24].copy_from_slice(&((ROM_BASE + 16) as u32).to_le_bytes());
        assert_eq!(resource_table(&rom), Ok(16));
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
