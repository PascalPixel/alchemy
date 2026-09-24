//! The README's two figures, drawn in whole game pixels from the tracked
//! glyph sheet and written at `FIGURE_SCALE`: PROGRESS_CHART.png, each
//! game's DONE by calendar day, and PROGRESS.png, the map of tracked files.
//! Both are opaque, so they read the same on light and dark pages.
use super::boxtree::{
    color, content_mix, content_style, directories, disk_tiles, display_bytes, legend_items, quiet,
    source_name, tracked_only, BEVEL_DARK, BEVEL_LIGHT, CHART_BACKGROUND, DISPLAY_CATEGORIES,
};
use super::history::{day_number, percent};
use super::jsnum::commas;
use super::letters::{Letters, LINE};
use super::model::{treemap, Category, Rect, Tile};
use super::raster::Canvas;
use serde_json::Value;
use std::path::Path;

/// Both figures are this many game pixels wide, shown at 830 CSS pixels.
pub(crate) const WIDTH: i32 = 830;
pub(crate) const CHART: &str = "PROGRESS_CHART.png";
pub(crate) const MAP: &str = "PROGRESS.png";
const INK: &str = "#fff";
const SHADOW: &str = "#000";
/// The Broken Seal in gold, The Lost Age in a pale window blue; the plot is
/// a darker teal well with quiet grid lines and lighter stricter-rule days.
const GOLD: &str = "#f4c84f";
const BLUE: &str = "#a8c4f8";
const WELL: &str = "#17606f";
const GRID: &str = "#246f7e";
const STRICTER: &str = "#3a8c9c";
const MUTED: &str = "#b4ccd2";

// ------------------------------------------------------------------ chart

/// The daily chart of `history`: x is calendar days since the project
/// began, y is 0–100%, stricter-rule days are light bands, and today's
/// values are labelled at the right end.
pub(crate) fn chart(letters: &Letters, history: &Value) -> Canvas {
    let height = 256;
    let mut canvas = Canvas::new(WIDTH, height, CHART_BACKGROUND);
    canvas.bevel(0, 0, WIDTH, height, BEVEL_LIGHT, BEVEL_DARK);
    let began = history["began"]
        .as_str()
        .and_then(day_number)
        .unwrap_or_default();
    let days = history["days"].as_array().cloned().unwrap_or_default();
    let last = days
        .iter()
        .filter_map(|row| row["date"].as_str().and_then(day_number))
        .max()
        .unwrap_or(began)
        .max(began + 1);
    let series = [
        ("tbs", "The Broken Seal", GOLD),
        ("tla", "The Lost Age", BLUE),
    ];
    let latest = series.map(|(key, _, _)| {
        days.iter()
            .rev()
            .find_map(|row| percent(&row[key]))
            // Floored to two places, as the README's status line reads.
            .map(|value| format!("{:.2}%", ((value * 100.0) + 1e-6).floor() / 100.0))
    });
    // Title and key along the top line.
    canvas.text(letters, 8, 6, "DONE by day", INK, Some(SHADOW));
    let mut key_x = WIDTH - 8;
    for (_, name, ink) in series.iter().rev() {
        key_x -= letters.width(name) as i32;
        canvas.text(letters, key_x, 6, name, INK, Some(SHADOW));
        key_x -= 14;
        canvas.fill(key_x, 10, 10, 10, SHADOW);
        canvas.fill(key_x - 1, 9, 10, 10, ink);
        key_x -= 12;
    }
    let stricter = "Stricter rules";
    key_x -= letters.width(stricter) as i32;
    canvas.text(letters, key_x, 6, stricter, INK, Some(SHADOW));
    canvas.fill(key_x - 14, 9, 10, 10, STRICTER);
    // The plot well.
    let label_room = latest
        .iter()
        .flatten()
        .map(|label| letters.width(label) as i32)
        .max()
        .unwrap_or(0)
        + 12;
    let (left, top) = (40, 32);
    let (right, bottom) = (WIDTH - 8 - label_room, height - 32);
    let (plot_w, plot_h) = (right - left, bottom - top);
    let span = (last - began) as i32;
    let x_of = |day: i64| left + ((day - began) as i32 * (plot_w - 1) + span / 2) / span;
    let y_of = |value: f64| {
        bottom - 1 - ((value.clamp(0.0, 100.0) / 100.0) * (plot_h - 2) as f64).round() as i32
    };
    canvas.fill(left, top, plot_w, plot_h, WELL);
    for change in history["stricter"].as_array().into_iter().flatten() {
        if let Some(day) = change["date"].as_str().and_then(day_number) {
            // The band covers the step into the stricter day.
            let (from, to) = (x_of(day - 1) + 1, x_of(day) + 1);
            canvas.fill(from, top + 1, to - from, plot_h - 2, STRICTER);
            canvas.fill(from, top - 4, to - from, 3, STRICTER);
        }
    }
    for quarter in 0..=4 {
        let value = quarter as f64 * 25.0;
        let y = y_of(value);
        if quarter > 0 && quarter < 4 {
            canvas.fill(left + 1, y, plot_w - 2, 1, GRID);
        }
        let label = format!("{}%", quarter * 25);
        let width = letters.width(&label) as i32;
        canvas.text(
            letters,
            left - 6 - width,
            y - 8,
            &label,
            MUTED,
            Some(SHADOW),
        );
    }
    canvas.bevel(
        left - 1,
        top - 1,
        plot_w + 2,
        plot_h + 2,
        BEVEL_DARK,
        BEVEL_LIGHT,
    );
    // Month starts, and the first day, along the bottom.
    let mut labelled_until = i32::MIN;
    for day in began..=last {
        let (name, first) = super::history::day_label(day);
        if !(first || day == began) {
            continue;
        }
        let x = x_of(day);
        canvas.fill(x, bottom + 1, 1, 3, MUTED);
        let width = letters.width(&name) as i32;
        let at = (x - width / 2).clamp(left - 8, WIDTH - 8 - width);
        if at > labelled_until + 8 {
            canvas.text(letters, at, bottom + 6, &name, MUTED, Some(SHADOW));
            labelled_until = at + width;
        }
    }
    // The lines, then today's values beside their ends.
    let mut ends = Vec::new();
    for ((key, _, ink), label) in series.iter().zip(&latest) {
        let points = days
            .iter()
            .filter_map(|row| {
                let day = row["date"].as_str().and_then(day_number)?;
                Some((x_of(day), y_of(percent(&row[*key])?)))
            })
            .collect::<Vec<_>>();
        for pair in points.windows(2) {
            canvas.line(
                (pair[0].0, pair[0].1 - 1),
                (pair[1].0, pair[1].1 - 1),
                2,
                ink,
            );
        }
        if let (Some(end), Some(label)) = (points.last(), label) {
            canvas.fill(end.0 - 1, end.1 - 2, 4, 4, ink);
            ends.push((end.1, label.clone(), *ink));
        }
    }
    ends.sort_by_key(|(y, _, _)| *y);
    let mut floor = i32::MIN;
    for (y, label, ink) in ends {
        let at = (y - 8).max(floor).min(bottom - 16);
        canvas.text(letters, right + 8, at, &label, ink, Some(SHADOW));
        floor = at + LINE as i32;
    }
    canvas
}

// -------------------------------------------------------------------- map

/// The tracked files of the Camelot-shaped trees and `recon/`, by size on
/// disk, in the dashboard's palette.
pub(crate) fn map(letters: &Letters, root: &Path) -> Canvas {
    map_of(letters, tracked_only(root, disk_tiles(root)))
}
pub(crate) fn map_of(letters: &Letters, tiles: Vec<Tile>) -> Canvas {
    let height = (WIDTH * 16 + 8) / 9;
    let mut canvas = Canvas::new(WIDTH, height, CHART_BACKGROUND);
    let total: i64 = tiles.iter().map(|tile| tile.bytes).sum();
    let legend = legend_items(&tiles.iter().collect::<Vec<_>>())
        .into_iter()
        .map(|(name, swatch, bytes)| {
            let share = 100.0 * bytes as f64 / total.max(1) as f64;
            (format!("{name} {share:.1}%"), swatch)
        })
        .collect::<Vec<_>>();
    let entry = |label: &str| 24 + letters.width(label) as i32;
    let mut rows = 1;
    let mut row_x = 8;
    for (label, _) in &legend {
        if row_x > 8 && row_x + entry(label) > WIDTH - 8 {
            rows += 1;
            row_x = 8;
        }
        row_x += entry(label);
    }
    canvas.text(letters, 8, 8, "Alchemy files", INK, Some(SHADOW));
    let corner = commas(total);
    let corner_width = letters.width(&corner) as i32;
    canvas.text(
        letters,
        WIDTH - 8 - corner_width,
        8,
        &corner,
        INK,
        Some(SHADOW),
    );
    let frame = (4, 32, WIDTH - 8, height - 44 - rows * 24);
    canvas.fill(frame.0, frame.1, frame.2, frame.3, INK);
    let nested = directories(tiles, "");
    draw(
        &mut canvas,
        letters,
        &nested.iter().collect::<Vec<_>>(),
        frame,
        None,
        &mut Vec::new(),
    );
    let (mut x, mut y) = (8, frame.1 + frame.3 + 12);
    for (label, swatch) in legend {
        if x > 8 && x + entry(&label) > WIDTH - 8 {
            x = 8;
            y += 24;
        }
        canvas.fill(x, y, 16, 16, SHADOW);
        canvas.fill(x, y, 15, 15, swatch);
        canvas.text(letters, x + 20, y, &label, INK, Some(SHADOW));
        x += entry(&label);
    }
    canvas.bevel(0, 0, WIDTH, height, BEVEL_LIGHT, BEVEL_DARK);
    canvas
}

type Box = (i32, i32, i32, i32);

/// A treemap cell on whole pixels: each edge rounds on its own, so
/// neighbours share their boundary exactly.
fn snap(rect: Rect) -> Box {
    let (x0, y0) = (rect.x.round() as i32, rect.y.round() as i32);
    let (x1, y1) = (
        (rect.x + rect.width).round() as i32,
        (rect.y + rect.height).round() as i32,
    );
    (x0, y0, x1 - x0, y1 - y0)
}
fn overlaps(a: Box, b: Box) -> bool {
    a.0 < b.0 + b.2 && a.0 + a.2 > b.0 && a.1 < b.1 + b.3 && a.1 + a.3 > b.1
}

fn draw(
    canvas: &mut Canvas,
    letters: &Letters,
    tiles: &[&Tile],
    frame: Box,
    parent_source: Option<&str>,
    reserved: &mut Vec<Box>,
) {
    let area = Rect {
        x: frame.0 as f64,
        y: frame.1 as f64,
        width: frame.2 as f64,
        height: frame.3 as f64,
    };
    for placed in treemap(tiles, |tile| tile.bytes, area) {
        let tile = tiles[placed.index];
        let cell = snap(placed.rect);
        if cell.2 <= 0 || cell.3 <= 0 {
            continue;
        }
        let container = !tile.children.is_empty();
        let folder = container && tile.source.as_deref().is_some_and(|s| s.ends_with('/'));
        let inset = if container && cell.2 >= 8 && cell.3 >= 8 {
            2
        } else {
            0
        };
        let mut body = if container {
            (
                cell.0 + inset,
                cell.1 + inset,
                cell.2 - 2 * inset,
                cell.3 - 2 * inset,
            )
        } else {
            // A file leaves its right and bottom pixel to the frame behind it.
            (cell.0, cell.1, (cell.2 - 1).max(1), (cell.3 - 1).max(1))
        };
        let expanded = container && body.2 >= 44 && body.3 >= 34;
        if expanded {
            canvas.fill(cell.0, cell.1, cell.2, cell.3, CHART_BACKGROUND);
            canvas.bevel(cell.0, cell.1, cell.2, cell.3, BEVEL_LIGHT, BEVEL_DARK);
        } else {
            stack(canvas, tile, body);
        }
        let name = match tile.source.as_deref() {
            Some(source) if !source.ends_with('/') || container => source_name(source),
            Some(_) => &tile.label,
            None => "",
        };
        let shown = (folder || container || !quiet(tile))
            && (tile.source.is_none()
                || tile.source.as_deref() != parent_source
                || (!container && tile.source.as_deref().is_some_and(|p| p.ends_with('/'))));
        let caption = caption(letters, name, body, folder)
            .filter(|(_, bounds)| shown && !reserved.iter().any(|r| overlaps(*bounds, *r)));
        if let Some((lines, bounds)) = &caption {
            if folder {
                canvas.text(letters, bounds.0, bounds.1, &lines[0], INK, Some(SHADOW));
                if expanded {
                    body.1 += LINE as i32 + 2;
                    body.3 -= LINE as i32 + 2;
                }
            } else {
                reserved.push(*bounds);
            }
        }
        if expanded {
            draw(
                canvas,
                letters,
                &tile.children.iter().collect::<Vec<_>>(),
                body,
                tile.source.as_deref(),
                reserved,
            );
        }
        if let Some((lines, bounds)) = caption.filter(|_| !folder) {
            for (row, line) in lines.iter().enumerate() {
                let width = letters.width(line) as i32;
                let x = bounds.0 + (bounds.2 - width) / 2;
                canvas.text(
                    letters,
                    x,
                    bounds.1 + row as i32 * LINE as i32,
                    line,
                    INK,
                    Some(SHADOW),
                );
            }
        }
    }
}
/// A tile's fill: its kinds stacked by bytes, the largest share at the bottom.
fn stack(canvas: &mut Canvas, tile: &Tile, body: Box) {
    let parts = if tile.categories[Category::AssetData as usize] == tile.bytes {
        content_mix(tile)
            .into_iter()
            .map(|(_, swatch, bytes)| (swatch, bytes))
            .collect::<Vec<_>>()
    } else {
        DISPLAY_CATEGORIES
            .iter()
            .map(|(category, _)| {
                let swatch = if *category == Category::AssetData {
                    content_style(tile).1
                } else {
                    color(*category)
                };
                (swatch, display_bytes(&tile.categories, *category))
            })
            .filter(|(_, bytes)| *bytes > 0)
            .collect()
    };
    let total = tile.bytes.max(1) as f64;
    let mut below = 0.0;
    let bottom = body.1 + body.3;
    for (swatch, bytes) in parts {
        let from = bottom - (below * body.3 as f64 / total).round() as i32;
        below += bytes as f64;
        let to = bottom - (below * body.3 as f64 / total).round() as i32;
        canvas.fill(body.0, to, body.2, from - to, swatch);
    }
}
/// A name that fits its box: one line on a folder's heading, or a file's
/// name broken at `_ . - space` into centred lines on boxes large enough to
/// read at a glance.
fn caption(letters: &Letters, name: &str, body: Box, folder: bool) -> Option<(Vec<String>, Box)> {
    let line = LINE as i32;
    if name.is_empty() {
        return None;
    }
    if folder {
        let width = letters.width(name) as i32;
        if body.2 < width + 4 || body.3 < line + 4 {
            return None;
        }
        return Some((
            vec![name.to_string()],
            (body.0 + 2, body.1, width, line + 2),
        ));
    }
    let room = body.2 - 8;
    if body.3 < line + 12 || body.2 < 64 {
        return None;
    }
    let mut lines = Vec::new();
    let mut current = String::new();
    for part in name.split_inclusive(['_', '.', '-', ' ']) {
        if letters.width(part) as i32 > room {
            return None;
        }
        if letters.width(&format!("{current}{part}")) as i32 > room {
            lines.push(std::mem::take(&mut current));
        }
        current.push_str(part);
    }
    if !current.is_empty() {
        lines.push(current);
    }
    let rows = lines.len() as i32;
    if rows * line + 8 > body.3 {
        return None;
    }
    let width = lines
        .iter()
        .map(|l| letters.width(l) as i32)
        .max()
        .unwrap_or(0);
    let bounds = (
        body.0 + (body.2 - width) / 2,
        body.1 + (body.3 - rows * line) / 2,
        width,
        rows * line,
    );
    Some((lines, bounds))
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::coverage::letters::fixture;
    use serde_json::json;
    #[test]
    fn folder_names_fit_by_their_glyph_advances() {
        let letters = fixture();
        let body = (0, 0, 16, 20);
        let (lines, bounds) = caption(&letters, "LIB", body, true).unwrap();
        assert_eq!((lines[0].as_str(), bounds.2), ("LIB", 12));
        assert!(caption(&letters, "LIBS", body, true).is_none());
        assert!(caption(&letters, "LIB", (0, 0, 16, 19), true).is_none());
        assert!(caption(&letters, "LIB", body, false).is_none());
        let (lines, _) = caption(&letters, "STILL_IMAGE.PNG", (0, 0, 64, 60), false).unwrap();
        assert_eq!(lines, ["STILL_IMAGE.", "PNG"]);
    }
    #[test]
    fn the_map_draws_folders_with_bevels_on_whole_pixels() {
        let letters = fixture();
        let tile = |source: &str, bytes| Tile {
            label: source.rsplit('/').next().unwrap().into(),
            source: Some(source.into()),
            bytes,
            categories: [0, 0, 0, 0, 0, bytes],
            group: Some(format!(
                "file:{}",
                source.rsplit('.').next().unwrap().to_lowercase()
            )),
            ..Tile::default()
        };
        let canvas = map_of(
            &letters,
            vec![tile("games/X/SRC/A.C", 600), tile("games/X/SRC/B.S", 400)],
        );
        assert_eq!((canvas.width, canvas.height), (830, 1476));
        // The outer frame's bevel, and the games folder's own at its corner.
        assert_eq!(
            canvas.get(0, 0),
            Some(super::super::raster::rgb(BEVEL_LIGHT))
        );
        assert_eq!(
            canvas.get(4, 32),
            Some(super::super::raster::rgb(BEVEL_LIGHT))
        );
        let png = canvas.png(2, "2026-09-24").unwrap();
        let again = map_of(
            &letters,
            vec![tile("games/X/SRC/A.C", 600), tile("games/X/SRC/B.S", 400)],
        );
        assert_eq!(png, again.png(2, "2026-09-24").unwrap());
    }
    #[test]
    fn the_chart_starts_each_line_at_its_first_measurement() {
        let letters = fixture();
        let history = json!({"began": "2026-07-16", "stricter": [{"date": "2026-09-09"}], "days": [
            {"date": "2026-07-16", "tbs": {"percent": 1.0}},
            {"date": "2026-09-09", "tbs": {"percent": 44.0}},
            {"date": "2026-09-18", "tbs": {"percent": 56.0}, "tla": {"percent": 0.2}},
            {"date": "2026-09-24", "tbs": {"done": 6486, "executable": 10000}, "tla": {"done": 213, "executable": 10000}}
        ]});
        let canvas = chart(&letters, &history);
        assert_eq!((canvas.width, canvas.height), (830, 256));
        let gold = super::super::raster::rgb(GOLD);
        let blue = super::super::raster::rgb(BLUE);
        let column = |x: i32, ink| (0..256).any(|y| canvas.get(x, y) == Some(ink));
        // Gold runs from the first day; blue only from 18 September.
        assert!(column(41, gold));
        assert!(!column(41, blue) && !column(300, blue));
        let right = (0..830).rev().find(|x| column(*x, blue)).unwrap();
        assert!(right > 600);
    }
}
