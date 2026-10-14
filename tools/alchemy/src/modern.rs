//! The modern build, pret's `make modern`: the same source, assets and
//! scaffold through a current GCC into a ROM that matches nothing. It shows
//! what the matching compilers hide (undefined behaviour, wrong prototypes and
//! types, FAKEMATCH devices that change meaning) and keeps every warning in
//! `out/modern/warnings.txt`. Its objects, scripts and images live under
//! `out/modern` only; rom.sha1 never names them.
//!
//! All of the game's C goes through the modern compiler (MODERN_CC, by
//! default Homebrew's arm-none-eabi-gcc) with one option set: pret's
//! `-mthumb -mthumb-interwork -O2 -mabi=apcs-gnu -mtune=arm7tdmi
//! -march=armv4t -fno-toplevel-reorder`, here with `-mcpu=arm7tdmi`, which
//! sets the same pair, and with `-marm` for the files the matching build
//! compiles as ARM. That is Camelot's C, Thumb and ARM, and Nintendo's
//! library C (MusicPlayer2000, flash) as well, as in pret: pokeemerald's
//! Makefile gives m4a.c old_agbcc and agb_flash*.c their own
//! `-O -mthumb-interwork` only when MODERN is 0, and its modern build
//! compiles both with the rest at its common options. Where the modern
//! compiler breaks a library, the defined form goes behind `UBFIX`, as in
//! pret's m4a.c. Assembly, MIDI, assets and the scaffold build exactly as
//! in the matching build. The rest of the option set:
//! - `-std=gnu89`: the source is C89 (S3); GCC 15 and later default to C23,
//!   where `bool` is a keyword and `f()` means `f(void)`. Under gnu89 the
//!   classes GCC 14 made errors for C99 and later (implicit declarations,
//!   int/pointer conversions, incompatible pointer types, mismatched
//!   returns) stay warnings, so the build takes no `-fpermissive`.
//! - `-fcall-used-r4` for Camelot's C: the game's calling convention (K1),
//!   which its assembly relies on, not a matching device. The library C
//!   keeps the convention of its matching compiler, r4 saved: the player
//!   tick in the library's assembly keeps a note in r4 across its call of
//!   `AudioCommand_InvokeSlot34`.
//! - `-fno-zero-initialized-in-bss`: Camelot's compiler kept zero-initialised
//!   tables in .data, which the game keeps in ROM; it clears no .bss.
//! - `-fno-tree-loop-distribute-patterns`: the game links no C library, so
//!   loops must not become memset or memcpy calls.
//! - `-ffreestanding -nostdinc`, `-DMODERN=1` for the source's guards, and
//!   `-Wall -Wextra -fdiagnostics-plain-output` for the report, less four
//!   classes that question style and no behaviour: sign-compare,
//!   unused-parameter, missing-field-initializers and pointer-sign.
//!
//! Of the classes GCC 14 made errors for C99 and later, which pret's modern
//! build, at GCC's default standard, gets as errors, a class whose count
//! reached 0 in all six editions becomes an error and stays at 0 (STRICT
//! below). Every other class, these while above 0 and the rest of -Wall
//! and -Wextra, is reported and held only by the count below.
//!
//! The count of distinct warnings in Camelot's C may only fall.
//! tools/alchemy/modern.tsv holds the count allowed for the compiler it was
//! measured with; a build of all six editions with that compiler fails
//! above it, and `--update` lowers it. Another compiler's count is reported
//! and not compared. The library C's own warnings are reported apart and
//! not counted: S3 keeps that source out of style passes, and what the
//! modern compiler breaks there takes UBFIX.
//!
//! Linking reuses the edition's linker script with GCC's extra section names
//! beside each object's own (`.rodata.str1.4` and the like), a catch-all ROM
//! section for interworking glue and runtime code, and the modern compiler's
//! own libgcc. The fixed-address and id gates judge the matching build only:
//! under MODERN, `FIXED_ADDRESS` calls the linked routine by name, and the
//! compiler folds ids into neighbouring constants.

use crate::build_rom::{link_with, Build, Commands, Linked};
use crate::compiler::routing::{cflags_for_family, family_for_source, CompilerFamily};
use crate::targets::{decomp_target, DecompTarget};
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::{Path, PathBuf};

const USAGE: &str = "usage: alchemy build modern [--target GAME-EDITION]... [--cc COMPILER] [--keep-going] [--update] [--commands]
Build each edition (all six of ☀️ by default) from the same source with a
modern GCC, as pret's `make modern`: out/modern/<edition>.gba, a ROM that
matches nothing, and every compiler and linker warning in
out/modern/warnings.txt with a summary by kind and file in
out/modern/warnings-summary.txt. The compiler is --cc, else $MODERN_CC,
else the arm-none-eabi-gcc on PATH. --keep-going writes an
image despite link errors. A build of all six editions fails when
Camelot's C has more distinct warnings than tools/alchemy/modern.tsv allows
for its compiler; --update lowers that count to the build's. --commands prints
each edition's commands as `alchemy build rom --commands` does.";

/// The count of distinct warnings a build of the six editions may have.
const ALLOWED: &str = "tools/alchemy/modern.tsv";
const ALLOWED_HEADER: &str = "compiler\twarnings";

const DEFAULT_CC: &str = "arm-none-eabi-gcc";
const OUTPUT: &str = "out/modern";

pub(crate) struct Modern {
    pub cc: String,
    /// The compiler's version and option set: part of every cache key.
    pub signature: String,
    /// The compiler's own runtime library, linked after the objects.
    pub libraries: Vec<PathBuf>,
    /// The compiler's major release, which decides the classes it knows.
    pub major: u32,
    /// `-Werror=` for each class of STRICT the compiler knows.
    pub strict: Vec<String>,
}

/// The options every modern compile shares, before the instruction set.
const COMMON_FLAGS: &[&str] = &[
    "-mthumb-interwork",
    "-mcpu=arm7tdmi",
    "-mabi=apcs-gnu",
    "-O2",
    "-fno-toplevel-reorder",
    "-std=gnu89",
    "-ffreestanding",
    "-nostdinc",
    "-fno-zero-initialized-in-bss",
    "-fno-tree-loop-distribute-patterns",
    "-DMODERN=1",
    "-Wall",
    "-Wextra",
    "-Wno-sign-compare",
    "-Wno-unused-parameter",
    "-Wno-missing-field-initializers",
    "-Wno-pointer-sign",
    "-fdiagnostics-plain-output",
];

/// The modern options for a C file of `family`. Camelot's C, Thumb and ARM,
/// takes the game's calling convention; the library C keeps the standard one.
pub(crate) fn family_flags(family: CompilerFamily) -> Vec<String> {
    let mut flags = vec![if family.is_arm() { "-marm" } else { "-mthumb" }.to_owned()];
    flags.extend(COMMON_FLAGS.iter().map(|flag| (*flag).to_owned()));
    if matches!(family, CompilerFamily::Game | CompilerFamily::GameArm) {
        flags.push("-fcall-used-r4".to_owned());
    }
    flags
}

/// The warning classes the modern build refuses, each with the GCC release
/// that introduced its option (0 for one every current GCC has): GCC
/// refuses `-Werror=` of an option it does not know, so an older compiler
/// leaves that class a warning. A class joins once its count is 0 in all
/// six ☀️ editions, and as an error it stays 0 (Pascal, 2026-10-14).
const STRICT: &[(&str, u32)] = &[
    ("implicit-int", 0),
    ("missing-parameter-type", 0),
    ("declaration-missing-parameter-type", 14),
];

/// The other classes GCC 14 made errors for C99 and later, not yet 0, with
/// the release that introduced each option. A build of the six editions
/// names any of them that reaches 0, for STRICT to take. `-Wreturn-type`
/// is none of them: a function whose value no caller reads is Camelot's
/// C89 and stays a warning.
const PENDING: &[(&str, u32)] = &[
    ("implicit-function-declaration", 0),
    ("return-mismatch", 14),
    ("int-conversion", 0),
    ("incompatible-pointer-types", 0),
];

/// `-Werror=` for each strict class GCC `major` knows.
fn strict_flags(major: u32) -> Vec<String> {
    STRICT
        .iter()
        .filter(|(_, since)| major >= *since)
        .map(|(class, _)| format!("-Werror={class}"))
        .collect()
}

/// The pending classes GCC `major` knows that no warning of a build
/// reports: each can join STRICT.
fn ready(diagnostics: &[Diagnostic], major: u32) -> Vec<&'static str> {
    PENDING
        .iter()
        .filter(|(class, since)| {
            major >= *since && !diagnostics.iter().any(|found| found.option == format!("-W{class}"))
        })
        .map(|(class, _)| *class)
        .collect()
}

/// The major release in what `gcc -dumpversion` prints: `16.2.0`, or `16`
/// from a compiler built to print only its major release.
fn major_release(text: &str) -> Option<u32> {
    text.trim().split('.').next()?.parse().ok()
}

impl Modern {
    pub(crate) fn new(cc: &str) -> Result<Modern, String> {
        let root = crate::compiler::routing::root();
        let version = crate::process::run(&[cc, "--version"], root)
            .map_err(|error| format!("modern compiler {cc}: {error}"))?;
        let version = version.lines().next().unwrap_or_default().to_owned();
        let release = crate::process::run(&[cc, "-dumpversion"], root)?;
        let major = major_release(&release)
            .ok_or_else(|| format!("{cc} -dumpversion names no release: {release:?}"))?;
        let strict = strict_flags(major);
        let libgcc =
            crate::process::run(&[cc, "-mthumb", "-mcpu=arm7tdmi", "-print-libgcc-file-name"], root)?;
        let libgcc = PathBuf::from(libgcc.trim());
        if !libgcc.is_absolute() || !libgcc.is_file() {
            return Err(format!("{cc} names no libgcc.a: {}", libgcc.display()));
        }
        let signature = format!(
            "{version}\0{} {}\0{}",
            COMMON_FLAGS.join(" "),
            strict.join(" "),
            libgcc.display()
        );
        Ok(Modern {
            cc: cc.to_owned(),
            signature,
            libraries: vec![libgcc],
            major,
            strict,
        })
    }
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
        step.extend(self.strict.iter().cloned());
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
    // A far-call bank pinned where Camelot's bytes put it stays there while the
    // modern code before it fits, and follows that code once it does not.
    // The address may be an expression over several lines, one arm for each
    // edition; it ends at the statement's `);`.
    let pin = regex::Regex::new(r"(?s)\. = ABSOLUTE\((.*?)\);").expect("static pattern");
    let text = pin.replace_all(text, |capture: &regex::Captures| {
        format!(". = MAX(ABSOLUTE(.), {});", &capture[1])
    });
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

/// An overlay's script for modern objects. An overlay has one image, so a
/// statement that takes only an object's code, `(.text)`, takes its
/// constants and data with it: they land in its listed place. Any other
/// statement keeps what it takes, widened to GCC's names as in the main
/// script: a plain place lists every object once for its code
/// (`.text .padding`), once for its constants (`.rodata`) and once for its
/// data, and each pass takes GCC's sections of its own kind.
pub(crate) fn overlay_text(text: &str) -> String {
    let object = regex::Regex::new(r#"("\*/[^"]+\.o")\(([^)]*)\)"#).expect("static pattern");
    let mut out = object
        .replace_all(text, |capture: &regex::Captures| {
            match capture[2].trim() {
                ".text" => format!("{}({CATCH_ALL})", &capture[1]),
                sections => format!("{}({})", &capture[1], widened(sections)),
            }
        })
        .into_owned();
    if !out.ends_with('\n') {
        out.push('\n');
    }
    out
}

/// Write the modern script of an overlay from its script's text as the
/// linker reads it, an object list's fragments in place.
pub(crate) fn overlay_script(text: &str, directory: &Path, id: &str) -> Result<PathBuf, String> {
    let path = directory.join(format!("resource_{id}.modern.ld"));
    write_if_changed(&path, &overlay_text(text))?;
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
    let mut update = false;
    let mut commands = None;
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
            "--update" => update = true,
            "--commands" => commands = Some(Commands::printed(crate::compiler::routing::root())),
            _ => return Err(USAGE.into()),
        }
    }
    let all = targets.is_empty();
    if all {
        for edition in ["ja", "en", "de", "es", "fr", "it"] {
            targets.push(decomp_target(Some(&format!("tbs-{edition}")))?);
        }
    }
    let modern = Modern::new(&cc)?;
    println!("modern compiler: {}", modern.signature.split('\0').next().unwrap_or(&cc));
    let root = crate::compiler::routing::root();
    let mut results = Vec::new();
    for target in &targets {
        let edition = Build {
            target: *target,
            modern: Some(&modern),
            commands: commands.as_ref(),
        };
        let result = build(root, edition, keep_going);
        match &result {
            Ok(line) => println!("{}: {line}", target.id),
            Err(error) => println!("{}: FAILED\n{error}", target.id),
        }
        results.push((*target, result));
    }
    let report = collect_warnings(root, &targets)?;
    fs::write(root.join(OUTPUT).join("warnings.txt"), &report.log)
        .map_err(|error| error.to_string())?;
    let summary = summarise(&report.diagnostics) + &library_summary(&report.library);
    fs::write(root.join(OUTPUT).join("warnings-summary.txt"), &summary)
        .map_err(|error| error.to_string())?;
    print!("{summary}");
    let failed: Vec<String> = results
        .iter()
        .filter(|(_, result)| result.is_err())
        .map(|(target, _)| target.id.to_string())
        .collect();
    if !failed.is_empty() {
        return Err(format!("the modern build failed for {}", failed.join(", ")));
    }
    if !all {
        return Ok(());
    }
    let compiler = modern.signature.split('\0').next().unwrap_or(&cc).to_owned();
    let path = root.join(ALLOWED);
    let before = fs::read_to_string(&path).unwrap_or_default();
    let (text, verdict) = hold(&before, &compiler, report.diagnostics.len(), update)?;
    if text != before {
        fs::write(&path, text).map_err(|error| format!("{ALLOWED}: {error}"))?;
    }
    println!("{verdict}");
    // An error stops the library's compiles too, so a class waits for both.
    for class in ready(&[report.diagnostics.as_slice(), report.library.as_slice()].concat(), modern.major) {
        println!("-W{class} is at 0 in all six editions: move it from PENDING to STRICT in tools/alchemy/src/modern.rs");
    }
    Ok(())
}

/// Hold a build's count of distinct warnings to the count allowed for its
/// compiler: the table's text afterwards and what to tell the builder, or
/// the refusal of a count that rose.
fn hold(table: &str, compiler: &str, found: usize, update: bool) -> Result<(String, String), String> {
    let mut rows: Vec<(String, usize)> = Vec::new();
    for line in table.lines().skip(1) {
        let (name, count) = line.split_once('\t').ok_or_else(|| format!("{ALLOWED}: {line:?} is no row"))?;
        let count = count.parse().map_err(|_| format!("{ALLOWED}: {count:?} is no count"))?;
        rows.push((name.to_owned(), count));
    }
    let allowed = rows.iter().find(|(name, _)| name == compiler).map(|(_, count)| *count);
    let verdict = match allowed {
        Some(allowed) if found > allowed => {
            return Err(format!(
                "{found} distinct warnings, and {ALLOWED} allows {allowed} for {compiler}: the count may only fall"
            ));
        }
        Some(allowed) if found < allowed && !update => {
            format!("{found} distinct warnings, {allowed} allowed: lower the count with --update")
        }
        Some(allowed) if !update || found == allowed => format!("{found} distinct warnings, {allowed} allowed"),
        None if !update => format!("{found} distinct warnings; {ALLOWED} holds no count for {compiler}, so none is compared"),
        _ => {
            rows.retain(|(name, _)| name != compiler);
            rows.push((compiler.to_owned(), found));
            rows.sort();
            format!("{found} distinct warnings, now the count allowed for {compiler}")
        }
    };
    let mut text = format!("{ALLOWED_HEADER}\n");
    for (name, count) in &rows {
        text += &format!("{name}\t{count}\n");
    }
    if rows.is_empty() && table.is_empty() {
        text.clear();
    }
    Ok((text, verdict))
}

fn build(root: &Path, edition: Build, keep_going: bool) -> Result<String, String> {
    let target = edition.target;
    let name = target.id.as_str();
    let output = Path::new(OUTPUT).join(name);
    fs::create_dir_all(root.join(&output)).map_err(|error| error.to_string())?;
    // The script as the linker reads it, an object list's fragments in
    // place: the modern script is one file, widened statement by statement.
    let text = crate::edition::image_script(root, target, crate::edition::Image::Main)?;
    let modern_script = output.join(format!("{name}.modern.ld"));
    write_if_changed(&root.join(&modern_script), &main_script(&text))?;
    let Linked { image, .. } = link_with(root, edition, &modern_script, &output, name, keep_going)?;
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
    /// The distinct warnings of Camelot's C: the count holds these.
    diagnostics: Vec<Diagnostic>,
    /// The distinct warnings only Nintendo's library C gives, reported apart.
    library: Vec<Diagnostic>,
}

/// Whether the object `name` (its source's path without the extension) is
/// Nintendo's library C, MusicPlayer2000 or flash.
fn library_object(target: DecompTarget, name: &str) -> bool {
    matches!(
        family_for_source(target.compiler, &format!("{name}.c")),
        CompilerFamily::Agbcc | CompilerFamily::AgbccFlash
    )
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
    let mut libraries = BTreeSet::new();
    for target in targets {
        let output = root.join(OUTPUT).join(target.id.as_str());
        // What this edition links now: an object it built once and links no
        // more (a file since merged into another, or renamed) left its
        // warnings behind.
        let mut linked = BTreeSet::new();
        let mut images = vec![crate::edition::Image::Main];
        let ids = crate::edition::overlay_ids(root, *target)?;
        images.extend(ids.iter().map(|id| crate::edition::Image::Overlay(id)));
        for image in images {
            linked.extend(crate::edition::built_objects(&crate::edition::linked_statements(root, *target, image)?));
        }
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
            // An object whose source was renamed or removed is built no more:
            // its warnings are a stale file's, not the tree's.
            if stale(root, &name) || (name.starts_with("games/") && !linked.contains(&name)) {
                let _ = fs::remove_file(&path);
                continue;
            }
            if library_object(*target, &name) {
                libraries.insert(name.clone());
            }
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
    let mut library = BTreeSet::new();
    for (name, variants) in &texts {
        for (text, editions) in variants {
            log.push_str(&format!("== {name} ({})\n", editions.join(", ")));
            log.push_str(text);
            if !text.ends_with('\n') {
                log.push('\n');
            }
            if libraries.contains(name) {
                library.extend(parse(text));
            } else {
                seen.extend(parse(text));
            }
        }
    }
    Ok(Report {
        log,
        library: library.difference(&seen).cloned().collect(),
        diagnostics: seen.into_iter().collect(),
    })
}

/// A warning file of an object under games/ that the checkout has no source
/// for any more.
fn stale(root: &Path, object: &str) -> bool {
    object.starts_with("games/") && crate::build_rom::source_for(root, object).is_err()
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

/// Nintendo's library C, apart from the count: each of its warnings by line.
fn library_summary(library: &[Diagnostic]) -> String {
    let mut text = format!(
        "\n{} distinct warnings in Nintendo's library C (MusicPlayer2000, flash), not counted\n",
        library.len()
    );
    for diagnostic in library {
        text.push_str(&format!(
            "{:>7}  {}:{}: {} [{}]\n",
            "", diagnostic.file, diagnostic.line, diagnostic.message, diagnostic.option
        ));
    }
    text
}

pub(crate) fn summarise(diagnostics: &[Diagnostic]) -> String {
    let mut text = format!(
        "{} distinct warnings in Camelot's C across the modern builds\n\nBy kind\n",
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
    fn camelots_c_keeps_the_game_convention_and_library_c_the_standard_one() {
        let target = decomp_target(Some("tbs-en")).unwrap();
        let flags = |source: &str| family_flags(family_for_source(target.compiler, source));
        let r4 = "-fcall-used-r4".to_owned();
        let game = flags("games/tbs/src/battle/battle.c");
        assert_eq!(game[0], "-mthumb");
        for flag in ["-fcall-used-r4", "-mabi=apcs-gnu", "-fno-toplevel-reorder", "-std=gnu89"] {
            assert!(game.contains(&flag.to_owned()), "{flag}");
        }
        let arm = flags("games/tbs/src/window/moji.c");
        assert_eq!(arm[0], "-marm");
        assert!(arm.contains(&r4));
        // MusicPlayer2000 and the flash library: Thumb, C89, r4 saved.
        for source in ["games/tbs/src/sound/engine.c", "games/common/src/sound/sound.c", "games/common/src/system/save/flash.c"] {
            let library = flags(source);
            assert_eq!(library[0], "-mthumb", "{source}");
            assert!(library.contains(&"-std=gnu89".to_owned()), "{source}");
            assert!(!library.contains(&r4), "{source} saves r4");
        }
    }

    #[test]
    fn the_main_script_widens_sections_and_ends_rom_with_a_catch_all() {
        let text = "SECTIONS\n{\n    .rom_main :\n    {\n    \"*/A.o\"(.text .rodata .data .padding)\n    \"*/B.o\"(.text)\n        . = ABSOLUTE(0x080f0000);\n} > ROM\n    .iwram_mixer 0x3007000 :\n    {\n    } > IWRAM AT>ROM\n    .rom_tail :\n    {\n    \"*/C.o\"(.rodata)\n    } > ROM\n    X = LOADADDR(.iwram_mixer);\n    /discard/ : { \"*/D.o\"(.text.spawn) }\n}\n";
        let out = main_script(text);
        assert!(out.contains("\"*/A.o\"(.text .text.unlikely .text.hot .text.startup .rodata .rodata.str* .rodata.cst* .data .padding)"));
        assert!(out.contains("\"*/B.o\"(.text .text.unlikely .text.hot .text.startup)"));
        assert!(out.contains("\"*/C.o\"(.rodata .rodata.str* .rodata.cst*)"));
        assert!(out.contains("\"*/D.o\"(.text.spawn)"), "named sections stay as they are");
        assert!(out.contains("        . = MAX(ABSOLUTE(.), 0x080f0000);\n"), "a bank pin yields");
        let editions = text.replace("0x080f0000", "DEFINED(G_EDITION_JA) ? 0x080f0000 :\n                     0x080f9000");
        assert!(
            main_script(&editions).contains("        . = MAX(ABSOLUTE(.), DEFINED(G_EDITION_JA) ? 0x080f0000 :\n                     0x080f9000);\n"),
            "a pin with an address for each edition yields as one statement"
        );
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
    fn a_plain_overlay_place_takes_gccs_sections_pass_by_pass() {
        let text = "SECTIONS\n{\n    .text 0x2008000 :\n    {\n    \"*/a.o\"(.text .padding)\n    \"*libgcc.a:_call_via_rX.o\"(.text .padding)\n    \"*/a.o\"(.rodata)\n    \"*libgcc.a:_call_via_rX.o\"(.rodata)\n    \"*/a.o\"(.data .overlays)\n    }\n    .bss (NOLOAD) :\n    {\n    \"*/a.o\"(.bss COMMON)\n    }\n}\n";
        let out = overlay_text(text);
        let lines: Vec<&str> = out.lines().map(str::trim).filter(|line| line.starts_with('"')).collect();
        assert_eq!(
            lines,
            [
                "\"*/a.o\"(.text .text.unlikely .text.hot .text.startup .padding)",
                "\"*libgcc.a:_call_via_rX.o\"(.text .padding)",
                "\"*/a.o\"(.rodata .rodata.str* .rodata.cst*)",
                "\"*libgcc.a:_call_via_rX.o\"(.rodata)",
                "\"*/a.o\"(.data .overlays)",
                "\"*/a.o\"(.bss COMMON)",
            ],
            "each pass takes GCC's names of its own kind, and a library member stays as listed"
        );
        // A row that names all four of an object's sections is widened too.
        assert!(overlay_text("\"*/b.o\"(.text .rodata .data .padding)\n")
            .contains("\"*/b.o\"(.text .text.unlikely .text.hot .text.startup .rodata .rodata.str* .rodata.cst* .data .padding)"));
    }

    /// The object list as it stands: no C object of an overlay leaves its
    /// modern code, constants or data to the linker's orphan placement.
    #[test]
    fn every_overlay_of_the_list_places_gccs_sections_of_its_c_objects() {
        let root = crate::compiler::routing::root();
        let mut checked = 0;
        for edition in ["ja", "en", "de", "es", "fr", "it"] {
            let target = decomp_target(Some(&format!("tbs-{edition}"))).unwrap();
            for id in crate::edition::overlay_ids(root, target).unwrap() {
                let image = crate::edition::Image::Overlay(&id);
                let text = crate::edition::image_script(root, target, image).unwrap();
                let statements = crate::edition::statements(&overlay_text(&text)).unwrap();
                for object in crate::edition::built_objects(&statements) {
                    if !root.join(&object).with_extension("c").is_file() {
                        continue;
                    }
                    let taken: BTreeSet<&str> = statements
                        .iter()
                        .filter(|statement| statement.object == object && statement.section == ".text")
                        .flat_map(|statement| statement.sections.split_whitespace())
                        .collect();
                    for section in [".text", ".text.unlikely", ".text.hot", ".rodata", ".rodata.str*", ".rodata.cst*", ".data"] {
                        assert!(taken.contains(section), "{} overlay {id}: {object} has no place for {section}", target.id);
                    }
                    checked += 1;
                }
            }
        }
        assert!(checked > 6 * 90, "{checked} C objects in ☀️'s overlays");
    }

    #[test]
    fn warnings_parse_by_option_and_group_by_kind() {
        let text = "games/a.c: In function 'f':\n\
games/a.c:3:5: warning: implicit declaration of function 'g' [-Wimplicit-function-declaration]\n\
games/a.c:4:9: warning: 'x' may be used uninitialized [-Wmaybe-uninitialized]\n\
games/a.c:2:6: note: 'x' was declared here\n\
games/b.c:7:1: warning: array subscript 4 is above array bounds [-Warray-bounds=]\n";
        let found = parse(text);
        assert_eq!(found.len(), 3);
        assert_eq!(found[0].file, "games/a.c");
        assert_eq!(found[0].line, 3);
        assert_eq!(found[2].option, "-Warray-bounds");
        assert_eq!(kind(&found[0].option), "implicit declarations");
        assert_eq!(kind(&found[1].option), "uninitialised variables");
        let summary = summarise(&found);
        assert!(summary.starts_with("3 distinct warnings"));
        assert!(summary.contains("      2  games/a.c"));
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

    #[test]
    fn the_count_of_warnings_may_only_fall() {
        let table = "compiler\twarnings\ngcc 16\t100\n";
        assert_eq!(hold(table, "gcc 16", 100, false).unwrap(), (table.to_owned(), "100 distinct warnings, 100 allowed".to_owned()));
        assert!(hold(table, "gcc 16", 101, false).unwrap_err().contains("allows 100 for gcc 16: the count may only fall"));
        assert!(hold(table, "gcc 16", 101, true).unwrap_err().contains("may only fall"));
        let (kept, verdict) = hold(table, "gcc 16", 90, false).unwrap();
        assert_eq!((kept.as_str(), verdict.as_str()), (table, "90 distinct warnings, 100 allowed: lower the count with --update"));
        let (lowered, _) = hold(table, "gcc 16", 90, true).unwrap();
        assert_eq!(lowered, "compiler\twarnings\ngcc 16\t90\n");
        // Another compiler has other warnings: its count is not compared
        // until it has a row of its own.
        let (kept, verdict) = hold(table, "gcc 15", 500, false).unwrap();
        assert_eq!(kept, table);
        assert!(verdict.contains("holds no count for gcc 15"));
        let (both, _) = hold(table, "gcc 15", 500, true).unwrap();
        assert_eq!(both, "compiler\twarnings\ngcc 15\t500\ngcc 16\t100\n");
        assert_eq!(hold("", "gcc 16", 7, false).unwrap().0, "");
        assert_eq!(hold("", "gcc 16", 7, true).unwrap().0, "compiler\twarnings\ngcc 16\t7\n");
    }

    #[test]
    fn a_strict_class_is_an_error_where_the_compiler_has_it_and_a_pending_one_is_named_at_zero() {
        assert_eq!(major_release("16.2.0\n"), Some(16));
        assert_eq!(major_release("13\n"), Some(13));
        assert_eq!(major_release("unknown\n"), None);
        assert_eq!(
            strict_flags(16),
            ["-Werror=implicit-int", "-Werror=missing-parameter-type", "-Werror=declaration-missing-parameter-type"]
        );
        assert_eq!(strict_flags(13), ["-Werror=implicit-int", "-Werror=missing-parameter-type"]);
        let classes: BTreeSet<&str> = STRICT.iter().chain(PENDING).map(|(class, _)| *class).collect();
        assert_eq!(classes.len(), STRICT.len() + PENDING.len(), "a class is strict or pending, once");
        assert!(!classes.contains("return-type"), "a value no caller reads stays a warning");
        let found = parse("games/a.c:3:5: warning: implicit declaration of function 'g' [-Wimplicit-function-declaration]\n");
        assert_eq!(ready(&found, 16), ["return-mismatch", "int-conversion", "incompatible-pointer-types"]);
        assert_eq!(ready(&found, 13), ["int-conversion", "incompatible-pointer-types"]);
    }

    #[test]
    fn the_library_objects_are_reported_apart() {
        let target = decomp_target(Some("tbs-en")).unwrap();
        for library in ["games/tbs/src/sound/engine", "games/common/src/sound/sound", "games/common/src/system/save/sst"] {
            assert!(library_object(target, library), "{library}");
        }
        assert!(!library_object(target, "games/tbs/src/sound/sound"), "Camelot's own sound C");
        assert!(!library_object(target, "games/tbs/src/battle/battle"));
        let found = parse("games/common/src/system/save/atmel.c:164:1: warning: label 'retry_failed' defined but not used [-Wunused-label]\n");
        let text = library_summary(&found);
        assert!(text.starts_with("\n1 distinct warnings in Nintendo's library C"), "{text}");
        assert!(text.contains("  games/common/src/system/save/atmel.c:164: label 'retry_failed' defined but not used [-Wunused-label]\n"));
    }

    #[test]
    fn warnings_of_a_source_that_is_gone_are_stale() {
        let root = tempfile::tempdir().unwrap();
        fs::create_dir_all(root.path().join("games/g/src")).unwrap();
        fs::write(root.path().join("games/g/src/kept.c"), "").unwrap();
        assert!(!stale(root.path(), "games/g/src/kept"));
        assert!(stale(root.path(), "games/g/src/renamed"));
        // What the build makes itself has no source and is never stale.
        assert!(!stale(root.path(), "resource_3ad_overlay"));
    }
}
