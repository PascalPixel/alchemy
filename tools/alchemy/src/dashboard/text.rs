//! Read-only comparison of the tracked PO message catalogs for all editions.
//! Keys are physical archive indexes and differ between editions, which insert
//! and drop messages, so rows follow English and every other edition is
//! aligned to it by message shape; a cell's title gives its own key.
use super::http::{self, Response};
use super::{cache, chrome, root};
use crate::coverage::boxtree::esc;
use crate::coverage::jsnum::commas;
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
fn button(href: &str, label: &str, pressed: bool) -> String {
    format!(
        "<a class=\"button\" href=\"{href}\"{}>{label}</a>",
        if pressed {
            " aria-pressed=\"true\""
        } else {
            ""
        }
    )
}
/// A message's edition-independent shape: the control codes that name
/// something or end the message, and its digits, plus
/// its text when it is one short word, as names and labels often are. Two
/// editions' messages with the same shape are candidates for the same line.
fn signature(message: Option<&str>) -> String {
    let Some(text) = message else {
        return String::new();
    };
    let mut shape = String::new();
    let mut plain = String::new();
    let mut chars = text.chars().peekable();
    while let Some(c) = chars.next() {
        if c == '{' && chars.peek() != Some(&'{') {
            let mut code = String::from('{');
            for c in chars.by_ref() {
                code.push(c);
                if c == '}' {
                    break;
                }
            }
            // Each language wraps, paces and styles its own lines, so only
            // codes that name something or end the message carry shape.
            let name = code.trim_matches(|c| c == '{' || c == '}');
            let name = name.split(':').next().unwrap_or_default();
            if name.contains("name") || name.starts_with("argument") || name.starts_with("end_") {
                shape.push_str(&code);
            }
        } else if c.is_ascii_digit() {
            shape.push(c);
        } else {
            plain.push(c);
        }
    }
    let plain = plain.trim();
    if !plain.is_empty() && plain.chars().count() <= 16 && !plain.contains(' ') {
        shape.push('|');
        shape.push_str(plain);
    }
    shape
}
/// What survives translation of one message: its shape, its length in
/// characters, how it ends and how many breaks it holds.
struct Line {
    shape: String,
    length: f64,
    end: char,
    breaks: usize,
}
/// The message without its control codes.
fn visible(text: &str) -> String {
    let mut out = String::new();
    let mut depth = 0;
    for c in text.chars() {
        match c {
            '{' => depth += 1,
            '}' if depth > 0 => depth -= 1,
            _ if depth == 0 => out.push(c),
            _ => {}
        }
    }
    out
}
fn line(message: Option<&str>) -> Line {
    let text = message.unwrap_or("");
    Line {
        shape: signature(message),
        // Visible characters only: control codes differ between editions.
        length: visible(text).chars().count() as f64,
        end: text
            .trim_end()
            .chars()
            .last()
            .filter(|c| matches!(c, '.' | '?' | '!' | '}' | '。' | '？' | '！'))
            .unwrap_or(' '),
        breaks: text.matches("_break}").count(),
    }
}
/// Pairs of message indexes, `None` where one edition has no counterpart:
/// shapes unique to both ranges anchor the longest ordered run of pairs, the
/// ranges between anchors are aligned the same way, and each range left without
/// an anchor is aligned by length, ending and breaks.
fn align(a: &[Line], b: &[Line]) -> Vec<(Option<usize>, Option<usize>)> {
    let total = |lines: &[Line]| lines.iter().map(|line| line.length).sum::<f64>().max(1.0);
    let ratio = total(b) / total(a);
    let mut out = Vec::new();
    align_range(a, b, ratio, (0, a.len()), (0, b.len()), &mut out);
    out
}
fn align_range(
    a: &[Line],
    b: &[Line],
    ratio: f64,
    (mut a0, mut a1): (usize, usize),
    (mut b0, mut b1): (usize, usize),
    out: &mut Vec<(Option<usize>, Option<usize>)>,
) {
    let mut tail = Vec::new();
    while a0 < a1 && b0 < b1 && !a[a0].shape.is_empty() && a[a0].shape == b[b0].shape {
        out.push((Some(a0), Some(b0)));
        a0 += 1;
        b0 += 1;
    }
    while a0 < a1 && b0 < b1 && !a[a1 - 1].shape.is_empty() && a[a1 - 1].shape == b[b1 - 1].shape {
        a1 -= 1;
        b1 -= 1;
        tail.push((Some(a1), Some(b1)));
    }
    let mut counts = std::collections::HashMap::<&str, (usize, usize, usize)>::new();
    for (i, line) in a.iter().enumerate().take(a1).skip(a0) {
        let entry = counts.entry(line.shape.as_str()).or_default();
        entry.0 += 1;
        entry.2 = i;
    }
    for line in &b[b0..b1] {
        if let Some(entry) = counts.get_mut(line.shape.as_str()) {
            entry.1 += 1;
        }
    }
    let mut pairs = Vec::new();
    for (j, line) in b.iter().enumerate().take(b1).skip(b0) {
        if let Some(&(1, 1, i)) = counts.get(line.shape.as_str()) {
            if !line.shape.is_empty() {
                pairs.push((i, j));
            }
        }
    }
    pairs.sort_unstable();
    let anchors = increasing(&steady(&pairs, b.len() as f64 / a.len().max(1) as f64));
    if anchors.is_empty() && (a1 - a0) * (b1 - b0) <= 4_000_000 {
        similar(a, b, ratio, (a0, a1), (b0, b1), out);
    } else if anchors.is_empty() {
        // Too large to compare: unmatched messages sit just before the next
        // anchor, so the shorter side pairs with the end of the longer one.
        let (n, m) = (a1 - a0, b1 - b0);
        let width = n.max(m);
        for k in 0..width {
            let i = (k + n >= width).then(|| a0 + k + n - width);
            let j = (k + m >= width).then(|| b0 + k + m - width);
            out.push((i, j));
        }
    } else {
        let (mut i, mut j) = (a0, b0);
        for (ai, bj) in anchors {
            align_range(a, b, ratio, (i, ai), (j, bj), out);
            out.push((Some(ai), Some(bj)));
            (i, j) = (ai + 1, bj + 1);
        }
        align_range(a, b, ratio, (i, a1), (j, b1), out);
    }
    out.extend(tail.into_iter().rev());
}
/// The cheapest alignment of two unanchored ranges: pairing costs the length
/// difference (on a log scale, after the editions' overall length ratio) plus
/// differing endings and breaks, with a slight preference for pairs equally
/// far from the following anchor; leaving a message unpaired costs a fixed gap.
fn similar(
    a: &[Line],
    b: &[Line],
    ratio: f64,
    (a0, a1): (usize, usize),
    (b0, b1): (usize, usize),
    out: &mut Vec<(Option<usize>, Option<usize>)>,
) {
    const GAP: f64 = 1.2;
    let (n, m) = (a1 - a0, b1 - b0);
    let pair = |i: usize, j: usize| {
        let (x, y) = (&a[a0 + i], &b[b0 + j]);
        let unlike = ((x.length * ratio + 1.0) / (y.length + 1.0)).ln().abs()
            + if x.end == y.end { 0.0 } else { 0.5 }
            + 0.3
                * ((x.breaks as f64 + 1.0) / (y.breaks as f64 + 1.0))
                    .ln()
                    .abs();
        // Below two gaps, so every message pairs where the counts allow and
        // the score only decides where the unpaired ones go; ties, and only
        // ties, go to pairs as far from the next anchor on both sides.
        unlike.min(2.0) + 0.001 * (n - i).abs_diff(m - j).min(50) as f64
    };
    let width = m + 1;
    let mut cost = vec![0.0f64; (n + 1) * width];
    let mut step = vec![0u8; (n + 1) * width];
    for i in 0..=n {
        for j in 0..=m {
            let at = i * width + j;
            if i == 0 && j == 0 {
                continue;
            }
            let mut best = (f64::INFINITY, 0u8);
            if i > 0 && j > 0 {
                best = (cost[at - width - 1] + pair(i - 1, j - 1), 0);
            }
            if i > 0 && cost[at - width] + GAP < best.0 {
                best = (cost[at - width] + GAP, 1);
            }
            if j > 0 && cost[at - 1] + GAP < best.0 {
                best = (cost[at - 1] + GAP, 2);
            }
            (cost[at], step[at]) = best;
        }
    }
    let mut path = Vec::new();
    let (mut i, mut j) = (n, m);
    while i > 0 || j > 0 {
        match step[i * width + j] {
            0 => {
                i -= 1;
                j -= 1;
                path.push((Some(a0 + i), Some(b0 + j)));
            }
            1 => {
                i -= 1;
                path.push((Some(a0 + i), None));
            }
            _ => {
                j -= 1;
                path.push((None, Some(b0 + j)));
            }
        }
    }
    out.extend(path.into_iter().rev());
}
/// Candidate anchors near their proportional place whose offset between the
/// editions agrees with the median offset of their neighbours: a shape unique
/// to both editions by coincidence jumps far from the true line and would
/// force lopsided gaps.
fn steady(pairs: &[(usize, usize)], scale: f64) -> Vec<(usize, usize)> {
    const NEIGHBOURS: usize = 12;
    const DRIFT: i64 = 120;
    let offset = |&(i, j): &(usize, usize)| j as i64 - i as i64;
    (0..pairs.len())
        .filter(|&k| {
            // Editions differ by a few hundred messages at most, so an anchor
            // must also lie near where the catalogs' proportions put it.
            let (i, j) = pairs[k];
            if (j as f64 - i as f64 * scale).abs() > 400.0 + 0.05 * i as f64 {
                return false;
            }
            let low = k.saturating_sub(NEIGHBOURS);
            let high = (k + NEIGHBOURS + 1).min(pairs.len());
            let mut around = pairs[low..high].iter().map(offset).collect::<Vec<_>>();
            around.sort_unstable();
            (offset(&pairs[k]) - around[around.len() / 2]).abs() <= DRIFT
        })
        .map(|k| pairs[k])
        .collect()
}
/// The longest run of pairs increasing in both indexes (patience sorting).
fn increasing(pairs: &[(usize, usize)]) -> Vec<(usize, usize)> {
    let mut piles: Vec<usize> = Vec::new();
    let mut back = vec![usize::MAX; pairs.len()];
    for (k, &(_, j)) in pairs.iter().enumerate() {
        let at = piles.partition_point(|&p| pairs[p].1 < j);
        if at > 0 {
            back[k] = piles[at - 1];
        }
        if at == piles.len() {
            piles.push(k);
        } else {
            piles[at] = k;
        }
    }
    let mut run = Vec::new();
    let mut k = piles.last().copied().unwrap_or(usize::MAX);
    while k != usize::MAX {
        run.push(pairs[k]);
        k = back[k];
    }
    run.reverse();
    run
}

/// One line of the comparison: the English key, if English has the message,
/// and each edition's key for the message on this line.
struct Row {
    key: Option<usize>,
    cells: Vec<Option<usize>>,
}
fn rows(editions: &[Edition]) -> Vec<Row> {
    let reference = editions
        .iter()
        .position(|edition| edition.label == "EN")
        .unwrap_or(0);
    let shapes = |edition: &Edition| {
        edition
            .messages
            .iter()
            .map(|message| line(message.as_deref()))
            .collect::<Vec<_>>()
    };
    let english = shapes(&editions[reference]);
    let count = english.len();
    let mut matched = vec![vec![None; editions.len()]; count];
    // Messages another edition adds, placed after the English line they follow.
    let mut added = vec![Vec::<(usize, usize)>::new(); count + 1];
    for (column, edition) in editions.iter().enumerate() {
        if column == reference {
            for (key, cells) in matched.iter_mut().enumerate() {
                cells[column] = Some(key);
            }
            continue;
        }
        let mut after = 0;
        for pair in align(&english, &shapes(edition)) {
            match pair {
                (Some(key), other) => {
                    matched[key][column] = other;
                    after = key + 1;
                }
                (None, Some(own)) => added[after].push((column, own)),
                (None, None) => {}
            }
        }
    }
    let mut out = Vec::new();
    for key in 0..=count {
        for &(column, own) in &added[key] {
            let mut cells = vec![None; editions.len()];
            cells[column] = Some(own);
            out.push(Row { key: None, cells });
        }
        if key < count {
            out.push(Row {
                key: Some(key),
                cells: std::mem::take(&mut matched[key]),
            });
        }
    }
    out
}
/// The view body and its status-bar note.
fn render(game: &'static str, page: usize) -> Result<(String, String), String> {
    let editions = archives(game)
        .iter()
        .map(load)
        .collect::<Result<Vec<_>, _>>()?;
    let lines = rows(&editions);
    let start = page
        .checked_mul(PAGE)
        .filter(|start| *start < lines.len())
        .ok_or("message page is outside catalog")?;
    let end = (start + PAGE).min(lines.len());
    let pages = lines.len().div_ceil(PAGE);
    let previous = if page > 0 {
        button(&format!("/text/{game}/{}", page - 1), "◀ Previous", false)
    } else {
        "<span class=\"button\" aria-disabled=\"true\">◀ Previous</span>".into()
    };
    let next = if page + 1 < pages {
        button(&format!("/text/{game}/{}", page + 1), "Next ▶", false)
    } else {
        "<span class=\"button\" aria-disabled=\"true\">Next ▶</span>".into()
    };
    let mut html = format!(
        "<main class=\"text-view\"><div class=\"toolbar\">{}{}<span class=\"separator\"></span>{previous}<span class=\"toolbar-label\">Page {} of {pages}</span>{next}</div><div class=\"well table translation-table\"><table><thead><tr><th>EN key</th>",
        button("/text/tbs", "The Broken Seal", game == "tbs"),
        button("/text/tla", "The Lost Age", game == "tla"),
        page + 1,
    );
    for edition in &editions {
        html.push_str(&format!("<th>{}</th>", esc(&edition.label)));
    }
    html.push_str("</tr></thead><tbody>");
    for row in &lines[start..end] {
        let key = row
            .key
            .map_or_else(|| "+".to_string(), |key| key.to_string());
        html.push_str(&format!("<tr><th>{key}</th>"));
        for (edition, cell) in editions.iter().zip(&row.cells) {
            match cell {
                Some(own) => {
                    let value = edition.messages[*own]
                        .as_deref()
                        .map(readable)
                        .unwrap_or_else(|| "—".into());
                    html.push_str(&format!(
                        "<td title=\"{} key {own}\" data-face=\"{}\">{}</td>",
                        esc(&edition.label),
                        // Quoted game text in its edition's own dialogue font.
                        if edition.label == "JA" { "j" } else { "d" },
                        esc(&value)
                    ));
                }
                None => html.push_str("<td class=\"quiet\">—</td>"),
            }
        }
        html.push_str("</tr>");
    }
    html.push_str("</tbody></table></div></main>");
    let english = lines.iter().filter(|row| row.key.is_some()).count();
    Ok((
        html,
        format!(
            "Lines {}–{} of {} · {} English messages, other editions aligned by message shape",
            commas(start as i64),
            commas(end as i64 - 1),
            commas(lines.len() as i64),
            commas(english as i64)
        ),
    ))
}

pub(super) fn response(path: &str) -> Option<Response> {
    if path != "/text" && !path.starts_with("/text/") {
        return None;
    }
    let Some((game, page)) = route(path) else {
        return Some(http::not_found());
    };
    Some(match cache::view(path, || render(game, page)) {
        Ok((html, note)) => chrome::page(path, &html, &note),
        Err(error) => Response::new(
            404,
            "Not Found",
            Some("text/plain; charset=utf-8"),
            "no-store",
            error.into_bytes(),
        ),
    })
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn inserted_messages_open_a_line_instead_of_shifting_the_rest() {
        let english = [
            "{end}",
            "Settings",
            "Speed",
            "Isaac",
            "Garet",
            "Save?{end_now}",
        ]
        .map(|text| line(Some(text)));
        let german =
            ["{end}", "Isaac", "Garet", "Speichern?{end_now}"].map(|text| line(Some(text)));
        assert_eq!(
            align(&english, &german),
            [
                (Some(0), Some(0)),
                (Some(1), None),
                (Some(2), None),
                (Some(3), Some(1)),
                (Some(4), Some(2)),
                (Some(5), Some(3))
            ]
        );
    }
    #[test]
    fn routes_are_bounded_and_message_markup_is_readable() {
        assert_eq!(route("/text"), Some(("tbs", 0)));
        assert_eq!(route("/text/tla/4"), Some(("tla", 4)));
        assert_eq!(route("/text/gs1"), None);
        assert_eq!(response("/text/gs1").unwrap().status, 404);
        assert!(response("/textual").is_none());
        assert_eq!(
            readable("Hi{line_break}{text_color:2}!"),
            "Hi\n{text_color:2}!"
        );
    }
}
