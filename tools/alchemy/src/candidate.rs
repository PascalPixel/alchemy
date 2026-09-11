//! Compile candidate C, link it at its ROM address, and return both byte spans.
use crate::compiler::plan::{basename, extname};
use crate::compiler::plan::{source_to_assembly_plan, SourceToAssemblyPlanOptions};
use crate::compiler::routing::{root, CompilerTarget};
use crate::compiler::source_paths::{SourceOwner, SourcePaths};
use crate::compiler::symbols::{external_symbol, ExternalSymbol, CALL_VIA_BASE};
use crate::compiler::translation_units::{AbsoluteSymbol, AbsoluteSymbolKind};
use psynergy::process::run;
use std::collections::BTreeMap;
use std::path::{Path, PathBuf};
pub const ROM_BASE: f64 = 0x0800_0000 as f64;
#[derive(Debug, Clone, Default)]
pub struct CandidateCompilerConfiguration {
    pub reference_symbols: bool,
    pub absolute_symbols: BTreeMap<String, AbsoluteSymbol>,
    pub call_via_base: Option<u64>,
    /// Complete reference extent for an overlay owner, independent of candidate size.
    pub overlay_extent: Option<usize>,
    /// Compiled owner symbol when it differs from the compare address. GCC
    /// 2.96's codegen is name-dependent (symbol hashes steer allocation
    /// tie-breaks), so cross-edition scoring compiles the owner under its
    /// canonical name and links at the edition's address instead of renaming
    /// the function per edition.
    pub owner_symbol: Option<String>,
}
#[derive(Debug, Clone)]
pub struct Verification {
    pub actual: Vec<u8>,
    pub expected: Vec<u8>,
}
pub fn source_stem(path: &str) -> String {
    let base = basename(path);
    let ext = extname(path);
    base.strip_suffix(ext)
        .filter(|stem| !ext.is_empty() && !stem.is_empty())
        .unwrap_or(base)
        .to_string()
}
pub fn assemble(assembly: &str, object: &str) -> Result<(), String> {
    run(
        &crate::compiler::routing::assembly_command(assembly, object),
        root(),
    )
    .map(drop)
}
pub fn copy_text(object: &str, binary: &str) -> Result<(), String> {
    run(
        &[
            "arm-none-eabi-objcopy",
            "-O",
            "binary",
            "-j",
            ".text",
            object,
            binary,
        ],
        root(),
    )
    .map(drop)
}
fn parse_hex(value: &str) -> Result<u64, String> {
    if value.is_empty() || !value.bytes().all(|byte| byte.is_ascii_hexdigit()) {
        return Err(format!("invalid hexadecimal value: {value}"));
    }
    u64::from_str_radix(value, 16).map_err(|error| error.to_string())
}

fn byte_span(data: &[u8], offset: u64, size: u64) -> Result<Vec<u8>, String> {
    let end = offset.checked_add(size).ok_or("byte span overflows")?;
    if size == 0 || end > data.len() as u64 {
        return Err(format!(
            "byte span 0x{offset:x}+0x{size:x} is outside {} bytes",
            data.len()
        ));
    }
    Ok(data[offset as usize..end as usize].to_vec())
}
pub fn compile_to_assembly(
    source: &str,
    routing_source: &str,
    output_directory: &str,
    extra_compiler_flags: &[String],
    compiler: CompilerTarget,
) -> Result<String, String> {
    let stem = source_stem(source);
    std::fs::create_dir_all(output_directory)
        .map_err(|error| format!("{output_directory}: {error}"))?;
    let assembly = Path::new(output_directory)
        .join(format!("{stem}.s"))
        .to_string_lossy()
        .into_owned();
    compile_source(
        source,
        routing_source,
        &assembly,
        extra_compiler_flags,
        compiler,
        root(),
    )?;
    Ok(assembly)
}
pub fn compile_source(
    source: &str,
    routing_source: &str,
    assembly: &str,
    extra_flags: &[String],
    compiler: CompilerTarget,
    cwd: &Path,
) -> Result<(), String> {
    let mut options = SourceToAssemblyPlanOptions::new(compiler, routing_source, source, assembly);
    let bindings = resolve_against_cwd(assembly, cwd).with_extension("bindings.h");
    write(
        &bindings.to_string_lossy(),
        production_symbol_bindings(root(), routing_source, source, compiler)?.as_bytes(),
    )?;
    options.preprocessor_flags = vec!["-include".into(), bindings.to_string_lossy().into_owned()];
    options.support_flags = extra_flags.to_vec();
    options.preprocessed_output = Some(
        Path::new(assembly)
            .with_extension("i")
            .to_string_lossy()
            .into_owned(),
    );
    for command in source_to_assembly_plan(&options)? {
        run(&command, cwd)?;
    }
    Ok(())
}
/// Match production's register-derived compiler names, including overlay scope.
pub fn source_symbol_bindings(
    repository: &Path,
    routing_source: &str,
    compiler: CompilerTarget,
) -> Result<String, String> {
    let overlay = SourceOwner::from_legacy_stem(&source_stem(routing_source))
        .and_then(SourceOwner::overlay_id);
    Ok(SourcePaths::load_for_game(repository, compiler.as_str())?
        .symbol_bindings(overlay.as_deref()))
}
/// Register names plus the per-source address map for one production file.
pub fn production_symbol_bindings(
    repository: &Path,
    routing_source: &str,
    production_source: &str,
    compiler: CompilerTarget,
) -> Result<String, String> {
    let register = source_symbol_bindings(repository, routing_source, compiler)?;
    crate::compiler::source_bindings::production_bindings(
        repository,
        &register,
        Some(Path::new(production_source)),
    )
}
fn resolve_against_cwd(path: &str, cwd: &Path) -> PathBuf {
    let path = Path::new(path);
    if path.is_absolute() {
        path.to_path_buf()
    } else {
        cwd.join(path)
    }
}
#[allow(clippy::too_many_arguments)]
pub fn verify_candidate_owned_routed(
    source: &str,
    routing_source: &str,
    owner_stem: &str,
    rom: &[u8],
    output_directory: &str,
    extra_compiler_flags: &[String],
    image_base: f64,
    compiler: CompilerTarget,
    configuration: &CandidateCompilerConfiguration,
) -> Result<Verification, String> {
    verify_candidate_owned_routed_with_object(
        source,
        routing_source,
        owner_stem,
        rom,
        output_directory,
        extra_compiler_flags,
        image_base,
        compiler,
        configuration,
        None,
    )
}
#[allow(clippy::too_many_arguments)]
pub fn verify_candidate_owned_routed_with_object(
    source: &str,
    routing_source: &str,
    owner_stem: &str,
    rom: &[u8],
    output_directory: &str,
    extra_compiler_flags: &[String],
    image_base: f64,
    compiler: CompilerTarget,
    configuration: &CandidateCompilerConfiguration,
    precompiled_object: Option<&str>,
) -> Result<Verification, String> {
    let actual = link_candidate_owned_routed_with_object(
        source,
        routing_source,
        owner_stem,
        rom,
        output_directory,
        extra_compiler_flags,
        image_base,
        compiler,
        configuration,
        precompiled_object,
    )?;
    let offset = parse_hex(owner_stem)?
        .checked_sub(image_base as u64)
        .ok_or("owner precedes reference image")?;
    let expected = byte_span(rom, offset, actual.len() as u64)?;
    Ok(Verification { actual, expected })
}

/// Compile and link an owner without claiming reference equality. Reference
/// bytes are used only by reference-symbol inference and overlay relocation.
#[allow(clippy::too_many_arguments)]
pub fn link_candidate_owned_routed_with_object(
    source: &str,
    routing_source: &str,
    owner_stem: &str,
    rom: &[u8],
    output_directory: &str,
    extra_compiler_flags: &[String],
    image_base: f64,
    compiler: CompilerTarget,
    configuration: &CandidateCompilerConfiguration,
    precompiled_object: Option<&str>,
) -> Result<Vec<u8>, String> {
    if configuration.overlay_extent.is_some() && configuration.reference_symbols {
        return Err("overlay calls require stable reference bindings, not candidate-position symbol inference".into());
    }
    if !image_base.is_finite()
        || image_base.fract() != 0.0
        || !(0.0..=u32::MAX as f64).contains(&image_base)
    {
        return Err("image base must be a 32-bit unsigned address".into());
    }
    let stem = owner_stem.to_string();
    let address = parse_hex(&stem)?;
    let canonical_symbol = configuration
        .owner_symbol
        .clone()
        .unwrap_or_else(|| format!("Func_{address:08x}"));
    let short_symbol = if configuration.owner_symbol.is_some() {
        canonical_symbol.clone()
    } else {
        format!("Func_{}", format!("{address:08x}").trim_start_matches('0'))
    };
    let out = Path::new(output_directory);
    let path = |suffix: &str| {
        out.join(format!("{stem}{suffix}"))
            .to_string_lossy()
            .into_owned()
    };
    let assembly = path(".s");
    let object = precompiled_object
        .map(str::to_owned)
        .unwrap_or_else(|| path(".o"));
    let symbols_source = path(".symbols.s");
    let symbols_object = path(".symbols.o");
    let canonical_object = path(".canonical.o");
    let elf = path(".elf");
    let binary = path(".bin");
    let cwd = root();
    if precompiled_object.is_none() {
        compile_source(
            source,
            routing_source,
            &assembly,
            extra_compiler_flags,
            compiler,
            root(),
        )?;
        run(
            &crate::compiler::routing::compiler_assembly_command(&assembly, &object),
            cwd,
        )?;
    }
    // GCC 2.96 keeps observable state across functions, so some owners need
    // their original translation-unit context. Link the section so the
    // requested symbol, rather than the start of .text, lands at its address.
    let object_symbols = run(&["arm-none-eabi-nm", "-S", &object], cwd)?;
    let (symbol, object_fields) = [&canonical_symbol, &short_symbol]
        .into_iter()
        .find_map(|symbol| {
            symbol_fields(&object_symbols, symbol).map(|fields| (symbol.as_str(), fields))
        })
        .ok_or_else(|| format!("missing object symbol: {canonical_symbol}"))?;
    let owner_section_offset = parse_hex(
        object_fields
            .first()
            .ok_or("missing object symbol address")?,
    )?;
    let owner_size = parse_hex(object_fields.get(1).ok_or("missing object symbol size")?)?;
    let runtime_address = if configuration.overlay_extent.is_some() {
        address
            + u64::from(
                crate::compiler::overlay::RUNTIME_BASE - crate::compiler::overlay::RESOURCE_BASE,
            )
    } else {
        address
    };
    let link_address = runtime_address
        .checked_sub(owner_section_offset)
        .ok_or_else(|| format!("owner symbol offset exceeds link address: {symbol}"))?;
    let owner_offset =
        usize::try_from(owner_section_offset).map_err(|_| "owner object offset is too large")?;
    let owner_size = usize::try_from(owner_size).map_err(|_| "owner object size is too large")?;
    let peers = configuration
        .absolute_symbols
        .keys()
        .filter(|name| **name != canonical_symbol && **name != short_symbol)
        .filter(|name| symbol_fields(&object_symbols, name).is_some())
        .collect::<Vec<_>>();
    let link_object = if peers.is_empty() {
        &object
    } else {
        let mut command = vec!["arm-none-eabi-objcopy".to_string()];
        command.extend(peers.iter().map(|name| format!("--weaken-symbol={name}")));
        command.extend([object.clone(), canonical_object.clone()]);
        run(&command, cwd)?;
        &canonical_object
    };
    let owner_relocations = object_relocations(link_object, owner_offset, owner_size)?;
    let overlay_calls = configuration
        .overlay_extent
        .map(|extent| {
            let offset = address
                .checked_sub(u64::from(crate::compiler::overlay::RESOURCE_BASE))
                .ok_or("overlay owner precedes resource base")? as usize;
            crate::compiler::overlay::call_symbols(rom, offset, extent)
        })
        .transpose()?;
    let call_via_base = configuration.call_via_base.unwrap_or(CALL_VIA_BASE);
    let mut names: Vec<String> = Vec::new();
    let undefined_symbols = run(&["arm-none-eabi-nm", "-u", link_object], cwd)?;
    for external in undefined_symbols
        .lines()
        .filter_map(|line| line.split_whitespace().last())
    {
        // Grouped translation units can contain unrelated imports belonging
        // only to neighboring functions. They cannot affect this owner.
        if !owner_relocations.contains_key(external) {
            continue;
        }
        if !configuration.absolute_symbols.contains_key(external)
            && external_symbol(external, call_via_base).is_none()
        {
            if configuration.reference_symbols {
                return Err(format!("reference-symbol inference requires an address-encoded or manifest absolute symbol: {external}"));
            }
            return Err(format!("unsupported external symbol: {external}"));
        }
        names.push(external.to_string());
    }
    names.extend(
        peers
            .into_iter()
            .filter(|name| owner_relocations.contains_key(name.as_str()))
            .cloned(),
    );
    names.sort();
    names.dedup();
    let mut symbols_text = String::from(".syntax unified\n.thumb\n");
    let inferred = names
        .iter()
        .filter(|name| !configuration.absolute_symbols.contains_key(*name))
        .cloned()
        .collect::<Vec<_>>();
    let resolved = if configuration.reference_symbols && !inferred.is_empty() {
        derive_reference_symbols(
            link_object,
            owner_offset,
            owner_size,
            &owner_relocations,
            &inferred,
            rom,
            address,
            image_base,
        )?
    } else {
        BTreeMap::new()
    };
    for name in &names {
        let (address, directive) = if let Some(symbol) = configuration.absolute_symbols.get(name) {
            (symbol.address, absolute_symbol_directive(symbol.kind))
        } else if let Some(calls) = &overlay_calls {
            let is_call = owner_relocations
                .get(name)
                .is_some_and(|sites| sites.iter().any(|site| site.kind == "R_ARM_THM_CALL"));
            let symbol = crate::compiler::overlay::external(name, is_call, rom, calls)?;
            (symbol.address, absolute_symbol_directive(symbol.kind))
        } else {
            let symbol = resolved
                .get(name)
                .copied()
                .or_else(|| external_symbol(name, call_via_base))
                .ok_or_else(|| format!("unsupported external symbol: {name}"))?;
            (
                symbol.address,
                if symbol.thumb { ".thumb_set" } else { ".set" },
            )
        };
        symbols_text.push_str(&format!(
            ".global {name}\n{directive} {name}, 0x{address:08x}\n"
        ));
    }
    write(&symbols_source, symbols_text.as_bytes())?;
    assemble(&symbols_source, &symbols_object)?;
    run(
        &[
            "arm-none-eabi-ld",
            "--unresolved-symbols=ignore-all",
            &format!("-Ttext=0x{link_address:08x}"),
            "-e",
            symbol,
            "-o",
            &elf,
            link_object,
            &symbols_object,
        ],
        cwd,
    )?;
    copy_text(&elf, &binary)?;
    let symbols = run(&["arm-none-eabi-nm", "-S", &elf], cwd)?;
    let fields = symbol_fields(&symbols, symbol)
        .ok_or_else(|| format!("missing linked symbol: {symbol}"))?;
    let binary_bytes = std::fs::read(&binary).map_err(|error| format!("{binary}: {error}"))?;
    let size = parse_hex(fields[1])?;
    let linked_symbol_address = parse_hex(fields.first().ok_or("missing linked symbol address")?)?;
    let binary_offset = linked_symbol_address
        .checked_sub(link_address)
        .ok_or("linked symbol precedes its section")?;
    let actual = byte_span(&binary_bytes, binary_offset, size)?;
    let offset = address
        .checked_sub(image_base as u64)
        .ok_or("owner precedes reference image")?;
    let actual = if configuration.overlay_extent.is_some() {
        crate::compiler::overlay::encode(&actual, offset as usize)?
    } else {
        actual
    };
    Ok(actual)
}
fn symbol_fields<'a>(listing: &'a str, symbol: &str) -> Option<Vec<&'a str>> {
    listing
        .lines()
        .map(|line| line.split_whitespace().collect::<Vec<_>>())
        .find(|fields| {
            fields.len() == 4
                && fields[3] == symbol
                && fields[2] != "U"
                && u64::from_str_radix(fields[0], 16).is_ok()
                && u64::from_str_radix(fields[1], 16).is_ok()
        })
}
fn absolute_symbol_directive(kind: AbsoluteSymbolKind) -> &'static str {
    if kind == AbsoluteSymbolKind::Thumb {
        ".thumb_set"
    } else {
        ".set"
    }
}
#[derive(Clone, Debug)]
struct ReferenceRelocation {
    offset: usize,
    kind: String,
}
fn validate_reference_topology(
    object_text: &[u8],
    owner_offset: usize,
    owner_size: usize,
    rom: &[u8],
    rom_start: usize,
    relocations: &BTreeMap<String, Vec<ReferenceRelocation>>,
) -> Result<(), String> {
    let object = object_text
        .get(owner_offset..owner_offset + owner_size)
        .ok_or("candidate owner extends past object text")?;
    let reference = rom
        .get(rom_start..rom_start + owner_size)
        .ok_or("candidate owner extends past reference image")?;
    let relocated = |offset| {
        relocations.values().flatten().any(|site| {
            matches!(site.kind.as_str(), "R_ARM_ABS32" | "R_ARM_THM_CALL")
                && (site.offset..site.offset.saturating_add(4)).contains(&offset)
        })
    };
    if let Some(offset) =
        (0..owner_size).find(|offset| !relocated(*offset) && object[*offset] != reference[*offset])
    {
        return Err(format!("reference-symbol inference requires relocation-aligned code; non-relocation byte differs at owner offset 0x{offset:x}; use manifest absolute symbols until the core aligns"));
    }
    Ok(())
}
#[allow(clippy::too_many_arguments)]
fn derive_reference_symbols(
    object: &str,
    owner_offset: usize,
    owner_size: usize,
    relocations: &BTreeMap<String, Vec<ReferenceRelocation>>,
    names: &[String],
    rom: &[u8],
    address: u64,
    image_base: f64,
) -> Result<BTreeMap<String, ExternalSymbol>, String> {
    let image_base = image_base as u64;
    let rom_start = address
        .checked_sub(image_base)
        .ok_or("owner address precedes image base")?;
    let rom_start = usize::try_from(rom_start).map_err(|_| "ROM offset is too large")?;
    let object_text_path = format!("{object}.text.bin");
    copy_text(object, &object_text_path)?;
    let object_text =
        std::fs::read(&object_text_path).map_err(|error| format!("{object_text_path}: {error}"))?;
    validate_reference_topology(
        &object_text,
        owner_offset,
        owner_size,
        rom,
        rom_start,
        relocations,
    )?;
    let mut resolved = BTreeMap::new();
    for name in names {
        let sites = relocations
            .get(name)
            .ok_or_else(|| format!("no reference relocation for external symbol: {name}"))?;
        let known = external_symbol(name, CALL_VIA_BASE);
        let thumb = known
            .map(|symbol| symbol.thumb)
            .unwrap_or_else(|| sites.iter().any(|site| site.kind == "R_ARM_THM_CALL"));
        let mut value = None;
        for site in sites {
            let site_value = match site.kind.as_str() {
                "R_ARM_THM_CALL" => thumb_bl_target(rom, rom_start, address, site.offset)?,
                "R_ARM_ABS32" => {
                    let reference = read_word(rom, rom_start + site.offset, name, "reference")?;
                    let addend = read_word(
                        &object_text,
                        owner_offset + site.offset,
                        name,
                        "object addend",
                    )?;
                    let address = reference.wrapping_sub(addend) as u64;
                    if thumb {
                        address & !1
                    } else {
                        address
                    }
                }
                kind => {
                    return Err(format!(
                        "unsupported reference relocation {kind} for {name}"
                    ))
                }
            };
            if let Some(previous) = value {
                if previous != site_value {
                    return Err(format!(
                        "{name} resolves inconsistently: 0x{previous:08x} and 0x{site_value:08x}"
                    ));
                }
            } else {
                value = Some(site_value);
            }
        }
        resolved.insert(
            name.clone(),
            ExternalSymbol {
                address: value.ok_or_else(|| format!("no reference relocation for {name}"))?,
                thumb,
            },
        );
    }
    Ok(resolved)
}
fn object_relocations(
    object: &str,
    owner_offset: usize,
    owner_size: usize,
) -> Result<BTreeMap<String, Vec<ReferenceRelocation>>, String> {
    let output = run(&["arm-none-eabi-objdump", "-r", object], root())?;
    let mut relocations = BTreeMap::<String, Vec<ReferenceRelocation>>::new();
    for line in output.lines() {
        let fields = line.split_whitespace().collect::<Vec<_>>();
        if fields.len() < 3 || !fields[1].starts_with("R_ARM_") {
            continue;
        }
        let raw_offset = usize::from_str_radix(fields[0], 16)
            .map_err(|error| format!("invalid relocation offset {}: {error}", fields[0]))?;
        let Some(offset) = raw_offset.checked_sub(owner_offset) else {
            continue;
        };
        if offset >= owner_size {
            continue;
        }
        let symbol = fields[2].split('+').next().unwrap_or(fields[2]);
        relocations
            .entry(symbol.to_string())
            .or_default()
            .push(ReferenceRelocation {
                offset,
                kind: fields[1].to_string(),
            });
    }
    Ok(relocations)
}
fn read_word(data: &[u8], offset: usize, symbol: &str, label: &str) -> Result<u32, String> {
    let bytes = data
        .get(offset..offset + 4)
        .ok_or_else(|| format!("{label} relocation for {symbol} extends past its image"))?;
    Ok(u32::from_le_bytes(
        bytes.try_into().expect("four-byte slice"),
    ))
}
fn thumb_bl_target(
    rom: &[u8],
    rom_start: usize,
    address: u64,
    offset: usize,
) -> Result<u64, String> {
    let bytes = rom
        .get(rom_start + offset..rom_start + offset + 4)
        .ok_or_else(|| format!("call at 0x{offset:x} extends past the reference image"))?;
    let displacement = psynergy::thumb::bl_displacement(bytes)
        .ok_or_else(|| format!("reference relocation at 0x{offset:x} is not a Thumb BL"))?;
    let pc = address as i64 + offset as i64 + 4;
    u64::try_from(pc + i64::from(displacement))
        .map_err(|_| format!("call at 0x{offset:x} is below ROM"))
}
pub(crate) fn write(path: &str, bytes: &[u8]) -> Result<(), String> {
    std::fs::write(path, bytes).map_err(|error| format!("{path}: {error}"))
}
#[cfg(test)]
mod reference_symbol_tests {
    use super::*;

    /// Preprocessed source for resource_37a:02001be8. Under address-space
    /// randomisation the approved cc1 compiled this to two different byte
    /// strings across thirty runs (26 to 4), because its CSE tables hash the
    /// heap addresses of symbol names and labels. The tool executor now
    /// spawns compilers with randomisation off; this holds that invariant.
    const ADDRESS_SENSITIVE_INPUT: &str = "\
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
extern u8 Value_0000100d[];
extern u8 Value_0000102b[];
extern u8 Value_000011b4[];
extern u8 Data_03001ebc[];
void Func_02004252();
void Func_02004270();
void Func_02004606();
void Func_02004612();
void Func_02004626();
void Func_0200462c();
void Func_02004630();
void Func_02004636();
void Func_02004654();
void Func_0200465e();
void Func_02004662();
void Func_02004682();
void Func_0200468c();
void Func_02004698_a();
void Func_02004698_b();
void Func_0200469a();
void Func_020046a8();
void Func_020046aa();
void Func_020046e2();
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;
    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}
void FieldScene_RunActorPositionTransition(void)
{
    u32 i;
    s32 record;
    Func_020046e2(21);
    Call3(Func_02004606, 0, 0x178, 184);
    Func_02004626(0, 0);
    Call3(Func_0200462c, 16, 0x1780000, 0xb80000);
    Call3(Func_02004612, 16, 0x10000, 0x8000);
    Call3(Func_02004636, 16, 0x188, 168);
    Call3(Func_020046aa, 16, 0x8000, 30);
    Func_02004662(16, 1);
    Call1(Func_02004698_a, (s32)Value_0000102b);
    Func_02004682(16, 4, 30);
    Func_02004252(16, 6);
    Func_0200469a(0, 2);
    Func_02004630(6);
    Func_02004698_b(16, 3);
    Func_02004270(16, 6);
    Call3(Func_0200468c, 16, 0x178, 184);
    Call3(Func_020046a8, 16, 0x6480000, 0x6480000);
    Func_0200465e(4);
    Call1(Func_02004654, 0x811);
}
";

    #[test]
    fn repeated_compiler_runs_produce_one_byte_string() {
        let cc1 = crate::compiler::routing::bundle().join("cc1");
        if !cc1.exists() {
            return;
        }
        let work = tempfile::tempdir().unwrap();
        let input = work.path().join("in.i");
        std::fs::write(&input, ADDRESS_SENSITIVE_INPUT).unwrap();
        let mut outputs = std::collections::BTreeSet::new();
        for index in 0..12 {
            let assembly = work.path().join(format!("run{index}.s"));
            let command = [
                cc1.to_string_lossy().into_owned(),
                "-O2".into(),
                "-mthumb".into(),
                "-mthumb-interwork".into(),
                "-mcpu=arm7tdmi".into(),
                "-fno-builtin".into(),
                "-ffreestanding".into(),
                "-fcall-used-r4".into(),
                "-quiet".into(),
                "-o".into(),
                assembly.to_string_lossy().into_owned(),
                input.to_string_lossy().into_owned(),
            ];
            run(&command, work.path()).unwrap();
            outputs.insert(std::fs::read(&assembly).unwrap());
        }
        assert_eq!(
            outputs.len(),
            1,
            "the compiler produced {} different outputs for one input",
            outputs.len()
        );
    }

    #[test]
    fn linking_without_a_reference_is_not_verification() {
        let work = tempfile::tempdir().unwrap();
        let source = work.path().join("08000000.c");
        std::fs::write(&source, "int Func_08000000(void) { return 1; }\n").unwrap();
        let source = source.to_str().unwrap();
        let output = work.path().to_str().unwrap();
        let config = CandidateCompilerConfiguration::default();
        let linked = link_candidate_owned_routed_with_object(
            source,
            source,
            "08000000",
            &[],
            output,
            &[],
            ROM_BASE,
            CompilerTarget::Gs1,
            &config,
            None,
        )
        .unwrap();
        assert!(!linked.is_empty());
        let verify = |reference: &[u8]| {
            verify_candidate_owned_routed_with_object(
                source,
                source,
                "08000000",
                reference,
                output,
                &[],
                ROM_BASE,
                CompilerTarget::Gs1,
                &config,
                None,
            )
        };
        assert!(verify(&[]).unwrap_err().contains("outside 0 bytes"));
        let verified = verify(&linked).unwrap();
        assert_eq!(verified.actual, verified.expected);
    }

    #[test]
    fn hexadecimal_fields_accept_only_complete_ascii_digits() {
        for value in ["0800ABCD", "0800abcd"] {
            assert_eq!(parse_hex(value).unwrap(), 0x0800_abcd);
        }
        assert_eq!(parse_hex("F").unwrap(), 15);
        for value in ["", "0x10", "１２", "١٢", " 10", "10\n"] {
            assert!(parse_hex(value).is_err());
        }
    }

    #[test]
    fn comparison_spans_must_be_complete_and_nonempty() {
        let data = [0, 1, 2, 3];
        assert_eq!(byte_span(&data, 1, 3).unwrap(), [1, 2, 3]);
        for (offset, size) in [(0, 0), (4, 1), (3, 2), (u64::MAX, 1)] {
            assert!(byte_span(&data, offset, size).is_err());
        }
    }
    #[test]
    fn owner_symbols_require_a_defined_sized_nm_record() {
        let undefined = "         U Func_0808e5d8\n0808e5d8 A Func_0808e5d8\n";
        assert!(symbol_fields(undefined, "Func_0808e5d8").is_none());
        let listing = format!("{undefined}000003a0 000000a8 T Func_0808e5d8\n");
        assert_eq!(
            symbol_fields(&listing, "Func_0808e5d8").unwrap(),
            ["000003a0", "000000a8", "T", "Func_0808e5d8"]
        );
    }
    #[test]
    fn candidate_bindings_use_the_game_and_overlay_register_namespace() {
        let root = std::env::temp_dir().join(format!("candidate-bindings-{}", std::process::id()));
        for game in ["gs1", "gs2"] {
            std::fs::create_dir_all(root.join("games").join(game)).unwrap();
            std::fs::write(
                root.join("games").join(game).join("source-paths.json"),
                format!(r#"{{"format":3,"owners":{{"main:08001234":{{"name":"{game}_Main"}},"resource_380:02000100":{{"name":"Scene_Run"}},"resource_381:02000200":{{"name":"Scene_Run"}}}}}}"#),
            ).unwrap();
        }
        assert_eq!(
            source_symbol_bindings(&root, "games/gs1/src/08001234.c", CompilerTarget::Gs1).unwrap(),
            "#define gs1_Main Func_08001234\n"
        );
        assert_eq!(
            source_symbol_bindings(&root, "games/gs2/src/08001234.c", CompilerTarget::Gs2).unwrap(),
            "#define gs2_Main Func_08001234\n"
        );
        assert_eq!(
            source_symbol_bindings(
                &root,
                "games/gs1/src/resource_380_c_02000100.c",
                CompilerTarget::Gs1
            )
            .unwrap(),
            "#define Scene_Run Func_02000100\n"
        );
        std::fs::remove_dir_all(root).unwrap();
    }
    #[test]
    fn decodes_a_forward_thumb_call_at_the_owner_address() {
        let rom = [0x00, 0xf0, 0x18, 0xf9];
        assert_eq!(
            thumb_bl_target(&rom, 0, 0x0800_1000, 0).unwrap(),
            0x0800_1234
        );
    }
    #[test]
    fn rejects_a_non_call_reference_site() {
        let error = thumb_bl_target(&[0, 0, 0, 0], 0, 0x0800_1000, 0).unwrap_err();
        assert!(error.contains("not a Thumb BL"));
    }
    #[test]
    fn absolute_symbol_kind_not_name_selects_thumb_state() {
        assert_eq!(
            absolute_symbol_directive(AbsoluteSymbolKind::Thumb),
            ".thumb_set"
        );
        assert_eq!(absolute_symbol_directive(AbsoluteSymbolKind::Data), ".set");
        assert_eq!(absolute_symbol_directive(AbsoluteSymbolKind::Arm), ".set");
    }
    #[test]
    fn reference_inference_allows_relocated_targets_but_rejects_core_differences() {
        let relocations = BTreeMap::from([(
            "Data_02000000".to_string(),
            vec![ReferenceRelocation {
                offset: 4,
                kind: "R_ARM_ABS32".to_string(),
            }],
        )]);
        let object = [0x09, 0x48, 0x70, 0x47, 0, 0, 0, 0];
        let reference = [0x09, 0x48, 0x70, 0x47, 1, 2, 3, 4];
        assert!(
            validate_reference_topology(&object, 0, object.len(), &reference, 0, &relocations)
                .is_ok()
        );
        let mut swapped = reference;
        swapped[1] = 0x49;
        let error =
            validate_reference_topology(&object, 0, object.len(), &swapped, 0, &relocations)
                .unwrap_err();
        assert!(error.contains("non-relocation byte differs at owner offset 0x1"));
    }
}
