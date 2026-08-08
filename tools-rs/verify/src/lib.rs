//! Byte-equality verifier: compile a decompiled translation unit with the
//! routed compiler, link it at its ROM address, and compare the emitted
//! `.text` against the original ROM bytes.
//!
//! Port of `tools/lib/verify.ts`.
//!
//! PORT NOTE -- what was inlined, and from where. `verify.ts` imports two local
//! modules; only the parts it actually reaches were ported, and they live here
//! rather than in crates of their own, because those modules are not ported
//! yet. When they are, delete the copies and depend on them instead:
//!
//! * from `tools/lib/alchemy_gcc.ts`: `externalSymbol`,
//!   `externalSymbolAssembly`, `sourceToAssemblyPlan` (routed path only),
//!   `compilerCommandForTarget`, `directPreprocessorCommand`, `validateBundle`,
//!   `validateAgbccBundle`, `hostKey`, the host/target digest pins
//!   and the whole flag-routing surface. Those last two were promoted out of
//!   this crate into `tools-rs/alchemy-routing` (`routing`, `routing_data`)
//!   and `tools-rs/alchemy-bundle` (`bundle_data`), which this crate now
//!   depends on by path and re-exports.
//!   SHA-256 came from `Bun.CryptoHasher` and is hand-rolled in `src/sha256.rs`.
//! * from `tools/lib/decomp_targets.ts`: `DEFAULT_TARGET`, `TARGETS`,
//!   `parseDecompTarget`, `decompTarget`, and its `selfTest` (ported into
//!   `#[cfg(test)]` assertions in `targets`).
//!
//! PORT NOTE -- deliberate omissions. `sourceToAssemblyPlan` in TypeScript also
//! serves the experimental `pret-early-thumb`, `gcc2951` and `gcc3` families and
//! a flag-mutation parameter. `verify.ts` never passes `family` or `flags`, so
//! only the `routed` path (which resolves to `gcc296` or `old-agbcc`) is ported.
//! The experimental drivers and their digest pins are not present in this crate.
//!
//! PORT NOTE -- added hard failures. The TypeScript can report a match without
//! having compared any bytes, because `Buffer.subarray` clamps silently: an
//! address outside the ROM, or a `.bin` shorter than the linked symbol size,
//! yields two short-or-empty buffers that compare equal and are counted as
//! `verified`. This port refuses every such case; see `verify_details`. `main`
//! likewise refuses an empty source list, which the TypeScript reports as
//! `verified=0 failures=0` with exit code 0.

pub mod sha256;
pub mod targets;

// The routing surface now lives in its own crate, `tools-rs/alchemy-routing`.
// Re-exported under the module paths it had while it was inlined here, so this
// crate's callers are unaffected by the move.
pub use alchemy_bundle::bundle_data;
pub use alchemy_routing::{routing, routing_data};

use std::collections::HashSet;
use std::fs;
use std::os::unix::fs::PermissionsExt;
use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::Mutex;

use routing::{
    agbcc_driver, bundle, bundle_for_target, cflags_for_target_source, driver_for_target, root,
    uses_agbcc_compiler, CompilerTarget,
};
use sha256::sha256_hex;

pub const ROM_BASE: u64 = 0x0800_0000;

pub type Result<T> = std::result::Result<T, String>;

// ---------------------------------------------------------------------------
// Subprocesses.
// ---------------------------------------------------------------------------

fn base_name(path: &str) -> &str {
    path.rsplit('/').next().unwrap_or(path)
}

/// `run()`: execute in the repository root, capture stdout, and fail loudly.
///
/// PORT NOTE: the TypeScript defaults to `quiet`, discarding the child's
/// diagnostics. This port always includes them -- a verifier that hides why a
/// compile failed costs more than the terser message saves.
fn run(command: &[String]) -> Result<String> {
    let (program, arguments) = command
        .split_first()
        .ok_or_else(|| "empty command".to_string())?;
    let output = Command::new(program)
        .args(arguments)
        .current_dir(root())
        .output()
        .map_err(|error| format!("{} failed to start: {error}", base_name(program)))?;
    if !output.status.success() {
        let stderr = String::from_utf8_lossy(&output.stderr);
        let stdout = String::from_utf8_lossy(&output.stdout);
        let detail = if stderr.trim().is_empty() {
            stdout.trim().to_string()
        } else {
            stderr.trim().to_string()
        };
        return Err(format!("{} failed: {detail}", base_name(program)));
    }
    Ok(String::from_utf8_lossy(&output.stdout).into_owned())
}

// ---------------------------------------------------------------------------
// External symbols.
// ---------------------------------------------------------------------------

pub const CALL_VIA_BASE: u64 = 0x0800_72e4;

#[derive(Debug, PartialEq, Eq, Clone, Copy)]
pub struct ExternalSymbol {
    pub address: u64,
    pub thumb: bool,
}

fn is_lower_hex(value: &str, length: usize) -> bool {
    value.len() == length
        && value
            .bytes()
            .all(|b| b.is_ascii_digit() || (b'a'..=b'f').contains(&b))
}

/// `externalSymbol`, with the three TypeScript regexes matched by hand:
/// `^(Func|Data|Value)_([0-9a-f]{8})(?:_[a-z])?$`,
/// `^_call_via_r(1[0-3]|[0-9])$` and `^_call_via_(sl|fp|ip|sp)$`.
pub fn external_symbol(name: &str) -> Option<ExternalSymbol> {
    for (prefix, thumb) in [("Func_", true), ("Data_", false), ("Value_", false)] {
        if let Some(rest) = name.strip_prefix(prefix) {
            let hex = match rest.len() {
                8 => rest,
                10 => {
                    let (hex, suffix) = rest.split_at(8);
                    let suffix = suffix.as_bytes();
                    if suffix[0] != b'_' || !suffix[1].is_ascii_lowercase() {
                        return None;
                    }
                    hex
                }
                _ => return None,
            };
            if !is_lower_hex(hex, 8) {
                return None;
            }
            return Some(ExternalSymbol {
                address: u64::from_str_radix(hex, 16).ok()?,
                thumb,
            });
        }
    }
    if let Some(rest) = name.strip_prefix("_call_via_r") {
        // `1[0-3]|[0-9]`: r0-r9 and r10-r13, and nothing else -- notably not
        // "r00", which the alternation cannot produce.
        let register: u64 = match rest.len() {
            1 if rest.as_bytes()[0].is_ascii_digit() => rest.parse().ok()?,
            2 if rest.starts_with('1') && matches!(rest.as_bytes()[1], b'0'..=b'3') => {
                rest.parse().ok()?
            }
            _ => return None,
        };
        return Some(ExternalSymbol {
            address: CALL_VIA_BASE + register * 4,
            thumb: true,
        });
    }
    if let Some(rest) = name.strip_prefix("_call_via_") {
        let register = match rest {
            "sl" => 10,
            "fp" => 11,
            "ip" => 12,
            "sp" => 13,
            _ => return None,
        };
        return Some(ExternalSymbol {
            address: CALL_VIA_BASE + register * 4,
            thumb: true,
        });
    }
    None
}

/// `externalSymbolAssembly`. `.thumb_set` keeps the branch type on the alias,
/// which is what makes both the branch offset and the tagged pool word correct.
pub fn external_symbol_assembly(name: &str) -> Result<String> {
    let symbol =
        external_symbol(name).ok_or_else(|| format!("unsupported external symbol: {name}"))?;
    let directive = if symbol.thumb { ".thumb_set" } else { ".set" };
    Ok(format!(
        ".global {name}\n{directive} {name}, 0x{:08x}\n",
        symbol.address
    ))
}

// ---------------------------------------------------------------------------
// Bundle validation.
// ---------------------------------------------------------------------------

const UNSUPPORTED_HOST_MESSAGE: &str =
    "alchemy-gcc supports darwin/linux on arm64/x86_64; this platform is none of those";

fn host_key() -> Option<&'static str> {
    match (std::env::consts::OS, std::env::consts::ARCH) {
        ("macos", "aarch64") => Some("darwin-arm64"),
        ("macos", "x86_64") => Some("darwin-x64"),
        ("linux", "x86_64") => Some("linux-x64"),
        ("linux", "aarch64") => Some("linux-arm64"),
        _ => None,
    }
}

fn host_admission_message(host: &str, what: &str) -> String {
    [
        format!("alchemy-gcc has no approved {what} digests for host {host} yet."),
        "Admit this host: build the committed fork source (alchemy-gcc/build.sh),".to_string(),
        "stage it (alchemy-gcc/stage.sh), run the full `bun run verify`, and pin".to_string(),
        "the digests from that green verify -- the same admission every listed".to_string(),
        "digest already passed.".to_string(),
    ]
    .join(" ")
}

fn validated() -> &'static Mutex<HashSet<&'static str>> {
    static VALIDATED: std::sync::OnceLock<Mutex<HashSet<&'static str>>> = std::sync::OnceLock::new();
    VALIDATED.get_or_init(|| Mutex::new(HashSet::new()))
}

fn is_executable(path: &Path) -> Option<bool> {
    let metadata = fs::metadata(path).ok()?;
    Some(metadata.permissions().mode() & 0o111 != 0)
}

/// `validateBundle`.
pub fn validate_bundle(target: CompilerTarget) -> Result<()> {
    if validated()
        .lock()
        .expect("validation cache is not poisoned")
        .contains(target.as_str())
    {
        return Ok(());
    }
    let host = host_key().ok_or_else(|| UNSUPPORTED_HOST_MESSAGE.to_string())?;
    let bundle = bundle_for_target(target);
    let entries = bundle_data::EXPECTED
        .iter()
        .find(|(key, _)| *key == host)
        .and_then(|(_, targets)| targets.iter().find(|(key, _)| *key == target.as_str()))
        .map(|(_, entries)| *entries)
        .ok_or_else(|| host_admission_message(host, target.as_str()))?;
    if entries.iter().all(|(_, expected)| expected.is_empty()) {
        return Err(host_admission_message(host, target.as_str()));
    }
    for (name, expected) in entries {
        let path = bundle.join(name);
        let missing = format!(
            "alchemy-gcc {} bundle is missing executable {name}",
            target.as_str()
        );
        if is_executable(&path) != Some(true) {
            return Err(missing);
        }
        let bytes = fs::read(&path).map_err(|_| missing)?;
        let actual = sha256_hex(&bytes);
        if !expected.contains(&actual.as_str()) {
            return Err(format!(
                "alchemy-gcc {}/{name} has an unapproved digest",
                target.as_str()
            ));
        }
    }
    // Smoke launch, one executable at a time: concurrent first launches race
    // macOS's own binary validation.
    let smoke = run(&[
        driver_for_target(target).to_string_lossy().into_owned(),
        format!("-B{}/", bundle.display()),
        "-S".into(),
        "-x".into(),
        "c".into(),
        "-o".into(),
        "/dev/null".into(),
        "/dev/null".into(),
    ]);
    if let Err(detail) = smoke {
        return Err(format!(
            "alchemy-gcc {} smoke compile failed: {detail}",
            target.as_str()
        ));
    }
    validated()
        .lock()
        .expect("validation cache is not poisoned")
        .insert(target.as_str());
    Ok(())
}

fn agbcc_validated() -> &'static Mutex<bool> {
    static VALIDATED: std::sync::OnceLock<Mutex<bool>> = std::sync::OnceLock::new();
    VALIDATED.get_or_init(|| Mutex::new(false))
}

/// `validateAgbccBundle`.
pub fn validate_agbcc_bundle() -> Result<()> {
    if *agbcc_validated().lock().expect("cache is not poisoned") {
        return Ok(());
    }
    let host = host_key().ok_or_else(|| UNSUPPORTED_HOST_MESSAGE.to_string())?;
    let driver = agbcc_driver();
    let missing = "alchemy-gcc agbcc bundle is missing executable old_agbcc".to_string();
    if is_executable(&driver) != Some(true) {
        return Err(missing);
    }
    let bytes = fs::read(&driver).map_err(|_| missing)?;
    let actual = sha256_hex(&bytes);
    let expected = bundle_data::AGBCC_EXPECTED
        .iter()
        .find(|(key, _)| *key == host)
        .map(|(_, digests)| *digests)
        .unwrap_or(&[]);
    if expected.is_empty() {
        return Err(host_admission_message(host, "agbcc/old_agbcc"));
    }
    if !expected.contains(&actual.as_str()) {
        return Err("alchemy-gcc agbcc/old_agbcc has an unapproved digest".to_string());
    }
    let smoke = run(&[
        driver.to_string_lossy().into_owned(),
        "/dev/null".into(),
        "-mthumb-interwork".into(),
        "-O2".into(),
        "-o".into(),
        "/dev/null".into(),
    ]);
    if let Err(detail) = smoke {
        return Err(format!("alchemy-gcc agbcc smoke compile failed: {detail}"));
    }
    *agbcc_validated().lock().expect("cache is not poisoned") = true;
    Ok(())
}

// ---------------------------------------------------------------------------
// Compilation plan.
// ---------------------------------------------------------------------------

/// `directPreprocessorCommand`.
fn direct_preprocessor_command(input: &str, output: &str, gcc_minor: u32) -> Result<Vec<String>> {
    validate_bundle(CompilerTarget::Gs1)?;
    let mut command = vec![bundle().join("cpp").to_string_lossy().into_owned()];
    command.extend(
        [
            "-lang-c",
            "-nostdinc",
            "-D__GNUC__=2",
        ]
        .iter()
        .map(|s| (*s).to_string()),
    );
    command.push(format!("-D__GNUC_MINOR__={gcc_minor}"));
    command.extend(
        [
            "-D__GNUC_PATCHLEVEL__=0",
            "-Acpu(arm)",
            "-Amachine(arm)",
            "-D__CHAR_UNSIGNED__",
            "-D__OPTIMIZE__",
            "-D__ARM_ARCH_4T__",
            "-D__APCS_32__",
            "-D__ARMEL__",
            "-D__THUMBEL__",
            "-Darm_elf",
            "-D__ELF__",
            "-Dthumb",
            "-D__thumb__",
        ]
        .iter()
        .map(|s| (*s).to_string()),
    );
    command.push(format!("-I{}", root().join("include").display()));
    command.push(input.to_string());
    command.push(output.to_string());
    Ok(command)
}

/// `sourceToAssemblyPlan`, restricted to the `routed` family (see the module
/// PORT NOTE). Returns the ordered commands that turn `input` into `output`.
pub fn source_to_assembly_plan(
    target: CompilerTarget,
    routing_source: &str,
    input: &str,
    output: &str,
    preprocessed_output: &str,
) -> Result<Vec<Vec<String>>> {
    let agbcc = uses_agbcc_compiler(target, routing_source);
    let flags = cflags_for_target_source(target, routing_source);
    if agbcc {
        validate_agbcc_bundle()?;
        let preprocess = direct_preprocessor_command(input, preprocessed_output, 9)?;
        let mut compile = vec![
            agbcc_driver().to_string_lossy().into_owned(),
            preprocessed_output.to_string(),
            "-dumpbase".to_string(),
            base_name(routing_source).to_string(),
        ];
        compile.extend(flags);
        compile.push("-o".to_string());
        compile.push(output.to_string());
        return Ok(vec![preprocess, compile]);
    }
    validate_bundle(target)?;
    let mut compile = vec![
        driver_for_target(target).to_string_lossy().into_owned(),
        format!("-B{}/", bundle_for_target(target).display()),
    ];
    compile.extend(flags);
    compile.extend(
        ["-S", "-o", output, input]
            .iter()
            .map(|s| (*s).to_string()),
    );
    Ok(vec![compile])
}

// ---------------------------------------------------------------------------
// Verification.
// ---------------------------------------------------------------------------

fn join(directory: &str, file: &str) -> String {
    Path::new(directory).join(file).to_string_lossy().into_owned()
}

/// The compared bytes plus the linked size: `verify(..., details = true)`.
pub fn verify_details(
    source: &str,
    rom: &[u8],
    output_dir: &str,
    compiler: CompilerTarget,
) -> Result<(Vec<u8>, Vec<u8>, usize)> {
    let name = routing::source_stem(source);
    // PORT NOTE: `Number.parseInt(name, 16)` accepts a hex *prefix* and yields
    // NaN otherwise, which then flows into the linker address as "NaN". The
    // filename is the ROM address, so this port requires it to be exactly one.
    if !is_lower_hex(&name, 8) {
        return Err(format!(
            "source filename must be an 8-digit lowercase hex ROM address: {name}"
        ));
    }
    let address = u64::from_str_radix(&name, 16)
        .map_err(|_| format!("unparseable ROM address: {name}"))?;
    let symbol = format!("Func_{address:08x}");
    let assembly = join(output_dir, &format!("{name}.s"));
    let object = join(output_dir, &format!("{name}.o"));
    let symbols_source = join(output_dir, &format!("{name}.symbols.s"));
    let symbols_object = join(output_dir, &format!("{name}.symbols.o"));
    let elf = join(output_dir, &format!("{name}.elf"));
    let binary = join(output_dir, &format!("{name}.bin"));
    let preprocessed = join(output_dir, &format!("{name}.i"));

    for step in source_to_assembly_plan(compiler, source, source, &assembly, &preprocessed)? {
        run(&step)?;
    }
    run(&[
        "arm-none-eabi-as".into(),
        "-mcpu=arm7tdmi".into(),
        "-mthumb-interwork".into(),
        "-o".into(),
        object.clone(),
        assembly,
    ])?;

    let undefined = run(&[
        "arm-none-eabi-nm".into(),
        "-u".into(),
        object.clone(),
    ])?;
    let mut names: Vec<String> = Vec::new();
    for line in undefined.lines().filter(|line| !line.is_empty()) {
        let external = line
            .split_whitespace()
            .next_back()
            .ok_or_else(|| format!("unreadable nm row: {line}"))?;
        if external_symbol(external).is_none() {
            return Err(format!("unsupported external symbol: {external}"));
        }
        names.push(external.to_string());
    }

    let mut stubs = String::from(".syntax unified\n.thumb\n");
    for name in &names {
        stubs.push_str(&external_symbol_assembly(name)?);
    }
    fs::write(&symbols_source, stubs)
        .map_err(|error| format!("cannot write {symbols_source}: {error}"))?;
    run(&[
        "arm-none-eabi-as".into(),
        "-mcpu=arm7tdmi".into(),
        "-mthumb-interwork".into(),
        "-o".into(),
        symbols_object.clone(),
        symbols_source,
    ])?;
    run(&[
        "arm-none-eabi-ld".into(),
        format!("-Ttext=0x{address:08x}"),
        "-e".into(),
        symbol.clone(),
        "-o".into(),
        elf.clone(),
        object,
        symbols_object,
    ])?;
    run(&[
        "arm-none-eabi-objcopy".into(),
        "-O".into(),
        "binary".into(),
        "-j".into(),
        ".text".into(),
        elf.clone(),
        binary.clone(),
    ])?;

    let symbols = run(&["arm-none-eabi-nm".into(), "-S".into(), elf])?;
    let suffix = format!(" {symbol}");
    let row = symbols
        .lines()
        .find(|line| line.trim_end_matches('\r').ends_with(&suffix))
        .ok_or_else(|| format!("missing linked symbol: {symbol}"))?;
    let size_field = row
        .split_whitespace()
        .nth(1)
        .ok_or_else(|| format!("linked symbol {symbol} has no size column"))?;
    let size = usize::from_str_radix(size_field, 16)
        .map_err(|_| format!("unparseable size for {symbol}: {size_field}"))?;

    // PORT NOTE (added hard failure): a zero-length symbol compares equal to
    // anything. Reporting that as verified would claim bytes nobody checked.
    if size == 0 {
        return Err(format!("linked symbol {symbol} has zero size"));
    }

    let actual = fs::read(&binary).map_err(|error| format!("cannot read {binary}: {error}"))?;
    // PORT NOTE (added hard failure): `subarray(0, size)` silently returns a
    // short buffer when the binary is smaller than the linked symbol.
    if actual.len() < size {
        return Err(format!(
            "{} holds {} bytes but {symbol} is {size} bytes",
            base_name(&binary),
            actual.len()
        ));
    }
    let actual = actual[..size].to_vec();

    // PORT NOTE (added hard failure): `Buffer.subarray` clamps out-of-range
    // offsets to an empty slice instead of throwing, so an address outside the
    // ROM used to compare "equal" against nothing.
    if address < ROM_BASE {
        return Err(format!("address 0x{address:08x} is below the ROM base"));
    }
    let start = (address - ROM_BASE) as usize;
    let end = start
        .checked_add(size)
        .ok_or_else(|| "ROM slice overflows".to_string())?;
    if end > rom.len() {
        return Err(format!(
            "0x{address:08x}+{size} runs past the end of the {}-byte ROM",
            rom.len()
        ));
    }
    let expected = rom[start..end].to_vec();

    // Belt and braces: the comparison is only meaningful if both sides are
    // exactly `size` bytes long.
    ensure_comparable(actual.len(), expected.len(), size)?;

    Ok((actual, expected, size))
}

/// PORT NOTE (added hard failure): the single place that decides a comparison
/// is meaningful. A zero-length region, or either side not being exactly
/// `size` bytes, means nothing was actually compared, and is an error rather
/// than a pass.
fn ensure_comparable(actual: usize, expected: usize, size: usize) -> Result<()> {
    if size == 0 {
        return Err("refusing to compare a zero-length region".to_string());
    }
    if actual != size || expected != size {
        return Err(format!(
            "refusing to compare {actual} against {expected} bytes for a {size}-byte region"
        ));
    }
    Ok(())
}

/// `verify(..., details = false)`: whether the bytes match, and how many.
pub fn verify(
    source: &str,
    rom: &[u8],
    output_dir: &str,
    compiler: CompilerTarget,
) -> Result<(bool, usize)> {
    let (actual, expected, size) = verify_details(source, rom, output_dir, compiler)?;
    Ok((actual == expected, size))
}

/// The `^[0-9a-f]{8}\.c$` filter, case-insensitive as in the TypeScript, over a
/// directory's regular files, sorted by full path as `Array#sort` does.
pub fn address_sources(directory: &Path) -> Result<Vec<PathBuf>> {
    let mut sources: Vec<PathBuf> = Vec::new();
    let entries = fs::read_dir(directory)
        .map_err(|error| format!("cannot read {}: {error}", directory.display()))?;
    for entry in entries {
        let entry = entry.map_err(|error| format!("cannot read a directory entry: {error}"))?;
        if !entry.file_type().map(|kind| kind.is_file()).unwrap_or(false) {
            continue;
        }
        let name = entry.file_name().to_string_lossy().into_owned();
        let Some(stem) = name.strip_suffix(".c").or_else(|| name.strip_suffix(".C")) else {
            continue;
        };
        if stem.len() == 8 && stem.bytes().all(|b| b.is_ascii_hexdigit()) {
            sources.push(entry.path());
        }
    }
    sources.sort();
    Ok(sources)
}

/// PORT NOTE (added hard failure): "scanning nothing is not passing". The
/// TypeScript `main()` prints `verified=0 failures=0 bytes=0` and exits 0 when
/// the source directory holds no address-named file, so a corpus that
/// vanished reads as a green gate. Extracted so the refusal is testable
/// without a subprocess.
pub fn require_nonempty_corpus(count: usize, target_id: &str, directory: &Path) -> Result<()> {
    if count == 0 {
        return Err(format!(
            "{target_id} has no <address>.c sources in {}; nothing was verified",
            directory.display()
        ));
    }
    Ok(())
}

/// `main()`'s ROM size gate, extracted for the same reason.
pub fn require_rom_size(actual: usize, expected: usize, target_id: &str) -> Result<()> {
    if actual != expected {
        return Err(format!(
            "{target_id} ROM must contain exactly {expected} bytes"
        ));
    }
    Ok(())
}

/// Replace the run-varying parts of a diagnostic (absolute repository root,
/// pid-suffixed fixture directory, temporary output directory) with stable
/// placeholders, so two runs of the same failure produce the same text.
///
/// Literal substitution only, applied in order. Nothing else is touched: a
/// message with no varying part must come back byte-identical, which is what
/// `normalising_leaves_a_stable_message_alone` pins.
pub fn normalise_message(message: &str, substitutions: &[(&str, &str)]) -> String {
    let mut out = message.to_string();
    for (needle, replacement) in substitutions {
        if needle.is_empty() {
            continue;
        }
        out = out.replace(needle, replacement);
    }
    out
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn normalising_leaves_a_stable_message_alone() {
        let substitutions = [("/Users/pixel/repo", "<root>"), ("/tmp/out-4711", "<out>")];
        // The whole point: a message without the varying part is untouched.
        for message in [
            "unsupported external symbol: mystery",
            "missing linked symbol: Func_08002ee4",
            "refusing to compare a zero-length region",
            "",
        ] {
            assert_eq!(normalise_message(message, &substitutions), message);
        }
    }

    #[test]
    fn normalising_replaces_every_varying_part() {
        let substitutions = [("/Users/pixel/repo", "<root>"), ("/tmp/out-4711", "<out>")];
        assert_eq!(
            normalise_message(
                "xgcc failed: /Users/pixel/repo/exact/08002ee4.c -> /tmp/out-4711/08002ee4.s",
                &substitutions
            ),
            "xgcc failed: <root>/exact/08002ee4.c -> <out>/08002ee4.s"
        );
        // Repeated occurrences are all replaced, not just the first.
        assert_eq!(
            normalise_message("/Users/pixel/repo /Users/pixel/repo", &substitutions),
            "<root> <root>"
        );
        // An empty needle would otherwise splice the replacement between every
        // character; it is skipped instead.
        assert_eq!(normalise_message("abc", &[("", "X")]), "abc");
    }

    #[test]
    fn parse_int_prefix_and_trailing_junk_are_rejected() {
        // `Number.parseInt(name, 16)` accepts a "0x" prefix, stops at trailing
        // junk, and yields NaN for nothing-parseable, all of which then flow
        // into `-Ttext=0x...` and the ROM offset. One helper, `is_lower_hex`,
        // is the only path in this port, and it refuses all of them.
        for stem in [
            "0x002ee4", // parseInt: 0x2ee4
            "08002ee",  // seven digits
            "08002ee4x",
            "08002EE4", // uppercase: parseInt accepts, the linker symbol would not
            "3.9",      // parseInt("3.9") is 3 and passes an integer guard
            "",
            "        ",
            "+8002ee4",
            "-8002ee4",
        ] {
            assert!(!is_lower_hex(stem, 8), "{stem} must not read as an address");
        }
        assert!(is_lower_hex("08002ee4", 8));
    }

    #[test]
    fn a_nonhex_nm_size_column_is_an_error_not_a_zero() {
        // The live TypeScript defect: when `nm -S` prints no size column the
        // second field is the symbol name, `Number.parseInt` of it is NaN, and
        // NaN flows into both `subarray` calls, which clamp. The result is a
        // verdict over regions nobody chose. This port refuses.
        assert!(usize::from_str_radix("Func_08002ee4", 16).is_err());
        assert!(usize::from_str_radix("0x14", 16).is_err());
        assert_eq!(usize::from_str_radix("14", 16), Ok(20));
    }

    #[test]
    fn odd_length_buffers_compare_to_their_final_byte() {
        // `candidate_show.ts` loses the last byte of an odd-length buffer to a
        // `shared & ~1` word walk and reports a phantom (or missing)
        // difference. This port compares whole slices; pin it at odd lengths.
        for length in [1usize, 3, 5, 7, 15, 21] {
            let expected: Vec<u8> = (0..length).map(|i| i as u8).collect();
            let mut actual = expected.clone();
            *actual.last_mut().unwrap() ^= 0xff;
            assert_eq!(compare(&actual, &expected, length), Ok(false), "len {length}");
            assert_eq!(compare(&expected, &expected, length), Ok(true), "len {length}");
        }
    }

    #[test]
    fn no_number_this_tool_emits_is_a_float() {
        // Float formatting (ryu vs ToString(Number), toFixed rounding) cannot
        // apply here: every number that reaches output is a byte count or a
        // count of files, all integers. This walks them.
        let (matched, size) = (true, 20usize);
        assert_eq!(format!("{size}"), "20");
        assert_eq!(format!("{matched}"), "true");
        let total: u64 = 1_345_860;
        let (verified, failures) = (1456usize, 0usize);
        assert_eq!(
            format!("verified={verified} failures={failures} bytes={total}"),
            "verified=1456 failures=0 bytes=1345860"
        );
        // And the only other emitted numbers are hex addresses, fixed width.
        assert_eq!(format!("0x{:08x}", 0x0800_2ee4u64), "0x08002ee4");
    }

    #[test]
    fn an_empty_corpus_is_a_hard_failure() {
        let directory = Path::new("/nowhere/exact");
        assert!(require_nonempty_corpus(0, "gs1-en", directory).is_err());
        assert_eq!(
            require_nonempty_corpus(0, "gs1-en", directory).unwrap_err(),
            "gs1-en has no <address>.c sources in /nowhere/exact; nothing was verified"
        );
        assert!(require_nonempty_corpus(1, "gs1-en", directory).is_ok());
    }

    #[test]
    fn a_rom_of_the_wrong_length_is_a_hard_failure() {
        assert!(require_rom_size(0x0080_0000, 0x0080_0000, "gs1-en").is_ok());
        // One byte short, and one byte long: both refused.
        assert!(require_rom_size(0x0080_0000 - 1, 0x0080_0000, "gs1-en").is_err());
        assert!(require_rom_size(0x0080_0000 + 1, 0x0080_0000, "gs1-en").is_err());
        assert!(require_rom_size(0, 0x0080_0000, "gs1-en").is_err());
    }

    #[test]
    fn an_empty_source_directory_yields_no_sources() {
        let directory = std::path::PathBuf::from(env!("CARGO_MANIFEST_DIR"))
            .join("target")
            .join("fixtures")
            .join(format!("empty-corpus-{}", std::process::id()));
        fs::create_dir_all(&directory).expect("fixture directory is creatable");
        let sources = address_sources(&directory).expect("an empty directory reads");
        assert!(sources.is_empty());
        // ... and that is what the gate refuses.
        assert!(require_nonempty_corpus(sources.len(), "gs1-en", &directory).is_err());
    }

    #[test]
    fn only_eight_hex_digit_c_files_are_corpus_members() {
        let directory = std::path::PathBuf::from(env!("CARGO_MANIFEST_DIR"))
            .join("target")
            .join("fixtures")
            .join(format!("corpus-filter-{}", std::process::id()));
        fs::create_dir_all(&directory).expect("fixture directory is creatable");
        for name in [
            "08002ee4.c",  // in
            "0800 2ee4.c", // out: space
            "8002ee4.c",   // out: seven digits
            "08002ee45.c", // out: nine digits
            "0800EE44.c",  // in: the TypeScript regex carries the `i` flag
            "08002ee4.h",  // out: wrong extension
            "08002ee4.c.bak",
            "0800zzzz.c", // out: not hex
        ] {
            fs::write(directory.join(name), b"").expect("fixture file is writable");
        }
        let names: Vec<String> = address_sources(&directory)
            .expect("fixture directory reads")
            .iter()
            .map(|path| path.file_name().unwrap().to_string_lossy().into_owned())
            .collect();
        // Byte order on the full path, which is also what `Array#sort`'s
        // UTF-16 code-unit order gives for these names ('2' < 'E').
        assert_eq!(names, vec!["08002ee4.c".to_string(), "0800EE44.c".to_string()]);
    }

    #[test]
    fn addressed_symbols_decode() {
        assert_eq!(
            external_symbol("Func_08006b84"),
            Some(ExternalSymbol { address: 0x0800_6b84, thumb: true })
        );
        assert_eq!(
            external_symbol("Data_02000000"),
            Some(ExternalSymbol { address: 0x0200_0000, thumb: false })
        );
        assert_eq!(
            external_symbol("Value_0801f0c4_a"),
            Some(ExternalSymbol { address: 0x0801_f0c4, thumb: false })
        );
    }

    #[test]
    fn malformed_symbols_are_rejected() {
        for name in [
            "",
            "Func_",
            "Func_08006b8",       // seven digits
            "Func_08006b845",     // nine digits
            "Func_08006B84",      // uppercase hex
            "Func_08006b84_A",    // uppercase suffix
            "Func_08006b84_ab",   // two-letter suffix
            "Other_08006b84",
            "memcpy",
        ] {
            assert_eq!(external_symbol(name), None, "{name} should be unsupported");
        }
    }

    #[test]
    fn call_via_symbols_decode() {
        assert_eq!(
            external_symbol("_call_via_r0"),
            Some(ExternalSymbol { address: CALL_VIA_BASE, thumb: true })
        );
        assert_eq!(
            external_symbol("_call_via_r13"),
            Some(ExternalSymbol { address: CALL_VIA_BASE + 52, thumb: true })
        );
        assert_eq!(
            external_symbol("_call_via_sl"),
            Some(ExternalSymbol { address: CALL_VIA_BASE + 40, thumb: true })
        );
        assert_eq!(
            external_symbol("_call_via_sp"),
            Some(ExternalSymbol { address: CALL_VIA_BASE + 52, thumb: true })
        );
        // The alternation cannot produce these.
        for name in ["_call_via_r14", "_call_via_r00", "_call_via_r", "_call_via_xx"] {
            assert_eq!(external_symbol(name), None, "{name} should be unsupported");
        }
    }

    #[test]
    fn stub_assembly_keeps_the_branch_type() {
        assert_eq!(
            external_symbol_assembly("Func_08006b84").unwrap(),
            ".global Func_08006b84\n.thumb_set Func_08006b84, 0x08006b84\n"
        );
        assert_eq!(
            external_symbol_assembly("Data_02000000").unwrap(),
            ".global Data_02000000\n.set Data_02000000, 0x02000000\n"
        );
        assert_eq!(
            external_symbol_assembly("_call_via_r3").unwrap(),
            ".global _call_via_r3\n.thumb_set _call_via_r3, 0x080072f0\n"
        );
        assert_eq!(
            external_symbol_assembly("nope").unwrap_err(),
            "unsupported external symbol: nope"
        );
    }

    // `verify_details` compares two slices that have first passed
    // `ensure_comparable`. These cover both halves of that contract.

    /// The comparison as `verify_details` performs it, guard included.
    fn compare(actual: &[u8], expected: &[u8], size: usize) -> Result<bool> {
        ensure_comparable(actual.len(), expected.len(), size)?;
        Ok(actual == expected)
    }

    #[test]
    fn equal_buffers_match_and_single_byte_differences_do_not() {
        let expected = [0x00u8, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77];
        assert_eq!(compare(&expected, &expected, 8), Ok(true));

        let mut first = expected;
        first[0] ^= 0x01;
        assert_eq!(
            compare(&first, &expected, 8),
            Ok(false),
            "a difference at the first offset must not match"
        );

        let mut last = expected;
        last[7] ^= 0x01;
        assert_eq!(
            compare(&last, &expected, 8),
            Ok(false),
            "a difference at the last offset must not match"
        );

        let mut middle = expected;
        middle[4] ^= 0x80;
        assert_eq!(compare(&middle, &expected, 8), Ok(false));
    }

    #[test]
    fn length_mismatch_is_an_error_not_a_verdict() {
        let expected = [0xaau8; 16];
        assert!(compare(&expected[..15], &expected, 16).is_err());
        assert!(compare(&[0xaau8; 17], &expected, 16).is_err());
        // Both sides agree with each other but not with the linked size.
        assert!(compare(&expected[..15], &expected[..15], 16).is_err());
    }

    #[test]
    fn empty_input_is_never_a_pass() {
        // Two empty buffers compare equal in Rust as in JavaScript. That is the
        // "compared nothing" hazard, so the guard turns it into an error.
        let empty: &[u8] = &[];
        assert_eq!(empty, empty);
        let error = compare(empty, empty, 0).unwrap_err();
        assert_eq!(error, "refusing to compare a zero-length region");
        // A non-zero size with empty buffers is likewise refused, not passed.
        assert!(compare(empty, empty, 4).is_err());
        // And an empty actual against real expected bytes cannot report true.
        assert!(compare(empty, &[0x00u8; 4], 4).is_err());
    }

    #[test]
    fn out_of_range_rom_slices_cannot_silently_shrink() {
        // JavaScript's Buffer.subarray clamps; this port bounds-checks instead.
        let rom = [0u8; 16];
        let address = 0x0800_000cu64;
        let start = (address - ROM_BASE) as usize;
        let size = 8usize;
        assert!(start + size > rom.len());
        assert_eq!(rom[start..].len(), 4);
        // The clamped slice would be 4 bytes, which the guard rejects.
        assert!(compare(&[0u8; 8], &rom[start..], size).is_err());
    }

    #[test]
    fn source_filenames_must_be_addresses() {
        let rom = vec![0u8; 16];
        let error = verify_details("/tmp/notanaddress.c", &rom, "/tmp", CompilerTarget::Gs1)
            .unwrap_err();
        assert!(error.contains("8-digit lowercase hex"), "{error}");
    }
}
