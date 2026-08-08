// The Alchemy coverage dashboard, served as HTML instead of built in a browser.
//
// WHY THIS EXISTS
//
// The dashboard used to be two TypeScript files: dashboard_server.ts (Bun.serve,
// filesystem watchers, an SSE stream, and an on-demand Bun.build of the client)
// and dashboard/client.ts (175 lines of DOM code whose entire job was to fetch
// four SVG files and put them on the page). TypeScript is not in this project's
// language allowlist -- ASM, C, Rust, WAV, PCM, and JSON as a fallback -- so a
// browser bundle of it is exactly the kind of thing the allowlist exists to keep
// out. The view is a static 2x2 grid of four generated SVGs; it does not need a
// renderer, a bundler, or a build step. It needs a string.
//
// WHAT WAS DELIBERATELY GIVEN UP (each is a PORT NOTE at its site too):
//
//  * Filesystem watching and push updates. dashboard_server.ts watched asm/,
//    assets/, metrics/, semantic/ and rebuilt the coverage map in-process, then
//    pushed a snapshot down /events. Rebuilding the map means porting
//    tools/metrics/coverage_map.ts (2,995 lines), which is far outside this
//    change. Instead this binary re-reads the SVG files from disk on every
//    request, so regenerating them by any means -- `bun run coverage`, or
//    pointing --svg-dir at a directory something else writes -- shows up on the
//    next load. --refresh turns that into an automatic poll.
//  * The floating hover tooltip. Replaced by native SVG <title> elements, which
//    carry the identical label text (see svgdoc.rs). The trade: the browser
//    draws them, so they appear after a hover delay and are not styled in
//    Weyard. Nothing is lost from the information.
//  * The /snapshot JSON endpoint, the /events SSE stream, the /client.js bundle
//    route, the /styles.css route, and the /svg/<tree> routes. Styles are inline
//    and trees are inline; the other three had no consumer once the client left.
//  * "scanning" state and the "Reading the stones…" placeholder. The server has
//    nothing to scan, so the page is never in a pre-content state.
//  * Auto-reload when the page's own source changes, and the self-restart when
//    the server source changes. Rebuild and rerun.

mod http;
mod json;
mod page;
mod svgdoc;

use std::net::TcpListener;
use std::path::{Path, PathBuf};
use std::process::ExitCode;

struct Config {
    root: PathBuf,
    svg_dir: PathBuf,
    font: PathBuf,
    map: PathBuf,
    target: String,
    port: u16,
    refresh_seconds: u64,
}

const USAGE: &str = "\
usage: dashboard [options]

  --port <n>        listen port (default 4649, or $ALCHEMY_DASHBOARD_PORT)
  --root <dir>      repository root (default: cwd)
  --svg-dir <dir>   where <target>-<tree>.svg live (default <root>/assets/readme)
  --target <name>   SVG filename prefix (default gs1-en)
  --refresh <n>     seconds between page self-refreshes, 0 to disable (default 0)
  --once            write the document to stdout and exit, without listening
  --self-test       check that the real trees load and render, then exit
  --help
";

fn default_config() -> Config {
    let root = std::env::current_dir().unwrap_or_else(|_| PathBuf::from("."));
    let port = std::env::var("ALCHEMY_DASHBOARD_PORT")
        .ok()
        .and_then(|value| value.parse().ok())
        .unwrap_or(4649);
    Config {
        svg_dir: root.join("assets").join("readme"),
        font: root.join("assets").join("fonts").join("weyard.otf"),
        map: root.join("metrics").join("gs1-en-coverage-map.json"),
        target: "gs1-en".to_string(),
        port,
        refresh_seconds: 0,
        root,
    }
}

/// Read the coverage percentage that titles the page. A missing or unparseable
/// map is not an error: the page just loses its subtitle.
fn exact_percent(map: &Path) -> Option<f64> {
    let text = std::fs::read_to_string(map).ok()?;
    json::parse(&text)
        .ok()?
        .path("categories.exact_c.percent_of_executable")
        .and_then(json::Value::as_f64)
}

fn load_panels(config: &Config) -> Vec<page::Panel> {
    page::TREES
        .iter()
        .map(|(tree, title)| {
            let file = config.svg_dir.join(format!("{}-{tree}.svg", config.target));
            page::Panel {
                tree,
                title,
                svg: std::fs::read_to_string(&file)
                    .map_err(|error| format!("{}: {error}", file.display())),
            }
        })
        .collect()
}

fn render(config: &Config) -> String {
    page::document(
        &load_panels(config),
        exact_percent(&config.map),
        config.refresh_seconds,
    )
}

fn handle(config: &Config, request: &http::Request) -> http::Response {
    match request.path.as_str() {
        "/" => http::Response::ok("text/html; charset=utf-8", render(config).into_bytes()),
        "/weyard.otf" => match std::fs::read(&config.font) {
            Ok(bytes) => http::Response::cached("font/otf", bytes),
            Err(_) => http::Response::not_found(),
        },
        _ => http::Response::not_found(),
    }
}

/// The equivalent of dashboard_server.ts --self-test, restricted to what this
/// binary is actually responsible for: the four trees load, they render, and the
/// output has no script in it.
fn self_test(config: &Config) -> Result<(), String> {
    let panels = load_panels(config);
    for panel in &panels {
        let svg = panel.svg.as_ref().map_err(|error| error.clone())?;
        if !svg.trim_start().starts_with("<svg ") {
            return Err(format!("{} is not an SVG document", panel.tree));
        }
    }
    let html = page::document(&panels, exact_percent(&config.map), 0);
    for (tree, _) in page::TREES {
        if !html.contains(&format!("<section class=\"panel p-{tree}\">")) {
            return Err(format!("{tree} panel is missing from the document"));
        }
    }
    let titles = html.matches("<title>").count();
    if titles < 5 {
        return Err(format!("only {titles} hover labels survived into the page"));
    }
    if html.contains("<script") || html.contains("EventSource") {
        return Err("script leaked back into the served document".to_string());
    }
    if !html.contains("font-family:Weyard") {
        return Err("the trees lost their own Weyard chrome".to_string());
    }
    println!(
        "self-test=ok trees=4 hover-labels={} bytes={}",
        titles - 1,
        html.len()
    );
    Ok(())
}

fn parse_args(mut config: Config, args: &[String]) -> Result<(Config, Option<&'static str>), String> {
    let mut mode = None;
    let mut explicit_svg_dir = None;
    let mut at = 0;
    while at < args.len() {
        let flag = args[at].as_str();
        let value = || {
            args.get(at + 1)
                .cloned()
                .ok_or_else(|| format!("{flag} needs a value"))
        };
        match flag {
            "--help" | "-h" => return Ok((config, Some("help"))),
            "--once" => mode = Some("once"),
            "--self-test" => mode = Some("self-test"),
            "--port" => {
                config.port = value()?.parse().map_err(|_| "--port must be a number")?;
                at += 1;
            }
            "--refresh" => {
                config.refresh_seconds =
                    value()?.parse().map_err(|_| "--refresh must be a number")?;
                at += 1;
            }
            "--target" => {
                config.target = value()?;
                at += 1;
            }
            "--svg-dir" => {
                explicit_svg_dir = Some(PathBuf::from(value()?));
                at += 1;
            }
            "--root" => {
                let root = PathBuf::from(value()?);
                config.svg_dir = root.join("assets").join("readme");
                config.font = root.join("assets").join("fonts").join("weyard.otf");
                config.map = root.join("metrics").join("gs1-en-coverage-map.json");
                config.root = root;
                at += 1;
            }
            other => return Err(format!("unknown option {other}")),
        }
        at += 1;
    }
    // Resolve the derived paths once at the end so flag order does not matter:
    // --target after --root must still pick the right map, and an explicit
    // --svg-dir must survive a later --root.
    if let Some(directory) = explicit_svg_dir {
        config.svg_dir = directory;
    }
    config.map = config
        .root
        .join("metrics")
        .join(format!("{}-coverage-map.json", config.target));
    Ok((config, mode))
}

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    let (config, mode) = match parse_args(default_config(), &args) {
        Ok(parsed) => parsed,
        Err(error) => {
            eprintln!("dashboard: {error}\n\n{USAGE}");
            return ExitCode::from(2);
        }
    };
    match mode {
        Some("help") => {
            print!("{USAGE}");
            ExitCode::SUCCESS
        }
        Some("once") => {
            print!("{}", render(&config));
            ExitCode::SUCCESS
        }
        Some("self-test") => match self_test(&config) {
            Ok(()) => ExitCode::SUCCESS,
            Err(error) => {
                eprintln!("dashboard self-test failed: {error}");
                ExitCode::FAILURE
            }
        },
        _ => {
            let listener = match TcpListener::bind(("127.0.0.1", config.port)) {
                Ok(listener) => listener,
                Err(error) => {
                    eprintln!("dashboard: cannot bind port {}: {error}", config.port);
                    return ExitCode::FAILURE;
                }
            };
            println!(
                "Alchemy dashboard on http://127.0.0.1:{} (trees from {})",
                config.port,
                config.svg_dir.display()
            );
            if let Err(error) = http::serve(listener, &|request| handle(&config, request)) {
                eprintln!("dashboard: {error}");
                return ExitCode::FAILURE;
            }
            ExitCode::SUCCESS
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    /// Build a throwaway repository root with the file layout the binary expects,
    /// so the tests exercise real path resolution rather than mocked reads.
    struct Fixture {
        root: PathBuf,
    }

    impl Fixture {
        fn new(name: &str) -> Self {
            let root = std::env::temp_dir().join(format!("alchemy-dashboard-test-{name}"));
            let _ = std::fs::remove_dir_all(&root);
            std::fs::create_dir_all(root.join("assets").join("readme")).unwrap();
            std::fs::create_dir_all(root.join("assets").join("fonts")).unwrap();
            std::fs::create_dir_all(root.join("metrics")).unwrap();
            std::fs::write(root.join("assets").join("fonts").join("weyard.otf"), b"OTTO").unwrap();
            for (tree, _) in page::TREES {
                std::fs::write(
                    root.join("assets").join("readme").join(format!("gs1-en-{tree}.svg")),
                    format!(
                        "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 540 304\">\
                         <defs><style>.weyard{{font-family:Weyard;font-size:16px}}</style></defs>\
                         <g aria-label=\"{tree} root: 1,024 bytes\"><rect/></g></svg>"
                    ),
                )
                .unwrap();
            }
            std::fs::write(
                root.join("metrics").join("gs1-en-coverage-map.json"),
                r#"{"categories":{"exact_c":{"bytes":316342,"percent_of_executable":23.5}}}"#,
            )
            .unwrap();
            Self { root }
        }

        fn config(&self) -> Config {
            let (config, _) = parse_args(
                default_config(),
                &["--root".to_string(), self.root.display().to_string()],
            )
            .unwrap();
            config
        }
    }

    impl Drop for Fixture {
        fn drop(&mut self) {
            let _ = std::fs::remove_dir_all(&self.root);
        }
    }

    #[test]
    fn renders_a_complete_page_from_a_repository_layout() {
        let fixture = Fixture::new("render");
        let html = render(&fixture.config());
        assert!(html.contains("<title>Alchemy — 23.50% exact C</title>"));
        assert_eq!(html.matches("class=\"tree-image\"").count(), 4);
        assert!(html.contains("<title>core root: 1,024 bytes</title>"));
        assert!(!html.contains("class=\"error\""));
    }

    #[test]
    fn self_test_passes_on_a_complete_layout_and_fails_on_a_missing_tree() {
        let fixture = Fixture::new("selftest");
        assert!(self_test(&fixture.config()).is_ok());
        std::fs::remove_file(
            fixture.root.join("assets").join("readme").join("gs1-en-music.svg"),
        )
        .unwrap();
        let error = self_test(&fixture.config()).unwrap_err();
        assert!(error.contains("gs1-en-music.svg"), "unexpected error: {error}");
    }

    #[test]
    fn routing_serves_the_document_the_font_and_nothing_else() {
        let fixture = Fixture::new("routes");
        let config = fixture.config();
        let request = |path: &str| http::Request {
            method: "GET".to_string(),
            path: path.to_string(),
        };
        let root = handle(&config, &request("/"));
        assert_eq!(root.status, 200);
        assert_eq!(root.content_type, "text/html; charset=utf-8");
        let font = handle(&config, &request("/weyard.otf"));
        assert_eq!(font.status, 200);
        assert_eq!(font.body, b"OTTO");
        for gone in ["/snapshot", "/events", "/client.js", "/styles.css", "/svg/core"] {
            assert_eq!(handle(&config, &request(gone)).status, 404, "{gone}");
        }
    }

    #[test]
    fn a_missing_coverage_map_only_costs_the_page_its_subtitle() {
        let fixture = Fixture::new("nomap");
        std::fs::remove_file(fixture.root.join("metrics").join("gs1-en-coverage-map.json"))
            .unwrap();
        let html = render(&fixture.config());
        assert!(html.contains("<title>Alchemy</title>"));
        assert_eq!(html.matches("class=\"tree-image\"").count(), 4);
    }

    #[test]
    fn options_are_parsed_and_bad_ones_are_refused() {
        let (config, mode) = parse_args(
            default_config(),
            &[
                "--port".to_string(),
                "5000".to_string(),
                "--refresh".to_string(),
                "15".to_string(),
                "--target".to_string(),
                "gs2-en".to_string(),
                "--once".to_string(),
            ],
        )
        .unwrap();
        assert_eq!(config.port, 5000);
        assert_eq!(config.refresh_seconds, 15);
        assert_eq!(config.target, "gs2-en");
        assert!(config.map.ends_with("gs2-en-coverage-map.json"));
        assert_eq!(mode, Some("once"));
        assert!(parse_args(default_config(), &["--nope".to_string()]).is_err());
        assert!(parse_args(default_config(), &["--port".to_string()]).is_err());
        assert!(parse_args(default_config(), &["--port".to_string(), "x".to_string()]).is_err());
    }

    #[test]
    fn the_target_option_selects_the_svg_filenames() {
        let fixture = Fixture::new("target");
        let (config, _) = parse_args(
            default_config(),
            &[
                "--root".to_string(),
                fixture.root.display().to_string(),
                "--target".to_string(),
                "gs2-en".to_string(),
            ],
        )
        .unwrap();
        let html = render(&config);
        assert!(html.contains("gs2-en-core.svg"), "should report the missing gs2 tree");
        assert!(html.contains("class=\"error\""));
    }
}
