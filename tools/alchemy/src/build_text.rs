//! The message archive's build rule. Each game's `TEXT/MESSAGES.S` includes
//! `text/messages.inc`, which `build rom` writes from the edition's PO catalog
//! as pret's mapjson writes the assembly its data files include: the Huffman
//! archive, whose address words name its own labels, and an absolute symbol
//! `Msg<Name>` for each message the code names, set to that message's number
//! in this edition. The linker places the archive and fills every literal
//! pool that loads a message number.
use crate::targets::{DecompTarget, TARGET_IDS};
use ags::text::{self, ARCHIVES};
use std::collections::BTreeSet;
use std::fs;
use std::path::Path;

/// The generated include, under the build directory the assembler searches.
pub(crate) const INCLUDE: &str = "text/messages.inc";

/// The archive's labels: `Text_MessageModels`, `Text_MessageContexts` (read
/// by the symbol decoder) and `Text_MessageBanks` (read by the lookup).
const LABEL: &str = "Text_Message";

/// Write `output/text/messages.inc` for `target`, leaving an unchanged file
/// untouched.
pub(crate) fn build(root: &Path, target: DecompTarget, output: &Path) -> Result<(), String> {
    let spec = ARCHIVES
        .iter()
        .find(|spec| spec.target == target.id.as_str())
        .ok_or_else(|| format!("{} has no message catalog", target.id))?;
    let source = text::read_source(&root.join(spec.output))?;
    if source.target != spec.target {
        return Err(format!("{} is the {} catalog", spec.output, source.target));
    }
    let assembly = text::messages_include(&source, LABEL, spec.output)?;
    let path = output.join(INCLUDE);
    if fs::read_to_string(&path).ok().as_deref() == Some(assembly.as_str()) {
        return Ok(());
    }
    fs::create_dir_all(path.parent().expect("include directory"))
        .map_err(|error| error.to_string())?;
    fs::write(&path, assembly).map_err(|error| format!("{}: {error}", path.display()))
}

/// Every `Msg<Name>` a game's source uses is the `msgctxt` of exactly one
/// entry in each of its editions' catalogs, and no linker script under
/// `games/` assigns one a number by hand.
pub(crate) fn check_names(root: &Path) -> Result<(), String> {
    let mut problems = Vec::new();
    let mut games = BTreeSet::new();
    for id in TARGET_IDS {
        games.insert(crate::targets::target_for(id).game_dir());
    }
    for game in games {
        let mut used = BTreeSet::new();
        for directory in [game, "games/COMMON"] {
            used_names(&root.join(directory), &mut used, &mut problems)?;
        }
        for spec in ARCHIVES
            .iter()
            .filter(|spec| Path::new(spec.output).starts_with(game))
        {
            let names = text::catalog_names(&text::read_catalog(&root.join(spec.output))?)
                .map_err(|error| format!("{}: {error}", spec.output))?;
            let named: BTreeSet<&str> = names.iter().map(|(name, _)| name.as_str()).collect();
            for name in used.iter().filter(|name| !named.contains(name.as_str())) {
                problems.push(format!("{}: no message is named {name}", spec.output));
            }
        }
    }
    if problems.is_empty() {
        Ok(())
    } else {
        Err(problems.join("\n"))
    }
}

/// Collect the message names in `directory`'s C, headers and assembly, and
/// refuse any linker script that sets one.
fn used_names(
    directory: &Path,
    used: &mut BTreeSet<String>,
    problems: &mut Vec<String>,
) -> Result<(), String> {
    let name = regex::Regex::new(r"\bMsg[A-Z][A-Za-z0-9]*\b").expect("static pattern");
    let assignment =
        regex::Regex::new(r"(?m)^\s*(Msg[A-Z][A-Za-z0-9]*)\s*=").expect("static pattern");
    for entry in walkdir::WalkDir::new(directory) {
        let entry = entry.map_err(|error| error.to_string())?;
        let path = entry.path();
        let extension = path
            .extension()
            .and_then(|extension| extension.to_str())
            .unwrap_or_default()
            .to_ascii_lowercase();
        if !entry.file_type().is_file()
            || !matches!(extension.as_str(), "c" | "h" | "s" | "inc" | "ld")
        {
            continue;
        }
        let source =
            fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
        if extension == "ld" {
            for capture in assignment.captures_iter(&source) {
                problems.push(format!(
                    "{}: {} is numbered by hand; name it in the catalogs",
                    path.display(),
                    &capture[1]
                ));
            }
            continue;
        }
        used.extend(
            name.find_iter(&source)
                .map(|found| found.as_str().to_owned())
                .filter(|found| text::message_name(found)),
        );
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn every_message_name_the_code_uses_is_named_once_in_each_catalog() {
        check_names(crate::compiler::routing::root()).unwrap();
    }

    #[test]
    fn names_are_collected_from_source_and_refused_in_linker_scripts() {
        let directory = std::env::temp_dir().join(format!("alchemy-names-{}", std::process::id()));
        let _ = fs::remove_dir_all(&directory);
        fs::create_dir_all(directory.join("SRC")).unwrap();
        fs::write(
            directory.join("SRC/SHOW.C"),
            "extern char MsgHpRecover;\nvoid Msg_Show(void) { Show(&MsgHpRecover, MsgX_y); }\n",
        )
        .unwrap();
        fs::write(
            directory.join("MAIN.LD"),
            "MsgHpFull = 0x820;\nValue_1 = 1;\n",
        )
        .unwrap();
        let (mut used, mut problems) = (BTreeSet::new(), Vec::new());
        used_names(&directory, &mut used, &mut problems).unwrap();
        fs::remove_dir_all(&directory).unwrap();
        assert_eq!(used.into_iter().collect::<Vec<_>>(), ["MsgHpRecover"]);
        assert_eq!(problems.len(), 1);
        assert!(problems[0].contains("MsgHpFull is numbered by hand"));
    }
}

#[cfg(test)]
mod archive_layout_tests {
    use ags::text::ARCHIVES;

    #[test]
    fn layouts_cover_each_registered_target_once() {
        let mut ids = ARCHIVES.iter().map(|spec| spec.target).collect::<Vec<_>>();
        ids.sort_unstable();
        ids.dedup();
        assert_eq!(ARCHIVES.len(), ids.len());
        assert_eq!(ids.len(), crate::targets::TARGET_IDS.len());
        for id in crate::targets::TARGET_IDS {
            let target = crate::targets::target_for(id);
            let spec = ARCHIVES
                .iter()
                .find(|spec| spec.target == id.as_str())
                .unwrap();
            assert_eq!(
                spec.output,
                format!(
                    "{}/TEXT/{}.PO",
                    target.game_dir(),
                    spec.language.to_uppercase()
                )
            );
        }
    }
}
