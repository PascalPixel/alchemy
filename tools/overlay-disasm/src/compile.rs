use crate::paths::{basename, root, OverlaySource};
use crate::regex::Regex;
use compiler_core::plan::{source_to_assembly_plan, CompilerFlagMutations, SourceToAssemblyPlanOptions};
use compiler_core::routing::CompilerTarget;
use compiler_core::sha256;
use compiler_core::source_paths::{SourceOwner, SourcePaths};
use compiler_core::{external_symbol, external_symbol_assembly, overlay_call_via_base};
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::Mutex;
use tempfile::tempdir;
pub fn hex(value: i64, width: usize) -> String {
    if value < 0 {
        let body = format!("{:x}", value.unsigned_abs());
        let padded = format!("{body:0>width$}", width = width);
        return format!("-{padded}");
    }
    format!("{value:0width$x}", width = width)
}
const LOCAL_LABEL_DEFINITION: &str = r"^(\.L[A-Za-z0-9_$.]*):";
const LOCAL_LABEL_WORD: &str = r"^(\s*\.word\s+)(\.L[A-Za-z0-9_$.]*)\s*$";
pub const OVERLAY_LINK_BIAS: i64 = 0x8000;
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct BiasResult {
    pub text: String,
    pub biased: usize,
}
pub fn bias_in_image_label_words(assembly: &str) -> BiasResult {
    let definition = Regex::new(LOCAL_LABEL_DEFINITION, "");
    let word = Regex::new(LOCAL_LABEL_WORD, "");
    let lines: Vec<&str> = assembly.split('\n').collect();
    let mut defined: std::collections::BTreeSet<String> = std::collections::BTreeSet::new();
    for line in &lines {
        let trimmed = crate::regex::js_trim(line);
        if let Some(found) = definition.exec(trimmed) {
            defined.insert(found.group(trimmed, 1).expect("group 1").to_string());
        }
    }
    let mut biased = 0usize;
    let mut out: Vec<String> = Vec::with_capacity(lines.len());
    for line in &lines {
        match word.exec(line) {
            Some(found) => {
                let label = found.group(line, 2).expect("group 2");
                if !defined.contains(label) {
                    out.push((*line).to_string());
                    continue;
                }
                biased += 1;
                let head = found.group(line, 1).expect("group 1");
                out.push(format!("{head}{label} + 0x{:x}", OVERLAY_LINK_BIAS));
            }
            None => out.push((*line).to_string()),
        }
    }
    BiasResult { text: out.join("\n"), biased }
}
pub fn overlay_c_cache_dir() -> PathBuf {
    match std::env::var_os("ALCHEMY_OVERLAY_C_CACHE") {
        Some(value) => PathBuf::from(value),
        None => root().join("out/cache/overlay-c"),
    }
}
const SELF_SOURCE: [&[u8]; 6] = [
    include_bytes!("lib.rs"),
    include_bytes!("compile.rs"),
    include_bytes!("disasm.rs"),
    include_bytes!("paths.rs"),
    include_bytes!("regex.rs"),
    include_bytes!("cli.rs"),
];
const LINKED_POSTPROCESS_SOURCE: [&[u8]; 3] = [
    include_bytes!("../../compiler-core/src/lib.rs"),
    include_bytes!("../../compiler-core/src/call_via_data.rs"),
    include_bytes!("../../compiler-core/src/symbols.rs"),
];
pub fn self_digest() -> String {
    static CACHE: Mutex<Option<String>> = Mutex::new(None);
    let mut slot = CACHE.lock().expect("self-digest lock");
    if let Some(found) = slot.as_ref() {
        return found.clone();
    }
    let mut stream: Vec<u8> = Vec::new();
    for part in SELF_SOURCE {
        stream.extend_from_slice(part);
    }
    for part in LINKED_POSTPROCESS_SOURCE {
        stream.extend_from_slice(part);
    }
    assert!(!stream.is_empty(), "overlay_disasm read an EMPTY source; refusing to key the cache");
    let digest = sha256::hex(&stream);
    *slot = Some(digest.clone());
    digest
}
fn plan_stamp(commands: &[Vec<String>], work: &str) -> String {
    static MEMO: Mutex<Option<BTreeMap<Vec<u8>, String>>> = Mutex::new(None);
    let identity = command_identity(commands, work);
    {
        let guard = MEMO.lock().expect("plan-stamp lock");
        if let Some(found) = guard.as_ref().and_then(|memo| memo.get(&identity)) {
            return found.clone();
        }
    }
    let mut stream = identity.clone();
    for command in commands {
        let Some(binary) = command.first() else {
            continue;
        };
        if !binary.starts_with('/') {
            continue;
        }
        match fs::read(binary) {
            Ok(bytes) => append_frame(&mut stream, &bytes),
            Err(_) => append_frame(&mut stream, b"unreadable"),
        }
    }
    let stamp = sha256::hex(&stream);
    let mut guard = MEMO.lock().expect("plan-stamp lock");
    guard.get_or_insert_with(BTreeMap::new).insert(identity, stamp.clone());
    stamp
}
fn append_frame(stream: &mut Vec<u8>, bytes: &[u8]) {
    stream.extend_from_slice(&(bytes.len() as u64).to_be_bytes());
    stream.extend_from_slice(bytes);
}
fn command_identity(commands: &[Vec<String>], work: &str) -> Vec<u8> {
    let mut identity = Vec::new();
    append_frame(&mut identity, b"overlay-plan-v2");
    identity.extend_from_slice(&(commands.len() as u64).to_be_bytes());
    for command in commands {
        identity.extend_from_slice(&(command.len() as u64).to_be_bytes());
        for part in command {
            let normalized = if part.starts_with(work) { "<work>" } else { part };
            append_frame(&mut identity, normalized.as_bytes());
        }
    }
    identity
}
fn overlay_c_sources_checked(source: &OverlaySource) -> Result<Vec<PathBuf>, String> {
    let Some(anchor) = source.c_source_anchor() else {
        return Ok(Vec::new());
    };
    let overlay = Regex::new(r"_overlay\.s$", "").replace_first(basename(anchor), "");
    let text = source.read_text().map_err(|error| error.to_string())?;
    let paths = SourcePaths::load(&root())?;
    let mut found = Vec::new();
    for address in placeholder_addresses(&text) {
        let owner = SourceOwner::parse(&format!("{overlay}:{address:08x}"))?;
        let path = paths.source_path(owner);
        if !path.exists() {
            return Err(format!(
                "{overlay}:{address:08x} has an AlchemyC placeholder but no exact C source at {}",
                path.display()
            ));
        }
        found.push(path);
    }
    Ok(found)
}

fn placeholder_addresses(assembly: &str) -> Vec<u32> {
    let mut found = assembly
        .lines()
        .filter_map(|line| {
            line.trim()
                .strip_prefix("AlchemyC_")?
                .strip_suffix(':')
                .and_then(|address| u32::from_str_radix(address, 16).ok())
        })
        .collect::<Vec<_>>();
    found.sort_unstable();
    found.dedup();
    found
}

pub fn overlay_c_sources(source: &OverlaySource) -> Result<Vec<PathBuf>, String> {
    overlay_c_sources_checked(source)
}

fn address_stem(path: &Path, overlay: &str) -> Result<(SourceOwner, String, i64), String> {
    let paths = SourcePaths::load(&root())?;
    if let Some(owner) = paths.overlay_owner_for_path(overlay, path)? {
        let stem = owner.address_stem();
        return Ok((owner, stem, owner.address() as i64));
    }
    let display = path.to_string_lossy().to_string();
    let name = basename(&display);
    let stem = name.strip_suffix(".c").unwrap_or(name);
    let tail = stem.rsplit_once("_c_").map(|(_, tail)| tail).unwrap_or(stem);
    if tail.len() != 8 || !tail.bytes().all(|b| b.is_ascii_hexdigit()) {
        return Err(format!("overlay C filename is not an address: {display}"));
    }
    let address = i64::from_str_radix(tail, 16).map_err(|error| error.to_string())?;
    let resource = overlay
        .strip_prefix("resource_")
        .and_then(|value| u16::from_str_radix(value, 16).ok())
        .ok_or_else(|| format!("invalid overlay owner {overlay:?}"))?;
    let owner = SourceOwner::from_legacy_stem(stem)
        .filter(|owner| owner.overlay_id().as_deref() == Some(overlay))
        .unwrap_or(SourceOwner::Overlay { resource, address: address as u32 });
    Ok((owner, tail.to_ascii_lowercase(), address))
}
fn checked(command: &[String], cwd: &Path) -> Result<String, String> {
    let Some((binary, rest)) = command.split_first() else {
        return Err("empty command".to_string());
    };
    let output = Command::new(binary)
        .args(rest)
        .current_dir(cwd)
        .output()
        .map_err(|error| format!("{} failed: {error}", basename(binary)))?;
    let stdout = String::from_utf8_lossy(&output.stdout).to_string();
    if output.status.code() != Some(0) {
        let stderr = String::from_utf8_lossy(&output.stderr).to_string();
        let detail = crate::regex::js_trim(if stderr.is_empty() { &stdout } else { &stderr });
        let suffix = if detail.is_empty() { String::new() } else { format!(": {detail}") };
        return Err(format!("{} failed{suffix}", basename(binary)));
    }
    Ok(stdout)
}
pub(crate) fn spawn_raw(command: &[String], cwd: &Path) -> Result<Vec<u8>, String> {
    let (binary, rest) = command.split_first().ok_or_else(|| "empty command".to_string())?;
    let output =
        Command::new(binary).args(rest).current_dir(cwd).output().map_err(|error| format!("{binary}: {error}"))?;
    if output.status.code() != Some(0) {
        return Err(crate::regex::js_trim(&String::from_utf8_lossy(&output.stderr)).to_string());
    }
    Ok(output.stdout)
}
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Compiled {
    pub address: i64,
    pub data: Vec<u8>,
}
const OVERLAY_HOST_TOOLS: [&str; 4] =
    ["arm-none-eabi-as", "arm-none-eabi-nm", "arm-none-eabi-ld", "arm-none-eabi-objcopy"];
fn cache_record_path(cached: &Path) -> PathBuf {
    cached.with_extension("record")
}
fn cache_record(data: &[u8]) -> String {
    format!("overlay-c-cache-v1\n{}\n{}\n", data.len(), sha256::hex(data))
}
fn read_valid_cache_entry(cached: &Path) -> Option<Vec<u8>> {
    let data = fs::read(cached).ok()?;
    let record = fs::read_to_string(cache_record_path(cached)).ok()?;
    let mut lines = record.lines();
    if lines.next() != Some("overlay-c-cache-v1") {
        return None;
    }
    let size = lines.next()?.parse::<usize>().ok()?;
    let digest = lines.next()?;
    if lines.next().is_some() || size != data.len() || digest.len() != 64 || digest != sha256::hex(&data) {
        return None;
    }
    Some(data)
}
fn publish_cache_entry(cached: &Path, data: &[u8]) {
    if cache_entry::write_cache_entry_atomically(cached, data).is_ok() {
        let _ = cache_entry::write_cache_entry_atomically(&cache_record_path(cached), cache_record(data).as_bytes());
    }
}
fn overlay_cache_key(
    compiler_signature: &str,
    host_signature: &str,
    plan_signature: &str,
    address: i64,
    call_via_base: i64,
    source_inputs: &[u8],
) -> String {
    let mut key = Vec::new();
    append_frame(&mut key, b"overlay-c-cache-v3");
    append_frame(&mut key, self_digest().as_bytes());
    append_frame(&mut key, compiler_signature.as_bytes());
    append_frame(&mut key, host_signature.as_bytes());
    append_frame(&mut key, plan_signature.as_bytes());
    append_frame(&mut key, hex(address, 8).as_bytes());
    append_frame(&mut key, hex(call_via_base, 8).as_bytes());
    append_frame(&mut key, source_inputs);
    sha256::hex(&key)
}

fn rooted_path(path: &Path) -> PathBuf {
    if path.is_absolute() {
        path.to_path_buf()
    } else {
        root().join(path)
    }
}

fn compiler_include_dirs(commands: &[Vec<String>]) -> Vec<PathBuf> {
    let mut directories = BTreeSet::new();
    for command in commands {
        let mut arguments = command.iter();
        while let Some(argument) = arguments.next() {
            let include = if argument == "-I" {
                arguments.next().map(String::as_str)
            } else {
                argument.strip_prefix("-I").filter(|path| !path.is_empty())
            };
            if let Some(include) = include {
                directories.insert(rooted_path(Path::new(include)));
            }
        }
    }
    directories.into_iter().collect()
}

fn collect_source_inputs(
    path: &Path,
    include_dirs: &[PathBuf],
    seen: &mut BTreeSet<PathBuf>,
    inputs: &mut Vec<(PathBuf, Vec<u8>)>,
) -> Result<(), String> {
    let canonical = fs::canonicalize(path).map_err(|error| format!("{}: {error}", path.display()))?;
    if !seen.insert(canonical.clone()) {
        return Ok(());
    }
    let bytes = fs::read(&canonical).map_err(|error| format!("{}: {error}", canonical.display()))?;
    let text = String::from_utf8_lossy(&bytes);
    let mut includes = Vec::new();
    for include in text.lines().filter_map(quoted_include) {
        let include = Path::new(include);
        let local = canonical.parent().unwrap_or(Path::new("")).join(include);
        if let Some(resolved) = std::iter::once(local)
            .chain(include_dirs.iter().map(|directory| directory.join(include)))
            .find(|candidate| candidate.is_file())
        {
            includes.push(resolved);
        }
    }
    inputs.push((canonical, bytes));
    for include in includes {
        collect_source_inputs(&include, include_dirs, seen, inputs)?;
    }
    Ok(())
}

fn source_input_signature(source: &Path, commands: &[Vec<String>]) -> Result<Vec<u8>, String> {
    let mut seen = BTreeSet::new();
    let mut inputs = Vec::new();
    collect_source_inputs(source, &compiler_include_dirs(commands), &mut seen, &mut inputs)?;
    let mut stream = Vec::new();
    for (path, bytes) in inputs {
        append_frame(&mut stream, path.to_string_lossy().as_bytes());
        append_frame(&mut stream, &bytes);
    }
    Ok(sha256::hex(&stream).into_bytes())
}
fn definition_guard(name: &str) -> Regex {
    Regex::new(&format!(r"\b{name}\s*\([^;{{}}]*\)\s*\{{"), "")
}
fn c_identifier(text: &str) -> bool {
    let mut bytes = text.bytes();
    if !matches!(bytes.next(), Some(first) if first == b'_' || first.is_ascii_alphabetic()) {
        return false;
    }
    bytes.all(|byte| byte == b'_' || byte.is_ascii_alphanumeric())
}
fn aliases_for_symbol(text: &str, symbol: &str) -> Vec<String> {
    let mut aliases = Vec::new();
    for line in text.lines() {
        let fields: Vec<_> = line.split_ascii_whitespace().collect();
        if fields.len() != 3 || fields[0] != "#define" || fields[2] != symbol {
            continue;
        }
        let alias = fields[1];
        if c_identifier(alias) {
            aliases.push(alias.to_string());
        }
    }
    aliases
}
fn source_defines_symbol(text: &str, symbol: &str) -> bool {
    definition_guard(symbol).is_match(text)
        || aliases_for_symbol(text, symbol).iter().any(|alias| definition_guard(alias).is_match(text))
}
fn quoted_include(line: &str) -> Option<&str> {
    let rest = line.trim().strip_prefix("#include")?.trim();
    let rest = rest.strip_prefix('"')?;
    let end = rest.find('"')?;
    Some(&rest[..end])
}
fn source_defines_symbol_through_header(source: &Path, text: &str, symbol: &str) -> bool {
    if source_defines_symbol(text, symbol) {
        return true;
    }
    for include in text.lines().filter_map(quoted_include) {
        let local = source.parent().map(|parent| parent.join(include));
        let project = root().join("games/gs1/include").join(include);
        for header in local.into_iter().chain(std::iter::once(project)) {
            let Ok(header_text) = fs::read_to_string(header) else {
                continue;
            };
            if aliases_for_symbol(&header_text, symbol).iter().any(|alias| definition_guard(alias).is_match(text)) {
                return true;
            }
        }
    }
    false
}
pub fn compile_overlay_c(
    source: &Path,
    work: &Path,
    overlay: &str,
    routing_source: Option<&Path>,
    extra_flags: &[String],
) -> Result<Compiled, String> {
    let mutations = if extra_flags.is_empty() {
        None
    } else {
        Some(CompilerFlagMutations { add_flags: extra_flags.to_vec(), remove_flags: Vec::new() })
    };
    compile_overlay_with_mutations(source, work, overlay, routing_source, mutations.as_ref())
}
fn compile_overlay_with_mutations(
    source: &Path,
    work: &Path,
    overlay: &str,
    routing_source: Option<&Path>,
    mutations: Option<&CompilerFlagMutations>,
) -> Result<Compiled, String> {
    let extra_flags: Vec<String> = mutations
        .map(|m| {
            let mut all = m.add_flags.clone();
            all.extend(m.remove_flags.iter().cloned());
            all
        })
        .unwrap_or_default();
    let source_display = source.to_string_lossy().to_string();
    let source_paths = SourcePaths::load(&root())?;
    let routed_owner =
        routing_source.map(|path| source_paths.overlay_owner_for_path(overlay, path)).transpose()?.flatten();
    let (owner, stem, address) = match routed_owner {
        Some(owner) => (owner, owner.address_stem(), owner.address() as i64),
        None => address_stem(source, overlay)?,
    };
    let routing_source = match routing_source {
        Some(path) => source_paths
            .overlay_owner_for_path(overlay, path)?
            .map(SourceOwner::routing_path)
            .unwrap_or_else(|| path.to_path_buf()),
        None => owner.routing_path(),
    }
    .to_string_lossy()
    .into_owned();
    let call_via_base =
        source_paths.registered_call_via(owner).map(u64::from).unwrap_or_else(|| overlay_call_via_base(overlay)) as i64;
    let symbol = format!("Func_{}", stem.to_lowercase());
    let text = fs::read_to_string(source).map_err(|error| format!("{source_display}: {error}"))?;
    if !source_defines_symbol_through_header(source, &text, &symbol) {
        return Err(format!("overlay C source does not define {symbol}: {source_display}"));
    }
    let work_display = work.to_string_lossy().to_string();
    let at = |name: &str| work.join(name).to_string_lossy().to_string();
    let assembly = at(&format!("{stem}.s"));
    let object = at(&format!("{stem}.o"));
    let symbols_source = at(&format!("{stem}.symbols.s"));
    let symbols_object = at(&format!("{stem}.symbols.o"));
    let elf = at(&format!("{stem}.elf"));
    let binary = at(&format!("{stem}.bin"));
    let mut options =
        SourceToAssemblyPlanOptions::new(CompilerTarget::Gs1, routing_source, source_display.clone(), assembly.clone());
    options.preprocessed_output = Some(at(&format!("{stem}.i")));
    if let Some(requested) = mutations {
        options.flags = Some(requested.clone());
    }
    let plan = source_to_assembly_plan(&options).map_err(|error| error.to_string())?;
    let steps: Vec<Vec<String>> = plan.steps.iter().map(|step| step.command.clone()).collect();
    let source_inputs = source_input_signature(source, &steps)?;
    let plan_signature = plan_stamp(&steps, &work_display);
    let host_signature = compiler_core::bundle::host_executable_signature(&OVERLAY_HOST_TOOLS)
        .map_err(|error| format!("overlay host tool signature: {error}"))?;
    let cached = overlay_c_cache_dir().join(format!(
        "{}.bin",
        overlay_cache_key(
            &compiler_core::bundle::compiler_bundle_signature(),
            &host_signature,
            &plan_signature,
            address,
            call_via_base,
            &source_inputs,
        )
    ));
    if extra_flags.is_empty() {
        if let Some(data) = read_valid_cache_entry(&cached) {
            return Ok(Compiled { address, data });
        }
    }
    for step in &plan.steps {
        checked(&step.command, work)?;
    }
    let produced = fs::read_to_string(&assembly).map_err(|error| format!("{assembly}: {error}"))?;
    fs::write(&assembly, bias_in_image_label_words(&produced).text).map_err(|error| format!("{assembly}: {error}"))?;
    checked(&strings(&["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", &object, &assembly]), work)?;
    let undefined_symbols: Vec<String> = split_lines(&checked(&strings(&["arm-none-eabi-nm", "-u", &object]), work)?)
        .into_iter()
        .filter(|line| !line.is_empty())
        .map(|line| {
            crate::regex::js_trim(&line)
                .split(|c: char| crate::regex::is_js_space(c))
                .rfind(|part| !part.is_empty())
                .expect("a non-empty trimmed line has a last field")
                .to_string()
        })
        .collect();
    for external in &undefined_symbols {
        if external_symbol(external, call_via_base as u64).is_none() {
            return Err(format!("unsupported overlay C external symbol: {external}"));
        }
    }
    let mut stubs = String::from(".syntax unified\n.thumb\n");
    for name in &undefined_symbols {
        stubs.push_str(&external_symbol_assembly(name, call_via_base as u64)?);
    }
    fs::write(&symbols_source, stubs).map_err(|error| format!("{symbols_source}: {error}"))?;
    checked(
        &strings(&["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", &symbols_object, &symbols_source]),
        work,
    )?;
    checked(
        &strings(&[
            "arm-none-eabi-ld",
            &format!("-Ttext=0x{}", hex(address, 8)),
            "-e",
            &symbol,
            "-o",
            &elf,
            &object,
            &symbols_object,
        ]),
        work,
    )?;
    checked(&strings(&["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", &elf, &binary]), work)?;
    let listing = checked(&strings(&["arm-none-eabi-nm", "-S", &elf]), work)?;
    let needle = format!(" {symbol}");
    let row = split_lines(&listing)
        .into_iter()
        .find(|line| line.ends_with(&needle))
        .ok_or_else(|| format!("missing linked overlay C symbol: {symbol}"))?;
    let size_field = crate::regex::js_trim(&row)
        .split(|c: char| crate::regex::is_js_space(c))
        .filter(|part| !part.is_empty())
        .nth(1)
        .ok_or_else(|| format!("nm -S row has no size field: {row}"))?
        .to_string();
    let size = js_parse_int_hex(&size_field).ok_or_else(|| format!("nm -S size is not hex: {size_field}"))?;
    let whole = fs::read(&binary).map_err(|error| format!("{binary}: {error}"))?;
    let end = (size.max(0) as usize).min(whole.len());
    let data = whole[..end].to_vec();
    let _ = fs::create_dir_all(overlay_c_cache_dir());
    publish_cache_entry(&cached, &data);
    Ok(Compiled { address, data })
}
pub fn compile_overlay_candidate(
    source: &Path,
    work: &Path,
    overlay: &str,
    routing_source: Option<&Path>,
    extra_flags: &[String],
) -> Result<Compiled, String> {
    compile_overlay_c(source, work, overlay, routing_source, extra_flags)
}
pub fn compile_overlay_mutated(
    source: &Path,
    work: &Path,
    overlay: &str,
    routing_source: Option<&Path>,
    mutations: &CompilerFlagMutations,
) -> Result<Compiled, String> {
    compile_overlay_with_mutations(source, work, overlay, routing_source, Some(mutations))
}
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Span {
    pub start: i64,
    pub end: i64,
}
pub fn overlay_c_spans(source: &OverlaySource, base: i64) -> Result<Vec<Span>, String> {
    let display = source.to_display_string();
    let overlay = Regex::new(r"_overlay\.s$", "").replace_first(basename(&display), "");
    let mut spans = Vec::new();
    for c_source in overlay_c_sources(source)? {
        let work = tempdir().map_err(|error| error.to_string())?;
        let compiled = compile_overlay_c(&c_source, work.path(), &overlay, None, &[])?;
        let start = compiled.address - base;
        spans.push(Span { start, end: start + compiled.data.len() as i64 });
    }
    spans.sort_by_key(|span| span.start);
    Ok(spans)
}
pub fn assemble_overlay(source: &OverlaySource, base: i64) -> Result<Vec<u8>, String> {
    let work = tempdir().map_err(|error| error.to_string())?;
    let at = |name: &str| work.path().join(name).to_string_lossy().to_string();
    let assembly = at("o.s");
    let object = at("o.o");
    let elf = at("o.elf");
    let binary = at("o.bin");
    let text = source.read_text().map_err(|error| error.to_string())?;
    fs::write(&assembly, text).map_err(|error| error.to_string())?;
    spawn_raw(
        &strings(&["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", &object, &assembly]),
        work.path(),
    )?;
    spawn_raw(
        &strings(&["arm-none-eabi-ld", &format!("-Ttext=0x{}", hex(base, 8)), "-o", &elf, &object]),
        work.path(),
    )?;
    spawn_raw(&strings(&["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", &elf, &binary]), work.path())?;
    let mut result = fs::read(&binary).map_err(|error| error.to_string())?;
    let display = source.to_display_string();
    let overlay = Regex::new(r"_overlay\.s$", "").replace_first(basename(&display), "");
    let mut occupied: std::collections::BTreeSet<usize> = std::collections::BTreeSet::new();
    for c_source in overlay_c_sources_checked(source)? {
        let compiled = compile_overlay_c(&c_source, work.path(), &overlay, None, &[])
            .map_err(|cause| format!("{overlay}: {}: {cause}", basename(&c_source.to_string_lossy())))?;
        let offset = compiled.address - base;
        if offset < 0 || offset + compiled.data.len() as i64 > result.len() as i64 {
            return Err(format!("overlay C span is outside {display}: {}", c_source.display()));
        }
        let offset = offset as usize;
        for (index, existing) in result[offset..offset + compiled.data.len()].iter().enumerate() {
            let byte = offset + index;
            if occupied.contains(&byte) {
                return Err(format!("overlapping overlay C span: {}", c_source.display()));
            }
            occupied.insert(byte);
            if *existing != 0 {
                return Err(format!("overlay C placeholder is not zero at 0x{}", hex(base + byte as i64, 8)));
            }
        }
        result[offset..offset + compiled.data.len()].copy_from_slice(&compiled.data);
    }
    Ok(result)
}
pub(crate) fn strings(parts: &[&str]) -> Vec<String> {
    parts.iter().map(|part| (*part).to_string()).collect()
}
pub(crate) fn split_lines(text: &str) -> Vec<String> {
    let mut out = Vec::new();
    let mut current = String::new();
    let mut chars = text.chars().peekable();
    while let Some(c) = chars.next() {
        if c == '\r' && chars.peek() == Some(&'\n') {
            chars.next();
            out.push(std::mem::take(&mut current));
        } else if c == '\n' {
            out.push(std::mem::take(&mut current));
        } else {
            current.push(c);
        }
    }
    out.push(current);
    out
}

#[cfg(test)]
mod source_activation_tests {
    use super::*;
    use tempfile::tempdir;

    #[test]
    fn only_explicit_overlay_placeholders_activate_exact_c() {
        assert_eq!(
            placeholder_addresses("Func_02000104:\n  bx lr\nAlchemyC_02000104:\n  .space 8\nAlchemyC_02000314:\n"),
            vec![0x0200_0104, 0x0200_0314]
        );
        assert!(placeholder_addresses("Func_02000104:\n  bx lr\n").is_empty());
    }

    #[test]
    fn public_source_diagnostic_preserves_missing_source_errors() {
        let work = tempdir().unwrap();
        let assembly = work.path().join("resource_382_overlay.s");
        fs::write(&assembly, "AlchemyC_0200dead:\n  .space 4\n").unwrap();
        let error = overlay_c_sources(&OverlaySource::path(assembly)).unwrap_err();
        assert!(error.contains("resource_382:0200dead has an AlchemyC placeholder"));
    }

    #[test]
    fn included_header_can_name_an_overlay_owner() {
        let work = tempdir().unwrap();
        let source = work.path().join("spawn_effect.c");
        let header = work.path().join("effect.h");
        fs::write(&source, "#include \"effect.h\"\nvoid SceneEffect_Spawn(void) {}\n").unwrap();
        fs::write(header, "#define SceneEffect_Spawn Func_0200013c\n").unwrap();
        let text = fs::read_to_string(&source).unwrap();
        assert!(source_defines_symbol_through_header(&source, &text, "Func_0200013c"));
    }

    #[test]
    fn included_source_changes_are_part_of_the_overlay_cache_identity() {
        let work = tempdir().unwrap();
        let source = work.path().join("owner.c");
        let body = work.path().join("body.inc");
        fs::write(&source, "#include \"body.inc\"\n").unwrap();
        fs::write(&body, "void owner(void) {}\n").unwrap();
        let first = source_input_signature(&source, &[]).unwrap();
        fs::write(&body, "void owner(void) { for (;;) {} }\n").unwrap();
        let second = source_input_signature(&source, &[]).unwrap();
        assert_ne!(first, second);
    }
}
pub(crate) fn js_parse_int_hex(text: &str) -> Option<i64> {
    let body = crate::regex::js_trim(text);
    let (negative, body) = match body.strip_prefix('-') {
        Some(rest) => (true, rest),
        None => (false, body.strip_prefix('+').unwrap_or(body)),
    };
    let body = body.strip_prefix("0x").or_else(|| body.strip_prefix("0X")).unwrap_or(body);
    let digits: String = body.chars().take_while(|c| c.is_ascii_hexdigit()).collect();
    if digits.is_empty() {
        return None;
    }
    let value = i64::from_str_radix(&digits, 16).ok()?;
    Some(if negative { -value } else { value })
}
