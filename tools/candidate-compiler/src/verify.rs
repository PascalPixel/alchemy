//! Compile candidate C, link it at its ROM address, and return both byte spans.

use std::collections::BTreeMap;
use std::path::Path;
use std::process::Command;

use compiler_core::nodepath::{basename, extname};
use compiler_core::plan::{
    source_to_assembly_plan, CompilerFamily, CompilerFlagMutations, SourceToAssemblyPlanOptions,
};
use compiler_core::routing::{root, CompilerTarget};
use compiler_core::translation_units::{AbsoluteSymbol, AbsoluteSymbolKind};
use compiler_core::{external_symbol, ExternalSymbol, CALL_VIA_BASE};

use crate::jsnum::{hex8, parse_hex};

pub const ROM_BASE: f64 = 0x0800_0000 as f64;

pub type CandidateCompilerFamily = CompilerFamily;

#[derive(Debug, Clone, Default)]
pub struct CandidateCompilerConfiguration {
    pub family: Option<CandidateCompilerFamily>,
    pub add_flags: Vec<String>,
    pub remove_flags: Vec<String>,
    pub reference_symbols: bool,
    pub absolute_symbols: BTreeMap<String, AbsoluteSymbol>,
    pub call_via_base: Option<u64>,
    pub label_word_bias: Option<u64>,
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

pub fn run<S: AsRef<str>>(command: &[S], cwd: &Path) -> Result<String, String> {
    let program = command
        .first()
        .ok_or_else(|| "run: empty command".to_string())?
        .as_ref();
    let output = Command::new(program)
        .args(command[1..].iter().map(|argument| argument.as_ref()))
        .current_dir(cwd)
        .output()
        .map_err(|error| format!("{}: {error}", basename(program)))?;
    let stdout = String::from_utf8_lossy(&output.stdout).into_owned();
    if output.status.success() {
        return Ok(stdout);
    }
    let stderr = String::from_utf8_lossy(&output.stderr).into_owned();
    let detail_source = if stderr.is_empty() { &stdout } else { &stderr };
    let detail = detail_source.trim();
    let name = basename(program);
    if detail.is_empty() {
        Err(format!("{name} failed"))
    } else {
        Err(format!("{name} failed: {detail}"))
    }
}

fn assemble(assembly: &str, object: &str) -> Result<(), String> {
    run(
        &[
            "arm-none-eabi-as",
            "-mcpu=arm7tdmi",
            "-mthumb-interwork",
            "-o",
            object,
            assembly,
        ],
        root(),
    )
    .map(drop)
}

fn copy_text(object: &str, binary: &str) -> Result<(), String> {
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

pub fn js_subarray(data: &[u8], begin: f64, end: f64) -> Vec<u8> {
    let len = data.len() as f64;
    let resolve = |relative: f64| -> usize {
        let value = if relative.is_nan() {
            0.0
        } else {
            relative.trunc()
        };
        (if value < 0.0 { len + value } else { value }).clamp(0.0, len) as usize
    };
    let start = resolve(begin);
    let stop = resolve(end);
    data.get(start..stop).unwrap_or_default().to_vec()
}

pub fn compile_to_assembly(
    source: &str,
    routing_source: &str,
    output_directory: &str,
    extra_compiler_flags: &[String],
    compiler: CompilerTarget,
    configuration: &CandidateCompilerConfiguration,
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
        configuration,
    )?;
    Ok(assembly)
}

fn compile_source(
    source: &str,
    routing_source: &str,
    assembly: &str,
    extra_flags: &[String],
    compiler: CompilerTarget,
    configuration: &CandidateCompilerConfiguration,
) -> Result<(), String> {
    let mut options = SourceToAssemblyPlanOptions::new(compiler, routing_source, source, assembly);
    options.family = configuration.family;
    let mut add_flags = extra_flags.to_vec();
    add_flags.extend(configuration.add_flags.iter().cloned());
    options.flags = Some(CompilerFlagMutations {
        add_flags,
        remove_flags: configuration.remove_flags.clone(),
    });
    options.preprocessed_output = Some(
        Path::new(assembly)
            .with_extension("i")
            .to_string_lossy()
            .into_owned(),
    );
    for step in source_to_assembly_plan(&options)?.steps {
        run(&step.command, root())?;
    }
    apply_label_word_bias(assembly, configuration.label_word_bias)
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
    let stem = owner_stem.to_string();
    let address = parse_hex(&stem)?;
    let canonical_symbol = format!("Func_{}", hex8(address));
    let short_symbol = format!("Func_{}", hex8(address).trim_start_matches('0'));

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
            configuration,
        )?;
        assemble(&assembly, &object)?;
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
    let link_address = address
        .checked_sub(owner_section_offset)
        .ok_or_else(|| format!("owner symbol offset exceeds link address: {symbol}"))?;
    let owner_offset =
        usize::try_from(owner_section_offset).map_err(|_| "owner object offset is too large")?;
    let owner_size = usize::try_from(owner_size).map_err(|_| "owner object size is too large")?;
    let owner_relocations = object_relocations(&object, owner_offset, owner_size)?;

    let call_via_base = configuration.call_via_base.unwrap_or(CALL_VIA_BASE);
    let mut names: Vec<String> = Vec::new();
    let undefined_symbols = run(&["arm-none-eabi-nm", "-u", &object], cwd)?;
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

    let mut symbols_text = String::from(".syntax unified\n.thumb\n");
    let inferred = names
        .iter()
        .filter(|name| !configuration.absolute_symbols.contains_key(*name))
        .cloned()
        .collect::<Vec<_>>();
    let resolved = if configuration.reference_symbols && !inferred.is_empty() {
        derive_reference_symbols(
            &object,
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
            &format!("-Ttext=0x{}", hex8(link_address)),
            "-e",
            symbol,
            "-o",
            &elf,
            &object,
            &symbols_object,
        ],
        cwd,
    )?;
    copy_text(&elf, &binary)?;

    let symbols = run(&["arm-none-eabi-nm", "-S", &elf], cwd)?;
    let fields = symbol_fields(&symbols, symbol)
        .ok_or_else(|| format!("missing linked symbol: {symbol}"))?;
    let binary_bytes = std::fs::read(&binary).map_err(|error| format!("{binary}: {error}"))?;
    let size = if fields.len() >= 4 {
        parse_hex(fields[1])?
    } else {
        binary_bytes.len() as u64
    };
    let linked_symbol_address = parse_hex(fields.first().ok_or("missing linked symbol address")?)?;
    let binary_offset = linked_symbol_address as f64 - link_address as f64;

    let actual = js_subarray(&binary_bytes, binary_offset, binary_offset + size as f64);
    let offset = address as f64 - image_base;
    let expected = js_subarray(rom, offset, offset + size as f64);
    Ok(Verification { actual, expected })
}

fn symbol_fields<'a>(listing: &'a str, symbol: &str) -> Option<Vec<&'a str>> {
    listing
        .lines()
        .map(|line| line.split_whitespace().collect::<Vec<_>>())
        .find(|fields| fields.last() == Some(&symbol))
}

fn absolute_symbol_directive(kind: AbsoluteSymbolKind) -> &'static str {
    if kind == AbsoluteSymbolKind::Thumb {
        ".thumb_set"
    } else {
        ".set"
    }
}

fn apply_label_word_bias(path: &str, bias: Option<u64>) -> Result<(), String> {
    let Some(bias) = bias.filter(|bias| *bias != 0) else {
        return Ok(());
    };
    let text = std::fs::read_to_string(path).map_err(|error| format!("{path}: {error}"))?;
    let labels = text
        .lines()
        .filter_map(|line| {
            line.trim()
                .strip_suffix(':')
                .filter(|label| label.starts_with(".L"))
        })
        .map(str::to_string)
        .collect::<std::collections::BTreeSet<_>>();
    let mut output = text
        .lines()
        .map(|line| {
            let operand = line.trim().strip_prefix(".word").map(str::trim);
            if operand.is_some_and(|operand| labels.contains(operand)) {
                format!("{line} + 0x{bias:x}")
            } else {
                line.to_string()
            }
        })
        .collect::<Vec<_>>()
        .join("\n");
    if text.ends_with('\n') {
        output.push('\n');
    }
    write(path, output.as_bytes())
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
    let high = u16::from_le_bytes([bytes[0], bytes[1]]);
    let low = u16::from_le_bytes([bytes[2], bytes[3]]);
    if high & 0xf800 != 0xf000 || low & 0xf800 != 0xf800 {
        return Err(format!(
            "reference relocation at 0x{offset:x} is not a Thumb BL"
        ));
    }
    let mut displacement = (((high & 0x07ff) as i64) << 12) | (((low & 0x07ff) as i64) << 1);
    if displacement & (1 << 22) != 0 {
        displacement -= 1 << 23;
    }
    let pc = address as i64 + offset as i64 + 4;
    u64::try_from(pc + displacement).map_err(|_| format!("call at 0x{offset:x} is below ROM"))
}

pub(crate) fn write(path: &str, bytes: &[u8]) -> Result<(), String> {
    std::fs::write(path, bytes).map_err(|error| format!("{path}: {error}"))
}

#[cfg(test)]
mod reference_symbol_tests {
    use super::*;

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

    #[test]
    fn overlay_bias_applies_only_to_defined_local_label_words() {
        let path = std::env::temp_dir().join("alchemy-overlay-label-bias.s");
        std::fs::write(&path, ".L2:\n.word .L2\n.word External\n").unwrap();
        apply_label_word_bias(path.to_str().unwrap(), Some(0x8000)).unwrap();
        assert_eq!(
            std::fs::read_to_string(&path).unwrap(),
            ".L2:\n.word .L2 + 0x8000\n.word External\n"
        );
        let _ = std::fs::remove_file(path);
    }
}
