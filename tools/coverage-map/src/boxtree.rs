use crate::jsnum::{commas, js_number_string};
use crate::model::{treemap, Area, Rect, Tile, CATEGORIES};
use crate::pipeline::CoverageMap;
use crate::sha1::sha1_hex;
use crate::tree::root;

pub const BOX_TREES: [&str; 4] = ["core", "overlays", "images", "music"];
const ASM: &str = "#7dd3fc";

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

fn hue(tree: &str) -> (f64, f64, &'static str, &'static str) {
    match tree {
        "core" => (275.0, 295.0, "#6d4fc2", "Main-image code"),
        "overlays" => (190.0, 200.0, "#1f7f93", "Decoded code-overlay"),
        "music" => (28.0, 55.0, "#c85d00", "Music maturity"),
        _ => (330.0, 355.0, "#bb2f77", "Images maturity"),
    }
}
fn esc(value: &str) -> String {
    value
        .replace('&', "&amp;")
        .replace('<', "&lt;")
        .replace('>', "&gt;")
        .replace('"', "&quot;")
}
fn label(text: &str, width: f64, height: f64) -> String {
    if width >= 90.0 && height >= 12.0 {
        text.to_string()
    } else {
        String::new()
    }
}
fn fill(tree: &str, category: &str, fraction: f64) -> String {
    if category == "retained_asm" {
        return format!("fill:{ASM}");
    }
    if category == "tracked_c" {
        return "fill:#f4c95d".into();
    }
    let (h, okh, _, _) = hue(tree);
    let f = fraction.clamp(0.0, 1.0);
    let light = 93.0 - 38.0 * f;
    let sat = (f * 95.0).round();
    format!(
        "fill:hsl({} {}% {}%);fill:oklch({:.3} {:.3} {})",
        js_number_string(h),
        js_number_string(sat),
        js_number_string(light.round()),
        0.93 - 0.38 * f,
        0.17 * f,
        js_number_string(okh)
    )
}
fn tile_fraction(tile: &Tile, category: &str) -> f64 {
    if tile.bytes <= 0 {
        0.0
    } else {
        tile.category(category) as f64 / tile.bytes as f64
    }
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
    let mut tiles: Vec<&Tile> = area.tiles.iter().collect();
    if tree == "music" {
        let filtered: Vec<_> = tiles
            .iter()
            .copied()
            .filter(|t| {
                let l = t.label.to_ascii_lowercase();
                l.contains("music")
                    || l.contains("sound")
                    || l.contains("pcm")
                    || l.contains("wave")
            })
            .collect();
        if !filtered.is_empty() {
            tiles = filtered;
        }
    }
    (area, tiles)
}
fn title(tree: &str, _area: &Area) -> String {
    let (_, _, _, name) = hue(tree);
    format!(
        "{} Series",
        if tree == "core" {
            "Map Changelog"
        } else if tree == "overlays" {
            "2nd"
        } else {
            name
        }
    )
}
fn svg(tree: &str, map: &CoverageMap) -> String {
    let (area, tiles) = tree_tiles(map, tree);
    let (h, _, edge, description) = hue(tree);
    let frame = Rect {
        x: 3.0,
        y: 22.0,
        width: 534.0,
        height: 258.0,
    };
    let mut out = vec![format!("<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 540 304\" width=\"540\" height=\"304\" shape-rendering=\"crispEdges\" role=\"img\" aria-label=\"{} box tree, {} band\">", description, edge)];
    if let Ok(bytes) = std::fs::read(root().join("games/gs1/assets/fonts/weyard.otf")) {
        out.push(format!("<defs><style>@font-face{{font-family:Weyard;src:url(data:font/otf;base64,{}) format('opentype');font-style:italic;}}.weyard{{font-family:Weyard;font-size:16px;font-style:italic;fill:#fff;}}.rectangle-label{{font-size:8px;}}</style></defs>", base64(&bytes)));
    } else {
        out.push("<style>.weyard{font-family:monospace;font-size:16px;fill:#fff}.rectangle-label{font-size:8px}</style>".into());
    }
    out.push(format!(
        "<rect x=\"0\" y=\"0\" width=\"540\" height=\"304\" fill=\"{}\" rx=\"8\"/>",
        edge
    ));
    out.push(format!("<rect x=\"1\" y=\"1\" width=\"538\" height=\"302\" fill=\"none\" stroke=\"{}\" stroke-width=\"2\" rx=\"7\"/>", edge));
    out.push(format!(
        "<text class=\"weyard\" x=\"6\" y=\"15\">{}</text>",
        esc(&title(tree, area))
    ));
    out.push(format!(
        "<text class=\"weyard\" x=\"534\" y=\"15\" text-anchor=\"end\">{}</text>",
        commas(area.bytes)
    ));
    out.push(format!(
        "<rect x=\"{}\" y=\"{}\" width=\"{}\" height=\"{}\" fill=\"#fff\"/>",
        frame.x, frame.y, frame.width, frame.height
    ));
    for placed in treemap(&tiles, |tile| tile.bytes, frame) {
        let tile = tiles[placed.index];
        let rect = placed.rect;
        out.push(format!(
            "<g aria-label=\"{}: {} bytes\">",
            esc(&tile.label),
            commas(tile.bytes)
        ));
        let body = Rect {
            x: rect.x + 0.5,
            y: rect.y + 0.5,
            width: (rect.width - 1.0).max(0.5),
            height: (rect.height - 1.0).max(0.5),
        };
        let mut y = body.y + body.height;
        for category in CATEGORIES {
            let n = tile.category(category);
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
                fill(tree, category, tile_fraction(tile, category))
            ));
        }
        let name = label(&tile.label, body.width - 6.0, body.height);
        if !name.is_empty() {
            out.push(format!("<rect x=\"{}\" y=\"{}\" width=\"{}\" height=\"10\" fill=\"hsl({} 70% 24%)\" fill-opacity=\".9\"/>", rect.x, rect.y, rect.width, js_number_string(h)));
            out.push(format!(
                "<text class=\"weyard rectangle-label\" x=\"{}\" y=\"{}\">{}</text>",
                rect.x + 3.0,
                rect.y + 8.0,
                esc(&name)
            ));
        }
        out.push("</g>".into());
    }
    out.push("</svg>".into());
    out.join("\n") + "\n"
}

pub fn render_box_trees(
    map: &CoverageMap,
    _tree: Option<&crate::tree::SourceTree>,
    _verify: bool,
) -> Result<Vec<(&'static str, String)>, String> {
    Ok(BOX_TREES
        .iter()
        .map(|tree| (*tree, svg(tree, map)))
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
pub fn readme_path() -> std::path::PathBuf {
    root().join("README.md")
}
