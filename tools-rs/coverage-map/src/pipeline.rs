// Assembling the map from tracked evidence, and refusing to publish a lie.
//
// WHY: this is the only place where the executable audit, the derived exact and
// semantic ownership, the retained-assembly contracts and the asset manifest
// meet. Every one of them can be individually right and jointly inconsistent,
// so the two conservation checks near the end -- ROM areas summing to the ROM
// size, executable areas summing to the audited union -- are the point of the
// function, not decoration. A map that cannot account for every byte is not
// published at all.

use crate::assets::{manifest_asset_tiles, overlay_streams, retained_main_spans};
use crate::intervals::{union_intervals, Interval};
use crate::json::Value;
use crate::model::{area, Area, Tile};
use crate::ordered::OrderedMap;
use crate::ownership::{
    exact_main_spans, exact_overlay_owners, main_boundaries, semantic_spans, OverlayOwner,
    SemanticCoverage,
};
use crate::spans::{intersect, normalize, span_bytes, subtract, Span};
use crate::tiles::{group_tiles, main_bands, main_owner_tiles, overlay_owner_tiles};
use crate::tree::{read_json, SourceTree, ROM_BASE};

pub fn rom_size(target: &str) -> Result<i64, String> {
    match target {
        "gs1-en" => Ok(0x0080_0000),
        "gs2-en" => Ok(0x0100_0000),
        other => Err(format!(
            "unsupported decomp target {:?}; expected gs1-en or gs2-en",
            other
        )),
    }
}

pub struct BuildOptions<'a> {
    pub target: String,
    pub exact: &'a SourceTree,
    pub semantic: Option<&'a SourceTree>,
    pub validate_tracked_progress: bool,
    pub prefer_verified_assets: bool,
}

/// The emitted document, kept as an insertion-ordered JSON value so the writer
/// never has to guess a key order.
pub struct CoverageMap {
    pub document: Value,
    pub rom_areas: Vec<Area>,
    pub executable_areas: Vec<Area>,
}

pub fn intervals_of(node: &Value) -> Vec<Interval> {
    let mut out = Vec::new();
    for entry in node.get("intervals").and_then(|value| value.as_array()).unwrap_or(&[]) {
        let (Some(start), Some(end)) =
            (entry.get("start").and_then(|v| v.as_f64()), entry.get("end").and_then(|v| v.as_f64()))
        else {
            continue;
        };
        out.push(Interval {
            start,
            end,
            kind: entry.get("kind").and_then(|v| v.as_str()).unwrap_or("").to_string(),
            evidence: entry.get("evidence").and_then(|v| v.as_str()).unwrap_or("").to_string(),
        });
    }
    out
}

pub fn to_spans(spans: Vec<crate::intervals::Span>) -> Vec<Span> {
    spans.into_iter().map(|span| Span::new(span.start as i64, span.end as i64)).collect()
}

fn area_value(item: &Area) -> Value {
    let mut tiles = Vec::new();
    for tile in &item.tiles {
        let mut fields: Vec<(String, Value)> = vec![
            ("label".into(), Value::Str(tile.label.clone())),
            ("bytes".into(), Value::Num(tile.bytes as f64)),
            (
                "categories".into(),
                Value::Obj(
                    tile.categories
                        .iter()
                        .map(|(key, bytes)| (key.clone(), Value::Num(*bytes as f64)))
                        .collect(),
                ),
            ),
        ];
        // PORT NOTE: the TypeScript builds the tile literal with the optional
        // members present only when set, and `canonicalJson` drops `undefined`
        // members entirely. Omitting them here reproduces that exactly.
        if let Some(group) = &tile.group {
            fields.push(("group".into(), Value::Str(group.clone())));
        }
        if let Some(subgroup) = &tile.subgroup {
            fields.push(("subgroup".into(), Value::Str(subgroup.clone())));
        }
        if let Some(address) = tile.address {
            fields.push(("address".into(), Value::Num(address as f64)));
        }
        tiles.push(Value::Obj(fields));
    }
    Value::Obj(vec![
        ("id".into(), Value::Str(item.id.clone())),
        ("label".into(), Value::Str(item.label.clone())),
        ("bytes".into(), Value::Num(item.bytes as f64)),
        (
            "categories".into(),
            Value::Obj(
                item.categories
                    .iter()
                    .map(|(key, bytes)| (key.clone(), Value::Num(*bytes as f64)))
                    .collect(),
            ),
        ),
        ("tiles".into(), Value::Arr(tiles)),
    ])
}

fn category_entry(bytes: i64, executable_bytes: i64) -> Result<Value, String> {
    let percent =
        crate::intervals::round_half_up_percent(bytes as f64, executable_bytes as f64)?;
    Ok(Value::Obj(vec![
        ("bytes".into(), Value::Num(bytes as f64)),
        (
            "percent_of_executable".into(),
            Value::Num(percent),
        ),
    ]))
}

pub fn build_coverage_map(options: &BuildOptions) -> Result<CoverageMap, String> {
    let rom_bytes = rom_size(&options.target)?;
    let inventory = read_json(options.exact, &format!("metrics/{}-executable.json", options.target))?;
    let tracked = if options.validate_tracked_progress {
        Some(read_json(options.exact, &format!("metrics/{}-progress.json", options.target))?)
    } else {
        None
    };
    if inventory.get("audit").and_then(|v| v.as_str()) != Some("complete") {
        return Err(format!(
            "{} executable audit is incomplete; coverage map withheld",
            options.target
        ));
    }

    let main_node = inventory.get("main").cloned().unwrap_or(Value::Null);
    let main_executable = to_spans(union_intervals(&intervals_of(&main_node))?);
    let overlay_nodes: Vec<Value> =
        inventory.get("overlays").and_then(|v| v.as_array()).unwrap_or(&[]).to_vec();

    let mut overlay_executable: OrderedMap<String, Vec<Span>> = OrderedMap::new();
    let mut overlay_retained: OrderedMap<String, Vec<Span>> = OrderedMap::new();
    let mut overlay_ids: Vec<String> = Vec::new();
    for overlay in &overlay_nodes {
        let id = overlay.get("id").and_then(|v| v.as_str()).unwrap_or("").to_string();
        overlay_ids.push(id.clone());
        let intervals = intervals_of(overlay);
        overlay_executable.insert(id.clone(), to_spans(union_intervals(&intervals)?));
        // Fixed ldr/bx veneers, literal pools and audited alignment bytes are
        // already proven structural by the executable inventory: source-owned
        // exact assembly, not decompilation debt.
        let structural: Vec<Interval> = intervals
            .into_iter()
            .filter(|interval| {
                matches!(
                    interval.kind.as_str(),
                    "veneer" | "literal_pool" | "executable_alignment"
                )
            })
            .collect();
        overlay_retained.insert(id, to_spans(union_intervals(&structural)?));
    }
    let executable_of = |id: &str| -> Vec<Span> {
        overlay_executable.get(&id.to_string()).cloned().unwrap_or_default()
    };

    let exact_main: Vec<Span> =
        exact_main_spans(options.exact, &main_executable)
            .iter()
            .flat_map(|(_, spans)| spans.iter().copied())
            .collect();
    let exact_overlay_owners_by_resource = exact_overlay_owners(options.exact);
    let mut exact_overlay_by_resource: OrderedMap<String, Vec<Span>> = OrderedMap::new();
    for (overlay, owners) in exact_overlay_owners_by_resource.iter() {
        let flattened: Vec<Span> =
            owners.iter().flat_map(|owner| owner.spans.iter().copied()).collect();
        exact_overlay_by_resource
            .insert(overlay.clone(), intersect(&flattened, &executable_of(overlay)));
    }

    let boundaries = main_boundaries(options.exact);
    let semantic_coverage: SemanticCoverage = match options.semantic {
        Some(tree) => semantic_spans(tree, &boundaries, &main_executable, &overlay_executable),
        None => SemanticCoverage::empty(),
    };

    // Exact always wins over semantic. Supersession (exact C replaced a draft)
    // and outside-extent (a semantic source claims an address the audit does
    // not call executable) are counted apart on purpose: folding them into one
    // figure has already caused one misreading of this map.
    let exact_main_union = normalize(&exact_main);
    let semantic_main_input: Vec<Span> =
        semantic_coverage.main.iter().flat_map(|(_, s)| s.iter().copied()).collect();
    let semantic_main = subtract(&semantic_main_input, &exact_main_union);
    let mut semantic_overlay_by_resource: OrderedMap<String, Vec<Span>> = OrderedMap::new();
    let mut semantic_superseded =
        span_bytes(&semantic_main_input) - span_bytes(&semantic_main);
    let mut semantic_outside_extent = 0i64;
    for (overlay, spans) in semantic_coverage.overlays.iter() {
        let executable = executable_of(overlay);
        let in_extent = intersect(spans, &executable);
        let owned = subtract(
            &in_extent,
            &exact_overlay_by_resource.get(overlay).cloned().unwrap_or_default(),
        );
        semantic_outside_extent += span_bytes(spans) - span_bytes(&in_extent);
        semantic_superseded += span_bytes(&in_extent) - span_bytes(&owned);
        if !owned.is_empty() {
            semantic_overlay_by_resource.insert(overlay.clone(), owned);
        }
    }

    let exact_main_bytes = span_bytes(&exact_main_union);
    let exact_overlay_bytes: i64 =
        exact_overlay_by_resource.values().map(|spans| span_bytes(spans)).sum();
    if let Some(tracked) = &tracked {
        let tracked_main = tracked
            .get("main")
            .and_then(|node| node.get("full_c_bytes"))
            .and_then(|v| v.as_f64())
            .unwrap_or(f64::NAN);
        let tracked_overlays = tracked
            .get("overlays")
            .and_then(|node| node.get("full_c_bytes"))
            .and_then(|v| v.as_f64())
            .unwrap_or(f64::NAN);
        if !equals_number(exact_main_bytes, tracked_main)
            || !equals_number(exact_overlay_bytes, tracked_overlays)
        {
            return Err(format!(
                "derived exact ownership disagrees with the tracked Full-C report \
                 (main {} vs {}, overlays {} vs {}); regenerate metrics/ before the coverage map",
                exact_main_bytes,
                crate::jsnum::js_number_string(tracked_main),
                exact_overlay_bytes,
                crate::jsnum::js_number_string(tracked_overlays),
            ));
        }
    }

    let semantic_main_bytes = span_bytes(&semantic_main);
    let semantic_overlay_bytes: i64 =
        semantic_overlay_by_resource.values().map(|spans| span_bytes(spans)).sum();
    let executable_bytes = inventory
        .get("total_union_bytes")
        .and_then(|v| v.as_f64())
        .ok_or_else(|| "executable inventory has no total_union_bytes".to_string())?
        as i64;
    let exact_bytes = exact_main_bytes + exact_overlay_bytes;
    let semantic_bytes = semantic_main_bytes + semantic_overlay_bytes;

    // ------------------------------------------------ executable universe
    // Dark grey is reserved for spans with an explicit retained-assembly
    // contract. A closed semantic census does not by itself prove the rest
    // permanent, so any complement without evidence stays unresolved assembly.
    let main_retained = retained_main_spans();
    let mut executable_areas: Vec<Area> = vec![area(
        "main",
        "Main image",
        main_owner_tiles(
            &main_executable,
            &boundaries,
            &exact_main_union,
            &semantic_main,
            &main_retained,
        ),
    )];
    let mut overlay_tiles: Vec<Tile> = Vec::new();
    for id in &overlay_ids {
        let executable = executable_of(id);
        let empty_owners: Vec<OverlayOwner> = Vec::new();
        let exact_owners =
            exact_overlay_owners_by_resource.get(id).cloned().unwrap_or(empty_owners);
        let census_owners = semantic_coverage.overlay_owners.get(id).cloned().unwrap_or_default();
        let semantic_owners = if !census_owners.is_empty() {
            census_owners
        } else {
            semantic_overlay_by_resource.get(id).cloned().unwrap_or_default()
        };
        overlay_tiles.extend(overlay_owner_tiles(
            id,
            &executable,
            &exact_owners,
            &semantic_owners,
            &overlay_retained.get(id).cloned().unwrap_or_default(),
        ));
    }
    executable_areas.push(area("overlays", "Decoded code overlays", overlay_tiles));

    // ----------------------------------------------------------- ROM image
    let streams = overlay_streams(options.exact)?;
    let stream_spans: Vec<Span> = streams
        .values()
        .map(|stream| Span::new(stream.start, stream.start + stream.rom_bytes))
        .collect();
    let mut code_input = main_executable.clone();
    code_input.extend(stream_spans.iter().copied());
    let code_spans = normalize(&code_input);
    let data_spans = subtract(&[Span::new(ROM_BASE, ROM_BASE + rom_bytes)], &code_spans);

    let mut rom_areas: Vec<Area> = vec![area(
        "rom-main-code",
        "Main image code",
        main_bands(
            &main_executable,
            &exact_main_union,
            &semantic_main,
            &main_retained,
            65536,
        ),
    )];
    let mut ordered_streams: Vec<(String, crate::assets::Stream)> =
        streams.iter().map(|(id, stream)| (id.clone(), *stream)).collect();
    // Stable, like `Array#sort`: streams sharing a start keep manifest order.
    ordered_streams.sort_by_key(|entry| entry.1.start);
    let mut stream_tiles: Vec<Tile> = Vec::new();
    for (overlay, stream) in &ordered_streams {
        let decoded = span_bytes(&executable_of(overlay));
        // A compressed stream has no per-byte correspondence with the code it
        // decodes to, so its tile is sized in ROM bytes and merely *shaded* by
        // the share of decoded bytes each category owns.
        let exact_share = if decoded != 0 {
            span_bytes(&exact_overlay_by_resource.get(overlay).cloned().unwrap_or_default()) as f64
                / decoded as f64
        } else {
            0.0
        };
        let semantic_share = if decoded != 0 {
            span_bytes(&semantic_overlay_by_resource.get(overlay).cloned().unwrap_or_default())
                as f64
                / decoded as f64
        } else {
            0.0
        };
        let exact_part = crate::jsnum::js_round(stream.rom_bytes as f64 * exact_share) as i64;
        let semantic_part = crate::jsnum::js_round(stream.rom_bytes as f64 * semantic_share) as i64;
        let mut tile = Tile {
            label: overlay.strip_prefix("resource_").unwrap_or(overlay).to_string(),
            bytes: stream.rom_bytes,
            ..Tile::default()
        };
        tile.set_category("exact_c", exact_part);
        tile.set_category("semantic_c", semantic_part);
        tile.set_category("assembly", stream.rom_bytes - exact_part - semantic_part);
        stream_tiles.push(tile);
    }
    rom_areas.push(area(
        "rom-overlay-streams",
        "Compressed code overlays",
        group_tiles(&stream_tiles, 49152),
    ));

    // PORT NOTE: `preferVerifiedAssets` falls back to the tracked manifest when
    // `verifiedAssetTiles` returns undefined (no verified build on disk). A
    // *throw* inside the verified reader is not a fallback: `??` only catches
    // `undefined`, so a schema or conservation refusal propagates.
    let asset_tiles = match options.prefer_verified_assets {
        true => match crate::verified::verified_asset_tiles(options.exact, &options.target)? {
            Some(tiles) => tiles,
            None => manifest_asset_tiles(options.exact, rom_bytes, &data_spans)?,
        },
        false => manifest_asset_tiles(options.exact, rom_bytes, &data_spans)?,
    };
    rom_areas.push(area("rom-data", "Assets & data", asset_tiles));

    let rom_check: i64 = rom_areas.iter().map(|item| item.bytes).sum();
    if rom_check != rom_bytes {
        return Err(format!("ROM areas cover {rom_check} of {rom_bytes} bytes"));
    }
    let executable_check: i64 = executable_areas.iter().map(|item| item.bytes).sum();
    if executable_check != executable_bytes {
        return Err(format!(
            "executable areas cover {executable_check} of {executable_bytes} bytes"
        ));
    }

    let retained_bytes: i64 = executable_areas
        .iter()
        .map(|item| item.categories.get(&"retained_asm".to_string()).copied().unwrap_or(0))
        .sum();
    let assembly_bytes = executable_bytes - exact_bytes - semantic_bytes - retained_bytes;

    let mut unresolved = semantic_coverage.unresolved.clone();
    unresolved.sort_by(|left, right| crate::jsnum::utf16_cmp(left, right));

    let document = Value::Obj(vec![
        ("format".into(), Value::Num(1.0)),
        ("kind".into(), Value::Str("golden-sun-rom-coverage-map".into())),
        ("target".into(), Value::Str(options.target.clone())),
        ("derivation".into(), Value::Str("tracked-evidence-v1".into())),
        ("rom_bytes".into(), Value::Num(rom_bytes as f64)),
        ("executable_bytes".into(), Value::Num(executable_bytes as f64)),
        (
            "categories".into(),
            Value::Obj(vec![
                ("exact_c".into(), category_entry(exact_bytes, executable_bytes)?),
                ("semantic_c".into(), category_entry(semantic_bytes, executable_bytes)?),
                ("assembly".into(), category_entry(assembly_bytes, executable_bytes)?),
                ("retained_asm".into(), category_entry(retained_bytes, executable_bytes)?),
                (
                    "asset_data".into(),
                    Value::Obj(vec![
                        ("bytes".into(), Value::Num(span_bytes(&data_spans) as f64)),
                        ("percent_of_executable".into(), Value::Num(0.0)),
                    ]),
                ),
            ]),
        ),
        (
            "main".into(),
            Value::Obj(vec![
                ("executable_bytes".into(), Value::Num(span_bytes(&main_executable) as f64)),
                ("exact_c_bytes".into(), Value::Num(exact_main_bytes as f64)),
                ("semantic_c_bytes".into(), Value::Num(semantic_main_bytes as f64)),
            ]),
        ),
        (
            "overlays".into(),
            Value::Obj(vec![
                (
                    "executable_bytes".into(),
                    Value::Num((executable_bytes - span_bytes(&main_executable)) as f64),
                ),
                ("exact_c_bytes".into(), Value::Num(exact_overlay_bytes as f64)),
                ("semantic_c_bytes".into(), Value::Num(semantic_overlay_bytes as f64)),
            ]),
        ),
        (
            "provenance".into(),
            Value::Obj(vec![
                ("exact_source".into(), Value::Str(options.exact.id().to_string())),
                (
                    "semantic_source".into(),
                    Value::Str(
                        options.semantic.map(|tree| tree.id().to_string()).unwrap_or_else(
                            || "absent".to_string(),
                        ),
                    ),
                ),
                ("semantic_sources".into(), Value::Num(semantic_coverage.sources as f64)),
                (
                    "main_semantic_census".into(),
                    Value::Str(
                        if semantic_coverage.main_census_closed { "closed" } else { "open" }
                            .to_string(),
                    ),
                ),
                ("semantic_superseded_bytes".into(), Value::Num(semantic_superseded as f64)),
                (
                    "semantic_outside_extent_bytes".into(),
                    Value::Num(semantic_outside_extent as f64),
                ),
                (
                    "semantic_unresolved".into(),
                    Value::Arr(unresolved.into_iter().map(Value::Str).collect()),
                ),
            ]),
        ),
        ("rom_areas".into(), Value::Arr(rom_areas.iter().map(area_value).collect())),
        (
            "executable_areas".into(),
            Value::Arr(executable_areas.iter().map(area_value).collect()),
        ),
    ]);

    Ok(CoverageMap { document, rom_areas, executable_areas })
}

/// `derived !== tracked` where `tracked` may be missing.
///
/// PORT NOTE: a missing tracked field is `undefined` in JavaScript, and
/// `number !== undefined` is true, so the report is rejected. A `NaN` stand-in
/// compares unequal to everything for the same result. clippy would like this
/// written as a direct comparison; it is a named helper precisely because the
/// NaN case is the interesting one.
fn equals_number(derived: i64, tracked: f64) -> bool {
    derived as f64 == tracked
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn rom_size_rejects_an_unknown_target() {
        assert_eq!(rom_size("gs1-en"), Ok(0x0080_0000));
        assert_eq!(rom_size("gs2-en"), Ok(0x0100_0000));
        let error = rom_size("gs3").expect_err("unknown target");
        assert!(error.contains("unsupported decomp target \"gs3\""), "{error}");
    }

    #[test]
    fn a_missing_tracked_field_never_compares_equal() {
        assert!(equals_number(5, 5.0));
        assert!(!equals_number(5, f64::NAN), "an absent tracked figure must not pass");
    }

    #[test]
    fn area_value_omits_unset_tile_members() {
        let mut tile = Tile { label: "a".into(), bytes: 4, ..Tile::default() };
        tile.set_category("exact_c", 4);
        let built = area_value(&area("main", "Main image", vec![tile]));
        let tiles = built.get("tiles").and_then(|v| v.as_array()).expect("tiles");
        assert!(tiles[0].get("group").is_none(), "an unset group is not written as null");
        assert!(tiles[0].get("address").is_none());
        assert_eq!(tiles[0].get("label").and_then(|v| v.as_str()), Some("a"));
    }
}

