// The command line, and the two refusals that guard publication.
//
// WHY: a redraw must be *stable*. The recorded source refs in the tracked map
// are reused unless explicitly overridden, so running the tool twice on the
// same tree cannot silently switch which sources it measured. The two refusal
// paths exist for the same reason: an exact ref that cannot be resolved is an
// error rather than a fallback (falling back would republish an older
// measurement as if it were current), and a tree that cannot resolve the
// recorded semantic source refuses to publish that coverage as zero.

use crate::json::{canonical_json, Value};
use crate::names::main_source;
use crate::pipeline::{build_coverage_map, BuildOptions, CoverageMap};
use crate::tree::{ref_tree, root, work_tree, SourceTree};

const USAGE: &str = "usage: coverage_map.ts [--target gs1-en|gs2-en] [--exact-ref <ref>|worktree] \
[--semantic-ref <ref>|worktree|none] [--write|--check|--self-test]";

#[derive(Debug, Default, PartialEq)]
pub struct Options {
    pub target: String,
    pub exact: Option<String>,
    pub semantic: Option<String>,
    pub write: bool,
    pub check: bool,
    pub self_test: bool,
    pub help: bool,
}

pub fn parse_target(value: &str) -> Result<String, String> {
    match value {
        "gs1-en" | "gs2-en" => Ok(value.to_string()),
        other => Err(format!(
            "unsupported decomp target {}; expected gs1-en or gs2-en",
            crate::json::quote(other)
        )),
    }
}

/// PORT NOTE: `argv[++index]` past the end of the array is `undefined` in
/// JavaScript, and `parseDecompTarget(undefined)` then reports
/// `unsupported decomp target undefined`. A missing value for `--exact-ref` or
/// `--semantic-ref` becomes `undefined`, which reads as "not requested". Both
/// behaviours are reproduced rather than turned into a distinct arity error.
pub fn options_of(argv: &[String]) -> Result<Options, String> {
    let mut options = Options {
        target: "gs1-en".to_string(),
        ..Options::default()
    };
    let mut index = 0;
    while index < argv.len() {
        let argument = argv[index].as_str();
        match argument {
            "--target" => {
                index += 1;
                let value = argv.get(index).cloned();
                options.target = match value {
                    Some(text) => parse_target(&text)?,
                    None => {
                        return Err(
                            "unsupported decomp target undefined; expected gs1-en or gs2-en"
                                .to_string(),
                        )
                    }
                };
            }
            "--exact-ref" => {
                index += 1;
                options.exact = argv.get(index).cloned();
            }
            "--semantic-ref" => {
                index += 1;
                options.semantic = argv.get(index).cloned();
            }
            "--write" => options.write = true,
            "--check" => options.check = true,
            "--self-test" => options.self_test = true,
            // PORT NOTE: the TypeScript prints the usage and calls
            // `process.exit(0)` *inside the parse loop*, so a bad argument
            // after `--help` is never reached and never reported. Breaking out
            // here reproduces that.
            "-h" | "--help" => {
                options.help = true;
                break;
            }
            other => return Err(format!("unrecognized argument: {other}")),
        }
        index += 1;
    }
    Ok(options)
}

/// A ref that cannot be resolved is an error, never a silent fallback.
pub fn resolve_exact_tree(
    requested: Option<&str>,
    recorded: Option<&str>,
) -> Result<SourceTree, String> {
    let wanted = match requested {
        Some(value) => Some(value.to_string()),
        None => recorded
            .filter(|value| *value != "worktree")
            .map(str::to_string),
    };
    match wanted.as_deref() {
        None | Some("worktree") => Ok(work_tree()),
        Some(value) => ref_tree(value).ok_or_else(|| {
            format!(
                "exact source ref {value} is not available here; run: git fetch origin {} \
                 (or --exact-ref worktree to draw this tree's own exact C)",
                value.strip_prefix("origin/").unwrap_or(value)
            )
        }),
    }
}

/// A recorded source wins over the describes-itself heuristic.
pub fn resolve_semantic_tree(
    exact: &SourceTree,
    requested: Option<&str>,
    recorded: Option<&str>,
) -> Result<Option<SourceTree>, String> {
    if requested == Some("none") {
        return Ok(None);
    }
    let wanted = match requested {
        Some(value) => Some(value.to_string()),
        None => recorded
            .filter(|value| !["worktree", "absent", "none"].contains(value))
            .map(str::to_string),
    };
    if let Some(value) = wanted {
        let tree = if value == "worktree" {
            Some(work_tree())
        } else {
            ref_tree(&value)
        };
        return tree
            .ok_or_else(|| format!("cannot resolve semantic source ref {value}"))
            .map(Some);
    }
    if exact
        .list("semantic")
        .iter()
        .any(|name| main_source(name).is_some())
    {
        return Ok(Some(exact.clone()));
    }
    Ok(None)
}

/// A tree that cannot resolve the recorded semantic source must not publish
/// that coverage as zero.
pub fn semantic_erase_refusal(
    resolved: bool,
    requested: Option<&str>,
    tracked: Option<&Value>,
) -> Option<String> {
    if resolved || requested == Some("none") {
        return None;
    }
    let bytes = tracked
        .and_then(|map| map.get("categories"))
        .and_then(|node| node.get("semantic_c"))
        .and_then(|node| node.get("bytes"))
        .and_then(|value| value.as_f64())
        .unwrap_or(0.0);
    // PORT NOTE: `undefined ?? 0` and a `NaN` reading both fall to the guard
    // below. `NaN <= 0` is false in JavaScript, so a non-numeric tracked figure
    // would produce a refusal naming `NaN`; the check is written to match.
    if !js_greater_than_zero(bytes) {
        return None;
    }
    let source = tracked
        .and_then(|map| map.get("provenance"))
        .and_then(|node| node.get("semantic_source"))
        .and_then(|value| value.as_str())
        .unwrap_or("undefined");
    Some(format!(
        "refusing to erase semantic coverage: the tracked map records {} semantic bytes from {}, \
         which is not available here; fetch that ref or pass --semantic-ref none to publish \
         without it",
        crate::jsnum::js_number_string(bytes),
        source
    ))
}

/// JavaScript's `value > 0`, kept as a named helper.
///
/// PORT NOTE: clippy wants `!(x > 0.0)` rewritten as `x <= 0.0`, which is a
/// behaviour change: for `NaN` the first is true and the second is false. The
/// tracked figure can read `NaN`, so the distinction is live.
fn js_greater_than_zero(value: f64) -> bool {
    value > 0.0
}

fn field(document: &Value, path: &[&str]) -> f64 {
    let mut node = document;
    for key in path {
        match node.get(key) {
            Some(next) => node = next,
            None => return f64::NAN,
        }
    }
    node.as_f64().unwrap_or(f64::NAN)
}

pub fn summarize(document: &Value) -> Result<String, String> {
    let exact = field(document, &["categories", "exact_c", "bytes"]);
    let semantic = field(document, &["categories", "semantic_c", "bytes"]);
    let executable = field(document, &["executable_bytes"]);
    let combined = exact + semantic;
    Ok([
        format!(
            "target={}",
            document
                .get("target")
                .and_then(|v| v.as_str())
                .unwrap_or("undefined")
        ),
        format!(
            "rom={}",
            crate::js::commas(field(document, &["rom_bytes"]) as i64)
        ),
        format!("executable={}", crate::js::commas(executable as i64)),
        format!(
            "exact={} ({}%)",
            crate::js::commas(exact as i64),
            crate::jsnum::js_number_string(field(
                document,
                &["categories", "exact_c", "percent_of_executable"]
            ))
        ),
        format!(
            "semantic={} ({}%)",
            crate::js::commas(semantic as i64),
            crate::jsnum::js_number_string(field(
                document,
                &["categories", "semantic_c", "percent_of_executable"]
            ))
        ),
        format!(
            "combined={} ({}%)",
            crate::js::commas(combined as i64),
            crate::jsnum::js_number_string(crate::intervals::round_half_up_percent(
                combined, executable
            )?)
        ),
        format!(
            "semantic_source={}",
            document
                .get("provenance")
                .and_then(|node| node.get("semantic_source"))
                .and_then(|v| v.as_str())
                .unwrap_or("undefined")
        ),
    ]
    .join(" "))
}

/// The tracked shape: areas keep their totals but only a tile *count*.
pub fn tracked_document(map: &CoverageMap) -> Value {
    let Value::Obj(fields) = &map.document else {
        return map.document.clone();
    };
    let summarize_area = |area: &Value| -> Value {
        let tiles = area
            .get("tiles")
            .and_then(|v| v.as_array())
            .map(|list| list.len())
            .unwrap_or(0);
        Value::Obj(vec![
            ("id".into(), area.get("id").cloned().unwrap_or(Value::Null)),
            (
                "label".into(),
                area.get("label").cloned().unwrap_or(Value::Null),
            ),
            (
                "bytes".into(),
                area.get("bytes").cloned().unwrap_or(Value::Null),
            ),
            (
                "categories".into(),
                area.get("categories").cloned().unwrap_or(Value::Null),
            ),
            ("tiles".into(), Value::Num(tiles as f64)),
        ])
    };
    // PORT NOTE: the object spread rewrites `rom_areas` and `executable_areas`
    // in place, keeping their original key positions, because a spread followed
    // by an existing key overwrites the value without moving the key.
    let rewritten: Vec<(String, Value)> = fields
        .iter()
        .map(|(key, value)| match (key.as_str(), value) {
            ("rom_areas" | "executable_areas", Value::Arr(areas)) => (
                key.clone(),
                Value::Arr(areas.iter().map(&summarize_area).collect()),
            ),
            _ => (key.clone(), value.clone()),
        })
        .collect();
    Value::Obj(rewritten)
}

fn map_path(target: &str) -> std::path::PathBuf {
    root()
        .join("metrics")
        .join(format!("{target}-coverage-map.json"))
}

pub fn run(argv: &[String]) -> Result<String, String> {
    let options = options_of(argv)?;
    if options.help {
        return Ok(USAGE.to_string());
    }
    if options.self_test {
        return crate::selftest::self_test();
    }

    let tracked_on_disk = std::fs::read_to_string(map_path(&options.target))
        .ok()
        .and_then(|text| crate::json::parse(&text).ok());
    let recorded_exact = tracked_on_disk
        .as_ref()
        .and_then(|map| map.get("provenance"))
        .and_then(|node| node.get("exact_source"))
        .and_then(|value| value.as_str())
        .map(str::to_string);
    let exact = resolve_exact_tree(options.exact.as_deref(), recorded_exact.as_deref())?;
    let recorded_semantic = tracked_on_disk
        .as_ref()
        .and_then(|map| map.get("provenance"))
        .and_then(|node| node.get("semantic_source"))
        .and_then(|value| value.as_str())
        .map(str::to_string);
    let semantic = resolve_semantic_tree(
        &exact,
        options.semantic.as_deref(),
        recorded_semantic.as_deref(),
    )?;

    let map = build_coverage_map(&BuildOptions {
        target: options.target.clone(),
        exact: &exact,
        semantic: semantic.as_ref(),
        validate_tracked_progress: true,
        prefer_verified_assets: true,
    })?;
    let trees = crate::boxtree::render_box_trees(&map, Some(&exact), true)?;
    let json = canonical_json(&tracked_document(&map));

    if options.check {
        return check(&options, &map, &trees, &json, semantic.as_ref());
    }
    if options.write {
        return write(&options, &map, &trees, &json, semantic.is_some());
    }
    summarize(&map.document)
}

fn tree_text(trees: &[(&'static str, String)], name: &str) -> String {
    trees
        .iter()
        .find(|(id, _)| *id == name)
        .map(|(_, text)| text.clone())
        .unwrap_or_default()
}

/// JavaScript's `!==` on two numbers.
///
/// PORT NOTE: `field` yields `NaN` for an absent key, and `NaN !== NaN` is
/// `true` in JavaScript exactly as `!=` is in Rust, so a tracked map missing
/// the key reads as stale in both. The one divergence -- both sides absent --
/// cannot arise, because the right-hand side is the document this run just
/// built and always carries the key.
fn js_not_equal(left: f64, right: f64) -> bool {
    left != right
}

fn read_text(path: &std::path::Path) -> Result<String, String> {
    std::fs::read(path)
        .map(|bytes| String::from_utf8_lossy(&bytes).into_owned())
        .map_err(|error| format!("cannot read {}: {error}", path.display()))
}

fn expected_readme(
    target: &str,
    trees: &[(&'static str, String)],
    map: &CoverageMap,
    readme: &str,
) -> Result<String, String> {
    let versions: Vec<(&'static str, String)> = crate::boxtree::BOX_TREES
        .iter()
        .map(|tree| {
            (
                *tree,
                crate::boxtree::svg_cache_version(&tree_text(trees, tree)),
            )
        })
        .collect();
    let busted = crate::boxtree::readme_with_cache_buster(readme, target, &versions);
    crate::boxtree::readme_with_done_headline(&busted, map)
}

fn check(
    options: &Options,
    map: &CoverageMap,
    trees: &[(&'static str, String)],
    json: &str,
    semantic: Option<&SourceTree>,
) -> Result<String, String> {
    let tracked_map = read_text(&map_path(&options.target))?;
    let tracked_trees: Vec<(&'static str, String)> = crate::boxtree::BOX_TREES
        .iter()
        .map(|tree| {
            read_text(&crate::boxtree::box_tree_path(&options.target, tree))
                .map(|text| (*tree, text))
        })
        .collect::<Result<_, _>>()?;
    let tracked = crate::json::parse(&tracked_map)?;

    let mut stale: Vec<&str> = Vec::new();
    if js_not_equal(
        field(&tracked, &["categories", "exact_c", "bytes"]),
        field(&map.document, &["categories", "exact_c", "bytes"]),
    ) || js_not_equal(
        field(&tracked, &["main", "exact_c_bytes"]),
        field(&map.document, &["main", "exact_c_bytes"]),
    ) || js_not_equal(
        field(&tracked, &["overlays", "exact_c_bytes"]),
        field(&map.document, &["overlays", "exact_c_bytes"]),
    ) {
        stale.push("exact coverage");
    }
    // PORT NOTE: the guard reads the *resolved tree*, not the document's
    // recorded provenance. When no semantic tree resolved, `semantic?.id` is
    // `undefined`, which is never equal to the tracked string -- even to the
    // literal "none" the document records -- so an unresolved run can never
    // report the rendered map stale.
    let semantic_id = semantic.map(|tree| tree.id());
    if semantic_id == Some("worktree")
        && js_not_equal(
            field(&tracked, &["categories", "semantic_c", "bytes"]),
            field(&map.document, &["categories", "semantic_c", "bytes"]),
        )
    {
        stale.push("semantic coverage");
    }
    if js_not_equal(
        field(&tracked, &["executable_bytes"]),
        field(&map.document, &["executable_bytes"]),
    ) {
        stale.push("executable denominator");
    }
    let tracked_semantic_source = tracked
        .get("provenance")
        .and_then(|node| node.get("semantic_source"))
        .and_then(|value| value.as_str());
    let rendered_differs = tracked_map != json
        || crate::boxtree::BOX_TREES
            .iter()
            .any(|tree| tree_text(&tracked_trees, tree) != tree_text(trees, tree));
    if semantic_id == tracked_semantic_source && rendered_differs && stale.is_empty() {
        stale.push("rendered map");
    }

    let readme = read_text(&crate::boxtree::readme_path())?;
    // PORT NOTE: the expected README is busted with the *tracked* SVG bytes,
    // not the freshly rendered ones, so a redraw that changes only the SVG is
    // caught by the "rendered map" arm above rather than by this one.
    if expected_readme(&options.target, &tracked_trees, map, &readme)? != readme {
        stale.push("README DONE headline or cache-buster");
    }

    if !stale.is_empty() {
        return Err(format!(
            "tracked coverage map is stale ({}); run: make coverage",
            stale.join(", ")
        ));
    }
    Ok(format!(
        "coverage-map=current {}",
        summarize(&map.document)?
    ))
}

fn write(
    options: &Options,
    map: &CoverageMap,
    trees: &[(&'static str, String)],
    json: &str,
    semantic_resolved: bool,
) -> Result<String, String> {
    let tracked_on_disk = std::fs::read_to_string(map_path(&options.target))
        .ok()
        .and_then(|text| crate::json::parse(&text).ok());
    if let Some(refusal) = semantic_erase_refusal(
        semantic_resolved,
        options.semantic.as_deref(),
        tracked_on_disk.as_ref(),
    ) {
        return Err(refusal);
    }
    let map_file = map_path(&options.target);
    std::fs::write(&map_file, json)
        .map_err(|error| format!("cannot write {}: {error}", map_file.display()))?;
    for tree in crate::boxtree::BOX_TREES.iter() {
        let path = crate::boxtree::box_tree_path(&options.target, tree);
        std::fs::write(&path, tree_text(trees, tree))
            .map_err(|error| format!("cannot write {}: {error}", path.display()))?;
    }
    let readme_file = crate::boxtree::readme_path();
    let readme = read_text(&readme_file)?;
    let busted = expected_readme(&options.target, trees, map, &readme)?;
    if busted != readme {
        std::fs::write(&readme_file, &busted)
            .map_err(|error| format!("cannot write {}: {error}", readme_file.display()))?;
    }
    // `path.slice(ROOT.length + 1)` -- ROOT-relative, with the separator eaten.
    let relative = |path: std::path::PathBuf| -> String {
        path.strip_prefix(root())
            .map(|rest| rest.display().to_string())
            .unwrap_or_else(|_| path.display().to_string())
    };
    let tree_paths: Vec<String> = crate::boxtree::BOX_TREES
        .iter()
        .map(|tree| relative(crate::boxtree::box_tree_path(&options.target, tree)))
        .collect();
    Ok(format!(
        "map={} trees={} {}",
        relative(map_file),
        tree_paths.join(","),
        summarize(&map.document)?
    ))
}

/// `canonicalJson(trackedDocument(map))`, exposed for the parity harness.
pub fn tracked_json(map: &CoverageMap) -> String {
    canonical_json(&tracked_document(map))
}

#[cfg(test)]
mod tests {
    use super::*;

    fn argv(items: &[&str]) -> Vec<String> {
        items.iter().map(|item| item.to_string()).collect()
    }

    #[test]
    fn options_parse_every_flag() {
        let parsed = options_of(&argv(&[
            "--target",
            "gs2-en",
            "--exact-ref",
            "origin/main",
            "--semantic-ref",
            "none",
            "--write",
            "--check",
            "--self-test",
        ]))
        .expect("valid");
        assert_eq!(parsed.target, "gs2-en");
        assert_eq!(parsed.exact.as_deref(), Some("origin/main"));
        assert_eq!(parsed.semantic.as_deref(), Some("none"));
        assert!(parsed.write && parsed.check && parsed.self_test);
        assert_eq!(options_of(&argv(&[])).expect("empty").target, "gs1-en");
        assert!(options_of(&argv(&["-h"])).expect("help").help);
        let error = options_of(&argv(&["--nope"])).expect_err("rejected");
        assert_eq!(error, "unrecognized argument: --nope");
        let missing = options_of(&argv(&["--target"])).expect_err("rejected");
        assert!(
            missing.contains("undefined"),
            "a missing value reads as undefined: {missing}"
        );
        let bad = options_of(&argv(&["--target", "gs3"])).expect_err("rejected");
        assert!(
            bad.contains("\"gs3\""),
            "the target is quoted like JSON.stringify: {bad}"
        );
    }

    #[test]
    fn a_trailing_ref_flag_reads_as_not_requested() {
        let parsed = options_of(&argv(&["--exact-ref"])).expect("valid");
        assert_eq!(
            parsed.exact, None,
            "argv past the end is undefined, not an error"
        );
    }

    #[test]
    fn help_short_circuits_before_a_bad_argument() {
        assert!(
            options_of(&argv(&["--help", "--nope"]))
                .expect("help wins")
                .help
        );
        assert!(
            options_of(&argv(&["--nope", "--help"])).is_err(),
            "order still matters"
        );
        assert_eq!(run(&argv(&["--help"])).expect("usage"), USAGE);
    }

    #[test]
    fn semantic_erase_refusal_only_fires_when_coverage_would_vanish() {
        let tracked = crate::json::parse(
            r#"{"categories":{"semantic_c":{"bytes":40}},"provenance":{"semantic_source":"origin/x"}}"#,
        )
        .expect("fixture");
        assert!(
            semantic_erase_refusal(true, None, Some(&tracked)).is_none(),
            "resolved"
        );
        assert!(
            semantic_erase_refusal(false, Some("none"), Some(&tracked)).is_none(),
            "opted out"
        );
        let refusal = semantic_erase_refusal(false, None, Some(&tracked)).expect("refused");
        assert!(
            refusal.contains("records 40 semantic bytes from origin/x"),
            "{refusal}"
        );
        let empty =
            crate::json::parse(r#"{"categories":{"semantic_c":{"bytes":0}}}"#).expect("fixture");
        assert!(
            semantic_erase_refusal(false, None, Some(&empty)).is_none(),
            "nothing to erase"
        );
        assert!(
            semantic_erase_refusal(false, None, None).is_none(),
            "no tracked map"
        );
    }

    #[test]
    fn resolve_exact_tree_errors_rather_than_falling_back() {
        let error =
            resolve_exact_tree(Some("origin/does-not-exist-xyz"), None).expect_err("unresolvable");
        assert!(
            error.contains("git fetch origin does-not-exist-xyz"),
            "{error}"
        );
        assert_eq!(
            resolve_exact_tree(None, None).expect("worktree").id(),
            "worktree"
        );
        assert_eq!(
            resolve_exact_tree(None, Some("worktree"))
                .expect("worktree")
                .id(),
            "worktree",
            "a recorded worktree is not treated as a ref"
        );
    }

    #[test]
    fn summarize_formats_counts_with_separators() {
        let document = crate::json::parse(
            r#"{"target":"gs1-en","rom_bytes":8388608,"executable_bytes":1000,
                "categories":{"exact_c":{"bytes":250,"percent_of_executable":25},
                              "semantic_c":{"bytes":100,"percent_of_executable":10}},
                "provenance":{"semantic_source":"worktree"}}"#,
        )
        .expect("fixture");
        let line = summarize(&document).expect("summary");
        assert_eq!(
            line,
            "target=gs1-en rom=8,388,608 executable=1,000 exact=250 (25%) semantic=100 (10%) \
             combined=350 (35%) semantic_source=worktree"
        );
    }

    #[test]
    fn tracked_document_replaces_tiles_with_a_count_in_place() {
        let map = CoverageMap {
            document: crate::json::parse(
                r#"{"format":1,"rom_areas":[{"id":"a","label":"A","bytes":1,
                    "categories":{},"tiles":[{},{}]}],"executable_areas":[],"target":"gs1-en"}"#,
            )
            .expect("fixture"),
            rom_areas: Vec::new(),
            executable_areas: Vec::new(),
        };
        let tracked = tracked_document(&map);
        let Value::Obj(fields) = &tracked else {
            panic!("object")
        };
        let keys: Vec<&str> = fields.iter().map(|(key, _)| key.as_str()).collect();
        assert_eq!(
            keys,
            vec!["format", "rom_areas", "executable_areas", "target"],
            "the spread overwrites in place and never moves a key"
        );
        let areas = tracked
            .get("rom_areas")
            .and_then(|v| v.as_array())
            .expect("areas");
        assert_eq!(areas[0].get("tiles").and_then(|v| v.as_f64()), Some(2.0));
    }
}

#[cfg(test)]
mod real_tree_tests {
    use super::*;

    /// Every number that reaches the output must stringify the way JavaScript
    /// would. The document is *not* integers-only -- percentages are
    /// fractional -- so the claim being pinned is that `json::number` and the
    /// ECMAScript `ToString(Number)` implementation agree on every value the
    /// real map emits, and that at least one fractional value was seen (a
    /// walk that found none would pass vacuously).
    #[test]
    fn every_emitted_number_stringifies_like_javascript() {
        let exact = work_tree();
        let map = build_coverage_map(&BuildOptions {
            target: "gs1-en".to_string(),
            exact: &exact,
            semantic: Some(&exact),
            validate_tracked_progress: true,
            prefer_verified_assets: true,
        })
        .expect("map");
        let mut checked = 0usize;
        let mut fractional = 0usize;
        walk(&map.document, &mut checked, &mut fractional);
        assert!(
            checked > 1000,
            "only {checked} numbers walked; the walk is not reaching the tiles"
        );
        assert!(
            fractional > 0,
            "no fractional value seen, so the check would be vacuous"
        );
    }

    fn walk(value: &Value, checked: &mut usize, fractional: &mut usize) {
        match value {
            Value::Num(number) => {
                *checked += 1;
                if number.fract() != 0.0 {
                    *fractional += 1;
                }
                assert_eq!(
                    crate::json::number(*number),
                    crate::jsnum::js_number_string(*number),
                    "ToString(Number) disagreement for {number}"
                );
            }
            Value::Arr(items) => {
                for item in items {
                    walk(item, checked, fractional);
                }
            }
            Value::Obj(members) => {
                for (_, item) in members {
                    walk(item, checked, fractional);
                }
            }
            _ => {}
        }
    }
}
