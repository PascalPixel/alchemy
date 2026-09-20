//! The Lost Age half of the exact-source contract. Every owner the TLA register
//! names with a source, directly or through a declared unit, scores byte-exact
//! against the TLA ROM over its audited
//! extent, and every shared source under `games/COMMON/SRC` is one of them, so
//! a shared file cannot drift from the second game unnoticed.
//!
//! A main-image owner's extent is declared by its unit, inside the executable
//! inventory. An overlay owner's extent is its reviewed span in
//! `games/THE LOST AGE/semantic/regions.json`; its retained listing must hold
//! an `AlchemyC_` placeholder of exactly that span, and the listing assembled
//! with every placeholder compiled must reproduce the overlay the ROM loads.
use crate::compiler::overlay::placeholder_extent;
use crate::compiler::routing::CompilerTarget;
use crate::compiler::source_paths::{SourceOwner, SourcePaths, SHARED_SOURCE_ROOT};
use crate::compiler::translation_units::TranslationUnits;
use crate::overlay::assembly::OVERLAY_BASE;
use crate::overlay::compile::assemble_overlay;
use crate::overlay::owners::{production_target, register_path, reviewed_spans};
use crate::overlay::rom::CanonicalRom;
use crate::overlay::source::OverlaySource;
use serde_json::Value;
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};
use std::process::ExitCode;

const USAGE: &str = "usage: alchemy check tla-owners ROM";
/// The TLA executable inventory containing the declared main-owner extents.
const INVENTORY: &str = "games/THE LOST AGE/metrics/executable.json";

pub(super) fn entry(arguments: &[String]) -> ExitCode {
    let [rom] = arguments else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    if matches!(rom.as_str(), "-h" | "--help") {
        println!("{USAGE}");
        return ExitCode::SUCCESS;
    }
    let root = crate::compiler::routing::root();
    super::report(check(root, Path::new(rom)).map(|summary| println!("{summary}")))
}

/// One exact TLA owner: its register identity, source and audited extent.
#[derive(Debug, PartialEq)]
struct ScoredOwner {
    owner: SourceOwner,
    source: String,
    extent: usize,
}

/// Complete audited main-image executable ranges. Function boundaries belong
/// to translation-unit manifests; denominator intervals may contain many
/// owners and must never be mistaken for owner extents.
fn inventory_ranges(text: &str) -> Result<Vec<(u32, u32)>, String> {
    let inventory: Value =
        serde_json::from_str(text).map_err(|error| format!("{INVENTORY}: {error}"))?;
    if inventory.pointer("/main/audit").and_then(Value::as_str) != Some("complete") {
        return Err(format!("{INVENTORY}: main executable audit is incomplete"));
    }
    let intervals = inventory
        .pointer("/main/intervals")
        .and_then(Value::as_array)
        .ok_or_else(|| format!("{INVENTORY} has no main intervals"))?;
    let mut ranges = Vec::new();
    for interval in intervals {
        let bound = |key: &str| {
            interval
                .get(key)
                .and_then(Value::as_u64)
                .ok_or_else(|| format!("{INVENTORY}: an interval lacks {key}"))
        };
        let (start, end) = (bound("start")?, bound("end")?);
        if end <= start {
            return Err(format!("{INVENTORY}: empty interval at 0x{start:08x}"));
        }
        let start = u32::try_from(start).map_err(|_| format!("{INVENTORY}: start overflows"))?;
        let end = u32::try_from(end).map_err(|_| format!("{INVENTORY}: end overflows"))?;
        ranges.push((start, end));
    }
    ranges.sort_unstable();
    Ok(ranges)
}

/// Every sourced owner, including named unit members: a main owner's complete
/// declared extent or an overlay owner's reviewed span. An owner
/// without one, or a shared source no owner compiles, fails.
fn scored_owners(
    root: &Path,
    register: &SourcePaths,
    unit_extents: &BTreeMap<SourceOwner, (usize, PathBuf)>,
    executable: &[(u32, u32)],
    reviewed: &BTreeMap<SourceOwner, usize>,
    shared: &BTreeSet<PathBuf>,
) -> Result<Vec<ScoredOwner>, String> {
    let mut owners = Vec::new();
    let mut unscored = shared.clone();
    for owner in register.registered_owners() {
        let Some(path) = register.mapped_source_path(owner).or_else(|| {
            unit_extents
                .get(&owner)
                .map(|(_, source)| root.join(source))
        }) else {
            continue;
        };
        let extent = if owner.is_main() {
            let extent = unit_extents
                .get(&owner)
                .map(|(extent, _)| *extent)
                .ok_or_else(|| format!("{}: no translation-unit owner extent", owner.id()))?;
            let end = u32::try_from(extent)
                .ok()
                .and_then(|extent| owner.address().checked_add(extent))
                .ok_or_else(|| format!("{}: invalid translation-unit owner extent", owner.id()))?;
            if !executable
                .iter()
                .any(|&(start, bound)| start <= owner.address() && end <= bound)
            {
                return Err(format!(
                    "{}: translation-unit extent lies outside the audited main executable ranges in {INVENTORY}",
                    owner.id()
                ));
            }
            extent
        } else {
            *reviewed.get(&owner).ok_or_else(|| {
                format!(
                    "{}: no reviewed span in {}",
                    owner.id(),
                    register_path(Path::new(""), production_target(CompilerTarget::Tla)).display()
                )
            })?
        };
        let relative = path.strip_prefix(root).unwrap_or(&path).to_path_buf();
        unscored.remove(&relative);
        owners.push(ScoredOwner {
            owner,
            source: relative.to_string_lossy().into_owned(),
            extent,
        });
    }
    if let Some(path) = unscored.first() {
        return Err(format!(
            "shared source has no exact TLA owner: {}",
            path.display()
        ));
    }
    Ok(owners)
}

/// Repository-relative paths of every shared C source.
fn shared_sources(root: &Path) -> Result<BTreeSet<PathBuf>, String> {
    let mut sources = BTreeSet::new();
    let mut pending = vec![root.join(SHARED_SOURCE_ROOT)];
    while let Some(directory) = pending.pop() {
        let Ok(entries) = std::fs::read_dir(&directory) else {
            continue;
        };
        for entry in entries {
            let path = entry.map_err(|error| error.to_string())?.path();
            if path.is_dir() {
                pending.push(path);
            } else if path.extension().is_some_and(|extension| extension == "C") {
                sources.insert(path.strip_prefix(root).unwrap_or(&path).to_path_buf());
            }
        }
    }
    Ok(sources)
}

/// Differing halfwords between two equal-length windows.
fn differing_halfwords(built: &[u8], reference: &[u8]) -> usize {
    built
        .chunks(2)
        .zip(reference.chunks(2))
        .filter(|(left, right)| left != right)
        .count()
}

/// Each overlay's C owners, checked from the one listing that places them:
/// every owner needs a placeholder of its reviewed span, and the assembled
/// overlay must equal the ROM's inside each owner and as a whole.
fn overlay_mismatches(
    root: &Path,
    rom: &CanonicalRom,
    owners: &[&ScoredOwner],
    assembly_images: &[String],
) -> Result<Vec<String>, String> {
    let target = production_target(CompilerTarget::Tla);
    let mut by_overlay = BTreeMap::<String, Vec<&ScoredOwner>>::new();
    for scored in owners {
        let overlay = scored.owner.overlay_id().expect("overlay owner");
        by_overlay.entry(overlay).or_default().push(scored);
    }
    for image in assembly_images {
        by_overlay.entry(image.clone()).or_default();
    }
    let mut mismatches = Vec::new();
    for (overlay, members) in by_overlay {
        let listing = root.join(target.overlay_assembly(&overlay));
        let text = std::fs::read_to_string(&listing)
            .map_err(|error| format!("{}: {error}", listing.display()))?;
        for scored in &members {
            let placed = placeholder_extent(&text, scored.owner.address());
            if placed != Some(scored.extent) {
                return Err(format!(
                    "{} {} has no AlchemyC placeholder of its {}-byte reviewed span",
                    scored.owner.id(),
                    scored.source,
                    scored.extent
                ));
            }
        }
        let built = assemble_overlay(&OverlaySource::path(&listing), OVERLAY_BASE)?;
        let reference = rom.overlay(&overlay)?;
        if built.len() != reference.len() {
            mismatches.push(format!(
                "{overlay} assembles to {} bytes, the ROM loads {}",
                built.len(),
                reference.len()
            ));
            continue;
        }
        let before = mismatches.len();
        for scored in &members {
            let start = (i64::from(scored.owner.address()) - OVERLAY_BASE) as usize;
            let end = start + scored.extent;
            let differing = differing_halfwords(&built[start..end], &reference[start..end]);
            if differing != 0 {
                mismatches.push(format!(
                    "{} {} differs in {differing} halfwords",
                    scored.owner.id(),
                    scored.source
                ));
            }
        }
        if mismatches.len() == before && built != reference {
            mismatches.push(format!(
                "{overlay} differs from the ROM outside its C owners"
            ));
        }
    }
    Ok(mismatches)
}

fn check(root: &Path, rom: &Path) -> Result<String, String> {
    let progress_inputs = crate::coverage::proof::identity(root, "tla-en")?;
    if !rom.is_file() {
        return Err(format!("{}: TLA ROM not found", rom.display()));
    }
    let register = SourcePaths::load_for_game(root, "tla")?;
    let units = TranslationUnits::load_game(root, CompilerTarget::Tla)?;
    let inventory = std::fs::read_to_string(root.join(INVENTORY))
        .map_err(|error| format!("{INVENTORY}: {error}"))?;
    let shared = shared_sources(root)?;
    let reviewed = reviewed_spans(root, production_target(CompilerTarget::Tla))?;
    let unit_extents = register
        .registered_owners()
        .filter(|owner| owner.is_main())
        .filter_map(|owner| {
            let unit = units.unit_for_game_owner("tla", owner)?;
            let extent = unit
                .owners
                .iter()
                .find(|member| {
                    member.address == owner.address()
                        && member.state == crate::compiler::translation_units::OwnerState::ExactC
                })?
                .extent;
            Some((owner, (extent, unit.source.clone())))
        })
        .collect::<BTreeMap<_, _>>();
    let owners = scored_owners(
        root,
        &register,
        &unit_extents,
        &inventory_ranges(&inventory)?,
        &reviewed,
        &shared,
    )?;
    let (main, overlays): (Vec<&ScoredOwner>, Vec<&ScoredOwner>) =
        owners.iter().partition(|scored| scored.owner.is_main());
    let mut mismatches = Vec::new();
    let tree = crate::coverage::tree::work_tree_at(root.to_path_buf());
    let assembly = crate::coverage::pipeline::overlay_assembly_to_verify(
        &tree,
        &production_target(CompilerTarget::Tla),
    )?;
    let assembly_images = assembly.keys().cloned().collect::<Vec<_>>();
    if !overlays.is_empty() || !assembly_images.is_empty() {
        let canonical = CanonicalRom::from_file(rom, production_target(CompilerTarget::Tla))?;
        mismatches.extend(overlay_mismatches(
            root,
            &canonical,
            &overlays,
            &assembly_images,
        )?);
    }
    let mut compiled_units = BTreeMap::<String, String>::new();
    for scored in main {
        let mut options = crate::score::cli::Options::tbs(scored.source.clone());
        options.target = CompilerTarget::Tla;
        options.rom = Some(rom.to_string_lossy().into_owned());
        options.owner = Some(scored.owner.address());
        options.size = Some(scored.extent);
        options.work = Some(format!("out/tla-en/owners/{}", scored.owner.address_stem()));
        if let Some(unit) = units.unit_for_game_owner("tla", scored.owner) {
            options.configuration.absolute_symbols = unit.canonical_symbols()?;
            options.unit = Some(unit.id.clone());
            options.work = Some(format!("out/tla-en/owners/{}", unit.id));
            options.precompiled_object = compiled_units.get(&unit.id).cloned();
            if options.precompiled_object.is_none()
                && unit.owners[0].address != scored.owner.address()
            {
                return Err(format!(
                    "{}: first declared member is not being verified",
                    unit.id
                ));
            }
        }
        let rendered = crate::score::render::render(root, &options)?;
        if let Some(unit) = units.unit_for_game_owner("tla", scored.owner) {
            if !compiled_units.contains_key(&unit.id) {
                let object = root
                    .join(options.work.as_ref().unwrap())
                    .join(format!("{}.o", scored.owner.address_stem()));
                let layout = crate::score::validate_layout(unit, &object, scored.owner.address())?;
                if !layout.is_empty() {
                    return Err(format!(
                        "{}: unit layout differs: {}",
                        unit.id,
                        layout.join(", ")
                    ));
                }
                compiled_units.insert(unit.id.clone(), object.to_string_lossy().into_owned());
            }
        }
        if crate::score::exact_mismatch(&rendered) {
            mismatches.push(format!(
                "{} {} differs in {} halfwords",
                scored.owner.id(),
                scored.source,
                rendered.differing_halfwords
            ));
        }
    }
    if !mismatches.is_empty() {
        return Err(format!(
            "TLA owners are not byte-exact:\n{}",
            mismatches.join("\n")
        ));
    }
    let mut credits: Vec<_> = owners
        .iter()
        .map(|owner| crate::coverage::proof::Credit {
            image: owner.owner.overlay_id().unwrap_or_else(|| "main".into()),
            start: i64::from(owner.owner.address()),
            end: i64::from(owner.owner.address()) + owner.extent as i64,
            source: owner.source.clone(),
            kind: "c".into(),
        })
        .collect();
    let target = production_target(CompilerTarget::Tla);
    for (image, spans) in assembly {
        for span in spans {
            credits.push(crate::coverage::proof::Credit {
                source: target.overlay_assembly(&image),
                image: image.clone(),
                start: span.start,
                end: span.end,
                kind: "assembly".into(),
            });
        }
    }
    crate::coverage::proof::write(
        root,
        "tla-en",
        &std::fs::read(rom).map_err(|e| e.to_string())?,
        &progress_inputs,
        credits,
    )?;
    Ok(format!(
        "tla owners ok: {} exact owners ({} overlay), {} shared sources",
        owners.len(),
        overlays.len(),
        shared.len()
    ))
}

#[cfg(test)]
mod tests {
    use super::*;

    const REGISTER: &str = r#"{"format":3,"owners":{"main:081c2a3c":{"name":"Channel_Mute","source":"../../COMMON/SRC/SOUND/CHANNEL_MUTE.C"},"main:08001000":{"name":"Named_Only"}}}"#;

    fn fixture(register: &str, shared: &[&str]) -> tempfile::TempDir {
        let mut files = vec![("games/THE LOST AGE/source-paths.json", register, true)];
        files.extend(shared.iter().map(|path| (*path, "void f(void) {}\n", true)));
        super::super::fixture_repository(&files)
    }

    #[test]
    fn inventory_intervals_give_audited_ranges() {
        let ranges = inventory_ranges(
            r#"{"main":{"audit":"complete","intervals":[{"start":136063548,"end":136063628},{"start":136065856,"end":136065928}]}}"#,
        )
        .unwrap();
        assert_eq!(
            ranges,
            [(0x081c_2a3c, 0x081c_2a8c), (0x081c_3340, 0x081c_3388)]
        );
        assert!(inventory_ranges(
            r#"{"main":{"audit":"complete","intervals":[{"start":8,"end":8}]}}"#
        )
        .is_err());
        assert!(inventory_ranges(r#"{"main":{"audit":"incomplete","intervals":[]}}"#).is_err());
    }

    #[test]
    fn every_sourced_owner_needs_an_extent_and_every_shared_source_an_owner() {
        let root = fixture(REGISTER, &["games/COMMON/SRC/SOUND/CHANNEL_MUTE.C"]);
        let register = SourcePaths::load_for_game(root.path(), "tla").unwrap();
        let shared = shared_sources(root.path()).unwrap();
        let owner = SourceOwner::Main(0x081c_2a3c);
        let extents = BTreeMap::from([(
            owner,
            (80, PathBuf::from("games/COMMON/SRC/SOUND/CHANNEL_MUTE.C")),
        )]);
        let ranges = [(0x081c_0000, 0x081c_4000)];
        let owners = scored_owners(
            root.path(),
            &register,
            &extents,
            &ranges,
            &BTreeMap::new(),
            &shared,
        )
        .unwrap();
        assert_eq!(
            owners,
            [ScoredOwner {
                owner: SourceOwner::Main(0x081c_2a3c),
                source: "games/COMMON/SRC/SOUND/CHANNEL_MUTE.C".into(),
                extent: 80,
            }]
        );
        let missing = scored_owners(
            root.path(),
            &register,
            &BTreeMap::new(),
            &ranges,
            &BTreeMap::new(),
            &shared,
        );
        assert!(missing
            .unwrap_err()
            .contains("main:081c2a3c: no translation-unit owner extent"));

        let orphan = fixture(
            REGISTER,
            &[
                "games/COMMON/SRC/SOUND/CHANNEL_MUTE.C",
                "games/COMMON/SRC/SOUND/ORPHAN.C",
            ],
        );
        let register = SourcePaths::load_for_game(orphan.path(), "tla").unwrap();
        let shared = shared_sources(orphan.path()).unwrap();
        let error = scored_owners(
            orphan.path(),
            &register,
            &extents,
            &ranges,
            &BTreeMap::new(),
            &shared,
        )
        .unwrap_err();
        assert!(error.contains("SOUND/ORPHAN.C"), "{error}");
    }

    #[test]
    fn named_unit_members_without_repeated_source_paths_are_checked() {
        let source = "games/COMMON/SRC/SOUND/CHANNEL_MUTE.C";
        let register = r#"{"format":3,"owners":{
            "main:081c2a3c":{"name":"Cgb_ChannelMute","source":"../../COMMON/SRC/SOUND/CHANNEL_MUTE.C"},
            "main:081c2a8c":{"name":"Cgb_ChannelReset"}
        }}"#;
        let root = fixture(register, &[source]);
        let register = SourcePaths::load_for_game(root.path(), "tla").unwrap();
        let members = BTreeMap::from([
            (SourceOwner::Main(0x081c2a3c), (80, PathBuf::from(source))),
            (SourceOwner::Main(0x081c2a8c), (12, PathBuf::from(source))),
        ]);
        let owners = scored_owners(
            root.path(),
            &register,
            &members,
            &[(0x081c0000, 0x081c4000)],
            &BTreeMap::new(),
            &BTreeSet::from([PathBuf::from(source)]),
        )
        .unwrap();
        assert_eq!(owners.len(), 2);
        assert_eq!(owners[1].owner, SourceOwner::Main(0x081c2a8c));
        assert_eq!(owners[1].source, source);
        assert_eq!(owners[1].extent, 12);
    }

    #[test]
    fn overlay_owners_take_their_reviewed_span() {
        let register = r#"{"format":3,"owners":{"resource_650:02000038":{"name":"Scene_GetEntrances","source":"FIELD/IDEJIMA/ISLAND.C"}}}"#;
        let root = fixture(register, &[]);
        let register = SourcePaths::load_for_game(root.path(), "tla").unwrap();
        let owner = SourceOwner::parse("resource_650:02000038").unwrap();
        let unreviewed = scored_owners(
            root.path(),
            &register,
            &BTreeMap::new(),
            &[],
            &BTreeMap::new(),
            &BTreeSet::new(),
        )
        .unwrap_err();
        assert!(
            unreviewed.contains("resource_650:02000038: no reviewed span"),
            "{unreviewed}"
        );
        let owners = scored_owners(
            root.path(),
            &register,
            &BTreeMap::new(),
            &[],
            &BTreeMap::from([(owner, 8)]),
            &BTreeSet::new(),
        )
        .unwrap();
        assert_eq!(
            owners,
            [ScoredOwner {
                owner,
                source: "games/THE LOST AGE/SRC/FIELD/IDEJIMA/ISLAND.C".into(),
                extent: 8,
            }]
        );
    }

    #[test]
    fn assembly_only_overlays_cannot_escape_the_complete_image_check() {
        let root = crate::compiler::routing::root();
        let path = root.join("roms/tla-en.gba");
        if !path.is_file() {
            return;
        }
        crate::compiler::routing::prefer_installed_binutils();
        let target = production_target(CompilerTarget::Tla);
        let rom = CanonicalRom::from_file(&path, target).unwrap();
        let image = "resource_653";
        let reference = rom.overlay(image).unwrap();
        let temp = tempfile::tempdir().unwrap();
        let listing = temp.path().join(target.overlay_assembly(image));
        std::fs::create_dir_all(listing.parent().unwrap()).unwrap();
        std::fs::write(
            &listing,
            format!(".syntax unified\n.thumb\n.space {}\n", reference.len()),
        )
        .unwrap();
        let mismatches = overlay_mismatches(temp.path(), &rom, &[], &[image.into()]).unwrap();
        assert_eq!(
            mismatches,
            ["resource_653 differs from the ROM outside its C owners"]
        );
        std::fs::remove_file(listing).unwrap();
        assert!(overlay_mismatches(temp.path(), &rom, &[], &[image.into()]).is_err());
    }

    #[test]
    fn halfword_differences_are_counted_per_pair() {
        assert_eq!(differing_halfwords(&[1, 2, 3, 4], &[1, 2, 3, 4]), 0);
        assert_eq!(differing_halfwords(&[1, 2, 3, 4], &[1, 9, 3, 4]), 1);
        assert_eq!(differing_halfwords(&[1, 2, 3, 4], &[0, 0, 0, 0]), 2);
    }

    #[test]
    fn a_missing_rom_is_refused_before_scoring() {
        let root = fixture(REGISTER, &[]);
        let error = check(root.path(), &root.path().join("roms/tla-en.gba")).unwrap_err();
        assert!(error.contains("TLA ROM not found"));
    }

    /// With the local ROM present, the tracked register and shared sources pass.
    #[test]
    fn tracked_tla_owners_score_exact_against_the_local_rom() {
        let root = crate::compiler::routing::root();
        let rom = root.join("roms/tla-en.gba");
        if !rom.is_file() {
            return;
        }
        crate::compiler::routing::prefer_installed_binutils();
        let summary = check(root, &rom).unwrap();
        assert!(summary.starts_with("tla owners ok: "), "{summary}");
    }
}
