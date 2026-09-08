use crate::overlay::source::OverlaySource;
use compiler_core::overlay;
use compiler_core::overlay::placeholder_addresses;
pub use compiler_core::overlay::placeholder_extent;
use compiler_core::plan::{source_to_assembly_plan, SourceToAssemblyPlanOptions};
use compiler_core::routing::root;
use compiler_core::routing::CompilerTarget;
use compiler_core::sha256;
use compiler_core::source_inputs::compiler_source_tree_signature;
use compiler_core::source_paths::{SourceOwner, SourcePaths};
use compiler_core::translation_units::{
    AbsoluteSymbol, AbsoluteSymbolKind, TranslationUnit, TranslationUnits,
};
use psynergy::process::run as checked;
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
fn overlay_c_cache_path() -> PathBuf {
    match std::env::var_os("ALCHEMY_OVERLAY_C_CACHE") {
        Some(value) => PathBuf::from(value),
        None => root().join("out/cache/overlay-c.sqlite3"),
    }
}
fn overlay_c_cache() -> Result<&'static Mutex<compiler_core::cache::sqlite::SqliteCache>, String> {
    static CACHE: OnceLock<Result<Mutex<compiler_core::cache::sqlite::SqliteCache>, String>> =
        OnceLock::new();
    match CACHE.get_or_init(|| {
        compiler_core::cache::sqlite::SqliteCache::open(&overlay_c_cache_path()).map(Mutex::new)
    }) {
        Ok(cache) => Ok(cache),
        Err(error) => Err(error.clone()),
    }
}
fn translation_units() -> Result<&'static TranslationUnits, String> {
    static UNITS: OnceLock<Result<TranslationUnits, String>> = OnceLock::new();
    match UNITS.get_or_init(|| TranslationUnits::load(&root())) {
        Ok(units) => Ok(units),
        Err(error) => Err(error.clone()),
    }
}
fn write_overlay_bindings(overlay: &str, text: &str) -> Result<PathBuf, String> {
    let directory = root().join("out/overlay-bindings");
    fs::create_dir_all(&directory).map_err(|error| format!("{}: {error}", directory.display()))?;
    let path = directory.join(format!(
        "{overlay}-{}.h",
        &sha256::hex(text.as_bytes())[..16]
    ));
    if !fs::read(&path).is_ok_and(|bytes| bytes == text.as_bytes()) {
        compiler_core::cache::write_cache_entry_atomically(&path, text.as_bytes())
            .map_err(|error| format!("{}: {error}", path.display()))?;
    }
    Ok(path)
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
fn assemble_file(source: &str, object: &str, work: &Path) -> Result<(), String> {
    checked(
        &compiler_core::routing::assembly_command(source, object),
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
        return Err(String::from_utf8_lossy(&output.stderr).trim().to_string());
    }
    Ok(output.stdout)
}
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Compiled {
    pub address: i64,
    pub data: Vec<u8>,
}
const OVERLAY_HOST_TOOLS: [&str; 5] = [
    "arm-none-eabi-as",
    "arm-none-eabi-nm",
    "arm-none-eabi-ld",
    "arm-none-eabi-objcopy",
    "arm-none-eabi-objdump",
];
fn overlay_cache_key(
    compiler_signature: &str,
    host_signature: &str,
    plan_signature: &str,
    address: i64,
    source_inputs: &[u8],
) -> Result<String, String> {
    let identity = (
        "overlay-c-cache-v5",
        compiler_core::bundle::executable_signature()?,
        compiler_signature,
        host_signature,
        plan_signature,
        address,
        sha256::hex(source_inputs),
    );
    Ok(sha256::hex(
        &serde_json::to_vec(&identity).map_err(|error| error.to_string())?,
    ))
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
    is_call: bool,
    reference: &[u8],
    calls: &BTreeMap<String, BTreeSet<u64>>,
) -> Result<String, String> {
    let symbol = unit.and_then(|unit| unit.absolute_symbols.get(name).copied());
    if let Some(symbol) = symbol {
        return Ok(absolute_symbol_assembly(name, symbol));
    }
    overlay::external(name, is_call, reference, calls)
        .map(|symbol| absolute_symbol_assembly(name, symbol))
}

fn call_relocations(object: &str, work: &Path) -> Result<BTreeSet<String>, String> {
    Ok(
        checked(&strings(&["arm-none-eabi-objdump", "-r", object]), work)?
            .lines()
            .filter_map(|line| {
                let fields = line.split_whitespace().collect::<Vec<_>>();
                (fields.get(1) == Some(&"R_ARM_THM_CALL"))
                    .then(|| fields.get(2).map(|name| name.to_string()))
                    .flatten()
            })
            .collect(),
    )
}
fn translation_unit_signature() -> Result<Vec<u8>, String> {
    fs::read(root().join("games/gs1/recon/translation-units.json"))
        .map_err(|error| error.to_string())
}
pub fn compile_overlay_c(
    source: &Path,
    work: &Path,
    overlay: &str,
    extent: usize,
    routing_source: Option<&Path>,
    extra_flags: &[String],
) -> Result<Compiled, String> {
    let source_display = source.to_string_lossy().to_string();
    let source_paths = SourcePaths::load(&root())?;
    let route = routing_source.unwrap_or(source);
    let owner = source_paths
        .overlay_owner_for_path(overlay, route)?
        .or_else(|| {
            SourceOwner::from_legacy_stem(&route.file_stem()?.to_str()?.to_ascii_lowercase())
        })
        .ok_or_else(|| {
            format!(
                "{} has no overlay owner; supply a registered route",
                route.display()
            )
        })?;
    let owner = if owner.is_main() {
        SourceOwner::parse(&format!("{overlay}:{}", owner.address_stem()))?
    } else {
        owner
    };
    if owner.overlay_id().as_deref() != Some(overlay) {
        return Err(format!("{} does not belong to {overlay}", owner.id()));
    }
    let (stem, address) = (owner.address_stem(), i64::from(owner.address()));
    let units = translation_units()?;
    let unit = units.unit_for_game_owner("gs1", owner);
    let reference = crate::overlay::rom::canonical_overlay(&root(), overlay)?;
    let routing_source = owner.routing_path().to_string_lossy().into_owned();
    let work_display = work.to_string_lossy().to_string();
    let at = |name: &str| work.join(name).to_string_lossy().to_string();
    let assembly = at(&format!("{stem}.s"));
    let mut options = SourceToAssemblyPlanOptions::new(
        CompilerTarget::Gs1,
        routing_source.clone(),
        source_display.clone(),
        assembly.clone(),
    );
    options.preprocessed_output = Some(at(&format!("{stem}.i")));
    let binding_text = source_paths.symbol_bindings(Some(overlay));
    let bindings = write_overlay_bindings(overlay, &binding_text)?;
    options.preprocessor_flags = vec!["-include".into(), bindings.to_string_lossy().into_owned()];
    options.support_flags = extra_flags.to_vec();
    let plan = source_to_assembly_plan(&options).map_err(|error| error.to_string())?;
    let steps: Vec<Vec<String>> = plan.steps.iter().map(|step| step.command.clone()).collect();
    let configuration = crate::candidate::CandidateCompilerConfiguration {
        overlay_extent: Some(extent),
        absolute_symbols: unit
            .map(TranslationUnit::canonical_symbols)
            .transpose()?
            .unwrap_or_default(),
        ..Default::default()
    };
    let mut source_inputs = compiler_source_tree_signature(&root(), source, &steps)?;
    append_frame(&mut source_inputs, &translation_unit_signature()?);
    append_frame(
        &mut source_inputs,
        &fs::read(source_paths.manifest_path()).map_err(|error| error.to_string())?,
    );
    // The binding path carries a content hash, so a rename changes the
    // command; hashing the text keeps the key honest if that ever changes.
    append_frame(&mut source_inputs, binding_text.as_bytes());
    append_frame(&mut source_inputs, &reference);
    append_frame(&mut source_inputs, &extent.to_le_bytes());
    let plan_signature = sha256::hex(&command_identity(&steps, &work_display));
    let host_signature = compiler_core::bundle::host_executable_signature(&OVERLAY_HOST_TOOLS)
        .map_err(|error| format!("overlay host tool signature: {error}"))?;
    let cache_key = overlay_cache_key(
        &compiler_core::bundle::compiler_bundle_signature(),
        &host_signature,
        &plan_signature,
        address,
        &source_inputs,
    )?;
    // Compiles with local includes or diagnostic dumps are throwaway by
    // construction and must never be persisted: every candidate has unique
    // source, so caching them would grow the database without bound.
    if extra_flags.is_empty() {
        if let Ok(cache) = overlay_c_cache() {
            let hit = cache
                .lock()
                .ok()
                .and_then(|cache| cache.get(&cache_key).ok().flatten())
                .and_then(|entries| entries.into_iter().find(|(kind, _)| kind == "payload"));
            if let Some((_, data)) = hit {
                return Ok(Compiled { address, data });
            }
        }
    }
    let data = crate::candidate::verify_candidate_owned_routed(
        &source_display,
        &routing_source,
        &stem,
        &reference,
        &work_display,
        extra_flags,
        f64::from(overlay::RESOURCE_BASE),
        CompilerTarget::Gs1,
        &configuration,
    )?
    .actual;
    // Mirror the read-side guard above: never persist a flag-mutated compile.
    if extra_flags.is_empty() {
        if let Ok(cache) = overlay_c_cache() {
            if let Ok(cache) = cache.lock() {
                let _ = cache.put(&cache_key, &[("payload", &data)]);
            }
        }
    }
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
    placement: Option<&OverlayEditionPlacement<'_>>,
) -> Result<Vec<Compiled>, String> {
    let names = SourcePaths::load(&root())?;
    let source = root().join(&unit.source);
    let first = unit
        .owners
        .first()
        .ok_or("empty overlay translation unit")?;
    let at = |suffix: &str| {
        work.join(format!("{}.{suffix}", unit.id))
            .to_string_lossy()
            .into_owned()
    };
    let [assembly, object, symbols_source, symbols_object, elf] =
        ["s", "o", "symbols.s", "symbols.o", "elf"].map(at);
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
    let binding_text = names.symbol_bindings(Some(overlay));
    let bindings = write_overlay_bindings(overlay, &binding_text)?;
    options
        .preprocessor_flags
        .extend(["-include".into(), bindings.to_string_lossy().into_owned()]);
    for step in source_to_assembly_plan(&options)?.steps {
        checked(&step.command, work)?;
    }
    let produced = fs::read_to_string(&assembly).map_err(|error| error.to_string())?;
    // The unit's functions need not be contiguous in the image: retained
    // assembly, tables, and pools sit between them. Each function goes into
    // its own section and one link places every section at its owner's
    // address, so intra-unit calls and pool words see the real layout.
    let mut members = unit.symbols().collect::<Vec<_>>();
    members.sort_by_key(|member| member.0);
    let mut placed: Vec<(u32, String, usize)> = Vec::new();
    for (address, _, extent) in &members {
        placed.push((
            match placement {
                Some(placement) => *placement.addresses.get(address).ok_or_else(|| {
                    format!("{}: missing regional placement for {address:08x}", unit.id)
                })?,
                None => *address,
            },
            unit.source_owner(*address)?.legacy_name(),
            *extent,
        ));
    }
    let symbols: Vec<&str> = placed
        .iter()
        .map(|(_, symbol, _)| symbol.as_str())
        .collect();
    let sectioned =
        section_functions(&produced, &symbols).map_err(|error| format!("{}: {error}", unit.id))?;
    fs::write(&assembly, sectioned).map_err(|error| error.to_string())?;
    checked(
        &compiler_core::routing::compiler_assembly_command(&assembly, &object),
        work,
    )?;
    let listing = checked(
        &strings(&["arm-none-eabi-nm", "-S", "--defined-only", &object]),
        work,
    )?;
    for (_, symbol, extent) in &placed {
        let (_, size) = symbol_span(&listing, symbol)?;
        // A declared extent may end on the alignment halfword that follows
        // a function of an odd number of halfwords; the symbol itself stops
        // before it. Anything else is a real difference.
        if size > *extent || *extent - size >= 4 {
            return Err(format!(
                "{}: {symbol} extent differs ({size} compiled, {extent} declared)",
                unit.id
            ));
        }
    }
    let script = at("ld");
    let mut text = String::from("SECTIONS\n{\n");
    let canonical = crate::overlay::rom::canonical_overlay(&root(), overlay)?;
    let reference = placement.map_or(canonical.as_slice(), |placement| placement.reference);
    let loaded = match placement {
        Some(_) => Some((overlay::load(&canonical, 0)?, overlay::load(reference, 0)?)),
        None => None,
    };
    let mut calls = BTreeMap::<String, BTreeSet<u64>>::new();
    let mut edition_symbols = BTreeMap::new();
    for ((canonical_address, _, _), (address, _, extent)) in members.iter().zip(&placed) {
        let (found, translations) = if placement.is_some() {
            paired_overlay_calls(&canonical, *canonical_address, reference, *address, *extent)?
        } else {
            (
                overlay::call_symbols(
                    reference,
                    (*address - overlay::RESOURCE_BASE) as usize,
                    *extent,
                )?,
                BTreeMap::new(),
            )
        };
        for (name, targets) in found {
            calls.entry(name).or_default().extend(targets);
        }
        for (alias, symbol) in &unit.absolute_symbols {
            let translated = match symbol.kind {
                AbsoluteSymbolKind::Thumb => translations.get(&symbol.address).copied(),
                AbsoluteSymbolKind::Data | AbsoluteSymbolKind::Arm => match &loaded {
                    Some((canonical, edition)) => paired_data_alias(
                        canonical,
                        *canonical_address,
                        edition,
                        *address,
                        *extent,
                        symbol.address,
                    )?,
                    None => None,
                },
            };
            if let Some(address) = translated {
                let translated = AbsoluteSymbol { address, ..*symbol };
                if edition_symbols
                    .insert(alias.clone(), translated)
                    .is_some_and(|old| old != translated)
                {
                    return Err(format!("{alias}: conflicting regional symbol addresses"));
                }
            }
        }
    }
    if placement.is_some() {
        for (alias, symbol) in &unit.absolute_symbols {
            if symbol.kind != AbsoluteSymbolKind::Thumb && !edition_symbols.contains_key(alias) {
                return Err(format!("{alias}: no corresponding regional data address"));
            }
        }
    }
    for (address, symbol, _) in &placed {
        let address = address + overlay::RUNTIME_BASE - overlay::RESOURCE_BASE;
        text.push_str(&format!(
            "  .text.{symbol} 0x{address:08x} : {{ *(.text.{symbol}) }}\n"
        ));
    }
    text.push_str("  /DISCARD/ : { *(.text) *(.comment) *(.note*) }\n}\n");
    fs::write(&script, text).map_err(|error| format!("{script}: {error}"))?;
    link_placed_object(
        [&object, &symbols_source, &symbols_object, &elf],
        work,
        &script,
        Some(&placement.map_or_else(
            || unit.clone(),
            |_| {
                let mut edition_unit = unit.clone();
                edition_unit.absolute_symbols = edition_symbols;
                edition_unit
            },
        )),
        &names,
        &reference,
        &calls,
    )?;
    let mut compiled = Vec::new();
    for (address, symbol, extent) in &placed {
        let piece = at(&format!("{symbol}.bin"));
        checked(
            &strings(&[
                "arm-none-eabi-objcopy",
                "-O",
                "binary",
                "-j",
                &format!(".text.{symbol}"),
                &elf,
                &piece,
            ]),
            work,
        )?;
        let mut data = fs::read(&piece).map_err(|error| format!("{piece}: {error}"))?;
        // The assembler rounds a section up to its alignment; a function of
        // an odd number of halfwords carries two bytes of fill past its
        // declared extent, and the image's own alignment halfword owns that
        // slot. Only a shorter section is a real mismatch.
        if data.len() > *extent {
            data.truncate(*extent);
        }
        if data.len() != *extent {
            return Err(format!(
                "{}: {symbol} linked extent differs ({} != {extent})",
                unit.id,
                data.len()
            ));
        }
        compiled.push(Compiled {
            address: i64::from(*address),
            data: overlay::encode(&data, (*address - overlay::RESOURCE_BASE) as usize)?,
        });
    }
    Ok(compiled)
}

/// Gives every listed function its own `.text.<symbol>` section: the
/// section directive goes before the `.align` that opens the function's
/// block, so the function's literal pool, which follows its code, stays with
/// it. A function outside the list or a data section is an error, since the
/// placement script would drop it.
fn section_functions(assembly: &str, symbols: &[&str]) -> Result<String, String> {
    let lines: Vec<&str> = assembly.lines().collect();
    for line in &lines {
        let trimmed = line.trim();
        if trimmed.starts_with(".section") || trimmed == ".data" || trimmed == ".rodata" {
            return Err(format!("unit assembly switches sections: {trimmed}"));
        }
    }
    let mut inserts: Vec<(usize, String)> = Vec::new();
    for (index, line) in lines.iter().enumerate() {
        let Some(label) = line.strip_suffix(':') else {
            continue;
        };
        if label.starts_with('.') || label.contains(' ') {
            continue;
        }
        let is_function = index > 0
            && lines[..index]
                .iter()
                .rev()
                .take(4)
                .any(|previous| previous.trim() == ".thumb_func");
        if !is_function {
            continue;
        }
        if !symbols.contains(&label) {
            return Err(format!("function {label} is not a declared unit member"));
        }
        let mut start = index;
        while start > 0 {
            let previous = lines[start - 1].trim();
            if previous.starts_with(".align") {
                start -= 1;
                break;
            }
            if previous.starts_with(".thumb_func")
                || previous.starts_with(".global")
                || previous.starts_with(".globl")
                || previous.starts_with(".type")
            {
                start -= 1;
                continue;
            }
            break;
        }
        // The alignment directive before a function pads the previous
        // function's section, as it padded the previous function when the
        // compiler laid the file out; the new section opens after it.
        let at = if lines[start].trim().starts_with(".align") {
            start + 1
        } else {
            start
        };
        inserts.push((at, format!("\t.section\t.text.{label},\"ax\",%progbits")));
    }
    if inserts.len() != symbols.len() {
        return Err(format!(
            "unit assembly defines {} functions, {} declared",
            inserts.len(),
            symbols.len()
        ));
    }
    let mut out = Vec::with_capacity(lines.len() + inserts.len());
    let mut next = 0;
    for (index, line) in lines.iter().enumerate() {
        while next < inserts.len() && inserts[next].0 == index {
            out.push(inserts[next].1.clone());
            next += 1;
        }
        out.push((*line).to_string());
    }
    Ok(format!("{}\n", out.join("\n")))
}

/// `link_object` with a linker script instead of one text address: the
/// stubs for undefined symbols are the same.
fn link_placed_object(
    files: [&str; 4],
    work: &Path,
    script: &str,
    unit: Option<&TranslationUnit>,
    names: &SourcePaths,
    reference: &[u8],
    calls: &BTreeMap<String, BTreeSet<u64>>,
) -> Result<(), String> {
    let [object, symbols_source, symbols_object, elf] = files;
    let undefined = checked(&strings(&["arm-none-eabi-nm", "-u", object]), work)?;
    let relocations = call_relocations(object, work)?;
    let mut stubs = names.main_symbol_exports();
    for name in undefined
        .lines()
        .filter_map(|line| line.split_whitespace().last())
    {
        if names.main_symbol(name)?.is_some() {
            continue;
        }
        stubs.push_str(&overlay_external_assembly(
            name,
            unit,
            relocations.contains(name),
            reference,
            calls,
        )?);
    }
    fs::write(symbols_source, stubs).map_err(|error| format!("{symbols_source}: {error}"))?;
    assemble_file(symbols_source, symbols_object, work)?;
    checked(
        &strings(&[
            "arm-none-eabi-ld",
            "-T",
            script,
            "-o",
            elf,
            object,
            symbols_object,
        ]),
        work,
    )
    .map(drop)
}

fn paired_overlay_calls(
    canonical: &[u8],
    canonical_address: u32,
    edition: &[u8],
    edition_address: u32,
    extent: usize,
) -> Result<(BTreeMap<String, BTreeSet<u64>>, BTreeMap<u64, u64>), String> {
    let canonical_offset = (canonical_address - overlay::RESOURCE_BASE) as usize;
    let edition_offset = (edition_address - overlay::RESOURCE_BASE) as usize;
    let edition_calls = overlay::call_sites(edition, edition_offset, extent)?
        .into_iter()
        .map(|(site, _, target)| (site, target))
        .collect::<BTreeMap<_, _>>();
    let mut calls = BTreeMap::<String, BTreeSet<u64>>::new();
    let mut translations = BTreeMap::new();
    for (site, name, canonical_target) in overlay::call_sites(canonical, canonical_offset, extent)?
    {
        let edition_target = *edition_calls
            .get(&site)
            .ok_or("edition lacks a corresponding overlay call")?;
        calls.entry(name).or_default().insert(edition_target);
        if translations
            .insert(canonical_target, edition_target)
            .is_some_and(|old| old != edition_target)
        {
            return Err("canonical runtime call has inconsistent edition targets".into());
        }
    }
    Ok((calls, translations))
}

fn paired_data_alias(
    canonical: &[u8],
    canonical_address: u32,
    edition: &[u8],
    edition_address: u32,
    extent: usize,
    target: u64,
) -> Result<Option<u64>, String> {
    let target = u32::try_from(target).map_err(|_| "overlay data address exceeds 32 bits")?;
    let canonical_offset = (canonical_address - overlay::RESOURCE_BASE) as usize;
    let edition_offset = (edition_address - overlay::RESOURCE_BASE) as usize;
    let canonical = canonical
        .get(canonical_offset..canonical_offset + extent)
        .ok_or("canonical overlay owner exceeds reference")?;
    let edition = edition
        .get(edition_offset..edition_offset + extent)
        .ok_or("edition overlay owner exceeds reference")?;
    let canonical =
        compiler_core::thumb::relocation_info(canonical, u64::from(canonical_address)).1;
    let edition = compiler_core::thumb::relocation_info(edition, u64::from(edition_address)).1;
    let mut value = None;
    for site in canonical
        .iter()
        .filter(|site| site.0 == b'L' && site.3 == target)
    {
        let found = edition
            .iter()
            .find(|candidate| candidate.0 == b'L' && candidate.1 == site.1)
            .ok_or("edition lacks a corresponding literal load")?
            .3;
        if value.replace(found).is_some_and(|old| old != found) {
            return Err("canonical data alias has conflicting regional targets".into());
        }
    }
    Ok(value.map(u64::from))
}
pub fn compile_declared_overlay_unit(
    unit: &TranslationUnit,
    edition: &str,
    placement: Option<&OverlayEditionPlacement<'_>>,
) -> Result<Compiled, String> {
    if !unit.exact() || unit.overlay.is_none() {
        return Err(format!("{}: not a wholly exact overlay unit", unit.id));
    }
    let work = tempdir().map_err(|error| error.to_string())?;
    compiled_span(
        compile_overlay_unit(
            unit,
            work.path(),
            unit.overlay.as_deref().unwrap(),
            Some(edition),
            placement,
        )?,
        &unit.id,
    )
}
fn compiled_span(members: Vec<Compiled>, unit_id: &str) -> Result<Compiled, String> {
    // One span from the first member to the end of the last, zero between
    // members, so a caller indexes owners by their offset from the base.
    let first = members
        .iter()
        .map(|member| member.address)
        .min()
        .ok_or_else(|| format!("{unit_id}: no compiled members"))?;
    let end = members
        .iter()
        .map(|member| member.address + member.data.len() as i64)
        .max()
        .unwrap_or(first);
    let mut data = vec![0u8; (end - first) as usize];
    for member in &members {
        let offset = (member.address - first) as usize;
        data[offset..offset + member.data.len()].copy_from_slice(&member.data);
    }
    Ok(Compiled {
        address: first,
        data,
    })
}

pub struct OverlayEditionPlacement<'a> {
    pub reference: &'a [u8],
    pub addresses: BTreeMap<u32, u32>,
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
    let placeholders = placeholder_addresses(&text);
    let names = SourcePaths::load(&root())?;
    let units = translation_units()?;
    let mut paths = BTreeSet::new();
    for address in &placeholders {
        let owner = SourceOwner::parse(&format!("{overlay}:{address:08x}"))?;
        let path = names.source_path(owner);
        if !path.is_file() {
            return Err(format!(
                "{} has an AlchemyC placeholder but no exact C source at {}",
                owner.id(),
                path.display()
            ));
        }
        paths.insert(path);
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
        compiled.extend(
            compile_overlay_unit(unit, work, overlay, None, None)
                .map_err(|error| format!("unit {}: {error}", unit.id))?,
        );
    }
    for address in placeholders.difference(&handled) {
        let owner = SourceOwner::parse(&format!("{overlay}:{address:08x}"))?;
        let path = names.source_path(owner);
        let extent = placeholder_extent(&text, *address)
            .ok_or_else(|| format!("{} has no complete placeholder extent", owner.id()))?;
        compiled.push(
            compile_overlay_c(&path, work, overlay, extent, None, &[])
                .map_err(|error| format!("{}: {error}", owner.id()))?,
        );
    }
    compiled.sort_by_key(|member| member.address);
    Ok(compiled)
}
/// The listing assembled on its own: every `AlchemyC_` placeholder stays
/// zero, no production C is composed in. A parked owner's window can be
/// checked here without depending on every other owner still compiling.
pub fn assemble_overlay_raw(source: &OverlaySource, base: i64) -> Result<Vec<u8>, String> {
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
    fs::read(&binary).map_err(|error| error.to_string())
}
pub fn assemble_overlay(source: &OverlaySource, base: i64) -> Result<Vec<u8>, String> {
    let work = tempdir().map_err(|error| error.to_string())?;
    let mut result = assemble_overlay_raw(source, base)?;
    let display = source.to_display_string();
    let overlay = source.overlay_id().unwrap_or_default();
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
    fn regional_overlay_calls_keep_canonical_names_and_take_regional_targets() {
        let call = |value: u16| {
            let high = 0xf000 | value >> 12;
            let low = 0xf800 | (value >> 1) & 0x7ff;
            [high as u8, (high >> 8) as u8, low as u8, (low >> 8) as u8]
        };
        let mut canonical = vec![0; 0x108];
        let mut regional = vec![0; 0x12c];
        canonical[0x20..0x24].copy_from_slice(&call(0x100));
        regional[0x28..0x2c].copy_from_slice(&call(0x120));
        let (calls, translated) =
            paired_overlay_calls(&canonical, 0x0200_0020, &regional, 0x0200_0028, 4).unwrap();
        assert_eq!(calls["Func_02000124"], BTreeSet::from([0x0200_8122]));
        assert_eq!(translated[&0x0200_8102], 0x0200_8122);
    }
    #[test]
    fn regional_data_aliases_come_from_corresponding_reference_literals() {
        let mut canonical = vec![0; 0x34];
        let mut regional = vec![0; 0x3c];
        canonical[0x20..0x22].copy_from_slice(&0x4800u16.to_le_bytes());
        regional[0x28..0x2a].copy_from_slice(&0x4800u16.to_le_bytes());
        canonical[0x24..0x28].copy_from_slice(&0x0200_a000u32.to_le_bytes());
        regional[0x2c..0x30].copy_from_slice(&0x0200_b000u32.to_le_bytes());
        canonical[0x30..0x34].copy_from_slice(&0x0200_a000u32.to_le_bytes());
        assert_eq!(
            paired_data_alias(
                &canonical,
                0x0200_0020,
                &regional,
                0x0200_0028,
                20,
                0x0200_a000
            )
            .unwrap(),
            Some(0x0200_b000)
        );
        canonical[0x22..0x24].copy_from_slice(&0x4901u16.to_le_bytes());
        regional[0x2a..0x2c].copy_from_slice(&0x4901u16.to_le_bytes());
        canonical[0x28..0x2c].copy_from_slice(&0x0200_a000u32.to_le_bytes());
        regional[0x30..0x34].copy_from_slice(&0x0200_c000u32.to_le_bytes());
        assert!(paired_data_alias(
            &canonical,
            0x0200_0020,
            &regional,
            0x0200_0028,
            20,
            0x0200_a000
        )
        .is_err());
    }
    #[test]
    fn only_explicit_overlay_placeholders_activate_exact_c() {
        assert_eq!(
            placeholder_addresses(
                "Func_02000104:\n  bx lr\nAlchemyC_02000104:\n  .space 8\nAlchemyC_02000314:\n"
            ),
            BTreeSet::from([0x0200_0104, 0x0200_0314])
        );
        assert!(placeholder_addresses("Func_02000104:\n  bx lr\n").is_empty());
    }
    #[test]
    fn production_overlay_preserves_missing_source_errors() {
        let work = tempdir().unwrap();
        let assembly = work.path().join("resource_382_overlay.s");
        fs::write(&assembly, "AlchemyC_0200dead:\n  .space 4\n").unwrap();
        let error =
            compile_production_overlay(&OverlaySource::path(assembly), work.path(), "resource_382")
                .unwrap_err();
        assert!(
            error.contains("resource_382:0200dead has an AlchemyC placeholder"),
            "{error}"
        );
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
            canonical_name: format!("Owner_{address:08x}"),
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
    let body = text.trim();
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
