//! Port of `tools/make/build_claimed.ts`.
//!
//! WHAT THE TOOL DOES. Every file in the target's source tree named
//! `AAAAAAAA.c` claims the ROM address `0xAAAAAAAA`. This tool compiles each
//! one, links them all at their claimed addresses through a generated linker
//! script, and then checks the linked bytes against the real ROM. It emits an
//! object cache, an `externals.s`/`externals.o` pair for every undefined
//! symbol, a linker script, an ELF, a flat binary and a manifest.
//!
//! WHAT MOVED AND WHAT DID NOT. The compiler-planning layer, the external
//! symbol shaping and the target registry are NOT re-implemented here: they
//! already exist as proved ports in `alchemy-plan`, `alchemy-symbols` and
//! `decomp-targets`, and this crate calls them. That is the whole reason the
//! port is possible without a TypeScript compiler-module sidecar.
//!
//! NO BUN BRIDGE. Because those three layers are native Rust, this port never
//! shells out to `bun`, so it does not re-pay bun's cold import per call. It
//! does shell out to the ARM binutils and to the compiler driver, exactly as
//! the original does; those are the same processes on both sides and the
//! benchmark separates them from the work this crate actually owns.
//!
//! STANDING HAZARDS.
//!
//! ORDER IS OUTPUT. The linker script's section order, the manifest's region
//! order and the `externals.s` symbol order all come from list order. There is
//! no `HashMap` and no `HashSet` in this crate. A `Set` in the original is
//! insertion-ordered and its iteration order reaches `externals.s`; a hash
//! container would not fail a test, it would emit a different image.
//!
//! NUMBERS ARE DOUBLES. Symbol addresses and sizes come from
//! `Number.parseInt(field, 16)`, which yields `NaN` on a malformed `nm` line
//! rather than failing. That `NaN` then flows through `<= 0` (false), `Math.max`
//! (propagates) and `subarray` (becomes 0). Modelling those as integers would
//! turn a silently-wrong build into a different silently-wrong build, so every
//! address in this crate is an `f64` and every operation on one goes through a
//! named helper in [`js`].

pub mod js;
pub mod nodepath;
pub mod sha256;

use std::io::Write as _;
use std::path::Path;
use std::process::Command;
use std::sync::atomic::{AtomicUsize, Ordering as AtomicOrdering};
use std::sync::Mutex;

use alchemy_bundle::bundle::{compiler_bundle_signature, host_executable_signature};
use alchemy_plan::plan::{source_to_assembly_plan, SourceToAssemblyPlanOptions};
use alchemy_routing::routing::CompilerTarget;
use alchemy_symbols::symbols::{external_symbol, external_symbol_assembly, CALL_VIA_BASE};
use cache_entry::write_cache_entry_atomically;
use canonical_json::canonical_json;
use decomp_targets::{
    decomp_target, parse_decomp_target, target_for, DecompCompilerTarget, DecompTarget,
    DecompTargetId, DEFAULT_TARGET,
};
use serde_json::{Map, Value};

use js::{
    is_lowercase_func_symbol, is_main_image_source_name, js_default_sort, js_hex_pad8,
    js_is_integer, js_max, js_min, js_number, js_numeric_compare, js_or_string, js_parse_int,
    js_split_lines, js_subarray, js_trim, js_trim_split_whitespace,
};
use nodepath::{basename, basename_with_ext, extname, is_absolute, join, relative, resolve};

/// `const ROM_BASE = 0x08000000;`
pub const ROM_BASE: f64 = 0x0800_0000u32 as f64;

/// Every error in this crate is the text the TypeScript would have thrown.
pub type Result<T> = std::result::Result<T, String>;

// ---------------------------------------------------------------------------
// Repository root
// ---------------------------------------------------------------------------

/// `const ROOT = dirname(dirname(dirname(...import.meta.url)))`.
///
/// The TypeScript walks three directories up from `tools/make/build_claimed.ts`,
/// which is the repository root. `alchemy-routing::root()` already resolves the
/// same root for the Rust side and is reused so the two cannot drift.
///
/// PORT NOTE -- the `b3ab4841b` path-break check. That commit moved tools into
/// subfolders and left stale literal `join(ROOT, "tools", ...)` segments in some
/// of them. `build_claimed.ts` contains no such literal: the only `ROOT`-derived
/// paths it builds are `out/cache/claimed-objects` and the target's own relative
/// directories, and a real (non-`--self-test`) run of the TypeScript completes
/// with `linked=1456 failures=0`. This tool is NOT a victim, so nothing is
/// corrected here.
pub fn root() -> String {
    alchemy_routing::routing::root()
        .to_string_lossy()
        .into_owned()
}

/// `const OBJECT_CACHE = join(ROOT, "out/cache/claimed-objects")`.
pub fn object_cache_dir(root: &str) -> String {
    join(root, "out/cache/claimed-objects")
}

// ---------------------------------------------------------------------------
// stem
// ---------------------------------------------------------------------------

/// `stem(path)` -- `basename(path, extname(path))`.
pub fn stem(path: &str) -> &str {
    basename_with_ext(path, extname(path))
}

// ---------------------------------------------------------------------------
// Object cache key
// ---------------------------------------------------------------------------

/// `objectCacheKey(sourceBytes, planDescription)`.
///
/// THE KEY DIGESTS INPUTS, NOT A VERSION LITERAL. There is no `-vN` string
/// anywhere in it: the plan description names the compiler binaries and every
/// routed flag, and the toolchain stamp folds in the complete compiler-bundle
/// signature plus the trusted assembler/nm signature, so a changed compiler, a
/// changed flag route or a changed source all produce a different key. A
/// hand-bumped literal is what poisoned
/// `out/cache/overlay-c` across checkouts; this cache cannot acquire that
/// failure mode.
///
/// The NUL between the two chunks is a real separator: without it a plan
/// description ending in `x` over source `yz` would key the same as `xy` over
/// `z`.
pub fn object_cache_key(source_bytes: &[u8], plan_description: &str) -> String {
    let mut digest = sha256::Sha256::new();
    digest.update(plan_description.as_bytes());
    digest.update(b"\0");
    digest.update(source_bytes);
    digest.digest_hex()
}

/// The host tools used while compiling a claimed object and inspecting it.
const CLAIMED_OBJECT_BINUTILS: [&str; 2] = ["arm-none-eabi-as", "arm-none-eabi-nm"];

/// Inputs which are deliberately fixed for one cache namespace.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct CacheSignatures {
    compiler_bundle: String,
    binutils: String,
    implementation: String,
}

impl CacheSignatures {
    /// Resolve and sign every production input trusted by the object cache.
    pub fn production() -> Result<Self> {
        Ok(Self {
            compiler_bundle: compiler_bundle_signature(),
            binutils: host_executable_signature(&CLAIMED_OBJECT_BINUTILS)?,
            implementation: current_executable_signature()?,
        })
    }

    /// Supply deterministic signatures for tests without requiring a toolchain.
    pub fn injected(compiler_bundle: &str, binutils: &str, implementation: &str) -> Self {
        Self {
            compiler_bundle: compiler_bundle.to_string(),
            binutils: binutils.to_string(),
            implementation: implementation.to_string(),
        }
    }
}

fn current_executable_signature() -> Result<String> {
    let path = std::env::current_exe().map_err(|error| format!("current executable: {error}"))?;
    let bytes = std::fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))?;
    if bytes.is_empty() {
        return Err(format!("current executable is empty: {}", path.display()));
    }
    Ok(sha256::sha256_hex(&bytes))
}

fn append_frame(stream: &mut Vec<u8>, bytes: &[u8]) {
    stream.extend_from_slice(&(bytes.len() as u64).to_be_bytes());
    stream.extend_from_slice(bytes);
}

fn framed_commands(commands: &[Vec<String>]) -> Vec<u8> {
    let mut stream = Vec::new();
    append_frame(&mut stream, &(commands.len() as u64).to_be_bytes());
    for command in commands {
        append_frame(&mut stream, &(command.len() as u64).to_be_bytes());
        for argument in command {
            append_frame(&mut stream, argument.as_bytes());
        }
    }
    stream
}

/// Build the cache stamp from injective argv framing and the complete trusted
/// toolchain signatures. Adding this material intentionally migrates every old
/// claimed-object key once; old entries remain on disk but are unreachable.
pub fn toolchain_stamp_with_signatures(
    commands: &[Vec<String>],
    signatures: &CacheSignatures,
) -> String {
    let mut stream = Vec::new();
    append_frame(
        &mut stream,
        b"build-claimed cache identity: framed argv and signed toolchain",
    );
    append_frame(&mut stream, &framed_commands(commands));
    append_frame(&mut stream, signatures.compiler_bundle.as_bytes());
    append_frame(&mut stream, signatures.binutils.as_bytes());
    append_frame(&mut stream, signatures.implementation.as_bytes());
    sha256::sha256_hex(&stream)
}

/// Production form of [`toolchain_stamp_with_signatures`].
pub fn toolchain_stamp(commands: &[Vec<String>]) -> Result<String> {
    let signatures = CacheSignatures::production()?;
    Ok(toolchain_stamp_with_signatures(commands, &signatures))
}

/// `cachedToolchainStamp`, the in-process memo in front of [`toolchain_stamp`].
///
/// This is the ORIGINAL's memo, not an optimisation invented to win a
/// benchmark: without it every one of ~1,456 units re-hashes the same multi
/// megabyte compiler binaries. An insertion-ordered `Vec` stands in for the
/// `Map`; the iteration order is unobservable here, but the rule in this crate
/// is that no unordered container exists at all.
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
        let identity = framed_commands(commands);
        if let Some(found) = self
            .entries
            .lock()
            .expect("stamp cache is not poisoned")
            .iter()
            .find(|(key, _)| *key == identity)
        {
            return found.1.clone();
        }
        let stamp = toolchain_stamp_with_signatures(commands, &self.signatures);
        self.entries
            .lock()
            .expect("stamp cache is not poisoned")
            .push((identity, stamp.clone()));
        stamp
    }
}

// ---------------------------------------------------------------------------
// Symbol table
// ---------------------------------------------------------------------------

/// `Map<string, [number, number]>` -- name to `[address, size]`.
///
/// Insertion-ordered, because `symbols.size !== defined.size` reporting and the
/// missing-symbol diagnostic both read it back.
#[derive(Debug, Default, Clone)]
pub struct SymbolTable {
    entries: Vec<(String, (f64, f64))>,
}

impl SymbolTable {
    pub fn new() -> Self {
        Self::default()
    }

    /// `Map#set`: an existing key keeps its position and takes the new value.
    pub fn set(&mut self, name: &str, address: f64, size: f64) {
        if let Some(slot) = self.entries.iter_mut().find(|(key, _)| key == name) {
            slot.1 = (address, size);
            return;
        }
        self.entries.push((name.to_string(), (address, size)));
    }

    pub fn get(&self, name: &str) -> Option<(f64, f64)> {
        self.entries
            .iter()
            .find(|(key, _)| key == name)
            .map(|(_, value)| *value)
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

    pub fn addresses(&self) -> Vec<f64> {
        self.entries
            .iter()
            .map(|(_, (address, _))| *address)
            .collect()
    }
}

impl FromIterator<(String, (f64, f64))> for SymbolTable {
    fn from_iter<T: IntoIterator<Item = (String, (f64, f64))>>(iter: T) -> Self {
        let mut table = SymbolTable::new();
        for (name, (address, size)) in iter {
            table.set(&name, address, size);
        }
        table
    }
}

/// An insertion-ordered `Set<string>`.
///
/// `Set` is SameValueZero, which for strings is plain equality, so the only
/// thing that has to be preserved is the ORDER and the silent dedupe. Both are
/// load-bearing: the dedupe drives the `duplicate function definition` check,
/// and the order reaches `externals.s`.
#[derive(Debug, Default, Clone)]
pub struct OrderedStringSet {
    items: Vec<String>,
}

impl OrderedStringSet {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn add(&mut self, value: &str) {
        if !self.has(value) {
            self.items.push(value.to_string());
        }
    }

    pub fn has(&self, value: &str) -> bool {
        self.items.iter().any(|item| item == value)
    }

    pub fn len(&self) -> usize {
        self.items.len()
    }

    pub fn is_empty(&self) -> bool {
        self.items.is_empty()
    }

    pub fn iter(&self) -> std::slice::Iter<'_, String> {
        self.items.iter()
    }
}

// ---------------------------------------------------------------------------
// moduleEnd
// ---------------------------------------------------------------------------

/// `moduleEnd(names, symbols)`.
///
/// PORT NOTE -- BUG, REPRODUCED. `symbol[1] <= 0` is FALSE when the size is
/// `NaN`, so a symbol whose `nm` size field failed to parse passes the guard and
/// contributes `NaN` to the maximum. `Math.max` then propagates it, so the whole
/// module end becomes `NaN`. `f64::max` would swallow it and return a plausible
/// address instead, which is why [`js::js_max`] exists.
pub fn module_end(names: &[String], symbols: &SymbolTable) -> Result<f64> {
    if names.is_empty() {
        return Err("C module has no functions".to_string());
    }
    let mut ends = Vec::with_capacity(names.len());
    for name in names {
        let symbol = symbols.get(name);
        match symbol {
            None => return Err(format!("invalid module symbol {name}")),
            Some((address, size)) => {
                // `size <= 0` -- false for NaN, exactly as in JavaScript.
                if size <= 0.0 {
                    return Err(format!("invalid module symbol {name}"));
                }
                ends.push(address + size);
            }
        }
    }
    Ok(js_max(&ends))
}

// ---------------------------------------------------------------------------
// selfTest
// ---------------------------------------------------------------------------

/// `selfTest()`.
///
/// Returns the line the original prints instead of printing it, so a caller can
/// assert on it. The four failure conditions are checked in the same order.
pub fn self_test() -> Result<String> {
    let mut symbols = SymbolTable::new();
    for name in [
        "Func_0801c0c8",
        "Func_0801c0cc",
        "Func_0801c0d0",
        "Func_0801c0d4",
        "Func_0801c0d8",
    ] {
        let address = js_parse_int(&name[5..], 16).expect("literal hex");
        symbols.set(name, address, 2.0);
    }
    let names: Vec<String> = symbols
        .entries
        .iter()
        .map(|(name, _)| name.clone())
        .collect();
    if module_end(&names, &symbols)? != 0x0801_c0dau32 as f64 {
        return Err("multi-function C module range self-test failed".to_string());
    }
    let source = b"void Func_08000000(void) {}\n";
    let changed = b"void Func_08000000(void) { }\n";
    let base = object_cache_key(source, "plan-a");
    if base != object_cache_key(source, "plan-a") {
        return Err("object cache key is not deterministic".to_string());
    }
    if base == object_cache_key(changed, "plan-a") {
        return Err("object cache key ignores source bytes".to_string());
    }
    if base == object_cache_key(source, "plan-b") {
        return Err("object cache key ignores the command plan".to_string());
    }
    Ok("self-test=ok".to_string())
}

// ---------------------------------------------------------------------------
// Argument parsing
// ---------------------------------------------------------------------------

#[derive(Debug, Clone, PartialEq)]
pub struct Options {
    pub target: DecompTargetId,
    pub rom: String,
    /// `jobs` is a `number` in the original and reaches `Number.isInteger`
    /// BEFORE anything uses it, so a fractional or `NaN` value has to survive
    /// long enough to be rejected by that check with the original's message.
    pub jobs: f64,
    pub output: String,
    pub source_only: bool,
}

#[derive(Debug, Clone, PartialEq)]
pub enum ParsedArgs {
    /// `-h`/`--help`: usage was printed and the process exits 0.
    Help,
    Run(Box<Options>),
}

/// `usage()`.
pub fn usage_text() -> &'static str {
    "usage: build-claimed [-h] [--target gs1-en|gs2-en] [--source-only] \
[--jobs JOBS] [--output OUTPUT] [rom]"
}

/// The default job count: `Math.min(16, navigator.hardwareConcurrency || 1)`.
///
/// PORT NOTE -- this is NOT `resolveJobs()`. The original hard-codes a cap of 16
/// against the full core count, and the cache key, the emitted artifacts and the
/// failure ordering all have to match the TypeScript, so the default is
/// transcribed rather than replaced. Callers that want the repository's 80%
/// parallelism rule pass `--jobs` explicitly; the parity harness and the
/// benchmark in this crate both do.
pub fn default_jobs() -> f64 {
    let concurrency = std::thread::available_parallelism()
        .map(std::num::NonZeroUsize::get)
        .unwrap_or(0);
    // `navigator.hardwareConcurrency || 1` -- 0 is falsy.
    let concurrency = if concurrency == 0 { 1 } else { concurrency };
    16f64.min(concurrency as f64)
}

/// `parseArgs(argv)`.
pub fn parse_args(argv: &[String]) -> Result<ParsedArgs> {
    // First pass: `--target` only. The original scans the whole argv for it
    // before building the defaults, so `--output` defaulting sees the resolved
    // target even when `--target` comes last.
    let mut target_id: DecompTargetId = DEFAULT_TARGET;
    let mut index = 0usize;
    while index < argv.len() {
        let argument = &argv[index];
        if argument == "--target" {
            index += 1;
            let Some(value) = argv.get(index) else {
                return Err("--target requires a value".to_string());
            };
            target_id = parse_decomp_target(value)?;
        } else if let Some(value) = argument.strip_prefix("--target=") {
            target_id = parse_decomp_target(value)?;
        }
        index += 1;
    }
    let target = target_for(target_id);
    let mut options = Options {
        target: target_id,
        rom: target.rom.to_string(),
        jobs: default_jobs(),
        output: join(target.output_dir, "claimed"),
        source_only: false,
    };
    let mut positional = false;
    let mut index = 0usize;
    while index < argv.len() {
        let argument = &argv[index];
        if argument == "-h" || argument == "--help" {
            return Ok(ParsedArgs::Help);
        } else if argument == "--source-only" {
            options.source_only = true;
        } else if argument == "--target" {
            index += 1;
        } else if argument.starts_with("--target=") {
            index += 1;
            continue;
        } else if argument == "--jobs" || argument == "--output" {
            let flag = argument.clone();
            index += 1;
            let Some(value) = argv.get(index) else {
                return Err(format!("{flag} requires a value"));
            };
            if flag == "--jobs" {
                // THE ONLY INTEGER PARSE PATH. `parseInt` here accepts `"3.9"`
                // as 3 and `"12abc"` as 12; `from_str_radix` would reject both,
                // which would be a different tool.
                options.jobs = js_parse_int(value, 10).unwrap_or(f64::NAN);
            } else {
                options.output = value.clone();
            }
        } else if let Some(value) = argument.strip_prefix("--jobs=") {
            options.jobs = js_parse_int(value, 10).unwrap_or(f64::NAN);
        } else if let Some(value) = argument.strip_prefix("--output=") {
            options.output = value.to_string();
        } else if !argument.starts_with('-') && !positional {
            options.rom = argument.clone();
            positional = true;
        } else {
            return Err(format!("unrecognized argument: {argument}"));
        }
        index += 1;
    }
    if !js_is_integer(options.jobs) || options.jobs < 1.0 {
        return Err("jobs must be positive".to_string());
    }
    if options.source_only && positional {
        return Err("--source-only does not accept a ROM".to_string());
    }
    Ok(ParsedArgs::Run(Box::new(options)))
}

// ---------------------------------------------------------------------------
// Process execution
// ---------------------------------------------------------------------------

#[derive(Debug, Clone)]
pub struct RunResult {
    pub stdout: String,
    pub stderr: String,
}

/// `run(command)`.
///
/// PORT NOTE -- the failure message is `${basename(command[0])} failed:
/// ${detail}` where `detail` is `(stderr || stdout).trim()`. The `||` is a
/// string falsiness test, so an EMPTY stderr falls through to stdout; that is
/// [`js::js_or_string`], not `unwrap_or`. The output is decoded the way
/// `new Response(stream).text()` does, which is lossy UTF-8 and matches
/// `String::from_utf8_lossy`.
///
/// A spawn failure (the binary is missing) is not an exit code in either
/// language: Bun throws before there is any output. The message differs in
/// prose, which is why the harness asserts exit code and offending path rather
/// than text.
pub fn run(root: &str, command: &[String]) -> Result<RunResult> {
    let Some(program) = command.first() else {
        return Err("run() requires a command".to_string());
    };
    let output = Command::new(program)
        .args(&command[1..])
        .current_dir(root)
        .output()
        .map_err(|error| format!("{}: {error}", basename(program)))?;
    let stdout = String::from_utf8_lossy(&output.stdout).into_owned();
    let stderr = String::from_utf8_lossy(&output.stderr).into_owned();
    if !output.status.success() {
        let detail = js_trim(js_or_string(&stderr, &stdout));
        return Err(format!("{} failed: {detail}", basename(program)));
    }
    Ok(RunResult { stdout, stderr })
}

// ---------------------------------------------------------------------------
// compileSource
// ---------------------------------------------------------------------------

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Compiled {
    pub object: String,
    pub defined_names: Vec<String>,
    pub undefined_names: Vec<String>,
}

/// Which side of the object cache a unit came from. Not in the original; used
/// only by the benchmark and the harness so a cache hit and a live compile can
/// be told apart without changing what the tool emits.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum CacheOutcome {
    Hit,
    Miss,
}

fn write_claimed_cache_entry(
    cached_object: &str,
    object_bytes: &[u8],
    cached_assembly: &str,
    assembly_bytes: &[u8],
    cached_meta: &str,
    meta_bytes: &[u8],
) -> Result<()> {
    write_cache_entry_atomically(Path::new(cached_object), object_bytes)
        .map_err(|error| format!("{cached_object}: {error}"))?;
    write_cache_entry_atomically(Path::new(cached_assembly), assembly_bytes)
        .map_err(|error| format!("{cached_assembly}: {error}"))?;
    // Metadata is the commit marker: publish it only after both payloads are
    // complete, so an interrupted writer cannot make a partial entry look
    // valid to a later cache lookup.
    write_cache_entry_atomically(Path::new(cached_meta), meta_bytes)
        .map_err(|error| format!("{cached_meta}: {error}"))?;
    Ok(())
}

fn claimed_cache_metadata(
    object_bytes: &[u8],
    assembly_bytes: &[u8],
    defined: &[String],
    undefined: &[String],
) -> String {
    let mut meta = Map::new();
    meta.insert("definedNames".into(), string_values(defined));
    meta.insert("undefinedNames".into(), string_values(undefined));
    meta.insert("objectSize".into(), Value::from(object_bytes.len() as u64));
    meta.insert(
        "objectSha256".into(),
        Value::String(sha256::sha256_hex(object_bytes)),
    );
    meta.insert(
        "assemblySize".into(),
        Value::from(assembly_bytes.len() as u64),
    );
    meta.insert(
        "assemblySha256".into(),
        Value::String(sha256::sha256_hex(assembly_bytes)),
    );
    // Keep the historical no-newline metadata representation byte-exact.
    canonical_json(&Value::Object(meta))
}

fn payload_matches_metadata(
    metadata: &Value,
    size_key: &str,
    digest_key: &str,
    payload: &[u8],
) -> bool {
    if metadata.get(size_key).and_then(Value::as_u64) != Some(payload.len() as u64) {
        return false;
    }
    let Some(expected) = metadata.get(digest_key).and_then(Value::as_str) else {
        return false;
    };
    expected == sha256::sha256_hex(payload)
}

fn read_claimed_cache_hit(
    cached_object: &str,
    cached_assembly: &str,
    cached_meta: &str,
    object: &str,
    assembly: &str,
) -> Option<Vec<Vec<String>>> {
    let meta = std::fs::read(cached_meta).ok()?;
    let parsed: Value = serde_json::from_slice(&meta).ok()?;
    let defined = string_array(parsed.get("definedNames"))?;
    let undefined = string_array(parsed.get("undefinedNames"))?;
    let object_bytes = std::fs::read(cached_object).ok()?;
    let assembly_bytes = std::fs::read(cached_assembly).ok()?;
    if !payload_matches_metadata(&parsed, "objectSize", "objectSha256", &object_bytes)
        || !payload_matches_metadata(&parsed, "assemblySize", "assemblySha256", &assembly_bytes)
    {
        return None;
    }
    std::fs::write(object, &object_bytes).ok()?;
    std::fs::write(assembly, &assembly_bytes).ok()?;
    Some(vec![defined, undefined])
}

fn compiler_target(target: DecompCompilerTarget) -> CompilerTarget {
    match target {
        DecompCompilerTarget::Gs1 => CompilerTarget::Gs1,
        DecompCompilerTarget::Gs2 => CompilerTarget::Gs2,
    }
}

/// `compileSource(source, objectDir, compiler)`.
pub fn compile_source(
    root: &str,
    object_cache: &str,
    source: &str,
    object_dir: &str,
    compiler: DecompCompilerTarget,
    stamps: &ToolchainStampCache,
) -> Result<(Compiled, CacheOutcome)> {
    let name = stem(source).to_string();
    let assembly = join(object_dir, &format!("{name}.s"));
    let object = join(object_dir, &format!("{name}.o"));
    let mut plan_options = SourceToAssemblyPlanOptions::new(
        compiler_target(compiler),
        source,
        source,
        assembly.clone(),
    );
    plan_options.preprocessed_output = Some(join(object_dir, &format!("{name}.i")));
    let plan = source_to_assembly_plan(&plan_options)?;
    let source_bytes = std::fs::read(source).map_err(|error| format!("{source}: {error}"))?;
    let commands: Vec<Vec<String>> = plan.steps.iter().map(|step| step.command.clone()).collect();
    let key = object_cache_key(&source_bytes, &stamps.stamp(&commands));
    let cached_object = join(object_cache, &format!("{key}.o"));
    let cached_assembly = join(object_cache, &format!("{key}.s"));
    let cached_meta = join(object_cache, &format!("{key}.json"));

    // Metadata is the commit marker. A new-key entry is a hit only when both
    // payloads still match the size and digest recorded before that marker.
    let hit = read_claimed_cache_hit(
        &cached_object,
        &cached_assembly,
        &cached_meta,
        &object,
        &assembly,
    );
    if let Some(mut lists) = hit {
        let undefined_names = lists.pop().expect("two lists");
        let defined_names = lists.pop().expect("two lists");
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
    if !defined.contains(&expected)
        || defined
            .iter()
            .any(|symbol| !is_lowercase_func_symbol(symbol))
    {
        return Err(format!(
            "{}: expected {expected} and address-named functions, found {}",
            basename(source),
            json_stringify_strings(&defined)
        ));
    }
    let undefined_names =
        last_fields(&run(root, &strings(&["arm-none-eabi-nm", "-u", &object]))?.stdout);
    for external in &undefined_names {
        if external_symbol(external, CALL_VIA_BASE).is_none() {
            return Err(format!(
                "{}: unsupported external {external}",
                basename(source)
            ));
        }
    }
    std::fs::create_dir_all(object_cache).map_err(|error| format!("{object_cache}: {error}"))?;
    let object_bytes = std::fs::read(&object).map_err(|error| format!("{object}: {error}"))?;
    let assembly_bytes =
        std::fs::read(&assembly).map_err(|error| format!("{assembly}: {error}"))?;
    let meta_bytes =
        claimed_cache_metadata(&object_bytes, &assembly_bytes, &defined, &undefined_names);
    write_claimed_cache_entry(
        &cached_object,
        &object_bytes,
        &cached_assembly,
        &assembly_bytes,
        &cached_meta,
        meta_bytes.as_bytes(),
    )?;
    Ok((
        Compiled {
            object,
            defined_names: defined,
            undefined_names,
        },
        CacheOutcome::Miss,
    ))
}

fn string_array(value: Option<&Value>) -> Option<Vec<String>> {
    let items = value?.as_array()?;
    items
        .iter()
        .map(|item| item.as_str().map(str::to_string))
        .collect()
}

fn string_values(items: &[String]) -> Value {
    Value::Array(
        items
            .iter()
            .map(|item| Value::String(item.clone()))
            .collect(),
    )
}

fn strings(items: &[&str]) -> Vec<String> {
    items.iter().map(|item| (*item).to_string()).collect()
}

/// `JSON.stringify(arrayOfStrings)`.
fn json_stringify_strings(items: &[String]) -> String {
    serde_json::to_string(&string_values(items)).expect("strings always serialize")
}

/// `output.split(/\r?\n/).filter(Boolean).map((line) => line.trim().split(/\s+/).at(-1)!)`.
///
/// `.filter(Boolean)` drops empty lines, so `at(-1)` is never asked for the
/// `undefined` it would return on an empty array -- but only because of that
/// filter, which is why the two steps stay together in one helper.
fn last_fields(output: &str) -> Vec<String> {
    js_split_lines(output)
        .into_iter()
        .filter(|line| !line.is_empty())
        .map(|line| {
            let fields = js_trim_split_whitespace(line);
            (*fields.last().expect("split always yields one element")).to_string()
        })
        .collect()
}

// ---------------------------------------------------------------------------
// mapLimit
// ---------------------------------------------------------------------------

/// `mapLimit(items, limit, action)`.
///
/// PORT NOTE -- `Promise.all` rejects with whichever rejection happens FIRST in
/// time, so the original's reported error under concurrency is nondeterministic
/// when several units fail. This port instead reports the LOWEST-INDEX failure,
/// which is deterministic and is a member of the same set. The harness compares
/// success/failure and the failing unit set rather than which of several
/// simultaneous failures won the race.
pub fn map_limit<T, U, F>(items: &[T], limit: usize, action: F) -> Result<Vec<U>>
where
    T: Sync,
    U: Send,
    F: Fn(&T) -> Result<U> + Sync,
{
    let width = limit.min(items.len()).max(1);
    let cursor = AtomicUsize::new(0);
    let results: Mutex<Vec<Option<std::result::Result<U, String>>>> =
        Mutex::new((0..items.len()).map(|_| None).collect());
    std::thread::scope(|scope| {
        for _ in 0..width {
            scope.spawn(|| loop {
                let index = cursor.fetch_add(1, AtomicOrdering::SeqCst);
                if index >= items.len() {
                    return;
                }
                let outcome = action(&items[index]);
                results.lock().expect("results are not poisoned")[index] = Some(outcome);
            });
        }
    });
    let mut out = Vec::with_capacity(items.len());
    let taken = results.into_inner().expect("results are not poisoned");
    for slot in taken {
        out.push(slot.expect("every index was visited")?);
    }
    Ok(out)
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------

/// What `main` printed and how it exited.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct BuildSummary {
    pub linked: usize,
    pub failures: Vec<String>,
    pub claimed_bytes: String,
    pub image_bytes: usize,
    /// Every path this run wrote, in write order. Not in the original; the
    /// harness uses it to prove neither mirror touched a tracked path.
    pub written: Vec<String>,
    pub cache_hits: usize,
    pub cache_misses: usize,
}

impl BuildSummary {
    /// The single line the original prints on success.
    pub fn summary_line(&self) -> String {
        format!(
            "linked={} failures={} claimed_bytes={} image_bytes={}",
            self.linked,
            self.failures.len(),
            self.claimed_bytes,
            self.image_bytes
        )
    }
}

/// `rooted(path)`.
fn rooted(root: &str, path: &str) -> String {
    if is_absolute(path) {
        path.to_string()
    } else {
        resolve(root, path)
    }
}

/// `main()`, minus the process-level printing and exit code.
pub fn build(options: &Options, root: &str, cwd: &str) -> Result<BuildSummary> {
    let target: DecompTarget = decomp_target(Some(options.target.as_str()))?;
    let object_cache = object_cache_dir(root);
    let mut written: Vec<String> = Vec::new();

    // `readFileSync(resolve(process.cwd(), args.rom))` -- CWD-relative, NOT
    // root-relative. A relative `--rom` therefore resolves differently from
    // every other path in this tool, which is in the original and is preserved.
    let rom: Option<Vec<u8>> = if options.source_only {
        None
    } else {
        let path = resolve(cwd, &options.rom);
        Some(std::fs::read(&path).map_err(|error| format!("{path}: {error}"))?)
    };
    if let Some(bytes) = &rom {
        if bytes.len() as u64 != target.rom_size {
            return Err(format!(
                "{} ROM must contain exactly {} bytes",
                target.id, target.rom_size
            ));
        }
    }

    let source_directory = rooted(root, target.source_dir);
    // Restricted to the plain 8-hex-digit main-image naming convention. The
    // comment in the original explains why: `exact/` also holds overlay sources
    // whose non-hex prefix parses as `NaN`, and a `Set` of `NaN`s collapses to
    // ONE entry, reporting a false "duplicate source address". The filter is the
    // fix that was already applied there; this port keeps it.
    let mut sources: Vec<String> = Vec::new();
    let entries = std::fs::read_dir(&source_directory)
        .map_err(|error| format!("{source_directory}: {error}"))?;
    for entry in entries {
        let entry = entry.map_err(|error| format!("{source_directory}: {error}"))?;
        // `entry.isFile()` follows nothing: node's Dirent reports the entry's
        // own type, so a symlink to a file is NOT a file here.
        let file_type = entry
            .file_type()
            .map_err(|error| format!("{source_directory}: {error}"))?;
        let name = entry.file_name().to_string_lossy().into_owned();
        if file_type.is_file() && is_main_image_source_name(&name) {
            sources.push(join(&source_directory, &name));
        }
    }
    js_default_sort(&mut sources);
    if sources.is_empty() {
        return Err("no reconstructed sources".to_string());
    }
    let addresses: Vec<f64> = sources
        .iter()
        .map(|source| js_parse_int(stem(source), 16).unwrap_or(f64::NAN))
        .collect();
    if !unique_numbers(&addresses) {
        return Err("duplicate source address".to_string());
    }
    let limit = ROM_BASE + target.rom_size as f64;
    if addresses
        .iter()
        .any(|address| *address < ROM_BASE || *address >= limit)
    {
        return Err("source address outside ROM".to_string());
    }

    let output = rooted(root, &options.output);
    let object_dir = join(&output, "obj");
    std::fs::create_dir_all(&object_dir).map_err(|error| format!("{object_dir}: {error}"))?;

    let stamps = ToolchainStampCache::new()?;
    let jobs = options.jobs as usize;
    let compiled_pairs = map_limit(&sources, jobs, |source| {
        compile_source(
            root,
            &object_cache,
            source,
            &object_dir,
            target.compiler,
            &stamps,
        )
    })?;
    let cache_hits = compiled_pairs
        .iter()
        .filter(|(_, outcome)| *outcome == CacheOutcome::Hit)
        .count();
    let cache_misses = compiled_pairs.len() - cache_hits;
    let mut compiled: Vec<Compiled> = compiled_pairs.into_iter().map(|(item, _)| item).collect();

    let objects: Vec<String> = compiled.iter().map(|item| item.object.clone()).collect();
    let definitions: Vec<String> = compiled
        .iter()
        .flat_map(|item| item.defined_names.clone())
        .collect();
    let mut defined = OrderedStringSet::new();
    for name in &definitions {
        defined.add(name);
    }
    if defined.len() != definitions.len() {
        return Err("duplicate function definition across C modules".to_string());
    }
    let mut undefined_set = OrderedStringSet::new();
    for name in compiled.iter().flat_map(|item| item.undefined_names.iter()) {
        if !defined.has(name) {
            undefined_set.add(name);
        }
    }
    let mut undefined_names: Vec<String> = undefined_set.iter().cloned().collect();
    js_default_sort(&mut undefined_names);

    let symbols_source = join(&output, "externals.s");
    let symbols_object = join(&output, "externals.o");
    let mut externals = String::from(".syntax unified\n.thumb\n");
    for name in &undefined_names {
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
            &symbols_object,
            &symbols_source,
        ]),
    )?;
    written.push(symbols_object.clone());

    let linker = join(&output, "claimed.ld");
    let mut script = format!(
        "OUTPUT_ARCH(arm)\nENTRY(Func_{})\nSECTIONS\n{{\n",
        stem(&sources[0])
    );
    for (index, source) in sources.iter().enumerate() {
        script.push_str(&format!(
            "  .func_{stem} 0x{stem} : {{ {object}(.text) }}\n",
            stem = stem(source),
            object = relative(root, &objects[index]),
        ));
    }
    script.push_str("  /DISCARD/ : { *(.comment) *(.note*) }\n}\n");
    write_file(&linker, script.as_bytes(), &mut written)?;

    let elf = join(&output, "claimed.elf");
    let binary = join(&output, "claimed.bin");
    run(
        root,
        &strings(&[
            "arm-none-eabi-ld",
            "-T",
            &linker,
            "-o",
            &elf,
            &symbols_object,
        ]),
    )?;
    written.push(elf.clone());
    run(
        root,
        &strings(&["arm-none-eabi-objcopy", "-O", "binary", &elf, &binary]),
    )?;
    written.push(binary.clone());

    let mut symbols = SymbolTable::new();
    let nm = run(
        root,
        &strings(&["arm-none-eabi-nm", "-S", "--defined-only", &elf]),
    )?;
    for line in js_split_lines(&nm.stdout) {
        let fields = js_trim_split_whitespace(line);
        if fields.len() == 4 && defined.has(fields[3]) {
            // `Number.parseInt(field, 16)`, the only integer parse path. A
            // malformed field yields NaN here and poisons the extent maths
            // downstream rather than failing, exactly as in the original.
            symbols.set(
                fields[3],
                js_parse_int(fields[0], 16).unwrap_or(f64::NAN),
                js_parse_int(fields[1], 16).unwrap_or(f64::NAN),
            );
        }
    }
    if symbols.len() != defined.len() {
        let mut missing: Vec<String> = defined
            .iter()
            .filter(|name| !symbols.has(name))
            .cloned()
            .collect();
        js_default_sort(&mut missing);
        return Err(format!(
            "missing linked functions: {}",
            json_stringify_strings(&missing)
        ));
    }

    let image = std::fs::read(&binary).map_err(|error| format!("{binary}: {error}"))?;
    let image_base = js_min(&symbols.addresses());
    let mut manifest: Vec<Value> = Vec::new();
    let mut failures: Vec<String> = Vec::new();
    let mut total = 0f64;
    let mut previous_end = 0f64;
    for (source_index, source) in sources.iter().enumerate() {
        let name = format!("Func_{}", stem(source));
        let (address, _) = symbols
            .get(&name)
            .ok_or_else(|| format!("missing linked function {name}"))?;
        let claimed = js_parse_int(stem(source), 16).unwrap_or(f64::NAN);
        if address != claimed {
            failures.push(format!(
                "{}: linked at 0x{}",
                basename(source),
                js_hex_pad8(address)
            ));
            continue;
        }
        // `definedNames.sort(comparator)` mutates the compiled entry in place.
        // The comparator is `a - b` on addresses: it can return NaN, which the
        // specification treats as 0, so a NaN address leaves the pair in its
        // existing order rather than throwing. The sort is STABLE.
        let module_symbols = {
            let entry = &mut compiled[source_index].defined_names;
            entry.sort_by(|left, right| {
                let left = symbols.get(left).map_or(f64::NAN, |(address, _)| address);
                let right = symbols.get(right).map_or(f64::NAN, |(address, _)| address);
                js_numeric_compare(left, right)
            });
            entry.clone()
        };
        let end = module_end(&module_symbols, &symbols)
            .map_err(|cause| format!("{}: {cause}", basename(source)))?;
        let size = end - address;
        if address < previous_end {
            failures.push(format!("{}: overlaps previous function", basename(source)));
        }
        previous_end = js_max(&[previous_end, end]);
        let offset = address - image_base;
        let actual = js_subarray(&image, offset, offset + size);
        if end > limit {
            failures.push(format!("{}: linked extent outside ROM", basename(source)));
        }
        if let Some(rom) = &rom {
            let expected = js_subarray(rom, address - ROM_BASE, end - ROM_BASE);
            if actual != expected {
                failures.push(format!("{}: linked bytes differ", basename(source)));
            }
        }
        total += size;
        for symbol in &module_symbols {
            let expected_address = js_parse_int(&symbol[5..], 16).unwrap_or(f64::NAN);
            let actual_address = symbols.get(symbol).map_or(f64::NAN, |(address, _)| address);
            // `!==` is false for NaN on both sides, but the two range tests
            // below are also false for NaN, so a NaN address reports nothing.
            // Written with explicit comparisons rather than a clamp, because
            // `f64::clamp` panics on NaN bounds.
            if actual_address != expected_address
                || expected_address < address
                || expected_address >= end
            {
                failures.push(format!(
                    "{}: invalid module symbol {symbol}",
                    basename(source)
                ));
            }
        }
        let mut region = Map::new();
        region.insert("source".into(), Value::String(relative(root, source)));
        region.insert("symbol".into(), Value::String(name));
        region.insert("symbols".into(), string_values(&module_symbols));
        region.insert("address".into(), js_number(address));
        region.insert("size".into(), js_number(size));
        region.insert("end".into(), js_number(end));
        manifest.push(Value::Object(region));
    }

    let mut document = Map::new();
    document.insert("format".into(), js_number(1.0));
    document.insert("target".into(), Value::String(target.id.to_string()));
    document.insert(
        "compiler".into(),
        Value::String(target.compiler.to_string()),
    );
    document.insert("rom_base".into(), js_number(ROM_BASE));
    document.insert("rom_size".into(), js_number(target.rom_size as f64));
    document.insert(
        "verification".into(),
        Value::String(
            if options.source_only {
                "source_only"
            } else {
                "rom"
            }
            .to_string(),
        ),
    );
    document.insert("image_base".into(), js_number(image_base));
    document.insert("image_size".into(), js_number(image.len() as f64));
    document.insert("claimed_bytes".into(), js_number(total));
    document.insert("regions".into(), Value::Array(manifest.clone()));
    let text = format!("{}\n", canonical_json(&Value::Object(document)));
    write_file(
        &join(&output, "manifest.json"),
        text.as_bytes(),
        &mut written,
    )?;

    Ok(BuildSummary {
        linked: manifest.len(),
        failures,
        // `${total}` is a JS number template; an integral value prints without
        // a decimal point and NaN prints as `NaN`.
        claimed_bytes: js_number_to_display(total),
        image_bytes: image.len(),
        written,
        cache_hits,
        cache_misses,
    })
}

/// `${value}` for a number in a template literal.
pub fn js_number_to_display(value: f64) -> String {
    if value.is_nan() {
        return "NaN".to_string();
    }
    if value.is_infinite() {
        return if value > 0.0 {
            "Infinity".into()
        } else {
            "-Infinity".into()
        };
    }
    if js_is_integer(value) && value.abs() < 1e21 {
        return format!("{}", value as i64);
    }
    format!("{value}")
}

fn write_file(path: &str, bytes: &[u8], written: &mut Vec<String>) -> Result<()> {
    let mut file = std::fs::File::create(path).map_err(|error| format!("{path}: {error}"))?;
    file.write_all(bytes)
        .map_err(|error| format!("{path}: {error}"))?;
    written.push(path.to_string());
    Ok(())
}

/// `new Set(numbers).size === numbers.length`.
///
/// `Set` is SameValueZero: `NaN` equals `NaN` (so a list of two `NaN`s DEDUPES
/// and reports a duplicate) and `0` equals `-0`. Neither `PartialEq` nor
/// `Vec::contains` models that, which is why this is written out.
fn unique_numbers(values: &[f64]) -> bool {
    let mut seen: Vec<f64> = Vec::with_capacity(values.len());
    for &value in values {
        let duplicate = seen.iter().any(|&candidate| {
            // SameValueZero.
            (candidate.is_nan() && value.is_nan()) || candidate == value
        });
        if duplicate {
            return false;
        }
        seen.push(value);
    }
    true
}

/// A convenience for callers that only have a path.
pub fn exists(path: &str) -> bool {
    // `existsSync` follows symlinks; `Path::exists` does too, and
    // `symlink_metadata` is the `lstatSync` form. The follow is deliberate.
    Path::new(path).exists()
}

#[cfg(test)]
mod tests {
    use super::*;

    fn table(entries: &[(&str, f64, f64)]) -> SymbolTable {
        let mut symbols = SymbolTable::new();
        for (name, address, size) in entries {
            symbols.set(name, *address, *size);
        }
        symbols
    }

    #[test]
    fn self_test_reports_ok() {
        assert_eq!(self_test().unwrap(), "self-test=ok");
    }

    #[test]
    fn module_end_takes_the_furthest_extent() {
        let symbols = table(&[("Func_a", 100.0, 4.0), ("Func_b", 200.0, 8.0)]);
        let names = vec!["Func_a".to_string(), "Func_b".to_string()];
        assert_eq!(module_end(&names, &symbols).unwrap(), 208.0);
    }

    #[test]
    fn module_end_rejects_an_empty_module() {
        let symbols = SymbolTable::new();
        assert_eq!(
            module_end(&[], &symbols).unwrap_err(),
            "C module has no functions"
        );
    }

    #[test]
    fn module_end_rejects_a_missing_or_empty_symbol() {
        let symbols = table(&[("Func_a", 100.0, 0.0)]);
        assert_eq!(
            module_end(&["Func_a".to_string()], &symbols).unwrap_err(),
            "invalid module symbol Func_a"
        );
        assert_eq!(
            module_end(&["Func_z".to_string()], &symbols).unwrap_err(),
            "invalid module symbol Func_z"
        );
    }

    #[test]
    fn nan_size_passes_the_guard_and_poisons_the_extent() {
        // THE BUG, PINNED. `NaN <= 0` is false, so the guard admits it, and
        // `Math.max` then propagates it. A port using `f64::max` would return
        // 104 here and silently claim a plausible extent.
        let symbols = table(&[("Func_a", 100.0, 4.0), ("Func_b", 200.0, f64::NAN)]);
        let names = vec!["Func_a".to_string(), "Func_b".to_string()];
        assert!(module_end(&names, &symbols).unwrap().is_nan());
        assert_eq!(f64::NAN.max(104.0), 104.0, "what the naive port would say");
    }

    #[test]
    fn object_cache_key_separates_its_two_chunks() {
        // Without the NUL these two would collide.
        assert_ne!(object_cache_key(b"yz", "x"), object_cache_key(b"z", "xy"));
    }

    #[test]
    fn object_cache_key_has_no_version_literal() {
        let source = std::fs::read_to_string(concat!(env!("CARGO_MANIFEST_DIR"), "/src/lib.rs"))
            .expect("own source is readable");
        // A hand-maintained `-vN` key is the failure that poisoned
        // `out/cache/overlay-c`. Assert the shape can never appear here.
        let implementation = source
            .split("#[cfg(test)]")
            .next()
            .expect("the implementation precedes its tests");
        for marker in ["-v1\"", "-v2\"", "-v3\"", "-v4\"", "\"claimed-v"] {
            assert!(
                !implementation.contains(marker),
                "cache key must digest inputs, never a version literal: {marker}"
            );
        }
    }

    #[test]
    fn parse_args_defaults_to_gs1() {
        let ParsedArgs::Run(options) = parse_args(&[]).unwrap() else {
            panic!("expected a run");
        };
        assert_eq!(options.target, DecompTargetId::Gs1En);
        assert_eq!(options.rom, "roms/gs1-en.gba");
        assert_eq!(options.output, "out/claimed");
        assert!(!options.source_only);
    }

    #[test]
    fn target_is_resolved_before_the_output_default() {
        // `--target` is scanned in a separate first pass, so it wins even when
        // it appears AFTER the flag whose default depends on it.
        let ParsedArgs::Run(options) =
            parse_args(&["--source-only".into(), "--target=gs2-en".into()]).unwrap()
        else {
            panic!("expected a run");
        };
        assert_eq!(options.output, "out/gs2-en/claimed");
        assert_eq!(options.rom, "roms/gs2-en.gba");
    }

    #[test]
    fn jobs_goes_through_the_parse_int_path() {
        // `parseInt("3.9", 10)` is 3 and PASSES `Number.isInteger`.
        let ParsedArgs::Run(options) = parse_args(&["--jobs=3.9".into()]).unwrap() else {
            panic!("expected a run");
        };
        assert_eq!(options.jobs, 3.0);
        // `parseInt("8abc", 10)` is 8.
        let ParsedArgs::Run(options) = parse_args(&["--jobs".into(), "8abc".into()]).unwrap()
        else {
            panic!("expected a run");
        };
        assert_eq!(options.jobs, 8.0);
        // A `0x` prefix is accepted by parseInt at radix 10 only up to the `x`.
        // `parseInt("0x10", 10)` stops at the x, producing zero, which the
        // positivity check rejects.
        assert_eq!(
            parse_args(&["--jobs=0x10".into()]).unwrap_err(),
            "jobs must be positive"
        );
        assert_eq!(
            parse_args(&["--jobs=x".into()]).unwrap_err(),
            "jobs must be positive"
        );
        assert_eq!(
            parse_args(&["--jobs=0".into()]).unwrap_err(),
            "jobs must be positive"
        );
    }

    #[test]
    fn unrecognized_and_missing_values_are_rejected() {
        assert_eq!(
            parse_args(&["--nope".into()]).unwrap_err(),
            "unrecognized argument: --nope"
        );
        assert_eq!(
            parse_args(&["--jobs".into()]).unwrap_err(),
            "--jobs requires a value"
        );
        assert_eq!(
            parse_args(&["--target".into()]).unwrap_err(),
            "--target requires a value"
        );
        assert!(parse_args(&["--target=nope".into()])
            .unwrap_err()
            .starts_with("unsupported decomp target"));
    }

    #[test]
    fn source_only_refuses_a_rom_positional() {
        assert_eq!(
            parse_args(&["--source-only".into(), "roms/gs1-en.gba".into()]).unwrap_err(),
            "--source-only does not accept a ROM"
        );
        // A second positional is an unrecognized argument, not a second ROM.
        assert_eq!(
            parse_args(&["a".into(), "b".into()]).unwrap_err(),
            "unrecognized argument: b"
        );
    }

    #[test]
    fn help_short_circuits() {
        assert_eq!(parse_args(&["-h".into()]).unwrap(), ParsedArgs::Help);
        assert_eq!(
            parse_args(&["--help".into(), "--nope".into()]).unwrap(),
            ParsedArgs::Help,
            "help wins because it is reached first"
        );
    }

    #[test]
    fn duplicate_detection_is_same_value_zero() {
        assert!(unique_numbers(&[1.0, 2.0, 3.0]));
        assert!(!unique_numbers(&[1.0, 1.0]));
        // SameValueZero: two NaNs DEDUPE, so this reports a duplicate. This is
        // the exact false positive the filename filter exists to avoid.
        assert!(!unique_numbers(&[f64::NAN, f64::NAN]));
        // ...and 0 equals -0.
        assert!(!unique_numbers(&[0.0, -0.0]));
    }

    #[test]
    fn ordered_set_keeps_insertion_order_and_dedupes_silently() {
        let mut set = OrderedStringSet::new();
        for name in ["b", "a", "b", "c"] {
            set.add(name);
        }
        assert_eq!(
            set.iter().cloned().collect::<Vec<_>>(),
            vec!["b", "a", "c"],
            "insertion order, not sorted"
        );
        assert_eq!(set.len(), 3);
    }

    #[test]
    fn symbol_table_set_keeps_position_on_overwrite() {
        let mut symbols = SymbolTable::new();
        symbols.set("a", 1.0, 1.0);
        symbols.set("b", 2.0, 1.0);
        symbols.set("a", 9.0, 1.0);
        assert_eq!(symbols.get("a"), Some((9.0, 1.0)));
        assert_eq!(symbols.addresses(), vec![9.0, 2.0]);
    }

    #[test]
    fn last_fields_drops_blank_lines_and_takes_the_last_column() {
        let output = "0801c0c8 T Func_0801c0c8\n\n         U Func_08000100\r\n";
        assert_eq!(
            last_fields(output),
            vec!["Func_0801c0c8".to_string(), "Func_08000100".to_string()]
        );
        assert_eq!(last_fields(""), Vec::<String>::new());
    }

    #[test]
    fn map_limit_preserves_input_order_under_concurrency() {
        let items: Vec<usize> = (0..200).collect();
        let out = map_limit(&items, 8, |item| Ok(*item * 2)).unwrap();
        assert_eq!(out, items.iter().map(|item| item * 2).collect::<Vec<_>>());
    }

    #[test]
    fn map_limit_reports_the_lowest_index_failure() {
        let items: Vec<usize> = (0..50).collect();
        let out: Result<Vec<usize>> = map_limit(&items, 8, |item| {
            if *item == 7 || *item == 30 {
                Err(format!("boom {item}"))
            } else {
                Ok(*item)
            }
        });
        assert_eq!(out.unwrap_err(), "boom 7");
    }

    #[test]
    fn map_limit_handles_an_empty_list() {
        let items: Vec<usize> = Vec::new();
        assert_eq!(
            map_limit(&items, 8, |item| Ok(*item)).unwrap(),
            Vec::<usize>::new()
        );
    }

    #[test]
    fn stem_strips_only_the_final_extension() {
        assert_eq!(stem("/a/b/0801c0c8.c"), "0801c0c8");
        assert_eq!(stem("/a/b/claimed.elf"), "claimed");
        assert_eq!(stem("/a/b/noext"), "noext");
    }

    #[test]
    fn display_of_a_number_has_no_decimal_point() {
        assert_eq!(js_number_to_display(109_020.0), "109020");
        assert_eq!(js_number_to_display(0.0), "0");
        assert_eq!(js_number_to_display(f64::NAN), "NaN");
    }

    #[test]
    fn root_is_the_repository_root() {
        let root = root();
        assert!(exists(&join(&root, "tools/build-claimed/Cargo.toml")));
    }

    #[test]
    fn object_cache_path_is_the_documented_one() {
        assert_eq!(object_cache_dir("/r"), "/r/out/cache/claimed-objects");
        // NOT the poisoned `out/cache/overlay-c`. This tool never reads that
        // cache, so it cannot serve the 160-byte `resource_39c` entry.
        assert!(!object_cache_dir("/r").contains("overlay-c"));
    }

    #[test]
    fn claimed_cache_rewrites_keep_all_three_files_byte_exact() {
        let directory = std::env::temp_dir().join(format!(
            "alchemy-build-claimed-cache-{}",
            std::process::id()
        ));
        let _ = std::fs::remove_dir_all(&directory);
        std::fs::create_dir_all(&directory).unwrap();
        let object = directory.join("key.o");
        let assembly = directory.join("key.s");
        let metadata = directory.join("key.json");

        write_claimed_cache_entry(
            object.to_str().unwrap(),
            &[1, 2, 3, 4],
            assembly.to_str().unwrap(),
            b"long assembly",
            metadata.to_str().unwrap(),
            br#"{"definedNames":["Func_old"],"undefinedNames":[]}"#,
        )
        .unwrap();
        write_claimed_cache_entry(
            object.to_str().unwrap(),
            &[9, 8],
            assembly.to_str().unwrap(),
            b"short",
            metadata.to_str().unwrap(),
            br#"{"definedNames":["Func_new"],"undefinedNames":[]}"#,
        )
        .unwrap();

        assert_eq!(std::fs::read(&object).unwrap(), [9, 8]);
        assert_eq!(std::fs::read(&assembly).unwrap(), b"short");
        assert_eq!(
            std::fs::read(&metadata).unwrap(),
            br#"{"definedNames":["Func_new"],"undefinedNames":[]}"#
        );
        std::fs::remove_dir_all(directory).unwrap();
    }

    fn cache_fixture(
        name: &str,
        object_bytes: &[u8],
        assembly_bytes: &[u8],
        metadata: &[u8],
    ) -> (
        std::path::PathBuf,
        std::path::PathBuf,
        std::path::PathBuf,
        std::path::PathBuf,
        std::path::PathBuf,
    ) {
        let directory = std::env::temp_dir().join(format!(
            "alchemy-build-claimed-cache-{name}-{}",
            std::process::id()
        ));
        let _ = std::fs::remove_dir_all(&directory);
        std::fs::create_dir_all(&directory).unwrap();
        let cached_object = directory.join("cached.o");
        let cached_assembly = directory.join("cached.s");
        let cached_meta = directory.join("cached.json");
        let object = directory.join("output.o");
        std::fs::write(&cached_object, object_bytes).unwrap();
        std::fs::write(&cached_assembly, assembly_bytes).unwrap();
        std::fs::write(&cached_meta, metadata).unwrap();
        (
            directory,
            cached_object,
            cached_assembly,
            cached_meta,
            object,
        )
    }

    #[test]
    fn claimed_cache_hit_accepts_matching_payload_digests() {
        let object_bytes = b"object payload";
        let assembly_bytes = b"assembly payload";
        let defined = vec!["Func_08000000".to_string()];
        let metadata = claimed_cache_metadata(object_bytes, assembly_bytes, &defined, &[]);
        let (directory, cached_object, cached_assembly, cached_meta, object) =
            cache_fixture("valid", object_bytes, assembly_bytes, metadata.as_bytes());
        let assembly = directory.join("output.s");

        assert_eq!(
            read_claimed_cache_hit(
                cached_object.to_str().unwrap(),
                cached_assembly.to_str().unwrap(),
                cached_meta.to_str().unwrap(),
                object.to_str().unwrap(),
                assembly.to_str().unwrap(),
            ),
            Some(vec![defined, Vec::new()])
        );
        assert_eq!(std::fs::read(object).unwrap(), object_bytes);
        assert_eq!(std::fs::read(assembly).unwrap(), assembly_bytes);
        std::fs::remove_dir_all(directory).unwrap();
    }

    #[test]
    fn claimed_cache_hit_rejects_corrupt_or_truncated_payloads() {
        let object_bytes = b"object payload";
        let assembly_bytes = b"assembly payload";
        let metadata = claimed_cache_metadata(object_bytes, assembly_bytes, &[], &[]);
        let (directory, cached_object, cached_assembly, cached_meta, object) =
            cache_fixture("corrupt", object_bytes, assembly_bytes, metadata.as_bytes());
        let assembly = directory.join("output.s");
        let paths = || {
            (
                cached_object.to_str().unwrap(),
                cached_assembly.to_str().unwrap(),
                cached_meta.to_str().unwrap(),
                object.to_str().unwrap(),
                assembly.to_str().unwrap(),
            )
        };

        std::fs::write(&cached_object, b"object corrupt").unwrap();
        assert!(
            read_claimed_cache_hit(paths().0, paths().1, paths().2, paths().3, paths().4).is_none()
        );
        std::fs::write(&cached_object, object_bytes).unwrap();
        std::fs::write(&cached_assembly, b"corrupt").unwrap();
        assert!(
            read_claimed_cache_hit(paths().0, paths().1, paths().2, paths().3, paths().4).is_none()
        );
        std::fs::remove_dir_all(directory).unwrap();
    }

    #[test]
    fn claimed_cache_hit_rejects_metadata_without_new_digest_fields() {
        let object_bytes = b"object payload";
        let assembly_bytes = b"assembly payload";
        let legacy = br#"{"definedNames":[],"undefinedNames":[]}"#;
        let (directory, cached_object, cached_assembly, cached_meta, object) =
            cache_fixture("legacy-metadata", object_bytes, assembly_bytes, legacy);
        let assembly = directory.join("output.s");

        assert!(read_claimed_cache_hit(
            cached_object.to_str().unwrap(),
            cached_assembly.to_str().unwrap(),
            cached_meta.to_str().unwrap(),
            object.to_str().unwrap(),
            assembly.to_str().unwrap(),
        )
        .is_none());
        std::fs::remove_dir_all(directory).unwrap();
    }

    #[test]
    fn toolchain_stamp_frames_argv_and_removes_the_pinned_collision() {
        let left = vec![strings(&["/bin/does-not-exist", "-o", "x"])];
        let right = vec![strings(&["/bin/does-not-exist", "-ox"])];
        let signatures = CacheSignatures::injected("compiler-a", "binutils-a", "implementation-a");
        assert_ne!(
            toolchain_stamp_with_signatures(&left, &signatures),
            toolchain_stamp_with_signatures(&right, &signatures)
        );

        let cache = ToolchainStampCache::with_signatures(signatures);
        assert_ne!(cache.stamp(&left), cache.stamp(&right));
    }

    #[test]
    fn stamp_cache_returns_the_same_answer_as_the_uncached_form() {
        let signatures = CacheSignatures::injected("compiler-a", "binutils-a", "implementation-a");
        let cache = ToolchainStampCache::with_signatures(signatures.clone());
        let commands = vec![strings(&["arm-none-eabi-as", "-o", "x"])];
        assert_eq!(
            cache.stamp(&commands),
            toolchain_stamp_with_signatures(&commands, &signatures)
        );
        assert_eq!(
            cache.stamp(&commands),
            toolchain_stamp_with_signatures(&commands, &signatures)
        );
    }

    #[test]
    fn toolchain_stamp_changes_for_compiler_and_binutil_signatures() {
        let commands = vec![strings(&["compiler", "-o", "object"])];
        let base = CacheSignatures::injected("compiler-a", "binutils-a", "implementation-a");
        let changed_compiler =
            CacheSignatures::injected("compiler-b", "binutils-a", "implementation-a");
        let changed_binutils =
            CacheSignatures::injected("compiler-a", "binutils-b", "implementation-a");
        let changed_implementation =
            CacheSignatures::injected("compiler-a", "binutils-a", "implementation-b");
        let base_stamp = toolchain_stamp_with_signatures(&commands, &base);
        assert_ne!(
            base_stamp,
            toolchain_stamp_with_signatures(&commands, &changed_compiler)
        );
        assert_ne!(
            base_stamp,
            toolchain_stamp_with_signatures(&commands, &changed_binutils)
        );
        assert_ne!(
            base_stamp,
            toolchain_stamp_with_signatures(&commands, &changed_implementation)
        );
    }
}
