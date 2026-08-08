//! Build the complete ROM from source-owned regions.
//!
//! The pipeline drives the claimed-C, assembly, and asset builders, lays every produced region out in
//! address order, fills the gaps with `.incbin` slices of the base ROM,
//! assembles one object, links it with a generated linker script and finally
//! proves the result is byte-identical to the base ROM.
//!
//! PORT NOTEs (deliberate, documented differences):
//!
//! * PORT NOTE: the TS version spawns the sub-builders via `process.execPath`,
//!   which is the absolute path of the running Bun binary. Rust has no
//!   equivalent, so the port spawns `bun` (resolved through `PATH`), or the
//!   binary named by the `ALCHEMY_BUN` environment variable when set. The
//!   `basename()` used in failure messages is `bun` either way, so the error
//!   text is unchanged.
//! * PORT NOTE: an uncaught `throw` in Bun prints a source-annotated stack
//!   trace and exits 1. The port prints `error: <message>` to stderr and also
//!   exits 1. Every message the tool itself composes is reproduced verbatim,
//!   including the text propagated out of a failed sub-build.
//! * PORT NOTE: filesystem failures surface the host runtime's own wording, so
//!   a missing ROM reads `<path>: No such file or directory (os error 2)`
//!   instead of Node's `ENOENT: no such file or directory, open '<path>'`.
//!   The exit code (1) and the empty stdout are unchanged.

use std::path::{Path, PathBuf};
use std::process::Command;

pub const ROM_BASE: u64 = 0x0800_0000;

/// The repository root, derived the same way the TS file derives `ROOT`
/// (three `dirname` steps up from `tools/make/build_rom.ts`).
pub fn repo_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap()
        .to_path_buf()
}

// ---------------------------------------------------------------------------
// JavaScript semantics helpers
// ---------------------------------------------------------------------------

/// `Number.parseInt(value, 10)`.
///
/// Returns `None` for `NaN` (no digits consumed). Radix is fixed at 10, so
/// `"0x10"` yields `0` and `"1e9"` yields `1`, exactly like JS. The result is
/// an `f64` because JS produces a `Number`, and the caller's
/// `Number.isInteger` check must see the same value (`"1e999"` -> `1`,
/// `"99999999999999999999"` -> `1e20`, which *is* an integer in JS).
pub fn js_parse_int(value: &str) -> Option<f64> {
    let bytes: Vec<char> = value.chars().collect();
    let mut index = 0usize;
    // StrWhiteSpace per the spec (the common subset is enough here, but we
    // include the full set JS trims).
    while index < bytes.len() && is_js_whitespace(bytes[index]) {
        index += 1;
    }
    let mut negative = false;
    if index < bytes.len() && (bytes[index] == '+' || bytes[index] == '-') {
        negative = bytes[index] == '-';
        index += 1;
    }
    let start = index;
    while index < bytes.len() && bytes[index].is_ascii_digit() {
        index += 1;
    }
    if index == start {
        return None;
    }
    let digits: String = bytes[start..index].iter().collect();
    // Parsing through f64 reproduces JS's rounding for oversized literals.
    let magnitude: f64 = digits.parse().unwrap_or(f64::INFINITY);
    Some(if negative { -magnitude } else { magnitude })
}

fn is_js_whitespace(c: char) -> bool {
    matches!(
        c,
        '\u{9}'
            | '\u{a}'
            | '\u{b}'
            | '\u{c}'
            | '\u{d}'
            | '\u{20}'
            | '\u{a0}'
            | '\u{1680}'
            | '\u{2000}'..='\u{200a}'
            | '\u{2028}'
            | '\u{2029}'
            | '\u{202f}'
            | '\u{205f}'
            | '\u{3000}'
            | '\u{feff}'
    )
}

/// `Buffer.prototype.subarray` clamping (identical to `TypedArray#subarray`).
pub fn js_subarray(buffer: &[u8], start: i64, end: i64) -> &[u8] {
    let len = buffer.len() as i64;
    let clamp = |v: i64| -> usize {
        let v = if v < 0 { len + v } else { v };
        v.clamp(0, len) as usize
    };
    let lo = clamp(start);
    let hi = clamp(end);
    if hi <= lo {
        &buffer[0..0]
    } else {
        &buffer[lo..hi]
    }
}

/// `path.isAbsolute` on POSIX.
pub fn is_absolute(path: &str) -> bool {
    path.starts_with('/')
}

fn normalize_parts(path: &str) -> Vec<String> {
    let mut parts: Vec<String> = Vec::new();
    for segment in path.split('/') {
        match segment {
            "" | "." => {}
            ".." => {
                parts.pop();
            }
            other => parts.push(other.to_string()),
        }
    }
    parts
}

/// `path.resolve(base, path)` for absolute POSIX `base`.
pub fn resolve(base: &Path, path: &str) -> PathBuf {
    let joined = if is_absolute(path) {
        path.to_string()
    } else {
        format!("{}/{}", base.to_string_lossy(), path)
    };
    let parts = normalize_parts(&joined);
    if parts.is_empty() {
        PathBuf::from("/")
    } else {
        PathBuf::from(format!("/{}", parts.join("/")))
    }
}

/// `path.relative(from, to)` for absolute POSIX inputs.
pub fn relative(from: &Path, to: &Path) -> String {
    let from_parts = normalize_parts(&from.to_string_lossy());
    let to_parts = normalize_parts(&to.to_string_lossy());
    let mut common = 0usize;
    while common < from_parts.len()
        && common < to_parts.len()
        && from_parts[common] == to_parts[common]
    {
        common += 1;
    }
    let mut out: Vec<&str> = vec![".."; from_parts.len() - common];
    out.extend(to_parts[common..].iter().map(String::as_str));
    out.join("/")
}

/// `n.toString(16).padStart(8, "0")`.
pub fn hex8(value: u64) -> String {
    format!("{value:08x}")
}

/// `n.toString(16)`.
pub fn hex(value: u64) -> String {
    format!("{value:x}")
}

// ---------------------------------------------------------------------------
// Argument parsing
// ---------------------------------------------------------------------------

#[derive(Debug, Clone, PartialEq)]
pub struct Options {
    pub rom: String,
    pub output: String,
    pub jobs: f64,
}

impl Default for Options {
    fn default() -> Self {
        Options {
            rom: "roms/gs1-en.gba".to_string(),
            output: "out/rom".to_string(),
            jobs: 8.0,
        }
    }
}

pub const USAGE: &str = "usage: build_rom.ts [-h] [-o OUTPUT] [--jobs JOBS] [rom]";

/// The outcome of `parseArgs`: either options, or the `-h` early exit.
#[derive(Debug, Clone, PartialEq)]
pub enum ParseOutcome {
    Options(Options),
    Help,
}

pub fn parse_args(argv: &[String]) -> Result<ParseOutcome, String> {
    let mut options = Options::default();
    let mut positional = false;
    let mut index = 0usize;
    while index < argv.len() {
        let argument = &argv[index];
        if argument == "-h" || argument == "--help" {
            return Ok(ParseOutcome::Help);
        }
        let equal = argument.find('=');
        let option = match equal {
            Some(at) => &argument[..at],
            None => argument.as_str(),
        };
        if option == "-o" || option == "--output" || option == "--jobs" {
            let value = match equal {
                Some(at) => Some(argument[at + 1..].to_string()),
                None => {
                    index += 1;
                    argv.get(index).cloned()
                }
            };
            let value = match value {
                Some(value) => value,
                None => return Err(format!("{option} requires a value")),
            };
            if option == "--jobs" {
                options.jobs = js_parse_int(&value).unwrap_or(f64::NAN);
            } else {
                options.output = value;
            }
        } else if !argument.starts_with('-') && !positional {
            options.rom = argument.clone();
            positional = true;
        } else {
            return Err(format!("unrecognized argument: {argument}"));
        }
        index += 1;
    }
    // `!Number.isInteger(options.jobs) || options.jobs < 1`
    if !(options.jobs.is_finite() && options.jobs.fract() == 0.0) || options.jobs < 1.0 {
        return Err("jobs must be positive".to_string());
    }
    Ok(ParseOutcome::Options(options))
}

/// `String(args.jobs)` for the value handed to `build_claimed.ts`.
pub fn js_number_to_string(value: f64) -> String {
    if value.fract() == 0.0 && value.abs() < 1e21 {
        format!("{}", value as i128)
    } else {
        format!("{value}")
    }
}

// ---------------------------------------------------------------------------
// Piece layout
// ---------------------------------------------------------------------------

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Kind {
    Incbin(String),
    Skeleton,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Piece {
    pub address: u64,
    pub size: u64,
    pub kind: Kind,
}

/// `pieces.sort((l, r) => l[0] - r[0] || l[1] - r[1])` — JS `Array#sort` is
/// stable, so `sort_by` (also stable) is the faithful counterpart.
pub fn sort_pieces(pieces: &mut [Piece]) {
    pieces.sort_by(|left, right| {
        left.address
            .cmp(&right.address)
            .then(left.size.cmp(&right.size))
    });
}

/// Insert skeleton pieces for every gap, and reject overlaps.
pub fn fill_gaps(pieces: &[Piece], rom_len: u64) -> Result<Vec<Piece>, String> {
    let mut filled: Vec<Piece> = Vec::new();
    let mut cursor = ROM_BASE;
    for piece in pieces {
        if piece.address < cursor {
            return Err(format!(
                "overlapping piece at 0x{}",
                hex8(piece.address)
            ));
        }
        if piece.address > cursor {
            filled.push(Piece {
                address: cursor,
                size: piece.address - cursor,
                kind: Kind::Skeleton,
            });
        }
        filled.push(piece.clone());
        cursor = piece.address + piece.size;
    }
    if cursor < ROM_BASE + rom_len {
        filled.push(Piece {
            address: cursor,
            size: ROM_BASE + rom_len - cursor,
            kind: Kind::Skeleton,
        });
    }
    Ok(filled)
}

/// Build the `rom_fill.s` body and the linker `KEEP(...)` placements.
pub fn render_fill(
    filled: &[Piece],
    rom_path: &str,
    fill_object_relative: &str,
) -> (Vec<String>, Vec<String>) {
    let mut fill_lines = vec![".syntax unified".to_string()];
    let mut placements = Vec::new();
    for piece in filled {
        let name = format!(".rom_{}", hex8(piece.address));
        fill_lines.push(format!("\t.section {name}, \"ax\", %progbits"));
        match &piece.kind {
            Kind::Incbin(payload) => fill_lines.push(format!("\t.incbin \"{payload}\"")),
            Kind::Skeleton => fill_lines.push(format!(
                "\t.incbin \"{}\", 0x{}, 0x{}",
                rom_path,
                hex(piece.address - ROM_BASE),
                hex(piece.size)
            )),
        }
        placements.push(format!("    KEEP({fill_object_relative}({name}))"));
    }
    (fill_lines, placements)
}

pub fn render_linker_script(placements: &[String]) -> String {
    let mut script = String::new();
    script.push_str("OUTPUT_ARCH(arm)\n");
    script.push_str("MEMORY { ROM (rx) : ORIGIN = 0x08000000, LENGTH = 32M }\n");
    script.push_str("SECTIONS\n{\n");
    script.push_str("  .rom 0x08000000 : SUBALIGN(1)\n  {\n");
    script.push_str(&placements.join("\n"));
    script.push_str("\n  } > ROM\n");
    script.push_str("  /DISCARD/ : { *(.comment) *(.note*) *(.ARM.*) }\n");
    script.push_str("}\n");
    script
}

/// The mismatch error the TS raises when the linked image differs.
pub fn mismatch_error(built: &[u8], rom: &[u8]) -> String {
    let common = built.len().min(rom.len());
    let mut first: Option<usize> = None;
    for index in 0..common {
        if built[index] != rom[index] {
            first = Some(index);
            break;
        }
    }
    match first {
        Some(first) => format!(
            "linked ROM differs (size 0x{} vs 0x{}, first diff at 0x{})",
            hex(built.len() as u64),
            hex(rom.len() as u64),
            hex8(ROM_BASE + first as u64)
        ),
        None => format!(
            "linked ROM size 0x{} vs 0x{}",
            hex(built.len() as u64),
            hex(rom.len() as u64)
        ),
    }
}

/// The single stdout line the tool prints on success.
pub fn summary_line(filled: &[Piece], rom_len: u64) -> String {
    let claimed_bytes: u64 = filled
        .iter()
        .filter(|piece| piece.kind != Kind::Skeleton)
        .map(|piece| piece.size)
        .sum();
    let regions = filled
        .iter()
        .filter(|piece| piece.kind != Kind::Skeleton)
        .count();
    format!(
        "identical=True pieces={} regions={regions} source_bytes={claimed_bytes} skeleton_bytes={}",
        filled.len(),
        rom_len - claimed_bytes
    )
}

// ---------------------------------------------------------------------------
// JSON helpers
// ---------------------------------------------------------------------------

/// `readFileSync(path, "utf8")` replaces invalid UTF-8 with U+FFFD rather than
/// failing, so the port uses `String::from_utf8_lossy` for the same behaviour.
pub fn read_text(path: &Path) -> Result<String, String> {
    let bytes = std::fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?;
    Ok(String::from_utf8_lossy(&bytes).into_owned())
}

pub fn read_json(path: &Path) -> Result<serde_json::Value, String> {
    let text = read_text(path)?;
    serde_json::from_str(&text).map_err(|error| format!("{}: {error}", path.display()))
}

/// `Number(value)` for a JSON field that may be a number or a numeric string.
pub fn json_number(value: &serde_json::Value) -> Option<f64> {
    match value {
        serde_json::Value::Number(number) => number.as_f64(),
        serde_json::Value::String(text) => {
            let trimmed = text.trim_matches(is_js_whitespace);
            if trimmed.is_empty() {
                Some(0.0)
            } else {
                trimmed.parse::<f64>().ok()
            }
        }
        serde_json::Value::Bool(flag) => Some(if *flag { 1.0 } else { 0.0 }),
        serde_json::Value::Null => Some(0.0),
        _ => None,
    }
}

// ---------------------------------------------------------------------------
// Subprocess
// ---------------------------------------------------------------------------

/// `run(command)` — spawn in `ROOT`, capture both streams, throw on non-zero.
pub fn run(root: &Path, command: &[String]) -> Result<String, String> {
    let output = Command::new(&command[0])
        .args(&command[1..])
        .current_dir(root)
        .output()
        .map_err(|error| format!("{}: {error}", command[0]))?;
    let stdout = String::from_utf8_lossy(&output.stdout).into_owned();
    let stderr = String::from_utf8_lossy(&output.stderr).into_owned();
    if !output.status.success() {
        let picked = if stderr.is_empty() { &stdout } else { &stderr };
        let trimmed = picked.trim();
        if trimmed.is_empty() {
            let base = command[0].rsplit('/').next().unwrap_or(&command[0]);
            return Err(format!("{base} failed"));
        }
        return Err(trimmed.to_string());
    }
    Ok(stdout)
}

fn bun_binary() -> String {
    std::env::var("ALCHEMY_BUN").unwrap_or_else(|_| "bun".to_string())
}

// ---------------------------------------------------------------------------
// Main pipeline
// ---------------------------------------------------------------------------

pub fn main_pipeline(options: &Options, cwd: &Path) -> Result<String, String> {
    let root = repo_root();
    let rom_path = resolve(cwd, &options.rom);
    let rom = std::fs::read(&rom_path).map_err(|error| format!("{}: {error}", rom_path.display()))?;
    let out = if is_absolute(&options.output) {
        PathBuf::from(&options.output)
    } else {
        resolve(&root, &options.output)
    };
    std::fs::create_dir_all(&out).map_err(|error| format!("{}: {error}", out.display()))?;

    let rom_path_string = rom_path.to_string_lossy().into_owned();
    let bun = bun_binary();
    run(
        &root,
        &[
            root.join("tools-rs/build-claimed/target/release/build-claimed")
                .to_string_lossy()
                .into_owned(),
            rom_path_string.clone(),
            "--jobs".into(),
            js_number_to_string(options.jobs),
            "--output".into(),
            "out/claimed".into(),
        ],
    )?;
    run(
        &root,
        &[
            root.join("tools-rs/build-asm/target/release/build-asm")
                .to_string_lossy()
                .into_owned(),
            rom_path_string.clone(),
            "--output".into(),
            "out/asm".into(),
        ],
    )?;
    run(
        &root,
        &[
            bun,
            "tools/make/build_assets.ts".into(),
            rom_path_string.clone(),
            "--output".into(),
            "out/assets".into(),
        ],
    )?;

    let bytes_dir = out.join("regions");
    std::fs::create_dir_all(&bytes_dir)
        .map_err(|error| format!("{}: {error}", bytes_dir.display()))?;

    let mut pieces: Vec<Piece> = Vec::new();
    let claimed = read_json(&root.join("out/claimed/manifest.json"))?;
    let image = std::fs::read(root.join("out/claimed/claimed.bin"))
        .map_err(|error| format!("out/claimed/claimed.bin: {error}"))?;
    let image_base = json_number(&claimed["image_base"]).ok_or("image_base is not a number")?;
    let claimed_regions = claimed["regions"]
        .as_array()
        .ok_or("out/claimed/manifest.json: regions is not an array")?;
    for region in claimed_regions {
        let address = json_number(&region["address"]).ok_or("address is not a number")?;
        let size = json_number(&region["size"]).ok_or("size is not a number")?;
        let start = address - image_base;
        let blob = bytes_dir.join(format!("{}.bin", hex8(address as u64)));
        let slice = js_subarray(&image, start as i64, (start + size) as i64);
        std::fs::write(&blob, slice).map_err(|error| format!("{}: {error}", blob.display()))?;
        pieces.push(Piece {
            address: address as u64,
            size: size as u64,
            kind: Kind::Incbin(relative(&root, &blob)),
        });
    }

    for manifest in ["out/asm/manifest.json", "out/assets/manifest.json"] {
        let parsed = read_json(&root.join(manifest))?;
        let regions = parsed["regions"]
            .as_array()
            .ok_or_else(|| format!("{manifest}: regions is not an array"))?;
        for region in regions {
            let address = json_number(&region["address"]).ok_or("address is not a number")?;
            let size = json_number(&region["size"]).ok_or("size is not a number")?;
            let output = region["output"]
                .as_str()
                .ok_or_else(|| format!("{manifest}: region output is not a string"))?;
            pieces.push(Piece {
                address: address as u64,
                size: size as u64,
                kind: Kind::Incbin(relative(&root, Path::new(output))),
            });
        }
    }

    sort_pieces(&mut pieces);
    let filled = fill_gaps(&pieces, rom.len() as u64)?;

    let fill_object = out.join("rom_fill.o");
    let (fill_lines, placements) = render_fill(
        &filled,
        &rom_path_string,
        &relative(&root, &fill_object),
    );
    let fill_source = out.join("rom_fill.s");
    std::fs::write(&fill_source, fill_lines.join("\n") + "\n")
        .map_err(|error| format!("{}: {error}", fill_source.display()))?;
    run(
        &root,
        &[
            "arm-none-eabi-as".into(),
            "-mcpu=arm7tdmi".into(),
            "-mthumb-interwork".into(),
            "-o".into(),
            fill_object.to_string_lossy().into_owned(),
            fill_source.to_string_lossy().into_owned(),
        ],
    )?;

    let linker = out.join("ld_script.ld");
    std::fs::write(&linker, render_linker_script(&placements))
        .map_err(|error| format!("{}: {error}", linker.display()))?;
    let elf = out.join("rom.elf");
    run(
        &root,
        &[
            "arm-none-eabi-ld".into(),
            "-T".into(),
            linker.to_string_lossy().into_owned(),
            "-o".into(),
            elf.to_string_lossy().into_owned(),
            relative(&root, &fill_object),
        ],
    )?;
    let built_path = out.join("rom.gba");
    run(
        &root,
        &[
            "arm-none-eabi-objcopy".into(),
            "-O".into(),
            "binary".into(),
            elf.to_string_lossy().into_owned(),
            built_path.to_string_lossy().into_owned(),
        ],
    )?;
    let built = std::fs::read(&built_path)
        .map_err(|error| format!("{}: {error}", built_path.display()))?;
    if built != rom {
        return Err(mismatch_error(&built, &rom));
    }
    Ok(summary_line(&filled, rom.len() as u64))
}

#[cfg(test)]
mod tests {
    use super::*;

    // --- argument parsing -------------------------------------------------

    #[test]
    fn defaults_match_the_typescript() {
        let parsed = parse_args(&[]).unwrap();
        assert_eq!(
            parsed,
            ParseOutcome::Options(Options {
                rom: "roms/gs1-en.gba".into(),
                output: "out/rom".into(),
                jobs: 8.0,
            })
        );
    }

    #[test]
    fn help_flags_short_circuit() {
        assert_eq!(parse_args(&["-h".to_string()]).unwrap(), ParseOutcome::Help);
        assert_eq!(
            parse_args(&["--help".to_string()]).unwrap(),
            ParseOutcome::Help
        );
        // `-h` wins even after other arguments, and even after an error-looking one.
        assert_eq!(
            parse_args(&["--jobs".to_string(), "4".to_string(), "-h".to_string()]).unwrap(),
            ParseOutcome::Help
        );
    }

    #[test]
    fn separated_and_equals_forms_agree() {
        let separated = parse_args(&[
            "-o".to_string(),
            "out/x".to_string(),
            "--jobs".to_string(),
            "3".to_string(),
            "roms/gs2-en.gba".to_string(),
        ])
        .unwrap();
        let equals = parse_args(&[
            "-o=out/x".to_string(),
            "--jobs=3".to_string(),
            "roms/gs2-en.gba".to_string(),
        ])
        .unwrap();
        assert_eq!(separated, equals);
        assert_eq!(
            separated,
            ParseOutcome::Options(Options {
                rom: "roms/gs2-en.gba".into(),
                output: "out/x".into(),
                jobs: 3.0,
            })
        );
    }

    #[test]
    fn long_output_flag_is_accepted() {
        let parsed = parse_args(&["--output".to_string(), "out/y".to_string()]).unwrap();
        match parsed {
            ParseOutcome::Options(options) => assert_eq!(options.output, "out/y"),
            other => panic!("unexpected {other:?}"),
        }
    }

    #[test]
    fn only_the_first_positional_is_taken() {
        let error = parse_args(&["a.gba".to_string(), "b.gba".to_string()]).unwrap_err();
        assert_eq!(error, "unrecognized argument: b.gba");
    }

    #[test]
    fn unknown_dash_argument_errors() {
        assert_eq!(
            parse_args(&["--nope".to_string()]).unwrap_err(),
            "unrecognized argument: --nope"
        );
        // An `=`-carrying unknown flag reports the *whole* argument, not the prefix.
        assert_eq!(
            parse_args(&["--nope=1".to_string()]).unwrap_err(),
            "unrecognized argument: --nope=1"
        );
    }

    #[test]
    fn missing_value_errors_with_the_option_name() {
        assert_eq!(
            parse_args(&["-o".to_string()]).unwrap_err(),
            "-o requires a value"
        );
        assert_eq!(
            parse_args(&["--jobs".to_string()]).unwrap_err(),
            "--jobs requires a value"
        );
    }

    #[test]
    fn empty_equals_value_is_a_value_not_a_miss() {
        // `--jobs=` gives "" -> parseInt("") is NaN -> "jobs must be positive".
        assert_eq!(
            parse_args(&["--jobs=".to_string()]).unwrap_err(),
            "jobs must be positive"
        );
        // `-o=` legitimately sets an empty output string.
        match parse_args(&["-o=".to_string()]).unwrap() {
            ParseOutcome::Options(options) => assert_eq!(options.output, ""),
            other => panic!("unexpected {other:?}"),
        }
    }

    #[test]
    fn a_positional_containing_equals_is_still_positional() {
        match parse_args(&["roms/a=b.gba".to_string()]).unwrap() {
            ParseOutcome::Options(options) => assert_eq!(options.rom, "roms/a=b.gba"),
            other => panic!("unexpected {other:?}"),
        }
    }

    #[test]
    fn flag_values_are_consumed_even_when_dash_prefixed() {
        // `-o -h` consumes "-h" as the output value; it is NOT the help flag.
        match parse_args(&["-o".to_string(), "-h".to_string()]).unwrap() {
            ParseOutcome::Options(options) => assert_eq!(options.output, "-h"),
            other => panic!("unexpected {other:?}"),
        }
    }

    #[test]
    fn bad_job_counts_are_rejected() {
        // Verified against Bun: parseInt("1.5") === 1, which the TS accepts,
        // so "1.5" is deliberately absent from this list.
        for value in ["0", "-1", "abc", "", " ", "0x10"] {
            assert_eq!(
                parse_args(&["--jobs".to_string(), value.to_string()]).unwrap_err(),
                "jobs must be positive",
                "value {value:?}"
            );
        }
    }

    // --- JS parseInt traps ------------------------------------------------

    #[test]
    fn parse_int_follows_javascript() {
        assert_eq!(js_parse_int("8"), Some(8.0));
        assert_eq!(js_parse_int("  12  "), Some(12.0));
        assert_eq!(js_parse_int("+7"), Some(7.0));
        assert_eq!(js_parse_int("-7"), Some(-7.0));
        // Trailing garbage is ignored, unlike Rust's `str::parse`.
        assert_eq!(js_parse_int("8abc"), Some(8.0));
        assert_eq!(js_parse_int("1.9"), Some(1.0));
        assert_eq!(js_parse_int("1e9"), Some(1.0));
        // Radix 10 means "0x10" stops at the `x`.
        assert_eq!(js_parse_int("0x10"), Some(0.0));
        assert_eq!(js_parse_int(""), None);
        assert_eq!(js_parse_int("abc"), None);
        assert_eq!(js_parse_int("+"), None);
    }

    #[test]
    fn fractional_job_counts_are_truncated_not_rejected() {
        // Bun: parseInt("1.5", 10) === 1 -> accepted.
        match parse_args(&["--jobs".to_string(), "1.5".to_string()]).unwrap() {
            ParseOutcome::Options(options) => assert_eq!(options.jobs, 1.0),
            other => panic!("unexpected {other:?}"),
        }
    }

    #[test]
    fn parse_int_trailing_garbage_still_builds() {
        // `--jobs 8abc` is accepted by the TS (parseInt -> 8); the port agrees.
        match parse_args(&["--jobs".to_string(), "8abc".to_string()]).unwrap() {
            ParseOutcome::Options(options) => assert_eq!(options.jobs, 8.0),
            other => panic!("unexpected {other:?}"),
        }
    }

    #[test]
    fn huge_job_counts_stay_integral_like_javascript() {
        // Number.isInteger(1e20) === true, so the TS accepts this too. A naive
        // `as i32` port would have truncated/saturated here.
        match parse_args(&["--jobs".to_string(), "99999999999999999999".to_string()]).unwrap() {
            ParseOutcome::Options(options) => {
                assert_eq!(options.jobs, 1e20);
                assert_eq!(js_number_to_string(options.jobs), "100000000000000000000");
            }
            other => panic!("unexpected {other:?}"),
        }
    }

    #[test]
    fn number_to_string_matches_js_for_ordinary_counts() {
        assert_eq!(js_number_to_string(8.0), "8");
        assert_eq!(js_number_to_string(1.0), "1");
        assert_eq!(js_number_to_string(14.0), "14");
    }

    // --- subarray clamping ------------------------------------------------

    #[test]
    fn subarray_clamps_like_node() {
        let data = [0u8, 1, 2, 3, 4];
        assert_eq!(js_subarray(&data, 1, 3), &[1, 2]);
        assert_eq!(js_subarray(&data, 3, 99), &[3, 4]);
        assert_eq!(js_subarray(&data, 9, 12), &[] as &[u8]);
        assert_eq!(js_subarray(&data, 4, 2), &[] as &[u8]);
        // Negative start counts from the end — Rust slicing would panic here.
        assert_eq!(js_subarray(&data, -2, 5), &[3, 4]);
    }

    // --- path helpers -----------------------------------------------------

    #[test]
    fn resolve_and_relative_round_trip() {
        let root = Path::new("/repo");
        assert_eq!(resolve(root, "out/rom"), Path::new("/repo/out/rom"));
        assert_eq!(resolve(root, "/abs/rom"), Path::new("/abs/rom"));
        assert_eq!(resolve(root, "./a/../b"), Path::new("/repo/b"));
        assert_eq!(resolve(root, "a//b/"), Path::new("/repo/a/b"));
        assert_eq!(relative(root, Path::new("/repo/out/rom.o")), "out/rom.o");
        assert_eq!(relative(root, Path::new("/other/x")), "../other/x");
        assert_eq!(relative(root, Path::new("/repo")), "");
    }

    #[test]
    fn hex_formatting_is_lowercase_and_padded() {
        assert_eq!(hex8(0x0800_0000), "08000000");
        assert_eq!(hex8(0xdead_beef), "deadbeef");
        assert_eq!(hex8(0), "00000000");
        assert_eq!(hex(0x1ab), "1ab");
        assert_eq!(hex(0), "0");
    }

    // --- layout -----------------------------------------------------------

    fn incbin(address: u64, size: u64, payload: &str) -> Piece {
        Piece {
            address,
            size,
            kind: Kind::Incbin(payload.to_string()),
        }
    }

    #[test]
    fn sorting_is_by_address_then_size_and_stable() {
        let mut pieces = vec![
            incbin(0x0800_0010, 4, "c"),
            incbin(0x0800_0000, 8, "b"),
            incbin(0x0800_0000, 4, "a"),
            incbin(0x0800_0000, 4, "a2"),
        ];
        sort_pieces(&mut pieces);
        let payloads: Vec<&str> = pieces
            .iter()
            .map(|piece| match &piece.kind {
                Kind::Incbin(payload) => payload.as_str(),
                Kind::Skeleton => "skel",
            })
            .collect();
        // "a" before "a2" proves stability; "a"/"a2" before "b" proves the
        // size tiebreak.
        assert_eq!(payloads, ["a", "a2", "b", "c"]);
    }

    #[test]
    fn gaps_become_skeleton_pieces() {
        let pieces = vec![incbin(0x0800_0004, 4, "x")];
        let filled = fill_gaps(&pieces, 16).unwrap();
        assert_eq!(
            filled,
            vec![
                Piece {
                    address: 0x0800_0000,
                    size: 4,
                    kind: Kind::Skeleton
                },
                incbin(0x0800_0004, 4, "x"),
                Piece {
                    address: 0x0800_0008,
                    size: 8,
                    kind: Kind::Skeleton
                },
            ]
        );
    }

    #[test]
    fn a_fully_covered_rom_gets_no_skeleton() {
        let pieces = vec![incbin(0x0800_0000, 16, "x")];
        let filled = fill_gaps(&pieces, 16).unwrap();
        assert_eq!(filled.len(), 1);
        assert_eq!(summary_line(&filled, 16), "identical=True pieces=1 regions=1 source_bytes=16 skeleton_bytes=0");
    }

    #[test]
    fn overlaps_are_rejected_with_a_padded_address() {
        let pieces = vec![incbin(0x0800_0000, 8, "a"), incbin(0x0800_0004, 8, "b")];
        assert_eq!(
            fill_gaps(&pieces, 16).unwrap_err(),
            "overlapping piece at 0x08000004"
        );
    }

    #[test]
    fn summary_counts_only_non_skeleton_pieces() {
        let filled = fill_gaps(&[incbin(0x0800_0004, 4, "x")], 16).unwrap();
        assert_eq!(
            summary_line(&filled, 16),
            "identical=True pieces=3 regions=1 source_bytes=4 skeleton_bytes=12"
        );
    }

    // --- rendering --------------------------------------------------------

    #[test]
    fn fill_source_and_placements_render_exactly() {
        let filled = fill_gaps(&[incbin(0x0800_0004, 4, "out/rom/regions/08000004.bin")], 12)
            .unwrap();
        let (lines, placements) = render_fill(&filled, "/roms/gs1-en.gba", "out/rom/rom_fill.o");
        assert_eq!(
            lines,
            vec![
                ".syntax unified".to_string(),
                "\t.section .rom_08000000, \"ax\", %progbits".to_string(),
                "\t.incbin \"/roms/gs1-en.gba\", 0x0, 0x4".to_string(),
                "\t.section .rom_08000004, \"ax\", %progbits".to_string(),
                "\t.incbin \"out/rom/regions/08000004.bin\"".to_string(),
                "\t.section .rom_08000008, \"ax\", %progbits".to_string(),
                "\t.incbin \"/roms/gs1-en.gba\", 0x8, 0x4".to_string(),
            ]
        );
        assert_eq!(
            placements,
            vec![
                "    KEEP(out/rom/rom_fill.o(.rom_08000000))".to_string(),
                "    KEEP(out/rom/rom_fill.o(.rom_08000004))".to_string(),
                "    KEEP(out/rom/rom_fill.o(.rom_08000008))".to_string(),
            ]
        );
    }

    #[test]
    fn linker_script_renders_exactly() {
        let script = render_linker_script(&["    KEEP(a(.rom_08000000))".to_string()]);
        assert_eq!(
            script,
            "OUTPUT_ARCH(arm)\n\
             MEMORY { ROM (rx) : ORIGIN = 0x08000000, LENGTH = 32M }\n\
             SECTIONS\n{\n\
             \x20 .rom 0x08000000 : SUBALIGN(1)\n  {\n\
             \x20   KEEP(a(.rom_08000000))\n  } > ROM\n\
             \x20 /DISCARD/ : { *(.comment) *(.note*) *(.ARM.*) }\n\
             }\n"
        );
    }

    // --- mismatch reporting ----------------------------------------------

    #[test]
    fn mismatch_reports_the_first_differing_byte() {
        assert_eq!(
            mismatch_error(&[1, 2, 3], &[1, 9, 3]),
            "linked ROM differs (size 0x3 vs 0x3, first diff at 0x08000001)"
        );
    }

    #[test]
    fn pure_size_mismatch_has_no_first_diff() {
        assert_eq!(
            mismatch_error(&[1, 2, 3, 4], &[1, 2, 3]),
            "linked ROM size 0x4 vs 0x3"
        );
        assert_eq!(mismatch_error(&[], &[1]), "linked ROM size 0x0 vs 0x1");
    }

    // --- JSON / IO --------------------------------------------------------

    #[test]
    fn invalid_utf8_is_replaced_not_rejected() {
        let dir = std::env::temp_dir().join("build-rom-utf8-test");
        std::fs::create_dir_all(&dir).unwrap();
        let path = dir.join("bad.txt");
        std::fs::write(&path, [b'{', 0xff, b'}']).unwrap();
        let text = read_text(&path).unwrap();
        assert_eq!(text, "{\u{fffd}}");
        std::fs::remove_dir_all(&dir).unwrap();
    }

    #[test]
    fn json_numbers_accept_strings_like_the_number_cast() {
        assert_eq!(json_number(&serde_json::json!(134217728)), Some(134217728.0));
        assert_eq!(
            json_number(&serde_json::json!("134217728")),
            Some(134217728.0)
        );
        assert_eq!(json_number(&serde_json::json!("")), Some(0.0));
        assert_eq!(json_number(&serde_json::json!("nope")), None);
    }

    #[test]
    fn json_object_key_order_is_preserved() {
        // `preserve_order` is mandatory: re-serialising must not sort keys.
        let value: serde_json::Value =
            serde_json::from_str(r#"{"zebra":1,"alpha":2,"middle":3}"#).unwrap();
        assert_eq!(
            serde_json::to_string(&value).unwrap(),
            r#"{"zebra":1,"alpha":2,"middle":3}"#
        );
    }

    // --- subprocess -------------------------------------------------------

    #[test]
    fn run_returns_stdout_on_success() {
        let root = std::env::temp_dir();
        let out = run(
            &root,
            &["/bin/sh".into(), "-c".into(), "printf hello".into()],
        )
        .unwrap();
        assert_eq!(out, "hello");
    }

    #[test]
    fn run_prefers_trimmed_stderr_then_stdout_then_basename() {
        let root = std::env::temp_dir();
        // stderr wins, and is trimmed.
        assert_eq!(
            run(
                &root,
                &["/bin/sh".into(), "-c".into(), "echo '  boom  ' >&2; exit 3".into()]
            )
            .unwrap_err(),
            "boom"
        );
        // Empty stderr falls back to stdout.
        assert_eq!(
            run(
                &root,
                &["/bin/sh".into(), "-c".into(), "echo out-only; exit 4".into()]
            )
            .unwrap_err(),
            "out-only"
        );
        // Both empty falls back to `basename(command[0]) + " failed"`.
        assert_eq!(
            run(&root, &["/bin/sh".into(), "-c".into(), "exit 5".into()]).unwrap_err(),
            "sh failed"
        );
    }

    #[test]
    fn repo_root_is_the_alchemy_checkout() {
        let root = repo_root();
        assert!(root.join("tools-rs/build-rom/Cargo.toml").exists(), "{root:?}");
    }
}
