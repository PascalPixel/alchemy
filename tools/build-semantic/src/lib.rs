//! Native semantic-C validation and compilation.

use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::atomic::{AtomicU64, Ordering};

use alchemy_bundle::bundle::compiler_bundle_signature;
use alchemy_bundle::sha256;
use alchemy_plan::plan::{source_to_assembly_plan, SourceToAssemblyPlanOptions};
use alchemy_routing::routing::CompilerTarget;
use cache_entry::write_cache_entry_atomically;
use serde::Deserialize;
use serde::Serialize;
use serde_json::Value;

const SEMANTIC_CACHE_SCHEMA: u64 = 1;
const SEMANTIC_CACHE_DIR: &str = "out/cache/semantic-validation";

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

fn files_below(directory: &Path) -> Result<Vec<PathBuf>, String> {
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
            result.extend(files_below(&path)?);
        } else if std::fs::metadata(&path)
            .map_err(|error| format!("{}: {error}", path.display()))?
            .is_file()
        {
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
    if !overlay_disasm::source_defines_symbol(&text, &symbol) {
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
            let rest = rest.strip_prefix("volatile").map_or(rest, str::trim_start);
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

fn append_frame(stream: &mut Vec<u8>, bytes: &[u8]) {
    stream.extend_from_slice(&(bytes.len() as u64).to_be_bytes());
    stream.extend_from_slice(bytes);
}

fn append_text(stream: &mut Vec<u8>, text: &str) {
    append_frame(stream, text.as_bytes());
}

fn include_signature(root: &Path) -> Result<String, String> {
    let mut stream = Vec::new();
    append_text(&mut stream, "build-semantic include inputs v1");
    let include = root.join("include");
    for path in files_below(&include)? {
        let bytes = std::fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))?;
        append_text(&mut stream, &relative(root, &path));
        append_frame(&mut stream, &bytes);
    }
    Ok(sha256::hex(&stream))
}

fn executable_path(program: &str) -> Result<PathBuf, String> {
    let requested = Path::new(program);
    if requested.is_absolute() || requested.components().count() > 1 {
        return std::fs::canonicalize(requested)
            .map_err(|error| format!("{program}: {error}"));
    }
    let path = std::env::var_os("PATH").ok_or_else(|| "PATH is unset".to_string())?;
    for directory in std::env::split_paths(&path) {
        let candidate = directory.join(program);
        if is_executable_file(&candidate) {
            return std::fs::canonicalize(&candidate)
                .map_err(|error| format!("{}: {error}", candidate.display()));
        }
    }
    Err(format!("{program} cannot be resolved on PATH"))
}

fn is_executable_file(path: &Path) -> bool {
    let Ok(metadata) = std::fs::metadata(path) else { return false };
    if !metadata.is_file() {
        return false;
    }
    #[cfg(unix)]
    {
        use std::os::unix::fs::PermissionsExt;
        metadata.permissions().mode() & 0o111 != 0
    }
    #[cfg(not(unix))]
    {
        true
    }
}

fn host_tool_signature(commands: &[Vec<String>]) -> Result<String, String> {
    let mut stream = Vec::new();
    append_text(&mut stream, "build-semantic host tools v1");
    for command in commands {
        let program = command.first().ok_or_else(|| "empty compiler command".to_string())?;
        let path = executable_path(program)?;
        let bytes = std::fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))?;
        append_text(&mut stream, program);
        append_text(&mut stream, &path.to_string_lossy());
        append_frame(&mut stream, &bytes);
    }
    Ok(sha256::hex(&stream))
}

fn implementation_signature_for(path: &Path) -> Result<String, String> {
    let bytes = std::fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?;
    Ok(sha256::hex(&bytes))
}

fn implementation_signature() -> Result<String, String> {
    let path = std::env::current_exe().map_err(|error| format!("current executable: {error}"))?;
    implementation_signature_for(&path)
}

fn normalized_argument(argument: &str, scratch: &Path) -> String {
    let scratch = scratch.to_string_lossy();
    if argument == scratch {
        return "<scratch>".to_string();
    }
    if let Some(rest) = argument.strip_prefix(&format!("{scratch}/")) {
        let name = Path::new(rest)
            .file_name()
            .and_then(|name| name.to_str())
            .unwrap_or(rest);
        let stable = name
            .split_once('-')
            .filter(|(prefix, _)| !prefix.is_empty() && prefix.bytes().all(|byte| byte.is_ascii_digit()))
            .map_or(name, |(_, suffix)| suffix);
        return format!("<scratch>/{stable}");
    }
    argument.to_string()
}

fn plan_identity(plan: &alchemy_plan::plan::SourceToAssemblyPlan, scratch: &Path) -> Vec<u8> {
    let mut stream = Vec::new();
    append_text(&mut stream, "build-semantic compiler plan v1");
    append_text(&mut stream, plan.target.as_str());
    append_text(&mut stream, plan.requested_family.as_str());
    append_text(&mut stream, plan.family.as_str());
    append_text(&mut stream, &normalized_argument(&plan.routing_source, scratch));
    append_text(&mut stream, &normalized_argument(&plan.input, scratch));
    append_text(&mut stream, &normalized_argument(&plan.output, scratch));
    append_text(&mut stream, &normalized_argument(&plan.compiler_input, scratch));
    append_frame(&mut stream, &(plan.flags.len() as u64).to_be_bytes());
    for flag in &plan.flags {
        append_text(&mut stream, flag);
    }
    append_frame(&mut stream, &(plan.steps.len() as u64).to_be_bytes());
    for step in &plan.steps {
        append_text(&mut stream, step.kind.as_str());
        append_frame(&mut stream, &(step.command.len() as u64).to_be_bytes());
        for argument in &step.command {
            append_text(&mut stream, &normalized_argument(argument, scratch));
        }
    }
    stream
}

struct SemanticCacheIdentity<'a> {
    include_digest: &'a str,
    compiler_digest: &'a str,
    host_digest: &'a str,
    implementation_digest: &'a str,
}

fn semantic_cache_key(
    root: &Path,
    source: &Path,
    source_bytes: &[u8],
    plan: &alchemy_plan::plan::SourceToAssemblyPlan,
    scratch: &Path,
    identity: &SemanticCacheIdentity<'_>,
) -> String {
    let mut stream = Vec::new();
    append_text(&mut stream, "build-semantic per-source cache v1");
    append_text(&mut stream, &relative(root, source));
    append_frame(&mut stream, source_bytes);
    append_frame(&mut stream, &plan_identity(plan, scratch));
    append_text(&mut stream, identity.include_digest);
    append_text(&mut stream, identity.compiler_digest);
    append_text(&mut stream, identity.host_digest);
    append_text(&mut stream, identity.implementation_digest);
    sha256::hex(&stream)
}

#[derive(Debug, Clone, Deserialize, Serialize)]
struct SemanticCacheRecord {
    schema: u64,
    key: String,
    success: bool,
    output_size: u64,
    output_sha256: String,
}

fn semantic_cache_paths(root: &Path, key: &str) -> (PathBuf, PathBuf) {
    let directory = root.join(SEMANTIC_CACHE_DIR);
    (directory.join(format!("{key}.s")), directory.join(format!("{key}.json")))
}

fn valid_sha256(value: &str) -> bool {
    value.len() == 64 && value.bytes().all(|byte| byte.is_ascii_hexdigit() && !byte.is_ascii_uppercase())
}

fn read_semantic_cache(root: &Path, key: &str) -> bool {
    let (payload_path, record_path) = semantic_cache_paths(root, key);
    let Ok(record_bytes) = std::fs::read(record_path) else { return false };
    let Ok(record) = serde_json::from_slice::<SemanticCacheRecord>(&record_bytes) else { return false };
    if record.schema != SEMANTIC_CACHE_SCHEMA
        || record.key != key
        || !record.success
        || !valid_sha256(&record.output_sha256)
    {
        return false;
    }
    let Ok(payload) = std::fs::read(payload_path) else { return false };
    payload.len() as u64 == record.output_size && sha256::hex(&payload) == record.output_sha256
}

fn write_semantic_cache(root: &Path, key: &str, output: &Path) {
    let Ok(payload) = std::fs::read(output) else { return };
    let (payload_path, record_path) = semantic_cache_paths(root, key);
    if std::fs::create_dir_all(root.join(SEMANTIC_CACHE_DIR)).is_err() {
        return;
    }
    if write_cache_entry_atomically(&payload_path, &payload).is_err() {
        return;
    }
    let record = SemanticCacheRecord {
        schema: SEMANTIC_CACHE_SCHEMA,
        key: key.to_string(),
        success: true,
        output_size: payload.len() as u64,
        output_sha256: sha256::hex(&payload),
    };
    let Ok(record_bytes) = serde_json::to_vec(&record) else { return };
    let _ = write_cache_entry_atomically(&record_path, &record_bytes);
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
    let include_digest = if sources.is_empty() {
        String::new()
    } else {
        include_signature(&root)?
    };
    let compiler_digest = if sources.is_empty() {
        String::new()
    } else {
        compiler_bundle_signature()
    };
    let implementation_digest = if sources.is_empty() {
        String::new()
    } else {
        implementation_signature()?
    };
    let mut source_bytes = 0u64;
    let mut reviewed_bytes = 0u64;
    let mut admitted: Vec<Admitted> = Vec::new();
    let mut host_digests: Vec<(Vec<String>, String)> = Vec::new();
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
        let host_programs: Vec<String> = plan
            .steps
            .iter()
            .map(|step| {
                step.command
                    .first()
                    .cloned()
                    .ok_or_else(|| "empty compiler command".to_string())
            })
            .collect::<std::result::Result<Vec<_>, _>>()?;
        let host_digest = if let Some((_, digest)) = host_digests
            .iter()
            .find(|(programs, _)| *programs == host_programs)
        {
            digest.clone()
        } else {
            let commands = plan.steps.iter().map(|step| step.command.clone()).collect::<Vec<_>>();
            let digest = host_tool_signature(&commands)?;
            host_digests.push((host_programs, digest.clone()));
            digest
        };
        let identity = SemanticCacheIdentity {
            include_digest: &include_digest,
            compiler_digest: &compiler_digest,
            host_digest: &host_digest,
            implementation_digest: &implementation_digest,
        };
        let key = semantic_cache_key(
            &root,
            source,
            &text,
            &plan,
            &work.0,
            &identity,
        );
        if read_semantic_cache(&root, &key) {
            continue;
        }
        for step in &plan.steps {
            checked(&step.command, &work.0)?;
        }
        write_semantic_cache(&root, &key, &output);
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
    use alchemy_plan::plan::{
        CompilerCommandStep, CompilerFamily, ResolvedFamily, SourceToAssemblyPlan, StepKind,
    };

    fn test_plan() -> SourceToAssemblyPlan {
        SourceToAssemblyPlan {
            target: CompilerTarget::Gs1,
            requested_family: CompilerFamily::Routed,
            family: ResolvedFamily::Gcc296,
            routing_source: "/repo/semantic/08000000.c".into(),
            input: "/repo/semantic/08000000.c".into(),
            output: "/tmp/build-semantic/0000-08000000.s".into(),
            compiler_input: "/repo/semantic/08000000.c".into(),
            flags: vec!["-O2".into()],
            steps: vec![CompilerCommandStep {
                kind: StepKind::Compile,
                command: vec![
                    "/toolchain/xgcc".into(),
                    "/repo/semantic/08000000.c".into(),
                    "-S".into(),
                    "-o".into(),
                    "/tmp/build-semantic/0000-08000000.s".into(),
                ],
            }],
        }
    }

    fn test_key(source: &[u8], include: &str, plan: &SourceToAssemblyPlan) -> String {
        semantic_cache_key(
            Path::new("/repo"),
            Path::new("/repo/semantic/08000000.c"),
            source,
            plan,
            Path::new("/tmp/build-semantic"),
            &SemanticCacheIdentity {
                include_digest: include,
                compiler_digest: "compiler",
                host_digest: "host",
                implementation_digest: "implementation",
            },
        )
    }

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

    #[test]
    fn semantic_cache_key_changes_for_source_headers_and_plan() {
        let plan = test_plan();
        let base = test_key(b"int f(void) { return 1; }", "headers-a", &plan);
        assert_ne!(base, test_key(b"int f(void) { return 2; }", "headers-a", &plan));
        assert_ne!(base, test_key(b"int f(void) { return 1; }", "headers-b", &plan));
        let mut changed_plan = plan.clone();
        changed_plan.flags.push("-fno-builtin".into());
        assert_ne!(base, test_key(b"int f(void) { return 1; }", "headers-a", &changed_plan));
    }

    #[test]
    fn semantic_cache_key_ignores_enumeration_only_scratch_names() {
        let plan = test_plan();
        let base = test_key(b"int f(void) { return 1; }", "headers-a", &plan);
        let mut shifted = plan.clone();
        shifted.output = shifted.output.replace("0000-", "0999-");
        for step in &mut shifted.steps {
            for argument in &mut step.command {
                *argument = argument.replace("0000-", "0999-");
            }
        }
        assert_eq!(
            base,
            test_key(b"int f(void) { return 1; }", "headers-a", &shifted)
        );
    }

    #[test]
    fn semantic_cache_warm_hit_validates_record_and_payload() {
        let id = SCRATCH_ID.fetch_add(1, Ordering::Relaxed);
        let root = std::env::temp_dir().join(format!("alchemy-semantic-cache-test-{id}"));
        std::fs::create_dir_all(&root).unwrap();
        let output = root.join("output.s");
        std::fs::write(&output, b"compiled assembly\n").unwrap();
        write_semantic_cache(&root, "test-key", &output);
        assert!(read_semantic_cache(&root, "test-key"));

        let (_, record) = semantic_cache_paths(&root, "test-key");
        std::fs::write(&record, b"{\"schema\":1}").unwrap();
        assert!(!read_semantic_cache(&root, "test-key"));

        write_semantic_cache(&root, "test-key", &output);
        let (payload, _) = semantic_cache_paths(&root, "test-key");
        std::fs::write(&payload, b"truncated").unwrap();
        assert!(!read_semantic_cache(&root, "test-key"));
        let _ = std::fs::remove_dir_all(root);
    }
}
