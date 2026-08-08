//! Assemble retained source regions and emit their classified manifest.

use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};
use std::process::Command;

use alchemy_bundle::sha256;
use cache_entry::write_cache_entry_atomically;
use canonical_json::canonical_json;
use serde::Deserialize;
use serde_json::{Map, Number, Value};

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
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("build-asm lives under tools")
        .to_path_buf()
}

pub fn parse_args(argv: &[String]) -> Result<ParseOutcome, String> {
    let mut options = Options {
        rom: "roms/gs1-en.gba".into(),
        output: "out/asm".into(),
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
    } else if value.starts_with("out/") || value.starts_with("asm/") {
        root.join(path)
    } else {
        cwd.join(path)
    }
}

fn rooted(root: &Path, value: &str) -> PathBuf {
    let path = Path::new(value);
    if path.is_absolute() {
        path.to_path_buf()
    } else {
        root.join(path)
    }
}

fn relative(root: &Path, path: &Path) -> String {
    path.strip_prefix(root)
        .unwrap_or(path)
        .to_string_lossy()
        .into_owned()
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

fn read_json<T: for<'de> Deserialize<'de>>(path: &Path) -> Result<T, String> {
    let bytes = std::fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?;
    serde_json::from_slice(&bytes).map_err(|error| format!("{}: {error}", path.display()))
}

fn load_layout(root: &Path) -> Result<BTreeMap<String, Placement>, String> {
    let path = root.join("asm/manifest.json");
    if !path.exists() {
        return Ok(BTreeMap::new());
    }
    let value: Value = read_json(&path)?;
    if value["format"].as_u64() != Some(1) || !value["regions"].is_array() {
        return Err("asm/manifest.json: unsupported format".into());
    }
    let regions: Vec<LayoutRegion> = serde_json::from_value(value["regions"].clone())
        .map_err(|_| "asm/manifest.json: unsupported format".to_string())?;
    let mut result = BTreeMap::new();
    for item in regions {
        if result.contains_key(&item.source) {
            return Err("asm/manifest.json: invalid or duplicate source".into());
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

fn classify(
    name: &str,
    data: &[u8],
    config: &ClassificationConfig,
    explicit: &BTreeMap<String, ClassificationRule>,
) -> Result<Classification, String> {
    if let Some(fixed) = explicit.get(name) {
        return Ok(fixed.classification());
    }
    if long_call_veneer(data) {
        return config
            .structural
            .iter()
            .find(|rule| rule.kind == "linker_veneer")
            .map(ClassificationRule::classification)
            .ok_or_else(|| "missing linker veneer classification".into());
    }
    if alignment_padding(data) {
        return config
            .structural
            .iter()
            .find(|rule| rule.kind == "alignment_padding")
            .map(ClassificationRule::classification)
            .ok_or_else(|| "missing alignment padding classification".into());
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
    let bytes = std::fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))?;
    if bytes.is_empty() {
        return Err(format!(
            "build_asm read an EMPTY source at {}; refusing to key the cache",
            path.display()
        ));
    }
    Ok(sha256::hex(&bytes))
}

pub fn region_cache_key(source: &[u8], linked_address: u64) -> Result<String, String> {
    let mut bytes = format!("asm:{}:{linked_address:x}\0", self_digest()?).into_bytes();
    bytes.extend_from_slice(source);
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
    output_dir: &Path,
    cache_dir: &Path,
    run_address: Option<u64>,
) -> Result<BuiltRegion, String> {
    let name = stem(source);
    let address = u64::from_str_radix(&name, 16)
        .map_err(|_| format!("{}: invalid assembly filename", source.display()))?;
    let linked_address = run_address.unwrap_or(address);
    let object = output_dir.join(format!("{name}.o"));
    let elf = output_dir.join(format!("{name}.elf"));
    let binary = output_dir.join(format!("{name}.bin"));
    let source_bytes =
        std::fs::read(source).map_err(|error| format!("{}: {error}", source.display()))?;
    let cached = cache_dir.join(format!(
        "{}.bin",
        region_cache_key(&source_bytes, linked_address)?
    ));
    if let Ok(data) = std::fs::read(&cached) {
        std::fs::write(&binary, &data).map_err(|error| format!("{}: {error}", binary.display()))?;
        return Ok(BuiltRegion {
            address,
            run_address: linked_address,
            data,
        });
    }
    run(
        root,
        &[
            "arm-none-eabi-as".into(),
            "-mcpu=arm7tdmi".into(),
            "-mthumb-interwork".into(),
            "-o".into(),
            object.to_string_lossy().into_owned(),
            source.to_string_lossy().into_owned(),
        ],
    )?;
    let undefined = run(
        root,
        &[
            "arm-none-eabi-nm".into(),
            "-u".into(),
            object.to_string_lossy().into_owned(),
        ],
    )?;
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
        std::fs::write(&symbols_source, body)
            .map_err(|error| format!("{}: {error}", symbols_source.display()))?;
        run(
            root,
            &[
                "arm-none-eabi-as".into(),
                "-mcpu=arm7tdmi".into(),
                "-mthumb-interwork".into(),
                "-o".into(),
                symbols_object.to_string_lossy().into_owned(),
                symbols_source.to_string_lossy().into_owned(),
            ],
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
        elf.to_string_lossy().into_owned(),
    ];
    link.extend(
        objects
            .iter()
            .map(|path| path.to_string_lossy().into_owned()),
    );
    run(root, &link)?;
    run(
        root,
        &[
            "arm-none-eabi-objcopy".into(),
            "-O".into(),
            "binary".into(),
            "-j".into(),
            ".text".into(),
            elf.to_string_lossy().into_owned(),
            binary.to_string_lossy().into_owned(),
        ],
    )?;
    let data = std::fs::read(&binary).map_err(|error| format!("{}: {error}", binary.display()))?;
    std::fs::create_dir_all(cache_dir)
        .map_err(|error| format!("{}: {error}", cache_dir.display()))?;
    write_cache_entry_atomically(&cached, &data)
        .map_err(|error| format!("{}: {error}", cached.display()))?;
    Ok(BuiltRegion {
        address,
        run_address: linked_address,
        data,
    })
}

fn number(value: u64) -> Value {
    Value::Number(Number::from(value))
}

fn region_value(
    output: &Path,
    source: &str,
    built: &BuiltRegion,
    category: &Classification,
) -> Value {
    let mut object = Map::new();
    object.insert("address".into(), number(built.address));
    object.insert("run_address".into(), number(built.run_address));
    object.insert("size".into(), number(built.data.len() as u64));
    object.insert("source".into(), Value::String(source.to_string()));
    object.insert(
        "output".into(),
        Value::String(
            output
                .join(format!("{:08x}.bin", built.address))
                .to_string_lossy()
                .into_owned(),
        ),
    );
    object.insert("kind".into(), Value::String(category.kind.clone()));
    object.insert("origin".into(), Value::String(category.origin.clone()));
    object.insert(
        "retention".into(),
        Value::String(category.retention.clone()),
    );
    object.insert(
        "confidence".into(),
        Value::String(category.confidence.clone()),
    );
    object.insert(
        "evidence".into(),
        Value::String(category.evidence.join(",")),
    );
    Value::Object(object)
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
    let mut sources = assembly_sources(&root.join("asm"))?;
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
    let classification_path = root.join("asm/classification.json");
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
    let cache = root.join("out/cache/asm-regions");
    let mut found = BTreeSet::new();
    let mut counts: BTreeMap<String, Count> = BTreeMap::new();
    let mut regions: Vec<(u64, Value)> = Vec::new();
    for source in &sources {
        let source_name = relative(root, source);
        let placement = layout.get(&source_name);
        let built = build_region(
            root,
            source,
            &output,
            &cache,
            placement.map(|item| item.run_address),
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
        let category = classify(&name, &built.data, &classification, &explicit)?;
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
        let alignment_path = root.join("asm/alignment.json");
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
            std::fs::write(&output_path, &data)
                .map_err(|error| format!("{}: {error}", output_path.display()))?;
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
    let mut document = Map::new();
    document.insert("format".into(), number(1));
    document.insert("rom_base".into(), number(ROM_BASE));
    document.insert(
        "verification".into(),
        Value::String(
            if options.source_only {
                "source_only"
            } else {
                "rom"
            }
            .into(),
        ),
    );
    document.insert(
        "classification".into(),
        Value::String(relative(root, &classification_path)),
    );
    document.insert(
        "regions".into(),
        Value::Array(regions.iter().map(|item| item.1.clone()).collect()),
    );
    std::fs::write(
        output.join("manifest.json"),
        format!("{}\n", canonical_json(&Value::Object(document))),
    )
    .map_err(|error| format!("{}: {error}", output.join("manifest.json").display()))?;
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
    use super::*;

    fn args(values: &[&str]) -> Vec<String> {
        values.iter().map(|value| value.to_string()).collect()
    }

    #[test]
    fn argument_forms_match() {
        assert_eq!(
            parse_args(&args(&[
                "--source-only",
                "--output=out/x",
                "--source",
                "asm/a.s"
            ]))
            .unwrap(),
            ParseOutcome::Run(Options {
                rom: "roms/gs1-en.gba".into(),
                output: "out/x".into(),
                source: Some("asm/a.s".into()),
                source_only: true,
            })
        );
        assert!(parse_args(&args(&["--source-only", "rom.gba"])).is_err());
        assert_eq!(parse_args(&args(&["--help"])).unwrap(), ParseOutcome::Help);
    }

    #[test]
    fn classifications_recognise_structural_bytes() {
        assert!(long_call_veneer(&[0x00, 0x4c, 0x20, 0x47, 1, 0, 0, 8]));
        assert!(alignment_padding(&[0, 0]));
        assert!(!alignment_padding(&[0, 1]));
    }

    #[test]
    fn external_symbol_grammar_is_strict() {
        assert!(valid_external("Func_08001234"));
        assert!(valid_external("Data_02001234"));
        assert!(!valid_external("func_08001234"));
        assert!(!valid_external("Func_0800123G"));
    }

    #[test]
    fn cache_key_changes_with_source_and_address() {
        assert_ne!(
            region_cache_key(b"a", 1).unwrap(),
            region_cache_key(b"b", 1).unwrap()
        );
        assert_ne!(
            region_cache_key(b"a", 1).unwrap(),
            region_cache_key(b"a", 2).unwrap()
        );
    }

    #[test]
    fn addresses_accept_decimal_and_javascript_style_hex_strings() {
        assert_eq!(
            integer(&Value::String("134217728".into()), "address").unwrap(),
            ROM_BASE
        );
        assert_eq!(
            integer(&Value::String("0x08000000".into()), "address").unwrap(),
            ROM_BASE
        );
    }
}
