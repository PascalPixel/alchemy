//! Compose the claimed C, retained assembly, and asset outputs into the full ROM.

use std::collections::BTreeSet;
use std::path::{Path, PathBuf};
use std::process::{Command, Stdio};

use canonical_json::canonical_json;
use decomp_targets::{parse_decomp_target, target_for, DecompTargetId, DEFAULT_TARGET};
use serde_json::{json, Map, Number, Value};

const ROM_BASE: u64 = 0x0800_0000;

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
}

pub fn repository_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("build-full lives under tools-rs")
        .to_path_buf()
}

fn default_jobs() -> usize {
    std::thread::available_parallelism()
        .map_or(1, usize::from)
        .min(16)
}

fn defaults(target_id: DecompTargetId) -> Options {
    let target = target_for(target_id);
    let full = Path::new(target.output_dir).join("full");
    Options {
        target: target_id,
        rom: target.rom.into(),
        source_only: false,
        output: full.join("rebuilt.gba").to_string_lossy().into_owned(),
        claimed_output: full.join("claimed").to_string_lossy().into_owned(),
        asm_output: full.join("asm").to_string_lossy().into_owned(),
        asset_manifest: target.asset_manifest.into(),
        asset_output: full.join("assets").to_string_lossy().into_owned(),
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

fn rooted(root: &Path, value: &str) -> PathBuf {
    let path = Path::new(value);
    if path.is_absolute() {
        path.to_path_buf()
    } else {
        root.join(path)
    }
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
        if kind.is_file() && entry.path().extension().is_some_and(|ext| ext == "s") {
            return Ok(true);
        }
        if kind.is_dir() && has_assembly_sources(&entry.path())? {
            return Ok(true);
        }
    }
    Ok(false)
}

fn run(root: &Path, command: &[String]) -> Result<(), String> {
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

fn read_json(path: &Path) -> Result<Value, String> {
    let data = std::fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?;
    serde_json::from_slice(&data).map_err(|error| format!("{}: {error}", path.display()))
}

fn value_u64(value: &Value, label: &str) -> Result<u64, String> {
    value
        .as_u64()
        .or_else(|| value.as_str()?.parse().ok())
        .ok_or_else(|| format!("invalid {label}"))
}

fn regions(document: &Value) -> Result<Vec<Region>, String> {
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
                kind: item["kind"].as_str().map(str::to_string),
                retention: item["retention"].as_str().map(str::to_string),
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
    let retained = [
        "keep_asm",
        "keep_structured_asm",
        "adjacent_section_alignment",
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

fn write_json(path: &Path, value: Value) -> Result<(), String> {
    std::fs::write(path, format!("{}\n", canonical_json(&value)))
        .map_err(|error| format!("{}: {error}", path.display()))
}

fn gap_values(gaps: &[GapRegion], prefix: &str, kind: &str) -> Value {
    Value::Array(gaps.iter().map(|gap| json!({"address":gap.address,"size":gap.size,"source":format!("{prefix}/{:08x}",gap.address),"kind":kind})).collect())
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
            std::fs::read(&region.output)
                .map_err(|error| format!("{}: {error}", region.output.display()))?
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

pub fn build(root: &Path, cwd: &Path, options: &Options) -> Result<String, String> {
    let target = target_for(options.target);
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
    let mut command = vec![
        root.join("tools-rs/build-claimed/target/release/build-claimed")
            .to_string_lossy()
            .into_owned(),
        "--target".into(),
        target.id.to_string(),
    ];
    if options.source_only {
        command.push("--source-only".into());
    } else {
        command.push(rom_path.to_string_lossy().into_owned());
    }
    command.extend([
        "--jobs".into(),
        options.jobs.to_string(),
        "--output".into(),
        claimed_dir.to_string_lossy().into_owned(),
    ]);
    run(root, &command)?;
    let claimed_document = read_json(&claimed_dir.join("manifest.json"))?;
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
        let mut command = vec![root
            .join("tools-rs/build-asm/target/release/build-asm")
            .to_string_lossy()
            .into_owned()];
        if options.source_only {
            command.push("--source-only".into());
        } else {
            command.push(rom_path.to_string_lossy().into_owned());
        }
        command.extend(["--output".into(), output.to_string_lossy().into_owned()]);
        run(root, &command)?;
        asm_regions = regions(&read_json(&output.join("manifest.json"))?)?;
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
    let accounting = assembly_accounting(&asm_regions)?;

    let mut asset_regions = Vec::new();
    let asset_manifest = rooted(root, &options.asset_manifest);
    if asset_manifest.exists() {
        let output = rooted(root, &options.asset_output);
        let mut command = vec!["bun".into(), "tools/make/build_assets.ts".into()];
        if options.source_only {
            command.push("--source-only".into());
        } else {
            command.push(rom_path.to_string_lossy().into_owned());
        }
        command.extend([
            "--manifest".into(),
            asset_manifest.to_string_lossy().into_owned(),
            "--output".into(),
            output.to_string_lossy().into_owned(),
        ]);
        run(root, &command)?;
        asset_regions = regions(&read_json(&output.join("manifest.json"))?)?;
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
    if let Some(parent) = output.parent() {
        std::fs::create_dir_all(parent)
            .map_err(|error| format!("{}: {error}", parent.display()))?;
    }
    if let Some(bytes) = &rebuilt {
        std::fs::write(&output, bytes).map_err(|error| format!("{}: {error}", output.display()))?;
    }

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

    let report_base = output.with_extension("");
    let unowned_path = PathBuf::from(format!("{}.unowned.json", report_base.to_string_lossy()));
    let fallback_path = PathBuf::from(format!("{}.fallback.json", report_base.to_string_lossy()));
    write_json(
        &unowned_path,
        json!({"format":1,"semantics":"source_ownership","verification":if options.source_only{"source_only"}else{"rom"},"rom_base":ROM_BASE,"rom_size":mask.len(),"regions":gap_values(&gaps,"unowned","unowned")}),
    )?;
    write_json(
        &fallback_path,
        json!({"format":1,"semantics":if options.source_only{"compatibility_alias_for_unowned_ranges"}else{"private_rom_fallback"},"rom_base":ROM_BASE,"rom_size":mask.len(),"regions":gap_values(&gaps,"rom-fallback","rom_fallback")}),
    )?;

    let mut report = Map::new();
    macro_rules! field {
        ($name:expr,$value:expr) => {
            report.insert($name.into(), $value);
        };
    }
    field!("format", number(1));
    field!("target", json!(target.id.to_string()));
    field!("compiler", json!(target.compiler.to_string()));
    field!("rom_base", json!(ROM_BASE));
    field!("rom_size", number(mask.len()));
    field!("code_regions", number(claimed_regions.len()));
    field!("code_bytes", number(code_bytes));
    field!("asm_regions", number(asm_regions.len()));
    field!("asm_bytes", number(asm_bytes));
    field!("asm_c_debt_regions", number(accounting.c_debt_regions));
    field!("asm_c_debt_bytes", number(accounting.c_debt_bytes));
    field!(
        "asm_retained_structural_regions",
        number(accounting.retained_regions)
    );
    field!(
        "asm_retained_structural_bytes",
        number(accounting.retained_bytes)
    );
    field!("asset_regions", number(asset_regions.len()));
    field!("asset_bytes", number(asset_bytes));
    field!(
        "source_regions",
        number(claimed_regions.len() + asm_regions.len() + asset_regions.len())
    );
    field!("source_bytes", number(source_bytes));
    field!(
        "project_completion",
        project_audit(
            mask.len(),
            source_bytes,
            progress.bytes,
            code_bytes,
            accounting.c_debt_bytes,
            asset_regions.len(),
            asset_bytes
        )
    );
    field!("byte_reconstruction_bytes", number(progress.bytes));
    field!(
        "byte_reconstruction_remaining_bytes",
        number(progress.remaining_bytes)
    );
    field!("byte_reconstruction_percent", percentage(progress.percent));
    field!("total_decompilation_bytes", number(progress.bytes));
    field!(
        "total_decompilation_remaining_bytes",
        number(progress.remaining_bytes)
    );
    field!("total_decompilation_percent", percentage(progress.percent));
    field!(
        "total_decompilation_semantics",
        json!("deprecated_alias_for_byte_reconstruction")
    );
    field!("unowned_bytes", number(unowned_bytes));
    field!("unowned_regions", number(gaps.len()));
    field!("unowned_manifest", json!(unowned_path.to_string_lossy()));
    field!("rom_fallback_bytes", number(unowned_bytes));
    field!("fallback_regions", number(gaps.len()));
    field!("fallback_manifest", json!(fallback_path.to_string_lossy()));
    field!("rom_fallback_applicable", json!(!options.source_only));
    field!(
        "verification",
        json!(if options.source_only {
            "source_only"
        } else {
            "rom"
        })
    );
    field!("byte_identical", json!(!options.source_only));
    field!(
        "output",
        if options.source_only {
            Value::Null
        } else {
            json!(options.output)
        }
    );
    write_json(
        &PathBuf::from(format!("{}.json", report_base.to_string_lossy())),
        Value::Object(report),
    )?;

    Ok(format!("{} regions={} code={} asm={} assets={} source_bytes={} unowned_bytes={} asm_c_debt_bytes={} asm_retained_structural_bytes={} source_owned={} byte_identical={}{}",
        if options.source_only{"source_only=True"}else{"identical=True"}, claimed_regions.len()+asm_regions.len()+asset_regions.len(), claimed_regions.len(), asm_regions.len(), asset_regions.len(), source_bytes, unowned_bytes, accounting.c_debt_bytes, accounting.retained_bytes,
        if unowned_bytes==0{"yes"}else{"no"}, if options.source_only{"not-applicable"}else{"yes"}, if options.source_only{"".into()}else{format!(" rom_fallback_bytes={unowned_bytes}")}))
}

pub fn self_test() -> Result<(), String> {
    let gaps = unowned_regions(&[1, 0, 0, 1, 0], ROM_BASE)?;
    if gaps
        != [
            GapRegion {
                address: ROM_BASE + 1,
                size: 2,
            },
            GapRegion {
                address: ROM_BASE + 4,
                size: 1,
            },
        ]
    {
        return Err("unowned coverage self-test failed".into());
    }
    let progress = reconstruction_progress(100, 10, 50, 5, 25, 10)?;
    if progress
        != (ReconstructionProgress {
            bytes: 65,
            remaining_bytes: 35,
            percent: 65.0,
        })
    {
        return Err("byte reconstruction progress self-test failed".into());
    }
    if reconstruction_progress(100, 10, 50, 5, 25, 9).is_ok() {
        return Err("incomplete byte reconstruction count was accepted".into());
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    fn args(values: &[&str]) -> Vec<String> {
        values.iter().map(|value| value.to_string()).collect()
    }
    #[test]
    fn runtime_self_test() {
        self_test().unwrap();
    }
    #[test]
    fn arguments_follow_target_defaults() {
        let ParseOutcome::Run(options) =
            parse_args(&args(&["--target=gs2-en", "--source-only", "--jobs", "3"])).unwrap()
        else {
            panic!()
        };
        assert_eq!(options.target, DecompTargetId::Gs2En);
        assert_eq!(options.output, "out/gs2-en/full/rebuilt.gba");
        assert_eq!(options.jobs, 3);
    }
    #[test]
    fn positional_rom_is_rejected_in_source_only() {
        assert!(parse_args(&args(&["--source-only", "x.gba"])).is_err());
    }
}
