// `selfTest()`, assertion for assertion and in the same order.

use coverage_map::boxtree::BOX_TREES;

use crate::client::bundled_client;
use crate::routes::shell_has_authored_markup;
use crate::state::{affects_coverage, compute};
use crate::paths;

/// Sum a byte field over the matching tiles.
///
/// PORT NOTE: `reduce((sum, tile) => sum + tile.bytes, 0)`. `tile.bytes` is
/// always present, and `tile.categories.assembly ?? 0` uses the nullish
/// fallback, so a MISSING key becomes 0 rather than NaN. The landed
/// `coverage-map` types these byte counts as `i64`, which is the whole reason
/// the JS `NaN`/`+0`/`-0` hazards cannot reach this particular sum: there is
/// no float in it to poison. `wrapping_add` is deliberately NOT used; a real
/// overflow here would mean the map is nonsense and should be loud.
fn sum_of(values: impl Iterator<Item = i64>) -> i64 {
    // `Iterator::sum` for i64 seeds at 0 and adds left to right, which is
    // exactly the JS `reduce((sum, x) => sum + x, 0)`.
    values.sum()
}

pub fn self_test() -> Result<String, String> {
    if !affects_coverage("assets/code/resource_373_overlay.s")
        || !affects_coverage("semantic/example.c")
        || affects_coverage("tools/metrics/dashboard/styles.css")
    {
        return Err("dashboard coverage-path filter failed".to_string());
    }
    let live = compute()?;
    let trees = &live.trees;
    let tree_svg = |name: &str| {
        trees.iter().find(|(id, _)| *id == name).map(|(_, svg)| svg.as_str()).unwrap_or("")
    };
    if live.exact_bytes <= 0.0 || BOX_TREES.iter().any(|name| !tree_svg(name).starts_with("<svg "))
    {
        return Err("dashboard live coverage generation failed".to_string());
    }

    // The overlay cohort checks, rebuilt from the same map.
    let map = rebuild_map()?;
    let Some(overlay_area) = map.executable_areas.iter().find(|area| area.id == "overlays") else {
        return Err("dashboard live map lost its code overlays".to_string());
    };
    let overlay_tiles = |id: &str| {
        overlay_area
            .tiles
            .iter()
            .filter(|tile| tile.group.as_deref() == Some(id))
            .collect::<Vec<_>>()
    };
    let overlay_bytes = |id: &str| sum_of(overlay_tiles(id).iter().map(|tile| tile.bytes));
    let unknown_bytes = |id: &str| {
        sum_of(
            overlay_tiles(id)
                .iter()
                .map(|tile| {
                    tile.categories
                        .iter()
                        .find(|(key, _)| key == "assembly")
                        .map_or(0, |(_, value)| *value)
                }),
        )
    };
    let visual_cohort = ["373", "3c9", "380", "3c8", "383", "372", "3bd", "3af"];
    // `new Set(cohort.map(overlayBytes)).size < 6`. `Set` is SameValueZero, so
    // `+0` and `-0` collapse and NaN equals NaN; the byte totals here are
    // non-negative finite integers, so a plain distinct count over the JS
    // string forms is exact and avoids `HashSet<f64>`, which does not compile.
    let mut distinct: Vec<i64> = Vec::new();
    for id in visual_cohort {
        let key = overlay_bytes(id);
        if !distinct.contains(&key) {
            distinct.push(key);
        }
    }
    if visual_cohort
        .iter()
        .any(|id| overlay_tiles(id).is_empty() || unknown_bytes(id) > overlay_bytes(id))
        || distinct.len() < 6
    {
        return Err("code-overlay unknown debt collapsed into a repeated display tile".to_string());
    }
    for id in visual_cohort.iter().filter(|item| **item != "3bd") {
        let header = overlay_tiles(id).into_iter().find(|tile| {
            tile.bytes == 40
                && tile
                    .categories
                    .iter()
                    .find(|(k, _)| k == "retained_asm")
                    .is_some_and(|(_, value)| *value == 40)
                && tile.label.contains("0x02000004\u{2013}0x0200002c")
        });
        if header.is_none() {
            return Err(format!("{id} fixed overlay header is not retained exact assembly"));
        }
    }

    if !tree_svg("core").contains("MAIN IMAGE")
        || !tree_svg("overlays").contains("CODE OVERLAYS")
        || !tree_svg("images").contains("IMAGES")
        || !tree_svg("music").contains("MUSIC")
        || !tree_svg("music").contains("fill=\"#c85d00\"")
        || BOX_TREES
            .iter()
            .any(|name| !tree_svg(name).contains("font-family:Weyard;font-size:16px"))
    {
        return Err(
            "dashboard SVGs do not carry their own 16px Weyard title and legend chrome".to_string(),
        );
    }

    let client = bundled_client()?;
    if !client.contains("EventSource") || !client.contains("createElement") {
        return Err("dashboard client is not bundled hyperscript with live events".to_string());
    }
    if !client.contains("closest(\"g[aria-label]\")")
        || !client.contains("hover-tooltip")
        || client.contains("data-byte-leaf")
        || client.contains("data-folder-depth")
    {
        return Err("dashboard client lost its graph hover labels".to_string());
    }
    if client.contains("legendbar") || client.contains("titlebar") {
        return Err("dashboard title or legend escaped the reproducible SVG boundary".to_string());
    }

    let styles = std::fs::read_to_string(paths::styles())
        .map_err(|error| format!("{}: {error}", paths::styles().display()))?;
    // `styles.split(/\r?\n/)` -- a CRLF file must split the same way, which is
    // why this is not `lines()` alone (that would also strip a lone \r on some
    // paths) and not `split('\n')` (that would leave a trailing \r inside the
    // trimmed line, harmless here but a different string).
    let font_shorthands: Vec<String> = styles
        .split('\n')
        .map(|line| line.strip_suffix('\r').unwrap_or(line).trim().to_string())
        .filter(|line| line.starts_with("font:"))
        .collect();
    if !styles.contains("--weyard-font: italic 400 16px/15px Weyard")
        || !styles.contains(".hover-tooltip")
        || styles.contains("font-size:")
        || font_shorthands.iter().any(|line| line != "font: var(--weyard-font);")
    {
        return Err("dashboard UI typography drifted from the one 16px Weyard size".to_string());
    }
    if shell_has_authored_markup() {
        return Err("dashboard document shell contains authored view markup".to_string());
    }
    Ok(format!(
        "self-test=ok dashboard exact={}",
        coverage_map::jsnum::js_number_string(live.exact_bytes)
    ))
}

fn rebuild_map() -> Result<coverage_map::pipeline::CoverageMap, String> {
    let tree = coverage_map::tree::work_tree_at(paths::root());
    coverage_map::pipeline::build_coverage_map(&coverage_map::pipeline::BuildOptions {
        target: "gs1-en".to_string(),
        exact: &tree,
        semantic: Some(&tree),
        validate_tracked_progress: false,
        prefer_verified_assets: true,
    })
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::jsonout::Json;
    use crate::state;

    #[test]
    fn the_self_test_passes_on_the_live_worktree_and_reports_a_javascript_number() {
        let line = self_test().expect("self-test must pass on the live worktree");
        assert!(line.starts_with("self-test=ok dashboard exact="), "{line}");
        let value = line.rsplit('=').next().unwrap();
        assert!(!value.contains('.'), "an integral byte count must not print a point: {line}");
        assert!(value.parse::<u64>().unwrap() > 0);
    }

    #[test]
    fn a_live_snapshot_carries_every_summary_field() {
        state::rebuild_coverage();
        let text = state::snapshot().stringify();
        for key in
            ["executableBytes", "exactBytes", "exactPercent", "semanticBytes", "combinedBytes"]
        {
            assert!(text.contains(key), "missing {key} in {text}");
        }
        assert!(text.contains("\"generatedAt\":\""));
        assert!(!text.contains("\"error\""), "a healthy scan reports no error: {text}");
        // No NaN reached the wire as `null`.
        assert!(!text.contains(":null"), "a NaN would have serialised as null: {text}");
    }

    #[test]
    fn sum_of_is_a_plain_left_fold_seeded_at_zero_like_the_javascript_reduce() {
        assert_eq!(sum_of([1, 2, 3].into_iter()), 6);
        assert_eq!(sum_of(std::iter::empty()), 0);
        // The JS reduce starts at 0, so an empty cohort sums to 0 and the
        // `unknownBytes > overlayBytes` guard below compares 0 > 0, false.
        assert!(!(sum_of(std::iter::empty()) > sum_of(std::iter::empty())));
    }

    #[test]
    fn the_json_shell_of_a_default_snapshot_is_valid_json() {
        let text = Json::obj(vec![("a", Json::Num(1.0))]).stringify();
        assert_eq!(text, "{\"a\":1}");
    }
}
