//! Assemble retained source regions and emit their classified manifest.
use psynergy::process::run;
pub fn entry(arguments: &[String]) -> Result<(), String> {
    if arguments == ["--self-test"] {
        let sample = vec!["--source-only".to_string(), "--output=out/test".to_string()];
        match parse_args(&sample)? {
            ParseOutcome::Run(options) if options.source_only && options.output == "out/test" => {
                println!("self-test=ok");
                return Ok(());
            }
            _ => return Err("self-test failed".into()),
        }
    }
    let options = match parse_args(arguments)? {
        ParseOutcome::Help => {
            println!("usage: alchemy build asm [-h] [--target GAME-EDITION] [--source-only] [--output OUTPUT] [--source SOURCE] [rom]");
            return Ok(());
        }
        ParseOutcome::Run(options) => options,
    };
    let cwd = std::env::current_dir().map_err(|error| format!("cwd: {error}"))?;
    let report = build(&repository_root(), &cwd, &options)?;
    println!(
        "regions={} bytes={}\n{}",
        report.regions, report.bytes, report.counts
    );
    Ok(())
}
use crate::compiler::canonical_json::write_canonical;
use crate::compiler::{
    build_io::{argv, read, relative, rooted, text, write},
    bundle::host_executable_signature,
    runtime, sha256,
};
use psynergy::cache::SqliteCache;
use serde_json::{json, Value};
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};
const ROM_BASE: u64 = 0x0800_0000;
const ROM_SIZE: u64 = 0x0080_0000;
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Options {
    pub rom: String,
    pub output: String,
    pub source: Option<String>,
    pub source_only: bool,
    /// The target's retained assembly root, `games/<GAME>/asm`.
    pub asm_dir: String,
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
struct Classification {
    kind: String,
    origin: String,
    retention: String,
    confidence: String,
    evidence: Vec<String>,
    provenance: ClassificationProvenance,
}
#[derive(Debug, Clone, Default)]
struct ClassificationProvenance {
    credit: String,
    proof: String,
    object: String,
    evidence: Vec<String>,
}
#[derive(Debug, Clone, Copy, Default)]
struct Count {
    files: usize,
    bytes: usize,
}
pub fn repository_root() -> PathBuf {
    crate::compiler::routing::root().to_path_buf()
}
pub fn parse_args(argv: &[String]) -> Result<ParseOutcome, String> {
    let mut target = crate::targets::decomp_target(None)?;
    let mut options = Options {
        rom: String::new(),
        output: String::new(),
        source: None,
        source_only: false,
        asm_dir: String::new(),
    };
    let mut positional = false;
    let mut index = 0usize;
    while index < argv.len() {
        let argument = &argv[index];
        if argument == "-h" || argument == "--help" {
            return Ok(ParseOutcome::Help);
        } else if argument == "--source-only" {
            options.source_only = true;
        } else if argument == "--target" {
            index += 1;
            target = crate::targets::decomp_target(Some(
                argv.get(index).ok_or("--target requires a value")?,
            ))?;
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
    if !positional {
        options.rom = target.rom.into();
    }
    if options.output.is_empty() {
        options.output = format!("{}/asm", target.output_dir);
    }
    options.asm_dir = target.asm_dir.into();
    Ok(ParseOutcome::Run(options))
}
fn resolve(root: &Path, cwd: &Path, value: &str) -> PathBuf {
    let path = Path::new(value);
    if path.is_absolute() {
        path.to_path_buf()
    } else if value.starts_with("out/") || value.starts_with("games/") {
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

#[derive(Clone, Debug)]
pub(crate) struct MaintainedAssembly {
    pub source: PathBuf,
    pub load_address: u64,
    pub run_address: u64,
}

fn source_address(line: &str) -> Option<u64> {
    let label = line.trim().strip_suffix(':')?;
    let suffix = label.rsplit_once('_')?.1;
    (suffix.len() == 8 && suffix.starts_with("08"))
        .then(|| u64::from_str_radix(suffix, 16).ok())
        .flatten()
}

fn declared_address(text: &str, name: &str) -> Option<u64> {
    text.lines().find_map(|line| {
        let rest = line.trim().strip_prefix(".set")?.trim();
        let (symbol, value) = rest.split_once(',')?;
        (symbol.trim() == name)
            .then(|| u64::from_str_radix(value.trim().trim_start_matches("0x"), 16).ok())
            .flatten()
    })
}

fn asset_assembly_sources(
    root: &Path,
    target: crate::targets::DecompTarget,
) -> Result<BTreeSet<PathBuf>, String> {
    let path = root.join(target.asset_manifest);
    if !path.is_file() {
        return Ok(BTreeSet::new());
    }
    let value: Value =
        serde_json::from_slice(&std::fs::read(&path).map_err(|error| error.to_string())?)
            .map_err(|error| error.to_string())?;
    fn collect(value: &Value, root: &Path, output: &mut BTreeSet<PathBuf>) {
        match value {
            Value::Object(object) => {
                if let Some(source) = object.get("source").and_then(Value::as_str) {
                    if source.ends_with(".S") {
                        output.insert(root.join(source));
                    }
                }
                for child in object.values() {
                    collect(child, root, output);
                }
            }
            Value::Array(array) => {
                for child in array {
                    collect(child, root, output);
                }
            }
            _ => {}
        }
    }
    let mut output = BTreeSet::new();
    collect(&value, root, &mut output);
    Ok(output)
}

pub(crate) fn maintained_assembly(
    root: &Path,
    target: crate::targets::DecompTarget,
) -> Result<Vec<MaintainedAssembly>, String> {
    let assets = asset_assembly_sources(root, target)?;
    let mut modules = Vec::new();
    for entry in walkdir::WalkDir::new(root.join(target.source_dir)) {
        let entry = entry.map_err(|error| error.to_string())?;
        let source = entry.path();
        if !entry.file_type().is_file()
            || source.extension().and_then(|value| value.to_str()) != Some("S")
            || assets.contains(source)
        {
            continue;
        }
        let text = std::fs::read_to_string(source).map_err(|error| error.to_string())?;
        let labels = text
            .lines()
            .filter_map(source_address)
            .collect::<BTreeSet<_>>();
        let load_address = declared_address(&text, "AlchemyLoadAddress")
            .or_else(|| labels.iter().next().copied())
            .ok_or_else(|| {
                format!(
                    "{}: maintained assembly needs an address label or AlchemyLoadAddress",
                    source.display()
                )
            })?;
        let run_address = declared_address(&text, "AlchemyRunAddress").unwrap_or(load_address);
        modules.push(MaintainedAssembly {
            source: source.to_path_buf(),
            load_address,
            run_address,
        });
    }
    modules.sort_by_key(|module| module.load_address);
    Ok(modules)
}

pub(crate) fn maintained_assembly_bytes(
    root: &Path,
    target: crate::targets::DecompTarget,
) -> Result<Vec<(MaintainedAssembly, Vec<u8>)>, String> {
    let modules = maintained_assembly(root, target)?;
    let output = tempfile::tempdir().map_err(|error| error.to_string())?;
    let cache = SqliteCache::open(&root.join("out/cache/asm-regions.sqlite3"))?;
    let binutils = production_binutil_signatures()?;
    modules
        .into_iter()
        .map(|module| {
            let source = std::fs::read(&module.source)
                .map_err(|error| format!("{}: {error}", module.source.display()))?;
            let built = build_region(
                root,
                &module.source,
                &source,
                output.path(),
                &cache,
                Some(module.run_address),
                Some(module.load_address),
                &binutils,
            )?;
            Ok((module, built.data))
        })
        .collect()
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
/// Cache identity includes the running implementation, ordered tool signatures,
/// complete source, and linked address; serialization frames each field.
pub fn region_cache_key_with_signatures(
    source: &[u8],
    linked_address: u64,
    binutils: &[(String, String)],
) -> Result<String, String> {
    let identity = (
        "build-asm-cache-v3",
        crate::compiler::bundle::executable_signature()?,
        linked_address,
        binutils,
        sha256::hex(source),
    );
    Ok(sha256::hex(
        &serde_json::to_vec(&identity).map_err(|error| error.to_string())?,
    ))
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
    load_address: Option<u64>,
    binutils: &[(String, String)],
) -> Result<BuiltRegion, String> {
    let source_stem = stem(source);
    let address = load_address
        .or_else(|| u64::from_str_radix(&source_stem, 16).ok())
        .ok_or_else(|| {
            format!(
                "{}: named source requires a manifest address",
                source.display()
            )
        })?;
    let name = format!("{address:08x}");
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
        &argv(&[
            "arm-none-eabi-as",
            "-mcpu=arm7tdmi",
            "-mthumb-interwork",
            "-o",
            &text(&object),
            &text(source),
        ]),
        root,
    )?;
    let undefined = run(&argv(&["arm-none-eabi-nm", "-u", &text(&object)]), root)?;
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
            &argv(&[
                "arm-none-eabi-as",
                "-mcpu=arm7tdmi",
                "-mthumb-interwork",
                "-o",
                &text(&symbols_object),
                &text(&symbols_source),
            ]),
            root,
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
    run(&link, root)?;
    run(
        &argv(&[
            "arm-none-eabi-objcopy",
            "-O",
            "binary",
            "-j",
            ".text",
            &text(&elf),
            &text(&binary),
        ]),
        root,
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
    let mut value = json!({
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
    });
    if !category.provenance.credit.is_empty() {
        value["provenance"] = json!({
            "credit": category.provenance.credit,
            "proof": category.provenance.proof,
            "object": category.provenance.object,
            "evidence": category.provenance.evidence,
        });
    }
    value
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
    let asm = root.join(&options.asm_dir);
    let mut sources = assembly_sources(&asm)?;
    sources.sort();
    // These packages are assembled through the asset manifest, with their own
    // placement and compression. They are not standalone main-image regions.
    sources.retain(|source| {
        !source.starts_with(asm.join("overlays")) && !source.starts_with(asm.join("battle"))
    });
    let target = match options.asm_dir.as_str() {
        "games/THE BROKEN SEAL/raw" => {
            crate::targets::target_for(crate::targets::DecompTargetId::TbsEn)
        }
        "games/THE LOST AGE/raw" => {
            crate::targets::target_for(crate::targets::DecompTargetId::TlaEn)
        }
        _ => return Err(format!("unsupported assembly root {}", options.asm_dir)),
    };
    let maintained = maintained_assembly(root, target)?;
    sources.extend(maintained.iter().map(|module| module.source.clone()));
    sources.sort();
    let maintained = maintained
        .into_iter()
        .map(|module| (module.source.clone(), module))
        .collect::<BTreeMap<_, _>>();
    let mut addresses = BTreeSet::new();
    for source in &sources {
        let source_name = relative(root, source);
        let address = maintained
            .get(source)
            .map(|module| module.load_address)
            .or_else(|| u64::from_str_radix(&stem(source), 16).ok())
            .ok_or_else(|| format!("{source_name}: raw source filename must be its ROM address"))?;
        if !addresses.insert(address) {
            return Err(format!("duplicate assembly load address: 0x{address:08x}"));
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
    let cache = SqliteCache::open(&root.join("out/cache/asm-regions.sqlite3"))?;
    let binutils = production_binutil_signatures()?;
    let mut counts: BTreeMap<String, Count> = BTreeMap::new();
    let mut regions: Vec<(u64, Value)> = Vec::new();
    for source in &sources {
        let source_name = relative(root, source);
        let source_text = std::fs::read_to_string(source)
            .map_err(|error| format!("{}: {error}", source.display()))?;
        let module = maintained.get(source);
        let built = build_region(
            root,
            source,
            source_text.as_bytes(),
            &output,
            &cache,
            module.map(|module| module.run_address),
            module.map(|module| module.load_address),
            &binutils,
        )
        .map_err(|error| format!("{source_name}: {error}"))?;
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
        let category = Classification {
            kind: if module.is_some() {
                "maintained_assembly"
            } else {
                "raw_assembly"
            }
            .into(),
            origin: if module.is_some() {
                "source"
            } else {
                "unresolved"
            }
            .into(),
            retention: if module.is_some() {
                "keep_asm"
            } else {
                "c_candidate"
            }
            .into(),
            confidence: if module.is_some() {
                "verified"
            } else {
                "unknown"
            }
            .into(),
            evidence: Vec::new(),
            provenance: ClassificationProvenance::default(),
        };
        let count = counts.entry(category.kind.clone()).or_default();
        count.files += 1;
        count.bytes += built.data.len();
        regions.push((
            built.address,
            region_value(&output, &source_name, &built, &category),
        ));
    }
    let runtime_game = Path::new(runtime::REGISTRY).starts_with(
        Path::new(&options.asm_dir)
            .parent()
            .unwrap_or(Path::new("")),
    );
    if options.source.is_none() && runtime_game {
        // Main-image compiler runtime links are built from the licensed
        // container; no tracked source holds their code.
        let category = Classification {
            kind: "compiler_runtime".into(),
            origin: "compiler_runtime".into(),
            retention: "container_runtime".into(),
            confidence: "proven".into(),
            evidence: vec!["built_from_licensed_compiler_container".into()],
            provenance: ClassificationProvenance::default(),
        };
        let registry = runtime::Registry::load(root)?;
        for link in registry.links_for("main") {
            let data = runtime::build(root, link)?.text;
            let address = u64::from(link.text);
            let name = format!("{address:08x}");
            if let Some(rom) = rom.as_ref() {
                let start = (address - ROM_BASE) as usize;
                if rom.get(start..start + data.len()) != Some(data.as_slice()) {
                    return Err(format!("{name}: container-built runtime bytes differ"));
                }
            }
            write(output.join(format!("{name}.bin")), &data)?;
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
                region_value(&output, runtime::REGISTRY, &built, &category),
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
    let document = json!({
        "format":1,
        "rom_base":ROM_BASE,
        "verification":if options.source_only { "source_only" } else { "rom" },
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
