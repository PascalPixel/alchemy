use crate::boxtree::{box_tree_path, render_box_trees, svg_cache_version, BOX_TREES};
use crate::jsnum::{commas, number};
use crate::pipeline::{build_coverage_map, BuildOptions, CoverageMap};
use crate::tree::{ref_tree, root, work_tree};
use canonical_json::canonical_json;
use serde_json::Value;
use std::path::{Path, PathBuf};
const USAGE: &str = "usage: coverage-map [--target gs1-en|gs2-en] [--exact-ref <ref>|worktree] [--recon-ref <ref>|worktree|none] [--write|--check|--self-test]";
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
    self_test: bool,
    help: bool,
}
fn parse(argv: &[String]) -> Result<Options, String> {
    let mut o = Options {
        target: "gs1-en".into(),
        ..Options::default()
    };
    let mut i = 0;
    while i < argv.len() {
        match argv[i].as_str() {
            "--target" => {
                i += 1;
                o.target = match argv.get(i).map(String::as_str) {
                    Some("gs1-en") => "gs1-en".into(),
                    Some("gs2-en") => "gs2-en".into(),
                    Some(v) => {
                        return Err(format!(
                            "unsupported decomp target {}; expected gs1-en or gs2-en",
                            quote(v)
                        ))
                    }
                    None => {
                        return Err(
                            "unsupported decomp target undefined; expected gs1-en or gs2-en".into(),
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
    let exact = field(doc, &["categories", "exact_c", "bytes"]);
    let semantic = field(doc, &["categories", "tracked_c", "bytes"]);
    let retained = field(doc, &["categories", "retained_asm", "bytes"]);
    if !executable.is_finite() || !retained.is_finite() {
        return Err("coverage map lacks executable totals".into());
    }
    let ceiling = executable - retained;
    let percent = crate::jsnum::round_half_up(exact as i64, ceiling as i64);
    Ok(format!(
        "target={} rom={} executable={} exact={} ({}%) tracked={} ({}%) c_able={} exact_of_c_able={}% tracked_source={}",
        get(doc, "target").and_then(Value::as_str).unwrap_or("undefined"),
        commas(field(doc, &["rom_bytes"]) as i64),
        commas(executable as i64),
        commas(exact as i64),
        number(field(doc, &["categories", "exact_c", "percent_of_executable"])),
        commas(semantic as i64),
        number(field(doc, &["categories", "tracked_c", "percent_of_executable"])),
        commas(ceiling as i64),
        number(percent),
        get(get(doc, "provenance").unwrap_or(&Value::Null), "tracked_source").and_then(Value::as_str).unwrap_or("undefined")
    ))
}
fn readme_metrics(exact: f64, retained: f64, executable: f64) -> String {
    let done = exact + retained;
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
         | Exact C            | {:>11} | {:>8.1}% of executable |\n\
         | Permanent assembly | {:>11} | {:>8.1}% of executable |\n\
         | **DONE**           | **{:>7}** | **{:.1}% of executable** |",
        commas(exact as i64),
        share(exact),
        commas(retained as i64),
        share(retained),
        commas(done as i64),
        share(done)
    )
}
fn update_readme(
    text: &str,
    target: &str,
    map: &CoverageMap,
    trees: &[(&'static str, String)],
) -> String {
    let exact = field(&map.document, &["categories", "exact_c", "bytes"]);
    let retained = field(&map.document, &["categories", "retained_asm", "bytes"]);
    let executable = field(&map.document, &["executable_bytes"]);
    let done = exact + retained;
    let percent = if executable == 0.0 {
        0.0
    } else {
        done * 100.0 / executable
    };
    let mut out = text.to_string();
    if let Some(end) = out.find("\n\nDONE measures") {
        if let Some(start) = out[..end].rfind("\n## DONE:") {
            let head_end = start + 1;
            let replacement = format!("## DONE: Currently {}%", number(percent.round()));
            out.replace_range(head_end..end, &replacement);
        }
    }
    if let Some(start) = out.find("|                    |       bytes |                   share |")
    {
        if let Some(end) = out[start..].find("\n\nPermanent assembly") {
            out.replace_range(
                start..start + end,
                &readme_metrics(exact, retained, executable),
            );
        }
    }
    if let Some(start) = out.find("**exact C stands at ") {
        let value_start = start + "**exact C stands at ".len();
        if let Some(end) = out[value_start..].find("%**") {
            let c_able = executable - retained;
            let c_share = if c_able == 0.0 {
                0.0
            } else {
                exact * 100.0 / c_able
            };
            out.replace_range(value_start..value_start + end, &format!("{c_share:.1}"));
        }
    }
    for (id, svg) in trees {
        let version = svg_cache_version(svg);
        let needle = format!("games/gs1/assets/readme/{target}-{id}.svg");
        if let Some(pos) = out.find(&needle) {
            let end = pos + needle.len();
            let rest = &out[end..];
            let cut = rest.find(')').unwrap_or(0);
            let replacement = format!("{needle}?v={version}");
            out.replace_range(pos..end + cut, &replacement);
        }
    }
    out
}
#[cfg(test)]
mod tests {
    use super::readme_metrics;
    #[test]
    fn readme_metrics_reports_all_done_categories() {
        assert_eq!(
            readme_metrics(282_436.0, 343_206.0, 1_347_122.0),
            "|                    |       bytes |                   share |\n\
             | ------------------ | ----------: | ----------------------: |\n\
             | Exact C            |     282,436 |     21.0% of executable |\n\
             | Permanent assembly |     343,206 |     25.5% of executable |\n\
             | **DONE**           | **625,642** | **46.4% of executable** |"
        );
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
        prefer_verified_assets: true,
    })?;
    let rendered = render_box_trees(&map, Some(&exact), true)?;
    let map_json = canonical_json(&tracked(&map.document));
    if o.check {
        for (id, svg) in &rendered {
            if read(&box_tree_path(&o.target, id))? != *svg {
                return Err(format!(
                    "tracked {id} coverage figure is stale; run: make coverage"
                ));
            }
        }
        let readme = read(&root().join("README.md"))?;
        if update_readme(&readme, &o.target, &map, &rendered) != readme {
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
            &update_readme(&readme, &o.target, &map, &rendered),
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
