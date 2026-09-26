pub(crate) mod audit;
pub(crate) mod boxtree;
pub(crate) mod figure;
pub(crate) mod history;
pub(crate) mod jsnum;
pub(crate) mod letters;
pub(crate) mod model;
pub(crate) mod palette;
pub(crate) mod pipeline;
pub(crate) mod progress;
pub(crate) mod proof;
pub(crate) mod raster;
pub(crate) mod sessions;
pub(crate) mod tree;

use crate::compiler::canonical_json::canonical_json;
use crate::coverage::jsnum::{commas, number};
use crate::coverage::pipeline::{build_coverage_map, BuildOptions, CoverageMap};
use crate::coverage::progress::{game_done, measured, GameDone};
use crate::coverage::tree::{ref_tree, root, work_tree};
use serde_json::Value;
use std::path::{Path, PathBuf};
const USAGE: &str = "usage: alchemy check coverage [--target tbs-en|tla-en] [--exact-ref <ref>|worktree] [--recon-ref <ref>|worktree|none] [--write|--check|--files|--models|--assembly-spans|--self-test]";
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
    files: bool,
    models: bool,
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
            "--files" => o.files = true,
            "--models" => o.models = true,
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
    let score: GameDone = serde_json::from_value(doc["done"].clone())
        .map_err(|e| format!("coverage has no verified score: {e}"))?;
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
        commas(score.bytes()),
        number(score.percent()),
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
/// The README status line under "## Progress": ☀️ The Broken Seal and
/// ⚓️ The Lost Age, each pending until its executable audit gives it a
/// denominator.
fn status_line(sun: Option<GameDone>, anchor: Option<GameDone>) -> String {
    let show = |done: Option<GameDone>| {
        done.map_or("pending".to_string(), |d| format!("{:.2}%", d.percent()))
    };
    format!("**☀️ {} · ⚓️ {}**", show(sun), show(anchor))
}
fn readme_progress(text: &str) -> Option<&str> {
    text.lines()
        .skip_while(|line| *line != "## Progress")
        .skip(1)
        .find(|line| !line.trim().is_empty())
}
fn update_readme(text: &str, _target: &str, map: &CoverageMap, status: &str) -> String {
    let proven_c = field(&map.document, &["categories", "proven_c", "bytes"]);
    let proven_asm = field(&map.document, &["categories", "proven_asm", "bytes"]);
    let executable = field(&map.document, &["executable_bytes"]);
    let percent =
        crate::coverage::jsnum::done_percent(proven_c as i64, proven_asm as i64, executable as i64);
    let mut out = text.to_string();
    let progress_start = out
        .find("## Progress\n")
        .map(|start| start + "## Progress\n".len());
    let status_start = if let Some(start) = progress_start {
        let section = &out[start..];
        let end = section.find("\n## ").unwrap_or(section.len());
        section[..end].find("**☀️ ").map(|offset| start + offset)
    } else {
        out.find("**☀️ ").or_else(|| out.find("## Status:"))
    };
    if let Some(start) = status_start {
        let end = out[start..].find('\n').unwrap_or(out.len() - start);
        out.replace_range(start..start + end, status);
    }
    if readme_progress(&out) != Some(status) {
        if let Some(start) = out.find("## Progress\n") {
            out.insert_str(start + "## Progress\n".len(), &format!("\n{status}\n"));
        } else {
            let start = out.find("## Acknowledgements").unwrap_or(out.len());
            out.insert_str(start, &format!("\n## Progress\n\n{status}\n\n"));
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
    out
}
#[cfg(test)]
mod tests {
    use super::{readme_metrics, readme_progress, status_line, update_readme};
    use crate::coverage::pipeline::CoverageMap;
    use crate::coverage::progress::GameDone;
    use serde_json::json;
    #[test]
    fn figures_are_redrawn_with_each_count_and_match_the_readme() {
        use super::{check_figures, figure, figure_date_current, history, letters, write_figures};
        let root = tempfile::tempdir().unwrap();
        let root = root.path();
        let manifest = letters::sheet("THE BROKEN SEAL", letters::MENU);
        let table: serde_json::Value = serde_json::from_slice(
            &std::fs::read(crate::coverage::tree::root().join(&manifest)).unwrap(),
        )
        .unwrap();
        let image = table["components"][0]["source"]
            .as_str()
            .unwrap()
            .to_string();
        let widths = table["glyphs"]["advances"]["source"]
            .as_str()
            .unwrap()
            .to_string();
        for path in [&manifest, &image, &widths] {
            std::fs::create_dir_all(root.join(path).parent().unwrap()).unwrap();
            std::fs::copy(crate::coverage::tree::root().join(path), root.join(path)).unwrap();
        }
        std::fs::create_dir_all(history::path(root).parent().unwrap()).unwrap();
        std::fs::write(
            history::path(root),
            history::text(
                &json!({"format": 1, "began": "2026-07-16", "stricter": [], "days": [
                    {"date": "2026-07-16", "tbs": {"percent": 1.0}}
                ]}),
            ),
        )
        .unwrap();
        assert!(std::process::Command::new("git")
            .args(["init", "--quiet"])
            .current_dir(root)
            .status()
            .unwrap()
            .success());
        assert!(std::process::Command::new("git")
            .args(["add", "games"])
            .current_dir(root)
            .status()
            .unwrap()
            .success());
        let done = |bytes| GameDone {
            game_c: bytes,
            executable: 1000,
            ..GameDone::default()
        };
        std::fs::write(
            root.join("README.md"),
            "## Progress\n\n**☀️ 60.00% · ⚓️ pending**\n",
        )
        .unwrap();
        write_figures(root, Some(done(600)), None).unwrap();
        let chart = std::fs::read(root.join(figure::CHART)).unwrap();
        let map = std::fs::read(root.join(figure::MAP)).unwrap();
        check_figures(root, Some(done(600)), None).unwrap();
        // A later count the same day redraws the chart with it.
        write_figures(root, Some(done(610)), None).unwrap();
        std::fs::write(
            root.join("README.md"),
            "## Progress\n\n**☀️ 61.00% · ⚓️ pending**\n",
        )
        .unwrap();
        assert_ne!(std::fs::read(root.join(figure::CHART)).unwrap(), chart);
        let _ = map;
        let recorded = history::load(root).unwrap();
        let today = history::today();
        let row = recorded["days"].as_array().unwrap().last().unwrap().clone();
        assert_eq!(
            (row["date"].as_str(), history::percent(&row["tbs"])),
            (Some(today.as_str()), Some(61.0))
        );
        assert_eq!(history::percent(&recorded["figures"]["tbs"]), Some(61.0));
        check_figures(root, Some(done(610)), None).unwrap();
        // A README stating another number fails.
        std::fs::write(
            root.join("README.md"),
            "## Progress\n\n**☀️ 60.00% · ⚓️ pending**\n",
        )
        .unwrap();
        assert!(check_figures(root, Some(done(610)), None).is_err());
        // Missing progress cannot bypass the gate; correct numbers elsewhere cannot either.
        for readme in [
            "## Progress\n\n<img src=\"PROGRESS_CHART.png\">\n",
            "**☀️ 61.00% · ⚓️ pending**\n",
            "**☀️ 61.00% · ⚓️ pending**\n\n## Progress\n\n**☀️ 60.00% · ⚓️ pending**\n",
            "## Progress\n\n**☀️ 61.00% · ⚓️ 2.00%**\n",
        ] {
            std::fs::write(root.join("README.md"), readme).unwrap();
            assert!(check_figures(root, Some(done(610)), None).is_err());
        }
        std::fs::write(
            root.join("README.md"),
            "## Progress\n\n**☀️ 61.00% · ⚓️ pending**\n",
        )
        .unwrap();
        check_figures(root, Some(done(610)), None).unwrap();
        assert!(check_figures(
            root,
            Some(GameDone {
                game_c: 1220,
                executable: 2000,
                ..GameDone::default()
            }),
            None
        )
        .is_err());
        // Equal percentages must not conceal different measured byte counts.
        let mut changed = history::load(root).unwrap();
        changed["days"].as_array_mut().unwrap().last_mut().unwrap()["tbs"] =
            json!({"done": 1220, "executable": 2000});
        std::fs::write(history::path(root), history::text(&changed)).unwrap();
        assert!(check_figures(root, Some(done(610)), None).is_err());
        write_figures(root, Some(done(610)), None).unwrap();
        check_figures(root, Some(done(610)), None).unwrap();
        // A changed tracked tree requires a fresh map, even with unchanged DONE.
        std::fs::write(root.join("games/CHECK.C"), "void Check(void) {}\n").unwrap();
        assert!(std::process::Command::new("git")
            .args(["add", "games/CHECK.C"])
            .current_dir(root)
            .status()
            .unwrap()
            .success());
        assert!(check_figures(root, Some(done(610)), None).is_err());
        write_figures(root, Some(done(610)), None).unwrap();
        check_figures(root, Some(done(610)), None).unwrap();
        // A tampered chart fails; yesterday's figures pass only until today has a row.
        std::fs::write(root.join(figure::CHART), &map).unwrap();
        assert!(check_figures(root, Some(done(610)), None).is_err());
        let yesterday = history::previous(&today).unwrap();
        assert!(figure_date_current(&today, &today, true));
        assert!(figure_date_current(&yesterday, &today, false));
        assert!(!figure_date_current(&yesterday, &today, true));
        assert!(!figure_date_current(
            &history::previous(&yesterday).unwrap(),
            &today,
            false
        ));
    }
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
            executable_areas: Vec::new(),
        };
        let sun = GameDone {
            game_c: 250,
            game_asm: 340,
            executable: 1000,
            ..GameDone::default()
        };
        let status = status_line(Some(sun), None);
        assert_eq!(status, "**☀️ 59.00% · ⚓️ pending**");
        let updated = update_readme(
            "# Alchemy\n\n## Progress\n\n**☀️ 52% · ⚓️ 1%**\n\nDetails\n",
            "tbs-en",
            &map,
            &status,
        );
        assert!(updated.contains("## Progress\n\n**☀️ 59.00% · ⚓️ pending**\n"));
        assert!(!updated.contains("52%"));
        for missing in [
            "# Alchemy\n\n## Progress\n\n<img src=\"PROGRESS_CHART.png\">\n",
            "# Alchemy\n\n## Acknowledgements\n\nThanks\n",
            "# Alchemy\n\n## Progress\n\n**☀️ 52% · ⚓️ 1%**",
        ] {
            let updated = update_readme(missing, "tbs-en", &map, &status);
            assert_eq!(readme_progress(&updated), Some(status.as_str()));
            assert_eq!(update_readme(&updated, "tbs-en", &map, &status), updated);
        }
    }
}
/// The digest of what the file map draws: each tracked file and its size.
fn map_inputs(root: &Path) -> String {
    let listing = boxtree::tracked_only(root, boxtree::disk_tiles(root))
        .iter()
        .map(|tile| format!("{}\t{}\n", tile.source.as_deref().unwrap_or(""), tile.bytes))
        .collect::<String>();
    boxtree::content_version(&listing)
}
/// Both README figures as the history's recorded figure date draws them.
fn render_figures(
    root: &Path,
    history: &serde_json::Value,
) -> Result<(raster::Canvas, raster::Canvas), String> {
    let letters = letters::Letters::menu(root)?;
    let chart = figure::chart(&letters, &history::as_drawn(history));
    Ok((chart, figure::map(&letters, root)))
}
/// Record today's verified counts and redraw both figures, so the chart
/// always shows the numbers the README states.
fn write_figures(
    root: &Path,
    sun: Option<GameDone>,
    anchor: Option<GameDone>,
) -> Result<(), String> {
    let today = history::today();
    let mut history = history::load(root)?;
    history::record(&mut history, &today, sun, anchor);
    history::record_models(&mut history, &today, &history::models_on(root, &today)?);
    // The history is itself a tracked file the map draws: write it with a
    // placeholder of the digest's length, then record the digest of the
    // tree as it now stands, which leaves the history's size unchanged.
    history::mark_drawn(&mut history, &today, &"0".repeat(16));
    write(&history::path(root), &history::text(&history))?;
    history["figures"]["files"] = serde_json::json!(map_inputs(root));
    write(&history::path(root), &history::text(&history))?;
    let (chart, map) = render_figures(root, &history)?;
    let scale = letters::FIGURE_SCALE;
    let (chart, map) = (chart.png(scale, &today)?, map.png(scale, &today)?);
    std::fs::write(root.join(figure::CHART), chart)
        .map_err(|e| format!("{}: {e}", figure::CHART))?;
    std::fs::write(root.join(figure::MAP), map).map_err(|e| format!("{}: {e}", figure::MAP))
}
/// The committed figures are current when they carry the history's figure
/// date, that date is today (or yesterday while today has no row), they show
/// the latest recorded row and the README's progress line, the chart
/// is exactly what that day's rows draw, and the map is exactly what the
/// current tracked files draw.
fn check_figures(
    root: &Path,
    sun: Option<GameDone>,
    anchor: Option<GameDone>,
) -> Result<(), String> {
    let stale = |why: &str| {
        Err(format!(
            "README figures are stale ({why}); run: make coverage"
        ))
    };
    let history = history::load(root)?;
    let date = history["figures"]["date"]
        .as_str()
        .unwrap_or("")
        .to_string();
    let today = history::today();
    let has_today = history["days"]
        .as_array()
        .is_some_and(|days| days.iter().any(|row| row["date"] == today.as_str()));
    if !figure_date_current(&date, &today, has_today) {
        return stale(&format!("drawn on {date:?}"));
    }
    for (game, done) in [("tbs", sun), ("tla", anchor)] {
        if let Some(done) = done.filter(|done| done.executable > 0) {
            let measured = serde_json::json!({"done": done.bytes(), "executable": done.executable});
            if history["figures"][game] != measured {
                return stale(&format!("{game} does not match the verified byte counts"));
            }
        }
    }
    let latest = history["days"].as_array().and_then(|days| days.last());
    for game in ["tbs", "tla"] {
        if latest.map(|row| &row[game]) != Some(&history["figures"][game]) {
            return stale(&format!("{game} is not the latest recorded row"));
        }
    }
    let readme = std::fs::read_to_string(root.join("README.md")).unwrap_or_default();
    let expected = status_line(sun, anchor);
    if readme_progress(&readme) != Some(expected.as_str()) {
        return stale(&format!("the line under ## Progress must be {expected}"));
    }
    let chart =
        std::fs::read(root.join(figure::CHART)).map_err(|e| format!("{}: {e}", figure::CHART))?;
    let map = std::fs::read(root.join(figure::MAP)).map_err(|e| format!("{}: {e}", figure::MAP))?;
    for (name, png) in [(figure::CHART, &chart), (figure::MAP, &map)] {
        if raster::png_date(png).as_deref() != Some(date.as_str()) {
            return stale(&format!("{name} does not carry {date}"));
        }
    }
    // Compared by decoded pixels, so a check never has to deflate again.
    let (expected_chart, expected_map) = render_figures(root, &history)?;
    let scale = letters::FIGURE_SCALE;
    let drawn = |canvas: &raster::Canvas| {
        Some((
            canvas.width as u32 * scale,
            canvas.height as u32 * scale,
            canvas.rgba(scale),
        ))
    };
    if raster::decode(&chart) != drawn(&expected_chart) {
        return stale(&format!("{} differs from its rows", figure::CHART));
    }
    if history["figures"]["files"].as_str() != Some(map_inputs(root).as_str()) {
        return stale(&format!(
            "{} was drawn from different tracked files",
            figure::MAP
        ));
    }
    if raster::decode(&map) != drawn(&expected_map) {
        return stale(&format!("{} differs from the tracked files", figure::MAP));
    }
    Ok(())
}
fn figure_date_current(date: &str, today: &str, has_today: bool) -> bool {
    date == today || (!has_today && history::previous(today).as_deref() == Some(date))
}
fn run(argv: &[String]) -> Result<String, String> {
    let o = parse(argv)?;
    if o.help {
        return Ok(USAGE.into());
    }
    if o.self_test {
        return Ok("self-test=ok coverage-map".into());
    }
    if o.models {
        // Relabel every day's commits by model from the local agent logs.
        let mut history = history::load(&root())?;
        let moved = history::relabel_models(&root(), &mut history)?;
        write(&history::path(&root()), &history::text(&history))?;
        return Ok(moved
            .iter()
            .map(|((from, to), n)| format!("{n}\t{from} -> {to}"))
            .collect::<Vec<_>>()
            .join("\n"));
    }
    if o.files {
        if o.exact.is_some() || o.recon.is_some() || o.assembly_spans {
            return Err("--files accepts only --write or --check".into());
        }
        let (sun, anchor) = (measured(&root(), "tbs-en")?, measured(&root(), "tla-en")?);
        if o.check {
            check_figures(&root(), sun, anchor)?;
        } else if o.write {
            write_figures(&root(), sun, anchor)?;
        } else {
            return Err("--files requires --write or --check".into());
        }
        return Ok(format!("figures={} {}", figure::CHART, figure::MAP));
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
        check_figures(&root(), sun, anchor)?;
        let readme = read(&root().join("README.md"))?;
        if update_readme(&readme, &o.target, &map, &status) != readme {
            return Err("README coverage values are stale; run: make coverage".into());
        }
        return Ok(format!("coverage-map=current {}", summary(&map.document)?));
    }
    if o.write {
        write(&map_path(&o.target), &map_json)?;
        write_figures(&root(), sun, anchor)?;
        let readme = read(&root().join("README.md"))?;
        write(
            &root().join("README.md"),
            &update_readme(&readme, &o.target, &map, &status),
        )?;
        return Ok(format!(
            "map={} figures={},{} {}",
            map_path(&o.target).display(),
            figure::CHART,
            figure::MAP,
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
