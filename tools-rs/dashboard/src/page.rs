// The whole dashboard document, assembled on the server.
//
// This is the replacement for tools/metrics/dashboard/client.ts. That file built
// the view in the browser: it fetched /snapshot, fetched each /svg/<tree>,
// parsed them with DOMParser, and appended the result. Every byte of it existed
// because the server refused to emit markup -- dashboard_server.ts even had a
// self-test asserting the document shell contained no view markup.
//
// TypeScript is not in this repo's language allowlist (ASM, C, Rust, WAV, PCM,
// JSON), so the view has to be built somewhere that is. Building it here costs
// nothing: the four panels are a static 2x2 grid of four SVG files. There was
// never a reason for a client-side renderer beyond wanting live updates, and a
// <meta http-equiv="refresh"> buys those back without a script tag.
//
// Emitted HTML is output, not source, so the inline <style> below is fine. It is
// tools/metrics/dashboard/styles.css with the rules for the deleted script-only
// features removed.

use crate::svgdoc;

/// The four box trees, in the order the old TREE_LABELS record listed them.
pub const TREES: [(&str, &str); 4] = [
    ("core", "Main image"),
    ("overlays", "Code overlays"),
    ("images", "Images"),
    ("music", "Music"),
];

/// PORT NOTE: dashboard/styles.css minus `.hover-tooltip` (the floating <div>
/// the pointermove handler positioned -- native SVG <title> tooltips are drawn
/// by the browser and take no CSS) and minus `.loading` (there is no longer a
/// moment where the page exists but its content does not). Everything else,
/// including the aspect-ratio repacking cutovers and the one 16px Weyard size,
/// is carried over unchanged.
const STYLES: &str = r#"@font-face {
  font-family: Weyard;
  src: url("/weyard.otf") format("opentype");
  font-weight: 400;
  font-style: italic;
  font-display: block;
}

:root {
  --weyard-font: italic 400 16px/15px Weyard;
}

* {
  box-sizing: border-box;
  font-smooth: never;
  -webkit-font-smoothing: none;
  font-synthesis: none;
}

html,
body {
  width: 100vw;
  height: 100%;
  margin: 0;
  overflow: hidden;
  background: #fff;
}

#root {
  position: relative;
  width: 100vw;
  height: 100vh;
  height: 100dvh;
  overflow: hidden;
}

.trees {
  position: absolute;
  inset: 0;
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  grid-template-rows: repeat(2, minmax(0, 1fr));
  overflow: hidden;
}

.tree-image {
  width: 100%;
  height: 100%;
  max-width: 100%;
  max-height: 100%;
  display: block;
  object-fit: contain;
  object-position: center;
}

.panel {
  min-width: 0;
  min-height: 0;
  overflow: hidden;
}

.error {
  position: absolute;
  z-index: 1;
  top: 8px;
  left: 50%;
  max-width: calc(100% - 16px);
  transform: translateX(-50%);
  padding: 8px 10px;
  color: #7d211b;
  background: #fff1ef;
  border: 2px solid #7c2626;
  border-radius: 6px;
  font: var(--weyard-font);
}

@media (prefers-color-scheme: dark) {
  html, body { background: #131318; }
  .error { color: #ffc1bb; background: #371b1a; }
}

/* Each chart is 540:304. These cutovers select the packing that gives every
 * chart the largest possible contained rectangle for the viewport shape. */
@media (min-aspect-ratio: 135 / 38) {
  .trees {
    grid-template-columns: repeat(4, minmax(0, 1fr));
    grid-template-rows: minmax(0, 1fr);
  }
}

@media (max-aspect-ratio: 135 / 152) {
  .trees {
    grid-template-columns: minmax(0, 1fr);
    grid-template-rows: repeat(4, minmax(0, 1fr));
  }
}
"#;

pub fn escape(text: &str) -> String {
    let mut out = String::with_capacity(text.len());
    for character in text.chars() {
        match character {
            '&' => out.push_str("&amp;"),
            '<' => out.push_str("&lt;"),
            '>' => out.push_str("&gt;"),
            '"' => out.push_str("&quot;"),
            '\'' => out.push_str("&apos;"),
            _ => out.push(character),
        }
    }
    out
}

/// One panel's worth of loaded tree, or the reason it could not be loaded.
pub struct Panel {
    pub tree: &'static str,
    pub title: &'static str,
    pub svg: Result<String, String>,
}

/// PORT NOTE: the old title came from /snapshot's summary. Here it comes from
/// the same number in the coverage map on disk. `None` means the map has not
/// been generated yet, and the page falls back to the bare "Alchemy" the
/// document shell used before the first snapshot arrived.
pub fn title(exact_percent: Option<f64>) -> String {
    match exact_percent {
        Some(percent) => format!("Alchemy — {percent:.2}% exact C"),
        None => "Alchemy".to_string(),
    }
}

/// PORT NOTE: replaces the EventSource on /events. The server no longer pushes;
/// the page pulls on an interval. A refresh of 0 disables it entirely, which is
/// what you want when the SVGs are the committed ones and cannot change.
fn refresh_tag(refresh_seconds: u64) -> String {
    if refresh_seconds == 0 {
        String::new()
    } else {
        format!("<meta http-equiv=\"refresh\" content=\"{refresh_seconds}\">")
    }
}

pub fn document(panels: &[Panel], exact_percent: Option<f64>, refresh_seconds: u64) -> String {
    let mut out = String::with_capacity(4 << 20);
    out.push_str("<!doctype html>");
    out.push_str("<meta charset=\"utf-8\">");
    out.push_str("<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">");
    out.push_str(&refresh_tag(refresh_seconds));
    out.push_str(&format!("<title>{}</title>", escape(&title(exact_percent))));
    out.push_str("<style>");
    out.push_str(STYLES);
    out.push_str("</style>");
    out.push_str("<div id=\"root\">");

    let failures: Vec<&str> = panels
        .iter()
        .filter_map(|panel| panel.svg.as_ref().err().map(String::as_str))
        .collect();
    if !failures.is_empty() {
        out.push_str(&format!(
            "<div class=\"error\" role=\"alert\">Dashboard update failed: {}</div>",
            escape(&failures.join("; "))
        ));
    }

    out.push_str("<main class=\"trees\">");
    for panel in panels {
        out.push_str(&format!("<section class=\"panel p-{}\">", panel.tree));
        match &panel.svg {
            Ok(svg) => match svgdoc::prepare(svg) {
                Ok(prepared) => out.push_str(&prepared),
                Err(error) => out.push_str(&format!(
                    "<div class=\"error\" role=\"alert\">{}: {}</div>",
                    escape(panel.title),
                    escape(&error)
                )),
            },
            Err(error) => out.push_str(&format!(
                "<div class=\"error\" role=\"alert\">{}: {}</div>",
                escape(panel.title),
                escape(error)
            )),
        }
        out.push_str("</section>");
    }
    out.push_str("</main></div>");
    out
}

#[cfg(test)]
mod tests {
    use super::*;

    fn tree(tree: &'static str, title: &'static str, body: &str) -> Panel {
        Panel {
            tree,
            title,
            svg: Ok(format!(
                "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 540 304\">\
                 <g aria-label=\"{body}: 12 bytes\"><rect/></g></svg>"
            )),
        }
    }

    fn four_panels() -> Vec<Panel> {
        TREES
            .iter()
            .map(|(id, label)| tree(id, label, id))
            .collect()
    }

    #[test]
    fn the_document_carries_all_four_panels_and_their_svgs_inline() {
        let html = document(&four_panels(), Some(23.5), 0);
        for (id, _) in TREES {
            assert!(html.contains(&format!("<section class=\"panel p-{id}\">")));
            assert!(html.contains(&format!("<title>{id}: 12 bytes</title>")));
        }
        assert_eq!(html.matches("<svg ").count(), 4);
        assert_eq!(html.matches("class=\"tree-image\"").count(), 4);
        assert!(html.starts_with("<!doctype html>"));
    }

    #[test]
    fn the_page_title_carries_the_exact_c_percentage_to_two_places() {
        assert_eq!(title(Some(23.5)), "Alchemy — 23.50% exact C");
        assert_eq!(title(Some(23.456)), "Alchemy — 23.46% exact C");
        assert_eq!(title(None), "Alchemy");
        assert!(document(&four_panels(), Some(23.5), 0)
            .contains("<title>Alchemy — 23.50% exact C</title>"));
    }

    #[test]
    fn a_refresh_interval_becomes_a_meta_tag_and_zero_becomes_nothing() {
        assert!(document(&four_panels(), None, 5).contains("<meta http-equiv=\"refresh\" content=\"5\">"));
        assert!(!document(&four_panels(), None, 0).contains("http-equiv"));
    }

    #[test]
    fn a_missing_tree_reports_itself_instead_of_blanking_the_page() {
        let mut panels = four_panels();
        panels[1].svg = Err("gs1-en-overlays.svg: No such file or directory".to_string());
        let html = document(&panels, Some(23.5), 0);
        assert!(html.contains("Dashboard update failed: gs1-en-overlays.svg"));
        assert!(html.contains("Code overlays: gs1-en-overlays.svg"));
        // The three that did load are still drawn.
        assert_eq!(html.matches("<svg ").count(), 3);
    }

    #[test]
    fn no_script_element_survives_anywhere_in_the_output() {
        let html = document(&four_panels(), Some(23.5), 30);
        assert!(!html.contains("<script"));
        assert!(!html.contains("EventSource"));
        assert!(!html.contains("client.js"));
    }

    #[test]
    fn styles_are_inlined_and_still_hold_the_one_weyard_size() {
        let html = document(&four_panels(), None, 0);
        assert!(html.contains("--weyard-font: italic 400 16px/15px Weyard"));
        assert!(!html.contains("stylesheet"));
        assert!(!html.contains("hover-tooltip"));
        assert!(html.contains("@media (min-aspect-ratio: 135 / 38)"));
    }

    #[test]
    fn error_text_from_the_filesystem_cannot_inject_markup() {
        let mut panels = four_panels();
        panels[0].svg = Err("<img src=x onerror=alert(1)>".to_string());
        let html = document(&panels, None, 0);
        assert!(!html.contains("<img"));
        assert!(html.contains("&lt;img src=x onerror=alert(1)&gt;"));
    }
}
