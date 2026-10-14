//! Flag and sound ids still written as numbers at the positions `alchemy
//! camelotize constants` names, counted per game. A game with a names
//! header (HEADERS: ☀️) has a ratchet against tools/alchemy/constants.tsv:
//! a rise fails, to be named from the header, or recorded with --update
//! where the number has no proven name, which stays a number (S3). A game
//! with none (⚓️) is only reported: its adoptions bring literal ids that
//! nothing names yet.
//!
//! A game's count covers its own C, headers and assembler and the COMMON
//! source both games build; Nintendo's library files are not counted. A
//! literal counts whether it is the id or holds it beside other bits; an
//! expression that computes an id does not.

use crate::camelotize::constants::{sites, Called, Kind, Names, Site};
use crate::compiler::routing::{family_for_source, CompilerFamily, CompilerTarget};
use std::collections::BTreeMap;
use std::fs;
use std::path::Path;
use std::process::ExitCode;

pub(crate) const BASELINE: &str = "tools/alchemy/constants.tsv";

const USAGE: &str = "usage: alchemy check constants [--update | --list [PATH...]]\n\
Counts, for each game, the flag and sound ids written as integer literals\n\
at the positions `alchemy camelotize constants` names. For a game with a\n\
names header (☀️) it fails when a count rises above\n\
tools/alchemy/constants.tsv; a game with none (⚓️) is only reported. A\n\
number with no proven name stays a number (S3): name a new literal from\n\
the header where it has a name, and otherwise record it with --update.\n\
--update writes the current counts of the games with a header there;\n\
lower them whenever ids are named. --list prints each literal id left in\n\
the PATHs (files or folders; all of games/ by default), with the name its\n\
game's header gives it, then each file's count.";

/// The headers whose names a game's ids take, by kind, once they exist
/// (they replace flag_ids.h and sound_ids.h). Only a game listed here has
/// a ratchet.
const HEADERS: [(&str, Kind, &str); 2] =
    [("tbs", Kind::Flag, "games/tbs/include/flags.h"), ("tbs", Kind::Sound, "games/tbs/include/songs.h")];

/// Whether a rise in a game's counts fails the check: it has a names header.
fn ratcheted(game: &str) -> bool {
    HEADERS.iter().any(|(listed, _, _)| *listed == game)
}

const GAMES: [(&str, &str, CompilerTarget); 2] =
    [("tbs", "games/tbs/", CompilerTarget::Tbs), ("tla", "games/tla/", CompilerTarget::Tla)];

type Counts = BTreeMap<(String, String), usize>;

/// A literal id left in a file, and the games that build the file.
struct Left {
    path: String,
    games: Vec<&'static str>,
    written: String,
    site: Site,
}

pub fn entry(arguments: &[String]) -> ExitCode {
    match run(arguments) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}

fn run(arguments: &[String]) -> Result<(), String> {
    let root = crate::compiler::routing::root();
    match arguments.first().map(String::as_str) {
        None => check(root),
        Some("--update") if arguments.len() == 1 => {
            let counts = count(&scan(root)?);
            let kept: Counts = counts.iter().filter(|((game, _), _)| ratcheted(game)).map(|(k, v)| (k.clone(), *v)).collect();
            fs::write(root.join(BASELINE), render(&kept)).map_err(|e| format!("{BASELINE}: {e}"))?;
            print!("{}", render(&counts));
            Ok(())
        }
        Some("--list") => list(root, &arguments[1..]),
        Some("-h" | "--help") => {
            println!("{USAGE}");
            Ok(())
        }
        _ => Err(USAGE.into()),
    }
}

fn check(root: &Path) -> Result<(), String> {
    let left = scan(root)?;
    let counts = count(&left);
    let baseline = parse(&fs::read_to_string(root.join(BASELINE)).map_err(|e| format!("{BASELINE}: {e}"))?)?;
    let failures = ratchet(&baseline, &counts);
    if !failures.is_empty() {
        return Err(format!(
            "{}\nname each new literal id its header names (alchemy camelotize constants); a number with no proven \
             name stays a number (S3): record it with alchemy check constants --update",
            failures.join("\n")
        ));
    }
    let lower: Vec<String> = counts
        .iter()
        .filter(|(key, value)| ratcheted(&key.0) && baseline.get(*key).is_some_and(|before| **value < *before))
        .map(|((game, kind), value)| format!("{game} {kind} fell to {value}"))
        .collect();
    if !lower.is_empty() {
        eprintln!("{}; run alchemy check constants --update to lock it in", lower.join(", "));
    }
    let reported: Vec<String> = counts
        .iter()
        .filter(|((game, _), _)| !ratcheted(game))
        .map(|((game, kind), value)| format!("{game} {kind} {value}"))
        .collect();
    if !reported.is_empty() {
        eprintln!("{} ids written as numbers, reported only: no names header", reported.join(", "));
    }
    let names = headers(root);
    let named = left.iter().filter(|left| name(&names, left).is_some()).count();
    if named > 0 {
        eprintln!("{named} of the literal ids have a name in their game's header: alchemy camelotize constants");
    }
    Ok(())
}

/// Each game's names, by kind, from the headers that exist.
fn headers(root: &Path) -> BTreeMap<(&'static str, Kind), Names> {
    let mut names: BTreeMap<(&'static str, Kind), Names> = BTreeMap::new();
    for (game, kind, header) in HEADERS {
        if let Ok(text) = fs::read_to_string(root.join(header)) {
            names.entry((game, kind)).or_default().extend(Names::parse(&text));
        }
    }
    names
}

/// The one name the games that build a file give a literal's id.
fn name(names: &BTreeMap<(&'static str, Kind), Names>, left: &Left) -> Option<String> {
    let id = left.site.id()?;
    let found: Vec<&str> = left
        .games
        .iter()
        .filter_map(|game| match names.get(&(*game, left.site.position.kind))?.called(id) {
            Called::One(name) => Some(name),
            _ => None,
        })
        .collect();
    found.first().filter(|first| found.iter().all(|name| name == *first)).map(|name| name.to_string())
}

fn list(root: &Path, paths: &[String]) -> Result<(), String> {
    let names = headers(root);
    let wanted: Vec<String> = paths
        .iter()
        .map(|path| {
            let absolute = std::path::absolute(path).map_err(|e| format!("{path}: {e}"))?;
            Ok(absolute.strip_prefix(root).unwrap_or(&absolute).to_string_lossy().trim_end_matches('/').to_string())
        })
        .collect::<Result<_, String>>()?;
    let mut files: BTreeMap<(String, Kind), usize> = BTreeMap::new();
    for left in scan(root)? {
        if !wanted.is_empty() && !wanted.iter().any(|w| left.path == *w || left.path.starts_with(&format!("{w}/"))) {
            continue;
        }
        let kind = left.site.position.kind;
        println!(
            "{}:{}\t{}\t{}\t{}\t{}",
            left.path,
            left.site.line,
            kind.name(),
            left.written,
            left.site.position.place.label(),
            name(&names, &left).unwrap_or_default()
        );
        *files.entry((left.path.clone(), kind)).or_default() += 1;
    }
    for ((path, kind), count) in files {
        println!("{count}\t{}\t{path}", kind.name());
    }
    Ok(())
}

/// Every literal id left in the source of either game.
fn scan(root: &Path) -> Result<Vec<Left>, String> {
    let mut left = Vec::new();
    for entry in walkdir::WalkDir::new(root.join("games")).sort_by_file_name() {
        let entry = entry.map_err(|e| e.to_string())?;
        let path = entry.path().strip_prefix(root).unwrap_or(entry.path()).to_string_lossy().into_owned();
        if !entry.file_type().is_file() || !matches!(path.rsplit_once('.').map(|(_, e)| e), Some("c" | "h" | "s" | "S")) {
            continue;
        }
        let games: Vec<&'static str> = GAMES
            .iter()
            .filter(|(_, folder, target)| {
                (path.starts_with(folder) || path.starts_with("games/common/"))
                    && !(path.ends_with(".c")
                        && matches!(family_for_source(*target, &path), CompilerFamily::Agbcc | CompilerFamily::AgbccFlash))
            })
            .map(|(game, _, _)| *game)
            .collect();
        if games.is_empty() {
            continue;
        }
        let bytes = fs::read(entry.path()).map_err(|e| format!("{path}: {e}"))?;
        let text = String::from_utf8_lossy(&bytes);
        for site in sites(Path::new(&path), &text, &Kind::ALL) {
            if site.literal() {
                let written = text[site.span.clone()].split_whitespace().collect::<Vec<_>>().join(" ");
                left.push(Left { path: path.clone(), games: games.clone(), written, site });
            }
        }
    }
    Ok(left)
}

fn count(left: &[Left]) -> Counts {
    let mut counts = Counts::new();
    for (game, _, _) in GAMES {
        for kind in Kind::ALL {
            counts.insert((game.to_string(), kind.name().to_string()), 0);
        }
    }
    for left in left {
        for game in &left.games {
            *counts.entry((game.to_string(), left.site.position.kind.name().to_string())).or_default() += 1;
        }
    }
    counts
}

fn render(counts: &Counts) -> String {
    let mut text = String::from("game\tkind\tcount\n");
    for ((game, kind), value) in counts {
        text.push_str(&format!("{game}\t{kind}\t{value}\n"));
    }
    text
}

fn parse(text: &str) -> Result<Counts, String> {
    let mut counts = Counts::new();
    for (number, row) in text.lines().enumerate().skip(1) {
        let fields: Vec<&str> = row.split('\t').collect();
        let [game, kind, value] = fields[..] else {
            return Err(format!("{BASELINE}:{}: three fields expected", number + 1));
        };
        let value = value.parse().map_err(|_| format!("{BASELINE}:{}: bad count", number + 1))?;
        counts.insert((game.into(), kind.into()), value);
    }
    Ok(counts)
}

/// The counts above their baseline, of the games with a names header.
fn ratchet(baseline: &Counts, counts: &Counts) -> Vec<String> {
    counts
        .iter()
        .filter(|((game, _), _)| ratcheted(game))
        .filter_map(|(key, value)| {
            let allowed = baseline.get(key).copied().unwrap_or(0);
            (*value > allowed).then(|| format!("{} {} ids written as numbers: {value}, baseline {allowed}", key.0, key.1))
        })
        .collect()
}

#[cfg(test)]
mod tests {
    use super::*;

    fn tree(files: &[(&str, &str)]) -> tempfile::TempDir {
        let root = tempfile::tempdir().unwrap();
        for (path, text) in files {
            let file = root.path().join(path);
            fs::create_dir_all(file.parent().unwrap()).unwrap();
            fs::write(file, text).unwrap();
        }
        root
    }

    #[test]
    fn each_game_counts_its_own_and_the_common_literals() {
        let root = tree(&[
            ("games/tbs/src/scene/a.c", "void F(void)\n{\n\tGameFlag_Set(0x909);\n\tGameFlag_Set(FLAG_X);\n\tGameFlag_Set(181 << 1);\n\tSE(74);\n\tSE(a + 90);\n}\n"),
            ("games/tbs/src/object/RULES.S", "\tgroup_cue 2, -1, 0x16a, 74\n\tgroup_cue 2, -1, -1, 42\n"),
            ("games/tla/src/b.c", "void G(void)\n{\n\tEngine_GameFlagClear(0x100);\n}\n"),
            ("games/common/src/c.c", "void H(void)\n{\n\tAudioCommand_PlayFar(113);\n}\n"),
            ("games/tbs/src/notes.txt", "GameFlag_Set(0x909);\n"),
        ]);
        let counts = count(&scan(root.path()).unwrap());
        let get = |game: &str, kind: &str| counts[&(game.to_string(), kind.to_string())];
        // A literal, plain or packed, and an expression of literals count; a
        // name and a computed id do not.
        assert_eq!((get("tbs", "flag"), get("tbs", "sound")), (3, 4));
        assert_eq!((get("tla", "flag"), get("tla", "sound")), (1, 1));
        assert_eq!(render(&counts), "game\tkind\tcount\ntbs\tflag\t3\ntbs\tsound\t4\ntla\tflag\t1\ntla\tsound\t1\n");
        assert_eq!(parse(&render(&counts)).unwrap(), counts);
    }

    #[test]
    fn the_ratchet_refuses_a_rise_only() {
        let at = |flag: usize, sound: usize| -> Counts {
            [(("tbs".to_string(), "flag".to_string()), flag), (("tbs".to_string(), "sound".to_string()), sound)].into()
        };
        assert!(ratchet(&at(10, 5), &at(10, 5)).is_empty());
        assert!(ratchet(&at(10, 5), &at(3, 0)).is_empty());
        assert_eq!(ratchet(&at(10, 5), &at(11, 5)), ["tbs flag ids written as numbers: 11, baseline 10"]);
        // A count the baseline has no row for may only be zero.
        assert_eq!(ratchet(&Counts::new(), &at(0, 1)), ["tbs sound ids written as numbers: 1, baseline 0"]);
        // A game with no names header is never refused.
        let tla: Counts = [(("tla".to_string(), "flag".to_string()), 500)].into();
        assert!(ratchet(&Counts::new(), &tla).is_empty());
        assert!(parse("game\tkind\tcount\ntbs\tflag\n").is_err());
    }

    #[test]
    fn a_new_literal_fails_only_the_game_with_a_names_header() {
        let baseline = "game\tkind\tcount\ntbs\tflag\t0\ntbs\tsound\t0\n";
        let tla = ("games/tla/src/b.c", "void G(void)\n{\n\tGameFlag_Test(0x123);\n}\n");
        let root = tree(&[(BASELINE, baseline), tla]);
        check(root.path()).unwrap();
        let root = tree(&[(BASELINE, baseline), tla, ("games/tbs/src/a.c", "void F(void)\n{\n\tGameFlag_Test(0x123);\n}\n")]);
        let error = check(root.path()).unwrap_err();
        assert!(error.starts_with("tbs flag ids written as numbers: 1, baseline 0\n"), "{error}");
        assert!(!error.contains("tla"), "{error}");
    }

    #[test]
    fn a_header_gives_the_name_a_listed_literal_would_take() {
        let root = tree(&[
            ("games/tbs/include/flags.h", "#define FLAG_ALTIN_FREED 0x909\n"),
            ("games/tbs/include/songs.h", "enum SoundId\n{\n\tSOUND_MENU_CANCEL = 113,\n};\n"),
            ("games/tbs/include/sound_ids.h", "enum\n{\n\tSOUND_RETIRED = 74,\n};\n"),
            ("games/tbs/src/a.c", "void F(void)\n{\n\tGameFlag_Set(0x909);\n\tSE(113);\n\tSE(74);\n}\n"),
        ]);
        let names = headers(root.path());
        let left = scan(root.path()).unwrap();
        let named: Vec<Option<String>> = left.iter().map(|left| name(&names, left)).collect();
        assert_eq!(named, [Some("FLAG_ALTIN_FREED".into()), Some("SOUND_MENU_CANCEL".into()), None]);
    }
}
