//! The dashboard's one window: title bar, icon tabs lettered from the glyph sheet, the
//! view's sheet and a status bar with DONE and background progress.
use super::cache::{self, Phase};
use super::glyphs::{self, TAB_ICONS};
use super::http::Response;
use super::status;
use crate::coverage::boxtree::{esc, CHROME};
use crate::coverage::letters::{LINE, PIXEL};

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
/// Palette tokens from the figure, the lettering rules and the icons when built.
fn theme() -> String {
    let assets = glyphs::current();
    let mut css = String::from(":root{");
    for (name, color) in CHROME {
        css.push_str(&format!("--{name}:{color};"));
    }
    css.push('}');
    css.push_str(&glyphs::lettering_css());
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
                    slot as u32 * LINE * PIXEL
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
        "<!doctype html><html lang=\"en\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><title>Alchemy · {current}</title>{}<style>{}</style><style>{}</style></head><body><div class=\"window\"><header class=\"titlebar\"><span>Alchemy</span></header><nav class=\"tabs\" aria-label=\"Views\">{}</nav><div class=\"sheet\">{content}</div>{}</div></body></html>",
        letters_meta(),
        theme(),
        styles(),
        tabs(current),
        statusbar(note)
    );
    let mut response = Response::new(
        200,
        "OK",
        Some("text/html; charset=utf-8"),
        "no-store",
        glyphs::letter(&body),
    );
    let scripts = if path == "/maps" || path.starts_with("/maps/") {
        "'self'"
    } else {
        "'none'"
    };
    response.headers.push(("Content-Security-Policy", format!("default-src 'self'; script-src {scripts}; style-src 'self' 'unsafe-inline'; font-src 'self'; object-src 'none'; base-uri 'none'")));
    response
}
/// A bare message page, such as the Finder action's result, in the same chrome.
pub(super) fn fragment(message: &str) -> String {
    glyphs::letter(&format!(
        "<!doctype html><html><head><style>{}</style><style>{}</style><style>html,body{{background:transparent}}</style></head><body>{}</body></html>",
        theme(),
        styles(),
        esc(message)
    ))
}
/// The sheet and advances for script-drawn text (the map canvas), when built.
fn letters_meta() -> String {
    glyphs::current()
        .letters
        .map(|(stamp, letters)| {
            let advances = letters
                .advance
                .iter()
                .map(u32::to_string)
                .collect::<Vec<_>>()
                .join(",");
            format!("<meta name=\"letters\" content=\"/cache/letters-{stamp}.png\" data-advances=\"{advances}\">")
        })
        .unwrap_or_default()
}
/// The stylesheet at `PIXEL`: it is written in game pixels.
pub(super) fn styles() -> &'static str {
    static SCALED: std::sync::OnceLock<String> = std::sync::OnceLock::new();
    SCALED.get_or_init(|| pixels(STYLES))
}
/// Multiply every whole `Npx` length in `css`, written in game pixels, by
/// `PIXEL`; identifiers and colours never end in a digit run before `px`.
pub(super) fn pixels(css: &str) -> String {
    let mut out = String::with_capacity(css.len());
    let mut digits = String::new();
    let mut rest = css;
    while let Some(c) = rest.chars().next() {
        rest = &rest[c.len_utf8()..];
        if c.is_ascii_digit() {
            let boundary = digits.is_empty()
                && out
                    .chars()
                    .next_back()
                    .is_some_and(|p| p.is_ascii_alphanumeric() || p == '.' || p == '#');
            if !boundary {
                digits.push(c);
                continue;
            }
        }
        if !digits.is_empty() {
            if c == 'p' && rest.starts_with('x') {
                let value: u32 = digits.parse().expect("digit run");
                out.push_str(&(value * PIXEL).to_string());
            } else {
                out.push_str(&digits);
            }
            digits.clear();
        }
        out.push(c);
    }
    out.push_str(&digits);
    out
}
