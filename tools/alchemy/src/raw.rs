//! Rebuildable unresolved assembly ownership.
//!
//! The ROM supplies complete overlay listings. Maintained translation-unit
//! declarations supply the ranges owned by C; the normal build then verifies
//! those declarations. Rebuilding never consults a saved
//! disassembly, classification database, compression recipe or answer table.

use crate::compiler::routing;
use crate::compiler::source_paths::{SourceOwner, SourcePaths};
use crate::compiler::translation_units::{OwnerState, TranslationUnits};
use crate::coverage::proof;
use crate::overlay::assembly::{build_region_source, export_overlay_source, OVERLAY_BASE};
use crate::overlay::compile::assemble_overlay_raw;
use crate::overlay::rom::CanonicalRom;
use crate::overlay::source::OverlaySource;
use crate::overlay::{internal_aliases, listing_offsets, placeholder_lines, region_lines};
use crate::targets::{self, DecompTarget, DecompTargetId};
use psynergy::assembly::thumb_source;
use psynergy::discovery::{Discovery, Mode, ROM_BASE};
use std::path::Path;

const USAGE: &str = "usage: alchemy raw status --target tbs-en|tla-en\n\
       alchemy raw rebuild --target tbs-en|tla-en\n\
status reports discovery and receipt state. rebuild recreates raw/overlays from\n\
the ROM, omitting only ranges declared as exact C.";

struct Options<'a> {
    command: &'a str,
    target_name: &'a str,
}

fn parse(arguments: &[String]) -> Result<Options<'_>, String> {
    if arguments == ["--help"] || arguments == ["-h"] {
        return Err(String::new());
    }
    let command = arguments.first().map(String::as_str).ok_or(USAGE)?;
    if !matches!(command, "status" | "rebuild") {
        return Err(USAGE.into());
    }
    let mut target_name = None;
    let mut index = 1;
    while index < arguments.len() {
        match arguments[index].as_str() {
            "--target" if target_name.is_none() => {
                target_name = Some(
                    arguments
                        .get(index + 1)
                        .ok_or_else(|| "--target needs a value".to_string())?
                        .as_str(),
                );
                index += 2;
            }
            flag => return Err(format!("unknown or duplicate flag {flag}\n{USAGE}")),
        }
    }
    Ok(Options {
        command,
        target_name: target_name.ok_or_else(|| "--target is required".to_string())?,
    })
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let options = match parse(arguments) {
        Err(error) if error.is_empty() => {
            println!("{USAGE}");
            return Ok(());
        }
        result => result?,
    };
    let target = targets::decomp_target(Some(options.target_name))?;
    canonical_target(target)?;
    let root = routing::root();
    match options.command {
        "status" => status(&root, target, options.target_name),
        "rebuild" => rebuild(&root, target, options.target_name),
        _ => unreachable!(),
    }
}

fn canonical_target(target: DecompTarget) -> Result<(), String> {
    if matches!(target.id, DecompTargetId::TbsEn | DecompTargetId::TlaEn) {
        Ok(())
    } else {
        Err("raw ownership currently uses the canonical tbs-en or tla-en image".into())
    }
}

fn status(root: &Path, target: DecompTarget, target_name: &str) -> Result<(), String> {
    let rom =
        std::fs::read(root.join(target.rom)).map_err(|error| format!("{}: {error}", target.rom))?;
    let mut discovery = Discovery::new(&rom, ROM_BASE);
    discovery.run();
    let (credit_state, exact_c_bytes) = match proof::read(root, target_name) {
        Ok(receipt) => (
            "fresh",
            receipt
                .credits
                .iter()
                .filter(|credit| credit.kind == "c")
                .map(|credit| credit.end.saturating_sub(credit.start))
                .sum::<i64>(),
        ),
        Err(_) => ("missing-or-stale", 0),
    };
    let raw_files = walkdir::WalkDir::new(root.join(target.asm_dir))
        .into_iter()
        .filter_map(Result::ok)
        .filter(|entry| entry.file_type().is_file())
        .count();
    println!(
        "target={target_name} functions={} instructions={} exact_c_receipt={credit_state} exact_c_bytes={exact_c_bytes} raw_files={raw_files}",
        discovery.function_count(),
        discovery.instructions.len(),
    );
    Ok(())
}

fn rebuild(root: &Path, target: DecompTarget, target_name: &str) -> Result<(), String> {
    let units = TranslationUnits::declared_game(root, target.compiler)?;
    let holes = overlay_holes(&units)?;
    let veneer_includes = veneer_includes(root, target)?;
    let rom = CanonicalRom::load_target(root, target)?;
    let resources = rom.overlay_resources(target.overlay_entry_veneers);
    let workers = crate::parallel::workers(resources.len());
    eprintln!(
        "rebuilding {} raw overlays with {workers} workers",
        resources.len()
    );
    let next = std::sync::atomic::AtomicUsize::new(0);
    let results = std::sync::Mutex::new(Vec::with_capacity(resources.len()));
    std::thread::scope(|scope| {
        for _ in 0..workers {
            let next = &next;
            let results = &results;
            let resources = &resources;
            let rom = &rom;
            let holes = &holes;
            let veneer_includes = &veneer_includes;
            scope.spawn(move || loop {
                let index = next.fetch_add(1, std::sync::atomic::Ordering::Relaxed);
                let Some(&resource) = resources.get(index) else {
                    break;
                };
                let overlay = format!("resource_{resource:03x}");
                let declared = holes.get(&overlay).map(Vec::as_slice).unwrap_or(&[]);
                results.lock().unwrap().push((
                    index,
                    rebuild_overlay(root, target, rom, resource, declared, veneer_includes),
                ));
            });
        }
    });
    let mut results = results.into_inner().unwrap();
    results.sort_by_key(|(index, _)| *index);

    let game = root.join(target.game_dir());
    let stage = tempfile::Builder::new()
        .prefix(".raw-overlays-")
        .tempdir_in(&game)
        .map_err(|error| error.to_string())?;
    for (_, result) in results {
        let (name, source) = result?;
        std::fs::write(stage.path().join(name), source).map_err(|error| error.to_string())?;
    }

    let raw = root.join(target.asm_dir);
    std::fs::create_dir_all(&raw).map_err(|error| error.to_string())?;
    let destination = raw.join("overlays");
    let backup = raw.join(".overlays-previous");
    if backup.exists() {
        return Err(format!(
            "stale rebuild backup exists at {}",
            backup.display()
        ));
    }
    if destination.exists() {
        std::fs::rename(&destination, &backup).map_err(|error| error.to_string())?;
    }
    if let Err(error) = std::fs::rename(stage.keep(), &destination) {
        if backup.exists() {
            let _ = std::fs::rename(&backup, &destination);
        }
        return Err(error.to_string());
    }
    if backup.exists() {
        std::fs::remove_dir_all(backup).map_err(|error| error.to_string())?;
    }
    let main_files = rebuild_main(root, target, &units)?;
    println!(
        "target={target_name} overlays={} main_regions={main_files} workers={workers} directory={}",
        resources.len(),
        destination.display()
    );
    Ok(())
}

fn merge_ranges(mut ranges: Vec<(i64, i64)>) -> Vec<(i64, i64)> {
    ranges.retain(|(start, end)| end > start);
    ranges.sort_unstable();
    let mut merged: Vec<(i64, i64)> = Vec::new();
    for range in ranges {
        if let Some(last) = merged.last_mut() {
            if range.0 <= last.1 {
                last.1 = last.1.max(range.1);
                continue;
            }
        }
        merged.push(range);
    }
    merged
}

fn subtract_ranges(ranges: Vec<(i64, i64)>, cuts: Vec<(i64, i64)>) -> Vec<(i64, i64)> {
    let cuts = merge_ranges(cuts);
    let mut output = Vec::new();
    for (start, end) in merge_ranges(ranges) {
        let mut cursor = start;
        for &(cut_start, cut_end) in &cuts {
            if cut_end <= cursor {
                continue;
            }
            if cut_start >= end {
                break;
            }
            if cut_start > cursor {
                output.push((cursor, cut_start.min(end)));
            }
            cursor = cursor.max(cut_end);
        }
        if cursor < end {
            output.push((cursor, end));
        }
    }
    output
}

fn function_range(discovery: &Discovery, entry: i64, limit: i64) -> Option<(i64, i64)> {
    let function = discovery.function(entry)?;
    if function.mode != Mode::Thumb
        || function.instructions.is_empty()
        || function.instructions.iter().any(|address| *address < entry)
    {
        return None;
    }
    let mut end = entry;
    for &address in &function.instructions {
        let instruction = discovery.instructions.get(&address)?;
        end = end.max(address + instruction.size);
        if instruction.size == 2 {
            let at = usize::try_from(address - discovery.base).ok()?;
            let bytes = discovery.data.get(at..at + 2)?;
            let half = u16::from_le_bytes([bytes[0], bytes[1]]);
            if half & 0xf800 == 0x4800 {
                let pool = ((address + 4) & !3) + i64::from(half & 0xff) * 4;
                if discovery.inside(pool, 4) {
                    end = end.max(pool + 4);
                }
            }
        }
    }
    Some((entry, end.min(limit))).filter(|(start, end)| end > start)
}

fn rebuild_main(
    root: &Path,
    target: DecompTarget,
    units: &TranslationUnits,
) -> Result<usize, String> {
    let bytes = std::fs::read(root.join(target.rom)).map_err(|error| error.to_string())?;
    let mut discovery = Discovery::new(&bytes, ROM_BASE);
    discovery.run();
    let paths = SourcePaths::load_for_game(root, target.compiler.as_str())?;
    let mut unresolved = Vec::new();
    let mut maintained = crate::build_asm::maintained_assembly_bytes(root, target)?
        .into_iter()
        .map(|(module, module_bytes)| {
            let start = i64::try_from(module.load_address).map_err(|error| error.to_string())?;
            let end =
                start + i64::try_from(module_bytes.len()).map_err(|error| error.to_string())?;
            let offset = usize::try_from(start - ROM_BASE).map_err(|error| error.to_string())?;
            if module_bytes.as_slice()
                != bytes
                    .get(offset..offset + module_bytes.len())
                    .ok_or_else(|| {
                        format!(
                            "{}: maintained assembly is outside ROM",
                            module.source.display()
                        )
                    })?
            {
                return Err(format!(
                    "{}: maintained assembly differs from ROM",
                    module.source.display()
                ));
            }
            Ok((start, end))
        })
        .collect::<Result<Vec<_>, String>>()?;
    let runtime = crate::compiler::runtime::Registry::load(root)?;
    for link in runtime.links_for("main") {
        let data = crate::compiler::runtime::build(root, link)?.text;
        maintained.push((
            i64::from(link.text),
            i64::from(link.text) + data.len() as i64,
        ));
    }
    maintained.extend(
        units
            .units
            .iter()
            .filter(|unit| unit.overlay.is_none())
            .flat_map(|unit| unit.owners.iter())
            .filter(|owner| owner.state == OwnerState::ExactC)
            .map(|owner| {
                (
                    i64::from(owner.address),
                    i64::from(owner.address) + owner.extent as i64,
                )
            }),
    );
    let retained = units
        .units
        .iter()
        .filter(|unit| unit.overlay.is_none())
        .flat_map(|unit| unit.owners.iter())
        .filter(|owner| owner.state == OwnerState::RetainedAssembly)
        .map(|owner| {
            (
                i64::from(owner.address),
                i64::from(owner.address) + owner.extent as i64,
            )
        })
        .collect::<Vec<_>>();
    let mut entries = discovery.function_entries();
    entries.sort_unstable();
    for (index, &entry) in entries.iter().enumerate() {
        let limit = entries.get(index + 1).copied().unwrap_or(discovery.limit);
        let Some(range) = function_range(&discovery, entry, limit) else {
            continue;
        };
        if paths
            .mapped_source_path(SourceOwner::Main(entry as u32))
            .is_some()
        {
            maintained.push(range);
        } else {
            unresolved.push(range);
        }
    }
    let gaps = executable_gap_ranges(root, target)?;
    let mut cuts = maintained;
    cuts.extend(retained.iter().copied());
    cuts.extend(gaps);
    let mut ranges = subtract_ranges(unresolved, cuts);
    ranges.extend(retained);
    ranges.sort_unstable();
    let workers = crate::parallel::workers(ranges.len());
    eprintln!(
        "rebuilding {} raw main regions with {workers} workers",
        ranges.len()
    );
    let next = std::sync::atomic::AtomicUsize::new(0);
    let results = std::sync::Mutex::new(Vec::with_capacity(ranges.len()));
    std::thread::scope(|scope| {
        for _ in 0..workers {
            let next = &next;
            let results = &results;
            let ranges = &ranges;
            let bytes = &bytes;
            scope.spawn(move || loop {
                let index = next.fetch_add(1, std::sync::atomic::Ordering::Relaxed);
                let Some(&(start, end)) = ranges.get(index) else {
                    break;
                };
                let offset = (start - ROM_BASE) as usize;
                let image = &bytes[offset..offset + (end - start) as usize];
                let readable = build_region_source(image, start).and_then(|source| {
                    let assembled =
                        assemble_overlay_raw(&OverlaySource::text(source.clone()), start)?;
                    if assembled == image {
                        Ok(source)
                    } else {
                        Err("reconstructed listing differs from ROM".into())
                    }
                });
                let result = match readable {
                    Ok(source) => Ok((format!("{start:08x}.s"), source)),
                    Err(_) => thumb_source(image, start as u32, start as u32, image.len() as u32)
                        .and_then(|source| {
                            assemble_overlay_raw(&OverlaySource::text(source.clone()), start)
                                .and_then(|assembled| {
                                    (assembled == image)
                                        .then_some((format!("{start:08x}.s"), source))
                                        .ok_or_else(|| {
                                            format!(
                                            "main:{start:08x}: literal listing differs from ROM"
                                        )
                                        })
                                })
                        }),
                };
                results.lock().unwrap().push((index, result));
            });
        }
    });
    let mut results = results.into_inner().unwrap();
    results.sort_by_key(|(index, _)| *index);
    let raw = root.join(target.asm_dir);
    let stage = tempfile::Builder::new()
        .prefix(".raw-main-")
        .tempdir_in(root.join(target.game_dir()))
        .map_err(|error| error.to_string())?;
    for (_, result) in results {
        let (name, source) = result?;
        std::fs::write(stage.path().join(name), source).map_err(|error| error.to_string())?;
    }
    for entry in std::fs::read_dir(&raw).map_err(|error| error.to_string())? {
        let path = entry.map_err(|error| error.to_string())?.path();
        if path.extension().and_then(|extension| extension.to_str()) == Some("s") {
            std::fs::remove_file(path).map_err(|error| error.to_string())?;
        }
    }
    for entry in std::fs::read_dir(stage.path()).map_err(|error| error.to_string())? {
        let path = entry.map_err(|error| error.to_string())?.path();
        std::fs::rename(&path, raw.join(path.file_name().unwrap()))
            .map_err(|error| error.to_string())?;
    }
    Ok(ranges.len())
}

fn executable_gap_ranges(root: &Path, target: DecompTarget) -> Result<Vec<(i64, i64)>, String> {
    let directory = root.join(target.game_dir()).join("raw/executable_gaps");
    if !directory.is_dir() {
        return Ok(Vec::new());
    }
    let mut ranges = Vec::new();
    for entry in walkdir::WalkDir::new(directory) {
        let entry = entry.map_err(|e| e.to_string())?;
        if !entry.file_type().is_file()
            || entry.path().extension().and_then(|e| e.to_str()) != Some("s")
        {
            continue;
        }
        let stem = entry
            .path()
            .file_stem()
            .and_then(|s| s.to_str())
            .ok_or("executable gap has no address name")?;
        let start = i64::from_str_radix(stem, 16).map_err(|e| e.to_string())?;
        let source = std::fs::read_to_string(entry.path()).map_err(|e| e.to_string())?;
        let bytes = assemble_overlay_raw(&OverlaySource::text(source), start)?;
        ranges.push((start, start + bytes.len() as i64));
    }
    Ok(merge_ranges(ranges))
}

fn veneer_includes(root: &Path, target: DecompTarget) -> Result<Vec<(String, String)>, String> {
    let mut includes = Vec::new();
    for entry in walkdir::WalkDir::new(root.join(target.source_dir)) {
        let entry = entry.map_err(|error| error.to_string())?;
        if !entry.file_type().is_file()
            || !matches!(entry.file_name().to_str(), Some("ENTRY.INC" | "IMPORT.INC"))
        {
            continue;
        }
        let path = entry
            .path()
            .strip_prefix(root)
            .map_err(|error| error.to_string())?
            .to_string_lossy()
            .into_owned();
        let body = std::fs::read_to_string(entry.path()).map_err(|error| error.to_string())?;
        includes.push((body.trim().to_string(), format!(".include \"{path}\"")));
    }
    includes.sort_by(|left, right| left.1.cmp(&right.1));
    Ok(includes)
}

fn use_maintained_veneer_includes(mut source: String, includes: &[(String, String)]) -> String {
    for (body, include) in includes {
        if source.contains(body) {
            source = source.replacen(body, include, 1);
        }
    }
    source
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
enum HoleKind {
    Code,
    Data,
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
struct Hole {
    start: i64,
    end: i64,
    kind: HoleKind,
}

fn overlay_holes(
    units: &TranslationUnits,
) -> Result<std::collections::BTreeMap<String, Vec<Hole>>, String> {
    let mut by_image: std::collections::BTreeMap<String, Vec<Hole>> =
        std::collections::BTreeMap::new();
    for unit in &units.units {
        if let Some(image) = &unit.overlay {
            for owner in &unit.owners {
                if owner.state == OwnerState::ExactC {
                    by_image.entry(image.clone()).or_default().push(Hole {
                        start: i64::from(owner.address),
                        end: i64::from(owner.address) + owner.extent as i64,
                        kind: HoleKind::Code,
                    });
                }
            }
            if unit.exact() {
                by_image
                    .entry(image.clone())
                    .or_default()
                    .extend(unit.local_symbols.iter().map(|symbol| Hole {
                        start: i64::from(symbol.address),
                        end: i64::from(symbol.address) + symbol.extent as i64,
                        kind: HoleKind::Code,
                    }));
                if let Some(data) = unit.data {
                    by_image.entry(image.clone()).or_default().push(Hole {
                        start: i64::from(data.address),
                        end: i64::from(data.address) + data.extent as i64,
                        kind: HoleKind::Data,
                    });
                }
            }
        }
        if unit.exact() {
            for (image, instance) in &unit.instances {
                if image == "main" {
                    continue;
                }
                by_image
                    .entry(image.clone())
                    .or_default()
                    .extend(instance.owners.values().map(|owner| Hole {
                        start: i64::from(owner.address),
                        end: i64::from(owner.address) + owner.extent as i64,
                        kind: HoleKind::Code,
                    }));
            }
        }
    }
    for (image, holes) in &mut by_image {
        holes.sort_by_key(|hole| (hole.start, hole.end));
        holes.dedup();
        for pair in holes.windows(2) {
            if pair[0].end > pair[1].start {
                return Err(format!(
                    "{image}: declared C ranges overlap at {:#x}..{:#x}",
                    pair[1].start, pair[0].end
                ));
            }
        }
    }
    Ok(by_image)
}

fn rebuild_overlay(
    root: &Path,
    target: DecompTarget,
    rom: &CanonicalRom,
    resource: usize,
    holes: &[Hole],
    veneer_includes: &[(String, String)],
) -> Result<(String, String), String> {
    let overlay = format!("resource_{resource:03x}");
    let stream = rom.stream(resource)?;
    let source = use_maintained_veneer_includes(
        export_overlay_source(
            &stream.decoded,
            OVERLAY_BASE,
            &target.overlay_macro(),
            target.overlay_entry_veneers,
        )
        .map_err(|error| format!("{overlay}: {error}"))?,
        veneer_includes,
    );
    let assembled = assemble_overlay_raw(&OverlaySource::text(source.clone()), OVERLAY_BASE)?;
    if assembled != stream.decoded {
        return Err(format!("{overlay}: reconstructed listing differs from ROM"));
    }

    let output = root.join("out");
    std::fs::create_dir_all(&output).map_err(|error| error.to_string())?;
    let work = tempfile::Builder::new()
        .prefix("alchemy-raw-")
        .tempdir_in(output)
        .map_err(|error| error.to_string())?;
    let listing = work.path().join(format!("{overlay}.s"));
    std::fs::write(&listing, &source).map_err(|error| error.to_string())?;
    let offsets = listing_offsets(&listing)?;
    let original = source.lines().map(str::to_owned).collect::<Vec<_>>();
    let mut replacements = holes
        .iter()
        .map(|hole| {
            let offset = hole.start - OVERLAY_BASE;
            let span = hole.end - hole.start;
            if offset < 0 || span <= 0 || hole.end > OVERLAY_BASE + stream.decoded.len() as i64 {
                return Err(format!(
                    "{overlay}: invalid declared range {:#x}..{:#x}",
                    hole.start, hole.end
                ));
            }
            let (first, last) = region_lines(&offsets, offset, span)?;
            let aliases = internal_aliases(&original, first, last, offset, span)?;
            let mut replacement = placeholder_lines(&format!("{:08x}", hole.start), span, &aliases);
            if hole.kind == HoleKind::Data {
                replacement[0] = replacement[0].replacen("AlchemyC_", "AlchemyData_", 1);
            }
            Ok((first, last, replacement))
        })
        .collect::<Result<Vec<_>, String>>()?;
    replacements.sort_by_key(|(first, _, _)| std::cmp::Reverse(*first));
    let mut lines = original;
    for (first, last, replacement) in replacements {
        lines.splice((first - 1) as usize..last as usize, replacement);
    }
    Ok((format!("{overlay}_overlay.s"), lines.join("\n") + "\n"))
}

#[cfg(test)]
mod tests {
    use super::{canonical_target, parse};
    use crate::targets::{target_for, DecompTargetId};

    fn arguments(items: &[&str]) -> Vec<String> {
        items.iter().map(|item| item.to_string()).collect()
    }

    #[test]
    fn requires_command_and_canonical_target() {
        assert!(parse(&arguments(&["status"])).is_err());
        assert!(parse(&arguments(&["rebuild", "--target", "tbs-en"])).is_ok());
        assert!(canonical_target(target_for(DecompTargetId::TbsJa)).is_err());
        assert!(canonical_target(target_for(DecompTargetId::TlaEn)).is_ok());
    }
}
