//! PROGRESS.svg, the one public figure: a Golden Sun menu window holding a
//! gauge for each game's verified DONE above the tracked-files tree. Its tones
//! repeat the dashboard's window colours; it embeds no game font or picture.
use super::boxtree::files_svg;
use super::jsnum::commas;
use super::progress::GameDone;

pub(crate) const WIDTH: f64 = 830.0;
const MARGIN: f64 = 8.0;
const FRAME: f64 = 14.0;
const HEADER: f64 = 188.0;
/// Window tones, as `dashboard/style.css` names them: light, mid, shade and
/// dark rings, the fill ramp and the page behind.
const LIGHT: &str = "#f8f8f8";
const MID: &str = "#a0a8c0";
const SHADE: &str = "#586890";
const DARK: &str = "#101828";
const RAMP_TOP: [&str; 3] = ["#7898d0", "#6888c0", "#5878b0"];
const RAMP_BOTTOM: [&str; 3] = ["#344c7c", "#3c568c", "#44609a"];
const FILL: &str = "#4c6aa4";
const DEEP: &str = "#0c1830";
const GOLD: &str = "#e8b830";
const PALE_GOLD: &str = "#fff3ac";
const FONT: &str = "-apple-system,BlinkMacSystemFont,'Segoe UI',Helvetica,Arial,sans-serif";

/// The figure for both games; a game without a verified count shows `?`.
pub(crate) fn progress_svg(sun: Option<GameDone>, anchor: Option<GameDone>) -> String {
    let tree = files_svg(WIDTH);
    let tree_height = attribute(&tree, "height").unwrap_or(WIDTH * 16.0 / 9.0);
    let inner = WIDTH - 2.0 * (MARGIN + FRAME);
    let scale = inner / WIDTH;
    let height = (2.0 * (MARGIN + FRAME) + HEADER + tree_height * scale).ceil();
    let mut out = format!(
        "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 {WIDTH} {height}\" width=\"{WIDTH}\" height=\"{height}\" shape-rendering=\"crispEdges\" role=\"img\" aria-label=\"Alchemy progress: The Broken Seal {} and The Lost Age {}, above the repository's files\">",
        percent(sun),
        percent(anchor)
    );
    out.push_str(&format!(
        "<rect width=\"{WIDTH}\" height=\"{height}\" fill=\"{DEEP}\"/>"
    ));
    out.push_str(&window(
        MARGIN,
        MARGIN,
        WIDTH - 2.0 * MARGIN,
        height - 2.0 * MARGIN,
    ));
    let left = MARGIN + FRAME + 16.0;
    let right = WIDTH - MARGIN - FRAME - 16.0;
    out.push_str(&text(
        left,
        MARGIN + FRAME + 34.0,
        26.0,
        GOLD,
        "Alchemy",
        "start",
    ));
    out.push_str(&text(
        right,
        MARGIN + FRAME + 34.0,
        15.0,
        LIGHT,
        "Golden Sun decompilation progress",
        "end",
    ));
    for (row, (mark, name, done)) in [
        ("☀️", "The Broken Seal", sun),
        ("⚓️", "The Lost Age", anchor),
    ]
    .into_iter()
    .enumerate()
    {
        let top = MARGIN + FRAME + 58.0 + row as f64 * 60.0;
        out.push_str(&gauge(left, top, right - left, mark, name, done));
    }
    out.push_str(&format!(
        "<g transform=\"translate({} {}) scale({scale})\">{}</g></svg>\n",
        MARGIN + FRAME,
        MARGIN + FRAME + HEADER,
        tree
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
/// Text with the game's one-pixel dark shadow, drawn as a second copy.
fn text(x: f64, y: f64, size: f64, fill: &str, content: &str, anchor: &str) -> String {
    let content = super::boxtree::esc(content);
    format!(
        "<text x=\"{}\" y=\"{}\" font-family=\"{FONT}\" font-size=\"{size}\" font-weight=\"600\" text-anchor=\"{anchor}\" fill=\"{DARK}\">{content}</text><text x=\"{x}\" y=\"{y}\" font-family=\"{FONT}\" font-size=\"{size}\" font-weight=\"600\" text-anchor=\"{anchor}\" fill=\"{fill}\">{content}</text>",
        x + 2.0,
        y + 2.0
    )
}
/// The game's window: mid, light, shade and dark rings lit from the top left,
/// a three-step ramp inside them, then the fill.
fn window(x: f64, y: f64, width: f64, height: f64) -> String {
    let rings = [
        (MID, DARK),
        (LIGHT, SHADE),
        (SHADE, LIGHT),
        (DARK, MID),
        (RAMP_TOP[0], RAMP_BOTTOM[0]),
        (RAMP_TOP[1], RAMP_BOTTOM[1]),
        (RAMP_TOP[2], RAMP_BOTTOM[2]),
    ];
    let mut out = format!(
        "<rect x=\"{x}\" y=\"{y}\" width=\"{width}\" height=\"{height}\" rx=\"6\" fill=\"{FILL}\"/>"
    );
    for (ring, (top_left, bottom_right)) in rings.into_iter().enumerate() {
        let inset = 2.0 * ring as f64 + 1.0;
        let (x0, y0) = (x + inset, y + inset);
        let (x1, y1) = (x + width - inset, y + height - inset);
        out.push_str(&format!(
            "<path d=\"M{x0} {y1}V{y0}H{x1}\" fill=\"none\" stroke=\"{top_left}\" stroke-width=\"2\"/><path d=\"M{x0} {y1}H{x1}V{y0}\" fill=\"none\" stroke=\"{bottom_right}\" stroke-width=\"2\"/>"
        ));
    }
    out
}
/// One game's line: its mark, name and DONE, then a gold gauge whose bright
/// part is exact C and pale part evidenced permanent assembly.
fn gauge(x: f64, y: f64, width: f64, mark: &str, name: &str, done: Option<GameDone>) -> String {
    let mut out = text(x, y + 18.0, 18.0, LIGHT, &format!("{mark} {name}"), "start");
    out.push_str(&text(
        x + width,
        y + 18.0,
        18.0,
        GOLD,
        &percent(done),
        "end",
    ));
    let (bar_y, bar_height) = (y + 28.0, 20.0);
    out.push_str(&format!(
        "<rect x=\"{x}\" y=\"{bar_y}\" width=\"{width}\" height=\"{bar_height}\" rx=\"2\" fill=\"{DARK}\"/><rect x=\"{}\" y=\"{}\" width=\"{}\" height=\"{}\" fill=\"{DEEP}\"/>",
        x + 4.0,
        bar_y + 4.0,
        width - 8.0,
        bar_height - 8.0
    ));
    if let Some(done) = done.filter(|done| done.executable > 0) {
        let span = width - 8.0;
        let share = |bytes: i64| span * bytes as f64 / done.executable as f64;
        let code = share(done.common_c + done.game_c);
        let assembly = share(done.common_asm + done.game_asm);
        out.push_str(&format!(
            "<rect x=\"{}\" y=\"{}\" width=\"{code:.2}\" height=\"{}\" fill=\"{GOLD}\"/><rect x=\"{:.2}\" y=\"{}\" width=\"{assembly:.2}\" height=\"{}\" fill=\"{PALE_GOLD}\"/>",
            x + 4.0,
            bar_y + 4.0,
            bar_height - 8.0,
            x + 4.0 + code,
            bar_y + 4.0,
            bar_height - 8.0
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
        let line = gauge(0.0, 0.0, 408.0, "☀️", "The Broken Seal", Some(done));
        assert!(line.contains("50.00%"));
        assert!(line.contains("width=\"160.00\""));
        assert!(line.contains("width=\"40.00\""));
        let pending = gauge(0.0, 0.0, 408.0, "⚓️", "The Lost Age", None);
        assert!(pending.contains(">?</text>") && !pending.contains(PALE_GOLD));
        assert_eq!(
            attribute("<svg width=\"830\" height=\"12.5\">", "height"),
            Some(12.5)
        );
    }
}
