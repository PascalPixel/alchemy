// The 540x304 SpaceMonger-style box trees embedded in the README.
//
// WHY a second renderer: the dashboard card in `svg.rs` answers "how much", the
// box trees answer "which bytes, and who owns them". Parent rectangles here are
// real logical owners whose size comes exclusively from their descendants, so
// hierarchy never steals or invents byte area.
//
// PORT NOTE (trailing newline): `renderSvg` returns `lines.join("\n") + "\n"`.
// `renderBoxTree` returns `lines.join("\n")` with NO trailing newline. Four
// tracked files depend on that difference byte for byte.

use crate::base64;
use crate::jsnum::{commas, hex8, js_number_string, js_round, to_fixed};
use crate::jstext::{escape_text, utf16_len, utf16_slice_to};
use crate::model::{area, Area, Tile, ASSET_TIERS};
use crate::pipeline::CoverageMap;
use crate::sha1::sha1_hex;
use crate::svg::{document_number, document_string};
use crate::treemap::{squarify, Rect};

// ------------------------------------------------------------------ palette

pub struct HueBand {
    pub hsl_hue: f64,
    pub ok_hue: f64,
    pub ok_cmax: f64,
    pub edge: &'static str,
}

pub const CORE_HUE: HueBand = HueBand {
    hsl_hue: 275.0,
    ok_hue: 295.0,
    ok_cmax: 0.26,
    edge: "#6d4fc2",
};
pub const OVERLAY_HUE: HueBand = HueBand {
    hsl_hue: 190.0,
    ok_hue: 200.0,
    ok_cmax: 0.17,
    edge: "#1f7f93",
};
pub const ASSET_HUE: HueBand = HueBand {
    hsl_hue: 330.0,
    ok_hue: 355.0,
    ok_cmax: 0.30,
    edge: "#bb2f77",
};
pub const MUSIC_HUE: HueBand = HueBand {
    hsl_hue: 28.0,
    ok_hue: 55.0,
    ok_cmax: 0.20,
    edge: "#c85d00",
};

const GROUND: &str = "#ffffff";

pub const CODE_FRACTION: [(&str, f64); 5] = [
    ("humanized_c", 1.0),
    ("exact_c", 0.75),
    ("semantic_c", 0.0),
    ("assembly", 0.0),
    ("retained_asm", 0.5),
];

pub const ASSET_FRACTION: [(&str, f64); 6] = [
    ("asset_bytes", 0.08),
    ("asset_unclassified", 0.08),
    ("asset_bw", 0.34),
    ("asset_color", 0.67),
    ("asset_objects", 1.0),
    ("asset_data", 0.08),
];

const BOX_TREE_LEGEND: [(&str, &str); 9] = [
    ("exact_c", "Exact"),
    ("semantic_c", "Semantic"),
    ("retained_asm", "Permanent ASM"),
    ("asset_objects", "Objects"),
    ("asset_color", "Color images"),
    ("asset_bw", "B&W"),
    ("asset_bytes", "Encoded bytes"),
    ("asset_unclassified", "Unclassified"),
    ("asset_data", "Data / assets"),
];

// The 540 px README canvas and the one permitted 16 px pixel font are both
// fixed. These shorter forms leave room for an honest one-decimal percentage.
const BOX_TREE_COMPACT_LEGEND: [(&str, &str); 3] = [
    ("asset_color", "Color"),
    ("asset_bytes", "Encoded"),
    ("asset_unclassified", "Unknown"),
];

fn lookup_number(table: &[(&str, f64)], key: &str) -> Option<f64> {
    table
        .iter()
        .find(|(name, _)| *name == key)
        .map(|(_, value)| *value)
}

fn lookup_text<'a>(table: &'a [(&'a str, &'a str)], key: &str) -> Option<&'a str> {
    table
        .iter()
        .find(|(name, _)| *name == key)
        .map(|(_, value)| *value)
}

// --------------------------------------------------------------------- font

/// `embeddedWeyardFont()`: the OTF inlined as base64 into an `@font-face`.
///
/// PORT NOTE: the TypeScript memoizes with `??=` on a module-level binding.
/// A `OnceLock` reproduces that (read once per process) without the mutable
/// static. `readFileSync` throwing on a missing font becomes an `Err` here,
/// which every caller propagates instead of aborting.
pub fn embedded_weyard_font() -> Result<&'static str, String> {
    static CACHE: std::sync::OnceLock<Result<String, String>> = std::sync::OnceLock::new();
    let path = crate::tree::root()
        .join("assets")
        .join("fonts")
        .join("weyard.otf");
    match CACHE.get_or_init(|| {
        std::fs::read(&path)
            .map(|bytes| base64::encode(&bytes))
            .map_err(|error| format!("cannot read {}: {error}", path.display()))
    }) {
        Ok(text) => Ok(text.as_str()),
        Err(message) => Err(message.clone()),
    }
}

/// `weyardTextWidth`: the pixel font is exactly half-em wide per code unit.
pub fn weyard_text_width(text: &str, size: f64) -> f64 {
    utf16_len(text) as f64 * size / 2.0
}

fn below_label_floor(room: f64) -> bool {
    room.is_nan() || room < 3.0
}

/// `fitWeyardText`.
///
/// PORT NOTE: unlike `fitText` in `svg.rs`, this one has NO `Math.max` on the
/// slice bound. `room >= 3` makes `room - 1 >= 2` anyway, so the two agree in
/// practice, but the spellings are kept distinct because they are distinct
/// upstream.
pub fn fit_weyard_text(text: &str, width: f64, size: f64) -> Option<String> {
    let room = (width / (size / 2.0)).floor();
    // `if (room < 3) return undefined` in JS is a NEGATED `>=` once NaN is in
    // play: a NaN room (zero size, or a NaN width) fails `room < 3` in Rust and
    // would slice, where JS's comparison chain refuses. Naming the refusal
    // keeps that explicit instead of suppressing the lint.
    if below_label_floor(room) {
        return None;
    }
    let room = room as usize;
    if utf16_len(text) <= room {
        Some(text.to_string())
    } else {
        Some(format!("{}\u{2026}", utf16_slice_to(text, room - 1)))
    }
}

// ---------------------------------------------------------------- geometry

/// `precise`: round to six decimals, then stringify the JavaScript way.
pub fn precise(value: f64) -> String {
    let rounded = js_round(value * 1_000_000.0) / 1_000_000.0;
    if rounded == 0.0 {
        // `Object.is(rounded, -0) ? "0" : String(rounded)`; `String(0)` is also
        // "0", so both zeros collapse here.
        return "0".to_string();
    }
    js_number_string(rounded)
}

fn precise_rect(rectangle: Rect, attributes: &str) -> String {
    let x = js_round(rectangle.x * 1_000_000.0) / 1_000_000.0;
    let y = js_round(rectangle.y * 1_000_000.0) / 1_000_000.0;
    let right = js_round((rectangle.x + rectangle.width) * 1_000_000.0) / 1_000_000.0;
    let bottom = js_round((rectangle.y + rectangle.height) * 1_000_000.0) / 1_000_000.0;
    format!(
        "<rect x=\"{}\" y=\"{}\" width=\"{}\" height=\"{}\" {attributes}/>",
        precise(x),
        precise(y),
        precise(right - x),
        precise(bottom - y),
    )
}

// -------------------------------------------------------------- hierarchy

enum BoxNode {
    Group {
        label: String,
        children: Vec<BoxNode>,
    },
    Leaf(Tile),
}

/// `boxTreeNodes`.
///
/// PORT NOTE: the TypeScript also builds an `id` for every node
/// (`root/0-3/leaf-2-08123456`). Nothing ever reads it -- it reaches no
/// attribute and no comparison -- so it is not reproduced here. `hex8` is
/// referenced by the surrounding module for the same address spelling
/// elsewhere; the id itself is genuinely dead.
fn box_tree_nodes(tiles: &[Tile], folders: &[&str], depth: usize) -> Vec<BoxNode> {
    if depth >= folders.len() {
        return tiles
            .iter()
            .map(|tile| BoxNode::Leaf(tile.clone()))
            .collect();
    }
    let key = folders[depth];
    // PORT NOTE: a JS `Map` iterates in insertion order and that order decides
    // which folder is drawn where. A `HashMap` would randomize four tracked
    // files, so the grouping is an ordered vector.
    let mut groups: Vec<(String, Vec<Tile>)> = Vec::new();
    let mut direct: Vec<Tile> = Vec::new();
    for tile in tiles {
        let label = if key == "group" {
            tile.group.clone()
        } else {
            tile.subgroup.clone()
        };
        let Some(label) = label else {
            direct.push(tile.clone());
            continue;
        };
        match groups.iter_mut().find(|(name, _)| *name == label) {
            Some((_, children)) => children.push(tile.clone()),
            None => groups.push((label, vec![tile.clone()])),
        }
    }
    let mut out = box_tree_nodes(&direct, folders, folders.len());
    for (label, children) in groups {
        let nested = box_tree_nodes(&children, folders, depth + 1);
        out.push(BoxNode::Group {
            label,
            children: nested,
        });
    }
    out
}

fn box_node_bytes(node: &BoxNode) -> f64 {
    match node {
        BoxNode::Leaf(tile) => tile.bytes as f64,
        BoxNode::Group { children, .. } => children.iter().map(box_node_bytes).sum(),
    }
}

/// `folderDisplayName`.
fn folder_display_name(label: &str) -> String {
    if let Some(rest) = strip_prefix_ci(label, "resource_") {
        if !rest.is_empty() && rest.chars().all(|c| c.is_ascii_hexdigit()) {
            return rest.to_uppercase();
        }
    }
    let mut text = label
        .strip_prefix("golden-sun-")
        .unwrap_or(label)
        .to_string();
    if let Some(rest) = text.strip_prefix("gba-") {
        text = format!("GBA {rest}");
    }
    text = text.replace('-', " ");
    // `/\b\w/g` with JS semantics: `\w` and `\b` are both ASCII-only, so a
    // word character preceded by anything outside `[A-Za-z0-9_]` (or by
    // nothing) is uppercased.
    let mut out = String::with_capacity(text.len());
    let mut previous_is_word = false;
    for character in text.chars() {
        let is_word = character.is_ascii_alphanumeric() || character == '_';
        if is_word && !previous_is_word {
            out.extend(character.to_uppercase());
        } else {
            out.push(character);
        }
        previous_is_word = is_word;
    }
    out
}

/// `/^resource_/i` as a prefix strip.
fn strip_prefix_ci<'a>(text: &'a str, prefix: &str) -> Option<&'a str> {
    if text.len() < prefix.len() {
        return None;
    }
    let (head, rest) = text.split_at(prefix.len());
    if head.eq_ignore_ascii_case(prefix) {
        Some(rest)
    } else {
        None
    }
}

fn box_tree_category_bytes(item: &Area, category: &str) -> i64 {
    item.tiles.iter().map(|tile| tile.category(category)).sum()
}

fn box_tree_percent(bytes: i64, total: i64) -> String {
    if total <= 0 {
        return "0.0%".to_string();
    }
    let value = js_round(bytes as f64 * 1_000.0 / total as f64) / 10.0;
    format!("{}%", to_fixed(value, 1))
}

// ---------------------------------------------------------------- renderer

struct BoxContext<'a> {
    hue: &'a HueBand,
    category_fraction: &'a [(&'a str, f64)],
    category_order: &'a [&'a str],
    lines: Vec<String>,
}

impl BoxContext<'_> {
    fn cell_attributes(&self, fraction: f64) -> String {
        // One ramp for every hue: lightness falls 0.93 -> 0.55 and chroma rises
        // to the hue's P3 edge as completion rises.
        let light = 0.93 - 0.38 * fraction;
        let hsl = format!(
            "hsl({}, {}%, {}%)",
            js_number_string(self.hue.hsl_hue),
            js_number_string(js_round(fraction * 95.0)),
            js_number_string(js_round(93.0 - 38.0 * fraction)),
        );
        let ok = format!(
            "oklch({} {} {})",
            to_fixed(light, 3),
            to_fixed(self.hue.ok_cmax * fraction, 3),
            js_number_string(self.hue.ok_hue),
        );
        format!("style=\"fill:{hsl};fill:{ok}\"")
    }

    fn cell_rect(&self, rectangle: Rect, fraction: f64) -> String {
        precise_rect(rectangle, &self.cell_attributes(fraction))
    }

    fn fraction_of(&self, category: &str) -> f64 {
        lookup_number(self.category_fraction, category).unwrap_or(0.08)
    }

    fn draw_rectangle_label(&mut self, rectangle: Rect, text: &str) {
        if rectangle.height < 10.0 {
            return;
        }
        let Some(display) = fit_weyard_text(text, rectangle.width - 6.0, 8.0) else {
            return;
        };
        let banner = Rect {
            height: 10.0,
            ..rectangle
        };
        self.lines.push(precise_rect(
            banner,
            &format!(
                "fill=\"hsl({} 70% 24%)\" fill-opacity=\"0.9\"",
                js_number_string(self.hue.hsl_hue)
            ),
        ));
        self.lines.push(format!(
            "<text class=\"weyard rectangle-label\" x=\"{}\" y=\"{}\">{}</text>",
            precise(rectangle.x + 3.0),
            precise(rectangle.y + 8.0),
            escape_text(&display),
        ));
    }

    fn draw_leaf(&mut self, tile: &Tile, rectangle: Rect) {
        let populated: Vec<&str> = self
            .category_order
            .iter()
            .copied()
            .filter(|category| tile.category(category) > 0)
            .collect();
        let total: i64 = populated
            .iter()
            .map(|category| tile.category(category))
            .sum();
        self.lines.push(format!(
            "<g aria-label=\"{}\">",
            escape_text(&format!(
                "{}: {} bytes",
                tile.label,
                commas(tile.bytes as f64)
            ))
        ));
        if total <= 0 {
            let fallback = self.category_order.last().copied().unwrap_or("");
            let fraction = self.fraction_of(fallback);
            let cell = self.cell_rect(rectangle, fraction);
            self.lines.push(cell);
            let name = leaf_display_name(tile);
            self.draw_rectangle_label(rectangle, &name);
            self.lines.push("</g>".to_string());
            return;
        }
        let mut credited = 0i64;
        let last = populated.len() - 1;
        for (index, category) in populated.iter().enumerate() {
            let top = rectangle.y + rectangle.height * (credited as f64 / total as f64);
            credited += tile.category(category);
            let bottom = if index == last {
                rectangle.y + rectangle.height
            } else {
                rectangle.y + rectangle.height * (credited as f64 / total as f64)
            };
            let band = Rect {
                x: rectangle.x,
                y: top,
                width: rectangle.width,
                height: bottom - top,
            };
            let fraction = self.fraction_of(category);
            let cell = self.cell_rect(band, fraction);
            self.lines.push(cell);
        }
        let name = leaf_display_name(tile);
        self.draw_rectangle_label(rectangle, &name);
        self.lines.push("</g>".to_string());
    }

    fn draw_nodes(
        &mut self,
        nodes: &[BoxNode],
        rectangle: Rect,
        depth: usize,
    ) -> Result<(), String> {
        for placed in squarify(nodes, box_node_bytes, rectangle)? {
            match &nodes[placed.index] {
                BoxNode::Leaf(tile) => self.draw_leaf(tile, placed.rect),
                BoxNode::Group { label, children } => {
                    let bytes = box_node_bytes(&nodes[placed.index]);
                    let folder_depth = depth + 1;
                    self.lines.push(format!(
                        "<g aria-label=\"{}\">",
                        escape_text(&format!("{label}: {} bytes", commas(bytes)))
                    ));
                    self.draw_nodes(children, placed.rect, folder_depth)?;
                    let stroke = if folder_depth == 1 { 2 } else { 1 };
                    self.lines.push(precise_rect(
                        placed.rect,
                        &format!(
                            "fill=\"none\" stroke=\"hsl({} 70% 24%)\" stroke-width=\"{stroke}\" \
                             vector-effect=\"non-scaling-stroke\"",
                            js_number_string(self.hue.hsl_hue)
                        ),
                    ));
                    let name = folder_display_name(label);
                    self.draw_rectangle_label(placed.rect, &name);
                    self.lines.push("</g>".to_string());
                }
            }
        }
        Ok(())
    }
}

/// `leafDisplayName`.
fn leaf_display_name(tile: &Tile) -> String {
    // PORT NOTE: the TypeScript builds `new RegExp("^" + escapeRegex(group) +
    // " · ")`. Every metacharacter is escaped, so the pattern can only ever
    // match a literal anchored prefix; a plain `strip_prefix` is exactly
    // equivalent and needs no regex engine.
    let without_group = match &tile.group {
        Some(group) => {
            let prefix = format!("{group} \u{b7} ");
            tile.label
                .strip_prefix(&prefix)
                .unwrap_or(&tile.label)
                .to_string()
        }
        None => tile.label.clone(),
    };
    if let Some(exact) = capture_after_ci(&without_group, "AlchemyC_") {
        return exact;
    }
    for marker in ["owner ", "assembly "] {
        if let Some(address) = capture_address_ci(&without_group, marker) {
            return address.replace("0x", "");
        }
    }
    // `split(" · ", 1)[0]` keeps only the first segment.
    let head = without_group
        .split(" \u{b7} ")
        .next()
        .unwrap_or("")
        .to_string();
    head.replace("0x", "")
}

/// `/AlchemyC_([0-9a-f]+)/i` -- the first hex run after a case-insensitive
/// literal.
fn capture_after_ci(text: &str, marker: &str) -> Option<String> {
    let lowered = text.to_ascii_lowercase();
    let start = lowered.find(&marker.to_ascii_lowercase())? + marker.len();
    let run: String = text[start..]
        .chars()
        .take_while(char::is_ascii_hexdigit)
        .collect();
    if run.is_empty() {
        None
    } else {
        Some(run)
    }
}

/// `/(?:owner |assembly )0x([0-9a-f]+(?:–0x[0-9a-f]+)?)/i` for one alternative.
fn capture_address_ci(text: &str, marker: &str) -> Option<String> {
    let lowered = text.to_ascii_lowercase();
    let mut from = 0usize;
    while let Some(found) = lowered[from..].find(&marker.to_ascii_lowercase()) {
        let start = from + found + marker.len();
        let rest = &text[start..];
        if let Some(body) = rest.strip_prefix("0x").or_else(|| rest.strip_prefix("0X")) {
            let first: String = body.chars().take_while(char::is_ascii_hexdigit).collect();
            if !first.is_empty() {
                let mut captured = format!("0x{first}");
                let tail = &body[first.len()..];
                if let Some(after) = tail.strip_prefix("\u{2013}") {
                    if let Some(hex) = after
                        .strip_prefix("0x")
                        .or_else(|| after.strip_prefix("0X"))
                    {
                        let second: String =
                            hex.chars().take_while(char::is_ascii_hexdigit).collect();
                        if !second.is_empty() {
                            captured.push_str("\u{2013}0x");
                            captured.push_str(&second);
                        }
                    }
                }
                return Some(captured);
            }
        }
        from = start;
    }
    None
}

/// `renderBoxTree`.
pub fn render_box_tree(
    item: &Area,
    aria_label: &str,
    hue: &HueBand,
    category_fraction: &[(&str, f64)],
    category_order: &[&str],
    folders: &[&str],
    title: &str,
) -> Result<String, String> {
    // Keeping the viewBox 1:1 with the CSS size is what makes the one permitted
    // 16 px Weyard face pixel-exact instead of scaling it down.
    let width = 540.0f64;
    let height = 304.0f64;
    let plot = Rect {
        x: 3.0,
        y: 22.0,
        width: width - 6.0,
        height: height - 46.0,
    };
    let mut context = BoxContext {
        hue,
        category_fraction,
        category_order,
        lines: Vec::new(),
    };
    context.lines.push(format!(
        "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 {} {}\" width=\"{}\" \
         height=\"{}\" shape-rendering=\"crispEdges\" role=\"img\" aria-label=\"{}\">",
        js_number_string(width),
        js_number_string(height),
        js_number_string(width),
        js_number_string(height),
        escape_text(aria_label),
    ));
    context.lines.push(format!(
        "<defs><style>@font-face{{font-family:Weyard;src:url(data:font/otf;base64,{}) \
         format('opentype');font-style:italic;}}\
         .weyard{{font-family:Weyard;font-size:16px;font-style:italic;fill:#fff;}}\
         .rectangle-label{{font-size:8px;}}</style></defs>",
        embedded_weyard_font()?,
    ));
    context.lines.push(precise_rect(
        Rect {
            x: 0.0,
            y: 0.0,
            width,
            height,
        },
        &format!("fill=\"{}\" rx=\"8\"", hue.edge),
    ));
    context.lines.push(precise_rect(
        Rect {
            x: 1.0,
            y: 1.0,
            width: width - 2.0,
            height: height - 2.0,
        },
        "fill=\"none\" stroke=\"#1c1c26\" stroke-width=\"2\" rx=\"7\"",
    ));
    context
        .lines
        .push(precise_rect(plot, &format!("fill=\"{GROUND}\" rx=\"3\"")));

    let nodes = box_tree_nodes(&item.tiles, folders, 0);
    let total: f64 = nodes.iter().map(box_node_bytes).sum();
    if total != item.bytes as f64 {
        return Err(format!(
            "{} hierarchy does not conserve its byte total",
            item.id
        ));
    }
    context.draw_nodes(&nodes, plot, 0)?;
    context.lines.push(format!(
        "<text class=\"weyard\" x=\"6\" y=\"17\">{}</text>",
        escape_text(&title.to_uppercase())
    ));

    let completion_category = if category_order.contains(&"exact_c") {
        Some("exact_c")
    } else if category_order.contains(&"asset_objects") {
        Some("asset_objects")
    } else {
        category_order.first().copied()
    };
    if let Some(category) = completion_category {
        let completion_label = if category == "exact_c" {
            "DONE".to_string()
        } else {
            lookup_text(&BOX_TREE_LEGEND, category)
                .unwrap_or(category)
                .to_uppercase()
        };
        // Permanent assembly is source-owned and byte-exact too, so DONE is
        // explicitly the sum of exact C and permanent asm.
        let completion_bytes = box_tree_category_bytes(item, category)
            + if category == "exact_c" {
                box_tree_category_bytes(item, "retained_asm")
            } else {
                0
            };
        let completion = box_tree_percent(completion_bytes, item.bytes);
        context.lines.push(format!(
            "<text class=\"weyard\" x=\"{}\" y=\"17\" text-anchor=\"end\">{}</text>",
            js_number_string(width - 6.0),
            escape_text(&format!("{completion_label} {completion}")),
        ));
    }

    let mut legend_x = 6.0f64;
    let legend_center_y = height - 11.0;
    for category in category_order {
        let Some(legend_label) = lookup_text(&BOX_TREE_LEGEND, category) else {
            continue;
        };
        let category_bytes = box_tree_category_bytes(item, category);
        // A zero-valued category must not survive as a misleading 0.0% item.
        if category_bytes == 0 {
            continue;
        }
        let percentage = box_tree_percent(category_bytes, item.bytes);
        let compact = lookup_text(&BOX_TREE_COMPACT_LEGEND, category).unwrap_or(legend_label);
        let display_label = format!("{compact} {percentage}");
        let swatch = Rect {
            x: legend_x,
            y: legend_center_y - 5.0,
            width: 10.0,
            height: 10.0,
        };
        let fraction = context.fraction_of(category);
        let cell = precise_rect(swatch, &context.cell_attributes(fraction));
        context.lines.push(cell);
        context.lines.push(format!(
            "<text class=\"weyard\" x=\"{}\" y=\"{}\" dominant-baseline=\"middle\">{}</text>",
            precise(legend_x + 14.0),
            js_number_string(legend_center_y),
            escape_text(&display_label),
        ));
        legend_x += 16.0 + weyard_text_width(&display_label, 16.0);
    }
    context.lines.push("</svg>".to_string());
    Ok(context.lines.join("\n"))
}

// ------------------------------------------------------------- the four trees

pub const BOX_TREES: [&str; 4] = ["core", "overlays", "images", "music"];

pub fn box_tree_path(target: &str, tree: &str) -> std::path::PathBuf {
    crate::tree::root()
        .join("assets")
        .join("readme")
        .join(format!("{target}-{tree}.svg"))
}

struct AssetTreeContext {
    area: Area,
    category_order: Vec<&'static str>,
    folders: Vec<&'static str>,
    detailed: bool,
}

fn asset_tree_context(
    map: &CoverageMap,
    tree: Option<&crate::tree::SourceTree>,
    prefer_verified_assets: bool,
) -> Result<AssetTreeContext, String> {
    let Some(rom_data) = map.rom_areas.iter().find(|item| item.id == "rom-data") else {
        return Err("coverage map is missing its asset area".to_string());
    };
    let embedded_maturity: Vec<Tile> = rom_data
        .tiles
        .iter()
        .filter(|tile| {
            ASSET_TIERS.iter().any(|tier| tile.category(tier) > 0)
                || tile.category("asset_unclassified") > 0
        })
        .cloned()
        .collect();
    let verified_maturity = match (prefer_verified_assets, tree) {
        (true, Some(tree)) => {
            crate::verified::verified_asset_tiles(tree, &document_string(map, &["target"]))?
        }
        _ => None,
    };
    // New maps carry exact addressed package leaves. The fallback keeps older
    // tracked or synthetic maps renderable.
    let maturity: Vec<Tile> = match &verified_maturity {
        Some(tiles) => tiles.clone(),
        None => {
            if !embedded_maturity.is_empty() {
                embedded_maturity.clone()
            } else if let Some(tree) = tree {
                crate::assets::asset_maturity_tiles(tree)?
            } else {
                Vec::new()
            }
        }
    };
    let catalogued_bytes: i64 = maturity
        .iter()
        .map(|tile| {
            if tile.category("asset_unclassified") > 0 {
                0
            } else {
                tile.bytes
            }
        })
        .sum();
    let embedded_unclassified: i64 = maturity
        .iter()
        .map(|tile| tile.category("asset_unclassified"))
        .sum();
    let exact_leaves = verified_maturity.is_some() || !embedded_maturity.is_empty();
    let unclassified_bytes = if exact_leaves {
        embedded_unclassified
    } else {
        // `Math.max(a, 0)`; neither operand can be NaN here.
        let remainder = rom_data.bytes - catalogued_bytes;
        if remainder > 0 {
            remainder
        } else {
            0
        }
    };
    // The maturity census is manifest-backed and does not claim unknown ROM
    // data as decoded; that remainder stays in the tree at the byte-represented
    // floor rather than being credited to a higher tier.
    let maturity_with_remainder: Vec<Tile> = if exact_leaves {
        maturity
    } else if tree.is_some() && unclassified_bytes != 0 {
        let mut extended = maturity;
        let mut remainder = Tile {
            label: "Unclassified ROM-image data (byte-represented)".to_string(),
            bytes: unclassified_bytes,
            ..Tile::default()
        };
        remainder.set_category("asset_unclassified", unclassified_bytes);
        extended.push(remainder);
        extended
    } else {
        maturity
    };
    let detailed = tree.is_some() || exact_leaves;
    let folders: Vec<&'static str> = if maturity_with_remainder
        .iter()
        .any(|tile| tile.group.is_some())
    {
        vec!["group", "subgroup"]
    } else {
        vec![]
    };
    Ok(AssetTreeContext {
        area: if detailed {
            Area {
                id: "rom-data".to_string(),
                label: rom_data.label.clone(),
                bytes: rom_data.bytes,
                categories: crate::ordered::OrderedMap::new(),
                tiles: maturity_with_remainder,
            }
        } else {
            rom_data.clone()
        },
        category_order: if detailed {
            vec![
                "asset_unclassified",
                "asset_bytes",
                "asset_bw",
                "asset_color",
                "asset_objects",
            ]
        } else {
            vec!["asset_data"]
        },
        folders,
        detailed,
    })
}

fn render_asset_tree(
    context: &AssetTreeContext,
    asset_area: &Area,
    title: &str,
    hue: &HueBand,
    band: &str,
) -> Result<String, String> {
    let detail = if context.detailed {
        format!(
            "{} bytes are tiered by their tracked representation maturity",
            commas(asset_area.bytes as f64)
        )
    } else {
        format!("{} ROM-image bytes", commas(asset_area.bytes as f64))
    };
    render_box_tree(
        asset_area,
        &format!("{title} maturity box tree, {band} band; {detail}"),
        hue,
        &ASSET_FRACTION,
        &context.category_order,
        &context.folders,
        title,
    )
}

fn is_music_asset(tile: &Tile) -> bool {
    let identity = format!(
        "{} {} {}",
        tile.group.clone().unwrap_or_default(),
        tile.subgroup.clone().unwrap_or_default(),
        tile.label
    );
    let lowered = identity.to_ascii_lowercase();
    [
        "sound",
        "audio",
        "wave",
        "pcm",
        "music",
        "midi",
        "sequence",
        "soundfont",
        "sound-font",
    ]
    .iter()
    .any(|needle| lowered.contains(needle))
}

/// `renderBoxTrees`: the four tracked panels, keyed in `BOX_TREES` order.
pub fn render_box_trees(
    map: &CoverageMap,
    tree: Option<&crate::tree::SourceTree>,
    prefer_verified_assets: bool,
) -> Result<Vec<(&'static str, String)>, String> {
    let core = map.executable_areas.iter().find(|item| item.id == "main");
    let overlays = map
        .executable_areas
        .iter()
        .find(|item| item.id == "overlays");
    let (Some(core), Some(overlays)) = (core, overlays) else {
        return Err("coverage map is missing a box-tree area".to_string());
    };
    let assets = asset_tree_context(map, tree, prefer_verified_assets)?;
    let music_tiles: Vec<Tile> = assets
        .area
        .tiles
        .iter()
        .filter(|tile| is_music_asset(tile))
        .cloned()
        .collect();
    let image_tiles: Vec<Tile> = assets
        .area
        .tiles
        .iter()
        .filter(|tile| !is_music_asset(tile))
        .cloned()
        .collect();
    let images = area("images", "Images", image_tiles);
    let music = area("music", "Music", music_tiles);
    if images.bytes + music.bytes != assets.area.bytes {
        return Err("published asset split does not conserve its byte total".to_string());
    }
    let code_order = ["semantic_c", "retained_asm", "exact_c", "humanized_c"];
    Ok(vec![
        (
            "core",
            render_box_tree(
                core,
                "Main-image code coverage box tree, purple band; 64 KiB address banks contain \
                 audited source-owner leaves at their natural executable byte size",
                &CORE_HUE,
                &CODE_FRACTION,
                &code_order,
                &["group"],
                "Main image",
            )?,
        ),
        (
            "overlays",
            render_box_tree(
                overlays,
                "Decoded code-overlay coverage box tree, cyan band; each resource contains its \
                 exact, semantic, and assembly source regions",
                &OVERLAY_HUE,
                &CODE_FRACTION,
                &code_order,
                &["group"],
                "Code overlays",
            )?,
        ),
        (
            "images",
            render_asset_tree(&assets, &images, "Images", &ASSET_HUE, "pink")?,
        ),
        (
            "music",
            render_asset_tree(&assets, &music, "Music", &MUSIC_HUE, "orange")?,
        ),
    ])
}

// ------------------------------------------------------------------- README

pub fn svg_cache_version(svg: &str) -> String {
    sha1_hex(svg.as_bytes())[..8].to_string()
}

/// Rewrite the README's embeds to carry per-tree versions, replacing any
/// present.
///
/// PORT NOTE: the pattern is `\({escaped file}(?:\?v=[0-9a-f]*)?\)` with the
/// `g` flag. `escapeRegex` makes the file part a literal, and the optional tail
/// is a bounded hex run, so a scanner reproduces it without a regex engine.
/// Note `*` not `+`: a bare `?v=` also matches and is replaced.
pub fn readme_with_cache_buster(readme: &str, target: &str, versions: &[(&str, String)]) -> String {
    let mut out = readme.to_string();
    for tree in BOX_TREES {
        let Some((_, version)) = versions.iter().find(|(name, _)| *name == tree) else {
            continue;
        };
        let file = format!("assets/readme/{target}-{tree}.svg");
        let needle = format!("({file}");
        let replacement = format!("({file}?v={version})");
        let mut result = String::with_capacity(out.len());
        let mut rest = out.as_str();
        while let Some(found) = rest.find(&needle) {
            let after = &rest[found + needle.len()..];
            let tail = if let Some(hex) = after.strip_prefix("?v=") {
                let run: usize = hex
                    .chars()
                    .take_while(char::is_ascii_hexdigit)
                    .map(char::len_utf8)
                    .sum();
                if hex[run..].starts_with(')') {
                    Some(3 + run + 1)
                } else {
                    None
                }
            } else if after.starts_with(')') {
                Some(1)
            } else {
                None
            };
            match tail {
                Some(length) => {
                    result.push_str(&rest[..found]);
                    result.push_str(&replacement);
                    rest = &rest[found + needle.len() + length..];
                }
                None => {
                    result.push_str(&rest[..found + needle.len()]);
                    rest = &rest[found + needle.len()..];
                }
            }
        }
        result.push_str(rest);
        out = result;
    }
    out
}

/// Keep the README's headline aligned with the box-tree DONE metric.
///
/// PORT NOTE: `/^## (?:Progress|DONE):.*$/m` replaces only the FIRST match
/// (no `g`). JS `.` excludes the four line terminators and `$` with `m`
/// matches before `\n`, so the run is bounded by the next newline.
pub fn readme_with_done_headline(readme: &str, map: &CoverageMap) -> Result<String, String> {
    let done_bytes = document_number(map, &["categories", "exact_c", "bytes"])
        + document_number(map, &["categories", "retained_asm", "bytes"]);
    let executable = document_number(map, &["executable_bytes"]);
    let done = js_round(crate::intervals::round_half_up_percent(
        done_bytes, executable,
    )?);
    let headline = format!("## DONE: Currently {}%", js_number_string(done));
    let mut offset = 0usize;
    for line in readme.split_inclusive('\n') {
        let body = line.strip_suffix('\n').unwrap_or(line);
        if body.starts_with("## Progress:") || body.starts_with("## DONE:") {
            let mut out = String::with_capacity(readme.len());
            out.push_str(&readme[..offset]);
            out.push_str(&headline);
            out.push_str(&readme[offset + body.len()..]);
            return Ok(out);
        }
        offset += line.len();
    }
    Ok(readme.to_string())
}

pub fn readme_path() -> std::path::PathBuf {
    crate::tree::root().join("README.md")
}

/// PORT NOTE (reproduced bug): `svgPath` exists in the TypeScript and is never
/// called -- no writer, no checker, no test reaches it. It is kept here so the
/// two files stay comparable line for line, and named in the crate report.
#[allow(dead_code)]
fn svg_path(target: &str) -> std::path::PathBuf {
    crate::tree::root()
        .join("assets")
        .join("readme")
        .join(format!("{target}-coverage.svg"))
}

/// `hex8` is the address spelling the box-tree node ids used upstream; kept
/// re-exported so the omission of those dead ids stays auditable.
pub fn address_label(address: i64) -> String {
    hex8(address)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn precise_trims_to_six_decimals_and_prints_the_javascript_spelling() {
        assert_eq!(precise(3.0), "3", "not \"3.0\"");
        assert_eq!(precise(1.0 / 3.0), "0.333333");
        assert_eq!(precise(-0.0000001), "0", "negative zero prints unsigned");
        assert_eq!(precise(540.0), "540");
    }

    #[test]
    fn a_rectangle_shares_its_far_edge_rather_than_summing_a_rounded_width() {
        // Rounding x and right independently is what keeps adjacent cells from
        // leaving a hairline gap.
        let tag = precise_rect(
            Rect {
                x: 0.1234567,
                y: 0.0,
                width: 0.1234567,
                height: 4.0,
            },
            "fill=\"none\"",
        );
        assert!(tag.contains("x=\"0.123457\""), "{tag}");
        assert!(tag.contains("width=\"0.123456\""), "{tag}");
        // 0.246913 - 0.123457 is 0.123456, not the rounded width 0.123457.
    }

    #[test]
    fn the_weyard_fitter_has_no_max_on_its_slice_bound() {
        assert_eq!(
            fit_weyard_text("abcdef", 100.0, 8.0),
            Some("abcdef".to_string())
        );
        assert_eq!(
            fit_weyard_text("abcdef", 16.0, 8.0),
            Some("abc\u{2026}".to_string())
        );
        assert_eq!(
            fit_weyard_text("abcdef", 11.0, 8.0),
            None,
            "room 2 is below the floor"
        );
    }

    #[test]
    fn folder_names_uppercase_ascii_word_starts_only() {
        assert_eq!(folder_display_name("resource_1a2b"), "1A2B");
        assert_eq!(folder_display_name("golden-sun-sprite-bank"), "Sprite Bank");
        assert_eq!(folder_display_name("gba-header"), "GBA Header");
        assert_eq!(
            folder_display_name("\u{e9}clair-bank"),
            "\u{e9}Clair Bank",
            "JS \\w is ASCII-only: e-acute is a non-word char, so `c` is the word start"
        );
    }

    #[test]
    fn the_percentage_rounds_half_up_the_javascript_way() {
        assert_eq!(box_tree_percent(0, 0), "0.0%");
        assert_eq!(box_tree_percent(1, 3), "33.3%");
        assert_eq!(box_tree_percent(1, 1), "100.0%");
    }

    #[test]
    fn the_leaf_name_strips_its_group_and_every_hex_prefix() {
        let mut tile = Tile {
            label: "Bank 08 \u{b7} owner 0x08000000\u{2013}0x08000100".to_string(),
            bytes: 1,
            ..Tile::default()
        };
        tile.group = Some("Bank 08".to_string());
        assert_eq!(leaf_display_name(&tile), "08000000\u{2013}08000100");

        let alchemy = Tile {
            label: "AlchemyC_08001234 stub".to_string(),
            bytes: 1,
            ..Tile::default()
        };
        assert_eq!(leaf_display_name(&alchemy), "08001234");

        let plain = Tile {
            label: "0x0800 \u{b7} tail".to_string(),
            bytes: 1,
            ..Tile::default()
        };
        assert_eq!(leaf_display_name(&plain), "0800");
    }

    #[test]
    fn the_cache_buster_replaces_a_present_version_and_adds_a_missing_one() {
        let readme = "![a](assets/readme/gs1-en-core.svg?v=deadbeef)\n\
                      ![b](assets/readme/gs1-en-music.svg)\n\
                      ![c](assets/readme/gs1-en-core.svg)\n";
        let versions = vec![
            ("core", "11112222".to_string()),
            ("music", "33334444".to_string()),
        ];
        let out = readme_with_cache_buster(readme, "gs1-en", &versions);
        assert_eq!(out.matches("gs1-en-core.svg?v=11112222").count(), 2);
        assert!(out.contains("gs1-en-music.svg?v=33334444"));
        assert!(!out.contains("deadbeef"));
    }

    #[test]
    fn an_empty_version_tail_is_still_a_match() {
        // The pattern is `[0-9a-f]*`, not `+`.
        let out = readme_with_cache_buster(
            "(assets/readme/gs1-en-core.svg?v=)",
            "gs1-en",
            &[("core", "abc".to_string())],
        );
        assert_eq!(out, "(assets/readme/gs1-en-core.svg?v=abc)");
    }
}
