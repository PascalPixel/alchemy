//! Local source, ROM, music, map and text debugging views in one window.
//! Background jobs fill the dashboard cache under `out/dashboard/`; views
//! read it and show a progress bar while it builds.
mod cache;
mod chrome;
mod files;
mod glyphs;
mod maps;
mod media;
mod roms;
mod status;
mod text;
use super::http::{self, root, Response};
use serde_json::{json, Value};
use std::{
    net::TcpListener,
    path::{Component, Path, PathBuf},
    process::Command,
    time::{Duration, SystemTime, UNIX_EPOCH},
};
use walkdir::WalkDir;

/// Inputs every view reads: the game and scaffolding trees, generated
/// reports and manifests, the local ROMs and the receipt identity's tooling.
fn watched(root: &Path) -> Vec<PathBuf> {
    let mut paths = [
        "games",
        "recon",
        "roms",
        "tools/alchemy/src",
        "tools/psynergy/src",
        "out/tbs-en/full/asm/manifest.json",
        "out/tbs-en/full/assets/manifest.json",
        "out/tbs-en/assets/manifest.json",
        "out/tla-en/assets/manifest.json",
    ]
    .iter()
    .map(|path| root.join(path))
    .collect::<Vec<_>>();
    for id in crate::targets::TARGET_IDS {
        paths.push(root.join(format!("out/{}/reports", id.as_str())));
    }
    paths
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
static REFRESHED: std::sync::Mutex<Option<String>> = std::sync::Mutex::new(None);

fn snapshot() -> Value {
    let jobs = cache::jobs();
    json!({
        "page": crate::coverage::boxtree::content_version(chrome::styles()),
        "revision": cache::generation().to_string(),
        "generatedAt": REFRESHED.lock().unwrap_or_else(|e| e.into_inner()).clone(),
        "scanning": jobs.iter().any(|job| job.phase == cache::Phase::Running),
        "done": status::snapshot(),
        "jobs": jobs.iter().map(|job| json!({
            "id": job.id, "label": job.label, "done": job.done, "total": job.total,
            "state": match job.phase {
                cache::Phase::Running => "running",
                cache::Phase::Ready => "ready",
                cache::Phase::Failed => "failed",
            },
            "note": job.note, "milliseconds": job.millis as u64
        })).collect::<Vec<_>>(),
        "project": {
            "title": "Golden Sun · The Lost Age",
            "tbs": "ja · en · de · es · fr · it",
            "tla": "ja · en · de · es · fr · it",
            "fullTarget": "tbs-en"
        }
    })
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

/// Run every cache job once: the quick ones first, then the receipts, whose
/// verification hashes every build input.
fn refresh(root: &Path) {
    cache::begin("assets", "Building lettering and icons", 1);
    cache::finish("assets", glyphs::refresh(root));
    cache::begin(
        "roms",
        "Reading ROM indexes",
        crate::targets::TARGET_IDS.len(),
    );
    cache::finish("roms", roms::refresh(root));
    cache::begin("progress", "Reading receipts", status::GAMES.len());
    cache::finish("progress", status::refresh(root));
    *REFRESHED.lock().unwrap_or_else(|e| e.into_inner()) = Some(iso_now());
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
    Response::new(
        status,
        reason,
        Some("text/html; charset=utf-8"),
        "no-store",
        chrome::fragment(&message),
    )
}
fn cached_file(path: &str) -> Option<Response> {
    let name = path.strip_prefix("/cache/")?;
    Some(match glyphs::file(&root(), name) {
        Some((mime, bytes)) => Response::new(
            200,
            "OK",
            Some(mime),
            "public, max-age=31536000, immutable",
            bytes,
        ),
        None => http::not_found(),
    })
}
fn response(path: &str) -> Response {
    let root = root();
    if let Some(response) = cached_file(path)
        .or_else(|| maps::response(path))
        .or_else(|| media::response(path))
        .or_else(|| text::response(path))
        .or_else(|| roms::page(path))
        .or_else(|| files::page(&root, path))
    {
        return response;
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
/// Rebuilds the cache after watched inputs change and then stay still for two
/// ticks, so a build writing many files triggers one refresh.
struct Watcher {
    inputs: Vec<(PathBuf, Fingerprint)>,
    dirty: bool,
    stable_ticks: u8,
}
impl Watcher {
    fn new(root: &Path) -> Self {
        Self {
            inputs: watched(root)
                .into_iter()
                .map(|path| {
                    let print = fingerprint(&path);
                    (path, print)
                })
                .collect(),
            dirty: false,
            stable_ticks: 0,
        }
    }
    fn tick(&mut self, root: &Path) {
        let mut changed = false;
        for (path, old) in &mut self.inputs {
            let now = fingerprint(path);
            changed |= now != *old;
            *old = now
        }
        if changed {
            self.dirty = true;
            self.stable_ticks = 0;
            cache::invalidate();
        } else if self.dirty {
            self.stable_ticks = self.stable_ticks.saturating_add(1);
            if self.stable_ticks >= 2 {
                self.dirty = false;
                self.stable_ticks = 0;
                refresh(root);
            }
        }
    }
}

pub fn entry(args: &[String]) -> Result<(), String> {
    let Some(bind) = http::bind(args, "dashboard", 4650)? else {
        return Ok(());
    };
    let listener = TcpListener::bind(bind).map_err(|e| e.to_string())?;
    std::thread::spawn(|| {
        let root = root();
        let mut watcher = Watcher::new(&root);
        refresh(&root);
        loop {
            std::thread::sleep(Duration::from_secs(1));
            watcher.tick(&root);
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
    fn snapshot_reports_jobs_and_never_invents_done() {
        let snapshot = snapshot();
        for game in ["tbs-en", "tla-en"] {
            let state = snapshot["done"][game]["state"].as_str().unwrap();
            assert!(
                matches!(state, "checking" | "pending" | "verified"),
                "{state}"
            );
            if state != "verified" {
                assert!(snapshot["done"][game]["donePercent"].is_null());
            }
        }
        assert!(snapshot["jobs"].is_array());
        assert_eq!(snapshot["project"]["fullTarget"], "tbs-en");
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
    fn views_need_no_script_except_maps_and_retired_routes_stay_gone() {
        for path in [
            "/music/catalog",
            "/music/soundfont",
            "/music/tla/soundfont",
            "/music/bgm_000.mid",
            "/client.js",
            "/weyard.otf",
            "/svg/rom/800",
            "/inspect/8001000/",
            "/shared/",
            "/cache/font.ttf",
            "/cache/../roms/tbs-en.gba",
        ] {
            assert_eq!(response(path).status, 404, "{path}");
        }
        for path in ["/", "/roms", "/text", "/music"] {
            let page = response(path);
            assert_eq!(page.status, 200, "{path}");
            let html = String::from_utf8(page.body).unwrap();
            assert!(!html.contains("<script"), "{path}");
            assert!(page
                .headers
                .iter()
                .any(|(key, value)| *key == "Content-Security-Policy"
                    && value.contains("script-src 'none'")
                    && value.contains("font-src 'self'")));
            assert_eq!(html.matches("class=\"tab\"").count(), 5, "{path}");
            assert!(html.contains("class=\"statusbar\""), "{path}");
        }
    }

    #[test]
    fn the_stylesheet_is_golden_sun_chrome_on_one_pixel_grid_without_remote_assets() {
        let styles = chrome::styles();
        assert!(!styles.contains("underline"));
        assert!(!styles.contains("@font-face"));
        assert!(!styles.contains("url("));
        assert!(!styles.contains("http"));
        assert!(styles.contains(".tab {"));
        assert!(
            styles.contains("border-radius:3px 3px 0 0")
                && styles.contains("box-shadow:var(--frame)")
        );
        // Text is glyph sprites; the system face only fills in what the sheet lacks.
        assert!(styles.contains(".t b {") && styles.contains("--text: 12px/16px var(--system)"));
        assert!(!styles.contains("font-style:italic"));
        // Every length is a whole number of game pixels, each PIXEL CSS pixels.
        let source = include_str!("style.css");
        for (at, _) in source.match_indices("px") {
            let before = source[..at].trim_end_matches(|c: char| c.is_ascii_digit());
            assert!(
                before.len() < at && !before.ends_with('.'),
                "fractional or empty length near {}",
                &source[at.saturating_sub(40)..at + 2]
            );
        }
        let pixel = crate::coverage::letters::PIXEL;
        assert_eq!(
            chrome::pixels("inset -3px 12px var(--w3) #0c1830 1.5px 40%"),
            format!(
                "inset -{}px {}px var(--w3) #0c1830 1.5px 40%",
                3 * pixel,
                12 * pixel
            )
        );
        assert!(!styles.contains(".music-player"));
    }

    #[test]
    fn navigation_rejects_invalid_paths() {
        for path in [
            "/view/ff",
            "/view/2e2e2f",
            "/file/",
            "/file/zz",
            "/roms/gs3-en",
        ] {
            assert_eq!(response(path).status, 404, "{path}");
        }
    }
}
