use crate::coverage::model::Tile;

// The file-type palette of the figure: each kind of file has its own clear
// pastel on the teal chart (the chrome is in `palette`). C source and its
// headers are rose-greys, drafts paler; sprite sheets purple, stills blue,
// maps green; translation text a teal that recedes into the chart, so the
// code and art lead the eye.
const SOURCE_ROSE: &str = "#c4b4b7";
const DRAFT_ROSE: &str = "#e2d6d7";
const HEADER_ROSE: &str = "#a8959a";
const SPRITE_PURPLE: &str = "#b5a0de";
const IMAGE_BLUE: &str = "#8fb7ec";
const MAP_GREEN: &str = "#b5cc82";
const TABLE_LAVENDER: &str = "#9aa4c2";
const DRAFT_GREY: &str = "#8eaab0";
const TRANSLATION_TEAL: &str = "#6cafb2";
const MIDI_GREEN: &str = "#81d6b2";
const PCM_ORANGE: &str = "#efbb82";
const OTHER_TAN: &str = "#bda995";
/// Library or handwritten assembly its header credits, a deeper teal.
const CREDITED_ASSEMBLY: &str = "#4f94a0";
/// Uncredited assembly: C that is not written yet (AGENTS.md rule 3).
pub(crate) const NOT_YET_C: &str = "Not yet C";

/// A tracked file's kind from its extension and the module that holds it:
/// accepted C lives in SRC, complete but nonexact drafts in the
/// `recon/<game>` scaffolding; images take the colour of what they show.
pub(crate) fn content_style(tile: &Tile) -> (&'static str, &'static str) {
    let source = tile.source.as_deref().unwrap_or("");
    match tile.extension.as_str() {
        "c" if source.starts_with("recon/") => ("Drafted C", DRAFT_GREY),
        "c" => ("C", SOURCE_ROSE),
        "h" | "inc" => ("Headers", HEADER_ROSE),
        "s-credited" => ("Assembly", CREDITED_ASSEMBLY),
        "s" => (NOT_YET_C, DRAFT_ROSE),
        "wav" => ("WAV audio", PCM_ORANGE),
        "mid" => ("MIDI music", MIDI_GREEN),
        "po" => ("Translations", TRANSLATION_TEAL),
        "png" if source.contains("/GRAPHICS/CHARACTER/") => ("Sprite sheets", SPRITE_PURPLE),
        "png" if source.contains("/GRAPHICS/") => ("Images", IMAGE_BLUE),
        "png" if source.contains("/FIELD/") => ("Maps", MAP_GREEN),
        "png" => ("Images", IMAGE_BLUE),
        "tsv" => ("Tables", TABLE_LAVENDER),
        _ => ("Other files", OTHER_TAN),
    }
}
/// Kinds drawn without a label on each file: the translation catalogs, read
/// as one box per game under its TEXT heading.
pub(crate) fn quiet(tile: &Tile) -> bool {
    content_style(tile).0 == "Translations"
}
/// The content types of a tile's files and their bytes, largest first.
pub(crate) fn content_mix(tile: &Tile) -> Vec<(&'static str, &'static str, i64)> {
    fn gather(tile: &Tile, mix: &mut Vec<(&'static str, &'static str, i64)>) {
        if !tile.children.is_empty() {
            for child in &tile.children {
                gather(child, mix);
            }
            return;
        }
        let (name, color) = content_style(tile);
        match mix.iter_mut().find(|(n, c, _)| *n == name && *c == color) {
            Some(entry) => entry.2 += tile.bytes,
            None => mix.push((name, color, tile.bytes)),
        }
    }
    let mut mix = Vec::new();
    gather(tile, &mut mix);
    mix.sort_by(|a, b| b.2.cmp(&a.2).then(a.0.cmp(b.0)));
    mix
}
pub(crate) fn leaves<'a>(tiles: &[&'a Tile]) -> Vec<&'a Tile> {
    tiles
        .iter()
        .flat_map(|tile| {
            if tile.children.is_empty() {
                vec![*tile]
            } else {
                leaves(&tile.children.iter().collect::<Vec<_>>())
            }
        })
        .collect()
}
/// One vocabulary, palette and byte total for the legend.
pub(crate) fn legend_items(tiles: &[&Tile]) -> Vec<(&'static str, &'static str, i64)> {
    let mut kinds = std::collections::BTreeMap::new();
    for tile in leaves(tiles) {
        *kinds.entry(content_style(tile)).or_insert(0) += tile.bytes;
    }
    kinds
        .into_iter()
        .map(|((name, swatch), bytes)| (name, swatch, bytes))
        .collect()
}
// Keep single-child directories: their path is part of the displayed hierarchy.
pub(crate) fn directories(tiles: Vec<Tile>, base: &str) -> Vec<Tile> {
    let base = if tiles
        .iter()
        .filter_map(|tile| tile.source.as_deref())
        .all(|source| source.starts_with(base))
    {
        base
    } else {
        ""
    };
    let mut folders = std::collections::BTreeMap::<String, Vec<Tile>>::new();
    let mut files = std::collections::BTreeMap::<String, Vec<Tile>>::new();
    let mut out = Vec::new();
    for tile in tiles {
        let Some(source) = tile.source.as_deref().filter(|s| !s.is_empty()) else {
            out.push(tile);
            continue;
        };
        let relative = source.strip_prefix(base).unwrap_or(source);
        if relative.is_empty() {
            // Bytes assigned only to this real directory have no file owner.
            out.push(tile);
            continue;
        }
        if let Some((folder, _)) = relative.split_once('/') {
            folders
                .entry(format!("{base}{folder}/"))
                .or_default()
                .push(tile);
        } else {
            files.entry(source.into()).or_default().push(tile);
        }
    }
    for (source, mut children) in files {
        let mut tile = if children.len() == 1 {
            children.pop().unwrap()
        } else {
            source_container(source.clone(), children)
        };
        if !tile.children.is_empty()
            && !tile
                .children
                .iter()
                .all(|child| child.source == tile.source)
        {
            let base = source
                .rsplit_once('/')
                .map_or(String::new(), |(dir, _)| format!("{dir}/"));
            tile.children = directories(tile.children, &base);
        }
        out.push(tile);
    }
    for (path, children) in folders {
        let children = directories(children, &path);
        out.push(source_container(path, children));
    }
    out
}
/// A folder tile holding `children`, its bytes their sum.
fn source_container(source: String, children: Vec<Tile>) -> Tile {
    Tile {
        label: source_name(&source).into(),
        bytes: children.iter().map(|child| child.bytes).sum(),
        extension: String::new(),
        source: Some(source),
        children,
    }
}
pub(crate) fn source_name(source: &str) -> &str {
    let trimmed = source.trim_end_matches('/');
    trimmed.rsplit('/').next().unwrap_or(trimmed)
}

pub(crate) fn tracked_only(repository: &std::path::Path, tiles: Vec<Tile>) -> Vec<Tile> {
    let Ok(output) = std::process::Command::new("git")
        .args(["ls-files", "-z", "--", "games", "recon"])
        .current_dir(repository)
        .output()
    else {
        return Vec::new();
    };
    let tracked: std::collections::BTreeSet<String> = String::from_utf8_lossy(&output.stdout)
        .split('\0')
        .filter(|name| !name.is_empty())
        .map(str::to_owned)
        .collect();
    tiles
        .into_iter()
        .filter(|tile| tile.source.as_ref().is_some_and(|s| tracked.contains(s)))
        .collect()
}

#[test]
fn published_view_leaves_out_untracked_private_inputs() {
    let temp = tempfile::tempdir().unwrap();
    let dir = temp.path().join("games/test");
    std::fs::create_dir_all(&dir).unwrap();
    std::fs::write(dir.join("MAP.BIN"), [0u8; 123]).unwrap();
    std::fs::write(dir.join("MAP.S"), [0u8; 45]).unwrap();
    let git = |args: &[&str]| {
        assert!(std::process::Command::new("git")
            .args(args)
            .current_dir(temp.path())
            .status()
            .unwrap()
            .success());
    };
    git(&["init", "--quiet"]);
    git(&["add", "games/test/MAP.S"]);
    let tiles = tracked_only(temp.path(), disk_tiles(temp.path()));
    assert_eq!(tiles.len(), 1);
    assert_eq!(tiles[0].source.as_deref(), Some("games/test/MAP.S"));
}

/// Whether an assembly module's header credits it (`@ credit: library|
/// handwritten — <object>`).
fn credited(path: &std::path::Path) -> bool {
    use std::io::Read;
    let mut head = [0u8; 512];
    let read = std::fs::File::open(path)
        .and_then(|mut file| file.read(&mut head))
        .unwrap_or(0);
    String::from_utf8_lossy(&head[..read]).lines().any(|line| {
        line.trim_start().starts_with("@ credit: library")
            || line.trim_start().starts_with("@ credit: handwritten")
    })
}
/// Every nonempty file of the Camelot-shaped trees and the reconstruction
/// scaffolding kept beside them under `recon/`.
pub(crate) fn disk_tiles(repository: &std::path::Path) -> Vec<Tile> {
    ["games", "recon"]
        .into_iter()
        .flat_map(|tree| walkdir::WalkDir::new(repository.join(tree)).follow_links(false))
        .filter_map(Result::ok)
        .filter(|e| e.file_type().is_file())
        .filter_map(|entry| {
            let bytes = i64::try_from(entry.metadata().ok()?.len()).ok()?;
            if bytes == 0 {
                return None;
            }
            let source = entry
                .path()
                .strip_prefix(repository)
                .ok()?
                .to_str()?
                .to_string();
            let mut extension = entry
                .path()
                .extension()
                .and_then(|e| e.to_str())
                .unwrap_or("")
                .to_ascii_lowercase();
            // Assembly counts as assembly only when its header credits it as
            // library or handwritten code; the rest is C not yet written.
            if extension == "s" && credited(entry.path()) {
                extension = "s-credited".into();
            }
            Some(Tile {
                label: entry.file_name().to_string_lossy().into(),
                bytes,
                extension,
                source: Some(source),
                children: Vec::new(),
            })
        })
        .collect()
}

#[cfg(test)]
mod tests {
    use super::{content_style, directories, leaves};
    use crate::coverage::model::Tile;

    #[test]
    fn all_directories_wrap_files_without_duplicating_bytes() {
        let tile = Tile {
            source: Some("games/THE BROKEN SEAL/SRC/battle/effects/fire.c".into()),
            bytes: 100,
            extension: "c".into(),
            ..Tile::default()
        };
        let other = tile.clone();
        let grouped = directories(vec![tile, other], "");
        let mut node = &grouped[0];
        for path in [
            "games/",
            "games/THE BROKEN SEAL/",
            "games/THE BROKEN SEAL/SRC/",
            "games/THE BROKEN SEAL/SRC/battle/",
            "games/THE BROKEN SEAL/SRC/battle/effects/",
        ] {
            assert_eq!(node.source.as_deref(), Some(path));
            assert_eq!(node.children.len(), 1);
            assert_eq!(node.bytes, 200);
            node = &node.children[0];
        }
        assert_eq!(node.label, "fire.c");
        assert_eq!(node.children.len(), 2);
        assert_eq!(
            leaves(&grouped.iter().collect::<Vec<_>>())
                .iter()
                .map(|tile| tile.bytes)
                .sum::<i64>(),
            200
        );
        let referenced = directories(vec![node.clone()], "outside/");
        assert_eq!(referenced[0].source.as_deref(), Some("games/"));
    }

    #[test]
    fn requested_file_palette_is_explicit() {
        let tile = |extension: &str| Tile {
            extension: extension.into(),
            ..Tile::default()
        };
        assert_eq!(content_style(&tile("s")), ("Not yet C", super::DRAFT_ROSE));
        assert_eq!(
            content_style(&tile("s-credited")),
            ("Assembly", super::CREDITED_ASSEMBLY)
        );
        assert_eq!(content_style(&tile("c")), ("C", super::SOURCE_ROSE));
        let draft = Tile {
            source: Some("recon/tbs/en/main/08006878.c".into()),
            ..tile("c")
        };
        assert_eq!(content_style(&draft), ("Drafted C", super::DRAFT_GREY));
        assert_eq!(
            content_style(&tile("po")),
            ("Translations", super::TRANSLATION_TEAL)
        );
        let placed = |source: &str, extension: &str| Tile {
            source: Some(source.into()),
            ..tile(extension)
        };
        for (source, extension, kind) in [
            (
                "games/X/SRC/GRAPHICS/CHARACTER/HERO.PNG",
                "png",
                "Sprite sheets",
            ),
            ("games/X/SRC/GRAPHICS/FONT/GLYPHS.PNG", "png", "Images"),
            ("games/X/SRC/FIELD/AREA/AREA.PNG", "png", "Maps"),
            ("games/X/TEXT/STAFF_ROLL.PNG", "png", "Images"),
            ("recon/tbs/metrics/history.tsv", "tsv", "Tables"),
            ("games/X/SRC/FIELD/AREA/FIELD_DATA.INC", "inc", "Headers"),
            ("games/X/SRC/FIELD/AREA/OVERLAY.LD", "ld", "Other files"),
        ] {
            assert_eq!(
                content_style(&placed(source, extension)).0,
                kind,
                "{source}"
            );
        }
        assert!(super::quiet(&placed("games/X/TEXT/DE.PO", "po")));
        assert!(!super::quiet(&placed("games/X/SRC/FIELD/A/A.S", "s")));
        assert_eq!(
            content_style(&tile("mid")),
            ("MIDI music", super::MIDI_GREEN)
        );
        assert_eq!(
            content_style(&tile("wav")),
            ("WAV audio", super::PCM_ORANGE)
        );
        assert_eq!(
            content_style(&tile("xyz")),
            ("Other files", super::OTHER_TAN)
        );
        // A collapsed folder is drawn by the bytes of what it holds.
        let file = |name: &str, extension: &str, bytes| Tile {
            label: name.into(),
            bytes,
            extension: extension.into(),
            source: Some(format!("games/X/SRC/FIELD/AREA/{name}")),
            children: Vec::new(),
        };
        let folder = Tile {
            bytes: 120,
            children: vec![
                file("ENTRY.S", "s-credited", 5),
                file("IMPORT.S", "s-credited", 5),
                file("AREA.PNG", "png", 40),
                file("SCENE.C", "c", 70),
            ],
            ..Tile::default()
        };
        assert_eq!(
            super::content_mix(&folder),
            vec![
                ("C", super::SOURCE_ROSE, 70),
                ("Maps", super::MAP_GREEN, 40),
                ("Assembly", super::CREDITED_ASSEMBLY, 10)
            ]
        );
    }
    #[test]
    fn the_legend_totals_each_kind_once() {
        let tile = |bytes, extension: &str| Tile {
            bytes,
            extension: extension.into(),
            ..Tile::default()
        };
        let tiles = vec![tile(10, "s"), tile(20, "s"), tile(70, "tsv")];
        assert_eq!(
            super::legend_items(&tiles.iter().collect::<Vec<_>>()),
            vec![
                (super::NOT_YET_C, super::DRAFT_ROSE, 30),
                ("Tables", super::TABLE_LAVENDER, 70),
            ]
        );
    }
}
