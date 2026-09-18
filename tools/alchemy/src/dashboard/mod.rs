//! Live coverage only.
use super::http::{self, root, Response};
use crate::coverage::{
    boxtree::{render_box_trees, svg_cache_version, BOX_TREES},
    pipeline::{build_coverage_map, BuildOptions},
    tree::work_tree_at,
};
use serde_json::{json, Map, Value};
use std::{
    net::TcpListener,
    path::{Component, Path, PathBuf},
    process::Command,
    sync::Mutex,
    time::{Duration, SystemTime, UNIX_EPOCH},
};
use walkdir::WalkDir;
const STYLES: &str = include_str!("style.css");
const TREES: [(&str, &str); 1] = [("rom", "ROM contents")];
const COVERAGE_DIRS: &[&str] = &[
    "games/THE BROKEN SEAL/locations.tsv",
    "games/THE BROKEN SEAL/raw",
    "games/THE BROKEN SEAL/SOURCE.JSON",
    "games/THE BROKEN SEAL/SRC/GRAPHICS",
    "games/THE BROKEN SEAL/SOUND",
    "games/THE BROKEN SEAL/TEXT",
    "games/THE BROKEN SEAL/metrics",
    "games/THE BROKEN SEAL/semantic",
    "games/THE BROKEN SEAL/SRC",
    "games/THE BROKEN SEAL/INCLUDE",
    "games/COMMON",
    "games/THE BROKEN SEAL/source-paths.json",
    "games/THE BROKEN SEAL/recon",
    "games/THE BROKEN SEAL/project.json",
    "games/THE LOST AGE",
    "games/THE LOST AGE/PROJECT.JSON",
    "out/tbs-en/reports",
    "out/tla-en/reports/verified-code.json",
];
fn page_version() -> String {
    svg_cache_version(STYLES)
}
pub struct Live {
    revision: String,
    generated: String,
    trees: Vec<(&'static str, String)>,
    map: Option<crate::coverage::pipeline::CoverageMap>,
    summary: Value,
}
#[derive(Default)]
pub struct State {
    coverage: Option<Live>,
    error: Option<String>,
    scanning: bool,
}
static STATE: Mutex<Option<State>> = Mutex::new(None);
fn state<R>(f: impl FnOnce(&mut State) -> R) -> R {
    f(STATE
        .lock()
        .unwrap_or_else(|e| e.into_inner())
        .get_or_insert_with(State::default))
}
fn document_number(document: &Value, path: &[&str]) -> Option<f64> {
    path.iter()
        .try_fold(document, |v, key| v.get(key))?
        .as_f64()
}
fn compute() -> Result<Live, String> {
    let tree = work_tree_at(root());
    let map = build_coverage_map(&BuildOptions {
        target: "tbs-en".into(),
        exact: &tree,
        recon: Some(&tree),
    })?;
    let trees = render_box_trees(&map);
    let mut live = live_from(map.document.clone(), trees)?;
    live.map = Some(map);
    Ok(live)
}
fn cached() -> Result<Live, String> {
    let report = root().join("out/tbs-en/reports/coverage-map.json");
    // Published charts survive removal of disposable build reports. They are
    // a fallback display, never proof of the current checkout's coverage.
    let document = std::fs::read(&report)
        .ok()
        .and_then(|bytes| serde_json::from_slice(&bytes).ok())
        .unwrap_or(Value::Null);
    let trees = BOX_TREES
        .iter()
        .map(|name| {
            let path = root()
                .join("games/THE BROKEN SEAL/PREVIEW")
                .join(format!("tbs-en-{name}.svg").to_ascii_uppercase());
            std::fs::read_to_string(&path)
                .map(|svg| (*name, svg))
                .map_err(|error| format!("{}: {error}", path.display()))
        })
        .collect::<Result<Vec<_>, _>>()?;
    let mut live = live_from(document, trees)?;
    live.generated.clear();
    Ok(live)
}

impl Live {
    fn chart_at(&self, id: &str, size: Option<(u16, u16)>, folder: &str) -> Option<String> {
        if let (Some(map), Some((width, height))) = (&self.map, size) {
            return Some(crate::coverage::boxtree::svg_sized(
                id,
                map,
                f64::from(width),
                if height == 0 {
                    f64::from(width) * 16.0 / 9.0
                } else {
                    f64::from(height)
                },
                folder,
            ));
        }
        if !folder.is_empty() {
            return None;
        }
        self.trees
            .iter()
            .find(|(key, _)| *key == id)
            .map(|(_, svg)| svg.clone())
    }
}
fn live_from(document: Value, trees: Vec<(&'static str, String)>) -> Result<Live, String> {
    let revision = trees
        .iter()
        .map(|(_, svg)| svg_cache_version(svg))
        .collect::<Vec<_>>()
        .join("-");
    let n = |key| document_number(&document, key).unwrap_or(0.0);
    let done = document
        .get("done")
        .cloned()
        .map(serde_json::from_value::<crate::coverage::progress::GameDone>)
        .transpose()
        .map_err(|e| e.to_string())?;
    let summary = json!({
        "games": document.get("games").and_then(Value::as_object)
            .into_iter().flatten().map(|(target, score)| {
                let done: crate::coverage::progress::GameDone =
                    serde_json::from_value(score.clone()).map_err(|e| e.to_string())?;
                Ok((target.clone(), json!({
                    "doneBytes": done.bytes(), "executableBytes": done.executable,
                    "donePercent": done.percent(),
                    "exactCBytes": done.common_c + done.game_c,
                    "permanentAssemblyBytes": done.common_asm + done.game_asm,
                    "parts": done
                })))
            }).collect::<Result<Map<String, Value>, String>>()?,
        "executableBytes": done.map(|d| d.executable),
        "provenCBytes": done.map(|d| d.common_c + d.game_c),
        "provenCPercent": number(n(&["categories", "proven_c", "percent_of_executable"])),
        "draftCBytes": number(n(&["categories", "draft_c", "bytes"])),
        "draftCPercent": number(n(&["categories", "draft_c", "percent_of_executable"])),
        "provenAsmBytes": done.map(|d| d.common_asm + d.game_asm),
        "doneBytes": done.map(|d| d.bytes()),
        "donePercent": done.map(|d| number(d.percent())),
        "historicalTargets": 12, "fullTargets": 1, "compileOnlyTargets": 11
    });
    Ok(Live {
        revision,
        generated: iso_now(),
        trees,
        map: None,
        summary,
    })
}
fn iso_now() -> String {
    let ms = SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .map_or(0, |duration| duration.as_millis() as i64);
    let days = ms.div_euclid(86_400_000);
    let rest = ms.rem_euclid(86_400_000);
    let z = days + 719468;
    let era = z.div_euclid(146097);
    let doe = z.rem_euclid(146097);
    let yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    let year = yoe + era * 400;
    let day_of_year = doe - (365 * yoe + yoe / 4 - yoe / 100);
    let month_part = (5 * day_of_year + 2) / 153;
    let day = day_of_year - (153 * month_part + 2) / 5 + 1;
    let month = if month_part < 10 {
        month_part + 3
    } else {
        month_part - 9
    };
    let year = if month <= 2 { year + 1 } else { year };
    format!(
        "{year:04}-{month:02}-{day:02}T{:02}:{:02}:{:02}.{:03}Z",
        rest / 3_600_000,
        rest / 60_000 % 60,
        rest / 1000 % 60,
        rest % 1000
    )
}
fn number(value: f64) -> Value {
    serde_json::from_str(&crate::coverage::jsnum::number(value)).unwrap_or(Value::Null)
}
fn snapshot_from(state: &State) -> Value {
    let mut document = json!({
            "page": page_version(),
            "revision": state.coverage.as_ref().map_or("starting", |live| live.revision.as_str()),
            "generatedAt": state.coverage.as_ref().map(|live| live.generated.as_str()),
            "scanning": state.scanning,
            "trees": TREES.into_iter()
                .map(|(key, title)| (key.into(), Value::String(title.into())))
                .collect::<Map<_, _>>(),
            "project": {
                "title": "Golden Sun · The Lost Age",
                "tbs": "ja · en · de · es · fr · it",
                "tla": "ja · en · de · es · fr · it",
                "fullTarget": "tbs-en"
            }
    });
    let object = document.as_object_mut().expect("dashboard snapshot object");
    if let Some(error) = &state.error {
        object.insert("error".into(), json!(error));
    }
    if let Some(live) = &state.coverage {
        object.insert("hasCharts".into(), json!(!live.trees.is_empty()));
        let published = live.map.is_none() || state.error.is_some();
        object.insert("published".into(), json!(published));
        if !published {
            object.insert("summary".into(), live.summary.clone());
        }
    }
    document
}
fn snapshot() -> Value {
    state(|state| snapshot_from(state))
}
fn snapshot_text() -> String {
    serde_json::to_string(&snapshot()).expect("dashboard snapshot serializes")
}
fn event_stream() -> Response {
    Response::new(
        200,
        "OK",
        Some("text/event-stream; charset=utf-8"),
        "no-store",
        format!("event: update\ndata: {}\nretry: 1000\n\n", snapshot_text()),
    )
}
fn rebuild() -> bool {
    state(|state| state.scanning = true);
    let result = compute();
    state(|state| {
        let succeeded = result.is_ok();
        match result {
            Ok(coverage) => {
                state.coverage = Some(coverage);
                state.error = None;
            }
            Err(error) => {
                state.error = Some(error);
            }
        }
        state.scanning = false;
        succeeded
    })
}

fn reveal_path(repo: &Path, encoded: &str) -> Result<PathBuf, String> {
    let mut bytes = Vec::new();
    let mut input = encoded.bytes();
    while let Some(byte) = input.next() {
        bytes.push(if byte == b'%' {
            let high = input.next().and_then(|b| (b as char).to_digit(16));
            let low = input.next().and_then(|b| (b as char).to_digit(16));
            match (high, low) {
                (Some(high), Some(low)) => (high * 16 + low) as u8,
                _ => return Err("Invalid source path".into()),
            }
        } else {
            byte
        });
    }
    let source = String::from_utf8(bytes).map_err(|_| "Invalid source path")?;
    let path = Path::new(&source);
    if source.is_empty()
        || !path
            .components()
            .all(|part| matches!(part, Component::Normal(_)))
    {
        return Err("Source must be inside this repository".into());
    }
    let repo = repo.canonicalize().map_err(|e| e.to_string())?;
    let path = repo
        .join(path)
        .canonicalize()
        .map_err(|_| "Item is not on disk")?;
    if !path.starts_with(repo) || !(path.is_file() || path.is_dir()) {
        return Err("Source must be inside this repository".into());
    }
    Ok(path)
}
fn reveal(path: &str) -> Response {
    let Some(encoded) = path.strip_prefix("/reveal/") else {
        return http::not_found();
    };
    let result = reveal_path(&root(), encoded).and_then(|path| {
        let mut command = if cfg!(target_os = "macos") {
            let mut command = Command::new("open");
            command.arg("-R").arg(&path);
            command
        } else if cfg!(target_os = "windows") {
            let mut command = Command::new("explorer.exe");
            command.arg(format!("/select,{}", path.display()));
            command
        } else {
            let mut command = Command::new("xdg-open");
            command.arg(if path.is_dir() {
                path.as_path()
            } else {
                path.parent().expect("repository file parent")
            });
            command
        };
        command
            .output()
            .map_err(|e| e.to_string())
            .and_then(|output| {
                if output.status.success() {
                    Ok(())
                } else {
                    Err("Could not open file browser".into())
                }
            })
    });
    let (status, reason, message) = match result {
        Ok(()) => (200, "OK", "Shown in Finder".into()),
        Err(message) => (400, "Bad Request", message),
    };
    Response::new(status, reason, Some("text/html; charset=utf-8"), "no-store",
        format!("<!doctype html><style>body{{margin:0;background:#1f7f93;color:white;font:14px monospace}}</style>{}", crate::coverage::boxtree::esc(&message)))
}
fn response(path: &str) -> Response {
    if path == "/"
        || path.starts_with("/view/")
        || path.starts_with("/inspect/")
        || path.starts_with("/shared/")
    {
        return page(path);
    }
    match path {
        "/snapshot" => Response::new(
            200,
            "OK",
            Some("application/json"),
            "no-store",
            snapshot_text(),
        ),
        "/events" => event_stream(),
        path if path.starts_with("/svg/") => {
            let mut parts = path[5..].split('/');
            let id = parts.next().unwrap_or("");
            let width = match parts.next() {
                None => None,
                Some(value) => match value.split_once('x').map_or_else(
                    || value.parse::<u16>().map(|w| (w, 0)),
                    |(w, h)| {
                        w.parse::<u16>()
                            .and_then(|w| h.parse::<u16>().map(|h| (w, h)))
                    },
                ) {
                    Ok((width @ 240..=10000, height @ 0..=10000)) => Some((width, height)),
                    _ => {
                        return Response::new(
                            400,
                            "Bad Request",
                            None,
                            "no-store",
                            b"Invalid chart width".to_vec(),
                        )
                    }
                },
            };
            let folder = parts.collect::<Vec<_>>().join("/");
            if (!folder.is_empty()
                && (!folder.ends_with('/') || folder.split('/').any(|part| part == "..")))
                || !TREES.iter().any(|(key, _)| *key == id)
            {
                return Response::new(
                    404,
                    "Not Found",
                    None,
                    "no-store",
                    b"Unknown chart".to_vec(),
                );
            }
            state(|s| {
                s.coverage
                    .as_ref()
                    .and_then(|c| c.chart_at(id, width, &folder))
                    .ok_or_else(|| {
                        s.error
                            .clone()
                            .unwrap_or_else(|| "Coverage is still being read".into())
                    })
                    .map(|s| {
                        Response::new(
                            200,
                            "OK",
                            Some("image/svg+xml; charset=utf-8"),
                            "no-store",
                            s.into_bytes(),
                        )
                    })
                    .unwrap_or_else(|e| {
                        Response::new(
                            503,
                            "Service Unavailable",
                            Some("text/plain; charset=utf-8"),
                            "no-store",
                            e.into_bytes(),
                        )
                    })
            })
        }
        _ => Response::new(
            404,
            "Not Found",
            Some("text/plain; charset=utf-8"),
            "no-store",
            b"Not found".to_vec(),
        ),
    }
}

#[derive(Clone, PartialEq)]
struct Fingerprint {
    newest: f64,
    entries: usize,
}
fn fingerprint(path: &Path) -> Fingerprint {
    let mut out = Fingerprint {
        newest: 0.0,
        entries: 0,
    };
    for entry in WalkDir::new(path).into_iter().filter_map(Result::ok) {
        let Ok(metadata) = std::fs::symlink_metadata(entry.path()) else {
            continue;
        };
        out.entries += 1;
        out.newest = out.newest.max(
            metadata
                .modified()
                .ok()
                .and_then(|time| time.duration_since(UNIX_EPOCH).ok())
                .map_or(0.0, |duration| duration.as_secs_f64() * 1000.0),
        );
    }
    out
}
struct Watcher {
    coverage: Vec<(PathBuf, Fingerprint)>,
    dirty: bool,
    stable_ticks: u8,
}
impl Watcher {
    fn new(retry_initial_scan: bool) -> Self {
        let r = root();
        let mut coverage = COVERAGE_DIRS
            .iter()
            .map(|d| r.join(d))
            .map(|p| (p.clone(), fingerprint(&p)))
            .collect::<Vec<_>>();
        for p in [
            "out/tbs-en/full/asm/manifest.json",
            "out/tbs-en/full/assets/manifest.json",
            "out/decomp/diagnose/.revision",
        ]
        .iter()
        .map(|p| r.join(p))
        {
            coverage.push((p.clone(), fingerprint(&p)))
        }
        Self {
            coverage,
            dirty: retry_initial_scan,
            stable_ticks: 0,
        }
    }
    fn tick(&mut self) {
        let mut changed = false;
        for (p, old) in &mut self.coverage {
            let now = fingerprint(p);
            changed |= now != *old;
            *old = now
        }
        if changed {
            self.dirty = true;
            self.stable_ticks = 0;
        } else if self.dirty {
            self.stable_ticks = self.stable_ticks.saturating_add(1);
            if self.stable_ticks >= 2 {
                if rebuild() {
                    self.dirty = false;
                }
                self.stable_ticks = 0;
            }
        }
    }
}

fn page(path: &str) -> Response {
    let (encoded, selected, shared) = if let Some(path) = path.strip_prefix("/inspect/") {
        let Some((address, folder)) = path.split_once('/') else {
            return http::not_found();
        };
        let Ok(address) = i64::from_str_radix(address, 16) else {
            return http::not_found();
        };
        (folder, Some(address), false)
    } else if let Some(folder) = path.strip_prefix("/shared/") {
        (folder, None, true)
    } else {
        (path.strip_prefix("/view/").unwrap_or(""), None, false)
    };
    let Some(folder) = crate::coverage::boxtree::decode_folder(encoded) else {
        return http::not_found();
    };
    let content = state(|s| {
        s.coverage
            .as_ref()
            .and_then(|live| live.map.as_ref())
            .map(|map| crate::coverage::boxtree::html_page(map, &folder, selected, shared))
    });
    let (refresh, content) = match content {
        Some(Some(content)) => ("", content),
        Some(None) => return http::not_found(),
        None => (
            "<meta http-equiv=\"refresh\" content=\"2\">",
            "<main class=\"loading\">Reading ROM coverage…</main>".into(),
        ),
    };
    let mut response=Response::new(200,"OK",Some("text/html; charset=utf-8"),"no-store",format!("<!doctype html><html lang=\"en\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">{refresh}<title>Alchemy</title><style>{STYLES}</style></head><body>{content}</body></html>"));
    response.headers.push(("Content-Security-Policy","default-src 'self'; script-src 'none'; style-src 'self' 'unsafe-inline'; font-src 'self'; object-src 'none'; base-uri 'none'".into()));
    response
}
pub fn entry(args: &[String]) -> Result<(), String> {
    let Some(bind) = http::bind(args, "dashboard", 4650)? else {
        return Ok(());
    };
    let listener = TcpListener::bind(bind).map_err(|e| e.to_string())?;
    if let Ok(coverage) = cached() {
        state(|state| state.coverage = Some(coverage));
    }
    std::thread::spawn(|| {
        let retry = !rebuild();
        let mut watcher = Watcher::new(retry);
        loop {
            std::thread::sleep(Duration::from_secs(1));
            watcher.tick();
        }
    });
    println!(
        "Alchemy dashboard on http://{}/",
        listener.local_addr().map_err(|e| e.to_string())?
    );
    http::run(listener, response, reveal).map_err(|e| e.to_string())
}
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn reveal_resolves_encoded_files_and_folders_but_never_escapes_repository() {
        let repo = tempfile::tempdir().unwrap();
        std::fs::create_dir(repo.path().join("ART")).unwrap();
        std::fs::write(repo.path().join("ART/My sheet.PNG"), b"fixture").unwrap();
        assert_eq!(
            reveal_path(repo.path(), "ART%2FMy%20sheet.PNG").unwrap(),
            repo.path().join("ART/My sheet.PNG").canonicalize().unwrap()
        );
        assert!(reveal_path(repo.path(), "ART%2F").unwrap().is_dir());
        for bad in [
            "",
            "%",
            "%GG",
            "%FF",
            "%2Fetc%2Fpasswd",
            "..%2FREADME.md",
            "ART%2F..%2FREADME.md",
            "missing",
        ] {
            assert!(reveal_path(repo.path(), bad).is_err(), "{bad}");
        }
        #[cfg(unix)]
        {
            let outside = tempfile::tempdir().unwrap();
            std::os::unix::fs::symlink(outside.path(), repo.path().join("escape")).unwrap();
            assert!(reveal_path(repo.path(), "escape").is_err());
        }
        assert_eq!(response("/reveal/ART").status, 404);
        assert_eq!(reveal("/other").status, 404);
    }

    #[test]
    fn published_charts_survive_missing_reports_without_claiming_live_progress() {
        let live = live_from(Value::Null, vec![("code", "<svg/>".into())]).unwrap();
        assert_eq!(
            live.chart_at("code", Some((800, 0)), "").as_deref(),
            Some("<svg/>")
        );
        let snapshot = snapshot_from(&State {
            coverage: Some(live),
            error: Some("missing build manifest".into()),
            scanning: false,
        });
        assert_eq!(snapshot["hasCharts"], true);
        assert_eq!(snapshot["published"], true);
        assert!(snapshot.get("summary").is_none());
        assert_eq!(snapshot["error"], "missing build manifest");
    }

    #[test]
    fn summary_comes_only_from_coverage_document() {
        let live = live_from(
            json!({"done":{"executable":1000,"common_c":0,"game_c":400,
                "common_asm":0,"game_asm":100},"executable_bytes":1000,"categories":{
                "proven_c":{"bytes":400,"percent_of_executable":40},
                "proven_asm":{"bytes":100},"draft_c":{"bytes":200,"percent_of_executable":20}
            }}),
            Vec::new(),
        )
        .unwrap();
        assert_eq!(live.summary["donePercent"], 50);
        assert_eq!(live.summary["doneBytes"], 500);
        assert_eq!(live.summary["provenCBytes"], 400);
        assert_eq!(live.summary["draftCBytes"], 200);
        assert!(live.summary.get("correspondenceAvailable").is_none());
        assert!(live.summary.get("tbsJaSources").is_none());
    }

    #[test]
    fn old_map_without_a_verified_score_does_not_invent_done() {
        let live = live_from(
            json!({"executable_bytes":1000,
            "categories":{"proven_c":{"bytes":1000}}}),
            Vec::new(),
        )
        .unwrap();
        assert!(live.summary["donePercent"].is_null());
        assert!(live.summary["doneBytes"].is_null());
    }

    #[test]
    fn snapshot_preserves_javascript_numbers_and_omits_absent_fields() {
        assert_eq!(number(1.0), json!(1));
        let snapshot = snapshot_from(&State::default());
        assert_eq!(snapshot["revision"], "starting");
        assert!(snapshot["generatedAt"].is_null());
        for field in ["error", "summary"] {
            assert!(snapshot.get(field).is_none(), "unexpected {field}");
        }
    }

    #[test]
    fn snapshot_and_event_stream_preserve_public_compatibility() {
        let state = State {
            coverage: Some(Live {
                revision: "revision".into(),
                generated: "2001-08-27T12:34:56.789Z".into(),
                trees: Vec::new(),
                map: None,
                summary: json!({"donePercent":100}),
            }),
            error: None,
            scanning: false,
        };
        assert_eq!(
            snapshot_from(&state)["generatedAt"],
            "2001-08-27T12:34:56.789Z"
        );
        let response = event_stream();
        assert!(
            response
                .headers
                .iter()
                .any(|(name, value)| *name == "Content-Type"
                    && value.starts_with("text/event-stream"))
        );
        assert!(String::from_utf8(response.body)
            .unwrap()
            .starts_with("event: update\ndata: {"));
    }

    #[test]
    fn dashboard_has_no_playback_routes_or_client() {
        assert_eq!(response("/").status, 200);
        for path in [
            "/music/catalog",
            "/music/soundfont",
            "/music/tla/soundfont",
            "/music/bgm_000.mid",
        ] {
            assert_eq!(response(path).status, 404);
        }
        assert_eq!(response("/client.js").status, 404);
        let page = response("/");
        assert!(!String::from_utf8(page.body).unwrap().contains("<script"));
        assert!(page
            .headers
            .iter()
            .any(|(key, value)| *key == "Content-Security-Policy"
                && value.contains("script-src 'none'")));
        assert!(!STYLES.contains(".music-player"));
    }
    #[test]
    fn dashboard_serves_no_font_and_styles_labels_like_the_figure() {
        assert_eq!(response("/weyard.otf").status, 404);
        assert!(!STYLES.contains("@font-face"));
        assert!(!STYLES.contains("url("));
        assert!(STYLES.contains("13px/20px -apple-system"));
    }
    #[test]
    fn navigation_rejects_invalid_paths() {
        for path in [
            "/view/ff",
            "/view/2e2e2f",
            "/inspect/not-an-address/",
            "/inspect/42/ff",
        ] {
            assert_eq!(response(path).status, 404);
        }
    }
}
