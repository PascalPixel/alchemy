//! Small, dependency-light live dashboard server.

pub mod assets;
pub mod client;

use coverage_map::{
    boxtree::{render_box_trees, svg_cache_version, BOX_TREES},
    pipeline::{build_coverage_map, BuildOptions, CoverageMap},
    tree::work_tree_at,
};
use serde_json::{json, Map, Value};
use std::{
    io::{BufRead, BufReader, Write},
    net::{IpAddr, Ipv4Addr, SocketAddr, TcpListener, TcpStream},
    path::{Path, PathBuf},
    sync::Mutex,
    time::{Duration, SystemTime, UNIX_EPOCH},
};
use walkdir::WalkDir;

const TREES: [(&str, &str); 4] = [
    ("core", "Main image"),
    ("overlays", "Code overlays"),
    ("images", "Images"),
    ("music", "Music"),
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

pub mod cli {
    pub fn entry(args: &[String]) {
        let mut bind = None;
        let mut i = 0;
        while i < args.len() {
            match args[i].as_str() {
                "-h" | "--help" => {
                    return println!("Usage: dashboard-server [--bind HOST:PORT] [--self-test]")
                }
                "--self-test" if i + 1 == args.len() => {
                    return match crate::self_test() {
                        Ok(s) => println!("{s}"),
                        Err(e) => {
                            eprintln!("error: {e}");
                            std::process::exit(1);
                        }
                    }
                }
                "--bind" if i + 1 < args.len() => {
                    bind = args.get(i + 1).and_then(|s| s.parse().ok());
                    i += 1;
                }
                value if value.starts_with("--bind=") => bind = value[7..].parse().ok(),
                value => {
                    eprintln!("error: unknown argument {value:?}");
                    std::process::exit(2);
                }
            }
            i += 1;
        }
        if let Err(e) = crate::run(bind) {
            eprintln!("error: {e}");
            std::process::exit(1);
        }
    }
}

fn root() -> PathBuf {
    std::env::var_os("ALCHEMY_DASHBOARD_ROOT")
        .map(PathBuf::from)
        .filter(|p| !p.as_os_str().is_empty())
        .unwrap_or_else(|| {
            Path::new(env!("CARGO_MANIFEST_DIR"))
                .parent()
                .unwrap()
                .parent()
                .unwrap()
                .into()
        })
}
fn font() -> PathBuf {
    root().join("games/gs1/assets/fonts/weyard.otf")
}
fn page_version() -> String {
    let client = client::bundled_client().unwrap_or_default();
    coverage_map::sha1::sha1_hex(format!("{}\0{client}", assets::STYLES).as_bytes())[..16].into()
}
fn port() -> u16 {
    std::env::var("ALCHEMY_DASHBOARD_PORT")
        .ok()
        .and_then(|v| v.parse().ok())
        .unwrap_or(4649)
}
fn host() -> IpAddr {
    std::env::var("ALCHEMY_DASHBOARD_HOST")
        .ok()
        .and_then(|v| v.parse().ok())
        .unwrap_or(IpAddr::V4(Ipv4Addr::UNSPECIFIED))
}

pub struct Live {
    revision: String,
    generated: String,
    trees: Vec<(&'static str, String)>,
    executable: f64,
    proven_c: f64,
    proven_c_percent: f64,
    draft_c: f64,
    draft_c_percent: f64,
    proven_asm: f64,
    gs1_ja_sources: usize,
    gs1_en_sources: usize,
    gs2_ja_sources: usize,
    gs2_en_sources: usize,
    correspondence: Option<Correspondence>,
}

struct Correspondence {
    total: usize,
    matched: usize,
    shared: usize,
    regional: usize,
    unresolved: usize,
}
impl Correspondence {
    fn add(self, other: Self) -> Self {
        Self {
            total: self.total + other.total,
            matched: self.matched + other.matched,
            shared: self.shared + other.shared,
            regional: self.regional + other.regional,
            unresolved: self.unresolved + other.unresolved,
        }
    }
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
fn map_number(map: &CoverageMap, path: &[&str]) -> Option<f64> {
    let mut v = &map.document;
    for key in path {
        v = v.get(key)?;
    }
    v.as_f64()
}
fn count_c(path: &Path) -> usize {
    WalkDir::new(path)
        .into_iter()
        .filter_map(Result::ok)
        .filter(|entry| {
            entry.file_type().is_file() && entry.path().extension().is_some_and(|x| x == "c")
        })
        .count()
}
fn correspondence(path: &Path) -> Result<Option<Correspondence>, String> {
    let bytes = match std::fs::read(path) {
        Ok(bytes) => bytes,
        Err(error) if error.kind() == std::io::ErrorKind::NotFound => return Ok(None),
        Err(error) => return Err(format!("{}: {error}", path.display())),
    };
    let value: serde_json::Value =
        serde_json::from_slice(&bytes).map_err(|error| format!("{}: {error}", path.display()))?;
    let number = |key: &str| {
        value
            .get(key)
            .and_then(serde_json::Value::as_u64)
            .map(|value| value as usize)
            .ok_or_else(|| format!("{} lacks numeric {key}", path.display()))
    };
    Ok(Some(Correspondence {
        total: number("owners_total")?,
        matched: number("matched_owners")?,
        shared: number("shared_core_owners")?,
        regional: number("regional_core_owners")?,
        unresolved: number("unresolved_owners")?,
    }))
}
fn compute() -> Result<Live, String> {
    let tree = work_tree_at(root());
    let map = build_coverage_map(&BuildOptions {
        target: "gs1-en".into(),
        exact: &tree,
        recon: Some(&tree),
        prefer_verified_assets: true,
    })?;
    let trees = render_box_trees(&map, Some(&tree), true)?;
    let gs1_ja_sources = count_c(&root().join("games/gs1/recon/ja"));
    let gs1_en_sources = count_c(&root().join("games/gs1/recon/en"));
    let gs2_ja_sources = count_c(&root().join("games/gs2/recon/ja"));
    let gs2_en_sources = count_c(&root().join("games/gs2/recon/en"));
    let reports = root().join("out/gs1-en/reports");
    let correspondence = match (
        correspondence(&reports.join("exact-correspondence.json"))?,
        correspondence(&reports.join("exact-overlay-correspondence.json"))?,
    ) {
        (Some(main), Some(overlays)) => Some(main.add(overlays)),
        _ => None,
    };
    let correspondence_revision = correspondence.as_ref().map_or_else(
        || "correspondence-unavailable".into(),
        |correspondence| {
            format!(
                "{}-{}-{}-{}",
                correspondence.matched,
                correspondence.shared,
                correspondence.regional,
                correspondence.unresolved
            )
        },
    );
    let revision = BOX_TREES
        .iter()
        .map(|name| {
            svg_cache_version(
                trees
                    .iter()
                    .find(|(id, _)| id == name)
                    .map_or("", |(_, s)| s),
            )
        })
        .collect::<Vec<_>>()
        .into_iter()
        .chain([
            gs1_ja_sources.to_string(),
            gs1_en_sources.to_string(),
            gs2_ja_sources.to_string(),
            gs2_en_sources.to_string(),
            correspondence_revision,
        ])
        .collect::<Vec<_>>()
        .join("-");
    let n = |key| map_number(&map, key).unwrap_or(0.0);
    Ok(Live {
        revision,
        generated: iso_now(),
        trees,
        executable: n(&["executable_bytes"]),
        proven_c: n(&["categories", "proven_c", "bytes"]),
        proven_c_percent: n(&["categories", "proven_c", "percent_of_executable"]),
        draft_c: n(&["categories", "draft_c", "bytes"]),
        draft_c_percent: n(&["categories", "draft_c", "percent_of_executable"]),
        proven_asm: n(&["categories", "proven_asm", "bytes"]),
        gs1_ja_sources,
        gs1_en_sources,
        gs2_ja_sources,
        gs2_en_sources,
        correspondence,
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
    serde_json::from_str(&coverage_map::jsnum::js_number_string(value)).unwrap_or(Value::Null)
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
        let mut summary = json!({
                "executableBytes": number(live.executable),
                "provenCBytes": number(live.proven_c),
                "provenCPercent": number(live.proven_c_percent),
                "draftCBytes": number(live.draft_c),
                "draftCPercent": number(live.draft_c_percent),
                "provenAsmBytes": number(live.proven_asm),
                "doneBytes": number(live.proven_c + live.proven_asm),
                "donePercent": number((live.proven_c + live.proven_asm) * 100.0 / live.executable.max(1.0)),
                "gs1JaSources": live.gs1_ja_sources,
                "gs1EnSources": live.gs1_en_sources,
                "gs2JaSources": live.gs2_ja_sources,
                "gs2EnSources": live.gs2_en_sources,
                "historicalTargets": 12,
                "fullTargets": 1,
                "compileOnlyTargets": 11,
                "correspondenceAvailable": live.correspondence.is_some()
        });
        if let Some(value) = &live.correspondence {
            summary
                .as_object_mut()
                .expect("dashboard summary object")
                .extend([
                    ("correspondenceTotal".into(), json!(value.total)),
                    ("correspondenceMatched".into(), json!(value.matched)),
                    ("correspondenceShared".into(), json!(value.shared)),
                    ("correspondenceRegional".into(), json!(value.regional)),
                    ("correspondenceUnresolved".into(), json!(value.unresolved)),
                ]);
        }
        object.insert("summary".into(), summary);
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
fn rebuild() {
    state(|state| state.scanning = true);
    let result = compute();
    state(|state| {
        match result {
            Ok(coverage) => {
                state.coverage = Some(coverage);
                state.error = None;
            }
            Err(error) => state.error = Some(error),
        }
        state.scanning = false;
    })
}

struct Response {
    status: u16,
    reason: &'static str,
    headers: Vec<(&'static str, String)>,
    body: Vec<u8>,
}
impl Response {
    fn new(
        status: u16,
        reason: &'static str,
        ty: Option<&'static str>,
        cache: &'static str,
        body: impl Into<Vec<u8>>,
    ) -> Self {
        let mut headers = vec![("Cache-Control", cache.into())];
        if let Some(ty) = ty {
            headers.insert(0, ("Content-Type", ty.into()))
        }
        Self {
            status,
            reason,
            headers,
            body: body.into(),
        }
    }
    fn write(self, stream: &mut TcpStream, body: bool) -> std::io::Result<()> {
        let mut h = format!("HTTP/1.1 {} {}\r\n", self.status, self.reason);
        for (k, v) in self.headers {
            h.push_str(&format!("{k}: {v}\r\n"))
        }
        h.push_str(&format!(
            "Content-Length: {}\r\nConnection: close\r\n\r\n",
            self.body.len()
        ));
        stream.write_all(h.as_bytes())?;
        if body {
            stream.write_all(&self.body)?
        }
        Ok(())
    }
}
fn request(stream: &TcpStream) -> Result<(String, String), &'static str> {
    let mut r = BufReader::new(stream);
    let mut line = String::new();
    if r.read_line(&mut line).map_err(|_| "read failed")? == 0 {
        return Err("empty request");
    }
    let parts = line.trim_end().split(' ').collect::<Vec<_>>();
    if parts.len() != 3 {
        return Err("bad request");
    }
    let method = parts[0].to_string();
    let target = parts[1].to_string();
    let version = parts[2];
    if !version.starts_with("HTTP/1.") {
        return Err("bad request");
    }
    loop {
        line.clear();
        if r.read_line(&mut line).map_err(|_| "read failed")? == 0 {
            return Err("headers incomplete");
        }
        if line == "\r\n" || line == "\n" {
            break;
        }
    }
    Ok((method, target.split('?').next().unwrap_or(&target).into()))
}
fn response(path: &str) -> Response {
    match path {
        "/" => Response::new(
            200,
            "OK",
            Some("text/html; charset=utf-8"),
            "no-store",
            format!("<!doctype html><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><title>Alchemy</title><link rel=\"stylesheet\" href=\"/styles.css?v={}\"><script type=\"module\" src=\"/client.js?v={}\"></script>", page_version(), page_version())
                .into_bytes(),
        ),
        "/styles.css" => Response::new(200, "OK", Some("text/css; charset=utf-8"), "no-store", assets::STYLES.as_bytes()),
        "/client.js" => Response::new(200, "OK", Some("text/javascript; charset=utf-8"), "no-store", client::bundled_client().unwrap().into_bytes()),
        "/snapshot" => Response { status: 200, reason: "OK", headers: vec![("Cache-Control", "no-store".into()), ("Content-Type", "application/json;charset=utf-8".into())], body: snapshot_text().into_bytes() },
        "/events" => event_stream(),
        "/weyard.otf" => Response::new(200, "OK", Some("font/otf"), "public, max-age=300", std::fs::read(font()).unwrap_or_default()),
        path if path.starts_with("/svg/") && TREES.iter().any(|(id, _)| path == format!("/svg/{id}")) => {
            let id = &path[5..];
            state(|s| {
                s.coverage
                    .as_ref()
                    .and_then(|c| c.trees.iter().find(|(k, _)| *k == id).map(|(_, v)| v.clone()))
                    .ok_or_else(|| s.error.clone().unwrap_or_else(|| "Coverage is still being read".into()))
                    .map(|s| Response::new(200, "OK", Some("image/svg+xml; charset=utf-8"), "no-store", s.into_bytes()))
                    .unwrap_or_else(|e| Response::new(503, "Service Unavailable", Some("text/plain; charset=utf-8"), "no-store", e.into_bytes()))
            })
        }
        _ => Response::new(404, "Not Found", Some("text/plain; charset=utf-8"), "no-store", b"Not found".to_vec()),
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
}
impl Watcher {
    fn new() -> Self {
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
        Self { coverage }
    }
    fn tick(&mut self) {
        let mut changed = false;
        for (p, old) in &mut self.coverage {
            let now = fingerprint(p);
            changed |= now != *old;
            *old = now
        }
        if changed {
            rebuild()
        }
    }
}

fn serve(mut stream: TcpStream) {
    let Ok((method, path)) = request(&stream) else {
        return;
    };
    let include = method != "HEAD";
    if method != "GET" && method != "HEAD" {
        let _ = Response::new(
            405,
            "Method Not Allowed",
            Some("text/plain; charset=utf-8"),
            "no-store",
            b"Method not allowed".to_vec(),
        )
        .write(&mut stream, include);
        return;
    }
    let _ = response(&path).write(&mut stream, include);
}
pub fn run(bind: Option<SocketAddr>) -> std::io::Result<()> {
    rebuild();
    let listener = TcpListener::bind(bind.unwrap_or_else(|| SocketAddr::new(host(), port())))?;
    println!("Alchemy dashboard on http://{}/", listener.local_addr()?);
    std::thread::spawn(|| {
        let mut w = Watcher::new();
        loop {
            std::thread::sleep(Duration::from_secs(1));
            w.tick();
        }
    });
    for stream in listener.incoming().flatten() {
        std::thread::spawn(|| serve(stream));
    }
    Ok(())
}

pub fn self_test() -> Result<String, String> {
    let js = client::bundled_client().map_err(|e| e.to_string())?;
    if !assets::STYLES.contains(".hover-tooltip")
        || !assets::STYLES.contains(".products")
        || !js.contains("EventSource")
        || !js.contains("closest(\"g[aria-label]\")")
        || !js.contains("historicalProduct")
        || !js.contains("compile-only")
        || !js.contains("Run make reports to refresh cross-edition reports")
    {
        return Err("dashboard assets are incomplete".into());
    }
    if !response("/").body.starts_with(b"<!doctype html>") {
        return Err("dashboard shell failed".into());
    }
    if response("/nope").status != 404 {
        return Err("404 route failed".into());
    }
    let events = response("/events");
    if events.status != 200
        || !events
            .headers
            .iter()
            .any(|(name, value)| *name == "Content-Type" && value.starts_with("text/event-stream"))
        || !events.body.starts_with(b"event: update\ndata: {")
    {
        return Err("event stream route failed".into());
    }
    Ok("self-test=ok dashboard".into())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn missing_correspondence_report_is_unavailable() {
        let path = std::env::temp_dir().join(format!(
            "alchemy-missing-correspondence-{}.json",
            std::process::id()
        ));
        assert!(correspondence(&path).unwrap().is_none());
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
                executable: 1.0,
                proven_c: 1.0,
                proven_c_percent: 100.0,
                draft_c: 0.0,
                draft_c_percent: 0.0,
                proven_asm: 0.0,
                gs1_ja_sources: 0,
                gs1_en_sources: 0,
                gs2_ja_sources: 0,
                gs2_en_sources: 0,
                correspondence: None,
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
}
