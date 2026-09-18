use super::jsnum::commas;
use crate::coverage::model::{treemap, Category, Rect, Tile, UNIDENTIFIED};

const DISPLAY_CATEGORIES: [(Category, &str); 5] = [
    (Category::Unknown, UNIDENTIFIED),
    (Category::DraftC, "Drafted"),
    (Category::ProvenAsm, "Assembly"),
    (Category::ProvenC, "C"),
    (Category::AssetData, "Data"),
];

fn display_bytes(categories: &[i64; 6], category: Category) -> i64 {
    categories[category as usize]
        + if category == Category::ProvenAsm {
            categories[Category::DraftAsm as usize]
        } else {
            0
        }
}
use crate::coverage::pipeline::{source_container, CoverageMap};
use crate::coverage::tree::root;
use sha1::{Digest, Sha1};
mod html;
pub use html::{decode_folder, page as html_page};
pub use html::{file_page, rom_page};

pub const BOX_TREES: [&str; 1] = ["rom"];
const CHART_BACKGROUND: &str = "#1f7f93";
const UNKNOWN: &str = "#d9d9d4";
const SOUND_TYPES: [(&str, &str); 5] = [
    ("MIDI music", "#81d6b2"),
    ("SFX", "#f29b91"),
    ("PCM samples", "#efbb82"),
    ("Tables", "#9aa4c2"),
    (UNIDENTIFIED, UNKNOWN),
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
fn content_style(tile: &Tile) -> (&'static str, &'static str) {
    let group = tile.group.as_deref().unwrap_or("");
    if let Some(extension) = group.strip_prefix("file:") {
        return match extension {
            "c" => ("C", "#f0c57d"),
            "h" | "inc" => ("Headers", "#eadb83"),
            "s" => ("Assembly", "#78afb7"),
            "png" => ("Images", "#8fb7ec"),
            "wav" => ("Audio", "#efbb82"),
            "mid" => ("Music", "#81d6b2"),
            "json" | "tsv" => ("Metadata", "#9aa4c2"),
            "bin" => ("Binary inputs", "#b5cc82"),
            _ => ("Other files", UNKNOWN),
        };
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
        "gba-tilemap16" => ("Tile maps", "#b5cc82"),
        "mixed-data" | "components" => ("Mixed data", "#c4b4b7"),
        "golden-sun-general-lz" | "golden-sun-kind2-lz" => ("Compressed data", "#c4b4b7"),
        "u8-array" | "s8-array" | "le-u16-array" | "le-u32-array" | "be-s16-array" => {
            ("Tables", "#9aa4c2")
        }
        "1bpp-tiles" | "1bpp-rows" => ("Images", "#8fb7ec"),
        "golden-sun-static-sprite-series" => ("Sprite sheets", "#b5a0de"),
        "gba-4bpp-tiles" | "gba-8bpp-tiles" | "golden-sun-delta7-still" => ("Images", "#8fb7ec"),
        "compressed-resource" => ("Compressed data", "#c4b4b7"),
        "gba-palette" | "gba-palette-rgba" | "bgr555-banks" => ("Palettes", "#e8a6d3"),
        "golden-sun-kana-glyph-bank" | "golden-sun-namae-nyuuryoku" => ("Fonts", "#eadb83"),
        "golden-sun-message-archive" | "golden-sun-staff-roll" => ("Text", "#85cbd2"),
        _ if source.contains("/fonts_") || source.contains("/GRAPHICS/FONT/") => {
            ("Fonts", "#eadb83")
        }
        _ if source.to_ascii_lowercase().ends_with(".png") => ("Images", "#8fb7ec"),
        "typed-table" | "record-table" | "pointer-table" => ("Tables", "#9aa4c2"),
        "gba-cartridge-header-standard-fields" => ("ROM header", "#eadb83"),
        "byte-fill" => ("Padding", "#bda995"),
        _ => (UNIDENTIFIED, UNKNOWN),
    }
}
fn asset_note(tile: &Tile, verification: Option<&str>) -> &'static str {
    if tile
        .group
        .as_deref()
        .is_some_and(|kind| kind.starts_with("indexed-"))
    {
        return " · ROM index: format evidence only; no reconstructed source or DONE credit";
    }
    if tile.group.as_deref() == Some("unreconstructed-data") {
        return " · No reconstructed source; not asset-build verified";
    }
    match verification {
        Some("rom") => " · Last asset build: ROM bytes matched; appearance not verified",
        Some("source_only") => " · Last asset build: not compared with ROM",
        _ => " · Asset verification unavailable",
    }
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
        assert!(!asset_note(&tile, Some("rom")).contains("ROM bytes matched"));
        assert!(asset_note(&tile, Some("rom")).contains("no reconstructed source"));
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
/// One vocabulary, palette and byte total for the HTML and README legends.
fn legend_items(tiles: &[&Tile]) -> Vec<(&'static str, &'static str, i64)> {
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
fn source_name(source: &str) -> &str {
    let trimmed = source.trim_end_matches('/');
    trimmed.rsplit('/').next().unwrap_or(trimmed)
}

fn draw_tiles(
    out: &mut Vec<String>,
    tiles: &[&Tile],
    frame: Rect,
    parent_source: Option<&str>,
    reserved: &[Rect],
    asset_verification: Option<&str>,
) {
    for placed in treemap(tiles, |tile| tile.bytes, frame) {
        let tile = tiles[placed.index];
        let rect = placed.rect;
        let container = !tile.children.is_empty();
        let folder = container
            && tile
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
        let mut status = DISPLAY_CATEGORIES
            .iter()
            .filter(|(category, _)| display_bytes(&tile.categories, *category) > 0)
            .map(|(category, name)| match category {
                Category::AssetData if !folder => content_style(tile).0,
                _ => *name,
            })
            .collect::<Vec<_>>()
            .join(", ");
        if !folder && tile.categories[Category::AssetData as usize] == tile.bytes {
            status.push_str(asset_note(tile, asset_verification));
        }
        let label = format!(
            "{}: {} bytes · {}{}{}",
            if folder {
                source_name(tile.source.as_deref().unwrap())
            } else {
                &tile.label
            },
            commas(tile.bytes),
            status,
            if container {
                format!(" · {} items", tile.children.len())
            } else {
                String::new()
            },
            tile.address
                .map(|address| format!(" · 0x{address:08x}"))
                .unwrap_or_default()
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
        } else if tile.categories[Category::AssetData as usize] == tile.bytes {
            let (name, color) = content_style(tile);
            out.push(format!("<rect data-content-type=\"{name}\" x=\"{}\" y=\"{}\" width=\"{}\" height=\"{}\" style=\"fill:{color}\"/>", body.x, body.y, body.width, body.height));
        } else {
            let mut y = body.y + body.height;
            for (category, _) in DISPLAY_CATEGORIES {
                let n = display_bytes(&tile.categories, category);
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
        let name = match tile.source.as_deref() {
            Some(source) if !source.ends_with('/') || container => source_name(source),
            Some(_) => &tile.label,
            None => "",
        };
        let caption = caption(name, body, folder).filter(|(_, bounds)| {
            (tile.source.is_none()
                || tile.source.as_deref() != parent_source
                || (!container
                    && tile
                        .source
                        .as_deref()
                        .is_some_and(|path| path.ends_with('/'))))
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
                &tile.children.iter().collect::<Vec<_>>(),
                body,
                tile.source.as_deref(),
                &reserved,
                asset_verification,
            );
        }
        if let Some((text, _)) = caption.filter(|_| !folder) {
            out.push(text);
        }
        out.push("</g>".into());
    }
}

/// One advance per character for 13px system sans labels, wide enough for
/// the upper-case folder names; SF Pro capitals average about 8px.
const LABEL_ADVANCE: f64 = 8.0;

fn label_width(name: &str) -> f64 {
    name.chars().count() as f64 * LABEL_ADVANCE
}
fn caption(name: &str, body: Rect, folder: bool) -> Option<(String, Rect)> {
    if folder {
        let width = label_width(name);
        if name.is_empty() || body.width < width + 4.0 || body.height < 20.0 {
            return None;
        }
        let bounds = Rect {
            x: body.x + 2.0,
            y: body.y,
            width,
            height: 18.0,
        };
        return Some((format!("<text class=\"label rectangle-label folder-label\" x=\"{}\" y=\"{}\" pointer-events=\"none\">{}</text>", bounds.x, body.y + 14.0, esc(name)), bounds));
    }
    let columns = ((body.width - 8.0) / LABEL_ADVANCE).max(0.0) as usize;
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
        * LABEL_ADVANCE;
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
        format!("<text class=\"label rectangle-label folder-label\" x=\"{}\" y=\"{}\" pointer-events=\"none\">{}</text>", bounds.x, body.y + 16.0, esc(name))
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
        format!("<text class=\"label rectangle-label file-label\" text-anchor=\"middle\" dominant-baseline=\"middle\" pointer-events=\"none\">{lines}</text>")
    };
    Some((text, bounds))
}

fn bevel(out: &mut Vec<String>, rect: Rect) {
    out.push(format!("<path class=\"bevel-light\" d=\"M{} {} V{} H{}\" fill=\"none\" stroke=\"#c9e1dc\" stroke-width=\"1\" vector-effect=\"non-scaling-stroke\"/>", rect.x, rect.y + rect.height, rect.y, rect.x + rect.width));
    out.push(format!("<path class=\"bevel-dark\" d=\"M{} {} H{} V{}\" fill=\"none\" stroke=\"#103840\" stroke-width=\"1\" vector-effect=\"non-scaling-stroke\"/>", rect.x, rect.y + rect.height, rect.x + rect.width, rect.y));
}

pub(crate) fn esc(value: &str) -> String {
    value
        .replace('&', "&amp;")
        .replace('<', "&lt;")
        .replace('>', "&gt;")
        .replace('"', "&quot;")
}
fn color(category: Category) -> &'static str {
    match category {
        Category::Unknown => UNKNOWN,
        Category::DraftAsm => "#b4ccd2",
        Category::DraftC => "#96c8c9",
        Category::ProvenAsm => "#6cafb2",
        Category::ProvenC => "#326b7d",
        Category::AssetData => "#92a8ac",
    }
}
fn fill(category: Category) -> String {
    format!("fill:{}", color(category))
}
fn tree_tiles(map: &CoverageMap) -> Vec<&Tile> {
    map.rom_areas
        .iter()
        .flat_map(|area| area.tiles.iter())
        .collect()
}
pub fn svg(tree: &str, map: &CoverageMap, width: f64) -> String {
    svg_at(tree, map, width, "")
}

pub fn svg_at(tree: &str, map: &CoverageMap, width: f64, folder: &str) -> String {
    svg_sized(tree, map, width, width * 16.0 / 9.0, folder)
}
pub fn svg_sized(tree: &str, map: &CoverageMap, width: f64, height: f64, folder: &str) -> String {
    assert_eq!(tree, "rom", "only the unified ROM viewer is supported");
    let shared = map.document["shared_map_assets"][folder]
        .as_array()
        .cloned()
        .unwrap_or_default();
    let tiles: Vec<_> = leaves(&tree_tiles(map))
        .into_iter()
        .filter(|tile| {
            folder.is_empty()
                || tile
                    .source
                    .as_deref()
                    .is_some_and(|source| source.starts_with(folder))
        })
        .collect();
    let description = "ROM contents";
    let identity = map.document["target"]
        .as_str()
        .map(|target| target.replace('-', " ").to_uppercase());
    let title = if folder.is_empty() {
        "Alchemy".into()
    } else {
        let location = source_name(folder);
        identity.map_or_else(|| location.into(), |id| format!("{id} · {location}"))
    };
    let displayed_bytes: i64 = tiles.iter().map(|tile| tile.bytes).sum();
    let labels: Vec<_> = legend_items(&tiles)
        .into_iter()
        .map(|(name, color, bytes)| {
            let percent = 100.0 * bytes as f64 / displayed_bytes.max(1) as f64;
            (format!("{name} {percent:.1}%"), format!("fill:{color}"))
        })
        .collect();
    let mut row_width = 8.0;
    let mut rows = 1;
    for (label, _) in &labels {
        let size = 24.0 + label_width(label);
        if row_width > 8.0 && row_width + size > width - 8.0 {
            rows += 1;
            row_width = 8.0;
        }
        row_width += size;
    }
    let frame = Rect {
        x: 4.0,
        y: 32.0,
        width: width - 8.0,
        height: height - 44.0 - rows as f64 * 24.0 - if shared.is_empty() { 0.0 } else { 24.0 },
    };
    let mut out = vec![format!("<title>{}</title>", esc(&title))];
    out.push("<style>.label{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Helvetica,Arial,sans-serif;font-size:13px;fill:#fff;text-shadow:1px 1px 0 #000;}</style>".into());
    out.push(format!(
        "<svg x=\"{}\" y=\"0\" width=\"{}\" height=\"30\" overflow=\"hidden\"><text class=\"label\" x=\"0\" y=\"22\">{}</text></svg>",
        if folder.is_empty() { 8 } else { 36 },
        (width - if folder.is_empty() { 112.0 } else { 140.0 }).max(0.0),
        esc(&title)
    ));
    let corner = commas(displayed_bytes);
    out.push(format!(
        "<text class=\"label\" x=\"{}\" y=\"22\" text-anchor=\"end\">{}</text>",
        width - 8.0,
        corner
    ));
    out.push(format!(
        "<rect x=\"{}\" y=\"{}\" width=\"{}\" height=\"{}\" fill=\"#fff\"/>",
        frame.x, frame.y, frame.width, frame.height
    ));
    let nested = directories(tiles.iter().map(|tile| (*tile).clone()).collect(), folder);
    draw_tiles(
        &mut out,
        &nested.iter().collect::<Vec<_>>(),
        frame,
        None,
        &[],
        map.document
            .get("asset_verification")
            .and_then(serde_json::Value::as_str),
    );
    let mut legend_x = 8.0;
    let mut legend_y = frame.y + frame.height + 12.0;
    if !shared.is_empty() {
        out.push(format!("<g data-action=\"shared\" role=\"button\" tabindex=\"0\" aria-label=\"Show shared map files\"><text class=\"label\" x=\"8\" y=\"{}\">Shared files ({})</text></g>", legend_y + 12.0, shared.len()));
        legend_y += 24.0;
    }
    for (display, color) in labels {
        let entry_width = 24.0 + label_width(&display);
        if legend_x > 8.0 && legend_x + entry_width > width - 8.0 {
            legend_x = 8.0;
            legend_y += 24.0;
        }
        out.push(format!(
            "<rect x=\"{}\" y=\"{}\" width=\"16\" height=\"16\" style=\"{}\"/>",
            legend_x, legend_y, color
        ));
        out.push(format!(
            "<text class=\"label legend-label\" x=\"{}\" y=\"{}\" dominant-baseline=\"middle\">{}</text>",
            legend_x + 20.0,
            legend_y + 8.0,
            esc(&display)
        ));
        legend_x += entry_width;
    }
    let mut rendered = vec![format!("<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 {width} {height}\" width=\"{width}\" height=\"{height}\" shape-rendering=\"crispEdges\" role=\"img\" aria-label=\"{description} box tree\">"), format!("<rect x=\"0\" y=\"0\" width=\"{width}\" height=\"{height}\" fill=\"{CHART_BACKGROUND}\"/>")];
    rendered.extend(out);
    rendered.insert(
        1,
        format!(
            "<metadata data-shared=\"{}\" data-usage-revision=\"{}\"/>",
            esc(&serde_json::to_string(&shared).unwrap()),
            svg_cache_version(&map.document["shared_map_assets"].to_string())
        ),
    );
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

pub fn render_box_trees(map: &CoverageMap) -> Vec<(&'static str, String)> {
    BOX_TREES
        .iter()
        .map(|tree| (*tree, svg(tree, map, 830.0)))
        .collect()
}
pub fn svg_cache_version(svg: &str) -> String {
    format!("{:x}", Sha1::digest(svg.as_bytes()))[..16].into()
}

#[test]
fn content_version_uses_standard_sha1_prefix() {
    assert_eq!(svg_cache_version("abc"), "a9993e364706816a");
}
pub fn box_tree_path(target: &str, tree: &str) -> std::path::PathBuf {
    if target == "tbs-en" && tree == "rom" {
        root().join("PROGRESS.svg")
    } else {
        root().join(format!("out/{target}/reports/{tree}.svg"))
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn folder_names_fit_at_one_label_advance() {
        let body = super::Rect {
            x: 0.0,
            y: 0.0,
            width: 28.0,
            height: 20.0,
        };
        let (text, bounds) = super::caption("LIB", body, true).unwrap();
        assert_eq!(bounds.width, 24.0);
        assert!(text.contains(">LIB</text>"));
        assert_eq!(super::caption("WWW", body, true).unwrap().1.width, 24.0);
        assert!(super::caption("LIBS", body, true).is_none());
        assert!(super::caption(
            "LIB",
            super::Rect {
                height: 19.0,
                ..body
            },
            true
        )
        .is_none());
        assert!(super::caption("LIB", body, false).is_none());
    }
    use super::{
        content_style, directories, draw_tiles, leaves, sound_type, svg, tree_tiles, BOX_TREES,
        SOUND_TYPES,
    };
    use crate::coverage::model::{Area, Category, Rect, Tile};
    use crate::coverage::pipeline::CoverageMap;
    use serde_json::Value;

    #[test]
    fn file_details_expose_mixed_status_without_claiming_all_bytes_are_c() {
        let map = CoverageMap {
            document: Value::Null,
            executable_areas: vec![],
            rom_areas: vec![Area {
                tiles: vec![Tile {
                    label: "Battle loop".into(),
                    source: Some("MAIN.C".into()),
                    bytes: 100,
                    categories: [10, 20, 30, 0, 40, 0],
                    ..Tile::default()
                }],
                ..Area::default()
            }],
        };
        let rendered = super::svg_at("rom", &map, 540.0, "");
        assert!(rendered.contains("Unidentified, Drafted, Assembly, C"));
        assert!(!rendered.contains("Proven ASM"));
        assert!(!rendered.contains("Draft ASM"));
    }

    #[test]
    fn folder_view_uses_only_its_files_without_scaling_the_font() {
        let map = CoverageMap {
            document: serde_json::json!({"target":"tbs-en", "shared_map_assets": {
                "FIELD/XIAN/": ["GRAPHICS/TILE/SHARED.PNG"]
            }}),
            executable_areas: vec![],
            rom_areas: vec![Area {
                tiles: vec![super::source_container(
                    "PACK.json".into(),
                    ["FIELD/XIAN/ROOMS.C", "FIELD/HEIDIA/ROOMS.C"]
                        .iter()
                        .map(|path| Tile {
                            source: Some((*path).into()),
                            bytes: 100,
                            categories: [100, 0, 0, 0, 0, 0],
                            ..Tile::default()
                        })
                        .collect(),
                )],
                ..Area::default()
            }],
        };
        let rendered = super::svg_at("rom", &map, 540.0, "FIELD/XIAN/");
        assert!(rendered.contains("FIELD/XIAN/ROOMS.C"));
        assert!(!rendered.contains("HEIDIA"));
        assert!(rendered.contains("font-size:13px"));
        assert!(rendered.contains("sans-serif"));
        for embedded in ["base64", "@font-face", "data:", "url("] {
            assert!(!rendered.contains(embedded), "{embedded}");
        }
        assert!(rendered.contains("viewBox=\"0 0 540 960\""));
        assert!(rendered.contains("TBS EN · XIAN"));
        assert!(rendered.contains("Shared files (1)"));
        assert!(rendered.contains("GRAPHICS/TILE/SHARED.PNG"));
        assert!(rendered.contains("data-usage-revision="));
        assert!(!super::svg_at("rom", &map, 540.0, "FIELD/HEIDIA/").contains("Shared files"));
        assert!(super::svg_at("rom", &map, 320.0, "").contains("<title>Alchemy</title>"));
    }

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
    fn unreconstructed_data_has_no_fictitious_file_or_extra_folder() {
        let pending = Tile {
            label: "Unidentified".into(),
            source: Some("games/THE LOST AGE/".into()),
            group: Some("unreconstructed-data".into()),
            bytes: 100,
            categories: [0, 0, 0, 0, 0, 100],
            ..Tile::default()
        };
        let file = Tile {
            label: "DIRECTORY.JSON".into(),
            source: Some("games/THE LOST AGE/SRC/SYSTEM/RESOURCE/DIRECTORY.JSON".into()),
            bytes: 20,
            categories: [0, 0, 0, 0, 0, 20],
            ..Tile::default()
        };
        let grouped = directories(vec![pending.clone(), file], "");
        let game = &grouped[0].children[0];
        assert_eq!(game.source.as_deref(), Some("games/THE LOST AGE/"));
        assert_eq!(game.bytes, 120);
        assert_eq!(game.children.len(), 2);
        assert_eq!(game.children[0].label, "Unidentified");
        assert!(game.children[0].children.is_empty());
        let mut out = Vec::new();
        draw_tiles(
            &mut out,
            &[&pending],
            Rect {
                x: 0.0,
                y: 0.0,
                width: 400.0,
                height: 200.0,
            },
            None,
            &[],
            Some("rom"),
        );
        let rendered = out.join("\n");
        assert!(rendered.contains("Unidentified"));
        assert!(rendered.contains("not asset-build verified"));
        assert!(!rendered.contains("ROM bytes matched"));
        assert!(!rendered.contains("data-kind=\"folder\""));
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
                &grouped.iter().collect::<Vec<_>>(),
                Rect {
                    x: 0.0,
                    y: 0.0,
                    width,
                    height,
                },
                None,
                &[],
                Some("rom"),
            );
            let svg = out.join("\n");
            assert_eq!(svg.matches("data-kind=\"folder\"").count(), 1);
            assert_eq!(
                svg.matches("Last asset build: ROM bytes matched").count(),
                2
            );
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
        let map = CoverageMap {
            document: Value::Null,
            rom_areas: vec![Area {
                tiles,
                ..Area::default()
            }],
            executable_areas: vec![],
        };
        let svg = svg("rom", &map, 540.0);
        assert_eq!(svg.matches("Unidentified 30.0%").count(), 1);
        assert!(svg.contains("Tables 70.0%"));
        let html = super::html_page(&map, "", None, false).unwrap();
        let footer = html.split("<footer").nth(1).unwrap();
        assert_eq!(footer.matches(">Unidentified</span>").count(), 1);
        for obsolete in [
            "Unknown",
            "Unclassified",
            "Unreconstructed data",
            "Other data",
        ] {
            assert!(!footer.contains(obsolete), "{obsolete}");
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
    fn code_and_data_combine_all_members_without_double_counting() {
        let tile = |bytes, categories| Tile {
            bytes,
            categories,
            ..Tile::default()
        };
        let map = CoverageMap {
            document: Value::Null,
            executable_areas: vec![
                Area {
                    tiles: vec![tile(100, [0, 0, 0, 0, 100, 0])],
                    ..Area::default()
                },
                Area {
                    tiles: vec![tile(300, [0, 0, 300, 0, 0, 0])],
                    ..Area::default()
                },
            ],
            rom_areas: vec![
                Area {
                    id: "rom-data".into(),
                    tiles: vec![
                        Tile {
                            group: Some("golden-sun-pcm-wave".into()),
                            ..tile(40, [0, 0, 0, 0, 0, 40])
                        },
                        Tile {
                            source: Some("image.png".into()),
                            ..tile(60, [0, 0, 0, 0, 0, 60])
                        },
                    ],
                    ..Area::default()
                },
                Area {
                    id: "compressed-code".into(),
                    tiles: vec![tile(500, [0, 0, 0, 0, 0, 500])],
                    ..Area::default()
                },
            ],
        };
        assert_eq!(BOX_TREES, ["rom"]);
        // Physical streams, not decoded executable owners, determine ROM area.
        assert_eq!(tree_tiles(&map).iter().map(|t| t.bytes).sum::<i64>(), 600);
        for width in [320.0, 540.0, 830.0] {
            let rendered = svg("rom", &map, width);
            assert!(rendered.contains(&format!("viewBox=\"0 0 {width} {}\"", width * 16.0 / 9.0)));
            assert!(rendered.contains("font-size:13px"));
            assert!(!rendered.contains("DONE"));
            assert!(rendered.contains("PCM samples"));
        }
    }

    #[test]
    fn rom_tree_displays_code_status_without_redefining_done() {
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
            rom_areas: vec![area],
            executable_areas: Vec::new(),
        };
        let rendered = svg("rom", &map, 540.0);
        for tree in ["rom"] {
            let chart = svg(tree, &map, 540.0);
            assert!(chart.contains("fill=\"#1f7f93\""));
            assert!(chart.contains("class=\"chart-frame\""));
            assert!(!chart.contains("rx="));
            assert!(!chart.contains("font-size:8px"));
            assert!(!chart.contains("font-size:12px"));
            assert!(chart.contains("fill:#fff;text-shadow:1px 1px 0 #000;"));
        }
        assert!(!rendered.contains("DONE"));
        let unknown = rendered.find("Unidentified 25.0%").unwrap();
        let draft_c = rendered.find("Drafted 10.0%").unwrap();
        let proven_asm = rendered.find("Assembly 40.0%").unwrap();
        let proven_c = rendered.find("C 25.0%").unwrap();
        assert!(!rendered.contains("Draft ASM"));
        assert!(!rendered.contains("Proven ASM"));
        assert!(!rendered.contains("Proven C"));
        assert!(!rendered.contains("Draft C"));
        assert!(!rendered.contains("Exact C"));
        assert_eq!(rendered.matches("Assembly 40.0%").count(), 1);
        assert!(unknown < draft_c);
        assert!(draft_c < proven_asm);
        assert!(proven_asm < proven_c);
        assert!(rendered.contains("legend-label"));
        assert!(rendered.contains("data-address=\"0x080bbb0c\""));
        assert!(rendered.contains(
            "<title>owner: 100 bytes · Unidentified, Drafted, Assembly, C · 0x080bbb0c</title>"
        ));
        assert!(!rendered.contains(">owner</tspan>"));
        assert!(!rendered.contains(">0x080bbb0c</tspan>"));
    }
}
