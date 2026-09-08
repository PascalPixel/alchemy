//! Live coverage only. Music playback is a separate optional process.
use super::http::{self, root, Response};
use crate::coverage::{
    boxtree::{render_box_trees, svg_cache_version, BOX_TREES},
    pipeline::{build_coverage_map, BuildOptions},
    tree::work_tree_at,
};
use serde_json::{json, Map, Value};
use std::{
    net::TcpListener,
    path::{Path, PathBuf},
    sync::Mutex,
    time::{Duration, SystemTime, UNIX_EPOCH},
};
use walkdir::WalkDir;
const CLIENT: &str = include_str!("client.js");
const STYLES: &str = include_str!("style.css");
const TREES: [(&str, &str); 4] = [
    ("core", "Main image"),
    ("overlays", "Code overlays"),
    ("images", "Graphics"),
    ("music", "Sound"),
];
const COVERAGE_DIRS: [&str; 12] = [
    "games/gs1/asm",
    "games/gs1/assets",
    "games/gs1/metrics",
    "games/gs1/semantic",
    "games/gs1/src",
    "games/gs1/source-paths.json",
    "games/gs1/recon",
    "games/gs1/project.json",
    "games/gs2",
    "games/gs2/project.json",
    "games/alchemy",
    "out/gs1-en/reports",
];
fn page_version() -> String {
    svg_cache_version(&format!("{STYLES}\0{CLIENT}"))
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
    let mut v = document;
    for key in path {
        v = v.get(key)?;
    }
    v.as_f64()
}
fn compute() -> Result<Live, String> {
    let tree = work_tree_at(root());
    let map = build_coverage_map(&BuildOptions {
        target: "gs1-en".into(),
        exact: &tree,
        recon: Some(&tree),
    })?;
    let trees = render_box_trees(&map);
    let mut live = live_from(map.document.clone(), trees)?;
    live.map = Some(map);
    Ok(live)
}
fn cached() -> Result<Live, String> {
    let report = root().join("out/gs1-en/reports/coverage-map.json");
    let document = serde_json::from_slice(
        &std::fs::read(&report).map_err(|error| format!("{}: {error}", report.display()))?,
    )
    .map_err(|error| format!("{}: {error}", report.display()))?;
    let trees = BOX_TREES
        .iter()
        .map(|name| {
            let path = root().join(format!("games/gs1/assets/readme/gs1-en-{name}.svg"));
            std::fs::read_to_string(&path)
                .map(|svg| (*name, svg))
                .map_err(|error| format!("{}: {error}", path.display()))
        })
        .collect::<Result<Vec<_>, _>>()?;
    live_from(document, trees)
}
fn live_from(document: Value, trees: Vec<(&'static str, String)>) -> Result<Live, String> {
    let revision = trees
        .iter()
        .map(|(_, svg)| svg_cache_version(svg))
        .collect::<Vec<_>>()
        .join("-");
    let n = |key| document_number(&document, key).unwrap_or(0.0);
    let executable = n(&["executable_bytes"]);
    let proven_c = n(&["categories", "proven_c", "bytes"]);
    let proven_asm = n(&["categories", "proven_asm", "bytes"]);
    let summary = json!({
        "executableBytes": number(executable),
        "provenCBytes": number(proven_c),
        "provenCPercent": number(n(&["categories", "proven_c", "percent_of_executable"])),
        "draftCBytes": number(n(&["categories", "draft_c", "bytes"])),
        "draftCPercent": number(n(&["categories", "draft_c", "percent_of_executable"])),
        "provenAsmBytes": number(proven_asm),
        "doneBytes": number(proven_c + proven_asm),
        "donePercent": number((proven_c + proven_asm) * 100.0 / executable.max(1.0)),
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
                "gs1": "ja · en · de · es · fr · it",
                "gs2": "ja · en · de · es · fr · it",
                "fullTarget": "gs1-en",
                "integration": "Alchemy"
            }
    });
    let object = document.as_object_mut().expect("dashboard snapshot object");
    if let Some(error) = &state.error {
        object.insert("error".into(), json!(error));
    }
    if let Some(live) = &state.coverage {
        object.insert("summary".into(), live.summary.clone());
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

fn response(path: &str) -> Response {
    match path {
        "/" => http::shell("Alchemy", STYLES),
        "/client.js" => Response::new(
            200,
            "OK",
            Some("text/javascript; charset=utf-8"),
            "no-store",
            CLIENT,
        ),
        "/weyard.otf" => http::font(),
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
                Some(value) => match value.parse::<u16>() {
                    Ok(width @ 240..=2000) => Some(width),
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
            if parts.next().is_some() || !TREES.iter().any(|(key, _)| *key == id) {
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
                    .and_then(|c| match width {
                        Some(width) => c
                            .map
                            .as_ref()
                            .map(|map| crate::coverage::boxtree::svg(id, map, f64::from(width))),
                        None => c
                            .trees
                            .iter()
                            .find(|(k, _)| *k == id)
                            .map(|(_, v)| v.clone()),
                    })
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
            "out/gs1-en/full/asm/manifest.json",
            "out/gs1-en/full/assets/manifest.json",
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
    http::run(listener, response).map_err(|e| e.to_string())
}
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn summary_comes_only_from_coverage_document() {
        let live = live_from(
            json!({"executable_bytes":1000,"categories":{
                "proven_c":{"bytes":400,"percent_of_executable":40},
                "proven_asm":{"bytes":100},"draft_c":{"bytes":200,"percent_of_executable":20}
            }}),
            Vec::new(),
        )
        .unwrap();
        assert_eq!(live.summary["donePercent"], 50);
        assert_eq!(live.summary["provenCBytes"], 400);
        assert_eq!(live.summary["draftCBytes"], 200);
        assert!(live.summary.get("correspondenceAvailable").is_none());
        assert!(live.summary.get("gs1JaSources").is_none());
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
            "/music/gs2/soundfont",
            "/music/bgm_000.mid",
        ] {
            assert_eq!(response(path).status, 404);
        }
        for token in [
            "AudioContext",
            "musicPlayer",
            "/music/",
            "createBufferSource",
        ] {
            assert!(!CLIENT.contains(token));
        }
        assert!(CLIENT.contains("EventSource"));
        assert!(!STYLES.contains(".music-player"));
    }
    #[test]
    fn browser_regressions() {
        assert!(std::process::Command::new("bun")
            .args([
                "test",
                concat!(env!("CARGO_MANIFEST_DIR"), "/src/dashboard/client.test.js")
            ])
            .status()
            .expect("Bun is required for dashboard tests")
            .success());
    }
}
