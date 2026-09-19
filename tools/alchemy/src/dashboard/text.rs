//! Read-only comparison of the tracked PO message catalogs for all editions.

use super::root;
use crate::coverage::boxtree::esc;
use crate::text_catalog::{archives, path, ArchiveSpec};
use psynergy::assets::po;
use std::fs;

const PAGE: usize = 256;

struct Edition {
    label: String,
    messages: Vec<Option<String>>,
}

fn route(path: &str) -> Option<(&'static str, usize)> {
    let parts = path.trim_matches('/').split('/').collect::<Vec<_>>();
    match parts.as_slice() {
        ["text"] => Some(("tbs", 0)),
        ["text", "tbs"] => Some(("tbs", 0)),
        ["text", "tla"] => Some(("tla", 0)),
        ["text", "tbs", page] => Some(("tbs", page.parse().ok()?)),
        ["text", "tla", page] => Some(("tla", page.parse().ok()?)),
        _ => None,
    }
}

fn load(spec: &ArchiveSpec) -> Result<Edition, String> {
    let source = path(&root(), spec);
    let catalog = po::read(
        &fs::read_to_string(&source).map_err(|error| format!("{}: {error}", source.display()))?,
    )
    .map_err(|error| error.to_string())?;
    let count = catalog
        .headers
        .get("X-Alchemy-Message-Count")
        .ok_or("message count is absent from PO")?
        .parse::<usize>()
        .map_err(|_| "message count in PO is invalid")?;
    let mut messages = vec![None; count];
    for entry in catalog.entries {
        if entry.context.as_deref() != Some("message") {
            return Err("PO entry context must be message".into());
        }
        let key = entry
            .id
            .parse::<usize>()
            .map_err(|_| "PO message key is invalid")?;
        let slot = messages
            .get_mut(key)
            .ok_or("PO message key is outside archive")?;
        if slot.is_some() {
            return Err(format!("duplicate PO message key {key}"));
        }
        *slot = Some(if entry.flags.iter().any(|flag| flag == "alchemy-null") {
            "∅".into()
        } else {
            entry.value
        });
    }
    Ok(Edition {
        label: spec.language.to_uppercase(),
        messages,
    })
}

fn readable(text: &str) -> String {
    text.replace("{page_break}", "\n\n")
        .replace("{line_break}", "\n")
        .replace("{{", "{")
        .replace("}}", "}")
}

pub(super) fn page(path_value: &str) -> Option<Result<String, String>> {
    if !path_value.starts_with("/text") {
        return None;
    }
    let Some((game, page)) = route(path_value) else {
        return Some(Err("Unknown text view".into()));
    };
    let title = if game == "tbs" {
        "The Broken Seal"
    } else {
        "The Lost Age"
    };
    let editions = match archives(game)
        .iter()
        .map(load)
        .collect::<Result<Vec<_>, _>>()
    {
        Ok(editions) => editions,
        Err(error) => return Some(Err(error)),
    };
    let messages = editions
        .iter()
        .map(|edition| edition.messages.len())
        .max()
        .unwrap_or(0);
    let Some(start) = page.checked_mul(PAGE).filter(|start| *start < messages) else {
        return Some(Err("message page is outside catalog".into()));
    };
    let end = (start + PAGE).min(messages);
    let pages = messages.div_ceil(PAGE);
    let mut html = format!(
        "<main class=\"text-view\"><header><span>{title} · Text</span><nav><a href=\"/text/tbs\"{}>TBS</a><a href=\"/text/tla\"{}>TLA</a></nav></header><section class=\"text-content\"><div class=\"text-pages\"><span>Keys {start}–{} of {messages}</span>",
        if game == "tbs" { " aria-current=\"page\"" } else { "" },
        if game == "tla" { " aria-current=\"page\"" } else { "" }, end - 1,
    );
    if page > 0 {
        html.push_str(&format!(
            "<a href=\"/text/{game}/{}\">Previous</a>",
            page - 1
        ));
    }
    html.push_str(&format!("<span>Page {} of {pages}</span>", page + 1));
    if page + 1 < pages {
        html.push_str(&format!("<a href=\"/text/{game}/{}\">Next</a>", page + 1));
    }
    html.push_str("</div><p class=\"text-note\">Keys are physical message-archive indexes. An edition may intentionally reuse an index for different text; the viewer does not invent a translation link where the ROMs record none.</p><div class=\"translation-table\"><table><thead><tr><th>Key</th>");
    for edition in &editions {
        html.push_str(&format!("<th>{}</th>", esc(&edition.label)));
    }
    html.push_str("</tr></thead><tbody>");
    for key in start..end {
        html.push_str(&format!("<tr><th>{key}</th>"));
        for edition in &editions {
            let value = edition
                .messages
                .get(key)
                .and_then(Option::as_deref)
                .map(readable)
                .unwrap_or_else(|| "—".into());
            html.push_str(&format!("<td>{}</td>", esc(&value)));
        }
        html.push_str("</tr>");
    }
    html.push_str("</tbody></table></div></section></main>");
    Some(Ok(html))
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn routes_are_bounded_and_message_markup_is_readable() {
        assert_eq!(route("/text"), Some(("tbs", 0)));
        assert_eq!(route("/text/tla/4"), Some(("tla", 4)));
        assert_eq!(route("/text/gs1"), None);
        assert_eq!(
            readable("Hi{line_break}{text_color:2}!"),
            "Hi\n{text_color:2}!"
        );
    }
}
