use crate::coverage::model::{Category, Tile, UNIDENTIFIED};

pub(crate) const DISPLAY_CATEGORIES: [(Category, &str); 5] = [
    (Category::Unknown, UNIDENTIFIED),
    (Category::DraftC, "Drafted"),
    (Category::ProvenAsm, "Assembly"),
    (Category::ProvenC, "C"),
    (Category::AssetData, "Data"),
];

pub(crate) fn display_bytes(categories: &[i64; 6], category: Category) -> i64 {
    categories[category as usize]
        + if category == Category::ProvenAsm {
            categories[Category::DraftAsm as usize]
        } else {
            0
        }
}
use crate::coverage::pipeline::source_container;
use sha1::{Digest, Sha1};

// The file-type palette of the figure and dashboard: each kind of file has
// its own clear pastel on the teal chart (the chrome is in `palette`); assembly and executable code sit in teal.
pub(crate) const UNKNOWN: &str = "#d9d9d4";
pub(crate) const C_TEAL: &str = "#326b7d";
const DRAFTED: &str = "#96c8c9";
pub(crate) const ASSEMBLY: &str = "#6cafb2";
pub(crate) const DRAFT_ASSEMBLY: &str = "#b4ccd2";
const TEXT_CYAN: &str = "#85cbd2";
/// C source and its headers in rose-greys, drafts paler; sprite sheets purple,
/// stills blue, maps green; registries a quiet lavender. Translation text (a
/// grey that recedes into the chart) and the frozen compression answers are
/// calm so the code and art lead the eye.
const SOURCE_ROSE: &str = "#c4b4b7";
const DRAFT_ROSE: &str = "#e2d6d7";
const HEADER_ROSE: &str = "#a8959a";
const SPRITE_PURPLE: &str = "#b5a0de";
const IMAGE_BLUE: &str = "#8fb7ec";
const MAP_GREEN: &str = "#b5cc82";
const TABLE_LAVENDER: &str = "#9aa4c2";
const REGISTRY_LAVENDER: &str = "#bcc1d4";
const TRANSLATION_GREY: &str = "#8eaab0";
const ANSWER_TAUPE: &str = "#a0968d";
const MIDI_GREEN: &str = "#81d6b2";
const PCM_ORANGE: &str = "#efbb82";
const OTHER_TAN: &str = "#bda995";
const HEADER_GOLD: &str = "#eadb83";
const SOUND_TYPES: [(&str, &str); 5] = [
    ("MIDI music", MIDI_GREEN),
    ("SFX", "#f29b91"),
    ("PCM samples", PCM_ORANGE),
    ("Tables", "#9aa4c2"),
    ("Sound sequences", "#a8d4bc"),
];
/// Sound assets are the sequence and PCM kinds plus every table described
/// under the sound directory (engine data, the sound table, residual headers).
fn is_sound(tile: &Tile) -> bool {
    matches!(
        tile.group.as_deref(),
        Some("golden-sun-sound-sequence" | "golden-sun-pcm-wave")
    ) || tile.source.as_deref().is_some_and(|source| {
        source.starts_with("games/THE BROKEN SEAL/SOUND/")
            || source.starts_with("games/THE LOST AGE/SOUND/")
    })
}
fn sound_type(tile: &Tile) -> usize {
    match (tile.group.as_deref(), tile.subgroup.as_deref()) {
        (Some("golden-sun-sound-sequence"), Some("music")) => 0,
        (Some("golden-sun-sound-sequence"), Some("sfx")) => 1,
        (Some("golden-sun-pcm-wave"), _) => 2,
        (Some("golden-sun-sound-sequence"), _) => 4,
        _ if is_sound(tile) => 3,
        _ => 4,
    }
}
pub(crate) fn content_style(tile: &Tile) -> (&'static str, &'static str) {
    let group = tile.group.as_deref().unwrap_or("");
    if let Some(extension) = group.strip_prefix("file:") {
        return file_style(extension, tile.source.as_deref().unwrap_or(""));
    }
    let kind = group.strip_prefix("indexed-").unwrap_or(group);
    let source = tile.source.as_deref().unwrap_or("");
    if kind == "golden-sun-pcm-wave" {
        return SOUND_TYPES[2];
    }
    if is_sound(tile) || kind.contains("sound") || kind.contains("audio") || kind.contains("music")
    {
        return SOUND_TYPES[sound_type(tile)];
    }
    if kind.starts_with("golden-sun-map-")
        || source.contains("/maps/")
        || source.contains("/map_resource_")
    {
        return ("Maps", "#b5cc82");
    }
    match kind {
        "golden-sun-thumb-overlay" => ("Code overlays", "#78afb7"),
        "gba-tilemap16" => ("Tile maps", "#b5cc82"),
        "mixed-data" | "components" => ("Mixed data", "#c4b4b7"),
        "golden-sun-general-lz" | "golden-sun-kind2-lz" => ("Compressed data", "#c4b4b7"),
        "u8-array" | "s8-array" | "le-u16-array" | "le-u32-array" | "be-s16-array" => {
            ("Tables", "#9aa4c2")
        }
        "1bpp-tiles" | "1bpp-rows" => ("Images", "#8fb7ec"),
        "golden-sun-static-sprite-series" | "golden-sun-character-graphics" | "zero-skip-bytes" => {
            ("Sprite sheets", "#b5a0de")
        }
        "gba-4bpp-tiles" | "gba-8bpp-tiles" | "golden-sun-delta7-still" => ("Images", "#8fb7ec"),
        "compressed-resource" => ("Compressed data", "#c4b4b7"),
        "gba-palette" | "gba-palette-rgba" | "bgr555-banks" => ("Palettes", "#e8a6d3"),
        "golden-sun-kana-glyph-bank" | "golden-sun-namae-nyuuryoku" => ("Fonts", HEADER_GOLD),
        "golden-sun-message-archive" | "golden-sun-staff-roll" => ("Text", TEXT_CYAN),
        _ if source.contains("/fonts_") || source.contains("/GRAPHICS/FONT/") => {
            ("Fonts", HEADER_GOLD)
        }
        _ if source.to_ascii_lowercase().ends_with(".png") => ("Images", "#8fb7ec"),
        "typed-table" | "record-table" | "pointer-table" => ("Tables", "#9aa4c2"),
        "gba-cartridge-header-standard-fields" => ("ROM header", HEADER_GOLD),
        "byte-fill" => ("Padding", "#bda995"),
        _ => (UNIDENTIFIED, UNKNOWN),
    }
}
/// A tracked file's kind from its extension and the module that holds it:
/// accepted C lives in SRC, complete but nonexact drafts in the
/// `recon/<game>` scaffolding; data takes the colour of what it describes.
fn file_style(extension: &str, source: &str) -> (&'static str, &'static str) {
    let recon = source.starts_with("recon/");
    let data = matches!(extension, "json" | "tsv" | "bin" | "png");
    let name = source_name(source);
    match extension {
        "c" if recon => ("Drafted C", DRAFT_ROSE),
        "c" => ("C", SOURCE_ROSE),
        "h" | "inc" => ("Headers", HEADER_ROSE),
        "s" => ("Assembly", ASSEMBLY),
        "wav" => ("WAV audio", PCM_ORANGE),
        "mid" => ("MIDI music", MIDI_GREEN),
        "po" | "md" | "txt" => ("Translations", TRANSLATION_GREY),
        "tokens" => (COMPRESSION_ANSWERS, ANSWER_TAUPE),
        _ if data && name.starts_with("COMPRESSION.") => (COMPRESSION_ANSWERS, ANSWER_TAUPE),
        _ if data && recon => ("Registries", REGISTRY_LAVENDER),
        _ if data && source.contains("/GRAPHICS/CHARACTER/") => ("Sprite sheets", SPRITE_PURPLE),
        _ if data && source.contains("/GRAPHICS/") => ("Images", IMAGE_BLUE),
        _ if data && source.contains("/FIELD/") => ("Maps", MAP_GREEN),
        "png" => ("Images", IMAGE_BLUE),
        _ if data => ("Tables", TABLE_LAVENDER),
        _ => ("Other files", OTHER_TAN),
    }
}
/// Kinds drawn without a label on each file: the translation catalogs, read
/// as one box per game under its TEXT heading, and the frozen compression
/// answers, which wait to be replaced by encoder options.
const COMPRESSION_ANSWERS: &str = "Compression answers";
pub(crate) fn quiet(tile: &Tile) -> bool {
    matches!(content_style(tile).0, "Translations" | COMPRESSION_ANSWERS)
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
#[test]
fn indexed_formats_have_colors_without_claiming_reconstructed_assets() {
    for (kind, name) in [
        ("golden-sun-pcm-wave", "PCM samples"),
        ("gba-4bpp-tiles", "Images"),
        ("golden-sun-map-container", "Maps"),
        ("golden-sun-static-sprite-series", "Sprite sheets"),
    ] {
        let tile = Tile {
            group: Some(format!("indexed-{kind}")),
            ..Tile::default()
        };
        assert_eq!(content_style(&tile).0, name);
    }
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
/// One vocabulary, palette and byte total for the HTML and README legends.
pub(crate) fn legend_items(tiles: &[&Tile]) -> Vec<(&'static str, &'static str, i64)> {
    let tiles = leaves(tiles);
    let mut items = Vec::new();
    for (category, name) in DISPLAY_CATEGORIES {
        if category == Category::AssetData {
            continue;
        }
        let bytes = tiles
            .iter()
            .map(|tile| display_bytes(&tile.categories, category))
            .sum();
        if bytes > 0 {
            items.push((name, color(category), bytes));
        }
    }
    let mut data = std::collections::BTreeMap::new();
    for tile in tiles {
        if tile.categories[Category::AssetData as usize] == tile.bytes {
            *data.entry(content_style(tile)).or_insert(0) += tile.bytes;
        }
    }
    for ((name, swatch), bytes) in data {
        if let Some(item) = items.iter_mut().find(|item| item.0 == name) {
            item.2 += bytes;
        } else {
            items.push((name, swatch, bytes));
        }
    }
    items
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
pub(crate) fn source_name(source: &str) -> &str {
    let trimmed = source.trim_end_matches('/');
    trimmed.rsplit('/').next().unwrap_or(trimmed)
}

pub(crate) fn esc(value: &str) -> String {
    value
        .replace('&', "&amp;")
        .replace('<', "&lt;")
        .replace('>', "&gt;")
        .replace('"', "&quot;")
}
pub(crate) fn color(category: Category) -> &'static str {
    match category {
        Category::Unknown => UNKNOWN,
        Category::DraftAsm => DRAFT_ASSEMBLY,
        Category::DraftC => DRAFTED,
        Category::ProvenAsm => ASSEMBLY,
        Category::ProvenC => C_TEAL,
        Category::AssetData => "#92a8ac",
    }
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
    std::fs::write(dir.join("MAP.JSON"), [0u8; 45]).unwrap();
    let git = |args: &[&str]| {
        assert!(std::process::Command::new("git")
            .args(args)
            .current_dir(temp.path())
            .status()
            .unwrap()
            .success());
    };
    git(&["init", "--quiet"]);
    git(&["add", "games/test/MAP.JSON"]);
    let tiles = tracked_only(temp.path(), disk_tiles(temp.path()));
    assert_eq!(tiles.len(), 1);
    assert_eq!(tiles[0].source.as_deref(), Some("games/test/MAP.JSON"));
}

/// reconstruction scaffolding kept beside them under `recon/`.
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
            let extension = entry
                .path()
                .extension()
                .and_then(|e| e.to_str())
                .unwrap_or("")
                .to_ascii_lowercase();
            Some(Tile {
                label: entry.file_name().to_string_lossy().into(),
                bytes,
                categories: [0, 0, 0, 0, 0, bytes],
                source: Some(source),
                group: Some(format!("file:{extension}")),
                ..Tile::default()
            })
        })
        .collect()
}
/// A short content digest for cache stamps.
pub fn content_version(text: &str) -> String {
    format!("{:x}", Sha1::digest(text.as_bytes()))[..16].into()
}

#[test]
fn content_version_uses_standard_sha1_prefix() {
    assert_eq!(content_version("abc"), "a9993e364706816a");
}
#[cfg(test)]
mod tests {
    use super::{content_style, directories, leaves, sound_type, SOUND_TYPES};
    use crate::coverage::model::{Category, Tile};

    #[test]
    fn all_directories_wrap_files_without_duplicating_bytes() {
        let tile = Tile {
            source: Some("games/THE BROKEN SEAL/SRC/battle/effects/fire.c".into()),
            address: Some(0x080bbb0c),
            bytes: 100,
            categories: [100, 0, 0, 0, 0, 0],
            ..Tile::default()
        };
        let mut other = tile.clone();
        other.address = Some(0x080bbb70);
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
            assert_eq!(node.categories, [200, 0, 0, 0, 0, 0]);
            assert_eq!(node.address, None);
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
    fn content_colors_use_types_not_the_card_theme() {
        let mut tile = Tile {
            group: Some("golden-sun-static-sprite-series".into()),
            ..Tile::default()
        };
        let sprites = content_style(&tile);
        tile.group = Some("golden-sun-map-metatiles".into());
        let maps = content_style(&tile);
        tile.group = Some("gba-palette".into());
        assert_ne!(sprites.1, maps.1);
        assert_ne!(maps.1, content_style(&tile).1);
    }

    #[test]
    fn requested_file_palette_is_explicit() {
        let tile = |extension: &str| Tile {
            group: Some(format!("file:{extension}")),
            ..Tile::default()
        };
        assert_eq!(content_style(&tile("s")), ("Assembly", super::ASSEMBLY));
        assert_eq!(content_style(&tile("c")), ("C", super::SOURCE_ROSE));
        let draft = Tile {
            source: Some("recon/tbs/en/main/08006878.c".into()),
            ..tile("c")
        };
        assert_eq!(content_style(&draft), ("Drafted C", super::DRAFT_ROSE));
        assert_eq!(
            content_style(&tile("po")),
            ("Translations", super::TRANSLATION_GREY)
        );
        let placed = |source: &str, extension: &str| Tile {
            source: Some(source.into()),
            ..tile(extension)
        };
        for (source, extension, kind) in [
            (
                "games/X/SRC/GRAPHICS/CHARACTER/COMMON.JSON",
                "json",
                "Sprite sheets",
            ),
            ("games/X/SRC/GRAPHICS/COMMON/STILL.JSON", "json", "Images"),
            ("games/X/SRC/FIELD/AREA/AREA.JSON", "json", "Maps"),
            ("games/X/SRC/GAME/DATABASES.JSON", "json", "Tables"),
            ("recon/tbs/source-paths.json", "json", "Registries"),
            (
                "games/X/SRC/GRAPHICS/COMMON/COMPRESSION.TOKENS",
                "tokens",
                "Compression answers",
            ),
            (
                "games/X/SRC/GRAPHICS/COMMON/COMPRESSION.JSON",
                "json",
                "Compression answers",
            ),
        ] {
            assert_eq!(
                content_style(&placed(source, extension)).0,
                kind,
                "{source}"
            );
        }
        assert!(super::quiet(&placed("games/X/TEXT/DE.PO", "po")));
        assert!(!super::quiet(&placed("games/X/SRC/FIELD/A/A.JSON", "json")));
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
            categories: [0, 0, 0, 0, 0, bytes],
            source: Some(format!("games/X/SRC/FIELD/AREA/{name}")),
            group: Some(format!("file:{extension}")),
            ..Tile::default()
        };
        let folder = Tile {
            bytes: 120,
            categories: [0, 0, 0, 0, 0, 120],
            children: vec![
                file("ENTRY.INC", "inc", 5),
                file("IMPORT.INC", "inc", 5),
                file("AREA.JSON", "json", 40),
                file("SCENE.C", "c", 70),
            ],
            ..Tile::default()
        };
        assert_eq!(
            super::content_mix(&folder),
            vec![
                ("C", super::SOURCE_ROSE, 70),
                ("Maps", super::MAP_GREEN, 40),
                ("Headers", super::HEADER_ROSE, 10)
            ]
        );
        assert_eq!(super::color(Category::DraftC), super::DRAFTED);
        assert_eq!(super::color(Category::ProvenAsm), super::ASSEMBLY);
        assert_eq!(super::color(Category::ProvenC), super::C_TEAL);
    }
    #[test]
    fn unidentified_has_one_shared_legend_entry_and_tables_have_a_type() {
        let tile = |bytes, categories, group: &str| Tile {
            bytes,
            categories,
            group: Some(group.into()),
            ..Tile::default()
        };
        let tiles = vec![
            tile(10, [0, 0, 10, 0, 0, 0], ""),
            tile(20, [0, 0, 0, 0, 0, 20], "unreconstructed-data"),
            tile(70, [0, 0, 0, 0, 0, 70], "typed-table"),
        ];
        assert_eq!(
            super::legend_items(&tiles.iter().collect::<Vec<_>>()),
            vec![
                ("Unidentified", super::UNKNOWN, 30),
                ("Tables", "#9aa4c2", 70),
            ]
        );
        for kind in [
            "",
            "unreconstructed-data",
            "unclassified",
            "new-unknown-format",
        ] {
            assert_eq!(
                content_style(&tile(1, [0, 0, 0, 0, 0, 1], kind)),
                ("Unidentified", super::UNKNOWN)
            );
        }
    }

    #[test]
    fn midi_sequence_roles_get_distinct_colors() {
        let mut tile = Tile {
            group: Some("golden-sun-sound-sequence".into()),
            ..Tile::default()
        };
        for (class, expected) in [(Some("music"), 0), (Some("sfx"), 1), (None, 4)] {
            tile.subgroup = class.map(String::from);
            assert_eq!(sound_type(&tile), expected);
        }
        assert_ne!(SOUND_TYPES[0].1, SOUND_TYPES[1].1);
    }

    #[test]
    fn known_rom_index_formats_have_identified_content_types() {
        for kind in [
            "golden-sun-sound-sequence",
            "golden-sun-thumb-overlay",
            "golden-sun-character-graphics",
            "zero-skip-bytes",
        ] {
            let tile = Tile {
                group: Some(kind.into()),
                ..Tile::default()
            };
            assert_ne!(content_style(&tile).0, super::UNIDENTIFIED, "{kind}");
        }
    }
}
