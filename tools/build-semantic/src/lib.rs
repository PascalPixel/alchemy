//! Native semantic-C validation and compilation.

use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::atomic::{AtomicU64, Ordering};

use alchemy_plan::plan::{source_to_assembly_plan, SourceToAssemblyPlanOptions};
use alchemy_routing::routing::CompilerTarget;
use serde::Deserialize;
use serde_json::Value;

#[derive(Debug, Clone, Deserialize)]
struct OverlayFunction {
    overlay: String,
    entry: u64,
    span_bytes: u64,
}

#[derive(Debug, Deserialize)]
struct OverlayInventory {
    functions: Vec<OverlayFunction>,
}

#[derive(Debug, Clone, Deserialize)]
struct ManualRegion {
    overlay: String,
    entry: String,
    span_bytes: u64,
    evidence: String,
}

#[derive(Debug, Deserialize)]
struct ManualDocument {
    #[allow(dead_code)]
    format: u64,
    manual_regions: Vec<ManualRegion>,
}

#[derive(Debug, Clone, Deserialize)]
pub struct MainManifestRegion {
    pub address: u64,
    pub size: u64,
    pub retention: String,
}

#[derive(Debug, Clone, Deserialize)]
pub struct MainClaimedRegion {
    pub address: u64,
    pub size: u64,
    pub source: String,
}

#[derive(Debug, Clone, Deserialize)]
pub struct MainExecutableRange {
    pub address: String,
    pub size: u64,
}

#[derive(Debug, Clone, Deserialize)]
pub struct MainSemanticOwner {
    pub entry: String,
    pub evidence: String,
    pub executable_ranges: Vec<MainExecutableRange>,
}

#[derive(Debug, Deserialize)]
struct MainOwnerDocument {
    format: u64,
    main_owners: Vec<MainSemanticOwner>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Range {
    pub address: u64,
    pub size: u64,
}

#[derive(Debug, Clone)]
pub struct ValidatedMainSemanticOwner {
    pub entry: u64,
    pub evidence: String,
    pub executable_ranges: Vec<Range>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct BuildReport {
    pub sources: u64,
    pub source_bytes: u64,
    pub reviewed_bytes: u64,
    pub outside_executable_bytes: u64,
    pub semantic_bytes: u64,
    pub main_semantic_bytes: u64,
    pub overlay_semantic_bytes: u64,
    pub expressed_bytes: u64,
    pub executable_bytes: u64,
}

#[derive(Debug, Clone)]
struct Identity {
    kind: Kind,
    address: u64,
    owner: String,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Kind {
    Main,
    Overlay,
}

#[derive(Debug)]
struct Admitted {
    overlay: String,
    ranges: Vec<Range>,
    source: PathBuf,
}

pub fn repository_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("build-semantic lives under tools")
        .to_path_buf()
}

fn parse_address(value: &str, field: &str) -> Result<u64, String> {
    let Some(hex) = value.strip_prefix("0x").or_else(|| value.strip_prefix("0X")) else {
        return Err(format!("{field} must be a hexadecimal address"));
    };
    if hex.is_empty() || !hex.bytes().all(|byte| byte.is_ascii_hexdigit()) {
        return Err(format!("{field} must be a hexadecimal address"));
    }
    u64::from_str_radix(hex, 16).map_err(|_| format!("{field} is not a safe integer"))
}

fn contains(outer: &Range, inner: &Range) -> bool {
    outer.address <= inner.address
        && inner.address.checked_add(inner.size).is_some_and(|end| {
            outer.address.checked_add(outer.size).is_some_and(|outer_end| end <= outer_end)
        })
}

fn overlaps(left: &Range, right: &Range) -> bool {
    left.address < right.address.saturating_add(right.size)
        && right.address < left.address.saturating_add(left.size)
}

pub fn validate_main_semantic_owners(
    root: &Path,
    owners: &[MainSemanticOwner],
    manifest_regions: &[MainManifestRegion],
    claimed_regions: &[MainClaimedRegion],
    source: &str,
) -> Result<Vec<ValidatedMainSemanticOwner>, String> {
    let mut validated: Vec<ValidatedMainSemanticOwner> = Vec::new();
    let mut entries: Vec<u64> = Vec::new();
    for (owner_index, owner) in owners.iter().enumerate() {
        let owner_field = format!("{source} owner {owner_index}");
        let entry = parse_address(&owner.entry, &format!("{owner_field} entry"))?;
        if entries.contains(&entry) {
            return Err(format!("{source} repeats entry {}", owner.entry));
        }
        entries.push(entry);
        if owner.evidence.trim().is_empty() {
            return Err(format!("{owner_field} has empty evidence"));
        }
        if owner.executable_ranges.is_empty() {
            return Err(format!("{owner_field} has no executable ranges"));
        }
        let mut ranges = Vec::new();
        for (range_index, range) in owner.executable_ranges.iter().enumerate() {
            let range_field = format!("{owner_field} range {range_index}");
            let address = parse_address(&range.address, &format!("{range_field} address"))?;
            if range.size == 0 || address.checked_add(range.size).is_none() {
                return Err(format!("{range_field} size must be a positive safe integer"));
            }
            let candidate = Range { address, size: range.size };
            let in_manifest = manifest_regions.iter().any(|item| {
                contains(&Range { address: item.address, size: item.size }, &candidate)
            });
            let exact_name = owner.entry.trim_start_matches("0x").trim_start_matches("0X");
            let exact_path = root.join("exact").join(format!("{exact_name}.c"));
            let noncanonical = read_optional(&exact_path)?
                .is_some_and(|text| !canonical_c_source(&text));
            let expected = format!("exact/{}.c", exact_name.to_ascii_lowercase());
            let in_claim = noncanonical && claimed_regions.iter().any(|item| {
                item.source.to_ascii_lowercase() == expected
                    && contains(&Range { address: item.address, size: item.size }, &candidate)
            });
            if !in_manifest && !in_claim {
                return Err(format!(
                    "{range_field} is not fully contained in an assembly-manifest row or its same-entry noncanonical claimed-C row"
                ));
            }
            ranges.push(candidate);
        }
        ranges.sort_by_key(|range| range.address);
        if !ranges.iter().any(|range| {
            range.address <= entry && entry < range.address.saturating_add(range.size)
        }) {
            return Err(format!("{owner_field} executable ranges do not include its entry"));
        }
        if ranges.windows(2).any(|pair| overlaps(&pair[0], &pair[1])) {
            return Err(format!("{owner_field} has overlapping executable ranges"));
        }
        if validated.iter().any(|prior| {
            ranges.iter().any(|range| prior.executable_ranges.iter().any(|other| overlaps(range, other)))
        }) {
            return Err(format!("{owner_field} overlaps another tracked main owner"));
        }
        validated.push(ValidatedMainSemanticOwner {
            entry,
            evidence: owner.evidence.clone(),
            executable_ranges: ranges,
        });
    }
    Ok(validated)
}

fn read_optional(path: &Path) -> Result<Option<String>, String> {
    match std::fs::read(path) {
        Ok(bytes) => Ok(Some(String::from_utf8_lossy(&bytes).into_owned())),
        Err(error) if error.kind() == std::io::ErrorKind::NotFound => Ok(None),
        Err(error) => Err(format!("{}: {error}", path.display())),
    }
}

fn read_json<T: for<'de> Deserialize<'de>>(path: &Path) -> Result<T, String> {
    let bytes = std::fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?;
    serde_json::from_slice(&bytes).map_err(|error| format!("{}: {error}", path.display()))
}

fn sources_below(directory: &Path) -> Result<Vec<PathBuf>, String> {
    if !directory.exists() {
        return Ok(Vec::new());
    }
    let mut result = Vec::new();
    let listing = std::fs::read_dir(directory)
        .map_err(|error| format!("{}: {error}", directory.display()))?;
    for entry in listing {
        let entry = entry.map_err(|error| format!("{}: {error}", directory.display()))?;
        let path = entry.path();
        let kind = entry.file_type().map_err(|error| format!("{}: {error}", path.display()))?;
        if kind.is_dir() {
            result.extend(sources_below(&path)?);
        } else if kind.is_file() && path.extension().is_some_and(|extension| extension == "c") {
            result.push(path);
        }
    }
    result.sort();
    Ok(result)
}

fn relative(root: &Path, path: &Path) -> String {
    path.strip_prefix(root).unwrap_or(path).to_string_lossy().into_owned()
}

fn validate_source(root: &Path, source: &Path) -> Result<Identity, String> {
    let name = source.file_name().and_then(|name| name.to_str()).unwrap_or_default();
    let stem = name.strip_suffix(".c").unwrap_or("");
    let (kind, address_text, owner) = if stem.len() == 8
        && stem.starts_with("08")
        && stem.bytes().all(|byte| byte.is_ascii_hexdigit())
    {
        (Kind::Main, stem.to_ascii_lowercase(), "main".to_string())
    } else if let Some((owner, address)) = stem.rsplit_once("_c_") {
        if !owner.starts_with("resource_")
            || address.len() != 8
            || !address.bytes().all(|byte| byte.is_ascii_hexdigit())
        {
            return Err(format!(
                "semantic C source must use its eight-digit address: {}",
                relative(root, source)
            ));
        }
        (Kind::Overlay, address.to_ascii_lowercase(), owner.to_string())
    } else {
        return Err(format!(
            "semantic C source must use its eight-digit address: {}",
            relative(root, source)
        ));
    };
    let symbol = format!("Func_{address_text}");
    let text = std::fs::read(source)
        .map(|bytes| String::from_utf8_lossy(&bytes).into_owned())
        .map_err(|error| format!("{}: {error}", source.display()))?;
    if !defines_function(&text, &symbol) {
        return Err(format!("{} does not define {symbol}", relative(root, source)));
    }
    if inline_assembly(&text) {
        return Err(format!("{} contains inline assembly", relative(root, source)));
    }
    if ["M2C_ERROR", "M2C_UNK", "GLOBAL_ASM"].iter().any(|word| contains_word(&text, word)) {
        return Err(format!(
            "{} contains an unresolved decompiler construct",
            relative(root, source)
        ));
    }
    Ok(Identity {
        kind,
        address: u64::from_str_radix(&address_text, 16).expect("validated hex"),
        owner,
    })
}

fn identifier(byte: u8) -> bool {
    byte.is_ascii_alphanumeric() || byte == b'_'
}

fn contains_word(text: &str, word: &str) -> bool {
    text.match_indices(word).any(|(at, _)| {
        (at == 0 || !identifier(text.as_bytes()[at - 1]))
            && (at + word.len() == text.len() || !identifier(text.as_bytes()[at + word.len()]))
    })
}

fn defines_function(text: &str, symbol: &str) -> bool {
    for (at, _) in text.match_indices(symbol) {
        if (at > 0 && identifier(text.as_bytes()[at - 1]))
            || (at + symbol.len() < text.len() && identifier(text.as_bytes()[at + symbol.len()]))
        {
            continue;
        }
        let rest = &text[at + symbol.len()..];
        let rest = rest.trim_start();
        if !rest.starts_with('(') {
            continue;
        }
        let mut depth = 0usize;
        let mut close = None;
        for (index, character) in rest.char_indices() {
            match character {
                '(' => depth += 1,
                ')' => {
                    depth -= 1;
                    if depth == 0 {
                        close = Some(index);
                        break;
                    }
                }
                _ => {}
            }
        }
        let Some(close) = close else { continue };
        let arguments = &rest[1..close];
        if arguments.contains(';') || arguments.contains('{') || arguments.contains('}') {
            continue;
        }
        if rest[close + 1..].trim_start().starts_with('{') {
            return true;
        }
    }
    false
}

fn inline_assembly(text: &str) -> bool {
    for spelling in ["__asm__", "asm"] {
        for (at, _) in text.match_indices(spelling) {
            if (at > 0 && identifier(text.as_bytes()[at - 1]))
                || (at + spelling.len() < text.len()
                    && identifier(text.as_bytes()[at + spelling.len()]))
            {
                continue;
            }
            let rest = text[at + spelling.len()..].trim_start();
            if rest.starts_with('(') || rest.starts_with('{') {
                return true;
            }
        }
    }
    false
}

fn canonical_c_source(text: &str) -> bool {
    !inline_assembly(text) && !text.contains(".incbin") && !contains_word(text, "M2C_ERROR")
}

fn checked(command: &[String], cwd: &Path) -> Result<(), String> {
    let Some(program) = command.first() else {
        return Err("empty compiler command".to_string());
    };
    let output = Command::new(program)
        .args(&command[1..])
        .current_dir(cwd)
        .output()
        .map_err(|error| format!("{program}: {error}"))?;
    if output.status.success() {
        return Ok(());
    }
    let detail = if output.stderr.is_empty() { &output.stdout } else { &output.stderr };
    let name = Path::new(program).file_name().and_then(|name| name.to_str()).unwrap_or(program);
    let detail = String::from_utf8_lossy(detail);
    let detail = detail.trim();
    Err(if detail.is_empty() {
        format!("{name} failed")
    } else {
        format!("{name} failed: {detail}")
    })
}

fn value_u64(value: &Value, field: &str) -> Result<u64, String> {
    value.as_u64().ok_or_else(|| format!("{field} must be an unsigned integer"))
}

fn intervals(value: &Value, field: &str) -> Result<Vec<Range>, String> {
    let list = value.as_array().ok_or_else(|| format!("{field} must be an array"))?;
    list.iter().enumerate().map(|(index, interval)| {
        let start = value_u64(&interval["start"], &format!("{field} {index} start"))?;
        let end = value_u64(&interval["end"], &format!("{field} {index} end"))?;
        if end <= start {
            return Err(format!("{field} {index} has a non-positive span"));
        }
        Ok(Range { address: start, size: end - start })
    }).collect()
}

fn union_intervals(mut ranges: Vec<Range>) -> Vec<Range> {
    ranges.sort_by_key(|range| (range.address, range.size));
    let mut out: Vec<Range> = Vec::new();
    for range in ranges {
        if let Some(last) = out.last_mut() {
            let last_end = last.address + last.size;
            if range.address <= last_end {
                last.size = last_end.max(range.address + range.size) - last.address;
                continue;
            }
        }
        out.push(range);
    }
    out
}

fn covered_bytes(admitted: &[Admitted], namespace: &str, executable: &[Range]) -> u64 {
    admitted
        .iter()
        .filter(|item| item.overlay == namespace)
        .flat_map(|item| item.ranges.iter())
        .map(|range| {
            executable.iter().map(|interval| {
                let start = range.address.max(interval.address);
                let end = (range.address + range.size).min(interval.address + interval.size);
                end.saturating_sub(start)
            }).sum::<u64>()
        })
        .sum()
}

static SCRATCH_ID: AtomicU64 = AtomicU64::new(0);

struct Scratch(PathBuf);

impl Scratch {
    fn create() -> Result<Self, String> {
        let id = SCRATCH_ID.fetch_add(1, Ordering::Relaxed);
        let path = std::env::temp_dir().join(format!(
            "alchemy-semantic-{}-{id}",
            std::process::id()
        ));
        std::fs::create_dir(&path).map_err(|error| format!("{}: {error}", path.display()))?;
        Ok(Self(path))
    }
}

impl Drop for Scratch {
    fn drop(&mut self) {
        let _ = std::fs::remove_dir_all(&self.0);
    }
}

pub fn build_semantic(directory: &Path) -> Result<BuildReport, String> {
    let root = repository_root();
    let semantic = root.join("semantic");
    let sources = sources_below(directory)?;
    let inventory_path = root.join("out/decomp/overlays.json");
    if !sources.is_empty() && !inventory_path.exists() {
        return Err("semantic build requires out/decomp/overlays.json; run the code-overlay inventory first".to_string());
    }
    let inventory = if sources.is_empty() {
        Vec::new()
    } else {
        read_json::<OverlayInventory>(&inventory_path)?.functions
    };
    let manifest_path = root.join("out/full/asm/manifest.json");
    let manifest_value: Value = if manifest_path.exists() { read_json(&manifest_path)? } else { serde_json::json!({"regions":[]}) };
    let manifest_regions: Vec<MainManifestRegion> = serde_json::from_value(manifest_value["regions"].clone())
        .map_err(|error| format!("{}: {error}", manifest_path.display()))?;
    let claimed_path = root.join("out/full/claimed/manifest.json");
    let claimed_value: Value = if claimed_path.exists() { read_json(&claimed_path)? } else { serde_json::json!({"regions":[]}) };
    let claimed_regions: Vec<MainClaimedRegion> = serde_json::from_value(claimed_value["regions"].clone())
        .map_err(|error| format!("{}: {error}", claimed_path.display()))?;
    let owner_path = semantic.join("main-regions.json");
    let owner_document = if owner_path.exists() {
        read_json::<MainOwnerDocument>(&owner_path)?
    } else {
        MainOwnerDocument { format: 1, main_owners: Vec::new() }
    };
    if owner_document.format != 1 {
        return Err(format!("{} has an unsupported schema", relative(&root, &owner_path)));
    }
    let main_owners = validate_main_semantic_owners(
        &root,
        &owner_document.main_owners,
        &manifest_regions,
        &claimed_regions,
        &relative(&root, &owner_path),
    )?;
    let manual_path = semantic.join("regions.json");
    let manual = if manual_path.exists() {
        let document: ManualDocument = read_json(&manual_path)?;
        if document.format != 1 {
            return Err(format!("{} has an unsupported schema", relative(&root, &manual_path)));
        }
        document.manual_regions
    } else {
        Vec::new()
    };
    let work = Scratch::create()?;
    let mut source_bytes = 0u64;
    let mut reviewed_bytes = 0u64;
    let mut admitted: Vec<Admitted> = Vec::new();
    for (index, source) in sources.iter().enumerate() {
        let identity = validate_source(&root, source)?;
        let text = std::fs::read(source).map_err(|error| format!("{}: {error}", source.display()))?;
        source_bytes += text.len() as u64;
        let name = source.file_name().unwrap();
        let exact_source = root.join("exact").join(name);
        if read_optional(&exact_source)?.is_some_and(|text| canonical_c_source(&text)) {
            return Err(format!(
                "{} duplicates exact source {}",
                relative(&root, source),
                relative(&root, &exact_source)
            ));
        }
        let inventoried = (identity.kind == Kind::Overlay).then(|| {
            inventory.iter().find(|item| item.overlay == identity.owner && item.entry == identity.address)
        }).flatten();
        let reviewed = (identity.kind == Kind::Overlay).then(|| {
            manual.iter().find(|item| {
                item.overlay == identity.owner
                    && parse_address(&item.entry, "manual region entry").ok() == Some(identity.address)
            })
        }).flatten();
        let main_region = (identity.kind == Kind::Main).then(|| {
            manifest_regions.iter().find(|item| {
                item.address == identity.address
                    && ["c_candidate", "split_first", "merge_with_continuations", "merge_with_owner"]
                        .contains(&item.retention.as_str())
            })
        }).flatten();
        let tracked = (identity.kind == Kind::Main).then(|| {
            main_owners.iter().find(|item| item.entry == identity.address)
        }).flatten();
        let ranges = if let Some(owner) = tracked {
            owner.executable_ranges.clone()
        } else if let Some(region) = reviewed {
            if region.span_bytes == 0 || region.evidence.trim().is_empty() {
                return Err(format!("{} contains an invalid reviewed boundary", relative(&root, &manual_path)));
            }
            vec![Range { address: identity.address, size: region.span_bytes }]
        } else if let Some(region) = inventoried {
            vec![Range { address: identity.address, size: region.span_bytes }]
        } else if let Some(region) = main_region {
            vec![Range { address: identity.address, size: region.size }]
        } else {
            return Err(format!("{} has no admitted semantic owner", relative(&root, source)));
        };
        reviewed_bytes += ranges.iter().map(|range| range.size).sum::<u64>();
        if let Some(prior) = admitted.iter().find(|prior| {
            prior.overlay == identity.owner
                && ranges.iter().any(|range| prior.ranges.iter().any(|other| overlaps(range, other)))
        }) {
            return Err(format!(
                "{} overlaps {} in {}",
                relative(&root, source),
                relative(&root, &prior.source),
                identity.owner
            ));
        }
        admitted.push(Admitted { overlay: identity.owner.clone(), ranges, source: source.clone() });
        let stem = format!("{index:04}-{}", source.file_stem().unwrap().to_string_lossy());
        let output = work.0.join(format!("{stem}.s"));
        let mut options = SourceToAssemblyPlanOptions::new(
            CompilerTarget::Gs1,
            source.to_string_lossy(),
            source.to_string_lossy(),
            output.to_string_lossy(),
        );
        options.preprocessed_output = Some(work.0.join(format!("{stem}.i")).to_string_lossy().into_owned());
        let plan = source_to_assembly_plan(&options)?;
        for step in plan.steps {
            checked(&step.command, &work.0)?;
        }
    }
    let progress: Value = read_json(&root.join("metrics/gs1-en-progress.json"))?;
    let full_c_bytes = value_u64(&progress["full_c_bytes"], "full_c_bytes")?;
    let executable_bytes = value_u64(&progress["executable_bytes"], "executable_bytes")?;
    let executable: Value = read_json(&root.join("metrics/gs1-en-executable.json"))?;
    let main_intervals = union_intervals(intervals(&executable["main"]["intervals"], "main intervals")?);
    let main_semantic_bytes = covered_bytes(&admitted, "main", &main_intervals);
    let overlays = executable["overlays"].as_array().ok_or("overlays must be an array")?;
    let mut overlay_semantic_bytes = 0u64;
    for (index, overlay) in overlays.iter().enumerate() {
        let id = overlay["id"].as_str().ok_or_else(|| format!("overlay {index} id must be a string"))?;
        let ranges = union_intervals(intervals(&overlay["intervals"], &format!("overlay {id} intervals"))?);
        overlay_semantic_bytes += covered_bytes(&admitted, id, &ranges);
    }
    let semantic_bytes = main_semantic_bytes + overlay_semantic_bytes;
    Ok(BuildReport {
        sources: sources.len() as u64,
        source_bytes,
        reviewed_bytes,
        outside_executable_bytes: reviewed_bytes - semantic_bytes,
        semantic_bytes,
        main_semantic_bytes,
        overlay_semantic_bytes,
        expressed_bytes: full_c_bytes + semantic_bytes,
        executable_bytes,
    })
}

pub fn self_test() -> Result<(), String> {
    let root = repository_root();
    let manifest = vec![
        MainManifestRegion { address: 0x08001000, size: 12, retention: "split_first".into() },
        MainManifestRegion { address: 0x08001008, size: 4, retention: "merge_with_function_owner".into() },
        MainManifestRegion { address: 0x08001020, size: 8, retention: "merge_with_function_owner".into() },
    ];
    let valid = validate_main_semantic_owners(&root, &[MainSemanticOwner {
        entry: "0x08001000".into(), evidence: "test owner".into(), executable_ranges: vec![
            MainExecutableRange { address: "0x08001000".into(), size: 12 },
            MainExecutableRange { address: "0x08001020".into(), size: 8 },
        ],
    }], &manifest, &[], "self-test")?;
    if valid[0].executable_ranges.iter().map(|range| range.size).sum::<u64>() != 20 {
        return Err("noncontiguous owner byte sum rejected".into());
    }
    for owner in [
        MainSemanticOwner { entry: "0x08001000".into(), evidence: "bad size".into(), executable_ranges: vec![MainExecutableRange { address: "0x08001000".into(), size: 0 }] },
        MainSemanticOwner { entry: "0x08001000".into(), evidence: "missing row".into(), executable_ranges: vec![MainExecutableRange { address: "0x08000ffc".into(), size: 8 }] },
        MainSemanticOwner { entry: "0x08001030".into(), evidence: "outside rows".into(), executable_ranges: vec![MainExecutableRange { address: "0x08001030".into(), size: 4 }] },
        MainSemanticOwner { entry: "0x08001000".into(), evidence: "overlap".into(), executable_ranges: vec![MainExecutableRange { address: "0x08001000".into(), size: 12 }, MainExecutableRange { address: "0x08001008".into(), size: 4 }] },
    ] {
        if validate_main_semantic_owners(&root, &[owner], &manifest, &[], "self-test").is_ok() {
            return Err("invalid main semantic owner accepted".into());
        }
    }
    if validate_source(&root, &root.join("semantic/080006fc.c")).is_err() {
        return Err("real main semantic source rejected".into());
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn unit_self_test_passes() {
        self_test().unwrap();
    }

    #[test]
    fn interval_union_merges_touching_ranges() {
        assert_eq!(
            union_intervals(vec![Range { address: 8, size: 4 }, Range { address: 4, size: 4 }]),
            vec![Range { address: 4, size: 8 }]
        );
    }

    #[test]
    fn source_validation_rejects_assembly_and_bad_names() {
        assert!(inline_assembly("void f(void) { asm volatile(\"nop\"); }"));
        assert!(!inline_assembly("int plasma = 1;"));
        assert!(canonical_c_source("int f(void) { return 1; }"));
        assert!(!canonical_c_source("__asm__(\"nop\")"));
    }
}
