//! PROGRESS.svg, the one public figure: a slim gold gauge for each game's
//! verified DONE above the tracked-files tree, both on the tree's teal chart
//! with its bevels and palette. It embeds no game font or picture.
use super::boxtree::{esc, files_svg, BEVEL_DARK, BEVEL_LIGHT, CHART_BACKGROUND};
use super::jsnum::commas;
use super::progress::GameDone;

pub(crate) const WIDTH: f64 = 830.0;
const PAD: f64 = 14.0;
const ROW: f64 = 44.0;
const HEADER: f64 = 2.0 * ROW + 16.0;
const BAR: f64 = 10.0;
/// Exact C in gold, evidenced permanent assembly in pale gold, the rest a
/// darker teal well cut into the chart.
const GOLD: &str = "#f4c84f";
const PALE_GOLD: &str = "#f9e7a8";
const WELL: &str = "#17606f";

/// The figure for both games; a game without a verified count shows `?`.
pub(crate) fn progress_svg(sun: Option<GameDone>, anchor: Option<GameDone>) -> String {
    let tree = files_svg(WIDTH);
    let tree_height = attribute(&tree, "height").unwrap_or(WIDTH * 16.0 / 9.0);
    let height = (HEADER + tree_height).ceil();
    let mut out = format!(
        "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 {WIDTH} {height}\" width=\"{WIDTH}\" height=\"{height}\" role=\"img\" aria-label=\"Alchemy progress: The Broken Seal {} and The Lost Age {}, above the repository's files\">",
        percent(sun),
        percent(anchor)
    );
    out.push_str(&format!(
        "<rect width=\"{WIDTH}\" height=\"{height}\" fill=\"{CHART_BACKGROUND}\"/><path d=\"M0.5 {HEADER}V0.5H{}\" fill=\"none\" stroke=\"{BEVEL_LIGHT}\"/><path d=\"M0.5 {}H{}V0.5\" fill=\"none\" stroke=\"{BEVEL_DARK}\"/>",
        WIDTH - 0.5,
        HEADER - 0.5,
        WIDTH - 0.5
    ));
    for (row, (mark, name, done)) in [
        ("☀️", "The Broken Seal", sun),
        ("⚓️", "The Lost Age", anchor),
    ]
    .into_iter()
    .enumerate()
    {
        let top = 10.0 + row as f64 * ROW;
        out.push_str(&gauge(PAD, top, WIDTH - 2.0 * PAD, mark, name, done));
    }
    out.push_str(&format!(
        "<g transform=\"translate(0 {HEADER})\">{}</g></svg>\n",
        tree.trim_end()
    ));
    out
}

fn percent(done: Option<GameDone>) -> String {
    done.map_or("?".into(), |done| format!("{:.2}%", done.percent()))
}
fn attribute(svg: &str, name: &str) -> Option<f64> {
    let head = &svg[..svg.find('>')?];
    let start = head.find(&format!(" {name}=\""))? + name.len() + 3;
    head[start..].split('"').next()?.parse().ok()
}
/// Text in the tree's one label style, like a handheld's single font: the
/// same face, size and one-pixel shadow everywhere, only the colour changes.
fn text(x: f64, y: f64, fill: &str, content: &str, anchor: &str) -> String {
    format!(
        "<text class=\"label\" x=\"{x}\" y=\"{y}\" text-anchor=\"{anchor}\" style=\"fill:{fill}\">{}</text>",
        esc(content)
    )
}
/// One game's line: its mark, name and DONE, then a slim gold gauge whose
/// bright part is exact C and pale part evidenced permanent assembly.
fn gauge(x: f64, y: f64, width: f64, mark: &str, name: &str, done: Option<GameDone>) -> String {
    let mut out = text(x, y + 16.0, "#fff", &format!("{mark} {name}"), "start");
    out.push_str(&text(x + width, y + 16.0, GOLD, &percent(done), "end"));
    let bar_y = y + 25.0;
    out.push_str(&format!(
        "<rect x=\"{x}\" y=\"{bar_y}\" width=\"{width}\" height=\"{BAR}\" rx=\"{}\" fill=\"{WELL}\" stroke=\"{BEVEL_DARK}\" stroke-opacity=\"0.5\"/>",
        BAR / 2.0
    ));
    if let Some(done) = done.filter(|done| done.executable > 0) {
        let share = |bytes: i64| width * bytes as f64 / done.executable as f64;
        let code = share(done.common_c + done.game_c);
        let assembly = share(done.common_asm + done.game_asm);
        let clip = name.replace(' ', "-").to_ascii_lowercase();
        out.push_str(&format!(
            "<clipPath id=\"{clip}\"><rect x=\"{x}\" y=\"{bar_y}\" width=\"{width}\" height=\"{BAR}\" rx=\"{}\"/></clipPath><g clip-path=\"url(#{clip})\"><rect x=\"{x}\" y=\"{bar_y}\" width=\"{code:.2}\" height=\"{BAR}\" fill=\"{GOLD}\"/><rect x=\"{:.2}\" y=\"{bar_y}\" width=\"{assembly:.2}\" height=\"{BAR}\" fill=\"{PALE_GOLD}\"/></g>",
            BAR / 2.0,
            x + code
        ));
        out.push_str(&format!(
            "<title>{name}: {} of {} executable bytes; exact C {}, permanent assembly {}</title>",
            commas(done.bytes()),
            commas(done.executable),
            commas(done.common_c + done.game_c),
            commas(done.common_asm + done.game_asm)
        ));
    }
    out
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn gauges_show_verified_counts_and_question_marks_otherwise() {
        let done = GameDone {
            common_asm: 0,
            common_c: 10,
            game_asm: 20,
            game_c: 70,
            executable: 200,
        };
        let line = gauge(0.0, 0.0, 400.0, "☀️", "The Broken Seal", Some(done));
        assert!(line.contains("50.00%"));
        assert!(line.contains("width=\"160.00\""));
        assert!(line.contains("width=\"40.00\""));
        let pending = gauge(0.0, 0.0, 400.0, "⚓️", "The Lost Age", None);
        assert!(pending.contains(">?</text>") && !pending.contains(PALE_GOLD));
        assert_eq!(
            attribute("<svg width=\"830\" height=\"12.5\">", "height"),
            Some(12.5)
        );
    }
}
