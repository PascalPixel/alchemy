// Leaves from the byte-verified asset build.
//
// WHY this exists next to the tracked-manifest walk: `out/full/assets/manifest.json`
// is the schema-aware expansion of every package and series, so it is the
// authoritative dashboard source whenever a full build is present. It is also
// build output, which a fresh clone does not have, so the generic walk stays as
// the publication fallback. Absence returns `None`; a present-but-wrong
// manifest is an error, never a quietly degraded picture.

use crate::assets::{asset_tier_of, has_extension, manifest_ranges, overlay_streams};
use crate::json::Value;
use crate::jsnum::hex8;
use crate::model::Tile;
use crate::ordered::OrderedMap;
use crate::pipeline::{intervals_of, rom_size, to_spans};
use crate::spans::{intersect, normalize, span_bytes, subtract, Span};
use crate::tree::{read_json, SourceTree, ROM_BASE};

/// `path.dirname` for the POSIX paths a tracked index can hold.
fn dirname(path: &str) -> String {
    match path.rfind('/') {
        Some(0) => "/".to_string(),
        Some(index) => path[..index].to_string(),
        None => ".".to_string(),
    }
}

/// `path.join(base, value)` for POSIX segments, including the `.`/`..`
/// normalization Node performs. `join("", x)` is `x`.
fn join_path(base: &str, value: &str) -> String {
    if base.is_empty() {
        return normalize_path(value);
    }
    normalize_path(&format!("{base}/{value}"))
}

fn normalize_path(path: &str) -> String {
    let absolute = path.starts_with('/');
    let mut out: Vec<&str> = Vec::new();
    for segment in path.split('/') {
        match segment {
            "" | "." => {}
            ".." => {
                match out.last() {
                    Some(&last) if last != ".." => {
                        out.pop();
                    }
                    _ => {
                        if !absolute {
                            out.push("..");
                        }
                    }
                }
            }
            other => out.push(other),
        }
    }
    let joined = out.join("/");
    if absolute {
        format!("/{joined}")
    } else if joined.is_empty() {
        ".".to_string()
    } else {
        joined
    }
}

fn basename(path: &str) -> String {
    match path.rfind('/') {
        Some(index) => path[index + 1..].to_string(),
        None => path.to_string(),
    }
}

/// `/^(?:assets|asm|semantic|src)\//`
fn is_rooted_reference(value: &str) -> bool {
    ["assets/", "asm/", "semantic/", "src/"].iter().any(|prefix| value.starts_with(prefix))
}

fn families_visit(
    tree: &SourceTree,
    value: &Value,
    family: &str,
    base: &str,
    families: &mut OrderedMap<String, String>,
    visited: &mut Vec<String>,
) {
    match value {
        Value::Arr(items) => {
            for child in items {
                families_visit(tree, child, family, base, families, visited);
            }
        }
        Value::Obj(members) => {
            for (_, child) in members {
                families_visit(tree, child, family, base, families, visited);
            }
        }
        Value::Str(text) => {
            // PORT NOTE: this walk spells the extension bound `{2,5}` while
            // `assetMaturityTiles` spells it `{2,4}`. The drift is upstream and
            // observable, so both spellings are preserved verbatim.
            if !has_extension(text, 5) {
                return;
            }
            let path =
                if is_rooted_reference(text) { text.clone() } else { join_path(base, text) };
            if !families.contains_key(&path) {
                families.insert(path.clone(), family.to_string());
            }
            if !path.ends_with(".json") || visited.contains(&path) {
                return;
            }
            visited.push(path.clone());
            let Some(text) = tree.read(&path) else { return };
            // A malformed or intentionally non-JSON reference contributes no
            // lineage; the TypeScript swallows the parse error identically.
            if let Ok(document) = crate::json::parse(&text) {
                families_visit(tree, &document, family, &dirname(&path), families, visited);
            }
        }
        _ => {}
    }
}

/// `assetFamiliesBySource(tree)`.
///
/// PORT NOTE: a JS `Map` and `Set` both iterate in insertion order, and the
/// first writer of a path wins (`if (!families.has(path))`), so the order of
/// the three sections is load-bearing. An `OrderedMap` and a `Vec` reproduce
/// it; a `HashMap`/`HashSet` would not, and a `HashSet` would additionally hide
/// the revisit guard's meaning.
pub fn asset_families_by_source(tree: &SourceTree) -> Result<OrderedMap<String, String>, String> {
    let manifest = read_json(tree, "assets/manifest.json")?;
    let mut families: OrderedMap<String, String> = OrderedMap::new();
    let mut visited: Vec<String> = Vec::new();
    for section in ["series", "regions", "closure_packages"] {
        let Some(Value::Arr(entries)) = manifest.get(section) else { continue };
        for entry in entries {
            let Value::Obj(_) = entry else { continue };
            let family =
                entry.get("kind").and_then(|value| value.as_str()).unwrap_or("asset").to_string();
            families_visit(tree, entry, &family, "", &mut families, &mut visited);
        }
    }
    Ok(families)
}

struct BuiltAssetRegion {
    address: i64,
    size: i64,
    kind: String,
    sources: Vec<String>,
    /// The raw numbers, kept so the safe-integer refusal can see a fractional
    /// or out-of-range value instead of a silently truncated one.
    raw_address: f64,
    raw_size: f64,
}

fn built_region(node: &Value) -> BuiltAssetRegion {
    let number = |key: &str| node.get(key).and_then(|value| value.as_f64()).unwrap_or(f64::NAN);
    let raw_address = number("address");
    let raw_size = number("size");
    BuiltAssetRegion {
        address: raw_address as i64,
        size: raw_size as i64,
        kind: node.get("kind").and_then(|value| value.as_str()).unwrap_or("").to_string(),
        sources: match node.get("sources") {
            Some(Value::Arr(items)) => items
                .iter()
                .filter_map(|item| item.as_str().map(str::to_string))
                .collect(),
            _ => Vec::new(),
        },
        raw_address,
        raw_size,
    }
}

/// `verifiedAssetTiles(tree, target)`. `Ok(None)` is "no full build present".
pub fn verified_asset_tiles(
    tree: &SourceTree,
    target: &str,
) -> Result<Option<Vec<Tile>>, String> {
    let Some(text) = tree.read("out/full/assets/manifest.json") else { return Ok(None) };
    let document = crate::json::parse(&text)?;
    let rom_bytes = rom_size(target)?;
    let format = document.get("format").and_then(|value| value.as_f64());
    let manifest_rom_size = document.get("rom_size").and_then(|value| value.as_f64());
    let verification =
        document.get("verification").and_then(|value| value.as_str()).unwrap_or("").to_string();
    let regions = match document.get("regions") {
        Some(Value::Arr(items)) => Some(items.clone()),
        _ => None,
    };
    let Some(regions) = regions else {
        return Err("verified asset manifest has an unsupported or incomplete schema".to_string());
    };
    if format != Some(1.0)
        || manifest_rom_size != Some(rom_bytes as f64)
        || !["rom", "source_only"].contains(&verification.as_str())
    {
        return Err("verified asset manifest has an unsupported or incomplete schema".to_string());
    }

    let inventory = read_json(tree, &format!("metrics/{target}-executable.json"))?;
    let main_node = inventory.get("main").cloned().unwrap_or(Value::Null);
    let main_executable =
        to_spans(crate::intervals::union_intervals(&intervals_of(&main_node))?);
    let streams = overlay_streams(tree)?;
    let mut code_input = main_executable;
    for stream in streams.values() {
        code_input.push(Span::new(stream.start, stream.start + stream.rom_bytes));
    }
    let code_spans = normalize(&code_input);
    let data_spans = subtract(&[Span::new(ROM_BASE, ROM_BASE + rom_bytes)], &code_spans);

    let mut ordered: Vec<BuiltAssetRegion> = regions.iter().map(built_region).collect();
    // PORT NOTE: `a - b || c - d` in JS falls through on 0 AND on NaN. A
    // fractional or missing address makes the first term NaN, which JS treats
    // as 0 and Rust's `partial_cmp` cannot express; the explicit form below
    // reproduces the fall-through, and the safe-integer refusal below rejects
    // such a region anyway.
    ordered.sort_by(|left, right| {
        let primary = left.raw_address - right.raw_address;
        if primary < 0.0 {
            return std::cmp::Ordering::Less;
        }
        if primary > 0.0 {
            return std::cmp::Ordering::Greater;
        }
        let secondary = left.raw_size - right.raw_size;
        if secondary < 0.0 {
            std::cmp::Ordering::Less
        } else if secondary > 0.0 {
            std::cmp::Ordering::Greater
        } else {
            std::cmp::Ordering::Equal
        }
    });

    let lineage = manifest_ranges(tree, rom_bytes)?;
    let source_families = asset_families_by_source(tree)?;
    let asset_manifest = read_json(tree, "assets/manifest.json")?;
    let mut top_families: Vec<String> = Vec::new();
    for section in ["series", "regions"] {
        let Some(Value::Arr(entries)) = asset_manifest.get(section) else { continue };
        for entry in entries {
            if let Some(kind) = entry.get("kind").and_then(|value| value.as_str()) {
                if !top_families.iter().any(|seen| seen == kind) {
                    top_families.push(kind.to_string());
                }
            }
        }
    }

    let family_of = |region: &BuiltAssetRegion| -> String {
        let direct_series = format!("{}-series", region.kind);
        if top_families.contains(&direct_series) {
            return direct_series;
        }
        for source in &region.sources {
            if let Some(family) = source_families.get(source) {
                return family.clone();
            }
        }
        let end = region.address + region.size;
        let mut enclosing: Vec<&crate::assets::RomRange> = lineage
            .iter()
            .filter(|range| range.start <= region.address && range.end >= end)
            .collect();
        // JS `Array#sort` is stable, and so is `sort_by`; ties keep manifest
        // order, which decides which family a nested range reports.
        enclosing.sort_by_key(|range| range.end - range.start);
        enclosing.first().map(|range| range.family.clone()).unwrap_or_else(|| region.kind.clone())
    };

    let mut previous_end = ROM_BASE;
    let mut tiles: Vec<Tile> = Vec::new();
    let mut covered: Vec<Span> = Vec::new();
    for region in &ordered {
        if !crate::jsnum::is_safe_integer(region.raw_address)
            || !crate::jsnum::is_safe_integer(region.raw_size)
            || region.size <= 0
            || region.address < ROM_BASE
            || region.address + region.size > ROM_BASE + rom_bytes
        {
            return Err("verified asset manifest contains an invalid region".to_string());
        }
        if region.address < previous_end {
            return Err("verified asset manifest regions overlap".to_string());
        }
        previous_end = region.address + region.size;
        let span = Span::new(region.address, region.address + region.size);
        let data = intersect(&[span], &data_spans);
        let bytes = span_bytes(&data);
        if bytes <= 0 {
            continue;
        }
        if bytes != region.size {
            return Err(format!(
                "asset region 0x{} partially overlaps code",
                hex8(region.address)
            ));
        }
        covered.extend_from_slice(&data);
        let tier = asset_tier_of(&region.sources, &region.kind);
        let family = family_of(region);
        let identity = match region.sources.first() {
            Some(source) => basename(source),
            None => region.kind.clone(),
        };
        let mut tile = Tile {
            label: format!("{identity} \u{b7} {} \u{b7} 0x{}", region.kind, hex8(region.address)),
            bytes,
            ..Tile::default()
        };
        tile.set_category("asset_data", bytes);
        tile.set_category(tier, bytes);
        tile.group = Some(family.clone());
        tile.subgroup =
            if family == region.kind { None } else { Some(region.kind.clone()) };
        tile.address = Some(region.address);
        tiles.push(tile);
    }
    let expected = span_bytes(&data_spans);
    let actual = span_bytes(&normalize(&covered));
    if actual != expected {
        return Err(format!(
            "verified asset leaves cover {actual} of {expected} ROM-data bytes"
        ));
    }
    Ok(Some(tiles))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn paths_join_and_normalize_the_way_node_does() {
        assert_eq!(join_path("", "a/b.json"), "a/b.json");
        assert_eq!(join_path("assets/x", "../y/z.png"), "assets/y/z.png");
        assert_eq!(join_path("assets", "./z.png"), "assets/z.png");
        assert_eq!(dirname("assets/x/y.json"), "assets/x");
        assert_eq!(dirname("y.json"), ".");
        assert_eq!(basename("assets/x/y.json"), "y.json");
    }

    #[test]
    fn a_rooted_reference_bypasses_the_relative_join() {
        assert!(is_rooted_reference("assets/a.png"));
        assert!(is_rooted_reference("semantic/a.c"));
        assert!(!is_rooted_reference("data/a.png"));
    }

    #[test]
    fn the_family_walk_uses_the_five_character_extension_bound() {
        // Only a five-character extension separates the two spellings.
        // `.json` (four) and `.bin` (three) are seen by both walks; `.gbapal`
        // (six) and `.tilemap` (seven) by neither.
        assert!(has_extension("a.midix", 5), "five characters: the wider bound only");
        assert!(!has_extension("a.midix", 4), "the maturity walk cannot see it");
        assert!(has_extension("a.json", 4));
        assert!(!has_extension("a.gbapal", 5));
    }
}
