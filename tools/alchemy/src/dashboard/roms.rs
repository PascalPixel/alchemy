//! ROM coverage: the twelve physical cartridge indexes, each summarised once
//! per change into the dashboard cache so the overview never reparses them.
//! Colours identify content only; they never grant DONE credit.
use super::cache::{self, Phase, Store};
use super::chrome;
use super::http::Response;
use crate::coverage::audit::index::input_current;
use crate::coverage::boxtree::{content_style, esc, C_TEAL, UNKNOWN};
use crate::coverage::jsnum::commas;
use crate::coverage::model::{Tile, UNIDENTIFIED};
use crate::coverage::tree::work_tree_at;
use crate::targets::{parse_decomp_target, DecompTargetId, TARGET_IDS};
use serde::{Deserialize, Serialize};
use std::{collections::BTreeMap, path::Path, sync::Mutex};

const ROM_BASE: i64 = 0x0800_0000;
/// The overview is at most a few thousand pixels wide; each bar keeps one
/// dominant content type per bucket while the totals keep every interval.
const BUCKETS: i64 = 512;

#[derive(Clone, Debug, Serialize, Deserialize)]
struct Input {
    path: String,
    digest: String,
    seen: String,
    current: bool,
}
#[derive(Clone, Debug, Serialize, Deserialize)]
pub(super) struct Summary {
    rom_bytes: i64,
    identified: i64,
    current: bool,
    inputs: Vec<Input>,
    segments: Vec<(i64, i64, String, String)>,
    totals: Vec<(String, String, i64)>,
    /// Source file → (indexed regions, stored ROM bytes), for file details.
    sources: BTreeMap<String, (usize, i64)>,
}
static SUMMARIES: Mutex<BTreeMap<&'static str, Summary>> = Mutex::new(BTreeMap::new());

#[derive(Deserialize)]
struct Index {
    format: String,
    target: String,
    rom_bytes: i64,
    #[serde(default)]
    inputs: BTreeMap<String, String>,
    regions: Vec<Row>,
}
#[derive(Deserialize)]
struct Row {
    start: i64,
    end: i64,
    kind: String,
    #[serde(default)]
    sources: Vec<String>,
}

fn style(kind: &str, source: Option<&str>) -> (&'static str, &'static str) {
    let tile = |group: &str| Tile {
        group: Some(group.into()),
        source: source.map(String::from),
        ..Tile::default()
    };
    match kind {
        "executable" => ("Executable code", C_TEAL),
        "encoded-overlay" => content_style(&tile("golden-sun-thumb-overlay")),
        "unresolved-data" => (UNIDENTIFIED, UNKNOWN),
        _ => content_style(&tile(kind)),
    }
}
fn unidentified(kind: &str) -> bool {
    let unknown_style = !matches!(kind, "executable" | "encoded-overlay")
        && content_style(&Tile {
            group: Some(kind.into()),
            ..Tile::default()
        })
        .0 == UNIDENTIFIED;
    unknown_style
        || matches!(
            kind,
            "unresolved-data"
                | "compressed-resource"
                | "golden-sun-general-lz"
                | "golden-sun-kind2-lz"
        )
}
fn summarize(index: Index, target: &str, attribute: bool) -> Result<Summary, String> {
    if index.format != "alchemy-rom-index-v1" || index.target != target || index.rom_bytes <= 0 {
        return Err(format!("{target}: unsupported ROM index"));
    }
    let size = index.rom_bytes;
    let mut runs = Vec::<(i64, i64, &'static str, &'static str)>::new();
    let mut totals = BTreeMap::<(&str, &str), i64>::new();
    let mut unresolved = 0;
    let mut sources = BTreeMap::<String, (usize, i64)>::new();
    for row in &index.regions {
        let (label, color) = style(&row.kind, row.sources.first().map(String::as_str));
        let bytes = row.end - row.start;
        *totals.entry((label, color)).or_default() += bytes;
        if unidentified(&row.kind) {
            unresolved += bytes;
        }
        if attribute {
            for source in &row.sources {
                let entry = sources.entry(source.clone()).or_default();
                entry.0 += 1;
                entry.1 += bytes;
            }
        }
        match runs.last_mut() {
            Some(last) if last.1 == row.start && last.2 == label && last.3 == color => {
                last.1 = row.end
            }
            _ => runs.push((row.start, row.end, label, color)),
        }
    }
    let mut segments = Vec::<(i64, i64, String, String)>::new();
    let mut cursor = 0;
    for bucket in 0..BUCKETS {
        let start = ROM_BASE + size * bucket / BUCKETS;
        let end = ROM_BASE + size * (bucket + 1) / BUCKETS;
        while cursor < runs.len() && runs[cursor].1 <= start {
            cursor += 1;
        }
        let mut weights = BTreeMap::<(&str, &str), i64>::new();
        let mut row = cursor;
        while row < runs.len() && runs[row].0 < end {
            let overlap = runs[row].1.min(end) - runs[row].0.max(start);
            if overlap > 0 {
                *weights.entry((runs[row].2, runs[row].3)).or_default() += overlap;
            }
            row += 1;
        }
        let (label, color) = weights
            .into_iter()
            .max_by_key(|(_, bytes)| *bytes)
            .map_or((UNIDENTIFIED, UNKNOWN), |(style, _)| style);
        match segments.last_mut() {
            Some(last) if last.1 == start && last.2 == label && last.3 == color => last.1 = end,
            _ => segments.push((start, end, label.into(), color.into())),
        }
    }
    let mut totals = totals
        .into_iter()
        .map(|((label, color), bytes)| (label.to_string(), color.to_string(), bytes))
        .collect::<Vec<_>>();
    totals.sort_by(|a, b| b.2.cmp(&a.2).then(a.0.cmp(&b.0)));
    Ok(Summary {
        rom_bytes: size,
        identified: size - unresolved,
        current: false,
        inputs: index
            .inputs
            .into_iter()
            .map(|(path, digest)| Input {
                path,
                digest,
                seen: String::new(),
                current: false,
            })
            .collect(),
        segments,
        totals,
        sources,
    })
}
fn read(root: &Path, target: &str) -> Result<Summary, String> {
    let path = root.join(format!("out/{target}/reports/rom-index.json"));
    let bytes = std::fs::read(&path).map_err(|e| format!("{}: {e}", path.display()))?;
    let index: Index = serde_json::from_slice(&bytes).map_err(|e| format!("{target}: {e}"))?;
    summarize(index, target, target.ends_with("-en"))
}

/// Summarise every generated index, reusing cached summaries whose index file
/// is unchanged and re-hashing only inputs whose metadata changed.
pub(super) fn refresh(root: &Path) -> Result<String, String> {
    let store = Store::at(root);
    let tree = work_tree_at(root.to_path_buf());
    let (mut read_count, mut failures) = (0, Vec::new());
    for (step, id) in TARGET_IDS.into_iter().enumerate() {
        let target = id.as_str();
        cache::advance("roms", step, format!("Reading {target}"));
        let index = root.join(format!("out/{target}/reports/rom-index.json"));
        if !index.is_file() {
            SUMMARIES
                .lock()
                .unwrap_or_else(|e| e.into_inner())
                .remove(target);
            continue;
        }
        let name = format!("rom-{target}");
        let stamp = cache::metadata_stamp(&[index]);
        let cached = store
            .load(&name, &stamp, "json")
            .and_then(|bytes| serde_json::from_slice::<Summary>(&bytes).ok());
        let (mut summary, mut dirty) = match cached {
            Some(summary) => (summary, false),
            None => match read(root, target) {
                Ok(summary) => (summary, true),
                Err(error) => {
                    failures.push(error);
                    SUMMARIES
                        .lock()
                        .unwrap_or_else(|e| e.into_inner())
                        .remove(target);
                    continue;
                }
            },
        };
        for input in &mut summary.inputs {
            let seen = cache::metadata_stamp(&[root.join(&input.path)]);
            if seen != input.seen {
                input.current = input_current(&tree, &input.path, &input.digest);
                input.seen = seen;
                dirty = true;
            }
        }
        summary.current = summary.inputs.iter().all(|input| input.current);
        if dirty {
            store.save(
                &name,
                &stamp,
                "json",
                &serde_json::to_vec(&summary).map_err(|e| e.to_string())?,
            )?;
        }
        SUMMARIES
            .lock()
            .unwrap_or_else(|e| e.into_inner())
            .insert(target, summary);
        read_count += 1;
    }
    cache::invalidate();
    if failures.is_empty() {
        Ok(format!("{read_count} of 12 cartridge indexes"))
    } else {
        Err(failures.join("; "))
    }
}
/// Regions and stored bytes an English index attributes to one source file.
pub(super) fn attribution(target: &str, source: &str) -> Option<(usize, i64, bool)> {
    let summaries = SUMMARIES.lock().unwrap_or_else(|e| e.into_inner());
    let summary = summaries.get(target)?;
    summary
        .sources
        .get(source)
        .map(|(regions, bytes)| (*regions, *bytes, summary.current))
}

fn game(id: DecompTargetId) -> (&'static str, String) {
    let name = id.as_str();
    (
        if name.starts_with("tbs-") {
            "The Broken Seal"
        } else {
            "The Lost Age"
        },
        name.rsplit_once('-')
            .map(|(_, edition)| edition.to_ascii_uppercase())
            .unwrap_or_default(),
    )
}
fn bar(summary: &Summary) -> String {
    summary
        .segments
        .iter()
        .map(|(start, end, label, color)| {
            format!(
                "<span title=\"0x{start:08x}–0x{end:08x} · {} bytes · {}\" style=\"left:{}%;width:{}%;background:{color}\"></span>",
                commas(end - start),
                esc(label),
                (start - ROM_BASE) as f64 / summary.rom_bytes as f64 * 100.0,
                (end - start) as f64 / summary.rom_bytes as f64 * 100.0,
            )
        })
        .collect()
}
fn details(id: DecompTargetId, summary: Option<&Summary>, reading: bool) -> String {
    let (title, edition) = game(id);
    let Some(summary) = summary else {
        return format!(
            "<section class=\"rom-detail\"><h2>{title} {edition}</h2><p class=\"quiet\">{}</p></section>",
            if reading { "Reading the cartridge index…".to_string() } else { format!("No ROM index. Run alchemy coverage audit --target {} --data.", id.as_str()) }
        );
    };
    let mut rows = String::new();
    for (label, color, bytes) in &summary.totals {
        rows.push_str(&format!(
            "<tr><td><i class=\"swatch\" style=\"background:{color}\"></i>{}</td><td>{}</td><td>{:.2}%</td></tr>",
            esc(label),
            commas(*bytes),
            *bytes as f64 / summary.rom_bytes as f64 * 100.0
        ));
    }
    format!(
        "<section class=\"rom-detail\"><h2>{title} {edition}{}</h2><div class=\"well table\"><table><thead><tr><th>Content</th><th>Bytes</th><th>Share</th></tr></thead><tbody>{rows}</tbody></table></div></section>",
        if summary.current {
            ""
        } else {
            " <span class=\"stale\" title=\"An input recorded by this index changed; run the audit again\">stale</span>"
        }
    )
}
pub(super) fn page(path: &str) -> Option<Response> {
    let target = match path {
        "/roms" | "/rom" => "tbs-en",
        _ => path
            .strip_prefix("/roms/")
            .or_else(|| path.strip_prefix("/rom/"))?,
    };
    let Ok(selected) = parse_decomp_target(target) else {
        return Some(super::http::not_found());
    };
    let job = cache::job("roms");
    // Until the first summary pass finishes, absent rows are still being read.
    let reading = job.as_ref().is_none_or(|job| job.phase == Phase::Running);
    let summaries = SUMMARIES.lock().unwrap_or_else(|e| e.into_inner()).clone();
    let mut list = String::new();
    for id in TARGET_IDS {
        let name = id.as_str();
        let (title, edition) = game(id);
        let summary = summaries.get(name);
        let (bar, state) = match summary {
            Some(summary) => (
                format!(
                    "<span class=\"rom-bar well\" role=\"img\" aria-label=\"{title} {edition}: {:.2}% identified\">{}</span>",
                    100.0 * summary.identified as f64 / summary.rom_bytes as f64,
                    bar(summary)
                ),
                format!(
                    "{:.2}%{}",
                    100.0 * summary.identified as f64 / summary.rom_bytes as f64,
                    if summary.current { "" } else { " <span class=\"stale\">stale</span>" }
                ),
            ),
            None if reading => (chrome::progress(None, "Reading index"), "Reading…".into()),
            None => (
                "<span class=\"rom-bar well empty\"></span>".into(),
                "Not audited".into(),
            ),
        };
        list.push_str(&format!(
            "<a class=\"rom-row\" href=\"/roms/{name}\"{}><span class=\"rom-name\">{title}</span><span class=\"rom-edition\">{edition}</span>{bar}<span class=\"rom-state\">{state}</span></a>",
            if id == selected { " aria-current=\"true\"" } else { "" },
        ));
    }
    let content = format!(
        "{}<main class=\"roms\"><div class=\"toolbar\"><span class=\"toolbar-label\">Cartridges</span><a class=\"button\" href=\"{path}\">Refresh</a></div><div class=\"rom-body\"><div class=\"well rom-list\">{list}</div>{}</div></main>",
        if reading { "<meta http-equiv=\"refresh\" content=\"1\">" } else { "" },
        details(selected, summaries.get(selected.as_str()), reading),
    );
    let audited = summaries.len();
    Some(chrome::page(
        path,
        &content,
        &format!("{audited} of 12 cartridges indexed · 0x08000000 to each cartridge's end"),
    ))
}

#[cfg(test)]
mod tests {
    use super::*;
    fn index(regions: &str) -> Index {
        serde_json::from_str(&format!(
            r#"{{"format":"alchemy-rom-index-v1","target":"tbs-en","rom_bytes":1024,
            "inputs":{{"out/tbs-en/reports/executable.json":"00"}},"regions":[{regions}]}}"#
        ))
        .unwrap()
    }
    #[test]
    fn summaries_bucket_the_display_but_total_every_byte() {
        let summary = summarize(
            index(
                r#"{"start":134217728,"end":134218240,"kind":"executable","evidence":"x"},
                {"start":134218240,"end":134218496,"kind":"gba-4bpp-tiles","sources":["games/X/TILE.PNG"]},
                {"start":134218496,"end":134218752,"kind":"unresolved-data"}"#,
            ),
            "tbs-en",
            true,
        )
        .unwrap();
        assert_eq!(summary.identified, 768);
        assert_eq!(summary.segments.len(), 3);
        assert_eq!(summary.segments[0].2, "Executable code");
        assert_eq!(summary.totals.iter().map(|t| t.2).sum::<i64>(), 1024);
        assert_eq!(summary.sources["games/X/TILE.PNG"], (1, 256));
        assert!(!summary.current);
        assert!(summarize(index(""), "tla-en", false).is_err());
        assert!(!summary.totals.iter().any(|(label, _, _)| label == "C"));
    }
    #[test]
    fn routes_select_known_cartridges_only() {
        assert_eq!(page("/roms/tbs-xx").unwrap().status, 404);
        assert!(page("/music").is_none());
        let html = String::from_utf8(page("/roms").unwrap().body).unwrap();
        assert_eq!(html.matches("class=\"rom-row\"").count(), 12);
        for id in TARGET_IDS {
            assert!(html.contains(&format!("href=\"/roms/{}\"", id.as_str())));
        }
        assert!(html.contains("aria-current=\"true\"><span class=\"rom-name\">The Broken Seal</span><span class=\"rom-edition\">EN"));
        assert!(!html.contains("<script"));
    }
}
