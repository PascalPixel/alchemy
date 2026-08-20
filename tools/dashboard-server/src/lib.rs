//! Small, dependency-light live dashboard server.

pub mod assets;
pub mod client;

use coverage_map::{
    boxtree::{render_box_trees, svg_cache_version, BOX_TREES},
    pipeline::{build_coverage_map, BuildOptions, CoverageMap},
    tree::work_tree_at,
};
use std::{
    io::{BufRead, BufReader, Write},
    net::{IpAddr, Ipv4Addr, SocketAddr, TcpListener, TcpStream},
    path::{Path, PathBuf},
    sync::{
        mpsc::{Receiver, SyncSender, TrySendError},
        Mutex,
    },
    time::{Duration, SystemTime, UNIX_EPOCH},
};

const TREES: [(&str, &str); 4] = [
    ("core", "Main image"),
    ("overlays", "Code overlays"),
    ("images", "Images"),
    ("music", "Music"),
];
const COVERAGE_DIRS: [&str; 5] = ["asm", "assets", "metrics", "semantic", "exact"];

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

#[derive(Clone, Debug, PartialEq)]
pub enum Json {
    Undefined,
    Bool(bool),
    Num(f64),
    Str(String),
    Obj(Vec<(String, Json)>),
}
impl Json {
    fn str(s: &str) -> Self {
        Self::Str(s.into())
    }
    fn obj(xs: Vec<(&str, Self)>) -> Self {
        Self::Obj(xs.into_iter().map(|(k, v)| (k.into(), v)).collect())
    }
    fn stringify(&self) -> String {
        let mut s = String::new();
        write_json(self, &mut s);
        s
    }
}
fn quote(s: &str) -> String {
    let mut out = String::with_capacity(s.len() + 2);
    out.push('"');
    for c in s.chars() {
        match c {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\u{8}' => out.push_str("\\b"),
            '\u{c}' => out.push_str("\\f"),
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            c if (c as u32) < 32 => out.push_str(&format!("\\u{:04x}", c as u32)),
            c => out.push(c),
        }
    }
    out.push('"');
    out
}
fn write_json(v: &Json, out: &mut String) {
    match v {
        Json::Undefined => out.push_str("null"),
        Json::Bool(b) => out.push_str(if *b { "true" } else { "false" }),
        Json::Num(n) => out.push_str(
            if n.is_finite() {
                coverage_map::jsnum::js_number_string(*n)
            } else {
                "null".into()
            }
            .as_str(),
        ),
        Json::Str(s) => out.push_str(&quote(s)),
        Json::Obj(xs) => {
            out.push('{');
            let mut first = true;
            for (k, v) in xs {
                if matches!(v, Json::Undefined) {
                    continue;
                }
                if !first {
                    out.push(',')
                }
                first = false;
                out.push_str(&quote(k));
                out.push(':');
                write_json(v, out);
            }
            out.push('}')
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
    root().join("assets/fonts/weyard.otf")
}
fn mtime(path: &Path) -> f64 {
    std::fs::metadata(path)
        .and_then(|m| m.modified())
        .ok()
        .and_then(|t| t.duration_since(UNIX_EPOCH).ok())
        .map_or(0.0, |d| {
            d.as_secs() as f64 * 1000.0 + f64::from(d.subsec_nanos()) / 1e6
        })
}
fn page_version() -> String {
    coverage_map::sha1::sha1_hex(assets::STYLES.as_bytes())[..16].into()
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
    exact: f64,
    percent: f64,
    retained: f64,
}
#[derive(Default)]
pub struct State {
    coverage: Option<Live>,
    error: Option<String>,
    scanning: bool,
    queued: bool,
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
fn compute() -> Result<Live, String> {
    let tree = work_tree_at(root());
    let map = build_coverage_map(&BuildOptions {
        target: "gs1-en".into(),
        exact: &tree,
        semantic: Some(&tree),
        validate_tracked_progress: false,
        prefer_verified_assets: true,
    })?;
    let trees = render_box_trees(&map, Some(&tree), true)?;
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
        .join("-");
    let n = |key| map_number(&map, key).unwrap_or(0.0);
    Ok(Live {
        revision,
        generated: iso_now(),
        trees,
        executable: n(&["executable_bytes"]),
        exact: n(&["categories", "exact_c", "bytes"]),
        percent: n(&["categories", "exact_c", "percent_of_executable"]),
        retained: n(&["categories", "retained_asm", "bytes"]),
    })
}
fn iso_now() -> String {
    let ms = SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .map_or(0, |d| d.as_millis() as i64);
    let days = ms.div_euclid(86_400_000);
    let rest = ms.rem_euclid(86_400_000);
    let z = days + 719468;
    let era = z.div_euclid(146097);
    let doe = z.rem_euclid(146097);
    let yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    let y = yoe + era * 400;
    let doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    let mp = (5 * doy + 2) / 153;
    let day = doy - (153 * mp + 2) / 5 + 1;
    let month = if mp < 10 { mp + 3 } else { mp - 9 };
    let year = if month <= 2 { y + 1 } else { y };
    format!(
        "{year:04}-{month:02}-{day:02}T{:02}:{:02}:{:02}.{:03}Z",
        rest / 3_600_000,
        rest / 60_000 % 60,
        rest / 1000 % 60,
        rest % 1000
    )
}
fn snapshot() -> Json {
    state(|s| {
        let trees = Json::Obj(
            TREES
                .iter()
                .map(|(k, v)| ((*k).into(), Json::str(v)))
                .collect(),
        );
        let summary = s.coverage.as_ref().map_or(Json::Undefined, |c| {
            Json::obj(vec![
                ("executableBytes", Json::Num(c.executable)),
                ("exactBytes", Json::Num(c.exact)),
                ("exactPercent", Json::Num(c.percent)),
                ("retainedBytes", Json::Num(c.retained)),
                ("doneBytes", Json::Num(c.exact + c.retained)),
            ])
        });
        Json::obj(vec![
            ("page", Json::str(&page_version())),
            (
                "revision",
                Json::str(
                    &s.coverage
                        .as_ref()
                        .map_or_else(|| "starting".into(), |c| c.revision.clone()),
                ),
            ),
            (
                "generatedAt",
                s.coverage
                    .as_ref()
                    .map_or(Json::Undefined, |c| Json::str(&c.generated)),
            ),
            ("scanning", Json::Bool(s.scanning)),
            ("error", s.error.clone().map_or(Json::Undefined, Json::Str)),
            ("trees", trees),
            ("summary", summary),
        ])
    })
}
fn rebuild() {
    state(|s| {
        if s.scanning {
            s.queued = true
        } else {
            s.scanning = true
        }
    });
    notify();
    loop {
        let result = compute();
        let again = state(|s| {
            match result {
                Ok(v) => {
                    s.coverage = Some(v);
                    s.error = None
                }
                Err(e) => s.error = Some(e),
            }
            let again = s.queued;
            s.queued = false;
            s.scanning = again;
            again
        });
        notify();
        if !again {
            break;
        }
    }
}

struct Subscriber {
    id: u64,
    tx: SyncSender<Vec<u8>>,
}
static SUBSCRIBERS: Mutex<Vec<Subscriber>> = Mutex::new(Vec::new());
static NEXT_ID: Mutex<u64> = Mutex::new(1);
fn frame(event: &str, data: &str) -> Vec<u8> {
    format!("event: {event}\ndata: {data}\n\n").into_bytes()
}
fn register() -> (u64, Receiver<Vec<u8>>) {
    let (tx, rx) = std::sync::mpsc::sync_channel(64);
    let mut n = NEXT_ID.lock().unwrap();
    let id = *n;
    *n += 1;
    SUBSCRIBERS.lock().unwrap().push(Subscriber { id, tx });
    (id, rx)
}
fn unregister(id: u64) {
    SUBSCRIBERS.lock().unwrap().retain(|s| s.id != id)
}
fn notify() {
    let msg = frame("update", &snapshot().stringify());
    SUBSCRIBERS.lock().unwrap().retain(|s| {
        !matches!(
            s.tx.try_send(msg.clone()),
            Err(TrySendError::Disconnected(_)) | Err(TrySendError::Full(_))
        )
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
fn response(path: &str) -> Result<Response, Route> {
    match path { "/" => Ok(Response::new(200, "OK", Some("text/html; charset=utf-8"), "no-store", format!("<!doctype html><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><title>Alchemy</title><link rel=\"stylesheet\" href=\"/styles.css?v={}\"><script type=\"module\" src=\"/client.js?v={}\"></script>", page_version(), page_version()).into_bytes())), "/styles.css" => Ok(Response::new(200, "OK", Some("text/css; charset=utf-8"), "no-store", assets::STYLES.as_bytes())), "/client.js" => Ok(Response::new(200, "OK", Some("text/javascript; charset=utf-8"), "no-store", client::bundled_client().unwrap().into_bytes())), "/snapshot" => Ok(Response { status: 200, reason: "OK", headers: vec![("Cache-Control", "no-store".into()), ("Content-Type", "application/json;charset=utf-8".into())], body: snapshot().stringify().into_bytes() }), "/events" => Err(Route::Events), "/weyard.otf" => Ok(Response::new(200, "OK", Some("font/otf"), "public, max-age=300", std::fs::read(font()).unwrap_or_default())), path if path.starts_with("/svg/") && TREES.iter().any(|(id, _)| path == format!("/svg/{id}")) => { let id = &path[5..]; Ok(state(|s| s.coverage.as_ref().and_then(|c| c.trees.iter().find(|(k, _)| *k == id).map(|(_, v)| v.clone())).ok_or_else(|| s.error.clone().unwrap_or_else(|| "Coverage is still being read".into())).map(|s| Response::new(200, "OK", Some("image/svg+xml; charset=utf-8"), "no-store", s.into_bytes())).unwrap_or_else(|e| Response::new(503, "Service Unavailable", Some("text/plain; charset=utf-8"), "no-store", e.into_bytes())))) }, _ => Err(Route::Response(Response::new(404, "Not Found", Some("text/plain; charset=utf-8"), "no-store", b"Not found".to_vec()))) }
}
enum Route {
    Events,
    Response(Response),
}

#[derive(Clone, PartialEq)]
struct Fingerprint {
    newest: f64,
    entries: usize,
}
fn fingerprint(path: &Path) -> Fingerprint {
    let mut stack = vec![path.to_path_buf()];
    let mut out = Fingerprint {
        newest: 0.0,
        entries: 0,
    };
    while let Some(p) = stack.pop() {
        let Ok(m) = std::fs::symlink_metadata(&p) else {
            continue;
        };
        out.entries += 1;
        out.newest = out.newest.max(mtime(&p));
        if m.is_dir() {
            if let Ok(es) = std::fs::read_dir(p) {
                stack.extend(es.flatten().map(|e| e.path()))
            }
        }
    }
    out
}
struct Watcher {
    coverage: Vec<(PathBuf, Fingerprint)>,
    restart: Vec<(PathBuf, Fingerprint)>,
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
            "out/full/asm/manifest.json",
            "out/full/assets/manifest.json",
            "out/decomp/diagnose/.revision",
        ]
        .iter()
        .map(|p| r.join(p))
        {
            coverage.push((p.clone(), fingerprint(&p)))
        }
        let mut files = Vec::new();
        for d in [
            r.join("tools/dashboard-server/src"),
            r.join("tools/coverage-map/src"),
        ] {
            collect_rs(&d, &mut files)
        }
        files.sort();
        Self {
            coverage,
            restart: files
                .into_iter()
                .map(|p| (p.clone(), fingerprint(&p)))
                .collect(),
        }
    }
    fn tick(&mut self) -> bool {
        for (p, old) in &mut self.restart {
            let now = fingerprint(p);
            if now != *old {
                *old = now;
                return true;
            }
        }
        let mut changed = false;
        for (p, old) in &mut self.coverage {
            let now = fingerprint(p);
            changed |= now != *old;
            *old = now
        }
        if changed {
            rebuild()
        }
        false
    }
}
fn collect_rs(dir: &Path, out: &mut Vec<PathBuf>) {
    if let Ok(es) = std::fs::read_dir(dir) {
        for e in es.flatten() {
            let p = e.path();
            if p.is_dir() {
                collect_rs(&p, out)
            } else if p.extension().is_some_and(|x| x == "rs") {
                out.push(p)
            }
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
    match response(&path) {
        Ok(r) => {
            let _ = r.write(&mut stream, include);
        }
        Err(Route::Response(r)) => {
            let _ = r.write(&mut stream, include);
        }
        Err(Route::Events) => events(stream, include),
    }
}
fn events(mut stream: TcpStream, include: bool) {
    let head = b"HTTP/1.1 200 OK\r\nContent-Type: text/event-stream; charset=utf-8\r\nCache-Control: no-store\r\nConnection: keep-alive\r\nTransfer-Encoding: chunked\r\n\r\n";
    if stream.write_all(head).is_err() || !include {
        return;
    }
    let (id, rx) = register();
    let send = |s: &mut TcpStream, b: &[u8]| -> std::io::Result<()> {
        write!(s, "{:x}\r\n", b.len())?;
        s.write_all(b)?;
        s.write_all(b"\r\n")
    };
    if send(&mut stream, &frame("update", &snapshot().stringify())).is_err() {
        unregister(id);
        return;
    }
    while let Ok(msg) = rx.recv() {
        if send(&mut stream, &msg).is_err() {
            break;
        }
    }
    unregister(id)
}
pub fn run(bind: Option<SocketAddr>) -> std::io::Result<()> {
    rebuild();
    let listener = TcpListener::bind(bind.unwrap_or_else(|| SocketAddr::new(host(), port())))?;
    println!("Alchemy dashboard on http://{}/", listener.local_addr()?);
    std::thread::spawn(|| loop {
        std::thread::sleep(Duration::from_secs(5));
        let msg = b": keepalive\n\n";
        SUBSCRIBERS.lock().unwrap().retain(|s| {
            !matches!(
                s.tx.try_send(msg.to_vec()),
                Err(TrySendError::Disconnected(_)) | Err(TrySendError::Full(_))
            )
        })
    });
    std::thread::spawn(|| {
        let mut w = Watcher::new();
        loop {
            std::thread::sleep(Duration::from_millis(250));
            if w.tick() {
                std::process::exit(0)
            }
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
        || !js.contains("EventSource")
        || !js.contains("closest(\"g[aria-label]\")")
    {
        return Err("dashboard assets are incomplete".into());
    }
    if !response("/")
        .map_err(|_| "shell route failed")?
        .body
        .starts_with(b"<!doctype html>")
    {
        return Err("dashboard shell failed".into());
    }
    if !matches!(
        response("/nope"),
        Err(Route::Response(Response { status: 404, .. }))
    ) {
        return Err("404 route failed".into());
    }
    Ok("self-test=ok dashboard".into())
}
