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
fn color(category: Category) -> &'static str {
    match category {
        Category::Unknown => "#d9d9d4",
        Category::DraftC => "#96c8c9",
        Category::ProvenAsm => "#6cafb2",
        Category::ProvenC => "#326b7d",
        _ => "#bda995",
    }
}
fn tiles(
    out: &mut String,
    entries: &[&Tile],
    frame: Rect,
    folder: &str,
    verification: &str,
    widths: &mut std::collections::BTreeSet<usize>,
) {
    for placed in treemap(entries, |tile| tile.bytes, frame) {
        let tile = entries[placed.index];
        let rect = placed.rect;
        let nested = !tile.children.is_empty();
        let source = tile.source.as_deref().unwrap_or("");
        let directory = source.ends_with('/');
        let name = if source.is_empty() {
            &tile.label
        } else {
            source_name(source)
        };
        let note = if tile.categories[Category::AssetData as usize] == tile.bytes && !directory {
            match verification {
                "rom" => " · Last asset build: ROM bytes matched; appearance not verified",
                "source_only" => " · Last asset build: not compared with ROM",
                _ => " · Asset verification unavailable",
            }
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
            rect.x / 830.0 * 100.0,
            rect.y / 467.0 * 100.0,
            rect.width / 830.0 * 100.0,
            rect.height / 467.0 * 100.0,
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
        } else {
            tile.address
                .map(|a| format!("/inspect/{a:x}/{}", encode(folder)))
        };
        if let Some(href) = href {
            let minimum = (label_width(name) + 8.0).ceil().max(72.0) as usize;
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
        }
        out.push_str("</div>");
        if nested {
            let inset = 2.0_f64.min(rect.width / 4.0).min(rect.height / 4.0);
            let heading = if directory {
                20.0_f64.min((rect.height - inset * 2.0).max(0.0))
            } else {
                0.0
            };
            tiles(
                out,
                &tile.children.iter().collect::<Vec<_>>(),
                Rect {
                    x: rect.x + inset,
                    y: rect.y + inset + heading,
                    width: rect.width - inset * 2.0,
                    height: (rect.height - inset * 2.0 - heading).max(0.0),
                },
                folder,
                verification,
                widths,
            );
        }
    }
}
fn reveal_form(source: &str) -> String {
    let path: String = source.bytes().map(|b| format!("%{b:02X}")).collect();
    format!("<form method=\"post\" action=\"/reveal/{path}\" target=\"reveal-result\"><button type=\"submit\">Show in Finder</button></form>")
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
    let nested = directories(entries, folder);
    let verification = map.document["asset_verification"].as_str().unwrap_or("");
    let heading = if folder.is_empty() {
        "Alchemy".into()
    } else {
        format!("TBS EN · {}", source_name(folder))
    };
    let mut out=format!("<main><header><a class=\"back\" href=\"{}\" aria-label=\"Back\">‹</a><span>{}</span><a class=\"refresh\" href=\"{}\">Refresh</a></header><section class=\"chart\" aria-label=\"ROM contents\">",url(parent(folder)),esc(&heading),url(folder));
    let mut widths = std::collections::BTreeSet::new();
    tiles(
        &mut out,
        &nested.iter().collect::<Vec<_>>(),
        Rect {
            x: 4.0,
            y: 4.0,
            width: 822.0,
            height: 459.0,
        },
        folder,
        verification,
        &mut widths,
    );
    out.push_str("</section>");
    out.push_str("<style>");
    for width in widths {
        out.push_str(&format!("@container (min-width:{width}px) and (min-height:24px){{.label-w{width} span{{visibility:visible}}}}"));
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
    out.push_str("<footer class=\"legend\"><span style=\"--swatch:#d9d9d4\">Unknown</span><span style=\"--swatch:#96c8c9\">Draft C</span><span style=\"--swatch:#6cafb2\">Assembly</span><span style=\"--swatch:#326b7d\">C</span><span style=\"--swatch:#bda995\">Assets</span></footer>");
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
