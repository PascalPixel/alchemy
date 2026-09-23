//! The dashboard's one window: title bar, icon tabs in the game font, the
//! view's sheet and a status bar with DONE and background progress.
use super::cache::{self, Phase};
use super::glyphs::{self, TAB_ICONS};
use super::http::Response;
use super::status;
use crate::coverage::boxtree::{esc, CHROME};

const STYLES: &str = include_str!("style.css");
pub(super) const TABS: [(&str, &str); 5] = [
    ("/", "Files"),
    ("/roms", "ROM coverage"),
    ("/music", "Music"),
    ("/maps", "Maps"),
    ("/text", "Text"),
];

pub(super) fn active(path: &str) -> &'static str {
    if path.starts_with("/rom") {
        "ROM coverage"
    } else if path.starts_with("/music") {
        "Music"
    } else if path.starts_with("/maps") {
        "Maps"
    } else if path.starts_with("/text") {
        "Text"
    } else {
        "Files"
    }
}
/// Palette tokens from the figure, and the cached faces and icons when built.
fn theme() -> String {
    let assets = glyphs::current();
    let mut css = String::new();
    for (kind, style, stamp) in [
        ("font", "normal", &assets.font),
        ("italic", "italic", &assets.italic),
    ] {
        if let Some(stamp) = stamp {
            css.push_str(&format!(
                "@font-face{{font-family:\"{}\";font-style:{style};src:url(/cache/{kind}-{stamp}.ttf) format(\"truetype\");font-display:swap}}",
                glyphs::FAMILY
            ));
        }
    }
    css.push_str(":root{");
    for (name, color) in CHROME {
        css.push_str(&format!("--{name}:{color};"));
    }
    css.push_str(&format!("--game:\"{}\",", glyphs::FAMILY));
    css.push_str("-apple-system,BlinkMacSystemFont,'Segoe UI',Helvetica,Arial,sans-serif}");
    if let Some(stamp) = &assets.icons {
        css.push_str(&format!(
            ".icon{{background-image:url(/cache/icons-{stamp}.png)}}"
        ));
    }
    css
}
fn tabs(current: &str) -> String {
    let icons = glyphs::current().icons.is_some();
    TABS.iter()
        .map(|(href, name)| {
            let slot = TAB_ICONS
                .iter()
                .position(|(tab, _, _)| tab == name)
                .unwrap_or(0);
            let icon = if icons {
                format!(
                    "<i class=\"icon\" style=\"background-position:-{}px 0\" aria-hidden=\"true\"></i>",
                    slot * 32
                )
            } else {
                String::new()
            };
            format!(
                "<a class=\"tab\" href=\"{href}\"{}>{icon}<span>{name}</span></a>",
                if *name == current {
                    " aria-current=\"page\""
                } else {
                    ""
                }
            )
        })
        .collect()
}
/// A gold gauge like the game's HP and PP bars; `None` draws an indeterminate sweep.
pub(super) fn progress(fraction: Option<f64>, label: &str) -> String {
    match fraction {
        Some(value) => {
            let percent = (value.clamp(0.0, 1.0) * 100.0).round();
            format!("<span class=\"progress\" role=\"progressbar\" aria-label=\"{}\" aria-valuemin=\"0\" aria-valuemax=\"100\" aria-valuenow=\"{percent}\"><span style=\"width:{percent}%\"></span></span>", esc(label))
        }
        None => format!(
            "<span class=\"progress busy\" role=\"progressbar\" aria-label=\"{}\"><span></span></span>",
            esc(label)
        ),
    }
}
fn statusbar(note: &str) -> String {
    let jobs = cache::jobs();
    let running = jobs.iter().find(|job| job.phase == Phase::Running);
    let failed = jobs.iter().find(|job| job.phase == Phase::Failed);
    let activity = match (running, failed) {
        (Some(job), _) => format!(
            "<span class=\"field activity\" title=\"{}\">{}<span>{}</span></span>",
            esc(&job.note),
            progress(
                (job.total > 0).then(|| job.fraction()),
                &format!("{} progress", job.label)
            ),
            esc(job.label)
        ),
        (None, Some(job)) => format!(
            "<span class=\"field activity failed\" title=\"{}\">{} failed</span>",
            esc(&job.note),
            esc(job.label)
        ),
        (None, None) => {
            let summary = jobs
                .iter()
                .map(|job| format!("{}: {} ms", job.label, job.millis))
                .collect::<Vec<_>>()
                .join(" · ");
            format!(
                "<span class=\"field activity\" title=\"{}\">Ready</span>",
                esc(&summary)
            )
        }
    };
    format!(
        "<footer class=\"statusbar\">{}{activity}<span class=\"field note\">{}</span></footer>",
        status::fields(),
        esc(note)
    )
}
/// The whole page around a view's content.
pub(super) fn page(path: &str, content: &str, note: &str) -> Response {
    let current = active(path);
    let body = format!(
        "<!doctype html><html lang=\"en\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><title>Alchemy · {current}</title><style>{}</style><style>{STYLES}</style></head><body><div class=\"window\"><header class=\"titlebar\"><span>Alchemy</span></header><nav class=\"tabs\" aria-label=\"Views\">{}</nav><div class=\"sheet\">{content}</div>{}</div></body></html>",
        theme(),
        tabs(current),
        statusbar(note)
    );
    let mut response = Response::new(
        200,
        "OK",
        Some("text/html; charset=utf-8"),
        "no-store",
        body,
    );
    let scripts = if path == "/maps" || path.starts_with("/maps/") {
        "'self'"
    } else {
        "'none'"
    };
    response.headers.push(("Content-Security-Policy", format!("default-src 'self'; script-src {scripts}; style-src 'self' 'unsafe-inline'; font-src 'self'; object-src 'none'; base-uri 'none'")));
    response
}
pub(super) fn styles() -> &'static str {
    STYLES
}
