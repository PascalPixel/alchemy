//! Compile and verify every exact C owner claimed by the main image.
//! Paths and numbers are native Rust types; `serde_json` and `sha2` provide the
//! only serialization and hashing needed by the cache and manifest.

pub mod cli;

use cache_entry::write_cache_entry_atomically;
use canonical_json::canonical_json;
use compiler_core::bundle::{compiler_bundle_signature, host_executable_signature};
use compiler_core::plan::{source_to_assembly_plan, SourceToAssemblyPlanOptions};
use compiler_core::routing::CompilerTarget;
use compiler_core::source_paths::{SourceFile, SourceOwner, SourcePaths};
use compiler_core::symbols::{external_symbol, external_symbol_assembly, CALL_VIA_BASE};
use decomp_targets::{
    decomp_target, parse_decomp_target, target_for, BuildSupport, DecompCompilerTarget,
    DecompTarget, DecompTargetId, DEFAULT_TARGET,
};
use serde_json::{Map, Value};
use sha2::{Digest, Sha256};
use std::{
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
fn stem(path: &str) -> &str {
    Path::new(path)
        .file_stem()
        .and_then(|s| s.to_str())
        .unwrap_or(path)
}
fn text(path: PathBuf) -> String {
    path.to_string_lossy().into_owned()
}
fn root_path() -> PathBuf {
    compiler_core::routing::root().to_path_buf()
}
pub fn root() -> String {
    text(root_path())
}
pub fn object_cache_dir(root: &str) -> String {
    text(Path::new(root).join("out/cache/claimed-objects"))
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
fn fields(line: &str) -> impl Iterator<Item = &str> {
    line.split_whitespace()
}
fn last_fields(output: &str) -> Vec<String> {
    output
        .lines()
        .filter(|l| !l.is_empty())
        .filter_map(|l| fields(l).last())
        .map(str::to_string)
        .collect()
}
fn json_strings(items: &[String]) -> Value {
    Value::Array(items.iter().cloned().map(Value::String).collect())
}
fn json_stringify(items: &[String]) -> String {
    serde_json::to_string(&json_strings(items)).expect("strings serialize")
}
fn number(value: u32) -> Value {
    Value::from(value)
}
fn usize_number(value: usize) -> Value {
    Value::from(value as u64)
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
    pub fn injected(compiler_bundle: &str, binutils: &str, implementation: &str) -> Self {
        Self {
            compiler_bundle: compiler_bundle.into(),
            binutils: binutils.into(),
            implementation: implementation.into(),
        }
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
pub fn toolchain_stamp(commands: &[Vec<String>]) -> Result<String> {
    Ok(toolchain_stamp_with_signatures(
        commands,
        &CacheSignatures::production()?,
    ))
}
pub struct ToolchainStampCache {
    signatures: CacheSignatures,
    entries: Mutex<Vec<(Vec<u8>, String)>>,
}
impl ToolchainStampCache {
    pub fn new() -> Result<Self> {
        Ok(Self::with_signatures(CacheSignatures::production()?))
    }
    pub fn with_signatures(signatures: CacheSignatures) -> Self {
        Self {
            signatures,
            entries: Mutex::new(Vec::new()),
        }
    }
    pub fn stamp(&self, commands: &[Vec<String>]) -> String {
        let key = command_frame(commands);
        let mut entries = self.entries.lock().unwrap();
        if let Some((_, value)) = entries.iter().find(|(old, _)| *old == key) {
            return value.clone();
        }
        let value = toolchain_stamp_with_signatures(commands, &self.signatures);
        entries.push((key, value.clone()));
        value
    }
}

#[derive(Debug, Default, Clone)]
pub struct SymbolTable {
    entries: Vec<(String, (u32, usize))>,
}
impl SymbolTable {
    pub fn new() -> Self {
        Self::default()
    }
    pub fn set(&mut self, name: &str, address: u32, size: usize) {
        if let Some((_, value)) = self.entries.iter_mut().find(|(key, _)| key == name) {
            *value = (address, size);
        } else {
            self.entries.push((name.into(), (address, size)));
        }
    }
    pub fn get(&self, name: &str) -> Option<(u32, usize)> {
        self.entries
            .iter()
            .find(|(key, _)| key == name)
            .map(|(_, v)| *v)
    }
    pub fn has(&self, name: &str) -> bool {
        self.get(name).is_some()
    }
    pub fn len(&self) -> usize {
        self.entries.len()
    }
    pub fn is_empty(&self) -> bool {
        self.entries.is_empty()
    }
    pub fn addresses(&self) -> Vec<u32> {
        self.entries.iter().map(|(_, (a, _))| *a).collect()
    }
}
impl FromIterator<(String, (u32, usize))> for SymbolTable {
    fn from_iter<T: IntoIterator<Item = (String, (u32, usize))>>(items: T) -> Self {
        let mut out = Self::new();
        for (n, (a, s)) in items {
            out.set(&n, a, s);
        }
        out
    }
}
#[derive(Debug, Default, Clone)]
pub struct OrderedStringSet(Vec<String>);
impl OrderedStringSet {
    pub fn new() -> Self {
        Self::default()
    }
    pub fn add(&mut self, value: &str) {
        if !self.has(value) {
            self.0.push(value.into());
        }
    }
    pub fn has(&self, value: &str) -> bool {
        self.0.iter().any(|item| item == value)
    }
    pub fn len(&self) -> usize {
        self.0.len()
    }
    pub fn is_empty(&self) -> bool {
        self.0.is_empty()
    }
    pub fn iter(&self) -> std::slice::Iter<'_, String> {
        self.0.iter()
    }
}
pub fn module_end(names: &[String], symbols: &SymbolTable) -> Result<u32> {
    if names.is_empty() {
        return Err("C module has no functions".into());
    }
    names
        .iter()
        .map(|name| {
            symbols
                .get(name)
                .filter(|(_, size)| *size > 0)
                .map(|(a, s)| a.saturating_add(s as u32))
                .ok_or_else(|| format!("invalid module symbol {name}"))
        })
        .max()
        .transpose()?
        .ok_or_else(|| "C module has no functions".into())
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

#[derive(Debug, Clone)]
pub struct RunResult {
    pub stdout: String,
    pub stderr: String,
}
pub fn run(root: &str, command: &[String]) -> Result<RunResult> {
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
    Ok(RunResult { stdout, stderr })
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Compiled {
    pub object: String,
    pub defined_names: Vec<String>,
    pub undefined_names: Vec<String>,
}
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum CacheOutcome {
    Hit,
    Miss,
}
fn write_cache(items: &[(&str, &[u8])]) -> Result<()> {
    for (path, bytes) in items {
        write_cache_entry_atomically(Path::new(path), bytes).map_err(|e| format!("{path}: {e}"))?;
    }
    Ok(())
}
fn metadata(object: &[u8], assembly: &[u8], defined: &[String], undefined: &[String]) -> String {
    let mut map = Map::new();
    map.insert("definedNames".into(), json_strings(defined));
    map.insert("undefinedNames".into(), json_strings(undefined));
    map.insert("objectSize".into(), usize_number(object.len()));
    map.insert("objectSha256".into(), Value::String(digest(object)));
    map.insert("assemblySize".into(), usize_number(assembly.len()));
    map.insert("assemblySha256".into(), Value::String(digest(assembly)));
    canonical_json(&Value::Object(map))
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

pub fn compile_source(
    root: &str,
    object_cache: &str,
    source: &str,
    object_dir: &str,
    compiler: DecompCompilerTarget,
    edition_define: &str,
    stamps: &ToolchainStampCache,
) -> Result<(Compiled, CacheOutcome)> {
    let owner = address(stem(source)).ok_or_else(|| {
        format!(
            "{}: legacy source filename is not an address",
            basename(source)
        )
    })?;
    compile_source_for_owner(
        owner,
        root,
        object_cache,
        source,
        object_dir,
        compiler,
        edition_define,
        stamps,
    )
}

pub fn compile_source_for_owner(
    owner: u32,
    root: &str,
    object_cache: &str,
    source: &str,
    object_dir: &str,
    compiler: DecompCompilerTarget,
    edition_define: &str,
    stamps: &ToolchainStampCache,
) -> Result<(Compiled, CacheOutcome)> {
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
    let source_bytes = std::fs::read(source).map_err(|e| format!("{source}: {e}"))?;
    let commands: Vec<Vec<String>> = plan.steps.iter().map(|step| step.command.clone()).collect();
    let key = object_cache_key(&source_bytes, &stamps.stamp(&commands));
    let cached = [
        text(Path::new(object_cache).join(format!("{key}.o"))),
        text(Path::new(object_cache).join(format!("{key}.s"))),
        text(Path::new(object_cache).join(format!("{key}.json"))),
    ];
    if let Some((defined_names, undefined_names)) =
        cache_hit([&cached[0], &cached[1], &cached[2]], [&object, &assembly])
    {
        return Ok((
            Compiled {
                object,
                defined_names,
                undefined_names,
            },
            CacheOutcome::Hit,
        ));
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
    let defined = last_fields(
        &run(
            root,
            &strings(&["arm-none-eabi-nm", "-g", "--defined-only", &object]),
        )?
        .stdout,
    );
    let expected = format!("Func_{name}");
    if !defined.iter().any(|s| s == &expected) || defined.iter().any(|s| !function_name(s)) {
        return Err(format!(
            "{}: expected {expected} and address-named functions, found {}",
            basename(source),
            json_stringify(&defined)
        ));
    }
    let undefined = last_fields(&run(root, &strings(&["arm-none-eabi-nm", "-u", &object]))?.stdout);
    for name in &undefined {
        if external_symbol(name, CALL_VIA_BASE).is_none() {
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
    Ok((
        Compiled {
            object,
            defined_names: defined,
            undefined_names: undefined,
        },
        CacheOutcome::Miss,
    ))
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
    pub written: Vec<String>,
    pub cache_hits: usize,
    pub cache_misses: usize,
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
fn write_file(path: &Path, bytes: &[u8], written: &mut Vec<String>) -> Result<()> {
    let mut file = std::fs::File::create(path).map_err(|e| format!("{}: {e}", path.display()))?;
    file.write_all(bytes)
        .map_err(|e| format!("{}: {e}", path.display()))?;
    written.push(text(path.to_path_buf()));
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
        return Err(format!(
            "{} is compile-only; run `make {}` until its edition link map and ownership surfaces are reconstructed",
            target.id, target.id
        ));
    }
    let mut written = Vec::new();
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
    let cache_dir = object_cache_dir(root);
    let stamps = ToolchainStampCache::new()?;
    let pairs = map_limit(&sources, options.jobs as usize, |source| {
        compile_source_for_owner(
            source.owner.address(),
            root,
            &cache_dir,
            &text(source.path.clone()),
            &text(object_dir.clone()),
            target.compiler,
            target.edition_define,
            &stamps,
        )
    })?;
    let cache_hits = pairs
        .iter()
        .filter(|(_, hit)| *hit == CacheOutcome::Hit)
        .count();
    let cache_misses = pairs.len() - cache_hits;
    let mut compiled: Vec<_> = pairs.into_iter().map(|(c, _)| c).collect();
    let objects: Vec<_> = compiled.iter().map(|c| c.object.clone()).collect();
    let definitions: Vec<_> = compiled
        .iter()
        .flat_map(|c| c.defined_names.clone())
        .collect();
    let mut defined = OrderedStringSet::new();
    for name in &definitions {
        defined.add(name);
    }
    if defined.len() != definitions.len() {
        return Err("duplicate function definition across C modules".into());
    }
    let mut undefined_set = OrderedStringSet::new();
    for name in compiled.iter().flat_map(|c| c.undefined_names.iter()) {
        if !defined.has(name) {
            undefined_set.add(name);
        }
    }
    let mut undefined: Vec<_> = undefined_set.iter().cloned().collect();
    undefined.sort();
    if options.compile_only {
        let modules = sources
            .iter()
            .zip(&compiled)
            .map(|(source, module)| {
                let mut row = Map::new();
                row.insert(
                    "source".into(),
                    Value::String(relative(root, &text(source.path.clone()))),
                );
                row.insert(
                    "object".into(),
                    Value::String(relative(root, &module.object)),
                );
                row.insert("symbols".into(), json_strings(&module.defined_names));
                row.insert(
                    "undefined_symbols".into(),
                    json_strings(&module.undefined_names),
                );
                Value::Object(row)
            })
            .collect();
        let mut document = Map::new();
        document.insert("format".into(), number(1));
        document.insert("target".into(), Value::String(target.id.to_string()));
        document.insert(
            "compiler".into(),
            Value::String(target.compiler.to_string()),
        );
        document.insert(
            "edition_define".into(),
            Value::String(target.edition_define.into()),
        );
        document.insert("verification".into(), Value::String("compile_only".into()));
        document.insert("modules".into(), Value::Array(modules));
        write_file(
            &output.join("manifest.json"),
            format!("{}\n", canonical_json(&Value::Object(document))).as_bytes(),
            &mut written,
        )?;
        return Ok(BuildSummary {
            compiled: compiled.len(),
            linked: 0,
            failures: Vec::new(),
            claimed_bytes: "not-linked".into(),
            image_bytes: 0,
            written,
            cache_hits,
            cache_misses,
        });
    }
    let symbols_source = output.join("externals.s");
    let symbols_object = output.join("externals.o");
    let mut externals = ".syntax unified\n.thumb\n".to_string();
    for name in &undefined {
        externals.push_str(&external_symbol_assembly(name, CALL_VIA_BASE)?);
    }
    write_file(&symbols_source, externals.as_bytes(), &mut written)?;
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
    written.push(text(symbols_object.clone()));
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
    write_file(&linker, script.as_bytes(), &mut written)?;
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
    written.push(text(elf.clone()));
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
    written.push(text(binary.clone()));
    let nm = run(
        root,
        &strings(&["arm-none-eabi-nm", "-S", "--defined-only", &text(elf)]),
    )?;
    let mut symbols = SymbolTable::new();
    for line in nm.stdout.lines() {
        let f: Vec<_> = fields(line).collect();
        if f.len() == 4 && defined.has(f[3]) {
            if let (Some(a), Some(s)) = (address(f[0]), address(f[1])) {
                symbols.set(f[3], a, s as usize);
            }
        }
    }
    if symbols.len() != defined.len() {
        let mut missing: Vec<_> = defined
            .iter()
            .filter(|n| !symbols.has(n))
            .cloned()
            .collect();
        missing.sort();
        return Err(format!(
            "missing linked functions: {}",
            json_stringify(&missing)
        ));
    }
    let image = std::fs::read(&binary).map_err(|e| format!("{}: {e}", binary.display()))?;
    let image_base = symbols.addresses().into_iter().min().unwrap_or(0);
    let mut manifest = Vec::new();
    let mut failures = Vec::new();
    let mut total = 0usize;
    let mut previous_end = 0u32;
    for (i, source) in sources.iter().enumerate() {
        let owner = source.owner.address_stem();
        let source_text = text(source.path.clone());
        let symbol_name = format!("Func_{owner}");
        let (start, _) = symbols
            .get(&symbol_name)
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
            .sort_by_key(|n| symbols.get(n).map(|(a, _)| a).unwrap_or(u32::MAX));
        let names = compiled[i].defined_names.clone();
        let end =
            module_end(&names, &symbols).map_err(|e| format!("{}: {e}", basename(&source_text)))?;
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
        for name in &names {
            let expected = address(name.strip_prefix("Func_").unwrap_or(""));
            if expected != symbols.get(name).map(|(a, _)| a)
                || symbols
                    .get(name)
                    .map(|(a, _)| a < start || a >= end)
                    .unwrap_or(true)
            {
                failures.push(format!(
                    "{}: invalid module symbol {name}",
                    basename(&source_text)
                ));
            }
        }
        let mut region = Map::new();
        region.insert("source".into(), Value::String(relative(root, &source_text)));
        region.insert("symbol".into(), Value::String(symbol_name));
        region.insert("symbols".into(), json_strings(&names));
        region.insert("address".into(), number(start));
        region.insert("size".into(), usize_number(size));
        region.insert("end".into(), number(end));
        manifest.push(Value::Object(region));
        total += size;
    }
    let mut document = Map::new();
    document.insert("format".into(), number(1));
    document.insert("target".into(), Value::String(target.id.to_string()));
    document.insert(
        "compiler".into(),
        Value::String(target.compiler.to_string()),
    );
    document.insert("rom_base".into(), number(ROM_BASE));
    document.insert("rom_size".into(), Value::from(target.rom_size));
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
    document.insert("image_base".into(), number(image_base));
    document.insert("image_size".into(), usize_number(image.len()));
    document.insert("claimed_bytes".into(), usize_number(total));
    document.insert("regions".into(), Value::Array(manifest.clone()));
    write_file(
        &output.join("manifest.json"),
        format!("{}\n", canonical_json(&Value::Object(document))).as_bytes(),
        &mut written,
    )?;
    Ok(BuildSummary {
        compiled: compiled.len(),
        linked: manifest.len(),
        failures,
        claimed_bytes: total.to_string(),
        image_bytes: image.len(),
        written,
        cache_hits,
        cache_misses,
    })
}

pub fn exists(path: &str) -> bool {
    Path::new(path).exists()
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn self_test_ok() {
        assert_eq!(self_test().unwrap(), "self-test=ok");
    }
    #[test]
    fn cache_key_separates_inputs() {
        assert_ne!(object_cache_key(b"yz", "x"), object_cache_key(b"z", "xy"));
    }
    #[test]
    fn module_range() {
        let mut s = SymbolTable::new();
        s.set("a", 100, 4);
        s.set("b", 200, 8);
        assert_eq!(module_end(&["a".into(), "b".into()], &s).unwrap(), 208);
    }
    #[test]
    fn args_resolve_target_before_default_output() {
        let ParsedArgs::Run(o) =
            parse_args(&["--source-only".into(), "--target=gs2-en".into()]).unwrap()
        else {
            panic!()
        };
        assert_eq!(o.output, "out/gs2-en/claimed");
    }
    #[test]
    fn compile_only_is_an_explicit_non_linking_mode() {
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
    }
    #[test]
    fn map_is_ordered() {
        let values: Vec<_> = (0..32).collect();
        assert_eq!(
            map_limit(&values, 4, |v| Ok(v * 2)).unwrap(),
            values.iter().map(|v| v * 2).collect::<Vec<_>>()
        );
    }
    #[test]
    fn function_names_are_strict() {
        assert!(function_name("Func_0801c0c8"));
        assert!(!function_name("Func_0801C0C8"));
    }
}
