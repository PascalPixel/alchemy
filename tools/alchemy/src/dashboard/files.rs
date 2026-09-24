//! Files: every file of the Camelot-shaped game trees and of the `recon/`
//! scaffolding beside them, drawn by size on disk in the figure's palette.
use super::cache;
use super::chrome;
use super::http::{self, Response};
use crate::coverage::boxtree::{
    color, content_mix, content_style, directories, disk_tiles, display_bytes, esc, legend_items,
    source_name, DISPLAY_CATEGORIES,
};
use crate::coverage::jsnum::commas;
use crate::coverage::letters::{LINE, PIXEL};
use crate::coverage::model::{treemap, Category, Rect, Tile};
use std::path::Path;

fn encode(value: &str) -> String {
    value.bytes().map(|byte| format!("{byte:02x}")).collect()
}
pub(super) fn decode_folder(value: &str) -> Option<String> {
    if value.len() % 2 != 0 {
        return None;
    }
    let bytes = value
        .as_bytes()
        .chunks_exact(2)
        .map(|pair| {
            let high = (pair[0] as char).to_digit(16)?;
            let low = (pair[1] as char).to_digit(16)?;
            Some((high * 16 + low) as u8)
        })
        .collect::<Option<Vec<_>>>()?;
    let folder = String::from_utf8(bytes).ok()?;
    if !folder.is_empty()
        && (!folder.ends_with('/')
            || folder.starts_with('/')
            || folder.split('/').any(|part| part == ".." || part == "."))
    {
        return None;
    }
    Some(folder)
}
fn url(folder: &str) -> String {
    if folder.is_empty() {
        "/".into()
    } else {
        format!("/view/{}", encode(folder))
    }
}
/// The CSS width a name needs as glyph sprites, plus the label's own four
/// pixels of padding.
fn name_width(name: &str) -> usize {
    (super::glyphs::width(name) + 4 * PIXEL) as usize
}
fn parent(folder: &str) -> &str {
    folder
        .trim_end_matches('/')
        .rsplit_once('/')
        .map_or("", |(parent, _)| &folder[..parent.len() + 1])
}
/// A folder's name bar is one text line whatever the window size; the layout
/// estimates it in chart units only to shape its children.
const HEADING_UNITS: f64 = 6.0;
/// A folder opens only when its body is this large, as in the figure; a
/// smaller one is drawn by the file types it holds and links to its view.
const OPEN_FOLDER: (f64, f64) = (44.0, 34.0);
/// Stacked fills, largest share at the bottom, as one CSS background.
fn fill(tile: &Tile, collapsed: bool) -> String {
    let parts = if collapsed || tile.categories[Category::AssetData as usize] == tile.bytes {
        content_mix(tile)
            .into_iter()
            .map(|(_, color, bytes)| (color, bytes))
            .collect::<Vec<_>>()
    } else {
        DISPLAY_CATEGORIES
            .iter()
            .map(|(category, _)| {
                let color = if *category == Category::AssetData {
                    content_style(tile).1
                } else {
                    color(*category)
                };
                (color, display_bytes(&tile.categories, *category))
            })
            .filter(|(_, bytes)| *bytes > 0)
            .collect()
    };
    if parts.len() == 1 {
        return parts[0].0.into();
    }
    let total = parts.iter().map(|(_, bytes)| bytes).sum::<i64>().max(1) as f64;
    let mut at = 0.0;
    let stops = parts
        .iter()
        .map(|(color, bytes)| {
            let end = at + *bytes as f64 / total * 100.0;
            let stop = format!("{color} {at:.2}% {end:.2}%");
            at = end;
            stop
        })
        .collect::<Vec<_>>();
    format!("linear-gradient(to top,{})", stops.join(","))
}
/// Draw `entries` into a box `area` chart units wide and tall. Each folder
/// holds its children below its name bar, so bars keep a fixed pixel height
/// while tiles scale with the window.
fn tiles(
    out: &mut String,
    entries: &[&Tile],
    frame: Rect,
    area: (f64, f64),
    widths: &mut std::collections::BTreeSet<usize>,
) {
    for placed in treemap(entries, |tile| tile.bytes, frame) {
        let tile = entries[placed.index];
        let rect = placed.rect;
        let nested = !tile.children.is_empty();
        let open =
            nested && rect.width - 4.0 >= OPEN_FOLDER.0 && rect.height - 4.0 >= OPEN_FOLDER.1;
        let source = tile.source.as_deref().unwrap_or("");
        let directory = source.ends_with('/') && nested;
        let name = if source.is_empty() || (source.ends_with('/') && !nested) {
            &tile.label
        } else {
            source_name(source)
        };
        let title = format!(
            "{name}: {} bytes{}",
            commas(tile.bytes),
            if nested && !open {
                format!(" · {} items", tile.children.len())
            } else {
                String::new()
            }
        );
        let href = if directory {
            url(source)
        } else {
            format!("/file/{}", encode(&format!("{source}/")))
        };
        let minimum = name_width(name);
        widths.insert(minimum);
        let place = format!(
            "left:{:.3}%;top:{:.3}%;width:{:.3}%;height:{:.3}%",
            rect.x / area.0 * 100.0,
            rect.y / area.1 * 100.0,
            rect.width / area.0 * 100.0,
            rect.height / area.1 * 100.0,
        );
        if !open {
            let size = if nested {
                String::new()
            } else {
                format!(
                    "<small class=\"file-size\">{} bytes</small>",
                    commas(tile.bytes)
                )
            };
            out.push_str(&format!(
                "<a class=\"tile leaf label-w{minimum}\" href=\"{href}\" title=\"{}\" style=\"{place};background:{}\"><span>{}</span>{size}</a>",
                esc(&title),
                fill(tile, nested),
                esc(name)
            ));
            continue;
        }
        out.push_str(&format!(
            "<div class=\"tile container\" style=\"{place}\" title=\"{}\"><a class=\"folder-label label-w{minimum}\" href=\"{href}\"><span>{}</span></a>",
            esc(&title),
            esc(name)
        ));
        let heading = if directory { HEADING_UNITS } else { 0.0 };
        let inner = (
            (rect.width - 4.0).max(1.0),
            (rect.height - 4.0 - heading).max(1.0),
        );
        out.push_str(if directory {
            "<div class=\"area headed\">"
        } else {
            "<div class=\"area\">"
        });
        tiles(
            out,
            &tile.children.iter().collect::<Vec<_>>(),
            Rect {
                x: 0.0,
                y: 0.0,
                width: inner.0,
                height: inner.1,
            },
            inner,
            widths,
        );
        out.push_str("</div></div>");
    }
}
fn reveal_form(source: &str) -> String {
    let path: String = source.bytes().map(|b| format!("%{b:02X}")).collect();
    format!("<form method=\"post\" action=\"/reveal/{path}\" target=\"reveal-result\"><button type=\"submit\">Show in Finder</button></form>")
}

fn properties(file: &Tile, folder: &str) -> String {
    let path = file.source.as_deref().unwrap_or("");
    let target = if path.starts_with("games/THE LOST AGE/") || path.starts_with("recon/tla/") {
        "tla-en"
    } else {
        "tbs-en"
    };
    let rom = match super::roms::attribution(target, path) {
        Some((regions, bytes, current)) => format!(
            "{} regions · {} bytes · {target}{}",
            commas(regions as i64),
            commas(bytes),
            if current { "" } else { " (stale index)" }
        ),
        None => "—".into(),
    };
    format!(
        "<div class=\"dialog floating\" role=\"dialog\" aria-labelledby=\"properties-title\"><div class=\"titlebar\"><span id=\"properties-title\">{} Properties</span><a class=\"close\" href=\"{}\" aria-label=\"Close\">×</a></div><div class=\"dialog-body\"><dl class=\"properties\"><dt>Type</dt><dd>{}</dd><dt>Location</dt><dd>{}</dd><dt>Size</dt><dd>{} bytes</dd><dt>ROM</dt><dd>{}</dd></dl><div class=\"dialog-buttons\">{}<a class=\"button\" href=\"{}\">Close</a></div></div></div><iframe name=\"reveal-result\" class=\"action-result\" title=\"Finder action result\"></iframe>",
        esc(&file.label),
        url(folder),
        esc(content_style(file).0),
        esc(parent(&format!("{path}/"))),
        commas(file.bytes),
        esc(&rom),
        reveal_form(path),
        url(folder),
    )
}
/// The treemap for one folder, with an optional file's properties.
fn render(root: &Path, folder: &str, selected: Option<&str>) -> Option<(String, String)> {
    let files = disk_tiles(root);
    let chosen = match selected {
        Some(path) => Some(files.iter().find(|t| t.source.as_deref() == Some(path))?),
        None => None,
    };
    let entries = files
        .iter()
        .filter(|tile| {
            folder.is_empty()
                || tile
                    .source
                    .as_deref()
                    .is_some_and(|source| source.starts_with(folder))
        })
        .cloned()
        .collect::<Vec<_>>();
    if !folder.is_empty() && entries.is_empty() {
        return None;
    }
    let bytes = entries.iter().map(|tile| tile.bytes).sum::<i64>();
    let legend = legend_items(&entries.iter().collect::<Vec<_>>())
        .into_iter()
        .map(|(name, swatch, part)| {
            format!(
                "<span title=\"{} bytes\"><i class=\"swatch\" style=\"background:{swatch}\"></i>{}</span>",
                commas(part),
                esc(name)
            )
        })
        .collect::<String>();
    let count = entries.len();
    let nested = directories(entries, folder);
    let up = if folder.is_empty() {
        "<span class=\"button\" aria-disabled=\"true\">Up</span>".to_string()
    } else {
        format!(
            "<a class=\"button\" href=\"{}\" title=\"Up one folder\">Up</a>",
            url(parent(folder))
        )
    };
    let mut out = format!(
        "<main class=\"files\"><div class=\"toolbar\">{up}<span class=\"toolbar-label\">Address</span><span class=\"address field\">{}</span><a class=\"button\" href=\"{}\">Refresh</a></div><section class=\"chart well\" aria-label=\"Files by size on disk\">",
        if folder.is_empty() { "/".to_string() } else { esc(folder) },
        url(folder)
    );
    let mut widths = std::collections::BTreeSet::new();
    tiles(
        &mut out,
        &nested.iter().collect::<Vec<_>>(),
        Rect {
            x: 0.0,
            y: 0.0,
            width: 830.0,
            height: 467.0,
        },
        (830.0, 467.0),
        &mut widths,
    );
    out.push_str("</section><style>");
    // A folder's name bar opens exactly when its name fits, under one query.
    // One line is LINE game pixels; the tile's border adds one on each side.
    let (line, one, two) = (LINE * PIXEL, (LINE + 2) * PIXEL, (2 * LINE + 2) * PIXEL);
    for width in widths {
        out.push_str(&format!("@container (min-width:{width}px) and (min-height:{one}px){{.label-w{width}>span{{visibility:visible}}.label-w{width}~.area.headed{{top:{line}px}}}}@container (min-width:{width}px) and (min-height:{two}px){{.label-w{width}>.file-size{{visibility:visible}}}}"));
    }
    out.push_str(&format!(
        "</style><footer class=\"legend\" aria-label=\"File types\" tabindex=\"0\">{legend}</footer>"
    ));
    if let Some(file) = chosen {
        out.push_str(&properties(file, folder));
    }
    out.push_str("</main>");
    Some((
        out,
        format!(
            "{} files · {} bytes on disk",
            commas(count as i64),
            commas(bytes)
        ),
    ))
}
pub(super) fn page(root: &Path, path: &str) -> Option<Response> {
    let (folder, file) = if let Some(encoded) = path.strip_prefix("/file/") {
        let Some(file) = decode_folder(encoded).filter(|file| !file.is_empty()) else {
            return Some(http::not_found());
        };
        let file = file.trim_end_matches('/').to_string();
        let folder = file
            .rsplit_once('/')
            .map_or(String::new(), |(dir, _)| format!("{dir}/"));
        (folder, Some(file))
    } else if let Some(encoded) = path.strip_prefix("/view/") {
        let Some(folder) = decode_folder(encoded) else {
            return Some(http::not_found());
        };
        (folder, None)
    } else if path == "/" {
        (String::new(), None)
    } else {
        return None;
    };
    Some(
        match cache::view(path, || render(root, &folder, file.as_deref()).ok_or(())) {
            Ok((html, note)) => chrome::page(path, &html, &note),
            Err(()) => http::not_found(),
        },
    )
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn folder_routes_are_roundtrip_and_reject_escape_or_invalid_encoding() {
        let folder = "games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/";
        assert_eq!(decode_folder(&encode(folder)).as_deref(), Some(folder));
        for bad in ["x", "ff", "0g"] {
            assert!(decode_folder(bad).is_none());
        }
        for bad in ["../", "/etc/", "games/../", "games/THE BROKEN SEAL"] {
            assert!(decode_folder(&encode(bad)).is_none());
        }
    }
    #[test]
    fn disk_view_has_one_tile_per_real_file_in_both_trees() {
        let temp = tempfile::tempdir().unwrap();
        let dir = temp.path().join("games/test");
        std::fs::create_dir_all(&dir).unwrap();
        std::fs::write(dir.join("MAP.BIN"), [0u8; 123]).unwrap();
        let recon = temp.path().join("recon/test");
        std::fs::create_dir_all(&recon).unwrap();
        std::fs::write(recon.join("source-paths.json"), [0u8; 45]).unwrap();
        std::fs::write(recon.join("08006878.c"), [0u8; 12]).unwrap();
        let tiles = disk_tiles(temp.path());
        assert_eq!(tiles.len(), 3);
        assert_eq!(tiles[0].source.as_deref(), Some("games/test/MAP.BIN"));
        assert!(tiles.iter().all(|tile| tile.address.is_none()));
        let (html, note) = render(temp.path(), "", None).unwrap();
        assert!(html.contains("<span>games</span>"));
        assert!(html.contains(">Drafted C</span>"));
        assert_eq!(note, "3 files · 180 bytes on disk");
        assert!(!html.contains("<script"));
        assert!(!html.contains("<p>"));
        let (selected, _) = render(temp.path(), "games/test/", Some("games/test/MAP.BIN")).unwrap();
        assert!(selected.contains("MAP.BIN Properties"));
        assert!(selected.contains("method=\"post\""));
        assert!(render(temp.path(), "missing/", None).is_none());
        assert!(render(temp.path(), "", Some("games/test/NONE")).is_none());
    }
}
