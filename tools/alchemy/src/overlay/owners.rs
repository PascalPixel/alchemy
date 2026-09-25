//! Which game an overlay belongs to, and that game's reviewed owner register.
//!
//! Each game keeps its own `semantic/regions.json`: the complete overlay
//! owners a reviewer has bounded. Adoption, parking, and compilation resolve
//! the register, the retained assembly, the ROM, and the compiler from one
//! production target, so a Lost Age owner never reads Broken Seal evidence.

use crate::compiler::routing::CompilerTarget;
use crate::compiler::source_paths::SourceOwner;
use crate::targets::{target_for, DecompTarget, DecompTargetId, DEFAULT_TARGET};
use serde::Deserialize;
use std::collections::BTreeMap;
use std::path::{Path, PathBuf};

#[derive(Deserialize)]
struct Register {
    manual_regions: Vec<Region>,
}
#[derive(Deserialize)]
struct Region {
    overlay: String,
    entry: String,
    span_bytes: usize,
}

/// The edition whose ROM a game's retained overlay assembly reproduces: the
/// one its `recon/<game>/assets.json` addresses. Other editions are compiled
/// against it, never adopted from it.
pub fn production_target(game: CompilerTarget) -> DecompTarget {
    target_for(match game {
        CompilerTarget::Tbs => DEFAULT_TARGET,
        CompilerTarget::Tla => DecompTargetId::TlaEn,
    })
}

/// The production target whose retained overlay directory holds `path`. A
/// listing outside every game's directory (an in-memory or scratch listing)
/// belongs to the default target, as it always has.
pub fn assembly_target(path: &Path) -> DecompTarget {
    [CompilerTarget::Tbs, CompilerTarget::Tla]
        .into_iter()
        .map(production_target)
        .find(|target| {
            let directory = target.overlay_dir();
            path.to_string_lossy().contains(&format!("{directory}/"))
        })
        .unwrap_or_else(|| target_for(DEFAULT_TARGET))
}

/// The overlay a listing file name holds: `resource_3c8_overlay.s` holds
/// `resource_3c8`.
pub fn listing_overlay(file_name: &str) -> Option<&str> {
    file_name
        .strip_suffix("_overlay.s")
        .filter(|overlay| overlay.starts_with("resource_"))
}

/// The not-yet-C owners of the target's game: each reviewed owner whose
/// listing has no `AlchemyC_` placeholder yet, with a `Func_` name and its
/// reviewed extent.
pub fn listed_owners(
    root: &Path,
    target: DecompTarget,
) -> Result<BTreeMap<SourceOwner, (String, usize)>, String> {
    let mut placeholders: BTreeMap<String, BTreeMap<u32, usize>> = BTreeMap::new();
    let mut owners = BTreeMap::new();
    for (owner, extent) in reviewed_spans(root, target)? {
        let overlay = owner
            .overlay_id()
            .ok_or_else(|| format!("{} is not an overlay owner", owner.id()))?;
        if !placeholders.contains_key(&overlay) {
            let path = root.join(target.overlay_assembly(&overlay));
            let text = std::fs::read_to_string(&path)
                .map_err(|error| format!("{}: {error}", path.display()))?;
            placeholders.insert(
                overlay.clone(),
                crate::compiler::overlay::placeholder_extents(&text),
            );
        }
        if placeholders[&overlay].contains_key(&owner.address()) {
            continue;
        }
        owners.insert(owner, (format!("Func_{:08x}", owner.address()), extent));
    }
    Ok(owners)
}

/// `recon/<game>/semantic/regions.json`.
pub fn register_path(root: &Path, target: DecompTarget) -> PathBuf {
    root.join(target.recon_dir()).join("semantic/regions.json")
}

/// Every reviewed complete owner of the target's game with its extent.
pub fn reviewed_spans(
    root: &Path,
    target: DecompTarget,
) -> Result<BTreeMap<SourceOwner, usize>, String> {
    let path = register_path(root, target);
    let register: Register = crate::compiler::build_io::read_json(&path)?;
    let mut spans = BTreeMap::new();
    for region in register.manual_regions {
        let owner = SourceOwner::parse(&format!(
            "{}:{}",
            region.overlay,
            region.entry.trim_start_matches("0x")
        ))?;
        if region.span_bytes == 0 {
            return Err(format!(
                "{} has no positive reviewed span_bytes",
                owner.id()
            ));
        }
        if spans.insert(owner, region.span_bytes).is_some() {
            return Err(format!(
                "{} has duplicate reviewed owner entries",
                owner.id()
            ));
        }
    }
    Ok(spans)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::compiler::routing::root;

    #[test]
    fn each_game_resolves_its_own_production_paths() {
        let tbs = production_target(CompilerTarget::Tbs);
        let tla = production_target(CompilerTarget::Tla);
        assert_eq!(tbs.id, DecompTargetId::TbsEn);
        assert_eq!(tla.id, DecompTargetId::TlaEn);
        assert_eq!(
            register_path(Path::new("r"), tla),
            Path::new("r/recon/tla/semantic/regions.json")
        );
        let repository = Path::new("/repo");
        assert_eq!(
            assembly_target(&repository.join(tla.overlay_assembly("resource_64e"))).id,
            DecompTargetId::TlaEn
        );
        assert_eq!(
            assembly_target(&repository.join(tbs.overlay_assembly("resource_36f"))).id,
            DecompTargetId::TbsEn
        );
        assert_eq!(
            assembly_target(Path::new("/tmp/o.s")).id,
            DecompTargetId::TbsEn
        );
    }

    #[test]
    fn broken_seal_register_reads_as_the_compiler_reads_it() {
        assert_eq!(
            reviewed_spans(root(), production_target(CompilerTarget::Tbs)).unwrap(),
            crate::compiler::translation_units::reviewed_overlay_spans(root()).unwrap()
        );
    }

    #[test]
    fn listed_owners_are_the_reviewed_owners_without_placeholders() {
        let work = tempfile::tempdir().unwrap();
        let target = production_target(CompilerTarget::Tla);
        let path = register_path(work.path(), target);
        std::fs::create_dir_all(path.parent().unwrap()).unwrap();
        std::fs::write(
            &path,
            r#"{"format":1,"manual_regions":[
                {"overlay":"resource_64e","entry":"0x02000080","span_bytes":36},
                {"overlay":"resource_64e","entry":"0x020000a4","span_bytes":16}]}"#,
        )
        .unwrap();
        let listing = work.path().join(target.overlay_assembly("resource_64e"));
        std::fs::create_dir_all(listing.parent().unwrap()).unwrap();
        std::fs::write(
            &listing,
            "\t.space 0x80\n\t.space 0x24\nAlchemyC_020000a4:\n\t.space 0x10\n",
        )
        .unwrap();
        let owners = listed_owners(work.path(), target).unwrap();
        let owner = SourceOwner::parse("resource_64e:02000080").unwrap();
        assert_eq!(owners.len(), 1);
        assert_eq!(owners[&owner], ("Func_02000080".to_string(), 36));
        assert_eq!(
            listing_overlay("resource_64e_overlay.s"),
            Some("resource_64e")
        );
        assert_eq!(listing_overlay("resource_64e.s"), None);
    }

    #[test]
    fn lost_age_register_is_present_and_well_formed() {
        reviewed_spans(root(), production_target(CompilerTarget::Tla)).unwrap();
    }

    #[test]
    fn register_rejects_duplicates_and_empty_extents() {
        let work = tempfile::tempdir().unwrap();
        let target = production_target(CompilerTarget::Tla);
        let path = register_path(work.path(), target);
        std::fs::create_dir_all(path.parent().unwrap()).unwrap();
        let row = r#"{"overlay":"resource_64e","entry":"0x02000080","span_bytes":36}"#;
        std::fs::write(
            &path,
            format!(r#"{{"format":1,"manual_regions":[{row},{row}]}}"#),
        )
        .unwrap();
        assert!(reviewed_spans(work.path(), target)
            .unwrap_err()
            .contains("duplicate"));
        std::fs::write(
            &path,
            r#"{"format":1,"manual_regions":[{"overlay":"resource_64e","entry":"0x02000080","span_bytes":0}]}"#,
        )
        .unwrap();
        assert!(reviewed_spans(work.path(), target)
            .unwrap_err()
            .contains("positive"));
    }
}
