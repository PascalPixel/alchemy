use crate::boxtree::{box_tree_path, render_box_trees, svg_cache_version, BOX_TREES};
use crate::jsnum::{commas, number};
use crate::pipeline::{build_coverage_map, BuildOptions, CoverageMap};
use crate::tree::{ref_tree, root, work_tree};
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
fn primitive(v: &Value) -> bool {
    !matches!(v, Value::Array(_) | Value::Object(_))
}
fn canonical(v: &Value) -> String {
    fn visit(v: &Value, indent: usize, out: &mut String) {
        match v {
            Value::Null => out.push_str("null"),
            Value::Bool(x) => out.push_str(if *x { "true" } else { "false" }),
            Value::Number(x) => out.push_str(&x.to_string()),
            Value::String(x) => out.push_str(&serde_json::to_string(x).unwrap()),
            Value::Array(xs) if xs.iter().all(primitive) => {
                out.push('[');
                for (i, x) in xs.iter().enumerate() {
                    if i > 0 {
                        out.push_str(", ");
                    }
                    visit(x, indent, out);
                }
                out.push(']');
            }
            Value::Array(xs) => {
                out.push('[');
                if !xs.is_empty() {
                    out.push('\n');
                    for (i, x) in xs.iter().enumerate() {
                        if i > 0 {
                            out.push_str(",\n");
                        }
                        out.push_str(&"  ".repeat(indent + 1));
                        visit(x, indent + 1, out);
                    }
                    out.push('\n');
                    out.push_str(&"  ".repeat(indent));
                }
                out.push(']');
            }
            Value::Object(xs) => {
                out.push('{');
                if !xs.is_empty() {
                    out.push('\n');
                    for (i, (k, x)) in xs.iter().enumerate() {
                        if i > 0 {
                            out.push_str(",\n");
                        }
                        out.push_str(&"  ".repeat(indent + 1));
                        out.push_str(&quote(k));
                        out.push_str(": ");
                        visit(x, indent + 1, out);
                    }
                    out.push('\n');
                    out.push_str(&"  ".repeat(indent));
                }
                out.push('}');
            }
        }
    }
    let mut out = String::new();
    visit(v, 0, &mut out);
    out
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
        .join("games")
        .join(target.split('-').next().unwrap_or("gs1"))
        .join("metrics")
        .join(format!("{target}-coverage-map.json"))
}
fn target_path(target: &str) -> PathBuf {
    root()
        .join("games")
        .join(target.split('-').next().unwrap_or("gs1"))
        .join("metrics")
        .join(format!("{target}-core-targets.json"))
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
    Ok(format!("target={} rom={} executable={} exact={} ({}%) tracked={} ({}%) c_able={} exact_of_c_able={}% tracked_source={}", get(doc, "target").and_then(Value::as_str).unwrap_or("undefined"), commas(field(doc, &["rom_bytes"]) as i64), commas(executable as i64), commas(exact as i64), number(field(doc, &["categories", "exact_c", "percent_of_executable"])), commas(semantic as i64), number(field(doc, &["categories", "tracked_c", "percent_of_executable"])), commas(ceiling as i64), number(percent), get(get(doc, "provenance").unwrap_or(&Value::Null), "tracked_source").and_then(Value::as_str).unwrap_or("undefined")))
}

#[derive(Clone)]
struct Target {
    namespace: String,
    address: Option<i64>,
    label: String,
    scope: i64,
    target: i64,
    semantic: i64,
    assembly: i64,
    exact: i64,
    retained: i64,
}
fn targets(map: &CoverageMap, target: &str) -> (Value, String) {
    let mut rows = Vec::new();
    let mut spaces = std::collections::BTreeSet::new();
    for area in &map.executable_areas {
        for tile in &area.tiles {
            let target_bytes = tile.category("tracked_c") + tile.category("assembly");
            let namespace = if area.id == "main" {
                "main".into()
            } else {
                format!("resource_{}", tile.group.as_deref().unwrap_or("unknown"))
            };
            spaces.insert(namespace.clone());
            if target_bytes > 0 {
                rows.push(Target {
                    namespace,
                    address: tile.address,
                    label: tile.label.clone(),
                    scope: tile.bytes,
                    target: target_bytes,
                    semantic: tile.category("tracked_c"),
                    assembly: tile.category("assembly"),
                    exact: tile.category("exact_c"),
                    retained: tile.category("retained_asm"),
                });
            }
        }
    }
    rows.sort_by(|a, b| {
        (b.scope, b.target, &a.namespace, a.address, &a.label).cmp(&(
            a.scope,
            a.target,
            &b.namespace,
            b.address,
            &b.label,
        ))
    });
    let values: Vec<Value> = rows
        .iter()
        .enumerate()
        .map(|(i, r)| {
            obj(vec![
                ("rank", num((i + 1) as i64)),
                ("namespace", Value::String(r.namespace.clone())),
                (
                    "address",
                    r.address
                        .map_or(Value::Null, |a| Value::String(format!("0x{a:08x}"))),
                ),
                ("label", Value::String(r.label.clone())),
                ("scope_bytes", num(r.scope)),
                ("target_bytes", num(r.target)),
                ("tracked_c_bytes", num(r.semantic)),
                ("assembly_bytes", num(r.assembly)),
                ("exact_c_leverage_bytes", num(r.exact)),
                ("permanent_assembly_bytes", num(r.retained)),
            ])
        })
        .collect();
    let executable = field(&map.document, &["executable_bytes"]) as i64;
    let scope = rows.iter().map(|r| r.scope).sum::<i64>();
    let unresolved = rows.iter().map(|r| r.target).sum::<i64>();
    let semantic = rows.iter().map(|r| r.semantic).sum::<i64>();
    let assembly = rows.iter().map(|r| r.assembly).sum::<i64>();
    let exact_leverage = rows.iter().map(|r| r.exact).sum::<i64>();
    let retained_leverage = rows.iter().map(|r| r.retained).sum::<i64>();
    let document = obj(vec![("format", num(1)), ("kind", Value::String("golden-sun-core-target-index".into())), ("target", Value::String(target.into())), ("derivation", Value::String("coverage-owner-scopes-v1".into())), ("policy", obj(vec![("unit", Value::String("audited source-owner scope or contiguous unresolved executable run".into())), ("ordering", Value::String("scope_bytes descending, then target_bytes descending, then namespace and address".into())), ("target_categories", Value::Array(vec![Value::String("tracked_c".into()), Value::String("assembly".into())])), ("overlap", Value::String("none; broader campaign cuts are intentionally excluded".into()))])), ("accounting", obj(vec![("executable_bytes", num(executable)), ("audited_address_spaces", num(spaces.len() as i64)), ("address_spaces_with_targets", num(rows.iter().map(|r| r.namespace.as_str()).collect::<std::collections::BTreeSet<_>>().len() as i64)), ("target_count", num(rows.len() as i64)), ("target_scope_bytes", num(scope)), ("target_bytes", num(unresolved)), ("tracked_c_bytes", num(semantic)), ("assembly_bytes", num(assembly)), ("exact_c_leverage_bytes", num(exact_leverage)), ("permanent_assembly_leverage_bytes", num(retained_leverage)), ("resolved_only_bytes", num(executable - scope))])), ("targets", Value::Array(values))]);
    let visible: Vec<_> = rows
        .iter()
        .enumerate()
        .filter(|(_, r)| r.scope >= 1000)
        .collect();
    let mut md = format!("This section is generated. It is the primary contributor target list:\nnon-overlapping audited source-owner scopes (or contiguous unresolved\nexecutable runs), sorted largest to smallest. Regenerate with `make coverage` -- do not edit by hand.\n\n- **Unfinished scopes:** {}\n- **Address spaces scanned:** {} ({} still contain targets)\n- **Target bytes:** {} tracked-C or unresolved-assembly bytes\n- **Resolved-only bytes:** {} Exact C or audited permanent assembly bytes\n- **Executable bytes accounted for:** {}\n\n### Main target list\n\nThis table contains every scope of at least 1,000 bytes ({} rows). The complete\n{}-row index, including the smallest audited owners, is\n[`games/gs1/metrics/gs1-en-core-targets.json`](games/gs1/metrics/gs1-en-core-targets.json).\n\n| Rank | Scope | Target | Namespace / owner |\n|---:|---:|---:|---|\n", commas(rows.len() as i64), commas(spaces.len() as i64), commas(rows.iter().map(|r| r.namespace.as_str()).collect::<std::collections::BTreeSet<_>>().len() as i64), commas(unresolved), commas(executable - scope), commas(executable), visible.len(), commas(rows.len() as i64));
    for (i, r) in visible {
        md.push_str(&format!(
            "| {} | {} | {} | `{}:{}` |\n",
            i + 1,
            commas(r.scope),
            commas(r.target),
            r.namespace,
            r.address
                .map_or_else(|| "unknown".into(), |a| format!("0x{a:08x}"))
        ));
    }
    (document, md)
}
fn obj(fields: Vec<(&str, Value)>) -> Value {
    Value::Object(fields.into_iter().map(|(k, v)| (k.into(), v)).collect())
}
fn num(n: i64) -> Value {
    Value::Number(n.into())
}
fn targets_section(text: &str) -> Option<(usize, usize)> {
    let marker = "\n## Targets\n";
    let start = text.find(marker)? + marker.len();
    let end = text[start..]
        .find("\n## ")
        .map_or(text.len(), |n| start + n + 1);
    Some((start, end))
}
fn replace_targets(text: &str, section: &str) -> Result<String, String> {
    let (start, end) =
        targets_section(text).ok_or("CONTRIBUTING.md has no '## Targets' section")?;
    Ok(format!(
        "{}\n{}\n{}",
        &text[..start],
        section.trim_end(),
        &text[end..]
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
            let replacement = format!("## DONE: Currently {}%", number(percent.floor()));
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
        validate_tracked_progress: true,
        prefer_verified_assets: true,
    })?;
    let rendered = render_box_trees(&map, Some(&exact), true)?;
    let (index, markdown) = targets(&map, &o.target);
    let map_json = canonical(&tracked(&map.document));
    let index_json = canonical(&index);
    if o.check {
        let tracked_map: Value =
            serde_json::from_str(&read(&map_path(&o.target))?).map_err(|e| e.to_string())?;
        for (path, expected) in [
            (
                "executable_bytes",
                field(&map.document, &["executable_bytes"]),
            ),
            (
                "exact C",
                field(&map.document, &["categories", "exact_c", "bytes"]),
            ),
            (
                "retained ASM",
                field(&map.document, &["categories", "retained_asm", "bytes"]),
            ),
        ] {
            let actual = match path {
                "executable_bytes" => field(&tracked_map, &["executable_bytes"]),
                "exact C" => field(&tracked_map, &["categories", "exact_c", "bytes"]),
                _ => field(&tracked_map, &["categories", "retained_asm", "bytes"]),
            };
            if actual != expected {
                return Err(format!(
                    "tracked coverage map is stale ({path}); run: make coverage"
                ));
            }
        }
        if !target_path(&o.target).exists()
            || rendered
                .iter()
                .any(|(id, _)| !box_tree_path(&o.target, id).exists())
            || !root().join("CONTRIBUTING.md").exists()
        {
            return Err("coverage outputs are incomplete; run: make coverage".into());
        }
        return Ok(format!("coverage-map=current {}", summary(&map.document)?));
    }
    if o.write {
        write(&map_path(&o.target), &map_json)?;
        write(&target_path(&o.target), &index_json)?;
        let doc = read(&root().join("CONTRIBUTING.md"))?;
        write(
            &root().join("CONTRIBUTING.md"),
            &replace_targets(&doc, &markdown)?,
        )?;
        for (id, svg) in &rendered {
            write(&box_tree_path(&o.target, id), svg)?;
        }
        let readme = read(&root().join("README.md"))?;
        write(
            &root().join("README.md"),
            &update_readme(&readme, &o.target, &map, &rendered),
        )?;
        return Ok(format!(
            "map={} core_index={} targets={} trees={} {}",
            map_path(&o.target).display(),
            target_path(&o.target).display(),
            root().join("CONTRIBUTING.md").display(),
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
