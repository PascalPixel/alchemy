//! `alchemy drafts`: compile every C draft under `recon/<game>` and score
//! each function it defines against that function's listing, as pret compiled
//! its NONMATCHING C beside the assembly it replaced, so a draft's recorded
//! difference never goes stale. The scores print to stdout; nothing is stored.
use super::parse::definitions;
use super::{Config, Problem};
use crate::compiler::routing::root;
use crate::targets::{decomp_target, TARGET_IDS};
use std::path::{Path, PathBuf};

const USAGE: &str = "usage: alchemy drafts [DRAFT.c...]\n\
Compile each C draft under recon/tbs and recon/tla (or the drafts named)\n\
for its language folder, or Japanese when no language folder is specified,\n\
with its routed compiler, and score every function it defines against the\n\
function's listing with the permuter's scores, where 0 is exact. Prints one\n\
line per function, best first: score, differing instructions, function and\n\
draft; then the drafts that could not be scored and why. Nothing is written.";

pub fn run(arguments: &[String]) -> Result<(), String> {
    if arguments
        .iter()
        .any(|argument| argument == "--help" || argument == "-h")
    {
        println!("{USAGE}");
        return Ok(());
    }
    let root = root();
    let drafts = if arguments.is_empty() {
        all_drafts(&root)?
    } else {
        arguments.iter().map(PathBuf::from).collect()
    };
    let scratch = tempfile::Builder::new()
        .prefix("alchemy-drafts-")
        .tempdir()
        .map_err(|error| error.to_string())?;
    let mut scored = Vec::new();
    let mut unscored = Vec::new();
    let mut labels = std::collections::HashMap::new();
    let mut unlisted = Vec::new();
    for (index, draft) in drafts.iter().enumerate() {
        let shown = draft
            .strip_prefix(&root)
            .unwrap_or(draft)
            .display()
            .to_string();
        let Some(id) = game_of(draft) else {
            unscored.push(format!("{shown}: not under recon/tbs or recon/tla"));
            continue;
        };
        let target = decomp_target(Some(id))?;
        let names = std::fs::read_to_string(draft)
            .map_err(|error| error.to_string())
            .and_then(|source| definitions(&source));
        let names = match names {
            Ok(names) if !names.is_empty() => names,
            Ok(_) => {
                unscored.push(format!("{shown}: defines no function"));
                continue;
            }
            Err(error) => {
                unscored.push(format!("{shown}: {}", first_line(&error)));
                continue;
            }
        };
        let game = root.join("recon").join(&id[..3]);
        let labels = labels
            .entry(id)
            .or_insert_with(|| listing_labels(&game.join("raw")));
        // A draft named for its address sits beside a listing whose first
        // label is that function's, often still a placeholder.
        let stem = draft.file_stem().unwrap_or_default().to_string_lossy();
        let beside = game.join("raw").join(format!("{stem}.s"));
        let unlabelled: Vec<&String> = names
            .iter()
            .filter(|(name, inline)| !inline && !labels.contains_key(name))
            .map(|(name, _)| name)
            .collect();
        let placeholder = match unlabelled.as_slice() {
            [_] => first_label(&beside),
            _ => None,
        };
        let mut missing = Vec::new();
        for (name, inline) in names {
            // Inline helpers have no code of their own to score.
            if inline {
                continue;
            }
            let (listing, symbol) = match (labels.get(&name), &placeholder) {
                (Some(listing), _) => (listing.clone(), None),
                (None, Some(label)) => (beside.clone(), Some(label.clone())),
                (None, None) => {
                    missing.push(name);
                    continue;
                }
            };
            let listing = Some(listing);
            let config = Config {
                draft: draft.clone(),
                function: Some(name.clone()),
                listing,
                symbol,
                target,
                route: None,
                elf: None,
                focus: None,
            };
            let directory = scratch.path().join(format!("{index}-{name}"));
            std::fs::create_dir_all(&directory).map_err(|error| error.to_string())?;
            let result = Problem::load(&config, &directory)
                .and_then(|problem| problem.evaluate(&problem.draft, &directory.join("setup")));
            match result {
                Ok(score) => scored.push((score.total, score.lines.len(), name, shown.clone())),
                Err(error) => unscored.push(format!("{shown}: {name}: {}", first_line(&error))),
            }
        }
        if !missing.is_empty() {
            unlisted.push(format!("{shown}: {}", missing.join(", ")));
        }
    }
    scored.sort();
    for (total, differing, name, shown) in &scored {
        println!("{total}\t{differing}\t{name}\t{shown}");
    }
    let exact = scored.iter().filter(|row| row.0 == 0).count();
    println!(
        "drafts: {} functions scored ({exact} exact), {} could not be scored; {} drafts define functions no listing labels",
        scored.len(),
        unscored.len(),
        unlisted.len()
    );
    for line in &unscored {
        println!("unscored: {line}");
    }
    for line in &unlisted {
        println!("unlisted: {line}");
    }
    Ok(())
}

/// The build target a draft is scored for, from its game and language folders.
/// Drafts with no explicit language use that game's Japanese source edition.
fn game_of(draft: &Path) -> Option<&'static str> {
    let components: Vec<_> = draft.components().collect();
    let folder = components.windows(3).find(|parts| {
        parts[0].as_os_str() == "recon"
            && matches!(parts[1].as_os_str().to_str(), Some("tbs" | "tla"))
    })?;
    let game = folder[1].as_os_str().to_str()?;
    let language = folder[2].as_os_str().to_str()?;
    TARGET_IDS
        .into_iter()
        .map(|id| id.as_str())
        .find(|id| id.split_once('-') == Some((game, language)))
        .or(match game {
            "tbs" => Some("tbs-ja"),
            "tla" => Some("tla-ja"),
            _ => None,
        })
}

/// Every C draft under `recon/<game>`, except the permuter's written
/// candidates and the `units` wrappers that only include a draft.
fn all_drafts(root: &Path) -> Result<Vec<PathBuf>, String> {
    let mut drafts = Vec::new();
    for game in ["recon/tbs", "recon/tla"] {
        let directory = root.join(game);
        if !directory.is_dir() {
            continue;
        }
        for entry in walkdir::WalkDir::new(&directory) {
            let entry = entry.map_err(|error| error.to_string())?;
            let path = entry.path();
            let name = path.file_name().unwrap_or_default().to_string_lossy();
            let wrapper = path.strip_prefix(&directory).is_ok_and(|relative| {
                relative
                    .components()
                    .any(|part| part.as_os_str() == "units")
            });
            if entry.file_type().is_file()
                && name.ends_with(".c")
                && !name.ends_with(".permute.c")
                && !wrapper
            {
                drafts.push(path.to_path_buf());
            }
        }
    }
    drafts.sort();
    Ok(drafts)
}

/// Every label a game's listings define, with the listing that defines it.
fn listing_labels(raw: &Path) -> std::collections::HashMap<String, PathBuf> {
    let label = regex::Regex::new(r"(?m)^([A-Za-z_][A-Za-z0-9_]*):").expect("static pattern");
    let mut found = std::collections::HashMap::new();
    let mut listings: Vec<_> = walkdir::WalkDir::new(raw)
        .into_iter()
        .flatten()
        .filter(|entry| {
            entry.file_type().is_file()
                && entry
                    .path()
                    .extension()
                    .is_some_and(|extension| extension == "s")
                && !entry.file_name().to_string_lossy().starts_with("draft_")
        })
        .map(|entry| entry.into_path())
        .collect();
    listings.sort();
    for path in listings {
        if let Ok(text) = std::fs::read_to_string(&path) {
            for capture in label.captures_iter(&text) {
                found
                    .entry(capture[1].to_string())
                    .or_insert_with(|| path.clone());
            }
        }
    }
    found
}

/// The first label a listing defines.
fn first_label(listing: &Path) -> Option<String> {
    let text = std::fs::read_to_string(listing).ok()?;
    let label = regex::Regex::new(r"(?m)^([A-Za-z_][A-Za-z0-9_]*):").expect("static pattern");
    label.captures(&text).map(|capture| capture[1].to_string())
}

fn first_line(text: &str) -> &str {
    text.lines().next().unwrap_or(text)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn drafts_are_every_game_c_file_but_permuter_output_and_unit_wrappers() {
        let temp = tempfile::tempdir().unwrap();
        let root = temp.path();
        for path in [
            "recon/tbs/en/main/08003e58.c",
            "recon/tbs/en/main/08003e58.permute.c",
            "recon/tbs/en/overlays/resource_372.c",
            "recon/tbs/en/units/0801.c",
            "recon/tbs/en/party_active_owners.c",
            "recon/tla/en/main/08026278.c",
            "recon/tla/en/main/notes.h",
            "recon/tbs/ja/main/japanese.c",
            "recon/tla/fr/main/french.c",
            "recon/tla/fr/units/include.c",
            "recon/tla/raw/shared.c",
        ] {
            std::fs::create_dir_all(root.join(path).parent().unwrap()).unwrap();
            std::fs::write(root.join(path), "").unwrap();
        }
        let drafts: Vec<String> = all_drafts(root)
            .unwrap()
            .iter()
            .map(|path| path.strip_prefix(root).unwrap().display().to_string())
            .collect();
        assert_eq!(
            drafts,
            [
                "recon/tbs/en/main/08003e58.c",
                "recon/tbs/en/overlays/resource_372.c",
                "recon/tbs/en/party_active_owners.c",
                "recon/tbs/ja/main/japanese.c",
                "recon/tla/en/main/08026278.c",
                "recon/tla/fr/main/french.c",
                "recon/tla/raw/shared.c",
            ]
        );
        assert_eq!(
            game_of(Path::new("recon/tla/en/main/08026278.c")),
            Some("tla-en")
        );
        assert_eq!(game_of(Path::new("games/X.C")), None);
    }

    #[test]
    fn language_folders_preserve_every_explicit_target() {
        for id in TARGET_IDS {
            let (game, language) = id.as_str().split_once('-').unwrap();
            let path = PathBuf::from(format!("recon/{game}/{language}/main/draft.c"));
            assert_eq!(game_of(&path), Some(id.as_str()));
            assert_eq!(
                game_of(&Path::new("/work/alchemy").join(path)),
                Some(id.as_str())
            );
        }
        assert_eq!(game_of(Path::new("recon/tbs/raw/draft.c")), Some("tbs-ja"));
        assert_eq!(game_of(Path::new("recon/tla/draft.c")), Some("tla-ja"));
        assert_eq!(game_of(Path::new("other-recon/tbs/en/draft.c")), None);
        assert_eq!(game_of(Path::new("recon/tbs-other/en/draft.c")), None);
    }

    #[test]
    fn discovery_includes_all_twelve_language_folders() {
        let temp = tempfile::tempdir().unwrap();
        for id in TARGET_IDS {
            let (game, language) = id.as_str().split_once('-').unwrap();
            let path = temp
                .path()
                .join(format!("recon/{game}/{language}/main/draft.c"));
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, "").unwrap();
        }
        let drafts = all_drafts(temp.path()).unwrap();
        assert_eq!(drafts.len(), TARGET_IDS.len());
        let selected: std::collections::HashSet<_> =
            drafts.iter().filter_map(|path| game_of(path)).collect();
        assert_eq!(
            selected,
            TARGET_IDS.into_iter().map(|id| id.as_str()).collect()
        );
    }

    #[test]
    fn listing_ownership_skips_preserved_attempts_and_has_stable_order() {
        let temp = tempfile::tempdir().unwrap();
        for (path, text) in [
            ("draft_attempt.s", "OnlyAttempt:\nShared:\n"),
            ("z_listing.s", "Shared:\n"),
            ("a_listing.s", "Shared:\nMaintained:\n"),
        ] {
            std::fs::write(temp.path().join(path), text).unwrap();
        }
        let labels = listing_labels(temp.path());
        assert!(!labels.contains_key("OnlyAttempt"));
        assert_eq!(labels["Shared"], temp.path().join("a_listing.s"));
        assert_eq!(labels["Maintained"], temp.path().join("a_listing.s"));
    }

    #[test]
    fn inline_helpers_are_told_apart_from_the_functions_a_draft_scores() {
        let source = "static inline int Call1(int (*f)(int), int a) { return f(a); }\nint Scene_Run(int a) { return Call1(0, a); }\n";
        assert_eq!(
            definitions(source).unwrap(),
            [
                ("Call1".to_string(), true),
                ("Scene_Run".to_string(), false)
            ]
        );
    }
}
