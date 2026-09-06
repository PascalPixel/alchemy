use crate::jsnum::commas;
use crate::model::{treemap, Area, Category, Rect, Tile, CATEGORIES};
use crate::pipeline::{source_container, CoverageMap};
use crate::sha1::sha1_hex;
use crate::tree::root;

pub const BOX_TREES: [&str; 4] = ["core", "overlays", "images", "music"];
const CHART_BACKGROUND: &str = "#1f7f93";
const UNKNOWN: &str = "#c9d5d5";
const SOUND_TYPES: [(&str, &str); 5] = [
    ("MIDI music", "#99cbbb"),
    ("SFX", "#cda3ad"),
    ("PCM samples", "#d8bd8a"),
    ("Tables", "#8fa4b0"),
    ("Unclassified", UNKNOWN),
];
fn sound_type(tile: &Tile) -> usize {
    match (tile.group.as_deref(), tile.subgroup.as_deref()) {
        (Some("golden-sun-sound-sequence"), Some("music")) => 0,
        (Some("golden-sun-sound-sequence"), Some("sfx")) => 1,
        (Some("golden-sun-pcm-wave"), _) => 2,
        (
            Some(
                "golden-sun-audio-engine-data"
                | "golden-sun-sound-table"
                | "golden-sun-music-residual",
            ),
            _,
        ) => 3,
        _ => 4,
    }
}
fn content_style(tile: &Tile) -> (&'static str, &'static str) {
    let kind = tile.group.as_deref().unwrap_or("");
    let source = tile.source.as_deref().unwrap_or("");
    if kind.contains("sound")
        || kind.contains("audio")
        || kind.contains("music")
        || kind == "golden-sun-pcm-wave"
    {
        return SOUND_TYPES[sound_type(tile)];
    }
    if kind.starts_with("golden-sun-map-")
        || source.contains("/maps/")
        || source.contains("/map_resource_")
    {
        return ("Maps", "#9bbfaf");
    }
    match kind {
        "golden-sun-static-sprite-series" => ("Sprite sheets", "#afa6c6"),
        "gba-palette" | "gba-palette-rgba" => ("Palettes", "#c8a2b0"),
        "golden-sun-localization-font"
        | "golden-sun-kana-glyph-bank"
        | "golden-sun-namae-nyuuryoku" => ("Fonts", "#d6c58e"),
        "golden-sun-message-archive" | "golden-sun-staff-roll" => ("Text", "#b6ccc1"),
        _ if source.contains("/fonts_") => ("Fonts", "#d6c58e"),
        _ if source.ends_with(".png") => ("Images", "#8eafc7"),
        _ => ("Other data", "#92a8ac"),
    }
}
fn leaves<'a>(tiles: &[&'a Tile]) -> Vec<&'a Tile> {
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
// Keep single-child directories: their path is part of the displayed hierarchy.
fn directories(tiles: Vec<Tile>, base: &str) -> Vec<Tile> {
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
fn draw_tiles(
    out: &mut Vec<String>,
    tree: &str,
    tiles: &[&Tile],
    frame: Rect,
    parent_source: Option<&str>,
    reserved: &[Rect],
) {
    let assets = matches!(tree, "images" | "music");
    for placed in treemap(tiles, |tile| tile.bytes, frame) {
        let tile = tiles[placed.index];
        let rect = placed.rect;
        let container = !tile.children.is_empty();
        let folder = tile
            .source
            .as_deref()
            .is_some_and(|source| source.ends_with('/'));
        let address = tile
            .address
            .map(|a| format!(" data-address=\"0x{a:08x}\""))
            .unwrap_or_default();
        let source = tile
            .source
            .as_ref()
            .map(|s| format!(" data-source=\"{}\"", esc(s)))
            .unwrap_or_default();
        let kind = if container { "container" } else { "leaf" };
        let label = format!(
            "{}: {} bytes{}",
            tile.label,
            commas(tile.bytes),
            if container {
                format!(" · {} items", tile.children.len())
            } else {
                String::new()
            }
        );
        out.push(format!(
            "<g aria-label=\"{}\" data-node=\"{kind}\" data-kind=\"{}\" data-bytes=\"{}\"{address}{source}>",
            esc(&label),
            if folder { "folder" } else { "file" },
            tile.bytes
        ));
        out.push(format!("<title>{}</title>", esc(&label)));
        let inset = (if container { 2.0_f64 } else { 0.5 })
            .min(rect.width / 4.0)
            .min(rect.height / 4.0);
        let mut body = Rect {
            x: rect.x + inset,
            y: rect.y + inset,
            width: rect.width - 2.0 * inset,
            height: rect.height - 2.0 * inset,
        };
        if container {
            out.push(format!("<rect class=\"container-frame\" x=\"{}\" y=\"{}\" width=\"{}\" height=\"{}\" fill=\"{CHART_BACKGROUND}\"/>", rect.x, rect.y, rect.width, rect.height));
            bevel(out, rect);
        } else if assets {
            let (name, color) = content_style(tile);
            out.push(format!("<rect data-content-type=\"{name}\" x=\"{}\" y=\"{}\" width=\"{}\" height=\"{}\" style=\"fill:{color}\"/>", body.x, body.y, body.width, body.height));
        } else {
            let mut y = body.y + body.height;
            for (category, _, _) in CATEGORIES {
                let n = tile.categories[category as usize];
                if n <= 0 {
                    continue;
                }
                let height = body.height * n as f64 / tile.bytes.max(1) as f64;
                y -= height;
                out.push(format!(
                    "<rect x=\"{}\" y=\"{}\" width=\"{}\" height=\"{}\" style=\"{}\"/>",
                    body.x,
                    y,
                    body.width,
                    height,
                    fill(category)
                ));
            }
        }
        let name = tile
            .source
            .as_deref()
            .map(|source| {
                source
                    .trim_end_matches('/')
                    .rsplit('/')
                    .next()
                    .unwrap_or(source)
            })
            .unwrap_or(&tile.label);
        let caption = caption(name, body, folder).filter(|(_, bounds)| {
            (tile.source.is_none() || tile.source.as_deref() != parent_source)
                && !reserved.iter().any(|r| {
                    bounds.x < r.x + r.width
                        && bounds.x + bounds.width > r.x
                        && bounds.y < r.y + r.height
                        && bounds.y + bounds.height > r.y
                })
        });
        let mut reserved = reserved.to_vec();
        if let Some((text, bounds)) = &caption {
            if folder {
                out.push(text.clone());
            } else {
                reserved.push(*bounds);
            }
        }
        if caption.is_some() && folder {
            body.y += 20.0;
            body.height -= 20.0;
        }
        if container {
            draw_tiles(
                out,
                tree,
                &tile.children.iter().collect::<Vec<_>>(),
                body,
                tile.source.as_deref(),
                &reserved,
            );
        }
        if let Some((text, _)) = caption.filter(|_| !folder) {
            out.push(text);
        }
        out.push("</g>".into());
    }
}

fn caption(name: &str, body: Rect, folder: bool) -> Option<(String, Rect)> {
    let columns = ((body.width - 8.0) / 9.0).max(0.0) as usize;
    if columns == 0 || name.is_empty() || body.height < 28.0 || (!folder && body.width < 64.0) {
        return None;
    }
    let mut lines = Vec::new();
    let mut line = String::new();
    for part in name.split_inclusive(['_', '.', '-', ' ']) {
        if part.chars().count() > columns {
            return None;
        }
        if line.chars().count() + part.chars().count() > columns {
            lines.push(std::mem::take(&mut line));
        }
        line.push_str(part);
    }
    if !line.is_empty() {
        lines.push(line);
    }
    let rows = lines.len();
    if (folder && rows > 1) || rows as f64 * 18.0 + 8.0 > body.height {
        return None;
    }
    let width = lines
        .iter()
        .map(|line| line.chars().count())
        .max()
        .unwrap_or(0) as f64
        * 9.0;
    let height = rows as f64 * 18.0;
    let bounds = Rect {
        x: if folder {
            body.x + 2.0
        } else {
            body.x + (body.width - width) / 2.0
        },
        y: if folder {
            body.y
        } else {
            body.y + (body.height - height) / 2.0
        },
        width,
        height,
    };
    let text = if folder {
        format!("<text class=\"weyard rectangle-label folder-label\" x=\"{}\" y=\"{}\" pointer-events=\"none\">{}</text>", bounds.x, body.y + 16.0, esc(name))
    } else {
        let x = body.x + body.width / 2.0;
        let lines = lines
            .iter()
            .enumerate()
            .map(|(row, line)| {
                format!(
                    "<tspan x=\"{x}\" y=\"{}\">{}</tspan>",
                    bounds.y + row as f64 * 18.0 + 9.0,
                    esc(line)
                )
            })
            .collect::<String>();
        format!("<text class=\"weyard rectangle-label file-label\" text-anchor=\"middle\" dominant-baseline=\"middle\" pointer-events=\"none\">{lines}</text>")
    };
    Some((text, bounds))
}

fn bevel(out: &mut Vec<String>, rect: Rect) {
    out.push(format!("<path class=\"bevel-light\" d=\"M{} {} V{} H{}\" fill=\"none\" stroke=\"#c9e1dc\" stroke-width=\"1\" vector-effect=\"non-scaling-stroke\"/>", rect.x, rect.y + rect.height, rect.y, rect.x + rect.width));
    out.push(format!("<path class=\"bevel-dark\" d=\"M{} {} H{} V{}\" fill=\"none\" stroke=\"#103840\" stroke-width=\"1\" vector-effect=\"non-scaling-stroke\"/>", rect.x, rect.y + rect.height, rect.x + rect.width, rect.y));
}

fn base64(data: &[u8]) -> String {
    const TABLE: &[u8] = b"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    let mut out = String::new();
    for chunk in data.chunks(3) {
        let n = (u32::from(chunk[0]) << 16)
            | (u32::from(*chunk.get(1).unwrap_or(&0)) << 8)
            | u32::from(*chunk.get(2).unwrap_or(&0));
        out.push(TABLE[(n >> 18 & 63) as usize] as char);
        out.push(TABLE[(n >> 12 & 63) as usize] as char);
        out.push(if chunk.len() > 1 {
            TABLE[(n >> 6 & 63) as usize] as char
        } else {
            '='
        });
        out.push(if chunk.len() > 2 {
            TABLE[(n & 63) as usize] as char
        } else {
            '='
        });
    }
    out
}

fn titles(tree: &str) -> (&'static str, &'static str) {
    match tree {
        "core" => ("Main-image code", "Main game"),
        "overlays" => ("Decoded code-overlay", "Code overlays"),
        "music" => ("Sound", "Sound"),
        "images" => ("Graphics", "Graphics"),
        _ => ("ROM contents", "ROM contents"),
    }
}
fn esc(value: &str) -> String {
    value
        .replace('&', "&amp;")
        .replace('<', "&lt;")
        .replace('>', "&gt;")
        .replace('"', "&quot;")
}
fn fill(category: Category) -> String {
    let color = match category {
        Category::Unknown => UNKNOWN,
        Category::DraftAsm => "#b4ccd2",
        Category::DraftC => "#96c8c9",
        Category::ProvenAsm => "#6cafb2",
        Category::ProvenC => "#326b7d",
        Category::AssetData => "#92a8ac",
    };
    format!("fill:{color}")
}
fn tree_tiles<'a>(map: &'a CoverageMap, tree: &str) -> (&'a Area, Vec<&'a Tile>) {
    let area = match tree {
        "core" => &map.executable_areas[0],
        "overlays" => &map.executable_areas[1],
        _ => map
            .rom_areas
            .iter()
            .find(|a| a.id == "rom-data")
            .unwrap_or(&map.rom_areas[0]),
    };
    let tiles = area
        .tiles
        .iter()
        .filter(|tile| {
            if !matches!(tree, "images" | "music") {
                return true;
            }
            let sound = matches!(
                tile.group.as_deref(),
                Some(
                    "golden-sun-sound-sequence"
                        | "golden-sun-sound-table"
                        | "golden-sun-pcm-wave"
                        | "golden-sun-audio-engine-data"
                        | "golden-sun-music-residual"
                )
            );
            sound == (tree == "music")
        })
        .collect();
    (area, tiles)
}
pub fn svg(tree: &str, map: &CoverageMap, width: f64) -> String {
    let (_area, tiles) = tree_tiles(map, tree);
    let (description, title) = titles(tree);
    let edge = CHART_BACKGROUND;
    let assets = matches!(tree, "images" | "music");
    let frame = Rect {
        x: 4.0,
        y: 32.0,
        width: width - 8.0,
        height: (width * 258.0 / 540.0).max(260.0),
    };
    let mut out = vec![format!("<title>{}</title>", esc(title))];
    if let Ok(bytes) = std::fs::read(root().join("games/gs1/assets/fonts/weyard.otf")) {
        out.push(format!("<defs><style>@font-face{{font-family:Weyard;src:url(data:font/otf;base64,{}) format('opentype');font-style:italic;}}.weyard{{font-family:Weyard;font-size:16px;font-style:italic;fill:#fff;text-shadow:1px 1px 0 #000;}}</style></defs>", base64(&bytes)));
    } else {
        out.push("<style>.weyard{font-family:monospace;font-size:16px;fill:#fff;text-shadow:1px 1px 0 #000;}</style>".into());
    }
    out.push(format!(
        "<text class=\"weyard\" x=\"8\" y=\"22\">{}</text>",
        esc(title)
    ));
    let displayed_bytes: i64 = tiles.iter().map(|tile| tile.bytes).sum();
    let done_bytes: i64 = tiles
        .iter()
        .map(|tile| {
            tile.categories[Category::ProvenC as usize]
                + tile.categories[Category::ProvenAsm as usize]
        })
        .sum();
    let corner = if matches!(tree, "core" | "overlays") {
        format!(
            "{:.2}% DONE",
            100.0 * done_bytes as f64 / displayed_bytes.max(1) as f64
        )
    } else {
        commas(displayed_bytes)
    };
    out.push(format!(
        "<text class=\"weyard\" x=\"{}\" y=\"22\" text-anchor=\"end\">{}</text>",
        width - 8.0,
        corner
    ));
    out.push(format!(
        "<rect x=\"{}\" y=\"{}\" width=\"{}\" height=\"{}\" fill=\"#fff\"/>",
        frame.x, frame.y, frame.width, frame.height
    ));
    let nested = directories(tiles.iter().map(|tile| (*tile).clone()).collect(), "");
    draw_tiles(
        &mut out,
        tree,
        &nested.iter().collect::<Vec<_>>(),
        frame,
        None,
        &[],
    );
    let mut legend_x = 8.0;
    let mut legend_y = frame.y + frame.height + 12.0;
    let legend: Vec<_> = if assets {
        let mut totals = std::collections::BTreeMap::new();
        for tile in leaves(&tiles) {
            *totals.entry(content_style(tile)).or_insert(0_i64) += tile.bytes;
        }
        totals
            .into_iter()
            .map(|((name, color), bytes)| (name, format!("fill:{color}"), bytes))
            .collect()
    } else {
        CATEGORIES
            .iter()
            .map(|(category, _, name)| {
                (
                    *name,
                    fill(*category),
                    tiles
                        .iter()
                        .map(|tile| tile.categories[*category as usize])
                        .sum(),
                )
            })
            .collect()
    };
    for (name, color, category_bytes) in legend {
        if category_bytes <= 0 || displayed_bytes <= 0 {
            continue;
        }
        let percentage = 100.0 * category_bytes as f64 / displayed_bytes as f64;
        let display = format!("{name} {percentage:.1}%");
        let label_width = 24.0 + display.chars().count() as f64 * 8.0;
        if legend_x > 8.0 && legend_x + label_width > width - 8.0 {
            legend_x = 8.0;
            legend_y += 24.0;
        }
        out.push(format!(
            "<rect x=\"{}\" y=\"{}\" width=\"16\" height=\"16\" style=\"{}\"/>",
            legend_x, legend_y, color
        ));
        out.push(format!(
            "<text class=\"weyard legend-label\" x=\"{}\" y=\"{}\" dominant-baseline=\"middle\">{}</text>",
            legend_x + 20.0,
            legend_y + 8.0,
            esc(&display)
        ));
        legend_x += label_width;
    }
    let height = (legend_y + 24.0).ceil();
    let mut rendered = vec![format!("<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 {width} {height}\" width=\"{width}\" height=\"{height}\" shape-rendering=\"crispEdges\" role=\"img\" aria-label=\"{description} box tree\">"), format!("<rect x=\"0\" y=\"0\" width=\"{width}\" height=\"{height}\" fill=\"{edge}\"/>")];
    rendered.extend(out);
    rendered.push("<g class=\"chart-frame\" pointer-events=\"none\">".into());
    bevel(
        &mut rendered,
        Rect {
            x: 0.5,
            y: 0.5,
            width: width - 1.0,
            height: height - 1.0,
        },
    );
    rendered.push("</g></svg>".into());
    rendered.join("\n") + "\n"
}

pub fn render_box_trees(
    map: &CoverageMap,
    _tree: Option<&crate::tree::SourceTree>,
    _verify: bool,
) -> Result<Vec<(&'static str, String)>, String> {
    Ok(BOX_TREES
        .iter()
        .map(|tree| (*tree, svg(tree, map, 540.0)))
        .collect())
}
pub fn svg_cache_version(svg: &str) -> String {
    sha1_hex(svg.as_bytes())[..16].into()
}
pub fn box_tree_path(target: &str, tree: &str) -> std::path::PathBuf {
    root()
        .join("games/gs1/assets/readme")
        .join(format!("{target}-{tree}.svg"))
}

#[cfg(test)]
mod tests {
    use super::{
        content_style, directories, draw_tiles, leaves, sound_type, svg, tree_tiles, SOUND_TYPES,
    };
    use crate::model::{Area, Category, Rect, Tile};
    use crate::pipeline::CoverageMap;
    use serde_json::Value;

    #[test]
    fn all_directories_wrap_files_without_duplicating_bytes() {
        let tile = Tile {
            source: Some("games/gs1/src/battle/effects/fire.c".into()),
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
            "games/gs1/",
            "games/gs1/src/",
            "games/gs1/src/battle/",
            "games/gs1/src/battle/effects/",
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
    fn folders_have_one_pixel_bevels_and_files_have_only_fitting_centered_labels() {
        let tile = Tile {
            source: Some("sound/wave.wav".into()),
            bytes: 100,
            categories: [0, 0, 0, 0, 0, 100],
            ..Tile::default()
        };
        let grouped = directories(vec![tile], "");
        for (width, height, visible) in
            [(200.0, 100.0, true), (40.0, 20.0, false), (8.0, 8.0, false)]
        {
            let mut out = Vec::new();
            draw_tiles(
                &mut out,
                "music",
                &grouped.iter().collect::<Vec<_>>(),
                Rect {
                    x: 0.0,
                    y: 0.0,
                    width,
                    height,
                },
                None,
                &[],
            );
            let svg = out.join("\n");
            assert_eq!(svg.matches("data-kind=\"folder\"").count(), 1);
            assert!(svg.contains("class=\"bevel-light\""));
            assert!(svg.contains("class=\"bevel-dark\""));
            assert_eq!(
                svg.matches("stroke-width=\"1\" vector-effect=\"non-scaling-stroke\"")
                    .count(),
                2
            );
            assert_eq!(svg.contains(">wave.wav</tspan>"), visible);
            assert_eq!(
                svg.contains("text-anchor=\"middle\" dominant-baseline=\"middle\""),
                visible
            );
            assert!(!svg.contains("height=\"10\""));
            assert!(!svg.contains("..."));
        }
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
    fn graphics_and_sound_partition_assets_by_type_not_label() {
        for (kind, sound) in [
            ("golden-sun-sound-sequence", true),
            ("golden-sun-sound-table", true),
            ("golden-sun-pcm-wave", true),
            ("golden-sun-audio-engine-data", true),
            ("golden-sun-music-residual", true),
            ("gba-palette", false),
            ("", false),
        ] {
            let mut map = CoverageMap {
                document: Value::Null,
                executable_areas: Vec::new(),
                rom_areas: vec![Area {
                    id: "rom-data".into(),
                    tiles: vec![Tile {
                        label: if sound {
                            "se_197.mid"
                        } else {
                            "sound_wave.png"
                        }
                        .into(),
                        group: (!kind.is_empty()).then(|| kind.into()),
                        address: Some(0x08182830),
                        bytes: 564,
                        categories: [0, 0, 0, 0, 0, 564],
                        ..Tile::default()
                    }],
                    ..Area::default()
                }],
            };
            for (tree, title, included) in
                [("images", "Graphics", !sound), ("music", "Sound", sound)]
            {
                assert_eq!(
                    tree_tiles(&map, tree).1.len(),
                    usize::from(included),
                    "{kind}: {tree}"
                );
                let rendered = svg(tree, &map, 540.0);
                assert!(rendered.contains("fill=\"#1f7f93\""));
                assert!(rendered.contains("fill:#fff;text-shadow:1px 1px 0 #000;"));
                assert!(rendered.contains(&format!("<title>{title}</title>")));
                assert_eq!(rendered.contains("data-address=\"0x08182830\""), included);
                if tree == "music" && included {
                    let (name, color) = SOUND_TYPES[sound_type(&map.rom_areas[0].tiles[0])];
                    assert!(rendered.contains(&format!("style=\"fill:{color}\"")));
                    assert!(rendered.contains(&format!("{name} 100.0%")));
                    assert!(!rendered.contains("Data 100.0%"));
                }
            }
            let child = map.rom_areas[0].tiles[0].clone();
            let parent = &mut map.rom_areas[0].tiles[0];
            parent.children = vec![child];
            parent.label = "catalog.tsv".into();
            parent.address = None;
            let tree = if sound { "music" } else { "images" };
            let rendered = svg(tree, &map, 540.0);
            assert!(rendered.contains("data-node=\"container\""));
            assert!(rendered.contains("data-node=\"leaf\""));
            assert_eq!(
                leaves(&tree_tiles(&map, tree).1)
                    .iter()
                    .map(|tile| tile.bytes)
                    .sum::<i64>(),
                564
            );
        }
    }

    #[test]
    fn code_tree_uses_five_ordered_progress_states_and_done_corner() {
        let mut categories = [0; 6];
        categories[Category::Unknown as usize] = 25;
        categories[Category::DraftAsm as usize] = 15;
        categories[Category::DraftC as usize] = 10;
        categories[Category::ProvenAsm as usize] = 25;
        categories[Category::ProvenC as usize] = 25;
        let area = Area {
            id: "main-code".into(),
            label: "Main game".into(),
            bytes: 100,
            categories,
            tiles: vec![Tile {
                label: "owner".into(),
                bytes: 100,
                categories,
                address: Some(0x080bbb0c),
                ..Tile::default()
            }],
        };
        let map = CoverageMap {
            document: Value::Null,
            rom_areas: Vec::new(),
            executable_areas: vec![area.clone(), area],
        };
        let rendered = svg("core", &map, 540.0);
        for tree in ["core", "overlays"] {
            let chart = svg(tree, &map, 540.0);
            assert!(chart.contains("fill=\"#1f7f93\""));
            assert!(chart.contains("class=\"chart-frame\""));
            assert!(!chart.contains("rx="));
            assert!(!chart.contains("font-size:8px"));
            assert!(!chart.contains("font-size:12px"));
            assert!(chart.contains("fill:#fff;text-shadow:1px 1px 0 #000;"));
        }
        assert!(rendered.contains("50.00% DONE"));
        let unknown = rendered.find("Unknown 25.0%").unwrap();
        let draft_asm = rendered.find("Draft ASM 15.0%").unwrap();
        let draft_c = rendered.find("Draft C 10.0%").unwrap();
        let proven_asm = rendered.find("Proven ASM 25.0%").unwrap();
        let proven_c = rendered.find("Proven C 25.0%").unwrap();
        assert!(unknown < draft_asm);
        assert!(draft_asm < draft_c);
        assert!(draft_c < proven_asm);
        assert!(proven_asm < proven_c);
        assert!(rendered.contains("legend-label"));
        assert!(rendered.contains("data-address=\"0x080bbb0c\""));
        assert!(rendered.contains("<title>owner: 100 bytes</title>"));
    }
}
