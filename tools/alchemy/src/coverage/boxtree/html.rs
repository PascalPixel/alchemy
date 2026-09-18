use super::*;

fn encode(value: &str) -> String {
    value.bytes().map(|byte| format!("{byte:02x}")).collect()
}
pub fn decode_folder(value: &str) -> Option<String> {
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
fn parent(folder: &str) -> &str {
    folder
        .trim_end_matches('/')
        .rsplit_once('/')
        .map_or("", |(parent, _)| &folder[..parent.len() + 1])
}
fn legend(entries: &[Tile]) -> String {
    legend_items(&entries.iter().collect::<Vec<_>>())
        .into_iter()
        .map(|(name, swatch, _)| format!("<span style=\"--swatch:{swatch}\">{}</span>", esc(name)))
        .collect::<Vec<_>>()
        .join("")
}
/// A folder's name bar is 16px tall in the stylesheet whatever the window
/// size; the layout estimates it in chart units only to shape its children.
const HEADING_UNITS: f64 = 12.0;
/// Draw `entries` into a box `area` chart units wide and tall. Each tile holds
/// its children in an inner box below its name bar, so bars keep a fixed pixel
/// height while tiles scale with the window.
fn tiles(
    out: &mut String,
    entries: &[&Tile],
    frame: Rect,
    area: (f64, f64),
    folder: &str,
    verification: &str,
    widths: &mut std::collections::BTreeSet<usize>,
) {
    for placed in treemap(entries, |tile| tile.bytes, frame) {
        let tile = entries[placed.index];
        let rect = placed.rect;
        let nested = !tile.children.is_empty();
        let source = tile.source.as_deref().unwrap_or("");
        // A leaf holding its folder's unclassified bytes is named by its label.
        let directory = source.ends_with('/') && nested;
        let name = if source.is_empty()
            || (source.ends_with('/') && !nested)
            || (!nested && tile.address.is_none())
        {
            &tile.label
        } else {
            source_name(source)
        };
        let file = tile
            .group
            .as_deref()
            .is_some_and(|g| g.starts_with("file:"));
        let note = if file {
            " on disk"
        } else if tile.categories[Category::AssetData as usize] == tile.bytes && !directory {
            asset_note(tile, Some(verification))
        } else {
            ""
        };
        let title = format!(
            "{name}: {} bytes{note}{}",
            commas(tile.bytes),
            tile.address
                .map_or(String::new(), |a| format!(" · 0x{a:08x}"))
        );
        out.push_str(&format!(
            "<div class=\"tile {}\" style=\"left:{}%;top:{}%;width:{}%;height:{}%\" title=\"{}\">",
            if nested { "container" } else { "leaf" },
            rect.x / area.0 * 100.0,
            rect.y / area.1 * 100.0,
            rect.width / area.0 * 100.0,
            rect.height / area.1 * 100.0,
            esc(&title)
        ));
        if !nested {
            let mut bottom = 0.0;
            for (category, _) in DISPLAY_CATEGORIES {
                let bytes = display_bytes(&tile.categories, category);
                if bytes <= 0 {
                    continue;
                }
                let height = bytes as f64 / tile.bytes.max(1) as f64 * 100.0;
                let fill = if category == Category::AssetData {
                    content_style(tile).1
                } else {
                    color(category)
                };
                out.push_str(&format!("<span class=\"tile-color\" style=\"bottom:{bottom}%;height:{height}%;background:{fill}\"></span>"));
                bottom += height;
            }
        }
        let href = if directory {
            Some(url(source))
        } else if file {
            Some(format!("/file/{}", encode(&format!("{source}/"))))
        } else {
            tile.address
                .map(|a| format!("/inspect/{a:x}/{}", encode(folder)))
        };
        let minimum = (label_width(name) + 8.0).ceil().max(72.0) as usize;
        if let Some(href) = href {
            widths.insert(minimum);
            out.push_str(&format!(
                "<a class=\"{} label-w{minimum}\" href=\"{href}\" aria-label=\"{}\"><span>{}</span></a>",
                if directory {
                    "folder-label"
                } else {
                    "leaf-label"
                },
                esc(&title),
                esc(name)
            ));
        } else if !nested {
            // Aggregates with no single honest address still need a visible
            // name; they are not links because there is nowhere exact to open.
            widths.insert(minimum);
            out.push_str(&format!(
                "<span class=\"leaf-label label-w{minimum}\"><span>{}</span></span>",
                esc(name)
            ));
        }
        if nested {
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
                folder,
                verification,
                widths,
            );
            out.push_str("</div>");
        }
        out.push_str("</div>");
    }
}
fn reveal_form(source: &str) -> String {
    let path: String = source.bytes().map(|b| format!("%{b:02X}")).collect();
    format!("<form method=\"post\" action=\"/reveal/{path}\" target=\"reveal-result\"><button type=\"submit\">Show in Finder</button></form>")
}

#[test]
fn disk_view_has_one_tile_per_real_file() {
    let temp = tempfile::tempdir().unwrap();
    let dir = temp.path().join("games/test");
    std::fs::create_dir_all(&dir).unwrap();
    std::fs::write(dir.join("MAP.BIN"), [0u8; 123]).unwrap();
    let tiles = disk_tiles(temp.path());
    assert_eq!(tiles.len(), 1);
    assert_eq!(tiles[0].bytes, 123);
    assert_eq!(tiles[0].source.as_deref(), Some("games/test/MAP.BIN"));
    assert_eq!(tiles[0].address, None);
}

fn disk_tiles(repository: &std::path::Path) -> Vec<Tile> {
    walkdir::WalkDir::new(repository.join("games"))
        .follow_links(false)
        .into_iter()
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

pub fn file_page(
    map: &CoverageMap,
    folder: &str,
    selected: Option<&str>,
    shared: bool,
) -> Option<String> {
    let files = disk_tiles(&root());
    let chosen = match selected {
        Some(path) => Some(files.iter().find(|t| t.source.as_deref() == Some(path))?),
        None => None,
    };
    let mut document = map.document.clone();
    document["view"] = serde_json::json!("files");
    let disk = CoverageMap {
        document,
        rom_areas: vec![crate::coverage::model::area(
            "files",
            "Files",
            files.clone(),
        )],
        executable_areas: vec![],
    };
    let mut html = page(&disk, folder, None, shared)?;
    if let Some(file) = chosen {
        let path = file.source.as_deref()?;
        let mut details=format!("<aside><a href=\"{}\">Close</a><h2>{}</h2><p>{} bytes on disk. ROM regions below are separate build outputs, not additional files.</p>{}",url(folder),esc(path),commas(file.bytes),reveal_form(path));
        let mut regions = std::collections::BTreeMap::new();
        let target = if path.starts_with("games/THE LOST AGE/") {
            "tla-en"
        } else {
            "tbs-en"
        };
        let index =
            crate::coverage::audit::index::current(&crate::coverage::tree::work_tree(), target);
        for tile in leaves(&tree_tiles(map))
            .into_iter()
            .filter(|t| t.source.as_deref() == Some(path))
        {
            if let Some(address) = tile.address {
                let kind = index
                    .as_ref()
                    .and_then(|i| i["regions"].as_array())
                    .and_then(|rs| rs.iter().find(|r| r["start"].as_i64() == Some(address)))
                    .and_then(|r| r["kind"].as_str());
                let style = kind
                    .map(|k| {
                        content_style(&Tile {
                            group: Some(k.into()),
                            ..Tile::default()
                        })
                    })
                    .unwrap_or_else(|| content_style(tile));
                regions.insert((address, tile.bytes), style.0);
            }
        }
        for ((address, bytes), kind) in &regions {
            details.push_str(&format!(
                "<p>0x{address:08x} · {} stored ROM bytes · {}</p>",
                commas(*bytes),
                esc(kind)
            ));
        }
        if regions.is_empty() {
            details.push_str("<p>No direct ROM-region attribution recorded for this file.</p>");
        }
        details.push_str("</aside><iframe name=\"reveal-result\" class=\"action-result\" title=\"Finder action result\"></iframe>");
        html = html.replace("</main>", &format!("{details}</main>"));
    }
    Some(html)
}

pub fn rom_page(target: &str) -> Option<String> {
    if !matches!(target, "tbs-en" | "tla-en") {
        return None;
    }
    let tree = crate::coverage::tree::work_tree();
    let mut html=format!("<main class=\"rom-view\"><header><span>Alchemy</span><a class=\"refresh\" href=\"/rom/{target}\">Refresh</a></header><section class=\"rom-content\"><nav aria-label=\"Game\"><a href=\"/rom/tbs-en\">The Broken Seal</a> · <a href=\"/rom/tla-en\">The Lost Age</a></nav>");
    let Some(index) = crate::coverage::audit::index::current(&tree, target) else {
        html.push_str("<p>The ROM identification index is absent or stale. Rebuild it with alchemy coverage audit --target TARGET --data.</p></section></main>");
        return Some(html);
    };
    let size = index["rom_bytes"].as_i64()?;
    let rows = index["regions"].as_array()?;
    let unresolved = rows
        .iter()
        .filter(|r| {
            let kind = r["kind"].as_str().unwrap_or("unresolved-data");
            let unknown_style = !matches!(kind, "executable" | "encoded-overlay")
                && content_style(&Tile {
                    group: Some(kind.into()),
                    ..Tile::default()
                })
                .0 == UNIDENTIFIED;
            unknown_style
                || matches!(
                    r["kind"].as_str(),
                    Some(
                        "unresolved-data"
                            | "compressed-resource"
                            | "golden-sun-general-lz"
                            | "golden-sun-kind2-lz"
                    )
                )
        })
        .map(|r| r["bytes"].as_i64().unwrap_or(0))
        .sum::<i64>();
    html.push_str(&format!("<h2>{} · English ROM</h2><p>Identified format: {:.2}% · {} / {} cartridge bytes</p><p>Address order, left to right then down. Each row is 256 KiB. Colours show content, not reconstruction. Identification does not count toward ☀️ / ⚓️ DONE.</p><div class=\"rom-address-map\" role=\"img\" aria-label=\"ROM address map\">",if target=="tbs-en" {"The Broken Seal"}else{"The Lost Age"},100.0*(size-unresolved) as f64/size as f64,commas(size-unresolved),commas(size)));
    let row_bytes = 256 * 1024i64;
    let row_count = (size + row_bytes - 1) / row_bytes;
    let mut totals = std::collections::BTreeMap::<(&str, &str), i64>::new();
    for row in rows {
        let start = row["start"].as_i64()?;
        let end = row["end"].as_i64()?;
        let kind = row["kind"].as_str()?;
        let (label, color) = match kind {
            "executable" => ("Code", "#f0c57d"),
            "encoded-overlay" => ("Code overlays", "#78afb7"),
            "unresolved-data" => ("Not yet identified", UNKNOWN),
            _ => content_style(&Tile {
                group: Some(kind.into()),
                source: row["sources"][0].as_str().map(String::from),
                ..Tile::default()
            }),
        };
        *totals.entry((label, color)).or_default() += end - start;
        let mut cursor = start - 0x08000000;
        while cursor < end - 0x08000000 {
            let line = cursor / row_bytes;
            let stop = ((line + 1) * row_bytes).min(end - 0x08000000);
            let title = format!(
                "0x{start:08x}–0x{end:08x} · {} ROM bytes · {label} · {}",
                commas(end - start),
                row["evidence"].as_str().unwrap_or("")
            );
            html.push_str(&format!("<span title=\"{}\" style=\"left:{}%;top:{}%;width:{}%;height:{}%;background:{color}\"></span>",esc(&title),(cursor%row_bytes) as f64/row_bytes as f64*100.0,line as f64/row_count as f64*100.0,(stop-cursor) as f64/row_bytes as f64*100.0,100.0/row_count as f64));
            cursor = stop;
        }
    }
    html.push_str("</div><p>0x08000000 → cartridge end. Hover a region for its address, size and evidence.</p><details><summary>Content totals</summary><ul>");
    for ((label, _), bytes) in &totals {
        html.push_str(&format!(
            "<li>{}: {} bytes ({:.2}%)</li>",
            esc(label),
            commas(*bytes),
            *bytes as f64 / size as f64 * 100.0
        ));
    }
    html.push_str("</ul></details></section><footer class=\"legend\" aria-label=\"ROM content types\" tabindex=\"0\">");
    for ((label, color), bytes) in totals {
        html.push_str(&format!(
            "<span style=\"--swatch:{color}\">{} {:.1}%</span>",
            esc(label),
            bytes as f64 / size as f64 * 100.0
        ));
    }
    html.push_str("</footer></main>");
    Some(html)
}
pub fn page(
    map: &CoverageMap,
    folder: &str,
    selected: Option<i64>,
    shared: bool,
) -> Option<String> {
    let all = tree_tiles(map);
    let entries = leaves(&all)
        .into_iter()
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
    let legend = legend(&entries);
    let nested = directories(entries, folder);
    let verification = map.document["asset_verification"].as_str().unwrap_or("");
    let heading: String = if folder.is_empty() {
        if map.document["view"] == "files" {
            "Files · disk size".into()
        } else {
            "Alchemy".into()
        }
    } else {
        source_name(folder).into()
    };
    let back = if folder.is_empty() {
        String::new()
    } else {
        format!(
            "<a class=\"back\" href=\"{}\" aria-label=\"Back\">‹</a>",
            url(parent(folder))
        )
    };
    let files = map.document["view"] == "files";
    let mut out=format!("<main><header>{back}<span>{}</span><a class=\"refresh\" href=\"{}\">Refresh</a></header><section class=\"chart\" aria-label=\"{}\">",esc(&heading),url(folder),if files {"Repository files · area is size on disk"} else {"ROM contents"});
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
        folder,
        verification,
        &mut widths,
    );
    out.push_str("</section>");
    out.push_str("<style>");
    // A folder's name bar opens exactly when its name fits, under one query.
    for width in widths {
        out.push_str(&format!("@container (min-width:{width}px) and (min-height:24px){{.label-w{width} span{{visibility:visible}}.label-w{width}~.area.headed{{top:16px}}}}"));
    }
    out.push_str("</style>");
    if let Some(items) = map.document["shared_map_assets"][folder]
        .as_array()
        .filter(|a| !a.is_empty())
    {
        out.push_str(&format!(
            "<a class=\"shared-link\" href=\"/shared/{}\">Shared files ({})</a>",
            encode(folder),
            items.len()
        ));
    }
    if shared {
        out.push_str(&format!(
            "<aside><a href=\"{}\">Close</a><h2>Shared files</h2>",
            url(folder)
        ));
        for source in map.document["shared_map_assets"][folder]
            .as_array()
            .into_iter()
            .flatten()
            .filter_map(|v| v.as_str())
        {
            out.push_str(&format!("<p>{}</p>{}", esc(source), reveal_form(source)));
        }
        out.push_str("</aside>");
    } else if let Some(address) = selected {
        let tile = leaves(&all)
            .into_iter()
            .find(|tile| tile.address == Some(address))?;
        out.push_str(&format!(
            "<aside><a href=\"{}\">Close</a><h2>{}</h2><p>{} bytes · 0x{address:08x}</p><p>{}</p>",
            url(folder),
            esc(&tile.label),
            commas(tile.bytes),
            esc(tile.source.as_deref().unwrap_or(""))
        ));
        if tile.categories[Category::AssetData as usize] == tile.bytes {
            out.push_str("<p>Stored bytes and scene appearance are separate checks.</p>");
        }
        if let Some(source) = &tile.source {
            out.push_str(&reveal_form(source));
        }
        out.push_str("</aside>");
    }
    out.push_str(&format!(
        "<footer class=\"legend\" aria-label=\"File types\" tabindex=\"0\">{legend}</footer>"
    ));
    if shared || selected.is_some() {
        out.push_str("<iframe name=\"reveal-result\" class=\"action-result\" title=\"Finder action result\"></iframe>");
    }
    out.push_str("</main>");
    Some(out)
}

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
fn rust_viewer_links_folders_details_and_shared_sources_without_scripts() {
    use crate::coverage::model::Area;
    use serde_json::json;
    let folder = "games/THE BROKEN SEAL/SRC/GRAPHICS/CHARACTER/";
    let file = format!("{folder}CHAR_IWAN.PNG");
    let tile = Tile {
        label: "CHAR_IWAN.PNG".into(),
        source: Some(file.clone()),
        address: Some(0x8001000),
        bytes: 1024,
        categories: [0, 0, 0, 0, 0, 1024],
        ..Tile::default()
    };
    let map = CoverageMap {
        document: json!({"asset_verification":"rom","shared_map_assets":{folder:["games/THE BROKEN SEAL/SRC/GRAPHICS/COMMON/PALETTE.JSON"]}}),
        rom_areas: vec![Area {
            tiles: vec![tile],
            ..Area::default()
        }],
        executable_areas: vec![],
    };
    let root = page(&map, "", None, false).unwrap();
    assert!(root.contains("href=\"/view/"));
    assert!(root.contains("/inspect/8001000/"));
    let selected = page(&map, folder, Some(0x8001000), false).unwrap();
    assert!(selected.contains("method=\"post\""));
    assert!(selected.contains("Last asset build: ROM bytes matched; appearance not verified"));
    assert!(selected.contains("Shared files (1)"));
    let shared = page(&map, folder, None, true).unwrap();
    assert!(shared.contains("COMMON/PALETTE.JSON"));
    assert!(!selected.contains("<script"));
    assert!(!selected.contains("javascript:"));
    assert!(page(&map, "missing/", None, false).is_none());
    assert!(page(&map, folder, Some(42), false).is_none());
}
