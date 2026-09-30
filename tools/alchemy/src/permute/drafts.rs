//! `alchemy drafts`: compile every C draft under `recon/<game>/en` and score
//! each function it defines against that function's listing, as pret compiled
//! its NONMATCHING C beside the assembly it replaced, so a draft's recorded
//! difference never goes stale. The scores print to stdout; nothing is stored.
use super::parse::definitions;
use super::{Config, Problem};
use crate::compiler::routing::root;
use crate::targets::decomp_target;
use std::path::{Path, PathBuf};

const USAGE: &str = "usage: alchemy drafts [DRAFT.c...]\n\
Compile each C draft under recon/tbs/en and recon/tla/en (or the drafts named)\n\
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

/// The build target a draft is scored for, from the game folder it sits in.
fn game_of(draft: &Path) -> Option<&'static str> {
    let text = draft.to_string_lossy();
    if text.contains("recon/tbs/") {
        Some("tbs-en")
    } else if text.contains("recon/tla/") {
        Some("tla-en")
    } else {
        None
    }
}

/// Every C draft under `recon/<game>/en`, except the permuter's written
/// candidates and the `units` wrappers that only include a draft.
fn all_drafts(root: &Path) -> Result<Vec<PathBuf>, String> {
    let mut drafts = Vec::new();
    for game in ["recon/tbs/en", "recon/tla/en"] {
        let directory = root.join(game);
        if !directory.is_dir() {
            continue;
        }
        for entry in walkdir::WalkDir::new(&directory) {
            let entry = entry.map_err(|error| error.to_string())?;
            let path = entry.path();
            let name = path.file_name().unwrap_or_default().to_string_lossy();
            let wrapper = path
                .strip_prefix(&directory)
                .is_ok_and(|relative| relative.starts_with("units"));
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
    for entry in walkdir::WalkDir::new(raw).into_iter().flatten() {
        let path = entry.path();
        if path.extension().is_some_and(|extension| extension == "s") {
            if let Ok(text) = std::fs::read_to_string(path) {
                for capture in label.captures_iter(&text) {
                    found
                        .entry(capture[1].to_string())
                        .or_insert_with(|| path.to_path_buf());
                }
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
                "recon/tla/en/main/08026278.c",
            ]
        );
        assert_eq!(
            game_of(Path::new("recon/tla/en/main/08026278.c")),
            Some("tla-en")
        );
        assert_eq!(game_of(Path::new("games/X.C")), None);
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
