use std::collections::BTreeSet;
use std::fs;
use std::io;
use std::path::{Path, PathBuf};
use std::process::Command;

/// Source code a game tree tracks beside its material; builds compile it.
const CODE_EXTENSIONS: &[&str] = &["c", "h", "inc", "s"];
/// Game-root registries of owner names, addresses and locations.
const GAME_REGISTRIES: &[&str] = &["locations.tsv", "project.json", "source-paths.json"];

/// Match a filename against an anchored `*`/`?` glob.
pub fn glob_matches(pattern: &str, name: &str) -> bool {
    fn walk(pattern: &[char], name: &[char]) -> bool {
        match pattern.first() {
            None => name.is_empty(),
            Some('*') => (0..=name.len()).any(|split| walk(&pattern[1..], &name[split..])),
            Some('?') => !name.is_empty() && walk(&pattern[1..], &name[1..]),
            Some(literal) => name.first() == Some(literal) && walk(&pattern[1..], &name[1..]),
        }
    }
    let pattern: Vec<char> = pattern.chars().collect();
    let name: Vec<char> = name.chars().collect();
    walk(&pattern, &name)
}

/// Delete unclaimed matching files directly inside `directory`.
pub fn prune_files(
    directory: &Path,
    pattern: &str,
    keep: impl IntoIterator<Item = impl AsRef<Path>>,
) -> io::Result<Vec<PathBuf>> {
    let keep: BTreeSet<_> = keep
        .into_iter()
        .filter_map(|path| path.as_ref().file_name().map(ToOwned::to_owned))
        .collect();
    if !directory.exists() {
        return Ok(Vec::new());
    }
    let mut removed = Vec::new();
    for entry in fs::read_dir(directory)?.filter_map(Result::ok) {
        let path = entry.path();
        if path.is_file()
            && path.file_name().is_some_and(|name| {
                glob_matches(pattern, &name.to_string_lossy()) && !keep.contains(name)
            })
        {
            removed.push(path);
        }
    }
    removed.sort();
    removed.iter().try_for_each(fs::remove_file)?;
    Ok(removed)
}

/// Whether a tracked file under `games/<game>/` carries no game data by
/// category, so no build needs to read it. Everything else there is game
/// material: pictures, samples, music, text, maps, tables and their indexes.
fn carries_no_game_data(game_relative: &str) -> bool {
    let components: Vec<_> = game_relative.split('/').collect();
    let leaf = components.last().copied().unwrap_or("");
    let suffix = leaf.rsplit_once('.').map_or("", |(_, suffix)| suffix);
    let is = |value: &str, choices: &[&str]| {
        choices
            .iter()
            .any(|choice| value.eq_ignore_ascii_case(choice))
    };
    match components.as_slice() {
        // Code the builds compile.
        _ if leaf == ".gitkeep" || is(suffix, CODE_EXTENSIONS) => true,
        // Owner names, addresses and locations.
        [registry] => is(registry, GAME_REGISTRIES),
        _ => false,
    }
}
/// Tracked game material under `game_dir` that is in no consumer's input set.
pub fn unconsumed_material(
    tracked: impl IntoIterator<Item = impl AsRef<str>>,
    game_dir: &str,
    consumed: &BTreeSet<String>,
) -> Vec<String> {
    let prefix = format!("{game_dir}/");
    tracked
        .into_iter()
        .filter_map(|path| {
            let path = path.as_ref();
            let game_relative = path.strip_prefix(&prefix)?;
            (!carries_no_game_data(game_relative) && !consumed.contains(path))
                .then(|| path.to_string())
        })
        .collect()
}
/// Tracked game material under `game_dir` that no build consumer read.
pub fn unconsumed_tracked_material(
    root: &Path,
    game_dir: &str,
    consumed: &BTreeSet<String>,
) -> io::Result<Vec<String>> {
    if !root.join(".git").exists() {
        return Ok(Vec::new());
    }
    let output = Command::new("git")
        .args(["ls-files", "-z", "--", game_dir])
        .current_dir(root)
        .output()?;
    if !output.status.success() {
        return Err(io::Error::other(String::from_utf8_lossy(&output.stderr)));
    }
    let listing = String::from_utf8_lossy(&output.stdout);
    Ok(unconsumed_material(
        listing.split('\0').filter(|name| !name.is_empty()),
        game_dir,
        consumed,
    ))
}
#[test]
fn only_game_material_needs_a_consumer_and_exemptions_are_categories() {
    let game = "games/X";
    let tracked = [
        "games/X/SRC/A.JSON",
        "games/X/SRC/B.JSON",
        "games/X/SRC/X.C",
        "games/X/SRC/FIELD/MAP.INC",
        "games/X/raw/overlays/resource_001_overlay.s",
        "games/X/raw/overlays/resource_001_stream.lz.json",
        "games/X/raw/overlays/.gitkeep",
        "games/X/raw/manifest.json",
        "games/X/raw/islands/0800a000/index.json",
        "games/X/raw/islands/TABLE.TSV",
        "games/X/recon/en/dossiers.json",
        "games/X/semantic/regions.json",
        "games/X/metrics/x-en-executable.json",
        "games/X/PREVIEW/TITLE.PNG",
        "games/X/source-paths.json",
        "games/X/PROJECT.JSON",
        "games/X/locations.tsv",
        "games/X/SOURCE.JSON",
        "games/X/TEXT/MESSAGE_ARCHIVE.JSON",
        "games/X/SOUND/SEQUENCE/A.MID",
        "games/XY/SRC/C.JSON",
        "tools/alchemy/src/main.rs",
    ];
    let consumed: BTreeSet<String> = [
        "games/X/SRC/A.JSON",
        "games/X/raw/overlays/resource_001_stream.lz.json",
        "games/X/SOURCE.JSON",
    ]
    .map(String::from)
    .into();
    assert_eq!(
        unconsumed_material(tracked, game, &consumed),
        [
            "games/X/SRC/B.JSON",
            "games/X/raw/manifest.json",
            "games/X/raw/islands/0800a000/index.json",
            "games/X/raw/islands/TABLE.TSV",
            "games/X/recon/en/dossiers.json",
            "games/X/semantic/regions.json",
            "games/X/metrics/x-en-executable.json",
            "games/X/PREVIEW/TITLE.PNG",
            "games/X/TEXT/MESSAGE_ARCHIVE.JSON",
            "games/X/SOUND/SEQUENCE/A.MID",
        ]
    );
}
#[test]
fn tracked_material_audit_reads_the_index_not_the_worktree() {
    let directory = tempfile::tempdir().unwrap();
    let root = directory.path();
    let git = |args: &[&str]| {
        let status = Command::new("git")
            .args(args)
            .current_dir(root)
            .status()
            .unwrap();
        assert!(status.success(), "git {args:?}");
    };
    git(&["init", "--quiet"]);
    for (name, text) in [
        ("games/X/SRC/A.JSON", "{}"),
        ("games/X/SRC/B.JSON", "{}"),
        ("games/X/SRC/X.C", "void X(void) {}"),
        ("games/X/SRC/UNTRACKED.JSON", "{}"),
    ] {
        let path = root.join(name);
        fs::create_dir_all(path.parent().unwrap()).unwrap();
        fs::write(path, text).unwrap();
    }
    git(&[
        "add",
        "games/X/SRC/A.JSON",
        "games/X/SRC/B.JSON",
        "games/X/SRC/X.C",
    ]);
    let consumed = BTreeSet::from(["games/X/SRC/A.JSON".to_string()]);
    assert_eq!(
        unconsumed_tracked_material(root, "games/X", &consumed).unwrap(),
        ["games/X/SRC/B.JSON"]
    );
}
