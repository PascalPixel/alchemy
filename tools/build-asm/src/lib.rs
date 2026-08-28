//! Assemble retained source regions and emit their classified manifest.
pub mod cli;
use cache_entry::sqlite::SqliteCache;
use canonical_json::write_canonical;
use compiler_core::{
    build_io::{argv, read, read_json, relative, rooted, text, write},
    bundle::host_executable_signature,
    sha256,
    thumb::standalone_wide_transfer_lines,
};
use serde::Deserialize;
use serde_json::{json, Value};
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};
use std::process::Command;
const ROM_BASE: u64 = 0x0800_0000;
const ROM_SIZE: u64 = 0x0080_0000;
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Options {
    pub rom: String,
    pub output: String,
    pub source: Option<String>,
    pub source_only: bool,
}
#[derive(Debug, Clone, PartialEq, Eq)]
pub enum ParseOutcome {
    Help,
    Run(Options),
}
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct BuildReport {
    pub regions: usize,
    pub bytes: usize,
    pub counts: String,
}
#[derive(Debug, Clone)]
struct BuiltRegion {
    address: u64,
    run_address: u64,
    data: Vec<u8>,
}
#[derive(Debug, Clone)]
struct Placement {
    address: u64,
    run_address: u64,
}
#[derive(Debug, Clone, Deserialize)]
struct LayoutRegion {
    source: String,
    address: Option<Value>,
    run_address: Value,
}
#[derive(Debug, Clone, Deserialize)]
struct Classification {
    kind: String,
    origin: String,
    retention: String,
    confidence: String,
    evidence: Vec<String>,
}
#[derive(Debug, Clone, Deserialize)]
struct ClassificationRule {
    kind: String,
    origin: String,
    retention: String,
    confidence: String,
    evidence: Vec<String>,
    expected_files: usize,
    expected_bytes: usize,
    files: Option<Vec<String>>,
    matcher: Option<String>,
}
impl ClassificationRule {
    fn classification(&self) -> Classification {
        Classification {
            kind: self.kind.clone(),
            origin: self.origin.clone(),
            retention: self.retention.clone(),
            confidence: self.confidence.clone(),
            evidence: self.evidence.clone(),
        }
    }
}
#[derive(Debug, Clone, Deserialize)]
struct ClassificationConfig {
    format: u64,
    default: Classification,
    structural: Vec<ClassificationRule>,
    groups: Vec<ClassificationRule>,
}
#[derive(Debug, Clone, Copy, Default)]
struct Count {
    files: usize,
    bytes: usize,
}
pub fn repository_root() -> PathBuf {
    compiler_core::routing::root().to_path_buf()
}
pub fn parse_args(argv: &[String]) -> Result<ParseOutcome, String> {
    let mut options = Options {
        rom: "roms/gs1-en.gba".into(),
        output: "out/gs1-en/asm".into(),
        source: None,
        source_only: false,
    };
    let mut positional = false;
    let mut index = 0usize;
    while index < argv.len() {
        let argument = &argv[index];
        if argument == "-h" || argument == "--help" {
            return Ok(ParseOutcome::Help);
        } else if argument == "--source-only" {
            options.source_only = true;
        } else if argument == "--output" || argument == "--source" {
            index += 1;
            let value = argv
                .get(index)
                .ok_or_else(|| format!("{argument} requires a value"))?
                .clone();
            if argument == "--output" {
                options.output = value;
            } else {
                options.source = Some(value);
            }
        } else if let Some(value) = argument.strip_prefix("--output=") {
            options.output = value.to_string();
        } else if let Some(value) = argument.strip_prefix("--source=") {
            options.source = Some(value.to_string());
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
fn resolve(root: &Path, cwd: &Path, value: &str) -> PathBuf {
    let path = Path::new(value);
    if path.is_absolute() {
        path.to_path_buf()
    } else if value.starts_with("out/") || value.starts_with("games/gs1/asm/") {
        root.join(path)
    } else {
        cwd.join(path)
    }
}
fn stem(path: &Path) -> String {
    path.file_stem()
        .and_then(|name| name.to_str())
        .unwrap_or_default()
        .to_string()
}
fn assembly_sources(directory: &Path) -> Result<Vec<PathBuf>, String> {
    let mut result = Vec::new();
    let entries = std::fs::read_dir(directory)
        .map_err(|error| format!("{}: {error}", directory.display()))?;
    for entry in entries {
        let entry = entry.map_err(|error| format!("{}: {error}", directory.display()))?;
        let name = entry.file_name();
        if name.to_string_lossy().starts_with('.') {
            continue;
        }
        let path = entry.path();
        let kind = entry
            .file_type()
            .map_err(|error| format!("{}: {error}", path.display()))?;
        if kind.is_dir() {
            result.extend(assembly_sources(&path)?);
        } else if kind.is_file() && path.extension().is_some_and(|extension| extension == "s") {
            result.push(path);
        }
    }
    result.sort();
    Ok(result)
}
fn run(root: &Path, command: &[String]) -> Result<String, String> {
    let program = command.first().ok_or("empty command")?;
    let output = Command::new(program)
        .args(&command[1..])
        .current_dir(root)
        .output()
        .map_err(|error| format!("{program}: {error}"))?;
    let stdout = String::from_utf8_lossy(&output.stdout).into_owned();
    let stderr = String::from_utf8_lossy(&output.stderr).into_owned();
    if output.status.success() {
        Ok(stdout)
    } else {
        let detail = if stderr.is_empty() { stdout } else { stderr };
        let name = Path::new(program)
            .file_name()
            .and_then(|name| name.to_str())
            .unwrap_or(program);
        Err(format!("{name} failed: {}", detail.trim()))
    }
}
fn integer(value: &Value, name: &str) -> Result<u64, String> {
    let parsed = match value {
        Value::Number(number) => number.as_u64(),
        Value::String(text) => text
            .strip_prefix("0x")
            .or_else(|| text.strip_prefix("0X"))
            .map_or_else(
                || text.parse::<u64>().ok(),
                |digits| u64::from_str_radix(digits, 16).ok(),
            ),
        _ => None,
    };
    parsed
        .filter(|value| *value <= u32::MAX as u64)
        .ok_or_else(|| format!("{name}: invalid address"))
}
fn load_layout(root: &Path) -> Result<BTreeMap<String, Placement>, String> {
    let path = root.join("games/gs1/asm/manifest.json");
    if !path.exists() {
        return Ok(BTreeMap::new());
    }
    let value: Value = read_json(&path)?;
    if value["format"].as_u64() != Some(1) || !value["regions"].is_array() {
        return Err("games/gs1/asm/manifest.json: unsupported format".into());
    }
    let regions: Vec<LayoutRegion> = serde_json::from_value(value["regions"].clone())
        .map_err(|_| "games/gs1/asm/manifest.json: unsupported format".to_string())?;
    let mut result = BTreeMap::new();
    for item in regions {
        if result.contains_key(&item.source) {
            return Err("games/gs1/asm/manifest.json: invalid or duplicate source".into());
        }
        let inferred = u64::from_str_radix(&stem(Path::new(&item.source)), 16)
            .map_err(|_| format!("{}: invalid address", item.source))?;
        let address = match item.address.as_ref() {
            Some(value) => integer(value, &item.source)?,
            None => inferred,
        };
        if address != inferred {
            return Err(format!(
                "{}: load address differs from filename",
                item.source
            ));
        }
        let run_address = integer(&item.run_address, &item.source)?;
        result.insert(
            item.source,
            Placement {
                address,
                run_address,
            },
        );
    }
    Ok(result)
}
fn load_classification(path: &Path) -> Result<ClassificationConfig, String> {
    let config: ClassificationConfig = read_json(path)?;
    if config.format != 1 {
        return Err(format!(
            "unsupported assembly classification format: {}",
            config.format
        ));
    }
    for rule in &config.structural {
        if rule.files.is_some() || rule.matcher.is_some() {
            return Err(format!(
                "{}: structural rules cannot name sources",
                rule.kind
            ));
        }
    }
    for rule in &config.groups {
        match (rule.files.as_deref(), rule.matcher.as_deref()) {
            (Some(files), None) if !files.is_empty() => {}
            (None, Some("thumb_standalone_wide_transfer")) => {}
            _ => {
                return Err(format!(
                    "{}: invalid assembly classification rule",
                    rule.kind
                ))
            }
        }
    }
    Ok(config)
}
fn load_alignments(path: &Path) -> Result<Vec<(u64, Vec<u8>)>, String> {
    let value: Value = read_json(path)?;
    if value["format"].as_u64() != Some(1)
        || value["kind"].as_str() != Some("thumb-function-alignment")
        || value["width"].as_u64() != Some(2)
        || value["value"].as_u64() != Some(0)
        || !value["addresses"].is_array()
    {
        return Err("unsupported alignment source".into());
    }
    let mut found = BTreeSet::new();
    let mut result = Vec::new();
    for (index, item) in value["addresses"].as_array().unwrap().iter().enumerate() {
        let text = item.as_str().unwrap_or("");
        if text.len() != 10
            || !text.starts_with("0x080")
            || !text[2..].bytes().all(|byte| byte.is_ascii_hexdigit())
        {
            return Err(format!("alignment {index}: invalid address"));
        }
        let address = u64::from_str_radix(&text[2..], 16)
            .map_err(|_| format!("alignment {index}: invalid address"))?;
        if address & 3 != 2 || !found.insert(address) {
            return Err(format!("alignment {index}: invalid boundary"));
        }
        result.push((address, vec![0, 0]));
    }
    result.sort_by_key(|item| item.0);
    Ok(result)
}
fn explicit_classifications(
    config: &ClassificationConfig,
) -> Result<BTreeMap<String, ClassificationRule>, String> {
    let mut result = BTreeMap::new();
    for group in &config.groups {
        for name in group.files.as_deref().unwrap_or(&[]) {
            if name.len() != 8
                || !name
                    .bytes()
                    .all(|byte| byte.is_ascii_hexdigit() && !byte.is_ascii_uppercase())
            {
                return Err(format!("invalid classified assembly stem: {name}"));
            }
            if result.insert(name.clone(), group.clone()).is_some() {
                return Err(format!("duplicate assembly classification: {name}"));
            }
        }
    }
    Ok(result)
}
fn long_call_veneer(data: &[u8]) -> bool {
    data.len() == 8
        && u16::from_le_bytes([data[0], data[1]]) == 0x4c00
        && u16::from_le_bytes([data[2], data[3]]) == 0x4720
        && u32::from_le_bytes([data[4], data[5], data[6], data[7]]) & 1 != 0
}
fn alignment_padding(data: &[u8]) -> bool {
    data == [0, 0]
}
fn thumb_standalone_wide_transfer(source: &str) -> bool {
    !standalone_wide_transfer_lines(source).is_empty()
}
fn classify(
    name: &str,
    data: &[u8],
    source: &str,
    config: &ClassificationConfig,
    explicit: &BTreeMap<String, ClassificationRule>,
) -> Result<Classification, String> {
    if let Some(fixed) = explicit.get(name) {
        return Ok(fixed.classification());
    }
    let structural = if long_call_veneer(data) {
        Some(("linker_veneer", "missing linker veneer classification"))
    } else if alignment_padding(data) {
        Some((
            "alignment_padding",
            "missing alignment padding classification",
        ))
    } else {
        None
    };
    if let Some((kind, missing)) = structural {
        return config
            .structural
            .iter()
            .find(|rule| rule.kind == kind)
            .map(ClassificationRule::classification)
            .ok_or_else(|| missing.into());
    }
    if thumb_standalone_wide_transfer(source) {
        return config
            .groups
            .iter()
            .find(|rule| rule.matcher.as_deref() == Some("thumb_standalone_wide_transfer"))
            .map(ClassificationRule::classification)
            .ok_or_else(|| "missing Thumb multi-register classification".into());
    }
    Ok(config.default.clone())
}
fn validate_counts(
    config: &ClassificationConfig,
    counts: &BTreeMap<String, Count>,
) -> Result<(), String> {
    for rule in config.structural.iter().chain(&config.groups) {
        let count = counts.get(&rule.kind).copied().unwrap_or_default();
        if count.files != rule.expected_files || count.bytes != rule.expected_bytes {
            return Err(format!(
                "{}: expected {} files/{} bytes, got {} files/{} bytes",
                rule.kind, rule.expected_files, rule.expected_bytes, count.files, count.bytes
            ));
        }
    }
    Ok(())
}
fn self_digest() -> Result<String, String> {
    let path = Path::new(env!("CARGO_MANIFEST_DIR")).join("src/lib.rs");
    let bytes = read(&path)?;
    if bytes.is_empty() {
        return Err(format!(
            "build_asm read an EMPTY source at {}; refusing to key the cache",
            path.display()
        ));
    }
    Ok(sha256::hex(&bytes))
}
const ASSEMBLY_BINUTILS: [&str; 4] = [
    "arm-none-eabi-as",
    "arm-none-eabi-nm",
    "arm-none-eabi-ld",
    "arm-none-eabi-objcopy",
];
fn production_binutil_signatures() -> Result<Vec<(String, String)>, String> {
    ASSEMBLY_BINUTILS
        .iter()
        .map(|name| {
            host_executable_signature(&[*name])
                .map(|signature| ((*name).to_string(), signature))
                .map_err(|error| format!("{name}: {error}"))
        })
        .collect()
}
fn append_frame(stream: &mut Vec<u8>, bytes: &[u8]) {
    stream.extend_from_slice(&(bytes.len() as u64).to_be_bytes());
    stream.extend_from_slice(bytes);
}
/// Build a region key from injectable tool signatures so tests do not depend
/// on an installed ARM toolchain. The ordered name/signature pairs are part of
/// the key, and this material intentionally migrates the old region namespace
/// once; old entries remain on disk but are unreachable.
pub fn region_cache_key_with_signatures(
    source: &[u8],
    linked_address: u64,
    binutils: &[(String, String)],
) -> Result<String, String> {
    let mut bytes = Vec::new();
    append_frame(
        &mut bytes,
        b"build-asm cache identity: signed ordered binutils",
    );
    append_frame(&mut bytes, self_digest()?.as_bytes());
    append_frame(&mut bytes, &linked_address.to_be_bytes());
    append_frame(&mut bytes, &(binutils.len() as u64).to_be_bytes());
    for (name, signature) in binutils {
        append_frame(&mut bytes, name.as_bytes());
        append_frame(&mut bytes, signature.as_bytes());
    }
    append_frame(&mut bytes, source);
    Ok(sha256::hex(&bytes))
}
fn valid_external(name: &str) -> bool {
    let Some((prefix, address)) = name.rsplit_once('_') else {
        return false;
    };
    ["Func", "Data", "Value"].contains(&prefix)
        && address.len() == 8
        && address
            .bytes()
            .all(|byte| byte.is_ascii_hexdigit() && !byte.is_ascii_uppercase())
}
fn build_region(
    root: &Path,
    source: &Path,
    source_bytes: &[u8],
    output_dir: &Path,
    cache: &SqliteCache,
    run_address: Option<u64>,
    binutils: &[(String, String)],
) -> Result<BuiltRegion, String> {
    let name = stem(source);
    let address = u64::from_str_radix(&name, 16)
        .map_err(|_| format!("{}: invalid assembly filename", source.display()))?;
    let linked_address = run_address.unwrap_or(address);
    let object = output_dir.join(format!("{name}.o"));
    let elf = output_dir.join(format!("{name}.elf"));
    let binary = output_dir.join(format!("{name}.bin"));
    let cache_key = region_cache_key_with_signatures(source_bytes, linked_address, binutils)?;
    if let Some(data) = cache
        .get(&cache_key)
        .ok()
        .flatten()
        .and_then(|entries| entries.into_iter().find(|(kind, _)| kind == "payload"))
        .map(|(_, data)| data)
    {
        write(&binary, &data)?;
        return Ok(BuiltRegion {
            address,
            run_address: linked_address,
            data,
        });
    }
    run(
        root,
        &argv(&[
            "arm-none-eabi-as",
            "-mcpu=arm7tdmi",
            "-mthumb-interwork",
            "-o",
            &text(&object),
            &text(source),
        ]),
    )?;
    let undefined = run(root, &argv(&["arm-none-eabi-nm", "-u", &text(&object)]))?;
    let names: Vec<String> = undefined
        .lines()
        .filter(|line| !line.is_empty())
        .filter_map(|line| line.split_whitespace().last().map(str::to_string))
        .collect();
    for external in &names {
        if !valid_external(external) {
            return Err(format!(
                "{}: unsupported external symbol {external}",
                source.file_name().unwrap().to_string_lossy()
            ));
        }
    }
    let mut objects = vec![object.clone()];
    if !names.is_empty() {
        let symbols_source = output_dir.join(format!("{name}.symbols.s"));
        let symbols_object = output_dir.join(format!("{name}.symbols.o"));
        let mut body = ".syntax unified\n.thumb\n".to_string();
        for external in &names {
            body.push_str(&format!(
                ".global {external}\n{}.set {external}, 0x{}\n",
                if external.starts_with("Func_") {
                    ".thumb_func\n"
                } else {
                    ""
                },
                external.rsplit_once('_').unwrap().1
            ));
        }
        write(&symbols_source, body)?;
        run(
            root,
            &argv(&[
                "arm-none-eabi-as",
                "-mcpu=arm7tdmi",
                "-mthumb-interwork",
                "-o",
                &text(&symbols_object),
                &text(&symbols_source),
            ]),
        )?;
        objects.push(symbols_object);
    }
    let formatted = format!("{linked_address:08x}");
    let mut link = vec![
        "arm-none-eabi-ld".into(),
        format!("-Ttext=0x{formatted}"),
        "-e".into(),
        format!("0x{formatted}"),
        "-o".into(),
        text(&elf),
    ];
    link.extend(objects.iter().map(text));
    run(root, &link)?;
    run(
        root,
        &argv(&[
            "arm-none-eabi-objcopy",
            "-O",
            "binary",
            "-j",
            ".text",
            &text(&elf),
            &text(&binary),
        ]),
    )?;
    let data = read(&binary)?;
    cache.put(&cache_key, &[("payload", &data)])?;
    Ok(BuiltRegion {
        address,
        run_address: linked_address,
        data,
    })
}
fn region_value(
    output: &Path,
    source: &str,
    built: &BuiltRegion,
    category: &Classification,
) -> Value {
    json!({
        "address":built.address,
        "run_address":built.run_address,
        "size":built.data.len(),
        "source":source,
        "output":output.join(format!("{:08x}.bin", built.address)).to_string_lossy(),
        "kind":category.kind,
        "origin":category.origin,
        "retention":category.retention,
        "confidence":category.confidence,
        "evidence":category.evidence.join(","),
    })
}
pub fn build(root: &Path, cwd: &Path, options: &Options) -> Result<BuildReport, String> {
    let rom = if options.source_only {
        None
    } else {
        let path = resolve(root, cwd, &options.rom);
        Some(std::fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))?)
    };
    let output = rooted(root, &options.output);
    std::fs::create_dir_all(&output).map_err(|error| format!("{}: {error}", output.display()))?;
    let mut sources = assembly_sources(&root.join("games/gs1/asm"))?;
    let mut stems = BTreeSet::new();
    for source in &sources {
        let name = stem(source);
        if !stems.insert(name.clone()) {
            return Err(format!("duplicate assembly source stem: {name}"));
        }
    }
    if let Some(selected) = options.source.as_deref() {
        let selected_path = rooted(root, selected);
        sources.retain(|source| *source == selected_path);
        if sources.is_empty() {
            return Err(format!("{selected}: assembly source not found"));
        }
    }
    if sources.is_empty() {
        return Err("no reconstructed assembly sources".into());
    }
    let layout = load_layout(root)?;
    let classification_path = root.join("games/gs1/asm/classification.json");
    let classification = load_classification(&classification_path)?;
    let explicit = explicit_classifications(&classification)?;
    let source_names: BTreeSet<String> = sources
        .iter()
        .map(|source| relative(root, source))
        .collect();
    if options.source.is_none() {
        for source in layout.keys() {
            if !source_names.contains(source) {
                return Err(format!("{source}: layout source not found"));
            }
        }
    }
    let cache = SqliteCache::open(&root.join("out/cache/asm-regions.sqlite3"))?;
    let binutils = production_binutil_signatures()?;
    let mut found = BTreeSet::new();
    let mut counts: BTreeMap<String, Count> = BTreeMap::new();
    let mut regions: Vec<(u64, Value)> = Vec::new();
    for source in &sources {
        let source_name = relative(root, source);
        let placement = layout.get(&source_name);
        let source_text = std::fs::read_to_string(source)
            .map_err(|error| format!("{}: {error}", source.display()))?;
        let built = build_region(
            root,
            source,
            source_text.as_bytes(),
            &output,
            &cache,
            placement.map(|item| item.run_address),
            &binutils,
        )
        .map_err(|error| format!("{source_name}: {error}"))?;
        if let Some(placement) = placement {
            if placement.address != built.address {
                return Err(format!("{source_name}: layout address differs"));
            }
        }
        let limit = rom
            .as_ref()
            .map_or(ROM_BASE + ROM_SIZE, |bytes| ROM_BASE + bytes.len() as u64);
        if built.address < ROM_BASE
            || built.address >= limit
            || built.data.is_empty()
            || built.address + built.data.len() as u64 > limit
        {
            return Err(format!(
                "{}: region outside ROM",
                source.file_name().unwrap().to_string_lossy()
            ));
        }
        if let Some(rom) = rom.as_ref() {
            let start = (built.address - ROM_BASE) as usize;
            if built.data != rom[start..start + built.data.len()] {
                return Err(format!(
                    "{}: assembled bytes differ",
                    source.file_name().unwrap().to_string_lossy()
                ));
            }
        }
        let name = stem(source);
        let category = classify(&name, &built.data, &source_text, &classification, &explicit)?;
        let count = counts.entry(category.kind.clone()).or_default();
        count.files += 1;
        count.bytes += built.data.len();
        found.insert(name);
        regions.push((
            built.address,
            region_value(&output, &source_name, &built, &category),
        ));
    }
    if options.source.is_none() {
        let alignment_path = root.join("games/gs1/asm/alignment.json");
        let category = classification
            .structural
            .iter()
            .find(|item| item.kind == "alignment_padding")
            .map(ClassificationRule::classification)
            .ok_or("missing alignment padding classification")?;
        for (address, data) in load_alignments(&alignment_path)? {
            let name = format!("{address:08x}");
            let output_path = output.join(format!("{name}.bin"));
            if let Some(rom) = rom.as_ref() {
                let start = (address - ROM_BASE) as usize;
                if data != rom[start..start + data.len()] {
                    return Err(format!("{name}: alignment bytes differ"));
                }
            }
            write(&output_path, &data)?;
            let count = counts.entry(category.kind.clone()).or_default();
            count.files += 1;
            count.bytes += data.len();
            let built = BuiltRegion {
                address,
                run_address: address,
                data,
            };
            regions.push((
                address,
                region_value(&output, &relative(root, &alignment_path), &built, &category),
            ));
        }
    }
    regions.sort_by_key(|item| item.0);
    let mut previous_end = 0u64;
    for (_, region) in &regions {
        let address = region["address"].as_u64().unwrap();
        if address < previous_end {
            return Err(format!("overlapping assembly region at 0x{address:x}"));
        }
        previous_end = address + region["size"].as_u64().unwrap();
    }
    if options.source.is_none() {
        for name in explicit.keys() {
            if !found.contains(name) {
                return Err(format!("classified assembly source is missing: {name}.s"));
            }
        }
        validate_counts(&classification, &counts)?;
    }
    let document = json!({
        "format":1,
        "rom_base":ROM_BASE,
        "verification":if options.source_only { "source_only" } else { "rom" },
        "classification":relative(root, &classification_path),
        "regions":regions.iter().map(|item| item.1.clone()).collect::<Vec<_>>(),
    });
    write_canonical(&output.join("manifest.json"), &document)?;
    let bytes = regions
        .iter()
        .map(|item| item.1["size"].as_u64().unwrap() as usize)
        .sum();
    let counts_text = counts
        .iter()
        .map(|(kind, count)| format!("{kind}={}/{}", count.files, count.bytes))
        .collect::<Vec<_>>()
        .join(" ");
    Ok(BuildReport {
        regions: regions.len(),
        bytes,
        counts: counts_text,
    })
}
#[cfg(test)]
mod tests {
    use super::thumb_standalone_wide_transfer;
    #[test]
    fn recognizes_only_standalone_wide_thumb_transfers() {
        assert!(!thumb_standalone_wide_transfer("\tldmia\tr3!, {r2}\n"));
        assert!(!thumb_standalone_wide_transfer("\tstmia\tr5!, {r0, r1}\n"));
        assert!(thumb_standalone_wide_transfer(
            "\tstmia\tr5!, {r0, r1, r2}\n"
        ));
        assert!(thumb_standalone_wide_transfer("\tldmia\tr3!, {r0-r3}\n"));
        assert!(!thumb_standalone_wide_transfer(
            "\tldmia\tr3!, {r0, r1, r2}\n\tstmia\tr4!, {r0-r2}\n"
        ));
    }
    #[test]
    fn ignores_comments_and_data() {
        assert!(!thumb_standalone_wide_transfer(
            "@ stmia r5!, {r0, r1, r2}\n"
        ));
        assert!(!thumb_standalone_wide_transfer(
            "\t.ascii \"ldmia {r0, r1, r2}\"\n"
        ));
        assert!(thumb_standalone_wide_transfer(
            ".L_copy: stmia r5!, {r0, r1, r2} @ targeted wide store\n"
        ));
        assert!(thumb_standalone_wide_transfer(
            "\tldmia r3!, {r0-r2}\n.L_target:\n\tstmia r4!, {r0-r2}\n"
        ));
    }
}
