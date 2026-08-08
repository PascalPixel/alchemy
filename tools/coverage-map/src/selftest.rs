// `selfTest`: the tool's own executable specification.
//
// WHY it is a runtime path and not `#[cfg(test)]`: the TypeScript exposes it as
// `--self-test`, and the native repository gate drives it without a test runner. Porting
// it into `mod tests` would move the assertions out of the shipped binary and
// change what `--self-test` means. The crate's unit tests cover the same
// ground from the other direction; this file is the oracle-shaped copy.
//
// PORT NOTE: every `throw new Error(...)` becomes `return Err(...)` with the
// message verbatim, and the final `console.log` line is returned to the caller
// rather than printed here, so the CLI owns all output.

use crate::assets::asset_bucket;
use crate::boxtree::{readme_path, readme_with_cache_buster, readme_with_done_headline};
use crate::boxtree::{render_box_trees, svg_cache_version, CODE_FRACTION, CORE_HUE};
use crate::cli::{options_of, resolve_exact_tree, resolve_semantic_tree, semantic_erase_refusal};
use crate::model::{area, category_total, Area, Tile, CATEGORY_ORDER};
use crate::ordered::OrderedMap;
use crate::ownership::{overlay_placeholder_spans, semantic_spans};
use crate::pipeline::CoverageMap;
use crate::spans::{intersect, normalize, region_span, span_bytes, subtract, Span};
use crate::svg::{category_style, render_svg};
use crate::tiles::{main_bands, main_owner_tiles, overlay_owner_tiles};
use crate::tree::{mem_tree, ref_tree, SourceTree};
use crate::treemap::{squarify, Rect};

fn refuse(condition: bool, message: &str) -> Result<(), String> {
    if condition {
        return Err(message.to_string());
    }
    Ok(())
}

/// JavaScript's `left > right` and `left < right`, named so that the negation
/// in the sub-pixel assertion stays honest.
///
/// PORT NOTE: clippy wants `!(a > b)` rewritten through `partial_cmp`, which
/// changes what a `NaN` layout means: the TypeScript throws when the
/// comparison does not *hold*, and `NaN > 0` not holding is exactly the
/// failure the assertion is there to catch. Extracting the comparison keeps
/// both the lint and the semantics.
fn js_greater(left: f64, right: f64) -> bool {
    left > right
}

fn js_less(left: f64, right: f64) -> bool {
    left < right
}

fn span(start: i64, end: i64) -> Span {
    Span { start, end }
}

fn tile(label: &str, bytes: i64, categories: &[(&str, i64)]) -> Tile {
    let mut built = Tile {
        label: label.to_string(),
        bytes,
        ..Tile::default()
    };
    for (name, value) in categories {
        built.set_category(name, *value);
    }
    built
}

/// `renderBoxTree(area, ariaLabel)` with every default supplied.
fn default_box_tree(item: &Area, aria_label: &str) -> Result<String, String> {
    crate::boxtree::render_box_tree(
        item,
        aria_label,
        &CORE_HUE,
        &CODE_FRACTION,
        &["semantic_c", "retained_asm", "exact_c", "humanized_c"],
        &[],
        &item.label,
    )
}

fn count_occurrences(haystack: &str, needle: &str) -> usize {
    haystack.matches(needle).count()
}

pub fn self_test() -> Result<String, String> {
    spans_and_regions()?;
    placeholders_and_buckets()?;
    whole_overlay_claims()?;
    tree_resolution()?;
    cache_buster_and_headline()?;
    readme_and_ref_tree()?;
    erase_refusals()?;
    layout_and_tiles()?;
    let map = self_test_map();
    renderers(&map)?;
    maturity_remainder(&map)?;

    refuse(
        options_of(&[String::from("--unknown")]).is_ok(),
        "unknown argument was accepted",
    )?;
    let parsed = options_of(&[String::from("--semantic-ref"), String::from("none")])
        .map_err(|_| "option parsing failed".to_string())?;
    refuse(
        parsed.semantic.as_deref() != Some("none"),
        "option parsing failed",
    )?;

    Ok("self-test=ok tool=coverage_map".to_string())
}

fn spans_and_regions() -> Result<(), String> {
    refuse(
        span_bytes(&normalize(&[span(0, 4), span(2, 8)])) != 8,
        "span normalization failed",
    )?;
    refuse(
        span_bytes(&intersect(&[span(0, 10)], &[span(4, 6), span(8, 20)])) != 4,
        "span intersection failed",
    )?;
    let removed = subtract(&[span(0, 10)], &[span(2, 4)]);
    refuse(
        removed.len() != 2 || removed[0].end != 2 || removed[1].start != 4,
        "span subtraction failed",
    )?;

    let boundaries = [0x0800_0000, 0x0800_0010, 0x0800_0030];
    let found = region_span(0x0800_0010, &boundaries, 0x0800_0040);
    refuse(
        found.start != 0x0800_0010 || found.end != 0x0800_0030,
        "region span failed",
    )?;
    refuse(
        region_span(0x0800_0030, &boundaries, 0x0800_0040).end != 0x0800_0040,
        "final region span failed",
    )
}

fn placeholders_and_buckets() -> Result<(), String> {
    let placeholders = overlay_placeholder_spans(
        "AlchemyC_02000010:\n\t.space 0x10\n.L_02000020:\n\tbx lr\nAlchemyC_02000030:\n\n\t.space 8\n\tpush\t{lr}\n",
    );
    refuse(
        placeholders.len() != 2 || span_bytes(&placeholders) != 24,
        "overlay placeholder measurement failed",
    )?;
    refuse(
        !overlay_placeholder_spans("\t.space 0x40\n").is_empty(),
        "assembly reservation counted as C",
    )?;
    refuse(
        asset_bucket("golden-sun-pcm-wave-series").id != "audio",
        "audio bucket failed",
    )?;
    refuse(
        asset_bucket("brand-new-package").id != "other",
        "unknown bucket failed",
    )
}

/// The synthetic tree `selfTest` builds from two closures.
fn source_tree(regions: &str, overlay_sources: &[&str]) -> SourceTree {
    mem_tree(
        "test",
        &[("semantic", overlay_sources)],
        &[("semantic/regions.json", regions.to_string())],
    )
}

fn whole_overlay_claims() -> Result<(), String> {
    let mut extent: OrderedMap<String, Vec<Span>> = OrderedMap::new();
    extent.insert(
        "resource_375".to_string(),
        vec![span(0x0200_0000, 0x0200_0100)],
    );

    let claimed = semantic_spans(
        &source_tree(
            r#"{"full_overlays":[{"overlay":"resource_375","evidence":"converted in full"}]}"#,
            &["resource_375_c_02000030.c"],
        ),
        &[],
        &[],
        &extent,
    );
    refuse(
        span_bytes(
            claimed
                .overlays
                .get(&"resource_375".to_string())
                .map(Vec::as_slice)
                .unwrap_or(&[]),
        ) != 256,
        "a whole-code-overlay claim did not take the code overlay's executable extent",
    )?;
    refuse(
        !claimed.unresolved.is_empty() || claimed.sources != 1,
        "a claimed overlay's owners were still reported unresolved",
    )?;

    let unbacked = semantic_spans(
        &source_tree(
            r#"{"full_overlays":[{"overlay":"resource_375","evidence":"no sources"}]}"#,
            &[],
        ),
        &[],
        &[],
        &extent,
    );
    refuse(
        !unbacked
            .overlays
            .get(&"resource_375".to_string())
            .map(Vec::is_empty)
            .unwrap_or(true),
        "a whole-code-overlay claim with no semantic source credited bytes",
    )?;

    let no_extent = semantic_spans(
        &source_tree(
            r#"{"full_overlays":[{"overlay":"resource_999","evidence":"not audited"}]}"#,
            &["resource_999_c_02000030.c"],
        ),
        &[],
        &[],
        &extent,
    );
    refuse(
        !no_extent
            .overlays
            .get(&"resource_999".to_string())
            .map(Vec::is_empty)
            .unwrap_or(true),
        "a whole-code-overlay claim without an audited extent credited bytes",
    )?;

    let unlisted = semantic_spans(
        &source_tree("{}", &["resource_375_c_02000030.c"]),
        &[],
        &[],
        &extent,
    );
    refuse(
        unlisted.unresolved.len() != 1,
        "an unlisted overlay owner is no longer reported",
    )?;

    let per_owner = semantic_spans(
        &source_tree(
            r#"{"manual_regions":[{"overlay":"resource_375","entry":"0x02000030","span_bytes":64}]}"#,
            &["resource_375_c_02000030.c"],
        ),
        &[],
        &[],
        &extent,
    );
    refuse(
        span_bytes(
            per_owner
                .overlays
                .get(&"resource_375".to_string())
                .map(Vec::as_slice)
                .unwrap_or(&[]),
        ) != 64,
        "per-owner manual_regions sizing regressed",
    )
}

fn tree_resolution() -> Result<(), String> {
    let semantic_bearing = mem_tree("exact-with-semantic", &[("semantic", &["08000000.c"])], &[]);
    refuse(
        resolve_semantic_tree(&semantic_bearing, None, Some("refs/heads/no-such-source")).is_ok(),
        "an unresolvable recorded semantic source was accepted",
    )?;
    refuse(
        resolve_semantic_tree(&semantic_bearing, None, None)
            .ok()
            .flatten()
            .map(|tree| tree.id().to_string())
            .as_deref()
            != Some("exact-with-semantic"),
        "the describes-itself heuristic stopped applying with no recorded source",
    )?;
    refuse(
        resolve_semantic_tree(&semantic_bearing, None, Some("worktree"))
            .ok()
            .flatten()
            .map(|tree| tree.id().to_string())
            .as_deref()
            != Some("exact-with-semantic"),
        "a recorded worktree source did not fall through to the heuristic",
    )?;
    refuse(
        resolve_semantic_tree(&semantic_bearing, Some("none"), Some("refs/heads/example"))
            .ok()
            .flatten()
            .is_some(),
        "an explicit --semantic-ref none was overridden by the record",
    )
}

fn cache_buster_and_headline() -> Result<(), String> {
    let embed = "![a](assets/readme/gs1-en-core.svg) ![b](assets/readme/gs1-en-overlays.svg) \
                 ![c](assets/readme/gs1-en-images.svg) ![d](assets/readme/gs1-en-music.svg)";
    let versions = |core: &str| -> Vec<(&'static str, String)> {
        vec![
            ("core", core.to_string()),
            ("overlays", "abcd1234".to_string()),
            ("images", "abcd1234".to_string()),
            ("music", "abcd1234".to_string()),
        ]
    };
    let same = versions("abcd1234");
    let once = readme_with_cache_buster(embed, "gs1-en", &same);
    refuse(
        once != "![a](assets/readme/gs1-en-core.svg?v=abcd1234) \
                 ![b](assets/readme/gs1-en-overlays.svg?v=abcd1234) \
                 ![c](assets/readme/gs1-en-images.svg?v=abcd1234) \
                 ![d](assets/readme/gs1-en-music.svg?v=abcd1234)"
            .replace("                 ", ""),
        "the cache-buster was not applied to a bare embed",
    )?;
    refuse(
        readme_with_cache_buster(&once, "gs1-en", &same) != once,
        "the cache-buster is not idempotent",
    )?;
    let replaced = readme_with_cache_buster(&once, "gs1-en", &versions("99887766"));
    refuse(
        !replaced.contains("![a](assets/readme/gs1-en-core.svg?v=99887766)"),
        "an existing cache-buster was not replaced",
    )?;
    refuse(
        svg_cache_version("<svg/>") == svg_cache_version("<svg />"),
        "the cache version does not track the SVG bytes",
    )?;
    refuse(
        svg_cache_version("<svg/>") != svg_cache_version("<svg/>"),
        "the cache version is not deterministic",
    )?;

    let headline = CoverageMap {
        document: crate::json::parse(
            r#"{"executable_bytes":1000,"categories":{"exact_c":{"bytes":201},
                "retained_asm":{"bytes":58}}}"#,
        )
        .map_err(|error| error.to_string())?,
        rom_areas: Vec::new(),
        executable_areas: Vec::new(),
    };
    refuse(
        readme_with_done_headline("## Progress: Currently 20% of the way done", &headline)?
            != "## DONE: Currently 26%",
        "the README DONE headline does not track exact C plus permanent ASM",
    )
}

/// The four recovered Djinn stay at their native 64 px presentation size.
fn readme_and_ref_tree() -> Result<(), String> {
    let readme = std::fs::read(readme_path())
        .map(|bytes| String::from_utf8_lossy(&bytes).into_owned())
        .map_err(|error| format!("cannot read the README: {error}"))?;
    let mut found = 0usize;
    let mut correct = 0usize;
    // PORT NOTE: hand-scanned rather than regex-matched. The TypeScript pattern
    // `<img src="assets\/readme\/djinn_10[1-4]_idle\.gif" width="(\d+)" height="(\d+)"`
    // has no lookaround and one bounded character class, so a literal scan is
    // exactly equivalent -- and `\d` there is ASCII-only, which `is_ascii_digit`
    // matches and a Unicode-aware digit test would not.
    for index in b'1'..=b'4' {
        let needle = format!(
            "<img src=\"assets/readme/djinn_10{}_idle.gif\" width=\"",
            index as char
        );
        let Some(at) = readme.find(&needle) else {
            continue;
        };
        found += 1;
        let rest = &readme[at + needle.len()..];
        let width: String = rest.chars().take_while(char::is_ascii_digit).collect();
        let after = &rest[width.len()..];
        let Some(after) = after.strip_prefix("\" height=\"") else {
            continue;
        };
        let height: String = after.chars().take_while(char::is_ascii_digit).collect();
        if width == "64" && height == "64" {
            correct += 1;
        }
    }
    refuse(
        found != 4 || correct != 4,
        "README Djinn must remain exactly 64 by 64 pixels",
    )?;

    let head_tree = ref_tree("HEAD").ok_or_else(|| "HEAD did not resolve to a tree".to_string())?;
    refuse(
        !head_tree.list("asm").iter().any(|name| !name.contains('.')),
        "ref tree listed no asm/ subdirectories; a tree walk would stop at the first level",
    )?;

    refuse(
        resolve_exact_tree(None, None)
            .map(|tree| tree.id().to_string())
            .as_deref()
            != Ok("worktree"),
        "the exact source did not default to the worktree",
    )?;
    refuse(
        resolve_exact_tree(None, Some("worktree"))
            .map(|tree| tree.id().to_string())
            .as_deref()
            != Ok("worktree"),
        "a recorded worktree exact source was not honoured",
    )?;
    refuse(
        resolve_exact_tree(Some("worktree"), Some("refs/heads/example"))
            .map(|tree| tree.id().to_string())
            .as_deref()
            != Ok("worktree"),
        "an explicit --exact-ref worktree did not override the recorded ref",
    )?;
    refuse(
        resolve_exact_tree(None, Some("origin/no-such-lighthouse")).is_ok(),
        "an unavailable recorded exact ref was accepted",
    )?;
    refuse(
        resolve_exact_tree(Some("origin/no-such-lighthouse"), None).is_ok(),
        "an unavailable requested exact ref was accepted",
    )
}

fn erase_refusals() -> Result<(), String> {
    let with_semantic = crate::json::parse(
        r#"{"categories":{"semantic_c":{"bytes":391428}},
            "provenance":{"semantic_source":"refs/heads/example"}}"#,
    )
    .map_err(|error| error.to_string())?;
    refuse(
        semantic_erase_refusal(false, None, Some(&with_semantic)).is_none(),
        "unresolved semantic coverage was allowed to erase a tracked value",
    )?;
    refuse(
        semantic_erase_refusal(true, None, Some(&with_semantic)).is_some(),
        "resolved semantic coverage was refused",
    )?;
    refuse(
        semantic_erase_refusal(false, Some("none"), Some(&with_semantic)).is_some(),
        "an explicit --semantic-ref none was refused",
    )?;
    refuse(
        semantic_erase_refusal(false, None, None).is_some(),
        "a first write with no tracked map was refused",
    )?;
    let empty = crate::json::parse(
        r#"{"categories":{"semantic_c":{"bytes":0}},"provenance":{"semantic_source":"none"}}"#,
    )
    .map_err(|error| error.to_string())?;
    refuse(
        semantic_erase_refusal(false, None, Some(&empty)).is_some(),
        "a tracked map with no semantic coverage was refused",
    )
}

fn self_test_tiles() -> Vec<Tile> {
    vec![
        tile("a", 60, &[("exact_c", 30), ("retained_asm", 30)]),
        tile("b", 40, &[("semantic_c", 40)]),
    ]
}

fn layout_and_tiles() -> Result<(), String> {
    let tiles = self_test_tiles();
    let placed = squarify(
        &tiles,
        |item| item.bytes as f64,
        Rect {
            x: 0.0,
            y: 0.0,
            width: 100.0,
            height: 100.0,
        },
    )?;
    refuse(placed.len() != 2, "treemap placement failed")?;
    let placed_area: f64 = placed
        .iter()
        .map(|item| item.rect.width * item.rect.height)
        .sum();
    refuse(
        (placed_area - 10000.0).abs() > 1.0,
        "treemap does not fill its rectangle",
    )?;
    for item in &placed {
        refuse(
            item.rect.x < -0.01
                || item.rect.y < -0.01
                || item.rect.x + item.rect.width > 100.01
                || item.rect.y + item.rect.height > 100.01,
            "treemap escaped its rectangle",
        )?;
    }
    let sizes = [999_999.0f64, 1.0f64];
    let subpixel = squarify(
        &sizes,
        |item| *item,
        Rect {
            x: 0.0,
            y: 0.0,
            width: 1600.0,
            height: 900.0,
        },
    )?;
    let smallest = subpixel
        .get(1)
        .map(|item| item.rect.width.min(item.rect.height))
        .unwrap_or(f64::NAN);
    refuse(
        subpixel.len() != 2 || !js_greater(smallest, 0.0) || !js_less(smallest, 0.5),
        "treemap enlarged or dropped a sub-0.5-pixel byte leaf",
    )?;

    let bands = main_bands(
        &[span(0x0800_0000, 0x0800_0100)],
        &[span(0x0800_0000, 0x0800_0040)],
        &[span(0x0800_0040, 0x0800_0080)],
        &[span(0x0800_00c0, 0x0800_00e0)],
        128,
    );
    refuse(
        bands.len() != 2
            || bands[0].bytes != 128
            || bands[0].category("exact_c") != 64
            || bands[0].category("semantic_c") != 64
            || bands[1].category("assembly") != 96
            || bands[1].category("retained_asm") != 32,
        "main band composition failed",
    )?;

    let overlay_classification = overlay_owner_tiles(
        "resource_test",
        &[span(0, 100)],
        &[crate::ownership::OverlayOwner {
            label: "AlchemyC_00000000".to_string(),
            entry: 0,
            spans: vec![span(0, 20)],
        }],
        &[span(20, 40)],
        &[span(40, 50), span(90, 100)],
    );
    refuse(
        category_total(&overlay_classification, "exact_c") != 20
            || category_total(&overlay_classification, "semantic_c") != 20
            || category_total(&overlay_classification, "retained_asm") != 60
            || category_total(&overlay_classification, "assembly") != 0
            || overlay_classification
                .iter()
                .map(|tile| tile.bytes)
                .sum::<i64>()
                != 100,
        "overlay owner tiles lost exact structural-assembly classification",
    )?;

    let owner_tiles = main_owner_tiles(
        &[span(0x0800_0000, 0x0800_0100)],
        &[0x0800_0000, 0x0800_0030, 0x0800_00a0],
        &[span(0x0800_0000, 0x0800_0040)],
        &[span(0x0800_0040, 0x0800_0080)],
        &[span(0x0800_00c0, 0x0800_00e0)],
    );
    let widths: Vec<String> = owner_tiles
        .iter()
        .map(|tile| tile.bytes.to_string())
        .collect();
    refuse(
        owner_tiles.len() != 3
            || widths.join(",") != "48,112,96"
            || owner_tiles[1].category("semantic_c") != 64
            || owner_tiles[2].category("retained_asm") != 32,
        "main owner tiles did not retain audited boundaries",
    )?;

    let crossing = main_owner_tiles(
        &[span(0x0800_fff0, 0x0801_0010)],
        &[0x0800_fff0],
        &[],
        &[],
        &[],
    );
    let crossing_widths: Vec<String> = crossing.iter().map(|tile| tile.bytes.to_string()).collect();
    // PORT NOTE: `new Set(...).size` deduplicates; an ordered vector with an
    // explicit dedupe reproduces it without importing a hashing container.
    let mut groups: Vec<String> = crossing
        .iter()
        .map(|tile| tile.group.clone().unwrap_or_default())
        .collect();
    groups.dedup();
    groups.sort_by(|left, right| crate::jsnum::utf16_cmp(left, right));
    groups.dedup();
    refuse(
        crossing.len() != 2 || crossing_widths.join(",") != "16,16" || groups.len() != 2,
        "a main-image owner was not split at its real 64 KiB bank edge",
    )?;

    let retained_tree = default_box_tree(
        &area(
            "retained",
            "Retained",
            vec![tile("r", 32, &[("retained_asm", 32)])],
        ),
        "retained colour test",
    )?;
    refuse(
        !retained_tree.contains("oklch(0.740"),
        "permanent assembly did not inherit the former semantic colour",
    )?;
    refuse(
        !retained_tree.contains("DONE 100.0%"),
        "permanent assembly was omitted from DONE",
    )?;
    refuse(
        retained_tree.contains("Unknown 0.0%"),
        "zero-byte dashboard category leaked into the legend",
    )?;
    let semantic_tree = default_box_tree(
        &area(
            "semantic",
            "Semantic",
            vec![tile("s", 32, &[("semantic_c", 32)])],
        ),
        "semantic colour test",
    )?;
    refuse(
        !semantic_tree.contains("oklch(0.930 0.000") || semantic_tree.contains("#333"),
        "semantic C did not inherit the former light-gray colour",
    )
}

/// The literal `map` of the TypeScript self-test.
fn self_test_map() -> CoverageMap {
    let document = crate::json::parse(
        r#"{"format":1,"kind":"golden-sun-rom-coverage-map","target":"gs1-en",
            "derivation":"tracked-evidence-v1","rom_bytes":1024,"executable_bytes":200,
            "categories":{
              "exact_c":{"bytes":60,"percent_of_executable":30},
              "semantic_c":{"bytes":40,"percent_of_executable":20},
              "assembly":{"bytes":80,"percent_of_executable":40},
              "retained_asm":{"bytes":20,"percent_of_executable":10},
              "asset_data":{"bytes":824,"percent_of_executable":0}},
            "main":{"executable_bytes":120,"exact_c_bytes":60,"semantic_c_bytes":20},
            "overlays":{"executable_bytes":80,"exact_c_bytes":0,"semantic_c_bytes":20},
            "provenance":{"exact_source":"worktree","semantic_source":"refs/heads/example",
              "semantic_sources":2,"semantic_superseded_bytes":0,
              "semantic_outside_extent_bytes":0,"main_semantic_census":"open",
              "semantic_unresolved":[]}}"#,
    )
    .expect("the self-test map literal is valid JSON");
    CoverageMap {
        document,
        rom_areas: vec![area(
            "rom-data",
            "Assets & data",
            vec![tile("Other data", 824, &[("asset_data", 824)])],
        )],
        executable_areas: vec![area("main", "Main image", self_test_tiles())],
    }
}

fn renderers(map: &CoverageMap) -> Result<(), String> {
    let svg = render_svg(map)?;
    refuse(
        !svg.starts_with("<svg ") || !svg.trim_end().ends_with("</svg>"),
        "SVG shape failed",
    )?;
    let box_tree = default_box_tree(&map.executable_areas[0], "self-test box tree")?;
    refuse(
        !box_tree.contains("font-family:Weyard;font-size:16px")
            || !box_tree.contains(".rectangle-label{font-size:8px;}"),
        "box-tree chrome or rectangle-label typography drifted",
    )?;
    refuse(
        !box_tree.contains("Exact 30.0%")
            || !box_tree.contains("Semantic 40.0%")
            || !box_tree.contains("Permanent ASM 30.0%")
            || box_tree.contains("Unknown"),
        "box-tree title or legend is missing from the reproducible SVG",
    )?;
    refuse(
        !box_tree.contains("DONE 60.0%") || !box_tree.contains("dominant-baseline=\"middle\""),
        "box-tree completion or vertically centred legend is missing",
    )?;
    let legend_order: Vec<Option<usize>> = ["Semantic 40.0%", "Permanent ASM 30.0%", "Exact 30.0%"]
        .iter()
        .map(|legend| box_tree.find(&format!(">{legend}</text>")))
        .collect();
    let ascending = legend_order.iter().all(Option::is_some)
        && legend_order.windows(2).all(|pair| pair[0] < pair[1]);
    refuse(
        !ascending,
        "box-tree legend no longer runs from semantic through assembly to exact",
    )?;
    refuse(
        !box_tree.contains("viewBox=\"0 0 540 304\"")
            || !box_tree.contains("width=\"540\" height=\"304\""),
        "box tree lost its fixed pixel-exact dashboard canvas",
    )?;
    refuse(
        !box_tree.contains("width=\"540\" height=\"304\" fill=\"#6d4fc2\" rx=\"8\""),
        "box tree background no longer follows its rounded outer border",
    )?;

    let folder_tree = crate::boxtree::render_box_tree(
        &area(
            "folders",
            "Folders",
            vec![
                Tile {
                    group: Some("first".to_string()),
                    ..tile("one", 60, &[("exact_c", 60)])
                },
                Tile {
                    group: Some("second".to_string()),
                    ..tile("two", 40, &[("assembly", 40)])
                },
            ],
        ),
        "folder self-test",
        &CORE_HUE,
        &CODE_FRACTION,
        &["exact_c", "assembly"],
        &["group"],
        "Folders",
    )?;
    refuse(
        count_occurrences(&folder_tree, "<g aria-label=") != 4
            || count_occurrences(&folder_tree, "<text class=\"weyard rectangle-label\"") != 4
            || folder_tree.contains("data-folder-depth")
            || folder_tree.contains("data-byte-leaf")
            || folder_tree.contains("clip-path")
            || folder_tree.contains("<title>"),
        "box-tree labels are not one unclipped accessible invariant",
    )?;

    refuse(
        svg.contains("undefined") || svg.contains("NaN"),
        "SVG contains unresolved values",
    )?;
    for category in CATEGORY_ORDER {
        let style = category_style(category);
        refuse(
            !svg.to_uppercase()
                .contains(&crate::jstext::escape_text(style.label).to_uppercase()),
            &format!("SVG does not name the {category} category"),
        )?;
        refuse(
            !svg.contains(style.fill),
            &format!("SVG does not draw the {category} category"),
        )?;
    }
    refuse(count_occurrences(&svg, "<rect ") < 4, "SVG drew no tiles")
}

fn maturity_remainder(map: &CoverageMap) -> Result<(), String> {
    let maturity_tree = mem_tree(
        "maturity-test",
        &[],
        &[(
            "assets/manifest.json",
            r#"{"kind":"sprite","size":100,"source":"assets/example.bin"}"#.to_string(),
        )],
    );
    let mut executable_areas = map.executable_areas.clone();
    executable_areas.push(area(
        "overlays",
        "Decoded code overlays",
        vec![tile("test", 80, &[("assembly", 80)])],
    ));
    let maturity_map = CoverageMap {
        document: map.document.clone(),
        rom_areas: vec![area(
            "rom-data",
            "Assets & data",
            vec![tile("Other data", 200, &[("asset_data", 200)])],
        )],
        executable_areas,
    };
    let trees = render_box_trees(&maturity_map, Some(&maturity_tree), false)?;
    let images = trees
        .iter()
        .find(|(name, _)| *name == "images")
        .map(|(_, svg)| svg.clone())
        .unwrap_or_default();
    refuse(
        !images.contains("200 bytes are tiered")
            || !images.contains("Unclassified ROM-image data (byte-represented): 100 bytes")
            || !images.contains("Unknown 50.0%"),
        "asset maturity tree did not include its unclassified remainder",
    )
}

#[cfg(test)]
mod tests {
    /// The self-test is the tool's own specification, so it runs under `cargo
    /// test` as well as under `--self-test`. A failure here is a real defect,
    /// not a fixture problem.
    #[test]
    fn the_self_test_passes() {
        assert_eq!(
            super::self_test().expect("self-test"),
            "self-test=ok tool=coverage_map"
        );
    }
}
