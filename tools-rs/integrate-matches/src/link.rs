//! `run`, `commandError` and `linkedBytes` -- the half of the tool that reaches
//! the toolchain.
//!
//! `linkedBytes` is called twice per candidate: once on the hand-written
//! `asm/<stem>.s` to produce the bytes the ROM wants, and once on the m2c
//! candidate C file to produce the bytes it would install. Both go through
//! `as`, `nm -u`, a generated symbol stub, `ld` at the real ROM address, and
//! `objcopy`; only the C side additionally measures its extent with `nm -S`.

use std::path::Path;
use std::process::Command;

use alchemy_plan::plan::{source_to_assembly_plan, SourceToAssemblyPlanOptions};
use alchemy_routing::routing::{root, CompilerTarget};
use alchemy_symbols::{external_symbol, external_symbol_assembly, CALL_VIA_BASE};
use match_m2c::jsstring::{js_split_lines, js_split_whitespace_runs, js_trim};

use crate::extent::linked_function_extent;
use crate::jsint::{hexadecimal, is_safe_integer, parse_int_hex};
use crate::jsregex::is_source_address;

/// Which of the two inputs `linkedBytes` was handed.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Kind {
    Asm,
    C,
}

/// The record `run` returns.
pub struct Run {
    pub code: i32,
    pub stdout: String,
    pub stderr: String,
}

/// `run(command, cwd = ROOT)`.
///
/// PORT NOTE -- `Bun.spawnSync` THROWS on a missing binary rather than
/// returning a non-zero code, and that throw escapes `linkedBytes` uncaught by
/// any of the `if (code !== 0)` tests, landing in `main`'s `catch` as a
/// rejection whose message is Bun's ENOENT text. The Rust spawn failure is
/// surfaced the same way, as an `Err` from `linked_bytes`, with different
/// prose. Bun's ENOENT output carries NO `error: ` prefix, so neither side is
/// matchable on text; the parity harness asserts same-failure, and preflights
/// the four `arm-none-eabi-*` binaries so this path is never the reason a run
/// looks equal.
///
/// PORT NOTE -- `process.exitCode` is `null` when a child dies on a signal, and
/// `null !== 0` is true, so the TypeScript treats a signalled child as a
/// failure. `-1` here does the same.
pub fn run(command: &[String], cwd: &Path) -> Result<Run, String> {
    let program = command.first().ok_or_else(|| "run: empty command".to_string())?;
    let output = Command::new(program)
        .args(&command[1..])
        .current_dir(cwd)
        .output()
        .map_err(|error| format!("{program}: {error}"))?;
    Ok(Run {
        code: output.status.code().unwrap_or(-1),
        // `Buffer.from(bytes).toString("utf8")` replaces invalid sequences with
        // U+FFFD rather than throwing, which is `from_utf8_lossy`.
        stdout: String::from_utf8_lossy(&output.stdout).into_owned(),
        stderr: String::from_utf8_lossy(&output.stderr).into_owned(),
    })
}

/// `commandError(result)` -- `(result.stderr || result.stdout).trim()`.
///
/// PORT NOTE -- `||`, not `??`. An EMPTY stderr falls through to stdout; a
/// stderr of only whitespace does NOT, because a whitespace string is truthy,
/// so it wins and then trims away to nothing. The message becomes
/// `"assembler failed: "` with a trailing space, not the compiler's stdout.
/// Both halves are reproduced; `tests/js_traps.rs` pins the whitespace case.
pub fn command_error(result: &Run) -> &str {
    let source = if result.stderr.is_empty() { &result.stdout } else { &result.stderr };
    js_trim(source)
}

/// `linkedBytes(stem, source, scratch, kind)`.
///
/// `root_directory` is the repository root the TypeScript spells `ROOT`, passed
/// in so that the routed-name construction can be pointed at a fixture tree by
/// the parity harness without ever touching the tracked `exact/`.
pub fn linked_bytes(
    stem: &str,
    source: &str,
    scratch: &Path,
    kind: Kind,
    root_directory: &Path,
) -> Result<Vec<u8>, String> {
    let address = parse_int_hex(stem);
    // ORDER: `!Number.isSafeInteger(address) || !/^08[0-9a-f]{6}$/.test(stem)`.
    // `||` short-circuits, so a NaN address never reaches the pattern test;
    // both spellings reject the same set, and both raise the same message.
    if !is_safe_integer(address) || !is_source_address(stem) {
        return Err("invalid source address".to_string());
    }
    let prefix = scratch.join(format!("{stem}.{}probe", match kind {
        Kind::Asm => "asm",
        Kind::C => "c",
    }));
    let prefix = prefix.to_string_lossy().into_owned();
    let listing = format!("{prefix}.s");
    let object = format!("{prefix}.o");

    let cwd = root();
    if kind == Kind::C {
        // The comment in the TypeScript is load-bearing and is repeated here:
        // candidates arrive as `src_<stem>.c`, but the per-source
        // compiler-mode allowlists key off the bare stem, so the ROUTING name
        // is `exact/<stem>.c` -- the name the file will carry once installed --
        // while the compiler INPUT stays wherever the draft actually sits.
        // Routing on the scratch path would silently drop every per-source
        // flag override and change the emitted code.
        let routed = root_directory
            .join("exact")
            .join(format!("{stem}.c"))
            .to_string_lossy()
            .into_owned();
        let mut options =
            SourceToAssemblyPlanOptions::new(CompilerTarget::Gs1, routed, source, listing.clone());
        options.preprocessed_output = Some(format!("{prefix}.i"));
        let plan = source_to_assembly_plan(&options)?;
        for step in &plan.steps {
            let compiled = run(&step.command, cwd)?;
            if compiled.code != 0 {
                return Err(format!("{} failed: {}", step.kind.as_str(), command_error(&compiled)));
            }
        }
    } else {
        std::fs::copy(source, &listing)
            .map_err(|error| format!("{source}: {error}"))?;
    }

    let assembled = run(&assemble(&object, &listing), cwd)?;
    if assembled.code != 0 {
        return Err(format!("assembler failed: {}", command_error(&assembled)));
    }
    let undefined_result = run(&argv(&["arm-none-eabi-nm", "-u", &object]), cwd)?;
    if undefined_result.code != 0 {
        return Err(format!("nm failed: {}", command_error(&undefined_result)));
    }
    // A `Vec`, never a `Set`. `nm -u` can name the same symbol twice and the
    // TypeScript emits a stub for each occurrence; deduplicating would change
    // the generated assembly, and a `HashSet` would also lose the order the
    // stubs are written in.
    let mut names: Vec<String> = Vec::new();
    for line in js_split_lines(&undefined_result.stdout) {
        if line.is_empty() {
            continue;
        }
        let fields = js_split_whitespace_runs(js_trim(line));
        names.push((*fields.last().expect("split yields at least one field")).to_string());
    }
    for name in &names {
        if external_symbol(name, CALL_VIA_BASE).is_none() {
            #[cfg(feature = "negative-control")]
            return Err(format!("unsupported external symbol <{name}>"));
            #[cfg(not(feature = "negative-control"))]
            return Err(format!("unsupported external symbol {name}"));
        }
    }

    let symbols_source = format!("{prefix}.symbols.s");
    let symbols_object = format!("{prefix}.symbols.o");
    let mut symbols_text = String::from(".syntax unified\n.thumb\n");
    for name in &names {
        symbols_text.push_str(&external_symbol_assembly(name, CALL_VIA_BASE)?);
    }
    std::fs::write(&symbols_source, symbols_text.as_bytes())
        .map_err(|error| format!("{symbols_source}: {error}"))?;
    let symbols_assembled = run(&assemble(&symbols_object, &symbols_source), cwd)?;
    if symbols_assembled.code != 0 {
        return Err(format!("symbol assembler failed: {}", command_error(&symbols_assembled)));
    }

    let elf = format!("{prefix}.elf");
    let binary = format!("{prefix}.bin");
    let formatted = hexadecimal(address);
    let linked = run(
        &argv(&[
            "arm-none-eabi-ld",
            &format!("-Ttext=0x{formatted}"),
            "-e",
            &format!("Func_{formatted}"),
            "-o",
            &elf,
            &object,
            &symbols_object,
        ]),
        cwd,
    )?;
    if linked.code != 0 {
        return Err(format!("linker failed: {}", command_error(&linked)));
    }
    let copied = run(
        &argv(&["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", &elf, &binary]),
        cwd,
    )?;
    if copied.code != 0 {
        return Err(format!("objcopy failed: {}", command_error(&copied)));
    }
    let bytes = std::fs::read(&binary).map_err(|error| format!("{binary}: {error}"))?;
    if kind == Kind::Asm {
        return Ok(bytes);
    }
    let symbol_result = run(&argv(&["arm-none-eabi-nm", "-S", "--defined-only", &elf]), cwd)?;
    if symbol_result.code != 0 {
        return Err(format!("nm failed: {}", command_error(&symbol_result)));
    }
    let extent = linked_function_extent(
        &symbol_result.stdout,
        &format!("Func_{formatted}"),
        address,
        bytes.len(),
    )?;
    // `bytes.subarray(0, extent)`. `extent` is proved to be in `1..=len` by
    // `linked_function_extent`, so this cannot be the clamp-versus-panic trap;
    // the proof lives there and is pinned by its tests.
    Ok(bytes[..extent].to_vec())
}

fn assemble(object: &str, listing: &str) -> Vec<String> {
    argv(&["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", object, listing])
}

fn argv(parts: &[&str]) -> Vec<String> {
    parts.iter().map(|part| (*part).to_string()).collect()
}
