//! Which game an overlay belongs to, and that game's overlay owners.
//!
//! Each game's retained overlay listings bound its owners: an `AlchemyC_`
//! placeholder bounds an exact C owner and a listing label bounds a
//! not-yet-C one (see [`crate::overlay::listing`]). Adoption, parking, and
//! compilation resolve the listings, the ROM, and the compiler from one
//! production target, so a Lost Age owner never reads Broken Seal evidence.

use crate::compiler::routing::CompilerTarget;
use crate::compiler::source_paths::SourceOwner;
use crate::overlay::listing::{self, Listing};
use crate::targets::{target_for, DecompTarget, DecompTargetId, DEFAULT_TARGET};
use std::collections::BTreeMap;
use std::path::Path;

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

/// Every retained overlay listing of the target's game, laid out, by overlay.
/// A game with no retained listings has none.
pub fn listings(root: &Path, target: DecompTarget) -> Result<BTreeMap<String, Listing>, String> {
    let directory = root.join(target.overlay_dir());
    let Ok(entries) = std::fs::read_dir(&directory) else {
        return Ok(BTreeMap::new());
    };
    let veneer_macro = target.overlay_macro();
    let mut listings = BTreeMap::new();
    for entry in entries {
        let path = entry.map_err(|error| error.to_string())?.path();
        let Some(overlay) = path
            .file_name()
            .and_then(|name| name.to_str())
            .and_then(listing_overlay)
        else {
            continue;
        };
        listings.insert(
            overlay.to_string(),
            listing::read(root, &path, &veneer_macro)?,
        );
    }
    Ok(listings)
}

/// Every overlay owner of the target's game with its extent: exact C owners
/// from their placeholders and not-yet-C owners from their listing labels.
pub fn owner_spans(
    root: &Path,
    target: DecompTarget,
) -> Result<BTreeMap<SourceOwner, usize>, String> {
    let mut spans = BTreeMap::new();
    for (overlay, listing) in listings(root, target)? {
        let owners = listing
            .placeholders()
            .chain(listing.owners().map(|(_, entry, extent)| (entry, extent)));
        for (entry, extent) in owners {
            let owner = SourceOwner::parse(&format!("{overlay}:{entry:08x}"))?;
            if spans.insert(owner, extent).is_some() {
                return Err(format!("{} is bounded twice in its listing", owner.id()));
            }
        }
    }
    Ok(spans)
}

/// The not-yet-C owners of the target's game: each owner's listing name and
/// complete extent.
pub fn listed_owners(
    root: &Path,
    target: DecompTarget,
) -> Result<BTreeMap<SourceOwner, (String, usize)>, String> {
    let mut owners = BTreeMap::new();
    for (overlay, listing) in listings(root, target)? {
        for (name, entry, extent) in listing.owners() {
            let owner = SourceOwner::parse(&format!("{overlay}:{entry:08x}"))?;
            owners.insert(owner, (name.to_string(), extent));
        }
    }
    Ok(owners)
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
        assert_eq!(
            listing_overlay("resource_36f_overlay.s"),
            Some("resource_36f")
        );
        assert_eq!(listing_overlay("resource_36f.plan.json"), None);
    }

    #[test]
    fn both_games_lay_out_every_listing() {
        for game in [CompilerTarget::Tbs, CompilerTarget::Tla] {
            let target = production_target(game);
            let spans = owner_spans(root(), target).unwrap();
            let listed = listed_owners(root(), target).unwrap();
            assert!(!spans.is_empty() && !listed.is_empty(), "{}", target.id);
            assert!(listed
                .iter()
                .all(|(owner, (_, extent))| spans.get(owner) == Some(extent)));
        }
    }

    #[test]
    fn a_listing_label_bounds_a_not_yet_c_owner() {
        let work = tempfile::tempdir().unwrap();
        let target = production_target(CompilerTarget::Tla);
        let path = work.path().join(target.overlay_assembly("resource_64e"));
        std::fs::create_dir_all(path.parent().unwrap()).unwrap();
        std::fs::write(
            &path,
            "AlchemyC_02000000:\n\t.space 0x4\nField_Run:\n\tpush\t{lr}\n\tbl\tsub_02000100\n\tpop\t{r0}\n",
        )
        .unwrap();
        let spans = owner_spans(work.path(), target).unwrap();
        assert_eq!(
            spans,
            BTreeMap::from([
                (SourceOwner::parse("resource_64e:02000000").unwrap(), 4),
                (SourceOwner::parse("resource_64e:02000004").unwrap(), 8),
            ])
        );
    }
}
