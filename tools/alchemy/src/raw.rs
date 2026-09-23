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
use crate::overlay::assembly::{
    build_overlay_source, build_region_source, export_overlay_source, OVERLAY_BASE,
};
use crate::overlay::compile::assemble_overlay_raw;
use crate::overlay::rom::CanonicalRom;
use crate::overlay::source::OverlaySource;
use crate::overlay::{internal_aliases, listing_offsets, placeholder_lines, region_lines};
use crate::targets::{self, BuildSupport, DecompTarget, DecompTargetId};
use psynergy::assembly::thumb_source;
use psynergy::discovery::{Discovery, Mode, ROM_BASE};
use std::path::Path;

const USAGE: &str = "usage: alchemy raw status --target tbs-en|tla-en\n\
       alchemy raw rebuild --target tbs-en|tla-en\n\
status reports discovery and receipt state. rebuild recreates raw/overlays from\n\
the ROM, omitting only ranges declared as exact C, and the main listings; with a\n\
full ROM contract it compiles the claimed C and gives every other unowned byte\n\
of the main image, which ends at the first declared resource, a listing.";

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
    let runtime = runtime_windows(root, target)?;
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
            let runtime = &runtime;
            let veneer_includes = &veneer_includes;
            scope.spawn(move || loop {
                let index = next.fetch_add(1, std::sync::atomic::Ordering::Relaxed);
                let Some(&resource) = resources.get(index) else {
                    break;
                };
                let overlay = format!("resource_{resource:03x}");
                let declared = holes.get(&overlay).map(Vec::as_slice).unwrap_or(&[]);
                let reserved = runtime.get(&overlay).map(Vec::as_slice).unwrap_or(&[]);
                results.lock().unwrap().push((
                    index,
                    rebuild_overlay(
                        root,
                        target,
                        rom,
                        resource,
                        declared,
                        reserved,
                        veneer_includes,
                    ),
                ));
            });
        }
    });
    let mut results = results.into_inner().unwrap();
    results.sort_by_key(|(index, _)| *index);

    let recon = root.join(target.recon_dir());
    let stage = tempfile::Builder::new()
        .prefix(".raw-overlays-")
        .tempdir_in(&recon)
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

/// A registered owner that no listing, source or declared asset covers gets a
/// listing of its own, up to the next listing, owner or covered byte.
fn owner_listings(
    mut ranges: Vec<(i64, i64)>,
    owners: &std::collections::BTreeSet<i64>,
    cuts: &[(i64, i64)],
    limit: i64,
) -> Vec<(i64, i64)> {
    let covered = |address: i64| {
        ranges
            .iter()
            .chain(cuts)
            .any(|&(start, end)| start <= address && address < end)
    };
    let mut added = Vec::new();
    for &owner in owners {
        if owner >= limit || covered(owner) {
            continue;
        }
        let next = ranges
            .iter()
            .chain(cuts)
            .map(|&(start, _)| start)
            .chain(owners.range(owner + 1..).next().copied())
            .filter(|&start| start > owner)
            .min()
            .unwrap_or(limit)
            .min(limit);
        if next > owner {
            added.push((owner, next));
        }
    }
    ranges.extend(added);
    ranges.sort_unstable();
    ranges
}

/// The main-image ranges the target's claimed build compiles, links and
/// compares with the ROM: what C owns, as a walk of the ROM can only estimate.
fn claimed_ranges(root: &Path, target: DecompTarget) -> Result<Vec<(i64, i64)>, String> {
    let output = root.join("out");
    std::fs::create_dir_all(&output).map_err(|error| format!("{}: {error}", output.display()))?;
    let work = tempfile::Builder::new()
        .prefix("alchemy-raw-claimed-")
        .tempdir_in(&output)
        .map_err(|error| error.to_string())?;
    let executable =
        std::env::current_exe().map_err(|error| format!("alchemy executable: {error}"))?;
    let status = std::process::Command::new(executable)
        .args([
            "build",
            "claimed",
            "--target",
            &target.id.to_string(),
            "--output",
        ])
        .arg(work.path())
        .arg(root.join(target.rom))
        .current_dir(root)
        .stdin(std::process::Stdio::null())
        .stdout(std::process::Stdio::null())
        .status()
        .map_err(|error| format!("claimed build: {error}"))?;
    if !status.success() {
        return Err(format!(
            "claimed build for {} failed; raw ownership needs the C it compiles",
            target.id
        ));
    }
    let path = work.path().join("manifest.json");
    let manifest: serde_json::Value =
        serde_json::from_slice(&std::fs::read(&path).map_err(|error| error.to_string())?)
            .map_err(|error| format!("{}: {error}", path.display()))?;
    manifest["regions"]
        .as_array()
        .ok_or("claimed manifest lists no regions")?
        .iter()
        .map(|region| {
            let address = region["address"].as_i64().ok_or("claimed region address")?;
            let size = region["size"].as_i64().ok_or("claimed region size")?;
            Ok((address, address + size))
        })
        .collect()
}

/// Which listings end with the zero halfword that word-aligns what follows: a
/// word-aligned listing that stops on a halfword boundary before an unowned
/// zero halfword. A retained member of a declared unit keeps its declared
/// extent; the unit, not the listing, owns alignment between its members.
fn alignment_halfwords(
    ranges: &[(i64, i64)],
    cuts: &[(i64, i64)],
    declared: &[(i64, i64)],
    bytes: &[u8],
    limit: i64,
) -> Vec<bool> {
    let owned = |address: i64| {
        ranges
            .iter()
            .chain(cuts)
            .any(|&(start, end)| start <= address && address < end)
    };
    ranges
        .iter()
        .map(|&(start, end)| {
            let offset = (end - ROM_BASE) as usize;
            start % 4 == 0
                && end % 4 == 2
                && end + 2 <= limit
                && !declared.contains(&(start, end))
                && bytes.get(offset..offset + 2) == Some(&[0, 0][..])
                && !owned(end)
                && !owned(end + 1)
        })
        .collect()
}

/// Every main-image byte below `limit` that no listing or cut owns still needs
/// a source, or the full build cannot own it. A hole directly after an
/// unresolved listing is the rest of that listing's section: its alignment,
/// data or code no walk reached. Any other hole is a listing of its own, with
/// no entry (`false`) unless its bytes prove one. Retained unit members keep
/// their declared extents. Each listing is `(start, end, entry)`.
fn complete_listings(
    ranges: Vec<(i64, i64)>,
    retained: &[(i64, i64)],
    cuts: &[(i64, i64)],
    base: i64,
    limit: i64,
) -> Vec<(i64, i64, bool)> {
    let owned = ranges.iter().chain(cuts).copied().collect::<Vec<_>>();
    let holes = subtract_ranges(vec![(base, limit)], owned);
    let mut listings = ranges
        .into_iter()
        .map(|(start, end)| (start, end, true))
        .collect::<Vec<_>>();
    for (start, end) in holes {
        match listings
            .iter_mut()
            .find(|(first, last, _)| *last == start && !retained.contains(&(*first, *last)))
        {
            Some(listing) => listing.1 = end,
            None => listings.push((start, end, false)),
        }
    }
    listings.sort_unstable();
    listings
}

/// A `push {..., lr}` opening, the prologue the listing walk treats as a
/// function entry.
fn thumb_prologue(image: &[u8]) -> bool {
    image.len() >= 2 && image[1] == 0xb5
}

/// A hole that no walk reaches and that opens with no prologue: the listing
/// decodes only what a prologue, veneer or call inside it reaches and writes
/// the rest as data, under no entry label.
fn hole_source(image: &[u8], start: i64) -> Result<String, String> {
    let label = format!("Overlay_{start:08x}");
    let global = format!("\t.global {label}");
    let definition = format!("{label}:");
    let source = build_overlay_source(image, start)?
        .lines()
        .filter(|line| *line != global && *line != definition)
        .map(|line| format!("{line}\n"))
        .collect::<String>();
    if assemble_overlay_raw(&OverlaySource::text(source.clone()), start)? != image {
        return Err("hole listing differs from ROM".into());
    }
    Ok(source)
}

/// Split each range at the registered owner starts strictly inside it.
fn split_at_owners(
    ranges: Vec<(i64, i64)>,
    owners: &std::collections::BTreeSet<i64>,
) -> Vec<(i64, i64)> {
    let mut split = Vec::with_capacity(ranges.len());
    for (start, end) in ranges {
        let mut cursor = start;
        for &owner in owners.range(start + 1..end) {
            split.push((cursor, owner));
            cursor = owner;
        }
        split.push((cursor, end));
    }
    split
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
    // A registered owner is evidence of a function entry that discovery may
    // not reach from the image's own seeds. Only the owner itself joins the
    // entries; whatever its walk calls is not evidence of anything.
    let mut entries = discovery.function_entries();
    for owner in paths.registered_owners().filter(|owner| owner.is_main()) {
        let address = i64::from(owner.address());
        if discovery.function(address).is_none()
            && discovery.add_seed(address, Mode::Thumb, "owner register")
        {
            discovery.walk_function(address);
            entries.push(address);
        }
    }
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
    if let Some(runtime) =
        crate::compiler::runtime::Registry::load_if_present(root, target.compiler)?
    {
        for link in runtime.links_for("main") {
            let data = crate::compiler::runtime::build(root, target.compiler, link)?.text;
            maintained.push((
                i64::from(link.text),
                i64::from(link.text) + data.len() as i64,
            ));
        }
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
    // Static functions of an exact unit are compiled with it.
    maintained.extend(
        units
            .units
            .iter()
            .filter(|unit| unit.overlay.is_none() && unit.exact())
            .flat_map(|unit| unit.local_symbols.iter())
            .map(|symbol| {
                (
                    i64::from(symbol.address),
                    i64::from(symbol.address) + symbol.extent as i64,
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
    // With a full ROM contract, compiling decides what C owns: the claimed
    // build's linked ranges replace the walk's estimate of each C owner.
    let claimed = if target.build_support == BuildSupport::Full {
        Some(claimed_ranges(root, target)?)
    } else {
        None
    };
    if let Some(claimed) = &claimed {
        maintained.extend(claimed.iter().copied());
    }
    entries.sort_unstable();
    entries.dedup();
    for (index, &entry) in entries.iter().enumerate() {
        let limit = entries.get(index + 1).copied().unwrap_or(discovery.limit);
        let Some(range) = function_range(&discovery, entry, limit) else {
            continue;
        };
        if paths
            .mapped_source_path(SourceOwner::Main(entry as u32))
            .is_none()
        {
            unresolved.push(range);
        } else if claimed.is_none() {
            maintained.push(range);
        }
    }
    let gaps = executable_gap_ranges(root, target)?;
    let mut cuts = maintained;
    cuts.extend(declared_asset_ranges(root, target)?);
    cuts.extend(retained.iter().copied());
    cuts.extend(gaps);
    let mut ranges = subtract_ranges(unresolved, cuts.clone());
    ranges.extend(retained.iter().copied());
    ranges.sort_unstable();
    // Every registered owner starts its own listing, even where discovery
    // walked one function into the next.
    let owners = paths
        .registered_owners()
        .filter(|owner| owner.is_main())
        .map(|owner| i64::from(owner.address()))
        .collect::<std::collections::BTreeSet<_>>();
    let ranges = owner_listings(
        split_at_owners(ranges, &owners),
        &owners,
        &cuts,
        discovery.limit,
    );
    // Only a full ROM contract defines every other owner, so only there can
    // the remaining bytes be completed without guessing what C owns.
    let ranges = if claimed.is_some() {
        complete_listings(
            ranges,
            &retained,
            &cuts,
            ROM_BASE,
            main_image_end(root, target, &bytes)?.min(discovery.limit),
        )
    } else {
        ranges
            .into_iter()
            .map(|(start, end)| (start, end, true))
            .collect()
    };
    // Alignment halfwords are read from the final listings. Completion leaves
    // no unowned byte before the resource directory, so a zero halfword after
    // a listing there is already the tail of that listing.
    let spans = ranges
        .iter()
        .map(|&(start, end, _)| (start, end))
        .collect::<Vec<_>>();
    let aligned = alignment_halfwords(&spans, &cuts, &retained, &bytes, discovery.limit);
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
            let paths = &paths;
            let aligned = &aligned;
            scope.spawn(move || loop {
                let index = next.fetch_add(1, std::sync::atomic::Ordering::Relaxed);
                let Some(&(start, end, entry)) = ranges.get(index) else {
                    break;
                };
                let align = aligned[index];
                let offset = (start - ROM_BASE) as usize;
                let image = &bytes[offset..offset + (end - start) as usize];
                if !entry && !thumb_prologue(image) {
                    let result = hole_source(image, start)
                        .map(|source| (format!("{start:08x}.s"), source))
                        .map_err(|error| format!("main:{start:08x}: {error}"));
                    results.lock().unwrap().push((index, result));
                    continue;
                }
                let readable = build_region_source(image, start)
                    .map(|source| owner_entry(&source, start, paths))
                    .and_then(|source| {
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
                        .map(|source| owner_entry(&source, start, paths))
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
                let result = result.and_then(|(name, source)| {
                    if !align {
                        return Ok((name, source));
                    }
                    let source = format!("{source}\t.align 2, 0\n");
                    let image = &bytes[offset..offset + (end - start) as usize + 2];
                    (assemble_overlay_raw(&OverlaySource::text(source.clone()), start)? == image)
                        .then_some((name, source))
                        .ok_or_else(|| {
                            format!("main:{start:08x}: aligned listing differs from ROM")
                        })
                });
                results.lock().unwrap().push((index, result));
            });
        }
    });
    let mut results = results.into_inner().unwrap();
    results.sort_by_key(|(index, _)| *index);
    let raw = root.join(target.asm_dir);
    let stage = tempfile::Builder::new()
        .prefix(".raw-main-")
        .tempdir_in(root.join(target.recon_dir()))
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

/// Where the main image ends and the resources the asset build owns begin:
/// the resource directory itself (its first entry names the ROM base and its
/// second the directory), or any earlier resource a later entry points at
/// that the asset build declares. Bytes beyond it are never a listing's; an
/// unreproduced asset byte there stays unowned rather than copied.
fn main_image_end(root: &Path, target: DecompTarget, bytes: &[u8]) -> Result<i64, String> {
    let table = crate::overlay::rom::resource_table(bytes)?;
    let rom = CanonicalRom::load_target(root, target)?;
    let assets = declared_asset_ranges(root, target)?
        .into_iter()
        .map(|(start, _)| start)
        .collect::<std::collections::BTreeSet<_>>();
    let mut end = ROM_BASE + i64::try_from(table).map_err(|error| error.to_string())?;
    for resource in 2..rom.resource_count() {
        let address = ROM_BASE
            + i64::try_from(rom.resource_pointer(resource)?).map_err(|error| error.to_string())?;
        if assets.contains(&address) {
            end = end.min(address);
        }
    }
    Ok(end)
}

/// Name a main-image listing's entry as the owner register does: the
/// registered name, the address name that older callers link against, and the
/// Thumb function marker that gives both their interworking bit.
fn owner_entry(source: &str, start: i64, paths: &SourcePaths) -> String {
    let owner = SourceOwner::Main(start as u32);
    let address = owner.legacy_name();
    let name = paths.registered_name(owner).unwrap_or(&address);
    let entries = [
        format!("Overlay_{start:08x}"),
        address.clone(),
        name.to_string(),
    ];
    let mut head = vec![format!("\t.global {name}")];
    if name != address {
        head.push(format!("\t.global {address}"));
    }
    head.push("\t.thumb_func".into());
    head.push(format!("{name}:"));
    if name != address {
        head.push(format!("{address}:"));
    }
    let mut lines = Vec::new();
    let mut named = false;
    for line in source.lines() {
        let entry = line
            .strip_prefix("\t.global ")
            .or_else(|| line.strip_suffix(':'))
            .is_some_and(|symbol| entries.iter().any(|entry| entry == symbol));
        if entry || line == "\t.thumb_func" {
            if !named {
                lines.extend(head.iter().cloned());
                named = true;
            }
            continue;
        }
        lines.push(line.to_string());
    }
    format!("{}\n", lines.join("\n"))
}

/// The regions the asset build declares: listings never cover declared data.
/// The asset manifest is expanded as the asset build expands it, series and
/// closure packages included; the private-input registry adds its regions.
fn declared_asset_ranges(root: &Path, target: DecompTarget) -> Result<Vec<(i64, i64)>, String> {
    let mut ranges = Vec::new();
    if root.join(target.asset_manifest).is_file() {
        for (start, end) in crate::build_assets::declared_ranges(root, target.asset_manifest)? {
            let start = i64::try_from(start).map_err(|error| error.to_string())?;
            let end = i64::try_from(end).map_err(|error| error.to_string())?;
            if end > start {
                ranges.push((start, end));
            }
        }
    }
    for name in ["private-inputs.json"] {
        let path = root.join(target.recon_dir()).join(name);
        if !path.is_file() {
            continue;
        }
        let document: serde_json::Value =
            serde_json::from_slice(&std::fs::read(&path).map_err(|e| e.to_string())?)
                .map_err(|e| format!("{}: {e}", path.display()))?;
        for region in document["regions"].as_array().into_iter().flatten() {
            let number = |key: &str| {
                region[key]
                    .as_str()
                    .and_then(|text| i64::from_str_radix(text.trim_start_matches("0x"), 16).ok())
            };
            if let (Some(address), Some(size)) = (number("address"), number("size")) {
                if size > 0 {
                    ranges.push((address, address + size));
                }
            }
        }
    }
    Ok(ranges)
}

fn executable_gap_ranges(root: &Path, target: DecompTarget) -> Result<Vec<(i64, i64)>, String> {
    let directory = root.join(target.asm_dir).join("executable_gaps");
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

/// The compiler-runtime windows of each overlay: the bytes the container
/// builds for every registered link, placed at its text and read-only data
/// addresses. Listings reserve these windows and never carry the bytes.
fn runtime_windows(
    root: &Path,
    target: DecompTarget,
) -> Result<std::collections::BTreeMap<String, Vec<(i64, Vec<u8>)>>, String> {
    let mut windows = std::collections::BTreeMap::<String, Vec<(i64, Vec<u8>)>>::new();
    let Some(registry) =
        crate::compiler::runtime::Registry::load_if_present(root, target.compiler)?
    else {
        return Ok(windows);
    };
    for link in registry.links.iter().filter(|link| link.overlay()) {
        let linked = crate::compiler::runtime::build(root, target.compiler, link)?;
        let image = windows.entry(link.image.clone()).or_default();
        image.push((i64::from(link.text), linked.text));
        if let Some(rodata) = link.rodata {
            image.push((i64::from(rodata), linked.rodata));
        }
    }
    Ok(windows)
}

#[allow(clippy::too_many_arguments)]
fn rebuild_overlay(
    root: &Path,
    target: DecompTarget,
    rom: &CanonicalRom,
    resource: usize,
    holes: &[Hole],
    runtime: &[(i64, Vec<u8>)],
    veneer_includes: &[(String, String)],
) -> Result<(String, String), String> {
    let overlay = format!("resource_{resource:03x}");
    let stream = rom.stream(resource)?;
    // Each declared range and runtime window is later cut out on whole
    // listing rows, so no data row may carry a byte across either of its ends.
    let reserved = holes
        .iter()
        .copied()
        .chain(runtime.iter().map(|(start, bytes)| Hole {
            start: *start,
            end: start + bytes.len() as i64,
            kind: HoleKind::Code,
        }))
        .collect::<Vec<_>>();
    let boundaries: std::collections::BTreeSet<i64> = reserved
        .iter()
        .flat_map(|hole| [hole.start, hole.end])
        .collect();
    let source = use_maintained_veneer_includes(
        export_overlay_source(
            &stream.decoded,
            OVERLAY_BASE,
            &target.overlay_macro(),
            target.overlay_entry_veneers,
            &boundaries,
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
    for (start, bytes) in runtime {
        let offset = start - OVERLAY_BASE;
        let span = bytes.len() as i64;
        let image = usize::try_from(offset)
            .ok()
            .and_then(|at| stream.decoded.get(at..at + bytes.len()));
        if image != Some(bytes.as_slice()) {
            return Err(format!(
                "{overlay}: container-built runtime differs from the image at {start:#x}"
            ));
        }
        // Code that calls into the window names its entries by address, as
        // it names any routine outside the listing.
        let (first, last) = region_lines(&offsets, offset, span)?;
        let mut replacement = internal_aliases(&original, first, last, offset, span)?
            .into_iter()
            .map(|alias| format!("\t.set {}, 0x{:08x}", alias.label, start + alias.offset))
            .collect::<Vec<_>>();
        replacement.push(format!("{}{start:08x}:", crate::compiler::runtime::LABEL));
        replacement.push(format!("\t.space {span:#x}"));
        replacements.push((first, last, replacement));
    }
    replacements.sort_by_key(|(first, _, _)| std::cmp::Reverse(*first));
    let mut lines = original;
    for (first, last, replacement) in replacements {
        lines.splice((first - 1) as usize..last as usize, replacement);
    }
    let text = lines.join("\n") + "\n";
    let placed = assemble_overlay_raw(&OverlaySource::text(text.clone()), OVERLAY_BASE)?;
    holes_replace_only_declared_bytes(&placed, &stream.decoded, &reserved)
        .map_err(|error| format!("{overlay}: {error}"))?;
    Ok((format!("{overlay}_overlay.s"), text))
}

/// The listing with its declared ranges cut out must still have the ROM
/// image's layout: every declared byte a zero placeholder, every other byte
/// the ROM's own. A row that ran past a range would shift what follows.
fn holes_replace_only_declared_bytes(
    placed: &[u8],
    decoded: &[u8],
    holes: &[Hole],
) -> Result<(), String> {
    if placed.len() != decoded.len() {
        return Err(format!(
            "placeholder listing assembles to {:#x} bytes, the image has {:#x}",
            placed.len(),
            decoded.len()
        ));
    }
    let declared = |address: i64| {
        holes
            .iter()
            .any(|hole| hole.start <= address && address < hole.end)
    };
    for (offset, (placed, decoded)) in placed.iter().zip(decoded).enumerate() {
        let address = OVERLAY_BASE + offset as i64;
        let expected = if declared(address) { 0 } else { *decoded };
        if *placed != expected {
            return Err(format!(
                "placeholder listing differs from the image layout at {address:#010x}"
            ));
        }
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::{
        alignment_halfwords, canonical_target, holes_replace_only_declared_bytes, owner_entry,
        parse, Hole, HoleKind, ROM_BASE,
    };
    use crate::compiler::source_paths::SourcePaths;
    use crate::overlay::assembly::OVERLAY_BASE;
    use crate::targets::{target_for, DecompTargetId};

    #[test]
    fn only_word_aligned_listings_take_an_unowned_zero_alignment_halfword() {
        let at = |offset: i64| ROM_BASE + offset;
        let mut bytes = vec![0xffu8; 0x40];
        for zero in [0x06, 0x16, 0x26, 0x32, 0x3a] {
            bytes[zero] = 0;
            bytes[zero + 1] = 0;
        }
        let ranges = [
            (at(0x00), at(0x06)), // takes the zero halfword before 0x08
            (at(0x08), at(0x10)), // already ends on a word
            (at(0x10), at(0x16)), // the halfword belongs to a cut
            (at(0x1a), at(0x26)), // starts on a halfword
            (at(0x2c), at(0x32)), // a declared unit member keeps its extent
            (at(0x34), at(0x3a)), // the zero halfword runs into the limit
        ];
        let cuts = [(at(0x16), at(0x18))];
        let declared = [(at(0x2c), at(0x32))];
        assert_eq!(
            alignment_halfwords(&ranges, &cuts, &declared, &bytes, at(0x3b)),
            [true, false, false, false, false, false]
        );
        bytes[0x06] = 1;
        assert!(!alignment_halfwords(&ranges[..1], &[], &[], &bytes, at(0x40))[0]);
    }

    #[test]
    fn placeholder_listing_keeps_every_undeclared_byte_in_place() {
        let decoded = [0x70, 0x47, 0x00, 0x00, 0x00, 0xb5];
        let hole = Hole {
            start: OVERLAY_BASE,
            end: OVERLAY_BASE + 2,
            kind: HoleKind::Code,
        };
        let placed = [0x00, 0x00, 0x00, 0x00, 0x00, 0xb5];
        assert!(holes_replace_only_declared_bytes(&placed, &decoded, &[hole]).is_ok());
        // A four-byte row cut as a two-byte owner loses its alignment halfword.
        let shifted = [0x00, 0x00, 0x00, 0xb5];
        assert!(holes_replace_only_declared_bytes(&shifted, &decoded, &[hole]).is_err());
        let unfilled = [0x70, 0x47, 0x00, 0x00, 0x00, 0xb5];
        assert!(holes_replace_only_declared_bytes(&unfilled, &decoded, &[hole]).is_err());
    }

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

    #[test]
    fn uncovered_owners_get_a_listing_to_the_next_boundary() {
        let owners = std::collections::BTreeSet::from([0x100, 0x180, 0x200, 0x280]);
        assert_eq!(
            super::owner_listings(vec![(0x100, 0x140)], &owners, &[(0x1f0, 0x220)], 0x300),
            [(0x100, 0x140), (0x180, 0x1f0), (0x280, 0x300)]
        );
    }

    #[test]
    fn every_unowned_byte_joins_a_listing() {
        // Listings at 0x100 and 0x140 (retained at its declared extent), C at
        // 0x180..0x1c0; holes after each and before the first.
        let listings = super::complete_listings(
            vec![(0x100, 0x13e), (0x140, 0x170)],
            &[(0x140, 0x170)],
            &[(0x140, 0x170), (0x180, 0x1c0)],
            0xf0,
            0x200,
        );
        assert_eq!(
            listings,
            [
                (0xf0, 0x100, false),
                (0x100, 0x140, true),
                (0x140, 0x170, true),
                (0x170, 0x180, false),
                (0x1c0, 0x200, false)
            ]
        );
    }

    #[test]
    fn unreached_holes_are_written_without_an_entry() {
        let alignment = super::hole_source(&[0, 0], 0x0800300a).unwrap();
        assert!(!alignment.contains("Overlay_") && !alignment.contains("thumb_func"));
        assert!(alignment.contains(".2byte 0x0000"));
        assert!(super::thumb_prologue(&[0x70, 0xb5]) && !super::thumb_prologue(&[0, 0]));
    }

    #[test]
    fn registered_owners_start_their_own_listings() {
        let owners = std::collections::BTreeSet::from([0x100, 0x130, 0x200, 0x300]);
        assert_eq!(
            super::split_at_owners(
                vec![(0x100, 0x180), (0x1f0, 0x240), (0x300, 0x310)],
                &owners
            ),
            [
                (0x100, 0x130),
                (0x130, 0x180),
                (0x1f0, 0x200),
                (0x200, 0x240),
                (0x300, 0x310)
            ]
        );
    }

    #[test]
    fn main_listings_take_the_registered_owner_name() {
        let paths = SourcePaths::parse(
            std::path::Path::new("."),
            r#"{"format":3,"owners":{"main:0800383c":{"name":"QueueIoWriteDelay1"}}}"#,
        )
        .unwrap();
        let region = ".syntax unified\n\t.thumb\n\t.set sub_08000010, 0x08000010\n\t.global Overlay_0800383c\nOverlay_0800383c:\n\tbx\tlr\n";
        assert_eq!(
            owner_entry(region, 0x0800383c, &paths),
            ".syntax unified\n\t.thumb\n\t.set sub_08000010, 0x08000010\n\t.global QueueIoWriteDelay1\n\t.global Func_0800383c\n\t.thumb_func\nQueueIoWriteDelay1:\nFunc_0800383c:\n\tbx\tlr\n"
        );
        let literal = ".syntax unified\n\t.thumb\n\t.global Func_08000100\n\t.thumb_func\nFunc_08000100:\n\tbx\tlr\n";
        assert_eq!(owner_entry(literal, 0x08000100, &paths), literal);
    }
}
