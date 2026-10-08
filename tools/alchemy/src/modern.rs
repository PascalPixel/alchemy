//! The modern build, pret's `make modern`: the same source, assets and
//! scaffold through a current GCC into a ROM that matches nothing. It shows
//! what the matching compilers hide (undefined behaviour, wrong prototypes and
//! types, FAKEMATCH devices that change meaning) and keeps every warning in
//! `out/modern/warnings.txt`. Its objects, scripts and images live under
//! `out/modern` only; rom.sha1 never names them.
//!
//! Camelot's C, Thumb and ARM, goes through the modern compiler (MODERN_CC,
//! by default Homebrew's arm-none-eabi-gcc) with one option set: pret's
//! `-mthumb -mthumb-interwork -O2 -mabi=apcs-gnu -mcpu=arm7tdmi
//! -fno-toplevel-reorder`, with `-marm` for the files the matching build
//! compiles as ARM. Nintendo's library C (MusicPlayer2000, flash) keeps its
//! matching compiler, as pret keeps m4a on old_agbcc: its sound driver
//! relies on behaviour a modern compiler changes. Assembly, MIDI, assets and
//! the scaffold build exactly as in the matching build. The rest of the
//! option set:
//! - `-std=gnu89`: the source is C89 (S3); GCC 15 and later default to C23,
//!   where `bool` is a keyword and `f()` means `f(void)`.
//! - `-fpermissive`: GCC 14 made implicit declarations, int/pointer
//!   conversions and incompatible pointer types errors; here they stay
//!   warnings, reported rather than stopping the build.
//! - `-fcall-used-r4`: the game's calling convention (K1), which its
//!   assembly relies on, not a matching device.
//! - `-fno-zero-initialized-in-bss`: Camelot's compiler kept zero-initialised
//!   tables in .data, which the game keeps in ROM; it clears no .bss.
//! - `-fno-tree-loop-distribute-patterns`: the game links no C library, so
//!   loops must not become memset or memcpy calls.
//! - `-ffreestanding -nostdinc`, `-DMODERN=1` for the source's guards, and
//!   `-Wall -Wextra -fdiagnostics-plain-output` for the report.
//!
//! Linking reuses the edition's linker script with GCC's extra section names
//! beside each object's own (`.rodata.str1.4` and the like), a catch-all ROM
//! section for interworking glue and runtime code, and the modern compiler's
//! own libgcc. The fixed-address and id gates judge the matching build only:
//! under MODERN, `FIXED_ADDRESS` calls the linked routine by name, and the
//! compiler folds ids into neighbouring constants.

use crate::build_rom::{link_with, Linked};
use crate::compiler::routing::{cflags_for_family, family_for_source, CompilerFamily};
use crate::targets::{decomp_target, DecompTarget};
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::{Path, PathBuf};

const USAGE: &str = "usage: alchemy build modern [--target GAME-EDITION]... [--cc COMPILER] [--keep-going]
Build each edition (all six of ☀️ by default) from the same source with a
modern GCC, as pret's `make modern`: out/modern/<edition>.gba, a ROM that
matches nothing, and every compiler and linker warning in
out/modern/warnings.txt with a summary by kind and file in
out/modern/warnings-summary.txt. The compiler is --cc, else $MODERN_CC,
else Homebrew's /opt/homebrew/bin/arm-none-eabi-gcc. --keep-going writes an
image despite link errors.";

const DEFAULT_CC: &str = "/opt/homebrew/bin/arm-none-eabi-gcc";
const OUTPUT: &str = "out/modern";

pub(crate) struct Modern {
    pub cc: String,
    /// The compiler's version and option set: part of every cache key.
    pub signature: String,
    /// The compiler's own runtime library, linked after the objects.
    pub libraries: Vec<PathBuf>,
}

/// The options every modern compile shares, before the instruction set.
const COMMON_FLAGS: &[&str] = &[
    "-mthumb-interwork",
    "-mcpu=arm7tdmi",
    "-mabi=apcs-gnu",
    "-O2",
    "-fno-toplevel-reorder",
    "-std=gnu89",
    "-fpermissive",
    "-ffreestanding",
    "-nostdinc",
    "-fno-zero-initialized-in-bss",
    "-fno-tree-loop-distribute-patterns",
    "-DMODERN=1",
    "-Wall",
    "-Wextra",
    "-fdiagnostics-plain-output",
    "-fcall-used-r4",
];

/// The modern options for a C file of `family`.
pub(crate) fn family_flags(family: CompilerFamily) -> Vec<String> {
    let mut flags = vec![if family.is_arm() { "-marm" } else { "-mthumb" }.to_owned()];
    flags.extend(COMMON_FLAGS.iter().map(|flag| (*flag).to_owned()));
    flags
}

impl Modern {
    pub(crate) fn new(cc: &str) -> Result<Modern, String> {
        let root = crate::compiler::routing::root();
        let version = crate::process::run(&[cc, "--version"], root)
            .map_err(|error| format!("modern compiler {cc}: {error}"))?;
        let version = version.lines().next().unwrap_or_default().to_owned();
        let libgcc =
            crate::process::run(&[cc, "-mthumb", "-mcpu=arm7tdmi", "-print-libgcc-file-name"], root)?;
        let libgcc = PathBuf::from(libgcc.trim());
        if !libgcc.is_absolute() || !libgcc.is_file() {
            return Err(format!("{cc} names no libgcc.a: {}", libgcc.display()));
        }
        let signature = format!(
            "{version}\0{}\0{}",
            COMMON_FLAGS.join(" "),
            libgcc.display()
        );
        Ok(Modern {
            cc: cc.to_owned(),
            signature,
            libraries: vec![libgcc],
        })
    }
}

/// Whether the modern build compiles `source` with the modern compiler: all
/// of Camelot's C, Thumb and ARM. Nintendo's library C (MusicPlayer2000 and
/// the flash library) keeps the compiler and options of the matching build,
/// as pret keeps m4a on old_agbcc: the sound driver relies on behaviour a
/// modern compiler changes, and the game crashes in it otherwise.
pub(crate) fn compiles(target: DecompTarget, source: &Path) -> bool {
    matches!(
        family_for_source(target.compiler, &source.to_string_lossy()),
        CompilerFamily::Game | CompilerFamily::GameArm
    )
}

impl Modern {
    /// One compile, source straight to object; its stderr is the warning log.
    pub(crate) fn c_steps(
        &self,
        target: DecompTarget,
        source: &Path,
        object: &Path,
    ) -> Vec<Vec<String>> {
        let text = source.to_string_lossy().into_owned();
        let family = family_for_source(target.compiler, &text);
        let mut step = vec![self.cc.clone()];
        step.extend(family_flags(family));
        step.extend(
            cflags_for_family(target.compiler, CompilerFamily::Game)
                .into_iter()
                .filter(|flag| flag.starts_with("-I")),
        );
        step.push(format!("-D{}=1", target.edition_define));
        if let Some(build) = object
            .ancestors()
            .find(|path| path.file_name().is_some_and(|name| name == "obj"))
            .and_then(Path::parent)
        {
            step.push(format!("-I{}", build.display()));
        }
        step.extend([
            "-c".to_owned(),
            "-o".to_owned(),
            object.to_string_lossy().into_owned(),
            "-x".to_owned(),
            "c".to_owned(),
            text,
        ]);
        vec![step]
    }
}

/// Section names GCC emits beside the ones Camelot's compiler did.
fn widened(sections: &str) -> String {
    sections
        .split_whitespace()
        .map(|section| match section {
            ".text" => ".text .text.unlikely .text.hot .text.startup".to_owned(),
            ".rodata" => ".rodata .rodata.str* .rodata.cst*".to_owned(),
            other => other.to_owned(),
        })
        .collect::<Vec<_>>()
        .join(" ")
}

const CATCH_ALL: &str = ".text .text.unlikely .text.hot .text.startup .glue_7 .glue_7t .vfp11_veneer .v4_bx .rodata .rodata.str* .rodata.cst* .data";

/// The main linker script for modern objects: every object's sections
/// widened to GCC's names, and a catch-all ROM section after the last one.
pub(crate) fn main_script(text: &str) -> String {
    let object = regex::Regex::new(r#"("\*/[^"]+\.o")\(([^)]*)\)"#).expect("static pattern");
    let mut lines: Vec<String> = text
        .lines()
        .map(|line| {
            object
                .replace_all(line, |capture: &regex::Captures| {
                    format!("{}({})", &capture[1], widened(&capture[2]))
                })
                .into_owned()
        })
        .collect();
    // After the last ROM section: the linker gives each section to the first
    // pattern that matches it, so a catch-all any earlier would take the
    // sections the script places later.
    if let Some(end) = lines.iter().rposition(|line| line.trim() == "} > ROM") {
        lines.insert(
            end + 1,
            format!("    .rom_modern : {{ *({CATCH_ALL}) }} > ROM"),
        );
    }
    lines.join("\n") + "\n"
}

/// An overlay's script for modern objects: an overlay has one image, so each
/// object's code, constants and data all land in its listed place.
pub(crate) fn overlay_text(text: &str) -> String {
    let object = regex::Regex::new(r#"("\*/[^"]+\.o")\(\.text\)"#).expect("static pattern");
    let mut out = object
        .replace_all(text, |capture: &regex::Captures| {
            format!("{}({CATCH_ALL})", &capture[1])
        })
        .into_owned();
    if !out.ends_with('\n') {
        out.push('\n');
    }
    out
}

pub(crate) fn overlay_script(
    root: &Path,
    script: &Path,
    directory: &Path,
    id: &str,
) -> Result<PathBuf, String> {
    let text = fs::read_to_string(root.join(script))
        .map_err(|error| format!("{}: {error}", script.display()))?;
    let path = directory.join(format!("resource_{id}.modern.ld"));
    write_if_changed(&path, &overlay_text(&text))?;
    Ok(path)
}

fn write_if_changed(path: &Path, text: &str) -> Result<(), String> {
    if fs::read_to_string(path).ok().as_deref() != Some(text) {
        fs::write(path, text).map_err(|error| format!("{}: {error}", path.display()))?;
    }
    Ok(())
}

pub fn run(args: &[String]) -> Result<(), String> {
    let mut targets = Vec::new();
    let mut cc = std::env::var("MODERN_CC")
        .ok()
        .filter(|cc| !cc.is_empty())
        .unwrap_or_else(|| DEFAULT_CC.to_owned());
    let mut keep_going = false;
    let mut args = args.iter();
    while let Some(arg) = args.next() {
        match arg.as_str() {
            "-h" | "--help" => {
                println!("{USAGE}");
                return Ok(());
            }
            "--target" => targets.push(decomp_target(Some(args.next().ok_or(USAGE)?))?),
            "--cc" => cc = args.next().ok_or(USAGE)?.clone(),
            "--keep-going" => keep_going = true,
            _ => return Err(USAGE.into()),
        }
    }
    if targets.is_empty() {
        for edition in ["ja", "en", "de", "es", "fr", "it"] {
            targets.push(decomp_target(Some(&format!("tbs-{edition}")))?);
        }
    }
    let modern = Modern::new(&cc)?;
    println!("modern compiler: {}", modern.signature.split('\0').next().unwrap_or(&cc));
    let root = crate::compiler::routing::root();
    let mut results = Vec::new();
    for target in &targets {
        let result = build(root, *target, &modern, keep_going);
        match &result {
            Ok(line) => println!("{}: {line}", target.id),
            Err(error) => println!("{}: FAILED\n{error}", target.id),
        }
        results.push((*target, result));
    }
    let report = collect_warnings(root, &targets)?;
    fs::write(root.join(OUTPUT).join("warnings.txt"), &report.log)
        .map_err(|error| error.to_string())?;
    let summary = summarise(&report.diagnostics);
    fs::write(root.join(OUTPUT).join("warnings-summary.txt"), &summary)
        .map_err(|error| error.to_string())?;
    print!("{summary}");
    let failed: Vec<String> = results
        .iter()
        .filter(|(_, result)| result.is_err())
        .map(|(target, _)| target.id.to_string())
        .collect();
    if failed.is_empty() {
        Ok(())
    } else {
        Err(format!("the modern build failed for {}", failed.join(", ")))
    }
}

fn build(
    root: &Path,
    target: DecompTarget,
    modern: &Modern,
    keep_going: bool,
) -> Result<String, String> {
    let name = target.id.as_str();
    let output = Path::new(OUTPUT).join(name);
    fs::create_dir_all(root.join(&output)).map_err(|error| error.to_string())?;
    let script = crate::edition::script(root, target, &output)?;
    let text = fs::read_to_string(root.join(&script))
        .map_err(|error| format!("{}: {error}", script.display()))?;
    let modern_script = output.join(format!("{name}.modern.ld"));
    write_if_changed(&root.join(&modern_script), &main_script(&text))?;
    let Linked { image, .. } = link_with(
        root,
        target,
        &modern_script,
        &output,
        name,
        keep_going,
        Some(modern),
    )?;
    let rom = root.join(OUTPUT).join(format!("{name}.gba"));
    fs::copy(&image, &rom).map_err(|error| format!("{}: {error}", rom.display()))?;
    let bytes = fs::read(&rom).map_err(|error| error.to_string())?;
    unplaced_data(&root.join(&output).join(format!("{name}.elf")))?;
    let header = header_check(&bytes)?;
    Ok(format!("linked {} ({} bytes); {header}", rom.display(), bytes.len()))
}

/// The cartridge header a GBA BIOS checks before it starts the ROM: what
/// `alchemy gbafix` writes must already be there, and the first word must be
/// the ARM branch to the entry point.
pub(crate) fn header_check(bytes: &[u8]) -> Result<String, String> {
    if bytes.len() < crate::gbafix::HEADER_SIZE {
        return Err("the image is shorter than a cartridge header".into());
    }
    let mut fixed = bytes[..crate::gbafix::HEADER_SIZE].to_vec();
    crate::gbafix::fix(&mut fixed, &crate::gbafix::Fields::default(), false);
    if fixed != bytes[..crate::gbafix::HEADER_SIZE] {
        return Err("the cartridge header differs from what gbafix writes".into());
    }
    if bytes[3] != 0xea {
        return Err(format!(
            "the first word {:02x}{:02x}{:02x}{:02x} is not an ARM branch",
            bytes[3], bytes[2], bytes[1], bytes[0]
        ));
    }
    let offset = (u32::from_le_bytes([bytes[0], bytes[1], bytes[2], 0]) << 8) as i32 >> 6;
    let entry = 0x0800_0008u32.wrapping_add(offset as u32);
    Ok(format!(
        "header valid (logo, complement), entry branch to {entry:#010x}"
    ))
}

struct Report {
    log: String,
    diagnostics: Vec<Diagnostic>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord)]
pub(crate) struct Diagnostic {
    pub file: String,
    pub line: u32,
    pub message: String,
    pub option: String,
}

/// Every warning file of the editions' builds, one block per source and
/// distinct text, naming the editions that produced it.
fn collect_warnings(root: &Path, targets: &[DecompTarget]) -> Result<Report, String> {
    let mut texts: BTreeMap<String, BTreeMap<String, Vec<String>>> = BTreeMap::new();
    for target in targets {
        let output = root.join(OUTPUT).join(target.id.as_str());
        let mut files = Vec::new();
        walk(&output.join("obj"), &mut files);
        let link = output.join(format!("{}.link-warnings", target.id));
        if link.is_file() {
            files.push(link);
        }
        for path in files {
            // Paths are the checkout's own, as the source names its includes.
            let text = fs::read_to_string(&path)
                .unwrap_or_default()
                .replace(&format!("{}/", root.display()), "");
            let text = without_empty_orphans(&text);
            if text.trim().is_empty() {
                continue;
            }
            let name = path
                .strip_prefix(output.join("obj"))
                .or_else(|_| path.strip_prefix(&output))
                .unwrap_or(&path)
                .with_extension("")
                .to_string_lossy()
                .into_owned();
            texts
                .entry(name)
                .or_default()
                .entry(text)
                .or_default()
                .push(target.id.to_string());
        }
    }
    let mut log = String::new();
    let mut seen = BTreeSet::new();
    for (name, variants) in &texts {
        for (text, editions) in variants {
            log.push_str(&format!("== {name} ({})\n", editions.join(", ")));
            log.push_str(text);
            if !text.ends_with('\n') {
                log.push('\n');
            }
            seen.extend(parse(text));
        }
    }
    Ok(Report {
        log,
        diagnostics: seen.into_iter().collect(),
    })
}

/// The linker names every empty .bss and dynamic-linking section of the
/// assembly as an orphan; only the others are warnings worth reading, and
/// unplaced_data refuses a .bss that holds anything.
fn without_empty_orphans(text: &str) -> String {
    let pattern = regex::Regex::new(
        r"orphan section `(\.bss|\.iplt|\.rel\.iplt|\.igot\.plt|\.debug_\w+)'",
    )
    .expect("static pattern");
    text.lines()
        .filter(|line| !pattern.is_match(line))
        .fold(String::new(), |mut kept, line| {
            kept.push_str(line);
            kept.push('\n');
            kept
        })
}

/// The game clears no .bss: a modern object's zero-initialised data would
/// start as whatever RAM held.
fn unplaced_data(elf: &Path) -> Result<(), String> {
    use object::{Object, ObjectSection};
    let bytes = fs::read(elf).map_err(|error| format!("{}: {error}", elf.display()))?;
    let file = object::File::parse(bytes.as_slice()).map_err(|error| error.to_string())?;
    match file
        .sections()
        .find(|section| section.name() == Ok(".bss") && section.size() > 0)
    {
        Some(section) => Err(format!(
            "{} carries {:#x} bytes of .bss, which the game never clears",
            elf.display(),
            section.size()
        )),
        None => Ok(()),
    }
}

fn walk(directory: &Path, files: &mut Vec<PathBuf>) {
    let Ok(entries) = fs::read_dir(directory) else {
        return;
    };
    for entry in entries.flatten() {
        let path = entry.path();
        if path.is_dir() {
            walk(&path, files);
        } else if path.extension().is_some_and(|extension| extension == "warnings") {
            files.push(path);
        }
    }
}

/// The warnings in GCC's plain diagnostic output, without their notes.
pub(crate) fn parse(text: &str) -> Vec<Diagnostic> {
    let pattern = regex::Regex::new(r"^(.+?):(\d+):(?:\d+:)? warning: (.*?)(?: \[(-W[^\]]+)\])?$")
        .expect("static pattern");
    text.lines()
        .filter_map(|line| {
            let capture = pattern.captures(line)?;
            Some(Diagnostic {
                file: capture[1].to_owned(),
                line: capture[2].parse().ok()?,
                message: capture[3].to_owned(),
                option: match capture.get(4) {
                    Some(option) => option.as_str().trim_end_matches('=').to_owned(),
                    None => unconditional(&capture[3]).to_owned(),
                },
            })
        })
        .collect()
}

/// A name for a warning GCC gives without an option, from its message.
fn unconditional(message: &str) -> &'static str {
    const KNOWN: &[(&str, &str)] = &[
        ("called through a non-compatible type", "(call through a non-compatible type)"),
        ("conflicting types for", "(conflicting types)"),
        ("declared inside parameter list", "(declared inside parameter list)"),
        ("variably modified", "(variably modified at file scope)"),
        ("redefined", "(macro redefined)"),
    ];
    KNOWN
        .iter()
        .find(|(text, _)| message.contains(text))
        .map_or("(no option)", |(_, name)| name)
}

/// The kind of problem a warning option reports, for the summary.
pub(crate) fn kind(option: &str) -> &'static str {
    match option {
        "-Wimplicit-function-declaration" | "-Wimplicit-int" | "-Wimplicit" => {
            "implicit declarations"
        }
        "-Wbuiltin-declaration-mismatch"
        | "-Wold-style-declaration"
        | "-Wmissing-parameter-type"
        | "-Wdeprecated-non-prototype"
        | "-Wlto-type-mismatch"
        | "(conflicting types)"
        | "(declared inside parameter list)" => "prototype mismatches",
        "(call through a non-compatible type)" | "-Wcast-function-type" => {
            "calls through a cast function type"
        }
        "-Wint-conversion" | "-Wpointer-to-int-cast" | "-Wint-to-pointer-cast" => {
            "int/pointer conversions"
        }
        "-Wincompatible-pointer-types" | "-Wpointer-sign" | "-Wdiscarded-qualifiers" => {
            "pointer type mismatches"
        }
        "-Wuninitialized" | "-Wmaybe-uninitialized" => "uninitialised variables",
        "-Warray-bounds"
        | "-Wstringop-overflow"
        | "-Wstringop-overread"
        | "-Wzero-length-bounds"
        | "-Waggressive-loop-optimizations" => "array bounds",
        "-Wstrict-aliasing" => "strict aliasing",
        "-Wreturn-type" | "-Wreturn-mismatch" | "-Wreturn-local-addr" => {
            "missing or mismatched returns"
        }
        "-Wsequence-point"
        | "-Wshift-count-overflow"
        | "-Wshift-negative-value"
        | "-Woverflow"
        | "-Wdiv-by-zero" => "undefined arithmetic",
        "-Wunused-variable"
        | "-Wunused-but-set-variable"
        | "-Wunused-function"
        | "-Wunused-value"
        | "-Wunused-label"
        | "-Wunused-parameter"
        | "-Wunused-but-set-parameter"
        | "-Wunused-const-variable" => "unused code",
        "-Wsign-compare" | "-Wtype-limits" => "signedness and range",
        "-Wmissing-field-initializers" | "-Wmissing-braces" => "initialisers",
        "-Wparentheses"
        | "-Wmisleading-indentation"
        | "-Wempty-body"
        | "-Wimplicit-fallthrough"
        | "-Wdangling-else" => "control-flow style",
        _ => "other",
    }
}

pub(crate) fn summarise(diagnostics: &[Diagnostic]) -> String {
    let mut text = format!(
        "{} distinct warnings across the modern builds\n\nBy kind\n",
        diagnostics.len()
    );
    let mut kinds: BTreeMap<&str, BTreeMap<&str, usize>> = BTreeMap::new();
    let mut files: BTreeMap<&str, usize> = BTreeMap::new();
    for diagnostic in diagnostics {
        *kinds
            .entry(kind(&diagnostic.option))
            .or_default()
            .entry(&diagnostic.option)
            .or_default() += 1;
        *files.entry(&diagnostic.file).or_default() += 1;
    }
    let mut ranked: Vec<_> = kinds.iter().collect();
    ranked.sort_by_key(|(name, options)| (std::cmp::Reverse(options.values().sum::<usize>()), **name));
    for (name, options) in ranked {
        text.push_str(&format!("{:>7}  {name}\n", options.values().sum::<usize>()));
        let mut options: Vec<_> = options.iter().collect();
        options.sort_by_key(|(option, count)| (std::cmp::Reverse(**count), **option));
        for (option, count) in options {
            text.push_str(&format!("{count:>15}  {option}\n"));
        }
    }
    text.push_str("\nFiles with the most warnings\n");
    let mut files: Vec<_> = files.into_iter().collect();
    files.sort_by_key(|(file, count)| (std::cmp::Reverse(*count), *file));
    for (file, count) in files.iter().take(30) {
        text.push_str(&format!("{count:>7}  {file}\n"));
    }
    text
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn camelots_c_goes_modern_with_the_game_convention_and_library_c_does_not() {
        let game = family_flags(CompilerFamily::Game);
        assert_eq!(game[0], "-mthumb");
        for flag in ["-fcall-used-r4", "-mabi=apcs-gnu", "-fno-toplevel-reorder", "-std=gnu89"] {
            assert!(game.contains(&flag.to_owned()), "{flag}");
        }
        assert_eq!(family_flags(CompilerFamily::GameArm)[0], "-marm");
        let target = decomp_target(Some("tbs-en")).unwrap();
        assert!(compiles(target, Path::new("games/THE BROKEN SEAL/SRC/BATTLE/TGTEXEC.C")));
        assert!(compiles(target, Path::new("games/THE BROKEN SEAL/SRC/GRAPHICS/TEXT/MOJI.C")));
        assert!(!compiles(target, Path::new("games/COMMON/SRC/SOUND/ENGINE.C")));
        assert!(!compiles(target, Path::new("games/COMMON/SRC/SYSTEM/SAVE/READ_FLASH.C")));
    }

    #[test]
    fn the_main_script_widens_sections_and_ends_rom_with_a_catch_all() {
        let text = "SECTIONS\n{\n    .rom_main :\n    {\n    \"*/A.o\"(.text .rodata .data .padding)\n    \"*/B.o\"(.text)\n} > ROM\n    .iwram_mixer 0x3007000 :\n    {\n    } > IWRAM AT>ROM\n    .rom_tail :\n    {\n    \"*/C.o\"(.rodata)\n    } > ROM\n    X = LOADADDR(.iwram_mixer);\n    /DISCARD/ : { \"*/D.o\"(.text.spawn) }\n}\n";
        let out = main_script(text);
        assert!(out.contains("\"*/A.o\"(.text .text.unlikely .text.hot .text.startup .rodata .rodata.str* .rodata.cst* .data .padding)"));
        assert!(out.contains("\"*/B.o\"(.text .text.unlikely .text.hot .text.startup)"));
        assert!(out.contains("\"*/C.o\"(.rodata .rodata.str* .rodata.cst*)"));
        assert!(out.contains("\"*/D.o\"(.text.spawn)"), "named sections stay as they are");
        let catch = out.find(".rom_modern").unwrap();
        assert!(catch > out.find(".rom_tail").unwrap());
        assert!(catch < out.find("X = LOADADDR").unwrap());
    }

    #[test]
    fn an_overlay_object_keeps_all_its_sections_in_its_place() {
        let text = "SECTIONS\n{\n    .text 0x2008000 :\n    {\n        \"*/A.o\"(.text)\n        \"*/r_overlay.o\"(.rodata.x0200a3f0)\n    }\n}";
        let out = overlay_text(text);
        assert!(out.contains(&format!("\"*/A.o\"({CATCH_ALL})")));
        assert!(out.contains("\"*/r_overlay.o\"(.rodata.x0200a3f0)"));
    }

    #[test]
    fn warnings_parse_by_option_and_group_by_kind() {
        let text = "games/A.C: In function 'f':\n\
games/A.C:3:5: warning: implicit declaration of function 'g' [-Wimplicit-function-declaration]\n\
games/A.C:4:9: warning: 'x' may be used uninitialized [-Wmaybe-uninitialized]\n\
games/A.C:2:6: note: 'x' was declared here\n\
games/B.C:7:1: warning: array subscript 4 is above array bounds [-Warray-bounds=]\n";
        let found = parse(text);
        assert_eq!(found.len(), 3);
        assert_eq!(found[0].file, "games/A.C");
        assert_eq!(found[0].line, 3);
        assert_eq!(found[2].option, "-Warray-bounds");
        assert_eq!(kind(&found[0].option), "implicit declarations");
        assert_eq!(kind(&found[1].option), "uninitialised variables");
        let summary = summarise(&found);
        assert!(summary.starts_with("3 distinct warnings"));
        assert!(summary.contains("      2  games/A.C"));
    }

    #[test]
    fn the_header_check_wants_gbafix_output_and_an_entry_branch() {
        let mut image = vec![0u8; 0x200];
        image[..4].copy_from_slice(&[0x2e, 0x00, 0x00, 0xea]);
        assert!(header_check(&image).is_err(), "no logo yet");
        crate::gbafix::fix(&mut image, &crate::gbafix::Fields::default(), false);
        let line = header_check(&image).unwrap();
        assert!(line.ends_with("0x080000c0"), "{line}");
        image[3] = 0;
        crate::gbafix::fix(&mut image, &crate::gbafix::Fields::default(), false);
        assert!(header_check(&image).is_err());
    }
}
