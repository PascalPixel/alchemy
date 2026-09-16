//! The Lost Age half of the exact-source contract. Every owner the TLA register
//! names with a source scores byte-exact against the TLA ROM over its audited
//! extent, and every shared source under `games/COMMON/SRC` is one of them, so
//! a shared file cannot drift from the second game unnoticed.
use crate::compiler::routing::CompilerTarget;
use crate::compiler::source_paths::{SourceOwner, SourcePaths, SHARED_SOURCE_ROOT};
use serde_json::Value;
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};
use std::process::ExitCode;

const USAGE: &str = "usage: alchemy check tla-owners ROM";
/// The TLA executable inventory whose main intervals audit each owner's extent.
const INVENTORY: &str = "games/THE LOST AGE/metrics/tla-en-executable.json";

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

/// Main-image interval extents keyed by their first address.
fn inventory_extents(text: &str) -> Result<BTreeMap<u32, usize>, String> {
    let inventory: Value =
        serde_json::from_str(text).map_err(|error| format!("{INVENTORY}: {error}"))?;
    let intervals = inventory
        .pointer("/main/intervals")
        .and_then(Value::as_array)
        .ok_or_else(|| format!("{INVENTORY} has no main intervals"))?;
    let mut extents = BTreeMap::new();
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
        extents.insert(start, (end - u64::from(start)) as usize);
    }
    Ok(extents)
}

/// Every owner the register gives a source, with its audited extent. An owner
/// without an audited interval, or a shared source no owner compiles, fails.
fn scored_owners(
    root: &Path,
    register: &SourcePaths,
    extents: &BTreeMap<u32, usize>,
    shared: &BTreeSet<PathBuf>,
) -> Result<Vec<ScoredOwner>, String> {
    let mut owners = Vec::new();
    let mut unscored = shared.clone();
    for owner in register.registered_owners() {
        let Some(path) = register.mapped_source_path(owner) else {
            continue;
        };
        if !owner.is_main() {
            return Err(format!(
                "{}: TLA overlay owners are not scored yet",
                owner.id()
            ));
        }
        let extent = *extents
            .get(&owner.address())
            .ok_or_else(|| format!("{}: no audited extent in {INVENTORY}", owner.id()))?;
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

fn check(root: &Path, rom: &Path) -> Result<String, String> {
    if !rom.is_file() {
        return Err(format!("{}: TLA ROM not found", rom.display()));
    }
    let register = SourcePaths::load_for_game(root, "tla")?;
    let inventory = std::fs::read_to_string(root.join(INVENTORY))
        .map_err(|error| format!("{INVENTORY}: {error}"))?;
    let shared = shared_sources(root)?;
    let owners = scored_owners(root, &register, &inventory_extents(&inventory)?, &shared)?;
    let mut mismatches = Vec::new();
    for scored in &owners {
        let mut options = crate::score::cli::Options::tbs(scored.source.clone());
        options.target = CompilerTarget::Tla;
        options.rom = Some(rom.to_string_lossy().into_owned());
        options.owner = Some(scored.owner.address());
        options.size = Some(scored.extent);
        options.work = Some(format!("out/tla-en/owners/{}", scored.owner.address_stem()));
        let rendered = crate::score::render::render(root, &options)?;
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
    Ok(format!(
        "tla owners ok: {} exact owners, {} shared sources",
        owners.len(),
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
    fn inventory_intervals_give_audited_extents() {
        let extents = inventory_extents(
            r#"{"main":{"intervals":[{"start":136063548,"end":136063628},{"start":136065856,"end":136065928}]}}"#,
        )
        .unwrap();
        assert_eq!(extents.get(&0x081c_2a3c), Some(&80));
        assert_eq!(extents.get(&0x081c_3340), Some(&72));
        assert!(inventory_extents(r#"{"main":{"intervals":[{"start":8,"end":8}]}}"#).is_err());
        assert!(inventory_extents(r#"{"overlays":[]}"#).is_err());
    }

    #[test]
    fn every_sourced_owner_needs_an_extent_and_every_shared_source_an_owner() {
        let root = fixture(REGISTER, &["games/COMMON/SRC/SOUND/CHANNEL_MUTE.C"]);
        let register = SourcePaths::load_for_game(root.path(), "tla").unwrap();
        let shared = shared_sources(root.path()).unwrap();
        let extents = BTreeMap::from([(0x081c_2a3c, 80)]);
        let owners = scored_owners(root.path(), &register, &extents, &shared).unwrap();
        assert_eq!(
            owners,
            [ScoredOwner {
                owner: SourceOwner::Main(0x081c_2a3c),
                source: "games/COMMON/SRC/SOUND/CHANNEL_MUTE.C".into(),
                extent: 80,
            }]
        );
        let missing = scored_owners(root.path(), &register, &BTreeMap::new(), &shared);
        assert!(missing
            .unwrap_err()
            .contains("main:081c2a3c: no audited extent"));

        let orphan = fixture(
            REGISTER,
            &[
                "games/COMMON/SRC/SOUND/CHANNEL_MUTE.C",
                "games/COMMON/SRC/SOUND/ORPHAN.C",
            ],
        );
        let register = SourcePaths::load_for_game(orphan.path(), "tla").unwrap();
        let shared = shared_sources(orphan.path()).unwrap();
        let error = scored_owners(orphan.path(), &register, &extents, &shared).unwrap_err();
        assert!(error.contains("SOUND/ORPHAN.C"), "{error}");
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
