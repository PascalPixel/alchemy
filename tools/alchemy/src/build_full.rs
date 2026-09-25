//! Compose the claimed C, retained assembly, and asset outputs into the full ROM.
use crate::compiler::build_io::{argv, read, read_json, rooted, text, write};
use crate::compiler::canonical_json::write_canonical;
use crate::compiler::source_paths::{
    source_paths_manifest, SourceOwner, SourcePaths, SHARED_SOURCE_ROOT,
};
use crate::compiler::translation_units::{AbsoluteSymbolKind, OwnerState, TranslationUnits};
use crate::targets::{
    parse_decomp_target, target_for, BuildSupport, DecompTarget, DecompTargetId, DEFAULT_TARGET,
};
use serde_json::{json, Number, Value};
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};
use std::process::{Command, Stdio};
const ROM_BASE: u64 = 0x0800_0000;

pub fn run(arguments: &[String]) -> Result<(), String> {
    if arguments == ["--self-test"] {
        self_test()?;
        println!("self-test=ok");
        return Ok(());
    }
    let options = match parse_args(arguments)? {
        ParseOutcome::Help => {
            println!("usage: alchemy build full [-h] [--target GAME-EDITION] [-o OUTPUT] [--claimed-output CLAIMED_OUTPUT] [--asm-output ASM_OUTPUT] [--asset-manifest ASSET_MANIFEST] [--asset-output ASSET_OUTPUT] [--jobs JOBS] [--source-only] [rom] | --self-test");
            return Ok(());
        }
        ParseOutcome::Run(options) => options,
    };
    let cwd = std::env::current_dir().map_err(|error| format!("cwd: {error}"))?;
    println!("{}", build(&repository_root(), &cwd, &options)?);
    Ok(())
}
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Options {
    pub target: DecompTargetId,
    pub rom: String,
    pub source_only: bool,
    pub output: String,
    pub claimed_output: String,
    pub asm_output: String,
    pub asset_manifest: String,
    pub asset_output: String,
    pub jobs: usize,
}
#[derive(Debug, Clone, PartialEq, Eq)]
pub enum ParseOutcome {
    Help,
    Run(Options),
}
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct GapRegion {
    pub address: u64,
    pub size: usize,
}
#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
pub struct AssemblyAccounting {
    pub c_debt_regions: usize,
    pub c_debt_bytes: usize,
    pub retained_regions: usize,
    pub retained_bytes: usize,
}
#[derive(Debug, Clone, Copy, PartialEq)]
pub struct ReconstructionProgress {
    pub bytes: usize,
    pub remaining_bytes: usize,
    pub percent: f64,
}
#[derive(Debug, Clone)]
struct Region {
    address: u64,
    size: usize,
    output: PathBuf,
    kind: Option<String>,
    retention: Option<String>,
    translation_unit: Option<String>,
    composition: Option<String>,
    byte_verification: Option<String>,
    source: Option<String>,
    symbols: Vec<String>,
    run_address: Option<u64>,
    origin: Option<String>,
    confidence: Option<String>,
    evidence: Option<String>,
}
impl Region {
    fn is_owner(&self, address: u64, size: usize) -> bool {
        self.address == address && self.size == size
    }
}
pub fn repository_root() -> PathBuf {
    crate::compiler::routing::root().to_path_buf()
}
/// The game a full build composes: its owner register is the identity
/// authority, and its translation units, retained overlay listings and
/// overlay-assembly evidence live in its reconstruction scaffolding.
#[derive(Clone, Copy)]
struct Game {
    target: DecompTarget,
}
impl Game {
    fn of(target: DecompTarget) -> Self {
        Self { target }
    }
    /// The build id units and registers name the game by: `tbs` or `tla`.
    fn id(self) -> &'static str {
        self.target.compiler.as_str()
    }
    fn identity_authority(self) -> Result<String, String> {
        source_paths_manifest(self.id()).map(text)
    }
    fn translation_units(self) -> String {
        format!("{}/translation-units.json", self.target.recon_dir())
    }
    fn overlay_assembly_evidence(self) -> String {
        format!("{}/semantic/overlay-assembly.json", self.target.recon_dir())
    }
    fn overlay_listing(self, overlay: &str) -> String {
        self.target.overlay_assembly(overlay)
    }
    fn register(self, root: &Path) -> Result<SourcePaths, String> {
        SourcePaths::load_for_game(root, self.id())
    }
    fn units(self, root: &Path) -> Result<TranslationUnits, String> {
        TranslationUnits::load_game(root, self.target.compiler)
    }
}
fn default_jobs() -> usize {
    std::thread::available_parallelism()
        .map_or(1, usize::from)
        .min(16)
}
fn sidecar_path(output: &Path, suffix: &str) -> Result<PathBuf, String> {
    let mut path = output.with_extension("");
    let mut name = path
        .file_name()
        .ok_or_else(|| format!("{} has no output file name", output.display()))?
        .to_os_string();
    name.push(".");
    name.push(suffix);
    path.set_file_name(name);
    Ok(path)
}
fn defaults(target_id: DecompTargetId) -> Options {
    let target = target_for(target_id);
    let full = Path::new(target.output_dir).join("full");
    Options {
        target: target_id,
        rom: target.rom.into(),
        source_only: false,
        output: text(full.join("rebuilt.gba")),
        claimed_output: text(full.join("claimed")),
        asm_output: text(full.join("asm")),
        asset_manifest: target.asset_manifest.into(),
        asset_output: text(full.join("assets")),
        jobs: default_jobs(),
    }
}
pub fn parse_args(argv: &[String]) -> Result<ParseOutcome, String> {
    if argv.iter().any(|arg| arg == "-h" || arg == "--help") {
        return Ok(ParseOutcome::Help);
    }
    let mut target_id = DEFAULT_TARGET;
    let mut index = 0usize;
    while index < argv.len() {
        if argv[index] == "--target" {
            index += 1;
            target_id = parse_decomp_target(argv.get(index).ok_or("--target requires a value")?)?;
        } else if let Some(value) = argv[index].strip_prefix("--target=") {
            target_id = parse_decomp_target(value)?;
        }
        index += 1;
    }
    let mut options = defaults(target_id);
    let mut positional = false;
    index = 0;
    while index < argv.len() {
        let argument = &argv[index];
        if argument == "--source-only" {
            options.source_only = true;
            index += 1;
            continue;
        }
        if argument == "--target" {
            index += 2;
            continue;
        }
        if argument.starts_with("--target=") {
            index += 1;
            continue;
        }
        let (option, inline) = argument
            .split_once('=')
            .map_or((argument.as_str(), None), |(a, b)| (a, Some(b)));
        let field = match option {
            "-o" | "--output" => Some("output"),
            "--claimed-output" => Some("claimed"),
            "--asm-output" => Some("asm"),
            "--asset-manifest" => Some("manifest"),
            "--asset-output" => Some("assets"),
            "--jobs" => Some("jobs"),
            _ => None,
        };
        if let Some(field) = field {
            let value = if let Some(value) = inline {
                value.to_string()
            } else {
                index += 1;
                argv.get(index)
                    .ok_or_else(|| format!("{option} requires a value"))?
                    .clone()
            };
            match field {
                "output" => options.output = value,
                "claimed" => options.claimed_output = value,
                "asm" => options.asm_output = value,
                "manifest" => options.asset_manifest = value,
                "assets" => options.asset_output = value,
                "jobs" => options.jobs = parse_jobs(&value)?,
                _ => unreachable!(),
            }
        } else if !argument.starts_with('-') && !positional {
            options.rom = argument.clone();
            positional = true;
        } else {
            return Err(format!("unrecognized argument: {argument}"));
        }
        index += 1;
    }
    if options.source_only && positional {
        return Err("--source-only does not accept a ROM".into());
    }
    Ok(ParseOutcome::Run(options))
}
fn parse_jobs(value: &str) -> Result<usize, String> {
    let digits: String = value
        .chars()
        .take_while(|ch| ch.is_ascii_digit() || *ch == '+' || *ch == '-')
        .collect();
    let parsed = digits.parse::<isize>().ok();
    parsed
        .filter(|jobs| *jobs >= 1)
        .map(|jobs| jobs as usize)
        .ok_or_else(|| "jobs must be positive".into())
}
fn has_assembly_sources(directory: &Path) -> Result<bool, String> {
    for entry in
        std::fs::read_dir(directory).map_err(|error| format!("{}: {error}", directory.display()))?
    {
        let entry = entry.map_err(|error| format!("{}: {error}", directory.display()))?;
        if entry.file_name().to_string_lossy().starts_with('.') {
            continue;
        }
        let kind = entry
            .file_type()
            .map_err(|error| format!("{}: {error}", entry.path().display()))?;
        if kind.is_file()
            && entry
                .path()
                .extension()
                .is_some_and(|ext| ext.eq_ignore_ascii_case("s"))
        {
            return Ok(true);
        }
        if kind.is_dir() && has_assembly_sources(&entry.path())? {
            return Ok(true);
        }
    }
    Ok(false)
}
fn run_stage(root: &Path, command: &[String]) -> Result<(), String> {
    let (program, args) = command.split_first().ok_or("empty command")?;
    let status = Command::new(program)
        .args(args)
        .current_dir(root)
        .stdin(Stdio::null())
        .stdout(Stdio::inherit())
        .stderr(Stdio::inherit())
        .status()
        .map_err(|error| format!("{program}: {error}"))?;
    if status.success() {
        Ok(())
    } else {
        Err(format!(
            "{} failed with status {}",
            args.first().unwrap_or(program),
            status.code().unwrap_or(-1)
        ))
    }
}
/// Run one stage through the unified contributor executable.
fn stage_command(stage: &str) -> Result<Vec<String>, String> {
    let executable =
        std::env::current_exe().map_err(|error| format!("alchemy executable: {error}"))?;
    Ok(argv(&[&text(executable), "build", stage]))
}
fn value_u64(value: &Value, label: &str) -> Result<u64, String> {
    value
        .as_u64()
        .or_else(|| {
            let text = value.as_str()?.trim();
            u64::from_str_radix(
                text.strip_prefix("0x").unwrap_or(text),
                if text.starts_with("0x") { 16 } else { 10 },
            )
            .ok()
        })
        .ok_or_else(|| format!("invalid {label}"))
}
fn regions(document: &Value) -> Result<Vec<Region>, String> {
    let string = |item: &Value, field: &str| item[field].as_str().map(str::to_string);
    document["regions"]
        .as_array()
        .ok_or("manifest regions differ")?
        .iter()
        .map(|item| {
            Ok(Region {
                address: value_u64(&item["address"], "region address")?,
                size: value_u64(&item["size"], "region size")? as usize,
                output: item["output"]
                    .as_str()
                    .map_or_else(PathBuf::new, PathBuf::from),
                kind: string(item, "kind"),
                retention: string(item, "retention"),
                translation_unit: string(item, "translation_unit"),
                composition: string(item, "composition"),
                byte_verification: string(item, "byte_verification"),
                source: string(item, "source"),
                symbols: item["symbols"]
                    .as_array()
                    .into_iter()
                    .flatten()
                    .filter_map(|symbol| symbol.as_str().map(str::to_string))
                    .chain(item["symbol"].as_str().map(str::to_string))
                    .collect::<BTreeSet<_>>()
                    .into_iter()
                    .collect(),
                run_address: item["run_address"]
                    .as_u64()
                    .or_else(|| item["run_address"].as_str()?.parse().ok()),
                origin: string(item, "origin"),
                confidence: string(item, "confidence"),
                evidence: string(item, "evidence"),
            })
        })
        .collect()
}
pub fn unowned_regions(mask: &[u8], base: u64) -> Result<Vec<GapRegion>, String> {
    if base
        .checked_add(mask.len() as u64)
        .is_none_or(|end| end > u32::MAX as u64 + 1)
    {
        return Err("invalid coverage base".into());
    }
    let mut result = Vec::new();
    let mut start: Option<usize> = None;
    for index in 0..=mask.len() {
        let value = if index == mask.len() { 1 } else { mask[index] };
        if value > 1 {
            return Err(format!("invalid coverage byte at {index}"));
        }
        if value == 0 && start.is_none() {
            start = Some(index);
        }
        if value == 1 {
            if let Some(begin) = start.take() {
                result.push(GapRegion {
                    address: base + begin as u64,
                    size: index - begin,
                });
            }
        }
    }
    Ok(result)
}
fn assembly_accounting(regions: &[Region]) -> Result<AssemblyAccounting, String> {
    let debt = [
        "c_candidate",
        "split_first",
        "merge_with_owner",
        "merge_with_function_owner",
        "merge_with_continuations",
    ];
    // Compiler runtime built from its licensed container is explained, not C debt.
    let retained = [
        "keep_asm",
        "keep_structured_asm",
        "not_yet_c",
        "adjacent_section_alignment",
        "container_runtime",
    ];
    let mut result = AssemblyAccounting::default();
    for region in regions {
        if region.size == 0 {
            return Err("invalid assembly accounting size".into());
        }
        let retention = region
            .retention
            .as_deref()
            .ok_or("unsupported assembly retention: null")?;
        if debt.contains(&retention) {
            result.c_debt_regions += 1;
            result.c_debt_bytes += region.size;
        } else if retained.contains(&retention) {
            result.retained_regions += 1;
            result.retained_bytes += region.size;
        } else {
            return Err(format!("unsupported assembly retention: {retention}"));
        }
    }
    Ok(result)
}
fn validate_alignments(claimed: &[Region], assembly: &[Region]) -> Result<(), String> {
    let bodies = claimed.iter().chain(
        assembly
            .iter()
            .filter(|region| region.kind.as_deref() != Some("alignment_padding")),
    );
    let starts: BTreeSet<u64> = bodies.clone().map(|region| region.address).collect();
    let ends: BTreeSet<u64> = bodies
        .map(|region| region.address + region.size as u64)
        .collect();
    for region in assembly
        .iter()
        .filter(|region| region.kind.as_deref() == Some("alignment_padding"))
    {
        if region.size != 2
            || region.address & 3 != 2
            || region.retention.as_deref() != Some("adjacent_section_alignment")
            || !ends.contains(&region.address)
            || !starts.contains(&(region.address + 2))
        {
            return Err(format!(
                "unproven assembly alignment at 0x{:08x}",
                region.address
            ));
        }
    }
    Ok(())
}
struct RegisteredOwnerCoverage {
    registered: usize,
    registered_main: usize,
    registered_overlay: usize,
    declared: usize,
    declared_main: usize,
    declared_overlay: usize,
    missing: usize,
    unexpected: usize,
}
fn registered_owner_coverage(
    root: &Path,
    game: Game,
    units: &TranslationUnits,
) -> Result<RegisteredOwnerCoverage, String> {
    let registered = game
        .register(root)?
        .registered_owners()
        .collect::<BTreeSet<_>>();
    let mut declared = BTreeSet::new();
    for unit in &units.units {
        for image in unit.images() {
            for member in unit.members_in(image) {
                declared.insert(unit.source_owner(image, member.address)?);
            }
        }
    }
    let counts = |owners: &BTreeSet<SourceOwner>| {
        let main = owners.iter().filter(|owner| owner.is_main()).count();
        (owners.len(), main, owners.len() - main)
    };
    let (registered_count, registered_main, registered_overlay) = counts(&registered);
    let (declared_count, declared_main, declared_overlay) = counts(&declared);
    Ok(RegisteredOwnerCoverage {
        registered: registered_count,
        registered_main,
        registered_overlay,
        declared: declared_count,
        declared_main,
        declared_overlay,
        missing: registered.difference(&declared).count(),
        unexpected: declared.difference(&registered).count(),
    })
}
#[derive(Clone)]
struct InventoryMember {
    unit: String,
    /// The image whose instance links this member; none for the canonical image.
    instance: Option<String>,
    source: String,
    role: &'static str,
    ordinal: usize,
    state: Option<&'static str>,
    alias: String,
    extent: usize,
}
fn owner_state(state: OwnerState) -> &'static str {
    match state {
        OwnerState::ExactC => "exact-c",
        OwnerState::NotYetC => "not-yet-c",
    }
}
fn absolute_kind(kind: AbsoluteSymbolKind) -> &'static str {
    match kind {
        AbsoluteSymbolKind::Data => "data",
        AbsoluteSymbolKind::Thumb => "thumb",
        AbsoluteSymbolKind::Arm => "arm",
    }
}
fn region_starts<'a>(
    regions: &'a [Region],
    label: &str,
) -> Result<BTreeMap<u64, &'a Region>, String> {
    let mut starts = BTreeMap::new();
    for region in regions {
        if starts.insert(region.address, region).is_some() {
            return Err(format!(
                "duplicate {label} region at 0x{:08x}",
                region.address
            ));
        }
    }
    Ok(starts)
}
fn claimed_symbols<'a>(regions: &'a [Region]) -> Result<BTreeMap<String, &'a Region>, String> {
    let mut symbols = BTreeMap::new();
    for region in regions {
        for symbol in &region.symbols {
            if symbols.insert(symbol.clone(), region).is_some() {
                return Err(format!("duplicate claimed symbol {symbol}"));
            }
        }
    }
    Ok(symbols)
}
struct LinkedFunction<'a> {
    address: u64,
    size: usize,
    artifact: &'a Region,
}
fn linked_functions<'a>(
    document: &Value,
    symbols: &BTreeMap<String, &'a Region>,
) -> Result<BTreeMap<String, LinkedFunction<'a>>, String> {
    let rows = document["functions"]
        .as_array()
        .ok_or("claimed manifest lacks linked function extents")?;
    let mut functions = BTreeMap::new();
    for row in rows {
        let symbol = row["symbol"]
            .as_str()
            .ok_or("linked function lacks symbol")?;
        let address = value_u64(&row["address"], "linked function address")?;
        let size = value_u64(&row["size"], "linked function size")? as usize;
        let module = row["module"]
            .as_str()
            .ok_or("linked function lacks module")?;
        if symbol != format!("Func_{address:08x}") || size == 0 {
            return Err(format!("invalid linked function {symbol}"));
        }
        let artifact = symbols
            .get(module)
            .copied()
            .ok_or_else(|| format!("{symbol}: missing claimed module {module}"))?;
        if !artifact.symbols.iter().any(|item| item == symbol) {
            return Err(format!("{symbol}: module {module} does not export it"));
        }
        if functions
            .insert(
                symbol.into(),
                LinkedFunction {
                    address,
                    size,
                    artifact,
                },
            )
            .is_some()
        {
            return Err(format!("duplicate linked function {symbol}"));
        }
    }
    Ok(functions)
}
fn inventory_members(
    game: Game,
    units: &TranslationUnits,
) -> Result<BTreeMap<SourceOwner, InventoryMember>, String> {
    let mut members = BTreeMap::new();
    for unit in units.units.iter().filter(|unit| unit.game == game.id()) {
        let source = text(&unit.source);
        let mut insert = |image: &str, ordinal, address, role, state, alias: &str, extent| {
            let key = unit.source_owner(image, address)?;
            let value = InventoryMember {
                unit: unit.id.clone(),
                instance: (image != unit.image()).then(|| image.to_string()),
                source: source.clone(),
                role,
                ordinal,
                state,
                alias: alias.into(),
                extent,
            };
            if members.insert(key, value).is_some() {
                return Err(format!("duplicate reconstruction-unit member {}", key.id()));
            }
            Ok(())
        };
        for (ordinal, owner) in unit.owners.iter().enumerate() {
            insert(
                unit.image(),
                ordinal,
                owner.address,
                "owner",
                Some(owner_state(owner.state)),
                &owner.canonical_name,
                owner.extent,
            )?;
        }
        for (ordinal, symbol) in unit.local_symbols.iter().enumerate() {
            insert(
                unit.image(),
                ordinal,
                symbol.address,
                "local-symbol",
                None,
                &symbol.canonical_name,
                symbol.extent,
            )?;
        }
        // Instances are wholly exact, and keep each member's role and ordinal.
        for image in unit.instances.keys() {
            let (mut owners, mut locals) = (0, 0);
            for member in unit.members_in(image) {
                let (role, ordinal, state) = match member.owner {
                    true => ("owner", &mut owners, Some(owner_state(OwnerState::ExactC))),
                    false => ("local-symbol", &mut locals, None),
                };
                insert(
                    image,
                    *ordinal,
                    member.address,
                    role,
                    state,
                    member.name,
                    member.extent,
                )?;
                *ordinal += 1;
            }
        }
    }
    Ok(members)
}
fn source_group(game: Game, source: &str) -> Option<String> {
    let source = Path::new(source);
    source
        .strip_prefix(game.target.source_dir)
        .or_else(|_| source.strip_prefix(SHARED_SOURCE_ROOT))
        .ok()?
        .parent()?
        .to_str()
        .filter(|group| !group.is_empty())
        .map(str::to_string)
}
fn source_path(paths: &SourcePaths, owner: SourceOwner) -> Option<String> {
    paths
        .mapped_relative_path(owner)
        .map(|_| text(paths.repository_relative_path(owner)))
}
fn hex(value: u64) -> String {
    format!("0x{value:08x}")
}
fn span_values(spans: &[(u64, usize)]) -> Value {
    Value::Array(
        spans
            .iter()
            .map(|(start, size)| json!({"start":hex(*start),"end":hex(*start+*size as u64),"bytes":size}))
            .collect(),
    )
}
fn c_label(line: &str) -> Option<u64> {
    let value = line.trim().strip_prefix("AlchemyC_")?.strip_suffix(':')?;
    (value.len() == 8 && value.bytes().all(|byte| byte.is_ascii_hexdigit()))
        .then(|| u64::from_str_radix(value, 16).ok())
        .flatten()
}
fn space_size(line: &str) -> Option<usize> {
    let value = line.trim().strip_prefix(".space")?.trim();
    let radix = if value.starts_with("0x") { 16 } else { 10 };
    usize::from_str_radix(value.trim_start_matches("0x"), radix).ok()
}
fn overlay_placeholders(
    root: &Path,
    game: Game,
) -> Result<BTreeMap<SourceOwner, Vec<(u64, usize)>>, String> {
    let directory = root.join(game.target.overlay_dir());
    let mut files = std::fs::read_dir(&directory)
        .map_err(|error| format!("{}: {error}", directory.display()))?
        .filter_map(Result::ok)
        .filter_map(|entry| {
            let name = text(entry.file_name());
            name.strip_prefix("resource_")?
                .strip_suffix("_overlay.s")
                .map(|id| (format!("resource_{id}"), entry.path()))
        })
        .collect::<Vec<_>>();
    files.sort_by(|left, right| left.0.cmp(&right.0));
    let mut placeholders = BTreeMap::new();
    for (overlay, path) in files {
        let lines = std::fs::read_to_string(&path)
            .map_err(|error| format!("{}: {error}", path.display()))?
            .lines()
            .map(str::to_string)
            .collect::<Vec<_>>();
        for (start, line) in lines.iter().enumerate() {
            let Some(address) = c_label(line) else {
                continue;
            };
            let mut cursor = address;
            let mut spans = Vec::new();
            for line in &lines[start + 1..] {
                let text = line.trim();
                if c_label(text).is_some() {
                    break;
                }
                if let Some(size) = space_size(text) {
                    if size == 0 {
                        return Err(format!(
                            "zero overlay placeholder at {overlay}:{address:08x}"
                        ));
                    }
                    spans.push((cursor, size));
                    cursor += size as u64;
                    continue;
                }
                if !text.is_empty() && !text.starts_with(".L_") {
                    break;
                }
            }
            if spans.is_empty() {
                return Err(format!(
                    "empty overlay placeholder at {overlay}:{address:08x}"
                ));
            }
            let owner = SourceOwner::parse(&format!("{overlay}:{address:08x}"))?;
            if placeholders.insert(owner, spans).is_some() {
                return Err(format!("duplicate overlay placeholder {}", owner.id()));
            }
        }
    }
    Ok(placeholders)
}
fn semantic_overlay_spans(
    root: &Path,
    game: Game,
) -> Result<BTreeMap<SourceOwner, Vec<(u64, usize)>>, String> {
    let document = read_json::<Value>(&root.join(game.overlay_assembly_evidence()))?;
    if document["format"].as_u64() != Some(1) {
        return Err("overlay assembly evidence format differs".into());
    }
    let mut spans = BTreeMap::new();
    for row in document["regions"]
        .as_array()
        .ok_or("overlay assembly regions differ")?
    {
        if !matches!(
            row["retention"].as_str(),
            Some("keep_structured_asm" | "not_yet_c")
        ) {
            continue;
        }
        let overlay = row["overlay"]
            .as_str()
            .ok_or("overlay evidence lacks overlay")?;
        let start = value_u64(&row["start"], "overlay evidence start")?;
        let end = value_u64(&row["end"], "overlay evidence end")?;
        let size = end
            .checked_sub(start)
            .ok_or("invalid overlay evidence extent")? as usize;
        if size == 0 {
            return Err("empty overlay evidence extent".into());
        }
        let owner = SourceOwner::parse(&format!("{overlay}:{start:08x}"))?;
        spans
            .entry(owner)
            .or_insert_with(Vec::new)
            .push((start, size));
    }
    Ok(spans)
}
fn reject_overlay_evidence_overlap(
    placeholders: &BTreeMap<SourceOwner, Vec<(u64, usize)>>,
    semantic: &BTreeMap<SourceOwner, Vec<(u64, usize)>>,
) -> Result<(), String> {
    for (exact, c_spans) in placeholders {
        for (retained, asm_spans) in semantic
            .iter()
            .filter(|(owner, _)| owner.overlay_id() == exact.overlay_id())
        {
            if c_spans.iter().any(|(start, size)| {
                asm_spans.iter().any(|(other, extent)| {
                    *start < *other + *extent as u64 && *other < *start + *size as u64
                })
            }) {
                return Err(format!(
                    "{} retained assembly overlaps exact C {}",
                    retained.id(),
                    exact.id()
                ));
            }
        }
    }
    Ok(())
}
fn artifact(region: &Region) -> Value {
    json!({"source":region.source,"start":hex(region.address),"bytes":region.size,"composition":region.composition})
}
fn owner_inventory(
    root: &Path,
    game: Game,
    claimed_document: &Value,
    claimed: &[Region],
    assembly: &[Region],
    units: &TranslationUnits,
) -> Result<Value, String> {
    let paths = game.register(root)?;
    let registered = paths.registered_owners().collect::<BTreeSet<_>>();
    let members = inventory_members(game, units)?;
    if let Some(owner) = members.keys().find(|owner| !registered.contains(owner)) {
        return Err(format!(
            "reconstruction unit names unregistered owner {}",
            owner.id()
        ));
    }
    let claimed_symbols = claimed_symbols(claimed)?;
    let functions = linked_functions(claimed_document, &claimed_symbols)?;
    let assembly_starts = region_starts(assembly, "assembly")?;
    let placeholders = overlay_placeholders(root, game)?;
    let semantic = semantic_overlay_spans(root, game)?;
    reject_overlay_evidence_overlap(&placeholders, &semantic)?;
    let mut owners = Vec::new();
    let mut groups = BTreeMap::<String, usize>::new();
    let mut states = BTreeMap::<String, usize>::new();
    let mut known_extents = 0usize;
    for owner in &registered {
        let member = members.get(owner);
        let registered_source = source_path(&paths, *owner);
        let role = member.map_or("owner", |member| member.role);
        let alias = member.map_or_else(
            || {
                paths
                    .registered_name(*owner)
                    .unwrap_or_default()
                    .to_string()
            },
            |member| member.alias.clone(),
        );
        let unit = member.map(|member| {
            json!({"id":member.unit,"instance":member.instance,"role":member.role,"ordinal":member.ordinal,"declared_state":member.state})
        });
        let declared = member.is_some();
        let exact = member.map_or(registered_source.is_some(), |member| {
            member.state.unwrap_or("exact-c") == "exact-c"
        });
        let state = if exact { "exact-c" } else { "not-yet-c" };
        let (source, spans, extent_evidence, artifact_value) = match owner {
            SourceOwner::Main(address) => {
                let (region, spans) = if exact {
                    let function = functions
                        .get(&owner.legacy_name())
                        .ok_or_else(|| format!("{} has no linked function", owner.id()))?;
                    let differs = function.address != u64::from(*address)
                        || member.is_some_and(|member| function.size != member.extent);
                    if differs {
                        let detail = if declared {
                            "declared function extent"
                        } else {
                            "linked address"
                        };
                        return Err(format!("{} has a differing {detail}", owner.id()));
                    }
                    (function.artifact, vec![(function.address, function.size)])
                } else {
                    let region = assembly_starts
                        .get(&u64::from(*address))
                        .copied()
                        .ok_or_else(|| {
                            format!(
                                "{} has no {}assembly region",
                                owner.id(),
                                if declared { "declared " } else { "" }
                            )
                        })?;
                    if member.is_some_and(|member| region.size != member.extent) {
                        return Err(format!(
                            "{} has a differing declared assembly extent",
                            owner.id()
                        ));
                    }
                    (region, vec![(u64::from(*address), region.size)])
                };
                (
                    member.map_or_else(
                        || {
                            registered_source
                                .clone()
                                .unwrap_or_else(|| region.source.clone().unwrap_or_default())
                        },
                        |member| member.source.clone(),
                    ),
                    spans,
                    if declared {
                        "declared-reconstruction-unit"
                    } else if exact {
                        "linked-elf-symbol"
                    } else {
                        "assembly-manifest"
                    },
                    artifact(region),
                )
            }
            SourceOwner::Overlay { resource, .. } => {
                let spans = if exact {
                    placeholders
                        .get(owner)
                        .cloned()
                        .ok_or_else(|| format!("{} has no overlay placeholder", owner.id()))?
                } else {
                    semantic
                        .get(owner)
                        .cloned()
                        .ok_or_else(|| format!("{} has no overlay assembly evidence", owner.id()))?
                };
                if member.is_some_and(|member| {
                    spans.iter().map(|(_, size)| *size).sum::<usize>() != member.extent
                }) {
                    return Err(format!(
                        "{} has a differing declared overlay extent",
                        owner.id()
                    ));
                }
                let overlay = game.overlay_listing(&format!("resource_{resource:03x}"));
                let artifact = if exact || declared {
                    json!({"source":overlay,"composition":if exact{"overlay-placeholder"}else{"structured-overlay-assembly"},"overlapping_retained_evidence":semantic.get(owner).map(|spans|span_values(spans))})
                } else {
                    json!({"source":overlay,"composition":"structured-overlay-assembly"})
                };
                (
                    member.map_or_else(
                        || registered_source.clone().unwrap_or_else(|| overlay.clone()),
                        |member| member.source.clone(),
                    ),
                    spans,
                    if declared {
                        "declared-reconstruction-unit"
                    } else if exact {
                        "overlay-placeholder"
                    } else {
                        "overlay-assembly-evidence"
                    },
                    artifact,
                )
            }
        };
        let extent = spans.iter().map(|(_, size)| *size).sum::<usize>();
        if !spans.is_empty() {
            known_extents += 1;
        }
        *states.entry(state.into()).or_default() += 1;
        if let Some(group) = source_group(game, &source) {
            *groups.entry(group).or_default() += 1;
        }
        owners.push(json!({
            "id":owner.id(),"name":paths.registered_name(*owner),"address":hex(u64::from(owner.address())),
            "container":if owner.is_main(){json!({"kind":"main-rom","overlay":Value::Null})}else{json!({"kind":"overlay-image","overlay":owner.overlay_id()})},
            "registration":{"source_path":registered_source,"call_via":paths.registered_call_via(*owner).map(|value| hex(u64::from(value)))},
            "reconstruction_unit":unit,"production":{"state":state,"source":source,"source_group":source_group(game, &source),"extent_bytes":if spans.is_empty(){Value::Null}else{number(extent)},"extent_evidence":extent_evidence,"segments":span_values(&spans),"artifact":artifact_value},
            "role":role,"alias":alias
        }));
    }
    let units = units.units.iter().filter(|unit| unit.game == game.id()).map(|unit| {
        let members = unit.owners.iter().enumerate().map(|(ordinal, member)| json!({"owner":unit.source_owner(unit.image(), member.address).unwrap().id(),"role":"owner","ordinal":ordinal,"alias":member.canonical_name,"extent":member.extent,"declared_state":owner_state(member.state)})).chain(unit.local_symbols.iter().enumerate().map(|(ordinal, member)| json!({"owner":unit.source_owner(unit.image(), member.address).unwrap().id(),"role":"local-symbol","ordinal":ordinal,"alias":member.canonical_name,"extent":member.extent,"declared_state":Value::Null}))).collect::<Vec<_>>();
        let absolute_symbols = unit.absolute_symbols.iter().map(|(name, symbol)| json!({"name":name,"address":hex(symbol.address),"kind":absolute_kind(symbol.kind)})).collect::<Vec<_>>();
        let instances = unit.instances.keys().map(|image| {
            let members = unit.members_in(image).map(|member| json!({"owner":unit.source_owner(image, member.address).unwrap().id(),"role":if member.owner {"owner"} else {"local-symbol"},"alias":member.name,"extent":member.extent})).collect::<Vec<_>>();
            json!({"overlay":image,"members":members})
        }).collect::<Vec<_>>();
        json!({"id":unit.id,"game":unit.game,"source":unit.source,"container":if unit.overlay.is_none(){json!({"kind":"main-rom","overlay":Value::Null})}else{json!({"kind":"overlay-image","overlay":unit.overlay})},"production_composition_sections":unit.composition_sections(),"absolute_symbols":absolute_symbols,"members":members,"instances":instances})
    }).collect::<Vec<_>>();
    let auxiliary_regions = assembly.iter().filter(|region| !registered.contains(&SourceOwner::Main(region.address as u32))).map(|region| json!({"role":"non-owner-region","container":{"kind":"main-rom"},"address":hex(region.address),"run_address":region.run_address.map(hex),"extent":region.size,"source":region.source,"kind":region.kind,"origin":region.origin,"retention":region.retention,"confidence":region.confidence,"evidence":region.evidence})).collect::<Vec<_>>();
    let evidence = game.overlay_assembly_evidence();
    let auxiliary_overlay_regions = semantic
        .iter()
        .filter(|(owner, _)| !registered.contains(owner))
        .flat_map(|(owner, spans)| {
            let source = game.overlay_listing(&owner.overlay_id().unwrap_or_default());
            let evidence = evidence.clone();
            spans.iter().map(move |(start, size)| {
                json!({"role":"unregistered-retained-region","container":{"kind":"overlay-image","overlay":owner.overlay_id()},"address":hex(*start),"extent":size,"source":source,"retention":"not_yet_c","evidence":evidence})
            })
        })
        .collect::<Vec<_>>();
    let full = defaults(game.target.id);
    Ok(json!({
        "format":1,"kind":format!("{}-production-owner-inventory", game.id()),"scope":"derived production/retention inventory; source-paths is the sole name authority and no original translation-unit boundary is asserted",
        "target":game.target.id.as_str(),"identity_authority":game.identity_authority()?,"inputs":{"translation_units":game.translation_units(),"claimed_manifest":format!("{}/manifest.json", full.claimed_output),"asm_manifest":format!("{}/manifest.json", full.asm_output),"overlay_sources":game.overlay_listing("resource_*"),"overlay_assembly_evidence":evidence},
        "summary":{"registered":registered.len(),"main":registered.iter().filter(|owner| owner.is_main()).count(),"overlay":registered.iter().filter(|owner| !owner.is_main()).count(),"known_production_extents":known_extents,"complete_registered_identity_coverage":true,"states":states,"current_source_groups":groups},
        "reconstruction_units":units,"owners":owners,"auxiliary_main_assembly_regions":auxiliary_regions,"auxiliary_overlay_structured_assembly_regions":auxiliary_overlay_regions
    }))
}
fn validate_translation_units(
    root: &Path,
    game: Game,
    document: &Value,
    claimed: &[Region],
    assembly: &[Region],
) -> Result<(usize, RegisteredOwnerCoverage), String> {
    let units = game.units(root)?;
    let verification = document["verification"]
        .as_str()
        .ok_or("claimed manifest lacks byte-verification mode")?;
    let compiles = document["translation_unit_compiles"]
        .as_array()
        .ok_or("claimed manifest lacks translation-unit compile evidence")?;
    let main = units
        .units
        .iter()
        .filter(|unit| unit.game == game.id())
        .map(crate::compiler::translation_units::TranslationUnit::main_placement)
        .collect::<Result<Vec<_>, _>>()?
        .into_iter()
        .flatten()
        .collect::<Vec<_>>();
    let expected = main.iter().map(|unit| {
        let exact = unit.exact_owner_count();
        let retained = unit.owners.len() - exact;
        json!({"id":unit.id,"source":unit.source,"c_compiles":1,"composition":if unit.linked_whole(){"complete-tu-object"}else{"complete-tu-owner-slices"},"exact_owners":exact,"retained_owners":retained})
    }).collect::<Vec<_>>();
    if compiles != &expected {
        return Err("production TU compile records differ from manifest".into());
    }
    for unit in &main {
        let exact = unit.exact_owner_count();
        let mixed = !unit.linked_whole();
        let regions = claimed
            .iter()
            .filter(|region| region.translation_unit.as_deref() == Some(unit.id.as_str()))
            .collect::<Vec<_>>();
        let expected_regions = if exact == 0 {
            0
        } else if mixed {
            exact
        } else {
            1
        };
        let composition = if mixed {
            "complete-tu-owner-slice"
        } else {
            "complete-tu-object"
        };
        if regions.len() != expected_regions
            || regions.iter().any(|region| {
                region.composition.as_deref() != Some(composition)
                    || region.byte_verification.as_deref() != Some(verification)
            })
        {
            return Err(format!("{}: invalid exact-owner region metadata", unit.id));
        }
        if exact > 0 && !mixed {
            let (start, end) =
                unit.symbols()
                    .fold((u64::MAX, 0), |(start, end), (address, _, extent)| {
                        (
                            start.min(u64::from(address)),
                            end.max(u64::from(address) + extent as u64),
                        )
                    });
            if !regions[0].is_owner(start, (end - start) as usize) {
                return Err(format!("{}: complete TU object extent differs", unit.id));
            }
        }
        for owner in &unit.owners {
            let address = u64::from(owner.address);
            let present = match owner.state {
                OwnerState::ExactC if mixed => {
                    regions
                        .iter()
                        .filter(|region| region.is_owner(address, owner.extent))
                        .count()
                        == 1
                }
                OwnerState::ExactC => true,
                OwnerState::NotYetC => {
                    assembly
                        .iter()
                        .filter(|region| region.is_owner(address, owner.extent))
                        .count()
                        == 1
                }
            };
            if !present {
                return Err(format!(
                    "{}: {:08x} is absent from its production component",
                    unit.id, owner.address
                ));
            }
        }
    }
    if claimed
        .iter()
        .filter_map(|region| region.translation_unit.as_deref())
        .any(|id| !main.iter().any(|unit| unit.id == id))
    {
        return Err("claimed region names an unexpected translation unit".into());
    }
    let exports = document["main_symbol_exports"]
        .as_str()
        .ok_or("claimed manifest lacks main symbol exports")?;
    if std::fs::read(rooted(root, exports)).map_err(|error| format!("{exports}: {error}"))?
        != game.register(root)?.main_symbol_exports().as_bytes()
    {
        return Err("claimed main symbol exports differ from the owner register".into());
    }
    Ok((main.len(), registered_owner_coverage(root, game, &units)?))
}
pub fn reconstruction_progress(
    rom_size: usize,
    code: usize,
    assets: usize,
    retained: usize,
    debt: usize,
    unowned: usize,
) -> Result<ReconstructionProgress, String> {
    if rom_size == 0 {
        return Err("invalid byte reconstruction count".into());
    }
    let bytes = code + assets + retained;
    let remaining_bytes = debt + unowned;
    if bytes + remaining_bytes != rom_size {
        return Err("byte reconstruction count differs".into());
    }
    Ok(ReconstructionProgress {
        bytes,
        remaining_bytes,
        percent: round_percent(bytes, rom_size),
    })
}
fn round_percent(bytes: usize, total: usize) -> f64 {
    ((bytes as f64 * 10_000.0 / total as f64).round()) / 100.0
}
fn number(value: usize) -> Value {
    Value::Number(Number::from(value as u64))
}
fn percentage(value: f64) -> Value {
    if value.fract() == 0.0 {
        number(value as usize)
    } else {
        Value::Number(Number::from_f64(value).unwrap())
    }
}
fn measured(bytes: usize, total: usize) -> Value {
    json!({"status":"measured","bytes":bytes,"total_bytes":total,"remaining_bytes":total-bytes,"percent":percentage(round_percent(bytes,total))})
}
fn project_audit(
    rom_size: usize,
    source: usize,
    reconstructed: usize,
    code: usize,
    debt: usize,
    asset_regions: usize,
    asset_bytes: usize,
) -> Value {
    json!({
        "status":"audit_pending", "percent":Value::Null,
        "scoring":"withheld_until_all_dimensions_are_audited",
        "blockers":["asset_semantics","repository_organization"],
        "dimensions":{
            "source_ownership":measured(source,rom_size),
            "byte_reconstruction":measured(reconstructed,rom_size),
            "code_decompilation":measured(code,code+debt),
            "asset_semantics":{"status":"audit_pending","round_trip_regions":asset_regions,"round_trip_bytes":asset_bytes,"classified_regions":0,"classified_bytes":0,"unclassified_regions":asset_regions,"unclassified_bytes":asset_bytes,"classification_manifest":Value::Null},
            "repository_organization":{"status":"audit_pending","audit_manifest":Value::Null}
        }
    })
}
fn gap_values(gaps: &[GapRegion], prefix: &str, kind: &str) -> Value {
    Value::Array(gaps.iter().map(|gap| json!({"address":gap.address,"size":gap.size,"source":format!("{prefix}/{:08x}",gap.address),"kind":kind})).collect())
}
fn require_source_ownership(source_only: bool, unowned_bytes: usize) -> Result<(), String> {
    if !source_only && unowned_bytes != 0 {
        return Err(format!(
            "full build leaves {unowned_bytes} ROM bytes unowned; ROM fallback is forbidden"
        ));
    }
    Ok(())
}
fn place_regions(
    regions: &[Region],
    image: &[u8],
    image_base: Option<u64>,
    rom: Option<&[u8]>,
    rebuilt: &mut Option<Vec<u8>>,
    mask: &mut [u8],
    label: &str,
) -> Result<(), String> {
    for region in regions {
        let start = region
            .address
            .checked_sub(ROM_BASE)
            .ok_or_else(|| format!("{label} outside ROM image at 0x{:08x}", region.address))?
            as usize;
        let end = start
            .checked_add(region.size)
            .ok_or_else(|| format!("{label} outside ROM image at 0x{:08x}", region.address))?;
        if end > mask.len() || end <= start {
            return Err(format!(
                "{label} outside ROM image at 0x{:08x}",
                region.address
            ));
        }
        if mask[start..end].iter().any(|byte| *byte != 0) {
            return Err(format!(
                "{label} overlaps another source at 0x{:08x}",
                region.address
            ));
        }
        let source = if let Some(image_base) = image_base {
            let offset = region
                .address
                .checked_sub(image_base)
                .ok_or("compiled image region differs")? as usize;
            image
                .get(offset..offset + region.size)
                .ok_or("compiled image region differs")?
                .to_vec()
        } else {
            read(&region.output)?
        };
        if source.len() != region.size {
            return Err(format!("{label} size differs at 0x{:08x}", region.address));
        }
        if let Some(expected) = rom {
            if source != expected[start..end] {
                return Err(format!(
                    "{label} mismatch at 0x{:08x} (0x{:x} bytes)",
                    region.address, region.size
                ));
            }
        }
        if let Some(output) = rebuilt.as_mut() {
            output[start..end].copy_from_slice(&source);
        }
        mask[start..end].fill(1);
    }
    Ok(())
}
/// Builds the full ROM. The target's earlier full-build proof is withdrawn
/// before anything runs and again when the build fails, so only a build
/// that has just succeeded leaves one.
pub fn build(root: &Path, cwd: &Path, options: &Options) -> Result<String, String> {
    let target = target_for(options.target);
    crate::coverage::proof::withdraw_full_build(root, target)?;
    build_stages(root, cwd, options).or_else(|error| {
        crate::coverage::proof::withdraw_full_build(root, target).map_err(|failure| {
            format!("{error}\nthe earlier full-build proof was not withdrawn: {failure}")
        })?;
        Err(error)
    })
}
fn build_stages(root: &Path, cwd: &Path, options: &Options) -> Result<String, String> {
    let target = target_for(options.target);
    let game = Game::of(target);
    let progress_inputs = crate::coverage::proof::identity(root, target.id.as_str())?;
    if target.build_support != BuildSupport::Full {
        return Err(format!("{} is compile-only; run `make {}` until its edition link map, assembly, and assets are reconstructed", target.id, target.id));
    }
    let rom_path = if Path::new(&options.rom).is_absolute() {
        PathBuf::from(&options.rom)
    } else {
        cwd.join(&options.rom)
    };
    let rom = if options.source_only {
        None
    } else {
        let bytes =
            std::fs::read(&rom_path).map_err(|error| format!("{}: {error}", rom_path.display()))?;
        if bytes.len() != target.rom_size as usize {
            return Err(format!(
                "{} ROM must contain exactly {} bytes",
                target.id, target.rom_size
            ));
        }
        Some(bytes)
    };
    let mut rebuilt = rom.clone();
    let mut mask = vec![0u8; target.rom_size as usize];
    let claimed_dir = rooted(root, &options.claimed_output);
    let mut command = stage_command("claimed")?;
    command.extend(["--target".into(), target.id.to_string()]);
    if options.source_only {
        command.push("--source-only".into());
    } else {
        command.push(text(&rom_path));
    }
    command.extend([
        "--jobs".into(),
        options.jobs.to_string(),
        "--output".into(),
        text(&claimed_dir),
    ]);
    run_stage(root, &command)?;
    let claimed_document = read_json::<Value>(&claimed_dir.join("manifest.json"))?;
    let claimed_regions = regions(&claimed_document)?;
    let claimed_image = std::fs::read(claimed_dir.join("claimed.bin"))
        .map_err(|error| format!("claimed.bin: {error}"))?;
    let image_base = value_u64(&claimed_document["image_base"], "image base")?;
    place_regions(
        &claimed_regions,
        &claimed_image,
        Some(image_base),
        rom.as_deref(),
        &mut rebuilt,
        &mut mask,
        "source",
    )?;
    let mut asm_regions = Vec::new();
    let asm_dir = rooted(root, target.asm_dir);
    if asm_dir.exists() && has_assembly_sources(&asm_dir)? {
        let output = rooted(root, &options.asm_output);
        let mut command = stage_command("asm")?;
        command.extend(["--target".into(), target.id.to_string()]);
        if options.source_only {
            command.push("--source-only".into());
        } else {
            command.push(text(&rom_path));
        }
        command.extend(["--output".into(), text(&output)]);
        run_stage(root, &command)?;
        asm_regions = regions(&read_json::<Value>(&output.join("manifest.json"))?)?;
        validate_alignments(&claimed_regions, &asm_regions)?;
        place_regions(
            &asm_regions,
            &[],
            None,
            rom.as_deref(),
            &mut rebuilt,
            &mut mask,
            "assembly",
        )?;
    }
    let (translation_units, owner_coverage) = validate_translation_units(
        root,
        game,
        &claimed_document,
        &claimed_regions,
        &asm_regions,
    )?;
    let main_symbol_exports = claimed_document["main_symbol_exports"]
        .as_str()
        .ok_or("claimed manifest lacks main symbol exports")?;
    let accounting = assembly_accounting(&asm_regions)?;
    let mut asset_regions = Vec::new();
    let asset_manifest = rooted(root, &options.asset_manifest);
    if asset_manifest.exists() {
        let output = rooted(root, &options.asset_output);
        let mut command = stage_command("assets")?;
        command.extend(["--target".into(), target.id.to_string()]);
        if options.source_only {
            command.push("--source-only".into());
        } else {
            command.push(text(&rom_path));
        }
        command.extend([
            "--manifest".into(),
            text(&asset_manifest),
            "--output".into(),
            text(&output),
        ]);
        run_stage(root, &command)?;
        asset_regions = regions(&read_json::<Value>(&output.join("manifest.json"))?)?;
        place_regions(
            &asset_regions,
            &[],
            None,
            rom.as_deref(),
            &mut rebuilt,
            &mut mask,
            "asset",
        )?;
    }
    if let (Some(actual), Some(expected)) = (&rebuilt, &rom) {
        if actual != expected {
            return Err("full rebuild differs from reference ROM image".into());
        }
    }
    let output = rooted(root, &options.output);
    let source_bytes = mask.iter().map(|byte| *byte as usize).sum::<usize>();
    let code_bytes = value_u64(&claimed_document["claimed_bytes"], "claimed bytes")? as usize;
    let asm_bytes = asm_regions.iter().map(|region| region.size).sum::<usize>();
    let asset_bytes = asset_regions
        .iter()
        .map(|region| region.size)
        .sum::<usize>();
    if code_bytes + asm_bytes + asset_bytes != source_bytes {
        return Err("source component count differs".into());
    }
    if accounting.c_debt_bytes + accounting.retained_bytes != asm_bytes {
        return Err("assembly source accounting differs".into());
    }
    let gaps = unowned_regions(&mask, ROM_BASE)?;
    let unowned_bytes = gaps.iter().map(|gap| gap.size).sum::<usize>();
    if unowned_bytes != mask.len() - source_bytes {
        return Err("unowned coverage count differs".into());
    }
    let progress = reconstruction_progress(
        mask.len(),
        code_bytes,
        asset_bytes,
        accounting.retained_bytes,
        accounting.c_debt_bytes,
        unowned_bytes,
    )?;
    let unowned_path = sidecar_path(&output, "unowned.json")?;
    let fallback_path = sidecar_path(&output, "fallback.json")?;
    let inventory_path = sidecar_path(&output, "owner-inventory.json")?;
    // The unowned ranges are written before anything that needs a complete
    // image, so a build that fails for them names every one.
    write_canonical(
        &unowned_path,
        &json!({"format":1,"semantics":"source_ownership","verification":if options.source_only{"source_only"}else{"rom"},"rom_base":ROM_BASE,"rom_size":mask.len(),"regions":gap_values(&gaps,"unowned","unowned")}),
    )?;
    write_canonical(
        &fallback_path,
        &json!({"format":1,"semantics":if options.source_only{"compatibility_alias_for_unowned_ranges"}else{"private_rom_fallback"},"rom_base":ROM_BASE,"rom_size":mask.len(),"regions":gap_values(&gaps,"rom-fallback","rom_fallback")}),
    )?;
    require_source_ownership(options.source_only, unowned_bytes).map_err(|error| {
        format!(
            "{error}; its {} unowned ranges are listed in {}",
            gaps.len(),
            unowned_path
                .strip_prefix(root)
                .unwrap_or(&unowned_path)
                .display()
        )
    })?;
    let inventory = owner_inventory(
        root,
        game,
        &claimed_document,
        &claimed_regions,
        &asm_regions,
        &game.units(root)?,
    )?;
    let inventory_summary = inventory["summary"].clone();
    write_canonical(&inventory_path, &inventory)?;
    if let Some(parent) = output.parent() {
        std::fs::create_dir_all(parent)
            .map_err(|error| format!("{}: {error}", parent.display()))?;
    }
    if let Some(bytes) = &rebuilt {
        write(&output, bytes)?;
    }
    // Coverage's readers use the canonical stage locations. A custom build
    // must not certify unrelated manifests left in those locations, so only
    // a canonical build proves the main image or earns a receipt.
    let defaults = defaults(target.id);
    let canonical = [
        (&options.claimed_output, &defaults.claimed_output),
        (&options.asm_output, &defaults.asm_output),
        (&options.asset_manifest, &defaults.asset_manifest),
        (&options.asset_output, &defaults.asset_output),
        (&options.output, &defaults.output),
    ]
    .into_iter()
    .all(|(chosen, default)| rooted(root, chosen) == rooted(root, default));
    let main_image_proof = match rom.as_ref().filter(|_| canonical) {
        Some(rom) => json!(crate::coverage::proof::full_build_proof(
            root,
            target,
            &progress_inputs,
            rom
        )?),
        None => Value::Null,
    };
    let report = json!({
        "format":1,
        "target":target.id.to_string(),
        "compiler":target.compiler.as_str(),
        "rom_base":ROM_BASE,
        "rom_size":mask.len(),
        "code_regions":claimed_regions.len(),
        "code_bytes":code_bytes,
        "translation_units":translation_units,
        "declared_main_translation_units_strict":true,
        "registered_owner_coverage":{
            "authority":game.identity_authority()?,
            "scope":"registered-owner lower bound; does not assert original translation-unit boundaries",
            "registered":owner_coverage.registered,
            "registered_main":owner_coverage.registered_main,
            "registered_overlay":owner_coverage.registered_overlay,
            "declared":owner_coverage.declared,
            "declared_main":owner_coverage.declared_main,
            "declared_overlay":owner_coverage.declared_overlay,
            "missing":owner_coverage.missing,
            "unexpected":owner_coverage.unexpected,
            "complete":owner_coverage.missing == 0 && owner_coverage.unexpected == 0
        },
        "owner_inventory":{"path":inventory_path.to_string_lossy(),"summary":inventory_summary},
        "strict_translation_units":owner_coverage.missing == 0 && owner_coverage.unexpected == 0,
        "strict_translation_units_scope":"complete reconstruction-composition contract coverage; original translation units remain unknown",
        "main_symbol_exports":main_symbol_exports,
        "asm_regions":asm_regions.len(),
        "asm_bytes":asm_bytes,
        "asm_c_debt_regions":accounting.c_debt_regions,
        "asm_c_debt_bytes":accounting.c_debt_bytes,
        "asm_retained_structural_regions":accounting.retained_regions,
        "asm_retained_structural_bytes":accounting.retained_bytes,
        "asset_regions":asset_regions.len(),
        "asset_bytes":asset_bytes,
        "source_regions":claimed_regions.len() + asm_regions.len() + asset_regions.len(),
        "source_bytes":source_bytes,
        "project_completion":project_audit(
            mask.len(),
            source_bytes,
            progress.bytes,
            code_bytes,
            accounting.c_debt_bytes,
            asset_regions.len(),
            asset_bytes
        ),
        "byte_reconstruction_bytes":progress.bytes,
        "byte_reconstruction_remaining_bytes":progress.remaining_bytes,
        "byte_reconstruction_percent":percentage(progress.percent),
        "unowned_bytes":unowned_bytes,
        "unowned_regions":gaps.len(),
        "unowned_manifest":unowned_path.to_string_lossy(),
        "rom_fallback_bytes":unowned_bytes,
        "fallback_regions":gaps.len(),
        "fallback_manifest":fallback_path.to_string_lossy(),
        "rom_fallback_applicable":!options.source_only,
        "verification":if options.source_only {
            "source_only"
        } else {
            "rom"
        },
        "byte_identical":!options.source_only,
        "output":if options.source_only { Value::Null } else { json!(options.output) },
        "main_image_proof":main_image_proof,
    });
    write_canonical(&sidecar_path(&output, "json")?, &report)?;
    // The receipt needs the independently verified executable inventory,
    // and `--inventory` needs this build's proof. The inventory depends on
    // the reference ROM and this build's asset layout alone, so it is derived
    // again only when absent or when those changed; while it stays pending,
    // only the receipt is withheld.
    let mut receipt = String::new();
    if let Some(rom) = rom.as_ref().filter(|_| canonical) {
        if crate::coverage::pipeline::authoritative_inventory(root, target)?.is_none() {
            match crate::coverage::audit::refresh_inventory(root, target) {
                Ok(summary) => println!("{summary}"),
                Err(error) => eprintln!("executable audit failed: {error}"),
            }
        }
        if crate::coverage::pipeline::authoritative_inventory(root, target)?.is_some() {
            let tree = crate::coverage::tree::work_tree_at(root.to_path_buf());
            let credits = crate::coverage::pipeline::verified_credits(
                &crate::coverage::pipeline::BuildOptions {
                    target: target.id.to_string(),
                    exact: &tree,
                    recon: None,
                },
            )?;
            crate::coverage::proof::write(
                root,
                target.id.as_str(),
                rom,
                &progress_inputs,
                credits,
            )?;
        } else {
            receipt = " receipt=withheld (executable audit pending)".into();
        }
    }
    Ok(format!(
        "{} regions={} code={} asm={} assets={} source_bytes={} unowned_bytes={} asm_c_debt_bytes={} asm_retained_structural_bytes={} source_owned={} byte_identical={}{}{receipt}",
        if options.source_only { "source_only=True" } else { "identical=True" },
        claimed_regions.len() + asm_regions.len() + asset_regions.len(),
        claimed_regions.len(),
        asm_regions.len(),
        asset_regions.len(),
        source_bytes,
        unowned_bytes,
        accounting.c_debt_bytes,
        accounting.retained_bytes,
        if unowned_bytes == 0 { "yes" } else { "no" },
        if options.source_only { "not-applicable" } else { "yes" },
        if options.source_only { "".into() } else { format!(" rom_fallback_bytes={unowned_bytes}") }
    ))
}
pub fn self_test() -> Result<(), String> {
    let gaps = unowned_regions(&[1, 0, 0, 1, 0], ROM_BASE)?;
    if gaps
        .iter()
        .map(|gap| (gap.address, gap.size))
        .collect::<Vec<_>>()
        != [(ROM_BASE + 1, 2), (ROM_BASE + 4, 1)]
    {
        return Err("unowned coverage self-test failed".into());
    }
    let progress = reconstruction_progress(100, 10, 50, 5, 25, 10)?;
    if progress.bytes != 65 || progress.remaining_bytes != 35 || progress.percent != 65.0 {
        return Err("byte reconstruction progress self-test failed".into());
    }
    if reconstruction_progress(100, 10, 50, 5, 25, 9).is_ok() {
        return Err("incomplete byte reconstruction count was accepted".into());
    }
    if require_source_ownership(false, 1).is_ok() {
        return Err("ROM fallback was accepted by the full-build gate".into());
    }
    require_source_ownership(true, 1)?;
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn each_game_composes_from_its_own_registers_and_listings() {
        let tbs = Game::of(target_for(DecompTargetId::TbsEn));
        let tla = Game::of(target_for(DecompTargetId::TlaEn));
        for (game, id, recon, source) in [
            (tbs, "tbs", "recon/tbs", "games/THE BROKEN SEAL/SRC"),
            (tla, "tla", "recon/tla", "games/THE LOST AGE/SRC"),
        ] {
            assert_eq!(game.id(), id);
            assert_eq!(
                game.identity_authority().unwrap(),
                format!("{recon}/source-paths.json")
            );
            assert_eq!(
                game.translation_units(),
                format!("{recon}/translation-units.json")
            );
            assert_eq!(
                game.overlay_assembly_evidence(),
                format!("{recon}/semantic/overlay-assembly.json")
            );
            assert_eq!(
                game.overlay_listing("resource_649"),
                format!("{recon}/raw/overlays/resource_649_overlay.s")
            );
            assert_eq!(
                source_group(game, &format!("{source}/GRAPHICS/TILE/DRAW_MAP.S")).as_deref(),
                Some("GRAPHICS/TILE")
            );
        }
        // One game's tree is not another's source group.
        assert_eq!(
            source_group(tla, "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/DRAW_MAP.S"),
            None
        );
    }

    #[test]
    fn owner_inventory_reports_unit_and_instance_for_every_member() {
        use crate::compiler::translation_units::fixture::{Repository, FIND, REDRAW};
        let repository = Repository::new();
        let units = repository.load().unwrap();
        let game = Game::of(target_for(DEFAULT_TARGET));
        let members = inventory_members(game, &units).unwrap();
        assert_eq!(members.len(), 6);
        let member = |id: &str| &members[&SourceOwner::parse(id).unwrap()];
        for (id, instance, ordinal, alias, extent) in [
            ("resource_3bf:0200034c", None, 0, FIND, 296),
            ("resource_3bf:020008c0", None, 1, REDRAW, 284),
            (
                "resource_389:020008c0",
                Some("resource_389"),
                1,
                REDRAW,
                284,
            ),
            ("resource_39b:02000630", Some("resource_39b"), 0, FIND, 296),
            (
                "resource_39b:02000ba4",
                Some("resource_39b"),
                1,
                REDRAW,
                284,
            ),
        ] {
            let member = member(id);
            assert_eq!(member.unit, "staged-actor", "{id}");
            assert_eq!(member.instance.as_deref(), instance, "{id}");
            assert_eq!((member.role, member.ordinal), ("owner", ordinal), "{id}");
            assert_eq!(
                (member.alias.as_str(), member.extent),
                (alias, extent),
                "{id}"
            );
            assert_eq!(member.state, Some("exact-c"), "{id}");
        }
    }
    /// A full build withdraws the proof an earlier build left before it runs
    /// and again when it fails, so a failed build never leaves that proof
    /// standing, in either game, nor does a build of an edition without a
    /// supported full build.
    #[test]
    fn a_failed_full_build_leaves_no_earlier_proof() {
        use crate::coverage::proof::{full_build, full_build_fixture, fully_buildable};
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        for id in [DecompTargetId::TbsEn, DecompTargetId::TlaEn] {
            let target = target_for(id);
            full_build_fixture(root, target, &json!({"regions": []}));
            assert!(full_build(root, target).is_ok(), "{id}");
            let options = Options {
                rom: "roms/absent.gba".into(),
                ..defaults(target.id)
            };
            let error = build(root, root, &options).unwrap_err();
            assert!(error.contains("absent.gba"), "{id}: {error}");
            let reason = full_build(root, target).map(|_| ()).unwrap_err();
            assert!(
                reason.contains(&format!("cannot read out/{id}/full/rebuilt.json")),
                "{reason}"
            );
            assert!(!root.join(format!("out/{id}/full/rebuilt.gba")).exists());
        }

        let edition = target_for(DecompTargetId::TlaJa);
        full_build_fixture(root, fully_buildable(edition), &json!({"regions": []}));
        let error = build(root, root, &defaults(edition.id)).unwrap_err();
        assert!(error.contains("compile-only"), "{error}");
        for artifact in ["rebuilt.json", "rebuilt.gba"] {
            assert!(!root.join("out/tla-ja/full").join(artifact).exists());
        }
    }

    /// A full build that leaves any ROM byte without a source fails: it
    /// writes the ranges it could not own, never a rebuilt ROM or a proof,
    /// so the game's main image and DONE stay pending.
    #[test]
    fn unowned_bytes_fail_the_build_after_naming_them() {
        let mask = [1, 1, 0, 0, 1, 0];
        let gaps = unowned_regions(&mask, ROM_BASE).unwrap();
        assert_eq!(
            gaps.iter()
                .map(|gap| (gap.address, gap.size))
                .collect::<Vec<_>>(),
            [(ROM_BASE + 2, 2), (ROM_BASE + 5, 1)]
        );
        let error = require_source_ownership(false, 3).unwrap_err();
        assert!(error.contains("leaves 3 ROM bytes unowned"), "{error}");
        require_source_ownership(false, 0).unwrap();
    }
    #[test]
    fn output_sidecars_preserve_multi_dot_stems() {
        let output = Path::new("out/custom/rebuilt.debug.gba");
        assert_eq!(
            sidecar_path(output, "json").unwrap(),
            Path::new("out/custom/rebuilt.debug.json")
        );
        assert_eq!(
            sidecar_path(output, "owner-inventory.json").unwrap(),
            Path::new("out/custom/rebuilt.debug.owner-inventory.json")
        );
        assert_eq!(
            sidecar_path(Path::new("out/rebuilt.gba"), "unowned.json").unwrap(),
            Path::new("out/rebuilt.unowned.json")
        );
    }
}
