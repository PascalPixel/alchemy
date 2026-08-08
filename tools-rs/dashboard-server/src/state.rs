// Live coverage state: `mtime`, `pageVersion`, `snapshot` and
// `rebuildCoverage` from `dashboard_server.ts`.

use std::path::Path;
use std::sync::Mutex;

use coverage_map::boxtree::{render_box_trees, svg_cache_version, BOX_TREES};
use coverage_map::jsnum::js_number_string;
use coverage_map::pipeline::{build_coverage_map, BuildOptions, CoverageMap};
use coverage_map::tree::work_tree_at;

use crate::jsonout::Json;
use crate::paths;

/// `TREE_LABELS`, in the source's key order. A `Vec` and never a `HashMap`:
/// this is serialised into `/snapshot` and the client renders one panel per
/// entry in iteration order.
pub const TREE_LABELS: [(&str, &str); 4] = [
    ("core", "Main image"),
    ("overlays", "Code overlays"),
    ("images", "Images"),
    ("music", "Music"),
];

pub struct LiveCoverage {
    pub revision: String,
    pub generated_at: String,
    pub trees: Vec<(&'static str, String)>,
    pub executable_bytes: f64,
    pub exact_bytes: f64,
    pub exact_percent: f64,
    pub semantic_bytes: f64,
}

#[derive(Default)]
pub struct State {
    pub coverage: Option<LiveCoverage>,
    pub scan_error: Option<String>,
    pub scanning: bool,
    pub scan_queued: bool,
}

pub static STATE: Mutex<Option<State>> = Mutex::new(None);

pub fn with_state<R>(body: impl FnOnce(&mut State) -> R) -> R {
    let mut guard = STATE.lock().unwrap_or_else(|poisoned| poisoned.into_inner());
    body(guard.get_or_insert_with(State::default))
}

/// `existsSync(file) ? statSync(file).mtimeMs : 0`.
///
/// PORT NOTE: Node's `mtimeMs` is `seconds * 1000 + nanoseconds / 1e6` in
/// f64, not `as_secs_f64() * 1000.0`, which rounds differently in the low
/// digits and would change the `?v=` cache buster the document shell emits.
pub fn mtime(file: &Path) -> f64 {
    let Ok(metadata) = std::fs::metadata(file) else { return 0.0 };
    let Ok(modified) = metadata.modified() else { return 0.0 };
    let Ok(since) = modified.duration_since(std::time::UNIX_EPOCH) else { return 0.0 };
    (since.as_secs() as f64) * 1000.0 + f64::from(since.subsec_nanos()) / 1e6
}

/// `PAGE_FILES.map(mtime).join(":")`.
///
/// PORT NOTE: `Array#join` stringifies with `ToString(Number)`, not with
/// Rust's `Display`. `js_number_string` is the only formatter used.
pub fn page_version() -> String {
    paths::page_files()
        .iter()
        .map(|file| js_number_string(mtime(file)))
        .collect::<Vec<_>>()
        .join(":")
}

fn map_number(map: &CoverageMap, path: &[&str]) -> Option<f64> {
    let mut node = &map.document;
    for key in path {
        node = node.get(key)?;
    }
    node.as_f64()
}

/// `map?.categories.exact_c.bytes ?? 0`.
///
/// PORT NOTE: `??` falls back only for `null`/`undefined`. A present `NaN`
/// passes straight through and poisons the `exact + semantic` sum. Modelled
/// exactly: a missing key becomes 0, a present NaN stays NaN.
fn nullish_or_zero(value: Option<f64>) -> f64 {
    value.unwrap_or(0.0)
}

pub fn rebuild_coverage() {
    let already = with_state(|state| {
        if state.scanning {
            state.scan_queued = true;
            return true;
        }
        state.scanning = true;
        false
    });
    if already {
        return;
    }
    crate::events::notify();

    let outcome = compute();
    let queued = with_state(|state| {
        match outcome {
            Ok(coverage) => {
                state.coverage = Some(coverage);
                state.scan_error = None;
            }
            Err(message) => state.scan_error = Some(message),
        }
        state.scanning = false;
        let queued = state.scan_queued;
        state.scan_queued = false;
        queued
    });
    crate::events::notify();
    if queued {
        rebuild_coverage();
    }
}

pub fn compute() -> Result<LiveCoverage, String> {
    let tree = work_tree_at(paths::root());
    let map = build_coverage_map(&BuildOptions {
        target: "gs1-en".to_string(),
        exact: &tree,
        semantic: Some(&tree),
        validate_tracked_progress: false,
        prefer_verified_assets: true,
    })?;
    let trees = render_box_trees(&map, Some(&tree), true)?;
    let revision = BOX_TREES
        .iter()
        .map(|name| {
            let svg = trees.iter().find(|(id, _)| id == name).map(|(_, svg)| svg.as_str());
            svg_cache_version(svg.unwrap_or(""))
        })
        .collect::<Vec<_>>()
        .join("-");
    Ok(LiveCoverage {
        revision,
        generated_at: crate::clock::iso_now(),
        executable_bytes: nullish_or_zero(map_number(&map, &["executable_bytes"])),
        exact_bytes: nullish_or_zero(map_number(&map, &["categories", "exact_c", "bytes"])),
        exact_percent: nullish_or_zero(map_number(
            &map,
            &["categories", "exact_c", "percent_of_executable"],
        )),
        semantic_bytes: nullish_or_zero(map_number(&map, &["categories", "semantic_c", "bytes"])),
        trees,
    })
}

/// `snapshot()`. Key order is the object-literal order in the source and is
/// load-bearing for the compared bytes.
pub fn snapshot() -> Json {
    with_state(|state| snapshot_of(state))
}

pub fn snapshot_of(state: &State) -> Json {
    let coverage = state.coverage.as_ref();
    let labels = Json::Obj(
        TREE_LABELS.iter().map(|(id, label)| ((*id).to_string(), Json::str(label))).collect(),
    );
    let summary = match coverage {
        None => Json::Undefined,
        Some(live) => {
            let exact = live.exact_bytes;
            let semantic = live.semantic_bytes;
            Json::obj(vec![
                ("executableBytes", Json::Num(live.executable_bytes)),
                ("exactBytes", Json::Num(exact)),
                ("exactPercent", Json::Num(live.exact_percent)),
                ("semanticBytes", Json::Num(semantic)),
                // PORT NOTE: JS `+` on two numbers. If either side were NaN
                // the sum is NaN and stringifies as `null`; f64 addition
                // already models that, so no `js_add` helper is needed here.
                ("combinedBytes", Json::Num(exact + semantic)),
            ])
        }
    };
    Json::obj(vec![
        ("page", Json::Str(page_version())),
        (
            "revision",
            Json::Str(coverage.map_or_else(|| "starting".to_string(), |c| c.revision.clone())),
        ),
        (
            "generatedAt",
            coverage.map_or(Json::Undefined, |c| Json::Str(c.generated_at.clone())),
        ),
        ("scanning", Json::Bool(state.scanning)),
        ("error", state.scan_error.clone().map_or(Json::Undefined, Json::Str)),
        ("trees", labels),
        ("summary", summary),
    ])
}

/// `affectsCoverage`, exported by the TypeScript and exercised by its
/// self-test.
///
/// PORT NOTE (BUG, reproduced): the alternation is `asm|assets|metrics|
/// semantic|src` while the directories actually watched are `asm, assets,
/// metrics, semantic, exact`. `src/` is claimed and not watched, `exact/` is
/// watched and not claimed. The function has no caller outside the self-test,
/// so the mismatch is inert today; it is reproduced rather than fixed.
///
/// PORT NOTE: the regex `/^(?:asm|assets|metrics|semantic|src)\//` carries NO
/// `i` flag, so `ASM/x.s` must NOT match. A case-insensitive port passes a
/// naive smoke test and fails the pinned test below.
pub fn affects_coverage(path: &str) -> bool {
    let normalized = path.replace('\\', "/");
    // `.replace(/^\.\//, "")` -- anchored, non-global, so exactly one leading
    // "./" is removed and "././x" keeps its second one.
    let normalized = normalized.strip_prefix("./").unwrap_or(&normalized);
    ["asm/", "assets/", "metrics/", "semantic/", "src/"]
        .iter()
        .any(|prefix| normalized.starts_with(prefix))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn coverage_paths_match_the_typescript_self_test_triple() {
        assert!(affects_coverage("assets/code/resource_373_overlay.s"));
        assert!(affects_coverage("semantic/example.c"));
        assert!(!affects_coverage("tools/metrics/dashboard/styles.css"));
    }

    #[test]
    fn the_pattern_has_no_ignore_case_flag() {
        assert!(!affects_coverage("ASM/main.s"));
        assert!(!affects_coverage("Assets/x.s"));
        assert!(affects_coverage("asm/main.s"));
    }

    #[test]
    fn only_one_leading_dot_slash_is_stripped() {
        assert!(affects_coverage("./asm/main.s"));
        assert!(!affects_coverage(".././asm/main.s"));
        assert!(!affects_coverage("././asm/main.s"));
    }

    #[test]
    fn backslashes_normalise_to_forward_slashes_everywhere_not_just_the_first() {
        // `replaceAll`, not `replace`.
        assert!(affects_coverage("asm\\sub\\main.s"));
        assert!(affects_coverage(".\\asm\\main.s".replace('\\', "/").as_str()));
    }

    #[test]
    fn the_watched_directories_and_the_predicate_disagree_as_in_the_source() {
        // Pinning the reproduced bug so a "tidy-up" that aligns the two lists
        // fails here and has to be a deliberate decision.
        assert!(paths::COVERAGE_DIRECTORIES.contains(&"exact"));
        assert!(!affects_coverage("exact/thing.c"));
        assert!(!paths::COVERAGE_DIRECTORIES.contains(&"src"));
        assert!(affects_coverage("src/thing.c"));
    }

    #[test]
    fn a_missing_file_has_mtime_zero_and_stringifies_as_zero() {
        assert_eq!(mtime(Path::new("/nonexistent/dashboard/file")), 0.0);
        assert_eq!(js_number_string(0.0), "0");
    }

    #[test]
    fn page_version_is_two_colon_separated_javascript_numbers() {
        let version = page_version();
        let parts: Vec<&str> = version.split(':').collect();
        assert_eq!(parts.len(), 2, "PAGE_FILES has exactly two entries: {version}");
        for part in parts {
            assert!(!part.contains("e"), "an mtime must not reach exponential form: {part}");
            assert!(!part.ends_with(".0"), "Rust's `1.0` would leak here: {part}");
        }
    }

    #[test]
    fn a_starting_snapshot_omits_generated_at_and_summary() {
        let state = State::default();
        let text = snapshot_of(&state).stringify();
        assert!(text.contains(r#""revision":"starting""#));
        assert!(!text.contains("generatedAt"), "{text}");
        assert!(!text.contains("summary"), "{text}");
        assert!(!text.contains("error"), "{text}");
        // Key order survives.
        let page = text.find("\"page\"").unwrap();
        let scanning = text.find("\"scanning\"").unwrap();
        let trees = text.find("\"trees\"").unwrap();
        assert!(page < scanning && scanning < trees);
    }

    #[test]
    fn an_error_snapshot_carries_the_message_before_the_trees() {
        let state = State { scan_error: Some("boom".to_string()), ..State::default() };
        let text = snapshot_of(&state).stringify();
        assert!(text.contains(r#""error":"boom","trees":"#), "{text}");
    }
}
