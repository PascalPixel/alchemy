use crate::compiler::overlay;
pub use crate::compiler::overlay::placeholder_extent;
use crate::compiler::overlay::{placeholder_addresses, OverlayImage, SymbolUses};
use crate::compiler::plan::{source_to_assembly_plan, SourceToAssemblyPlanOptions};
use crate::compiler::routing::root;
use crate::compiler::routing::CompilerTarget;
use crate::compiler::sha256;
use crate::compiler::source_inputs::compiler_source_tree_signature;
use crate::compiler::source_paths::{SourceOwner, SourcePaths};
use crate::compiler::translation_units::{
    AbsoluteSymbol, AbsoluteSymbolKind, TranslationUnit, TranslationUnits,
};
use crate::overlay::source::OverlaySource;
use crate::targets::DecompTarget;
use psynergy::process::run as checked;
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::OnceLock;
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
fn overlay_c_cache() -> Result<&'static psynergy::cache::SqliteCache, String> {
    static CACHE: OnceLock<Result<psynergy::cache::SqliteCache, String>> = OnceLock::new();
    match CACHE.get_or_init(|| psynergy::cache::SqliteCache::open(&overlay_c_cache_path())) {
        Ok(cache) => Ok(cache),
        Err(error) => Err(error.clone()),
    }
}
/// The game's own translation-unit manifest, loaded once per game.
fn translation_units(game: CompilerTarget) -> Result<&'static TranslationUnits, String> {
    static BROKEN_SEAL: OnceLock<Result<TranslationUnits, String>> = OnceLock::new();
    static LOST_AGE: OnceLock<Result<TranslationUnits, String>> = OnceLock::new();
    let cell = match game {
        CompilerTarget::Tbs => &BROKEN_SEAL,
        CompilerTarget::Tla => &LOST_AGE,
    };
    match cell.get_or_init(|| TranslationUnits::load_game(&root(), game)) {
        Ok(units) => Ok(units),
        Err(error) => Err(error.clone()),
    }
}
/// The game's owner register, loaded once per game like its units.
fn source_paths(game: &str) -> Result<&'static SourcePaths, String> {
    static BROKEN_SEAL: OnceLock<Result<SourcePaths, String>> = OnceLock::new();
    static LOST_AGE: OnceLock<Result<SourcePaths, String>> = OnceLock::new();
    let cell = match game {
        "tbs" => &BROKEN_SEAL,
        "tla" => &LOST_AGE,
        _ => return Err(format!("invalid game id {game:?}")),
    };
    match cell.get_or_init(|| SourcePaths::load_for_game(&root(), game)) {
        Ok(paths) => Ok(paths),
        Err(error) => Err(error.clone()),
    }
}
/// The digest of the game's unit manifest and owner register, the
/// registries every overlay compile binds through, taken once per game.
fn registry_digest(game: CompilerTarget) -> Result<String, String> {
    static BROKEN_SEAL: OnceLock<Result<String, String>> = OnceLock::new();
    static LOST_AGE: OnceLock<Result<String, String>> = OnceLock::new();
    let cell = match game {
        CompilerTarget::Tbs => &BROKEN_SEAL,
        CompilerTarget::Tla => &LOST_AGE,
    };
    cell.get_or_init(|| {
        let mut registries = Vec::new();
        append_frame(&mut registries, &translation_unit_signature(game)?);
        append_frame(
            &mut registries,
            &fs::read(source_paths(game.as_str())?.manifest_path())
                .map_err(|error| error.to_string())?,
        );
        Ok(sha256::hex(&registries))
    })
    .clone()
}
fn write_overlay_bindings(overlay: &str, text: &str) -> Result<PathBuf, String> {
    let directory = root().join("out/overlay-bindings");
    fs::create_dir_all(&directory).map_err(|error| format!("{}: {error}", directory.display()))?;
    let path = directory.join(format!(
        "{overlay}-{}.h",
        &sha256::hex(text.as_bytes())[..16]
    ));
    if !fs::read(&path).is_ok_and(|bytes| bytes == text.as_bytes()) {
        psynergy::cache::write_cache_entry_atomically(&path, text.as_bytes())
            .map_err(|error| format!("{}: {error}", path.display()))?;
    }
    Ok(path)
}
fn append_frame(stream: &mut Vec<u8>, bytes: &[u8]) {
    stream.extend_from_slice(&(bytes.len() as u64).to_be_bytes());
    stream.extend_from_slice(bytes);
}
fn command_identity(commands: &[Vec<String>], work: &str) -> Vec<u8> {
    let root = root().to_string_lossy().into_owned();
    let mut identity = Vec::new();
    append_frame(&mut identity, b"overlay-plan-v2");
    identity.extend_from_slice(&(commands.len() as u64).to_be_bytes());
    for command in commands {
        identity.extend_from_slice(&(command.len() as u64).to_be_bytes());
        for part in command {
            // The work directory is per compile and the checkout per
            // worktree; neither names an input.
            let normalized = if part.starts_with(work) {
                "<work>".to_string()
            } else {
                part.replace(&root, "<root>")
            };
            append_frame(&mut identity, normalized.as_bytes());
        }
    }
    identity
}
fn assemble_file(source: &str, object: &str, work: &Path) -> Result<(), String> {
    checked(
        &crate::compiler::routing::assembly_command(source, object),
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
        crate::compiler::bundle::executable_signature()?,
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
/// Every undefined symbol of an object, with the relocations that reach it.
fn undefined_symbols(object: &str, work: &Path) -> Result<BTreeMap<String, SymbolUses>, String> {
    let mut undefined = checked(&strings(&["arm-none-eabi-nm", "-u", object]), work)?
        .lines()
        .filter_map(|line| line.split_whitespace().last())
        .map(|name| (name.to_string(), SymbolUses::default()))
        .collect::<BTreeMap<_, _>>();
    for line in checked(&strings(&["arm-none-eabi-objdump", "-r", object]), work)?.lines() {
        let fields = line.split_whitespace().collect::<Vec<_>>();
        if let (Some(kind), Some(uses)) = (
            fields.get(1).filter(|kind| kind.starts_with("R_ARM_")),
            fields.get(2).and_then(|name| undefined.get_mut(*name)),
        ) {
            uses.record(kind);
        }
    }
    Ok(undefined)
}
/// Declared absolute symbols plus each undefined name the register binds in
/// `image`, the unit's canonical image where register addresses hold; a
/// placement translates both alike.
fn registered_symbols(
    image: &OverlayImage<'_>,
    declared: &BTreeMap<String, AbsoluteSymbol>,
    undefined: &BTreeMap<String, SymbolUses>,
) -> Result<BTreeMap<String, AbsoluteSymbol>, String> {
    let mut symbols = declared.clone();
    for (name, uses) in undefined {
        if symbols.contains_key(name) {
            continue;
        }
        if let Some(symbol) = image.registered(name, *uses)? {
            symbols.insert(name.clone(), symbol);
        }
    }
    Ok(symbols)
}
/// One fresh compile of an overlay unit in its canonical image, every member
/// placed at its own address: the object the production listing links.
pub fn compile_overlay_unit_fresh(unit: &TranslationUnit) -> Result<Vec<Compiled>, String> {
    let work = tempdir().map_err(|error| error.to_string())?;
    compile_overlay_unit(unit, work.path(), unit.image(), None, None, None, None)
}
/// One fresh compile of an overlay owner of `target`'s game: nothing read
/// from or written to the cache, so repeated calls measure the compiler.
pub fn compile_overlay_c_fresh(
    target: DecompTarget,
    source: &Path,
    work: &Path,
    overlay: &str,
    extent: usize,
) -> Result<Compiled, String> {
    compile_overlay_c_for(target, source, work, overlay, extent, false)
}
fn translation_unit_signature(game: CompilerTarget) -> Result<Vec<u8>, String> {
    let path = root().join(game.recon()).join("translation-units.json");
    match fs::read(&path) {
        Ok(bytes) => Ok(bytes),
        Err(error)
            if game != CompilerTarget::Tbs && error.kind() == std::io::ErrorKind::NotFound =>
        {
            Ok(Vec::new())
        }
        Err(error) => Err(format!("{}: {error}", path.display())),
    }
}
fn compile_overlay_c_for(
    target: DecompTarget,
    source: &Path,
    work: &Path,
    overlay: &str,
    extent: usize,
    cached: bool,
) -> Result<Compiled, String> {
    let game = target.compiler.as_str();
    let source_display = source.to_string_lossy().to_string();
    let source_paths = source_paths(game)?;
    let owner = source_paths
        .overlay_owner_for_path(overlay, source)?
        .or_else(|| {
            SourceOwner::from_legacy_stem(&source.file_stem()?.to_str()?.to_ascii_lowercase())
        })
        .ok_or_else(|| {
            format!(
                "{} has no overlay owner; supply a registered route",
                source.display()
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
    let units = translation_units(target.compiler)?;
    let unit = units.unit_for_game_owner(game, owner);
    if let Some(unit) = unit {
        if unit.instance_owner(overlay, owner.address()).is_some() {
            return Err(format!(
                "{} is an instance owner of unit {} and compiles only with that unit",
                owner.id(),
                unit.id
            ));
        }
    }
    let reference = crate::overlay::rom::canonical_overlay_for(&root(), target, overlay)?;
    let routing_source = owner
        .routing_path_for_game(game)
        .to_string_lossy()
        .into_owned();
    let work_display = work.to_string_lossy().to_string();
    let at = |name: &str| work.join(name).to_string_lossy().to_string();
    let assembly = at(&format!("{stem}.s"));
    let mut options = SourceToAssemblyPlanOptions::new(
        target.compiler,
        routing_source.clone(),
        source_display.clone(),
        assembly.clone(),
    );
    options.preprocessed_output = Some(at(&format!("{stem}.i")));
    let register = source_paths.symbol_bindings(Some(owner));
    // The recovered-binding manifest names Broken Seal symbols only; a
    // Lost Age owner binds through its own register alone.
    let recovered = match target.compiler {
        CompilerTarget::Tbs => crate::compiler::source_bindings::production_bindings(
            &root(),
            CompilerTarget::Tbs,
            &register,
            Some(Path::new(&source_display)),
        )?,
        CompilerTarget::Tla => String::new(),
    };
    let binding_text = crate::compiler::source_bindings::with_register(&register, &recovered);
    let bindings = write_overlay_bindings(overlay, &binding_text)?;
    options.preprocessor_flags = vec!["-include".into(), bindings.to_string_lossy().into_owned()];
    let steps = source_to_assembly_plan(&options)?;
    let configuration = crate::candidate::CandidateCompilerConfiguration {
        overlay_extent: Some(extent),
        absolute_symbols: unit
            .map(TranslationUnit::canonical_symbols)
            .transpose()?
            .unwrap_or_default(),
        ..Default::default()
    };
    let mut source_inputs = compiler_source_tree_signature(&root(), source, &steps)?;
    append_frame(
        &mut source_inputs,
        registry_digest(target.compiler)?.as_bytes(),
    );
    // The binding path carries a content hash, so a rename changes the
    // command; hashing the text keeps the key honest if that ever changes.
    append_frame(&mut source_inputs, binding_text.as_bytes());
    append_frame(&mut source_inputs, &reference);
    append_frame(&mut source_inputs, &extent.to_le_bytes());
    let plan_signature = sha256::hex(&command_identity(&steps, &work_display));
    let host_signature = crate::compiler::bundle::host_executable_signature(&OVERLAY_HOST_TOOLS)
        .map_err(|error| format!("overlay host tool signature: {error}"))?;
    let cache_key = overlay_cache_key(
        &crate::compiler::bundle::compiler_bundle_signature(),
        &host_signature,
        &plan_signature,
        address,
        &source_inputs,
    )?;
    if cached {
        if let Ok(cache) = overlay_c_cache() {
            let hit = cache
                .get(&cache_key)
                .ok()
                .flatten()
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
        &[],
        f64::from(overlay::RESOURCE_BASE),
        target.compiler,
        &configuration,
    )?
    .actual;
    if cached {
        if let Ok(cache) = overlay_c_cache() {
            let _ = cache.put(&cache_key, &[("payload", &data)]);
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
/// One compile of a unit's source linked into `image`: its canonical overlay
/// or an instance. The object is always the canonical compile, whose register
/// binds member names to canonical legacy symbols; a placement moves each
/// member to where `image` links it in `edition`. Calls and literals then pair
/// from the canonical image's sites, and declared entries win. An English
/// instance also binds every registered name in its own image, and the paired
/// sites must reach exactly those addresses.
fn compile_overlay_unit(
    unit: &TranslationUnit,
    work: &Path,
    image: &str,
    edition: Option<&str>,
    placement: Option<&OverlayPlacement<'_>>,
    selected: Option<u32>,
    candidate: Option<&Path>,
) -> Result<Vec<Compiled>, String> {
    let instance = image != unit.image();
    if !unit.links(image) {
        return Err(format!("{}: {image} does not link this unit", unit.id));
    }
    if instance && placement.is_none() {
        return Err(format!(
            "{}: instance {image} links only through a placement",
            unit.id
        ));
    }
    let game = unit.target()?;
    let names = source_paths(game.as_str())?;
    let source = root().join(&unit.source);
    let first = unit
        .owners
        .first()
        .ok_or("empty overlay translation unit")?;
    let routed = unit.source_owner(unit.image(), first.address)?;
    let at = |suffix: &str| {
        work.join(format!("{}.{suffix}", unit.id))
            .to_string_lossy()
            .into_owned()
    };
    let [assembly, object, symbols_source, symbols_object, elf] =
        ["s", "o", "symbols.s", "symbols.o", "elf"].map(at);
    let mut options = SourceToAssemblyPlanOptions::new(
        game,
        routed
            .routing_path_for_game(game.as_str())
            .to_string_lossy(),
        candidate.unwrap_or(&source).to_string_lossy(),
        assembly.clone(),
    );
    options.preprocessed_output = Some(at("i"));
    if let Some(edition) = edition {
        options
            .preprocessor_flags
            .push(format!("-DTBS_EDITION_{}=1", edition.to_ascii_uppercase()));
    }
    // A candidate takes the installed source's bindings, as it would once installed.
    let register = names.symbol_bindings(Some(routed));
    // The recovered-binding manifest names Broken Seal symbols only; a Lost
    // Age unit binds through its own register and absolute symbols alone.
    let recovered = match game {
        CompilerTarget::Tbs => crate::compiler::source_bindings::production_bindings(
            &root(),
            CompilerTarget::Tbs,
            &register,
            Some(source.as_path()),
        )?,
        CompilerTarget::Tla => String::new(),
    };
    let binding_text = crate::compiler::source_bindings::with_register(&register, &recovered);
    let bindings = write_overlay_bindings(unit.image(), &binding_text)?;
    options
        .preprocessor_flags
        .extend(["-include".into(), bindings.to_string_lossy().into_owned()]);
    let commands = source_to_assembly_plan(&options)?;
    // A production link of the installed source is answered from the cache
    // like an owner compile; a selected owner or a candidate always compiles.
    let cache_key = match (selected, candidate) {
        (None, None) => Some(unit_cache_key(
            unit,
            game,
            image,
            edition,
            placement,
            &source,
            &commands,
            &binding_text,
            work,
        )?),
        _ => None,
    };
    if let Some(hit) = cache_key.as_deref().and_then(cached_unit) {
        return Ok(hit);
    }
    for command in &commands {
        checked(command, work)?;
    }
    let produced = fs::read_to_string(&assembly).map_err(|error| error.to_string())?;
    // The unit's functions need not be contiguous in the image: retained
    // assembly, tables, and pools sit between them. Each function goes into
    // its own section and one link places every section at its owner's
    // address, so intra-unit calls and pool words see the real layout.
    let mut members = unit.members_in(image).collect::<Vec<_>>();
    members.sort_by_key(|member| member.canonical);
    let symbols = members
        .iter()
        .map(|member| {
            unit.source_owner(unit.image(), member.canonical)
                .map(|owner| owner.legacy_name())
        })
        .collect::<Result<Vec<_>, _>>()?;
    let mut sectioned = section_functions(
        &produced,
        &symbols.iter().map(String::as_str).collect::<Vec<_>>(),
        unit.data.is_some(),
    )
    .map_err(|error| format!("{}: {error}", unit.id))?;
    let unselected = members
        .iter()
        .zip(&symbols)
        .filter(|(member, _)| selected.is_some_and(|selected| member.canonical != selected))
        .map(|(member, symbol)| (member.canonical, symbol.clone()))
        .collect::<Vec<_>>();
    // A selected member links alone: only its section stays, so a call or
    // literal reaching another member is an undefined name. It binds at that
    // member's canonical address and, in a placement, pairs from the canonical
    // site like any other binding. A data unit keeps every function linked
    // instead, since its tables point at them.
    let mut unit_symbols = unit.absolute_symbols.clone();
    if let Some(selected) = selected {
        let index = members
            .iter()
            .position(|member| member.canonical == selected)
            .ok_or_else(|| format!("{}: undeclared selected owner", unit.id))?;
        if unit.data.is_none() {
            sectioned = selected_section(&sectioned, &symbols[index])
                .map_err(|error| format!("{}: {error}", unit.id))?;
        }
        for (member, symbol) in members.iter().zip(&symbols) {
            if unit.data.is_none() && member.canonical != selected {
                unit_symbols.insert(
                    symbol.clone(),
                    AbsoluteSymbol {
                        address: u64::from(
                            member.canonical + overlay::RUNTIME_BASE - overlay::RESOURCE_BASE,
                        ),
                        kind: AbsoluteSymbolKind::Thumb,
                    },
                );
            }
        }
        members.retain(|member| member.canonical == selected);
    }
    fs::write(&assembly, sectioned).map_err(|error| error.to_string())?;
    let mut placed: Vec<(u32, String, usize)> = Vec::new();
    for member in &members {
        let regional =
            edition.and_then(|edition| unit.edition_owner(image, edition, member.canonical));
        placed.push((
            match placement {
                Some(placement) => {
                    *placement.addresses.get(&member.canonical).ok_or_else(|| {
                        format!(
                            "{}: missing regional placement for {:08x}",
                            unit.id, member.canonical
                        )
                    })?
                }
                None => member.address,
            },
            unit.source_owner(unit.image(), member.canonical)?
                .legacy_name(),
            regional.map_or(member.extent, |owner| owner.extent),
        ));
    }
    let mut spans = placed.iter().collect::<Vec<_>>();
    spans.sort_by_key(|member| member.0);
    for pair in spans.windows(2) {
        let end = u64::from(pair[0].0) + pair[0].2 as u64;
        if end > u64::from(pair[1].0) {
            return Err(format!(
                "{}: regional extents overlap: {} and {}",
                unit.id, pair[0].1, pair[1].1
            ));
        }
    }
    checked(
        &crate::compiler::routing::compiler_assembly_command(&assembly, &object),
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
    let canonical = crate::overlay::rom::canonical_overlay_for(
        &root(),
        crate::overlay::owners::production_target(game),
        unit.image(),
    )?;
    let reference = placement.map_or(canonical.as_slice(), |placement| placement.reference);
    // A data unit links into its canonical image alone, so its members'
    // canonical extents are the extents placed there.
    let data_address = match (unit.data, placement) {
        (None, _) => None,
        (Some(data), None) => Some(data.address),
        (Some(data), Some(_)) => {
            let spans = members
                .iter()
                .map(|member| (member.canonical, member.name, member.extent))
                .collect::<Vec<_>>();
            Some(regional_data_address(
                data,
                &spans,
                &placed,
                &overlay::load(&canonical, 0)?,
                &overlay::load(reference, 0)?,
            )?)
        }
    };
    // Only the image's own reference can prove separately owned alignment.
    let english = edition.is_none_or(|edition| edition == "en");
    let gaps = match placement {
        None => unit.compiler_gaps_in(image),
        Some(_) if instance && english => unit.compiler_gaps_in(image),
        Some(_) => &[],
    };
    let script = at("ld");
    let mut text = String::from("SECTIONS\n{\n");
    for (address, symbol, extent) in &placed {
        let (_, size) = symbol_span(&listing, symbol)?;
        let aligned = size < *extent
            || gaps
                .iter()
                .any(|gap| address.checked_add(*extent as u32) == Some(gap.start));
        let alignment = if aligned {
            format!(" . = ALIGN(ALIGNOF(.text.{symbol}));")
        } else {
            String::new()
        };
        let address = address + overlay::RUNTIME_BASE - overlay::RESOURCE_BASE;
        text.push_str(&format!(
            "  .text.{symbol} 0x{address:08x} : {{ *(.text.{symbol}){alignment} }}\n"
        ));
    }
    if let Some(address) = data_address {
        let address = address + overlay::RUNTIME_BASE - overlay::RESOURCE_BASE;
        text.push_str(&format!("  .rodata 0x{address:08x} : {{ *(.rodata) }}\n"));
        if selected.is_some() {
            // The tables point at the unit's other functions, so those stay
            // linked; each keeps its own placement so the selected owner's
            // calls to them resolve as in the image. Only the selected owner
            // is extracted, and a function without a placement links well
            // away from the overlay.
            text.push_str(&unselected_sections(&unselected, placement));
            text.push_str("  .text.unselected 0x02100000 : { *(.text.*) }\n");
        }
    }
    text.push_str("  /DISCARD/ : { *(.text*) *(.comment) *(.note*) }\n}\n");
    fs::write(&script, text).map_err(|error| format!("{script}: {error}"))?;
    if selected.is_some() {
        let selected_object = at("selected.o");
        checked(
            &strings(&[
                "arm-none-eabi-ld",
                "-r",
                "-T",
                &script,
                "-o",
                &selected_object,
                &object,
            ]),
            work,
        )?;
        checked(
            &strings(&[
                "arm-none-eabi-objcopy",
                "--strip-unneeded",
                &selected_object,
                &object,
            ]),
            work,
        )?;
    }
    let undefined = undefined_symbols(&object, work)?;
    let main = overlay::main_image(CompilerTarget::Tbs)?;
    // Register addresses are English, so only an English instance can bind
    // names in its own image; other editions rely on pairing alone.
    let mut declared = match instance && english {
        true => unit.declared_symbols(image).cloned().unwrap_or_default(),
        false => BTreeMap::new(),
    };
    if let Some(layout) = edition.and_then(|edition| unit.edition_layout(image, edition)) {
        declared.extend(layout.absolute_symbols.clone());
    }
    let links = members
        .iter()
        .zip(&placed)
        .map(|(member, (address, _, extent))| {
            member_link(unit, image, edition, member.canonical, *address, *extent)
        })
        .collect::<Vec<_>>();
    let image_at = |overlay, reference| OverlayImage {
        overlay,
        reference,
        main,
        names: &names,
    };
    let (symbols, calls) = bind_placed_unit(
        &PlacedLink {
            unit: &unit.id,
            canonical: image_at(unit.image(), &canonical),
            placed: placement.map(|placement| image_at(image, placement.reference)),
            direct: instance && english,
            unit_symbols: &unit_symbols,
            declared: &declared,
            members: &links,
            keeps_unselected: unit.data.is_some() && selected.is_some(),
        },
        &undefined,
    )?;
    link_placed_object(
        [&object, &symbols_source, &symbols_object, &elf],
        work,
        &script,
        &undefined,
        &symbols,
        reference,
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
        for gap in gaps {
            if address.checked_add(*extent as u32) != Some(gap.start) {
                continue;
            }
            let gap_len = (gap.end - gap.start) as usize;
            let loaded_reference = overlay::load(reference, 0)?;
            let offset = (gap.start - overlay::RESOURCE_BASE) as usize;
            verify_compiler_gap(&data, *extent, &loaded_reference, offset, gap_len)
                .map_err(|error| format!("{}: {error} at {:08x}", unit.id, gap.start))?;
        }
        // Separately declared alignment belongs to the gap, not the function.
        // It was compared above before extracting the unchanged owner extent.
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
    if let (Some(data), Some(address), None, None) = (unit.data, data_address, placement, selected)
    {
        let piece = at("rodata.bin");
        checked(
            &strings(&[
                "arm-none-eabi-objcopy",
                "-O",
                "binary",
                "-j",
                ".rodata",
                &elf,
                &piece,
            ]),
            work,
        )?;
        let bytes = fs::read(&piece).map_err(|error| format!("{piece}: {error}"))?;
        if bytes.len() != data.extent {
            return Err(format!(
                "{}: read-only data linked extent differs ({} != {})",
                unit.id,
                bytes.len(),
                data.extent
            ));
        }
        compiled.push(Compiled {
            address: i64::from(address),
            data: overlay::encode(&bytes, (address - overlay::RESOURCE_BASE) as usize)?,
        });
    }
    if let (Some(key), Ok(cache)) = (&cache_key, overlay_c_cache()) {
        let _ = cache.put(key, &[("unit", &unit_payload(&compiled))]);
    }
    Ok(compiled)
}
/// The identity of one production unit link: the unit and register
/// manifests, its complete source tree and bindings, the compile plan, the
/// image and edition it links into with their reference bytes, and the
/// signed compiler, host tools and build implementation.
#[allow(clippy::too_many_arguments)]
fn unit_cache_key(
    unit: &TranslationUnit,
    game: CompilerTarget,
    image: &str,
    edition: Option<&str>,
    placement: Option<&OverlayPlacement<'_>>,
    source: &Path,
    commands: &[Vec<String>],
    binding_text: &str,
    work: &Path,
) -> Result<String, String> {
    let mut inputs = compiler_source_tree_signature(&root(), source, commands)?;
    append_frame(&mut inputs, registry_digest(game)?.as_bytes());
    append_frame(&mut inputs, binding_text.as_bytes());
    append_frame(&mut inputs, unit.id.as_bytes());
    append_frame(&mut inputs, image.as_bytes());
    append_frame(&mut inputs, edition.unwrap_or("").as_bytes());
    let canonical = crate::overlay::rom::canonical_overlay_for(
        &root(),
        crate::overlay::owners::production_target(game),
        unit.image(),
    )?;
    append_frame(&mut inputs, &canonical);
    if let Some(placement) = placement {
        append_frame(&mut inputs, placement.reference);
        for (canonical, placed) in &placement.addresses {
            append_frame(&mut inputs, &canonical.to_le_bytes());
            append_frame(&mut inputs, &placed.to_le_bytes());
        }
    }
    let host = crate::compiler::bundle::host_executable_signature(&OVERLAY_HOST_TOOLS)
        .map_err(|error| format!("overlay host tool signature: {error}"))?;
    overlay_cache_key(
        &crate::compiler::bundle::compiler_bundle_signature(),
        &host,
        &sha256::hex(&command_identity(commands, &work.to_string_lossy())),
        -1,
        &inputs,
    )
    .map(|key| format!("unit:{key}"))
}
fn unit_payload(compiled: &[Compiled]) -> Vec<u8> {
    let mut payload = Vec::new();
    for member in compiled {
        payload.extend_from_slice(&member.address.to_le_bytes());
        append_frame(&mut payload, &member.data);
    }
    payload
}
fn cached_unit(key: &str) -> Option<Vec<Compiled>> {
    let entries = overlay_c_cache().ok()?.get(key).ok().flatten()?;
    let (_, payload) = entries.into_iter().find(|(kind, _)| kind == "unit")?;
    let mut compiled = Vec::new();
    let mut rest: &[u8] = &payload;
    while !rest.is_empty() {
        let address = i64::from_le_bytes(rest.get(..8)?.try_into().ok()?);
        let length = u64::from_be_bytes(rest.get(8..16)?.try_into().ok()?) as usize;
        let data = rest.get(16..16 + length)?.to_vec();
        rest = &rest[16 + length..];
        compiled.push(Compiled { address, data });
    }
    Some(compiled)
}

/// Linker placements for the functions a selected-owner build of a data unit
/// keeps: the canonical address, or the edition's placement when one is given.
fn unselected_sections(
    unselected: &[(u32, String)],
    placement: Option<&OverlayPlacement<'_>>,
) -> String {
    unselected
        .iter()
        .filter_map(|(address, symbol)| {
            let address = match placement {
                Some(placement) => *placement.addresses.get(address)?,
                None => *address,
            };
            let address = address + overlay::RUNTIME_BASE - overlay::RESOURCE_BASE;
            Some(format!(
                "  .text.{symbol} 0x{address:08x} : {{ *(.text.{symbol}) }}\n"
            ))
        })
        .collect()
}
/// The data section's address in another edition. Every canonical literal
/// that points into the section must pair with an edition literal at the same
/// site, and all pairs must agree on one section base; a unit whose functions
/// never load a data address keeps the canonical placement.
fn regional_data_address(
    data: crate::compiler::translation_units::UnitData,
    members: &[(u32, &str, usize)],
    placed: &[(u32, String, usize)],
    canonical: &[u8],
    edition: &[u8],
) -> Result<u32, String> {
    let start = data.address + overlay::RUNTIME_BASE - overlay::RESOURCE_BASE;
    let end = u64::from(start) + data.extent as u64;
    let mut base = None;
    for ((canonical_address, _, canonical_extent), (edition_address, _, extent)) in
        members.iter().zip(placed)
    {
        let slice = |image: &[u8], address: u32, extent: usize| {
            let offset = (address - overlay::RESOURCE_BASE) as usize;
            image
                .get(offset..offset + extent)
                .ok_or_else(|| format!("overlay owner 0x{address:08x} exceeds its image"))
                .map(|bytes| psynergy::thumb::relocation_info(bytes, u64::from(address)).1)
        };
        let canonical_sites = slice(canonical, *canonical_address, *canonical_extent)?;
        let edition_sites = slice(edition, *edition_address, *extent)?;
        for site in canonical_sites
            .iter()
            .filter(|site| site.0 == b'L' && (u64::from(start)..end).contains(&u64::from(site.3)))
        {
            let paired = edition_sites
                .iter()
                .find(|candidate| candidate.0 == b'L' && candidate.1 == site.1)
                .ok_or("edition lacks a literal load of the unit's data")?;
            let regional = paired
                .3
                .checked_sub(site.3 - start)
                .ok_or("edition data literal precedes its section")?;
            if base.replace(regional).is_some_and(|old| old != regional) {
                return Err("edition data literals disagree on the section base".into());
            }
        }
    }
    Ok(base.map_or(data.address, |base| {
        base - overlay::RUNTIME_BASE + overlay::RESOURCE_BASE
    }))
}

/// Where one link places a unit member.
#[derive(Debug, PartialEq)]
struct MemberLink {
    canonical: u32,
    address: u32,
    extent: usize,
    /// The canonical extent to pair sites over, when the placed code is the
    /// canonical code: no source variant, placed at the canonical extent or,
    /// in an instance, that extent plus its trailing alignment halfword.
    paired: Option<usize>,
}
/// Where `image` links the member at `canonical` in `edition`, pairing its
/// sites over the canonical extent when the placed code is the canonical code.
fn member_link(
    unit: &TranslationUnit,
    image: &str,
    edition: Option<&str>,
    canonical: u32,
    address: u32,
    extent: usize,
) -> MemberLink {
    let canonical_extent = unit
        .symbols()
        .find(|member| member.0 == canonical)
        .map_or(0, |member| member.2);
    let variant = edition
        .and_then(|edition| unit.edition_owner(image, edition, canonical))
        .is_some_and(|owner| owner.source_variant);
    // An instance may also own the alignment halfword after its code.
    let same =
        extent == canonical_extent || image != unit.image() && extent == canonical_extent + 2;
    MemberLink {
        canonical,
        address,
        extent,
        paired: (!variant && same && canonical_extent != 0).then_some(canonical_extent),
    }
}
/// What one link of a unit's object binds its undefined names against.
struct PlacedLink<'a> {
    unit: &'a str,
    /// The unit's canonical English image, where the register binds names.
    canonical: OverlayImage<'a>,
    /// The linked image in its edition; none for the canonical English link.
    placed: Option<OverlayImage<'a>>,
    /// Whether the placed image also binds registered names itself, as an
    /// English instance does.
    direct: bool,
    /// The unit's canonical declarations.
    unit_symbols: &'a BTreeMap<String, AbsoluteSymbol>,
    /// The placed image's or edition's declarations, which win over pairing.
    declared: &'a BTreeMap<String, AbsoluteSymbol>,
    members: &'a [MemberLink],
    /// A selected-owner build of a data unit keeps its other functions
    /// linked; a name only they need may keep its canonical binding, since
    /// their bytes are never extracted and a selected owner that needed it
    /// still fails its compare.
    keeps_unselected: bool,
}
type LinkBindings = (
    BTreeMap<String, AbsoluteSymbol>,
    BTreeMap<String, BTreeSet<u64>>,
);
/// The absolute symbols and legacy call keys one link resolves. The canonical
/// link binds declared and registered names where they are. A placed link
/// pairs every call and literal site from the canonical image, including
/// address-named values; in an English instance each registered name must
/// also bind, by name, to exactly the paired address.
fn bind_placed_unit(
    link: &PlacedLink<'_>,
    undefined: &BTreeMap<String, SymbolUses>,
) -> Result<LinkBindings, String> {
    let canonical = link.canonical.reference;
    let symbols = registered_symbols(&link.canonical, link.unit_symbols, undefined)?;
    let mut calls = BTreeMap::<String, BTreeSet<u64>>::new();
    let Some(placed) = &link.placed else {
        for member in link.members {
            let offset = (member.address - overlay::RESOURCE_BASE) as usize;
            for (name, targets) in overlay::call_symbols(canonical, offset, member.extent)? {
                calls.entry(name).or_default().extend(targets);
            }
        }
        return Ok((symbols, calls));
    };
    let (image, reference) = (placed.overlay, placed.reference);
    // In another image a canonical address names the wrong place; a regional
    // edition of the canonical image keeps an unpaired spelling.
    let instance = image != link.canonical.overlay;
    let direct = match link.direct {
        true => Some(registered_symbols(placed, &BTreeMap::new(), undefined)?),
        false => None,
    };
    // An address-named value spells where it points in the canonical image;
    // elsewhere its paired literal says where the same value points.
    let spelled = undefined
        .iter()
        .filter(|(name, uses)| !uses.call && !symbols.contains_key(*name))
        .filter_map(|(name, _)| {
            let symbol = crate::compiler::symbols::external_symbol(
                name,
                crate::compiler::symbols::CALL_VIA_BASE,
            )
            .filter(|_| !name.starts_with("_call_via_"))?;
            let kind = match symbol.thumb {
                true => AbsoluteSymbolKind::Thumb,
                false => AbsoluteSymbolKind::Data,
            };
            let symbol = AbsoluteSymbol {
                address: symbol.address,
                kind,
            };
            Some((name.clone(), symbol))
        })
        .collect::<BTreeMap<_, _>>();
    let loaded = (overlay::load(canonical, 0)?, overlay::load(reference, 0)?);
    let mut bound = link.declared.clone();
    for member in link.members {
        let (found, translations) = match member.paired {
            Some(extent) => paired_overlay_calls(
                canonical,
                member.canonical,
                reference,
                member.address,
                extent,
            )?,
            None => (
                overlay::call_symbols(
                    reference,
                    (member.address - overlay::RESOURCE_BASE) as usize,
                    member.extent,
                )?,
                BTreeMap::new(),
            ),
        };
        for (name, targets) in found {
            calls.entry(name).or_default().extend(targets);
        }
        let Some(extent) = member.paired else {
            continue;
        };
        for (alias, symbol) in symbols.iter().chain(&spelled) {
            if link.declared.contains_key(alias) {
                continue;
            }
            let thumb = symbol.kind == AbsoluteSymbolKind::Thumb;
            let translated = match translations.get(&symbol.address) {
                Some(target) if thumb => Some(*target),
                _ => paired_data_alias(
                    &loaded.0,
                    member.canonical,
                    &loaded.1,
                    member.address,
                    extent,
                    symbol.address | u64::from(thumb),
                )?
                .map(|address| address & !u64::from(thumb)),
            };
            let Some(address) = translated else {
                continue;
            };
            let translated = AbsoluteSymbol { address, ..*symbol };
            if let Some(named) = direct.as_ref().and_then(|direct| direct.get(alias)) {
                if *named != translated {
                    return Err(format!(
                        "{alias}: {image} binds this name to 0x{:08x}, but the paired site of {} reaches 0x{address:08x}",
                        named.address, link.unit
                    ));
                }
            }
            if bound
                .insert(alias.clone(), translated)
                .is_some_and(|old| old != translated)
            {
                return Err(format!("{alias}: conflicting regional symbol addresses"));
            }
        }
    }
    let window =
        u64::from(overlay::RUNTIME_BASE)..u64::from(overlay::RUNTIME_BASE) + canonical.len() as u64;
    for alias in undefined.keys() {
        let canonical_value = instance
            && spelled
                .get(alias)
                .is_some_and(|symbol| window.contains(&symbol.address));
        if (symbols.contains_key(alias) || canonical_value) && !bound.contains_key(alias) {
            match symbols.get(alias).filter(|_| link.keeps_unselected) {
                Some(symbol) => bound.insert(alias.clone(), *symbol),
                None => return Err(format!("{alias}: no corresponding regional data address")),
            };
        }
    }
    // A registered name means one function in every image: an instance that
    // registers no owner of that name would reach something else.
    if let Some(direct) = &direct {
        let unbound = symbols.keys().find(|alias| {
            !link.unit_symbols.contains_key(*alias)
                && !link.declared.contains_key(*alias)
                && !direct.contains_key(*alias)
        });
        if let Some(alias) = unbound {
            return Err(format!(
                "{alias}: {image} registers no owner of that name, so its instance of {} would reach a different function",
                link.unit
            ));
        }
    }
    Ok((bound, calls))
}

fn verify_compiler_gap(
    section: &[u8],
    owner_extent: usize,
    reference: &[u8],
    offset: usize,
    length: usize,
) -> Result<(), String> {
    let emitted = section
        .get(owner_extent..owner_extent + length)
        .ok_or("missing compiler gap bytes")?;
    let expected = reference
        .get(offset..offset + length)
        .ok_or("compiler gap outside reference")?;
    if emitted != expected {
        return Err("compiler gap differs".into());
    }
    Ok(())
}

/// Gives every listed function its own `.text.<symbol>` section: the
/// section directive goes before the `.align` that opens the function's
/// block, so the function's literal pool, which follows its code, stays with
/// it. A function outside the list is an error, since the placement script
/// would drop it; so is any data section other than the `.rodata` of a unit
/// that declares where its read-only data links.
fn section_functions(
    assembly: &str,
    symbols: &[&str],
    read_only_data: bool,
) -> Result<String, String> {
    let lines: Vec<&str> = assembly.lines().collect();
    for line in &lines {
        let trimmed = line.trim();
        let rodata = trimmed
            .strip_prefix(".section")
            .is_some_and(|name| name.trim() == ".rodata");
        if (rodata && !read_only_data)
            || (!rodata && trimmed.starts_with(".section"))
            || trimmed == ".data"
            || trimmed == ".rodata"
        {
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
        // Keep the compiler's alignment on the function's own section so
        // the linker can preserve it when placing independently owned ranges.
        inserts.push((start, format!("\t.section\t.text.{label},\"ax\",%progbits")));
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

/// Sectioned unit assembly reduced to its file header and one member's
/// section, pool included. The other members' labels become undefined.
fn selected_section(sectioned: &str, symbol: &str) -> Result<String, String> {
    let opening = format!("\t.section\t.text.{symbol},\"ax\",%progbits");
    let mut out = Vec::new();
    let (mut header, mut keep, mut found) = (true, false, false);
    for line in sectioned.lines() {
        if line.trim_start().starts_with(".section") {
            header = false;
            keep = line == opening;
            found |= keep;
        }
        if header || keep {
            out.push(line);
        }
    }
    if !found {
        return Err(format!("unit assembly has no section for {symbol}"));
    }
    Ok(format!("{}\n", out.join("\n")))
}

/// `link_object` with a linker script instead of one text address: the
/// stubs for undefined symbols are the same. `absolute_symbols` already
/// holds the registered names, so the rest are legacy names.
fn link_placed_object(
    files: [&str; 4],
    work: &Path,
    script: &str,
    undefined: &BTreeMap<String, SymbolUses>,
    absolute_symbols: &BTreeMap<String, AbsoluteSymbol>,
    reference: &[u8],
    calls: &BTreeMap<String, BTreeSet<u64>>,
) -> Result<(), String> {
    let [object, symbols_source, symbols_object, elf] = files;
    let mut stubs = String::new();
    for (name, uses) in undefined {
        let symbol = match absolute_symbols.get(name) {
            Some(symbol) => *symbol,
            None => overlay::external(name, uses.call, reference, calls)?,
        };
        stubs.push_str(&absolute_symbol_assembly(name, symbol));
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

/// Where an edition keeps one of the unit's functions, read from the literal
/// that loads its address at the same site of a paired owner.
pub(crate) fn paired_function_address(
    canonical: &[u8],
    canonical_address: u32,
    edition: &[u8],
    edition_address: u32,
    extent: usize,
    function: u32,
) -> Result<Option<u32>, String> {
    let pointer = function
        .checked_sub(overlay::RESOURCE_BASE)
        .ok_or("overlay function lies below the resource base")?
        + overlay::RUNTIME_BASE
        | 1;
    let found = paired_data_alias(
        canonical,
        canonical_address,
        edition,
        edition_address,
        extent,
        u64::from(pointer),
    )?;
    found
        .map(|value| {
            (value as u32 & !1)
                .checked_sub(overlay::RUNTIME_BASE)
                .map(|offset| offset + overlay::RESOURCE_BASE)
                .ok_or_else(|| "regional function pointer lies below the runtime base".to_string())
        })
        .transpose()
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
    let canonical = psynergy::thumb::relocation_info(canonical, u64::from(canonical_address)).1;
    let edition = psynergy::thumb::relocation_info(edition, u64::from(edition_address)).1;
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
/// A wholly exact overlay unit linked into `image` for `edition`; `selected`
/// names one member by its canonical address.
pub fn compile_declared_overlay_unit(
    unit: &TranslationUnit,
    image: &str,
    edition: &str,
    placement: Option<&OverlayPlacement<'_>>,
    selected: Option<u32>,
) -> Result<Compiled, String> {
    if !unit.exact() || unit.overlay.is_none() {
        return Err(format!("{}: not a wholly exact overlay unit", unit.id));
    }
    let work = tempdir().map_err(|error| error.to_string())?;
    compiled_span(
        compile_overlay_unit(
            unit,
            work.path(),
            image,
            Some(edition),
            placement,
            selected,
            None,
        )?,
        &unit.id,
    )
}
/// A wholly exact overlay unit linked into `image` as the English production
/// build links it, compiling `candidate` in place of the unit source when one
/// is given, in `work` when one is given.
pub fn compile_unit_in_image(
    unit: &TranslationUnit,
    image: &str,
    candidate: Option<&Path>,
    work: Option<&Path>,
) -> Result<Compiled, String> {
    if !unit.exact() || unit.overlay.is_none() {
        return Err(format!("{}: not a wholly exact overlay unit", unit.id));
    }
    let reference = match image != unit.image() {
        true => Some(crate::overlay::rom::canonical_overlay(&root(), image)?),
        false => None,
    };
    let placement = reference
        .as_deref()
        .map(|reference| OverlayPlacement::declared(unit, image, reference));
    let temporary = tempdir().map_err(|error| error.to_string())?;
    let work = match work {
        Some(work) => {
            fs::create_dir_all(work).map_err(|error| format!("{}: {error}", work.display()))?;
            work
        }
        None => temporary.path(),
    };
    let members = compile_overlay_unit(
        unit,
        work,
        image,
        Some("en"),
        placement.as_ref(),
        None,
        candidate,
    )?;
    compiled_span(members, &unit.id)
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

/// Where one image links a unit's members: an instance, or a regional
/// edition of either image.
pub struct OverlayPlacement<'a> {
    /// That image's resource-form bytes in the edition being linked.
    pub reference: &'a [u8],
    /// Each member's placed address, by its canonical address.
    pub addresses: BTreeMap<u32, u32>,
}
impl<'a> OverlayPlacement<'a> {
    /// The English addresses `image` declares for every member.
    pub fn declared(unit: &TranslationUnit, image: &str, reference: &'a [u8]) -> Self {
        let addresses = unit
            .members_in(image)
            .map(|member| (member.canonical, member.address))
            .collect();
        Self {
            reference,
            addresses,
        }
    }
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
    // A module linked into several images compiles only with its unit, even
    // where one of them links a single owner.
    let module = names.links_module_path(path);
    if owners.len() <= 1 && !module {
        return Ok(());
    }
    let covering = units
        .iter()
        .filter(|unit| {
            unit.exact()
                && repository.join(&unit.source) == path
                && unit
                    .owners_in(overlay)
                    .map(|owner| owner.address)
                    .collect::<BTreeSet<_>>()
                    == owners
        })
        .count();
    if covering == 1 {
        return Ok(());
    }
    if module {
        return Err(format!(
            "{} is linked into several images, but no one wholly exact translation unit links its {} {overlay} owners",
            path.display(),
            owners.len()
        ));
    }
    Err(format!(
        "{} maps to multiple {overlay} owners without one wholly exact translation unit",
        path.display()
    ))
}
/// The wholly exact grouped units that link members into `overlay`, as its
/// canonical image or an instance, and the placeholders they handle: each
/// unit compiles once here, and every member it places must be a distinct
/// placeholder of the listing.
fn production_units<'a>(
    units: &'a [TranslationUnit],
    repository: &Path,
    source_root: &Path,
    overlay: &str,
    placeholders: &BTreeSet<u32>,
) -> Result<(Vec<&'a TranslationUnit>, BTreeSet<u32>), String> {
    let mut handled = BTreeSet::new();
    let linking = units
        .iter()
        .filter(|unit| {
            unit.links(overlay)
                && unit.exact()
                && repository.join(&unit.source).starts_with(source_root)
        })
        .collect::<Vec<_>>();
    for unit in &linking {
        for member in unit.members_in(overlay) {
            if !placeholders.contains(&member.address) || !handled.insert(member.address) {
                return Err(format!(
                    "{}: undeclared or duplicate grouped owner",
                    unit.id
                ));
            }
        }
    }
    Ok((linking, handled))
}
fn compile_production_overlay(
    source: &OverlaySource,
    work: &Path,
    overlay: &str,
) -> Result<Vec<Compiled>, String> {
    let text = source.read_text().map_err(|error| error.to_string())?;
    let placeholders = placeholder_addresses(&text);
    // A listing in a game's retained overlay directory compiles for that
    // game; in-memory listings keep the default target.
    let target = match source {
        OverlaySource::Path(path) => crate::overlay::owners::assembly_target(path),
        _ => crate::targets::target_for(crate::targets::DEFAULT_TARGET),
    };
    let names = source_paths(target.compiler.as_str())?;
    // Units compose only from the game's own source root, which keeps a
    // Broken Seal unit out of a Lost Age overlay and the reverse.
    let units = translation_units(target.compiler)?;
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
    let (linking, handled) = production_units(
        &units.units,
        &root(),
        &names.source_root(),
        overlay,
        &placeholders,
    )?;
    let mut data_blocks = text
        .lines()
        .filter_map(|line| {
            u32::from_str_radix(
                line.trim()
                    .strip_prefix("AlchemyData_")?
                    .strip_suffix(':')?,
                16,
            )
            .ok()
        })
        .collect::<BTreeSet<_>>();
    let mut compiled = Vec::new();
    let mut reference = None;
    for unit in linking {
        let placement = match overlay != unit.image() {
            true => {
                if reference.is_none() {
                    reference = Some(crate::overlay::rom::canonical_overlay(&root(), overlay)?);
                }
                let reference = reference.as_deref().unwrap_or_default();
                Some(OverlayPlacement::declared(unit, overlay, reference))
            }
            false => None,
        };
        if let Some(data) = unit.data {
            if overlay::data_placeholder_extent(&text, data.address) != Some(data.extent)
                || !data_blocks.remove(&data.address)
            {
                return Err(format!(
                    "{}: unit data has no AlchemyData_{:08x} placeholder of {} bytes",
                    unit.id, data.address, data.extent
                ));
            }
        }
        compiled.extend(
            compile_overlay_unit(unit, work, overlay, None, placement.as_ref(), None, None)
                .map_err(|error| format!("unit {}: {error}", unit.id))?,
        );
    }
    if let Some(address) = data_blocks.first() {
        return Err(format!(
            "{overlay}: AlchemyData_{address:08x} is not declared by an exact unit"
        ));
    }
    for address in placeholders.difference(&handled) {
        let owner = SourceOwner::parse(&format!("{overlay}:{address:08x}"))?;
        let path = names.source_path(owner);
        let extent = placeholder_extent(&text, *address)
            .ok_or_else(|| format!("{} has no complete placeholder extent", owner.id()))?;
        compiled.push(
            compile_overlay_c_for(target, &path, work, overlay, extent, true)
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
            "-I",
            &root().to_string_lossy(),
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
#[test]
fn shared_src_veneer_macro_resolves_from_temporary_assembly() {
    let source = OverlaySource::text(concat!(
        ".syntax unified\n.thumb\n",
        ".include \"games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC\"\n",
        "overlay_veneer 0x08000000\noverlay_veneer 0x08000001\n"
    ));
    assert_eq!(
        assemble_overlay_raw(&source, 0x02000000).unwrap(),
        [
            0x00, 0x4c, 0x20, 0x47, 0x00, 0x00, 0x00, 0x08, 0x00, 0x4c, 0x20, 0x47, 0x01, 0x00,
            0x00, 0x08
        ]
    );
}

pub fn assemble_overlay(source: &OverlaySource, base: i64) -> Result<Vec<u8>, String> {
    let work = tempdir().map_err(|error| error.to_string())?;
    let mut result = assemble_overlay_raw(source, base)?;
    let display = source.to_display_string();
    let overlay = source.overlay_id().unwrap_or_default();
    let mut occupied: std::collections::BTreeSet<usize> = std::collections::BTreeSet::new();
    let mut placeholder_failures = Vec::new();
    // Compiler runtime windows take bytes built from the licensed container.
    let listing = source.read_text().map_err(|error| error.to_string())?;
    let game = match source {
        OverlaySource::Path(path) => crate::overlay::owners::assembly_target(path).compiler,
        _ => CompilerTarget::Tbs,
    };
    let runtime = crate::compiler::runtime::overlay_fill(&root(), game, &overlay, &listing)?
        .into_iter()
        .map(|(address, data)| Compiled {
            address: i64::from(address),
            data,
        });
    for compiled in compile_production_overlay(source, work.path(), &overlay)?
        .into_iter()
        .chain(runtime)
    {
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
                let failure = format!(
                    "overlay C placeholder is not zero at 0x{}",
                    hex(base + byte as i64, 8)
                );
                eprintln!("diagnostic {failure}");
                placeholder_failures.push(failure);
            }
        }
        result[offset..offset + compiled.data.len()].copy_from_slice(&compiled.data);
    }
    if !placeholder_failures.is_empty() {
        return Err(format!(
            "{display}: {} overlay C placeholder failure(s):\n  {}",
            placeholder_failures.len(),
            placeholder_failures.join("\n  ")
        ));
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
    #[test]
    fn compiler_gap_rejects_modern_nop_fill_and_missing_bytes() {
        let reference = [0x70, 0x47, 0, 0];
        assert!(super::verify_compiler_gap(&reference, 2, &reference, 2, 2).is_ok());
        assert!(
            super::verify_compiler_gap(&[0x70, 0x47, 0xc0, 0x46], 2, &reference, 2, 2).is_err()
        );
        assert!(super::verify_compiler_gap(&reference[..2], 2, &reference, 2, 2).is_err());
        assert!(super::verify_compiler_gap(&reference, 2, &reference[..2], 2, 2).is_err());
    }

    use super::*;
    use crate::compiler::translation_units::{OwnerState, TranslationOwner};
    use tempfile::tempdir;
    #[test]
    fn unselected_data_unit_functions_keep_their_placements() {
        let unselected = [
            (0x0200_00d4, "Func_020000d4".to_string()),
            (0x0200_0210, "Func_02000210".to_string()),
        ];
        assert_eq!(
            unselected_sections(&unselected, None),
            "  .text.Func_020000d4 0x020080d4 : { *(.text.Func_020000d4) }\n  .text.Func_02000210 0x02008210 : { *(.text.Func_02000210) }\n"
        );
        let placement = OverlayPlacement {
            reference: &[],
            addresses: BTreeMap::from([(0x0200_00d4, 0x0200_00e0)]),
        };
        assert_eq!(
            unselected_sections(&unselected, Some(&placement)),
            "  .text.Func_020000d4 0x020080e0 : { *(.text.Func_020000d4) }\n"
        );
    }
    #[test]
    fn selected_data_unit_owner_calls_its_siblings_where_the_image_has_them() {
        let manifest = crate::compiler::translation_units::TranslationUnits::load(&root()).unwrap();
        let unit = manifest.unit("runpa-dou-cave").unwrap();
        // The gate puddle's Frost script calls the scene's gate redraw.
        let owner = unit
            .owners
            .iter()
            .find(|owner| owner.address == 0x0200_01b0)
            .unwrap();
        let compiled =
            compile_declared_overlay_unit(unit, "resource_3ad", "en", None, Some(owner.address))
                .unwrap();
        let reference = crate::overlay::rom::canonical_overlay(&root(), "resource_3ad").unwrap();
        let start = (owner.address - overlay::RESOURCE_BASE) as usize;
        let offset = (i64::from(owner.address) - compiled.address) as usize;
        assert_eq!(
            &compiled.data[offset..offset + owner.extent],
            &reference[start..start + owner.extent]
        );
    }
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
    fn regional_function_addresses_come_from_paired_pointer_literals() {
        let mut canonical = vec![0; 0x34];
        let mut regional = vec![0; 0x3c];
        canonical[0x20..0x22].copy_from_slice(&0x4800u16.to_le_bytes());
        regional[0x28..0x2a].copy_from_slice(&0x4800u16.to_le_bytes());
        canonical[0x24..0x28].copy_from_slice(&0x0200_8469u32.to_le_bytes());
        regional[0x2c..0x30].copy_from_slice(&0x0200_84a9u32.to_le_bytes());
        assert_eq!(
            paired_function_address(
                &canonical,
                0x0200_0020,
                &regional,
                0x0200_0028,
                8,
                0x0200_0468
            )
            .unwrap(),
            Some(0x0200_04a8)
        );
        assert_eq!(
            paired_function_address(
                &canonical,
                0x0200_0020,
                &regional,
                0x0200_0028,
                8,
                0x0200_0500
            )
            .unwrap(),
            None
        );
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
    fn regional_data_follows_the_paired_literal_of_any_table_in_the_section() {
        let data = crate::compiler::translation_units::UnitData {
            address: 0x0200_0100,
            extent: 0x40,
        };
        let mut canonical = vec![0; 0x140];
        let mut regional = vec![0; 0x150];
        canonical[0x30..0x32].copy_from_slice(&0x4800u16.to_le_bytes());
        canonical[0x34..0x38].copy_from_slice(&0x0200_8110u32.to_le_bytes());
        regional[0x38..0x3a].copy_from_slice(&0x4800u16.to_le_bytes());
        regional[0x3c..0x40].copy_from_slice(&0x0200_8124u32.to_le_bytes());
        let members = [(0x0200_0030u32, "Func_02000030", 8usize)];
        let placed = [(0x0200_0038u32, "Func_02000030".to_string(), 8usize)];
        assert_eq!(
            regional_data_address(data, &members, &placed, &canonical, &regional),
            Ok(0x0200_0114)
        );
        regional[0x3c..0x40].fill(0);
        regional[0x38..0x3a].fill(0);
        assert!(regional_data_address(data, &members, &placed, &canonical, &regional).is_err());
        canonical[0x34..0x38].copy_from_slice(&0x0200_8020u32.to_le_bytes());
        assert_eq!(
            regional_data_address(data, &members, &placed, &canonical, &regional),
            Ok(0x0200_0100)
        );
    }
    #[test]
    fn only_a_data_unit_may_switch_to_its_read_only_section() {
        let assembly = "\t.align\t2\n\t.global\tFunc_02000030\n\t.thumb_func\nFunc_02000030:\n\tbx\tlr\n\t.section .rodata\n\t.align\t2\ngTable:\n\t.word\t1\n";
        let sectioned = section_functions(assembly, &["Func_02000030"], true).unwrap();
        assert!(sectioned.contains(".section\t.text.Func_02000030"));
        assert!(sectioned.contains(".section .rodata"));
        assert!(section_functions(assembly, &["Func_02000030"], false).is_err());
        let writable = assembly.replace(".section .rodata", ".data");
        assert!(section_functions(&writable, &["Func_02000030"], true).is_err());
    }
    #[test]
    fn sectioning_preserves_native_linker_alignment() {
        let assembly = "\t.thumb\n.text\n\t.align\t2\n\t.global\tFirst\n\t.thumb_func\nFirst:\n\tbx\tlr\n\t.align\t2\n\t.global\tLast\n\t.thumb_func\nLast:\n\tbx\tlr\n\t.section .rodata\n\t.align\t2\n\t.word\t1\n";
        let split = section_functions(assembly, &["First", "Last"], true).unwrap();
        let work = tempfile::tempdir_in(root().join("out")).unwrap();
        let mut bytes = Vec::new();
        for (name, source, script) in [
            ("whole", assembly, "SECTIONS { .text 0x02008000 : { *(.text*) } .rodata : { *(.rodata) } }"),
            ("split", split.as_str(), "SECTIONS { .text.First 0x02008000 : { *(.text.First) . = ALIGN(ALIGNOF(.text.First)); } .text.Last 0x02008004 : { *(.text.Last) . = ALIGN(ALIGNOF(.text.Last)); } .rodata : { *(.rodata) } }"),
        ] {
            let input = work.path().join(format!("{name}.s"));
            let object = work.path().join(format!("{name}.o"));
            let binary = work.path().join(format!("{name}.bin"));
            let linker_script = work.path().join(format!("{name}.ld"));
            let elf = work.path().join(format!("{name}.elf"));
            fs::write(&input, source).unwrap();
            fs::write(&linker_script, script).unwrap();
            psynergy::process::run(
                &crate::compiler::routing::compiler_assembly_command(
                    input.to_str().unwrap(),
                    object.to_str().unwrap(),
                ),
                work.path(),
            )
            .unwrap();
            psynergy::process::run(
                &[
                    crate::compiler::routing::binutils_prefix().join("bin/arm-none-eabi-ld").to_string_lossy().into_owned(),
                    "-T".into(), linker_script.to_string_lossy().into_owned(),
                    "-o".into(), elf.to_string_lossy().into_owned(),
                    object.to_string_lossy().into_owned(),
                ], work.path(),
            ).unwrap();
            psynergy::process::run(
                &[
                    crate::compiler::routing::binutils_prefix()
                        .join("bin/arm-none-eabi-objcopy")
                        .to_string_lossy()
                        .into_owned(),
                    "-O".into(),
                    "binary".into(),
                    elf.to_string_lossy().into_owned(),
                    binary.to_string_lossy().into_owned(),
                ],
                work.path(),
            )
            .unwrap();
            bytes.push(fs::read(binary).unwrap());
        }
        assert_eq!(bytes[0].len(), 12);
        assert_eq!(bytes[1], bytes[0]);
        verify_compiler_gap(&bytes[1][4..], 2, &bytes[0], 6, 2).unwrap();
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
            _declared_state: None,
        };
        let mut unit = TranslationUnit {
            id: "shared".into(),
            game: "tbs".into(),
            _declared_game: None,
            _declared_route: None,
            source: "games/THE BROKEN SEAL/SRC/overlays/shared.c".into(),
            overlay: Some("resource_382".into()),
            absolute_symbols: BTreeMap::new(),
            editions: BTreeMap::new(),
            instances: BTreeMap::new(),
            local_symbols: Vec::new(),
            compiler_gaps: Vec::new(),
            data: None,
            owners: vec![owner(0x0200_0100), owner(0x0200_0104)],
        };
        let check = |units: &[TranslationUnit]| {
            validate_shared_overlay_source(root.path(), &names, units, "resource_382", &path)
        };
        assert!(check(&[]).is_err());
        assert!(check(std::slice::from_ref(&unit)).is_ok());
        let mut retained = unit.clone();
        retained.owners[1].state = OwnerState::NotYetC;
        assert!(check(&[retained]).is_err());

        // A module linked into resource_385 as well compiles there only as
        // that unit's instance, even for a single owner in that image.
        let names = SourcePaths::parse(
            root.path(),
            r#"{"format":3,"owners":{
                "resource_382:02000100":{"name":"Owner_02000100","source":"overlays/shared.c"},
                "resource_382:02000104":{"name":"Owner_02000104","source":"overlays/shared.c"},
                "resource_385:02000200":{"name":"Owner_02000100","source":"overlays/shared.c"}}}"#,
        )
        .unwrap();
        let in_385 = |units: &[TranslationUnit]| {
            validate_shared_overlay_source(root.path(), &names, units, "resource_385", &path)
        };
        let error = in_385(std::slice::from_ref(&unit)).unwrap_err();
        assert!(error.contains("is linked into several images"), "{error}");
        unit.owners.truncate(1);
        let placed = crate::compiler::translation_units::InstanceOwner {
            address: 0x0200_0200,
            extent: 4,
        };
        let mut instance = crate::compiler::translation_units::UnitInstance::default();
        instance.owners.insert("Owner_02000100".into(), placed);
        unit.instances.insert("resource_385".into(), instance);
        assert!(in_385(std::slice::from_ref(&unit)).is_ok());
        // Two units claiming the same instance owners are not one unit.
        assert!(in_385(&[unit.clone(), unit.clone()]).is_err());
        unit.instances
            .get_mut("resource_385")
            .unwrap()
            .owners
            .values_mut()
            .next()
            .unwrap()
            .address = 0x0200_0204;
        assert!(in_385(&[unit]).is_err());
    }
    #[test]
    fn production_overlay_compiles_each_instance_once() {
        use crate::compiler::translation_units::fixture::Repository;
        let repository = Repository::new();
        let root = repository.0.path();
        let units = repository.load().unwrap().units;
        let source_root = root.join("games/THE BROKEN SEAL/SRC");
        let select = |units: &[TranslationUnit], overlay: &str, placeholders: &[u32]| {
            let placeholders = placeholders.iter().copied().collect::<BTreeSet<_>>();
            production_units(units, root, &source_root, overlay, &placeholders).map(
                |(linking, handled)| {
                    let ids = linking
                        .iter()
                        .map(|unit| unit.id.clone())
                        .collect::<Vec<_>>();
                    (ids, handled.into_iter().collect::<Vec<_>>())
                },
            )
        };
        // The instance in resource_39b handles both of its placeholders; the
        // remaining one is left for standalone C.
        assert_eq!(
            select(
                &units,
                "resource_39b",
                &[0x0200_0630, 0x0200_0ba4, 0x0200_1000]
            )
            .unwrap(),
            (vec!["staged-actor".into()], vec![0x0200_0630, 0x0200_0ba4])
        );
        assert_eq!(
            select(&units, "resource_3bf", &[0x0200_034c, 0x0200_08c0]).unwrap(),
            (vec!["staged-actor".into()], vec![0x0200_034c, 0x0200_08c0])
        );
        assert_eq!(
            select(&units, "resource_39c", &[0x0200_0630]).unwrap(),
            (vec![], vec![])
        );
        for (units, placeholders) in [
            (units.clone(), vec![0x0200_0630]),
            (
                [units.clone(), units.clone()].concat(),
                vec![0x0200_0630, 0x0200_0ba4],
            ),
        ] {
            let error = select(&units, "resource_39b", &placeholders).unwrap_err();
            assert!(
                error.contains("staged-actor: undeclared or duplicate grouped owner"),
                "{error}"
            );
        }
        // An instance links through its image's own declared addresses.
        let reference = [0u8; 4];
        let placement = OverlayPlacement::declared(&units[0], "resource_39b", &reference);
        assert_eq!(
            placement.addresses,
            BTreeMap::from([(0x0200_034c, 0x0200_0630), (0x0200_08c0, 0x0200_0ba4)])
        );
    }
    #[test]
    fn semantic_main_alias_is_in_the_generated_export_graph() {
        let names = SourcePaths::load(&root()).unwrap();
        let binding = ".thumb_set RunBattleEffect16, 0x0809b698\n";
        assert!(binding.contains("0x0809b698"));
        assert!(names.main_symbol_exports().contains(&binding));
    }

    const IMPORT_LIST: &str = "games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/IMPORT.INC";

    fn veneer(target: u32) -> [u8; 8] {
        let mut bytes = [0; 8];
        bytes[..4].copy_from_slice(&0x4720_4c00u32.to_le_bytes());
        bytes[4..].copy_from_slice(&target.to_le_bytes());
        bytes
    }

    /// A register, a main image whose far-call veneer at 08000040 reaches
    /// 08000080, and a resource_3bf image whose only import veneer, at
    /// runtime 02008010, targets that far-call veneer.
    struct Binding {
        root: tempfile::TempDir,
        names: SourcePaths,
        main: Vec<u8>,
        reference: Vec<u8>,
    }
    impl Binding {
        fn new() -> Self {
            let root = tempdir().unwrap();
            let listing = root
                .path()
                .join("recon/tbs/raw/overlays/resource_3bf_overlay.s");
            fs::create_dir_all(listing.parent().unwrap()).unwrap();
            fs::write(
                &listing,
                format!("\t.space 16\n.include \"{IMPORT_LIST}\"\n"),
            )
            .unwrap();
            let names = SourcePaths::parse(
                root.path(),
                r#"{"format":3,"owners":{
                "main:08000040":{"name":"Object_DispatchFar"},
                "main:08000080":{"name":"Object_GetById"},
                "main:080000c0":{"name":"Object_Unimported"},
                "resource_3bf:02000100":{"name":"Scene_RunHelper","source":"FIELD/RUNPA_JO/HELPER.C"},
                "resource_3bf:02000200":{"name":"Scene_Twice"},
                "resource_3bf:02000300":{"name":"Scene_Twice"},
                "resource_3bf:02000400":{"name":"Func_02000400"},
                "resource_3be:02000500":{"name":"Scene_Elsewhere"}}}"#,
            )
            .unwrap();
            let mut main = vec![0; 0x100];
            main[0x40..0x48].copy_from_slice(&veneer(0x0800_0081));
            main[0x80..0x82].copy_from_slice(&0xb500u16.to_le_bytes());
            let mut reference = vec![0; 0x40];
            reference[..8].copy_from_slice(&veneer(0x0200_8101));
            reference[0x10..0x18].copy_from_slice(&veneer(0x0800_0041));
            Self {
                root,
                names,
                main,
                reference,
            }
        }
        fn image(&self) -> OverlayImage<'_> {
            OverlayImage {
                overlay: "resource_3bf",
                reference: &self.reference,
                main: &self.main,
                names: &self.names,
            }
        }
        fn bind(
            &self,
            declared: &BTreeMap<String, AbsoluteSymbol>,
            uses: &[(&str, bool, bool)],
        ) -> Result<BTreeMap<String, AbsoluteSymbol>, String> {
            let undefined = uses
                .iter()
                .map(|(name, call, value)| {
                    let uses = SymbolUses {
                        call: *call,
                        value: *value,
                    };
                    (name.to_string(), uses)
                })
                .collect();
            registered_symbols(&self.image(), declared, &undefined)
        }
    }
    fn thumb(address: u64) -> AbsoluteSymbol {
        AbsoluteSymbol {
            address,
            kind: AbsoluteSymbolKind::Thumb,
        }
    }

    #[test]
    fn main_name_call_binds_to_overlay_import_veneer() {
        let mut binding = Binding::new();
        let bound = binding
            .bind(
                &BTreeMap::new(),
                &[
                    ("Object_GetById", true, false),
                    ("Object_DispatchFar", true, false),
                ],
            )
            .unwrap();
        assert_eq!(bound["Object_GetById"], thumb(0x0200_8010));
        assert_eq!(bound["Object_DispatchFar"], thumb(0x0200_8010));

        // The linked call reaches the veneer; the loader keeps it there.
        let work = tempdir().unwrap();
        let at = |name: &str| work.path().join(name).to_string_lossy().into_owned();
        fs::write(
            at("unit.s"),
            ".syntax unified\n.thumb\n.section .text.Func_02000100,\"ax\",%progbits\n\
             .global Func_02000100\n.thumb_func\nFunc_02000100:\n\
             push {lr}\nbl Object_GetById\npop {r0}\nbx r0\n",
        )
        .unwrap();
        checked(
            &crate::compiler::routing::assembly_command(&at("unit.s"), &at("unit.o")),
            work.path(),
        )
        .unwrap();
        let undefined = undefined_symbols(&at("unit.o"), work.path()).unwrap();
        assert_eq!(
            undefined,
            BTreeMap::from([(
                "Object_GetById".to_string(),
                SymbolUses {
                    call: true,
                    value: false
                }
            )])
        );
        let symbols = registered_symbols(&binding.image(), &BTreeMap::new(), &undefined).unwrap();
        fs::write(
            at("unit.ld"),
            "SECTIONS\n{\n  .text.Func_02000100 0x02008100 : { *(.text.Func_02000100) }\n}\n",
        )
        .unwrap();
        link_placed_object(
            [
                &at("unit.o"),
                &at("symbols.s"),
                &at("symbols.o"),
                &at("unit.elf"),
            ],
            work.path(),
            &at("unit.ld"),
            &undefined,
            &symbols,
            &binding.reference,
            &BTreeMap::new(),
        )
        .unwrap();
        checked(
            &strings(&[
                "arm-none-eabi-objcopy",
                "-O",
                "binary",
                "-j",
                ".text.Func_02000100",
                &at("unit.elf"),
                &at("unit.bin"),
            ]),
            work.path(),
        )
        .unwrap();
        let runtime = fs::read(at("unit.bin")).unwrap();
        let loaded = overlay::load(&overlay::encode(&runtime, 0x100).unwrap(), 0x100).unwrap();
        let displacement = psynergy::thumb::bl_displacement(&loaded[2..6]).unwrap();
        assert_eq!(0x0200_8100 + 6 + i64::from(displacement), 0x0200_8010);

        // A second import veneer reaching the same function is ambiguous;
        // the far-call veneer's own name still reaches only the first.
        binding.reference[0x18..0x20].copy_from_slice(&veneer(0x0800_0081));
        let error = binding
            .bind(&BTreeMap::new(), &[("Object_GetById", true, false)])
            .unwrap_err();
        assert!(error.contains("0x02008010, 0x02008018"), "{error}");
        assert!(error.contains("all reach main:08000080"), "{error}");
        assert!(error.contains(IMPORT_LIST), "{error}");
        let bound = binding
            .bind(&BTreeMap::new(), &[("Object_DispatchFar", true, false)])
            .unwrap();
        assert_eq!(bound["Object_DispatchFar"], thumb(0x0200_8010));
    }

    #[test]
    fn main_name_without_import_is_rejected_naming_import_list() {
        let binding = Binding::new();
        let error = binding
            .bind(&BTreeMap::new(), &[("Object_Unimported", true, false)])
            .unwrap_err();
        assert!(
            error.contains("no import veneer in resource_3bf reaches main:080000c0"),
            "{error}"
        );
        assert!(error.contains(IMPORT_LIST), "{error}");
        fs::remove_dir_all(binding.root.path().join("recon/tbs/raw")).unwrap();
        let error = binding
            .bind(&BTreeMap::new(), &[("Object_Unimported", true, false)])
            .unwrap_err();
        assert!(
            error.contains("IMPORT.INC that ") && error.contains("resource_3bf_overlay.s"),
            "{error}"
        );
    }

    #[test]
    fn main_name_used_as_call_and_pointer_is_rejected() {
        let binding = Binding::new();
        let error = binding
            .bind(&BTreeMap::new(), &[("Object_GetById", true, true)])
            .unwrap_err();
        assert!(error.contains("both called and used as a value"), "{error}");
        let bound = binding
            .bind(&BTreeMap::new(), &[("Object_GetById", false, true)])
            .unwrap();
        assert_eq!(bound["Object_GetById"], thumb(0x0800_0080));
    }

    #[test]
    fn registered_overlay_owner_name_binds_runtime_address() {
        let binding = Binding::new();
        let bound = binding
            .bind(
                &BTreeMap::new(),
                &[
                    ("Scene_RunHelper", true, true),
                    ("Scene_Elsewhere", true, false),
                    ("Func_02000400", true, false),
                    ("Func_020000a2", true, false),
                ],
            )
            .unwrap();
        // Registered in this image: its runtime address, for calls and pointers.
        assert_eq!(bound["Scene_RunHelper"], thumb(0x0200_8100));
        // Another image's name and address-encoded spellings are not bound
        // here; the legacy resolver still owns them.
        assert_eq!(bound.len(), 1);
        // A declared absolute symbol comes first.
        let declared = BTreeMap::from([("Scene_RunHelper".to_string(), thumb(0x0200_8020))]);
        let bound = binding
            .bind(&declared, &[("Scene_RunHelper", true, false)])
            .unwrap();
        assert_eq!(bound, declared);
    }

    #[test]
    fn owner_name_registered_twice_in_one_image_is_ambiguous() {
        let binding = Binding::new();
        let error = binding
            .bind(&BTreeMap::new(), &[("Scene_Twice", true, false)])
            .unwrap_err();
        assert!(
            error.contains("resource_3bf:02000200, resource_3bf:02000300"),
            "{error}"
        );
    }

    /// A runtime Thumb `bl` at `from` reaching `to`.
    fn bl(from: u32, to: u32) -> [u8; 4] {
        let offset = to.wrapping_sub(from + 4);
        let high = 0xf000 | ((offset >> 12) & 0x7ff) as u16;
        let low = 0xf800 | ((offset >> 1) & 0x7ff) as u16;
        let mut pair = [0; 4];
        pair[..2].copy_from_slice(&high.to_le_bytes());
        pair[2..].copy_from_slice(&low.to_le_bytes());
        pair
    }
    /// A 0x2200-byte overlay image, in resource form, whose import veneer
    /// at `veneer` reaches main:08000081 and whose 32-byte member at `member`
    /// calls that veneer, `helper` and `other`, then loads `table` and `value`.
    fn placed_image(veneer: usize, member: u32, calls: [u32; 2], literals: [u32; 2]) -> Vec<u8> {
        let mut runtime = vec![0u8; 0x2200];
        runtime[veneer..veneer + 8].copy_from_slice(&veneer_word(0x0800_0081));
        let base = (member - overlay::RESOURCE_BASE) as usize;
        let at = |offset: usize| overlay::RUNTIME_BASE + (base + offset) as u32;
        let targets = [overlay::RUNTIME_BASE + veneer as u32, calls[0], calls[1]];
        for (index, target) in targets.into_iter().enumerate() {
            let site = base + index * 4;
            runtime[site..site + 4].copy_from_slice(&bl(at(index * 4), target));
        }
        // ldr r0, [pc, #8]; ldr r1, [pc, #12]; bx lr; then the pool.
        for (offset, word) in [(0x0c, 0x4802u16), (0x0e, 0x4903), (0x10, 0x4770)] {
            runtime[base + offset..base + offset + 2].copy_from_slice(&word.to_le_bytes());
        }
        for (offset, literal) in [(0x18, literals[0]), (0x1c, literals[1])] {
            runtime[base + offset..base + offset + 4].copy_from_slice(&literal.to_le_bytes());
        }
        overlay::encode(&runtime, 0).unwrap()
    }
    fn veneer_word(target: u32) -> [u8; 8] {
        let mut bytes = [0; 8];
        bytes[..4].copy_from_slice(&0x4720_4c00u32.to_le_bytes());
        bytes[4..].copy_from_slice(&target.to_le_bytes());
        bytes
    }
    const TABLE: &str = "StagedActor_RegionTable";
    const SPELLED: &str = "Data_0200a100";
    /// The staged-actor member at 0200_0040 in resource_3bf and at 0200_0080
    /// in resource_389, whose veneer, helper, other callee and tables all sit
    /// at different addresses.
    struct Placement {
        _root: tempfile::TempDir,
        names: SourcePaths,
        main: Vec<u8>,
        canonical: Vec<u8>,
        instance: Vec<u8>,
        unit_symbols: BTreeMap<String, AbsoluteSymbol>,
        undefined: BTreeMap<String, SymbolUses>,
    }
    impl Placement {
        fn new(register: &str) -> Self {
            let root = tempdir().unwrap();
            let names = SourcePaths::parse(root.path(), register).unwrap();
            let canonical = placed_image(
                0x10,
                0x0200_0040,
                [0x0200_8100, 0x0200_8180],
                [0x0200_a000, 0x0200_a100],
            );
            let instance = placed_image(
                0x30,
                0x0200_0080,
                [0x0200_8200, 0x0200_81c0],
                [0x0200_a800, 0x0200_a900],
            );
            let call = SymbolUses {
                call: true,
                value: false,
            };
            let value = SymbolUses {
                call: false,
                value: true,
            };
            // The legacy key the canonical disassembly gave the third call.
            let other = overlay::call_sites(&canonical, 0x40, 0x20).unwrap()[2]
                .1
                .clone();
            let undefined = BTreeMap::from([
                ("Object_GetById".to_string(), call),
                ("Scene_Helper".to_string(), call),
                (other, call),
                (TABLE.to_string(), value),
                (SPELLED.to_string(), value),
                ("Data_03001ebc".to_string(), value),
            ]);
            let table = AbsoluteSymbol {
                address: 0x0200_a000,
                kind: AbsoluteSymbolKind::Data,
            };
            Self {
                _root: root,
                names,
                main: vec![0; 0x100],
                canonical,
                instance,
                unit_symbols: BTreeMap::from([(TABLE.to_string(), table)]),
                undefined,
            }
        }
        fn bind(
            &self,
            placed: bool,
            direct: bool,
            declared: &BTreeMap<String, AbsoluteSymbol>,
        ) -> Result<LinkBindings, String> {
            self.bind_as(placed.then_some("resource_389"), direct, declared)
        }
        /// Links the member into `placed`, reading that image's bytes from
        /// the instance fixture; resource_3bf there is a regional edition.
        fn bind_as(
            &self,
            placed: Option<&str>,
            direct: bool,
            declared: &BTreeMap<String, AbsoluteSymbol>,
        ) -> Result<LinkBindings, String> {
            let image = |overlay, reference| OverlayImage {
                overlay,
                reference,
                main: &self.main,
                names: &self.names,
            };
            let members = [MemberLink {
                canonical: 0x0200_0040,
                address: if placed.is_some() {
                    0x0200_0080
                } else {
                    0x0200_0040
                },
                extent: 0x20,
                paired: Some(0x20),
            }];
            let link = PlacedLink {
                unit: "staged-actor",
                canonical: image("resource_3bf", &self.canonical),
                placed: placed.map(|overlay| image(overlay, &self.instance)),
                direct,
                unit_symbols: &self.unit_symbols,
                declared,
                members: &members,
                keeps_unselected: false,
            };
            bind_placed_unit(&link, &self.undefined)
        }
    }
    const REGISTER: &str = r#"{"format":3,"owners":{
        "main:08000080":{"name":"Object_GetById"},
        "main:080000c0":{"name":"Object_Other"},
        "resource_3bf:02000100":{"name":"Scene_Helper"},
        "resource_389:02000200":{"name":"Scene_Helper"}}}"#;
    fn data(address: u64) -> AbsoluteSymbol {
        AbsoluteSymbol {
            address,
            kind: AbsoluteSymbolKind::Data,
        }
    }

    #[test]
    fn instance_placement_translates_calls_and_literals_across_different_overlays() {
        let placement = Placement::new(REGISTER);
        let other = placement
            .undefined
            .keys()
            .find(|name| name.starts_with("Func_"))
            .unwrap()
            .clone();
        let (bound, calls) = placement.bind(true, true, &BTreeMap::new()).unwrap();
        // The main call binds to resource_389's own import veneer, the helper
        // to its own owner of that name, and both tables to its literals.
        assert_eq!(bound["Object_GetById"], thumb(0x0200_8030));
        assert_eq!(bound["Scene_Helper"], thumb(0x0200_8200));
        assert_eq!(bound[TABLE], data(0x0200_a800));
        assert_eq!(bound[SPELLED], data(0x0200_a900));
        // A value outside the canonical image keeps the address it spells.
        assert!(!bound.contains_key("Data_03001ebc"));
        assert_eq!(calls[&other], BTreeSet::from([0x0200_81c0]));
        // The canonical link binds the same names where resource_3bf has them.
        let (canonical, calls) = placement.bind(false, false, &BTreeMap::new()).unwrap();
        assert_eq!(canonical["Object_GetById"], thumb(0x0200_8010));
        assert_eq!(canonical["Scene_Helper"], thumb(0x0200_8100));
        assert_eq!(canonical[TABLE], data(0x0200_a000));
        assert!(!canonical.contains_key(SPELLED));
        assert_eq!(calls[&other], BTreeSet::from([0x0200_8180]));
        // Declared entries win over pairing.
        let declared = BTreeMap::from([(TABLE.to_string(), data(0x0200_b000))]);
        let (bound, _) = placement.bind(true, true, &declared).unwrap();
        assert_eq!(bound[TABLE], data(0x0200_b000));
        // An address-named value inside the canonical image must pair.
        let mut unpaired = Placement::new(REGISTER);
        let value = unpaired.undefined[SPELLED];
        unpaired.undefined.insert("Data_0200a1f0".into(), value);
        let error = unpaired.bind(true, true, &BTreeMap::new()).unwrap_err();
        assert!(
            error.contains("Data_0200a1f0: no corresponding regional data address"),
            "{error}"
        );
        // A regional edition of the canonical image keeps that spelling.
        let (bound, _) = unpaired
            .bind_as(Some("resource_3bf"), false, &BTreeMap::new())
            .unwrap();
        assert!(!bound.contains_key("Data_0200a1f0"));
        assert_eq!(bound[SPELLED], data(0x0200_a900));
        assert_eq!(bound["Scene_Helper"], thumb(0x0200_8200));
    }
    #[test]
    fn selected_member_reaches_other_members_where_they_are_placed() {
        // A member linked alone keeps only its own section, so its call to
        // another member and that member's address in its pool are undefined
        // names, bound at their canonical address and paired from there.
        let mut placement = Placement::new(REGISTER);
        placement.canonical = placed_image(
            0x10,
            0x0200_0040,
            [0x0200_8100, 0x0200_8180],
            [0x0200_a000, 0x0200_8181],
        );
        placement.instance = placed_image(
            0x30,
            0x0200_0080,
            [0x0200_8200, 0x0200_81c0],
            [0x0200_a800, 0x0200_81c1],
        );
        let member = "Func_02000180";
        placement
            .undefined
            .retain(|name, _| !name.starts_with("Func_") && name != SPELLED);
        placement.undefined.insert(
            member.into(),
            SymbolUses {
                call: true,
                value: true,
            },
        );
        placement
            .unit_symbols
            .insert(member.into(), thumb(0x0200_8180));
        let (canonical, _) = placement.bind(false, false, &BTreeMap::new()).unwrap();
        assert_eq!(canonical[member], thumb(0x0200_8180));
        // An instance registers no owner of a legacy member name; pairing speaks.
        let (instance, _) = placement.bind(true, true, &BTreeMap::new()).unwrap();
        assert_eq!(instance[member], thumb(0x0200_81c0));
        let (edition, _) = placement
            .bind_as(Some("resource_3bf"), false, &BTreeMap::new())
            .unwrap();
        assert_eq!(edition[member], thumb(0x0200_81c0));
        // A member the placed code does not reach is an error, not a guess.
        placement.instance[0x88..0x8c].copy_from_slice(&[0; 4]);
        let error = placement.bind(true, true, &BTreeMap::new()).unwrap_err();
        assert!(
            error.contains("edition lacks a corresponding overlay call"),
            "{error}"
        );
    }
    #[test]
    fn selected_section_keeps_the_file_header_and_one_member() {
        let sectioned = "\t.file\t\"UNIT.C\"\n\t.code\t16\n.text\n\t.align\t2\n\
            \t.section\t.text.Func_02000030,\"ax\",%progbits\n\t.global\tFunc_02000030\nFunc_02000030:\n\tbl\tFunc_0200006c\n\t.align\t2\n\
            \t.section\t.text.Func_0200006c,\"ax\",%progbits\n\t.global\tFunc_0200006c\nFunc_0200006c:\n\tbx\tlr\n.L4:\n\t.word\tFunc_02000030\n";
        assert_eq!(
            selected_section(sectioned, "Func_0200006c").unwrap(),
            "\t.file\t\"UNIT.C\"\n\t.code\t16\n.text\n\t.align\t2\n\
            \t.section\t.text.Func_0200006c,\"ax\",%progbits\n\t.global\tFunc_0200006c\nFunc_0200006c:\n\tbx\tlr\n.L4:\n\t.word\tFunc_02000030\n"
        );
        assert_eq!(
            selected_section(sectioned, "Func_02000030").unwrap(),
            "\t.file\t\"UNIT.C\"\n\t.code\t16\n.text\n\t.align\t2\n\
            \t.section\t.text.Func_02000030,\"ax\",%progbits\n\t.global\tFunc_02000030\nFunc_02000030:\n\tbl\tFunc_0200006c\n\t.align\t2\n"
        );
        let error = selected_section(sectioned, "Func_02000244").unwrap_err();
        assert!(error.contains("no section for Func_02000244"), "{error}");
    }
    #[test]
    fn instance_binding_reaching_a_different_main_target_is_rejected() {
        // The paired call reaches a veneer to Object_Other; Object_GetById's
        // own veneer in resource_389 sits elsewhere.
        let mut placement = Placement::new(REGISTER);
        placement.instance[0x30..0x38].copy_from_slice(&veneer_word(0x0800_00c1));
        placement.instance[0x38..0x40].copy_from_slice(&veneer_word(0x0800_0081));
        let error = placement.bind(true, true, &BTreeMap::new()).unwrap_err();
        assert!(
            error.contains("Object_GetById: resource_389 binds this name to 0x02008038, but the paired site of staged-actor reaches 0x02008030"),
            "{error}"
        );
        // Without any veneer to Object_GetById the instance cannot call it.
        placement.instance[0x38..0x40].fill(0);
        let error = placement.bind(true, true, &BTreeMap::new()).unwrap_err();
        assert!(
            error.contains("no import veneer in resource_389 reaches main:08000080"),
            "{error}"
        );
        // A regional edition has no English register to bind by name, so only
        // pairing speaks there.
        let (bound, _) = placement.bind(true, false, &BTreeMap::new()).unwrap();
        assert_eq!(bound["Object_GetById"], thumb(0x0200_8030));
    }
    #[test]
    fn instance_members_pair_over_the_canonical_extent_unless_their_code_differs() {
        use crate::compiler::translation_units::fixture::{Repository, REDRAW};
        use crate::compiler::translation_units::EditionOwner;
        let repository = Repository::new();
        let mut unit = repository.load().unwrap().units.remove(0);
        let link = |unit: &TranslationUnit, image, edition, extent| {
            member_link(unit, image, edition, 0x0200_08c0, 0x0200_0ba4, extent).paired
        };
        // An instance owns the canonical code, optionally with its alignment halfword.
        assert_eq!(link(&unit, "resource_39b", None, 284), Some(284));
        assert_eq!(link(&unit, "resource_39b", None, 286), Some(284));
        assert_eq!(link(&unit, "resource_39b", Some("ja"), 286), Some(284));
        for extent in [282, 288] {
            assert_eq!(link(&unit, "resource_39b", None, extent), None);
        }
        // The canonical image's regional layouts pair only at the same extent.
        assert_eq!(link(&unit, "resource_3bf", Some("ja"), 284), Some(284));
        assert_eq!(link(&unit, "resource_3bf", Some("ja"), 286), None);
        // A source variant is different code wherever it is linked.
        let variant = EditionOwner {
            address: 0x0200_0bb4,
            extent: 284,
            source_variant: true,
        };
        let layout = unit
            .instances
            .get_mut("resource_39b")
            .unwrap()
            .editions
            .get_mut("ja")
            .unwrap();
        layout.owners.insert(REDRAW.into(), variant);
        assert_eq!(link(&unit, "resource_39b", Some("ja"), 284), None);
        assert_eq!(link(&unit, "resource_39b", Some("de"), 284), Some(284));
        assert_eq!(
            member_link(&unit, "resource_39b", None, 0x0200_0ba0, 0x0200_0ba4, 284).paired,
            None
        );
    }
    #[test]
    fn instance_register_name_mismatch_is_rejected() {
        // resource_389 registers no Scene_Helper at all.
        let missing = REGISTER.replace(
            r#""resource_389:02000200":{"name":"Scene_Helper"}"#,
            r#""resource_389:02000200":{"name":"Scene_Unrelated"}"#,
        );
        let error = Placement::new(&missing)
            .bind(true, true, &BTreeMap::new())
            .unwrap_err();
        assert!(
            error.contains("Scene_Helper: resource_389 registers no owner of that name, so its instance of staged-actor would reach a different function"),
            "{error}"
        );
        // resource_389's Scene_Helper is not the function the paired call reaches.
        let elsewhere = REGISTER.replace("resource_389:02000200", "resource_389:02000240");
        let error = Placement::new(&elsewhere)
            .bind(true, true, &BTreeMap::new())
            .unwrap_err();
        assert!(
            error.contains("Scene_Helper: resource_389 binds this name to 0x02008240, but the paired site of staged-actor reaches 0x02008200"),
            "{error}"
        );
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
