pub(crate) mod boxtree;
pub(crate) mod jsnum;
pub(crate) mod model;
pub(crate) mod pipeline;
pub(crate) mod progress;
pub(crate) mod tree;

use self::boxtree::{box_tree_path, render_box_trees, svg_cache_version, BOX_TREES};
use crate::compiler::canonical_json::canonical_json;
use crate::coverage::jsnum::{commas, number};
use crate::coverage::pipeline::{build_coverage_map, BuildOptions, CoverageMap};
use crate::coverage::progress::{game_done, measured, GameDone};
use crate::coverage::tree::{ref_tree, root, work_tree};
use serde_json::Value;
use std::path::{Path, PathBuf};
const USAGE: &str = "usage: alchemy check coverage [--target tbs-en|tla-en] [--exact-ref <ref>|worktree] [--recon-ref <ref>|worktree|none] [--write|--check|--assembly-spans|--self-test]";
fn get<'a>(v: &'a Value, key: &str) -> Option<&'a Value> {
    v.as_object()?.get(key)
}
fn field(v: &Value, path: &[&str]) -> f64 {
    path.iter()
        .try_fold(v, |node, key| get(node, key))
        .and_then(Value::as_f64)
        .unwrap_or(f64::NAN)
}
fn quote(s: &str) -> String {
    serde_json::to_string(s).unwrap_or_else(|_| "\"\"".into())
}
fn read(path: &Path) -> Result<String, String> {
    std::fs::read(path)
        .map(|b| String::from_utf8_lossy(&b).into_owned())
        .map_err(|e| format!("cannot read {}: {e}", path.display()))
}
fn write(path: &Path, text: &str) -> Result<(), String> {
    std::fs::write(path, text).map_err(|e| format!("cannot write {}: {e}", path.display()))
}
fn map_path(target: &str) -> PathBuf {
    root()
        .join("out")
        .join(target)
        .join("reports/coverage-map.json")
}
#[derive(Default)]
struct Options {
    target: String,
    exact: Option<String>,
    recon: Option<String>,
    write: bool,
    check: bool,
    assembly_spans: bool,
    self_test: bool,
    help: bool,
}
fn parse(argv: &[String]) -> Result<Options, String> {
    let mut o = Options {
        target: "tbs-en".into(),
        ..Options::default()
    };
    let mut i = 0;
    while i < argv.len() {
        match argv[i].as_str() {
            "--target" => {
                i += 1;
                o.target = match argv.get(i).map(String::as_str) {
                    Some("tbs-en") => "tbs-en".into(),
                    Some("tla-en") => "tla-en".into(),
                    Some(v) => {
                        return Err(format!(
                            "unsupported decomp target {}; expected tbs-en or tla-en",
                            quote(v)
                        ))
                    }
                    None => {
                        return Err(
                            "unsupported decomp target undefined; expected tbs-en or tla-en".into(),
                        )
                    }
                };
            }
            "--exact-ref" => {
                i += 1;
                o.exact = argv.get(i).cloned();
            }
            "--recon-ref" | "--semantic-ref" => {
                i += 1;
                o.recon = argv.get(i).cloned();
            }
            "--write" => o.write = true,
            "--check" => o.check = true,
            "--assembly-spans" => o.assembly_spans = true,
            "--self-test" => o.self_test = true,
            "-h" | "--help" => {
                o.help = true;
                break;
            }
            other => return Err(format!("unrecognized argument: {other}")),
        }
        i += 1;
    }
    Ok(o)
}
fn tracked(map: &Value) -> Value {
    let mut out = map.clone();
    if let Some(object) = out.as_object_mut() {
        for key in ["rom_areas", "executable_areas"] {
            if let Some(Value::Array(areas)) = object.get_mut(key) {
                for area in areas {
                    if let Some(a) = area.as_object_mut() {
                        let count = a.get("tiles").and_then(Value::as_array).map_or(0, Vec::len);
                        a.insert("tiles".into(), Value::Number((count as u64).into()));
                    }
                }
            }
        }
    }
    out
}
fn summary(doc: &Value) -> Result<String, String> {
    let executable = field(doc, &["executable_bytes"]);
    let proven_c = field(doc, &["categories", "proven_c", "bytes"]);
    let draft_c = field(doc, &["categories", "draft_c", "bytes"]);
    let proven_asm = field(doc, &["categories", "proven_asm", "bytes"]);
    if !executable.is_finite() || !proven_asm.is_finite() {
        return Err("coverage map lacks executable totals".into());
    }
    let done = crate::coverage::jsnum::done_bytes(proven_c as i64, proven_asm as i64);
    let percent =
        crate::coverage::jsnum::done_percent(proven_c as i64, proven_asm as i64, executable as i64);
    Ok(format!(
        "target={} rom={} executable={} proven_c={} ({}%) draft_c={} ({}%) proven_asm={} done={} ({}%) draft_source={}",
        get(doc, "target").and_then(Value::as_str).unwrap_or("undefined"),
        commas(field(doc, &["rom_bytes"]) as i64),
        commas(executable as i64),
        commas(proven_c as i64),
        number(field(doc, &["categories", "proven_c", "percent_of_executable"])),
        commas(draft_c as i64),
        number(field(doc, &["categories", "draft_c", "percent_of_executable"])),
        commas(proven_asm as i64),
        commas(done),
        number(percent),
        get(get(doc, "provenance").unwrap_or(&Value::Null), "draft_source").and_then(Value::as_str).unwrap_or("undefined")
    ))
}
fn readme_metrics(proven_c: f64, proven_asm: f64, executable: f64) -> String {
    let done = crate::coverage::jsnum::done_bytes(proven_c as i64, proven_asm as i64) as f64;
    let share = |bytes: f64| {
        if executable == 0.0 {
            0.0
        } else {
            bytes * 100.0 / executable
        }
    };
    format!(
        "|                    |       bytes |                   share |\n\
         | ------------------ | ----------: | ----------------------: |\n\
         | Proven C           | {:>11} | {:>8.1}% of executable |\n\
         | Proven ASM         | {:>11} | {:>8.1}% of executable |\n\
         | **DONE**           | **{:>7}** | **{:.1}% of executable** |",
        commas(proven_c as i64),
        share(proven_c),
        commas(proven_asm as i64),
        share(proven_asm),
        commas(done as i64),
        share(done)
    )
}
/// The README status line: ☀️ The Broken Seal and ⚓️ The Lost Age, each
/// pending until its executable audit gives it a denominator.
fn status_line(sun: Option<GameDone>, anchor: Option<GameDone>) -> String {
    let show = |done: Option<GameDone>| {
        done.map_or("pending".to_string(), |d| format!("{:.2}%", d.percent()))
    };
    format!("## Status: ☀️ {} · ⚓️ {}", show(sun), show(anchor))
}
fn update_readme(
    text: &str,
    target: &str,
    map: &CoverageMap,
    trees: &[(&'static str, String)],
    status: &str,
) -> String {
    let proven_c = field(&map.document, &["categories", "proven_c", "bytes"]);
    let proven_asm = field(&map.document, &["categories", "proven_asm", "bytes"]);
    let executable = field(&map.document, &["executable_bytes"]);
    let percent =
        crate::coverage::jsnum::done_percent(proven_c as i64, proven_asm as i64, executable as i64);
    let mut out = text.to_string();
    if let Some(start) = out.find("## Status:") {
        if let Some(end) = out[start..].find('\n') {
            out.replace_range(start..start + end, status);
        }
    }
    if let Some(end) = out.find("\n\nDONE measures") {
        if let Some(start) = out[..end].rfind("\n## DONE:") {
            let head_end = start + 1;
            let replacement = format!("## DONE: Currently {percent:.2}%");
            out.replace_range(head_end..end, &replacement);
        }
    }
    if let Some(start) = out.find("|                    |       bytes |                   share |")
    {
        if let Some(end) = out[start..].find("\n\nProven ASM") {
            out.replace_range(
                start..start + end,
                &readme_metrics(proven_c, proven_asm, executable),
            );
        }
    }
    if let Some(start) = out.find("**Proven C stands at ") {
        let value_start = start + "**Proven C stands at ".len();
        if let Some(end) = out[value_start..].find("%**") {
            let c_able = executable;
            let c_share = if c_able == 0.0 {
                0.0
            } else {
                proven_c * 100.0 / c_able
            };
            out.replace_range(value_start..value_start + end, &format!("{c_share:.1}"));
        }
    }
    for (id, svg) in trees {
        let version = svg_cache_version(svg);
        let needle = format!(
            "games/THE BROKEN SEAL/PREVIEW/{}",
            format!("{target}-{id}.svg").to_ascii_uppercase()
        );
        if let Some(pos) = out.find(&needle) {
            let end = pos + needle.len();
            let rest = &out[end..];
            let cut = rest.find(['>', ')']).unwrap_or(0);
            let replacement = format!("{needle}?v={version}");
            out.replace_range(pos..end + cut, &replacement);
        }
    }
    out
}
#[cfg(test)]
mod tests {
    use super::{readme_metrics, status_line, update_readme};
    use crate::coverage::pipeline::CoverageMap;
    use crate::coverage::progress::GameDone;
    use serde_json::json;
    #[test]
    fn readme_metrics_reports_all_done_categories() {
        assert_eq!(
            readme_metrics(282_436.0, 343_206.0, 1_347_122.0),
            "|                    |       bytes |                   share |\n\
             | ------------------ | ----------: | ----------------------: |\n\
             | Proven C           |     282,436 |     21.0% of executable |\n\
             | Proven ASM         |     343,206 |     25.5% of executable |\n\
             | **DONE**           | **625,642** | **46.4% of executable** |"
        );
    }

    #[test]
    fn readme_status_includes_exact_c_and_retained_assembly() {
        let map = CoverageMap {
            document: json!({
                "executable_bytes": 1000,
                "categories": {
                    "proven_c": {"bytes": 250},
                    "proven_asm": {"bytes": 340}
                }
            }),
            rom_areas: Vec::new(),
            executable_areas: Vec::new(),
        };
        let sun = GameDone {
            game_c: 250,
            game_asm: 340,
            executable: 1000,
            ..GameDone::default()
        };
        let status = status_line(Some(sun), None);
        assert_eq!(status, "## Status: ☀️ 59.00% · ⚓️ pending");
        let updated = update_readme(
            "# Alchemy\n\n## Status: 52% DONE\n\nDetails\n",
            "tbs-en",
            &map,
            &[],
            &status,
        );
        assert!(updated.contains("## Status: ☀️ 59.00% · ⚓️ pending"));
        assert!(!updated.contains("52% DONE"));
        let image = "![ROM contents](<games/THE BROKEN SEAL/PREVIEW/TBS-EN-ROM.SVG?v=old>)";
        let updated = update_readme(image, "tbs-en", &map, &[("rom", "<svg/>".into())], &status);
        assert!(updated.starts_with("![ROM contents](<games/THE BROKEN SEAL/"));
        assert!(updated.ends_with(">)"));
        assert!(!updated.contains("v=old"));
    }
}
fn run(argv: &[String]) -> Result<String, String> {
    let o = parse(argv)?;
    if o.help {
        return Ok(USAGE.into());
    }
    if o.self_test {
        return Ok("self-test=ok coverage-map".into());
    }
    let exact = match o.exact.as_deref() {
        None | Some("worktree") => work_tree(),
        Some(id) => {
            ref_tree(id).ok_or_else(|| format!("exact source ref {id} is not available here"))?
        }
    };
    let semantic = match o.recon.as_deref() {
        Some("none") => None,
        None | Some("worktree") => Some(work_tree()),
        Some(id) => Some(
            ref_tree(id)
                .ok_or_else(|| format!("reconstruction source ref {id} is not available here"))?,
        ),
    };
    let map = build_coverage_map(&BuildOptions {
        target: o.target.clone(),
        exact: &exact,
        recon: semantic.as_ref(),
    })?;
    if o.assembly_spans {
        let mut rows = map
            .executable_areas
            .iter()
            .flat_map(|area| &area.tiles)
            .filter(|tile| tile.categories[2] > 0)
            .collect::<Vec<_>>();
        rows.sort_by_key(|tile| std::cmp::Reverse(tile.categories[2]));
        return Ok(rows
            .into_iter()
            .map(|tile| format!("{}\t{}", tile.categories[2], tile.label))
            .collect::<Vec<_>>()
            .join("\n"));
    }
    let rendered = render_box_trees(&map);
    let map_json = canonical_json(&tracked(&map.document));
    let sun = if o.target == "tbs-en" {
        Some(game_done(&map)?)
    } else {
        measured(&root(), "tbs-en")?
    };
    let anchor = if o.target == "tla-en" {
        Some(game_done(&map)?)
    } else {
        measured(&root(), "tla-en")?
    };
    let status = status_line(sun, anchor);
    if o.check {
        for (id, svg) in &rendered {
            if read(&box_tree_path(&o.target, id))? != *svg {
                return Err(format!(
                    "tracked {id} coverage figure is stale; run: make coverage"
                ));
            }
        }
        let readme = read(&root().join("README.md"))?;
        if update_readme(&readme, &o.target, &map, &rendered, &status) != readme {
            return Err("README coverage values are stale; run: make coverage".into());
        }
        return Ok(format!("coverage-map=current {}", summary(&map.document)?));
    }
    if o.write {
        write(&map_path(&o.target), &map_json)?;
        for (id, svg) in &rendered {
            write(&box_tree_path(&o.target, id), svg)?;
        }
        let readme = read(&root().join("README.md"))?;
        write(
            &root().join("README.md"),
            &update_readme(&readme, &o.target, &map, &rendered, &status),
        )?;
        return Ok(format!(
            "map={} trees={} {}",
            map_path(&o.target).display(),
            BOX_TREES
                .iter()
                .map(|id| box_tree_path(&o.target, id).display().to_string())
                .collect::<Vec<_>>()
                .join(","),
            summary(&map.document)?
        ));
    }
    summary(&map.document)
}
pub fn entry(arguments: &[String]) {
    match run(arguments) {
        Ok(line) => println!("{line}"),
        Err(error) => {
            eprintln!("error: {error}");
            std::process::exit(1);
        }
    }
}
