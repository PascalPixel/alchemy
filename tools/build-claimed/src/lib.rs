//! Compile, verify, cache, and manifest every exact C owner in the main image.
pub mod cli;
use cache_entry::write_cache_entry_atomically;
use candidate_compiler::verify::{
    verify_candidate_owned_routed_with_object, CandidateCompilerConfiguration,
};
use canonical_json::canonical_json;
use compiler_core::bundle::{compiler_bundle_signature, host_executable_signature};
use compiler_core::plan::{source_to_assembly_plan, SourceToAssemblyPlanOptions};
use compiler_core::routing::CompilerTarget;
use compiler_core::source_inputs::compiler_source_tree_signature;
use compiler_core::source_paths::{SourceFile, SourceOwner, SourcePaths};
use compiler_core::symbols::{external_symbol, external_symbol_assembly, CALL_VIA_BASE};
use compiler_core::translation_units::{OwnerState, TranslationUnit, TranslationUnits};
use decomp_targets::{
    decomp_target, parse_decomp_target, target_for, BuildSupport, DecompCompilerTarget,
    DecompTarget, DecompTargetId, DEFAULT_TARGET,
};
use serde_json::{json, Value};
use sha2::{Digest, Sha256};
use std::{
    collections::{BTreeMap, BTreeSet},
    io::Write,
    path::{Path, PathBuf},
    process::Command,
    sync::{
        atomic::{AtomicUsize, Ordering},
        Mutex,
    },
};
pub const ROM_BASE: u32 = 0x0800_0000;
pub type Result<T> = std::result::Result<T, String>;
const BINUTILS: [&str; 2] = ["arm-none-eabi-as", "arm-none-eabi-nm"];
fn digest(bytes: &[u8]) -> String {
    let mut h = Sha256::new();
    h.update(bytes);
    format!("{:x}", h.finalize())
}
fn basename(path: &str) -> &str {
    Path::new(path)
        .file_name()
        .and_then(|s| s.to_str())
        .unwrap_or(path)
}
fn text(path: PathBuf) -> String {
    path.to_string_lossy().into_owned()
}
pub fn root() -> String {
    text(compiler_core::routing::root().to_path_buf())
}
fn strings(items: &[&str]) -> Vec<String> {
    items.iter().map(|s| (*s).into()).collect()
}
fn address(value: &str) -> Option<u32> {
    u32::from_str_radix(value.trim_start_matches("0x"), 16).ok()
}
fn parse_jobs(value: &str) -> f64 {
    let s = value.trim();
    let sign = if s.starts_with('-') { -1.0 } else { 1.0 };
    let d: String = s
        .trim_start_matches(['+', '-'])
        .chars()
        .take_while(char::is_ascii_digit)
        .collect();
    if d.is_empty() {
        f64::NAN
    } else {
        sign * d.parse::<f64>().unwrap_or(f64::NAN)
    }
}
fn function_name(name: &str) -> bool {
    let Some(rest) = name.strip_prefix("Func_") else {
        return false;
    };
    rest.len() == 8
        && rest
            .bytes()
            .all(|b| b.is_ascii_digit() || (b'a'..=b'f').contains(&b))
}
fn last_fields(output: &str) -> Vec<String> {
    output
        .lines()
        .filter(|l| !l.is_empty())
        .filter_map(|l| l.split_whitespace().last())
        .map(str::to_string)
        .collect()
}
fn subarray(bytes: &[u8], start: u32, end: u32) -> &[u8] {
    let (start, end) = (start as usize, end as usize);
    if start >= bytes.len() || end <= start {
        &[]
    } else {
        &bytes[start..end.min(bytes.len())]
    }
}
pub fn object_cache_key(source: &[u8], plan: &str) -> String {
    let mut input = Vec::with_capacity(plan.len() + source.len() + 1);
    input.extend_from_slice(plan.as_bytes());
    input.push(0);
    input.extend_from_slice(source);
    digest(&input)
}
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct CacheSignatures {
    compiler_bundle: String,
    binutils: String,
    implementation: String,
}
impl CacheSignatures {
    pub fn production() -> Result<Self> {
        let executable = std::env::current_exe().map_err(|e| e.to_string())?;
        Ok(Self {
            compiler_bundle: compiler_bundle_signature(),
            binutils: host_executable_signature(&BINUTILS)?,
            implementation: digest(&std::fs::read(executable).map_err(|e| e.to_string())?),
        })
    }
}
fn frame(out: &mut Vec<u8>, bytes: &[u8]) {
    out.extend_from_slice(&(bytes.len() as u64).to_be_bytes());
    out.extend_from_slice(bytes);
}
fn command_frame(commands: &[Vec<String>]) -> Vec<u8> {
    let mut out = Vec::new();
    frame(&mut out, &(commands.len() as u64).to_be_bytes());
    for command in commands {
        frame(&mut out, &(command.len() as u64).to_be_bytes());
        for arg in command {
            frame(&mut out, arg.as_bytes());
        }
    }
    out
}
pub fn toolchain_stamp_with_signatures(commands: &[Vec<String>], sig: &CacheSignatures) -> String {
    let mut out = Vec::new();
    frame(
        &mut out,
        b"build-claimed cache identity: framed argv and signed toolchain",
    );
    frame(&mut out, &command_frame(commands));
    frame(&mut out, sig.compiler_bundle.as_bytes());
    frame(&mut out, sig.binutils.as_bytes());
    frame(&mut out, sig.implementation.as_bytes());
    digest(&out)
}
pub fn module_end(names: &[String], symbols: &BTreeMap<String, (u32, usize)>) -> Result<u32> {
    names
        .iter()
        .map(|name| {
            symbols
                .get(name)
                .copied()
                .filter(|(_, size)| *size > 0)
                .map(|(a, s)| a.saturating_add(s as u32))
                .ok_or_else(|| format!("invalid module symbol {name}"))
        })
        .max()
        .transpose()?
        .ok_or_else(|| "C module has no functions".into())
}
fn linked_function_record(
    root: &str,
    symbol: &str,
    linked_address: u32,
    size: usize,
    source: &str,
    module_symbol: &str,
    unit: Option<&TranslationUnit>,
) -> Result<Value> {
    if !function_name(symbol)
        || address(symbol.strip_prefix("Func_").unwrap_or_default()) != Some(linked_address)
        || size == 0
        || !function_name(module_symbol)
    {
        return Err(format!("invalid linked function {symbol}"));
    }
    Ok(json!({
        "symbol":symbol,"address":linked_address,"size":size,"source":relative(root,source),
        "module":module_symbol,"extent_evidence":"linked-elf-symbol",
        "translation_unit":unit.map(|unit| unit.id.as_str()),
    }))
}
fn module_contract(
    root: &Path,
    game: &str,
    source: &SourceFile,
    units: &TranslationUnits,
) -> Result<Vec<(String, u32, usize)>> {
    let Some(unit) = units.units.iter().find(|unit| {
        unit.game == game
            && unit.overlay.is_none()
            && unit
                .owners
                .iter()
                .any(|owner| owner.address == source.owner.address())
    }) else {
        return Ok(Vec::new());
    };
    if root.join(&unit.source) == source.path {
        if !unit.exact() {
            return Err(format!(
                "{}: grouped production C must be wholly exact",
                unit.id
            ));
        }
        return unit
            .symbols()
            .map(|(address, _, extent)| {
                Ok((unit.source_owner(address)?.legacy_name(), address, extent))
            })
            .collect();
    }
    let owner = unit
        .owners
        .iter()
        .find(|owner| owner.address == source.owner.address())
        .filter(|owner| owner.state == OwnerState::ExactC)
        .ok_or_else(|| format!("{}: C source is not an exact owner", source.owner.id()))?;
    Ok(vec![(
        source.owner.legacy_name(),
        owner.address,
        owner.extent,
    )])
}
pub fn self_test() -> Result<String> {
    let key = object_cache_key(b"void Func_08000000(void) {}\n", "plan-a");
    if key != object_cache_key(b"void Func_08000000(void) {}\n", "plan-a")
        || key == object_cache_key(b"changed", "plan-a")
        || key == object_cache_key(b"void Func_08000000(void) {}\n", "plan-b")
    {
        return Err("object cache key self-test failed".into());
    }
    Ok("self-test=ok".into())
}
#[derive(Debug, Clone, PartialEq)]
pub struct Options {
    pub target: DecompTargetId,
    pub rom: String,
    pub jobs: f64,
    pub output: String,
    pub source_only: bool,
    pub compile_only: bool,
}
#[derive(Debug, Clone, PartialEq)]
pub enum ParsedArgs {
    Help,
    Run(Box<Options>),
}
pub fn usage_text() -> &'static str {
    "usage: build-claimed [-h] [--target GAME-EDITION] [--compile-only|--source-only] [--jobs JOBS] [--output OUTPUT] [rom]"
}
pub fn default_jobs() -> f64 {
    std::thread::available_parallelism().map_or(1, |n| n.get().min(16)) as f64
}
pub fn parse_args(argv: &[String]) -> Result<ParsedArgs> {
    let mut id = DEFAULT_TARGET;
    for (i, arg) in argv.iter().enumerate() {
        if arg == "--target" {
            id = parse_decomp_target(argv.get(i + 1).ok_or("--target requires a value")?)?;
        } else if let Some(v) = arg.strip_prefix("--target=") {
            id = parse_decomp_target(v)?;
        }
    }
    let target = target_for(id);
    let mut options = Options {
        target: id,
        rom: target.rom.into(),
        jobs: default_jobs(),
        output: text(Path::new(target.output_dir).join("claimed")),
        source_only: false,
        compile_only: false,
    };
    let mut positional = false;
    let mut i = 0;
    while i < argv.len() {
        let arg = &argv[i];
        match arg.as_str() {
            "-h" | "--help" => return Ok(ParsedArgs::Help),
            "--source-only" => options.source_only = true,
            "--compile-only" => options.compile_only = true,
            "--target" => i += 1,
            "--jobs" | "--output" => {
                let value = argv
                    .get(i + 1)
                    .ok_or_else(|| format!("{arg} requires a value"))?;
                if arg == "--jobs" {
                    options.jobs = parse_jobs(value);
                } else {
                    options.output = value.clone();
                }
                i += 1;
            }
            _ if arg.starts_with("--target=") => {}
            _ if let Some(v) = arg.strip_prefix("--jobs=") => options.jobs = parse_jobs(v),
            _ if let Some(v) = arg.strip_prefix("--output=") => options.output = v.into(),
            _ if !arg.starts_with('-') && !positional => {
                options.rom = arg.clone();
                positional = true;
            }
            _ => return Err(format!("unrecognized argument: {arg}")),
        }
        i += 1;
    }
    if !options.jobs.is_finite() || options.jobs.fract() != 0.0 || options.jobs < 1.0 {
        return Err("jobs must be positive".into());
    }
    if options.source_only && options.compile_only {
        return Err("--source-only and --compile-only are mutually exclusive".into());
    }
    if (options.source_only || options.compile_only) && positional {
        return Err(format!(
            "{} does not accept a ROM",
            if options.compile_only {
                "--compile-only"
            } else {
                "--source-only"
            }
        ));
    }
    Ok(ParsedArgs::Run(Box::new(options)))
}

pub fn run(root: &str, command: &[String]) -> Result<String> {
    let program = command.first().ok_or("run() requires a command")?;
    let output = Command::new(program)
        .args(&command[1..])
        .current_dir(root)
        .output()
        .map_err(|e| format!("{}: {e}", basename(program)))?;
    let stdout = String::from_utf8_lossy(&output.stdout).into_owned();
    let stderr = String::from_utf8_lossy(&output.stderr).into_owned();
    if !output.status.success() {
        let detail = if stderr.trim().is_empty() {
            stdout.trim()
        } else {
            stderr.trim()
        };
        return Err(format!("{} failed: {detail}", basename(program)));
    }
    Ok(stdout)
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Compiled {
    pub object: String,
    pub defined_names: Vec<String>,
    pub undefined_names: Vec<String>,
}

#[allow(clippy::too_many_arguments)]
fn materialize_unit_owner(
    root: &str,
    unit: &TranslationUnit,
    owner: u32,
    extent: usize,
    object: &str,
    object_dir: &Path,
    compiler: CompilerTarget,
    rom: Option<&Vec<u8>>,
) -> Result<Compiled> {
    let stem = format!("{owner:08x}");
    let source = text(Path::new(root).join(&unit.source));
    let work = text(object_dir.join("tu").join(&unit.id));
    let verification = verify_candidate_owned_routed_with_object(
        &source,
        &unit
            .source_owner(unit.owners[0].address)?
            .routing_path()
            .to_string_lossy(),
        &stem,
        rom.map(Vec::as_slice).unwrap_or_default(),
        &work,
        &[],
        f64::from(ROM_BASE),
        compiler,
        &CandidateCompilerConfiguration {
            absolute_symbols: unit.canonical_symbols()?,
            ..Default::default()
        },
        Some(object),
    )?;
    if verification.actual.len() != extent
        || rom.is_some() && verification.actual != verification.expected
    {
        return Err(format!("{}: Func_{stem} is not byte-exact", unit.id));
    }
    let assembly = object_dir.join(format!("{stem}.s"));
    let slice_assembly = Path::new(&work).join(format!("{stem}.slice.s"));
    let output = object_dir.join(format!("{stem}.o"));
    let mut generated = format!(".syntax unified\n.thumb\n.global Func_{stem}\n.type Func_{stem},%function\n.thumb_func\nFunc_{stem}:\n");
    for bytes in verification.actual.chunks(16) {
        generated.push_str(".byte ");
        generated.push_str(
            &bytes
                .iter()
                .map(|byte| format!("0x{byte:02x}"))
                .collect::<Vec<_>>()
                .join(","),
        );
        generated.push('\n');
    }
    generated.push_str(&format!(".size Func_{stem},.-Func_{stem}\n"));
    write_file(&slice_assembly, generated.as_bytes())?;
    run(
        root,
        &strings(&[
            "arm-none-eabi-as",
            "-mcpu=arm7tdmi",
            "-mthumb-interwork",
            "-o",
            &text(output.clone()),
            &text(slice_assembly),
        ]),
    )?;
    std::fs::copy(Path::new(object).with_extension("s"), &assembly)
        .map_err(|error| format!("{}: {error}", assembly.display()))?;
    Ok(Compiled {
        object: text(output),
        defined_names: vec![format!("Func_{stem}")],
        undefined_names: Vec::new(),
    })
}
fn write_cache(items: &[(&str, &[u8])]) -> Result<()> {
    for (path, bytes) in items {
        write_cache_entry_atomically(Path::new(path), bytes).map_err(|e| format!("{path}: {e}"))?;
    }
    Ok(())
}
fn metadata(object: &[u8], assembly: &[u8], defined: &[String], undefined: &[String]) -> String {
    canonical_json(&json!({
        "definedNames": defined,
        "undefinedNames": undefined,
        "objectSize": object.len(),
        "objectSha256": digest(object),
        "assemblySize": assembly.len(),
        "assemblySha256": digest(assembly),
    }))
}
fn string_array(value: Option<&Value>) -> Option<Vec<String>> {
    value?
        .as_array()?
        .iter()
        .map(|v| v.as_str().map(str::to_string))
        .collect()
}
fn cache_hit(cached: [&str; 3], output: [&str; 2]) -> Option<(Vec<String>, Vec<String>)> {
    let meta: Value = serde_json::from_slice(&std::fs::read(cached[2]).ok()?).ok()?;
    let defined = string_array(meta.get("definedNames"))?;
    let undefined = string_array(meta.get("undefinedNames"))?;
    let object = std::fs::read(cached[0]).ok()?;
    let assembly = std::fs::read(cached[1]).ok()?;
    let valid = |data: &[u8], size: &str, hash: &str| {
        meta.get(size).and_then(Value::as_u64) == Some(data.len() as u64)
            && meta.get(hash).and_then(Value::as_str) == Some(&digest(data))
    };
    if !valid(&object, "objectSize", "objectSha256")
        || !valid(&assembly, "assemblySize", "assemblySha256")
    {
        return None;
    }
    std::fs::write(output[0], object).ok()?;
    std::fs::write(output[1], assembly).ok()?;
    Some((defined, undefined))
}
fn compiler_target(target: DecompCompilerTarget) -> CompilerTarget {
    match target {
        DecompCompilerTarget::Gs1 => CompilerTarget::Gs1,
        DecompCompilerTarget::Gs2 => CompilerTarget::Gs2,
    }
}

#[allow(clippy::too_many_arguments)]
pub fn compile_source_for_owner(
    owner: u32,
    root: &str,
    object_cache: &str,
    source: &str,
    object_dir: &str,
    compiler: DecompCompilerTarget,
    edition_define: &str,
    signatures: &CacheSignatures,
    allowed_undefined: &[String],
) -> Result<Compiled> {
    let name = format!("{owner:08x}");
    let object = text(Path::new(object_dir).join(format!("{name}.o")));
    let assembly = text(Path::new(object_dir).join(format!("{name}.s")));
    let routing_source = match compiler {
        DecompCompilerTarget::Gs1 => SourceOwner::Main(owner)
            .routing_path()
            .to_string_lossy()
            .into_owned(),
        DecompCompilerTarget::Gs2 => SourceOwner::Main(owner)
            .routing_path_for_game("gs2")
            .to_string_lossy()
            .into_owned(),
    };
    let mut options = SourceToAssemblyPlanOptions::new(
        compiler_target(compiler),
        routing_source,
        source,
        assembly.clone(),
    );
    options.preprocessor_flags = vec![format!("-D{edition_define}=1")];
    options.preprocessed_output = Some(text(Path::new(object_dir).join(format!("{name}.i"))));
    let plan = source_to_assembly_plan(&options)?;
    let commands: Vec<Vec<String>> = plan.steps.iter().map(|step| step.command.clone()).collect();
    let source_inputs =
        compiler_source_tree_signature(Path::new(root), Path::new(source), &commands)?;
    let key = object_cache_key(
        &source_inputs,
        &toolchain_stamp_with_signatures(&commands, signatures),
    );
    let cached = [
        text(Path::new(object_cache).join(format!("{key}.o"))),
        text(Path::new(object_cache).join(format!("{key}.s"))),
        text(Path::new(object_cache).join(format!("{key}.json"))),
    ];
    if let Some((defined_names, undefined_names)) =
        cache_hit([&cached[0], &cached[1], &cached[2]], [&object, &assembly])
    {
        if let Some(name) = undefined_names.iter().find(|name| {
            external_symbol(name, CALL_VIA_BASE).is_none() && !allowed_undefined.contains(name)
        }) {
            return Err(format!("{}: unsupported external {name}", basename(source)));
        }
        return Ok(Compiled {
            object,
            defined_names,
            undefined_names,
        });
    }
    for step in &plan.steps {
        run(root, &step.command)?;
    }
    run(
        root,
        &strings(&[
            "arm-none-eabi-as",
            "-mcpu=arm7tdmi",
            "-mthumb-interwork",
            "-o",
            &object,
            &assembly,
        ]),
    )?;
    let defined = last_fields(&run(
        root,
        &strings(&["arm-none-eabi-nm", "-g", "--defined-only", &object]),
    )?);
    let expected = format!("Func_{name}");
    if !defined.iter().any(|s| s == &expected) || defined.iter().any(|s| !function_name(s)) {
        return Err(format!(
            "{}: expected {expected} and address-named functions, found {}",
            basename(source),
            serde_json::to_string(&defined).unwrap()
        ));
    }
    let undefined = last_fields(&run(root, &strings(&["arm-none-eabi-nm", "-u", &object]))?);
    for name in &undefined {
        if external_symbol(name, CALL_VIA_BASE).is_none() && !allowed_undefined.contains(name) {
            return Err(format!("{}: unsupported external {name}", basename(source)));
        }
    }
    std::fs::create_dir_all(object_cache).map_err(|e| format!("{object_cache}: {e}"))?;
    let object_bytes = std::fs::read(&object).map_err(|e| format!("{object}: {e}"))?;
    let assembly_bytes = std::fs::read(&assembly).map_err(|e| format!("{assembly}: {e}"))?;
    let meta = metadata(&object_bytes, &assembly_bytes, &defined, &undefined);
    write_cache(&[
        (&cached[0], &object_bytes),
        (&cached[1], &assembly_bytes),
        (&cached[2], meta.as_bytes()),
    ])?;
    Ok(Compiled {
        object,
        defined_names: defined,
        undefined_names: undefined,
    })
}

pub fn map_limit<T: Sync, U: Send, F: Fn(&T) -> Result<U> + Sync>(
    items: &[T],
    limit: usize,
    action: F,
) -> Result<Vec<U>> {
    let next = AtomicUsize::new(0);
    let slots = Mutex::new(
        (0..items.len())
            .map(|_| None)
            .collect::<Vec<Option<Result<U>>>>(),
    );
    std::thread::scope(|scope| {
        for _ in 0..limit.min(items.len()).max(1) {
            scope.spawn(|| loop {
                let i = next.fetch_add(1, Ordering::Relaxed);
                if i >= items.len() {
                    break;
                }
                slots.lock().unwrap()[i] = Some(action(&items[i]));
            });
        }
    });
    slots
        .into_inner()
        .unwrap()
        .into_iter()
        .map(|v| v.expect("worker visited item"))
        .collect()
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct BuildSummary {
    pub compiled: usize,
    pub linked: usize,
    pub failures: Vec<String>,
    pub claimed_bytes: String,
    pub image_bytes: usize,
}
impl BuildSummary {
    pub fn summary_line(&self) -> String {
        format!(
            "compiled={} linked={} failures={} claimed_bytes={} image_bytes={}",
            self.compiled,
            self.linked,
            self.failures.len(),
            self.claimed_bytes,
            self.image_bytes
        )
    }
}
fn rooted(root: &str, path: &str) -> PathBuf {
    let path = Path::new(path);
    if path.is_absolute() {
        path.into()
    } else {
        Path::new(root).join(path)
    }
}
fn relative(root: &str, path: &str) -> String {
    let root_path = Path::new(root);
    let path = Path::new(path);
    if let Ok(rest) = path.strip_prefix(root_path) {
        return rest.to_string_lossy().replace('\\', "/");
    }
    let from: Vec<_> = root_path.components().collect();
    let to: Vec<_> = path.components().collect();
    let common = from.iter().zip(&to).take_while(|(a, b)| a == b).count();
    let mut parts = vec![".."; from.len().saturating_sub(common)];
    parts.extend(
        to[common..]
            .iter()
            .map(|part| part.as_os_str().to_str().unwrap_or("")),
    );
    parts.join("/")
}
fn write_file(path: &Path, bytes: &[u8]) -> Result<()> {
    let mut file = std::fs::File::create(path).map_err(|e| format!("{}: {e}", path.display()))?;
    file.write_all(bytes)
        .map_err(|e| format!("{}: {e}", path.display()))?;
    Ok(())
}

fn main_sources(root: &str, source_directory: &str) -> Result<Vec<SourceFile>> {
    if let Some(game) = source_directory
        .strip_prefix("games/")
        .and_then(|path| path.strip_suffix("/src"))
        .filter(|game| !game.contains('/'))
    {
        return SourcePaths::load_for_game(Path::new(root), game)?.main_sources();
    }
    let directory = rooted(root, source_directory);
    let mut sources = Vec::new();
    for entry in std::fs::read_dir(&directory)
        .map_err(|error| format!("{}: {error}", directory.display()))?
    {
        let entry = entry.map_err(|error| error.to_string())?;
        if !entry
            .file_type()
            .map_err(|error| error.to_string())?
            .is_file()
        {
            continue;
        }
        let path = entry.path();
        let Some(owner) = path
            .file_stem()
            .and_then(|stem| stem.to_str())
            .and_then(SourceOwner::from_legacy_stem)
            .filter(|owner| owner.is_main())
        else {
            continue;
        };
        sources.push(SourceFile { owner, path });
    }
    sources.sort_by_key(|source| source.owner);
    Ok(sources)
}

pub fn build(options: &Options, root: &str, cwd: &str) -> Result<BuildSummary> {
    let target: DecompTarget = decomp_target(Some(options.target.as_str()))?;
    if !options.compile_only && target.build_support != BuildSupport::Full {
        return Err(format!("{} is compile-only; run `make {}` until its edition link map and ownership surfaces are reconstructed", target.id, target.id));
    }
    let rom_path = Path::new(cwd).join(&options.rom);
    let rom = (!options.source_only && !options.compile_only)
        .then(|| std::fs::read(&rom_path).map_err(|e| format!("{}: {e}", rom_path.display())))
        .transpose()?;
    if let Some(bytes) = &rom {
        if bytes.len() as u64 != target.rom_size {
            return Err(format!(
                "{} ROM must contain exactly {} bytes",
                target.id, target.rom_size
            ));
        }
    }
    let sources = main_sources(root, target.source_dir)?;
    if sources.is_empty() {
        return Err("no reconstructed sources".into());
    }
    let units = TranslationUnits::load(Path::new(root))?;
    let game = compiler_target(target.compiler).as_str();
    let contracts = sources
        .iter()
        .map(|source| module_contract(Path::new(root), game, source, &units))
        .collect::<Result<Vec<_>>>()?;
    let addresses: Vec<_> = sources
        .iter()
        .map(|source| source.owner.address())
        .collect();
    if addresses.windows(2).any(|w| w[0] == w[1]) {
        return Err("duplicate source address".into());
    }
    let limit = ROM_BASE.saturating_add(target.rom_size as u32);
    if addresses.iter().any(|a| *a < ROM_BASE || *a >= limit) {
        return Err("source address outside ROM".into());
    }
    let output = rooted(root, &options.output);
    let object_dir = output.join("obj");
    std::fs::create_dir_all(&object_dir).map_err(|e| format!("{}: {e}", object_dir.display()))?;
    let export_path = (!options.compile_only).then(|| output.join("main-symbols.s"));
    if let Some(path) = &export_path {
        write_file(path, units.main_symbol_exports().as_bytes())?;
    }
    let cache_dir = text(Path::new(root).join("out/cache/claimed-objects"));
    let signatures = CacheSignatures::production()?;
    let declared_units = units
        .units
        .iter()
        .filter(|unit| unit.game == game && unit.overlay.is_none())
        .collect::<Vec<_>>();
    let unit_ids = declared_units
        .iter()
        .map(|unit| unit.id.as_str())
        .collect::<BTreeSet<_>>();
    let standalone = sources
        .iter()
        .enumerate()
        .filter(|(_, source)| {
            units
                .unit_for_game_owner(game, source.owner)
                .is_none_or(|unit| !unit_ids.contains(unit.id.as_str()))
        })
        .map(|(index, _)| index)
        .collect::<Vec<_>>();
    let ordinary = map_limit(&standalone, options.jobs as usize, |index| {
        let source = &sources[*index];
        compile_source_for_owner(
            source.owner.address(),
            root,
            &cache_dir,
            &text(source.path.clone()),
            &text(object_dir.clone()),
            target.compiler,
            target.edition_define,
            &signatures,
            &[],
        )
    })?;
    let mut compiled = vec![None; sources.len()];
    for (index, module) in standalone.iter().copied().zip(ordinary) {
        compiled[index] = Some(module);
    }
    let mut unit_compiles = Vec::new();
    for unit in declared_units {
        let mixed = !unit.exact();
        let work = if mixed {
            object_dir.join("tu").join(&unit.id)
        } else {
            object_dir.clone()
        };
        std::fs::create_dir_all(&work).map_err(|error| format!("{}: {error}", work.display()))?;
        let base = compile_source_for_owner(
            unit.owners[0].address,
            root,
            &cache_dir,
            &text(Path::new(root).join(&unit.source)),
            &text(work),
            target.compiler,
            target.edition_define,
            &signatures,
            &unit.absolute_symbols.keys().cloned().collect::<Vec<_>>(),
        )?;
        let mut declared = unit
            .symbols()
            .map(|(address, _, _)| Ok((address, unit.source_owner(address)?.legacy_name())))
            .collect::<Result<Vec<_>>>()?;
        declared.sort();
        if base.defined_names != declared.into_iter().map(|item| item.1).collect::<Vec<_>>() {
            return Err(format!(
                "{}: object functions disagree with ordered manifest",
                unit.id
            ));
        }
        for owner in unit
            .owners
            .iter()
            .filter(|owner| owner.state == OwnerState::ExactC)
        {
            let index = sources
                .iter()
                .position(|source| {
                    if mixed {
                        source.owner.address() == owner.address
                    } else {
                        source.path == Path::new(root).join(&unit.source)
                    }
                })
                .ok_or_else(|| format!("{}: exact owner is absent from source paths", unit.id))?;
            if mixed {
                compiled[index] = Some(materialize_unit_owner(
                    root,
                    unit,
                    owner.address,
                    owner.extent,
                    &base.object,
                    &object_dir,
                    compiler_target(target.compiler),
                    rom.as_ref(),
                )?);
            } else {
                compiled[index] = Some(base.clone());
                break;
            }
        }
        let exact = unit.exact_owner_count();
        let retained = unit.owners.len() - exact;
        unit_compiles.push(json!({"id":unit.id,"source":unit.source,"c_compiles":1,"composition":if mixed{"complete-tu-owner-slices"}else{"complete-tu-object"},"exact_owners":exact,"retained_owners":retained}));
    }
    let mut compiled = compiled
        .into_iter()
        .map(|module| module.ok_or_else(|| "source lacks a production module".into()))
        .collect::<Result<Vec<_>>>()?;
    let c_compiles = standalone.len() + unit_compiles.len();
    for ((source, module), contract) in sources.iter().zip(&compiled).zip(&contracts) {
        if !contract.is_empty() {
            let expected = contract.iter().map(|item| &item.0).collect::<BTreeSet<_>>();
            let actual = module.defined_names.iter().collect::<BTreeSet<_>>();
            if actual != expected {
                return Err(format!(
                    "{}: definitions disagree with translation-unit contract",
                    source.owner.id()
                ));
            }
        }
    }
    let objects: Vec<_> = compiled.iter().map(|c| c.object.clone()).collect();
    let definitions: Vec<_> = compiled
        .iter()
        .flat_map(|c| c.defined_names.clone())
        .collect();
    let defined = definitions.iter().cloned().collect::<BTreeSet<_>>();
    if defined.len() != definitions.len() {
        return Err("duplicate function definition across C modules".into());
    }
    let mut undefined_set = BTreeSet::new();
    for name in compiled.iter().flat_map(|c| c.undefined_names.iter()) {
        if !defined.contains(name) {
            undefined_set.insert(name.clone());
        }
    }
    let undefined: Vec<_> = undefined_set.into_iter().collect();
    if options.compile_only {
        let modules: Vec<_> = sources
            .iter()
            .zip(&compiled)
            .map(|(source, module)| {
                json!({
                    "source": relative(root, &text(source.path.clone())),
                    "object": relative(root, &module.object),
                    "symbols": module.defined_names,
                    "undefined_symbols": module.undefined_names,
                })
            })
            .collect();
        let document = json!({
            "format": 1,
            "target": target.id.to_string(),
            "compiler": target.compiler.to_string(),
            "edition_define": target.edition_define,
            "verification": "compile_only",
            "translation_unit_compiles": unit_compiles,
            "modules": modules,
        });
        write_file(
            &output.join("manifest.json"),
            format!("{}\n", canonical_json(&document)).as_bytes(),
        )?;
        return Ok(BuildSummary {
            compiled: c_compiles,
            linked: 0,
            failures: Vec::new(),
            claimed_bytes: "not-linked".into(),
            image_bytes: 0,
        });
    }
    let symbols_source = output.join("externals.s");
    let symbols_object = output.join("externals.o");
    let mut externals = ".syntax unified\n.thumb\n".to_string();
    for name in &undefined {
        externals.push_str(&external_symbol_assembly(name, CALL_VIA_BASE)?);
    }
    write_file(&symbols_source, externals.as_bytes())?;
    run(
        root,
        &strings(&[
            "arm-none-eabi-as",
            "-mcpu=arm7tdmi",
            "-mthumb-interwork",
            "-o",
            &text(symbols_object.clone()),
            &text(symbols_source.clone()),
        ]),
    )?;
    let linker = output.join("claimed.ld");
    let mut script = format!(
        "OUTPUT_ARCH(arm)\nENTRY(Func_{})\nSECTIONS\n{{\n",
        sources[0].owner.address_stem()
    );
    for (source, object) in sources.iter().zip(&objects) {
        let owner = source.owner.address_stem();
        script.push_str(&format!(
            "  .func_{} 0x{} : {{ {}(.text) }}\n",
            owner,
            owner,
            relative(root, object)
        ));
    }
    script.push_str("  /DISCARD/ : { *(.comment) *(.note*) }\n}\n");
    write_file(&linker, script.as_bytes())?;
    let elf = output.join("claimed.elf");
    let binary = output.join("claimed.bin");
    run(
        root,
        &strings(&[
            "arm-none-eabi-ld",
            "-T",
            &text(linker.clone()),
            "-o",
            &text(elf.clone()),
            &text(symbols_object.clone()),
        ]),
    )?;
    run(
        root,
        &strings(&[
            "arm-none-eabi-objcopy",
            "-O",
            "binary",
            &text(elf.clone()),
            &text(binary.clone()),
        ]),
    )?;
    let nm = run(
        root,
        &strings(&["arm-none-eabi-nm", "-S", "--defined-only", &text(elf)]),
    )?;
    let mut symbols = BTreeMap::new();
    for line in nm.lines() {
        let f: Vec<_> = line.split_whitespace().collect();
        if f.len() == 4 && defined.contains(f[3]) {
            if !function_name(f[3]) || !matches!(f[2], "T" | "t") {
                return Err(format!("linked symbol is not a Thumb function: {}", f[3]));
            }
            if let (Some(a), Some(s)) = (address(f[0]), address(f[1])) {
                if symbols.insert(f[3].into(), (a, s as usize)).is_some() {
                    return Err(format!("duplicate linked function {}", f[3]));
                }
            }
        }
    }
    if symbols.len() != defined.len() {
        let missing: Vec<_> = defined
            .iter()
            .filter(|n| !symbols.contains_key(*n))
            .cloned()
            .collect();
        return Err(format!(
            "missing linked functions: {}",
            serde_json::to_string(&missing).unwrap()
        ));
    }
    let image = std::fs::read(&binary).map_err(|e| format!("{}: {e}", binary.display()))?;
    let image_base = symbols
        .values()
        .map(|(address, _)| *address)
        .min()
        .unwrap_or(0);
    let mut manifest = Vec::new();
    let mut functions = Vec::new();
    let mut failures = Vec::new();
    let mut total = 0usize;
    let mut previous_end = 0u32;
    for (i, source) in sources.iter().enumerate() {
        let owner = source.owner.address_stem();
        let source_text = text(source.path.clone());
        let symbol_name = format!("Func_{owner}");
        let (start, _) = symbols
            .get(&symbol_name)
            .copied()
            .ok_or_else(|| format!("missing linked function {symbol_name}"))?;
        let claimed = source.owner.address();
        if start != claimed {
            failures.push(format!(
                "{}: linked at 0x{start:08x}",
                basename(&source_text)
            ));
            continue;
        }
        compiled[i]
            .defined_names
            .sort_by_key(|n| symbols.get(n).map(|(a, _)| *a).unwrap_or(u32::MAX));
        let names = compiled[i].defined_names.clone();
        let end =
            module_end(&names, &symbols).map_err(|e| format!("{}: {e}", basename(&source_text)))?;
        for (name, address, extent) in &contracts[i] {
            if symbols.get(name).copied() != Some((*address, *extent)) {
                return Err(format!(
                    "{}: linked symbol {name} disagrees with translation-unit contract",
                    source.owner.id()
                ));
            }
        }
        let size = end.saturating_sub(start) as usize;
        if start < previous_end {
            failures.push(format!(
                "{}: overlaps previous function",
                basename(&source_text)
            ));
        }
        previous_end = previous_end.max(end);
        if end > limit {
            failures.push(format!(
                "{}: linked extent outside ROM",
                basename(&source_text)
            ));
        }
        let actual = subarray(&image, start - image_base, end - image_base);
        if let Some(rom) = &rom {
            if actual != subarray(rom, start - ROM_BASE, end - ROM_BASE) {
                failures.push(format!("{}: linked bytes differ", basename(&source_text)));
            }
        }
        let production_unit = units.unit_for_game_owner(game, source.owner);
        let production_source = production_unit.map_or_else(
            || source_text.clone(),
            |unit| text(Path::new(root).join(&unit.source)),
        );
        for name in &names {
            let expected = address(name.strip_prefix("Func_").unwrap_or(""));
            if expected != symbols.get(name).map(|(a, _)| *a)
                || symbols
                    .get(name)
                    .map(|(a, _)| *a < start || *a >= end)
                    .unwrap_or(true)
            {
                failures.push(format!(
                    "{}: invalid module symbol {name}",
                    basename(&source_text)
                ));
            }
            let (address, size) = symbols
                .get(name)
                .copied()
                .ok_or_else(|| format!("missing linked function {name}"))?;
            functions.push(linked_function_record(
                root,
                name,
                address,
                size,
                &production_source,
                &symbol_name,
                production_unit,
            )?);
        }
        manifest.push(json!({
            "source": relative(root, &production_source),
            "symbol": symbol_name,
            "symbols": names,
            "address": start,
            "size": size,
            "end": end,
            "translation_unit": production_unit.map(|unit| unit.id.as_str()),
            "composition": production_unit.map(|unit| if unit.exact() { "complete-tu-object" } else { "complete-tu-owner-slice" }),
            "byte_verification": if rom.is_some() { "rom" } else { "source_only" },
        }));
        total += size;
    }
    let document = json!({
        "format": 1,
        "target": target.id.to_string(),
        "compiler": target.compiler.to_string(),
        "rom_base": ROM_BASE,
        "rom_size": target.rom_size,
        "verification": if options.source_only { "source_only" } else { "rom" },
        "image_base": image_base,
        "image_size": image.len(),
        "claimed_bytes": total,
        "translation_unit_manifest": "games/gs1/recon/translation-units.json",
        "main_symbol_exports": export_path
            .as_ref()
            .map(|path| relative(root, &text(path.clone()))),
        "translation_unit_compiles": unit_compiles,
        "functions": functions,
        "regions": manifest,
    });
    write_file(
        &output.join("manifest.json"),
        format!("{}\n", canonical_json(&document)).as_bytes(),
    )?;
    Ok(BuildSummary {
        compiled: c_compiles,
        linked: manifest.len(),
        failures,
        claimed_bytes: total.to_string(),
        image_bytes: image.len(),
    })
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn fundamentals() {
        assert_eq!(self_test().unwrap(), "self-test=ok");
        assert_ne!(object_cache_key(b"yz", "x"), object_cache_key(b"z", "xy"));
        let mut s = BTreeMap::new();
        s.insert("a".into(), (100, 4));
        s.insert("b".into(), (200, 8));
        assert_eq!(module_end(&["a".into(), "b".into()], &s).unwrap(), 208);
        let ParsedArgs::Run(o) =
            parse_args(&["--source-only".into(), "--target=gs2-en".into()]).unwrap()
        else {
            panic!()
        };
        assert_eq!(o.output, "out/gs2-en/claimed");
        let ParsedArgs::Run(options) = parse_args(&[
            "--target=gs1-de".into(),
            "--compile-only".into(),
            "--output=out/gs1-de/compile".into(),
        ])
        .unwrap() else {
            panic!()
        };
        assert!(options.compile_only);
        assert!(!options.source_only);
        assert_eq!(options.output, "out/gs1-de/compile");
        assert!(parse_args(&["--source-only".into(), "--compile-only".into()]).is_err());
        let values: Vec<_> = (0..32).collect();
        assert_eq!(
            map_limit(&values, 4, |v| Ok(v * 2)).unwrap(),
            values.iter().map(|v| v * 2).collect::<Vec<_>>()
        );
        assert!(function_name("Func_0801c0c8"));
        assert!(!function_name("Func_0801C0C8"));
    }
    #[test]
    fn linked_function_manifest_uses_stable_module_evidence() {
        let value = linked_function_record(
            "/repo",
            "Func_08000000",
            0x0800_0000,
            4,
            "/repo/games/a.c",
            "Func_08000000",
            None,
        )
        .unwrap();
        assert_eq!(value["symbol"], "Func_08000000");
        assert_eq!(value["source"], "games/a.c");
        assert_eq!(value["module"], "Func_08000000");
        assert_eq!(value["extent_evidence"], "linked-elf-symbol");
        assert!(linked_function_record(
            "/repo",
            "Func_0800000A",
            0x0800_0000,
            4,
            "x",
            "Func_08000000",
            None
        )
        .is_err());
    }
}
