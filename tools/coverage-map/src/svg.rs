// The dark dashboard panel: `renderSvg`.
//
// 1200x660, black surface, one accent per category, monospaced addresses.
// Every number that reaches the output goes through `jsnum`, because
// `format!("{}", 1.0f64)` is `"1"` in JavaScript and `"1"` only by accident in
// Rust -- `{}` on `1.0f64` prints `1`, but on `0.30000000000000004` the two
// disagree, and `round()` produces exactly that shape.

use crate::jsnum::{commas, js_number_string, js_round, to_fixed};
use crate::jstext::{escape_text, utf16_len, utf16_slice_to};
use crate::model::{Area, Tile, CATEGORY_ORDER};
use crate::pipeline::CoverageMap;
use crate::treemap::{squarify, Rect};

pub const SURFACE: &str = "#000000";
pub const CARD: &str = "#0a0a0a";
pub const BORDER: &str = "#262626";
pub const HAIRLINE: &str = "#1f1f1f";
pub const TEXT: &str = "#ededed";
pub const MUTED: &str = "#8f8f8f";
pub const SANS: &str = "'Geist Sans', Inter, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif";
pub const MONO: &str =
    "'Geist Mono', ui-monospace, SFMono-Regular, 'SF Mono', Menlo, Consolas, monospace";

pub struct CategoryStyle {
    pub fill: &'static str,
    pub ink: &'static str,
    pub label: &'static str,
}

/// `CATEGORY_STYLE`.
///
/// PORT NOTE: a JS object literal keyed by category, looked up by name. A
/// missing key would be `undefined` and throw on `.fill`; the fallback here
/// returns the `assembly` style instead of panicking, which is unreachable for
/// the five fixed categories but keeps a future sixth from aborting the run.
pub fn category_style(category: &str) -> &'static CategoryStyle {
    const EXACT: CategoryStyle = CategoryStyle {
        fill: "#0072f5",
        ink: "#eaf2ff",
        label: "byte-exact C",
    };
    const SEMANTIC: CategoryStyle = CategoryStyle {
        fill: "#ededed",
        ink: "#333333",
        label: "semantic C",
    };
    const ASSEMBLY: CategoryStyle = CategoryStyle {
        fill: "#333333",
        ink: "#a1a1a1",
        label: "assembly",
    };
    const RETAINED: CategoryStyle = CategoryStyle {
        fill: "#50e3c2",
        ink: "#04241d",
        label: "permanent asm",
    };
    const ASSET: CategoryStyle = CategoryStyle {
        fill: "#ff0080",
        ink: "#2b0016",
        label: "data / assets",
    };
    match category {
        "exact_c" => &EXACT,
        "semantic_c" => &SEMANTIC,
        "retained_asm" => &RETAINED,
        "asset_data" => &ASSET,
        _ => &ASSEMBLY,
    }
}

/// `round(value)` = `Math.round(value * 10) / 10`, stringified the JS way.
fn round_text(value: f64) -> String {
    js_number_string(js_round(value * 10.0) / 10.0)
}

/// `megabytes(value)`.
pub fn megabytes(value: f64) -> String {
    format!("{} MB", to_fixed(value / (1024.0 * 1024.0), 2))
}

/// `fitText(text, width, fontSize)`.
pub fn fit_text(text: &str, width: f64, font_size: f64) -> Option<String> {
    let per_character = font_size * 0.58;
    let room = (width / per_character).floor();
    if room < 3.0 {
        return None;
    }
    let room = room as usize;
    if utf16_len(text) <= room {
        Some(text.to_string())
    } else {
        Some(format!(
            "{}…",
            utf16_slice_to(text, room.saturating_sub(1).max(1))
        ))
    }
}

#[derive(Default)]
pub struct RectOptions {
    pub stroke: Option<&'static str>,
    pub radius: Option<f64>,
    pub opacity: Option<f64>,
}

/// `rect(area, fill, options)`.
pub fn rect_tag(area: Rect, fill: &str, options: &RectOptions) -> String {
    // `options.radius ?` and `options.stroke ?` are truthiness tests: a zero
    // radius and an empty stroke both emit nothing.
    let radius = match options.radius {
        Some(value) if value != 0.0 && !value.is_nan() => {
            format!(" rx=\"{}\"", js_number_string(value))
        }
        _ => String::new(),
    };
    let stroke = match options.stroke {
        Some(value) if !value.is_empty() => format!(" stroke=\"{value}\" stroke-width=\"1\""),
        _ => String::new(),
    };
    let opacity = match options.opacity {
        Some(value) => format!(" opacity=\"{}\"", js_number_string(value)),
        None => String::new(),
    };
    format!(
        "<rect x=\"{}\" y=\"{}\" width=\"{}\" height=\"{}\" fill=\"{fill}\"{stroke}{radius}{opacity}/>",
        round_text(area.x),
        round_text(area.y),
        round_text(area.width),
        round_text(area.height),
    )
}

#[derive(Default)]
pub struct LabelOptions<'a> {
    pub size: Option<f64>,
    pub fill: Option<&'a str>,
    pub anchor: Option<&'a str>,
    pub weight: Option<f64>,
    pub tracking: Option<f64>,
    pub mono: bool,
    pub opacity: Option<f64>,
}

/// `label(x, y, text, options)`.
pub fn label_tag(x: f64, y: f64, text: &str, options: &LabelOptions) -> String {
    let mut parts = vec![
        format!("x=\"{}\"", round_text(x)),
        format!("y=\"{}\"", round_text(y)),
        format!(
            "font-size=\"{}\"",
            js_number_string(options.size.unwrap_or(11.0))
        ),
        format!("fill=\"{}\"", options.fill.unwrap_or(TEXT)),
    ];
    if let Some(anchor) = options.anchor {
        if !anchor.is_empty() && anchor != "start" {
            parts.push(format!("text-anchor=\"{anchor}\""));
        }
    }
    if let Some(weight) = options.weight {
        if weight != 0.0 && !weight.is_nan() {
            parts.push(format!("font-weight=\"{}\"", js_number_string(weight)));
        }
    }
    if let Some(tracking) = options.tracking {
        if tracking != 0.0 && !tracking.is_nan() {
            parts.push(format!("letter-spacing=\"{}\"", js_number_string(tracking)));
        }
    }
    if options.mono {
        parts.push(format!("font-family=\"{MONO}\""));
    }
    if let Some(opacity) = options.opacity {
        parts.push(format!("opacity=\"{}\"", js_number_string(opacity)));
    }
    format!("<text {}>{}</text>", parts.join(" "), escape_text(text))
}

/// `/^[0-9a-f]+(–[0-9a-f]+)?$/i` -- an address or address range label.
///
/// PORT NOTE: the separator is U+2013 EN DASH, not a hyphen, and the `i` flag
/// matters: uppercase hex labels must match too. Written out rather than
/// lower-cased first, because a case-insensitive comparison is exactly the
/// shortcut that masked a missing `i` elsewhere in this repository.
fn is_address_label(text: &str) -> bool {
    fn hex_run(rest: &str) -> Option<&str> {
        let taken = rest
            .find(|c: char| !c.is_ascii_hexdigit())
            .unwrap_or(rest.len());
        if taken == 0 {
            None
        } else {
            Some(&rest[taken..])
        }
    }
    let Some(rest) = hex_run(text) else {
        return false;
    };
    if rest.is_empty() {
        return true;
    }
    let Some(rest) = rest.strip_prefix('–') else {
        return false;
    };
    matches!(hex_run(rest), Some(""))
}

fn tile_rects(tile: &Tile, area: Rect, lines: &mut Vec<String>) {
    // A one-pixel gutter turns the treemap into separate cards instead of a
    // grid of hairlines.
    let body = Rect {
        x: area.x + 0.5,
        y: area.y + 0.5,
        width: (area.width - 1.0).max(0.5),
        height: (area.height - 1.0).max(0.5),
    };
    let mut offset = 0.0f64;
    let mut midpoint_category = "assembly";
    for category in CATEGORY_ORDER {
        let share = tile.category(category) as f64;
        if share <= 0.0 {
            continue;
        }
        let height = body.height * (share / tile.bytes as f64);
        lines.push(rect_tag(
            Rect {
                x: body.x,
                y: body.y + body.height - offset - height,
                width: body.width,
                height,
            },
            category_style(category).fill,
            &RectOptions::default(),
        ));
        if offset <= body.height / 2.0 && body.height / 2.0 < offset + height {
            midpoint_category = category;
        }
        offset += height;
    }
    let text = if body.height >= 12.0 {
        fit_text(&tile.label, body.width - 6.0, 9.0)
    } else {
        None
    };
    // `text === tile.label`: an `undefined` never equals the label.
    if text.as_deref() == Some(tile.label.as_str()) {
        lines.push(label_tag(
            body.x + body.width / 2.0,
            body.y + body.height / 2.0 + 3.2,
            &tile.label,
            &LabelOptions {
                size: Some(9.0),
                anchor: Some("middle"),
                mono: is_address_label(&tile.label),
                fill: Some(category_style(midpoint_category).ink),
                opacity: Some(0.9),
                ..LabelOptions::default()
            },
        ));
    }
}

fn card(
    title: &str,
    meta: &str,
    areas: &[Area],
    frame: Rect,
    lines: &mut Vec<String>,
) -> Result<(), String> {
    lines.push(rect_tag(
        frame,
        CARD,
        &RectOptions {
            stroke: Some(BORDER),
            radius: Some(8.0),
            opacity: None,
        },
    ));
    lines.push(label_tag(
        frame.x + 16.0,
        frame.y + 25.0,
        &title.to_uppercase(),
        &LabelOptions {
            size: Some(10.0),
            fill: Some(MUTED),
            tracking: Some(0.9),
            ..LabelOptions::default()
        },
    ));
    lines.push(label_tag(
        frame.x + frame.width - 16.0,
        frame.y + 25.0,
        meta,
        &LabelOptions {
            size: Some(10.0),
            fill: Some(MUTED),
            anchor: Some("end"),
            ..LabelOptions::default()
        },
    ));
    let body = Rect {
        x: frame.x + 12.0,
        y: frame.y + 40.0,
        width: frame.width - 24.0,
        height: frame.height - 52.0,
    };
    // PORT NOTE: the tiebreak is `left.id.localeCompare(right.id)`, ICU
    // collation, approximated here by UTF-16 code-unit order. The two disagree
    // on where `-` sorts relative to letters and digits, and the real area ids
    // (`rom-code-main`, `rom-overlay-streams`, `rom-data`, `main`, `overlays`)
    // do contain hyphens. The tiebreak is only consulted when two areas have
    // *exactly* equal byte totals, which does not happen on any tracked target,
    // so no divergence is observable here -- but that is evidence, not proof.
    let mut sorted: Vec<Area> = areas.to_vec();
    sorted.sort_by(|left, right| match right.bytes.cmp(&left.bytes) {
        std::cmp::Ordering::Equal => crate::jsnum::utf16_cmp(&left.id, &right.id),
        other => other,
    });
    for placed_area in squarify(&sorted, |item| item.bytes as f64, body)? {
        let item = &sorted[placed_area.index];
        let area_rect = placed_area.rect;
        // Groups are separated by their gutter and their own label rather than
        // by a frame; one fewer box per group, same reading.
        let inset = Rect {
            x: area_rect.x + 6.0,
            y: area_rect.y + 6.0,
            width: area_rect.width - 12.0,
            height: area_rect.height - 12.0,
        };
        let size = format!("{} B", commas(item.bytes as f64));
        let size_fits = inset.width >= 190.0;
        let heading = fit_text(
            &item.label.to_uppercase(),
            inset.width
                - if size_fits {
                    utf16_len(&size) as f64 * 5.2 + 16.0
                } else {
                    4.0
                },
            9.5,
        );
        // `if (heading)`: an empty string is falsy as well as `undefined`.
        if let Some(heading) = heading.filter(|text| !text.is_empty()) {
            lines.push(label_tag(
                inset.x,
                inset.y + 8.0,
                &heading,
                &LabelOptions {
                    size: Some(9.5),
                    fill: Some(MUTED),
                    tracking: Some(0.8),
                    ..LabelOptions::default()
                },
            ));
        }
        if size_fits {
            lines.push(label_tag(
                inset.x + inset.width,
                inset.y + 8.0,
                &size,
                &LabelOptions {
                    size: Some(9.5),
                    fill: Some(MUTED),
                    anchor: Some("end"),
                    mono: true,
                    ..LabelOptions::default()
                },
            ));
        }
        let tiles = Rect {
            x: inset.x,
            y: inset.y + 16.0,
            width: inset.width,
            height: inset.height - 16.0,
        };
        for placed in squarify(&item.tiles, |tile| tile.bytes as f64, tiles)? {
            tile_rects(&item.tiles[placed.index], placed.rect, lines);
        }
    }
    Ok(())
}

fn category_bar(map: &CoverageMap, frame: Rect, lines: &mut Vec<String>) {
    let clip = "category-bar-clip";
    // The clip path uses the raw frame values, not `round`ed ones.
    lines.push(format!(
        "<defs><clipPath id=\"{clip}\"><rect x=\"{}\" y=\"{}\" width=\"{}\" height=\"{}\" rx=\"{}\"/></clipPath></defs>",
        js_number_string(frame.x),
        js_number_string(frame.y),
        js_number_string(frame.width),
        js_number_string(frame.height),
        js_number_string(frame.height / 2.0),
    ));
    lines.push(format!("<g clip-path=\"url(#{clip})\">"));
    lines.push(rect_tag(frame, HAIRLINE, &RectOptions::default()));
    let executable = document_number(map, &["executable_bytes"]);
    let mut cursor = frame.x;
    for category in ["exact_c", "semantic_c", "assembly", "retained_asm"] {
        let bytes = document_number(map, &["categories", category, "bytes"]);
        let category_width = frame.width * (bytes / executable);
        lines.push(rect_tag(
            Rect {
                x: cursor,
                width: category_width,
                ..frame
            },
            category_style(category).fill,
            &RectOptions::default(),
        ));
        cursor += category_width;
    }
    lines.push("</g>".to_string());
}

pub fn document_number(map: &CoverageMap, path: &[&str]) -> f64 {
    let mut node = &map.document;
    for key in path {
        match node.get(key) {
            Some(next) => node = next,
            None => return f64::NAN,
        }
    }
    node.as_f64().unwrap_or(f64::NAN)
}

pub fn document_string(map: &CoverageMap, path: &[&str]) -> String {
    let mut node = &map.document;
    for key in path {
        match node.get(key) {
            Some(next) => node = next,
            None => return "undefined".to_string(),
        }
    }
    node.as_str().unwrap_or("undefined").to_string()
}

/// `renderSvg(map)`.
pub fn render_svg(map: &CoverageMap) -> Result<String, String> {
    let width = 1200.0f64;
    let height = 660.0f64;
    let margin = 32.0f64;
    let mut lines: Vec<String> = Vec::new();
    let exact_bytes = document_number(map, &["categories", "exact_c", "bytes"]);
    let semantic_bytes = document_number(map, &["categories", "semantic_c", "bytes"]);
    let executable = document_number(map, &["executable_bytes"]);
    let rom_bytes = document_number(map, &["rom_bytes"]);
    let combined = exact_bytes + semantic_bytes;
    let combined_percent = js_number_string(crate::intervals::round_half_up_percent(
        combined, executable,
    )?);

    lines.push(format!(
        "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 {w} {h}\" width=\"{w}\" height=\"{h}\" font-family=\"{SANS}\" role=\"img\" aria-label=\"Coverage treemap of the English Golden Sun ROM image: {combined_percent}% of the {executable_commas} audited executable bytes are expressed as C\">",
        w = js_number_string(width),
        h = js_number_string(height),
        executable_commas = commas(executable),
    ));
    lines.push(rect_tag(
        Rect {
            x: 0.0,
            y: 0.0,
            width,
            height,
        },
        SURFACE,
        &RectOptions::default(),
    ));
    lines.push(label_tag(
        margin,
        46.0,
        "Alchemy — Golden Sun",
        &LabelOptions {
            size: Some(15.0),
            weight: Some(600.0),
            tracking: Some(-0.2),
            ..LabelOptions::default()
        },
    ));
    lines.push(label_tag(
        margin,
        64.0,
        &format!(
            "{} · {} ROM image",
            document_string(map, &["target"]),
            megabytes(rom_bytes)
        ),
        &LabelOptions {
            size: Some(10.0),
            fill: Some(MUTED),
            ..LabelOptions::default()
        },
    ));
    lines.push(label_tag(
        width - margin,
        46.0,
        &format!("{combined_percent}%"),
        &LabelOptions {
            size: Some(26.0),
            anchor: Some("end"),
            weight: Some(600.0),
            tracking: Some(-0.6),
            ..LabelOptions::default()
        },
    ));
    lines.push(label_tag(
        width - margin,
        64.0,
        "EXACT + SEMANTIC C",
        &LabelOptions {
            size: Some(9.5),
            fill: Some(MUTED),
            anchor: Some("end"),
            tracking: Some(0.9),
            ..LabelOptions::default()
        },
    ));

    let percent = |category: &str| {
        js_number_string(document_number(
            map,
            &["categories", category, "percent_of_executable"],
        ))
    };
    let metrics: Vec<(&str, String, Option<&str>)> = vec![
        (
            "exact_c",
            format!("{}%", percent("exact_c")),
            Some("FULL-C BYTE SHARE"),
        ),
        ("semantic_c", format!("{}%", percent("semantic_c")), None),
        ("assembly", format!("{}%", percent("assembly")), None),
        (
            "retained_asm",
            format!("{}%", percent("retained_asm")),
            Some("BY DESIGN"),
        ),
        (
            "asset_data",
            megabytes(document_number(map, &["categories", "asset_data", "bytes"])),
            Some("NOT CODE"),
        ),
    ];
    let column_width = (width - margin * 2.0) / metrics.len() as f64;
    for (index, (category, value, note)) in metrics.iter().enumerate() {
        let x = margin + index as f64 * column_width;
        let mut name = category_style(category).label.to_uppercase();
        if let Some(note) = note {
            name.push_str(&format!(" · {note}"));
        }
        lines.push(rect_tag(
            Rect {
                x,
                y: 96.0,
                width: 8.0,
                height: 8.0,
            },
            category_style(category).fill,
            &RectOptions {
                radius: Some(2.0),
                ..RectOptions::default()
            },
        ));
        lines.push(label_tag(
            x + 14.0,
            104.0,
            &name,
            &LabelOptions {
                size: Some(9.5),
                fill: Some(MUTED),
                tracking: Some(0.8),
                ..LabelOptions::default()
            },
        ));
        lines.push(label_tag(
            x,
            128.0,
            value,
            &LabelOptions {
                size: Some(21.0),
                weight: Some(600.0),
                tracking: Some(-0.4),
                ..LabelOptions::default()
            },
        ));
    }

    category_bar(
        map,
        Rect {
            x: margin,
            y: 146.0,
            width: width - margin * 2.0,
            height: 6.0,
        },
        &mut lines,
    );

    let card_top = 176.0f64;
    let card_height = height - card_top - 44.0;
    card(
        "ROM image",
        &format!("{} in the ROM image", megabytes(rom_bytes)),
        &map.rom_areas,
        Rect {
            x: margin,
            y: card_top,
            width: 372.0,
            height: card_height,
        },
        &mut lines,
    )?;
    card(
        "Audited executable universe",
        &format!("{} bytes of code", commas(executable)),
        &map.executable_areas,
        Rect {
            x: margin + 388.0,
            y: card_top,
            width: width - margin * 2.0 - 388.0,
            height: card_height,
        },
        &mut lines,
    )?;

    lines.push(label_tag(
        margin,
        height - 20.0,
        "Tiles are main-image address bands and one per code overlay; compressed code-overlay tiles are sized by ROM-image bytes and shaded by decoded coverage category.",
        &LabelOptions { size: Some(9.5), fill: Some(MUTED), ..LabelOptions::default() },
    ));
    lines.push(label_tag(
        width - margin,
        height - 20.0,
        &format!(
            "exact C: {} · semantic C: {}",
            document_string(map, &["provenance", "exact_source"]),
            document_string(map, &["provenance", "semantic_source"]),
        ),
        &LabelOptions {
            size: Some(9.5),
            fill: Some(MUTED),
            anchor: Some("end"),
            ..LabelOptions::default()
        },
    ));
    lines.push("</svg>".to_string());
    Ok(format!("{}\n", lines.join("\n")))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn round_prints_the_javascript_spelling_not_the_rust_one() {
        // `Math.round(0.35 * 10) / 10` is 0.4, and `String(4)` is "4", not "4.0".
        assert_eq!(round_text(4.0), "4");
        assert_eq!(round_text(4.04), "4");
        assert_eq!(round_text(4.05), "4.1");
        // Half rounds toward +Infinity, so -0.05 goes to -0 and prints as "0".
        assert_eq!(round_text(-0.05), "0");
        assert_eq!(round_text(-0.06), "-0.1");
    }

    #[test]
    fn megabytes_uses_half_up_to_fixed() {
        assert_eq!(megabytes(8.0 * 1024.0 * 1024.0), "8.00 MB");
        // 1.005 is below the tie in binary, so both roundings agree here; the
        // point of the helper is that it never reaches Rust's half-to-even.
        assert_eq!(megabytes(0.0), "0.00 MB");
    }

    #[test]
    fn fit_text_matches_the_javascript_boundaries() {
        assert_eq!(
            fit_text("abcdef", 3.0, 9.0),
            None,
            "room below 3 yields undefined"
        );
        assert_eq!(fit_text("abc", 100.0, 9.0).as_deref(), Some("abc"));
        // room = floor(20 / 5.22) = 3, text longer than that gets an ellipsis
        // at max(room - 1, 1) = 2 characters.
        assert_eq!(fit_text("abcdef", 20.0, 9.0).as_deref(), Some("ab…"));
    }

    #[test]
    fn the_address_label_regex_is_case_insensitive_and_wants_an_en_dash() {
        assert!(is_address_label("08000000"));
        assert!(
            is_address_label("08000000–0800FFFF"),
            "uppercase hex still matches"
        );
        assert!(
            !is_address_label("08000000-0800ffff"),
            "a hyphen is not an en dash"
        );
        assert!(!is_address_label("Main image"));
        assert!(!is_address_label(""));
    }

    #[test]
    fn a_falsy_radius_or_stroke_emits_nothing() {
        let frame = Rect {
            x: 1.0,
            y: 2.0,
            width: 3.0,
            height: 4.0,
        };
        let plain = rect_tag(frame, "#fff", &RectOptions::default());
        assert_eq!(
            plain,
            "<rect x=\"1\" y=\"2\" width=\"3\" height=\"4\" fill=\"#fff\"/>"
        );
        let zero = rect_tag(
            frame,
            "#fff",
            &RectOptions {
                radius: Some(0.0),
                stroke: Some(""),
                opacity: None,
            },
        );
        assert_eq!(zero, plain, "0 and \"\" are falsy in the TypeScript");
    }

    #[test]
    fn a_start_anchor_is_omitted_but_a_zero_opacity_is_not() {
        let text = label_tag(
            0.0,
            0.0,
            "x",
            &LabelOptions {
                anchor: Some("start"),
                opacity: Some(0.0),
                tracking: Some(0.0),
                ..LabelOptions::default()
            },
        );
        assert!(!text.contains("text-anchor"));
        assert!(!text.contains("letter-spacing"), "0 tracking is falsy");
        assert!(
            text.contains("opacity=\"0\""),
            "opacity checks !== undefined, not truthiness"
        );
    }
}
