//! `verifyCandidate` -- compile one candidate C file, link it at its ROM
//! address, and hand back the produced bytes beside the ROM's bytes.
//!
//! Every step spawns a real process, so runtime is dominated by the same five
//! toolchain binaries regardless of the caller.

use std::collections::BTreeMap;
use std::path::Path;
use std::process::Command;

use compiler_core::nodepath::{basename, extname};
use compiler_core::plan::{
    source_to_assembly_plan, CompilerFamily, CompilerFlagMutations, SourceToAssemblyPlanOptions,
};
use compiler_core::routing::{root, CompilerTarget};
use compiler_core::{external_symbol, external_symbol_assembly, CALL_VIA_BASE};

use crate::jsnum::{hex8, parse_hex};
use crate::jsstring::{js_split_lines, js_split_whitespace_runs, js_trim};

/// `ROM_BASE`.
pub const ROM_BASE: f64 = 0x0800_0000 as f64;

/// `CandidateCompilerFamily` is `compiler_core`'s `CompilerFamily`; the
/// TypeScript declares the same six-member union a second time rather than
/// importing it. One name here, deliberately.
pub type CandidateCompilerFamily = CompilerFamily;

/// `CandidateCompilerConfiguration`.
#[derive(Debug, Clone, Default)]
pub struct CandidateCompilerConfiguration {
    pub family: Option<CandidateCompilerFamily>,
    pub add_flags: Vec<String>,
    pub remove_flags: Vec<String>,
    /// Resolve external relocation targets from the selected reference image.
    /// This is required when one source model is measured at another edition's
    /// addresses, or when a readable external name deliberately carries no
    /// address. Every relocation site for one symbol must resolve consistently.
    pub reference_symbols: bool,
}

/// `interface Verification`.
#[derive(Debug, Clone)]
pub struct Verification {
    pub actual: Vec<u8>,
    pub expected: Vec<u8>,
    /// `size` is a JavaScript number, and `parseHex` can in principle produce a
    /// non-integer for an absurd `nm` field. Kept as `f64` so the value written
    /// into the report is the value that was computed.
    pub size: f64,
}

/// `sourceStem(path)` -- `basename(path, extname(path))`.
pub fn source_stem(path: &str) -> String {
    let base = basename(path);
    let ext = extname(path);
    // Node's `basename(path, ext)` strips `ext` only when it is a proper,
    // non-equal suffix; `basename(".c", ".c")` is `.c`, not "".
    if !ext.is_empty() && base.len() > ext.len() && base.ends_with(ext) {
        base[..base.len() - ext.len()].to_string()
    } else {
        base.to_string()
    }
}

/// `run(command, cwd = ROOT)`.
///
/// PORT NOTE -- the failure message is `${basename(command[0])} failed` plus
/// `: ${detail}` where `detail` is `(stderr || stdout).trim()`. The `||` means
/// an EMPTY stderr falls through to stdout, and a stderr of only whitespace
/// does not (it is a non-empty string, so it wins and then trims to nothing,
/// producing the bare `X failed`). Both halves are reproduced.
pub fn run(command: &[String], cwd: &Path) -> Result<String, String> {
    let program = command
        .first()
        .ok_or_else(|| "run: empty command".to_string())?;
    let output = Command::new(program)
        .args(&command[1..])
        .current_dir(cwd)
        .output()
        .map_err(|error| {
            // Bun reports a missing binary as an ENOENT thrown from spawn,
            // before the exit-code check. Same failure, different prose; the
            // parity harness compares exit status and offending path, not text.
            format!("{}: {error}", basename(program))
        })?;
    let stdout = String::from_utf8_lossy(&output.stdout).into_owned();
    if output.status.success() {
        return Ok(stdout);
    }
    let stderr = String::from_utf8_lossy(&output.stderr).into_owned();
    let detail_source = if stderr.is_empty() { &stdout } else { &stderr };
    let detail = js_trim(detail_source);
    let name = basename(program);
    if detail.is_empty() {
        Err(format!("{name} failed"))
    } else {
        Err(format!("{name} failed: {detail}"))
    }
}

/// `%TypedArray%.prototype.subarray(begin, end)`, which is what
/// `Buffer#subarray` is.
///
/// PORT NOTE -- this is the trap that Rust slicing cannot express. `arr[a..b]`
/// PANICS on an out-of-range index; `subarray` CLAMPS, and a NEGATIVE index is
/// measured from the end rather than being an error. The historical verifier computes
/// `address - imageBase`, which is hugely negative for an overlay address such
/// as `0x02000000` against `ROM_BASE`; JavaScript clamps that to 0 and returns
/// a slice from the start of the ROM. That is almost certainly not what anyone
/// intended, but it is what every recorded sweep measured, so it is reproduced
/// exactly. See the `overlay_address_clamps_to_rom_start` test.
pub fn js_subarray(data: &[u8], begin: f64, end: f64) -> Vec<u8> {
    let len = data.len() as f64;
    let resolve = |relative: f64| -> usize {
        // `ToIntegerOrInfinity`: NaN becomes 0.
        let value = if relative.is_nan() {
            0.0
        } else {
            relative.trunc()
        };
        let resolved = if value < 0.0 {
            let from_end = len + value;
            if from_end > 0.0 {
                from_end
            } else {
                0.0
            }
        } else if value < len {
            value
        } else {
            len
        };
        resolved as usize
    };
    let start = resolve(begin);
    let stop = resolve(end);
    if stop <= start {
        return Vec::new();
    }
    data[start..stop].to_vec()
}

/// `verifyCandidate(source, rom, outputDirectory, extraCompilerFlags, imageBase,
/// compiler, configuration)`.
pub fn verify_candidate(
    source: &str,
    rom: &[u8],
    output_directory: &str,
    extra_compiler_flags: &[String],
    image_base: f64,
    compiler: CompilerTarget,
    configuration: &CandidateCompilerConfiguration,
) -> Result<Verification, String> {
    verify_candidate_routed(
        source,
        source,
        rom,
        output_directory,
        extra_compiler_flags,
        image_base,
        compiler,
        configuration,
    )
}

/// gcc `-S` only: same flags as [`verify_candidate_routed`], no assembler,
/// linker, or ROM. The confirmation loop that git-diffs `.s` files needs this
/// and nothing else -- linking the owner is what turned a 30 ms compile into
/// half a second.
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
    let preprocessed = Path::new(output_directory)
        .join(format!("{stem}.i"))
        .to_string_lossy()
        .into_owned();

    let mut options =
        SourceToAssemblyPlanOptions::new(compiler, routing_source, source, assembly.clone());
    options.family = configuration.family;
    let mut add_flags = extra_compiler_flags.to_vec();
    add_flags.extend(configuration.add_flags.iter().cloned());
    options.flags = Some(CompilerFlagMutations {
        add_flags,
        remove_flags: configuration.remove_flags.clone(),
    });
    options.preprocessed_output = Some(preprocessed);
    let plan = source_to_assembly_plan(&options)?;
    let cwd = root();
    for step in &plan.steps {
        run(&step.command, cwd)?;
    }
    Ok(assembly)
}

/// Compile a temporary candidate using the compiler family and flags owned by
/// `routing_source`. Keeping the physical input separate from its eventual
/// repository path is required by parallel search tools; otherwise every
/// temporary file silently falls back to the default compiler route.
#[allow(clippy::too_many_arguments)]
pub fn verify_candidate_routed(
    source: &str,
    routing_source: &str,
    rom: &[u8],
    output_directory: &str,
    extra_compiler_flags: &[String],
    image_base: f64,
    compiler: CompilerTarget,
    configuration: &CandidateCompilerConfiguration,
) -> Result<Verification, String> {
    let stem = source_stem(source);
    verify_candidate_owned_routed(
        source,
        routing_source,
        &stem,
        rom,
        output_directory,
        extra_compiler_flags,
        image_base,
        compiler,
        configuration,
    )
}

/// Compile a source whose descriptive filename no longer encodes its owner.
///
/// `owner_stem` is the canonical eight-digit main-image address. The physical
/// source path remains untouched so source-relative includes keep working;
/// `routing_source` remains the stable owner identity used by compiler tables.
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
    let stem = owner_stem.to_string();
    let address = parse_hex(&stem)?;
    let symbol = format!("Func_{}", hex8(address));

    let out = Path::new(output_directory);
    let path = |suffix: &str| {
        out.join(format!("{stem}{suffix}"))
            .to_string_lossy()
            .into_owned()
    };
    let assembly = path(".s");
    let object = path(".o");
    let symbols_source = path(".symbols.s");
    let symbols_object = path(".symbols.o");
    let elf = path(".elf");
    let binary = path(".bin");

    let mut options =
        SourceToAssemblyPlanOptions::new(compiler, routing_source, source, assembly.clone());
    options.family = configuration.family;
    // ORDER IS BEHAVIOUR: `[...extraCompilerFlags, ...(configuration.addFlags ?? [])]`,
    // and gcc is later-flag-wins. Swapping these two changes the machine code.
    let mut add_flags = extra_compiler_flags.to_vec();
    add_flags.extend(configuration.add_flags.iter().cloned());
    options.flags = Some(CompilerFlagMutations {
        add_flags,
        remove_flags: configuration.remove_flags.clone(),
    });
    options.preprocessed_output = Some(path(".i"));
    let plan = source_to_assembly_plan(&options)?;

    let cwd = root();
    for step in &plan.steps {
        run(&step.command, cwd)?;
    }
    run(
        &argv(&[
            "arm-none-eabi-as",
            "-mcpu=arm7tdmi",
            "-mthumb-interwork",
            "-o",
            &object,
            &assembly,
        ]),
        cwd,
    )?;

    // A `Vec`, not a `Set`. `nm -u` can list the same undefined symbol twice
    // and the TypeScript pushes both, emitting a duplicate `.global`/`.thumb_set`
    // pair. Deduplicating here would change the generated assembly.
    let mut names: Vec<String> = Vec::new();
    let undefined_symbols = run(&argv(&["arm-none-eabi-nm", "-u", &object]), cwd)?;
    for line in js_split_lines(&undefined_symbols) {
        // `.filter(Boolean)` drops only the empty string, not a
        // whitespace-only line.
        if line.is_empty() {
            continue;
        }
        let fields = js_split_whitespace_runs(js_trim(line));
        // `.at(-1)!` -- the array is never empty, so this cannot be undefined.
        let external = *fields.last().expect("split always yields one field");
        if !configuration.reference_symbols && external_symbol(external, CALL_VIA_BASE).is_none() {
            return Err(format!("unsupported external symbol: {external}"));
        }
        names.push(external.to_string());
    }

    let mut symbols_text = String::from(".syntax unified\n.thumb\n");
    if configuration.reference_symbols {
        let resolved =
            derive_reference_symbols(&object, &symbol, &names, rom, address, image_base)?;
        for name in &names {
            let symbol = resolved
                .get(name)
                .ok_or_else(|| format!("no reference relocation for external symbol: {name}"))?;
            let directive = if symbol.thumb { ".thumb_set" } else { ".set" };
            symbols_text.push_str(&format!(
                ".global {name}\n{directive} {name}, 0x{:08x}\n",
                symbol.address
            ));
        }
    } else {
        for name in &names {
            symbols_text.push_str(&external_symbol_assembly(name, CALL_VIA_BASE)?);
        }
    }
    write(&symbols_source, symbols_text.as_bytes())?;
    run(
        &argv(&[
            "arm-none-eabi-as",
            "-mcpu=arm7tdmi",
            "-mthumb-interwork",
            "-o",
            &symbols_object,
            &symbols_source,
        ]),
        cwd,
    )?;
    run(
        &argv(&[
            "arm-none-eabi-ld",
            &format!("-Ttext=0x{}", hex8(address)),
            "-e",
            &symbol,
            "-o",
            &elf,
            &object,
            &symbols_object,
        ]),
        cwd,
    )?;
    run(
        &argv(&[
            "arm-none-eabi-objcopy",
            "-O",
            "binary",
            "-j",
            ".text",
            &elf,
            &binary,
        ]),
        cwd,
    )?;

    let symbols = run(&argv(&["arm-none-eabi-nm", "-S", &elf]), cwd)?;
    let needle = format!(" {symbol}");
    let row = js_split_lines(&symbols)
        .into_iter()
        .find(|line| line.ends_with(&needle))
        .ok_or_else(|| format!("missing linked symbol: {symbol}"))?;
    let fields = js_split_whitespace_runs(js_trim(row));
    // Thumb/COFF assembly from stock GCC 2.95.1 has no ELF `.size` directive,
    // so nm reports `address T symbol` instead of `address size T symbol`.
    // Candidate translation units contain one text region; in that case the
    // linked binary's complete .text extent is the function extent.
    let binary_bytes = std::fs::read(&binary).map_err(|error| format!("{binary}: {error}"))?;
    let size = if fields.len() >= 4 {
        parse_hex(fields[1])?
    } else {
        binary_bytes.len() as f64
    };

    let actual = js_subarray(&binary_bytes, 0.0, size);
    let offset = address - image_base;
    let expected = js_subarray(rom, offset, offset + size);
    Ok(Verification {
        actual,
        expected,
        size,
    })
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
struct ResolvedSymbol {
    address: u64,
    thumb: bool,
}

#[derive(Clone, Debug)]
struct ReferenceRelocation {
    offset: usize,
    kind: String,
}

fn derive_reference_symbols(
    object: &str,
    owner_symbol: &str,
    names: &[String],
    rom: &[u8],
    address: f64,
    image_base: f64,
) -> Result<BTreeMap<String, ResolvedSymbol>, String> {
    let address = exact_u64(address, "owner address")?;
    let image_base = exact_u64(image_base, "image base")?;
    let rom_start = address
        .checked_sub(image_base)
        .ok_or("owner address precedes image base")?;
    let rom_start = usize::try_from(rom_start).map_err(|_| "ROM offset is too large")?;
    let owner_offset = object_symbol_offset(object, owner_symbol)?;
    let relocations = object_relocations(object, owner_offset)?;

    let object_text_path = format!("{object}.text.bin");
    run(
        &argv(&[
            "arm-none-eabi-objcopy",
            "-O",
            "binary",
            "-j",
            ".text",
            object,
            &object_text_path,
        ]),
        root(),
    )?;
    let object_text =
        std::fs::read(&object_text_path).map_err(|error| format!("{object_text_path}: {error}"))?;

    let mut resolved = BTreeMap::new();
    for name in names {
        let sites = relocations
            .get(name)
            .ok_or_else(|| format!("no reference relocation for external symbol: {name}"))?;
        let known = external_symbol(name, CALL_VIA_BASE);
        let thumb = known
            .map(|symbol| symbol.thumb)
            .unwrap_or_else(|| sites.iter().all(|site| site.kind == "R_ARM_THM_CALL"));
        if known.is_none() && sites.iter().any(|site| site.kind != "R_ARM_THM_CALL") {
            return Err(format!(
                "cannot infer code/data type for reference symbol {name}"
            ));
        }

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
            ResolvedSymbol {
                address: value.ok_or_else(|| format!("no reference relocation for {name}"))?,
                thumb,
            },
        );
    }
    Ok(resolved)
}

fn exact_u64(value: f64, label: &str) -> Result<u64, String> {
    if !value.is_finite() || value < 0.0 || value.fract() != 0.0 || value > 9_007_199_254_740_991.0
    {
        return Err(format!("{label} is not an exact non-negative integer"));
    }
    Ok(value as u64)
}

fn object_symbol_offset(object: &str, owner_symbol: &str) -> Result<usize, String> {
    let symbols = run(&argv(&["arm-none-eabi-nm", "-S", object]), root())?;
    for line in js_split_lines(&symbols) {
        let fields = js_split_whitespace_runs(js_trim(line));
        if fields.len() >= 3 && fields.last() == Some(&owner_symbol) {
            let address = parse_hex(fields[0])?;
            return usize::try_from(exact_u64(address, "owner object offset")?)
                .map_err(|_| "owner object offset is too large".into());
        }
    }
    Err(format!("missing object symbol: {owner_symbol}"))
}

fn object_relocations(
    object: &str,
    owner_offset: usize,
) -> Result<BTreeMap<String, Vec<ReferenceRelocation>>, String> {
    let output = run(&argv(&["arm-none-eabi-objdump", "-r", object]), root())?;
    let mut relocations = BTreeMap::<String, Vec<ReferenceRelocation>>::new();
    for line in js_split_lines(&output) {
        let fields = js_split_whitespace_runs(js_trim(line));
        if fields.len() < 3 || !fields[1].starts_with("R_ARM_") {
            continue;
        }
        let raw_offset = usize::from_str_radix(fields[0], 16)
            .map_err(|error| format!("invalid relocation offset {}: {error}", fields[0]))?;
        let Some(offset) = raw_offset.checked_sub(owner_offset) else {
            continue;
        };
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

fn argv(parts: &[&str]) -> Vec<String> {
    parts.iter().map(|s| (*s).to_string()).collect()
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
    fn exact_integer_gate_rejects_fractional_addresses() {
        assert!(exact_u64(0x0800_0000 as f64, "address").is_ok());
        assert!(exact_u64(1.5, "address").is_err());
    }
}
