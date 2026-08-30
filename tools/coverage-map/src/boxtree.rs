use crate::jsnum::{commas, js_number_string};
use crate::model::{treemap, Area, Category, Rect, Tile, CATEGORIES};
use crate::pipeline::CoverageMap;
use crate::sha1::sha1_hex;
use crate::tree::root;

pub const BOX_TREES: [&str; 4] = ["core", "overlays", "images", "music"];
const UNKNOWN: &str = "#d8d9df";

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

fn style(tree: &str) -> (f64, f64, &'static str, &'static str, &'static str) {
    match tree {
        "core" => (275.0, 295.0, "#6d4fc2", "Main-image code", "Main game"),
        "overlays" => (
            190.0,
            200.0,
            "#1f7f93",
            "Decoded code-overlay",
            "Code overlays",
        ),
        "music" => (28.0, 55.0, "#c85d00", "Music maturity", "Music and sound"),
        "images" => (
            330.0,
            355.0,
            "#bb2f77",
            "Images maturity",
            "Images and data",
        ),
        _ => (330.0, 355.0, "#bb2f77", "Images maturity", "ROM contents"),
    }
}
fn esc(value: &str) -> String {
    value
        .replace('&', "&amp;")
        .replace('<', "&lt;")
        .replace('>', "&gt;")
        .replace('"', "&quot;")
}
fn fill(tree: &str, category: Category) -> String {
    let (h, _, edge, _, _) = style(tree);
    match category {
        Category::Unknown => format!("fill:{UNKNOWN}"),
        Category::DraftAsm => format!("fill:hsl({} 34% 74%)", js_number_string(h)),
        Category::DraftC => format!("fill:hsl({} 52% 82%)", js_number_string(h)),
        Category::ProvenAsm => {
            format!("fill:hsl({} 58% 66%)", js_number_string(h))
        }
        Category::ProvenC | Category::AssetData => format!("fill:{edge}"),
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
fn svg(tree: &str, map: &CoverageMap) -> String {
    let (_area, tiles) = tree_tiles(map, tree);
    let (h, _, edge, description, title) = style(tree);
    let frame = Rect {
        x: 3.0,
        y: 22.0,
        width: 534.0,
        height: 258.0,
    };
    let mut out = vec![format!("<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 540 304\" width=\"540\" height=\"304\" shape-rendering=\"crispEdges\" role=\"img\" aria-label=\"{} box tree, {} band\">", description, edge)];
    out.push(format!("<title>{}</title>", esc(title)));
    if let Ok(bytes) = std::fs::read(root().join("games/gs1/assets/fonts/weyard.otf")) {
        out.push(format!("<defs><style>@font-face{{font-family:Weyard;src:url(data:font/otf;base64,{}) format('opentype');font-style:italic;}}.weyard{{font-family:Weyard;font-size:16px;font-style:italic;fill:#fff;}}.legend-label{{font-size:12px;}}.rectangle-label{{font-size:8px;}}</style></defs>", base64(&bytes)));
    } else {
        out.push("<style>.weyard{font-family:monospace;font-size:16px;fill:#fff}.legend-label{font-size:12px}.rectangle-label{font-size:8px}</style>".into());
    }
    out.push(format!(
        "<rect x=\"0\" y=\"0\" width=\"540\" height=\"304\" fill=\"{}\" rx=\"8\"/>",
        edge
    ));
    out.push(format!("<rect x=\"1\" y=\"1\" width=\"538\" height=\"302\" fill=\"none\" stroke=\"{}\" stroke-width=\"2\" rx=\"7\"/>", edge));
    out.push(format!(
        "<text class=\"weyard\" x=\"6\" y=\"15\">{}</text>",
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
            "{:.1}% DONE",
            100.0 * done_bytes as f64 / displayed_bytes.max(1) as f64
        )
    } else {
        commas(displayed_bytes)
    };
    out.push(format!(
        "<text class=\"weyard\" x=\"534\" y=\"15\" text-anchor=\"end\">{}</text>",
        corner
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
        out.push(format!(
            "<title>{}: {} bytes</title>",
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
                fill(tree, category)
            ));
        }
        let name = (body.width - 6.0 >= 90.0 && body.height >= 12.0)
            .then_some(tile.label.as_str())
            .unwrap_or("");
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
    let mut legend_x = 6.0;
    for (category, _, name) in CATEGORIES {
        let category_bytes: i64 = tiles.iter().map(|t| t.categories[category as usize]).sum();
        if category_bytes <= 0 || displayed_bytes <= 0 {
            continue;
        }
        let percentage = 100.0 * category_bytes as f64 / displayed_bytes as f64;
        let display = format!("{name} {percentage:.1}%");
        out.push(format!(
            "<rect x=\"{}\" y=\"288\" width=\"10\" height=\"10\" style=\"{}\"/>",
            legend_x,
            fill(tree, category)
        ));
        out.push(format!(
            "<text class=\"weyard legend-label\" x=\"{}\" y=\"293\" dominant-baseline=\"middle\">{}</text>",
            legend_x + 14.0,
            esc(&display)
        ));
        legend_x += 16.0 + display.chars().count() as f64 * 6.0;
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

#[cfg(test)]
mod tests {
    use super::svg;
    use crate::model::{Area, Category, Tile};
    use crate::pipeline::CoverageMap;
    use serde_json::Value;

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
                ..Tile::default()
            }],
        };
        let map = CoverageMap {
            document: Value::Null,
            rom_areas: Vec::new(),
            executable_areas: vec![area.clone(), area],
        };
        let rendered = svg("core", &map);
        assert!(rendered.contains("50.0% DONE"));
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
    }
}
