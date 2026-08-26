use crate::paths::{basename, root, OverlaySource};
use crate::regex::Regex;
use compiler_core::plan::{
    source_to_assembly_plan, CompilerFlagMutations, SourceToAssemblyPlanOptions,
};
use compiler_core::routing::CompilerTarget;
use compiler_core::sha256;
use compiler_core::source_inputs::compiler_source_tree_signature;
use compiler_core::source_paths::{SourceOwner, SourcePaths};
use compiler_core::translation_units::{
    AbsoluteSymbol, AbsoluteSymbolKind, TranslationUnit, TranslationUnits,
};
use compiler_core::{external_symbol_assembly, overlay_call_via_base};
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::{Mutex, OnceLock};
use tempfile::tempdir;
pub fn hex(value: i64, width: usize) -> String {
    if value < 0 {
        return format!("-{:0width$x}", value.unsigned_abs());
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
    BiasResult {
        text: out.join("\n"),
        biased,
    }
}
pub fn overlay_c_cache_dir() -> PathBuf {
    match std::env::var_os("ALCHEMY_OVERLAY_C_CACHE") {
        Some(value) => PathBuf::from(value),
        None => root().join("out/cache/overlay-c"),
    }
}
fn translation_units() -> Result<&'static TranslationUnits, String> {
    static UNITS: OnceLock<Result<TranslationUnits, String>> = OnceLock::new();
    match UNITS.get_or_init(|| TranslationUnits::load(&root())) {
        Ok(units) => Ok(units),
        Err(error) => Err(error.clone()),
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
    assert!(
        !stream.is_empty(),
        "overlay_disasm read an EMPTY source; refusing to key the cache"
    );
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
    guard
        .get_or_insert_with(BTreeMap::new)
        .insert(identity, stamp.clone());
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
            let normalized = if part.starts_with(work) {
                "<work>"
            } else {
                part
            };
            append_frame(&mut identity, normalized.as_bytes());
        }
    }
    identity
}
fn placeholder_addresses(assembly: &str) -> Vec<u32> {
    assembly
        .lines()
        .filter_map(|line| {
            line.trim()
                .strip_prefix("AlchemyC_")?
                .strip_suffix(':')
                .and_then(|address| u32::from_str_radix(address, 16).ok())
        })
        .collect::<BTreeSet<_>>()
        .into_iter()
        .collect()
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
    let tail = stem
        .rsplit_once("_c_")
        .map(|(_, tail)| tail)
        .unwrap_or(stem);
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
        .unwrap_or(SourceOwner::Overlay {
            resource,
            address: address as u32,
        });
    Ok((owner, tail.to_ascii_lowercase(), address))
}
fn checked(command: &[String], cwd: &Path) -> Result<String, String> {
    let (binary, rest) = command.split_first().ok_or("empty command")?;
    let output = Command::new(binary)
        .args(rest)
        .current_dir(cwd)
        .output()
        .map_err(|error| format!("{} failed: {error}", basename(binary)))?;
    let stdout = String::from_utf8_lossy(&output.stdout).to_string();
    if !output.status.success() {
        let stderr = String::from_utf8_lossy(&output.stderr).to_string();
        let detail = crate::regex::js_trim(if stderr.is_empty() { &stdout } else { &stderr });
        return Err(if detail.is_empty() {
            format!("{} failed", basename(binary))
        } else {
            format!("{} failed: {detail}", basename(binary))
        });
    }
    Ok(stdout)
}
fn assemble_file(source: &str, object: &str, work: &Path) -> Result<(), String> {
    checked(
        &strings(&[
            "arm-none-eabi-as",
            "-mcpu=arm7tdmi",
            "-mthumb-interwork",
            "-o",
            object,
            source,
        ]),
        work,
    )
    .map(drop)
}
fn copy_text(elf: &str, binary: &str, work: &Path) -> Result<(), String> {
    checked(
        &strings(&[
            "arm-none-eabi-objcopy",
            "-O",
            "binary",
            "-j",
            ".text",
            elf,
            binary,
        ]),
        work,
    )
    .map(drop)
}
pub(crate) fn spawn_raw(command: &[String], cwd: &Path) -> Result<Vec<u8>, String> {
    let (binary, rest) = command
        .split_first()
        .ok_or_else(|| "empty command".to_string())?;
    let output = Command::new(binary)
        .args(rest)
        .current_dir(cwd)
        .output()
        .map_err(|error| format!("{binary}: {error}"))?;
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
const OVERLAY_HOST_TOOLS: [&str; 4] = [
    "arm-none-eabi-as",
    "arm-none-eabi-nm",
    "arm-none-eabi-ld",
    "arm-none-eabi-objcopy",
];
fn cache_record_path(cached: &Path) -> PathBuf {
    cached.with_extension("record")
}
fn cache_record(data: &[u8]) -> String {
    format!(
        "overlay-c-cache-v1\n{}\n{}\n",
        data.len(),
        sha256::hex(data)
    )
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
    if lines.next().is_some()
        || size != data.len()
        || digest.len() != 64
        || digest != sha256::hex(&data)
    {
        return None;
    }
    Some(data)
}
fn publish_cache_entry(cached: &Path, data: &[u8]) {
    if cache_entry::write_cache_entry_atomically(cached, data).is_ok() {
        let _ = cache_entry::write_cache_entry_atomically(
            &cache_record_path(cached),
            cache_record(data).as_bytes(),
        );
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
fn absolute_symbol_assembly(name: &str, symbol: AbsoluteSymbol) -> String {
    let directive = [".set", ".thumb_set"][(symbol.kind == AbsoluteSymbolKind::Thumb) as usize];
    format!(
        ".global {name}\n{directive} {name}, 0x{:08x}\n",
        symbol.address
    )
}
fn overlay_external_assembly(
    name: &str,
    unit: Option<&TranslationUnit>,
    call_via_base: u64,
) -> Result<String, String> {
    let symbol = unit.and_then(|unit| unit.absolute_symbols.get(name).copied());
    if let Some(symbol) = symbol {
        return Ok(absolute_symbol_assembly(name, symbol));
    }
    external_symbol_assembly(name, call_via_base)
}
fn translation_unit_signature() -> Result<Vec<u8>, String> {
    fs::read(root().join("games/gs1/recon/translation-units.json"))
        .map_err(|error| error.to_string())
}
fn link_object(
    files: [&str; 5],
    work: &Path,
    address: i64,
    entry: Option<&str>,
    unit: Option<&TranslationUnit>,
    names: &SourcePaths,
    call_via: u64,
) -> Result<Vec<u8>, String> {
    let [object, symbols_source, symbols_object, elf, binary] = files;
    let undefined = checked(&strings(&["arm-none-eabi-nm", "-u", object]), work)?;
    let mut stubs = names.main_symbol_exports();
    for name in undefined
        .lines()
        .filter_map(|line| line.split_whitespace().last())
    {
        if names.main_symbol(name)?.is_some() {
            continue;
        }
        stubs.push_str(
            &overlay_external_assembly(name, unit, call_via)
                .map_err(|_| format!("unsupported overlay C external symbol: {name}"))?,
        );
    }
    fs::write(symbols_source, stubs).map_err(|error| format!("{symbols_source}: {error}"))?;
    assemble_file(symbols_source, symbols_object, work)?;
    let mut command = strings(&["arm-none-eabi-ld", &format!("-Ttext=0x{}", hex(address, 8))]);
    if let Some(entry) = entry {
        command.extend(strings(&["-e", entry]));
    }
    command.extend(strings(&["-o", elf, object, symbols_object]));
    checked(&command, work)?;
    copy_text(elf, binary, work)?;
    fs::read(binary).map_err(|error| format!("{binary}: {error}"))
}
pub fn compile_overlay_c(
    source: &Path,
    work: &Path,
    overlay: &str,
    routing_source: Option<&Path>,
    extra_flags: &[String],
) -> Result<Compiled, String> {
    let mutations = (!extra_flags.is_empty()).then(|| CompilerFlagMutations {
        add_flags: extra_flags.to_vec(),
        remove_flags: Vec::new(),
    });
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
    let routed_owner = routing_source
        .map(|path| source_paths.overlay_owner_for_path(overlay, path))
        .transpose()?
        .flatten();
    let (owner, stem, address) = match routed_owner {
        Some(owner) => (owner, owner.address_stem(), owner.address() as i64),
        None => address_stem(source, overlay)?,
    };
    let units = translation_units()?;
    let unit = units.unit_for_game_owner("gs1", owner);
    let routing_source = match routing_source {
        Some(path) => source_paths
            .overlay_owner_for_path(overlay, path)?
            .map(SourceOwner::routing_path)
            .unwrap_or_else(|| path.to_path_buf()),
        None => owner.routing_path(),
    }
    .to_string_lossy()
    .into_owned();
    let call_via_base = source_paths
        .registered_call_via(owner)
        .map(u64::from)
        .unwrap_or_else(|| overlay_call_via_base(overlay)) as i64;
    let symbol = format!("Func_{}", stem.to_lowercase());
    let work_display = work.to_string_lossy().to_string();
    let at = |name: &str| work.join(name).to_string_lossy().to_string();
    let assembly = at(&format!("{stem}.s"));
    let object = at(&format!("{stem}.o"));
    let symbols_source = at(&format!("{stem}.symbols.s"));
    let symbols_object = at(&format!("{stem}.symbols.o"));
    let elf = at(&format!("{stem}.elf"));
    let binary = at(&format!("{stem}.bin"));
    let mut options = SourceToAssemblyPlanOptions::new(
        CompilerTarget::Gs1,
        routing_source,
        source_display.clone(),
        assembly.clone(),
    );
    options.preprocessed_output = Some(at(&format!("{stem}.i")));
    if let Some(requested) = mutations {
        options.flags = Some(requested.clone());
    }
    let plan = source_to_assembly_plan(&options).map_err(|error| error.to_string())?;
    let steps: Vec<Vec<String>> = plan.steps.iter().map(|step| step.command.clone()).collect();
    let mut source_inputs = compiler_source_tree_signature(&root(), source, &steps)?;
    append_frame(&mut source_inputs, &translation_unit_signature()?);
    append_frame(
        &mut source_inputs,
        &fs::read(source_paths.manifest_path()).map_err(|error| error.to_string())?,
    );
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
    fs::write(&assembly, bias_in_image_label_words(&produced).text)
        .map_err(|error| format!("{assembly}: {error}"))?;
    assemble_file(&assembly, &object, work)?;
    let object_listing = checked(&strings(&["arm-none-eabi-nm", "-S", &object]), work)?;
    let (object_offset, object_size) = symbol_span(&object_listing, &symbol)?;
    let link_address = address
        .checked_sub(object_offset as i64)
        .ok_or_else(|| format!("overlay owner {symbol} precedes its translation unit"))?;
    let whole = link_object(
        [&object, &symbols_source, &symbols_object, &elf, &binary],
        work,
        link_address,
        Some(&symbol),
        unit,
        &source_paths,
        call_via_base as u64,
    )?;
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
    let size = js_parse_int_hex(&size_field)
        .ok_or_else(|| format!("nm -S size is not hex: {size_field}"))?;
    if size as usize != object_size {
        return Err(format!("linked overlay C symbol size changed: {symbol}"));
    }
    let end = object_offset + size.max(0) as usize;
    let data = whole
        .get(object_offset..end)
        .ok_or_else(|| format!("linked overlay C symbol is truncated: {symbol}"))?
        .to_vec();
    let _ = fs::create_dir_all(overlay_c_cache_dir());
    publish_cache_entry(&cached, &data);
    Ok(Compiled { address, data })
}
fn symbol_span(listing: &str, name: &str) -> Result<(usize, usize), String> {
    let fields = listing
        .lines()
        .map(|line| line.split_whitespace().collect::<Vec<_>>())
        .find(|fields| fields.last() == Some(&name))
        .ok_or_else(|| format!("missing translation-unit symbol {name}"))?;
    let value = |index: usize| {
        fields
            .get(index)
            .and_then(|field| usize::from_str_radix(field, 16).ok())
            .ok_or_else(|| format!("invalid translation-unit symbol {name}"))
    };
    Ok((value(0)?, value(1)?))
}
fn compile_overlay_unit(
    unit: &TranslationUnit,
    work: &Path,
    overlay: &str,
    edition: Option<&str>,
) -> Result<Compiled, String> {
    let names = SourcePaths::load(&root())?;
    let source = root().join(&unit.source);
    let first = unit
        .owners
        .first()
        .ok_or("empty overlay translation unit")?;
    let base = first.address as i64;
    let call_via = unit
        .owners
        .iter()
        .map(|member| {
            unit.source_owner(member.address).map(|owner| {
                names
                    .registered_call_via(owner)
                    .map(u64::from)
                    .unwrap_or_else(|| overlay_call_via_base(overlay))
            })
        })
        .collect::<Result<BTreeSet<_>, _>>()?;
    if call_via.len() != 1 {
        return Err(format!(
            "{}: grouped owners disagree on call-via bank",
            unit.id
        ));
    }
    let call_via = *call_via.first().unwrap();
    let at = |suffix: &str| {
        work.join(format!("{}.{suffix}", unit.id))
            .to_string_lossy()
            .into_owned()
    };
    let [assembly, object, symbols_source, symbols_object, elf, binary] =
        ["s", "o", "symbols.s", "symbols.o", "elf", "bin"].map(at);
    let mut options = SourceToAssemblyPlanOptions::new(
        CompilerTarget::Gs1,
        unit.source_owner(first.address)?
            .routing_path()
            .to_string_lossy(),
        source.to_string_lossy(),
        assembly.clone(),
    );
    options.preprocessed_output = Some(at("i"));
    if let Some(edition) = edition {
        options
            .preprocessor_flags
            .push(format!("-DGS1_EDITION_{}=1", edition.to_ascii_uppercase()));
    }
    for step in source_to_assembly_plan(&options)?.steps {
        checked(&step.command, work)?;
    }
    let produced = fs::read_to_string(&assembly).map_err(|error| error.to_string())?;
    fs::write(&assembly, bias_in_image_label_words(&produced).text)
        .map_err(|error| error.to_string())?;
    assemble_file(&assembly, &object, work)?;
    let listing = checked(
        &strings(&["arm-none-eabi-nm", "-S", "--defined-only", &object]),
        work,
    )?;
    let mut members = unit.symbols().collect::<Vec<_>>();
    members.sort_by_key(|member| member.0);
    let mut cursor = first.address;
    for (address, _, extent) in &members {
        if *address != cursor {
            return Err(format!("{}: grouped text has an undeclared gap", unit.id));
        }
        let symbol = unit.source_owner(*address)?.legacy_name();
        let offset = address
            .checked_sub(first.address)
            .ok_or_else(|| format!("{}: symbol precedes first owner", unit.id))?;
        let expected = (offset as usize, *extent);
        if symbol_span(&listing, &symbol)? != expected {
            return Err(format!("{}: {symbol} offset or extent differs", unit.id));
        }
        cursor = *address + *extent as u32;
    }
    let whole = link_object(
        [&object, &symbols_source, &symbols_object, &elf, &binary],
        work,
        base,
        None,
        Some(unit),
        &names,
        call_via,
    )?;
    if whole.len() != (cursor - first.address) as usize {
        return Err(format!("{}: grouped text extent differs", unit.id));
    }
    Ok(Compiled {
        address: base,
        data: whole,
    })
}
pub fn compile_declared_overlay_unit(
    unit: &TranslationUnit,
    edition: &str,
) -> Result<Compiled, String> {
    if !unit.exact() || unit.overlay.is_none() {
        return Err(format!("{}: not a wholly exact overlay unit", unit.id));
    }
    let work = tempdir().map_err(|error| error.to_string())?;
    compile_overlay_unit(
        unit,
        work.path(),
        unit.overlay.as_deref().unwrap(),
        Some(edition),
    )
}
fn validate_shared_overlay_source(
    repository: &Path,
    names: &SourcePaths,
    units: &[TranslationUnit],
    overlay: &str,
    path: &Path,
) -> Result<(), String> {
    let owners = names
        .owners_for_path(path)
        .into_iter()
        .filter(|owner| owner.overlay_id().as_deref() == Some(overlay))
        .map(|owner| owner.address())
        .collect::<BTreeSet<_>>();
    if owners.len() <= 1 {
        return Ok(());
    }
    let covering = units
        .iter()
        .filter(|unit| {
            unit.overlay.as_deref() == Some(overlay)
                && unit.exact()
                && repository.join(&unit.source) == path
                && unit
                    .owners
                    .iter()
                    .map(|owner| owner.address)
                    .collect::<BTreeSet<_>>()
                    == owners
        })
        .count();
    if covering == 1 {
        return Ok(());
    }
    Err(format!(
        "{} maps to multiple {overlay} owners without one wholly exact translation unit",
        path.display()
    ))
}
fn compile_production_overlay(
    source: &OverlaySource,
    work: &Path,
    overlay: &str,
) -> Result<Vec<Compiled>, String> {
    let text = source.read_text().map_err(|error| error.to_string())?;
    let placeholders = placeholder_addresses(&text)
        .into_iter()
        .collect::<BTreeSet<_>>();
    let names = SourcePaths::load(&root())?;
    let units = translation_units()?;
    let mut paths = BTreeSet::new();
    for address in &placeholders {
        let owner = SourceOwner::parse(&format!("{overlay}:{address:08x}"))?;
        paths.insert(names.source_path(owner));
    }
    for path in paths {
        validate_shared_overlay_source(&root(), &names, &units.units, overlay, &path)?;
    }
    let mut handled = BTreeSet::new();
    let mut compiled = Vec::new();
    for unit in units.units.iter().filter(|unit| {
        unit.overlay.as_deref() == Some(overlay)
            && unit.exact()
            && root().join(&unit.source).starts_with(names.source_root())
    }) {
        for (address, _, _) in unit.symbols() {
            if !placeholders.contains(&address) || !handled.insert(address) {
                return Err(format!(
                    "{}: undeclared or duplicate grouped owner",
                    unit.id
                ));
            }
        }
        compiled.push(compile_overlay_unit(unit, work, overlay, None)?);
    }
    for address in placeholders.difference(&handled) {
        let owner = SourceOwner::parse(&format!("{overlay}:{address:08x}"))?;
        let path = names.source_path(owner);
        if !path.is_file() {
            return Err(format!(
                "{} has an AlchemyC placeholder but no exact C source at {}",
                owner.id(),
                path.display()
            ));
        }
        compiled.push(compile_overlay_c(&path, work, overlay, None, &[])?);
    }
    compiled.sort_by_key(|member| member.address);
    Ok(compiled)
}
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Span {
    pub start: i64,
    pub end: i64,
}
pub fn overlay_c_spans(source: &OverlaySource, base: i64) -> Result<Vec<Span>, String> {
    let display = source.to_display_string();
    let overlay = Regex::new(r"_overlay\.s$", "").replace_first(basename(&display), "");
    let work = tempdir().map_err(|error| error.to_string())?;
    compile_production_overlay(source, work.path(), &overlay).map(|members| {
        members
            .into_iter()
            .map(|member| Span {
                start: member.address - base,
                end: member.address - base + member.data.len() as i64,
            })
            .collect()
    })
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
        &strings(&[
            "arm-none-eabi-as",
            "-mcpu=arm7tdmi",
            "-mthumb-interwork",
            "-o",
            &object,
            &assembly,
        ]),
        work.path(),
    )?;
    spawn_raw(
        &strings(&[
            "arm-none-eabi-ld",
            &format!("-Ttext=0x{}", hex(base, 8)),
            "-o",
            &elf,
            &object,
        ]),
        work.path(),
    )?;
    spawn_raw(
        &strings(&[
            "arm-none-eabi-objcopy",
            "-O",
            "binary",
            "-j",
            ".text",
            &elf,
            &binary,
        ]),
        work.path(),
    )?;
    let mut result = fs::read(&binary).map_err(|error| error.to_string())?;
    let display = source.to_display_string();
    let overlay = Regex::new(r"_overlay\.s$", "").replace_first(basename(&display), "");
    let mut occupied: std::collections::BTreeSet<usize> = std::collections::BTreeSet::new();
    for compiled in compile_production_overlay(source, work.path(), &overlay)? {
        let offset = compiled.address - base;
        if offset < 0 || offset + compiled.data.len() as i64 > result.len() as i64 {
            return Err(format!(
                "overlay C span is outside {display}: {}",
                hex(compiled.address, 8)
            ));
        }
        let offset = offset as usize;
        for (index, existing) in result[offset..offset + compiled.data.len()]
            .iter()
            .enumerate()
        {
            let byte = offset + index;
            if occupied.contains(&byte) {
                return Err(format!(
                    "overlapping overlay C span: {}",
                    hex(compiled.address, 8)
                ));
            }
            occupied.insert(byte);
            if *existing != 0 {
                return Err(format!(
                    "overlay C placeholder is not zero at 0x{}",
                    hex(base + byte as i64, 8)
                ));
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
    text.lines().map(str::to_owned).collect()
}
#[cfg(test)]
mod source_activation_tests {
    use super::*;
    use compiler_core::translation_units::{OwnerState, TranslationOwner};
    use tempfile::tempdir;
    #[test]
    fn only_explicit_overlay_placeholders_activate_exact_c() {
        assert_eq!(
            placeholder_addresses(
                "Func_02000104:\n  bx lr\nAlchemyC_02000104:\n  .space 8\nAlchemyC_02000314:\n"
            ),
            vec![0x0200_0104, 0x0200_0314]
        );
        assert!(placeholder_addresses("Func_02000104:\n  bx lr\n").is_empty());
    }
    #[test]
    fn public_source_diagnostic_preserves_missing_source_errors() {
        let work = tempdir().unwrap();
        let assembly = work.path().join("resource_382_overlay.s");
        fs::write(&assembly, "AlchemyC_0200dead:\n  .space 4\n").unwrap();
        let error = overlay_c_spans(&OverlaySource::path(assembly), 0x0200_0000).unwrap_err();
        assert!(error.contains("resource_382:0200dead has an AlchemyC placeholder"));
    }
    #[test]
    fn shared_overlay_source_requires_one_wholly_exact_unit() {
        let root = tempdir().unwrap();
        let names = SourcePaths::parse(
            root.path(),
            r#"{"format":3,"owners":{
                "resource_382:02000100":"overlays/shared.c",
                "resource_382:02000104":"overlays/shared.c"}}"#,
        )
        .unwrap();
        let path = names.source_path(SourceOwner::parse("resource_382:02000100").unwrap());
        let owner = |address| TranslationOwner {
            address,
            alias: format!("Owner_{address:08x}"),
            extent: 4,
            state: OwnerState::ExactC,
        };
        let mut unit = TranslationUnit {
            id: "shared".into(),
            game: "gs1".into(),
            source: "games/gs1/src/overlays/shared.c".into(),
            compiler_route: "canonical-gcc296".into(),
            overlay: Some("resource_382".into()),
            absolute_symbols: BTreeMap::new(),
            local_symbols: Vec::new(),
            composition_sections: BTreeMap::new(),
            owners: vec![owner(0x0200_0100), owner(0x0200_0104)],
        };
        let check = |units: &[TranslationUnit]| {
            validate_shared_overlay_source(root.path(), &names, units, "resource_382", &path)
        };
        assert!(check(&[]).is_err());
        assert!(check(std::slice::from_ref(&unit)).is_ok());
        unit.owners[1].state = OwnerState::RetainedAssembly;
        assert!(check(&[unit]).is_err());
    }
    #[test]
    fn semantic_main_alias_is_in_the_generated_export_graph() {
        let names = SourcePaths::load(&root()).unwrap();
        let binding = ".thumb_set RunBattleEffect16, 0x0809b698\n";
        assert!(binding.contains("0x0809b698"));
        assert!(names.main_symbol_exports().contains(&binding));
    }
}
pub(crate) fn js_parse_int_hex(text: &str) -> Option<i64> {
    let body = crate::regex::js_trim(text);
    let (negative, body) = match body.strip_prefix('-') {
        Some(rest) => (true, rest),
        None => (false, body.strip_prefix('+').unwrap_or(body)),
    };
    let body = body
        .strip_prefix("0x")
        .or_else(|| body.strip_prefix("0X"))
        .unwrap_or(body);
    let digits: String = body.chars().take_while(|c| c.is_ascii_hexdigit()).collect();
    if digits.is_empty() {
        return None;
    }
    let value = i64::from_str_radix(&digits, 16).ok()?;
    Some(if negative { -value } else { value })
}
