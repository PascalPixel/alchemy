//! Private disassembly from an approved, explicitly selected ROM.
//!
//! Discovery is an inspection aid, not build ownership or progress evidence.
//! Everything this command writes is disposable output under out/<target>.

use crate::compiler::routing;
use crate::overlay::rom::{CanonicalRom, OVERLAY_BASE};
use crate::targets::{self, DecompTarget};
use psynergy::assembly::{thumb_source, thumb_source_from_instructions};
use psynergy::decode::{decode_one, Kind};
use psynergy::discovery::{Discovery, FunctionInfo, Mode, ROM_BASE};
use std::collections::BTreeMap;
use std::path::{Path, PathBuf};

const USAGE: &str = "usage: alchemy raw status --target GAME-EDITION [--rom FILE]\n\
       alchemy raw rebuild --target GAME-EDITION [--rom FILE]\n\
Reads the target's approved ROM and discovers main-image functions and code\n\
overlays. rebuild writes private inspection listings to out/<target>/raw.\n\
Listings do not declare source ownership, count toward DONE, or enter a build.";

struct Options<'a> {
    command: &'a str,
    target: &'a str,
    rom: Option<&'a str>,
}

fn parse(arguments: &[String]) -> Result<Options<'_>, String> {
    let command = arguments.first().map(String::as_str).ok_or(USAGE)?;
    if !matches!(command, "status" | "rebuild") {
        return Err(USAGE.into());
    }
    let mut target = None;
    let mut rom = None;
    let mut index = 1;
    while index < arguments.len() {
        let flag = arguments[index].as_str();
        let destination = match flag {
            "--target" if target.is_none() => &mut target,
            "--rom" if rom.is_none() => &mut rom,
            _ => return Err(format!("unknown or duplicate flag {flag}\n{USAGE}")),
        };
        let value = arguments
            .get(index + 1)
            .filter(|value| !value.is_empty() && !value.starts_with("--"))
            .ok_or_else(|| format!("{flag} needs a value"))?;
        *destination = Some(value.as_str());
        index += 2;
    }
    Ok(Options {
        command,
        target: target.ok_or("--target is required")?,
        rom,
    })
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    if arguments == ["--help"] || arguments == ["-h"] {
        println!("{USAGE}");
        return Ok(());
    }
    let options = parse(arguments)?;
    let target = targets::decomp_target(Some(options.target))?;
    let root = routing::root();
    let rom = load_rom(root, target, options.rom)?;
    let mut discovery = Discovery::new(rom.bytes(), ROM_BASE);
    discovery.run();
    let overlays = rom.overlay_resources(target.overlay_entry_veneers);
    if options.command == "status" {
        println!(
            "target={} discovered_functions={} instructions={} code_overlays={} directory={}",
            target.id,
            discovery.function_count(),
            discovery.instructions.len(),
            overlays.len(),
            raw_directory(root, target).display(),
        );
        return Ok(());
    }
    let output = root.join(target.output_dir);
    let output = crate::compiler::build_io::generated_directory(root, &output)?;
    let stage = tempfile::Builder::new()
        .prefix(".raw-")
        .tempdir_in(&output)
        .map_err(|error| error.to_string())?;
    let main = stage.path().join("main");
    std::fs::create_dir(&main).map_err(|error| error.to_string())?;
    let arm = arm_rows(stage.path(), &discovery)?;
    let mut main_files = 0;
    for entry in discovery.function_entries() {
        let info = discovery.function(entry).expect("discovered function");
        let Some(end) = function_end(&discovery, info) else {
            continue;
        };
        let (extension, source) = match info.mode {
            Mode::Thumb => (
                "s",
                thumb_source_from_instructions(
                    rom.bytes(),
                    ROM_BASE as u32,
                    entry as u32,
                    (end - entry) as u32,
                    &info
                        .instructions
                        .iter()
                        .map(|address| *address as u32)
                        .collect(),
                )?,
            ),
            Mode::Arm => (
                "lst",
                arm.range(entry..end)
                    .map(|(address, text)| format!("{address:08x}: {text}\n"))
                    .collect(),
            ),
        };
        write_listing(&main.join(format!("Func_{entry:08x}.{extension}")), &source)?;
        main_files += 1;
    }
    let overlay_dir = stage.path().join("overlays");
    std::fs::create_dir(&overlay_dir).map_err(|error| error.to_string())?;
    for resource in &overlays {
        let name = format!("resource_{resource:03x}");
        let image = rom.overlay(&name)?;
        let source = thumb_source(
            &image,
            OVERLAY_BASE as u32,
            OVERLAY_BASE as u32,
            image.len() as u32,
        )?;
        write_listing(&overlay_dir.join(format!("{name}_overlay.s")), &source)?;
    }
    let destination = raw_directory(root, target);
    if destination.exists() {
        std::fs::remove_dir_all(&destination).map_err(|error| error.to_string())?;
    }
    std::fs::rename(stage.path(), &destination).map_err(|error| error.to_string())?;
    println!(
        "target={} main_listings={main_files} overlays={} directory={}",
        target.id,
        overlays.len(),
        destination.display(),
    );
    Ok(())
}

fn load_rom(
    root: &Path,
    target: DecompTarget,
    explicit: Option<&str>,
) -> Result<CanonicalRom, String> {
    let rom = CanonicalRom::from_file(&root.join(explicit.unwrap_or(target.rom)), target)?;
    target.verify_reference(rom.bytes())?;
    Ok(rom)
}

fn raw_directory(root: &Path, target: DecompTarget) -> PathBuf {
    root.join(target.output_dir).join("raw")
}

/// Include observed literal loads, but make no claim about authored boundaries.
fn function_end(discovery: &Discovery, info: &FunctionInfo) -> Option<i64> {
    let mut end = info
        .instructions
        .iter()
        .filter_map(|address| {
            discovery
                .instructions
                .get(address)
                .map(|ins| address + ins.size)
        })
        .max()?;
    if info.mode == Mode::Thumb {
        for address in &info.instructions {
            if let Some(ins) = decode_one(&discovery.data, discovery.base as u32, *address as u32) {
                if let Kind::LdrPool { .. } = ins.kind {
                    let half = discovery.u16(*address);
                    let pool = ((address + 4) & !3) + (half & 0xff) * 4;
                    if discovery.inside(pool, 4) {
                        end = end.max(pool + 4);
                    }
                }
            }
        }
    }
    (end > info.entry && end <= discovery.limit).then_some(end)
}

/// Discovery handles ARM flow; the installed binutils supplies its mnemonics.
fn arm_rows(work: &Path, discovery: &Discovery) -> Result<BTreeMap<i64, String>, String> {
    let arm: Vec<_> = discovery
        .instructions
        .iter()
        .filter(|(_, instruction)| instruction.mode == Mode::Arm)
        .collect();
    let (Some((start, _)), Some((last, instruction))) = (arm.first(), arm.last()) else {
        return Ok(BTreeMap::new());
    };
    let input = work.join("arm.bin");
    std::fs::write(&input, &discovery.data).map_err(|error| error.to_string())?;
    let result = psynergy::process::run(
        &[
            "arm-none-eabi-objdump".into(),
            "-D".into(),
            "-b".into(),
            "binary".into(),
            "-marmv4t".into(),
            format!("--adjust-vma=0x{:x}", discovery.base),
            format!("--start-address=0x{start:x}"),
            format!("--stop-address=0x{:x}", *last + instruction.size),
            input.to_string_lossy().into_owned(),
        ],
        work,
    );
    std::fs::remove_file(&input).map_err(|error| error.to_string())?;
    let mut rows = BTreeMap::new();
    for line in result?.lines() {
        let Some((address, rest)) = line.trim_start().split_once(':') else {
            continue;
        };
        let Ok(address) = i64::from_str_radix(address, 16) else {
            continue;
        };
        if discovery
            .instructions
            .get(&address)
            .is_some_and(|ins| ins.mode == Mode::Arm)
        {
            let text = rest
                .trim_start()
                .split_once(char::is_whitespace)
                .map(|(_, text)| text.trim_start())
                .unwrap_or("");
            rows.insert(address, text.to_string());
        }
    }
    Ok(rows)
}

fn write_listing(path: &Path, source: &str) -> Result<(), String> {
    let text =
        format!("@ Generated private inspection; no source ownership or DONE credit.\n{source}");
    std::fs::write(path, text).map_err(|error| format!("{}: {error}", path.display()))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn requires_a_target_and_rejects_ambiguous_flags() {
        for args in [
            vec!["rebuild"],
            vec!["rebuild", "--target"],
            vec!["rebuild", "--target", "--rom", "input.gba"],
            vec!["rebuild", "--target", "tbs-en", "--target", "tla-en"],
        ] {
            assert!(parse(&args.into_iter().map(String::from).collect::<Vec<_>>()).is_err());
        }
        let args = ["rebuild", "--target", "tbs-ja", "--rom", "my-rom.gba"].map(String::from);
        let options = parse(&args).unwrap();
        assert_eq!(options.target, "tbs-ja");
        assert_eq!(options.rom, Some("my-rom.gba"));
    }

    #[test]
    fn listings_are_isolated_from_maintained_sources() {
        let root = Path::new("/repo");
        for id in targets::TARGET_IDS {
            let target = targets::target_for(id);
            assert_eq!(
                raw_directory(root, target),
                root.join(format!("out/{id}/raw"))
            );
        }
    }

    #[test]
    fn discovered_thumb_extent_includes_its_pool() {
        // ldr r0,[pc,#0]; bx lr; literal word.
        let image = [0x00, 0x48, 0x70, 0x47, 0x78, 0x56, 0x34, 0x12];
        let mut discovery = Discovery::new(&image, OVERLAY_BASE);
        discovery.add_seed(OVERLAY_BASE, Mode::Thumb, "test");
        discovery.walk_function(OVERLAY_BASE);
        let info = discovery.function(OVERLAY_BASE).unwrap();
        assert_eq!(function_end(&discovery, info), Some(OVERLAY_BASE + 8));
        let source = thumb_source(&image, OVERLAY_BASE as u32, OVERLAY_BASE as u32, 8).unwrap();
        assert!(source.contains("ldr r0, [pc, #0]"));
        assert!(source.contains(".4byte 0x12345678"));
    }

    #[test]
    fn arm_listing_uses_mnemonics_and_removes_its_private_binary() {
        // mov r0,#1; bx lr.
        let image = [0x01, 0x00, 0xa0, 0xe3, 0x1e, 0xff, 0x2f, 0xe1];
        let mut discovery = Discovery::new(&image, ROM_BASE);
        discovery.add_seed(ROM_BASE, Mode::Arm, "test");
        discovery.walk_function(ROM_BASE);
        let work = tempfile::tempdir().unwrap();
        let rows = arm_rows(work.path(), &discovery).unwrap();
        assert!(rows[&ROM_BASE].starts_with("mov"));
        assert!(rows[&(ROM_BASE + 4)].starts_with("bx"));
        assert!(!work.path().join("arm.bin").exists());
    }

    #[test]
    fn explicit_rom_is_checked_before_any_output() {
        let root = tempfile::tempdir().unwrap();
        let target = targets::target_for(targets::DecompTargetId::TbsEn);
        let mut image = vec![0u8; target.rom_size as usize];
        image[0x100..0x104].copy_from_slice(&(ROM_BASE as u32).to_le_bytes());
        image[0x104..0x108].copy_from_slice(&((ROM_BASE + 0x100) as u32).to_le_bytes());
        std::fs::write(root.path().join("reference.gba"), image).unwrap();
        let error = load_rom(root.path(), target, Some("reference.gba"))
            .err()
            .unwrap();
        assert!(error.contains("differs from the approved tbs-en reference ROM"));
        assert!(!root.path().join("out").exists());
    }
}
