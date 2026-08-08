//! `verifyCandidate` -- compile one candidate C file, link it at its ROM
//! address, and hand back the produced bytes beside the ROM's bytes.
//!
//! This is the half of `match_m2c.ts` that reaches the toolchain. Every step
//! spawns a real process, so this module is where the port's runtime cost
//! lives and where it cannot beat the TypeScript by much: both are waiting on
//! the same five binaries.

use std::path::Path;
use std::process::Command;

use alchemy_plan::nodepath::{basename, extname};
use alchemy_plan::plan::{
    source_to_assembly_plan, CompilerFamily, CompilerFlagMutations, SourceToAssemblyPlanOptions,
};
use alchemy_routing::routing::{root, CompilerTarget};
use alchemy_symbols::{external_symbol, external_symbol_assembly, CALL_VIA_BASE};

use crate::jsnum::{hex8, parse_hex};
use crate::jsstring::{js_split_lines, js_split_whitespace_runs, js_trim};

/// `ROM_BASE`.
pub const ROM_BASE: f64 = 0x0800_0000 as f64;

/// The typedef and macro block prepended to every candidate. Byte-identical to
/// `M2C_PREAMBLE`, trailing blank line included: the candidate files in
/// `work/matches/m2c` were written with it and re-emitting them must not
/// produce a diff.
pub const M2C_PREAMBLE: &str = "typedef signed char s8;\n\
typedef unsigned char u8;\n\
typedef signed short s16;\n\
typedef unsigned short u16;\n\
typedef signed int s32;\n\
typedef unsigned int u32;\n\
typedef signed long long s64;\n\
typedef unsigned long long u64;\n\
typedef int bool;\n\
#define NULL ((void *)0)\n\
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))\n\
\n\
";

/// `CandidateCompilerFamily` is `alchemy_plan`'s `CompilerFamily`; the
/// TypeScript declares the same six-member union a second time rather than
/// importing it. One name here, deliberately.
pub type CandidateCompilerFamily = CompilerFamily;

/// `CandidateCompilerConfiguration`.
#[derive(Debug, Clone, Default)]
pub struct CandidateCompilerConfiguration {
    pub family: Option<CandidateCompilerFamily>,
    pub add_flags: Vec<String>,
    pub remove_flags: Vec<String>,
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
/// measured from the end rather than being an error. `match_m2c.ts` computes
/// `address - imageBase`, which is hugely negative for an overlay address such
/// as `0x02000000` against `ROM_BASE`; JavaScript clamps that to 0 and returns
/// a slice from the start of the ROM. That is almost certainly not what anyone
/// intended, but it is what every recorded sweep measured, so it is reproduced
/// exactly. See the `overlay_address_clamps_to_rom_start` test.
pub fn js_subarray(data: &[u8], begin: f64, end: f64) -> Vec<u8> {
    let len = data.len() as f64;
    let resolve = |relative: f64| -> usize {
        // `ToIntegerOrInfinity`: NaN becomes 0.
        let value = if relative.is_nan() { 0.0 } else { relative.trunc() };
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
    let stem = source_stem(source);
    let address = parse_hex(&stem)?;
    let symbol = format!("Func_{}", hex8(address));

    let out = Path::new(output_directory);
    let path = |suffix: &str| out.join(format!("{stem}{suffix}")).to_string_lossy().into_owned();
    let assembly = path(".s");
    let object = path(".o");
    let symbols_source = path(".symbols.s");
    let symbols_object = path(".symbols.o");
    let elf = path(".elf");
    let binary = path(".bin");

    let mut options =
        SourceToAssemblyPlanOptions::new(compiler, source, source, assembly.clone());
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
        if external_symbol(external, CALL_VIA_BASE).is_none() {
            return Err(format!("unsupported external symbol: {external}"));
        }
        names.push(external.to_string());
    }

    let mut symbols_text = String::from(".syntax unified\n.thumb\n");
    for name in &names {
        symbols_text.push_str(&external_symbol_assembly(name, CALL_VIA_BASE)?);
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
        &argv(&["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", &elf, &binary]),
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
    Ok(Verification { actual, expected, size })
}

fn argv(parts: &[&str]) -> Vec<String> {
    parts.iter().map(|s| (*s).to_string()).collect()
}

pub(crate) fn write(path: &str, bytes: &[u8]) -> Result<(), String> {
    std::fs::write(path, bytes).map_err(|error| format!("{path}: {error}"))
}
